
/*---------------------------------------------------------
	�Q�[���V�X�e�������������A�֘A
---------------------------------------------------------*/

#include "game_main.h"
#include "scenario_script.h"

extern SDL_Surface *back_screen;

/*---------------------------------------------------------
	�Q�[���V�X�e������������
---------------------------------------------------------*/

extern void init_imglist(void);
extern void init_math(void);
extern void keyboard_clear(void);
extern void ini_load(void); 	// [***090110
extern void ini_save(void); 	// [***090115
extern void bg2_system_init(void);
extern void preload_gfx(void);
extern void psp_video_init(void);
void game_system_init(void/*int argc, char *argv[]*/)
{
	#if (1==USE_GU)
	#else
	int my_err;
	my_err =
	#endif
	SDL_Init(SDL_INIT_VIDEO/*initflags*/ /*| SDL_INIT_JOYSTICK*/ | SDL_INIT_AUDIO );
	#if (1==USE_GU)
	#else
	if (my_err < 0)
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant init SDL:"/*" %s",SDL_GetError()*/);
	}
	#endif
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	/* ----- �Q�[�����W���[���I�� */
	//#if defined(ENABLE_PSP)
	//#else
	pspDebugScreenInit();
	//#endif

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	ini_load();

	/* ----- ������ */
	psp_video_init();

	/* ----- �Q�[���{�̏����� */
	init_audio();/*ini_load();����(data_dir���킩��Ȃ��ƌ��ʉ����ǂ߂Ȃ��̂�)*/
	init_math();
	init_imglist();
	keyboard_clear();
//
	script_system_init();/* �g�ݍ��� */
//
	#if (1==USE_GU)
	#else
	{//static SDL_Surface *loadpic = NULL;		// load��ʗp
		SDL_Surface *loadpic=loadbmp0("bg/loading.png", 0, 0);
		SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
		unloadbmp_by_surface(loadpic);
	}
	psp_pop_screen();
	#endif
//
	#if (1==USE_GU)
	#else
	SDL_Flip(sdl_screen[SDL_00_SCREEN]);
	#endif
	preload_gfx();	/*	�ǂݍ��񂾏��Ԃɉ摜�L���b�V���ɔz�u�����̂ŁA
						�摜�L���b�V���̏��Ԃ����߂�ׂ̓ǂݍ��� */
//
	font_init();
	menusystem_init();
	//fps_init();
	bg2_system_init();
	/* �Q�[���R�A game_core_init(); */
//
	play_music(BGM_00_intro);
	/* ���C�����j���[�ɖ߂� */
	psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(/*ST_INTRO*/ST_MENU/*ST_START_INTRO*/,0,1);
}

/*---------------------------------------------------------
	�Q�[���V�X�e���J������
---------------------------------------------------------*/
void game_system_exit(void)
{
	ini_save();
	/* TODO: Free everything (memory, SDL_Surfaces, Joysticks...) */
	script_system_exit();
	exit_audio();
	//fprintf(stdout,"Thank you for playing\n");
	SDL_Quit(); 	// [***** �ǉ�:090103
	#ifdef ENABLE_PROFILE
	gprof_cleanup();
	#endif
	sceKernelExitGame();
}
#if 0
psp-gprof ./kene.elf gmon.out > gmon.txt

#endif
