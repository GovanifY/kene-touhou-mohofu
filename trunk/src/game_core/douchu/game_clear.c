
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�G���f�B���O��̏I������
	-------------------------------------------------------
---------------------------------------------------------*/
extern void ini_file_save(void);

#if (0)/*(���p�X���[�h�@�\�B����)*/
	/*static*/ char str_pass_word[32];/*20*/
static void game_clear_set_password(void)
{
	#if (1)
	/******�����ŃR���p�C������l��******
	���̂܂܂��Ɣz�z�o�C�i���ƈႤ�̂�
	������"����"��K���ɕς��Ƃ��ĉ������B
	************************************/
	/* �`���h (chilled) �́A��p����邱�ƁB
	�u��₷�v���Ӗ�����p��̓����`�� (chill) �̉ߋ������ł���B
	���� chillno ����Ȃ��� cirno �Ȃ񂾂�ȁH */
	#define YUYUKO_PASSWORD 	"TABERARENAINO9"
	#define CIRNO_PASSWORD		"CIRNO9PASSWORD"
	#define REMILIA_PASSWORD	"RUMIAISMYBRIDE"
	if (0 < difficulty)/* NORMAL �ȏ�ŃN���A���� */
	{
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) /* ���ɃN���A���Ă��� */
		{
		/*	strcpy(str_pass_word, FURAN_PASSWORD)*/;	/* ??? �� �H�X�q �� �`���m �� ���~���A �J�� */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))	/* ���ɃN���A���Ă��� */
		{
			strcpy(str_pass_word, YUYUKO_PASSWORD); 	/* �H�X�q �� �`���m �� ���~���A �J�� */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD)) /* ���ɃN���A���Ă��� */
		{
			strcpy(str_pass_word, CIRNO_PASSWORD);		/* �`���m �� ���~���A �J�� */
		}
		else
		{
			strcpy(str_pass_word, REMILIA_PASSWORD);	/* ���~���A �J�� */
		}
	}
	#endif
	{
		ini_file_save();
	}
}
#else
	#define game_clear_set_password ini_file_save
#endif

extern void player_loop_quit(void);
global void game_command_04_game_all_clear(GAME_COMMAND *l)
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
