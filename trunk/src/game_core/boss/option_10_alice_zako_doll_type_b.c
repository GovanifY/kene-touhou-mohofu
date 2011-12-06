
#include "boss.h"//#include "game_main.h"

#if (1)
OBJ_CALL_FUNC(add_zako_alice_doll_type_b)
{
	/*(r36���ݎg���ĂȂ�!!!)*/
}

#else

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�l�`�J�[�h
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
	���ہu����v��(�e�͑S�e�z��Ȃ̂�)�S�e�z��𒲂ׂ�
	�u(�p�x��)�ύX�v�{�u(�V���ɒe��)�ǉ��v����񂶂�Ȃ����ȁH
	�܂��G�c�Ɏd�g�݂������΁A�u����v�e����Ȃ��āu�ǉ��v�e�B
	�u����v�Ɓu�ǉ��v���ƁA���ʓI�ɉ����Ⴄ���ƌ����΁A
	�ŏI�I�Ȓe�́u�\���v���C�I���e�B�[�v�ɔ����ȈႢ���o��B
	-------------------------------------------------------
	�܂�A7way�e�ɕ��􂷂�̂ł͂Ȃ��āA
	1�e�͕ϐg�����āA6�e�ǉ�����̂��Ǝv���B
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
		BOSS_DATA_00_target_x256	��Œ���W x�ʒu�Ƃ��Ďg�p�B
		BOSS_DATA_01_target_y256	��Œ���W y�ʒu�Ƃ��Ďg�p�B
		BOSS_DATA_05_move_jyumyou	���Ԃ�B�o�ߎ��ԁB
---------------------------------------------------------*/

// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()


//#define recursive user_data06 	/* �����(0>���􂵂Ȃ�) */
#define radius		user_data07 	/* �~�̔��a */


/*---------------------------------------------------------
	����(last)
	-------------------------------------------------------
	BU LLET_UROKO14_00_AOI,
	BU LLET_UROKO14_01_AKA, 	normal
	BU LLET_UROKO14_02_YUKARI,
	BU LLET_UROKO14_03_MIDORI,	luna
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_alice_doll_last_burrets)
{
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
//	HATSUDAN_01_speed256			= (t256(2.0));						/* �e�� */
	HATSUDAN_01_speed256			= (t256(1.0));						/* �e�� */
	HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
	HATSUDAN_03_angle65536			= ((src->tmp_angleCCW1024)<<6); 		/* �p�x */
	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	HATSUDAN_05_bullet_obj_type 	= BULLET_UROKO14_BASE + TAMA_IRO_01_AKA/*+(difficulty)*/;		/* [�ԗؒe] */
	HATSUDAN_06_n_way				= (7);								/* [7way] [8way] */
	HATSUDAN_07_div_angle65536		= (int)(65536/23);					/* �����p�x (1024/27) (1024/24) */
	hatudan_system_regist_katayori_n_way();/* (r33-) */
}

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_alice_doll_all)
{
	src->BOSS_DATA_05_move_jyumyou--;/* ���Ԍo�� */
	if (0 > src->BOSS_DATA_05_move_jyumyou)/* �ړ��I�� */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->center.y256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			move_alice_doll_last_burrets(src);/* �e�����B */
		}
		src->jyumyou = JYUMYOU_NASI;	/* �����܂� */
	//	return;
	}
	else/* �ړ����� */
	{
		src->rotationCCW1024	+= ( (-10));	/* �l�`�O����] */
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
			#if (0)//
			src->center.x256	= src->BOSS_DATA_00_target_x256 + ((si n1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/	/* CCW�̏ꍇ */
			src->center.x256	= src->BOSS_DATA_00_target_x256 + ((si n1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/
			src->center.y256	= src->BOSS_DATA_01_target_y256 + ((co s1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/
			src->center.y256	= src->BOSS_DATA_01_target_y256 + ((co s1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (bbb_tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
				src->center.x256			= src->BOSS_DATA_00_target_x256 + ((sin_value_t256)*(src->radius));/*fps_factor*/
				src->center.y256			= src->BOSS_DATA_01_target_y256 + ((cos_value_t256)*(src->radius));/*fps_factor*/
			}
			#endif
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

/**/ OBJ_CALL_FUNC(add_zako_alice_doll_type_b)
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
//	if (1 < (REG_0f_GAME_DIFFICULTY/*&3*/))/*(??????�I�������Ȃ��B�����H�H�H)*/
	if (0 != (REG_0f_GAME_DIFFICULTY/*&3*/))/*(easy �ȊO��)*/
	{
		REG_0f_GAME_DIFFICULTY = (1);/*(���� normal �ɂ���B)*/
	}/*(r36-: hard, luna ���n���O�A�b�v����̂ŋ��� normal �ɂ���B)*/
	//
	/*(????????r36���݂��������g���Ă�Badd_zako_alice_doll_type_a�͎g���ĂȂ������B)*/
	//
	const int add_angle1024 = ( (aaa_tbl[((REG_0f_GAME_DIFFICULTY))])); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
	//
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* ��� */
	{
		OBJ *h;
		h							= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->obj_type_set 		= TEKI_16_10;
		//	h->atari_hantei 		= (1����);/*(score���p)*/
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->tmp_angleCCW1024 	= (i_angle1024);
			h->rotationCCW1024		= (0);	/* �`��p�p�x */
	//
	//		h->base_score		= score(25*2);/*�_�~�[*/
			h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->BOSS_DATA_05_move_jyumyou			= (60);
			{
				/* �����ʒu */
			/*	h->center.x256 =*/ h->BOSS_DATA_00_target_x256 = (src->center.x256);
			/*	h->center.y256 =*/ h->BOSS_DATA_01_target_y256 = (src->center.y256);
			//
		//		h->v x256 = (0);/*�E����*/
		//		h->v y256 = (0);/*������*/
			}
			h->radius				= (0);
//			h->recursive			= ( ((3==difficulty)?(1):(0)));/*(Lunatic==�I�����A���l�`)*/
		}
	}
}
#endif
