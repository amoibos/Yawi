#include "global.h"
#include "engine.h"


#ifdef PLATFORM_SMS
SMS_EMBED_SDSC_HEADER_AUTO_DATE_16KB(1, 0, Darktrym, Yawi, "puzzle game")
#elif PLATFORM_SG
#endif

void main(void) {
    displayOn();
    setFrameInterruptHandler(timer);
    intro_screen(GAME_NAME);
}
