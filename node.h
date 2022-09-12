#ifndef __NODE_H
#define __NODE_H

#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

typedef struct NODE NODE;

NODE *
node_add (NODE *head, int type, void *data);

int
node_get_type (NODE *head);

void *
node_get_data (NODE *n);

int
node_delete (NODE *head, int num);

NODE *
node_get_next (NODE *n);

NODE *
node_clear (NODE *head);

#endif /* __NODE_H */
