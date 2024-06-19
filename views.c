#include "views.h"

void endscreen(char * menu_name) {
    char output[30+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 0, output, 128);

    print_str(0, 0, "Congrats", 128);
}

void deathscreen(char * menu_name) {
    char output[30+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 0, output, 128);

    print_str(0, 0, "Try it again", 128);
}

char menu(char **items, char amount, char start_line, MenuMode mode, _Bool numbers, signed char textcolor) {
    char output[SCREEN_MAX_X+1];
    char num[3+1];
    char option = 0;
    unsigned char color = 0;
    
    while(1) {
        if (textcolor != -1) {
            color = (color + 1) % 16;
            if ((textcolor != color))
                SG_setBackdropColor(color);
        }
        char line = start_line; 
        for (char n=0; n < amount; ++n) {
            output[0] = 0;
            strcat(output, "  ");
            if (numbers) {
                SEGA_itoa(n+1, num);
                if (n+1 < 10)
                    strcat(output, "0");    
                
                strcat(output, num);
                strcat(output, ". ");
            }
            strcat(output, items[n]);
            if (option == n) 
                output[0] = '>';

            char offset = 0;
            if (mode == MenuModeCenter)
                offset = -strlen(output) / 2 + SCREEN_MAX_X / 2;
            else if (mode == MenuModeLeft)
                offset = 3;          
            print_str(offset, line++, output, 128);    
        }

        _Bool selected = 0;
        while(!keypressed());
        switch (readkey()) {
            case PORT_A_KEY_START: {
                selected = 1;
                break;
            }
            case PORT_A_KEY_UP: {
                option = (option <= 0) ? amount - 1 : option - 1;
                break;
            }
            case PORT_A_KEY_DOWN: {
                option = (option+1 >= amount) ? 0 : option + 1;
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
    char output[30+1];
    
    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 0, output, 128);

    char option = menu(level_names, MAX_LEVEL, 3, MenuModeLeft, 1, -1);
    gameloop(option);
}

void intro(const char * menu_name) {
    char output[30+1];
    char line=4;
    
    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 2, output, 128);

    line+=2;
    strcpy(output, "Written by Darktrym");
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, line++, output, 128);
    strcpy(output, "in 2024");
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2,  line++, output, 128);
    strcpy(output, VERSION);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, line++, output, 128);

    char option = menu(intro_items, MAX_INTRO_ITEMS, 18, MenuModeCenter, 0, -1);
    if (option == 2)
        level_select("Level Select");
    else
        gameloop(1);
}