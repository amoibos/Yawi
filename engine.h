#ifndef ENGINE_H
#define ENGINE_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"

#include "data.h"
#include "views.h"

void load_font(void);

char load_leveldata(const char no, Leveldata * level);
_Bool in_Field(const signed char x, const signed char y);
_Bool is_border(const signed char x, const signed char y);
long levelcode(char level);
void update_statusline(Leveldata * level); 
void setup_level(Leveldata * level);
_Bool level_completed(Leveldata * level);
_Bool is_pushing_object(Leveldata * level, Direction dir);
void gameloop(unsigned char curr_level);

#endif