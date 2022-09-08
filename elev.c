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

void
elev_push_floor (unsigned long id, char *fn)
{
	FLOOR *nf = (FLOOR *) malloc (sizeof (FLOOR));
	
	nf->id = id;
	strcpy (nf->name, fn);
	
	node_add (elev.fl, NULL, nf);
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
	NODE *fh;
	const int type = DAT_ID('T','Y','P','E');
	const int name = DAT_ID('N','A','M','E');
	DATAFILE *df = load_datafile (dfn);
	
	fh = elev.fl;
	
	/* Count how many hotel maps */
	for (i = 0, elev.nf = 0; df[i].type != DAT_END; i++)
		if(!stricmp(get_datafile_property(df+i, type), "MAPD")){
			const char *mfn = get_datafile_property(df+i, name);
			if (!strstr ("HOTEL", mfn))
				elev.nf++;
		}
	
	log_print ("Number of hotel floors %d\n", elev.nf);
	
	unload_datafile (df);
	
	return 1;
}

