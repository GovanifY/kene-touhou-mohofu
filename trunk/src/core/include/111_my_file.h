
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͋��ʃt�@�C���@�\���g�p����ꍇ�̂݁A�C���N���[�h���܂��B
	"game_main.h"���A��ŃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _MY_FILE_H_
#define _MY_FILE_H_

extern char buffer_text_1_line[];	/* 256 parth text, 1 line buffer */ 	/* ��������s�̎擾 */
extern char my_fopen_file_name[];	/* 256 fopen����t�@�C���� (128) */
extern void *my_fopen(void);			/*const char *file_name*/ /*, const char *dummy*/
extern int my_file_fgets(void); 		/*char *buffer_name, int num, char *wfp*/
extern void my_fclose(void);			/*void *wfp*/

extern int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte);
extern char *load_my_file_get_str(char *str, char *buffer, int *end_arg);/*, const char kugiri_1_char*/

/* �w���p�[�֐� */

extern int my_file_now_size(void);

#endif /* _MY_FILE_H_ */
