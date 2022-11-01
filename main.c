#include <dos.h>
#include "main.h"

int main (int argc, char **argv)
{	
	MAP *m = NULL;
	NATE nate;
	CHGROOM *cr = NULL;
	GENERIC *gn = NULL;
	VENDING *vn = NULL;
	BITMAP *title = NULL;
	DATAFILE *df = NULL, *pal = NULL, *snd_door = NULL;
	NNODE *cn = NULL;
	char *text_msg = NULL:
	
	int nl, cl, quit = FALSE, cam_x = 0, cam_y = 0, cam_dx = 0, cam_dy = 0;
	int game_speed = 10;

	/* Initialize Allegro */
	nate_init ();
	
	/* TITLE SCREEN */
	draw_load_blit_show (NATE_DAT, "TITLE_BMP");
	sound_midi_load_play ("TITLE_MID", NATE_DAT);

	while (!keypressed ())
		;

	fadeout (10);
	sound_midi_stop_free ();
	
	/* PREPARE MAIN GAME LOOP */

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

	nate_focus_camera (m, nate.x, nate.y, &cam_x, &cam_y);
	
	/* Get palette for fade in */
	pal = load_datafile_object (NATE_DAT, "NATE_PAL");
	
	elev_floor_goto ();

	/* MAIN GAME LOOP */
	while (!quit){
	
		if (key[KEY_Q])
			quit = TRUE;
			
		if (key[KEY_UP]){
			
			if (invmenu_vis ()){
				invmenu_sel_up ();
			}else if (temp_vis ()){
				temp_pos_up ();
			}else if (elev_vis ()){
				elev_sel_up ();
			}else if (vend_vis ()){
				vend_move_up ();
			}else {
				
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x), LY(nate.y)-1)) &&
					!SOLID(map_get_tile_flags (m, 1, LX(nate.x), LY(nate.y)-1)) &&
					LY(nate.y) - 1 > -1){
						
						nate.y--;
					}

				nate.ckf = KF_UP;
			}
		}
		
		if (key[KEY_DOWN]){
			
			if (invmenu_vis ()){
				invmenu_sel_down ();
			}else if (temp_vis ()){
				temp_pos_down ();
			}else if (elev_vis ()){
				elev_sel_down ();
			}else if (vend_vis ()){
				vend_move_down ();
			}else{
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x), LY(nate.y)+1)) &&
					!SOLID(map_get_tile_flags (m, 1, LX(nate.x), LY(nate.y)+1)) &&
					LY(nate.y) + 1 < map_get_h (m)){
						nate.y++;
					}

				nate.ckf = KF_DOWN;
			}
		}
			
		if (key[KEY_LEFT]){
			
			if (invmenu_vis ()){
				invmenu_sel_left ();
			}else if (vend_vis ()){
				vend_move_left ();
			}else if (elev_vis ()){
				elev_sel_left ();
			}else{
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x)-1, LY(nate.y))) &&
					!SOLID(map_get_tile_flags (m, 1, LX(nate.x)-1, LY(nate.y))) &&
					LX(nate.x)-1 > -1){
						nate.x--;
					}
					
				nate.ckf = KF_LEFT;
			}
		}
			
		if (key[KEY_RIGHT]){
			if (invmenu_vis ()){
				invmenu_sel_right ();
			}else if (vend_vis ()){
				vend_move_right ();
			}else if (elev_vis ()){
				elev_sel_right ();
			}else{
				if (!SOLID(map_get_tile_flags (m, 0, LX(nate.x)+1, LY(nate.y))) &&
					!SOLID(map_get_tile_flags (m, 1, LX(nate.x)+1, LY(nate.y))) &&
					LX(nate.x) + 1 < map_get_w (m)){
						nate.x++;
					}
				
				nate.ckf = KF_RIGHT;
			}
		}
		
		if (key[KEY_ESC]){
			if (vend_vis ()){
				vend_free_dat ();
			}else if (temp_vis ()){
				temp_uninit ();
			}else if (elev_vis ()){
				elev_free ();
			}else {
				if (!invmenu_vis ())
					invmenu_init (NATE_DAT, "INVMENU_BMP", "ITEMS_BMP");
				else
					invmenu_free ();
			}
			
			fadeout (20);
		}
		
		/* Hotel temp always factored */
		temp_global_process ();

		/* NATE ANIMATION SPEED CONTROL */
		nate.ar--;
		if (nate.ar <= 0){
			
			nate.cf++;
			if (nate.cf >= sprite_keyframe_get_num_frames (nate.s, nate.ckf))
				nate.cf = 0;
				
			nate.ar = sprite_keyframe_get_frame_rest (nate.s, nate.ckf, nate.cf);
		}

		/* Check objects. Don't if any GUIs are active or no objects
		 * are in map */
		cn = map_get_node_head (m);
		while (cn &&
		       !vend_vis () && 
			   !invmenu_vis () &&
			   !temp_vis () &&
			   !elev_vis () &&
			   !fighting ()
			){
		
			/* Nate's computer will be a computer in a 
			 * computer */
			if (node_get_type (cn) == OBJ_COMPUTER ){
				
				gn = node_get_data (cn);
				if (nate.x == gn->x && nate.y == gn->y){
					
					break;
				}

			/* Nate activates a thermostat */
			}else if (node_get_type (cn) == OBJ_STAT){
				
				gn = node_get_data (cn);
				if (nate.x == gn->x && nate.y == gn->y){
					if (key[KEY_LCONTROL])
						temp_set_vis ();

						
					break;
				}
				
			/* Nate opens an itembox */
			}else if (node_get_type (cn) == OBJ_ITEMBOX){
				
				gn = node_get_data (cn);
				if (nate.x == gn->x && nate.y == gn->y){
					
					if (key[KEY_LCONTROL]){
						invmenu_init (NATE_DAT, "INVMENU_BMP", "ITEMS_BMP", "INV_FNT");
						boxmenu_set_active (TRUE);
					}
						
					break;
				}
				
			/* Nate checks out what's in the vending machine */
			}else if (node_get_type (cn) == OBJ_VENDING){
				
				vn = node_get_data (cn);
				if (nate.x == vn->x && nate.y == vn->y){
					if (key[KEY_LCONTROL])
						vend_init_dat (vn, NATE_DAT, "VEND_BMP", "ITEMS_BMP");

						
					break;
				}
			/* Nate looks at what floors are open on elevator */
			}else if (node_get_type (cn) == OBJ_ELEV_BUTT){
				
				gn = node_get_data (cn);
				if (nate.x == gn->x && nate.y == gn->y){
					if (key[KEY_LCONTROL])
						elev_init (NATE_DAT, "ELEV_BMP");

						
					break;
				}
			
			/* Nate is standing on room change object */
			}else if (node_get_type (cn) == OBJ_CHGROOM){
				cr = node_get_data (cn);
				
				if (nate.x == cr->x && nate.y == cr->y){

					
					/* Change to room if use button is pressed */
					if (key[KEY_LCONTROL]){
						nate_set_xy (&nate, cr->cx * TILE_W, cr->cy * TILE_H);
						play_sample ((SAMPLE *)snd_door->dat, 155, 128, 1000, NULL);
						//text_msg = strtmp (
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

		/* GUI CONTROL */
		
		/* Vending machine */
		if (vend_vis ()){
			
			int tc = inv_get_item_total (INV_MONEY), ib;

			if (key[KEY_LCONTROL]){
				ib = vend_buy_item (vn, tc);
				inv_add (ib, 1);
				inv_sub (INV_MONEY, vend_get_cost (ib));
			}

			vend_draw_backbuff (vn, tc, get_backbuff ());
		
		/* Thermostat */
		}else if (temp_vis ()){
			
			temp_draw_backbuff (get_backbuff (), NATE_DAT);
			
		/* Elevator buttons */
		}else if (elev_vis ()){
			
			if (key[KEY_LCONTROL])
				elev_press ();
			
			elev_draw_backbuff (get_backbuff ());
		
		/* Item box or inv menu */	
		}else if (invmenu_vis ()){
		
			if (key[KEY_LCONTROL]){
				if (boxmenu_active ())
					boxmenu_set_src_dest ();
			}
			
			invmenu_draw_backbuff (get_backbuff ());
		
		/* No special GUI visible? Draw game map and stuff instead */
		}else{
			/* Draw map layers */
			clear (get_backbuff ());
			for (cl = 0; cl < nl; cl++)
				draw_map_layer (m, cl, -cam_x, -cam_y);
		

			sprite_draw (nate.s, get_backbuff (), nate.ckf, nate.cf, nate.x-cam_x, nate.y-cam_y-TILE_H);
			
		}
		
		/* Special use key check to exit elevator room */
		if (key[KEY_LCONTROL]){
			
			if (!strcmp (map_get_name (m), "Elevator") &&
					  LX (nate.x) == 2 && LY (nate.y) == 3){
			
				int nx, ny;
				char *fs;

				map_free (m);
				m = map_new ();
				
				fs = elev_get_floor_goto_name ();
				
				if (fs[0] != '\0')
					load_map (m, NATE_DAT, elev_get_floor_goto_name ());
				else
					load_map (m, NATE_DAT, "HOTEL99_NAT");
				nl = map_get_nl (m);
				fadeout (5);
				
				nx = map_get_elev_x_pos (m);
				ny = map_get_elev_y_pos (m);
				nate_set_xy (&nate, nx, ny+map_get_th(m));
				
				/* Set camera based on nate location */
				nate_focus_camera (m, nate.x, nate.y, &cam_x, &cam_y);
			}
		}
		
		/* Blit backbuff to CRT */
		show_backbuff (0, 0);
		
		if (faded ())
			fadein (pal->dat, 64);
		
		while (elapsed_time > 0)
			;
			
		elapsed_time = game_speed;
	}
	
	/* Unload sounds */
	unload_datafile_object (snd_door);
	
	/* free nate sprite */
	sprite_free (nate.s);
	
	/* free pal */
	unload_datafile_object (pal);
	
	map_free (m);

	nate_exit ();
	return 0;
}
//END_OF_MAIN()

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
	
	if (mw < CAMERA_W / tw){
		*cam_x = -((CAMERA_W - mw * TILE_W) / 2);
	}else
		for (*cam_x = 0; *cam_x + 160 <= nx; *cam_x += tw)
			;

	if (mh < CAMERA_H / th)
		*cam_y = -((CAMERA_H - mh * TILE_H) / 2);
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
	n->x = 2 * TILE_W;
	n->y = 4 * TILE_H;
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
	
	LOCK_VARIABLE( elapsed_time );
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
