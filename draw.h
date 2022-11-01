#ifndef __DRAW_H
#define __DRAW_H

#include "map.h"

BITMAP *
get_backbuff (void);

void
show_backbuff (int x, int y);

int
init_backbuff (int w, int h);

int
free_backbuff (void);

int
set_pal (char *df, char *id);

int
draw_map_layer (MAP *m, int ln, int x, int y);

void
draw_load_blit_show (char *bn, char *dfn);

int
faded (void);

void
fadein (void *pal, int spd);

void
fadeout (int spd);

#endif
