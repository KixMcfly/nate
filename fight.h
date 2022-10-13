#ifndef __FIGHT_H
#define __FIGHT_H
#include <allegro.h>
#include <stdlib.h>
#include "tools.h"

#define MAX_ENEMY_NAME 20

#define MAX_ATTACK_NAME 30
#define MAX_ATTACK_DESC 100

typedef struct {
	
	char name[MAX_ATTACK_NAME];
	char desc[MAX_ATTACK_DESC];
	short dam;
	unsigned char prob;

} ATTACK;

typedef struct {
	
	char name[MAX_ENEMY_NAME];
	int x;
	int y;
	int money;
	int item;
	int na;
	int max_health;
	int cur_health;
	unsigned short imageid;
	ATTACK *att_l;
	
} ENEMY;

int
fighting (void);

void
fight_set_ret_room (char *room, int x, int y);

char *
fight_get_ret_room (int *x, int *y);

void
fight_chance_reset (void);

int
fight_chance_inc (int da);

void
fight_draw_stuff (BITMAP *bf);

void
fight_free_stuff (void);


#endif /* __FIGHT_H */
