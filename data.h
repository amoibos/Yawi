#ifndef DATA_H
#define DATA_H

#include "localization.h"

#define BANK_FONT                   ((1))
#define BANK_LEVELS                 ((2))
#define BANK_GFX                    ((3))
#define BANK_AUDIO                  ((4))


#define VERSION_STRING              "0.8"

#ifdef DEMO
#define VERSION                     "DEMO " VERSION_STRING
#else
#define VERSION                     VERSION_STRING
#endif 

// array of positions which store next items
// that should be investigate if it trigger something
#define MAX_EXPLOSIVE_ITEM          ((10))
#define MAX_MOTION_ITEMS            ((10))
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
#define PLAYER2_SYMBOL              (('O'))
#define GOLD_SYMBOL                 (('$'))
//level 1-12 do not use this
#define TELEPORTER_SYMBOL           (('#'))


#define EXPLOSIVEX_SYMBOL           (('X'))
#define EXPLOSIVEW_SYMBOL           (('W'))
#define EXPLOSIVEY_SYMBOL           (('Y'))
#define EXPLOSIVEZ_SYMBOL           (('Z'))

#define TANK_SYMBOL                 (('B'))

#define EXPLOSIVE_SYMBOLS           "XWYZB"
#define ROCK_SYMBOLS                "<>^v"

#define SOMETHING6_SYMBOL           (('o'))
#define THORNS_SYMBOL               (('&'))

//removable by player but also destroyable by falling rock
#define PEBBLE_SYMBOL               (('.'))
//removable by player
#define STONE_SYMBOL                ((':'))

#define ROCK_DOWN_SYMBOL            (('v'))
#define ROCK_UP_SYMBOL              (('^'))
#define ROCK_LEFT_SYMBOL            (('<'))
#define ROCK_RIGHT_SYMBOL           (('>'))

#define EMPTY_SYMBOL                ((' '))

#define BARRIER_SYMBOLS             (("&+|-%~"))
#define MOVABLE_SYMBOLS             (("<>^vBoWXYZ"))
#define BLOCKING_SYMBOLS            (("<>^v:$")) 

typedef enum eDirection {
    DirectionUndefined,
    DirectionRight,
    DirectionUp,
    DirectionLeft,
    DirectionDown, 
    DirectionExit
} Direction;

typedef enum eStatus {
    StatusUndefinied,    
    StatusAlive,
    StatusDied,
    StatusCompleted
} Status;

typedef struct tPosition {
    signed char x;
    signed char y;
} Position;

#define DEMO_START_AFTER            ((90))
#define COUNTER_STOPPED             ((-1))
#define DEMO_ABORT                  ((-2))

#define MAX_MOVE_DELAY              ((5))

#define MAX_STEP_SEQUENCE           ((64))
static const unsigned char level01_step_sequence[MAX_STEP_SEQUENCE] = { DirectionUp, 
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

typedef struct leveldata_t {
    unsigned char name[MAX_LEVEL_NAME];
    unsigned char gold;
    unsigned char max_gold;
    signed char start_x;
    signed char start_y;
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

static const unsigned char * level_names[MAX_LEVEL+1] = {
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

#define MAX_INTRO_ITEMS ((3))
static const unsigned char * intro_items[MAX_INTRO_ITEMS] = {
    MAIN_MENU_ITEM1,
    MAIN_MENU_ITEM2,
    MAIN_MENU_ITEM3
};

#define CREDIT_NAMES_MAX ((3))
static const unsigned char * credits_names[CREDIT_NAMES_MAX] = {
    CREDITS_LINE01,
    CREDITS_LINE02,
    CREDITS_LINE03
};

typedef enum eMenuMode {
    MenuModeUndefined,
    MenuModeCenter,
    MenuModeLeft
} MenuMode;

typedef enum eInputType {
    InputTypeUndefined,
    InputTypeNumerical,
    InputTypeNumericalExtended,
    InputTypeAlphaNumerical
} InputType;

#define NUMERIC                     "0123456789"
#define EXTENDED_NUMERIC NUMERIC    "+-."
#define ALPHA_NUMERIC               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                                    "abcdefghijklmnopqrstuvwxyz"\
                                    EXTENDED_NUMERIC


#define MAX_LEVELCODE_FACTOR        ((5))

#define MAX_SPRITE                  ((32))

#define FRAME_RATE                  ((60))

#endif