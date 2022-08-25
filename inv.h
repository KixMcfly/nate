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

void
invmenu_draw_backbuff (BITMAP *bf);

int
invmenu_vis (void);

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
inv_add (int id, int amt);

void
inv_log (void);


#endif
