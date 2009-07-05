
#include "enemy.h"

/*---------------------------------------------------------
		"紫編隊1",		"MI NG",
	-------------------------------------------------------
	10機三角錐状に続がり、編隊飛行してくるザコ敵
	mi ng
	rw ingx_cu rverに似てる
	-------------------------------------------------------
	バグあり？？？
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	/*dou ble*/int angle512;
	/*dou ble*/int speed256;
	int state;
	int level;
//
	int type;
} YUKARI1_DATA;
static int destoroy;

#define NUM_OF_ENEMIES (10)

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

static void lose_yukari1(SPRITE *s)
{
	int put_item;	put_item=99;
//	case SP_ZAKO_02_YUKARI1:
	if (rand_percent(30))
	{
		if (rand_percent(50))	{	put_item=(SP_ITEM_00_P001&0xff);}
		else					{	put_item=(SP_ITEM_06_TENSU&0xff);}
	}
//
	if (99!=put_item)
	{
		item_create(s, (put_item|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	}
//
	destoroy++;
	if ( (NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
	{
		destoroy = 0;
		item_create(/*zzz*/ s, SP_ITEM_00_P001, 1, (ITEM_MOVE_FLAG_06_RAND_XY) );
	}
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_yukari1(SPRITE *s)
{
	YUKARI1_DATA *d=(YUKARI1_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if (s->y256 >= ((256*256)-(difficulty<<(6+8) ))  /*t256(200)*/ )
		{
			d->state=1;
			d->speed256 = t256(5);
		}
		break;
	case 1:
		if (d->type==1)
		{
			d->angle512 += deg_360_to_512(4)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 >= deg_360_to_512(180))
			{
				d->angle512 = deg_360_to_512(180);
				d->state=2;
				if (d->level>0)
				{	bullet_create_aka_maru_jikinerai(s, t256(d->level+2));}
			}
		}
		else
		if (d->type==2)
		{
			d->angle512 -= deg_360_to_512(4)/**fps_fa ctor*/;/*簡略化の仕様上少し位置がずれる(※１)*/
			if (d->angle512 <= deg_360_to_512(0))
			{
				d->angle512 = deg_360_to_512(0);
				d->state=2;
				if (d->level>0)
				{	bullet_create_aka_maru_jikinerai(s, t256(d->level+2) );}
			}
		}
		else
		{
			d->state=2;
			if (d->level>0)
			{	bullet_create_aka_maru_jikinerai(s, t256(d->level+2) );}
		}
		break;
	case 2:
		if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
			(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
		{	s->flags	&= (~(SP_FLAG_VISIBLE));}
		break;
	}
	/*似てるがちょっと違う--以下rwingx.cと同じ*/
	s->x256+=((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
//	s->anim_frame = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1))*(32/2))>>8);
	s->anim_frame = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1)))>>4);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_yukari1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	destoroy = 0;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_YUKARI32_PNG);	//s->anim_speed=0;/*36"mi ng.png"*/
		s->type 			= SP_ZAKO/*_02_YUKARI1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_yukari1;
		s->callback_loser	= lose_yukari1;
		s->anim_frame		= 0;
		YUKARI1_DATA *data;
		data				= mmalloc(sizeof(YUKARI1_DATA));
		s->data 			= data;
		data->b.score		= score(25*2);
		data->b.health		= 1+(difficulty<<2);
		data->angle512		= deg_360_to_512(90);
		data->speed256		= t256(2)/*2+difficulty+lv/2*/;/*速すぎ*/
		data->state 		= 0;
		data->level 		= lv;
		//ウィンドウ幅の変更
		switch (i)
		{
		case 0: s->x256 = t256(GAME_WIDTH/2);								s->y256 = -t256(30);						data->type=0;	break;
		case 1: s->x256 = t256(GAME_WIDTH/2)-(s->w128); 					s->y256 = -t256(30)-(  (s->h128+s->h128));	data->type=1;	break;
		case 2: s->x256 = t256(GAME_WIDTH/2)+(s->w128); 					s->y256 = -t256(30)-(  (s->h128+s->h128));	data->type=2;	break;
		case 3: s->x256 = t256(GAME_WIDTH/2)-(s->w128+s->w128); 			s->y256 = -t256(30)-(  (s->h128)<<2);		data->type=1;	break;
		case 4: s->x256 = t256(GAME_WIDTH/2);								s->y256 = -t256(30)-(  (s->h128)<<2);		data->type=0;	break;
		case 5: s->x256 = t256(GAME_WIDTH/2)+(s->w128+s->w128); 			s->y256 = -t256(30)-(  (s->h128)<<2);		data->type=2;	break;
		case 6: s->x256 = t256(GAME_WIDTH/2)-(s->w128+s->w128)-(s->w128);	s->y256 = -t256(30)-(3*(s->h128+s->h128));	data->type=1;	break;
		case 7: s->x256 = t256(GAME_WIDTH/2)-(s->w128); 					s->y256 = -t256(30)-(3*(s->h128+s->h128));	data->type=1;	break;
		case 8: s->x256 = t256(GAME_WIDTH/2)+(s->w128); 					s->y256 = -t256(30)-(3*(s->h128+s->h128));	data->type=2;	break;
		case 9: s->x256 = t256(GAME_WIDTH/2)+(s->w128+s->w128)+(s->w128);	s->y256 = -t256(30)-(3*(s->h128+s->h128));	data->type=2;	break;
		}
	}
}
#undef NUM_OF_ENEMIES
