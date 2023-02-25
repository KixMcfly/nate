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
static NNODE *hroom_list = NULL;

void
init_hroom_stats (char *dfn)
{
	const int type = DAT_ID('T','Y','P','E');
	const int name = DAT_ID('N','A','M','E');
	int i = 0;
	DATAFILE *df = load_datafile (dfn);

	/* Exit hotel room init if datafile load fail */
	if (!df)
		return;

	/* Find hotel rooms in datafile and create status info for each */
	for (i = 0, num_hrooms = 0; df[i].type != DAT_END; i++)
		if(!stricmp(get_datafile_property(df+i, type), "MAPD")){
			const char *mfn = get_datafile_property (df+i, name);
			if (strstr (mfn, "HOTEL") && strstr (mfn, "-")){
				//int c = 0;
				//while (!isdigit(mfn[c]) && mfn[c])
					//c++;
				
				HROOM *hroom = (HROOM *) malloc (sizeof (HROOM));
				hroom->vac = TRUE;
				hroom->stay_day_max = 0;
				hroom->stay_day = 0;
				hroom->happy = 0;
				hroom->hungry = 0;
				hroom->thirsty = 0;
				hroom->comfy = 0;
				hroom->clean = 100;
				hroom->floor_num = 0;
				hroom->room_num = 0;
				
				hroom_list = node_add (hroom_list, 0, hroom);
				
			}
		}

	unload_datafile (df);

}

void
hroom_clear (void)
{
	hroom_list = node_clear (hroom_list);
}
