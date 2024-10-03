#include "console.h"

#if defined(PLATFORM_SG) || defined(PLATFORM_SC) || defined(PLATFORM_SMS)
void clear_screen(void)
{
    for(int y=0; y < SCREEN_MAX_Y; ++y)
        for(int x=0; x < SCREEN_MAX_X; ++x)
        {
            setNextTileatXY(x, y);
            setTile(' ');    
        }    
    setNextTileatXY(0, 0);
}
#endif

#if defined(PLATFORM_SG) || defined(PLATFORM_SC) || defined(PLATFORM_SMS)
void clear_line(unsigned char line)
{
    for(unsigned char x = 0; x < SCREEN_MAX_X; ++x)
    {
        setNextTileatXY(x, line);
        setTile(EMPTY_SYMBOL);       
    }    
    setNextTileatXY(0, line);
}
#endif

#if defined(PLATFORM_SG) || defined(PLATFORM_SC) || defined(PLATFORM_SMS)
void load_ascii_tiles(int position) {
    loadTiles(font__tiles__bin, position, font__tiles__bin_size);
    loadPalette(font__palette__bin, position, font__palette__bin_size); 
}
#endif

#if defined(PLATFORM_SG) || defined(PLATFORM_SC) || defined(PLATFORM_SMS)
inline void print_tile(unsigned char x, unsigned char y, unsigned int tileno) {   
    setNextTileatXY(x, y);
    setTile(tileno);
}
#endif

void print_str(unsigned char x, unsigned char y, char *str, int offset) {
    for(; *str; ++str) {
        if (x >= SCREEN_MAX_X)
            ++y, x=0;
        print_tile(x++, y, *str + offset);
    }
}

void print_num(unsigned char x, unsigned char y, long num, int offset) {
    char buffer[10+1]; 
    char *str; 

    str = buffer;
    SEGA_itoa(num, buffer);
    
    for(; *str; ++str) {
        if (x >= SCREEN_MAX_X)
            ++y, x=0;
        print_tile(x++, y, *str + offset);
    }
}
