#include "engine.h"

void load_font(void) {
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);    
}

char load_leveldata(const char no, Leveldata * level) {
    const char *data;
    
    if ( (no < 1) || (no > MAX_LEVEL)) {
        return 0;
    }

    //mapROMBank(no);

    switch (no) {
        case 1: {
            data = level1_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        }
        case 2: {
            data = level2_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        }   
       default: {
            data = level1_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        }         
    }
    
    level->max_gold = 0;
    level->teleport_found = 0; 

    for(char y=0; y < LEVEL_HEIGHT; ++y) {
        for(char x=0; x < SCREEN_MAX_X; ++x) {
            // skip control characters, 
            while (*data < ' ') data++;
            if (*data == PLAYER_SYMBOL) {
                level->start_x = x;
                level->start_y = y;    
            } else if (*data == EXIT_SYMBOL) {
                level->exit_x = x;
                level->exit_y = y;
            } else if (*data == GOLD_SYMBOL) {
                level->max_gold++;
            } else if ((*data == TELEPORTER_SYMBOL) && 
                       (level->teleport_found < MAX_TELEPORTER) ) {
                level->teleport[level->teleport_found].x = x;
                level->teleport[level->teleport_found].y = y;
                ++level->teleport_found;
            }

            //todo: checks if data is too short or too long 
            print_tile(x, y + OFFSET_MAP, *data);
            ++data;
        }
    }
    
    return 1;
}

_Bool in_Field(const signed char x, const signed char y) {
    return !((x < 0) ||
            (y < OFFSET_MAP) ||
            (x >= SCREEN_MAX_X) ||
            (y >= SCREEN_MAX_Y + OFFSET_MAP));  
}

_Bool is_border(const signed char x, const signed char y) {

    if (!in_Field(x, y))
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
    
    strcat(output, "Gold:    "); 
    SEGA_itoa(level->gold, numstr); 
    strcat(output, numstr);
    strcat(output, " / ");
    SEGA_itoa(level->max_gold, numstr); 
    strcat(output, numstr);
    
    clear_line(INFO_LINE);
    print_str(0, INFO_LINE, output);
}

void print_title(void) {

    clear_line(13);
    print_str(13, 0, GAME_NAME);
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

    print_title();

    strcat(output, "Chapter: "); 
    strcat(output, level->name);
    clear_line(STATUS_LINE);
    print_str(0, STATUS_LINE, output);
    update_statusline(level);
}

_Bool level_completed(Leveldata * level) {

   return ( (level->x    == level->exit_x) &&  
            (level->y    == level->exit_y) &&
            (level->gold == level->max_gold) && 
            1
           );   
}

Position player_direction(Direction dir) {
    Position pos;

    switch (dir) {
        case UP: {
            pos.x = 0, pos.y = -1;
            break;
        }
        case DOWN: {  
            pos.x = 0, pos.y = 1;
            break;
        }
        case LEFT: {  
            pos.x = -1, pos.y = 0;
            break;
        }
        case RIGHT: {  
            pos.x = 1, pos.y = 0;
            break;
        } 
        default:
            pos.x = 0, pos.y = 0;
            break;
    }
    return pos;
}

_Bool is_pushing_object(Leveldata * level, Direction dir) {
    Position diffs;
    Position new, neighbour;
    char item;

    diffs = player_direction(dir);
    new.x = level->x + diffs.x;
    new.y = level->y + diffs.y;
    neighbour.x = level->x + 2 * diffs.x;
    neighbour.y = level->y + 2 * diffs.y;

    item = get_tile(new.x, new.y + OFFSET_MAP); 

    _Bool precondition = 
        (item == ROCK_LEFT_SYMBOL) && ((dir == UP) || dir == DOWN) ||
        (item == ROCK_RIGHT_SYMBOL) && ((dir == UP) || dir == DOWN) ||
        (item == ROCK_UP_SYMBOL) && ((dir == LEFT) || dir == RIGHT) ||
        (item == ROCK_DOWN_SYMBOL) && ((dir == LEFT) || dir == RIGHT) ||
        0;

    //check if is space behind the movable object
    //but first check boundaries
    if (!in_Field(neighbour.x, neighbour.y))
        item = 0;
    else
        item = get_tile(neighbour.x, neighbour.y + OFFSET_MAP);    

    _Bool postcondition = (item == PEBBLE_SYMBOL) || 
                          (item == EMPTY_SYMBOL)  ||
                          0;

    return precondition && postcondition;
}

void gravitation(void) {

}

void gameloop(unsigned char curr_level) {
    Direction motion;
    Leveldata level;
    char found;
    _Bool moved_stone = 0;

    load_font();  
    if (load_leveldata(curr_level, &level)) {
        setup_level(&level); 
        while ( (level.status != DIED) &&  (level.status != COMPLETED)) 
        {
            motion = UNDEFINED_DIRECTION;

            if (keypressed()) {
                switch (readkey()) {
                    case PORT_A_KEY_LEFT: {
                        motion = LEFT;
                        break;
                    }
                    case PORT_A_KEY_RIGHT: {
                        motion = RIGHT;
                        break;
                    }
                    case PORT_A_KEY_UP: {
                        motion = UP;
                        break;
                    }
                    case PORT_A_KEY_DOWN: {
                        motion = DOWN;
                        break;
                    }    
                }
            }


            if (level_completed(&level)) {
                if (curr_level == MAX_LEVEL) {
                    level.status = COMPLETED;
                } else {
                    load_leveldata(++curr_level, &level);
                    setup_level(&level);      
                }  
            }  


            if (motion != UNDEFINED_DIRECTION) {
                Position new_pos, neighbour;
                Position diffs;
                diffs = player_direction(motion);
                new_pos = diffs;
                new_pos.x += level.x;
                new_pos.y += level.y;  
                char item = get_tile(new_pos.x, new_pos.y + OFFSET_MAP);

                if (is_border(new_pos.x, new_pos.y)) 
                    continue;
                
                
                if (strchr(MOVABLE_SYMBOL, item) != 0) {
                    if (is_pushing_object(&level, motion)) {
                        moved_stone = 1;
                        neighbour.x = level.x + 2 * diffs.x;
                        neighbour.y = level.y + 2 * diffs.y; 
                        char pushed = get_tile(new_pos.x, new_pos.y + OFFSET_MAP);
                        print_tile(neighbour.x, neighbour.y + OFFSET_MAP, pushed);

                        
                    } else
                        continue;    
                } 

                found = 0;
                //when entered teleport field, find other teleport
                if (item == TELEPORTER_SYMBOL) {
                    for (char n=0; n < level.teleport_found; ++n) {
                        if ((new_pos.x = level.teleport[n].x) &&
                            (new_pos.y = level.teleport[n].y) ) {
                                found = (n + 1) % level.teleport_found;
                                new_pos.x = level.teleport[found].x;
                                new_pos.y = level.teleport[found].y;
                                break;
                        }
                    }
                } else if (item == GOLD_SYMBOL)
                    ++level.gold;
                else {
                    for (char n=0; n < level.teleport_found; ++n) {
                        if ((level.x == level.teleport[n].x) &&
                            (level.y == level.teleport[n].y)) 
                            found = 1;
                    }
                }
                print_tile(level.x, level.y + OFFSET_MAP, 
                    (found) ? TELEPORTER_SYMBOL : EMPTY_SYMBOL);

                level.x = new_pos.x;
                level.y = new_pos.y;
                print_tile(level.x, level.y + OFFSET_MAP, PLAYER_SYMBOL);

                update_statusline(&level);

                if (moved_stone)
                    gravitation(); 
            }
            waitForVBlank();        
        }
    }
    if (level.status == COMPLETED)
        endscreen("Congratulations");
    else if (level.status == COMPLETED)
        deathscreen("Mission failed"); 
}




