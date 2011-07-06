
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e��
	-------------------------------------------------------
---------------------------------------------------------*/

#define MOMIJI_KODOMO_DATA_angle1024			user_data00 	/*union BULLET_ANGLE_DATA*/
#define MOMIJI_KODOMO_DATA_speed256 			user_data01 	/*union BULLET_ANGLE_DATA*/
//
#define time_out								user_data03 	/* �o�ߎ��� */
//
#define MOMIJI_KODOMO_DATA_angle222_1024		user_data04
#define MOMIJI_KODOMO_DATA_adjust_speed256		user_data05


/*---------------------------------------------------------
	����(last)
---------------------------------------------------------*/

static void move_aya_doll_last_burrets(SPRITE *src)
{
	obj_send1->cx256 					= (src->cx256);		/* �e��x256 */
	obj_send1->cy256 					= (src->cy256);		/* �e��y256 */
	br.BULLET_REGIST_speed256			= (t256(0.75)+(ra_nd()&0xff));					/* �e�� */
	br.BULLET_REGIST_angle1024			= ((ra_nd()&(1024-1))); 						/* src->tmp_angleCCW1024 */
	br.BULLET_REGIST_div_angle1024		= (int)(1024/23);								/* �p�x (1024/27) (1024/24) */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AKA+((difficulty&0x02)<<1);	/* [��(0x01)/��(0x05)�Ēe] */
	br.BULLET_REGIST_n_way				= (1+difficulty);								/* [7way] [8way] */
	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	bullet_regist_vector();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_bullet_momiji(SPRITE *src)
{
	src->time_out--;
	if ((0 > src->time_out))
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
			/*�˂�̂���*/
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
		/*�˂�̂���*/
		mask1024(src->MOMIJI_KODOMO_DATA_angle1024);
		#endif
	//	mono_angle_move(src,(BULLET_ANGLE_DATA *)data);
		src->vx256 = ((sin1024((src->MOMIJI_KODOMO_DATA_angle1024))*src->MOMIJI_KODOMO_DATA_speed256)>>8);/*fps_factor*/
		src->vy256 = ((cos1024((src->MOMIJI_KODOMO_DATA_angle1024))*src->MOMIJI_KODOMO_DATA_speed256)>>8);/*fps_factor*/
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
	for (jj=(0); jj<(5); jj++ )
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
		//	h->type 				= /*BULLET_MINI8_00_AKA*/BULLET_MARU8_00_AKA+((angle1024>>7)&7); /*BULLET_MINI8_00_AKA*/ /*S P_BULLET*/
			h->type 				= TEKI_24_YOUSEI2_1+((angle1024>>7)&0x07);/* 0 ... 8 */
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_bullet_momiji;
			h->time_out 			= (50); 	/* 200���� */
			{
				/* �����ʒu */
				h->cx256 			= src->cx256;
				h->cy256 			= src->cy256;
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
