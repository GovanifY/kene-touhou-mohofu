
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̒����U���i�C�t�e
	-------------------------------------------------------
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
	-------------------------------------------------------
	Todo:
	src->giji_jyumyou ��p�~���� src->jyumyou �Œu��������B
---------------------------------------------------------*/
#if 000

//typedef struct
//{
#define aaa_target_x256 						user_data00
#define aaa_target_y256 						user_data01
#define BULLET_SP1_DATA_angle1024				user_data02
#define BULLET_YUUDOU_DATA_hiritu256			user_data03
//} BULLET_SP1_DATA;

#define giji_jyumyou base_time_out

#if 1

static void s_tmp_angleCCW1024_src_nerai_auto(SPRITE *src, SPRITE *dest)
{
	if (ANGLE_JIKI_NERAI_DAN==dest->tmp_angleCCW1024)
	{
		/*s_*/tmp_angleCCW1024_src_nerai(src, dest);
	}
}
//static int angle_jikinerai512_auto(SPRITE *p, SPRITE *t, int angle512);
static void s_tmp_angleCCW1024_src_nerai_auto(SPRITE *src, SPRITE *t/*, int angle512*/);
static void move_bullet_sakuya_linear_yuudou(SPRITE *src)
{
	src->giji_jyumyou--;
	if (0 > src->giji_jyumyou)				/* �����^�� */
	{
		/* �ړ����x t256(1.0) �œ����ړ� */
		src->vx256 = ((sin1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
		src->vy256 = ((cos1024((src->BULLET_SP1_DATA_angle1024)) ));/*fps_factor*/
	}
	else
	if (0+2048 > src->giji_jyumyou) 		/* �U���^�� */
	{
		const u8 m_tbl[4] =
		{
		//	0,1,3,7,	/* �x��������� */
			1,0,7,3,	/* �ق��̍U���Ƃ̌��ˍ����ŁA�^�C�~���O���Ƃ�Â炢������� */
		};
		/* �����ɏW�܂� */
	//	if (0==(src->giji_jyumyou & (7>>difficulty) ))
	//	if (0==(src->giji_jyumyou & (7>>(3-difficulty)) ))/* �x��������� */
		if (0==(src->giji_jyumyou & (m_tbl[cg_game_difficulty]) ))
		{
			src->BULLET_YUUDOU_DATA_hiritu256--;
		}
	//	if (240/*1*/ < data->y_sum256)
		{	/* ���� == (�e�̌��ݍ��W - �e�̗U�����W) */
			int x_sa256 = (src->cx256 - src->aaa_target_x256);
			int y_sa256 = (src->cy256 - src->aaa_target_y256);
			/* ������x�A�U�����W�̋߂��܂ł����� */
			if (t256(2.0) > abs(x_sa256))
			{
				if (t256(2.0) > abs(y_sa256))
				{
					src->giji_jyumyou = 0;	/* �����^���� */
				}
			}
			/* ���Z���� == (�e�̍������W * �U���䗦) */
			int aaa_x256 = ((x_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			int aaa_y256 = ((y_sa256 * src->BULLET_YUUDOU_DATA_hiritu256)>>8);	/*fps_factor*/
			src->cx256 = src->aaa_target_x256 + (aaa_x256); /*fps_factor*/
			src->cy256 = src->aaa_target_y256 + (aaa_y256); /*fps_factor*/
		}
	}
	else
	if (0+2048==src->giji_jyumyou)			/* �_���e�ɕϐg���� */
	{
		src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
		src->BULLET_SP1_DATA_angle1024 = ANGLE_JIKI_NERAI_DAN;
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		{
			src->tmp_angleCCW1024			= src->BULLET_SP1_DATA_angle1024;
			s_tmp_angleCCW1024_src_nerai_auto(zzz_player, src);
			src->BULLET_SP1_DATA_angle1024	= src->tmp_angleCCW1024;
		}
		mask1024(src->BULLET_SP1_DATA_angle1024);
		src->rotationCCW1024 = src->BULLET_SP1_DATA_angle1024;
		src->vx256 = (0);	/*fps_factor*/
		src->vy256 = (0);	/*fps_factor*/
		/* �U�����W�̓v���C���[�̈ʒu�t�� */
		/* �Ƃ肠�����A���Ή��B�����I�ɐݒ肵���Ⴄ */
		src->aaa_target_x256	= (zzz_player->cx256);	/*fps_factor*/	// +t256(32/2)/*���̔���*/
		src->aaa_target_y256	= (zzz_player->cy256);	/*fps_factor*/	// +t256(16/2)/*�����̔���*/
		src->BULLET_YUUDOU_DATA_hiritu256		= t256(1.0);	/* (2.5==5*0.5) */
	}
//
//	move_bullet_vector(src);
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
}
#endif

/*---------------------------------------------------------
	���̒����U���i�C�t
	-------------------------------------------------------
	���̒e�͑��̒e�������̂ł����Ēx������B
---------------------------------------------------------*/

global void bullet_crate_sakuya_linear_yuudou(SPRITE *src)
{
	const unsigned int knife_color = BULLET_KNIFE20_04_AOI+((ra_nd()&(4-1)));	/* �i�C�t�̐F�̓����_�� */
	unsigned int j;
	for (j=(0); j<(8); j++)
	{
		SPRITE *h;
		h						= obj_add_00_tama_error();
		if (NULL != h)
		{
			h->cx256			= src->cx256;
			h->cy256			= src->cy256;
			h->type 			= (knife_color);/*S P_BULLET*/ /*SP_LASER*/
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_MARU16_PNG;
		//
			h->callback_mover	= move_bullet_sakuya_linear_yuudou;
			h->giji_jyumyou = 2048+20+(40+1);		/*data->timer		= 0;*/

			int angle1024		= src->tmp_angleCCW1024+((j-4)*84); 	/* 85.333 == cv1024r(30) */
			mask1024(angle1024);
			h->rotationCCW1024 = angle1024;
			h->vx256 = ((sin1024((angle1024))*t256(1.0))>>8);/*fps_factor*/
			h->vy256 = ((cos1024((angle1024))*t256(1.0))>>8);/*fps_factor*/

			h->BULLET_YUUDOU_DATA_hiritu256 	= t256(1.0);	/* (2.5==5*0.5) */
		//
			#if 1
			/* �Ƃ肠�����A��{�� */
		//	h->callback_2nd_regist	= callback_2nd_regist;
		//
			/* �Ƃ肠�����A�e�L�g�[ */
		//	h->BULLET_DATA_2nd_count	= (999/*30*/);		/* 30�J�E���g�����甼���� */
		//	h->BASE_add_speed256		= (t256(1.00)); 	/* ����/�����A�Ȃ� */
			#endif
		}
	}
}

#endif/* 000 */
