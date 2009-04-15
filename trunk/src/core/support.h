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
	//# /* カスタムライブラリを使う */
	#include "SDL.h"
#else
	//# /* 標準ライブラリを使う */
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

/* PSPの表示画面領域 */
#define PSP_WIDTH   480
#define PSP_HEIGHT  272

/* シューティングGAME部分の表示画面領域 */
#define GAME_WIDTH  380
#define GAME_HEIGHT 272

#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

//#define M_PI_H		0.52359877559829887307		/* π ÷ 6 */

#define degtorad(x) (((M_PI*2)/360.0)*(x))		/* 2π ÷ 360 * X */
#define radtodeg(x) ( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* １周が360度の単位系(deg360)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg360_2rad(x) (((M_PI*2)/(360.0))*(x))
#define rad2deg360(x) ( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* １周が512度の単位系(deg512)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg512_2rad(x) (((M_PI*2)/(512.0))*(x))
//#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))+512)%512 )
//#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))+512)&(512-1) )
#define rad2deg512(x) ( (int)((x)*((512.0)/(M_PI*2))/*+512*/)&(512-1) )

/* １周が4096度の単位系(deg4096)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg4096_2rad(x) (((M_PI*2)/(4096.0))*(x))
//#define rad2deg4096(x) ( (int)((x)*((4096.0)/(M_PI*2))+4096)%4096 )
//#define rad2deg4096(x) ( (int)((x)*((4096.0)/(M_PI*2))+4096)&(4096-1) )
#define rad2deg4096(x) ( (int)((x)*((4096.0)/(M_PI*2))/*+4096*/)&(4096-1) )

/* １周が65536度の単位系(deg65536)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg65536_2rad(x) (((M_PI*2)/(65536.0))*(x))
//#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))+65536)%65536 )
//#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))+65536)&(65536-1) )
#define rad2deg65536(x) ( (int)((x)*((65536.0)/(M_PI*2))/*+65536*/)&(65536-1) )

/* １周が360度の単位系(deg360)を１周が512度の単位系(deg512)へ変換。及び逆変換。 */
#define deg_360_to_512(x) ((int)((x)*(512.0/360.0)))
#define deg_512_to_360(x) ((int)((x)*(360.0/512.0)))

/* １周が360度の単位系(deg360)を１周が1024度の単位系(deg1024)へ変換。及び逆変換。 */
#define deg_360_to_1024(x) ((int)((x)*(1024.0/360.0)))
#define deg_1024_to_360(x) ((int)((x)*(360.0/1024.0)))

/* １周が360度の単位系(deg360)を１周が4096度の単位系(deg4096)へ変換。及び逆変換。 */
#define deg_360_to_4096(x) ((int)((x)*(4096.0/360.0)))
#define deg_4096_to_360(x) ((int)((x)*(360.0/4096.0)))

/* １周の範囲内にクリッピング */
#define mask512(aaa) {aaa &= (512-1);}
#define mask4096(aaa) {aaa &= (4096-1);}

/* 数字をスコアに変換。及び逆変換。 */
#define score(x)   ((int)(((int)x)/10))
#define score_r(x) ((int)(((int)x)*10))

/* 数字を256固定小数点形式に変換。及びdoubleへ逆変換。 */
#define t256(x)             ((int)(x*256))
#define t256_to_double(x)   (((double)x)*(1.0/256.0))


/* ラジアン撲滅対策。pspは doubleの値を保持するだけでも遅い。(例えば関数の引数とか)
intで値を保持して、使う度に変換、逆変換した方が、ずっと速い。
そこでラジアン撲滅する為のマクロ */
//2687682 2688146
#if 0
	/* １周が512度の単位系の atan2(y,x), cos(x), sin(x)  */
	#define atan_512(y,x)   (rad2deg512(atan2(y,x)))
	#define co_s512(x)   	(cos(deg512_2rad(x)))
	#define si_n512(x)   	(sin(deg512_2rad(x)))
	/* 変な名前になってるのは cos( とか sin( で検索するため */
#else
	#define SINTABLE_SIZE	512
	#define SINTABLE_MASK	(SINTABLE_SIZE-1)

	#define OFFS_SIN 0
	#define OFFS_COS ((SINTABLE_SIZE/4)-1)/*127 90*/
	extern int atan_512(int y, int x);
	extern double sin_tbl512[SINTABLE_SIZE];
	#define co_s512(x)   	(sin_tbl512[((x+OFFS_COS)&(512-1))])
	#define si_n512(x)   	(sin_tbl512[((x         )&(512-1))])
#endif

/* rand()撲滅対策。本家にはリプレイ機能がある。
リプレイ機能を再現する為には内蔵の組み込み関数rand()は使えない。
周期の長い自前の擬似乱数の関数を用意する必要がある。
リプレイ開始時やゲーム開始時(面の始め)に乱数系列の初期化をする。 */
#define ra_nd(x)    	(rand(x))
/* 変な名前になってるのは rand( で検索するため */


enum _errlevel
{
	ERR_FATAL=0,
	ERR_WARN,
//	ERR_INFO,
//	ERR_DEBUG,
};
#if 0
	#define CHECKPOINT error(ERR_DEBUG,"Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)
#else
	#define CHECKPOINT ;
#endif

enum _state
{
	ST_GAME_QUIT=0,
//	ST_START_INTRO,
//	ST_INTRO,
	ST_MENU,
//	ST_GAME_DEMO,
	ST_GAME_PLAY,
	ST_GAME_OVER,
	ST_STORY,		/* [***20090223	追加  */
	ST_SHOW_HCLIST,
	ST_ENTRY_HCLIST,
	ST_KEY_CONFIG,
	ST_PLAYER_SELECT,
};
enum _keynum_		//キーコンフィグ用
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
	KEY_SC_SHOT,
//	KEY_MAX 	/* キーコンフィグ用の最大数 */
};
#define PSP_KEY_NONE	0
#define PSP_KEY_SELECT	PSP_CTRL_SELECT
#define PSP_KEY_PAUSE	PSP_CTRL_START
#define PSP_KEY_UP		PSP_CTRL_UP
#define PSP_KEY_RIGHT	PSP_CTRL_RIGHT
#define PSP_KEY_DOWN	PSP_CTRL_DOWN
#define PSP_KEY_LEFT	PSP_CTRL_LEFT
#define PSP_KEY_SYSTEM	PSP_CTRL_LTRIGGER
#define PSP_KEY_SLOW	PSP_CTRL_RTRIGGER
#define PSP_KEY_SC_SHOT PSP_CTRL_TRIANGLE
#define PSP_KEY_CANCEL	PSP_CTRL_CIRCLE
#define PSP_KEY_SHOT	PSP_CTRL_CROSS
#define PSP_KEY_BOMB	PSP_CTRL_SQUARE

extern Uint32 my_pad;		/*今回入力*/
extern Uint32 my_pad_alter;	/*前回入力*/


#define IS_KEYBOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* 何かキーを離されたら */
/*  キーを離した瞬間が確定とする。押した瞬間が確定だと、押された間だけ動作する物が同時にあると都合が悪い。 */

enum _game_rank_
{
	RANK_EASY=0,
	RANK_NORMAL/*=1*/,
	RANK_HARD/*=2*/,
	RANK_LUNATIC/*=3*/,
	RANK_MAX	/* ランクの最大数==(最高ランク+1) */
};


struct _gamestate
{
	int mainstate;		/* Aktueller Status */
	int substate;		/* Aktueller Status Subsystem */
	int newstate;		/* 1: Statuswechsel */
};
typedef struct _gamestate GAMESTATE;

typedef struct
{
	int u;	//上
	int d;	//下
	int l;	//左
	int r;	//右
	int ba; //×
	int ma; //○
	int sa; //△
	int si; //□
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
#include "startintro.h"

//#include "_resource.h"

char moddir[20];
char password[20];		/* [***090222 */

extern void game_init(void/*int argc, char *argv[]*/);
//void toggle_fullscreen(void);
extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *loadbmp0(char *filename, int use_alpha);
extern SDL_Surface *loadbmp(char *filename);
extern SDL_Surface *loadbmp2(char *filename);




extern void unloadbmp_by_surface(SDL_Surface *s);
extern void unloadbmp_by_name(char *name);
//static void imglist_add(SDL_Surface *s, char *name);
//static SDL_Surface *imglist_search(char *name);
//static void imglist_garbagecollect(void);
extern Uint32 getpixel(SDL_Surface *surface, int x, int y);
extern void putpixel(SDL_Surface *surface, int x, int y, Uint32 color);
//extern void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2);
//extern void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1);
extern void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
//extern void blit_calpha(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
extern void keyboard_clear(void);
extern void keyboard_poll(void);
//extern int keyboard_keypressed(void);
extern void newstate(int m, int s, int n);
extern void *mmalloc(size_t size);
//static void preload_gfx(void);
extern void load_ing(void);
extern void display_vidinfo(void);


extern void psp_clear_screen(void);
extern void psp_push_screen(void);
extern void psp_pop_screen(void);

/* 1:エンディングデバッグ機能。0:この機能OFF */
#define USE_ENDING_DEBUG (1)

#endif
