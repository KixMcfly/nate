#ifndef __TEXT_H
#define __TEXT_H

#include <allegro.h>

void
text_load_font_dat (char *dn, char *fn);

void
text_destroy_font (void);

void
text_print_center (BITMAP *b, char *t);

#endif
