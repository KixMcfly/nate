#include <allegro.h>
#include "triggers.h"
#include "objects.h"


/* Please don't judge me */
int change_room_check( Map *map, int x, int y ){
	/* Leaving nates room and into the hallway */
	if( TILE_NUM(map->td[0][ x + y * map->mw ]) == 1 &&
		TSET_NUM(map->td[0][ x + y * map->mw ]) == 1 ){

			return GOTO_NATEROOM_OUT;
			
		}
				  
	
	return NULL;	
}
