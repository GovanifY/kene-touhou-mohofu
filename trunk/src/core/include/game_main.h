
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	インクルードファイルについて
---------------------------------------------------------*/

#if (0)

ファイル名の先頭が "000_" で始まる物は "game_main.h" 以外からインクルードしないでください。

これら "000_" で始まる物は 現状単に分割してあるだけで、分割してある意味はあまりありません。
判りにくくならないならば、適当に整理して纏める予定です。
(000_audio.h とか 000_support.h とかは、現状でも完全に機能で分割されています。
これらは纏めると判りにくくなるので纏めません。)

基本的に "game_main.h" をインクルードして、それ以外はインクルードしません。
例外はあります。

以下例外
---------------------------------------------------------
☆ シナリオ関連は考慮中なので、シナリオ関連を使う場合(主に漢字描画)
 "game_main.h" の後で、"scenario_script.h" を 直接インクルードします。
---------------------------------------------------------
☆ ファイル関連も拡張予定があります。ファイル関連を使う場合、
 "game_main.h" の後で、"111_my_file.h" を 直接インクルードします。
---------------------------------------------------------
☆ 道中の場合、道中共通処理が入るかもしれないので、
 "game_main.h" の代わりに "douchu.h" を 直接インクルードします。
---------------------------------------------------------
☆ ボスの場合、ボス共通処理が入るかもしれないので、
 "game_main.h" の代わりに "boss.h" を 直接インクルードします。
---------------------------------------------------------
☆ 描画関連場合、仕様がさっぱり決まってないので例外です。
---------------------------------------------------------

#endif /* (0) */

/*---------------------------------------------------------
	基本設定
	本ゲームの基本設定をこのファイルに書きます。
---------------------------------------------------------*/

#ifndef _MY_GAME_MAIN_H_
#define _MY_GAME_MAIN_H_

/*	インクルードファイル自体の基本設定	*/

/* 0:しない。 1:する。 [メモリゼロクリアー機能]
	1:なら、my_calloc()した際に 確保したメモリを0クリアーします。0ならしません。
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



/* 0:しない。 1:する。 現ver r31では 0 固定。既に 1 にできない。廃止予定。 */
#define USE_SIN_TABLE (0)

/* 0:しない。 1:する。 パーサー(設定読み込み時の字句解析)についてのデバッグ機能。 */
#define USE_PARTH_DEBUG (0)


#include "000_support.h"	/* (一番最初にインクルード) */

/*---------------------------------------------------------
	使用設定(次にインクルード)
	本システムをどのように使うか個別の設定
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
	SDL_00_VIEW_SCREEN		= 0,
	SDL_01_BACK_SCREEN,
//	SDL_02_TEX_SCREEN,
//	SDL_03_000_SCREEN,
	SDL_99_MAX_SCREEN,
};

//extern SDL_Surface *screen;
extern SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/

//1231101(Gu) 1229917(SDL)
//#define US E_GU			(1)

//(1==US E_GU)
//	#define SD L_VRAM_SCREEN	vr am_screen
	#define SDL_PSP_BUFFER_WIDTH512 PSP_BUFFER_WIDTH512
	#if 1
		/* ソフトウェアーで Zソート */
		#define USE_ZBUFFER 	(0)
	#else
		/* ハードウェアーでソート */
		#define USE_ZBUFFER 	(1)
	#endif

#if 0
// SDL */
	//#define SD L_VRAM_SCREEN		sdl_screen[SDL_00_VIEW_SCREEN]
	//#define SD L_PSP_BUFFER_WIDTH512	PSP_WIDTH480
#endif


enum /*_errlevel*/
{
	ERR_FATAL=0,
	ERR_WARN,
//	ERR_INFO,
//	ERR_DEBUG,
};
#if 0
	#define CHECKPOINT	error(ERR_DEBUG, "Checkpoint file:%s line:%d function:%s",__FILE__,__LINE__,__FUNCTION__)
#else
	#define CHECKPOINT	;
#endif


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

extern u32 my_pad;		/*今回入力*/
extern u32 my_pad_alter; /*前回入力*/


//#define IS_KEY BOARD_PULLED ((0==my_pad)&&(my_pad^my_pad_alter))/* 何かキーを離されたら */
/*	キーを離した瞬間が確定とする。押した瞬間が確定だと、押された間だけ動作する物が同時にあると都合が悪い。 */
/*
	r31:原作ではキーを押した瞬間が確定のものが多いので、おかしいところはキーを押した瞬間が確定に修正した。
*/
enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* ランクの最大数==(最高ランク+1) */
};


/*---------------------------------------------------------
	コールバック
---------------------------------------------------------*/

#if 1/*単純コールバック方式*/
extern void common_load_init(void);
extern void stage_clear_result_screen_start(void);/* for game_core.c ??stage_clear.c */
//
extern void stage_first_init(void);/* for game_core.c select_player.c */
extern void shooting_game_core_work(void);/* for pause.c ask_continue.c */
extern void difficulty_select_menu_start(void);
extern void gameover_start(void);/* for ask_continue.c */
extern void name_entry_start(void);/* for game_over.c */
//
extern void stage_select_menu_start(void);
extern void option_menu_start(void);
extern void story_script_start(void);
extern void yume_no_kiroku_start(void);
extern void key_config_start(void);
extern void music_room_start(void);
extern void title_menu_start(void);/*for pause.c ... */
extern void pause_menu_start(void);
extern void ask_continue_menu_start(void);
//

#define pause_out_call_func 	return_call_func/* ポーズ時の戻り先 */
#define menu_out_call_func		return_call_func/* 自機選択時の戻り先 */

extern void (*main_call_func)(void);/* メインコールバック */
extern void (*return_call_func)(void);/* ポーズ復帰後の戻り先コールバック */
/*
	return_call_func
	★ 用途1: pause_out_call_func : 今はgame_coreが1つしかないけど、game_coreが複数になる予定ですので、何処のgame_coreへ戻るかを保持。
	★ 用途2: menu_out_call_func  : メニューシステムでメニューがバックする場合に何処のメニューへ戻るかを保持。
*/
#endif

/*---------------------------------------------------------
	コンフィグ
---------------------------------------------------------*/

extern int pad_config[KEY_NUM12_MAX];
//extern int ke yconfig[KEY_NUM12_MAX];

/*---------------------------------------------------------
	スプライト
---------------------------------------------------------*/

#include "000_sprite_system.h"
//#include "bullet_system.h"

extern SPRITE *obj_player;
extern SPRITE *obj_boss;
extern SPRITE *obj_send1;

extern int player_now_stage;

extern int difficulty;

extern int msg_time;/* メッセージ(仮対応)表示時間 */

//#include "font.h"
//#include "menu.h"
//#ifndef _FONT_H_
//#define _FONT_H_

/*---------------------------------------------------------
	フォント
---------------------------------------------------------*/

enum FONTS
{
	FONT10W,	/*	8x10xWHITE 白 (mini) */
	FONT16R,	/* 16x16xRED   紅 */
	FONT16W,	/* 16x16xWHITE 白 */
	FONT_MAX
};

extern void font_init(void);

/* サーフェイスを作らないで、直接画面に表示 */
extern void font_print_screen_xy(char *text, int fontnr, int x, int y);

/* 新規サーフェイスを作成し、文字列をレンダリング */
extern SDL_Surface *font_render(char *text,int fontnr);

/* */
extern void font_print(char *text, int fontnr, int x, int y);

//#endif /* _FONT_H_ */


/* キャンセル音を鳴らしてメニューに強制復帰 */
extern void menu_cancel_and_voice(void);

#include "000_audio.h"

typedef struct
{
	char name[16/*4*/];
//
	u32 score;
	int player;
	int final_stage;	/* 到達ステージ */
	int difficulty; 	/* 難易度 */
//
	int use_continue;	/* コンティニュー回数 */
	int count_miss; 	/* ミス回数 */
	int used_bomber;	/* ボム使用回数 */
	int count_bonus;	/* スペルカードボーナス回数 */
} SCORE_FORMAT;

#define DIRECTRY_NAME_DATA_STR			"data"
/* 'data' の文字数は 4 文字 */
#define DIRECTRY_NAME_DATA_LENGTH		(4)

#define DIRECTRY_NAME_TEXT_STR			"/text/"
/* '/text/' の文字数は 6 文字 */
#define DIRECTRY_NAME_TEXT_LENGTH		(6)

		/* 'data' の文字数は 4 文字 */
	//	#define DIRECTRY_NAME_DATA_LENGTH		(4) 	// game_main.h で定義(変わる可能性があるから)
		/* '/text/' の文字数は 6 文字 */
	//	#define DIRECTRY_NAME_TEXT_LENGTH		(6)

#define DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR			".txt"




#include "000_player.h"
#include "000_bonus.h"
#include "000_load_stage.h"
#include "000_danmaku.h"
#include "000_bullet_object.h"

//#ifndef _HIGH_SCORE_H_
//#define _HIGH_SCORE_H_

#define MAX_8_SAVE_PLAYERS	(8)
#define MAX_5_RANKING		(5)

extern SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

//#endif /* _HIGH_SCORE_H_ */

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

extern int option_config[OPTION_CONFIG_08_MAX]; 	/* 8 */ 	// 数字=ボタン変数

enum /*_bg_resource_*/
{
	BG_TYPE_00_title_bg = 0,
	BG_TYPE_01_name_regist,
	BG_TYPE_02_key_config,
	BG_TYPE_03_music_room,
//	BG_TYPE_xx_loading,
	BG_TYPE_99_MAX_HAIKEI,
};
extern void load_SDL_bg(int bg_type_number);


/* Gu Render */
#include "000_gu_render.h"
#if 1

extern int draw_side_panel;/* パネル表示on(0以外)/off(0) */

extern int draw_boss_hp_value;	/* ボスhp描画値 */
extern int boss_life_value; 	/* ボスhp体力値 / ボス魔方陣サイズ描画値 */
#endif

extern void error(int errorlevel, char *msg, ...);
extern SDL_Surface *load_chache_bmp(char *filename);//, int use_alpha, int use_chache);

extern void unloadbmp_by_surface(SDL_Surface *s);

extern void *my_calloc(size_t size);


extern int tiny_strcmp(char *aaa, const char *bbb);/* MIPS R4000系にあわせて、最適化してみました。 */

extern char get_stage_chr(int i);

#if 1
extern void script_boss_load(int boss_number);
extern void script_boss_start(void/*int i*/);
#endif

extern void display_vidinfo(void);


extern void psp_pause_filter(void);/* 仮想スクリーンにもやもやエフェクトをかける */
extern void psp_clear_screen(void);/* 仮想スクリーンを黒で消す */
//extern void psp_push_screen(void);/* 仮想スクリーンを退避 */
//extern void psp_pop_screen(void);/* 仮想スクリーンを復活 */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_VIEW_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number );


#else
	include error!!
#endif /* _MY_GAME_MAIN_H_ */
