#include "fight.h"

#define FIGHT_CHANCE_MAX 100

static int fight_chance = 0;

int
fight_check (void)
{
	int rn;
	
	rn = rand() % (FIGHT_CHANCE_MAX + 1 - 1);
	return (rn <= fight_chance);
}

void
fight_chance_dec (int da)
{
	fight_chance -= da;
	if (fight_chance < 0)
		fight_chance = 0;
}

void
fight_chance_inc (int da)
{
	fight_chance += da;
	if (fight_chance > FIGHT_CHANCE_MAX)
		fight_chance = FIGHT_CHANCE_MAX;
}
