#include "src/SGlib.h"

#include "libs/console.h"
#include "libs/strings.h"

#define KEY_AMOUNT ((2))


void print_assets(char x, char y, char tileno)
{
    SG_setNextTileatXY(x, y);
    SG_setTile(tileno);   
}

void print_str(char x, char y, char* str) {
    for(; *str; ++str) {
        if (x >= 32)
            ++y, x=0;
        print_assets(x, y, *str);
        ++x;
    }
}


void main(void) {
    unsigned char no=1;
    unsigned int keys[KEY_AMOUNT];
    char keycode[4];

    load_ascii_tiles();
    clear_screen();
    SG_displayOn(); 
    //print_str(0, 0, "Hello World!"); 
    while(1) {
        SG_waitForVBlank();
        if (no != 0)
            no=SG_GetKeycode(keys, KEY_AMOUNT);
        if (no) {
            SEGA_itoa(keys[0], keycode, 0);
            print_str(0, 0, keycode);    
            no=0;
        }
    }

}