
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	エンディング後の終了判定
	-------------------------------------------------------
---------------------------------------------------------*/
extern/*static*/ int	now_max_continue;
extern void game_clear_set_password(void);
extern void player_loop_quit(void);
global void add_enemy_all_clear(STAGE_DATA *l)
{
	#if 0
	/* なんか知らんが、Lunaticでちょっとチェックしたら、オーバーフローするｗ。 */
	player_dummy_add_score(adjust_score_by_difficulty((
				(pd.zanki * score(3000000)) +	/* 残機   x 300万 */
				(pd.bombs * score(1000000)) 	/* 残ボム x 100万 */
			)));
	#else
	/* なんか知らんが、オーバーフローするので、個別に足してみる。 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)pd.zanki) * score(3000000)) ))); 	/* 残機   x 300万 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)pd.bombs) * score(1000000)) ))); 	/* 残ボム x 100万 */
	#endif
	/* 清算して消える */
	pd.zanki = 0;
	pd.bombs = 0;
	//
	game_clear_set_password();
	#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
	now_max_continue = 1;	/* コンティニューさせない */
	player_loop_quit();
	#endif
}
