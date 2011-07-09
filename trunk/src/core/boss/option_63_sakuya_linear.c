
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	咲夜の直線誘導ナイフ弾
	-------------------------------------------------------
	弾幕書き換え方式に移行するのでここは無くなる予定。
	-------------------------------------------------------
	Todo:
	src->giji_jyumyou を廃止して src->jyumyou で置き換える。
---------------------------------------------------------*/
#if 000

//typedef struct
//{
#define aaa_target_x256 						user_data00
#define aaa_target_y256 						user_data01
#define BULLET_SP1_DATA_angle1024				user_data02
#define BULLET_YUUDOU_DATA_hiritu256			user_data03
//} BULLET_SP1_DATA;

#define giji_jyumyou base_time_out

#if 1

static void s_tmp_angleCCW1024_src_nerai_auto(SPRITE *src, SPRITE *dest)
{
	if (ANGLE_JIKI_NERAI_DAN==dest->tmp_angleCCW1024)
	{
		/*s_*/tmp_angleCCW1024_src_nerai(src, dest);
	}
}
//static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void s_tmp_angleCCW1024_src_nerai_auto(SPRITE *src, SPRITE *t/*, int angle512*/);
static void move_bullet_sakuya_linear_yuudou(SPRITE *src)
{
	src->giji_jyumyou--;
	if (0 > src->giji_jyumyou)				/* 等速運動 */
	{
		/* 移動速度 t256(1.0) で等速移動 */
		src->vx256 = ((sin1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
		src->vy256 = ((cos1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
	}
	else
	if (0+2048 > src->giji_jyumyou) 		/* 誘導運動 */
	{
		const u8 m_tbl[4] =
		{
		//	0,1,3,7,	/* 遅い方が難しい */
			1,0,7,3,	/* ほかの攻撃との兼ね合いで、タイミングをとりづらい方が難しい */
		};
		/* 自分に集まる */
	//	if (0==(src->giji_jyumyou & (7>>difficulty) ))
	//	if (0==(src->giji_jyumyou & (7>>(3-difficulty)) ))/* 遅い方が難しい */
		if (0==(src->giji_jyumyou & (m_tbl[cg_game_difficulty]) ))
		{
			src->BULLET_YUUDOU_DATA_hiritu256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{	/* 差分 == (弾の現在座標 - 弾の誘導座標) */
			int x_sa256 = (src->cx256 - src->aaa_target_x256);
			int y_sa256 = (src->cy256 - src->aaa_target_y256);
			/* ある程度、誘導座標の近くまできたら */
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->giji_jyumyou = 0;	/* 等速運動へ */
				}
			}
			/* 加算差分 == (弾の差分座標 * 誘導比率) */
			int aaa_x256 = ((x_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			int aaa_y256 = ((y_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			src->cx256 = src->aaa_target_x256 + (aaa_x256); /*fps_factor*/
			src->cy256 = src->aaa_target_y256 + (aaa_y256); /*fps_factor*/
		}
	}
	else
	if (0+2048==src->giji_jyumyou)			/* 狙い弾に変身する */
	{
		src->type			= BULLET_KNIFE20_07_MIDORI;/* (青→緑ナイフに変身) */
		src->BULLET_SP1_DATA_angle1024 = ANGLE_JIKI_NERAI_DAN;
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		{
			src->tmp_angleCCW1024			= src->BULLET_SP1_DATA_angle1024;
			s_tmp_angleCCW1024_src_nerai_auto(zzz_player, src);
			src->BULLET_SP1_DATA_angle1024	= src->tmp_angleCCW1024;
		}
		mask1024(src->BULLET_SP1_DATA_angle1024);
		src->rotationCCW1024 = src->BULLET_SP1_DATA_angle1024;
		src->vx256 = (0);	/*fps_factor*/
		src->vy256 = (0);	/*fps_factor*/
		/* 誘導座標はプレイヤーの位置付近 */
		/* とりあえず、仮対応。強制的に設定しちゃう */
		src->aaa_target_x256	= (zzz_player->cx256);	/*fps_factor*/	// +t256(32/2)/*幅の半分*/
		src->aaa_target_y256	= (zzz_player->cy256);	/*fps_factor*/	// +t256(16/2)/*高さの半分*/
		src->BULLET_YUUDOU_DATA_hiritu256		= t256(1.0);	/* (2.5==5*0.5) */
	}
//
//	move_bullet_vector(src);
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
}
#endif

/*---------------------------------------------------------
	咲夜の直線誘導ナイフ
	-------------------------------------------------------
	この弾は他の弾が速いのであえて遅くする。
---------------------------------------------------------*/

global void bullet_crate_sakuya_linear_yuudou(SPRITE *src)
{
	const unsigned int knife_color = BULLET_KNIFE20_04_AOI+((ra_nd()&(4-1)));	/* ナイフの色はランダム */
	unsigned int j;
	for (j=(0); j<(8); j++)
	{
		SPRITE *h;
		h						= obj_add_00_tama_error();
		if (NULL != h)
		{
			h->cx256			= src->cx256;
			h->cy256			= src->cy256;
			h->type 			= (knife_color);/*S P_BULLET*/ /*SP_LASER*/
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		//
			h->callback_mover	= move_bullet_sakuya_linear_yuudou;
			h->giji_jyumyou = 2048+20+(40+1);		/*data->timer		= 0;*/

			int angle1024		= src->tmp_angleCCW1024+((j-4)*84); 	/* 85.333 == cv1024r(30) */
			mask1024(angle1024);
			h->rotationCCW1024 = angle1024;
			h->vx256 = ((sin1024((angle1024))*t256(1.0))>>8);/*fps_factor*/
			h->vy256 = ((cos1024((angle1024))*t256(1.0))>>8);/*fps_factor*/

			h->BULLET_YUUDOU_DATA_hiritu256 	= t256(1.0);	/* (2.5==5*0.5) */
		//
			#if 1
			/* とりあえず、基本版 */
		//	h->callback_2nd_regist	= callback_2nd_regist;
		//
			/* とりあえず、テキトー */
		//	h->BULLET_DATA_2nd_count	= (999/*30*/);		/* 30カウントしたら半分速 */
		//	h->BASE_add_speed256		= (t256(1.00)); 	/* 加速/減速、なし */
			#endif
		}
	}
}

#endif/* 000 */
