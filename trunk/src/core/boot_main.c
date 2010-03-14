
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
#if 0
PSP_THREAD_ATTR_VFPU VFPU�̗��p��L���ɂ��܂�
PSP_THREAD_ATTR_USER ���[�U���[�h�ŃX���b�h���N�����܂��B�e�X���b�h�����[�U���[�h�̏ꍇ�́A���Ɏw�肵�Ȃ��Ƃ����[�U���[�h�ŋN������܂��B
SP_THREAD_ATTR_USBWLAN USB/WlanAPI�Ŏg���Ă��܂��B�ʏ�A�w�肷�邱�Ƃ͂���܂���
PSP_THREAD_ATTR_VSH VSHAPI�Ŏg���Ă��܂��B�ʏ�A�w�肷�邱�Ƃ͂���܂���B
PSP_THREAD_ATTR_SCRATCH_SRAM �X�N���b�`�p�b�h�̗��p�������܂��BFW1.0�ł͎g���Ă��炸�A���Ɏw�肵�Ȃ��Ƃ����R�ɗ��p���\�ł��B
PSP_THREAD_ATTR_NO_FILLSTACK �X���b�h�N�����ɃX�^�b�N��0xFF�Ŗ��߂Ȃ��悤�Ɏw�肵�܂��B
PSP_THREAD_ATTR_CLEAR_STACK �X���b�h�I�����ɃX�^�b�N���[���N���A���܂��B
THREAD_ATTR_USER PSP_THREAD_ATTR_USER�̕ʖ��ł�
THREAD_ATTR_VFPU PSP_THREAD_ATTR_VFPU�̕ʖ��ł�
enum PspThreadAttributes
{
	/** Enable VFPU access for the thread. */
	PSP_THREAD_ATTR_VFPU =			0x00004000,
	/** Allow using scratchpad memory for a thread, NOT USABLE ON V1.0 */
	PSP_THREAD_ATTR_SCRATCH_SRAM =	0x00008000,
//
	/** Disables filling the stack with 0xFF on creation */
	PSP_THREAD_ATTR_NO_FILLSTACK =	0x00100000,
	/** Clear the stack when the thread is deleted */
	PSP_THREAD_ATTR_CLEAR_STACK =	0x00200000,
//
	/** Start the thread in user mode (done automatically
	  if the thread creating it is in user mode). */
	PSP_THREAD_ATTR_USER =			0x80000000,
	/** Thread is part of the USB/WLAN API. */
	PSP_THREAD_ATTR_USBWLAN =		0xa0000000,
	/** Thread is part of the VSH API. */
	PSP_THREAD_ATTR_VSH =			0xc0000000,
};
#endif

//PSP_MODULE_INFO("Cube Sample", 0x0000, 1, 1);
PSP_MODULE_INFO("KENE", 0x0000, 0, 4);	/* PSP��OS�ɋ����Ă����閼�O�B���[�U�[���[�h�ŋN������B */
PSP_MAIN_THREAD_ATTR(			/* ���̃v���O�������g��OS�����B*/
	PSP_THREAD_ATTR_VFPU |			/* VFPU���g�� */
//	PSP_THREAD_ATTR_SCRATCH_SRAM |	/* SCRATCH_SRAM �g��(�w�肵�Ȃ��ق����]�܂���) */
	/*	SCRATCH_SRAM �͎w�肵�Ȃ��Ă��f�t�H���g�Ŏg�p�\�ł���B
		FW ver 1.00�ɂ͖����̂Ō݊������l������Ȃ疳���č\��Ȃ� */
//	PSP_THREAD_ATTR_NO_FILLSTACK |	/* �J�n���ɃX�^�b�N�� 0xFF �Ŗ��߂����Ȃ��B */
	/* 0xff�Ŗ��߂Ă��ꂽ�����A�o�O�o���ɂ͓s�����ǂ��̂ł��̃I�v�V�����͎w�肵�Ȃ��B */
	PSP_THREAD_ATTR_CLEAR_STACK |
	/* �I�����ɂ��̃v���O�������g�����X�^�b�N(�ϐ��̈�)��OS�ɏ����Ă��炤�B */
	THREAD_ATTR_USER |			/* ���[�U�[���[�h�ŋN������B(kernel�n�R�[�����g��Ȃ��B) */
//	PSP_THREAD_ATTR_USBWLAN |	/* USB WLAN API�g��Ȃ� */
//	PSP_THREAD_ATTR_VSH |		/* VSH API�g��Ȃ�(dark-alex���łȂ��̂� VSH�g���܂��񂪁A����) */
	0);

PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (�v���O�������g�p����ϐ���)�X�^�b�N�̈�̃T�C�Y */
										/* �������傫����malloc�ł���ʂ����̕����� */
/* 2008�N���炢�̌Â� PSPSDK �� -1(����) �ɑΉ����Ă��Ȃ��̂� -1 �ɂ���ƃn���O�A�b�v���� */
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
	psp_loop = ST_PSP_QUIT; 	// [***** �ǉ�:090103
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

//extern void name_entry_init(void);
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
		/*const*/static void (*aaa_call_table[/*16*/(16)])(void) =
		{
			NULL,						/* ST_PSP_QUIT					= 0x0000, psp�I�� */
			common_load_init,			/* ST_INIT_GAME_PLAY_common 	= 0x0100, */
			shooting_game_core_work,	/* ST_WORK_GAME_PLAY */
			all_menu_init,				/* ST_INIT_MENU */

			all_menu_work,				/* ST_WORK_MENU */
			player_opt_work,			/* ST_WORK_PLAYER_SELECT */
			story_work,   				/* <<���݊J��>> */		/* ST_INIT_NAME_ENTRY */
			name_entry_work,			/* ST_WORK_NAME_ENTRY */

			stage_first_init,			/* ST_WORK_STAGE_FIRST */
			stage_clear_work,			/* ST_WORK_STAGE_CLEAR */
			gameover_work,				/* ST_WORK_GAME_OVER */
			result_work,				/* ST_WORK_RESULT */

			story_work, 				/* ST_WORK_STORY */
			option_menu_work,			/* ST_WORK_OPTION_MENU */
			key_config_work,			/* ST_WORK_KEY_CONFIG */
			music_room_work,			/* ST_WORK_MUSIC_ROOM */
		};
		/* ���s */
		{
			(aaa_call_table[(((u8)(psp_loop>>8)))])();
		}
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
