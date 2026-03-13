#include "animation.h"

const unsigned char INGAME_SPRITE[5][4] = {
    {69,70,71,72},{89,97,119,105},{97,119,105,89},{119,105,89,97},{105,89,97,119}
};
const unsigned char DEATH_SPRITE[1][4] = { {3,4,5,4} };
const unsigned char INTRO_SPRITE[1][3] = {
    {PEBBLE_SYMBOL_SHADOW, PLAYER1_SYMBOL_RIGHT_SHADOW, PLAYER1_SYMBOL_LEFT_SHADOW}
};
const unsigned char CREDITS_SPRITE[23][4] = {
    {80,0,80,0},{114,0,114,0},{101,0,101,0},{115,0,115,0},{115,0,115,0},
    {32,0,32,0},{97,0,97,0},{32,0,32,0},{107,0,107,0},{101,0,101,0},
    {121,0,121,0},{32,0,32,0},{116,0,116,0},{111,0,111,0},{32,0,32,0},
    {99,0,99,0},{111,0,111,0},{110,0,110,0},{116,0,116,0},{105,0,105,0},
    {110,0,110,0},{117,0,117,0},{101,0,101,0}
};

SpriteData get_sprite_data(signed short data) {
	SpriteData retval = {0, 0, -1};

	retval.x = 	0x1F & (data >> 5);
	retval.y =  0x1F & data;
	retval.index = data >> 10;

	return retval;
}

void animate_quarterly(Screens screen) {

	animation_frame = (animation_frame + 1)  % 4;
	for(unsigned char pos=0; pos < sprites_no; ++pos)

		if (all_sprites[pos] != NO_MOTION) {
			SpriteData sprite;
			sprite = get_sprite_data(all_sprites[pos]);

			switch(screen) {
				case ScreenIntro: {
					//get current player figure and overwrite current position
					print_tile(sprite.x, sprite.y, INTRO_EMPTY);
					
					if (sprite.x == TEXTCONSOLE_MAX_X - 1) {
						return; 	
					}
					
					++sprite.x;

					// save player position and draw player figure
					all_sprites[pos] = set_sprite_data(sprite.index, sprite.x, sprite.y);
					print_tile(sprite.x, sprite.y, (sprite.x % INTRO_FIGURE_FRAMES) + INTRO_FIGURE + 128);
					break;
				}
				case ScreenCredits: {
					print_tile(sprite.x, sprite.y, CREDITS_SPRITE[sprite.index][animation_frame] + 128);
					break;
				}
				case ScreenNextLevel: {
					break;
				}
				case ScreenCongratulation: {
					print_tile(sprite.x, sprite.y, CREDITS_SPRITE[sprite.index][animation_frame] + 128);
					break;
				}
				case ScreenLevelSelect: {
					break;
				}
				case ScreenIngame: {
					print_tile(sprite.x, sprite.y, INGAME_SPRITE[sprite.index][animation_frame]);
					break;
				}
				case ScreenDeath: {
					//TODO: V motion
					print_tile(sprite.x, sprite.y, DEATH_SPRITE[sprite.index][animation_frame]);
					break;
				}
				default:
					break;
			}
			
		}
		waitForVBlank();
}


void update_sprites_falling(void) {
    for(unsigned char pos=0; pos < MAX_SPRITE; ++pos) {
        unsigned char idx;

        idx = pos << 2;
        unsigned char y =       SpriteTable[idx + OFFSET_SPRITE_Y];
        unsigned char x =       SpriteTable[idx + OFFSET_SPRITE_X];
        unsigned char tile =    SpriteTable[idx + OFFSET_SPRITE_TILE];
        unsigned char attr =    SpriteTable[idx + OFFSET_SPRITE_ATTR];

        if (SpriteTable[idx +  OFFSET_SPRITE_Y] < (SCREEN_MAX_Y))
            SpriteTable[idx +  OFFSET_SPRITE_Y] += 1;


    }
    finalizeSprites();
    waitForVBlank();
    copySpritestoSAT();
    waitForVBlank();
}

void add_ball_sprite(void) {

    for(unsigned char pos=0; pos < MAX_SPRITE; ++pos)
        SG_loadSpritePatterns(font__tiles__bin + BALL_SYMBOL * 8, pos, 8);
}
