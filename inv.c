#include <stdio.h>
#include <string.h>
#include "inv.h"
#include "tools.h"

#define MAX_ITEMBOX 		100
#define MAX_INV				8
#define SEL_W				52
#define SEL_H				43
#define SEL_C				14
#define SEL_R				13

#define X					0
#define Y					1

#define TEXT_MAX_W			18
#define TEXT_X				9
#define TEXT_Y				9

#define SCROLL_H			39
#define SCROLL_X			201
#define SCROLL_Y			89

#define INV_BAT_C_WEAPON	71
#define INV_BAT_C_HEAL		124
#define INV_BAT_C_GENERAL	60

typedef struct {
	
	int id;
	int num;

} INVITEM;

typedef struct {
	int b_pos;
	int focus;
	int active;
	
} BOXMENU;

typedef struct {
	
	int vis;
	int sp;
	int active;
	
} INVMENU;

static INVMENU invmenu = { .vis = FALSE, .sp = 0, .active = FALSE };
static BOXMENU boxmenu = { .b_pos = 0, .focus = FALSE, .active = FALSE};

static INVITEM itembox[MAX_ITEMBOX] = {{0}};
static INVITEM inv[MAX_INV] = {	{.id = INV_MONEY, .num = 700},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0}
							};

/* Source and dest for item box swap */
static INVITEM *src = NULL;

static DATAFILE *inv_bmp = NULL;
static DATAFILE *items_bmp = NULL;
static DATAFILE *itemsslot_bmp = NULL;
static DATAFILE *invsel_wav = NULL;
static DATAFILE *put_wav = NULL;
static DATAFILE *unput_wav = NULL;
static FONT *inv_fnt = NULL;

static int m_rest = 0;

/* Nate stats */
static int food = 100;
static int sleep = 100;
static int water = 100;

/* X and Y pos of sel rec */
static int pos_lookup[MAX_INV][2] = {{212, 5},	{264, 5},
									{212, 48},	{264, 48},
									{212, 91},	{264, 91},
									{212, 134}, {264, 134}};
									 
/* X and Y pos of inv placement */
static int ip_lookup[MAX_INV][2] = {{217, 15},	{269, 15},  
									{217, 58},	{269, 58},
									{217, 101}, {269, 101},
									{217, 144}, {269, 144}};
									
/* X and Y pos of inv count placement */
static int inp_lookup[MAX_INV][2] = {{261, 7},	{313, 7},  
									{261, 50},	{313, 50}, 
									{261, 93}, 	{313, 93},
									{261, 136}, {313, 136}};

static char *inv_list[INV_NUM][2] = {
	
	{"NOTHING", "LOL"},
	/* VALUE */
	{"NATE BUCKS", "Big daddy money"},
	
	/* FOOD */
	{"NUGGETS", "Yummy nuggets"},
	{"DR. DOUCHE", "Remember, DO THE DOUCHE! Don't forget to drink much SODA!"},
	
	/* Weapons */
	{"STABBIN' KNIFE", "KNIFE FOR STABBIN."}

};

int
get_food (void)
{
	return food;
}

int
get_sleep (void)
{
	return sleep;
}

int
get_water (void)
{
	return water;
}

void
adj_food (int adj)
{
	food += adj;
	
	if (food > 100)
		food = 100;
		
	if (food < 0)
		food = 0;
}

void
adj_sleep (int adj)
{
	sleep += adj;
	
	if (sleep > 100)
		sleep = 100;

	if (sleep < 0)
		sleep = 0;
}

void
adj_water (int adj)
{
	water += adj;
	
	if (water > 100)
		water = 100;

	if (water < 0)
		water = 0;
}

static void
text_area_draw (BITMAP *b, char *s, int x, int y)
{
	int fh = text_height (inv_fnt) + 1, xo = 0;
	char *token, *ts = NULL;
	
	ts = (char *) malloc (strlen (s) + 1);
	strcpy (ts, s);
	
	token = strtok (ts, " ");
	
	while (token){

		textout_ex(b, inv_fnt, token, x+xo, y, -1, -1);
		
		xo += text_length (inv_fnt, token) + 4;
		
		if (x+xo > 158){
			xo = 0;
			y += fh;
		}

		token = strtok (NULL, " ");
	}
	
	free (ts);
}

void
inv_draw_battle (BITMAP *bf)
{
	int i, x, y = 181, w = 20, h = 12;
	for (i = 0, x = 141; i < MAX_INV; i++, x += 20){
		
		if (inv[i].id >= INV_KNIFE){
			rectfill (bf, x, y, x+w, y+h, INV_BAT_C_WEAPON);
		}else if (inv[i].id >= INV_NUGGETS){
			rectfill (bf, x, y, x+w, y+h, INV_BAT_C_HEAL);
		}else if (inv[i].id >= INV_MONEY)
			rectfill (bf, x, y, x+w, y+h, INV_BAT_C_GENERAL);
	}
}

void
invmenu_draw_backbuff (BITMAP *bf)
{ 	
	if (invmenu.vis){

		int px = pos_lookup[invmenu.sp][X], py = pos_lookup[invmenu.sp][Y];
		int ci;
		
		/* Draw inv menu */		
		blit ((BITMAP *)inv_bmp->dat, bf, 0, 0, 0, 0, 320, 200);
		
		/* Draw stats */
		rectfill (bf, 24, 143, 24 + water, 154, 167);
		rectfill (bf, 24, 161, 24 + food, 172, 85);
		rectfill (bf, 24, 179, 24 + sleep, 190, 184);
		
		
		/* Draw items */
		for (ci = 0; ci < MAX_INV; ci++){
			if (inv[ci].id){
				masked_blit ((BITMAP *)items_bmp->dat, bf, inv[ci].id * 41, 0, ip_lookup[ci][X], ip_lookup[ci][Y], 41, 31);
				textprintf_right_ex (bf, inv_fnt, inp_lookup[ci][X], inp_lookup[ci][Y], -1, -1, "%d", inv[ci].num);
			}
		}
	
		/* Draw inventory select */
		if (!boxmenu.focus)
			rect(bf, px, py, px + SEL_W, py + SEL_H, SEL_C);
		
		/* When inv was actived at item box */
		if (boxmenu.active){
			
			BITMAP *bt = itemsslot_bmp->dat;
			int p1, p2, p3;
			
			p1 = boxmenu.b_pos;
			
			if (p1 == 0){
				p2 = 99;
				p3 = 1;
			}else if (p1 == 99){
				p2 = 98;
				p3 = 0;
			}else{
				p2 = p1 - 1;
				p3 = p1 + 1;
			}
			
			blit (bt, bf, 0, 0, 9, 89, bt->w, bt->h);
			blit (bt, bf, 0, 0, 9, 89+13, bt->w, bt->h);
			blit (bt, bf, 0, 0, 9, 89+26, bt->w, bt->h);
			textprintf_ex (bf, inv_fnt, 11, 89+3, -1, -1, "%-30s", inv_list[itembox[p2].id][0]);
			textprintf_ex (bf, inv_fnt, 11, 89+3+13, -1, -1, "%-30s", inv_list[itembox[p1].id][0]);
			textprintf_ex (bf, inv_fnt, 11, 89+3+26, -1, -1, "%-30s", inv_list[itembox[p3].id][0]);
	
			if (itembox[p2].num)
				textprintf_right_ex (bf, inv_fnt, 197, 89+3, -1, -1, "%d", itembox[p2].num);
				
			if (itembox[p1].num)
				textprintf_right_ex (bf, inv_fnt, 197, 89+3+13, -1, -1, "%d", itembox[p1].num);
				
			if (itembox[p3].num)
				textprintf_right_ex (bf, inv_fnt, 197, 89+3+26, -1, -1, "%d", itembox[p3].num);
	
			/* Item box selected item yellow rect */
			rect(bf, 9, 102, 9+bt->w - 2, 102+bt->h, 3);
			
			/* SCROLL BAR HEIGHT * (ITEMBOX_POS - SCROLL_POS_H) / MAX_ITEMBOX_SLOTS */
			
			ci = SCROLL_H*(boxmenu.b_pos - 2) / MAX_ITEMBOX;
			
			/* Itembox scroll bar */
			rectfill(bf, SCROLL_X, SCROLL_Y+ci, SCROLL_X+4, SCROLL_Y+ci+2, 15);
		}
		
		if (!inv[invmenu.sp].id){
			rectfill(bf, 9, 9, 200, 88, 48);
		}else{
			
			char *name = inv_list[inv[invmenu.sp].id][0];
			char *desc = inv_list[inv[invmenu.sp].id][1];
			
			textprintf_ex (bf, inv_fnt, 14, 14, -1, -1, "%s:", name);
			text_area_draw (bf, desc, 14, 24);
		}
	
		if (m_rest)
			m_rest--;
	}
}

int
boxmenu_active (void)
{
	return boxmenu.active;
}

void
boxmenu_set_active (int t)
{
	boxmenu.active = t;
}

void
boxmenu_set_src_dest (void)
{
	int t_num, t_id;
	
	if (!m_rest){
		if (!src){
			if (boxmenu.focus){
				src = &itembox[boxmenu.b_pos];
				boxmenu.focus = FALSE;
			}else{
				boxmenu.focus = TRUE;
				src = &inv[invmenu.sp];
			}
			play_sample ((SAMPLE *)put_wav->dat, 155, 128, 1000, 0);
			
		}else{
			t_id = src->id;
			t_num = src->num;
			
			if (boxmenu.focus){
				inv[invmenu.sp].id = itembox[boxmenu.b_pos].id;
				inv[invmenu.sp].num = itembox[boxmenu.b_pos].num;
				
				itembox[boxmenu.b_pos].id = t_id;
				itembox[boxmenu.b_pos].num = t_num;
				
			}else{
				itembox[boxmenu.b_pos].id = inv[invmenu.sp].id;
				itembox[boxmenu.b_pos].num = inv[invmenu.sp].num;
				
				inv[invmenu.sp].id = t_id;
				inv[invmenu.sp].num = t_num;
			}
			play_sample ((SAMPLE *)unput_wav->dat, 155, 128, 1000, 0);
			src = NULL;
		}
		
		m_rest = SEL_R;
	}
}

void
inv_item_use (void)
{
	int used = FALSE;
	
	if (!m_rest){
		switch (inv[invmenu.sp].id)
		{
			case INV_NUGGETS: if (food < 100) {adj_food (5); used = TRUE;} break;
			case INV_DRDOUCHE: if (water < 100) {adj_water (5); used = TRUE;} break;
		}
		
		if (used)
			inv[invmenu.sp].num--;
			
		if (inv[invmenu.sp].num <= 0){
			inv[invmenu.sp].num = 0;
			inv[invmenu.sp].id = 0;
		}
		
		m_rest = 100;
	}
}

void
invmenu_sel_up (void)
{
	if (!m_rest && invmenu.vis){
		
		if (boxmenu.focus){
			
			boxmenu.b_pos--;
			play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, 0);
			m_rest = SEL_R;
			if (boxmenu.b_pos < 0)
				boxmenu.b_pos = MAX_ITEMBOX - 1;
		}else{
			
			if (invmenu.sp > 1){
				invmenu.sp -= 2;
				play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, 0);
				m_rest = SEL_R;
			}
		}
	}
}

void
invmenu_sel_down (void)
{
	if (!m_rest && invmenu.vis){
		
		if (boxmenu.focus){
			
			m_rest = SEL_R;
			boxmenu.b_pos++;
			play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, 0);
			if (boxmenu.b_pos > MAX_ITEMBOX - 1)
				boxmenu.b_pos = 0;

		}else{
			if (invmenu.sp < 6){
				play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, 0);
				invmenu.sp += 2;
				m_rest = SEL_R;
			}
		}
	}
}

void
invmenu_sel_right (void)
{
	if (!m_rest && invmenu.vis){

		if ((invmenu.sp == 0 || invmenu.sp == 2 || invmenu.sp == 4 || invmenu.sp == 6) && !boxmenu.focus){
			play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, 0);
			invmenu.sp++;
		}else if (boxmenu.active && boxmenu.focus && !src){
			boxmenu.focus = FALSE;
		}
		
		m_rest = SEL_R;
	}
}

void
invmenu_sel_left (void)
{
	if (!m_rest && invmenu.vis){
		if ((invmenu.sp == 1 || invmenu.sp == 3 || invmenu.sp == 5 || invmenu.sp == 7) && !boxmenu.focus){
			play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, 0);
			invmenu.sp--;
		}else if (boxmenu.active && !src){
			boxmenu.focus = TRUE;
		}
		
		m_rest = SEL_R;
	}
}

int
invmenu_vis (void)
{
	return invmenu.vis;
}

void
invmenu_init (char *dfn)
{
	char *names[] = {"INV_FNT", NULL};
	
	inv_bmp = load_datafile_object (dfn, "INVMENU_BMP");
	items_bmp = load_datafile_object (dfn, "ITEMS_BMP");
	inv_fnt = load_dat_font (dfn, NULL, names);
	invsel_wav = load_datafile_object (dfn, "INVSEL_WAV");
	put_wav = load_datafile_object (dfn, "PUT_WAV");
	unput_wav = load_datafile_object (dfn, "UNPUT_WAV");
	itemsslot_bmp = load_datafile_object (dfn, "ITEMSSLOT_BMP");

	invmenu.vis = TRUE;
}

void
invmenu_free (void)
{
	if (inv_bmp)
		unload_datafile_object (inv_bmp);
		
	if (items_bmp)
		unload_datafile_object (items_bmp);
		
	if (inv_fnt)
		destroy_font (inv_fnt);
		
	if (itemsslot_bmp)
		unload_datafile_object (itemsslot_bmp);
		
	if (invsel_wav)
		unload_datafile_object (invsel_wav);
		
	if (put_wav)
		unload_datafile_object (put_wav);
		
	if (unput_wav)
		unload_datafile_object (unput_wav);

	inv_bmp = NULL;
	items_bmp = NULL;
	inv_fnt = NULL;
	invsel_wav = NULL;
	put_wav = NULL;
	unput_wav = NULL;
	itemsslot_bmp = NULL;
	
	boxmenu.active = FALSE;
	boxmenu.focus = FALSE;
	invmenu.vis = FALSE;
	invmenu.sp = 0;
	
	src = NULL;
	
	m_rest = 0;
}

int
inv_get_item_total (int id)
{
	int ci, t;
	
	for (ci = 0, t = 0; ci < MAX_INV; ci++)
		if (id == inv[ci].id)
			t += inv[ci].num;
			
	return t;
}

int
inv_sub (int id, int amt)
{
	int ci, t;
	
	/* Check if enough across all inv spaces */
	for (ci = 0, t = 0; ci < MAX_INV; ci++){
		
		if (inv[ci].id == id)
			t += inv[ci].num;
	}
	
	if (t < amt)
		return -1;

	for (ci = 0; ci < MAX_INV && amt; ci++){
		if (inv[ci].id == id){
			
			if (inv[ci].num >= amt){
				inv[ci].num -= amt;
				t -= amt;
				amt = 0;
			}else{
				amt -= inv[ci].num;
				t -= inv[ci].num;
				inv[ci].num = 0;
			}		
		}
		
		if (!inv[ci].num)
			inv[ci].id = INV_NONE;
	}
	
	return t;
}

int
inv_add (int id, int amt)
{
	int i;
	for (i = 0; i < MAX_INV; i++){
		
		if (inv[i].id == id){
			inv[i].num += amt;
			return 1;
		}
	}
	
	for (i = 0; i < MAX_INV; i++){
		
		if (inv[i].id == INV_NONE){
			inv[i].id = id;
			inv[i].num = amt;
			return 1;
		}
	}
	
	return 0;
}

int
inv_get_item_id (int pos)
{
	return inv[pos].id;
}

int *
inv_get_item_amount (int pos)
{
	return &(inv[pos].num);
}
