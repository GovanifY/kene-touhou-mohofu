
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは共通ファイル機能を使用する場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _MY_FILE_H_
#define _MY_FILE_H_

#define MY_FILE_256_LINE_BUFFER_LENGTH		(256)
#define MY_FILE_256_FILE_NAME_LENGTH		(256)

extern char my_file_line_buffer256[MY_FILE_256_LINE_BUFFER_LENGTH]; 	/* 256 parth text, 1 line buffer */ 	/* 走査する行の取得 */
extern char my_file_common_name[MY_FILE_256_FILE_NAME_LENGTH];			/* 256 fopenするファイル名 (128) */
extern void *my_file_fopen(void);			/* const char *file_name */ /*, const char *dummy */
extern int my_file_fgets(void); 			/* char *buffer_name, int num, char *wfp */
extern void my_file_fclose(void);			/* void *wfp */

extern int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte);
extern char *load_my_file_get_str(char *str, char *buffer, int *end_arg);/* , const char kugiri_1_char */


/*---------------------------------------------------------
	ヘルパー関数
---------------------------------------------------------*/

extern int my_file_now_size(void);


/*---------------------------------------------------------
	1行単位読み込みサポート用、
	ヘルパーマクロ
---------------------------------------------------------*/

/*---------------------------------------------------------
	読み込みループ、ここから
	-------------------------------------------------------
	読み込みループ、1行読み込み
	-------------------------------------------------------
	仕様:
		';'で始まる行はコメント行なので、やらないで次の行まで飛ばします。
		空白やTABは除去されます。
		改行のみの行も除去されます。
		勝手に char *ch; が定義されます。chは読み込んだ行の先頭を指します。
---------------------------------------------------------*/
#define MY_FILE_LOOP_BEGIN \
	{loop:;\
		if (/*NULL*/0 != my_file_fgets()) \
		{ \
			char *ch;					/* 走査位置 */ \
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
	読み込みループ、もう一度読み込みから実行
---------------------------------------------------------*/
#define MY_FILE_LOOP_CONTINUE \
		goto loop;/*continue;*/

/*---------------------------------------------------------
	読み込みループ、ここまで
---------------------------------------------------------*/
#define MY_FILE_LOOP_END \
		} \
	}

#endif /* _MY_FILE_H_ */

