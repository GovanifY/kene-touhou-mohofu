
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�J�[�h
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
		BOSS_DATA_05_move_jyumyou	user_data05 	���Ԃ�B�o�ߎ��ԁB
---------------------------------------------------------*/

#define MOMIJI_KODOMO_DATA_angle1024			user_data00 	/*union BULLET_ANGLE_DATA*/
#define MOMIJI_KODOMO_DATA_speed256 			user_data01 	/*union BULLET_ANGLE_DATA*/
//
// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()

//
#define MOMIJI_KODOMO_DATA_angle222_1024		user_data03
#define MOMIJI_KODOMO_DATA_adjust_speed256		user_data07


/*---------------------------------------------------------
	����(last)
---------------------------------------------------------*/

static void move_aya_doll_last_burrets(SPRITE *src)
{
//	/*[r35�v������]*/HATSUDAN_01_speed256	= (t256(0.75)+(ra_nd()&0xff));					/* �e�� */
	/*[r35������]*/HATSUDAN_01_speed256 	= (t256(0.375)+(ra_nd()&0xff)); 				/* �e�� */
	HATSUDAN_02_speed_offset				= (2);/*(�Ƃ肠����)*/
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	HATSUDAN_03_angle65536					= ((ra_nd()&(65536-1)));						/* src->tmp_angleCCW1024 */
	HATSUDAN_05_bullet_obj_type 			= (BULLET_KOME_BASE + TAMA_IRO_01_AKA) + (((REG_0f_GAME_DIFFICULTY)&0x02)<<1);	/* [��(0x01)/��(0x05)�Ēe] */
	HATSUDAN_06_n_way						= (1+(REG_0f_GAME_DIFFICULTY)); 							/* [7way] [8way] */
	HATSUDAN_07_div_angle65536				= (int)(65536/23);								/* �p�x (65536/27) (65536/24) */
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	hatudan_system_regist_n_way();/* (r33-) */
//	bullet_regist_multi_vector_direct();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_momiji(SPRITE *src)
{
	src->BOSS_DATA_05_move_jyumyou--;
	if ((0 > src->BOSS_DATA_05_move_jyumyou))
	{
		move_aya_doll_last_burrets(src);
		src->jyumyou = JYUMYOU_NASI;	/* �����܂� */
	}
	else
	{
		if (src->MOMIJI_KODOMO_DATA_speed256 < t256(0.5) )
		{
			src->MOMIJI_KODOMO_DATA_angle222_1024 += (1024/2);	/* (1024/2) == ���� == [180/360 �x] */
			#if 1
			/* �˂�̂��� */
			mask1024(src->MOMIJI_KODOMO_DATA_angle222_1024);
			#endif
			src->MOMIJI_KODOMO_DATA_angle1024	= src->MOMIJI_KODOMO_DATA_angle222_1024;
			src->MOMIJI_KODOMO_DATA_adjust_speed256 = (-(src->MOMIJI_KODOMO_DATA_adjust_speed256));
		}
		src->MOMIJI_KODOMO_DATA_speed256 += src->MOMIJI_KODOMO_DATA_adjust_speed256;
		if (src->MOMIJI_KODOMO_DATA_angle1024-src->MOMIJI_KODOMO_DATA_angle222_1024 < (int)(1024*1/12) )
		{
			src->MOMIJI_KODOMO_DATA_angle1024 += (1+1);/*0.01*/
		}
		#if 1
		/* �˂�̂��� */
		mask1024(src->MOMIJI_KODOMO_DATA_angle1024);
		#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		#if (0)//
		src->vx256			= ((si n1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
		src->vx256			= ((si n1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		src->vy256			= ((co s1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		src->vy256			= ((co s1024((src->MOMIJI_KODOMO_DATA_angle1024))*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->MOMIJI_KODOMO_DATA_angle1024), &sin_value_t256, &cos_value_t256);
			src->vx256			= ((sin_value_t256*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
			src->vy256			= ((cos_value_t256*(src->MOMIJI_KODOMO_DATA_speed256))>>8);/*fps_factor*/
		}
		#endif
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/

	}
}

/*---------------------------------------------------------
	5�������݂����e�𐶐�
---------------------------------------------------------*/
#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024	/* �q���e�A�����p�x */

/*static*/ void add_zako_aya_5_momiji(SPRITE *src)
{
	int angle1024;
	angle1024 = (src->MOMIJI_KODOMO_next_angle1024)-((int)(1024*1/20));
//
	int jj;
	for (jj=(0); jj<(5); jj++)
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= (TEKI_32_20)+((angle1024>>7)&0x07);/* 0 ... 8 */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_bullet_momiji;
			h->BOSS_DATA_05_move_jyumyou			= (50); 	/* 200���� */
			{
				/* �����ʒu */
				h->cx256			= src->cx256;
				h->cy256			= src->cy256;
			}
		//
			h->MOMIJI_KODOMO_DATA_angle222_1024 	= (angle1024);
			h->MOMIJI_KODOMO_DATA_angle1024 		= (angle1024);
			h->MOMIJI_KODOMO_DATA_speed256			= t256(3.0);
			h->MOMIJI_KODOMO_DATA_adjust_speed256	= t256(-0.04);
		//
			angle1024 += (int)(1024*1/20);/*offset_angle*/
			mask1024(angle1024);
		}
	}
}
