
#include "bullet_object.h"

/*---------------------------------------------------------
	"メイド4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	右側から二列縦隊で現れ、下に向かい。下で旋回。
	中央から上昇し、右側に向かい。上で旋回。
	右側から下に向かい。画面外へ退場。
	-------------------------------------------------------
	ok
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int angleCCW512;
	int speed256;
	int state;
//	int level;
} MEIDO4_DATA;

#define NUM_OF_ENEMIES (24)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_meido4(SPRITE *s)
{
	item_create(s, SP_ITEM_06_TENSU, 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_meido4(SPRITE *s)
{
	MEIDO4_DATA *data = (MEIDO4_DATA *)s->data;
	switch (data->state)
	{
	case 0: /* 下に向かう / down */
		if (s->y256 > t256(128/*200*/)) 	/* 中心付近を越えて下がったら、少し速度を落とす */
		{
			data->state++/*=1*/;
			data->speed256=t256(2.5/*3.0*/);/* 速度を落とす */
		}
		break;
	case 1: /* 下で旋回 / turn */
#if 0
/* CWの場合 */
		data->angle512 -= ((5))/*2*/ /*deg_360_to_512(2)*/ /**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
	//	if (data->angle512  <	deg_360_to_512(0))
	//	{	data->angle512  += deg_360_to_512(360);	}
		mask512(data->angle512);
		if ((data->angle512 >= deg_360_to_512((270-3))) &&		/* ほぼ真上を向いたら */
			(data->angle512 <= deg_360_to_512((270+3))))
		{
			data->angle512 = deg_360_to_512((270));		/* 真上に強制補正 */
			data->state++/*=2*/;
			data->speed256=t256(2.0/*8.0*/);/* 速度を落とす */
		}
#else
/* CCWの場合 */
		data->angleCCW512 += ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ /**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
	//	if (data->angleCCW512  <	deg_360_to_512CCW(360-0))
	//	{	data->angleCCW512  += deg_360_to_512CCW(360-360);	}
		mask512(data->angleCCW512);
		if ( data->angleCCW512 > deg_360_to_512CCW((180)) 		)		/* ほぼ真上を向いたら */

		{
			data->angleCCW512 = deg_360_to_512CCW((180));		/* 真上に強制補正 */
			data->state++/*=2*/;
			data->speed256=t256(2.0/*8.0*/);/* 速度を落とす */
		}
#endif
		break;
	case 2: /* up */
		if (s->y256 < t256(64/*50*/))	/* 中心付近を越えて上がったら、少し速度を落とす */
		{
			data->state++/*=3*/;
			data->speed256=t256(1.5/*3.0*/);
		}
		break;
	case 3: /* turn */
#if 0
/* CWの場合 */
		data->angle512 += ((5))/*2*/ /*deg_360_to_512(2)*/ /**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
	//	if (data->angle512 < deg_360_to_512(0))
	//	{	data->angle512 += deg_360_to_512(360); 	}
		mask512(data->angle512);
		if ((data->angle512 >= deg_360_to_512((90-3))) &&		/* ほぼ真下を向いたら */
			(data->angle512 <= deg_360_to_512((90+3))))
		{
			data->angle512 = deg_360_to_512((90));		/* 真下に強制補正 */
			data->state++/*=4*/;
			data->speed256=t256(1.0/*6.0*/);
		}
#else
/* CCWの場合 */
		data->angleCCW512 -= ((5))/*2*/ /*deg_360_to_512CCW(360-2)*/ /**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
	//	if (data->angleCCW512 < deg_360_to_512CCW(360-0))
	//	{	data->angleCCW512 += deg_360_to_512CCW(360-360); 	}
		mask512(data->angleCCW512);
		if (data->angleCCW512 > deg_360_to_512CCW(270) ) 	/* ほぼ真下を向いたら */
		{
			data->angleCCW512 = deg_360_to_512CCW((0));		/* 真下に強制補正 */
			data->state++/*=4*/;
			data->speed256=t256(1.0/*6.0*/);
		}
#endif
		break;
	case 4: /* down */
		data->speed256 += t256(0.1);/*加速*/
		if (s->y256 > t256(GAME_HEIGHT))
		{
			s->type = SP_DELETE;
		}
		break;
	}
	/*以下rwingx.cと同じ*/
/* CCWの場合 */
	s->x256+=((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
#if 1
/* [CCWの場合(新)] CWの場合 */
	s->anim_frame = ((((data->angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/* 旧 */
#else
/* CCWの場合 */
	/* 新(まだ作ってない) */
	//s->anim_frame = ((((data->angleCCW512)&(512-1)))>>6);/*"rw ingx8.png"*/
	/*無理矢理旧互換*/
	{int aaa512;
		aaa512 = 128+ 512 - data->angleCCW512;
		mask512(aaa512);
		s->yx_anim_frame = (((aaa512))>>(6));
	}
#endif
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_meido4(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0;i<NUM_OF_ENEMIES;i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed=0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_15_MEIDO4*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido4;
		s->callback_loser	= lose_meido4;
//		s->anim_frame 		= 0;
		MEIDO4_DATA *data;
		data				= mmalloc(sizeof(MEIDO4_DATA));
		s->data 			= data;
		data->base.score	= score(/*25*/15*2);
		data->base.health	= 5+(difficulty<<2);
#if 0
/* CWの場合 */
		data->angle512		= deg_360_to_512(90);		/* 真下 */
#else
/* CCWの場合 */
		data->angleCCW512	= deg_360_to_512CCW(0); 	/* 真下 */
#endif
		data->speed256		= t256(3.0/*6.0*/);
		data->state 		= 0;
//		data->level 		= lv;
		s->x256 			= (0==(i&(2-1)/*%2*/))?t256(30):t256(50);
		s->y256 			= -(i*t256(20));
	}
}
#undef NUM_OF_ENEMIES
