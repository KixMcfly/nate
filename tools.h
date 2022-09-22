#ifndef __TOOLS_H
#define __TOOLS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include "map.h"

void
log_print (char *log_o, ...);

//void
//log_print_map (MAP *m);

char *
strtmp (char *str);

#endif /* __TOOLS_H */
