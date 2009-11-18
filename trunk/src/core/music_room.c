
#include "game_main.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	�~���[�W�b�N���[��
---------------------------------------------------------*/

enum
{
	MUSIC_ROOM_STATE_00_INIT = 0,
	MUSIC_ROOM_STATE_01_FADE_IN,
	MUSIC_ROOM_STATE_02_SELECT_MENU,
	MUSIC_ROOM_STATE_03_FADE_OUT,
};

enum
{
	MOJI_00 = 0,MOJI_01,	MOJI_02,	MOJI_03,
	MOJI_04,	MOJI_05,	MOJI_06,	MOJI_07,
	MOJI_08,	MOJI_09,	MOJI_10,	MOJI_11,
	MOJI_12,	MOJI_13,	MOJI_14,	MOJI_15,
	MOJI_16,	MOJI_17,//	MOJI_18,	MOJI_19,
	MOJI_17_MAX 	/* �ő吔 */
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
#define Y_LOCATE_OFFSET 	(100)
#define MAX_KEY_NAMES_20	(MOJI_17_MAX+MUSIC_ROOM_03_MAX)
static void music_room_draw_message(int cursor1, int cursor2)
{
	script_message_window_clear();
	static const char *const_kaisetu_str[(2)][(1+17)] =
	{
		{	/*"BGM ���"*/
"�Ȃ��~�߂܂�",
"No.1 �t�̖���\\n"							"1�ʂ̃e�[�}�ł��B",
"No.2 �t�H�[���I�u�t�H�[���@�` �H�߂���\\n" "2�ʂ̃e�[�}�ł��B",
"No.3 ���H���[�W��1969\\n"					"3�ʂ̃e�[�}�ł��B",
"No.4 �L�L�ˉ����� �` Till When?\\n"		"4�ʂ̃e�[�}�ł��B\\n"						"�{�X�Ȃł͂Ȃ��̂ł��B",
"No.5 �����L�̋��@�` Deep Mountain\\n"		"5�ʂ̃e�[�}�ł��B",
"No.6 �S�������ׂ̈̃Z�v�e�b�g\\n"			"6�ʂ̃e�[�}�ł��B" 						"�{�X�Ȃł͂Ȃ��̂ŁA\\n�����Ԃ�܂����肵�Ă܂��B",
"No.7 ��\\n"								"�X�^�b�t���[���̃e�[�}�ł��B",
"No.8 ���쌶�z����\\n"						"�G�L�X�g���X�e�[�W�̃e�[�}�ł��B\\n"		"2�N�O�A�����l�^�̋U���[����",
"No.9 �g���R���i��\\n"					"�^�C�g����ʂ̃e�[�}�ł��B",
"No.10 �l�`�ٔ��@�` �l�̌`�M�т�����\\n"	"�A���X�̃e�[�}�ł��B"						"�n���h�x�������Ă݂܂����B�l�`����Â����Ă�H",
"No.11 ���_�����@�` Full Version\\n"		"�˖��� ���̃e�[�}�ł��B\\n"				"1���[�v5���ȏ゠��̂ŁA�����ł��B",
"No.12 �܂�����Ă܂���\\n" 				"���Ă�����3�{�X�N�ɂ��悤���B\\n"			"�������ɂ������C�����܂��B",
"No.13 �|����ā@�` Lunatic Princess\\n"	"�H���R �P��̃e�[�}�ł��B\\n"				"���l�ɂȂ�킩�邩���B",
"No.14 ���N�g�K�[���@�` ��������\\n"		"�p�`�����[�E�m�[���b�W�̃e�[�}�ł��B\\n"	"���̋Ȃ̌��l�^�ɂ������ɁB",
"No.15 �����v�@�` ���i�E�_�C�A��\\n"		"�\\�Z�� ���̃e�[�}�ł��B\\n" 			"��Ńv���O�����ɂ��Ă݂܂����B",
"No.16 �v���C���[�Y�X�R�A\\n"				"�G���f�B���O�̃e�[�}�ł��B"				"���g�n�w",
"No.17 ���Ă�Η���\\n" 					"�`���m�̃e�[�}�ł��B\\n"					"���̋Ȃ͉��̂Ȃ�ł���ˁA�ꉞ�B",
		},
		{	/*"SOUND ���"*/
"�V���b�g", 		"���j���[����", 	"���j���[�I��", 	"�{�X�ޏ�",
"�s�`���[��",		"�A�C�e���擾", 	"������",			"�����X�y�J",
"�U�R�|����",		"�O���C�Y", 		"�{�X�V���b�g1",	"�L��������",
"�{�X�V���b�g2",	"�{�X�V���b�g3",	"�{�X�V���b�g4",	"���Ԑ���",
"16��",
"17��",
		},
	};
	print_kanji000((char *)const_kaisetu_str[cursor1][cursor2], /*int color_type*/7, /*int wait*/0);
}

/*static*/ void music_room_work(void)
{
	static int music_room_setting[2+2/*MUSIC_ROOM_03_MAX*/];
	static SDL_Surface *music_room_name_surface[MAX_KEY_NAMES_20];	// �����̉摜
	static int menu_cursor1;
	static int bg_alpha_aaa;
//
	switch (psp_loop)
	{
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_00_INIT):
		play_music_num(BGM_02_stage2);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/music_room.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}
		music_room_setting[0] = 0;
		music_room_setting[1] = 0;
		music_room_setting[2] = (USE_MUSIC_FILES/*MOJI_17_MAX*/-1);
		music_room_setting[3] = (VOICE16_MAX_FILES/*MOJI_17_MAX*/-1);
		{
			static const char *const_menu_str[(MAX_KEY_NAMES_20)] =
			{
				// [[ ���� ]]
				" 0",	" 1",	" 2",	" 3",
				" 4",	" 5",	" 6",	" 7",
				" 8",	" 9",	"10",	"11",
				"12",	"13",	"14",	"15",
				"16",	"17",// "18",	"19",
				// [[ ���j���[ ]]
				"BGM",
				"SOUND",
				"QUIT"
			};
			{
				int i;
				for (i=0; i<(MAX_KEY_NAMES_20); i++)
				{
					music_room_name_surface[i] = font_render((char *)const_menu_str[i],FONT03/*FO NT06*/);
				}
			}
		}
		menu_cursor1		= MUSIC_ROOM_02_QUIT/*0*/;
		bg_alpha_aaa		= 0;
		script_message_window_clear();
		msg_time = (60*5);
		print_kanji000("���z���َ�\\n"
			"���R�s�Ƃ����̔\\�͓I�ɖ����ł��̂ŁA\\nBGM�͑S�ȏ���ɃA�����W�łł��B"
			/*"���Ȃ̖ʉe���Ȃ��قǕς��������Ă�̂�����܂��B"*/, /*int color_type*/7, /*int wait*/0);
		psp_loop++;//psp_loop = (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_01_FADE_IN);
		//SDL_SetAlpha(key_haikei_surface, SDL_SRCALPHA, 0);
		//keyboa rd_clear();
		break;
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_01_FADE_IN):
		bg_alpha_aaa += /*1*/2/*6*/;/**fps_fa ctor*/
		if ((/*250-6*/200/*224*/) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			psp_push_screen();
			psp_loop++;//psp_loop = (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_02_SELECT_MENU);
		}
		SDL_SetAlpha(/*key_haikei_surface*/sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		break;
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_02_SELECT_MENU):
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		msg_time = (60*5);
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_UP)				// ��{�^������
			{
				if (MUSIC_ROOM_00_BGM == menu_cursor1)		{	menu_cursor1 = (MUSIC_ROOM_03_MAX-1);}
				else										{	menu_cursor1--;}
			}
			else
			if (my_pad_alter & PSP_KEY_DOWN)			// ���{�^������
			{
				if ((MUSIC_ROOM_03_MAX-1) == menu_cursor1)	{	menu_cursor1 = MUSIC_ROOM_00_BGM;}
				else										{	menu_cursor1++;}
			}
			else	/* �㉺�ȊO�̒ʏ폈��(�΂ߓ��͂�r������ׁA�㉺�̓��͂��������ꍇ�͂���ȊO�̏��������Ȃ�) */
			{
				if (MUSIC_ROOM_02_QUIT == menu_cursor1) // ����[ QUIT ] ��I��ł���ꍇ
				{
					if (my_pad_alter & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL))	// �V���b�g || �L�����Z���{�^������
					{
						#if (0==USE_DESIGN_TRACK)
						play_voice_auto_track(VOICE04_SHIP_HAKAI);
					//	play_voice_auto_track(VOICE02_MENU_SELECT);
						#else
						voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
					//	voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*�e�L�g�[*/
						#endif
						#if 0
						{
							int i;
							for (i=0; i<(MAX_KEY_NAMES_20); i++)
							{
								/* �����Ńn���O�A�b�v */
								SDL_FreeSurface(music_room_name_surface[i]);
							}
						}
						#endif
						play_music_num(BGM_09_menu1);
						{
							//key_haikei_surface	= loadbmp("bg/title_bg.jpg");
							SDL_Surface *loadpic	= loadbmp0("bg/title_bg.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
							SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
							unloadbmp_by_surface(loadpic);
							//unloadbmp_by_surface(key_haikei_surface);
						}
						msg_time = (0);
//						bg_alpha_aaa		= 255;
						bg_alpha_aaa		= 0;
						psp_loop++;//	psp_loop = (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_03_FADE_OUT);
					//	psp_loop++;//	psp_loop = (ST_WORK_MUSIC_ROOM|OPTION_MENU_STATE_06_FADE_INIT);
					}
				}
				else	/* �ݒ荀�ڂ�ύX����ꍇ */
				if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		// ���E�{�^������
				{
					if (my_pad_alter & PSP_KEY_LEFT)			// ���{�^������
					{
						music_room_setting[menu_cursor1]--;
						if ((0/*MOJI_00*/) > music_room_setting[menu_cursor1]) {	music_room_setting[menu_cursor1] = music_room_setting[menu_cursor1+2]; }
						music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
					}
					else
					if (my_pad_alter & PSP_KEY_RIGHT)			// �E�{�^������
					{
						music_room_setting[menu_cursor1]++;
						if (music_room_setting[menu_cursor1+2] < music_room_setting[menu_cursor1])	{	music_room_setting[menu_cursor1] = 0/*MOJI_00*/; }
						music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
					}
				}
				else	/* �ݒ荀�ڂ�ύX����ꍇ */
				if (my_pad_alter & PSP_KEY_SHOT_OK) // �V���b�g�{�^������
				{
					if (MUSIC_ROOM_00_BGM == menu_cursor1)	// ����[ BGM ] ��I��ł���ꍇ
					{
						play_music_num(music_room_setting[menu_cursor1]/*BGM_05_stage5*/);
					}
					else
//					if (MUSIC_ROOM_01_SOUND == menu_cursor1)	// ����[ SOUND ] ��I��ł���ꍇ
					{
						#if (0==USE_DESIGN_TRACK)
						play_voice_auto_track(music_room_setting[menu_cursor1]/*VOICE07_BOMB*/);
						#else
						voice_play(music_room_setting[menu_cursor1]/*VOICE07_BOMB*/, TRACK01_EXPLODE);/*�e�L�g�[*/
						#endif
					}
				}
				else	/* �ݒ荀�ڂ�ύX����ꍇ */
				if (my_pad_alter & PSP_KEY_BOMB_CANCEL) // �L�����Z���{�^������
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
			kk=0;
			#define MAX_MENU_02 			(MUSIC_ROOM_02_QUIT)
			for (i=0; i</*12*/(MAX_MENU_02/*MUSIC_ROOM_03_MAX*/)/*MUSIC_ROOM_03_MAX*/; i++)
			{
				/* �{�^���ݒ�`�� */
				{
					int kinou_number;
					kinou_number = music_room_setting[i];
					if (menu_cursor1==i)	{	jj= 2;	}
					else					{	jj= 0;	}
					rect_locate_offset.x = X_LOCATE_OFFSET_04-(jj);
					rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/kk)-(jj);
					SDL_SetAlpha(	music_room_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
					SDL_BlitSurface(music_room_name_surface[kinou_number], NULL,sdl_screen[SDL_00_SCREEN],&rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],&rect_src_offset,sdl_screen[SDL_00_SCREEN],&rect_locate_offset);
				}
				kk += 20;
			}
			/* �{�^�����̕`�� */
			kk=0;
			for (i=0; i<(MUSIC_ROOM_03_MAX)/*MUSIC_ROOM_03_MAX*/; i++)
			{
				if (menu_cursor1==i)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
				rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/kk)-(jj);
				SDL_SetAlpha(	music_room_name_surface[i+(MOJI_17_MAX)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(music_room_name_surface[i+(MOJI_17_MAX)], NULL,sdl_screen[SDL_00_SCREEN],&rect_locate_offset);
				kk += 20;
			}
		}
		break;
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_03_FADE_OUT):
		bg_alpha_aaa += /*1*/2/*8*/;/**fps_fa ctor*/
		if ((/*250-8*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			/* ���C�����j���[�ɖ߂� */
			psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	//newsta te(ST_MENU/*ST_INTRO*/,0,1);
		}
		SDL_SetAlpha(/*key_haikei_surface*/sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		break;
	}
}
