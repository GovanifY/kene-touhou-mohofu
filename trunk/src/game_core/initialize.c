
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�Q�[���V�X�e�������������A�֘A
---------------------------------------------------------*/

#include "kaiwa_system.h"

#include "kanji_system.h"


/*---------------------------------------------------------
	�Q�[���V�X�e���J������
---------------------------------------------------------*/
extern void ini_file_save(void);
/*only exit once*/extern void kanji_system_boot_exit(void);/* �O�� */
/*only exit once*/extern void kaiwa_system_boot_exit(void);/* �O�� */

/*only exit once*/extern void /*SD LCALL*/ PSPL_AudioQuit(void);
/*only exit once*/extern void /*SD LCALL*/ PSPL_VideoQuit(void);
void game_system_exit(void)
{
	ini_file_save();
	kanji_system_boot_exit();
//	kaiwa_system_boot_exit();
	exit_audio();
//	psp_denug_printf("Thank you for playing");
	#if (0)/*(r34)���������炸�A[�Ƃ肠����off](���Ԃ�t�H���g�֌W�̃������[���[�N)*/	//(1)/* r31 ���󂤂܂������Ȃ��ł��B*/
	/* ���Ԃ�SDL231�̊J�������Ƀo�O����Ǝv���܂��B
	�o�^(��̓I�ɂ�malloc�Ƃ��Ń������m��)���ĂȂ��̂�
	�J��(free�Ƃ�)���āA���낪����Ȃ��Ȃ��Ă�B
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
	�Q�[���V�X�e������������
---------------------------------------------------------*/

/*only boot once*/extern void kanji_system_boot_init(void);/* �g�ݍ��� */

extern void init_imglist(void);
//extern void init_math(void);
extern void ini_file_load(void);

extern void psp_pad_init(void);
extern void psp_video_init01(void);
extern void psp_video_init02(void);

/*only boot once*/extern void font_system_boot_init(void);
/*only boot once*/extern void kaiwa_system_boot_init(void);/* �g�ݍ��� */
void game_system_init(void)
{
	/* ----- �͕핗�ݒ�t�@�C���̓ǂݍ��� */
	ini_file_load();

	/* ----- ������ */
	psp_video_init01();
//	kanji_system_init();/* �g�ݍ��� */	/*(�����V�X�e���������ƃG���[���\���ł��Ȃ��B)*/
	psp_video_init02();
	psp_pad_init(); 	/* psp_video_init()����łȂ��Ɛ���� pad check �o���Ȃ��B */

	/* ----- �Q�[���{�̏����� */
	init_audio();
//	init_math();
	#if (1==USE_KETM_IMAGE_CHACHE)
	init_imglist();
	#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
//
	kaiwa_system_boot_init();/* �g�ݍ��� */
	kanji_system_boot_init();/* �g�ݍ��� */ 	/*(�����V�X�e���������ƃG���[���\���ł��Ȃ��B)*/
//����	ending_system_init();/* �g�ݍ��� */
//
	font_system_boot_init();/*"256x256, SDL�t�H���g�����[�h"*/

	/* �Q�[���R�A game_core_init(); */
	#if (1)/*Gu�ŕ`���O�ɕK�v�ȏ�����*/
	cg.bomber_time = 0;
	#endif
	cg.game_continue_stage			= (1-1);	/* (0) 0��1�ʂ���J�n�Ƃ����Ӗ��B */
//
//	pl ay_music_num(BGM_27_menu01);
	cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
}


/*---------------------------------------------------------
	�v���t�@�C���[ gprof �̎g���������B
---------------------------------------------------------*/
#if 0
psp-gprof ./kene.elf gmon.out > gmon.txt

#endif
