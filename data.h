#ifndef DATA_H
#define DATA_H

#define GAME_NAME (("Yawen"))
#define VERSION "version 0.6"

#define MAX_TELEPORTER ((5))

#define TITLE_LINE ((0))
#define STATUS_LINE ((22))
#define INFO_LINE ((23))

#define LEVEL_HEIGHT ((20))
#define OFFSET_MAP ((1))

#define EXIT_SYMBOL (('E'))
#define PLAYER_SYMBOL (('@'))
#define GOLD_SYMBOL (('$'))
#define TELEPORTER_SYMBOL (('#'))

#define SOMETHING1_SYMBOL (('X'))
#define SOMETHING2_SYMBOL (('&'))
#define SOMETHING3_SYMBOL (('W'))


//removable by player and coverable by a stone
#define PEBBLE_SYMBOL (('.'))
//removable by player
#define STONE_SYMBOL ((':'))

#define ROCK_DOWN_SYMBOL (('v'))
#define ROCK_UP_SYMBOL (('^'))
#define ROCK_LEFT_SYMBOL (('<'))
#define ROCK_RIGHT_SYMBOL (('>'))

#define EMPTY_SYMBOL ((' '))

#define BARRIER_SYMBOL (("+|-"))
#define MOVABLE_SYMBOL (("<>^v"))


typedef enum eDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UNDEFINED_DIRECTION
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
    char name[20+1];
    char max_gold;
    signed char start_x;
    signed char start_y;
    signed char exit_x;
    signed char exit_y;
    signed char x;
    signed char y;
    char gold;
    Status status;
    Position teleport[MAX_TELEPORTER];
    char teleport_found;
} Leveldata;

#define MAX_LEVEL ((12))
static char *level_names[] = {
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
static char *intro_items[] = {
    "New Game",
    "Level Select"
};


typedef enum eMenuMode {
    MenuModeCenter,
    MenuModeLeft,
    MenuModeUndefined
} MenuMode;

#endif