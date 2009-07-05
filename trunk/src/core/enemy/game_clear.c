
#include "support.h"

/*---------------------------------------------------------
	エンディング後の終了判定
	-------------------------------------------------------

---------------------------------------------------------*/
extern/*static*/ int	now_max_continue;
extern void game_clear_set_password(void);
extern void player_loop_quit(void);
void add_enemy_all_clear(STAGE_DATA *l)
{
	now_max_continue=1;
	{
		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		//	error(ERR_DEBUG,"sorry, no more levels in this alpha-version");
	//	pd->score += pd->zanki*(score(2000)+(difficulty*score(4000)));
		player_add_score(adjust_score_by_difficulty((
					(pd->zanki * score(3000000)) +	/* 残機   x 300万 */
					(pd->bombs * score(1000000))  	/* 残ボム x 100万 */
				)));
	}
	game_clear_set_password();
	player_loop_quit();
}

	//return;
//	if (/**level*/player_now_stage==6	)
//	if (gt>125000000/*60500000*/)/*125.0[sec]←60.5[sec]*/
