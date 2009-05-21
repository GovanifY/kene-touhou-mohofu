
#include "enemy.h"

/*---------------------------------------------------------
	"���C�h1",		"RWINGX",	rwingx
	-------------------------------------------------------
	�ォ�牡�ɓ��Ԋu�Ő��񂵂Č���A�v���C���[�Ɍ�����
	������x�߂Â��ƁA�e�u���Ĕ��]���ē�����B
---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int angle512;
	int speed256;
	int state;
	int level;
//
	int wait;
} MEIDO1_DATA;


#define NUM_OF_ENEMIES (8)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void lose_meido1(SPRITE *s)
{
	item_create(s, (enemy_get_random_item()), 1, ITEM_MOVE_FLAG_06_RAND_XY);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_meido1(SPRITE *s)
{
	MEIDO1_DATA *d=(MEIDO1_DATA *)s->data;
	switch (d->state)
	{
//	case 0: /* sleep(d->wait) ticks */
//	//	if (d->wait > 0)
//	//	{
//	//		d->wait -= 1/*fps_fa ctor*/;
//	//	}
//	//	else
//		{
//			/* �������o�� / langsam nach unten */
//			d->state		= 1;
//			d->angle512 	= deg_360_to_512(90);/*������*/
//		//	d->speed256 	= t256(1.0);
//			d->wait 		= 100;
//		}
//		break;
	case 0/*1*/: /* move (d->wait) ticks */
		if (d->wait > 0)
		{
			d->wait -= 1/*fps_fa ctor*/;
			if ( d->speed256 < t256(1.0))
			{
				d->speed256 += 5/*t256(0.1)*/;/*����*/
			}
		}
		else
		{
			d->state		= 1/*2*/;
			d->wait 		= 60;
		//	d->speed256 	= t256(0);
		}
		break;
	case 1/*2*/: /* sleep(d->wait) ticks */
		if (d->wait > 0)
		{
			d->wait -= 1/*fps_fa ctor*/;
			if (0 < d->speed256)
			{
				d->speed256 -= 5/*t256(0.1)*/;/*����*/
			}
		}
		else
		{
			/* �v���C���[�ɓ˂�����ł��� / schnell richtung player */
			d->state		= 2/*3*/;
			d->angle512 	= atan_512(player->y256-s->y256,player->x256-s->x256);
		//	d->speed256 	= /*t256(3+(d->level>>1))*/ /*(4+d->level)*/;/*���߂�*/
		//	d->wait 		= 50;/*???*/
		}
		break;
	case 2/*3*/: /* move (d->wait) ticks */
		d->speed256 	+= t256(0.1);/*�������Ȃ��� �v���C���[�ɓ˂�����ł���*/
		if (s->y256 >= t256(GAME_HEIGHT-150))
		{
			if (d->level)
			{	bullet_create_aka_maru_jikinerai(s, t256(1.0)+t256(d->level)/*t256(3)+t256(d->level)*/ );}/*���߂�*/
			d->state	= 3/*4*/;
			d->angle512 += deg_360_to_512(180);/* ���]���ē����� */
			mask512(d->angle512);// d->angle %= deg_360_to_512(360);
		//	d->speed256 = /*t256(6+d->level)*/;/*���߂�*/
		}
		break;
	case 3/*4*/:
		if ((s->x256<-((s->w)<<7)) || (s->x256 > t256(GAME_WIDTH)) ||
			(s->y256<-((s->h)<<7)) || (s->y256 > t256(GAME_HEIGHT)))
		{	s->type=SP_DELETE;}
		break;
	}
	/*�ȉ�rwingx.c�Ɠ���*/
	s->x256+=((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	s->anim_frame = ((((d->angle512)&(512-1)))>>6);/*"rw ingx8.png"*/
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_meido1(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_res(BASE_AKA_MEIDO08_PNG); 	//s->anim_speed	= 0;/*37"rw ingx8.png""rw ingx.png"*/
		s->type 			= SP_ZAKO/*_12_MEIDO1*/;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover	= move_meido1;
		s->callback_loser	= lose_meido1;
		MEIDO1_DATA *data;
		data				= mmalloc(sizeof(MEIDO1_DATA));
		s->data 			= data;
		data->b.score		= score(25*2)*(1+lv);
		data->b.health		= 1+lv+(difficulty<<2);
		data->angle512		= deg_360_to_512(90);//deg_360_to_512(270);/*������H�H*/
		data->speed256		= t256(0);//t256(0.5);
		data->state 		= 0;
		data->wait			= 100;//20;
		data->level 		= lv;
		s->x256 			= t256(GAME_WIDTH/8)*i+t256(10/*-20*/);		//�E�B���h�E���̕ύX
		s->y256 			= t256(-32)/*-50*/;
	}
}
#undef NUM_OF_ENEMIES
