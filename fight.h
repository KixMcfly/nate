#ifndef __FIGHT_H
#define __FIGHT_H

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
	int money;
	int item;
	int na;
	int max_health;
	int cur_health;
	unsigned short imageid;
	ATTACK *att_l;
	
} ENEMY;

#endif /* __FIGHT_H */
