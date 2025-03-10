#include "widgets.h"

unsigned char * decrypt(unsigned char * text) {
    //TODO: to implement 
    return text;
}

void (*timer_callback)(Screens);

void SetTimerCallback (void (*theHandlerFunction)(Screens)) __z88dk_fastcall {
    timer_callback=theHandlerFunction;
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
            
            if ((current_location == LocationIntro) && (seconds >= DEMO_START_AFTER_S) && (user_choice == 0)) {
                selected = 1;
                break; 
            } else if (current_location == LocationIntro) {
                if (animation_refresh && timer_callback) {
                    timer_callback(current_location);
                }
            }
        }
        
        unsigned short key = readkey();
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
    // garantee null character at the end, length is size + 1
    for (unsigned char c=0; c <= size; ++c)
        buffer[c] = 0;
    
    signed char pos = 0;
    unsigned char idx = 0;
    unsigned short key;
    do {
        while (!keypressed()) ;
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
        while (keypressed()) ;
    } while (key != (PORT_A_KEY_1 | PORT_A_KEY_2));

    return buffer;
}

void print_img_compressed( const unsigned char *tiledata,
                const unsigned char *colordata, 
                const unsigned short width, const unsigned char height, const unsigned char left, const unsigned char top, Effect effect) {
    const unsigned short start_img_tiles = 256;

    //mapROMBank(BANK_GFX);
    loadZX7compressedTiles(tiledata, start_img_tiles);
    loadZX7compressedBGColors(colordata, start_img_tiles);

    unsigned char height8=height>>3, width8=width>>3;
    unsigned short tileno = 0;
    unsigned char aborted = 0;        
    if (effect == EffectSpiral) {
        displayOn();
        signed char center_x, center_y;
        signed char x=0, y=0, x_off=width8>>1, y_off=height8>>1, dx=0, dy=-1, temp;
        unsigned short longside_squared = (width8 < height8) ? height8 * height8 : width8 * width8;
        for(unsigned short i=0; i < longside_squared; ++i) {
            if ((x >= -x_off) && (x <= x_off) && (y >= -y_off) && (y <= y_off)) {
                center_x = x + x_off, center_y = y + y_off;
                tileno = center_y * width8 + center_x + start_img_tiles;
                if ((center_y + top < top + height8) && (center_y >= 0))
                    print_tile(center_x + left, center_y + top, tileno);  
                if (!aborted) wait(1);
                if (keypressed())
                    aborted = 1;
            } 
            if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
                temp = dx;
                dx = -dy;
                dy = temp;    
            }

            x += dx;
            y += dy;   
        }
    } else { //linear
        for (unsigned char y=top; y < top + height8; ++y) {
            for(unsigned char x=left; x < left + width8; ++x) {
                print_tile(x, y, tileno + start_img_tiles);
                ++tileno;
            }
        }
    }
}

void print_window_borders(unsigned char left, unsigned char top, unsigned char width, unsigned char height, unsigned short tileno) {

   for (unsigned char x=left; x < left + width; ++x) {
        if (x >= SCREEN_MAX_X)
            continue;

        print_tile(x, top, tileno);
        print_tile(x, top + height - 1, tileno);
    }
    for (unsigned char y=top; y < top + height; ++y) {
        if (y >= SCREEN_MAX_Y)
            continue;

        print_tile(left, y, tileno);
        print_tile(left + width - 1, y, tileno);
    }

}