
//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のボス
---------------------------------------------------------*/
/* from game_core.c */
//extern OBJ_CALL_FUNC(common_bo ss_put_items);
global void game_command_02_kaiwa_start_boss(GAME_COMMAND *l)
{
	//----[BOSS]
	OBJ *h;
	h					= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(ボス本体)*/
	regist_settei_common(l, h);/* 中ボス/(boss)と共用する必要がある。(h->callback_loserが設定される) */
//[強制共通]	/*(再定義の必要あり)*/	h->callback_loser				= common_bo ss_put_items;
	NEXT_SCENE;/*(次の場面へ設定)*/
}

