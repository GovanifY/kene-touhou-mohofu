
#include "bullet_object.h"

/*---------------------------------------------------------
	魅魔 オプション
	-------------------------------------------------------


---------------------------------------------------------*/

#define xcenter256			user_data00
#define ycenter256			user_data01
#define time_out			user_data02

#define rotate_angle512 	user_data03 	/* ボスを中心として、回転角度。(下CCW512形式) */
#define fire_wait1			user_data04 	/* */
#define shot_angle512		user_data05 	/* */


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_mima_doll(SPRITE *src)
{
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_mima_doll(SPRITE *src)
{
	/* オプション位置、回転移動 */
	src->rotate_angle512++;
	mask512(src->rotate_angle512);
	#define HANKEI_45_DOT (45)				/* 半径 */
	src->x256 = src->xcenter256 + ((sin512((src->rotate_angle512))*(HANKEI_45_DOT)));	/*CCW*/
	src->y256 = src->ycenter256 + ((cos512((src->rotate_angle512))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
//
	{
		/*	旧作 & 咲夜風、回転ショット */
		src->fire_wait1--;
		if (0 > src->fire_wait1)
		{
			src->fire_wait1 = (8);				/* 次のショットを撃つまでの間隔、時間。 */
			src->shot_angle512 += (512/18); 	/* ショットを撃つ方向を、回転させる。 */
		//
			send1_obj->x256 = src->x256+t256(6.0);/* 6.0[dots]右 */
			send1_obj->y256 = src->y256+t256(4.0);/* 4.0[dots]下 */
			send1_obj->BULLET_REGIST_speed256			=	(t256(1.5))+(((difficulty)<<6));	/* 弾速 */
			send1_obj->BULLET_REGIST_angle512			=	(src->shot_angle512);				/* */
			send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/160); 					/* 密着弾 */
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KUNAI12_01_AKA;				/* [赤クナイ弾] */
			send1_obj->BULLET_REGIST_n_way				=	(2+difficulty); 					/* [2-5way] */
			bullet_regist_basic();
		}
	}
//
	#if 1
	src->time_out--;
	if (0 > src->time_out)/* スペカシステムの都合で要る。 */
	{
		/* 魅魔を倒すと皆破壊される。 */
		if (0==draw_boss_hp_value)
		{
			src->type = SP_DELETE;
		}
	}
	#endif
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_mima_dolls(SPRITE *src)
{
//	#define ADD_ANGLE (171) 	/* 1周を3分割した角度、170.66 == 512/3 */
//	#define ADD_ANGLE (85)		/* 1周を6分割した角度、 85.33 == 512/6 */
	#define ADD_ANGLE (102) 	/* 1周を5分割した角度、102.40 == 512/5 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//
	int i_angle;	/* 積算角度 */
	for (i_angle=0; i_angle<(512);	i_angle += (ADD_ANGLE)) /* 一周 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= BOSS_16_YOUSEI11;
		h->flags				= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
//
		h->callback_mover		= move_mima_doll;
		h->callback_loser		= lose_mima_doll;
		h->callback_hit_enemy	= callback_hit_zako;
//
		h->xcenter256			= (src->x256);
		h->ycenter256			= (src->y256);
//
		h->base_hp				= (8192-1);
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
		h->rotate_angle512		= i_angle;
		h->fire_wait1			= (0);
		h->shot_angle512		= (0);
		h->time_out 			= (100);/*よくわかんない(スペカシステムのせいでうまくいかない)*/
	}
}
