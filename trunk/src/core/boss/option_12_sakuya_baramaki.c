
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜 ばら撒き弾 オプション
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
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif


#define shot_angle1024		user_data04 	/* ショットを撃つ方向を保持 */


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_sakuya_baramaki(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_sakuya_baramaki(SPRITE *src)
{
	check_boss_option_time_out(src); 	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	#define AO_OR_AKA	(src->target_x256&1)/* [青赤情報] */
//
	/*	旧作 & 咲夜風、回転ショット */
	{
		if (0 == ((src->time_out)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			if (0 < src->vvv256 )
			{
				src->vvv256 -= (1);
			}
		//
			src->shot_angle1024 += ((AO_OR_AKA)?(-(1024/18)):((1024/18)));	/* ショットを撃つ方向を、回転させる。 */
		//
			obj_send1->x256 					= (src->x256)+t256(20.0);/* 6.0[dots]右 */
			obj_send1->y256 					= (src->y256)+t256(14.0);/* 4.0[dots]下 */
			br.BULLET_REGIST_speed256			= (t256(1.5))+(((difficulty)<<6));		/* 弾速 */
			br.BULLET_REGIST_angle1024			= (src->shot_angle1024);				/* */
			br.BULLET_REGIST_div_angle1024		= (int)(1024/160);						/* 密着弾 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_00_AOI+(AO_OR_AKA);	/* [青赤クナイ弾] */
			br.BULLET_REGIST_n_way				= (2+difficulty);						/* [2-5way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}
	#if 1
	/* 魔方陣アニメーション */
//	src->m_angleCCW1024--;/* 右回り */
	src->m_angleCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->m_angleCCW1024);
	#endif
	if (0x1ff > src->time_out)
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->time_out<<(23))&0xff000000);
	}
	/* オプション位置、移動 */
	/* 目標を設定し、誘導移動 */
//	alice_yuudou_move_only(src);
	alice_yuudou_calc(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_sakuya_baramaki(SPRITE *src)
{
	int i;
	for (i=0; i<(4); i++) /* 4つ */
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_sakuya_baramaki;
			h->callback_loser		= lose_sakuya_baramaki;
			h->callback_hit_enemy	= callback_hit_zako;
	//
			h->x256 				= ((src->x256) & 0xfffffffe);/* [青赤情報]インターリーブ用ビットを1ビット確保 */
			h->y256 				= (src->y256);
			h->vvv256				= t256(1.0);
		//
			{
				const s8 locate_xy_table[(4+4)] =
				{	/* 青 */	/* 赤 */	/* 青 */	/* 赤 */
					(-90),		(-120), 	(+120), 	(+90),	// x0, x1, x2, x3
					(+90),		(0),		(0),		(+90),	// y0, y1, y2, y3
				};
				h->target_x256	= (h->x256)+((locate_xy_table[i  ])<<8) + (i&1);/* [青赤情報]をインターリーブ */
				h->target_y256	= (h->y256)+((locate_xy_table[i+4])<<8);
			}
	//
			h->base_hp				= (8192-1);
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score( 100000), 	/* easy 10万 (計 50万==5x 10万) */
					score( 300000), 	/* norm 30万 (計150万==5x 30万) */
					score( 500000), 	/* hard 50万 (計250万==5x 50万) */
					score(1000000), 	/* luna100万 (計500万==5x100万) */
				};
			h->base_score			= difficulty_score_tbl[difficulty]; 	/* 難易度によるが、かなり稼げる。 */
		//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
			}
			h->shot_angle1024		= (0);
			h->time_out 			= (0x03ff); 	/* 制限時間 */
		}
	}
}
