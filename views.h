#ifndef VIEWS_H
#define VIEWS_H

#include "libs/console.h"
#include "libs/strings.h"
#include "assets/levels.h"
#include "assets/images.h"
#include "engine.h"
#include "widgets.h"
#include "animationdata.h"

#include "PSGlib/PSGlib.h"
#include "assets/audio.h"

#define CENTER(x) ((TEXTCONSOLE_MAX_X >> 1) - (strlen(x) >> 1))

#ifdef DEMO
#define MAX_LEVEL                   ((1))
#else
#define MAX_LEVEL                   ((12))
#endif

typedef enum eMainMenu {
     MainMenuUndefined
    ,MainMenuNewGame
    ,MainMenuLevelSelect
   // ,MainMenuHelp
    ,MainMenuCredits
    //,MainMenuCongratulation
} MainMenu;

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

void congratulation_screen(const unsigned char * menu_name);
void death_screen(const unsigned char * menu_name);
void next_level_screen(const unsigned char * menu_name, char level);
unsigned char level_select_screen(const unsigned char * menu_name);
void intro_screen(const unsigned char * menu_name);
void credits_screen(const unsigned char * menu_name);
void help_screen(const unsigned char * menu_name);

#endif
