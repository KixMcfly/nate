#include "inv.h"

#define MAX_ITEMBOX 		100
#define MAX_INV				8
#define SEL_W				52
#define SEL_H				43
#define SEL_C				14
#define SEL_R				13

typedef struct {
	
	int id;
	int num;

} INVITEM;

typedef struct {
	
	int vis;
	int sp;
	int rest;
	
} INVMENU;

static INVMENU invmenu;
static INVITEM itembox[MAX_ITEMBOX];
static INVITEM inv[MAX_INV];

/* X and Y pos of sel rec */
static int pos_lookup[MAX_INV][2] = {{212, 5},   {264, 5},  {212, 48},
									 {264, 48},  {212, 91}, {264, 91},
									 {212, 134}, {264, 134}};

static char *inv_list[INV_NUM] = {
	
	"NONE",
	/* VALUE */
	"MONEY",
	
	/* FOOD */
	"NUGGETS",
	"DR. DOUCHE"
	
};

void
invmenu_process ()
{
	if (invmenu.rest)
		invmenu.rest--;
}

int
invmenu_vis (void)
{
	return invmenu.vis;
}

void
invmenu_show (void)
{
	invmenu.vis = 1;
	invmenu.rest = 0;
}

void
invmenu_hide (void)
{
	invmenu.vis = 0;
}

int
invmenu_get_x (void)
{
	return pos_lookup[invmenu.sp][0];
}

int
invmenu_get_y (void)
{
	return pos_lookup[invmenu.sp][1];
}

int
invmenu_get_w (void)
{
	return SEL_W;
}

int
invmenu_get_h (void)
{
	return SEL_H;
}

int
invmenu_get_c (void)
{
	return SEL_C;
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
inv_init (void)
{
	invmenu.sp = 0;
	invmenu.vis = 0;
	invmenu.rest = 0;
	
	int i;
	for (i = 0; i < MAX_ITEMBOX; i++){
		itembox[i].id = INV_NONE;
		itembox[i].num = 0;
	}
	
	for (i = 0; i < MAX_INV; i++){
		inv[i].id = INV_NONE;
		inv[i].num = 0;
	}
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

void
inv_log (void)
{
	int i;
	for (i = 0; i < MAX_INV; i++){
		
		log_print ("ITEM: %s AMOUNT: %d\n", inv_list[ inv[i].id ], inv[i].num);
	}
}
