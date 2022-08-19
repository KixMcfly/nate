#ifndef __MAIN_H
#define __MAIN_H
#include <stdio.h>

#include "map.h"
#include "draw.h"
#include "sprite.h"
#include "node.h"
#include "inv.h"
#include "tools.h"
#include "text.h"

#define CAMERA_W 			320
#define CAMERA_H 			200

#define TILE_W				20
#define TILE_H				20

#define NATE_DAT 			"nate.DAT"



/* Make EXE smaller by removing unused drivers */
BEGIN_GFX_DRIVER_LIST
	GFX_DRIVER_VGA
END_GFX_DRIVER_LIST

BEGIN_COLOR_DEPTH_LIST
	COLOR_DEPTH_8
END_COLOR_DEPTH_LIST

BEGIN_DIGI_DRIVER_LIST
	DIGI_DRIVER_SB
END_DIGI_DRIVER_LIST

BEGIN_MIDI_DRIVER_LIST
	MIDI_DRIVER_ADLIB
END_MIDI_DRIVER_LIST

BEGIN_JOYSTICK_DRIVER_LIST
END_JOYSTICK_DRIVER_LIST

/* Timer chain */
volatile int elapsed_time;
void __inc_elapsed_time(){ elapsed_time--; };
END_OF_FUNCTION( __inc_elapsed_time );


enum {
	DOWN,
	UP,
	LEFT,
	RIGHT
};

typedef struct {
	
	int x;
	int y;
	int lx;
	int ly;
	int sx;
	int sy;
	int dir;
	int cf;
	int ar;
	SPRITE *s;
	
} NATE;

void
nate_focus_camera (MAP *m, int nx, int ny, int *cam_x, int *cam_y);

void
nate_set_xy (NATE *n, int x, int y);

void
nate_def (NATE *n);

void
nate_draw (NATE *n);

void
nate_init (void);

void
nate_exit (void);

#endif
