
#include "bullet_object.h"

/*---------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	星型描くぜ
	-------------------------------------------------------
	(無駄に複雑だったので、
		レーザーを撃つ機能と、
		星型を描く機能と、
		360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/

#define my_angle512 		tmp_angleCCW512 	/* 保持角度[星型を描く場合に使う角度](doll_point_angle512) */

#define doll_vx256			vx256
#define doll_vy256			vy256


#define time_out			user_data02 		/* 経過時間 */


/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

static SPRITE hosi_position_obj;/* 星型描いてる座標位置を保持。 */
/*static*/extern void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int time_out/**sd_nnn*/);
static void move_doll02(SPRITE *src)
{
	#if 1
	/* 魔方陣アニメーション */
	src->m_angleCCW512--;/* 右回り */
	mask512(src->m_angleCCW512);
	#endif
//
	src->time_out--;
	if ((64*8) < src->time_out)
	{
		/* 星型を描くよ */
		if (0==((src->time_out)&0x0f))
		{
			src->my_angle512 += (512*2/5);
			mask512(src->my_angle512);
			#define DOLL_SPEED10		(4)/*8*/
			src->doll_vx256 = ((sin512((src->my_angle512))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/				/* CCWの場合 */
			src->doll_vy256 = ((cos512((src->my_angle512))*/*t256*/(DOLL_SPEED10))/*>>8*/);/*fps_factor*/
			#undef DOLL_SPEED10
		}
		else
		{
			/* CCWの場合 */
			enemy_sp2_bullet_create01(&hosi_position_obj,
				src->my_angle512,
			//	t256(0.015),
			/* 与える残り時間 */	((src->time_out)&0x1ff)//((src->time_out)-(64*8))
			//	&(((BOSS06_DOLL_DATA *)src->data)->nnn)
			);
		}
		// 動作
		hosi_position_obj.x256 += (src->doll_vx256);/*fps_factor*/
		hosi_position_obj.y256 += (src->doll_vy256);/*fps_factor*/
	}
	#if 1
	else
	if (0 > src->time_out)
	{
		src->base_hp		= (0);/* 倒した */
		src->type = SP_DELETE;/* おしまい */
	}
	#endif
//
	#if 0
	//if (data->c->type==SP_DELETE) 		/*これではうまくいかない場合がある*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* 輝夜を倒すと皆破壊される。 */
	{
		src->base_hp		= (0);/* 倒した */
		src->type = SP_DELETE;
	}
	#endif
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_02_star(SPRITE *src)
{
	SPRITE *h;
	h							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//	h->type 					= TEKI_51_MAHOJIN1;
//	h->type 					= SP_MUTEKI;
	h->type 					= BOSS_16_YOUSEI11;
	h->base_hp					= (9999);/* 倒せない */
	h->flags					= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//	h->color32					= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
//	h->color32					= 0x80ff3388;	/*	obj_doll->alpha 		= 0x00;*/ /* 紅っぽく */
	h->color32					= 0x806633ff;	/*	obj_doll->alpha 		= 0x00;*/ /* 紅っぽく */
	h->callback_mover			= move_doll02;
//
	/* 子供魔方陣、配置位置 */
	#if 1
	h->x256 					= (src->x256);
	h->y256 					= (src->y256)-t256(16);/*咲夜 上方に配置*/
	#endif
	/* 弾を撃ち始める位置(星型描き始める位置) */
	hosi_position_obj.x256		= (src->x256)+(ra_nd()&0xfff);
	hosi_position_obj.y256		= (src->y256)+(ra_nd()&0xfff);/* 咲夜 下方から描く */
//
	h->time_out 				= ((64*8)+(5*16)+1);

	/* 星型を描く準備 */
	h->my_angle512				= 0;
//	h->doll_vx256				= t256(1.0);	/*-t256(0.125)*/ /*t256(1.0)*/
//	h->doll_vy256				= t256(0.0);	/* t256(2.0)*/ /*t256(0.0)*/
}
