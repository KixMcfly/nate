#include "elev.h"

typedef struct FLOOR FLOOR;
struct FLOOR {
	
	unsigned long id;
	char name[30];
};

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

static void
elev_push_floor (unsigned long id, char *fn)
{
	FLOOR *nf = (FLOOR *) malloc (sizeof (FLOOR));
	
	nf->id = id;
	strcpy (nf->name, fn);
	
	elev.fl = node_add (elev.fl, NULL, nf);
}

int
elev_list_floors (void)
{
	NODE *cf = elev.fl;
	int cnt = 0;
	
	while (cf){
		FLOOR *fl = node_get_data (cf);
		
		log_print ("FLOOR: %d NAME: %s\n", fl->id, fl->name);
		cnt++;
		cf = node_get_next (cf);
	}
	
	return cnt;
}

void
elev_free (void)
{
	node_clear (elev.fl);
}

int
elev_init (char *dfn)
{
	int i;
	const int type = DAT_ID('T','Y','P','E');
	const int name = DAT_ID('N','A','M','E');
	DATAFILE *df = load_datafile (dfn);
	
	/* Count how many hotel maps in datafile */
	for (i = 0, elev.nf = 0; df[i].type != DAT_END; i++)
		if(!stricmp(get_datafile_property(df+i, type), "MAPD")){
			const char *mfn = get_datafile_property(df+i, name);
			if (strstr (mfn, "HOTEL")){
				int c = 0;
				while (!isdigit(mfn[c]) && mfn[c])
					c++;
				
				elev_push_floor (strtol (&mfn[c], NULL, 10), (char *)mfn);
				elev.nf++;
			}
		}
	
	unload_datafile (df);
	
	return 1;
}

