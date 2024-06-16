#ifndef CONSOLE_H
#define CONSOLE_H


#define SCREEN_MAX_Y ((192 / 8))
#define SCREEN_MAX_X ((256 / 8))

void clear_screen(void);
void clear_line(unsigned char line);
void load_ascii_tiles(int position);

void displayOn();
void displayOff();
void waitForVBlank();

void print_tile(unsigned char x, unsigned char y, unsigned char tileno);

#ifdef PLATFORM_SMS
#define get_tile SMS_getTileatXY
#elif PLATFORM_SG
#define get_tile SG_mapROMBank
#endif 

#ifdef PLATFORM_SMS
#define mapROMBank SMS_mapROMBank
#elif PLATFORM_SG
#define mapROMBank SG_mapROMBank
#endif 


#endif