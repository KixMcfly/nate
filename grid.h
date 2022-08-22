#ifndef __GRID
#define __GRID

#include <stdlib.h>

#include "tools.h"

int
grid_set_w (int w);

int
grid_set_h (int h);

void
grid_snap_up (int *x, int *y, int *dy);

void
grid_snap_down (int *x, int *y, int *dy);

void
grid_snap_left (int *x, int *y, int *dx);

void
grid_snap_right (int *x, int *y, int *dx);

int
grid_snap_queue_add (int *x, int *y, int *dx, int *dy);

int
grid_snap_queue_free (void);

void
grid_snap_queue_proc (void);

#endif
