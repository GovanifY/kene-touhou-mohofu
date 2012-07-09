
/* ���{�X���J�[�h���� */
#include "./../boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�͓Z�߂�\��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�ݒ�
---------------------------------------------------------*/

	#define USE_X_HOUKOU (1)

/*---------------------------------------------------------
	����
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
#endif


	/* �G�̌��� tmp_angleCCW1024 */
//
	#define zd00_target_x256				user_data00
	#define zd01_target_y256				user_data01
	#define zd00_clip_left256				user_data00
	#define zd01_clip_right256				user_data01
//
	#define zd03_system_hoson_angle1024 	user_data03/*([system�l]�ۑ��p�x)*/
	#define zd03_turnspeed1024				user_data03
	#define zd03_px256						user_data03 	/* ���zx�ʒu(���]�p) */
	#define zd03_start_y256 				user_data03
//
	#define zd04_system_speed256			user_data04/*([system�l]���x)*/
	#define zd04_radius256					zd04_system_speed256
	#define zd04_zako_wait1 				user_data04 	/* [�����w]�J�E���^1 / [�Ζы�] �㉺�ɓ������J�E���^ */
//
	#define zd05_system_hoson_speed256		user_data05/*([system�l]�ۑ����x)*/
	#define zd05_kaisu_nnn					user_data05 	/* �J��Ԃ����񐔂̃J�E���^ / �^�[������� / �e������ */
	#define zd05_zako_wait2 				user_data05 	/* [�����w] �J�E���^2 */
	#define zd05_max_y256					user_data05
	#define zd05_kougeki_type				user_data05/*[�D�ы�]*/
	#define zd05_KARASU3_houkou 			user_data05
//
//

//#define zako_pattern_base 				user_data07 		/* �p�^�[���̊�ʒu�B�A�j������ꍇ�̃x�[�X */
#define zako_anime_rotate_angle1024 		user_data07/*zako_anime_base*/	/* �A�j����]�p�x1024 */

/*---------------------------------------------------------
	�U�����@
---------------------------------------------------------*/
enum
{
	ZAKO_SPEKA_00_hai_kedama1 = 0,// �D�ыʂ̃J�[�h1
	ZAKO_SPEKA_01_hai_kedama2,// �D�ыʂ̃J�[�h2
	ZAKO_SPEKA_02_hai_kedama3,// �D�ыʂ̃J�[�h3
	ZAKO_SPEKA_03_hai_kedama4,// �D�ыʂ̃J�[�h4
	ZAKO_SPEKA_04_hai_kedama5,// �D�ыʂ̃J�[�h5
	ZAKO_SPEKA_05_hai_kedama6,// �D�ыʂ̃J�[�h6
	ZAKO_SPEKA_06_hai_kedama7,// �D�ыʂ̃J�[�h7
	ZAKO_SPEKA_07_hai_kedama8,// �D�ыʂ̃J�[�h8
	//
	ZAKO_SPEKA_08_mahou_nerai,
	ZAKO_SPEKA_09_inyou1,
	ZAKO_SPEKA_0a_midori1,
	ZAKO_SPEKA_0b_meido2,
	ZAKO_SPEKA_0c_kisume,// �L�X���̃J�[�h1
	ZAKO_SPEKA_0d_yukari,
	ZAKO_SPEKA_0e_obake1,
	ZAKO_SPEKA_0f_mahou_common,
	//
	ZAKO_SPEKA_10_aaa,// ���g�p
	ZAKO_SPEKA_11_mahou_tate,
	ZAKO_SPEKA_12_mahou_random,
	ZAKO_SPEKA_13_aaa,// ���g�p
	ZAKO_SPEKA_14_mahou_qqq,//	4:�~��U���̃J�[�h(����)
	ZAKO_SPEKA_15_mahou_qqq,//	5:�~��U���̃J�[�h(���E)
	ZAKO_SPEKA_16_mahou_qqq,//	6:�~��U���̃J�[�h(�卶)
	ZAKO_SPEKA_17_mahou_qqq,//	4:�~��U���̃J�[�h(����)
	//
	ZAKO_SPEKA_18_aaa,// ���g�p
	ZAKO_SPEKA_19_midori2,
	ZAKO_SPEKA_1a_meido3,
	ZAKO_SPEKA_1b_ao2,
	ZAKO_SPEKA_1c_ao1,
	ZAKO_SPEKA_1d_karasu,
	ZAKO_SPEKA_1e_kedama1,
	ZAKO_SPEKA_1f_ao3_rendan,
	//
	ZAKO_SPEKA_99_MAX,
};
/*---------------------------------------------------------
	��-�{�X�G����
---------------------------------------------------------*/
/*global*/static OBJ_CALL_FUNC(item_create_15_rumia)
{
	const u8 item_tbl[4] =
	{
		(SP_ITEM_02_P128&0xff), 	/* [F] */
		(SP_ITEM_01_P008&0xff), 	/* [P]�� */
		(SP_ITEM_00_P001&0xff), 	/* [p]�� */
		(SP_ITEM_05_TENSU&0xff),	/* [�_] */
	};
	item_create_flags_num(src, /*item_type*/(OBJ_Z03_ITEM|(item_tbl[(cg.game_difficulty)])), (7), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
	/* ����΂� */
}
/*global*/static OBJ_CALL_FUNC(item_create_14_yokai)
{
	item_create_flags_num(src, SP_ITEM_05_TENSU, (7), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
	/* ����΂� */	/* �_�����o�� */
}
/*---------------------------------------------------------

---------------------------------------------------------*/


/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_00_item)	{item_create_mono(src, SP_ITEM_00_P001  );}
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_01_item)	{item_create_mono(src, SP_ITEM_01_P008  );}
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_02_item)	{item_create_mono(src, SP_ITEM_02_P128  );}
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_03_item)	{item_create_mono(src, SP_ITEM_03_1UP   );}
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_04_item)	{item_create_mono(src, SP_ITEM_04_BOMB  );}
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_05_item)	{item_create_mono(src, SP_ITEM_05_TENSU );}//static void item_create_018_meido4
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_06_item)	{item_create_mono(src, SP_ITEM_06_HOSI  );}
#define item_create_07_item item_create_random_table


/*---------------------------------------------------------
	�U�R�G����
---------------------------------------------------------*/
/* aaa%�̊m���� */
#define replay_rand_percent(aaa) ((unsigned char)(ra_nd())<=(unsigned char)( (aaa*256)/100 ) ) /* aaa%�̊m���� */

/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_000_kougeki_mahoujin)
{
	if (replay_rand_percent(30))	{item_create_mono(src, SP_ITEM_00_P001	);}
	if (replay_rand_percent(30))	{item_create_mono(src, SP_ITEM_00_P001	);}
	if (replay_rand_percent(30))	{item_create_mono(src, SP_ITEM_05_TENSU );}
}

static void zako_shot_supeka(OBJ/**/ *src, u8 zako_supeka_type);/*(�錾)*/
/*global*/static/*static*/ OBJ_CALL_FUNC(item_create_002_inyou_zako)
{
//	if (0==(replay_rand()&(16-1)))	/*�m���グ���B[1/16]��[1/20]*/ /*%20*/
//	if (0==(replay_rand()&( 8-1)))	/*�m���グ���B[1/8]��[1/20]*/ /*%20*/
	if (0==((src->center.x256)&( 8-1))) /*�m���グ���B[1/8]��[1/20]*/ /*%20*/
	{
		zako_shot_supeka(src, ZAKO_SPEKA_09_inyou1);
	}
	else
	{
	//	item_create_mono(src, (SP_ITEM_00_P001+(replay_rand()&1)) );
		item_create_mono(src, (SP_ITEM_00_P001+((src->center.x256)&1)) );
	}
}
/*50%(SP_ITEM_00_P001 or SP_ITEM_01_P008)*/
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008	 (SP_ITEM_EXTRA_HOMING+(replay_rand()&3/*%3*/)),


//static OBJ_CALL_FUNC(item_create_003_tatsumaki1)
//{
//	destoroy++;
//	if ((NUM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (replay_rand_percent(50/*10*/))
//		{
//			item_create(src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(replay_rand()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(replay_rand()&3/*%3*/)),


//static OBJ_CALL_FUNC(item_create_017_meido3)
//{
//	destoroy[static_last]++;
//	if ((NUM_OF_ENEMIES-1) < destoroy[static_last]/*all_destoroy*/)
//	{
//		destoroy[static_last] = 0;
//		item_create(src, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	}
//}
//		if (0==(replay_rand()&(512-1))) /* 1/512 �� 1/600 �̊m���m���ŃA�C�e���o�� */	/*%600*/
//		{
//			item_create(src, SP_ITEM_00_P001, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}


/*---------------------------------------------------------
	�U�R�̍U�����@�͓Z�߂�ׂ�
---------------------------------------------------------*/


/*---------------------------------------------------------
	������p�~�܂�e
	-------------------------------------------------------
	�S�e�������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/
/*---------------------------------------------------------
	�x�N�g���e(��{�I�ɂ́A�p�x�ω����Ȃ��e)
---------------------------------------------------------*/

#define BASE_add_speed256			user_data00 	/* ������ */
#define TOKUSYU_DAN_DATA_delta256	user_data01 	/* ���������� */




#define TOMARI_DAN_LIMIT_00_MAX 	(512+384)	/* �K���ɑ傫��1024==(512+64+�ő��~����) */
#define TOMARI_DAN_LIMIT_01_512 	(512)
#define TOMARI_DAN_LIMIT_02_032 	(32+0+0)

static OBJ_CALL_FUNC(bullet_move_douchu_tomari_dan)
{
	if (TOMARI_DAN_LIMIT_01_512 < src->jyumyou)/* 513 ... MAX */
	{
		/* �~�܂� */
	//	src->BASE_add_speed256 -= (1);
		src->BASE_add_speed256 -= (src->TOKUSYU_DAN_DATA_delta256);
		if (src->BASE_add_speed256 < t256(0))
		{
			src->BASE_add_speed256 = t256(0);
			src->jyumyou = (TOMARI_DAN_LIMIT_01_512);
		}
	}
	else
	if (TOMARI_DAN_LIMIT_02_032 < src->jyumyou)/* 33 ... 512 */
	{
		;	/* ���΂���~ */
	}
	else/* 0 ... 32 */
	{
		/* ������ */
		src->color32 -= 0x10000000; 		/*	src->alpha -= 0x10; */
		if (0x20000000 > src->color32)		/*	( 0x20 > src->alpha)	*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;	*/
			src->jyumyou = JYUMYOU_NASI;
		}
	}
	//
	src->center.y256 += t256(1.0);	/* ������p�e�Ȃ̂ŃX�N���[������ */
	//
//	move_bullet_vector(src);
	if (t256(1.00)!=(src->BASE_add_speed256))
	{
		#if (0)
	//	src->math_vector.x256 = ((src->math_vector.x256*src->BASE_add_speed256)>>8);/* ����/���� */ // ���̏ꍇ�_���B
		#else
		const int vx256_speed = (src->math_vector.x256*src->BASE_add_speed256);
		if (0 < vx256_speed) /* ���̏ꍇ */
		{
			src->math_vector.x256 = ((vx256_speed)>>8);/* ����/���� */
		}
		else /* ���̏ꍇ */
		{
			src->math_vector.x256 = -((-vx256_speed)>>8);/* ����/���� */
		}
		#endif
		//
		#if (1)/* ���e���������ĂȂ��̂�... */
		src->math_vector.y256 = ((src->math_vector.y256*src->BASE_add_speed256)>>8);/* ����/���� */ // ���̏ꍇ�_���B
		#else
		const int vy256_speed = (src->math_vector.y256*src->BASE_add_speed256);
		if (0 < vy256_speed) /* ���̏ꍇ */
		{
			src->math_vector.y256 = ((vy256_speed)>>8);/* ����/���� */
		}
		else /* ���̏ꍇ */
		{
			src->math_vector.y256 = -((-vy256_speed)>>8);/* ����/���� */
		}
		#endif
	}
	src->center.x256 += (src->math_vector.x256);	/*fps_factor*/
	src->center.y256 += (src->math_vector.y256);	/*fps_factor*/

}

/*---------------------------------------------------------
	�e��o�^����B
	-------------------------------------------------------
	�e�������̃G�t�F�N�g���̊֌W��A���e�����͑S���Z�߂�K�v������B
	-------------------------------------------------------
	�x�N�g���e�Ɗp�x�e�𓝍����邩������Ȃ��B
	-------------------------------------------------------
	ToDo:
	�e�V�X�e���Ɉڍs����ׁA�p�~�\��B
	�J�[�h�X�N���v�g�V�X�e�����ғ�����ׂɂ́A���e���ꃖ���ɓ������Ȃ���
	���x�I�Ȗʂł��̂܂܂̌`�Ԃł͖���������̂ŁA
	(�����̔��e�����̂����ő��̒e��������������)
	�����̔��e������ hatudan_system_regist_single �ɒu��������K�v������B
---------------------------------------------------------*/
/* �x�N�g������e(���A�ڍs�`��) */

#define hatudan_system_kousadan_angle65536 tmp_angleCCW1024
static OBJ_CALL_FUNC(angle_to_vector)/*(�p�x��X���AY���̃x�N�g�����x�֕ϊ�����)*/
{
	REG_0b_REG3 = ((src->hatudan_system_kousadan_angle65536));/*(�����e�p�ɔ��e���̊p�x��65536�ł���̂Ŏg���B)*/
		#if (0)//
		REG_02_DEST_X	= ((si n1024(((REG_0b_REG3)>>6))*(REG_09_REG1))>>8);	/* CCW�̏ꍇ */
		REG_03_DEST_Y	= ((co s1024(((REG_0b_REG3)>>6))*(REG_09_REG1))>>8);
		#else
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( (REG_0b_REG3>>6), &sin_value_t256, &cos_value_t256);
			/*	spell_register[register_number_value]	*/	REG_0c_REG4 = (sin_value_t256);
			/*	spell_register[register_number_value+1] */	REG_0d_REG5 = (cos_value_t256);
			REG_02_DEST_X	= (((REG_0c_REG4)*(REG_09_REG1))>>8);
			REG_03_DEST_Y	= (((REG_0d_REG5)*(REG_09_REG1))>>8);
		}
		#endif
	src->math_vector.x256	= (REG_02_DEST_X);/*fps_factor*/
	src->math_vector.y256	= (REG_03_DEST_Y);/*fps_factor*/
}
static OBJ_CALL_FUNC(bullet_regist_douchu_vector_tomari_dan)
{
	OBJ *h;
	h											= obj_regist_tama();/* ���e�o�^ */
	if (NULL != h)/* �o�^�ł����ꍇ */
	{
		h->center.x256							= REG_02_DEST_X;/* ���e�ʒu ���Wx */
		h->center.y256							= REG_03_DEST_Y;/* ���e�ʒu ���Wy */
		h->obj_type_set 						= (HATSUDAN_05_bullet_obj_type);
		reflect_sprite_spec(h, OBJ_BANK_SIZE_00_TAMA);	/* �e�O���ƒe�����蔻���ύX����B */
		//
		mask65536(HATSUDAN_03_angle65536);
		h->hatudan_system_kousadan_angle65536	= (HATSUDAN_03_angle65536);
		h->m_Hit256R							= TAMA_ATARI_MARU16_PNG;
		//
		/*angle512								= angle_jikinerai512_auto(obj_player, src, angle512);*/
		REG_09_REG1 							= (HATSUDAN_01_speed256);
		angle_to_vector(h);
		//
		/*user_data02==(r35-�e���V�X�e���p�r�Ń��C���[�Ǘ����Ă���̂ŏd�v)*/
		// [���K�V�[�V�X�e���Ή�]
		h->hatudan_register_spec_data			= (0);/*(r35���K�V�[�V�X�e�����ŁA�g��Ȃ��Ă��A�e���V�X�e�����Ŏg���̂ł����Őݒ肷��K�v������B)*/
		//
		/* �Ƃ肠�����A�e�L�g�[ */
	//	h->BASE_add_speed256					= (t256(1.00)); 	/* ����/�����A�Ȃ� */
		h->BASE_add_speed256					= 1+(t256(1.00));	/* ���� �e */
		//
		h->TOKUSYU_DAN_DATA_delta256			= (HATSUDAN_02_speed_offset);
		h->jyumyou								= (TOMARI_DAN_LIMIT_00_MAX);
		h->callback_mover						= bullet_move_douchu_tomari_dan;
	}
}


/*---------------------------------------------------------
	�U�R�̍U�����@�͓Z�߂�ׂ�
8 1.00
7 0.50
6 0.25
5 0.125
---------------------------------------------------------*/
typedef struct /*aa*/
{
	int bullet_obj_type;	// HATSUDAN_05_bullet_obj_type
	u16 kankaku;
	u16 speed256;			// HATSUDAN_01_speed256
	u16 n_way;				// HATSUDAN_06_n_way
	u16 div_angle65536; 	// HATSUDAN_07_div_angle65536
} ZAKO_SPELL_RESOURCE;
static ZAKO_SPELL_RESOURCE kougeki_tbl[(ZAKO_SPEKA_99_MAX)] =
{
	/* 0:�g���ۂ��ы�(���p�x ��e) t256(2.5) */
	{(BULLET_KOME_BASE),						0x3f, t256(1.25),	(5), (u16)(65536/12)},	/* ZAKO_SPEKA_00_hai_kedama1[e] (u32)(1024/12):�L�p�Ȃ̂ŊȒP */
	{(BULLET_KOME_BASE),						0x1f, t256(1.375),	(3), (u16)(65536/16)},	/* ZAKO_SPEKA_01_hai_kedama2[n] 0x0f:���ʂɔ������Ԋu */
	{(BULLET_KOME_BASE),						0x0f, t256(1.5),	(3), (u16)(65536/20)},	/* ZAKO_SPEKA_02_hai_kedama3[h] 0x0f:���ʂɔ������Ԋu */
	{(BULLET_KOME_BASE),						0x07, t256(1.625),	(5), (u16)(65536/24)},	/* ZAKO_SPEKA_03_hai_kedama4[l] (u32)(1024/24):���ʂ̋��p */	/* 1:�d���ۂ��ы�(90�x 4����) (���ʒe�Ȃ̂œ�Փx�͒Ⴂ��) */
	{(BULLET_KOME_BASE),						0x3f, t256(1.25),	(8), (u16)(65536/ 8)},	/* ZAKO_SPEKA_04_hai_kedama5[e] 8:�����e(���ʒe�Ȃ̂œ�Փx�͒Ⴂ��) */
	{(BULLET_KOME_BASE),						0x1f, t256(1.375),	(8), (u16)(65536/ 8)},	/* ZAKO_SPEKA_05_hai_kedama6[n] 0x1f:���ʂɔ������Ԋu */
	{(BULLET_KOME_BASE),						0x07, t256(1.5),	(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_06_hai_kedama7[h] */
	{(BULLET_KOME_BASE),						0x03, t256(1.625),	(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_07_hai_kedama8[l] 0x03:�A���e���ۂ� */
	//
	{(BULLET_MINI8_BASE + TAMA_IRO_03_AOI), 	0x03, t256(0.5),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_08_mahou_nerai */ 		// 6 -	// [�����w 3/�͗d��1:�����_���΂�܂��U��+���@�_��("�����w1")�̃J�[�h]
	{(BULLET_HARI32_01_AKA),					0x03, t256(2.5),	(5), (u16)(65536/24)},	/* ZAKO_SPEKA_09_inyou1 */				// 6 7	// [�j�e]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1.375),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0a_midori1 */ 			// 6 -	// [�Ζы�1/���C�h1 �̃J�[�h]
	{(BULLET_HARI32_00_AOI),					0x03, t256(2.5),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0b_meido2 */				// 6 -	// [ ���C�h2�̃J�[�h]
	{(BULLET_MINI8_BASE + TAMA_IRO_07_DAI_DAI), 0x03, t256(1.25),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0c_kisume */ // 6 -	// [ �L�X��1�̃J�[�h]
	{(BULLET_MINI8_BASE + TAMA_IRO_07_DAI_DAI), 0x03, t256(0.75),	(1), (u16)(65536/ 1)},	/* ZAKO_SPEKA_0d_yukari */				// 6 -	// [ ���ґ�2�̃J�[�h]
	{(BULLET_KNIFE20_04_AOI),					0x03, t256(0.625),	(4), (u16)(65536/18)},	/* ZAKO_SPEKA_0e_obake1 */				// . 7	// [���΂�1 �̃J�[�h]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1), /*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_0f_mahou_common */		//		// [�����w �Ƃ肠�������ʕ���]
	//
	{(0),										0x03, 0,			(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_10_aaa-*/
	{(BULLET_KOME_BASE + TAMA_IRO_01_AKA),		0x01, t256(1.25),	(2), (u16)(65536/ 2)},	/* ZAKO_SPEKA_11_mahou_tate */			// 6 7	// [�����w 1:�c�U���̃J�[�h]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(2.0),	(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_12_mahou_random */		// . .	// [�����w 2:�����_���΂�܂��U��("�����w2")�̃J�[�h]
	{(0),										0x03, 0,			(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_13_aaa-*/
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1), /*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_14_mahou_qqq */			//		// [�����w �Ƃ肠�������ʕ���]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1), /*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_15_mahou_qqq */			//		// [�����w �Ƃ肠�������ʕ���]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1), /*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_16_mahou_qqq */			//		// [�����w �Ƃ肠�������ʕ���]
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1), /*?*/(4), (u16)(65536/ 7)},	/* ZAKO_SPEKA_0f_mahou_common */		//		// [�����w �Ƃ肠�������ʕ���]
	//
	{(0),										0x03, 0,			(4), (u16)(65536/ 4)},	/* ZAKO_SPEKA_18_aaa-*/
	{(BULLET_MINI8_BASE + TAMA_IRO_01_AKA), 	0x03, t256(1.5),	(4), (u16)(65536/48)},	/* ZAKO_SPEKA_19_midori2 */ 			// . 7	// [�Ζы�2 �̃J�[�h]
	{(BULLET_UROKO14_BASE + TAMA_IRO_01_AKA),	0x03, t256(1.0),	(4), (u16)(65536/24)},	/* ZAKO_SPEKA_1a_meido3 */				// . 7	// [ ���C�h3�̃J�[�h]
	{(BULLET_UROKO14_BASE + TAMA_IRO_03_AOI),	0x03, t256(1.0),	(4), (u16)(65536/24)},	/* ZAKO_SPEKA_1b_ao2 */ 				// . 7	// [ �d��1/�d��4�̃J�[�h(B)]
	{(BULLET_KUNAI12_BASE), 					0x03, t256(1.0),	(8), (u16)(65536/24)},	/* ZAKO_SPEKA_1c_ao1 */ 				// 6 7	// [ �d��1/�d��4�̃J�[�h(A)]
	{(BULLET_KUNAI12_BASE), 					0x03, t256(1.0),	(8), (u16)(65536/ 8)},	/* ZAKO_SPEKA_1d_karasu */				// 6 7	// [ �d��3/�G1�̃J�[�h]
	{(BULLET_KOME_BASE),						0x03, t256(0.5),	(4), (u16)(65536/24)},	/* ZAKO_SPEKA_1e_kedama1 */ 			// . 7	// [ �ы�1�̃J�[�h]
	{(BULLET_MINI8_BASE),						0x03, t256(1.25),	(7), (u16)(65536/ 4)},	/* ZAKO_SPEKA_1f_ao3_rendan */			// 6 -	// [ �d��2�̃J�[�h]
	//
};
		//	/*[r35�v������]*/HATSUDAN_01_speed256	+= ((((cg_game_di fficulty))<<6));/* [2.5 - 3.25] */		// (t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
		//	/*[r35�v������]*/HATSUDAN_01_speed256	+= ((((zako_supeka_type)&(4-1))<<6));/* [2.5 - 3.25] */ 	// (t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
		//	/*[r35������]*/HATSUDAN_01_speed256 	+= (((zako_supeka_type & 0x03)<<5));/* [2.5 - 3.25] */		// (t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */

//label_ZAKO_SPEKA_0d_yukari:
//	/*[r35�v������]*/HATSUDAN_01_speed256				+= t256(2.5);				/* �e�� */	/*�����e*/	/*t256(3+teki_rank)+(0(teki_rank<<6))*/
//	/*[r35�v������]*/HATSUDAN_01_speed256				+= t256((cg.game_difficulty))+t256(1.5);		/* �e�� */
//�d�l��_��(rank�g��)	/*[r35������]*/HATSUDAN_01_speed256 				+= ((cg.game_difficulty)<<7);		/* �e�� */
//label_ZAKO_SPEKA_1d_karasu:	// [ �d��3/�G1�̃J�[�h]
//	/*[r35�v������]*/HATSUDAN_01_speed256				= (t256(2.0)+(((cg.game_difficulty))<<6));
//�d�l��_��(rank�g��)	/*[r35������]*/HATSUDAN_01_speed256 				+= (((cg.game_difficulty))<<5);

//label_ZAKO_SPEKA_0d_yukari:	// [ ���ґ�2�̃J�[�h]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_MINI8_BASE + TAMA_IRO_07_DAI_DAI);	/* [�Ԓe] */ /* �e�O�� */
//		HATSUDAN_06_n_way								= (1);							/* [1way] */
//	//	HATSUDAN_07_div_angle65536						= (0);							/* �_�~�[�p�x(���g�p) */
//		goto COMMON_JIKINERAI;


		//	HATSUDAN_01_speed256						= (t256(1.5)+(cos1024((src->jyumyou<<4)))); 				/* �e�� */
/*...*/ //	HATSUDAN_01_speed256						= (t256(1.5)+(sin1024((src->jyumyou<<4)+255))); 			/* �e�� */
		//	HATSUDAN_01_speed256						= (t256(1.0)+(cos1024((src->jyumyou<<3))<<2));					/* �e�� */
/*...*/ //	HATSUDAN_01_speed256						= (t256(1.0)+(sin1024((src->jyumyou<<3)+255)<<2));				/* �e�� */
		//	HATSUDAN_01_speed256						= (t256(1.0)+(cos1024((src->jyumyou<<2))<<2));					/* �e�� */
/*...*/ //	HATSUDAN_01_speed256						= (t256(1.0)+(sin1024((src->jyumyou<<2)+255)<<2));				/* �e�� */

static void zako_shot_supeka(OBJ/**/ *src, u8 zako_supeka_type)
{
	#if 0/*(�Ă���)*/
	if (0==(cg.game_difficulty))
	{
		return;
	}
	#endif
	const void *aaa[(ZAKO_SPEKA_99_MAX)] =
	{
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_00_hai_kedama1 �D�ы� �̃J�[�h1
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_01_hai_kedama2 �D�ы� �̃J�[�h2
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_02_hai_kedama3 �D�ы� �̃J�[�h3
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_03_hai_kedama4 �D�ы� �̃J�[�h4
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_04_hai_kedama5 �D�ы� �̃J�[�h5
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_05_hai_kedama6 �D�ы� �̃J�[�h6
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_06_hai_kedama7 �D�ы� �̃J�[�h7
		&&label_ZAKO_SPEKA_18_haikedama,	// ZAKO_SPEKA_07_hai_kedama8 �D�ы� �̃J�[�h8
		//
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_08_mahou_nerai	�����w 3:�����_���΂�܂��U��+���@�_��("�����w1")�̃J�[�h
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_09_inyou1 		�A�z��1 �̃J�[�h [�j�e]
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0a_midori1		�Ζы�1 �̃J�[�h
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0b_meido2 		�̃J�[�h
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0c_kisume 	�L�X�� �̃J�[�h
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_0d_yukari 	�̃J�[�h
		&&label_ZAKO_SPEKA_0e_obake1,		// ZAKO_SPEKA_0e_obake1 ���΂�1 �̃J�[�h
		&&label_ZAKO_SPEKA_0f_mahou_common, // ZAKO_SPEKA_0f_mahou_common �����w �Ƃ肠�������ʕ���
		//
		&&COMMON_JIKINERAI, 				// mahoujin00 ZAKO_SPEKA_10_aaa [���g�p]�̃J�[�h	/* �U�����Ȃ� */
		&&label_ZAKO_SPEKA_11_mahou_tate,	// mahoujin01 ZAKO_SPEKA_11_mahou_tate �����w 1:�c�U���̃J�[�h
		&&label_ZAKO_SPEKA_12_mahou_random, // mahoujin02 ZAKO_SPEKA_12_mahou_random �����w 2:�����_���΂�܂��U��("�����w2")�̃J�[�h	/* �΂�܂� #01(�������A��"�����w2") */
		&&COMMON_JIKINERAI, 				// mahoujin03 ZAKO_SPEKA_13_aaa [���g�p]�̃J�[�h	/* �΂�܂� #02(�������A��"�����w1") */
		&&label_muki_mahou_right,			// mahoujin04 ZAKO_SPEKA_14_mahou_qqq [���g�p]�̃J�[�h	/* 8way�Ԓe(��) */
		&&label_muki_mahou_left,			// mahoujin05 ZAKO_SPEKA_15_mahou_qqq [���g�p]�̃J�[�h	/* 8way�Ԓe(��) */
		&&label_muki_mahou_right,			// mahoujin06 ZAKO_SPEKA_16_mahou_qqq [���g�p]�̃J�[�h	/* 8way�Ԓe(��) */
		&&label_muki_mahou_right,			// mahoujin04 ZAKO_SPEKA_17_mahou_qqq [���g�p]�̃J�[�h	/* 8way�Ԓe */
		//
		&&COMMON_JIKINERAI, 				// ZAKO_SPEKA_18_aaa [���g�p]�̃J�[�h
		&&label_ZAKO_SPEKA_19_midori2,		// ZAKO_SPEKA_19_midori2 �Ζы�2 �̃J�[�h
		&&label_ZAKO_SPEKA_1a_meido3,		// ZAKO_SPEKA_1a_meido3 	�̃J�[�h
		&&label_ZAKO_SPEKA_1b_ao2,			// ZAKO_SPEKA_1b_ao2		�̃J�[�h
		&&label_ZAKO_SPEKA_1c_ao1,			// ZAKO_SPEKA_1c_ao1		�̃J�[�h
		&&COMMON_JIKINERAI_p_cx,			// ZAKO_SPEKA_1d_karasu 	�̃J�[�h
		&&label_ZAKO_SPEKA_1e_kedama1,		// ZAKO_SPEKA_1e_kedama1	�̃J�[�h
		&&label_ZAKO_SPEKA_1f_ao3_rendan,	// ZAKO_SPEKA_1f_ao3_rendan �̃J�[�h
		//
	};
	zako_supeka_type &= 0x1f;/*(�O�̈�)*/
	//
	HATSUDAN_05_bullet_obj_type 			= kougeki_tbl[(zako_supeka_type)].bullet_obj_type;
	//
	HATSUDAN_01_speed256					= ((kougeki_tbl[(zako_supeka_type)].speed256)+((cg.game_rank)>>8));
	HATSUDAN_06_n_way						= kougeki_tbl[(zako_supeka_type)].n_way;
	HATSUDAN_07_div_angle65536				= ((kougeki_tbl[(zako_supeka_type)].div_angle65536) );
	//
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	//
	goto *aaa[(zako_supeka_type)/*& 0x1f*/];
label_ZAKO_SPEKA_18_haikedama:
	//static void zako_spell_01_hai_kedama(OBJ/**/ *src, u8 zako_supeka_type)
	{
		//	zako_supeka_type -= (ZAKO_SPEKA_18_hai_kedama);
		//	zako_supeka_type &= (0x07);
		if (0==(src->jyumyou & kougeki_tbl[(zako_supeka_type & 0x07)].kankaku))/*0x0f*/
		{
			HATSUDAN_05_bullet_obj_type 			= BULLET_KOME_BASE + (src->center.x256 & 0x07);
			goto COMMON_JIKINERAI;
		}
		else
		{
			goto EXIT_ZAKO_SPEKA;
		}
	}
		goto EXIT_ZAKO_SPEKA;
//-------------------------
		//	attack_kougeki_mahoujin_common(src);
//-------------------------
label_muki_mahou_left:
		HATSUDAN_03_angle65536							= (((-(src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7)))<<6);	/* ���� */
		goto label_ZAKO_SPEKA_0f_mahou_common;
label_muki_mahou_right:
		HATSUDAN_03_angle65536							= ((( (src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7)))<<6);	/* ���� */
	//	goto label_ZAKO_SPEKA_0f_mahou_common;
label_ZAKO_SPEKA_0f_mahou_common:	// [�����w �Ƃ肠�������ʕ���]
	//	HATSUDAN_03_angle65536							= (int)(65536/(7)); 						/* �p�x(����� 7 ����) */	/* 1����n���������p�x */
		HATSUDAN_02_speed_offset						= (1);//t256(0.03); 						/* �����萔 */
		goto COMMON_LEGACY_DIRECT;

label_ZAKO_SPEKA_12_mahou_random:	// [�����w 2:�����_���΂�܂��U��("�����w2")�̃J�[�h]
		/* ���x�� 1.0-3.0�܂ł̖��i�K�����ɕύX */
		HATSUDAN_01_speed256							+= ((ra_nd()&(512-1))); 	/* �e�� */
		HATSUDAN_03_angle65536							= ((ra_nd()&(65536-1)));					/* �p�x */
		HATSUDAN_02_speed_offset						= (2);//10.24==t256(0.04);					/* �����萔 */
		goto COMMON_LEGACY_DIRECT;
//------------------------- �Œ�e
label_ZAKO_SPEKA_11_mahou_tate: 	// [�����w	1:�c�U���̃J�[�h]
		HATSUDAN_03_angle65536							= (int)(65536/(4)); 		/* �E����(�㉺�e)�A�����e�Ȃ̂Ō��ʓI�ɏ㉺�e�B */
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 	/* �ԕĒe */
//		HATSUDAN_06_n_way								= (2);						/* [2way] */
//		HATSUDAN_07_div_angle65536						= (int)(65536/(2)); 		/* �p�x(����� 2 ����) */	/* 1����n���������p�x */
		goto COMMON_MULTI_VECTER;

label_ZAKO_SPEKA_0e_obake1: 		// [���΂�1 �̃J�[�h]	/*(�d�l��easy�͖���)*/
		#if 0
		if (0==(cg.game_difficulty))	{	return; 	}	/*(�d�l��easy�͖���)*/
		#endif
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KNIFE20_04_AOI);		/* [�i�C�t�e] */
		HATSUDAN_06_n_way								= ((cg.game_difficulty)+(cg.game_difficulty)-1);	/* [- 1 3 5way] */
//		HATSUDAN_07_div_angle65536						= (int)(65536/(18));			/* �����p�x */
		goto COMMON_MULTI_VECTER;

//------------------------- �A�e
label_ZAKO_SPEKA_1f_ao3_rendan: 	// [ �d��2�̃J�[�h]
		{	/* �~�j�e�̘A�e */
		//	#define KKKKK (0)
			unsigned int KKKKK;
			for (KKKKK=0; KKKKK<6/*4*/; KKKKK++)
			{
				static const u16 bunkatsu65536_tbl[4] =
				{((int)(65536/8)), ((int)(65536/12)), ((int)(65536/16)), ((int)(65536/24))};
		//	/*[r35�v������]*/HATSUDAN_01_speed256		= (t256(2.5)+(KKKKK<<6));					/* �e�� */
			/*[r35������]*/HATSUDAN_01_speed256 		= (t256(1.25)+(KKKKK<<5));					/* �e�� */
				HATSUDAN_05_bullet_obj_type 			= ((BULLET_MINI8_BASE + TAMA_IRO_01_AKA)+(/*0*/6-KKKKK/*�Ƃ肠����*/)); /* [ �e] */
				HATSUDAN_06_n_way						= (7);									/* [7way] */
				HATSUDAN_07_div_angle65536				= (bunkatsu65536_tbl[((cg.game_difficulty))]);		/* �����p�x */
				/* ���@�_���e�����B */
				calculate_jikinerai();
			//	bullet_regist_multi_vector_direct();
				HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
		goto EXIT_ZAKO_SPEKA;
//-------------------------

	COMMON_LEGACY_DIRECT:
		#if (1)/*(��������ʉ����邩�͖���)*/
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);			/* �e�O�� */
		#endif
		bullet_regist_douchu_vector_tomari_dan(src);/*(�G���ł͂����ȊO����)*/
		goto EXIT_ZAKO_SPEKA;
	//
//------------------------- ���@�_��
label_ZAKO_SPEKA_19_midori2:	// [�Ζы�2 �̃J�[�h]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);	/* [�Ԓe] */ /* �e�O�� */
		HATSUDAN_06_n_way								= (1+(cg.game_difficulty)); 	/* [1 2 3 4way] */
//		HATSUDAN_07_div_angle65536						= (65536/48);					/* �����p�x */
//	//	HATSUDAN_07_div_angle65536						= (65536/(48*64));					/* �����p�x */
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1a_meido3: 	// [ ���C�h3�̃J�[�h]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);
		HATSUDAN_06_n_way								= ((cg.game_difficulty)<<1);	/*[0 2 4 6way]*/
//		HATSUDAN_07_div_angle65536						= (int)(65536/24);
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1b_ao2:	// [ �d��1/�d��4�̃J�[�h(B)]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI);		/* [�e] */
		HATSUDAN_06_n_way								= ((cg.game_difficulty));						/* [0 1 2 3way] */	/*1*/
		HATSUDAN_07_div_angle65536						-= ((cg.game_difficulty)<<6);	/* �����p�x */
		goto COMMON_JIKINERAI;

label_ZAKO_SPEKA_1c_ao1:	// [ �d��1/�d��4�̃J�[�h(A)]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KUNAI12_BASE);/*((replay_rand())&7)*/ 	/* [�e�O��] */
//		HATSUDAN_06_n_way								= (8);											/* 8way�e */
		HATSUDAN_07_div_angle65536						-= ((cg.game_difficulty)<<6);	/* �����p�x */
		goto COMMON_JIKINERAI_p_cx;

label_ZAKO_SPEKA_1e_kedama1:		// [ �ы�1�̃J�[�h]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KOME_BASE);
		HATSUDAN_06_n_way								= (3+(cg.game_difficulty)+(cg.game_difficulty));	/* [3 5 7 9way] */
//		HATSUDAN_07_div_angle65536						= (int)(65536/24);
//label_ZAKO_SPEKA_1d_karasu:	// [ �d��3/�G1�̃J�[�h]
//		HATSUDAN_05_bullet_obj_type 					= (BULLET_KUNAI12_BASE);/*((replay_rand())&7)*/
//		HATSUDAN_06_n_way								= (8);
//		HATSUDAN_07_div_angle65536						= (int)(65536/(8));
//		goto COMMON_JIKINERAI_p_cx;
		/*(not_break;)*/
	COMMON_JIKINERAI_p_cx:
		HATSUDAN_05_bullet_obj_type 					+= (src->center.x256&0x07);
	//	goto COMMON_JIKINERAI;
		/*(not_break;)*/
	COMMON_JIKINERAI:
		calculate_jikinerai();
		/*(not_break;)*/
	COMMON_MULTI_VECTER:
	//	HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= (2);/*(�Ƃ肠����)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		hatudan_system_regist_n_way();/* (r33-) */
		//	goto EXIT_ZAKO_SPEKA;
	EXIT_ZAKO_SPEKA:
			;
}


/*---------------------------------------------------------
	�U�R�ړ����ʁA�������ړ������B
	-------------------------------------------------------
	���x: src->zd04_system_speed256�B�G���o�^���Ɏ����ݒ���g�p����B
	�p�x: src->tmp_angleCCW1024�B	 �G���o�^���Ɏ����ݒ���g�p����B
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
�܂肽���o�ꂷ�邾���ŁA�������ʂȏ��������Ȃ��A
�������ړ�����G���Ȃ�A����Ȋ����B
static OBJ_CALL_FUNC(move_zako_sample)
{
	if (JYUMYOU_ZAKO_HASSEI -byou60(3) > src->jyumyou)/*(��������3[�b]�ȏ�o�߂����G������ʊO�Ȃ�Ώ���)*/
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	zako_system_move_vector(src);/*(�U�R�V�X�e���W���ړ�����)*/
	zako_anime_type01(src);/*(���C�h���ʁA�A�j���[�V����)*/
}
#endif

static OBJ_CALL_FUNC(zako_system_move_vector)
{
	//------------------
	HATSUDAN_01_speed256	= (src->zd04_system_speed256);
	HATSUDAN_03_angle65536	= deg1024to65536((src->tmp_angleCCW1024));
	sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
	src->center.x256 += REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	src->center.y256 += REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	//------------------
}

#if 0
	#if (0)//
	src->center.x256 += ((si n1024((src->tmp_angleCCW1024))*(src->zd04_system_speed256))>>8);/*fps_factor*/ /* CCW�̏ꍇ */
	src->center.y256 += ((co s1024((src->tmp_angleCCW1024))*(src->zd04_system_speed256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
		src->center.x256 += ((sin_value_t256*(src->zd04_system_speed256))>>8);/*fps_factor*/
		src->center.y256 += ((cos_value_t256*(src->zd04_system_speed256))>>8);/*fps_factor*/
	}
	#endif
#endif

/*---------------------------------------------------------
	�U�R�A�j������
---------------------------------------------------------*/
/* 0000 0100 0000 0000 = 0x0400 = 1024 */
/* 0000 0011 1--- ---- */
static OBJ_CALL_FUNC(zako_anime_type_99_rotate)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*�O����]*/
	mask1024(src->rotationCCW1024);
}
static OBJ_CALL_FUNC(zako_anime_type01)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
	//	src->obj_type_set				= ((src->obj_type_set)&(~0x07)) + ((((src->tmp_angleCCW1024)&(1024-1)))>>7);
		src->obj_type_set				= ((src->obj_type_set)&(~0x07)) + ((((src->tmp_angleCCW1024)&(0x0380)))>>7);
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static OBJ_CALL_FUNC(zako_anime_type02)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->obj_type_set				= ((src->obj_type_set)&(~0x03)) + ((src->jyumyou>>2)&(4-1));
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static OBJ_CALL_FUNC(zako_anime_type05)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->obj_type_set				= ((src->obj_type_set)&(~0x03)) + ((src->jyumyou>>4)&(4-1));
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static OBJ_CALL_FUNC(zako_anime_type03)
{
	if (0==(src->zako_anime_rotate_angle1024))
	{
		src->obj_type_set				= ((src->obj_type_set)&(~0x01)) + ((src->jyumyou&0x10)>>4);
	}
	else
	{
		zako_anime_type_99_rotate(src);
	}
}
static OBJ_CALL_FUNC(zako_anime_type04)
{
	src->rotationCCW1024 += (src->zako_anime_rotate_angle1024);/*�O����]*/
	mask1024(src->rotationCCW1024);
}



/*---------------------------------------------------------

---------------------------------------------------------*/
#include "zako_move.h"					/* �G���ړ�/�o�^ */

/*---------------------------------------------------------
	�|�����ꍇ�̃A�C�e���w��
	���{�X�Ƌ��p����K�v������B
---------------------------------------------------------*/

static void regist_items_common(GAME_COMMAND *l, OBJ/**/ *src)
{
	void (*aaa[(16)])(OBJ/**/ *src) =
	{
		item_create_00_item,			item_create_01_item,				item_create_02_item,			item_create_03_item,
		item_create_04_item,			item_create_05_item,				item_create_06_item,			item_create_07_item,
		item_create_random_table,		item_create_000_kougeki_mahoujin,	item_create_002_inyou_zako, 	item_create_random_table,
		item_create_random_table,		item_create_random_table,			item_create_14_yokai,			item_create_15_rumia,
	};
	src->callback_loser 	= aaa[(((l->user_item8))&0x0f)];	/* �u�U�R/���{�X/(�{�X)�����ꂽ�ꍇ�̏����v�ɁA�u�����_���e�[�u������A�C�e�������v��ݒ� */
}
/*
src->jyumyou		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/

/*---------------------------------------------------------
	�|�����ꍇ�̃A�C�e���w��
	���{�X/�{�X�Ƌ��p����K�v������B
	-------------------------------------------------------
	(r36-)�̎d�l��A�X�R�A�ƃU�R�̂����蔻��͓������B
	���������� DAT�� �X�R�A 0 �ɐݒ肵�Ă����ƁA���G�U�R�ɂȂ�B
---------------------------------------------------------*/

/*global*/static void regist_settei_common(GAME_COMMAND *l, OBJ *src)
{
	regist_items_common(l, src);/* ���{�X�Ƌ��p����K�v������B */
	src->m_Hit256R			= ZAKO_ATARI16_PNG; 	/* �O�����炠���蔻������߂�B */
	//
	src->center.x256		= ((l->user_locate_x)<<8);	/* [pixel�P��] �ݒ�t�@�C������ �o��x���W �����߂�B */
	src->center.y256		= ((l->user_locate_y)<<8);	/* [pixel�P��] �ݒ�t�@�C������ �o��y���W �����߂�B */
	src->base_hp			= ((l->user_hp));			/* �ݒ�t�@�C������ �̗� �����߂�B */
	src->base_score 		= ((l->user_score));		/* �ݒ�t�@�C������ �l���X�R�A �����߂�B(�����蔻�苤�p) */
//
	//------------------
	// �p�x=1024�Ŏ��@�_���p�𐶐�(r39�b��d�l)�B
	if (1023 < (l->user_angle1024))
	{
	//	REG_02_DEST_X	= ((src->center.x256));
	//	REG_03_DEST_Y	= ((src->center.y256));
		set_REG_DEST_XY(src);
		calculate_jikinerai();
	//	src->tmp_angleCCW1024	= deg65536to1024(HATSUDAN_03_angle65536);		/* �p�xdeg1024 */	/* [deg1024�P��]  �ݒ�t�@�C������ �����߂�B */
	}
	else
	{
		src->tmp_angleCCW1024	= (l->user_angle1024);		/* �p�xdeg1024 */	/* [deg1024�P��]  �ݒ�t�@�C������ �����߂�B */
		HATSUDAN_03_angle65536	= deg1024to65536((src->tmp_angleCCW1024));
	}
	src->zd04_system_speed256		= (l->user_speed256);		/* ���xt256 */	/* [t256�P��]  �ݒ�t�@�C������ �����߂�B */
	//------------------
	HATSUDAN_01_speed256	= (src->zd04_system_speed256);
	sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
	src->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	//------------------ [�����l]
	src->zd03_system_hoson_angle1024	= src->tmp_angleCCW1024;
	src->zd05_system_hoson_speed256 = src->zd04_system_speed256;
	//
	src->obj_type_set		= (OBJ_Z02_TEKI)|((l->user_select_gazou)&0x3f); /* �ݒ�t�@�C������摜�����߂�B */
	/*
		DAT�͐l�Ԃ������̂ŁA�Ԉ���Ă��肷��B
		�p�^�[���A�j���[�V������p�̃U�R�L�����ɁA
		��]�n�O�����w�肵���ꍇ�A
		�p�^�[���A�j���[�V�����o���Ȃ��̂ŁA
		�o�O�����L�����ɂȂ��Ă��܂��B
		�����ł��������ꍇ�́A�����I�ɉ�]�n�ɕ␳����B
	*/
	if (11<((l->user_select_gazou)&0x0f))/*(12, 13, 14, 15) */
	{
		src->zako_anime_rotate_angle1024 = (10);	/* ��]�A�j�� */	/* ��]���鑬�� */
	}
	else
	{
		src->zako_anime_rotate_angle1024 = (0); 	/* �O���^�C�v */	/* off */
	}
}
//	(r36) src->base_score === src->atari_hantei 	= (1/*�X�R�A���p*/);	/* �f�t�H���g�͂����蔻�肠�� */


/*---------------------------------------------------------
	�U�R�o�^�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/
static void regist_NULL(GAME_COMMAND *l, OBJ *h)	{	;	}

typedef struct
{
	void (*aaa_init_callback)(GAME_COMMAND *l, OBJ *src);	/* ������ ���� */
	void (*aaa_move_callback)(OBJ *sss);					/* �ړ� ���� */
} SPELL_aaa_RESOURCE;

global void game_command_00_regist_zako(GAME_COMMAND *l)
{
	if (0 != cg.chuu_boss_mode)
	{
		/* ��-�{�X������ꍇ�ɂ͎G���ǉ����Ȃ��B */
		return;/* error */
	}
	if ((20) < l->user_255_code)
	{
		return;/* error */
	}
	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	/* teki�̐�����ԍ��ŊǗ�(load_stage.c �� ctype_name[]�ɑΉ����Ă���) */
	SPELL_aaa_RESOURCE aaa_resource[(20)] =
	{
	/* �����w */
		{regist_zako_000_mahoujin1, 				move_zako_00_kougeki_mahoujin}, 	/* "�����{" /"�����w1"	TEKI_RED_BOOK	 MAHOUJIN A */	/* �΂�܂��U�������w */
		{regist_zako_001_mahoujin2, 				move_zako_00_kougeki_mahoujin}, 	/* "�����w" /"�����w2"	TEKI_MAHOJIN	 MAHOUJIN B */	/* �����閂���w */
	/* ���� �A�z�� */
		{regist_zako_002_inyou1,					move_zako_02_inyou1},				/* "�A�z��" /"�A�z��1"	TEKI_INYOU_DAI */
		{regist_zako_003_tatsumaki1,				move_zako_03_tatsumaki1},			/* "����"	/"����1"	TEKI_TATSUMAKI */
	/* �d�� */
		{regist_zako_004_kakomi1,					move_zako_04_kakomi1},				/* "�͗d��" /"�͗d��1"	TEKI_CHOU1 */
		{regist_zako_005_kisume,					move_zako_05_kisume_1st},			/* "�L�X��" /"�Ԗы�1"	TEKI_CHEN */	/* �� */
	/* �ы� */
		{regist_NULL/*(��/��]���p)*/,				move_zako_06_inseki},				/* "覐�"	/"覐�1"	TEKI_NIJI_HOSI */	/* ���̑��U�R */
		{regist_zako_007_yukari2/*(��]��p?)*/,	move_zako_07_yukari},				/* "���ґ�" /"���ґ�2"	TEKI_NIJI_HOSI */	/* ���̑��U�R */
		{regist_zako_008_midori_kedama1,			move_zako_08_ao_kedama},			/* "�ы�" /"�Ζы�1"	TEKI_AKA_KEDAMA */
		{regist_NULL/*(��]��p?)*/,				move_zako_09_midori_kedama},		/* "�Ζы�" /"�Ζы�2"	TEKI_AKA_KEDAMA */
		{regist_zako_008_midori_kedama1,			move_zako_10_hai_kedama},			/* "�D�ы�" /"�ы�1"	TEKI_HAI_KEDAMA */
		{regist_zako_011_kedama2,					move_zako_11_siro_kedama},			/* "���ы�" /"�ы�2"	TEKI_HAI_KEDAMA */
	/* [C�d��]���̑��U�R */
		{regist_zako_012_ao_yousei, 				move_zako_12_ao_yousei},			/* "�d��" /"�d��2"	TEKI_YOUSEI1_1 */
		{regist_zako_013_obake2,					move_zako_13_obake},				/* "���΂�" /"���΂�2"	TEKI_YOUSEI2_5	"���ы�1" */
		{regist_zako_014_karasu1,					move_zako_14_karasu},				/* "�G" 	/"�G1"		TEKI_YOUSEI2_1 */
	/* [B�d��]���d�� */
		{regist_NULL,								move_meido1},						/* "���C�h1"	TEKI_YOUSEI3_1 */
		{regist_NULL,								move_meido2},						/* "���C�h2"	TEKI_YOUSEI3_1 */
		{regist_zako_017_meido3,					move_meido3},						/* "���C�h3"	TEKI_YOUSEI3_1 */
		{regist_NULL,								move_zako_18_meido4},				/* "���C�h4"	TEKI_YOUSEI3_1 */
	/* [A�d��]���d�� */
		{regist_NULL,								move_ao_yousei1},					/* "�d��1"	TEKI_YOUSEI1_1 */
	};
//012�p�~{regist_NULL/*(��/��]���p)*/, 			move_zako_06_inseki},				/* "���΂�1"	TEKI_YOUSEI2_5 */
//020�p�~{regist_zako_012_ao_yousei,				move_zako_12_ao_yousei},			/* "�d��2"	TEKI_YOUSEI1_1 */
	//	{regist_zako_014_karasu1,					move_zako_14_karasu},				/* ["�G1"�֋z��]"�d��3"	TEKI_YOUSEI1_1 */
	//	{regist_NULL,								move_ao_yousei1},					/* ["�d��1"�֋z��]"�d��4"	TEKI_YOUSEI1_1 */ //(r33�ł͖��g�p)
	OBJ *h;
	h = obj_regist_teki();/*(�U�R�G��obj��V�K�쐬����B)*/
	if (NULL != h)/* �o�^�ł����ꍇ�̂�(�o�^�Ɏ��s�����ꍇ�́A��ʓ��̃U�R�G������������̂ł���ȏ�o�^�ł��Ȃ��B) */
	{
		// �W�����ʓo�^�����B
		regist_settei_common(l, h);/* ���{�X�Ƌ��p����K�v������B */
		h->callback_hit_teki	= callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�����l�Ƃ��ĕW���̏�����ݒ肵�Ă����B */
		//
		h->callback_mover = aaa_resource[ (int)(l->user_255_code) ].aaa_move_callback;	/* ���ԃR�[�h�`���̃R�}���h�ԍ�����ړ����������߂�B */
		(aaa_resource[ (int)(l->user_255_code) ].aaa_init_callback)(l, h);				/* ���ԃR�[�h�`���̃R�}���h�ԍ�����e�֐��ɕ��򂷂�B */
	}
}
#include "chuu_boss_111_douchu.h"/*(�����̒��{�X)*/
#include "boss_000_douchu.h"/*(�����̃{�X)*/
