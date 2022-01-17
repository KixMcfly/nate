#include "graphics.h"

PALETTE cur_pal;
BITMAP *back_buff;


void blit_sprite( Sprite *spr, BITMAP *d, int x, int y, int r, int c ){
	
	if( r == SPR_RIGHT )
		draw_sprite_h_flip( d, spr->dat[--r][c], x, y );
	else
		draw_sprite( d, spr->dat[r][c], x, y );
}

Sprite *new_sprite( void ){
	
	Sprite *spr = (Sprite *)malloc( sizeof(Sprite) );
	
	return spr; 
}

void load_sprite( Sprite *spr, char *bmp_name, int w, int h ){

    DATAFILE *dat;
    BITMAP *tmp;
    int x, y;
    
    dat = load_datafile_object( "nate.dat", bmp_name );
    
    if( !dat ){
        allegro_message( "Failed to load dat file\n" );
        return;
    }else{
        tmp = (BITMAP *)dat->dat;
 
		spr->num_r = tmp->w / w;
		spr->num_c = tmp->h / h;
        
        spr->dat = (BITMAP ***)malloc( spr->num_r * sizeof(BITMAP **) );
        
        for( y = 0; y < spr->num_r; y++ ){
			
			spr->dat[y] = (BITMAP **)malloc( spr->num_c * sizeof(BITMAP *) );
			
			for( x = 0; x < spr->num_c; x++ ){
				spr->dat[y][x] = create_bitmap( w, h );
				blit( tmp, spr->dat[y][x], x*w, y*h, 0, 0, w, h );
			}
		}
            
    }
    
    unload_datafile_object( dat );
    
}

void destroy_sprite( Sprite *spr ){
	
	int x, y;
	
	if( !spr )
		return;
	
	for( y = 0; y < spr->num_r; y++ ){
		
		for( x = 0; x < spr->num_c && spr->dat[y]; x++ )
			destroy_bitmap( spr->dat[y][x]  );
		
		free( spr->dat[y] );
	}
	
	free( spr );

}
