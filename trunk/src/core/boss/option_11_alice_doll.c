
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�l�`�e��
	-------------------------------------------------------
�����u�����̕������l�` -easy-�v
�A���X�̕������l�̂̐l�`���A��񕪗􂷂�7way�e�����B
4 x 7 x 7 == �ő� 196[�e](�d�X��)
3 x 7 x 7 == �ő� 147[�e](�͕핗)	//3 x 8 x 8 == 192
	-------------------------------------------------------
�����u�����̕������l�`�v	�ԗؒe
�A���X�̕������Z�̂̐l�`���A��񕪗􂷂�7way�e�����B
6 x 7 x 7 == �ő� 294[�e](�d�X��)
5 x 7 x 7 == �ő� 245[�e](�͕핗)	//5 x 8 x 8 == 320
	-------------------------------------------------------
�����u�����̕������l�` -hard-�v
�A���X�̕��������̂̐l�`���A��񕪗􂷂�7way�e�����B
8 x 7 x 7 == �ő� 392[�e](�d�X��)
9 x 7 x 7 == �ő� 441[�e](�͕핗)	//7 x 8 x 8 == 448
	-------------------------------------------------------
�����u�����̃I�����A���l�`�v	�ԗؒe���Ηؒe
�A���X�̕��������̂̐l�`���A�O�񕪗􂷂�7way�e�����B
8 x 7 x 7 x 7 == �ő�2744[�e](�d�X��)
2 x 7 x 7 x 7 == �ő� 686[�e](�͕핗)	//9 x 8 x 8 == 576
	-------------------------------------------------------
	�l�`������(�͕핗)
	�e������(�d�X��)
	���ݐl�`�͎G���𗘗p���Ă��邪�A
	���̂悤�ȕ����ɂȂ��Ă���̂́A�������ׁ̈B
	-------------------------------------------------------
	���ہu����v��(�e�͒e�����X�g�Ȃ̂�)�e�����X�g�𒲂ׂ�
	�u(�p�x��)�ύX�v�{�u(�V���ɒe��)�ǉ��v����񂶂�Ȃ����ȁH
	�܂��G�c�Ɏd�g�݂������΁A�u����v�e����Ȃ��āu�ǉ��v�e�B
	�u����v�Ɓu�ǉ��v���ƁA���ʓI�ɉ����Ⴄ���ƌ����΁A
	�ŏI�I�Ȓe�́u�\���v���C�I���e�B�[�v�ɔ����ȈႢ���o��B
	-------------------------------------------------------
	�܂�A7way�e�ɕ��􂷂�̂ł͂Ȃ��āA
	1�e�͕ϐg�����āA6�e�ǉ�����̂��Ǝv���B
---------------------------------------------------------*/

#define target_x256 	user_data00 	/* ��Œ� x�ʒu */
#define target_y256 	user_data01 	/* ��Œ� y�ʒu */
#define radius			user_data02 	/* �~�̔��a */
#define boss_time_out	user_data03 	/* �o�ߎ��� */
#define recursive		user_data04 	/* �����(0>���􂵂Ȃ�) */


/*---------------------------------------------------------
	����(last)
	-------------------------------------------------------
	BU LLET_UROKO14_00_AOI,
	BU LLET_UROKO14_01_AKA, 	normal
	BU LLET_UROKO14_02_YUKARI,
	BU LLET_UROKO14_03_MIDORI,	luna
//
	BULLET_UROKO14_00_SIRO, 		//��	//BU LLET_UROKO14_00_AOI,		//
	BULLET_UROKO14_01_AKA,			//��	//BU LLET_UROKO14_01_AKA,		//
	BULLET_UROKO14_02_YUKARI,		//��	//BU LLET_UROKO14_02_YUKARI,	//
	BULLET_UROKO14_03_AOI,			//��	//BU LLET_UROKO14_03_MIDORI,	//
	BULLET_UROKO14_04_MIZUIRO,		//��	//BU LLET_UROKO14_04_MIZUIRO,	//
	BULLET_UROKO14_05_MIDORI,		//��	//BU LLET_UROKO14_05_KIIRO, 	//
	BULLET_UROKO14_06_KIIRO,		//��	//BU LLET_UROKO14_06_dummy, 	//
	BULLET_UROKO14_07_DAIDAI,		//��	//BU LLET_UROKO14_07_dummy, 	//
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(SPRITE *src)
{
	obj_send1->cx256					= (src->cx256); 	/* �e��x256 */
	obj_send1->cy256					= (src->cy256); 	/* �e��y256 */
	#if 0
	br.BULLET_REGIST_00_speed256				= (t256(2.0));						/* �e�� */
	br.BULLET_REGIST_02_VECTOR_angle1024		= (src->rotationCCW1024);			/* �p�x */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_03_AOI+((cg_game_difficulty)); 		/* [�ؒe] */
	br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_06_n_way					= (7);								/* [7way] [8way] */
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/23);					/* �����p�x (1024/27) (1024/24) */
	bul let_reg ist_vec tor();
	#endif
	#if 1
	const u8 tama_color[4] =
	{
		3,//BULLET_UROKO14_03_AOI,
		1,//BULLET_UROKO14_01_AKA,
		2,//BULLET_UROKO14_02_YUKARI,
		5 //BULLET_UROKO14_05_MIDORI
	};
	br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_00_SIRO+(tama_color[((cg_game_difficulty))]);			/* [�ؒe] */
//����br.BULLET_REGIST_05_regist_type			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
//	br.BULLET_REGIST_06_n_way					= (1);								/* [7way] [8way] */
//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/23);					/* �����p�x (1024/27) (1024/24) */
	int first_angle_65536;
	first_angle_65536 = ((src->rotationCCW1024)<<6);
	unsigned int i;
	for (i=(0); i<(7); i += (1) )	/* �e�� */
	{
		enum
		{
			CCC_00_SPEED256 = 0,
			CCC_01_ADD_ANGLE65536,
			CCC_99_MAX
		};
		const int ccc[8][CCC_99_MAX] =
		{
			{ t256(1.00), ((65536*10)/(14))},	// ���x����
			{ t256(0.50), ((65536* 8)/(14))},	// ���x�x��
			{ t256(1.00), ((65536* 9)/(14))},	// ���x����
			{ t256(0.75), ((65536* 7)/(14))},	// ���x��
			//
			{ t256(1.00), ((65536* 6)/(14))},	// ���x����
			{ t256(0.50), ((65536* 5)/(14))},	// ���x�x��
			{ t256(1.00), ((65536* 4)/(14))},	// ���x����
			{ t256(1.00), ((65536* 7)/(14))},	// [���g�p](.align)
		};
		br.BULLET_REGIST_00_speed256				= (ccc[i][CCC_00_SPEED256]);									/* �e�� */
		br.BULLET_REGIST_02_angle65536				= (first_angle_65536+ccc[i][CCC_01_ADD_ANGLE65536]);			/* �p�x */
		tama_system_regist_single();/* (r33-) */
	}
	#endif
}
/*
	180�x���]����B
	�V�e�ɕ���
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
//
*/
/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src);

static void move_alice_doll_all(SPRITE *src)
{
	src->boss_time_out--;/* ���Ԍo�� */
	if ( 0 > src->boss_time_out )/* �ړ��I�� */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			if (0 > src->recursive)/* �ĕ��􂷂�K�v����H */
			{
				move_alice_doll_last_burrets(src);/* �ĕ��􂵂Ȃ��B�̂Œe�ɂ���B */
			}
			else/* �ĕ��􂷂� */
			{
				add_zako_alice_doll_2nd_CCW(src);/* (�ċA��)�ĕ��􂷂� */
			}
		}
		src->jyumyou = JYUMYOU_NASI;	/* �����܂� */
	//	return;
	}
	else/* �ړ����� */
	{
		src->rotationCCW1024 += (((src->recursive)&(1))?(10):(-10));/* �l�`���O����] */
		mask1024(src->rotationCCW1024);
		{
			int bbb_rotationCCW1024;
			bbb_rotationCCW1024 = src->rotationCCW1024+(1024/8);/*	(1024/8) ==  45/360�x�X���� */
		//	bbb_rotationCCW1024 = src->rotationCCW1024-(1024/8);/* -(1024/8) == -45/360�x�X���� */
			mask1024(bbb_rotationCCW1024);
			src->radius++;
			src->cx256 = src->target_x256 + ((sin1024((bbb_rotationCCW1024))*src->radius));/*fps_factor*/
			src->cy256 = src->target_y256 + ((cos1024((bbb_rotationCCW1024))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
//	const int add_angle = ( (1024/7) ); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
	int first_angle1024;	/* �J�n�p�x */
	first_angle1024 = (src->rotationCCW1024)+(1024/4);
	int i_angle1024;	/* �ώZ�p�x */
//	for (i_angle1024=(0); i_angle1024<(1024); i_angle1024 += (1024/7)/*add_angle*/) 	/* �e�� */
	/* ������7���� */
	for (i_angle1024=(0); i_angle1024<(1024/2); i_angle1024 += (1024/(7*2))/*add_angle*/)	/* �e�� */
	{
		SPRITE *h;
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= /*TEKI_16_YOUSEI1_5*/TEKI_12_YOUSEI1_1+((1)<<2)+((src->recursive)<<2);	/*SP_ZAKO*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_teki	= callback_hit_zako;
			h->rotationCCW1024		= (first_angle1024+i_angle1024);
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*�_�~�[*/
			/*data->base.*/h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->boss_time_out			= (60);/* 1[�b](60[frame])��ɁA�ĕ��� */
			{
				/* �����ʒu */
			/*	h->cx256 =*/ h->target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->target_y256 = (src->cy256);
			//
		//		h->vx256 = (0);/*�E����*/
		//		h->vy256 = (0);/*������*/
			}
			h->radius				= (0);
			h->recursive			= ( ((src->recursive)-1) );/* �ĕ���񐔂�1���炷�B(�e�����A) */
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[�͕핗]*/ 	/*(�I�����A���l�`)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[�d�X����]*/	/*(�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
		/* �͕핗��r32���݂����蔻��Ӑ}�I�ɏ��������Ă���̂ŁA������1�ʂƂ͂���
			����ϓ�Փx�Ⴗ����C������B(�I�����A���l�`)�ȊO�d�X�����ɂ����B */
	};
	const int add_angle1024 = ( (aaa_tbl[((cg_game_difficulty))])); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */

	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* ��� */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->rotationCCW1024		= (i_angle1024);
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*�_�~�[*/
			/*data->base.*/h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->boss_time_out			= (60);
			{
				/* �����ʒu */
			/*	h->cx256 =*/ h->target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->target_y256 = (src->cy256);
			//
		//		h->vx256 = (0);/*�E����*/
		//		h->vy256 = (0);/*������*/
			}
			h->radius				= (0);
			h->recursive			= ( ((3==(cg_game_difficulty))?(1):(0)));/*(Lunatic==�I�����A���l�`)*/
		}
	}
}



#if 00
/*---------------------------------------------------------
	�G��ǉ�����(common)
---------------------------------------------------------*/

static void add_zako_alice_doll_common(SPRITE *src, int is_the_first)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[�͕핗]*/ 	/*(�I�����A���l�`)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[�d�X����]*/	/*(�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
		/* �͕핗��r32���݂����蔻��Ӑ}�I�ɏ��������Ă���̂ŁA������1�ʂƂ͂���
			����ϓ�Փx�Ⴗ����C������B(�I�����A���l�`)�ȊO�d�X�����ɂ����B */
	};
	const int add_angle1024 = ((0==is_the_first)?(1024/7):(aaa_tbl[(difficulty)])); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
//
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* �e�� */
	{
		SPRITE *h;
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((src->recursive)<<2);
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
		//
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_teki	= callback_hit_zako;
		//
		//	h->base_score			= score(25*2);/*�_�~�[*/
			h->base_hp				= (9999);/*�|���Ȃ�*/
		//
			h->rotationCCW1024		= (i_angle1024);
			h->boss_time_out			= (60);
			{
				/* �����ʒu */
			/*	h->cx256 =*/ h->target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->target_y256 = (src->cy256);
			//
		//		h->vx256 = (0);/*�E����*/
		//		h->vy256 = (0);/*������*/
			}
			h->radius		= (0);
			h->recursive	= ((0==is_the_first)?((src->recursive)-1):((3==difficulty)?(1):(0)));
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
	add_zako_alice_doll_common(src, 0);
}


/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	add_zako_alice_doll_common(src, 1);
}
#endif
