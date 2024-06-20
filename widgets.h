#ifndef WIDGETS_H
#define WIDGETS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "data.h"

char menu(char **items, char amount, char start_line, char offset, 
			MenuMode mode, _Bool numbers);
char * input(char x, char y, char * buffer, char size, InputType input_type);

#endif