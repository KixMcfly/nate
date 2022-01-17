#include "nate.h"

volatile int elapsed_time = 5;
void __inc_elapsed_time(){ elapsed_time--; };
END_OF_FUNCTION( __inc_elapsed_time );

int main (void){
    
    DATAFILE *song;
    Sprite *nate;
    Map *map;

	if( !NATE_init() )
	   return NULL;
	   
	install_timer();
	   
	LOCK_VARIABLE( time );
	LOCK_FUNCTION( __inc_elapsed_time );
	install_int_ex( __inc_elapsed_time, BPS_TO_TIMER(1000) );
	
	back_buff = create_bitmap( 320, 200 );
	map = load_map( "NATEROOM_SED" );
    
    nate = new_sprite();
    load_sprite( nate, "NATESPR_PCX", 20, 40 );
    
    song = load_datafile_object( "nate.dat", "SONG_MID" );
    play_midi( (MIDI *)song->dat, 1 );
 
    while( !key[KEY_ESC] ){
        
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
        
        if( key[KEY_A] ){
			fade_out( 3 );
			fade_in( cur_pal, 3 );
		}
        
        snap_adj_x();
        snap_adj_y();
        
        draw_all_layers( back_buff, map );
        blit_sprite( nate, back_buff, nate_log_x, nate_log_y, nate_dir, nate_frm );
        blit( back_buff, screen, 0, 0, 0, 0, back_buff->w, back_buff->h );
        
        while( elapsed_time > 0 )
            ;
            
        elapsed_time = 20;
            
    }
    
    unload_datafile_object( song );
    
    free_map( map );
    destroy_sprite( nate );
    
    NATE_uninit();
    
    return 0;
}
END_OF_MAIN();


BOOL NATE_init( void ){

    BOOL error = FALSE;
    DATAFILE *pal;
        
    allegro_init();

    if( install_keyboard() ){
        allegro_message( "Unable to install keyboard %s\n", allegro_error );
        allegro_exit();
        return NULL;
    }

    set_color_depth( 8 );
    if( set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) ) {
        set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
   	    allegro_message( "Unable to set 320x200 graphics mode: %s\n", allegro_error );
   	    allegro_exit();
        return NULL;
	}
    
    if( install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) ){
        allegro_message( "Error initialising sound: %s\n", allegro_error );
        install_sound( DIGI_NONE, MIDI_NONE, NULL );
        error = TRUE;  
    }
    
    pal = load_datafile_object( "nate.dat", "PALETTE" );
    if( !pal ){
        allegro_message( "Unable to load palette from data file\n" );
        error = TRUE;
    }else{
        set_palette( pal->dat );
        get_palette( cur_pal );
        unload_datafile_object( pal );
    }
    
    if( error )
        readkey();

	return TRUE;
}

void NATE_uninit( void ){
    remove_keyboard();
    set_gfx_mode( GFX_TEXT, 0, 0, 0, 0 );
    allegro_exit();
}
