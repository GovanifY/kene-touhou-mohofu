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
	#define SDL_GU_PSM_0000 		GU_PSM_5551
	/*(PSPSDKの場合5-5-5-1)*/
	/*(pspのSDLでは特殊な操作しない限り5-5-5-0) */
	/* 5-5-5-1*/
	#define PSP_DEPTH16 			(16)
	#define SDL_5551_15 			(15)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x03e0)
	#define PSP_SCREEN_FORMAT_BMASK (0x7c00)
	#define PSP_SCREEN_FORMAT_AMASK (0x8000)
	#define PSP_SCREEN_FORMAT_LMASK (0x7bde)
#else
	#define SDL_GU_PSM_0000 		GU_PSM_5650
	/* 5-6-5-0 */
	#define PSP_DEPTH16 			(16)
	#define SDL_5551_15 			(16)
	#define PSP_SCREEN_FORMAT_RMASK (0x001f)
	#define PSP_SCREEN_FORMAT_GMASK (0x07e0)
	#define PSP_SCREEN_FORMAT_BMASK (0xf800)
	#define PSP_SCREEN_FORMAT_AMASK (0x0000)
	#define PSP_SCREEN_FORMAT_LMASK (0xf7de)
#endif

enum
{
	SDL_00_SCREEN		= 0,
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
//	ST_INIT_PLAYER_SELECT				吸収。なし
//	ST_INIT_GAME_OVER					吸収。なし
//	ST_INIT_RESULT						吸収。なし
//	ST_INIT_KEY_CONFIG					吸収。なし
//	ST_INIT_STORY						吸収。なし
//	ST_INTRO,
//	ST_START_INTRO,
//	ST_GAME_DEMO,

enum /*_state*/
{
	ST_PSP_QUIT 				= 0x0000,
	ST_INIT_GAME_PLAY_common	= 0x0100,
	ST_WORK_GAME_PLAY			= 0x0200,
	ST_INIT_MENU				= 0x0300,
	ST_WORK_MENU				= 0x0400,
	ST_WORK_PLAYER_SELECT		= 0x0500,
	ST_INIT_NAME_ENTRY			= 0x0600,
	ST_WORK_NAME_ENTRY			= 0x0700,
//
	ST_WORK_STAGE_FIRST 		= 0x0800,
	ST_WORK_STAGE_CLEAR 		= 0x0900,
//
	ST_WORK_GAME_OVER			= 0x0a00,
	ST_WORK_RESULT				= 0x0b00,

	ST_WORK_STORY				= 0x0c00,		/* [***20090223 追加  */
	ST_WORK_OPTION_MENU 		= 0x0d00,
	ST_WORK_KEY_CONFIG			= 0x0e00,
	ST_WORK_MUSIC_ROOM			= 0x0f00,
};
#if 0
enum PspCtrlButtons
{
	PSP_CTRL_SELECT 	= 0x00000001,	/* Select button. */
	PSP_CTRL_START		= 0x00000008,	/* Start button. */
	PSP_CTRL_UP 		= 0x00000010,	/* Up D-Pad button. */
	PSP_CTRL_RIGHT		= 0x00000020,	/* Right D-Pad button. */
	PSP_CTRL_DOWN		= 0x00000040,	/* Down D-Pad button. */
	PSP_CTRL_LEFT		= 0x00000080,	/* Left D-Pad button. */
	PSP_CTRL_LTRIGGER	= 0x00000100,	/* Left trigger. */
	PSP_CTRL_RTRIGGER	= 0x00000200,	/* Right trigger. */
	PSP_CTRL_TRIANGLE	= 0x00001000,	/* Triangle button. */
	PSP_CTRL_CIRCLE 	= 0x00002000,	/* Circle button. */
	PSP_CTRL_CROSS		= 0x00004000,	/* Cross button. */
	PSP_CTRL_SQUARE 	= 0x00008000,	/* Square button. */
	PSP_CTRL_HOME		= 0x00010000,	/* Home button. In user mode this bit is set if the exit dialog is visible. */
	PSP_CTRL_HOLD		= 0x00020000,	/* Hold button. */
	PSP_CTRL_NOTE		= 0x00800000,	/* Music Note button. */
	PSP_CTRL_SCREEN 	= 0x00400000,	/* Screen button. */
	PSP_CTRL_VOLUP		= 0x00100000,	/* Volume up button. */
	PSP_CTRL_VOLDOWN	= 0x00200000,	/* Volume down button. */
	PSP_CTRL_WLAN_UP	= 0x00040000,	/* Wlan switch up. */
	PSP_CTRL_REMOTE 	= 0x00080000,	/* Remote hold position. */
	PSP_CTRL_DISC		= 0x01000000,	/* Disc present. */
	PSP_CTRL_MS 		= 0x02000000,	/* Memory stick present. */
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
#define PSP_KEY_SNAP_SHOT		PSP_CTRL_TRIANGLE
#define PSP_KEY_OPTION			PSP_CTRL_CIRCLE
#define PSP_KEY_SHOT_OK 		PSP_CTRL_CROSS
#define PSP_KEY_BOMB_CANCEL 	PSP_CTRL_SQUARE

enum
{
	KEY_NUM00_SELECT = 0,	/* SELECT */
	KEY_NUM01_START,		/* START */
	KEY_NUM02_UP,			/* 上 */
	KEY_NUM03_RIGHT,		/* 右 */
	KEY_NUM04_DOWN, 		/* 下 */
	KEY_NUM05_LEFT, 		/* 左 */
	KEY_NUM06_L_TRIG,		/* L */
	KEY_NUM07_R_TRIG,		/* R */
	KEY_NUM08_TRIANGLE, 	/* △ */
	KEY_NUM09_CIRCLE,		/* ○ */
	KEY_NUM10_CROSS,		/* × */
	KEY_NUM11_SQUARE,		/* □ */
	KEY_NUM12_MAX			/* 最大数 */
};

extern Uint32 my_pad;		/*今回入力*/
extern Uint32 my_pad_alter; /*前回入力*/


#define IS_KEYBOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* 何かキーを離されたら */
/*	キーを離した瞬間が確定とする。押した瞬間が確定だと、押された間だけ動作する物が同時にあると都合が悪い。 */

enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* ランクの最大数==(最高ランク+1) */
};

extern int psp_loop;

extern int pad_config[KEY_NUM12_MAX];
//extern int ke yconfig[KEY_NUM12_MAX];



//extern dou  ble fps_fa ctor;
//extern SDL_Surface *screen;
extern SDL_Surface *sdl_screen[4];

#include "sprite.h"
#include "bullet_system.h"

extern SPRITE *player;
extern SPRITE *dummy_obj;

extern int player_now_stage;

extern int difficulty;

extern int msg_time;/* メッセージ(仮対応)表示時間 */

#include "font.h"
#include "menu.h"
#include "player.h"
//#include "fps.h"
#include "audio.h"//#include "so und_manager.h"

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


/* オプションメニューで設定する設定値 */

enum /*_option_config_*/
{
	OPTION_CONFIG_00_PLAYER = 0,
	OPTION_CONFIG_01_BOMB,
	OPTION_CONFIG_02_BGM,
	OPTION_CONFIG_03_SOUND,
	OPTION_CONFIG_04_CURRENT_DIFFICULTY,
	OPTION_CONFIG_05_CURRENT_PLAYER,
	OPTION_CONFIG_06_ANALOG,
	OPTION_CONFIG_07_OPEN,
	OPTION_CONFIG_08_MAX	/* 最大数 */
};

extern int option_config[OPTION_CONFIG_08_MAX];		/* 8 */		// 数字=ボタン変数 並びは "bg/key_haikei_surface.png"


/* Gu */
#if 1
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */
extern int draw_side_panel;/* パネル表示on(0以外)/off(0) */

extern int draw_boss_hp_value;	/* ボスhp描画値 */
extern int boss_life_value; 	/* ボス魔方陣サイズ描画値 */
#endif
extern int boss_bgm_mode;

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


extern int tiny_strcmp(char *aaa, const char *bbb);/* MIPS R4000系にあわせて、最適化してみました。 */

extern void display_vidinfo(void);


extern void psp_clear_screen(void);/* 仮想スクリーンを黒で消す */
//extern void psp_push_screen(void);/* 仮想スクリーンを退避 */
//extern void psp_pop_screen(void);/* 仮想スクリーンを復活 */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number );

/* 0:しない。 1:する。 [メモリゼロクリアー機能]
	1:なら、mmalloc()した際に 確保したメモリを0クリアーします。0ならしません。
	大量に弾を出す場合どちらがいいか判りません(0:速いがバグる可能性、1:遅いが安定度は高い)が、
	1:の方がいろんな意味でバグ抑制できます。 */
//#define USE_MEM_CLEAR (0)
#define USE_MEM_CLEAR (1)

/*廃止*/	/* 0:使わない。1:使う。 1:エンディングデバッグ機能。0:この機能OFF */
/*廃止*/	//#define US E_ENDING_DEBUG (1)
/*廃止*/	//#define MA X_STAGE6_FOR_CHECK (6/*5*/)

/* 0:使わない。1:使う。 1:キーコンフィグ使う。0:キーコンフィグ機能OFF */
//#define USE_KEY_CONFIG (0)
#define USE_KEY_CONFIG (1)

/* 0:使わない。1:使う。  [1:キーコンフィグ使う場合で] 1:方向キーのキーコンフィグを使う。0:方向キーのキーコンフィグ機能OFF */
#define USE_KEY_CONFIG_ALLOW (0)

/* 0:しない。 1:する。	コンティニューした場合、スコアランキング */
#define USE_CONTINUED_RANKING (1)

/* 0:しない。 1:する。 エクステンドチェック */
//#define USE_EXTEND_CHECK (0)
#define USE_EXTEND_CHECK (1)

/* 0:しない。 1:する。 カンスト(スコアカウンター ストップ)チェック */
//#define USE_MAX_SCORE_COUNTER_STOP_CHECK (0)
#define USE_MAX_SCORE_COUNTER_STOP_CHECK (0)
/*
カンストは 99,9999,9990pts (99億点)ですが、チョットぐらいの調整じゃカンストしそうにない。
(グレイズを10万の位まで稼いだり、いろいろ実験してみたけど...)
アリス：カンストなんて無いわ！
*/

#endif /* _GAME_MAIN_H_ */
