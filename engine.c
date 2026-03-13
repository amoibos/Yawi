#include "engine.h"

// layout defines offsets of eObjectMove values, stop pair indicate end of check sequence
static const Position MOTION_CHECKS[] = {
                                                         {END_STEP, END_STEP}
     ,{0,0}, {0,-1},        {1,0}, {-1,0}, {END_STEP, END_STEP}
     ,{0,0},        {0,1},  {1,0}, {-1,0}, {END_STEP, END_STEP}
     ,{0,0}, {0,-1}, {0,1},         {-1,0}, {END_STEP, END_STEP}
     ,{0,0}, {0,-1}, {0,1}, {1,0},          {END_STEP, END_STEP}
};

const unsigned char LEVEL01_STEP_SEQUENCE[MAX_STEP_SEQUENCE] = { DirectionUp,
                                DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,
                                DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,
                                DirectionUp,DirectionUp,
                                DirectionRight,DirectionRight,DirectionRight,
                                DirectionDown,DirectionDown,
                                DirectionUp,DirectionUp,DirectionUp,
                                DirectionLeft,
                                DirectionUp,
                                DirectionLeft,DirectionLeft,DirectionLeft,DirectionLeft,DirectionLeft,DirectionLeft,
                                DirectionUp,DirectionUp,DirectionUp,
                                DirectionLeft,DirectionLeft,
                                DirectionUp,DirectionUp,
                                DirectionLeft,DirectionLeft,
                                DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,
                                DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,
                                DirectionLeft,DirectionLeft,DirectionLeft
                            };

static _Bool is_barrier(unsigned short c) {
    switch(c) {
        case '+': case '|': case '-': case '%': case '~': return 1;
        default: return 0;
    }
}
static _Bool is_blocking(unsigned short c) {
    switch(c) {
        case '<': case '>': case '^': case 'v':
        case ':': case '$': case 'l': case 'm': case 'n': case 'o': return 1;
        default: return 0;
    }
}
static _Bool is_explosive(unsigned short c) {
    switch(c) {
        case 'U': case 'V': case 'W': case 'X': case 'B': return 1;
        default: return 0;
    }
}
static _Bool is_arrow(unsigned short c) {
    switch(c) {
        case '<': case '>': case '^': case 'v': return 1;
        default: return 0;
    }
}
static _Bool is_movable(unsigned short c) {
    switch(c) {
        case '<': case '>': case '^': case 'v':
        case 'B': case 'U': case 'V': case 'W': case 'X':
        case 'l': case 'm': case 'n': case 'o': return 1;
        default: return 0;
    }
}
static _Bool is_platform(unsigned short c) {
    switch(c) {
        case 'l': case 'm': case 'n': case 'o': return 1;
        default: return 0;
    }
}

//for acceleration of character movement
signed short xvel=0;
signed short yvel=0;

signed short xpos[2] = {0, 0};
signed short ypos[2] = {0, 0};


void load_font(void) {

    for(unsigned char n=0; n <= 2; n++) 
        load_ascii_tiles(n << 8);
}

char load_leveldata(const unsigned char no) {
    const unsigned char * data;
    const unsigned char * levels[] = {  level01_dat
#ifndef DEMO
                                        , level02_dat, level03_dat, level04_dat, 
                                        level05_dat, level06_dat, level07_dat, level08_dat, 
                                        level09_dat, level10_dat, level11_dat, level12_dat
#endif
                                    };
    unsigned char loc_no=no;

    if ( (no < 1) || (no > MAX_LEVEL))
        return 0;

    
    loc_no = no;
    data = levels[loc_no - 1];
    strcpy(level.name, LEVEL_NAMES[loc_no - 1]);

    reset_sprites();
    level.max_gold = 0;
    level.teleports_found = 0;
    unsigned char first_char=0;
    unsigned char rle_remaining=0;
    for(unsigned char y=OFFSET_MAP_Y; y < LEVEL_HEIGHT + OFFSET_MAP_Y; ++y) {
        for(unsigned char x=OFFSET_MAP_X; x < TEXTCONSOLE_MAX_X + OFFSET_MAP_X; ++x) {
            unsigned char tile;
            if (rle_remaining) {
                tile = 0x20;
                --rle_remaining;
            } else {
                while (*data < 0x20) data++;
                if (*data & 0x80) {
                    rle_remaining = (*data & 0x7F) - 1;
                    ++data;
                    tile = 0x20;
                } else {
                    tile = *data++;
                }
            }

            if (tile == PLAYER1_SYMBOL_LEFT) {
                level.x = x;
                level.y = y;
            } else if (tile == GOLD_SYMBOL) {
                level.max_gold++;
            } else if ((tile == TELEPORTER_SYMBOL) &&
                       (level.teleports_found < MAX_TELEPORTER)) {
                level.teleport[level.teleports_found].x = x;
                level.teleport[level.teleports_found].y = y;
                ++level.teleports_found;
            } else if (tile == EXIT_SYMBOL) {
                add_animation(x, y);
            }

            print_tile(x, y, tile);
            if (is_barrier(tile) && !(first_char))
                first_char = tile;
        }
    }

    //save 2 * TEXTCONSOLE_MAX_X data in every map
    for (unsigned char x=OFFSET_MAP_X; x < TEXTCONSOLE_MAX_X + OFFSET_MAP_X; ++x) {
        print_tile(x, OFFSET_MAP_Y - 1, first_char);
        print_tile(x, LEVEL_HEIGHT + OFFSET_MAP_Y, first_char);
    }

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

    return is_barrier((unsigned char)brick);
}

/*
    compute level select code
*/
unsigned int get_levelcode(unsigned char level) {
    unsigned int code=0;

    signed char factors[MAX_LEVELCODE_FACTOR] = {2, 3, 5, -2, 4};
    unsigned char len = strlen(LEVEL_NAMES[level - 1]);

    for(char idx=0; idx < len; ++idx)
        code += LEVEL_NAMES[level-1][idx] * factors[idx % MAX_LEVELCODE_FACTOR] * 10;
    return code;
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
    unsigned char output[7];

    if (current_location == LocationInGame) {
        SEGA_itoa(seconds, output);
        strcat(output, "s");
        print_str(TEXTCONSOLE_MAX_X - strlen(output), INFO_LINE, output, 0);
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
    setup acceleration for controls
*/
inline void reset_movement(void) {
    xvel = 0;
    yvel = 0;
    xpos[0] = 0;
    xpos[1] = 0;
    ypos[0] = 0;
    ypos[1] = 0;
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
    reset_movement();
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
                    is_blocking(dest_item)      ||
                    is_barrier(dest_item)       ||
                    is_explosive(falling_item)  ||
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
                    case PLAYER1_SYMBOL_LEFT: case PLAYER1_SYMBOL_RIGHT:
                    case PLAYER1_SYMBOL_LEFT_ALT: case PLAYER1_SYMBOL_RIGHT_ALT: { //killed by moving object
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
                        if (is_arrow(falling_item)) {
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
    static unsigned int prev_button = 0;

    scanKeyboardJoypad();
    button = getKeyboardJoypadStatus();
    if (getKeysHeld())
        button = readkey();

    switch (button) {
        case PORT_A_KEY_LEFT: {
            if (prev_button != PORT_A_KEY_LEFT) {
                xvel = 0;
                xpos[0] = 0;
                xpos[1] = 0;
            }

            xvel -= ACCEL_STEP;
            if (xvel <= -MAX_SPEED)
                xvel = -MAX_SPEED;

            signed short old_xpos = xpos[0] + xvel;
            if (old_xpos < -255) {
                --xpos[1];
            }
            break;
        }
        case PORT_A_KEY_RIGHT: {
            if (prev_button != PORT_A_KEY_RIGHT) {
                xvel = 0;
                xpos[0] = 0;
                xpos[1] = 0;
            }

            xvel += ACCEL_STEP;
            if (xvel >= MAX_SPEED)
                xvel = MAX_SPEED;

            signed short old_xpos = xpos[0] + xvel;
            if (old_xpos > 255) {
                ++xpos[1];
            }

            break;
        }
        case PORT_A_KEY_UP: {
            if (prev_button != PORT_A_KEY_UP) {
                yvel = 0;
                ypos[0] = 0;
                ypos[1] = 0;
            }

            yvel -= ACCEL_STEP;
            if (yvel <= -MAX_SPEED)
                yvel = -MAX_SPEED;

            signed short old_ypos = ypos[0] + yvel;
            if (old_ypos < -255) {
                --ypos[1];
            }

            break;
        }
        case PORT_A_KEY_DOWN: {
            if (prev_button != PORT_A_KEY_DOWN) {
                yvel = 0;
                ypos[0] = 0;
                ypos[1] = 0;
            }

            yvel += ACCEL_STEP;
            if (yvel >= MAX_SPEED)
                yvel = MAX_SPEED;

            signed short old_ypos = ypos[0] + yvel;
            if (old_ypos > 255) {
                ++ypos[1];
            }

            break;
        }
        case PORT_A_KEY_2 | PORT_B_KEY_2: {
            dir = DirectionExit;
            break;
        }
        default: {
            xvel = 0;
            yvel = 0;
            xpos[1] = 0;
            ypos[1] = 0;
            xpos[0] = 0;
            ypos[0] = 0;
            break;
        }
    }
    
    ypos[0] += yvel;
    xpos[0] += xvel;

    if (xpos[0] < -255) xpos[0] += 256;
    else if (xpos[0] > 255) xpos[0] -= 256;
    if (ypos[0] < -255) ypos[0] += 256;
    else if (ypos[0] > 255) ypos[0] -= 256;

    if (ypos[1] >= GEAR_STEP) {
        ypos[1] -= GEAR_STEP;
        dir = DirectionDown;
    } else if (ypos[1] <= -GEAR_STEP) {
        ypos[1] += GEAR_STEP;
        dir = DirectionUp;
    } else  if (xpos[1] >= GEAR_STEP) {
        xpos[1] -= GEAR_STEP;
        dir = DirectionRight;
    } else if (xpos[1] <= -GEAR_STEP) {
        xpos[1] += GEAR_STEP;
        dir = DirectionLeft;
    }

    // used for demo mode
    if (*demo_mode) {
        if ((dir > DirectionUndefined) && (dir < DirectionExit)) {
            //stop demo or stop counter before start of demo
            *demo_mode = 0;
        } else if (*demo_pos < MAX_STEP_SEQUENCE)
            dir = LEVEL01_STEP_SEQUENCE[(*demo_pos)++]; //max demo sequence reached
    }

    prev_button = button;
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
    Direction dir;
    unsigned char found;
    _Bool moved_stone=0;
    unsigned char demo_pos=0;
    unsigned char player_figure = PLAYER1_SYMBOL_LEFT;
    Position prev_position;

    // history of all objects which were activated by user action like releasing stones
    Position motion_objects[MAX_MOTION_ITEMS];
    dir = DirectionUndefined;

    displayOff();
    load_font();
    clear_screen();

    if (load_leveldata(curr_level)) {
        setup_level();
        displayOn();
        if (curr_level < 2)
            totaltime = 0;
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

                if (is_movable(item)) {
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

                        if (is_platform(item)) {
                            if ((dir > DirectionUndefined) && (dir < DirectionExit)) {
                                print_tile(movable_dst.x, movable_dst.y, item);
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
                    player_figure = (player_figure == PLAYER1_SYMBOL_RIGHT) ? PLAYER1_SYMBOL_RIGHT_ALT : PLAYER1_SYMBOL_RIGHT;
                else
                    player_figure = (player_figure == PLAYER1_SYMBOL_LEFT) ? PLAYER1_SYMBOL_LEFT_ALT : PLAYER1_SYMBOL_LEFT;
                
                print_tile(level.x, level.y, player_figure);
                
                check_for_changes(motion_objects, &prev_position, player_moved);
                gravitation(motion_objects);
            }
            //waitForVBlank();
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
