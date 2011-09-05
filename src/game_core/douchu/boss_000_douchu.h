
//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のボス
---------------------------------------------------------*/
/* from game_core.c */
extern void common_boss_put_items(OBJ *src);
global void game_command_05_kaiwa_start_boss(GAME_COMMAND *l)
{
	//----[BOSS]
	OBJ *h;
	h					= &obj99[OBJ_HEAD_01_0x0800_TEKI+TEKI_OBJ_00_BOSS_HONTAI];/*(ボス本体)*/
	{
		regist_settei_common(l, h);/* 中ボスと共用する必要がある。 */
		/*(再定義の必要あり)*/	h->callback_loser				= common_boss_put_items;
	}
	#if (1==USE_r36_SCENE_FLAG)
	NEXT_SCENE;/*(次の場面へ設定)*/
	#else
	/*(会話読みこみ、開始指示)*/
	cg.state_flag |= STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT;
	#endif
}
