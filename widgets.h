#ifndef WIDGETS_H
#define WIDGETS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "location.h"
#include "data.h"


unsigned char menu(unsigned char **items, unsigned char amount, unsigned char start_line, unsigned char offset, 
			MenuMode mode, _Bool ShowNumbers);
unsigned char * input(unsigned char x, unsigned char y, unsigned char * buffer, unsigned char size, InputType input_type);
void print_img_compressed(	const unsigned char *tiledata,
				const unsigned char *colordata,
				const unsigned short width, const unsigned char height, const unsigned char left, const unsigned char top, Effect effect);
extern char timer_enabled;
extern unsigned short seconds;
extern unsigned char fps;
extern Location current_location;

#endif