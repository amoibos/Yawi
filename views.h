#ifndef VIEWS_H
#define VIEWS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"
#include "assets/images.h"
#include "engine.h"
#include "widgets.h"

#include "PSGlib/PSGlib.h"
#include "assets/audio.h"

#define CENTER(x) ((SCREEN_MAX_X >> 1) - (strlen(x) >> 1))

void endscreen(const unsigned char * menu_name);
void deathscreen(const unsigned char * menu_name);
void next_level(const unsigned char * menu_name, char level);
void level_select(const unsigned char * menu_name);
void intro(const unsigned char * menu_name);
void credits(const unsigned char * menu_name);
void print_img(	const unsigned char *tiledata, const unsigned int tile_length, 
				const unsigned char *colordata, const unsigned int color_length, 
				const unsigned int width, const unsigned int height, const unsigned char left, const unsigned char top);

#endif