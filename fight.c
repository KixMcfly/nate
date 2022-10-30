#include "fight.h"

#define FIGHT_CHANCE_MAX 1000

enum {
	FIGHT_ITEM_SEL_1,
	FIGHT_ITEM_SEL_2,
	FIGHT_ITEM_SEL_3,
	FIGHT_ITEM_SEL_4,
	FIGHT_ITEM_SEL_5,
	FIGHT_ITEM_SEL_6,
	FIGHT_ITEM_SEL_7,
	FIGHT_ITEM_SEL_8
};

static int fight_chance = 1;
static int fight = FALSE;
static char ret_room[100];
static int ret_x, ret_y;
static BITMAP *gui = NULL;
static SPRITE *inv = NULL;
static int fire_pos = 320 / 2;
static int fire_pos_dir = -1;
static int fire_pos_spd_set = 0;
static int fire_pos_spd = 0;
static int fight_inv_sel = FALSE;
static int fight_inv_sel_pos = FIGHT_ITEM_SEL_1;

int
fighting (void)
{
	return fight;
}

void
fight_inv_sel_set (int bool)
{
	fight_inv_sel = bool;
}

int
fight_inv_is_sel (void)
{
	return fight_inv_sel;
}

int
fight_inv_get_sel (void)
{
	return fight_inv_sel_pos;
}

void
fight_inv_sel_right (void)
{
	fight_inv_sel_pos++;
	
	if (fight_inv_sel_pos > FIGHT_ITEM_SEL_8)
		fight_inv_sel_pos = 0;
}

void
fight_inv_sel_left (void)
{
	fight_inv_sel_pos--;
	
	if (fight_inv_sel_pos < 0)
		fight_inv_sel_pos = FIGHT_ITEM_SEL_8;
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
		
		/* Load battle GUI */
		DATAFILE *t_dat = load_datafile_object ("NATE.dat", "BAT_GUI_BMP");
		BITMAP *t_bmp = t_dat->dat;
		gui = create_bitmap (t_bmp->w, t_bmp->h);
		blit (t_bmp, gui, 0, 0, 0, 0, t_bmp->w, t_bmp->h);
		unload_datafile_object (t_dat);
		
		/* Load inv for battle view */
		inv = sprite_new ();
		sprite_keyframe_dat_div (inv, 7, 1, "NATE.dat", "ITEMS_BMP");
		
		fight = TRUE;
		fight_chance = 1;
	}
	
	/* log_print ("RAND NUM: %d FIGHT CHANCE: %d\n", rn, fight_chance); */

	return fight;
}

void
fight_draw_stuff (BITMAP *bf, int id, int *item_amt)
{
	if (fire_pos < 5)
		fire_pos_dir = 1;
		
	if (fire_pos > 315)
		fire_pos_dir = -1;
		
	fire_pos += fire_pos_dir;
	
	if (--fire_pos_spd < 0)
		fire_pos_spd = fire_pos_spd_set;
	
	blit (gui, bf, 0, 0, 0, 160, gui->w, gui->h);
	rectfill (bf, fire_pos-2, 170, fire_pos+2, 170+10, 2);
	
	if (fight_inv_sel){
		sprite_draw (inv, bf, 0, id, 100, 100);
	}

}
  
void
fight_free_stuff (void)
{
	destroy_bitmap (gui);
	sprite_free (inv);
	gui = NULL;
}
