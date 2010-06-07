
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�ړ��ʒe
---------------------------------------------------------*/

/*---------------------------------------------------------
	�e���쐬 / �����쐬
	-------------------------------------------------------
	��ŕω�����e����邩���B
	�{�X��|�������ɏ�����悤�ɂ��Ă���B
	�ǉ������e
	// [***090114	��e�̓�����ς��Ă݂�
	-------------------------------------------------------
	�������Ė{�ƌ������Ă݂�B
	�{�Ƃ̒e���Ɂu�ϐg�@�\�Ȃ�Ė����v�B
	�P�ɒe�����X�g���āA�e���ԍ��ŏ��������Ă�C�����Ă����B

	������������̕����ł͒e�����X�g���������́A
	�o���Ȃ������Ȃ����A���Ȃ�߂�ǂ������̂ŁA
	�e�����X�g���V���v���ȍ\���ɂ��������B(data�Amal loc���Ȃ�)
	-------------------------------------------------------

�� �e���V�X�e��(������)�B
 1.�e�̕ϐg:
 �e�̕ϐg�́A���܂Œe���ϐg����V�X�e�����Ǝv���Ă������A�{�Ƃ̒e�ɂ���ȋ@�\�͂Ȃ�(���Ԃ�)�B
�e�u���v�ϐg����񂶂�Ȃ��āA�e�u���v�ϐg������B���̈Ⴂ�͑傫���B
��������ϐg����e�͗v��Ȃ��B�e���V�X�e���ɒe�̕ϐg�@�\�͎������Ȃ��B(�O�����狭���I�ɕϐg������)

�Ⴆ�Ζ͕핗���́u���邭��i�C�t�v�Ő�������ƁA

���݂̖͕핗:
	��邪�u���邭��i�C�t�e�v�Ƃ�������e�𓊂���B
	�u���邭��i�C�t�e�v�͌����e�B
	���x�����ȉ��ɂȂ�Ǝ�����(�킴�Ƒ_��Ȃ�)���@�_���e�ɕϐg(�΃i�C�t)����B
	���@�_���e�Ƃ��Ĉړ�(���邪�����܂œ���e)�B

�ύX��:
	��邪�u�ʏ�e�v�𓊂���B(�Ⴆ�Βʏ�e�A�i�C�t�Ƃ܂������������́A
	���̂܂܉������Ȃ���Βʏ�e�A�i�C�t�Ƃ܂����������B������܂�������)
	�u�ʏ�e�v�͂����̌����e�B
	���t���[��(�e�Ƃ͊֌W�Ȃ��ꏊ�Ōv�������Ԃ�)�o�߂���ƁA��邪�u�e���`�F�b�N�C�x���g�����v������B
	��邪�u�e�����X�g�������v���A����(�Ⴆ�Α��x�����ȉ��̐i�C�t)�Ɉ�v�������̂�ϐg������B
	�ϐg���������(�킴�Ƒ_��Ȃ�)���@�_���u�ʏ�e�v�B
	(�ʏ�e�΃i�C�t�Ƃ܂�������������)
	-------------------------------------------------------
	�����͐F�X����B�Ⴆ�Δ��������莞�Ԍo�߂����e�Ƃ��B

���݂̖͕핗�͓���e����������B�{�Ƃ̒e�ɂ͓���e�͈������(���Ԃ�)�B
�������������łȂ��ƁA�d���̑�ʐ؂�݂����̂͒x���Ȃ肷���Ď����ł��Ȃ�(���Ԃ�)�B
(�e���{�X�����������Ă����̂��@�m���Ēe����̂ł́A��ʏ�̑S�e�ɖ��t���[�����̏���������̂ő��x���x���Ȃ肷����B
�{�X���u�e�����X�g�������v���A�����ɋ������߂��e�����؂�̂Ȃ�A���ōςނ̂ŏ\�������B
���̏���������ƂĂ��x�������ꍇ�ł�1�t���[���Ȃ̂őS�̓I�ɖ�肪�o�Ȃ�)

 2.�e�̊�_:
 ���Ă̒e�ɂ͊�_�Ɣ������Ԃ�����A�P���Ȋ֐��Ō��݈ʒu����x�Ŋ���o���Ă���C������B

---------------------------------------------------------*/

global BULLET_REGIST br;

#define callback_2nd_regist 	callback_loser	/* �Qnd�o�^ */




/*---------------------------------------------------------
	�Ƃ肠�����ڍs�p(2)�ǉ��e�A��{�@�\
---------------------------------------------------------*/

#define BULLET_DATA_2nd_count	user_data10 	/* �Qnd �ϐg */
#define BULLET_DATA_2nd_speed	user_data11 	/* �Qnd ���x */
#define BULLET_DATA_2nd_vx256	user_data12 	/* �Qnd vx256 */
#define BULLET_DATA_2nd_vy256	user_data13 	/* �Qnd vy256 */



/*---------------------------------------------------------
	�Qnd �ϐg�o�^�A��{��
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src)
{
	src->BULLET_DATA_2nd_count = 9999;				/* 3���ϐg���Ȃ� */
	src->vx256	= (src->BULLET_DATA_2nd_vx256); 	/* �Qnd vx256 */
	src->vy256	= (src->BULLET_DATA_2nd_vy256); 	/* �Qnd vy256 */
}




/*---------------------------------------------------------
	�x�N�g���e(�p�x�ω����Ȃ����̂͂��̕����̕�������)
---------------------------------------------------------*/

/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
#define BASE_add_speed256						user_data07 	/* �����x */


/* ���p�e(���x�ω�����) */
//typedef struct
//{
#define TOMARI_DAN_DATA_speed256				user_data00 	/* ���x */
#define TOMARI_DAN_DATA_delta256				user_data01 	/* �����x */
//
#define TOMARI_DAN_DATA_init_vx256				user_data02 	/* �ݒ菉�����x */
#define TOMARI_DAN_DATA_init_vy256				user_data03 	/* �ݒ菉�����x */
//} TOMARI_DAN_DATA;


/* ��{�e(���x�A�p�x�A�Ƃ��ɕω����Ȃ�) */
//typedef struct
//{
#define GRAVITY_BULLET_DATA_delta256			user_data01 	/* �����x(y�������̂�) */
//} GRAVITY_BULLET_DATA;


//typedef struct
//{
#define HAZUMI_DAN_DATA_delta256				user_data01 	/* �����x(y�������̂�) */
#define HAZUMI_DAN_DATA_bound_counts			user_data02 	/* ���˂�\��̉�(0�ŏI���) */
//} HAZUMI_DAN_DATA;

/*---------------------------------------------------------
	�x�N�g���W�J�e
	-------------------------------------------------------
	�w����W(���e����̑��Έʒu)�܂Ŕ䗦�œ����A
	��������̓x�N�g���e
---------------------------------------------------------*/



/*---------------------------------------------------------
	�p�x�e(�p�x�ω��������)
	-------------------------------------------------------
	�p�x�ω��������(�����e)�́A���t���[�����ƁA�p�x�ω��v�Z���Ȃ����Ă���ۂ��B(?)
---------------------------------------------------------*/

/* ���p�e */

//typedef struct
//{
#define KURUKURU_KNIFE_DATA_speed_2nd_256		user_data00
#define KURUKURU_KNIFE_DATA_height				user_data02
//} KURUKURU_KNIFE_DATA;


//typedef struct
//{
#define BULLET_SP1_DATA_angle1024				user_data00
#define BULLET_YUUDOU_DATA_hiritu256			user_data01
#define target_x256 							user_data02
#define target_y256 							user_data03
//} BULLET_SP1_DATA;


#define BULLET_HOSI_GATA_DATA_angle1024 		user_data00
#define BULLET_HOSI_GATA_DATA_speed256			user_data01 		/* ���x */
#define BULLET_HOSI_GATA_DATA_add_delta256		user_data02 		/* ���Z�A�����x */
#define BULLET_HOSI_GATA_DATA_star_remain_time	user_data03


/*---------------------------------------------------------
	���x�l�A�p�x�l�A���x�N�g���ʂɕϊ�����
---------------------------------------------------------*/

static void callback_2nd_regist(SPRITE *src);
static void regist_vector(SPRITE *src, int speed256, int angle1024)
{
	src->vx256	= ((sin1024((angle1024))*(speed256))>>8);	/*fps_factor*/
	src->vy256	= ((cos1024((angle1024))*(speed256))>>8);	/*fps_factor*/
//
	#if 0
	/* �e�����x������ꍇ�ɏC������ */
	if (( (8) > abs(src->vx256)) )	{ src->vx256 = (0<src->vx256)?( (8)):( (-8)); }
	if (( (8) > abs(src->vy256)) )	{ src->vy256 = (0<src->vy256)?( (8)):( (-8)); }
	#endif
//
	/* �Ƃ肠�����������ɂ��� */
	src->BULLET_DATA_2nd_vx256	= ((src->vx256)>>1);	/*fps_factor*/
	src->BULLET_DATA_2nd_vy256	= ((src->vy256)>>1);	/*fps_factor*/
//
	/* �Ƃ肠�����A��{�� */
	src->callback_2nd_regist	= callback_2nd_regist;
//
	/* �Ƃ肠�����A�e�L�g�[ */
	src->BULLET_DATA_2nd_count	= (30); 			/* 30�J�E���g�����甼���� */
//	src->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
	src->BASE_add_speed256		= 1+(t256(1.00));	/* ���� �e */

}

/*---------------------------------------------------------
	��{�ʈړ��A��ʊO�͏�����
	��ݏo���`�F�b�N�@�\�͂Ȃ��̂ŕʂł�鎖
---------------------------------------------------------*/

static void move_bullet_vector(SPRITE *src)
{
	if (t256(1.00)!=(src->BASE_add_speed256))
	{
		src->vx256 = ((src->vx256*src->BASE_add_speed256)>>8);/* ����/���� */
		src->vy256 = ((src->vy256*src->BASE_add_speed256)>>8);/* ����/���� */
	}
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//
//	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */

	#if 1/* (�����Ȃ�\���������\��) */
	src->BULLET_DATA_2nd_count--;		/*fps_factor*/
	if (0 > src->BULLET_DATA_2nd_count)
	{
		if (NULL != src->callback_2nd_regist)
		{
			(src->callback_2nd_regist)(src);
			src->callback_2nd_regist = NULL;
		}
	}
	#endif
}


/*---------------------------------------------------------
	���̒����U���i�C�t�e
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/

#if 1
//static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void s_tmp_angleCCW1024_jikinerai_auto(SPRITE *p, SPRITE *t/*, int angle512*/);
static void move_bullet_sakuya_linear_yuudou(SPRITE *src)
{
	src->base_time_out--;
	if (0 > src->base_time_out) 				/* �����^�� */
	{
		/* �ړ����x t256(1.0) �œ����ړ� */
		src->vx256 = ((sin1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
		src->vy256 = ((cos1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
	}
	else
	if (0+2048 > src->base_time_out)			/* �U���^�� */
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
			src->BULLET_YUUDOU_DATA_hiritu256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{	/* ���� == (�e�̌��ݍ��W - �e�̗U�����W) */
			int x_sa256 = (src->x256 - src->target_x256);
			int y_sa256 = (src->y256 - src->target_y256);
			/* ������x�A�U�����W�̋߂��܂ł����� */
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->base_time_out = 0; 	/* �����^���� */
				}
			}
			/* ���Z���� == (�e�̍������W * �U���䗦) */
			int aaa_x256 = ((x_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			int aaa_y256 = ((y_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			src->x256 = src->target_x256 + (aaa_x256);	/*fps_factor*/
			src->y256 = src->target_y256 + (aaa_y256);	/*fps_factor*/
		}
	}
	else
	if (0+2048==src->base_time_out) 			/* �_���e�ɕϐg���� */
	{
	//	src->type			= BULLET_MARU8_00_AKA+(1);
		src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		src->BULLET_SP1_DATA_angle1024 = ANGLE_JIKI_NERAI_DAN;
		{
			src->tmp_angleCCW1024			= src->BULLET_SP1_DATA_angle1024;
			s_tmp_angleCCW1024_jikinerai_auto(obj_player, src);
			src->BULLET_SP1_DATA_angle1024	= src->tmp_angleCCW1024;
		}
		mask1024(src->BULLET_SP1_DATA_angle1024);
		src->m_angleCCW1024 = src->BULLET_SP1_DATA_angle1024;
		src->vx256 = (0);	/*fps_factor*/
		src->vy256 = (0);	/*fps_factor*/
		/* �U�����W�̓v���C���[�̈ʒu�t�� */
		/* �Ƃ肠�����A���Ή��B�����I�ɐݒ肵���Ⴄ */
		src->target_x256	= (obj_player->x256+t256(32/2)/*���̔���*/);		/*fps_factor*/
		src->target_y256	= (obj_player->y256+t256(16/2)/*�����̔���*/);	/*fps_factor*/
		src->BULLET_YUUDOU_DATA_hiritu256		= t256(1.0);	/* (2.5==5*0.5) */
	}
//
	move_bullet_vector(src);
}
#endif



/*---------------------------------------------------------
	����		B.���^��`�������w�p
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/

static void move_bullet_sakuya_hosi_gata(SPRITE *src)
{
	{
	//	if (2==data->sdata->nnn/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (2==(*(data->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��1�̎��z�u���āA�X�e�[�g��2�ɂȂ����瓮�� */
	//	if (1==(*(data->sd_nnn))/*%(4-difficulty)*/)/* �X�e�[�g��0�̎��z�u���āA�X�e�[�g��1�ɂȂ����瓮�� */
		src->BULLET_HOSI_GATA_DATA_star_remain_time--;
		if (0 > (src->BULLET_HOSI_GATA_DATA_star_remain_time))/* ���� */
		{
			src->BULLET_HOSI_GATA_DATA_star_remain_time = 0;
			src->BULLET_HOSI_GATA_DATA_speed256 += (src->BULLET_HOSI_GATA_DATA_add_delta256); //(4) (3.84) == t256(0.015);
			src->vx256 = ((sin1024((src->BULLET_HOSI_GATA_DATA_angle1024))*(src->BULLET_HOSI_GATA_DATA_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->vy256 = ((cos1024((src->BULLET_HOSI_GATA_DATA_angle1024))*(src->BULLET_HOSI_GATA_DATA_speed256))>>8);/*fps_factor*/
		}
	}
	//move_bullet_vector(src);
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */
}

/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static void s_tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *t)
{
	#if 1
	t->tmp_angleCCW1024 =	(atan_1024((p->y256-t->y256), (p->x256-t->x256)));
	/* �����e�̏ꍇ�Ɏ��@�_��(�ȗ���) �e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ� */
	#endif
}
//static int angle_jikinerai1024_auto(SPRITE *p, SPRITE *t, int angle1024)
//{
//	if (ANGLE_JIKI_NERAI_DAN==angle1024)	{	angle1024	= angle_jikinerai1024(p,t);}
//	return (angle1024);
//}
static void s_tmp_angleCCW1024_jikinerai_auto(SPRITE *p, SPRITE *t/*, int angle1024*/)
{
	if (ANGLE_JIKI_NERAI_DAN==t->tmp_angleCCW1024/*angle1024*/)
	{
		s_tmp_angleCCW1024_jikinerai(p, t);
	//	angle1024 = t->tmp_angleCCW1024;
	}
//	return (angle1024);
}

/*---------------------------------------------------------

	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
#define TOMARI_DAN_next_angle1024	user_data04/*tmp_angleCCW1024*/
#define TOMARI_DAN_LIMIT_00_640 	(512+128)
#define TOMARI_DAN_LIMIT_00_576 	(512+64)
#define TOMARI_DAN_LIMIT_01_512 	(512+0)
#define TOMARI_DAN_LIMIT_02_000 	(0+0)
//	int speed256,
//	int angle512//,
//	int ransu512//,
//	int add_speed256/*, int xoffs256, int yoffs256*/

static void bullet_create_oodama22(SPRITE *src);
//atic void enemy_stop_bullet_move(SPRITE *src)
static void enemy_stop_bullet2_move(SPRITE *src)
{
	src->base_time_out--;
	if (TOMARI_DAN_LIMIT_00_640 < src->base_time_out)
	{
		/* �~�܂� */
		src->TOMARI_DAN_DATA_speed256 -= (src->TOMARI_DAN_DATA_delta256);
		src->vx256 = (((src->TOMARI_DAN_DATA_init_vx256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = (((src->TOMARI_DAN_DATA_init_vy256)*src->TOMARI_DAN_DATA_speed256)>>8);/*fps_factor*/
		if (src->TOMARI_DAN_DATA_speed256 < t256(0))
		{
			src->TOMARI_DAN_DATA_speed256 = t256(0);
			src->base_time_out = (TOMARI_DAN_LIMIT_00_640);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_00_576 < src->base_time_out)
	{
		/* ����(��e�ɕϐg) */
		src->type		= BULLET_MARU8_00_AKA + (( src->base_time_out>>3)&0x07);		/*�x��(�ϐg����̂�)*/
	}
	else
	if (TOMARI_DAN_LIMIT_01_512 < src->base_time_out)
	{
		if (ANGLE_NO_SHOT_DAN != src->TOMARI_DAN_next_angle1024 )
		{
			bullet_create_oodama22(src);
			src->base_time_out = (TOMARI_DAN_LIMIT_02_000);
		}
		else
		{
			src->base_time_out = (TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_000 < src->base_time_out)
	{
		;	/* ���΂���~ */
	}
	else
	{
		/* ������ */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if ( 0x20000000 > src->color32) 	/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->type = SP_DELETE;
		}
	}
	/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
	if (ANGLE_NO_SHOT_DAN == src->TOMARI_DAN_next_angle1024 )
	{	/* �X�N���[������ */
		src->y256 += t256(1.0); 	//	my_adjust_y(s,pd);
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
//	src->HAZUMI_DAN_DATA_angle512	= at an_512(sin512((src->HAZUMI_DAN_DATA_angle512)),-cos512((src->HAZUMI_DAN_DATA_angle512)));
//}
static void bullet_move_hazumi_dan(SPRITE *src)
{
	if (/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 < t256(15) )	/* �ő�d�͉����x */
	{
		/*src->HAZUMI_DAN_DATA_sum256*/src->vy256 += src->HAZUMI_DAN_DATA_delta256; /* �␳�l */
	}
//	s->y256 += src->HAZUMI_DAN_DATA_sum256; 		/* �ώZ�d�͉����x */
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
		if (0!=src->HAZUMI_DAN_DATA_bound_counts)
		{
			src->HAZUMI_DAN_DATA_bound_counts--;	/* ���˂�񐔂����炷 */
			src->type		= BULLET_MARU8_00_AKA+4+(src->HAZUMI_DAN_DATA_bound_counts);		// [***090116		�኱�ύX
		//	src->HAZUMI_DAN_DATA_sum256 	= -src->HAZUMI_DAN_DATA_sum256; 	/* �d�͉����x */
			#if 0
			src->HAZUMI_DAN_DATA_speed256	= -src->HAZUMI_DAN_DATA_speed256;		/* ���x */
			src->HAZUMI_DAN_DATA_speed256	+= src->HAZUMI_DAN_DATA_speed256;		/* ����(���ēV��֓�����) */
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
	src->vy256 += src->GRAVITY_BULLET_DATA_delta256;
//	data->speed256 += data->delta256;
//	src->y256 += data->speed256;
	move_bullet_vector(src);
}

/*---------------------------------------------------------
	���
	�v���C���[�̎�������邭�����Ă���A�����_���̃i�C�t�B
	���΂ɐF���ς��B
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
#define knife_tmp_angle1024 user_data04/*tmp_angleCCW1024*/

static void bullet_move_kurukuru_knife(SPRITE *src)
{
	src->base_time_out--;
	if ( 0 > src->base_time_out)
	{
		if (((src->x256 < t256(0))||(src->x256 > t256(GAME_WIDTH))||
			 (src->y256 < t256(0))||(src->y256 > t256(GAME_HEIGHT))))
		{
			src->type = SP_DELETE;/* ��ʊO�ɏo���炨���܂� */
		}
	}
	else
	if (250 > src->base_time_out)
	{
		;
	}
	else
	if (250 == src->base_time_out)
	{
		;
	//	if (src->KURUKURU_KNIFE_DATA_aaa_speed256 <= t256(0) )
		{
		//	src->KURUKURU_KNIFE_DATA_aaa_speed256	= src->KURUKURU_KNIFE_DATA_speed_2nd_256;
		//	src->KURUKURU_KNIFE_DATA_target 		= 1;
			src->base_time_out						= (250);
			src->type								= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
			src->tmp_angleCCW1024 = atan_1024(
				#if 1/*Gu(���S���W)*/
				(obj_player->y256)-(src->y256)+(src->KURUKURU_KNIFE_DATA_height*256),
				(obj_player->x256)-(src->x256)
				#endif
			);
			mask1024(src->tmp_angleCCW1024);/*�˂�̂���*/
			src->m_angleCCW1024 = ((src->tmp_angleCCW1024));	/* �\���p�x */
		}
		/*data->*/src->vx256 = ((sin1024((src->tmp_angleCCW1024))*(src->KURUKURU_KNIFE_DATA_speed_2nd_256))>>8);/*fps_factor*/
		/*data->*/src->vy256 = ((cos1024((src->tmp_angleCCW1024))*(src->KURUKURU_KNIFE_DATA_speed_2nd_256))>>8);/*fps_factor*/
	}
	else
//	if (0==src->KURUKURU_KNIFE_DATA_target)
	{
//		mono_angle_move(s,(BULLET_ANGLE_DATA *)d);

	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02);
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.02*2);/*2�{�ɂ����̂Œ��덇�킹*/
//		src->KURUKURU_KNIFE_DATA_aaa_speed256 -= t256(0.04);	/* �i�C�t���x�A�����萔 */
	//	src->KURUKURU_KNIFE_DATA_aaa_speed256 -= 1/*t256(0.04)*/; /* �i�C�t���x�A�����萔 */
		src->m_angleCCW1024 += (64);
		mask1024(src->m_angleCCW1024);/*�˂�̂���*/
//		/*data->*/src->vx256 = ((sin512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
//		/*data->*/src->vy256 = ((cos512((src->knife_tmp_angle512/*data->aaa_angle512*/))*(src->KURUKURU_KNIFE_DATA_aaa_speed256))>>8);/*fps_factor*/
	}
	src->x256 += (src->vx256);		/*fps_factor*/
	src->y256 += (src->vy256);		/*fps_factor*/
//
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
//SPRITE *src, int speed256, int angle512, int delta256, int bullet_obj_type);

//	SPRITE *src,
//	int br.BULLET_REGIST_speed256,
//	int br.BULLET_REGIST_angle1024,
//	int BULLET_REGIST_jyuryoku_dan_delta256,
//	int br.BULLET_REGIST_bullet_obj_type	/* �e�O�� */	/*BULLET_MARU8_00_AKA+(7)*/ 	/*(BULLET_KNIFE20_05_AKA)*/

/*bullet_create_sakuya_no_rot_knife*/
static void s_bullet_create_jyuryoku_dan(void)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h					= sprite_add_only_bullet_error();/*�e*/
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= (br.BULLET_REGIST_bullet_obj_type);/*BULLET_MARU8_07_AOI*/  /*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= bullet_move_gravity;	/*enemy_fall_knife_move*/
		#if 1/*Gu(���S���W)*/
		h->x256 			= /*src*/obj_send1->x256;
		h->y256 			= /*src*/obj_send1->y256;
		#endif
	//
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_angle1024);
	//	data->angle512		= (angle512);
	//	data->speed256		= (speed256);
		regist_vector(h, br.BULLET_REGIST_speed256, br.BULLET_REGIST_angle1024);
		h->GRAVITY_BULLET_DATA_delta256 	= (br.BULLET_REGIST_jyuryoku_dan_delta256);
	//	data->speed256		= t256(0);
	}
}


/*---------------------------------------------------------
	���̒����U���i�C�t
	-------------------------------------------------------
	���̒e�͑��̒e�������̂ł����Ēx������B
---------------------------------------------------------*/

global void bullet_crate_sakuya_linear_yuudou(SPRITE *src)
{
	int j;
	for (j=(0); j<(8); j++)
	{
		SPRITE *h;
		h						= sprite_add_only_bullet_error();
		if (NULL != h)
		{
			h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
			const unsigned int aaa = ((ra_nd()&(4-1))); 	/* �i�C�t�̐F�̓����_�� */
		//	h->type 			= BULLET_MARU8_00_AKA+4+(aaa);/*S P_BULLET*/ /*SP_LASER*/
			h->type 			= BULLET_KNIFE20_04_AOI+(aaa);/*S P_BULLET*/ /*SP_LASER*/
			h->callback_mover	= move_bullet_sakuya_linear_yuudou;
			#if 1/*Gu(���S���W)*/
			h->x256 			= src->x256;
			h->y256 			= src->y256;
			#endif
			h->base_time_out	= 2048+20+(40+1);		/*data->timer		= 0;*/

			int angle1024		= src->tmp_angleCCW1024+((j-4)*84); 	/* 85.333 == cv1024r(30) */
			mask1024(angle1024);
			h->m_angleCCW1024 = angle1024;
			h->vx256 = ((sin1024((angle1024))*t256(1.0))>>8);/*fps_factor*/
			h->vy256 = ((cos1024((angle1024))*t256(1.0))>>8);/*fps_factor*/

			h->BULLET_YUUDOU_DATA_hiritu256 	= t256(1.0);	/* (2.5==5*0.5) */
		//
			#if 1
			/* �Ƃ肠�����A��{�� */
		//	h->callback_2nd_regist	= callback_2nd_regist;
		//
			/* �Ƃ肠�����A�e�L�g�[ */
			h->BULLET_DATA_2nd_count	= (999/*30*/);		/* 30�J�E���g�����甼���� */
			h->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
			#endif
		}
	}
}

#if 1/* ���^��`�������w�p */
/*---------------------------------------------------------
	����		B.���^��`�������w�p
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/

global /*static*/ void bullet_crate_sakuya_hosi_gata(SPRITE *src)
{		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	SPRITE *h;
	h					= sprite_add_only_bullet_error();
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
		h->callback_mover	= move_bullet_sakuya_hosi_gata;
		#if 1/*Gu(���S���W)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
		h->vx256			= (0);
		h->vy256			= (0);
		h->BULLET_HOSI_GATA_DATA_star_remain_time	= (br.BULLET_REGIST_hosi_gata_time_out);
		h->BULLET_HOSI_GATA_DATA_angle1024			= (br.BULLET_REGIST_hosi_gata_angle1024);
		h->BULLET_HOSI_GATA_DATA_speed256			= t256(0);
		h->BULLET_HOSI_GATA_DATA_add_delta256		= (br.BULLET_REGIST_hosi_gata_add_speed256);

	/*����*/
	//
		#if 1
		/* �Ƃ肠�����A��{�� */
	//	h->callback_2nd_regist	= callback_2nd_regist;
	//
		/* �Ƃ肠�����A�e�L�g�[ */
		h->BULLET_DATA_2nd_count	= (999/*30*/);			/* 30�J�E���g�����甼���� */
		h->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
		#endif
		/* �Ƃ肠�����A��{�� */
		h->callback_2nd_regist	= NULL;
	}
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
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
	//SPRITE *src, int speed256, int angle512, int a256, int next_angle512);
//SPRITE *src,
//	int br.BULLET_REGIST_speed256,				/* �e�� */
//	int br.BULLET_REGIST_angle1024, 			/* �p�x */
//	int br.BULLET_REGIST_tomari2_dan_delta256,	/* �����萔 */
//	int br.BULLET_REGIST_tomari2_dan_next_angle1024


// bullet_create_tomari_dan
static void s_bullet_create_tomari2_dan(void)
{
	SPRITE *h;
	h					= sprite_add_only_bullet_error();	/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/	/*���Ԓe TAMA_TYPE_KUGEL_PNG */
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= BULLET_MARU8_00_AKA+(0);/*S P_BULLET*/
		h->callback_mover	= enemy_stop_bullet2_move;
		#if 1/*Gu(���S���W)*/
		h->x256 			= /*src*/obj_send1->x256;
		h->y256 			= /*src*/obj_send1->y256;
		#endif
		{
			/*src*/obj_send1->tmp_angleCCW1024	= br.BULLET_REGIST_angle1024;
			s_tmp_angleCCW1024_jikinerai_auto(obj_player, /*src*/obj_send1);
			br.BULLET_REGIST_angle1024				= /*src*/obj_send1->tmp_angleCCW1024;
		}
	//	data->angle512		= (angle512);
	//	data->speed256		= (speed256);
		h->TOMARI_DAN_DATA_speed256 	= (br.BULLET_REGIST_speed256);
		regist_vector(h, t256(1.0)/*speed256*/, br.BULLET_REGIST_angle1024);
		h->TOMARI_DAN_DATA_init_vx256	= /*data->*/h->vx256;
		h->TOMARI_DAN_DATA_init_vy256	= /*data->*/h->vy256;
	//
		//data->state		= 0;
		h->TOMARI_DAN_DATA_delta256 	= (br.BULLET_REGIST_tomari2_dan_delta256);
		if (ANGLE_NO_SHOT_DAN==br.BULLET_REGIST_tomari2_dan_next_angle1024)/* ���ʂɌ����Ȃ��e�A�̏ꍇ */
		{
			;// 	h->base_time_out	= 800+64;
		}
		else
		{
			;// 	h->base_time_out	= 100+64;
			mask1024(br.BULLET_REGIST_tomari2_dan_next_angle1024);
		}
		h->TOMARI_DAN_next_angle1024	= (br.BULLET_REGIST_tomari2_dan_next_angle1024);
		h->base_time_out				= 1024;/* �K���ɑ傫��1024==(512+64+�ő��~����) */
	}
}


/*---------------------------------------------------------
	���邭��i�C�t(����)
	-------------------------------------------------------
	�S�����i�C�t(��)
	�_���i�C�t(��)
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
	//SPRITE *src, int speed256, int angle512, int height);

//	SPRITE *src,
//	int speed256,	/* �e�� */
//	int angle512,	/* �p�x */
//	int height		/* (?) */
//)
static void s_bullet_create_sakuya_222kurukuru_knife(void)
{/* x, y*/
	SPRITE *h;
	h					= sprite_add_only_bullet_error();/*�S�����i�C�t(��)*/
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
		h->type 			= /*BULLET_KNIFE20_07_MIDORI*/BULLET_KNIFE20_04_AOI;/*BULLET_KNIFE20_04_AOI*/	/* (���΃i�C�t�ɕϐg) */		/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= bullet_move_kurukuru_knife;

		#if 1/*Gu(���S���W)*/
		h->x256 			= obj_send1->x256;
		h->y256 			= obj_send1->y256;/*x*/
		#endif
		h->base_time_out	= (300);
	//	h->knife_tmp_angle512/*data->aaa_angle512*/ 	= (br.BULLET_REGIST_angle512);
	//
		{
			int tmp_speed256;
			tmp_speed256 = (br.BULLET_REGIST_speed256);
	//		h->KURUKURU_KNIFE_DATA_aaa_speed256 	= (tmp_speed256/*+tmp_speed256*/);	/* �i�C�t�̓��������x */
			h->KURUKURU_KNIFE_DATA_speed_2nd_256	= (tmp_speed256+tmp_speed256);		/* �΃i�C�t��2nd�_�����x */
		}
		h->vx256			= (0);
		h->vy256			= (0);
	//	h->KURUKURU_KNIFE_DATA_target			= 0;
		h->KURUKURU_KNIFE_DATA_height			= (br.BULLET_REGIST_sakuya_kurukurku_knife_height);
	}
}

/*---------------------------------------------------------
	���p�̑�e(��)�B
	-------------------------------------------------------
	�����ځ��������蔻��ł��B
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/

static void bullet_create_oodama22(SPRITE *src)
{
	SPRITE *h;
	h					= sprite_add_only_bullet_error();  /* ��e(��) �\������*/
	if (NULL != h)
	{
	//	angle512			= angle_jikinerai512_auto(obj_player, src, angle512);
	//	mask512(angle512);
	//
		int angle1024;
		angle1024			= (src->TOMARI_DAN_next_angle1024);
	//
		h->m_Hit256R		= TAMA_ATARI_OODAMA_08_PNG;
		h->type 			= BULLET_OODAMA32_00_SIROI+((((angle1024>>7)&(0x03))));
		h->callback_mover	= move_bullet_vector;	/* (�v�����@�\) */
		#if 1/*Gu(���S���W)*/
		h->x256 			= src->x256;
		h->y256 			= src->y256;
		#endif
	//
		int speed256;
		speed256 = (t256(1.00));
		regist_vector(h, speed256, angle1024);
		h->BULLET_DATA_2nd_count	= (300);			/* 30�J�E���g�����甼���� */
		h->BASE_add_speed256		= (t256(1.05)); 	/* ���� */ /*add_speed256*/
	}
}

/*---------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	�P��A�ŏI�`�Ԃœ����Ă��邩�Ȃ薳���Ȓe�B
	-------------------------------------------------------
	delta256	�t���[�����ɉ������ɂǂꂾ���������邩
---------------------------------------------------------*/
//extern void bullet_create_hazumi_dan(void);
//SPRITE *src, int speed256, int angle512, int delta256, int bound_counts);

// [***090114	�ǉ�
//	SPRITE *src,
//	int br.BULLET_REGIST_speed256,
//	int br.BULLET_REGIST_angle1024,
//	int BULLET_REGIST_jyuryoku_dan_delta256,
//	int BULLET_REGIST_jyuryoku_dan_bound_counts

static void s_bullet_create_hazumi_dan(void)
{
	SPRITE *h;			/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	h					= sprite_add_only_bullet_error();/*�Ή��e*/
	if (NULL != h)
	{
		h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		h->type 			= BULLET_MARU8_00_AKA+(4+br.BULLET_REGIST_jyuryoku_dan_bound_counts);/*S P_BULLET*/ /*SP_LASER*/
		h->callback_mover	= bullet_move_hazumi_dan;
		#if 1/*Gu(���S���W)*/
		h->x256 			= /*src*/obj_send1->x256;
		h->y256 			= /*src*/obj_send1->y256;
		#endif
		/*angle512			= angle_jikinerai512_auto(obj_player, src, angle512);*/
		mask1024(br.BULLET_REGIST_angle1024);
	//	h->HAZUMI_DAN_DATA_angle512 	= (angle512);
	//	h->HAZUMI_DAN_DATA_speed256 	= (speed256);
		regist_vector(h, br.BULLET_REGIST_speed256, br.BULLET_REGIST_angle1024);

		h->HAZUMI_DAN_DATA_delta256 	= (br.BULLET_REGIST_jyuryoku_dan_delta256);
	//	h->HAZUMI_DAN_DATA_sum256		= t256(0);
		h->HAZUMI_DAN_DATA_bound_counts = br.BULLET_REGIST_jyuryoku_dan_bound_counts;
	}
}















/*---------------------------------------------------------
	�������A�������i�e��ǉ�����
	-------------------------------------------------------

---------------------------------------------------------*/
/*(int)(512/24)*/

//	SPRITE *src,			/*	*/
//	int BULLET_REGIST_speed256, 		/* �e�� */
//	int BULLET_REGIST_angle512, 		/* ���˒��S�p�x / ����@�\(���@�_��/��) */
//	int BULLET_REGIST_div_angle512, 	/* 1����n���������p�x */
//	int BULLET_REGIST_bullet_obj_type,	/* �e�O�� */
//	int BULLET_REGIST_n_way 			/* ���e�� */
	/*
		�悭�l������AANGLE_JIKI_NERAI_KISUDAN �͕K�v�Ȃ��B
		ANGLE_JIKINERAI_DAN ��������Ηǂ��A
		���e���ɋ������w�肷��΋����e�B
		���e���Ɋ���w�肷��Ί�e�B
		������܂������ǁA�C���t���Ȃ�����ww�B
	*/
static void s_bullet_regist_multi_angle(void)
{
	{
		obj_send1->tmp_angleCCW1024 		= br.BULLET_REGIST_angle1024;
		s_tmp_angleCCW1024_jikinerai_auto(obj_player, obj_send1/*src*/ ); /*ANGLE_JIKI_NERAI_DAN???(original)*/ /* ��e�̏ꍇ�Ɏ��@�_�� */
		br.BULLET_REGIST_angle1024	= obj_send1->tmp_angleCCW1024;
	}
	int i_angle1024;
	i_angle1024 		= br.BULLET_REGIST_angle1024-((br.BULLET_REGIST_n_way+2)*((br.BULLET_REGIST_div_angle1024>>1)/*(int)(1024/48)*/)); /* 48�����e */ // /* ��/24 0.13089969389957471827 */
	/* �O�̈׃}�X�N */
//	mask1024(i_angle1024);
//
	int i;
	for (i=0; i<br.BULLET_REGIST_n_way; i++)
	{
		SPRITE *h;
		// frame�̓t���[����-1 /*char *filename, int frame,*/
		h					= sprite_add_only_bullet_error(); /*_MING32_PNG*/
		if (NULL != h)
		{
			h->m_Hit256R		= TAMA_ATARI_COMMON16_PNG;
			h->type 			= (br.BULLET_REGIST_bullet_obj_type);/*BULLET_UROKO14_01_AKA+*/ /*S P_BULLET*/
			h->callback_mover	= move_bullet_vector;
			#if 1/*Gu(���S���W)*/
			h->x256 			= /*src*/obj_send1->x256;
			h->y256 			= /*src*/obj_send1->y256;
			#endif
			i_angle1024 		+= br.BULLET_REGIST_div_angle1024;	/*(2)*((int)(1024/48))*/ /* ��/12 0.26179938779914943654 */
			mask1024(i_angle1024);
			h->m_angleCCW1024		= i_angle1024;		/* �`��p�p�x(����0�x�ō����(�����v���)) */
			regist_vector(h, br.BULLET_REGIST_speed256, i_angle1024);
		}
	}
}


/*---------------------------------------------------------
	�e��o�^����B
	-------------------------------------------------------
	�֐���(�N���X���ۂ�)�B���B
	�e�������̃G�t�F�N�g���̊֌W��A���e�����͑S���Z�߂�K�v������B
	-------------------------------------------------------
	�x�N�g���e�Ɗp�x�e�𓝍����邩������Ȃ��B
---------------------------------------------------------*/

global void bullet_regist_vector(void)
{
	#if 1
	switch (br.BULLET_REGIST_regist_type)
	{
	case REGIST_TYPE_00_MULTI_VECTOR:	s_bullet_regist_multi_angle();					break;
	case REGIST_TYPE_01_HAZUMI: 		s_bullet_create_hazumi_dan();					break;
	case REGIST_TYPE_02_GRAVITY02:		s_bullet_create_jyuryoku_dan(); 				break;
	case REGIST_TYPE_03_TOMARI: 		s_bullet_create_tomari2_dan();					break;
	case REGIST_TYPE_04_KURU_KURU:		s_bullet_create_sakuya_222kurukuru_knife(); 	break;
	}
	#else
	const void (*aaa_func[(8)])(void) =
	{
		s_bullet_regist_multi_angle,					/* REGIST_TYPE_00_MULTI_VECTOR	*/
		s_bullet_create_hazumi_dan, 					/* REGIST_TYPE_01_HAZUMI		*/
		s_bullet_create_jyuryoku_dan,					/* REGIST_TYPE_02_GRAVITY02 	*/
		s_bullet_create_tomari2_dan,					/* REGIST_TYPE_03_TOMARI		*/
//
		s_bullet_create_sakuya_222kurukuru_knife,		/* REGIST_TYPE_04_KURU_KURU 	*/
		NULL,
		NULL,
		NULL,
	};
	(*aaa_func[ (br.BULLET_REGIST_regist_type/*&0x07*/) ])();	/* �e�֐��ɕ��򂷂� */
	#endif
}
