#ifndef __ELEV_H
#define __ELEV_H

#include <allegro.h>
#include "node.h"
#include "tools.h"

void
elev_push_floor (unsigned long id, char *fn);

void
elev_free (void);

int
elev_init (char *dfn);

#endif
