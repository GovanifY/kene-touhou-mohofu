
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔
	Mitei Madakimattenai.
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	ボス行動、第 1形態
---------------------------------------------------------*/
extern void add_zako_mima_dolls(SPRITE *src);
global void mima_01_keitai(SPRITE *src)
{
	static int mima_zako_tuika_timer = 0;	/* 雑魚、追加タイマー。追加間隔をあまり短くしない。 */
	if (0<mima_zako_tuika_timer)
	{
		mima_zako_tuika_timer--;
	}
	//
	static int mima_jikan;	/* カード追加タイマー。テキトー。 */
	mima_jikan--;
	/* カードを撃ってない場合に追加 */
//	if (SPELL_00==card.card_number) 	/* カード生成終了ならカード生成 */
	if (0 > mima_jikan) 				/* カード生成終了ならカード生成 */
	{
		const unsigned char aa_ra_nd = ra_nd();
		if (0==(aa_ra_nd&0x03))
		{
			if (0==mima_zako_tuika_timer)
			{
			//	mima_zako_tuika_timer = (128);
				mima_zako_tuika_timer = (256);
				add_zako_mima_dolls(src);
			}
		}
		mima_jikan = /*(123)*/(64) + (aa_ra_nd&0x3f);
		//
		src->BOSS_DATA_05_boss_base_state777++;
		src->BOSS_DATA_05_boss_base_state777 &= (8-1);
	//	if (0!=(src->BOSS_DATA_05_boss_base_state777))/*(r32)*/
		if (0!=(aa_ra_nd & 0xc0))/*(r33)*/
		{
			static const POINT_u16 hosi_no_zahyou[(8)] = /* 目標座標 */
			{	/* {(x),(y)}, */
				{(171),( 18)},	/* 南斗水鳥星へ */
				{(202),( 92)},	/* 北斗七星(5)へ */
				{(257),(107)},	/* 北斗七星(6)へ */
				{(339),( 74)},	/* 北斗七星(7)へ */
				{(	5),(116)},	/* 北斗七星(1)へ */
				{(	6),( 46)},	/* 北斗七星(2)へ */
				{(107),( 26)},	/* 北斗七星(3)へ */
				{(133),( 78)},	/* 北斗七星(4)へ */
			};
			/* 移動座標を決める */
			src->BOSS_DATA_00_target_x256 = ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].x)<<8);
			src->BOSS_DATA_01_target_y256 = ((hosi_no_zahyou[src->BOSS_DATA_05_boss_base_state777].y)<<8);
			/* カードを決める */
			card_maikai_init_and_get_spell_number(src); 	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
		}
		else
		{
			/* 移動座標を決める */
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			src->BOSS_DATA_00_target_x256 = (zzz_player->cx256);
			#if 0
		//	if (t256(16.0) > (src->BOSS_DATA_01_target_y256))
			if ( (((zzz_player->cy256)-t256(16.0))>>16) )
			{
				src->BOSS_DATA_01_target_y256 = t256(16.0);
			}
			else
			#endif
			{
				src->BOSS_DATA_01_target_y256 = (zzz_player->cy256);
		//		src->BOSS_DATA_01_target_y256 -= t256(8.0); 	/* ??? 何故か巧くいかない。 */
			}
			#if 1
			/* カードを決める */
			card.card_number	= SPELL_19_mima_sekkin; 	/* カードをセット */
			card_maikai_init(src);/*(r35-, カードの初期化。カードが変わると毎回行う必要がある)*/
			#endif
		}
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);	/* 攻撃アニメーション */
	}
	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}
