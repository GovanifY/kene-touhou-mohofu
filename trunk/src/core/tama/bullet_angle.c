
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�x�e
	-------------------------------------------------------
	-------------------------------------------------------
	��ݏo���`�F�b�N�ɂ��āA
	��ݏo���`�F�b�N�͖��t���[���s���K�v�����������ł���B
	�Ⴆ�� 8 �t���[���Ɉ��`�F�b�N���Ă��A
	���ۂɂ͖��t���[���`�F�b�N����̂Ɩw�Ǖς��Ȃ��B
	(�u�`�F�b�N���ׁv�������̂ɒe�����팸����Ȃ��B)
	�_��ȃ`�F�b�N�̈׎��O�ŁA�`�F�b�N��������ɂȂ��Ă���B
	�����炭�{�Ƃɂ����̎d�g�݂͂���B
	-------------------------------------------------------
	���u�`�F�b�N���ׁv���ۂɂقڂO���낤���A���鎖�͂���B
	�ړ�����Ƃ������́A�e�̍��W�����W�X�^�ɓ������ꍇ�Ȃ̂ŁA
	���̎��_�Ń`�F�b�N���s���Ƃ����l�����́A����͂���Ō����I�ł��邪�A
	�u�`�F�b�N�̕������l�X�ɕς��ꍇ�v��u���t���[���`�F�b�N���Ȃ��ꍇ�v��
	��ׂ�Ɣ�����ł���B
	(���W�X�^�Ƀ��[�h������� �� �A���S���Y���ŉ��P�������)
	�Ƃ������B
	����
---------------------------------------------------------*/


/*
	�p�x�e�ł́A�x�N�g���ړ������Ȃ��B
	����Ɋ�_���W�Ƃ��Ďg���B
*/
#define tx256				vx256/* ��_���Wx */
#define ty256				vy256/* ��_���Wy */

#define radius256			user_data01 	/* ���a */
#define speed256			user_data02 	/* ������ */
#define tra256				user_data03 	/* ���������� */
//#define mask				user_data04 	/* ��ʊO��������┽�ˋ@�\ */


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
/* �������Ȃ�(���������ړ�) */
static void move_bullet_vector_wide100(SPRITE *src)
{
	src->x256 += (src->tx256);/*fps_factor*/
	src->y256 += (src->ty256);/*fps_factor*/

//	move_bullet_vector_wide100(src);
//	gamen_gai_nara_osimai(src);/* ��ʊO�Ȃ炨���܂� */

	if ((src->x256 < t256(-100))||(src->x256 > t256(100)+t256(GAME_WIDTH))||
		(src->y256 < t256(-256))||(src->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{
		src->type = SP_DELETE;
	}
}
#endif

//	switch (src->BOSS02_ICE_DATA_state) case KS00:	/* �~��^�� */
	//	src->BOSS02_ICE_DATA_state = src->BOSS02_ICE_DATA_next_2nd_state;
	//	src->BOSS02_ICE_DATA_state++/* = KS01*/;

/*---------------------------------------------------------
	�p�x�e�̈ړ����s��
	��ݏo���`�F�b�N�@�\�͂Ȃ��̂ŕʂł�鎖
---------------------------------------------------------*/

static void move_bullet_angle01(SPRITE *src)
{
	src->speed256	+= (src->tra256);		/* ���������� */
	src->radius256	+= (src->speed256); 	/* ���x */
//
	src->x256 = (src->tx256) + ((sin1024((src->m_angleCCW1024))*(src->radius256))>>8);/*fps_factor*/
	src->y256 = (src->ty256) + ((cos1024((src->m_angleCCW1024))*(src->radius256))>>8);/*fps_factor*/
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
//	mask1024(src->m_angleCCW1024);

/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_change_meirin_yajirusi_one(SPRITE *h)
{
	/* ���݂̒e���W���A��_���W�ɂ���B */
	h->tx256 = h->x256;/*fps_factor*/
	h->ty256 = h->y256;/*fps_factor*/
	//
//	h->type 						= (BULLET_KUNAI12_00_AOI+1);/* �e���Ԓe�ɕϐg */
//	h->speed256 					= t256(0.5);		/* ���x */	/* ����(�ł��o�����x) */
//	h->speed256 					= t256(0.0)+((h->radius256)>>5);	/* ���a���傫�����A���������� */	/* ����(�ł��o�����x) */
	h->speed256 					= t256(0.0)+((h->radius256)>>6);	/* ���a���傫�����A���������� */	/* ����(�ł��o�����x) */
	h->radius256					= t256(0);/* ���a */
	h->tra256						= (1);		/* ���������e */
	const int ao_aka_tbl[(2)]	=
	{
	//	 (1024/2)+(1024/32),/* 180/360���傢��] */ 	/* �� */
	//	-(1024/2)-(1024/32),/* 180/360���傢��] */ 	/* �� */
		 (1024/2)+(1024/24),/* 180/360���傢��] */ 	/* �� */
		-(1024/2)-(1024/24),/* 180/360���傢��] */ 	/* �� */
	};
//	h->m_angleCCW1024				+= (1024/2)+(1024/16);/* 180/360���傢��] */
//	h->m_angleCCW1024				+= (1024/2)+(1024/32);/* 180/360���傢��] */
	h->m_angleCCW1024				+= ao_aka_tbl[((br.BULLET_REGIST_bullet_obj_type)&1)];/* 180/360���傢��] */
	mask1024(h->m_angleCCW1024);
}
global void exchange_damnaku_check_type(void)
{
	int check_type;
	check_type = br.BULLET_REGIST_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
		if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		{
			s_change_meirin_yajirusi_one(s);
		}
	}
}
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
		h									= sprite_add_only_bullet_error();
		if (NULL != h)
		{
			h->m_Hit256R					= TAMA_ATARI_JIPPOU32_PNG;
			h->type 						= (br.BULLET_REGIST_bullet_obj_type);
			h->speed256 					= (br.BULLET_REGIST_speed256);		/* ���x */	/* ����(�ł��o�����x) */
			h->callback_mover				= move_bullet_angle01;
			#if 1/*Gu(���S���W)*/
			h->tx256						= /*src*/obj_send1->x256;
			h->ty256						= /*src*/obj_send1->y256;
			#endif
//
//			h->base_time_out				= (120);/*(100)*/
			i_angle65536					+= (br.BULLET_REGIST_div_angle65536);	/*(32)*/	/* (1024-64)(30<<4) (1<<4) ICE_NU M_OF_ENEMIES */
			mask65536(i_angle65536);
			h->m_angleCCW1024				= (i_angle65536>>6);/* (i<<4) deg_360_to_512(90) */
//
			h->radius256					= t256(0);/* ���a */
			h->tra256						= (br.BULLET_REGIST_speed_offset);		/* ���������e */	/* ���̕����ɂȂ邩������ */
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
