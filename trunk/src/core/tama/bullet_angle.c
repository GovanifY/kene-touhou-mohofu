
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	角度弾
	-------------------------------------------------------
	-------------------------------------------------------
	喰み出しチェックについて、
	喰み出しチェックは毎フレーム行う必要が無い処理である。
	例えば 8 フレームに一回チェックしても、
	実際には毎フレームチェックするのと殆ど変わらない。
	(「チェック負荷」が高いのに弾数が削減されない。)
	柔軟なチェックの為自前で、チェックする方式になっている。
	おそらく本家にもこの仕組みはある。
	-------------------------------------------------------
	※「チェック負荷」実際にほぼ０だろうが、ある事はある。
	移動するという事は、弾の座標がレジスタに入った場合なので、
	その時点でチェックも行うという考え方は、これはこれで効率的であるが、
	「チェックの方式が様々に変わる場合」や「毎フレームチェックしない場合」に
	比べると非効率である。
	(レジスタにロードする効率 ＜ アルゴリズムで改善する効率)
	という事。
	仮に
---------------------------------------------------------*/


/*
	角度弾では、ベクトル移動をしない。
	代わりに基点座標として使う。
*/
#define tx256				vx256/* 基点座標x */
#define ty256				vy256/* 基点座標y */

#define radius256			user_data01 	/* 半径 */
#define speed256			user_data02 	/* 加減速 */
#define tra256				user_data03 	/* 加減速調整 */
//#define mask				user_data04 	/* 画面外消去判定や反射機能 */


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
/* 何もしない(等速直線移動) */
static void move_bullet_vector_wide100(SPRITE *src)
{
	src->x256 += (src->tx256);/*fps_factor*/
	src->y256 += (src->ty256);/*fps_factor*/

//	move_bullet_vector_wide100(src);
//	gamen_gai_nara_osimai(src);/* 画面外ならおしまい */

	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
		(src->y256 < t256(-256))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{
		src->type = SP_DELETE;
	}
}
#endif

//	switch (src->BOSS02_ICE_DATA_state) case KS00:	/* 円状運動 */
	//	src->BOSS02_ICE_DATA_state = src->BOSS02_ICE_DATA_next_2nd_state;
	//	src->BOSS02_ICE_DATA_state++/* = KS01*/;

/*---------------------------------------------------------
	角度弾の移動を行う
	喰み出しチェック機能はないので別でやる事
---------------------------------------------------------*/

static void move_bullet_angle01(SPRITE *src)
{
	src->speed256	+= (src->tra256);		/* 加減速調整 */
	src->radius256	+= (src->speed256); 	/* 速度 */
//
	src->x256 = (src->tx256) + ((sin1024((src->m_angleCCW1024))*(src->radius256))>>8);/*fps_factor*/
	src->y256 = (src->ty256) + ((cos1024((src->m_angleCCW1024))*(src->radius256))>>8);/*fps_factor*/
}

//	move_bullet_vector_wide100(src);
	#if 0/* 検討中 */
	check_bullet_angle01(src);
	#endif

//	src->base_time_out--;/*fps_factor*/
//	if (src->base_time_out < 0)
//	{
//		src->callback_mover 				= move_bullet_vector_wide100;
//	}
//	/* 表示用 */


//
//	mask1024(src->m_angleCCW1024);

/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_change_meirin_yajirusi_one(SPRITE *h)
{
	/* 現在の弾座標を、基点座標にする。 */
	h->tx256 = h->x256;/*fps_factor*/
	h->ty256 = h->y256;/*fps_factor*/
	//
//	h->type 						= (BULLET_KUNAI12_00_AOI+1);/* 青弾→赤弾に変身 */
//	h->speed256 					= t256(0.5);		/* 速度 */	/* 初速(打ち出し速度) */
//	h->speed256 					= t256(0.0)+((h->radius256)>>5);	/* 半径が大きい程、初速が速い */	/* 初速(打ち出し速度) */
	h->speed256 					= t256(0.0)+((h->radius256)>>6);	/* 半径が大きい程、初速が速い */	/* 初速(打ち出し速度) */
	h->radius256					= t256(0);/* 半径 */
	h->tra256						= (1);		/* 調整加速弾 */
	const int ao_aka_tbl[(2)]	=
	{
	//	 (1024/2)+(1024/32),/* 180/360ちょい回転 */ 	/* 青 */
	//	-(1024/2)-(1024/32),/* 180/360ちょい回転 */ 	/* 赤 */
		 (1024/2)+(1024/24),/* 180/360ちょい回転 */ 	/* 青 */
		-(1024/2)-(1024/24),/* 180/360ちょい回転 */ 	/* 赤 */
	};
//	h->m_angleCCW1024				+= (1024/2)+(1024/16);/* 180/360ちょい回転 */
//	h->m_angleCCW1024				+= (1024/2)+(1024/32);/* 180/360ちょい回転 */
	h->m_angleCCW1024				+= ao_aka_tbl[((br.BULLET_REGIST_bullet_obj_type)&1)];/* 180/360ちょい回転 */
	mask1024(h->m_angleCCW1024);
}
global void exchange_damnaku_check_type(void)
{
	int check_type;
	check_type = br.BULLET_REGIST_bullet_obj_type;/* 調べるタイプを受け取る */
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
		if (check_type == (s->type) )	/* 矢印の青弾か赤弾なら */
		{
			s_change_meirin_yajirusi_one(s);
		}
	}
}
/*---------------------------------------------------------
	天狗用
	ICE弾
	-------------------------------------------------------
---------------------------------------------------------*/
/*BULLET_CAP16_04_KOME_SIROI*/	/*S P_BULLET*/ /*S P_BO SS02ICE*/
static void s_bullet_regist_multi_angle(void)
{
	int i_angle65536;
	i_angle65536	= (br.BULLET_REGIST_angle65536);/* 開始向き */
//
	int i;
	for (i=(0); i<(br.BULLET_REGIST_n_way); i++)
	{
		SPRITE		*h;
		h									= sprite_add_only_bullet_error();
		if (NULL != h)
		{
			h->m_Hit256R					= TAMA_ATARI_JIPPOU32_PNG;
			h->type 						= (br.BULLET_REGIST_bullet_obj_type);
			h->speed256 					= (br.BULLET_REGIST_speed256);		/* 速度 */	/* 初速(打ち出し速度) */
			h->callback_mover				= move_bullet_angle01;
			#if 1/*Gu(中心座標)*/
			h->tx256						= /*src*/obj_send1->x256;
			h->ty256						= /*src*/obj_send1->y256;
			#endif
//
//			h->base_time_out				= (120);/*(100)*/
			i_angle65536					+= (br.BULLET_REGIST_div_angle65536);	/*(32)*/	/* (1024-64)(30<<4) (1<<4) ICE_NU M_OF_ENEMIES */
			mask65536(i_angle65536);
			h->m_angleCCW1024				= (i_angle65536>>6);/* (i<<4) deg_360_to_512(90) */
//
			h->radius256					= t256(0);/* 半径 */
			h->tra256						= (br.BULLET_REGIST_speed_offset);		/* 調整減速弾 */	/* この方式になるか検討中 */
		}
	}
}


/*---------------------------------------------------------
	弾を登録する。
	-------------------------------------------------------
	関数を(クラスっぽく)隠蔽。
	弾発動時のエフェクト等の関係上、発弾部分は全部纏める必要がある。
	-------------------------------------------------------
	ベクトル弾と角度弾を統合するかもしれない。
---------------------------------------------------------*/

global void bullet_regist_angle(void)
{
	s_bullet_regist_multi_angle();
}
