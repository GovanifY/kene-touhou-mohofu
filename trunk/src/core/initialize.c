
/*---------------------------------------------------------
	ゲームシステム初期化処理、関連
---------------------------------------------------------*/

#include "game_main.h"
#include "scenario_script.h"

extern SDL_Surface *back_screen;

/*---------------------------------------------------------
	ゲームシステム初期化処理
---------------------------------------------------------*/

extern void init_imglist(void);
extern void init_math(void);
extern void ini_load(void); 	// [***090110
extern void ini_save(void); 	// [***090115
extern void bg2_system_init(void);
//extern void pr eload_gfx(void);
extern void psp_pad_init(void);
extern void psp_video_init(void);

void game_system_init(void/*int argc, char *argv[]*/)
{
//	#if (1==USE_GU)
//	#else
//	int my_err;
//	my_err =
//	#endif
	SDL_Init(SDL_INIT_VIDEO/*initflags*/ /*| SDL_INIT_JOYSTICK*/ | SDL_INIT_AUDIO );
//	#if (1==USE_GU)
//	#else
//	if (my_err < 0)
//	{
//		CHECKPOINT;
//		error(ERR_FATAL, "cant init SDL:"/*" %s",SDL_GetError()*/);
//	}
//	#endif
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	/* ----- ゲームモジュール選択 */
	//#if defined(ENABLE_PSP)
	//#else
	pspDebugScreenInit();
	//#endif

	ini_load();

	/* ----- 初期化 */
	psp_video_init();
	psp_pad_init(); 	/* psp_video_init()より後でないと正常に pad check 出来ない。 */

	/* ----- ゲーム本体初期化 */
	init_audio();/*ini_load();より後(data_dirがわからないと効果音が読めないので)*/
	init_math();/*	keyboard_clear();*/
	init_imglist();
//
	script_system_init();/* 組み込み */
//
	#if (1==USE_GU)
	#else
	{//static SDL_Surface *loadpic = NULL;		// load画面用
		SDL_Surface *loadpic=loadbmp0("bg/loading.png", 0, 0);
		SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
		unloadbmp_by_surface(loadpic);
	}
	psp_pop_screen();
	#endif
//
	#if (1)/*Guで描く前に必要な初期化*/
	pd_bomber_time=0;
	#endif
//
	#if (1==USE_GU)
	#else
	SDL_Flip(sdl_screen[SDL_00_VIEW_SCREEN]);
	#endif
//	pr eload_gfx(); /*	読み込んだ順番に画像キャッシュに配置されるので、
//						画像キャッシュの順番を決める為の読み込み */
//
	font_init();
	menusystem_init();
	//fps_init();
	send1_obj		= mmalloc(sizeof(SPRITE));/* 引数受け渡し用 */
	bg2_system_init();
	/* ゲームコア game_core_init(); */
//
	play_music_num(BGM_21_menu01);
	/* メインメニューに戻る */
	psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);
}

/*---------------------------------------------------------
	ゲームシステム開放処理
---------------------------------------------------------*/
void game_system_exit(void)
{
	ini_save();
	/* TODO: Free everything (memory, SDL_Surfaces, Joysticks...) */
	script_system_exit();
	exit_audio();
	//fprintf(stdout,"Thank you for playing\n");
	SDL_Quit(); 	// [***** 追加:090103
	#ifdef ENABLE_PROFILE
	gprof_cleanup();
	#endif
	sceKernelExitGame();
}
#if 0
psp-gprof ./kene.elf gmon.out > gmon.txt

#endif
