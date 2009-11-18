
#include "bullet_object.h"

/*---------------------------------------------------------
		"虹毛玉1",		"EYEFO",
	-------------------------------------------------------
	バグあり？？？
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int angle512;
	int radius256;
	int level;
	int flag1;
	int xcenter256;
	int ycenter256;
	int time_out;
} NIJI_KEDAMA1_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (16)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_niji_kedama1(SPRITE *src)
{
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ src,
			(SP_ITEM_01_P008+(ra_nd()&1))/*50%(SP_ITEM_01_P008 or SP_ITEM_02_BOMB)*/
			//66%==SP_ITEM_02_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),
			, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_niji_kedama1(SPRITE *src)
{
	NIJI_KEDAMA1_DATA *data = (NIJI_KEDAMA1_DATA *)src->data;
	if (0 == data->flag1)
	{
		data->radius256 += t256(1)/*fps_fa ctor*/;
		if (data->radius256 > t256(64)/*150*/) /* 最大半径 */
		{
			data->flag1=1;
		}
	}
	else
	{
		data->radius256 -= t256(2)/*1*/ /*fps_fa ctor*/;
		if (data->radius256 < t256(8) /*<=10*/)	/* 最小半径 */
		{
			data->flag1=0;
		}
	}
	data->angle512 += /*deg_360_to_512(5)*/(1)/**fps_fa ctor*/;/* deg_360_to_512(5) 速すぎる */
	mask512(data->angle512);// if (data->angle>=360)	{	data->angle-=360;}
/* CCWの場合 */
	src->x256=((sin512((data->angle512))*(data->radius256))>>8)+data->xcenter256;
	src->y256=((cos512((data->angle512))*(data->radius256))>>8)+data->ycenter256;

	if (0 != difficulty)/* easy はホーミング撃たない */
	{
		if (data->level)
		{
		//	if (0==(ra_nd()%(1000-(data->level)*100)))
		//	if (0==(ra_nd()%((10-(data->level))*100)))
	//		if (0==(ra_nd()&((1<<(10-(data->level)))-1)))/*テキトー*/	/* [10]確率(撃つ頻度)高すぎる気がする */
//			if (0==(ra_nd()&((1<<(12-(data->level)))-1)))/*テキトー*/
			if (0==(ra_nd()&((1<<(11-(data->level)))-1)))/*テキトー*/
			{
				/* 画面外ならば撃たない */
				if (0 < src->y256)	/* 画面内なら撃つ */
				{
					bullet_create_enemy_homing(src);	/* 敵側の誘導ミサイルを作成する */
				}
			}
		}
	}
	data->time_out += 1/*fps_fa ctor*/;
	if (data->time_out < 300)
	{
	//	if (data->ycenter256 < t256(GAME_HEIGHT))
	//	if (data->ycenter256 < t256(difficulty<<6) )/* 難易度により下がってくる位置が違う。最大192(3*64) */
		if (data->ycenter256 < t256(difficulty<<5) )/* 難易度により下がってくる位置が違う。最大96(3*32) */
		{
			data->ycenter256 += t256(1)/*fps_fa ctor*/;
		}
	}
	else if (data->time_out > 1200)
	{
		src->type = SP_DELETE;	/* おしまい */
	}
	else if (data->time_out > 600)
	{
		data->ycenter256 -= t256(1)/*fps_fa ctor*/;
	//	if (data->ycenter256+src->y256 < -((src->w)<<8))
	//	{
	//	//	src->type = SP_DELETE;	/* おしまい */
	//	//	src->flags &= (~(SP_FLAG_VISIBLE));
	//		/* 明らかに少しはみ出て、また戻ってくる軌跡なのに、 */
	//		/*	画面外に出たのを見えなくする(一度出たら見えないまま)なのは */
	//		/*	この場合は意味無い */
	//	}
	}
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_niji_kedama1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	destoroy = 0;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s						= sprite_add_res(BASE_NIJI_KEDAMA16_PNG);	//s->anim_speed=1/*2*/;/*15"ey efo.png"*/
	//	s->anim_speed			= 1;	/*???*/
		s->anim_speed			= 8;	/*???*/
		s->type 				= SP_ZAKO/*_05_NIJI_KEDAMA1*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_niji_kedama1;
		s->callback_loser		= lose_niji_kedama1;
		s->callback_hit_enemy	= callback_hit_zako;
		s->anim_frame			= i/*%s->frames*/;
		NIJI_KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(NIJI_KEDAMA1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(30*2);
		/*data->base.*/s->base_health		= /*50*/ /*48*/32+(difficulty/*<<2*/);/*10+(difficulty<<2)*/	/* そこそこ倒せる強さにしとく(誘導ミサイルが強いので) */
		data->radius256 		= t256(10);
		data->flag1 			= 0;
		data->angle512			= (/*360*/512/NUM_OF_ENEMIES)*i;
		data->time_out			= 0;
		data->xcenter256		= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		data->ycenter256		= t256(-32)/*-100*/;
		data->level 			= lv;
	}
}
#undef NUM_OF_ENEMIES
