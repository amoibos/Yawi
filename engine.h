#ifndef ENGINE_H
#define ENGINE_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"

#include "data.h"
#include "views.h"

void load_font(void);

char load_leveldata(const unsigned char no, Leveldata * level);
_Bool in_field(const signed char x, const signed char y);
_Bool is_border(const signed char x, const signed char y);
long get_levelcode(const unsigned char level);
void update_statusline(Leveldata * level); 
void print_title(unsigned char * title);
void setup_level(Leveldata * level);
_Bool is_pushing_object(Leveldata * level, Direction dir);
void gameloop(unsigned char curr_level, unsigned char demo_mode);
signed int get_checked_tile(signed char x, signed char y);
void check_for_changes(Position * motion_objects, Position * source);
void timer(void);
void print_playtime(void); 

static unsigned char timer_enabled = 0;
static unsigned int seconds = 0;
static unsigned char fps = 0;

#endif