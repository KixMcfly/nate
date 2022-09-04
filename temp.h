#ifndef _TEMP_H
#define _TEMP_H

#include <allegro.h>

int
temp_vis (void);

void
temp_set_vis (void);

void
temp_process (BITMAP *bf, char *dfn);

void
temp_uninit (void);


#endif
