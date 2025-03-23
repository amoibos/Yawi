#include "console.h"

#if defined(PLATFORM_SG) || defined(PLATFORM_SC) || defined(PLATFORM_SMS)
void clear_screen(void)
{
    for(unsigned short y=0; y < TEXTCONSOLE_MAX_Y; ++y)
        for(unsigned short x=0; x < TEXTCONSOLE_MAX_X; ++x)
        {
            setNextTileatXY(x, y);
            setTile(' ');    
        }    
    setNextTileatXY(0, 0);
}

void clear_line(unsigned char line)
{
    for(unsigned char x = 0; x < TEXTCONSOLE_MAX_X; ++x)
    {
        setNextTileatXY(x, line);
        setTile(EMPTY_SYMBOL);       
    }    
    setNextTileatXY(0, line);
}

void load_ascii_tiles(unsigned short position) {
    //loadTiles(font__tiles__bin, position, font__tiles__bin_size);
    loadZX7compressedTiles(font__tiles__bin, position);
    loadZX7compressedBGColors(font__palette__bin, position); 
}

void wait(unsigned char duration) {

    for(unsigned char wait=0; wait < duration; ++wait) {
        waitForVBlank();
    }
}

void print_tile(unsigned char x, unsigned char y, unsigned short tileno) {   
    setNextTileatXY(x, y);
    setTile(tileno);
}
#endif

void print_str(unsigned char x, unsigned char y, char *str, short offset) {
    for(; *str; ++str) {
        if (x >= TEXTCONSOLE_MAX_X)
            ++y, x=0;
        print_tile(x++, y, *str + offset);
    }
}

void print_num(unsigned char x, unsigned char y, long num, short offset) {
    char buffer[10+1]; 
    char *str; 

    str = buffer;
    SEGA_itoa(num, buffer);
    
    for(; *str; ++str) {
        if (x >= TEXTCONSOLE_MAX_X)
            ++y, x=0;
        print_tile(x++, y, *str + offset);
    }
}
