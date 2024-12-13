#ifndef ANIMATION_H
#define ANIMATION_H

#include "engine.h"

unsigned char INGAME_SPRITE[5][4] = {
		/*EXIT*/	 {69, 70, 71, 72}
		/*Y*/		,{89, 97, 119, 105}
		/*a*/		,{97, 119, 105, 89}
		/*w*/		,{119, 105, 89, 97}
		/*i*/		,{105, 89, 97, 119}
};

unsigned char DEATH_SPRITE[1][4] = {
		/*#3*/		 {3, 4, 3, 4}
};

void animate_quarterly(Screens screen);
SpriteData get_sprite_data(signed int data);

#endif