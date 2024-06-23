#ifndef VIEWS_H
#define VIEWS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"
#include "assets/city.h"
#include "engine.h"
#include "widgets.h"

void endscreen(unsigned char * menu_name);
void deathscreen(unsigned char * menu_name);
void next_level(unsigned char * menu_name, char level);
void level_select(unsigned char * menu_name);
void intro(unsigned char * menu_name);
void print_img(	const unsigned char *tiledata, const unsigned int tile_length, 
				const unsigned char *colordata, const unsigned int color_length, 
				const unsigned int width, const unsigned int height, const unsigned char left, const unsigned char top);
#endif