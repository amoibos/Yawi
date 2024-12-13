#include "engine.h"

void load_font(void) {

    //mapROMBank(BANK_FONT);   
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);    
}

char load_leveldata(const char no, Leveldata * level) {
    const unsigned char * data;
    
    if ( (no < 1) || (no > MAX_LEVEL)) {
        return 0;
    }

    //mapROMBank(BANK_LEVELS);
    reset_sprites();
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
    level->teleports_found = 0; 
    unsigned char first_char=0;
    for(unsigned char y=OFFSET_MAP_Y; y < LEVEL_HEIGHT + OFFSET_MAP_Y; ++y) {
        for(unsigned char x=OFFSET_MAP_X; x < SCREEN_MAX_X + OFFSET_MAP_X; ++x) {
            // skip control characters, 
            while (*data < ' ') data++;
            if (*data == PLAYER1_SYMBOL_LEFT) {
                level->start_x = x, level->start_y = y;    
            } else if (*data == GOLD_SYMBOL) {
                level->max_gold++;
            } else if ((*data == TELEPORTER_SYMBOL) && 
                       (level->teleports_found < MAX_TELEPORTER) ) {
                level->teleport[level->teleports_found].x = x;
                level->teleport[level->teleports_found].y = y;
                ++level->teleports_found;
            } else if (*data == EXIT_SYMBOL) {
                add_animation(x, y);
            }

            //todo: checks if data is too short or too long 
            print_tile(x, y, *data);
            if ((strchr(BARRIER_SYMBOLS, *data) != 0) && !(first_char))
                first_char = *data;
            ++data;
        }
    }
    
    //save 2 * SCREEN_MAX_X data in every map
    for (unsigned char x=OFFSET_MAP_X; x < SCREEN_MAX_X + OFFSET_MAP_X; ++x) {
        print_tile(x, OFFSET_MAP_Y - 1, first_char);
        print_tile(x, LEVEL_HEIGHT + OFFSET_MAP_Y, first_char);
    }

    //mapROMBank(BANK_FONT);
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

    unsigned int brick = get_default_tile(x, y); 
    
    return strchr(BARRIER_SYMBOLS, (unsigned char)brick) != 0;
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
    unsigned char output[SCREEN_MAX_X+1];
    unsigned char numstr[10+1];

    strcat(strcat(strcpy(output, GOLD), SEGA_itoa(level->gold, numstr)),  " / ");
    SEGA_itoa(level->max_gold, numstr); 
    strcat(output, numstr);
    
    clear_line(INFO_LINE);
    print_str(0, INFO_LINE, output, 128);
}


/*
    showing game title during playing
*/
void print_title(unsigned char * title) {
    unsigned char output[SCREEN_MAX_X+1];

    clear_line(TITLE_LINE);
    strcpy(output, title);
    unsigned char x = CENTER(output);
    if (strlen(title) > strlen(GAME_NAME)) {
        print_str(x, TITLE_LINE, title, 128);
    }
    output[strlen(GAME_NAME)] = 0;
    print_str(x, TITLE_LINE, output, 0);
    
    add_animation(x++, TITLE_LINE);
    add_animation(x++, TITLE_LINE);
    add_animation(x++, TITLE_LINE);
    add_animation(x++, TITLE_LINE);
}


/*
    displaying play time
*/
void print_playtime(void) {
    unsigned char output[5];
    
    if (current_location == LocationInGame) {
        print_str(SCREEN_MAX_X - strlen(output), INFO_LINE, strcat(SEGA_itoa(seconds, output), "s"), 0);
    }
}

void reset_time(signed char timer_on) {
    if ((timer_on >= 0) && (timer_on <= 1))
        timer_enabled = timer_on;

    fps = 0;
    seconds = 0;
    animation_frame = 0;
    previous_fps_seqment = 0;
}

/*
    count fps to get the play time
*/
void timer(void) {
    unsigned fps_segment;

    if (timer_enabled) {
        if (++fps == FRAME_RATE) {
            ++seconds;
            fps = 0;
        }
    }

    if (timer_enabled && seconds) {
        fps_segment = (fps + 1) / (FRAME_RATE / 8);
        if (previous_fps_seqment != fps_segment) {
            animation_refresh = 1;
            previous_fps_seqment = fps_segment;
        } else
            animation_refresh = 0;
    }

    if (audio_enabled) {
        PSGSFXFrame();
        PSGFrame(); 
    } 
}


void reset_sprites(void) {

    animation_frame = 0;
    for(unsigned char pos=0; pos < MAX_SPRITE; ++pos) 
        all_sprites[pos] = -1;
    sprites_no = 0;
    animation_refresh = 0; 
}

/*
    reset level state, build screen elements
*/
void setup_level(Leveldata * level) {
    unsigned char output[SCREEN_MAX_X+1];

    level->x = level->start_x;
    level->y = level->start_y;
    level->gold = 0;
    level->status = StatusAlive;

    print_title(GAME_NAME);
    clear_line(PROGRESS_LINE);

    clear_line(STATUS_LINE);
    print_str(0, STATUS_LINE, strcat(strcpy(output, LEVEL), level->name), 128);
    update_statusline(level);
    reset_time(1);
}

/*
    map based on tile number the direction when gravity is active
*/
Position fall_direction(unsigned int tile) {
    Position dir;

    switch (tile) {
        case EXPLOSIVEZ_SYMBOL:
        case ROCK_UP_SYMBOL: {
            dir.x = 0, dir.y = -1;
            return dir;
        }
        case EXPLOSIVEX_SYMBOL:
        case ROCK_DOWN_SYMBOL: {
            dir.x = 0, dir.y = 1;
            return dir;
        }
        case EXPLOSIVEY_SYMBOL:
        case ROCK_LEFT_SYMBOL: {
            dir.x = -1, dir.y = 0;
            return dir;
        }
        case EXPLOSIVEW_SYMBOL:
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

/*
    compute difference for x and y axis based on given direction enum
*/
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


unsigned int get_default_tile(unsigned char x, unsigned char y) {
    signed int tile=-1;

    for(unsigned char pos=0; pos < sprites_no; ++pos)
        if (all_sprites[pos] != -1) {
            SpriteData sprite;
            sprite = get_sprite_data(all_sprites[pos]);
            unsigned char sprite_x = sprite.x;
            unsigned char sprite_y = sprite.y;
            signed char sprite_index = sprite.index;

            if ((sprite_x == x) && (sprite_y == y)) {
                tile = INGAME_SPRITE[sprite_index][sprite_index];
                break;
            }
        }

    if (tile == -1)
        tile = get_tile(x, y); 
    return tile;
}

/*
    return if object is pushable in that direction
*/
_Bool is_pushing_object(Leveldata * level, Direction dir) {
    Position diffs;
    Position dest, neighbour;
    unsigned int item;

    diffs = get_diff_position(dir);
    dest.x = level->x + diffs.x;
    dest.y = level->y + diffs.y;
    neighbour.x = level->x + (diffs.x << 1);
    neighbour.y = level->y + (diffs.y << 1);
    item = get_tile(dest.x, dest.y); 

    _Bool precondition = 
        (item == ROCK_LEFT_SYMBOL)      && (dir != DirectionRight)  ||
        (item == ROCK_RIGHT_SYMBOL)     && (dir != DirectionLeft)   ||
        (item == ROCK_UP_SYMBOL)        && (dir != DirectionDown)   ||
        (item == ROCK_DOWN_SYMBOL)      && (dir != DirectionUp)     ||
        (item == EXPLOSIVEX_SYMBOL)     && (dir != DirectionUp)     ||
        (item == EXPLOSIVEW_SYMBOL)     && (dir != DirectionLeft)   ||
        (item == EXPLOSIVEY_SYMBOL)     && (dir != DirectionRight)  ||
        (item == EXPLOSIVEZ_SYMBOL)     && (dir != DirectionDown);

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

/*
    start of helper function for continue a motion  
*/

/*
  check if object is already in motion list  
*/
signed char get_first_motion(Position * motion_objects, _Bool exist) {

    for (unsigned char pos=0; pos < MAX_MOTION_ITEMS; ++pos)
        if (exist) {
            if (motion_objects[pos].x != -1) 
                return pos;
            
        } else if (motion_objects[pos].x == -1)
            return pos;
    return -1;
}

/*
  lookup function for active motions
*/
signed char get_motion_position(Position * motion_objects, Position * movable) {

    for (unsigned char pos=0; pos < MAX_MOTION_ITEMS; ++pos)
        if ((motion_objects[pos].x == movable->x) && 
            (motion_objects[pos].y == movable->y))             
            return pos;
    return -1;    
}

/*
    add object to motion list
*/
signed char add_motion(Position * motion_objects, Position * item) {
    signed char slot;
    
    //check if it is already in this list
    slot = get_motion_position(motion_objects, item);
    if (slot != -1)
        return slot;
   
    slot = get_first_motion(motion_objects, 0); 
    if (slot > -1) {
        motion_objects[slot].x = item->x;
        motion_objects[slot].y = item->y;   
    }
    return slot;
}

/*
    end of helper function for moving objects
*/



/*
    save pseudo sprite
*/
void add_animation(unsigned char x, unsigned char y) {

    if (sprites_no < MAX_SPRITE)
        all_sprites[sprites_no] = ((signed int)(sprites_no << 10)) | (x << 5) | y;
    ++sprites_no;
}

/*
    return tile at given position with respecting borders
*/
signed int get_checked_tile(signed char x, signed char y) {
    
    if (!is_within(x, y))
        return -1;

    return get_default_tile(x, y);       
}

void gravitation(Position * motion_objects, Leveldata * level) {
    
    while (level->status != StatusDied) {
        signed char pos = get_first_motion(motion_objects, 1); 
        //no motion found then everything is done
        if (pos == -1) 
            break;
        
        Position * current_Motion;
        current_Motion = &motion_objects[pos];

        Position dest, src;
        src.x = current_Motion->x, src.y = current_Motion->y; 
        signed int falling_item = get_checked_tile(current_Motion->x, current_Motion->y);
        switch (falling_item) {
            case EXPLOSIVEW_SYMBOL:case EXPLOSIVEX_SYMBOL:
            case EXPLOSIVEY_SYMBOL:case EXPLOSIVEZ_SYMBOL:
            case ROCK_UP_SYMBOL:    case ROCK_DOWN_SYMBOL:
            case ROCK_LEFT_SYMBOL:  case ROCK_RIGHT_SYMBOL: {
                Position diff;
                //compute destination coordinates
                diff = fall_direction(falling_item);
                dest.x = current_Motion->x + diff.x;
                dest.y = current_Motion->y + diff.y;
                
                //check if falling ends here 
                signed int dest_item = get_checked_tile(dest.x, dest.y);
                if  (
                    (strchr(BLOCKING_SYMBOLS, dest_item) != 0) ||
                    (strchr(BARRIER_SYMBOLS, dest_item) != 0) ||
                    (strchr(EXPLOSIVE_SYMBOLS,falling_item) != 0) ||
                    (!is_within(dest.x, dest.y))
                    ) {
                    //reached boundary
                    current_Motion->x = -1;
                    print_tile(src.x, src.y, falling_item);
                    check_for_changes(motion_objects, &src); 
                    continue;
                }    
                
                switch (dest_item) {
                    case EMPTY_SYMBOL:
                    case PEBBLE_SYMBOL: { //continue falling
                        current_Motion->x = dest.x;
                        current_Motion->y = dest.y;
                        print_tile(dest.x, dest.y, falling_item);
                        //check_for_changes(motion_objects, dest);  
                        break;   
                    }
                    case PLAYER1_SYMBOL_LEFT: case PLAYER1_SYMBOL_RIGHT:{
                        print_tile(src.x, src.y, EMPTY_SYMBOL);
                        print_tile(dest.x, dest.y, falling_item);
                        PSGPlayNoRepeat(death_psg);
                        level->status = StatusDied;
                        continue;
                    }
                    
                    case EXPLOSIVEX_SYMBOL: 
                    case EXPLOSIVEW_SYMBOL:
                    case EXPLOSIVEY_SYMBOL:
                    case EXPLOSIVEZ_SYMBOL: {
                        if (strchr(ROCK_SYMBOLS, falling_item) != 0) {
                            //explosion    
                        }

                        current_Motion->x = -1;
                        break;   
                    } 
                    case TANK_SYMBOL: {
                        //explosion
                        current_Motion->x = -1;
                        break;
                    } 
                    default: {
                        current_Motion->x = -1;
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
                        add_motion(motion_objects, &motion);
                        return;
                    }
                    break;   
                }
                case ROCK_DOWN_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x, motion.y + 1) == EMPTY_SYMBOL) {
                        add_motion(motion_objects, &motion);
                        return;
                    }
                    break;     
                }
                case ROCK_LEFT_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x - 1, motion.y) == EMPTY_SYMBOL) {
                        add_motion(motion_objects, &motion);
                        return;
                    }
                    break;   
                }
                case ROCK_RIGHT_SYMBOL: {
                    motion.x = x, motion.y = y;
                    if (get_checked_tile(motion.x + 1, motion.y) == EMPTY_SYMBOL) {
                        add_motion(motion_objects, &motion);
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

Direction get_input(_Bool* demo_mode, unsigned char * demo_pos) {
    Direction dir=DirectionUndefined;
     
    if (getKeysHeld())      
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
        if ((dir > DirectionUndefined) && (dir < DirectionExit)) {
            //stop demo or stop counter before start of demo
            *demo_mode = 0;
        } else if (*demo_pos < MAX_STEP_SEQUENCE)
            dir = level01_step_sequence[(*demo_pos)++]; //max demo sequence reached
    }

    return dir;
}

void wait(unsigned char duration) {
    for(unsigned char wait=0; wait < duration; ++wait) 
        waitForVBlank();    
}

void extend_player_sprite(unsigned char player_figure, Leveldata *level) {

    initSprites();    
    addSprite((level->x << 3), (level->y << 3)-1, (player_figure == PLAYER1_SYMBOL_LEFT) ? 0 : 1, SG_COLOR_LIGHT_RED);
    addSprite((level->x << 3), (level->y << 3)-1, (player_figure == PLAYER1_SYMBOL_LEFT) ? 2 : 3, SG_COLOR_GRAY); 
    finalizeSprites();
    waitForVBlank();
    copySpritestoSAT(); 
}

void gameloop(unsigned char curr_level, _Bool demo_mode) {
    Direction dir, prev_dir;
    Leveldata level;
    unsigned char found;
    _Bool status_refresh=0;
    _Bool moved_stone=0;
    unsigned char demo_pos=0;
    unsigned char delay=MAX_MOVE_DELAY;
    unsigned char player_figure = PLAYER1_SYMBOL_LEFT;

    // history of all objects which were activated by user action like releasing stones
    Position motion_objects[MAX_MOTION_ITEMS];
    dir = prev_dir = DirectionUndefined;

    load_font();
    clear_screen();
    
    SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_LEFT_BODY   * 8, 0, 8);
    SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_RIGHT_BODY  * 8, 1, 8);
    SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_LEFT_REST   * 8, 2, 8);
    SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_RIGHT_REST  * 8, 3, 8);
    
    if (load_leveldata(curr_level, &level)) {
        setup_level(&level);
        extend_player_sprite(player_figure, &level);
        for (char n=0; n < MAX_MOTION_ITEMS; ++n) {
            motion_objects[n].x = -1;   
        }
        current_location = LocationInGame;
        reset_time(1);
        while ( (level.status != StatusDied) && (level.status != StatusCompleted)) {
            //TODO: why animation is stopped when pressed a key but time goes on?
            if (animation_refresh) {
                print_playtime();
                animate_quarterly(ScreenIngame);
            }
            
            dir = get_input(&demo_mode, &demo_pos);
            if (dir != DirectionUndefined) {
                wait((prev_dir != dir) ? 10-2 : 5-2);
              //  animation_refresh = 1; 
            }
            if ((prev_dir != DirectionUndefined) && (dir == DirectionUndefined))
                wait(10);

            prev_dir = dir;
            if (dir == DirectionExit)
                return;

            if (dir != DirectionUndefined) {
                //dest: next position, neighbour position afternext
                Position dest, neighbour, diffs, source;
                diffs = get_diff_position(dir);
                dest.x = diffs.x + level.x, dest.y = diffs.y + level.y;
                unsigned int item = get_default_tile(dest.x, dest.y);

                // walk against a wall, nothing happen
                if (is_border(dest.x, dest.y)) 
                    continue;
                
                if (strchr(MOVABLE_SYMBOLS, item) != 0) {
                    if (is_pushing_object(&level, dir)) {
                        moved_stone = 1;
                        source.x = level.x, source.y = level.y;
                        neighbour.x = source.x + (diffs.x << 1);
                        neighbour.y = source.y + (diffs.y << 1); 
                        unsigned int pushed = get_default_tile(dest.x, dest.y);

                        //check_for_changes(motion_objects, source, neighbour);
                        print_tile(neighbour.x, neighbour.y, pushed);
                    } else //walk against object but way is occupied, nothing happen
                        continue;    
                } 

                found = 0;
                //when entered teleport field, find other teleport
                if (item == TELEPORTER_SYMBOL) {
                    for (unsigned char n=0; (n < level.teleports_found) && (!found); ++n) {
                        if ((dest.x = level.teleport[n].x) &&
                            (dest.y = level.teleport[n].y) ) {
                                // set player nearby teleporter, search right, up, left then down
                                for (unsigned int dir=DirectionRight; dir <= DirectionDown; ++dir) {
                                    diffs = get_diff_position(dir);
                                    
                                    if (get_default_tile(diffs.x + level.teleport[n].x, 
                                                 diffs.y + level.teleport[n].y) == EMPTY_SYMBOL) {
                                        found = (n + 1) % level.teleports_found;
                                        dest.x = level.teleport[found].x + diffs.x;
                                        dest.y = level.teleport[found].y + diffs.y; 
                                        break;   
                                    }
                                }
                        }
                    }
                } else if (item == GOLD_SYMBOL) {
                    ++level.gold;
                    if (audio_enabled) {
                        PSGPlayNoRepeat(coin_psg);
                        status_refresh = 1;
                        //PSGSFXPlay(coin_psg, 0);
                    }
                } else if (item == THORNS_SYMBOL) {
                    level.status = StatusDied;
                    PSGPlayNoRepeat(death_psg);
                    continue;
                } else if (item == EXIT_SYMBOL) {
                    if (level.gold >= level.max_gold) {
                        if (curr_level == MAX_LEVEL) {
                            level.status = StatusCompleted;
                        } else {
                            PSGPlayNoRepeat(warp_psg);
                            next_level(NOT_HARD, ++curr_level);
                            load_leveldata(curr_level, &level);
                            setup_level(&level);
                            continue;      
                        }  
                    } else //not collected all gold 
                        continue;
                } else {
                    for (unsigned char n=0; n < level.teleports_found; ++n) {
                        if ((level.x == level.teleport[n].x) &&
                            (level.y == level.teleport[n].y)) 
                            found = 1;
                    }
                }
                print_tile(level.x, level.y, (found) ? TELEPORTER_SYMBOL : EMPTY_SYMBOL);

                level.x = dest.x;
                level.y = dest.y;
                if (dir == DirectionRight)
                    player_figure = PLAYER1_SYMBOL_RIGHT;
                else if (dir == DirectionLeft)
                    player_figure = PLAYER1_SYMBOL_LEFT;
                print_tile(level.x, level.y, player_figure);
                extend_player_sprite(player_figure, &level);
              
                if (status_refresh) {
                    update_statusline(&level);
                     status_refresh = 0;
                }
                check_for_changes(motion_objects, &dest);
                gravitation(motion_objects, &level);
            }
        }
    }
    /*if (audio_enabled) {
        PSGSFXStop();
        PSGStop(); 
    }*/

    if (level.status == StatusCompleted)
        endscreen(CONGRATULATIONS);
    else if (level.status == StatusDied) {
        print_title(GAME_NAME GAME_OVER);
        while(!keypressed()) waitForVBlank();
        deathscreen(MISSION_FAIL);    
    }
}
