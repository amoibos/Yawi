#ifndef GLOBAL_H
#define GLOBAL_H

#include "data.h"
#include "location.h"


// for pausing the timer and animation
extern char timer_enabled;
// counting passed seconds
extern unsigned short seconds;
// helper for counting seconds;
extern unsigned char fps;
//sum of level records
extern unsigned short totaltime;
//player position
extern Location current_location;
//current level state
extern Leveldata level;

// current animation frame for a sprite
extern unsigned char animation_frame;
// save tile index where the sprite is located
extern signed short all_sprites[MAX_SPRITE];
// update sprite state
extern _Bool animation_refresh;
// current number of sprites
extern unsigned char sprites_no;

extern _Bool audio_enabled;
extern unsigned char previous_fps_seqment;

//typedef void (*funcptr)(Screens screen);
//void (funcptr) timer_callback;

extern signed char xvel;
extern signed char yvel;

#endif
