
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	パチュリー・ノーレッジ
	Patchouli Knowledge.
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	弾幕を撃ってない場合に追加
---------------------------------------------------------*/

static void pache_add_spell(SPRITE *src)
{
	/* 弾幕を撃ってない場合に追加 */
	if (SPELL_00==spell_card.spell_type)		/* 弾幕生成終了なら弾幕生成 */
	{
		spell_card.number_temporaly++;
		spell_card.number_temporaly &= 0x0f;
		#if 1
		static const u8 pache_spell_table[16] =
		{
			SPELL_07_sakuya,
			SPELL_0a_dai_yousei_01,
			SPELL_0b_dai_yousei_02,
			SPELL_08_rumia,
			//
			SPELL_04_sakuya_pink_hearts,	// ???
			SPELL_09_alice,
			SPELL_05_sakuya,
			SPELL_25_houka_kenran,		/* SPELL_09_alice */
			//
			SPELL_10_cirno, 			// SPELL_06_sakuya,	/* チルノ禊弾幕 */		// SPELL_09_alice,
			SPELL_18_hana_test, 		// SPELL_07_sakuya,
			0,							// 喘息で息切れ
			SPELL_01_sakuya_misogi_normal,
			//
			SPELL_02_sakuya_old_test,	/* この弾幕は、既に咲夜では使ってない(r32) */
			SPELL_03_sakuya_old_test,	/* この弾幕は、既に咲夜では使ってない(r32) */
			SPELL_11_perfect_freeze,	// SPELL_04_sakuya_pink_hearts, 	// SPELL_05_sakuya,
			SPELL_16_alice_doll,		// SPELL_06_sakuya,
		};
		#endif
		spell_card.spell_type	= pache_spell_table[spell_card.number_temporaly];	/* 弾幕をセット */
		spell_set_time_out(src);		/* 弾幕の時間切れを設定 */
//		src->toutatu_wariai256 = t256(1.0);
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
		if (src->cx256 > t256(GAME_WIDTH)-t256(48) )	{	src->target_x256 = -t256(256);}
		{
			if (0==(my_rand&0x04))
			{
				src->kougeki_anime_count = (48);	/* 攻撃アニメーション開始 */
			}
			src->toutatu_wariai256 = t256(1.0);
		}
	}
//
	pache_add_spell(src);/* 弾幕を撃ってない場合に追加 */
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
	bullet_angle_all_gamen_gai_nara_kesu();/* 角度弾の喰み出しチェックを行う(毎フレーム行う必要はない) */
}


/*---------------------------------------------------------
	ボス行動、第 1形態
---------------------------------------------------------*/

global void pache_01_keitai(SPRITE *src)
{
	pache_add_spell(src);/* 弾幕を撃ってない場合に追加 */
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
