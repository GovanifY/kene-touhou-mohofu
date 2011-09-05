
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "game_main.h"
#include "card_address.h"


/* 目標を設定し、誘導移動 */
extern void boss_set_new_position(OBJ *src); 	/*(誘導移動座標を設定)*/

/* 時間切れの場合の透明度設定 */
extern void set_timeout_alpha(OBJ *src);

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


//廃止	#define bo ss_spell_timer		user_data05 	/* スペル経過時間 */
// REG_10_BOSS_SPELL_TIMER へ移行。
#endif


extern void add_zako_sakuya_doll_02_tama_oki(OBJ *src);/* 珠置きカード */


/*static*/extern void add_zako_sakuya_doll_01_laser(OBJ *src);/* レーザー */

/*static*/extern void add_zako_sakuya_doll_03_star(OBJ *src);/* 子供魔方陣(星型を描く) */
/*static*/extern void add_zako_sakuya_doll_04_360knife(OBJ *src);/* 子供魔方陣(360ナイフを撃つ) */
#endif /* _BOSS_H_ */

#ifndef _TUKAIMA_SYSTEM_H_
#define _TUKAIMA_SYSTEM_H_
extern void tukaima_system_add_dolls(OBJ *src);/*(使い魔システム)*/
//
extern void init_set_dolls_kaguya_T01(OBJ *src);
extern void init_set_dolls_kaguya_T02(OBJ *src);
extern void init_set_dolls_kaguya_T03(OBJ *src);
extern void init_set_dolls_mima_T04(OBJ *src);
extern void init_set_dolls_sakuya_T05(OBJ *src); 	/* 咲夜 ばら撒き2弾 オプション */
extern void init_set_dolls_sakuya_T06(OBJ *src); 	/* 咲夜 ばら撒き1弾 オプション */
//tern void init_set_dolls_pache_T07(OBJ *src);  	/*(未作成)*/

enum
{
	TUKAIMA_00_OFF = 0,
	TUKAIMA_01_kaguya_T01,
	TUKAIMA_02_kaguya_T02,
	TUKAIMA_03_kaguya_T03,
	TUKAIMA_04_mima_T04,
	TUKAIMA_05_sakuya_T05,
	TUKAIMA_06_sakuya_T06,
	TUKAIMA_07_pache_T07,/*(未使用)*/
	TUKAIMA_08_MAX/*(最大数)*/
};

#endif /* _TUKAIMA_SYSTEM_H_ */
