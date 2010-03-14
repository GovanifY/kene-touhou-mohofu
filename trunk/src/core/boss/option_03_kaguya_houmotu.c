
#include "bullet_object.h"

/*---------------------------------------------------------
	輝夜 オプション
	-------------------------------------------------------


---------------------------------------------------------*/

#define xcenter256			user_data00
#define ycenter256			user_data01
#define time_out			user_data02

#define rotate_angle512 	user_data03 	/* ボスを中心として、回転角度。(下CCW512形式) */
//#define fire_wait1			user_data04 	/* */
//#define shot_angle512 	user_data05 	/* */


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
	/* オプション位置、回転移動 */
	src->rotate_angle512++;
	mask512(src->rotate_angle512);
	#define HANKEI_45_DOT (45)				/* 半径 */
	#if 0
	src->x256 = (pd_boss)->x256 + ((sin512((src->rotate_angle512))*(HANKEI_45_DOT)));	/*CCW*/
	src->y256 = (pd_boss)->y256 + ((cos512((src->rotate_angle512))*(HANKEI_45_DOT)));
	#else
	src->x256 = src->xcenter256 + ((sin512((src->rotate_angle512))*(HANKEI_45_DOT)));	/*CCW*/
	src->y256 = src->ycenter256 + ((cos512((src->rotate_angle512))*(HANKEI_45_DOT)));
	#endif
	#undef HANKEI_45_DOT
//
	#if 1
	src->time_out--;
	if (0 > src->time_out)/* スペカシステムの都合で要る。 */
	{
		/* 輝夜を倒すと皆破壊される。 */
		if (0==draw_boss_hp_value)
		{
			src->type = SP_DELETE;
		}
	}
	#endif
}

void add_zako_kaguya_houmotsu(SPRITE *src)
{
//	#define ADD_ANGLE (171) 	/* 1周を3分割した角度、170.66 == 512/3 */
//	#define ADD_ANGLE (85)		/* 1周を6分割した角度、 85.33 == 512/6 */
	#define ADD_ANGLE (102) 	/* 1周を5分割した角度、102.40 == 512/5 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//
	int i;
	i=0;
	int i_angle;	/* 積算角度 */
	for (i_angle=0; i_angle<(512);	i_angle += (ADD_ANGLE)) /* 一周 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= BOSS_16_YOUSEI11+(i); 	i++;
		h->flags				= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		h->callback_mover		= move_kaguya_doll;
		h->callback_loser		= lose_kaguya_doll;
		h->callback_hit_enemy	= callback_hit_zako;
//
		h->xcenter256			= (src->x256);
		h->ycenter256			= (src->y256);
//
		h->base_hp				= ((8*1024)-1);
		h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40万 */
	//
		h->rotate_angle512		= i_angle;
		h->time_out 			= (100);/*よくわかんない(スペカシステムのせいでうまくいかない)*/
	}
}
	//		/* 4面の場合 */
	//	h->base.health			= (difficulty<<8/*[x256]*/)+ 200+1024;
//		h->base.life			= (((difficulty<<8/*[x256]*/)+ 200+1024)>>10);
	//	h->base.score			= score(600)*(difficulty+1);
