#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
//#include "spell_card_value.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/
extern unsigned int alice_anime_count;

extern void move_alice(SPRITE *src);

/* 目標を設定し、誘導移動 */
extern void alice_yuudou_move_only(SPRITE *src);	/* 移動のみ */
extern void alice_yuudou_calc(SPRITE *src); 		/* 誘導計算のみ */

#if 1
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
	#define boss_base_state777	user_data03 	/* 制限時間(boss_time_outと同じ) */
#endif


//extern void add_zako_alice_doll(SPRITE *src);/* アリス人形弾幕 */

#endif /* _BOSS_H_ */
