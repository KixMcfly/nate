#ifndef __SPRITE_H
#define __SPRITE_H

#include <allegro.h>
#include <stdio.h>

typedef struct SPRITE SPRITE;

SPRITE *
sprite_new (void);

void
sprite_free (SPRITE *spr);

void
sprite_draw (SPRITE *s, BITMAP *dest, int kf, int b, int x, int y);

int
sprite_keyframe_dat_div (SPRITE *m, int xd, int yd, char *df, char *bmp);

int
sprite_get_num_keyframes (SPRITE *spr);

int
sprite_keyframe_get_num_frames (SPRITE *spr, int kfn);

int
sprite_keyframe_get_frame_rest (SPRITE *spr, int kfn, int cf);

#endif /* __SPRITE_H */
