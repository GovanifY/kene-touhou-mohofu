
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
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
 "game_main.h" の後で、"kaiwa_system.h" を 直接インクルードします。
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

/* 0:使わない。1:使う。 1:キーコンフィグ使う。0:キーコンフィグ機能OFF */
//#define USE_KEY_CONFIG (0)
#define USE_KEY_CONFIG (1)

/* 0:使わない。1:使う。  [1:キーコンフィグ使う場合で] 1:方向キーのキーコンフィグを使う。0:方向キーのキーコンフィグ機能OFF */
#define USE_KEY_CONFIG_ALLOW (0)

/* 0:しない。 1:する。	コンティニューした場合、スコアランキング */
#define USE_CONTINUED_RANKING	(0)

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


/* 0:しない。 1:する。 パーサー(設定読み込み時の字句解析)についてのデバッグ機能。 */
#define USE_PARTH_DEBUG (0)






#define USE_32BIT_DRAW_MODE (0)

/* 0:しない。 1:する。 easyの場合、5面終わりでBAD END。 */
//#define US E_EASY_BADEND (1)


/* 0:しない。 1:する。 作り中(r34)のテスト。 */
#define USE_GU_TACHIE_TEST (0)


#include "000_support.h"	/* (一番最初にインクルード) */

/*---------------------------------------------------------
	使用設定(次にインクルード)
	本システムをどのように使うか個別の設定
---------------------------------------------------------*/

/* 色設定 */
#if (0==USE_32BIT_DRAW_MODE)
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
#else
		/* 色32bitモード */
		#define SDL_GU_PSM_0000 		GU_PSM_8888
		/* 5-6-5-0 */
		#define PSP_DEPTH16 			(32)
		#define SDL_5551_15 			(32)
		#define PSP_SCREEN_FORMAT_RMASK (0x000000ff)
		#define PSP_SCREEN_FORMAT_GMASK (0x0000ff00)
		#define PSP_SCREEN_FORMAT_BMASK (0x00ff0000)
		#define PSP_SCREEN_FORMAT_AMASK (0xff000000)
		#define PSP_SCREEN_FORMAT_LMASK (0xfefefefe)
#endif




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



enum /*_game_rank_*/
{
	RANK_EASY = 0,
	RANK_NORMAL,	/*=1*/
	RANK_HARD,		/*=2*/
	RANK_LUNATIC,	/*=3*/
	RANK_MAX		/* ランクの最大数==(最高ランク+1) */
};

#include "000_my_math.h"
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
extern void story_mode_start(void);
extern void yume_no_kiroku_start(void);
extern void key_config_start(void);
extern void music_room_start(void);
extern void title_menu_start(void);/*for pause.c ... */
extern void pause_menu_start(void);
extern void ask_continue_menu_start(void);
//

#define pause_out_call_func  	return_call_func/* ポーズ時の戻り先 */
#define menu_out_call_func		return_call_func/* 自機選択時の戻り先 / オプションメニューの戻り先 */

/*
	return_call_func
	★ 用途1: pause_out_call_func : 今はgame_coreが1つしかないけど、game_coreが複数になる予定ですので、何処のgame_coreへ戻るかを保持。
	★ 用途2: menu_out_call_func  : メニューシステムでメニューがバックする場合に何処のメニューへ戻るかを保持。
*/
#endif

#if 1/*(メニュー関連)*/
/* キャンセル音を鳴らしてメニューに強制復帰 */
extern void menu_cancel_and_voice(void);
#endif

/*---------------------------------------------------------
	スプライト
---------------------------------------------------------*/

#include "000_sprite_system.h"
#include "000_gu_obj_type.h"
//#include "bullet_system.h"

//extern SPRITE *ob j_player;
extern SPRITE *global_obj_boss;/*(r35現在の仕様では、あたり判定の都合上要る。)*/




/*---------------------------------------------------------
	弾システム
		カードCPU
		発弾システム
		レーザーシステム
---------------------------------------------------------*/

#include "000_tama_system.h"	/*(r33から追加)*/

/*---------------------------------------------------------
	スペル生成システム(カードを生成)
---------------------------------------------------------*/

	/* 弾源x256 弾源y256 ボス中心から発弾。 */
//#define set_REG_DEST_XY(aaa) {REG_02_SEND1_BOSS_X_LOCATE = (aaa->cx256);	REG_03_SEND1_BOSS_Y_LOCATE = (aaa->cy256); }
extern void set_REG_DEST_XY(SPRITE *src);
#if 0/*あとで有効にする*/
extern void tmp_angleCCW65536_src_nerai(void);
extern void calculate_jikinerai(void);

extern void spell_cpu_douchuu_init(void);
extern void card_maikai_init(SPRITE *src);
extern void card_generate(SPRITE *src);
#endif

/*---------------------------------------------------------
	ゲームコアサポート
---------------------------------------------------------*/

#include "000_core_support.h"

/*---------------------------------------------------------
	ゲームコアシステム
---------------------------------------------------------*/

#include "000_game_core.h"


/* Gu Render */
#include "000_gu_render.h"


/* 会話システムから呼ばれるもの */
#if 1
extern void called_from_kaiwa_system_boss_load(int boss_number);
#endif

#else
	include error!!
error! "このファイルは何度もインクルードしません。"
#endif /* _MY_GAME_MAIN_H_ */
