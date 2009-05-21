
#include "support.h"

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
//��{�͐�Βl�w��(�@�\�Œ�l)��support.c��keypoll�̒���keyconfig�����z��w��ԍ��ɕϐ��ikeyconfig[key_xxx�j�����
//�܂�keypoll�ɓ���l�́A0��1�����̂܂܂��ƃ{�^���ݒ���肪�o��̂Ńr�b�g�ŊǗ�����悤�ɂ���
/*static*/ //void key_config_init()
//{
//	//key_config_state= (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_00_INIT);
//	psp_loop=(ST_WORK_KEY_CONFIG|0);
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
	static const /*int*/Uint8 default_key_map[4][12] =
	{
		{	/* type 01 �͕핗 �W�� (�X�����̈ӌ��Ō��܂���) */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_09_SLOW,		KINOU_10_OPTION,	KINOU_11_SHOT,			KINOU_12_BOMB,
		},
		{	/* type 02 toho DX �݊� */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_09_SLOW,		KINOU_10_OPTION,	KINOU_12_BOMB,			KINOU_11_SHOT,
		},
		{	/* type 03 psp-1000 (���{�^�����J�X�Ȃ̂Ŏg���Ȃ��ꍇ) */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_09_SLOW,		KINOU_11_SHOT,		KINOU_12_BOMB,			KINOU_10_OPTION,
		},
		{	/* type 04 psp-2000 (���{�^�����V���b�g�̏ꍇ) */
			KINOU_01_SELECT,	KINOU_02_PAUSE, 	KINOU_03_UP,			KINOU_04_RIGHT, /* KINOU_12_BOMB KINOU_00_NONE*/
			KINOU_05_DOWN,		KINOU_06_LEFT,		KINOU_07_SNAP_SHOT, 	KINOU_09_SLOW,
			KINOU_12_BOMB,		KINOU_10_OPTION,	KINOU_09_SLOW,			KINOU_11_SHOT,
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

extern SDL_Surface *back_screen;
extern void ini_save(void); 	// [***090115
/*static*/ void key_config_work(void)
{

#define MAX_KINOU_11			(12+1)
#define MAX_BUTTON_12			(12)
#define MAX_MENU_02 			(2)
#define MAX_DEFAULT_MAP_04		(4)

#define MAX_KEY_SETTING_12 (1+12+1)
	static int key_setting[MAX_KEY_SETTING_12]; 						// ����=�{�^���ϐ� ���т� "bg/key_haikei_surface.png"
	static const int const_key_setting[MAX_KEY_SETTING_12/*11*/] =		// ����=�@�\�萔 ���т� "fonts/keylist.png"
	{	// ���� = �@�\�萔 ���сB	[[ �I��������я��� ]]
		KINOU_00_NONE,
	#if 0
		KINOU_11_SHOT,
		KINOU_12_BOMB,
		KINOU_09_SLOW,
		KINOU_03_UP,
		KINOU_05_DOWN,
		KINOU_06_LEFT,
		KINOU_04_RIGHT,
		KINOU_02_PAUSE,
		KINOU_10_OPTION,
		KINOU_07_SNAP_SHOT,
		KINOU_01_SELECT,
		KINOU_08_SYSTEM,
	#else
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
	#endif
		0
	};
	#define X_LOCATE_OFFSET_01	(64)/*(80-16)*/
	#define X_LOCATE_OFFSET_02	(80)
	#define X_LOCATE_OFFSET_03	(200)
	#define X_LOCATE_OFFSET_04	(250)
	#define Y_LOCATE_OFFSET 	(40)
	#define MAX_KEY_NAMES_12	(MAX_KEY_SETTING_12+MAX_BUTTON_12+MAX_MENU_02+MAX_DEFAULT_MAP_04)
	static SDL_Surface *key_name_surface[MAX_KEY_NAMES_12]; 	// �����̉摜


	static int menu_cursor1;
	static int key_setting_default_type;
//
	static int bg_alpha_aaa;
//	int i;
//1231881
	switch (psp_loop/*key_config_state*/)
	{
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_00_INIT):
		play_music(BGM_05_stage5);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/key_config.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, back_screen, NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}

		#if 0
		//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
		key_setting[ 0] = keyconfig[key_00_sl];
		key_setting[ 1] = keyconfig[key_01_st];
		key_setting[ 2] = keyconfig[key_02_u ];
		key_setting[ 3] = keyconfig[key_03_r ];
		key_setting[ 4] = keyconfig[key_04_d ];
		key_setting[ 5] = keyconfig[key_05_l ];
		key_setting[ 6] = keyconfig[key_06_lt];
		key_setting[ 7] = keyconfig[key_07_rt];
		key_setting[ 8] = keyconfig[key_08_sa];
		key_setting[ 9] = keyconfig[key_09_ma];
		key_setting[10] = keyconfig[key_10_ba];
		key_setting[11] = keyconfig[key_11_si];
		#else
		{
			int i;
			for (i=0; i<12; i++)
			{
				key_setting[i] = keyconfig[i];
			}
		}
		#endif
		{
			static const char *const_key_str[(MAX_KEY_NAMES_12)] =
			{
				//	[[ �I��������я��� ]]
				"NONE",
				#if 0
				"SHOT / OK",
				"BOMB / CANCEL",
				"SLOW",
				"UP",
				"DOWN",
				"LEFT",
				"RIGHT",
				"PAUSE",
				"OPTION",
				"SNAP SHOT",
				"SELECT",
				"SYSTEM",
				#else
				"SELECT",
				"PAUSE",
				"UP",
				"RIGHT",
				"DOWN",
				"LEFT",
				"SNAP SHOT",
				"SYSTEM",
				"SLOW",
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
		bg_alpha_aaa += /*1*/2/*6*/;/**fps_fa ctor*/
		if ((/*250-6*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			psp_push_screen();
			psp_loop++;//key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_02_SELECT_LEFT_MENU);
		}
		SDL_SetAlpha(/*key_haikei_surface*/back_screen, SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,screen,NULL);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_02_SELECT_LEFT_MENU):
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,screen,NULL);
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
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
					}
					else
					if (my_pad_alter & PSP_KEY_RIGHT)	// �E�{�^������
					{
						key_setting_default_type++;
						key_setting_default_type &= (4-1);
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
					}
					else
					if (my_pad_alter & PSP_KEY_SHOT_OK) // �V���b�g�{�^������
					{
						play_voice_auto_track(VOICE07_BOMB);
						set_default_key(key_setting, key_setting_default_type);
						menu_cursor1 = KEY_CONFIG_MENU_13_QUIT;
					}
				}
				else
				if (KEY_CONFIG_MENU_13_QUIT == menu_cursor1)	// ����[ QUIT ] ��I��ł���ꍇ
				{
					if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	// �L�����Z���{�^������
					{
						play_voice_auto_track(VOICE04_SHIP_HAKAI);
						psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT);
						//key_haikei_surface = loadbmp("bg/title_bg.jpg");
//						bg_alpha_aaa = 255;
//						play_music(BGM_00_intro);
					}
					else
					if (my_pad_alter & PSP_KEY_SHOT_OK) 	// �V���b�g�{�^������
					{
						#define FLAG_KEY_SHOT	(1<<0)
						#define FLAG_KEY_UP 	(1<<1)
						#define FLAG_KEY_DOWN	(1<<2)
						int saiteigen_exsist;	// ���݊m�F�p�B0�ő��݂��Ȃ��B���݂��Ȃ��ƃ��j���[�𔲂���Ȃ��B
						saiteigen_exsist = 0;
						{
							int i;
							for (i=0; i<MAX_KEY_SETTING_12; i++)	// �V���b�g�{�^��&��&�������邩�m�F
							{
								if (key_setting[i] == KINOU_11_SHOT)	{	saiteigen_exsist	|= FLAG_KEY_SHOT;	}
								if (key_setting[i] == KINOU_03_UP)		{	saiteigen_exsist	|= FLAG_KEY_UP; 	}
								if (key_setting[i] == KINOU_05_DOWN)	{	saiteigen_exsist	|= FLAG_KEY_DOWN;	}
							}
						}
						if ((FLAG_KEY_SHOT|FLAG_KEY_DOWN|FLAG_KEY_UP)==saiteigen_exsist)
						{
							psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_06_FADE_INIT);
							//key_haikei_surface = loadbmp("bg/title_bg.jpg");
//							bg_alpha_aaa = 255;
//							play_music(BGM_00_intro);
							// �ŏI�I�ɑ������镨
							#if 0
							//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
							keyconfig[key_00_sl] = key_setting[ 0];
							keyconfig[key_01_st] = key_setting[ 1];
							keyconfig[key_02_u]  = key_setting[ 2];
							keyconfig[key_03_r]  = key_setting[ 3];
							keyconfig[key_04_d]  = key_setting[ 4];
							keyconfig[key_05_l]  = key_setting[ 5];
							keyconfig[key_06_lt] = key_setting[ 6];
							keyconfig[key_07_rt] = key_setting[ 7];
							keyconfig[key_08_sa] = key_setting[ 8];
							keyconfig[key_09_ma] = key_setting[ 9];
							keyconfig[key_10_ba] = key_setting[10];
							keyconfig[key_11_si] = key_setting[11];
							#else
							{
								int i;
								for (i=0; i<12; i++)
								{
									keyconfig[i] = key_setting[i];
								}
							}
							#endif
							ini_save();
							play_voice_auto_track(VOICE02_ZAKO_HAKAI);
						}
						else
						{
						//	"dame dayo"
							play_voice_auto_track(VOICE09_GRAZE);
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
							if (const_key_setting[menu_cursor2] == key_setting[menu_cursor1])		// �I���������ɉ��������Ă���̂����ׂ�
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
						key_setting[menu_cursor1] = const_key_setting[menu_cursor2];
						//
						play_voice_auto_track(VOICE02_ZAKO_HAKAI);
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
			for (i=0; i</*12*/(12)/*MAX_KEY_SETTING_12*/; i++)
			{
				/* �{�^���ݒ�`�� */
				//kp_search(key_setting[i], i);
				//static void kp_search(int btn, int num)
				{
					int kinou_number;
					for (kinou_number=0; kinou_number<(MAX_KEY_NAMES_12-1); kinou_number++)/* (11==kinou_number �� NULL) */
					{
						if (const_key_setting[kinou_number] == /*btn*/key_setting[i])
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
					{	/* �W���`������key_config�摜�`��(key_bg�̕��ѕ�)�֕ϊ�����  */
					//	static const Uint8 gazo_iti_henkan[12] = { 10,11,4,7, 5,6,9,8, 3,0,1,2 };
						rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/ /*gazo_iti_henkan[i]*/i<<4)-(jj);
					}
					SDL_SetAlpha(	key_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
					SDL_BlitSurface(key_name_surface[kinou_number], NULL,screen,&rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	SDL_BlitSurface(back_screen,&rect_src_offset,screen,&rect_locate_offset);
				}
			}
			/* ���Z�b�g�^�C�v�̕`�� */
			{
				if (menu_cursor1==12)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = X_LOCATE_OFFSET_03-(jj);
				rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/12/*i*/<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[(MAX_KINOU_11+MAX_BUTTON_12+MAX_MENU_02)+key_setting_default_type], NULL,screen,&rect_locate_offset);
			}
			/* �{�^�����̕`�� */
			for (i=0; i<(MAX_BUTTON_12+MAX_MENU_02)/*MAX_KEY_SETTING_12*/; i++)
			{
				if (menu_cursor1==i)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
				rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/ /*gazo_iti_henkan[i]*/i<<4)-(jj);
				SDL_SetAlpha(	key_name_surface[i+(MAX_KINOU_11)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(key_name_surface[i+(MAX_KINOU_11)], NULL,screen,&rect_locate_offset);
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
		play_music(BGM_00_intro);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/title_bg.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, back_screen, NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}
		bg_alpha_aaa		= 0;
		psp_loop++;//	key_config_state = (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_07_FADE_OUT);
		break;
	case (ST_WORK_KEY_CONFIG|KEY_CONFIG_STATE_07_FADE_OUT):
		bg_alpha_aaa += /*1*/2/*8*/;/**fps_fa ctor*/
		if ((/*250-8*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			/* ���C�����j���[�ɖ߂� */
			psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	//newsta te(ST_MENU/*ST_INTRO*/,0,1);
		}
		SDL_SetAlpha(/*key_haikei_surface*/back_screen, SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,screen,NULL);
		break;
	}
}
