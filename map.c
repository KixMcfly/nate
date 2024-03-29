#include <allegro.h>
#include "map.h"

struct MAP{
	
	char 			*name;
	unsigned short 	w;
	unsigned short 	h;
	int				lw;
	int 			lh;
	unsigned char   tw;
	unsigned char	th;
	int				nl;
	int 			nts;
	int				ni;

	NNODE			*so;
	LAYER			*l_list;
	TILESET			*ts_list;
	BITMAP			**im_list;

};

struct LAYER{
	
	int type;
	BITMAP *img;
	void *data;
	
};

struct TILE{
	
	unsigned short tn;
	unsigned char ts;
	unsigned char flags;
};


void
map_log (MAP *m, int nx, int ny)
{
	int x, y;
	FILE *ol = fopen ("log.txt", "a");
	TILE *data = m->l_list[0].data;
	
	fprintf (ol, "NATE X: %d\n", nx);
	fprintf (ol, "NATE Y: %d\n", ny);
	fprintf (ol, "MAP WIDTH: %d\n", m->w);
	fprintf (ol, "MAP HEIGHT: %d\n", m->h);
	for (y = 0; y < m->h; y++){
		for (x = 0; x < m->w; x++)
			fprintf (ol, "%02x ", data[y*m->w+x].flags);
			
		fprintf (ol, "\n");
	}

	fclose (ol);
}

int
map_get_tw (MAP *m)
{
	return m->tw;
}

int
map_get_th (MAP *m)
{
	return m->th;
}

int
map_get_w (MAP *m)
{
	return m->w;
}

int
map_get_h (MAP *m)
{
	return m->h;
}

int
map_get_nl (MAP *m)
{
	return m->nl;
}

int
map_get_lw (MAP *m)
{
	return m->lw;
}

int
map_get_lh (MAP *m)
{
	return m->lh;
}

int
map_get_nts (MAP *m)
{
	return m->nts;
}

char *
map_get_name (MAP *m)
{
	return m->name;
}

BITMAP *
map_get_layer_img (MAP *m, int ln)
{
	return m->l_list[ln].img;
}

BITMAP *
map_get_object_ass (MAP *m, int id)
{
	return m->im_list[id];
}

int
map_get_layer_type (MAP *m, int ln){
	return m->l_list[ln].type;
}

unsigned char
map_get_tile_flags (MAP *m, int ln, int x, int y)
{
	TILE *data;
	if (ln < m->nl){
		data = m->l_list[ln].data;
		return data[x+m->w*y].flags;
	}else
		return 0;
}

NNODE *
map_get_node_head (MAP *m)
{
	return m->so;
}

MAP *
map_new (void)
{
	return (MAP *)malloc (sizeof (MAP));
}

int
map_get_elev_x_pos (MAP *m)
{
	NNODE *cn = m->so;
	
	while (cn){
		
		if (node_get_type (cn) == OBJ_CHGROOM){
			CHGROOM *cr = node_get_data (cn);
			if (!strcmp(cr->name, "ELEV_NAT"))
				return cr->x;
		}
		
		cn = node_get_next (cn);
	}
	
	return -1;
}

int
map_get_elev_y_pos (MAP *m)
{
	NNODE *cn = m->so;
	
	while (cn){
		
		if (node_get_type (cn) == OBJ_CHGROOM){
			CHGROOM *cr = node_get_data (cn);
			
			if (!strcmp(cr->name, "ELEV_NAT"))
				return cr->y;
		}
		
		cn = node_get_next (cn);
	}
	
	return -1;
}

int
load_map (MAP *m, char *dat_fn, char *dat_id)
{
	char *dat_fnid, *type, head[5];
	TILE *t_data;
	int size, i, co, noo, x, y, ntl, c, ci;
	signed int len;
	PACKFILE *fp;
	
	/* Non moving objects */
	m->so = NULL;
	
	/* Start map object list with none */
	
	size = strlen (dat_fn);
	size += strlen (dat_id);
	
	/* 1 char for '#' and another for NULL term */
	size += 2;

	dat_fnid = (char *)malloc (size);
	
	if (!dat_fnid) return 0;
	
	sprintf (dat_fnid, "%s%s%s",  dat_fn, "#", dat_id);
	
	fp = pack_fopen (dat_fnid, F_READ);
	free (dat_fnid);
	
	if (!fp) return -1;
	
	/* Ensure file is a NAT file */
	pack_fread (head, 5, fp);

	/* Map name */
	size = pack_getc (fp);
	m->name = (char *)malloc (size);
	
	if (m->name == NULL){
		pack_fclose (fp);
		return -2;
	}
	
	pack_fread (m->name, size, fp);
	
	/* Map width and height in tiles */
	m->w = (unsigned short) pack_igetw (fp);
	m->h = (unsigned short) pack_igetw (fp);
	
	/* Tile width and height in pixels */
	m->tw = (unsigned char) pack_getc (fp);
	m->th = (unsigned char) pack_getc (fp);
	
	/* Logical / physical width and height of map */
	m->lw = m->w * m->tw;
	m->lh = m->h * m->th;
	
	/* Get number of Tilesets */
	size = pack_getc (fp);
	m->nts = size;
	m->ts_list = (TILESET *)malloc (size * sizeof (TILESET));
	
	if (m->l_list == NULL){
		free (m->name);
		pack_fclose (fp);
		return -3;
	}

	/* Load tilesets the map uses */
	for (i = 0; i < size; i++){
		
		int len = pack_getc (fp);
		char *ts_id = (char *)malloc (len);
		int ret;
		
		pack_fread (ts_id, len, fp);

		ret = load_tileset (&m->ts_list[i], dat_fn, ts_id, m->tw, m->th);

		free (ts_id);
		
		if (ret != 1)
			printf ("FAILED LOADING TILESET %d RETURN CODE %d\n", i, ret);
	}

	/* Get number of Object image assets */
	m->ni = pack_getc (fp);
	if (m->ni){
		m->im_list = (BITMAP **) malloc (m->ni * sizeof (BITMAP *));
	}else
		m->im_list = NULL;
		
	/* Get object image assest */
	for (i = 0; i < m->ni; i++){
		int len = pack_getc (fp);
		char *tn_id = (char *) malloc (len);
		BITMAP *t_bmp;
		DATAFILE *t_dat;
		
		pack_fread (tn_id, len, fp);

		t_dat = load_datafile_object (dat_fn, tn_id);
		t_bmp = (BITMAP *)t_dat->dat;
		
		m->im_list[i] = create_bitmap (t_bmp->w, t_bmp->h);
		blit (t_bmp, m->im_list[i], 0, 0, 0, 0, t_bmp->w, t_bmp->h);
		
		unload_datafile_object (t_dat);
		
		free (tn_id);
	}

	/* Get number of layers */
	m->nl = pack_getc (fp);
	m->l_list = (LAYER *)malloc (m->nl * sizeof (LAYER));

	/* Parse layer data */
	for (i = 0, ntl = 0; i < m->nl; i++){
		
		/* Get layer type */
		m->l_list[i].type = pack_getc (fp);
		
		switch (m->l_list[i].type){
			case TILES:
			
				m->l_list[i].data = (TILE *)malloc((m->w*m->h) * sizeof (TILE));
	
				t_data = m->l_list[i].data;
				/* Get layer data*/
				for (c = 0; c < m->w*m->h; c++){

					t_data[c].tn = (unsigned short)pack_igetw (fp);
					
					/* 0xFFFF means no tile or data */
					if (t_data[c].tn ^ 0xFFFF){
						t_data[c].ts = pack_getc (fp);
						t_data[c].flags = pack_getc (fp);
					}else{
						t_data[c].ts = 0;
						t_data[c].flags = 0;
					}
				}
				
				ntl++;
			break;
			case OBJECTS:

				noo = pack_getc (fp);
				
				/* Subtract layer count as object layers are not
				 * drawn at layer iteration */
				m->nl -= 1;
				
				for (co = 0; co < noo; co++){
					unsigned long x, y;
					/* Type string length */
					size = pack_getc (fp);
					type = (char *)malloc (size);
					pack_fread (type, size, fp);
				
					/* X Y coord of object */
					x = pack_igetw (fp);
					y = pack_igetw (fp);

					if (!strcmp (type, "COMPUTER")){
						GENERIC *gen = (GENERIC *) malloc (sizeof (GENERIC));
						gen->x = x;
						gen->y = y;
						m->so = node_add (m->so, OBJ_COMPUTER, gen);
					
					}else if (!strcmp (type, "ITEMBOX")){
						GENERIC *gen = (GENERIC *) malloc (sizeof (GENERIC));
						gen->x = x;
						gen->y = y;
						m->so = node_add (m->so, OBJ_ITEMBOX, gen);
							
					}else if (!strcmp (type, "CHGROOM")){

						CHGROOM *room = (CHGROOM *) malloc (sizeof (CHGROOM));
						
						/* Get room name to change to */
						len = pack_getc (fp);
						pack_fread (room->name, len, fp);
						
						room->x = x;
						room->y = y;
						room->cx = pack_igetw (fp);
						room->cy = pack_igetw (fp);
						
						/* Add object to map object list */
						m->so = node_add (m->so, OBJ_CHGROOM, room);
					}else if (!strcmp (type, "VENDING")){
						
						VENDING *vend = (VENDING *) malloc (sizeof (VENDING));
						
						vend->x = x;
						vend->y = y;

						for (ci = 0; ci < 20; ci++)
							vend->inv_list[ci] = pack_igetw (fp);
						
						/* Add object to map object list */
						m->so = node_add (m->so, OBJ_VENDING, vend);
					}else if (!strcmp (type, "STAT")){
						GENERIC *gen = (GENERIC *) malloc (sizeof (GENERIC));
						gen->x = x;
						gen->y = y;
						m->so = node_add (m->so, OBJ_STAT, gen);
					}else if (!strcmp (type, "ELEV_BUTT")){
						GENERIC *gen = (GENERIC *) malloc (sizeof (GENERIC));
						gen->x = x;
						gen->y = y;
						m->so = node_add (m->so, OBJ_ELEV_BUTT, gen);
					}
					//}else if (!strcmp (type, "ENEMY")){
					//	int ca;
					//	ENEMY *enemy = (ENEMY *) malloc (sizeof (ENEMY));
					//	
					//	/* Enemy XY pos*/
					//	enemy->x = x;
					//	enemy->y = y;
					//	
					//	/* Image ID */
					//	enemy->imageid = pack_igetw (fp);
					//	
					//	/* Enemy name */
					//	len = pack_getc (fp);
					//	pack_fread (enemy->name, len, fp);
					//	
					//	/* Max health */
					//	enemy->max_health = pack_igetw (fp);
					//	
					//	/* Set cur health */
					//	enemy->cur_health = enemy->max_health;
					//	
					//	/* Money */
					//	enemy->money = pack_igetw (fp);
					//	
					//	/* Item */
					//	enemy->item = pack_igetw (fp);
					//	
					//	/* Number of attacks */
					//	enemy->na = pack_getc (fp);
					//	
					//	enemy->att_l = (ATTACK *) malloc (enemy->na * sizeof (ATTACK));
					//	
					//	for (ca = 0; ca < enemy->na; ca++){
					//		
					//		/* Attack name */
					//		len = pack_getc (fp);
					//		pack_fread (enemy->att_l[ca].name, len, fp);
					//
					//		/* Attack desc */
					//		len = pack_getc (fp);
					//		pack_fread (enemy->att_l[ca].desc, len, fp);
					//		
					//		/* Attack damage */
					//		enemy->att_l[ca].dam = pack_igetw (fp);
					//		
					//		/* Attack probability */
					//		enemy->att_l[ca].prob = pack_getc (fp);
					//	}
					//	
					//	m->so = node_add (m->so, OBJ_ENEMY, enemy);
					//}

					free (type);
				}
			break;
				
		}
	}
	
	/* Create buffers for layers */
	for (i = 0; i < m->nl; i++){
		
		if (m->l_list[i].type == TILES){
			
			TILE *data = m->l_list[i].data;
			
			m->l_list[i].img = create_bitmap (m->w*m->tw, m->h*m->th);
			
			for (y = 0; y < m->h; y++){
				for (x = 0; x < m->w; x++){
					unsigned char ts = data[y*m->w+x].ts;
					unsigned char tn = data[y*m->w+x].tn;
					
					if (data[y*m->w+x].tn ^ 0xFFFF){
						blit (m->ts_list[ts].tiles[tn],
							m->l_list[i].img, 0, 0,
							x*m->tw, y*m->th, m->tw, m->th);
					}else{
						rectfill(m->l_list[i].img,
							x*m->tw, y*m->th,
							x*m->tw+m->tw, y*m->th+m->th, 0);
					}
				}
			}
		}else{
			m->l_list[i].img = NULL;
		}
	}
	
	pack_fclose (fp);

	return 1;
}

int
map_free (MAP *m)
{
	int i, ct;
	NNODE *cur;

	if (m){

		free (m->name);
		
		for (i = 0; i < m->nl; i++){
			if (m->l_list[i].type == TILES){
			
				free (m->l_list[i].data);
				m->l_list[i].data = NULL;
			
				if (m->l_list[i].img)
					destroy_bitmap (m->l_list[i].img);
			}
		}
		
		free (m->l_list);
		
		/* Destroy image assets */
		for (i = 0; i < m->ni; i++)
			destroy_bitmap (m->im_list[i]);
		
		if (m->ni)
			free (m->im_list);
		
		for (i = 0; i < m->nts; i++){
			int ct;
			for (ct = 0; ct < m->ts_list[i].nt; ct++){

				destroy_bitmap (m->ts_list[i].tiles[ct]);
			}
				
			free (m->ts_list[i].tiles);
			m->ts_list[i].tiles = NULL;
		}

		/* Check nodes for enemy types to free attack data */
		cur = m->so;
		while (cur){
			
			if (node_get_type (cur) == OBJ_ENEMY){
				ENEMY *enemy = (ENEMY *) node_get_data (cur);
				
				if (enemy->att_l)
					free (enemy->att_l);
			}
			cur = node_get_next (cur);
		}

		m->so = node_clear (m->so);
		free (m->ts_list);
		m->ts_list = NULL;
		free (m);
		
		m = NULL;
		
		return TRUE;
	}else{

		return FALSE;
	}
	
}
