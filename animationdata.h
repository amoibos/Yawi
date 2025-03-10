#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H

#include "data.h"

static const unsigned char INGAME_SPRITE[5][4] = {
    /*EXIT*/     {69, 70, 71, 72}
    /*Y*/       ,{89, 97, 119, 105}
    /*a*/       ,{97, 119, 105, 89}
    /*w*/       ,{119, 105, 89, 97}
    /*i*/       ,{105, 89, 97, 119}
};

static const unsigned char DEATH_SPRITE[1][4] = {
    /*#3*/       {3, 4, 5, 4}
};

static const unsigned char INTRO_SPRITE[1][3] = {
                 {PEBBLE_SYMBOL_SHADOW, PLAYER1_SYMBOL_RIGHT_SHADOW, PLAYER1_SYMBOL_LEFT_SHADOW}
};


static const unsigned char CREDITS_SPRITE[23][4] = {
    /*P*/        {80,  0, 80,  0}
    /*r*/       ,{114, 0, 114, 0}
    /*e*/       ,{101, 0, 101, 0}
    /*s*/       ,{115, 0, 115, 0}
    /*s*/       ,{115, 0, 115, 0}
    /* */       ,{32,  0, 32,  0}
    /*a*/       ,{97,  0, 97,  0}
    /* */       ,{32,  0, 32,  0}
    /*k*/       ,{107, 0, 107, 0}
    /*e*/       ,{101, 0, 101, 0}
    /*y*/       ,{121, 0, 121, 0}
    /* */       ,{32,  0, 32,  0}
    /*t*/       ,{116, 0, 116, 0}
    /*o*/       ,{111, 0, 111, 0}
    /* */       ,{32,  0, 32,  0}
    /*c*/       ,{99,  0, 99,  0}
    /*o*/       ,{111, 0, 111, 0}
    /*n*/       ,{110, 0, 110, 0}
    /*t*/       ,{116, 0, 116, 0}
    /*i*/       ,{105, 0, 105, 0}
    /*n*/       ,{110, 0, 110, 0}
    /*u*/       ,{117, 0, 117, 0}
    /*e*/       ,{101, 0, 101, 0}
};

#endif