#include "elev.h"

struct ELEV {
	
	int floors;
	int cur_floor;
	char **fl;
	
} static elev = {
	.floors = 0,
	.cur_floor = 0
};

int
elev_init (char *dfn)
{
	DATAFILE *df = load_datafile (dfn);
	
	
	
	unload_datafile (df);
}

