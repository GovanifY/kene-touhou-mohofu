
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------

---------------------------------------------------------*/
#include "kanji_system.h"
extern void vbl_draw_screen(void);/*support.c*/

/*---------------------------------------------------------
	�L�[���͊֘A�̏���(�{���f�o�b�O�p)
	��̃L�[���͂ɓ������Ă��ǂ����A�L�[���͎��̂�
	���������Ȃ�ꍇ������̂ŁA�b��I�ɍŒ���̓��͂Ƃ���
	�c���Ă���B
---------------------------------------------------------*/

/*global*/static void hit_any_key(void)
{
	SceCtrlData cpad;
	/* �������܂ő҂� */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		if (0 == cpad.Buttons)
		{
			goto l_end1;
		//	break;
		}
	}
	l_end1:
	;
	/* �������܂ő҂� */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		/* Any Key */
		if (cpad.Buttons & (PSP_CTRL_SQUARE|PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE) )
		{
			goto l_end2;
		//	break;
		}
	}
	l_end2:
	;
}

/*---------------------------------------------------------
	�G���[����
---------------------------------------------------------*/

global void psp_fatal_error(char *msg, ...)/*int errorlevel,*/
{
	char msgbuf[128];	/* 128==4*32 (psp�̍\����32�̔{���Ŏw��) */
	va_list argptr;
	//
	va_start(argptr, msg);
	vsprintf(msgbuf, msg, argptr);
	va_end(argptr);
	strcpy(my_font_text, msgbuf);/*(�G���[�������my_font_text�ɏ�������)*/
//	cb.ma in_call_func = error_callback_init;
//	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
//	static void error_callback_init(void)
//	sw itch (errorlevel)
	{
	//ca se ERR_DEBUG:	if (debug)	{ fprintf(stdout,"DEBUG: %s\n",msgbuf); } break;
	//ca se ERR_INFO:		fprintf(stdout,"INFO: %s\n",msgbuf); break;
	//ca se ERR_FATAL: //	fprintf(stdout,"FATAL: %s\n",msgbuf);
	#if (1)/*(�����֘A�̏�����)*/
	kanji_init_standard();/*(�����֘A�̕W��������)*/
//	set_kanji_hide_line(ML_LINE_99_MAX);/*(�S�s�\������B)*/
	#endif
	#if (1)/*(�f�o�b�O)*/
	{
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_02_JIKI_SPELL_CARD);/* [��/���@�J�[�h�p�w�i]����Ӕw�ion */
		cg.msg_time 				= (65536);	/* �� 18 �� */
	//	strcpy(my_font_text, "�v���I�G���[�B" "\n");
	//	strcpy(my_font_text, msgbuf);
		kanji_color(7);
		kanji_draw();
	}
	#endif
	}
//	cb.ma in_call_func = error_01_callback_hit_any_key1;
	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
//	static void error_01_callback_hit_any_key1(void)
	hit_any_key();
//	cb.ma in_call_func = error_02_callback_draw_02;
//	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
//	global void error_02_callback_draw_02(void)
	kanji_window_all_clear();			/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
	strcpy(my_font_text, "�v���I�G���[�Ȃ̂ŏI�����܂��B" "\\n");
	kanji_color(7);
	kanji_draw();
//	cb.ma in_call_func = error_03_callback_hit_any_key2;
	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
//	static void error_03_callback_hit_any_key2(void)
	hit_any_key();
//	cb.ma in_call_func = error_04_callback_draw_03;
//	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
//	global void error_04_callback_draw_03(void)
	sceKernelExitGame();	//if (errorlevel==ERR_FATAL) exit(1);/*exit(1)��psp�Ŏg���Ȃ��̂Œ���*/
	//
//	cb.ma in_call_func = NULL;
//	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
}
