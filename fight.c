#include "fight.h"

typedef struct {
	
	char name[30];
	char desc[100];
	int dam;
	int prob;
	
} ATTACK;

struct ENEMY {
	
	char name[20];
	int money;
	const int health;
	ATTACK *att_l;
	
};

