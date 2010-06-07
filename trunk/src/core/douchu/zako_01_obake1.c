
#include "douchu.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"���΂�1",		"DRAGER",
	-------------------------------------------------------
	�ˌ����΂��A(���Ȃ肤��o���A���΂�����Ȃ����������H)
	-------------------------------------------------------
�d�l�F
	�� dat��score���w�肷��B
	�� dat�ő̗͂��w�肷��B
	�� dat�Ŋl���A�C�e�����w�肷��B
	�� dat�ŏo���ʒu x, y ���W���w�肷��B
	�� �G���̑��x�͓�Փx�Ɋ֌W�Ȃ��Œ�B���\���߁B(t256(2.0))
	�� �G���͎��@�_���œˌ����Ă���B(�r���Ō����͕ς��܂���)
	�� ���Ԋu�Œe�������Ă���B(�����|���������L��)
		easy	�����Ȃ��B
		normal	1way�e�B
		hard	3way�e�B
		luna	5way�e�B
	�� �e�̑��x�́A�G���̑��x���x���B���x�͓�Փx�Ɋ֌W�Ȃ��Œ�B(t256(1.25))
	(��ʂɎc��̂ő����|���������L��)
	-------------------------------------------------------
	�܂�t�����[�E�C�U���E�F�C�̋t�o�[�W�����ł��B
	�ƌ���������������₷�����ȁH
---------------------------------------------------------*/

/*---------------------------------------------------------
	"覐�1", "�� �� ��1",		"MING",
	-------------------------------------------------------
�d�l�F
	�� dat��score���w�肷��B
	�� dat�ő̗͂��w�肷��B
	�� dat�Ŋl���A�C�e�����w�肷��B
	�� dat�ŏo���ʒu x, y ���W���w�肷��B



	�P�̂ł΂�΂�ɁA�����Ă���U�R�G
	���@�_���ԒP�e��A�˂��Ă���
	-------------------------------------------------------
	�����ɕK�v���Y�񂾂��A���Ǝ����d�l�̕ґ���s�G�͕K�v�Ȃ��B
	�P���ɗ����Ă���G�ɕύX����B
	-------------------------------------------------------
	10�@�O�p����ɑ�����A�ґ���s���Ă���U�R�G
	ming
	rwingx_curver�Ɏ��Ă�
---------------------------------------------------------*/
#if 0/* �߂� */
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif

/* �G�̌��� tmp_angleCCW1024  */

#define time_out			user_data03 	/* �o�ߎ��� */
#define gra_anime_type		user_data04 	/* �O���^�C�v */


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_obake1(SPRITE *src)
{
	src->time_out--;
	if (( 0 > src->time_out ))
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	if (0==((src->time_out)&0x3f))/* ���Ԋu(�e�L�g�|) */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->y256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			if (0<difficulty)
			{
				obj_send1->x256 = src->x256;
				obj_send1->y256 = src->y256;
				br.BULLET_REGIST_speed256			= (t256(1.25));
				br.BULLET_REGIST_angle1024			= (src->tmp_angleCCW1024);		/*(ANGLE_JIKI_NERAI_DAN)*/
				br.BULLET_REGIST_div_angle1024		= (int)(1024/(18)); 			/* �����p�x */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
				br.BULLET_REGIST_n_way				= (difficulty+difficulty-1);	/* [1 3 5way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
	}
//
	{
		src->x256 += (src->vx256);/*fps_factor*/
		src->y256 += (src->vy256);/*fps_factor*/
	}
	if (SP_DELETE != src->type)
	{
		if (0==src->gra_anime_type)
		{
			/* offset�����͉�] */
			src->m_angleCCW1024 += (10);/*�O����]*/
			mask1024(src->m_angleCCW1024);
		}
		else
		{
			src->type			= TEKI_28_YOUSEI2_5+(((src->y256>>8)&0x03));
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_obake1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_28_YOUSEI2_5;
		h->callback_mover		= move_obake1;
		h->time_out 			= (60);
	//
		tmp_angleCCW1024_jikinerai(obj_player, h);
		h->vx256 = ((sin1024((h->tmp_angleCCW1024))));
		h->vy256 = ((cos1024((h->tmp_angleCCW1024))));
		h->vx256 += h->vx256;
		h->vy256 += h->vy256;
		h->gra_anime_type		= (1);	/* 0==��]�A�j���A�O���^�C�v */
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_inseki1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_61_NIJI_HOSI;
		h->callback_mover		= move_obake1;
		h->time_out 			= (0x3ff);
	//
		h->gra_anime_type		= (0);	/* 0==��]�A�j���A�O���^�C�v */
		{
			/* �����ʒu */
			h->x256 = (((u32)l->user_x)<<8);//((rrrr)&0xffff) + t256((GAME_WIDTH-256)/2);
			h->y256 = -t256(30);
		//
		u32 rrrr;
			rrrr = (ra_nd());
			h->vx256 = ((rrrr)&0x01ff);/*�E����*/
			/* ��ʉE���̏ꍇ�A�������֐i�� */
			if ( t256((GAME_WIDTH)/2) < h->x256)
			{
				h->vx256 = (-(h->vx256));/*������*/
			}
			h->vy256 = ((rrrr>>16)&0x01ff) + t256(1.00);/*������*/
		}
	}
}
