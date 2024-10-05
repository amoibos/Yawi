#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef PLATFORM_SMS
#include "../src/SMSlib.h"
#endif

#ifdef PLATFORM_SG
#include "../src/SGlib.h"
#include "../assets/font.h"
#endif

#include "../data.h"
#include "../libs/strings.h"


#define SCREEN_MAX_Y ((192 / 8))
#define SCREEN_MAX_X ((256 / 8))

void clear_screen(void);
void clear_line(unsigned char line);
void load_ascii_tiles(int position);

void displayOn(void);
void displayOff(void);

#ifdef PLATFORM_SMS
#define loadTiles SMS_loadTiles
#elif PLATFORM_SG || PLATFORM_SC
#define loadTiles SG_loadTilePatterns
#endif 

#ifdef PLATFORM_SMS
#define loadTiles SMS_loadBGPalette
#elif PLATFORM_SG || PLATFORM_SC
#define loadPalette SG_loadTileColours
#endif 

#ifdef PLATFORM_SMS
#define setNextTileatXY SMS_setNextTileatXY
#elif PLATFORM_SG || PLATFORM_SC
#define setNextTileatXY SG_setNextTileatXY
#endif 

#ifdef PLATFORM_SMS
#define setTile SMS_setTile
#elif PLATFORM_SG || PLATFORM_SC
#define setTile SG_setTile
#endif

#ifdef PLATFORM_SMS
#define getKeysHeld SMS_setTile
#elif PLATFORM_SG || PLATFORM_SC
#define getKeysHeld SG_getKeysHeld
#endif
 

#ifdef PLATFORM_SMS
#define displayOn SMS_displayOn
#elif PLATFORM_SG || PLATFORM_SC
#define displayOn SG_displayOn
#endif 

#ifdef PLATFORM_SMS
#define displayOff SMS_displayOff
#elif PLATFORM_SG || PLATFORM_SC
#define displayOff SG_displayOff
#endif 

#ifdef PLATFORM_SMS
#define get_tile SMS_getTileatXY
#elif PLATFORM_SG || PLATFORM_SC
#define get_tile SG_getTileatXY
#endif 

#ifdef PLATFORM_SMS
#define mapROMBank(x) SMS_mapROMBank(x)
#elif PLATFORM_SG || PLATFORM_SC
	#ifdef ALLOW_MAPPER
		#define mapROMBank(x) SG_mapROMBank(x)
	#else
		#define mapROMBank(x)	/**/
	#endif
#endif 

#ifdef PLATFORM_SMS
#define keypressed SMS_getKeysPressed 
#elif PLATFORM_SG || PLATFORM_SC
#define keypressed SG_getKeysPressed
#endif 

#ifdef PLATFORM_SMS
#define readkey SMS_getKeysStatus
#elif PLATFORM_SG || PLATFORM_SC
#define readkey SG_getKeysStatus
#endif 

#ifdef PLATFORM_SMS
#define waitForVBlank SMS_waitForVBlank 
#elif PLATFORM_SG || PLATFORM_SC
#define waitForVBlank SG_waitForVBlank 
#endif 



void print_tile(unsigned char x, unsigned char y, unsigned int tileno);
void print_str(unsigned char x, unsigned char y, char *str, int offset);
void print_num(unsigned char x, unsigned char y, long num, int offset);

#endif