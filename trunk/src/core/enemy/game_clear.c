
#include "game_main.h"

/*---------------------------------------------------------
	�G���f�B���O��̏I������
	-------------------------------------------------------

---------------------------------------------------------*/
extern/*static*/ int	now_max_continue;
extern void game_clear_set_password(void);
extern void player_loop_quit(void);
void add_enemy_all_clear(STAGE_DATA *l)
{
	{
		PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		//	error(ERR_DEBUG, "sorry, no more le vels in this alpha-version");
	//	pd->score += pd->zanki*(score(2000)+(difficulty*score(4000)));
		player_add_score(adjust_score_by_difficulty((
					(pd->zanki * score(3000000)) +	/* �c�@   x 300�� */
					(pd->bombs * score(1000000))  	/* �c�{�� x 100�� */
				)));
	}
	game_clear_set_password();
	#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
	now_max_continue = 1; 	/* �R���e�B�j���[�����Ȃ� */
	player_loop_quit();
	#endif
}

	//return;
//	if (/**le vel*/player_now_stage==6	)
//	if (gt>125000000/*60500000*/)/*125.0[sec]��60.5[sec]*/
