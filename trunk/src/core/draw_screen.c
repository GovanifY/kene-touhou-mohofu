
/*---------------------------------------------------------
	Guを使った画面描画処理
	-------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"

//#define USE_SWIZZLE 0
#define USE_SWIZZLE 1

//#define USE_16BIT_BLIT 0
#define USE_16BIT_BLIT 1

//#define USE_BLIT_COLOR_BIT 32
#define USE_BLIT_COLOR_BIT 16

/*	*/
//#if (1==USE_16BIT_BLIT)
//	#define USE_COLOR_16_AND_32 0
//#endif
	#define USE_COLOR_16_AND_32 1

//#define TW128 128/* 弾テクスチャの幅 */
//#define TH128 128/* 弾テクスチャの高さ */
//#define WK512 512/* 弾テクスチャを描画する場合に、描画ワークエリアの幅 */

/* 大きなスプライトを描画するときの分割サイズ */
#define SLICE_SIZE	(64)

/* 1:頂点カラーを使う。 */
#define USE_VCOLOR 1/*1*/

/* --- GU 描画のためのパケットサイズ */
#define PACKET_SIZE  (262144)

/* gulist設定が小さすぎると都合が悪い */
#if (PACKET_SIZE<(512*512)) /* shere short16==(512*512/2*short) and int32 ==(512*512*int) */
	#undef	PACKET_SIZE
	#define PACKET_SIZE  (512*512)
#endif

//#define UNKNOWN_MY_FLAGS (GU_COLOR_5650)

	#if (1==USE_VCOLOR)
		/* 頂点カラーを使う場合。一般的。 */
		#if (16==USE_BLIT_COLOR_BIT)
			/* 15/16bit描画 */
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//
		//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650_C32	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_4444 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#else
			/* 32bit描画 */
			#define TEXTURE_FLAGS5650	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#endif
	#else
		/* 頂点カラーを使わない場合。「GU_COLOR」を付けると動かない。 */
		//	#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	#endif


// /* Vertex Declarations Begin */
//#define GU_TEXTURE_SHIFT(n)	((n)<<0)
//#define GU_TEXTURE_8BIT		GU_TEXTURE_SHIFT(1) 	0x01
//#define GU_TEXTURE_16BIT		GU_TEXTURE_SHIFT(2) 	0x02
//#define GU_TEXTURE_32BITF 	GU_TEXTURE_SHIFT(3) 	0x03
//#define GU_TEXTURE_BITS		GU_TEXTURE_SHIFT(3) 	0x03

//	*	- GU_COLOR_5650 - 16-bit color (R5G6B5A0)
//	*	- GU_COLOR_5551 - 16-bit color (R5G5B5A1)
//	*	- GU_COLOR_4444 - 16-bit color (R4G4B4A4)
//	*	- GU_COLOR_8888 - 32-bit color (R8G8B8A8)

//#define GU_COLOR_SHIFT(n) ((n)<<2)
//#define GU_COLOR_RES1 	GU_COLOR_SHIFT(1)	0x04
//#define GU_COLOR_RES2 	GU_COLOR_SHIFT(2)	0x08
//#define GU_COLOR_RES3 	GU_COLOR_SHIFT(3)	0x0c
//#define GU_COLOR_5650 	GU_COLOR_SHIFT(4)	0x10
//#define GU_COLOR_5551 	GU_COLOR_SHIFT(5)	0x14
//#define GU_COLOR_4444 	GU_COLOR_SHIFT(6)	0x18
//#define GU_COLOR_8888 	GU_COLOR_SHIFT(7)	0x1c
//#define GU_COLOR_BITS 	GU_COLOR_SHIFT(7)	0x1c


/* 管理するテクスチャー */
enum
{
	TEX_00_BACK_GROUND = 0, 	/* [256x256]3D背景1 */
//	TEX_01_BACK_TEXTURE,		/* 背景障害物 */
	TEX_02_MAHOUJIN,			/* [128x128]魔方陣 */
	TEX_03_JIKI,				/* [256x256]自弾/自機 */
	TEX_04_TEKI,				/* ボス/ザコ敵 */
//	TEX_05_ITEM,				/* アイテム/漢字スコア */
	TEX_06_BULLET,				/* [128x128]敵弾 */
	TEX_07_FRONT,				/* [256x256]自機当たり表示/爆発/[コンティニュー文字(bank00)/メニュー文字(bank01)/メニュー文字(bank02)] */
//	TEX_08_SCORE_PANEL, 		/* [TEX_07_FRONTと融合する可能性が高い(256x512テクスチャ？)] スコアパネル/スコアフォント文字 */
//	TEX_09_TACHIE,				/* 立ち絵 */
//	TEX_10_MESSAGE, 			/* [TEX_07_FRONTと融合する可能性が高い(512x512テクスチャ？)]メッセージ固定文字 */
	TEXTURE_MAX 				/* --- 管理する最大テクスチャー数 */
};

//#define USE_SDL_image 1
#define USE_SDL_image 0
#if (1==USE_SDL_image)
	//#include "_SDL_image.h"//#include "_SDL_image.h"
	#define MY_DIB_SURFACE	SDL_Surface
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	unused1
#else
	#include "graphics.h"
	#define MY_DIB_SURFACE	my_image
//	#define MY_DIB_DATA 	data
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	wk512
#endif




typedef struct
{
	unsigned short	u;
	unsigned short	v;
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
			unsigned short	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			unsigned int	color;
		#endif
	#endif
	short			x;
	short			y;
	short			z;
} Vertex_uvcxyz;

typedef struct
{
	unsigned short	u;
	unsigned short	v;
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			unsigned short	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			unsigned int	color;
		#endif
	#endif
	short			x;
	short			y;
	short			z;
} Vertex_uvcxyz_C32;

typedef struct
{
	/*float*/unsigned short x;
	/*float*/unsigned short y;
	/*float*/unsigned short z;
} Point_Vertex;

typedef struct
{
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			unsigned short	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			unsigned int	color;
		#endif
	#endif
	/*float*/unsigned short x;
	/*float*/unsigned short y;
	/*float*/unsigned short z;
} Point_Vertex_color;

/*---------------------------------------------------------
	グローバル変数
---------------------------------------------------------*/
// /*extern*/SDL_Surface *screen;
// SDL_Surface *back_screen;
// SDL_Surface *tex_screen;

SDL_Surface *sdl_screen[4];


static UINT16 *render_image;

static UINT16 *render_image_back;
static UINT16 *render_image_tex;

/*static*/ UINT16 *msg_window_image;

//static UINT16 *bullet_image;
static SDL_Surface *SDL_VRAM_SCREEN;


static	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];

/*---------------------------------------------------------
	スクリーン管理
---------------------------------------------------------*/

static	TGameSprite  ggg_Sprites[SPRITEMAX];

typedef struct
{
	MY_DIB_SURFACE *my_texture; 		/* テクスチャ画像 */
	int 			texture_width;		/* テクスチャ幅 */
	int 			texture_height; 	/* テクスチャ高さ */
	int 			buffer_width;		/* 512 固定？ */
//
	int 			color_format;		/* 変換済み画像形式 */
	/*	読み込み時にARGB8888から画像変換を行うが、
		どういう形式に画像変換するか指定する。
		16bit形式でも ABGR0565 とか ABGR1555 とか ABGR4444 がある。
	 */
	int 			hh;/*予備*/
	int 			jj;/*予備*/
	char			*file_name;/* ファイル名 */
} MY_TEXTURE_RESOURCE;

//static	MY_DIB_SURFACE *my_texture[TEXTURE_MAX];
static	MY_TEXTURE_RESOURCE 	my_resource[TEXTURE_MAX];
/*static*/	const MY_TEXTURE_RESOURCE	initial_resource[TEXTURE_MAX] =
{
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//		TEX_00_BACK_GROUND = 0, 	/* 3D背景1 */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_01_BACK_TEXTURE,		/* 背景障害物 */
	{NULL,128,128,128,	0,0,0,DIRECTRY_NAME_DATA "/effect/mahoujin128.png"},	//		TEX_02_MAHOUJIN,			/* 魔方陣 */
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/jiki/jiki00.png"},			//		TEX_03_JIKI,				/* 自弾/自機 */
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/zako/teki0_256.png"},		//	//	TEX_04_TEKI,				/* ボス/ザコ敵 */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_05_ITEM,				/* アイテム/漢字スコア */
	{NULL,128,128,128,	0,0,0,DIRECTRY_NAME_DATA "/tama/bullet.png"},			//		TEX_06_BULLET,				/* 敵弾 */
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/effect/front256.png"},		//		TEX_07_FRONT,				/* 自機当たり表示/爆発/[コンティニュー文字(bank00)/メニュー文字(bank01)/メニュー文字(bank02)] */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_08_SCORE_PANEL, 		/* スコアパネル/スコアフォント文字 */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_09_TACHIE,				/* 立ち絵 */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_10_MESSAGE, 			/* メッセージ固定文字 */
};

void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;
int draw_side_panel;
int draw_boss_hp_value;
int boss_life_value;

unsigned int conv_bg_alpha;

/*---------------------------------------------------------
	画像関連
---------------------------------------------------------*/

void psp_clear_screen(void)
{
	/* 将来Guで描いた場合。ハードウェアー機能で、置き換えられるので今のうちにまとめとく */
	SDL_FillRect(sdl_screen[SDL_00_SCREEN],NULL,SDL_MapRGB(sdl_screen[SDL_00_SCREEN]->format,0,0,0));
}
//void psp_move_screen(SDL_Surface *src_screen, SDL_Surface *dst_screen )
void psp_move_screen(int src_screen_number, int dst_screen_number )
{
	/* 将来Guで描いた場合。ハードウェアー機能で、置き換えられるので今のうちにまとめとく */
	SDL_BlitSurface(sdl_screen[src_screen_number],NULL,sdl_screen[dst_screen_number],NULL);
}
//void psp_push_screen(void)
//{
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN],NULL,sdl_screen[SDL_01_BACK_SCREEN],NULL);
//}
//void psp_pop_screen(void)
//{
//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],NULL,sdl_screen[SDL_00_SCREEN],NULL);
//}


void gu_save_screen(void)
{
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); 	}	/* ロック */
	u16 *dest;	dest = render_image;
	u16 *src;	src = (UINT16 *)((UINT32) 0x44000000);/*VRAM*/
	#if 1
	{
		int ii;
		for (ii=0; ii<(512*272); ii++)
		{
			*dest = *src;
			dest++;
			src++;
		}
	}
	#else
	/* ダメ */
//	sceGuStart(GU_DIRECT, gulist );
//	sceGuCopyImage(
//		/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/,	//	int psm,	GU_PSM_8888 /* Image format */
//		0,				//	int sx, 		/* src location */
//		0,				//	int sy, 		/* src location */
//		512,			//	int width,		/* Image size */
//		272,			//	int height, 	/* Image size */
//		512,			//	int srcw,		/* src buffer width */
//		src,			//	void* src,		/* src Image from RAM */
//		0,				//	int dx, 		/* dest location */
//		0,				//	int dy, 		/* dest location */
//		512,			//	int destw,		/* dest buffer width */
//		dest			//	void* dest		/* dest Image to VRAM */
//	);
//	sceGuFinish();
//	sceGuSync(0, 0);
	#endif
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}	/* ロック解除 */
}

/*------------------------------------------------------------- */

extern int select_player;
static void TGameTexture_Load_Surface(int num);
void select_jiki_load_surface(void)
{
//	strcpy(filename, DIRECTRY_NAME_DATA "/jiki/jiki0Z.png");
	my_resource[TEX_03_JIKI].file_name[11+DIRECTRY_NAME_LENGTH] = ('0'+select_player);
	TGameTexture_Load_Surface( TEX_03_JIKI );
//	draw_jiki_screen = 1;
}
static void gu_draw_bg_fake3D(void);
static void gu_draw_bg_2D(void);
static void gu_draw_bg_eientei(void);
void stage_bg_load_surface(void)
{
//	strcpy(filename, DIRECTRY_NAME_DATA "/bg/backZ_256.png");
	my_resource[TEX_00_BACK_GROUND].file_name[8+DIRECTRY_NAME_LENGTH] = ('0'+player_now_stage);
	TGameTexture_Load_Surface( TEX_00_BACK_GROUND );
	void (*aaa[8])(void)  =
	{
		gu_draw_bg_2D,		/*loading*/
		gu_draw_bg_fake3D,	/*1面"魔法の森"*/
		gu_draw_bg_2D,		/*2面"秋めく滝"*/
		gu_draw_bg_eientei, /*3面"竹薮"*/
		gu_draw_bg_eientei, /*4面"永遠亭 廊下"*/
		gu_draw_bg_fake3D,	/*5面"紅魔館 書斎"*/
		gu_draw_bg_fake3D,	/*6面"紅魔館 大廊下"*/
		gu_draw_bg_fake3D,	/*ending*/
	};
	callback_gu_draw_haikei = aaa[player_now_stage&0x07];// 	dr aw_bg_screen = 1;
//	callback_gu_draw_haikei = callback_gu_draw_haikei_all;//	dr aw_bg_screen = 1;
}


void draw_loading_screen_test(void);
extern void sendCommandi(int cmd, int argument);
void psp_video_init(void)
{
	#if 0
	if (atexit(SDL_Quit))
	{
		CHECKPOINT;
		error(ERR_WARN, "atexit dont returns zero");
	}
	#endif
	SDL_VRAM_SCREEN = SDL_SetVideoMode(
		PSP_WIDTH480,
		PSP_HEIGHT272,
		/*int depth 		=*/ SDL_5551_15/*PSP_DEPTH16*/,
		/*UINT32 videoflags =*/ (SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL)
	//	/*UINT32 videoflags =*/ (SDL_FULLSCREEN /*| SDL_DOUBLEBUF*/ | SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL)
		);
	if (NULL == SDL_VRAM_SCREEN)
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant open screen: "/*"%s", SDL_GetError()*/);
	}
	#if (1==USE_GU)
	//#define SDL_BUF_WIDTH512 (512)
	sdl_screen[SDL_00_SCREEN] = SDL_CreateRGBSurface(
		/*SDL_SWSURFACE*/SDL_HWSURFACE,/*VRAMへ*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*sdl_screen[SDL_00_SCREEN]->w*/
		PSP_HEIGHT272,/*sdl_screen[SDL_00_SCREEN]->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
		if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); }
		render_image = (UINT16 *)sdl_screen[SDL_00_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}
	#endif
//	sdl_screen[SDL_01_BACK_SCREEN] = NULL;
	sdl_screen[SDL_01_BACK_SCREEN] = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*メインメモリへ*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
	#if (1==USE_GU)
		if (SDL_MUSTLOCK(sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_01_BACK_SCREEN]);	}
		render_image_back = (UINT16 *)sdl_screen[SDL_01_BACK_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_01_BACK_SCREEN]); }
	#endif
	#if (0)
	if (NULL == sdl_screen[SDL_01_BACK_SCREEN])
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant create SDL_Surface: "/*"%s", SDL_GetError()*/);
	}
	#endif
	sdl_screen[SDL_02_TEX_SCREEN] = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*メインメモリへ*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
	#if (1==USE_GU)
		if (SDL_MUSTLOCK(sdl_screen[SDL_02_TEX_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_02_TEX_SCREEN]); }
		render_image_tex = (UINT16 *)sdl_screen[SDL_02_TEX_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_02_TEX_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_02_TEX_SCREEN]); }
	#endif



	//# /* カスタムライブラリかGuを使う場合 */

#if (1==USE_16BIT_BLIT)
	#define  SCREEN_DEPTH	16
#else
	#define  SCREEN_DEPTH	32
#endif

	/* setup GU initialise */
	sceGuInit();
	#if 0
	/* 無くても殆ど同じ */
	sceGuDisplay(GU_FALSE);/*画面OFF*/
	#endif
	sceGuStart(GU_DIRECT, gulist);
	#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
	//	sceGuDrawBuffer(GU_PSM_5551, (void*)0, BUF_WIDTH512);
		sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x88000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
		sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x110000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#endif /* SCREEN_DEPTH */
	sceGuOffset(2048 - (PSP_WIDTH480 / 2), 2048 - (PSP_HEIGHT272 / 2));
	sceGuViewport(2048, 2048, PSP_WIDTH480, PSP_HEIGHT272);

	#if 1
	/* 描画範囲を設定する */
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);
	#endif

	#if 1/*???*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);		/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuDepthFunc(GU_GEQUAL);
	//sceGuEnable(GU_DEPTH_TEST);		/*sceGuDisable(GU_DEPTH_TEST);noiz07*/
	#endif/*???*/

#if 000/*???*/
	#if 1/*???*/
	/*「アルファブレンディング(透明度付き、色重ね合わせ)」*/
	sceGuAlphaFunc(GU_ALWAYS,0,0xff);//sceGuAlphaFunc(GU_LEQUAL, 0, 0x01);
	sceGuEnable(GU_ALPHA_TEST); 	/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);			/* 有効 */
	//sceGuDisable(GU_ALPHA_TEST);			/* 無効 */	/*noiz07*/
	#endif/*???*/
#endif/*000???*/

	#if (1==USE_ZBUFFER)
	//sceGuEnable(GU_DEPTH_TEST);			/* 有効 */
	//sceGuDisable(GU_DEPTH_TEST);			/* 無効 */	/*noiz07*/
	sceGuDisable(GU_DEPTH_TEST);
	/* psp の z値は 65535(近く) から 0(遠く)で、Z値が通常と逆。 */
	/* このz値は unsigned short なので、少しはみ出すと、回り込んで変な描画になってしまう。 */
	/* そこで、回り込まないように、適当に余裕分を見繕う。PSPSDK のサンプルでは */
	/* 0xc350(50000) 近く側の境界面、0x2710(10000)遠く側の境界面 となっているが、適当な値なら何でもよい */
	/* sceGuDepthRange(0xc350, 0x2710); */
	#define  Z_CLIP_LENGTH	(10000)
	sceGuDepthRange((0xffff-Z_CLIP_LENGTH), Z_CLIP_LENGTH);
//	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	#if 1
	/* カリングモード(裏向きポリゴンは描画しない)で、有効座標系(右手座標系にするか左手座標系にするか)を決める */
	sceGuEnable(GU_CULL_FACE);	/* カリング有効(裏向きポリゴンは描画しない) */
	sceGuFrontFace(GU_CW);		/* clock wise(時計周り) 		  右回り、右手座標系(OpenGL 標準?)	 */
//	sceGuFrontFace(GU_CCW); 	/* counter clock wise(反時計周り) 左回り、左手座標系(Direct X 標準?) */
/*
  時計回り(とけいまわり)というのは、時計の針が進む方向と同じ回転方向の事である。
上→右→下→左、あるいは北→東→南→西の順に回転する方向である。右回りとも言われる。
  逆の回転方向を「反時計回り」(はんとけいまわり)という。同様に、これは左回りと言われる。
反時計回りは英語圏では counterclockwise と呼ばれるが、これは CCW と略す場合が多い。

  時計回りの回転方向は日時計を模している。日時計は北回帰線より北で発明された。
日時計の回転方向は、北回帰線より北においては時計回りになるが南では反時計回りになる。

  方位(方位角)は、北を0°として時計回りに数を増やすが、
逆に数学においては、平面角は反時計回りの方向を正として計測される。

  瓶のふたやネジ等は、時計回りに回すと締まり、反時計回りに回すと緩むように作られているが、
これらは右ききの場合、時計回りの方が力が入り易いという理由による。
力の入れすぎで破損するのを防ぐ為にガス管ではこれらとは逆方向になっている。

そーなのかー
*/
	#endif

#if 1/*???*/
	#if 1/*???*/
	sceGuShadeModel(GU_SMOOTH);
	#endif/*???*/
#endif/*1???*/

	#if 0/*???*/
	sceGuDisable(GU_BLEND);
	#else
	sceGuEnable(GU_BLEND);			/* アルファブレンディング有効 */
	#endif/*???*/

	#if 1
	/* テクスチャー有効モードでは、線や面等のフラットポリゴンが使えない */
	sceGuEnable(GU_TEXTURE_2D); 	/* テクスチャー有効 */
	#else
	/* テクスチャー無効モードでは、線や面等のフラットポリゴンが使える(テクスチャーポリゴン、2D画像転送等は出来ない) */
	sceGuDisable(GU_TEXTURE_2D);	/* テクスチャー無効 */
	#endif

	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);//Z_BUFFER
/*??*/	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);	/*noiz07*/
	#endif/* (1==USE_ZBUFFER) */
//	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);	/*noiz07*/

	#if 1
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* 半透明 */
	#else
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);/* 半透明不可？ */
	#endif

//	sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*くっきり拡大画面(拡大時双曲線補完)*/
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*ぼやぼや拡大画面(拡大時直線補完)*/
	sceGuTexScale(1.0f, 1.0f);//sceGuTexScale(1,1);
	sceGuTexOffset(0.0f, 0.0f);
//
	/* 環境色を決める？ */
	sceGuTexEnvColor(	0x00000000);
//	sceGuTexEnvColor(	0x00ffff00);
	sceGuAmbientColor(	0xffffffff);
	sceGuColor( 		0xff000000);

//


//	u8	ColorKey	= 1;
/* カラーキーを設定する */
//	if (ColorKey)
	{
//		if (!sceGuGetStatus(GU_COLOR_TEST))
		{	sceGuEnable(GU_COLOR_TEST);}/*カラーキー有効*/
	//	sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00fefefe);
		sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00ffffff);
	//	sendCommandi(216, GU_NOTEQUAL & 0x03);
	//	sendCommandi(217, 0 & 0xffffff);
	//	sendCommandi(218, 0x00fefefe);
	}
//	else
//	{
//		if (sceGuGetStatus(GU_COLOR_TEST))
//		{	sceGuDisable(GU_COLOR_TEST);}/*カラーキー無効*/
//	}
	#if 1
	sceGuEnable(GU_CLIP_PLANES);
	#endif
//
	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/*vsync*/
	sceGuDisplay(GU_TRUE/*1*/);/*画面ON*/
	/* ここまで初期設定 */

//	sceDisplayWaitVblankStart();/*vsync*/

	/* --- 入力装置の初期設定 */
	sceCtrlSetSamplingCycle(0); 	/*???*/
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);	/* アナログキー有効(標準ではデジタルのみ) */

	//static TGameTexture *TGameTexture_Create(void)
	{
		unsigned int i;
		/* --- 汎用スプライトマネージャの初期化 */
		for (i=0; i<SPRITEMAX; i++)
		{
//			ggg_Sprites[i].texture_id	= 0/*-1*/;
		//	ggg_Sprites[i].Texture_ptr	= NULL;
			ggg_Sprites[i].used 		= 0/*FA LSE*/;
			#if (1==USE_ZOOM_XY)
			ggg_Sprites[i].zoom_x256	= 256/*1.0*/;
			ggg_Sprites[i].zoom_y256	= 256/*1.0*/;
			#else //(0==USE_ZOOM_XY)
			ggg_Sprites[i].zoom_xy256	= 256/*1.0*/;
			#endif/* (1==USE_ZOOM_XY) */
			ggg_Sprites[i].rotation_z	= 0/*.0*/;
		}
		/* --- テクスチャマネージャの初期化 */
		for (i=0; i<TEXTURE_MAX; i++)
		{
		//	my_resource[i].my_texture = NULL;
			my_resource[i].my_texture		= initial_resource[i].my_texture;
			my_resource[i].texture_width	= initial_resource[i].texture_width;
			my_resource[i].texture_height	= initial_resource[i].texture_height;
			my_resource[i].buffer_width 	= initial_resource[i].buffer_width;
		//
			my_resource[i].color_format 	= initial_resource[i].color_format;
			my_resource[i].hh				= initial_resource[i].hh;/*予備*/
			my_resource[i].jj				= initial_resource[i].jj;/*予備*/
			my_resource[i].file_name		= initial_resource[i].file_name;
			/* --- テクスチャの初期化 */
			{
			#if (0)
			//	TGameTexture_Load_Surface( TEX_00_BACK_GROUND	);/*title*/
				TGameTexture_Load_Surface( TEX_02_MAHOUJIN		);/*game*/
				TGameTexture_Load_Surface( TEX_04_TEKI			);/*game*/
				TGameTexture_Load_Surface( TEX_06_BULLET		);/*game*/
				TGameTexture_Load_Surface( TEX_07_FRONT 		);/*game*/
			#else //(0==USE_ZOOM_XY)
				TGameTexture_Load_Surface( i );/*game*/
			#endif/* (1==USE_ZOOM_XY) */
			}
		}
	}

	/* --- その他の初期設定 */
	draw_side_panel 		= 0;
	draw_boss_hp_value		= 0;
//	callback_gu_draw_haikei = NULL;//dr aw_bg_screen = 0;
	conv_bg_alpha			= 0xffffffff;
//	conv_bg_alpha			= 0x7f7f7f7f;
//

	/* --- ローディング画面 */
	player_now_stage		= 0;
	stage_bg_load_surface();
	draw_loading_screen_test();
}
/*-------------*/
/*-------------*/

/*---------------------------------------------------------
	テクスチャをロードする
---------------------------------------------------------*/

#define USE_T128_SWIZZLE 0

//#if 1//(1==USE_COLOR_16_AND_32)
#if (1==USE_COLOR_16_AND_32)
static void convert_swizzle
(
		  u8	*out_data,		/* 出力画像の先頭ポインタ */
	const u8	*in_data,		/* 入力画像の先頭ポインタ */
/* (例えば、512x512テクスチャーの場合) */
	const u32	width_x_size,	/* 幅かけるデーターサイズ */	/*max: 512*4 */
	/* (例えば、ARGB8888 なら 512*4==width_x_size) */
	/* (例えば、ARGB1555 なら 512*2==width_x_size) */
	const u32	height			/* 高さ */		/*max: 512*/
	/* (例えば、512==height) */
)
{
	#if (1==USE_SWIZZLE)
	const u8	*src_y = in_data;
	u32 		*dst  = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		const u8	*src_x = src_y;
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			const u32	*src = (u32*)src_x;
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += /*src_pitch*/((width_x_size-16)>>2/* /4 */);
			}
			src_x += 16;
		}
		src_y += /*src_row*/(width_x_size <<3/* *8 */);
	}
	#else
	u32 		*src = (u32*)in_data;
	u32 		*dst = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
			}
		}
	}
	#endif
}
#endif
//#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	//#define RMASK 0xff000000
	//#define GMASK 0x00ff0000
	//#define BMASK 0x0000ff00
	//#define AMASK 0x000000ff
	//#define RMASK16 0x0000f800
	//#define GMASK16 0x000007c0
	//#define BMASK16 0x0000003f
	//#define AMASK16 0x00000001
//#else
	//#define RMASK 0x000000ff
	//#define GMASK 0x0000ff00
	//#define BMASK 0x00ff0000
	//#define AMASK 0xff000000
	#define RMASK16 0x0000001f
	#define GMASK16 0x000003e0
	#define BMASK16 0x00007c00
	#define AMASK16 0x00008000
//#endif

/*---------------------------------------------------------
	テクスチャーの読み込み、登録
---------------------------------------------------------*/

static void TGameTexture_Load_Surface( int num )	/*, char *file_name*/
{
	/* ----- テクスチャ番号が不正だったら終了 */
//	if (num < 0) return;
//	if (num > TEXTURE_MAX) return;

	/* ----- 既にテクスチャがあったら解放 */
	if (NULL != my_resource[num].my_texture)
	{
		#if (1==USE_SDL_image)
		SDL_FreeSurface(my_resource[num].my_texture);
		#else
		png_free_my_image(my_resource[num].my_texture);
		#endif
		my_resource[num].my_texture = NULL;
	}

	/* ----- テクスチャーの読み込み */
	#if (1==USE_SDL_image)
	my_resource[num].my_texture = IMG_Load(my_resource[num].file_name);
//	my_resource[num].my_texture = SDL_LoadBMP(my_resource[num].file_name);
	#else
	my_resource[num].my_texture = png_load_my_image(my_resource[num].file_name);//
	#endif
	if (NULL == my_resource[num].my_texture)
	{
	//	my_resource[num].my_texture = NULL;
		#if (1)
		CHECKPOINT;
		error(ERR_FATAL, "texture:cant load image %s:", my_resource[num].file_name);/*" %s"*/ /*,SDL_GetError()*/
		#endif
		return;
	}

	/* --- PSP 向けにテクスチャを加工する */

	/* --- PSPは 16byte align に無いと 都合が悪い(遅い?) らしい。 */
	/* --- そこで memalign(); を使い、メモリ境界を合わせる。 */
	void *nonalign;
//	nonalign = 0;
	nonalign = (my_resource[num].my_texture)->MY_DIB_DATA;

	int mymap_128;
	//	mymap_128 = (my_resource[num].my_texture)->texture_width;
		mymap_128 = my_resource[num].texture_width;
		#define my_map_TW128 mymap_128
		#define my_map_TH128 mymap_128

	unsigned int msize;
//	msize = 0;
	#if (1==USE_SDL_image)
	msize = ((my_resource[num].my_texture)->w * (my_resource[num].my_texture)->h)
		 * (my_resource[num].my_texture)->format->BytesPerPixel;
	#else
	msize = (/*w*/my_map_TW128/*512*/ * /*h*/my_map_TH128/*512*/) * /*bpp*/4;
	#endif

#if (1==USE_COLOR_16_AND_32)
	/* --- PSP で簡易16ビット色を指定した場合は16bitに落とす */
	#if (1==USE_SDL_image)
	if (my_texture[num]->format->BytesPerPixel == 4)	/* PSP16bit mode && DIB==32bit color */
	#else
	if (/*BytesPerPixel*/4 == 4)	/* PSP16bit mode && DIB==32bit color */
	#endif
	//if (0)
	{
	unsigned short *pixdst;
	unsigned long  *pixsrc;
		/* --- 16bit色に 減色して保持 */
		(my_resource[num].my_texture)->pixels = (void*)memalign(16, (msize>>1)/*(msize / 2)*/);
		#if (1==USE_SDL_image)
		msize = ((my_resource[num].my_texture)->w * (my_resource[num].my_texture)->h);
		#else
		msize = (my_map_TW128 * my_map_TH128);
		#endif
		pixdst = (unsigned short *)/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/;
		pixsrc = (unsigned long *)nonalign;
		int loop;
	//	loop = 0;
		for (loop=0; loop<msize; loop++)
		{
			#if 0
			/* --- 15bit色に 減色して保持 (color key 使用可) */
			*pixdst =
				((*pixsrc & 0x80000000) >> 16) |
				((*pixsrc & 0x00f80000) >>	9) |
				((*pixsrc & 0x0000f800) >>	6) |
				((*pixsrc & 0x000000f8) >>	3);
		//	*pixdst = MAKECOL15A(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc), GETA32(*pixsrc));
			#else
			/* --- 16bit色に 減色して保持 (color key 使用不可) */
			*pixdst = MAKECOL16(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc));
			#endif
			pixdst++;
			pixsrc++;
		}

	//	unsigned short *pixsrc2;
	//	pixsrc2 = (unsigned short *)pclass->bitmap[num]->pixels;
	//	pixdst = (unsigned short *)gulist/*convert_works*/;
	//	for (loop=0; loop<msize; loop++)
	//	{
	//		(*pixdst) = (*pixsrc2);
	//		pixdst++;
	//		pixsrc2++;
	//	}
		#if (1==USE_SDL_image)
		my_texture[num]->format->BytesPerPixel =  2;
		my_texture[num]->format->BitsPerPixel  = 16;
		my_texture[num]->format->Rmask = /*PSP_SCREEN_FORMAT_RMASK*/RMASK16/*RMASK16*/;
		my_texture[num]->format->Gmask = /*PSP_SCREEN_FORMAT_GMASK*/GMASK16/*GMASK16*/;
		my_texture[num]->format->Bmask = /*PSP_SCREEN_FORMAT_BMASK*/BMASK16/*BMASK16*/;
		my_texture[num]->format->Amask = /*PSP_SCREEN_FORMAT_AMASK*/AMASK16/*AMASK16*/;
		#endif
	// 16bit色
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 2 */  my_map_TW128/*512*/*2, 	/* short だから 2倍 */
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
	}
	else/* 32bit mode */
#endif
	{
		/* --- 32bit色 通常モード */
		((my_resource[num].my_texture)->MY_DIB_DATA) = (void*)memalign(16, msize);
		memcpy(/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/, nonalign, msize);
		#if (1)
	// 32bit色
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 4 */  my_map_TW128/*512*/*4, 	/* int だから 4倍 */
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#endif
	}
	free(nonalign);
}

/*---------------------------------------------------------
	汎用スプライトの表示 (回転拡大縮小なし)ルーチン
	汎用スプライトの表示(画面転送)
---------------------------------------------------------*/

#if (1==USE_ZBUFFER)
	/* ハードウェアーでソート */
	#define SPR_PRIORITY	(spr->priority)
#else
	/* ソフトウェアーで Zソート */
	#define SPR_PRIORITY	(0)
#endif
/* --- 汎用スプライトを一枚スクリーンに貼り付ける */
static void render_object_no_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- 表示スイッチ */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;

	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			#if 0
				/*ARGB4444*/
				unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
			#endif
			#if 1
				/*ARGB0565*/
				unsigned /*int*/short blendlevel = MAKECOL16( (spr->alpha), (spr->alpha), (spr->alpha) );
			#endif
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (spr->color8888);
		#endif
	#endif

	/* --- 汎用スプライトを一枚描画する */
	/* ある程度の大きさを持ったスプライトは短冊状に分割しないと */
	/* とてつもなく遅くなる */

	unsigned int/*short*/ w_rest;
	w_rest = spr->w + SLICE_SIZE;

	unsigned int/*short*/ count2;
	#if 0
	//count2 = (((spr->w / SLICE_SIZE)+1)*2);
	#else
	count2 = 0;
//	for (j=0; (j+SLICE_SIZE)<=(/*480*/spr->w/*512*/); j+=SLICE_SIZE)
	unsigned int/*short*/ i;
	i = 0;
	do
	{
		count2 += 2;
		i += SLICE_SIZE;
		w_rest -= SLICE_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz_C32));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	for (; i<count2; )
	{
	//	if ((i1+1) < count)
	//	if (((i)+1*2) < (count2))	{}	else		/* 最後 */
		if (i==(count2-2))		/* 最後 */
		{
			w_size = w_rest;
		}
	//
		vertices[(i)].u = (spr->tx + pos);
		vertices[(i)].v = spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color = blendlevel;
		#endif
		vertices[(i)].x = (x_pos + pos);
		vertices[(i)].y = y_pos;
		vertices[(i)].z = SPR_PRIORITY;
	//
		vertices[(i)+1].u	= (spr->tx + pos) + w_size;
		vertices[(i)+1].v	= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color = blendlevel;
		#endif
		vertices[(i)+1].x	= (x_pos + pos) + w_size;
		vertices[(i)+1].y	= y_pos + spr->h;
		vertices[(i)+1].z	= SPR_PRIORITY;
		i += 2;
		pos += SLICE_SIZE;
	}
	/* --- 描画リクエスト */
//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS4444, (count2), NULL, vertices);
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650_C32, (count2), NULL, vertices);
}

/*---------------------------------------------------------
	汎用スプライトの表示 (回転拡大縮小あり)ルーチン
	-------------------------------------------------------
	現在(2009-11-13現在)横しか slice してないけど、どうも縦横に
	 slice しないと(チルノやレミリアボムの速度低下っぷりから判断して)
	ダメですね。あとで改良しよう。
	-------------------------------------------------------
	逆に弾幕は(64[dots]以下の > 大弾ぐらいなら) slice しなくても
	いいのかもしれない。
	弾幕はシステム的にも「汎用スプライトで描かない方が効率が良い」ので、
	専用ルーチンに分岐する予定(2009-11-13現在)です。
---------------------------------------------------------*/

static void render_object_use_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- 表示スイッチ */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- 回転拡大前処理 */
	int sin_angle;	/* 位相  +0度進み */
	int cos_angle;	/* 位相  +90度進み */
//	int c18_angle;	/* 位相 +180度進み */
	{
		/* 角度は0-511度 */
		unsigned int rotation_angle512;
	//	rotation_angle512	= ((spr->rotation_z) / (128/*65536/512*/)); 	/* 角度は0-65535度なので0-511度へ変換。 */
		rotation_angle512	= ((spr->rotation_z) /*>> (7)*/);
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle512)&(512-1))]/*<<8*/);
	}
//
	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			#if 0
				/*ARGB4444*/
				unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
			#endif
			#if 1
				/*ARGB0565*/
				unsigned /*int*/short blendlevel = MAKECOL16( (spr->alpha), (spr->alpha), (spr->alpha) );
			#endif
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (spr->color8888);
		#endif
	#endif

	/* --- 汎用スプライトを一枚描画する */
	/* ある程度の大きさを持ったスプライトは短冊状に分割しないと */
	/* とてつもなく遅くなる */

	unsigned int w_rest;
	w_rest = spr->w + SLICE_SIZE;

	unsigned int count4;
	#if 0
	//count4 = (((spr->w / SLICE_SIZE)+1)*4);
	#else
	count4 = 0;
	unsigned int i;
	i = 0;
	do
	{
		count4 += 4;
		i += SLICE_SIZE;
		w_rest -= SLICE_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count4 * sizeof(Vertex_uvcxyz_C32));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	for (; i<count4; )
	{
	//	if (((i)+1*4) < (count4))	{}	else		/* 最後 */
		if (i==(count4-4))	/* 最後 */
		{
			w_size = w_rest;
		}
		unsigned int pos_w_size = (pos + w_size);
		vertices[(i)].u 		= spr->tx + pos;
		vertices[(i)].v 		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color 	= blendlevel;
		#endif
		vertices[(i)].x 		= x_pos + pos;
		vertices[(i)].y 		= y_pos;
		vertices[(i)].z 		= SPR_PRIORITY;
	//
		vertices[(i)+1].u		= spr->tx + (pos_w_size);
		vertices[(i)+1].v		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color	= blendlevel;
		#endif
		vertices[(i)+1].x		= x_pos + (pos_w_size);
		vertices[(i)+1].y		= y_pos;
		vertices[(i)+1].z		= SPR_PRIORITY;
	//
		vertices[(i)+2].u		= spr->tx + pos;
		vertices[(i)+2].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+2].color	= blendlevel;
		#endif
		vertices[(i)+2].x		= x_pos + pos;
		vertices[(i)+2].y		= y_pos + spr->h;
		vertices[(i)+2].z		= SPR_PRIORITY;
	//
		vertices[(i)+3].u		= spr->tx + (pos_w_size);
		vertices[(i)+3].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+3].color	= blendlevel;
		#endif
		vertices[(i)+3].x		= x_pos + (pos_w_size);
		vertices[(i)+3].y		= y_pos + spr->h;
		vertices[(i)+3].z		= SPR_PRIORITY;
	//
		pos += SLICE_SIZE;
		/* --- 回転拡大処理 */
		int center_x;
		int center_y;
		center_x = ((spr->x256>>8)		) + (spr->w >> 1);/*/2*/
		center_y = ((spr->y256>>8)		) + (spr->h >> 1);/*/2*/
		#if (1==USE_ZOOM_XY)
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->zoom_x256) * ((65536/256));	/* 拡大率は0-256倍なので0-65536倍へ変換。 */
	//	zoom_y256 = (spr->zoom_y256) * ((65536/256));	/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_x256 = (spr->zoom_x256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_y256 = (spr->zoom_y256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		#else //(0==USE_ZOOM_XY)
		int zoom_xy256;
	//	zoom_xy256 = (spr->zoom_xy256) * ((65536/256)); /* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_xy256 = (spr->zoom_xy256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		#endif/* (1==USE_ZOOM_XY) */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
			ifx -= center_x;
			ify -= center_y;
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* [右CW]右が 0度で時計回りの角度系 */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [下CCW]下が 0度で反時計回りの角度系 */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [左CCW(?)]左が 0度で反時計回りの角度系 */
			irx = ((ifx * sin_angle) - (ify * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle) + (ify * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
//
			#if (1==USE_ZOOM_XY)
			irx = ((irx * (zoom_x256))>>8);
			iry = ((iry * (zoom_y256))>>8);
			#else //(0==USE_ZOOM_XY)
			irx = ((irx * (zoom_xy256))>>8);
			iry = ((iry * (zoom_xy256))>>8);
			#endif/* (1==USE_ZOOM_XY) */
		//	irx = (irx >>8/*16*/);	//	rx = rx / (256.0/*65536.0*/);
		//	iry = (iry >>8/*16*/);	//	ry = ry / (256.0/*65536.0*/);

			irx += center_x;
			iry += center_y;

			vertices[(i)+j].x = (short)irx;
			vertices[(i)+j].y = (short)iry;
		}
		i += 4;
	}
	/* --- 描画リクエスト */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), NULL, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (count4), NULL, vertices);
}

/*---------------------------------------------------------
	フレームタイミングによる全描画
---------------------------------------------------------*/

#if 000
static /*int*/UINT8 texture_cacheId = 127/*-1*/;
static void trans_texture(void)
{
	MY_DIB_SURFACE *texture_cache_ptr;
	//	texture_cache_ptr = NULL/*0*/;
	//	texture_cache_ptr = ggg_Sprites[i].Texture_ptr;
		texture_cache_ptr = my_texture[texture_cacheId];
		/* - テクスチャ転送コマンド */
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		#if (1==USE_COLOR_16_AND_32)
		if (/*g_scr.*/ texture_cache_ptr->format->BitsPerPixel == 16)
		{
			sceGuTexMode(GU_PSM_5551, 0, 0, /*1*/(USE_SWIZZLE)/*0 swizzle*/);
		}
		else
		#endif
		{
			sceGuTexMode(GU_PSM_8888, 0, 0, /*1*/(USE_SWIZZLE)/*0 swizzle*/);
		}
		sceGuTexImage(0,
			 512/*texture_cache_ptr->w*/,
			 512/*texture_cache_ptr->h*/,
			 512/*texture_cache_ptr->w*/,
			 texture_cache_ptr->MY_DIB_DATA);
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
//test		sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	//	sceGuTexScale(1.0f,1.0f);
	//	sceGuTexOffset(0.0f,0.0f);
}
#endif

//	/*extern*/ int debug_view_objs;
static void blit_all_objects(TGameSprite *aaa_Sprites/*void*/ /*TGameScreen *pclass*/)
{
//const TGameScreen *pclass =g_scr;

//	debug_view_objs=0;
	/* --- 汎用スプライト描画 */
	//for (i=0; i<SPRITEMAX; i++)
	offs_t i;
	i = SPRITEMAX;
	do
	{
		i--;/*註：先引き*/
		if (/*g_scr.*/ aaa_Sprites[i]./*->*/used == 1/*TR UE*/)
		{
		//	if ( aaa_Sprites[i].texture_id	!= 127/*-1*/)
		//	if ( aaa_Sprites[i].Texture_ptr != NULL)
			{
//				debug_view_objs++;
				/* --- テクスチャの転送が必要なら */
#if 000
				{
					if (( aaa_Sprites[i].texture_id  != texture_cacheId) /*||
						( aaa_Sprites[i].Texture_ptr != texture_cache_ptr)*/)
					{
						texture_cacheId   = aaa_Sprites[i].texture_id;
						trans_texture();
					}
				}
#endif
				/* --- 汎用スプライトの描画 */
				if (aaa_Sprites[i].used == 1/*TR UE*/)
				{
					//if (aaa_Sprites[i].Texture_ptr != NULL)
					{
						if ((/*g_scr.*/ aaa_Sprites[i]./*->*/rotation_z == 0/*.0*/) &&
							#if (1==USE_ZOOM_XY)
							(/*g_scr.*/ aaa_Sprites[i]./*->*/zoom_x256 == 256/*1.0*/) &&
							(/*g_scr.*/ aaa_Sprites[i]./*->*/zoom_y256 == 256/*1.0*/)
							#else //(0==USE_ZOOM_XY)
							(/*g_scr.*/ aaa_Sprites[i]./*->*/zoom_xy256 == 256/*1.0*/)
							#endif/* (1==USE_ZOOM_XY) */
						)
						{
							/* 回転拡大縮小なし、単純矩形コピー */
							render_object_no_rot_zoom( &aaa_Sprites[i]);
						}
						else
						{
							/* 回転拡大縮小あり */
							render_object_use_rot_zoom( &aaa_Sprites[i]);
						}
					}
				}
			}
		}
	} while (0 != i);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static	int ggg_SpriteSerial;

/*---------------------------------------------------------
	インクリメントで汎用スプライトを渡す
---------------------------------------------------------*/

TGameSprite *TGameScreen_GetSpriteSerial(void/*TGameScreen *pclass*/)
{
	if (/*pclass*/ggg_SpriteSerial < SPRITEMAX)
	{
		/*pclass*/ggg_SpriteSerial++;
	}
	return (/*pclass*/ /*g_scr->*/&ggg_Sprites[(/*pclass*/ggg_SpriteSerial - 1)]);
}

/*---------------------------------------------------------
	汎用スプライトの全クリア
---------------------------------------------------------*/

void TGameScreen_ClearSprite(void/*TGameScreen *pclass*/)
{
	int i;
	for (i=0; i<SPRITEMAX; i++)
	{
		/*pclass*/ /*g_scr->*/ggg_Sprites[i]./*->*/used =  0/*FA LSE*/;
	}
	/*pclass*/ggg_SpriteSerial = 0;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;

enum
{
	OBJ_BANK_00_TAMA = 0,
	OBJ_BANK_01_REIMU_A,
	OBJ_BANK_02_REIMU_B,
	OBJ_BANK_03_MARISA_A,
	OBJ_BANK_04_MARISA_B,
	OBJ_BANK_05_REMILIA,
	OBJ_BANK_06_YUYUKO,
	OBJ_BANK_07_CIRNO_A,
	OBJ_BANK_08_CIRNO_Q,
	OBJ_BANK_06_ITEM,
	OBJ_BANK_07_FRONT_BANK0,
//	OBJ_BANK_07_FRONT_BANK1,
//	OBJ_BANK_07_FRONT_BANK2,
//	OBJ_BANK_08_PANEL,
//	OBJ_BANK_09_EFFECT, /*[予定]*/
	OBJ_BANK_0a_TEKI_STAGE1,
//	OBJ_BANK_0b_TEKI_STAGE2,
//	OBJ_BANK_0c_TEKI_STAGE3,
//	OBJ_BANK_0d_TEKI_STAGE4,
//	OBJ_BANK_0e_TEKI_STAGE5,
//	OBJ_BANK_0f_TEKI_STAGE6,
//	OBJ_BANK_10_TEKI_STAGE7_dummy,
//	OBJ_BANK_11_TEKI_STAGE8,
	OBJ_BANK_MAX
};
#define 	OBJ_BANK_SIZE (8*8)

typedef struct
{
	unsigned char	u;
	unsigned char	v;
//
	unsigned char	w;
	unsigned char	h;
} VIRTUAL_OBJ_STATE;

#include "obj_table.h"
void common_transfer_objects(int num, int obj_group, VIRTUAL_OBJ_STATE *head_obj_status_table)
{
	TGameSprite *obj;
	SPRITE *sss = sprite_list000_head;/* リスト式スプライトで、リストの先頭 から探す */
	while (NULL != sss)/* リスト式スプライトで、リストの最後まで調べる */
	{
		if (/*S P_BULLET*/obj_group/*S P_GROUP_BULLETS*/ & sss->type)
		{
			/* --- オブジェクトポインタ受け取り */
			obj 				= TGameScreen_GetSpriteSerial();
			obj->used			= 1;					/*TR UE*/
		//	obj->alpha			= sss->alpha;			/* α値(0xff==255 で不透明、0 で透明) */
			obj->color8888		= sss->color32; 		/* α値(0xff==255 で不透明、0 で透明) */
//			obj->texture_id 	= TEX_06_BULLET;//sss->m_texture_id ;

			/* --- オブジェクト表示設定 */
			obj->x256			= ((int)sss->x256); 	/*>>8*/ /*/(2 *256)*/  /*+(56)*/ /*+ 80*/
			obj->y256			= ((int)sss->y256); 	/*>>8*/ /*/(2 *256)*/  /*+ 16*/
		//	if (sss->m_PosCenter == 1/*TR UE*/)
		//	{
		//		obj->x -= (sss->w / 2);
		//		obj->y -= (sss->h / 2);
		//	}
			{
				int tama_index = (SP_GROUP_SUB_TYPE_128 & sss->type);
				obj->w			= head_obj_status_table[tama_index].w;		/*((sss->w128)>>7)*/
				obj->h			= head_obj_status_table[tama_index].h;		/*((sss->h128)>>7)*/
				obj->tx 		= head_obj_status_table[tama_index].u;		/*1*/	//sss->texture_x;
				obj->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
			}
			#if (1==USE_ZOOM_XY)
			obj->zoom_x256	= sss->m_zoom_xy256;		/* 拡大率 0x100==256 が x1.0倍 */
			obj->zoom_y256	= sss->m_zoom_xy256;		/* 拡大率 0x100==256 が x1.0倍 */
			#else //(0==USE_ZOOM_XY)
		//	obj->zoom_xy256 = 256;//sss->m_zoom_xy256;		/* 拡大率 0x100==256 が x1.0倍 */
			obj->zoom_xy256 = sss->m_zoom_x256; 			//sss->m_zoom_xy256;	/* 拡大率 0x100==256 が x1.0倍 */
			#endif/* (1==USE_ZOOM_XY) */
			/* 描画用角度(下が0度で左回り(反時計回り)) */
			obj->rotation_z = (sss->m_angleCCW512/*<<(7)*/);	/*m_RollZ*/ 	/* 角度は0-512度 */ 	/* 角度は0-65535度 */
//
			#if (1==USE_ZBUFFER)
			obj->priority	= sss->m_priority;			/*aaa*/ 	/* 表示優先順位 */
			#endif/* (1==USE_ZBUFFER) */
		//	s->type = SP_DELETE;
		}
		sss = sss->next;/*次*/
	}
}

#include "gu_render.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

extern int draw_script_screen;					/* せりふウィンドウ表示フラグ */
/*extern*/ int msg_time;						/* せりふウィンドウ表示時間(仮) */
static void gu_draw_script_window(void)
{
	if (0 != draw_script_screen)
	{
		gu_draw_rect_window(HAIKEI_03_MESSAGE);
	}
	else
	if (0==spell_card_mode)
	{
		if (0 != pd_bomber_time)
		{
			gu_draw_rect_window(HAIKEI_01_JIKI_SPELL_CARD);
		}
	}
	else
	{
		gu_draw_rect_window(HAIKEI_00_BOSS_SPELL_CARD);
	}
//
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdou ble buffer???*/
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	sceGuTexImage(0, 512, 512, 512, msg_window_image);
	gu_draw_serifu_screen();
}


/*---------------------------------------------------------
	テクスチャをセット
	-------------------------------------------------------
	テクスチャ設定コマンド(sceGuTexImage)は内部で、キャッシュ(GPU)が
	フラッシュバックされ、速度低下します。
	使い過ぎる(100回以下ぐらいなら問題ないかも)と 60fpsを保つのは無理ですので、
	少なくなるようにゲームを設計します。
---------------------------------------------------------*/
static void gu_set_texture(int texture_number)
{
	//	sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);//???
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* 半透明 */
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
			#if (16==USE_BLIT_COLOR_BIT)
	//	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_4444/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
		sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5650/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
			#else
		sceGuTexMode(/*GU_PSM_5551*/GU_PSM_8888/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
			#endif
	//	sceGuTexImage(0, TW128, TH128, 512, bullet_image);
	//	sceGuTexImage(0, TW128/2, TH128/2, TW128/2/*WK512*/, my_texture[TEX_06_BULLET]->pixels);
	//	sceGuTexImage(0, TW128, TH128, TW128/*WK512*/, my_texture[TEX_00_BACK_GROUND]->pixels);
	//	sceGuTexImage(0, 256, 256, 256/*WK512*/, ((my_resource[texture_number].my_texture)->pixels));
		sceGuTexImage(0,
			(my_resource[texture_number].texture_width),
			(my_resource[texture_number].texture_height),
			(my_resource[texture_number].buffer_width), 	/*WK512*/
			((my_resource[texture_number].my_texture)->pixels));
//test		sceGuTexFilter(GU_LINEAR, GU_LINEAR);
}


/*---------------------------------------------------------
	ゲーム中の描画処理その１
	---------------------------------------------------------
	弾よりプライオリティーが下にあるものをプライオリティー順に描く。
	プライオリティー＃２．BG 画面を描画
	プライオリティー＃３．背景障害物を描画
	プライオリティー＃４．魔方陣を描画
	プライオリティー＃５．jikiを描画
---------------------------------------------------------*/

//static void callback_gu_draw_haikei_all(void)
static void gu_blit_haikei_maho_jiki(void)
{
/* -- １．BG 画面を描画 */
	#if (1)
	{
		gu_set_texture(TEX_00_BACK_GROUND);
	//	sceGuEnable(GU_CLIP_PLANES);
	//	#if (1==USE_F3D)
	//	gu_draw_bg_fake3D();
	//	#else
	//	gu_draw_bg_2D();
	//	#endif
		(callback_gu_draw_haikei)();	//callback_gu_draw_haikei_all();
	//	sceGuDisable(GU_CLIP_PLANES);
	}
	#endif
/* -- ２．背景障害物を描画 */
	#if (1)
	#endif
/* -- ３．魔方陣を描画 */
	#if (1)
	{
		if (0!=draw_boss_hp_value)
		{
			gu_set_texture(TEX_02_MAHOUJIN);	/* テクスチャをセット */
			gu_draw_big_maho_jin();
		}
	}
	#endif
/* -- ４．jikiを描画 */
	#if (1)
//	if (NULL != callback_gu_draw_haikei)	//共用if (0!=dr aw_bg_screen)
	{
		gu_set_texture(TEX_03_JIKI);
		/*jiki_transfer_object();*/
		{	TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_03_JIKI, SP_GROUP_JIKI_GET_ITEM, obj_status_table+(2*4*8*8));
		//	common_transfer_objects(TEX_03_JIKI, SP_GROUP_JIKI_GET_ITEM, obj_status_table+(8*8)+(select_player<<6) );
			common_transfer_objects(TEX_03_JIKI, SP_GROUP_JIKI_GET_ITEM, obj_status_table+(OBJ_BANK_01_REIMU_A*OBJ_BANK_SIZE)+(select_player<<6) );
		}
		blit_all_objects(ggg_Sprites);/*PRIORITY_02_PLAYER*/
	}
	#endif /*(000)*/
}


/*---------------------------------------------------------
	画面描画(汎用)
---------------------------------------------------------*/

#define USE_VSYNC			(0)
/* 0:VSYNC取らない。 USE_VSYNC いわゆるv-sync 1 で on だが、まだ遅いので無理。 */
#define USE_MAX_GU_TIME 	(0)
/* 0:USE_MAX_GU_TIME [Gu時間を最大に取るタイプ](色々制限あり)
	Guの転送を考えて、Gu側でなるべく並列動作させるようにタイミングを考えたもの。
	pspで始めて実装したのは、 http://www.fumi2kick.com/pspapp/ この辺。
	SDL共用する場合(ソフト描画)は、(タイミング的に)絶対に間に合わない。
	(フレームバッファを設けてスワップさせれば間に合う。つまりトリプルバッファ。但しメモリ食うし、遅くなるかも？)
	(Gu描画しかしないなら、1:USE_MAX_GU_TIMEで十分ではある。)
	現状はSDL共用するので、1:USE_MAX_GU_TIMEに出来ない。(SDL使わないなら0:USE_MAX_GU_TIMEに出来る)
*/
extern	void do_input_vbl(void);
void vbl_draw_screen(void)
{
	do_input_vbl();/*キー入力(処理の都合上、ここに移動)*/
	#if (1==USE_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu時間を最大に取るタイプ](色々制限あり) */
//	sceGuFinish();実験
	/* - パケット描画終了待ち */
	sceGuSync(0, 0);/* (ここまで Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* ここから Gu */
	/* --- スクリーンアップデート */
	sceGuStart(GU_DIRECT, gulist );

	/* --- プライオリティー＃１．画面クリア */
	#if 1
	{
		const unsigned int bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/*(黒)*/
			0xff106010,/*1面(緑)*/
			0xff802010,/*2面(青)*/	//	0xff102080,/*2面*/
			0xff104010,/*3面(緑)*/
			0xff402020,/*4面(青)*/
			0xff601030,/*5面(青)*/
			0xff601030,/*6面(青)*/
			0xff000000,/*ending(黒)*/
		//	0xff601010,/**/
		};
		sceGuClearColor(bg_color_list[player_now_stage&0x07]);/*AABBGGRR*/
	//	sceGuClearColor(/*0*/0xff601010/*0xff601010*/);/*AABBGGRR*/
	}
	#endif
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- 画面更新 */

//	sceDisplayWaitVblankStart();/*vsync*/
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN], NULL, SDL_VRAM_SCREEN, NULL);
//	{
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*256) );/*test*/
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*272*2) );
//	}
//	SDL_Flip(SDL_VRAM_SCREEN);

	//#if (0==USE_MAX_TRANS)
	sceKernelDcacheWritebackAll();
	//#endif // (0==USE_MAX_TRANS)
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
//
	/* -- BG背景 画面を描画 */
//	if (1==dr aw_bg_screen) /* pspは0レジスタがあるので0と比較したほうが速い */
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG 画面を描画 */
		/* -- プライオリティー＃２～＃５ 画面を描画 */
		gu_blit_haikei_maho_jiki();
	}
//
/* -- プライオリティー＃６ラインテスト画面を描画 */
	#if (1==LINE_TEST)
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- ラインテスト画面を描画 */
		gu_blit_lines();
	}
	#endif /*(000)*/
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
//
/* -- プライオリティー＃７．SDL 画面を描画 */
	#if (1)
		#if (0)
		/*ちょっとゲーム中SDL抜いてみるテスト*/
		if (NULL == callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
		#endif /*(000)*/
	{
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* 半透明 */
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdou ble buffer???*/
	//
	//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_SUBTRACT, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_SRC_COLOR, GU_DST_COLOR, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f007f7f, 0x3f3f3f00);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0, 0);
	//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
	//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
	//
		sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
		sceGuTexImage(0, 512, 512, 512, render_image);
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*くっきり拡大画面*/
		gu_draw_sdl_screen();
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*ぼやぼや拡大画面*/
	}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
//
/* -- プライオリティー＃８．敵を描画 */
	/* ☆ 敵面エフェクト */
	/* ザコ(敵面エフェクト) */
	/* ボス(敵面エフェクト) */
	#if (1)
	{
		gu_set_texture(TEX_04_TEKI);	/* テクスチャをセット */
		#if (1)
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_04_TEKI, S P_GROUP_BULLETS, obj_status_table+0);
			common_transfer_objects(TEX_04_TEKI, (SP_GROUP_ZAKO/*|SP_GROUP_BOSS*/), obj_status_table+(OBJ_BANK_0a_TEKI_STAGE1*OBJ_BANK_SIZE));
		}
		blit_all_objects(&ggg_Sprites[0]);/*PRIORITY_05_BULLETS*/
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- プライオリティー＃９．ITEMを描画 */
//	/* その他の自機素材(アイテム面エフェクト) */
	#if (1)
	{
		gu_set_texture(TEX_06_BULLET);	/* テクスチャをセット */
		#if (1)
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_06_BULLET, S P_GROUP_BULLETS, obj_status_table+0);
			common_transfer_objects(TEX_06_BULLET, SP_GROUP_ITEMS, obj_status_table+(OBJ_BANK_06_ITEM*OBJ_BANK_SIZE));
		}
		blit_all_objects(&ggg_Sprites[0]);/*PRIORITY_05_BULLETS*/
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- プライオリティー＃１０．敵弾を描画 */
	/* ☆ 弾幕面エフェクト */
	#if (1)
	{
	//	gu_set_texture(TEX_06_BULLET);	/* テクスチャをセット */
		#if (1)
		/* 弾幕リストは分割する予定 なので専用に書き換える予定 */
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_06_BULLET, S P_GROUP_BULLETS, obj_status_table+0);
			common_transfer_objects(TEX_06_BULLET, SP_GROUP_BULLETS, obj_status_table+(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE));
		}
		blit_all_objects(&ggg_Sprites[0]);/*PRIORITY_05_BULLETS*/
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- プライオリティー＃１１．フロント面を描画 */
	/* ☆ フロント面エフェクト */
//	/* 自分のあたり判定位置表示用コア(フロント面エフェクト) */
//	/* 小爆発 / ザコ消滅爆発(フロント面エフェクト) */
//	/* 火炎爆発(フロント面エフェクト) */
	/* ボンバーロゴ(フロント面エフェクト) */
	/* /[コンティニュー文字(bank00)/メニュー文字(bank01)/メニュー文字(bank02)] */
	#if (1)
	{
		gu_set_texture(TEX_07_FRONT);	/* テクスチャをセット */
		/*front_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+0);
			common_transfer_objects(TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+(OBJ_BANK_07_FRONT_BANK0*OBJ_BANK_SIZE));
		}
		blit_all_objects(ggg_Sprites);/*PRIORITY_06_FRONT*/
	}
	/* プライオリティー＃１２．☆ フロント面エフェクト(テクスチャ共用) */
	//{//PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (0 != pd_bomber_time)
	{
		gu_draw_front_spell_logo();
	}
	//}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
//
/* -- プライオリティー＃１３．パネル面を描画 */
	#if (1)
	if (0 != draw_side_panel)
	{
		gu_draw_side_panel();
	}
	#endif /*(000)*/
//
	/* -- プライオリティー＃１４．スクリプト画面を描画 */
//	if (1==draw_script_screen)		/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0 != (draw_script_screen+msg_time))
	{
		if (0 != (msg_time))/* メッセージ(仮対応)表示時間 */
		{
			msg_time--;
		}
		gu_draw_script_window();
	}

	/* -- プライオリティー＃１５．開発デバッグフォント */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - パケット積み、ここまで */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [通常タイプ]  */
	sceGuSync(0, 0);/* (ここまで Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
//
	#else
	//fps_show();
	SDL_Flip(sdl_screen[SDL_00_SCREEN]);
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	//SDL_UpdateRect(sdl_screen[SDL_00_SCREEN], 0, 0, 0, 0);
	#endif
//
	//fps_newframe();

	#if (1==USE_DESIGN_TRACK)
	/* 予約 voice を再生 */
	voice_play_vbl();
	#endif
}


/*---------------------------------------------------------
	画面描画(起動時専用)
---------------------------------------------------------*/

void draw_loading_screen_test(void)
{
	#if (1==USE_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu時間を最大に取るタイプ](色々制限あり) */
//	sceGuFinish();実験
	/* - パケット描画終了待ち */
	sceGuSync(0, 0);/* (ここまで Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* ここから Gu */
	/* --- スクリーンアップデート */
	sceGuStart(GU_DIRECT, gulist );

	/* --- 画面クリア */
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- 画面更新 */

//	sceDisplayWaitVblankStart();/*vsync*/
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN], NULL, SDL_VRAM_SCREEN, NULL);
//	{
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*256) );/*test*/
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*272*2) );
//	}
//	SDL_Flip(SDL_VRAM_SCREEN);

	//#if (0==USE_MAX_TRANS)
	sceKernelDcacheWritebackAll();
	//#endif // (0==USE_MAX_TRANS)
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
//
	/* -- BG背景 画面を描画 */
//	if (1==dr aw_bg_screen) /* pspは0レジスタがあるので0と比較したほうが速い */
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG 画面を描画 */
		gu_blit_haikei_maho_jiki();
	}
//

	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
//
	/* -- 開発デバッグフォント */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - パケット積み、ここまで */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [通常タイプ]  */
	sceGuSync(0, 0);/* (ここまで Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
//
	#else
	//fps_show();
	SDL_Flip(sdl_screen[SDL_00_SCREEN]);
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	//SDL_UpdateRect(sdl_screen[SDL_00_SCREEN], 0, 0, 0, 0);
	#endif
//
	//fps_newframe();
}
