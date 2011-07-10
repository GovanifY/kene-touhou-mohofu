
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�Q�[���J�n���̓�Փx/�v���C���[�I��
	-------------------------------------------------------
	[��Փx�I�����j���[]
	[�v���C���[�I�����j���[]
---------------------------------------------------------*/

#include "kanji_system.h"
#include "111_my_file.h"/*(my_file_common_name)*/

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
		{	"�Ώ�̕X��\\n" 		"�`���m (�H��)\\n"			"�A�C�V�N���j�[�h��\\n" 	"�K�b�`�K�`\\n",				"�ړ����x������������������\\n" 	"�U���͈́H�H�H\\n" 	"�U���́@�H�H�H",	},// No. 7 �`���m Q(�H�n)
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
	strcpy(my_font_text, (spec_message_00_SELECT_PLAYER==set_mode)?("\\n�N���s���́H"):("\\n\\n\\n\\n\\n���[�h��I�����Ă�")	 );
																		kanji_color(9);/*(��/���F)*/	kanji_draw();
}
	//	"Easy\\n"		"�ȒP�Ɍ����΁A�{���Q�ł��B\\n" 		"(�����ƑS�U��)", // No. 0 easy
	//	"Normal\\n" 	"���ʂɌ����΁A����ł��B\\n" 		"�@�@�@�@(�S�U��)", // No. 1 normal
	//	"Hard\\n"		"���d�������΁A�������ł��B\\n" 		"�@�@�@�@(�S�U��)", // No. 2 hard
	//	"Lunatic\\n"	"�����݂Ɍ����΁A�������ł��B\\n"		"�@�@�@�@(�S�U��)", // No. 3 lunatic
	//#if (1==US E_EASY_BADEND)
	//#else
	//	"Easy\\n"		"�ȒP�ɂ͍U���ł��Ȃ��B\\n" 			"�@�@�@�@(�S�U��)", // No. 0 easy
	//#endif

#define SOZAI_HABA_184 (185-1)		/* �f�ޕ�184[�h�b�g] */
#define SOZAI_TATE_064 ( 64-1)		/* �f�ޕ� 63[�h�b�g] */

/*---------------------------------------------------------
	���[�J���g�p�ϐ��B
---------------------------------------------------------*/
static SDL_Surface *player_select_bg0_surface;
static SDL_Surface *player_select_bg2_surface;
static SDL_Surface *player_select_fg0_surface;
static SDL_Surface *player_select_fg2_surface;

static int sp_scale256;
static int sp_diff256;
static int is_turn_right;
static SDL_Surface *difficulty_select_fg0_surface;




/*---------------------------------------------------------
	�����\���ړ�����
---------------------------------------------------------*/
static int kanji_set_x_position;
static int kanji_move_x_position;
static void move_kanji(void)
{
	const int sabun = (kanji_move_x_position-kanji_set_x_position);
	if (0 != (sabun))
	{
		const int value = (32 - __builtin_allegrex_clz(sabun - 1));
		if ( 0 > (sabun) )
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
static void player_select_03_load(void);/*(�錾���K�v)*/
static void player_select_05_select(void)/* [�I��] */
{
	cg.msg_time = byou60(5);/* �K�v */	/* �� 5 �b */
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* �����E���V���b�g���L�����Z���̂��Âꂩ */
		{	/* ��Ԃ��ς��ꍇ��*/
			/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
			unloadbmp_by_surface(player_select_bg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_bg0_surface = NULL;
			unloadbmp_by_surface(player_select_fg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_fg0_surface = NULL;
			unloadbmp_by_surface(player_select_bg2_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_bg2_surface = NULL;
			unloadbmp_by_surface(player_select_fg2_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_fg2_surface = NULL;
		//
			if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*�����E�̗���*/  ) /* ���E�{�^������ */
			{
				is_turn_right = (psp_pad.pad_data & PSP_KEY_RIGHT)?1:0;/* �E�Ȃ�1, ���Ȃ�0 */
				voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				cb.main_call_func = player_select_03_load;	/* [load] */	/*(�߂�)*/
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				cg.msg_time = (0);/* �K�v */
				{
					/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
					unloadbmp_by_surface(difficulty_select_fg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bdifficulty_select_fg0_surface = NULL;
				}
				voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* �e�L�g�[ */
				if (0!=cg.game_practice_mode)
				{/* �v���N�e�B�X���[�h�̏ꍇ */
					cb.main_call_func = stage_select_menu_start;	/* �X�e�[�W�I�����j���[�̊J�n */
				}
				else
				{
					cb.main_call_func = stage_first_init;			/* �Q�[���J�n */
				}
			}
			else
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
			{
				cg.msg_time = (0);/* �K�v */
				voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
				/* ��Փx�I�����j���[�ɖ߂� */
				cb.main_call_func = difficulty_select_menu_start;	/* ��Փx�I�����j���[��(�߂�) */
			}
		}
	}
	move_kanji();
}

/*---------------------------------------------------------
	[�v���C���[�I�����j���[]
---------------------------------------------------------*/
static void player_select_04_move_draw(void)/* [�`��1] */
{
	sp_scale256 -= (15);/*18*/
	if (sp_scale256 < 0 )
	{
		sp_scale256 = (0);
		cb.main_call_func = player_select_05_select;/* [�I��] */
	}
	SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256) );
	PSPL_UpperBlit(player_select_bg2_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	PSPL_UpperBlit(player_select_bg0_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	/* �v���C���[(1&2) / �w�i(1&2) / �`�� */
	{
	//	#define PLAYER_OBJ_LOCATE_X 	(250)		/* (r32) �f��[200x200] */
	//	#define PLAYER_OBJ_LOCATE_Y 	(50)		/* (r32) �f��[200x200] */
		#define PLAYER_OBJ_LOCATE_X 	(256-32)	/* (r33) �f��[256x256] */
		#define PLAYER_OBJ_LOCATE_Y 	(8) 		/* (r33) �f��[256x256] */
		SDL_Rect dr;
		dr.y = PLAYER_OBJ_LOCATE_Y;
		SDL_Surface *src;
	//	player_opt_img256(player_select_fg2_surface, (256-sp_scale256), (  is_turn_right)/*1*/);	/* �E1 ��0 */
	//	player_opt_img256(player_select_fg0_surface, (	  sp_scale256), (1-is_turn_right)/*0*/);	/* �E0 ��1 */
	//static void player_opt_img256() /* 0==r, 1==l, */
		{int aaa;
			sp_diff256 += (sp_scale256);
			aaa = (((sp_diff256))>>8)-16;
			if ((  is_turn_right))	{	aaa = -(aaa);}
			dr.x = PLAYER_OBJ_LOCATE_X + (aaa);
		}
		src 		= player_select_fg2_surface;
		dr.w = (src->w);
		dr.h = (src->h);
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, (	  sp_scale256));
		PSPL_UpperBlit(src, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		{int aaa;
			sp_diff256 += (sp_scale256);
			aaa=(((sp_diff256))>>8)-16;
			if ((1-is_turn_right))	{	aaa = -(aaa);}
			dr.x = PLAYER_OBJ_LOCATE_X+(aaa);
		}
		src 		= player_select_fg0_surface;
		dr.w = (src->w);
		dr.h = (src->h);
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, (255-sp_scale256));
		PSPL_UpperBlit(src, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
	}
	/* ��Փx�^�O��`�� */
	{
		SDL_Rect ds;
		ds.x = (0);
		ds.y = (/*jj*/((cg.game_difficulty))<<6);
		ds.w = (SOZAI_HABA_184);	/* �f�ޕ�191[�h�b�g] */
		ds.h = (SOZAI_TATE_064);	/* �f�ޕ� 63[�h�b�g] */
	//
		SDL_Rect dr;
		SDL_Surface *src;
		src 		= difficulty_select_fg0_surface;
		dr.w = (src->w);
		dr.h = (63/*src->h*/);
		{	/*(�\���ʒu)*/
			dr.x = (16);//(16/*128*/);
			dr.y = (16+(128)-8+(64));//(16)+(/*jj*/(2)<<6);
		}
	//	if ((jj==difficulty))
	//	{
	//		dr.x -= (8);
	//		dr.y -= (8);
	//	}
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, ((/*(jj==difficulty)?(255):*/(127))));
		PSPL_UpperBlit(src, &ds, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
	}
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

static void player_select_03_load(void)/* [load] */
{
	(cg_game_select_player) &= (0x07);
	//
	set_file_name(0);	player_select_fg2_surface = load_chache_bmp();
	set_file_name(1);	player_select_bg2_surface = load_chache_bmp();
	SDL_SetColorKey(player_select_fg2_surface, (SDL_SRCCOLORKEY), 0x00000000);
	//
	if (0==is_turn_right)
			{	(cg_game_select_player)--;	}
	else	{	(cg_game_select_player)++;	}
	(cg_game_select_player) &= 0x07;
	//
	set_file_name(0);	player_select_fg0_surface = load_chache_bmp();
	set_file_name(1);	player_select_bg0_surface = load_chache_bmp();
	SDL_SetColorKey(player_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
	//
	sp_scale256 	= (255);
	sp_diff256		= (0);
	//
	kanji_set_x_position = (24);/*(�����ړ�����W�ݒ�)*/
	draw_spec_message(spec_message_00_SELECT_PLAYER);	// [�v���C���[�I��]��Ԃ����b�Z�[�W�\���B
	cb.main_call_func = player_select_04_move_draw;
}


/*---------------------------------------------------------
	�L�����Z������炵�āA�^�C�g�����j���[�ֈړ��B
	���ʃT�u�B(�O���̂�����������Ă΂��)
---------------------------------------------------------*/

global void menu_cancel_and_voice(void)
{
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
	cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
}


/*---------------------------------------------------------
	[��Փx�I�����j���[]
---------------------------------------------------------*/
static void rank_select_00_initialize_value(void);/*(�錾���K�v)*/
static void rank_select_02_select(void)/* [�I��] */
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
				voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				cb.main_call_func = rank_select_00_initialize_value;	/* [load] */	/*(�߂�)*/
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				cg.msg_time = (0);/* �K�v */
				voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* �e�L�g�[ */
				/* �ʏ�^�v���N�e�B�X�A�Q�[���J�n */
				// static void player_opt_init(void)�z���B�Ȃ�
				{
					is_turn_right = 0;/* �K�v */
					(cg_game_select_player)++;/* �v���O����player_select_work()�̓s���㒲�� */	/*player_select_work()::[init]*/
				}
				cb.main_call_func = player_select_03_load;
			}
			else
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
			{
				cg.msg_time = (0);/* �K�v */
				{
					/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
					unloadbmp_by_surface(difficulty_select_fg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bdifficulty_select_fg0_surface = NULL;
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
static void rank_select_01_move_draw(void)/* [�`��1] */
{
	sp_scale256 -= 15/*18*/;
	if (sp_scale256 < 0 )
	{
		sp_scale256 = (0);
		cb.main_call_func = rank_select_02_select;/* [�I��] */
	}
//	SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256) );
//	PSPL_UpperBlit(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
	psp_pop_screen();
	u32 jj;
	for (jj=0; jj<4; jj++)
	{
		SDL_Rect ds;
		ds.x = (0);
		ds.y = (jj<<6);
		ds.w = (SOZAI_HABA_184);	/* �f�ޕ�184[�h�b�g] */
		ds.h = (SOZAI_TATE_064);	/* �f�ޕ� 63[�h�b�g] */
	//
		SDL_Rect dr;
		SDL_Surface *src;
		src 		= difficulty_select_fg0_surface;
		dr.w = (src->w);
		dr.h = (63/*src->h*/);
		{
			dr.x = (128);
			dr.y = (16)+(jj<<6);
		}
		if ((jj==((unsigned)cg.game_difficulty)))
		{
			dr.x -= (8);
			dr.y -= (8);
		}
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, (((jj==((unsigned)cg.game_difficulty))?(255):(127))));
		PSPL_UpperBlit(src, &ds, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
	}
	move_kanji();/*(����Ӗ��������ۂ�)*/
}

/*---------------------------------------------------------
	[��Փx�I�����j���[]�ϐ������J�n��Ԃ֏������ݒ�B
---------------------------------------------------------*/
static void rank_select_00_initialize_value(void)
{
	(cg.game_difficulty) &= (0x03);
	//
	sp_scale256 	= (255);
	sp_diff256		= (0);
	//
	kanji_set_x_position = (256+32);/*(�����ړ�����W�ݒ�)*/
	draw_spec_message(spec_message_01_SELECT_RANK); 	// [��Փx�I��]��Ԃ����b�Z�[�W�\���B
	cb.main_call_func = rank_select_01_move_draw;
}

/*---------------------------------------------------------
	[��Փx�I�����j���[]�̊J�n�B
---------------------------------------------------------*/
global void difficulty_select_menu_start(void)
{
//	psp_clear_screen();
	/* [init] */
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ */
	{
		psp_push_screen();
		/* [load] */
		{
			strcpy(my_file_common_name, (char *)DIRECTRY_NAME_DATA_STR "/teki/" "teki00_256.png" );
			difficulty_select_fg0_surface = load_chache_bmp();
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
		cb.main_call_func = rank_select_00_initialize_value;
	}
}
