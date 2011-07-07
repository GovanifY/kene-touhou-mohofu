
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	psp�̋N�����[�`����
---------------------------------------------------------*/

#if 0
	/* makefile �� -Dmain=SDL_main ������ꍇ */
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
#else
	/* makefile �� -Dmain=SDL_main ���Ȃ��ꍇ */
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
PSP_THREAD_ATTR_VFPU			VFPU�̗��p��L���ɂ��܂��B
PSP_THREAD_ATTR_USER			���[�U���[�h�ŃX���b�h���N�����܂��B�e�X���b�h�����[�U���[�h�̏ꍇ�́A���Ɏw�肵�Ȃ��Ƃ����[�U���[�h�ŋN������܂��B
SP_THREAD_ATTR_USBWLAN			USB/WlanAPI�Ŏg���Ă��܂��B�ʏ�A�w�肷�邱�Ƃ͂���܂���B
PSP_THREAD_ATTR_VSH 			VSHAPI�Ŏg���Ă��܂��B�ʏ�A�w�肷�邱�Ƃ͂���܂���B
PSP_THREAD_ATTR_SCRATCH_SRAM	�X�N���b�`�p�b�h�̗��p�������܂��BFW1.0�ł͎g���Ă��炸�A���Ɏw�肵�Ȃ��Ƃ����R�ɗ��p���\�ł��B
PSP_THREAD_ATTR_NO_FILLSTACK	�X���b�h�N�����ɃX�^�b�N��0xFF�Ŗ��߂Ȃ��悤�Ɏw�肵�܂��B
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
PSP_MODULE_INFO("KENE", 0x0000, 0, 4);		/* PSP��OS�ɋ����Ă����閼�O�B���[�U�[���[�h�ŋN������B */
PSP_MAIN_THREAD_ATTR(						/* ���̃v���O�������g��OS�����B*/
	PSP_THREAD_ATTR_VFPU |					/* VFPU���g�� */
//	PSP_THREAD_ATTR_SCRATCH_SRAM |			/* SCRATCH_SRAM �g��(�w�肵�Ȃ��ق����]�܂���) */
	/*	SCRATCH_SRAM �͎w�肵�Ȃ��Ă��f�t�H���g�Ŏg�p�\�ł���B
		FW ver 1.00�ɂ͖����̂Ō݊������l������Ȃ疳���č\��Ȃ� */
//	PSP_THREAD_ATTR_NO_FILLSTACK |			/* �J�n���ɃX�^�b�N�� 0xFF �Ŗ��߂����Ȃ��B */
	/* 0xff�Ŗ��߂Ă��ꂽ�����A�o�O�o���ɂ͓s�����ǂ��̂ł��̃I�v�V�����͎w�肵�Ȃ��B */
	PSP_THREAD_ATTR_CLEAR_STACK |
	/* �I�����ɂ��̃v���O�������g�����X�^�b�N(�ϐ��̈�)��OS�ɏ����Ă��炤�B */
	THREAD_ATTR_USER |						/* ���[�U�[���[�h�ŋN������B(kernel�n�R�[�����g��Ȃ��B) */
//	PSP_THREAD_ATTR_USBWLAN |				/* USB WLAN API�g��Ȃ� */
//	PSP_THREAD_ATTR_VSH |					/* VSH API�g��Ȃ�(dark-alex���łȂ��̂� VSH�g���܂��񂪁A����) */
	0);

PSP_MAIN_THREAD_STACK_SIZE_KB(32);			/* (�v���O�������g�p����ϐ���)�X�^�b�N�̈�̃T�C�Y */
											/* �������傫����malloc�ł���ʂ����̕����� */
/* 2008�N���炢�̌Â� PSPSDK �� -1(����) �ɑΉ����Ă��Ȃ��̂� -1 �ɂ���ƃn���O�A�b�v���� */
#endif

global void (*main_call_func)(void);
global void (*return_call_func)(void);

//#define USE_FIX_GP	(0)
#define USE_FIX_GP	(1)
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
	#endif


/*---------------------------------------------------------
	[HOME]�L�[�ŏI����������
	-------------------------------------------------------
	[HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^
	([HOME]�L�[���j���[�ŁA�I�����܂����H�u�͂��v���I�������Ǝ��s����镔��)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* �R�[���o�b�N������A���C�����̏�Ԃ𑀍삷�� */
	main_call_func = NULL;	/* �I���w�� */
	#if 1
	/* �{�� main_call_func �� �Z�}�t�H�Ǘ����ׂ������BsceKernelCreateSema()?? �����ǂ��ĂȂ��B */
	/* �v�邩�� */
	sceKernelDelayThread(1000000);/* �R�[���o�b�N����1�b�҂� */
	main_call_func = NULL;	/* �������I���w�� */
	#endif
	return (0); 	/* �R�[���o�b�N���̏I�� */
}
	/* ����ȊO�̎���������܂���(���C�����œ�����������̂�) */
//	game_exit();/* ������Ƃ܂���(game_exit()���Q��ɂȂ����Ⴄ) */
//	sceKernelExitGame();/* ������Ƃ܂���(home�I�������ꍇ�A�Q�[���Z�[�u���o���Ȃ��A�I���������o���Ȃ�) */


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
		if (NULL==main_call_func)
		{
			return; 	/* �I������B */
		}
		{
			main_call_func();	/* �e���������s */
		}
		vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
		goto my_loop;
	}
}


/*---------------------------------------------------------
	boot_main()
---------------------------------------------------------*/

#if (1==HACK_FPU)
extern void disable_FPU_exeptions_in_main(void);	/* FPU��O�𖳌��ɂ���B disablefpu.S */
#endif

extern void game_system_init(void); 	/* �Q�[���V�X�e�������� */
extern void game_system_exit(void); 	/* �Q�[���V�X�e���I�� */
extern void game_main(void);

/* ������ -Dmain=SDL_main �̏ꍇ�A�}�N���Ȃ̂Ŏ����I�� int SDL_main(int argc, char *argv[]) �ɂȂ�B�����SDL����main()����ĂԁB */
global int main(int argc, char *argv[])
{
	#if (1==USE_FIX_GP)
	FixedGP();	/* */
	#endif
	#if (1==HACK_FPU)
	disable_FPU_exeptions_in_main();	/* ���̊֐���main()�����ɏ����Ȃ��ƃ_����������Ȃ�($31��M��̂�) */
	#endif
	SetupCallbacks();		/* regist_home_key [HOME]�L�[�ŏI�����邽�߂̃R�[���o�b�N��o�^ */
	game_system_init(); 	/* �Q�[���V�X�e�������� */
//
	scePowerSetClockFrequency(333,333,166);/* psp �� �N���b�N�� 333MHz �ɂ���� */
	game_main();
	scePowerSetClockFrequency(222,222,111);/* psp �� �N���b�N�� 222MHz �ɖ߂��� */
//
	game_system_exit(); 	/* �Q�[���V�X�e���I��(�����ŏI������) */
//	sceKernelExitGame();
	return (0);/* �_�~�[(�����ɂ͐�΂ɗ��Ȃ�) */
}

#if 0
/*---------------------------------------------------------
	dummy signal
	-------------------------------------------------------
	���[��...
---------------------------------------------------------*/
#include <signal.h>
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "_ansi.h"
#include <sys/signal.h>

_BEGIN_STD_C

typedef int sig_atomic_t;		/* Atomic entity type (ANSI) */

#define SIG_DFL ((_sig_func_ptr)0)		/* Default action */
#define SIG_IGN ((_sig_func_ptr)1)		/* Ignore action */
#define SIG_ERR ((_sig_func_ptr)-1) 	/* Error return */

struct _reent;

_sig_func_ptr _EXFUN(_signal_r, (struct _reent *, int, _sig_func_ptr));
int _EXFUN(_raise_r, (struct _reent *, int));

#ifndef _REENT_ONLY
_sig_func_ptr _EXFUN(signal, (int, _sig_func_ptr));
int _EXFUN(raise, (int));
#endif

_END_STD_C

#endif /* _SIGNAL_H_ */

global _sig_func_ptr signal(int aaa, _sig_func_ptr bbb)
{
	return (SIG_DFL);
}
#endif



#if 0
/*---------------------------------------------------------
	dummy SDL's signal
	-------------------------------------------------------
	���[��...
	-------------------------------------------------------
---------------------------------------------------------*/
#define NO_SIGNAL_H
/* Public functions */
int SDL_QuitInit(void)
{
#ifndef NO_SIGNAL_H
		void (*ohandler)(int);

		/* Both SIGINT and SIGTERM are translated into quit interrupts */
		ohandler = signal(SIGINT,  SDL_HandleSIG);
		if ( ohandler != SIG_DFL )
				signal(SIGINT, ohandler);
		ohandler = signal(SIGTERM, SDL_HandleSIG);
		if ( ohandler != SIG_DFL )
				signal(SIGTERM, ohandler);
#endif /* NO_SIGNAL_H */

		/* That's it! */
		return(0);
}

#ifdef NO_SIGNAL_H

/* No signals on this platform, nothing to do.. */

void SDL_InstallParachute(void)
{
		return;
}

void SDL_UninstallParachute(void)
{
		return;
}
#endif /* NO_SIGNAL_H */

#endif
