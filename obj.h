#ifndef __OBJ_H
#define __OBJ_H

#define MAX_NAME_LEN 100
/* Object types */
enum {
	
	OBJ_NONE,
	OBJ_COMPUTER,
	OBJ_CHGROOM
	
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

#endif /* __OBJ_H */
