
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"メイド2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (赤い妖精)
	左右から現れ、自機狙い弾を打った後、真下へ直進してくる。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define state		user_data04
#define speed256	user_data05
#define wait		user_data06

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
	SS05,
};

static void move_meido2(SPRITE *src)
{
	src->wait++;/*fps_factor*/
	switch (src->state)
	{
	case SS00:
		if (src->wait >= 40)
		{
			src->state++/*SS01*/;
		}
		break;
	case SS01:
		src->speed256 -= t256(0.02);/*fps_factor*/
		if (src->speed256 <= 0)
		{
			src->speed256 = 0;
			src->state++/*SS02*/;
		}
		break;
	case SS02:
//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*src->le vel*/)<<8) ) )
#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (/*data_hari_rank256*/(1<<8)/*src->le vel*/) )

#if 0
/* CWの場合 */
		if (src->angle1024 < cv1024r(90))
		{
			src->angle1024 += cv1024r(3);/*fps_factor*/
			if (src->angle1024 >= cv1024r(89))
			{
				src->state++/*SS03*/;
			}
		}
		else
		{
			src->angle1024 -= cv1024r(3);/*fps_factor*/
			if (src->angle1024 <= cv1024r(91))
			{
				src->state++/*SS03*/;
			}
		}
#else
/* CCWの場合 */
		if (src->tmp_angleCCW1024 < cv1024r(180/*360+90-90*/))
		{
			src->tmp_angleCCW1024 -= cv1024r(3);/*fps_factor*/
			if (src->tmp_angleCCW1024 < cv1024r(0/*360+90-89*/))
			{
				src->state++/* = 3*/;
			}
		}
		else
		{
			src->tmp_angleCCW1024 += cv1024r(3);/*fps_factor*/
			if (src->tmp_angleCCW1024 > cv1024r(360/*360+90-91*/))
			{
				src->state++/* = 3*/;
			}
		}
#endif
		break;
	case SS03: /*弾撃つ*/
				/* CCWの場合 */
				src->tmp_angleCCW1024					= cv1024r(0);
				//if (data_hari_rank256/*src->le vel*/)
				{
					obj_send1->x256 					= (src->x256);
					obj_send1->y256 					= (src->y256);
					br.BULLET_REGIST_speed256			= HARIDAN_SPEED;				/* 弾速 */
					br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				//	br.BULLET_REGIST_div_angle1024		= (0);							/* ダミー角度(未使用) */
					br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 		/* [ 弾] */
					br.BULLET_REGIST_n_way				= (1);							/* [1way] */
					br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
					bullet_regist_vector();
				}
				src->state++/*SS04*/;
		break;
	case SS04: /*特攻*/
		src->speed256 += t256(0.2);/*加速*/ /*t256(0.3)*/
		if (src->speed256 >= t256(2.5)) /*t256(3.0)*/ /*(6+src->le vel)*/
		{
			src->speed256 = t256(2.5); /* 加速最大値 */ /*3*/ /*(6+src->le vel)*/
			src->state++;	/* = 5*/
		}
		break;
	case SS05:/*退場*/
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		break;
	}
	/*以下rwingx.cと同じ*/
	src->x256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ 	/* CCWの場合 */
	src->y256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
//
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_meido2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_13_MEIDO2*/
		h->callback_mover		= move_meido2;

		h->state				= SS00;
		h->wait 				= (0);
		h->speed256 			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
	/* CCWの場合 */
	//	if ((i&1))
		if (t256(GAME_WIDTH/2) > h->x256 )
		{
			h->tmp_angleCCW1024 	= cv1024r(90-10/*360-10*/);
		//	h->x256 				= -t256(32);
		}
		else
		{
			h->tmp_angleCCW1024 	= cv1024r(180+90+10/*360-170*/);
		//	h->x256 				= t256(GAME_WIDTH);
		}
	//	h->y256 = t256(50);
	}
}
