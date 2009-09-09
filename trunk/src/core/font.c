
/*---------------------------------------------------------
	�t�H���g
---------------------------------------------------------*/

#include "game_main.h"

/* �X�e�[�^�X�p�l����p�t�H���g�A�{�X���[�^�[ */
#define FONT01NAME "fonts/font01.png"
#define FONT01CHARS " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklm"/*"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "*/
#define FONT01W  8/*17*/
#define FONT01H 10/* 8 17*/

/* ���O���͂̃L�[�{�[�h�A�ʂɎ��A�t�H���g */
#define FONT02NAME "fonts/font02.png"
#define FONT02CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT02W 16
#define FONT02H 16

/* ���j���[��ʂ̎菑�����A�����t�H���g�A�L�[�R���t�B�O */
#define FONT03NAME "fonts/font03.png"
#define FONT03CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT03W 16
#define FONT03H 16
/* �菑�����A�����t�H���g�͎菑���� 32x32 �ō쐬���ăO���f�|���Ă���k�����Ď኱�C��(3�̎�)���� */

/* �_����ۂ��t�H���g�A�啶�����΁A�����������F�A�H�L�������F�Aascii�R�[�h(0x20-0x7e)���ɕ��ׂȂ����� */
#define FONT04NAME "fonts/font04.png"
#define FONT04CHARS " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
//efine FONT04CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@(){},.-;:_#*+?=!\"�$%&/�����ܵ^�'`��\\<>| "
#define FONT04W 16 /*18*/
#define FONT04H 20 /*24*/ /*37*/

/* ���j���[��ʂ̎菑�����A�����t�H���g(�g������) */
#define FONT05NAME "fonts/font05.png"
#define FONT05CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT05W 16/*17*/
#define FONT05H 16/*17*/

/* �S�V�b�N�̂��ۂ��t�H���g�A�n�C�X�R�A�^�C�g���A�Q�[���I�[�o�[ */
#define FONT06NAME "fonts/font06.png"
#define FONT06CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ,-./0123456789:"/*"<>?'()@!# "*/
#define FONT06W 16
#define FONT06H 20/*16*/

static FONT fonts[FONT_MAX];

/*---------------------------------------------------------
	�t�H���g
---------------------------------------------------------*/

void font_init(void)
{
	strcpy(fonts[FONT01].filename,FONT01NAME);	fonts[FONT01].w=FONT01W;	fonts[FONT01].h=FONT01H;	strcpy(fonts[FONT01].char_order,FONT01CHARS);
	strcpy(fonts[FONT02].filename,FONT02NAME);	fonts[FONT02].w=FONT02W;	fonts[FONT02].h=FONT02H;	strcpy(fonts[FONT02].char_order,FONT02CHARS);
	strcpy(fonts[FONT03].filename,FONT03NAME);	fonts[FONT03].w=FONT03W;	fonts[FONT03].h=FONT03H;	strcpy(fonts[FONT03].char_order,FONT03CHARS);
	strcpy(fonts[FONT04].filename,FONT04NAME);	fonts[FONT04].w=FONT04W;	fonts[FONT04].h=FONT04H;	strcpy(fonts[FONT04].char_order,FONT04CHARS);
	strcpy(fonts[FONT05].filename,FONT05NAME);	fonts[FONT05].w=FONT05W;	fonts[FONT05].h=FONT05H;	strcpy(fonts[FONT05].char_order,FONT05CHARS);
	strcpy(fonts[FONT06].filename,FONT06NAME);	fonts[FONT06].w=FONT06W;	fonts[FONT06].h=FONT06H;	strcpy(fonts[FONT06].char_order,FONT06CHARS);
//	strcpy(fonts[FONT07].filename,FONT07NAME);	fonts[FONT07].w=FONT07W;	fonts[FONT07].h=FONT07H;	strcpy(fonts[FONT07].char_order,FONT07CHARS);
	{
		int i;
		for (i=0; i<FONT_MAX; i++)
		{
			fonts[i].fontimg = loadbmp0(fonts[i].filename, 0, 1);
			SDL_SetColorKey(fonts[i].fontimg,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
		}
	}
//	font07_init();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*
SDL_Surface *font_render(char *text,int font_number)
{
	SDL_Surface *txt_image_surface;
	SDL_Rect s,d;
	unsigned int i,j;

	txt_image_surface=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,strlen(text)*fonts[font_number].w,fonts[font_number].h,
		sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
		sdl_screen[SDL_00_SCREEN]->format->Rmask,
		sdl_screen[SDL_00_SCREEN]->format->Gmask,
		sdl_screen[SDL_00_SCREEN]->format->Bmask,
		sdl_screen[SDL_00_SCREEN]->format->Amask);
	SDL_SetColorKey(txt_image_surface,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);

	for (i=0;i<strlen(text);i++) {

		for (j=0;j<strlen(fonts[font_number].char_order);j++)
			if (text[i]==fonts[font_number].char_order[j]) break;

		s.w=fonts[font_number].w;
		s.h=fonts[font_number].h;
		s.x=j*fonts[font_number].w;
		s.y=0;

		d.w=fonts[font_number].w;
		d.h=fonts[font_number].h;
		d.x=i*fonts[font_number].w;
		d.y=0;

		SDL_BlitSurface(fonts[font_number].fontimg,&s,txt_image_surface,&d);
	}
	return (txt_image_surface);
}
*/


/*---------------------------------------------------------
	�C�ӂ̃T�[�t�F�C�X�ɁA������������_�����O
---------------------------------------------------------*/

static void font_render_surface_xy(SDL_Surface *txt_image_surface, char *text, int font_number, int x_offset, int y_offset)
{
	SDL_Rect s;
	SDL_Rect d;
	s.w = fonts[font_number].w; d.w = (s.w)/*fonts[font_number].w*/;
	s.h = fonts[font_number].h; d.h = (s.h)/*fonts[font_number].h*/;
	unsigned int i;
	unsigned int j;
	for (i=0; i<strlen(text); i++)
	{
		for (j=0; j<strlen(fonts[font_number].char_order); j++)
		{
			if (text[i]==fonts[font_number].char_order[j])
			{
				break;
			}
		}
		s.x = j*(s.w)/*fonts[font_number].w*/;
		s.y = 0;
		d.x = x_offset+i*(s.w)/*fonts[font_number].w*/;
		d.y = y_offset/*0*/;
		SDL_BlitSurface(fonts[font_number].fontimg, &s, txt_image_surface, &d);
	}
}

/*---------------------------------------------------------
	�T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\��
---------------------------------------------------------*/

void font_print_screen_xy(char *text, int font_number, int x, int y)
{
//	SDL_SetColorKey(fonts[font_number].fontimg/*screen*/,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	font_render_surface_xy(sdl_screen[SDL_00_SCREEN], text, font_number, x, y);
}

/*---------------------------------------------------------
	�V�K�T�[�t�F�C�X���쐬���A������������_�����O
---------------------------------------------------------*/

SDL_Surface *font_render(char *text, int font_number)
{
	SDL_Surface *txt_image_surface;
	txt_image_surface = SDL_CreateRGBSurface(
		/* ���̕�(�n���O�A�b�v����)�o�O����񂾂��ǁA�ǂ��킩��Ȃ��B */
		#if 0
		/* VRAM�Ɏ�������������͂������A���ۂ͒x���Ȃ����肷��B�n�߂��������āA���񂾂�x���Ȃ�(���t���[�����Ƃ�)�ς����Ȃ����x���Ȃ�B */
		/* PSP�p��SDL��VRAM�Ǘ� http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.c �ł� */
		/* ���X�g�����ŊǗ����Ă邪�A���̃��X�g�����̂����ȋC������Brealloc(); ���̂����������B(???) */
		/* sceGeEdramGetAddr(); �� sceGeEdramGetSize(); �̕ӂ́A���m�ɃT�C�Y���擾�ł��Ȃ��C������B(???) */
		SDL_SRCCOLORKEY|SDL_HWSURFACE,	/* VRAM�֊m�ۂ���(VRAM������Ȃ��ꍇ�́ASDL�̓��C���������֎��) */
		#else
		/* ���󂱂����̕����܂����ۂ��B(???) */
		/* ���Ԃ񃁃����[���[�N����B(gameover.c SDL_FreeSurface(go_surface1); SDL_FreeSurface(go_surface2); �Ƃ���) */
		SDL_SRCCOLORKEY|SDL_SWSURFACE/*SDL_HWSURFACE*/,/* ���C���������֊m�ۂ��� */
		#endif
		strlen(text)*fonts[font_number].w,
		fonts[font_number].h,
		sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
		sdl_screen[SDL_00_SCREEN]->format->Rmask,
		sdl_screen[SDL_00_SCREEN]->format->Gmask,
		sdl_screen[SDL_00_SCREEN]->format->Bmask,
		sdl_screen[SDL_00_SCREEN]->format->Amask);
	SDL_SetColorKey(txt_image_surface,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	font_render_surface_xy(txt_image_surface, text, font_number, 0, 0);
	return (txt_image_surface);
}
#if 0
/*---------------------------------------------------------
	�t�H���g
---------------------------------------------------------*/

void font_print(char *text, int font_number, int x, int y)
{
	SDL_Surface *text_surface;
	SDL_Rect r;
	text_surface = font_render(text, font_number);
	r.x = x;
	r.y = y;
	r.w = text_surface->w;
	r.h = text_surface->h;
	SDL_BlitSurface(text_surface, NULL, sdl_screen[SDL_00_SCREEN], &r);
	SDL_FreeSurface(text_surface);
}
#endif
