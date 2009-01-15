#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include <SDL/SDL.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include <math.h>

#define clip_xmin(pnt) pnt->clip_rect.x
#define clip_xmax(pnt) pnt->clip_rect.x + pnt->clip_rect.w-1
#define clip_ymin(pnt) pnt->clip_rect.y
#define clip_ymax(pnt) pnt->clip_rect.y + pnt->clip_rect.h-1
#define sign(x) ((x) > 0 ? 1 : ((x) == 0 ? 0 : (-1) ))
#define CHECKPOINT error(ERR_DEBUG,"Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)

#define WIDTH 480 
#define HEIGHT 272 
#define WIDTH2 380

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define degtorad(x) ((M_PI/180.0)*(x))
#define radtodeg(x) ( (int)((x)/(M_PI/180.0)+360)%360 )

enum _errlevel { ERR_DEBUG, ERR_INFO, ERR_WARN, ERR_FATAL };

enum _state {
	ST_START_INTRO,
	ST_INTRO,
	ST_MENU,
	ST_GAME_DEMO,
	ST_GAME_PLAY,
	ST_GAME_OVER,
	ST_SHOW_HCLIST,
	ST_ENTRY_HCLIST,
	ST_GAME_QUIT
};
enum _keynum_{		//キーコンフィグ用
	KEY_NONE,
	KEY_SHOT,
	KEY_BOMB,
	KEY_SLOW,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_PAUSE,
	KEY_CANCEL,
	KEY_SC_SHOT
};

enum _diff { DIFF_EASY=0, DIFF_MEDIUM=1, DIFF_HARD=2 };

typedef struct _imglist {
	char name[256];
	int refcount;
	SDL_Surface *img;
	struct _imglist *next;
} IMGLIST;

struct _gamestate {
	int mainstate;		/* Aktueller Status */
	int substate;		/* Aktueller Status Subsystem */
	int newstate;		/* 1: Statuswechsel */
};
typedef struct _gamestate GAMESTATE;

typedef struct {
	int u;	//上
	int d;	//下
	int l;	//左
	int r;	//右
	int ba;	//×
	int ma;	//○
	int sa;	//△
	int si;	//□
	int rt;	//R
	int lt;	//L
	int sl;	//SELECT
	int st;	//START
} KEYCONFIG;


#include "font.h"
#include "menu.h"
#include "hiscore.h"
#include "fps.h"

char moddir[20];

void game_init(int argc, char *argv[]);
void toggle_fullscreen();
void error(int errorlevel, char *msg, ...);
SDL_Surface *loadbmp(char *filename);
SDL_Surface *loadbmp2(char *filename);
void unloadbmp_by_surface(SDL_Surface *s);
void unloadbmp_by_name(char *name);
void imglist_add(SDL_Surface *s, char *name);
SDL_Surface *imglist_search(char *name);
void imglist_garbagecollect();
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 color);
//void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2);
//void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1);
void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
void blit_calpha(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
void keyboard_clear();
void keyboard_poll();
int keyboard_keypressed();
void newstate(int m, int s, int n);
void *mmalloc(size_t size);
void preload_gfx();
void load_ing();
void display_vidinfo();
int ini_load();		//***090110
void ini_save();	//***090115

#endif
