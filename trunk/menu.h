#ifndef _MENU_H_
#define _MENU_H_
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "support.h"
#include "font.h"
#include "sprite.h"
#include "particle.h"
#include "fps.h"

/* Menues */
enum _menu_states { MEN_START, MEN_PAUSE, MEN_OPTION, MEN_VOLUME, MEN_DIFF, K_CONFIG, MEN_PLAYERS };

typedef struct {
	SPRITE *opts[20][5];	//項目、アニメーション(残像)レイヤー
	SDL_Surface *opts_surface[20];
	SDL_Surface *bg;
	int active_item;
	int select_finish;
	int nopt;
	int alpha;
	int alphadir;
	int fadeout;
	int timeout;
} MENU;

typedef struct {
	int i0;		//x
	int i1;		//y
	int i2;		//スプライト用
	int i3;		//スプライト用
} MENU_DATA;

void menusystem_init();
void menu_init();
void menu_work();
void startmenu_init();
void startmenu_work();
void pausemenu_init();
void pausemenu_work();
void optionmenu_init();
void optionmenu_work();
void volumemenu_init();
void volumemenu_work();
void difficultymenu_init();
void difficultymenu_work();
void key_config_init();
void key_config_work();
void kp_search(int btn, int num);
void player_opt_init();
void player_opt_work();
void genericmenu_init(char *options[], MENU *m, int fadeout, int timeout);
void genericmenu_work(MENU *m);
#endif

