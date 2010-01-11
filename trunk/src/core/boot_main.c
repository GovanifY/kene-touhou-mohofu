
/*---------------------------------------------------------
	pspの起動ルーチン等
---------------------------------------------------------*/

#include "game_main.h"

#if 0
	/* makefile に -Dmain=SDL_main がある場合  */
/*
	 makefile に -Dmain=SDL_main がある場合は、
	SDLが用意したメインルーチンを使用するので、ここには
	書けません。(C言語はmain()関数を2つリンク出来ません)
	SDLが用意したメインルーチンは、
	PSP-1000でもversionによって起動したりしなかったり、
	PSP-2000は起動しなかったり、C++を使うと何故か起動したりしなかったり、
	と色々問題が多いので使わない方が賢明です。
*/
#else
	/* makefile に -Dmain=SDL_main がない場合  */
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
PSP_THREAD_ATTR_VFPU VFPUの利用を有効にします
PSP_THREAD_ATTR_USER ユーザモードでスレッドを起動します。親スレッドがユーザモードの場合は、特に指定しなくともユーザモードで起動されます。
SP_THREAD_ATTR_USBWLAN USB/WlanAPIで使われています。通常、指定することはありません
PSP_THREAD_ATTR_VSH VSHAPIで使われています。通常、指定することはありません。
PSP_THREAD_ATTR_SCRATCH_SRAM スクラッチパッドの利用を許可します。FW1.0では使われておらず、特に指定しなくとも自由に利用が可能です。
PSP_THREAD_ATTR_NO_FILLSTACK スレッド起動時にスタックを0xFFで埋めないように指定します。
PSP_THREAD_ATTR_CLEAR_STACK スレッド終了時にスタックをゼロクリアします。
THREAD_ATTR_USER PSP_THREAD_ATTR_USERの別名です
THREAD_ATTR_VFPU PSP_THREAD_ATTR_VFPUの別名です
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
PSP_MODULE_INFO("KENE", 0x0000, 0, 4);	/* PSPのOSに教えてあげる名前。ユーザーモードで起動する。 */
PSP_MAIN_THREAD_ATTR(			/* このプログラムが使うOS資源。*/
	PSP_THREAD_ATTR_VFPU |			/* VFPUを使う */
//	PSP_THREAD_ATTR_SCRATCH_SRAM |	/* SCRATCH_SRAM 使う(指定しないほうが望ましい) */
	/*	SCRATCH_SRAM は指定しなくてもデフォルトで使用可能である。
		FW ver 1.00には無いので互換性を考慮するなら無くて構わない */
//	PSP_THREAD_ATTR_NO_FILLSTACK |	/* 開始時にスタックを 0xFF で埋めつくさない。 */
	/* 0xffで埋めてくれた方が、バグ出しには都合が良いのでこのオプションは指定しない。 */
	PSP_THREAD_ATTR_CLEAR_STACK |
	/* 終了時にこのプログラムが使ったスタック(変数領域)をOSに消してもらう。 */
	THREAD_ATTR_USER |			/* ユーザーモードで起動する。(kernel系コールを使わない。) */
//	PSP_THREAD_ATTR_USBWLAN |	/* USB WLAN API使わない */
//	PSP_THREAD_ATTR_VSH |		/* VSH API使わない(dark-alex氏でないので VSH使いませんが、何か) */
	0);

PSP_MAIN_THREAD_STACK_SIZE_KB(32);		/* (プログラムが使用する変数の)スタック領域のサイズ */
										/* ここが大きいとmallocできる量がその分減る */
/* 2008年ぐらいの古い PSPSDK は -1(自動) に対応していないので -1 にするとハングアップする */
#endif

//extern int psp_loop;
/*extern*/ int psp_loop;

/*---------------------------------------------------------
	[HOME]キーで終了するためのコールバックを登録
	([HOME]キーメニューで、終了しますか？「はい」が選択されると実行される部分)
---------------------------------------------------------*/

static int exit_callback(int arg1, int arg2, void *common)
{
	/* コールバック側から、メイン側の状態を操作する */
	psp_loop = ST_PSP_QUIT;//newsta te(ST_PSP_QUIT,0,1);// [***** 追加:090103
	#if 1
	/* 要るかも */
	sceKernelDelayThread(1000000);/* コールバック側が1秒待つ */
	psp_loop = ST_PSP_QUIT;/* もう一回終了フラグ */
	#endif
	return (0); 	/* コールバック側の終了 */
}
	/* それ以外の事をしたらまずい(メイン側で同じ事をするので) */
//	game_exit();/* ←あるとまずい(game_exit()が２回になっちゃう) */
//	sceKernelExitGame();/* ←あるとまずい(home終了した場合、ゲームセーブが出来ない、終了処理が出来ない) */


/*---------------------------------------------------------
	[HOME]キーで終了するためのコールバックを登録(コールバックを登録)
---------------------------------------------------------*/

static int callback_homekey_thread(SceSize args, void *argp)
{
	/* 新規にコールバックを作成 */
	int callback_id = sceKernelCreateCallback("exit callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(callback_id); 	/* [HOME]キー用のコールバックを登録 */
	sceKernelSleepThreadCB();
	return (0);
}

/*---------------------------------------------------------
	[HOME]キーで終了するためのコールバックを登録(スレッドを作成)
---------------------------------------------------------*/

/* Sets up the callback thread and returns its thread id */
static /*int*/void regist_home_key(void)
{
	/* 新規にスレッドを作成 */
	int thread_id = sceKernelCreateThread("update thread", callback_homekey_thread, 0x11, 0xFA0, 0, 0); /* スレッドの優先度等を設定 */
	if (0 <= thread_id) 	/* スレッドが作成出来たら */
	{
		sceKernelStartThread(thread_id, 0, 0);	/* スレッド開始 */
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
//		case (ST_INIT_PLAYER_SELECT>>8):	player_opt_init();			break;吸収。なし
		case (ST_WORK_PLAYER_SELECT>>8):	player_opt_work();			break;
		case (ST_INIT_NAME_ENTRY>>8):		name_entry_init();			break;
		case (ST_WORK_NAME_ENTRY>>8):		name_entry_work();			break;
//
		case (ST_WORK_STAGE_FIRST>>8):		stage_first_init(); 		break;
		case (ST_WORK_STAGE_CLEAR>>8):		stage_clear_work(); 		break;
//
	//	case (ST_INIT_GAME_OVER>>8):		gameover_init();			break;吸収。なし
		case (ST_WORK_GAME_OVER>>8):		gameover_work();			break;	/*newsta te(ST_INTRO,0,1);*/
	//	case (ST_INIT_RESULT>>8):			result_init();				break;吸収。なし
		case (ST_WORK_RESULT>>8):			result_work();				break;
	//	case (ST_INIT_KEY_CONFIG>>8):		key_config_init();			break;吸収。なし
		case (ST_WORK_KEY_CONFIG>>8):		key_config_work();			break;
	//	case (ST_INIT_STORY>>8):			story_init();				break;吸収。なし
		case (ST_WORK_STORY>>8):			story_work();				break;
	//	case (ST_INTRO>>8): 				intro_init();				break;
	//	case (ST_INTRO>>8): 				intro_work();				break;
	//	case (ST_START_INTRO>>8):			startintro_init();			break;
	//	case (ST_START_INTRO>>8):			startintro_work();			break;
		}
		#else
		/*const*/static void (*aaa_call_table[/*16*/(16)])(void) =
		{
			NULL,	/* psp終了 */
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
		/* 実行 */
		{
			(aaa_call_table[(((u8)(psp_loop>>8)))])();
		}
		#endif
		vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
	}
}


/*---------------------------------------------------------
	boot_main()
---------------------------------------------------------*/

#if (1==HACK_FPU)
extern void disable_FPU_exeptions_in_main(void);	/* FPU例外を無効にする。 disablefpu.S */
#endif

extern void game_system_init(void/*int argc, char *argv[]*/);
extern void game_system_exit(void);
extern void game_main(void);

/* ここは -Dmain=SDL_main の場合、マクロなので自動的に int SDL_main(int argc, char *argv[]) になる。それをSDL側のmain()から呼ぶ。 */
int main(int argc, char *argv[])
{
	#if (1==HACK_FPU)
	disable_FPU_exeptions_in_main();	/* この関数はmain()直下に書かないとダメかもしれない($31を弄るので) */
	#endif
//	psp_loop = 1;		/* --- ループフラグ(0で終了。0以外は起動中) */
	regist_home_key();/* [HOME]キーで終了するためのコールバックを登録 */
	game_system_init();/* ゲームシステム初期化 */
//
	scePowerSetClockFrequency(333,333,166);/* psp の クロックを 333MHz にするよ */
	game_main();
	scePowerSetClockFrequency(222,222,111);/* psp の クロックを 222MHz に戻すよ */
//
	game_system_exit();/* ゲームシステム終了 */
//	sceKernelExitGame();
	return (0);
}
