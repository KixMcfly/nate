#include "hroom.h"

/*
 *	hroom.h - Hotel Room Operations 
 * 
 * 
 *  Checks people into hotel rooms and makes them
 *  whine for stuff.
 * 
 */

typedef struct {
	int vac;
	int stay_day_max;
	int stay_day;
	int happy;
	int hungry;
	int thirsty;
	int comfy;
	int clean;
	int floor_num;
	int room_num;
} HROOM;

/* Number of hotel rooms are decided at load time */
static int num_hrooms = 0;
static HROOM *hroom_list = NULL;

int
init_hroom_stats (char *dfn)
{
	const int type = DAT_ID('T','Y','P','E');
	const int name = DAT_ID('N','A','M','E');
	int i = 0;
	DATAFILE *df = load_datafile (dfn);

	/* Exit hotel room init if datafile load fail */
	if (!df)
		return FALSE;

	/* Find hotel rooms in datafile and create status info for each */
	for (i = 0, num_hrooms = 0; df[i].type != DAT_END; i++)
		if(!stricmp(get_datafile_property(df+i, type), "MAPD")){
			const char *mfn = get_datafile_property (df+i, name);
			if (strstr (mfn, "HOTEL") && strstr (mfn, "-")){
				//int c = 0;
				//while (!isdigit(mfn[c]) && mfn[c])
					//c++;
				
				//elev_push_floor (strtol (&mfn[c], NULL, 10), (char *)mfn);
				printf ("HOTEL ROOM: %s\n", mfn);
			}
		}

	unload_datafile (df);
	
	return TRUE;
}
