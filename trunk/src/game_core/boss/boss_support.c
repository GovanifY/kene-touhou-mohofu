
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�{�X�̈ړ��B
	�{�X�̃A�N�V������̏����B
	-------------------------------------------------------
	�G���̃A�N�V������̏����B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/

//extern OBJ *obj_effect[8]; /* [��]�i�J�[�h�̃O���t�B�b�N�j */
		#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
	/* ���������G���� */
	bakuhatsu_add_type(src, BAKUHATSU_MINI00);
	cg.teki->jyumyou	= (0);/* ??? */
	set_REG_DEST_XY(src);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_FIRE08);
		#endif
/*---------------------------------------------------------
	�G�ړ�(�U�R���Ŕ����G�t�F�N�g)
---------------------------------------------------------*/
//	/* �U�R���Ŕ����G�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
//	set_REG_DEST_XY(src);/* ���e�ʒu ���Wxy */
//	bakuhatsu_add_type_ccc(BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/* �U�R���Ŕ��� */

static OBJ_CALL_FUNC(move_zako_yarare)
{
	/* �`��p�O����] */
	src->rotationCCW1024	= (src->jyumyou<<6);
	/* �O���g�� */
	src->m_zoom.x256		= (t256(4.0))-(src->jyumyou<<3);
	src->m_zoom.y256		= (t256(4.0))-(src->jyumyou<<3);
	/* ������ */
	src->color32 -= 0x08000000; /* 8==256/32 */
}
static OBJ_CALL_FUNC(init_zako_yarare)
{	// �U�R���Ŕ����G�t�F�N�g��o�^
	src->callback_mover 	= move_zako_yarare;
	src->callback_hit_teki	= NULL;
	src->jyumyou			= (32);/* �G�����ꎞ�ԁB */
	src->atari_hantei		= (ATARI_HANTEI_OFF);/*(�U�R���Ŕ����G�t�F�N�g�ɂ͓����蔻�薳��)*/
	static int bbb = 0;
	const u32 aaa[4] =
	{
		MAKE32RGBA(0xff, 0x00, 0x00, 0xff), // ��
		MAKE32RGBA(0x00, 0xff, 0x00, 0xff), // ��
		MAKE32RGBA(0x00, 0x00, 0xff, 0xff), // ��
		MAKE32RGBA(0xff, 0xff, 0x00, 0xff), // ��
	};
	bbb++;
	bbb &= (4-1);
	src->color32			= aaa[bbb];
}
/*---------------------------------------------------------
	�U�R���U�����āA�U�R�Ɏ��e�����������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
	�U�R�͓����U�R�ȊO�ɁA�{�X����������g�������܂ށB
	-------------------------------------------------------
	OBJ *src;	�U�R�G����
	OBJ *tama;	���e
---------------------------------------------------------*/

/* �g���������邩��global */
global void callback_hit_zako(OBJ/**/ *src, OBJ/**/ *tama)
{
	/* �G���Ɏ��e�����������ꍇ�̉ΉԃG�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
	set_REG_DEST_XY(tama);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
//
	/* ��ƕ������������R�[�h������������B */
	{
		src->base_hp -= tama->kougeki_ti; 	/* �U�����đ̗͌��炷(����������) */
		if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
		{
			/* (�U�R��)�J�X�^�����ꏈ�� */
			if (NULL != src->callback_loser)	/* �J�X�^�����ꏈ��������Ύ��s�B */
			{
				(src->callback_loser)(src); 	/* �J�X�^�����ꏈ�������s����B(�W���́u�����_���e�[�u������A�C�e�������v) */
				src->callback_loser = NULL; 	/* ���s�����̂�(�O�̈�)�����B */
				/* (���ꏈ����1�x�������s���Ȃ�) */
			}
			/* �U�R�A�X�R�A���Z���� */
			bonus_info_any_score_nodel(src, ((src->base_score)*(cg.game_now_stage)) );
		//	/* ���������֎d�l�ύX */src->jyumyou = JYUMYOU_NASI;/* �X�v���C�g�V�X�e���ɍė��p�����肢����B */
			init_zako_yarare(src);	// �U�R�����Ŕ����G�t�F�N�g�ɂ���B
			voice_play_menu(VOICE08_DEATH, TRACK05_ZAKO_DEATH);/* �G����������炷�B */
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static int ee_angle1024;
static int ff_angle1024;
/*static*/ OBJ_CALL_FUNC(move_card_square_effect)
{
	ee_angle1024 += (4);	/* 4 ��]���x */
	ff_angle1024 += (2);	/* 2 �g��k�����x */
	int ww_angle1024;	ww_angle1024 = ee_angle1024;
	int vv_angle1024;	vv_angle1024 = ff_angle1024;
	int radius;
	int i;
	for (i=0; i<5; i++)
	{
		ww_angle1024 += 20; 	/* 20 ��]���x */
		vv_angle1024 += 200;	/* 40 �g��k�����x */
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( ((((vv_angle1024))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
			radius = (( (sin_value_t256))>>2)+16; /* 80==16+64 */
		}
		OBJ *h;
		h					= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_08_EFFECT01+i];
		#if (0)//
		h->center.x256			= src->center.x256 + ((si n1024((ww_angle1024))*(radius)) );/*fps_factor*/	/* CCW�̏ꍇ */
		h->center.y256			= src->center.y256 + ((co s1024((ww_angle1024))*(radius)) );/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (ww_angle1024), &sin_value_t256, &cos_value_t256);
			h->center.x256			= src->center.x256 + ((sin_value_t256*(radius)) );/*fps_factor*/
			h->center.y256			= src->center.y256 + ((cos_value_t256*(radius)) );/*fps_factor*/
		}
		#endif
		h->m_zoom.x256		= ( (1/*+255*/+(radius<<2)) );
		h->m_zoom.y256		= ( (1/*+255*/+(radius<<2)) );
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		h->rotationCCW1024	= -ww_angle1024;
		#endif
	}
}

/*static*/ void boss_effect_kotei_obj_r36_taihi(void)
{
	{
		int i;
		for (i=0; i<5; i++)
		{
			OBJ *h;
			h = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_08_EFFECT01+i];
			sprite_kotei_obj_r36_taihi(h);/*(effect obj�g�p���ł��邪�ޔ��ʒu�֑ޔ�)*/
		}
	}
}


/*---------------------------------------------------------
	�{�X�I�v�V�����A����
---------------------------------------------------------*/
global int check_boss_action(void);
global OBJ_CALL_FUNC(check_tukaima_kougeki_time_out)
{
	/*(�U������������΍U������B)*/
	{/*(�U������)*/
		if (NULL != src->callback_kougeki)
		{
			(src->callback_kougeki)(src);
		}
	}
	//
	#if 0
	/* �{�X��|���ƊF�j�󂳂��B */
//	if (0==boss_life_value)/* �_������(?) */
//	if (0==src->base_hp)/* �_������(?) */
	OBJ *obj_boss;
	obj_boss = &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS_HONTAI];/*(�{�X�{��)*/
	if (0==obj_boss->base_hp)/* �_������(?) */
	{
		src->BOSS_DATA_05_move_jyumyou = (0);
	}
	#endif
	#if (1)
	/*(�{�X���X�y�J��ς����ꍇ�A�������ă{�X�I�v�V����������)*/
//	if (CARD_BOSS_TIMER_0000_HATUDAN <= card.mo de_state)/*���e���ȊO*/ /*(�J�[�h���ȊO)*/
//	if (boss_01_speka_kougeki != card.bo ss_select_action_callback)/*�{�X�A�X�y�J�ōU�����B�ȊO*/ /*(�J�[�h���ȊO)*/
	if (check_boss_action())/*�{�X�A�X�y�J�ōU�����B�ȊO*/ /*(�J�[�h���ȊO)*/
	{
		src->BOSS_DATA_05_move_jyumyou = (0);/*����*/
	}
	#endif
	src->BOSS_DATA_05_move_jyumyou--;
	if (0 > src->BOSS_DATA_05_move_jyumyou)
	{
		#if 1
		src->base_hp	= (0);/* (?) ����ŗǂ������؂̕K�v���� */
		#endif
		src->jyumyou		= JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	�{�X�I�v�V�����A����
	���Ԑ؂�̏ꍇ�̓����x�ݒ�
---------------------------------------------------------*/
global OBJ_CALL_FUNC(set_timeout_alpha)
{
	if (0x1ff > (src->BOSS_DATA_05_move_jyumyou))
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->BOSS_DATA_05_move_jyumyou<<(23))&0xff000000);
	}
}
