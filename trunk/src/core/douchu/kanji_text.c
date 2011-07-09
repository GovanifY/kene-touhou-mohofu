
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	演出用テキスト
---------------------------------------------------------*/

#include "kanji_system.h"

/* ゲーム中イベントメッセージ表示 */
global void game_command_00_kanji_hyouji(GAME_COMMAND *l)
{
	if ((255)!=l->user_255_code)
	{
		return; 	/* error!! */
	}
	if (0==l->user_x)
	{
		kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
		home_cursor();			/* カーソルをホームポジションへ移動 */
	}
	cg.msg_time = byou60(5);	/* 約 5 秒 */
//
	char *text;
	text = l->user_string;
	print_kanji000(text, /*int color_type*/(7), /*int wait*/(0) );
}

