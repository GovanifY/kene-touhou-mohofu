
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	弾幕
	-------------------------------------------------------
---------------------------------------------------------*/

//typedef struct
//{
#define EVEN_KNIFE_DATA_angle1024					user_data00 	/*union BULLET_ANGLE_DATA*/
#define EVEN_KNIFE_DATA_speed256					user_data01 	/*union BULLET_ANGLE_DATA*/
//
	//int time_out; // act

#define EVEN_KNIFE_DATA_d_angle1024					user_data02
//	int length256;	t256(128)
#define EVEN_KNIFE_DATA_x256						user_data03
#define EVEN_KNIFE_DATA_y256						user_data04
#define EVEN_KNIFE_DATA_state						user_data05
#define EVEN_KNIFE_DATA_wait1						user_data06 	/* fire */

#define EVEN_KNIFE_DATA_l_or_r						user_data07 	/* -1==l, 1==r */
//} EVEN_KNIFE_DATA;

/*---------------------------------------------------------
	自機狙い弾の角度を計算
---------------------------------------------------------*/
static void s_tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *t)
{
	#if 1
	t->tmp_angleCCW1024 	= (atan_1024((p->y256-t->y256), (p->x256-t->x256)));
	/* 偶数弾の場合に自機狙い(簡略版) 弾と自分が大きさが同じならずれない、違うとその分誤差になる */
	#endif
}
/*---------------------------------------------------------

	-------------------------------------------------------
	s->x256 	ナイフのx座標
	data->x256	ノードのx座標
	s->w/2		ナイフの横幅の半分
	ナイフのx座標 == ノードのx座標 + co_s1024((data->angle1024)) x length - ナイフの横幅の半分
---------------------------------------------------------*/
#define ryoute_knife_length256 t256(128)
//static void enemy_evenr_knife_move(SPRITE *src)
static void enemy_even_knife_move(SPRITE *src)
{
	switch (src->EVEN_KNIFE_DATA_state)
	{
	case 0:
		if (100 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 1;
		}
		src->EVEN_KNIFE_DATA_angle1024 -= (src->EVEN_KNIFE_DATA_d_angle1024*src->EVEN_KNIFE_DATA_l_or_r); 	/*fps_factor*/
		mask1024(src->EVEN_KNIFE_DATA_angle1024);
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->m_angleCCW1024 = (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*1/4)*src->EVEN_KNIFE_DATA_l_or_r));
		mask1024(src->m_angleCCW1024);/*必ず要る*/
	//	src->x256		= data->x256-((cos512((data->angle512))*data->length256)>>8)-((src->w128));
	//	src->y256		= data->y256-((sin512((data->angle512))*data->length256)>>8)-((src->h128));
	//	src->x256		= data->x256-((sin512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->w128));
	//	src->y256		= data->y256-((cos512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->h128));
		#if 1/*Gu(中心座標)*/
		src->x256		= src->EVEN_KNIFE_DATA_x256-((sin1024((src->EVEN_KNIFE_DATA_angle1024)))<<7 );
		src->y256		= src->EVEN_KNIFE_DATA_y256-((cos1024((src->EVEN_KNIFE_DATA_angle1024)))<<7 );
		#endif
		break;

	case 1:
		if (50 < src->EVEN_KNIFE_DATA_wait1)
		{
			src->EVEN_KNIFE_DATA_wait1 = 0;
			src->EVEN_KNIFE_DATA_state = 0;
		}
		//#if 1
		//data->speed=20/*speed*/;/*よくわかんないが止まっちゃう*/
		//#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin1024((src->EVEN_KNIFE_DATA_angle1024))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos1024((src->EVEN_KNIFE_DATA_angle1024))*src->EVEN_KNIFE_DATA_speed256)>>8);/*fps_factor*/
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
		break;
	}
	if (10 < src->EVEN_KNIFE_DATA_wait1)
	{
		src->EVEN_KNIFE_DATA_wait1 = 0;
		#if 1
		obj_send1->x256 = src->x256;
		obj_send1->y256 = src->y256;
//
	//	br.BULLET_REGIST_div_angle1024		= (int)(1024/(18));			/* ダミー分割角度(1wayなので使用していない) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;		/* [青ナイフ弾] */
		br.BULLET_REGIST_n_way				= (1);	/* [1way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
		br.BULLET_REGIST_speed256			= (t256(2.5)/*t256(5.0)*/);
		br.BULLET_REGIST_angle1024			= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*3/12)*src->EVEN_KNIFE_DATA_l_or_r));
		bullet_regist_vector();
		//
		br.BULLET_REGIST_speed256			= (t256(2.2)/*t256(4.0)*/);
		br.BULLET_REGIST_angle1024			= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*4/12)*src->EVEN_KNIFE_DATA_l_or_r));
		bullet_regist_vector();
//
		br.BULLET_REGIST_speed256			= (t256(2.0)/*t256(3.0)*/);
		br.BULLET_REGIST_angle1024			= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*5/12)*src->EVEN_KNIFE_DATA_l_or_r));
		bullet_regist_vector();
//
		if (1<difficulty)
		{
			br.BULLET_REGIST_speed256		= (t256(1.8)/*t256(2.0)*/);
			br.BULLET_REGIST_angle1024		= (src->EVEN_KNIFE_DATA_angle1024+((int)(1024*6/12)*src->EVEN_KNIFE_DATA_l_or_r));
			bullet_regist_vector();
		}
		#endif
	}
	src->EVEN_KNIFE_DATA_wait1++;
	/*data->*/src->base_time_out--;
	if ( /*data->*/src->base_time_out < 0 )
	{
		src->type = SP_DELETE;
	}
}


/*---------------------------------------------------------
	両手ナイフ(青)
	-------------------------------------------------------
	length	enemy-obj_player間の距離/√2
	r_or_l	0==右	1==左
---------------------------------------------------------*/

global void bullet_create_sakuya_ryoute_knife(SPRITE *src)
//, int speed256/*, int dummy_length256*/, int r_or_l)/*0==右*/ 	/*1==左*/
{
	int speed256;
	speed256	= t256(10);
	int r_or_l;
//	for (r_or_l=0; r_or_l<2; r_or_l++)
	for (r_or_l=-1; r_or_l<2; r_or_l+=2)
	{
		SPRITE *h;
		h					= sprite_add_only_bullet_error();/*全方向ナイフ(青)*/ /*h->use_alpha=0 ????*/
		if (NULL != h)
		{
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
			h->type 			= BULLET_KNIFE20_04_AOI;/*S P_BULLET*/ /*SP_LASER*/
		//
			#if 1/*Gu(中心座標)*/
			h->x256 			= src->x256;
			h->y256 			= src->y256;
			#endif
			/*data->*/h->base_time_out		= (150+difficulty*50);
		//
			s_tmp_angleCCW1024_jikinerai(obj_player, src);/*???(original)*/ /* 奇数弾の場合に自機狙い */
			int tmp1024;
			tmp1024 = src->tmp_angleCCW1024;
			h->callback_mover	= enemy_even_knife_move;
			#if 0
		//	if (1==r_or_l)	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0!=r_or_l)
			{
		//		h->callback_mover	= enemy_evenl_knife_move;
				tmp1024 += (128)/*1024*1/8*/;
				data->l_or_r			= -1;
			}
			else
		//	if (0==r_or_l)
			{
		//		h->callback_mover	= enemy_evenr_knife_move;
				tmp1024 -= (128)/*1024*1/8*/;
				data->l_or_r			= 1;
			}
			#else
			h->EVEN_KNIFE_DATA_l_or_r			= r_or_l;
			tmp1024 -= (128*r_or_l)/*1024*1/8*/;
			#endif
			mask1024(tmp1024);
		//	if (0==length256)	{length256=1*256;}
		//	data->length256 	= length256;
		//	data->x256			= /*s->x256+((s->w128))*/h->x256+((cos1024((tmp1024))*length256)>>8);
		//	data->y256			= /*s->y256+((s->h128))*/h->y256+((sin1024((tmp1024))*length256)>>8);
		//	data->x256			= /*s->x256+((s->w128))*/h->x256+((sin1024((tmp1024))*ryoute_knife_length256/*length256*/)>>8);
		//	data->y256			= /*s->y256+((s->h128))*/h->y256+((cos1024((tmp1024))*ryoute_knife_length256/*length256*/)>>8);
			h->EVEN_KNIFE_DATA_x256 		= /*s->x256+((s->w128))*/h->x256+((sin1024((tmp1024)) )<<7 );
			h->EVEN_KNIFE_DATA_y256 		= /*s->y256+((s->h128))*/h->y256+((cos1024((tmp1024)) )<<7 );
			h->EVEN_KNIFE_DATA_angle1024 	= ((tmp1024));
		//	h->EVEN_KNIFE_DATA_d_angle1024	= ra d2deg1024(speed/(M_PI*2*length));	//⊿angle≒任意/(2π×length/√2)
		//	int tmp_1024;
		//	tmp_1024 = speed/(/*1024*/1024*length); 	// ⊿angle ≒ 任意/(2π×length/√2)
			h->EVEN_KNIFE_DATA_d_angle1024	= (1+1);//	  /*ra d2deg1024*/(/*deg1024_2rad*/(tmp_1024));
		//
			h->EVEN_KNIFE_DATA_speed256 	= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*よくわかんないが止まっちゃう*/
			h->EVEN_KNIFE_DATA_state		= 0;
			h->EVEN_KNIFE_DATA_wait1		= 0;
		}
	}
}
