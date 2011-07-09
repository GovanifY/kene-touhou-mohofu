
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜 ばら撒き1弾 オプション
	-------------------------------------------------------
	ばら撒き 速度遅い
	寿命	 長い
	-------------------------------------------------------
	めも1:密着米(5)弾
		青米弾					緑米弾
		右回り					左回り
		(+90度) 				(-45度)

			緑米弾			青米弾
			左回り			右回り
			(+45度) 		(0度)
	-------------------------------------------------------
	めも2:密着クナイ(6)弾	[横の壁で反射する]
		赤クナイ弾				青クナイ弾
		右回り					左回り

			青クナイ弾		赤クナイ弾
			左回り			右回り
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
//
	#define boss_spell_timer		user_data05 	/* スペル時間 */
#endif


#define shot_angle1024		user_data06 	/* ショットを撃つ方向を保持 */


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_sakuya_baramaki1(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	#define AO_OR_AKA	(src->target_x256&1)/* [青赤情報] */
//
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->boss_time_out)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->boss_time_out)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			if (0 < src->toutatu_wariai256 )
			{
				src->toutatu_wariai256 -= (1);
			}
		//
			src->shot_angle1024 += ((AO_OR_AKA)?(-(1024/18)):((1024/18)));	/* ショットを撃つ方向を、回転させる。 */
		//
			send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
			br.BULLET_REGIST_00_speed256				= (t256(1.5))+((((cg_game_difficulty))<<6));		/* 弾速(r32/r33) */
		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI+(AO_OR_AKA)+(AO_OR_AKA);	/* [青緑米弾] */
			/* ここは 6 wayではなくて、 3 way を2回追加する。でないとプライオリティーが変になる。 */
		//	br.BULLET_REGIST_06_n_way					= (2+difficulty);						/* [2-5way](r32) */
			br.BULLET_REGIST_06_n_way					= (3);									/* [5way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/160);						/* 密着弾(r32)6.4 */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/256);						/* 密着弾(もっと密着)4 */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/512);						/* 密着弾(もっと密着)2 */
			int jj;
			for (jj=0; jj<2; jj++)
			{
				const int kakusan_tbl[4] =
				{(int)(1024/512), (int)(1024/512), (int)(1024/256), (int)(1024/128)};		// Lunatic はかなり拡散する。
				br.BULLET_REGIST_07_VECTOR_div_angle1024		= kakusan_tbl[(cg_game_difficulty)];						/* 密着弾(もっと密着)2 */
				if (0!=jj)	{br.BULLET_REGIST_07_VECTOR_div_angle1024	-=br.BULLET_REGIST_07_VECTOR_div_angle1024;}	/* 負方向 */
				br.BULLET_REGIST_02_VECTOR_angle1024			= (src->shot_angle1024)+(br.BULLET_REGIST_07_VECTOR_div_angle1024); 			/* */
				bullet_regist_multi_vector_direct();
			}
		}
	}
	#if 1
	/* 魔方陣アニメーション */
//	src->rotationCCW1024--;/* 右回り */
	src->rotationCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->rotationCCW1024);
	#endif
	set_timeout_alpha(src);
	/* オプション位置、移動 */
	/* 目標を設定し、誘導移動 */
//	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_sakuya_baramaki1(SPRITE *src)
{
	int i;
	for (i=0; i<(4); i++) /* 4つ */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_sakuya_baramaki1;
			h->callback_loser		= lose_option_00;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->cx256				= ((src->cx256) & 0xfffffffe);/* [青赤情報]インターリーブ用ビットを1ビット確保 */
			h->cy256				= (src->cy256);
			h->toutatu_wariai256				= t256(1.0);
		//
			{
				const s8 locate_xy_table[(4+4)] =
				{	/* 青 */	/* 赤 */	/* 青 */	/* 赤 */
				//	(-90),		(-120), 	(+120), 	(+90),	// x0, x1, x2, x3 (r32)
				//	(+90),		(0),		(0),		(+90),	// y0, y1, y2, y3 (r32)
					(-64),		(-100), 	(+100), 	(+64),	// x0, x1, x2, x3 (r33)
					(+64),		(0),		(0),		(+64),	// y0, y1, y2, y3 (r33)
				};
				h->target_x256	= (h->cx256)+((locate_xy_table[i  ])<<8) + (i&1);/* [青赤情報]をインターリーブ */
				h->target_y256	= (h->cy256)+((locate_xy_table[i+4])<<8);
			}
	//
			h->base_hp				= (8192-1);
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score(	10000), 	/* easy 1万 (計 5万==5x 1万) (r33) */
					score(	30000), 	/* norm 3万 (計15万==5x 3万) (r33) */
					score(	50000), 	/* hard 5万 (計25万==5x 5万) (r33) */
					score( 100000), 	/* luna10万 (計50万==5x10万) (r33) */
				};
			h->base_score			= difficulty_score_tbl[(cg_game_difficulty)];	/* 難易度によるが、かなり稼げる。 */
		//	h->base_score			= adjust_score_by_difficulty(score( 50000));	/* 5万 (計30万==6x5万) (r33) */
			}
			h->shot_angle1024		= (0);
		//	h->boss_time_out		= (0x03ff); 	/* 制限時間 (r32) */
			h->boss_time_out		= (0x03ff); 	/* 制限時間 (r33) */
		}
	}
}
