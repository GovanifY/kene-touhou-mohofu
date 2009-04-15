#include "enemy.h"

/*ming_greeter*/
/*rwingx_curver(ming)�Ɏ��Ă�*/

typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	int state;
	int level;
//
	double max_y;
} GREETER_DATA;

/*
�T�@���ɑ�����A�ґ���s���Ă���U�R�G



*/
#define NUM_OF_ENEMIES (5)
/*enemy_curver_controller()�Ƃقړ���(���ʉ��\�����H)*/
static void enemy_greeter_controller(CONTROLLER *c)
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
	{	static const Uint16/*int*/ item_table[8]=
		{
			SP_BONUS_07_ADDSPEED,
			SP_BONUS_02_BOMB,
			SP_BONUS_02_BOMB/*�ᑬ�{���ɋz��==SP_BONUS_EXTRA_HOMING*/,
			SP_BONUS_02_BOMB/*�ᑬ�{���ɋz��==SP_BONUS_EXTRA_HLASER*/,
			//
			SP_BONUS_01_FIREPOWER_G/*�E�F�|���A�C�e��(��)==SP_BONUS_FIREPOWER_G �E�F�|���A�C�e��(��)==SP_BONUS_EXTRA_SHIELD*/,
			SP_BONUS_00_FIREPOWER,
			SP_BONUS_01_FIREPOWER_G,/*�E�F�|���A�C�e��(��)==SP_BONUS_FIREPOWER_G*/
			SP_BONUS_06_COIN,
		};
		static unsigned int drop_item=0;
		bonus_multi_add(/*zzz*/ id_array[NUM_OF_ENEMIES]/*,id_array[NUM_OF_ENEMIES+1]*/,item_table[drop_item], 1, BONUS_FLAG_RAND_XY);
		drop_item++;
		drop_item &= (8-1);//if (drop_item==8)	{	drop_item=0;}
	}
	controller_remove(c);
}

static void enemy_greeter_move(SPRITE *s)
{
	GREETER_DATA *d=(GREETER_DATA *)s->data;
	switch (d->state)
	{
	case 0: /* nach unten */
		if (s->y >= d->max_y)
		{
			d->state=1;
			d->angle512 = atan_512(0-s->y,GAME_WIDTH/2-s->x);				//�E�B���h�E���̕ύX
			if (2 < d->speed)
			{	/*����*/
				d->speed-=0.2;
			//	d->speed=2/*3+difficulty*/;								//[***090201		�ύX
			}
		}
		if (d->level)
		{
			if (rand()%(105-(d->level-2+difficulty)*10)==0) 	//[***090126		�኱�ύX
			{
				enemy_bullet_create(s,1+difficulty+d->level/3);
			}
		}
		break;
	case 1:
		if (s->y<-s->h)
		{
			s->flags	&= ~SP_FLAG_VISIBLE;
		}
		break;
	}
	/*���Ă邪������ƈႤ--�ȉ�rwingx.c�Ɠ���*/
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
//	s->aktframe=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
	s->aktframe = ((((d->angle512/*+deg_360_to_512(270)*/)&(512-1))*(36/2))>>8);
}
#if 0
	case 0:		/* �E�ֈړ��� */
		if (s->x >= d->clip_right)
		{	d->state=1;}
		break;
	case 1:		/* �E����ŉ�]�� */
		d->angle512 -= d->turnspeed512/**fps_factor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		if (d->angle512 <= deg_360_to_512(180))
		{
			d->angle512 = deg_360_to_512(180);
			d->state=2;
			if (d->level>0)
			{	enemy_bullet_create(s,3+(d->level>>1));}
		}
		break;
	case 2:		/* ���ֈړ��� */
		if (s->x <= d->clip_left)
		{	d->state=3;}
		break;
	case 3:		/* ������ŉ�]�� */
		d->angle512 += d->turnspeed512/**fps_factor*/;/*�ȗ����̎d�l�㏭���ʒu�������(���P)*/
		if (d->angle512 >= deg_360_to_512(360) )
		{
			d->angle512 = deg_360_to_512(0);
			d->state=0/*4*/;
			if (d->level>0)
			{	enemy_bullet_create(s,3+(d->level>>1));}
		}
		break;

	case 8:		/* �E�ֈړ��� */
		if (s->x > GAME_WIDTH)		//�E�B���h�E���̕ύX
		{	s->flags&= ~SP_FLAG_VISIBLE;}
		break;
#endif
void enemy_greeter_add(int lv)
{
//	NUM_OF_ENEMIES=5;
	static int static_last=0;/* �o��ʒu�̐؂�ւ� */
	static_last++;
	static_last &= 1;
//
	CONTROLLER *c;
	c				= controller_add();
	/*int*/void *id_array;
	id_array		= mmalloc(sizeof(int)*(NUM_OF_ENEMIES+2));
	c->e			= id_array;
	c->con			= enemy_greeter_controller;
//
	/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
		s					= sprite_add_file("ming.png",36,PR_ENEMY);		s->anim_speed=0;
		id_array_bbb[i] 	= s/*->id*/;
		s->type 			= SP_EN_GREETER;
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover			= enemy_greeter_move;
		if (0==static_last)	{	s->x=0;					}	//�E�ォ��o��
		else				{	s->x=GAME_WIDTH-s->w;	}	//���ォ��o��
		s->y				= -i*s->h;
		GREETER_DATA *data;
		data				= mmalloc(sizeof(GREETER_DATA));
		s->data 			= data;
		data->angle512		= atan_512((GAME_HEIGHT-s->h-60)-s->y,GAME_WIDTH/2-s->x);	//�E�B���h�E���̕ύX
		data->max_y 		= (GAME_HEIGHT-s->h-60);
		data->speed 		= (3+difficulty) /*4*/;/*�n�߂����͑���*/
		data->state 		= 0;
		data->b.score		= score(5*2);
		data->b.health		= 1+(difficulty<<2);
		data->level 		= lv;
	}
}
#undef NUM_OF_ENEMIES
