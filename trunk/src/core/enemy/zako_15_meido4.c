
#include "enemy.h"

/*---------------------------------------------------------
	"メイド4",		"CIR",	rwingx_cir
	-------------------------------------------------------
	右側から二列縦隊で現れ、下に向かい。下で旋回。
	中央から上昇し、右側に向かい。上で旋回。
	右側から下に向かい。画面外へ退場。
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	/*dou ble*/int angle512;
	/*dou ble*/int speed256;
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
	MEIDO4_DATA *d=(MEIDO4_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* 下に向かう / down */
		if (s->y256 > t256(128/*200*/)) 	/* 中心付近を越えて下がったら、少し速度を落とす */
		{
			d->state=1;
			d->speed256=t256(2.5/*3.0*/);/* 速度を落とす */
		}
		break;
	case 1: /* 下で旋回 / turn */
		d->angle512 -= 5/*2*/ /*deg_360_to_512(2)*/ /**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
	//	if (d->angle512  <	deg_360_to_512(0))
	//	{	d->angle512  += deg_360_to_512(360);	}
		mask512(d->angle512);
		if ((d->angle512 >= deg_360_to_512(270-3)) &&		/* ほぼ真上を向いたら */
			(d->angle512 <= deg_360_to_512(270+3)))
		{
			d->angle512 = deg_360_to_512(270);		/* 真上に強制補正 */
			d->state=2;
			d->speed256=t256(2.0/*8.0*/);/* 速度を落とす */
		}
		break;
	case 2: /* up */
		if (s->y256 < t256(64/*50*/))	/* 中心付近を越えて上がったら、少し速度を落とす */
		{
			d->state=3;
			d->speed256=t256(1.5/*3.0*/);
		}
		break;
	case 3: /* turn */
		d->angle512 += 5/*2*/ /*deg_360_to_512(2)*/ /**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
	//	if (d->angle512 < deg_360_to_512(0))
	//	{	d->angle512 += deg_360_to_512(360); 	}
		mask512(d->angle512);
		if ((d->angle512 >= deg_360_to_512(90-3)) &&		/* ほぼ真下を向いたら */
			(d->angle512 <= deg_360_to_512(90+3)))
		{
			d->angle512 = deg_360_to_512(90);		/* 真下に強制補正 */
			d->state=4;
			d->speed256=t256(1.0/*6.0*/);
		}
		break;
	case 4: /* down */
		d->speed256 += t256(0.1);/*加速*/
		if (s->y256 > t256(GAME_HEIGHT))
		{
			s->type=SP_DELETE;
		}
		break;
	}
	/*以下rwingx.cと同じ*/
	s->x256+=((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->anim_frame = ((((d->angle512)&(512-1)))>>6);/*"rw ingx8.png"*/
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
		data->b.score		= score(/*25*/15*2);
		data->b.health		= 5+(difficulty<<2);
		data->angle512		= deg_360_to_512(90);
		data->speed256		= t256(3.0/*6.0*/);
		data->state 		= 0;
//		data->level 		= lv;
		s->x256 			= ((i&(2-1)/*%2*/)==0)?t256(30):t256(50);
		s->y256 			= -(i*t256(20));
	}
}
#undef NUM_OF_ENEMIES
