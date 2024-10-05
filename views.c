#include "views.h"

void endscreen(unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=20;

    current_location = LocationEndscreen;
    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(CENTER(output), 1, output, 128);
    
    strcpy(output, CONGRATULATIONS);
    print_str(CENTER(output), line, output, 128);

    while(!keypressed()) waitForVBlank();

}

void deathscreen(unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=20;

    current_location = LocationDeathscreen;
    print_title(GAME_NAME GAME_OVER);
    while(!keypressed()) waitForVBlank();
    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);
    
    strcpy(output, TRY_IT);
    print_str(CENTER(output), line, output, 128);

    while(!keypressed()) waitForVBlank();
}

void next_level(unsigned char * menu_name, unsigned char level) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char num[10+1];
    unsigned char line=2;
    const unsigned char offset=5;

    displayOff();
    load_font();
    clear_screen();
    displayOn();
    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);
    strcpy(output, TRY_NEXT);
    print_str(CENTER(output), line++, output, 128);

    line = 10;
    strcpy(output, LEVEL); 
    strcat(output, level_names[level-1]);
    print_str(offset, line++, output, 128);
    
    strcpy(output, LEVEL_CODE);
    SEGA_itoa(get_levelcode(level), num);
    strcat(output, num);
    print_str(offset, line++, output, 128);

    line = 20;
    strcpy(output, PRESS_TO_CONT);
    print_str(CENTER(output), line, output, 128);

    while(!keypressed()) waitForVBlank();
}

void level_select(unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    const unsigned char offset=5;
    unsigned char line=5;
    char option;

    current_location = LocationLevelSelect;
    displayOff();
    load_font();
    clear_screen();
    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);
    strcpy(output, LEVEL_CODE);
    print_str(offset, line, output, 128);
    /*
    input(strlen(output), line, output, SCREEN_MAX_X, InputTypeNumerical);
    long code = SEGA_atoi(unsigned char * str) 
    for (option=1; option <= MAX_LEVEL; ++option)
        if (code == get_levelcode(1))
            gameloop(option, 0);
    */
    displayOn();
    option = menu(level_names, MAX_LEVEL+1, 10, 4, MenuModeLeft, 1);
    if (option <= MAX_LEVEL)
        gameloop(option, 0);
}

void credits(const unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=5;

    current_location = LocationCredits;
    displayOff();
    load_font();
    clear_screen();
    displayOn();
    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);

    for (unsigned char entry=0; entry < CREDIT_NAMES_MAX; ++entry) {
        unsigned center = CENTER(strcpy(output, credits_names[entry]));

        for (unsigned char pos=0; pos < strlen(output); ++pos) {
            print_tile(center++, line + entry, 128 + output[pos]);
            for (unsigned char wait=0; (wait < 10) && (!keypressed()); ++wait) 
                waitForVBlank(); 
        }   
    }

    line = 20;
    strcpy(output, PRESS_TO_CONT);
    print_str(CENTER(output), line, output, 128);

    while(!keypressed()) waitForVBlank();
}

void intro(char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line;
    
    do {
        current_location = LocationIntro;
        displayOff();
        load_font();
        clear_screen();

        strcpy(output, menu_name);
        print_str(CENTER(output), TITLE_LINE + 1, output, 0);

        line=2;
        strcpy(output, WRITTEN_BY);
        print_str(CENTER(output), line++, output, 128);
        strcpy(output, IN_YEAR);
        print_str(CENTER(output), line++, output, 128);

        print_img(  city__tiles__bin, city__tiles__bin_size,
                    city__palette__bin, city__palette__bin_size,
                    256, 96, 0, 8); 

        line=20;
        for(unsigned char y=line; y < 24; ++y)
            print_str(0, y, "                                ", 128);
        
        strcpy(output, VERSION);
        print_str(SCREEN_MAX_X - strlen(output), SCREEN_MAX_Y - 1, output, 128);
        displayOn();
        reset_time(1);
        unsigned char option = menu(intro_items, MAX_INTRO_ITEMS, 21, 10, MenuModeCenter, 0);
        switch (option) {
            case 1: {
                gameloop(1, seconds >= DEMO_START_AFTER);
                break;
            }
            case 2: {
                level_select(LEVEL_SELECT);
                break;
            }
            case 3: {
                credits(CREDITS);
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
}
