
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
//#include "spell_card_value.h"

extern void common_boss_move(SPRITE *src);

/* 目標を設定し、誘導移動 */
extern void boss_yuudou_idou_nomi(SPRITE *src);  		/* 誘導移動のみ */
extern void boss_yuudou_hiritu_keisan(SPRITE *src); 	/* 誘導計算のみ */

/* 時間切れの場合の透明度設定 */
extern void set_timeout_alpha(SPRITE *src);

#if 1
/* ボス共通規格 */
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
//
	#define boss_spell_timer		user_data05 	/* スペル時間 */
#endif

extern void add_zako_alice_doll(SPRITE *src);/* アリス人形弾幕 */
extern void add_zako_sakuya_doll_02_tama_oki(SPRITE *src);/* 珠置き弾幕 */


/*static*/extern void add_zako_sakuya_doll_01_laser(SPRITE *src);/* レーザー */

/*static*/extern void add_zako_sakuya_doll_03_star(SPRITE *src);/* 子供魔方陣(星型を描く) */
/*static*/extern void add_zako_sakuya_doll_04_360knife(SPRITE *src);/* 子供魔方陣(360ナイフを撃つ) */
#endif /* _BOSS_H_ */

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_OPTION_H_
#define _BOSS_OPTION_H_

//#include "game_main.h"
//#include "spell_card_value.h"

extern void lose_option_00(SPRITE *src);


#endif /* _BOSS_OPTION_H_ */
