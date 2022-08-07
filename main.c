#include <allegro.h>
#include "main.h"

int main (void)
{	
	MAP *m = NULL;
	NATE nate;
	CHGROOM *cr = NULL;
	NODE *cn = NULL;
	
	int nl, cl, quit = FALSE;
	unsigned char solid;
	/* Initialize Nate */
	nate_init ();
	
	/* Load nate font */
	text_load_font_dat ("nate.DAT", "NATE_FNT");
	
	/* set starting room */
	set_pal ("nate.DAT", "NATE_PAL");
	m = map_new ();
	load_map (m, "nate.DAT", "NATEROOM_NAT");
	nl = map_get_nl (m);

	/* Init nate player data */
	nate.s = sprite_new ();
	sprite_keyframe_dat_div (nate.s, 3, 4, "nate.DAT", "NATESPR_BMP");
	nate_def (&nate);

	while (!quit){

		if (key[KEY_Q])
			quit = TRUE;
			
		if (key[KEY_UP]){
			solid = SOLID(map_get_tile_flags (m, 0, nate.x, nate.y-1));
			if (!nate.sy && !nate.sx && !solid && nate.y-1 > 0)
				nate.sy = -20;
			nate.dir = UP;
		}

		if (key[KEY_DOWN]){
			solid = SOLID(map_get_tile_flags (m, 0, nate.x, nate.y+1));
			if (!nate.sy && !nate.sx && !solid && nate.y+1 < map_get_h (m))
				nate.sy = 20;
			nate.dir = DOWN;
		}
			
		if (key[KEY_LEFT]){
			solid = SOLID(map_get_tile_flags (m, 0, nate.x-1, nate.y));
			if (!nate.sx && !nate.sy && !solid && nate.x-1 != -1)
				nate.sx = -20;
			nate.dir = LEFT;
		}
			
		if (key[KEY_RIGHT]){
			solid = SOLID(map_get_tile_flags (m, 0, nate.x+1, nate.y));
			if (!nate.sx && !nate.sy && !solid && nate.x+1 < map_get_w (m))
				nate.sx = 20;
			nate.dir = RIGHT;
		}

		/* Draw map layers */
		for (cl = 0; cl < nl; cl++){
			draw_map_layer (m, cl, 0, 0);
		}
		
		nate_process (&nate);

		/* Check objects */
		cn = map_get_node_head (m);
		while (cn){
			
			if (node_get_type (cn) == OBJ_CHGROOM){
				cr = node_get_data (cn);
				
				if (nate.lx == cr->x && nate.ly == cr->y){
					
					text_print_center (get_backbuff (), cr->name);
					//nate_set_xy (&nate, cr->cx, cr->cy);
					//
					//map_free (m);
					//m = map_new ();
					//load_map (m, "nate.DAT", cr->name);
					//nl = map_get_nl (m);
					
					break;
				}
					
			}
			
			cn = node_get_next (cn);
		}
		
		/* draw nathyn on backbuff */
		nate_draw (&nate);
		
		show_backbuff (0, 0);
		
		while (elapsed_time > 0)
			;
			
		elapsed_time = 10;
	}

	/* Destroy Nate font */
	text_destroy_font ();

	/* free nate sprite */
	sprite_free (nate.s);

	map_free (m);

	nate_exit ();
	return 0;
}
END_OF_MAIN()

void
nate_draw (NATE *n)
{
	sprite_draw (n->s, get_backbuff (), n->dir, n->cf, n->lx, n->ly-20);
}

void
nate_set_xy (NATE *n, int x, int y)
{
	n->x = x;
	n->y = y;
	n->lx = n->x * 20;
	n->ly = n->y * 20;
	n->sx = 0;
	n->sy = 0;
}

void
nate_def (NATE *n)
{
	n->x = 5;
	n->y = 5;
	n->lx = 5 * 20;
	n->ly = 5 * 20;
	n->sx = 0;
	n->sy = 0;
	n->ar = 0;
	n->dir = DOWN;
	n->cf = 0;
}

void
nate_process (NATE *n)
{
	
	/* Animate speed control */
	n->ar--;
	if (n->ar <= 0){
		n->cf++;

		if (n->cf >= sprite_keyframe_get_num_frames (n->s, n->dir))
			n->cf = 0;
			
		n->ar = sprite_keyframe_get_frame_rest (n->s, n->dir, n->cf);
	}
	
	/* Controls nates x / y grid snapping */
	
	/* Snap left */
	if (n->sx < 0){
		n->lx--;
		n->sx++;

		if (n->sx == 0){
			n->x--;
		}
	}
	
	/* Snap right */
	if (n->sx > 0){
		n->lx++;
		n->sx--;

		if (n->sx == 0)
			n->x++;
	}
	
	/* Snap up */
	if (n->sy < 0){
		n->ly--;
		n->sy++;

		if (n->sy == 0)
			n->y--;
	}
	
	/* Snap down */
	if (n->sy > 0){
		n->ly++;
		n->sy--;

		if (n->sy == 0)
			n->y++;
	}
}

void
nate_init (void)
{
    allegro_init();
    
    elapsed_time = 0;
	install_timer();
	
	LOCK_VARIABLE( time );
	LOCK_FUNCTION( __inc_elapsed_time );
	install_int_ex( __inc_elapsed_time, BPS_TO_TIMER(1000) );
    
    if (install_keyboard()){
        allegro_message ("Unable to install keyboard %s\n",
			allegro_error);
        allegro_exit ();
        return;
    }	

    set_color_depth( 8 );
    if (set_gfx_mode (GFX_AUTODETECT, 320, 200, 0, 0)) {
        set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
   	    allegro_message ("Unable to set 320x200 graphics mode: %s\n", 
			allegro_error);
		
		readkey ();	
   	    remove_keyboard ();
   	    
   	    allegro_exit();
        return;
	}
    
    if (install_sound (DIGI_AUTODETECT, MIDI_AUTODETECT, NULL)){
        allegro_message ("Error initialising sound: %s\n", 
				allegro_error);
        install_sound (DIGI_NONE, MIDI_NONE, NULL);
        readkey ();
    }
    
    init_backbuff (320, 200);
}

void
nate_exit( void )
{
    remove_keyboard ();
    remove_sound ();
    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
    free_backbuff ();
    allegro_exit ();
}
