
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���o�p�e�L�X�g
---------------------------------------------------------*/

#include "kanji_system.h"

/* �Q�[�����C�x���g���b�Z�[�W�\�� */
global void add_enemy_kanji_string(STAGE_DATA *l)
{
	if (0==l->user_x)
	{
		kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
		home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	}
	msg_time = (60*5);
//
	char *text;
	text = l->user_string;
	print_kanji000(text, /*int color_type*/7, /*int wait*/0);
}
