
//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃{�X
---------------------------------------------------------*/
/* from game_core.c */
extern void common_boss_put_items(SPRITE *src);
global void game_command_05_kaiwa_start_boss(GAME_COMMAND *l)
{
	//----[BOSS]
	SPRITE *h;
	h					= global_obj_boss;/*�P��{�l*/
//	h					= �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */		/* �d�l�o�O(?) */
	{
		regist_settei_common(l, h);/* ���{�X�Ƌ��p����K�v������B */
		/*(�Ē�`�̕K�v����)*/	h->callback_loser				= common_boss_put_items;
	}
	/*(��b�ǂ݂��݁A�J�n�w��)*/
	cg.state_flag |= STATE_FLAG_10_IS_LOAD_KAIWA_TXT;
}
