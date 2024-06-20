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
    
//#ifdef DEMO
//    strcpy(output, DEMO);
//#else
    strcpy(output, VERSION);
//#endif
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
