
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"毛玉1",	"BADGUY",
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

//
	#define target_x256 		user_data00
	#define target_y256 		user_data01
//
	#define time_out			user_data03
//
	#define speed256			user_data04
	#define state				user_data05
//

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	ST00 = 0,
//	ST01,
	ST02,
	ST03,
};

static void my_nerai(SPRITE *src)
{
	obj_send1->x256 	= (src->target_x256); 					/* 弾源x256 */
	obj_send1->y256 	= (src->target_y256-t256(64)/*+(20)*/);	/* 弾源y256 */
	tmp_angleCCW1024_jikinerai(obj_send1, src);
	src->vx256 = ((sin1024(src->tmp_angleCCW1024)));
	src->vy256 = ((cos1024(src->tmp_angleCCW1024)));
}

static void move_kedama1(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	switch (src->state)
	{
	case ST00:	/* 登場する */
		my_nerai(src);
		if ((src->x256 >= obj_player->x256) ||
			(src->y256 > t256(GAME_HEIGHT/2) ) || /*(480-80)*/ /*(272-32)*/
			(0 > src->time_out)
		 )
		{
			src->vy256 = (-src->vy256);
			src->state++;/* = ST02*/
			src->time_out	= 64;
			/* 第二目標は、 */
			src->target_x256		= t256(-100);
			src->target_y256		= t256(0);/*((obj_player->y256)&(128-1))*/
			src->speed256	= /*6*/t256(2.5/*5.0*/);/*fps_factor*/
		}
		break;/*??? [***090215 追加 */
	case ST02:
		if (0==(src->time_out & 0x0f))
		{
			if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
			{
				obj_send1->x256 					= src->x256;
				obj_send1->y256 					= src->y256;
				br.BULLET_REGIST_speed256			= t256(1.0);	//t256(1.0)+t256((src->enemy_rank)>>1),
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI+(src->x256&0x07);
				br.BULLET_REGIST_n_way				= (3+difficulty+difficulty);
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		if (0 > src->time_out)
		{
			src->state++;
		}
		break;
	case ST03:/* 退場 */
		my_nerai(src);
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		break;/*??? [***090215 追加 */
	}

	/* 移動する */
	src->x256+=(((src->vx256)*src->speed256)>>8);
	src->y256+=(((src->vy256)*src->speed256)>>8);
//
	src->m_angleCCW1024 += 10;
	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_59_HAI_KEDAMA;
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*白っぽく */
		h->callback_mover		= move_kedama1;
	//
		h->state				= ST00;
		h->time_out 			= 64;
		/* 第一目標はプレイヤーのいた位置 */
		h->target_x256				= obj_player->x256;
		h->target_y256				= obj_player->y256;
		h->vx256				= (0);
		h->vy256				= (0);
	//	h->speed256 			= ((ra_nd()&((256*2)-1)))*(1+difficulty+enemy_rank/3);/*fps_factor*/
		h->speed256 			= ((ra_nd()&((256*2)-1))) + ((difficulty<<8)+ (/*enemy_rank*/5<<7/**86*2*/) );/*fps_factor*/
	}
}
