#ifndef DATA_H
#define DATA_H

#define GAME_NAME (("Yawen 1.0"))

#define MAX_LEVEL ((1))
#define STATUS_LINE ((22))
#define INFO_LINE ((23))
#define LEVEL_HEIGHT ((20))
#define OFFSET_MAP ((1))

#define EXIT_SYMBOL (('E'))
#define PLAYER_SYMBOL (('@'))
#define GOLD_SYMBOL (('$'))

#define PEBBLE_SYMBOL (('.'))
#define STONE_SYMBOL ((':'))

#define ROCK_DOWN_SYMBOL (('v'))
#define ROCK_UP_SYMBOL (('^'))
#define ROCK_LEFT_SYMBOL (('<'))
#define ROCK_RIGHT_SYMBOL (('>'))

#define BARRIER_SYMBOL (("+|-"))
#define EMPTY_SYMBOL ((' '))


enum Status {
  ALIVE,
  DIED,
  COMPLETED
};

typedef struct leveldata_t {
    char name[20+1];
    char max_gold;
    char start_x;
    char start_y;
    char exit_x;
    char exit_y;
    char x;
    char y;
    char gold;
    enum Status status;
} Leveldata;


#endif