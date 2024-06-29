#include "engine.h"

void load_font(void) {
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);    
}

char load_leveldata(const char no, Leveldata * level) {
    const unsigned char * data;
    
    if ( (no < 1) || (no > MAX_LEVEL)) {
        return 0;
    }

    //mapROMBank(no);
    switch (no) {
        case  1: {
            data = level01_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        }  
#ifndef DEMO
        case  2: {
            data = level02_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  3: {
            data = level03_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  4: {
            data = level04_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  5: {
            data = level05_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  6: {
            data = level06_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  7: {
            data = level07_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  8: {
            data = level08_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case  9: {
            data = level09_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case 10: {
            data = level10_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case 11: {
            data = level11_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
        case 12: {
            data = level12_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
        } 
#endif               
       default: {
            data = level01_dat;
            strcpy(level->name, level_names[1]);
            break;
        }         
    }
    
    level->max_gold = 0;
    level->teleport_found = 0; 
    unsigned char first_char=0;
    for(unsigned char y=OFFSET_MAP_Y; y < LEVEL_HEIGHT + OFFSET_MAP_Y; ++y) {
        for(unsigned char x=OFFSET_MAP_X; x < SCREEN_MAX_X + OFFSET_MAP_X; ++x) {
            // skip control characters, 
            while (*data < ' ') data++;
            if (*data == PLAYER1_SYMBOL) {
                level->start_x = x, level->start_y = y;    
            } else if (*data == GOLD_SYMBOL) {
                level->max_gold++;
            } else if ((*data == TELEPORTER_SYMBOL) && 
                       (level->teleport_found < MAX_TELEPORTER) ) {
                level->teleport[level->teleport_found].x = x;
                level->teleport[level->teleport_found].y = y;
                ++level->teleport_found;
            }

            //todo: checks if data is too short or too long 
            print_tile(x, y, *data);
            if ((strchr(BARRIER_SYMBOL, *data) != 0) && !(first_char))
                first_char = *data;
            ++data;
        }
    }
    
    //save 2 * SCREEN_MAX_X data in every map
    for (unsigned char x=OFFSET_MAP_X; x < SCREEN_MAX_X + OFFSET_MAP_X; ++x) {
        print_tile(x, OFFSET_MAP_Y - 1, first_char);
        print_tile(x, LEVEL_HEIGHT + OFFSET_MAP_Y, first_char);
    }
    return 1;
}

_Bool is_within(const signed char x, const signed char y) {
    
    return  (x >= OFFSET_MAP_X) &&
            (y >= OFFSET_MAP_Y) && 
            (x < SCREEN_MAX_X) &&
            (y < LEVEL_HEIGHT + OFFSET_MAP_Y);  
}

_Bool is_border(const signed char x, const signed char y) {

    if (!is_within(x, y))
        return 1;    

    unsigned int brick = get_tile(x, y); 
   
    
    return strchr(BARRIER_SYMBOL, (unsigned char)brick) != 0;
}

/*
    compute level select code    
*/
long get_levelcode(const unsigned char level) {
    long code=0;

    signed char factors[MAX_LEVELCODE_FACTOR] = {2, 3, 5, -2, 4};
    
    for(char idx=0; idx < strlen(level_names[level-1]); ++idx)
        code += level_names[level-1][idx] * factors[idx % MAX_LEVELCODE_FACTOR] * 10; 
    return code * ((code < 0) ? -1 : 1);
}


/*
    printing status line
*/
void update_statusline(Leveldata * level) {
    unsigned char output[32+1];
    unsigned char numstr[10+1];

    output[0] = 0;
    numstr[0] = 0;
    
    strcat(output, "Gold:    "); 
    SEGA_itoa(level->gold, numstr); 
    strcat(output, numstr);
    strcat(output, " / ");
    SEGA_itoa(level->max_gold, numstr); 
    strcat(output, numstr);
    
    clear_line(INFO_LINE);
    print_str(0, INFO_LINE, output, 128);
}

void print_title(void) {

    clear_line(TITLE_LINE);
    print_str(13, TITLE_LINE, GAME_NAME, 0);
}

void setup_level(Leveldata * level) {
    unsigned char output[32+1];

    output[0] = 0;
    
    level->x = level->start_x;
    level->y = level->start_y;
    level->gold = 0;
    level->status = StatusAlive;

    print_title();

    strcat(output, "Level: "); 
    strcat(output, level->name);
    clear_line(STATUS_LINE);
    print_str(0, STATUS_LINE, output, 128);
    update_statusline(level);
}

Position rock_fall_direction(unsigned int tile) {
    Position dir;

    switch (tile) {
        case ROCK_UP_SYMBOL: {
            dir.x = 0, dir.y = -1;
            return dir;
        }
        case ROCK_DOWN_SYMBOL: {
            dir.x = 0, dir.y = 1;
            return dir;
        }
        case ROCK_LEFT_SYMBOL: {
            dir.x = -1, dir.y = 0;
            return dir;
        }
        case ROCK_RIGHT_SYMBOL: {
            dir.x = 1, dir.y = 0;
            return dir;
        }
        default: {
            dir.x = 0, dir.y = 0;
            return dir;
        }
    }
}

Position get_diff_position(Direction dir) {
    Position pos;

    switch (dir) {
        case DirectionUp: {
            pos.x = 0, pos.y = -1;
            break;
        }
        case DirectionDown: {  
            pos.x = 0, pos.y = 1;
            break;
        }
        case DirectionLeft: {  
            pos.x = -1, pos.y = 0;
            break;
        }
        case DirectionRight: {  
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
    Position dest, neighbour;
    unsigned int item;

    diffs = get_diff_position(dir);
    dest.x = level->x + diffs.x;
    dest.y = level->y + diffs.y;
    neighbour.x = level->x + 2 * diffs.x;
    neighbour.y = level->y + 2 * diffs.y;
    item = get_tile(dest.x, dest.y); 

    _Bool precondition = 
        (item == ROCK_LEFT_SYMBOL)      && (dir != DirectionRight)  ||
        (item == ROCK_RIGHT_SYMBOL)     && (dir != DirectionLeft)   ||
        (item == ROCK_UP_SYMBOL)        && (dir != DirectionDown)   ||
        (item == ROCK_DOWN_SYMBOL)      && (dir != DirectionUp)     ||
        (item == BOMB1_SYMBOL)          && (dir != DirectionUp)     ||
        (item == BOMB2_SYMBOL)          && (dir != DirectionLeft)   ||
        (item == BOMB3_SYMBOL)          && (dir != DirectionRight)  ||
        (item == BOMB4_SYMBOL)          && (dir != DirectionDown);

    //check if is space behind the movable object
    //but first check boundaries
    if (!is_within(neighbour.x, neighbour.y))
        item = 0;
    else
        item = get_tile(neighbour.x, neighbour.y);    

    _Bool postcondition = (item == PEBBLE_SYMBOL) || 
                          (item == EMPTY_SYMBOL)  ||
                          0;
   
    return precondition && postcondition;
}

signed int get_checked_tile(signed char x, signed char y) {
    
    if (!is_within(x, y))
        return -1;

    return get_tile(x, y);       
}

void gravitation(Position * motion_objects, Leveldata * level) {
    
    while (level->status != StatusDied) {
        //no motion found then everything is done
        if (motion_objects->x == -1) 
            break;
        
        //Position * current_Motion;
        //current_Motion = motion_objects;

        Position dest, src;
        src.x = motion_objects->x, src.y = motion_objects->y; 
        signed int falling_item = get_checked_tile(motion_objects->x, motion_objects->y);
        switch (falling_item) {
            case ROCK_UP_SYMBOL:    case ROCK_DOWN_SYMBOL:
            case ROCK_LEFT_SYMBOL:  case ROCK_RIGHT_SYMBOL: {
                Position diff;
                //compute destination coordinates
                diff = rock_fall_direction(falling_item);
                dest.x = motion_objects->x + diff.x;
                dest.y = motion_objects->y + diff.y;
                
                //check if falling ends here 
                signed int dest_item = get_checked_tile(dest.x, dest.y);
                if  (
                    (strchr(BLOCKING_SYMBOL, dest_item) != 0) ||
                    (strchr(BARRIER_SYMBOL, dest_item) != 0) ||
                    (!is_within(dest.x, dest.y))
                    ) {
                    //reached boundary
                    motion_objects->x = -1;
                    print_tile(src.x, src.y, falling_item);
                    check_for_changes(motion_objects, &src); 
                    continue;
                }    
                
                switch (dest_item) {
                    case EMPTY_SYMBOL:
                    case PEBBLE_SYMBOL: { //continue falling
                        motion_objects->x = dest.x;
                        motion_objects->y = dest.y;
                        print_tile(dest.x, dest.y, falling_item);
                        //check_for_changes(motion_objects, dest);  
                        break;   
                    }
                    case PLAYER1_SYMBOL: {
                        print_tile(src.x, src.y, EMPTY_SYMBOL);
                        print_tile(dest.x, dest.y, falling_item);
                        level->status = StatusDied;
                        continue;
                    }
                    case BOMB1_SYMBOL: case BOMB2_SYMBOL:
                    case BOMB3_SYMBOL: case BOMB4_SYMBOL:
                    case TANK_SYMBOL: {
                        //explosion
                        motion_objects->x = -1;
                        break;
                    } 
                    default: {
                        motion_objects->x = -1;
                        break;
                    }
                } //switch (dest_item) {
                break;
            }
            default: {
                break;
            }   
        } //switch (item) {
        print_tile(src.x, src.y, EMPTY_SYMBOL);
        check_for_changes(motion_objects, &src); 
    }
}

void check_for_changes(Position * motion_objects, Position * source) {
    Position motion;

    for (signed char y=source->y - 3; y < source->y + 3; ++y) 
        for (signed char x=source->x - 3; x < source->x + 3; ++x) {
            signed int item = get_checked_tile(x, y);
            switch (item) {
                case ROCK_UP_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x, motion.y - 1) == EMPTY_SYMBOL) {
                        motion_objects->x = motion.x, motion_objects->y = motion.y;
                        return;
                    }
                    break;   
                }
                case ROCK_DOWN_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x, motion.y + 1) == EMPTY_SYMBOL) {
                        motion_objects->x = motion.x, motion_objects->y = motion.y;
                        return;
                    }
                    break;     
                }
                case ROCK_LEFT_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x - 1, motion.y) == EMPTY_SYMBOL) {
                        motion_objects->x = motion.x, motion_objects->y = motion.y;
                        return;
                    }
                    break;   
                }
                case ROCK_RIGHT_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x + 1, motion.y) == EMPTY_SYMBOL) {
                        motion_objects->x = motion.x, motion_objects->y = motion.y;
                        return;
                    }
                    break;   
                }
                default: {
                    break;   
                }
            } 

        }
}


Direction get_input(unsigned char* demo_mode, unsigned char * demo_pos, unsigned char * delay) {
    Direction dir=DirectionUndefined;

    unsigned char aborted=0;
    for (unsigned char wait=0; wait < 2+*delay; ++wait) {
        if (getKeysHeld()) {
            if (++aborted >= 2)
                break;
        }
        waitForVBlank();
    }
          
    switch (readkey()) {
        case PORT_A_KEY_LEFT: {
            dir = DirectionLeft;
            break;
        }
        case PORT_A_KEY_RIGHT: {
            dir = DirectionRight;
            break;
        }
        case PORT_A_KEY_UP: {
            dir = DirectionUp;
            break;
        }
        case PORT_A_KEY_DOWN: {
            dir = DirectionDown;
            break;
        } 
        case PORT_A_KEY_2 | PORT_B_KEY_2: {
            dir = DirectionExit;
            break;
        }
        default: {
            dir = DirectionUndefined;
            break;
        } 
    }
    
    // used for demo mode
    if (*demo_mode) {
        if ((dir >= DirectionRight) && (dir <= DirectionDown)) {
            //stop demo or stop counter before start of demo
            *demo_mode = 0;
        } else if (*demo_pos < MAX_STEP_SEQUENCE)
            dir = level01_step_sequence[(*demo_pos)++]; //max demo sequence reached
    }

    //control acceleration
    if (aborted) {
        if (*delay > 0)
            --(*delay);
    } else //if (*delay < MAX_MOVE_DELAY)
        *delay = MAX_MOVE_DELAY;  

    for (unsigned char wait=0; wait < 2; ++wait) waitForVBlank();     

    return dir;
}

void gameloop(unsigned char curr_level, unsigned char demo_mode) {
    Direction dir;
    Leveldata level;
    unsigned char found;
    _Bool moved_stone=0;
    unsigned char demo_pos=0;
    unsigned char delay=MAX_MOVE_DELAY;

    // history of all objects which were activated by user action like releasing stones
    Position motion_objects;

    load_font();
    clear_screen();  
    if (load_leveldata(curr_level, &level)) {
        setup_level(&level);
        motion_objects.x = -1;   

        while ( (level.status != StatusDied) &&  (level.status != StatusCompleted)) {
            dir = get_input(&demo_mode, &demo_pos, &delay);
            if (dir == DirectionExit)
                return;

            if (dir != DirectionUndefined) {
                //dest: next position, neighbour position afternext 
                Position dest, neighbour, diffs, source;
                diffs = get_diff_position(dir);
                dest.x = diffs.x + level.x, dest.y = diffs.y + level.y;
                unsigned int item = get_tile(dest.x, dest.y);

                // walk against a wall, nothing happen
                if (is_border(dest.x, dest.y)) 
                    continue;
                
                if (strchr(MOVABLE_SYMBOL, item) != 0) {
                    if (is_pushing_object(&level, dir)) {
                        moved_stone = 1;
                        source.x = level.x, source.y = level.y;
                        neighbour.x = source.x + 2 * diffs.x;
                        neighbour.y = source.y + 2 * diffs.y; 
                        unsigned int pushed = get_tile(dest.x, dest.y);
                        //check_for_changes(motion_objects, source, neighbour);
                        print_tile(neighbour.x, neighbour.y, pushed);
                    } else //walk against object but way is occupied, nothing happen
                        continue;    
                } 

                found = 0;
                //when entered teleport field, find other teleport
                if (item == TELEPORTER_SYMBOL) {
                    for (unsigned char n=0; (n < level.teleport_found) && (!found); ++n) {
                        if ((dest.x = level.teleport[n].x) &&
                            (dest.y = level.teleport[n].y) ) {
                                // set player nearby teleporter, search right, up, left then down
                                for (unsigned int dir=DirectionRight; dir < DirectionUndefined; ++dir) {
                                    diffs = get_diff_position(dir);
                                    if (get_tile(diffs.x + level.teleport[n].x, 
                                                 diffs.y + level.teleport[n].y) == EMPTY_SYMBOL) {
                                        found = (n + 1) % level.teleport_found;
                                        dest.x = level.teleport[found].x + diffs.x;
                                        dest.y = level.teleport[found].y + diffs.y; 
                                        break;   
                                    }
                                }
                        }
                    }
                } else if (item == GOLD_SYMBOL) {
                    ++level.gold;
                } else if (item == THORNS_SYMBOL) {
                    level.status = StatusDied;
                    continue;
                } else if (item == EXIT_SYMBOL) {
                    if (level.gold >= level.max_gold) {
                        if (curr_level == MAX_LEVEL) {
                            level.status = StatusCompleted;
                        } else {
                            next_level("Not hard enough?", ++curr_level);
                            load_leveldata(curr_level, &level);
                            setup_level(&level);
                            continue;      
                        }  
                    } else //not collected all gold 
                        continue;
                } else {
                    for (unsigned char n=0; n < level.teleport_found; ++n) {
                        if ((level.x == level.teleport[n].x) &&
                            (level.y == level.teleport[n].y)) 
                            found = 1;
                    }
                }
                print_tile(level.x, level.y, (found) ? TELEPORTER_SYMBOL : EMPTY_SYMBOL);

                level.x = dest.x;
                level.y = dest.y;
                print_tile(level.x, level.y, PLAYER1_SYMBOL);

                update_statusline(&level);
                check_for_changes(&motion_objects, &dest);
                gravitation(&motion_objects, &level); 
            }
            waitForVBlank();        
        }
    }
    if (level.status == StatusCompleted)
        endscreen("Congratulations");
    else if (level.status == StatusDied)
        deathscreen("Mission failed"); 
}
