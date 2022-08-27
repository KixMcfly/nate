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
	
	int vis;
	int sp;
	int rest;
	
} INVMENU;

static INVMENU invmenu = {.vis = FALSE, .sp = 0, .rest = 0};
static INVITEM itembox[MAX_ITEMBOX] = {{0}};
static INVITEM inv[MAX_INV] = {{0}};

static DATAFILE *inv_bmp = NULL;
static DATAFILE *items_bmp = NULL;
static FONT *inv_fnt = NULL;

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
	
	{"NONE", "LOL"},
	/* VALUE */
	{"NATE BUCKS", "Big daddy money"},
	
	/* FOOD */
	{"NUGGETS", "DDDDDDDDDDD"},
	{"DR. DOUCHE", "Remember, DO THE DOUCHE! And don't forget to drink much SODA"}
	
};

static void
text_area_draw (BITMAP *b, char *s, int x, int y)
{
	int fh = text_height (inv_fnt), xo = 0;
	char *token, *ts = NULL;
	
	ts = (char *) malloc (strlen (s) + 1);
	strcpy (ts, s);
	
	token = strtok (ts, " ");
	
	while (token){

		textout_ex(b, inv_fnt, token, x+xo, y, -1, -1);
		
		xo += text_length (inv_fnt, token) + 4;
		
		if (x+xo > 158){
			xo = 0;
			y += 11;
		}

		token = strtok (NULL, " ");
	}
	
	free (ts);
}

void
invmenu_draw_backbuff (BITMAP *bf)
{ 
	
	int px = pos_lookup[invmenu.sp][X], py = pos_lookup[invmenu.sp][Y];
	int px2 = px + SEL_W, py2 = py + SEL_H;
	int ci;
	
	/* Draw inv menu */		
	blit ((BITMAP *)inv_bmp->dat, bf, 0, 0, 0, 0, 320, 200);
	
	/* Draw items */
	for (ci = 0; ci < MAX_INV; ci++){
		
		if (inv[ci].id ){
			masked_blit ((BITMAP *)items_bmp->dat, bf, inv[ci].id * 41, 0, ip_lookup[ci][X], ip_lookup[ci][Y], 41, 31);
			textprintf_right_ex (bf, inv_fnt, inp_lookup[ci][X], inp_lookup[ci][Y], -1, -1, "%d", inv[ci].num);
		}
	}
	
	rect(bf, px, py, px2, py2, SEL_C);
	
	if (!inv[invmenu.sp].id){
		rectfill(bf, 9, 9, 200, 88, 48);
	}else{
		
		char *name  = inv_list[inv[invmenu.sp].id][0];
		char *desc  = inv_list[inv[invmenu.sp].id][1];
		
		textout_ex(bf, inv_fnt, name, 14, 14, -1, -1);
		text_area_draw (bf, desc, 14, 24);
	}
	
	
	if (invmenu.rest)
		invmenu.rest--;
}

int
invmenu_vis (void)
{
	return invmenu.vis;
}

void
invmenu_sel_up (void)
{
	if (invmenu.sp > 1 && !invmenu.rest){
		invmenu.sp -= 2;
		invmenu.rest = SEL_R;
	}
}

void
invmenu_sel_down (void)
{
	if (invmenu.sp < 6 && !invmenu.rest){
		invmenu.sp += 2;
		invmenu.rest = SEL_R;
	}
}

void
invmenu_sel_right (void)
{
	if ((invmenu.sp == 0 || invmenu.sp == 2 || invmenu.sp == 4 || invmenu.sp == 6) &&
	    !invmenu.rest){
		invmenu.sp++;
		invmenu.rest = SEL_R;
	}
}

void
invmenu_sel_left (void)
{
	if ((invmenu.sp == 1 || invmenu.sp == 3 || invmenu.sp == 5 || invmenu.sp == 7) &&
		!invmenu.rest){
		invmenu.sp--;
		invmenu.rest = SEL_R;
	}
}

void
invmenu_init (char *dfn, char *invb, char *itemb, char *fdn)
{
	char *names[] = {fdn, NULL};
	
	inv_bmp = load_datafile_object (dfn, invb);
	items_bmp = load_datafile_object (dfn, itemb);
	inv_fnt = load_dat_font (dfn, NULL, names);
	
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
		
	inv_bmp = NULL;
	items_bmp = NULL;
	inv_fnt = NULL;
	
	invmenu.vis = FALSE;
	invmenu.sp = 0;
	invmenu.rest = 0;
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
