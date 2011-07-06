
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
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

#define sx256		user_data00 	/* ��Œ� x�ʒu */
#define sy256		user_data01 	/* ��Œ� y�ʒu */
#define time_out	user_data02 	/* �o�ߎ��� */
#define radius		user_data03 	/* �~�̔��a */
//#define recursive user_data04 	/* �����(0>���􂵂Ȃ�) */


/*---------------------------------------------------------
	����(last)
	-------------------------------------------------------
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,		normal
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,	luna
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(SPRITE *src)
{
	obj_send1->cx256 					= (src->cx256);		/* �e��x256 */
	obj_send1->cy256 					= (src->cy256);		/* �e��y256 */
//	br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� */
	br.BULLET_REGIST_speed256			= (t256(1.0));						/* �e�� */
	br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW1024)<<6); 		/* �p�x */
	br.BULLET_REGIST_div_angle65536 	= (int)(65536/23);					/* �����p�x (1024/27) (1024/24) */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI+1/*+(difficulty)*/;			/* [�ԗؒe] */
	br.BULLET_REGIST_n_way					= (7);								/* [7way] [8way] */
	br.BULLET_REGIST_speed_offset			= t256(1);/*�Ă���*/
//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* ���݂��ꂵ���Ȃ����v�� */
	bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_alice_doll_all(SPRITE *src)
{
	src->time_out--;/* ���Ԍo�� */
	if ( 0 > src->time_out )/* �ړ��I�� */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			move_alice_doll_last_burrets(src);/* �e�����B */
		}
		src->jyumyou = JYUMYOU_NASI;	/* �����܂� */
	//	return;
	}
	else/* �ړ����� */
	{
		src->rotationCCW1024 	+= ( (-10));	/* �l�`�O����] */
		mask1024(src->rotationCCW1024);
	//
//		src->tmp_angleCCW1024 += (((src->recursive)&(1))?(10):(-10));/* �l�`���O����] */
		src->tmp_angleCCW1024	+= ( (-10));	/* �l�`�ʒu��] */
		mask1024(src->tmp_angleCCW1024);
		{
			int bbb_tmp_angleCCW1024;
			bbb_tmp_angleCCW1024 = src->tmp_angleCCW1024+(1024/8);/*  (1024/8) ==  45/360�x�X���� */
		//	bbb_tmp_angleCCW1024 = src->tmp_angleCCW1024-(1024/8);/* -(1024/8) == -45/360�x�X���� */
			mask1024(bbb_tmp_angleCCW1024);
			src->radius++;
			src->cx256 = src->sx256 + ((sin1024((bbb_tmp_angleCCW1024))*src->radius));/*fps_factor*/
			src->cy256 = src->sy256 + ((cos1024((bbb_tmp_angleCCW1024))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

void add_zako_alice_doll_type_b(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[�͕핗]*/ 	/*(�I�����A���l�`)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
//		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[�d�X����]*/	/*(�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/8),/*[�d�X����]*/	/*(�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
		/* �͕핗��r32���݂����蔻��Ӑ}�I�ɏ��������Ă���̂ŁA������1�ʂƂ͂���
			����ϓ�Փx�Ⴗ����C������B(�I�����A���l�`)�ȊO�d�X�����ɂ����B */
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
			h->tmp_angleCCW1024 	= (i_angle1024);
			h->rotationCCW1024		= (0);	/* �`��p�p�x */
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*�_�~�[*/
			/*data->base.*/h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->time_out 			= (60);
			{
				/* �����ʒu */
			/*	h->cx256 =*/ h->sx256 = (src->cx256);
			/*	h->cy256 =*/ h->sy256 = (src->cy256);
			//
		//		h->vx256 = (0);/*�E����*/
		//		h->vy256 = (0);/*������*/
			}
			h->radius				= (0);
//			h->recursive			= ( ((3==difficulty)?(1):(0)));/*(Lunatic==�I�����A���l�`)*/
		}
	}
}

