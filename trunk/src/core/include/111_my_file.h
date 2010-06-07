
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは共通ファイル機能を使用する場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _MY_FILE_H_
#define _MY_FILE_H_

extern char buffer_text_1_line[];	/* 256 parth text, 1 line buffer */ 	/* 走査する行の取得 */
extern char my_fopen_file_name[];	/* 256 fopenするファイル名 (128) */
extern void *my_fopen(void);			/*const char *file_name*/ /*, const char *dummy*/
extern int my_file_fgets(void); 		/*char *buffer_name, int num, char *wfp*/
extern void my_fclose(void);			/*void *wfp*/

extern int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte);
extern char *load_my_file_get_str(char *str, char *buffer, int *end_arg);/*, const char kugiri_1_char*/

/* ヘルパー関数 */

extern int my_file_now_size(void);

#endif /* _MY_FILE_H_ */
