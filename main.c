#include "engine.h"

#ifdef PLATFORM_SMS
SMS_EMBED_SDSC_HEADER_AUTO_DATE_16KB(1,0,Darktrym,Yawen,"puzzle game")
#elif PLATFORM_SG

#endif

void gameloop(void);
void init_screen(void);
void load_font(void);

void gameloop(void) {
    unsigned char curr_level = 1;
    signed char new_x=0, x_diff, new_y=0, y_diff;
    _Bool moved;
    Leveldata level;

    if (load_leveldata(curr_level, &level)) {
        setup_level(&level); 
        while ( (level.status != DIED) &&  (level.status != COMPLETED)) 
        {
            x_diff = 0;
            y_diff = 0;
            moved = 0;

            if (keypressed()) {
                switch (readkey()) {
                    case PORT_A_KEY_LEFT: {
                        x_diff = -1, moved = 1;
                        break;
                    }
                    case PORT_A_KEY_RIGHT: {
                        x_diff = 1, moved = 1;
                        break;
                    }
                    case PORT_A_KEY_UP: {
                        y_diff = -1, moved = 1;
                        break;
                    }
                    case PORT_A_KEY_DOWN: {
                        y_diff = 1, moved = 1;
                        break;
                    }    
                }
            }

            
            //check if player move is valid
            if (moved)
            {   
                new_x = level.x + x_diff;
                new_y = level.y + y_diff;  
                if (is_border(new_x, new_y)) 
                    continue;
            }

            //reached all requirements
            if ( (level.x == level.exit_x) &&  
                 (level.x == level.exit_x) &&
                 (level.gold == level.max_gold)
                ) {
                
                if (curr_level == MAX_LEVEL) {
                    level.status = COMPLETED;
                } else {
                    load_leveldata(++curr_level, &level);
                    setup_level(&level); 
                }
            }

            if (moved) {
                print_tile(level.x, level.y + OFFSET_MAP, EMPTY_SYMBOL);
                if (get_tile(new_x, new_y + OFFSET_MAP) == GOLD_SYMBOL)
                    ++level.gold;

                level.x = new_x;
                level.y = new_y;
                print_tile(level.x, level.y + OFFSET_MAP, PLAYER_SYMBOL);

                update_statusline(&level);
            }
            waitForVBlank();        
        }
    }
    if (level.status == COMPLETED)
        endscreen();
    else if (level.status == COMPLETED)
        deathscreen();

    while(1); 
}

void init_screen(void) {
    clear_screen();
    displayOn(); 
}

void load_font(void) {
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);    
}

void main(void) {
    init_screen();
    load_font();    
    gameloop();
}