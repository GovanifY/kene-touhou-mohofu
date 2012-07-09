
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�I�v�V�������j���[
	-------------------------------------------------------
	(r34)������̐���: ���̃��j���[�Ń|�[�Y�������邱�Ƃ͂ł��܂���B
	(return_call_func�ϐ������p���Ă�̂ŏo���Ȃ��B)
	-------------------------------------------------------
	�����R�[�h�����͗����邪�A�S�̂̍\�����V���v���ɉ���B(r34)
	�{���͑O�̓z(r33)�̕��� psp �Ɍ����Č����I�ȕ����Ȃ񂾂��ǁA
	�������ς����ꍇ�Ƀo�O���o�ɂ�������D�悷��B
---------------------------------------------------------*/

enum
{
	OPTION_MENU_00_PLAYER = 0,
	OPTION_MENU_01_BOMB,
	OPTION_MENU_02_BGM,
	OPTION_MENU_03_SOUND,
//	OPTION_MENU_04_CR_DIFF,
//	OPTION_MENU_05_CR_PLAYER,
//	OPTION_MENU_06_ANALOG,
	// ------------- �������͓��ʋ@�\ ----------
	OPTION_MENU_07_RESET,
	OPTION_MENU_08_KEYCONFIG,
	OPTION_MENU_09_QUIT,
	OPTION_MENU_MAX,
};
//#define OPTION_MENU_CONFIG_MAX (OPTION_MENU_06_ANALOG+1)
#define OPTION_MENU_CONFIG_MAX (OPTION_MENU_07_RESET)

enum
{
	KEY_TAB_00_0 = 0,
	KEY_TAB_01_1,
	KEY_TAB_02_2,
	KEY_TAB_03_3,
	KEY_TAB_04_4,
	KEY_TAB_05_5,
//	KEY_TAB_06_6,
//	KEY_TAB_07_7,
//
	KEY_TAB_08_OFF,
	KEY_TAB_09_ON,
//	KEY_TAB_10_OFF,
//	KEY_TAB_11_OGG,
//	KEY_TAB_12_IT,
//
	KEY_TAB_17_MAX,
};

/*---------------------------------------------------------

---------------------------------------------------------*/

/*extern*/global int option_config[OPTION_CONFIG_08_MAX];						// ����=�{�^���ϐ�

typedef struct
{
	u8 item;
	u8 chr_length;
	u8 offset;
	u8 item_default;/*[�����ݒ�l(�I�v�V�����t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ�A�⏉���������ꍇ)]*/
} OPTION_STATUS_SETTING;
#define EXT_MAX (2)
static const OPTION_STATUS_SETTING option_status_setting[OPTION_MENU_CONFIG_MAX+EXT_MAX] =
{
	{5, (2<<4), KEY_TAB_01_1,		(2)}, // (2)==3�@�ݒ�B 	OPTION_CONFIG_00_PLAYER = 0,
	{4, (2<<4), KEY_TAB_00_0,		(3)}, // (3)==3�{���ݒ�B	OPTION_CONFIG_01_BOMB,
	{2, (4<<4), KEY_TAB_08_OFF, 	(1)}, // (1)==BGM ���o���B	OPTION_CONFIG_02_BGM,
	{2, (4<<4), KEY_TAB_08_OFF, 	(1)}, // (1)==���ʉ��o���B	OPTION_CONFIG_03_SOUND,
//OPTION_MENU_CONFIG_MAX
	{4, (4<<4), KEY_TAB_00_0,		(1)}, // (1)==normal, (0)==easy OPTION_CONFIG_04_CURRENT_DIFFICULTY,
	{8, (1<<4), KEY_TAB_00_0,		(1)}, // (1)==�얲(A)	OPTION_CONFIG_05_CURRENT_PLAYER,
//	{2, (4<<4), KEY_TAB_08_OFF, 	(1)}, // OPTION_CONFIG_06_ANALOG,
//	�Q�[���i���ݒ�							OPTION_CONFIG_07_OPEN,
//											OPTION_CONFIG_08_MAX	/* �ő吔 */
/*
	�u�Q�[���i���ݒ�(�B���v�f)�v�̓��Z�b�g���܂���B
	(���K�ł���ʁA�I�ׂ�v���C���[�A�G�L�X�g���X�e�[�W��)
*/
};


/*---------------------------------------------------------
	�ݒ�l���f�t�H���g�ɖ߂��B
---------------------------------------------------------*/

global /*static*/ void set_default_option(int *option_setting_map)
{
	unsigned int i;
	for (i=0; i<(OPTION_MENU_CONFIG_MAX); i++)
	{
		option_setting_map[i] = option_status_setting[i].item_default;
	}
}


/*---------------------------------------------------------
	�ݒ�l�͈̔͂��`�F�b�N����B
---------------------------------------------------------*/

global /*static*/ void check_limit_value_option(int *option_setting_map)
{
	unsigned int i;
	for (i=0; i<(OPTION_MENU_CONFIG_MAX+EXT_MAX); i++)
	{
		if ((unsigned)option_status_setting[i].item <= (unsigned)option_setting_map[i])
		{
			option_setting_map[i] = option_status_setting[i].item_default;
		}
	}
}


/*---------------------------------------------------------
	�ϐ� / �萔
---------------------------------------------------------*/

static int option_setting[OPTION_CONFIG_08_MAX/*OPTION_MENU_MAX*/]; 	/* �I�v�V�������� */

#define MAX_KEY_NAMES_12	((KEY_TAB_17_MAX+1-1)+(OPTION_MENU_MAX))
static SDL_Surface *option_menu_name_surface[(MAX_KEY_NAMES_12)];		/* �����̕\���p�摜 */

#define X_LOCATE_OFFSET_02	(8/*80*/)
#define X_LOCATE_OFFSET_04	(160/*250*/)
#define Y_LOCATE_OFFSET 	(128-16/*40*/)

static int menu_cursor1;
//
static int bg_alpha_aaa;
#define slide_offset_x	bg_alpha_aaa


/*---------------------------------------------------------
	�`��
---------------------------------------------------------*/

static void option_menu_byouga(void)
{
	/* �`�� */
//	/*static*/ SDL_Rect rect_src_offset;		/* �ʒu */
	/*static*/ SDL_Rect rect_locate_offset; 	/* �\���ʒu */
	int i;
	u8 aaa_alpha;
	int jj;
	int kk;/* �c�̕����Ԋu */
	/* �@�\�ԍ��̕`�� */
	kk = 0;/* �c�̕����Ԋu */
	for (i=0; i</*12*/(OPTION_MENU_CONFIG_MAX); i++)
	{
		/* �{�^���ݒ�`�� */
		int i2;
		int ix3;
		ix3 = 0;
		int kinou_number;
		kinou_number = /*option_setting[i]+*/(option_status_setting[i].offset);
		for (i2=0; i2</*12*/(option_status_setting[i].item); i2++)
		{
			if (i2==option_setting[i])	/* �I���s */
			{
				if ((menu_cursor1==i))
						{	jj = 3; 	aaa_alpha = (0xff); /* color32 = 0xffff3fbf; ��s���� */	}	/* �I��� */
				else	{	jj = 2; 	aaa_alpha = (0xff); /* color32 = 0xff7f7f7f; �D�s���� */	}
			}
			else	/* ��I���s */
			{
				if ((menu_cursor1==i))
						{	jj = 0; 	aaa_alpha = (0xbf); /* color32 = 0x7fff3fbf; �򔼓��� */	}	/* �I���ςݍ��� */
				else	{	jj = 0; 	aaa_alpha = (0x7f); /* color32 = 0x7f7f7f7f; �D������ */	}
			}
			rect_locate_offset.x = slide_offset_x + X_LOCATE_OFFSET_04-(jj)+ix3;
			ix3 += ((option_status_setting[i].chr_length));
			rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/kk)-(jj);
			SDL_SetAlpha(	option_menu_name_surface[kinou_number+i2], SDL_SRCALPHA, aaa_alpha );
			PSPL_UpperBlit(option_menu_name_surface[kinou_number+i2], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
		}
		kk += (20);/* �c�̕����Ԋu */
	}
	/* ���j���[���̕`�� */
	kk = 0;
	for (i=0; i<(OPTION_MENU_MAX); i++)
	{
		if (menu_cursor1==i)	{	jj= 2;	}
		else					{	jj= 0;	}
		rect_locate_offset.x = slide_offset_x + (X_LOCATE_OFFSET_02)-(jj);
		rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/ /*gazo_iti_henkan[i]*/kk)-(jj);
		SDL_SetAlpha(	option_menu_name_surface[i+(KEY_TAB_17_MAX)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
		PSPL_UpperBlit(option_menu_name_surface[i+(KEY_TAB_17_MAX)], NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
		kk += (20);/* �c�̕����Ԋu */
	}
}


/*---------------------------------------------------------
	�����܂�
---------------------------------------------------------*/

static MAIN_CALL_FUNC(option_menu_terminate)
{
	#if 1/* �����̕\���p�摜���J������ */
	{
		unsigned int i;
		for (i=0; i<(MAX_KEY_NAMES_12); i++)
		{
			/* �����Ńn���O�A�b�v */
			SDL_FreeSurface(option_menu_name_surface[i]);/*xxx_ok?*/
		}
	}
	#endif
	/* �w�肵���ꏊ�ֈړ� */
	cb.main_call_func	= cb.menu_out_call_func;
}


/*---------------------------------------------------------
	�I��
---------------------------------------------------------*/

static MAIN_CALL_FUNC(option_menu_state_01_select_menu)
{
	/* ���j���[�I�� */
	{
		psp_pop_screen();
		if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
		{
			if (OPTION_MENU_07_RESET > menu_cursor1)
			{
				/* ���܂�d�v�łȂ��̂ŏ㉺���A�����ɂ��Ă݂� */
				if (psp_pad.pad_data & (/*PSP_KEY_UP|PSP_KEY_DOWN|*/PSP_KEY_LEFT|PSP_KEY_RIGHT))	// �㉺���E�{�^������ */
				{
					voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);
				}
			}
		//
			if (psp_pad.pad_data & PSP_KEY_UP)				// ��{�^������ */
			{
				if (OPTION_MENU_00_PLAYER == menu_cursor1)		{	menu_cursor1 = (OPTION_MENU_MAX-1);}
				else											{	menu_cursor1--;}
			}
			else
			if (psp_pad.pad_data & PSP_KEY_DOWN)			// ���{�^������ */
			{
				if ((OPTION_MENU_MAX-1) == menu_cursor1)		{	menu_cursor1 = OPTION_MENU_00_PLAYER;}
				else											{	menu_cursor1++;}
			}
			else	/* �㉺�ȊO�̒ʏ폈��(�΂ߓ��͂�r������ׁA�㉺�̓��͂��������ꍇ�͂���ȊO�̏��������Ȃ�) */
			{
				if (OPTION_MENU_07_RESET == menu_cursor1)			// ����[ RESET ] ��I��ł���ꍇ
				{
					if (psp_pad.pad_data & PSP_KEY_SHOT_OK) // �V���b�g�{�^������ */
					{
						voice_play_menu(VOICE07_BOMB, TRACK01_MENU01);/*�e�L�g�[*/
						set_default_option(option_setting);
						menu_cursor1 = OPTION_MENU_09_QUIT;
					}
				}
				else
				if (OPTION_MENU_09_QUIT == menu_cursor1)	// ����[ QUIT ] ��I��ł���ꍇ
				{
					if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL) 	// �L�����Z���{�^������
					{
						voice_play_menu(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
						cb.menu_out_call_func	= title_menu_start; 	/* �^�C�g�����j���[�ֈړ��w�� */
						cb.main_call_func		= option_menu_terminate;
					}
					else
					if (psp_pad.pad_data & PSP_KEY_SHOT_OK) 	// �V���b�g�{�^������ */
					{
						// ���j���[�𔲂���B */
						{
							// �ŏI�I�ɑ������镨 */
							{
								int i;
								for (i=0; i<OPTION_CONFIG_08_MAX; i++)
								{
									//	�V�X�e��(PSP�̃n�[�h�E�F�A)�� */
									option_config[i] = option_setting[i];
								}
							}
						//	ini_file_save(); */
							voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);/*(�g���b�N�z�u�̓e�L�g�[)*/
						}
						cb.menu_out_call_func	= title_menu_start; 	/* �^�C�g�����j���[�ֈړ��w�� */
						cb.main_call_func		= option_menu_terminate;
					}
				}
				else
				if (OPTION_MENU_08_KEYCONFIG == menu_cursor1)	/* ����[ KEY CONFIG ] ��I��ł���ꍇ */
				{
					if (psp_pad.pad_data & PSP_KEY_SHOT_OK) 	/* �V���b�g�{�^������ */
					{
						voice_play_menu(VOICE07_BOMB, TRACK01_MENU01);/*(�g���b�N�z�u�̓e�L�g�[)*/
						cb.menu_out_call_func	= key_config_start; 	/* �L�[�R���t�B�O���j���[�ֈړ��w�� */
						cb.main_call_func		= option_menu_terminate;
					}
				}
				else	/* �ݒ荀�ڂ�ύX����ꍇ( ����[ RESET ] ����[ QUIT ]�ȊO��I��ł���ꍇ) */
				{
					if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT))	/* ���E�{�^������ */
					{
						int menu_cursor2;
						/* �I���������ɉ��������Ă���̂����ׂ� */
						menu_cursor2 = option_setting[menu_cursor1];
						//
						if (psp_pad.pad_data & PSP_KEY_LEFT)		/* ���{�^������ */
						{
							menu_cursor2--;
							if ((0/*KINOU_00_NONE*/) > menu_cursor2)	{	menu_cursor2 = (option_status_setting[menu_cursor1].item-1); }
						}
						else
						if (psp_pad.pad_data & PSP_KEY_RIGHT)		/* �E�{�^������ */
						{
							menu_cursor2++;
							if ((option_status_setting[menu_cursor1].item-1) < menu_cursor2)	{	menu_cursor2 = 0/*KINOU_00_NONE*/; }
						}
						option_setting[menu_cursor1] = menu_cursor2;
						//
//						voice_play(VOICE02_MENU_SELECT, TRACK01_MENU01);/*�e�L�g�[*/
					}
				}
			}
		}
	}
	option_menu_byouga();/*(�`��)*/
}


/*---------------------------------------------------------
	�t�F�[�h�C����
---------------------------------------------------------*/

static MAIN_CALL_FUNC(option_menu_state_00_slide_in)
{
	{
	//	bg_alpha_aaa += /*1*/2/*6*/;/*fps_factor*/
		slide_offset_x -= (10);/*(8)*/		/*fps_factor*/
	//	if ((/*250-6*/224) < bg_alpha_aaa)
		if (slide_offset_x < 0)
		{
	//		bg_alpha_aaa = 255;
			slide_offset_x = 0;
//			psp_push_screen();
			cb.main_call_func = option_menu_state_01_select_menu;
		}
//		SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();
	}
	option_menu_byouga();/*(�`��)*/
}


/*---------------------------------------------------------
	�J�n
---------------------------------------------------------*/

global MAIN_CALL_FUNC(option_menu_start)
{
	#if 0
	//	�V�X�e��(PSP�̃n�[�h�E�F�A)��
	option_setting[ 0] = option_config[ 0];
	option_setting[ 1] = option_config[ 1];
	option_setting[ 2] = option_config[ 2];
	option_setting[ 3] = option_config[ 3];
	option_setting[ 4] = option_config[ 4];
	option_setting[ 5] = option_config[ 5];
	option_setting[ 6] = option_config[ 6];
	option_setting[ 7] = option_config[ 7];
	#else
	{
		int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			option_setting[i] = option_config[i];
		}
	}
	#endif
	{
		static const char *const_key_str[(MAX_KEY_NAMES_12)] =
		{
			"0",
			"1",
			"2",
			"3",
			"4",
			"5",
//			"6",
//			"7",

			"Off",
			"On",
//			"Off",
//			"Ogg",
//			"IT",

			"Player",
			"Bomb",
			"BGM",
			"Sound",
//			"Current Diff",
//			"Current Player",
//			"Analog",

			"Reset",
			"Keyconfig",
			"Quit",
		};
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_12); i++)
			{
				strcpy(my_font_text, (char *)const_key_str[i]);
				option_menu_name_surface[i] = font16_render_new_surface();
			}
		}
	}
	menu_cursor1				= OPTION_MENU_09_QUIT/*0*/;
	slide_offset_x/*bg_alpha_aaa*/				= (256);	/*(480) 0*/
	//
//	menu_out_call_func = title_menu_start;		/* �^�C�g�����j���[�ֈړ��w�� */
	cb.main_call_func = option_menu_state_00_slide_in;
}
