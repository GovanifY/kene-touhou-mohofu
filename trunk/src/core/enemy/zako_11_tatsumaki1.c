
#include "bullet_object.h"

/*---------------------------------------------------------
		"竜巻1",		"PROBALL",
	-------------------------------------------------------
	バグあり？？？
	-------------------------------------------------------
→	↓	 ←
↑. ↓	/↑
↑ .↓ / ↑
↑	↓/  ↑
←←++→→
   /  .
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;			/* 状態 */
	int kaiten_houkou;	/* 回転方向 */
	int speed256;		/* 速度 */
	int px256;			/* 仮想x位置(反転用) */
//	int enemy_rank;		/* 設定ファイルからの敵の強さ */
} TATSUMAKI1_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (24)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_tatsumaki1(SPRITE *src)
{
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		if (rand_percent(50/*10*/))
		{
			item_create(/*zzz*/ src,	SP_ITEM_04_BOMB, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
};
static void move_tatsumaki1(SPRITE *src)
{
	TATSUMAKI1_DATA *data = (TATSUMAKI1_DATA *)src->data;
	data->speed256++;	/* t256(0.1) t256(0.2)速すぎ */
	switch (data->state)
	{
	case SS00:/* 下へ移動 */
		src->y256 += t256(1.0)/**fps_fa ctor*/;
		if (t256(GAME_HEIGHT/3/*2*/) < src->y256 )	/*t256(250)*/
		{
			data->state++;/* = SS01;*/
		}
		break;
	case SS01:/* 上へ移動 */
		src->y256 -= t256(1.125)/**fps_fa ctor*/;
		if (0 > src->y256 )
		{
		//	data->speed256 = (-(data->speed256));/* 反転 */
			data->state++;/*  = SS02;*/
		}
		break;
	case SS02:/* 下へ移動 */
		src->y256 += t256(1.25)/**fps_fa ctor*/;
		if (t256(GAME_HEIGHT) < src->y256 )
		{
			src->type = SP_DELETE;	/* おしまい */
		//	src->flags &= (~(SP_FLAG_VISIBLE));
		}
		break;
	}
	data->px256 -= (data->speed256)/**fps_fa ctor*/;
	if ((0 > data->px256)||(t256(GAME_WIDTH-10) < data->px256))
	{
		data->speed256 = (-(data->speed256));/* 反転 */
	}
//
	if (0 == data->kaiten_houkou)
			{	src->m_angleCCW512 += 5;	src->x256	= (data->px256); 	}
	else	{	src->m_angleCCW512 -= 5;	src->x256	= t256(GAME_WIDTH)-(data->px256); }
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_tatsumaki1(STAGE_DATA *l)/*int lv*/
{
//	int enemy_rank;	enemy_rank	= l->user_y;
//
	destoroy = 0;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_TATSUMAKI16_PNG); 	//s->anim_speed=2/*3*/;/*11"pr otectball.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed=2/*3*/;/*11"pr otectball.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_62_TATSUMAKI/*_10_TATSUMAKI1*/;
//		s->type 				= SP_ZAKO/*_10_TATSUMAKI1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_tatsumaki1;
		s->callback_loser		= lose_tatsumaki1;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= (i&(16-1));/*(i&(8-1)) (i%11)*/
		s->m_angleCCW512		= ((i&(16-1))<<5);
		s->y256 				= -(((i+1)*(t256(24)/*s->h128+s->h128*/)));
		TATSUMAKI1_DATA *data;
		data					= mmalloc(sizeof(TATSUMAKI1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(10*2);
		/*data->base.*/s->base_health		= (4*8)+(difficulty<<2);/*やわらかすぎ*/	/*(2+(difficulty<<2))*/
		data->state 			= SS00;
		data->speed256			= t256(1.2);/* t256(2.0)速すぎ */	/*3+difficulty+lv/3*/
		data->kaiten_houkou 	= (i&1);/*(0==i%2)?(0):(1)*/
		data->px256 				= /*0*/t256(GAME_WIDTH-10)/*t256(GAME_WIDTH/2)-((s->w128))*/;
//		data->enemy_rank 			= enemy_rank;
	}
}
#undef NUM_OF_ENEMIES
