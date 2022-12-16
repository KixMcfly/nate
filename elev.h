#ifndef __ELEV_H
#define __ELEV_H

#include <allegro.h>
#include <ctype.h>
#include "node.h"
#include "tools.h"

int
elev_vis (void);

void
elev_sel_up (void);

void
elev_sel_down (void);

void
elev_sel_left (void);

void
elev_sel_right (void);

int
elev_get_floor_goto_id (void);

char *
elev_get_floor_goto_name (void);

void
elev_floor_goto (void);

void
elev_press (void);

void
elev_draw_backbuff (BITMAP *bf);

void
elev_free (void);

int
elev_init (char *dfn, char *bn);

#endif /* __ELEV_H */
