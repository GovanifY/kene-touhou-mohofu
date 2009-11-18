
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
			item_create(/*zzz*/ src,	SP_ITEM_02_BOMB, 1, ITEM_MOVE_FLAG_06_RAND_XY);
		}
	}
}
/*50%(SP_ITEM_01_P008 or SP_ITEM_02_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_02_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_tatsumaki1(SPRITE *src)
{
	TATSUMAKI1_DATA *data = (TATSUMAKI1_DATA *)src->data;
	switch (data->state)
	{
	case 0:
		src->y256 += data->speed256/**fps_fa ctor*/;
		if (src->y256 >= t256(GAME_HEIGHT/2)/*250*/ )
		{
			data->speed256 = t256(1)/*2*/;/*速すぎ*/
			data->state=1;
		}
		break;
	case 1:
		data->speed256 += t256(0.1);
		if (0 == data->kaiten_houkou)
		{
			src->x256 -= data->speed256/**fps_fa ctor*/;
			if (src->x256 <= t256(10))
			{
				src->x256 = t256(10);
				data->speed256 = t256(2)/*3*/;/*速すぎ*/
				data->state=2;
			}
		}
		else
		{
			src->x256 += data->speed256/**fps_fa ctor*/;
			if (src->x256 >= t256(GAME_WIDTH-10)-((src->w128+src->w128)))
			{
				src->x256 = t256(GAME_WIDTH-10)-((src->w128+src->w128));
				data->speed256 = t256(2)/*3*/;/*速すぎ*/
				data->state = 2;
			}
		}
		break;
	case 2:
		src->y256 -= data->speed256/**fps_fa ctor*/;
		if (src->y256 <= 0)
		{
			src->y256 = 0;
			data->speed256 = t256(1)/*2*/;/*速すぎ*/
			data->state=3;
		}
		break;
	case 3:
		data->speed256 += t256(0.1)/*0.2*/;
		src->y256 += data->speed256/**fps_fa ctor*/;
		if (0 == data->kaiten_houkou)
		{
			src->x256 += data->speed256/**fps_fa ctor*/;
		}
		else
		{
			src->x256 -= data->speed256/**fps_fa ctor*/;
		}
		if (src->y256 > t256(GAME_HEIGHT) )/*>=*/
		{
			src->type = SP_DELETE;	/* おしまい */
		//	src->flags &= (~(SP_FLAG_VISIBLE));
		}
		break;
	}
//
	if (0 == data->kaiten_houkou)
			{	src->m_angleCCW512 += 5;	}
	else	{	src->m_angleCCW512 -= 5;	}
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
		s->x256 				= t256(GAME_WIDTH/2)-((s->w128));
		s->y256 				= -(((i+1)*(t256(24)/*s->h128+s->h128*/)));
		TATSUMAKI1_DATA *data;
		data					= mmalloc(sizeof(TATSUMAKI1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(10*2);
		/*data->base.*/s->base_health		= (4*8)+(difficulty<<2);/*やわらかすぎ*/	/*(2+(difficulty<<2))*/
		data->state 			= 0;
		data->speed256			= t256(2);/*速すぎ*/	/*3+difficulty+lv/3*/
		data->kaiten_houkou 	= (i&1);/*(0==i%2)?(0):(1)*/
//		data->enemy_rank 			= enemy_rank;
	}
}
#undef NUM_OF_ENEMIES
