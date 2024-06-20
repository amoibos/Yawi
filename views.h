#ifndef VIEWS_H
#define VIEWS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"
#include "assets/city.h"
#include "engine.h"
#include "widgets.h"

void endscreen(char * menu_name);
void deathscreen(char * menu_name);
void next_level(char * menu_name, char level);
void level_select(char * menu_name);
void intro(char * menu_name);
void print_img(	const char *tiledata, unsigned int tile_length, 
				const char *colordata, unsigned int color_length, 
				int width, int height, char left, char top);
#endif