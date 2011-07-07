
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	パチェレーザー
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif
//	ボス共通規格と同じ(boss.hインクルードしてもしなくても対応)
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif
//

#define target_x256 	user_data00 	/* 基準固定 x位置 */
#define target_y256 	user_data01 	/* 基準固定 y位置 */
//#define time_out		user_data03 	/* 経過時間 */
#define radius			user_data04 	/* 円の半径 */
#define rotate			user_data05 	/* 回転方向 */



#define my_angle1024		tmp_angleCCW1024	/* 保持角度[レーザーの撃つ向きを維持する角度] */


/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_doll01_laser(SPRITE *src)
{
//	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	src->boss_time_out--;
	if (0 > src->boss_time_out)
	{
		src->jyumyou		= JYUMYOU_NASI;
	}

	if ((256-85)<(src->boss_time_out))
	{
		src->radius++;
	}
	else
	{
	//	if (0x00==(src->boss_time_out&0x03))/* */
		{
//			voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
			obj_send1->cx256							= (src->cx256);/* 魔方陣の中心から弾撃つ */
			obj_send1->cy256							= (src->cy256);/* 魔方陣の中心から弾撃つ */

			br.BULLET_REGIST_00_speed256				= t256(2.0);				/* 弾速 */
			br.BULLET_REGIST_01_speed_offset			= t256(0);/*(テスト)*/
			br.BULLET_REGIST_02_angle65536				= ((src->my_angle1024)<<6);
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_00_AOI; 	/* [ 青針弾] */
		//未定br.BULLET_REGIST_05_regist_type			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
			br.BULLET_REGIST_06_n_way					= (3);						/* [3way] */
			br.BULLET_REGIST_07_div_angle65536			= (65536/3);				/* 分割角度 */
			/* CCWの場合 */
			tama_system_regist_katayori_n_way();/* (r33-) */
			src->my_angle1024 -= (src->rotate);/* 回転する */
			mask1024(src->my_angle1024);
		}
	}
//
			src->cx256 = src->target_x256 + ((sin1024((src->my_angle1024))*src->radius));/*fps_factor*/
			src->cy256 = src->target_y256 + ((cos1024((src->my_angle1024))*src->radius));/*fps_factor*/
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*---------------------------------------------------------
	子供魔方陣 生成 	子供魔方陣グラフィック生成
---------------------------------------------------------*/

/*static*/ void add_zako_pache_laser(SPRITE *src)
{
	static u8 aaa_count;
	aaa_count++;
	SPRITE *h;
	h						= obj_add_01_teki_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= BOSS_16_YOUSEI11;
		h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
		h->callback_mover		= move_doll01_laser;
		h->base_hp				= (9999);/* 倒せない */

	//	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0x00);	/*	obj_doll->alpha 		= 0x00;*/
		h->color32				= MAKE32RGBA(0xff, 0x88, 0xaa, 0x80);	/*	obj_doll->alpha 		= 0x00;*/ /* 紫っぽく */
		/* 子供魔方陣、配置位置 */
		#if 1
		h->target_x256			= (src->cx256);
		h->target_y256			= (src->cy256);
		#endif
		h->radius				= (0);
		if (0==(aaa_count & 0x02))
		{
			h->my_angle1024 		= (1024/6);
			h->rotate				= (1);
		}
		else
		{
			h->my_angle1024 		= (1024)-(1024/6);
			h->rotate				= (-1);
		}
	//
		h->boss_time_out		= (256);	/* 制限時間 */
	}
}
