
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	子供魔方陣 弾幕
	-------------------------------------------------------
	A.レーザーを撃つぜ
	(無駄に複雑だったので、
		A.レーザーを撃つ機能と、
		B.星型を描く機能と、
		C.360ナイフを撃つ機能と、
	を分離させた)
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif

#define my_difficulty		user_data04 		/* 難易度調整用 */

#define my_angle1024		tmp_angleCCW1024	/* 保持角度[レーザーの撃つ向きを維持する角度] */


/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

static void move_doll01_laser(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	if (0x0ff < src->boss_time_out)/* 0x100未満は共通規格上撃たない */
	{
		if (0x00==(src->boss_time_out&0x10f))/* */
		{
			/* CCWの場合 */
			obj_send1->cx256							= (src->cx256);/* 魔方陣の中心から弾撃つ */
			obj_send1->cy256							= (src->cy256);/* 魔方陣の中心から弾撃つ */
			br.BULLET_REGIST_00_speed256						= ((src->boss_time_out&0xff)/*>>4*/<<3/*1/10*/)/**256*/;
			br.BULLET_REGIST_07_VECTOR_tomari_dan_delta256		= t256(0.02);
		//
			br.BULLET_REGIST_05_regist_type 					= REGIST_TYPE_03_TOMARI;
			const int aaa = (src->my_angle1024)+(src->boss_time_out+src->boss_time_out);
			br.BULLET_REGIST_02_VECTOR_angle1024				= src->my_angle1024;				br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= aaa+cv1024r((180+  0));	bullet_regist_vector(); //ra d2deg512((src->boss_time_out)>>7/*/100*/)
			br.BULLET_REGIST_02_VECTOR_angle1024				= src->my_angle1024-cv1024r((120)); br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= aaa+cv1024r((180+120));	bullet_regist_vector(); //ra d2deg512((src->boss_time_out)>>7/*/100*/)
			br.BULLET_REGIST_02_VECTOR_angle1024				= src->my_angle1024+cv1024r((120)); br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= aaa+cv1024r((180-120));	bullet_regist_vector(); //ra d2deg512((src->boss_time_out)>>7/*/100*/)
		}
		if (0x100==(src->boss_time_out&0x103))/* */
		{
			src->my_difficulty--;
			if (0 > src->my_difficulty)
			{
				src->my_difficulty = (1+3-(cg_game_difficulty));/* luna,は1/4回, hard は1/8回, norma1/12回, easyは1/16回, */
			//
				voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
				obj_send1->cx256 = (src->cx256);/* 魔方陣の中心から弾撃つ */
				obj_send1->cy256 = (src->cy256);/* 魔方陣の中心から弾撃つ */
						br.BULLET_REGIST_07_VECTOR_div_angle1024		= cv1024r(120); 			/* 角度 */
						br.BULLET_REGIST_04_bullet_obj_type 			= BULLET_HARI32_00_AOI; 	/* [ 弾] */
						br.BULLET_REGIST_06_n_way						= (3);						/* [3way] */
						br.BULLET_REGIST_05_regist_type 				= REGIST_TYPE_00_MULTI_VECTOR;
				{unsigned int i;
					for (i=t256(5); i<t256(9); i+=t256(1))
					{
						/* CCWの場合 */
						br.BULLET_REGIST_00_speed256					= (i);						/* 弾速 */
						//
						br.BULLET_REGIST_02_VECTOR_angle1024			= src->my_angle1024;
						bullet_regist_vector();
					}
				}
				/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
				/* 24.44...  == 1024/41.8879008478638666666666666666667 */
				src->my_angle1024 -= (24);/* 右回り(CCWなので) */
			}
		}
	}
	//
	set_timeout_alpha(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

/*---------------------------------------------------------
	子供魔方陣 生成 	子供魔方陣グラフィック生成
---------------------------------------------------------*/

/*static*/ void add_zako_sakuya_doll_01_laser(SPRITE *src)
{
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
		h->cx256				= (src->cx256);
		h->cy256				= (src->cy256)-t256(16);/*咲夜 上方に配置*/
		#endif
		h->my_angle1024 		= (0);
		h->my_difficulty		= (0);	/* 難易度調整用 */
	//
		h->boss_time_out		= (0x0800); 	/* 制限時間 */
	}
}
