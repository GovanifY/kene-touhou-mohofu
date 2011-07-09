
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�P�� �I�v�V����
	-------------------------------------------------------
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

#define rotate_angle1024	user_data06 	/* �{�X�𒆐S�Ƃ��āA��]�p�x�B(��CCW1024�`��) */
#define shot_angle1024		user_data07 	/* */



/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_kaguya_doll(SPRITE *src)
{
	check_boss_option_time_out(src);	/* ���Ԍo�߂ŏI���B�{�X��|���ƊF�j�󂳂��B */
//
	/*	���� & ��镗�A��]�V���b�g */
	if (0x0ff < src->boss_time_out)/* 0x100�����͋��ʋK�i�㌂���Ȃ� */
	{
		if (0 == ((src->boss_time_out)&0x07))	/* ���̃V���b�g�����܂ł̊Ԋu�A���ԁB */
		{
//			src->shot_angle1024 += (1024/18);	/* �V���b�g�����������A��]������B */
//		//
//			br.BULLET_REGIST_00_speed256				= (t256(1.5))+(((difficulty)<<6));	/* �e�� */
//			br.BULLET_REGIST_02_VECTOR_angle1024		= (src->shot_angle1024);			/* */
//			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
//		//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
//			br.BULLET_REGIST_06_n_way					= (2+difficulty);					/* [2-5way] */
//			br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/160);					/* �����e */
//			bullet_regist_multi_vector_direct();
		{
			src->shot_angle1024 -= (16);		/* cv1024r(10)*/
//
				br.BULLET_REGIST_00_speed256					= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//				br.BULLET_REGIST_02_VECTOR_angle1024			= (src->shot_angle1024&(256-1))+512+128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
				br.BULLET_REGIST_02_VECTOR_angle1024			= (src->shot_angle1024&(256-1))-512-128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
				br.BULLET_REGIST_03_VECTOR_regist_type			= LEGACY_REGIST_TYPE_00_HANERU;
				br.BULLET_REGIST_04_bullet_obj_type 			= (BULLET_MINI8_03_AOI);	/* �e�O�� */
				#if (1==USE_HAZUMI)
				br.BULLET_REGIST_06_VECTOR_HANERU_KAISUU		= VECTOR_TYPE_000_HANE_NAI;
				#endif /* (1==USE_HAZUMI) */
			//	br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256	= ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
				br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
				bullet_regist_legacy_vector_send1_xy_src(src);  	/* �e��x256 y256 ���S���甭�e�B */
		}
		}
	}
	//
	set_timeout_alpha(src);
	//
	/* �I�v�V�����ʒu�A��]�ړ� */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* �I�v�V�����ʒu�A�ړ� */
	#define HANKEI_45_DOT (45)				/* ���a */
	src->cx256 = src->target_x256 + ((sin1024((src->rotate_angle1024))*(HANKEI_45_DOT)));	/*CCW*/
	src->cy256 = src->target_y256 + ((cos1024((src->rotate_angle1024))*(HANKEI_45_DOT)));
	#undef HANKEI_45_DOT
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_zako_kaguya_dolls01(SPRITE *src)
{
//	#define ADD_ANGLE (341) 	/* 1���� 3���������p�x�A341.33 == 1024/3  */
//	#define ADD_ANGLE (171) 	/* 1���� 6���������p�x�A170.66 == 1024/6  */
	#define ADD_ANGLE (205) 	/* 1���� 5���������p�x�A204.80 == 1024/5  */
//	#define ADD_ANGLE (51)		/* 1����20���������p�x�A 51.20 == 1024/20 */
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
			h->type 				= BOSS_16_YOUSEI11;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_kaguya_doll;
			h->callback_loser		= lose_option_00;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->target_x256			= (src->cx256);
			h->target_y256			= (src->cy256);
	//
			h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score( 100000), 	/* easy 10�� (�v 50��==5x 10��) */
					score( 300000), 	/* norm 30�� (�v150��==5x 30��) */
					score( 500000), 	/* hard 50�� (�v250��==5x 50��) */
					score(1000000), 	/* luna100�� (�v500��==5x100��) */
				};
			h->base_score			= difficulty_score_tbl[(cg_game_difficulty)];	/* ��Փx�ɂ�邪�A���Ȃ�҂���B */
		//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50�� (�v300��==6x50��) */
			}
		//
			h->rotate_angle1024 	= i_angle1024;
			h->shot_angle1024		= (0);
		//	h->boss_time_out		= (0x01ff); 	/* �������� */
			h->boss_time_out		= (0x02ff); 	/* �������� */
		}
	}
}
