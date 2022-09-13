#include "elev.h"

#define EREST_MAX 	30
#define X			0
#define Y			1

typedef struct {
	
	unsigned long id;
	char name[30];
} FLOOR;

struct ELEV {
	
	int nf;
	int cf;
	int lf;
	NODE *fl;

} static elev = {
	.nf = 0,
	.cf = 0,
	.lf = 0,
	.fl = NULL
};

static int e_rest = 0, vis = FALSE, cp = 0;
static BITMAP *elev_bmp = NULL;
static char sel[3] = {'0', '0', '\0'}, sel_p = 1;
static FONT *temp_fnt = NULL;

static int sp[12][2] = {
				{168, 27}, {181, 27}, {194, 27}, {207, 27},
				{168, 40}, {181, 40}, {194, 40}, {207, 40},
				{168, 53}, {181, 53}, {194, 53}, {207, 53}
			};

static FLOOR floor_to_load = { .id = 0, .name[0] = '\0'};

static void
elev_push_floor (unsigned long id, char *fn)
{
	FLOOR *nf = (FLOOR *) malloc (sizeof (FLOOR));
	
	nf->id = id;
	strcpy (nf->name, fn);
	elev.fl = node_add (elev.fl, NULL, nf);
}

int
elev_vis (void)
{
	return vis;
}

void
elev_sel_up (void)
{
	if (cp > 3 && !e_rest){
		cp -= 4;
		e_rest = 20;
	}
}

void
elev_sel_down (void)
{
	if (cp < 7 && !e_rest){
		cp += 4;
		e_rest = 20;
	}
}

void
elev_sel_left (void)
{
	if ((cp > 0 || cp > 4 || cp > 8) && !e_rest){
		cp--;
		e_rest = 20;
	}
}

void
elev_sel_right (void)
{
	if ((cp < 3 || cp < 7 || cp < 11) && !e_rest){
		cp++;
		e_rest = 20;
	}
}

int
elev_get_floor_goto_id (void)
{
	return floor_to_load.id;
}

char *
elev_get_floor_goto_name (void)
{
	return floor_to_load.name;
}

static void
elev_floor_goto (void)
{
	if (!e_rest){
		
		NODE *cf = elev.fl;
		unsigned long sfn = strtol (sel, NULL, 10);
		
		while (cf){
			FLOOR *fl = node_get_data (cf);
			if (fl->id == sfn){
				elev.cf = fl->id;
				floor_to_load.id = fl->id;
				strcpy (floor_to_load.name, fl->name);
				break;
			}
			cf = node_get_next (cf);
		}
		
		e_rest = 20;
	}
}

void
elev_press (void)
{
	if (!e_rest){
		
		if (cp < 10){
		
			if (sel_p == 0){
				sel[0] = '0';
				sel[1] = cp + '0';
			}else if (sel_p == 1){
				sel[0] = sel[1];
				sel[1] = cp + '0';
			}

			sel_p++;

			if (sel_p > 1)
				sel_p = 0;
		
		}else if (cp == 11){
			elev_floor_goto ();
		}
		
		e_rest = 20;
	}
}

void
elev_draw_backbuff (BITMAP *bf)
{

	if (e_rest)
		e_rest--;

	if (elev_bmp){
		blit (elev_bmp, bf, 0, 0, 0, 0, elev_bmp->w, elev_bmp->h);
		rect (bf, sp[cp][X], sp[cp][Y], sp[cp][X]+13, sp[cp][Y]+13, 2);
		textprintf_ex (bf, temp_fnt, 102, 28, 14, -1, "%s", sel);
	}
}

void
elev_free (void)
{
	if (!e_rest){
		elev.fl = node_clear (elev.fl);
		destroy_bitmap (elev_bmp);
		destroy_font (temp_fnt);
		elev_bmp = NULL;
		vis = FALSE;
	}
}

int
elev_init (char *dfn, char *bn)
{
	int i;
	char *names[] = {"THER_FNT", NULL};
	const int type = DAT_ID('T','Y','P','E');
	const int name = DAT_ID('N','A','M','E');
	NODE *cf = NULL;
	BITMAP *tb = NULL;
	DATAFILE *df = NULL;

	temp_fnt = load_dat_font (dfn, NULL, names);

	df = load_datafile (dfn);

	/* Count how many hotel maps in datafile */
	for (i = 0, elev.nf = 0; df[i].type != DAT_END; i++)
		if(!stricmp(get_datafile_property(df+i, type), "MAPD")){
			const char *mfn = get_datafile_property (df+i, name);
			if (strstr (mfn, "HOTEL")){
				int c = 0;
				while (!isdigit(mfn[c]) && mfn[c])
					c++;
				
				elev_push_floor (strtol (&mfn[c], NULL, 10), (char *)mfn);
				elev.nf++;
			}
		}

	unload_datafile (df);
	
	df = load_datafile_object (dfn, bn);
	tb = df->dat;
	
	elev_bmp = create_bitmap (tb->w, tb->h);
	blit (tb, elev_bmp, 0, 0, 0, 0, tb->w, tb->h);
	unload_datafile_object (df);
	
	cf = elev.fl;
	while (cf){
		FLOOR *fl = node_get_data (cf);
		int x = 97 + fl->id % 10 * 13, y = 69 + fl->id / 10 * 8;
		
		rectfill (elev_bmp, x, y, x+3, y+7, 2);
		cf = node_get_next (cf);
	}
	
	vis = TRUE;
	e_rest = EREST_MAX;

	return 1;
}
