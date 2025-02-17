#include "views.h"
#include "libs/strings.h"
#include "localization.h"

void congratulation_screen(const unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char num[5+1];
    unsigned char line=1;

    current_location = LocationEndscreen;
    load_font();
    clear_screen();
    reset_sprites();
    strcpy(output, menu_name);
    print_str(CENTER(output), line, output, 128);

    line = 3;
    output[0] = '\0';
    SEGA_itoa(totaltime, num);
    strcat(strcat(strcat(output, PLAY_TIME), num), "s");
    print_str(0, line++, output, 128);

    output[0] = '\0';
    strcat(output, VERIFICATION);
    print_str(0, line++, output, 128);


    if (audio_enabled) {
        PSGPlay(forelise_psg);
    }

    //add blinking continue text
    line = 20;
    strcpy(output, PRESS_TO_CONT);
    for (unsigned char pos=0; pos < strlen(output); ++pos) {
        add_animation(CENTER(output) + pos, line);
    }

    init_sprite_position(0);
    add_ball_sprite();

    // animation loop
    while(!keypressed()) {

        if (animation_refresh) {
            animate_quarterly(ScreenCongratulation);
            update_sprites_falling();
        }
        waitForVBlank();
    }

    if (audio_enabled) {
        PSGStop();
        PSGSFXStop();
    }
    clear_sprites();
}

void death_screen(const unsigned char *menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=20;

    current_location = LocationDeathscreen;
    load_font();
    clear_screen();
    reset_sprites();
    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);

    //add bat
    print_tile(CENTER(output) + strlen(output) + 2, TITLE_LINE + 1, 3);
    add_animation(CENTER(output) + strlen(output) + 2, TITLE_LINE + 1);

    strcpy(output, TRY_IT);
    print_str(CENTER(output), line, output, 128);

    print_img_compressed(  cemetry__tiles__bin,
                cemetry__palette__bin,
                SCREEN_MAX_RES_X, 96, 0, 8, EffectNone);

    for(unsigned char y=line; y < SCREEN_MAX_Y; ++y)
        print_str(0, y, "                                ", 128);

    print_str(CENTER(output), line + 2, output, 128);

    //animation loop and waiting for key pressed
    while(!keypressed()) {
        if (animation_refresh)
            animate_quarterly(ScreenDeath);
        waitForVBlank();
    }
}

void next_level_screen(const unsigned char * menu_name, unsigned char level) {
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
    strcat(output, (unsigned char *)level_names[level-1]);
    print_str(offset, line++, output, 128);

    strcpy(output, LEVEL_CODE);
    SEGA_itoa(get_levelcode(level), num);
    strcat(output, num);
    print_str(offset, line++, output, 128);


    line = 16;
    output[0] = '\0';
    SEGA_itoa(totaltime, num);
    strcat(strcat(strcat(output, PLAY_TIME), num), "s");
    print_str(0, line++, output, 128);

    line = 20;
    strcpy(output, PRESS_TO_CONT);
    print_str(CENTER(output), line, output, 128);

    while(!keypressed()) {
        PSGFrame();
        waitForVBlank();
    }
}

void level_select_screen(const unsigned char * menu_name) {
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
    displayOn();
    
    input(strlen(output) + offset + 1, line, output, SCREEN_MAX_X, InputTypeNumerical);
    
    long code = SEGA_atoi(output);
    unsigned char last_level = 1;
    for (option=1; option <= MAX_LEVEL; ++option)
        if (code == get_levelcode(option)) {
            last_level = option;
            break;
        }
    
   
    
    option = menu(level_names, last_level, line + 3, 4, MenuModeLeft, 1);
    if (option <= MAX_LEVEL)
        gameloop(option, 0);
}

void credits_screen(const unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=5;

    current_location = LocationCredits;
    displayOff();
    load_font();
    clear_screen();
    reset_sprites();
    displayOn();

    if (audio_enabled) {
        PSGPlay(mountain_psg);
    }

    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);

    for (unsigned char entry=0; entry < CREDIT_NAMES_MAX; ++entry) {
        unsigned center = CENTER(strcpy(output, credits_names[entry]));

        switch(entry) {
            case 0: {
                        print_str(0, line, CREDITS_DEVS, 128);
                        line+=2;
                        break;
                    }

            case 1: {
                        line+=2;
                        print_str(0, line++, CREDITS_THANKS, 128);
                        break;
                    }
            default:
                break;
        }

        for (unsigned char pos=0; pos < strlen(output); ++pos) {
            print_tile(center++, line + entry, 128 + output[pos]);
            for (unsigned char wait=0; (wait < 10) && (!keypressed()); ++wait)
                waitForVBlank();
        }
        ++line;
    }

    line = 20;
    strcpy(output, PRESS_TO_CONT);
    print_str(CENTER(output), line, output, 128);

    //add blinking continue text
    for (unsigned char pos=0; pos < strlen(output); ++pos) {
        add_animation(CENTER(output) + pos, line);
    }

    // animation loop
    while(!keypressed()) {
        if (animation_refresh)
            animate_quarterly(ScreenCredits);
        waitForVBlank();
    }

    if (audio_enabled) {
        PSGStop();
        PSGSFXStop();
    }
}

void help_screen(const unsigned char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line=5;

    current_location = LocationHelp;
    displayOff();
    load_font();
    clear_screen();
    reset_sprites();
    displayOn();

    strcpy(output, menu_name);
    print_str(CENTER(output), TITLE_LINE + 1, output, 128);
    // animation loop
    // animation loop
    while(!keypressed()) {
        if (animation_refresh) {
            animate_quarterly(ScreenHelp);
            update_sprites_falling();
        }
        waitForVBlank();
    }
}


void intro_screen(char * menu_name) {
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char line;

    do {
        current_location = LocationIntro;
        displayOff();
        load_font();
        clear_screen();

        strcpy(output, menu_name);
        print_str(CENTER(output), TITLE_LINE + 1, output, 0);

        line=2+1;
        strcpy(output, WRITTEN_BY);
        print_str(CENTER(output), line++, output, 128);
        //strcpy(output, IN_YEAR);
        //print_str(CENTER(output), line++, output, 128);

        print_img_compressed(  city__tiles__bin,
                    city__palette__bin,
                    SCREEN_MAX_RES_X, 96, 0, 8, EffectSpiral);

        line=20;
        for(unsigned char y=line; y < SCREEN_MAX_Y; ++y)
            print_str(0, y, "                                ", 128);

        strcpy(output, VERSION);
        print_str(SCREEN_MAX_X - strlen(output), SCREEN_MAX_Y - 1, output, 128);
        displayOn();
        reset_time(1);

        unsigned char option = menu(intro_items, MAX_INTRO_ITEMS, 21, 10, MenuModeCenter, 0);
        switch (option) {
            case (unsigned char) MainMenuNewGame: {
                gameloop(1+1, seconds >= DEMO_START_AFTER);
                break;
            }
            case (unsigned char) MainMenuLevelSelect: {
                level_select_screen(LEVEL_SELECT);
                break;
            }
            /*case (unsigned char) MainMenuHelp: {
                help_screen(HELP);
                break;
            }*/
            case (unsigned char) MainMenuCredits: {
                credits_screen(CREDITS);
                break;
            }
            /*
            case (unsigned char) MainMenuCongratulation: {
                congratulation_screen(CONGRATULATIONS);
                break;
            }*/
            default: {
                break;
            }
        }


     } while (1);
}