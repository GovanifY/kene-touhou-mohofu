
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�~���[�W�b�N���[�� ���z���َ�
---------------------------------------------------------*/

#include "kanji_system.h"

enum
{
	MOJI_00 = 0,MOJI_01,	MOJI_02,	MOJI_03,
	MOJI_04,	MOJI_05,	MOJI_06,	MOJI_07,
	MOJI_08,	MOJI_09,	MOJI_10,	MOJI_11,
	MOJI_12,	MOJI_13,	MOJI_14,	MOJI_15,
	MOJI_16,	MOJI_17,	MOJI_18,	MOJI_19,
	MOJI_20,	MOJI_21,	MOJI_22,	MOJI_23,
	MOJI_24,	MOJI_25,
	MOJI_23_MAX 	/* �ő吔 */
};

enum
{
	MUSIC_ROOM_00_BGM = 0,
	MUSIC_ROOM_01_SOUND,
	MUSIC_ROOM_02_QUIT,
	MUSIC_ROOM_03_MAX,
};

#define X_LOCATE_OFFSET_02	(64)
#define X_LOCATE_OFFSET_04	(224)
#define Y_LOCATE_OFFSET 	(128)/*(100)*/
#define MAX_KEY_NAMES_21	(MOJI_23_MAX+MUSIC_ROOM_03_MAX)
/*
���ߕ���(�V�t�gJIS��2�o�C�g�ڂ�0x5c�A���Ȃ킿'\'('�_')�ł��镶���̂��ƁB)
	�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
*/
static void music_room_draw_message(int cursor1, int cursor2)
{
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	static const char *const_kaisetu_str[(2)][(MOJI_23_MAX)] =
	{
		{	/*"BGM ���"*/
"�Ȃ��~�߂܂�", 								/* */
"No.1 �t�̖���\\n"								"1�ʂ̃e�[�}�ł��B",
"No.2 �o�b�h�E�A�b�v�� �` Bad Apple!!\\n"		"2�ʂ̃e�[�}�ł��B",
"No.3 ���H���[�W��1969\\n"						"3�ʂ̃e�[�}�ł��B",
"No.4 �t�H�[���I�u�t�H�[�� �` �H�߂���\\n"		"4�ʂ̃e�[�}�ł��B",
"No.5 ���A�����@�}����\\n"						"5�ʂ̃e�[�}�ł��B",
"No.6 ���C�h�ƌ��̉������v\\n"					"6�ʂ̃e�[�}�ł��B",
"No.7 ���� �` Innocent Treasures\\n"			"�X�^�b�t���[���̃e�[�}�ł��B", 			// ����Ȋw���I�u���� Innocent Treasures�v
"No.8 ���[�l�C�g�G���t\\n"						"�G�L�X�g���X�e�[�W�̃e�[�}�ł��B",
"No.9 ���쌶�z����\\n"							"�t�@���^�Y���X�e�[�W�̃e�[�}�ł��B",
"No.10 ��C�g����  �` Chinese Tea\\n"			"�G���f�B���O2�̃e�[�}�ł��B",
"No.11 �l�`�ٔ� �` �l�̌`�M�т�����\\n" 		"�A���X�̃e�[�}�ł��B",
"No.12 �R���v���[�g�E�_�[�N�l�X\\n" 			"�����̃e�[�}�ł��B",						// Complete Darkness
"No.13 �|����� �` Lunatic Princess\\n" 		"�H���R �P��̃e�[�}�ł��B",
"No.14 ���_����\\n" 							"�˖��� ���̃e�[�}�ł��B",
"No.15 ���N�g�K�[�� �` ��������\\n" 			"�p�`�����[�E�m�[���b�W�̃e�[�}�ł��B",
"No.16 �����v �` ���i�E�_�C�A��\\n" 			"�\\�Z�� ���̃e�[�}�ł��B",
"No.17 ���Ă�Η���\\n" 						"�`���m�̃e�[�}�ł��B",
"No.18 �O�����A �I�u �A���X\\n" 				"�A���X�̃e�[�}�ł��B", 					// Grimoire of Alice
"No.19 ���F�}�X�^�[�X�p�[�N\\n" 				"�������̃e�[�}�ł��B",
"No.20 �L�L�ˉ����� �` Till When?\\n"			"�d���̃e�[�}�ł��B",
"No.21 �S�������ׂ̈̃Z�v�e�b�g\\n" 			"���~���A�E�X�J�[���b�g�̃e�[�}�ł��B",
"No.22 U.N.�I�[�G���͔ޏ��Ȃ̂��H\\n"			"�t�����h�[���E�X�J�[���b�g�̃e�[�}�ł��B",
"No.23 �v���C���[�Y�X�R�A\\n"					"�G���f�B���O�̃e�[�}�ł��B",
"No.24 �����L�̋� �` Deep Mountain\\n"			"�L�[�R���t�B�O�̃e�[�}�ł��B",
"No.25 �g���R���i��\\n"						"�^�C�g����ʂ̃e�[�}�ł��B",
		},
		{	/*"SOUND ���"*/
"�V���b�g", 		"���j���[����", 	"���j���[�I��", 	"�{�X�ޏ�",
"�s�`���[��",		"�A�C�e���擾", 	"������",			"�����X�y�J",
"�U�R�|����",		"�O���C�Y", 		"���Ԑ���", 		"�L��������",
"�{�X�V���b�g1",	"�{�X�V���b�g2",	"�{�X�V���b�g3",	"�{�X�V���b�g4",
"�{�X�V���b�g5",	"�{�X����1",		"18��", 			"19��",
"20��", 			"21��", 			"22��", 			"23��",
"24��", 			"25��",
		},
	};
	print_kanji000((char *)const_kaisetu_str[cursor1][cursor2], /*int color_type*/(7), /*int wait*/(0) );
}
#if 0
global void music_game_draw_message(int cursor1)
{
	static const char *const_music_title_str[(MOJI_23_MAX)] =
	{	/*	�ő唼�p��31�����B */
	/*	"feeddccbbaa99887766554433221100",'\0' ���[�N������o�b�t�@�������ꂾ�������p�ӂ��Ȃ��\��Ȃ̂ŁA���ӂꂽ�玚���o�Ȃ��B */
		" �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@", // No. 0 "�Ȃ��~�߂܂�"
		" �@�@�@�@�@�@�@�@�@�@��t�̖���", // No. 1 "1�ʂ̃e�[�}�ł��B"
		"��i��̕� Imperishable Night", // No. 2 "2�ʂ̃e�[�}�ł��B" 				// ���}�`���͎~�߂ē���ւ���Ǝv���B
		" �@�@�@�@�@�@�􃔃H���[�W��1969", // No. 3 "3�ʂ̃e�[�}�ł��B"
		" �@ ��Fall of fall�@�` �H�߂���", // No. 4 "4�ʂ̃e�[�}�ł��B"
		" �@�@�@�@�@�@�􃔃A�����@�}����", // No. 5 "5�ʂ̃e�[�}�ł��B"
		" �@�@���S�������ׂ̈̃Z�v�e�b�g", // No. 6 "6�ʂ̃e�[�}�ł��B"
		" �@�@�@�@�@�@�@�@�@�@�@�@������", // No. 7 "�X�^�b�t���[���̃e�[�}�ł��B"
		" �@�@�@�@�@�@�􃋁[�l�C�g�G���t", // No. 8 "�G�L�X�g���X�e�[�W�̃e�[�}�ł��B"
		" �@�@�@�@�@�@�@�@�􉓖쌶�z����", // No. 9 "�t�@���^�Y���X�e�[�W�̃e�[�}�ł��B"
		" �@���C�g����  �` Chinese Tea", // No.10 "�G���f�B���O2�̃e�[�}�ł��B"
		"���l�`�ٔ��@�` �l�̌`�M�т�����", // No.11 "�A���X�̃e�[�}�ł��B"
		" �@�@�@�@�@ ��Complete Darkness", // No.12 "�����̃e�[�}�ł��B"
		"���|����ā@�` Lunatic Princess", // No.13 "�H���R �P��̃e�[�}�ł��B"
		" �@ �����_�����@�` Full Version", // No.14 "�˖��� ���̃e�[�}�ł��B"
		" �@ �􃉃N�g�K�[���@�` ��������", // No.15 "�p�`�����[�E�m�[���b�W�̃e�[�}�ł��B"
		" �@ �􌎎��v�@�` ���i�E�_�C�A��", // No.16 "�\\�Z�� ���̃e�[�}�ł��B"
		" �@�@�@�@�@�@�@�@�􂨂Ă�Η���", // No.17 "�`���m�̃e�[�}�ł��B"
		" �@�@�@�@�@ ��Grimoire of Alice", // No.18 "�A���X�̃e�[�}�ł��B"
		" �@�@�@�@�����F�}�X�^�[�X�p�[�N", // No.19 "�������̃e�[�}�ł��B"
		" �@��L�L�ˉ����� �` Till When?", // No.20 "�d���̃e�[�}�ł��B"				// �d���ǂ�����B
		" �@�@�@�@�@��v���C���[�Y�X�R�A", // No.20 "�G���f�B���O�̃e�[�}�ł��B"		// ���������ւ���Ǝv���B
		" �������L�̋��@�` Deep Mountain", // No.21 "�L�[�R���t�B�O�̃e�[�}�ł��B"
		" �@�@�@�@�@�@�@��g���R���i��", // No.22 "�^�C�g����ʂ̃e�[�}�ł��B"
	};
	print_kanji000((char *)const_music_title_str[cursor1], /*int color_type*/(7), /*int wait*/(0) );
}
#endif

static int music_room_setting[(2)+(2)];/*MUSIC_ROOM_03_MAX*/
static SDL_Surface *music_room_name_surface[MAX_KEY_NAMES_21];	/* �����̕\���p�摜 */
static int menu_cursor1;
static int bg_alpha_aaa;

/*---------------------------------------------------------
	���z���َ� �t�F�[�h�A�E�g���Ă����܂���
---------------------------------------------------------*/

static void music_room_state_03_fade_out(void)
{
	bg_alpha_aaa += (2);	/*(1) (8)*/ 	/*fps_factor*/
	if ((224) < bg_alpha_aaa)/*(250-8)*/
	{
		bg_alpha_aaa = (255);
		#if 1/* �����̕\���p�摜���J������ */
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_21); i++)
			{
				/* �����Ńn���O�A�b�v */
				SDL_FreeSurface(music_room_name_surface[i]);/*xxx_ok?*/
			}
		}
		#endif
		main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
	}
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------
	���z���َ� ���j���[�I�����āA�Đ�������A�`�悵���蒆
---------------------------------------------------------*/

static void music_room_state_02_select_menu(void)
{
	psp_pop_screen();
	cg.msg_time = byou60(5);	/* �� 5 �b */
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		if (psp_pad.pad_data & PSP_KEY_UP)			/* ��{�^������ */
		{
			if (MUSIC_ROOM_00_BGM == menu_cursor1)		{	menu_cursor1 = (MUSIC_ROOM_03_MAX-1);	}
			else										{	menu_cursor1--; 	}
		}
		else
		if (psp_pad.pad_data & PSP_KEY_DOWN)			/* ���{�^������ */
		{
			if ((MUSIC_ROOM_03_MAX-1) == menu_cursor1)	{	menu_cursor1 = MUSIC_ROOM_00_BGM;	}
			else										{	menu_cursor1++; 	}
		}
		else	/* �㉺�ȊO�̒ʏ폈��(�΂ߓ��͂�r������ׁA�㉺�̓��͂��������ꍇ�͂���ȊO�̏��������Ȃ�) */
		{
			if (MUSIC_ROOM_02_QUIT == menu_cursor1) /* ����[ QUIT ] ��I��ł���ꍇ */
			{
				if (psp_pad.pad_data & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL))	/* �V���b�g || �L�����Z���{�^������ */
				{
					#if (1)
					voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
				//	voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*�e�L�g�[*/
					#endif
					play_music_num(BGM_25_menu01);
					load_SDL_bg(BG_TYPE_00_title_bg);
					cg.msg_time = (0);	/* �� 0 �b */
//					bg_alpha_aaa		= 255;
					bg_alpha_aaa		= 0;
					main_call_func = music_room_state_03_fade_out;
				}
			}
			else	/* �ݒ荀�ڂ�ύX����ꍇ */
			if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		/* ���E�{�^������ */
			{
				if (psp_pad.pad_data & PSP_KEY_LEFT)			/* ���{�^������ */
				{
					music_room_setting[menu_cursor1]--;
					if ((0/*MOJI_00*/) > music_room_setting[menu_cursor1])	{	music_room_setting[menu_cursor1] = music_room_setting[menu_cursor1+(2)]; }
					music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
				}
				else
				if (psp_pad.pad_data & PSP_KEY_RIGHT)		/* �E�{�^������ */
				{
					music_room_setting[menu_cursor1]++;
					if (music_room_setting[menu_cursor1+(2)] < music_room_setting[menu_cursor1])	{	music_room_setting[menu_cursor1] = 0/*MOJI_00*/; }
					music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
				}
			}
			else	/* �ݒ荀�ڂ�ύX����ꍇ */
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK) /* �V���b�g�{�^������ */
			{
				if (MUSIC_ROOM_00_BGM == menu_cursor1)	/* ����[ BGM ] ��I��ł���ꍇ */
				{
					play_music_num(music_room_setting[menu_cursor1]/*BGM_05_stage5*/);
				}
				else
//				if (MUSIC_ROOM_01_SOUND == menu_cursor1)	/* ����[ SOUND ] ��I��ł���ꍇ */
				{
					#if (1)
					voice_play(music_room_setting[menu_cursor1]/*VOICE07_BOMB*/, TRACK01_EXPLODE);/*�e�L�g�[*/
					#endif
				}
			}
			else	/* �ݒ荀�ڂ�ύX����ꍇ */
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL) /* �L�����Z���{�^������ */
			{
				play_music_num(BGM_00_stop);
			}
		}
	}
	/* �`�� */
	{
	//	/*static*/ SDL_Rect rect_src_offset;		/* �ʒu */
		/*static*/ SDL_Rect rect_locate_offset; 	/* �\���ʒu */
		int i;
		int jj;
		int kk;
		/* �@�\�ԍ��̕`�� */
		kk = (0);
		#define MAX_MENU_02 			(MUSIC_ROOM_02_QUIT)
		for (i=0; i<(MAX_MENU_02); i++)/*MUSIC_ROOM_03_MAX*/ /*12*/
		{
			/* �{�^���ݒ�`�� */
			{
				int kinou_number;
				kinou_number = music_room_setting[i];
				if (menu_cursor1==i)	{	jj = 2; 	}
				else					{	jj = 0; 	}
				rect_locate_offset.x = X_LOCATE_OFFSET_04-(jj);
				rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/kk)-(jj);
				SDL_SetAlpha(	music_room_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(music_room_name_surface[kinou_number], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			//	rect_src_offset.x = 0;
			//	rect_src_offset.y = (kinou_number<<4);
			//	rect_src_offset.w = (16*9);
			//	rect_src_offset.h = (16);
			//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],&rect_src_offset,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
			kk += (20);
		}
		/* �{�^�����̕`�� */
		kk = (0);
		for (i=0; i<(MUSIC_ROOM_03_MAX); i++)/*MUSIC_ROOM_03_MAX*/
		{
			if (menu_cursor1==i)	{	jj = 2; 	}
			else					{	jj = 0; 	}
			rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
			rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/kk)-(jj);
			SDL_SetAlpha(	music_room_name_surface[i+(MOJI_23_MAX)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
			SDL_BlitSurface(music_room_name_surface[i+(MOJI_23_MAX)], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			kk += (20);
		}
	}
}


/*---------------------------------------------------------
	���z���َ� �t�F�[�h�C����
---------------------------------------------------------*/

static void music_room_state_01_fade_in(void)
{
	bg_alpha_aaa += (2);	/*fps_factor*/
	if ((200) < bg_alpha_aaa)/*(250-6) (224)*/
	{
		bg_alpha_aaa = (255);
		psp_push_screen();
		main_call_func = music_room_state_02_select_menu;
	}
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------
	���z���َ� �J�n����
---------------------------------------------------------*/

global void music_room_start(void)
{
	play_music_num(BGM_06_stage6);
	load_SDL_bg(BG_TYPE_03_music_room);
	music_room_setting[0] = (0);
	music_room_setting[1] = (0);
	music_room_setting[2] = (USE_26_MUSIC_FILES-1);/*MOJI_23_MAX*/
	music_room_setting[3] = (VOICE18_MAX_FILES-1);/*MOJI_23_MAX*/
	{
		static const char *const_menu_str[(MAX_KEY_NAMES_21)] =
		{
			/* [[ ���� ]] */
			" 0",	" 1",	" 2",	" 3",
			" 4",	" 5",	" 6",	" 7",
			" 8",	" 9",	"10",	"11",
			"12",	"13",	"14",	"15",
			"16",	"17",	"18",	"19",
			"20",	"21",	"22",	"23",
			"24",	"25",
			/* [[ ���j���[ ]] */
			"BGM",
			"SOUND",
			"QUIT"
		};
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_21); i++)
			{
				music_room_name_surface[i] = font_render( (char *)const_menu_str[i], FONT16R);
			}
		}
	}
	menu_cursor1		= MUSIC_ROOM_02_QUIT;
	bg_alpha_aaa		= (0);
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	cg.msg_time = byou60(5);	/* �� 5 �b */
	print_kanji000("�悤����\\n"//"���z���َ�\\n"
		"���R�s�Ƃ����̔\\�͓I�ɖ����ł��̂ŁA\\n"
		"BGM�͑S�ȏ���ɃA�����W�łł��B",
	//	"���Ȃ̖ʉe���Ȃ��قǕς��������Ă�̂�����܂��B",
		/*int color_type*/(7), /*int wait*/(0) );
	main_call_func = music_room_state_01_fade_in;
}
