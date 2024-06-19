#ifndef CONSOLE_H
#define CONSOLE_H

#ifdef PLATFORM_SMS
#include "../src/SMSlib.h"
#endif

#ifdef PLATFORM_SG
#include "../src/SGlib.h"
#include "../assets/assets.h"
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
void waitForVBlank(void);

#ifdef PLATFORM_SMS
#define get_tile SMS_getTileatXY
#elif PLATFORM_SG
#define get_tile SG_getTileatXY
#endif 

#ifdef PLATFORM_SMS
#define mapROMBank SMS_mapROMBank
#elif PLATFORM_SG
#define mapROMBank SG_mapROMBank
#endif 

#ifdef PLATFORM_SMS
#define keypressed SMS_getKeysPressed 
#elif PLATFORM_SG
#define keypressed SG_getKeysPressed
#endif 

#ifdef PLATFORM_SMS
#define readkey SMS_getKeysStatus
#elif PLATFORM_SG
#define readkey SG_getKeysStatus
#endif 

void print_tile(unsigned char x, unsigned char y, unsigned char tileno);
void print_str(unsigned char x, unsigned char y, char *str, int offset);
void print_num(unsigned char x, unsigned char y, long num, int offset);

#endif