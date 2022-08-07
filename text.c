#include "text.h"

static FONT *font_shut = NULL;

void
text_load_font_dat (char *dn, char *fn)
{
	char *names[] = { fn, NULL };
	
	if (font_shut)
		destroy_font (font_shut);
	
	font_shut = load_dat_font (dn, NULL, names);
}

void
text_destroy_font (void)
{
	destroy_font (font_shut);
}

void
text_print_center (BITMAP *b, char *t)
{
	textout_centre_ex (b, font_shut, t, b->w >> 1, 5, -1, -1);
}
