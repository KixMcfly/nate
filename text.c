#include "text.h"

static FONT *font_sis = NULL;
static BITMAP *border = NULL;

void
text_load_font_dat (char *dn, char *fn)
{
	DATAFILE *df = NULL;
	char *names[] = { fn, NULL };
	BITMAP *temp;
	
	if (!border){
		df = load_datafile_object (dn, "TXTBORDER_BMP");
		temp = (BITMAP *)df->dat;
		border = create_bitmap (temp->w, temp->h);
		blit (border, temp, 0, 0, 0, 0, temp->w, temp->h);
		
		unload_datafile_object (df);
	}
	
	if (font_sis)
		destroy_font (font_sis);
	
	font_sis = load_dat_font (dn, NULL, names);
}

void
text_destroy_font (void)
{
	destroy_bitmap (border);
	border = NULL;
	destroy_font (font_sis);
	font_sis = NULL;
}

void
text_print_center (BITMAP *b, char *t)
{
	
	textout_ex (b, font_sis, t, b->w >> 1, 5, -1, -1);
}
