
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"メイド3",		"CURVER",	rwingx_curver
	-------------------------------------------------------
上から現れてS字をn回描いて横へ

←←
↓
→→→→→
		↓
←←←←←
↓
→→→→→
		↓
←←←←←
	-------------------------------------------------------
	バグあり。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define clip_left256		user_data00
#define clip_right256		user_data01
#define state				user_data02
#define speed256			user_data03
#define turn_count			user_data04
#define turnspeed1024		user_data05

static int static_last = 0; 	/* 登場位置の切り替え */


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

//static void lose_meido3(SPRITE *src)
//{
//	destoroy[static_last]++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
//	{
//		destoroy[static_last] = 0;
//		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	}
//}

/*---------------------------------------------------------
	敵攻撃
---------------------------------------------------------*/

static void shot_meido3(SPRITE *src)
{
//	if (0 < /*src->*/enemy_rank)
	{
		obj_send1->x256 					= (src->x256);
		obj_send1->y256 					= (src->y256);
		br.BULLET_REGIST_speed256			= t256(2.0);
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
		br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
		br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;
		br.BULLET_REGIST_n_way				= (difficulty<<1);			/*[n]*/
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}	/* easy以外は狙い弾を撃つ */
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

static void move_meido3(SPRITE *src)
{
	switch (src->state)
	{
	case SS00:	/* 左周りで回転中 */
	//	src->tmp_angleCCW1024 -= src->turnspeed1024;/*fps_factor*/		/* CWの場合 */
		src->tmp_angleCCW1024 += src->turnspeed1024;/*fps_factor*/		/* CCWの場合 */
		mask1024(src->tmp_angleCCW1024);
		/* 右(斜め上)を向いたら */
		/* CWの場合 */
	//	if ( (1024-32) < src->tmp_angleCCW1024 )		/*eps*/ /*<= cv1024r(0)*/	//cv1024r(10/*90*/ /*-90*/ /*360*/ /*-10*/
		/* CCWの場合 */
		if ( 0==(0x200 & src->tmp_angleCCW1024) )/* (cv1024r(180) > src->tmp_angleCCW1024) */
		{
			if ( (256+32) < src->tmp_angleCCW1024 ) 	/*eps*/ /*<= cv1024r(0)*/	//cv1024r(10/*90*/ /*-90*/ /*360*/ /*-10*/
			{
			//	src->tmp_angleCCW1024 = cv1024r(0/*360*/ /*-10*/)/*cv1024r(360)*/;
				src->state++/*SS01*/;
				shot_meido3(src);	/* easy以外は狙い弾を撃つ */
			}
		}
		break;
	case SS01:	/* 右(斜め上)へ移動中 */
		if (0 < src->turn_count)
		{
			if (src->x256 >= src->clip_right256)	{ src->state++/*SS02*/; 	src->turn_count--; }
		}
		else	/* 退場中 */
		{
			gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		}
		break;
	case SS02:	/* 右周りで回転中 */
	//	src->tmp_angleCCW1024 += src->turnspeed1024;/*fps_factor*/		/* CWの場合 */
		src->tmp_angleCCW1024 -= src->turnspeed1024;/*fps_factor*/		/* CCWの場合 */
		mask1024(src->tmp_angleCCW1024);
		/* 左(斜め上)を向いたら */
	//	if ( (512+32) < src->tmp_angleCCW1024 ) 			//cv1024r(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		/* CCWの場合 */
		if ( 0!=(0x200 & src->tmp_angleCCW1024) )			/* (cv1024r(180) < src->tmp_angleCCW1024) */
		{
			if ( (512+256-32) > src->tmp_angleCCW1024 ) 	//cv1024r(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
			{
			//	src->tmp_angleCCW1024 = cv1024r(180/*+10*/);
				src->state++/*SS03*/;
				shot_meido3(src);	/* easy以外は狙い弾を撃つ */
			}
		}
		break;
	case SS03:	/* 左(斜め上)へ移動中 */
		if (0 < src->turn_count)
		{
			if (src->x256 <= src->clip_left256) 	{	src->state=0;	src->turn_count--;	}
		}
		else	/* 退場中 */
		{
			gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		}
		break;
	}
	/*以下rwingx.cと同じ*/
	src->x256+=((sin1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/ /* CCWの場合 */
	src->y256+=((cos1024((src->tmp_angleCCW1024))*src->speed256)>>8);/*fps_factor*/
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_36_YOUSEI3_1+((((src->tmp_angleCCW1024)&(1024-1)))>>7);
	}
/*現在仕様上おかしい*/
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_meido3(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_36_YOUSEI3_1;	/*SP_ZAKO*/ /*_14_MEIDO3*/
		h->callback_mover		= move_meido3;
		h->speed256 			= t256(1.5);		/*3+difficulty*/ /*はやすぎ*/
		h->turnspeed1024		= (7*2);			/*(8+difficulty+difficulty)*/
		if (t256(GAME_WIDTH/2) > h->x256 )
		{
			h->tmp_angleCCW1024 	= cv1024r(90-10/*360-10*/);
		//	h->x256 				= -t256(32);
			static_last 			= 0;
		}
		else
		{
			h->tmp_angleCCW1024 	= cv1024r(180+90+10/*360-170*/);
		//	h->x256 				= t256(GAME_WIDTH);
			static_last 			= 1;
		}

	//	h->clip_left256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
	//	h->clip_right256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
		h->clip_left256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
		h->clip_right256	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32-32); /* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
		h->state			= (static_last<<1);/*SS00*/
		h->turn_count		= 4;
	}
}

//	h->tmp_angleCCW1024 		= (256+128/*0*/);	/*256+16*/ /*cv1024r(270)*/;
