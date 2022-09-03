#include "temp.h"

#define LOW_MAX 		50
#define HI_MAX			90

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

struct THERMOSTAT {
	int set_temp;
	int cur_temp;
	int mode;
	int season;
	int rate;
	int rate_set;
} thermostat = { .set_temp = 72, .cur_temp = 88, .mode = COOL, .season = SUMMER, .rate = 0, .rate_set = 40};
		
void
temp_process (void)
{
	if (--thermostat.rate == 0){
		thermostat.rate = thermostat.rate_set;
		
		thermostat.cur_temp += thermostat.season;
		
		if (thermostat.cur_temp != thermostat.set_temp)
			thermostat.cur_temp += thermostat.mode;
	}
}
