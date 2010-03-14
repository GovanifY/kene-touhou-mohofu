
#include "bullet_object.h"
/*static*/extern void add_zako_aya_doll(SPRITE *src);/* ���e */

/*---------------------------------------------------------
	�˖��� ��
	Syameimaru Aya.
	-------------------------------------------------------
	�V��
	ToDo:
	�{�X�^�C�}�[�Ή���
	-------------------------------------------------------
	�����Ŏv����������2�`�Ԃ̍U���p�^�[�����J�I�X�ɂȂ��Ă��܂����B
	�����ƒ��J�ɍ�蒼�����������������B
	[*** 090114 	�V��̑��x���Փx���Ƃɕς��Ă݂��B
//
	�������̗�����e(���p�̗h�炮��e)�ɂ��Ď���(231)
	������e�� (easy�{�����e) �ӂ�܂łȂ�ʔ����̂ł����A
	���s���낵�����ʁA
	hard, lunatic ������̒e���V���[�����Ƃ��āA
	�e���x���������Ȃ��̂ŁA�������ۂ��Ȃ�Ȃ��ł��B
	�����������R�Ŏ~�߂܂����B
---------------------------------------------------------*/

//	int st ate1;		/* �`�ԃX�e�[�g */	/* �e���X�e�[�g */
#define aya_angle512			user_data00 	/* ���̊p�x */
//
#define wait1					user_data01 	/* �ړ��҂� */
#define animation_wait			user_data02 	/* �A�j���[�V�����p */
//

#define aya_speed				user_data03 	/* ��x�ʑ��x */
#define ice_number				user_data04 	/* ICE�e���ԍ� */


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
		src->ice_number++;
	//	if (DT02 < src->ice_number) {	src->ice_number = DT02; }
		src->boss_base_state001 = 0;
		src->wait1	= 40;
	//	player_add_score(src->boss_base_score);
	}
}

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
	return (atan_512((p->y256-t->y256), (p->x256-t->x256)));
	/* �����e�̏ꍇ�Ɏ��@�_��(�ȗ���) �e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ� */
	#endif
}


/*---------------------------------------------------------
	�V��p�̑�e(��)�B�����ŉ������ȓ���������B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
	���x�́A[���x�ɔ�Ⴗ����̂̈Ӗ��̗ǂ�����Ȃ��Œ�l]����A
	�P���ɒe���ւƎd�l�ύX�����B
---------------------------------------------------------*/

//extern void bullet_create_aya_ice(SPRITE *src);/*�d�l�ύX*/
/**/
static int bullet_create_oodama3(SPRITE *src, int speed256)
{
	int angle_jikinerai_512;
	angle_jikinerai_512 = angle_jikinerai512(player,src);
	int angle512;
	for (angle512=(angle_jikinerai_512); angle512<(angle_jikinerai_512+512); angle512+=deg_360_to_512CCW((30)) )/* 30�x�Â��(12==360/30������12����) */
	{
	//	unsigned int aaa = (((angle512>>6)&(0x06))|(1));
		unsigned int aaa = (((angle512>>6)&(0x03)));
	//
		bullet_create_offset_dan_type000(
			src,
			speed256,
			angle512,
			(BULLET_OODAMA32_00_SIROI+(aaa)) 	/* �e�O�� */	/*bullet_obj_type*/
		);
	}
	return (angle_jikinerai_512);
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void sakuya_wait_state(SPRITE *src/*, int next_set_state*/)
{

	if (0 < src->wait1)
	{
		src->wait1--;/*fps_factor*/
		src->animation_wait++;/*fps_factor*/
		if (7 < src->animation_wait)
		{
			src->animation_wait = 0;
		//	if (src->an im_frame<AYA_ANIME_CENTER_04)		{	src->an im_frame++; 	}
		//	else if (src->an im_frame>AYA_ANIME_CENTER_04)	{	src->an im_frame--; 	}
			u8 aaa = (src->type/*an im_frame*/ & 0x03);
			if (0!=(aaa))
			{
				aaa--;
				src->type/*an im_frame*/ = (BOSS_00_BOSS11)+((src->type/*an im_frame*/ & 0xfc)|(aaa));
			}
		}
	}
	else
	{
		src->boss_base_state001++/* = next_set_state*/;
		bullet_create_oodama3(src, /*�d�l�ύX*/(t256(4.0)+(difficulty<<7))/*, 10*/);
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime08(SPRITE *src)
{

			u8 aaa = (src->type/*an im_frame*/ & 0x03);
		//	if (src->an im_frame<AYA_ANIME_RIGHT_08)
			if (3!=aaa)
			{
				src->animation_wait++;/*fps_factor*/
				if (7 < src->animation_wait)
				{
					src->animation_wait=0;
				//	src->an im_frame++;
					aaa++;
					src->type/*an im_frame*/ = (BOSS_00_BOSS11)+((0x00)|(aaa));/*(src->an im_frame & 0xfc)*/
				}
			}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void sakuya_anime00(SPRITE *src)
{

			u8 aaa = (src->type/*an im_frame*/ & 0x03);
		//	if (src->an im_frame>AYA_ANIME_LEFT_00)
			if (3!=aaa)
			{
				src->animation_wait++;/*fps_factor*/
				if (7 < src->animation_wait)
				{
					src->animation_wait=0;
				//	src->an im_frame--;
					aaa++;
					src->type/*an im_frame*/ = (BOSS_00_BOSS11)+((/*0x10*/0x04)|(aaa));/*(src->an im_frame & 0xfc)*/
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

static void bullet_create_kougeki_01(SPRITE *src)
{
//	if (DT00==data->ice_number)
//	if (2 < ((data->ice_number+1) & 0x03/*DT00*/))
	{
		const unsigned char aaa_bbb[8] =
		{
			0,0,1,0, 0,1,0,1,
		};
		if (0 != aaa_bbb[(src->ice_number&7)/*+8*/])
		{
			//bullet_create_aya_ice(src);
			/* ���ɔ�����ۂ��e���������Ă݂�e�X�g */
			src->boss_base_danmaku_type 	= DANMAKU_12_aya_merin_test;					/* �S�e�����Z�b�g */
			src->boss_base_danmaku_time_out = (DANMAKU_01_SET_TIME+DANMAKU_01_SET_TIME);	/* �S�e���̔������� x 2 */
		}
	}
};
static void bullet_create_kougeki_02(SPRITE *src)
{
//	if (DT01==src->ice_number)
	if (0!=(src->ice_number & 1/*DT01*/))
	{
		add_zako_aya_doll(src);
	}
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

#define SAKUYA_POINT_X_LEFT_OUT 	(t256((GAME_WIDTH-50)*0/4))
#define SAKUYA_POINT_X_LEFT_MID 	(t256((GAME_WIDTH-50)*1/4))
#define SAKUYA_POINT_X_MID			(t256((GAME_WIDTH-50)*2/4))/*-src->w/2*/
#define SAKUYA_POINT_X_RIGHT_MID	(t256((GAME_WIDTH-50)*3/4))
#define SAKUYA_POINT_X_RIGHT_OUT	(t256((GAME_WIDTH-50)*4/4))/*-src->w  */
#define SAKUYA_POINT_X_MARGIN		(t256(25/*16*/))


#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT_OUT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT_OUT-SAKUYA_POINT_X_MARGIN)

//(t256(GAME_WIDTH)-((src->w128+src->w128)))


/*---------------------------------------------------------
	�ړ��p�^�[��(�ʏ�U��)
---------------------------------------------------------*/
#if 0
//extern const u8 aya_danmaku_table[16];
/*static*/static const u8 aya_danmaku_table[4/*16*/] =
{
/*	0:easy						1:normal					2:hard						3:lunatic */
	DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_08_rumia,	/* 1��� */
//	DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_08_rumia,	/* 2��� */
//	DANMAKU_08_rumia,			DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_08_rumia,	/* 3��� */
//	DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_10_cirno,			DANMAKU_08_rumia,	/* 4��� */
};
#endif
/*static*/ void aya_01_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//

	{
		if (0 >= my_wait)
		{	/* �ړ����������߂� */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
			//		dummy_PPP_00_VX = 0,		/* x �x�N�g���ړ��� */
			//		dummy_PPP_01_VY,			/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2 = 0,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,		/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
					PPP_04_MAX/* �ő區�ڐ� */
				};
				static const s8 ppp[8][PPP_04_MAX] =
				{
					{/*( 2),(-1),*/(100),( 1),},	/*�E���*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*( 2),( 1),*/(100),( 1),},	/*�E����*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
					{/*(-2),(-1),*/(100),( 1),},	/*�����*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*(-2),( 1),*/(100),( 1),},	/*������*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
				};
//				data->vx	= ppp[src->boss_base_state001][PPP_00_VX];
//				data->vy	= ppp[src->boss_base_state001][PPP_01_VY];
//				src->vx256	= (0);
//				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<4);
				my_wait 	= ppp[src->boss_base_state001][PPP_02_WAIT_DIV_2];	/* 50*4 60 �ړ��� */
				my_wait 	+= my_wait;
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* �U���A�j���[�V���� */
					{
//						alice_anime_count = 48;
					//	src->boss_base_danmaku_test &= 0x0f;
						src->boss_base_danmaku_type 	= DANMAKU_08_rumia;//aya_danmaku_table[src->boss_base_danmaku_test];	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
					//	src->boss_base_danmaku_test += 4;
						src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);	/* �S�e���̔������� x 60 */
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
	�ړ��p�^�[��2(�ؕ��u�F�؈�ࣁv�̃e�X�g)
---------------------------------------------------------*/
#if 0
//extern const u8 aya_danmaku_table[16];
/*static*/static const u8 aya_danmaku_table[16] =
{
/*	0:easy						1:normal					2:hard						3:lunatic */
	DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_08_rumia,			DANMAKU_0a_houka_kenran,	/* 1��� */
	DANMAKU_10_cirno,			DANMAKU_08_rumia,			DANMAKU_0a_houka_kenran,	DANMAKU_08_rumia,			/* 2��� */
	DANMAKU_08_rumia,			DANMAKU_10_cirno,			DANMAKU_10_cirno,			DANMAKU_0a_houka_kenran,	/* 3��� */
	DANMAKU_0a_houka_kenran,	DANMAKU_0a_houka_kenran,	DANMAKU_0a_houka_kenran,	DANMAKU_0a_houka_kenran,	/* 4��� */
};
#endif
/*static*/ void aya_02_keitai(SPRITE *src)
{
	static int my_wait;
	my_wait--;/*fps_factor*/
//

	{
		if (my_wait <= 0)
		{	/* �ړ����������߂� */
			src->boss_base_state001++;
			src->boss_base_state001 &= (8-1);
			{
				enum
				{
			//		dummy_PPP_00_VX = 0,		/* x �x�N�g���ړ��� */
			//		dummy_PPP_01_VY,			/* y �x�N�g���ړ��� */
					PPP_02_WAIT_DIV_2 = 0,		/* �E�F�C�g�J�E���^�̔����� */
					PPP_03_IS_RESET_ANIME,		/* �A�j���[�V�������Z�b�g 0:���Ȃ� 1:���� 2:����(�e������) */
					PPP_04_MAX/* �ő區�ڐ� */
				};
				static const s8 ppp[8][PPP_04_MAX] =
				{
					{/*( 2),(-1),*/(100),( 1),},	/*�E���*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*( 2),( 1),*/(100),( 1),},	/*�E����*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
					{/*(-2),(-1),*/(100),( 1),},	/*�����*/
					{/*( 0),( 0),*/( 50),( 2),},	/*wait*/
					{/*(-2),( 1),*/(100),( 1),},	/*������*/
					{/*( 0),( 0),*/( 10),( 0),},	/*wait*/
				};
//				data->vx	= ppp[src->boss_base_state001][PPP_00_VX];
//				data->vy	= ppp[src->boss_base_state001][PPP_01_VY];
//				src->vx256	= (0);
//				src->vy256	= ((ppp[src->boss_base_state001][PPP_01_VY])<<4);
				my_wait 	= ppp[src->boss_base_state001][PPP_02_WAIT_DIV_2];	/* 50*4 60 �ړ��� */
				my_wait 	+= my_wait;
				if (0!=ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME])
				{
					if (2==ppp[src->boss_base_state001][PPP_03_IS_RESET_ANIME]) /* �U���A�j���[�V���� */
					{
//						alice_anime_count = 48;
					//	src->boss_base_danmaku_test &= 0x0f;
						src->boss_base_danmaku_type 	= DANMAKU_0a_houka_kenran;//aya_danmaku_table[src->boss_base_danmaku_test]; /*DANMAKU_01*/	/* �S�e�����Z�b�g */
					//	src->boss_base_danmaku_test += 4;
						src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);	/* �S�e���̔������� x 60 */
					}
//					vvv256=1;
				}
			}
		}
	}
//	src->vx256 = (src->vx)*vvv256;
//	src->vy256 = (src->vy)*vvv256;
}


/*---------------------------------------------------------
	�ړ��p�^�[��1(�ᑬ�ړ�)
---------------------------------------------------------*/

/*static*/ void aya_04_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case ST_00: /* y-pos erreicht: rundumschuss */
		{
						src->boss_base_danmaku_time_out = ((0)/*DANMAKU_01_SET_TIME*6*10*/);	/* �e�����~�߂� */
		}
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		bullet_create_kougeki_01(src);
		bullet_create_kougeki_02(src);
		src->wait1 = 20;
		src->boss_base_state001++/* = ST_01*/;
		break;

	case ST_01: /* waiting */
		sakuya_wait_state(src/*,ST_02*/);
		break;

	case ST_02: /* nach links bis zum rand */
		if (src->x256 >= t256(0.0))
		{
			src->x256 -= t256(2.0);/*fps_factor*/
			sakuya_anime00(src);
		}
		else
		{
			src->boss_base_state001++/* = ST_04*/;
		}
		break;
	case ST_03: /*	*/
		{
			src->boss_base_state001++/* = ST_04*/;
			src->wait1 = 20;
			bullet_create_kougeki_02(src);
		}
		break;


	case ST_04: /* waiting */
		sakuya_wait_state(src/*,ST_05*/);
		break;

	case ST_05: /* nach rechts bis zur mitte */
		if (src->x256 < BOSS_XP256)
		{
			src->x256 += t256(2.0);/*fps_factor*/
			sakuya_anime08(src);
		}
		else
		{
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			src->wait1 = 100;
			src->boss_base_state001++;/* = ST_06*/
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_06: /* waiting */
		sakuya_wait_state(src/*,ST_07*/);
		break;

	case ST_07:
		bullet_create_kougeki_01(src);
		src->wait1 = 100;
		src->boss_base_state001++;/* = ST_08*/
		break;

	case ST_08: /* waiting */
		sakuya_wait_state(src/*,ST_09*/);
		break;

	case ST_09: /* nach rechts bis zum rand */
		#if 0/*SDL(�������W)*/
		if (src->x256 < (t256(GAME_WIDTH-50)-((src->w128+src->w128))))
		#else/*Gu(���S���W)*/
		if (src->x256 < t256(GAME_WIDTH-50) )
		#endif
		{
			src->x256 += t256(2.0);/*fps_factor*/
			sakuya_anime08(src);
		}
		else
		{
			src->wait1 = 20;
			src->boss_base_state001++;/* = ST_10*/
			bullet_create_kougeki_02(src);
		}
		break;

	case ST_10: /* waiting */
		sakuya_wait_state(src/*,ST_11*/);
		break;

	case ST_11: /* nach links bis zur mitte */
		if (src->x256 > BOSS_XP256)
		{
			src->x256 -= t256(2.0);/*fps_factor*/
			sakuya_anime00(src);
		}
		else
		{	src->boss_base_state001++;/* = ST_00*/ }
		break;
	case ST_12: /*	*/
		{
			src->boss_base_state001 = ST_00;
		}
		break;
	}
}


/*---------------------------------------------------------
	�ړ��U���p�^�[��2(�����ړ�)
---------------------------------------------------------*/

/*static*/ void aya_05_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case SS00:	/* �s��:�����ʒu���̎擾->SS01�� */
		send1_obj->x256 = t256((GAME_WIDTH-50)/2);
		send1_obj->y256 = t256(20.0);
	//	src->aya_angle512 = at an_512(send1_obj->y256-src->y256,send1_obj->x256-src->x256-((src->w128+src->w128)));
		src->aya_angle512 = angle_jikinerai512(send1_obj, src);
		src->boss_base_state001++/* = SS01*/;
		break;

	case SS01:	/* �s��:�����ʒu�֖߂�->SS02�� */
		if (src->y256 < t256(30.0))
		{
			src->boss_base_state001++/* = SS02*/;
			src->wait1 += 10;//+(3-3/*difficulty*/)*10
			{
				voice_play(VOICE11_BOSS_KIRARIN/*VOICE14_BOSS_KOUGEKI_01*/, TRACK04_TEKIDAN);/*�e�L�g�[*/
			//
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
				/* ���� */
				{
					int aaa;
					aaa = (difficulty<<6);/*src->aya_speed*/
					aaa += (t256(4.5)); 		/* 3.5����easy���������B t256(3.5) */
					int jj;
					for (jj=(0); jj<(8+difficulty); jj++ )
					{
						aaa -= (t256(0.125));
						/* �e�ɓœh���Ă���ݒ� */
						send1_obj->BULLET_REGIST_speed256			=	(aaa);									/* �e�� */
						send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI+(ra_nd()&0x0f);		/* �e�O�� */	/*BULLET_KUNAI12_00_AOI*/
						send1_obj->BULLET_REGIST_n_way				=	(11-jj);								/* [10way] */
						bullet_regist_basic();
					}
				}
			}
		}
		else
		{
#if 1
/* CCW�̏ꍇ */
			src->x256 += ((sin512((src->aya_angle512))*t256(6.0))>>8);/*fps_factor*/
			src->y256 += ((cos512((src->aya_angle512))*t256(6.0))>>8);/*fps_factor*/
#endif
			src->boss_base_state001--/* = SS00*/;
		}
		break;

	case SS02:	/* �����ʒu:��e3��->SS03, SS04, SS05 */
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{	int angle_jikinerai_512;
			angle_jikinerai_512 = bullet_create_oodama3(src, (src->aya_speed)-t256(2.0) );
					/*t256(5.0)*/ /* �e�� */		/*�d�l�ύX*/	// t256(/*1.5*/5.0/*5.0*/)
			//	,1/*ransu512*/
		//	src->boss_base_state001 = (ra_nd()%3)+SS03;
			src->boss_base_state001 = (((ra_nd()&(256-1))*3)>>8)+SS03;
			if (src->boss_base_state001 == SS05)
			{
				src->aya_angle512 = angle_jikinerai_512;
			}
			else
			{
				src->y256 += t256(6.0);/*fps_factor*/
			}
		}
		break;

	case SS03:	/* ���ړ���->SS06�� */
		if (0 <= src->x256)
		{
			src->x256 -= (src->aya_speed);/*fps_factor*/ 	 /*(t256(4.0)+(difficulty<<8))*/
			sakuya_anime00(src);
		}
		else
		{
			src->x256 += t256(1.0); 	// [***090114		�ǉ�
			src->wait1 = 5/*10*/;
			src->boss_base_state001 = SS06;
		}
		break;

	case SS04:	/* �E�ړ���->SS08�� */
		#if 0/*SDL(�������W)*/
		if (src->x256 < (t256((GAME_WIDTH-50))-((src->w128+src->w128))) )
		#else/*Gu(���S���W)*/
		if (src->x256 < t256((GAME_WIDTH-50)) )
		#endif
		{
			src->x256 += (src->aya_speed);/*fps_factor*/	/*(t256(4.0)+(difficulty<<8))*/
			sakuya_anime08(src);
		}
		else
		{
			src->x256 -= t256(1.0); 	// [***090114		�ǉ�
			src->wait1 = 5/*10*/;
			src->boss_base_state001 = SS08;
		}
		break;

	case SS05:	/* �v���C���[�ʒu�t�߈ړ���->SS10�� */
		/* aya�̉ғ��͈� */
		#if 0
		/* ��ʂ���ݏo���ꍇ�ɖ߂�(�{�X�����肠��ɂ����̂ŕ������ɓ��܂�č���) */
		if (((src->x256 < t256(0.0))||(src->y256 < t256(0.0)))||
			((src->x256+((s->w128+src->w128)) > t256((GAME_WIDTH-50)))||
		//	 (src->y256+((s->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((s->h128+src->h128)) > t256(220.0))))/*(���܂�Ȃ�)*/
		#endif
		/* �ғ��͈͂���ݏo���ꍇ�ɖ߂�(�{�X�����肠��ɂ����̂ŕ������ɓ��܂�č���) */
		#if 0/*SDL(�������W)*/
		if (((src->x256 < t256(0.0))||(src->y256 < t256(0.0)))||
			((src->x256+((src->w128+src->w128)) > t256((GAME_WIDTH-50)))||
		//	 (src->y256+((src->h128+src->h128)) > t256(GAME_HEIGHT))))
			 (src->y256+((src->h128+src->h128)) > (t256(128.0)+(difficulty<<(8+4/*5*/)))	))) 	/* difficulty x 16	 32 */
		#else/*Gu(���S���W)*/
		if (((src->x256 < t256(0.0))||(src->y256 < t256(0.0)))||
			((src->x256 > t256((GAME_WIDTH-50)))||
		//	 (src->y256 > t256(GAME_HEIGHT))))
			 (src->y256 > (t256(128.0)+(difficulty<<(8+4/*5*/)))	))) 	/* difficulty x 16	 32 */
		#endif
		/*
			easy:	128(���܂�Ȃ�)
			normal: 160(���܂�Ȃ�)
			hard:	192(���܂�Ȃ�)
			luna:	224(���܂��)
		*/
		{
			src->boss_base_state001 = SS10;
		}
		else
		{
#if 1
/* CCW�̏ꍇ */
			src->x256 += ((sin512((src->aya_angle512))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/
			src->y256 += ((cos512((src->aya_angle512))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/
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
			src->boss_base_state001 = SS00;
		}
		else
		{	src->boss_base_state001 = SS11;}
		break;
//--------
	case SS08:	/* �E:�ҋ@->SS09�� */
		sakuya_wait_state(src/*,SS09*/);
		break;

	case SS09:	/* �E:����]�U��->�ЂƂ܂�SS00�� */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add2(src);
		#endif //(1==USE_CONTROLLER)
		src->boss_base_state001 = SS00;
		break;
//--------
	case SS10:	/* �v���C���[�ʒu�t��:��e3��->�ЂƂ܂�SS00�� */
		bullet_create_oodama3(src, ((src->aya_speed)>>1));
		/* �e�� */	/*t256(3.0)*/	/*�d�l�ύX*/	//t256(/*1.3*/3.0/*3.0*/),
		//	,20/*ransu512*/
		/* ��e�̒ǉ� */
		src->boss_base_state001 = SS00;
		break;

	case SS11:	/* ��:�O��->SS12 */
		if (src->y256 > t256(GAME_HEIGHT/3) )
		{	src->boss_base_state001++;/* = SS12*/}
		else
		{	src->y256 += (t256(1.0)+(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/);/*fps_factor*/}
		break;

	case SS12:
		{
	//		-deg_360_to_512CCW((30)),
		/* CCW�̏ꍇ */
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			send1_obj->BULLET_REGIST_speed256			=	(src->aya_speed)-t256(2.0); 			/* �e�� */	/*t256(5.0)*/
			send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
			send1_obj->BULLET_REGIST_div_angle512		=	deg_360_to_512CCW(360-(30));
			send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_KOME_01_AOI+(ra_nd()&0x0f);		/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
			send1_obj->BULLET_REGIST_n_way				=	(5) ;									/* [5way] */
			bullet_regist_basic();
		}
		src->boss_base_state001 = SS00;
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

void add_boss_aya(STAGE_DATA *l)
{
	//----[BOSS]
	SPRITE *h;
	#if (0==USE_BOSS_COMMON_MALLOC)
	h								= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	#else
	h								= pd_boss;/*�P��{�l*/
	#endif
	h->flags						|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	h->type 						= BOSS_00_BOSS11;
	h->callback_mover				= move_aya;
	h->callback_loser				= aya_put_items;
	//
	h->base_score					= adjust_score_by_difficulty(score(2000000));	/* 200�� */
	//------------ �X�y�J�֘A
	spell_card_number					= SPELL_CARD_00_aya_000;
	spell_card_max						= SPELL_CARD_19_aya_jjj;
	#if (0==USE_BOSS_COMMON_MALLOC)
	spell_card_boss_init_regist(h);
	#else
	spell_card_boss_init_regist_void();
	#endif
	{
	h->boss_base_state001			= (0)/*ST_13*/;/*0*/
	h->wait1						= 50;
	h->animation_wait				= 0;
	h->aya_speed					= t256(4.0)+(difficulty<<8);
	h->ice_number					= (0);
	}
	//------------ ���ꏉ����
	#if 0
	/*	common_boss_init(); ����̕K�v������*/
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256(0.0);
		boss_clip_min.y256	= t256(0.0);
		boss_clip_max.x256	= t256((GAME_WIDTH-50));
		/*
		0	easy:	128 = 128+(32x0) (���܂�Ȃ�)
		1	normal: 160 = 128+(32x1) (���܂�Ȃ�)
		2	hard:	192 = 128+(32x2) (���܂�Ȃ�)
		3	luna:	224 = 128+(32x3) (���܂��)
		*/
	//	boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+5)));	/*t256(96.0)*/
		/* (���܂�Ȃ�)
		0	easy:	128 = 128+(16x0) (���܂�Ȃ�)
		1	normal: 144 = 128+(16x1) (���܂�Ȃ�)
		2	hard:	160 = 128+(16x2) (���܂�Ȃ�)
		3	luna:	176 = 128+(16x3) (���܂�Ȃ�)
		*/
		boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+4)));	/*t256(96.0)*/
	}
	#endif
}
