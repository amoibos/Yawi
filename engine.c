#include "engine.h"

char load_leveldata(const char no, Leveldata * level) {
    const char *data;
    
    if ( (no < 1) || (no > MAX_LEVEL)) {
        return 0;
    }

    //mapROMBank(no);

    switch (no) {
        case 1: {
            data = level1_dat;
            strcpy(level->name, "Lateral Thinking");
            break;
        }
        default:
            data = level1_dat;
            break;         
    }
 
    for(char y=0; y < LEVEL_HEIGHT; ++y) {
        for(char x=0; x < SCREEN_MAX_X; ++x) {
            // skip control characters, 
            while (*data < 32) data++;
            if (*data == PLAYER_SYMBOL) {
                level->start_x = x;
                level->start_y = y;    
            } else if (*data == EXIT_SYMBOL) {
                level->exit_x = x;
                level->exit_y = y;
            } else if (*data == GOLD_SYMBOL) {
                level->max_gold++;
            }
            //todo: checks if data is too short or too long 
            print_tile(x, y + OFFSET_MAP, *data);
            ++data;
        }
    }
    
    return 1;
}


char is_border(const signed char x, const signed char y) {

    if ((x < 0) ||
        (y < OFFSET_MAP) ||
        (x >= SCREEN_MAX_X) ||
        (y >= SCREEN_MAX_Y + OFFSET_MAP))
    return 1;    

    unsigned int brick = get_tile(x, y + OFFSET_MAP); 
     
    if (strchr(BARRIER_SYMBOL, brick) != 0)
        return 1;

    return 0;
}

void update_statusline(Leveldata * level) {
    char output[30+1];
    char numstr[10+1];

    output[0] = 0;
    numstr[0] = 0;
    
    strcat(output, "Gold: "); 
    SEGA_itoa(level->gold, numstr); 
    strcat(output, numstr);
    strcat(output, " / ");
    SEGA_itoa(level->max_gold, numstr); 
    strcat(output, numstr);
    
    clear_line(INFO_LINE);
    print_str(0, INFO_LINE, output);
}

void setup_level(Leveldata * level) {
    char output[30+1];
    char numstr[10+1];

    output[0] = 0;
    numstr[0] = 0;
    
    level->x = level->start_x;
    level->y = level->start_y;
    level->gold = 0;
    level->status = ALIVE;

    strcat(output, "Level: "); 
    strcat(output, level->name);
    
    print_str(10, 0, GAME_NAME);
    clear_line(STATUS_LINE);
    print_str(0, STATUS_LINE, output);
    update_statusline(level);
}

void endscreen(void) {
    clear_screen();
    print_str(0, 0, "Congratulation");
}

void deathscreen(void) {
    clear_screen();
    print_str(0, 0, "Try it again");
}
