
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
//#include "card_address.h"

extern void common_boss_move(SPRITE *src);

/* 目標を設定し、誘導移動 */
extern void boss_yuudou_idou_nomi(SPRITE *src); 		/* 誘導移動のみ */
extern void boss_yuudou_hiritu_keisan(SPRITE *src); 	/* 誘導計算のみ */

/* 時間切れの場合の透明度設定 */
extern void set_timeout_alpha(SPRITE *src);

#if 1
/* ボス共通規格 */
	#define BOSS_DATA_00_target_x256					user_data00 	/* 目標x座標 */
	#define BOSS_DATA_01_target_y256					user_data01 	/* 目標y座標 */
//
// バグ対策。 user_data02 良くワカンナイけど空けとく。()
//
	#define BOSS_DATA_03_kougeki_anime_count			user_data03 	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
	#define BOSS_DATA_04_toutatu_wariai256				user_data04 	/* 目標座標への到達割合 */
//
	#define BOSS_DATA_05_move_jyumyou					user_data05 	/* 制限時間 */
	#define BOSS_DATA_05_boss_base_state777 			BOSS_DATA_05_move_jyumyou	/* 制限時間(boss_move_timerと同じ) */
//

	/*(咲夜)*/
	#define BOSS_DATA_06_sakuya_data_move_angle1024 	user_data06 	/* 咲夜の角度 */
	#define BOSS_DATA_07_sakuya_data_common_wait256 	user_data07 	/* 咲夜の */
	/*(文)*/
	#define BOSS_DATA_06_aya_atooidan_type				user_data06 	/* 後追い弾の種類 */


//廃止	#define bo ss_spell_timer		user_data05 	/* スペル経過時間 */
// REG_10_BOSS_SPELL_TIMER へ移行。
#endif

extern void add_zako_alice_doll(SPRITE *src);/* アリス人形カード */
extern void add_zako_sakuya_doll_02_tama_oki(SPRITE *src);/* 珠置きカード */


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
//#include "card_address.h"

extern void lose_tukaima_00(SPRITE *src);
extern void move_tukaima00(SPRITE *src);

#endif /* _BOSS_OPTION_H_ */
