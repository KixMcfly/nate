#include "temp.h"

/* Thermostat settings */
enum {
	COOL = -1,
	WARM = 1
};

/* Seasons */
enum {
	WINTER = -1,
	SUMMER = 1
};

#define LOW_MAX 		50
#define HI_MAX			90

#define DEF_STAT_TEMP	72
#define DEF_CUR_TEMP	88
#define DEF_RATE		40
#define DEF_SEASON		WINTER
#define DEF_STAT_MODE	WARM

struct THERMOSTAT {
	int set_temp;
	int cur_temp;
	int mode;
	int season;
	int rate;
	int rate_set;
	int vis;
	int pos;
} thermostat = { 	.set_temp = DEF_STAT_TEMP,
					.cur_temp = DEF_CUR_TEMP,
					.mode = DEF_STAT_MODE,
					.season = DEF_SEASON,
					.rate = 0,
					.rate_set = DEF_RATE,
					.vis = FALSE,
					.pos = 0
				};

static DATAFILE *temp_dat = NULL;
static DATAFILE *temp_ctl_dat = NULL;
static FONT *temp_fnt = NULL;
static int t_rest = 0;

int
temp_vis (void)
{
	return thermostat.vis;
}

void
temp_set_vis (void)
{
	thermostat.vis = TRUE;
	t_rest = 40;
}

void
temp_process (BITMAP *bf, char *dfn)
{
	BITMAP *temp_bmp, *temp_ctl_bmp;
	char *names[] = {"THER_FNT", NULL};
	
	if (!temp_dat)
		temp_dat = load_datafile_object (dfn, "STAT_BMP");
		
	if (!temp_ctl_dat){
		temp_ctl_dat = load_datafile_object (dfn, "TEMP_CTL_BMP");
		clear (bf);
	}
	
	if (!temp_fnt)
		temp_fnt = load_dat_font (dfn, NULL, names);
	
	if (--thermostat.rate == 0){
		thermostat.rate = thermostat.rate_set;
		thermostat.cur_temp += thermostat.season;
		
		if (thermostat.cur_temp != thermostat.set_temp)
			thermostat.cur_temp += thermostat.mode;
	}
	
	temp_bmp = temp_dat->dat;
	temp_ctl_bmp = temp_ctl_dat->dat;
	masked_blit (temp_bmp, bf, 0, 0, 0, 0, temp_bmp->w, temp_bmp->h);
	
	if (t_rest > 0)
		t_rest--;
}

void
temp_uninit (void)
{
	if (!t_rest){
		unload_datafile_object (temp_dat);
		unload_datafile_object (temp_ctl_dat);
		destroy_font (temp_fnt);
		temp_dat = NULL;
		temp_ctl_dat = NULL;
		temp_fnt = NULL;
		
		thermostat.vis = FALSE;
	}
}
