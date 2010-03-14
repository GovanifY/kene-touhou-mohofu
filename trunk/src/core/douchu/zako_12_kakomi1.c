
#include "douchu.h"

/*---------------------------------------------------------
		"囲妖怪1",		"CUBE",
	-------------------------------------------------------

---------------------------------------------------------*/

#define radius256		user_data00
#define angle512		user_data01
#define time_out		user_data02


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_kakomi1(SPRITE *src)
{
	src->time_out--; /*fps_factor*/
	if (0 < src->time_out)
	{
		{
			src->radius256 -= t256(1.0);		/*fps_factor*/
		}
		if (0==(ra_nd()&(512-1)))	/* 1/512 ← 1/500 の確率で弾打つ */ 			/*%500*/
		{
			bullet_create_aka_maru_jikinerai(src, t256(1.0));
		}
		if (0==(ra_nd()&(512-1)))	/* 1/512 ← 1/600 の確率確率でアイテム出す */	/*%600*/
		{
			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
		src->angle512--;	/*fps_factor*/	/*deg_360_to_512(3)*/	/*2*/	/* deg_360_to_512(3) 速すぎる */
		mask512(src->angle512); //if (src->angle360<0)	{	src->angle360+=360;}
	}
	else
	{
		src->radius256 += t256(1.2/*2.0*/); 		/*fps_factor*/
		if (src->radius256 > t256(350.0))
		{
			src->type = SP_DELETE;	/* おしまい */
		}
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);	/* アニメーション */
	}

/* CCWの場合 */
	src->x256=((sin512((src->angle512))*(src->radius256))>>8)+t256(GAME_WIDTH/2);
	src->y256=((cos512((src->angle512))*(src->radius256))>>8)+t256(GAME_HEIGHT/2);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_kakomi1(STAGE_DATA *l)
{
//	int enemy_rank; 	enemy_rank	= l->user_y;	/* 設定ファイルからの敵の強さ */
/*
;[16分割]
  0,  32,  64,  96,
128, 160, 192, 224,
256, 288, 320, 352,
384, 416, 448, 480,
[0==512,]
;[13分割]
  0,   0, -0
 39,  40, -1
 78,  80, -2
118, 120, -2
157, 160, -3
196, 200, -4
236, 240, -4
275, 280, -5
314, 320, -6
354, 360, -6
393, 400, -7
432, 440, -8
472, 480, -8
*/
	{
		SPRITE *h;
		h							= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 					= TEKI_54_CHOU1;
		h->flags					|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover			= move_kakomi1;
		h->callback_hit_enemy		= callback_hit_zako;
//
		/*h->base.*/h->base_score	= score(15*2)*(1+2/*enemy_rank*/);
		/*h->base.*/h->base_hp		= (8*30)+/*enemy_rank+*/(1/*di fficulty*/<<(2+3));	/*1+lv+(di fficulty<<2)*/
		h->radius256				= t256(350);
		h->angle512 				= (l->user_x);/*(i<<5)*/	//	/*360*/(512/16)*i;
		h->time_out 				= (295);
	}
}

