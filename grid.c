#include "grid.h"

#define GRID_MAX_W				128
#define GRID_MAX_H				128

static int grid_w = 0;
static int grid_h = 0;

typedef struct GRID GRID;
struct GRID{
	
	int *x;
	int *y;
	int *dx;
	int *dy;
	GRID *next;
	
};

GRID *head = 0;

int
grid_set_w (int w)
{
	if (w < GRID_MAX_W)
		grid_w = w;
	else
		return 0;
		
	return 1;
}

int
grid_set_h (int h)
{
	if (h < GRID_MAX_H)
		grid_h = h;
	else
		return 0;
		
	return 1;
}

void
grid_snap_up (int *x, int *y, int *dy)
{
	if (*y % grid_h == 0 && *x % grid_w == 0){
		*dy = -1;
		*y -= 1;
	}
}

void
grid_snap_down (int *x, int *y, int *dy)
{
	if (*y % grid_h == 0 && *x % grid_w == 0){
		*dy = 1;
		*y += 1;
	}
}

void
grid_snap_left (int *x, int *y, int *dx)
{
	if (*x % grid_w == 0 && *y % grid_h == 0){
		*dx = -1;
		*x -= 1;
	}
}

void
grid_snap_right (int *x, int *y, int *dx)
{
	if (*x % grid_w == 0 && *y % grid_h == 0){
		*dx = 1;
		*x += 1;
	}
}

int
grid_snap_queue_add (int *x, int *y, int *dx, int *dy)
{
	GRID *gt = (GRID *) malloc (sizeof (GRID));
	
	if (!gt)
		return 0;
	
	gt->next = head;
	gt->x = x;
	gt->y = y;
	gt->dx = dx;
	gt->dy = dy;
	
	head = gt;
	
	return 1;
}

int
grid_snap_queue_free (void)
{
	GRID *cur = head, *prev = 0;
	int n_del = 0;
	

	while (cur){
		prev = cur;
		cur = cur->next;
		free (prev);
		n_del++;
	}
	
	head = 0;
	
	return n_del;
}

void
grid_snap_queue_proc (void)
{
	GRID *cur = head;
	
	while (cur){
		
		/* Snap X */
		
		if (*(cur->x) % grid_w > 0){
			if (*(cur->dx) == -1){
				*(cur->x) -= 1;
				
			}else if (*(cur->dx) == 1){
				*(cur->x) += 1;
			}
		}
			
		/* Snap Y */
		if (*(cur->y) % grid_h > 0){
			if (*(cur->dy) == -1)
				*(cur->y) -= 1;
			else if (*(cur->dy) == 1)
				*(cur->y) += 1;
		}
		
		cur = cur->next;
	}
}
