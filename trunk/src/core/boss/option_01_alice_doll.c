
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�l�`�e��
	-------------------------------------------------------
�����u�����̕������l�` -easy-�v
�A���X�̕������l�̂̐l�`���A��񕪗􂷂�7way�e�����B
4 x 7 x 7 == �ő� 196[�e](�d�X��)
3 x 7 x 7 == �ő� 147[�e](�͕핗)	//3 x 8 x 8 == 192
	-------------------------------------------------------
�����u�����̕������l�`�v
�A���X�̕������Z�̂̐l�`���A��񕪗􂷂�7way�e�����B
6 x 7 x 7 == �ő� 294[�e](�d�X��)
5 x 7 x 7 == �ő� 245[�e](�͕핗)	//5 x 8 x 8 == 320
	-------------------------------------------------------
�����u�����̕������l�` -hard-�v
�A���X�̕��������̂̐l�`���A��񕪗􂷂�7way�e�����B
8 x 7 x 7 == �ő� 392[�e](�d�X��)
9 x 7 x 7 == �ő� 441[�e](�͕핗)	//7 x 8 x 8 == 448
	-------------------------------------------------------
�����u�����̃I�����A���l�`�v
�A���X�̕��������̂̐l�`���A�O�񕪗􂷂�7way�e�����B
8 x 7 x 7 x 7 == �ő�2744[�e](�d�X��)
2 x 7 x 7 x 7 == �ő� 686[�e](�͕핗)	//9 x 8 x 8 == 576
---------------------------------------------------------*/

#define sx256		user_data00 	/* ��Œ� x�ʒu */
#define sy256		user_data01 	/* ��Œ� y�ʒu */
#define time_out	user_data02 	/* �o�ߎ��� */
#define radius		user_data03 	/* �~�̔��a */
#define recursive	user_data04 	/* �����(0>���􂵂Ȃ�) */


/*---------------------------------------------------------
	����(last)
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(SPRITE *src)
{
	obj_send1->x256 					= (src->x256);		/* �e��x256 */
	obj_send1->y256 					= (src->y256);		/* �e��y256 */
	br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� */
	br.BULLET_REGIST_angle1024			= (src->m_angleCCW1024);			/* �p�x */
	br.BULLET_REGIST_div_angle1024		= (int)(1024/23);					/* �����p�x (1024/27) (1024/24) */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;			/* [�ؒe] */
	br.BULLET_REGIST_n_way				= (7);								/* [7way] [8way] */
	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	bullet_regist_vector();
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src);

static void move_alice_doll_all(SPRITE *src)
{
	src->time_out--;
	if (( 0 > src->time_out ))
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			if (0 > src->recursive)
			{
				move_alice_doll_last_burrets(src);
			}
			else
			{
				add_zako_alice_doll_2nd_CCW(src);
			}
		}
		src->type = SP_DELETE;	/* �����܂� */
	//	return;
	}
	else
	{
		src->m_angleCCW1024 += (((src->recursive)&(1))?(10):(-10));/* �l�`���O����] */
		mask1024(src->m_angleCCW1024);
		{
			int bbb_m_angleCCW1024;
			bbb_m_angleCCW1024 = src->m_angleCCW1024+(1024/8);/*  (1024/8) ==  45/360�x�X���� */
		//	bbb_m_angleCCW1024 = src->m_angleCCW1024-(1024/8);/* -(1024/8) == -45/360�x�X���� */
			mask1024(bbb_m_angleCCW1024);
			src->radius++;
			src->x256 = src->sx256 + ((sin1024((bbb_m_angleCCW1024))*src->radius));/*fps_factor*/
			src->y256 = src->sy256 + ((cos1024((bbb_m_angleCCW1024))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
//	const int add_angle = ( (1024/7) ); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (1024/7)/*add_angle*/)	/* �e�� */
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= /*TEKI_16_YOUSEI1_5*/TEKI_12_YOUSEI1_1+((1)<<2)+((src->recursive)<<2);	/*SP_ZAKO*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_enemy	= callback_hit_zako;
			h->m_angleCCW1024		= (i_angle1024);
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*�_�~�[*/
			/*data->base.*/h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->time_out 			= (60);
			{
				/* �����ʒu */
			/*	h->x256 =*/ h->sx256 = (src->x256);
			/*	h->y256 =*/ h->sy256 = (src->y256);
			//
		//		h->vx256 = (0);/*�E����*/
		//		h->vy256 = (0);/*������*/
			}
			h->radius				= (0);
			h->recursive			= ( ((src->recursive)-1) );
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
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/9),
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/42),
//		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/12),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/20),/* 20 Lunatic ��� */
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/17),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/8),
		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*(�I�����A���l�`)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
	};
	const int add_angle1024 = ( (aaa_tbl[(difficulty)]));	/* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */

	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* ��� */
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
			h->m_angleCCW1024		= (i_angle1024);
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*�_�~�[*/
			/*data->base.*/h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->time_out 			= (60);
			{
				/* �����ʒu */
			/*	h->x256 =*/ h->sx256 = (src->x256);
			/*	h->y256 =*/ h->sy256 = (src->y256);
			//
		//		h->vx256 = (0);/*�E����*/
		//		h->vy256 = (0);/*������*/
			}
			h->radius				= (0);
			h->recursive			= ( ((3==difficulty)?(1):(0)));/*(Lunatic==�I�����A���l�`)*/
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
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/9),
//		(int)(1024/3), (int)(1024/5), (int)(1024/7), (int)(1024/42),
//		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/12),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/20),/* 20 Lunatic ��� */
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/17),
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/8),
		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*(Lunatic==�I�����A���l�`)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(Lunatic==�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
	};
	const int add_angle1024 = ((0==is_the_first)?(1024/7):(aaa_tbl[(difficulty)])); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
//
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* �e�� */
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((src->recursive)<<2);
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
		//
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_enemy	= callback_hit_zako;
		//
		//	h->base_score			= score(25*2);/*�_�~�[*/
			h->base_hp				= (9999);/*�|���Ȃ�*/
		//
			h->m_angleCCW1024		= (i_angle1024);
			h->time_out 			= (60);
			{
				/* �����ʒu */
			/*	h->x256 =*/ h->sx256 = (src->x256);
			/*	h->y256 =*/ h->sy256 = (src->y256);
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
