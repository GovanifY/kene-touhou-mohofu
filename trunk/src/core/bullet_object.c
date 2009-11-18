
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

/* ���p�e(���x�ω�����) */
typedef struct
{
	int speed256;								/* ���x */
	int delta256;								/* �����x */
//
	int init_vx256; 	/* �ݒ菉�����x */
	int init_vy256; 	/* �ݒ菉�����x */
//
	//int next_angle512;/*ST2*/ /* tmp_angleCCW512 */
	//int state;
} TOMARI_DAN_DATA;
	//int time_out; 	/* timer */


/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
typedef struct
{
//	int speed256;			/* sum256 */		/* ���x */
	int delta256;			/* gra256 */		/* �����x(y�������̂�) */
} GRAVITY_BULLET_DATA;


typedef struct
{
//	int speed256;			/* sum256 */		/* ���x */
	int delta256;			/* gra256 */		/* �����x(y�������̂�) */
	int bound_counts;	/* ���˂�\��̉�(0�ŏI���) */
} HAZUMI_DAN_DATA;

/*---------------------------------------------------------
	�x�N�g���W�J�e
	-------------------------------------------------------
	�w����W(���e����̑��Έʒu)�܂Ŕ䗦�œ����A
	��������̓x�N�g���e
---------------------------------------------------------*/



/*---------------------------------------------------------
	�p�x�e(�p�x�ω��������)
	-------------------------------------------------------
	�ǂ����A�{�Ƃɂ͊p�x�e�͖������ۂ��B
	(�{�Ƃ̒e���͂��̕����ȊO�Ŏ������Ă���ۂ�)
	�p�x�ω�������̂́A���t���[�����ƁA�p�x�ω��v�Z���Ȃ����Ă���ۂ��B
---------------------------------------------------------*/

/* ���p�e */

typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	int angle222_512;
	int adjust_speed256;
//	int state;
	//int time_out;
} MOMIJI_KODOMO_DATA;

typedef struct
{
//	int aaa_angle512;	/*union BULLET_ANGLE_DATA*/
	int aaa_speed256;	/*union BULLET_ANGLE_DATA*/
//
	//int time_out;
	int speed_2nd_256;
	int height;
	int target;
//	int p_angle512;/*�\���p�p�x*/
} FOLLOW_KNIFE_DATA;

typedef struct
{
	int angle512;	/*union BULLET_ANGLE_DATA*/
	int speed256;	/*union BULLET_ANGLE_DATA*/
//
	//int time_out; // act

	int d_angle512;
//	int length256;	t256(128)
	int x256;
	int y256;
	int state;
	int wait1;	// fire
//	int p_angle512;/*�\���p�p�x*/
	int l_or_r; 	/* -1==l, 1==r */
} EVEN_KNIFE_DATA;


/* �����e ���� */
typedef struct
{
//	int angle512;
	int state;
	int speed256;
//
	//int time_out;
//	int add_angle512;
	int next_2nd_state;/*������Ǝ���*/
} BOSS02_ICE_DATA;

//typedef struct
//{
//	//int time_out; 	/* timer */
//} MOMIJI_DATA;


typedef struct
{
//	SPRITE *hyouji;
	//int time_out; 	/*OLD_BIG_BULLET_DATA*/ 	// int wait_bg;
	int ransu512;		/*OLD_BIG_BULLET_DATA*/
	int add_speed256;	/*NEW_BIG_BULLET_DATA*/
} OODAMA_OYA_DATA;

#if 0
typedef struct
{
	//int time_out;
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
	int angle512;
	int y_sum256;
	int player_x256;
	int player_y256;
} BULLET_SP1_DATA;
//	int x_sa256;
//	int y_sa256;

//	int speed256;
//
//	int gra256; 	/* gra256 */
//	int sum256;
//	//int time_out; 	/* timer */

//	void/*BOSS05_DATA*/ *sd;
//	int *sd_angle512;
//	int angle2_512;
//	int r_or_l; 	//	/*+1==R*/	/*-1==L*/

typedef struct
{
	int speed256;								/* ���x */
//	int delta256;		/* t256(0.015) gra256 */			/* �����x */
//
//	void/*BOSS05_MAHO_DATA *sd;
//	int  *sd_nnn;
//
//	int state;
	int angle512;
	int  star_remain_time;
} BULLET_SP2_DATA;


/*---------------------------------------------------------
	���x�l�A�p�x�l�A���x�N�g���ʂɕϊ�����
---------------------------------------------------------*/
static void regist_vector(SPRITE *src, int speed256, int angle512)
{
	src->vx256	= ((sin512((angle512))*(speed256))>>8); 	/**fps_fa ctor*/
	src->vy256	= ((cos512((angle512))*(speed256))>>8); 	/**fps_fa ctor*/
}

/*---------------------------------------------------------
	��{�ʈړ��A��ʊO�͏�����
---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *src)
{
	src->x256 += (src->vx256);		/**fps_fa ctor*/
	src->y256 += (src->vy256);		/**fps_fa ctor*/
	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT)))
	{
		src->type = SP_DELETE;/*��ʊO�ɂł��炨���܂�*/
	}
}


/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e
---------------------------------------------------------*/

#if 1
static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void move_bullet_sakuya_sp1(SPRITE *src)
{
	BULLET_SP1_DATA *data = (BULLET_SP1_DATA *)src->data;
	src->base_time_out--;	//	data->timer++;
	if (0 > src->base_time_out)//	if (data->timer > 40)
	{
	//	else
		{
			src->vx256 = ((sin512((data->angle512)) ) )/**fps_fa ctor*/;
			src->vy256 = ((cos512((data->angle512)) ) )/**fps_fa ctor*/;
		}
	}
	else
	if (0+2048 > src->base_time_out)//	if (data->timer > 40)
	{
		const u8 m_tbl[4] =
		{
		//	0,1,3,7,	/* �x��������� */
			1,0,7,3,	/* �ق��̍U���Ƃ̌��ˍ����ŁA�^�C�~���O���Ƃ�Â炢������� */
		};
		/* �����ɏW�܂� */
	//	if (0==(src->base_time_out & (7>>difficulty) ))
	//	if (0==(src->base_time_out & (7>>(3-difficulty)) ))/* �x��������� */
		if (0==(src->base_time_out & (m_tbl[difficulty]) ))
		{
			data->y_sum256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{
			int x_sa256 = (src->x256 - data->player_x256);
			int y_sa256 = (src->y256 - data->player_y256);
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->base_time_out = 0;
				}
			}
			int aaa_x256 = ((x_sa256 * data->y_sum256)>>8); 	/**fps_fa ctor*/
			int aaa_y256 = ((y_sa256 * data->y_sum256)>>8); 	/**fps_fa ctor*/
			src->x256 = data->player_x256 + (aaa_x256); 		/**fps_fa ctor*/
			src->y256 = data->player_y256 + (aaa_y256); 		/**fps_fa ctor*/
		}
	}
	else
	if (0+2048==src->base_time_out) //else if (data->timer==40)
	{
	//	src->yx_anim_frame = (1);
		src->type		 = BULLET_MARU8_00_AKA+(1);
		data->angle512 = ANGLE_JIKI_NERAI_DAN;
		data->angle512 = angle_jikinerai512_auto(player, src, data->angle512);
		mask512(data->angle512);
	//	data->angle2_512 = (*(data->sd_angle512))/*(data->sdata->angle512)*/;
//		data->angle2_512 = (s->tmp_angleCCW512)/*(data->sdata->angle512)*/;
		src->vx256 = (0) /**fps_fa ctor*/;
		src->vy256 = (0) /**fps_fa ctor*/;
		/* �Ƃ肠�����A���Ή��B�����I�ɐݒ肵���Ⴄ */
		data->player_x256 = (player->x256+t256(32/2)/*���̔���*/) /**fps_fa ctor*/;
		data->player_y256 = (player->y256+t256(16/2)/*�����̔���*/) /**fps_fa ctor*/;
//			data->flag_first = 1;
			data->y_sum256 = t256(1.0); 	/* (2.5==5*0.5) */
	}
//	else
//	{
/* CCW�̏ꍇ */
//	}
//
	move_bullet_vector(src);
}
#endif


/* CCW�̏ꍇ */
	//	src->vx256 = ((((sin512((data->angle512))*(data->speed256))>>8)*(data->sum256>>2/*/5*/))>>8) + ((sin512((data->angle2_512))*(data->sum256))>>8)/**fps_fa ctor*/;
	//	src->vy256 = ((((cos512((data->angle512))*(data->speed256))>>8)*(data->sum256>>2/*/5*/))>>8) + ((cos512((data->angle2_512))*(data->sum256))>>8)/**fps_fa ctor*/;
//		src->vx256 = ((((sin512((data->angle512))*(data->speed256))>>8)*(data->sum256 ))>>8) /**fps_fa ctor*/;
//		src->vy256 = ((((cos512((data->angle512))*(data->speed256))>>8)*(data->sum256 ))>>8) /**fps_fa ctor*/;


//		data->angle512 = (*(data->sd_angle512))/*(data->sdata->angle512)*/+deg_360_to_512(90)*data->r_or_l;
//		data->angle512 = (*(data->sd_angle512))/*(data->sdata->angle512)*/-deg_360_to_512CCW(90)*data->r_or_l;
//		data->angle512 = (*(data->sd_angle512))/*(data->sdata->angle512)*/  ;
//		data->angle512 = (s->tmp_angleCCW512)/*(data->sdata->angle512)*/-deg_360_to_512CCW(90)*data->r_or_l;

//		data->sum256 += data->gra256;
//
//		if (0 == data->flag_first)
//		{
//			data->flag_first = 1;
//			data->y_sum256 = t256(1.0); 	/* (2.5==5*0.5) */
//		}
//			int aaa_x256;
//			int aaa_y256;
//			data->y_sum256 = 120/*0*/;
//			data->x_sa256 = (src->x256 - data->player_x256);
//			data->y_sa256 = (src->y256 - data->player_y256);
//			aaa_x256 = ((data->x_sa256 * data->y_sum256)>>8);	/**fps_fa ctor*/
//			aaa_y256 = ((data->y_sa256 * data->y_sum256)>>8);	/**fps_fa ctor*/
		//	src->vx256 = ((data->x_sa256 * data->y_sum256)>>8);	/**fps_fa ctor*/
		//	src->vy256 = ((data->y_sa256 * data->y_sum256)>>8);	/**fps_fa ctor*/
//			src->vx256 = (aaa_x256);	/**fps_fa ctor*/
//			src->vy256 = (aaa_y256);	/**fps_fa ctor*/
		//	src->vx256 = ((data->x_sa256)<<16);	/**fps_fa ctor*/
		//	src->vy256 = ((data->y_sa256)<<16);	/**fps_fa ctor*/
		#if 1
		#endif


/*---------------------------------------------------------
	���p�̖����w�p
---------------------------------------------------------*/

static void move_bullet_sakuya_sp2(SPRITE *src)
{
	{	BULLET_SP2_DATA *data = (BULLET_SP2_DATA *)src->data;
	//	if (2==data->sdata->nnn/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (2==(*(data->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (1==(*(data->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��0�̎��z�u���āA�X�e�[�g��1�ɂȂ����瓮�� */
		data->star_remain_time--;
		if (0>=(data->star_remain_time))/* ���� */
		{
			data->star_remain_time = 0;
			data->speed256 += t256(0.015);
/* CCW�̏ꍇ */
			src->vx256 = ((sin512((data->angle512))*(data->speed256))>>8)/**fps_fa ctor*/;
			src->vy256 = ((cos512((data->angle512))*(data->speed256))>>8)/**fps_fa ctor*/;
		}
	}
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	��e�Q
	-------------------------------------------------------
	���p�̑f���ȑ�e
---------------------------------------------------------*/

static void move_bullet_oodama2_standard(SPRITE *src)
{
	OODAMA_OYA_DATA *data = (OODAMA_OYA_DATA *)src->data;
	src->vx256 += ((src->vx256*data->add_speed256)>>8);/*����*/
	src->vy256 += ((src->vy256*data->add_speed256)>>8);/*����*/
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	��e1
	-------------------------------------------------------
	���p�̗h�炮��e
---------------------------------------------------------*/

static void move_bullet_oodama1_aya_yuragi(SPRITE *src)
{
	OODAMA_OYA_DATA *data = (OODAMA_OYA_DATA *)src->data;

	// [***090124		�ǉ��ꏊ�B���܂Ńt���[�����Ɍv�Z���Ă�������5(4)�t���[�����ɕύX
	/*data->*/src->base_time_out--;
//	/*data->*/src->base_time_out -= difficulty;
	if (0==(/*data->*/src->base_time_out&0x03)) //if (data->wait_bg>0)	{	data->wait_bg--;}	else
//	if (0==(/*data->*/src->base_time_out&0x3f)) //if (data->wait_bg>0)	{	data->wait_bg--;}	else
	{
	//	data->wait_bg=5;
	//	data->angle512 += rad2 deg512(((ra_nd()%data->ransu)-data->ransu/2)/10);
//		data->angle512 += ((((ra_nd()&(256-1))*data->ransu512)-((data->ransu512)<<7))>>8);
//		mask512(data->angle512);
		int data_add_speed256  = /*256+*/ ((((ra_nd()&(256-1))*data->ransu512)-((data->ransu512)<<7))>>8);
		src->vx256 += ((src->vx256*data_add_speed256)>>8);/*����*/
		src->vy256 += ((src->vy256*data_add_speed256)>>8);/*����*/
	}
	move_bullet_vector(src);
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

---------------------------------------------------------*/

#define MOMIJI_KODOMO_next_angle512 tmp_angleCCW512
//
static void bullet_create_momiji_seed(SPRITE *src, int speed256, int angle512, int offset_angle512, int adjust_speed256);
static void move_bullet_momiji_oya(SPRITE *src)
{
//	MOMIJI_DATA *data = (MOMIJI_DATA *)src->data;
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	src->x256 += (/*data->v*/src->vx256)/**fps_fa ctor*/;
	src->y256 += (/*data->v*/src->vy256)/**fps_fa ctor*/;
	/*data->*/src->base_time_out -= 1/*fps_fa ctor*/;
	if (/*data->*/src->base_time_out < 1)
	{
		/* 5�������݂��e���쐬 */
		int jj;
		for (jj=(int)((512)-(512*2/12)); jj<(int)((512)+(512*3/12)); jj+=(int)(512*1/12) )
		{
			bullet_create_momiji_seed(src, t256(3.0), src->MOMIJI_KODOMO_next_angle512/*data->angle512*/, jj,			 t256(-0.04));
		}
		src->type = SP_DELETE;
	}
	if ((src->x256<-((src->w128+src->w128)))||(src->x256 > t256(GAME_WIDTH))||
		(src->y256<-((src->h128+src->h128)))||(src->y256 > t256(GAME_HEIGHT)))
	{
		src->type = SP_DELETE;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_smallbullet_re_move(SPRITE *src)
{
	MOMIJI_KODOMO_DATA *data = (MOMIJI_KODOMO_DATA *)src->data;
//	switch (data->state)
//	{
//	case 0:
		if (data->speed256 < t256(0.5) )
		{
			data->angle222_512 += 256/*rad2 deg512(M_PI)*/;
			#if 1
			/*�˂�̂���*/
			mask512(data->angle222_512);
			#endif
			data->angle512	= data->angle222_512;
			data->adjust_speed256 = (-(data->adjust_speed256));
//			data->state++/* 	= 1*/;
		}
//		break;
//	case 1:
//		break;
//	}
	data->speed256 += data->adjust_speed256;
	if (data->angle512-data->angle222_512 < (int)(512*1/12) )
	{
		data->angle512 += 1/*0.01*/;
	}
	#if 1
	/*�˂�̂���*/
	mask512(data->angle512);
	#endif
//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
	src->vx256 = ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->vy256 = ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->x256 += (src->vx256)/**fps_fa ctor*/;
	src->y256 += (src->vy256)/**fps_fa ctor*/;

	/*data->*/src->base_time_out -= 1/*fps_fa ctor*/;
//	if (0 > src->timeover_ticks)	/* ����ܗǂ��Ȃ� */
	if (0 > /*data->*/src->base_time_out)/*200*/
	{
		if ((src->x256<-((src->w128+src->w128)))||(src->x256 > t256(GAME_WIDTH))||
			(src->y256<-((src->h128+src->h128)))||(src->y256 > t256(GAME_HEIGHT)))
		{
			src->type = SP_DELETE;
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
#define TOMARI_DAN_next_angle512 tmp_angleCCW512
#define TOMARI_DAN_LIMIT_00_640 (512+128)
#define TOMARI_DAN_LIMIT_00_576 (512+64)
#define TOMARI_DAN_LIMIT_01_512 (512+0)
#define TOMARI_DAN_LIMIT_02_000 (0+0)
static void bullet_create_oodama22(
	SPRITE *src,
	int speed256,
	int angle512,
	int ransu512,
	int add_speed256/*, int xoffs256, int yoffs256*/);
//atic void enemy_stop_bullet_move(SPRITE *src)
static void enemy_stop_bullet2_move(SPRITE *src)
{
	TOMARI_DAN_DATA *data = (TOMARI_DAN_DATA *)src->data;
	/*data->*/src->base_time_out--;
	if (TOMARI_DAN_LIMIT_00_640 < /*data->*/src->base_time_out)
	{
//	case 0: 	/* �~�܂� */
		data->speed256 -= (data->delta256);
		/*data->v*/src->vx256 = (((data->init_vx256)*data->speed256)>>8)/**fps_fa ctor*/;
		/*data->v*/src->vy256 = (((data->init_vy256)*data->speed256)>>8)/**fps_fa ctor*/;
		if (data->speed256 < t256(0))
		{
			data->speed256 = t256(0);
		//	data->state = 1;
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < /*data->*/src->base_time_out)
	{
//		break;
//	case 1: 	/* ���΂���~ */
//		if (64 > /*data->*/src->base_time_out)
//		{
//			data->state = 2;
//		}
//		break;
//	case 2: 	/* ����(��e�ɕϐg) */
	//	src->anim_frame =						((/*data->*/src->base_time_out>>3)/*&0x07*/);	/*�x��(�ϐg����̂�)*/
		src->type		= BULLET_MARU8_00_AKA+	((/*data->*/src->base_time_out>>3)&0x07);		/*�x��(�ϐg����̂�)*/
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < /*data->*/src->base_time_out)
	{
	//	data->timer = 0;
	//	data->state = 3;
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle512/*data->next_angle512*/)
		{
			bullet_create_oodama22(src,
				t256(1.0/*0.0*/),
				(src->TOMARI_DAN_next_angle512)/*data->next_angle512*/,
				0,
				(data->delta256/**7*/)/*, 0, 0*/);
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			/*data->*/src->base_time_out=(TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < /*data->*/src->base_time_out)
	{
		;	/* ���΂���~ */
	}
	else
	{
//		break;
//	case 3: 	/* ������ */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if ( 0x20000000 > src->color32) 	/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->type = SP_DELETE;
		}
//		break;
	}
	/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle512/*data->next_angle512*/)
	{	/* �X�N���[������ */
		src->y256 += t256(1);	//	my_adjust_y(s,pd);
	}
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	�������̂ŁA�F�X��̉�www�B
	�ł��A�܂�����B
---------------------------------------------------------*/

/* 90/360�x��] */
//static void rot_90_angle512(HAZUMI_DAN_DATA *d)
//{
//	data->angle512	= atan_512(sin512((data->angle512)),-cos512((data->angle512)));
//}
static void bullet_move_hazumi_dan(SPRITE *src)
{
	HAZUMI_DAN_DATA *data = (HAZUMI_DAN_DATA *)src->data;
	if (/*data->sum256*/src->vy256 < t256(15) ) 	/* �ő�d�͉����x */
	{
		/*data->sum256*/src->vy256 += data->delta256;	/* �␳�l */
	}
//	s->y256 += data->sum256;			/* �ώZ�d�͉����x */
//	mono_angle_move(s,(BULLET_ANGLE_DATA *)d);
	src->y256 += src->vy256;
	src->x256 += src->vx256;

	/* ���̕� */
	if ((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH)))
	{
		#if 1
		src->vx256 = -(src->vx256);
	//	rot_90_angle512(d); 	/*	���˕Ԃ� */
		#else
		src->type = SP_DELETE;		/*	������ */
		#endif
	}
	/* �� */
	else if (src->y256 > t256(GAME_HEIGHT))
	{
		/* �ݒ蒵�˂�� */
		if (0!=data->bound_counts)
		{
			data->bound_counts--;	/* ���˂�񐔂����炷 */
		//	src->anim_frame = 4+(data->bound_counts);		// [***090116		�኱�ύX
			src->type		= BULLET_MARU8_00_AKA+4+(data->bound_counts);		// [***090116		�኱�ύX
		//	data->sum256		= -data->sum256;		/* �d�͉����x */
			#if 0
			data->speed256	= -data->speed256;		/* ���x */
			data->speed256	+= data->speed256;		/* ����(���ēV��֓�����) */
		//	rot_90_angle512(d); 		// [***090116		�ǉ�
			#else
			src->vy256	= -(src->vy256);	/* ���x */
			src->vy256	+= (src->vy256>>2); /* 1.25�{�� ����(���ēV��֓�����) */
			src->vx256	+= src->vx256;		/* ���� */
			#endif
		}
		else
		{
			src->type = SP_DELETE;		/*	������ */
		}
	}
	#if 1
	/* �V�� */
	else if (src->y256 < 0)
	{
		src->type = SP_DELETE;		/*	������ */
	}
	#endif
}

/*---------------------------------------------------------
	�d�͒e�̈ړ�
	�����~���i�C�t(��)
---------------------------------------------------------*/

static void bullet_move_gravity(SPRITE *src)
{
	GRAVITY_BULLET_DATA *data = (GRAVITY_BULLET_DATA *)src->data;
	src->vy256 += data->delta256;
//	data->speed256 += data->delta256;
//	src->y256 += data->speed256;
	move_bullet_vector(src);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
#define knife_tmp_angle512 tmp_angleCCW512

static void enemy_follow_knife_move(SPRITE *src)
{
	FOLLOW_KNIFE_DATA *data = (FOLLOW_KNIFE_DATA *)src->data;
	if (0==data->target)
	{
//		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	//	data->aaa_speed256 -= t256(0.02);
	//	data->aaa_speed256 -= t256(0.02*2);/*2�{�ɂ����̂Œ��덇�킹*/
		data->aaa_speed256 -= t256(0.04);	/* �i�C�t���x�A�����萔 */
	//	if (s->anim_frame==17)	{	src->anim_frame = 0;}
	//	else					{	src->anim_frame++;}
		/*data->p_angle512*/src->m_angleCCW512 += 32;
		mask512(src->m_angleCCW512/*data->p_angle512*/);/*�˂�̂���*/
		if (data->aaa_speed256 <= t256(0) )
		{
			src->knife_tmp_angle512/*data->aaa_angle512*/ = atan_512(
				(player->y256)-(src->y256)+(player->h128)+(data->height*256),
				(player->x256)-(src->x256)-(player->w128)  );
			data->aaa_speed256 = data->speed_2nd_256;
			data->target=1;
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		//	src->anim_frame=/*17-*/(deg_512_to_360(data->angle512)/20)%18;
		//	src->anim_frame=/*17-*/(deg_512_to_360(data->angle512)/20)%16;
			mask512(src->knife_tmp_angle512/*data->aaa_angle512*/);/*�˂�̂���*/
		//	src->anim_frame=/*17-*/((data->angle512)>>5);
			src->m_angleCCW512/*data->p_angle512*/=/*17-*/((src->knife_tmp_angle512/*data->aaa_angle512*/));
		}
		/*data->*/src->vx256 = ((sin512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(data->aaa_speed256))>>8)/**fps_fa ctor*/;
		/*data->*/src->vy256 = ((cos512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(data->aaa_speed256))>>8)/**fps_fa ctor*/;
	}
	src->x256 += (/*data->*/src->vx256)/**fps_fa ctor*/;
	src->y256 += (/*data->*/src->vy256)/**fps_fa ctor*/;
//
	/*data->*/src->base_time_out--;
	if (/*data->*/src->base_time_out<0)
	{
		if (((src->x256+((src->w128+src->w128)) < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256+((src->h128+src->h128)) < t256(0))||(src->y256 > t256(GAME_HEIGHT))))
		{
			src->type = SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
		}
	}
}

/*---------------------------------------------------------

	-------------------------------------------------------
	s->x256 	�i�C�t��x���W
	data->x256	�m�[�h��x���W
	s->w/2		�i�C�t�̉����̔���
	�i�C�t��x���W���m�[�h��x���W�{co_s512((data->angle512))�~length�|�i�C�t�̉����̔���
---------------------------------------------------------*/
#define ryoute_knife_length256 t256(128)
//static void enemy_evenr_knife_move(SPRITE *src)
static void enemy_even_knife_move(SPRITE *src)
{
	EVEN_KNIFE_DATA *data = (EVEN_KNIFE_DATA *)src->data;
	switch (data->state)
	{
	case 0:
		if (100 < data->wait1)
		{
			data->wait1 = 0;
			data->state = 1;
		}
		data->angle512 -= (data->d_angle512*data->l_or_r) /**fps_fa ctor*/;
		mask512(data->angle512);
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->m_angleCCW512 = (data->angle512+((int)(512*1/4)*data->l_or_r));
		mask512(src->m_angleCCW512);/*�K���v��*/
	//	src->x256		= data->x256-((cos512((data->angle512))*data->length256)>>8)-((src->w128));
	//	src->y256		= data->y256-((sin512((data->angle512))*data->length256)>>8)-((src->h128));
	//	src->x256		= data->x256-((sin512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->w128));
	//	src->y256		= data->y256-((cos512((data->angle512))*ryoute_knife_length256/*data->length256*/)>>8)-((src->h128));
		src->x256		= data->x256-((sin512((data->angle512)))<<7 )-((src->w128));
		src->y256		= data->y256-((cos512((data->angle512)))<<7 )-((src->h128));
		break;

	case 1:
		if (50 < data->wait1)
		{
			data->wait1 = 0;
			data->state = 0;
		}
		//#if 1
		//data->speed=20/*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		//#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		src->vy256 = ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		src->x256 += (src->vx256)/**fps_fa ctor*/;
		src->y256 += (src->vy256)/**fps_fa ctor*/;
		break;
	}
	if (10 < data->wait1)
	{
	 	data->wait1 = 0;
		#if 1
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.5)/*t256(5.0)*/),
			(data->angle512+((int)(512*3/12)*data->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.2)/*t256(4.0)*/),
			(data->angle512+((int)(512*4/12)*data->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		bullet_create_n_way_dan_sa_type(src,
			(t256(2.0)/*t256(3.0)*/),
			(data->angle512+((int)(512*5/12)*data->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		if (difficulty>1)
		{
		bullet_create_n_way_dan_sa_type(src,
			(t256(1.8)/*t256(2.0)*/),
			(data->angle512+((int)(512*6/12)*data->l_or_r)),
			(int)(512/(18)),		/* �_�~�[�����p�x(1way�Ȃ̂Ŏg�p���Ă��Ȃ�) */
			BULLET_KNIFE20_04_AOI,
			(1));/* [1way�i�C�t�e] */
		}
		#else
		bullet_create_sakuya_knife(src, t256(5.0), data->angle512+(int)(512*3/12), data->p_angle512); //+M_PI/2
		bullet_create_sakuya_knife(src, t256(4.0), data->angle512+(int)(512*4/12), data->p_angle512);
		bullet_create_sakuya_knife(src, t256(3.0), data->angle512+(int)(512*5/12), data->p_angle512);
		if (difficulty>1)
		{	bullet_create_sakuya_knife(src, t256(2.0), data->angle512+(int)(512*6/12), data->p_angle512);}
		#endif
	}
	data->wait1++;
	/*data->*/src->base_time_out--;
	if ( /*data->*/src->base_time_out < 0 )
	{
		src->type = SP_DELETE;
	}
}

enum
{
	KS00 = 0,
//	KS03_type2,
	KS01,
	KS02,
//	KS04,
};
/*---------------------------------------------------------

---------------------------------------------------------*/
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
#define ice_angle512		m_angleCCW512
#define ice_add_angle512	tmp_angleCCW512

static void enemy_boss02_ice_move(SPRITE *src)
{
	BOSS02_ICE_DATA *data = (BOSS02_ICE_DATA *)src->data;
	switch (data->state)
	{
	case KS00:	/* �~��^�� */
		/*data->*/src->base_time_out -= 1/*fps_fa ctor*/;
		if (/*data->*/src->base_time_out <= 0)
		{
			data->state = data->next_2nd_state;
		//	data->state++/* = KS01*/;
		}
		src->ice_angle512 += src->ice_add_angle512/**fps_fa ctor*/;
//
		mask512(src->ice_angle512);
		src->vx256 = ((sin512((src->ice_angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		src->vy256 = ((cos512((src->ice_angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		break;
	case KS01:	/* �o���o���ړ� */
		data->state++/* = KS02*/;
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
//		src->ice_angle512	= (ra_nd()&(128-1)/*%90*/)+deg_360_to_512(45);
		src->ice_angle512	= (ra_nd()&(128-1)/*%90*/)-deg_360_to_512CCW(45);
//
		mask512(src->ice_angle512);
		src->vx256 = ((sin512((src->ice_angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		src->vy256 = ((cos512((src->ice_angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		break;
//	case KS02:	/* �������Ȃ�(���������ړ�) */
//		break;
	}
//
	src->x256 += (src->vx256)/**fps_fa ctor*/;
	src->y256 += (src->vy256)/**fps_fa ctor*/;
//	move_bullet_vector(src);
	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
		(src->y256 < t256(-100))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{
		src->type = SP_DELETE;
	}
}


/*---------------------------------------------------------
	�����e
---------------------------------------------------------*/

void bullet_create_momiji_dan(SPRITE *src, int speed256, int angle512)
{	/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/sp ell_bullet_r.png", 1, PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
	h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);
	h->type 			= BULLET_MARU8_00_AKA+(1);/*1==�Ƃ肠����*/ /*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= move_bullet_momiji_oya;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(1);/*1==�Ƃ肠����*/
	h->x256 			= (src->x256)+((src->w128));
	h->y256 			= (src->y256)+((src->h128));
	/*data->*/h->base_time_out		= ((30)+1);
	#if 1/*???*/
	h->MOMIJI_KODOMO_next_angle512		= (angle512);
	#endif
//
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
//

//	MOMIJI_DATA *data;
//	data				= mmalloc(sizeof(MOMIJI_DATA));
//	h->data 			= data;
}

/*---------------------------------------------------------
	���e����
---------------------------------------------------------*/

static void bullet_create_momiji_seed(SPRITE *src, int speed256, int angle512, int offset_angle512, int adjust_speed256)
{
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/kugel2.png", 1, /*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/, 0);
//	h					= spr ite_add_res(BASE_TAMA_KUGEL_MINI2_PNG);
	h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);
	h->type 			= /*BULLET_MINI8_00_AKA*/BULLET_MARU8_00_AKA+((angle512>>6)&7); /*BULLET_MINI8_00_AKA*/ /*S P_BULLET*/
	h->callback_mover	= enemy_smallbullet_re_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(8+(angle512>>6)) /*8*/ /*0*/;
	h->x256 			= (src->x256)+((src->w128));
	h->y256 			= (src->y256)+((src->h128));
	/*data->*/h->base_time_out		= 200;	/* ���� */
//
	MOMIJI_KODOMO_DATA *data;
	data				= mmalloc(sizeof(MOMIJI_KODOMO_DATA));
	h->data 			= data;
//	data->state 		= 0;
	angle512 += offset_angle512;
	mask512(angle512);
	data->angle222_512	= (angle512);
	data->angle512		= (angle512);
	data->speed256		= (speed256);
	data->adjust_speed256	= adjust_speed256;
}

/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/

// [***090114	�ǉ�
void bullet_create_hazumi_dan(SPRITE *src, int speed256, int angle512, int delta256, int bound_counts)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot2.png",3,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);/*�Ή��e*/
	h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);/*�Ή��e*/
	h->type 			= BULLET_MARU8_00_AKA+(4+bound_counts);/*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_hazumi_dan;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(4+bound_counts);/*�Ƃ肠����*/
	h->x256 			= src->x256+((src->w128-h->w128));
	h->y256 			= src->y256+((src->h128-h->h128));
	HAZUMI_DAN_DATA *data;
	data				= mmalloc(sizeof(HAZUMI_DAN_DATA));
	h->data 			= data;
	/*angle512			= angle_jikinerai512_auto(player, src, angle512);*/
	mask512(angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);

	data->delta256		= (delta256);
//	data->sum256		= t256(0);
	data->bound_counts	= bound_counts;
}

/*---------------------------------------------------------
	�d�͒e�B�A���X�̐l�`�����e�B
	-------------------------------------------------------
	�����~���i�C�t(��)
	-------------------------------------------------------
	�������ɉ������Ă������e�B
	�t���[�����Ōv�Z���Ă���̂ł��Ȃ�d�͉����x�����������Ȃ��Ƒ�ςȂ��ƂɁB
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/
/*bullet_create_sakuya_no_rot_knife*/
void bullet_create_jyuryoku_dan000(
	SPRITE *src,
	int speed256,
	int angle512,
	int delta256,
	int bullet_obj_type 	/* �e�O�� */	/*BULLET_MARU8_00_AKA+(7)*/ 	/*(BULLET_KNIFE20_05_AKA)*/
)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//	h					= spr ite_add_file 0("tama/bs hoot.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,0);/*�e*/
//	h					= spr ite_add_file 0("tama/knife.png",1,/*PRIORITY_05_BULLETS*/PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY*/,1);/*�����~���i�C�t(��)*/
//	h					= sprite_add_gu(TAMA_TYPE_BULLET_KNIFE01_PNG);/*�����~���i�C�t(��)*/
	h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);/*�e*/
	h->type 			= (bullet_obj_type);/*BULLET_MARU8_07_AOI*/  /*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= bullet_move_gravity;	/*enemy_fall_knife_move*/
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(7)/*0*/;/*�Ƃ肠����*/
	h->x256 			= src->x256+((src->w128-h->w128));
	h->y256 			= src->y256+((src->h128-h->h128));
	/*FALL_KNIFE_DATA*/GRAVITY_BULLET_DATA *data;
	data				= mmalloc(sizeof(GRAVITY_BULLET_DATA/*FALL_KNIFE_DATA*/));
	h->data 			= data;
//
	/*angle512			= angle_jikinerai512_auto(player, src, angle512);*/
	mask512(angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	regist_vector(h, speed256, angle512);
	data->delta256		= (delta256);
//	data->speed256		= t256(0);
}


/*---------------------------------------------------------
	���p�̉��F�}�X�J�b�g�e
	-------------------------------------------------------
	���̒e�͑��̒e�������̂ł����Ēx������B
---------------------------------------------------------*/

/*static*/ //void enemy_sp1_bullet_create(SPRITE *src, int speed256, int angle512, int gra256, int r_or_l, int *sd_angle512)

void sakuya_sp1_bullet_create_bbb(SPRITE *src)
{
	int j;
	for (j=(0/*k*/); j<(8/*-k*/); j++)
	{
		//static void sakuya_sp1_bullet_create(/*-1==L*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*//*, int *sd_angle512*/)
		SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
//		h					= spr ite_add_file 0("tama/bs hoot2.png", 3, PRIORITY_03_ENEMY, 0);
//		h					= spr ite_add_file 0("tama/bullet_maru8.png", 8, PRIORITY_03_ENEMY, 0);
		h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);		h->priority 	= PRIORITY_03_ENEMY;
		unsigned int aaa = ((ra_nd()&(4-1))+4);
		h->type 			= BULLET_MARU8_00_AKA+(aaa);/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= move_bullet_sakuya_sp1;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//		h->anim_frame		= 0;// /*4*/(aaa)/*0*/;/*�Ƃ肠����*/
//		h->anim_speed		= 0;
		h->x256 			= src->x256+((src->w128-h->w128));
		h->y256 			= src->y256+((src->h128-h->h128));
		h->base_time_out	= 2048+20+(40+1);		/*data->timer		= 0;*/

		int angle512		= src->tmp_angleCCW512+((j-4)*42)/*deg_360_to_512(30)*/;
		h->vx256 = ((sin512((angle512))*t256(1.0))>>8)/**fps_fa ctor*/;
		h->vy256 = ((cos512((angle512))*t256(1.0))>>8)/**fps_fa ctor*/;



		BULLET_SP1_DATA *data;
		data				= mmalloc(sizeof(BULLET_SP1_DATA));
		h->data 			= data;
		data->y_sum256		= t256(1.0);	/* (2.5==5*0.5) */
	}
}
//		if (angle512==999/*-2*/)
	//	angle512			= angle_jikinerai512_auto(player, src, angle512);
	//	data->angle512		= angle512;
	//	int speed256		= (ss[j]);

//		data->gra256		= (ss[j+8]);
//		data->sum256		= t256(0);


//		data->angle2_512	= (0);
//		data->sd			= (BOSS05_DATA *)s->data;
//		data->sd_angle512	= &(src->tmp_angleCCW512)/*sd_angle512*/;
//		data->r_or_l		= /*r_or_l*/((j<4)?(-1):(1));

//	const u8 kk[4] =	{		2,		1,		0,		0,	};
//	const u16 ss[16] =
//	{
	//	t256(1.15), 	t256(1.00), 	t256(0.85), 	t256(0.70),
	//	t256(0.70), 	t256(0.85), 	t256(1.00), 	t256(1.15), //
	//	t256(0.040),	t256(0.036),	t256(0.033),	t256(0.030),
	//	t256(0.030),	t256(0.033),	t256(0.036),	t256(0.040),
	//	294,	256,	217,	179,
	//	179,	217,	256,	294, //
//		256,	240,	216,	200,
//		200,	216,	240,	256, //
//		10,  9,  8,  7,
//		 7,  8,  9, 10,
//	};
//	int k;
//	k = kk[difficulty /*&3*/ ];

//		sakuya_sp1_bullet_create(src, (t256(1.15)), src->tmp_angleCCW512-4*42/*deg_360_to_512(30)*/, (t256(0.040)), -1/*-1==L*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*/
//		sakuya_sp1_bullet_create(src, (t256(1.00)), src->tmp_angleCCW512-3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/ /*, s_data_angle512*/ );/*1.08 == 1.8*0.6*/
//		sakuya_sp1_bullet_create(src, (t256(0.85)), src->tmp_angleCCW512-2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/ /*, s_data_angle512*/ );/*0.9 == 1.5*0.6*/
//		sakuya_sp1_bullet_create(src, (t256(0.70)), src->tmp_angleCCW512-1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/ /*, s_data_angle512*/ );/*0.72 == 1.2*0.6*/
//
//		sakuya_sp1_bullet_create(src, (t256(0.70)), src->tmp_angleCCW512+1*42/*deg_360_to_512(30)*/, (t256(0.030)),  1/* 1==R*/ /*, s_data_angle512*/ );/*0.72 == 1.2*0.6*/
//		sakuya_sp1_bullet_create(src, (t256(0.85)), src->tmp_angleCCW512+2*42/*deg_360_to_512(30)*/, (t256(0.033)),  1/* 1==R*/ /*, s_data_angle512*/ );/*0.9 == 1.5*0.6*/
//		sakuya_sp1_bullet_create(src, (t256(1.00)), src->tmp_angleCCW512+3*42/*deg_360_to_512(30)*/, (t256(0.036)),  1/* 1==R*/ /*, s_data_angle512*/ );/*1.08 == 1.8*0.6*/
//		sakuya_sp1_bullet_create(src, (t256(1.15)), src->tmp_angleCCW512+4*42/*deg_360_to_512(30)*/, (t256(0.040)),  1/* 1==R*/ /*, s_data_angle512*/ );/*1.26 == 2.1*0.6*/

#if 1/*�����w�p*/
/*---------------------------------------------------------
	���̖����w�p
	angle512	512�x�w��
---------------------------------------------------------*/

/*static*/ void enemy_sp2_bullet_create01(SPRITE *src, int angle512, int star_remain_time/**sd_nnn*/)
{		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
//	h					= spr ite_add_file 0("tama/kugel.png", 1, PRIORITY_03_ENEMY, 0);
//	h					= spr ite_add_res(BASE_TAMA_KUGEL_PNG); h->priority 	= PRIORITY_03_ENEMY;
	h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);	h->priority 	= PRIORITY_03_ENEMY;
	h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
	h->callback_mover	= move_bullet_sakuya_sp2;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(0);
//	h->anim_speed		= 0;
	h->x256 			= src->x256+((src->w128-h->w128));
	h->y256 			= src->y256+((src->h128-h->h128));
	h->vx256			= 0;
	h->vy256			= 0;

	BULLET_SP2_DATA *data;
	data				= mmalloc(sizeof(BULLET_SP2_DATA));
	h->data 			= data;
//	data->sd			= (BOSS05_MAHO_DATA *)s->data;
	data->star_remain_time		= star_remain_time;
	data->angle512		= (angle512);
	data->speed256		= t256(0);
}
//	data->state 		= 0;
//	data->delta256		= (delta256);

#endif



/*---------------------------------------------------------
	�΂�܂��U�������w(�ʏ̃}���z�[��)���΂�܂��A�~�܂�e
	-------------------------------------------------------
	��邪�΂�܂��A�~�܂�e�B��e�ɕω�����
	-------------------------------------------------------
	angle512	ANGLE_JIKINERAI_DAN (999) �Ńv���C���[�_��
	delta256	�t���[������speed���ǂꂾ���������邩
---------------------------------------------------------*/
// bullet_create_tomari_dan
void bullet_create_tomari2_dan(
	SPRITE *src,
	int speed256,	/* �e�� */
	int angle512,	/* �p�x */
	int delta256,	/* �����萔 */
	int next_angle512
	)
{
	SPRITE *h;
//	h					= sprite_add_gu(TAMA_TYPE_KUGEL_PNG);/*���Ԓe*/
	h					= sprite_add_gu(TAMA_TYPE_BULLET_MARU16_PNG);		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
	h->callback_mover	= enemy_stop_bullet2_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(0);
	h->x256 			= src->x256+((src->w128-h->w128));
	h->y256 			= src->y256+((src->h128-h->h128));
	TOMARI_DAN_DATA *data;
	data				= mmalloc(sizeof(TOMARI_DAN_DATA));
	h->data 			= data;
	angle512			= angle_jikinerai512_auto(player, src, angle512);
//	data->angle512		= (angle512);
//	data->speed256		= (speed256);
	data->speed256		= (speed256);
	regist_vector(h, t256(1.0)/*speed256*/, angle512);
	data->init_vx256	= /*data->*/h->vx256;
	data->init_vy256	= /*data->*/h->vy256;
//
	//data->state		= 0;
	data->delta256		= (delta256);
	if (ANGLE_NO_SHOT_DAN==next_angle512)/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	{
;// 	/*data->*/h->base_time_out	= 800+64;
	}
	else
	{
;// 	/*data->*/h->base_time_out	= 100+64;
		mask512(next_angle512);
	}
	/*data->next_angle512*/h->TOMARI_DAN_next_angle512 = (next_angle512);
	/*data->*/h->base_time_out	= 1024;/* �K���ɑ傫��1024==(512+64+�ő��~����) */
}


/*---------------------------------------------------------
	�S�����i�C�t(��)
	�_���i�C�t(��)
---------------------------------------------------------*/

void bullet_create_sakuya_follow_knife1(SPRITE *src, int speed256, int angle512, int height)
{/* x, y*/
	SPRITE *h;
	h					= sprite_add_gu(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/
	h->type 			= /*BULLET_KNIFE20_07_MIDORI*/BULLET_KNIFE20_04_AOI;/*BULLET_KNIFE20_04_AOI*/	/* (���΃i�C�t�ɕϐg) */		/*S P_BULLET*/ /*SP_LASER*/
	h->callback_mover	= enemy_follow_knife_move;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//0;
//	h->x256 			= src->x256;
//	h->y256 			= src->y256/*x*/;
	h->x256 			= src->x256+((src->w128-h->w128));
	h->y256 			= src->y256+((src->h128-h->h128));
	/*data->*/h->base_time_out		= 300;
	h->knife_tmp_angle512/*data->aaa_angle512*/ 	= (angle512);
//
	FOLLOW_KNIFE_DATA *data;
	data				= mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data 			= data;
	data->aaa_speed256	= (speed256/*+speed256*/);	/* �i�C�t�̓��������x */
	data->speed_2nd_256 = (speed256+speed256);	/* �i�C�t�̑_�����x */
	data->target		= 0;
	data->height		= height;
}


/*---------------------------------------------------------
	����i�C�t(��)
	-------------------------------------------------------
	length	enemy-player�Ԃ̋���/��2
	r_or_l	0==�E	1==��
---------------------------------------------------------*/

void bullet_create_sakuya_even_knife_bbb(SPRITE *src)
//, int speed256/*, int dummy_length256*/, int r_or_l)/*0==�E*/ 	/*1==��*/
{
int speed256;
	speed256	= t256(10);
	int r_or_l;
//	for (r_or_l=0; r_or_l<2; r_or_l++)
	for (r_or_l=-1; r_or_l<2; r_or_l+=2)
	{
		SPRITE *h;
		h					= sprite_add_gu(TAMA_TYPE_BULLET_KNIFE18_PNG);/*�S�����i�C�t(��)*/	/*h->use_alpha=0 ????*/
		h->type 			= BULLET_KNIFE20_04_AOI;/*S P_BULLET*/ /*SP_LASER*/
	//
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	//	h->anim_frame		= 0;//0;
		h->x256 			= src->x256+((src->w128-h->w128));
		h->y256 			= src->y256+((src->h128-h->h128));
		/*data->*/h->base_time_out		= (150+difficulty*50);
	//
		EVEN_KNIFE_DATA *data;
		data				= mmalloc(sizeof(EVEN_KNIFE_DATA));
		h->data 			= data;
	//
		int tmp512;
		tmp512 = angle_jikinerai512(player,src);/*???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
		h->callback_mover	= enemy_even_knife_move;
		#if 0
	//	if (1==r_or_l)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (0!=r_or_l)
		{
	//		h->callback_mover	= enemy_evenl_knife_move;
			tmp512 += (64)/*M_PI*2*1/8*/;
			data->l_or_r			= -1;
		}
		else
	//	if (0==r_or_l)
		{
	//		h->callback_mover	= enemy_evenr_knife_move;
			tmp512 -= (64)/*M_PI*2*1/8*/;
			data->l_or_r			= 1;
		}
		#else
		data->l_or_r			= r_or_l;
		tmp512 -= (64*r_or_l)/*M_PI*2*1/8*/;
		#endif
		mask512(tmp512);
	//	if (0==length256)	{length256=1*256;}
	//	data->length256 	= length256;
	//	data->x256			= /*s->x256+((s->w128))*/h->x256+((cos512((tmp512))*length256)>>8);
	//	data->y256			= /*s->y256+((s->h128))*/h->y256+((sin512((tmp512))*length256)>>8);
	//	data->x256			= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512))*ryoute_knife_length256/*length256*/)>>8);
	//	data->y256			= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512))*ryoute_knife_length256/*length256*/)>>8);
		data->x256			= /*s->x256+((s->w128))*/h->x256+((sin512((tmp512)) )<<7 );
		data->y256			= /*s->y256+((s->h128))*/h->y256+((cos512((tmp512)) )<<7 );
		data->angle512		= /*ra d2deg512*/(/*deg512_2rad*/(tmp512));
	//	data->d_angle512	= ra d2deg512(speed/(M_PI*2*length));	//��angle���C��/(2�΁~length/��2)
	//	int tmp_512;
	//	tmp_512=speed/(/*M_PI*2*/512*length);	//��angle���C��/(2�΁~length/��2)
		data->d_angle512	= 1;//	  /*ra d2deg512*/(/*deg512_2rad*/(tmp_512));
	//
		data->speed256		= /*10*/t256(10)/*8+difficulty*/ /*speed*/;/*�悭�킩��Ȃ����~�܂����Ⴄ*/
		data->state 		= 0;
		data->wait1 		= 0;
	}
}


/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/
void bullet_create_n_way_dan_sa_type(
	SPRITE *src,			/*	*/
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
	angle512			= angle_jikinerai512_auto(player, src, angle512); /*ANGLE_JIKI_NERAI_DAN???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
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
		h					= sprite_add_gu(TAMA_TYPE_BULLET_DUMMY/*_MING32_PNG*/);
		h->type 			= (bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*S P_BULLET*/
		h->callback_mover	= move_bullet_vector;
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		/*h->anim_speed 	= 0;*/ /*if (frame) {	h->anim_speed=1;}*/
//		h->anim_frame		= 0;//((i_angle512)>>4)/*0*/;
		h->x256 			= src->x256+((src->w128-h->w128));
		h->y256 			= src->y256+((src->h128-h->h128));
//		BULLET_VECTOR_DATA *data;
//		data				= mmalloc(sizeof(BULLET_VECTOR_DATA));
//		h->data 			= data;
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

#if 0
/*---------------------------------------------------------
	���ׂĂ̒e�������ʉ�
	-------------------------------------------------------

---------------------------------------------------------*/

BULLET_STATUS bullet_resource[BULLET_RES_MAX] =
{
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
	{	&dummy_obj, 	0,	0,	0,	0,	0,	0,	0,	NULL,	NULL,	NULL,	NULL,	},
};
void bullet_create_resource(int type)
{

}
#endif


/*---------------------------------------------------------
	�v���C���[�_���e(��)���쐬����
---------------------------------------------------------*/
//enemy_bullet_create
void bullet_create_aka_maru_jikinerai(
	SPRITE *src,
	int speed256
)
{
	bullet_create_n_way_dan_sa_type(
		src,
		speed256,
		ANGLE_JIKI_NERAI_DAN,
		(0),					/* �_�~�[�p�x(���g�p) */
		BULLET_MARU8_00_AKA,	/* [�Ԓe] */
		(1));					/* [1way] */
}

/*---------------------------------------------------------
	�m�[�}���e
---------------------------------------------------------*/
/*bullet_create_maru8_frame*/
/*---------------------------------------------------------
	�o���ʒu�I�t�Z�b�g�e���쐬����
	�j�e���쐬����
---------------------------------------------------------*/
void bullet_create_offset_dan_type000(
	SPRITE *src,			/*	*/
	int speed256,			/* �e�� */
	int angle512			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
	/*, int x_offset256, int y_offset256*/,
	int bullet_obj_type 	/* �e�O�� */
)
{
	bullet_create_n_way_dan_sa_type(
		src,
		speed256,
		angle512,
		(0),				/* �_�~�[�p�x(���g�p) */
		bullet_obj_type,	/* [�Ԓe] */
		(1));				/* [1way] */
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

void bullet_create_oodama00(
	SPRITE *src,
	int speed256,
	int angle512,
	int ransu512,
	int add_speed256/*,int xoffs256,int yoffs256*/)
{
	angle512			= angle_jikinerai512_auto(player, src, angle512);
	mask512(angle512);
//
	SPRITE *h;
	h					= sprite_add_gu(TAMA_TYPE_OODAMA_08_PNG);	 /* ��e(��) �\������*/
	h->priority 		= PRIORITY_03_ENEMY;
//	unsigned int aaa = (((angle512>>6)&(0x06))|(1));
	unsigned int aaa = (((angle512>>6)&(0x03)));
	h->type 			= BULLET_OODAMA32_00_SIROI+(aaa);/*S P_BULLET*/ /*SP_BIGBULLET*/
	h->callback_mover	= move_bullet_oodama1_aya_yuragi;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(aaa);/*((rand()&(0x06))|(1)) (1)*/
	h->x256 			= src->x256-((h->w128))/*+xoffs256*/;
	h->y256 			= src->y256-((h->h128))/*+yoffs256*/;
	/*data->*/h->base_time_out		= 16*16;//	data->wait_bg	= 10;
//
	OODAMA_OYA_DATA *data;
	data				= mmalloc(sizeof(OODAMA_OYA_DATA));
	h->data 			= data;
	data->add_speed256	= add_speed256;/*0*/
//
	data->ransu512		= ransu512;

//	data->angle512		= (angle512);
//	data->speed256		= speed256;
	regist_vector(h, speed256, angle512);
}

/*---------------------------------------------------------
	���p�̑�e(��)�B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
---------------------------------------------------------*/

void bullet_create_oodama22(
	SPRITE *src,
	int speed256,
	int angle512,
	int ransu512,
	int add_speed256/*, int xoffs256, int yoffs256*/)
{
	angle512			= angle_jikinerai512_auto(player, src, angle512);
	mask512(angle512);
//
	SPRITE *h;
	h					= sprite_add_gu(TAMA_TYPE_OODAMA_08_PNG);	 /* ��e(��) �\������*/
	h->priority 		= PRIORITY_03_ENEMY;
//	unsigned int aaa = (((angle512>>6)&(0x05))|(2));
	unsigned int aaa = (((angle512>>6)&(0x03)));
	h->type 			= BULLET_OODAMA32_00_SIROI+(aaa);/*S P_BULLET*/ /*SP_BIGBULLET*/
	h->callback_mover	= move_bullet_oodama2_standard;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	h->anim_frame		= 0;//(aaa);/*(2) ((rand()&(0x05))|(2))*/
	h->x256 			= src->x256-((h->w128));
	h->y256 			= src->y256-((h->h128));
	OODAMA_OYA_DATA *data;
	data				= mmalloc(sizeof(OODAMA_OYA_DATA));
	h->data 			= data;
	data->add_speed256	= add_speed256;
//
//	data->angle512		= (angle512);
//	data->speed256		= speed256;
	regist_vector(h, speed256, angle512);
}

/*---------------------------------------------------------
	�V��p
	ICE�e
	-------------------------------------------------------
---------------------------------------------------------*/

/*static*/ void bullet_create_aya_ice(SPRITE *src)
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
			h					= sprite_add_gu(TAMA_TYPE_BULLET_JIPPOU32_PNG);
			h->type 			= BULLET_CAP16_04_SIROI;/*S P_BULLET*/ /*SP_BOSS02ICE*/
			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
			h->callback_mover	= enemy_boss02_ice_move;

		//	h->x256 			= src->x256 + ((0==j)?(t256(-19)):(t256(81)));
		//	h->x256 			= src->x256 + ((0==j)?(t256(-64)):(t256(64)));
		{	s16 aaa[4]			=	{t256(-64),t256(64),t256(-32),t256(32)};
			h->x256 			= src->x256 + aaa[(j)];
		}	h->y256 			= src->y256 /*+ t256(135 138)*/;
			/*data->*/h->base_time_out		= 120/*100*/;
//
			BOSS02_ICE_DATA *data;
			data				= mmalloc(sizeof(BOSS02_ICE_DATA));
			h->data 			= data;
//
		//	h->ice_add_angle512 = (j == 0)?(deg_360_to_512(+5)):(deg_360_to_512(-5));
			h->ice_add_angle512 = rand_add_angle-j;
			h->ice_angle512 	= (i<<4)/*deg_360_to_512(90)*/;
//
			data->state 		= 0;
		//	data->next_2nd_state	= KS01; 	/* �o���o���ړ� */
			data->next_2nd_state	= KS02; 	/* �������Ȃ�(���������ړ�) */
			data->speed256		= t256(2.5)+(difficulty<<6);/*t256(4.0);*/	/* (difficulty x 64) */
		}
		rand_add_angle = -rand_add_angle;
	}
}


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
