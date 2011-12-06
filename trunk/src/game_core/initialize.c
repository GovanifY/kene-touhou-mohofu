
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ゲームシステム初期化処理、関連
---------------------------------------------------------*/

#include "kaiwa_system.h"

#include "kanji_system.h"


/*---------------------------------------------------------
	ゲームシステム開放処理
---------------------------------------------------------*/
extern void ini_file_save(void);
/*only exit once*/extern void kanji_system_boot_exit(void);/* 外す */
/*only exit once*/extern void kaiwa_system_boot_exit(void);/* 外す */

/*only exit once*/extern void /*SD LCALL*/ PSPL_AudioQuit(void);
/*only exit once*/extern void /*SD LCALL*/ PSPL_VideoQuit(void);
void game_system_exit(void)
{
	ini_file_save();
	kanji_system_boot_exit();
//	kaiwa_system_boot_exit();
	exit_audio();
//	psp_denug_printf("Thank you for playing");
	#if (0)/*(r34)原因解からず、[とりあえずoff](たぶんフォント関係のメモリーリーク)*/	//(1)/* r31 現状うまくいかないです。*/
	/* たぶんSDL231の開放処理にバグあると思います。
	登録(具体的にはmallocとかでメモリ確保)してないのに
	開放(freeとか)して、辻褄が合わなくなってる。
 */
//	SDL_Quit();
	PSPL_VideoQuit();/*only exit once*/
	PSPL_AudioQuit();/*only exit once*/
	#endif /* (0) */
	#ifdef ENABLE_PROFILE
	gprof_cleanup();
	#endif
	sceKernelExitGame();
}

/*---------------------------------------------------------
	ゲームシステム初期化処理
---------------------------------------------------------*/

/*only boot once*/extern void kanji_system_boot_init(void);/* 組み込み */

extern void init_imglist(void);
//extern void init_math(void);
extern void ini_file_load(void);

extern void psp_pad_init(void);
extern void psp_video_init01(void);
extern void psp_video_init02(void);

/*only boot once*/extern void font_system_boot_init(void);
/*only boot once*/extern void kaiwa_system_boot_init(void);/* 組み込み */
void game_system_init(void)
{
	/* ----- 模倣風設定ファイルの読み込み */
	ini_file_load();

	/* ----- 初期化 */
	psp_video_init01();
//	kanji_system_init();/* 組み込み */	/*(漢字システムが無いとエラーが表示できない。)*/
	psp_video_init02();
	psp_pad_init(); 	/* psp_video_init()より後でないと正常に pad check 出来ない。 */

	/* ----- ゲーム本体初期化 */
	init_audio();
//	init_math();
	#if (1==USE_KETM_IMAGE_CHACHE)
	init_imglist();
	#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
//
	kaiwa_system_boot_init();/* 組み込み */
	kanji_system_boot_init();/* 組み込み */ 	/*(漢字システムが無いとエラーが表示できない。)*/
//未定	ending_system_init();/* 組み込み */
//
	font_system_boot_init();/*"256x256, SDLフォントをロード"*/

	/* ゲームコア game_core_init(); */
	#if (1)/*Guで描く前に必要な初期化*/
	cg.bomber_time = 0;
	#endif
	cg.game_continue_stage			= (1-1);	/* (0) 0は1面から開始という意味。 */
//
//	pl ay_music_num(BGM_27_menu01);
	cb.main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
}


/*---------------------------------------------------------
	プロファイラー gprof の使い方メモ。
---------------------------------------------------------*/
#if 0
psp-gprof ./kene.elf gmon.out > gmon.txt

#endif
