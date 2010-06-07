
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
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
	obj_send1->x256 					= (src->x256);		/* 弾源x256 */
	obj_send1->y256 					= (src->y256);		/* 弾源y256 */
	br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
	br.BULLET_REGIST_angle1024			= (src->m_angleCCW1024);			/* 角度 */
	br.BULLET_REGIST_div_angle1024		= (int)(1024/23);					/* 分割角度 (1024/27) (1024/24) */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;			/* [青鱗弾] */
	br.BULLET_REGIST_n_way				= (7);								/* [7way] [8way] */
	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	bullet_regist_vector();
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
				add_zako_alice_doll_2nd_CCW(src);
			}
		}
		src->type = SP_DELETE;	/* おしまい */
	//	return;
	}
	else
	{
		src->m_angleCCW1024 += (((src->recursive)&(1))?(10):(-10));/* 人形＆グラ回転 */
		mask1024(src->m_angleCCW1024);
		{
			int bbb_m_angleCCW1024;
			bbb_m_angleCCW1024 = src->m_angleCCW1024+(1024/8);/*  (1024/8) ==  45/360度傾ける */
		//	bbb_m_angleCCW1024 = src->m_angleCCW1024-(1024/8);/* -(1024/8) == -45/360度傾ける */
			mask1024(bbb_m_angleCCW1024);
			src->radius++;
			src->x256 = src->sx256 + ((sin1024((bbb_m_angleCCW1024))*src->radius));/*fps_factor*/
			src->y256 = src->sy256 + ((cos1024((bbb_m_angleCCW1024))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	敵を追加する(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
//	const int add_angle = ( (1024/7) ); /* 加算角度 */	/* ２回目以降の分列数は常に7回 */
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (1024/7)/*add_angle*/)	/* 弾数 */
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= /*TEKI_16_YOUSEI1_5*/TEKI_12_YOUSEI1_1+((1)<<2)+((src->recursive)<<2);	/*SP_ZAKO*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_enemy	= callback_hit_zako;
			h->m_angleCCW1024		= (i_angle1024);
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
			h->radius				= (0);
			h->recursive			= ( ((src->recursive)-1) );
		}
	}
}


/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/9),
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/42),
//		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/12),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/20),/* 20 Lunatic 難しい */
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/17),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/8),
		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*(オルレアン人形)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(オルレアン人形)*/ 	/* [現在の方式では無理] */
	};
	const int add_angle1024 = ( (aaa_tbl[(difficulty)]));	/* 加算角度 */	/* ２回目以降の分列数は常に7回 */

	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* 一周 */
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
			h->m_angleCCW1024		= (i_angle1024);
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
			h->radius				= (0);
			h->recursive			= ( ((3==difficulty)?(1):(0)));/*(Lunatic==オルレアン人形)*/
		}
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
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/9),
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/42),
//		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/12),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/20),/* 20 Lunatic 難しい */
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/17),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/8),
		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*(Lunatic==オルレアン人形)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(Lunatic==オルレアン人形)*/	/* [現在の方式では無理] */
	};
	const int add_angle1024 = ((0==is_the_first)?(1024/7):(aaa_tbl[(difficulty)])); /* 加算角度 */	/* ２回目以降の分列数は常に7回 */
//
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* 弾数 */
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((src->recursive)<<2);
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
		//
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_enemy	= callback_hit_zako;
		//
		//	h->base_score			= score(25*2);/*ダミー*/
			h->base_hp				= (9999);/*倒せない*/
		//
			h->m_angleCCW1024		= (i_angle1024);
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
			h->recursive	= ((0==is_the_first)?((src->recursive)-1):((3==difficulty)?(1):(0)));
		}
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
