#ifndef _INTRO_H_
#define _INTRO_H_
#include <SDL/SDL.h>

#include "sprite.h"
#include "menu.h"
#include "zenkaku.h"

enum _intro_states
{
	INTRO_INIT,
	INTRO_FADEIN_MOON,
	INTRO_FADEIN_KETM,
	INTRO_SLEEP,
	INTRO_FADEOUT,
	INTRO_QUIT
};

extern void intro_init(void);
extern void intro_work(void);
//static void intro_fadein_moon(void);
//static void intro_fadein_ketm(void);
//static void intro_sleep(void);
#endif
