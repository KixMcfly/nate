#include "main.h"

int main (int argc, char **argv)
{
	NATE nate;
	MAP *cur_map = NULL;
	VENDING *vn = NULL;

	DATAFILE *pal = NULL;
	NNODE *cn = NULL;
	char *text_msg = NULL;
	
	int nl, cl, quit = FALSE, cam_x = 0, cam_y = 0, cam_dx = 0, cam_dy = 0;
	int game_speed = 10, menu_all_off = TRUE;
	
	/* Resolution of stat buff decrease */
	int food_res_max = 500, food_res = 0;
	int water_res_max = 500, water_res = 0;
	int sleep_res_max = 500, sleep_res = 0;
	
	/* Initialize Nathyn's Quest */
	nate_init ();

	init_hroom_stats (NATE_DAT);
	readkey ();

	/* Title Screen stuff */
	draw_load_blit_show (NATE_DAT, "TITLE_BMP", "TITLE_PAL");
	sound_midi_load_play (NATE_DAT, "TITLE_MID");

	while (!keypressed ())
		;
	
	fadeout (10);
	sound_midi_stop_free ();
	
	/* PREPARE MAIN LOOP */
	
	/* set starting room */
	cur_map = map_new ();
	load_map (cur_map, NATE_DAT, "NATEROOM_NAT");
	nl = map_get_nl (cur_map);

	/* Load player sprite */
	nate.s = sprite_new ();
	sprite_keyframe_dat_div (nate.s, 3, 4, NATE_DAT, "NATESPR_BMP");
	
	/* players starting stats */
	nate_def (&nate);

	/* focus view on nate / player position */
	nate_focus_camera (cur_map, nate.x, nate.y, &cam_x, &cam_y);
	
	/* Get palette for fade in */
	pal = load_datafile_object (NATE_DAT, "NATE_PAL");
	
	/* Set default dest floor */
	elev_floor_goto ();

	/* MAIN GAME LOOP */
	while (!quit){

		if (key[KEY_Q])
			quit = TRUE;
			
		if (key[KEY_UP]){
			
			invmenu_sel_up ();
			temp_pos_up ();
			elev_sel_up ();
			vend_move_up ();

			if (menu_all_off){
				
				//log_print ("NATE Y: %d\n", nate.y);
				
				if (nate.y){
					
					nate.y--;
				}
				
				if (nate.y < cam_y + CAMERA_H / 2)
					cam_y--;

				nate.ckf = KF_UP;
			}
		}
		
		if (key[KEY_DOWN]){
			
			invmenu_sel_down ();
			temp_pos_down ();
			elev_sel_down ();
			vend_move_down ();
			
			if (menu_all_off){
				if (nate.y + TILE_H + 1 < map_get_h (cur_map) * map_get_th (cur_map)){
					
					nate.y++;
					
					if (nate.y > cam_y + CAMERA_H / 2)
						cam_y++;
				}

				nate.ckf = KF_DOWN;
			}
		}
			
		if (key[KEY_LEFT]){
			
			invmenu_sel_left ();
			elev_sel_left ();
			vend_move_left ();
			
			if (menu_all_off){
				
				if (nate.x){
					
					nate.x--;
					
					if (nate.x < cam_x + CAMERA_W / 2)
						cam_x--;
				}

				nate.ckf = KF_LEFT;
			}
		}
			
		if (key[KEY_RIGHT]){
			
			invmenu_sel_right ();
			elev_sel_right ();
			vend_move_right ();
			
			if (menu_all_off){
				if (nate.x + TILE_W + 1 < map_get_w (cur_map) * map_get_tw (cur_map)){
					
					nate.x++;
					
					if (nate.x > cam_x + CAMERA_W / 2)
						cam_x++;
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
			}else{
				if (!invmenu_vis ())
					invmenu_init (NATE_DAT);
				else
					invmenu_free ();
			}
			fadeout (20);
		}
		
		if (key[KEY_LCONTROL]){
			/* Check objects. Don't if any GUIs are active or no objects
			* are in map */

			if (vend_vis ()){
				int item_id;
				
				item_id = vend_buy_item (vn, inv_get_item_total (INV_MONEY));
				
				if (item_id){
					inv_sub (INV_MONEY, vend_get_cost (item_id));
					inv_add (item_id, 1);
				}
				
			}else if (elev_vis ()){
				
				elev_press ();
				
			}else if (invmenu_vis ()){
				
				/* If item box in use, set to swap mode */
				if (boxmenu_active ())
					boxmenu_set_src_dest ();
					
				/* If just in the inventory menu, use item */
				else
					inv_item_use ();
					
			}else if (temp_vis ()){
				
				temp_draw_backbuff (get_backbuff (), NATE_DAT);

			/* Leave elevator */
			}else if (!strcmp (map_get_name (cur_map), "Elevator") &&
				nate_obj_at_pos (&nate, 2*20, 3*20, 20, 20)){
	
				int nx, ny;
				char *fs;
	
				map_free (cur_map);
				cur_map = map_new ();
				
				fs = elev_get_floor_goto_name ();
				
				if (fs[0] != '\0')
					load_map (cur_map, NATE_DAT, elev_get_floor_goto_name ());
				else
					load_map (cur_map, NATE_DAT, "HOTEL99_NAT");
				
				nl = map_get_nl (cur_map);
				fadeout (5);
				
				nx = map_get_elev_x_pos (cur_map);
				ny = map_get_elev_y_pos (cur_map);
				nate_set_xy (&nate, nx, ny+map_get_th(cur_map));

				nate_focus_camera (cur_map, nate.x, nate.y, &cam_x, &cam_y);
			
			}else{
				
				/* OBJ INTERACTION CHECK ***************/
				cn = map_get_node_head (cur_map);
				while (cn){

					if (node_get_type (cn) == OBJ_COMPUTER){
						
						GENERIC *gn = node_get_data (cn);
						if (nate_obj_at_pos (&nate, gn->x, gn->y, 20, 20)){
							
							break;
						}
	
					/* Nate activates a thermostat */
					}else if (node_get_type (cn) == OBJ_STAT){
						
						GENERIC *gn = node_get_data (cn);
						if (nate_obj_at_pos (&nate, gn->x, gn->y, 20, 20)){
							temp_set_vis ();
							break;
						}
						
					/* Nate opens an itembox */
					}else if (node_get_type (cn) == OBJ_ITEMBOX){
						
						GENERIC *gn = node_get_data (cn);
						if (nate_obj_at_pos (&nate, gn->x, gn->y, 20, 20)){
							invmenu_init (NATE_DAT);
							boxmenu_set_active (TRUE);
							break;
						}
						
					/* Nate checks out what's in the vending machine */
					}else if (node_get_type (cn) == OBJ_VENDING){
						
						vn = node_get_data (cn);
						if (nate_obj_at_pos (&nate, vn->x, vn->y, 20, 20)){
							vend_init_dat (vn, NATE_DAT, "VEND_BMP", "ITEMS_BMP");
							break;
						}
						
					/* Nate looks at what floors are open on elevator */
					}else if (node_get_type (cn) == OBJ_ELEV_BUTT){
						
						GENERIC *gn = node_get_data (cn);
						if (nate_obj_at_pos (&nate, gn->x, gn->y, 20, 20)){
							elev_init (NATE_DAT, "ELEV_BMP");
							break;
						}

					}else if (node_get_type (cn) == OBJ_CHGROOM){
						
						CHGROOM *cr = node_get_data (cn);
		
						/* Nate is standing on room change object */
						if (nate_obj_at_pos (&nate, cr->x, cr->y, 20, 20)){
		
							/* Change to room if use button is pressed */
							nate_set_xy (&nate, cr->cx * TILE_W, cr->cy * TILE_H);
							sound_play (SND_DOOR);
							text_msg = strtmp (cr->name);
							map_free (cur_map);
							cur_map = map_new ();
							load_map (cur_map, NATE_DAT, text_msg);
							
							nl = map_get_nl (cur_map);
							fadeout (5);
							
							/* Set camera based on nate location */
							nate_focus_camera (cur_map, nate.x, nate.y, &cam_x, &cam_y);
							break;
						}
					}
					
					cn = node_get_next (cn);
				} /* END OBJ INTERACTION CHECK */
			} /* OBJ LOOP END */
		} /* END USE KEY CHECK */

		/* Only adjust Nate stat buffs when not in menu */
		if (menu_all_off){
			/* Stat buff adjust */
			if (++food_res >= food_res_max){
				food_res = 0;
				adj_food (-1);
			}
				
			if (++water_res >= water_res_max){
				water_res = 0;
				adj_water (-1);
			}
				
			if (++sleep_res >= sleep_res_max){
				sleep_res = 0;
				adj_sleep (-1);
			}
		}

		/* NATE ANIMATION SPEED CONTROL */
		if (--nate.ar <= 0){
			
			nate.cf++;
			if (nate.cf >= sprite_keyframe_get_num_frames (nate.s, nate.ckf))
				nate.cf = 0;
				
			nate.ar = sprite_keyframe_get_frame_rest (nate.s, nate.ckf, nate.cf);
		}
		
		/* DRAW MAP LAYERS */
		clear (get_backbuff ());
		for (cl = 0; cl < nl; cl++)
			draw_map_layer (cur_map, cl, -cam_x, -cam_y);
		
		/* Temp draw current tile for testing */
		//rectfill (get_backbuff (), nate.x + -cam_x, nate.y + -cam_y, nate.x + 19 + -cam_x, nate.y + 19 + -cam_y, 124);
		
		/* DRAW NATE */
		sprite_draw (nate.s, get_backbuff (), nate.ckf, nate.cf, nate.x-cam_x, nate.y-cam_y-TILE_H);
		
		/* All menus off? Release main controls for nate interaction */
		menu_all_off = FALSE;
		if (!invmenu_vis () && !temp_vis () && !elev_vis () && !vend_vis ())
			menu_all_off = TRUE;
		
		/* Stuff that runs in the background */
		temp_global_process ();
		
		/* PROCESS MENUS */
		invmenu_draw_backbuff (get_backbuff ());
		elev_draw_backbuff (get_backbuff ());
		vend_process (vn, inv_get_item_total (INV_MONEY), get_backbuff ());
		temp_draw_backbuff (get_backbuff (), NATE_DAT);

		
		/* Blit backbuff to CRT */
		show_backbuff (0, 0);
		
		if (faded ())
			fadein (pal->dat, 64);
		
		while (elapsed_time > 0)
			;
			
		elapsed_time = game_speed;
	}

	/* free nate sprite */
	sprite_free (nate.s);
	
	/* free pal */
	unload_datafile_object (pal);
	
	map_free (cur_map);

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
	
	if (mw < CAMERA_W / tw){
		*cam_x = -((CAMERA_W - mw * tw) / 2);
	}else
		for (*cam_x = 0; *cam_x + 160 <= nx; *cam_x += tw)
			;

	if (mh < CAMERA_H / th)
		*cam_y = -((CAMERA_H - mh * th) / 2);
	else
		for (*cam_y = 0; *cam_y + 100 <= ny && *cam_y + CAMERA_H != mh * TILE_H; *cam_y += th)
			;
}

int
nate_obj_at_pos (NATE *n, int x, int y, int w, int h)
{
	if (n->x + 20 > x && n->x < x + w &&
	    n->y + 20 > y && n->y < y + h)
			return TRUE;
	
	return FALSE;
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
        allegro_message ("Error initialising sound: %s\n", allegro_error);
        install_sound (DIGI_NONE, MIDI_NONE, NULL);
        readkey ();
    }
    
    init_backbuff (320, 200);
    
    /* Load global sounds */
    sound_global_load (NATE_DAT);
}

void
nate_exit (void)
{
	/* free assets */
	sound_free ();
	
    remove_keyboard ();
    remove_sound ();
    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
    free_backbuff ();
    allegro_exit ();
}
