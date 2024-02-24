#include "src/SGlib.h"

#include "libs/console.h"
#include "libs/strings.h"

#define KEY_AMOUNT ((8))


void print_assets(unsigned char x, unsigned char y, unsigned char tileno)
{
    SG_setNextTileatXY(x, y);
    SG_setTile(tileno+256);   
}

void print_str(unsigned char x, unsigned char y, char *str) {
    for(; *str; ++str) {
        if (x >= 32)
            ++y, x=0;
        print_assets(x, y, *str);
        ++x;
    }
}


void main(void) {
    unsigned char no;
    unsigned int keys[KEY_AMOUNT];
    unsigned char keycode[6];
    
    keycode[0] = 0;
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);

    clear_screen();
    SG_displayOn(); 
    
    SEGA_itoa(-295, keycode);  
    print_str(0, 10, keycode);
    
    /*
    for(char y=0; y < 24; y++)
        for(char x=0; x < 32; x++)
            print_str(x, y, "Q");       
    clear_screen();
    */
    while(1) 
    {
        clear_line(0);
        no=SG_GetKeycode(keys, KEY_AMOUNT);
        if (no) 
        {
            print_str(0, 0, "R");  

            SEGA_itoa(no, keycode);
            print_str(5, 0, keycode);
            
            SEGA_itoa(keys[0], keycode);  
            print_str(10, 0, keycode);

            if (keys[0] < 0)
                print_str(1, 0, "-");
            else if (keys[0] > 0)
               print_str(1, 0, "+");
            else      
               print_str(1, 0, "0");

            SEGA_itoa(keys[1], keycode);  
            print_str(15, 0, keycode);
        }
        else 
        {
            print_str(0, 0, "N");  
            SEGA_itoa(no, keycode);
            print_str(5, 0, keycode);          
        }
        SG_waitForVBlank(); 
        SG_waitForVBlank();
        SG_waitForVBlank();  
    }

}