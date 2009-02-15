#ifndef _BG_H_
#define _BG_H_

#include <SDL/SDL.h>
#include "support.h"
#include "sprite.h"

enum _bg_type
{
	BG_BLACK=0,
	BG_CLOUDS,
	BG_STARS,
	BG_TILE,
	BG_ADD,
	BG_LAST
};

extern void tile_add(void/*int lev*/);

extern void bg_init(int bg_type/*,int lev*/);
extern void bg_work(void);
extern void bg_destroy(void);

//static void tile_work(void);
//static void tile_display(void);
//static void clouds_remove(void);
//static void tile_remove(void);
//static void clouds_mover(SPRITE *s);
//static void clouds_init(void/*int lev*/);
//static void tile_init(void/*int lev*/);
#endif
