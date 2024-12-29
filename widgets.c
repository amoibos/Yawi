#include "widgets.h"

unsigned char * decrypt(unsigned char * text) {
    return text;
}

unsigned char menu(unsigned char **items, unsigned char amount, unsigned char start_line, unsigned char offset, 
                    MenuMode mode, _Bool ShowNumbers) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char num[3+1];
    unsigned char option=0;
    signed char previous=-1;
    unsigned char selected = 0;
    _Bool user_choice = 0;
    
    //mapROMBank(BANK_FONT);
    while(!selected) {
        char line = start_line; 
        
        for (unsigned char n=0; n < amount; ++n) {
            if (!((previous == -1) || (n == option) || ((signed int)n == (signed int)previous)))
                continue;    
            
            strcpy(output, "  ");
            if (option == n) {
                output[0] = '>';
                
            }
            if (ShowNumbers)
                strcat(output, "   ");
            strcat(output, items[n]);
            if (option == n)
                to_upper(output);

            if (mode == MenuModeCenter)
                offset =  (SCREEN_MAX_X >> 1) - (strlen(output) >> 1);
            else if (mode == MenuModeLeft)
                ;      

            line = start_line + n;
            print_str(offset, line, output, 128);
            
            if (ShowNumbers) {
                output[0] = 0;
                if (n + 1 < 10)
                    strcat(output, "0");
                       
                print_str(offset + strlen("> "), line, strcat(strcat(output, SEGA_itoa(n + 1, num)), "."), 128);
            }   
        }

        while(!keypressed()) {
            waitForVBlank(); 
            
            if ((current_location == LocationIntro) && (seconds >= DEMO_START_AFTER) && (user_choice == 0)) {
                selected = 1;
                break; 
            }
        }
        
        unsigned int key = readkey();
        if (key != 0)
            user_choice = 1;
        switch (key) {
            case PORT_A_KEY_START: {
                selected = 1;
                break;
            }
            case PORT_A_KEY_UP: {
                waitForVBlank();
                previous = option;
                option = (option <= 0) ? amount - 1 : option - 1;
                break;
            }
            case PORT_A_KEY_DOWN: {
                waitForVBlank();
                previous = option;
                option = (option + 1) >= amount ? 0 : option + 1;
                break;
            }
            default: {
                break;
            }
        }
    }
    return option + 1;
}

unsigned char * input(unsigned char x, unsigned char y, unsigned char * buffer, unsigned char size, InputType input_type) {
    const unsigned char * valid_chars;

    //mapROMBank(BANK_FONT);
    switch(input_type) {
        case InputTypeNumerical: {
            valid_chars = NUMERIC;
            break;
        }
        case InputTypeAlphaNumerical: {
            valid_chars = ALPHA_NUMERIC;
            break;
        }
        default: {
            valid_chars = ALPHA_NUMERIC;
            break;
        }
    }

    unsigned char set_size = strlen(valid_chars);
    // garantee null character at the end
    for (unsigned char c=0; c < size; ++c)
        buffer[c] = 0;
    
    signed char pos = 0;
    unsigned char idx = 0;
    unsigned int key;
    do {
        while (!keypressed) ;
        //slow down key processing
        for (unsigned char i=0; i < 5; ++i)
            waitForVBlank();
        key = readkey();
        switch(key) {
           case PORT_A_KEY_2: {
                pos = (pos + 1) % set_size;
                buffer[idx] = valid_chars[pos];
                break;
            }
            case PORT_A_KEY_UP: {
                pos -= 10;
                if (pos < 0)
                    pos += set_size;
                
                buffer[idx] = valid_chars[pos];
                break;
            }
            case PORT_A_KEY_DOWN: {
                pos = (pos + 10) % set_size;
                buffer[idx] = valid_chars[pos];
                break;
            }
            case PORT_A_KEY_LEFT: {
                if (idx > 0) {
                    //show changed output on screen
                    buffer[idx] = ' ';
                    print_str(x, y, buffer, 128);
                    buffer[idx--] = 0;  
                }
                break;
            }
            case PORT_A_KEY_RIGHT: {
                if (idx < size - 1) {
                    ++idx;
                    pos = 1;
                    buffer[idx] = valid_chars[pos];
                }
                break;
            }
            default: {
                break;
            }
        }
        
        print_str(x, y, buffer, 128);
    } while (key != (PORT_A_KEY_1 | PORT_A_KEY_2));

    return buffer;
}

/*
void print_img( const unsigned char *tiledata, unsigned int tile_length,
                const unsigned char *colordata, unsigned int color_length,
                const unsigned int width, const unsigned int height, const unsigned char left, const unsigned char top) {
    const unsigned int start_img_tiles = 256;

    //mapROMBank(BANK_GFX);
    loadTiles(tiledata, start_img_tiles, tile_length);
    loadPalette(colordata, start_img_tiles, color_length);

    unsigned int tileno = 0;
    for (unsigned char y=top; y < top + (height >> 3); ++y) {
        for(unsigned char x=left; x < left + (width >> 3); ++x) {
            print_tile(x, y, tileno + start_img_tiles);
            ++tileno;
        }
    }
}*/


void print_img_compressed( const unsigned char *tiledata, unsigned int tile_length,
                const unsigned char *colordata, unsigned int color_length,
                const unsigned int width, const unsigned int height, const unsigned char left, const unsigned char top) {
    const unsigned int start_img_tiles = 256;

    //mapROMBank(BANK_GFX);
    //loadTiles(tiledata, start_img_tiles, tile_length);
    loadZX7compressedTiles(tiledata, start_img_tiles);
    loadPalette(colordata, start_img_tiles, color_length);

    unsigned int tileno = 0;
    for (unsigned char y=top; y < top + (height >> 3); ++y) {
        for(unsigned char x=left; x < left + (width >> 3); ++x) {
            print_tile(x, y, tileno + start_img_tiles);
            ++tileno;
        }
    }
}