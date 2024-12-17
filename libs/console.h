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


#define SCREEN_MAX_RES_Y    ((192))
#define SCREEN_MAX_RES_X    ((256))

#define SCREEN_MAX_Y        ((24))
#define SCREEN_MAX_X        ((32))

void clear_screen(void);
void clear_line(unsigned char line);
void load_ascii_tiles(int position);

void displayOn(void);
void displayOff(void);

#ifdef PLATFORM_SMS
#define loadTiles SMS_loadTiles
#else //elif PLATFORM_SG || PLATFORM_SC
#define loadTiles SG_loadTilePatterns
#endif

#ifdef PLATFORM_SMS
#define loadTiles SMS_loadBGPalette
#else //elif PLATFORM_SG || PLATFORM_SC
#define loadPalette SG_loadTileColours
#endif

#ifdef PLATFORM_SMS
#define setNextTileatXY SMS_setNextTileatXY
#else //elif PLATFORM_SG || PLATFORM_SC
#define setNextTileatXY SG_setNextTileatXY
#endif

#ifdef PLATFORM_SMS
#define setTile SMS_setTile
#else //elif PLATFORM_SG || PLATFORM_SC
#define setTile SG_setTile
#endif

#ifdef PLATFORM_SMS
#define getKeysHeld SMS_setTile
#else //elif PLATFORM_SG || PLATFORM_SC
#define getKeysHeld SG_getKeysHeld
#endif

#ifdef PLATFORM_SMS
#define displayOn SMS_displayOn
#else //elif PLATFORM_SG || PLATFORM_SC
#define displayOn SG_displayOn
#endif

#ifdef PLATFORM_SMS
#define displayOff SMS_displayOff
#else //elif PLATFORM_SG || PLATFORM_SC
#define displayOff SG_displayOff
#endif

#ifdef PLATFORM_SMS
#define get_tile SMS_getTileatXY
#else //elif PLATFORM_SG || PLATFORM_SC
#define get_tile SG_getTileatXY
#endif

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
#define keypressed SMS_getKeysPressed
#else //elif PLATFORM_SG || PLATFORM_SC
#define keypressed SG_getKeysPressed
#endif

#ifdef PLATFORM_SMS
#define readkey SMS_getKeysStatus
#else //elif PLATFORM_SG || PLATFORM_SC
#define readkey SG_getKeysStatus
#endif

#ifdef PLATFORM_SMS
#define waitForVBlank SMS_waitForVBlank
#else //elif PLATFORM_SG || PLATFORM_SC
#define waitForVBlank SG_waitForVBlank
#endif

#ifdef PLATFORM_SMS
#define setFrameInterruptHandler SMS_setFrameInterruptHandler
#else //elif PLATFORM_SG || PLATFORM_SC
#define setFrameInterruptHandler SG_setFrameInterruptHandler
#endif

#ifdef PLATFORM_SMS
#define initSprites SMS_initSprites
#else //elif PLATFORM_SG || PLATFORM_SC
#define initSprites SG_initSprites
#endif

#ifdef PLATFORM_SMS
#define addSprite SMS_addSprite
#else //elif PLATFORM_SG || PLATFORM_SC
#define addSprite SG_addSprite
#endif

#ifdef PLATFORM_SMS
#define finalizeSprites SMS_finalizeSprites
#else //elif PLATFORM_SG || PLATFORM_SC
#define finalizeSprites SG_finalizeSprites
#endif

#ifdef PLATFORM_SMS
#define copySpritestoSAT SMS_copySpritestoSAT
#else //elif PLATFORM_SG || PLATFORM_SC
#define copySpritestoSAT SG_copySpritestoSAT
#endif


void print_tile(unsigned char x, unsigned char y, unsigned int tileno);
void print_str(unsigned char x, unsigned char y, char *str, int offset);
void print_num(unsigned char x, unsigned char y, long num, int offset);

#endif
