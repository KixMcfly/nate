#ifndef __INV_H
#define __INV_H

#include <allegro.h>

/* Inventory list */
enum {
	INV_NONE,
	
	/* VALUE */
	INV_MONEY,
	
	/* FOOD */
	INV_NUGGETS,
	INV_DRDOUCHE,
	
	/* WEAPONS */
	INV_KNIFE,
	
	INV_NUM
};

void
inv_draw_battle (BITMAP *bf);

void
invmenu_draw_backbuff (BITMAP *bf);

int
boxmenu_active (void);

void
boxmenu_set_active (int t);

void
boxmenu_set_src_dest (void);

void
invmenu_sel_up (void);

void
invmenu_sel_down (void);

void
invmenu_sel_right (void);

void
invmenu_sel_left (void);

int
invmenu_vis (void);

void
invmenu_init (char *dfn);

void
invmenu_free (void);

int
inv_get_item_total (int id);

int
inv_sub (int id, int amt);

int
inv_add (int id, int amt);

int
inv_get_item_id (int pos);

int *
inv_get_item_amount (int pos);

#endif
