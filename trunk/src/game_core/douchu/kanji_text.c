
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���o�p�e�L�X�g
---------------------------------------------------------*/

#include "kanji_system.h"

/* �Q�[�����C�x���g���b�Z�[�W�\�� */
global void game_command_00_kanji_hyouji(GAME_COMMAND *l)
{
	#if (1)/*(r36�g���ĂȂ��̂Ŕp�~���邩��)*/
	if ((255)!=l->user_255_code)
	{
		return; 	/* error!! */
	}
	if (0==l->user_x)
	{
		kanji_window_all_clear();	/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	}
	cg.msg_time = byou60(5);	/* �� 5 �b */
//
	char *text;
	text = l->user_string;
	strcpy(my_font_text, text);
	kanji_color(7);
	kanji_draw();
	#endif
}

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�B���{�[�i�X�`�F�b�N
---------------------------------------------------------*/
static int kakusi_check;

/*---------------------------------------------------------
	�B���{�[�i�X�`�F�b�N�I���B���ʕ\���B
	-------------------------------------------------------
	�V�[�N���b�g��ԏI���B���ʕ\���B
---------------------------------------------------------*/
global void game_command_03_check_secret_bonus( 	GAME_COMMAND *l)
{
	int secret_bonus;
	if (kakusi_check != cg.player_data_used_bomber)
	{
		/* �{���g�p������A�{�[�i�X�����B */
		secret_bonus = (0);
	}
	else
	{
		int secret_score_table[4] =
		{
			score( 100000),/*(easy     10��[pts])*/
			score( 500000),/*(normal   50��[pts])*/
			score(1000000),/*(hard    100��[pts])*/
			score(5000000),/*(lunatic 500��[pts])*/
		};
		secret_bonus = secret_score_table[((cg.game_difficulty)/*&0x03*/)];
	}
	/* */
	player_dummy_add_score(secret_bonus);
}

/*---------------------------------------------------------
	�B���{�[�i�X�`�F�b�N�J�n�B
	-------------------------------------------------------
	�V�[�N���b�g��ԊJ�n
---------------------------------------------------------*/
global void game_command_04_begin_secret_bonus( 	GAME_COMMAND *l)
{
	kakusi_check = cg.player_data_used_bomber;/* �J�n���̃{���g�p�����o���Ă��� */
}

