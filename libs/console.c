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



void load_ascii_tiles(void)
{
#ifdef PLATFORM_SMS
    SMS_autoSetUpTextRenderer();  
    //SMS_loadTiles(font__tiles__bin, 0, font__tiles__bin_size);
    //SMS_loadBGPalette(font_palette_bin);
#elif PLATFORM_SG
    SG_loadTilePatterns(font__tiles__bin, 0, font__tiles__bin_size);
    SG_loadTileColours(font_palette_bin, 0, font_palette_bin_size); 
#endif
}
