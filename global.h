#ifndef GLOBAL_H
#define GLOBAL_H

#include "data.h"
#include "location.h"


// for pausing the timer and animation
char timer_enabled;
// counting passed seconds
unsigned short seconds;
// helper for counting seconds;
unsigned char fps;
//sum of level records
unsigned short totaltime=0;
//player position
Location current_location;
//current level state
Leveldata level;

// current animation frame for a sprite
unsigned char animation_frame;
// save tile index where the sprite is located
signed short all_sprites[MAX_SPRITE];
// update sprite state
_Bool animation_refresh;
// current number of sprites
unsigned char sprites_no;

_Bool audio_enabled=1;
unsigned char previous_fps_seqment=0;

//typedef void (*funcptr)(Screens screen);
//void (funcptr) timer_callback;

#endif
