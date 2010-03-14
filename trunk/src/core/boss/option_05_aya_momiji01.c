
#include "bullet_object.h"

/*---------------------------------------------------------

弾幕
	-------------------------------------------------------


---------------------------------------------------------*/

#define time_out	user_data02 						/* 経過時間 */
#define MOMIJI_KODOMO_next_angle512 tmp_angleCCW512		/* 子供弾、生成角度 */

/*---------------------------------------------------------

---------------------------------------------------------*/

//
/*static*/extern void add_zako_aya_5_momiji(SPRITE *src);
static void move_aya_momiji_oya(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
	if ((0 > src->time_out))
	{
		add_zako_aya_5_momiji(src); 	/* 5方向もみじ小弾を生成 */
		src->type = SP_DELETE;
	}
	src->x256 += (/*data->v*/src->vx256);/*fps_factor*/
	src->y256 += (/*data->v*/src->vy256);/*fps_factor*/
//
	/* 親が画面外なら分裂しない */
	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ void add_zako_aya_doll(SPRITE *src)
{
	int angle512;
//	for (angle512=(int)((512)-(512*2/24)); angle512<(int)((512)+(512*3/24)); angle512+=(int)(512*1/24) )
	for (angle512=(0); angle512<(512); angle512+=(int)(512*1/24) )
	{
		SPRITE *h;
		h					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	//	h->type 			= BULLET_MARU8_00_AKA+(1);/*1==とりあえず*/ /*S P_BULLET*/ /*SP_LASER*/
		h->type 			= TEKI_24_YOUSEI2_1+((angle512>>6)&0x07);/* 0 ... 8 */
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover	= move_aya_momiji_oya;
	//	h->callback_loser		= NULL;
//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
		h->m_angleCCW512		= (angle512);
//
//		/*data->base.*/h->base_score		= score(25*2);/*ダミー*/
		/*data->base.*/h->base_hp			= (9999);/*倒せない*/
//
		h->time_out 		= ((30+15)/*+1*/);
		{
			/* 初期位置 */
			h->x256 			= src->x256;
			h->y256 			= src->y256;
		}
		#if 1/*???*/
		h->MOMIJI_KODOMO_next_angle512		= (angle512);
		#endif
//		regist_vector(h, speed256, angle512);
	//	const int speed256 = t256(1.00);
	//	h->vx256	= ((sin512((angle512))*(speed256))>>8); 	/*fps_factor*/
	//	h->vy256	= ((cos512((angle512))*(speed256))>>8); 	/*fps_factor*/
		h->vx256	= ((sin512((angle512)))); 	/*fps_factor*/
		h->vy256	= ((cos512((angle512)))); 	/*fps_factor*/
	}
}
