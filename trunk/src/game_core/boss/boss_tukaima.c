
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�g�����n�́A�K�i�����ʉ�����K�v������B
	(r39u1)
	ToDo:
	�g�����V�X�e���̓X�N���v�g�ォ��A�Ăяo���\�Ƃ��A
	�X�N���v�g�O�̒��{�X������R���g���[���\�ɂ���K�v������B
	-------------------------------------------------------
	���� �I�v�V����
	�P�� �I�v�V����
	-------------------------------------------------------
	��� �΂�T��1�e �I�v�V����
	-------------------------------------------------------
	�΂�T�� ���x�x��
	����	 ����
    -------------------------------------------------------
    �߂�1:������(5)�e
        �Ēe                  �ΕĒe
        �E���                  �����
        (+90�x)                 (-45�x)

            �ΕĒe          �Ēe
            �����          �E���
            (+45�x)         (0�x)
    -------------------------------------------------------
    �߂�2:�����N�i�C(6)�e   [���̕ǂŔ��˂���]
        �ԃN�i�C�e              �N�i�C�e
        �E���                  �����

            �N�i�C�e      �ԃN�i�C�e
            �����          �E���
    -------------------------------------------------------
    ��� �΂�T��2�e �I�v�V����
    -------------------------------------------------------
    �΂�T�� ���x����
    ����     �Z��
    -------------------------------------------------------
    �߂�1:������(5)�e
        �Ēe                  �ΕĒe
        �E���                  �����
        (+90�x)                 (-45�x)

            �ΕĒe          �Ēe
            �����          �E���
            (+45�x)         (0�x)
    -------------------------------------------------------
    �߂�2:�����N�i�C(6)�e   [���̕ǂŔ��˂���]
        �ԃN�i�C�e              �N�i�C�e
        �E���                  �����

            �N�i�C�e      �ԃN�i�C�e
            �����          �E���
    -------------------------------------------------------
    �{�X���ʋK�i�g�p�f�[�^�[:
        BOSS_DATA_00_target_x256    �ڕWx���W x�ʒu�Ƃ��Ďg�p�B
        BOSS_DATA_01_target_y256    �ڕWy���W y�ʒu�Ƃ��Ďg�p�B
        BOSS_DATA_05_move_jyumyou   user_data05
---------------------------------------------------------*/



/*---------------------------------------------------------
	�g�����G����
---------------------------------------------------------*/

static/*global*/ OBJ_CALL_FUNC(lose_tukaima_00)
{
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}

/*---------------------------------------------------------
	�g�����G����
---------------------------------------------------------*/
#if 0
static OBJ_CALL_FUNC(lose_mima_doll)
{
//	item_create_for_boss(src, ITEM_CREATE_MODE_02);/* easy�̓{�����o���Ȃ��čςވʓ�炩������ */
	#if 1
	item_create(src, SP_ITEM_05_TENSU, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
	#else
	{	int i;
		for (i=0; i<(5); i++)
		{
			item_create_random_table(src);
		}
	}
	#endif
}
#endif


// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()

// ����: �P��:
#define rotate_angle1024			user_data03 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW1024�`��) */
#define shot_angle65536 			user_data04 	/*(�g��������) �V���b�g����������ێ� */
#define now_hankei_256				user_data06 	/* ���ݔ��at256()�`��[pixel] */
#define set_hankei_256				user_data07 	/* �w�蔼�at256()�`��[pixel] */

// ���: �΂�T��1�e / �΂�T��2�e

enum
{
	AO_AKA_MASK 			= 0x01,
	MODE_TYPE_00_BARAMAKI01 = 0x00,
	MODE_TYPE_01_BARAMAKI02 = 0x02,
	MODE_TYPE_MASK			= 0x02
};
	#define AO_OR_AKA	(src->set_hankei_256&AO_AKA_MASK)/* [�ԏ��] */
//
/*---------------------------------------------------------
	�q�������w �J�[�h
---------------------------------------------------------*/
static OBJ_CALL_FUNC(tama_uti_sakuya_tukaima)
{
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
		/* ������ 6 way�ł͂Ȃ��āA 3 way ��2��ǉ�����B�łȂ��ƃv���C�I���e�B�[���ςɂȂ�B */
	//	HATSUDAN_06_n_way					= (2+difficulty);						/* [2-5way](r32) */
	//	HATSUDAN_06_n_way					= (3);		/* [�j��] */				/* [5way] */ /* [3way x 2] */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/160); 					/* �����e(r32)6.4 */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/256); 					/* �����e(�����Ɩ���)4 */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/512); 					/* �����e(�����Ɩ���)2 */
// MODE_TYPE_00_BARAMAKI01
//		HATSUDAN_01_speed256				= (t256(1.5))+((((REG_0f_GAME_DIFFICULTY))<<6));		/* �e��(r32/r33) */
		HATSUDAN_01_speed256				= (t256(0.75))+((((REG_0f_GAME_DIFFICULTY))<<5));		/* �e��(r35) */
// MODE_TYPE_01_BARAMAKI02
	//	HATSUDAN_01_speed256				= (t256(1.5))+(((difficulty)<<6));		/* �e��(r32) */
//		HATSUDAN_01_speed256				= (t256(2.0))+((((REG_0f_GAME_DIFFICULTY))<<6));		/* �e��(r33) */
	//	HATSUDAN_01_speed256				= (t256(1.0))+((((REG_0f_GAME_DIFFICULTY))<<5));		/* �e��(r35) */
//
	if (MODE_TYPE_00_BARAMAKI01==((src->set_hankei_256)&MODE_TYPE_MASK))
	{
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI) + (AO_OR_AKA) + (AO_OR_AKA); 	/* [�ΕĒe] */
		HATSUDAN_06_n_way					= (5);		/* [�j��] */				/* [5way] */
	}
	else
	{
		HATSUDAN_01_speed256 += (t256(0.25));
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA) + (AO_OR_AKA) + (AO_OR_AKA);	/* [�ԃN�i�C�e] */
		HATSUDAN_06_n_way					= (6);		/* [�j��] */				/* [3way x 2] */
	}
	HATSUDAN_02_speed_offset				= (0);/*(�e�X�g)*/
	HATSUDAN_03_angle65536					= (src->shot_angle65536);			/* [�j��] */
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	{
		const u16 kakusan_tbl[4] =
		{(int)(65536/512), (int)(65536/512), (int)(65536/256), (int)(65536/128)};		// Lunatic �͂��Ȃ�g�U����B
		HATSUDAN_07_div_angle65536			= kakusan_tbl[(REG_0f_GAME_DIFFICULTY)];		/* [�j��] */		/* �����e(�����Ɩ���)2 */
	}
	hatudan_system_regist_n_way();		/* (r33-) */
}


//		HATSUDAN_01_speed256				= (t256(1.5))+(((difficulty)<<6));	/* �e�� */
//		HATSUDAN_03_angle65536				= ((src->shot_angle65536) );			/* */
//		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
//		HATSUDAN_06_n_way					= (2+difficulty);					/* [2-5way] */
//		HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* �����e */
//		bullet_regist_multi_vector_direct();

static OBJ_CALL_FUNC(tama_uti_mima_tukaima)
{
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
//
//	/*[r35�v������]*/HATSUDAN_01_speed256				= t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6);		/* �e�� */
	/*[r35������]*/HATSUDAN_01_speed256 				= t256(0.75)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* �e�� */
	HATSUDAN_02_speed_offset			= (0);/*(�e�X�g)*/
	HATSUDAN_03_angle65536				= (src->shot_angle65536);		/* [�j��] */
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [�ԃN�i�C�e] */
	HATSUDAN_06_n_way					= (2+(REG_0f_GAME_DIFFICULTY)); 	/* [�j��] */	/* [2-5way] */
	HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* [�j��] */	/* �����e */
//	hatudan_system_regist_katayori_n_way();/* (r33-) */
	hatudan_system_regist_n_way();		/* (r33-) */
}

static OBJ_CALL_FUNC(tama_uti_alice_tukaima)
{
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
//
//	/*[r35�v������]*/HATSUDAN_01_speed256				= t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6);		/* �e�� */
//	/*[r35������]*/HATSUDAN_01_speed256 				= t256(0.75)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* �e�� */
	/*[r35������]*/HATSUDAN_01_speed256 				= t256(0.75);	/* �e�� */
	HATSUDAN_02_speed_offset			= (0);/*(�e�X�g)*/
//	HATSUDAN_03_angle65536				= (src->shot_angle65536);		/* [�j��] */
	HATSUDAN_03_angle65536				= (src->shot_angle65536);		/* [�j��] */
	//
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	HATSUDAN_05_bullet_obj_type 			= (BULLET_MINI8_BASE + TAMA_IRO_03_AOI);	/* �e�O�� */
	hatudan_system_regist_single(); 	/* (r33-) */
	//
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	HATSUDAN_05_bullet_obj_type 			= (BULLET_MINI8_BASE + TAMA_IRO_03_AOI);	/* �e�O�� */
	hatudan_system_regist_single(); 	/* (r33-) */
}

extern OBJ_CALL_FUNC(shot_common_gin_tama);
static OBJ_CALL_FUNC(tama_uti_kaguya_tukaima)
{
/*
0:easy	   f  16���1�񔭒e�B
1:normal   7   8���1�񔭒e�B
2:hard	   3   4���1�񔭒e�B
3:lunatic  1   2���1�񔭒e�B
*/
	if (0==(((1<<(4-REG_0f_GAME_DIFFICULTY))-1) & ((src->jyumyou>>3))))
//	if ((REG_0f_GAME_DIFFICULTY+1) > ((src->jyumyou>>3)&0x03))
	{
		set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
		REG_0e_REG6 = (src->shot_angle65536);/*(1024to65536)*/
		shot_common_gin_tama(src);/*(���ʕ���)*/
	}
}


/*---------------------------------------------------------
	�g�����G �U���̂�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(spell_T01_NULL)
{
	/*(�U�����Ȃ�)*/
}
static OBJ_CALL_FUNC(spell_T02_kaguya_doll)
{
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			src->shot_angle65536 -= ((int)(65536/64));		/* cv1024r(10)*/
			tama_uti_kaguya_tukaima(src);
		}
	}
}

//	if ((unsigned int)(0x01ff/3) < src->BOSS_DATA_05_move_jyumyou)/* �Ō�� 1/3 ���Ԃ͌����ė��Ȃ��B */
static OBJ_CALL_FUNC(spell_T04_mima_doll)
{
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			src->shot_angle65536 += (65536/18); /* �V���b�g�����������A��]������B */
			tama_uti_mima_tukaima(src);
		}
	}
}

static OBJ_CALL_FUNC(spell_T05_sakuya_doll)
{
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			src->shot_angle65536 += ((AO_OR_AKA)?(-(65536/18)):((65536/18)));	/* �V���b�g�����������A��]������B */
			tama_uti_sakuya_tukaima(src);
		}
	}

}
static OBJ_CALL_FUNC(spell_T07_alice_doll)
{
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
		//	src->shot_angle65536 -= ((int)(65536/64));		/* cv1024r(10)*/
//			src->shot_angle65536 += ((int)((2)*64));/*64==(65536/1024)*/		/* cv1024r(10)*/
			src->shot_angle65536 = ((src->rotate_angle1024+128)<<6);/*(�O���V���b�g45�x�X����)*/	/*64==(65536/1024)*/
			tama_uti_alice_tukaima(src);
		}
	}
}




/*---------------------------------------------------------
	�g�����G �ړ��̂�
---------------------------------------------------------*/
static /*global*/ OBJ_CALL_FUNC(move_tukaima00)
{
	//------------------
	HATSUDAN_01_speed256	= (((src->now_hankei_256)));/*(t256�`��)*/
	HATSUDAN_03_angle65536	= deg1024to65536((src->rotate_angle1024));
	sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
	src->center.x256 = (src->BOSS_DATA_00_target_x256) + REG_03_DEST_Y;//sin_value_t256 // �c
	src->center.y256 = (src->BOSS_DATA_01_target_y256) + REG_02_DEST_X;//cos_value_t256 // ��
	//------------------
	//define now_hankei_256 t256(45)			/* ���a */
	if ((src->set_hankei_256) > (src->now_hankei_256)) {	src->now_hankei_256 += t256(1); };
}

/*---------------------------------------------------------
	�I�v�V�����ʒu�A��]�ړ� ���ʕ���
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_option_rotate)
{
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* �I�v�V�����ʒu�A�ړ� */
	move_tukaima00(src);
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_T05_sakuya_doll)
{
	check_tukaima_kougeki_time_out(src);/*(�U������/���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B)*/
	#if 1
	/* �����w��]�A�j���[�V���� */
//	src->rotationCCW1024--;/* �E��� */
	src->rotationCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->rotationCCW1024);
	#endif
	set_timeout_alpha(src);
	/* �I�v�V�����ʒu�A��]�ړ� */
	/* �I�v�V�����ʒu�A�ړ� */
	move_tukaima00(src);
}

/*---------------------------------------------------------
	�q�������w �ړ� ���ʕ���
---------------------------------------------------------*/



/*---------------------------------------------------------
	�� �G�ړ�
	�q�������w01 �ړ�
	�q�������w �ړ�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_T01_kaguya_doll)
{
	check_tukaima_kougeki_time_out(src);/*(�U������/���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B)*/
	set_timeout_alpha(src);
	move_option_rotate(src);
}


/*---------------------------------------------------------
	�q�������w02 �ړ�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_T03_kaguya_doll)
{
	check_tukaima_kougeki_time_out(src);/*(�U������/���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B)*/
	set_timeout_alpha(src);
	/* �I�v�V�����ʒu�A��]�ړ� */
	/* �I�v�V�����ʒu�A�ړ� */
	if (0x2ff == src->BOSS_DATA_05_move_jyumyou)
	{
		AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;
	//	AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;
		AUDIO_19_voice_truck	= TRACK02_JIKI_BOMBER;/*(���p����)*/	cpu_voice_play();
		AUDIO_19_voice_truck	= TRACK03_IVENT_DAN;/*(���p����)*/		cpu_voice_play();
	}
	if (0x2ff > src->BOSS_DATA_05_move_jyumyou)
	{
		move_tukaima00(src);
	}
}


/*---------------------------------------------------------
	�q�������w07 �ړ�
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_T07_alice_doll)
{
	check_tukaima_kougeki_time_out(src);/*(�U������/���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B)*/
	set_timeout_alpha(src);
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (3-(REG_0f_GAME_DIFFICULTY));/*(���Z���u�W��+2�ɉ�����v)*/
	/*(����[+3]�Ȃ�A��]�����������Ԋu���L���葽���ȒP�B)*/
	move_option_rotate(src);
	src->set_hankei_256--;
}


/*---------------------------------------------------------
	�q�������w01/02 �o�^���ʕ���
---------------------------------------------------------*/

static OBJ_CALL_FUNC(set_score_common)
{
	static const unsigned int difficulty_score_tbl[4] = // �X�R�A�e�[�u��
	{
	//	score( 100000), 	/* easy 10�� (�v 50��==5x 10��) (r32) */
	//	score( 300000), 	/* norm 30�� (�v150��==5x 30��) (r32) */
	//	score( 500000), 	/* hard 50�� (�v250��==5x 50��) (r32) */
	//	score(1000000), 	/* luna100�� (�v500��==5x100��) (r32) */
		score(	10000), 	/* easy 1�� (�v 5��==5x 1��) (r33) */
		score(	30000), 	/* norm 3�� (�v15��==5x 3��) (r33) */
		score(	50000), 	/* hard 5�� (�v25��==5x 5��) (r33) */
		score( 100000), 	/* luna10�� (�v50��==5x10��) (r33) */
	};
	src->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
//	src->base_score			= adjust_score_by_difficulty(score( 50000));	/* 5�� (�v30��==6x5��) (r33) */
//	src->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) (r32) */
//	src->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
}

/*---------------------------------------------------------
	�q�������w01/02 �o�^���ʕ���
---------------------------------------------------------*/
//	#define ADD_ANGLE		(341)	/* 1���� 3���������p�x�A341.33 == 1024/3  */
//	#define ADD_ANGLE		(171)	/* 1���� 6���������p�x�A170.66 == 1024/6  */
//	#define ADD_ANGLE205	(205)	/* 1���� 5���������p�x�A204.80 == 1024/5  */
//	#define ADD_ANGLE		(51)	/* 1����20���������p�x�A 51.20 == 1024/20 */
//	const int add_angle = (ADD_ANGLE205);	/* ���Z�p�x */
	//	const int add_angle = (ADD_ANGLE);	/* ���Z�p�x */
//	#define ADD_ANGLE043 (43)		/* 1����24���������p�x�A 42.66 == 1024/24 */	/* 1�� �� 24 ���� */

typedef struct
{
	u32 loop_start; 	/*(�J�n�p�x)*/
	u32 loop_stop;		/*(�I���p�x)*/
	u32 loop_step;		/*(�����p�x)*/
	u32 add_pattern256;	/*(�O�����Z��)*/
	//
	int limit_hankei_256;	/*(�ő唼�a)*/
	int limit_move_jyumyou;
	void (*move_callback)(OBJ *sss);
	void (*kougeki_callback)(OBJ *sss);
} tukaima_status_BBB;
static tukaima_status_BBB tukaima_status[(16)/*(8)*/] =
{
/*T00*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T01*/ {0, 						(1024), 						(1024/5),	(0x100), t256(45),		(0x01ff),			move_T01_kaguya_doll,			spell_T01_NULL, 			},	/* �w�蔼�a 45[pixel] */	/* �������� */
/*T02*/ {0, 						(1024), 						(1024/5),	(0x000), t256(45+48),	(0x02ff),			move_T01_kaguya_doll,			spell_T02_kaguya_doll,		}, /* (0x01ff)�������� */ /* ��� */
/*T03*/ {((1024/2)-(1024*1/6)), 	((1024/2)+(1024*1/6)),			(1024/24),	(0x000), t256(32*7),	(0x02ff)-(1024/3),	move_T03_kaguya_doll,			spell_T02_kaguya_doll,		}, /* �w�蔼�a 224[pixel] 256[pixel] */ /* �z�u�͈�(��𒆐S�Ƃ���1/3��) */
/*T04*/ {0, 						(1024), 						(1024/5),	(0x000), t256(45),		(0x02ff),			move_T01_kaguya_doll,			spell_T04_mima_doll,		}, /* �w�蔼�a 45[pixel] */ /* �������� / �\�z�ȏ�ɑ���������ʂɁB(r33) (0x01ff) */ /* ��� */
/*T05*/ {((1024*3/4))+0,			((1024*3/4)+((1024/2)+1)),		(1024/6),	(0x000), t256(64),		(0x04ff),			move_T05_sakuya_doll,			spell_T05_sakuya_doll,		}, /* 4�� */ /* ��������(����) (0x03ff) */
/*T06*/ {((1024*3/4))+0,			((1024*3/4)+((1024/2)+1)),		(1024/6),	(0x000), t256(64),		(0x04ff),			move_T05_sakuya_doll,			spell_T05_sakuya_doll,		}, /* 4�� */ /* ��������(����) (0x03ff) */
/*T07*/ {0, 						(1024), 						(1024/3),	(0x000), t256(32),		(0x08ff),			move_T07_alice_doll,			spell_T07_alice_doll,		}, /* (0x01ff)�������� */ /* ��� */
//
/*T08*/ {0, 						(1024), 						(1024/24),	(0x055), 0, 0, 0, 0,},/*(0x55==((1024*256)/(24*128)))*/
/*T09*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T0a*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T0b*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T0c*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T0d*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T0e*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T0f*/ {0, 0, 0, (0), 0, 0, 0, 0,},
};


		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
//			h->base_hp				= ((2*1024)-1); 	/* 8192==(8*1024) (r36)���炩�� */
//			h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40�� */

//	const int add_angle = (ADD_ANGLE205);	/* ���Z�p�x */


/*---------------------------------------------------------
	(�g�����V�X�e��)�G��ǉ�����B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(tukaima_system_add_dolls)
{
	card.tukaima_used_number &= 0x07;
	//
	int jjj;
	jjj=0;
	int i_type256;
	i_type256 = 0x000;/*(�O�������p�^�[��)*/
	u32 i_angle1024;	/* �ώZ�p�x */
	for (
		i_angle1024=tukaima_status[card.tukaima_used_number].loop_start;
		i_angle1024 < tukaima_status[card.tukaima_used_number].loop_stop;
		i_angle1024 += tukaima_status[card.tukaima_used_number].loop_step
		) /* ��� */
	{
		OBJ *h;
		h							= obj_regist_teki();
		/*(�g���Ԃ�[A02]�ėp�̈�ł̓}�Y�C�B[A01]�Œ�̈�ɕύX����K�v������B)*/
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->rotate_angle1024 	= ((i_angle1024)&(1024-1));
			// ��ʒu(�P��)
			h->BOSS_DATA_00_target_x256 = (src->center.x256);
			h->BOSS_DATA_01_target_y256 = (src->center.y256);
			//
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->obj_type_set 		= BOSS_20_YOUSEI1_SYOUMEN;
			#if (1)/*(�ǉ�����)*/
			h->obj_type_set 		+= (((i_type256)>>8)&0x07);/*(�O���͍ő�8�p�^�[��)*/
			i_type256 += (tukaima_status[card.tukaima_used_number].add_pattern256);
			#endif
			h->atari_hantei 		= (1/*�X�R�A���p*/);
			//
			h->callback_loser		= lose_tukaima_00;//lose_mima_doll;
			h->callback_hit_teki	= callback_hit_zako;
			//
			h->now_hankei_256		= t256(0);			/* ���ݔ��a  0[pixel] */
			h->shot_angle65536		= (0);
		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
		//	h->base_hp				= ((2*1024)-1); 	/* 8192==(8*1024) (r36)���炩�� */
		//	h->base_hp				= ((3*1024)-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
		//	h->base_hp				= (((difficulty+1)<<10)-1); 	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			/* ��x�オ��Ɣ����ɍd���Ȃ�(==�|���̂ɂ��ꂾ�����Ԃ�������) */
		//	h->base_hp				= (((1024)+((REG_0f_GAME_DIFFICULTY)<<8))-1);	/* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			h->base_hp				= (((3*1024)+((REG_0f_GAME_DIFFICULTY)<<8))-1); /* 8192==(8*1024) */	/* �d�����Ǔ|���� */
			//
			set_score_common(h);
			{
				h->callback_mover					= tukaima_status[card.tukaima_used_number].move_callback;
				h->callback_kougeki 				= tukaima_status[card.tukaima_used_number].kougeki_callback;
				h->set_hankei_256					= tukaima_status[card.tukaima_used_number].limit_hankei_256;
				h->BOSS_DATA_05_move_jyumyou		= tukaima_status[card.tukaima_used_number].limit_move_jyumyou;
			}
			const void *aaa[(8)] =
			{
				&&label_continue_T00,
				&&label_continue_T00,/*(���ʂ̂�)�� �G��ǉ����� */
				&&label_create_dolls_kaguya_T02,
				&&label_create_dolls_kaguya_T03,
				//
				&&label_continue_T00,/*(���ʂ̂�)*/
				&&label_create_dolls_sakuya_T05_T06,
				&&label_create_dolls_sakuya_T05_T06,
				&&label_continue_T00,/*(alice)*/
			};
			goto *aaa[card.tukaima_used_number];
label_create_dolls_kaguya_T03:			/* �q�������w02 �G��ǉ����� */
			{
				/* ��� 1�� �� 1/3 ���� */
				// ��ʒu(��ʉ������S)
				h->BOSS_DATA_00_target_x256 		= (t256((GAME_WIDTH/2)));//;(src->center.x256);
				h->BOSS_DATA_01_target_y256 		= (t256(272)		   );//;(src->center.y256);
				//
			//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 		/* �������� */
			//	h->BOSS_DATA_05_move_jyumyou		= (0x02ff)+(i<<6);	/* �������� */
			//	h->BOSS_DATA_05_move_jyumyou		= (0x02ff)-(1024/3)+(i_angle1024);	/* �������� */
				h->BOSS_DATA_05_move_jyumyou		+= (i_angle1024);	/* �������� */
			}
			goto label_continue_T00;
label_create_dolls_kaguya_T02:/* �q�������w01 �G��ǉ����� */
			{
				h->set_hankei_256					-= (REG_0f_GAME_DIFFICULTY<<(4+8)); 	/* �w�蔼�a 45[pixel] */
			}
			goto label_continue_T00;
label_create_dolls_sakuya_T05_T06:
			{
				#if (1)/*(�ǉ�����)*/
				h->base_hp				= (8192-1);/*(�d��)*/
				#endif
				h->set_hankei_256	 += (card.tukaima_used_number&2/*mode*/) + (jjj);/* [�ԏ��]���C���^�[���[�u */
			}
		//	goto label_continue_T00;
label_continue_T00:// �Ȃɂ�����
				;jjj^=1;
		}
	}
}

/*---------------------------------------------------------
	(�g�����V�X�e��)�ǉ��G��ݒ肷��B
---------------------------------------------------------*/

global OBJ_CALL_FUNC(init_set_dolls_kaguya_T01) 	{	card.tukaima_used_number = TUKAIMA_01_kaguya_T01;}
global OBJ_CALL_FUNC(init_set_dolls_kaguya_T02) 	{	card.tukaima_used_number = TUKAIMA_02_kaguya_T02;}
global OBJ_CALL_FUNC(init_set_dolls_kaguya_T03) 	{	card.tukaima_used_number = TUKAIMA_03_kaguya_T03;}
global OBJ_CALL_FUNC(init_set_dolls_mima_T04)		{	card.tukaima_used_number = TUKAIMA_04_mima_T04;}
global OBJ_CALL_FUNC(init_set_dolls_sakuya_T05) 	{	card.tukaima_used_number = TUKAIMA_05_sakuya_T05;}
global OBJ_CALL_FUNC(init_set_dolls_sakuya_T06) 	{	card.tukaima_used_number = TUKAIMA_06_sakuya_T06;}
global OBJ_CALL_FUNC(init_set_dolls_alice_T07)		{	card.tukaima_used_number = TUKAIMA_07_alice_T07;}
//obal OBJ_CALL_FUNC(init_set_dolls_pache_T07)		{	card.tukaima_used_number = TUKAIMA_07_pache_T07;}
