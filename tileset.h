#ifndef TILESET_H
#define TILESET_H

typedef struct {
	
	BITMAP **tiles;
	unsigned long num_tiles;
	
} Tileset;

Tileset *new_tileset( void );
void tileset_set( Tileset *tileset, BITMAP *bm, int tw, int th );
void tileset_free( Tileset *tileset );



#endif
