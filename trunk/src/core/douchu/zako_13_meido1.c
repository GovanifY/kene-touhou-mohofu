
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"メイド1",		"RWINGX",	rwingx
	-------------------------------------------------------
	上から横に等間隔で整列して現れ、プレイヤーに向かい
	ある程度近づくと、弾置いて反転して逃げる。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define time_out	user_data03
#define speed256	user_data04


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido1(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if (0 > src->time_out)
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
	}
	else
	if (1024 > src->time_out)
	{
		src->speed256	+= t256(0.1);/*加速しながら プレイヤーに突っ込んでくる*/
		if (src->y256 >= t256(GAME_HEIGHT-150))
		{
		//	if (src->enemy_rank)
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= t256(2.8);				/* 弾速 */	 // //t256(1.0)+t256(src->enemy_rank)/*t256(3)+t256(src->enemy_rank)*/	/*速過ぎ*/
			//	br.BULLET_REGIST_speed256			= speed256; 				/* 弾速 */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* ダミー角度(未使用) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [赤弾] */ /* 弾グラ */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			src->time_out = 1023;
			src->tmp_angleCCW1024 += (1024/2);	/* (1024/2) == [180/360 度] 反転して逃げる */
			mask1024(src->tmp_angleCCW1024);
		//	src->speed256 = /*t256(6+src->enemy_rank)*/;/*速過ぎ*/
		}
	}
	else
	if (2048 > src->time_out)
	{
		/* プレイヤーに突っ込んでくる / schnell richtung player */
		src->time_out = 2047;
		tmp_angleCCW1024_jikinerai(obj_player, src);
	//	src->speed256	= /*t256(3+(src->enemy_rank>>1))*/ /*(4+src->enemy_rank)*/;/*速過ぎ*/
	}
	else
	if (2049 > src->time_out)/* 2048+1 */
	{
		if (0 < src->speed256)
		{
			src->speed256 -= (5);/*t256(0.1)*/ /*減速*/
		}
	}
	else
	if (2109 > src->time_out)/* 2048+1+60 */
	{
		if ( src->speed256 < t256(1.0))
		{
			src->speed256 += (5);/*t256(0.1)*/ /*加速*/
		}
	}
//
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
	src->x256 += ((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	src->y256 += ((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_meido1(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_12_MEIDO1*/
		h->callback_mover		= move_meido1;
	//
		h->tmp_angleCCW1024 	= (0);/* 下向き */		/* CCWの場合 */
	//
		h->speed256 			= t256(0);//t256(0.5);
		h->time_out 			= (2209);//(100);//20;
	}
}
