#include "engine.h"

void load_font(void) {

    //mapROMBank(BANK_FONT);
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);
}

char load_leveldata(const unsigned char no) {
    const unsigned char * data;

    if ( (no < 1) || (no > MAX_LEVEL)) {
        return 0;
    }

    //mapROMBank(BANK_LEVELS);
    reset_sprites();
    switch (no) {
        case  1: {
            data = level01_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
#ifndef DEMO
        case  2: {
            data = level02_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  3: {
            data = level03_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  4: {
            data = level04_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  5: {
            data = level05_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  6: {
            data = level06_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  7: {
            data = level07_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  8: {
            data = level08_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case  9: {
            data = level09_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case 10: {
            data = level10_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case 11: {
            data = level11_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
        case 12: {
            data = level12_dat;
            strcpy(level.name, LEVEL_NAMES[no - 1]);
            break;
        }
#endif
       default: {
            data = level01_dat;
            strcpy(level.name, LEVEL_NAMES[1]);
            break;
        }
    }

    level.max_gold = 0;
    level.teleports_found = 0;
    unsigned char first_char=0;
    for(unsigned char y=OFFSET_MAP_Y; y < LEVEL_HEIGHT + OFFSET_MAP_Y; ++y) {
        for(unsigned char x=OFFSET_MAP_X; x < TEXTCONSOLE_MAX_X + OFFSET_MAP_X; ++x) {
            // skip control characters,
            while (*data < ' ') data++;
            if (*data == PLAYER1_SYMBOL_LEFT) {
                level.x = x;
                level.y = y;
            } else if (*data == GOLD_SYMBOL) {
                level.max_gold++;
            } else if ((*data == TELEPORTER_SYMBOL) &&
                       (level.teleports_found < MAX_TELEPORTER) ) {
                level.teleport[level.teleports_found].x = x;
                level.teleport[level.teleports_found].y = y;
                ++level.teleports_found;
            } else if (*data == EXIT_SYMBOL) {
                add_animation(x, y);
            }

            //todo: checks if data is too short or too long
            print_tile(x, y, *data);
            if ((strchr(BARRIER_SYMBOLS, *data) != -1) && !(first_char))
                first_char = *data;
            ++data;
        }
    }

    //save 2 * TEXTCONSOLE_MAX_X data in every map
    for (unsigned char x=OFFSET_MAP_X; x < TEXTCONSOLE_MAX_X + OFFSET_MAP_X; ++x) {
        print_tile(x, OFFSET_MAP_Y - 1, first_char);
        print_tile(x, LEVEL_HEIGHT + OFFSET_MAP_Y, first_char);
    }

    //mapROMBank(BANK_FONT);
    return 1;
}

inline _Bool is_within(const signed char x, const signed char y) {

    return  (x >= OFFSET_MAP_X) &&
            (y >= OFFSET_MAP_Y) &&
            (x < TEXTCONSOLE_MAX_X) &&
            (y < LEVEL_HEIGHT + OFFSET_MAP_Y);
}

_Bool is_border(const signed char x, const signed char y) {

    if (!is_within(x, y))
        return 1;

    unsigned short brick = get_default_tile(x, y, LocationInGame);

    return strchr(BARRIER_SYMBOLS, (unsigned char)brick) != -1;
}

/*
    compute level select code
*/
long get_levelcode(unsigned char level) {
    long code=0;

    signed char factors[MAX_LEVELCODE_FACTOR] = {2, 3, 5, -2, 4};

    for(char idx=0; idx < strlen(LEVEL_NAMES[level - 1]); ++idx)
        code += LEVEL_NAMES[level-1][idx] * factors[idx % MAX_LEVELCODE_FACTOR] * 10;
    return code * ((code < 0) ? -1 : 1);
}


/*
    printing status line
*/
void update_statusline(void) {
    unsigned char output[TEXTCONSOLE_MAX_X+1];
    unsigned char numstr[10+1];

    strcat(strcat(strcpy(output, GOLD), SEGA_itoa(level.gold, numstr)),  " / ");

    SEGA_itoa(level.max_gold, numstr);
    strcat(output, numstr);

    //clear_line(INFO_LINE);
    print_str(0, INFO_LINE, output, 128);
}


/*
    showing game title during playing
*/
void print_title(unsigned char * title) {
    unsigned char output[TEXTCONSOLE_MAX_X+1];

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
        print_str(TEXTCONSOLE_MAX_X - strlen(output), INFO_LINE, strcat(SEGA_itoa(seconds, output), "s"), 0);
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
            animation_refresh = 1;
            fps = 0;
        }
    }

    if (timer_enabled && seconds) {
        fps_segment = (fps + 1) / (FRAME_RATE >> 3);
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

/*
    reset level state, build screen elements
*/
void setup_level(void) {
    unsigned char output[TEXTCONSOLE_MAX_X+1];

    //level.x = level.start_x;
    //level.y = level.start_y;
    level.gold = 0;
    level.status = StatusAlive;

    print_title(GAME_NAME);
    clear_line(PROGRESS_LINE);

    clear_line(STATUS_LINE);
    print_str(0, STATUS_LINE, strcat(strcpy(output, LEVEL), level.name), 128);
    update_statusline();
    reset_time(1);
}

/*
    map based on tile number the direction when gravity is active
*/
inline Position fall_direction(unsigned short tile) {
    Position dir;

    switch (tile) {
        case ROCKETU_SYMBOL:
        case PLATFORMU_SYMBOL:    
        case ARROW_UP_SYMBOL: {
            dir.x = 0, dir.y = -1;
            return dir;
        }
        case ROCKETD_SYMBOL:
        case PLATFORMD_SYMBOL:    
        case ARROW_DOWN_SYMBOL: {
            dir.x = 0, dir.y = 1;
            return dir;
        }
        case ROCKETL_SYMBOL:
        case PLATFORML_SYMBOL:    
        case ARROW_LEFT_SYMBOL: {
            dir.x = -1, dir.y = 0;
            return dir;
        }
        case ROCKETR_SYMBOL:
        case PLATFORMR_SYMBOL:
        case ARROW_RIGHT_SYMBOL: {
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


unsigned short get_default_tile(unsigned char x, unsigned char y, Location location ) {
    signed short tile=TILE_UNKNOWN;

    for(unsigned char pos=0; pos < sprites_no; ++pos)
        if (all_sprites[pos] != TILE_UNKNOWN) {
            SpriteData sprite;
            sprite = get_sprite_data(all_sprites[pos]);
            unsigned char sprite_x = sprite.x;
            unsigned char sprite_y = sprite.y;
            signed char sprite_index = sprite.index;

            
            if ((sprite_x == x) && (sprite_y == y)) {
                switch(location) {
                    case LocationInGame: {
                        tile = INGAME_SPRITE[sprite_index][sprite_index];
                        break;
                    }
                }
            }
        }

    if (tile == TILE_UNKNOWN)
        tile = get_tile(x, y);
    return tile;
}

/*
    return if object is pushable in that direction
*/
_Bool is_pushing_object(Direction dir) {
    Position diffs;
    Position dest, neighbour;
    unsigned short item;

    diffs = get_diff_position(dir);
    dest.x = level.x + diffs.x;
    dest.y = level.y + diffs.y;
    neighbour.x = level.x + (diffs.x << 1);
    neighbour.y = level.y + (diffs.y << 1);
    item = get_tile(dest.x, dest.y);

    _Bool precondition =
        (item == ARROW_LEFT_SYMBOL)      && (dir != DirectionRight)  ||
        (item == ARROW_RIGHT_SYMBOL)     && (dir != DirectionLeft)   ||
        (item == ARROW_UP_SYMBOL)        && (dir != DirectionDown)   ||
        (item == ARROW_DOWN_SYMBOL)      && (dir != DirectionUp)     ||
        (item == ROCKETD_SYMBOL)        && (dir != DirectionUp)     ||
        (item == ROCKETR_SYMBOL)        && (dir != DirectionLeft)   ||
        (item == ROCKETL_SYMBOL)        && (dir != DirectionRight)  ||
        (item == ROCKETU_SYMBOL)        && (dir != DirectionDown)   ||
        (item == PLATFORMU_SYMBOL)                                  ||
        (item == PLATFORMD_SYMBOL)                                  ||
        (item == PLATFORML_SYMBOL)                                  ||
        (item == PLATFORMR_SYMBOL)                                  

        ;

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
signed char get_first_motion(Position * motion_objects, _Bool exist, unsigned char offset) {

    if (exist) {
        for (unsigned char pos=offset; pos < MAX_MOTION_ITEMS; ++pos)
            if (motion_objects[pos].x != NO_MOTION)
                return pos;

    } else {
        for (unsigned char pos=offset; pos < MAX_MOTION_ITEMS; ++pos)
            if (motion_objects[pos].x == NO_MOTION)
                return pos;
        } 
    return NO_MOTION;
}

/*
  lookup function for active motions
*/
signed char get_motion_position(Position * motion_objects, Position * movable) {
    signed char x, y;

    x = movable->x, y = movable->y;  
    for (unsigned char pos=0; pos < MAX_MOTION_ITEMS; ++pos)
        if ((motion_objects[pos].x == x) &&
            (motion_objects[pos].y == y))
            return pos;
    return NO_MOTION;
}

/*
    add object to motion list
*/
signed char add_motion(Position * motion_objects, Position * item, unsigned char offset) {
    signed char slot;

    //check if it is already in this list
    slot = get_motion_position(motion_objects, item);
    if (slot == NO_MOTION) {
        slot = get_first_motion(motion_objects, SEARCH_FREE_SLOT, offset);
        if (slot > NO_MOTION) {
            motion_objects[slot].x = item->x;
            motion_objects[slot].y = item->y;
        }
    }
    return slot;
}

/*
    end of helper function for moving objects
*/

/*
    return tile at given position with respecting borders
*/
signed short get_checked_tile(signed char x, signed char y) {

    if (!is_within(x, y))
        return -1;

    return get_default_tile(x, y, LocationInGame);
}


_Bool destroyable(signed char x, signed char y) {
    _Bool ret;

    ret =   is_within(x, y) &&
            get_default_tile(x, y, LocationInGame) != WALL_UNDESTROY_SYMBOL &&
            1;

    return ret;    
}

void gravitation(Position * motion_objects) {

    while (level.status != StatusDied) {
        ObjectMove object_moved;
        object_moved = ObjectMoveUndefined;
        signed char pos = get_first_motion(motion_objects, SEARCH_OCCUPIED_SLOT, 0);
        //no motion found then everything is done
        if (pos == -1)
            break;

        Position * current_Motion;
        current_Motion = &motion_objects[pos];
        
        Position dest, src;
        src.x = current_Motion->x, src.y = current_Motion->y;
        signed short falling_item = get_checked_tile(current_Motion->x, current_Motion->y);
        switch (falling_item) {
            //gravitation platforms
            case PLATFORMU_SYMBOL:  case PLATFORMD_SYMBOL:
            case PLATFORML_SYMBOL:  case PLATFORMR_SYMBOL: 
            //rockets
            case ROCKETU_SYMBOL:    case ROCKETD_SYMBOL:
            case ROCKETL_SYMBOL:    case ROCKETR_SYMBOL:
            //stones
            case ARROW_UP_SYMBOL:    case ARROW_DOWN_SYMBOL:
            case ARROW_LEFT_SYMBOL:  case ARROW_RIGHT_SYMBOL: {
                Position diff;
                //compute destination coordinates
                diff = fall_direction(falling_item);
                dest.x = current_Motion->x + diff.x;
                dest.y = current_Motion->y + diff.y;
                object_moved = get_object_move(get_Direction(&diff));

                //check if falling ends here
                signed short dest_item = get_checked_tile(dest.x, dest.y);
                if  (
                    (strchr(BLOCKING_SYMBOLS, dest_item)    != -1) ||
                    (strchr(BARRIER_SYMBOLS, dest_item)     != -1) ||
                    (strchr(EXPLOSIVE_SYMBOLS,falling_item) != -1) ||
                    (!is_within(dest.x, dest.y))
                    ) {
                        //reached boundary, remove object from list
                        current_Motion->x = NO_MOTION;
                        print_tile(src.x, src.y, falling_item);

                        check_for_changes(motion_objects, &src, object_moved);
                        continue;
                }

                switch (dest_item) {
                    case EMPTY_SYMBOL:
                    case PEBBLE_SYMBOL: { //continue falling
                        current_Motion->x = dest.x;
                        current_Motion->y = dest.y;
                        print_tile(dest.x, dest.y, falling_item);
                        //every step due otherwise the distance it too high

                        //check_for_changes(motion_objects, current_Motion, object_moved);
                        break;
                    }
                    case PLAYER1_SYMBOL_LEFT: case PLAYER1_SYMBOL_RIGHT: { //killed by moving object
                        print_tile(src.x, src.y, EMPTY_SYMBOL);
                        print_tile(dest.x, dest.y, falling_item);
                        PSGPlayNoRepeat(death_psg);
                        level.status = StatusDied;
                        continue;
                    }

                    case ROCKETU_SYMBOL:
                    case ROCKETD_SYMBOL:
                    case ROCKETL_SYMBOL:
                    case ROCKETR_SYMBOL: {
                        if (strchr(ARROW_SYMBOLS, falling_item) != -1) {
                            //explosion
                               
                        }
                        //remove object from list
                        current_Motion->x = NO_MOTION;
                        break;
                    }
                    default: {
                        current_Motion->x = NO_MOTION;
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
        check_for_changes(motion_objects, &src, object_moved);
    }
}

void check_for_changes(Position * motion_objects, Position * source, ObjectMove move) {
    Position motion;
    unsigned char i=0;

                
    while(1)
    {
        signed char x = MOTION_CHECKS[(unsigned char)move + i].x;
        
        if (x == END_STEP)
            break;

        signed char y = MOTION_CHECKS[(unsigned char)move + i].y;

        ++i;

        x += source->x;
        y += source->y;

        signed short item = get_checked_tile(x, y);
        if (item == -1) 
            continue;

        switch (item) {
            case ARROW_UP_SYMBOL: {
                motion.x = x, motion.y = y;
                if (get_checked_tile(motion.x, motion.y - 1) == EMPTY_SYMBOL) {
                    add_motion(motion_objects, &motion, MOTION_UP_OFFSET);
                }
                break;
            }
            case ARROW_DOWN_SYMBOL: {
                motion.x = x, motion.y = y;
                if (get_checked_tile(motion.x, motion.y + 1) == EMPTY_SYMBOL) {
                    add_motion(motion_objects, &motion, MOTION_DOWN_OFFSET);
                }
                break;
            }
            case ARROW_LEFT_SYMBOL: {
                motion.x = x, motion.y = y;
                if (get_checked_tile(motion.x - 1, motion.y) == EMPTY_SYMBOL) {
                    add_motion(motion_objects, &motion, MOTION_LEFT_OFFSET);
                }
                break;
            }
            case ARROW_RIGHT_SYMBOL: {
                motion.x = x, motion.y = y;
                if (get_checked_tile(motion.x + 1, motion.y) == EMPTY_SYMBOL) {
                    add_motion(motion_objects, &motion, MOTION_RIGHT_OFFSET);
                }
                break;
            }
            default: {
                break;
            }
        }

    }
}

inline Direction get_Direction(Position *pos) {
    Direction result=DirectionUndefined; 

    if (pos->x == 0) {
        if (pos->y == 1) {
            result = DirectionDown;
        } else if (pos->y == -1) {
            result = DirectionUp;
        }
    } else if (pos->y == 0) {
        if (pos->x == 1) {
            result = DirectionRight;
        } else if (pos->x == -1) {
            result = DirectionLeft;
        }
    }     

    return result;
}

Direction get_input(_Bool* demo_mode, unsigned char * demo_pos) {
    Direction dir=DirectionUndefined;
    unsigned int button;
   
    button = getKeyboardJoypadStatus();
    scanKeyboardJoypad();
    if (getKeysHeld())
        button = readkey();
    
    switch (button) {
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
            dir = LEVEL01_STEP_SEQUENCE[(*demo_pos)++]; //max demo sequence reached
    }

    return dir;
}

inline ObjectMove get_object_move(Direction dir) {
    ObjectMove result;

    switch(dir) {
        case DirectionUp: {
             result = ObjectMoveNorth;
             break;
        }
        case DirectionDown: {
             result = ObjectMoveSouth;
             break;
        }
        case DirectionLeft: {
             result = ObjectMoveWest;
             break;
        }
        case DirectionRight: {
             result = ObjectMoveEast;
             break;
        }
        default:
            result = ObjectMoveUndefined;

    }
    return result;

}

void gameloop(unsigned char curr_level, _Bool demo_mode) {
    Direction dir, prev_dir;
    unsigned char found;
    _Bool moved_stone=0;
    unsigned char demo_pos=0;
    unsigned char delay=MAX_MOVE_DELAY;
    unsigned char player_figure = PLAYER1_SYMBOL_LEFT;
    Position prev_position;

    // history of all objects which were activated by user action like releasing stones
    Position motion_objects[MAX_MOTION_ITEMS];
    dir = prev_dir = DirectionUndefined;

    load_font();
    clear_screen();

    //add_player_sprite();
    if (load_leveldata(curr_level)) {
        setup_level();
        if (curr_level < 2)
            totaltime = 0;
        //extend_player_sprite(player_figure, (unsigned char) level.x << 3, (unsigned char)level.y << 3);
        for (unsigned char n=0; n < MAX_MOTION_ITEMS; ++n) {
            motion_objects[n].x = -1;
        }
        current_location = LocationInGame;
        reset_time(1);
        prev_position.x = level.x, prev_position.y = level.y;
        while ( (level.status != StatusDied) && (level.status != StatusCompleted)) {
            //TODO: why animation is stopped when pressed a key but time goes on?

            print_playtime();
            if (animation_refresh) {
                //print_playtime();
                animate_quarterly(ScreenIngame);
            }

            dir = get_input(&demo_mode, &demo_pos);
            if (dir != DirectionUndefined) {
                wait((prev_dir != dir) ? 10-2 : 2-2);
            }
            if ((prev_dir != DirectionUndefined) && (dir == DirectionUndefined)) {
                wait(5);
            }

            prev_dir = dir;
            if (dir == DirectionExit)
                break;

            if (dir != DirectionUndefined) {
                ObjectMove player_moved;
                player_moved = get_object_move(dir);

                Position dest, movable_src, movable_dst, diffs, source;
                diffs = get_diff_position(dir);
                dest.x = diffs.x + level.x;
                dest.y = diffs.y + level.y;
                unsigned short item = get_default_tile(dest.x, dest.y, LocationInGame);

                // walk against a wall, nothing happen
                if (is_border(dest.x, dest.y))
                    continue;

                if (strchr(MOVABLE_SYMBOLS, item) != -1) {
                    if (is_pushing_object(dir)) {
                        // object is movable but is the place next to free?
                        moved_stone = 1;
                        source.x = level.x; source.y = level.y;
                        movable_src.x = source.x + diffs.x;
                        movable_src.y = source.y + diffs.y;
                        movable_dst.x = movable_src.x + diffs.x;
                        movable_dst.y = movable_src.y + diffs.y;

                        unsigned short pushed = get_default_tile(dest.x, dest.y, LocationInGame);
                        ObjectMove object_moved=ObjectMoveUndefined; 
                        object_moved = get_object_move(dir);

                        signed char pos = strchr(PLATFORM_SYMBOLS, item);
                        if (pos != -1) {
                            if ((dir > DirectionUndefined) && (dir < DirectionExit)) {
                                signed short tile_platform = PLATFORM_SYMBOLS[pos];
                                print_tile(movable_dst.x, movable_dst.y, tile_platform);
                            }
                        } else {
                            print_tile(movable_dst.x, movable_dst.y, pushed);    
                        }
                        check_for_changes(motion_objects, &movable_dst, object_moved);
                        

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
                                for (unsigned short dir=DirectionRight; dir <= DirectionDown; ++dir) {
                                    diffs = get_diff_position(dir);

                                    if (get_default_tile(diffs.x + level.teleport[n].x,
                                                 diffs.y + level.teleport[n].y, LocationInGame) == EMPTY_SYMBOL) {
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
                        //PSGSFXPlay(coin_psg, 0);
                        update_statusline();
                    }
                } else if (item == WATER_SYMBOL) {
                    level.status = StatusDied;
                    PSGPlayNoRepeat(death_psg);
                    continue;
                } else if (item == EXIT_SYMBOL) {
                    if (level.gold >= level.max_gold) {
                        totaltime += seconds;
                        if (curr_level == MAX_LEVEL) {
                            level.status = StatusCompleted;
                        } else {
                            PSGPlayNoRepeat(warp_psg);
                            next_level_screen(NOT_HARD, ++curr_level);
                            load_leveldata(curr_level);
                            setup_level();
                            continue;
                        }
                    } else //not all gold collected, continue playing
                        continue;
                } else {
                    for (unsigned char n=0; n < level.teleports_found; ++n) {
                        if ((level.x == level.teleport[n].x) &&
                            (level.y == level.teleport[n].y))
                            found = 1;
                    }
                }
                print_tile(level.x, level.y, (found) ? TELEPORTER_SYMBOL : EMPTY_SYMBOL);

                /* next player position */
                prev_position.x = level.x, prev_position.y = level.y;
                level.x = dest.x;
                level.y = dest.y;
                
                /* draw player representation */
                if (dir == DirectionRight)
                    player_figure = PLAYER1_SYMBOL_RIGHT;
                else if (dir == DirectionLeft)
                    player_figure = PLAYER1_SYMBOL_LEFT;
                print_tile(level.x, level.y, player_figure);
                //extend_player_sprite(player_figure, (unsigned char) level.x << 3, (unsigned char)level.y << 3);

                check_for_changes(motion_objects, &prev_position, player_moved);
                gravitation(motion_objects);
            }
        }
    }

    clear_sprites();
    if (level.status == StatusCompleted)
        congratulation_screen(CONGRATULATIONS);
    else if (level.status == StatusDied) {
        print_title(GAME_NAME GAME_OVER);
        while(!pressed_anything()) waitForVBlank();
        death_screen(MISSION_FAIL);
    }
}

void init_sprite_position(unsigned char color) {

    initSprites();
    for(unsigned char row=0; row < MAX_SPRITE / MAX_SPRITE_PER_LINE; ++row)
        for(unsigned char column=0; column < MAX_SPRITE_PER_LINE; ++column) {
            unsigned char pos = row * MAX_SPRITE_PER_LINE + column;
            unsigned char sprite_pos_x = (column << 6) - 1 + ((row % 2) == 0 ? 0 : MAX_SPRITE_PER_LINE << 3);
            unsigned char sprite_pos_y = row << 4;
            unsigned sprite_color;

            if (color == 0)
                sprite_color = ((pos % 16) < 2 ? 15 : (pos % 16));
            else
                sprite_color = color;
            addSprite(sprite_pos_x, sprite_pos_y, pos, sprite_color);
    }
}


void clear_sprites(void) {
    initSprites();
    finalizeSprites();
    waitForVBlank();
    copySpritestoSAT();
    waitForVBlank();
}

void extend_player_sprite(unsigned char player_figure, unsigned char current_x, unsigned char current_y) {

    for(unsigned char pos=0; pos < 4; ++pos) {
        unsigned char idx;

        idx = (pos << 2) + OFFSET_SPRITE_Y;
        if  (player_figure == PLAYER1_SYMBOL_LEFT) {
            SpriteTable[idx] = ((pos % 2) ? 0xE0 : current_y - 1);
        } else {
            SpriteTable[idx] = ((pos % 2) ? current_y - 1: 0xE0);
        }
        SpriteTable[idx + 1] = current_x;
    }

    finalizeSprites();
    waitForVBlank();
    copySpritestoSAT();
}

void add_player_sprite(void) {

    clear_sprites();

    //SG_loadZX7compressedSpritesTiles(font__tiles__bin + PLAYER1_SYMBOL_LEFT_BODY/*   * 8*/, 0);
    //SG_loadZX7compressedSpritesTiles(font__tiles__bin + PLAYER1_SYMBOL_LEFT_BODY/*   * 8*/, 1);
    //SG_loadZX7compressedSpritesTiles(font__tiles__bin + PLAYER1_SYMBOL_LEFT_BODY/*   * 8*/, 2);
    //SG_loadZX7compressedSpritesTiles(font__tiles__bin + PLAYER1_SYMBOL_LEFT_BODY/*   * 8*/, 3);
    //SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_LEFT_BODY   * 8, 0, 8);
    //SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_RIGHT_BODY  * 8, 1, 8);
    //SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_LEFT_REST   * 8, 2, 8);
    //SG_loadSpritePatterns(font__tiles__bin + PLAYER1_SYMBOL_RIGHT_REST  * 8, 3, 8);
    addSprite((level.x << 3), (level.y << 3)-1, 0, SG_COLOR_LIGHT_RED);
    addSprite((level.x << 3), (level.y << 3)-1, 1, SG_COLOR_LIGHT_RED);
    addSprite((level.x << 3), (level.y << 3)-1, 2, SG_COLOR_GRAY);
    addSprite((level.x << 3), (level.y << 3)-1, 3, SG_COLOR_GRAY);
}

/*
    save pseudo sprite
*/

inline signed short set_sprite_data(unsigned char sprite, unsigned char x, unsigned char y) {

    return ((signed short)(sprite << 10)) | (x << 5) | y;
}

void add_animation(unsigned char x, unsigned char y) {

    if (sprites_no < MAX_SPRITE)
        all_sprites[sprites_no] = set_sprite_data(sprites_no, x, y);
    ++sprites_no;
}

void reset_sprites(void) {

    animation_frame = 0;
    for(unsigned char pos=0; pos < MAX_SPRITE; ++pos)
        all_sprites[pos] = NO_MOTION;
    sprites_no = 0;
    animation_refresh = 0;
}
