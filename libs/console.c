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
    for(int y=0; y < 192 / 8; ++y)
        for(int x=0; x < 256 / 8; ++x)
            SMS_putchar(' ');
    SMS_setNextTileatXY(0,0);
}
#endif

#ifdef PLATFORM_SG
void clear_screen(void)
{
    for(int y=0; y < 192 / 8; ++y)
        for(int x=0; x < 256 / 8; ++x)
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
    for(int x=0; x < 256 / 8; ++x)
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
