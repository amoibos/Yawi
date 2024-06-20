#ifndef DATA_H
#define DATA_H

#define GAME_NAME (("Yawen"))
#define VERSION_STRING "v0.6"

#ifdef DEMO
#define VERSION "DEMO " VERSION_STRING
#else
#define VERSION VERSION_STRING
#endif 

#define MAX_TELEPORTER ((5))

#define TITLE_LINE ((0))

#define PROGRESS_LINE ((21))
#define STATUS_LINE ((22))
#define INFO_LINE ((23))

#define LEVEL_HEIGHT ((20))
#define OFFSET_MAP ((1))

#define EXIT_SYMBOL (('E'))
#define PLAYER_SYMBOL (('@'))
#define GOLD_SYMBOL (('$'))
//level 1-12 do not use this
#define TELEPORTER_SYMBOL (('#'))

#define SOMETHING1_SYMBOL (('X'))
#define SOMETHING2_SYMBOL (('8'))
#define SOMETHING3_SYMBOL (('W'))
#define SOMETHING4_SYMBOL (('o'))


//removable by player and coverable by a stone
#define PEBBLE_SYMBOL (('.'))
//removable by player
#define STONE_SYMBOL ((':'))

#define ROCK_DOWN_SYMBOL (('v'))
#define ROCK_UP_SYMBOL (('^'))
#define ROCK_LEFT_SYMBOL (('<'))
#define ROCK_RIGHT_SYMBOL (('>'))

#define EMPTY_SYMBOL ((' '))

#define BARRIER_SYMBOL (("&O+|-%"))
#define MOVABLE_SYMBOL (("<>^v8oWXYZ"))


typedef enum eDirection {
    DirectionRight,
    DirectionUp,
    DirectionLeft,
    DirectionDown, 
    DirectionUndefined
} Direction;

typedef enum eStatus {
  ALIVE,
  DIED,
  COMPLETED,
  UNDEFINED_STATUS
} Status;

typedef struct position_t {
    signed char x;
    signed char y;
} Position;

//Position steps[4] = { (0, -1), (0, 1), (-1, 0), (1, 0)};

typedef struct leveldata_t {
    char name[25+1];
    char gold;
    char max_gold;
    signed char start_x;
    signed char start_y;
    signed char exit_x;
    signed char exit_y;
    signed char x;
    signed char y;
    Status status;
    Position teleport[MAX_TELEPORTER];
    char teleport_found;
} Leveldata;

#define MAX_LEVEL ((12))
static char *level_names[MAX_LEVEL] = {
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
static char *intro_items[MAX_INTRO_ITEMS] = {
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
    InputTypeAlphaNumerical,
    InputTypeUndefined
} InputType;

#define NUMERICSTRING "-0123456789."
#define ALPHANUMERICAL "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                       "abcdefghijklmnopqrstuvwxyz"\
                       NUMERICSTRING


#endif