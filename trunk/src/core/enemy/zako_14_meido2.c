
#include "bullet_object.h"

/*---------------------------------------------------------
	"メイド2",		"ZATAK",
	-------------------------------------------------------
	rwingx_zatak (赤い妖精)
	左右から現れ、自機狙い弾を打った後、真下へ直進してくる。
	-------------------------------------------------------
	ok
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
//	int angleCCW512;
	int speed256;
	int state;
//	int enemy_rank; 	/* 設定ファイルからの敵の強さ */
//
	int wait;
} MEIDO2_DATA;

static int data_hari_rank256;
#define NUM_OF_ENEMIES (12)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_meido2(SPRITE *src)
{
	item_create(src, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido2(SPRITE *src)
{
	MEIDO2_DATA *data = (MEIDO2_DATA *)src->data;
	data->wait += 1/*fps_fa ctor*/;
	switch (data->state)
	{
	case 0:
		if (data->wait >= 40)
		{
			data->state++/*=1*/;
		}
		break;
	case 1:
		data->speed256 -= t256(0.02/**fps_fa ctor*/);
		if (data->speed256 <= 0)
		{
			data->speed256 = 0;
			data->state++/*=2*/;
		}
		break;
	case 2:
//efine HARIDAN_SPEED ( /*speed256*/(t256(5)+((data_le vel/*data->le vel*/)<<8) ) )
#define HARIDAN_SPEED ( /*speed256*/t256(4/*5*/) + (data_hari_rank256/*data->le vel*/)	)

#if 0
/* CWの場合 */
		if (data->angle512 < deg_360_to_512(90))
		{
			data->angle512 += deg_360_to_512(3)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (data->angle512 >= deg_360_to_512(89))
			{
				data->state++/* = 3*/;
			}
		}
		else
		{
			data->angle512 -= deg_360_to_512(3)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (data->angle512 <= deg_360_to_512(91))
			{
				data->state++/* = 3*/;
			}
		}
#else
/* CCWの場合 */
		if (src->tmp_angleCCW512 < deg_360_to_512CCW(180/*360+90-90*/))
		{
			src->tmp_angleCCW512 -= deg_360_to_512CCW(3)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (src->tmp_angleCCW512 < deg_360_to_512CCW(0/*360+90-89*/))
			{
				data->state++/* = 3*/;
			}
		}
		else
		{
			src->tmp_angleCCW512 += deg_360_to_512CCW(3)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (src->tmp_angleCCW512 > deg_360_to_512CCW(360/*360+90-91*/))
			{
				data->state++/* = 3*/;
			}
		}
#endif
		break;
	case 3: /*弾撃つ*/
#if 0
/* CWの場合 */
				data->angle512 = deg_360_to_512(90);
				if (data_hari_rank256/*data->le vel*/)
				{
					bullet_create_offset_dan_type000(src, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
				}
#else
/* CCWの場合 */
				src->tmp_angleCCW512 = deg_360_to_512CCW(0);
				if (data_hari_rank256/*data->le vel*/)
				{
					bullet_create_offset_dan_type000(src, HARIDAN_SPEED, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
				}
#endif
				data->state++/* = 4*/;
		break;
	case 4: /*特攻*/
		data->speed256 += t256(0.2)/*0.3*/;/*加速*/
		if (data->speed256 >= t256(2.5)/*3*/ /*(6+data->le vel)*/)
		{
			data->speed256 = t256(2.5)/*3*/ /*(6+data->le vel)*/;/* 加速最大値 */
			data->state++/* = 5*/;
		}
		break;
	case 5:/*退場*/
#if 0
		if ()
		{
			if ()
			{
			}else
			{
			}
		}
#endif
	//	if (src->y256 < 0)
		if (src->y256 > t256(GAME_HEIGHT))
		{
			src->type = SP_DELETE;	/* おしまい */
		}
		break;
	}
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
	src->x256+=((sin512((src->tmp_angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((src->tmp_angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_40_CHUU_YOUSEI1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
}

/*
(※１):大昔のversionからズレまくってるが、fps_fa ctorが一定しないから、
fps_fa ctorを入れようがズレるのはKETM自体の仕様。
*/

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_meido2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank;
		enemy_rank	= l->user_y;
//
	data_hari_rank256 = (enemy_rank<<7);	//	data->le vel = enemy_rank;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_AKA_MEIDO08_PNG);	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_40_CHUU_YOUSEI1/*_13_MEIDO2*/;
//		s->type 				= SP_ZAKO/*_13_MEIDO2*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_meido2;
		s->callback_loser		= lose_meido2;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		MEIDO2_DATA *data;
		data					= mmalloc(sizeof(MEIDO2_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(25*2);
		/*data->base.*/s->base_health		= 1+(difficulty<<2);
		data->state 			= 0;
		data->wait				= 0;
		data->speed256			= ((ra_nd()&((256*4)-1)));/*2.0*(((dou ble)ra_nd()/RAND_MAX)*2)*/
#if 0
/* CWの場合 */
		if (i<6)
		{
			data->angle512		= deg_360_to_512(10);
			s->x256 			= -((s->w128+s->w128));
		}
		else
		{
			data->angle512		= deg_360_to_512(180-10/*170*/);
			s->x256 			= t256(GAME_WIDTH);
		}
#else
/* CCWの場合 */
		if (i<6)
		{
			s->tmp_angleCCW512	= deg_360_to_512CCW(90-10/*360-10*/);
			s->x256 			= -t256(32)/*-((s->w128+s->w128))*/;
		}
		else
		{
			s->tmp_angleCCW512	= deg_360_to_512CCW(180+90+10/*360-170*/);
			s->x256 			= t256(GAME_WIDTH);
		}
#endif
		s->y256 = t256(50);
	}
}
#undef NUM_OF_ENEMIES
