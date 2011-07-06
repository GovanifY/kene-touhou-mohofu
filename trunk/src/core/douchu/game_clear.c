
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�G���f�B���O��̏I������
	-------------------------------------------------------
---------------------------------------------------------*/
extern/*static*/ int	now_max_continue;
extern void game_clear_set_password(void);
extern void player_loop_quit(void);
global void add_enemy_all_clear(STAGE_DATA *l)
{
	#if 0
	/* �Ȃ񂩒m��񂪁ALunatic�ł�����ƃ`�F�b�N������A�I�[�o�[�t���[���邗�B */
	player_dummy_add_score(adjust_score_by_difficulty((
				(pd.zanki * score(3000000)) +	/* �c�@   x 300�� */
				(pd.bombs * score(1000000)) 	/* �c�{�� x 100�� */
			)));
	#else
	/* �Ȃ񂩒m��񂪁A�I�[�o�[�t���[����̂ŁA�ʂɑ����Ă݂�B */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)pd.zanki) * score(3000000)) ))); 	/* �c�@   x 300�� */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)pd.bombs) * score(1000000)) ))); 	/* �c�{�� x 100�� */
	#endif
	/* ���Z���ď����� */
	pd.zanki = 0;
	pd.bombs = 0;
	//
	game_clear_set_password();
	#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
	now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
	player_loop_quit();
	#endif
}
