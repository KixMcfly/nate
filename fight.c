#include "fight.h"

#define FIGHT_CHANCE_MAX 1000

static int fight_chance = 1;
static int fight = FALSE;
static char ret_room[100];
static int ret_x, ret_y;
static BITMAP *gui = NULL;
static int fire_pos = 320 / 2;
static int fire_pos_dir = -1;
static int fire_pos_spd_set = 0;
static int fire_pos_spd = 0;

int
fighting (void)
{
	return fight;
}

void
fight_set_ret_room (char *room, int x, int y)
{
	strcpy (ret_room, room);
	ret_x = x;
	ret_y = y;
}

char *
fight_get_ret_room (int *x, int *y)
{
	*x = ret_x;
	*y = ret_y;
	
	return ret_room;
}

void
fight_chance_reset (void)
{
	fight_chance = 0;
}

int
fight_chance_inc (int da)
{
	int rn;
	
	fight_chance += da;
	if (fight_chance > FIGHT_CHANCE_MAX)
		fight_chance = FIGHT_CHANCE_MAX;
		
	rn = rand() % (FIGHT_CHANCE_MAX + 1);
	if ( rn < fight_chance){
		DATAFILE *t_dat = load_datafile_object ("NATE.dat", "BAT_GUI_BMP");
		BITMAP *t_bmp = t_dat->dat;
		
		gui = create_bitmap (t_bmp->w, t_bmp->h);
		blit (t_bmp, gui, 0, 0, 0, 0, t_bmp->w, t_bmp->h);
		unload_datafile_object (t_dat);
		
		fight = TRUE;
		fight_chance = 1;
	}
	
	/* log_print ("RAND NUM: %d FIGHT CHANCE: %d\n", rn, fight_chance); */

	return fight;
}

void
fight_draw_stuff (BITMAP *bf)
{
	if (fire_pos < 5)
		fire_pos_dir = 1;
		
	if (fire_pos > 315)
		fire_pos_dir = -1;
		
	fire_pos += fire_pos_dir;
	
	if (--fire_pos_spd < 0){
		fire_pos_spd = fire_pos_spd_set;
	}
	
	blit (gui, bf, 0, 0, 0, 165, gui->w, gui->h);
	rectfill (bf, fire_pos-2, 170, fire_pos+2, 170+10, 2);
}

void
fight_free_stuff (void)
{
	destroy_bitmap (gui);
	gui = NULL;
}
