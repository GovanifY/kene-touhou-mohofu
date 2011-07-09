
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

global void lose_option_00(SPRITE *src)
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
			item_create_99_random_item(src);
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/

//extern SPRITE *obj_effect[8]; /* [��]�i�X�y���J�[�h�̃O���t�B�b�N�j */
		#if 0/* ���������G�͓|���Ȃ��G��������Ȃ��̂ŁA�����ŕK�������̂͂܂����C������ */
	/* ���������G���� */
	bakuhatsu_add_type(src, BAKUHATSU_MINI00);
	cg.teki->type	= SP_DELETE;/* ??? */
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
		#endif
/*---------------------------------------------------------
	�G�ړ�(�U�R���Ŕ����G�t�F�N�g)
---------------------------------------------------------*/
//	/* �U�R���Ŕ����G�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
//	bakuhatsu_add_type_ddd(src, BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/* �U�R���Ŕ��� */

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
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
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
	�{�X���S����
	-------------------------------------------------------
	�� �U���̏ꍇ�̎��S���� 		DESTROY_CHECK_00_WIN_BOSS
	�� ���Ԑ؂�̏ꍇ�̎��S���� 	DESTROY_CHECK_01_IS_TIME_OUT
---------------------------------------------------------*/

/*static*/ void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type)
{
	if (0 >= /*data->boss_base.boss*/src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
	{
		/*data->boss_base.boss*/src->base_hp = 0;
		if (DESTROY_CHECK_00_WIN_BOSS == check_type)
		{
			cg.state_flag		|= STATE_FLAG_09_IS_WIN_BOSS;	/* �U���œ|�����t���Oon */
		}
	//	#if 1
		lose_boss(src);/* ���� */
	//	#else
	//	if (NULL != src->callback_loser)
	//	{
	//		(src->callback_loser)(src);
	//		src->callback_loser = NULL;
	//	}
	//	#endif
	//	voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int ee_angle1024;
static int ff_angle1024;
/*static*/ void move_boss_effect(SPRITE *src)
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
		radius = ((sin1024(vv_angle1024))>>2)+16; /* 80==16+64 */
		SPRITE *h;
		h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
		h->cx256			= src->cx256 + ((sin1024((ww_angle1024))*radius));
		h->cy256			= src->cy256 + ((cos1024((ww_angle1024))*radius));
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
				h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
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

---------------------------------------------------------*/

#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 			user_data00 	/* �ڕWx���W */
	#define target_y256 			user_data01 	/* �ڕWy���W */
	#define toutatu_wariai256		user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define kougeki_anime_count 	user_data03 	/* �U���A�j���[�V�����p�J�E���^ */
	#define boss_time_out			user_data04 	/* �������� */
	#define boss_base_state777		user_data04 	/* ��������(boss_time_out�Ɠ���) */
//
	#define boss_spell_timer		user_data05 	/* �X�y������ */
#endif


/*---------------------------------------------------------
	�{�X�U���ړ��̂݁B
---------------------------------------------------------*/

global void boss_yuudou_idou_nomi(SPRITE *src)
{
	src->toutatu_wariai256 -= (1);
	if (1 > src->toutatu_wariai256 )
	{
		src->toutatu_wariai256 = (0);
	}
}

/*---------------------------------------------------------
	�{�X�U���䗦�v�Z�B
---------------------------------------------------------*/

global void boss_yuudou_hiritu_keisan(SPRITE *src)	/* �ڕW��ݒ肵�A�U���ړ� */
{
	{	/* ���� == (�e�̌��ݍ��W - �e�̗U�����W) */
		int x_sa256 = (src->cx256 - src->target_x256);
		int y_sa256 = (src->cy256 - src->target_y256);
		/* ���Z���� == (�e�̍������W * �U���䗦) */
		int aaa_x256 = ((x_sa256 * ((src->toutatu_wariai256) ))>>8);	/*fps_factor*/
		int aaa_y256 = ((y_sa256 * ((src->toutatu_wariai256) ))>>8);	/*fps_factor*/
		src->cx256 = (src->target_x256) + (aaa_x256);		/*fps_factor*/
		src->cy256 = (src->target_y256) + (aaa_y256);		/*fps_factor*/
	}
}



/*---------------------------------------------------------
	�{�X�I�v�V�����A����
---------------------------------------------------------*/
global void check_boss_option_time_out(SPRITE *src)
{
	#if 1
	/* �{�X��|���ƊF�j�󂳂��B */
	if (0==boss_life_value)/* �_������(?) */
	{
		src->boss_time_out = (0);
	}
	src->boss_time_out--;
	if (0 > src->boss_time_out)
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
	if (0x1ff > src->boss_time_out)
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->boss_time_out<<(23))&0xff000000);
	}
}


/*---------------------------------------------------------
	�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)
	-------------------------------------------------------
???��R�����g???	�{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)
???��R�����g???	(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�)
---------------------------------------------------------*/

extern void player_loop_quit(void);

extern void set_clear_pdc_special_ivent(void);
extern void set_bg_alpha(int set_bg_alpha);

global /*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY ���K�v */
//
	set_clear_pdc_special_ivent();
	{
	//	pd_bo ssmode = B02_BOSS_DESTROY;
//		cg.state_flag |= ST ATE_FLAG_11_IS_BOSS_DESTROY;
		#if 1
		/* �o�O�ŃA�C�e���o�Ȃ�(???) */
	//	if (cg.state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* �U���œ|�����ꍇ�̂� */
		#endif
		{
		//	cg.state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* �I��� */
			/* ���ł��̏ꍇ�A������炢�����A�L�[���͗L��(0) */
//			cg.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL));	/* �I��� */
			cg.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS/*???|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL*/));	/* �I��� */
			if (0 >= spell_card.boss_timer)
			{
				;/* ���Ԑ؂�̏ꍇ�̓{�[�i�X�A�C�e���Ɠ��_�Ȃ��B */
			}
			else
			{
				item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* �{�[�i�X�A�C�e�����o�� */
				player_dummy_add_score(src->base_score);			/* �{�X�̓��_���Z */
			}
		}
	}
	//		data->boss_base.boss_health = 0;/* �Ȃ񂩃o�O��̂Œǉ� */
	/* �R�[���o�b�N�o�^ */
	src->callback_hit_teki	= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	#if 0
//	src->callback_mover 		= NULL; 		���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
//	src->type					= SP_DELETE;	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	#else
	src->callback_mover 		= common_99_keitai;/* ���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������) */
	#endif
//
//
//	set_clear_pdc_special_ivent();
//
	cg.bomber_time		= 0;
	#if 1
//	pd_bomber_time = 0;/* �s���� */
	set_bg_alpha(255);/* ��ʂ𖾂邭���� */
	#endif

	#if 1
	src->base_hp = 0;
	boss_life_value = 0;
	#endif
	#if 1
	draw_boss_hp_value	= 0;/* �悭�킩��Ȃ� */	/* �A���X��|���ƊF�j�󂳂��B */
	#endif
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* ������ */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* ������ */
	boss_effect_initialize_position();

	/*---------------------------------------------------------
		�{�X���ꏈ��
	---------------------------------------------------------*/
	//	if (B02_BOSS_DESTROY==pd_bo ssmode) //�{�X��|�����Ƃ��̏���
	//	if (/*ST ATE_FLAG_11_IS_BOSS_DESTROY==*/ (cg.state_flag & ST ATE_FLAG_11_IS_BOSS_DESTROY))
	//void boss_destroy_aaa(void)	/* �{�X��|��������A�u�{�X��C�x���g�v�O�̏��� */
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		// TIME_20_DBWAIT�t���[���҂��Ă�����s�B�{�X��|�������ɉ�ʂɕ\������Ă���e��S�ď��������̂��߂ɕK�v�B
		#if 0/* �{�ƕ��Ȃ�off */
		play_music_num(BGM_00_stop);
		#endif
//
		voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);
//		voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*�\��(���邳��)*/
//
		{
			cg.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//			cg.state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */
			if (
				(0==cg.game_practice_mode)/* ���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B */
				||/*(�܂���)*/
				(	/* �B���G���f�B���O */
					(1==cg.game_practice_mode)/* ���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B */
					&&/*(����)*/
					((0)==(cg_game_difficulty))/*(easy�̏ꍇ)*/
					&&/*(����)*/
					((6) == (cg.game_now_stage))/*(6�ʂ̏ꍇ�A�B���G���h)*/
				)
			)
			{
			//	pd_bo ssmode	= B07_AFTER_LOAD;
				cg.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
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
//
}
