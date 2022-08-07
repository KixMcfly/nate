/**
 * @file tileset.h
 * @author KixMcfly
 * @date 5/12/2022
 * @brief Functions to load tilesets
 * 
 * Load an indexed version of a tileset
 */

#ifndef __TILESET_H
#define __TILESET_H
#include <allegro.h>
#include <stdio.h>

/**
 * @brief @c TILESET object
 * 
 * When loaded with data, a @c TILESET object should
 * be used in concert with a MAP object
 * 
 */
typedef struct {

	int nt;					/**< Number of Tiles */
	BITMAP **tiles;			/**< Array of tile data in tileset */
	
} TILESET;

/**
 * @brief loads a @c BITMAP from a @c DATAFILE object.
 * 
 * @param *ts @c TILESET to populate
 * @param *dat_fn abs path of @c DATAFILE object to load @c BITMAP from.
 * @param *dat_id name of @c BITMAP in passed @c DATAFILE object
 * @param tw Width of tiles to be parsed from @c BITMAP
 * @param th Height of tiles to be parsed from @c BITMAP
 * @return 1 if successfuly
 * 
 * @note If accumulated tile width or tile height doesn't add up with
 * @c BITMAP dimentions, any left over pixels will be ignored.
 */
int
load_tileset (TILESET *ts, char *dat_fn, char *dat_id, int tw, int th);

#endif
