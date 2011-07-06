
#include "boss.h"//#include "game_main.h"//#include "bullet_object.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------2
	�U�R�o�^�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

global void add_zako_common(STAGE_DATA *l, SPRITE *src)
{
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* �f�t�H���g�͂����蔻�肠�� */
	src->callback_loser 	= lose_random_item; 		/* �u�U�R�����ꂽ�ꍇ�̏����v�ɁA�u�����_���e�[�u������A�C�e�������v��ݒ� */
	src->callback_hit_enemy = callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
//
	src->cx256				= ((l->user_x)<<8); 	/* �ݒ�t�@�C������o��x���W�����߂�B */
	src->cy256				= ((l->user_y)<<8); 	/* �ݒ�t�@�C������o��y���W�����߂�B */
	src->base_hp			= ((l->user_hp));		/* �ݒ�t�@�C������̗͂����߂�B */
	src->base_score 		= ((l->user_score));	/* �ݒ�t�@�C������l���X�R�A�����߂�B */
}


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/

//extern SPRITE *obj_effect[8];	/* [��]�i�X�y���J�[�h�̃O���t�B�b�N�j */

/*---------------------------------------------------------
	�U�R���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

void callback_hit_zako(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{
		src->base_hp -= tama->base_weapon_strength; 	/* �U�����đ̗͌��炷(����������) */
		if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
		{
			bakuhatsu_add_type_ddd(src, BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/*�U�R���Ŕ���*/
			/* (�U�R��)�J�X�^�����ꏈ�� */
			if (NULL != src->callback_loser)	/* �J�X�^�����ꏈ��������Ύ��s�B */
			{
				(src->callback_loser)(src); 	/* �J�X�^�����ꏈ�������s����B(�W���́u�����_���e�[�u������A�C�e�������v) */
				src->callback_loser = NULL; 	/* ���s�����̂�(�O�̈�)�����B */
			}
			bonus_info_any_score_nodel(src, ((/*data->*/src->base_score)*(/*pd_now_stage*/pd.player_now_stage/*le vel*/)) );
			/*���������֎d�l�ύX*/src->jyumyou = JYUMYOU_NASI;
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
		}
	}
}


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/
int boss_hp_dec_by_frame;/*�{�X�U�������l�A�t���[���P��*/

/*static*/ void callback_hit_boss(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)/*, int angle*/
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{
		boss_hp_dec_by_frame += /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
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
			pd.state_flag		|= STATE_FLAG_09_IS_WIN_BOSS;	/* �U���œ|�����t���Oon */
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
		h					= &obj00[FIX_OBJ_08_EFFECT01+i];
		h->cx256 			= src->cx256 + ((sin1024((ww_angle1024))*radius));
		h->cy256 			= src->cy256 + ((cos1024((ww_angle1024))*radius));
		#if (1==USE_ZOOM_XY)
		h->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		h->m_zoom_y256		= ( (1/*+255*/+(radius<<2)) );
		#else
		h->m_zoom_xy256 	= ( (1/*+255*/+(radius<<2)) );
		#endif
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
			h						= sprite_add_gu_error();
			#else
			h						= sprite_add_direct(FIX_OBJ_08_EFFECT01+i); 	/* �K���o�^�ł���B  */
			#endif
//			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */	/*...���[��*/		/* �d�l�o�O(?) */
			h->jyumyou				= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//			{
//				obj00[i]		= h;
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
				h					= &obj00[FIX_OBJ_08_EFFECT01+i];
				h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= (SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
			}
		}
	}

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)

	// �ᑬeffect
	SPRITE *h;
	h					= &obj00[FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
	h->color32		= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* ���@�A������ */	/*	s1->alpha			= 0x50;*/
}
/*static*/ void boss_effect_initialize_position(void)
{
	{int i;
		for (i=0; i<5; i++)
		{
		//	obj_effect[i]->type 				= SP_DELETE;
			sprite_initialize_position(&obj00[FIX_OBJ_08_EFFECT01+i]);
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define boss_time_out		user_data03 	/* �������� */
#endif


global unsigned int alice_anime_count;

global void alice_yuudou_move_only(SPRITE *src)
{
	src->vvv256 -= (1);
	if (1 > src->vvv256 )
	{
		src->vvv256 = (0);
	}
}

global void alice_yuudou_calc(SPRITE *src)	/* �ڕW��ݒ肵�A�U���ړ� */
{
	{	/* ���� == (�e�̌��ݍ��W - �e�̗U�����W) */
		int x_sa256 = (src->cx256 - src->target_x256);
		int y_sa256 = (src->cy256 - src->target_y256);
		/* ���Z���� == (�e�̍������W * �U���䗦) */
		int aaa_x256 = ((x_sa256 * ((src->vvv256) ))>>8);	/*fps_factor*/
		int aaa_y256 = ((y_sa256 * ((src->vvv256) ))>>8);	/*fps_factor*/
		src->cx256 = (src->target_x256) + (aaa_x256);	/*fps_factor*/
		src->cy256 = (src->target_y256) + (aaa_y256);	/*fps_factor*/
	}
}

static void alice_animation_only(SPRITE *src)
{
	if (alice_anime_count)	/* �U���A�j���[�V���� */
	{
		alice_anime_count--;
				if ((32)>alice_anime_count) 	{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>alice_anime_count) 	{	src->type = TEKI_10_BOSS33; }	/*src->an im_frame = 0x22;*/
		else									{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* �ړ��A�j���[�V���� */
	{
		int aaa;
		aaa = (src->cx256> src->target_x256)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /*���E*/

		#if 0
				if ( ( 16) > src->vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->vvv256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->vvv256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->vvv256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->vvv256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#endif
		src->type = aaa;
	}
}


/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

global void move_alice(SPRITE *src)
{
//	ch eck_regist_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ */
//	sp ell_card_generator222(src);	/* �X�y�J���� */
	check_regist_generate_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ / �X�y�J���� */
	#if 1/* [�X�y�J�V�X�e�����Ɉړ��\��] */
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	boss_move_vx_vy(src);
	boss_clip_rect(src);
	alice_animation_only(src);
//	move_all_doll(src);
	#endif
//
	move_boss_effect(src);		/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
}

/*---------------------------------------------------------
	�{�X�I�v�V�����A����
---------------------------------------------------------*/
global void check_boss_option_time_out(SPRITE *src)
{
	#if 1
	/* �{�X��|���ƊF�j�󂳂��B */
	if (0==boss_life_value)/*�_������(?)*/
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
	�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)
	-------------------------------------------------------
???��R�����g???	�{�X��|�����炷���Ă΂��(�v���C���[�𖳓G�ɂ����)
???��R�����g???	(�t���O�͎d�l�㎞�ԑ҂�������ׁA���݂��̗p�r�ɂ͎g���Ȃ�)
---------------------------------------------------------*/
extern void player_loop_quit(void);
extern int	now_max_continue;

extern void set_clear_pdc_special_ivent(void);
extern void set_bg_alpha(int set_bg_alpha);
extern int practice_mode;

global /*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY ���K�v */
//
	set_clear_pdc_special_ivent();
	{
	//	pd_bo ssmode = B02_BOSS_DESTROY;
//		pd.state_flag |= ST ATE_FLAG_11_IS_BOSS_DESTROY;
		#if 1
		/* �o�O�ŃA�C�e���o�Ȃ�(???) */
	//	if (pd.state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* �U���œ|�����ꍇ�̂� */
		#endif
		{
		//	pd.state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* �I��� */
			/* ���ł��̏ꍇ�A������炢�����A�L�[���͗L��(0) */
//			pd.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL));	/* �I��� */
			pd.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS/*???|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL*/));	/* �I��� */
			if (0 >= spell_card_boss_timer)
			{
				;/* ���Ԑ؂�̏ꍇ�̓{�[�i�X�A�C�e���Ɠ��_�Ȃ��B */
			}
			else
			{
				item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* �{�[�i�X�A�C�e�����o�� */
				player_dummy_add_score(src->base_score);					/* �{�X�̓��_���Z */
			}
		}
	}
	//		data->boss_base.boss_health = 0;/* �Ȃ񂩃o�O��̂Œǉ� */
	/* �R�[���o�b�N�o�^ */
	src->callback_hit_enemy 	= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
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
	pd.bomber_time		= 0;
	#if 1
//	pd_bomber_time = 0;/*�s����*/
	set_bg_alpha(255);/* ��ʂ𖾂邭���� */
	#endif

	#if 1
	src->base_hp = 0;
	boss_life_value = 0;
	#endif
	#if 1
	draw_boss_hp_value	= 0;/* �悭�킩��Ȃ� */	/* �P��/�A���X��|���ƊF�j�󂳂��B */
	#endif
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* ������ */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* ������ */
	boss_effect_initialize_position();

	/*---------------------------------------------------------
		�{�X���ꏈ��
	---------------------------------------------------------*/
	//	if (B02_BOSS_DESTROY==pd_bo ssmode) //�{�X��|�����Ƃ��̏���
	//	if (/*ST ATE_FLAG_11_IS_BOSS_DESTROY==*/ (pd.state_flag & ST ATE_FLAG_11_IS_BOSS_DESTROY))
	//void boss_destroy_aaa(void)	/* �{�X��|��������A�u�{�X��C�x���g�v�O�̏��� */
	{
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		// TIME_20_DBWAIT�t���[���҂��Ă�����s�B�{�X��|�������ɉ�ʂɕ\������Ă���e��S�ď��������̂��߂ɕK�v�B
		play_music_num(BGM_00_stop);
//
		voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		// [***090313		�ǉ�
//		voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*�\��(���邳��)*/
//
		{
			pd.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//			pd.state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */
			if (0==practice_mode)/* ���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B */
			{
			//	pd_bo ssmode	= B07_AFTER_LOAD;
				pd.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			}
			else/* ���K���[�h�̏ꍇ�A�I������ */
			{
				#if 1/* ���̂Q�̃Z�b�g�Ŏ����I�ɏI��(GAME OVER)���� */
				now_max_continue = 1;	/* �R���e�B�j���[�����Ȃ� */
				player_loop_quit();
				#endif
			}
		}
	}
//
}
