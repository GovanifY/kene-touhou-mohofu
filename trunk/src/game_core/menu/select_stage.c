
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


static unsigned int 	aaa_max_items;			/* �I�v�V�����̍��ڐ� */
static int				aaa_selected_number;	/* ���݃��j���[��őI������Ă��鍀�ڔԍ� */
static int				aaa_menu_brite; 		/* ���j���[�̖��邳(���l) */


/*---------------------------------------------------------

---------------------------------------------------------*/

//	#define FPS_MENU_FACTOR8	(8*fps_factor)
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	MENU_RESOURCES_00_STAGE01 = 0,
	MENU_RESOURCES_01_STAGE02,
	MENU_RESOURCES_02_STAGE03,
	MENU_RESOURCES_03_STAGE04,
	MENU_RESOURCES_04_STAGE05,
	MENU_RESOURCES_05_STAGE06,
//	MR_06_STOP,//----------------
	MENU_RESOURCES_06_MAX
};


/*---------------------------------------------------------

---------------------------------------------------------*/
static void stage_select_menu_draw_all(void)
{
	/* �`�� */
	psp_pop_screen();
	{
		unsigned int yyy18;
		yyy18 = (0);/* Y�ʒu */
		unsigned int yyy21;
		yyy21 = (113);/* Y�ʒu */
		kanji_cursor_move_home_position();			/* �����J�[�\�����z�[���|�W�V�����ֈړ� */
		cg.msg_time = (65536);	/* �� 18 �� */
		unsigned int i;
		for (i=0; i<aaa_max_items; i++)
		{
			unsigned int text_color;
			text_color = (4);/*(���ΐF)*/
			//
			ml_font[i].x		= (256);//(aaa_menu_item_my_obj[i].offset_x);
			ml_font[i].y		= (yyy21);//(aaa_menu_item_my_obj[i].offset_y);
			if (((signed)i)==aaa_selected_number)	/* �I�����ꂽ */
			{
				text_color = (12);/*(�ΐF)*/
				ml_font[i].x += (2);	/* �I�t�Z�b�g�� 2[pixel] �ɂ��� */
				ml_font[i].y += (2);	/* �I�t�Z�b�g�� 2[pixel] �ɂ��� */
			}
			ml_font[i].timer	= ML_ON;
			//
			kanji_window_clear_line(i); 	/* �����E�B���h�E��n�s��(==0)�̓��e�������B */
			set_kanji_xy( (0), (yyy18) );
			strcpy(my_font_text, (char *)"STAGE 0 0000000000");
			my_font_text[6] = ('1'+i);
			kanji_color(text_color);
			kanji_draw();
			//
			yyy18 += (18);
			yyy21 += (21);
		}
	}
}


/*---------------------------------------------------------
	�I�������ƈړ������B
---------------------------------------------------------*/
extern void player_continue_value(void);
static MAIN_CALL_FUNC(common_menu_work_MENU_STATE_03_FININSH)
{
//	SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, 255);
	psp_pop_screen();
	//
	if ((0)>aaa_selected_number)/*(CANCEL�̏ꍇ)*/
	{
		cb.main_call_func = rank_select_menu_start; 	/* �I�����j���[�֖߂� */
	}
	else/*(OK�̏ꍇ)*/
	{
		cg.game_continue_stage = aaa_selected_number;
		/* �v���N�e�B�X �Q�[���J�n */
		cb.main_call_func = stage_first_init;			/* �Q�[���J�n */
	}
}


/*---------------------------------------------------------
	���񂾂�Â�����B�Â��Ȃ����玟�ցB
---------------------------------------------------------*/

static MAIN_CALL_FUNC(common_menu_work_MENU_STATE_02_FADE_OUT)
{
	aaa_menu_brite -= FPS_MENU_FACTOR8;
	if (0 >= (aaa_menu_brite) )
	{
		aaa_menu_brite	= (0);
		cb.main_call_func = common_menu_work_MENU_STATE_03_FININSH; /* */
	}
	stage_select_menu_draw_all();
}
/*---------------------------------------------------------
	���j���[�L�[���͎󂯕t���B�m�肵���玟�ցB
---------------------------------------------------------*/

static MAIN_CALL_FUNC(stage_select_menu_work_MENU_STATE_01_WORK_MENU)
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
			if (aaa_selected_number == aaa_max_items-1)
					{	aaa_selected_number = 0;	}
			else	{	aaa_selected_number++;		}
		}
		else
		if (psp_pad.pad_data & PSP_KEY_UP)
		{
			if (0 == aaa_selected_number)
					{	aaa_selected_number = aaa_max_items-1;	}
			else	{	aaa_selected_number--;		}
		}
		#else/*(r35-)*/
		/*(�㉺���������ꍇ�A�ő�ŏ��I��������B���j���[�㉺�͕K����~)*/
		if (psp_pad.pad_data & PSP_KEY_DOWN)
		{	aaa_selected_number++;		aaa_selected_number = psp_min(aaa_selected_number, (aaa_max_items-1) ); 	}	/* ���j���[���Œ�~ */
		else
		if (psp_pad.pad_data & PSP_KEY_UP)
		{	aaa_selected_number--;		aaa_selected_number = psp_max(aaa_selected_number, (0) );					}	/* ���j���[��Œ�~ */
		#endif
		/*(OK�{�^��)*/
		if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
		{
			voice_play_menu(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_MENU01);
			cb.main_call_func = common_menu_work_MENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
		}
		else
		/*(CANCEL�{�^��)*/
		if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
		{
			voice_play_menu(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* ���@���ɉ��́A�Ȃ�ׂ��d�˂Ȃ� */
			aaa_selected_number = (-1);/*(�����Ȃ�Ȃ�ł�����)*/
			cb.main_call_func = common_menu_work_MENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
		}
	}
	stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	���񂾂񖾂邭����B���邭�Ȃ����玟�ցB
---------------------------------------------------------*/

static MAIN_CALL_FUNC(common_menu_work_MENU_STATE_00_FADE_IN_MENU)
{
	aaa_menu_brite += FPS_MENU_FACTOR8;
	if ((255) <= (aaa_menu_brite) )
	{
		aaa_menu_brite = (255);
		cb.main_call_func = stage_select_menu_work_MENU_STATE_01_WORK_MENU; /* */
	}
	stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	���j���[�������B
---------------------------------------------------------*/



/*---------------------------------------------------------
	�X�e�[�W�I�����j���[�̊J�n�����B
	-------------------------------------------------------
---------------------------------------------------------*/
/*static*/global MAIN_CALL_FUNC(stage_select_menu_start)
{
	psp_push_screen();
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
		aaa_selected_number = cg.game_continue_stage;
	}
	// ���j���[�Ɏg�p����T�u���ڂ̐��𒲂ׂ�B
	{
		aaa_max_items		= (0);
		{
	loop:
			/* �Z���N�g�X�e�[�W���j���[�̏ꍇ */
			/* ������ꂽ�����܂ŁB */
			if (aaa_max_items>(option_config[OPTION_CONFIG_07_OPEN]&0x07))	{	goto loop_quit;}
			aaa_max_items++;
			/* (�I���܂Œ��ׂ�) */
			/*while*/if (MENU_RESOURCES_06_MAX ==  /*aaa_menu_offset+*/aaa_max_items )		{	goto loop_quit;}
			goto loop;
	loop_quit:
			;
		}
	}
	// ���j���[�Ɏg�p����T�u���ڂ̕����B
	aaa_menu_brite		= (0);
//	fade_out_flag		= (0)/*set_fade_out_flag*/;/*�\���I���*/
	cg.msg_time 		= (65536);	/* �� 18 �� */
	cb.main_call_func = common_menu_work_MENU_STATE_00_FADE_IN_MENU;
}

