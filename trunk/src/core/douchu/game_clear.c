
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

		//	error(ERR_DEBUG, "sorry, no more le vels in this alpha-version");
	//	pd_score += pd_zanki*(score(2000)+(difficulty*score(4000)));
	#if 0
	/* �Ȃ񂩒m��񂪁ALunatic�ł�����ƃ`�F�b�N������A�I�[�o�[�t���[���邗�B */
		player_add_score(adjust_score_by_difficulty((
					(pd_zanki * score(3000000)) +	/* �c�@   x 300�� */
					(pd_bombs * score(1000000))  	/* �c�{�� x 100�� */
				)));
	#else
	/* �Ȃ񂩒m��񂪁A�I�[�o�[�t���[����̂ŁA�ʂɑ����Ă݂�B */
		player_add_score(adjust_score_by_difficulty((	(((u32)pd_zanki) * score(3000000)) )));	/* �c�@   x 300�� */
		player_add_score(adjust_score_by_difficulty((	(((u32)pd_bombs) * score(1000000)) )));	/* �c�{�� x 100�� */
	#endif
		/* ���Z���ď����� */
		pd_zanki = 0;
		pd_bombs = 0;
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
