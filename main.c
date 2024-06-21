#include "engine.h"

#ifdef PLATFORM_SMS
SMS_EMBED_SDSC_HEADER_AUTO_DATE_16KB(1,0,Darktrym,Yawen,"puzzle game")
#elif PLATFORM_SG

#endif

void main(void) {
    displayOn(); 
    intro(GAME_NAME);
}