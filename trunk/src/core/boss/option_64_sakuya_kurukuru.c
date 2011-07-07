
#include "game_main.h"

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
	�e���������������Ɉڍs����̂ł����͖����Ȃ�\��B
---------------------------------------------------------*/

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
		{
			src->type			= BULLET_KNIFE20_07_MIDORI;/* (���΃i�C�t�ɕϐg) */
			#if 1/* �킴�Ƒ_��Ȃ� */
			u32 ra_nd32 = ra_nd();
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			obj_send1->cx256	= (zzz_player->cx256)+((ra_nd32<<8)&0x1fff)-t256(16);
			obj_send1->cy256	= (zzz_player->cy256)+((ra_nd32  )&0x0fff)-t256(8);
			#endif
			tmp_angleCCW1024_src_nerai(obj_send1, src);
			mask1024(src->tmp_angleCCW1024);/* �˂�̂��� */
			src->rotationCCW1024 = ((src->tmp_angleCCW1024));	/* �\���p�x */
		}
		src->vx256 = ((sin1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* �΃i�C�t��2nd�_�����x */ /* ���x */
		src->vy256 = ((cos1024((src->tmp_angleCCW1024)) )<<1);/*fps_factor*/	/* �΃i�C�t��2nd�_�����x */ /* ���x */
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
	for (ii=0; ii<65536; ii += tmp_step[(cg_game_difficulty)] ) 	/* �v���C���[�̎����8�e�z�u���� */
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();
		if (NULL != h)
		{
			h->cx256			= tmp_x256 + ((sin1024((ii>>6)))<<7);
			h->cy256			= tmp_y256 + ((cos1024((ii>>6)))<<7);
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
