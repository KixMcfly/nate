#include <allegro.h>
#include "main.h"

int main (void)
{	
	MAP *m = NULL;
	NATE nate;
	CHGROOM *cr = NULL;
	GENERIC *gn = NULL;
	BITMAP *title = NULL;
	DATAFILE *df = NULL, *pal = NULL, *snd_door = NULL, *inv_bmp = NULL;
	NODE *cn = NULL;
	char *text_msg = NULL;
	
	int nl, cl, quit = FALSE, cam_x = 0, cam_y = 0;
	unsigned char solid;
	/* Initialize Nate */
	nate_init ();
	
	/* START MAIN MENU **********************/
	
	/* Title BMP */
	df = load_datafile_object (NATE_DAT, "TITLE_BMP");
	title = (BITMAP *)df->dat;
	blit (title, get_backbuff (), 0, 0, 0, 0, title->w, title->h);
	unload_datafile_object (df);
	set_pal (NATE_DAT, "TITLE_PAL");
	show_backbuff (0, 0);
	
	/* Load title MIDI*/
	df = load_datafile_object (NATE_DAT, "TITLE_MID");
	play_midi ((MIDI *)df->dat, TRUE);

	/* Press any key */
	while (!keypressed ())
		;

	fadeout (10);

	/* Stop title midi */
	stop_midi ();
	unload_datafile_object (df);

	/* END MAIN MENU ************************/
	
	/* Load nate font */
	text_load_font_dat (NATE_DAT, "NATE_FNT");
	
	/* Load sounds */
	snd_door = load_datafile_object (NATE_DAT, "DOOR_WAV");
	
	/* set starting room */
	m = map_new ();
	load_map (m, NATE_DAT, "NATEROOM_NAT");
	nl = map_get_nl (m);

	/* Init nate player data */
	nate.s = sprite_new ();
	sprite_keyframe_dat_div (nate.s, 3, 4, NATE_DAT, "NATESPR_BMP");
	nate_def (&nate);
	
	/* Inv BMP */
	inv_bmp = load_datafile_object (NATE_DAT, "INVMENU_BMP");
	
	/* Get palette for fade in */
	pal = load_datafile_object (NATE_DAT, "NATE_PAL");
	
	df = load_datafile_object (NATE_DAT, "NATEROOM_MID");
	play_midi ((MIDI *)df->dat, TRUE);

	inv_init ();

	while (!quit){
	
		if (key[KEY_Q])
			quit = TRUE;
			
		if (key[KEY_UP]){
			if (!invmenu_vis ()){
				solid = SOLID(map_get_tile_flags (m, 0, nate.x, nate.y-1));
				if (nate.y-1 > 0)
					nate.y--;
				nate.dir = UP;
			}else
				invmenu_sel_up ();
		}
		
		if (key[KEY_DOWN]){
			
			if (!invmenu_vis ()){
				solid = SOLID(map_get_tile_flags (m, 0, nate.x, nate.y+1));
				if (nate.y+1 < map_get_h (m))
					nate.y++;
				nate.dir = DOWN;
			}else
				invmenu_sel_down ();
		}
			
		if (key[KEY_LEFT]){
			
			if (!invmenu_vis ()){
				solid = SOLID(map_get_tile_flags (m, 0, nate.x-1, nate.y));
				if (nate.x-1 != -1)
					nate.x--;
				nate.dir = LEFT;
			}else
				invmenu_sel_left ();
		}
			
		if (key[KEY_RIGHT]){
			if (!invmenu_vis ()){
				solid = SOLID(map_get_tile_flags (m, 0, nate.x+1, nate.y));
				if (nate.x+1 < map_get_w (m))
					nate.x++;
				nate.dir = RIGHT;
			}else
				invmenu_sel_right ();
		}
		
		if (key[KEY_ESC]){
			if (!invmenu_vis ())
				invmenu_show ();
			else
				invmenu_hide ();
		
			fadeout (20);
		}
		
		/* Nate inventory managment */
		
		invmenu_process ();
		
		/* NATE ANIMATION SPEED CONTROL */
		nate.ar--;
		if (nate.ar <= 0){
			nate.cf++;
	
			if (nate.cf >= sprite_keyframe_get_num_frames (nate.s, nate.dir))
				nate.cf = 0;
				
			nate.ar = sprite_keyframe_get_frame_rest (nate.s, nate.dir, nate.cf);
		}

		/* Check objects */
		cn = map_get_node_head (m);
		while (cn){
		
			if (node_get_type (cn) == OBJ_COMPUTER){
				
				gn = node_get_data (cn);
				if (nate.x == gn->x && nate.y == gn->y){
		
				}
			}else if (node_get_type (cn) == OBJ_CHGROOM){
				cr = node_get_data (cn);
				
				if (nate.x == cr->x && nate.y == cr->y){
					
					text_msg = strtmp (cr->name);
					
					/* Change to room if use button is pressed */
					if (key[KEY_LCONTROL]){
						nate_set_xy (&nate, cr->cx, cr->cy);
						play_sample ((SAMPLE *)snd_door->dat, 255, 128, 1000, NULL);
						map_free (m);
						m = map_new ();
						load_map (m, NATE_DAT, text_msg);
						nl = map_get_nl (m);
						fadeout (5);
						
						/* Set camera based on nate location */
						nate_focus_camera (m, nate.x, nate.y, &cam_x, &cam_y);
						
						break;
					}
				}
			}
			
			cn = node_get_next (cn);
		}

		if (!invmenu_vis ()){
			/* Draw map layers */
			for (cl = 0; cl < nl; cl++)
				draw_map_layer (m, cl, -cam_x, -cam_y);
		
			/* draw nathyn on backbuff */
			nate_draw (&nate);
			
			text_print_center (get_backbuff (), text_msg);
			text_msg = NULL;
		}else{
			
			int px = invmenu_get_x (), py = invmenu_get_y ();
			int px2 = px + invmenu_get_w (), py2 = py + invmenu_get_h ();
			
			blit ((BITMAP *)inv_bmp->dat, get_backbuff (), 0, 0, 0, 0, 320, 200);
			rect(get_backbuff (), px, py, px2, py2, invmenu_get_c ());
		}
		
		show_backbuff (0, 0);
		
		if (faded ()){
			log_print ("CAM X: %d - CAM Y: %d\n", cam_x, cam_y);
			fadein (pal->dat, 64);
		}
		
		while (elapsed_time > 0)
			;
			
		elapsed_time = 10;
	}
	
	/* Unload BMPS */
	unload_datafile_object (inv_bmp);
	
	/* Unload sounds */
	unload_datafile_object (snd_door);
	
	/* Unload music */
	stop_midi ();
	unload_datafile_object (df);
	
	/* Destroy Nate font */
	text_destroy_font ();
	
	/* free nate sprite */
	sprite_free (nate.s);
	
	/* free pal */
	unload_datafile_object (pal);
	
	map_free (m);

	nate_exit ();
	return 0;
}
END_OF_MAIN()

void
nate_focus_camera (MAP *m, int nx, int ny, int *cam_x, int *cam_y)
{
	int mw, mh;
	
	if (!m)
		return;
		
	mw = map_get_w (m);
	mh = map_get_h (m);
	
	if (mw < CAMERA_W / TILE_W)
		*cam_x = CAMERA_W / 2 - mw / 2;
	else if (mw == CAMERA_W / TILE_W)
		*cam_x = 0;
	else
		for (*cam_x = 0; *cam_x + 160 != nx; *cam_x += TILE_W)
			;
		
	if (mh < CAMERA_H / TILE_H)
		*cam_y = CAMERA_H / 2 - mh / 2;
	else if (mh == CAMERA_H / TILE_H)
		*cam_y = 0;
	else 
		for (*cam_y = 0; *cam_y + 100 != ny; *cam_y += TILE_H)
			;

}

void
nate_draw (NATE *n)
{
	sprite_draw (n->s, get_backbuff (), n->dir, n->cf, n->x, n->y-TILE_W);
}

void
nate_set_xy (NATE *n, int x, int y)
{
	n->x = x;
	n->y = y;
}

void
nate_def (NATE *n)
{
	n->x = 5 * TILE_W;
	n->y = 5 * TILE_H;
	n->ar = 0;
	n->dir = DOWN;
	n->cf = 0;
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
