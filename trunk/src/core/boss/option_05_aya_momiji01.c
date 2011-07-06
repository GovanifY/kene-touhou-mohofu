
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	もみじ弾幕
	-------------------------------------------------------
---------------------------------------------------------*/

#define time_out						user_data02 						/* 経過時間 */
#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024					/* 子供弾、生成角度 */


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/extern void add_zako_aya_5_momiji(SPRITE *src);
static void move_aya_momiji_oya(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if ((0 > src->time_out))
	{
		add_zako_aya_5_momiji(src); 	/* 5方向もみじ小弾を生成 */
		src->jyumyou = JYUMYOU_NASI;
	}
	src->cx256 += (/*data->v*/src->vx256);/*fps_factor*/
	src->cy256 += (/*data->v*/src->vy256);/*fps_factor*/
//
	/* 親が画面外なら分裂しない */
	gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ void add_zako_aya_doll(SPRITE *src)
{
	int angle1024;
//	for (angle1024=(int)((1024)-(1024*2/24)); angle1024<(int)((1024)+(1024*3/24)); angle1024+=(int)(1024*1/24) )
	for (angle1024=(0); angle1024<(1024); angle1024+=(int)(1024*1/24) )
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
		//	h->type 				= BULLET_MARU8_00_AKA+(1);/*1==とりあえず*/ /*S P_BULLET*/ /*SP_LASER*/
		//	h->type 				= TEKI_24_YOUSEI2_1+((angle512>>6)&0x07);/* 0 ... 8 */
			h->type 				= TEKI_24_YOUSEI2_1+((angle1024>>7)&0x07);/* 0 ... 8 */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_aya_momiji_oya;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
			h->rotationCCW1024		= (angle1024);
	//
	//		h->base_score			= score(25*2);/*ダミー*/
			h->base_hp				= (9999);/*倒せない*/
	//
			h->time_out 			= ((30+15)/*+1*/);
			{
				/* 初期位置 */
				h->cx256 			= src->cx256;
				h->cy256 			= src->cy256;
			}
			#if 1/*???*/
			h->MOMIJI_KODOMO_next_angle1024 	= (angle1024);
			#endif
	//		regist_vector(h, speed256, angle512);
		//	const int speed256 = t256(1.00);
		//	h->vx256	= ((sin1024((angle1024))*(speed256))>>8);	/*fps_factor*/
		//	h->vy256	= ((cos1024((angle1024))*(speed256))>>8);	/*fps_factor*/
			h->vx256	= ((sin1024((angle1024)))); 	/*fps_factor*/
			h->vy256	= ((cos1024((angle1024)))); 	/*fps_factor*/
		}
	}
}
