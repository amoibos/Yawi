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
#ifndef DEMO
        case 2: {
            data = level2_dat;
            strcpy(level->name, level_names[no - 1]);
            break;
            } 
     /*   case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:*/
#endif            
            
       default: {
            data = level1_dat;
            strcpy(level->name, level_names[1]);
            break;
        }         
    }
    
    level->max_gold = 0;
    level->teleport_found = 0; 
    char first_char = *data;
    for(char y=0; y < LEVEL_HEIGHT; ++y) {
        for(char x=0; x < SCREEN_MAX_X; ++x) {
            // skip control characters, 
            while (*data < ' ') data++;
            if (*data == PLAYER1_SYMBOL) {
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
    
    //save 2 * SCREEN_MAX_X data in every map
    for (char x=0; x < SCREEN_MAX_X; ++x) {
        print_tile(x, OFFSET_MAP - 1, first_char);
        print_tile(x, LEVEL_HEIGHT + OFFSET_MAP, first_char);
    }
    return 1;
}

_Bool in_Field(const signed char x, const signed char y) {
    return  (x >= 0) &&
            (y >= OFFSET_MAP) && 
            (x < SCREEN_MAX_X) &&
            (y < LEVEL_HEIGHT + OFFSET_MAP);  
}

_Bool is_border(const signed char x, const signed char y) {

    if (!in_Field(x, y))
        return 1;    

    unsigned int brick = get_tile(x, y); 
   
    if (strchr(BARRIER_SYMBOL, brick) != 0)
        return 1;

    return 0;
}

long levelcode(char level) {
    return level + 0;
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
    print_str(0, INFO_LINE, output, 128);
}

void print_title(void) {


    clear_line(TITLE_LINE);
    print_str(13, TITLE_LINE, GAME_NAME, 128);
}

void setup_level(Leveldata * level) {
    char output[30+1];
    char numstr[10+1];

    output[0] = 0;
    numstr[0] = 0;
    
    level->x = level->start_x;
    level->y = level->start_y;
    level->gold = 0;
    level->status = StatusAlive;

    print_title();

    strcat(output, "Chapter: "); 
    strcat(output, level->name);
    clear_line(STATUS_LINE);
    print_str(0, STATUS_LINE, output, 128);
    update_statusline(level);
}

Position item_direction(unsigned int tile) {
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

Position player_direction(Direction dir) {
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
    Position new, neighbour;
    unsigned int item;

    diffs = player_direction(dir);
    new.x = level->x + diffs.x;
    new.y = level->y + diffs.y;
    neighbour.x = level->x + 2 * diffs.x;
    neighbour.y = level->y + 2 * diffs.y;

    item = get_tile(new.x, new.y + OFFSET_MAP); 

    _Bool precondition = 
        (item == ROCK_LEFT_SYMBOL)      && (dir != DirectionRight)  ||
        (item == ROCK_RIGHT_SYMBOL)     && (dir != DirectionLeft)   ||
        (item == ROCK_UP_SYMBOL)        && (dir != DirectionDown)   ||
        (item == ROCK_DOWN_SYMBOL)      && (dir != DirectionUp)     ||
        (item == BOMB1_SYMBOL)     && (dir != DirectionUp)     ||
        (item == BOMB2_SYMBOL)     && (dir != DirectionLeft)   ||
        (item == BOMB3_SYMBOL)     && (dir != DirectionRight)  ||
        (item == BOMB4_SYMBOL)     && (dir != DirectionDown)   ||
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

signed char firstMotion(Position * motionObjects, _Bool exist) {

    for (char pos=0; pos < MAX_MOTION_ITEMS; ++pos)
        if ( (exist) ? (motionObjects[pos].x != -1) : (motionObjects[pos].x == -1))
            return pos;
    return -1;
}

signed char existMotion(Position * motionObjects, Position moveable) {

    for (char pos=0; pos < MAX_MOTION_ITEMS; ++pos)
        if ((motionObjects[pos].x == moveable.x) && (motionObjects[pos].x == moveable.x))             
            return pos;
    return -1;    
}

signed char addMotion(Position * motionObjects, Position item) {
    signed char slot;

    slot = existMotion(motionObjects, item);
    if (slot != -1)
        return slot;
   
    slot = firstMotion(motionObjects, 0); 
    if (slot > -1) {
        motionObjects[slot].x = item.x;
        motionObjects[slot].y = item.y;   
    }
}

void gravitation(Position * motionObjects, Leveldata * level) {

    while (1) {
        signed char pos = firstMotion(motionObjects, 1); 
        if (pos == -1) 
            break;

        unsigned int item = get_tile(motionObjects[pos].x, motionObjects[pos].y);
        switch (item) {
            case ROCK_UP_SYMBOL: case ROCK_DOWN_SYMBOL:
            case ROCK_LEFT_SYMBOL: case ROCK_RIGHT_SYMBOL: {
                
                Direction dir;


                //compute next step 
                Position diff;
                diff = player_direction(dir);
                Position dest;
                dest = item_direction(item);

                dest.x = motionObjects[pos].x + diff.x;
                dest.y = motionObjects[pos].y + diff.y;
                if (!in_Field(dest.x, dest.y) || is_border(dest.x, dest.y)) {
                    //remove entry, reached boundary
                    motionObjects[pos].x = -1; // set y not needed
                    continue;
                }    
                
                unsigned int dest_item = get_tile(dest.x, dest.y);
                switch (dest_item) {
                    case EMPTY_SYMBOL: { //continue falling
                        motionObjects[pos].x = dest.x;
                        motionObjects[pos].y = dest.y;
                        continue;    
                    }
                    default: {
                        break;
                    }
                } 
                break;
            }
            default: {
                break;
            } 
        }
    }
}

void gameloop(unsigned char curr_level) {
    Direction motion;
    Leveldata level;
    char found;
    _Bool moved_stone = 0;
    // history of all objects which were activated by user action like releasing stones
    Position MotionObjects[MAX_MOTION_ITEMS];

    load_font();
    clear_screen();  
    if (load_leveldata(curr_level, &level)) {
        setup_level(&level);
        for (char n=0; n < MAX_MOTION_ITEMS; ++n) {
            MotionObjects[n].x = -1, MotionObjects[n].y = -1;   
        }

        while ( (level.status != StatusDied) &&  (level.status != StatusCompleted)) 
        {
            motion = DirectionUndefined;

            if (keypressed()) {
                switch (readkey()) {
                    case PORT_A_KEY_LEFT: {
                        motion = DirectionLeft;
                        break;
                    }
                    case PORT_A_KEY_RIGHT: {
                        motion = DirectionRight;
                        break;
                    }
                    case PORT_A_KEY_UP: {
                        motion = DirectionUp;
                        break;
                    }
                    case PORT_A_KEY_DOWN: {
                        motion = DirectionDown ;
                        break;
                    }
                    case PORT_B_KEY_1 /*|| PORT_B_KEY_2*/: {
                        for(char i=0; i < 4; ++i) {
                            print_str(0, PROGRESS_LINE, "loading..", 128);
                            for(char j=0; j < 12; ++j)  waitForVBlank();
                            clear_line(PROGRESS_LINE);
                            for(char j=0; j < 12; ++j)  waitForVBlank();    
                        }
                        clear_line(PROGRESS_LINE);
                        break;
                    }
                    case /* PORT_A_KEY_1 || */ PORT_A_KEY_2 : {
                        for(char i=0; i < 4; ++i) {
                            print_str(0, PROGRESS_LINE, "saving..", 128);
                            for(char j=0; j < 12; ++j)  waitForVBlank();
                            clear_line(PROGRESS_LINE);
                            for(char j=0; j < 12; ++j)  waitForVBlank();    
                        }
                        clear_line(PROGRESS_LINE);
                        break;
                    }       
                }
            }

            if (motion != DirectionUndefined) {
                Position new_pos, neighbour;
                Position diffs;
                diffs = player_direction(motion);
                new_pos = diffs;
                new_pos.x += level.x;
                new_pos.y += level.y;  
                unsigned int item = get_tile(new_pos.x, new_pos.y + OFFSET_MAP);

                if (is_border(new_pos.x, new_pos.y + OFFSET_MAP)) 
                    continue;
                
                
                if (strchr(MOVABLE_SYMBOL, item) != 0) {
                    if (is_pushing_object(&level, motion)) {
                        moved_stone = 1;
                        neighbour.x = level.x + 2 * diffs.x;
                        neighbour.y = level.y + 2 * diffs.y; 
                        unsigned int pushed = get_tile(new_pos.x, new_pos.y + OFFSET_MAP);
                        print_tile(neighbour.x, neighbour.y + OFFSET_MAP, pushed);
 
                    } else
                        continue;    
                } 

                found = 0;
                //when entered teleport field, find other teleport
                if (item == TELEPORTER_SYMBOL) {
                    for (char n=0; (n < level.teleport_found) && (!found); ++n) {
                        if ((new_pos.x = level.teleport[n].x) &&
                            (new_pos.y = level.teleport[n].y) ) {
                                // set player nearby teleporter, search right, up, left then down
                                for (int dir=DirectionRight; dir < DirectionUndefined; ++dir) {
                                    Position diffs;
                                    diffs = player_direction(dir);
                                    if (get_tile(diffs.x + level.teleport[n].x, 
                                                diffs.y + level.teleport[n].y + OFFSET_MAP) == EMPTY_SYMBOL) {
                                        found = (n + 1) % level.teleport_found;
                                        new_pos.x = level.teleport[found].x + diffs.x;
                                        new_pos.y = level.teleport[found].y + diffs.y; 
                                        break;   
                                    }
                                }
                        }
                    }
                } else if (item == GOLD_SYMBOL) {
                    ++level.gold;
                } else if (item == THORNS_SYMBOL) {
                    level.status = StatusDied;
                } else if (item == EXIT_SYMBOL) {
                    if (level.gold >= level.max_gold) {
                        if (curr_level == MAX_LEVEL) {
                            level.status = StatusCompleted;
                        } else {
                            load_leveldata(++curr_level, &level);
                            setup_level(&level);      
                        }  
                    } else //not collected all gold 
                        continue;
                } else {
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
                print_tile(level.x, level.y + OFFSET_MAP, PLAYER1_SYMBOL);

                update_statusline(&level);

                if (moved_stone)
                    gravitation(MotionObjects, &level); 
            }
            waitForVBlank();        
        }
    }
    if (level.status == StatusCompleted)
        endscreen("Congratulations");
    else if (level.status == StatusDied)
        deathscreen("Mission failed"); 
}
