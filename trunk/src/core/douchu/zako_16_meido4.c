
#include "douchu.h"

/*---------------------------------------------------------
	"メイド4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	右側から二列縦隊で現れ、下に向かい。下で旋回。
	中央から上昇し、右側に向かい。上で旋回。
	右側から下に向かい。画面外へ退場。
---------------------------------------------------------*/

#define state		user_data02
#define speed256	user_data03

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_meido4(SPRITE *src)
{
	item_create(src, SP_ITEM_05_TENSU, 1, ITEM_MOVE_FLAG_06_RAND_XY);
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido4(SPRITE *src)
{
	switch (src->state)
	{
	case 0: /* 下に向かう / down */
		if (src->y256 > t256(128/*200*/)) 	/* 中心付近を越えて下がったら、少し速度を落とす */
		{
			src->state++/*=1*/;
			src->speed256=t256(2.5/*3.0*/);/* 速度を落とす */
		}
		break;
	case 1: /* 下で旋回 / turn */
#if 0
/* CWの場合 */
		src->angle512 -= ((5))/*2*/ /*deg_360_to_512(2)*/ ;/*fps_factor*/
	//	if (src->angle512  <	deg_360_to_512(0))
	//	{	src->angle512  += deg_360_to_512(360);	}
		mask512(src->angle512);
		if ((src->angle512 >= deg_360_to_512((270-3))) &&		/* ほぼ真上を向いたら */
			(src->angle512 <= deg_360_to_512((270+3))))
		{
			src->angle512 = deg_360_to_512((270));		/* 真上に強制補正 */
			src->state++/*=2*/;
			src->speed256=t256(2.0/*8.0*/);/* 速度を落とす */
		}
#else
/* CCWの場合 */
		src->tmp_angleCCW512 += ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ ;/*fps_factor*/
	//	if (src->tmp_angleCCW512  <	deg_360_to_512CCW(360-0))
	//	{	src->tmp_angleCCW512  += deg_360_to_512CCW(360-360);	}
		mask512(src->tmp_angleCCW512);
		if ( src->tmp_angleCCW512 > deg_360_to_512CCW((180)) 		)		/* ほぼ真上を向いたら */

		{
			src->tmp_angleCCW512 = deg_360_to_512CCW((180));		/* 真上に強制補正 */
			src->state++/*=2*/;
			src->speed256=t256(2.0/*8.0*/);/* 速度を落とす */
		}
#endif
		break;
	case 2: /* up */
		if (src->y256 < t256(64/*50*/))	/* 中心付近を越えて上がったら、少し速度を落とす */
		{
			src->state++/*=3*/;
			src->speed256=t256(1.5/*3.0*/);
		}
		break;
	case 3: /* turn */
#if 0
/* CWの場合 */
		src->angle512 += ((5))/*2*/ /*deg_360_to_512(2)*/ ;/*fps_factor*/
	//	if (src->angle512 < deg_360_to_512(0))
	//	{	src->angle512 += deg_360_to_512(360); 	}
		mask512(src->angle512);
		if ((src->angle512 >= deg_360_to_512((90-3))) &&		/* ほぼ真下を向いたら */
			(src->angle512 <= deg_360_to_512((90+3))))
		{
			src->angle512 = deg_360_to_512((90));		/* 真下に強制補正 */
			src->state++/*=4*/;
			src->speed256=t256(1.0/*6.0*/);
		}
#else
/* CCWの場合 */
		src->tmp_angleCCW512 -= ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ ;/*fps_factor*/
	//	if (src->tmp_angleCCW512 < deg_360_to_512CCW(360-0))
	//	{	src->tmp_angleCCW512 += deg_360_to_512CCW(360-360); 	}
		mask512(src->tmp_angleCCW512);
		if (src->tmp_angleCCW512 > deg_360_to_512CCW(270) ) 	/* ほぼ真下を向いたら */
		{
			src->tmp_angleCCW512 = deg_360_to_512CCW((0));		/* 真下に強制補正 */
			src->state++/*=4*/;
			src->speed256=t256(1.0/*6.0*/);
		}
#endif
		break;
	case 4: /* down */
		src->speed256 += t256(0.1);/*加速*/
		gamen_gai_nara_osimai(src);/* 画面外ならおしまい */
		break;
	}
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
	src->x256+=((sin512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
	src->y256+=((cos512((src->tmp_angleCCW512))*src->speed256)>>8);/*fps_factor*/
//	src->an im_frame=(deg_512_to_360(src->angle512+deg_360_to_512(270))/10)%36;
//	src->an im_frame = ((((src->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 00
/* [CCWの場合(新)] CWの場合 */
	src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* 旧 */
//#else
/* CCWの場合 */
	/* 新(まだ作ってない) */
	//src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*無理矢理旧互換*/
	{int aaa512;
		aaa512 = 128+ 512 - src->tmp_angleCCW512;
		mask512(aaa512);
		src->yx_an im_frame = (((aaa512))>>(6));
	}
#endif
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_meido4(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_15_MEIDO4*/
	add_zako_common(l, h);
//	h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->callback_mover		= move_meido4;
	h->callback_loser		= lose_meido4;
	h->tmp_angleCCW512		= deg_360_to_512CCW(0); 	/* 真下 */	/* CCWの場合 */
	h->speed256				= t256(3.0/*6.0*/);
	h->state 				= 0;
}
//	h->callback_hit_enemy	= callback_hit_zako;
//
//	/*h->base.*/h->base_score		= score(/*25*/15*2);
//	/*h->base.*/h->base_hp			= (8*5)+(1/*di fficulty*/<<(2+3));
//	h->x256 				= (0==(i&(2-1)/*%2*/))?t256(30):t256(50);
//	h->y256 				= -(i*t256(20));
