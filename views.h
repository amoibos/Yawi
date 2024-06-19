#ifndef VIEWS_H
#define VIEWS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"
#include "engine.h"

//#include "data.h"

void endscreen(char * menu_name);
void deathscreen(char * menu_name);
char menu(char **items, char amount, char start_line, enum eMenuMode mode, _Bool numbers, signed char textcolor);
void level_select(char * menu_name);
void intro(char * menu_name);

#endif