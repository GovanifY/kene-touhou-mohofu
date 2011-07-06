
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔
	Mitei Madakimattenai.
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
	#define boss_base_state777	user_data03 	/* 制限時間(boss_time_outと同じ) */
#endif


/*---------------------------------------------------------
	ボス行動、第 1形態
---------------------------------------------------------*/
extern void add_zako_mima_dolls(SPRITE *src);
global void mima_01_keitai(SPRITE *src)
{
	static int mima_jikan;
	mima_jikan--;
	/* 弾幕を撃ってない場合に追加 */
//	if (DANMAKU_00==src->boss_base_danmaku_type)		/* 弾幕生成終了なら弾幕生成 */
	if (0 > mima_jikan)		/* 弾幕生成終了なら弾幕生成 */
	{
		{
			const unsigned char aa_ra_nd = ra_nd();
			if (0==(aa_ra_nd&0x03))
			{
				add_zako_mima_dolls(src);
			}
			mima_jikan = /*123*/64 + (aa_ra_nd&0x3f);
		}
		//
		src->boss_base_state777++;
		src->boss_base_state777 &= (8-1);
		if (0==src->boss_base_state777)
		{
		//	if (1<difficulty)
			{
				/* 移動座標を決める */
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				src->target_x256 = (zzz_player->cx256);
				#if 0
			//	if (t256(16.0) > (src->target_y256))
				if ( (((zzz_player->cy256)-t256(16.0))>>16) )
				{
					src->target_y256 = t256(16.0);
				}
				else
				#endif
				{
					src->target_y256 = (zzz_player->cy256);
			//		src->target_y256 -= t256(8.0);	/* ??? 何故か巧くいかない。 */
				}
				#if 1
				/* 弾幕を決める */
				src->boss_base_danmaku_type 	= DANMAKU_19_mima_sekkin;	/* 弾幕をセット */
				danmaku_set_time_out(src);		/* 弾幕の制限時間を設定(予め弾幕ごとに設定されている標準時間に設定) */
				#endif
			}
		}
		else
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
			src->target_x256 = ((hosi_no_zahyou[src->boss_base_state777].x)<<8);
			src->target_y256 = ((hosi_no_zahyou[src->boss_base_state777].y)<<8);
			/* 弾幕を決める */
			spell_card_get_danmaku_number(src); 	/* スペカシステムから、現在の形態で撃つ弾幕番号を手に入れ、標準のboss_base_danmaku_time_out時間を設定。 */
		//	src->boss_base_danmaku_type 	= DANMAKU_0c_hana_test; 	/* 弾幕をセット */
		//	danmaku_set_time_out(src);		/* 弾幕の制限時間を設定(予め弾幕ごとに設定されている標準時間に設定) */
		}
		/* */
		src->vvv256 = t256(1.0);
	}
	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
	bullet_angle_all_gamen_gai_nara_kesu();/*角度弾の喰み出しチェックを行う(毎フレーム行う必要はない)*/
}
