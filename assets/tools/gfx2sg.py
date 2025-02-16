#!/usr/bin/env python
# coding: utf-8



# for zx7 implemenation taken from c source
'''
 * (c) Copyright 2012-2016 by Einar Saukas. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of its author may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 '''
# for the rest license BSD-3 clause

import os
from os import path
import sys
import struct
import argparse
from PIL import Image
from collections import defaultdict
from dataclasses import dataclass
#import numpy as np
#import cProfile
from functools import cache

__VERSION__ = "0.8.2"

# 32 x 24 tiles filling a screen where a tile 8x8 tile dimension
# for the SG the color depth is 1bit = 2 colors per tile line
PAL_COLORS = 1  # in bits
MAX_X = 256
MAX_Y = 192
# transparence has no color thats why -1
MAX_COLORS = 16 - 1

TILE_WIDTH = 8
TILE_HEIGHT = 8

MAX_OFFSET = 2176
MAX_LEN = 256**2

TILE_EXT = ".bin"
PALETTE_EXT = ".bin"

# first index of color palette is in the VDP TMS9918 always transparent
SG_COLOR_PALETTE = ((0xFFFF, 0xFFFF, 0xFFFF), (0x00, 0x00, 0x00), (0x21, 0xC8, 0x42), (0x5E, 0xDC, 0x78),
                    (0x54, 0x55, 0xED), (0x7D, 0x76, 0xFC), (0xD4, 0x52, 0x4D), (0x42, 0xEB, 0xF5),
                    (0xFC, 0x55, 0x54), (0xFF, 0x79, 0x78), (0xD4, 0xC1, 0x54), (0xE6, 0xCE, 0x80),
                    (0x21, 0xB0, 0x3B), (0xC9, 0x5B, 0xBA), (0xCC, 0xCC, 0xCC), (0xFF, 0xFF, 0xFF))


@dataclass
class State:
    output_data: list
    bitmask: int = 0
    bit_index: int = 0
    output_index: int = 0

@dataclass
class Optimal:
    bits: int = 0
    offset: int = 0
    len: int  = 0

def write_byte(state, byte):
    state.output_data[state.output_index] = byte
    state.output_index += 1

def write_bit(state, bit):
    if state.bitmask == 0:
        state.bitmask = 0x80
        state.bit_index = state.output_index
        write_byte(state, 0)
    if bit > 0:
        state.output_data[state.bit_index] |= state.bitmask

    state.bitmask >>= 1

def encode_sequence_length(state, value):
    i = 2

    while i <= value:
        write_bit(state, 0)
        i <<= 1

    while (i := i >> 1) > 0:
        write_bit(state, value & i)

def compress_data(input_data, optimal):
    # first character always uncompressed as literal
    state = State([], 0, 0, 0)

    input_size = len(input_data)
    input_index = input_size - 1
    output_size = (optimal[input_index].bits + 18 + 7) // 8
    state.output_data = [0] * output_size

    # un-reverse optimal sequence
    optimal[input_index].bits = 0
    while input_index != 0:
        input_prev =  input_index - (optimal[input_index].len if optimal[input_index].len > 0 else 1)
        optimal[input_prev].bits = input_index
        input_index = input_prev

    # first byte is always literal
    write_byte(state, input_data[input_index])

    while (input_index := optimal[input_index].bits) > 0:

        if optimal[input_index].len  == 0:
            # literal indicator
            write_bit(state, 0)

            # literal value
            write_byte(state, input_data[input_index])
        else:
            # sequence indicator
            write_bit(state, 1)

            # sequence length
            encode_sequence_length(state, optimal[input_index].len - 1)

            offset1 = optimal[input_index].offset - 1

            # sequence offset
            if offset1 < 0x80:
                write_byte(state, offset1)
            else:
                offset1 -= 0x80
                write_byte(state, (offset1 & 0x7F) | 0x80)
                mask = 1024
                while mask > 0x7F:
                    write_bit(state, offset1 & mask)
                    mask >>= 1

    # sequence indicator
    write_bit(state, 1)

    # end marker
    for _ in range(16):
        write_bit(state, 0)
    write_bit(state, 1);

    return state.output_data

@cache
def elias_gamma_bits(value):
    bits = 1

    while value > 1:
        bits += 2
        value >>= 1

    return bits

@cache
def count_bits(offset, length):
    return 1 + (12 if offset > 128 else 8) + elias_gamma_bits(length - 1)

def optimize(input_data):
    min = [0] * (MAX_OFFSET + 1)
    max = [0] * (MAX_OFFSET + 1)
    matches = [0] * 256**2
    match_slots = [0] * len(input_data)
    optimal = [Optimal(0, 0, 0) for _ in input_data]

    input_size = len(input_data)
    skip = 0
    i = 1

    optimal[skip].bits = 8
    for i in range(skip + 1, input_size):
        optimal[i].bits = optimal[i - 1].bits + 9
        match_index = (input_data[i - 1] << 8) | input_data[i]
        best_length = 1

        match = matches[match_index]
        while (match != 0) and (best_length < MAX_LEN):
            offset = i - match
            if offset > MAX_OFFSET:
                match = 0
                break

            length = 2
            while (length <= MAX_LEN) and (i >= skip + length):
                if length > best_length:
                    best_len = length
                    bits = optimal[i - length].bits + count_bits(offset, length)
                    if optimal[i].bits > bits:
                        optimal[i].bits = bits
                        optimal[i].offset = offset
                        optimal[i].len = length
                elif (max[offset] != 0) and ((i + 1 == max[offset]) + length):
                    length = i - min[offset]
                    if length > best_length:
                        length = best_length

                if (i < offset + length) or (input_data[i - length] != input_data[i - length - offset]):
                    break
                length += 1

            #print("DEBUG " + str(offset))
            min[offset] = i + 1 - length
            max[offset] = i

            match = match_slots[match]

        match_slots[i] = matches[match_index]
        matches[match_index] = i

    #breakpoint()
    return optimal

def check_color_limit(img, limit):
    width, height = img.size
    for tile_y in range(height // TILE_HEIGHT):
        for tile_x in range(width // TILE_WIDTH):
            region = img.crop((tile_x * TILE_WIDTH, tile_y * TILE_HEIGHT, (tile_x + 1) * TILE_WIDTH,
                                       (tile_y + 1) * TILE_HEIGHT))
            #There is no check if there are more than 2 color per tile row!
            #In this case, the 2 colors that occur most frequently are used
            if len(region.getcolors()) > limit:
                return False
    return True

@cache
def nearest_color(subjects, query):
    return min(subjects, key=lambda subject: sum((s - q) ** 2 for s, q in zip(subject, query)))

def convert(output_name, options):
    transparent_color, preview, warn, compress = options["transparent_color"], options["preview"], options["warn"], options["compress"]
    #print(f"params: {output_name} {transparent_color} {preview} {warn}")
    with Image.open(output_name) as img:
        if preview:
            preview_img = img.copy()
        width, height = img.size
        #import pdb; pdb.set_trace()
        color_cnt = len(img.getcolors(width * height))

        # do not use transparency color
        if not check_color_limit(img, MAX_COLORS):
            print(f"too many colors in one tile, platform supports only {MAX_COLORS} colors per tile", file=sys.stderr)
            return

        # no scaling supported
        if width > MAX_X or height > MAX_Y:
            print(f"invalid image dimensions, platform supports maximal resolution {MAX_X}x{MAX_Y}", file=sys.stderr)
            return

        # convert single band color representation to RGB
        if "".join(img.getbands()) != "RGB":
            img = img.convert('RGB')

        # store color palette information
        color_map = {}
        # translate color information to corresponding index of color palette
        color_index = {(0, 0, 0): 0}

        for idx, color in enumerate(img.getcolors()):
            #import pdb; pdb.set_trace()
            index = SG_COLOR_PALETTE.index(nearest_color(SG_COLOR_PALETTE, color[-1]))
            # do not use transparent as black, overwrite transparent palette index
            if index == 0:
                index = 1
                print(f"remove transparence: replaced color {color[-1]} from palette index 0 with 1")
            # allow specifying transparent palette index, useful for sprites
            # background tiles could have transparence which leads to backdrop color
            if index == transparent_color:
                index = 0
                print(f"transparence correction: replaced color {color[-1]} from palette index {transparent_color} with 0")

            color_index[color[-1]] = index

        filename = path.splitext(output_name)[0]
        # write tiles data
        palette_container = []
        tile_container = []
        with open(f"{filename} (tiles){TILE_EXT}" , "wb") as tile_writer, open(f"{filename} (palette){PALETTE_EXT}", "wb") as palette_writer:
            print(f"processing {output_name}..")
            used_tile = defaultdict(int)
            index = 0
            for tile_y in range(height // TILE_HEIGHT):
                for tile_x in range(width // TILE_WIDTH):
                    region = img.crop((tile_x * TILE_WIDTH, tile_y * TILE_HEIGHT, (tile_x + 1) * TILE_WIDTH,
                                       (tile_y + 1) * TILE_HEIGHT))
                    # no idea why I have to mirror the tile
                    region = region.transpose(Image.FLIP_LEFT_RIGHT)
                    data = [color_index[item] for item in region.getdata()]
                    colors = []
                    for pos in range(0, len(data), TILE_HEIGHT):
                        colors_in_line = defaultdict(int)
                        # get color distribution in line of the current tile
                        for column in range(TILE_WIDTH):
                            colors_in_line[data[pos + column]] += 1
                        # color with the highest amount in line is background color,
                        # second place is foreground color
                        # if there is no second place then use transparent color

                        # warn if were lose color information due tile line limation of TMS9918
                        if len(colors_in_line) > 2 and warn:
                            print(f"[info]: color clash({len(colors_in_line)} > 2) detected in line {pos} at tile index x={tile_x} y={tile_y}")

                        # use color index for ordering
                        # when computed 2 sprite colors per line, reuse it, otherwise we see problems in encoding of sprites
                        if not transparent_color or not colors or len(list(dict.fromkeys(data))) > 2:
                            colors = sorted(colors_in_line.items(), key=lambda x: x[0] if transparent_color else x[-1])

                        #if tile_x == 9 and tile_y == 0:
                        #    import pdb; pdb.set_trace()

                        # we need a second color when there line is complete transparent and background color when there is filled by another color
                        if transparent_color and len(colors) == 1:
                            missing_colors = sorted(list(dict.fromkeys(data)))
                            if colors[0][0] in missing_colors:
                                missing_colors.remove(colors[0][0])
                            if len(missing_colors) >= 1:
                                colors.insert(1 if colors[0][0] == 0 else 0, (missing_colors[0], 0))

                        if len(colors) < 2:
                            if colors[0][0] == 0:
                                colors.insert(1, (1, 0))
                            else:
                                colors.insert(0, (1, 0))
                        foreground = colors[-2][0]
                        background = colors[-1][0]

                        #if tile_x == 7 and tile_y == 0:
                        #    import pdb; pdb.set_trace()

                        val = 0
                        for column in range(TILE_WIDTH):
                            val += (int(data[pos + column] == background)) <<  column
                            if preview:
                                preview_img.putpixel((tile_x * TILE_WIDTH + 7 - column, tile_y * TILE_HEIGHT + pos // 8),
                                    SG_COLOR_PALETTE[background if int(data[pos + column] == background) else foreground])

                        tile_data = val
                        tile_container.append(tile_data)
                        color_data = (background << 4) + foreground

                        palette_container.append(color_data)
                    if transparent_color and len(dict.fromkeys(data)) > 2:
                        print(f"tile x={tile_x:02} y={tile_y:02} has {len(dict.fromkeys(data)):02} colors, not suitable for sprites")


                    #import pdb; pdb.set_trace()
                    key = tuple(data)
                    if key in used_tile:
                        if warn:
                            print(f"[info]: duplicate tile {key} detected at index {index}, last seen at {used_tile[key]}")
                    used_tile[key] = index
                    index += 1

            data = palette_container if not compress else compress_data(palette_container, optimize(palette_container))
            if compress:
                print(f"palette data: {len(palette_container)} bytes, compressed: {len(data)} bytes, compression ratio: {len(data) / len(palette_container) * 100:.2f}%")
            for color_data in data:
                palette_writer.write(struct.pack('B', color_data))

            #breakpoint()
            data = tile_container if not compress else compress_data(tile_container, optimize(tile_container))
            if compress:
                print(f"tiles data: {len(tile_container)} bytes, compressed: {len(data)} bytes, compression ratio: {len(data) / len(tile_container) * 100:.2f}%")

            for tile_data in data:
                tile_writer.write(struct.pack('B', tile_data))
        if preview:
            preview_img.show()

def process(filename, options):
    convert(filename, options)

def main():
    print(f"gfx2sg v{__VERSION__}")
    parser = argparse.ArgumentParser()
    parser.add_argument("filename", help="image file", type=str)
    parser.add_argument("--transparent", help="index of color used used for transparency in sprites", type=int)
    parser.add_argument("--preview", help="switch for show image after color conversion",  action="store_true")
    parser.add_argument("--warn", help="show warnings",  action="store_true")
    parser.add_argument("--compress", help="compress ZX7",  action="store_true")
    args = parser.parse_args()
    if  args. transparent:
        if args.transparent < 1 or args.transparent > MAX_COLORS:
            print('color index out of range', file=sys.stderr)
            exit(-1)


    transparent_color = args.transparent if args.transparent else None
    options = {"transparent_color": transparent_color, "preview": args.preview, "warn": args.warn, "compress": args.compress}
    if path.exists(args.filename):
        process(args.filename, options)
    else:
        print("file %s doesn't exist" % (sys.argv[1]), file=sys.stderr)
        exit(-2)

if __name__ == '__main__':
    #cProfile.run('main()')
    main()
