
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"メイド4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	右側から二列縦隊で現れ、下に向かい。下で旋回。
	中央から上昇し、右側に向かい。上で旋回。
	右側から下に向かい。画面外へ退場。
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

enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};

static void move_meido4(SPRITE *src)
{
	switch (src->state)
	{
	case SS00: /* 下に向かう / down */
		if (src->y256 > t256(128))			/* 中心付近を越えて下がったら、少し速度を落とす */	/*200*/
		{
			src->state++;/*SS01*/
			src->speed256 = t256(2.5);		/* 速度を落とす */	/*3.0*/
		}
		break;
	case SS01: /* 下で旋回 / turn */
#if 0
/* CWの場合 */
		src->angle1024 -= (10);				/*4*/ /*cv1024r(2)*/ /*fps_factor*/
		mask1024(src->angle1024);
		if ((src->angle1024 >= cv1024r((270-3))) &&		/* ほぼ真上を向いたら */
			(src->angle1024 <= cv1024r((270+3))))
		{
			src->angle1024 = cv1024r((270));		/* 真上に強制補正 */
			src->state++;/*SS02*/
			src->speed256 = t256(2.0);		/*8.0*/ /* 速度を落とす */
		}
#else
/* CCWの場合 */
		src->tmp_angleCCW1024 += (10);		/*4*/ /*cv1024r(360-2)*/ /*fps_factor*/
		mask1024(src->tmp_angleCCW1024);
		if ( src->tmp_angleCCW1024 > cv1024r((180))	)		/* ほぼ真上を向いたら */
		{
			src->tmp_angleCCW1024 = cv1024r((180));		/* 真上に強制補正 */
			src->state++;/*SS02*/
			src->speed256 = t256(2.0);		/*8.0*/ 	/* 速度を落とす */
		}
#endif
		break;
	case SS02: /* up */
		if (src->y256 < t256(64/*50*/)) 	/* 中心付近を越えて上がったら、少し速度を落とす */
		{
			src->state++;/*SS03*/
			src->speed256 = t256(1.5);		/*3.0*/
		}
		break;
	case SS03: /* turn */
#if 0
/* CWの場合 */
		src->angle1024 += (10);				/*4*/ /*cv1024r(2)*/ /*fps_factor*/
		mask1024(src->angle1024);
		if ((src->angle1024 >= cv1024r((90-3))) &&		/* ほぼ真下を向いたら */
			(src->angle1024 <= cv1024r((90+3))))
		{
			src->angle1024 = cv1024r((90));		/* 真下に強制補正 */
			src->state++;/*=SS04*/
			src->speed256 = t256(1.0);	/*6.0*/
		}
#else
/* CCWの場合 */
		src->tmp_angleCCW1024 -= (10);		/*2*/ /*cv1024r(360-2)*/ /*fps_factor*/
		mask1024(src->tmp_angleCCW1024);
		if (src->tmp_angleCCW1024 > cv1024r(270) ) 	/* ほぼ真下を向いたら */
		{
			src->tmp_angleCCW1024 = cv1024r((0));		/* 真下に強制補正 */
			src->state++;/*=SS04*/
			src->speed256 = t256(1.0);	/*6.0*/
		}
#endif
		break;
	case SS04: /* down */
		src->speed256 += t256(0.1);/*加速*/
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		break;
	}
	/*以下rwingx.cと同じ*/
	src->x256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ 	/* CCWの場合 */
	src->y256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_meido4(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;/*SP_ZAKO*/ /*_15_MEIDO4*/
		h->callback_mover		= move_meido4;
		h->callback_loser		= lose_meido4;
		h->tmp_angleCCW1024		= cv1024r(0);		/* 真下 */	/* CCWの場合 */
		h->speed256 			= t256(3.0);	/*6.0*/
		h->state				= SS00;
	}
}
//	h->callback_hit_enemy	= callback_hit_zako;
