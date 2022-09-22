#ifndef __MAP_H
#define __MAP_H

#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "tileset.h"
#include "obj.h"
#include "node.h"

enum {
	SOLID
};

/* Layer type */
enum {
	TILES,
	OBJECTS
};

typedef struct MAP MAP;
typedef struct LAYER LAYER;
typedef struct TILE TILE;

int
map_get_tw (MAP *m);

int
map_get_th (MAP *m);

int
map_get_w (MAP *m);

int
map_get_h (MAP *m);

int
map_get_lw (MAP *m);

int
map_get_lh (MAP *m);

int
map_get_nl (MAP *m);

int
map_get_nts (MAP *m);

char *
map_get_name (MAP *m);

BITMAP *
map_get_layer_img (MAP *m, int ln);

int
map_get_layer_type (MAP *m, int ln);

NNODE *
map_get_node_head (MAP *m);

MAP *
map_new (void);

unsigned char
map_get_tile_flags (MAP *m, int ln, int x, int y);

int
map_get_elev_x_pos (MAP *m);

int
map_get_elev_y_pos (MAP *m);

int
load_map (MAP *m, char *dat_fn, char *dat_id);

int
map_free (MAP *map);

#endif
