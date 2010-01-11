#ifndef _MY_FILE_H_
#define _MY_FILE_H_

extern/*static*/ char buffer_text_1_line[256];	/* parth text, 1 line buffer */ 	/* 走査する行の取得 */
extern/*static*/ char my_fopen_file_name[128];	/* fopenするファイル名 */
extern/*static*/ void *my_fopen(void/*const char *file_name*/ /*, const char *dummy*/);
extern/*static*/ int my_fgets(void/*char *buffer_name, int num, char *wfp*/);
extern/*static*/ void my_fclose(void/*void *wfp*/);

//1981937 1981965 1981853

extern/*static*/ int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte);
extern/*static*/ char *load_my_file_get_str(char *str, char *buffer, int *end_arg/*, const char kugiri_1_char*/);


//

#endif /* _MY_FILE_H_ */

