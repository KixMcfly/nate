#include "sprite.h"

typedef struct {
	
	int flags;		/* UNUSED FOR NOW */
	int num_f;
	int offs_l;		/* UNUSED FOR NOW */
	int *wait_b;
	BITMAP **bl;
	
} KEYFRAME;

struct SPRITE {

	int num_kf;
	KEYFRAME *kfl;

};

SPRITE *
sprite_new (void)
{
	SPRITE *spr = (SPRITE *) malloc (sizeof (SPRITE));
	
	if (!spr)
		return NULL;

	spr->num_kf = 0;
	spr->kfl = NULL;
	
	return spr;
}

void
sprite_free (SPRITE *spr)
{
	int cb, ckf;
	if (spr){

		if (spr->kfl){
			
			for (ckf = 0; ckf < spr->num_kf; ckf++){
					
				for (cb = 0; cb < spr->kfl[ckf].num_f; cb++)
					destroy_bitmap (spr->kfl[ckf].bl[cb]);

				free (spr->kfl[ckf].bl);
				free (spr->kfl[ckf].wait_b);
				
				spr->kfl[ckf].bl = NULL;
			}

			free (spr->kfl);
			spr->kfl = NULL;
		}
		
		free (spr);
		spr = NULL;
	}
}

void
sprite_draw (SPRITE *s, BITMAP *dest, int kf, int b, int x, int y)
{
	draw_sprite (dest, s->kfl[kf].bl[b], x, y);
}

int
sprite_keyframe_dat_div (SPRITE *m, int xd, int yd, char *df, char *bmp)
{
	DATAFILE *spr = NULL;
	BITMAP *t_bmp = NULL;
	int cw, ch, x, y, ckf, cb;
	
	if (xd < 2 || yd < 2 || m->num_kf)
		return 0;
	
	spr = load_datafile_object (df, bmp);
	if (!spr)
		return -1;
		
	t_bmp = (BITMAP *)spr->dat;
	
	cw = t_bmp->w / xd;
	ch = t_bmp->h / yd;
	
	m->kfl = (KEYFRAME *) malloc (sizeof (KEYFRAME) * yd);
	if (m->kfl == NULL){
		unload_datafile_object (spr);
		return -2;
	}

	for (y = 0, ckf = 0; ckf < yd; y += ch, ckf++){
		
		/* Allocate space for frame and wait buffers */
		m->kfl[ckf].bl = (BITMAP **) malloc (sizeof (BITMAP *) * xd);
		m->kfl[ckf].wait_b = (int *) malloc (sizeof (int) * xd);

		m->kfl[ckf].num_f = xd;
		
		for (x = 0, cb = 0; cb < xd; x += cw, cb++){
			m->kfl[ckf].bl[cb] = create_bitmap (cw, ch);
			m->kfl[ckf].wait_b[cb] = 12;
			blit (t_bmp, m->kfl[ckf].bl[cb], x, y, 0, 0, cw, ch);
		}
	}

	m->num_kf = yd;

	unload_datafile_object (spr);

	return 1;
}

int
sprite_get_num_keyframes (SPRITE *spr)
{		
	return spr->num_kf;
}

int
sprite_keyframe_get_num_frames (SPRITE *spr, int kfn)
{		
	return spr->kfl[kfn].num_f;
}

int
sprite_keyframe_get_frame_rest (SPRITE *spr, int kfn, int cf)
{
	return spr->kfl[kfn].wait_b[cf];
}
