#ifndef __NODE_H
#define __NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

typedef struct {
	int type;
	void *data;
	struct NNODE *next;
} NNODE;

NNODE *
node_add (NNODE *head, int type, void *data);

int
node_get_type (NNODE *head);

void *
node_get_data (NNODE *n);

int
node_delete (NNODE *head, int num);

NNODE *
node_get_next (NNODE *n);

NNODE *
node_clear (NNODE *head);

#endif /* __NNODE_H */
