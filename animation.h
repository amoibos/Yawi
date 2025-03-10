#ifndef ANIMATION_H
#define ANIMATION_H

#include "engine.h"
#include "animationdata.h"

//Press a key to continue

void animate_quarterly(Screens screen);
SpriteData get_sprite_data(signed short data);

void init_sprite_position(unsigned char color);
void add_ball_sprite(void);
void update_sprites_falling(void);

//extern unsigned char INTRO_SPRITE[1][3];
//extern unsigned char CREDITS_SPRITE[23][4];
//extern unsigned char DEATH_SPRITE[1][4];

#endif
