
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	蓬莱山 輝夜
	Houraisan Kaguya.
	-------------------------------------------------------
---------------------------------------------------------*/

static void kaguya_common_shot_check(SPRITE *src)
{
	/* 移動方向を決める */
	src->BOSS_DATA_05_boss_base_state777++;
	src->BOSS_DATA_05_boss_base_state777 &= (4-1);
	#if (1)
	if ((/*(unsigned)*/REG_0f_GAME_DIFFICULTY) >= /*(unsigned)*/(src->BOSS_DATA_05_boss_base_state777) )/*(直前の論理ANDで必ずunsignedが保証されるからunsigned比較を使う)*/
	{
		card_maikai_init_and_get_spell_number(src);
	}
	#endif
}

/*---------------------------------------------------------
	ボス行動、第 4形態
	斜め移動
---------------------------------------------------------*/

global void kaguya_04_keitai(SPRITE *src)
{
	static int kaguya_BOSS_DATA_00_target_x256;
	static int aaa_wait;
	aaa_wait--;/*fps_factor*/
	if (aaa_wait <= 0)
	{
		aaa_wait = 8;
		/* 目標にちかづこうと努力。 */
		src->BOSS_DATA_00_target_x256 += ((src->BOSS_DATA_00_target_x256 < kaguya_BOSS_DATA_00_target_x256) ? t256(8) : -t256(8) );
		src->BOSS_DATA_00_target_x256 &= 0xffff;
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( (((src->BOSS_DATA_00_target_x256>>4)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
		src->BOSS_DATA_01_target_y256 = (t256(25)+( (sin_value_t256)<<6));
	}
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	}
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		/* 気分で目標を決める。 */
		kaguya_BOSS_DATA_00_target_x256 = ((ra_nd()&0xffff))+(t256(48));
		kaguya_common_shot_check(src);
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	ボス行動、第 1形態
	微上下移動
---------------------------------------------------------*/

global void kaguya_01_keitai(SPRITE *src)
{
	static int aaa_wait;
	aaa_wait += (10);
//	mask1024(aaa_wait);
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( ((((aaa_wait))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
		src->cy256 = (t256(25)+( ((sin_value_t256))<<2));
	}
//
	/* カードを撃ってない場合に追加 */
	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	{
		kaguya_common_shot_check(src);
	}
}
