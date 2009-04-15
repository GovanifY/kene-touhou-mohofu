#ifndef _MENU_H_
#define _MENU_H_
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "support.h"
#include "font.h"
#include "sprite.h"
//#include "particle.h"
#include "fps.h"

/* Menues */
enum _menu_states
{
	MENU_START=0,	/*intro.c*/
	MENU_PAUSE, 	/*thegame.c*/
	MENU_OPTION,
	//#if (1==DEBUG_MODE)
	MENU_STAGE_SELECT,
	//#endif
	MENU_VOLUME,
	MENU_DIFF
};

typedef struct
{
	SPRITE *opts[20][5];	//項目、アニメーション(残像)レイヤー
	SDL_Surface *opts_surface[20];		//画像化した各項目のアドレス???
//	SDL_Surface *bg;
	int active_item;
	int select_finish;
	int nopt;			//オプションの項目数を数えてる???
	int alpha;
	int alphadir;		//各項目のα値の状態。フェードin or out or done???
	int fadeout;
	int timeout;
} MENU;

//static void genericmenu_work(MENU *m);
//static void genericmenu_init(char *options[], MENU *m, int fadeout, int timeout);

//static void startmenu_init(void);
//static void startmenu_work(void);
//static void pausemenu_init(void);
//static void pausemenu_work(void);
//static void optionmenu_init(void);
//static void optionmenu_work(void);
//static void volumemenu_init(void);
//static void volumemenu_work(void);
//static void difficultymenu_init(void);
//static void difficultymenu_work(void);
extern void key_config_init(void);
//static void kp_search(int btn, int num);
extern void key_config_work(void);

extern void player_opt_init(void);
//static void player_opt_img(SDL_Surface *src, double scale, int l_or_r);
extern void player_opt_work(void);

extern void menu_init(void);
extern void menu_work(void);

extern void menusystem_init(void);
#endif

