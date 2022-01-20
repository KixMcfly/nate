#ifndef OBJECTS_H
#define OBJECTS_H
#include <allegro.h>
#include "graphics.h"
#include "map.h"

extern int nate_x, nate_y;
extern int nate_log_x, nate_log_y;
extern int nate_dir;
extern int nate_frm;
extern int snap_x, snap_y;

void process_nate( Map *map );
void nate_change_y( int y );
void nate_change_x( int x );
void snap_adj_x( void );
void snap_adj_y( void );

#endif
