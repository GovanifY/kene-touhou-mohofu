#ifndef _BG_H_
#define _BG_H_

#include <SDL/SDL.h>
#include "support.h"
#include "sprite.h"

enum _bg_type {
	BG_BLACK=0,
	BG_CLOUDS,
	BG_STARS,
	BG_TILE,
	BG_LAST
};

typedef struct {
	double speed_base;
	double speed_rand;
} CLOUDS_DATA;


void bg_init(int bg_type,int lev);
void bg_work();
void bg_destroy();
void clouds_init(int lev);
void clouds_remove();
void clouds_mover(SPRITE *s);
void tile_init(int lev);
void tile_work();
void tile_display();
void tile_remove();

#endif
