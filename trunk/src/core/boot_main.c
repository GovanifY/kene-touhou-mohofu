
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	pspの起動ルーチン等
---------------------------------------------------------*/

#if 0
	/* makefile に -Dmain=SDL_main がある場合 */
/*
	 makefile に -Dmain=SDL_main がある場合は、
	SDLが用意したメインルーチンを使用するので、ここには
	書けません。(C言語はmain()関数を2つリンク出来ません)
	SDLが用意したメインルーチン
	http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fmain%2Fpsp%2FSDL_psp_main.c
	は、
	PSP-1000でもversionによって起動したりしなかったり、
	PSP-2000は起動しなかったり、C++を使うと何故か起動したりしなかったり、
	と色々問題が多いので使わない方が賢明です。
*/
#else
	/* makefile に -Dmain=SDL_main がない場合 */
/*
	こちらの場合は PSPSDK の用意した起動ルーチンを使用する場合です。
	起動ルーチンは簡単なアセンブラ＋PSPのOSに利用関数を知らせる表で
	下記マクロは起動アセンブラの一部にに展開されますし、
	表は PSPSDK が自動作成してくれます。
*/
/*---------------------------------------------------------
	PSP module info section
---------------------------------------------------------*/
#if 0
PSP_THREAD_ATTR_VFPU			VFPUの利用を有効にします。
PSP_THREAD_ATTR_USER			ユーザモードでスレッドを起動します。親スレッドがユーザモードの場合は、特に指定しなくともユーザモードで起動されます。
SP_THREAD_ATTR_USBWLAN			USB/WlanAPIで使われています。通常、指定することはありません。
PSP_THREAD_ATTR_VSH 			VSHAPIで使われています。通常、指定することはありません。
PSP_THREAD_ATTR_SCRATCH_SRAM	スクラッチパッドの利用を許可します。FW1.0では使われておらず、特に指定しなくとも自由に利用が可能です。
PSP_THREAD_ATTR_NO_FILLSTACK	スレッド起動時にスタックを0xFFで埋めないように指定します。
PSP_THREAD_ATTR_CLEAR_STACK 	スレッド終了時にスタックをゼロクリアします。
THREAD_ATTR_USER				PSP_THREAD_ATTR_USERの別名です。
THREAD_ATTR_VFPU				PSP_THREAD_ATTR_VFPUの別名です。
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
PSP_MODULE_INFO("KENE", 0x0000, 0, 4);		/* PSPのOSに教えてあげる名前。ユーザーモードで起動する。 */
PSP_MAIN_THREAD_ATTR(						/* このプログラムが使うOS資源。*/
	PSP_THREAD_ATTR_VFPU |					/* VFPUを使う */
//	PSP_THREAD_ATTR_SCRATCH_SRAM |			/* SCRATCH_SRAM 使う(指定しないほうが望ましい) */
	/*	SCRATCH_SRAM は指定しなくてもデフォルトで使用可能である。
		FW ver 1.00には無いので互換性を考慮するなら無くて構わない */
//	PSP_THREAD_ATTR_NO_FILLSTACK |			/* 開始時にスタックを 0xFF で埋めつくさない。 */
	/* 0xffで埋めてくれた方が、バグ出しには都合が良いのでこのオプションは指定しない。 */
	PSP_THREAD_ATTR_CLEAR_STACK |
	/* 終了時にこのプログラムが使ったスタック(変数領域)をOSに消してもらう。 */
	THREAD_ATTR_USER |						/* ユーザーモードで起動する。(kernel系コールを使わない。) */
//	PSP_THREAD_ATTR_USBWLAN |				/* USB WLAN API使わない */
//	PSP_THREAD_ATTR_VSH |					/* VSH API使わない(dark-alex氏でないので VSH使いませんが、何か) */
	0);

PSP_MAIN_THREAD_STACK_SIZE_KB(32);			/* (プログラムが使用する変数の)スタック領域のサイズ */
											/* ここが大きいとmallocできる量がその分減る */
/* 2008年ぐらいの古い PSPSDK は -1(自動) に対応していないので -1 にするとハングアップする */
#endif

global void (*main_call_func)(void);
global void (*return_call_func)(void);

//#define USE_FIX_GP	(0)
#define USE_FIX_GP	(1)
/*---------------------------------------------------------
	GPレジスタの値をチェックし、異常な場合は変更する
	-------------------------------------------------------
	HBLのversionによっては、起動時の GPレジスタに異常値が入る事が
	あるらしいのでその対策。(Ruka氏)
	-------------------------------------------------------
	GPレジスタ(GP相対16ビットレジスタ)(GPREL16, GP rerative 16 bit register)というのは
	MIPSのCPUが持っているレジスタで、
	+32k[bytes]～-32k[bytes]の範囲(64k[bytes]の範囲)で、相対アクセスする為のレジスタ。
---------------------------------------------------------*/
	#if (1==USE_FIX_GP)
static void FixedGP(void)
{
	void* my_gp_value;
	/* GPレジスタの値をチェック */
	asm volatile ("move %0, $gp\n" : "=r" (my_gp_value));
	if (module_info.gp_value != my_gp_value)
	{
		/* GPレジスタの値が異常な場合は修正 */
		asm volatile ("move $gp, %0\n" :: "r" (module_info.gp_value));
	}
}
	#endif


/*---------------------------------------------------------
	[HOME]キーで終了処理部分
	-------------------------------------------------------
	[HOME]キーで終了するためのコールバックを登録
	([HOME]キーメニューで、終了しますか？「はい」が選択されると実行される部分)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* コールバック側から、メイン側の状態を操作する */
	main_call_func = NULL;	/* 終了指示 */
	#if 1
	/* 本来 main_call_func は セマフォ管理すべきかも。sceKernelCreateSema()?? だけどしてない。 */
	/* 要るかも */
	sceKernelDelayThread(1000000);/* コールバック側が1秒待つ */
	main_call_func = NULL;	/* もう一回終了指示 */
	#endif
	return (0); 	/* コールバック側の終了 */
}
	/* それ以外の事をしたらまずい(メイン側で同じ事をするので) */
//	game_exit();/* ←あるとまずい(game_exit()が２回になっちゃう) */
//	sceKernelExitGame();/* ←あるとまずい(home終了した場合、ゲームセーブが出来ない、終了処理が出来ない) */


/*---------------------------------------------------------
	[HOME]キー用、コールバック登録
	-------------------------------------------------------
	[HOME]キーで終了するためにコールバックを登録します。
---------------------------------------------------------*/

/* Thread to create the callbacks and then begin polling */
static int CallbackThread(SceSize args, void *argp)
{
	/* 新規にコールバックを作成 */
	int callback_id = sceKernelCreateCallback("Exit Callback", ( int (*)(int, int, void*) )exit_callback, NULL);
	sceKernelRegisterExitCallback(callback_id); 	/* [HOME]キー用のコールバックを登録 */
	sceKernelSleepThreadCB();
	return (0);
}

/*---------------------------------------------------------
	[HOME]キー用、スレッド作成
	-------------------------------------------------------
	[HOME]キーで終了するためのスレッドを作成します。
---------------------------------------------------------*/

/* Sets up the callback thread and returns its thread id */
static /*int*/void SetupCallbacks(void)
{
	/* 新規にスレッドを作成 */
	SceUID/*int*/ thread_id;
	thread_id = 0;/* 万が一正常に作成できない場合を考えて 0 を書く。 */
	thread_id = sceKernelCreateThread("Update Thread", CallbackThread, 0x11, 0xFA0, 0, 0); /* スレッドの優先度等を設定 */
	if (0 <= thread_id) 	/* スレッドが作成出来たら */
	{
		sceKernelStartThread(thread_id, 0, 0);	/* スレッド開始 */
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
			return; 	/* 終了する。 */
		}
		{
			main_call_func();	/* 各処理を実行 */
		}
		vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
		goto my_loop;
	}
}


/*---------------------------------------------------------
	boot_main()
---------------------------------------------------------*/

#if (1==HACK_FPU)
extern void disable_FPU_exeptions_in_main(void);	/* FPU例外を無効にする。 disablefpu.S */
#endif

extern void game_system_init(void); 	/* ゲームシステム初期化 */
extern void game_system_exit(void); 	/* ゲームシステム終了 */
extern void game_main(void);

/* ここは -Dmain=SDL_main の場合、マクロなので自動的に int SDL_main(int argc, char *argv[]) になる。それをSDL側のmain()から呼ぶ。 */
global int main(int argc, char *argv[])
{
	#if (1==USE_FIX_GP)
	FixedGP();	/* */
	#endif
	#if (1==HACK_FPU)
	disable_FPU_exeptions_in_main();	/* この関数はmain()直下に書かないとダメかもしれない($31を弄るので) */
	#endif
	SetupCallbacks();		/* regist_home_key [HOME]キーで終了するためのコールバックを登録 */
	game_system_init(); 	/* ゲームシステム初期化 */
//
	scePowerSetClockFrequency(333,333,166);/* psp の クロックを 333MHz にするよ */
	game_main();
	scePowerSetClockFrequency(222,222,111);/* psp の クロックを 222MHz に戻すよ */
//
	game_system_exit(); 	/* ゲームシステム終了(内部で終了する) */
//	sceKernelExitGame();
	return (0);/* ダミー(ここには絶対に来ない) */
}

#if 0
/*---------------------------------------------------------
	dummy signal
	-------------------------------------------------------
	うーん...
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
	うーん...
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
