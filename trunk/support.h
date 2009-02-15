#ifndef _SUPPORT_H_
#define _SUPPORT_H_


#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <pspdebug.h>

#include <unistd.h>
#include <stdarg.h>
//#include <math.h>
#include <dirent.h>
#include <ctype.h>

#ifdef ENABLE_PSP
	//# /* ƒJƒXƒ^ƒ€ƒ‰ƒCƒuƒ‰ƒŠ‚ğg‚¤ */
	#include "SDL.h"
#else
	//# /* •W€ƒ‰ƒCƒuƒ‰ƒŠ‚ğg‚¤ */
	#include <SDL/SDL.h>
#endif





//#include <SDL/SDL.h>
//#include <pspkernel.h>
//#include <pspdebug.h>
//#include <pspctrl.h>
//#include <stdlib.h>
//#include <string.h>
//#include "stdarg.h"
#include <math.h>

#define clip_xmin(pnt) pnt->clip_rect.x
#define clip_xmax(pnt) pnt->clip_rect.x + pnt->clip_rect.w-1
#define clip_ymin(pnt) pnt->clip_rect.y
#define clip_ymax(pnt) pnt->clip_rect.y + pnt->clip_rect.h-1
#define sign(x) ((x) > 0 ? 1 : ((x) == 0 ? 0 : (-1) ))
#define CHECKPOINT error(ERR_DEBUG,"Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)

#define WIDTH  480
#define HEIGHT 272
#define WIDTH2 380

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

#define degtorad(x) (((M_PI*2)/360.0)*(x))		/* 2ƒÎ € 360 * X */
#define radtodeg(x) ( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* ‚Pü‚ª360“x‚Ì’PˆÊŒn(deg360)‚ğ‚Pü‚ª‚QƒÎ‚Ì’PˆÊŒn(radian)‚Ö•ÏŠ·B‹y‚Ñ‹t•ÏŠ·B */
#define deg360_2rad(x) (((M_PI*2)/(360.0))*(x))
#define rad2deg360(x) ( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* ‚Pü‚ª512“x‚Ì’PˆÊŒn(deg512)‚ğ‚Pü‚ª‚QƒÎ‚Ì’PˆÊŒn(radian)‚Ö•ÏŠ·B‹y‚Ñ‹t•ÏŠ·B */
#define deg512_2rad(x) (((M_PI*2)/(512.0))*(x))
//#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))+512)%512 )
//#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))+512)&(512-1) )
#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))/*+512*/)&(512-1) )

/* ‚Pü‚ª65536“x‚Ì’PˆÊŒn(deg65536)‚ğ‚Pü‚ª‚QƒÎ‚Ì’PˆÊŒn(radian)‚Ö•ÏŠ·B‹y‚Ñ‹t•ÏŠ·B */
#define deg65536_2rad(x) (((M_PI*2)/(65536.0))*(x))
//#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))+65536)%65536 )
//#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))+65536)&(65536-1) )
#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))/*+65536*/)&(65536-1) )

/* ‚Pü‚ª360“x‚Ì’PˆÊŒn(deg360)‚ğ‚Pü‚ª512“x‚Ì’PˆÊŒn(deg512)‚Ö•ÏŠ·B‹y‚Ñ‹t•ÏŠ·B */
#define deg_360_to_512(x) ((int)((x)*(512.0/360.0)))
#define deg_512_to_360(x) ((int)((x)*(360.0/512.0)))

/* ”š‚ğƒXƒRƒA‚É•ÏŠ·B‹y‚Ñ‹t•ÏŠ·B */
#define score(x)   ((int)(((int)x)/10))
#define score_r(x) ((int)(((int)x)*10))

enum _errlevel { ERR_DEBUG, ERR_INFO, ERR_WARN, ERR_FATAL };

enum _state
{
	ST_START_INTRO,
	ST_INTRO,
	ST_MENU,
	ST_GAME_DEMO,
	ST_GAME_PLAY,
	ST_GAME_OVER,
	ST_SHOW_HCLIST,
	ST_ENTRY_HCLIST,
	ST_KEY_CONFIG,
	ST_PLAYER_SELECT,
	ST_GAME_QUIT
};
enum _keynum_		//ƒL[ƒRƒ“ƒtƒBƒO—p
{
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

typedef struct _imglist
{
	char name[256];
	int refcount;
	SDL_Surface *img;
	struct _imglist *next;
} IMGLIST;

struct _gamestate
{
	int mainstate;		/* Aktueller Status */
	int substate;		/* Aktueller Status Subsystem */
	int newstate;		/* 1: Statuswechsel */
};
typedef struct _gamestate GAMESTATE;

typedef struct
{
	int u;	//ã
	int d;	//‰º
	int l;	//¶
	int r;	//‰E
	int ba; //~
	int ma; //›
	int sa; //¢
	int si; // 
	int rt; //R
	int lt; //L
	int sl; //SELECT
	int st; //START
} KEYCONFIG;


#include "font.h"
#include "menu.h"
#include "hiscore.h"
#include "fps.h"
#include "soundmanager.h"
#include "bg.h"

//#include "_resource.h"

char moddir[20];

extern void game_init(int argc, char *argv[]);
//void toggle_fullscreen(void);
extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *loadbmp0(char *filename, int use_alpha);
extern SDL_Surface *loadbmp(char *filename);
extern SDL_Surface *loadbmp2(char *filename);




extern void unloadbmp_by_surface(SDL_Surface *s);
extern void unloadbmp_by_name(char *name);
extern void imglist_add(SDL_Surface *s, char *name);
extern SDL_Surface *imglist_search(char *name);
extern void imglist_garbagecollect(void);
extern Uint32 getpixel(SDL_Surface *surface, int x, int y);
extern void putpixel(SDL_Surface *surface, int x, int y, Uint32 color);
//extern void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2);
//extern void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1);
extern void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
//extern void blit_calpha(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
extern void keyboard_clear(void);
extern void keyboard_poll(void);
extern int keyboard_keypressed(void);
extern void newstate(int m, int s, int n);
extern void *mmalloc(size_t size);
extern void preload_gfx(void);
extern void load_ing(void);
extern void display_vidinfo(void);
extern int ini_load(void); 	//[***090110
extern void ini_save(void);	//[***090115

extern void psp_push_screen(void);
extern void psp_pop_screen(void);

#define DEBUG_MODE (1)

#endif
