#include "animation.h"

#define NDEBUG 1


SpriteData get_sprite_data(signed int data) {
	SpriteData retval = {0, 0, -1};

	retval.x = 	0x1F & (data >> 5);
	retval.y =  0x1F & data;
	retval.index = data >> 10;

	return retval;
}

void animate_quarterly(Screens screen) {

	animation_frame = (animation_frame + 1)  % 4;
	for(unsigned char pos=0; pos < sprites_no; ++pos)

		if (all_sprites[pos] != -1) {
			SpriteData sprite;
			sprite = get_sprite_data(all_sprites[pos]);
            unsigned char sprite_x = sprite.x;
            unsigned char sprite_y = sprite.y;
            signed char sprite_index = sprite.index;
			
#ifdef DEBUG 
			print_num(0, 0, sprite_x, 128);
			print_num(5, 0, sprite_y, 128);
			print_num(10, 0, sprite_index, 128);
			print_num(15, 0, all_sprites[pos], 128);
			print_num(20, 0, animation_frame, 128);
#endif

			switch(screen) {
				case ScreenIntro: {
					break;
				}
				case ScreenCredits: {
					print_tile(sprite_x, sprite_y, CREDITS_SPRITE[sprite_index][animation_frame] + 128);
					break;
				}
				case ScreenNextLevel: {
					break;
				}
				case ScreenCongratulation: {
					print_tile(sprite_x, sprite_y, CREDITS_SPRITE[sprite_index][animation_frame] + 128);
					break;
				}
				case ScreenLevelSelect: {
					break;
				}
				case ScreenIngame: {
					//print_num(25, 0, INGAME_SPRITE[sprite_index][animation_frame], 128);
					print_tile(sprite_x, sprite_y, INGAME_SPRITE[sprite_index][animation_frame]);	
					break;
				}
				case ScreenDeath: {
					//print_num(25, 0, DEATH_SPRITE[sprite_index][animation_frame], 128);
					print_tile(sprite_x, sprite_y, DEATH_SPRITE[sprite_index][animation_frame]);	
					break;
					}
				default:
					break;
			}
			//waitForVBlank();
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

        if (SpriteTable[idx +  OFFSET_SPRITE_Y] < (SCREEN_MAX_Y << 3))
            SpriteTable[idx +  OFFSET_SPRITE_Y] += 3;
        
        finalizeSprites();
        waitForVBlank();
        copySpritestoSAT(); 
        waitForVBlank();
    } 
}

void add_ball_sprite(void) {

    for(unsigned char pos=0; pos < MAX_SPRITE; ++pos) 
        SG_loadSpritePatterns(font__tiles__bin + BALL_SYMBOL * 8, pos, 8);
}

void init_sprite_position(void) {

    initSprites();       
    for(unsigned char row=0; row < MAX_SPRITE / MAX_SPRITE_PER_LINE; ++row) 
        for(unsigned char column=0; column < MAX_SPRITE_PER_LINE; ++column) {
            unsigned char pos = row * MAX_SPRITE_PER_LINE + column;
            unsigned char sprite_pos_x = (column << 6) - 1 + ((row % 2) == 0 ? 0 : MAX_SPRITE_PER_LINE << 3);
            unsigned char sprite_pos_y = row << 4;
            addSprite(sprite_pos_x, sprite_pos_y, pos, ((pos % 16) < 2 ? 15 : (pos % 16)));
    } 
}
