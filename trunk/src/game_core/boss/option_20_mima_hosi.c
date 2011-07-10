
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���� �I�v�V����
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
		BOSS_DATA_00_target_x256	�ڕWx���W x�ʒu�Ƃ��Ďg�p�B
		BOSS_DATA_01_target_y256	�ڕWy���W y�ʒu�Ƃ��Ďg�p�B
		BOSS_DATA_05_move_jyumyou	user_data05
---------------------------------------------------------*/

// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()
#define rotate_angle1024		user_data03 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW1024�`��) */
#define shot_angle65536 		user_data06 	/* */
#define now_hankei_dot			user_data07 	/* ���a */


/*---------------------------------------------------------
	�q�������w �ړ��̂�
---------------------------------------------------------*/
global void move_tukaima00(SPRITE *src)
{
	#if (0)// 1802402
	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
	#else// 1802482 1802450
	{
		int sin_value_t256; //	sin_value_t256 = 0;
		int cos_value_t256; //	cos_value_t256 = 0;
		int256_sincos1024( (src->rotate_angle1024), &sin_value_t256, &cos_value_t256);
		src->cx256 = (src->BOSS_DATA_00_target_x256) + ((sin_value_t256*(src->now_hankei_dot)));	/*CCW*/
		src->cy256 = (src->BOSS_DATA_01_target_y256) + ((cos_value_t256*(src->now_hankei_dot)));
	}
	#endif
}
/*---------------------------------------------------------
	�q�������w �ړ�
---------------------------------------------------------*/

static void move_mima_doll(SPRITE *src)
{
	check_tukaima_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
//	if ( (unsigned int)(0x01ff/3) < src->BOSS_DATA_05_move_jyumyou)/* �Ō�� 1/3 ���Ԃ͌����ė��Ȃ��B */
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			src->shot_angle65536 += (65536/18); /* �V���b�g�����������A��]������B */
		//
			set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
		//
		//	/*[r35�v������]*/HATSUDAN_01_speed256				= t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6);		/* �e�� */
			/*[r35������]*/HATSUDAN_01_speed256 				= t256(0.75)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* �e�� */
			HATSUDAN_02_speed_offset			= (0);/*(�e�X�g)*/
			HATSUDAN_03_angle65536				= ((src->shot_angle65536) );		/* [�j��] */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
			HATSUDAN_06_n_way					= (2+(REG_0f_GAME_DIFFICULTY)); 	/* [�j��] */	/* [2-5way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* [�j��] */	/* �����e */
		//	hatudan_system_regist_katayori_n_way();/* (r33-) */
			hatudan_system_regist_n_way();		/* (r33-) */
		}
	}
	//
	set_timeout_alpha(src);
	//
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* �I�v�V�����ʒu�A�ړ� */
	//define now_hankei_dot (45)			/* ���a */
	if ((45) > (src->now_hankei_dot))	{	src->now_hankei_dot++;	};
	move_tukaima00(src);

	//#undef now_hankei_dot
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_mima_dolls(SPRITE *src)
{
//	#define ADD_ANGLE (171*2)	/* 1����3���������p�x�A170.66*2 == 1024/3 */
//	#define ADD_ANGLE (85*2)	/* 1����6���������p�x�A 85.33*2 == 1024/6 */
	#define ADD_ANGLE (102*2)	/* 1����5���������p�x�A102.40*2 == 1024/5 */
//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
//
	int i_angle1024;	/* �ώZ�p�x */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* ��� */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_20_YOUSEI1_SYOUMEN;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_mima_doll;
			h->callback_loser		= lose_tukaima_00;//lose_mima_doll;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->BOSS_DATA_00_target_x256 		= (src->cx256);
			h->BOSS_DATA_01_target_y256 		= (src->cy256);
	//
		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
		//	h->base_hp				= ((3*1024)-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
		//	h->base_hp				= (((difficulty+1)<<10)-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			/* ��x�オ��Ɣ����ɍd���Ȃ�(==�|���̂ɂ��ꂾ�����Ԃ�������) */
			h->base_hp				= (((1024)+((REG_0f_GAME_DIFFICULTY)<<8))-1);	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score( 100000), 	/* easy 10�� (�v 50��==5x 10��) */
					score( 300000), 	/* norm 30�� (�v150��==5x 30��) */
					score( 500000), 	/* hard 50�� (�v250��==5x 50��) */
					score(1000000), 	/* luna100�� (�v500��==5x100��) */
				};
			h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
		//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
			}
		//
			h->now_hankei_dot		= (0);			/* �������a 0[dot] */
			h->rotate_angle1024 	= i_angle1024;
			h->shot_angle65536		= (0);
		//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 	/* �������� / �\�z�ȏ�ɑ���������ʂɁB(r32) */
			h->BOSS_DATA_05_move_jyumyou		= (0x02ff); 	/* �������� / �\�z�ȏ�ɑ���������ʂɁB(r33) */
		}
	}
}
