#include "enemy.h"

/*rwingx_curver*/

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	int state;
	int turn_count;
	int level;
//
	/*double*/int turnspeed512;
	double clip_left;
	double clip_right;
} CURVER_DATA;

/*
10機
横から現れてS字を２回描いて横へ
→→→→→
↑		↓
←←←←←
↓		↑
→→→→→


*/


//	NUM_OF_ENEMIES=10;
//#define NUM_OF_ENEMIES (10)
#define NUM_OF_ENEMIES (4+difficulty)

static void enemy_curver_controller(CONTROLLER *c)
{
	int i;
//	int *id_array=c->e;
	/*int*/ SPRITE **id_array= (SPRITE **)c->e;
	SPRITE *s;
	int invisible=0;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		s=/*sprite_get_by_id*/(id_array[i]);
		if (s!=NULL)
		{
			if (!s->flags&SP_FLAG_VISIBLE)
			{
				invisible++;
			}
		}
	}
	if (invisible==NUM_OF_ENEMIES)
	{
		for (i=0; i<NUM_OF_ENEMIES; i++)
		{
			s=/*sprite_get_by_id*/(id_array[i]);
			s->type=SP_DELETE;
		}
		controller_remove(c);
		return;
	}
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		s=/*sprite_get_by_id*/(id_array[i]);
		if (s!=NULL)
		{
			id_array[NUM_OF_ENEMIES]=s/*->x*/;
		//	id_array[NUM_OF_ENEMIES+1]=s->y;
			return;
		}
	}
	bonus_multi_add(/*zzz*/ id_array[NUM_OF_ENEMIES]/*,id_array[NUM_OF_ENEMIES+1]*/,SP_BONUS_01_FIREPOWER_G, 1, BONUS_FLAG_RAND_XY);
	controller_remove(c);
}

static void enemy_curver_move(SPRITE *s)
{
	int test_angle512;
	/* 今まで、ずっと右回りの単位系(KETMも右回りの単位系、右が0度)でやってきたけど、
		東方本家って、左回りの単位系(下が0度)じゃね？と今更ながら気が付いた。 */
	CURVER_DATA *d=(CURVER_DATA *)s->data;
	switch (d->state)
	{
	case 0: 	/* 左周りで回転中 */
		d->angle512 -= d->turnspeed512/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		test_angle512 = d->angle512 +128;/* 判定用角度：９０度回転させて下が０とする */ 	/*擬似単位系変換*/
		mask512(test_angle512);
		if (test_angle512 < deg_360_to_512(90/*-90*/ /*360*/ /*-10*/)/*<= deg_360_to_512(0)*/)/*eps*/
		{
		//	d->angle512 = deg_360_to_512(0/*360*/ /*-10*/)/*deg_360_to_512(360)*/;
			d->state=1;
			if (0 < d->level)	{	enemy_n_way_bullet_type(s, /*bu_type*/BU_TYPE01_KUNAI_PNG, /*n*/(difficulty<<1), /*speed*/2, ANGLE_JIKINERAI_DAN/*angle512*/);	}/* easy以外は狙い弾を打つ */
	/*enemy_bullet_create(s,3+(d->level>>1));*/
		}
		break;
	case 1: 	/* 右(斜め上)へ移動中 */
		if (0 < d->turn_count)
		{
			if (s->x >= d->clip_right)	{	d->state=2;		d->turn_count--;	}
		}
		else	/* 退場中 */
		{
			if (s->x > GAME_WIDTH)		//ウィンドウ幅の変更
			{	s->flags&= ~SP_FLAG_VISIBLE;}
		}
		break;
	case 2: 	/* 右周りで回転中 */
		d->angle512 += d->turnspeed512/**fps_factor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
		test_angle512 = d->angle512 +128;/* 判定用角度：９０度回転させて下が０とする */ 	/*擬似単位系変換*/
		mask512(test_angle512);
		if (test_angle512 > deg_360_to_512(270/*90+*/ /*180-90-90*/ /*+10*/))
		{
		//	d->angle512 = deg_360_to_512(180/*+10*/);
			d->state=3;
			if (0 < d->level)	{	enemy_n_way_bullet_type(s, /*bu_type*/BU_TYPE01_KUNAI_PNG, /*n*/(difficulty<<1), /*speed*/2, ANGLE_JIKINERAI_DAN/*angle512*/);		}/* easy以外は狙い弾を打つ */
	/*enemy_bullet_create(s,3+(d->level>>1));*/
		}
		break;
	case 3: 	/* 左(斜め上)へ移動中 */
		if (0 < d->turn_count)
		{
			if (s->x <= d->clip_left)	{	d->state=0;		d->turn_count--;		}
		}
		else	/* 退場中 */
		{
			if (s->x < 0)		//ウィンドウ幅の変更
			{	s->flags&= ~SP_FLAG_VISIBLE;}
		}
		break;
	}
	/*以下rwingx.cと同じ*/
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
//	s->aktframe=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->aktframe = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->aktframe = ((((d->angle512)&(512-1)))>>6);/*"rwingx8.png"*/
}
/*
(※１):大昔のversionからズレまくってるが、fps_factorが一定しないから、
fps_factorを入れようがズレるのはKETM自体の仕様。
*/

void enemy_curver_add(int lv)
{
	static int static_last=0;/* 登場位置の切り替え */
	static_last++;
	static_last &= 1;
//
	CONTROLLER *c;
	c			= controller_add();
	/*int*/void *id_array;
	id_array	= mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e		= id_array;
	c->con		= enemy_curver_controller;
//
	/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_file(/*"rwing x.png"*/"rwingx8.png",8/*37*/,PR_ENEMY); s->anim_speed=0;
		id_array_bbb[i] 	= s/*->id*/;
		s->type 			= SP_EN_CURVER;
		s->flags			|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover			= enemy_curver_move;
		CURVER_DATA *data;
		data				= mmalloc(sizeof(CURVER_DATA));
		s->data 			= data;
		data->b.score		= score(/*25*/50*2);
		data->b.health		= (12*8)+(difficulty<<3/*4*/)/*5+(difficulty<<2)*/;/*よわすぎ*/
		data->angle512		= deg_360_to_512(0);
		data->speed 		= 2/*3+difficulty*/;/*はやすぎ*/
		data->turnspeed512	= /*deg_360_to_512*/(4+difficulty);/*簡略化(360度→512度)*/
		data->clip_left 	= ((static_last<<7)+(static_last<<6)+(32)) /*50*/;		/* 32,  224(128+64+32)  */
		data->clip_right	= ((static_last<<7)+(static_last<<6)+(32))+(128-32);	/* 128, 320(256+64)   */	 //  /*GAME_WIDTH-50-s->w*/ ウィンドウ幅の変更
		data->state 		= (static_last<<1)/*0*/;
		data->turn_count	= 4;
		data->level 		= lv;
		s->x				= (static_last<<7)+(32)+((s->w*(i+1))*0.75)/*-(s->w*(i+1))*0.5*/;/*くっつきすぎ*/
		s->y				= 0/*50*/;
	}
}
#undef NUM_OF_ENEMIES
