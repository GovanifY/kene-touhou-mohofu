
#if (1)
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̂��邭��i�C�t�e(����)
	���
	�v���C���[�̎�������邭�����Ă���A�����_���̃i�C�t�B
	���΂ɐF���ς��B
	-------------------------------------------------------
	�S�����i�C�t(��)
	�_���i�C�t(��)
	-------------------------------------------------------
	�v���C���[�̎���ɎU��΂�A���Ԃ��o�ƃv���C���[�_���e
	-------------------------------------------------------
	�e��S�e�������������Ɉڍs����̂ł����͖����Ȃ�\��B
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
---------------------------------------------------------*/

// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()
#define HATUDAN_TIME256 	(256)

static void bullet_move_kurukuru_knife(SPRITE *src)
{
	if (HATUDAN_TIME256 > src->jyumyou)
	{
		;
	}
	else
	if (HATUDAN_TIME256 == src->jyumyou)
	{
		/* �ϐg����B */
		{
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
			reflect_sprite_spec444(src, OBJ_BANK_SIZE_00_TAMA);
			#if 1/* �킴�Ƒ_��Ȃ� */
			u32 ra_nd32 = ra_nd();
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		//	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			#endif
			REG_00_SRC_X	= (zzz_player->cx256)+((ra_nd32<<8)&0x1fff)-t256(16);
			REG_01_SRC_Y	= (zzz_player->cy256)+((ra_nd32  )&0x0fff)-t256(8);
			REG_02_DEST_X	= ((src->cx256));
			REG_03_DEST_Y	= ((src->cy256));
			tmp_angleCCW65536_src_nerai();
			src->tmp_angleCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));		/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
			mask1024(src->tmp_angleCCW1024);/* �˂�̂��� */
			src->rotationCCW1024 = ((src->tmp_angleCCW1024));	/* �\���p�x */
		}
		#if (0)//
		src->vx256 = ((si n1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* �΃i�C�t��2nd�_�����x */ /* ���x */	/* CCW�̏ꍇ */
		src->vy256 = ((co s1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* �΃i�C�t��2nd�_�����x */ /* ���x */
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
			src->vx256 = ((sin_value_t256 )<<1);/*fps_factor*/
			src->vy256 = ((cos_value_t256 )<<1);/*fps_factor*/
		}
		#endif
	}
	else
	{
		/* ���邭�邳���� */
		src->rotationCCW1024 += (64);
		mask1024(src->rotationCCW1024);/* �˂�̂��� */
	}
	src->cx256 += (src->vx256); 	/*fps_factor*/
	src->cy256 += (src->vy256); 	/*fps_factor*/
}

/*
	-------------------------------------------------------
	ToDo:
	�e�V�X�e���Ɉڍs����ׁA�p�~�\��B
	�J�[�h�X�N���v�g�V�X�e�����ғ�����ׂɂ́A���e���ꃖ���ɓ������Ȃ���
	���x�I�Ȗʂł��̂܂܂̌`�Ԃł͖���������̂ŁA
	(�����̔��e�����̂����ő��̒e��������������)
	�����̔��e������ hatudan_system_regist_single �ɒu��������K�v������B
---------------------------------------------------------*/

global void bullet_crate_sakuya_kurukuru(SPRITE *src)
{
	SPRITE *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	const int tmp_x256		= zzz_player->cx256;
	const int tmp_y256		= zzz_player->cy256;
	/* ������x�����ɏo�Ȃ��ƔF�����Â炢 */
	/* �L����(�R�A)���ł����Ɣ�����O�Ɏ��ʁB(�i�C�t���L����O�ɂ�����) */
	const int tmp_step[4]	=
	{
		(65536/8),
		(65536/10),
		(65536/12),
		(65536/16),
	};
	int ii;
	for (ii=0; ii<65536; ii += tmp_step[(REG_0f_GAME_DIFFICULTY)] ) 	/* �v���C���[�̎����8�e�z�u���� */
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();
		if (NULL != h)
		{
			#if (0)//
			h->cx256 = (tmp_x256) + ((si n1024((deg65536to1024(ii))))<<7);/*fps_factor*/	/* CCW�̏ꍇ */
			h->cy256 = (tmp_y256) + ((co s1024((deg65536to1024(ii))))<<7);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(ii)), &sin_value_t256, &cos_value_t256); 	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
				h->cx256 = (tmp_x256) + ((sin_value_t256)<<7);/*fps_factor*/
				h->cy256 = (tmp_y256) + ((cos_value_t256)<<7);/*fps_factor*/
			}
			#endif
			h->type 			= BULLET_KNIFE20_04_AOI;	/* (���΃i�C�t�ɕϐg) */
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
		//
			h->callback_mover	= bullet_move_kurukuru_knife;
			h->jyumyou			= (HATUDAN_TIME256+64);
		//
			h->vx256			= (0);
			h->vy256			= (0);
		}
	}
}
#endif
