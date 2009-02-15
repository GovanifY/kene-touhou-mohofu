#ifndef _FONT_H_
#define _FONT_H_
#include <SDL/SDL.h>
#include <string.h>

#include "support.h"

enum FONTS { FONT01, FONT02, FONT03, FONT04, FONT05, FONT06, FONT07, LAST_FONT };


typedef struct
{
	char filename[256];
	SDL_Surface *fontimg;
	int w;
	int h;
	char charorder[256];
} FONT;

extern void font_init(void);

/* サーフェイスを作らないで、直接画面に表示 */
extern void font_print_screen_xy(char *text, int fontnr, int x, int y);

/* 新規サーフェイスを作成し、文字列をレンダリング */
extern SDL_Surface *font_render(char *text,int fontnr);

/* */
extern void font_print(char *text, int fontnr, int x, int y);

#endif
