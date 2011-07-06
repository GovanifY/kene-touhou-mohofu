
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͊����\���Ɋ֌W������ꍇ�̂݁A�C���N���[�h���܂��B
	"game_main.h"���A��ŃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _KANJI_SYSTEM_H_
#define _KANJI_SYSTEM_H_

/* �O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`���B */
extern int print_kanji000(const char *str, int color_type, int wait);

/*
	print_kanji000 �̓���@�\(color_type�Ɏw��)
	STR_CODE_NO_ENTER	���s���Ȃ�
*/
#define STR_CODE_NO_ENTER (0x10)


/*---------------------------------------------------------

---------------------------------------------------------*/

/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
/* ��ʏ��� */
extern void kanji_window_clear(void);
/* �w��s��1�s���� */
extern void kanji_window_clear_line(unsigned int line_num);

/* �J�[�\�����z�[���|�W�V�����ֈړ� */
extern void home_cursor(void);

/* �J�[�\�����w��ʒu�ֈړ� */
extern void set_cursor(unsigned int set_x, unsigned int set_y);

/* �O�����烁�b�Z�[�W�E�B���h�E�Ɋ�����`�悷��ꍇ�̃��Z�b�g */
//extern void sc ript_message_window_clear(void);


#endif /* _KANJI_SYSTEM_H_ */
