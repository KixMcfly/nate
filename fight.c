#include "fight.h"

#define MAX_ENEMY_NAME 20

#define MAX_ATTACK_NAME 30
#define MAX_ATTACK_DESC 100


typedef struct {
	
	char name[MAX_ATTACK_NAME];
	char desc[MAX_ATTACK_DESC];
	int dam;
	int prob;
	
} ATTACK;

struct ENEMY {
	
	char name[MAX_ENEMY_NAME];
	int money;
	const int max_health;
	int cur_health;
	ATTACK *att_l;
	
};

static int num_enemy = 0;
static ENEMY *enemy_list = NULL;

int
enemy_list_alloc (int ne)
{
	if (enemy_list)
		return 0;
		
	enemy_list = (ENEMY *) malloc (ne * sizeof (ENEMY));
	
	return 1;
}

int
enemy_push (char *name, int dam, int prob, int num_att)
{
	if (!enemy_list)
		
	
	if (strlen (name) > MAX_ENEMY_NAME)
		return 0;
	
	return 1;
}

