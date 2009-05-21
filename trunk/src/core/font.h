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

/* �T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\�� */
extern void font_print_screen_xy(char *text, int fontnr, int x, int y);

/* �V�K�T�[�t�F�C�X���쐬���A������������_�����O */
extern SDL_Surface *font_render(char *text,int fontnr);

/* */
extern void font_print(char *text, int fontnr, int x, int y);

#endif /* _FONT_H_ */
