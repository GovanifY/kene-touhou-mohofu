
/*---------------------------------------------------------
	psp�̋N�����[�`����
---------------------------------------------------------*/

#include "support.h"

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

PSP_MODULE_INFO("KENE", 0x0000, 0, 4);	/* PSP��OS�ɋ����Ă����閼�O�Ƃ��X���b�h�̗D��x�Ƃ����� */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER); /* ���[�U�[���[�h�ŋN������� */
PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (�v���O�������g�p����ϐ���)�X�^�b�N�̈�̃T�C�Y���� */
										/* �������傫����malloc�ł���ʂ����̕������ */
/* 2008�N���炢�̌Â� PSPSDK �� -1(����) �ɑΉ����Ă��Ȃ��̂� -1 �ɂ���ƃn���O�A�b�v����� */
#endif

int psp_loop;

/*---------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^
	([HOME]�L�[���j���[�ŁA�I�����܂����H�u�͂��v���I�������Ǝ��s����镔��)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* �R�[���o�b�N������A���C�����̏�Ԃ𑀍삷�� */
	psp_loop = ST_PSP_QUIT;//newsta te(ST_PSP_QUIT,0,1);// [***** �ǉ�:090103
	return (0); 	/* �R�[���o�b�N���̏I�� */
}
	/* ����ȊO�̎���������܂���(���C�����œ�����������̂�) */
//	game_exit();/*������Ƃ܂���*/
//	sceKernelExitGame();/*������Ƃ܂���*/


/*---------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^(�R�[���o�b�N��o�^)
---------------------------------------------------------*/

static int callback_homekey_thread(SceSize args, void *argp)
{
	/* �V�K�ɃR�[���o�b�N���쐬 */
	int callback_id;
	callback_id = sceKernelCreateCallback("exit callback", exit_callback, NULL);
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
	int thread_id/* = 0*/;
	thread_id = sceKernelCreateThread("update thread", callback_homekey_thread, 0x11, 0xFA0, 0, 0); /* �X���b�h�̗D��x����ݒ� */
	if (thread_id >= 0) 	/* �X���b�h���쐬�o������ */
	{
		sceKernelStartThread(thread_id, 0, 0);	/* �X���b�h�J�n */
	}
	//return thread_id;
}

/*---------------------------------------------------------
	�w�b�_
---------------------------------------------------------*/

extern void shooting_game_init(void);
extern void shooting_game_work(void);

extern void all_menu_init(void);
extern void all_menu_work(void);

extern void player_opt_init(void);
extern void player_opt_work(void);

extern void name_entry_init(void);
extern void name_entry_work(void);
//
extern void stage_clear_work(void);
//

//tern void gameover_init(void);
extern void gameover_work(void);

//tern void result_init(void);
extern void result_work(void);

//tern void key_config_init(void);
extern void key_config_work(void);

//tern void story_init(void);
extern void story_work(void);

extern void game_system_init(void/*int argc, char *argv[]*/);
extern void game_system_exit(void);

extern void vbl_draw_screen(void);/*support.c*/
/* ������ -Dmain=SDL_main �̏ꍇ�A�}�N���Ȃ̂Ŏ����I�� int SDL_main(int argc, char *argv[]) �ɂȂ�B�����SDL����main()����ĂԁB */
int main(int argc, char *argv[])
{
//	psp_loop = 1;		/* --- ���[�v�t���O(0�ŏI���B0�ȊO�͋N����) */
	regist_home_key();/* [HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^ */
	game_system_init();/* �Q�[���V�X�e�������� */
//
	scePowerSetClockFrequency(333,333,166);/* psp �� �N���b�N�� 333MHz �ɂ���� */
	while (ST_PSP_QUIT != psp_loop)
	{
		switch ((Uint8)(psp_loop>>8))
		{
		case (ST_INIT_GAME_PLAY>>8):		shooting_game_init();	break;
		case (ST_WORK_GAME_PLAY>>8):		shooting_game_work();	break;
		case (ST_INIT_MENU>>8): 			all_menu_init();		break;
		case (ST_WORK_MENU>>8): 			all_menu_work();		break;
		case (ST_INIT_PLAYER_SELECT>>8):	player_opt_init();		break;
		case (ST_WORK_PLAYER_SELECT>>8):	player_opt_work();		break;
		case (ST_INIT_NAME_ENTRY>>8):		name_entry_init();		break;
		case (ST_WORK_NAME_ENTRY>>8):		name_entry_work();		break;
//
		case (ST_WORK_STAGE_CLEAR>>8):		stage_clear_work(); 	break;
//
	//	case (ST_INIT_GAME_OVER>>8):		gameover_init();		break;�z���B�Ȃ�
		case (ST_WORK_GAME_OVER>>8):		gameover_work();		break;	/*newsta te(ST_INTRO,0,1);*/
	//	case (ST_INIT_RESULT>>8):			result_init();			break;�z���B�Ȃ�
		case (ST_WORK_RESULT>>8):			result_work();			break;
	//	case (ST_INIT_KEY_CONFIG>>8):		key_config_init();		break;�z���B�Ȃ�
		case (ST_WORK_KEY_CONFIG>>8):		key_config_work();		break;
	//	case (ST_INIT_STORY>>8):			story_init();			break;�z���B�Ȃ�
		case (ST_WORK_STORY>>8):			story_work();			break;
	//	case ST_INTRO:						intro_init();			break;
	//	case ST_INTRO:						intro_work();			break;
	//	case ST_START_INTRO:				startintro_init();		break;
	//	case ST_START_INTRO:				startintro_work();		break;
		}
		vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
	}
	scePowerSetClockFrequency(222,222,111);/* psp �� �N���b�N�� 222MHz �ɖ߂��� */
//
	game_system_exit();/* �Q�[���V�X�e���I�� */
//	sceKernelExitGame();
	return (0);
}
