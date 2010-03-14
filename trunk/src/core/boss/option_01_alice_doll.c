
#include "bullet_object.h"

/*---------------------------------------------------------
	アリス人形弾幕
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形 -easy-」
アリスの放った四体の人形が、二回分裂する7way弾を撃つ。
4 x 7 x 7 == 最大 196[弾](妖々夢)
3 x 7 x 7 == 最大 147[弾](模倣風)	//3 x 8 x 8 == 192
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形」
アリスの放った六体の人形が、二回分裂する7way弾を撃つ。
6 x 7 x 7 == 最大 294[弾](妖々夢)
5 x 7 x 7 == 最大 245[弾](模倣風)	//5 x 8 x 8 == 320
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形 -hard-」
アリスの放った八体の人形が、二回分裂する7way弾を撃つ。
8 x 7 x 7 == 最大 392[弾](妖々夢)
9 x 7 x 7 == 最大 441[弾](模倣風)	//7 x 8 x 8 == 448
	-------------------------------------------------------
蒼符「博愛のオルレアン人形」
アリスの放った八体の人形が、三回分裂する7way弾を撃つ。
8 x 7 x 7 x 7 == 最大2744[弾](妖々夢)
2 x 7 x 7 x 7 == 最大 686[弾](模倣風)	//9 x 8 x 8 == 576
---------------------------------------------------------*/

#define sx256		user_data00 	/* 基準固定 x位置 */
#define sy256		user_data01 	/* 基準固定 y位置 */
#define time_out	user_data02 	/* 経過時間 */
#define radius		user_data03 	/* 円の半径 */
#define recursive	user_data04 	/* 分裂回数(0>分裂しない) */


/*---------------------------------------------------------
	分裂(last)
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(SPRITE *src)
{
	send1_obj->x256 = src->x256;		/* 弾源x256 */
	send1_obj->y256 = src->y256;		/* 弾源y256 */
	send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));						/* 弾速 */		/*+((src->boss_base_danmaku_time_out)<<2)*/
	send1_obj->BULLET_REGIST_angle512			=	(src->m_angleCCW512);				/* src->tmp_angleCCW512 */
	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/23);						/* 角度 (512/27) (512/24) */
	send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_00_AOI;				/* [青鱗弾] */
	send1_obj->BULLET_REGIST_n_way				=	(7);								/* [7way] [8way] */
	bullet_regist_basic();
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src);

static void move_alice_doll_all(SPRITE *src)
{
	src->time_out--;
	if (( 0 > src->time_out ))
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* このラインより下からは敵が撃たない */
		{
			if (0 > src->recursive)
			{
				move_alice_doll_last_burrets(src);
			}
			else
			{
			//	bullet_create_aka_maru_jikinerai(src, t256(2.0) );
				add_zako_alice_doll_2nd_CCW(src);
			}
		}
		src->type = SP_DELETE;	/* おしまい */
	//	return;
	}
	else
	{
		src->m_angleCCW512 += (((src->recursive)&(1))?(5):(-5));/* 人形＆グラ回転 */
		mask512(src->m_angleCCW512);
		{
			int bbb_m_angleCCW512;
			bbb_m_angleCCW512 = src->m_angleCCW512+(64);/* 64==45度傾ける */
		//	bbb_m_angleCCW512 = src->m_angleCCW512-(64);/* -64 == -45度傾ける */
			mask512(bbb_m_angleCCW512);
			src->radius++;
			src->x256 = src->sx256 + ((sin512((bbb_m_angleCCW512))*src->radius));/*fps_factor*/
			src->y256 = src->sy256 + ((cos512((bbb_m_angleCCW512))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	敵を追加する(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
//	const int add_angle = ( (512/7) );	/* 加算角度 */	/* ２回目以降の分列数は常に7回 */
	int i_angle;	/* 積算角度 */
	for (i_angle=0; i_angle<(512);	i_angle += (512/7)/*add_angle*/)	/* 弾数 */
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= /*TEKI_16_YOUSEI1_5*/TEKI_12_YOUSEI1_1+((1)<<2)+((src->recursive)<<2); 	/*SP_ZAKO*/ /*_02_YUKARI1*/
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover		= move_alice_doll_all;
	//	h->callback_loser		= NULL;
		h->callback_hit_enemy	= callback_hit_zako;
		h->m_angleCCW512		= (i_angle);
//
//		/*data->base.*/h->base_score		= score(25*2);/*ダミー*/
		/*data->base.*/h->base_hp			= (9999);/*倒せない*/
//
		h->time_out 			= (60);
		{
			/* 初期位置 */
		/*	h->x256 =*/ h->sx256 = (src->x256);
		/*	h->y256 =*/ h->sy256 = (src->y256);
		//
	//		h->vx256 = (0);/*右方向*/
	//		h->vy256 = (0);/*下方向*/
		}
		h->radius		= (0);
		h->recursive	= ( ((src->recursive)-1) );
	}
}


/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/9),
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/42),
//		(int)(512/4), (int)(512/6), (int)(512/8), (int)(512/12),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/20),/* 20 Lunatic 難しい */
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/17),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/8),
		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/2),/*(オルレアン人形)*/
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/3),/*(オルレアン人形)*/ 	/* [現在の方式では無理] */
	};
	const int add_angle = ( (aaa_tbl[(difficulty)]));	/* 加算角度 */	/* ２回目以降の分列数は常に7回 */

	int i_angle;	/* 積算角度 */
	for (i_angle=0; i_angle<(512);	i_angle += add_angle)	/* 一周 */
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover		= move_alice_doll_all;
	//	h->callback_loser		= NULL;
//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
		h->m_angleCCW512		= (i_angle);
//
//		/*data->base.*/h->base_score		= score(25*2);/*ダミー*/
		/*data->base.*/h->base_hp			= (9999);/*倒せない*/
//
		h->time_out 			= (60);
		{
			/* 初期位置 */
		/*	h->x256 =*/ h->sx256 = (src->x256);
		/*	h->y256 =*/ h->sy256 = (src->y256);
		//
	//		h->vx256 = (0);/*右方向*/
	//		h->vy256 = (0);/*下方向*/
		}
		h->radius		= (0);
		h->recursive	= ( ((3==difficulty)?(1):(0)));/*(Lunatic==オルレアン人形)*/
	}
}



#if 00
/*---------------------------------------------------------
	敵を追加する(common)
---------------------------------------------------------*/

static void add_zako_alice_doll_common(SPRITE *src, int is_the_first)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/9),
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/42),
//		(int)(512/4), (int)(512/6), (int)(512/8), (int)(512/12),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/20),/* 20 Lunatic 難しい */
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/17),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/8),
		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/2),/*(Lunatic==オルレアン人形)*/
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/3),/*(Lunatic==オルレアン人形)*/ 	/* [現在の方式では無理] */
	};
	const int add_angle = ((0==is_the_first)?(512/7):(aaa_tbl[(difficulty)]));	/* 加算角度 */	/* ２回目以降の分列数は常に7回 */
//
	int i_angle;	/* 積算角度 */
	for (i_angle=0; i_angle<(512);	i_angle += add_angle)	/* 弾数 */
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= TEKI_12_YOUSEI1_1+((src->recursive)<<2);
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	//
		h->callback_mover		= move_alice_doll_all;
	//	h->callback_loser		= NULL;
		h->callback_hit_enemy	= callback_hit_zako;
	//
	//	h->base_score			= score(25*2);/*ダミー*/
		h->base_hp				= (9999);/*倒せない*/
	//
		h->m_angleCCW512		= (i_angle);
		h->time_out 			= 60;
		{
			/* 初期位置 */
		/*	h->x256 =*/ h->sx256 = (src->x256);
		/*	h->y256 =*/ h->sy256 = (src->y256);
		//
	//		h->vx256 = (0);/*右方向*/
	//		h->vy256 = (0);/*下方向*/
		}
		h->radius		= (0);
		h->recursive	= ((0==is_the_first)?((src->recursive)-1):((3==difficulty)?(1):(0)));
	}
}


/*---------------------------------------------------------
	敵を追加する(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
	add_zako_alice_doll_common(src, 0);
}


/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	add_zako_alice_doll_common(src, 1);
}
#endif
