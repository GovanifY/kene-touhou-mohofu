
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	輝夜 オプション
	-------------------------------------------------------
---------------------------------------------------------*/

#define xcenter256			user_data00
#define ycenter256			user_data01

#define time_out			user_data03

#define rotate_angle1024	user_data04 	/* ボスを中心として、回転角度。(下CCW1024形式) */
//#define fire_wait1		user_data05 	/* */
//#define shot_angle1024	user_data06 	/* */


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_kaguya_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_kaguya_doll(SPRITE *src)
{
	check_boss_option_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	/* オプション位置、回転移動 */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	#define HANKEI_45_DOT (45)				/* 半径 */
	src->cx256 = src->xcenter256 + ((sin1024((src->rotate_angle1024))*(HANKEI_45_DOT))); 	/*CCW*/
	src->cy256 = src->ycenter256 + ((cos1024((src->rotate_angle1024))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
	src->color32		= (src->color32 & 0x00ffffff) | ((src->time_out<<(23))&0xff000000);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_kaguya_houmotsu(SPRITE *src)
{
//	#define ADD_ANGLE (171*2)	/* 1周を3分割した角度、170.66*2 == 1024/3 */
//	#define ADD_ANGLE (85*2)	/* 1周を6分割した角度、 85.33*2 == 1024/6 */
	#define ADD_ANGLE (102*2)	/* 1周を5分割した角度、102.40*2 == 1024/5 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//
	int i;
	i=0;
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* 一周 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_16_YOUSEI11+(i); 	i++;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_kaguya_doll;
			h->callback_loser		= lose_kaguya_doll;
			h->callback_hit_enemy	= callback_hit_zako;
	//
			h->xcenter256			= (src->cx256);
			h->ycenter256			= (src->cy256);
	//
			h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
			h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40万 */
		//
			h->rotate_angle1024 	= i_angle1024;
			h->time_out 			= (0x01ff); 	/* 制限時間 */
		}
	}
}
