#ifndef ENGINE_H
#define ENGINE_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"


#include "location.h"
#include "data.h"
#include "views.h"

#include "localization.h"

void load_font(void);

char load_leveldata(const unsigned char no);
_Bool in_field(const signed char x, const signed char y);
_Bool is_border(const signed char x, const signed char y);
long get_levelcode(unsigned char level);
void update_statusline(void);
void print_title(unsigned char * title);
void reset_sprites(void);
void setup_level(void);
unsigned int get_default_tile(unsigned char x, unsigned char y);
_Bool is_pushing_object(Direction dir);
void wait(unsigned char duration);
void clear_sprites(void);
void extend_player_sprite(unsigned char player_figure, unsigned char current_x, unsigned char current_y);
void add_player_sprite(void);
void gameloop(unsigned char curr_level, _Bool demo_mode);
void add_animation(unsigned char x, unsigned char y);
signed int get_checked_tile(signed char x, signed char y);
void check_for_changes(Position * motion_objects, Position * source);
void timer(void);
void print_playtime(void);
void reset_time(signed char timer_on);

extern Leveldata level;
extern Location current_location;

/* timer functions */
extern char timer_enabled;
extern unsigned int seconds;
extern unsigned char fps;
extern unsigned int totaltime;


/* sprite handling */
extern SpriteData get_sprite_data(signed int data);
extern void animate_quarterly(Screens screen);
extern unsigned char animation_frame;
extern signed int all_sprites[MAX_SPRITE];
extern unsigned char sprites_no;
extern _Bool animation_refresh;

extern _Bool audio_enabled;
extern unsigned char previous_fps_seqment;
extern unsigned char INGAME_SPRITE[5][4];

extern unsigned char SpriteTable[MAX_SPRITE * 4];

extern void init_sprite_position(unsigned char color);
extern void add_ball_sprite(void);
extern void update_sprites_falling(void);

#endif
