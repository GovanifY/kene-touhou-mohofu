
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���ʑI�����j���[
	-------------------------------------------------------
	�X�e�[�W�I�����j���[
	�|�[�Y ���j���[
	�R���e�B�j���[�H���j���[
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kaiwa_system.h"	/* ��b�V�X�e����SDL�ĕ`��w���B */
#include "kanji_system.h"
//#include "common_menu.h"

enum
{
//	MENU_TYPE_00_SELECT_STAGE	= 0,
	MENU_TYPE_01_PAUSE	= 0,
	MENU_TYPE_02_ASK_CONTINUE,
	MENU_TYPE_99_MAX	/* ���j���[�̐� */
};

#if 0
enum
{
	MENU_ITEM_00_CONTINUE_GAME	= 0,
	MENU_ITEM_01_RETRY_GAME,
	MENU_ITEM_02_QUIT_GAME,
	MENU_ITEM_99_MAX	/* �I�u�W�F�N�g�̐� */
};

enum
{
	MENU_ITEM_00_CONTINUE_YES	= 0,
	MENU_ITEM_01_CONTINUE_NO,
	MENU_ITEM_99_MAX	/* �I�u�W�F�N�g�̐� */
};
#endif

typedef struct
{
	u16 offset_x;	// �\���p x �I�t�Z�b�g */
	u16 offset_y;	// �\���p y �I�t�Z�b�g */
} MY_OFFSET;

#define SPEC_08_MENU_ITEM_MAX (8)
typedef struct
{
	MY_OFFSET		menu_item_my_obj[SPEC_08_MENU_ITEM_MAX];			/* �I�u�W�F�N�g */
	unsigned int	max_items;			/* �I�v�V�����̍��ڐ� */
	int 			selected_number;	/* ���݃��j���[��őI������Ă��鍀�ڔԍ� */
	int 			menu_offset;		/* ���j���[�̃I�t�Z�b�g */
//
	int 			menu_brite; 		/* ���j���[�̖��邳(���l) */
	int 			menu_type;			/* ���j���[�̎�� */
} MENU;
static MENU aaa;

/*---------------------------------------------------------

---------------------------------------------------------*/

//	#define FPS_MENU_FACTOR8	(8*fps_factor)
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
//	MENU_RESOURCES_00_STAGE01 = 0,
//	MENU_RESOURCES_01_STAGE02,
//	MENU_RESOURCES_02_STAGE03,
//	MENU_RESOURCES_03_STAGE04,
//	MENU_RESOURCES_04_STAGE05,
//	MENU_RESOURCES_05_STAGE06,
//	MR_06_STOP,//----------------
	MENU_RESOURCES_07_CONTINUE = 0,
	MENU_RESOURCES_08_RESTART,
	MENU_RESOURCES_09_QUIT,
	MR_10_STOP,//----------------
	MENU_RESOURCES_11_YES,
	MENU_RESOURCES_12_NO,
	MR_13_STOP,//----------------
	MENU_RESOURCES_14_TITLE01,
	MENU_RESOURCES_15_TITLE02,
	MENU_RESOURCES_16_TITLE03,
	MENU_RESOURCES_16_TITLE04,
	MR_17_STOP,//----------------
	MENU_RESOURCES_99_MAX
};
typedef struct
{
	u16 x_offset;	// �\���p x �I�t�Z�b�g[pixel] */
	u16 y_offset;	// �\���p y �I�t�Z�b�g[pixel] */
	const char *str_name;
} MENU_RESOURCE;

static const MENU_RESOURCE my_menu_resource[/*8*/MENU_RESOURCES_99_MAX/*10*/] =
{
	//	RES01_STAGE_SELECT	/*const*/ char *stage_select_menu_options[] =/*(char *)*/stage_select_menu_options
//	{	360,				(113) +((0)*21),	"1",						},	// stage1 */
//	{	350,				(113) +((1)*21),	"2",						},	// stage2 */
//	{	340,				(113) +((2)*21),	"3",						},	// stage3 */
//	{	330,				(113) +((3)*21),	"4",						},	// stage4 */
//	{	320,				(113) +((4)*21),	"5",						},	// stage5 */
//	{	310,				(113) +((5)*21),	"6",						},	// stage6 */
//	{	275,				(113) +((6)*21),	"ENDING",					},	// ending */
//	{	0,0,NULL,	},//----------------
	{	GAME_X_OFFSET+(96), (115) +((0)*21),	"�ꎞ��~����", 			},
	{	GAME_X_OFFSET+(96), (115) +((1)*21),	"�n�߂����蒼��", 		},
	{	GAME_X_OFFSET+(96), (115) +((2)*21),	"�^�C�g���֖߂�",			},
	{	0,0,NULL,	},//----------------
	{	GAME_X_OFFSET+(144), (138) +((0)*21),	"�͂�", 					},
	{	GAME_X_OFFSET+(136), (138) +((1)*21),	"������",					},
	{	0,0,NULL,	},//---------------- --------012345
	{	GAME_X_OFFSET+(32), (16),				"���� 0��R���e�B�j���[�ł��܂�",	},
	{	GAME_X_OFFSET+(32), (48),				"�R���e�B�j���[�����ꍇ�A", 		},
	{	GAME_X_OFFSET+(32), (48+18),			"�X�R�A�����L���O����܂���",		},
	{	GAME_X_OFFSET+(32), (120),				"�R���e�B�j���[���܂����H", 		},
	{	0,0,NULL,	},//----------------
};
/*---------------------------------------------------------
	�R���e�B�j���[���j���[�̏ꍇ�̌��o��������`��
---------------------------------------------------------*/
static void render_continue(void)
{
		kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
		cg.msg_time = (65536);	/* �� 18 �� */
	{
		unsigned int yyy18;
		yyy18 = (3*18);/* Y�ʒu */
		unsigned int i;
		for (i=0; i<4; i++)
		{
			/* 4�s�ڂ̂ݓ���Ȉʒu�ֈړ��B */
			ml_font[(i+3)].x		= my_menu_resource[i+MENU_RESOURCES_14_TITLE01].x_offset;/* X�ʒu */
			ml_font[(i+3)].y		= my_menu_resource[i+MENU_RESOURCES_14_TITLE01].y_offset;/* Y�ʒu */
			ml_font[(i+3)].timer	= ML_ON;
			strcpy(my_font_text, my_menu_resource[i+MENU_RESOURCES_14_TITLE01].str_name );
			/* �R���e�j���[�񐔂̕\�� */
			if (0==i)
			{
				dec_print_format( ((cg.game_now_max_continue)), 2, (char *)&my_font_text[4]);
			}
			kanji_window_clear_line((i+3)); 	/* �����E�B���h�E��4�s��(==3)�̓��e�������B */
			set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (yyy18) ); /* �J�[�\����4�s��(==3)�ֈړ� */
			kanji_color(7);/*(���F)*/
			kanji_draw();
			//
			yyy18 += (18);
		}
	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/
static void stage_select_menu_draw_all(void)
{
	/* �`�� */
	psp_pop_screen();
	{
		unsigned int yyy18;
		yyy18 = (0);/* Y�ʒu */
		kanji_cursor_move_home_position();		/* �����J�[�\�����z�[���|�W�V�����ֈړ� */
		cg.msg_time = (65536);	/* �� 18 �� */
		unsigned int i;
		for (i=0; i<aaa.max_items; i++)
		{
			unsigned int text_color;
			text_color = (7);/*(���F)*/
			//
			ml_font[i].x		= (aaa.menu_item_my_obj[i].offset_x);
			ml_font[i].y		= (aaa.menu_item_my_obj[i].offset_y);
			if (((signed)i)==aaa.selected_number)	/* �I�����ꂽ */
			{
				text_color = (14);/*(���F)*/
				ml_font[i].x += (2);	/* �I�t�Z�b�g�� 2[pixel] �ɂ��� */
				ml_font[i].y += (2);	/* �I�t�Z�b�g�� 2[pixel] �ɂ��� */
			}
			ml_font[i].timer	= ML_ON;
			//
			kanji_window_clear_line(i); 	/* �����E�B���h�E��n�s��(==0)�̓��e�������B */
			set_kanji_xy( (0), (yyy18) );
			strcpy(my_font_text, (char *)my_menu_resource[aaa.menu_offset+i].str_name);
			kanji_color(text_color);
			kanji_draw();
			//
			yyy18 += (18);
		}
	}
}


/*---------------------------------------------------------
	�I�������ƈړ������B
---------------------------------------------------------*/

extern void gamecore_term(void);
extern void player_continue_value(void);
static void common_menu_work_MENU_STATE_03_FININSH(void)
{
//	SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, 255);
	psp_pop_screen();
	//
//	clear_kanji_line(); 	/* �����E�B���h�E�̈ʒu��߂����菉����������B */
//	set_kanji_hide_line(ML_LINE_02);/*(2�s�ڈȉ����\���ɂ���B)*/ 	/*(�Q�[�����[�h�̏ꍇ)*/
	set_kanji_hide_line(ML_LINE_04);/*(4�s�ڈȉ����\���ɂ���B)*/ 	/*(�V�i���I���[�h�̏ꍇ)*/
	kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	//
	if (MENU_TYPE_01_PAUSE==aaa.menu_type)
	{
		/* �{�X�f�o�b�O�p USE_r36_SCENE_FLAG */
		#if 0//1/*(1==DEBUG_MODE)*/
	//	if (psp_pad.pad_data & PSP_KEY_RIGHT)
		{
		//	cg_game_now_max_continue = (90);/*test*/	/* �����L���O�ɂ����Ȃ� */
		//	cg.game_score	= (8);/*test*/
		//	cg.graze_point	= (0);/*test*/
		//	cg.zanki		= (8);/*test*/
		//	cg.bombs		= (8);/*test*/
		//	#if (0)
		//	/* �{�X�`�F�b�N�p */
		//	pd_weapon_power = MAX_POWER_IS_128;/*test*/
		//	#endif
			cg.game_score	= ((cg.state_flag>>8) & 0xff);/*(��ɕ\��)*/
			cg.graze_point	= (cg.game_score);/*test*/
		}
		#endif
		#if 0//1/*(1==DEBUG_MODE)*/
	//	else
		if (psp_pad.pad_data & PSP_KEY_LEFT)
		{
			#if (1)//(USE_r36_SCENE_FLAG)
			NEXT_SCENE;
			#endif
		//	cg_game_now_max_continue = (90);/*test*/	/* �����L���O�ɂ����Ȃ� */
			cg.game_score	= (0);/*test*/
		//	cg.graze_point	= (0);/*test*/
		//	cg.zanki		= (8);/*test*/
		//	cg.bombs		= (8);/*test*/
		//	#if (0)
		//	/* �{�X�`�F�b�N�p */
		//	pd_weapon_power = MAX_POWER_IS_128;/*test*/
		//	#endif
		}
		#endif
		if (/*MENU_ITEM_00_CONTINUE_GAME*/0 == aaa.selected_number)/* Continue Game */
		{
			#if 1/*(Gu������������v��Ȃ��Ȃ�)*/
			{
				psp_clear_screen(); 	/* [PAUSE] ���A����SDL��ʂ������B */
			}
			/* SDL�Ȃ̂ł��̋@�\���K�v�BGu������������v��Ȃ��Ȃ� */
			kaiwa_system_set_re_draw(); 	/* ��b�V�X�e����SDL�ĕ`��w���B */
			#endif
			cb.main_call_func = cb.pause_out_call_func;
		}
		else
		if (/*MENU_ITEM_01_RETRY_GAME*/1 == aaa.selected_number)/* Retry Game */
		{
			gamecore_term();
		//	cg.msg_time = (0);/* �K�v(?) */ 	/* �� 0 �b */
			cb.main_call_func = stage_first_init;	/* �n�߂��� */
		}
		else
	//	if (MENU_ITEM_02_QUIT_GAME == selected_number)/* Quit Game */
		{
			gamecore_term();
			cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
		}
	}
	else
	if (MENU_TYPE_02_ASK_CONTINUE==aaa.menu_type)
	{
		if (/*MENU_ITEM_00_CONTINUE_YES*/0 == aaa.selected_number)
		{
			/* Continue Game */
			player_continue_value();

			#if (1)//(USE_r36_SCENE_FLAG)
			/*(??????????????)*/
			//	  (cg.state_flag & 0x00008000u)//�v���C���[���[�v�𔲂��鏈��(�Ƃ肠����??????)
			cg.state_flag &= 0xffff7fffu;/*(�Ƃ肠�����A���ݎd�l���ǂ��킩��Ȃ�)*/
			#endif

			#if 1/*(Gu������������v��Ȃ��Ȃ�)*/
			{
				psp_clear_screen(); /* [PAUSE] ���A����SDL��ʂ������B */
			}
			#endif
			cb.main_call_func = shooting_game_core_work;
		}
		else
		{ /* Game over /Quit Game */
		//	gamecore_term();
		//	cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
			cb.main_call_func = gameover_start;
		}
	}
}


/*---------------------------------------------------------
	���񂾂�Â�����B�Â��Ȃ����玟�ցB
---------------------------------------------------------*/

static void common_menu_work_MENU_STATE_02_FADE_OUT(void)
{
	aaa.menu_brite -= FPS_MENU_FACTOR8;
	if (0 >= (aaa.menu_brite) )
	{
		aaa.menu_brite	= (0);
		cb.main_call_func = common_menu_work_MENU_STATE_03_FININSH; /* */
	}
	stage_select_menu_draw_all();
}
/*---------------------------------------------------------
	���j���[�L�[���͎󂯕t���B�m�肵���玟�ցB
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_01_WORK_MENU(void)
{
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		/*(�㉺���������ꍇ�ɑ��쉹���o���B)*/
		if (psp_pad.pad_data & (PSP_KEY_DOWN|PSP_KEY_UP/*|PSP_KEY_PAUSE|PSP_KEY_SELECT*/))
		{
			voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);
		}
		#if 0/*(-r34)*/
		/*(�㉺���������ꍇ�A��荞�ݑI��������B�㉺�͕K�����[�v)*/
		if (psp_pad.pad_data & PSP_KEY_DOWN)
		{
			if (aaa.selected_number == aaa.max_items-1)
					{	aaa.selected_number = 0;	}
			else	{	aaa.selected_number++;		}
		}
		else
		if (psp_pad.pad_data & PSP_KEY_UP)
		{
			if (0 == aaa.selected_number)
					{	aaa.selected_number = aaa.max_items-1;	}
			else	{	aaa.selected_number--;		}
		}
		#else/*(r35-)*/
		/*(�㉺���������ꍇ�A�ő�ŏ��I��������B���j���[�㉺�͕K����~)*/
		if (psp_pad.pad_data & PSP_KEY_DOWN)
		{	aaa.selected_number++;		aaa.selected_number = psp_min(aaa.selected_number, (aaa.max_items-1) ); 	}	/* ���j���[���Œ�~ */
		else
		if (psp_pad.pad_data & PSP_KEY_UP)
		{	aaa.selected_number--;		aaa.selected_number = psp_max(aaa.selected_number, (0) );					}	/* ���j���[��Œ�~ */
		#endif
		{ /* �V���[�g�J�b�g�@�\ */
		//	if (MENU_TYPE_00_SELECT_STAGE==aaa.menu_type)
		//	{
		//	}
		//	else
//			if (MENU_TYPE_00_SELECT_STAGE!=aaa.menu_type)
		//	if (MENU_TYPE_01_PAUSE==aaa.menu_type)
		//	if (MENU_TYPE_02_ASK_CONTINUE==aaa.menu_type)
			{
				/* �Z���N�g�L�[���������ꍇ�A�N�C�b�N���X�^�[�g�B("�n�߂���")  (����̃L�[�{�[�h�V���[�g�J�b�g�@�\) */
				/* �Z���N�g�L�[���������ꍇ�A�K���R���e�B�j���[���Ȃ��B */
				if (psp_pad.pad_data & PSP_KEY_SELECT)					/* [select]�{�^����("�n�߂���") */
			//	if (psp_pad.pad_data & PSP_KEY_SELECT)					/* [select]�{�^���ŕK���R���e�B�j���[���Ȃ� */
				{
					aaa.selected_number 	= 1;//MENU_ITEM_01_RETRY_GAME;
				//	aaa.selected_number 	= 1;//MENU_ITEM_01_CONTINUE_NO;
					cb.main_call_func = common_menu_work_MENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
				}
				/* �|�[�Y�L�[���������ꍇ�A�|�[�Y�����B */
				/* �|�[�Y�L�[���������ꍇ�A�K���R���e�B�j���[����B */
				if (psp_pad.pad_data & PSP_KEY_PAUSE)				/* [start]�{�^���Ń|�[�Y���� */
			//	if (psp_pad.pad_data & PSP_KEY_PAUSE)				/* [start]�{�^���ŕK���R���e�B�j���[���� */
				{
					aaa.selected_number 	= 0;//MENU_ITEM_00_CONTINUE_GAME;
				//	aaa.selected_number 	= 0;//MENU_ITEM_00_CONTINUE_YES;
					cb.main_call_func = common_menu_work_MENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
				}

			}
		}
		if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
		{
			voice_play_menu(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_MENU01);
			cb.main_call_func = common_menu_work_MENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
		}
	}
	stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	���񂾂񖾂邭����B���邭�Ȃ����玟�ցB
---------------------------------------------------------*/

static void common_menu_work_MENU_STATE_00_FADE_IN_MENU(void)
{
	aaa.menu_brite += FPS_MENU_FACTOR8;
	if ((255) <= (aaa.menu_brite) )
	{
		aaa.menu_brite = (255);
		cb.main_call_func = stage_select_menu_work_MENU_STATE_01_WORK_MENU; /* */
	}
	stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	���ʃ��j���[�������B
---------------------------------------------------------*/

static void common_menu_initialize(void)
{
	{
		/* ���ʃ��j���[�A���[�N�G���A�̏����� */
		unsigned int i;
		for (i=0; i<SPEC_08_MENU_ITEM_MAX; i++)
		{
			aaa.menu_item_my_obj[i].offset_x	= (0);
			aaa.menu_item_my_obj[i].offset_y	= (0);
		}
		// ���j���[�Ɏg�p����T�u���ڂ̐��𒲂ׂ�B
		{
			aaa.max_items		= (0);
			{
		loop:
				#if 0
				/* �Z���N�g�X�e�[�W���j���[�̏ꍇ */
				if (MENU_TYPE_00_SELECT_STAGE==aaa.menu_type)
				{
					/* ������ꂽ�����܂ŁB */
					if (aaa.max_items>(option_config[OPTION_CONFIG_07_OPEN]&0x07))	{	goto loop_quit;}
				}
				#endif
				/* (�I���܂Œ��ׂ�) */
				/*while*/if (NULL == my_menu_resource[aaa.menu_offset+aaa.max_items].str_name)		{	goto loop_quit;}
				aaa.max_items++;
				goto loop;
		loop_quit:
				;
			}
		}
		// ���j���[�Ɏg�p����T�u���ڂ̕����B
		// �I�t�Z�b�g�ʒu�������ݒ肷��B
		for (i=(0); i<(aaa.max_items); i++)
		{
			aaa.menu_item_my_obj[i].offset_x	= my_menu_resource[aaa.menu_offset+i].x_offset;
			aaa.menu_item_my_obj[i].offset_y	= my_menu_resource[aaa.menu_offset+i].y_offset;
		}
	//
		aaa.menu_brite			= (0);
	//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*�\���I���*/
	}
		cg.msg_time = (65536);	/* �� 18 �� */
}

#if 0
/*---------------------------------------------------------
	�X�e�[�W�I�����j���[�̊J�n�����B
	-------------------------------------------------------
---------------------------------------------------------*/
/*static*/global void stage_select_menu_start(void)
{
		psp_push_screen();
	{
		aaa.menu_type	= MENU_TYPE_00_SELECT_STAGE;
		aaa.menu_offset = MENU_RESOURCES_00_STAGE01;
		{
			#if 1
			/*
				�G�L�X�g���X�e�[�W���v���C������ŁA
				�u�G�L�X�g���X�e�[�W�A�v���N�e�B�X�v��
				�u�G���f�B���O�A�X�^�b�t���[���A�v���N�e�B�X�v��
				�I�ׂ�o�O������̂ŁA�C������B
			*/
			if ((6-1) < cg.game_continue_stage) 	{cg.game_continue_stage = (6-1);	/* (6-1) */}
			#endif
			aaa.selected_number = cg.game_continue_stage;
		}
		common_menu_initialize();
		cb.main_call_func = common_menu_work_MENU_STATE_00_FADE_IN_MENU;
	}
}
#endif

/*---------------------------------------------------------
	�|�[�Y���j���[�̊J�n�����B
	-------------------------------------------------------
	�ꎞ��~
	�ꎞ��~����
	�^�C�g���֖߂�
---------------------------------------------------------*/
extern void set_core_game_time_MAX(void);
/*static*/global MAIN_CALL_FUNC(pause_menu_start)
{
	{
		/*(�|�[�Y���j���[�̏ꍇ�̓��ꏈ��)	*/
		if (
				(7==cg.game_now_stage)	/* �X�^�b�t���[��1 */
			|| (10==cg.game_now_stage)	/* �X�^�b�t���[��2 */
		)
		{
			set_core_game_time_MAX();
			cb.main_call_func = cb.pause_out_call_func;
			return;/*(�����I�ɖ߂�)*/
		}
	}
//	else
	{
		/*(�|�[�Y���j���[�̏ꍇ�̓|�[�X�t�B���^�[��������)*/
		psp_pause_filter(); 	/* �|�[�Y�t�B���^�[ */
		psp_push_screen();
	//
		aaa.menu_type	= MENU_TYPE_01_PAUSE;
		aaa.menu_offset = MENU_RESOURCES_07_CONTINUE;
		aaa.selected_number 		= (0);// = MENU_ITEM_00_CONTINUE_GAME;/*"CONTINUE GAME"*/
		common_menu_initialize();
	//	fade_out_flag		= (0)/*set_fade_out_flag*/;/*�\���I���*/
	//	cb.main_call_func = pause_menu_local_workMENU_STATE_00_FADE_IN_MENU;
		cb.main_call_func = common_menu_work_MENU_STATE_00_FADE_IN_MENU;
	}
}


/*---------------------------------------------------------
	�R���e�B�j���[���邩���H���j���[�̊J�n�����B
	-------------------------------------------------------
---------------------------------------------------------*/

/*static*/global MAIN_CALL_FUNC(ask_continue_menu_start)
{
		render_continue();/*(�R���e�B�j���[���j���[�̏ꍇ�̌��o��������`��)*/
	//
		/*(�R���e�B�j���[���j���[�̏ꍇ�̓|�[�X�t�B���^�[��������)*/
		psp_pause_filter(); 	/* �|�[�Y�t�B���^�[ */
		psp_push_screen();
	{
		aaa.menu_type	= MENU_TYPE_02_ASK_CONTINUE;
		aaa.menu_offset = MENU_RESOURCES_11_YES;
		aaa.selected_number 		= (0);// = MENU_ITEM_00_CONTINUE_YES;/*"CONTINUE YES"*/
		common_menu_initialize();
	//	fade_out_flag		= (0)/*set_fade_out_flag*/;/*�\���I���*/
	//	cb.main_call_func = ask_continue_menu_00_FADE_IN_MENU;
		cb.main_call_func = common_menu_work_MENU_STATE_00_FADE_IN_MENU;
	}
}
