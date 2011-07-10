
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�{�X�̈ړ��B
	�{�X�̃A�N�V������̏����B
	-------------------------------------------------------
	�G���̃A�N�V������̏����B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

global void lose_tukaima_00(SPRITE *src)
{
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/
#if 0
static void lose_mima_doll(SPRITE *src)
{
//	item_create_for_boss(src, ITEM_CREATE_MODE_02);/* easy�̓{�����o���Ȃ��čςވʓ�炩������ */
	#if 1
	item_create(src, SP_ITEM_05_TENSU, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
	#else
	{	int i;
		for (i=0; i<(5); i++)
		{
			item_create_random_table(src);
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/

//extern SPRITE *obj_effect[8]; /* [��]�i�J�[�h�̃O���t�B�b�N�j */
		#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
	/* ���������G���� */
	bakuhatsu_add_type(src, BAKUHATSU_MINI00);
	cg.teki->type	= SP_DELETE;/* ??? */
	set_REG_DEST_XY(src);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_FIRE08);
		#endif
/*---------------------------------------------------------
	�G�ړ�(�U�R���Ŕ����G�t�F�N�g)
---------------------------------------------------------*/
//	/* �U�R���Ŕ����G�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
//	set_REG_DEST_XY(src);/* ���e�ʒu ���Wxy */
//	bakuhatsu_add_type_ccc(BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/* �U�R���Ŕ��� */

static void move_zako_yarare(SPRITE *src)
{
	/* �`��p�O����] */
	src->rotationCCW1024	= (src->jyumyou<<6);
	/* �O���g�� */
	src->m_zoom_x256		= (t256(4.0))-(src->jyumyou<<3);
	src->m_zoom_y256		= (t256(4.0))-(src->jyumyou<<3);
	/* ������ */
	src->color32 -= 0x08000000; /* 8==256/32 */
}
static void init_zako_yarare(SPRITE *src)
{	// �U�R���Ŕ����G�t�F�N�g��o�^
	src->callback_mover 	= move_zako_yarare;
	src->callback_hit_teki	= NULL;
	src->jyumyou			= (32);/* �G�����ꎞ�ԁB */
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
	SPRITE *src;	�U�R�G����
	SPRITE *tama;	���e
---------------------------------------------------------*/

/* �g���������邩��global */
global void callback_hit_zako(SPRITE *src, SPRITE *tama)
{
	/* �G���Ɏ��e�����������ꍇ�̉ΉԃG�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
	set_REG_DEST_XY(tama);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
//
	/* ��ƕ������������R�[�h������������B */
	{
		src->base_hp -= tama->base_weapon_strength; 	/* �U�����đ̗͌��炷(����������) */
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
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);/* �G����������炷�B */
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static int ee_angle1024;
static int ff_angle1024;
/*static*/ void move_card_square_effect(SPRITE *src)
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
		SPRITE *h;
		h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
		#if (0)//
		h->cx256			= src->cx256 + ((si n1024((ww_angle1024))*(radius)) );/*fps_factor*/	/* CCW�̏ꍇ */
		h->cy256			= src->cy256 + ((co s1024((ww_angle1024))*(radius)) );/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (ww_angle1024), &sin_value_t256, &cos_value_t256);
			h->cx256			= src->cx256 + ((sin_value_t256*(radius)) );/*fps_factor*/
			h->cy256			= src->cy256 + ((cos_value_t256*(radius)) );/*fps_factor*/
		}
		#endif
		h->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		h->m_zoom_y256		= ( (1/*+255*/+(radius<<2)) );
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		h->rotationCCW1024	= -ww_angle1024;
		#endif
	}
}

/*static*/ void boss_effect_sprite_add(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<(5+3/*+3*/); i++)
		{
			SPRITE *h;
			#if 0
			h						= obj_add_01_teki_error();
			#else
			h						= obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i);	/* �K���o�^�ł���B */
			#endif
//			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */	/*...���[��*/		/* �d�l�o�O(?) */
			h->jyumyou				= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//			{
//				obj99[OBJ_HEAD_02_KOTEI+i]		= h;
//			}
		}
	}
}
/*static*/ void boss_effect_reset(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<(5/*+1*/); i++)
		{
			{
				SPRITE *h;
				h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->flags			&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= (SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/* ��\�� */
			}
		}
	}

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)

	// �ᑬeffect
	SPRITE *h;
	h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
	h->color32		= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* ���@�A������ */	/*	s1->alpha			= 0x50;*/
}
/*static*/ void boss_effect_initialize_position(void)
{
	{int i;
		for (i=0; i<5; i++)
		{
		//	obj_effect[i]->type 				= SP_DELETE;
			sprite_initialize_position(&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i]);
		}
	}
}


/*---------------------------------------------------------
	�{�X�U���ړ��̂݁B
---------------------------------------------------------*/

global void boss_yuudou_idou_nomi(SPRITE *src)
{
	src->BOSS_DATA_04_toutatu_wariai256 -= (1);
	if (1 > src->BOSS_DATA_04_toutatu_wariai256 )
	{
		src->BOSS_DATA_04_toutatu_wariai256 = (0);
	}
}


/*---------------------------------------------------------
	�{�X�U���䗦�v�Z�B
---------------------------------------------------------*/

global void boss_yuudou_hiritu_keisan(SPRITE *src)	/* �ڕW��ݒ肵�A�U���ړ� */
{
	{	/* ���� == (�e�̌��ݍ��W - �e�̗U�����W) */
		int x_sa256 = (src->cx256 - src->BOSS_DATA_00_target_x256);
		int y_sa256 = (src->cy256 - src->BOSS_DATA_01_target_y256);
		/* ���Z���� == (�e�̍������W * �U���䗦) */
		int aaa_x256 = ((x_sa256 * ((src->BOSS_DATA_04_toutatu_wariai256) ))>>8);	/*fps_factor*/
		int aaa_y256 = ((y_sa256 * ((src->BOSS_DATA_04_toutatu_wariai256) ))>>8);	/*fps_factor*/
		src->cx256 = (src->BOSS_DATA_00_target_x256) + (aaa_x256);		/*fps_factor*/
		src->cy256 = (src->BOSS_DATA_01_target_y256) + (aaa_y256);		/*fps_factor*/
	}
}


/*---------------------------------------------------------
	�{�X�I�v�V�����A����
---------------------------------------------------------*/
global void check_tukaima_time_out(SPRITE *src)
{
	#if 1
	/* �{�X��|���ƊF�j�󂳂��B */
//	if (0==bo ss_life_value)/* �_������(?) */
//	if (0==src->base_hp)/* �_������(?) */
	SPRITE *obj_boss;
	obj_boss = global_obj_boss;
//	obj_boss = �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
	if (0==obj_boss->base_hp)/* �_������(?) */
	{
		src->BOSS_DATA_05_move_jyumyou = (0);
	}
	src->BOSS_DATA_05_move_jyumyou--;
	if (0 > src->BOSS_DATA_05_move_jyumyou)
	{
		#if 1
		src->base_hp	= (0);/* (?) ����ŗǂ������؂̕K�v���� */
		#endif
		src->jyumyou		= JYUMYOU_NASI;
	}
	#endif
}


/*---------------------------------------------------------
	�{�X�I�v�V�����A����
	���Ԑ؂�̏ꍇ�̓����x�ݒ�
---------------------------------------------------------*/
global void set_timeout_alpha(SPRITE *src)
{
	if (0x1ff > (src->BOSS_DATA_05_move_jyumyou))
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->BOSS_DATA_05_move_jyumyou<<(23))&0xff000000);
	}
}


/*---------------------------------------------------------
	�{�X���S����
	-------------------------------------------------------
	�� �U���̏ꍇ�̎��S���� 		DESTROY_CHECK_00_WIN_BOSS
	�� ���Ԑ؂�̏ꍇ�̎��S���� 	DESTROY_CHECK_01_IS_TIME_OUT
	-------------------------------------------------------
	�{�X�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)
---------------------------------------------------------*/
extern void player_loop_quit(void);
extern void jiki_eien_muteki_on(void);/*(���@�N���Xjiki::eien_muteki_on(void);) (game_core/jiki/jiki.c)*/
extern void set_bg_alpha(int set_bg_alpha);
/*static*/ void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type)
{
	if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
	{
		src->base_hp = 0;
		/* �R�[���o�b�N�o�^ */
		src->callback_hit_teki		= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
		src->callback_mover 		= common_99_keitai;/* ���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������) */
		#if 0
		src->base_hp = 0;
		//	bakuhatsu_add_circle(src, 1);/*(�����G�t�F�N�g)*/
		SPRITE *obj_boss;
		obj_boss = global_obj_boss;
	//	obj_boss = �����蔻��̓s���㖳��&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
		obj_boss->base_hp = 0;//	bo ss_life_value = 0;
		#endif
		{
			if (DESTROY_CHECK_00_WIN_BOSS == check_type)		/* �U���œ|�����ꍇ�̂� */
			{
				if (0 >= card.boss_timer)
				{
					;/* ���Ԑ؂�̏ꍇ�̓{�[�i�X�A�C�e���Ɠ��_�Ȃ��B */
				}
				else
				{
					item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* �{�[�i�X�A�C�e�����o�� */
					player_dummy_add_score(src->base_score);			/* �{�X�̓��_���Z */
				}
			}
			//
			jiki_eien_muteki_on();/*(���@���ꎞ�I�ɁA���G��Ԃɂ���)*/
			set_bg_alpha(255);/* ��ʂ𖾂邭���� */
		//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* ������ */
		//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* ������ */
			boss_effect_initialize_position();
			// �{�X��|�����Ƃ��̏���
			bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
			voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);
		//	voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*�\��(���邳��)*/
			cg.draw_boss_hp_value	= 0;/* �悭�킩��Ȃ� */	/* �A���X��|���ƊF�j�󂳂��B */	 /* �Ȃ񂩃o�O��̂Œǉ� */
			cg.bomber_time			= (0);
		//	pd_bomber_time = 0;/* �s���� */
			// /* �{�X��|��������A�u�{�X��C�x���g�v�O�̏��� */
			{
				cg.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
		//		cg.state_flag		&= (~(JIKI_FLAG_0x04_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* �I��� */
				cg.state_flag		|= ( (JIKI_FLAG_0x04_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* �������W�J�n */
				/* �������W���[�h�̓X�e�[�W���[�h���ɋ������������B */
				if (
					(0==cg.game_practice_mode)/* ���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B */
					||/*(�܂���)*/
					(	/* �B���G���f�B���O */
						((1)==cg.game_practice_mode)/* ���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B */
						&&/*(����)*/
						((0)==(cg.game_difficulty))/*(easy�̏ꍇ)*/
						&&/*(����)*/
						((6) == (cg.game_now_stage))/*(6�ʂ̏ꍇ�A�B���G���h)*/
					)
				)
				{
				//	pd_bo ssmode	= B07_AFTER_LOAD;
					cg.state_flag |= STATE_FLAG_10_IS_LOAD_KAIWA_TXT;
				}
				else/* ���K���[�h�̏ꍇ�A�I������ */
				{
					#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
					cg.game_now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
					player_loop_quit();
					#endif
				}
			}
		}
	}
}
