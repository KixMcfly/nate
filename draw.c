#include "draw.h"

static BITMAP *backbuff = NULL;
static int fd = TRUE;

void
show_backbuff (int x, int y)
{
	blit (backbuff, screen, 0, 0, x, y, backbuff->w, backbuff->h);
}

BITMAP *
get_backbuff (void)
{
	return backbuff;
}

int
init_backbuff (int w, int h)
{
	backbuff = create_bitmap (w, h);
	
	if (!backbuff)
		return 0;
		
	return 1;
}

int
free_backbuff (void)
{
	if (!backbuff)
		return 0;
	
	destroy_bitmap (backbuff);
	backbuff = NULL;
	
	return 1;
}

int
set_pal (char *dfn, char *pn)
{
	DATAFILE *df = load_datafile_object (dfn, pn);
	
	if (!df)
		return 0;
		
	set_palette (df->dat);
	
	unload_datafile_object (df);
	
	return 1;
}

int
draw_map_layer (MAP *m, int ln, int x, int y)
{
	int lt = map_get_layer_type (m, ln);
	BITMAP *img;

	switch (lt){
		case TILES:
			img = map_get_layer_img (m, ln);

			if (!img){
				img = NULL;
				return 0;
			}
			clear (backbuff);
			blit (img, backbuff, 0, 0, x, y, img->w, img->h);
		break;
		case OBJECTS:
		
		break;
	}
	
	img = NULL;
	
	return 1;
}

int faded (void)
{
	return fd;
}

void
fadein (void *pal, int spd)
{
	fade_in (pal, spd);
	fd = FALSE;
}

void
fadeout (int spd)
{
	fade_out (spd);
	fd = TRUE;
}
