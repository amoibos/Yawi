#ifndef DATA_H
#define DATA_H

//#define GAME_NAME (("Yawi"))
#define VERSION_STRING "v0.6"

#ifdef DEMO
#define VERSION "DEMO " VERSION_STRING
#else
#define VERSION VERSION_STRING
#endif 

#define MAX_MOTION_ITEMS ((20))
#define MAX_TELEPORTER ((5))

#define TITLE_LINE ((0))

#define PROGRESS_LINE ((21))
#define STATUS_LINE ((22))
#define INFO_LINE ((23))

#define LEVEL_HEIGHT ((18))
#define OFFSET_MAP_Y ((2))
#define OFFSET_MAP_X ((0))


#define EXIT_SYMBOL (('E'))
#define PLAYER1_SYMBOL (('@'))
#define PLAYER2_SYMBOL (('O'))
#define GOLD_SYMBOL (('$'))
//level 1-12 do not use this
#define TELEPORTER_SYMBOL (('#'))


#define BOMB1_SYMBOL (('X'))
#define BOMB2_SYMBOL (('W'))
#define BOMB3_SYMBOL (('Y'))
#define BOMB4_SYMBOL (('Z'))

#define TANK_SYMBOL (('B'))

#define BOMBS_SYMBOL "XWYZB"


#define SOMETHING6_SYMBOL (('o'))
#define THORNS_SYMBOL (('&'))

//removable by player but also destroyable by falling rock
#define PEBBLE_SYMBOL (('.'))
//removable by player
#define STONE_SYMBOL ((':'))

#define ROCK_DOWN_SYMBOL (('v'))
#define ROCK_UP_SYMBOL (('^'))
#define ROCK_LEFT_SYMBOL (('<'))
#define ROCK_RIGHT_SYMBOL (('>'))

#define EMPTY_SYMBOL ((' '))

#define BARRIER_SYMBOL (("&+|-%~"))
#define MOVABLE_SYMBOL (("<>^vBoWXYZ"))
#define BLOCKING_SYMBOL (("<>^v:@$")) 

typedef enum eDirection {
    DirectionRight,
    DirectionUp,
    DirectionLeft,
    DirectionDown, 
    DirectionUndefined
} Direction;

typedef enum eStatus {
  StatusAlive,
  StatusDied,
  StatusCompleted,
  StatusUndefinied
} Status;

typedef struct tPosition {
    signed char x;
    signed char y;
} Position;

//Position steps[4] = { (0, -1), (0, 1), (-1, 0), (1, 0)};

typedef struct leveldata_t {
    unsigned char name[25+1];
    unsigned char gold;
    unsigned char max_gold;
    signed char start_x;
    signed char start_y;
    signed char exit_x;
    signed char exit_y;
    signed char x;
    signed char y;
    Status status;
    Position teleport[MAX_TELEPORTER];
    unsigned char teleport_found;
} Leveldata;

#define MAX_LEVEL ((12))
static unsigned char *level_names[MAX_LEVEL] = {
    "Darkness Falls",
    "Lateral Thinking",
    "Explosively Simple",
    "Question of Substance",
    "Ethereal Structures",
    "Order of Magnitude",
    "Seventh Heaven",
    "House of Cards",
    "Complexity",
    "Nature of the Beast",
    "Devil\'s Despair",
    "Shrouded Glory"
};

#define MAX_INTRO_ITEMS ((2))
static unsigned char *intro_items[MAX_INTRO_ITEMS] = {
    "New Game",
    "Level Select"
};


typedef enum eMenuMode {
    MenuModeCenter,
    MenuModeLeft,
    MenuModeUndefined
} MenuMode;

typedef enum eInputType {
    InputTypeNumerical,
    InputTypeNumericalExtended,
    InputTypeAlphaNumerical,
    InputTypeUndefined
} InputType;

#define NUMERIC "0123456789"
#define EXTENDED_NUMERIC NUMERIC "+-."
#define ALPHA_NUMERIC   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                        "abcdefghijklmnopqrstuvwxyz"\
                        EXTENDED_NUMERIC


#define MAX_LEVELCODE_FACTOR ((3))
#endif