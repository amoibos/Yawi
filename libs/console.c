#ifdef PLATFORM_SMS
#include "../src/SMSlib.h"
#endif

#ifdef PLATFORM_SG
#include "../src/SGlib.h"
#endif

#include "../assets/assets.h"
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
    for(int x=0; x < SCREEN_MAX_X; ++x)
    {
        SG_setNextTileatXY(x, line);
        SG_setTile(' ');    
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

void displayOn() {
#ifdef PLATFORM_SMS
    SMS_displayOn();
#elif PLATFORM_SG
    SG_displayOn();
#endif    
}

void displayOff() {
#ifdef PLATFORM_SMS
    SMS_displayOff();
#elif PLATFORM_SG
    SG_displayOff();
#endif    
}


void waitForVBlank() {
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
