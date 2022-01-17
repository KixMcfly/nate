#ifndef MAP_H
#define MAP_H

#include "tileset.h"

typedef struct {
	
	char *name;
	Tileset **tsets;
	unsigned char tran_r, tran_g, tran_b;
	unsigned short tw, th, nt, nl;
	unsigned long **td, mw, mh;

} Map;

extern unsigned char map_nates_room[10][16];

int is_solid( Map *m, int x, int y );
Map *load_map( char *map_name );
void draw_all_layers( BITMAP *d, Map *m );
void mem_forw( unsigned char **mem, unsigned long num );
char *get_string( unsigned char **mem, int num );
unsigned char get_byte( unsigned char **mem );
unsigned short get_word( unsigned char **mem );
unsigned long get_dword( unsigned char **mem );
void free_map( Map *m );

#endif
