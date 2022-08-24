#include <allegro.h>
#include "main.h"

int main (void)
{	
	MAP *m = NULL;
	NATE nate;
	CHGROOM *cr = NULL;
	GENERIC *gn = NULL;
	VENDING *vn = NULL;
	BITMAP *title = NULL;
	DATAFILE *df = NULL, *pal = NULL, *snd_door = NULL, *inv_bmp = NULL;
	NODE *cn = NULL;
	char *text_msg = NULL;
	
	int nl, cl, quit = FALSE, cam_x = 0, cam_y = 0, cam_dx = 0, cam_dy = 0;

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
	
	/* Load title MIDI */
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
	
	grid_set_w (20);
	grid_set_h (20);
	
	grid_snap_queue_add (&cam_x, &cam_y, &cam_dx, &cam_dy);
	grid_snap_queue_add (&nate.x, &nate.y, &nate.dx, &nate.dy);

	nate_focus_camera (m, nate.x, nate.y, &cam_x, &cam_y);
	
	/* Inv BMP */
	inv_bmp = load_datafile_object (NATE_DAT, "INVMENU_BMP");
	
	/* Get palette for fade in */
	pal = load_datafile_object (NATE_DAT, "NATE_PAL");
	
	df = load_datafile_object (NATE_DAT, "NATEROOM_MID");
	//play_midi ((MIDI *)df->dat, TRUE);

	inv_init ();

	while (!quit){
	
		if (key[KEY_Q])
			quit = TRUE;
			
		if (key[KEY_UP]){
			
			if (invmenu_vis ()){
				invmenu_sel_up ();
			}else if (vend_vis ()){
				
				vend_move_up ();
			}else{
				
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x), LY(nate.y)-1)) &&
					LY(nate.y) - 1 > -1){
						grid_snap_up (&nate.x, &nate.y, &nate.dy);
					}

				nate.ckf = KF_UP;
			}
		}
		
		if (key[KEY_DOWN]){
			
			if (invmenu_vis ()){
				
				invmenu_sel_down ();
			}else if (vend_vis ()){
				
				vend_move_down ();
			}else{
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x), LY(nate.y)+1)) &&
					LY(nate.y) + 1 < map_get_h (m)){
						grid_snap_down (&nate.x, &nate.y, &nate.dy);
					}

				nate.ckf = KF_DOWN;
			}
		}
			
		if (key[KEY_LEFT]){
			
			if (invmenu_vis ()){
					
				invmenu_sel_left ();
				
			}else if (vend_vis ()){
				vend_move_left ();
			}else {
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x)-1, LY(nate.y))) &&
					LX(nate.x)-1 > -1){
						grid_snap_left (&nate.x, &nate.y, &nate.dx);
					}
					
				nate.ckf = KF_LEFT;
			}
		}
			
		if (key[KEY_RIGHT]){
			if (invmenu_vis ()){

				invmenu_sel_right ();
				
			}else if (vend_vis ()){
				
				vend_move_right ();
				
			}else{
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x)+1, LY(nate.y))) &&
					LX(nate.x) + 1 < map_get_w (m)){
						grid_snap_right (&nate.x, &nate.y, &nate.dx);
					}
				
				nate.ckf = KF_RIGHT;
			}
		}
		
		if (key[KEY_ESC]){

			if (vend_vis ()){
				vend_free_dat ();
			}else {
				
				if (!invmenu_vis ())
					invmenu_show ();
				else
					invmenu_hide ();
			}
		
			fadeout (20);
		}
		
		/* Nate inventory managment */
		invmenu_process ();

		/* Camera adjust */
		if (nate.x - cam_x <= 60 && cam_x > 0)
			grid_snap_left (&cam_x, &cam_y, &cam_dx);
			
		if (nate.x - cam_x >= 220 && cam_x + SCREEN_W < map_get_lw (m))
			grid_snap_right (&cam_x, &cam_y, &cam_dx);
			
		if (nate.y - cam_y <= 80 && cam_y)
			grid_snap_up (&cam_x, &cam_y, &cam_dy);
			
		if (nate.y - cam_y >= 120 && cam_y + SCREEN_H < map_get_lh (m))
			grid_snap_down (&cam_x, &cam_y, &cam_dy);
	
		/* Snap all queued coord to grid */
		grid_snap_queue_proc ();

		/* NATE ANIMATION SPEED CONTROL */
		nate.ar--;
		if (nate.ar <= 0){
			nate.cf++;
	
			if (nate.cf >= sprite_keyframe_get_num_frames (nate.s, nate.ckf))
				nate.cf = 0;
				
			nate.ar = sprite_keyframe_get_frame_rest (nate.s, nate.ckf, nate.cf);
		}

		/* Check objects */
		cn = map_get_node_head (m);
		while (cn && !vend_vis () && !invmenu_vis ()){
		
			if (node_get_type (cn) == OBJ_COMPUTER){
				
				gn = node_get_data (cn);
				if (nate.x == gn->x && nate.y == gn->y){
		
				}
			}else if (node_get_type (cn) == OBJ_VENDING){
				
				vn = node_get_data (cn);

				if (nate.x == vn->x && nate.y == vn->y){
					
					if (key[KEY_LCONTROL]){
						vend_init_dat (vn, NATE_DAT, "VEND_BMP", "ITEMS_BMP");
					}else
						text_msg = strtmp ("Vending machine!");
				}
				
			}else if (node_get_type (cn) == OBJ_CHGROOM){
				cr = node_get_data (cn);
				
				if (nate.x == cr->x && nate.y == cr->y){
					
					text_msg = strtmp (cr->name);
					
					/* Change to room if use button is pressed */
					if (key[KEY_LCONTROL]){
						nate_set_xy (&nate, cr->cx * TILE_W, cr->cy * TILE_H);
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


		if (vend_vis ()){

			vend_draw_backbuff (vn, get_backbuff ());
			
		}else if (!invmenu_vis ()){
			/* Draw map layers */
			for (cl = 0; cl < nl; cl++)
				draw_map_layer (m, cl, -cam_x, -cam_y);
		
			/* draw nathyn on backbuff */
			sprite_draw (nate.s, get_backbuff (), nate.ckf, nate.cf, nate.x-cam_x, nate.y-cam_y-TILE_H);
			
			text_print_center (get_backbuff (), text_msg);
			text_msg = NULL;
			
		}else if (invmenu_vis ()){
			
			int px = invmenu_get_x (), py = invmenu_get_y ();
			int px2 = px + invmenu_get_w (), py2 = py + invmenu_get_h ();
			
			blit ((BITMAP *)inv_bmp->dat, get_backbuff (), 0, 0, 0, 0, 320, 200);
			rect(get_backbuff (), px, py, px2, py2, invmenu_get_c ());
			
		}
		
		show_backbuff (0, 0);
		
		if (faded ()){
			fadein (pal->dat, 64);
		}
		
		while (elapsed_time > 0)
			;
			
		elapsed_time = 10;
	}
	
	log_print ("============================\n");
	
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
	int mw, mh, tw, th;
	
	if (!m)
		return;
		
	mw = map_get_w (m);
	mh = map_get_h (m);
	
	tw = map_get_tw (m);
	th = map_get_th (m);
	
	if (mw < CAMERA_W / tw)
		*cam_x = CAMERA_W / 2 - mw / 2;
	else
		for (*cam_x = 0; *cam_x + 160 <= nx; *cam_x += tw)
			;

	if (mh < CAMERA_H / th)
		*cam_y = mh / 2 - CAMERA_H / 2;
	else{
		for (*cam_y = 0; *cam_y + 100 <= ny && *cam_y + CAMERA_H != mh * TILE_H; *cam_y += th)
			;
		}

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
	n->ckf = KF_DOWN;
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
