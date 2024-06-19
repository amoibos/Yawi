#include "views.h"

void endscreen(char * menu_name) {
    char output[SCREEN_MAX_X+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, 1, output, 128);

    print_str(0, 0, "Congrats", 128);
}

void deathscreen(char * menu_name) {
    char output[SCREEN_MAX_X+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, 1, output, 128);

    print_str(0, 0, "Try it again", 128);
}

void next_level(char * menu_name, char level) {
    char output[SCREEN_MAX_X+1];
    char num[10+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, 1, output, 128);

    strcpy(output, "Password code: ");
    
    num[0] = 0;
    SEGA_itoa(levelcode(level), num);

    strcat(output, num);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, 1, output, 128);

    strcpy(output, "Press a key to continue");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, 1, output, 128);

    while(!keypressed()) waitForVBlank();
}

char menu(char **items, char amount, char start_line, char offset, MenuMode mode, _Bool numbers) {
    char output[SCREEN_MAX_X+1];
    char num[3+1];
    char option = 0;
    
    while(1) {
        char line = start_line; 
        for (char n=0; n < amount; ++n) {
            
            
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

            print_str(offset, line++, output, 128);
            if (numbers) {
                output[0] = 0;
                if (n + 1 < 10)
                    strcat(output, "0");
                SEGA_itoa(n + 1, num);        
                
                strcat(output, num);
                strcat(output, ".");
                print_str(offset + strlen("> "), line-1, output, 0);
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
                option = (option <= 0) ? amount - 1 : option - 1;
                break;
            }
            case PORT_A_KEY_DOWN: {
                waitForVBlank();
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

void level_select(char * menu_name) {
    char output[SCREEN_MAX_X+1];
    
    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2 , 1, output, 128);

    char option = menu(level_names, MAX_LEVEL, 10, 4, MenuModeLeft, 1);
    gameloop(option);
}

void intro(char * menu_name) {
    char output[SCREEN_MAX_X+1];
    char line=1;
    long time = 3000;
    unsigned char color=0; 

    load_font();
    clear_screen();

    while(--time)
        SG_setBackdropColor(color = (color + 1) % 16);
    SG_setBackdropColor(0);

    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line++, output, 128);

    strcpy(output, "Written by Darktrym");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line++, output, 128);
    strcpy(output, "in 2024");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line++, output, 128);

    print_img(  city__tiles__bin, city__tiles__bin_size,
                city__palette__bin, city__palette__bin_size,
                256, 96, 0, 8); 
    for(char line=20; line < 24; ++line)
        print_str(0, line, "                                ", 128);
    
    strcpy(output, VERSION);
    print_str(SCREEN_MAX_X - strlen(output), SCREEN_MAX_Y - 1, output, 128);

    char option = menu(intro_items, MAX_INTRO_ITEMS, 21, 10, MenuModeCenter, 0);
    if (option == 2)
        level_select("Level Select");
    else
        gameloop(1);
}

void print_img( const char *tiledata, unsigned int tile_length, 
                const char *colordata, unsigned int color_length, 
                int width, int height, char left, char top) {
    const int start_img_tiles = 256; 

    loadTiles(tiledata, start_img_tiles, tile_length);
    loadPalette(colordata, start_img_tiles, color_length); 
    
    int tileno = 0;
    for (char y=top; y < top + height / 8; ++y) {
        for(char x=left; x < left + width / 8; ++x) {
            print_tile(x, y, tileno + start_img_tiles);
            ++tileno;
        }
    }
}
