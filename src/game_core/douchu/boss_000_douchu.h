
//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のボス
---------------------------------------------------------*/
/* from game_core.c */
extern void common_boss_put_items(SPRITE *src);
global void game_command_05_kaiwa_start_boss(GAME_COMMAND *l)
{
	//----[BOSS]
	SPRITE *h;
	h					= global_obj_boss;/*輝夜本人*/
//	h					= あたり判定の都合上無理&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
	{
		regist_settei_common(l, h);/* 中ボスと共用する必要がある。 */
		/*(再定義の必要あり)*/	h->callback_loser				= common_boss_put_items;
	}
	/*(会話読みこみ、開始指示)*/
	cg.state_flag |= STATE_FLAG_10_IS_LOAD_KAIWA_TXT;
}
