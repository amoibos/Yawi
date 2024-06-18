#include "views.h"

void endscreen(char * menu_name) {
    char output[30+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 0, output);

    print_str(0, 0, "Congrats");
}

void deathscreen(char * menu_name) {
    char output[30+1];

    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 0, output);

    print_str(0, 0, "Try it again");
}


void level_select(char * menu_name) {
    char output[30+1];
    char num[3+1];
    char option = 0;
    _Bool selected = 0;

    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 0, output);

    //char menu_options = 2;
    while(1) {
        char line = 3; 
        for (char n=0; n < MAX_LEVEL; ++n) {
            output[0] = 0;
            strcat(output, "  ");
            SEGA_itoa(n+1, num);
            if (n+1 < 10)
                strcat(output, "0");    
            
            strcat(output, num);
            strcat(output, ". ");
            strcat(output, level_names[n]);
            if (option == n) 
                output[0] = '>';
          
            print_str(3, line++, output);    
        }

        while(!keypressed());
        switch (readkey()) {
            case PORT_A_KEY_START: {
                selected = 1;
                break;
            }
            case PORT_A_KEY_UP: {
                option = (option == 0) ? MAX_LEVEL - 1 : option - 1;    
                break;
            }
            case PORT_A_KEY_DOWN: {
                option = (option + 1) % MAX_LEVEL;
                break;
            }
            default:    {
                break;
            }
        }
        if (selected)
            break;
    }
    gameloop(option + 1);
}

void intro(const char * menu_name) {
    char output[30+1];
    char line=4;
    
    clear_screen();
    strcpy(output, menu_name);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, 2, output);

    line+=2;
    strcpy(output, "Written by Darktrym");
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, line++, output);
    strcpy(output, "in 2024");
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2,  line++, output);
    strcpy(output, "game design based on Enigma");
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2,  line++, output);
    strcpy(output, VERSION);
    print_str(-strlen(output) / 2 + SCREEN_MAX_X / 2, line++, output);

    char menu_options = 2;
    char option = 0;
    _Bool selected = 0;
    while(1) {

        strcpy(output, "  1 New Game");
        if (option == 0) {
            output[0] = '>';
        }  
        print_str(SCREEN_MAX_X / 4, 18, output);
        
        strcpy(output, "  2 Level Select");
        if (option == 1) {
            output[0] = '>';
        }
        print_str(SCREEN_MAX_X / 4, 19, output);
        
        while(!keypressed());
        switch (readkey()) {
            case PORT_A_KEY_START: {
                selected = 1;
                break;
            }
            case PORT_A_KEY_UP: {
                option = (option == 0) ? menu_options - 1 : 0;    
                break;
            }
            case PORT_A_KEY_DOWN: {
                option = (option + 1) % menu_options;
                break;
            }
            default:    {
                break;
            }
        }
        if (selected)
            break;
    }
    if (option == 1)
        level_select("Level Select");
    else
        gameloop(1);
}