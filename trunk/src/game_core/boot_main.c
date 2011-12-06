
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W				http://code.google.com/p/kene-touhou-mohofu/
	�\�[�X�R�[�h�\��				http://code.google.com/p/kene-touhou-mohofu/source/browse/trunk/
	�\�[�X�t�@�C���P�̃_�E�����[�h	http://kene-touhou-mohofu.googlecode.com/svn/trunk/
	�_�E�����[�h��					http://kene-touhou-mohofu.googlecode.com/files/231_kene_r35-r37.zip
	231�̊J���񍐃y�[�W 			http://mohou.huuryuu.com/
	-------------------------------------------------------
	psp�̋N�����[�`����
---------------------------------------------------------*/

#define FIX_TEST (1)
//#define FIX_TEST (0)

//#define USE_FIX_GP	(0)
#define USE_FIX_GP	(1)

/*---------------------------------------------------------
	PSP module info section.
---------------------------------------------------------*/

#define APP_NAME_STR			"KENE"

#if (1)/* Makefile ���Ԉ���ĂȂ���Ζ{���v��Ȃ����A�R���p�C���ł��Ȃ����ւ肪�͂��Ɩʓ|�Ȃ̂ŁB */
	#ifndef APP_RELEASE_VERSION
		#define APP_RELEASE_VERSION 	(39)
	#endif
	#ifndef APP_UPDATE_VERSION
		#define APP_UPDATE_VERSION		(1)
	#endif
#endif

PSP_MODULE_INFO(APP_NAME_STR, PSP_MODULE_USER, APP_RELEASE_VERSION, APP_UPDATE_VERSION);

PSP_MAIN_THREAD_ATTR(						/*(���̃v���O�������g��OS�����B)*/
	PSP_THREAD_ATTR_VFPU |					/*(VFPU���g��)*/
//	PSP_THREAD_ATTR_SCRATCH_SRAM |			/*(SCRATCH_SRAM �g��(�w�肵�Ȃ��Ă��g�p�\�B)*/
	/*(FW ver 1.00�ɂ͖����̂Ō݊������l������Ȃ疳���č\��Ȃ�)*/
//	PSP_THREAD_ATTR_NO_FILLSTACK |			/*(�J�n���ɃX�^�b�N�� 0xff �Ŗ��߂����Ȃ��B)*/
	/*(0xff�Ŗ��߂Ă��ꂽ�����A�o�O�o���ɂ͓s�����ǂ��̂ł��̃I�v�V�����͎w�肵�Ȃ��B)*/
	PSP_THREAD_ATTR_CLEAR_STACK |
	/*(�I�����ɂ��̃v���O�������g�����X�^�b�N(�ϐ��̈�)��OS�ɏ����Ă��炤�B)*/
	THREAD_ATTR_USER |						/*(���[�U�[���[�h�ŋN������B(kernel�n�R�[�����g��Ȃ��B))*/
//	PSP_THREAD_ATTR_USBWLAN |				/*(USB WLAN API�g��Ȃ�)*/
//	PSP_THREAD_ATTR_VSH |					/*(VSH API�g��Ȃ�)*/
	0);
	/* http://dxlibp.sourceforge.jp/cgi-bin/patio/read.cgi?no=857 */
	//	PSP_HEAP_SIZE_KB(16*1024); // �V�K�ǉ�
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); // �V�K�ǉ�

	#if (0==APP_SYOMEI_OFW)
	// 0 == fw1.00�Ή���
	// �񏐖���
		PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (�v���O�������g�p����ϐ���)�X�^�b�N�̈�̃T�C�Y */
												/* �������傫����malloc�ł���ʂ����̕����� */
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(48);		/* (r35) */
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); 	/* (r35) */
	#endif
	//
	#if (1==APP_SYOMEI_OFW)
	// 0 == ofw�Ή���
	// ������
//	PSP_HEAP_SIZE_KB(4*1024); // fatal error: load0 cant load image data/bg/title_bg.png
	PSP_HEAP_SIZE_KB(12*1024); // (12MBytes, oslib) (r38)MusicRoom�Ńn���O�A�b�v����B
//	PSP_HEAP_SIZE_KB(14*1024); // (14MBytes)
//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); // (r38)MusicRoom�Ńn���O�A�b�v����B(512k[bytes]==0.5M[bytes]�͑傫������B)
//	PSP_MAIN_THREAD_STACK_SIZE_KB(256); // (r38)minimalist, MusicRoom�Ńn���O�A�b�v����Bcygwin�Ȃ� ok!
	PSP_MAIN_THREAD_STACK_SIZE_KB(128); // (r38) ����128k[bytes]������Ώ\���Ȃ񂶂�Ȃ�����?
//	PSP_MAIN_THREAD_STACK_SIZE_KB(64); //
	#endif

	// �q�[�v12MB�̏ꍇ
	// FW1.00			�N���Ɏ��s���܂���(80020148)
	// CFW5.00M33-6

/* 2008�N���炢�̌Â� PSPSDK �� -1(����) �ɑΉ����Ă��Ȃ��̂� -1 �ɂ���ƃn���O�A�b�v���� */


/*---------------------------------------------------------
	GP���W�X�^�̒l���`�F�b�N���A�ُ�ȏꍇ�͕ύX����
	-------------------------------------------------------
	HBL��version�ɂ���ẮA�N������ GP���W�X�^�Ɉُ�l�����鎖��
	����炵���̂ł��̑΍�B(Ruka��)
	-------------------------------------------------------
	GP���W�X�^(GP����16�r�b�g���W�X�^)(GPREL16, GP rerative 16 bit register)�Ƃ����̂�
	MIPS��CPU�������Ă��郌�W�X�^�ŁA
	+32k[bytes]�`-32k[bytes]�͈̔�(64k[bytes]�͈̔�)�ŁA���΃A�N�Z�X����ׂ̃��W�X�^�B
---------------------------------------------------------*/
#if (1==FIX_TEST)
	#if (1==USE_FIX_GP)
static void FixedGP(void)
{
	void* my_gp_value;
	/* GP���W�X�^�̒l���`�F�b�N */
	asm volatile ("move %0, $gp\n" : "=r" (my_gp_value));
	if (module_info.gp_value != my_gp_value)
	{
		/* GP���W�X�^�̒l���ُ�ȏꍇ�͏C�� */
		asm volatile ("move $gp, %0\n" :: "r" (module_info.gp_value));
	}
}
	#endif /*(1==USE_FIX_GP)*/
#endif /*(1==FIX_TEST)*/


global GAME_CORE_GLOBAL_CALLBACK cb;
/*---------------------------------------------------------
	[HOME]�L�[�ŏI����������
	-------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^
	([HOME]�L�[���j���[�ŁA�I�����܂����H�u�͂��v���I�������Ǝ��s����镔��)
	-------------------------------------------------------
	�R�[���o�b�N���ŃQ�[���̕ۑ��������Ȃǂ̏��������Ă͂����Ȃ��B
	���C�����ł����̏����͕K���s���̂ŁA
	�����ł����̏���������ƁA���ʓI�ɂQ�񏈗����s�����ɂȂ�B
	(���C�����͂Q�񏈗����s���Ă���薳���悤�ɍ���ĂȂ�)
	-------------------------------------------------------
	����ȊO�̎���������܂���(���C�����œ�����������̂�)
	game_exit();������Ƃ܂���(game_exit()���Q��ɂȂ����Ⴄ)
	sceKernelExitGame();������Ƃ܂���(home�I�������ꍇ�A�Q�[���Z�[�u���o���Ȃ��A�I���������o���Ȃ�)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* �R�[���o�b�N������A���C�����̏�Ԃ𑀍삷�� */
	cb.main_call_func = NULL;/*(�A�v���I���w��)*/
	#if (1)/*(�v�邩��)*/
	/* �{�� cb.main_call_func �� �Z�}�t�H�Ǘ����ׂ������BsceKernelCreateSema()?? �����ǂ��ĂȂ��B */
	sceKernelDelayThread(1000000);/*(�R�[���o�b�N����1�b�҂�)*/
	cb.main_call_func = NULL;/*(�������I���w��)*/
	#endif
	return (0);/*(�R�[���o�b�N���̏I��)*/
}


/*---------------------------------------------------------
	[HOME]�L�[�p�A�R�[���o�b�N�o�^
	-------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂ɃR�[���o�b�N��o�^���܂��B
---------------------------------------------------------*/

/* Thread to create the callbacks and then begin polling */
static int CallbackThread(SceSize args, void *argp)
{
	/* �V�K�ɃR�[���o�b�N���쐬 */
	int callback_id = sceKernelCreateCallback("Exit Callback", ( int (*)(int, int, void*) )exit_callback, NULL);
	sceKernelRegisterExitCallback(callback_id); 	/* [HOME]�L�[�p�̃R�[���o�b�N��o�^ */
	sceKernelSleepThreadCB();
	return (0);
}


/*---------------------------------------------------------
	[HOME]�L�[�p�A�X���b�h�쐬
	-------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃X���b�h���쐬���܂��B
---------------------------------------------------------*/

/* Sets up the callback thread and returns its thread id */
static /*int*/void SetupCallbacks(void)
{
	/* �V�K�ɃX���b�h���쐬 */
	SceUID/*int*/ thread_id;
	thread_id = 0;/* �����ꐳ��ɍ쐬�ł��Ȃ��ꍇ���l���� 0 �������B */
	thread_id = sceKernelCreateThread("Update Thread", CallbackThread, 0x11, 0xFA0, 0, 0); /* �X���b�h�̗D��x����ݒ� */
	if (0 <= thread_id) 	/* �X���b�h���쐬�o������ */
	{
		sceKernelStartThread(thread_id, 0, 0);	/* �X���b�h�J�n */
	}
//	return (thread_id);
}


/*---------------------------------------------------------
	game_main()
---------------------------------------------------------*/

extern void vbl_draw_screen(void);/*support.c*/

static void game_main(void)
{
	{
my_loop:
		if (NULL==cb.main_call_func)
		{
			return; 	/* �I������B */
		}
		{
			cb.main_call_func();	/* �e���������s */
		}
		vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
		goto my_loop;
	}
}


/*---------------------------------------------------------
	boot_main()
---------------------------------------------------------*/
// CFLAGS += -DHACK_FPU=1 �́Asrc/alegrex/alegrex.mak �Ŏw�肵�Ă��܂��B
#if (1==HACK_FPU)
extern void disable_FPU_exeptions_in_main(void);	/* FPU��O�𖳌��ɂ���B disablefpu.S */
#endif
#if 1/*(�Ă���)*/
extern void ini_file_save(void);
#endif

extern void game_system_init(void); 	/* �Q�[���V�X�e�������� */
extern void game_system_exit(void); 	/* �Q�[���V�X�e���I�� */
extern void game_main(void);

/* ������ -Dmain=SDL_main �̏ꍇ�A�}�N���Ȃ̂Ŏ����I�� int SDL_main(int argc, char *argv[]) �ɂȂ�B�����SDL����main()����ĂԁB */
/*
	-Dmain=SDL_main �͋����I�ɖ����ɂȂ��Ă���̂ŁA(PSPSDK�̃��[�_�[���)��������N������B
	�͕핗�ł� atexit �̓t�b�N����Ă��Ȃ�����z�肵�Ă���̂ŁA exit(); �͎g��Ȃ��B
	�͕핗�ł� sceKernelExitGame(); �ŏI������B
*/

/*---------------------------------------------------------
	Makefile�� -Dmain=SDL_main �������Ă��A�����I�� -Dmain=SDL_main �𖳌��ɂ���B
---------------------------------------------------------*/
#ifdef main/* makefile �� -Dmain=SDL_main ������ꍇ */
	#undef main/* �����I�� -Dmain=SDL_main �𖳌��ɂ���B */
	/* -Dmain=SDL_main �͔�T�|�[�g�ł��B */
//	error! -Dmain=SDL_main �͔�T�|�[�g�ł��B
#endif
global int main(int argc, char *argv[])
{
#if (1==FIX_TEST)
	#if (1==USE_FIX_GP)
	FixedGP();	/* �N������ GP���W�X�^���ُ�l�ł���΁A�C������B */
	#endif
	#if (1==HACK_FPU)
	/* fpu ��O�����������ꍇ�̊��荞�ݏ������Apsp�̃n�[�h�E�F�A�[���x���Ŗ����ɂ���B */
	disable_FPU_exeptions_in_main();	/* ���̊֐���main()�����ɏ����Ȃ��ƃ_����������Ȃ�($31��M��̂�) */
	#endif
#endif
	SetupCallbacks();		/* regist_home_key [HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^ */
	game_system_init(); 	/* �Q�[���V�X�e�������� */
//
	scePowerSetClockFrequency(333,333,166);/* psp �� �N���b�N�� 333MHz �ɂ���� */
	game_main();
	scePowerSetClockFrequency(222,222,111);/* psp �� �N���b�N�� 222MHz �ɖ߂��� */
	/*(�Ă���)*/
	#if (1)/*(r34�A���̂��I���I���ł��Ȃ��̂łƂ肠����)*/
	ini_file_save();
	sceKernelExitGame();
	#else
	game_system_exit(); 	/* �Q�[���V�X�e���I��(�����ŏI������) */
	#endif
//	sceKernelExitGame();
	return (0);/* �_�~�[(�����ɂ͐�΂ɗ��Ȃ�) */
}
