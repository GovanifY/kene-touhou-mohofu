
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���o�p�e�L�X�g
---------------------------------------------------------*/

#include "kanji_system.h"

/* �Q�[�����C�x���g���b�Z�[�W�\�� */
global void game_command_00_kanji_hyouji(GAME_COMMAND *l)
{
	if ((255)!=l->user_255_code)
	{
		return; 	/* error!! */
	}
	if (0==l->user_x)
	{
		kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
		home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	}
	cg.msg_time = byou60(5);	/* �� 5 �b */
//
	char *text;
	text = l->user_string;
	print_kanji000(text, /*int color_type*/(7), /*int wait*/(0) );
}

