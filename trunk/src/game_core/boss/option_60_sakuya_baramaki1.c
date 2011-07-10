
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
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
	�{�X���ʋK�i�g�p�f�[�^�[:
		BOSS_DATA_00_target_x256
		BOSS_DATA_01_target_y256
		BOSS_DATA_04_toutatu_wariai256
		BOSS_DATA_05_move_jyumyou			user_data05
---------------------------------------------------------*/

// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()
#define shot_angle65536 	user_data06 	/* �V���b�g����������ێ� */


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_sakuya_baramaki1(SPRITE *src)
{
	check_tukaima_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	#define AO_OR_AKA	(src->BOSS_DATA_00_target_x256&1)/* [�ԏ��] */
//
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			if (0 < src->BOSS_DATA_04_toutatu_wariai256 )
			{
				src->BOSS_DATA_04_toutatu_wariai256 -= (1);
			}
		//
			src->shot_angle65536 += ((AO_OR_AKA)?(-(65536/18)):((65536/18)));	/* �V���b�g�����������A��]������B */
		//
			set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			/* ������ 6 way�ł͂Ȃ��āA 3 way ��2��ǉ�����B�łȂ��ƃv���C�I���e�B�[���ςɂȂ�B */
		//	HATSUDAN_06_n_way					= (2+difficulty);						/* [2-5way](r32) */
		//	HATSUDAN_06_n_way					= (3);		/* [�j��] */								/* [5way] */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/160); 					/* �����e(r32)6.4 */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/256); 					/* �����e(�����Ɩ���)4 */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/512); 					/* �����e(�����Ɩ���)2 */
	//
//			HATSUDAN_01_speed256				= (t256(1.5))+((((REG_0f_GAME_DIFFICULTY))<<6));		/* �e��(r32/r33) */
			HATSUDAN_01_speed256				= (t256(0.75))+((((REG_0f_GAME_DIFFICULTY))<<5));		/* �e��(r35) */
			HATSUDAN_02_speed_offset			= (0);/*(�e�X�g)*/
			HATSUDAN_03_angle65536				= (src->shot_angle65536);			/* [�j��] */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI) + (AO_OR_AKA)+(AO_OR_AKA);	/* [�ΕĒe] */
			HATSUDAN_06_n_way					= (5);		/* [�j��] */								/* [5way] */
			{
				const u16 kakusan_tbl[4] =
				{(int)(65536/512), (int)(65536/512), (int)(65536/256), (int)(65536/128)};		// Lunatic �͂��Ȃ�g�U����B
				HATSUDAN_07_div_angle65536		= kakusan_tbl[(REG_0f_GAME_DIFFICULTY)];		/* [�j��] */		/* �����e(�����Ɩ���)2 */
			}
			hatudan_system_regist_n_way();		/* (r33-) */
		}
	}
	#if 1
	/* �����w�A�j���[�V���� */
//	src->rotationCCW1024--;/* �E��� */
	src->rotationCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->rotationCCW1024);
	#endif
	set_timeout_alpha(src);
	/* �I�v�V�����ʒu�A�ړ� */
	/* �ڕW��ݒ肵�A�U���ړ� */
//	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_sakuya_baramaki1(SPRITE *src)
{
	int i;
	for (i=0; i<(4); i++) /* 4�� */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_20_YOUSEI1_SYOUMEN;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_sakuya_baramaki1;
			h->callback_loser		= lose_tukaima_00;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->cx256				= ((src->cx256) & 0xfffffffe);/* [�ԏ��]�C���^�[���[�u�p�r�b�g��1�r�b�g�m�� */
			h->cy256				= (src->cy256);
			h->BOSS_DATA_04_toutatu_wariai256				= t256(1.0);
		//
			{
				const s8 locate_xy_table[(4+4)] =
				{	/* �� */	/* �� */	/* �� */	/* �� */
				//	(-90),		(-120), 	(+120), 	(+90),	// x0, x1, x2, x3 (r32)
				//	(+90),		(0),		(0),		(+90),	// y0, y1, y2, y3 (r32)
					(-64),		(-100), 	(+100), 	(+64),	// x0, x1, x2, x3 (r33)
					(+64),		(0),		(0),		(+64),	// y0, y1, y2, y3 (r33)
				};
				h->BOSS_DATA_00_target_x256 = (h->cx256)+((locate_xy_table[i  ])<<8) + (i&1);/* [�ԏ��]���C���^�[���[�u */
				h->BOSS_DATA_01_target_y256 = (h->cy256)+((locate_xy_table[i+4])<<8);
			}
	//
			h->base_hp				= (8192-1);
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score(	10000), 	/* easy 1�� (�v 5��==5x 1��) (r33) */
					score(	30000), 	/* norm 3�� (�v15��==5x 3��) (r33) */
					score(	50000), 	/* hard 5�� (�v25��==5x 5��) (r33) */
					score( 100000), 	/* luna10�� (�v50��==5x10��) (r33) */
				};
			h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
		//	h->base_score			= adjust_score_by_difficulty(score( 50000));	/* 5�� (�v30��==6x5��) (r33) */
			}
			h->shot_angle65536		= (0);
		//	h->BOSS_DATA_05_move_jyumyou		= (0x03ff); 	/* �������� (r32) */
			h->BOSS_DATA_05_move_jyumyou		= (0x03ff); 	/* �������� (r33) */
		}
	}
}
