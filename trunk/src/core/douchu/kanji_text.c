
#include "bullet_object.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	演出用テキスト
---------------------------------------------------------*/

/* ゲーム中イベントメッセージ表示 */
void add_enemy_kanji_string(STAGE_DATA *l)
{
	if (0==l->user_x)
	{
		script_message_window_clear();
	}
	msg_time = (60*5);
//
	char *text;
	text = l->user_string;
	print_kanji000(text, /*int color_type*/7, /*int wait*/0);
}
