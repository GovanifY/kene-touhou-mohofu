
/*---------------------------------------------------------
	フォント
---------------------------------------------------------*/

#include "game_main.h"

/* ステータスパネル専用フォント、ボスメーター */
#define FONT01NAME "fonts/font01.png"
#define FONT01CHARS " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklm"/*"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-.:'\"x/=><*()!?@_ "*/
#define FONT01W  8/*17*/
#define FONT01H 10/* 8 17*/

/* 名前入力のキーボード、玉に字、フォント */
#define FONT02NAME "fonts/font02.png"
#define FONT02CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT02W 16
#define FONT02H 16

/* メニュー画面の手書き風、白いフォント、キーコンフィグ */
#define FONT03NAME "fonts/font03.png"
#define FONT03CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT03W 16
#define FONT03H 16
/* 手書き風、白いフォントは手書きで 32x32 で作成してグラデ掛けてから縮小して若干修正(3の字)した */

/* 点画っぽいフォント、大文字＝緑、小文字＝黄色、？記号＝黄色、asciiコード(0x20-0x7e)順に並べなおした */
#define FONT04NAME "fonts/font04.png"
#define FONT04CHARS " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
//efine FONT04CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@(){},.-;:_#*+?=!\"ｧ$%&/�ﾖﾄﾜｵ^ｰ'`ｴﾟ\\<>| "
#define FONT04W 16 /*18*/
#define FONT04H 20 /*24*/ /*37*/

/* メニュー画面の手書き風、白いフォント(紅くした) */
#define FONT05NAME "fonts/font05.png"
#define FONT05CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ.,:!?[]/-1234567890"
#define FONT05W 16/*17*/
#define FONT05H 16/*17*/

/* ゴシック体っぽいフォント、ハイスコアタイトル、ゲームオーバー */
#define FONT06NAME "fonts/font06.png"
#define FONT06CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ,-./0123456789:"/*"<>?'()@!# "*/
#define FONT06W 16
#define FONT06H 20/*16*/

static FONT fonts[FONT_MAX];

/*---------------------------------------------------------
	フォント
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
	任意のサーフェイスに、文字列をレンダリング
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
	サーフェイスを作らないで、直接画面に表示
---------------------------------------------------------*/

void font_print_screen_xy(char *text, int font_number, int x, int y)
{
//	SDL_SetColorKey(fonts[font_number].fontimg/*screen*/,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	font_render_surface_xy(sdl_screen[SDL_00_SCREEN], text, font_number, x, y);
}

/*---------------------------------------------------------
	新規サーフェイスを作成し、文字列をレンダリング
---------------------------------------------------------*/

SDL_Surface *font_render(char *text, int font_number)
{
	SDL_Surface *txt_image_surface;
	txt_image_surface = SDL_CreateRGBSurface(
		/* この辺(ハングアップする)バグあるんだけど、良くわからない。 */
		#if 0
		/* VRAMに取った方が速いはずだが、実際は遅くなったりする。始めだけ速くて、だんだん遅くなり(数フレーム分とか)耐えられない程遅くなる。 */
		/* PSP用のSDLのVRAM管理 http://psp.jim.sh/svn/filedetails.php?repname=psp&path=%2Ftrunk%2FSDL%2Fsrc%2Fvideo%2Fpsp%2FSDL_pspvideo.c では */
		/* リスト処理で管理してるが、このリスト処理のせいな気がする。realloc(); 自体も怪しいし。(???) */
		/* sceGeEdramGetAddr(); や sceGeEdramGetSize(); の辺は、正確にサイズを取得できない気がする。(???) */
		SDL_SRCCOLORKEY|SDL_HWSURFACE,	/* VRAMへ確保する(VRAMが足りない場合は、SDLはメインメモリへ取る) */
		#else
		/* 現状こっちの方がましっぽい。(???) */
		/* たぶんメモリーリークする。(gameover.c SDL_FreeSurface(go_surface1); SDL_FreeSurface(go_surface2); とか他) */
		SDL_SRCCOLORKEY|SDL_SWSURFACE/*SDL_HWSURFACE*/,/* メインメモリへ確保する */
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
	フォント
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
