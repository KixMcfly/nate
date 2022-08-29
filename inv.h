#ifndef __INV_H
#define __INV_H

#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include "tools.h"

/* Inventory list */
enum {
	INV_NONE,
	
	/* VALUE */
	INV_MONEY,
	
	/* FOOD */
	INV_NUGGETS,
	INV_DRDOUCHE,
	INV_NUM
};

static void
text_area_draw (BITMAP *b, char *s, int x, int y);

void
invmenu_draw_backbuff (BITMAP *bf);

int
boxmenu_active (void);

void
boxmenu_set_sactive (int t);

int
invmenu_vis (void);

void
boxmenu_set_active (int t);

void
invmenu_sel_up (void);

void
invmenu_sel_down (void);

void
invmenu_sel_right (void);

void
invmenu_sel_left (void);

void
invmenu_init (char *dfn, char *invb, char *itemb, char *fdn);

void
invmenu_free (void);

int
inv_get_item_total (int id);

int
inv_sub (int id, int amt);

int
inv_add (int id, int amt);

#endif
