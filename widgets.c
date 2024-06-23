#include "widgets.h"

unsigned char menu(unsigned char **items, unsigned char amount, unsigned char start_line, unsigned char offset, MenuMode mode, _Bool numbers) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char num[3+1];
    unsigned char option = 0;
    signed char previous=-1;
    
    while(1) {
        char line = start_line; 
        
        for (unsigned char n=0; n < amount; ++n) {
            if (!((previous == -1) || (n == option) || ((signed int)n == (signed int)previous)))
                continue;    
            
            strcpy(output, "  ");
            if (option == n) {
                output[0] = '>';
                
            }
            if (numbers)
                strcat(output, "   ");
            strcat(output, items[n]);
            if (option == n)
                to_upper(output);

            if (mode == MenuModeCenter)
                offset =  SCREEN_MAX_X / 2 - strlen(output) / 2;
            else if (mode == MenuModeLeft)
                ;      

            line = start_line + n;
            print_str(offset, line, output, 128);
            
            if (numbers) {
                output[0] = 0;
                if (n + 1 < 10)
                    strcat(output, "0");
                SEGA_itoa(n + 1, num);        
                
                strcat(output, num);
                strcat(output, ".");
                print_str(offset + strlen("> "), line, output, 0);
            }   
        }

        _Bool selected = 0;
        while(!keypressed()) waitForVBlank();
        unsigned int key = readkey();
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
        if (selected)
            break;
    }
    return option + 1;
}

unsigned char * input(unsigned char x, unsigned char y, unsigned char * buffer, unsigned char size, InputType input_type) {
    const unsigned char * valid_chars;

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