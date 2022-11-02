#ifndef __OBJ_H
#define __OBJ_H

#define MAX_ENEMY_NAME 20
#define MAX_ATTACK_NAME 30
#define MAX_ATTACK_DESC 100

#define MAX_NAME_LEN 100
/* Object types */
enum {
	
	OBJ_NONE,
	OBJ_COMPUTER,
	OBJ_CHGROOM,
	OBJ_VENDING,
	OBJ_ITEMBOX,
	OBJ_STAT,
	OBJ_ELEV_BUTT,
	OBJ_ENEMY
};

typedef struct {
	int x;
	int y;
} GENERIC;

typedef struct {
	int x;
	int y;
	int cx;
	int cy;
	char name[MAX_NAME_LEN];
} CHGROOM;

typedef struct {
	int x;
	int y;
	int inv_list[20];
} VENDING;

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

#endif /* __OBJ_H */
