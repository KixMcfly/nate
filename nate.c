#include "nate.h"
#include "triggers.h"

volatile int elapsed_time = 5;
void __inc_elapsed_time(){ elapsed_time--; };
END_OF_FUNCTION( __inc_elapsed_time );

int main (void){
    
    DATAFILE *song, *dat;
    Sprite *nate;
    Map *map = NULL;
    BITMAP *ball_spr = NULL, *title_img = NULL;
    BOOL in_menu = TRUE;
    int room_change = NULL;
    BOOL quit = FALSE;
	int ball_x = -20, ball_y = 100, ball_y_dir = -1, ball_float = 0; 


	if( !NATE_init() )
	   return NULL;
	   
	install_timer();
	
	LOCK_VARIABLE( time );
	LOCK_FUNCTION( __inc_elapsed_time );
	install_int_ex( __inc_elapsed_time, BPS_TO_TIMER(1000) );
	
	dat = load_datafile_object( "nate.dat", "TITLE_PCX" );
	back_buff = create_bitmap( 320, 200 );
	title_img = create_bitmap( 320, 200 );
	blit( (BITMAP *)dat->dat, title_img, 0, 0, 0, 0, 320, 200 );
	unload_datafile_object( dat );
	
	dat = load_datafile_object( "nate.dat", "BALL_PCX" );
	ball_spr = create_bitmap( 48, 68 );
	blit( (BITMAP *)dat->dat, ball_spr, 0, 0, 0, 0, 48, 68 );
	unload_datafile_object( dat );
	
	dat = load_datafile_object( "nate.dat", "TITLE_PAL" );
    set_palette( dat->dat );
    unload_datafile_object( dat );
    
    nate = new_sprite();
    load_sprite( nate, "NATESPR_PCX", 20, 40 );
 
	song = load_datafile_object( "nate.dat", "UP_MID" );
	play_midi( (MIDI *)song->dat, 1 );
 
    while( !quit ){
        
        if( !in_menu )
			process_nate( map );
        
        if( !in_menu )
			room_change = change_room_check( map, nate_x, nate_y);
        
        if( !in_menu && key[KEY_ESC] )
			quit = TRUE;
        
        if( key[KEY_LCONTROL] ){
			
			if( room_change && !in_menu ){
				free_map( map );
				map = load_map( "HALLWAY_SED" );
				nate_change_x( 4 );
				nate_change_y( 5 );
			}else if( in_menu ){
				map = load_map( "NATEROOM_SED" );
				stop_midi();
				unload_datafile_object( song );
				song = load_datafile_object( "nate.dat", "SONG_MID" );
				play_midi( (MIDI *)song->dat, 1 );
				dat = load_datafile_object( "nate.dat", "PALETTE" );
				set_palette( dat->dat );
				unload_datafile_object( dat );
				set_palette( dat->dat );
				get_palette( cur_pal );
				destroy_bitmap( title_img );
				in_menu = FALSE;
				
			}
		}
                
        snap_adj_x();
        snap_adj_y();
    
		if(!in_menu)
			draw_all_layers( back_buff, map );
		else{
			blit( title_img, back_buff, 0, 0, 0, 0, back_buff->w, back_buff->h );
			draw_sprite(back_buff, ball_spr, ++ball_x, ball_y);
			
			if( ++ball_float == 40 ){
				ball_float = 0;
				
				if( ball_y_dir == -1 )
					ball_y_dir = 1;
				else
					ball_y_dir = -1;
			}
			
			
			
			ball_y += ball_y_dir;
			
			if( ball_x > 400 ){
				ball_x = -50;
				ball_y = (rand() % (160 - -20 + 1)) + -20;
			}
		}

        if( !in_menu )
			blit_sprite( nate, back_buff, nate_log_x, nate_log_y, nate_dir, nate_frm );
        
		/* Display trigger data */
        switch( room_change ){
			
			case GOTO_NATEROOM_OUT:
				textout_ex( back_buff, font, "To Hallway", 10, 10, 15, -1 );
				break;
			
		}
        
        blit( back_buff, screen, 0, 0, 0, 0, back_buff->w, back_buff->h );
        
        while( elapsed_time > 0 )
            ;
            
        elapsed_time = 20;
            
    }
    
    unload_datafile_object( song );
    
    free_map( map );
    destroy_bitmap( ball_spr );
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
