
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風	〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"緑毛玉1",		"XEV",
	-------------------------------------------------------
	緑毛玉	単体で突っ込んでくる。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

	#define target_x256 		user_data00
	#define target_y256 		user_data01

	#define time_out			user_data03

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *src)
{
	int speed256;
	src->time_out--;
	if (0 > src->time_out)		/* 離脱 */
	{
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		speed256 = t256(3.5/*6*/);
	}
	else
	{
		if ((src->x256 >= obj_player->x256)||
			(src->y256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->time_out	= 0;
			src->target_x256	= t256(-100);
		//	src->target_y256	= obj_player->y256-t256(48);
			src->target_y256	= obj_player->y256-t256(48);
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= t256(2.75);				/* 弾速 */
			//	br.BULLET_REGIST_speed256			= speed256; 				/* 弾速 */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* ダミー角度(未使用) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [赤弾] */ /* 弾グラ */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			//
				lose_random_item(src);
			}//t256(1)+((4/*data->enemy_rank*/)<<8)
		}
		speed256 = t256(2.5/*3*/);
	}
	obj_send1->x256 	= (src->target_x256);					/* 弾源x256 */
	obj_send1->y256 	= (src->target_y256);					/* 弾源y256 */
	tmp_angleCCW1024_jikinerai(obj_send1, src);
	src->x256 += ((sin1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/	/* CCWの場合 */
	src->y256 += ((cos1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/
//
	src->m_angleCCW1024 += 5;
	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_midori_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_60_AKA_KEDAMA;
		h->callback_mover		= move_midori_kedama1;
	//
		h->target_x256			= obj_player->x256;
	//	h->target_y256			= obj_player->y256; 		/* 特攻禁止 */
		h->target_y256			= ((obj_player->y256)>>1);
		h->time_out 			= (320);/* テキトー */
	}
}
