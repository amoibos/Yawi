#ifndef WIDGETS_H
#define WIDGETS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "data.h"

unsigned char menu(unsigned char **items, unsigned char amount, unsigned char start_line, unsigned char offset, 
			MenuMode mode, _Bool numbers);
unsigned char * input(unsigned char x, unsigned char y, unsigned char * buffer, unsigned char size, InputType input_type);

#endif