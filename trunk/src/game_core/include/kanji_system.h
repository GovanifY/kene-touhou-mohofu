
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͊����\���Ɋ֌W������ꍇ�̂݁A�C���N���[�h���܂��B
	"game_main.h"���A��ŃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _KANJI_SYSTEM_H_
#define _KANJI_SYSTEM_H_

/* �O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`���B */

extern void kanji_color(unsigned int set_color_type);
extern void kanji_wait(unsigned int set_wait);

/* �O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`���B */
extern int kanji_draw(void);

/*(���b�Z�[�W�E�B���h�E�� r35font.png ���g���ĕ`���B)*/
extern void kanji_ascii_font16_draw(void);

/*
	kanji_draw �̓���@�\(color_type�Ɏw��)
	STR_CODE_NO_ENTER	���s���Ȃ�
*/
#define STR_CODE_NO_ENTER (0x10)
//
#define KANJI_FONT_08_HARF_WIDTH	(8) 							/* ���p�����̕� */
#define KANJI_FONT_16_WIDTH 		(KANJI_FONT_08_HARF_WIDTH*2)	/* �S�p�����̕� */
#define KANJI_FONT_16_HEIGHT_P0 	(16)							/* �S�p�����̏��̂̍��� */
#define KANJI_FONT_18_HEIGHT_P2 	(KANJI_FONT_16_HEIGHT_P0+2) 	/* �S�p�����̍s�̍��� */


/*---------------------------------------------------------

---------------------------------------------------------*/

/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
/* ������ʂ�S�s�����B */
extern void kanji_window_clear(void);
/* �w��s��1�s�����B */
extern void kanji_window_clear_line(unsigned int line_num);

/* �����J�[�\�����z�[���|�W�V�����ֈړ� */
extern void kanji_cursor_move_home_position(void);

/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ� */
extern void kanji_window_all_clear(void);

/* �J�[�\�����w��ʒu�ֈړ� */
/*
	x���W�̒P�ʂ�[pixel]
	y���W�̒P�ʂ�[pixel]
	y���W�� KANJI_FONT_18_HEIGHT_P2 �̔{���Ŏw�肵�Ȃ��ꍇ�A�ςȕ\���ɂȂ�B
	�b��d�l�Ȃ̂ŁA�ς��\����B
*/
// ���z�o�b�t�@�̃����_�����O�ʒu���w��B
extern void set_kanji_xy(unsigned int set_x_offset_pixel, unsigned int set_y_offset_pixel);
// ��ʕ\���̊�ʒu���z���w��B
extern void set_kanji_origin_xy(unsigned int set_x_offset_pixel, unsigned int set_y_offset_pixel);
extern void set_kanji_origin_kankaku(unsigned int set_y_offset_pixel);
/* �O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`�悷��ꍇ�̃��Z�b�g */
//extern void sc ript_message_window_clear(void);

#if 1
/*---------------------------------------------------------
	�Ɨ����C���֘A
---------------------------------------------------------*/
enum
{
	ML_OFF	= 0x00000000,	/* ��\����Ԃ𖾎�����B */
	ML_ON	= 0x00ffffff,	/*	 �\����Ԃ𖾎�����B */
};
enum
{					// ���{��ŃR�����g�����Ă�ƍ�������̂ŁA�萔���R�����g�ɍ��킹��B
	ML_LINE_01 = 0, // 0 == 1�s��(0�s�ڂ͖����̂Œ���)
	ML_LINE_02, 	// 1 == 2�s��
	ML_LINE_03, 	// 2 == 3�s��
	ML_LINE_04, 	// 3 == 4�s��
	ML_LINE_05, 	// 4 == 5�s��
	ML_LINE_06, 	// 5 == 6�s��
	ML_LINE_07, 	// 6 == 7�s��
	ML_LINE_08, 	// 7 == 8�s��
	ML_LINE_99_MAX/*(2^n�̍ő�s)*/
};

enum
{
	ML_HAIKEI_m1_OFF		= -1,
	ML_HAIKEI_00_MISIYOU	= 0,	/*[��/���g�p]*/
	ML_HAIKEI_01_BOSS_SPELL_CARD,	/*[��/�{�X�J�[�h�p�w�i]*/
	ML_HAIKEI_02_JIKI_SPELL_CARD,	/*[��/���@�J�[�h�p�w�i]*/
	ML_HAIKEI_03_MESSAGE,			/*[��/��b�p�w�i]*/
	ML_HAIKEI_04_MAX/*(�ő�)*/
};

typedef struct
{
	int timer;		// 0�Ŕ�\���B1�t���[���\�����閈�Ƀf�N�������g�B(�����Œl��1���炷)�B
	int haikei; 	// �����̔w�i�̎�ށB
	int x;			/* x�ʒu[pixel] */
	int y;			/* y�ʒu[pixel] */
} ML_FONT;
//	char *str;		/* ������(���̂ւ̃|�C���^�̂ݕێ�) */
//	void *render;	/* �����_�����O�ʒu */

extern ML_FONT ml_font[ML_LINE_99_MAX];

//	kanji_system_hide_message(ML_LINE_02);/*(2�s�ڈȉ����\���ɂ���B)*/
extern void set_kanji_hide_line(unsigned int line_num);
#endif

/*---------------------------------------------------------
	�X�N���v�g���p�A�W��������
---------------------------------------------------------*/

extern void kanji_init_standard(void);/*(�����֘A�̕W��������)*/

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void dec_print_format(unsigned int num, int size, char *my_str);

#endif /* _KANJI_SYSTEM_H_ */
