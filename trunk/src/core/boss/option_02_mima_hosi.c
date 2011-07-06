
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔 オプション
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif
//	ボス共通規格と同じ(boss.hインクルードしてもしなくても対応)
#ifndef target_x256
	#define target_x256 		user_data00
#endif
#ifndef target_y256
	#define target_y256 		user_data01
#endif
#ifndef boss_time_out
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif
//

#define rotate_angle1024	user_data04 	/* ボスを中心として、回転角度。(下CCW1024形式) */
#define shot_angle1024		user_data05 	/* */


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_mima_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
//	item_create_for_boss(src, ITEM_CREATE_MODE_02);/* easyはボムを出さなくて済む位軟らかくした */
	#if 1
	item_create(src, SP_ITEM_05_TENSU, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
	#else
	{	int i;
		for (i=0; i<(5); i++)
		{
			lose_random_item(src);
		}
	}
	#endif
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_mima_doll(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	if ( (unsigned int)(0x01ff/3) < src->boss_time_out)/* 最後の 1/3 時間は撃って来ない。 */
	{
		/*	旧作 & 咲夜風、回転ショット */
		if (0 == ((src->boss_time_out)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			src->shot_angle1024 += (1024/18);	/* ショットを撃つ方向を、回転させる。 */
		//
			obj_send1->cx256 					= (src->cx256)+t256(6.0);/* 6.0[dots]右 */
			obj_send1->cy256 					= (src->cy256)+t256(4.0);/* 4.0[dots]下 */
			br.BULLET_REGIST_speed256			= (t256(1.5))+(((difficulty)<<6));	/* 弾速 */
			br.BULLET_REGIST_angle1024			= (src->shot_angle1024);			/* */
			br.BULLET_REGIST_div_angle1024		= (int)(1024/160);					/* 密着弾 */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [赤クナイ弾] */
			br.BULLET_REGIST_n_way				= (2+difficulty);					/* [2-5way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}
//
	/* オプション位置、回転移動 */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	#define HANKEI_45_DOT (45)				/* 半径 */
	src->cx256 = src->target_x256 + ((sin1024((src->rotate_angle1024))*(HANKEI_45_DOT)));	/*CCW*/
	src->cy256 = src->target_y256 + ((cos1024((src->rotate_angle1024))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
	src->color32		= (src->color32 & 0x00ffffff) | ((src->boss_time_out<<(23))&0xff000000);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_mima_dolls(SPRITE *src)
{
//	#define ADD_ANGLE (171*2)	/* 1周を3分割した角度、170.66*2 == 1024/3 */
//	#define ADD_ANGLE (85*2)	/* 1周を6分割した角度、 85.33*2 == 1024/6 */
	#define ADD_ANGLE (102*2)	/* 1周を5分割した角度、102.40*2 == 1024/5 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* 一周 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_mima_doll;
			h->callback_loser		= lose_mima_doll;
			h->callback_hit_enemy	= callback_hit_zako;
	//
			h->target_x256			= (src->cx256);
			h->target_y256			= (src->cy256);
	//
		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
		//	h->base_hp				= ((3*1024)-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
		//	h->base_hp				= (((difficulty+1)<<10)-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
			/* 難度上がると微妙に硬くなる(==倒すのにそれだけ時間がかかる) */
			h->base_hp				= (((1024)+(difficulty<<8))-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score( 100000), 	/* easy 10万 (計 50万==5x 10万) */
					score( 300000), 	/* norm 30万 (計150万==5x 30万) */
					score( 500000), 	/* hard 50万 (計250万==5x 50万) */
					score(1000000), 	/* luna100万 (計500万==5x100万) */
				};
			h->base_score			= difficulty_score_tbl[difficulty]; 	/* 難易度によるが、かなり稼げる。 */
		//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
			}
		//
			h->rotate_angle1024 	= i_angle1024;
			h->shot_angle1024		= (0);
			h->boss_time_out			= (0x01ff); 	/* 制限時間 / 予想以上に速く消える位に。 */
		}
	}
}
