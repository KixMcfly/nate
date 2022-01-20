#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

#define DEBUG FALSE

int is_solid( Map *m, int x, int y ){
	
	if( IS_SOLID( m->td[ 0 ][ y*m->mw+x ] ) )
		return TRUE;
	else
		return FALSE;
}

Map *load_map( char *map_name ){
	
	DATAFILE *map;
	Map *new_m;
	unsigned char *map_data;
	unsigned char **map_p;
	unsigned char *save;
	int i;
	
	map = load_datafile_object( "nate.dat", map_name );
	
	if( !map ){
		return NULL;
	}
	
	map_data = malloc( map->size );
	save = map_data;
	new_m = malloc( sizeof(Map) );
	
	if( !new_m ){
		printf( "Couldn't alloc memory for map!\n"  );
		return NULL;
	}
	
	memcpy( map_data, map->dat, map->size );
	
	unload_datafile_object( map );
	
	map_p = &map_data;
	
	#if DEBUG
	mem_forw( map_p, 8 );
	printf( "MAP VERSION MINOR: %u\n", get_byte(map_p) );
	printf( "MAP VERSION MINOR: %u\n", get_byte(map_p) );
	#else
	mem_forw( map_p, 10 );
	#endif
	
	new_m->name = get_string( map_p, get_byte(map_p) );
	
	new_m->tran_r = get_byte( map_p );
	new_m->tran_g = get_byte( map_p );
	new_m->tran_b = get_byte( map_p );
	 
	new_m->tw = get_word( map_p );
	new_m->th = get_word( map_p );

	new_m->mw = get_dword( map_p );
	new_m->mh = get_dword( map_p );
	
	new_m->nt = get_byte( map_p );
	
	/* Allocate tileset data  */
	new_m->tsets = (Tileset **)malloc( new_m->nt * sizeof(Tileset *) );
	
	if( !new_m->tsets ){
		printf( "Couldn't alloc memory for tileset!\n" );
		free( new_m->name );
		free( new_m );
		return NULL;
		
	}
	
	for( i = 0; i < new_m->nt; i++ ){
		
		char *tsetn;
		DATAFILE *dt;
		BITMAP *bt;
		mem_forw( map_p, 4 );
		
		tsetn = get_string( map_p, get_byte(map_p) );
		dt = load_datafile_object( "nate.dat", tsetn );
		
		free( tsetn );
		bt = (BITMAP *)dt->dat;
		
		new_m->tsets[i] = new_tileset();
		tileset_set( new_m->tsets[i], bt, new_m->tw, new_m->th );
		
		unload_datafile_object( dt );
		
	}
	
	new_m->nl = get_byte( map_p );
	new_m->td = (unsigned long **)malloc( new_m->nl * sizeof(long *) );
	for( i = 0; i < new_m->nl; i++ ){
		
		mem_forw( map_p, 3 );
		new_m->td[ i ] = (unsigned long *)malloc( sizeof(long) * (new_m->mw * new_m->mh) );
		long y, x;
		for( y = 0; y < new_m->mh; y++ ){
			for( x = 0; x < new_m->mw; x++ ){
				
				new_m->td[i][y * new_m->mw + x] = get_dword( map_p );
				
				//if( new_m->td[i][y * new_m->mw + x] )
					//new_m->td[i][y * new_m->mw + x]--;
				
				#if DEBUG
				printf( "%lx ", TILE_NUM(new_m->td[i][y * new_m->mw + x]) );
				#endif
			}
			
			#if DEBUG
			printf("\n");
			#endif
		}
	}
	
	#if DEBUG
	printf( "MAP NAME: %s\n", new_m->name );
	printf( "TILE WIDTH: %u\n", new_m->tw );
	printf( "TILE HEIGHT: %u\n", new_m->th );
	printf( "MAP WIDTH: %lu\n", new_m->mw );
	printf( "MAP HEIGHT: %lu\n", new_m->mh );
	printf( "R: %u\n", new_m->tran_r );
	printf( "G: %u\n", new_m->tran_g );
	printf( "B: %u\n", new_m->tran_b );
	printf( "TILESETS: %u\n", new_m->nt );
	printf( "LAYERS: %u\n", new_m->nl );
	
	while( !key[KEY_D] )
		;
	
	#endif
	
	/* Restore original pointer position before freeing */
	map_data = save;
	free( map_data );
	
	return new_m;
}

void free_map( Map *m ){
	
	int i;
	free( m->name );
	
	i = 0;
	while( i < m->nt ){
		tileset_free( m->tsets[i] );
		i++;
	}
	
	i = 0;
	while( i < m->nl ){
		free( m->td[i] );
		i++;
	}
	
	free( m->td );
	free( m );
}

void draw_all_layers( BITMAP *d, Map *m ){
	
	unsigned long x, y, i, tx, ty, tn, ts;
	
	for( i = 0; i < m->nl; i++ )
		for( y = 0, ty = 0; y < m->mh; y++, ty += m->th )
			for( x = 0, tx = 0; x < m->mw; x++, tx += m->tw ){
				tn = TILE_NUM(m->td[i][y*(m->mw)+x]);
				ts = TSET_NUM(m->td[i][y*(m->mw)+x]);
				
				if( tn )
					blit( m->tsets[ts]->tiles[--tn], d, 0, 0, tx, ty, m->tw, m->th );
				else if( !i )
					rectfill( d, tx, ty, tx + m->tw, ty + m->th, 0 );
			}

}

void mem_forw( unsigned char **mem, unsigned long num ){
	
	*mem += num;
}

char *get_string( unsigned char **mem, int num ){
	
	char *temp = calloc( num + 1, sizeof(char) );
	memcpy( temp, *mem, num );
	*mem += num;
	return temp;
}

unsigned char get_byte( unsigned char **mem ){
	
	unsigned char temp;
	memcpy( &temp, *mem, sizeof(char) );
	*mem += sizeof(char);
	return temp;
}

unsigned short get_word( unsigned char **mem ){
	
	unsigned short temp;
	memcpy( &temp, *mem, sizeof(short) );
	*mem += sizeof(short);
	return temp;
}

unsigned long get_dword( unsigned char **mem ){
	
	unsigned long temp;
	memcpy( &temp, *mem, sizeof(long) );
	*mem += sizeof(long);
	return temp;
}
