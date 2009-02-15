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

/* �T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\�� */
extern void font_print_screen_xy(char *text, int fontnr, int x, int y);

/* �V�K�T�[�t�F�C�X���쐬���A������������_�����O */
extern SDL_Surface *font_render(char *text,int fontnr);

/* */
extern void font_print(char *text, int fontnr, int x, int y);

#endif
