
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�Q�[���J�n���̓�Փx/�v���C���[�I��
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

//global int (cg_game_select_player);

static int is_turn_right;
static SDL_Surface *difficulty_select_fg0_surface;

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void ini_save(void); 	// [***090115
/******�����ŃR���p�C������l��******
���̂܂܂��Ɣz�z�o�C�i���ƈႤ�̂�
������"����"��K���ɕς��Ƃ��ĉ������B
************************************/
/* �`���h (chilled) �́A��p����邱�ƁB
�u��₷�v���Ӗ�����p��̓����`�� (chill) �̉ߋ������ł���B
���� chillno ����Ȃ��� cirno �Ȃ񂾂�ȁH */

//#define YUYUKO_PASSWORD		"TABERARENAINO9"
//#define CIRNO_PASSWORD		"CIRNO9PASSWORD"
//#define REMILIA_PASSWORD		"RUMIAISMYBRIDE"

//	/*static*/ char str_pa ss_word[32/*20*/];		/* [***090222 */
global void game_clear_set_password(void)
{
#if 0
	if (0 < difficulty)/* NORMAL �ȏ�ŃN���A���� */
	{
		/* [***090222	�ǉ� */
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) /* ���ɃN���A���Ă��� */
		{
		/*	strcpy(str_pass_word, FURAN_PASSWORD)*/;	/* ??? �� �H�X�q �� �`���m �� ���~���A �J�� */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))	/* ���ɃN���A���Ă��� */
		{
			strcpy(str_pass_word, YUYUKO_PASSWORD); /* �H�X�q �� �`���m �� ���~���A �J�� */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD)) /* ���ɃN���A���Ă��� */
		{
			strcpy(str_pass_word, CIRNO_PASSWORD);	/* �`���m �� ���~���A �J�� */
		}
		else
		{
			strcpy(str_pass_word, REMILIA_PASSWORD);	/* ���~���A �J�� */
		}
	}
#endif
	{
		ini_save();
	}
}



//player select�p

#define MAX_PLAYER (8/*5*/)/*4*/

#define BASE_SP_BG_PNG		(MAX_PLAYER*0)
#define BASE_SP_ST_PNG		(MAX_PLAYER*1)
#define BASE_SP_PNG_MAX 	(MAX_PLAYER*2)

/*---------------------------------------------------------

---------------------------------------------------------*/

static /*const*/const char *player00_res[(BASE_SP_PNG_MAX)] =
{
	/*	0 REIMU */		"select/bg00.png",	//	"select/bg_re.png", 	// "select/sp_reimu_bg.png",
	/*	1 REIMU */		"select/bg01.png",	//	"select/bg_re.png", 	// "select/sp_reimu_bg.png",
	/*	2 MARISA */ 	"select/bg02.png",	//	"select/bg_ma.png", 	// "select/sp_marisa_bg.png",
	/*	3 MARISA */ 	"select/bg03.png",	//	"select/bg_ma.png", 	// "select/sp_marisa_bg.png",
	/*	4 REMILIA */	"select/bg04.png",	//	"select/bg_ma.png", 	// "select/sp_marisa_bg.png",
	/*	5 YUYUKO */ 	"select/bg05.png",	//	"select/bg_oz.png", 	// "select/sp_remiria_bg.png",
	/*	6 CIRNO */		"select/bg06.png",	//	"select/bg_ci.png", 	// "select/sp_cirno_bg.png",
	/*	7 CIRNO */		"select/bg07.png",	//	"select/bg_yu.png", 	// "select/sp_yuyuko_bg.png",
//
	/*	8 REIMU */		"select/pl00.png",	//	"select/p_re.png",		// "select/sp_reimu_st.png",
	/*	9 REIMU */		"select/pl01.png",	//	"select/p_re.png",		// "select/sp_reimu_st.png",
	/* 10 MARISA */ 	"select/pl02.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 11 MARISA */ 	"select/pl03.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 12 REMILIA */	"select/pl04.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 13 YUYUKO */ 	"select/pl05.png",	//	"select/p_oz.png",		// "select/sp_remiria_st.png",
	/* 14 CIRNO */		"select/pl06.png",	//	"select/p_ci.png",		// "select/sp_cirno_st.png",
	/* 15 CIRNO */		"select/pl07.png",	//	"select/p_yu.png",		// "select/sp_yuyuko_st.png",
};


/*---------------------------------------------------------
	�v���C���[�I�����j���[
---------------------------------------------------------*/

static int sp_diff256;

static void draw_player_spec_message(void)
{
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	static const char *const_player_spec_str[(8)] =
	{
		"�z�[�~���O�A�~�����b�g\\n" 	"���z����\\n"						"���x������ �U��������",		// No. 0 �얲 A(�아)
		"�����j\\n" 					"���z���E\\n"						"���x������ �U��������",		// No. 1 �얲 B(����)
		"�}�W�b�N�~�T�C��\\n"			"�X�^�[�_�X�g�����@���G\\n" 		"���x������ �U��������",		// No. 2 ������ A(����)
		"�C�����[�W�������[�U�[\\n" 	"�}�X�^�[�X�p�[�N\\n"				"���x������ �U��������",		// No. 3 ������ B(����)
		"�i�C�g�_���X\\n"				"�s���u���b�h\\n" 				"���x������ �U��������",		// No. 4 ���~���A(����)
		"�Ί݂̗U��\\n" 				"�M���X�g���h���[��\\n" 			"���x������ �U��������",		// No. 5 �H�X�q(����)
		"�A�C�V�N���j�[�h��\\n" 		"�A�C�V�N���X�g���C�N\\n"			"���x������ �U��������",		// No. 6 �`���m A(�X��)
		"�A�C�V�N���j�[�h��\\n" 		"�X�Ƃ�\\n" 						"���x�͂��� �U������",		// No. 7 �`���m Q(�H�n)
	};/* �X�� == Icicle == �A�C�V�N�� */
	print_kanji000((char *)const_player_spec_str[((cg_game_select_player))], /*int color_type*/7, /*int wait*/0);
}

enum
{
	RANK_SELECT_00_LOAD = 0,
	RANK_SELECT_01_MOVE_DRAW,
	RANK_SELECT_02_SELECT,
//
	PLAYER_SELECT_03_LOAD,
	PLAYER_SELECT_04_MOVE_DRAW,
	PLAYER_SELECT_05_SELECT,
};


#define SOZAI_HABA_184 (185-1)		/* �f�ޕ�184[�h�b�g] */
#define SOZAI_TATE_064 ( 64-1)		/* �f�ޕ� 63[�h�b�g] */

static u8 my_ppp_loop;
static void player_select_work(void)
{
	static SDL_Surface *player_select_bg0_surface;
	static SDL_Surface *player_select_bg2_surface;
	static SDL_Surface *player_select_fg0_surface;
	static SDL_Surface *player_select_fg2_surface;
	static int sp_scale256;
//	psp_clear_screen();

	if ((PLAYER_SELECT_03_LOAD)==my_ppp_loop)/* [load] */
	{
		(cg_game_select_player)	&= 7;
//
		player_select_bg2_surface=load_chache_bmp( (char *)player00_res[BASE_SP_BG_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		player_select_fg2_surface=load_chache_bmp( (char *)player00_res[BASE_SP_ST_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg2_surface, (SDL_SRCCOLORKEY), 0x00000000);
		if (0==is_turn_right)
				{	(cg_game_select_player)--;	}
		else	{	(cg_game_select_player)++;	}
		(cg_game_select_player) &= 0x07;
		player_select_bg0_surface=load_chache_bmp( (char *)player00_res[BASE_SP_BG_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		player_select_fg0_surface=load_chache_bmp( (char *)player00_res[BASE_SP_ST_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
//
		sp_scale256 	= (255);
		sp_diff256		= (0);
//
		draw_player_spec_message();
		my_ppp_loop++;
	}
	else
	if ((PLAYER_SELECT_04_MOVE_DRAW)==my_ppp_loop)/* [�`��1] */
	{
		sp_scale256 -= 15/*18*/;
		if (sp_scale256 < 0 )
		{
			sp_scale256 = (0);
			my_ppp_loop++;/* [�I��] */
		}
		SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256)/*bg_alpha_bbb*/);
		SDL_BlitSurface(player_select_bg2_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
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
				sp_diff256+=(sp_scale256);
				aaa=(((sp_diff256))>>8)-16;
				if ((  is_turn_right))	{	aaa = -(aaa);}
				dr.x = PLAYER_OBJ_LOCATE_X+(aaa);
			}
			src 		= player_select_fg2_surface;
			dr.w = (src->w);
			dr.h = (src->h);
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (	  sp_scale256));
			SDL_BlitSurface(src, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
			{int aaa;
				sp_diff256+=(sp_scale256);
				aaa=(((sp_diff256))>>8)-16;
				if ((1-is_turn_right))	{	aaa = -(aaa);}
				dr.x = PLAYER_OBJ_LOCATE_X+(aaa);
			}
			src 		= player_select_fg0_surface;
			dr.w = (src->w);
			dr.h = (src->h);
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (255-sp_scale256));
			SDL_BlitSurface(src, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
		/* ��Փx�^�O��`�� */
		{
			SDL_Rect ds;
			ds.x = (0);
			ds.y = (/*jj*/((cg_game_difficulty))<<6);
			ds.w = (SOZAI_HABA_184);	/* �f�ޕ�191[�h�b�g] */
			ds.h = (SOZAI_TATE_064);	/* �f�ޕ� 63[�h�b�g] */
		//
			SDL_Rect dr;
			SDL_Surface *src;
			src 		= difficulty_select_fg0_surface;
			dr.w = (src->w);
			dr.h = (63/*src->h*/);
			{
				dr.x = (16);//(16/*128*/);
				dr.y = (16+(128)-8);//(16)+(/*jj*/(2)<<6);
			}
		//	if ((jj==difficulty))
		//	{
		//		dr.x -= (8);
		//		dr.y -= (8);
		//	}
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, ((/*(jj==difficulty)?(255):*/(127))));
			SDL_BlitSurface(src, &ds, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
	}
	else
	if ((PLAYER_SELECT_05_SELECT)==my_ppp_loop)/* [�I��] */
	{
		cg.msg_time = byou60(5);/* �K�v */	/* �� 5 �b */
		if (0==cg_my_pad_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
		{
			if (cg_my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* �����E���V���b�g���L�����Z���̂��Âꂩ */
			{	/* ��Ԃ��ς��ꍇ��*/
				/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
				unloadbmp_by_surface(player_select_bg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_bg0_surface = NULL;
				unloadbmp_by_surface(player_select_fg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_fg0_surface = NULL;
				unloadbmp_by_surface(player_select_bg2_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_bg2_surface = NULL;
				unloadbmp_by_surface(player_select_fg2_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bplayer_select_fg2_surface = NULL;
			//
				if (cg_my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*�����E�̗���*/  ) /* ���E�{�^������ */
				{
					is_turn_right = (cg_my_pad & PSP_KEY_RIGHT)?1:0;/* �E�Ȃ�1, ���Ȃ�0 */
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
					my_ppp_loop = (PLAYER_SELECT_03_LOAD);	/* [load] */
				}
				if (cg_my_pad & PSP_KEY_SHOT_OK)
				{
					cg.msg_time = (0);/* �K�v */
					{
						/* ��Ԃ��ς��ꍇ�A������ SDL�T�[�t�F�C�X���J�� ���Ȃ��ƃ������[���[�N����� */
						unloadbmp_by_surface(difficulty_select_fg0_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bdifficulty_select_fg0_surface = NULL;
					}
					voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* �e�L�g�[ */
					main_call_func = stage_first_init;
				}
				else
				if (cg_my_pad & PSP_KEY_BOMB_CANCEL)
				{
					cg.msg_time = (0);/* �K�v */
					voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
					/* ��Փx�I�����j���[�ɖ߂� */
					main_call_func = difficulty_select_menu_start;	/* ��Փx�I�����j���[�� */
				}
			}
		}
	}
}


/*---------------------------------------------------------
	�L�����Z������炵�āA�^�C�g�����j���[�ֈړ��B
	���ʃT�u�B(�O���̂�����������Ă΂��)
---------------------------------------------------------*/

global void menu_cancel_and_voice(void)
{
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
	main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
}

/*---------------------------------------------------------
	��Փx�I�����j���[(���[�h��I�����Ă�)
	��Փx��I�����Ă�
---------------------------------------------------------*/

static void draw_difficulty_spec_message(void)
{
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	static const char *const_difficulty_spec_str[(4)] =
	{
	//	"Easy\\n"		"�ȒP�Ɍ����΁A�{���Q�ł��B\\n" 		"�@�@�@�@�@�@�@�@�@(�����ƑS�U��)",	// No. 0 easy
	//	"Normal\\n" 	"���ʂɌ����΁A����ł��B\\n" 		"�@�@�@�@�@�@�@�@�@�@�@�@�@(�S�U��)",	// No. 1 normal
	//	"Hard\\n"		"���d�������΁A�������ł��B\\n" 		"�@�@�@�@�@�@�@�@�@�@�@�@�@(�S�U��)",	// No. 2 hard
	//	"Lunatic\\n"	"�����݂Ɍ����΁A�������ł��B\\n"		"�@�@�@�@�@�@�@�@�@�@�@�@�@(�S�U��)",	// No. 3 lunatic
		"Easy\\n"		"�ȒP�ɂ͍U���ł��Ȃ��B\\n" 			"�@�@�@�@�@�@�@�@�@(�����悻�S�U��)",	// No. 0 easy
		"Normal\\n" 	"���ʂ̐l�ɂ͂����߂ł��Ȃ��B\\n"		"�@�@�@�@�@�@�@�@�@�@�@�@�@(�S�U��)",	// No. 1 normal
		"Hard\\n"		"�d���B\\n" 							"�@�@�@�@�@�@�@�@�@�@�@�@�@(�S�U��)",	// No. 2 hard
		"Lunatic\\n"	"�����݂Ƃ͌���Ȃ��B\\n"				"�@�@�@�@�@�@�@�@�@�@�@�@�@(�S�U��)",	// No. 3 lunatic
	};
	print_kanji000((char *)const_difficulty_spec_str[((cg_game_difficulty))], /*int color_type*/7, /*int wait*/0);
}

static void difficulty_select_menu_local_work(void)
{
	static int sp_scale256;
	if ((RANK_SELECT_00_LOAD)==my_ppp_loop) 	/* [load] */
	{
		(cg_game_difficulty) &= 0x03;
//
		sp_scale256 	= (255);
		sp_diff256		= (0);
//
		draw_difficulty_spec_message();
		my_ppp_loop++;
	}
	else
	if ((RANK_SELECT_01_MOVE_DRAW)==my_ppp_loop)/* [�`��1] */
	{
		sp_scale256 -= 15/*18*/;
		if (sp_scale256 < 0 )
		{
			sp_scale256 = (0);
			my_ppp_loop++;/* [�I��] */
		}
//		SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256)/*bg_alpha_bbb*/);
//		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		psp_pop_screen();
		int jj;
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
			if ((jj==(cg_game_difficulty)))
			{
				dr.x -= (8);
				dr.y -= (8);
			}
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (((jj==(cg_game_difficulty))?(255):(127))));
			SDL_BlitSurface(src, &ds, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
	}
	else
	if ((RANK_SELECT_02_SELECT)==my_ppp_loop)/* [�I��] */
	{
		cg.msg_time = byou60(5);/* �K�v */	/* �� 5 �b */
		if (0==cg_my_pad_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
		{
			if (cg_my_pad & (PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* �����E���V���b�g���L�����Z���̂��Âꂩ */
			{	/* ��Ԃ��ς��ꍇ��*/
			//
				if (cg_my_pad & (PSP_KEY_UP|PSP_KEY_DOWN)/*�ォ���̗���*/ ) /* �㉺�{�^������ */
				{
					if (cg_my_pad & PSP_KEY_UP)
							{	(cg_game_difficulty)--;	}
					else	{	(cg_game_difficulty)++;	}
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
					my_ppp_loop = (RANK_SELECT_00_LOAD);	/* [load] */
				}
				if (cg_my_pad & PSP_KEY_SHOT_OK)
				{
					cg.msg_time = (0);/* �K�v */
					voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* �e�L�g�[ */
					/* �ʏ�^�v���N�e�B�X�A�Q�[���J�n */
					// static void player_opt_init(void)�z���B�Ȃ�
					{
						is_turn_right = 0;/* �K�v */
						(cg_game_select_player)++;/* �v���O����player_select_work()�̓s���㒲�� */	/*player_select_work()::[init]*/
					}
					my_ppp_loop++;/* = (PLAYER_SELECT_03_LOAD) */
					main_call_func = player_select_work;
				}
				else
				if (cg_my_pad & PSP_KEY_BOMB_CANCEL)
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
	}
}

global void difficulty_select_menu_start(void)
{
//	psp_clear_screen();
	/* [init] */
	if (0==cg_my_pad_alter)/* ����������������ĂȂ������ꍇ */
	{
		psp_push_screen();
		/* [load] */
		{
			difficulty_select_fg0_surface = load_chache_bmp( (char *)"teki/teki00_256.png");//, 0, 0/*1*/);
			SDL_SetColorKey(difficulty_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
		}
		my_ppp_loop = (RANK_SELECT_00_LOAD);
		main_call_func = difficulty_select_menu_local_work;
	}
}
