#include "console.h"

#ifdef PLATFORM_SMS
void clear_screen(void)
{
    SMS_setNextTileatXY(0,0);
    for(int y=0; y < SCREEN_MAX_Y; ++y)
        for(int x=0; x < SCREEN_MAX_X; ++x)
            SMS_putchar(' ');
    SMS_setNextTileatXY(0,0);
}
#endif

#ifdef PLATFORM_SG
void clear_screen(void)
{
    for(int y=0; y < SCREEN_MAX_Y; ++y)
        for(int x=0; x < SCREEN_MAX_X; ++x)
        {
            SG_setNextTileatXY(x, y);
            SG_setTile(' ');    
        }    
    SG_setNextTileatXY(0, 0);
}
#endif

#ifdef PLATFORM_SG
void clear_line(unsigned char line)
{
    for(unsigned char x = 0; x < SCREEN_MAX_X; ++x)
    {
        SG_setNextTileatXY(x, line);
        SG_setTile(EMPTY_SYMBOL);    
    }    
    SG_setNextTileatXY(0, line);
}
#endif


void load_ascii_tiles(int position)
{
#ifdef PLATFORM_SMS
    SMS_autoSetUpTextRenderer();  
    //SMS_loadTiles(font__tiles__bin, position, font__tiles__bin_size);
    //SMS_loadBGPalette(font_palette_bin);
#elif PLATFORM_SG
    SG_loadTilePatterns(font__tiles__bin, position, font__tiles__bin_size);
    SG_loadTileColours(font__palette__bin, position, font__palette__bin_size); 
#endif
}

void displayOn(void) {
#ifdef PLATFORM_SMS
    SMS_displayOn();
#elif PLATFORM_SG
    SG_displayOn();
#endif    
}

void displayOff(void) {
#ifdef PLATFORM_SMS
    SMS_displayOff();
#elif PLATFORM_SG
    SG_displayOff();
#endif    
}


void waitForVBlank(void) {
#ifdef PLATFORM_SMS
    SMS_waitForVBlank();
#elif PLATFORM_SG
    SG_waitForVBlank();
#endif    
}

void print_tile(unsigned char x, unsigned char y, unsigned char tileno)
{   
#ifdef PLATFORM_SMS

#elif PLATFORM_SG
    SG_setNextTileatXY(x, y);
    SG_setTile(tileno);
#endif 
}

void print_str(unsigned char x, unsigned char y, char *str) {
    for(; *str; ++str) {
        if (x >= SCREEN_MAX_Y)
            ++y, x=0;
        print_tile(x, y, *str);
        ++x;
    }
}

void print_num(unsigned char x, unsigned char y, long num) {
    char buffer[10+1]; 
    char *str; 

    str = buffer;
    SEGA_itoa(num, buffer);
    
    for(; *str; ++str) {
        if (x >= SCREEN_MAX_X)
            ++y, x=0;
        print_tile(x, y, *str);
        ++x;
    }
}
