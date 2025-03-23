#ifndef VIEWS_H
#define VIEWS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"
#include "assets/images.h"
#include "engine.h"
#include "widgets.h"
#include "animationdata.h"

#include "PSGlib/PSGlib.h"
#include "assets/audio.h"

#define CENTER(x) ((TEXTCONSOLE_MAX_X >> 1) - (strlen(x) >> 1))

void congratulation_screen(const unsigned char * menu_name);
void death_screen(const unsigned char * menu_name);
void next_level_screen(const unsigned char * menu_name, char level);
unsigned char level_select_screen(const unsigned char * menu_name);
void intro_screen(const unsigned char * menu_name);
void credits_screen(const unsigned char * menu_name);
void help_screen(const unsigned char * menu_name);

#endif
