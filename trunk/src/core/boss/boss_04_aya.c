
#include "boss.h"

/*static*/extern void add_zako_aya_doll(SPRITE *src);/* ���e */

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�˖��� ��
	Syameimaru Aya.
	-------------------------------------------------------
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
#if 0/* �߂��F����́A���ʕ����Ŏg���Ă�B */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif


//#de fine an imation_wait			user_data02 	/* �A�j���[�V�����p */


//	int st ate1;		/* �`�ԃX�e�[�g */	/* �e���X�e�[�g */
#define aya_angle1024			user_data04 	/* ���̊p�x */
//
#define wait1					user_data05 	/* �ړ��҂� */
//
#define aya_speed				user_data06 	/* ��x�ʑ��x */
#define ice_number				user_data07 	/* ICE�e���ԍ� */


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
	//	if (DT02 < src->ice_number) 	{	src->ice_number = DT02; }
		src->boss_base_state001 = 0;
		src->wait1	= 40;
	//	player_add_score(src->boss_base_score);
	}
}


/*---------------------------------------------------------
	�V��p�̑�e(��)�B�����ŉ������ȓ���������B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
	���x�́A[���x�ɔ�Ⴗ����̂̈Ӗ��̗ǂ�����Ȃ��Œ�l]����A
	�P���ɒe���ւƎd�l�ύX�����B
---------------------------------------------------------*/

/**/
static void s_bullet_create_aya_oodama3(SPRITE *src, int speed256)
{
			obj_send1->x256 					= (src->x256);
			obj_send1->y256 					= (src->y256);
//
			br.BULLET_REGIST_speed256			= speed256; 		/* �e�� */
			br.BULLET_REGIST_n_way				= (1);				/* [1way] */
//			br.BULLET_REGIST_div_angle1024		= (0);				/* �_�~�[�p�x(���g�p) */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
	tmp_angleCCW1024_jikinerai(obj_player, src);
	{	int angle1024;
		for (angle1024=(src->tmp_angleCCW1024); angle1024<(src->tmp_angleCCW1024+1024); angle1024 += cv1024r((30)) )/* 30�x�Â��(12==360/30������12����) */
		{
			br.BULLET_REGIST_angle1024			= angle1024;
			br.BULLET_REGIST_bullet_obj_type	= (BULLET_OODAMA32_00_SIROI+((u32)(((angle1024>>7)&(0x03)))));	/* �e�O�� */
			bullet_regist_vector();
		}
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static int src_animation_wait;
static void sakuya_wait_state(SPRITE *src/*, int next_set_state*/)
{
	if (0 < src->wait1)
	{
		src->wait1--;/*fps_factor*/
		src_animation_wait++;/*fps_factor*/
		if (7 < src_animation_wait)
		{
			src_animation_wait = 0;
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
		s_bullet_create_aya_oodama3(src, /*�d�l�ύX*/(t256(4.0)+(difficulty<<7))/*, 10*/);
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
		src_animation_wait++;/*fps_factor*/
		if (7 < src_animation_wait)
		{
			src_animation_wait = 0;
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
		src_animation_wait++;/*fps_factor*/
		if (7 < src_animation_wait)
		{
			src_animation_wait = 0;
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
	ST3_00 = 0,
	ST3_01,
	ST3_02,
	ST3_04,
	ST3_05,
	ST3_06,
	ST3_07,
	ST3_08,
	ST3_09,
	ST3_10,
	ST3_11,
	ST3_12,
};enum
{
	ST4_00 = 0,
	ST4_01,
	ST4_02,
	ST4_04,
	ST4_05,
	ST4_06,
	ST4_07,
	ST4_08,
	ST4_09,
	ST4_10,
	ST4_11,
	ST4_12,
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
};


/*---------------------------------------------------------
	�A�C�X�e
---------------------------------------------------------*/

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
	�ړ��p�^�[��(�u�ʏ�U��3�v�̃e�X�g)
---------------------------------------------------------*/

/*static*/ void aya_01_keitai(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		src->boss_base_danmaku_type 	= DANMAKU_12_aya_merin_test;//DANMAKU_08_rumia; 	/*DANMAKU_01*/	/* �S�e�����Z�b�g */
	//	danmaku_set_time_out(src);		/* �e���̐������Ԃ�ݒ�(�\�ߒe�����Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
		src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);			/* */
//		vvv256 = t256(1.0);
	}
}


/*---------------------------------------------------------
	�ړ��p�^�[��2(�ؕ��u�F�؈�ࣁv�̃e�X�g)
---------------------------------------------------------*/

/*static*/ void aya_02_keitai(SPRITE *src)
{
	/* �e���������ĂȂ��ꍇ�ɒǉ� */
	if (DANMAKU_00==src->boss_base_danmaku_type)		/* �e�������I���Ȃ�e������ */
	{
		src->boss_base_danmaku_type 	= DANMAKU_0a_houka_kenran;		/*DANMAKU_01*/	/* �S�e�����Z�b�g */
	//	danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
		src->boss_base_danmaku_time_out = ((60*10)/*DANMAKU_01_SET_TIME*6*10*/);			/* */
//		vvv256 = t256(1.0);
	}
}
/*---------------------------------------------------------
	���ɔ�����ۂ��e���������Ă݂�e�X�g
---------------------------------------------------------*/

static void bullet_create_kougeki_01(SPRITE *src)
{
	src->boss_base_danmaku_type 	= DANMAKU_12_aya_merin_test;					/* �S�e�����Z�b�g */
	danmaku_set_time_out(src);		/* �e���̎��Ԑ؂��ݒ� */
}
/*---------------------------------------------------------
	�ړ��p�^�[��0(�ᑬ�ړ�)
---------------------------------------------------------*/
/* 1�Ȃ̂͐����Â��d�l�B���݂̓V�X�e���� ketm ������ CONTROLLER �͖����B
������ɑ��铌���� ������ CONTROLLER �͍�邩������Ȃ��B
( CONTROLLER ���Ă̂́A�{�X�̎���ӂ�ӂ킵�Ēe�o���A���B) */
#define USE_CONTROLLER (0)
/*static*/ void aya_03_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case ST3_00: /* y-pos erreicht: rundumschuss */
		{
			src->boss_base_danmaku_time_out = ((0)/*DANMAKU_01_SET_TIME*6*10*/);	/* �e�����~�߂� */
		}
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
		bullet_create_kougeki_01(src); /* �U�� */

		bullet_create_kougeki_02(src); /* �U�� */
		src->wait1 = 20;
		src->boss_base_state001++;/* = ST3_01*/
		break;
//--------
	case ST3_01: /* �e�����I���܂ő҂� */
		danmaku_state_check_holding(src);/*,ST3_02*/
		break;

	case ST3_02: /* ���ֈړ��A��ʒ[�܂ŁB */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < t256(0.0))
		{
			src->boss_base_state001++/* = ST3_04*/;
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* �U�� */
		}
		break;
//--------
	case ST3_04: /* �҂� */
		sakuya_wait_state(src/*,ST3_05*/);
		break;

	case ST3_05: /* �E�ֈړ��A��ʒ��S�܂ŁB */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		if (src->x256 > BOSS_XP256)
		{
			src->boss_base_state001++;/* = ST3_06*/
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			src->wait1 = 100;
			bullet_create_kougeki_02(src); /* �U�� */
		}
		break;
//--------
	case ST3_06: /* �҂� */
		sakuya_wait_state(src/*,ST3_07*/);
		break;

	case ST3_07:
		src->boss_base_state001++;/* = ST3_08*/
		src->wait1 = 100;
	//	bullet_create_kougeki_01(); /* �U�� */
		break;
//--------
	case ST3_08: /* �҂� */
		sakuya_wait_state(src/*,ST3_09*/);
		break;

	case ST3_09: /* �E�ֈړ��A��ʒ[�܂ŁB */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		#if 0/*SDL(�������W)*/
		if (src->x256 > (t256(GAME_WIDTH-50)-((src->w128+src->w128))))
		#else/*Gu(���S���W)*/
		if (src->x256 > t256(GAME_WIDTH-50) )
		#endif
		{
			src->boss_base_state001++;/* = ST3_10*/
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* �U�� */
		}
		break;
//--------
	case ST3_10: /* �҂� */
		sakuya_wait_state(src/*,ST3_11*/);
		break;

	case ST3_11: /* ���ֈړ��A��ʒ��S�܂ŁB */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < BOSS_XP256)
		{
		//	src->boss_base_state001++;/* = ST3_00*/
			src->boss_base_state001 = ST3_00;
		}
		break;
	}
}


/*---------------------------------------------------------
	�ړ��p�^�[��1(�ᑬ�ړ�)
---------------------------------------------------------*/

/*static*/ void aya_04_keitai(SPRITE *src)
{
	switch (src->boss_base_state001)
	{
	case ST4_00: /* y-pos erreicht: rundumschuss */
		{
			src->boss_base_danmaku_time_out = ((0)/*DANMAKU_01_SET_TIME*6*10*/);	/* �e�����~�߂� */
		}
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);
		#endif //(1==USE_CONTROLLER)
	//	bullet_create_kougeki_01(src); /* �U�� */

		bullet_create_kougeki_02(src); /* �U�� */
		src->wait1 = 20;
		src->boss_base_state001++;/* = ST4_01*/
		break;
//--------
	case ST4_01: /* �҂� */
		sakuya_wait_state(src);/*,ST4_02*/
		break;

	case ST4_02: /* ���ֈړ��A��ʒ[�܂ŁB */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < t256(0.0))
		{
			src->boss_base_state001++/* = ST4_04*/;
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* �U�� */
		}
		break;
//--------
	case ST4_04: /* �҂� */
		sakuya_wait_state(src/*,ST4_05*/);
		break;

	case ST4_05: /* �E�ֈړ��A��ʒ��S�܂ŁB */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		if (src->x256 > BOSS_XP256)
		{
			src->boss_base_state001++;/* = ST4_06*/
			#if (1==USE_CONTROLLER)
			enemy_boss02_sr_add1(src);
			#endif //(1==USE_CONTROLLER)
			src->wait1 = 100;
			bullet_create_kougeki_02(src); /* �U�� */
		}
		break;
//--------
	case ST4_06: /* �҂� */
		sakuya_wait_state(src/*,ST4_07*/);
		break;

	case ST4_07:
		src->boss_base_state001++;/* = ST4_08*/
		src->wait1 = 100;
	//	bullet_create_kougeki_01(); /* �U�� */
		break;
//--------
	case ST4_08: /* �҂� */
		sakuya_wait_state(src/*,ST4_09*/);
		break;

	case ST4_09: /* �E�ֈړ��A��ʒ[�܂ŁB */
		sakuya_anime08(src);
		src->x256 += t256(2.0);/*fps_factor*/
		#if 0/*SDL(�������W)*/
		if (src->x256 > (t256(GAME_WIDTH-50)-((src->w128+src->w128))))
		#else/*Gu(���S���W)*/
		if (src->x256 > t256(GAME_WIDTH-50) )
		#endif
		{
			src->boss_base_state001++;/* = ST4_10*/
			src->wait1 = 20;
			bullet_create_kougeki_02(src); /* �U�� */
		}
		break;
//--------
	case ST4_10: /* �҂� */
		sakuya_wait_state(src/*,ST4_11*/);
		break;

	case ST4_11: /* ���ֈړ��A��ʒ��S�܂ŁB */
		sakuya_anime00(src);
		src->x256 -= t256(2.0);/*fps_factor*/
		if (src->x256 < BOSS_XP256)
		{
		//	src->boss_base_state001++;/* = ST4_00*/
			src->boss_base_state001 = ST4_00;
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
		obj_send1->x256 = t256((GAME_WIDTH-50)/2);
		obj_send1->y256 = t256(20.0);
		tmp_angleCCW1024_jikinerai(obj_send1, src);
		src->aya_angle1024 = src->tmp_angleCCW1024;
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
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= (int)(1024/24);
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
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
						br.BULLET_REGIST_speed256			= (aaa);									/* �e�� */
						int tama_graphics;
						tama_graphics = (ra_nd()&0x0f);
						if (0==tama_graphics)	{ tama_graphics = 0x08; }	/* 0 ��(���_�Ȃ̂�)������B */
						br.BULLET_REGIST_bullet_obj_type	= (BULLET_KOME_01_AOI-1) + (tama_graphics); 	/* �e�O�� */	/*BULLET_KUNAI12_00_AOI*/
						br.BULLET_REGIST_n_way				= (11-jj);								/* [10way] */
						bullet_regist_vector();
					}
				}
			}
		}
		else
		{
#if 1
/* CCW�̏ꍇ */
			src->x256 += ((sin1024((src->aya_angle1024))*t256(6.0))>>8);/*fps_factor*/
			src->y256 += ((cos1024((src->aya_angle1024))*t256(6.0))>>8);/*fps_factor*/
#endif
			src->boss_base_state001--/* = SS00*/;
		}
		break;

	case SS02:	/* �����ʒu:��e3��->SS03, SS04, SS08 */
		src->wait1--;/*fps_factor*/
		if (0 > src->wait1)
		{
			src->wait1 = 5/*10*/;
			s_bullet_create_aya_oodama3(src, (src->aya_speed)-t256(2.0) );
			int angle_jikinerai_1024;
			angle_jikinerai_1024 = src->tmp_angleCCW1024;
					/*t256(5.0)*/ /* �e�� */		/*�d�l�ύX*/	// t256(/*1.5*/5.0/*5.0*/)
			{
				const u8 sst[8] = { SS03, SS03, SS03, SS04, SS04, SS04, SS08, SS08 }; /* (1/3)�̊m���ŕ��򂷂�B */
				src->boss_base_state001 = sst[((ra_nd()&(8-1)))];	/* SS03, SS04, SS08 */
			}
			if (src->boss_base_state001 == SS08)
			{
				src->aya_angle1024 = angle_jikinerai_1024;
			}
			else
			{
				src->y256 += t256(6.0);/*fps_factor*/
			}
		}
		break;

//-------- ���� [SS03]
	case SS03: /* ���ړ���->SS05�� */
		sakuya_anime00(src);
		src->x256 -= (src->aya_speed);/*fps_factor*/	 /*(t256(4.0)+(difficulty<<8))*/
		if (0 > src->x256)
		{
			src->boss_base_state001=SS05/*SS05*/;
		}
		break;
//-------- ���� [SS04]
	case SS04: /* �E�ړ���->SS08�� */
		sakuya_anime08(src);
		src->x256 += (src->aya_speed);/*fps_factor*/	/*(t256(4.0)+(difficulty<<8))*/
		#if 0/*SDL(�������W)*/
		if (src->x256 > (t256((GAME_WIDTH-50))-((src->w128+src->w128))) )
		#else/*Gu(���S���W)*/
		if (src->x256 > t256((GAME_WIDTH-50)) )
		#endif
		{
			src->boss_base_state001++/*SS05*/;
		}
		break;
//--------

		//	src->x256 += t256(1.0); 	// [***090114		�ǉ�
		//	src->x256 -= t256(1.0); 	// [***090114		�ǉ�


	case SS05: /* ��:�ҋ@->SS06�� */
		sakuya_wait_state(src/*,SS06*/);
		break;

	case SS06: /* ��:�E��]�U��->SS00, SS07 */
		#if (1==USE_CONTROLLER)
		enemy_boss02_sr_add1(src);/* ��:�E��]�U�� */
		enemy_boss02_sr_add2(src);/* �E:����]�U�� */
		#endif //(1==USE_CONTROLLER)

	//	/* (1/2)�̊m����[�U��A]�֕��򂷂�B */
	//	if (0==(ra_nd()&(2-1)))
		/* (1/4)�̊m����[�U��A]�֕��򂷂�B */
		if (0!=(ra_nd()&(4-1)))
		{
			/* (1/2)�̊m����[�U��A]�֕��򂵂Ȃ��B */
		//	src->boss_base_state001 = SS00;
			src->boss_base_state001 = SS00;
		}
		else
		{	src->boss_base_state001++/* = SS07*/;}
		break;
//-------- [�U��A]
	case SS07: /* ��:�O�� -> SS11 */
		src->y256 += (t256(1.0)+(src->aya_speed));/*fps_factor*/	/*(t256(4.0)+(difficulty<<8))*/
		if (src->y256 > t256(GAME_HEIGHT/3) )
		{
		//	src->boss_base_state001++;/* = SS11*/
			{
		//		-cv1024r((30)),
			/* CCW�̏ꍇ */
				obj_send1->x256 					= (src->x256);
				obj_send1->y256 					= (src->y256);
				br.BULLET_REGIST_speed256			= (src->aya_speed)-t256(2.0);			/* �e�� */	/*t256(5.0)*/
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= cv1024r(360-(30));
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI+(ra_nd()&0x0f);	/* �e�O�� */	/* �e�ɓœh���Ă���ݒ� */
				br.BULLET_REGIST_n_way				= (5);									/* [5way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			src->boss_base_state001 = SS00;
		}
		break;
//-------- ���� [SS08]
	case SS08: /* �v���C���[�ʒu�t�߈ړ��� -> �v���C���[�ʒu�t��:��e3�� -> SS00�� */
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
			s_bullet_create_aya_oodama3(src, ((src->aya_speed)>>1));
			/* �e�� */	/*t256(3.0)*/	/*�d�l�ύX*/	//t256(/*1.3*/3.0/*3.0*/),
			//	,20/*ransu512*/
			/* ��e�̒ǉ� */
			src->boss_base_state001 = SS00; /* SS00�� */
		//	break;
		}
		else
		{
#if 1
			src->x256 += ((sin1024((src->aya_angle1024))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->y256 += ((cos1024((src->aya_angle1024))*(src->aya_speed)/*(t256(4.0)+(difficulty<<8))*/)>>8);/*fps_factor*/
#endif
		}
		break;
	}
}


/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

static void move_aya(SPRITE *src)
{
//	ch eck_regist_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ */
//	sp ell_card_generator222(src);	/* �X�y�J���� */
	check_regist_generate_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ / �X�y�J���� */
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
	move_boss_effect(src);		/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
//
	bullet_angle_all_hamidasi_check();/*�p�x�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)*/
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void boss_init_aya(SPRITE *h)
{
	h->callback_mover			= move_aya;
	h->callback_loser			= aya_put_items;

	h->wait1					= (50);
	src_animation_wait			= (0);
	h->aya_speed				= t256(4.0)+(difficulty<<8);
	h->ice_number				= (0);

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
//
//	bullet_clip_min.x256 = t256(-32);
//	bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
//	bullet_clip_min.y256 = t256(-256);
//	bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
	bullet_clip_min.x256 = t256(-100);
	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-256);
	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
}
