#ifndef __INV_H
#define __INV_H

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
invmenu_process (void);

int
invmenu_vis (void);

void
invmenu_show (void);

void
invmenu_hide (void);

int
invmenu_get_x (void);

int
invmenu_get_y (void);

int
invmenu_get_w (void);

int
invmenu_get_h (void);

int
invmenu_get_c (void);

void
invmenu_sel_up (void);

void
invmenu_sel_down (void);

void
invmenu_sel_right (void);

void
invmenu_sel_left (void);

void
inv_init (void);

int
inv_add (int id, int amt);

void
inv_log (void);

#endif
