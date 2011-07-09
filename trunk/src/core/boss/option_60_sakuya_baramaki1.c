
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
		�Ēe					�ΕĒe
		�E���					�����
		(+90�x) 				(-45�x)

			�ΕĒe			�Ēe
			�����			�E���
			(+45�x) 		(0�x)
	-------------------------------------------------------
	�߂�2:�����N�i�C(6)�e	[���̕ǂŔ��˂���]
		�ԃN�i�C�e				�N�i�C�e
		�E���					�����

			�N�i�C�e		�ԃN�i�C�e
			�����			�E���
---------------------------------------------------------*/
#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif


#define shot_angle1024		user_data06 	/* �V���b�g����������ێ� */


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_sakuya_baramaki1(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	#define AO_OR_AKA	(src->target_x256&1)/* [�ԏ��] */
//
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->boss_time_out)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->boss_time_out)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
			if (0 < src->toutatu_wariai256 )
			{
				src->toutatu_wariai256 -= (1);
			}
		//
			src->shot_angle1024 += ((AO_OR_AKA)?(-(1024/18)):((1024/18)));	/* �V���b�g�����������A��]������B */
		//
			send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
			br.BULLET_REGIST_00_speed256				= (t256(1.5))+((((cg_game_difficulty))<<6));		/* �e��(r32/r33) */
		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_03_AOI+(AO_OR_AKA)+(AO_OR_AKA);	/* [�ΕĒe] */
			/* ������ 6 way�ł͂Ȃ��āA 3 way ��2��ǉ�����B�łȂ��ƃv���C�I���e�B�[���ςɂȂ�B */
		//	br.BULLET_REGIST_06_n_way					= (2+difficulty);						/* [2-5way](r32) */
			br.BULLET_REGIST_06_n_way					= (3);									/* [5way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/160);						/* �����e(r32)6.4 */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/256);						/* �����e(�����Ɩ���)4 */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/512);						/* �����e(�����Ɩ���)2 */
			int jj;
			for (jj=0; jj<2; jj++)
			{
				const int kakusan_tbl[4] =
				{(int)(1024/512), (int)(1024/512), (int)(1024/256), (int)(1024/128)};		// Lunatic �͂��Ȃ�g�U����B
				br.BULLET_REGIST_07_VECTOR_div_angle1024		= kakusan_tbl[(cg_game_difficulty)];						/* �����e(�����Ɩ���)2 */
				if (0!=jj)	{br.BULLET_REGIST_07_VECTOR_div_angle1024	-=br.BULLET_REGIST_07_VECTOR_div_angle1024;}	/* ������ */
				br.BULLET_REGIST_02_VECTOR_angle1024			= (src->shot_angle1024)+(br.BULLET_REGIST_07_VECTOR_div_angle1024); 			/* */
				bullet_regist_multi_vector_direct();
			}
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
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_sakuya_baramaki1;
			h->callback_loser		= lose_option_00;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->cx256				= ((src->cx256) & 0xfffffffe);/* [�ԏ��]�C���^�[���[�u�p�r�b�g��1�r�b�g�m�� */
			h->cy256				= (src->cy256);
			h->toutatu_wariai256				= t256(1.0);
		//
			{
				const s8 locate_xy_table[(4+4)] =
				{	/* �� */	/* �� */	/* �� */	/* �� */
				//	(-90),		(-120), 	(+120), 	(+90),	// x0, x1, x2, x3 (r32)
				//	(+90),		(0),		(0),		(+90),	// y0, y1, y2, y3 (r32)
					(-64),		(-100), 	(+100), 	(+64),	// x0, x1, x2, x3 (r33)
					(+64),		(0),		(0),		(+64),	// y0, y1, y2, y3 (r33)
				};
				h->target_x256	= (h->cx256)+((locate_xy_table[i  ])<<8) + (i&1);/* [�ԏ��]���C���^�[���[�u */
				h->target_y256	= (h->cy256)+((locate_xy_table[i+4])<<8);
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
			h->base_score			= difficulty_score_tbl[(cg_game_difficulty)];	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
		//	h->base_score			= adjust_score_by_difficulty(score( 50000));	/* 5�� (�v30��==6x5��) (r33) */
			}
			h->shot_angle1024		= (0);
		//	h->boss_time_out		= (0x03ff); 	/* �������� (r32) */
			h->boss_time_out		= (0x03ff); 	/* �������� (r33) */
		}
	}
}
