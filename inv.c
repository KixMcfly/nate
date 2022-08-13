#include "inv.h"

#define MAX_ITEMBOX 		100
#define MAX_INV				8

typedef struct {
	
	int id;
	int num;
	
} INVITEM;

INVITEM itembox[MAX_ITEMBOX];
INVITEM inv[MAX_INV];

char *inv_list[INV_NUM] = {
	
	"NONE",
	/* VALUE */
	"MONEY",
	
	/* FOOD */
	"NUGGETS",
	"DR. DOUCHE"
	
};

void
inv_init (void)
{
	int i;	
	for (i = 0; i < MAX_ITEMBOX; i++){
		itembox[i].id = INV_NONE;
		itembox[i].num = 0;
	}
	
	for (i = 0; i < MAX_ITEMBOX; i++){
		inv[i].id = INV_NONE;
		inv[i].num = 0;
	}
}

int
inv_add (int id, int amt)
{
	int i;
	for (i = 0; i < MAX_INV; i++){
		
		if (inv[i].id == id){
			inv[i].num += amt;
			return 1;
		}
	}
	
	for (i = 0; i < MAX_INV; i++){
		
		if (inv[i].id == INV_NONE){
			inv[i].id = id;
			inv[i].num = amt;
			return 1;
		}
	}
	
	return 0;
}

void
inv_log (void)
{
	int i;
	for (i = 0; i < MAX_INV; i++){
		
		log_print ("ITEM: %s AMOUNT: %d\n", inv_list[ inv[i].id ], inv[i].num);
	}
}

