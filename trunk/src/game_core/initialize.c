
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ゲームシステム初期化処理、関連
---------------------------------------------------------*/

#include "kaiwa_system.h"

#include "kanji_system.h"


/*---------------------------------------------------------
	ゲームシステム初期化処理
---------------------------------------------------------*/

extern void render_blit_fake_loading_init(void);
extern void render_blit_fake_loading_full(void);

extern void kanji_system_init(void);/* 組み込み */
extern void kanji_system_exit(void);/* 外す */

extern void init_imglist(void);
//extern void init_math(void);
extern void ini_load(void);
extern void ini_save(void);
//extern void bg2_system_init(void);
//extern void pr eload_gfx(void);
extern void psp_pad_init(void);
extern void psp_video_init01(void);
extern void psp_video_init02(void);
extern void old_menu_system_init(void);

void game_system_init(void)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ); /*| SDL_INIT_JOYSTICK*/
	/* ----- */
	pspDebugScreenInit();
//	render_blit_fake_loading_init();
//	render_blit_fake_loading_full();
	ini_load();

	/* ----- 初期化 */
	psp_video_init01();
//	render_blit_fake_loading_full();
	psp_video_init02();
	psp_pad_init(); 	/* psp_video_init()より後でないと正常に pad check 出来ない。 */


	/* ----- ゲーム本体初期化 */
	init_audio();
//	init_math();/*keyboard_clear();*/
	init_imglist();
//
	kaiwa_system_init();/* 組み込み */
//未定	ending_system_init();/* 組み込み */
	kanji_system_init();/* 組み込み */
//
	#if 1//(1==US E_GU)
	#else
	load_SDL_bg(BG_TYPE_xx_loading);
	psp_pop_screen();
	#endif
//
	#if (1)/*Guで描く前に必要な初期化*/
	cg.bomber_time = 0;
	#endif
//
	#if 1//(1==US E_GU)
	#else
	SDL_Flip(sdl_screen[SDL_00_VIEW_SCREEN]);
	#endif
//	pr eload_gfx(); /*	読み込んだ順番に画像キャッシュに配置されるので、
//						画像キャッシュの順番を決める為の読み込み */
//
	font_init();

	//fps_init();
//	obj_se nd1		= my_calloc(sizeof(SPRITE));/* 引数受け渡し用 */
//	bg2_system_init();//	psp起動時に一度だけ初期化する
	/* ゲームコア game_core_init(); */
	cg.game_continue_stage			= (1-1);	/* (0) 0は1面から開始という意味。 */
//	pr actice_mode					= 0;
//	vo lume 						= 0;
//	za nki							= 2;
	old_menu_system_init();
//
	play_music_num(BGM_26_menu01);
	cb.main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
}


/*---------------------------------------------------------
	ゲームシステム開放処理
---------------------------------------------------------*/
void game_system_exit(void)
{
	ini_save();
	kanji_system_exit();
//	kaiwa_system_exit();
	exit_audio();
//	psp_denug_printf("Thank you for playing");
	#if (0)/*(r34)原因解からず、[とりあえずoff](たぶんフォント関係のメモリーリーク)*/	//(1)/* r31 現状うまくいかないです。*/
	/* たぶんSDL231の開放処理にバグあると思います。
	登録(具体的にはmallocとかでメモリ確保)してないのに
	開放(freeとか)して、辻褄が合わなくなってる。
 */
	SDL_Quit();
	#endif /* (0) */
	#ifdef ENABLE_PROFILE
	gprof_cleanup();
	#endif
	sceKernelExitGame();
}

/*---------------------------------------------------------
	プロファイラー gprof の使い方メモ。
---------------------------------------------------------*/
#if 0
psp-gprof ./kene.elf gmon.out > gmon.txt

#endif
