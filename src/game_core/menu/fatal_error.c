
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------

---------------------------------------------------------*/
#include "kanji_system.h"
extern void vbl_draw_screen(void);/*support.c*/

/*---------------------------------------------------------
	キー入力関連の処理(本来デバッグ用)
	上のキー入力に統合しても良いが、キー入力自体が
	おかしくなる場合もあるので、暫定的に最低限の入力として
	残してある。
---------------------------------------------------------*/

/*global*/static void hit_any_key(void)
{
	SceCtrlData cpad;
	/* 離されるまで待つ */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		if (0 == cpad.Buttons)
		{
			goto l_end1;
		//	break;
		}
	}
	l_end1:
	;
	/* 押されるまで待つ */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		/* Any Key */
		if (cpad.Buttons & (PSP_CTRL_SQUARE|PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE) )
		{
			goto l_end2;
		//	break;
		}
	}
	l_end2:
	;
}

/*---------------------------------------------------------
	エラー処理
---------------------------------------------------------*/

global void psp_fatal_error(char *msg, ...)/*int errorlevel,*/
{
	char msgbuf[128];	/* 128==4*32 (pspの構造上32の倍数で指定) */
	va_list argptr;
	//
	va_start(argptr, msg);
	vsprintf(msgbuf, msg, argptr);
	va_end(argptr);
	strcpy(my_font_text, msgbuf);/*(エラー文字列をmy_font_textに書きこむ)*/
//	cb.ma in_call_func = error_callback_init;
//	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
//	static void error_callback_init(void)
//	sw itch (errorlevel)
	{
	//ca se ERR_DEBUG:	if (debug)	{ fprintf(stdout,"DEBUG: %s\n",msgbuf); } break;
	//ca se ERR_INFO:		fprintf(stdout,"INFO: %s\n",msgbuf); break;
	//ca se ERR_FATAL: //	fprintf(stdout,"FATAL: %s\n",msgbuf);
	#if (1)/*(漢字関連の初期化)*/
	kanji_init_standard();/*(漢字関連の標準初期化)*/
//	set_kanji_hide_line(ML_LINE_99_MAX);/*(全行表示する。)*/
	#endif
	#if (1)/*(デバッグ)*/
	{
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_02_JIKI_SPELL_CARD);/* [赤/自機カード用背景]せりふ背景on */
		cg.msg_time 				= (65536);	/* 約 18 分 */
	//	strcpy(my_font_text, "致命的エラー。" "\n");
	//	strcpy(my_font_text, msgbuf);
		kanji_color(7);
		kanji_draw();
	}
	#endif
	}
//	cb.ma in_call_func = error_01_callback_hit_any_key1;
	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
//	static void error_01_callback_hit_any_key1(void)
	hit_any_key();
//	cb.ma in_call_func = error_02_callback_draw_02;
//	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
//	global void error_02_callback_draw_02(void)
	kanji_window_all_clear();			/* メッセージウィンドウの内容を消す。 */
	strcpy(my_font_text, "致命的エラーなので終了します。" "\\n");
	kanji_color(7);
	kanji_draw();
//	cb.ma in_call_func = error_03_callback_hit_any_key2;
	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
//	static void error_03_callback_hit_any_key2(void)
	hit_any_key();
//	cb.ma in_call_func = error_04_callback_draw_03;
//	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
//	global void error_04_callback_draw_03(void)
	sceKernelExitGame();	//if (errorlevel==ERR_FATAL) exit(1);/*exit(1)はpspで使えないので注意*/
	//
//	cb.ma in_call_func = NULL;
//	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
}
