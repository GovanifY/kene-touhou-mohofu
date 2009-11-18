
#include "bullet_object.h"

/*---------------------------------------------------------
		"メイド3",		"CURVER",	rwingx_curver
	-------------------------------------------------------
上から現れてS字をn回描いて横へ

←←
↓
→→→→→
		↓
←←←←←
↓
→→→→→
		↓
←←←←←
	-------------------------------------------------------
	バグあり。
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
//	int angleCCW512;
	int speed256;
	int state;
	int turn_count;
//
	int turnspeed512;
	int clip_left256;
	int clip_right256;
} MEIDO3_DATA;

static int static_last=0;	/* 登場位置の切り替え */
static int enemy_rank;		/* 設定ファイルからの敵の強さ */
static int destoroy[2];

//	NUM_OF_ENEMIES=10;		10機
//#define NUM_OF_ENEMIES (10)
//#define NUM_OF_ENEMIES (4+difficulty)
#define NUM_OF_ENEMIES (4+3)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_meido3(SPRITE *src)
{
	destoroy[static_last]++;
	if ( (NUM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
	{
		destoroy[static_last] = 0;
		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
}

/*---------------------------------------------------------
	敵攻撃
---------------------------------------------------------*/

static void shot_meido3(SPRITE *src)
{
	if (0 < /*data->*/enemy_rank)
	{
		bullet_create_n_way_dan_sa_type(src,
			/*speed256*/t256(2),
			ANGLE_JIKI_NERAI_DAN/*angle512*/,
			(int)(512/24),
			BULLET_UROKO14_01_AKA,
			/*n*/(difficulty<<1)
		);
	}	/* easy以外は狙い弾を撃つ */
}
			/*bullet_create_aka_maru_jikinerai(src, t256(3+(data->enemy_rank>>1)));*/
			/*bullet_create_aka_maru_jikinerai(src, t256(3+(data->enemy_rank>>1)));*/

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido3(SPRITE *src)
{
	MEIDO3_DATA *data = (MEIDO3_DATA *)src->data;
	switch (data->state)
	{
	case 0: 	/* 左周りで回転中 */
		/* CWの場合 */
	//	src->tmp_angleCCW512 -= data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		/* CCWの場合 */
		src->tmp_angleCCW512 += data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		mask512(src->tmp_angleCCW512);
		/* 右(斜め上)を向いたら */
		/* CWの場合 */
	//	if ( (512-16) < src->tmp_angleCCW512 )/*eps*/ /*<= deg_360_to_512(0)*/ 	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
		/* CCWの場合 */
		if ( 0==(0x100 & src->tmp_angleCCW512) )/* (deg_360_to_512CCW(180) > src->tmp_angleCCW512) */
		{
			if ( (128+16) < src->tmp_angleCCW512 )/*eps*/ /*<= deg_360_to_512(0)*/ 	//deg_360_to_512(10/*90*/ /*-90*/ /*360*/ /*-10*/
			{
			//	src->tmp_angleCCW512 = deg_360_to_512(0/*360*/ /*-10*/)/*deg_360_to_512(360)*/;
				data->state++/*=1*/;
				shot_meido3(src); 	/* easy以外は狙い弾を撃つ */
			}
		}
		break;
	case 1: 	/* 右(斜め上)へ移動中 */
		if (0 < data->turn_count)
		{
			if (src->x256 >= data->clip_right256) {	data->state++/*=2*/;	data->turn_count--; }
		}
		else	/* 退場中 */
		{
			if (src->x256 > t256(GAME_WIDTH) )
			{
			//	src->flags &= (~(SP_FLAG_VISIBLE));
				src->type = SP_DELETE;	/* おしまい */
			}
		}
		break;
	case 2: 	/* 右周りで回転中 */
	//	src->tmp_angleCCW512 += data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		/* CCWの場合 */
		src->tmp_angleCCW512 -= data->turnspeed512/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		mask512(src->tmp_angleCCW512);
		/* 左(斜め上)を向いたら */
	//	if ( (256+16) < src->tmp_angleCCW512 ) 	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
		/* CCWの場合 */
		if ( 0!=(0x100 & src->tmp_angleCCW512) )/* (deg_360_to_512CCW(180) < src->tmp_angleCCW512) */
		{
			if ( (256+128-16) > src->tmp_angleCCW512 ) 	//deg_360_to_512(180 /*270*/ /*90+*/ /*180-90-90*/ /*+10*/)
			{
			//	src->tmp_angleCCW512 = deg_360_to_512(180/*+10*/);
				data->state++/*=3*/;
				shot_meido3(src); 	/* easy以外は狙い弾を撃つ */
			}
		}
		break;
	case 3: 	/* 左(斜め上)へ移動中 */
		if (0 < data->turn_count)
		{
			if (src->x256 <= data->clip_left256) {	data->state=0;	data->turn_count--; 	}
		}
		else	/* 退場中 */
		{
			if (src->x256 < t256(0) )
			{
			//	src->flags &= (~(SP_FLAG_VISIBLE));
				src->type = SP_DELETE;	/* おしまい */
			}
		}
		break;
	}
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
	src->x256+=((sin512((src->tmp_angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((src->tmp_angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//	src->an im_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	src->an im_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 00
/* [CCWの場合(新)] CWの場合 */
	src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* 旧 */
//#else
/* CCWの場合 */
	/* 新(まだ作ってない) */
	//src->an im_frame = ((((src->tmp_angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*無理矢理旧互換*/
	{int aaa512;
		aaa512 = 128+ 512 - src->tmp_angleCCW512;
		mask512(aaa512);
		src->yx_an im_frame = (((aaa512))>>(6));
	}
#endif
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_40_CHUU_YOUSEI1+((((src->tmp_angleCCW512)&(512-1)))>>6);
	}
/*現在仕様上おかしい*/
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_meido3(STAGE_DATA *l)/*int lv*/
{
//	int enemy_rank;
	enemy_rank	= l->user_y;
//	/*data->*/enemy_rank 		= enemy_rank;
//
	static_last++;
	static_last &= 1;
//
	destoroy[static_last] = 0;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_40_CHUU_YOUSEI1/*_14_MEIDO3*/;
//		s->type 				= SP_ZAKO/*_14_MEIDO3*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_meido3;
		s->callback_loser		= lose_meido3;
		s->callback_hit_enemy	= callback_hit_zako;
		MEIDO3_DATA *data;
		data					= mmalloc(sizeof(MEIDO3_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(/*25*/50*2);
	//	/*data->base.*/s->base_health		= (12*8)+(difficulty<<3/*4*/)/*5+(difficulty<<2)*/;/*よわすぎ*/
	//	/*data->base.*/s->base_health		= (12*8)+(difficulty<<2/*4*/)/*5+(difficulty<<2)*/;/*他が強いので弱くする*/
		/*data->base.*/s->base_health		= (10*8)+(difficulty<<2/*4*/)/*5+(difficulty<<2)*/;/*他が強いので弱くする*/
		s->tmp_angleCCW512		= (256+128/*0*/)/*256+16*/ /*deg_360_to_512(270)*/;
		data->speed256			= t256(1.5) /*3+difficulty*/;/*はやすぎ*/
		data->turnspeed512		= (7) /*deg_360_to_512*/ /*(4+difficulty)*/;/*簡略化(360度→512度)*/
//		data->clip_left256		= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
//		data->clip_right256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
		data->clip_left256		= (t256(static_last<<7)+t256(static_last<<6)+t256(32)) /*50*/;			/* 32,	224(128+64+32)	*/
		data->clip_right256 	= (t256(static_last<<7)+t256(static_last<<6)+t256(32))+t256(128-32-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/
		data->state 			= (static_last<<1)/*0*/;
		data->turn_count		= 4;
//		s->x256 				= t256((static_last<<7))+t256((32))+(((s->w128)*(i+1))*t256(0.375))/* 0.375=0.75/2.0 -(s->w*(i+1))*0.5*/;/*くっつきすぎ*/
		s->x256 				= t256(10);/*くっつきすぎ*/
//		s->y256 				= t256(0)/*50*/;
		s->y256 				= t256(10)/*50*/;
	}
}
#undef NUM_OF_ENEMIES
