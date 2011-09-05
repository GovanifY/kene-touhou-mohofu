
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	SDLフォント
	-------------------------------------------------------
	なんかフォントが多すぎてVRAM圧迫してGu化を阻むので色々廃止中。
	Guになれば、拡大縮小や色変えたりできるし。
	-------------------------------------------------------
16x6

 !"#$%&'()*+,-./
0123456789:;<=>?
@ABCDEFGHIJKLMNO
PQRSTUVWXYZ[\]^_
`abcdefghijklmno
pqrstuvwxyz{|}~d

---------------------------------------------------------*/

#include "111_my_file.h"/*(my_file_common_name)*/

global char my_font_text[MY_FONT_256_TEXT_LENGTH];
/*---------------------------------------------------------
	SDLフォント
---------------------------------------------------------*/

/*static*/global SDL_Surface *FONT_fontimg;

/*---------------------------------------------------------
	SDLフォントを起動時に一度だけ初期化する。
---------------------------------------------------------*/

global void font_init(void)
{
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/fonts/" "r35font.png");
	#if (0)/*(キャッシュしたらマズい。と思う。多分。)*/
	FONT_fontimg = load_chache_bmp();//, 0, 1);
	#else/*(r35)*/
	{
		SDL_Surface *s1;
		s1 = IMG_Load(my_file_common_name);
		FONT_fontimg = SDL_DisplayFormat(s1);/* サーフェスを表示フォーマットに変換する。 */
	}
	#endif
	SDL_SetColorKey(FONT_fontimg, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
}


//typedef struct
//{
//	u8 w;
//	u8 h;
//	u8 x_offset;
//	u8 y_offset;
//} FONT_SIZE;
//static const FONT_SIZE fonts[FONT_MAX] =
//{
//	{ 8, 10, 0,   0},/* ステータスパネル専用フォント、ボスメーター */
//	{16, 16, 0,  64},/* 漢字のＢＧ用 */
//	{16, 16, 0,  32},/* キーコンフィグ用 */
//	{16, 16, 0, 160},/* メニュー画面の手書き風、白いフォント(紅くした) */
//};

/*---------------------------------------------------------
	任意のサーフェイスに、文字列をレンダリング
---------------------------------------------------------*/
/*, char *text, int font_number, int x_offset, int y_offset*/
static void font16_render_surface_xy(SDL_Surface *txt_image_surface)
{
	SDL_Rect s;
	SDL_Rect d;
	s.w = (16); d.w = (16);
	s.h = (16); d.h = (16);
	unsigned int/*char*/ i;
	unsigned int/*char*/ char_number;
	i = 0;
//	for (i=0; i<strlen(my_font_text); i++)
	{
	loop_str:
		char_number = my_font_text[i];
		if (0==char_number)
		{
			return;
		}
		char_number ^= 0x80;// 上下入れ替え
//
		s.x = ((char_number & 0x0f)<<4);
		s.y = (((char_number&0xf0) ));  /*+y_offset*/
		d.x = cg.PSPL_font_x + ((i)<<(4));
		d.y = cg.PSPL_font_y;			/*0*/
		PSPL_UpperBlit(FONT_fontimg, &s, txt_image_surface, &d);
		i++;
		goto loop_str;
	}
}

#if 1
/*---------------------------------------------------------
	サーフェイスを作らないで、直接画面に表示
---------------------------------------------------------*/

//char *text, int font_number, int x, int y)
//global void font88_print_screen_xy(void)
global void font_render_view_screen(void)
{
//	SDL_SetColorKey(fonts[font_number].fontimg/*screen*/, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	font16_render_surface_xy(cb.sdl_screen[SDL_00_VIEW_SCREEN]/*, text, font_number, x, y*/);
}
#endif

/*---------------------------------------------------------
	[廃止予定] 新規サーフェイスを作成し、文字列をレンダリング
---------------------------------------------------------*/
//static int my_strlen(u8 *my_font_text)
static int my_font_len16(void)
{
	int i;
	i = 0;
	int count;
	count = 0;
count_loop:
	if (0==my_font_text[i])
	{
		return (count);
	}
	i++;
	count += 16;
	goto count_loop;
}

/*char *text, int font_number*/
global SDL_Surface *font16_render_new_surface(void)
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
	//	my_strlen(my_font_text)*(16)/*font width*/,
		my_font_len16()/*font width*/,
		(16)/*font height*/,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->BitsPerPixel,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Rmask,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Gmask,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Bmask,
		cb.sdl_screen[SDL_00_VIEW_SCREEN]->format->Amask);
	SDL_FillRect(txt_image_surface, NULL, 0/*SD L_MapRGB(txt_image_surface->format,0,0,0)*/);
	SDL_SetColorKey(txt_image_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	cg.PSPL_font_x = 0;
	cg.PSPL_font_y = 0;
	font16_render_surface_xy(txt_image_surface);/*, text, font_number, 0, 0*/
	return (txt_image_surface);
}
