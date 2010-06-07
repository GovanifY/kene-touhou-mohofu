
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�t�H���g
	-------------------------------------------------------
	�Ȃ񂩃t�H���g����������VRAM��������Gu����j�ނ̂ŐF�X�p�~���B
	Gu�ɂȂ�΁A�g��k����F�ς�����ł��邵�B
---------------------------------------------------------*/

/* �X�e�[�^�X�p�l����p�t�H���g�A�{�X���[�^�[ */
#define FONT10WNAME "fonts/font01.png"
#define FONT10WCHARS " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmno" 	/*"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "*/
#define FONT10WW  8/*17*/
#define FONT10WH 10/* 8 17*/

/* ���j���[��ʂ̎菑�����A�����t�H���g�A�L�[�R���t�B�O */
#define FONT16RNAME "fonts/font03.png"
#define FONT16RCHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT16RW 16
#define FONT16RH 16
/* �菑�����A�����t�H���g�͎菑���� 32x32 �ō쐬���ăO���f�|���Ă���k�����Ď኱�C��(3�̎�)���� */

/* ���j���[��ʂ̎菑�����A�����t�H���g(�g������) */
#define FONT16WNAME "fonts/font05.png"
#define FONT16WCHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT16WW 16/*17*/
#define FONT16WH 16/*17*/

typedef struct
{
	char filename[256];
	SDL_Surface *fontimg;
	int w;
	int h;
	char char_order[256];
} FONT;
static FONT fonts[FONT_MAX];


/*---------------------------------------------------------
	�t�H���g
---------------------------------------------------------*/

global void font_init(void)
{
	strcpy(fonts[FONT10W].filename,FONT10WNAME);	fonts[FONT10W].w=FONT10WW;	fonts[FONT10W].h=FONT10WH;	strcpy(fonts[FONT10W].char_order,FONT10WCHARS);
	strcpy(fonts[FONT16R].filename,FONT16RNAME);	fonts[FONT16R].w=FONT16RW;	fonts[FONT16R].h=FONT16RH;	strcpy(fonts[FONT16R].char_order,FONT16RCHARS);
	strcpy(fonts[FONT16W].filename,FONT16WNAME);	fonts[FONT16W].w=FONT16WW;	fonts[FONT16W].h=FONT16WH;	strcpy(fonts[FONT16W].char_order,FONT16WCHARS);
	{
		int i;
		for (i=0; i<FONT_MAX; i++)
		{
			fonts[i].fontimg = load_chache_bmp(fonts[i].filename);//, 0, 1);
			SDL_SetColorKey(fonts[i].fontimg, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
		}
	}

}


/*---------------------------------------------------------
	�C�ӂ̃T�[�t�F�C�X�ɁA������������_�����O
---------------------------------------------------------*/

static void font_render_surface_xy(SDL_Surface *txt_image_surface, char *text, int font_number, int x_offset, int y_offset)
{
	SDL_Rect s;
	SDL_Rect d;
	s.w = fonts[font_number].w; d.w = (s.w);	/*fonts[font_number].w*/
	s.h = fonts[font_number].h; d.h = (s.h);	/*fonts[font_number].h*/
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
		s.x = j*(s.w);				/*fonts[font_number].w*/
		s.y = 0;
		d.x = x_offset+i*(s.w); 	/*fonts[font_number].w*/
		d.y = y_offset; 			/*0*/
		SDL_BlitSurface(fonts[font_number].fontimg, &s, txt_image_surface, &d);
	}
}


/*---------------------------------------------------------
	�T�[�t�F�C�X�����Ȃ��ŁA���ډ�ʂɕ\��
---------------------------------------------------------*/

global void font_print_screen_xy(char *text, int font_number, int x, int y)
{
//	SDL_SetColorKey(fonts[font_number].fontimg/*screen*/, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	font_render_surface_xy(sdl_screen[SDL_00_VIEW_SCREEN], text, font_number, x, y);
}


/*---------------------------------------------------------
	�V�K�T�[�t�F�C�X���쐬���A������������_�����O
---------------------------------------------------------*/

global SDL_Surface *font_render(char *text, int font_number)
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
		sdl_screen[SDL_00_VIEW_SCREEN]->format->BitsPerPixel,
		sdl_screen[SDL_00_VIEW_SCREEN]->format->Rmask,
		sdl_screen[SDL_00_VIEW_SCREEN]->format->Gmask,
		sdl_screen[SDL_00_VIEW_SCREEN]->format->Bmask,
		sdl_screen[SDL_00_VIEW_SCREEN]->format->Amask);
	SDL_FillRect(txt_image_surface, NULL, 0/*SD L_MapRGB(txt_image_surface->format,0,0,0)*/);
	SDL_SetColorKey(txt_image_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	font_render_surface_xy(txt_image_surface, text, font_number, 0, 0);
	return (txt_image_surface);
}
