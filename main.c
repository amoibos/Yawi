#include "src/SGlib.h"

#include "libs/console.h"
#include "libs/strings.h"

#define KEY_AMOUNT ((4))

#define INFOLINE ((10))


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

void print_num(unsigned char x, unsigned char y, long num) {
    char buffer[10+1]; 
    char *str; 

    str = buffer;
    SEGA_itoa(num, buffer);
    
    for(; *str; ++str) {
        if (x >= 32)
            ++y, x=0;
        print_assets(x, y, *str);
        ++x;
    }
}

void main(void) {
    unsigned char no, row;
    unsigned int keys[KEY_AMOUNT];
    unsigned char keycode[5+1];
    
    load_ascii_tiles(0);
    load_ascii_tiles(256);
    load_ascii_tiles(512);

    clear_screen();
    SG_displayOn(); 
    
    //SEGA_itoa(-295, keycode);  
    //print_str(0, 10, keycode);
    
    /*
    for(char y=0; y < 24; y++)
        for(char x=0; x < 32; x++)
        {
            print_str(x, y, "Q");  
            for(no=0; no < 10; no++)
                SG_waitForVBlank();      
        }*/
    clear_screen();
    //print_str(0,10, "Hello World!");
    while(1) 
    {
        clear_line(INFOLINE);
        for(unsigned char n=0; n < KEY_AMOUNT; n++)
            keys[n] = 0;
        no=SG_GetKeycode(keys, KEY_AMOUNT);
        if (no) 
        {
            print_str(0, INFOLINE, "R");  

            //number of detected keys
            print_num(1, INFOLINE, no);
            //print_num(1, INFOLINE, keys[0]);

            row=keys[0] >> 12; keys[0] &= 0x0FFF;  
            print_num(4-1, INFOLINE, row);
            print_num(4, INFOLINE, keys[0]);

            row=keys[1] >> 12; keys[1] &= 0x0FFF;  
            print_num(11-1, INFOLINE, row);
            print_num(11, INFOLINE, keys[1]);

            row=keys[2] >> 12; keys[2] &= 0x0FFF;  
            print_num(18-1, INFOLINE, row);
            print_num(18, INFOLINE, keys[2]);

            row=keys[3] >> 12; keys[3] &= 0x0FFF;
            print_num(25-1, INFOLINE, row);
            print_num(25, INFOLINE, keys[2]);

            print_str(0, 15, "keypressed");
        }
        else 
        {
            print_str(0, INFOLINE, "N");  
            SEGA_itoa(no, keycode);
            print_str(5, INFOLINE, keycode);     
            print_str(0, 15, "no keys");     
        }
        for(no=0; no < 4; no++)
            SG_waitForVBlank();  
    }

}