
#include "bullet_object.h"

/*---------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	レーザーを撃つぜ
	(無駄に複雑だったので、
		レーザーを撃つ機能と、
		星型を描く機能と、
		360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/

#define state_d01			user_data00 		/* 行動 */
#define state_d02			user_data01 		/* 形態 */
#define time_out			user_data02
//
#define wait1				user_data03 		/* */
#define my_angle512 		tmp_angleCCW512 	/* 保持角度[レーザーの撃つ向きを維持する角度] */


enum
{
//--- 星型以前
	DS00 = 0,
	DS01,
	DS02,
	DS03,
};
/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_doll01_laser(SPRITE *src)
{
	switch (src->state_d02)
	{
//------------------------------------------
	case DS00:
	//	src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/
	//	if (0x96000000 < src->color32)		/*	(0x96 < src->alpha) */
		{
	//		src->color32		= 0x96ffffff;	/*	src->alpha		= 0x96;*/
			src->state_d02++;/* = DS01*/
			src->wait1		= 50;
		}
		break;

	case DS01:
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{
			src->state_d02++;
			src->state_d01 = 0x100;
		}
		break;
	case DS02:
		if (0==(src->state_d01&0x0f))/*%10*/
		{
			/* CCWの場合 */
			send1_obj->x256 = (src->x256)+t256(8.0);/* 8.0[dots]右 */
			send1_obj->y256 = (src->y256)+t256(4.0);/* 4.0[dots]下 */
			bullet_create_tomari2_dan(send1_obj, (src->state_d01/*>>4*/<<3/*1/10*/)/**256*/, src->my_angle512,					  		t256(0.02), src->my_angle512+(src->state_d01)+deg_360_to_512CCW((180	)));	//ra d2deg512((src->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(send1_obj, (src->state_d01/*>>4*/<<3/*1/10*/)/**256*/, src->my_angle512-deg_360_to_512CCW((120)), t256(0.02), src->my_angle512+(src->state_d01)+deg_360_to_512CCW((180+120)));	//ra d2deg512((src->state_d01)>>7/*/100*/)
			bullet_create_tomari2_dan(send1_obj, (src->state_d01/*>>4*/<<3/*1/10*/)/**256*/, src->my_angle512+deg_360_to_512CCW((120)), t256(0.02), src->my_angle512+(src->state_d01)+deg_360_to_512CCW((180-120)));	//ra d2deg512((src->state_d01)>>7/*/100*/)
		}
		src->state_d01--;
		if (0 > src->state_d01)
		{
			src->state_d02++;
			src->wait1		= 15;
			src->state_d01 = 0x10;
		}
		break;
	case DS03:
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{
			src->wait1 = 12;
			voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
			send1_obj->x256 = (src->x256)+t256(8.0);/* 8.0[dots]右 */
			send1_obj->y256 = (src->y256)+t256(4.0);/* 4.0[dots]下 */
			{int i;
				for (i=t256(5); i<t256(9); i+=t256(1))
				{
					/* CCWの場合 */
					bullet_create_offset_dan_type000(send1_obj, i, src->my_angle512,						/*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(send1_obj, i, src->my_angle512-deg_360_to_512CCW(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
					bullet_create_offset_dan_type000(send1_obj, i, src->my_angle512+deg_360_to_512CCW(120), /*t256(0), t256(0),*/ BULLET_HARI32_00_AOI);
				}
			}
			/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
			/* 12.2230999796235950864828297767852 == 512/41.8879008478638666666666666666667 */
			src->my_angle512 -= 12;/* 右回り(CCWなので) */
			src->state_d01--;
		}
		if (0 > src->state_d01)
		{
			src->state_d02--;/* = DS02*/
			src->state_d01 = 0x50;
			#if 1
			src->base_hp			= (0);/* 倒した */
			src->type = SP_DELETE;	/* おしまい */
			#endif
		}
		break;

	}

	#if 1
	if (0 > src->time_out)
	{
		src->base_hp		= (0);/* 倒した */
		src->type = SP_DELETE;/* おしまい */
	}
	#endif
//
}

	#if 0
	//if (src->c->type==SP_DELETE)		/*これではうまくいかない場合がある*/
	if (0/*FLG_NO_DOLL*/==common_boss_flags)	/* 輝夜を倒すと皆破壊される。 */
	{
		src->base_hp			= (0);/* 倒した */
		src->type = SP_DELETE;
	}
	#endif


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*---------------------------------------------------------
	子供魔方陣 生成 	子供魔方陣グラフィック生成
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_01_laser(SPRITE *src)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= BOSS_16_YOUSEI11;
	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
	h->callback_mover		= move_doll01_laser;
//	h->callback_loser		= NULL;
//	h->callback_hit_enemy	= callback_hit_zako;
//
//	h->base_score			= score(25*2);/*ダミー*/
	h->base_hp				= (9999);/* 倒せない */
//	h->color32				= 0x00ffffff;	/*	obj_doll->alpha 		= 0x00;*/
	h->color32				= 0x80aa88ff;	/*	obj_doll->alpha 		= 0x00;*/ /* 紫っぽく */
	#if 1
	h->x256 				= (src->x256);
	h->y256 				= (src->y256)-t256(16);/*咲夜 上方に配置*/
//	h->x256 				= ((pd_boss))->x256;
//	h->y256 				= ((pd_boss))->y256-t256(16);
	#endif
	h->state_d02			= 0;
	h->state_d01			= 0x00;
	h->wait1				= 0;
	h->my_angle512			= (0);
//
		h->time_out 			= (100);/*よくわかんない(スペカシステムのせいでうまくいかない)*/
}
