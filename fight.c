#include "fight.h"

struct ENEMY {
	
	char *name;
	int money;
	const int health;
	
}

pot_belly_tard	= { .name = "Pot Belly Tard",	.money = 15,	.health = 2000},
beach_bald		= { .name = "Beach Bald",		.money = 1,		.health = 1};

typedef struct {
	
	struct ENEMY *enemy;
	int cur_health;
	int x;
	int y;

} ENEMY_GRP;

ENEMY_GRP hotel_1[2] = {
	
	{&pot_belly_tard, 0, 0},
	{&beach_bald, 4, 4}
};
