
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリス・マーガトロイド
	Alice Margatroid.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
#if 0
//extern const u8 alice_danmaku_table[16];
static const u8 alice_danmaku_table[16] =
{
	DANMAKU_07_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_08_rumia,
	DANMAKU_0b_alice_doll,	//DANMAKU_04_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_09_alice,
	DANMAKU_0b_alice_doll,	//DANMAKU_05_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_0a_houka_kenran/*DANMAKU_09_alice*/,
	DANMAKU_0b_alice_doll,	//DANMAKU_06_sakuya,
	DANMAKU_0b_alice_doll,	//DANMAKU_09_alice,
//
	DANMAKU_07_sakuya,
	0,
	DANMAKU_01_sakuya,
	DANMAKU_02_sakuya,
	DANMAKU_03_sakuya,
	DANMAKU_04_sakuya,
	DANMAKU_05_sakuya,
	DANMAKU_06_sakuya,
};
#endif
/*static*/ void alice_01_keitai(SPRITE *src)
{
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		alice_anime_count = 48;
		src->boss_base_danmaku_type 	= DANMAKU_09_alice; 	/*DANMAKU_01*/	/* 禊弾幕をセット */
	//	src->boss_base_danmaku_type 	= DANMAKU_13_perfect_freeze; 	/*DANMAKU_01*/	/* 禊弾幕をセット */
		danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
		src->vvv256 = t256(1.0);
	}
//
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}
/*static*/ void alice_02_keitai(SPRITE *src)
{
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		alice_anime_count = 48;
	//	src->boss_base_danmaku_type 	= DANMAKU_09_alice; 	/*DANMAKU_01*/	/* 禊弾幕をセット */
		src->boss_base_danmaku_type 	= DANMAKU_13_perfect_freeze; 	/*DANMAKU_01*/	/* 禊弾幕をセット */
		danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
	}
//
	static int my_wait;
	my_wait--;/*fps_factor*/
	{
		if (my_wait <= 0)
		{
			my_wait 	= (300);	/* 移動時間、間隔 */
			int rand32 = ((ra_nd()));
			/* 移動座標を決める */
			src->target_x256 = ((rand32&0xff00)+(((rand32<<8)&0x3f00)));	/* (256+64) == 320 == (480-128-32) */
			src->target_y256 = (((rand32>>8)&0x3f00));	/* (64) */
			/* 攻撃アニメーション */
			src->vvv256 = t256(1.0);
		}
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

/*static*/ void alice_03_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
	{
		if (my_wait <= 0)
		{	/* 移動方向を決める */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
					PPP_00_TX256 = 0,			/* x ベクトル移動量 */
					PPP_01_TY256,				/* y ベクトル移動量 */
					PPP_02_WAIT_COUNT,			/* ウェイトカウンタの量 */
					PPP_03_IS_RESET_ANIME,		/* アニメーションリセット 0:しない 1:する 2:特別(弾幕撃ち) */
				};
				static const u16 ppp[8][4] =
				{
					{(256),(20),(10*8),( 1),},	/*右上へ*/
					{(256),(20),( 5*8),( 2),},	/*wait*/
					{(320),(80),(10*8),( 1),},	/*右下へ*/
					{(320),(80),( 1*8),( 0),},	/*wait*/
					{(128),(70),(10*8),( 1),},	/*左上へ*/
					{(128),(70),( 5*8),( 2),},	/*wait*/
					{(160),(30),(10*8),( 1),},	/*左下へ*/
					{(160),(30),( 1*8),( 0),},	/*wait*/
				};
				src->target_x256 = (ppp[src->boss_base_state001][PPP_00_TX256]<<8);
				src->target_y256 = (ppp[src->boss_base_state001][PPP_01_TY256]<<8);
				my_wait = ppp[src->boss_base_state001][PPP_02_WAIT_COUNT];		/* 移動量 */
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) 	/* 攻撃アニメーション */
					{
						alice_anime_count = 48;

						src->boss_base_danmaku_type 	= DANMAKU_0b_alice_doll;	/*DANMAKU_01*/	/* 禊弾幕をセット */
						danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
					}
					src->vvv256 = t256(1.0);
				}
			}
		}
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_hamidasi_check();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

