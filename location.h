#ifndef LOCATION_H
#define LOCATION_H

typedef enum eLocation {
    LocationUnknown,
    LocationIntro,
    LocationLevelSelect,
    LocationInGame,
    LocationCredits,
    LocationEndscreen,
    LocationDeathscreen
} Location;

#endif
