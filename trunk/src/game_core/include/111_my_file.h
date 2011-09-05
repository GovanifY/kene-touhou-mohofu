
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͋��ʃt�@�C���@�\���g�p����ꍇ�̂݁A�C���N���[�h���܂��B
	"game_main.h"���A��ŃC���N���[�h���܂��B
---------------------------------------------------------*/

#ifndef _MY_FILE_H_
#define _MY_FILE_H_

#define MY_FILE_256_LINE_BUFFER_LENGTH		(256)
#define MY_FILE_256_FILE_NAME_LENGTH		(256)

extern char my_file_line_buffer256[MY_FILE_256_LINE_BUFFER_LENGTH]; 	/* 256 parth text, 1 line buffer */ 	/* ��������s�̎擾 */
extern char my_file_common_name[MY_FILE_256_FILE_NAME_LENGTH];			/* 256 fopen����t�@�C���� (128) */
extern void *my_file_fopen(void);			/* const char *file_name */ /*, const char *dummy */
extern int my_file_fgets(void); 			/* char *buffer_name, int num, char *wfp */
extern void my_file_fclose(void);			/* void *wfp */

extern int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte);
extern char *load_my_file_get_str(char *str, char *buffer, int *end_arg);/* , const char kugiri_1_char */


/*---------------------------------------------------------
	�w���p�[�֐�
---------------------------------------------------------*/

extern int my_file_now_size(void);


/*---------------------------------------------------------
	1�s�P�ʓǂݍ��݃T�|�[�g�p�A
	�w���p�[�}�N��
---------------------------------------------------------*/

/*---------------------------------------------------------
	�ǂݍ��݃��[�v�A��������
	-------------------------------------------------------
	�ǂݍ��݃��[�v�A1�s�ǂݍ���
	-------------------------------------------------------
	�d�l:
		';'�Ŏn�܂�s�̓R�����g�s�Ȃ̂ŁA���Ȃ��Ŏ��̍s�܂Ŕ�΂��܂��B
		�󔒂�TAB�͏�������܂��B
		���s�݂̂̍s����������܂��B
		����� char *ch; ����`����܂��Bch�͓ǂݍ��񂾍s�̐擪���w���܂��B
---------------------------------------------------------*/
#define MY_FILE_LOOP_BEGIN \
	{loop:;\
		if (/*NULL*/0 != my_file_fgets()) \
		{ \
			char *ch;					/* �����ʒu */ \
			ch = my_file_line_buffer256; \
			/* skiped lines. */ \
			{my_isspace:; \
				if (' '>=(*ch)) \
				{ \
					ch++; \
					if (0x0a==(*ch)) \
					{	MY_FILE_LOOP_CONTINUE; }	/* skiped null line. */ \
					else \
					{	goto my_isspace;	} \
				} \
			}

/*---------------------------------------------------------
	�ǂݍ��݃��[�v�A������x�ǂݍ��݂�����s
---------------------------------------------------------*/
#define MY_FILE_LOOP_CONTINUE \
		goto loop;/*continue;*/

/*---------------------------------------------------------
	�ǂݍ��݃��[�v�A�����܂�
---------------------------------------------------------*/
#define MY_FILE_LOOP_END \
		} \
	}

#endif /* _MY_FILE_H_ */

