#ifndef ENGINE_H
#define ENGINE_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"

#include "data.h"

char load_leveldata(const char no, Leveldata * level);
char is_border(const signed char x, const signed char y);
void update_statusline(Leveldata * level); 
void setup_level(Leveldata * level);
void endscreen(void);
void deathscreen(void);

#endif