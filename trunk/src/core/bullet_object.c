
/*---------------------------------------------------------
	�e���쐬 / �����쐬
	-------------------------------------------------------
	��ŕω�����e����邩���B
	�{�X��|�������ɏ�����悤�ɂ��Ă���B
	�ǉ������e
	// [***090114	��e�̓�����ς��Ă݂�
---------------------------------------------------------*/

#include "game_main.h"
#include "bullet_object.h"

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
//
	int init_vx256; 	/* �ݒ菉�����x */
	int init_vy256; 	/* �ݒ菉�����x */
} TOMARI_DAN_DATA;

/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
typedef struct
{
	int vx256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
	int vy256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
//	int angle512;	/*union BULLET_ANGLE_DATA*/
//	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	int gra256;
	int sum256;
} GRAVITY_BULLET_DATA;
#define FALL_KNIFE_DATA GRAVITY_BULLET_DATA

/*---------------------------------------------------------
	�x�N�g���W�J�e
	-------------------------------------------------------
	�w����W(���e����̑��Έʒu)�܂Ŕ䗦�œ����A
	��������̓x�N�g���e
---------------------------------------------------------*/

/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
//typedef struct
//{
//	int vx256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */	int x_sa256;	/* ���� x */
//	int vy256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */	int y_sa256;	/* ���� y */
//	int speed256;								/* ���x */	int y_sum256;	/* �A�C�e�������o���p y�� �ώZ�l(y���A������}�C�i�X) */
//} BULLET_SET_VECTOR_DATA;



/*---------------------------------------------------------
	�p�x�e(�p�x�ω��������)
	-------------------------------------------------------
	�ǂ����A�{�Ƃɂ͊p�x�e�͖������ۂ��B
	(�{�Ƃ̒e���͂��̕����ȊO�Ŏ������Ă���ۂ�)
	�p�x�ω�������̂́A���t���[�����ƁA�p�x�ω��v�Z���Ȃ����Ă���ۂ��B
---------------------------------------------------------*/


typedef struct
{
	int angle512;	/* ���p */
	int speed256;	/* ���p */
} BULLET_ANGLE_DATA;


/* ��{�e(���x�A�p�x) */

//typedef struct
//{
//	int angle512;	/*union BULLET_ANGLE_DATA*/
//	int speed256;	/*union BULLET_ANGLE_DATA*/

//} LASER_DATA;/*BULLET_ANGLE_DATA*/

/* ���p�e */

typedef struct
{
	ENEMY_BASE b;
	int angle512;
	int speed256;
	int time_out;	/* range delay */
} ENEMY_HOMING_DATA;

typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	int gra256;
	int sum256;
	int bound_counts;	/* ���˂�\��̉�(0�ŏI���) */
} HAZUMI_DAN_DATA;

typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	int time_out;	/* timer */
} MOMIJI_DATA;

typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	int angle222_512;
	int adjust_speed256;
	int state;
	int time_out;
} SMALL_RE_DATA;


typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
//	SPRITE *hyouji;
	int time_out;		/*OLD_BIG_BULLET_DATA*/ 	// int wait_bg;
	int ransu512;		/*OLD_BIG_BULLET_DATA*/
	int add_speed256;	/*NEW_BIG_BULLET_DATA*/
} OODAMA_OYA_DATA;

#if 0
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
#endif



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
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/

	int time_out;
	int speed2_256;
	int height;
	int target;
	int p_angle512;/*�\���p�p�x*/
} FOLLOW_KNIFE_DATA;

typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	int time_out;	// act


	int d_angle512;
	int length256;
	int x256;
	int y256;
	int state;
	int wait1;	// fire
//	int p_angle512;/*�\���p�p�x*/
	int l_or_r;		/* -1==l, 1==r */
} EVEN_KNIFE_DATA;


typedef struct
{
	int angle512;
	int speed256;
//
	int time_out;
	int add_angle512;
	int state;
} BOSS02_ICE_DATA;


/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	int anime_count;			/* �A�j���[�V�������x�̃J�E���^ */
	int anime_speed;			/* �A�j���[�V�������x�̐ݒ葬�x */
	int wait_count1;			/* �n�܂�܂ł̑҂����� */
} EXPLOSION_DATA;

/*---------------------------------------------------------
	�p�x�e(��{�ʈړ�)
---------------------------------------------------------*/

static void mono_angle_move(SPRITE *s, BULLET_ANGLE_DATA *d)
{
//	s->x256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//	s->y256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->x256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	s->y256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
}

/*---------------------------------------------------------
	���x�l�A�p�x�l�A���x�N�g���ʂɕϊ�����
---------------------------------------------------------*/
static void regist_vector(SPRITE *s, int speed256, int angle512)
{
	BULLET_VECTOR_DATA *data=(BULLET_VECTOR_DATA *)s->data;
//	data->vx256 	= ((cos512((angle512))*(speed256))>>8)/**fps_fa ctor*/;
//	data->vy256 	= ((sin512((angle512))*(speed256))>>8)/**fps_fa ctor*/;
	data->vx256 	= ((sin512((angle512))*(speed256))>>8)/**fps_fa ctor*/;
	data->vy256 	= ((cos512((angle512))*(speed256))>>8)/**fps_fa ctor*/;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *s)
{
	BULLET_VECTOR_DATA *data = (BULLET_VECTOR_DATA *)s->data;
	//s->anim_frame=(radtodeg(d->angle)/10)+9;	//s->anim_frame%=18;
//	mono_angle_move(s,/*(BULLET_ANGLE_DATA *)*/d);
	s->x256 += (data->vx256)/**fps_fa ctor*/;
	s->y256 += (data->vy256)/**fps_fa ctor*/;

	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;
	}
}


/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e
---------------------------------------------------------*/

#if 1
static void move_bullet_sakuya_sp1(SPRITE *s)
{
	BULLET_SP1_DATA *d = (BULLET_SP1_DATA *)s->data;
	d->timer++;
	if (d->timer > 40)
	{
		d->sum256 += d->gra256;
//		s->x256 += ((((cos512((d->angle512))*(d->speed256))>>8)*(d->sum256>>2/*/5*/))>>8) + ((cos512((d->angle2_512))*(d->sum256))>>8)/**fps_fa ctor*/;
//		s->y256 += ((((sin512((d->angle512))*(d->speed256))>>8)*(d->sum256>>2/*/5*/))>>8) + ((sin512((d->angle2_512))*(d->sum256))>>8)/**fps_fa ctor*/;
		s->x256 += ((((sin512((d->angle512))*(d->speed256))>>8)*(d->sum256>>2/*/5*/))>>8) + ((sin512((d->angle2_512))*(d->sum256))>>8)/**fps_fa ctor*/;
		s->y256 += ((((cos512((d->angle512))*(d->speed256))>>8)*(d->sum256>>2/*/5*/))>>8) + ((cos512((d->angle2_512))*(d->sum256))>>8)/**fps_fa ctor*/;
	}
	else if (d->timer==40)
	{
	//	s->yx_anim_frame = (1);
		s->type 		 = BULLET_MARU8_00_AKA+(1);
//		d->angle512 = (*(d->sd_angle512))/*(d->sd->angle512)*/+deg_360_to_512(90)*d->r_or_l;
		d->angle512 = (*(d->sd_angle512))/*(d->sd->angle512)*/-deg_360_to_512CCW(90)*d->r_or_l;
		mask512(d->angle512);
		d->angle2_512 = (*(d->sd_angle512))/*(d->sd->angle512)*/;
	}
	else
	{
//		s->x256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
//		s->y256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		s->x256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		s->y256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	}
	if ((s->x256<0/*(???)-s->x256*/)||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<0/*(???)-s->y256*/)||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;
	}
}
#endif

/*---------------------------------------------------------
	���p�̖����w�p
---------------------------------------------------------*/

static void move_bullet_sakuya_sp2(SPRITE *s)
{
	{	BULLET_SP2_DATA *d = (BULLET_SP2_DATA *)s->data;
	//	if (2==d->sd->nnn/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (2==(*(d->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
		if (1==(*(d->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��0�̎��z�u���āA�X�e�[�g��1�ɂȂ����瓮�� */
		{
			d->sum256 += d->gra256;
//			s->x256 += ((cos512((d->angle512))*(d->sum256))>>8)/**fps_fa ctor*/;
//			s->y256 += ((sin512((d->angle512))*(d->sum256))>>8)/**fps_fa ctor*/;
			s->x256 += ((sin512((d->angle512))*(d->sum256))>>8)/**fps_fa ctor*/;
			s->y256 += ((cos512((d->angle512))*(d->sum256))>>8)/**fps_fa ctor*/;
		}
	}
	if ((s->x256<0/*(???)-s->x256*/)||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<0/*(???)-s->y256*/)||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;
	}
}

/*---------------------------------------------------------
	��e�Q
	-------------------------------------------------------
	���p�̑f���ȑ�e
---------------------------------------------------------*/

static void move_bullet_oodama2_standard(SPRITE *s)
{
	OODAMA_OYA_DATA *d = (OODAMA_OYA_DATA *)s->data;
	d->speed256 += d->add_speed256;
	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
	}
//	oodama_hyouji(d->hyouji, s);
}

/*---------------------------------------------------------
	��e1
	-------------------------------------------------------
	���p�̗h�炮��e
---------------------------------------------------------*/

static void move_bullet_oodama1_aya_yuragi(SPRITE *s)
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
	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(0))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
	}
//	oodama_hyouji(d->hyouji, s);
}

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
		return (atan_512(p->y256-t->y256				 ,p->x256-t->x256				  ));//2082417					/* �����e�̏ꍇ�Ɏ��@�_��(�ȗ���) �e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ� */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_��(?) �Ȃ񂩍��ɂ���� */
//		return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128-t->w128))));/**/ 				/* ��e�̏ꍇ�Ɏ��@�_�� */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256+((p->w128-t->w128))));/**/ 				/* �����e�̏ꍇ�Ɏ��@�_�� */
	//	return (atan_512(p->y256-t->y256+((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	//	return (atan_512(p->y256-t->y256-((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	#endif
}
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512)
{
	if (ANGLE_JIKI_NERAI_DAN==angle512) 	{	angle512	= angle_jikinerai512(p,t);}
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
	//	bonus_info_score(s,SCORE_1000);/*���������֎d�l�ύXs->type = SP_DELETE;*/
		item_create(s, SP_ITEM_05_HOSI, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );s->type = SP_DELETE;
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
		//	s->yx_anim_frame = (deg_512_to_360(d->angle512)*20)/360; s->yx_anim_frame %=20;
		//	s->yx_anim_frame = (((d->angle512)*(20/2))>>8);
		//	s->yx_anim_frame = (((d->angle512)*(16/2))>>8);

			#if 0
			s->yx_anim_frame = (((d->angle512))>>(8-3));
			#else
			/*��������݊�*/
			{int aaa512;
				aaa512 = 128+ 512 - d->angle512;
				mask512(aaa512);
				s->yx_anim_frame = (((aaa512))>>(8-3));
			}
			#endif
		}
	}
	else	/*if (d->delay > 0)*/
	{
		#if 0
		/*�_��mono_angle_move(s,(BULLET_ANGLE_DATA *)d);*/
		#else
	//	s->x256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
	//	s->y256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		s->x256 += ((sin512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		s->y256 += ((cos512((d->angle512))*d->speed256)>>8)/**fps_fa ctor*/;
		#endif
	}
	/* max. reichweite erreicht? self-destruct! */
	if (d->time_out < 1 )	//if (d->range<=0)
	{
		s->type = SP_DELETE;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//
static void bullet_create_momiji_seed(SPRITE *s, int speed256, int angle512, int offset_angle512, int adjust_speed256);
static void move_bullet_momiji_oya(SPRITE *s)
{
	MOMIJI_DATA *d = (MOMIJI_DATA *)s->data;
	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	d->time_out -= 1/*fps_fa ctor*/;
	if (d->time_out < 1)
	{
		/* 5�������݂��e���쐬 */
		int jj;
		for (jj=(int)((512)-(512*2/12)); jj<(int)((512)+(512*3/12)); jj+=(int)(512*1/12) )
		{
		//	bullet_create_momiji_seed(s, t256(3), d->angle512, -(int)(512*2/12), t256(0.04));
		//	bullet_create_momiji_seed(s, t256(3), d->angle512, -(int)(512*1/12), t256(0.04));
			bullet_create_momiji_seed(s, t256(3), d->angle512, jj,				 t256(0.04));
		//	bullet_create_momiji_seed(s, t256(3), d->angle512, +(int)(512*1/12), t256(0.04));
		//	bullet_create_momiji_seed(s, t256(3), d->angle512, +(int)(512*2/12), t256(0.04));
		}
		s->type = SP_DELETE;
	}
	if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;
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
	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	d->time_out++;
//	if (200 < s->timeover_ticks)	/* ����ܗǂ��Ȃ� */
	if (200 < d->time_out)
	{
		if ((s->x256<-((s->w128+s->w128)))||(s->x256 > t256(GAME_WIDTH))||
			(s->y256<-((s->h128+s->h128)))||(s->y256 > t256(GAME_HEIGHT)))
		{
			s->type = SP_DELETE;
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
		d->vx256 = (((d->init_vx256)*d->speed256)>>8)/**fps_fa ctor*/;
		d->vy256 = (((d->init_vy256)*d->speed256)>>8)/**fps_fa ctor*/;
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
	case 2:/*����(��e�ɕϐg)*/
		d->timer--;
	//	s->anim_frame	= ((d->timer>>3)/*&0x07*/); 			/*�x��(�ϐg����̂�)*/
		s->type 		= BULLET_MARU8_00_AKA+((d->timer>>3)/*&0x07*/); 	/*�x��(�ϐg����̂�)*/
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
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
//	s->x256+=(((d->vx256)*d->speed256)>>8)/**fps_fa ctor*/;
//	s->y256+=(((d->vy256)*d->speed256)>>8)/**fps_fa ctor*/;
	s->x256+=(d->vx256)/**fps_fa ctor*/;
	s->y256+=(d->vy256)/**fps_fa ctor*/;
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
		s->type = SP_DELETE;
	}
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	�������̂ŁA�F�X��̉�www�B
	�ł��A�܂�����B
---------------------------------------------------------*/

/* 90/360�x��] */
static void rot_90_angle512(HAZUMI_DAN_DATA *d)
{
	d->angle512 	= atan_512(sin512((d->angle512)),-cos512((d->angle512)));
}
static void bullet_move_hazumi_dan(SPRITE *s)
{
	HAZUMI_DAN_DATA *d = (HAZUMI_DAN_DATA *)s->data;
	if (d->sum256 < t256(15) )		/* �ő�d�͉����x */
	{
		d->sum256 += d->gra256; 	/* �␳�l */
	}
	s->y256 += d->sum256;			/* �ώZ�d�͉����x */
	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	/* ���̕� */
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH)))
	{
		#if 1
		rot_90_angle512(d); 	/*	���˕Ԃ� */
		#else
		s->type = SP_DELETE;		/*	������ */
		#endif
	}
	/* �� */
	else if (s->y256 > t256(GAME_HEIGHT))
	{
		/* �ݒ蒵�˂�� */
		if (0!=d->bound_counts)
		{
			d->bound_counts--;	/* ���˂�񐔂����炷 */
		//	s->anim_frame	= 4+(d->bound_counts);		// [***090116		�኱�ύX
			s->type 		= BULLET_MARU8_00_AKA+4+(d->bound_counts);		// [***090116		�኱�ύX
			d->sum256		= -d->sum256;		/* �d�͉����x */
			d->speed256 	= -d->speed256; 	/* ���x */
			#if 1
			d->speed256 	+= d->speed256; 	/* ����(���ēV��֓�����) */
			#endif
			rot_90_angle512(d); 		// [***090116		�ǉ�
		}
		else
		{
			s->type = SP_DELETE;		/*	������ */
		}
	}
	#if 1
	/* �V�� */
	else if (s->y256 < 0)
	{
		s->type = SP_DELETE;		/*	������ */
	}
	#endif
}

/*---------------------------------------------------------
	�d�͒e�̈ړ�
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *s)
{
	GRAVITY_BULLET_DATA *data = (GRAVITY_BULLET_DATA *)s->data;
	data->sum256 += data->gra256;
	s->y256 += data->sum256;
	#if 0
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	s->x256 += (data->vx256)/**fps_fa ctor*/;
	s->y256 += (data->vy256)/**fps_fa ctor*/;

	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(-64/*-50*/))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;
	}
	#else
	move_bullet_vector(s);
	#endif
}

/*---------------------------------------------------------
	�����~���i�C�t(��)
---------------------------------------------------------*/

static void enemy_fall_knife_move(SPRITE *s)
{
	FALL_KNIFE_DATA *data = (FALL_KNIFE_DATA *)s->data;
//
	if (((data->vy256))+data->sum256 <= t256(3) )
	{	data->sum256 += data->gra256;}
	s->y256 += data->sum256;
	#if 0
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	s->x256 += (data->vx256)/**fps_fa ctor*/;
	s->y256 += (data->vy256)/**fps_fa ctor*/;
	if ((s->x256 < t256(0))||(s->x256 > t256(GAME_WIDTH))||
		(s->y256 < t256(-64/*-100*/))||(s->y256 > t256(GAME_HEIGHT)))
	{
		s->type = SP_DELETE;
	}
	#else
	move_bullet_vector(s);
	#endif
}
//	tmp256 = ((sin512((d->angle512))*(d->speed256))>>8)/**fps_fa ctor*/;
//	tmp256 = ((cos512((d->angle512))*(d->speed256))>>8)/**fps_fa ctor*/;

//	s->x256 += ((cos512((d->angle512))*(d->speed256))>>8)/**fps_fa ctor*/;
//	s->x256 += ((sin512((d->angle512))*(d->speed256))>>8)/**fps_fa ctor*/;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_follow_knife_move(SPRITE *s)
{
	FOLLOW_KNIFE_DATA *d = (FOLLOW_KNIFE_DATA *)s->data;
	if (0==d->target)
	{
		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
		d->speed256 -= t256(0.02);
	//	if (s->anim_frame==17)	{	s->anim_frame = 0;}
	//	else					{	s->anim_frame++;}
		d->p_angle512 += 32;
		mask512(d->p_angle512);/*�˂�̂���*/
		if (d->speed256 <= t256(0) )
		{
			d->angle512 = atan_512(
				(player->y256)-(s->y256)+(player->h128)+(d->height*256),
				(player->x256)-(s->x256)-(player->w128)  );
			d->target=1;
			s->type 			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		//	s->anim_frame=/*17-*/(deg_512_to_360(d->angle512)/20)%18;
		//	s->anim_frame=/*17-*/(deg_512_to_360(d->angle512)/20)%16;
			mask512(d->angle512);/*�˂�̂���*/
		//	s->anim_frame=/*17-*/((d->angle512)>>5);
			d->p_angle512=/*17-*/((d->angle512));
		}
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
		//	s->m_angleCCW512		= tmp512;
		//	int aaa512; 	aaa512 = (/*127*/255-d->p_angle512);	mask512((aaa512));
//			int aaa512; 	aaa512 = (/*127*/127-d->p_angle512);	mask512((aaa512));
//			int aaa512; 	aaa512 = (d->p_angle512);	mask512((aaa512));
//			s->m_angleCCW512		= aaa512;
			s->m_angleCCW512		= (d->p_angle512);
			#endif
	}
	else
//	if (d->target)
	{
//		s->x256 += ((cos512((d->angle512))*(d->speed2_256))>>8)*2/**fps_fa ctor*/;
//		s->y256 += ((sin512((d->angle512))*(d->speed2_256))>>8)*2/**fps_fa ctor*/;
		s->x256 += ((sin512((d->angle512))*(d->speed2_256))>>8)*2/**fps_fa ctor*/;
		s->y256 += ((cos512((d->angle512))*(d->speed2_256))>>8)*2/**fps_fa ctor*/;
	}
	d->time_out--;
	if (d->time_out<0)
	{
		if (((s->x256+((s->w128+s->w128)) < t256(0))||(s->x256 > t256(GAME_WIDTH))||
			 (s->y256+((s->h128+s->h128)) < t256(0))||(s->y256 > t256(GAME_HEIGHT))))
		{
			s->type = SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
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

//static void enemy_evenr_knife_move(SPRITE *s)
static void enemy_even_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d = (EVEN_KNIFE_DATA *)s->data;
	switch (d->state)
	{
	case 0:
		if (100 < d->wait1)
		{
			d->wait1 = 0;
			d->state = 1;
		}
		d->angle512 -= (d->d_angle512*d->l_or_r) /**fps_fa ctor*/;
		mask512(d->angle512);
		{
			int tmpCCW512;
			tmpCCW512 = (d->angle512+((int)(512*1/4)*d->l_or_r));
			mask512(tmpCCW512);/*�K���v��*/
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			s->m_angleCCW512		= (tmpCCW512);
			#endif
		}
	//	s->x256 		= d->x256-((cos512((d->angle512))*d->length256)>>8)-((s->w128));
	//	s->y256 		= d->y256-((sin512((d->angle512))*d->length256)>>8)-((s->h128));
		s->x256 		= d->x256-((sin512((d->angle512))*d->length256)>>8)-((s->w128));
		s->y256 		= d->y256-((cos512((d->angle512))*d->length256)>>8)-((s->h128));
		break;

	case 1:
		if (50 < d->wait1)
		{
			d->wait1 = 0;
			d->state = 0;
		}
		//#if 1
		//d->speed=20/*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		//#endif
		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
		break;
	}
	if (10 < d->wait1)
	{
		d->wait1 = 0;
		#if 1
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.5)/*t256(5.0)*/),
			(d->angle512+((int)(512*3/12)*d->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.2)/*t256(4.0)*/),
			(d->angle512+((int)(512*4/12)*d->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		bullet_create_n_way_dan_sa_type(s,
			(t256(2.0)/*t256(3.0)*/),
			(d->angle512+((int)(512*5/12)*d->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		if (difficulty>1)
		{
		bullet_create_n_way_dan_sa_type(s,
			(t256(1.8)/*t256(2.0)*/),
			(d->angle512+((int)(512*6/12)*d->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		}
		#else
		bullet_create_sakuya_knife(s, t256(5.0), d->angle512+(int)(512*3/12), d->p_angle512); //+M_PI/2
		bullet_create_sakuya_knife(s, t256(4.0), d->angle512+(int)(512*4/12), d->p_angle512);
		bullet_create_sakuya_knife(s, t256(3.0), d->angle512+(int)(512*5/12), d->p_angle512);
		if (difficulty>1)
		{	bullet_create_sakuya_knife(s, t256(2.0), d->angle512+(int)(512*6/12), d->p_angle512);}
		#endif
	}
	d->wait1++;
	d->time_out++;
	if ( (150+difficulty*50) < d->time_out )
	{
		s->type = SP_DELETE;
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss02_ice_move(SPRITE *s)
{
	BOSS02_ICE_DATA *data = (BOSS02_ICE_DATA *)s->data;
	switch (data->state)
	{
	case 0: 	/* �~��^�� */
		data->time_out -= 1/*fps_fa ctor*/;
		if (data->time_out <= 0)
		{	data->state++/* = 1*/;}
		data->angle512 += data->add_angle512/**fps_fa ctor*/;
		break;
	case 1: 	/* �o���o���ړ� */
		{
			#if 0
			const short sss[4] =
			{
				t256(2.5),		/* easy:   2 [�������ȒP�ɕύX]  2 ���ꂪ������Ȃ����Ď��͖����Ǝv���P�h */
				t256(3.0),		/* normal: 4 [�������ȒP�ɕύX]  3 */
				t256(3.5),		/* hard:   6 [�������ȒP�ɕύX]  5 ���������Ă悭����Δ�����܂� */
				t256(4.0)		/* luna:  12 [�������ȒP�ɕύX] 10 ���������Ă悭����Δ�����܂����A���܂Ɏ��s����www */
			};
			data->speed256 = sss[difficulty]/*t256(12)*/;
			#else
		//	data->speed256 = t256(2.5)+(difficulty<<7); 	/* (difficulty x 128) */
			data->speed256 = t256(2.5)+(difficulty<<6); 	/* (difficulty x 64) */ 	/* �e���������ē���Ȃ����̂Œx������ */
			#endif
		}
//		data->angle512	= (ra_nd()&(128-1)/*%90*/)+deg_360_to_512(45);
		data->angle512	= (ra_nd()&(128-1)/*%90*/)-deg_360_to_512CCW(45);
		data->state++/* = 2*/;
		break;
//	case 2: 	/* �������Ȃ� */
//		break;
	}
//	if (d->angle512 < deg_360_to_512(0))	{	d->angle512 += deg_360_to_512(360);}
//	if (d->angle512 > deg_360_to_512(360))	{	d->angle512 -= deg_360_to_512(360);}
	mask512(data->angle512);

	/*�ȉ�rwingx.c�Ɠ���*/
//	s->x256+=((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
//	s->y256+=((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	s->x256+=((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
#if 0
//	s->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
//	s->anim_frame = ((((data->angle512)&(512-1))*(32/2))>>8);
	s->anim_frame = 0;//((((data->angle512)&(512-1)))>>4);
#endif
	/*(�����܂�)[�ȉ�rwingx.c�Ɠ���]*/
//
			#if 1
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
		//	s->m_angleCCW512		= data->angle512;
		//	int aaa512; 	aaa512 = (/*127*/255-data->angle512);	mask512((aaa512));
//			int aaa512; 	aaa512 = (/*127*/127-data->angle512);	mask512((aaa512));
			int aaa512; 	aaa512 = (data->angle512);	mask512((aaa512));
			s->m_angleCCW512		= aaa512;
			#endif
	if ((s->x256 < t256(-100))||(s->x256 > t256(100)+t256(GAME_WIDTH))||
		(s->y256 < t256(-100))||(s->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{
		s->type = SP_DELETE;
	}
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
			s->anim_frame++;
			if (s->anim_frame > (s->yx_frames&YX_FRAME_LOW_X) )
			{
				s->type = SP_DELETE;
			}
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
//	h					= sprite_add_bullet(TAMA_TYPE_KUGEL_PNG);
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);
	h->type 			= BULLET_MARU8_00_AKA+(0);/*SP_BULLET*/
	h->callback_mover	= move_bullet_vector;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= (0);
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	BULLET_VECTOR_DATA *data;
	data				= mmalloc(sizeof(BULLET_VECTOR_DATA));
	h->data 			= data;
//
//	data->angle512		= angle_jikinerai512(player,s);
//	data->speed256		= speed256;
	int i_angle512;
	i_angle512		= angle_jikinerai512(player,s);
	regist_vector(h, speed256, i_angle512);
}


/*---------------------------------------------------------
	�o���ʒu�I�t�Z�b�g�e���쐬����
	�j�e���쐬����
---------------------------------------------------------*/

//void bu llet_create_hari_dan180(SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256)
void bullet_create_offset_dan_type(SPRITE *s, int speed256, int angle512, int x_offset256, int y_offset256, int bullet_obj_type )
{
	angle512			= angle_jikinerai512_auto(player, s, angle512);
//
	SPRITE *h;
//	h					= spr ite_add_file 0(/*"bs hoot.png"*/"tama/bullet_beam16.png",16/*1*/,PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY_WEAPON*/ /*PRIORITY_03_ENEMY*/, 0);
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_DUMMY/*BEAM16_PNG*/);
	h->type 			= (bullet_obj_type);/*BULLET_HARI32_00_AOI*/ /*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= move_bullet_vector;/*enemy_laser_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//((angle512>>4)&0x0f);/*180�x*/	//b->anim_frame = 0;
	h->x256 			= s->x256+((s->w128-h->w128)) + x_offset256;
	h->y256 			= s->y256+((s->h128-h->h128)) + y_offset256;
	{	/*LASER_DATA*/BULLET_VECTOR_DATA *data;
		data			= mmalloc(sizeof(/*LASER_DATA*/BULLET_VECTOR_DATA));
		h->data 		= data;
		mask512(angle512);/*�O�̈�*/
	//	data->angle512 = /*deg512_2rad*/(angle512);
	//	data->speed256 = (speed256);
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
	//	h->m_angleCCW512		= angle512;
	//	int aaa512; 	aaa512 = (/*127*/255-angle512); mask512((aaa512));
//		int aaa512; 	aaa512 = (/*127*/127-angle512); mask512((aaa512));
		int aaa512; 	aaa512 = (angle512); mask512((aaa512));
		h->m_angleCCW512		= aaa512;
		#endif
		regist_vector(h, speed256, angle512);
	}
}

/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

void bullet_create_enemy_homing(SPRITE *s)
{
	SPRITE *r;
//	r					= sprite_add_res(BASE_HOMING16_PNG);	/*�f�ޕύX*/ /*20"rotating_rocket.png"*/
//	r->type 			= SP_ZAKO;/*SP_ZAKO*/ /*SP_BULLET*/ /*SP_ENEMY_HOMING*/
	r					= sprite_add_bullet(TAMA_TYPE_HOMING16_PNG);	/*�f�ޕύX*/ /*20"rotating_rocket.png"*/
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
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);
	h->type 			= BULLET_MARU8_00_AKA+(set_frame);/*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= move_bullet_vector;/*enemy_laser_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(set_frame);
	h->x256 			= (s->x256)+((s->w128-h->w128));
	h->y256 			= (s->y256)+((s->h128-h->h128));
	BULLET_VECTOR_DATA *data;
	data				= NULL;
	data				= mmalloc(sizeof(BULLET_VECTOR_DATA));
	h->data 			= data;
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
}

/*---------------------------------------------------------
	�����e
---------------------------------------------------------*/

void bullet_create_momiji_dan(SPRITE *s, int speed256, int angle512)
{	/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/sp ell_bullet_r.png", 1, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);
	h->type 			= BULLET_MARU8_00_AKA+(1);/*1==�Ƃ肠����*/ /*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= move_bullet_momiji_oya;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(1);/*1==�Ƃ肠����*/
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

static void bullet_create_momiji_seed(SPRITE *s, int speed256, int angle512, int offset_angle512, int adjust_speed256)
{
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/kugel2.png", 1, /*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
//	h					= spr ite_add_res(BASE_TAMA_KUGEL_MINI2_PNG);
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);
	h->type 			= BULLET_MINI8_00_AKA+((angle512>>6)&7); /*BULLET_MINI8_00_AKA*/ /*SP_BULLET*/
	h->callback_mover	= enemy_smallbullet_re_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(8+(angle512>>6)) /*8*/ /*0*/;
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
	data->time_out		= 0;
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/

// [***090114	�ǉ�
void bullet_create_hazumi_dan(SPRITE *s, int speed256, int angle512, int delta256, int bound_counts)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot2.png",3,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);/*�Ή��e*/
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);/*�Ή��e*/
	h->type 			= BULLET_MARU8_00_AKA+(4+bound_counts);/*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_hazumi_dan;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(4+bound_counts);/*�Ƃ肠����*/
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
	data->bound_counts	= bound_counts;
}

/*---------------------------------------------------------
	�d�͒e�B�A���X�̐l�`�����e�B
	-------------------------------------------------------
	�������ɉ������Ă������e�B
	�t���[�����Ōv�Z���Ă���̂ł��Ȃ�d�͉����x�����������Ȃ��Ƒ�ςȂ��ƂɁB
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/

void bullet_create_jyuryoku_dan(SPRITE *s, int speed256, int angle512, int delta256)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);/*�e*/
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);/*�e*/
	h->type 			= BULLET_MARU8_00_AKA+(7);/*BULLET_MARU8_07_AOI*/  /*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_gravity;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(7)/*0*/;/*�Ƃ肠����*/
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	GRAVITY_BULLET_DATA *data;
	data				= mmalloc(sizeof(GRAVITY_BULLET_DATA));
	h->data 			= data;
//
	angle512			= angle_jikinerai512_auto(player, s, angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);

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
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);		h->priority 	= PRIORITY_03_ENEMY;
	unsigned int aaa = ((ra_nd()&(4-1))+4);
	h->type 			= BULLET_MARU8_00_AKA+(aaa);/*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= move_bullet_sakuya_sp1;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;// /*4*/(aaa)/*0*/;/*�Ƃ肠����*/
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
	data->speed256		= (speed256);
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
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);	h->priority 	= PRIORITY_03_ENEMY;
	h->type 			= BULLET_MARU8_00_AKA+(0);/*SP_BULLET*/
	h->callback_mover	= move_bullet_sakuya_sp2;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(0);
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
	��邪�΂�܂��A�~�܂�e�B��e�ɕω�����
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[������speed���ǂꂾ���������邩
---------------------------------------------------------*/
//id bullet_create_tomari_dan( SPRITE *s, int speed256, int angle512, int delta256, int next_angle512)
void bullet_create_tomari2_dan(SPRITE *s, int speed256, int angle512, int delta256, int next_angle512)
{
	SPRITE *h;
//	h					= sprite_add_bullet(TAMA_TYPE_KUGEL_PNG);/*���Ԓe*/
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_MARU16_PNG);		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h->type 			= BULLET_MARU8_00_AKA+(0);/*SP_BULLET*/
	h->callback_mover	= enemy_stop_bullet2_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(0);
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
	data->init_vx256	= data->vx256;
	data->init_vy256	= data->vy256;
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
	�����~���i�C�t(��)
---------------------------------------------------------*/

void bullet_create_sakuya_no_rot_knife(SPRITE *s, int speed256, int angle512, int gra256)
{
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/knife.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,1);/*�����~���i�C�t(��)*/
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_KNIFE01_PNG);/*�����~���i�C�t(��)*/
	h->type 			= BULLET_KNIFE20_05_AKA;/*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= enemy_fall_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	FALL_KNIFE_DATA *data;
	data				= mmalloc(sizeof(FALL_KNIFE_DATA));
	h->data 			= data;
	mask512(angle512);

//	data->vx256 = ((sin512((angle512))*(speed256))>>8)/**fps_fa ctor*/;
//	data->vy256 = ((cos512((angle512))*(speed256))>>8)/**fps_fa ctor*/;
	regist_vector(h, speed256, angle512);
//	data->angle512		= (angle512);
//	data->speed256		= speed256;
	data->gra256		= (gra256);
	data->sum256		= t256(0);
}


/*---------------------------------------------------------
	�S�����i�C�t(��)	[����i�C�t]
	�_���i�C�t(��)
---------------------------------------------------------*/

void bullet_create_sakuya_follow_knife1(SPRITE *s, int speed256, int angle512, int height)
{
	SPRITE *h;
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= /*BULLET_KNIFE20_07_MIDORI*/BULLET_KNIFE20_04_AOI/*BULLET_KNIFE20_04_AOI*/;	/* (���΃i�C�t�ɕϐg) */		/*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= enemy_follow_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//0;
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

/*---------------------------------------------------------
	�S�����i�C�t(��)	[����i�C�t]
---------------------------------------------------------*/

#if 0
void bullet_create_sakuya_follow_knife2(SPRITE *s, int speed256, int angle512, int height)
{/* x, y*/
	SPRITE *h;
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= BULLET_KNIFE20_04_AOI;/*SP_BULLET*/ /*SP_LASER*/
	h->callback_mover	= enemy_follow_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//0;
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
void bullet_create_sakuya_even_knife(SPRITE *s, int speed256, int length256, int r_or_l)
{
	SPRITE *h;
	h					= sprite_add_bullet(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/	/*h->use_alpha=0 ????*/
	h->type 			= BULLET_KNIFE20_04_AOI;/*SP_BULLET*/ /*SP_LASER*/
//
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//0;
	h->x256 			= s->x256+((s->w128-h->w128));
	h->y256 			= s->y256+((s->h128-h->h128));
	EVEN_KNIFE_DATA *data;
	data				= mmalloc(sizeof(EVEN_KNIFE_DATA));
	h->data 			= data;
//
	int tmp512;
	tmp512 = angle_jikinerai512(player,s);/*???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
	h->callback_mover	= enemy_even_knife_move;
	#if 1
//	if (1==r_or_l)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0!=r_or_l)
	{
//		h->callback_mover	= enemy_evenl_knife_move;
		tmp512 += (64)/*M_PI*2*1/8*/;
		data->l_or_r			= -1;
	}
	else
//	if (0==r_or_l)
	#endif
	{
//		h->callback_mover	= enemy_evenr_knife_move;
		tmp512 -= (64)/*M_PI*2*1/8*/;
		data->l_or_r			= 1;
	}
	mask512(tmp512);
	if (0==length256)	{length256=1*256;}
	data->length256 	= length256;
//	data->x256			= /*s->x256+((s->w128))*/h->x256+((cos512((tmp512))*length256)>>8);
//	data->y256			= /*s->y256+((s->h128))*/h->y256+((sin512((tmp512))*length256)>>8);
	data->x256			= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512))*length256)>>8);
	data->y256			= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512))*length256)>>8);
	data->angle512		= /*ra d2deg512*/(/*deg512_2rad*/(tmp512));
//	data->d_angle512	= ra d2deg512(speed/(M_PI*2*length));	//��angle���C��/(2�΁~length/��2)
//	int tmp_512;
//	tmp_512=speed/(/*M_PI*2*/512*length);	//��angle���C��/(2�΁~length/��2)
	data->d_angle512	= 1;//	  /*ra d2deg512*/(/*deg512_2rad*/(tmp_512));
//
	data->speed256		= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
	data->state 		= 0;
	data->wait1 		= 0;
	data->time_out		= 0;
}
#endif

/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/
void bullet_create_n_way_dan_sa_type(
	SPRITE *s,				/*	*/
	int speed256,			/* �e�� */
	int angle512,			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
	int div_angle512,		/* 1����n���������p�x */
	int bullet_obj_type,	/* �e�O�� */
	int n_way				/* ���e�� */
)
	/*
		�悭�l������AANGLE_JIKI_NERAI_KISUDAN �͕K�v�Ȃ��B
		ANGLE_JIKINERAI_DAN ��������Ηǂ��A
		���e���ɋ������w�肷��΋����e�B
		���e���Ɋ���w�肷��Ί�e�B
		������܂������ǁA�C���t���Ȃ�����ww�B
	*/
{
	angle512			= angle_jikinerai512_auto(player, s, angle512); /*ANGLE_JIKI_NERAI_DAN???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_�� */
	int i_angle512;
	i_angle512			= angle512-((n_way+2)*((div_angle512>>1)/*(int)(512/48)*/))/*ra d2deg512((M_PI*2)/48))*/; /* 48�����e */	// /* ��/24 0.13089969389957471827 */
	/* �O�̈׃}�X�N */
	mask512(i_angle512);
//
	int i;
	for (i=0; i<n_way; i++)
	{
		SPRITE *h;
		// frame�̓t���[����-1 /*char *filename, int frame,*/
	//	h					= spr ite_add_file 0("tama/bullet_ming32.png"/*"kugel.png"filename*/, 32/*(0)frame+1*/, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);
		h					= sprite_add_bullet(TAMA_TYPE_BULLET_DUMMY/*_MING32_PNG*/);
		h->type 			= (bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*SP_BULLET*/
		h->callback_mover	= move_bullet_vector;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		/*h->anim_speed 	= 0;*/ /*if (frame) {	h->anim_speed=1;}*/
//		h->anim_frame		= 0;//((i_angle512)>>4)/*0*/;
		h->x256 			= s->x256+((s->w128-h->w128));
		h->y256 			= s->y256+((s->h128-h->h128));
		BULLET_VECTOR_DATA *data;
		data				= mmalloc(sizeof(BULLET_VECTOR_DATA));
		h->data 			= data;
		i_angle512			+= div_angle512;	/*(2)*((int)(512/48))*/ /* ��/12 0.26179938779914943654 */
		mask512(i_angle512);
	//	data->angle512		= /*deg512_2rad*/(i_angle512);	//
	//	data->speed256		= speed256;
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
	//	h->m_angleCCW512		= i_angle512;
	//	int aaa512; 	aaa512 = (/*127*/255-i_angle512);	mask512((aaa512));
//		int aaa512; 	aaa512 = (/*127*/127-i_angle512);	mask512((aaa512));
		int aaa512; 	aaa512 = (i_angle512);	mask512((aaa512));
		h->m_angleCCW512		= aaa512;
		#endif
		regist_vector(h, speed256, i_angle512);
	}
}


/*---------------------------------------------------------
	�V��p�̑�e(��)�B�����ŉ������ȓ���������B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
	ransu512	�����512�������ꂽ�P�ʌn�ŁA�w��p�ȓ��̃����_���ȓ�����������B
				�Ⴆ�� 60 ���� +30 ���� -30 �ȓ��̃����_���Ȓl���p�x�ɉ�����B
				//
				0�`60���炢�܂ŁB�傫���قǋ����s�R�ȓ����ɂȂ�
				�p�x�I��+�����֗����X���������C������=>�V����`�ԁB�C������K�v�����肻���B
	angle512	ANGLE_JIKINERAI_DAN (999) ���w�肷��ƃv���C���[�Ɍ������B��{�͊p�x�w��(�����512�x)
	ex,ey		�o���ʒu
---------------------------------------------------------*/

void bullet_create_oodama0(SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256)
{
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	mask512(angle512);
//
	SPRITE *h;
	h					= sprite_add_bullet(TAMA_TYPE_OODAMA_08_PNG);	 /* ��e(��) �\������*/
	h->priority 		= PRIORITY_03_ENEMY;
	unsigned int aaa = (((angle512>>6)&(0x06))|(1));
	h->type 			= BULLET_OODAMA32_00_SIROI+(aaa);/*SP_BULLET*/ /*SP_BIGBULLET*/
	h->callback_mover	= move_bullet_oodama1_aya_yuragi;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(aaa);/*((rand()&(0x06))|(1)) (1)*/
	h->x256 			= s->x256-((h->w128))+xoffs256;
	h->y256 			= s->y256-((h->h128))+yoffs256;
	OODAMA_OYA_DATA *data;
	data				= mmalloc(sizeof(OODAMA_OYA_DATA));
	h->data 			= data;
//
	data->angle512		= (angle512);
	data->speed256		= speed256;
	data->ransu512		= ransu512;
	data->time_out		= 16*16;//	data->wait_bg	= 10;
}

/*---------------------------------------------------------
	���p�̑�e(��)�B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
---------------------------------------------------------*/

void bullet_create_oodama2(SPRITE *s, int speed256, int angle512, int ransu512, int add_speed256, int xoffs256, int yoffs256)
{
	angle512			= angle_jikinerai512_auto(player, s, angle512);
	mask512(angle512);
//
	SPRITE *h;
	h					= sprite_add_bullet(TAMA_TYPE_OODAMA_08_PNG);	 /* ��e(��) �\������*/
	h->priority 		= PRIORITY_03_ENEMY;
	unsigned int aaa= (((angle512>>6)&(0x05))|(2));
	h->type 			= BULLET_OODAMA32_00_SIROI+(aaa);/*SP_BULLET*/ /*SP_BIGBULLET*/
	h->callback_mover	= move_bullet_oodama2_standard;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(aaa);/*(2) ((rand()&(0x05))|(2))*/
	h->x256 			= s->x256-((h->w128));
	h->y256 			= s->y256-((h->h128));
	OODAMA_OYA_DATA *data;
	data				= mmalloc(sizeof(OODAMA_OYA_DATA));
	h->data 			= data;
//
	data->angle512		= (angle512);
	data->speed256		= speed256;
	data->add_speed256	= add_speed256;
}

/*---------------------------------------------------------
	�V��p
	ICE�e
	-------------------------------------------------------
---------------------------------------------------------*/

/*static*/ void bullet_create_aya_ice(SPRITE *s)
{
	int i;
	int j;
	int rand_add_angle;
	rand_add_angle=(ra_nd()&(8-1));
	for (j=0; j</*2*/(1+difficulty); j++)
	{
		for (i=0; i<30/*ICE_NUM_OF_ENEMIES*/; i++)
		{
			SPRITE		*h;
		//	h					= spr ite_add_file 0("tama/jippou32.png",32,PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY_WEAPON*/, 0);/*36"boss02_w.png"*/
		//	h					= sprite_add_res(BASE_TAMA_BULLET_JIPPOU32_PNG);
			h					= sprite_add_bullet(TAMA_TYPE_BULLET_JIPPOU32_PNG);
			h->type 			= BULLET_CAP16_04_SIROI;/*SP_BULLET*/ /*SP_BOSS02ICE*/
			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
			h->callback_mover	= enemy_boss02_ice_move;

		//	h->x256 			= s->x256 + ((0==j)?(t256(-19)):(t256(81)));
		//	h->x256 			= s->x256 + ((0==j)?(t256(-64)):(t256(64)));
		{	s16 aaa[4] 			=	{t256(-64),t256(64),t256(-32),t256(32)};
			h->x256 			= s->x256 + aaa[(j)];
		}	h->y256 			= s->y256 /*+ t256(135 138)*/;

			BOSS02_ICE_DATA *data;
			data				= mmalloc(sizeof(BOSS02_ICE_DATA));
			h->data 			= data;
		//	data->add_angle512	= (j == 0)?(deg_360_to_512(+5)):(deg_360_to_512(-5));
			data->add_angle512	= rand_add_angle-j;
			data->angle512		= (i<<4)/*deg_360_to_512(90)*/;
			data->state 		= 0;
			data->time_out		= 120/*100*/;
			data->speed256		= t256(2.5)+(difficulty<<6);/*t256(4.0);*/	/* (difficulty x 64) */
		}
		rand_add_angle = -rand_add_angle;
	}
}



/*---------------------------------------------------------
	��ޕʂɔ�����ǉ�����
---------------------------------------------------------*/

void explosion_add_type(int x256, int y256, /*int delay_wait,*/ int type)
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
	default:	error(ERR_WARN, "explosion_add: unknown type: %d",type);
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
	ex->type			= SP_MUTEKI;
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
		//	src->x256+((cos512(j))*i),
		//	src->y256+((sin512(j))*i),
			src->x256+((sin512(j))*i),
			src->y256+((cos512(j))*i),
		//	ra_nd()&(64-1)/*%40*/,
		//	EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/ | ((1==mode)?(ra_nd()&((64-1)<<8)):(0)));	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		//	EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/ | ((0!=mode)?(ra_nd()&((64-1)<<8)):(0)));	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/ | ((0==mode)?(0):(ra_nd()&((64-1)<<8))));	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
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
	�����_���A�C�e���G�̏ꍇ�Ɏg���A���ʔ����e�[�u��
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
