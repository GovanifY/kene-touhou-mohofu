
/*---------------------------------------------------------
	psp�̋N�����[�`����
---------------------------------------------------------*/

#include "game_main.h"

#if 0
	/* makefile �� -Dmain=SDL_main ������ꍇ  */
/*
	 makefile �� -Dmain=SDL_main ������ꍇ�́A
	SDL���p�ӂ������C�����[�`�����g�p����̂ŁA�����ɂ�
	�����܂���B(C�����main()�֐���2�����N�o���܂���)
	SDL���p�ӂ������C�����[�`���́A
	PSP-1000�ł�version�ɂ���ċN�������肵�Ȃ�������A
	PSP-2000�͋N�����Ȃ�������AC++���g���Ɖ��̂��N�������肵�Ȃ�������A
	�ƐF�X��肪�����̂Ŏg��Ȃ����������ł��B
*/
#else
	/* makefile �� -Dmain=SDL_main ���Ȃ��ꍇ  */
/*
	������̏ꍇ�� PSPSDK �̗p�ӂ����N�����[�`�����g�p����ꍇ�ł��B
	�N�����[�`���͊ȒP�ȃA�Z���u���{PSP��OS�ɗ��p�֐���m�点��\��
	���L�}�N���͋N���A�Z���u���̈ꕔ�ɂɓW�J����܂����A
	�\�� PSPSDK �������쐬���Ă���܂��B
*/
/*---------------------------------------------------------
	PSP module info section
---------------------------------------------------------*/

PSP_MODULE_INFO("KENE", 0x0000, 0, 4);	/* PSP��OS�ɋ����Ă����閼�O�B���[�U�[���[�h�ŋN������B */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER|PSP_THREAD_ATTR_CLEAR_STACK); /* ���̃v���O�������g��OS�����B*/
/* ���[�U�[���[�h�œ��삷��(�͈͂����g��Ȃ�)��B(kernel�n�R�[�����g��Ȃ��B) */
/* VFPU �Ƃ� USB/WLAN �Ƃ� SCRATCH_SRAM �Ƃ��g��Ȃ���B */
/* �I�����ɂ��̃v���O�������g�����X�^�b�N(�ϐ��̈�)��OS�ɏ����Ă��炤��B */
PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (�v���O�������g�p����ϐ���)�X�^�b�N�̈�̃T�C�Y���� */
										/* �������傫����malloc�ł���ʂ����̕������ */
/* 2008�N���炢�̌Â� PSPSDK �� -1(����) �ɑΉ����Ă��Ȃ��̂� -1 �ɂ���ƃn���O�A�b�v����� */
#endif

//extern int psp_loop;
/*extern*/ int psp_loop;

/*---------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^
	([HOME]�L�[���j���[�ŁA�I�����܂����H�u�͂��v���I�������Ǝ��s����镔��)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* �R�[���o�b�N������A���C�����̏�Ԃ𑀍삷�� */
	psp_loop = ST_PSP_QUIT;//newsta te(ST_PSP_QUIT,0,1);// [***** �ǉ�:090103
	#if 1
	/* �v�邩�� */
	sceKernelDelayThread(1000000);/* �R�[���o�b�N����1�b�҂� */
	psp_loop = ST_PSP_QUIT;/* �������I���t���O */
	#endif
	return (0); 	/* �R�[���o�b�N���̏I�� */
}
	/* ����ȊO�̎���������܂���(���C�����œ�����������̂�) */
//	game_exit();/* ������Ƃ܂���(game_exit()���Q��ɂȂ����Ⴄ) */
//	sceKernelExitGame();/* ������Ƃ܂���(home�I�������ꍇ�A�Q�[���Z�[�u���o���Ȃ��A�I���������o���Ȃ�) */


/*---------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^(�R�[���o�b�N��o�^)
---------------------------------------------------------*/

static int callback_homekey_thread(SceSize args, void *argp)
{
	/* �V�K�ɃR�[���o�b�N���쐬 */
	int callback_id = sceKernelCreateCallback("exit callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(callback_id); 	/* [HOME]�L�[�p�̃R�[���o�b�N��o�^ */
	sceKernelSleepThreadCB();
	return (0);
}

/*---------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^(�X���b�h���쐬)
---------------------------------------------------------*/

/* Sets up the callback thread and returns its thread id */
static /*int*/void regist_home_key(void)
{
	/* �V�K�ɃX���b�h���쐬 */
	int thread_id = sceKernelCreateThread("update thread", callback_homekey_thread, 0x11, 0xFA0, 0, 0); /* �X���b�h�̗D��x����ݒ� */
	if (0 <= thread_id) 	/* �X���b�h���쐬�o������ */
	{
		sceKernelStartThread(thread_id, 0, 0);	/* �X���b�h�J�n */
	}
	//return (thread_id);
}

/*---------------------------------------------------------
	game_main()
---------------------------------------------------------*/
extern void common_load_init(void);
extern void stage_first_init(void);
extern void shooting_game_core_work(void);

extern void all_menu_init(void);
extern void all_menu_work(void);

extern void player_opt_init(void);
extern void player_opt_work(void);

extern void name_entry_init(void);
extern void name_entry_work(void);
//
extern void stage_clear_work(void);/*stage_clear.c*/
//

//tern void gameover_init(void);
extern void gameover_work(void);

//tern void result_init(void);
extern void result_work(void);

//tern void story_init(void);
extern void story_work(void);

extern void option_menu_work(void);
extern void option_menu_init(void);

//tern void key_config_init(void);
extern void key_config_work(void);
extern void music_room_work(void);


extern void vbl_draw_screen(void);/*support.c*/

static void game_main(void)
{
	while (ST_PSP_QUIT != psp_loop)
	{
		#if 0
		switch ((u8)(psp_loop>>8))
		{
		case (ST_INIT_GAME_PLAY_common>>8): common_load_init(); 		break;
		case (ST_WORK_GAME_PLAY>>8):		shooting_game_core_work();	break;
		case (ST_INIT_MENU>>8): 			all_menu_init();			break;
		case (ST_WORK_MENU>>8): 			all_menu_work();			break;
//		case (ST_INIT_PLAYER_SELECT>>8):	player_opt_init();			break;�z���B�Ȃ�
		case (ST_WORK_PLAYER_SELECT>>8):	player_opt_work();			break;
		case (ST_INIT_NAME_ENTRY>>8):		name_entry_init();			break;
		case (ST_WORK_NAME_ENTRY>>8):		name_entry_work();			break;
//
		case (ST_WORK_STAGE_FIRST>>8):		stage_first_init(); 		break;
		case (ST_WORK_STAGE_CLEAR>>8):		stage_clear_work(); 		break;
//
	//	case (ST_INIT_GAME_OVER>>8):		gameover_init();			break;�z���B�Ȃ�
		case (ST_WORK_GAME_OVER>>8):		gameover_work();			break;	/*newsta te(ST_INTRO,0,1);*/
	//	case (ST_INIT_RESULT>>8):			result_init();				break;�z���B�Ȃ�
		case (ST_WORK_RESULT>>8):			result_work();				break;
	//	case (ST_INIT_KEY_CONFIG>>8):		key_config_init();			break;�z���B�Ȃ�
		case (ST_WORK_KEY_CONFIG>>8):		key_config_work();			break;
	//	case (ST_INIT_STORY>>8):			story_init();				break;�z���B�Ȃ�
		case (ST_WORK_STORY>>8):			story_work();				break;
	//	case (ST_INTRO>>8): 				intro_init();				break;
	//	case (ST_INTRO>>8): 				intro_work();				break;
	//	case (ST_START_INTRO>>8):			startintro_init();			break;
	//	case (ST_START_INTRO>>8):			startintro_work();			break;
		}
		#else
							/*const*/static void (*aaa_call_table[/*16*/(16)])(void) =
					{
						NULL,
						common_load_init,
						shooting_game_core_work,
						all_menu_init,

						all_menu_work,
						player_opt_work,
						name_entry_init,
						name_entry_work,

						stage_first_init,
						stage_clear_work,
						gameover_work,
						result_work,

						story_work,
						option_menu_work,
						key_config_work,
						music_room_work,
					};
					{
						(aaa_call_table[(((u8)(psp_loop>>8)))])();
					}
		#endif
		vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
	}
}


/*---------------------------------------------------------
	boot_main()
---------------------------------------------------------*/

#if (1==HACK_FPU)
extern void disable_FPU_exeptions_in_main(void);	/* FPU��O�𖳌��ɂ���B disablefpu.S */
#endif

extern void game_system_init(void/*int argc, char *argv[]*/);
extern void game_system_exit(void);
extern void game_main(void);

/* ������ -Dmain=SDL_main �̏ꍇ�A�}�N���Ȃ̂Ŏ����I�� int SDL_main(int argc, char *argv[]) �ɂȂ�B�����SDL����main()����ĂԁB */
int main(int argc, char *argv[])
{
	#if (1==HACK_FPU)
	disable_FPU_exeptions_in_main();	/* ���̊֐���main()�����ɏ����Ȃ��ƃ_����������Ȃ�($31��M��̂�) */
	#endif
//	psp_loop = 1;		/* --- ���[�v�t���O(0�ŏI���B0�ȊO�͋N����) */
	regist_home_key();/* [HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^ */
	game_system_init();/* �Q�[���V�X�e�������� */
//
	scePowerSetClockFrequency(333,333,166);/* psp �� �N���b�N�� 333MHz �ɂ���� */
	game_main();
	scePowerSetClockFrequency(222,222,111);/* psp �� �N���b�N�� 222MHz �ɖ߂��� */
//
	game_system_exit();/* �Q�[���V�X�e���I�� */
//	sceKernelExitGame();
	return (0);
}
