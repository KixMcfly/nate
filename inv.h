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

int
invmenu_vis (void);

void
invmenu_show (void);

void
invmenu_hide (void);

void
inv_init (void);

int
inv_add (int id, int amt);

void
inv_log (void);

#endif
