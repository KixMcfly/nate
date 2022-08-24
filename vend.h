#ifndef __VEND_H
#define __VEND_H

#include <allegro.h>
#include "obj.h"

int
vend_buy_item (int *cash);

void
vend_move_up (VENDING *v);

void
vend_move_down (VENDING *v);

void
vend_move_left (VENDING *v);

void
vend_move_right (VENDING *v);

void
vend_draw_backbuff (BITMAP *bf);

int
vend_init_dat (VENDING *v, char *dfn, char *vb, char *ib);

void
vend_free_dat (void);


#endif /* __VEND_H */
