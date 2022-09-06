#ifndef _TEMP_H
#define _TEMP_H

#include <allegro.h>
#include "tools.h"

int
temp_vis (void);

void
temp_set_vis (void);

void
temp_pos_up (void);

void
temp_pos_down (void);

int
temp_global_process (void);

void
temp_draw_backbuff (BITMAP *bf, char *dfn);

void
temp_uninit (void);


#endif
