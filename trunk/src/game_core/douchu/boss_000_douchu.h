
//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃{�X
---------------------------------------------------------*/
/* from game_core.c */
//extern OBJ_CALL_FUNC(common_bo ss_put_items);
global void game_command_02_kaiwa_start_boss(GAME_COMMAND *l)
{
	//----[BOSS]
	OBJ *h;
	h					= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
	regist_settei_common(l, h);/* ���{�X/(boss)�Ƌ��p����K�v������B(h->callback_loser���ݒ肳���) */
//[��������]	/*(�Ē�`�̕K�v����)*/	h->callback_loser				= common_bo ss_put_items;
	NEXT_SCENE;/*(���̏�ʂ֐ݒ�)*/
}

