#ifndef __VEND_H
#define __VEND_H

#include <allegro.h>
#include "obj.h"
#include "tools.h"

int
vend_get_cost (int id);

void
vend_draw_backbuff (VENDING *vend, int money, BITMAP *bf)

void
vend_move_up (void);

void
vend_move_down (void);

void
vend_move_left (void);

void
vend_move_right (void);

int
vend_vis (void);

void
vend_draw_backbuff (VENDING *vend, BITMAP *bf);

int
vend_init_dat (VENDING *v, char *dfn, char *vb, char *ib);

void
vend_free_dat (void);


#endif /* __VEND_H */
