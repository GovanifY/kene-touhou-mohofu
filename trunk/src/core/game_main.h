#ifndef _GAME_MAIN_H_
#define _GAME_MAIN_H_

#include "support.h"

/*---------------------------------------------------------
  macros
---------------------------------------------------------*/

/* 色設定 */
#define USE_PSP_5551	0
//#define USE_PSP_5551	1
#if (1==USE_PSP_5551)
	#define SDL_GU_PSM_0000 	GU_PSM_5551
	/*(PSPSDKの場合5-5-5-1)*/
	/*(pspのSDLでは特殊な操作しない限り5-5-5-0) */
	/* 5-5-5-1*/
	#define PSP_DEPTH16 	(16)
	#define SDL_5551_15 	(15)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x03e0)
	#define PSP_SCREEN_FORMAT_BMASK (0x7c00)
	#define PSP_SCREEN_FORMAT_AMASK (0x8000)
	#define PSP_SCREEN_FORMAT_LMASK (0x7bde)
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
#endif

enum
{
	SDL_00_SCREEN=0,
	SDL_01_BACK_SCREEN,
	SDL_02_TEX_SCREEN,
	SDL_03_000_SCREEN,
};

//1231101(Gu) 1229917(SDL)
#define USE_GU			(1)
//#define USE_GU			(0)
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
	#define CHECKPOINT error(ERR_DEBUG, "Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)
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



//extern dou  ble fps_fa ctor;
//extern SDL_Surface *screen;
extern SDL_Surface *sdl_screen[4];

#include "sprite.h"

extern SPRITE *player;

extern int player_now_stage;

extern int difficulty;


#include "font.h"
#include "menu.h"
#include "player.h"
//#include "fps.h"
#include "sound_manager.h"

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

#endif /* _GAME_MAIN_H_ */
