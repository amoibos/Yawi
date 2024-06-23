#include "views.h"

void endscreen(unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=20;

    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, 1, output, 128);
    
    strcpy(output, "Congrats");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line, output, 128);

    while(!keypressed()) waitForVBlank();

}

void deathscreen(unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=20;

    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, TITLE_LINE + 1, output, 128);
    
    strcpy(output, "Try it again");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line, output, 128);

    while(!keypressed()) waitForVBlank();
}

void next_level(unsigned char * menu_name, unsigned char level) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char num[10+1];
    unsigned char line=2;
    const unsigned char offset=5;

    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, TITLE_LINE + 1, output, 128);
    strcpy(output, "try the next level");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line++, output, 128);

    line = 10;
    strcpy(output, "Level: ");
    strcat(output, level_names[level]);
    print_str(offset, line++, output, 128);
    
    strcpy(output, "Level code: ");
    num[0] = 0;
    SEGA_itoa(get_levelcode(level), num);
    strcat(output, num);
    print_str(offset, line++, output, 128);

    line = 20;
    strcpy(output, "Press a key to continue");
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line, output, 128);

    while(!keypressed()) waitForVBlank();
}

void level_select(unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    
    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2 , TITLE_LINE + 1, output, 128);

    char option = menu(level_names, MAX_LEVEL, 10, 4, MenuModeLeft, 1);
    gameloop(option);
}

void credits(const unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=5;

    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, TITLE_LINE + 1, output, 128);

    for (unsigned char entry=0; entry < CREDIT_NAMES_MAX; ++entry) {
        strcpy(output, credits_names[entry]);
        unsigned center = SCREEN_MAX_X / 2 - strlen(output) / 2;

        for (unsigned char pos=0; pos < strlen(output); ++pos) {
            print_tile(center++, line + entry, 128 + output[pos]);
            for (unsigned char wait=0; (wait < 10) && (!keypressed()); ++wait) waitForVBlank(); 
        }   
    }

    while(!keypressed()) waitForVBlank();
}

void intro(char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line;
    
    do {
        load_font();
        clear_screen();

        
        strcpy(output, menu_name);
        print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, TITLE_LINE + 1, output, 0);

        line=2;
        strcpy(output, "Written by Darktrym");
        print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line++, output, 128);
        strcpy(output, "in 2024");
        print_str(SCREEN_MAX_X / 2 - strlen(output) / 2, line++, output, 128);

        print_img(  city__tiles__bin, city__tiles__bin_size,
                    city__palette__bin, city__palette__bin_size,
                    256, 96, 0, 8); 
        for(char y=20; y < 24; ++y)
            print_str(0, y, "                                ", 128);
        

        strcpy(output, VERSION);
        print_str(SCREEN_MAX_X - strlen(output), SCREEN_MAX_Y - 1, output, 128);

        char option = menu(intro_items, MAX_INTRO_ITEMS, 21, 10, MenuModeCenter, 0);
        switch (option) {
            case 1: {
                gameloop(1);
                break;
            }
            case 2: {
                level_select("Level Select");
                break;
            }
            case 3: {
                credits("Credits");
                break;
            }
            default: {
                break;
            }
        }

            
     } while (1);
}

void print_img( const unsigned char *tiledata, unsigned int tile_length, 
                const unsigned char *colordata, unsigned int color_length, 
                const unsigned int width, const unsigned int height, const unsigned char left, const unsigned char top) {
    const unsigned int start_img_tiles = 256; 

    loadTiles(tiledata, start_img_tiles, tile_length);
    loadPalette(colordata, start_img_tiles, color_length); 
    
    int tileno = 0;
    for (unsigned char y=top; y < top + height / 8; ++y) {
        for(unsigned char x=left; x < left + width / 8; ++x) {
            print_tile(x, y, tileno + start_img_tiles);
            ++tileno;
        }
    }
}
