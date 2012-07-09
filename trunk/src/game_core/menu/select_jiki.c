
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http:// mohou.huuryuu.com/
	-------------------------------------------------------
	�Q�[���J�n���̓�Փx/�v���C���[�I��
	-------------------------------------------------------
	[��Փx�I�����j���[]
	[�v���C���[�I�����j���[]
---------------------------------------------------------*/

#include "kanji_system.h"
#include "111_my_file.h"/*(my_file_common_name)*/


	/*(Gu�̏ꍇ�A��b�X�v���C�g���g��)*/
#include "kaiwa_sprite.h"
#include "../draw/gu_draw_screen.h"

/*---------------------------------------------------------
	player select�p
---------------------------------------------------------*/

#define BASE_SP_BG_PNG		(MAX_08_PLAYER*0)
#define BASE_SP_ST_PNG		(MAX_08_PLAYER*1)
#define BASE_SP_PNG_MAX 	(MAX_08_PLAYER*2)

/*---------------------------------------------------------
	�K���ǂ������B
---------------------------------------------------------*/
enum
{
	spec_message_00_SELECT_PLAYER = 0,
	spec_message_01_SELECT_RANK,
};
//		 if (spec_message_00_SELECT_PLAYER==set_mode)	{	... 	}
//	else if (spec_message_01_SELECT_RANK==set_mode) 	{	... 	}

/*---------------------------------------------------------
	��Ԃ����b�Z�[�W�\���B
	-------------------------------------------------------
	[��Փx�I�����j���[]
	[�v���C���[�I�����j���[]
---------------------------------------------------------*/

static void draw_spec_message(unsigned int set_mode)
{
	kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	static const char *const_player_spec_str[(8)/*(8�l)*/+(4)/*(4��Փx)*/][(2)/*(������+������)*/] =
	{	/*"�j�ޏ��̍U���͂̉���́A���������C�����邯�Ǔ\����Ȃ���Ύア�B"*/
		{	"�y���̑f�G�țޏ�\\n"	"���� �얲 (�아)\\n"		"�z�[�~���O�A�~�����b�g\\n" "���z����\\n",					"�ړ����x������\\n" 				"�U���͈́�����\\n" 	"�U���́@������",	},// No. 0 �얲 A(�아)
		{	"�y���̑f�G�țޏ�\\n"	"���� �얲 (����)\\n"		"�����j\\n" 				"���z���E\\n",					"�ړ����x������\\n" 				"�U���͈́�����\\n" 	"�U���́@������",	},// No. 1 �얲 B(����)
		{	"���ʂ̍����p�t\\n" 	"���J ������ (����)\\n" 	"�}�W�b�N�~�T�C��\\n"		"�X�^�[�_�X�g�����@���G\\n",	"�ړ����x������\\n" 				"�U���͈́�����\\n" 	"�U���́@������",	},// No. 2 ������ A(����)
		{	"�Õ��Ȗ��@�g��\\n" 	"���J ������ (����)\\n" 	"�C�����[�W�������[�U�[\\n" "�}�X�^�[�X�p�[�N\\n",			"�ړ����x������\\n" 				"�U���͈́�����\\n" 	"�U���́@������",	},// No. 3 ������ B(����)
		{	"�z���S\\n" 			"���~���A �X�J�[���b�g\\n"	"�i�C�g�_���X\\n"			"�s���u���b�h\\n",			"�ړ����x������\\n" 				"�U���͈́�����\\n" 	"�U���́@������",	},// No. 4 ���~���A(����)
		{	"����̖S��\\n" 		"���s�� �H�X�q\\n"			"�Ί݂̗U��\\n" 			"�M���X�g���h���[��\\n",		"�ړ����x������\\n" 				"�U���͈́�����\\n" 	"�U���́@������",	},// No. 5 �H�X�q(����)
		{	"�Ώ�̕X��\\n" 		"�`���m (�X��)\\n"			"�A�C�V�N���j�[�h��\\n" 	"�A�C�V�N���X�g���C�N\\n",		"�ړ����x��������\\n"				"�U���͈́H�H�H\\n" 	"�U���́@������",	},// No. 6 �`���m A(�X��)
	//	{	"�Ώ�̕X��\\n" 		"�`���m (�H��)\\n"			"�A�C�V�N���j�[�h��\\n" 	"�K�b�`�K�`\\n",				"�ړ����x������������������\\n" 	"�U���͈́H�H�H\\n" 	"�U���́@�H�H�H",	},// No. 7 �`���m Q(�H�n)
		{	"�Ώ�̕X��\\n" 		"�`���m (�H��)\\n"			"���A�C�X�\\�[�h\\n" 		"�Ⓚ��+99999\\n",				"�ړ����x����������\\n" 		 	"�U���͈́H�H�H\\n" 	"�U���́@�H�H�H",	},// No. 7 �`���m Q(�H�n)
		{	"Easy\\n"				"�ȒP���ǂ����͐l�ɂ��B\\n",															"�G���f�B���O����\\n"	"(�S�T��)", },// No. 0 easy
		{	"Normal\\n" 			"���ʂȐl�ɂ̓n�[�h�B\\n",																"(�S�U��)", },// No. 1 normal
		{	"Hard\\n"				"�n�[�h�Ȑl�ɂ͕��ʁB\\n",																"(�S�U��)", },// No. 2 hard
		{	"Lunatic\\n"			"�����K�v�B\\n",																		"(�S�U��)", },// No. 3 lunatic
		// 1�ߖ�(��)																										// 2�ߖ�(���F)
	};/* �X�� == Icicle == �A�C�V�N�� */
	//
	const unsigned int aaa = (spec_message_00_SELECT_PLAYER==set_mode)?((unsigned)cg_game_select_player):((8)+((unsigned)cg.game_difficulty));
	// ���������߂�B													// �F�����߂�B 				// �`�悷��B
	strcpy(my_font_text, (char *)const_player_spec_str[((aaa))][0]);	kanji_color(7);/*(���F)*/		kanji_draw();
	strcpy(my_font_text, (char *)const_player_spec_str[((aaa))][1]);	kanji_color(14);/*(���F)*/		kanji_draw();
	strcpy(my_font_text, (spec_message_00_SELECT_PLAYER==set_mode)?("\\n�N���s���́H"):("\\n\\n\\n\\n\\n���[�h��I�����Ă�") );
																		kanji_color(9);/*(��/���F)*/	kanji_draw();
}
// "�ȒP�Ɍ����΁A�{���Q�ł��B"
// "�ȒP�ɂ͍U���ł��Ȃ��B"
// "���ʂɌ����΁A����ł��B"
// "���d�������΁A�������ł��B"
// "�����݂Ɍ����΁A�������ł��B"

#define SOZAI_HABA_184 (185-1)		/* �f�ޕ�184[�h�b�g] */
#define SOZAI_TATE_064 ( 64-1)		/* �f�ޕ� 63[�h�b�g] */

/*---------------------------------------------------------
	���[�J���g�p�ϐ��B
---------------------------------------------------------*/

static int sp_scale256; 	 /*(���@��I�����Ȃ����ƁA�m�肷��܂ŁA����Ă���X���W)*/
static int sp_diff256;
static int is_turn_right;
static SDL_Surface *difficulty_select_fg0_surface;
static SDL_Surface *player_select_bg0_surface;
static SDL_Surface *player_select_bg2_surface;

/*(�L���b�V���ɂ����ƁA�A���t�@���o�Ȃ��B)*/
#define DFF_USE_KETM_IMAGE_CHACHE (0)

/*---------------------------------------------------------
	�����\���ړ�����
---------------------------------------------------------*/
static int kanji_set_x_position;
static int kanji_move_x_position;
static void move_kanji(void)
{
	const int sabun = (kanji_move_x_position-kanji_set_x_position);
	if (0 != sabun)
	{
		const int value = (32 - __builtin_allegrex_clz(sabun - 1));
		if (0 > sabun)
		{
			kanji_move_x_position += value;
		}
		else
		{
			kanji_move_x_position -= value;
		}
	}
	unsigned int ii;
	for (ii=0; ii<7; ii++)
	{
		ml_font[ii].x		= (kanji_move_x_position);/* X�ʒu */
	}
}


/*---------------------------------------------------------
	[�v���C���[�I�����j���[]
---------------------------------------------------------*/
static void player_select_08_load(void);/*(�錾���K�v)*/
static void player_select_10_select(void)/* [�I��] */
{
	cg.msg_time = byou60(5);/* �K�v */	/* �� 5 �b */
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* �����E���V���b�g���L�����Z���̂��Âꂩ */
		{	/* ��Ԃ��ς��ꍇ��*/
			/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
					#if (1==USE_KETM_IMAGE_CHACHE)
			unloadbmp_by_surface(player_select_bg0_surface);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_bg0_surface = NULL;
			unloadbmp_by_surface(player_select_bg2_surface);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_bg2_surface = NULL;
					#else
			SDL_FreeSurface(player_select_bg0_surface); // �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��B
			player_select_bg0_surface = NULL;
			SDL_FreeSurface(player_select_bg2_surface); // �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��B
			player_select_bg2_surface = NULL;
					#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
		//
			if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*�����E�̗���*/) /* ���E�{�^������ */
			{
				is_turn_right = (psp_pad.pad_data & PSP_KEY_RIGHT)?1:0;/* �E�Ȃ�1, ���Ȃ�0 */
				voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);
				cb.main_call_func = player_select_08_load;	/* [load] */	/*(�߂�)*/
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				cg.msg_time = (0);/* �K�v */
				{
					/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
					#if (1==DFF_USE_KETM_IMAGE_CHACHE)
					unloadbmp_by_surface(difficulty_select_fg0_surface);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bdifficulty_select_fg0_surface = NULL;
					#else
					SDL_FreeSurface(difficulty_select_fg0_surface); // �L���b�V���ɓ����Ă�̂� NULL �ɏo���Ȃ��B
					difficulty_select_fg0_surface = NULL;
					#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
				}
				voice_play_menu(VOICE01_MENU_OK, TRACK01_MENU01);/* �e�L�g�[ */
				if (0!=cg.game_practice_mode)
				{	/* �v���N�e�B�X���[�h�̏ꍇ */
					cb.main_call_func = stage_select_menu_start;	/* �X�e�[�W�I�����j���[�̊J�n */
				}
				else/* �v���N�e�B�X���[�h�ȊO�̏ꍇ */
				{
					cb.main_call_func = stage_first_init;			/* �Q�[���J�n */
				}
			}
			else
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
			{
				cg.msg_time = (0);/* �K�v */
				voice_play_menu(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
				/* ��Փx�I�����j���[�ɖ߂� */
				cb.main_call_func = rank_select_menu_start; 	/* ��Փx�I�����j���[��(�߂�) */
			}
		}
	}
	move_kanji();
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static int toutatu_wariai;

/* �o�ꑬ�x�́A�������B */
	#define SPEED_04_FADE_IN	(4) 	/* 4*FPS_MENU_FACTOR */
/* �ޔ𑬓x�́A�����B */
	#define SPEED_06_FADE_OUT	(6) 	/* 6*FPS_MENU_FACTOR */


static int move_mode;
/*(�v4���==3���+1���)*/
enum
{
	MODE_00_TOUJYOU = 0,			/*(move_mode[0]: ���j���[�����o�ꒆ)*/
	MODE_01_MENU_SELECT,			/*(move_mode[1]: ���j���[�I��)*/
	MODE_02_TAIJYOU_EASY,			/*(move_mode[2]: ���j���[�����ޔ�)*/
	MODE_03_TAIHI_ITI_EASY, 		/*(+�ޔ��ʒu�̐�ǂݕ�)*/
	MODE_04_TAIJYOU_NORMAL, 		/*(move_mode[2]: ���j���[�����ޔ�)*/
	MODE_05_TAIHI_ITI_NORMAL,		/*(+�ޔ��ʒu�̐�ǂݕ�)*/
	MODE_06_TAIJYOU_HARD,			/*(move_mode[2]: ���j���[�����ޔ�)*/
	MODE_07_TAIHI_ITI_HARD, 		/*(+�ޔ��ʒu�̐�ǂݕ�)*/
	MODE_08_TAIJYOU_LUNATIC,		/*(move_mode[2]: ���j���[�����ޔ�)*/
	MODE_09_TAIHI_ITI_LUNATIC,		/*(+�ޔ��ʒu�̐�ǂݕ�)*/
	MODE_10_MAX_JYOUTAI/*(�ő�l)*/
};
typedef struct
{
	s16 x_offset[MODE_10_MAX_JYOUTAI];
	s16 y_offset[MODE_10_MAX_JYOUTAI];

} MENU222_RESOURCE;
static const MENU222_RESOURCE my_menu_resource[MODE_10_MAX_JYOUTAI] =
{
// mode:				   [easy-------]   [normal------]  [hard--------]  [lunatic-----]
// mode:	[0] 	[1] 	[2e]	[2+1]	[4n]	[4+1]	[6h]	[6+1]	[8l]	[8+1]
	{	{	512,	128,	128,   (16),	128,	  0,	128,	  0,	128,	  0,  },
		{	-44,	 16,	 16,  (216),	 16,	272,	 16,	272,	 16,	272,  }, },// <EASY>
	{	{	512,	128,	128,	  0,	128,   (16),	128,	  0,	128,	  0,  },
		{	 50,	 80,	 80,	272,	 80,  (216),	 80,	272,	 80,	272,  }, },// <NORMAL>
	{	{	512,	128,	128,	  0,	128,	  0,	128,   (16),	128,	  0,  },
		{	174,	144,	144,	272,	144,	272,	144,  (216),	144,	272,  }, },// <HARD>
	{	{	512,	128,	128,	  0,	128,	  0,	128,	  0,	128,   (16),  },
		{	268,	208,	208,	272,	208,	272,	208,	272,	208,  (216),  }, },// <LUNATIC>
};


/*---------------------------------------------------------
	��Փx�^�O����`��
---------------------------------------------------------*/
#if 0
	//	(128+toutatu_wariai),/*(�Ƃ肠����)*/
	//	(16)+(tag_number<<6),
//
	//	(16),// (16/*128*/);
	//	(16+(128)-8+(64)), // (16)+(/*jj*/(2)<<6);
		(127)/*set_alpha256*/
// , /*int set_x_offset, int set_y_offset,*/ int set_alpha256
#endif
static void draw_tag_number(u32 tag_number)
{
	int move_rate256;
	move_rate256 = toutatu_wariai;
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
	{
		SDL_Rect ds;
		ds.x = (0);
		ds.y = (/*jj*/((tag_number))<<6);
		ds.w = (SOZAI_HABA_184);	/* �f�ޕ�184[�h�b�g] */ /* �f�ޕ�191[�h�b�g] */
		ds.h = (SOZAI_TATE_064);	/* �f�ޕ� 63[�h�b�g] */
	//
		SDL_Rect dr;
		SDL_Surface *src;
		src 		= difficulty_select_fg0_surface;
		dr.w = (src->w);
		dr.h = (63/*src->h*/);

			#if (1)/*(���B�������l�����āA��������B)*/
			REG_00_SRC_X	= ((my_menu_resource[tag_number].x_offset[move_mode  ])<<8);	/*(�����ʒu[A]t256()�`��)*/
			REG_01_SRC_Y	= ((my_menu_resource[tag_number].y_offset[move_mode  ])<<8);	/*(�����ʒu[A]t256()�`��)*/
			REG_02_DEST_X	= ((my_menu_resource[tag_number].x_offset[move_mode+1])<<8);	/*(�����ʒu[B]t256()�`��)*/
			REG_03_DEST_Y	= ((my_menu_resource[tag_number].y_offset[move_mode+1])<<8);	/*(�����ʒu[B]t256()�`��)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(��������t256()�`��)*/
			multiprex_rate_vector();/*(�j�󃌃W�X�^�����̂Œ���)*/
			#endif
			// ���̎��_�ŁA
			// REG_02_DEST_X: �����l
			// REG_03_DEST_Y: �����l
			// -----------------------
		//	menu_item_my_obj[i].x256 = (REG_02_DEST_X)+(REG_08_REG0);		// �������h�炷�B
		//	menu_item_my_obj[i].y256 = (REG_03_DEST_Y);// +(REG_09_REG1);	// �c�͗h�炳�Ȃ��B(== y offset �͎g��Ȃ��B)

		{	/*(�\���ʒu)*/
			dr.x = ((REG_02_DEST_X)>>8);/*(X�ʒu)*/
			dr.y = ((REG_03_DEST_Y)>>8);/*(Y�ʒu)*/
		//	dr.x = (set_x_offset);
		//	dr.y = (set_y_offset);
		}
		{
			int set_alpha256;
			if ((tag_number==((unsigned)cg.game_difficulty)))/*(�I������Ă���ꍇ)*/
			{
				dr.x -= (16);/*(���ɂ��炷)*/
				dr.y -= (8);/*(��ɂ��炷)*/
				set_alpha256 = (255);/*(�������Ȃ�)*/
			}
			else/*(�I������Ă��Ȃ��ꍇ)*/
			{
				/*(��ʒu����A���炳�Ȃ�)*/
				set_alpha256 = (64);// (127);/*(����������)*/
			}
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, set_alpha256);/*(�������ݒ�A�]�����摜�̔��������L��)*/
		}
		PSPL_UpperBlit(src, &ds, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);/*(���z��ʂ֕`��)*/
	}
}


/*---------------------------------------------------------
	��Փx�^�O���l�`��
---------------------------------------------------------*/
static void move_tag_4(void)/* [�`��444] */
{
	u32 tag_number;
	for (tag_number=0; tag_number<4; tag_number++)
	{
		draw_tag_number(tag_number);
	}
}


/*---------------------------------------------------------
	[�v���C���[�I�����j���[]
---------------------------------------------------------*/
static void player_select_09_move_draw(void)/* [�`��1] */
{
//	sp_scale256 -= (18);	// 18 == 255/14[��]�B14���`�悷��B/*(�Â��z)*/
//	sp_scale256 -= (15);	// 15 == 255/17[��]�B17���`�悷��B [�����G��PSPL�`��] + [BG��PSPL�`��]�̏ꍇ�̐ݒ�l(15)�B(-r35u1)
	sp_scale256 -=	(8);	//	8 == 255/32[��]�B32���`�悷��B [�����G��Gu�`��] + [BG��PSPL�`��]�̏ꍇ�̐ݒ�l(15)�B(r35u2-)
	if (sp_scale256 < 0 )
	{
		sp_scale256 = (0);
		cb.main_call_func = player_select_10_select;/* [�I��] */
	}
	SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256) );
	PSPL_UpperBlit(player_select_bg2_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	PSPL_UpperBlit(player_select_bg0_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	/* �v���C���[(1&2) / �w�i(1&2) / �`�� */
	{
		/*(Gu�̏ꍇ�A��b�X�v���C�g���g��)*/
		#define PLAYER_OBJ_LOCATE_X 	(256-32)		/* cx256(r35) �f��[256x256] */
		#define PLAYER_OBJ_LOCATE_Y 	(8) 			/* cy256(r35) �f��[256x256] */
			kaiwa_sprite[0].center.y256 = ((PLAYER_OBJ_LOCATE_Y)<<8);
		{int aaa;
			sp_diff256 += (sp_scale256);
			aaa = (((sp_diff256))>>8)-16;
			if ((  is_turn_right))	{	aaa = -(aaa);}
			kaiwa_sprite[0].center.x256 = ((PLAYER_OBJ_LOCATE_X)<<8) + ((aaa)<<8);
		}
			kaiwa_sprite[0].color32 = ((	sp_scale256)<<24)|(0x00ffffff);/*(����)*/
		//
			kaiwa_sprite[1].center.y256 = ((PLAYER_OBJ_LOCATE_Y)<<8);
		{int aaa;
			sp_diff256 += (sp_scale256);
			aaa = (((sp_diff256)>>8)-16);
			if (1-is_turn_right)	{	aaa = (-(aaa));}
			kaiwa_sprite[1].center.x256 = ((PLAYER_OBJ_LOCATE_X)<<8) + ((aaa)<<8);
		}
			kaiwa_sprite[1].color32 = ((255-sp_scale256)<<24)|(0x00ffffff);/*(����)*/
	}
	/* ��Փx�^�O��`�� */
	move_tag_4();
	#if 0
	draw_tag_number(cg.game_difficulty);
	#endif
	move_kanji();
}


/*---------------------------------------------------------
	�I���v���C���[�摜�Ɣw�i�̖��O�����߂�B
---------------------------------------------------------*/
static void set_file_name(unsigned char is_pl)
{
	strcpy(my_file_common_name, (char *)DIRECTRY_NAME_DATA_STR "/select/" "pl00.png");
	my_file_common_name[DIRECTRY_NAME_DATA_LENGTH+(10)] = '0'+(cg_game_select_player);
	my_file_common_name[DIRECTRY_NAME_DATA_LENGTH+(11)] = '0'+(is_pl);
}


/*---------------------------------------------------------
	[�v���C���[�I�����j���[]�I���v���C���[�摜�Ɣw�i�����[�h�B
---------------------------------------------------------*/
static MAIN_CALL_FUNC(player_select_08_load)/* [load] */
{
	(cg_game_select_player) &= (0x07);
	//
	set_file_name(0);	strcpy(&my_resource[TEX_09_TACHIE_L+0].file_name[0], (&my_file_common_name[0]) );
						kaiwa_sprite[0].draw_flag = (1);	/* �`�悷��B */
						psp_load_texture(TEX_09_TACHIE_L+0);
	set_file_name(1);
	#if (1==USE_KETM_IMAGE_CHACHE)
	player_select_bg2_surface = load_chache_bmp();
	#else
	player_select_bg2_surface = IMG_Load(my_file_common_name);
	#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
	//
	if (0==is_turn_right)
			{	(cg_game_select_player)--;	}
	else	{	(cg_game_select_player)++;	}
	(cg_game_select_player) &= 0x07;
	//
	set_file_name(0);	strcpy(&my_resource[TEX_09_TACHIE_L+1].file_name[0], (&my_file_common_name[0]) );
						kaiwa_sprite[1].draw_flag = (1);	/* �`�悷��B */
						psp_load_texture(TEX_09_TACHIE_L+1);
	set_file_name(1);
	#if (1==USE_KETM_IMAGE_CHACHE)
	player_select_bg0_surface = load_chache_bmp();
	#else
	player_select_bg0_surface = IMG_Load(my_file_common_name);
	#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
	//
	sp_scale256 	= (255);
	sp_diff256		= (0);
	//
	kanji_set_x_position = (24);/*(�����ړ�����W�ݒ�)*/
	draw_spec_message(spec_message_00_SELECT_PLAYER);	// [�v���C���[�I��]��Ԃ����b�Z�[�W�\���B
	cb.main_call_func = player_select_09_move_draw;
}


/*---------------------------------------------------------
	�L�����Z������炵�āA�^�C�g�����j���[�ֈړ��B
	���ʃT�u�B(�O���̂�����������Ă΂��)
---------------------------------------------------------*/
global void menu_cancel_and_voice(void)
{
	voice_play_menu(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
	cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
}


/*---------------------------------------------------------
	[��Փx�I�����j���[:�����ޔ�]
---------------------------------------------------------*/
static MAIN_CALL_FUNC(rank_select_07_moji_taihi)/* [�����ޔ�] */
{
	toutatu_wariai += (SPEED_06_FADE_OUT);
	if (255 <= (toutatu_wariai) )
	{
		toutatu_wariai = (0);
		move_mode++;// (3=2+1=easy, 5=4+1=normal, 7=6+1=hard, 9=8+1=lunatic)�ɂȂ�B
		cb.main_call_func = player_select_08_load;
	}
	psp_pop_screen();
	move_tag_4();
	/*(???)*/move_kanji();/*(����Ӗ��������ۂ�)*/
}


/*---------------------------------------------------------
	[��Փx�I�����j���[]
---------------------------------------------------------*/
static MAIN_CALL_FUNC(rank_select_04_initialize_value);/*(�錾���K�v)*/
static MAIN_CALL_FUNC(rank_select_06_select)/* [�I��] */
{
	cg.msg_time = byou60(5);/* �K�v */	/* �� 5 �b */
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* �����E���V���b�g���L�����Z���̂��Âꂩ */
		{	/* ��Ԃ��ς��ꍇ��*/
		//
			if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN)/*�ォ���̗���*/ ) /* �㉺�{�^������ */
			{
				if (psp_pad.pad_data & PSP_KEY_UP)
						{	cg.game_difficulty--;	cg.game_difficulty = psp_max(cg.game_difficulty, (0)   );	}	/* ���j���[��Œ�~ */
				else	{	cg.game_difficulty++;	cg.game_difficulty = psp_min(cg.game_difficulty, (4-1) );	}	/* ���j���[���Œ�~ */
				voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);
				cb.main_call_func = rank_select_04_initialize_value;	/* [load] */	/*(�߂�)*/
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				cg.msg_time = (0);/* �K�v */
				voice_play_menu(VOICE01_MENU_OK, TRACK01_MENU01);/* �e�L�g�[ */
				/* �ʏ�^�v���N�e�B�X�A�Q�[���J�n */
				// static void player_opt_init(void)�z���B�Ȃ�
				{
					is_turn_right = 0;/* �K�v */
					(cg_game_select_player)++;/* �v���O����player_select_work()�̓s���㒲�� */	/*player_select_work()::[init]*/
				}
				cb.main_call_func = rank_select_07_moji_taihi;
				move_mode++;// (2)�ɂȂ�B
				move_mode += ((cg.game_difficulty)<<1);// (2=easy, 4=normal, 6=hard, 8=lunatic)�ɂȂ�B
			}
			else
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
			{
				cg.msg_time = (0);/* �K�v */
				{
					/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
				//	unloadbmp_by_surface(difficulty_select_fg0_surface);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bdifficulty_select_fg0_surface = NULL;
					/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
					#if (1==DFF_USE_KETM_IMAGE_CHACHE)
					unloadbmp_by_surface(difficulty_select_fg0_surface);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bdifficulty_select_fg0_surface = NULL;
					#else
					SDL_FreeSurface(difficulty_select_fg0_surface); 		// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��B
					difficulty_select_fg0_surface = NULL;
					#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
				}
				menu_cancel_and_voice();	/* �^�C�g�����j���[�ֈړ� */
			}
		}
	}
	move_kanji();
}

/*---------------------------------------------------------
	[��Փx�I�����j���[]
---------------------------------------------------------*/
static MAIN_CALL_FUNC(rank_select_05_move_draw)/* [�`��1] */
{
	sp_scale256 -= 15/*18*/;
	if (sp_scale256 < 0 )
	{
		sp_scale256 = (0);
		cb.main_call_func = rank_select_06_select;/* [�����W�܂�] */
	}
//	SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256) );
//	PSPL_UpperBlit(player_select_bg0_surface, NULL, sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	psp_pop_screen();
	move_tag_4();
	move_kanji();/*(����Ӗ��������ۂ�)*/
}

/*---------------------------------------------------------
	[��Փx�I�����j���[]�ϐ������J�n��Ԃ֏������ݒ�B
---------------------------------------------------------*/
static MAIN_CALL_FUNC(rank_select_04_initialize_value)
{
	(cg.game_difficulty) &= (0x03);
	//
	sp_scale256 	= (255);
	sp_diff256		= (0);
	//
	kanji_set_x_position = (256+32);/*(�����ړ�����W�ݒ�)*/
	draw_spec_message(spec_message_01_SELECT_RANK); 	// [��Փx�I��]��Ԃ����b�Z�[�W�\���B
	cb.main_call_func = rank_select_05_move_draw;
}

/*---------------------------------------------------------
	[��Փx�I�����j���[:�����W�܂�]
---------------------------------------------------------*/
static MAIN_CALL_FUNC(rank_select_03_atumaru)/* [�����W�܂�] */
{
	toutatu_wariai += (SPEED_06_FADE_OUT);
	if (255 <= (toutatu_wariai) )
	{
		toutatu_wariai = (0);
		move_mode++;// (1)�ɂȂ�B
		cb.main_call_func = rank_select_04_initialize_value;/* [�I��] */
	}
	psp_pop_screen();
	move_tag_4();
	/*(???)*/move_kanji();/*(����Ӗ��������ۂ�)*/
}


/*---------------------------------------------------------
	[��Փx�I�����j���[#02]�̏������ݒ�B(��蒼���̏ꍇ�͂�������)
---------------------------------------------------------*/
extern void kaiwa_obj_set_256(void);
global MAIN_CALL_FUNC(rank_select_menu_start)
{
	/* [load] */
	{
		strcpy(my_file_common_name, (char *)DIRECTRY_NAME_DATA_STR "/teki/" "teki00_256.png" );
		#if (1==DFF_USE_KETM_IMAGE_CHACHE)
		difficulty_select_fg0_surface = load_chache_bmp();
		#else
		difficulty_select_fg0_surface = IMG_Load(my_file_common_name);
		#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
		SDL_SetColorKey(difficulty_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
	}
	{
		#if (1)/*(�����֘A�̏�����)*/
		set_kanji_origin_xy(24, 64);/*(�\�����_�̐ݒ�)*/
		set_kanji_origin_kankaku(19);/*(���Ԃ������L����)*/
		set_kanji_hide_line(ML_LINE_99_MAX);/*(�S�s�\������B)*/
		kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
		cg.msg_time = (65536);	/* �� 18 �� */
		#endif
	{
		/* 8�s�ڂ̂ݓ���Ȉʒu�ֈړ��B */
		ml_font[ML_LINE_08].x		= (16);/* X�ʒu */
		ml_font[ML_LINE_08].y		= (16);/* Y�ʒu */
	}
//		kanji_window_clear_line(5); /* �����E�B���h�E��1�s��(==0)�̓��e�������B */
//		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (5) ); /* �J�[�\����1�s��(==0)�ֈړ� */
//		//
//		kanji_color((9));
//		strcpy(my_font_text, "���[�h��I�����Ă�"	 );
//		kanji_draw();
	}
	kaiwa_obj_set_256();
	#if (1)
	{	/*(��bobj��`�悵�Ȃ��B)*/
		kaiwa_sprite[0].draw_flag = (0);	/* �`�悵�Ȃ��B */
		kaiwa_sprite[1].draw_flag = (0);	/* �`�悵�Ȃ��B */
	}
	#endif
	/*(��bobj���t����ʂŕ\��)*/
	{	/*(�t�����=="Story" ���[�h��p�̗����G�ړ�)*/
		cg.side_panel_draw_flag 		= (0);/*(�E�T�C�h�̃X�R�A�p�l���\��off)*/
	}
	//
	move_mode			= (0);
	toutatu_wariai		= (0);
	cb.main_call_func = rank_select_03_atumaru;
}
