#ifndef _THEGAME_H_
#define _THEGAME_H_

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sprite.h"
//#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "menu.h"
#include "score.h"
#include "bg.h"
#include "loadlv.h"
#include "scenario.h"
#include "soundmanager.h"

extern void thegame_init(void);
extern void thegame_work(void);
//static void thegame_level(void);
#endif
