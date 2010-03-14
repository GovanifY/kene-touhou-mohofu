
#include "game_main.h"

/*---------------------------------------------------------
	エンディング後の終了判定
	-------------------------------------------------------

---------------------------------------------------------*/
extern/*static*/ int	now_max_continue;
extern void game_clear_set_password(void);
extern void player_loop_quit(void);
void add_enemy_all_clear(STAGE_DATA *l)
{
	{

		//	error(ERR_DEBUG, "sorry, no more le vels in this alpha-version");
	//	pd_score += pd_zanki*(score(2000)+(difficulty*score(4000)));
	#if 0
	/* なんか知らんが、Lunaticでちょっとチェックしたら、オーバーフローするｗ。 */
		player_add_score(adjust_score_by_difficulty((
					(pd_zanki * score(3000000)) +	/* 残機   x 300万 */
					(pd_bombs * score(1000000))  	/* 残ボム x 100万 */
				)));
	#else
	/* なんか知らんが、オーバーフローするので、個別に足してみる。 */
		player_add_score(adjust_score_by_difficulty((	(((u32)pd_zanki) * score(3000000)) )));	/* 残機   x 300万 */
		player_add_score(adjust_score_by_difficulty((	(((u32)pd_bombs) * score(1000000)) )));	/* 残ボム x 100万 */
	#endif
		/* 清算して消える */
		pd_zanki = 0;
		pd_bombs = 0;
	}
	game_clear_set_password();
	#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
	now_max_continue = 1; 	/* コンティニューさせない */
	player_loop_quit();
	#endif
}

	//return;
//	if (/**le vel*/player_now_stage==6	)
//	if (gt>125000000/*60500000*/)/*125.0[sec]←60.5[sec]*/
