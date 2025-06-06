#ifndef ENGINE_H
#define ENGINE_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"

#include "location.h"
#include "data.h"
#include "views.h"

#include "localization.h"
#include "animationdata.h"

// used to priotize search direction
#define MOTION_UP_OFFSET            ((00))
#define MOTION_DOWN_OFFSET          ((10))
#define MOTION_RIGHT_OFFSET         ((20))
#define MOTION_LEFT_OFFSET          ((30))

typedef enum eObjectMove {
    ObjectMoveUndefined=0,
    ObjectMoveSouth=    0+1,
    ObjectMoveNorth=    0+1+5,
    ObjectMoveEast=     0+1+5+5,
    ObjectMoveWest=     0+1+5+5+5
} ObjectMove;

#define LUT_CENTER                  {0,  0}
#define LUT_UP                      {0, -1}
#define LUT_DOWN                    {0,  1}
#define LUT_RIGHT                   {1,  0}
#define LUT_LEFT                    {-1, 0}
#define LUT_END                     {END_STEP, END_STEP}

// layout defines offsets of eObjectMove values, stop pair indicate end of check sequence
static const Position MOTION_CHECKS[] = {
                                                         LUT_END
     ,LUT_CENTER, LUT_UP,           LUT_RIGHT, LUT_LEFT, LUT_END
     ,LUT_CENTER,         LUT_DOWN, LUT_RIGHT, LUT_LEFT, LUT_END
     ,LUT_CENTER, LUT_UP, LUT_DOWN,            LUT_LEFT, LUT_END
     ,LUT_CENTER, LUT_UP, LUT_DOWN, LUT_RIGHT,           LUT_END
};

void load_font(void);

char load_leveldata(const unsigned char no);
_Bool in_field(const signed char x, const signed char y);
_Bool is_border(const signed char x, const signed char y);
long get_levelcode(unsigned char level);
void update_statusline(void);
void print_title(unsigned char * title);
void setup_level(void);
unsigned short get_default_tile(unsigned char x, unsigned char y, Location location);
_Bool is_pushing_object(Direction dir);
inline ObjectMove get_object_move(Direction dir);
void gameloop(unsigned char curr_level, _Bool demo_mode);
signed short get_checked_tile(signed char x, signed char y);
void check_for_changes(Position * motion_objects, Position * source, ObjectMove move);
inline Direction get_Direction(Position *pos);
Direction get_input(_Bool* demo_mode, unsigned char * demo_pos);
void timer(void);
void print_playtime(void);
void reset_time(signed char timer_on);

void clear_sprites(void);
void extend_player_sprite(unsigned char player_figure, unsigned char current_x, unsigned char current_y);
void add_player_sprite(void);
void add_animation(unsigned char x, unsigned char y);
void reset_sprites(void);

extern Leveldata level;
extern Location current_location;

/* timer functions */
extern char timer_enabled;
extern unsigned short seconds;
extern unsigned char fps;
extern unsigned short totaltime;

/* sprite handling */
extern inline signed short set_sprite_data(unsigned char sprite, unsigned char x, unsigned char y);
extern SpriteData get_sprite_data(signed short data);
extern void animate_quarterly(Screens screen);
extern unsigned char animation_frame;
extern signed short all_sprites[MAX_SPRITE];
extern unsigned char sprites_no;
extern _Bool animation_refresh;

extern _Bool audio_enabled;
extern unsigned char previous_fps_seqment;

extern unsigned char SpriteTable[MAX_SPRITE * 4];

extern void init_sprite_position(unsigned char color);
extern void add_ball_sprite(void);
extern void update_sprites_falling(void);



#endif
