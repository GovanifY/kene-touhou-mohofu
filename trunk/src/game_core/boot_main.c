
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

/* */
#define FIX_TEST (1)
//#define FIX_TEST (0)

//#define USE_FIX_GP	(0)
#define USE_FIX_GP	(1)


/*---------------------------------------------------------
	Makefile�� -Dmain=SDL_main �������Ă��A�����I�� -Dmain=SDL_main �𖳌��ɂ���B
---------------------------------------------------------*/

#ifdef main/* makefile �� -Dmain=SDL_main ������ꍇ */
	#undef main/* �����I�� -Dmain=SDL_main �𖳌��ɂ���B */
	/* -Dmain=SDL_main �͔�T�|�[�g�ł��B */
//	error! -Dmain=SDL_main �͔�T�|�[�g�ł��B
	/*
	 makefile �� -Dmain=SDL_main ������ꍇ�́A
	SDL���p�ӂ������C�����[�`�����g�p����̂ŁA�����ɂ�
	�����܂���B(C�����main()�֐���2�����N�o���܂���)
	SDL���p�ӂ������C�����[�`��
	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fmain%2Fpsp%2FSDL_psp_main.c
	�́A
	PSP-1000�ł�version�ɂ���ċN�������肵�Ȃ�������A
	PSP-2000�͋N�����Ȃ�������AC++���g���Ɖ��̂��N�������肵�Ȃ�������A
	�ƐF�X��肪�����̂Ŏg��Ȃ����������ł��B
	*/
#endif

/*---------------------------------------------------------
	PSP module info section
---------------------------------------------------------*/

#if (1)/* Makefile �� -Dmain=SDL_main ���Ȃ��ꍇ */
	/*
	������̏ꍇ�� PSPSDK �̗p�ӂ����N�����[�`�����g�p����ꍇ�ł��B
	�N�����[�`���͊ȒP�ȃA�Z���u���{PSP��OS�ɗ��p�֐���m�点��\��
	���L�}�N���͋N���A�Z���u���̈ꕔ�ɂɓW�J����܂����A
	�\�� PSPSDK �������쐬���Ă���܂��B
	*/

#if 0
PSP_THREAD_ATTR_VFPU			VFPU�̗��p��L���ɂ��܂��B
PSP_THREAD_ATTR_USER			���[�U���[�h�ŃX���b�h���N�����܂��B�e�X���b�h�����[�U���[�h�̏ꍇ�́A���Ɏw�肵�Ȃ��Ƃ����[�U���[�h�ŋN������܂��B
PSP_THREAD_ATTR_USBWLAN			USB/WlanAPI�Ŏg���Ă��܂��B�ʏ�A�w�肷�邱�Ƃ͂���܂���B
PSP_THREAD_ATTR_VSH 			VSHAPI�Ŏg���Ă��܂��B�ʏ�A�w�肷�邱�Ƃ͂���܂���B
PSP_THREAD_ATTR_SCRATCH_SRAM	�X�N���b�`�p�b�h�̗��p�������܂��BFW1.0�ł͎g���Ă��炸�A���Ɏw�肵�Ȃ��Ƃ����R�ɗ��p���\�ł��B
PSP_THREAD_ATTR_NO_FILLSTACK	�X���b�h�N�����ɃX�^�b�N��0xff�Ŗ��߂Ȃ��悤�Ɏw�肵�܂��B
PSP_THREAD_ATTR_CLEAR_STACK 	�X���b�h�I�����ɃX�^�b�N���[���N���A���܂��B
THREAD_ATTR_USER				PSP_THREAD_ATTR_USER�̕ʖ��ł��B
THREAD_ATTR_VFPU				PSP_THREAD_ATTR_VFPU�̕ʖ��ł��B
enum PspThreadAttributes
{
	/** Enable VFPU access for the thread. */
	PSP_THREAD_ATTR_VFPU =			0x00004000,
	/** Allow using scratchpad memory for a thread, NOT USABLE ON V1.0 */
	PSP_THREAD_ATTR_SCRATCH_SRAM =	0x00008000,
//
	/** Disables filling the stack with 0xff on creation */
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

#define KENE_NAME_STR			"KENE"

#if (1)/* Makefile ���Ԉ���ĂȂ���Ζ{���v��Ȃ����A�R���p�C���ł��Ȃ����ւ肪�͂��Ɩʓ|�Ȃ̂ŁB */
	#ifndef KENE_RELEASE_VERSION
		#define KENE_RELEASE_VERSION	(36)
	#endif
	#ifndef KENE_UPDATE_VERSION
		#define KENE_UPDATE_VERSION 	(1)
	#endif
#endif

PSP_MODULE_INFO(
	KENE_NAME_STR,				/* name:			PSP��OS�ɋ����Ă����閼�O�B */
	0x0000, 					/* attributes:		���[�U�[���[�h�ŋN������B */
	KENE_RELEASE_VERSION,		/* major_version:	���̃A�v���̃o�[�W�����i���o�[ version 35.-- */
	KENE_UPDATE_VERSION);		/* minor_version:	���̃A�v���̃o�[�W�����i���o�[ version --.01 */

PSP_MAIN_THREAD_ATTR(						/* ���̃v���O�������g��OS�����B*/
	PSP_THREAD_ATTR_VFPU |					/* VFPU���g�� */
//	PSP_THREAD_ATTR_SCRATCH_SRAM |			/* SCRATCH_SRAM �g��(�w�肵�Ȃ��ق����]�܂���) */
	/*	SCRATCH_SRAM �͎w�肵�Ȃ��Ă��f�t�H���g�Ŏg�p�\�ł���B
		FW ver 1.00�ɂ͖����̂Ō݊������l������Ȃ疳���č\��Ȃ� */
//	PSP_THREAD_ATTR_NO_FILLSTACK |			/* �J�n���ɃX�^�b�N�� 0xff �Ŗ��߂����Ȃ��B */
	/* 0xff�Ŗ��߂Ă��ꂽ�����A�o�O�o���ɂ͓s�����ǂ��̂ł��̃I�v�V�����͎w�肵�Ȃ��B */
	PSP_THREAD_ATTR_CLEAR_STACK |
	/* �I�����ɂ��̃v���O�������g�����X�^�b�N(�ϐ��̈�)��OS�ɏ����Ă��炤�B */
	THREAD_ATTR_USER |						/* ���[�U�[���[�h�ŋN������B(kernel�n�R�[�����g��Ȃ��B) */
//	PSP_THREAD_ATTR_USBWLAN |				/* USB WLAN API�g��Ȃ� */
//	PSP_THREAD_ATTR_VSH |					/* VSH API�g��Ȃ�(dark-alex���łȂ��̂� VSH�g���܂��񂪁A����) */
	0);
	/* http://dxlibp.sourceforge.jp/cgi-bin/patio/read.cgi?no=857 */
	//	PSP_HEAP_SIZE_KB(16*1024); // �V�K�ǉ�
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); // �V�K�ǉ�


	#if (0==KENE_SYOMEI_OFW)
	// 0 == fw1.00�Ή���
	// �񏐖���
		PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (�v���O�������g�p����ϐ���)�X�^�b�N�̈�̃T�C�Y */
												/* �������傫����malloc�ł���ʂ����̕����� */
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(48);		/* (r35) */
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); 	/* (r35) */
	#endif
	//
	#if (1==KENE_SYOMEI_OFW)
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
#endif

/*
	�����N���ɂ��Ă̌����B

	[ �͕핗�̃������z�u ](�{���͂���������ƕ��G���Ǝv�����Ǌȗ������Ă���B���Ă������ǂ��m��Ȃ��̂őz���������Ă�)
0000+------------------------+
	| �X�N���b�`�p�b�hSRAM(?)| 2kBytes�������B(�͕핗�ł͎g�p���Ă��Ȃ�)
	+------------------------+
	|	 �n�r�Ƃ�BIOS�Ƃ�	 |(�J�[�l���������B�͕핗�̓��[�U�[���[�h�N���Ȃ̂ŃA�N�Z�X�ł��Ȃ��B
	|	(8MBytes���炢�H)	 |����o�O�Ŗ͕핗���A�N�Z�X�����ꍇ�ɂ́Apsp�̓d�����؂��̂����퓮��)
	+------------------------+
	|	���̃v���O����		 |(���̃v���O�C���Ƃ�)
	+------------------------+
	|	�͕핗�̃v���O����	 | (EBOOT.PBP�̑傫���� 1.5MB���炢�����ǁA
	|(�ʏ��1MB���炢�Ȃ̂���| �͕핗�̃v���O�����͑S����1.0MB���Ȃ��B
	|�����ł�6MBytes���炢(?)| EBOOT.PBP��0.5MB�ȏ��XMB�ł����g�p���Ȃ�
	|		�ɖc��オ��)	 | �f�[�^�[�Ȃ̂Ŏ��s����psp�̃������Ƀ��[�h����Ȃ�)
	+------------------------+
	| �͕핗�� const �f�[�^�[|(�q�[�v�̈���������ݒ肵���ꍇ�Aconst �f�[�^�[���A�q�[�v�Ƀf�[�^�[��u�����������s���x��������������B
	|						 |���R�� GCC�ł� �֐����Ăԏꍇ�ɕK���X�^�b�N���g���̂ŁA�q�[�v�̈悪�������ƃA�h���X���t���b�V�����Ȃ��Ȃ�
	|						 |�L���b�V�������ɂ������炾�Ǝv��)
	+------------------------+
	|						 |(HOME�R�[���o�b�N�̃X���b�h�Ƃ����Ƃ�����������B���Ԃ�)
	|	   �󂫃�����		 |
	|						 |
	+------------------------+
	|	   �q�[�v�̈�		 |(malloc�Ƃ��̃������͂�������m�ۂ���B�͕핗�Łu���C���������v���ČĂ�ł�̂͂����̎�)
	|		  ��			 |
	|						 |�� �q�[�v�̈�̑傫���̌��E��ݒ�ł���B(PSP_HEAP_SIZE_KB();)
	|	   �󂫃�����		 |
	|						 |�� �X�^�b�N�̈�̑傫���̌��E��ݒ�ł���B(PSP_MAIN_THREAD_STACK_SIZE_KB();)
	|		  ��			 |
	|	  �X�^�b�N�̈�		 |(mips�ł͊֐����Ăԏꍇ�ɃX�^�b�N�������B)
	+------------------------+(psp-1000��16MBytes, psp-2000�ȍ~��32MBytes)
	�������ς�Ŗ�������
	+------------------------+ 0x04000000 (�ʏ�0x44000000�Ə����������A�h���X�B��ʃr�b�g�̓A�h���X�łȂ��ăL���b�V���̗L��)
	| VRAM�Ƃ��n�[�h�E�F�A�[ |(�͕핗�ŁuVRAM�v���ČĂ�ł�̂͂����̎��B)
ffff+------------------------+

	�����N������ׂɃv���O�������傫���Ȃ�K�v������B(1.0MB�ȉ� �� 6.0MB�ȏ��)
	//
	�����łł̓v���O�������傫���Ȃ�Ɓu�q�[�v�̈�v������Ȃ��Ȃ�B(���Ԃ�)
	//
	psp�ł̓q�[�v�̈�̌��E��ݒ�ł���̂ŁA
	�u�󂫃������v���܂�����̂Ɂu�q�[�v�̈�v������Ȃ��Ȃ�B
	�Ƃ�����ԂɂȂ�N�����Ȃ��B
//
	���̖�����������ɂ́u�q�[�v�̈�v��傫�߂ɐݒ肷��Ηǂ��B
	�����ǁu�q�[�v�̈�v��傫����������Ɓu�X�^�b�N�̈�v��j�󂵂āA
	�v���O�����������Ȃ��댯������B
	�Ǝv����̂Œ��X�ɂ��Ƃ��B
//
	����������ŁA12MBytes �ɂȂ��Ă�B
	���Ԃ񂱂̃T�C�Y�́u�q�[�v�̈�v�̃T�C�Y�ł͂Ȃ��āA
	12MBytes == �u�����Ńv���O���� + ������const �f�[�^�[ + �q�[�v�̈�v�̃T�C�Y�B
	���Ǝv����B
*/

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
	cb.main_call_func = NULL;	/* �I���w�� */
	#if 1
	/* �{�� cb.main_call_func �� �Z�}�t�H�Ǘ����ׂ������BsceKernelCreateSema()?? �����ǂ��ĂȂ��B */
	/* �v�邩�� */
	sceKernelDelayThread(1000000);/* �R�[���o�b�N����1�b�҂� */
	cb.main_call_func = NULL;	/* �������I���w�� */
	#endif
	return (0); 	/* �R�[���o�b�N���̏I�� */
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
extern void ini_save(void);
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
	ini_save();
	sceKernelExitGame();
	#else
	game_system_exit(); 	/* �Q�[���V�X�e���I��(�����ŏI������) */
	#endif
//	sceKernelExitGame();
	return (0);/* �_�~�[(�����ɂ͐�΂ɗ��Ȃ�) */
}
