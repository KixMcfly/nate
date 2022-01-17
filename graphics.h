#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <allegro.h>

#define SPR_UP 1
#define SPR_DOWN 0
#define SPR_LEFT 2
#define SPR_RIGHT 3

typedef struct {
	
	int num_r, num_c;
	BITMAP ***dat;
	
} Sprite;

extern PALETTE cur_pal;
extern BITMAP *back_buff;

void blit_sprite( Sprite *spr, BITMAP *d, int x, int y, int r, int c );
Sprite *new_sprite( void );
void load_sprite( Sprite *spr, char *bmp_name, int w, int h );
void destroy_sprite( Sprite *spr );


#endif
