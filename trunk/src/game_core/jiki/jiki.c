
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�e�v���C���[
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
	���� �얲 A (�아)					(�͂��ꂢ �ꂢ��)
	���� �얲 B (����)					(�͂��ꂢ �ꂢ��)
	���J ������ A (����)				(���肳�� �܂肳)
	���J ������ B (����)				(���肳�� �܂肳)
	���~���A �X�J�[���b�g				(Remilia Scarlet)
	����̖S��			���s�� �H�X�q	(�������傤�� ��䂱)
	�H���O�t�̖S�쏭��	���s�� �H�X�q	(�������傤�� ��䂱)
	�`���m A (�X��) 					(Cirno)
	�`���m �H (�H��)					(Cirno)
---------------------------------------------------------*/

#include "jiki_local.h"
#include "kanji_system.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1/* ���Ԃ񂱂̈ʒu�ŁB */
//global   signed int cg.game_difficulty;	/* GAME_CORE_GLOBAL_CLASS�ɁA�Ӑ}�I�ɓ���Ȃ����� */
global unsigned int cg_game_select_player;	/* GAME_CORE_GLOBAL_CLASS�ɁA�Ӑ}�I�ɓ���Ȃ����� */
global GAME_CORE_GLOBAL_CLASS __attribute__((aligned(32))) cg;
#endif


/*---------------------------------------------------------
	�G��T���q�֐�
---------------------------------------------------------*/

/* static */global OBJ *search_teki_from_obj(void)
{
	int ii;
	for (ii=0; ii<OBJ_POOL_01_TEKI_0256_MAX; ii++)/* �S�����ׂ�B */
	{
		OBJ *h;
		h = &obj99[OBJ_HEAD_01_0x0800_TEKI+ii];
	//
		if (
			#if 1
			(JYUMYOU_NASI <= h->jyumyou) && /* �폜�ς݂͔�΂� */
			#endif
			(0 != (h->obj_type_set & (OBJ_Z02_TEKI )) ) 	/* �v���C���[�ɂƂ��Ă̓G(�U�R��{�X) */
		)
		{
			{
				#if 1/* Gu(���S���W) */
				if ((h->cx256 > 0) && (h->cx256 < t256(GAME_WIDTH ) ) &&
					(h->cy256 > 0) && (h->cy256 < t256(GAME_HEIGHT) ))	/* �\����ʓ��̓G */
				#endif
				{
					return (h);/* �������� */
				}
			}
		}
	}
	return (&obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER]);/* ������Ȃ����� */
}


/*---------------------------------------------------------
	�J�[�h�������̗����G
---------------------------------------------------------*/

static void player_move_tachie(OBJ *src)
{
	if ((128-1) < cg.bomber_time)/* �c�莞�Ԃ� 128 �ȏ�Ȃ�A�ړ����đҋ@ */
	{
		/* ��ʊO����o���A��~�B���΂��҂B */
		if (t256(100) > src->cx256)
		{
			src->cx256 += t256(5.0);
		}
	}
	else
	if ((1/*-1 */) < cg.bomber_time)/* �g�� */
	{
		/* (24+(1))  �A�����ł� cg.bomber_time �́A�ő�0x7f�Ȃ̂ŁB */
		src->color32		= (cg.bomber_time<<(24+(1)))|0x00ffffff;
		src->m_zoom_x256	+= (10);
		src->m_zoom_y256	+= (10);
	}
	else
	{
		src->jyumyou = JYUMYOU_NASI;	/* ���Ԑ����ł����܂� */
	}
}


/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------- */

/*---------------------------------------------------------
	�v���C���[�e(�V���b�g/�j���p)�����A�q�֐�
	�v���C���[�A�j�e(�얲)�̒ǉ�
		(�얲)		[�j�e]�A
		(������)	[�X�e]�A
		(���~���A)	[�R�E�����e]�A
		(�H�X�q)	[���e] �A
		(���`���m)	[�ەX�e]�A
---------------------------------------------------------*/


/*
	cg.jiki_weapon_level_offset == player�ԍ� + (���탌�x�� x 8)
	cg.jiki_weapon_level_offset =  (cg_game_select_player) + (weapon_level<<3);
 */



/*---new add-----*/


/*---------------------------------------------------------
	�v���C���[�����G����
	(�H�X�q)[�H�X�q�{���̐�{��]
---------------------------------------------------------*/

static void player_create_bomber_tachie(void/* OBJ *src */)
{
	OBJ *h;
	h					= obj_add_A01_teki_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* (?) */
	{
		h->m_Hit256R			= JIKI_ATARI_ITEM_80;
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);
		h->callback_mover		= player_move_tachie;
		h->obj_type_set 				= (JIKI_BOMBER_00|SP_GROUP_SHOT_ZAKO);	/* OBJ_Z01_JIKI_GET_ITEM */ /* JI KI_SHOT_00 */ /* �{�X�̒��ڍU���͋֎~ */	/* (OBJ_Z01_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO) �{�X�ɖ��� */
		/* �����G�����ʒu */
		h->cx256				= -t256(150);	// (200)	/* 0 */  /* (src->cx256) */
		h->cy256				=  t256(150);	// (100)	/* GAME_HEIGHT+1 */ /* (src->cy256) */
	//
		/* �`��p�p�x */
		h->rotationCCW1024		= (0);//(256);
	//
	/* ??? */	h->kougeki_ti	= (0); /* �����G�Ȃ̂ŋ���0. */ 	/*1  8*1 */ 	/* ��{�� �̋��� */
	}
}


enum /* _player_state_ */
{
	//...
	LIMIT_m64_MINUS_LIMIT		= (-64),// LIMIT_m64_MINUS_LIMIT		= (-255),
	LIMIT_fukki2				= (-48),// ���A2 < LIMIT_fukki2
	LIMIT_m127_MIN				= (-32),// ���A1 < LIMIT_m127_MIN		= (-127),
	LIMIT_m16_GAME_OUT			= (-16),
	LIMIT_m1_KURAI_BOMB_UKETUKE = (-1),
	LIMIT_0_ZERO				= (0),
	LIMIT_1_ONE 				= (1),
//	LIMIT_128					= (128),
//	LIMIT_256					= (256),
	LIMIT_300_CONTINUE			= (300),	/* ��5[�b]		�R���e�B�j���[�����ꍇ�̕��A����		5.00[�b] == 300 / 60. */
	LIMIT_512_STAGE_START		= (512),	/* ��8.53[�b]	�X�e�[�W�J�n���̖��G����(���ʂɒ���)	8.53[�b] == 512 / 60. */
	LIMIT_65535_BOSS_TAOSITA				= (65535),	/* ��18.2[��]	�{�X�|�����ꍇ�̖��G����(���ʂɒ�����)	18.2[��] == 65535 / 60 / 60. */
};

/* 0==����C�x���g�������Ă��Ȃ��B */
static signed int pds_status_timer; /* ����C�x���g�������^�C�}�[ */
//static int cg.special_ivent_type; /* ����C�x���g�̎�� */


extern void reimu_yuyuko_create_bomber_kekkai_parrent(OBJ *src); 			/* �얲��p �{���̐e */
//extern void yuyuko_create_bomber_gyastry_dream_parrent(OBJ *src);		/* �H�X�q��p */
#define yuyuko_create_bomber_gyastry_dream_parrent reimu_yuyuko_create_bomber_kekkai_parrent

extern void marisa_create_bomber_master_spark_parrent(OBJ *src); 		/* ������ B��p �{���̐e */
extern void chrno_q_create_bomber_gyastry_dream_parrent(OBJ *src);		/* �`���m�H��p */
extern void marisa_create_bomber_star_dust_levarie_parrent(OBJ *src);	/* ������ A��p �ᑬ�{���̐e */
extern void remilia_create_bomber_cross_shot_parrent(OBJ *src);			/* ���~���A�{���̐e */
extern void yuyuko_create_bomber_meifu_parrent(OBJ *src);				/* �H�X�q��p �ᑬ�{���̐e */
extern void marisa_create_bomber_homing_parrent(OBJ *src);				/* �ᑬ�{���̐e */
extern void reimu_create_bomber_homing_shot_parrent(OBJ *src);			/* �얲��p �ᑬ�{���̐e */
/*---------------------------------------------------------
	�v���C���[�L�[����
---------------------------------------------------------*/

extern void set_bg_alpha(int set_bg_alpha);
extern void register_main_shot(OBJ *s1);

extern int jiki_shot_get_player_animation_speed(void);/* �v���C���[�̃A�j���[�V�������x��ݒ� */
extern void jiki_shot_oya_hide(void);/*(���@�V���b�g�̐e���\���ɂ���B)*/

//enum /* _player_state_ */
//{
//	PLAYER_STATE_00_NORMAL,
//	PLAYER_STATE_01_KURAI_BOMB_UKETUKE,
//	PLAYER_STATE_02_SAVE_01,
//	PLAYER_STATE_03_SAVE_02,
//	PLAYER_STATE_04_GAME_OUT,
//};
extern void jiki_shot_set_bomber_time(void);/*(�v���C���[�B �{���̎�����ݒ�)*/
static void player_keycontrol(OBJ *s1)
{
	/*---------------------------------------------------------
		�v���C���[�{���֘A����
	---------------------------------------------------------*/
//	if (STATE_FLAG_0x0200_IS_KAIWA_MODE==(cg.state_flag & STATE_FLAG_0x0200_IS_KAIWA_MODE))
//	{
//		;	/* ��b�C�x���g���̓{���֘A�̏����͂��Ȃ� */
//	}
//	else
	/*(�{���o�[�L�����Ԓ�)*/
	if (0 < cg.bomber_time) 	/* �{���E�F�C�g���� */
	{
		/* �V�i���I���ɂ����@�{���o�[���Ԍo�߂���B(����my_special()�Ŏ��@�{���I�����Ȃ��ƃV�i���I�ǂ܂Ȃ��d�l�̈�) */
		cg.bomber_time--; /*fps_factor*/
		if (2 > cg.bomber_time)
		{
			set_bg_alpha(255);/* ��ʂ𖾂邭���� */
			jiki_shot_oya_hide();/*(���@�V���b�g�̐e���\���ɂ���B)*/
	//	}
	//	else
	//	if (190 > cg.bomber_time)
	//	{
			cg.state_flag &= (~JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU);	/* �{���ɂ�鎩�����W�͏I��� */
		}
	}
	/*(�{���o�[�������Ԓ�)*/
	else
//	if (0 >= cg.bomber_time)
	{
		cg.bomber_time = 0;
		/* �V�i���I���ɂ͗U���e��ǉ����Ȃ� */
		if (IS_SCENE_KAIWA_TATAKAU)
		{
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
			{
				if (0 < cg.bombs)/*(�c�{���o�[������ꍇ)*/
				{
					cg.bombs--;
					cg.player_data_used_bomber++;	/* �W�v�V�X�e��(player_data)�{���g�p�� */
					cg.state_flag	|= JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU; /* �{���ɂ�鎩�����W�\ */
					voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/* �e�L�g�[ */
					set_bg_alpha(50);/* (100);(127);��ʂ��Â����� */
					/* �����G */
					player_create_bomber_tachie(/* obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER] */);
					//
					//
					static /* const */ void (*regist_call_table[(PLAYERS8*2)])(OBJ *src) =/* 16 */
					{
						reimu_create_bomber_homing_shot_parrent,				reimu_yuyuko_create_bomber_kekkai_parrent,	/* �얲 A */
						reimu_yuyuko_create_bomber_kekkai_parrent,				marisa_create_bomber_homing_parrent,		/* �얲 B */
						marisa_create_bomber_star_dust_levarie_parrent, 		marisa_create_bomber_homing_parrent,		/* ������ A */
						marisa_create_bomber_master_spark_parrent,				marisa_create_bomber_homing_parrent,		/* ������ B */
						remilia_create_bomber_cross_shot_parrent,				marisa_create_bomber_homing_parrent,		/* ���~���A */
						yuyuko_create_bomber_gyastry_dream_parrent, 			yuyuko_create_bomber_meifu_parrent, 		/* �H�X�q */
						marisa_create_bomber_star_dust_levarie_parrent, 		reimu_create_bomber_homing_shot_parrent,	/* �`���m A */
						chrno_q_create_bomber_gyastry_dream_parrent,			marisa_create_bomber_homing_parrent,		/* �`���m Q */
					};
						int index_aaa;
						index_aaa = (((cg_game_select_player))+((cg_game_select_player))+((psp_pad.pad_data & PSP_KEY_SLOW)?1:0));
					{
						(regist_call_table[(index_aaa)])(&obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER]);
					}
					s1->color32 				= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/* ������ */
					{
						OBJ *zzz_obj_maru;
						zzz_obj_maru = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU];
						zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0x88, 0x88, 0x50);	/* ������ */
					}
//					if (PLAYER_STATE_01_KURAI_BOMB_UKETUKE==cg.special_ivent_type)/* ��炢�{����t�� */
					if (LIMIT_0_ZERO > pds_status_timer)/* ��炢�{����t�� */
					{
						cg.player_data_use_kurai_bomb++;/* �W�v�V�X�e��(player_data)��炢�{������ */
					}
//					pds_status_timer = LIMIT_128;// cg.special_ivent_type	= PLAYER_STATE_03_SAVE_02;
					/* cg.bomber_time �� ->jyumyou�œ����ɓ��� */
					jiki_shot_set_bomber_time();
					//	#define USER_BOMOUT_WAIT (30)
					pds_status_timer	= cg.bomber_time + USER_BOMOUT_WAIT;/* �������G���� */
					/*	�������G���� == (�ݒ薳�G���� + USER_BOMOUT_WAIT)�B 	�������G���� == �ݒ薳�G���� ���ƁA�킩��ɂ����̂Ŏ኱�̗]�T���K�v�B */
				//
					kanji_window_clear_line(0); 			/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
					kanji_cursor_move_home_position();		/* �J�[�\����1�s��(==0)�ֈړ� */
					{
						ml_font[(0)].haikei 		= (ML_HAIKEI_02_JIKI_SPELL_CARD);/* [��/���@�J�[�h�p�w�i]����Ӕw�ion */
						cg.msg_time 				= cg.bomber_time;	/* byou60(5) */ /* �� 5 �b */
						typedef struct
						{
							const char *spell_str_name; 	/* �J�[�h���� */
						} aaa_RESOURCE;
						static const aaa_RESOURCE my_aaa_resource[(PLAYERS8*2)] =
						{
//	 "feeddccbbaa99887766554433221100",'\0' 	 "feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
	{"�아�u���z����v" 				"\n"},	{"�P���u���z���E�v" 				"\n"},//�아�u���z���E�E�~�v
	{"�����u���z���E�v" 				"\n"},	{"�~���u���z�����v" 				"\n"},//�����u���z���E�E�a�v
	{"�����u�X�^�[�_�X�g�����@���G�v"	"\n"},	{"�M���u���쐬�U���v"				"\n"},
	{"�����u�}�X�^�[�X�p�[�N�v" 		"\n"},	{"�����u���쐬�U���v"				"\n"},
	{"�g���u�s���u���b�h�v"			"\n"},	{"�����u�A�N�}�N���X�v" 			"\n"},
	{"�����u�M���X�g���h���[���v"		"\n"},	{"���Z�u�����v" 				"\n"},// ���A�u���v�A�u�Ŋ��v�B�u���v���́B�ł��u�̂Ă�v�B /* dream cast. ghastly dream(�d�X��). ghastly dream(�얲). */
	{"�ŕ��u�A�C�V�N���X�g���C�N�v" 	"\n"},	{"�X���u�I���U���b�N�v" 			"\n"},	/* ��(�����): �J����������A�n���������Ⴊ�Ăѓ����č~��B */
	{"�ŋ��u�A�^�C�̌��v"				"\n"},	{"��蹁u�㕶�L�b�N�v"				"\n"},	/* �� ���� ��(�݂���): �J�ƐႪ�������č~��B */
						};
						strcpy(my_font_text, my_aaa_resource[index_aaa].spell_str_name);
						kanji_color((7)|STR_CODE_NO_ENTER);
						kanji_draw();
					}
				}
			}
		}
	}

//
//	if (PLAYER_STATE_01_KURAI_BOMB_UKETUKE==cg.special_ivent_type) return;/* ��炢�{����t�� */
	if (LIMIT_0_ZERO > pds_status_timer)	{	return; 	}	/* ��炢�{����t�� */
//
	/*---------------------------------------------------------
		�v���C���[�ړ��֘A����
	---------------------------------------------------------*/
	#if (0)
	/*
		�g����
		�d�X��
		�i�鏴
		�ԉf��
		���Ԓ�
		���_�^0.02a
	*/
	static const s16 jiki_move_length[16][2] =
	{
	/* LDRU */ /* y x */
	// �΂߈ړ��������̂��������̂��C���B
	/* 0000 */ {	  0,	0},
	/* 0001 */ {	  0, -256},/* 0 U */
	/* 0010 */ {	256,	0},/* 2 R */
	/* 0011 */ {	256, -256},/* 1 UR */
	/* 0100 */ {	  0,  256},/* 4 D */
	/* 0101 */ {	  0,	0},
	/* 0110 */ {	256,  256},/* 3 DR */
	/* 0111 */ {	  0,	0},
	/* 1000 */ {   -256,	0},/* 6 L */
	/* 1001 */ {   -256, -256},/* 7 UL */
	/* 1010 */ {	  0,	0},
	/* 1011 */ {	  0,	0},
	/* 1100 */ {   -256,  256},/* 5 DL */
	/* 1101 */ {	  0,	0},
	/* 1110 */ {	  0,	0},
	/* 1111 */ {	  0,	0},
	};
	/* �΂߈ړ��� ���_�^0.02a �܂ł� �l�����Ȃ��B */
	/*
		�g���� ���� ���_�^0.02a �܂ł́A�΂߈ړ��������B
		�͕핗�́A�����̏ꏊ�������̂ŁA��������Ȃ�{���΂߈ړ��𑬂�����ׂ��B
		�������Apsp�̉�ʂ͏������ׁA�΂߈ړ��������̂��ڗ��B
		�΂߈ړ��������Ƃ������ăo�����X�������̂ŁA�͕핗�ł͎΂߈ړ���x������B(�����ɍ��킹��)
		���������Ė͕핗�ł͖{���I�� �g���� ���� ���_�^0.02a �܂ł̃Q�[���o�����X�͍Č��ł��Ȃ������d�l�Ƃ���B
		����͉��ɒe���������ꍇ�ł��A�Ⴄ����(�Ⴄ������)���o���Ă��܂��ׁB
	*/
	#else
	/* ���_�^ ��0.02a�ȍ~ */
	/*
		256�Œ菬���_�Ŏ��@�̈ړ���
		1.0[pixel]	 == 256 == t256(1.0),			�c���̏ꍇ
		��0.7[pixel] == 181 == t256(1.0/��2.0)		�΂߂̏ꍇ
	 */
	static const s16 jiki_move_length[16][2] =
	{
	/* LDRU */ /* y x */
	// �΂߈ړ������߂���̂��C���B
	/* 0000 */ {	  0,	0},
	/* 0001 */ {	  0, -256},/* 0 U */
	/* 0010 */ {	256,	0},/* 2 R */
	/* 0011 */ {	181, -181},/* 1 UR */
	/* 0100 */ {	  0,  256},/* 4 D */
	/* 0101 */ {	  0,	0},
	/* 0110 */ {	181,  181},/* 3 DR */
	/* 0111 */ {	  0,	0},
	/* 1000 */ {   -256,	0},/* 6 L */
	/* 1001 */ {   -181, -181},/* 7 UL */
	/* 1010 */ {	  0,	0},
	/* 1011 */ {	  0,	0},
	/* 1100 */ {   -181,  181},/* 5 DL */
	/* 1101 */ {	  0,	0},
	/* 1110 */ {	  0,	0},
	/* 1111 */ {	  0,	0},
	};
	/*
		psp�̉�ʂ͏������ׁA�΂߈ړ��������̂��ڗ��B(�����炭��ʂ��傫����΁A�]��C�ɂȂ�Ȃ�)
		(�g���� ���� ���_�^0.02a �܂ł́A�΂߈ړ��������B���A�]��C�ɂȂ�Ȃ�)
		�΂߈ړ��������̂���a��������̂ŁA�΂߈ړ��͕␳����B
	*/
	#endif






/* �ԉf�ˉ�b������T�C�g http://ambriel.hp.infoseek.co.jp/th9/  ��{���\��r �ړ����x(�������[�h)
														[51200/xxxF]
200/50		50F ��										0x0400[1024.000000000000000000000000000000]3.63636363636363636363636363636364
200/55		55F [������] �~�X�e�B�A (���f�B,  �l�G) 	0x03a3[ 930.909090909090909090909090909091]3.63636363636363636363636363636364
200/58		58F [�`���m]								0x0373[ 882.758620689655172413793103448276]3.44827586206896551724137931034483
200/61		61F (���,	���i�T) �Ă�					0x0347[ 839.344262295081967213114754098361]3.27868852459016393442622950819672		[REMILIA]
200/65		65F �����J									0x0314[ 787.692307692307692307692307692308]3.07692307692307692307692307692308
200/67		67F ����									0x02fc[ 764.179104477611940298507462686567]2.98507462686567164179104477611940
200/68		68F [�얲] ��� 							0x02f1[ 752.941176470588235294117647058824]2.94117647058823529411764705882353
200/72		72F ��������								0x02c7[ 711.111111111111111111111111111111]2.77777777777777777777777777777778		[YUYUKO]
200/91		91F �H��									0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220

�z��ړ����x(�ᑬ���[�h)
200/79		79F 	��									0x0288[ 648.101265822784810126582278481013]2.53164556962025316455696202531646
200/80		80F 	�~�X�e�B�A							0x0280[ 640.000000000000000000000000000000]2.50000000000000000000000000000000
200/83		83F 	[�`���m]							0x0269[ 616.867469879518072289156626506024]2.40963855421686746987951807228916
200/85		85F 	���								0x025a[ 602.352941176470588235294117647059]2.35294117647058823529411764705882
200/91		91F 	[������]							0x0233[ 562.637362637362637362637362637363]2.19780219780219780219780219780220
200/109 	109F	(���,	���i�T) 					0x01d6[ 469.724770642201834862385321100917]1.83486238532110091743119266055046		[REMILIA]
200/114 	114F	(���f�B,  �l�G) 					0x01c1[ 449.122807017543859649122807017544]1.75438596491228070175438596491228
200/124 	124F	�����J ���� 						0x019d[ 412.903225806451612903225806451613]1.61290322580645161290322580645161
200/130 	130F	�d��								0x018a[ 393.846153846153846153846153846154]1.53846153846153846153846153846154
200/136 	136F	�������� [�얲] 					0x0178[ 376.470588235294117647058823529412]1.47058823529411764705882352941176
200/145 	145F	�Ă�								0x0161[ 353.103448275862068965517241379310]1.37931034482758620689655172413793		[YUYUKO]
200/148 	148F	�H��								0x015a[ 345.945945945945945945945945945946]1.35135135135135135135135135135135

 */
/*
http://wikiwiki.jp/let/?etc%2F%C3%CE%BC%B1%2F%BC%AB%B5%A1%A4%CE%B0%DC%C6%B0%C2%AE%C5%D9

���x�̒P�ʂ̓h�b�g���t���[���B

(����)
�L����		���� �ᑬ
(�g����)
�얲		4.0 2.0
������		5.0 2.5
-------------------
(�d�X��)
�얲		4.0 1.6
������		5.0 2.0
���		4.0 2.2
-------------------
(�i�鏴)
���E�g		4.0 2.0 (�얲+��)
�r���g		5.0 2.2 (������+�A���X)
�g���g		4.0 2.3 (���+���~���A)
���E�g		5.0 1.9 (�H�X�q+�d��)
-------------------
�얲		4.0 2.0 ��			4.0 2.0
������		5.0 2.2 �A���X		4.0 2.2 (�������֍��킹��)
���		4.0 2.3 ���~���A	5.0 2.3 (�x�����֍��킹��)
�d��		5.0 1.9 �H�X�q		4.0 1.9 (�������֍��킹��)
-------------------
(�ԉf��)
��			5.5 3.5

�~�X�e�B�A	5.0 3.4
������		5.0 3.0
���f�B�X��	5.0 2.4
�l�G�f�P	5.0 2.4
�d��		5.0 2.1

�`���m		4.7 3.3

���		4.5 2.5
���i�T		4.5 2.5
�Ă�		4.5 1.9

�����J		4.2 2.2
����		4.1 2.2

���		4.0 3.2
�얲		4.0 2.0

��������	3.8 2.0
�H��		3.0 1.8
-------------------
(���Ԓ�)
��			5.0 2.3 0.5(������)[*1] [*1] ��������0.5
-------------------
(���_�^0.02a)
�얲		4.5 2.0
������AB	5.0 2.0
������C 	5.0 3.0
-------------------
(���_�^)
�얲		4.5 (��3.18)[*2] 2.0 (��1.41)[*3]
������		5.0 (��3.53)[*4] 2.0 (��1.41)[*5]

[*2] �΂߈ړ��̏ꍇ�Ax��������y��������3.18�h�b�g�ړ�[*3] �΂߈ړ��̏ꍇ�Ax��������y��������1.41�h�b�g�ړ�
[*4] �΂߈ړ��̏ꍇ�Ax��������y��������3.53�h�b�g�ړ�[*5] �΂߈ړ��̏ꍇ�Ax��������y��������1.41�h�b�g�ړ�
*/


//#define BASE_SPEED_MINIMUM		(PLAYERS8*4)
//#define BASE_SPEED_MAXIMUM		(PLAYERS8*5)
//	 2,  3,  4,  7,  3, 	/* �Œᑬ�x player_speed_minimum */
//	 4,  5,  4,  7,  3, 	/* �ō����x player_speed_maximum */

	/* �������[�h(�ʏ펞) */					/* �������[�h(�{��������) */
	#if (1)/*(�͕핗)*/
		#define SPEED_HIGH_REIMU_A		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
		#define SPEED_HIGH_REIMU_B		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
		#define SPEED_HIGH_MARISA_A 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
		#define SPEED_HIGH_MARISA_B 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
		#define SPEED_HIGH_REMILIA		(0x0347)	/* t256(3.28) 3.27734375		  t256(5.0), */
		#define SPEED_HIGH_YUYUKO		(0x02c7)	/* t256(2.78) 2.77734375		  t256(5.0), */
		#define SPEED_HIGH_CIRNO_A		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 	/* �H������ᑬ�̕������� */
		#define SPEED_HIGH_CIRNO_Q		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 	/* �H������ᑬ�̕������� */
												/* �ᑬ���[�h(�ʏ펞) */							/* �ᑬ���[�h(�{��������) */
		#define SPEED_LOW_REIMU_A		(0x0178)	/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
		#define SPEED_LOW_REIMU_B		(0x0178)	/* t256(1.47) 1.46875000 t256(2.0) t256(2.0), */
		#define SPEED_LOW_MARISA_A		(0x0233)	/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
		#define SPEED_LOW_MARISA_B		(0x0233)	/* t256(2.20) 2.19921875 t256(2.5) t256(2.0), */
		#define SPEED_LOW_REMILIA		(0x01d6)	/* t256(1.84) 1.83593750 t256(2.5) t256(4.0), */
		#define SPEED_LOW_YUYUKO		(0x0161)	/* t256(1.38) 1.37890625 t256(2.0) t256(4.5), */
		#define SPEED_LOW_CIRNO_A		(0x0269)	/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 �H������ᑬ�̕������� 0x0400==��������==���z���ő��H */
		#define SPEED_LOW_CIRNO_Q		(0x0700)	/* t256(2.41) 2.41015625 t256(7.0) t256(5.0), */ /* 0x0700 �H������ᑬ�̕������� 0x0400==��������==���z���ő��H */
	#else/*(������̃^�C�v�̕�����Җ{���̃C���[�W�ɋ߂��̂����m��Ȃ����Apsp�ł͖ʔ����Ȃ��B)*/
		#define SPEED_HIGH_REIMU_A		(0x0300)	/* t256(3.000) 3.000 == 4.0x0.75 (�g����, �d�X��, �i�鏴, �ԉf��) */
		#define SPEED_HIGH_REIMU_B		(0x0300)	/* t256(3.000) 3.000 == 4.0x0.75 (�g����, �d�X��, �i�鏴, �ԉf��) */
		#define SPEED_HIGH_MARISA_A 	(0x03c0)	/* t256(3.750) 3.750 == 5.0x0.75 (�g����, �d�X��, �i�鏴, �ԉf��, ���_�^) */
		#define SPEED_HIGH_MARISA_B 	(0x03c0)	/* t256(3.750) 3.750 == 5.0x0.75 (�g����, �d�X��, �i�鏴, �ԉf��, ���_�^) */
		#define SPEED_HIGH_REMILIA		(0x03c0)	/* t256(3.750) 3.750 == 5.0x0.75 ( �i�鏴[�P] ) (4.0==�i�鏴[�g],  ��� < ���~���A) */
		#define SPEED_HIGH_YUYUKO		(0x0300)	/* t256(3.000) 3.000 == 4.0x0.75 ( �i�鏴[�P] ) (5.0==�i�鏴[�g],  �H�X�q < �d��) */
		#define SPEED_HIGH_CIRNO_A		(0x0386)	/* t256(3.525) 3.525 == 4.7x0.75 (�ԉf��)  (�A���ԉf�˂͔ԊO�҂Ȃ̂ŁA�{�҂ŏo���Ƃ���ƈႤ�����ɂȂ�\���̕�������)  */
		#define SPEED_HIGH_CIRNO_Q		(0x0386)	/* t256(3.525) */	/* �H������ᑬ�̕������� */
												/* �ᑬ���[�h(�ʏ펞) */							/* �ᑬ���[�h(�{��������) */
		#define SPEED_LOW_REIMU_A		(0x0180)	/* t256(1.500) 1.500 == 2.0x0.75 (�g����, �i�鏴, �ԉf��, ���_�^) (1.60�d�X��) */
		#define SPEED_LOW_REIMU_B		(0x0180)	/* t256(1.500) 1.500 == 2.0x0.75 (�g����, �i�鏴, �ԉf��, ���_�^) (1.60�d�X��) */
		#define SPEED_LOW_MARISA_A		(0x01e0)	/* t256(1.875) 1.875 == 2.5x0.75 (2.50�g����) (2.00�d�X��) (2.20�i�鏴) (3.00�ԉf��) (2.00���_�^) (3.00���_�^C) */
		#define SPEED_LOW_MARISA_B		(0x0240)	/* t256(2.250) 2.250 == 3.0x0.75 (2.50�g����) (2.00�d�X��) (2.20�i�鏴) (3.00�ԉf��) (2.00���_�^) (3.00���_�^C) */
		#define SPEED_LOW_REMILIA		(0x01b9)	/* t256(1.725) 1.725 == 2.3x0.75 ( �i�鏴[�P] ) (2.3==�i�鏴[�g],  ��� < ���~���A) */
		#define SPEED_LOW_YUYUKO		(0x016c)	/* t256(1.425) 1.425 == 1.9x0.75 ( �i�鏴[�P] ) (1.9==�i�鏴[�g],  �H�X�q < �d��) */
		#define SPEED_LOW_CIRNO_A		(0x0279)	/* t256(2.475) 2.475 == 3.3x0.75 (�ԉf��)  (�A���ԉf�˂͔ԊO�҂Ȃ̂ŁA�{�҂ŏo���Ƃ���ƈႤ�����ɂȂ�\���̕�������)  */
		#define SPEED_LOW_CIRNO_Q		(0x0700)	/* t256(7.000) �H������ᑬ�̕������� 0x0580==��������==���z���ő��H */
	#endif

	#define PLAYER_WIDTH			(50)	/*	�Œ�T�C�Y */
	#define PLAYER_HEIGHT			(50)	/*	�Œ�T�C�Y */

	static const signed /* int */short player_speed256[(PLAYERS8*4)] =
	{	/* �������[�h(�ʏ펞) */	/* �������[�h(�{��������) */
		SPEED_HIGH_REIMU_A, 		/* REIMU_A */
		SPEED_HIGH_REIMU_B, 		/* REIMU_B */
		SPEED_HIGH_MARISA_A,		/* MARISA_A */
		SPEED_HIGH_MARISA_B,		/* MARISA_B */
		SPEED_HIGH_REMILIA, 		/* REMILIA */
		SPEED_HIGH_YUYUKO,			/* YUYUKO */
		SPEED_HIGH_CIRNO_A, 		/* CIRNO_A */
		SPEED_HIGH_CIRNO_Q, 		/* CIRNO_Q */
		/* �ᑬ���[�h(�ʏ펞) */	/* �ᑬ���[�h(�{��������) */
		SPEED_LOW_REIMU_A,			/* REIMU_A */
		SPEED_LOW_REIMU_B,			/* REIMU_B */
		SPEED_LOW_MARISA_A, 		/* MARISA_A */
		SPEED_LOW_MARISA_B, 		/* MARISA_B */
		SPEED_LOW_REMILIA,			/* REMILIA */
		SPEED_LOW_YUYUKO,			/* YUYUKO */
		SPEED_LOW_CIRNO_A,			/* CIRNO_A */
		SPEED_LOW_CIRNO_Q,			/* CIRNO_Q */
	#if 0/* [�}�X�^�[�X�p�[�N���́A���x�����B] */
		/* �������[�h(�ʏ펞) */	/* �������[�h(�{��������) */
		SPEED_HIGH_REIMU_A, 		/* REIMU_A */
		SPEED_HIGH_REIMU_B, 		/* REIMU_B */
		SPEED_HIGH_MARISA_A,		/* MARISA_A */
		(SPEED_HIGH_MARISA_B>>1),	/* MARISA_B */
		SPEED_HIGH_REMILIA, 		/* REMILIA */
		SPEED_HIGH_YUYUKO,			/* YUYUKO */
		SPEED_HIGH_CIRNO_A, 		/* CIRNO_A */
		SPEED_HIGH_CIRNO_Q, 		/* CIRNO_Q */
		/* �ᑬ���[�h(�ʏ펞) */	/* �ᑬ���[�h(�{��������) */
		SPEED_LOW_REIMU_A,			/* REIMU_A */
		SPEED_LOW_REIMU_B,			/* REIMU_B */
		SPEED_LOW_MARISA_A, 		/* MARISA_A */
		(SPEED_LOW_MARISA_B>>1),	/* MARISA_B */
		SPEED_LOW_REMILIA,			/* REMILIA */
		SPEED_LOW_YUYUKO,			/* YUYUKO */
		SPEED_LOW_CIRNO_A,			/* CIRNO_A */
		SPEED_LOW_CIRNO_Q,			/* CIRNO_Q */
	#endif
	};
	/* ���@���x�����߂�B */
	unsigned int is_slow;
	is_slow = (psp_pad.pad_data & PSP_KEY_SLOW);
	/* const */ signed int my_speed = player_speed256[(cg_game_select_player) + ((is_slow)?(PLAYERS8):(0))];
	/* �ړ��ʂ����߂�B(�ړ��� = ���@���x x �A�i���O�L�[�A�f�W�^���̏ꍇ�͗\�߃A�i���O�L�[�ړ��ʂɕϊ����Ă���) */
	#if 1/* [�}�X�^�[�X�p�[�N���́A���x�����B] */
	if (
		(MARISA_B==(cg_game_select_player)) &&	/* ������B(����) */
		(0<cg.bomber_time)				/* �}�X�^�[�X�p�[�N�� */
	)
		#if 0
	{
		;/* �}�X�^�[�X�p�[�N���̓V���b�g�����ĂȂ��炵�� */
	}
	if (is_master_spark)
		#endif
	{	(my_speed >>= 1);	}
	#endif
	short	aaa_cg_analog_x = (((my_speed)*(psp_pad.analog_absolute_value_x))>>8);
	short	aaa_cg_analog_y = (((my_speed)*(psp_pad.analog_absolute_value_y))>>8);
	/* �΂߂��l�����Ĉړ�����B */
	s1->cx256 += ((((signed int)(jiki_move_length[((psp_pad.pad_data&0xf0)>>4)][0]))*(aaa_cg_analog_x))>>8);	/* fps_factor */
	s1->cy256 += ((((signed int)(jiki_move_length[((psp_pad.pad_data&0xf0)>>4)][1]))*(aaa_cg_analog_y))>>8);	/* fps_factor */
	/* ��ʊO�ɁA�݂͂�������C���B(���S���W�Ŕ���) */
//		 if (s1->cx256 < t256(0))				{	s1->cx256 = t256(0);				}/* ���`�F�b�N */
//	else if (s1->cx256 > t256(GAME_WIDTH)  )	{	s1->cx256 = t256(GAME_WIDTH);		}/* �E�`�F�b�N */
//		 if (s1->cy256 < t256(0))				{	s1->cy256 = t256(0);				}/* ��`�F�b�N */
//	else if (s1->cy256 > t256(GAME_HEIGHT) )	{	s1->cy256 = t256(GAME_HEIGHT);		}/* ���`�F�b�N */
	/*	psp �́A�e�ŃQ�[�������Č�����ɂ́A������Ȃ�ł��㉺����������B
		�㉺����ݏo���̂̓Q�[�����ƁA�Q�[����ʐ݌v�̗����̖��ňӐ}�I�ł���B
		�Q�[����ʐ݌v��͍��E�͋�ݏo���Ă��ǂ��̂����A�͕핗�ł͋�ݏo���Ȃ����ɂ���B
		����͍��E�̕ǂŔ��˂���e�������A��ݏo�����ƒx���Ȃ肻���Ȃ̂œs����̖��B
		��邳��̏�����͉����ɂȂ�����o����̂��B */
	#if 0/*(-r34)*/
		 if (s1->cx256 < t256(PLAYER_WIDTH/2))					{	s1->cx256 = t256(PLAYER_WIDTH/2);					}/* ���`�F�b�N(��ݏo���Ȃ�) */
	else if (s1->cx256 > t256(GAME_WIDTH-(PLAYER_WIDTH/2))	)	{	s1->cx256 = t256(GAME_WIDTH-(PLAYER_WIDTH/2));		}/* �E�`�F�b�N(��ݏo���Ȃ�) */
//		 if (s1->cy256 < t256(PLAYER_HEIGHT/2)) 				{	s1->cy256 = t256((PLAYER_HEIGHT/2));				}/* ��`�F�b�N(��ݏo���Ȃ�) */
//	else if (s1->cy256 > t256(GAME_HEIGHT-(PLAYER_HEIGHT/2)) )	{	s1->cy256 = t256(GAME_HEIGHT-(PLAYER_HEIGHT/2));	}/* ���`�F�b�N(��ݏo���Ȃ��A5[pixel]�������ۂ���psp�͏c�𑜓x�����߂���) */
		 if (s1->cy256 < t256(0))								{	s1->cy256 = t256(0);								}/* ��`�F�b�N(�オ��ݏo�����A��ݏo���Ȃ����܂��d�l�����܂��ĂȂ�) */
	else if (s1->cy256 > t256(GAME_HEIGHT)) 					{	s1->cy256 = t256(GAME_HEIGHT);						}/* ���`�F�b�N(������ݏo���͈̂Ӑ}�I) */
	#else/*(r35-)*/
//		 if (s1->cx256 < t256(GAME_X_OFFSET))					{	s1->cx256 = t256(GAME_X_OFFSET);					}/* ���`�F�b�N(��ݏo���Ȃ�) */
//	else if (s1->cx256 > t256(GAME_WIDTH)	)					{	s1->cx256 = t256(GAME_WIDTH);						}/* �E�`�F�b�N(��ݏo���Ȃ�) */
//		 if (s1->cy256 < t256(0))								{	s1->cy256 = t256(0);								}/* ��`�F�b�N(�オ��ݏo�����A��ݏo���Ȃ����܂��d�l�����܂��ĂȂ�) */
//	else if (s1->cy256 > t256(GAME_HEIGHT)) 					{	s1->cy256 = t256(GAME_HEIGHT);						}/* ���`�F�b�N(������ݏo���͈̂Ӑ}�I) */
	/* ��ݏo���Ȃ���[pixel] */
	#define ADJUST_JIKI_WIDTH	(8)
	s1->cx256 = psp_min(s1->cx256, t256(GAME_WIDTH-ADJUST_JIKI_WIDTH)); 		/* �E�`�F�b�N(��ݏo��) */
	s1->cx256 = psp_max(s1->cx256, t256(GAME_X_OFFSET+ADJUST_JIKI_WIDTH));		/* ���`�F�b�N(��ݏo��) */
	s1->cy256 = psp_min(s1->cy256, t256(GAME_HEIGHT));							/* ���`�F�b�N(������ݏo���͈̂Ӑ}�I) */
	s1->cy256 = psp_max(s1->cy256, t256(0));									/* ��`�F�b�N(�オ��ݏo�����A��ݏo���Ȃ����܂��d�l�����܂��ĂȂ�) */
	#endif
	/* �R�A�ړ� */
	{
		OBJ *s2;
	//	s2 = obj_maru;
		s2 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU];
		/* Gu(���S���W) */
		s2->cx256 = s1->cx256;
		s2->cy256 = s1->cy256;
		/* �ᑬ���[�h�p�G�t�F�N�g�̕\�� */
		{
			OBJ *obj_tei_soku;
//			obj_tei_soku					= obj_effect[TEISOKU_EFFECT_00_OBJ];
			obj_tei_soku					= &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
			if (is_slow)	/* �ᑬ���[�h�p�G�t�F�N�g�A���A�\�� */
			{
//				OBJ *zzz_obj_maru;
//				zzz_obj_maru				= &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU];
				obj_tei_soku->obj_type_set	= (SPELL_SQUERE_);
				obj_tei_soku->cx256 		= (s1->cx256);	/* ���S���W */
				obj_tei_soku->cy256 		= (s1->cy256);	/* ���S���W */
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
				obj_tei_soku->rotationCCW1024 += (10);
				#endif
			}
			else	/* �ᑬ���[�h�p�G�t�F�N�g�A���A��\�� */
			{
				obj_tei_soku->obj_type_set		= (SP_DUMMY_MUTEKI);
			//	obj_tei_soku->cy256 			= (t256(300));	/* ���S���W */
			}
		}
	}
	/* MAX���̃A�C�e���������W */
	if (psp_pad.pad_data & PSP_KEY_UP/* PSP_CTRL_UP */) /* ���F�΂ߏ�ł�����\ */ /* && (s1->y>0) */
	{
		#if 1
			/* �S�L����: �A�C�e���㕔���W�����ł��\ */
		#else
		if ((cg.weapon_power==MAX_POWER_IS_128) 	// 128 �ύX /* max==MAX_POWER_IS_128==�u129�i�K�v */
			/* ������ ����\�́F�A�C�e���㕔���W�����ł��\ */
			|| (MARISA_A==(cg_game_select_player))
			|| (MARISA_B==(cg_game_select_player))
		//	|| (MARISA_C==(cg_game_select_player))
		)	/* �������͏�ɏ㕔�������W���\ */
		#endif
		{
			if (s1->cy256 < ITEM_GET_BORDER_LINE256)/* [�A�C�e�������񂹃��C��] */
			{
				cg.state_flag |= JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU;	/* �㕔�������W�\ */
			}
		}
	}
	else
	if (psp_pad.pad_data & PSP_KEY_DOWN)	/* ���F�΂߉��ł���߂� */ /* PSP_CTRL_DOWN */
	{
		//if (cg.state_flag & (JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU /* | JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU */) )/* ???���Ԃ� */
		{
			cg.state_flag &= (~JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU); 	/* �㕔�������W�s�� */
		}
	}
//
		/*---------------------------------------------------------
			�v���C���[�V���b�g�֘A����
		---------------------------------------------------------*/
	/* �V�i���I��b���[�h�ł̓V���b�g�{�^�����͖��� */
	if (IS_SCENE_KAIWA_TATAKAU)
	{
		if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
		{
			cg.state_flag |= JIKI_FLAG_0x0080_SHOT_KEY_SEND_FOR_OPTION; 	/* on(�I�v�V�������V���b�g������) */
			register_main_shot(s1);
		}
	}
	#if 0/* �f�o�b�O�p */
	p d_my_score	= (cg.jiki_weapon_level_offset);/* �f�o�b�O */
	p d_graze_point = (cg.jiki_weapon_level_offset);/* �f�o�b�O */
	#endif

	/*---------------------------------------------------------
		�v���C���[�A�j���[�V�����֘A����
	---------------------------------------------------------*/
	{static int anime_delay;
		anime_delay -= 1;	/* fps_fa_ctor */
		if (0 > anime_delay)
		{
			anime_delay = jiki_shot_get_player_animation_speed();
			//
			static int auto_anime_frame = (4);/* 5 */
				 if (psp_pad.pad_data & PSP_KEY_LEFT/* PSP_CTRL_LEFT */)	{	if ((0) < auto_anime_frame) 	{auto_anime_frame--;} }
			else if (psp_pad.pad_data & PSP_KEY_RIGHT/* PSP_CTRL_RIGHT */)	{	if ((8) > auto_anime_frame) 	{auto_anime_frame++;} }
			else
			{
				if (auto_anime_frame>4/* 5 */) auto_anime_frame--;
				if (auto_anime_frame<4/* 5 */) auto_anime_frame++;
			}
			#if 1
			{
				int now_anime_frame;
				static int auto_aniime;
				auto_aniime++;
				auto_aniime &= 3;
					 if (4==auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + auto_aniime); }	/* ���S1-4 */
				else if (4<auto_anime_frame)	{	now_anime_frame = (auto_anime_frame + (3) + (8) );	}	/* �E��1-4 */	/* +(8) == �I�v�V�����C���^�[���[�u */
				else							{	now_anime_frame = (auto_anime_frame );				}	/* ����1-4 */
				s1->obj_type_set = (JIKI_PLAYER_00_LEFT)+(now_anime_frame);
			}
			#else
			{
				static int auto_aniime;
				auto_aniime++;
				auto_aniime &= 3;
				s1->obj_type_set = (JIKI_PLAYER_00_LEFT)+(auto_anime_frame );
					 if (4==auto_anime_frame)	{	s1->obj_type_set += ( auto_aniime); }	/* ���S1-4 */
				else if (4<auto_anime_frame)	{	s1->obj_type_set += ( 3+8); 		}	/* �E��1-4 */
			//	else							{										}	/* ����1-4 */
			}
			#endif
		}
	}
/*
0 1 2 3 4 5 6  7  8  :: auto_anime_frame[0 to 8]
0 1 2 3 4 8 9 10 11  :: now_anime_frame [0 to 11]
		5
		6
		7
 */
}

/*---------------------------------------------------------
	���탌�x�����ύX���ꂽ�ꍇ�ɁA�����I�v�V�����̏�Ԃ��`�F�b�N���ĕύX
---------------------------------------------------------*/
/* [cg.weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
static void check_weapon_level(void)
{
	/*---------------------------------------------------------
		����̒i�K�����߂�
	---------------------------------------------------------*/
	// weapon�̒i�K���獡�̑��������߂� 	// ���X�g�����đ����̕ύX
	// �ő�129�ցB
	// (0-128��129�i�K�ɏC��)
	#if 0//
	/* ���낢��撣���Ă݂����A���̒���(6,11,61,86)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	u8 pds_weapon;	pds_weapon = cg.weapon_power;
		 if (pds_weapon < ( 8)) 	{	cg.jiki_weapon_level_offset = (WEAPON_L0<<3);	}	/* WEAPON_L0(P000-P008) */	/* 6 */
	else if (pds_weapon < ( 16))	{	cg.jiki_weapon_level_offset = (WEAPON_L1<<3);	}	/* WEAPON_L1(P008-P015) */	/* 11 */
	else if (pds_weapon < ( 32))	{	cg.jiki_weapon_level_offset = (WEAPON_L2<<3);	}	/* WEAPON_L2(P016-P031) */	/* 61 */
	else if (pds_weapon < ( 64))	{	cg.jiki_weapon_level_offset = (WEAPON_L3<<3);	}	/* WEAPON_L3(P032-P063) */	/* 86 */
	else if (pds_weapon < (128))	{	cg.jiki_weapon_level_offset = (WEAPON_L4<<3);	}	/* WEAPON_L4(P064-P127) */
	else							{	cg.jiki_weapon_level_offset = (WEAPON_L5<<3);	}	/* WEAPON_L5(P128)		 */ /* max==P128==�u129�i�K�v */
	cg.jiki_weapon_level_offset += (cg_game_select_player);
	#endif
	#if 1//
	u8 pds_weapon;	pds_weapon = cg.weapon_power;
	/* 0000 0001 1111 1---*/
	/* �g�A���ׂ��璚�x2�{���ۂ��B */
	{	/* ���x2�{���Ƃ���Ȋ����Ŋȗ����ł���B */
	//	cg.jiki_weapon_level_offset = ((cg_game_select_player)/* 0 */);
	//	cg.jiki_weapon_level_offset += (WEAPON_L5<<3);
	//	cg.jiki_weapon_level = ((0)/* 0 */);
		cg.jiki_weapon_level = (WEAPON_L5);
		unsigned int jj;
		for (jj=(0); jj<(5); jj++)
		{
			if (pds_weapon & ((0x80)>>jj))
			{
				goto loop_end;//break;
			}
		//	cg.jiki_weapon_level_offset -= (0x08);
			cg.jiki_weapon_level -= (0x01);
		}
		loop_end:;
	}/*
0000 0000 0000 0000
		  5432 1 0
		  0123 4
 */
	#endif
//
	/*---------------------------------------------------------
		�v���C���[�I�v�V�����`�F�b�N(�K�v�ł���΃I�v�V�������Z�b�g)
	---------------------------------------------------------*/
	#if 0/* (r32) */
	{
		/* ���낢��撣���Ă݂����A���̒���(35,55,80,108)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	//	static const u8 jjj_tbl[4] =
	//	{
	//		( 8),		// (35-1),
	//		(16),		// (55-1),
	//		(32),		// (80-1),
	//		(64)		// (108-1),
	//	};
		unsigned int jj;
		for (jj=(0); jj<(4); jj++)/* �I�v�V������4�� */
		{
			OBJ *zzz_obj_option;
			zzz_obj_option = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_04_JIKI_OPTION0+jj];
			if ((pds_weapon > ((0x08)<<jj)/* jjj_tbl[jj] */))	/* ���x2�{���Ƃ���Ȋ����Ŋȗ����ł���B */
			{
				/* �I�v�V�����o�� */
				/* �����V�K�o��Ȃ� */
				if (PL_OPTION_FLAG_00_OPTION_OFF==(zzz_obj_option->PL_OPTION_DATA_yuukou_flag) )
				{
					zzz_obj_option->PL_OPTION_DATA_yuukou_flag	= (PL_OPTION_FLAG_01_OPTION_ON);	// ���t���O��On(��)
					const OBJ *s1 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
					zzz_obj_option->cx256 = s1->cx256;
					zzz_obj_option->cy256 = s1->cy256;
				}
			}
			else
			{
				/* �I�v�V�����B�� */
				zzz_obj_option->PL_OPTION_DATA_yuukou_flag	= (PL_OPTION_FLAG_00_OPTION_OFF);	// ���t���O��OFF(�s��)
				zzz_obj_option->cy256=t256(GAME_HEIGHT+1+16);
			}
		}
	}
	#endif
	#if 1/* (r33)�`���m�͂Ƃ��������E�Ώ̂���Ȃ��Ƃ����������C�����Ă����B */
	{
		/* ���낢��撣���Ă݂����A���̒���(35,55,80,108)�͒N��(?)�Ȃ̂ŕς���(2�{�𒴂���l�̒����͈Ӗ������C��������) */
	//	static const u8 jjj_tbl[4] =
	//	{
	//		( 8),		// (35-1),
	//		(16),		// (55-1),
	//		(32),		// (80-1),
	//		(64)		// (108-1),
	//	};
	// jj0 00
	// jj1 01
	// jj2 10
	// jj3 11
		unsigned int jj;
		for (jj=(0); jj<(4); jj++)/* �I�v�V������4�� */
		{
			OBJ *option_s1;
			option_s1 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_04_JIKI_OPTION0+jj];
			const u8 jjj_tbl[4] = /* �I�v�V�����̏o������ */
			{
				( 8-1), 	// �I�v�V����#0��power 8�ȏ�ŕt���B
				( 8-1), 	// �I�v�V����#1��power 8�ȏ�ŕt���B
				(96-1), 	// �I�v�V����#2��power96�ȏ�ŕt���B
				(96-1), 	// �I�v�V����#3��power96�ȏ�ŕt���B
			};
			if ((pds_weapon > jjj_tbl[jj]) )/* �I�v�V�����̏o������ */
			{
				/* �I�v�V�����o�� */
				/* �����V�K�o��Ȃ� */
				if (PL_OPTION_FLAG_00_OPTION_OFF==(option_s1->PL_OPTION_DATA_yuukou_flag) )
				{
					option_s1->PL_OPTION_DATA_yuukou_flag = (PL_OPTION_FLAG_01_OPTION_ON);	// ���t���O��On(��)
					const OBJ *s1 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
					option_s1->cx256 = s1->cx256;
					option_s1->cy256 = s1->cy256;
				}
			}
			else
			{
				/* �I�v�V�����B�� */
				option_s1->PL_OPTION_DATA_yuukou_flag = (PL_OPTION_FLAG_00_OPTION_OFF); 	// ���t���O��OFF(�s��)
				option_s1->cy256 = t256(GAME_HEIGHT+1+16);
			}
		}
	}
	#endif
}

/*---------------------------------------------------------
	�v���C���[�ƃA�C�e���̂����蔻��1(�A�C�e����p)
	-------------------------------------------------------
		src 	�v���C���[
		ttt 	�A�C�e��
---------------------------------------------------------*/
static int player_add_power_ttt_type(OBJ *src, u8 ttt_type)
{
		int add_score_point;
	{	const u8 aaa[4] = { 1, 8, 127, 1/* �`���mA�p */};
	//	const int add_power = aaa[/* ((ttt_type)) */((NULL==ttt)?(0/* 3 �`���m�p */):((ttt->type)&0x03))];
		const int add_power = aaa[( ((ttt_type)&0x03))];
		int previous_power; previous_power = cg.weapon_power;		/* �A�C�e���擾���钼�O�̃p���[�l */
		cg.weapon_power += add_power;	/* �p���[�l�A���Z */
		/* �p���[�l�A�����Ă���C�� */
		#if (0)
		if ((MAX_POWER_IS_128-1) < cg.weapon_power )/* 127==128�i�K�� */	{	cg.weapon_power = MAX_POWER_IS_128;/* �ő�l */ 	}
		#else/*(ok?)*/
		cg.weapon_power = psp_min(cg.weapon_power, (MAX_POWER_IS_128));
		#endif
		if (MAX_POWER_IS_128 == cg.weapon_power/* previous_power */) /* max�̏ꍇ */
		{
			if (previous_power != cg.weapon_power)/* ���O��MAX����Ȃ������̂ɁA����MAX�ɂȂ����ꍇ */
			{
				#if 1
				/* ����@�\ */
				bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
				#endif
			}
			cg.chain_point += add_power;	/* �`�F�C���A���Z */
			/* �`�F�C���A�����Ă���C�� */
			#if (0)
			if (31 < cg.chain_point)	{	cg.chain_point = 31;/* �ő�l */	}
			#else/*(ok?)*/
			cg.chain_point = psp_min(cg.chain_point, (31));
			#endif
			/* (�m�[�~�X�{�[�i�X)�E�F�|���{�[�i�X���_�v�Z */
			add_score_point = (/* score_type */(cg.chain_point));
			#if 1
			/* �`�F�C���̓{��MAX�łȂ��Ǝ����A���܂�Ȃ� */
				#if 0
		//	if (NULL==ttt)/* �`���m�Ȃ� */
				#else
			/* ���~���A & �H�X�q & �`���m�́A�{������^(��{�I�ɂ��܂蒙�߂Ȃ��łǂ�ǂ�g��==���ƒ������܂�)�ɂ��悤���ƁB */
			if ((REMILIA-1) < (cg_game_select_player))/* ���~���A & �H�X�q & �`���m�Ȃ� */
				#endif
			{
				if ((8-1) < cg.bombs)	{	;	}/* ���ɍő�l(8)�Ȃ�΁A�Ȃɂ������B */
				else
				{
					cg.bombs++; 	/* �{���ǉ� */
					cg.weapon_power = (MAX_POWER_IS_128>>1);/* �ő�l�̔��� */
				}
			}
			#endif
		}
		else	/* max�łȂ��ꍇ�A���_�v�Z */
		{
			/* (MAX���ȊO��) [P]��10�_ */
			/* (MAX���ȊO��) [P��]��80�_ */
			add_score_point = ((add_power/*-1 */)/* SCORE_10 */);
		}
	}
	check_weapon_level();/* [cg.weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
//
	voice_play(VOICE05_BONUS, /*TRACK07_GRAZE*/TRACK06_ALEART_IVENT_02);/* �e�L�g�[ */
	return (add_score_point);
}
/* http://hossy.info/game/toho/k_score.php
B �A�C�e���̓_��
��Փx	�ʏ�F��   �ʏ�F�� 1px�� ������
easy	27800	   60000	 100 100000
normal	27800	   60000	 100 100000
hard	42040	  100000	 180 150000
lunatic 63330	  150000	 270 200000
extra	71200	  200000	 400 300000

---------------
�u�A�C�e�������񂹃��C���v�ȉ��Ŏ��Ɓu�A�C�e�������񂹃��C���v����322���C���܂ŋ����Ō��_�B

�v���C�t�B�[���h�� 384x448 �Ȃ̂ŁA448-322 == 126
�u�A�C�e�������񂹃��C���v�͑����ォ��128�h�b�g���ȁH

(B�A�C�e���̑傫����16x16[pixel]�Ȃ̂ŁAB�A�C�e���̏�������� 448+(16/2)���C���ōs������A
�Ō��B�A�C�e���̔��肪����̂��AB�A�C�e���̂x�����S��456[���C��]�ɂ����ꍇ�B
456-322 == 134, 134-128 == 6. ����6[���C��]�͂����������@���S����ʉ����ɍs���Ȃ��������h�b�g(5[pixel]?)����B
�c��1[pixel]�͕s�����Ŕ��肵������? )
---------------
�͕핗�ł͂a����ĉ��_�͖���(r33����)�B
 */
static void player_collision_check_item(OBJ *src)/* , int mask */ /* ,OBJ_Z03_ITEM */
{
	/* ���@���A�C�e���ɂ��������ꍇ */
	OBJ *ttt;	//�Ώ�
	ttt = obj_collision_check_00_tama(src, OBJ_Z03_ITEM); /* �e��p(�A�C�e��) */	/* mask */
	if (NULL != ttt)
	{
		const void *aaa_aaa[(8)] =
		{
			&&case_POWER_UP,				// case_SP_ITEM_00_P001
			&&case_POWER_UP,				// case_SP_ITEM_01_P008
			&&case_POWER_UP,				// case_SP_ITEM_02_P128
			&&case_SP_ITEM_03_1UP,			// case_SP_ITEM_03_1UP
			//
			&&case_SP_ITEM_04_BOMB, 		// case_SP_ITEM_04_BOMB
			&&case_SP_ITEM_05_TENSU,		// case_SP_ITEM_05_TENSU
			&&case_break,					// case_SP_ITEM_06_HOSI 	���_�́A�����ŏ������Ȃ��B�y�ѐ��_�́A�����蔻��Ȃ��B
			&&case_break,					// case_SP_ITEM_07_SPECIAL
		};
		goto *aaa_aaa[((ttt->obj_type_set)&0x07)];
	//	case_SP_ITEM_00_P001:	//	player_add_power(src, ttt, 1);		break;	// �E�F�|���A�C�e��[��p]
	//	case_SP_ITEM_01_P008:	//	player_add_power(src, ttt, 8);		break;	// �E�F�|���A�C�e��[��P]
	//	case_SP_ITEM_02_P128:	//	player_add_power(src, ttt, 127);	break;	// �E�F�|���A�C�e��[F]
		case_POWER_UP:	// �E�F�|���A�C�e��[��p][��P][F]
			;
	//	if (NULL!=ttt)/* �`���m�ȊO�Ȃ� */
		{	const int add_score_point = player_add_power_ttt_type(src, ttt->obj_type_set);
			bonus_info_score_nodel(ttt, (add_score_point)/*add_score_point*/);
			/* */ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
		}
			goto case_break;
		//
		case_SP_ITEM_04_BOMB:
			if ((8-1) < cg.bombs)	{	goto add_10000pts;	}/* ���ɍő�l(8)�Ȃ�΁A10000+ [pts] */
			cg.bombs++;
			#if 1/* �o�Ofix? */
			ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
			#endif
			voice_play(VOICE05_BONUS, /*TRACK07_GRAZE*/TRACK06_ALEART_IVENT_02);/* �e�L�g�[ */
			goto case_break;
		case_SP_ITEM_03_1UP:
			if ((8-1) < cg.zanki)	{	goto add_10000pts;	}/* ���ɍő�l(8)�Ȃ�΁A10000+ [pts] */
			cg.zanki++;
			#if 1/* �o�Ofix? */
			ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
			#endif
			/* effect_sound_number= */voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC);/* �e�L�g�[ */
			goto case_break;
		case_SP_ITEM_05_TENSU:
			// �`���mQ�̏ꍇ�A�_���A�C�e���Ńp���[�A�b�v[p]power+1�B
			if (CIRNO_Q == cg_game_select_player)/* 7:CIRNO_Q only. (r35-) */
			{	/*(�`���mQ�̏ꍇ����)*/
				ttt->obj_type_set = SP_ITEM_00_P001;	/* �p���[�A�b�v ��[p]�� �Ɠ����B */
				goto case_POWER_UP;
			}
			else/*(�ʏ�)*/
			{
				/* ITEM_GET_BORDER_LINE256 ����Ŏ��� 10000pts. ... ���Ŏ���(?)��100pts. */
				/* (���90pts�A���ɓ�����A����΂��(�����钼�O��3���C��)70pts�܂ŉ\�炵��) */
				int add_score_point;
				if (ITEM_GET_BORDER_LINE256 < src->cy256)	/* ITEM_GET_BORDER_LINE256 �����̏ꍇ�́AITEM_GET_BORDER_LINE256�܂ł̋����ɉ����Č��_ */
				{
				//	add_score_point -= ((ttt->cy256-ITEM_GET_BORDER_LINE256)>>(4+8));
				//	add_score_point = ((/* 0x1f-(3) */(SCORE_9000)-((ttt->cy256-ITEM_GET_BORDER_LINE256)>>(3+8)))/* &0x1f */);
					add_score_point = (SCORE_9000) - ((ttt->cy256-ITEM_GET_BORDER_LINE256)>>(3+8));
				}
				else
				{
					add_10000pts:/* �����ɓ��_10000+ [pts] */
				//	add_score_point = (SCORE_10000);	/* ��{�_ 10000[pts] */
					add_score_point = (SCORE_10000+(cg.game_difficulty));	/* ��{�_ 10000[pts](easy), 11000(normal), 12000(hard), 512000(lunatic). */
				}
				bonus_info_score_nodel(ttt, add_score_point);
				/* */ttt->jyumyou = JYUMYOU_NASI;/* �����܂� */
			}
			voice_play(VOICE05_BONUS, /*TRACK07_GRAZE*/TRACK06_ALEART_IVENT_02);/* �e�L�g�[ */
		case_break:
			;
	}
}
	//	case_SP_ITEM_06_HOSI:
	//	case_SP_ITEM_07_SPECIAL:
	//		goto case_break;
		#if 0
		/* ���_�́A�����蔻��Ȃ� */
		case_SP_ITEM_05_HOSI:		/* not_break; */
			ttt->jyumyou = JYUMYOU_NASI;/* ���_�̂ݓ��ʏ��� */
			voice_play(VOICE05_BONUS, /*TRACK07_GRAZE*/TRACK06_ALEART_IVENT_02);/* �e�L�g�[ */
			goto case_break;
		#endif


/*---------------------------------------------------------
	�҂���[�񏈗�(�T�u)
	-------------------------------------------------------
	�v���C���[�� �G�O���[�v �܂��� �G�e �ɓ��������ꍇ�A
	��炢�{���J�n�ׂ̈́A�҂���[�񏈗����s���B
---------------------------------------------------------*/
static void player_explode(OBJ *s1);
extern int jiki_shot_get_kurai_bombtimer(void);/* ��炢�{���̓��͎󂯕t�����Ԃ�ݒ� */
static void player_set_pichuun(void)
{
	pds_status_timer	= jiki_shot_get_kurai_bombtimer();
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/* TRACK01_EXPLODE */);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
	{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
		player_explode(zzz_player);/* �v���C���[���� */
	}
}


/*---------------------------------------------------------
	�A�C�e���ȊO�̃v���C���[�̂����蔻��B
	-------------------------------------------------------
	�u�G�e�ɂ��O���C�Y�v
	�u�G�e�ɂ�鎀�S�v
	�u�G(�G�����{�X)�ɂ�鎀�S�v
---------------------------------------------------------*/
	//	player_collision_check_graze(s1);	/* �O���C�Y�̓����蔻�� */
	//	player_collision_check_teki(s1);	/* �G�����{�X�̓����蔻�� */

#define TEST_FIX_GRAZE048	(t256(128.0))
#define TEST_FIX_DEATH001	(t256(1.0))
#define TEST_FIX_TEKI001	(t256(1.0))

static void player_collision_check_tama(OBJ *src)
{
	//static void player_collision_check_graze(OBJ *src)
	/*---------------------------------------------------------
		�v���C���[�ƓG�e�̂����蔻��2(�O���C�Y��p)
		-------------------------------------------------------
			src 	�v���C���[
			ttt 	�G�e
	---------------------------------------------------------*/
	#if (1)
	src->m_Hit256R		= TEST_FIX_GRAZE048;/* �Ă��ƁA�O���C�Y����B */
	#endif
	OBJ *ttt;	/* �G�e */
	ttt = NULL;
	ttt = obj_collision_check_00_tama(src, OBJ_Z04_TAMA);/* �e��p */
	if (NULL != ttt)
	{
		/* �v���C���[�ɓG�e�����������ꍇ�̓O���C�Y���� */
		if (0==(ttt->hatudan_register_spec_data & TAMA_SPEC_4000_GRAZE))/* �O���C�Y��? */
		{
			ttt->hatudan_register_spec_data |= TAMA_SPEC_4000_GRAZE;/* �O���C�Y�� */
			cg.graze_point++;/* �O���C�Y�� */
			// �`���mA��Q�̏ꍇ�A�O���C�Y�Ńp���[�A�b�v�B
		//	if ((CIRNO_A-1) < cg_game_select_player)/* 6:CIRNO_A or 7:CIRNO_Q (-r34) */
			// �`���mA�̏ꍇ�̂݁A�O���C�Y�Ńp���[�A�b�v�B
			if (CIRNO_A == cg_game_select_player)/* 6:CIRNO_A only. (r35-) */
			{
				player_add_power_ttt_type(src, /* ttt->type */(3)/*(3==�`���mA��p)*/);
			}
			else
			{
				player_dummy_add_score((score(100)+score(200)*(cg.game_difficulty)));/* score(500) */
			}
			/* �O���C�Y�� */
			voice_play_graze(); 	/* �O���C�Y���Ɋւ��ẮA��p�̔����������K�v�B */
		}
		#if 1
		/* �v���C���[�X�v���C�g���e�ɐG��Ă��邩 */
		{
			OBJ *uuu;	/* uuu */
			uuu = NULL;
		#if (0)
			OBJ *zzz_obj_maru;
			zzz_obj_maru = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU];
			/* (�O���C�Y���Ȃ�R�A�ƓG�e�̂����蔻�������) */
//			#if (0)
//			/* �R�A�Ŕ���(�p�~) */
//			uuu = obj_collision_check_00_tama(zzz_obj_maru, OBJ_Z04_TAMA);/* �e��p */
//			#else
			/* ���@�Ŕ��� */
			int chache_m_Hit256R;
			chache_m_Hit256R	= src->m_Hit256R;/* ���@�̂����蔻��T�C�Y */
			src->m_Hit256R		= zzz_obj_maru->m_Hit256R;/* ���̂����蔻��T�C�Y */
			uuu = obj_collision_check_00_tama(src, OBJ_Z04_TAMA);/* �e��p */
			src->m_Hit256R		= chache_m_Hit256R;/* �߂� */
//			#endif
		#else
			src->m_Hit256R		= TEST_FIX_DEATH001;/* �Ă��ƁA���S����B */
			/* ���@�Ŕ��� */
			uuu = obj_collision_check_00_tama(src, OBJ_Z04_TAMA);/* �e��p */
			src->m_Hit256R		= TEST_FIX_GRAZE048;/* �Ă��ƁA�O���C�Y����B */
		#endif
			if (NULL != uuu)	{	player_set_pichuun();	}
		}
		#endif
	}
}
	/*---------------------------------------------------------
		�v���C���[�ƓG�̂����蔻��3(�G��p)
		-------------------------------------------------------
			src 	�v���C���[
			ttt 	�G
		-------------------------------------------------------
		���@���G(�{�X/���^/�U�R)�ɑ̓����肳�ꂽ�ꍇ
	---------------------------------------------------------*/
extern void player_weapon_collision_check(OBJ *shot, int erase_shot_type);
static void player_collision_check_teki(OBJ *src)
{
	#if (1)
	src->m_Hit256R		= TEST_FIX_TEKI001;/* �Ă��ƁA�G����B(�G�Ƃ̓����蔻��Ŏ���) */
	#endif
	//	static void player_collision_check_teki(OBJ *src)
	{
		OBJ *ttt;	/* �G����(�{�X/���^/�U�R) */
		/* */  // (OBJ_Z02_TEKI );
		ttt = obj_collision_check_01_teki(src);// /* �G��p */
		if (NULL != ttt)
		{
			if (0 == pds_status_timer)	/* �ʏ펞�̏ꍇ */
			{
				player_set_pichuun();
			}
			else
			#if (1==USE_r36_SCENE_FLAG)
			if (IS_SCENE_DOUCHU_TUIKA)
			#else
			if (0==(cg.state_flag & STATE_FLAG_0x0800_IS_BOSS))
			#endif
			/*(�{�X���͎G���ǉ����Ȃ�)*/
			/*�����̏ꍇ�̂�(�G���ǉ���������)*/
			{
			//	if (�U�R==ttt)
			//	{
			//		�G�����_���[�W���󂯂�B
					player_weapon_collision_check(src, PLAYER_WEAPON_TYPE_01_BOMB_PLAYER);/* �{��/player�œG��|�������蔻�� */
			//	}
			}
		}
	}
}


/*---------------------------------------------------------
	�v���C���[�ړ�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�v���C���[���[�v�𔲂��鏈��
---------------------------------------------------------*/
global void player_loop_quit(void)
{
	#if (1==USE_r36_SCENE_FLAG)
	/* off / �����R�}���h�ǉ��ǂݍ��ݏ������~����B */
//	cg.state_flag		&= (~SCENE_NUMBER_MASK); 	/*(�V�[��������)*/
//�_���B���R==continue(?????)	cg.state_flag		&= (0xffff00ffu); 	/*(�V�[��������)*/
	cg.state_flag		|= (0x00008000u); //�v���C���[���[�v�𔲂��鏈��(�Ƃ肠����)
	#else
//	/* (r32) */cg.state_flag		|= ST ATE_FLAG_14_GAME_LOOP_QUIT;
	/* (r32) */cg.state_flag		&= (~STATE_FLAG_14_DOUCHU_TUIKA);	/* off / �����R�}���h�ǉ��ǂݍ��ݏ������~����B */
	#endif
	pds_status_timer	= (LIMIT_m16_GAME_OUT); 	/* �Ƃ肠���� */	/* 150	 120�ł͎኱�Z�� 100 */
}
	#if (0000)
	option[0]->PL_OPTION_DATA_yuukou_flag = (PL_OPTION_FLAG_00_OPTION_OFF); /* �I�v�V���������� */
	option[1]->PL_OPTION_DATA_yuukou_flag = (PL_OPTION_FLAG_00_OPTION_OFF); /* �I�v�V���������� */
	option[2]->PL_OPTION_DATA_yuukou_flag = (PL_OPTION_FLAG_00_OPTION_OFF); /* �I�v�V���������� */
	option[3]->PL_OPTION_DATA_yuukou_flag = (PL_OPTION_FLAG_00_OPTION_OFF); /* �I�v�V���������� */
	#endif/* 0000 */
//	pds_status_timer	= (150);	/* 150	 120�ł͎኱�Z�� 100 */
//	cg.special_ivent_type	= PLAYER_STATE_04_GAME_OUT;/* GAME_OUT�� */
	#if (00)
	/* ���@�A��\�� */
	/* ���A��\�� */
//	obj_maru->alpha 	= 0x00; 		// �����������߂ɓ��ꂽ���ǈӖ�����������������Ȃ��B	// �Q�[���I�[�o�[���́��̕\������
	#endif
	/* �I�v�V��������\���̏ꍇ�A�������Ȃ��B */
//	if (PL_OPTION_FLAG_00_OPTION_OFF==(src->PL_OPTION_DATA_yuukou_flag))	{	return; 	}


/*---------------------------------------------------------
	�v���C���[��������
---------------------------------------------------------*/

static void player_option_position_init(OBJ *s1) 	/* �v���C���[�ʒu�̏����� */
{
	OBJ *option_s1;
	option_s1 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_04_JIKI_OPTION0];/*(��ڂ̃I�v�V�����ɃZ�b�g�B)*/
	unsigned int jj;
	for (jj=0; jj<(OPTION_04_MAX); jj++)
	{
		option_s1->cx256 = s1->cx256;/*(���@�̍��W)*/
		if (PL_OPTION_FLAG_00_OPTION_OFF!=(option_s1->PL_OPTION_DATA_yuukou_flag))	// ���t���O��On(��)�Ȃ�B
		{	/* �I�v�V�����o�� */
			option_s1->cy256 = s1->cy256;/*(���@�̍��W)*/
		}
		else	// ���t���O��OFF(�s��)�Ȃ�B
		{	/* �I�v�V�����B�� */
			option_s1->cy256 = t256(GAME_HEIGHT+1+16);
		}
		option_s1++;/*(���̃I�v�V�����ɃZ�b�g�B)*/
	}
}
static void player_fukkatsu_aaa(OBJ *s1) 	/* �v���C���[�ʒu�̏����� */
{
	/* Gu(���S���W) */
	s1->cx256 = t256(GAME_WIDTH/2);
	s1->cy256 = t256(220);/* 240 */ /* 217.6 == (272)GAME_HEIGHT x 0.80 */
	s1->m_zoom_x256 = t256(1.0);/*	�����o�ꎞ�A�g�嗦 x 1.0[�{] */
	s1->m_zoom_y256 = t256(1.0);/*	�����o�ꎞ�A�g�嗦 x 1.0[�{] */
}

/* �X�e�[�W���ɕ�������ꍇ�̖��G�ݒ� */
static void player_few_muteki(void)
{
//	pds_status_timer	= (40); 						// ���G���� 		/* 120 */ /* 150-120 */
//	cg.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
	pds_status_timer	= (LIMIT_m127_MIN); 			// ���G���� 		/* 120 */ /* 150-120 */
}

static void player_fukkatsu_bbb222(OBJ *s1)
{
	player_few_muteki();/* �X�e�[�W�J�n���̂ݎ኱�̖��G��ԂɃZ�b�g */
	#if (00)
	/* ���@�A�\�� */
	#endif
	/* obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER] */s1->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	/* ���@�A������ */	/*	s1->alpha	= 0x50; */
//
	{
		OBJ *zzz_obj_maru;
		zzz_obj_maru = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU];
		/* ���A�\�� */
		zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0x88, 0x88, 0x50);	/* ���A������ */	/*	obj_maru->alpha = 0x50; */
	}
	// �`���mA��Q�̏ꍇ�A�u�{�������v����X�^�[�g.
//	if ((CIRNO_A-1) < cg_game_select_player)/* 6:CIRNO_A or 7:CIRNO_Q (-r34) */
	// �`���mA�̏ꍇ�̂݁A�u�{�������v����X�^�[�g.
	if (CIRNO_A == cg_game_select_player)/* 6:CIRNO_A only. (r35-) */
	{
		cg.bombs			= (0);	/* �{���������̏����� */
	}
	else/* �`���m�ȊO */
	{
		cg.bombs			= option_config[OPTION_CONFIG_01_BOMB]; 	/* �{���������̏����� */
		/* static_fix_status[BASE_BOMBS+(cg_game_select_player)] */  /* 3 */
	}
	cg.chain_point			= (0);										/* �m�[�~�X�{�[�i�X�̏����� */
	// cg.chain_point(�m�[�~�X�{�[�i�X)�̏����� // �ǂꂾ���A��(�m�[�~�X)��wepon_up���������
//
	check_weapon_level();/* [cg.weapon_power���ύX���ꂽ�ꍇ�ɕK���s����`�F�b�N] */
}

/* �X�e�[�W�f�[�^�[�ǂݍ��݌�̖��G�ݒ� */
global void player_init_stage(void)
{
	OBJ *s1;
	s1 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
	player_fukkatsu_aaa(s1);			/* �v���C���[�ʒu�̏����� */
	player_option_position_init(s1);	/* �v���C���[�I�v�V�����ʒu�̏����� */

//	pds_status_timer		= (40); 						// ���G���� 		/* 120 */ /* 150-120 */
//	cg.special_ivent_type	= PLAYER_STATE_02_SAVE_01;		// ���G��ԁH
	pds_status_timer		= (LIMIT_512_STAGE_START);		// �X�e�[�W�J�n���A���G���� 	/* 120 */ /* 150-120 */
}
/*---------------------------------------------------------
	�v���C���[���ꏈ��
---------------------------------------------------------*/

static void player_explode(OBJ *s1)
{
	/* �A�C�e����f���o�� */
	{
		if ((0==cg.zanki)
			/* &&(0==difficulty) */ /* (�g�A���ׂ����Փx�Ɋ֌W�Ȃ�[F]���o��)	 */
		)
		{
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)easy �̏ꍇ */
			/* ���ʂ�8�f������ */
			/* �R���e�B�j���[�̏ꍇeasy �̏ꍇ�F (�L���b�`�o���鐔�œ_���͈Ⴄ���ǁA�ǂꂩ�P�L���b�`����� POWER �͓���������) */
		//	item_create_flags_num(s1, (0==difficulty)?(SP_ITEM_02_P128):(SP_ITEM_01_P008), (8), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
			item_create_flags_num(s1, (SP_ITEM_02_P128), (8), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
		}
		else
		{
			/* �R���e�B�j���[�̏ꍇnormal�ȏ�̏ꍇ�F FIRE_POWER_G (8) x (8) == 64/128 (�S���W�߂�50%) �B */
			/* �R���e�B�j���[�̏ꍇ(GAME_OUT)normal�ȏ�̏ꍇ */
			/* �R���e�B�j���[�ȊO�̏ꍇ */
			/* �ɒ[�Ɏキ�Ď��񂾏ꍇ�̋~�ύ�:
				  ��肢�l��(���)���Ղ� FULLPOWER �ɂȂ�Ǝv�����A
				���ɖ}�~�X�Ŏ��񂾂Ƃ��Ă��{������Ȃǂ� POWER ���ێ�����̂ŁA
				�ɒ[�Ɏキ�Ď��ʎ��͂قڂȂ��B
				  �ɒ[�Ɏキ�Ď��ʂ̂͌��\���肾�Ǝv���̂ŁA�኱�̋~�ύ􂪕K�v���Ǝv���B
				���̏ꍇ FIRE_POWER_G (8) x (7+1) == 64/128 (�S���W�߂�50%) �ɂ����B
			 */
			#if (0)
			/*
				�p���[�A�b�v�V�X�e�����኱���������̂ŁA(30-1) -> (16) �֊�������������B
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
					WEAPON_L1�ȉ��Ŏ��񂾏ꍇ�̂݋~�ύ􂪂���A
					�S������WEAPON_L3�ɂȂ�̂ŁA�Ó����Ǝv���B���A������������?
					WEAPON_L1[P15] -> ���� -> �S�����[P8]x6[��] -> [P15+P8x6]==WEAPON_L3[P63]
					WEAPON_L1[P15] -> ���� -> �P���[P8]x1[��] -> [P15+P8x1]==WEAPON_L2[P23]
					WEAPON_L0[P00] -> ���� -> �S�����[P8]x6[��] -> [P00+P8x6]==WEAPON_L3[P48]
					WEAPON_L0[P00] -> ���� -> �P���[P8]x1[��] -> [P00+P8x1]==WEAPON_L1[P08]
					WEAPON_L0[P00] -> ���� -> �R���[P8]x3[��] -> [P00+P8x3]==WEAPON_L2[P24]
					WEAPON_L2[P16] -> ���� -> �S�����[P1]x6[��] -> [P16+P1x6]==WEAPON_L2[P22]
					����Ŕ���(3��)���ƕK��WEAPON_L2�ȏ�ɂȂ�B
			 */
		//	item_create(s1, (((30-1) > cg.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (7)/* 5 */, ITEM_MOVE_FLAG_06_RAND_XY);
		//	item_create(s1, (((16) > cg.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (6), ITEM_MOVE_FLAG_06_RAND_XY);/* �����牺��ł��A������������? */
		//	item_create(s1, SP_ITEM_01_P008, 1, ITEM_MOVE_FLAG_06_RAND_XY);
			#else
			/* �����C�����Ă݂� */
			item_create_flags_num(s1, (((16) > cg.weapon_power)?(SP_ITEM_01_P008):(SP_ITEM_00_P001)), (3), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );/* ����Ȑl������΂��3���鎖���O�� */
			item_create_flags_num(s1, SP_ITEM_00_P001, (5), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );/* P001���΂�܂�(���܂��A����Ȑl�́A����΂��Ă������ɂ͎��Ȃ��Ƃ��������O��) */
			#endif
		}
		//
		/* ���~���A ����\�́F������������ƕK���{�����o�� */
		if (REMILIA==(cg_game_select_player))	/* ���~���A�̏ꍇ */
		{
			/* ������������� */
			if (0 != cg.bombs)
			{
				/* �g���ĂȂ����̎����Ă�{����f���o�� */
				item_create_flags_num(s1, SP_ITEM_04_BOMB, (cg.bombs), (ITEM_MOVE_FLAG_02_WAIT|ITEM_MOVE_FLAG_06_RAND_XY) );
			}
		}
	}
//		cg.explode		= 0;
//	if (0==cg.explode)
	{
	//	bakuhatsu_add_type(t->cx256+t256(5),t->cy256+t256(5),/* 0, */BAKUHATSU_MINI00);
	//	t->jyumyou = JYUMYOU_NASI;/* �����܂� */			/* ���������G���� */
	//	cg.explode=0;
	//	cg.bonus=0;
		//	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/* TRACK01_EXPLODE */);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */	// �ꏊ���ړ�����B// �ύX��
		/* �����G�t�F�N�g */
		/* �������� */
		#if 1
		set_REG_DEST_XY(s1);/* ���e�ʒu ���Wxy */
		{
			int jj;
			for (jj=0; jj<(4); jj++)
			{
				bakuhatsu_add_type_ccc(BAKUHATSU_MINI00);
			}
		}
		#endif
		/*	 */
		cg.bomber_time	= 0;			/* �����������������Ŗ�����(&�`���m)�{�����ĂȂ��Ȃ��Ă� */
		#if 1
	//	cg.bomber_time = 0;/* �s���� */
		set_bg_alpha(255);/* ��ʂ𖾂邭���� */
		#endif

	//	cg.state_flag		= JIKI_FLAG_0x00_ALL_OFF;/* ??? */		/*(�S�t���O����������@�\)*/
		cg.state_flag		&= (~(	JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU			|		/* �I��� */
									JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU 			|		/* �I��� */
									JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* �I��� */
	}
}

/*---------------------------------------------------------
	�v���C���[�ړ��A���ꏈ��
	-------------------------------------------------------
	���ꒆ�A�������A�{�����AGAME_OUT���Ȃǂ̓��ꏈ��
	-------------------------------------------------------
	�\��:�^�C�}�[�Đ݌v�B

	�O�ȊO�̏ꍇ�^�C�}�[�͎�����1���Z(�f�N�������g)�B
	pds_status_timer--;
	pds_status_timer &= 0xff; // ���炩�̒l�Ń}�X�N
//
	���l(-255): LIMIT_m64_MINUS_LIMIT
		������							(�����L�[���͖����B�{���ᑬ���L�[���́A����)
	���l(-127): LIMIT_m127_MIN
		�Q�[���I�[�o�[�f��				(�����L�[���͖����B�{���ᑬ���L�[���́A�����B�G�����薳���B�e�����薳���B)
	���l(-16): LIMIT_m16_GAME_OUT		PLAYER_STATE_04_GAME_OUT����(-256)�B
		��炢�{����t					(�����L�[���͖����B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
		�}�C�i�X���l�܂ł͋�炢�{����t���ԁB�e�ɂ��������ꍇ�A�L�����ʂŏ����l(-240�Ƃ�-200�Ƃ�)���Ⴄ�B
	���l(-1): LIMIT_m1_KURAI_BOMB_UKETUKE
		�e�ɂ��������ꍇ�A(�l��)���[�v
	ZERO(0):	LIMIT_0_ZERO			�ʏ펞(psp��ZERO���W�X�^�����邽��0��r�͑���)
		�ʏ펞							(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����蔻��L��B�e�����蔻��L��)
	ZERO(0):	LIMIT_0_ZERO			�ʏ펞(psp��ZERO���W�X�^�����邽��0��r�͑���)
	ITI(1): 	LIMIT_1_ONE 			��b���쎞(�C�x���g��)
		�������ɕt�����G�B				(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
	128?(128):	LIMIT_128				�v���C���[�������ԁB(�Œ�?)
		�������ɕt�����G�B				(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
	255:
		�Q�[���J�n���ɕt�����G�B		(�����L�[���͗L���B�{���ᑬ���L�[���́A�L���B�G�����薳���B�e�����薳���B)
	512:
---------------------------------------------------------*/
//void render_continue(void/* int cg_game_now_max_continue */);

static void player_move_special_ivent(OBJ *s1)
{
	pds_status_timer--;
	#if 0
//	pds_status_timer = (signed int)(pds_status_timer & (0x1ff)); // ���炩�̒l�Ń}�X�N
	#else
	if (LIMIT_m64_MINUS_LIMIT > pds_status_timer)	{	pds_status_timer += (512); }
//	if (-511 > pds_status_timer)					{	pds_status_timer += 1024; }/* �f�o�b�O���ؗp */
	#endif
//
	if (LIMIT_m127_MIN > pds_status_timer)
	{
	//	if (LIMIT_fukki2 == pds_status_timer)
	//	{
	//		;
	//	}
		/* �������̏����A�g�債�Ă��� X �̂ݏk���BY �͕ύX�����B */
		if (t256(1.0) < s1->m_zoom_x256)
		{
			s1->m_zoom_x256 -= (5*4);// 		s1->m_zoom_x256 -= (5);
		//	s1->m_zoom_y256 -= (5);
		}
		else
		{
			s1->m_zoom_x256 = (t256(1.0));
		//	s1->m_zoom_y256 = (t256(1.0));
		}
	}
	else
	if (LIMIT_m127_MIN == pds_status_timer)
	{
		//�Q�[���I�[�o�[�f��
//		case PLAYER_STATE_04_GAME_OUT:/* GAME_OUT�� */
//			cg.special_ivent_type	= 0;/* ??? */
			cg.game_now_max_continue--;
			if (
					(0 < cg.game_now_max_continue)
				#if (0)/* (0==DE BUG) */
				/* �܂����� */
				&& ( (6) != cg.game_now_stage)	/* �ŏI�ʂ̓R���e�B�j���[�o���Ȃ��B */
				#endif
				)
			{
				cg.player_data_use_continue++;	/* �R���e�B�j���[�� */
			//	render_continue(/* cg_game_now_max_continue */);
				cb.main_call_func = ask_continue_menu_start;
			}
			else
			{
				cb.main_call_func = gameover_start;
			}
//			break;
	}
	else
	if (LIMIT_m16_GAME_OUT > pds_status_timer)
	{
		;/* [�Ȃɂ����Ȃ�] */
		/* (�~�X�m�肵�Ă���)�Q�[���A�E�g�ɂȂ�܂ł̗P�\���� */
		/* (�����̃A�C�e������юU�鎞��) */
	}
	else
	if (LIMIT_m16_GAME_OUT == pds_status_timer) /* �v���C���[�A�K�b�c�������� */
	{
		/* ���݃I�v�V�������������āA�㔼�d���Ȃ�o�O������B */
		#if (0000)
		option[0]->PL_OPTION_DATA_yuukou_flag	= (PL_OPTION_FLAG_00_OPTION_OFF);	/* �I�v�V���������� */
		option[1]->PL_OPTION_DATA_yuukou_flag	= (PL_OPTION_FLAG_00_OPTION_OFF);	/* �I�v�V���������� */
		option[2]->PL_OPTION_DATA_yuukou_flag	= (PL_OPTION_FLAG_00_OPTION_OFF);	/* �I�v�V���������� */
		option[3]->PL_OPTION_DATA_yuukou_flag	= (PL_OPTION_FLAG_00_OPTION_OFF);	/* �I�v�V���������� */
		#endif/* 0000 */
		cg.player_data_count_miss++;	/* �~�X�� */
		/*(�Q�[�������N��������)*/
		cg.game_rank >>= 1;
		/* �c�`�����X���炷 */
		cg.zanki--;
		if (0 > cg.zanki)	/* �c�`�����X���Ȃ���� */
		{
			player_loop_quit();
		}
		else
		{
		/*	MAX(128)->����->(112)->����->(96)->����->(80)->����->(64)	(���16���ň��)	 */
			cg.weapon_power -= (16);	/* ���탌�x���̒ቺ(�g�A���ׂ����Փx�Ɋ֌W�Ȃ�P16��������) */
			#if (0)
			if (0 > cg.weapon_power)	{	cg.weapon_power = 0;	}
			#else/*(ok?)*/
			cg.weapon_power = psp_max(cg.weapon_power, (0));
			#endif
			player_fukkatsu_aaa(s1);	/* �v���C���[�ʒu�̏����� */
			player_fukkatsu_bbb222(s1);
			s1->m_zoom_x256 = t256(1.0)+(120*5);/* �g�� */
			s1->m_zoom_y256 = t256(1.0);//+(120*5);/* �g�� */
		}
	}
	else
	if (LIMIT_m1_KURAI_BOMB_UKETUKE > pds_status_timer)
	{
		;/* [�Ȃɂ����Ȃ�] */
		/* ��炢�{����t�̗P�\���� */
		//��炢�{����t���̏���
//		case PLAYER_STATE_01_KURAI_BOMB_UKETUKE:/* ��炢�{����t�� */
//			/* ���S�m�� */
//			break;
	}
	else
	/*
		(LIMIT_1_ONE-1)�͂܂� ���� �l(0) �����ǁA�Ӗ���(LIMIT_1_ONE-1)�Ƃ����Ӗ��B
		(LIMIT_1_ONE���� 1 ���������́A���� �l(-1) ��(player_move_special_ivent();�J�n����)pds_status_timer--;��␳���镪�B)
		(���ʓI��(���݂�)LIMIT_0_ZERO�Ɠ��������A�Ӗ���LIMIT_0_ZERO�Ɠ����ł͂Ȃ��B)
		LIMIT_1_ONE�͌Œ�l(1)�ł͂Ȃ��A���� �l(1) �����Ǐ����͔���Ȃ��B
		 */
	if ((LIMIT_1_ONE-1) == pds_status_timer)
	{
		/* ��������ʏ�ɖ߂� */
		s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);				/*	s1->alpha			= 0xff; */
		{
			OBJ *zzz_obj_maru;
			zzz_obj_maru = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU];
			zzz_obj_maru->color32	= MAKE32RGBA(0xff, 0x88, 0x88, 0xee);		/* ���A */	/*	zzz_obj_maru->alpha = 0xff; */
		}
		/*(�A�C�e���������W�t���O��߂�)*/
		{
			/* �������W���[�h�t���O��S�ď����B */
			cg.state_flag &=
			(	~(	JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU				|	/* MAX���̏��� */
					JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU 				|	/* �{���������̂݉�� */
					JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU 		/* �{�X���j���̈ꎞ��� */
				)
			);
		}
	}
//	else
//	if ((LIMIT_128-1) == pds_status_timer)
//	{
//		case PLAYER_STATE_03_SAVE_02:/* �v���C���[�������Q */
//			cg.special_ivent_type	= PLAYER_STATE_00_NORMAL;
//			s1->m_zoom_x256 		= t256(1.0)+100;	 /* �T�C�Y */
//			break;
//	}
}


/*---------------------------------------------------------
	�v���C���[�ړ��A���C������
	-------------------------------------------------------
	�{�X�Ń{�������G
	�{�����́A�{�X / ���U�R�G �ɂ������Ď���
	��b���[�h���쒆 �́A�G�����蔻��͂Ȃ�
	�L�[���͖�����(==������) �́A�G�����蔻��͂Ȃ�
	�{�����́A �{�X/�U�R�G �ɂ������Ď��ȂȂ�
	��炢�{����t�� �́A�G�����蔻��͂Ȃ�
---------------------------------------------------------*/

static void any_player_move(OBJ *s1)
{
	cg.state_flag &= (~JIKI_FLAG_0x0080_SHOT_KEY_SEND_FOR_OPTION);	/* off(�I�v�V�������V���b�g�����̂��~�߂�) */
	if (IS_SCENE_KAIWA_MODE)
	{
		pds_status_timer = (LIMIT_1_ONE);	/* ��b���[�h���쒆 �� 1 */
	}
	/* LIMIT_m16_GAME_OUT���傫���ꍇ�A�ړ��\�A�{�^���󂯕t���\ */
	if (LIMIT_m16_GAME_OUT < pds_status_timer)
	{
		player_keycontrol(s1);/* �ړ��{�{�^������ */
	}
	/* �ʏ펞 ���� ��O���̏����B(��O���̏����͕������Ƃ�) */
	if (0 == pds_status_timer)	/* �ʏ펞�̏ꍇ */
	{
		/* �v���C���[�ƁA�u�G�e�ɂ��O���C�Y�v/�u�G�e�ɂ�鎀�S�v�̓����蔻�� */
		player_collision_check_tama(s1);
		{
			/* [0] �Q�[�������N���グ��B */
			cg.game_rank++;
			cg.game_rank = psp_min(cg.game_rank, 65535);/*(�ő僉���N)*/
			#if (0)/*�f�o�b�O�p(�X�R�A���ɃQ�[�������N��\��)*/
			cg.game_score	= cg.game_rank;
			#endif
		}
	}
	else	/* ��O��(���ꏈ��)�̏ꍇ */
	{
		player_move_special_ivent(s1);	/* ���ꏈ�� */
	}
	/*(�G�̏ꍇ)*/
//	if (0 == pds_status_timer)	/* �ʏ펞�̏ꍇ */
	#if (1)
	if (IS_SCENE_KAIWA_TATAKAU)
	#endif
	{
		/*(��b�V�[���Ŏ��@���S�̂����蔻������Ȃ��B)*/
		/* �v���C���[�ƁA�u�G(�G�����{�X)�ɂ�鎀�S�v�̓����蔻�� */
		/*(�����ʏ펞�A���@vs�{�X�ŁA���@���ʔ���)*/		/*(�K�v)[�ʏ펞�ɂ͑̓�����ŋ�炢�{���󂯕t����Ԃֈڍs����]*/
		/*(�����ʏ펞�A���@vs�G���ŁA���@���ʔ���)*/		/*(�K�v)[�ʏ펞�ɂ͑̓�����ŋ�炢�{���󂯕t����Ԃֈڍs����]*/
		/*(�������G���A���@vs�{�X�ŁA�{�X���ʔ���)*/		/*(�v�f�o�b�O)*/
		/*(�������G���A���@vs�G���ŁA�G�����ʔ���)*/		/*(�K�v)[�������G���ɂ͑̓�����ŎG����|�������o����B]*/
		/*(��b�C�x���g���G���A���@vs�{�X�ŁA�{�X���ȂȂ�)*/	/*(�H)*/
		/*(��b�C�x���g���G���A���@vs�G���ŁA�G�����ȂȂ�)*/	/*(�K�v)[��b�C�x���g���G���ɂ͑̓�����ŎG����|�������o���Ȃ��B]*/
		player_collision_check_teki(s1);
	}
	/* �A�C�e���͏�Ɏ��� */
	player_collision_check_item(s1);		/* �A�C�e���̓����蔻�� */
}

//	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
//	if (0 <= pds_status_timer)	/* ���l�̏ꍇ�A�ړ��\ */

	#if 0/* �f�o�b�O */
	if (0 <= pds_status_timer)	/* ���l�̏ꍇ */
	{
		p d_my_score	= pds_status_timer;
		p d_graze_point = 0;
	}
	else
	{
		p d_my_score	= -pds_status_timer;
		p d_graze_point = 11;
	}
	#endif


/*---------------------------------------------------------
	�v���C���[������()
---------------------------------------------------------*/
#if (1==USE_EXTEND_CHECK)
extern void player_init_extend_score(void);
#endif
//extern int zanki;
global void player_continue_value(void)
{
	cg.zanki				= (1+option_config[OPTION_CONFIG_00_PLAYER]);	/* cg.base_zanki */ /* ((zanki&0x03)+2) */ /* static_fix_status[BASE_LIVES+(cg_game_select_player)] */
	cg.game_score			= score(0);
	#if (1==USE_r36_SCENE_FLAG)
		/*(����)*/
	#else
		#if 1/*???????????(r35u1�o�O)*/
		if (0==(cg.state_flag & STATE_FLAG_0x0800_IS_BOSS))
		/*(�{�X���͎G���ǉ����Ȃ�)*/
		/*�����̏ꍇ�̂�(�G���ǉ���������)*/
		{
		//	/* (r32) */cg.state_flag		&= (~ST ATE_FLAG_14_GAME_LOOP_QUIT);	/* ���A */
			/* (r32) */cg.state_flag		|= STATE_FLAG_14_DOUCHU_TUIKA;	/* on ���A */
		}
		#endif
	#endif
	#if (1==USE_EXTEND_CHECK)
	player_init_extend_score();
	#endif
	OBJ *s1;
	s1 = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
	player_fukkatsu_aaa(s1);	/* �v���C���[�ʒu�̏����� */
	player_fukkatsu_bbb222(s1);/* �I�v�V������ǉ����� */

	/*
		[�R���e�B�j���[�X�^�[�g��]�y�сA[�V�K�X�^�[�g��]�̓��ʏ����B(�܂�Q�[����������[�ȊO��]�ꍇ)
		------------------------------------------------------------
		�����̏ꍇ�́A�I����ɖ��G�ɂȂ�A
		�������ɓ�����(�����L�[���͗L��...)�悤�ɍ׍H����B
		------------------------------------------------------------
		(�Q�[�����������̏ꍇ�͈�莞�ԓ����Ȃ��B)
	 */
	/* �g�嗦��1.0�{�ɂ���B */
//	s1->m_zoom_x256 = (t256(1.0));
	/* (���G���Ԃ�)�����ɓ�����B */
	pds_status_timer = (LIMIT_300_CONTINUE);
}

/*---------------------------------------------------------
	�v���C���[���@���쐬�B
		FIX_OBJ_00_PLAYER
		FIX_OBJ_01_JIKI_MARU
		FIX_OBJ_02_BOMBER_HONTAI
	//	FIX_O_BJ_03_SEND2
	���@�I�v�V�������쐬�B
		FIX_OBJ_04_JIKI_OPTION0
		FIX_OBJ_05_JIKI_OPTION1
		FIX_OBJ_06_JIKI_OPTION2
		FIX_OBJ_07_JIKI_OPTION3
---------------------------------------------------------*/
extern int jiki_shot_get_player_graze_hankei(void);
static void jiki_create(void)
{
	#if 1
	OBJ *s1;/*(�v���C���[���@�{��)*/
	s1					= obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER);	/* �K���o�^�ł���B */
	s1->jyumyou 		= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
	s1->obj_type_set	= (OBJ_Z01_JIKI_GET_ITEM);
	s1->m_Hit256R		= ((jiki_shot_get_player_graze_hankei())<<8);/* �O���C�Y����B�擾�l��[pixel]�Ȃ̂ŁA256�{����t256()�`���ɕϊ��B */
	s1->atari_hantei	= (1/*�X�R�A���p*/);		/* �����蔻���ON==(1)==���@�����G�̏ꍇ�ɎG���ɑ΂��鋭���B */
	s1->callback_mover	= any_player_move;
	#endif
//
	#if 1
	OBJ *s2;/* �v���C���[�A��������p�R�A(obj_maru == ��)�̒ǉ� */
	s2					= obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_01_JIKI_MARU); /* �K���o�^�ł���B */
	s2->jyumyou 		= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//	s2->obj_type_set	= (JIKI_CORE_00_REIMU_A+(cg_game_select_player));
//	s2->obj_type_set	= (SPELL_SQUERE_);
	s2->obj_type_set	= (SP_DUMMY_MUTEKI);/*(�Ƃ肠�����\�����Ȃ�)*/
	s2->m_Hit256R		= t256(1.0);/*(���a1[pixel]==���a2[pixel]�̉~�Ŕ���)*/
	#endif
//
	#if 1
	OBJ *s3;/* �v���C���[�A�{���o�[�{�̂̒ǉ� */
	s3					= obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_02_BOMBER_HONTAI); /* �K���o�^�ł���B */
	s3->jyumyou 		= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//	s3->obj_type_set	= (JIKI_CORE_00_REIMU_A+(cg_game_select_player));
//	s3->obj_type_set	= (SPELL_SQUERE_);
	s3->obj_type_set	= (SP_DUMMY_MUTEKI);/*(�Ƃ肠�����\�����Ȃ�)*/
	s1->atari_hantei	= (0/*�X�R�A���p*/);		/* �����蔻���OFF==(0) */
	s3->m_Hit256R		= (0);
	#endif

//	#define TEISOKU_EFFECT_00_OBJ (6)
//	#define TEISOKU_EFFECT_01_OBJ (7)

//	// �ᑬeffect
//	OBJ *h;
//	h					= &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
//	h->atari_hantei 	= (ATARI_HANTEI_OFF);	/* �����蔻�薳�� */
//	h->color32			= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* ���@�A������ */	/*	s1->alpha			= 0x50;*/
}
	//	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */
	//	s1->m_Hit256R		= JIKI_ATARI_ITEM_16;/* t256(16.0); �O���C�Y���� */
	//	s1->m_Hit256R		= graze_atari[(cg_game_select_player)];/* �O���C�Y���� */

/*---------------------------------------------------------
	�v���C���[������
---------------------------------------------------------*/

/*static*/extern void kaiwa_init_obj_position(void);/*(r35u2�Ƃ肠����)*/

extern void jiki_option_create(void);
extern void init_player_shot(void);
extern void select_jiki_load_surface(void);
extern void sprite_panel_init(void);
global void jiki_class_initialize(void)
{
	select_jiki_load_surface();
	obj_cleanup_all();/* [A00�e�̈�]��[A01�G�̈�]��[A02�Œ�̈�]��[A03�p�l���̈�]��OBJ��S�����B */
	jiki_create();			/* ���@ ���쐬 */
	jiki_option_create();	/* ���@�I�v�V�������쐬 */
	//----[EFFECT]	/* �{�Xeffect ���쐬 */
	{
		{	unsigned int i;
			for (i=0; i<(5+3/*+3*/); i++)
			{
				OBJ *h;
				h						= obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_08_EFFECT01+i); /* �K���o�^�ł���B */
				h->obj_type_set 		= (SPELL_SQUERE_);
		//�d��	h->jyumyou				= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
		//�d��	h->atari_hantei 		= ATARI_HANTEI_OFF; 		/* �����蔻�薳�� */
			}
		}
		boss_effect_kotei_obj_r36_taihi();
	}
	//----[BOSS]	/* �{�X�{�� ���쐬 */
	/* �����蔻��̓s���� */
	{	OBJ *b1;
	//	b1								= obj_add_A01_teki_error();	/* �Gobj�ԍ���0�Ԃ�K���擾�ł���B */
		/*(�����蔻��̓s����A[A02�Œ�̈�]�ɂ͓o�^�ł��Ȃ��B[A01�G�̈�]�ɓo�^����K�v������B)*/
		b1								= obj_add_Ann_direct(OBJ_HEAD_01_0x0800_TEKI+TEKI_OBJ_00_BOSS_HONTAI); /* �K���o�^�ł���B */
	//	b1 = &obj99[OBJ_HEAD_01_0x0800_TEKI+TEKI_OBJ_00_BOSS_HONTAI];
		#if (0)/*(�����Ă���)*/
		b1->obj_type_set 				= BOSS_00_11;//???
		#endif
	//�d��	b1->jyumyou 					= (JYUMYOU_MUGEN);/* ���ԂŎ����������Ȃ� */	/*(�Œ�m�ۂŔ��ʂ����t���O)*/
		sprite_kotei_obj_r36_taihi(b1);/*(�{�Xobj�g�p���ł��邪�ޔ��ʒu�֑ޔ�)*/
	}
	//
	cg.player_data_use_continue 		= 0;	/* �R���e�B�j���[�� */
	cg.player_data_count_miss			= 0;	/* �~�X�� */
	cg.player_data_used_bomber			= 0;	/* �{���g�p�� */
	cg.player_data_use_kurai_bomb		= 0;	/* ��炢�{���g�p�� */
//	cg.player_data_count_bonus			= 0;	/* �J�[�h�{�[�i�X�� */
//
	cg.bomber_time						= 0;	/* ==bomb_wait */
	cg.graze_point						= 0;
	cg.game_rank						= 0;	/* �Q�[�������N */
//
	/* (r32) */cg.state_flag			= JIKI_FLAG_0x00_ALL_OFF;/*(�S�t���O����������@�\)*/
//
	card.boss_hp_dec_by_frame			= 0;/* �{�X�U�������l�A�t���[���P�� */
//
//	cg.jiki_weapon_level_offset 		= (cg_game_select_player) + (/* 0==���탌�x�� */0<<3);
	cg.jiki_weapon_level				= (/* 0==���탌�x�� */0<<3);/*(r35u2)*/
	/* ���K���[�h�̏ꍇ�̓t���p���[�Ŏn�߂�(���̑���N���A��̃C�x���g������Ȃ�) */
	cg.weapon_power 					= (0==cg.game_practice_mode)?(0):(MAX_POWER_IS_128);
//
	cg.game_now_max_continue			= DEFAULT_MAX_CONTINUE;/* (3) */
	cg.chuu_boss_mode					= (0);/* �ǂ����ŏ��������K���K�v */
//�p�~	cg.dr aw_flag_kaiwa_screen		= (0); /* ����ӃE�B���h�E�\���t���O off */
	init_player_shot();/*(�V���b�g�̐��\���v���C���[�ʂɏ�����)*/
	#if 1
	player_continue_value();
	#else
//	p d_zanki							= /* cg.base_zanki */ static_fix_status[BASE_LIVES+(cg_game_select_player)];
//	p d_my_score						= score(0);
//	/* Gu(���S���W) */
//	s1->cx256							= (t256(GAME_WIDTH/2));
//	s1->cy256							= (t256(GAME_HEIGHT));
//	player_fukkatsu_aaa(s1);	/* �v���C���[�ʒu�̏����� */
//	player_fukkatsu_bbb222(obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER]);/* �I�v�V������ǉ����� */
	#endif
	sprite_panel_init();

	#if 1/* (r33�d�l�ύX�ɂ��)�ǂ����ŏ��������K���K�v(�����łȂ������ǂ������H) */
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(0);/* �ǂ����ŏ��������K���K�v */
	#endif

	#if 1/*(r35u2�Ƃ肠����)*/
	{
		kaiwa_init_obj_position();
	}
	#endif
}


/*---------------------------------------------------------
	�G����
	�{�X�|�����ꍇ�̏���
	(�v���C���[���܂�)
	-------------------------------------------------------
	�{�X��|�����ꍇ�ɁA�v���C���[���ꎞ�I�ɖ��G�ɂȂ�B
	���̏����̈׃{�X��|�����ꍇ�ɂ����������ɌĂ΂��B
	-------------------------------------------------------
	(���̕����Ƃ��āA�t���O�̓���C�x���g�ɂ��đΏ�������@������B)
---------------------------------------------------------*/
global void jiki_eien_muteki_on(void)
{
//	cg.special_ivent_type	= PLAYER_STATE_02_SAVE_01;	// ���G��ԁH
//	cg.special_ivent_type	= PLAYER_STATE_03_SAVE_02;	/* �H�ɁA���܂������Ȃ� */
	/*
		���G���� �{�X�|������ꎞ�I�ɁA�i��(18.5��)�ɖ��G�ɂ���B
		���̖��G�́A���̃X�e�[�W���n�܂�ꍇ�ɉ��������B
	*/
	pds_status_timer		= (LIMIT_65535_BOSS_TAOSITA);
}
