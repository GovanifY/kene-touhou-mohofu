
#include "boss.h"//#include "game_main.h"

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
#define recursive		user_data06 	/* �����(0>���􂵂Ȃ�) */
#define radius			user_data07 	/* �~�̔��a */


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
	#if 0
	HATSUDAN_01_speed256			= (t256(2.0));						/* �e�� */
	HATSUDAN_03_angle65536			= ((src->rotationCCW1024)<<6);			/* �p�x */
	HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI) + ((REG_0f_GAME_DIFFICULTY)); 	/* [�ؒe] */
	HATSUDAN_06_n_way				= (7);								/* [7way] [8way] */
	HATSUDAN_07_div_angle65536		= (int)(65536/23);					/* �����p�x (65536/27) (65536/24) */
	bul let_reg ist_multi_vec tor();
	#endif

static OBJ_CALL_FUNC(move_alice_doll_last_burrets)
{
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	#if 1
	const u8 tama_color[4] =
	{
		TAMA_IRO_03_AOI,
		TAMA_IRO_01_AKA,
		TAMA_IRO_02_YUKARI,
		TAMA_IRO_05_MIDORI
	};
	HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	#if (1)
	HATSUDAN_05_bullet_obj_type 	= BULLET_UROKO14_BASE + (tama_color[((REG_0f_GAME_DIFFICULTY)&3)]); 		/* [�ؒe] */
	#else
/*????????????*/
	HATSUDAN_05_bullet_obj_type 	= BULLET_UROKO14_BASE + (tama_color[((0)&3)]);			/* [�ؒe] */
	#endif

//	HATSUDAN_06_n_way				= (1);								/* [7way] [8way] */
//	HATSUDAN_07_div_angle65536		= (int)(65536/23);					/* �����p�x (1024/27) (1024/24) */
	int first_angle_65536;
	first_angle_65536 = ((src->rotationCCW1024)<<6);
	unsigned int i;
	for (i=0; i<(7); i++)	/* �e�� */
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
		HATSUDAN_01_speed256				= (ccc[i][CCC_00_SPEED256]);									/* �e�� */
		HATSUDAN_03_angle65536				= (first_angle_65536+ccc[i][CCC_01_ADD_ANGLE65536]);			/* �p�x */
		hatudan_system_regist_single();/* (r33-) */
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
static OBJ_CALL_FUNC(add_zako_alice_doll_2nd_CCW);

static OBJ_CALL_FUNC(move_alice_doll_all)
{
	src->BOSS_DATA_05_move_jyumyou--;/* ���Ԍo�� */
	if (0 > src->BOSS_DATA_05_move_jyumyou)/* �ړ��I�� */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->center.y256)	/* ���̃��C����艺����͓G�������Ȃ� */
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
			#if (0)//
			src->center.x256 = src->BOSS_DATA_00_target_x256 + ((si n1024((bbb_rotationCCW1024))*(src->radius)));/*fps_factor*/	/* CCW�̏ꍇ */
			src->center.y256 = src->BOSS_DATA_01_target_y256 + ((co s1024((bbb_rotationCCW1024))*(src->radius)));/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (bbb_rotationCCW1024), &sin_value_t256, &cos_value_t256);
				src->center.x256 = src->BOSS_DATA_00_target_x256 + ((sin_value_t256*(src->radius)));/*fps_factor*/
				src->center.y256 = src->BOSS_DATA_01_target_y256 + ((cos_value_t256*(src->radius)));/*fps_factor*/
			}
			#endif
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����(2nd)
---------------------------------------------------------*/
static OBJ_CALL_FUNC(add_zako_alice_doll_2nd_CCW)
{
//	const int add_angle = ( (1024/7) ); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
	int first_angle1024;	/* �J�n�p�x */
	first_angle1024 = (src->rotationCCW1024)+(1024/4);
	int i_angle1024;	/* �ώZ�p�x */
//	for (i_angle1024=(0); i_angle1024<(1024); i_angle1024 += (1024/7)/*add_angle*/) 	/* �e�� */
	/* ������7���� */
	for (i_angle1024=(0); i_angle1024<(1024/2); i_angle1024 += (1024/(7*2))/*add_angle*/)	/* �e�� */
	{
		OBJ *h;
		h						= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->obj_type_set 		= TEKI_16_10+((1)<<2)+((src->recursive)<<2);
			h->atari_hantei 		= (1/*�X�R�A���p*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_teki	= callback_hit_zako;
			h->rotationCCW1024		= (first_angle1024+i_angle1024);
	//
	//		h->base_score		= score(25*2);/*�_�~�[*/
			h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->BOSS_DATA_05_move_jyumyou			= (60);/* 1[�b](60[frame])��ɁA�ĕ��� */
			{
				/* �����ʒu */
			/*	h->center.x256 =*/ h->BOSS_DATA_00_target_x256 = (src->center.x256);
			/*	h->center.y256 =*/ h->BOSS_DATA_01_target_y256 = (src->center.y256);
			}
			// [���K�V�[�V�X�e���Ή�]�G�̈悾���疳��(?)
			h->hatudan_register_spec_data	= (0);/*([���C���[�V�X�e���������Ȃ��悤�ɂ���]�悭�킩��Ȃ�)*/
			h->radius				= (0);
			h->recursive			= ( ((src->recursive)-1) );/* �ĕ���񐔂�1���炷�B(�e�����A) */
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

/**/ OBJ_CALL_FUNC(add_zako_alice_doll_type_a)
{
	#if (1)
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[�͕핗]*/ 	/*(�I�����A���l�`)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(�I�����A���l�`)*/ 	/* [���݂̕����ł͖���] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[�d�X����]*/	/*(�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
//		(int)(1024/4), (int)(1024/6), (int)(1024/7), (int)(1024/2),/*[�d�X����]*/	/*(�I�����A���l�`)*/	/* [���݂̕����ł͖���] */
	/*(
	hard �n���O�A�b�v����B�������ǂ�����Ȃ��B
		r36, hard zako�̈�g���s�����āH�H�n���O�A�b�v����̂�8->7�ɂ���
	)*/
		/* �͕핗��r32���݂����蔻��Ӑ}�I�ɏ��������Ă���̂ŁA������1�ʂƂ͂���
			����ϓ�Փx�Ⴗ����C������B(�I�����A���l�`)�ȊO�d�X�����ɂ����B */
	};
//	if (2==(REG_0f_GAME_DIFFICULTY&3)){REG_0f_GAME_DIFFICULTY=1;}
//	if (1 < (REG_0f_GAME_DIFFICULTY/*&3*/))/*(??????�I�������Ȃ��B�����H�H�H)*/
//	if (0 != (REG_0f_GAME_DIFFICULTY/*&3*/))/*(easy �ȊO��)*/
//	{
//		REG_0f_GAME_DIFFICULTY = (1);/*(���� normal �ɂ���B)*/
//	}/*(r36-: hard, luna ���n���O�A�b�v����̂ŋ��� normal �ɂ���B)*/
	//
	const int add_angle1024 = ( (aaa_tbl[((REG_0f_GAME_DIFFICULTY)&3)])); /* ���Z�p�x */	/* �Q��ڈȍ~�̕��񐔂͏��7�� */
	#else
	/*(????????????)*/
	const int add_angle1024 = (int)(1024/6);
	#endif
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* ��� */
	{
		OBJ *h;
		h							= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->obj_type_set 		= TEKI_16_10+((0/*2*/)<<2);
			h->atari_hantei 		= (1/*�X�R�A���p*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->rotationCCW1024		= (i_angle1024);
	//
	//		h->base_score		= score(25*2);/*�_�~�[*/
			h->base_hp			= (9999);/*�|���Ȃ�*/
	//
			h->BOSS_DATA_05_move_jyumyou			= (60);
			{
				/* �����ʒu */
			/*	h->center.x256 =*/ h->BOSS_DATA_00_target_x256 = (src->center.x256);
			/*	h->center.y256 =*/ h->BOSS_DATA_01_target_y256 = (src->center.y256);
			}
			// [���K�V�[�V�X�e���Ή�]�G�̈悾���疳��(?)
//			h->hatudan_register_spec_data			= (0);/*([���C���[�V�X�e���������Ȃ��悤�ɂ���]�悭�킩��Ȃ�)*/
			h->radius				= (0);
			h->recursive			= ( ((3==(REG_0f_GAME_DIFFICULTY))?(1):(0)));/*(Lunatic==�I�����A���l�`)*/
		}
	}
}



#if 00
/*---------------------------------------------------------
	�G��ǉ�����(common)
---------------------------------------------------------*/

static void add_zako_alice_doll_common(OBJ/**/ *src, int is_the_first)
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
		OBJ *h;
		h							= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->obj_type_set 		= (TEKI_16_10)+((src->recursive)<<2);
			h->atari_hantei 		= (1/*�X�R�A���p*/);
		//
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_teki	= callback_hit_zako;
		//
		//	h->base_score			= score(25*2);/*�_�~�[*/
			h->base_hp				= (9999);/*�|���Ȃ�*/
		//
			h->rotationCCW1024		= (i_angle1024);
			h->BOSS_DATA_05_move_jyumyou			= (60);
			{
				/* �����ʒu */
			/*	h->center.x256 =*/ h->BOSS_DATA_00_target_x256 = (src->center.x256);
			/*	h->center.y256 =*/ h->BOSS_DATA_01_target_y256 = (src->center.y256);
			}
			h->user_data02		= (0);/*(�悭�킩��Ȃ�)*/
			h->radius		= (0);
			h->recursive	= ((0==is_the_first)?((src->recursive)-1):((3==difficulty)?(1):(0)));
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(2nd)
---------------------------------------------------------*/
static OBJ_CALL_FUNC(add_zako_alice_doll_2nd_CCW)
{
	add_zako_alice_doll_common(src, 0);
}


/*---------------------------------------------------------
	�G��ǉ�����(1st)
---------------------------------------------------------*/

global OBJ_CALL_FUNC(add_zako_alice_doll)
{
	add_zako_alice_doll_common(src, 1);
}
#endif
