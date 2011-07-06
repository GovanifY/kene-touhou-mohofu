
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風  ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	角度弾
	-------------------------------------------------------
	-------------------------------------------------------
	喰み出しチェックについて、
	「移動時に」喰み出しチェックをしない事が前提で設計されている弾幕が多い。
	この為「移動時に」システムで喰み出しチェックをしません。
	例えば画面端で跳ね返る弾幕は「移動時に」喰み出しチェックをしない事により実現している機能です。
	「移動と別で」弾幕リストを調べて「喰み出しチェック」を行います。
---------------------------------------------------------*/

/*
	角度弾では、ベクトル移動をしない。
	代わりに基点座標として使う。
*/
#if 1/* 角度弾規格(策定案) */
	#define tx256				vx256/* 基点座標x */
	#define ty256				vy256/* 基点座標y */
	//
	#define radius256			user_data01 	/* 半径 */
//	#define speed256			user_data02 	/* 加減速 */
	#define speed65536			user_data02 	/* 加減速 */
	#define tra65536			user_data03 	/* 加減速調整 */
	#define rotate1024			user_data04 	/* 回転角度調整 */
	#define bullet_status_mask	user_data05 	/* 画面外消去判定や反射機能 */
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
/* 何もしない(等速直線移動) */
static void move_bullet_vector_wide100(SPRITE *src)
{
	src->cx256 += (src->tx256);/*fps_factor*/
	src->cy256 += (src->ty256);/*fps_factor*/
}
#endif


/*---------------------------------------------------------
	角度弾の移動を行う
	喰み出しチェック機能はないので別でやる事
---------------------------------------------------------*/

static void move_bullet_angle01(SPRITE *src)
{
	#if 1/* 検討中(無くても出来るけど、あった方が簡単) */
	src->speed65536 		+= (src->tra65536); 	/* 加減速調整 */
	#endif
//	src->radius256			+= (src->speed256); 	/* 速度 */
	src->radius256			+= (src->speed65536>>8);	/* 速度 */
	#if 0/* 検討中(無くても出来るけど、あった方が簡単) */
	/* パチェ() とか ルーミア(交差弾) とかに必要(無くても出来るけど、あった方が簡単) */
	/* 交差弾の場合この方式だとグラが難しい。(?) */
	src->rotationCCW1024	+= (src->rotate1024);	/* 回転角度調整 */
	#endif
//
	src->cx256 = (src->tx256) + ((sin1024((src->rotationCCW1024))*(src->radius256))>>8);/*fps_factor*/
	src->cy256 = (src->ty256) + ((cos1024((src->rotationCCW1024))*(src->radius256))>>8);/*fps_factor*/
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
//	mask1024(src->rotationCCW1024);



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
		h									= sprite_add_444only_bullet_error();
		if (NULL != h)
		{
			h->type 						= (br.BULLET_REGIST_bullet_obj_type);
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R					= TAMA_ATARI_JIPPOU32_PNG;
		//
		//	h->speed256 					= ((br.BULLET_REGIST_speed256)	 ); 	/* 速度 */	/* 初速(打ち出し速度) */
			h->speed65536					= ((br.BULLET_REGIST_speed256)<<8); 	/* 速度 */	/* 初速(打ち出し速度) */
			h->callback_mover				= move_bullet_angle01;
			#if 1/*Gu(中心座標)*/
			h->tx256						= /*src*/obj_send1->cx256;
			h->ty256						= /*src*/obj_send1->cy256;
			#endif
//
//			h->base_time_out				= (120);/*(100)*/
			i_angle65536					+= (br.BULLET_REGIST_div_angle65536);	/*(32)*/	/* (1024-64)(30<<4) (1<<4) ICE_NU M_OF_ENEMIES */
			mask65536(i_angle65536);
			h->rotationCCW1024				= (i_angle65536>>6);/* (i<<4) deg_360_to_512(90) */
//
			h->radius256					= t256(0);/* 半径 */
//			h->tra65536 					= ((br.BU LLET_REGIST_speed_offset)<<8);	/* 調整減速弾 */	/* この方式になるか検討中 */
			h->tra65536 					= ((br.BULLET_REGIST_speed_offset));	/* 調整減速弾 */	/* この方式になるか検討中 */
			h->rotate1024					= (0);		/* この方式になるか検討中 */
			h->bullet_status_mask			= (0);		/* この方式になるか検討中 */
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
