
#include "game_main.h"

/*---------------------------------------------------------
	�L�[�R���t�B�O
---------------------------------------------------------*/

enum
{
	KEY_CONFIG_STATE_00_INIT = 0,
	KEY_CONFIG_STATE_01_FADE_IN,
	KEY_CONFIG_STATE_02_SELECT_LEFT_MENU,
	KEY_CONFIG_STATE_06_FADE_INIT,
	KEY_CONFIG_STATE_07_FADE_OUT,
};
//	[***20080110-20080115
//	[***20090110
//�L�[�R���t�B�O�ǉ��B
//��{�͐�Βl�w��(�@�\�Œ�l)��support.c��keypoll�̒���pad_config����
//�z��w��ԍ��ɕϐ��ipad_config[key_xxx�j�����
//�܂�keypoll�ɓ���l�́A0��1�����̂܂܂��ƃ{�^���ݒ���肪�o��̂Ńr�b�g�ŊǗ�����悤�ɂ���
/*static*/ //void key_config_init()
//{
//	//key_config_state= (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_00_INIT);
//	psp_loop = (ST_WORK_KEY_CONFIG|0);
//}

enum
{	//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
	KEY_CONFIG_MENU_00_SELECT = 0,
	KEY_CONFIG_MENU_01_START,
	KEY_CONFIG_MENU_02_UE,
	KEY_CONFIG_MENU_03_MIGI,
	KEY_CONFIG_MENU_04_SITA,
	KEY_CONFIG_MENU_05_HIDARI,
	KEY_CONFIG_MENU_06_L,
	KEY_CONFIG_MENU_07_R,
	KEY_CONFIG_MENU_08_SANKAKU,
	KEY_CONFIG_MENU_09_MARU,
	KEY_CONFIG_MENU_10_BATU,
	KEY_CONFIG_MENU_11_SIKAKU,
	//	�����ݒ� / �I��
	KEY_CONFIG_MENU_12_RESET,
	KEY_CONFIG_MENU_13_QUIT,
	KEY_CONFIG_MENU_MAX,
};

/*---------------------------------------------------------
	KINOU_07_SNAP_SHOT �� �Q�[��(������)�ɂ��܂�֌W�Ȃ��̂ŁA
	��ʓI�ɃN���e�B�J���ȓ���ł͎g���Â炢�Ǝv����k�{�^���Ɉړ��B
	�X�g���[�N���Z�����A���������₷���t���b�g�{�^����slow�����蓖�Ă�B
	(�k�A�q�A�{�^���̓t���b�g�{�^���ɔ�ׂăX�g���[�N���������A�����n�߂Ă���F���܂łɎ��Ԃ�������B)
	�܂��g�ы@�䂦�ɁA�k�A�q�A�{�^���͔����ɉ�ʂ��Ԃ��̂ŁA
	slow�؂�ւ������g����󋵂��ƃh�b�g�P�ʂ̐�������������B
---------------------------------------------------------*/

/*static*/ void set_default_key(int *key_setting_map, int key_setting_type)
{
	static const int default_key_map[4][KEY_NUM12_MAX] =
	{
		{	/* type 01 �͕핗 �W�� (�X�����̈ӌ��Ō��܂���) */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_OPTION, 	PSP_KEY_SHOT_OK,		PSP_KEY_BOMB_CANCEL,
		},
		{	/* type 02 toho DX �݊� */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_OPTION, 	PSP_KEY_BOMB_CANCEL,	PSP_KEY_SHOT_OK,
		},
		{	/* type 03 psp-1000 (���{�^�����J�X�Ȃ̂Ŏg���Ȃ��ꍇ) */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_SLOW,			PSP_KEY_SHOT_OK,	PSP_KEY_BOMB_CANCEL,	PSP_KEY_OPTION,
		},
		{	/* type 04 psp-2000 (���{�^�����V���b�g�̏ꍇ) */
			PSP_KEY_SELECT, 		PSP_KEY_PAUSE,		PSP_KEY_UP, 			PSP_KEY_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			PSP_KEY_DOWN,			PSP_KEY_LEFT,		PSP_KEY_SNAP_SHOT,		PSP_KEY_SLOW,
			PSP_KEY_BOMB_CANCEL,	PSP_KEY_OPTION, 	PSP_KEY_SLOW,			PSP_KEY_SHOT_OK,
		},
	};
	{
		int i;
		for (i=0; i<12; i++)
		{
			key_setting_map[i] = default_key_map[key_setting_type][i];
		}
	}
}

enum /*_keynum_*/		//�L�[�R���t�B�O�p
{
	KINOU_00_NONE = 0,
	KINOU_01_SELECT,
	KINOU_02_PAUSE,
	KINOU_03_UP,
	KINOU_04_RIGHT,
	KINOU_05_DOWN,
	KINOU_06_LEFT,
	KINOU_07_SNAP_SHOT,
	KINOU_08_SYSTEM,
	KINOU_09_SLOW,
	KINOU_10_OPTION,
	KINOU_11_SHOT,
	KINOU_12_BOMB,
	KINOU_13_MAX	/* �L�[�R���t�B�O�p�̍ő吔 */
};

extern void ini_save(void); 	// [***090115
/*static*/ void key_config_work(void)
{

#define MAX_KINOU_11			(12+1)
#define MAX_BUTTON_12			(12)
#define MAX_MENU_02 			(2)
#define MAX_DEFAULT_MAP_04		(4)

	static int key_setting[KEY_CONFIG_MENU_MAX]; 						// ����=�{�^���ϐ� ���т� "bg/key_haikei_surface.png"
	static const int const_pad_setting[KEY_CONFIG_MENU_MAX/*11*/] =		// ����=�@�\�萔 ���т� "fonts/keylist.png"
	{	// ���� = �@�\�萔 ���сB	[[ �I��������я��� ]]
		PSP_KEY_NONE,			//	KINOU_00_NONE,
		PSP_KEY_SELECT, 		//	KINOU_01_SELECT,
		PSP_KEY_PAUSE,			//	KINOU_02_PAUSE,
		PSP_KEY_UP, 			//	KINOU_03_UP,
		PSP_KEY_RIGHT,			//	KINOU_04_RIGHT,
		PSP_KEY_DOWN,			//	KINOU_05_DOWN,
		PSP_KEY_LEFT,			//	KINOU_06_LEFT,
		PSP_KEY_SYSTEM, 		//	KINOU_07_SNAP_SHOT,
		PSP_KEY_SLOW,			//	KINOU_08_SYSTEM,
		PSP_KEY_SNAP_SHOT,		//	KINOU_09_SLOW,
		PSP_KEY_OPTION, 		//	KINOU_10_OPTION,
		PSP_KEY_SHOT_OK,		//	KINOU_11_SHOT,
		PSP_KEY_BOMB_CANCEL,	//	KINOU_12_BOMB,
		0						//	0
	};



	#define X_LOCATE_OFFSET_01	(64)/*(80-16)*/
	#define X_LOCATE_OFFSET_02	(80)
	#define X_LOCATE_OFFSET_03	(200)
	#define X_LOCATE_OFFSET_04	(250)
	#define Y_LOCATE_OFFSET 	(40)
	#define MAX_KEY_NAMES_12	(KEY_CONFIG_MENU_MAX+MAX_BUTTON_12+MAX_MENU_02+MAX_DEFAULT_MAP_04)
	static SDL_Surface *key_name_surface[MAX_KEY_NAMES_12]; 	// �����̉摜


	static int menu_cursor1;
	static int key_setting_default_type;
//
	static int bg_alpha_aaa;
//	int i;
	switch (psp_loop/*key_config_state*/)
	{
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_00_INIT):
		play_music_num(BGM_20_menu02);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/key_config.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}

		#if 0
		//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
		key_setting[ 0] = pad_config[KEY_NUM00_SELECT];
		key_setting[ 1] = pad_config[KEY_NUM01_START];
		key_setting[ 2] = pad_config[KEY_NUM02_UP ];
		key_setting[ 3] = pad_config[KEY_NUM03_RIGHT ];
		key_setting[ 4] = pad_config[KEY_NUM04_DOWN ];
		key_setting[ 5] = pad_config[KEY_NUM05_LEFT ];
		key_setting[ 6] = pad_config[KEY_NUM06_L_TRIG];
		key_setting[ 7] = pad_config[KEY_NUM07_R_TRIG];
		key_setting[ 8] = pad_config[KEY_NUM08_TRIANGLE];
		key_setting[ 9] = pad_config[KEY_NUM09_CIRCLE];
		key_setting[10] = pad_config[KEY_NUM10_CROSS];
		key_setting[11] = pad_config[KEY_NUM11_SQUARE];
		#else
		{
			int i;
			for (i=0; i<KEY_NUM12_MAX; i++)
			{
				key_setting[i] = pad_config[i];
			}
		}
		#endif
		{
			static const char *const_key_str[(MAX_KEY_NAMES_12)] =
			{
				//	[[ �I��������я��� ]]
				"NONE",
				#if 1
				"SELECT",
				"PAUSE",
				"UP",
				"RIGHT",
				"DOWN",
				"LEFT",
				"SYSTEM",		//	"SNAP SHOT",
				"SLOW", 		//	"SYSTEM",
				"SNAP SHOT",	//	"SLOW",
				"OPTION",
				"SHOT / OK",
				"BOMB / CANCEL",
				#endif
				//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
				"SELECT",
				"START",
				"UP",
				"RIGHT",
				"DOWN",
				"LEFT",
				"L",
				"R",
				"SANKAKU",
				"MARU",
				"BATU",
				"SIKAKU",
				//	�����ݒ� / �I��
				"RESET",
				"QUIT",
				//	�����ݒ�^�C�v�̑I��
				"TYPE 01",
				"TYPE 02",
				"TYPE 03",
				"TYPE 04",
				NULL
			};
			{
				int i;
				i = 0;
				while (NULL != const_key_str[i])
				{
					key_name_surface[i] = font_render((char *)const_key_str[i],FONT03/*FO NT06*/);
					i++;
				}
			}
		}
		menu_cursor1				= KEY_CONFIG_MENU_13_QUIT/*0*/;
		key_setting_default_type	= 0;
		//SDL_SetAlpha(key_haikei_surface, SDL_SRCALPHA, 0);
		//keyboa rd_clear();

		bg_alpha_aaa		= 0;
		psp_loop++;//key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_01_FADE_IN);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_01_FADE_IN):
		bg_alpha_aaa += /*1*/2/*6*/;/*fps_factor*/
		if ((/*250-6*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			psp_push_screen();
			psp_loop++;//key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_02_SELECT_LEFT_MENU);
		}
		SDL_SetAlpha(/*key_haikei_surface*/sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_02_SELECT_LEFT_MENU):
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_UP)				// ��{�^������
			{
				if (KEY_CONFIG_MENU_00_SELECT == menu_cursor1)	{	menu_cursor1 = (KEY_CONFIG_MENU_MAX-1);}
				else											{	menu_cursor1--;}
			}
			else
			if (my_pad_alter & PSP_KEY_DOWN)			// ���{�^������
			{
				if ((KEY_CONFIG_MENU_MAX-1) == menu_cursor1)	{	menu_cursor1 = KEY_CONFIG_MENU_00_SELECT;}
				else											{	menu_cursor1++;}
			}
			else	/* �㉺�ȊO�̒ʏ폈��(�΂ߓ��͂�r������ׁA�㉺�̓��͂��������ꍇ�͂���ȊO�̏��������Ȃ�) */
			{
				if (KEY_CONFIG_MENU_12_RESET == menu_cursor1)			// ����[ RESET ] ��I��ł���ꍇ
				{
					if (my_pad_alter & PSP_KEY_LEFT)	// ���{�^������
					{
						key_setting_default_type--;
						key_setting_default_type &= (4-1);
						voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*�e�L�g�[*/
					}
					else
					if (my_pad_alter & PSP_KEY_RIGHT)	// �E�{�^������
					{
						key_setting_default_type++;
						key_setting_default_type &= (4-1);
						voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*�e�L�g�[*/
					}
					else
					if (my_pad_alter & PSP_KEY_SHOT_OK) // �V���b�g�{�^������
					{
						voice_play(VOICE07_BOMB, TRACK01_EXPLODE);/*�e�L�g�[*/
						set_default_key(key_setting, key_setting_default_type);
						menu_cursor1 = KEY_CONFIG_MENU_13_QUIT;
					}
				}
				else
				if (KEY_CONFIG_MENU_13_QUIT == menu_cursor1)	// ����[ QUIT ] ��I��ł���ꍇ
				{
					if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	// �L�����Z���{�^������
					{
						voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
						psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT);
					}
					else
					if (my_pad_alter & PSP_KEY_SHOT_OK) 	// �V���b�g�{�^������
					{
						int saiteigen_exsist;	// ���݊m�F�p�B0�ő��݂��Ȃ��B���݂��Ȃ��ƃ��j���[�𔲂���Ȃ��B
						saiteigen_exsist = 0;
						{
							int i;
							for (i=0; i<KEY_NUM12_MAX/*KEY_CONFIG_MENU_MAX*/; i++)
							{
								saiteigen_exsist	|= key_setting[i];
							}
						}
						if (0!=(saiteigen_exsist&(PSP_KEY_SHOT_OK|PSP_KEY_DOWN|PSP_KEY_UP)))	// �V���b�g�{�^��&��&�������邩�m�F
						{
							psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT);
							// �ŏI�I�ɑ������镨
							#if 0
							//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
							pad_config[KEY_NUM00_SELECT]	= key_setting[ 0];
							pad_config[KEY_NUM01_START] 	= key_setting[ 1];
							pad_config[KEY_NUM02_UP]		= key_setting[ 2];
							pad_config[KEY_NUM03_RIGHT] 	= key_setting[ 3];
							pad_config[KEY_NUM04_DOWN]		= key_setting[ 4];
							pad_config[KEY_NUM05_LEFT]		= key_setting[ 5];
							pad_config[KEY_NUM06_L_TRIG]	= key_setting[ 6];
							pad_config[KEY_NUM07_R_TRIG]	= key_setting[ 7];
							pad_config[KEY_NUM08_TRIANGLE]	= key_setting[ 8];
							pad_config[KEY_NUM09_CIRCLE]	= key_setting[ 9];
							pad_config[KEY_NUM10_CROSS] 	= key_setting[10];
							pad_config[KEY_NUM11_SQUARE]	= key_setting[11];
							#else
							{
								int i;
								for (i=0; i<KEY_NUM12_MAX; i++)
								{
									pad_config[i] = key_setting[i];
								}
							}
							#endif
							ini_save();
							voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*�e�L�g�[*/
						}
						else
						{
						//	"dame dayo"
							voice_play(VOICE09_GRAZE, TRACK01_EXPLODE);/*�e�L�g�[*/
						}
					}
				}
				else	/* �ݒ荀�ڂ�ύX����ꍇ( ����[ RESET ] ����[ QUIT ]�ȊO��I��ł���ꍇ) */
				{
					if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		// ���E�{�^������
					{
						int menu_cursor2;
						for (menu_cursor2=0; menu_cursor2<(KINOU_13_MAX-1)/*11 ???*/; menu_cursor2++)
						{
							if (const_pad_setting[menu_cursor2] == key_setting[menu_cursor1])		// �I���������ɉ��������Ă���̂����ׂ�
							{
								break;
							}
						}
						//
						if (my_pad_alter & PSP_KEY_LEFT)			// ���{�^������
						{
							menu_cursor2--;
							if ((KINOU_00_NONE) > menu_cursor2) {	menu_cursor2 = (KINOU_13_MAX-1); }
						}
						else
						if (my_pad_alter & PSP_KEY_RIGHT)			// �E�{�^������
						{
							menu_cursor2++;
							if ((KINOU_13_MAX-1) < menu_cursor2)	{	menu_cursor2 = KINOU_00_NONE; }
						}
						key_setting[menu_cursor1] = const_pad_setting[menu_cursor2];
						//
						voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*�e�L�g�[*/
					}
				}
			}
		}
		/* �`�� */
		{
		//	/*static*/ SDL_Rect rect_src_offset;		/* �ʒu */
			/*static*/ SDL_Rect rect_locate_offset; 	/* �\���ʒu */
			int i;
			int jj;
			/* �@�\�ԍ��̕`�� */
			for (i=0; i</*12*/(12)/*KEY_CONFIG_MENU_MAX*/; i++)
			{
				/* �{�^���ݒ�`�� */
				//kp_search(key_setting[i], i);
				//static void kp_search(int btn, int num)
				{
					int kinou_number;
					for (kinou_number=0; kinou_number<(MAX_KEY_NAMES_12-1); kinou_number++)/* (11==kinou_number �� NULL) */
					{
						if (const_pad_setting[kinou_number] == /*btn*/key_setting[i])
						{
							goto aaa_break/*break*/;
						}
					}
				aaa_break:;
					if ((MAX_KEY_NAMES_12-1)==kinou_number)/* ���t����Ȃ����� (11==kinou_number �� NULL) */
					{	kinou_number = 0;}
					if (menu_cursor1==i)	{	jj= 2;	}
					else					{	jj= 0;	}
					rect_locate_offset.x = X_LOCATE_OFFSET_04-(jj);
					rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/i<<4)-(jj);
					SDL_SetAlpha(	key_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
					SDL_BlitSurface(key_name_surface[kinou_number], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],&rect_src_offset,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
				}
			}
			/* ���Z�b�g�^�C�v�̕`�� */
			{
				if (menu_cursor1==12)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = X_LOCATE_OFFSET_03-(jj);
				rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/12/*i*/<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
			/* �{�^�����̕`�� */
			for (i=0; i<(MAX_BUTTON_12+MAX_MENU_02)/*KEY_CONFIG_MENU_MAX*/; i++)
			{
				if (menu_cursor1==i)	{	jj = 2; 	}
				else					{	jj = 0; 	}
				rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
				rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/ /*gazo_iti_henkan[i]*/i<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[i+(MAX_KINOU_11)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[i+(MAX_KINOU_11)], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
		}
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT):
		#if 0
		{
			int i;
			for (i=0; i<(MAX_KEY_NAMES_12-1)/*12 ???*/; i++)
			{
				/* �����Ńn���O�A�b�v */
				SDL_FreeSurface(key_name_surface[i]);
			}
		}
		#endif
		play_music_num(BGM_21_menu01);
		{
			//key_haikei_surface	= loadbmp("bg/title_bg.jpg");
			SDL_Surface *loadpic	= loadbmp0("bg/title_bg.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}
//		bg_alpha_aaa		= 255;
		bg_alpha_aaa		= 0;
		psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_07_FADE_OUT);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_07_FADE_OUT):
		bg_alpha_aaa += /*1*/2/*8*/;/*fps_factor*/
		if ((/*250-8*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			/* ���C�����j���[�ɖ߂� */
			psp_loop = (ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);
		}
		SDL_SetAlpha(/*key_haikei_surface*/sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		break;
	}
}
