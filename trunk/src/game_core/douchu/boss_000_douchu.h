
//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃{�X
---------------------------------------------------------*/
/* from game_core.c */
extern void common_boss_put_items(OBJ *src);
global void game_command_05_kaiwa_start_boss(GAME_COMMAND *l)
{
	//----[BOSS]
	OBJ *h;
	h					= &obj99[OBJ_HEAD_01_0x0800_TEKI+TEKI_OBJ_00_BOSS_HONTAI];/*(�{�X�{��)*/
	{
		regist_settei_common(l, h);/* ���{�X�Ƌ��p����K�v������B */
		/*(�Ē�`�̕K�v����)*/	h->callback_loser				= common_boss_put_items;
	}
	#if (1==USE_r36_SCENE_FLAG)
	NEXT_SCENE;/*(���̏�ʂ֐ݒ�)*/
	#else
	/*(��b�ǂ݂��݁A�J�n�w��)*/
	cg.state_flag |= STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT;
	#endif
}
