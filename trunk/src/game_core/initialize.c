
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�Q�[���V�X�e�������������A�֘A
---------------------------------------------------------*/

#include "kaiwa_system.h"

#include "kanji_system.h"


/*---------------------------------------------------------
	�Q�[���V�X�e������������
---------------------------------------------------------*/

extern void render_blit_fake_loading_init(void);
extern void render_blit_fake_loading_full(void);

extern void kanji_system_init(void);/* �g�ݍ��� */
extern void kanji_system_exit(void);/* �O�� */

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

	/* ----- ������ */
	psp_video_init01();
//	render_blit_fake_loading_full();
	psp_video_init02();
	psp_pad_init(); 	/* psp_video_init()����łȂ��Ɛ���� pad check �o���Ȃ��B */


	/* ----- �Q�[���{�̏����� */
	init_audio();
//	init_math();/*keyboard_clear();*/
	init_imglist();
//
	kaiwa_system_init();/* �g�ݍ��� */
//����	ending_system_init();/* �g�ݍ��� */
	kanji_system_init();/* �g�ݍ��� */
//
	#if 1//(1==US E_GU)
	#else
	load_SDL_bg(BG_TYPE_xx_loading);
	psp_pop_screen();
	#endif
//
	#if (1)/*Gu�ŕ`���O�ɕK�v�ȏ�����*/
	cg.bomber_time = 0;
	#endif
//
	#if 1//(1==US E_GU)
	#else
	SDL_Flip(sdl_screen[SDL_00_VIEW_SCREEN]);
	#endif
//	pr eload_gfx(); /*	�ǂݍ��񂾏��Ԃɉ摜�L���b�V���ɔz�u�����̂ŁA
//						�摜�L���b�V���̏��Ԃ����߂�ׂ̓ǂݍ��� */
//
	font_init();

	//fps_init();
//	obj_se nd1		= my_calloc(sizeof(SPRITE));/* �����󂯓n���p */
//	bg2_system_init();//	psp�N�����Ɉ�x��������������
	/* �Q�[���R�A game_core_init(); */
	cg.game_continue_stage			= (1-1);	/* (0) 0��1�ʂ���J�n�Ƃ����Ӗ��B */
//	pr actice_mode					= 0;
//	vo lume 						= 0;
//	za nki							= 2;
	old_menu_system_init();
//
	play_music_num(BGM_26_menu01);
	cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
}


/*---------------------------------------------------------
	�Q�[���V�X�e���J������
---------------------------------------------------------*/
void game_system_exit(void)
{
	ini_save();
	kanji_system_exit();
//	kaiwa_system_exit();
	exit_audio();
//	psp_denug_printf("Thank you for playing");
	#if (0)/*(r34)���������炸�A[�Ƃ肠����off](���Ԃ�t�H���g�֌W�̃������[���[�N)*/	//(1)/* r31 ���󂤂܂������Ȃ��ł��B*/
	/* ���Ԃ�SDL231�̊J�������Ƀo�O����Ǝv���܂��B
	�o�^(��̓I�ɂ�malloc�Ƃ��Ń������m��)���ĂȂ��̂�
	�J��(free�Ƃ�)���āA���낪����Ȃ��Ȃ��Ă�B
 */
	SDL_Quit();
	#endif /* (0) */
	#ifdef ENABLE_PROFILE
	gprof_cleanup();
	#endif
	sceKernelExitGame();
}

/*---------------------------------------------------------
	�v���t�@�C���[ gprof �̎g���������B
---------------------------------------------------------*/
#if 0
psp-gprof ./kene.elf gmon.out > gmon.txt

#endif
