
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	パチュリー・ノーレッジ
	Patchouli Knowledge.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif


/*---------------------------------------------------------
	弾幕を撃ってない場合に追加
---------------------------------------------------------*/

static void pache_add_danmaku(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		src->boss_base_danmaku_test++;
		src->boss_base_danmaku_test &= 0x0f;
		#if 1
		static const u8 pache_danmaku_table[16] =
		{
			DANMAKU_07_sakuya,
			DANMAKU_0a_dai_yousei_01,
			DANMAKU_0b_dai_yousei_02,
			DANMAKU_08_rumia,
			//
			DANMAKU_04_sakuya,				//???
			DANMAKU_09_alice,
			DANMAKU_05_sakuya,
			DANMAKU_25_houka_kenran,		/*DANMAKU_09_alice*/
			//
			DANMAKU_10_cirno,				//DANMAKU_06_sakuya,	/* チルノ禊弾幕 */		//	DANMAKU_09_alice,
			DANMAKU_18_hana_test,			//DANMAKU_07_sakuya,
			0,								// 喘息で息切れ
			DANMAKU_01_sakuya,
			//
			DANMAKU_02_sakuya_old_test, 	/* この弾幕は、既に咲夜では使ってない(r32) */
			DANMAKU_03_sakuya_old_test, 	/* この弾幕は、既に咲夜では使ってない(r32) */
			DANMAKU_11_perfect_freeze,		//DANMAKU_04_sakuya,		//	DANMAKU_05_sakuya,
			DANMAKU_16_alice_doll,			//DANMAKU_06_sakuya,
		};
		#endif
		src->boss_base_danmaku_type 	= pache_danmaku_table[src->boss_base_danmaku_test]; 	/* 弾幕をセット */
		danmaku_set_time_out(src);		/* 弾幕の時間切れを設定 */
//		src->vvv256 = t256(1.0);
//		alice_anime_count = 48;
	}
}


/*---------------------------------------------------------
	ボス行動、第 4形態
---------------------------------------------------------*/

global void pache_04_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
	if (my_wait <= 0)
	{	/* 移動方向を決める */
		u16 my_rand;	my_rand = ra_nd();
		my_wait 	= (my_rand & 0xff); 	/* 50*4 60 移動量 */
		//
		src->target_y256 = (((my_rand)&0x7f00));
		src->target_x256 = ((my_rand&0x1ff)<<8);
		if (src->cx256 > t256(GAME_WIDTH)-t256(48) ) 	{	src->target_x256 = -t256(256);}
		{
			if (0==(my_rand&0x04))
			{
				alice_anime_count = 48;
			}
			src->vvv256 = t256(1.0);
		}
	}
//
	pache_add_danmaku(src);/* 弾幕を撃ってない場合に追加 */
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}


/*---------------------------------------------------------
	ボス行動、第 1形態
---------------------------------------------------------*/

global void pache_01_keitai(SPRITE *src)
{
	pache_add_danmaku(src);/* 弾幕を撃ってない場合に追加 */
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
#if 0
global void bullet_regist_multi_fix(void);/* パチェレーザー用特殊初期化 */
global void boss_init_pache(SPRITE *h)
{
//	bullet_regist_multi_fix();/* パチェレーザー用特殊初期化 */
}
#endif
