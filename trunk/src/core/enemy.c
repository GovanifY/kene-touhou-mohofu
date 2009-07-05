
/*---------------------------------------------------------
	�e���쐬 / �����쐬
	-------------------------------------------------------
	��ŕω�����e����邩���B
	�{�X��|�������ɏ�����悤�ɂ��Ă���B
	�ǉ������e
	// [***090114	��ʂ̓�����ς��Ă݂�
---------------------------------------------------------*/

#include "support.h"
#include "enemy.h"

/*---------------------------------------------------------
	�x�N�g���e(�p�x�ω����Ȃ����̂͂��̕����̕�������)
---------------------------------------------------------*/

/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
typedef struct
{
	int vx256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
	int vy256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
} BULLET_VECTOR_DATA;

/* ���p�e(���x�ω�����) */
typedef struct
{
	int vx256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
	int vy256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
	int speed256;								/* ���x */
//
	int timer;
	int delta256;
//
	int next_angle512;/*ST2*/
	int state;
} TOMARI_DAN_DATA;


/*---------------------------------------------------------
	����\4096�e
---------------------------------------------------------*/

typedef struct
{
	int vx256;	/*union BULLET_VECTOR_DATA*/	/*���p(����\��������)*/
	int vy256;	/*union BULLET_VECTOR_DATA*/	/*���p(����\��������)*/
	int speed256;					/* ���x */
//
	int time_count;
	int angle4096;	/* 1/4096�ȏ�̕���\�̐����e(1/512�ł͑���Ȃ�) = angle512_4096 */
	int d_angle4096; /* 1/4096�ȏ�̕���\�̐����e(1/512�ł͑���Ȃ�) */
} BULLET_4096_DATA;


/*---------------------------------------------------------
	�p�x�e(�p�x�ω��������)
---------------------------------------------------------*/

/* ��{�e(���x�A�p�x) */

//typedef struct
//{
//	int angle512;	/*union BULLET_DATA*/
//	int speed256;	/*union BULLET_DATA*/

//} LASER_DATA;/*BULLET_DATA*/

/* ���p�e */



typedef struct
{
	ENEMY_BASE b;
	int angle512;
	int speed256;
	int time_out;// dou ble range;	dou ble delay;
} ENEMY_HOMING_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/
//
	int state_lo;
	int state_hi;
} G_BULLET_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/
//
	int gra256;
	int sum256;
} GR_BULLET_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/
//
	int gra256;
	int sum256;
	int bounds;
} HAZUMI_DAN_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/
//
	int time_out;// dou ble timer;
} MOMIJI_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/
//
	int angle222_512;
	int adjust_speed256;
	int state;
} SMALL_RE_DATA;


typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/
//
	SPRITE *hyouji;
	int time_out;/*OLD_ BIG BULLET_DATA*/				// int wait_bg;
	int ransu512;/*OLD_ BIG BULLET_DATA*/
	int add_speed256;/*NEW_ BIG BULLET_DATA*/
} OODAMA_OYA_DATA;

typedef struct
{
	int time_out;
	int check_x256;
	int check_y256;
//
//	int *sx256p;
//	int *sy256p;
//	int *type;
//	int sw;
//	int sh;
} OODAMA_KO_DATA;




typedef struct
{
//	void/*BOSS05_DATA*/ *sd;
	int *sd_angle512;
	int angle512;
	int angle2_512;
	int speed256;
//
	int gra256;
	int sum256;
	int timer;
	int r_or_l; 	//	/*+1==R*/	/*-1==L*/
} BULLET_SP1_DATA;

typedef struct
{
//	void/*BOSS05_MAHO_DATA *sd;
	int  *sd_nnn;
//
	int state;
	int angle512;
	int gra256;
	int sum256;
} BULLET_SP2_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA(?)*/
	int speed256;	/*union BULLET_DATA*/

	int sum256;
	int gra256;
} FALL_KNIFE_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/

	int time_out;
	int speed2_256;
	int height;
	int target;
} FOLLOW_KNIFE_DATA;

typedef struct
{
	int angle512;	/*union BULLET_DATA*/
	int speed256;	/*union BULLET_DATA*/

	int d_angle512;
	int length256;
	int x256;
	int y256;
	int state;
	int wait1;	//fire
	int wait2;	//act
} EVEN_KNIFE_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	int anime_count;			/* �A�j���[�V�������x�̃J�E���^ */
	int anime_speed;			/* �A�j���[�V�������x�̐ݒ葬�x */
	int wait_count1;			/* �n�܂�܂ł̑҂����� */
} EXPLOSION_DATA;

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
/*
���낢��ƌ��ؗp�Ȃ̂ŃR�����g�A�E�g
void bullet_way(SPRITE *s ,int way, dou ble angle512, dou ble angle2, dou ble spd)
{
	//�X�v���C�g���, n way, �p�x�x�[�X, �����p�x��, ���x
	int i;
	for (i=0;i<way;i++)
	{
		SPRITE *h;
		h					= spr ite_add_file 0("kugel.png",1,PRIORITY_05_BULLETS/ * PRIORITY_03_ENEMY * /,0);
		h->type 			= SP_BULLET;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover	= move_bullet_angle512;
		h->anim_frame		= 0;
		h->x256 			= s->x256+((s->w128-h->w128));
		h->y256 			= s->y256+((s->h128-h->h128));
		BULLET_DATA *data;
		data				= mmalloc(sizeof(BULLET_DATA));
		h->data 			= data;
		data->angle 		= angle-angle2*(way/2)+angle2*i;
		data->speed 		= spd;
	}
}
*/
#endif

/*---------------------------------------------------------
	�p�x�e(��{�ʈړ�)
---------------------------------------------------------*/

static void mono_move(SPRITE *s, BULLET_DATA *d)
{
	s->x256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
}

/*---------------------------------------------------------
	���x�l�A�p�x�l�A���x�N�g���ʂɕϊ�����
---------------------------------------------------------*/
static void regist_vector(SPRITE *s, int speed256, int angle512)
{
	BULLET_VECTOR_DATA *data=(BULLET_VECTOR_DATA *)s->data;
	data->vx256 	= ((cos512((angle512))*speed256)>>8)/**fps_fa ctor*/;
	data->vy256 	= ((sin512((angle512))*speed256)>>8)/**fps_fa ctor*/;
}

/*[r13] cosf sinf */
#if 0
/*---------------------------------------------------------

---------------------------------------------------------*/
//static void enemy_laser_move2(SPRITE *s)
//static void enemy_laser_move(SPRITE *s)
static void move_bullet_angle512(SPRITE *s)
{
	/*LASER_DATA*/BULLET_DATA *d = (/*LASER_DATA*/BULLET_DATA *)s->data;

	//s->anim_frame=(radtodeg(d->angle)/10)+9;	//s->anim_frame%=18;
	mono_move(s,/*(BULLET_DATA *)*/d);

	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}
#endif
/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *s)
{
	BULLET_VECTOR_DATA *data=(BULLET_VECTOR_DATA *)s->data;
	//s->anim_frame=(radtodeg(d->angle)/10)+9;	//s->anim_frame%=18;
//	mono_move(s,/*(BULLET_DATA *)*/d);
	s->x256 += (data->vx256)/**fps_fa ctor*/;
	s->y256 += (data->vy256)/**fps_fa ctor*/;

	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}


/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e
---------------------------------------------------------*/

#if 1
static void enemy_sp1_bullet_move(SPRITE *s)
{
	BULLET_SP1_DATA *d = (BULLET_SP1_DATA *)s->data;
	d->timer++;
	if (d->timer > 40)
	{
		d->sum256 += d->gra256;
		s->x256 += ((((cos512((d->angle512))*(d->speed256))>>8)*(d->sum256>>2/*/5*/))>>8) + ((cos512((d->angle2_512))*(d->sum256))>>8)/**fps_fa ctor*/;
		s->y256 += ((((sin512((d->angle512))*(d->speed256))>>8)*(d->sum256>>2/*/5*/))>>8) + ((sin512((d->angle2_512))*(d->sum256))>>8)/**fps_fa ctor*/;
	}
	else if (d->timer==40)
	{
		s->anim_frame = 1;
		d->angle512 = (*(d->sd_angle512))/*(d->sd->angle512)*/+deg_360_to_512(90)*d->r_or_l;
		mask512(d->angle512);
		d->angle2_512 = (*(d->sd_angle512))/*(d->sd->angle512)*/;
	}
	else
	{
		s->x256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		s->y256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	}
	if ((s->x256<0/*(???)-s->x256*/)||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<0/*(???)-s->y256*/)||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}
#endif

/*---------------------------------------------------------
	���p�̖����w�p
---------------------------------------------------------*/

static void enemy_sp2_bullet_move(SPRITE *s)
{
	{	BULLET_SP2_DATA *d = (BULLET_SP2_DATA *)s->data;
	//	if (2==d->sd->nnn/*%(4-difficulty)*/)/* �X�e�[�g���P�̎��z�u���āA�X�e�[�g���Q�ɂȂ����瓮�� */
		if (2==(*(d->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g���P�̎��z�u���āA�X�e�[�g���Q�ɂȂ����瓮�� */
		{
			d->sum256 += d->gra256;
			s->x256 += ((cos512((d->angle512))*(d->sum256))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((d->angle512))*(d->sum256))>>8)/**fps_fa ctor*/;
		}
	}
	if ((s->x256<0/*(???)-s->x256*/)||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<0/*(???)-s->y256*/)||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}


/*---------------------------------------------------------
	�\���̂ݕ���
	-------------------------------------------------------
	�얲�̗쌂�̏ꍇ�A��ʂ�������̂ŁA�V�X�e�����Őe����������B
	���ׁ̈A�q�͓Ɨ����Ď�����������B
	��ʂ�������̂��킩��Ղ��悤�ɁA8�t���[���A�E�F�C�g��������
	��ʎ��̂̏d��(�h��������)��ǉ����Ă݂��B
	�h�������������炵�����ꍇ�́A�����t���[���������炷�B
---------------------------------------------------------*/

static void move_oodama_kodomo(SPRITE *s)
{
	OODAMA_KO_DATA *data=(OODAMA_KO_DATA *)s->data;

	/* ���t���[�������Ȃ��ꍇ�́A������������ */
	if ((data->check_x256 != s->x256) ||
		(data->check_y256 != s->y256))
	{
		data->time_out	= 8;/* �������玩���������Ȃ� */
	}
	data->time_out--;
	if ( 1 > data->time_out)
	{
		s->type=SP_DELETE;/*�����܂�*/
	}
	data->check_x256 = s->x256;
	data->check_y256 = s->y256;
}

/* ���̕����ł͐e����Ɏ���Ŏq�����s���s���ɂȂ�ꍇ������B(�T�C�Y�� �e==�q�Ȃ���Ȃ�)
	�T�C�Y�� �e���q �ȈׁA�e�̂݉�ʊO�ɂ͂ݏo�����ꍇ��
	�P�[�X���O�o�[�W�����ōl������Ă��Ȃ��A
	�P���� if (*(d->type)==SP_DELETE) �Ŕ��f�o���Ȃ��B
	���ɍė��p����Ă���ꍇ������B  */
//	s->x256 = *(data->sx256p)+((data->sw-s->w128));
//	s->y256 = *(data->sy256p)+((data->sh-s->h128));
//	if (*(data->type)==SP_DELETE)���ɍė��p����Ă���ꍇ������̂ŁA����ł͂��������Ȃ�
//	{
//		s->type=SP_DELETE;
//	}

static void oodama_hyouji(SPRITE *ko, SPRITE *s)
{
	ko->x256=s->x256+((s->w128-ko->w128));/**/
	ko->y256=s->y256+((s->h128-ko->h128));/**/
//	if (SP_DELETE==s->type)/* �e�Ɠ����ɂ����܂� */
//	{
//	//	ko->type=SP_DELETE;/*�����܂�*/
//	}
}

/*---------------------------------------------------------
	�{�̕���
---------------------------------------------------------*/

static void move_oodama1_oya(SPRITE *s)
{
	OODAMA_OYA_DATA *d = (OODAMA_OYA_DATA *)s->data;

	// [***090124		�ǉ��ꏊ�B���܂Ńt���[�����Ɍv�Z���Ă�������5(4)�t���[�����ɕύX
	d->time_out--;
	if (0==(d->time_out&0x03)) //if (d->wait_bg>0)	{	d->wait_bg--;}	else
	{
	//	d->wait_bg=5;
	//	d->angle512 += rad2 deg512(((ra_nd()%d->ransu)-d->ransu/2)/10);
		d->angle512 += ((((ra_nd()&(256-1))*d->ransu512)-((d->ransu512)<<7))>>8);
		mask512(d->angle512);
	}
	mono_move(s,(BULLET_DATA *)d);
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
	}
	oodama_hyouji(d->hyouji, s);
}

/*---------------------------------------------------------
	�{�̕����Q
---------------------------------------------------------*/

static void move_oodama2_oya(SPRITE *s)
{
	OODAMA_OYA_DATA *d = (OODAMA_OYA_DATA *)s->data;
	d->speed256 += d->add_speed256;
	mono_move(s,(BULLET_DATA *)d);
	if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
	}
	oodama_hyouji(d->hyouji, s);
}

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
	//	return (atan_512(p->y256-t->y256				 ,p->x256-t->x256				  ));//2082417					/* �ȗ���(�e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ�) */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_��(?) �Ȃ񂩍��ɂ���� */
		return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128-t->w128))));/**/ 				/* ��e�̏ꍇ�Ɏ��@�_�� */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256+((p->w128-t->w128))));/**/ 				/* �����e�̏ꍇ�Ɏ��@�_�� */
	//	return (atan_512(p->y256-t->y256+((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	//	return (atan_512(p->y256-t->y256-((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	#endif
}
//1287513
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512)
{
	if (ANGLE_JIKINERAI_DAN==angle512)	{	angle512	= angle_jikinerai512(p,t);}
	return (angle512);
}
/*---------------------------------------------------------
	�G�̗U���z�[�~���O�{��(�Z�p�`�Ş�F�̓z)
---------------------------------------------------------*/

static void enemy_homing_move(SPRITE *s)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	if (pd->state_flag & STATE_FLAG_03_SCORE_AUTO_GET_ITEM)
	if (pd->state_flag & (STATE_FLAG_06_IS_SCRIPT|STATE_FLAG_05_IS_BOSS) )
	{	/* �{�X�Ɏ������ނƍ����_ */
	//	bonus_info_score(s,SCORE_1000);/*���������֎d�l�ύXs->type=SP_DELETE;*/
		item_create(s, SP_ITEM_05_HOSI, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );s->type=SP_DELETE;
	}
//
	//SDL_Surface *tmp;
	ENEMY_HOMING_DATA *d = (ENEMY_HOMING_DATA *)s->data;
	d->time_out -= 1/*fps_fa ctor*/;	//d->delay -= 1/*fps_fa ctor*/; d->range-=1/*fps_fa ctor*/;
	if ( 0x00 == (d->time_out&0x0f) )/*0x07*/
	{
		/* nur jeden 10ten Frame Winkel aktualisieren */
		//d->delay=10;
		//enemy_homing_update(s);
		//static void enemy_homing_update(SPRITE *s)
		{
			ENEMY_HOMING_DATA *d = (ENEMY_HOMING_DATA *)s->data;
			/* Zielwinkel erfassen */
			d->angle512 = angle_jikinerai512(player,s);
			mask512(d->angle512);
		//	s->anim_frame = (deg_512_to_360(d->angle512)*20)/360; s->anim_frame %=20;
		//	s->anim_frame = (((d->angle512)*(20/2))>>8);
		//	s->anim_frame = (((d->angle512)*(16/2))>>8);
			s->anim_frame = (((d->angle512))>>(8-3));
		}
	}
	else	/*if (d->delay > 0)*/
	{
		#if 0
		/*�_��mono_move(s,(BULLET_DATA *)d);*/
		#else
		s->x256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		s->y256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		#endif
	}
	/* max. reichweite erreicht? self-destruct! */
	if (d->time_out < 1 )	//if (d->range<=0)
	{
		s->type=SP_DELETE;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//
static void enemy_smallbullet_re_create(SPRITE *s, int speed256, int angle512, int offset_angle512, int adjust_speed256);
static void enemy_momiji_move(SPRITE *s)
{
	MOMIJI_DATA *d = (MOMIJI_DATA *)s->data;
	mono_move(s,(BULLET_DATA *)d);
	d->time_out -= 1/*fps_fa ctor*/;
	if (d->time_out < 1)
	{
		/* 5�������݂��e���쐬 */
		int jj;
		for (jj=-(int)(512*2/12); jj<(int)(512*3/12); jj+=(int)(512*1/12) )
		{
		//	enemy_smallbullet_re_create(s, t256(3), d->angle512, -(int)(512*2/12), t256(0.04));
		//	enemy_smallbullet_re_create(s, t256(3), d->angle512, -(int)(512*1/12), t256(0.04));
			enemy_smallbullet_re_create(s, t256(3), d->angle512, jj,				 t256(0.04));
		//	enemy_smallbullet_re_create(s, t256(3), d->angle512, +(int)(512*1/12), t256(0.04));
		//	enemy_smallbullet_re_create(s, t256(3), d->angle512, +(int)(512*2/12), t256(0.04));
		}
		s->type=SP_DELETE;
	}
	if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_smallbullet_re_move(SPRITE *s)
{
	SMALL_RE_DATA *d = (SMALL_RE_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if (d->speed256 < t256(0.5) )
		{
			d->state		= 1;
			d->angle222_512 += 256/*rad2 deg512(M_PI)*/;
			#if 1
			/*�˂�̂���*/
			mask512(d->angle222_512);
			#endif
			d->angle512 	= d->angle222_512;
		}
		else
		{
			d->speed256 -= d->adjust_speed256;
		}
		break;
	case 1:
		d->speed256 += d->adjust_speed256;
		break;
	}
	if (d->angle512-d->angle222_512 < (int)(512*1/12) ) 	{	d->angle512 += 1/*0.01*/;	}
			#if 1
			/*�˂�̂���*/
			mask512(d->angle512);
			#endif
	mono_move(s,(BULLET_DATA *)d);

	/* ����ܗǂ��Ȃ� */
	if (200 < s->timeover_ticks)
	{
		if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
			(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
		{
			s->type=SP_DELETE;
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static void bullet_create_oodama2(SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256);
//atic void enemy_stop_bullet_move(SPRITE *s)
static void enemy_stop_bullet2_move(SPRITE *s)
{
	TOMARI_DAN_DATA *d = (TOMARI_DAN_DATA *)s->data;
	switch (d->state)
	{
	case 0:/*�~�܂�*/
		d->speed256 -= (d->delta256);
		if (d->speed256 < t256(0))
		{
			d->speed256 = t256(0);
			d->state	= 1;
		}
		break;
	case 1:/*���΂���~*/
		d->timer--;
		if (64 > d->timer)
		{
			d->state	= 2;
		}
		break;
	case 2:/*����(��ʂɕϐg)*/
		d->timer--;
		s->anim_frame	= ((d->timer>>3)/*&0x07*/); 			/*�x��(�ϐg����̂�)*/
		if (1 > d->timer)
		{
		//	d->timer	= 0;
			d->state	= 3;
			if (ANGLE_NO_SHOT_DAN != d->next_angle512)
			{
				bullet_create_oodama2(s, t256(0), d->next_angle512, 0, (d->delta256*7), 0, 0);
			}
		}
		break;
	case 3: /*������*/
		s->alpha -= 16;
		if ( 32 > s->alpha)
		{
			s->alpha = 0;
			s->type = SP_DELETE;
		}
		break;
	}
	/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	if (ANGLE_NO_SHOT_DAN == d->next_angle512)
	{	/* �X�N���[������ */
		s->y256 += t256(1); 	//	my_adjust_y(s,pd);
	}
//	mono_move(s,(BULLET_DATA *)d);
	s->x256+=(((d->vx256)*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=(((d->vy256)*d->speed256)>>8)/**fps_fa ctor*/;

	#if 1
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(-100))||(s->y256 > t256(GAME_HEIGHT))
	#else
	if ((s->x256<t256(-50))||(s->x256>t256(GAME_WIDTH+60))||
		(s->y256<t256(-50))||(s->y256>t256(GAME_HEIGHT+60))
	#endif
	//	||(s->alpha<0)
	)
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_g_bullet_move(SPRITE *s)
{
	G_BULLET_DATA *d = (G_BULLET_DATA *)s->data;
	s->x256 += ((d->state_lo/*%10*/)<<8);
	s->y256 += ((d->state_hi/*/10*/)<<8);

	mono_move(s,(BULLET_DATA *)d);
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
---------------------------------------------------------*/

static void enemy_pong_bullet_move(SPRITE *s)
{
	HAZUMI_DAN_DATA *d = (HAZUMI_DAN_DATA *)s->data;
	if (d->sum256 < t256(15) )
	{	d->sum256 += d->gra256;}
	s->y256 += d->sum256;
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH)))
	{
		d->angle512 		= atan_512(sin512((d->angle512)),-cos512((d->angle512)));
	}
	else if (s->y256 > t256(GAME_HEIGHT))
	{
		if (d->bounds)
		{
			d->speed256 	= -d->speed256;
			d->sum256		= -d->sum256;
			d->bounds--;
			s->anim_frame	= d->bounds;		// [***090116		�኱�ύX
			d->angle512 	= atan_512(sin512((d->angle512)),-cos512((d->angle512)));		// [***090116		�ǉ�
		}
		else
		{	s->type=SP_DELETE;}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_gr_bullet_move(SPRITE *s)
{
	GR_BULLET_DATA *d = (GR_BULLET_DATA *)s->data;
	d->sum256+=d->gra256;
	s->y256+=d->sum256;
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(-50))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_angle_bullet_move(SPRITE *s)
{
	BULLET_4096_DATA *data=(BULLET_4096_DATA *)s->data;
	if (data->time_count<90)
	{
		data->time_count++;
		data->angle4096 += data->d_angle4096;
		mask4096(data->angle4096);
	//	data->angle512_4096 = ((d->angle4096)>>3);
	//	mask512(data->angle512_4096);
		int angle512 = ((data->angle4096)>>3);
		data->vx256 	= ((cos512((angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		data->vy256 	= ((sin512((angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	}
	s->y256 += t256(1)/*fps_fa ctor*/;
//	mono_move(s,(BULLET_DATA *)d);
	s->x256+=(data->vx256)/**fps_fa ctor*/;
	s->y256+=(data->vy256)/**fps_fa ctor*/;
//
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(-100))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
static void enemy_knife_move(SPRITE *s)
{
	BULLET_DATA *d = (BULLET_DATA *)s->data;
	mono_move(s,d);
	if ((s->x256+((s->w128+s->w128)) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256+((s->h128+s->h128)) < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_fall_knife_move(SPRITE *s)
{
	FALL_KNIFE_DATA *d = (FALL_KNIFE_DATA *)s->data;
	int tmp256;
	tmp256 = ((sin512((d->angle512))*(d->speed256))>>8)/**fps_fa ctor*/;
	if (tmp256+d->sum256 <= t256(3) )
	{	d->sum256 += d->gra256;}
	s->x256 += ((cos512((d->angle512))*(d->speed256))>>8)/**fps_fa ctor*/;
	s->y256 += tmp256 + d->sum256;
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(-100))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type=SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_follow_knife_move(SPRITE *s)
{
	FOLLOW_KNIFE_DATA *d = (FOLLOW_KNIFE_DATA *)s->data;
	if (d->target)
	{
		s->x256 += ((cos512((d->angle512))*(d->speed2_256))>>8)*2/**fps_fa ctor*/;
		s->y256 += ((sin512((d->angle512))*(d->speed2_256))>>8)*2/**fps_fa ctor*/;
	}
	else
	{
		mono_move(s,(BULLET_DATA *)d);
		d->speed256 -= t256(0.02);
		if (s->anim_frame==17)	{	s->anim_frame = 0;}
		else					{	s->anim_frame++;}
		if (d->speed256 <= t256(0) )
		{
			d->angle512 = atan_512(
				(player->y256)-(s->y256)+(player->h128)+(d->height*256),
				(player->x256)-(s->x256)-(player->w128)  );
			d->target=1;
		//	s->anim_frame=/*17-*/(deg_512_to_360(d->angle512)/20)%18;
		//	s->anim_frame=/*17-*/(deg_512_to_360(d->angle512)/20)%16;
			mask512(d->angle512);/*�˂�̂���*/
			s->anim_frame=/*17-*/((d->angle512)>>5);
		}
	}
	d->time_out--;
	if (d->time_out<0)
	{
		if (((s->x256+((s->w128+s->w128)) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
			 (s->y256+((s->h128+s->h128)) < t256(0))||(s->y256 > t256(GAME_HEIGHT))))
		{
			s->type=SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
		}
	}
}

/*---------------------------------------------------------

	-------------------------------------------------------
	s->x256 	�i�C�t��x���W
	d->x256 	�m�[�h��x���W
	s->w/2		�i�C�t�̉����̔���
	�i�C�t��x���W���m�[�h��x���W�{co_s512((d->angle512))�~length�|�i�C�t�̉����̔���
---------------------------------------------------------*/

#if 1
static void enemy_evenl_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d = (EVEN_KNIFE_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if (d->wait1>100)
		{
			d->wait1 = 0;
			d->state = 1;
		}
		d->angle512 += d->d_angle512 /**fps_fa ctor*/;
		mask512(d->angle512);//if (d->angle512 > rad2 deg512(M_PI) )		{	d->angle512 =- rad2 deg512(M_PI);}
	//	s->anim_frame	= /*17-*/(deg_512_to_360(d->angle512-(int)(512*1/4))/20)%18;
	//	s->anim_frame	= /*17-*/(deg_512_to_360(d->angle512-(int)(512*1/4))/20)%16;
		{
			int tmp512;
			tmp512 = (d->angle512-(int)(512*1/4));
			mask512(tmp512);/*�K���v��*/
			s->anim_frame	= /*17-*/((tmp512)>>5);
		}
		s->x256 		= d->x256-((cos512((d->angle512))*d->length256)>>8)-((s->w128));
		s->y256 		= d->y256-((sin512((d->angle512))*d->length256)>>8)-((s->h128));
		//s->anim_frame = (d->angle/10)%36;
		break;

	case 1:
		if (d->wait1>50)
		{
			d->wait1 = 0;
			d->state = 0;
		}
		mono_move(s,(BULLET_DATA *)d);
		break;
	}
	if (d->wait1>10)
	{
		d->wait1 = 0;
		enemy_knife_create(s, t256(5.0), d->angle512-(int)(512*3/12), s->anim_frame);
		enemy_knife_create(s, t256(4.0), d->angle512-(int)(512*4/12), s->anim_frame);
		enemy_knife_create(s, t256(3.0), d->angle512-(int)(512*5/12), s->anim_frame);
		if (difficulty>1)
		{	enemy_knife_create(s, t256(2.0), d->angle512-(int)(512*6/12), s->anim_frame);}
	}
	//s->type=SP_DELETE;
	if (d->wait2>150+difficulty*50)
	{	s->type=SP_DELETE;}

	d->wait1++;
	d->wait2++;
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_evenr_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d = (EVEN_KNIFE_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if (d->wait1>100)
		{
			d->wait1 = 0;
			d->state = 1;
		}
		d->angle512 -= d->d_angle512 /**fps_fa ctor*/;
		mask512(d->angle512);//if (d->angle512 < -rad2 deg512(M_PI))	{	d->angle512=rad2 deg512(M_PI);}
	//	s->anim_frame	= /*17-*/(deg_512_to_360(d->angle512+(int)(512*1/4))/20)%18;
	//	s->anim_frame	= /*17-*/(deg_512_to_360(d->angle512+(int)(512*1/4))/20)%16;
		{
			int tmp512;
			tmp512 = (d->angle512+(int)(512*1/4));
			mask512(tmp512);/*�K���v��*/
			s->anim_frame	= /*17-*/((tmp512)>>5);
		}
		s->x256 		= d->x256-((cos512((d->angle512))*d->length256)>>8)-((s->w128));
		s->y256 		= d->y256-((sin512((d->angle512))*d->length256)>>8)-((s->h128));
		//s->anim_frame = (d->angle/10)%36;
		break;

	case 1:
		if (d->wait1>50)
		{
			d->wait1 = 0;
			d->state = 0;
		}
		//#if 1
		//d->speed=20/*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		//#endif
		mono_move(s,(BULLET_DATA *)d);
		break;
	}



	if (d->wait1>10)
	{
		d->wait1 = 0;
		enemy_knife_create(s, t256(5.0), d->angle512+(int)(512*3/12), s->anim_frame); //+M_PI/2
		enemy_knife_create(s, t256(4.0), d->angle512+(int)(512*4/12), s->anim_frame);
		enemy_knife_create(s, t256(3.0), d->angle512+(int)(512*5/12), s->anim_frame);
		if (difficulty>1)
		{	enemy_knife_create(s, t256(2.0), d->angle512+(int)(512*6/12), s->anim_frame);}
	}
	//s->type=SP_DELETE;
	if (d->wait2>150+difficulty*50)
	{	s->type=SP_DELETE;}

	d->wait1++;
	d->wait2++;
}

/*---------------------------------------------------------
	�����G�t�F�N�g�̕\��
---------------------------------------------------------*/

static void move_explosion(SPRITE *s)
{
	EXPLOSION_DATA *data=(EXPLOSION_DATA *)s->data;
	if (0 < data->wait_count1)
	{	data->wait_count1 -= 1/*fps_fa ctor*/;}
	else
	{
		s->flags			|= (SP_FLAG_VISIBLE);/* �\�� on */
		data->anime_count -= 1/*fps_fa ctor*/;
		if (data->anime_count < 1)
		{
			data->anime_count = data->anime_speed;
			if (s->anim_frame < s->frames-1)	{	s->anim_frame++;	}
			else								{	s->type=SP_DELETE;	}
		}
	}
}

/*---------------------------------------------------------
	�v���C���[�_���e(��)���쐬����
---------------------------------------------------------*/
//enemy_bullet_create
void bullet_create_aka_maru_jikinerai(SPRITE *s, int speed256)
{
	SPRITE *h;
//	h					= sprite_add_res(BASE_TAMA_KUGEL_PNG);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);
	h->type 			= SP_BULLET;
	h->callback_mover	= move_bullet_vector/*move_bullet_angle512*/;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	BULLET_VECTOR_DATA *data;
	data				= mmalloc(sizeof(BULLET_DATA));
	h->data 			= data;
//
//	data->angle512		= angle_jikinerai512(player,s);
//	data->speed256		= speed256;
	int i_angle512;
	i_angle512		= angle_jikinerai512(player,s);
	regist_vector(h, speed256, i_angle512);
}


/*---------------------------------------------------------
	�j�e���쐬����
---------------------------------------------------------*/

void bullet_create_hari_dan180(SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256 )
{
	angle512			= angle_jikinerai512_auto(player, s, angle512);
//
	SPRITE *h;
//	h					= spr ite_add_file 0(/*"bs hoot.png"*/"tama/bullet_beam16.png",16/*1*/,PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY_WEAPON*/ /*PRIORITY_03_ENEMY*/, 0);
	h					= sprite_add_res(BASE_TAMA_BULLET_BEAM16_PNG);
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= move_bullet_vector/*move_bullet_angle512*/;/*enemy_laser_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= ((angle512>>4)&0x0f);/*180�x*/	//b->anim_frame = 0;
	h->x256 			= s->x256+((s->w128-h->w128)) + x_offset256;
	h->y256 			= s->y256+((s->h128-h->h128)) + y_offset256;
	{	/*LASER_DATA*/BULLET_VECTOR_DATA *data;
		data			= mmalloc(sizeof(/*LASER_DATA*/BULLET_DATA));
		h->data 		= data;
		mask512(angle512);/*�O�̈�*/
	//	data->angle512 = /*deg512_2rad*/(angle512);
	//	data->speed256 = (speed256);
	regist_vector(h, speed256, angle512);
	}
}

/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

void bullet_create_enemy_homing(SPRITE *s)
{
	SPRITE *r;
	r					= sprite_add_res(BASE_HOMING16_PNG);	/*�f�ޕύX*/ /*20"rotating_rocket.png"*/
	r->type 			= SP_ZAKO;/*SP_ZAKO*/ /*SP_BULLET*/ /*SP_ENEMY_HOMING*/
	r->callback_mover	= enemy_homing_move;
	r->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	r->x256 			= s->x256;
	r->y256 			= s->y256;
	ENEMY_HOMING_DATA *data;
	data				= mmalloc(sizeof(ENEMY_HOMING_DATA));
	r->data 			= data;
	data->b.score		= score(5*2);
	data->b.health		= 1;		/* �Ȃ񂩋����C������ */
	data->angle512		= (0);
	data->speed256		= t256(0.5)+((difficulty)<<7)/*(0==difficulty)?(1):(2)*/;
	data->time_out		= ((12*16)+1);//data->range 	= 200;	data->delay 	= 1/*10*/;	/*enemy_homing_update(r);*/
}

/*---------------------------------------------------------
	�m�[�}���e
---------------------------------------------------------*/
void bullet_create_maru8_frame(SPRITE *s, int speed256, int angle512, int set_frame)
{		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
	h					= NULL;
//	h					= spr ite_add_file 0("tama/bs hoot.png", 1, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
//	h					= spr ite_add_file 0("tama/bullet_maru8.png", 8, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= move_bullet_vector/*move_bullet_angle512*/;/*enemy_laser_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= set_frame;
	h->x256 			= (s->x256)+((s->w128-h->w128));
	h->y256 			= (s->y256)+((s->h128-h->h128));
	BULLET_VECTOR_DATA *data;
	data				= NULL;
	data				= mmalloc(sizeof(BULLET_DATA));
	h->data 			= data;
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
}

/*---------------------------------------------------------
	�����e
---------------------------------------------------------*/

void enemy_momiji_create(SPRITE *s, int speed256, int angle512)
{	/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/sp ell_bullet_r.png", 1, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_momiji_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 1;/*�Ƃ肠����*/
	h->x256 			= (s->x256)+((s->w128));
	h->y256 			= (s->y256)+((s->h128));
	MOMIJI_DATA *data;
	data				= mmalloc(sizeof(MOMIJI_DATA));
	h->data 			= data;
	data->angle512		= (angle512);
	data->speed256		= (speed256);
	data->time_out		= ((30)+1);
}

/*---------------------------------------------------------
	���e����
---------------------------------------------------------*/

static void enemy_smallbullet_re_create(SPRITE *s, int speed256, int angle512, int offset_angle512, int adjust_speed256)
{
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/kugel2.png", 1, /*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
//	h					= spr ite_add_res(BASE_TAMA_KUGEL_MINI2_PNG);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);
	h->type 			= SP_BULLET;
	h->callback_mover	= enemy_smallbullet_re_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= (8+(angle512>>6)) /*8*/ /*0*/;
	h->x256 			= (s->x256)+((s->w128));
	h->y256 			= (s->y256)+((s->h128));
	SMALL_RE_DATA *data;
	data				= mmalloc(sizeof(SMALL_RE_DATA));
	h->data 			= data;
	data->state 		= 0;
	angle512 += offset_angle512;
	mask512(angle512);
	data->angle222_512	= (angle512);
	data->angle512		= (angle512);
	data->speed256		= (speed256);
	data->adjust_speed256	= adjust_speed256;
}


/*---------------------------------------------------------

	���e�ɕςȊp�x�w��@�\��t�������́B
	���C�t�������ǂ���}�C�i�X�̒l�����Ȃ��ˁB�Ӗ��킩���B
	�g���Ă�͓̂V�炾���B
	-------------------------------------------------------
	state		�\�i���ň�̈ʂ�x�A�\�̈ʂ�y	�w�肵���������p�x���ς��B
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�Ɍ������B
---------------------------------------------------------*/

void enemy_gg_bullet_create(SPRITE *s, int speed256, int angle512, int state_hi, int state_lo)
{
	SPRITE *h;
//	h					= sprite_add_res(BASE_TAMA_KUGEL_PNG);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);
	h->type 			= SP_BULLET;
	h->callback_mover	= enemy_g_bullet_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	G_BULLET_DATA *data;
	data				= mmalloc(sizeof(G_BULLET_DATA));
	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	data->angle512		= angle512;
	data->speed256		= speed256;
	data->state_hi		= state_hi;
	data->state_lo		= state_lo;
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/

// [***090114	�ǉ�
void bullet_create_hazumi_dan(SPRITE *s, int speed256, int angle512, int delta256, int bounds)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot2.png",3,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);/*�Ή��e*/
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);/*�Ή��e*/
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_pong_bullet_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 4+bounds;/*�Ƃ肠����*/
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	HAZUMI_DAN_DATA *data;
	data				= mmalloc(sizeof(HAZUMI_DAN_DATA));
	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	data->angle512		= angle512;
	data->speed256		= speed256;
	data->gra256		= (delta256);
	data->sum256		= t256(0);
	data->bounds		= bounds;
}

/*---------------------------------------------------------

	-------------------------------------------------------
	�������ɉ������Ă������e�B
	�t���[�����Ōv�Z���Ă���̂ł��Ȃ�����x�����������Ȃ��Ƒ�ςȂ��ƂɁB
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/

void enemy_gr_bullet_create(SPRITE *s, int speed256, int angle512, int delta256)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);/*�e*/
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);/*�e*/
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_gr_bullet_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 7/*0*/;/*�Ƃ肠����*/
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	GR_BULLET_DATA *data;
	data				= mmalloc(sizeof(GR_BULLET_DATA));
	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	data->angle512		= angle512;
	data->speed256		= speed256;
	data->gra256		= (delta256);
	data->sum256		= t256(0);
}



/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e
	-------------------------------------------------------
	���̒e�͑��̒e�������̂ł����Ēx������B
---------------------------------------------------------*/

/*static*/ void enemy_sp1_bullet_create(SPRITE *s, int speed256, int angle512, int gra256, int r_or_l, int *sd_angle512)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot2.png", 3, PRIORITY_03_ENEMY, 0);
//	h					= spr ite_add_file 0("tama/bullet_maru8.png", 8, PRIORITY_03_ENEMY, 0);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);		h->priority 	= PRIORITY_03_ENEMY;
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_sp1_bullet_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= /*4*/((ra_nd()&(4-1))+4)/*0*/;/*�Ƃ肠����*/
	h->anim_speed		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	BULLET_SP1_DATA *data;
	data				= mmalloc(sizeof(BULLET_SP1_DATA));
	h->data 			= data;
//	if (angle512==999/*-2*/)
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	data->angle512		= angle512;
	data->angle2_512	= (0);
//	data->sd			= (BOSS05_DATA *)s->data;
	data->sd_angle512	= sd_angle512;
	data->speed256		= /*t256_to_dou ble*/(speed256);
	data->gra256		= gra256;
	data->sum256		= t256(0);
	data->timer 		= 0;
	data->r_or_l		= r_or_l;
}

#if 1/*�����w�p*/


/*---------------------------------------------------------
	���̖����w�p
	angle512	512�x�w��
---------------------------------------------------------*/

/*static*/ void enemy_sp2_bullet_create(SPRITE *s, int angle512, int gra256, int *sd_nnn)
{		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/kugel.png", 1, PRIORITY_03_ENEMY, 0);
//	h					= spr ite_add_res(BASE_TAMA_KUGEL_PNG); h->priority 	= PRIORITY_03_ENEMY;
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);	h->priority 	= PRIORITY_03_ENEMY;
	h->type 			= SP_BULLET;
	h->callback_mover	= enemy_sp2_bullet_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->anim_speed		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	BULLET_SP2_DATA *data;
	data				= mmalloc(sizeof(BULLET_SP2_DATA));
	h->data 			= data;
//	data->sd			= (BOSS05_MAHO_DATA *)s->data;
	data->sd_nnn		= sd_nnn;
	data->state 		= 0;
	data->angle512		= (angle512);
	data->gra256		= gra256;
	data->sum256		= t256(0);
}
#endif



/*---------------------------------------------------------
	�΂�܂��U�������w(�ʏ̃}���z�[��)���΂�܂��A�~�܂�e
	-------------------------------------------------------
	��邪�΂�܂��A�~�܂�e�B��ʂɕω�����
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[������speed���ǂꂾ���������邩
---------------------------------------------------------*/
//id bullet_create_tomari_dan( SPRITE *s, int speed256, int angle512, int delta256, int next_angle512)
void bullet_create_tomari2_dan(SPRITE *s, int speed256, int angle512, int delta256, int next_angle512)
{
	SPRITE *h;
//	h					= sprite_add_res(BASE_TAMA_KUGEL_PNG);/*���Ԓe*/
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h->type 			= SP_BULLET;
	h->callback_mover	= enemy_stop_bullet2_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	TOMARI_DAN_DATA *data;
	data				= mmalloc(sizeof(TOMARI_DAN_DATA));
	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, s, angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	data->speed256		= (speed256);
	regist_vector(h, t256(1.0)/*speed256*/, angle512);
//
	data->state 		= 0;
	data->delta256		= (delta256);
	if (ANGLE_NO_SHOT_DAN==next_angle512)/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	{
		data->timer 	= 800;
	}
	else
	{
		data->timer 	= 100;
		mask512(next_angle512);
	}
	data->next_angle512 = (next_angle512);
}

/*---------------------------------------------------------
	4096�����e
---------------------------------------------------------*/

void enemy_angle_bullet_create(SPRITE *s, int speed256, int angle4096, int d_angle4096)
{
	SPRITE *h;
//	h					= sprite_add_res(BASE_TAMA_KUGEL_PNG);
	h					= sprite_add_res(BASE_TAMA_BULLET_MARU16_PNG);
	h->type 			= SP_BULLET;
	h->callback_mover	= enemy_angle_bullet_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	BULLET_4096_DATA *data;
	data				= mmalloc(sizeof(BULLET_4096_DATA));
	h->data 			= data;
	mask4096(angle4096);
	data->angle4096 	= (angle4096);
//	data->angle512_4096 = ((angle4096)>>3);
//	mask512(data->angle512_4096);
//	int speed256;
//	speed256 = /*t256_to_dou ble*/(speed256);
	data->speed256		= speed256;
	data->d_angle4096	= (d_angle4096);
	data->time_count	= 0;
}

/*---------------------------------------------------------
	�����~���i�C�t(��)
---------------------------------------------------------*/

void enemy_fall_knife_create(SPRITE *s, int speed256, int angle512, int gra256)
{
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/knife.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,1);/*�����~���i�C�t(��)*/
	h					= sprite_add_res(BASE_TAMA_BULLET_KNIFE01_PNG);/*�����~���i�C�t(��)*/
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_fall_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	FALL_KNIFE_DATA *data;
	data				= mmalloc(sizeof(FALL_KNIFE_DATA));
	h->data 			= data;
	mask512(angle512);
	data->angle512		= (angle512);
	data->speed256		= speed256;
	data->gra256		= (gra256);
	data->sum256		= t256(0);
}

/*---------------------------------------------------------
	�S�����i�C�t(��)	[�S�����i�C�t]
---------------------------------------------------------*/
extern int select_player;
void enemy_knife_create(SPRITE *s, int speed256, int angle512, int anime16_pattern)
{
	SPRITE *h;
	#if 0
	char filename[32/*30*/];
	sp rintf(filename,"tama/knife%d.png",anime18_pattern);/* ���x�̗v��ꏊ�ł� sp rintf() �͎g��Ȃ������ǂ� */	/* �����񏈗�(sp rintf)�͔��ɒx�� */
	h					= spr ite_add_file 0(filename,1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,1);/*�S�����i�C�t(��)*/
	h->type 			= SP_BULLET;
	h->callback_mover	= move_bullet_vector/*move_bullet_angle512*/;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= /*anim%18*/0;
	#else
//	h					= spr ite_add_file 0("tama/knife_core.png",18,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,1);/*�S�����i�C�t(��)*/
	h					= sprite_add_res(BASE_TAMA_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= SP_BULLET;
	h->callback_mover	= move_bullet_vector/*move_bullet_angle512*/;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= anime18_pattern%18/*0*/;
	h->anim_frame		= anime16_pattern /*%16*/ /*0*/;
	#endif
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));

	BULLET_VECTOR_DATA *data;
	data				= mmalloc(sizeof(BULLET_DATA));
	h->data 			= data;
//
	/* vs REMILIA  ���~�̏ꍇ�A�e�����K */
	if (2==select_player)
	{
		angle512 += 256;
	}
	#if 1
	/*���S��*/
	mask512(angle512);
	#endif
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
}

/*---------------------------------------------------------
	�S�����i�C�t(��)	[����i�C�t]
---------------------------------------------------------*/

void enemy_follow_knife_create1(SPRITE *s, int speed256, int angle512, int height)
{
	SPRITE *h;
	h					= sprite_add_res(BASE_TAMA_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_follow_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	FOLLOW_KNIFE_DATA *data;
	data				= mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data 			= data;
	data->angle512		= (angle512);
	data->speed256		= (speed256);
	data->speed2_256	= (speed256);
	data->target		= 0;
	data->height		= height;
	data->time_out		= 300;
}
#if 0
void enemy_follow_knife_create2(SPRITE *s, int speed256, int angle512, int height)
{/*dou ble x, dou ble y*/
	SPRITE *h;
	h					= sprite_add_res(BASE_TAMA_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= SP_BULLET/*SP_LASER*/;
	h->callback_mover	= enemy_follow_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256;
	h->y256 			= s->y256/*x*/;
	FOLLOW_KNIFE_DATA *data;
	data				= mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data 			= data;
	data->angle512		= (angle512);
	data->speed256		= (speed256);
	data->speed2_256	= (speed256);
	data->target		= 0;
	data->height		= height;
	data->time_out		= 300;
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
	/*
		length	enemy-player�Ԃ̋���/��2
		r_or_l	0==�E	1==��
	*/
void enemy_even_knife_create(SPRITE *s, int speed256, int length256, int r_or_l)
{
	SPRITE *h;
	h					= sprite_add_res(BASE_TAMA_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/ 	/*h->use_alpha=0 ????*/
	h->type 			= SP_BULLET;
//
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	h->anim_frame		= 0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	EVEN_KNIFE_DATA *data;
	data=mmalloc(sizeof(EVEN_KNIFE_DATA));
	h->data=data;

//
	int tmp512;
	tmp512 = angle_jikinerai512(player,s);/*???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
	#if 1
	if (1==r_or_l)
	{
		h->callback_mover	= enemy_evenl_knife_move;
		//if (tmp512>ra d2deg512(M_PI*2*3/8))	{	tmp512 -= ra d2deg512(M_PI*2*7/8);}
		//else									{	tmp512 += ra d2deg512(M_PI*2*1/8);}
		tmp512 += (64)/*M_PI*2*1/8*/;
	}
	else
//	if (0==r_or_l)
	#endif
	{
		h->callback_mover	= enemy_evenr_knife_move;
		//if (tmp512 < -ra d2deg512(M_PI*2*3/8)) {	tmp512 =  ra d2deg512(M_PI*2*7/8)-tmp512;}
		//else									{	tmp512 -= ra d2deg512(M_PI*2*1/8);}
		tmp512 -= (64)/*M_PI*2*1/8*/;
	}
	mask512(tmp512);
	if (0==length256)	{length256=1*256;}
	data->length256 	= length256;
	data->x256			= /*s->x256+((s->w128))*/h->x256+((cos512((tmp512))*length256)>>8);
	data->y256			= /*s->y256+((s->h128))*/h->y256+((sin512((tmp512))*length256)>>8);
	data->angle512		= /*ra d2deg512*/(/*deg512_2rad*/(tmp512));
//	data->d_angle512	= ra d2deg512(speed/(M_PI*2*length));	//��angle���C��/(2�΁~length/��2)
//	int tmp_512;
//	tmp_512=speed/(/*M_PI*2*/512*length);	//��angle���C��/(2�΁~length/��2)
	data->d_angle512	= 1;//	  /*ra d2deg512*/(/*deg512_2rad*/(tmp_512));
//
	data->speed256		= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
	data->state 		= 0;
	data->wait1 		= 0;
	data->wait2 		= 0;
}
#endif

/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
---------------------------------------------------------*/

void bullet_create_n_way_dan_type(SPRITE *s, int speed256, int angle512, int bu_type,/*char *filename, int frame,*/ int n)
{
	//angle_radian�̓��W�A��
	//frame�̓t���[����-1
	angle512			= angle_jikinerai512_auto(player, s, angle512); /*ANGLE_JIKINERAI_KISUDAN???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_�� */
	int i_angle512;
	i_angle512			= angle512-((n+2)*((int)(512/48)))/*ra d2deg512((M_PI*2)/48))*/; /* 48�����e */ 	//��/24 /*0.13089969389957471827*/
	/* �O�̈׃}�X�N */
	mask512(i_angle512);
//
	int i;
	for (i=0;i<n;i++)
	{
		SPRITE *h;
	//	h					= spr ite_add_file 0("tama/bullet_ming32.png"/*"kugel.png"filename*/, 32/*(0)frame+1*/, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);
		h					= sprite_add_res(BASE_TAMA_BULLET_MING32_PNG);
		h->type 			= SP_BULLET;
		h->callback_mover	= move_bullet_vector/*move_bullet_angle512*/;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		/*h->anim_speed 	= 0;*/ /*if (frame) {	h->anim_speed=1;}*/
		h->anim_frame		= ((/*ra d2deg512(angle_radian)*/i_angle512)>>4)/*0*/;
		h->x256 			= s->x256+((s->w128-h->w128));
		h->y256 			= s->y256+((s->h128-h->h128));
		//
		BULLET_VECTOR_DATA *data;
		data				= mmalloc(sizeof(BULLET_VECTOR_DATA));
		h->data 			= data;

		i_angle512 += /*ra d2deg512*/((2)*((int)(512/48))); /*0.26179938779914943654*/
	//	if (i_angle_radian>M_PI)		{	i_angle_radian-=2*M_PI;}
	//	else if (i_angle_radian<-M_PI)	{	i_angle_radian+=2*M_PI;}
		mask512(i_angle512);
	//	data->angle512		= /*deg512_2rad*/(i_angle512);	//��/12
	//	data->speed256		= speed256;
		regist_vector(h, speed256, i_angle512);
	}
}


/*---------------------------------------------------------
	�V��p�̑�e(��)�B�����ŉ������ȓ���������B
	-------------------------------------------------------
	�������e�B1�̒e�ŃX�v���C�g��2�g���Ă���B
	�����蔻��͉����̌����Ȃ����ɂ����Ȃ��B
	-------------------------------------------------------

	ransu512	�����512�������ꂽ�P�ʌn�ŁA�w��p�ȓ��̃����_���ȓ�����������B
				�Ⴆ�� 60 ���� +30 ���� -30 �ȓ��̃����_���Ȓl���p�x�ɉ�����B
				//
				0�`60���炢�܂ŁB�傫���قǋ����s�R�ȓ����ɂȂ�
				�p�x�I��+�����֗����X���������C������=>�V����`�ԁB�C������K�v�����肻���B
	angle512	ANGLE_JIKINERAI_DAN (999) ���w�肷��ƃv���C���[�Ɍ������B��{�͊p�x�w��(�����512�x)
	ex,ey		�o���ʒu
---------------------------------------------------------*/

//void bullet_create_oodama 1(
//	SPRITE *s,
//	int xoffs256,
//	int yoffs256,
//	int speed256,
//	int angle512,
//	int ransu512
//)
void bullet_create_oodama0(SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256)
{
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	mask512(angle512);
//
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/bigkugel2.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_03_ENEMY/*PRIORITY_03_ENEMY*/,0);/* ����(��)  �����蔻�蕔��*/
//	h					= sprite_add_res(BASE_TAMA_OODAMA_00_PNG);/* ����(��)  �����蔻�蕔��*/
	h					= sprite_add_res(BASE_TAMA_OODAMA_08_PNG);/* ����(��)  �����蔻�蕔��*/
	h->priority 		= PRIORITY_03_ENEMY;
	h->type 			= SP_BULLET/*SP_BIGBULLET*/;
	h->callback_mover	= move_oodama1_oya;
//	h->flags			&= (~(SP_FLAG_VISIBLE));/*�\���Ȃ�*/
//	h->flags			|= (/*�\���Ȃ�SP_FLAG_VISIBLE|*/SP_FLAG_COLISION_CHECK);
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);/*�\�����Ȃ��Ƃ����蔻�肵�Ă���Ȃ�*/
	h->anim_frame		= 0;
	h->x256 			= s->x256-((h->w128))+xoffs256;
	h->y256 			= s->y256-((h->h128))+yoffs256;
	#if 1
	SPRITE *h2;
//	h2					= spr ite_add_file 0("tama/bigkugel1.png",1,PRIORITY_05_BULLETS/*P R_TMP*/,1);/* ���(��) �\������ */
//	h2					= sprite_add_res(BASE_TAMA_OODAMA_01_PNG);/* ���(��) �\������ */
	h2					= sprite_add_res(BASE_TAMA_OODAMA_08_PNG);/* ���(��) �\������ */
	h2->type			= SP_ETC/*S P_EN_BIGBULLET_DUMMY*/;
	h2->callback_mover	= /*NULL*/move_oodama_kodomo;
	h2->flags			&= (~(SP_FLAG_COLISION_CHECK));/*�����蔻��Ȃ�*/
	h2->flags			|= (SP_FLAG_VISIBLE/*�����蔻��Ȃ�|SP_FLAG_COLISION_CHECK*/|SP_FLAG_TIME_OVER);
	h2->anim_frame		= (((angle512>>6)&(0x06))|(1));/*((rand()&(0x06))|(1)) (1)*/
	h2->x256			= s->x256-((h2->w128))+xoffs256;
	h2->y256			= s->y256-((h2->h128))+yoffs256;
	#endif
	OODAMA_OYA_DATA *data;
	data				= mmalloc(sizeof(OODAMA_OYA_DATA));
	h->data 			= data;
	#if 1
	OODAMA_KO_DATA *h2_data;
	h2_data 			= mmalloc(sizeof(OODAMA_KO_DATA));
	h2->data			= /*NULL*/h2_data;
	h2_data->time_out	= 8;
	#endif
//
	data->angle512		= (angle512);
	data->speed256		= speed256;
	data->ransu512		= ransu512;
	data->time_out		= 16*16;//	data->wait_bg	= 10;
//
	#if 1
	data->hyouji		= h2;
	#endif
//	h2_data->sw 		= h->w;
//	h2_data->sh 		= h->h;
//	h2_data->type		= &h->type;
//	h2_data->sx256p 	= &h->x256;
//	h2_data->sy256p 	= &h->y256;
}
/*---------------------------------------------------------
	���p�̑�e(��)�B�f���ȑ�e�ł��B
	����͌����ځ��������蔻�肾�������ǁA�����g�͂��������m��Ȃ����ǁA
	���ꂶ�Ⴀ���̒e�ł������Ȃ̂�(�X�v���C�g�Q�g���Ӗ�������)
	��e�炵�������ځ��������蔻��ɒ����܂����B
	-------------------------------------------------------
	�������e�B1�̒e�ŃX�v���C�g��2�g���Ă���B
	�����蔻��͉����̌����Ȃ����ɂ����Ȃ��B
---------------------------------------------------------*/

void bullet_create_oodama2(SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256)
{
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	mask512(angle512);
//
	SPRITE *h;
//	h					= spr ite_add_file 0(/*"new_bigkugel.png"*/"bigkugel1.png"/*"bigkugel1.png"*/,1,/*PRIORITY_05_BULLETS*/PRIORITY_03_ENEMY/*PRIORITY_03_ENEMY*/,0);/* ���(��) �����蔻�蕔��*/
	/* "bigkugel1.png" ���(��) �����蔻����g���ƁA�摜�L���b�V���̏󋵂ɂ��APSP���n���O�A�b�v�B���Ă�[�������蔻��̑傫���������Ȃ�A���ʂ̒e�ł��������(��) */
//	h					= spr ite_add_file 0("tama/bigkugel2.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_03_ENEMY/*PRIORITY_03_ENEMY*/,0);/* ����(��)  �����蔻�蕔��*/
//	h					= sprite_add_res(BASE_TAMA_OODAMA_00_PNG);/* ����(��)  �����蔻�蕔��*/
	h					= sprite_add_res(BASE_TAMA_OODAMA_08_PNG);/* ����(��)  �����蔻�蕔��*/
	h->priority 		= PRIORITY_03_ENEMY;
	h->type 			= SP_BULLET/*SP_BIGBULLET*/;
	h->callback_mover	= move_oodama2_oya;
//	h->flags			&= (~(SP_FLAG_VISIBLE));/*�\���Ȃ�*/
//	h->flags			|= (/*�\���Ȃ�SP_FLAG_VISIBLE|*/SP_FLAG_COLISION_CHECK);
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);/*�\�����Ȃ��Ƃ����蔻�肵�Ă���Ȃ�*/
	h->anim_frame		= 0;
	h->x256 			= s->x256-((h->w128));
	h->y256 			= s->y256-((h->h128));
	#if 1
	SPRITE *h2;
//	h2					= spr ite_add_file 0("tama/new_bigkugel.png",1,PRIORITY_05_BULLETS/*P R_TMP*/,1);/* ���(��) �\������*/ /*"new_bigkugel.png"*/	/*"bigkugel1.png"*/
//	h2					= sprite_add_res(BASE_TAMA_OODAMA_02_PNG);/* ���(��) �\������*/	/*"new_bigkugel.png"*/	/*"bigkugel1.png"*/
	h2					= sprite_add_res(BASE_TAMA_OODAMA_08_PNG);/* ���(��) �\������*/	/*"new_bigkugel.png"*/	/*"bigkugel1.png"*/
	h2->type			= SP_ETC/*S P_EN_BIGBULLET_DUMMY*/;
	h2->callback_mover	= /*NULL*/move_oodama_kodomo;
	h2->flags			&= (~(SP_FLAG_COLISION_CHECK));/*�����蔻��Ȃ�*/
	h2->flags			|= (SP_FLAG_VISIBLE/*�����蔻��Ȃ�|SP_FLAG_COLISION_CHECK*/|SP_FLAG_TIME_OVER);
	h2->anim_frame		= (((angle512>>6)&(0x05))|(2));/*(2) ((rand()&(0x05))|(2))*/
	h2->x256			= s->x256-((h2->w128));
	h2->y256			= s->y256-((h2->h128));
	#endif
	OODAMA_OYA_DATA *data;
	data				= mmalloc(sizeof(OODAMA_OYA_DATA));
	h->data 			= data;
	#if 1
	OODAMA_KO_DATA *h2_data;
	h2_data 			= mmalloc(sizeof(OODAMA_KO_DATA));
	h2->data			= /*NULL*/h2_data;
	h2_data->time_out	= 8;
	#endif
//
	data->angle512		= (angle512);
	data->speed256		= speed256;
	data->add_speed256	= add_speed256;
//
	#if 1
	data->hyouji		= h2;
	#endif
//	h2_data->sw 		= h->w;
//	h2_data->sh 		= h->h;
//	h2_data->type		= &h->type;
//	h2_data->sx256p 	= &h->x256;
//	h2_data->sy256p 	= &h->y256;
}




/*---------------------------------------------------------
	��ޕʂɔ�����ǉ�����
---------------------------------------------------------*/

void explosion_add_type(int x256, int y256, /*dou ble int delay_wait,*/ int type)
{
	SPRITE *ex = NULL;
	EXPLOSION_DATA *data;
	data=mmalloc(sizeof(EXPLOSION_DATA));
//	if (EXPLOSION_MINI00==type) {	type = (ra_nd()%3+1);	}/* ���������ɂ���ƒx�� */
	if (0==(type&0xfb))
	{
		type |= (ra_nd()&0x03); if (0==(type & 0x03)) { 	type |= (0x01); }
	}
	switch (type & 0xff)
	{
//	case 0: break;/* ������ */
//	case 1: 	ex = spr ite_add_file 0("tr_blue.png",	6, PRIORITY_04_ITEM, 0);	data->anime_speed=1;	break;
//	case 2: 	ex = spr ite_add_file 0("tr_red.png",	6, PRIORITY_04_ITEM, 0);	data->anime_speed=1;	break;
//	case 3: 	ex = spr ite_add_file 0("tr_green.png", 6, PRIORITY_04_ITEM, 0);	data->anime_speed=1;	break;
	case 1: 	ex = sprite_add_res(BASE_TR_BLUE_PNG);		data->anime_speed=1;	break;
	case 2: 	ex = sprite_add_res(BASE_TR_RED_PNG);		data->anime_speed=1;	break;
	case 3: 	ex = sprite_add_res(BASE_TR_GREEN_PNG); 	data->anime_speed=1;	break;
//
//	case 4: break;/* �U�R���Ŕ��� */
//	case 5: 	ex = spr ite_add_file 0("bakuha05.png", 5, PRIORITY_04_ITEM, 1);	data->anime_speed=/*8*/(3)/*1*/; ex->alpha = 150;	break;
//	case 6: 	ex = spr ite_add_file 0("bakuha06.png", 5, PRIORITY_04_ITEM, 1);	data->anime_speed=/*8*/(3)/*1*/; ex->alpha = 150;	break;
//	case 7: 	ex = spr ite_add_file 0("bakuha07.png", 5, PRIORITY_04_ITEM, 1);	data->anime_speed=/*8*/(3)/*1*/; ex->alpha = 150;	break;
	case 5: 	ex = sprite_add_res(BASE_BAKUHA05_PNG); 	data->anime_speed=/*8*/(3)/*1*/;	ex->alpha = 150;	break;
	case 6: 	ex = sprite_add_res(BASE_BAKUHA06_PNG); 	data->anime_speed=/*8*/(3)/*1*/;	ex->alpha = 150;	break;
	case 7: 	ex = sprite_add_res(BASE_BAKUHA07_PNG); 	data->anime_speed=/*8*/(3)/*1*/;	ex->alpha = 150;	break;
//		/* �Ή����� */
//	case 8: 	ex = spr ite_add_file 0("ex.png", 29, PRIORITY_04_ITEM, 0); 	data->anime_speed=3/*2*/;	break;
	case 8: 	ex = sprite_add_res(BASE_BAKUHA_EX_PNG);	data->anime_speed=3/*2*/;	break;
	default:	error(ERR_WARN,"explosion_add: unknown type: %d",type);
	}
	data->anime_count	= 0;
//	data->wait_count1	= delay_wait;
	data->wait_count1	= ((type>>8) & 0xff);
//
	ex->data			= data;
	ex->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
	ex->flags			&= (~(SP_FLAG_VISIBLE));/* �\���ꎞoff */
	ex->anim_speed		= 0;
	ex->anim_frame		= 0/*-1 ???*/;
	ex->x256			= x256-(ex->w128);
	ex->y256			= y256-(ex->h128);
	ex->type			= SP_ETC;
	ex->callback_mover	= move_explosion;
}

/*---------------------------------------------------------
	�~��̈�ɍL���锚��
---------------------------------------------------------*/

void explosion_add_circle(SPRITE *src, int mode)
{
	int i;
	int j;
	j=0;
	for (i=0; i</*64*/64/*25*/; i+=8)
	{
		j += (ra_nd()&(/*32*/256-1));
		explosion_add_type(
			src->x256+((cos512(j))*i),
			src->y256+((sin512(j))*i),
		//	ra_nd()&(64-1)/*%40*/,
			EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/ | ((1==mode)?(ra_nd()&((64-1)<<8)):(0)));
	}
}

/*---------------------------------------------------------
	��`�̈�̔���(KETM)
---------------------------------------------------------*/
#if 0
void explosion_add_rect(SPRITE *src)
{
	int i;
	int j;
	for (i=0; i<100; i+=30)
	{
		for (j=0; j<100; j+=30)
		{
			explosion_add_type(src->x256+i*256,src->y256+j*256,/*(ra_nd()&(32-1)/ *%30* /),*/EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
		}
	}
//	for (i=0; i<192; i+=40)
//	{
//		for (j=0; j<192; j+=40)
//		{
//			explosion_add_type(src->x256+i*256,src->y256+j*256,/ *ra_nd()%40,* /EXPLOSION_FIRE08);
//		}
//	}
}
#endif

/*---------------------------------------------------------
	�~��̈�ɍL���锚��
---------------------------------------------------------*/


static unsigned int random_item_seed/*=0*/;

void enemy_set_random_seed(void/*int set_seed*/)
{
	random_item_seed = (ra_nd()&(8-1))/*set_seed*/;
}
int enemy_get_random_item(void)
{
	static const Uint16/*int*/ item_table[32] =
	{
		SP_ITEM_01_P008,	SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_06_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_06_TENSU,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
		SP_ITEM_06_TENSU,	SP_ITEM_06_TENSU,	SP_ITEM_06_TENSU,	SP_ITEM_00_P001,
		SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_06_TENSU,	SP_ITEM_06_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_06_TENSU,	SP_ITEM_00_P001,	SP_ITEM_06_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_06_TENSU,	SP_ITEM_00_P001,	SP_ITEM_06_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_06_TENSU,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
		SP_ITEM_06_TENSU,	SP_ITEM_06_TENSU,	SP_ITEM_06_TENSU,	SP_ITEM_00_P001,
	};
	random_item_seed++; 	random_item_seed &= 0x1f;
	return (item_table[random_item_seed]);
}




//	int put_item;	put_item=99;
//	case SP_ZAKO_03_YUKARI2:
//	if (rand_percent(30))
//	{
//		if (rand_percent(50))	{	put_item=(SP_ITEM_00_P001&0xff);}
//		else					{	put_item=(SP_ITEM_06_TENSU&0xff);}
//	}
//(put_item|SP_ITEM_00_P001)
//	if (99!=put_item)

//		static const Uint16/*int*/ item_table[8] =
//		{
//			SP_ITEM_07_SPECIAL,
//			SP_ITEM_02_BOMB,
//			SP_ITEM_02_BOMB/*�ᑬ�{���ɋz��==SP_ITEM_EXTRA_HOMING*/,
//			SP_ITEM_02_BOMB/*�ᑬ�{���ɋz��==SP_ITEM_EXTRA_HLASER*/,
//			//
//			SP_ITEM_01_P008/*�E�F�|���A�C�e��(��)==SP_ITEM_FIRE_POWER_G �E�F�|���A�C�e��(��)==SP_ITEM_EXTRA_SHIELD*/,
//			SP_ITEM_00_P001,
//			SP_ITEM_01_P008,/*�E�F�|���A�C�e��(��)==SP_ITEM_FIRE_POWER_G*/
//			SP_ITEM_06_TENSU,
//		};
//		static unsigned int drop_item=0;
//		drop_item++;
//		drop_item &= (8-1);//if (drop_item==8)	{	drop_item=0;}
/* [***20090223 �ǉ� �e�[�u���ɂ����� */
//					SP_ITEM_00_P001,		/*0x1000*/
//					SP_ITEM_02_BOMB,		/*0x1003*/
//					SP_ITEM_01_P008,		/*0x1007*/	//	SP_ITEM_00_P001/*0x1000*/,
//					SP_ITEM_00_P001,		/*0x1002*/	/*SP_ITEM_07_SPECIAL*/
//					SP_ITEM_00_P001,		/*0x1000*/
//					SP_ITEM_01_P008,		/*0x1004*/	//	SP_ITEM_01_P008/*0x1007*/,
//					//
//					SP_ITEM_02_BOMB,		/*0x1003*/
//					SP_ITEM_00_P001,		/*0x1002*/		/*SP_ITEM_07_SPECIAL*/
//					SP_ITEM_06_TENSU,		/*0x1001*/
//					SP_ITEM_03_1UP			/*0x1005*/
