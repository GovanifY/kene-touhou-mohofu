
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"�����w2"		�����閂���w
	"�����w1"		�΂�܂��U�������w
	-------------------------------------------------------
	�n�`�Ɋ֘A����G
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

#define time_out				user_data03/*tmp_angleCCW1024*/
#define MAHOU_TEKI_wait1		user_data04
#define MAHOU_TEKI_wait2		user_data05
#define start_y256				user_data06


/* ���Ԑݒ� */

//#define MAHOU_TIME_LIMIT_00_MAX	(0x1ff)
//#define MAHOU_TIME_LIMIT_00_MAX	(0x17f)
#define MAHOU_TIME_LIMIT_00_MAX 	(0x13f)
#define MAHOU_TIME_LIMIT_01_ADJ 	((int)(0x0ff/3))
#define MAHOU_TIME_LIMIT_02_ADJ 	(MAHOU_TIME_LIMIT_00_MAX - MAHOU_TIME_LIMIT_01_ADJ)
#define MAHOU_TIME_LIMIT_03_EPS 	(0x04)

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

static void lose_kougeki_mahoujin(SPRITE *src)
{
	if (rand_percent(30))	{	item_create(src, SP_ITEM_00_P001,	 1, (ITEM_MOVE_FLAG_06_RAND_XY) );}
	if (rand_percent(30))	{	item_create(src, SP_ITEM_00_P001,	 1, (ITEM_MOVE_FLAG_06_RAND_XY) );}
	if (rand_percent(30))	{	item_create(src, SP_ITEM_05_TENSU,	 1, (ITEM_MOVE_FLAG_06_RAND_XY) );}
}

/*---------------------------------------------------------
	�G�U�����Ȃ�
---------------------------------------------------------*/

static void attack_kougeki_mahoujin00(SPRITE *src)
{
	/* �U�����Ȃ� */
}
/*---------------------------------------------------------
	�c�U���̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin01(SPRITE *src)
{
	/* �U���J�n�t���[������16�t���[���o�ߌ�� 1�񂾂��U�� */
	if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	{
		obj_send1->x256 					= src->x256;
		obj_send1->y256 					= src->y256;
		br.BULLET_REGIST_speed256			= (t256(2.5));			/* �e�� */
		br.BULLET_REGIST_angle1024			= (0);					/* ������(�㉺�e) */
		br.BULLET_REGIST_div_angle1024		= (int)(1024/(2));		/* �p�x(����� 2 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_02_AKA;	/* �ԕĒe */
		br.BULLET_REGIST_n_way				= (2);					/* [2way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
}

/*---------------------------------------------------------
	�G�U��("�����w2")�̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin02(SPRITE *src)
{
	if (0 < src->MAHOU_TEKI_wait2)		// [***090128		�ύX
	{
		src->MAHOU_TEKI_wait1--;			/* �΂�܂��Ԋu�J�E���^ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* �΂�܂��Ԋu�J�E���^ */
		{
			src->MAHOU_TEKI_wait1 = (8)/*5*/;/*�v���O�����������Ȃ��Ă镪�x������*/
			src->MAHOU_TEKI_wait2--;
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*�e�L�g�[*/
			/* ���x�� 1.0-3.0�܂ł̖��i�K�����ɕύX */
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);
			br.BULLET_REGIST_speed256						= ((ra_nd()&(512-1))+256);		/* �e�� */	/*t256(sp)*/
			br.BULLET_REGIST_angle1024						= (ra_nd()&(1024-1));			/* �p�x */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.04);					/* �����萔 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	�G�U��("�����w1")�̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin03(SPRITE *src)
{
	if (
			(src->y256 > obj_player->y256)
	 || 	(src->y256 > t256(100))
	)
	{
		obj_send1->x256 					= (src->x256);
		obj_send1->y256 					= (src->y256);
	//	src->time_out++;/* = ST01;*/
		src->time_out						= MAHOU_TIME_LIMIT_01_ADJ; /* ������ */
		br.BULLET_REGIST_speed256			= t256(1.0);				/* �e�� */
	//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
		br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
	//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [�Ԓe] */ /* �e�O�� */
		br.BULLET_REGIST_n_way				= (1);						/* [1way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_vector();
	}
//	if (src->MAHOU_TEKI_wait2<(difficulty*7)+10) // [***090128 �ύX */
	{
		src->MAHOU_TEKI_wait1--;			/* �΂�܂��Ԋu�J�E���^ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* �΂�܂��Ԋu�J�E���^ */
		{
			src->MAHOU_TEKI_wait1 = (25-(difficulty*5)); /* �΂�܂��Ԋu�J�E���^ */
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*�e�L�g�[*/
			/* ���x�� 1.0-4.0�܂ł̗���(�����������ɕ΍�)�ɕύX */
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);

			br.BULLET_REGIST_speed256						= ((ra_nd()&(1024-1))|(0x100)); 	/* �e�� */	/*t256(sp)*/
			br.BULLET_REGIST_angle1024						= ((ra_nd()&(1024-1))); 			/* �p�x */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.03);	/* �����萔 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	�~��U���̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin04(SPRITE *src)
{
	/* �U���J�n�t���[������16�t���[���o�ߌ�� 1�񂾂��U�� */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->time_out&0x07))
	{
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);
			br.BULLET_REGIST_speed256						= (t256(2.5));			/* �e�� */
			br.BULLET_REGIST_angle1024						= ( (src->time_out+src->time_out)+((src->time_out&0x07)<<7));	/* ���� */
	//		br.BULLET_REGIST_angle1024						= (int)(1024/(7));		/* �p�x(����� 7 ����) */	/* 1����n���������p�x */
	//		BULLET_KOME_02_AKA; 	/* �ԕĒe */
	//		(7));/* [7way�ԕĒe] */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.03);	/* �����萔 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
	}
}

/*---------------------------------------------------------
	�~��U���̃X�y�J
---------------------------------------------------------*/

static void attack_kougeki_mahoujin05(SPRITE *src)
{
	/* �U���J�n�t���[������16�t���[���o�ߌ�� 1�񂾂��U�� */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->time_out&0x07))
	{
			obj_send1->x256 								= (src->x256);
			obj_send1->y256 								= (src->y256);
			br.BULLET_REGIST_speed256						= (t256(2.5));			/* �e�� */
			br.BULLET_REGIST_angle1024						= (-(src->time_out+src->time_out)+((src->time_out&0x07)<<7));	/* ���� */
	//		br.BULLET_REGIST_angle1024						= (int)(1024/(7)),		/* �p�x(����� 7 ����) */	/* 1����n���������p�x */
	//		BULLET_KOME_02_AKA, 	/* �ԕĒe */
	//		(7));/* [7way�ԕĒe] */
			br.BULLET_REGIST_tomari2_dan_delta256			= t256(0.03);	/* �����萔 */
			br.BULLET_REGIST_tomari2_dan_next_angle1024 	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_regist_type					= REGIST_TYPE_03_TOMARI;
			bullet_regist_vector();
	}
}


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
		#if 0
		if (S P_MUTEKI == src->type)
		{			attack_kougeki_mahoujin02(src); }	/* ("�����w2") �����閂���w */
		else
	//	if (SP_ZAKO == src->type)
		{			attack_kougeki_mahoujin03(src); }	/* ("�����w1") �΂�܂��U�������w */
		#endif

static void move_kougeki_mahoujin(SPRITE *src)
{
	src->time_out--;
	/* �o�� */
	if (MAHOU_TIME_LIMIT_02_ADJ < src->time_out)
	{
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/ /*4*/ /*fps_factor*/
	}
	/* �U�� */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ < src->time_out)
	{
		/* �X�y�J�I�� */
		void (*aaa[8])(SPRITE *src) =
		{
			attack_kougeki_mahoujin00,		/* �U�����Ȃ� */
			attack_kougeki_mahoujin01,		/* �c�e */
			attack_kougeki_mahoujin02,		/* �΂�܂� #01(�������A��"�����w2") */
			attack_kougeki_mahoujin03,		/* �΂�܂� #02(�������A��"�����w1") */
			attack_kougeki_mahoujin04,		/* 8way�Ԓe */
			attack_kougeki_mahoujin05,		/* 8way�Ԓe */
			attack_kougeki_mahoujin04,		/* 8way�Ԓe */
			attack_kougeki_mahoujin04,		/* 8way�Ԓe */
		};
		(*aaa[ (int)((src->x256)&0x07) ])(src); 	/* �e�֐��ɕ��򂷂� */
	}
	/* ������ */
	else
	if ( MAHOU_TIME_LIMIT_03_EPS < src->time_out )	/*	if ( 0x04 0x00 > (unsigned int)(src->alpha))*/
	{
		src->color32 -= 0x03000000; 	/*	src->alpha += 0x03;*/ /*4*/ /*fps_factor*/
	}
	/* �����܂� */
	else
	{
	//	src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;*/
		src->type = SP_DELETE;
	}
//
	src->y256 = src->start_y256 + ((cos1024(((src->time_out<<5)&(1024-1))))<<2);

//	src->m_angleCCW1024++;
//	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_enemy_mahoujin(STAGE_DATA *l)
{
	SPRITE *h;		//	h			= NULL;
	h									= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		if (CTYPE_19_MAHOUJIN_A==((char)l->user_i_code))
		{		/* �΂�܂��U�������w */	//	"�����w1",	/*	"GROUNDER",*/
			h->m_Hit256R				= ZAKO_ATARI04_PNG;
			h->type 					= TEKI_58_RED_BOOK; 	/*SP_ZAKO*/ /*SP_TIKEI*/ /*SP_GROUNDER*/
			h->callback_hit_enemy		= callback_hit_zako;
			h->callback_loser			= lose_kougeki_mahoujin;
	//		h->MAHOU_TEKI_wait1 		= 100;	// [***090124		�ǉ�
		//	h->MAHOU_TEKI_wait2 		= 0;/* ���g�p  */
		}
		else	//CTYPE_20_MAHOUJIN_B:		/* �����閂���w */	//	"�����w2",	/*	"MAGICF",*/
		{
			h->m_Hit256R				= ZAKO_ATARI16_PNG;
			h->type 					= TEKI_52_MAHOJIN;	/*SP_TIKEI*/ /*SP_GROUNDER*/
//			/*data->base.*/h->base_hp	= (9999);/* �|���Ȃ� */
		//	h->callback_hit_enemy		= callback_hit_zako;
	//		h->MAHOU_TEKI_wait1 		= 10/*+90*/;/* */
		//	h->MAHOU_TEKI_wait2 		= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;/* �ő�܉� */
		}
		h->callback_mover				= move_kougeki_mahoujin;
		/* KETM�̑��΍��W�w��͔p�~�B��ʍ��W�w��ɂ��� */
		h->x256 						= (((short)l->user_x)<<8)|(((char)l->user_255_code)&0x07);/* ��������C���^�[���[�u */
		h->start_y256					= (((short)l->user_y)<<8);
	//
		h->time_out 					= MAHOU_TIME_LIMIT_00_MAX;/* SS00 �܂��� ST00 */

		h->MAHOU_TEKI_wait1 			= (10); 	/*+90*/ /* */
		h->MAHOU_TEKI_wait2 			= (5);		/* �ő�܉� */	/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/
		h->color32						= 0x00ffffff;	/*	h->alpha				= 0x02 0x00;*/
	}
}
//		/*data->base.*/h->base_hp		+= (8*25)+(1/*di fficulty*/<<(3/*4*/+3));	//+(di fficulty*15)
//		/*data->base.*/h->base_score	= score(200*2);
