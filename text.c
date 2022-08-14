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
		blit (temp, border, 0, 0, 0, 0, temp->w, temp->h);
		
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
	int len, slen, tx, x; 
	
	if (!t)
		return;
		
	len = text_length (font_sis, t);
	slen = strlen (t);
	tx = b->w / 2 - len / 2;
	
	blit (border, b, 0, 0, tx-21, 1, 20, 20);
	for (x = 0; x < slen-1; x++)
		blit (border, b, 20, 0, (tx-1)+x*10, 1, 10, 20);
		
	blit (border, b, 0, 0, (tx-1)+x*10, 1, 20, 20);
		
	textout_ex (b, font_sis, t, tx, 5, -1, -1);
}
