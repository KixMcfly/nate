#ifndef __ELEV_H
#define __ELEV_H

#include <allegro.h>
#include <ctype.h>
#include "node.h"
#include "tools.h"

int
elev_list_floors (void);

void
elev_free (void);

int
elev_init (char *dfn);

#endif
