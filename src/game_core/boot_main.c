
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ				http://code.google.com/p/kene-touhou-mohofu/
	ソースコード表示				http://code.google.com/p/kene-touhou-mohofu/source/browse/trunk/
	ソースファイル単体ダウンロード	http://kene-touhou-mohofu.googlecode.com/svn/trunk/
	ダウンロード例					http://kene-touhou-mohofu.googlecode.com/files/231_kene_r35-r37.zip
	231の開発報告ページ 			http://mohou.huuryuu.com/
	-------------------------------------------------------
	pspの起動ルーチン等
---------------------------------------------------------*/

/* */
#define FIX_TEST (1)
//#define FIX_TEST (0)

//#define USE_FIX_GP	(0)
#define USE_FIX_GP	(1)


/*---------------------------------------------------------
	Makefileに -Dmain=SDL_main があっても、強制的に -Dmain=SDL_main を無効にする。
---------------------------------------------------------*/

#ifdef main/* makefile に -Dmain=SDL_main がある場合 */
	#undef main/* 強制的に -Dmain=SDL_main を無効にする。 */
	/* -Dmain=SDL_main は非サポートです。 */
//	error! -Dmain=SDL_main は非サポートです。
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
#endif

/*---------------------------------------------------------
	PSP module info section
---------------------------------------------------------*/

#if (1)/* Makefile に -Dmain=SDL_main がない場合 */
	/*
	こちらの場合は PSPSDK の用意した起動ルーチンを使用する場合です。
	起動ルーチンは簡単なアセンブラ＋PSPのOSに利用関数を知らせる表で
	下記マクロは起動アセンブラの一部にに展開されますし、
	表は PSPSDK が自動作成してくれます。
	*/

#if 0
PSP_THREAD_ATTR_VFPU			VFPUの利用を有効にします。
PSP_THREAD_ATTR_USER			ユーザモードでスレッドを起動します。親スレッドがユーザモードの場合は、特に指定しなくともユーザモードで起動されます。
PSP_THREAD_ATTR_USBWLAN			USB/WlanAPIで使われています。通常、指定することはありません。
PSP_THREAD_ATTR_VSH 			VSHAPIで使われています。通常、指定することはありません。
PSP_THREAD_ATTR_SCRATCH_SRAM	スクラッチパッドの利用を許可します。FW1.0では使われておらず、特に指定しなくとも自由に利用が可能です。
PSP_THREAD_ATTR_NO_FILLSTACK	スレッド起動時にスタックを0xffで埋めないように指定します。
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

#if (1)/* Makefile が間違ってなければ本来要らないが、コンパイルできないお便りが届くと面倒なので。 */
	#ifndef KENE_RELEASE_VERSION
		#define KENE_RELEASE_VERSION	(36)
	#endif
	#ifndef KENE_UPDATE_VERSION
		#define KENE_UPDATE_VERSION 	(1)
	#endif
#endif

PSP_MODULE_INFO(
	KENE_NAME_STR,				/* name:			PSPのOSに教えてあげる名前。 */
	0x0000, 					/* attributes:		ユーザーモードで起動する。 */
	KENE_RELEASE_VERSION,		/* major_version:	このアプリのバージョンナンバー version 35.-- */
	KENE_UPDATE_VERSION);		/* minor_version:	このアプリのバージョンナンバー version --.01 */

PSP_MAIN_THREAD_ATTR(						/* このプログラムが使うOS資源。*/
	PSP_THREAD_ATTR_VFPU |					/* VFPUを使う */
//	PSP_THREAD_ATTR_SCRATCH_SRAM |			/* SCRATCH_SRAM 使う(指定しないほうが望ましい) */
	/*	SCRATCH_SRAM は指定しなくてもデフォルトで使用可能である。
		FW ver 1.00には無いので互換性を考慮するなら無くて構わない */
//	PSP_THREAD_ATTR_NO_FILLSTACK |			/* 開始時にスタックを 0xff で埋めつくさない。 */
	/* 0xffで埋めてくれた方が、バグ出しには都合が良いのでこのオプションは指定しない。 */
	PSP_THREAD_ATTR_CLEAR_STACK |
	/* 終了時にこのプログラムが使ったスタック(変数領域)をOSに消してもらう。 */
	THREAD_ATTR_USER |						/* ユーザーモードで起動する。(kernel系コールを使わない。) */
//	PSP_THREAD_ATTR_USBWLAN |				/* USB WLAN API使わない */
//	PSP_THREAD_ATTR_VSH |					/* VSH API使わない(dark-alex氏でないので VSH使いませんが、何か) */
	0);
	/* http://dxlibp.sourceforge.jp/cgi-bin/patio/read.cgi?no=857 */
	//	PSP_HEAP_SIZE_KB(16*1024); // 新規追加
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); // 新規追加


	#if (0==KENE_SYOMEI_OFW)
	// 0 == fw1.00対応版
	// 非署名版
		PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (プログラムが使用する変数の)スタック領域のサイズ */
												/* ここが大きいとmallocできる量がその分減る */
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(48);		/* (r35) */
	//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); 	/* (r35) */
	#endif
	//
	#if (1==KENE_SYOMEI_OFW)
	// 0 == ofw対応版
	// 署名版
//	PSP_HEAP_SIZE_KB(4*1024); // fatal error: load0 cant load image data/bg/title_bg.png
	PSP_HEAP_SIZE_KB(12*1024); // (12MBytes, oslib) (r38)MusicRoomでハングアップする。
//	PSP_HEAP_SIZE_KB(14*1024); // (14MBytes)
//	PSP_MAIN_THREAD_STACK_SIZE_KB(512); // (r38)MusicRoomでハングアップする。(512k[bytes]==0.5M[bytes]は大きすぎる。)
//	PSP_MAIN_THREAD_STACK_SIZE_KB(256); // (r38)minimalist, MusicRoomでハングアップする。cygwinなら ok!
	PSP_MAIN_THREAD_STACK_SIZE_KB(128); // (r38) 多分128k[bytes]もあれば十分なんじゃないかな?
//	PSP_MAIN_THREAD_STACK_SIZE_KB(64); //
	#endif

	// ヒープ12MBの場合
	// FW1.00			起動に失敗しました(80020148)
	// CFW5.00M33-6

/* 2008年ぐらいの古い PSPSDK は -1(自動) に対応していないので -1 にするとハングアップする */
#endif

/*
	署名起動についての見解。

	[ 模倣風のメモリ配置 ](本当はもうちょっと複雑だと思うけど簡略化してある。っていうか良く知らないので想像も入ってる)
0000+------------------------+
	| スクラッチパッドSRAM(?)| 2kBytesだっけ。(模倣風では使用していない)
	+------------------------+
	|	 ＯＳとかBIOSとか	 |(カーネルメモリ。模倣風はユーザーモード起動なのでアクセスできない。
	|	(8MBytesぐらい？)	 |万一バグで模倣風がアクセスした場合には、pspの電源が切れるのが正常動作)
	+------------------------+
	|	他のプログラム		 |(他のプラグインとか)
	+------------------------+
	|	模倣風のプログラム	 | (EBOOT.PBPの大きさは 1.5MBぐらいだけど、
	|(通常は1MBぐらいなのだが| 模倣風のプログラムは全部で1.0MBもない。
	|署名版は6MBytesぐらい(?)| EBOOT.PBPの0.5MB以上はXMBでしか使用しない
	|		に膨れ上がる)	 | データーなので実行時にpspのメモリにロードされない)
	+------------------------+
	| 模倣風の const データー|(ヒープ領域を小さく設定した場合、const データーより、ヒープにデーターを置いた方が実行速度が速い事がある。
	|						 |理由は GCCでは 関数を呼ぶ場合に必ずスタックを使うので、ヒープ領域が小さいとアドレスがフラッシュしなくなり
	|						 |キャッシュが壊れにくいからだと思う)
	+------------------------+
	|						 |(HOMEコールバックのスレッドとか作るとここが消費される。たぶん)
	|	   空きメモリ		 |
	|						 |
	+------------------------+
	|	   ヒープ領域		 |(mallocとかのメモリはここから確保する。模倣風で「メインメモリ」って呼んでるのはここの事)
	|		  ↓			 |
	|						 |← ヒープ領域の大きさの限界を設定できる。(PSP_HEAP_SIZE_KB();)
	|	   空きメモリ		 |
	|						 |← スタック領域の大きさの限界を設定できる。(PSP_MAIN_THREAD_STACK_SIZE_KB();)
	|		  ↑			 |
	|	  スタック領域		 |(mipsでは関数を呼ぶ場合にスタックを消費する。)
	+------------------------+(psp-1000は16MBytes, psp-2000以降は32MBytes)
	メモリ積んで無い部分
	+------------------------+ 0x04000000 (通常0x44000000と書くが同じアドレス。上位ビットはアドレスでなくてキャッシュの有無)
	| VRAMとかハードウェアー |(模倣風で「VRAM」って呼んでるのはここの事。)
ffff+------------------------+

	署名起動する為にプログラムが大きくなる必要がある。(1.0MB以下 が 6.0MB以上に)
	//
	署名版ではプログラムが大きくなると「ヒープ領域」が足りなくなる。(たぶん)
	//
	pspではヒープ領域の限界を設定できるので、
	「空きメモリ」がまだあるのに「ヒープ領域」が足りなくなる。
	という状態になり起動しない。
//
	この問題を解決するには「ヒープ領域」を大きめに設定すれば良い。
	だけど「ヒープ領域」を大きくしすぎると「スタック領域」を破壊して、
	プログラムが動かない危険がある。
	と思われるので程々にしとく。
//
	そういう訳で、12MBytes になってる。
	たぶんこのサイズは「ヒープ領域」のサイズではなくて、
	12MBytes == 「署名版プログラム + 署名版const データー + ヒープ領域」のサイズ。
	だと思われる。
*/

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
#if (1==FIX_TEST)
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
	#endif /*(1==USE_FIX_GP)*/
#endif /*(1==FIX_TEST)*/


global GAME_CORE_GLOBAL_CALLBACK cb;
/*---------------------------------------------------------
	[HOME]キーで終了処理部分
	-------------------------------------------------------
	[HOME]キーで終了するためのコールバックを登録
	([HOME]キーメニューで、終了しますか？「はい」が選択されると実行される部分)
	-------------------------------------------------------
	コールバック側でゲームの保存や解放等などの処理をしてはいけない。
	メイン側でこれらの処理は必ず行うので、
	ここでこれらの処理をすると、結果的に２回処理を行う事になる。
	(メイン側は２回処理を行っても問題無いように作ってない)
	-------------------------------------------------------
	それ以外の事をしたらまずい(メイン側で同じ事をするので)
	game_exit();←あるとまずい(game_exit()が２回になっちゃう)
	sceKernelExitGame();←あるとまずい(home終了した場合、ゲームセーブが出来ない、終了処理が出来ない)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* コールバック側から、メイン側の状態を操作する */
	cb.main_call_func = NULL;	/* 終了指示 */
	#if 1
	/* 本来 cb.main_call_func は セマフォ管理すべきかも。sceKernelCreateSema()?? だけどしてない。 */
	/* 要るかも */
	sceKernelDelayThread(1000000);/* コールバック側が1秒待つ */
	cb.main_call_func = NULL;	/* もう一回終了指示 */
	#endif
	return (0); 	/* コールバック側の終了 */
}


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
		if (NULL==cb.main_call_func)
		{
			return; 	/* 終了する。 */
		}
		{
			cb.main_call_func();	/* 各処理を実行 */
		}
		vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
		goto my_loop;
	}
}


/*---------------------------------------------------------
	boot_main()
---------------------------------------------------------*/
// CFLAGS += -DHACK_FPU=1 は、src/alegrex/alegrex.mak で指定しています。
#if (1==HACK_FPU)
extern void disable_FPU_exeptions_in_main(void);	/* FPU例外を無効にする。 disablefpu.S */
#endif
#if 1/*(てすと)*/
extern void ini_save(void);
#endif

extern void game_system_init(void); 	/* ゲームシステム初期化 */
extern void game_system_exit(void); 	/* ゲームシステム終了 */
extern void game_main(void);

/* ここは -Dmain=SDL_main の場合、マクロなので自動的に int SDL_main(int argc, char *argv[]) になる。それをSDL側のmain()から呼ぶ。 */
/*
	-Dmain=SDL_main は強制的に無効になっているので、(PSPSDKのローダー後に)ここから起動する。
	模倣風では atexit はフックされていない事を想定しているので、 exit(); は使わない。
	模倣風では sceKernelExitGame(); で終了する。
*/
global int main(int argc, char *argv[])
{
#if (1==FIX_TEST)
	#if (1==USE_FIX_GP)
	FixedGP();	/* 起動時の GPレジスタが異常値であれば、修正する。 */
	#endif
	#if (1==HACK_FPU)
	/* fpu 例外が発生した場合の割り込み処理を、pspのハードウェアーレベルで無効にする。 */
	disable_FPU_exeptions_in_main();	/* この関数はmain()直下に書かないとダメかもしれない($31を弄るので) */
	#endif
#endif
	SetupCallbacks();		/* regist_home_key [HOME]キーで終了するためのコールバックを登録 */
	game_system_init(); 	/* ゲームシステム初期化 */
//
	scePowerSetClockFrequency(333,333,166);/* psp の クロックを 333MHz にするよ */
	game_main();
	scePowerSetClockFrequency(222,222,111);/* psp の クロックを 222MHz に戻すよ */
	/*(てすと)*/
	#if (1)/*(r34、何故か巧く終了できないのでとりあえず)*/
	ini_save();
	sceKernelExitGame();
	#else
	game_system_exit(); 	/* ゲームシステム終了(内部で終了する) */
	#endif
//	sceKernelExitGame();
	return (0);/* ダミー(ここには絶対に来ない) */
}
