
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���݂��J�[�h
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
		BOSS_DATA_05_move_jyumyou	user_data05 	���Ԃ�B�o�ߎ��ԁB
---------------------------------------------------------*/

// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()

#define MOMIJI_KODOMO_next_angle1024	tmp_angleCCW1024					/* �q���e�A�����p�x */


/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/extern OBJ_CALL_FUNC(add_zako_aya_5_momiji);
static OBJ_CALL_FUNC(move_aya_momiji_oya)
{
	src->BOSS_DATA_05_move_jyumyou--;/*fps_factor*/
	if ((0 > src->BOSS_DATA_05_move_jyumyou))
	{
		add_zako_aya_5_momiji(src); 	/* 5�������݂����e�𐶐� */
		src->jyumyou = JYUMYOU_NASI;
	}
	src->center.x256 += (src->math_vector.x256);/*fps_factor*/
	src->center.y256 += (src->math_vector.y256);/*fps_factor*/
//
	/* �e����ʊO�Ȃ番�􂵂Ȃ� */
	gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
}


/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ OBJ_CALL_FUNC(add_zako_aya_doll)
{
	int angle1024;
//	for (angle1024=(int)((1024)-(1024*2/24)); angle1024<(int)((1024)+(1024*3/24)); angle1024+=(int)(1024*1/24) )
	for (angle1024=(0); angle1024<(1024); angle1024+=(int)(1024*1/24) )
	{
		OBJ *h;
		h							= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->obj_type_set 		= (TEKI_32_20)+((angle1024>>7)&0x07);/* 0 ... 8 */
			/* �����蔻�薳�� */
			h->atari_hantei 		= (ATARI_HANTEI_OFF/*�X�R�A���p*/);
			h->callback_mover		= move_aya_momiji_oya;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->rotationCCW1024		= (angle1024);
	//
	//		h->base_score			= score(25*2);/*�_�~�[*/
			h->base_hp				= (9999);/*�|���Ȃ�*/
	//
			h->BOSS_DATA_05_move_jyumyou			= ((30+15)/*+1*/);
			{
				/* �����ʒu */
				h->center.x256			= src->center.x256;
				h->center.y256			= src->center.y256;
			}
			#if 1/*???*/
			h->MOMIJI_KODOMO_next_angle1024 	= (angle1024);
			#endif
	//		regist_vector(h, speed256, angle512);
		//	const int speed256 = t256(1.00);
		//	h->math_vector.x256 = ((si n1024((angle1024))*(speed256))>>8);	/*fps_factor*/
		//	h->math_vector.y256 = ((co s1024((angle1024))*(speed256))>>8);	/*fps_factor*/
			#if (0)//
			h->math_vector.x256 = ((si n1024((angle1024))));	/*fps_factor*/	/* CCW�̏ꍇ */
			h->math_vector.y256 = ((co s1024((angle1024))));	/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (angle1024), &sin_value_t256, &cos_value_t256);
				h->math_vector.x256 		= ((sin_value_t256));/*fps_factor*/
				h->math_vector.y256 		= ((cos_value_t256));/*fps_factor*/
			}
			#endif
		}
	}
}
