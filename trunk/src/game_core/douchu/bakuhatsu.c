
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����쐬
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����G�t�F�N�g�̕\��
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_bakuhatsu)
{
	/* ���������邤���͓������B(�����o�߂̓V�X�e����������) */
	if (0 < src->jyumyou)
	{
		src->center.x256 += src->math_vector.x256;
		src->center.y256 += src->math_vector.y256;
	}
}

/*---------------------------------------------------------
	��ޕʂɔ�����ǉ�����
Todo:
	���v���C�Ɋ֌W�̖���������ra_nd()���g�p���Ȃ��ŁA
	vfpu_ra_nd()���g�p����
---------------------------------------------------------*/
/*int x256, int y256*/
/*int delay_wait,*/
global void bakuhatsu_add_type_ccc(int type)
{
	{
		OBJ *h;
		h						= obj_regist_teki();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R		= TAMA_ATARI_JIPPOU32_PNG;/*????*/
			h->jyumyou			= (30);/*30 �t���[��*/

		//	/* �����蔻���OFF(���G) */
			h->atari_hantei 		= (ATARI_HANTEI_OFF/*�X�R�A���p*/);
			#if 1
			h->center.x256			= REG_02_DEST_X;/* ���e�ʒu ���Wx */
			h->center.y256			= REG_03_DEST_Y;/* ���e�ʒu ���Wy */
			#endif
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x66);	/*	s->alpha			= 0x80;*/
			//
		//	if (BAKUHATSU_MINI00==type) 	{type = (/*vfpu_*/ra_nd()%3+1);}/* ���������ɂ���ƒx�� */
			if (0==(type&0xfb))
			{
				type |= (/*vfpu_*/ra_nd()&0x03); if (0==(type & 0x03))	{type |= (0x01);}
			}
			if (12==(type & 0xff))
			{
				h->math_vector.x256 	= (/*vfpu_*/ra_nd()&0x07ff)-(0x03ff);
				h->math_vector.y256 	= (/*vfpu_*/ra_nd()&0x07ff)-(0x03ff);
			}
			else
			{
				h->math_vector.x256 	= (0);
				h->math_vector.y256 	= (0);
			}
			h->obj_type_set 			= SP_FRONT_YUKI;
			h->callback_mover			= move_bakuhatsu;
		}
	}
}


/*---------------------------------------------------------
	�~��̈�ɍL���锚��
---------------------------------------------------------*/

global void bakuhatsu_add_circle(OBJ/**/ *src, int mode)
{
	int i;
	int j;
	j=0;
	for (i=0; i</*64*/(64)/*25*/; i+=8)
	{
		j += (ra_nd()&(/*64*/512-1));
		#if (0)//
		REG_02_DEST_X = (src->center.x256) + ((si n1024((j))*(i)));/*fps_factor*/ /* CCW�̏ꍇ */
		REG_03_DEST_Y = (src->center.y256) + ((co s1024((j))*(i)));/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (j), &sin_value_t256, &cos_value_t256);
			REG_02_DEST_X = (src->center.x256) + ((sin_value_t256*(i)));/*fps_factor*/
			REG_03_DEST_Y = (src->center.y256) + ((cos_value_t256*(i)));/*fps_factor*/
		}
		#endif
//
		//	REG_02_DEST_X;/* ���e�ʒu ���Wx */
		//	REG_03_DEST_Y;/* ���e�ʒu ���Wy */
		bakuhatsu_add_type_ccc(
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((1==mode)?(ra_nd()&((64-1)<<8)):(0)));	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0!=mode)?(ra_nd()&((64-1)<<8)):(0)));	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
			BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0==mode)?(0):(ra_nd()&((64-1)<<8))));	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	}
}
