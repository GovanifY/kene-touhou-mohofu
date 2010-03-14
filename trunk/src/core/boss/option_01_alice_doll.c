
#include "bullet_object.h"

/*---------------------------------------------------------
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
	send1_obj->x256 = src->x256;		/* �e��x256 */
	send1_obj->y256 = src->y256;		/* �e��y256 */
	send1_obj->BULLET_REGIST_speed256			=	(t256(2.0));						/* �e�� */		/*+((src->boss_base_danmaku_time_out)<<2)*/
	send1_obj->BULLET_REGIST_angle512			=	(src->m_angleCCW512);				/* src->tmp_angleCCW512 */
	send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/23);						/* �p�x (512/27) (512/24) */
	send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_00_AOI;				/* [�ؒe] */
	send1_obj->BULLET_REGIST_n_way				=	(7);								/* [7way] [8way] */
	bullet_regist_basic();
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
			//	bullet_create_aka_maru_jikinerai(src, t256(2.0) );
				add_zako_alice_doll_2nd_CCW(src);
			}
		}
		src->type = SP_DELETE;	/* �����܂� */
	//	return;
	}
	else
	{
		src->m_angleCCW512 += (((src->recursive)&(1))?(5):(-5));/* �l�`���O����] */
		mask512(src->m_angleCCW512);
		{
			int bbb_m_angleCCW512;
			bbb_m_angleCCW512 = src->m_angleCCW512+(64);/* 64==45�x�X���� */
		//	bbb_m_angleCCW512 = src->m_angleCCW512-(64);/* -64 == -45�x�X���� */
			mask512(bbb_m_angleCCW512);
			src->radius++;
			src->x256 = src->sx256 + ((sin512((bbb_m_angleCCW512))*src->radius));/*fps_factor*/
			src->y256 = src->sy256 + ((cos512((bbb_m_angleCCW512))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
//	const int add_angle = ( (512/7) );	/* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
	int i_angle;	/* �ώZ�p�x */
	for (i_angle=0; i_angle<(512);	i_angle += (512/7)/*add_angle*/)	/* �e�� */
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= /*TEKI_16_YOUSEI1_5*/TEKI_12_YOUSEI1_1+((1)<<2)+((src->recursive)<<2); 	/*SP_ZAKO*/ /*_02_YUKARI1*/
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover		= move_alice_doll_all;
	//	h->callback_loser		= NULL;
		h->callback_hit_enemy	= callback_hit_zako;
		h->m_angleCCW512		= (i_angle);
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
		h->radius		= (0);
		h->recursive	= ( ((src->recursive)-1) );
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/9),
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/42),
//		(int)(512/4), (int)(512/6), (int)(512/8), (int)(512/12),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/20),/* 20 Lunatic ��� */
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/17),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/8),
		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/2),/*(�I�����A���l�`)*/
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/3),/*(�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
	};
	const int add_angle = ( (aaa_tbl[(difficulty)]));	/* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */

	int i_angle;	/* �ώZ�p�x */
	for (i_angle=0; i_angle<(512);	i_angle += add_angle)	/* ��� */
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		h->callback_mover		= move_alice_doll_all;
	//	h->callback_loser		= NULL;
//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
		h->m_angleCCW512		= (i_angle);
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
		h->radius		= (0);
		h->recursive	= ( ((3==difficulty)?(1):(0)));/*(Lunatic==�I�����A���l�`)*/
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
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/9),
//		(int)(512/3), (int)(512/5), (int)(512/7), (int)(512/42),
//		(int)(512/4), (int)(512/6), (int)(512/8), (int)(512/12),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/20),/* 20 Lunatic ��� */
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/17),
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/8),
		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/2),/*(Lunatic==�I�����A���l�`)*/
//		(int)(512/3), (int)(512/5), (int)(512/9), (int)(512/3),/*(Lunatic==�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
	};
	const int add_angle = ((0==is_the_first)?(512/7):(aaa_tbl[(difficulty)]));	/* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
//
	int i_angle;	/* �ώZ�p�x */
	for (i_angle=0; i_angle<(512);	i_angle += add_angle)	/* �e�� */
	{
		SPRITE *h;
		h						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		h->type 				= TEKI_12_YOUSEI1_1+((src->recursive)<<2);
		h->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	//
		h->callback_mover		= move_alice_doll_all;
	//	h->callback_loser		= NULL;
		h->callback_hit_enemy	= callback_hit_zako;
	//
	//	h->base_score			= score(25*2);/*�_�~�[*/
		h->base_hp				= (9999);/*�|���Ȃ�*/
	//
		h->m_angleCCW512		= (i_angle);
		h->time_out 			= 60;
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
