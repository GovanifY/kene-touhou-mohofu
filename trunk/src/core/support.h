#ifndef _SUPPORT_H_
#define _SUPPORT_H_

// PSPSDK(standard)

#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>

// C(standard)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// PSPSDK(debug)

#include <pspdebug.h>
#ifdef ENABLE_PROFILE
	#include <pspprof.h>
#endif

// PSPSDK(optional)

//#include <pspsdk.h>
//#include <psputility.h>
//#include <pspmoduleinfo.h>
//#include <pspnet_apctl.h>
//#include <pspnet_inet.h>
//#include <pspnet_resolver.h>
//#include <netinet/in.h>
//#include "net_io.h"
//#include <pspuser.h>

// C(optional)

#include <unistd.h>
#include <stdarg.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>/*sin(),atan2()*/
//#include <signal.h>


#ifdef ENABLE_PSP
	//# /* カスタムライブラリを使う */
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_mixer.h"
#else
	//# /* 標準ライブラリを使う */
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_mixer.h>/*#include "SDL_mixer.h"*/
#endif

	//
	#ifndef UINT8
		#define UINT8 Uint8
	#endif
	#ifndef UINT16
		#define UINT16 Uint16
	#endif
	#ifndef UINT32
		#define UINT32 Uint32
	#endif
	//
	#ifndef INT8
		#define INT8 Sint8
	#endif
	#ifndef INT16
		#define INT16 Sint16
	#endif
	#ifndef INT32
		#define INT32 Sint32
	#endif

/*---------------------------------------------------------
  macros
---------------------------------------------------------*/
/* キャッシュなしVRAM  VRAM[y][x]としてアクセス */
//#define	VRAM		((long(*)[BUF_WIDTH])(((char*)0x4000000)+0x40000000))




/* 色設定 */
#define USE_PSP_5551	0
#if (1==USE_PSP_5551)
	#define SDL_GU_PSM_0000 	GU_PSM_5551
	/* 5-5-5-1(pspのSDLでは特殊な操作しない限り5-5-5-0) */
	#define PSP_DEPTH16 	(16)
	#define SDL_5551_15 	(15)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x03e0)
	#define PSP_SCREEN_FORMAT_BMASK (0x7c00)
	#define PSP_SCREEN_FORMAT_AMASK (0x8000)
	#define PSP_SCREEN_FORMAT_LMASK (0x7bde)
	/*
	15: abbb bbgg gggr rrrr
	15: -bbb b-gg gg-r rrr- (pspのSDLではaが使えない？ので)
	*/
	#define MAKECOL15(r, g, b)	(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3))
	#define GETR15F(col)		(((col << 3) & 0xf8) /*| ((col >>  2) & 0x07)*/)
	#define GETG15F(col)		(((col >> 2) & 0xf8) /*| ((col >>  7) & 0x07)*/)
	#define GETB15F(col)		(((col >> 7) & 0xf8) /*| ((col >> 12) & 0x07)*/)
	#define GETR15(col) 		(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
	#define GETG15(col) 		(((col >> 2) & 0xf8) | ((col >>  7) & 0x07))
	#define GETB15(col) 		(((col >> 7) & 0xf8) | ((col >> 12) & 0x07))
#else
	#define SDL_GU_PSM_0000 	GU_PSM_5650
	/* 5-6-5-0 */
	#define PSP_DEPTH16 	(16)
	#define SDL_5551_15 	(16)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x07e0)
	#define PSP_SCREEN_FORMAT_BMASK (0xf800)
	#define PSP_SCREEN_FORMAT_AMASK (0x0000)
	#define PSP_SCREEN_FORMAT_LMASK (0xf7de)
	/*
	16: bbbb bggg gggr rrrr
	16: bbbb -ggg gg-r rrr-
	*/
	#define MAKECOL16(r, g, b)	(((b & 0xf8) << 8) | ((g & 0xfc) << 3) | ((r & 0xf8) >> 3))
	#define GETR16F(col)		(((col << 3) & 0xf8) /*| ((col >>  2) & 0x07)*/)
	#define GETG16F(col)		(((col >> 3) & 0xfc) /*| ((col >>  8) & 0x03)*/)
	#define GETB16F(col)		(((col >> 8) & 0xf8) /*| ((col >> 13) & 0x07)*/)
	#define GETR16(col) 		(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
	#define GETG16(col) 		(((col >> 3) & 0xf8) | ((col >>  8) & 0x07))
	#define GETB16(col) 		(((col >> 8) & 0xf8) | ((col >> 13) & 0x07))
#endif

/* PSPの表示画面領域 */
#define BUF_WIDTH512	(512)
#define PSP_WIDTH480	(480)
#define PSP_HEIGHT272	(272)

/* シューティングGAME部分の表示画面領域 */
#define GAME_WIDTH		(380)
#define GAME_HEIGHT 	(272)

enum
{
	SDL_00_SCREEN=0,
	SDL_01_BACK_SCREEN,
	SDL_02_TEX_SCREEN,
	SDL_03_000_SCREEN,
};

//1231101(Gu) 1229917(SDL)
//#define USE_GU			(1)
#define USE_GU			(0)
#if (1==USE_GU)
	#define SDL_VRAM_SCREEN 	vram_screen
	#define SDL_BUF_WIDTH512	BUF_WIDTH512
	#if 1
		/* ソフトウェアーで Zソート */
		#define USE_ZBUFFER 	(0)
	#else
		/* ハードウェアーでソート */
		#define USE_ZBUFFER 	(1)
	#endif
#else
	#define SDL_VRAM_SCREEN 	sdl_screen[SDL_00_SCREEN]
	#define SDL_BUF_WIDTH512	PSP_WIDTH480
#endif

#if 1
	/* 単純拡大 */
	#define USE_ZOOM_XY 	0
#else
	/* 縦横拡大 */
	#define USE_ZOOM_XY 	1
#endif

/* C:/cygwin/pspdev/psp/include/math.h で宣言されているので要らない */
//#ifndef M_PI
//	#define M_PI			(3.14159265358979323846)
//#endif

//#define M_PI_H			(0.52359877559829887307)		/* π ÷ 6 */

//#define degtorad(x)		(((M_PI*2)/360.0)*(x))		/* 2π ÷ 360 * X */
//#define radtodeg(x)		( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* １周が360度の単位系(deg360)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg360_2rad(x)		(((M_PI*2)/(360.0))*(x))
#define rad2deg360(x)		( (int)((x)*((360.0)/(M_PI*2))+360)%360 )

/* １周が512度の単位系(deg512)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg512_2rad(x)		(((M_PI*2)/(512.0))*(x))
//#define rad2deg512(x) 	( (int)((x)*((512.0)/(M_PI*2))+512)%512 )
//#define rad2deg512(x) 	( (int)((x)*((512.0)/(M_PI*2))+512)&(512-1) )
#define rad2deg512(x)		( (int)((x)*((512.0)/(M_PI*2))/*+512*/)&(512-1) )

/* １周が4096度の単位系(deg4096)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg4096_2rad(x) 	(((M_PI*2)/(4096.0))*(x))
//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(M_PI*2))+4096)%4096 )
//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(M_PI*2))+4096)&(4096-1) )
#define rad2deg4096(x)		( (int)((x)*((4096.0)/(M_PI*2))/*+4096*/)&(4096-1) )

/* １周が65536度の単位系(deg65536)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
#define deg65536_2rad(x)	(((M_PI*2)/(65536.0))*(x))
//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(M_PI*2))+65536)%65536 )
//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(M_PI*2))+65536)&(65536-1) )
#define rad2deg65536(x) 	( (int)((x)*((65536.0)/(M_PI*2))/*+65536*/)&(65536-1) )

/* １周が360度の単位系(deg360)を１周が512度の単位系(deg512)へ変換。及び逆変換。 */
#define deg_360_to_512(x)	((int)((x)*(512.0/360.0)))
#define deg_512_to_360(x)	((int)((x)*(360.0/512.0)))

/* １周が360度の単位系(deg360)を１周が1024度の単位系(deg1024)へ変換。及び逆変換。 */
#define deg_360_to_1024(x)	((int)((x)*(1024.0/360.0)))
#define deg_1024_to_360(x)	((int)((x)*(360.0/1024.0)))

/* １周が360度の単位系(deg360)を１周が4096度の単位系(deg4096)へ変換。及び逆変換。 */
#define deg_360_to_4096(x)	((int)((x)*(4096.0/360.0)))
#define deg_4096_to_360(x)	((int)((x)*(360.0/4096.0)))

/* １周の範囲内にクリッピング */
#define mask512(aaa)	{aaa &= (512-1);}
#define mask4096(aaa)	{aaa &= (4096-1);}

/* 数字をスコアに変換。及び逆変換。 */
#define score(x)	((int)(((int)(x))/10))
#define score_r(x)	((int)(((int)(x))*10))

/* 数字を256固定小数点形式に変換。及びdou bleへ逆変換。 */
#define t256(x) 				((int)((x)*256))
//#define t256_to_dou ble(x)	(((dou ble)(x))*(1.0/256.0))
#define t256_floor(x)			((x)>>(8))

/* 数字を8固定小数点形式に変換。及びdou bleへ逆変換。 */
#define t8(x)					((short)((x)*8))
//#define t8_to_dou ble(x)		(((dou ble)(x))*(1.0/8.0))
#define t8_floor(x) 			((x)>>(3))


/* aaa%の確率で */
#define rand_percent(aaa) ((unsigned char)(ra_nd())<=(unsigned char)( (aaa*256)/100 ) ) /* aaa%の確率で */

/* ラジアン撲滅対策。pspは dou bleの値を保持するだけでも遅い。(例えば関数の引数とか)
intで値を保持して、使う度に変換、逆変換した方が、ずっと速い。
そこでラジアン撲滅する為のマクロ */
#if 0
	/* １周が512度の単位系の atan2(y,x), cos(x), sin(x)  */
	#define atan_512(y,x)	(rad2deg512(atan2((y),(x))))
	#define co_s512(x)		(cos(deg512_2rad((x))))
	#define si_n512(x)		(sin(deg512_2rad((x))))
	/* 変な名前になってるのは cos( とか sin( で検索するため */
#else
	#define SINTABLE_SIZE	512
	#define SINTABLE_MASK	(SINTABLE_SIZE-1)

	#define OFFS_SIN 0
	#define OFFS_COS ((SINTABLE_SIZE/4)-1)/*127 90*/
	extern int atan_512(int y, int x);
	extern /*dou ble*/int sin_tbl512[SINTABLE_SIZE];
	#define cos512(x)		(sin_tbl512[(((x)+OFFS_COS)&(512-1))])
	#define sin512(x)		(sin_tbl512[(((x)		  )&(512-1))])
#endif

/* rand()撲滅対策。本家にはリプレイ機能がある。
リプレイ機能を再現する為には内蔵の組み込み関数rand()は使えない。
周期の長い自前の擬似乱数の関数を用意する必要がある。
リプレイ開始時やゲーム開始時(面の始め)に乱数系列の初期化をする。 */
/* 変な名前になってるのは rand( で検索するため */
#if 0
	#define ra_nd(x)		(rand(x))
#else
	extern int ra_nd(void);
#endif

/* 難易度スコア補正 */
extern int adjust_score_by_difficulty(int convert_score);

enum /*_errlevel*/
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

enum /*_state*/
{
	ST_PSP_QUIT 			= 0x0000,
	ST_INIT_GAME_PLAY		= 0x0100,
	ST_WORK_GAME_PLAY		= 0x0200,
	ST_INIT_MENU			= 0x0300,
	ST_WORK_MENU			= 0x0400,
//	ST_INIT_PLAYER_SELECT				吸収。なし
	ST_WORK_PLAYER_SELECT	= 0x0500,
	ST_INIT_NAME_ENTRY		= 0x0600,
	ST_WORK_NAME_ENTRY		= 0x0700,
//
	ST_WORK_STAGE_CLEAR 	= 0x0800,
//
//	ST_INIT_GAME_OVER					吸収。なし
	ST_WORK_GAME_OVER		= 0x0900,
//	ST_INIT_RESULT						吸収。なし
	ST_WORK_RESULT			= 0x0a00,
//	ST_INIT_KEY_CONFIG					吸収。なし
	ST_WORK_KEY_CONFIG		= 0x0b00,
//	ST_INIT_STORY						吸収。なし
	ST_WORK_STORY			= 0x0c00,		/* [***20090223 追加  */
//	ST_INTRO,
//	ST_START_INTRO,
//	ST_GAME_DEMO,
};
#if 0
enum /*_keynum_*/		//キーコンフィグ用
{
	KINOU_00_NONE = 0,
	KINOU_11_SHOT,
	KINOU_12_BOMB,
	KINOU_09_SLOW,
	KINOU_03_UP,
	KINOU_05_DOWN,
	KINOU_06_LEFT,
	KINOU_04_RIGHT,
	KINOU_02_PAUSE,
	KINOU_10_OPTION,
	KINOU_07_SNAP_SHOT,
	KINOU_01_SELECT,
	KINOU_08_SYSTEM,
	KINOU_13_MAX	/* キーコンフィグ用の最大数 */
};
#else
enum /*_keynum_*/		//キーコンフィグ用
{
	KINOU_00_NONE = 0,
	KINOU_01_SELECT,
	KINOU_02_PAUSE,
	KINOU_03_UP,
	KINOU_04_RIGHT,
	KINOU_05_DOWN,
	KINOU_06_LEFT,
	KINOU_07_SNAP_SHOT,
	KINOU_08_SYSTEM,
	KINOU_09_SLOW,
	KINOU_10_OPTION,
	KINOU_11_SHOT,
	KINOU_12_BOMB,
	KINOU_13_MAX	/* キーコンフィグ用の最大数 */
};
#endif

#define PSP_KEY_NONE			0
#define PSP_KEY_SELECT			PSP_CTRL_SELECT
#define PSP_KEY_PAUSE			PSP_CTRL_START
#define PSP_KEY_UP				PSP_CTRL_UP
#define PSP_KEY_RIGHT			PSP_CTRL_RIGHT
#define PSP_KEY_DOWN			PSP_CTRL_DOWN
#define PSP_KEY_LEFT			PSP_CTRL_LEFT
#define PSP_KEY_SYSTEM			PSP_CTRL_LTRIGGER
#define PSP_KEY_SLOW			PSP_CTRL_RTRIGGER
#define PSP_KEY_SC_SHOT 		PSP_CTRL_TRIANGLE
#define PSP_KEY_OPTION			PSP_CTRL_CIRCLE
#define PSP_KEY_SHOT_OK 		PSP_CTRL_CROSS
#define PSP_KEY_BOMB_CANCEL 	PSP_CTRL_SQUARE

extern Uint32 my_pad;		/*今回入力*/
extern Uint32 my_pad_alter; /*前回入力*/


#define IS_KEYBOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* 何かキーを離されたら */
/*	キーを離した瞬間が確定とする。押した瞬間が確定だと、押された間だけ動作する物が同時にあると都合が悪い。 */

enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL/*=1*/,
	RANK_HARD/*=2*/,
	RANK_LUNATIC/*=3*/,
	RANK_MAX	/* ランクの最大数==(最高ランク+1) */
};

extern int psp_loop;

enum
{
	key_00_sl = 0,	//SELECT
	key_01_st,		//START
	key_02_u,	//上
	key_03_r,	//右
	key_04_d,	//下
	key_05_l,	//左
	key_06_lt,	//L,
	key_07_rt,	//R,
	key_08_sa,	//△,
	key_09_ma,	//○,
	key_10_ba,	//×,
	key_11_si,	//□,
	key_MAX 	/* 最大数 */
};

extern int keyconfig[key_MAX];



//extern dou ble fps_fa ctor;
//extern SDL_Surface *screen;
extern SDL_Surface *sdl_screen[4];

#include "sprite.h"

extern SPRITE *player;

extern int player_now_stage;

extern int difficulty;


#include "font.h"
#include "menu.h"
#include "player.h"
#include "fps.h"
#include "soundmanager.h"

//#include "sta rtint ro.h"
//#include "particle.h"

//#include "_resource.h"

typedef struct
{
	char name[4];
	int score;
	int player;
} SCORE_FORMAT;

//extern char moddir[20];
//extern char data_dir[20];
#define DIRECTRY_NAME_DATA			"data"
#define DIRECTRY_NAME_LENGTH		(4)


//void toggle_fullscreen(void);
extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *loadbmp0(char *filename, int use_alpha, int use_chache);
extern SDL_Surface *loadbmp1(char *filename);
extern SDL_Surface *loadbmp2(char *filename);


extern void unloadbmp_by_surface(SDL_Surface *s);
//extern void unload_bmp_by_name(char *name);
//static void imglist_add(SDL_Surface *s, char *name);
//static SDL_Surface *imglist_search(char *name);
//static void imglist_garbagecollect(void);
extern Uint32 getpixel(SDL_Surface *surface, int x, int y);
extern void putpixel(SDL_Surface *surface, int x, int y, Uint32 color);
//extern void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2);
//extern void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1);
extern void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);
//extern void blit_calpha(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct);


//extern int keyboard_keypressed(void);
//extern void newsta te(int m, int s, int n);
extern void *mmalloc(size_t size);


extern void display_vidinfo(void);


extern void psp_clear_screen(void);/* 仮想スクリーンを黒で消す */
//extern void psp_push_screen(void);/* 仮想スクリーンを退避 */
//extern void psp_pop_screen(void);/* 仮想スクリーンを復活 */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number );

/* 1:エンディングデバッグ機能。0:この機能OFF */
#define USE_ENDING_DEBUG (1)


/* 1:キーコンフィグ使う。0:キーコンフィグ機能OFF */

//#define USE_KEY_CONFIG (0)
#define USE_KEY_CONFIG (1)

/* [1:キーコンフィグ使う場合で] 1:方向キーのキーコンフィグを使う。0:方向キーのキーコンフィグ機能OFF */
#define USE_KEY_CONFIG_ALLOW (0)

#endif
