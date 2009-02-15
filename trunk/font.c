#include "font.h"

static FONT fonts[LAST_FONT];

extern SDL_Surface *screen;

#define FONT01NAME "font01.png"
#define FONT01CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "
#define FONT01W 17
#define FONT01H 17

#define FONT02NAME "font02.png"
#define FONT02CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT02W 16
#define FONT02H 16

#define FONT03NAME "font03.png"
#define FONT03CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT03W 16
#define FONT03H 16

#define FONT04NAME "font04.png"
#define FONT04CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@(){},.-;:_#*+?=!\"§$%&/φδόΦΔά΅^°'`΄ί\\<>| "
#define FONT04W 18
#define FONT04H 37

#define FONT05NAME "font05.png"
#define FONT05CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "
#define FONT05W 17
#define FONT05H 17

#define FONT06NAME "font06.png"
#define FONT06CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ,-./0123456789:<>?'()@!# "
#define FONT06W 16
#define FONT06H 16

#define FONT07NAME "font07.png"
#define FONT07CHARS " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^"
#define FONT07W 8
#define FONT07H 8

void font_init(void)
{
	strcpy(fonts[FONT01].filename,FONT01NAME);
	fonts[FONT01].w=FONT01W;
	fonts[FONT01].h=FONT01H;
	strcpy(fonts[FONT01].charorder,FONT01CHARS);

	strcpy(fonts[FONT02].filename,FONT02NAME);
	fonts[FONT02].w=FONT02W;
	fonts[FONT02].h=FONT02H;
	strcpy(fonts[FONT02].charorder,FONT02CHARS);

	strcpy(fonts[FONT03].filename,FONT03NAME);
	fonts[FONT03].w=FONT03W;
	fonts[FONT03].h=FONT03H;
	strcpy(fonts[FONT03].charorder,FONT03CHARS);

	strcpy(fonts[FONT04].filename,FONT04NAME);
	fonts[FONT04].w=FONT04W;
	fonts[FONT04].h=FONT04H;
	strcpy(fonts[FONT04].charorder,FONT04CHARS);

	strcpy(fonts[FONT05].filename,FONT05NAME);
	fonts[FONT05].w=FONT05W;
	fonts[FONT05].h=FONT05H;
	strcpy(fonts[FONT05].charorder,FONT05CHARS);

	strcpy(fonts[FONT06].filename,FONT06NAME);
	fonts[FONT06].w=FONT06W;
	fonts[FONT06].h=FONT06H;
	strcpy(fonts[FONT06].charorder,FONT06CHARS);

	strcpy(fonts[FONT07].filename,FONT07NAME);
	fonts[FONT07].w=FONT07W;
	fonts[FONT07].h=FONT07H;
	strcpy(fonts[FONT07].charorder,FONT07CHARS);

	int i;
	for (i=0;i<LAST_FONT;i++)
	{
		fonts[i].fontimg=loadbmp(fonts[i].filename);
	}
}

/*
SDL_Surface *font_render(char *text,int fontnr)
{
	SDL_Surface *txtimg;
	SDL_Rect s,d;
	unsigned int i,j;

	txtimg=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_HWSURFACE,strlen(text)*fonts[fontnr].w,fonts[fontnr].h,
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask);
	SDL_SetColorKey(txtimg,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);

	for (i=0;i<strlen(text);i++) {

		for (j=0;j<strlen(fonts[fontnr].charorder);j++)
			if (text[i]==fonts[fontnr].charorder[j]) break;

		s.w=fonts[fontnr].w;
		s.h=fonts[fontnr].h;
		s.x=j*fonts[fontnr].w;
		s.y=0;

		d.w=fonts[fontnr].w;
		d.h=fonts[fontnr].h;
		d.x=i*fonts[fontnr].w;
		d.y=0;

		SDL_BlitSurface(fonts[fontnr].fontimg,&s,txtimg,&d);
	}
	return(txtimg);
}
*/

/* CΣΜT[tFCXΙAΆρπ_O */
static void font_render_surface_xy(SDL_Surface *txtimg, char *text, int fontnr, int x_offset, int y_offset)
{
	SDL_Rect s,d;
	s.w=fonts[fontnr].w; d.w=(s.w)/*fonts[fontnr].w*/;
	s.h=fonts[fontnr].h; d.h=(s.h)/*fonts[fontnr].h*/;
	unsigned int i,j;
	for (i=0;i<strlen(text);i++)
	{
		for (j=0;j<strlen(fonts[fontnr].charorder);j++)
		{
			if (text[i]==fonts[fontnr].charorder[j])
			{
				break;
			}
		}
		s.x=j*(s.w)/*fonts[fontnr].w*/;
		s.y=0;
		d.x=x_offset+i*(s.w)/*fonts[fontnr].w*/;
		d.y=y_offset/*0*/;
		SDL_BlitSurface(fonts[fontnr].fontimg,&s,txtimg,&d);
	}
}

/* T[tFCXπμηΘ’ΕAΌΪζΚΙ\¦ */
void font_print_screen_xy(char *text, int fontnr, int x, int y)
{
	SDL_SetColorKey(fonts[fontnr].fontimg/*screen*/,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	font_render_surface_xy( screen, text, fontnr, x, y);
}

/* VKT[tFCXπμ¬΅AΆρπ_O */
SDL_Surface *font_render(char *text,int fontnr)
{
	SDL_Surface *txtimg;
	txtimg=SDL_CreateRGBSurface(
	/* ±ΜΣoO ιρΎ―ΗAΗ­ν©ηΘ’B */
	#if 0
	/* VRAMΙζΑ½ϋͺ¬’ΝΈΎͺAΐΫΝx­ΘΑ½θ·ιBnίΎ―¬­ΔAΎρΎρx­Θθ(t[ͺΖ©)Ο¦ηκΘ’φx­ΘιB */
	/* PSPpΜSDLΜVRAMΗ http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.c ΕΝ */
	/* XgΕΗ΅ΔιͺA±ΜXgΜΉ’ΘCͺ·ιBrealloc(); ©Μΰφ΅’΅B(???) */
	/* sceGeEdramGetAddr(); β sceGeEdramGetSize(); ΜΣΝA³mΙTCYπζΎΕ«Θ’Cͺ·ιB(???) */
	SDL_SRCCOLORKEY|SDL_HWSURFACE,	/* VRAMΦmΫ·ι(VRAMͺ«θΘ’κΝASDLΝCΦζι) */
	#else
	/* »σ±ΑΏΜϋͺά΅ΑΫ’B(???) */
	/* ½Τρ[[N·ιB(gameover.c SDL_FreeSurface(go_surface1); SDL_FreeSurface(go_surface2); Ζ©Ό) */
	SDL_SRCCOLORKEY|SDL_SWSURFACE/*SDL_HWSURFACE*/,/* CΦmΫ·ι */
	#endif
	strlen(text)*fonts[fontnr].w,
	fonts[fontnr].h,
	screen->format->BitsPerPixel,
	screen->format->Rmask,
	screen->format->Gmask,
	screen->format->Bmask,
	screen->format->Amask);
	SDL_SetColorKey(txtimg,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	font_render_surface_xy( txtimg, text, fontnr, 0, 0);
	return(txtimg);
}

void font_print(char *text, int fontnr, int x, int y)
{
	SDL_Surface *textsurface;
	SDL_Rect r;

	textsurface=font_render(text,fontnr);
	r.x=x;
	r.y=y;
	r.w=textsurface->w;
	r.h=textsurface->h;
	SDL_BlitSurface(textsurface,NULL,screen,&r);
	SDL_FreeSurface(textsurface);
}
