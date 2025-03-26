#ifndef WIDGETS_H
#define WIDGETS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "location.h"
#include "data.h"

typedef enum eInputType {
    InputTypeUndefined,
    InputTypeNumerical,
    InputTypeNumericalExtended,
    InputTypeAlphaNumerical
} InputType;

#define NUMERIC                     "0123456789"
#define EXTENDED_NUMERIC NUMERIC    "+-."
#define ALPHA_NUMERIC               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                                    "abcdefghijklmnopqrstuvwxyz"\
                                    EXTENDED_NUMERIC


unsigned char menu(unsigned char **items, unsigned char amount, unsigned char start_line, unsigned char offset, 
			MenuMode mode, _Bool ShowNumbers);
_Bool pressed_anything(void);
unsigned char * input(unsigned char x, unsigned char y, unsigned char * buffer, unsigned char size, InputType input_type, unsigned short offset);
void print_img_compressed(	const unsigned char *tiledata,
				const unsigned char *colordata,
				const unsigned short width, const unsigned char height, const unsigned char left, const unsigned char top, Effect effect);
void print_window_borders(unsigned char left, unsigned char top, unsigned char width, unsigned char height, unsigned short tileno);

extern char timer_enabled;
extern unsigned short seconds;
extern unsigned char fps;
extern _Bool animation_refresh; 
extern Location current_location;


extern void SetTimerCallback(void (*theHandlerFunction)(Screens)) __z88dk_fastcall;

#endif