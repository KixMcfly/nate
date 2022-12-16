#include "temp.h"

#define COOL			-1
#define OFF				0
#define WARM			1
#define WINTER			-1
#define SUMMER			1

#define LOW_MAX 		50
#define HI_MAX			90

#define DEF_STAT_TEMP	72
#define DEF_CUR_TEMP	55
#define DEF_SET_RATE	1000
#define DEF_SEASON		SUMMER
#define DEF_STAT_MODE	COOL
#define DEF_OUT_TEMP	88

#define X				0
#define Y				1

struct THERMOSTAT {
	int set_temp;
	int cur_temp;
	int mode;
	int season;
	int rate;
	int rate_set;
	int vis;
	int pos;
	int out_temp;
} thermostat = { 	.set_temp = DEF_STAT_TEMP,
					.cur_temp = DEF_CUR_TEMP,
					.mode = DEF_STAT_MODE,
					.season = DEF_SEASON,
					.rate = 1,
					.rate_set = DEF_SET_RATE,
					.vis = FALSE,
					.pos = 0,
					.out_temp = DEF_OUT_TEMP
				};

static int tp[4][2] = { {197, 78},
						{197, 91},
						{197, 105},
						{197, 115}
					  };

static DATAFILE *temp_dat = NULL;
static DATAFILE *temp_ctl_dat = NULL;
static BITMAP *temp_ctl_sel = NULL;
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

static void
temp_set_up (void)
{
	if (thermostat.set_temp < HI_MAX && !t_rest){
		thermostat.set_temp++;
		t_rest = 20;
	}
}

static void
temp_set_down (void)
{
	if (thermostat.set_temp > LOW_MAX && !t_rest){
		thermostat.set_temp--;
		t_rest = 20;
	}
}

static void
temp_mode_toggle (void)
{
	if (!t_rest && thermostat.vis){
		t_rest = 30;
		if (thermostat.mode == COOL)
			thermostat.mode = OFF;
		else if (thermostat.mode == OFF)
			thermostat.mode = WARM;
		else if (thermostat.mode == WARM)
			thermostat.mode = COOL;
	}
}

void
temp_pos_up (void)
{
	if (thermostat.pos && !t_rest && thermostat.vis){
		thermostat.pos--;
		t_rest = 20;
	}
}

void
temp_pos_down (void)
{
	if (thermostat.pos < 3 && !t_rest && thermostat.vis){
		thermostat.pos++;
		t_rest = 20;
	}
}

int
temp_global_process (void)
{
	if (--thermostat.rate == 0){
		thermostat.rate = thermostat.rate_set;

		switch (thermostat.mode){
			case COOL:
				if (thermostat.cur_temp > thermostat.set_temp)
					thermostat.cur_temp--;
				break;
			case WARM:
				if (thermostat.cur_temp < thermostat.set_temp)
					thermostat.cur_temp++;	
				break;
			case OFF:
				if (thermostat.season == SUMMER &&
					thermostat.cur_temp < thermostat.out_temp)
						thermostat.cur_temp++;
				else if (thermostat.season == WINTER &&
					thermostat.cur_temp > thermostat.out_temp)
						thermostat.cur_temp--;
				break;
		}
	}
	
	return thermostat.cur_temp;
}

void
temp_draw_backbuff (BITMAP *bf, char *dfn)
{
	
	static BITMAP *temp_bmp, *temp_ctl_bmp;
	char *names[] = {"THER_FNT", NULL};
	int x, y;
	
	if (thermostat.vis){

		if (!temp_dat){
			temp_dat = load_datafile_object (dfn, "STAT_BMP");
			temp_bmp = temp_dat->dat;
		}
		
		if (!temp_ctl_dat){
			temp_ctl_dat = load_datafile_object (dfn, "TEMP_CTL_BMP");
			temp_ctl_bmp = temp_ctl_dat->dat;
			temp_ctl_sel = create_bitmap (temp_ctl_bmp->w, temp_ctl_bmp->h);
			blit (temp_ctl_bmp, temp_ctl_sel, 0, 0, 0, 0,
				temp_ctl_sel->w, temp_ctl_sel->h);
				
			/* Replace color 255 with color 3 for button sel */
			for (y = 0; y < temp_ctl_sel->h; y++){
				for (x = 0; x < temp_ctl_sel->w; x++){
					int p = _getpixel (temp_ctl_sel, x, y);
					if (!(p ^ 0xFF))
						_putpixel (temp_ctl_sel, x, y, 3);
				}
			}
			
			clear (bf);
		}
		
		if (!temp_fnt)
			temp_fnt = load_dat_font (dfn, NULL, names);
		
		masked_blit (temp_bmp, bf, 0, 0, 0, 0, temp_bmp->w, temp_bmp->h);
		
		/* Temp up button */
		if (thermostat.pos == 0)
			
			if (key[KEY_LCONTROL]){
				masked_blit (temp_ctl_sel, bf, 0, 0, tp[0][X]+1, tp[0][Y]+1, 28, 12);
				temp_set_up ();
			}else{
				masked_blit (temp_ctl_sel, bf, 0, 0, tp[0][X], tp[0][Y], 28, 12);
			}
		else{
			masked_blit (temp_ctl_bmp, bf, 0, 0, tp[0][X], tp[0][Y], 28, 12);
		}
		
		if (thermostat.pos == 1)	
			if (key[KEY_LCONTROL]){
				masked_blit (temp_ctl_sel, bf, 0, 12, tp[1][X]+1, tp[1][Y]+1, 28, 12);
				temp_set_down ();
			}else{
				masked_blit (temp_ctl_sel, bf, 0, 12, tp[1][X], tp[1][Y], 28, 12);
			}
		else{
			masked_blit (temp_ctl_bmp, bf, 0, 12, tp[1][X], tp[1][Y], 28, 12);
		}
		
		if (thermostat.pos == 2){
			
			if (key[KEY_LCONTROL]){
				masked_blit (temp_ctl_sel, bf, 0, 24, tp[2][X]+1, tp[2][Y]+1, 28, 8);
				temp_mode_toggle ();
			}else{
				masked_blit (temp_ctl_sel, bf, 0, 24, tp[2][X], tp[2][Y], 28, 8);
			}
		}else{
			masked_blit (temp_ctl_bmp, bf, 0, 24, tp[2][X], tp[2][Y], 28, 8);
		}
			
		if (thermostat.pos == 3)
			masked_blit (temp_ctl_sel, bf, 0, 24, tp[3][X], tp[3][Y], 28, 8);
		else
			masked_blit (temp_ctl_bmp, bf, 0, 24, tp[3][X], tp[3][Y], 28, 8);
		
		if (thermostat.mode == COOL)
			textprintf_right_ex (bf, font, 190, 106, 147, -1, "%s", "COL");
		else if (thermostat.mode == WARM)
			textprintf_right_ex (bf, font, 190, 106, 147, -1, "%s", "WRM");
		else
			textprintf_right_ex (bf, font, 190, 106, 147, -1, "%s", "OFF");
		
		textprintf_ex (bf, temp_fnt, 112, 81, 147, -1, "%d", thermostat.cur_temp);
		textprintf_ex (bf, font, 172, 81, 147, -1, "%d", thermostat.set_temp);
		
		if (t_rest > 0)
			t_rest--;
		
	}
}

void
temp_uninit (void)
{
	if (!t_rest){
		unload_datafile_object (temp_dat);
		unload_datafile_object (temp_ctl_dat);
		destroy_bitmap (temp_ctl_sel);
		destroy_font (temp_fnt);
		temp_ctl_sel = NULL;
		temp_dat = NULL;
		temp_ctl_dat = NULL;
		temp_fnt = NULL;
		
		thermostat.vis = FALSE;
	}
}
