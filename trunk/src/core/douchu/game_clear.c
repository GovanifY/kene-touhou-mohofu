
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�G���f�B���O��̏I������
	-------------------------------------------------------
---------------------------------------------------------*/

extern void game_clear_set_password(void);
extern void player_loop_quit(void);
global void game_command_01_game_all_clear(GAME_COMMAND *l)
{
	#if 0
	/* �Ȃ񂩒m��񂪁ALunatic�ł�����ƃ`�F�b�N������A�I�[�o�[�t���[���邗�B */
	player_dummy_add_score(adjust_score_by_difficulty((
				(cg.zanki * score(3000000)) +	/* �c�@   x 300�� */
				(cg.bombs * score(1000000)) 	/* �c�{�� x 100�� */
			)));
	#else
	/* �Ȃ񂩒m��񂪁A�I�[�o�[�t���[����̂ŁA�ʂɑ����Ă݂�B */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.zanki) * score(3000000)) ))); 	/* �c�@   x 300�� */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.bombs) * score(1000000)) ))); 	/* �c�{�� x 100�� */
	#endif
	/* ���Z���ď����� */
	cg.zanki = 0;
	cg.bombs = 0;
	//
	game_clear_set_password();
	#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
	cg.game_now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
	player_loop_quit();
	#endif
}
