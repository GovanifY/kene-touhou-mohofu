
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	(�w��摜��������)�v���I�G���[�̏ꍇ�A
	���b�Z�[�W���o���ăQ�[���������I������B
---------------------------------------------------------*/
#include "kanji_system.h"
extern void vbl_draw_screen(void);/*support.c*/

/*---------------------------------------------------------
	(�L�[�R���t�B�O�ݒ蒆��)�v���I�G���[�ł�
	�L�[���͎��̂����������Ȃ�ꍇ�����邩������Ȃ��̂ŁA
	�Œ���̓��͂Ƃ��Đ�p�ȈՓ��͏������s���B
---------------------------------------------------------*/

static void hit_any_key(void)
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

global void psp_fatal_error(char *msg, ...)
{
	// [�G���[���b�Z�[�W��`��]
	char msgbuf[128];	/* 128==4*32 (psp�̍\����32�̔{���Ŏw��) */
	va_list argptr;
	//
	va_start(argptr, msg);
	vsprintf(msgbuf, msg, argptr);
	va_end(argptr);
	strcpy(my_font_text, msgbuf);/*(�G���[�������my_font_text�ɏ�������)*/
//	sw itch (errorlevel)/*int errorlevel,*/
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
	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
	hit_any_key();/*[�ȈՓ��͑҂�]*/
	//[�����I�����܂��B�ƕ`��]
	kanji_window_all_clear();			/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
	strcpy(my_font_text, "�v���I�G���[�Ȃ̂ŏI�����܂��B" "\\n");
	kanji_color(7);
	kanji_draw();
	vbl_draw_screen();	/* ��ʕ`��ƃL�[����(�{���� v-blanc �^�C�~���O��) */
	hit_any_key();/*[�ȈՓ��͑҂�]*/
	//[�����I������]
	sceKernelExitGame();	//if (errorlevel==ERR_FATAL) exit(1);/*exit(1)��psp�Ŏg���Ȃ��̂Œ���*/
	//
//	cb.ma in_call_func = NULL;
}
