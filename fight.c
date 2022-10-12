#include "fight.h"

#define FIGHT_CHANCE_MAX 1000

static int fight_chance = 1;
static int fight = FALSE;
static char ret_room[100];
static int ret_x, ret_y;

int
fighting (void)
{
	return fight;
}

void
fight_set_ret_room (char *room, int x, int y)
{
	strcpy (ret_room, room);
	ret_x = x;
	ret_y = y;
}

char *
fight_get_ret_room (int *x, int *y)
{
	*x = ret_x;
	*y = ret_y;
	
	return ret_room;
}

void
fight_chance_reset (void)
{
	fight_chance = 0;
}

int
fight_chance_inc (int da)
{
	int rn;
	
	fight_chance += da;
	if (fight_chance > FIGHT_CHANCE_MAX)
		fight_chance = FIGHT_CHANCE_MAX;
		
	rn = rand() % (FIGHT_CHANCE_MAX + 1);
	if ( rn < fight_chance){
		fight = TRUE;
		
		fight_chance = 1;
		
	}
	
	log_print ("RAND NUM: %d FIGHT CHANCE: %d\n", rn, fight_chance);
	
		
	
	return fight;
}
