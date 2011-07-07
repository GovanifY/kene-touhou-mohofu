
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	エンディング後の終了判定
	-------------------------------------------------------
---------------------------------------------------------*/

extern void game_clear_set_password(void);
extern void player_loop_quit(void);
global void game_command_01_game_all_clear(GAME_COMMAND *l)
{
	#if 0
	/* なんか知らんが、Lunaticでちょっとチェックしたら、オーバーフローするｗ。 */
	player_dummy_add_score(adjust_score_by_difficulty((
				(cg.zanki * score(3000000)) +	/* 残機   x 300万 */
				(cg.bombs * score(1000000)) 	/* 残ボム x 100万 */
			)));
	#else
	/* なんか知らんが、オーバーフローするので、個別に足してみる。 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.zanki) * score(3000000)) ))); 	/* 残機   x 300万 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.bombs) * score(1000000)) ))); 	/* 残ボム x 100万 */
	#endif
	/* 清算して消える */
	cg.zanki = 0;
	cg.bombs = 0;
	//
	game_clear_set_password();
	#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
	cg.game_now_max_continue = 1;	/* コンティニューさせない */
	player_loop_quit();
	#endif
}
