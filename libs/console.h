#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef PLATFORM_SMS
#include "../SMSlib/SMSlib.h"
//#endif
#else
//#ifdef PLATFORM_SG
#include "../SGlib/SGlib.h"
#include "../assets/font.h"
#endif

#include "../data.h"
#include "../libs/strings.h"


#define SCREEN_MAX_X    ((256))
#define SCREEN_MAX_Y    ((192))

#define TEXTCONSOLE_MAX_X        ((32))
#define TEXTCONSOLE_MAX_Y        ((24))


void clear_screen(void);
void clear_line(unsigned char line);
void load_ascii_tiles(unsigned short position);


#ifdef PLATFORM_SMS
#define mapROMBank(x) SMS_mapROMBank(x)
#else //elif PLATFORM_SG || PLATFORM_SC
	#ifdef ALLOW_MAPPER
		#define mapROMBank(x) SG_mapROMBank(x)
	#else
		#define mapROMBank(x)	/**/
	#endif
#endif


#ifdef PLATFORM_SMS
#define loadTiles SMS_loadTiles
#define setNextTileatXY SMS_setNextTileatXY
#define setTile SMS_setTile
#define getKeysHeld SMS_setTile
#define displayOn SMS_displayOn
#define displayOff SMS_displayOff
#define get_tile SMS_getTileatXY
#define keypressed SMS_getKeysPressed
#define keyreleased SMS_getKeysReleased
#define readkey SMS_getKeysStatus
#define waitForVBlank SMS_waitForVBlank
#define setFrameInterruptHandler SMS_setFrameInterruptHandler
#define initSprites SMS_initSprites
#define addSprite SMS_addSprite
#define finalizeSprites SMS_finalizeSprites
#define loadZX7compressedTiles SMS_decompressZX7toVRAM
#define copySpritestoSAT SMS_copySpritestoSAT
#else //elif PLATFORM_SG || PLATFORM_SC
#define loadTiles SG_loadTilePatterns
#define loadPalette SG_loadTileColours
#define setNextTileatXY SG_setNextTileatXY
#define setTile SG_setTile
#define getKeysHeld SG_getKeysHeld
#define displayOn SG_displayOn
#define displayOff SG_displayOff
#define get_tile SG_getTileatXY
#define keypressed SG_getKeysPressed
#define keyreleased SG_getKeysReleased
#define readkey SG_getKeysStatus
#define scanKeyboardJoypad	SG_scanKeyboardJoypad
#define getKeyboardJoypadPressed SG_getKeyboardJoypadPressed
#define getKeyboardJoypadStatus SG_getKeyboardJoypadStatus
#define waitForVBlank SG_waitForVBlank
#define setFrameInterruptHandler SG_setFrameInterruptHandler
#define initSprites SG_initSprites
#define addSprite SG_addSprite
#define finalizeSprites SG_finalizeSprites
#define loadZX7compressedTiles SG_loadZX7compressedBGTiles
#define copySpritestoSAT SG_copySpritestoSAT
#define loadZX7compressedBGColors SG_loadZX7compressedBGColors
#endif

void wait(unsigned char duration);

void print_tile(unsigned char x, unsigned char y, unsigned short tileno);
void print_str(unsigned char x, unsigned char y, char *str, short offset);
void print_num(unsigned char x, unsigned char y, long num, short offset);

#endif
