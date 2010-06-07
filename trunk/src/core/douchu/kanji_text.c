
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	演出用テキスト
---------------------------------------------------------*/

#include "kanji_system.h"

/* ゲーム中イベントメッセージ表示 */
global void add_enemy_kanji_string(STAGE_DATA *l)
{
	if (0==l->user_x)
	{
		kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
		home_cursor();			/* カーソルをホームポジションへ移動 */
	}
	msg_time = (60*5);
//
	char *text;
	text = l->user_string;
	print_kanji000(text, /*int color_type*/7, /*int wait*/0);
}
