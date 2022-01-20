#include "objects.h"

int nate_x = 4, nate_y = 9;
int nate_log_x = 80;
int nate_log_y = 160;
int nate_frm = 0;
int nate_dir = 0;
int snap_x = 0, snap_y = 0;

void process_nate( Map *map ){
	
	if( key[KEY_UP] && !snap_y && !is_solid( map, nate_x, nate_y - 1 ) ){
        snap_y = -20;
        nate_dir = SPR_UP;
        nate_frm++;
        nate_y--;
    }
    
    if( key[KEY_DOWN] && !snap_y &&  nate_y + 1 < map->mh && !is_solid( map, nate_x, nate_y + 1 )  ){
        snap_y = 20;
        nate_dir = SPR_DOWN;
        nate_frm++;
        nate_y++;
    }
    
    if( key[KEY_LEFT] && !snap_x && !is_solid( map, nate_x - 1, nate_y ) ){
        snap_x = -20;
        nate_dir = SPR_LEFT;
        nate_frm++;
        nate_x--;
    }
    
    if( key[KEY_RIGHT] && !snap_x && !is_solid( map, nate_x + 1, nate_y ) ){
        snap_x = 20;
        nate_dir = SPR_RIGHT;
        nate_frm++;
        nate_x++;
    }
}

void nate_change_y( int y ){
	
	nate_y = y;
	nate_log_y = nate_y * 20;
}

void nate_change_x( int x ){
	
	nate_x = x;
	nate_log_x = nate_x * 20;
}

void snap_adj_x( void ){
    
    if( snap_x > 0 ){
        nate_log_x += 2;
        snap_x -= 2;
   
    }else if( snap_x < 0 ){
        
        nate_log_x -= 2;
        snap_x += 2;
        
    }
    
    if( nate_frm > 2 )
		nate_frm = 0;
}

void snap_adj_y( void ){

    if( snap_y > 0 ){
        nate_log_y += 2;
        snap_y -= 2;
            
    }else if( snap_y < 0 ){
        
        nate_log_y -= 2;
        snap_y += 2;
       
    }
    
    if( nate_frm > 2 )
		nate_frm = 0;
}
