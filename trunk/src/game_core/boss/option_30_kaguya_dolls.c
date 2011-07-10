
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�P�� �I�v�V����
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
		BOSS_DATA_00_target_x256	�ڕWx���W x�ʒu�Ƃ��Ďg�p�B
		BOSS_DATA_01_target_y256	�ڕWy���W y�ʒu�Ƃ��Ďg�p�B
		BOSS_DATA_05_move_jyumyou	user_data05
---------------------------------------------------------*/

// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()
#define rotate_angle1024			user_data03 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW1024�`��) */
#define shot_angle65536 			user_data06 	/* */
#define now_hankei_dot				user_data07 	/* ���a */



//		src->shot_angle65536 += (65536/18); /* �V���b�g�����������A��]������B */
//	//
//		HATSUDAN_01_speed256				= (t256(1.5))+(((difficulty)<<6));	/* �e�� */
//		HATSUDAN_03_angle65536				= ((src->shot_angle65536) );			/* */
//		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
//		HATSUDAN_06_n_way					= (2+difficulty);					/* [2-5way] */
//		HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* �����e */
//		bullet_regist_multi_vector_direct();

/*---------------------------------------------------------
	�q�������w �J�[�h
---------------------------------------------------------*/
extern void shot_common_gin_tama(SPRITE *src);
static void shot_kaguya_doll(SPRITE *src)
{
/*
0:easy	   f  16���1�񔭒e�B
1:normal   7   8���1�񔭒e�B
2:hard	   3   4���1�񔭒e�B
3:lunatic  1   2���1�񔭒e�B
*/
	if ( 0==(((1<<(4-REG_0f_GAME_DIFFICULTY))-1) & ((src->jyumyou>>3))) )
//	if ( (REG_0f_GAME_DIFFICULTY+1) > ((src->jyumyou>>3)&0x03) )
	{
		set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
		REG_0e_REG6 = ((src->shot_angle65536) );/*(1024to65536)*/
		shot_common_gin_tama(src);/*(���ʕ���)*/
	}
}

/*---------------------------------------------------------
	�q�������w �ړ� ���ʕ���
---------------------------------------------------------*/

static void move_kaguya_doll_common(SPRITE *src)
{
	check_tukaima_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			src->shot_angle65536 -= (16*64);		/* cv1024r(10)*/
			shot_kaguya_doll(src);
		}
	}
	set_timeout_alpha(src);
}


/*---------------------------------------------------------
	�q�������w01 �ړ�
---------------------------------------------------------*/
static void move_kaguya_doll01(SPRITE *src)
{
	move_kaguya_doll_common(src);
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* �I�v�V�����ʒu�A�ړ� */
	move_tukaima00(src);
	//	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	//	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
}


/*---------------------------------------------------------
	�q�������w02 �ړ�
---------------------------------------------------------*/

static void move_kaguya_doll02(SPRITE *src)
{
	move_kaguya_doll_common(src);
	/* �I�v�V�����ʒu�A��]�ړ� */
	/* �I�v�V�����ʒu�A�ړ� */
	if (0x2ff == src->BOSS_DATA_05_move_jyumyou)
	{
		voice_play(VOICE11_BOSS_KIRARIN, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
		voice_play(VOICE11_BOSS_KIRARIN, TRACK03_SHORT_MUSIC);/*�e�L�g�[*/
	}
	if (0x2ff > src->BOSS_DATA_05_move_jyumyou)
	{
		move_tukaima00(src);
	//	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	//	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
	}
}

/*---------------------------------------------------------
	�� �G�ړ�
---------------------------------------------------------*/

static void move_kaguya_houmotu_doll(SPRITE *src)
{
	check_tukaima_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
	//
	set_timeout_alpha(src);
	//
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* �I�v�V�����ʒu�A�ړ� */
	move_tukaima00(src);
	//	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	//	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
}

/*---------------------------------------------------------
	�q�������w01/02 �o�^���ʕ���
---------------------------------------------------------*/

static void regist_kaguya_dolls_common(SPRITE *h)
{
	h->m_Hit256R			= ZAKO_ATARI16_PNG;
	h->type 				= BOSS_20_YOUSEI1_SYOUMEN;
	h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
	h->callback_loser		= lose_tukaima_00;
	h->callback_hit_teki	= callback_hit_zako;
	//
	h->shot_angle65536		= (0);
	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
	{
		static const unsigned int difficulty_score_tbl[4] = // �X�R�A�e�[�u��
		{
			score( 100000), 	/* easy 10�� (�v 50��==5x 10��) */
			score( 300000), 	/* norm 30�� (�v150��==5x 30��) */
			score( 500000), 	/* hard 50�� (�v250��==5x 50��) */
			score(1000000), 	/* luna100�� (�v500��==5x100��) */
		};
		h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
//		h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
	}
}

/*---------------------------------------------------------
	�q�������w02 �G��ǉ�����
---------------------------------------------------------*/

void add_zako_kaguya_dolls02(SPRITE *src)
{
	#define ADD_ANGLE (43)		/* 1����24���������p�x�A 42.66 == 1024/24 */	/* 1�� �� 24 ���� */
//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
//
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=((1024/2)-(1024*1/6)); i_angle1024<((1024/2)+(1024*1/6)); i_angle1024 += (ADD_ANGLE)) /* �z�u�͈�(��𒆐S�Ƃ���1/3��) */
	/* ��� 1�� �� 1/3 ���� */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->rotate_angle1024 	= i_angle1024;
			regist_kaguya_dolls_common(h);
			h->callback_mover		= move_kaguya_doll02;
			// ��ʒu(��ʉ������S)
			h->BOSS_DATA_00_target_x256 		= (t256((GAME_WIDTH/2)));//;(src->cx256);
			h->BOSS_DATA_01_target_y256 		= (t256(272)		   );//;(src->cy256);
			//
			h->now_hankei_dot		= (32*7);			/* �������a 224[dot] 256[dot] */
		//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 		/* �������� */
		//	h->BOSS_DATA_05_move_jyumyou		= (0x02ff)+(i<<6);	/* �������� */
			h->BOSS_DATA_05_move_jyumyou		= (0x02ff)-(1024/3)+(i_angle1024);	/* �������� */
		}
	}
}
/*---------------------------------------------------------
	�q�������w01 �G��ǉ�����
---------------------------------------------------------*/

void add_zako_kaguya_dolls01(SPRITE *src)
{
//	#define ADD_ANGLE (341) 	/* 1���� 3���������p�x�A341.33 == 1024/3  */
//	#define ADD_ANGLE (171) 	/* 1���� 6���������p�x�A170.66 == 1024/6  */
	#define ADD_ANGLE (205) 	/* 1���� 5���������p�x�A204.80 == 1024/5  */
//	#define ADD_ANGLE (51)		/* 1����20���������p�x�A 51.20 == 1024/20 */
//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* ��� */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->rotate_angle1024 	= i_angle1024;
			regist_kaguya_dolls_common(h);
			h->callback_mover		= move_kaguya_doll01;
			// ��ʒu(�P��)
			h->BOSS_DATA_00_target_x256 		= (src->cx256);
			h->BOSS_DATA_01_target_y256 		= (src->cy256);
			//
		//	h->now_hankei_dot		= (45); 		/* �������a 45[dot] */
			h->now_hankei_dot		= (45) +  48-(REG_0f_GAME_DIFFICULTY<<4);		/* �������a 45[dot] */
		//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 	/* �������� */
			h->BOSS_DATA_05_move_jyumyou		= (0x02ff); 	/* �������� */
		}
	}
}

/*---------------------------------------------------------
	�� �G��ǉ�����
---------------------------------------------------------*/

void add_zako_kaguya_houmotsu(SPRITE *src)
{
	int i_type;
	i_type=0;
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* ��� */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->rotate_angle1024 	= i_angle1024;
//			regist_kaguya_dolls_common(h);
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_20_YOUSEI1_SYOUMEN + (i_type);	i_type++;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//
			h->callback_loser		= lose_tukaima_00;
			h->callback_hit_teki	= callback_hit_zako;
			//
			h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
			h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40�� */
			//
			h->callback_mover		= move_kaguya_houmotu_doll;
			// ��ʒu(�P��)
			h->BOSS_DATA_00_target_x256 		= (src->cx256);
			h->BOSS_DATA_01_target_y256 		= (src->cy256);
			//
			h->now_hankei_dot		= (45); 			/* �������a 45[dot] */
			h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 		/* �������� */
		}
	}
}
