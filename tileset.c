#include <allegro.h>
#include <stdio.h>
#include "tileset.h"

Tileset *new_tileset( void ){
	
	Tileset *ts = (Tileset *)malloc( sizeof(Tileset) );
	
	return ts;
}

void tileset_set( Tileset *tileset, BITMAP *bm, int tw, int th ){
	
	int num_tiles, x, y, ct;
	
	num_tiles = (bm->w / tw) * (bm->h / th);

	tileset->tiles = (BITMAP **)calloc( num_tiles, sizeof(BITMAP *) );
	
	tileset->num_tiles = num_tiles;
	
	for( y = 0, ct = 0; y < bm->h; y += th ){
		for( x = 0; x < bm->w; x += tw, ct++ ){
			tileset->tiles[ ct ] = create_bitmap( tw, th );
			
			if( !tileset->tiles[ ct ] ){
				printf( "Failed to create bitmap!\n" );
			}
			
			blit( bm, tileset->tiles[ct], x, y, 0, 0, tw, th );
		}
	}
	
	blit( tileset->tiles[0], screen, 0, 0, x, y, tw, th );
}

void tileset_free( Tileset *tileset ){
	
	int ct;
	for( ct = 0; ct < tileset->num_tiles; ct++ )
		destroy_bitmap( tileset->tiles[ct] );
		
	free( tileset->tiles );
	free( tileset );

}
