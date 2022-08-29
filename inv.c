#include "inv.h"

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

typedef struct {
	
	int id;
	int num;

} INVITEM;

typedef struct {
	int b_pos;
	int s_pos;
	int focus;
	int s_active;
	int active;
	
} BOXMENU;

typedef struct {
	
	int vis;
	int sp;
	int active;
	
} INVMENU;

static INVMENU invmenu = { .vis = FALSE, .sp = 0, .active = FALSE };
static BOXMENU boxmenu = { .s_pos = 0, .b_pos = 0, .focus = FALSE,
						   .s_active = FALSE, .active = FALSE};

static INVITEM itembox[MAX_ITEMBOX] = {{0}};
static INVITEM inv[MAX_INV] = {	{.id = INV_MONEY, .num = 120},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0},
								{.id = INV_NONE, .num = 0}
							};

static DATAFILE *inv_bmp = NULL;
static DATAFILE *items_bmp = NULL;
static DATAFILE *itemsslot_bmp = NULL;
static DATAFILE *invsel_wav = NULL;
static FONT *inv_fnt = NULL;

static int m_rest = 0;

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
	{"DR. DOUCHE", "Remember, DO THE DOUCHE! Don't forget to drink much SODA!"}

};

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
invmenu_draw_backbuff (BITMAP *bf)
{ 
	
	int px = pos_lookup[invmenu.sp][X], py = pos_lookup[invmenu.sp][Y];
	int sx = pos_lookup[boxmenu.s_pos][X], sy = pos_lookup[boxmenu.s_pos][Y];
	int ci;
	
	/* Draw inv menu */		
	blit ((BITMAP *)inv_bmp->dat, bf, 0, 0, 0, 0, 320, 200);
	
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

	/* Draw yellow select box for sel mode */
	if (boxmenu.s_active)
		rect(bf, sx, sy, sx + SEL_W, sy + SEL_H, 3);
	
	/* When inv was actived at item box */
	if (boxmenu.active){
		
		BITMAP *bt = itemsslot_bmp->dat;
		int p1, p2, p3;
		
		p1 = boxmenu.b_pos;
		
		if (p1 == 99){
			p2 = 0;
			p3 = 1;
		}else if (p1 == 98){
			p2 = 99;
			p3 = 0;
		}else if (p1 == 97){
			p2 = 98;
			p3 = 99; 
		}else{
			p2 = p1 + 1;
			p3 = p2 + 2;
		}
		
		blit (bt, bf, 0, 0, 9, 89, bt->w, bt->h);
		blit (bt, bf, 0, 0, 9, 89+13, bt->w, bt->h);
		blit (bt, bf, 0, 0, 9, 89+26, bt->w, bt->h);
		textprintf_ex (bf, inv_fnt, 11, 89+3, -1, -1, "%-30s% 4d",
			inv_list[itembox[p1].id][0], itembox[p1].num);
		textprintf_ex (bf, inv_fnt, 11, 89+3+13, -1, -1, "%-30s% 4d",
			inv_list[itembox[p2].id][0], itembox[p2].num);
		textprintf_ex (bf, inv_fnt, 11, 89+3+26, -1, -1, "%-30s% 4d",
			inv_list[itembox[p3].id][0], itembox[p3].num);
			
		rect(bf, 9, 102, 9+bt->w, 102+bt->h, 3);
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

int
boxmenu_active (void)
{
	return boxmenu.active;
}

void
boxmenu_set_sactive (int t)
{
	boxmenu.s_active = t;
	boxmenu.s_pos = invmenu.sp;
}

void
boxmenu_set_active (int t)
{
	boxmenu.active = t;
}

int
boxmenu_swap_item (void)
{
	int t_num, t_id;
	if (boxmenu.focus){
		t_id  = itembox[boxmenu.b_pos].id;
		t_num = itembox[boxmenu.b_pos].num;
		
		itembox[boxmenu.b_pos].id = inv[invmenu.sp].id;
		itembox[boxmenu.b_pos].num = inv[invmenu.sp].num;
		
		inv[invmenu.sp].id = t_id;
		inv[invmenu.sp].num = t_num;
	}else{
		t_id  = inv[invmenu.sp].id;
		t_num = itembox[boxmenu.b_pos].num;
		

	}
}

void
invmenu_sel_up (void)
{
	if (!m_rest){
		
		if (boxmenu.focus){
			
			boxmenu.b_pos--;

			if (boxmenu.b_pos < 0)
				boxmenu.b_pos = MAX_ITEMBOX - 1;
		}else{
			
			if (!boxmenu.s_active){
				if (invmenu.sp > 1)
					invmenu.sp -= 2;
			}else if (boxmenu.s_active){
				if (boxmenu.s_pos > 1)
					boxmenu.s_pos -= 2;
			}
				
		}
		
		play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, NULL);
		m_rest = SEL_R;
	}
}

void
invmenu_sel_down (void)
{
	if (!m_rest){
		
		if (boxmenu.focus){
			
			boxmenu.b_pos++;
	
			if (boxmenu.b_pos > MAX_ITEMBOX - 1)
				boxmenu.b_pos = 0;
		}else{
			
			if (!boxmenu.s_active){
				if (invmenu.sp < 6)
					invmenu.sp += 2;
			}else{
				if (boxmenu.s_pos < 6)
					boxmenu.s_pos += 2;
			}
		}
		
		play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, NULL);
		m_rest = SEL_R;
	}
}

void
invmenu_sel_right (void)
{
	if (!m_rest){
		
		play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, NULL);

		if (boxmenu.focus){
			boxmenu.focus = FALSE;
		}else{
			if (!boxmenu.s_active){
				if (invmenu.sp == 0 || invmenu.sp == 2 || invmenu.sp == 4 || invmenu.sp == 6)
					invmenu.sp++;
			}else{
				if (boxmenu.s_pos == 0 || boxmenu.s_pos == 2 || boxmenu.s_pos == 4 || boxmenu.s_pos == 6)
					boxmenu.s_pos++;
			}
		}
		
		m_rest = SEL_R;
	}
}

void
invmenu_sel_left (void)
{
	if (!m_rest){
		
		play_sample ((SAMPLE *)invsel_wav->dat, 155, 128, 1000, NULL);
		
		if (!boxmenu.s_active){
			if (invmenu.sp == 1 || invmenu.sp == 3 || invmenu.sp == 5 || invmenu.sp == 7){
				invmenu.sp--;
			}else if (boxmenu.active){
				boxmenu.focus = TRUE;
			}
		}else{
			if (boxmenu.s_pos == 1 || boxmenu.s_pos == 3 || boxmenu.s_pos == 5 || boxmenu.s_pos == 7){
				boxmenu.s_pos--;
			}else{
				boxmenu.focus = TRUE;
			}
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
invmenu_init (char *dfn, char *invb, char *itemb, char *fdn)
{
	char *names[] = {fdn, NULL};
	
	inv_bmp = load_datafile_object (dfn, invb);
	items_bmp = load_datafile_object (dfn, itemb);
	inv_fnt = load_dat_font (dfn, NULL, names);
	invsel_wav = load_datafile_object (dfn, "INVSEL_WAV");
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

	inv_bmp = NULL;
	items_bmp = NULL;
	inv_fnt = NULL;
	invsel_wav = NULL;
	itemsslot_bmp = NULL;
	
	boxmenu.active = FALSE;
	boxmenu.s_active = FALSE;
	invmenu.vis = FALSE;
	invmenu.sp = 0;
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
