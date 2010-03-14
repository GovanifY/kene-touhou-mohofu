
#include "bullet_object.h"

/*---------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	360ナイフを撃つぜ
	-------------------------------------------------------
	(無駄に複雑だったので、
		レーザーを撃つ機能と、
		星型を描く機能と、
		360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/

#define time_out	user_data02 		/* 経過時間 */


/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

static void move_doll03(SPRITE *src)
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
		if (0==((src->time_out)&(64-1)))/* 64回に1回発弾する */
		{
			static int count_jjj;
			count_jjj++;
			if ((3-difficulty) < count_jjj)
			{
				count_jjj = 0;
				/*---------------------------------------------------------
					360ナイフ
					魔方陣用
					全方向に青ナイフ弾を撃つ
				---------------------------------------------------------*/
				send1_obj->x256 = (src->x256)+t256(8.0);/* 8.0[dots]右 */
				send1_obj->y256 = (src->y256)+t256(4.0);/* 4.0[dots]下 */
				send1_obj->BULLET_REGIST_speed256			=	(t256(1.2));				/*(speed256)*/
				send1_obj->BULLET_REGIST_angle512			=	(0);						/* 弾源角度512 */	/*aaa_angle512*/
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/(18));			/* 分割角度([360/360]度を18分割) */
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KNIFE20_06_YUKARI;	/* [ピンクナイフ弾] */	/*BULLET_KNIFE20_04_AOI*/
				send1_obj->BULLET_REGIST_n_way				=	(18);						/* [18way] */
				bullet_regist_basic();
			}
		}
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

/*static*/ void add_zako_sakuya_doll_03_360knife(SPRITE *src)
{
	SPRITE *h;
	h							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//	h->type 					= TEKI_51_MAHOJIN1;
//	h->type 					= SP_MUTEKI;
	h->type 					= BOSS_16_YOUSEI11;
	h->base_hp					= (9999);/* 倒せない */
	h->flags					= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//	h->color32					= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
	h->color32					= 0x80ff3388;	/*	obj_doll->alpha 		= 0x00;*/ /* 紅っぽく */
	h->callback_mover			= move_doll03;
	#if 1
	h->x256 					= src->x256;
	h->y256 					= src->y256-t256(16);/*咲夜 上方に配置*/
	#endif
	h->time_out 				= (64*8)+(16*64);/* 16[弾]撃つ */
}
