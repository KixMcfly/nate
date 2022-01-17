#include "objects.h"

int nate_x = 4, nate_y = 9;
int nate_log_x = 80;
int nate_log_y = 160;
int nate_frm = 0;
int nate_dir = 0;
int snap_x = 0, snap_y = 0;

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
