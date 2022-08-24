#include "vend.h"

#define MAX_VEND 		20
#define X 				0
#define Y				1

#define SW				41
#define SH				31
#define SC				13

static DATAFILE *vend_bmp = NULL;
static DATAFILE *items_bmp = NULL;

static int ps[MAX_VEND][2] = 

			{{95, 7},   {139, 7},  {183, 7}, {227, 7},
			{95, 49},  {139, 49}, {183, 49}, {227, 49},
			{95, 86}, {139, 86}, {183, 86}, {227, 86},
			{95, 125}, {139, 125}, {183, 125}, {227, 125},
			{95, 162}, {139, 162}, {183, 162}, {227, 162}};
			
static int ic[20] = 

			{0,				/* INV_NONE */
			 0,				/* INV MONEY */
			 20,			/* INV_DRDOUCHE */
			 15,			/* INV_NUGGETS */
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0,
			 0
			};

static int vv = FALSE;
static int vp = 0;
static int si = 0;

int
vend_buy_item (int *cash)
{
	if (*cash >= ic[si]){
		*cash -= ic[si];
		return si;
	}else
		return 0;
}

void
vend_move_up (void)
{
	if (vp > 3)
		vp -= 4;
}

void
vend_move_down (void)
{
	if (vp < 17)
		vp += 4;
}

void
vend_move_left (void)
{
	if (vp > 0 && vp > 4 && vp > 8 && vp > 12 && vp > 16)
		vp--;
}

void
vend_move_right (void)
{
	if (vp < 3 && vp < 7 && vp < 11 && vp < 15 && vp < 19)
		vp++;
}

int
vend_vis (void)
{
	return vv;
}

void
vend_draw_backbuff (VENDING *vend, BITMAP *bf)
{
	
	BITMAP *v = vend_bmp->dat;
	BITMAP *i = items_bmp->dat;
	int ci;
	
	blit (v, bf, 0, 0, 0, 0, v->w, v->h);
	
	
	for (ci = 0; ci < MAX_VEND; ci++)
		masked_blit (i, bf, vend->inv_list[ci]*SW, 0, ps[ci][X], ps[ci][Y], SW, SH);
	
	rect(bf, ps[vp][X], ps[vp][Y], ps[vp][X]+SW, ps[vp][Y]+SH, SC);
}

int
vend_init_dat (VENDING *v, char *dfn, char *vb, char *ib)
{
	
	vend_bmp = load_datafile_object (dfn, vb);
	
	if (!vend_bmp)
		return 0;
		
	items_bmp = load_datafile_object (dfn, ib);
	
	if (!items_bmp){
		unload_datafile_object (vend_bmp);
		vend_bmp = NULL;
		return -1;
	}
	
	si = v->inv_list[vp];
	vv = TRUE;
	
	return vv;
}

void
vend_free_dat (void)
{
	if (vend_bmp){
		unload_datafile_object (vend_bmp);
		vend_bmp = NULL;
	}
		
	if (items_bmp){
		unload_datafile_object (items_bmp);
		items_bmp = NULL;
	}
	
	vv = FALSE;
}
