
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	(指定画像が無い等)致命的エラーの場合、
	メッセージを出してゲームを強制終了する。
---------------------------------------------------------*/
#include "kanji_system.h"
extern void vbl_draw_screen(void);/*support.c*/

/*---------------------------------------------------------
	(キーコンフィグ設定中等)致命的エラーでは
	キー入力自体がおかしくなる場合もあるかもしれないので、
	最低限の入力として専用簡易入力処理を行う。
---------------------------------------------------------*/

static void hit_any_key(void)
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

global void psp_fatal_error(char *msg, ...)
{
	// [エラーメッセージを描画]
	char msgbuf[128];	/* 128==4*32 (pspの構造上32の倍数で指定) */
	va_list argptr;
	//
	va_start(argptr, msg);
	vsprintf(msgbuf, msg, argptr);
	va_end(argptr);
	strcpy(my_font_text, msgbuf);/*(エラー文字列をmy_font_textに書きこむ)*/
//	sw itch (errorlevel)/*int errorlevel,*/
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
	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
	hit_any_key();/*[簡易入力待ち]*/
	//[強制終了します。と描画]
	kanji_window_all_clear();			/* メッセージウィンドウの内容を消す。 */
	strcpy(my_font_text, "致命的エラーなので終了します。" "\\n");
	kanji_color(7);
	kanji_draw();
	vbl_draw_screen();	/* 画面描画とキー入力(本当は v-blanc タイミングで) */
	hit_any_key();/*[簡易入力待ち]*/
	//[強制終了する]
	sceKernelExitGame();	//if (errorlevel==ERR_FATAL) exit(1);/*exit(1)はpspで使えないので注意*/
	//
//	cb.ma in_call_func = NULL;
}
