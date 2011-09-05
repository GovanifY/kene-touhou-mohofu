
#include "boss.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�J�[�h�Ǘ��V�X�e��
	-------------------------------------------------------
	�J�[�h�̓V�X�e���ŃT�|�[�g���Ă���̂ŁA�ǂ̃{�X���A�ǂ̃J�[�h�ł����Ă܂��B
	-------------------------------------------------------
	ToDo:
	�� �U���̊Ǘ�
	�� ����̊Ǘ�(���\����)
	�� �G��(���G��)���J�[�h�����Ă�@�\�́A�p�~���������ǂ������m��Ȃ��B
	����(���G�������G���J�[�h�@�\)��p�~���āA����ɓ����Ƀ{�X���o��������(�܂蒆-�{�X)
	�̕����A�V�X�e�����������肵�ėǂ��C������B��������ׂɂ͐F�X������������B
	����� dat �̋K�i���ƁA�����͏o���Ȃ��̂ŁA��� dat �̋K�i��ς��āA
	dat ����{�X�̉�b���N������悤�ɂ���K�v������B
	�v���C���[�ɋ����ĉ�b���Ⴄ�̂ŁA���̕ӂ̋K�i����͂܂�����B(����V�i���I�Ƃ�)
	-------------------------------------------------------
	�� �G��(���G��)���J�[�h�����Ă�@�\�́A�p�~���āA
	�G��(���G��)���J�[�h�����������ꍇ�́A�u�J�[�h�����Ă�U�R�v�Ƃ�������V�݂��Ă�����g��
	�V�X�e���ł̓T�|�[�g���Ȃ������ǂ��Ǝv���B�̂ł������������ŉ����\��B
---------------------------------------------------------*/

#include "kanji_system.h"

#include "../menu/kaiwa_sprite.h"

/*---------------------------------------------------------
	�{�X�ǉ�
---------------------------------------------------------*/

// �`���m EX1��
// ��� 6��
extern void boss_move_17_sakuya_bimyou_idou(OBJ *src);/*(�����Ɉړ�����)*/
extern void boss_move_16_sakuya_nazo_keitai(OBJ *src);/*(�������̓�`��)*/
extern void boss_move_15_sakuya_festival_knife(OBJ *src);/*(�����u�t�F�X�e�B�o���i�C�t�v)*/
extern void boss_move_14_sakuya_miss_direction(OBJ *src);/*(��p�u�~�X�f�B���N�V�����v)*/

// �p�`�F 5��
//���[�U�[
extern void add_laser_off(OBJ *src);
extern void add_laser_on(OBJ *src);
// �� 4��
extern void boss_move_13_aya_taifu(OBJ *src);
// �P�� 3��
extern void boss_move_12_kaguya_funya_idou(OBJ *src);
extern void boss_move_11_kaguya_yureru(OBJ *src);
extern void boss_init_kaguya(OBJ *src);
// ���� 2��
extern void boss_move_10_mima_keitai(OBJ *src);
// �A���X 1��


// ���ʌ`��
extern void boss_move_05_xy_douki_differential64(OBJ *src);
extern void boss_move_04_xy_douki_differential32(OBJ *src);
extern void boss_move_03_x_douki(OBJ *src);
extern void boss_move_02_xy_hidouki(OBJ *src);
extern void boss_move_01_taihi_ue_naka(OBJ *src);


/* ���ʕ� */
static void init_00_boss_clip000(OBJ *h);/* �W���^�C�v */
static void init_00_boss_clip111(OBJ *h);/* ��ɍL���^�C�v */
static void init_00_boss_clip222(OBJ *h);/* �ゾ���L���^�C�v */



//------------ �J�[�h�֘A
#include "card_address.h"
#if (1)
	/* ���݂̌`�ԂŌ��Ă�J�[�h�ԍ����J�[�h�V�X�e���ɐݒ�B */
	/* AYA�T�`�Ԃ̏ꍇ�B(r35-)�̃V�X�e���Œʏ�e�𗘗p����ׂɁA�J�[�h�V�X�e���̏��������K�v�B */
	/* ���������āA�ǂ�Ȍ`�Ԃł�(r35�ȍ~��)�K���X�y�J�ɂ���K�v������B */
	#define init_48_r34_gokan_kinou 	card_maikai_init_and_get_spell_number
	#if (1)
		#define SPELL_r34_GOKAN_KINOU		SPELL_48_r34_gokan_kinou/*(�K�v)*/
	#else
	//	#define SPELL_r34_GOKAN_KINOU		SPELL_00/*(�_��)*/
	#endif
#endif
//
// int card.limit_health;	/* �K��l�ȉ��ɂȂ�� �J�[�h���[�h���� */
// int card.boss_state; 	/* ���l�ɂȂ�΃{�X �J�[�h���[�h�ɓ���Ȃ� */
// int card.mode;			/* �J�[�h���[�h */
// int card.boss_timer; 	/* ���p */	// ��������
// int card.number; 		/* ���p */	// �J�[�h�ԍ�

global CARD_SYSTEM_GLOBAL_CLASS card;

/*---------------------------------------------------------
	�X�v���C�g ���X�g�ɓo�^���ꂽ�X�v���C�g��S�� �B
---------------------------------------------------------*/
#define TEST_ZAKO_HIDE (0)
#if (1==TEST_ZAKO_HIDE)/* �{�X���e���󂯂� */
static void sprite_sethide_all(OBJ *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* �S�����ׂ�B */
	{
		s->BOSS_DATA_05_move_jyumyou	= 0xff;
		#if (1)/* �f�o�b�O�Ă��� */
		s->cx256		= (-1);
		#endif
		s++;
	}
}
static void zako_all_timeup(void)/*int ty pe*/
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_01_0x0800_TEKI+0];
	sprite_sethide_all(s, OBJ_POOL_01_TEKI_0256_MAX);
}
#endif


/*---------------------------------------------------------
	[r36�V]�{�X�U���ړ��̂݁B
---------------------------------------------------------*/

static int bos_ddd_obj_alt_x256;
static int bos_ddd_obj_alt_y256;
static int bos_ddd_obj_target_x256;
static int bos_ddd_obj_target_y256;
//static int bos_ddd_obj_ido_speed256;


/*---------------------------------------------------------
	�{�X�U���ړ��̂݁B
---------------------------------------------------------*/
/* �U���ړ�/�U���v�Z */
static void boss_r36_yuudou(OBJ *src)
{
	{	/*(�Ƃ肠����)*/
		src->BOSS_DATA_04_toutatu_wariai256 -= (6);/*(�{�X�ޔ��A���x�B1/t256()�`���B(�t���Ŏw�肷��) )*/
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (bos_ddd_obj_ido_speed256);/*(�{�X�ޔ��A���x)*/
		src->BOSS_DATA_04_toutatu_wariai256 = psp_max(src->BOSS_DATA_04_toutatu_wariai256, 0);
		//
		if (0 < src->BOSS_DATA_04_toutatu_wariai256)/*(�ړ����K�v�ȏꍇ�݈̂ړ��������s��)*/
		{
			int move_rate256;
			move_rate256 = src->BOSS_DATA_04_toutatu_wariai256;
			move_rate256 = psp_min(move_rate256, 256);
			move_rate256 = psp_max(move_rate256, 0);
			#if (1)/* �񓙕��l�̌v�Z */
			{
				u32 i_rate65536;
				i_rate65536 = (move_rate256 << 8);
			//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
				i_rate65536 = (int)(vfpu_ease_in_out65536(i_rate65536));	/* t65536�̓����J�E���^�l���g���A�񓙕��ɂȂ�悤�l���C������B
				�񓙕��̕����́uease_in_out�v����́A�n�߂ƍŌオ�������ŊԂ������Ƃ����z�������B */
				move_rate256 = (i_rate65536 >> 8);
			}
			#endif
			//------------------
			#if (1)/*(���B�������l�����āA��������B)*/
		//	REG_00_SRC_X	= ((bos_ddd_obj_alt_x256)); 	/*(�����ʒu[A]t256()�`��)*/
		//	REG_01_SRC_Y	= ((bos_ddd_obj_alt_y256)); 	/*(�����ʒu[A]t256()�`��)*/
		//	REG_02_DEST_X	= ((bos_ddd_obj_target_x256));	/*(�����ʒu[B]t256()�`��)*/
		//	REG_03_DEST_Y	= ((bos_ddd_obj_target_y256));	/*(�����ʒu[B]t256()�`��)*/
			REG_00_SRC_X	= ((bos_ddd_obj_target_x256));	/*(�����ʒu[A]t256()�`��)*/
			REG_01_SRC_Y	= ((bos_ddd_obj_target_y256));	/*(�����ʒu[A]t256()�`��)*/
			REG_02_DEST_X	= ((bos_ddd_obj_alt_x256)); 	/*(�����ʒu[B]t256()�`��)*/
			REG_03_DEST_Y	= ((bos_ddd_obj_alt_y256)); 	/*(�����ʒu[B]t256()�`��)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(��������t256()�`��)*/
			multiprex_rate_vector();/*(�j�󃌃W�X�^�����̂Œ���)*/
			#endif
			// ���̎��_�ŁA
			// REG_02_DEST_X: �����l
			// REG_03_DEST_Y: �����l
			//-----------------------
			src->cx256 = (REG_02_DEST_X);
			src->cy256 = (REG_03_DEST_Y);
		}
	}
}
/*---------------------------------------------------------
	�{�X�U���ړ��̂݁B
---------------------------------------------------------*/
global void boss_set_new_position(OBJ *src)
{
	src->BOSS_DATA_04_toutatu_wariai256 	= t256(1.0);/*(�����l)*/
	/*(���݈ʒu��ݒ�)*/
	bos_ddd_obj_alt_x256		= (src->cx256);
	bos_ddd_obj_alt_y256		= (src->cy256);
	/*(�ޔ��ʒu��ݒ�)*/
	bos_ddd_obj_target_x256 	= REG_02_DEST_X;/*(t256()�`��)*/
	bos_ddd_obj_target_y256 	= REG_03_DEST_Y;/*(t256()�`��)*/
	/*(�ړ����x��ݒ�)*/
//	bos_ddd_obj_ido_speed256	= HATSUDAN_01_speed256;/*(==REG_11)*/
}

/*---------------------------------------------------------
	���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������)
	[�J�[�h�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/

static void common_99_keitai(OBJ *src)
{
	boss_r36_yuudou(src);
	/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
	if (src->BOSS_DATA_04_toutatu_wariai256 <= t256(0))
	{
		#if (1)
	//�d��	src->callback_mover 	= NULL; 		/* �����܂� */
		sprite_kotei_obj_r36_taihi(src);/*(�{�Xobj�g�p���ł��邪�ޔ��ʒu�֑ޔ�)*/
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
		#endif
		boss_effect_kotei_obj_r36_taihi();
		//return /*(1)*/;/*�ړ�����*/
	}
}
/* ���j��ɉ�ʊO�Ƀ{�X�������� */
static void common_88_keitai(OBJ *src)
{
	/*(���[�U�[���[�h�́A�����I��off�ɂ���B)*/
	add_laser_off(NULL);
	#if (1)
	/*(�ޔ��ʒu��ݒ�)*/
	REG_02_DEST_X = (int)(GAME_X_OFFSET*256+(GAME_320_WIDTH*256/2));
	REG_03_DEST_Y = -t256((50.0/2));/* +t256(50.0) �{�X�O���̍ő�T�C�Y(50[pixel]) */
	boss_set_new_position(src);
	#endif
	src->BOSS_DATA_05_boss_base_state777			= (0);	/*ST_00*/	/*�����l��0�ɂ���B*/
	src->callback_mover 	= common_99_keitai; 		/* �ŏI�`�Ԃɂ���B */
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
/*	�� �U���̏ꍇ�̎��S���� 	�� ���Ԑ؂�̏ꍇ�̎��S���� */

extern void player_loop_quit(void);
extern void jiki_eien_muteki_on(void);/*(���@�N���Xjiki::eien_muteki_on(void);) (game_core/jiki/jiki.c)*/
extern void set_bg_alpha(int set_bg_alpha);

/* ���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������) */
/*(���j�`��)*/
/*(???)*/static void NULL_keitai(OBJ *src)/*(src==�{�X�G�{��)*/
{
	/*(card.boss_move_card_callback��NULL�ɑΉ����ĂȂ��ꍇ�ɁA�K�v�ȃ_�~�[�`��)*/
}
//	/*static*/ void boss_destroy_check_type(OBJ *src/*�G����*/, int check_type)


//	if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
	// /* �{�X��|��������A�u�{�X��C�x���g�v�O�̏��� */
//	#if (0)
//	enum
//	{
//		DESTROY_CHECK_00_WIN_BOSS = 0,
//		DESTROY_CHECK_01_IS_TIME_OUT,
//	};
//	int check_type;
//	check_type = DESTROY_CHECK_00_WIN_BOSS;/*(�Ƃ肠����)*/
//	if (DESTROY_CHECK_00_WIN_BOSS == check_type)
//	#endif

static void gekiha_keitai(OBJ *src)/*(src==�{�X�G�{��)*/
{
	#if 0
//	bakuhatsu_add_circle(src, 1);/*(�����G�t�F�N�g)*/
	#endif
	src->base_hp = 0;
	/* �R�[���o�b�N�o�^ */
	src->callback_hit_teki			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	src->callback_mover 			= common_88_keitai;/* ���ʃ{�X�ޔ�(���j��ɉ�ʊO�Ƀ{�X��������) */
//�d��(���ɐݒ�ς�)	card.boss_move_card_callback	= NULL_keitai;/*(�e�X�g)*/
	{
		#if (1==USE_BOSS_JIKANGIRE)/*(�g�p�\�肠��B������)*/
		if (0 != (cg.state_flag&JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE))/*0 >= card.boss_timer*/
		{
			;/* ���Ԑ؂�̏ꍇ�̓{�[�i�X�A�C�e���Ɠ��_�Ȃ��B */
		}
		else
		#endif
		{	/* �U���œ|�����ꍇ�̂� */
			item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* �{�[�i�X�A�C�e�����o�� */
			player_dummy_add_score(src->base_score);			/* �{�X�̓��_���Z */
		}
	}
	//
	#if (0)/*(�f�o�b�O)*/
	{	psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load stage: STAGE%d" "\\n"
			"�{�X�A���j�e�X�gOK�B", cg.game_now_stage);
	}
	#endif
//
	jiki_eien_muteki_on();/*(���@���ꎞ�I�ɁA���G��Ԃɂ���)*/
	set_bg_alpha(255);/* ��ʂ𖾂邭���� */
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* ������ */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* ������ */
	// �{�X��|�����Ƃ��̏���
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);
//	voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*�\��(���邳��)*/

	cg.draw_boss_hp_value	= (0); /* �K�v */ /*(boss_hp_frame_check()�𖳌��ɂ���BGu���Ń`�F�b�N�����Ȃ�)*/
	cg.bomber_time			= (0); /* �s���� */
	{
		#if (1==USE_r36_SCENE_FLAG)
		/*(����==NEXT_SCENE;�Ɠ���)*/
		#else
		cg.state_flag &= (~(STATE_FLAG_15_DRAW_BOSS_GAUGE));/*off*/
		#endif
//		cg.state_flag		&= (~(JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* �I��� */
		cg.state_flag		|= ( (JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* �������W�J�n */
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
			#if (1==USE_r36_SCENE_FLAG)
			NEXT_SCENE;/*(���̏�ʂ֐ݒ�)*/
			#else
		//	pd_bo ssmode	= B07_AFTER_LOAD;
			cg.state_flag |= STATE_FLAG_0x0100_IS_LOAD_KAIWA_TXT;
			#endif
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


/*---------------------------------------------------------
	[�J�[�h�V�X�e�����Ɉړ��\��]
---------------------------------------------------------*/


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
	OBJ *src;	�{�X�G����
	OBJ *tama;	���e
---------------------------------------------------------*/

/*static*/static/*global*/	void s_callback_hit_boss(OBJ *src, OBJ *tama)
{
	/* �{�X & ��-�{�X�Ɏ��e�����������ꍇ�̉ΉԃG�t�F�N�g��o�^(����Gu�������Ȃ��̂ŕ`�悵�ĂȂ�) */
	set_REG_DEST_XY(tama);/* ���e�ʒu ���Wxy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* ��Ɏ��s������������ */
//
	/* ��ƕ������������R�[�h������������B */
	{
		card.boss_hp_dec_by_frame += /*w->*/tama->kougeki_ti; /* �U�����đ̗͌��炷(����������) */
	}
}


/*---------------------------------------------------------
	�{�X�̋��ʁA�P��ڏ��������[�`��
	����A�U���s��[��b���̌`��]�A�����ݒ�

---------------------------------------------------------*/

/*---------------------------------------------------------
	�{�X�̋��ʁA�Q��ڏ��������[�`��
	[��b���̌`��]�A��b�I���܂őҋ@����B
	�ҋ@���I�������A�U���\�ɂ���B
---------------------------------------------------------*/
/*([��b���̌`��]�A�����ݒ�)*/
/* �o����x���W */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* ���S���W�Ȃ̂� */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* ���S���W�Ȃ̂� */

static void card_address_incliment(void);
static void card_set_boss_move_callback(void);
static void kaiwa_00_keitai(OBJ *src)/*([��b���̌`��]�A��b�I���܂őҋ@����)*/
{
	#if (1==USE_r36_SCENE_FLAG)
	if (SCENE_NUMBER_0x0800_BOSS_TATAKAU==(cg.state_flag & 0x0f00)) /* �{�X�퓬�O�̉�b�I���ς�? */
	#else
	if (cg.state_flag & STATE_FLAG_0x0800_IS_BOSS)	/* �{�X�퓬�O�̉�b�I���ς�? */
	#endif
	{
		/* �v���C���[�e�󂯕t���A�R�[���o�b�N��o�^ */
		src->callback_hit_teki = s_callback_hit_boss;	/* �R�[���o�b�N�o�^ */
		/* card common init */
		card.mode_timer 	= (CARD_BOSS_TIMER_0255_IDO_JYUNNBI);/*on*/
	//	card.mode		= (0);/*off*/
	//	/*���Ԑ����J�E���g�L����*/
	//	data->boss_base.state001++/* = ST_02*/;
		card_address_incliment();
		card_set_boss_move_callback();
	}
}


////////  �`�Ԍn�͂�������ɋL�q���B

/*---------------------------------------------------------
	�J�[�h�ݒ�B
---------------------------------------------------------*/
#include "card_resource.h"

/*---------------------------------------------------------
	�J�[�h�����ɐi�߂�B
---------------------------------------------------------*/
static void card_address_incliment(void)
{
	/*(���j�`�Ԃ̏ꍇ�A�J�[�h�����ɐi�߂Ȃ��B)*/
//	if (SPELL_49_r36_gekiha != my_card_resource[(card.address_set)].spell_set_number)
	{
		card.address_set += (4);/*(�J�[�h�����ɐi�߂�B)*/
	}
}
/*---------------------------------------------------------
	card���{�X�ړ�������ݒ肷��B
---------------------------------------------------------*/
static void card_set_boss_move_callback(void)
{
	card.boss_move_card_callback = my_card_resource[(card.address_set)].boss_move_keitai_set_callback;/*(card���{�X�ړ�������ݒ肷��)*/
}
static void set_new_limit(void)
{
	card.limit_health -= (my_card_resource[(card.address_set + (4))].spell_life);	/*1000 500*/
	card.limit_health = psp_max(card.limit_health, 0);		/*(0����==�����ɂ��Ȃ�)*/
	//
	#if 0
	/*(�������Ԃ𑫂�)*/
	card.boss_timer += (((my_card_resource[(card.address_set + (4))].spell_limit_time)));	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	#else
	/*
		(r35 �Œ萧�����Ԃ�ݒ�[���d�l])
		�^�C�}�[�l�̕ێ�������ꃖ�����������̂ŁA�Ƃ肠�������������`�ɂ��Ƃ����B(�������Ԃ̐ݒ蕔����������)
	*/
	card.boss_timer = (((my_card_resource[(card.address_set + (4))].spell_limit_time)));	/* 75*64==75[count] 	��99[�b(64/60)](�P�ʂ͕b�ł͂Ȃ�) */
	#endif
}
/*---------------------------------------------------------
	�J�[�h�V�X�e���̃{�X�`�Ԃɓo�^���ꂽ�J�[�h�ԍ����擾���A
//	�����ɃJ�[�h�̎��Ԑ؂��ݒ肷��B
	�����ɃX�y���̏��������s���B

---------------------------------------------------------*/

global void card_maikai_init_and_get_spell_number(OBJ *src)
{
	card.spell_used_number	= my_card_resource[(card.address_set)].spell_set_number;	/* �J�[�h���Z�b�g */
	//
	card_maikai_init(src);		/* �J�[�h�̐������Ԃ�ݒ�(�\�߃J�[�h���Ƃɐݒ肳��Ă���W�����Ԃɐݒ�) */
}

/*---------------------------------------------------------
	�{�X�`�ԕύX���̋��ʃ��[�`��
	�J�[�h���j��A�C�e���o���B
---------------------------------------------------------*/

global void common_boss_put_items(OBJ *src)
{
//++	pd_bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
	#if (0)/*�����*/
	bakuhatsu_add_circle(src, 1);
	#endif
//
	voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
	#if (1)
	/*---------------------------------------------------------
		�J�[�h���̕\��
		�J�[�h�w�i�́A�܂��������ǁA�����I�Ɋg���\��B
	---------------------------------------------------------*/
	//static void draw_card_name(void)
	{
		kanji_window_clear_line(ML_LINE_04);	/* �����E�B���h�E��4�s��(==3)�̓��e�������B */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_04)*(KANJI_FONT_18_HEIGHT_P2));	/* �J�[�\����4�s��(==3)�ֈړ� */
	//
		if (NULL != my_card_resource[(card.address_set)].spell_str_name)
		{
			/* �J�[�h�w�i������ꍇ */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_supeka;
			//
			// ""�J�[�h�̕����\���ʒu�B
			{
			//	ml_font[(ML_LINE_04)].timer 	= ML_ON;	/*"�\��ON"*/
				ml_font[(ML_LINE_04)].timer 	= byou60(5);	/* �� 5 �b */
				ml_font[(ML_LINE_04)].x 		= (96);/* X�ʒu */
				ml_font[(ML_LINE_04)].y 		= (12);/* Y�ʒu */
			//	ml_font[(ML_LINE_04)].render	= NULL;
			//	ml_font[(ML_LINE_04)].str		= my_font_text;/*(�f�o�b�O)*/	/*"�\��on"*/
			}
			ml_font[(0)].haikei 		= (ML_HAIKEI_01_BOSS_SPELL_CARD);/* [��/�{�X�J�[�h�p�w�i]�{�X�J�[�h�p����Ӕw�ion */
			cg.msg_time = byou60(5);	/* �� 5 �b */
			strcpy(my_font_text, my_card_resource[(card.address_set)].spell_str_name);
			kanji_color(/*int color_type*/(7)|STR_CODE_NO_ENTER);	/* ���s���Ȃ� */
			kanji_draw();
		}
		else
		{
			/* �J�[�h�w�i�������ꍇ */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
		}
	}
	#endif
	/*--- BOSS ���ʂ��Ēl�� 0 �ɂ���B------------*/
	src->BOSS_DATA_05_move_jyumyou = (0);
//	src->BOSS_DATA_05_boss_base_state777 = (0);/* ���ʂ��� �l�� 0 �ɂ��� */
//	player_dummy_add_score(src->boss_base_score);
}


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̃t���[���`�F�b�N
	-------------------------------------------------------
	���C�t�����Ȃ��Ȃ�ƃ{�X���Ȃ��Ȃ����ȂȂ��̂́A���o�������ł��B
	�Ă��Ɓ[�ɍČ��H
	-------------------------------------------------------
	ToDo:
	���|��A�ŋ߂̓��C�t�͂���ς胊�j�A�Ɍ����ĂāA
	�\������log�Ȃ񂶂�ˁH���ċC�����Ă����B
	�����������ɂ��悤���ȁB�Q�[���o�����X�Ƃ��F�X�ς�邩��B
---------------------------------------------------------*/

static void boss_hp_frame_check(OBJ *src)/*(�{�X�{��)*/
{
	{
		s32 limit_max_hp_dec_boss_by_frame;
	//	u8 test_draw_boss_hp_value;
		u16 test_draw_boss_hp_value;
	//	test_draw_boss_hp_value = (src->base_hp>>5) & 0xff; /* 8192/32 == 256 (r32) */
	//	test_draw_boss_hp_value = (src->base_hp>>7) & 0xff; /* 32768/32 == 256 (r33) */
		test_draw_boss_hp_value = (src->base_hp>>9) & 0x03ff;	/* ?/32 == 256*4 (r35u2-) */
		#if 1
	//	limit_max_hp_dec_boss_by_frame = (test_draw_boss_hp_value>>2) | (0x0f);
	//	limit_max_hp_dec_boss_by_frame = (test_draw_boss_hp_value>>2) | (0x08);
	//	limit_max_hp_dec_boss_by_frame = (test_draw_boss_hp_value) | (0x10);/*(-r35u1)*/
		limit_max_hp_dec_boss_by_frame = (test_draw_boss_hp_value) | (0x40);/*(r35u2-)*/
		#endif
		//
	//	if (limit_max_hp_dec_boss_by_frame < card.boss_hp_dec_by_frame)
	//	{
	//		card.boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_frame;
	//	}
		card.boss_hp_dec_by_frame = psp_min(card.boss_hp_dec_by_frame, limit_max_hp_dec_boss_by_frame);
	}
//
	src->base_hp -= card.boss_hp_dec_by_frame;
	card.boss_hp_dec_by_frame = 0;/* �g�����̂ŏ���(�t���[���P��) */
	if (card.limit_health >= src->base_hp)	/* �K��l�ȉ��ɂȂ�΃J�[�h���[�h���� */
	{
		src->base_hp = card.limit_health;	/* �K��l�����ɂ��Ȃ��B */
		card.mode_timer 		= (CARD_BOSS_TIMER_0256_SET);
	//	boss_destroy_check_type(src, DESTROY_CHECK_00_WIN_BOSS);
		cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
/*---------------------------------------------------------
	�J�[�h�o�^
---------------------------------------------------------*/

/*
	 [-1]���e��
  >
 0
	 [0...253]�����G�ړ�
  >
 254 == 256-2
	 [254]�ݒ�
 255 == 256-1
	 [255...]

*/


/*global*/global/*static*/ void card_boss_move_generate_check_regist(OBJ *src)
{
	card.mode_timer--;
	/*
		[�J�[�h�U���݂̂Ɏd�l�ύX]
		(�������̈ړ��Ȃǂ��J�[�h����)
		(�J�[�h�����Ȃ����������)
	*/
	/* �{�X�s�� */
	if (CARD_BOSS_TIMER_0000_HATUDAN > card.mode_timer)/*���e��*/ /*(�J�[�h��)*/
	{
		/* (�Ƃ肠����)�J�[�h���[�h���̂݃{�X���Ԍo�� */
		{
			card.boss_timer--;/*fps_factor*/
			if (0 >= (card.boss_timer)) /*1*/
			{
				card.boss_timer 	= (0);/*(�ꎞ�I��0�ɂ���)*/ 	/*(�������̃X�y�J������΁A�X�y�J�V�X�e�����Ŏ��ֈڍs����B���̍ۂ� card.boss_timer �Ɏ��Ԃ����Z�����B)*/
				card.mode_timer 	= CARD_BOSS_TIMER_0000_HATUDAN/*off*/;/**/
				src->base_hp		= card.limit_health;		/* (�Ƃ肠����) */
			//	boss_destroy_check_type(h/*�G����*/, DESTROY_CHECK_01_IS_TIME_OUT);/*	�� �U���̏ꍇ�̎��S���� 	�� ���Ԑ؂�̏ꍇ�̎��S���� */
				#if (1==USE_BOSS_JIKANGIRE)/*(�g�p�\�肠��B������)*/
				cg.state_flag |= JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE;/*(���Ԑ؂�t���Oon)*/
				#endif
			}
		}
	//	#if (0)/*(r36, NULL�͓o�^�ł��Ȃ��B�������Ȃ��ꍇ�� NULL_keitai ��o�^����B)*/
	//	if (NULL != card.boss_move_card_callback)
	//	#endif
		{
			(card.boss_move_card_callback)(src);/*(�{�X�ړ��`�Ԗ��ɁA�{�X�ړ����������s����)*/
		}
		boss_r36_yuudou(src);
		if (TUKAIMA_00_OFF != card.tukaima_used_number) 	/* �g���������͕K�v�H */
		{
			tukaima_system_add_dolls(src);/*(�����̎g�����B�̐���������B�g�����͈�l�łȂ��ĕ����̕��������B)*/
			card.tukaima_used_number = TUKAIMA_00_OFF;/*(�������������̂� off �ɂ���B)*/
		}
		/*�{�X���U�������ꍇ�̃t���[���`�F�b�N*/
		if (0!=cg.draw_boss_hp_value)/*(boss_mode)*/
		{	boss_hp_frame_check(src);}/*�{�X���U�������ꍇ�̃t���[���`�F�b�N/�J�[�h���[�h�`�F�b�N*/
	}
	else
//	if (CARD_BO SS_MODE_02_TAIHI==card.mode) /* ���e�ʒu�܂ňړ����B */
	if ((CARD_BOSS_TIMER_0256_SET-2) > card.mode_timer) /* ���e�ʒu�܂ňړ����B */
	{
		#if 1/*(r32p)*/
	//	bo ss_y uudou_idou_nomi(src);/*(r32p)*/ //	src->BOSS_DATA_04_toutatu_wariai256 -= (1); /* [��	4 �b]== 4.2666==(256/60[frame]) */
	//	bo ss_y uudou_hiritu_keisan(src);
		boss_r36_yuudou(src);
		#if 0
	//	if (0 > src->BOSS_DATA_04_toutatu_wariai256 )	/* �قډ�ʒ��S�t�߂܂ŁA�ړ������B */
		if ((0==src->BOSS_DATA_04_toutatu_wariai256))
		{
	//		src->BOSS_DATA_04_toutatu_wariai256 = (0);
			card.mode_timer 	= (CARD_BOSS_TIMER_0000_HATUDAN);/*on*/
		}
		#endif
		#endif/*(r32p)*/
		// �{�X(�����G�ړ�)
		#if 0/*(�����ړ�)*/
		kaiwa_sprite[1].cy256		= t256(272+128) - ((card.mode_timer)<<9);
		#else/*(�������ړ�)*/
		{
			u32 aaa = (card.mode_timer);
			aaa += 128;/*(�㉺����ւ�)*/
			aaa &= 0xff;/*(0...255)*/
		//	aaa = ((aaa)|(aaa<<8));/*(0...65535)*/
			aaa <<= 8;/*(0...65535)*/
			aaa = vfpu_ease_in_out65536(aaa);
			aaa += 32768;/*(�㉺����ւ�)*/
			aaa &= 0xffff;/*(0...65535)*/
			aaa += aaa;/*(2�{)*/
			kaiwa_sprite[1].cy256		= t256(272+128) - (aaa);
		}
		#endif
	}
	else
	if ((CARD_BOSS_TIMER_0256_SET-1) > card.mode_timer) /* ���e�ʒu�܂ňړ����B */
	{
		REG_10_BOSS_SPELL_TIMER = (0);	/* �J�[�h�����������I�Ɏ~�߂�B */
		bullets_to_hosi();		/* ���Ă̓G�e���Ahosi�A�C�e���ɕς��� */
		/* �^���t�߂ɑޔ� */
	//	src->BOSS_DATA_04_to utatu_wariai256	= t256(  0);/* �������ς݂̕K�v���� */
	//	src->BOSS_DATA_04_to utatu_wariai256	= t256(1.0);/* �������ς݂̕K�v���� */
	//	src->BOSS_DATA_00_ta rget_x256			= BOSS_XP256; //t256(0); t256(153);
	//	src->BOSS_DATA_01_ta rget_y256			= t256(16.0); //t256(0); src->cy256;
		REG_02_DEST_X	= (BOSS_XP256);/*(t256()�`��)*/
		REG_03_DEST_Y	= (t256(16.0));/*(t256()�`��)*/
		boss_set_new_position(src);/*(�U���ړ����W��ݒ�)*/
		//
		#if 1/* (�V[�J�[�h�n�܂�O�ɏ�����]) ��0�`�Ԃ���A�K���ĂԔ��B */
		/* �J�[�h������ */
	//	#if (0)/*(r36, NULL�͓o�^�ł��Ȃ��B�������Ȃ��ꍇ�� NULL_keitai ��o�^����B)*/
	//	if (NULL != my_card_resource[(card.address_set)].spell_init_callback)
	//	#endif
		{
			(my_card_resource[(card.address_set)].spell_init_callback)(src);
		}
		#endif
		// �{�X
		kaiwa_sprite[1].cx256		= t256(256);
	}
	else
//	if ((CARD_BOSS_TIMER_0256_SET) > card.mode_timer)/*(���f)*/  /*off*/
	{
		/* [(�Ƃ肠����)�J�[�h�U���݂̂Ɏd�l�ύX]�����̂ŁA�Ō㌂�Ă�J�[�h���Ȃ��Ȃ����ꍇ�ɍU��������ׁB */
		/*(�K��l�̎Z�o)*/
		/*(���~�b�g������)*/
		set_new_limit();
		{
			card_address_incliment();
			card_set_boss_move_callback();
			#if 0//(1==TEST_ZAKO_HIDE)/* �{�X���e���󂯂� */
			zako_all_timeup();/* �U�R�^�C���A�E�g(�t�F�C�h�A�E�g���������ֈڍs) */
			#endif
			/* �A�C�e���f�� */
			if (NULL != src->callback_loser)
			{
				(src->callback_loser)(src); 	/* sakuya_put_items(src); */
			}
		}
	}
}


/*---------------------------------------------------------
	�e�͈̔͂��u�W���v�ɐݒ�
	-------------------------------------------------------
	set_default_bullet_clip
---------------------------------------------------------*/
static void init_00_boss_clip000(OBJ *h)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET);						/*(���͕W���͈�)*/
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH);	/*(���͕W���͈�)*/
	rect_clip.bullet_clip_min.y256 = t256(-32); 			/*(��͕W���͈�)*/
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT); 	/*(���͕W���͈�)*/
}

/*---------------------------------------------------------
	�e�͈̔͂��u��Ɖ��̍L�͈́v�ɐݒ�
	-------------------------------------------------------
	set_aya_bullet_clip
---------------------------------------------------------*/
static void init_00_boss_clip111(OBJ *h)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET)					  + t256(-100);/*(���͍L�͈�)*/
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH) + t256( 100);/*(���͍L�͈�)*/
	rect_clip.bullet_clip_min.y256 = t256(-256);			/*(��͍L�͈�)*/		/* ���� */
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT); 	/*(���͕W���͈�)*/
}

/*---------------------------------------------------------
	�e�͈̔͂��u�ゾ���L���^�C�v�v�ɐݒ�
	-------------------------------------------------------
	set_cirno_bullet_clip
---------------------------------------------------------*/

static void init_00_boss_clip222(OBJ *h)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET);						/*(���͕W���͈�)*/
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH);	/*(���͕W���͈�)*/
	rect_clip.bullet_clip_min.y256 = t256(-256);			/*(��͍L�͈�)*/		/*(�Ă��ƃ`���m)*/
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT); 	/*(���͕W���͈�)*/
}

/*---------------------------------------------------------
	�����p�́A�e�͈̔͂��u�W���v�ɐݒ�B(from load_stage.c)
---------------------------------------------------------*/
global void set_default_bullet_clip(void)	/* �W���^�C�v */
{
	init_00_boss_clip000(NULL); 			/* �e�͈̔͂�W���ɐݒ� */
}


/*---------------------------------------------------------
	�{�X�G�̏������B
---------------------------------------------------------*/
		#if 0/* �������ς݂̕K�v���� */
		h->vx256						= t256( 0);
		h->vy256						= t256( 0);
		#endif
	//

	extern void stage_boss_load_texture(void);
// src/core/douchu/boss.h �̏��������Q�Ƃ��鎖�B
extern void root_boss_mover(OBJ *src);
global void called_from_kaiwa_system_boss_load(int boss_number)
{
	/*(�{�X�ԍ��� 0-7 �̂����ꂩ�ɏ���ɌŒ�(r36����)�B)*/
	boss_number &= (8-1);
	/*(�{�X�{�̂̍s���͈͂𐧌�����B)*/
	{
		/* boss_rect_init */
		rect_clip.boss_clip_min.x256	= t256(GAME_X_OFFSET)+t256( 			0)+t256(24);
		rect_clip.boss_clip_max.x256	= t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH)-t256(24);
		rect_clip.boss_clip_min.y256	= t256(0);
		rect_clip.boss_clip_max.y256	= t256(96);
	}
	//----[BOSS]
	OBJ *h;
	h					= &obj99[OBJ_HEAD_01_0x0800_TEKI+TEKI_OBJ_00_BOSS_HONTAI];/*(�{�X�{��)*/
	{
		h->obj_type_set 				= BOSS_00_11;	/*(�Ē�`�̕K�v����)*/
		h->callback_mover				= root_boss_mover;
		h->callback_hit_teki			= NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	//
		h->BOSS_DATA_03_kougeki_anime_count 		= (0);	/* �U���A�j���[�V�����p�J�E���^ / 0�ȉ��Ȃ�ړ��A�j���[�V���� */
		h->BOSS_DATA_05_boss_base_state777			= (0);	/*ST_00*/	/*(�����l��0�ɂ���B) (-1) */
		//------------ �J�[�h�֘A
		{
			static const u16 boss_start_card_address[(8)] =/* �J�[�h�A�h���X��256��ނ𒴂����̂ōŒ�ł� u16 �͕K�v�B */
			{	/* �{�X�J�n�J�[�h�ԍ�(easy) */
				CARD_ADDRESS_00E_chrno_000, 	/* �`���m(?) */ 	/* �G�L�X�g���p(boss0) */
				CARD_ADDRESS_00E_alice_000, 	/* �A���X */
				CARD_ADDRESS_00E_mima_000,		/* ���� */
				CARD_ADDRESS_00E_kaguya_000,	/* �P�� */
			//
				CARD_ADDRESS_00E_aya_000,		/* �� */
				CARD_ADDRESS_00E_pacheA_000,	/* �p�`�F */
				CARD_ADDRESS_00E_sakuya_000,	/* ��� */
				CARD_ADDRESS_00E_pacheA_000,	/* �t����(?) */ 	/* �t�@���^�Y���p(boss7) */
			};
			card.address_set =
				#if (1)/*(�{�X���Ⴆ�΃X�y�J���Ⴄ)*/
				boss_start_card_address[boss_number]	/*(��ʒu)*/
				#endif
				#if (1)/*(��Փx�ʂɈႤ�X�y�J������)*/
				+ ((cg.game_difficulty)&0x03);	/*(��Փx�ʃI�t�Z�b�g)*/
				#endif
			if (5==boss_number)/*(5==�p�`�F�̏ꍇ�A�v���C���[���ɈႤ�X�y�J������)*/	/*(�p�`�F�̏ꍇ�I�t�Z�b�g)*/
			{
				u8 aaa;
				aaa = ((((cg_game_select_player)&(4-1)))<<2);/* 4��(0 ... 3)�ɐ������Ă���A4�{����B(rank == E,N,H,L) */
				card.address_set += ((aaa<<3)+(aaa));/*(aaa *= 9; 9�{����Br36_gekiha �܂߃X�y�J�̈� 9 ��ށB)*/
			}
			h->base_hp				= (my_card_resource[(card.address_set)].spell_life);	/* �S�̗̂̑� */	/*(�Ē�`�̕K�v����)*/
			/*(����̋K��l)*/
			card.limit_health			= (h->base_hp);
			card.mode_timer 			= (CARD_BOSS_TIMER_0000_HATUDAN);/*on*/ 	/* ��b�`�� */	/* ����H (CARD_BOSS_TIMER_0255_IDO_JYUNNBI) */
			card.spell_used_number		= SPELL_00; 	/* �J�[�h�����I���t���O */
			card.address_temporaly		= (0);			/*(SPELL_08_rumia-1)*/ /*0*/
			#if (0)//(1)
			card_set_boss_move_callback();
			#else/*(���Ԃ�A����)*/
			card.boss_move_card_callback = kaiwa_00_keitai;/*(�K���A��b0�`�Ԃ���n�܂�B)*/
			#endif
		}
	}
	/*(���~�b�g������)*/		/*(����U��)�̍U�������� */
	set_new_limit();			/* �ʏ�U��(����U��)�̍U���� */
	// �{�X�e�N�X�`���ǂݍ���
	stage_boss_load_texture();

	#if (0)/*(�f�o�b�O)*/
	kanji_window_clear_line(0); 			/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
	kanji_cursor_move_home_position();		/* �J�[�\����1�s��(==0)�ֈړ� */
	{
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_02_JIKI_SPELL_CARD);/* [��/���@�J�[�h�p�w�i]����Ӕw�ion */
		cg.msg_time 				= (65536);	/* �� 18 �� */
		strcpy(my_font_text, "�{�X�A���[�h������B" "\n");
		kanji_color((7)|STR_CODE_NO_ENTER);
		kanji_draw();
	}
	#endif
	//
	#if (0)/*(�f�o�b�O)*/
	{	psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load stage: STAGE%d" "\\n"
			"�{�X�A���[�h�e�X�gOK�B", cg.game_now_stage);
	}
	#endif
	/*---------------------------------------------------------
		�udat�Őݒ肵���{�X�o���ʒu�v����A
		�u�{�X�o���Œ�ʒu�v�܂ňړ�����B
		�悤�ɏ����ݒ���s���B
	---------------------------------------------------------*/
	// �o�����W�̏����ݒ�B
	REG_02_DEST_X	= (BOSS_XP256);/*(t256()�`���A�{�X�o���Œ�ʒu)*/
	REG_03_DEST_Y	= (t256(16.0));/*(t256()�`���A�{�X�o���Œ�ʒu)*/
	boss_set_new_position(h);/*(�U���ړ����W��ݒ�)*/
	//
}
