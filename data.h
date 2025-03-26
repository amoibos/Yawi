#ifndef DATA_H
#define DATA_H

#include "localization.h"

#define BANK_FONT                   ((1))
#define BANK_LEVELS                 ((2))
#define BANK_GFX                    ((3))
#define BANK_AUDIO                  ((4))

#define VERSION_STRING              "0.9.3"

#ifdef DEMO
#define VERSION                     "DEMO " VERSION_STRING
#else
#define VERSION                     VERSION_STRING
#endif

#define SEARCH_FREE_SLOT            ((0))
#define SEARCH_OCCUPIED_SLOT        ((1))

// array of positions which store next items
// that should be investigate if it trigger something
#define MAX_EXPLOSIVE_ITEM          ((10))
#define MAX_MOTION_ITEMS            ((40))
#define MAX_TELEPORTER              ((5))

#define TITLE_LINE                  ((0))

#define PROGRESS_LINE               ((1))
#define STATUS_LINE                 ((22))
#define INFO_LINE                   ((23))

#define LEVEL_HEIGHT                ((18))
#define OFFSET_MAP_Y                ((3))
#define OFFSET_MAP_X                ((0))

#define EXIT_SYMBOL                 (('E'))
#define PLAYER1_SYMBOL_LEFT         (('@'))
#define PLAYER1_SYMBOL_RIGHT        (('A'))

//use for more colors for player figure 
#define PLAYER1_SYMBOL_LEFT_BODY    ((PLAYER1_SYMBOL_LEFT + 3))
#define PLAYER1_SYMBOL_LEFT_REST    ((PLAYER1_SYMBOL_LEFT_BODY + 6))
#define PLAYER1_SYMBOL_RIGHT_BODY   ((PLAYER1_SYMBOL_RIGHT + 3))
#define PLAYER1_SYMBOL_RIGHT_REST   ((PLAYER1_SYMBOL_RIGHT_BODY + 6))

#define DOPPELGANGER_SYMBOL         (('O'))
#define GOLD_SYMBOL                 (('$'))
//level 1-12 do not use this
#define TELEPORTER_SYMBOL           (('#'))

#define ROCKETL_SYMBOL              (('U'))
#define ROCKETR_SYMBOL              (('W'))
//unused in original level set
#define ROCKETD_SYMBOL              (('X'))
#define ROCKETU_SYMBOL              (('V'))

#define WALL_UNDESTROY_SYMBOL       (('~'))
#define BLOCK_SYMBOL                (('B'))

#define EXPLOSIVE_SYMBOLS           "UVWXB"
#define ARROW_SYMBOLS                "<>^v"

// platforms with gravitation
#define PLATFORMU_SYMBOL            (('l'))
#define PLATFORMD_SYMBOL            (('m'))
#define PLATFORML_SYMBOL            (('n'))
#define PLATFORMR_SYMBOL            (('o'))

#define PLATFORM_SYMBOLS            (("lmno"))

//deadly water, do not enter
#define WATER_SYMBOL                (('&'))

//removable by player but also destroyable by falling rock
#define PEBBLE_SYMBOL               (('.'))
//removable by player
#define STONE_SYMBOL                ((':'))

#define ARROW_DOWN_SYMBOL            (('v'))
#define ARROW_UP_SYMBOL              (('^'))
#define ARROW_LEFT_SYMBOL            (('<'))
#define ARROW_RIGHT_SYMBOL           (('>'))

#define EMPTY_SYMBOL                ((' '))

#define BARRIER_SYMBOLS             (("+|-%~"))
#define MOVABLE_SYMBOLS             (("<>^vBUVWXlmno"))
#define BLOCKING_SYMBOLS            (("<>^v:$lmno"))

// used for rain/ball slideshow
#define BALL_SYMBOL                 (('\x07'))

#define MAX_INPUT_DIGIT             ((10))
#define BORDER_BRICK                ((33))

// for animation in intro screen, offset of 128 required because first tiles are used for image
#define PLAYER1_SYMBOL_LEFT_SHADOW  (('('))
#define PLAYER1_SYMBOL_RIGHT_SHADOW ((')'))
#define PEBBLE_SYMBOL_SHADOW        (('.')) 

#define PEBBLE_SYMBOL_INDEX         ((0))
#define PLAYER1_SYMBOL_RIGHT_INDEX  ((1))
#define PLAYER1_SYMBOL_LEFT_INDEX   ((2))

#define INTRO_FIGURE                (('#'))
#define INTRO_FIGURE_FRAMES         ((3))
#define INTRO_EMPTY                 ((128))

#define MAX_KEYS                     ((1))    


typedef enum eDirection {
    DirectionUndefined,
    DirectionRight,
    DirectionUp,
    DirectionLeft,
    DirectionDown,
    DirectionExit,
    DirectionHelp
} Direction;

typedef enum eStatus {
    StatusUndefinied,
    StatusAlive,
    StatusDied,
    StatusCompleted
} Status;

typedef enum eEffect {
    EffectNone,
    EffectSpiral
} Effect;

typedef struct sPosition {
    signed char x;
    signed char y;
} Position;

#define TILE_UNKNOWN                ((-1))
#define NO_MOTION                   ((-1))
#define END_STEP                    ((-2))

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
             
#define DEMO_START_AFTER_S          ((90))
#define COUNTER_STOPPED             ((-1))
#define DEMO_ABORT                  ((-2))

#define MAX_MOVE_DELAY              ((5))

#define MAX_STEP_SEQUENCE           ((64))
static const unsigned char LEVEL01_STEP_SEQUENCE[MAX_STEP_SEQUENCE] = { DirectionUp,
                                DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,
                                DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,
                                DirectionUp,DirectionUp,
                                DirectionRight,DirectionRight,DirectionRight,
                                DirectionDown,DirectionDown,
                                DirectionUp,DirectionUp,DirectionUp,
                                DirectionLeft,
                                DirectionUp,
                                DirectionLeft,DirectionLeft,DirectionLeft,DirectionLeft,DirectionLeft,DirectionLeft,
                                DirectionUp,DirectionUp,DirectionUp,
                                DirectionLeft,DirectionLeft,
                                DirectionUp,DirectionUp,
                                DirectionLeft,DirectionLeft,
                                DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,DirectionDown,
                                DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,DirectionRight,
                                DirectionLeft,DirectionLeft,DirectionLeft
                            };

#define MAX_LEVEL_NAME              ((25+1))

typedef struct sLeveldata {
    unsigned char name[MAX_LEVEL_NAME];
    unsigned char gold;
    unsigned char max_gold;
    signed char x;
    signed char y;
    Status status;
    Position teleport[MAX_TELEPORTER];
    unsigned char teleports_found;
} Leveldata;

#ifdef DEMO
#define MAX_LEVEL                   ((1))
#else
#define MAX_LEVEL                   ((12))
#endif

static const unsigned char * LEVEL_NAMES[MAX_LEVEL+1] = {
    LEVEL_NAME01,
#ifndef DEMO
    LEVEL_NAME02,
    LEVEL_NAME03,
    LEVEL_NAME04,
    LEVEL_NAME05,
    LEVEL_NAME06,
    LEVEL_NAME07,
    LEVEL_NAME08,
    LEVEL_NAME09,
    LEVEL_NAME10,
    LEVEL_NAME11,
    LEVEL_NAME12,
#endif
    ".."
};

typedef enum eMainMenu {
     MainMenuUndefined
    ,MainMenuNewGame
    ,MainMenuLevelSelect
   // ,MainMenuHelp
    ,MainMenuCredits
    //,MainMenuCongratulation
} MainMenu;

typedef enum eScreens {
     ScreenUndefined
    ,ScreenIntro
    ,ScreenHelp
    ,ScreenIngame
    ,ScreenNextLevel
    ,ScreenDeath
    ,ScreenCongratulation
    ,ScreenLevelSelect
    ,ScreenCredits
} Screens;

#define MAX_INTRO_ITEMS ((3))
static const unsigned char * INTRO_ITEMS[MAX_INTRO_ITEMS] = {
     MAIN_MENU_ITEM1
    ,MAIN_MENU_ITEM2
//    ,MAIN_MENU_ITEM3
    ,MAIN_MENU_ITEM4
};

#define CREDIT_NAMES_MAX ((3))
static const unsigned char * CREDIT_NAMES[CREDIT_NAMES_MAX] = {
    CREDITS_LINE01
    ,CREDITS_LINE02
    ,CREDITS_LINE03
};

typedef struct sSpriteData {
    unsigned char x;
    unsigned char y;
    signed char index;
} SpriteData;

typedef enum eMenuMode {
    MenuModeUndefined,
    MenuModeCenter,
    MenuModeLeft
} MenuMode;

#define MAX_LEVELCODE_FACTOR        ((5))

#define MAX_SPRITE                  ((32))

#define MAX_SPRITE_PER_LINE         ((4))

#define FRAME_RATE                  ((60))

#define OFFSET_SPRITE_Y             ((0))
#define OFFSET_SPRITE_X             ((1))
#define OFFSET_SPRITE_TILE          ((2))
#define OFFSET_SPRITE_ATTR          ((3))

#endif
