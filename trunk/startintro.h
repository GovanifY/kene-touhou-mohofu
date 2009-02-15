#ifndef _STARTINTRO_H_
#define _STARTINTRO_H_
#include <SDL/SDL.h>

#include "support.h"
//#include "particle.h"
#include "font.h"

enum _startintro_states
{
	STIN_INIT,
	STIN_FADEIN_BB,
	STIN_FADEIN_PRESENTS,
	STIN_FADEIN_KETM,
	STIN_SLEEP,
	STIN_QUIT
};

extern void startintro_init(void);
extern void startintro_work(void);
//static void startintro_centerimg(void);
#endif
