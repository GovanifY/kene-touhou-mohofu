
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�x�e
	-------------------------------------------------------
	-------------------------------------------------------
	��ݏo���`�F�b�N�ɂ��āA
	�u�ړ����Ɂv��ݏo���`�F�b�N�����Ȃ������O��Ő݌v����Ă���e���������B
	���ׁ̈u�ړ����Ɂv�V�X�e���ŋ�ݏo���`�F�b�N�����܂���B
	�Ⴆ�Ή�ʒ[�Œ��˕Ԃ�e���́u�ړ����Ɂv��ݏo���`�F�b�N�����Ȃ����ɂ��������Ă���@�\�ł��B
	�u�ړ��ƕʂŁv�e�����X�g�𒲂ׂāu��ݏo���`�F�b�N�v���s���܂��B
---------------------------------------------------------*/

/*
	�p�x�e�ł́A�x�N�g���ړ������Ȃ��B
	����Ɋ�_���W�Ƃ��Ďg���B
*/
#if 1/* �p�x�e�K�i(�����) */
	#define tx256				vx256/* ��_���Wx */
	#define ty256				vy256/* ��_���Wy */
	//
	#define radius256			user_data01 	/* ���a */
//	#define speed256			user_data02 	/* ������ */
	#define speed65536			user_data02 	/* ������ */
	#define tra65536			user_data03 	/* ���������� */
	#define rotate1024			user_data04 	/* ��]�p�x���� */
	#define bullet_status_mask	user_data05 	/* ��ʊO��������┽�ˋ@�\ */
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
/* �������Ȃ�(���������ړ�) */
static void move_bullet_vector_wide100(SPRITE *src)
{
	src->cx256 += (src->tx256);/*fps_factor*/
	src->cy256 += (src->ty256);/*fps_factor*/
}
#endif


/*---------------------------------------------------------
	�p�x�e�̈ړ����s��
	��ݏo���`�F�b�N�@�\�͂Ȃ��̂ŕʂł�鎖
---------------------------------------------------------*/

static void move_bullet_angle01(SPRITE *src)
{
	#if 1/* ������(�����Ă��o���邯�ǁA�����������ȒP) */
	src->speed65536 		+= (src->tra65536); 	/* ���������� */
	#endif
//	src->radius256			+= (src->speed256); 	/* ���x */
	src->radius256			+= (src->speed65536>>8);	/* ���x */
	#if 0/* ������(�����Ă��o���邯�ǁA�����������ȒP) */
	/* �p�`�F() �Ƃ� ���[�~�A(�����e) �Ƃ��ɕK�v(�����Ă��o���邯�ǁA�����������ȒP) */
	/* �����e�̏ꍇ���̕������ƃO��������B(?) */
	src->rotationCCW1024	+= (src->rotate1024);	/* ��]�p�x���� */
	#endif
//
	src->cx256 = (src->tx256) + ((sin1024((src->rotationCCW1024))*(src->radius256))>>8);/*fps_factor*/
	src->cy256 = (src->ty256) + ((cos1024((src->rotationCCW1024))*(src->radius256))>>8);/*fps_factor*/
}

//	move_bullet_vector_wide100(src);
	#if 0/* ������ */
	check_bullet_angle01(src);
	#endif

//	src->base_time_out--;/*fps_factor*/
//	if (src->base_time_out < 0)
//	{
//		src->callback_mover 				= move_bullet_vector_wide100;
//	}
//	/* �\���p */


//
//	mask1024(src->rotationCCW1024);



/*---------------------------------------------------------
	�V��p
	ICE�e
	-------------------------------------------------------
---------------------------------------------------------*/
/*BULLET_CAP16_04_KOME_SIROI*/	/*S P_BULLET*/ /*S P_BO SS02ICE*/
static void s_bullet_regist_multi_angle(void)
{
	int i_angle65536;
	i_angle65536	= (br.BULLET_REGIST_angle65536);/* �J�n���� */
//
	int i;
	for (i=(0); i<(br.BULLET_REGIST_n_way); i++)
	{
		SPRITE		*h;
		h									= sprite_add_444only_bullet_error();
		if (NULL != h)
		{
			h->type 						= (br.BULLET_REGIST_bullet_obj_type);
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R					= TAMA_ATARI_JIPPOU32_PNG;
		//
		//	h->speed256 					= ((br.BULLET_REGIST_speed256)	 ); 	/* ���x */	/* ����(�ł��o�����x) */
			h->speed65536					= ((br.BULLET_REGIST_speed256)<<8); 	/* ���x */	/* ����(�ł��o�����x) */
			h->callback_mover				= move_bullet_angle01;
			#if 1/*Gu(���S���W)*/
			h->tx256						= /*src*/obj_send1->cx256;
			h->ty256						= /*src*/obj_send1->cy256;
			#endif
//
//			h->base_time_out				= (120);/*(100)*/
			i_angle65536					+= (br.BULLET_REGIST_div_angle65536);	/*(32)*/	/* (1024-64)(30<<4) (1<<4) ICE_NU M_OF_ENEMIES */
			mask65536(i_angle65536);
			h->rotationCCW1024				= (i_angle65536>>6);/* (i<<4) deg_360_to_512(90) */
//
			h->radius256					= t256(0);/* ���a */
//			h->tra65536 					= ((br.BU LLET_REGIST_speed_offset)<<8);	/* ���������e */	/* ���̕����ɂȂ邩������ */
			h->tra65536 					= ((br.BULLET_REGIST_speed_offset));	/* ���������e */	/* ���̕����ɂȂ邩������ */
			h->rotate1024					= (0);		/* ���̕����ɂȂ邩������ */
			h->bullet_status_mask			= (0);		/* ���̕����ɂȂ邩������ */
		}
	}
}


/*---------------------------------------------------------
	�e��o�^����B
	-------------------------------------------------------
	�֐���(�N���X���ۂ�)�B���B
	�e�������̃G�t�F�N�g���̊֌W��A���e�����͑S���Z�߂�K�v������B
	-------------------------------------------------------
	�x�N�g���e�Ɗp�x�e�𓝍����邩������Ȃ��B
---------------------------------------------------------*/

global void bullet_regist_angle(void)
{
	s_bullet_regist_multi_angle();
}
