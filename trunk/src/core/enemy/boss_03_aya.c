
#include "bullet_object.h"

/*---------------------------------------------------------
	�˖��� ��
	�V��
	-------------------------------------------------------
	ToDo:
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	�����Ŏv����������2�`�Ԃ̍U���p�^�[�����J�I�X�ɂȂ��Ă��܂����B
	�����ƒ��J�ɍ�蒼�����������������B
	[*** 090114 	�V��̑��x���Փx���Ƃɕς��Ă݂��B
---------------------------------------------------------*/

typedef struct /*_boss02_data*/
{
	BOSS_BASE boss_base;
//------------ �ړ��֘A
	int state1; 		/* �`�ԃX�e�[�g */	/* �e���X�e�[�g */
	int aya_angle512;	/* ���̊p�x */

	int wait1;			/* �ړ��҂� */
	int animation_wait; 		/* �A�j���[�V�����p */

	int n_wait3;
	int aya_speed;		/*	��x�ʑ��x */
	int ice_number; 	/*	ICE�e���ԍ� */
//
} BOSS02_DATA;

/*---------------------------------------------------------
	[�X�y�J�V�X�e�����Ɉړ��\��]	�X�y�J���j��A�C�e���o��
---------------------------------------------------------*/

static void aya_put_items(SPRITE *src)
{
	common_boss_put_items(src);
//
	bakuhatsu_add_circle(/*d*/src, 1);
//
	{
		BOSS02_DATA *data=(BOSS02_DATA *)src->data;
		data->ice_number++;
	//	if (DT02 < data->ice_number)	{	data->ice_number = DT02;	}
		data->state1	= 0;
		data->wait1 	= 40;
	//	player_add_score(data->boss_base.score);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
//#define AYA_ANIME_LEFT_00 (0)
//#define AYA_ANIME_CENTER_04 (4)
//#define AYA_ANIME_RIGHT_08	(8)

static void sakuya_wait_state(SPRITE *src/*, int next_set_state*/)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	if (data->wait1 > 0)
	{
		data->wait1 -= 1/*fps_fa ctor*/;
		data->animation_wait += 1/*fps_fa ctor*/;
		if (7 < data->animation_wait)
		{
			data->animation_wait = 0;
		//	if (src->anim_frame<AYA_ANIME_CENTER_04)		{	src->anim_frame++;		}
		//	else if (src->anim_frame>AYA_ANIME_CENTER_04)	{	src->anim_frame--;		}
			u8 aaa = (src->anim_frame & 0x03);
			if (0!=(aaa))
			{
				aaa--;
				src->anim_frame = (src->anim_frame & 0xfc)|(aaa);
			}
		}
	}
	else
	{
		data->state1++/* = next_set_state*/;
/* CCW�̏ꍇ */
		dummy_obj->w128 = src->w128;
		dummy_obj->h128 = src->h128;
		dummy_obj->x256 = src->x256 + (src->w128);	/* �{���͔��e�ʒu�� offset �p */
		dummy_obj->y256 = src->y256 + (src->h128);	/* �{���͔��e�ʒu�� offset �p */

		bullet_create_oodama00(dummy_obj,
			t256(4)+(difficulty<<7),
		//	t256(1.0)+(difficulty<<4),	/*�d�l�ύX*/	/*t256(4)+(difficulty<<7)*/
			ANGLE_JIKI_NERAI_DAN,
			10,
			0
		);
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime08(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
			u8 aaa = (src->anim_frame & 0x03);
		//	if (src->anim_frame<AYA_ANIME_RIGHT_08)
			if (3!=aaa)
			{
				data->animation_wait += 1/*fps_fa ctor*/;
				if (7 < data->animation_wait)
				{
					data->animation_wait=0;
				//	src->anim_frame++;
					aaa++;
					src->anim_frame = /*(src->anim_frame & 0xfc)*/(0x00)|(aaa);
				}
			}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime00(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
			u8 aaa = (src->anim_frame & 0x03);
		//	if (src->anim_frame>AYA_ANIME_LEFT_00)
			if (3!=aaa)
			{
				data->animation_wait += 1/*fps_fa ctor*/;
				if (7 < data->animation_wait)
				{
					data->animation_wait=0;
				//	src->anim_frame--;
					aaa++;
					src->anim_frame = /*(src->anim_frame & 0xfc)*/(0x10)|(aaa);
				}
			}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
};
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
	SS05,
	SS06,
	SS07,
	SS08,
	SS09,
	SS10,
	SS11,
	SS12,
};

/*---------------------------------------------------------

---------------------------------------------------------*/

static void bullet_create_aya_momiji(SPRITE *src)
{
#if 0
/* CW�̏ꍇ */
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512((30*(3-2))) );
	bullet_create_momiji_dan(src, t256(4.0), deg_360_to_512((30*(3+0))) );
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512((30*(3+2))) );
#else
/* CCW�̏ꍇ */
//	bullet_create_momiji_dan(src, t256(3.5), -deg_360_to_512CCW((30*(3-2))) );
//	bullet_create_momiji_dan(src, t256(4.0), -deg_360_to_512CCW((30*(3+0))) );
//	bullet_create_momiji_dan(src, t256(3.5), -deg_360_to_512CCW((30*(3+2))) );
//2016201
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512CCW(360+90-(30*(3-2))) );
	bullet_create_momiji_dan(src, t256(4.0), deg_360_to_512CCW(360+90-(30*(3+0))) );
	bullet_create_momiji_dan(src, t256(3.5), deg_360_to_512CCW(360+90-(30*(3+2))) );
#endif
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

/*---------------------------------------------------------

---------------------------------------------------------*/

extern void bullet_create_aya_ice(SPRITE *src);

static int bullet_create_oodama3(SPRITE *src, /*�d�l�ύX*/int speed256, int ransu512)
{	int angle_jikinerai_512;
//	angle_jikinerai_512 = at an_512(player->y256-src->y256,player->x256-src->x256);
	angle_jikinerai_512 = angle_jikinerai512(player,src);
/* CCW�̏ꍇ */
		dummy_obj->w128 = src->w128;
		dummy_obj->h128 = src->h128;
		dummy_obj->x256 = src->x256 + ((src->w128*86*2)>>8);	/* �{���͔��e�ʒu�� offset �p */
		dummy_obj->y256 = src->y256 + (src->h128);	/* �{���͔��e�ʒu�� offset �p */
//		dummy_obj->tmp_angleCCW512 = (0);/* ������ */
	bullet_create_oodama00(dummy_obj, speed256, angle_jikinerai_512+deg_360_to_512CCW((30)), ransu512, 0 ); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama00(dummy_obj, speed256, angle_jikinerai_512 					   , ransu512, 0 ); /*t256(s->w/3), t256(s->h/2)*/
	bullet_create_oodama00(dummy_obj, speed256, angle_jikinerai_512-deg_360_to_512CCW((30)), ransu512, 0 ); /*t256(s->w/3), t256(s->h/2)*/
	return (angle_jikinerai_512);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void bullet_create_kougeki_01(SPRITE *src)
{	BOSS02_DATA *data=(BOSS02_DATA *)src->data;
//	if (DT00==data->ice_number)
//	if (2 < ((data->ice_number+1) & 0x03/*DT00*/))
	{
		const unsigned char aaa_bbb[8] =
		{
			0,0,1,0, 0,1,0,1,
		};
		if (0 != aaa_bbb[(data->ice_number&7)/*+8*/])
		{	bullet_create_aya_ice(src);}
	}
};
static void bullet_create_kougeki_02(SPRITE *src)
{	BOSS02_DATA *data=(BOSS02_DATA *)src->data;
//	if (DT01==data->ice_number)
	if (0!=(data->ice_number & 1/*DT01*/))
	{	bullet_create_aya_momiji(src);}
};
/*
000 0 --
001 1
010 2
011 3 --
100 4

*/

/*---------------------------------------------------------
	�����Ȃ񂾂�������Ȃ��Ȃ�̂ŁA���O�����悤
---------------------------------------------------------*/

/* ��邳��̗U���|�C���g���W */

#define SAKUYA_POINT_X_LEFT_OUT 	(t256(GAME_WIDTH*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256(GAME_WIDTH*1/4))
#define SAKUYA_POINT_X_MID			(t256(GAME_WIDTH*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256(GAME_WIDTH*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256(GAME_WIDTH*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(25/*16*/))


#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

//(t256(GAME_WIDTH)-((src->w128+src->w128)))

/*---------------------------------------------------------
	�ړ��p�^�[��
---------------------------------------------------------*/
extern const u8 alice_danmaku_table[16];
/*static*/ void aya_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait -= (1)/*fps_fa ctor*/;
//
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			data->state1++;
			data->state1 &= (8-1);
			{
				enum
				{
					dummy_PPP_00_VX = 0,	/* x �x�N�g���ړ��� */
					PPP_01_VY,				/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,	/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
				};
				s8 ppp[8][4] =
				{
					{( 2),(-1),(100),( 1),},	/*�E���*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{( 2),( 1),(100),( 1),},	/*�E����*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
					{(-2),(-1),(100),( 1),},	/*�����*/
					{( 0),( 0),( 50),( 2),},	/*wait*/
					{(-2),( 1),(100),( 1),},	/*������*/
					{( 0),( 0),( 10),( 0),},	/*wait*/
				};
//				data->vx	= ppp[data->state1][PPP_00_VX];
//				data->vy	= ppp[data->state1][PPP_01_VY];
//				src->vx256	= (0);
//				src->vy256	= ((ppp[data->state1][PPP_01_VY])<<4);
				my_wait 	= ppp[data->state1][PPP_02_WAIT_DIV_2]; 	/* 50*4 60 �ړ��� */
				my_wait 	+= my_wait;
				if (0!=ppp[data->state1][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[data->state1][PPP_03_IS_RESET_ANIME])	/* �U���A�j���[�V���� */
					{
//						alice_anime_count = 48;
						data->boss_base.danmaku_test++;
						data->boss_base.danmaku_test &= 0x07;
						data->boss_base.danmaku_type		= alice_danmaku_table[data->boss_base.danmaku_test];	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
						data->boss_base.danmaku_time_out	= (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
					}
//					vvv256=1;
				}
			}
		}
	}
//	src->vx256 = (data->vx)*vvv256;
//	src->vy256 = (data->vy)*vvv256;
}



/*---------------------------------------------------------
	�ړ��p�^�[��1(�ᑬ�ړ�)
---------------------------------------------------------*/

/*static*/ void aya_04_keitai(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	switch (data->state1)
	{
	case ST_00: /* y-pos erreicht: rundumschuss */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		bullet_create_kougeki_01(src);
		bullet_create_kougeki_02(src);
		data->wait1 = 20;
		data->state1++/* = ST_01*/;
		break;

	case ST_01: /* waiting */
		sakuya_wait_state(src/*,ST_02*/);
		break;

	case ST_02: /* nach links bis zum rand */
		if (src->x256 >= t256(0))
		{
			src->x256 -= t256(2)/**fps_fa ctor*/;
			sakuya_anime00(src);
		}
		else
		{
			data->state1++/* = ST_04*/;
		}
		break;
	case ST_03: /*	*/
		{
			data->state1++/* = ST_04*/;
			data->wait1 = 20;
			bullet_create_kougeki_02(src);
		}
		break;


	case ST_04: /* waiting */
		sakuya_wait_state(src/*,ST_05*/);
		break;

	case ST_05: /* nach rechts bis zur mitte */
		if (src->x256 < BOSS_XP256)
		{
			src->x256 += t256(2)/**fps_fa ctor*/;
			sakuya_anime08(src);
		}
		else
		{
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			data->wait1 = 100;
			data->state1++/* = ST_06*/;
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_06: /* waiting */
		sakuya_wait_state(src/*,ST_07*/);
		break;

	case ST_07:
		bullet_create_kougeki_01(src);
		data->wait1 = 100;
		data->state1++/* = ST_08*/;
		break;

	case ST_08: /* waiting */
		sakuya_wait_state(src/*,ST_09*/);
		break;

	case ST_09: /* nach rechts bis zum rand */
		if (src->x256 < (t256(GAME_WIDTH)-((src->w128+src->w128))))
		{
			src->x256 += t256(2)/**fps_fa ctor*/;
			sakuya_anime08(src);
		}
		else
		{
			data->wait1 = 20;
			data->state1++/* = ST_10*/;
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_10: /* waiting */
		sakuya_wait_state(src/*,ST_11*/);
		break;

	case ST_11: /* nach links bis zur mitte */
		if (src->x256 > BOSS_XP256)
		{
			src->x256 -= t256(2)/**fps_fa ctor*/;
			sakuya_anime00(src);
		}
		else
		{	data->state1++/* = ST_00*/; }
		break;
	case ST_12: /*	*/
		{
			data->state1 = ST_00;
		}
		break;
	}
}
/*---------------------------------------------------------
	�ړ��U���p�^�[��2(�����ړ�)
---------------------------------------------------------*/

/*static*/ void aya_05_keitai(SPRITE *src)
{
	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	switch (data->state1)
	{
	case SS00:	/* �s��:�����ʒu���̎擾->SS01�� */
		dummy_obj->x256 = t256(GAME_WIDTH/2);
		dummy_obj->y256 = t256(20);
	//	data->aya_angle512 = at an_512(dummy_obj->y256-src->y256,dummy_obj->x256-src->x256-((src->w128+src->w128)));
		data->aya_angle512 = angle_jikinerai512(dummy_obj,src);
		data->state1++/* = SS01*/;
		break;

	case SS01:	/* �s��:�����ʒu�֖߂�->SS02�� */
		if (src->y256 < t256(30))
		{
			data->state1++/* = SS02*/;
			data->wait1 += 10+(3-difficulty)*10;
/*
[0] 1  2 [0] 1	2 [0] 1  2 [0] 1  2 				/ 3��Ɉ��
<0> 1  2  3 [0] 1  2  3 [0] 1  2  3 				/ 4��Ɉ��
<0> 1  2  3  4	5  6  7  8	9 10 11 				/ 12��Ɉ��ȊO
[-][-][-][-][+][+][+][+][-][-][-][-][+][+][+][+]
[0] 1  2 [0] 1	2 [0] 1 [0] 1  2 [0] 1	2 [0] 1 	/ 3��Ɉ��
 0 [1] 2  3  0 [1] 2  3  0 [1] 2  3  0 [1] 2  3 	/ 4��Ɉ��
 0	1  2  3  4	5  6  7  8	9 10 11 12 13 14 15 	/ 16��
*/
			{	static const Uint8 n_wait3_tbl[8/*16*/] =
				{ 1,2,0,1, 0,2,1,0, /*1,2,0,1, 0,2,1,0,*/ };
				/* 3��Ɉ��   (0 == ((data->n_wait3)%3)) */
				data->n_wait3++;
				data->n_wait3 &= 0x07;
				if (n_wait3_tbl[data->n_wait3]&0x01)
				{
					/* �e�ɓœh���Ă���ݒ� */
					bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 8);	/*�Ȃ�ׂ����ʉ�*/
				}
				/* 4��Ɉ��   (0 == ((data->n_wait3)%4)) && */
				/* �A��12��Ɉ��ȊO  (0 != ((data->n_wait3)%12)) */
				if (n_wait3_tbl[data->n_wait3]&0x02)
				{
					/* �e�ɓœh���Ă���ݒ� */
					bullet_create_n_way_dan_sa_type(src, t256(4), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_00_AOI*/, 11); /*�Ȃ�ׂ����ʉ�*/
					bullet_create_n_way_dan_sa_type(src, t256(3), ANGLE_JIKI_NERAI_DAN, (int)(512/24), BULLET_KOME_01_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_02_MIDORI*/, 11); /*�Ȃ�ׂ����ʉ�*/
				}
			}
		}
		else
		{
#if 1
/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->aya_angle512))*t256(6))>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->aya_angle512))*t256(6))>>8)/**fps_fa ctor*/;
#endif
			data->state1--/* = SS00*/;
			#if 1/*�������݊�*/
			data->n_wait3--;
			#endif
		}
		break;

	case SS02:	/* �����ʒu:��e3��->SS03, SS04, SS05 */
		if (data->wait1<0)
		{	int angle_jikinerai_512;
			angle_jikinerai_512 = bullet_create_oodama3(src,	/*�d�l�ύX*/t256(/*1.5*/5.0/*5.0*/), 1/*ransu512*/);
		//	data->state1 = (ra_nd()%3)+SS03;
			data->state1 = (((ra_nd()&(256-1))*3)>>8)+SS03;
			if (data->state1 == SS05)
			{
				data->aya_angle512 = angle_jikinerai_512;
			}
			else
			{
				src->y256 += t256(6)/**fps_fa ctor*/;
			}
		}
		else
		{
			data->wait1 -= 1/*fps_fa ctor*/;
		}
		break;

	case SS03:	/* ���ړ���->SS06�� */
		if (src->x256 >= 0)
		{
			src->x256 -= /*(t256(4)+(difficulty<<8))*/(data->aya_speed)/**fps_fa ctor*/;
			sakuya_anime00(src);
		}
		else
		{
			src->x256 += t256(1);		// [***090114		�ǉ�
			data->wait1 = 10;
			data->state1 = SS06;
		}
		break;

	case SS04:	/* �E�ړ���->SS08�� */
		if (src->x256 < (t256(GAME_WIDTH)-((src->w128+src->w128))))
		{
			src->x256 += /*(t256(4)+(difficulty<<8))*/(data->aya_speed)/**fps_fa ctor*/;
			sakuya_anime08(src);
		}
		else
		{
			src->x256 -= t256(1);		// [***090114		�ǉ�
			data->wait1 = 10;
			data->state1 = SS08;
		}
		break;

	case SS05:	/* �v���C���[�ʒu�t�߈ړ���->SS10�� */
		/* aya�̉ғ��͈� */
		#if 0
		/* ��ʂ���ݏo���ꍇ�ɖ߂�(�{�X�����肠��ɂ����̂ŕ������ɓ��܂�č���) */
		if (((src->x256 < t256(0))||(src->y256 < t256(0)))||
			((src->x256+((s->w128+src->w128)) > t256(GAME_WIDTH))||
		//	 (src->y256+((s->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((s->h128+src->h128)) > t256(220))))/*(���܂�Ȃ�)*/
		#else
		/* �ғ��͈͂���ݏo���ꍇ�ɖ߂�(�{�X�����肠��ɂ����̂ŕ������ɓ��܂�č���) */
		if (((src->x256 < t256(0))||(src->y256 < t256(0)))||
			((src->x256+((src->w128+src->w128)) > t256(GAME_WIDTH))||
		//	 (src->y256+((src->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((src->h128+src->h128)) > (t256(128)+(difficulty<<(8+5)))  )))	/* difficulty x 32 */
		/*
			easy:	128(���܂�Ȃ�)
			normal: 160(���܂�Ȃ�)
			hard:	192(���܂�Ȃ�)
			luna:	224(���܂��)
		*/
		#endif
		{
			data->state1 = SS10;
		}
		else
		{
#if 1
/* CCW�̏ꍇ */
			src->x256 += ((sin512((data->aya_angle512))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
			src->y256 += ((cos512((data->aya_angle512))*(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)>>8)/**fps_fa ctor*/;
#endif
		}
		break;
//--------
	case SS06:	/* ��:�ҋ@->SS07�� */
		sakuya_wait_state(src/*,SS07*/);
		break;

	case SS07:	/* ��:�E��]�U��->SS00, SS11 */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		if (0==(ra_nd()&(2-1)))
		{
			data->state1 = SS00;/*	data->n_wait3++;*/
		}
		else
		{	data->state1 = SS11;}
		break;
//--------
	case SS08:	/* �E:�ҋ@->SS09�� */
		sakuya_wait_state(src/*,SS09*/);
		break;

	case SS09:	/* �E:����]�U��->�ЂƂ܂�SS00�� */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add2(src);
		#endif //(1==USE_CONTROLLER)
		data->state1 = SS00;/*	data->n_wait3++;*/
		break;
//--------
	case SS10:	/* �v���C���[�ʒu�t��:��e3��->�ЂƂ܂�SS00�� */
		bullet_create_oodama3(src,	/*�d�l�ύX*/t256(/*1.3*/3.0/*3.0*/), 20/*ransu512*/);	/* ��e�̒ǉ� */
		data->state1 = SS00;/*	data->n_wait3++;*/
		break;

	case SS11:	/* ��:�O��->SS12 */
		if (src->y256 > t256(GAME_HEIGHT/3) )
		{	data->state1++/* = SS12*/;}
		else
		{	src->y256 += (t256(1)+(data->aya_speed)/*(t256(4)+(difficulty<<8))*/)/**fps_fa ctor*/;}
		break;

	case SS12:
		{
			/* �e�ɓœh���Ă���ݒ� */
#if 0
/* CW�̏ꍇ */
//			bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, deg_360_to_512((30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*�Ȃ�ׂ����ʉ�*/
#else
/* CCW�̏ꍇ */
//			bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, -deg_360_to_512CCW((30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),	 5);	/*�Ȃ�ׂ����ʉ�*/
			bullet_create_n_way_dan_sa_type(src, t256(5), ANGLE_JIKI_NERAI_DAN, deg_360_to_512CCW(360-(30)), BULLET_KOME_01_AOI+(ra_nd()&0x0f),  5);	/*�Ȃ�ׂ����ʉ�*/
#endif
		//	int angle_jikinerai_512;
		//	angle_jikinerai_512=at an_512(player->y256-src->y256,player->x256-src->x256);
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512+deg_360_to_512(60),	2/*state_hi*/, 0/*state_lo*/);		//����Ȃ��e�B�B������������������
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512+deg_360_to_512(30),	1/*state_hi*/, 0/*state_lo*/);
		//	bullet_create_aka_maru_jikinerai(src, t256(5));
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512-deg_360_to_512(30), -1/*state_hi*/, 0/*state_lo*/);
		//	bullet_create_gg_dan(src, t256(5), angle_jikinerai_512-deg_360_to_512(60), -2/*state_hi*/, 0/*state_lo*/);
		}
		data->state1 = SS00;/*	data->n_wait3++;*/
		break;
	}
}


/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

static void move_aya(SPRITE *src)
{
	/* �X�y�J�o�^ */
			if (0/*off*/==spell_card_mode)
			{
				regist_spell_card222(src);
			}
	spell_card_generator222(src);	/* �X�y�J���� */
	#if 1/* [�X�y�J�V�X�e�����Ɉړ��\��] */
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	//boss_move96(src);
	#endif
//
	boss_effect(src);			/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
//
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
	//	sakuya								= spr ite_add_file 0("boss/aya.png"/*"boss02_v2.png"*/,9,PRIORITY_03_ENEMY, 0); s->anim_speed=0;
	//	sakuya->base_health 				= (difficulty<<10/**1024*/)+3071+1024;
	//	sakuya->base_health 				= (1024-1);
	//	sakuya->base_health 				= (1024-1)*((difficulty)+3);
	//	sakuya->base_score					= score(2000)+score(1500)*difficulty;
	//	sakuya->base_health 				= ((1024)-1)*((difficulty)+3);/*test*/

void add_boss_aya(STAGE_DATA *l)/*int lv*/
{
	boss_bgm_mode		= (l->user_y);
//
//----[BOSS]
		SPRITE *sakuya;
		sakuya								= sprite_add_res(BASE_BOSS_AYA_PNG);
		sakuya->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		sakuya->yx_anim_frame				= 0x00/*AYA_ANIME_CENTER_04*/;
		sakuya->type						= SP_BOSS/*SP_BOSS02*/;
		sakuya->callback_mover				= move_aya;
		sakuya->callback_loser				= aya_put_items;
	//
	//	sakuya->base_health 				= ((1024)-1)*4;/*test*/
		sakuya->base_health 				= ((1024)-1)*6;/*test*/
		sakuya->base_score					= adjust_score_by_difficulty(score(2000000));	/* 200�� */
	//------------ �X�y�J�֘A
		spell_card_number					= SPELL_CARD_00_aya_000;
		spell_card_max						= SPELL_CARD_19_aya_jjj;
		spell_card_boss_init_regist(sakuya);
	{
		BOSS02_DATA *data;
		data								= mmalloc(sizeof(BOSS02_DATA));
		sakuya->data						= data;
		data->state1						= (0)/*ST_13*/;/*0*/
		data->wait1 						= 50;
		data->animation_wait				= 0;
		data->n_wait3						= 0/*1*/;
		data->aya_speed 					= t256(4)+(difficulty<<8);
		data->ice_number					= (0);
		//
		#if 1
	//------------ �e���֘A
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		data->boss_base.danmaku_test		= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	}
	//------------ ���ꏉ����
	#if 0
	/*	common_boss_init(); ����̕K�v������*/
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256(0);
		boss_clip_min.y256	= t256(0);
		boss_clip_max.x256	= t256(GAME_WIDTH);
		boss_clip_max.y256	= (t256(128)+(difficulty<<(8+5)));	/*t256(96)*/
	}
		/*
		0	easy:	128(���܂�Ȃ�)
		1	normal: 160(���܂�Ȃ�)
		2	hard:	192(���܂�Ȃ�)
		3	luna:	224(���܂��)
		*/
	#endif
}
