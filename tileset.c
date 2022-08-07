#include "tileset.h"

int
load_tileset (TILESET *ts, char *dat_fn, char *dat_id, int tw, int th)
{
	DATAFILE *df;
	int wit, hit, x, y, ct;
	BITMAP *tmp;
	
	df = load_datafile_object (dat_fn, dat_id);
	if (!df){
		return 0;
	}
	
	tmp = (BITMAP *)df->dat;
	
	/* tiles per row */
	wit = tmp->w / tw;
	hit = tmp->h / th;
	
	/* Assign number of tiles in tileset */
	ts->nt = wit * hit;
	
	/* Get tileset list this map uses */
	ts->tiles = (BITMAP **)malloc (sizeof (BITMAP *) * (wit * hit));
	
	if (ts->nt == NULL)
		return -1;
	
	for (y = 0, ct = 0; y < hit; y++){
		for (x = 0; x < wit; x++, ct++ ){
			ts->tiles[ct] = create_bitmap (tw, th);
			blit (tmp, ts->tiles[ct], x*tw, y*th, 0, 0, tw, th);
		}
	}
	
	unload_datafile_object (df);

	return 1;
}

