#ifndef _FONT_H_
#define _FONT_H_

#include "support.h"

enum FONTS
{
	FONT01,
	FONT02,
	FONT03,
	FONT04,
	FONT05,
	FONT06,
//	FONT07,
	FONT_MAX
};

typedef struct
{
	char filename[256];
	SDL_Surface *fontimg;
	int w;
	int h;
	char char_order[256];
} FONT;

extern void font_init(void);
extern void font07_init(void);

/* サーフェイスを作らないで、直接画面に表示 */
extern void font_print_screen_xy(char *text, int fontnr, int x, int y);

/* 新規サーフェイスを作成し、文字列をレンダリング */
extern SDL_Surface *font_render(char *text,int fontnr);

/* */
extern void font_print(char *text, int fontnr, int x, int y);

#endif /* _FONT_H_ */
