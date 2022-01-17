#ifndef OBJECTS_H
#define OBJECTS_H
#include <allegro.h>
#include "graphics.h"

extern int nate_x, nate_y;
extern int nate_log_x, nate_log_y;
extern int nate_dir;
extern int nate_frm;
extern int snap_x, snap_y;

void snap_adj_x( void );
void snap_adj_y( void );

#endif
