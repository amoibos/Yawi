#include "src/SGlib.h"

#include "libs/console.h"
#include "libs/strings.h"

#include "assets/levels.h"

#define MAX_LEVEL ((2))
#define LEVEL_HEIGHT ((20))


char load_leveldata(char no, unsigned char *start_x, unsigned char *start_y) {
    char *data;
    if ( (no < 1) || (no > MAX_LEVEL))
        return 0;

    //mapROMBank(no);

    switch (no) {
        case 1: {
            data = level1_dat;
            break;
        }
        case 2: {
            //data = level2_dat;
            break;
        }  
              
    }
 
    for(char y=0; y < LEVEL_HEIGHT; ++y) {
        for(char x=0; x < SCREEN_MAX_X; ++x) {
            // skip control characters, 
            while (*data < 32) data++;
            if (*data == '@') {
                *start_x = x;
                *start_y = y;    
            }

            //todo: checks if data is too short or too long 
            print_tile(x, y, *data);
            ++data;
        }
    }
    
    return 1;
}


char check_border(unsigned char x, unsigned y) {
   
    if ((x < 0) ||
        (y < 0) ||
        (x >= SCREEN_MAX_X) ||
        (y >= SCREEN_MAX_Y))
    return 0;    

    //TODO: collision checks within
    //requires SG_GetTile(x, y)
}

void gameloop() {
    unsigned char curr_level = 1;
    unsigned char x, x_diff, y, y_diff;
    unsigned int  key;


    if (load_leveldata(curr_level, &x, &y)) {
        while (1) 
        {
            key = 0;
            x_diff = 0;
            y_diff = 0;

            if (SG_getKeysPressed) {
                key = SG_getKeysStatus();
                switch (key) {
                    case PORT_A_KEY_LEFT: {
                        x_diff = -1;
                        break;
                    }
                    case PORT_A_KEY_RIGHT: {
                        x_diff = 1;
                        break;
                    }
                    case PORT_A_KEY_UP: {
                        y_diff = -1;
                        break;
                    }
                    case PORT_A_KEY_DOWN: {
                        y_diff = 1;
                        break;
                    }    

                }
            }


            if ((y_diff != 0) || (x_diff != 0))
                if (check_border(x + x_diff, y + y_diff)) 
                    continue;

            waitForVBlank();        
        }
    }
}

void init_screen() {
    clear_screen();
    displayOn(); 
}

void load_font() {
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);    
}

void main(void) {

    init_screen();
    load_font();    
    gameloop();
}