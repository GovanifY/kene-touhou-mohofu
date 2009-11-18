
/*---------------------------------------------------------
	Gu���g������ʕ`�揈��
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

//#define TW128 128/* �e�e�N�X�`���̕� */
//#define TH128 128/* �e�e�N�X�`���̍��� */
//#define WK512 512/* �e�e�N�X�`����`�悷��ꍇ�ɁA�`�惏�[�N�G���A�̕� */

/* �傫�ȃX�v���C�g��`�悷��Ƃ��̕����T�C�Y */
#define SLICE_SIZE	(64)

/* 1:���_�J���[���g���B */
#define USE_VCOLOR 1/*1*/

/* --- GU �`��̂��߂̃p�P�b�g�T�C�Y */
#define PACKET_SIZE  (262144)

/* gulist�ݒ肪����������Ɠs�������� */
#if (PACKET_SIZE<(512*512)) /* shere short16==(512*512/2*short) and int32 ==(512*512*int) */
	#undef	PACKET_SIZE
	#define PACKET_SIZE  (512*512)
#endif

//#define UNKNOWN_MY_FLAGS (GU_COLOR_5650)

	#if (1==USE_VCOLOR)
		/* ���_�J���[���g���ꍇ�B��ʓI�B */
		#if (16==USE_BLIT_COLOR_BIT)
			/* 15/16bit�`�� */
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
			/* 32bit�`�� */
			#define TEXTURE_FLAGS5650	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#endif
	#else
		/* ���_�J���[���g��Ȃ��ꍇ�B�uGU_COLOR�v��t����Ɠ����Ȃ��B */
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


/* �Ǘ�����e�N�X�`���[ */
enum
{
	TEX_00_BACK_GROUND = 0, 	/* [256x256]3D�w�i1 */
//	TEX_01_BACK_TEXTURE,		/* �w�i��Q�� */
	TEX_02_MAHOUJIN,			/* [128x128]�����w */
	TEX_03_JIKI,				/* [256x256]���e/���@ */
	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	TEX_05_ITEM,				/* �A�C�e��/�����X�R�A */
	TEX_06_BULLET,				/* [128x128]�G�e */
	TEX_07_FRONT,				/* [256x256]���@������\��/����/[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
//	TEX_08_SCORE_PANEL, 		/* [TEX_07_FRONT�ƗZ������\��������(256x512�e�N�X�`���H)] �X�R�A�p�l��/�X�R�A�t�H���g���� */
//	TEX_09_TACHIE,				/* �����G */
//	TEX_10_MESSAGE, 			/* [TEX_07_FRONT�ƗZ������\��������(512x512�e�N�X�`���H)]���b�Z�[�W�Œ蕶�� */
	TEXTURE_MAX 				/* --- �Ǘ�����ő�e�N�X�`���[�� */
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
	�O���[�o���ϐ�
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
	�X�N���[���Ǘ�
---------------------------------------------------------*/

static	TGameSprite  ggg_Sprites[SPRITEMAX];

typedef struct
{
	MY_DIB_SURFACE *my_texture; 		/* �e�N�X�`���摜 */
	int 			texture_width;		/* �e�N�X�`���� */
	int 			texture_height; 	/* �e�N�X�`������ */
	int 			buffer_width;		/* 512 �Œ�H */
//
	int 			color_format;		/* �ϊ��ς݉摜�`�� */
	/*	�ǂݍ��ݎ���ARGB8888����摜�ϊ����s�����A
		�ǂ������`���ɉ摜�ϊ����邩�w�肷��B
		16bit�`���ł� ABGR0565 �Ƃ� ABGR1555 �Ƃ� ABGR4444 ������B
	 */
	int 			hh;/*�\��*/
	int 			jj;/*�\��*/
	char			*file_name;/* �t�@�C���� */
} MY_TEXTURE_RESOURCE;

//static	MY_DIB_SURFACE *my_texture[TEXTURE_MAX];
static	MY_TEXTURE_RESOURCE 	my_resource[TEXTURE_MAX];
/*static*/	const MY_TEXTURE_RESOURCE	initial_resource[TEXTURE_MAX] =
{
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//		TEX_00_BACK_GROUND = 0, 	/* 3D�w�i1 */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_01_BACK_TEXTURE,		/* �w�i��Q�� */
	{NULL,128,128,128,	0,0,0,DIRECTRY_NAME_DATA "/effect/mahoujin128.png"},	//		TEX_02_MAHOUJIN,			/* �����w */
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/jiki/jiki00.png"},			//		TEX_03_JIKI,				/* ���e/���@ */
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/zako/teki0_256.png"},		//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_05_ITEM,				/* �A�C�e��/�����X�R�A */
	{NULL,128,128,128,	0,0,0,DIRECTRY_NAME_DATA "/tama/bullet.png"},			//		TEX_06_BULLET,				/* �G�e */
	{NULL,256,256,256,	0,0,0,DIRECTRY_NAME_DATA "/effect/front256.png"},		//		TEX_07_FRONT,				/* ���@������\��/����/[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_08_SCORE_PANEL, 		/* �X�R�A�p�l��/�X�R�A�t�H���g���� */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_09_TACHIE,				/* �����G */
//	{NULL,256,256,512,	0,0,0,DIRECTRY_NAME_DATA "/bg/back0_256.png"},			//	//	TEX_10_MESSAGE, 			/* ���b�Z�[�W�Œ蕶�� */
};

void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;
int draw_side_panel;
int draw_boss_hp_value;
int boss_life_value;

unsigned int conv_bg_alpha;

/*---------------------------------------------------------
	�摜�֘A
---------------------------------------------------------*/

void psp_clear_screen(void)
{
	/* ����Gu�ŕ`�����ꍇ�B�n�[�h�E�F�A�[�@�\�ŁA�u����������̂ō��̂����ɂ܂Ƃ߂Ƃ� */
	SDL_FillRect(sdl_screen[SDL_00_SCREEN],NULL,SDL_MapRGB(sdl_screen[SDL_00_SCREEN]->format,0,0,0));
}
//void psp_move_screen(SDL_Surface *src_screen, SDL_Surface *dst_screen )
void psp_move_screen(int src_screen_number, int dst_screen_number )
{
	/* ����Gu�ŕ`�����ꍇ�B�n�[�h�E�F�A�[�@�\�ŁA�u����������̂ō��̂����ɂ܂Ƃ߂Ƃ� */
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
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); 	}	/* ���b�N */
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
	/* �_�� */
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
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}	/* ���b�N���� */
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
		gu_draw_bg_fake3D,	/*1��"���@�̐X"*/
		gu_draw_bg_2D,		/*2��"�H�߂���"*/
		gu_draw_bg_eientei, /*3��"�|��"*/
		gu_draw_bg_eientei, /*4��"�i���� �L��"*/
		gu_draw_bg_fake3D,	/*5��"�g���� ����"*/
		gu_draw_bg_fake3D,	/*6��"�g���� ��L��"*/
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
		/*SDL_SWSURFACE*/SDL_HWSURFACE,/*VRAM��*/
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
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
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
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
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



	//# /* �J�X�^�����C�u������Gu���g���ꍇ */

#if (1==USE_16BIT_BLIT)
	#define  SCREEN_DEPTH	16
#else
	#define  SCREEN_DEPTH	32
#endif

	/* setup GU initialise */
	sceGuInit();
	#if 0
	/* �����Ă��w�Ǔ��� */
	sceGuDisplay(GU_FALSE);/*���OFF*/
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
	/* �`��͈͂�ݒ肷�� */
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
	/*�u�A���t�@�u�����f�B���O(�����x�t���A�F�d�ˍ��킹)�v*/
	sceGuAlphaFunc(GU_ALWAYS,0,0xff);//sceGuAlphaFunc(GU_LEQUAL, 0, 0x01);
	sceGuEnable(GU_ALPHA_TEST); 	/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);			/* �L�� */
	//sceGuDisable(GU_ALPHA_TEST);			/* ���� */	/*noiz07*/
	#endif/*???*/
#endif/*000???*/

	#if (1==USE_ZBUFFER)
	//sceGuEnable(GU_DEPTH_TEST);			/* �L�� */
	//sceGuDisable(GU_DEPTH_TEST);			/* ���� */	/*noiz07*/
	sceGuDisable(GU_DEPTH_TEST);
	/* psp �� z�l�� 65535(�߂�) ���� 0(����)�ŁAZ�l���ʏ�Ƌt�B */
	/* ����z�l�� unsigned short �Ȃ̂ŁA�����͂ݏo���ƁA��荞��ŕςȕ`��ɂȂ��Ă��܂��B */
	/* �����ŁA��荞�܂Ȃ��悤�ɁA�K���ɗ]�T�������U���BPSPSDK �̃T���v���ł� */
	/* 0xc350(50000) �߂����̋��E�ʁA0x2710(10000)�������̋��E�� �ƂȂ��Ă��邪�A�K���Ȓl�Ȃ牽�ł��悢 */
	/* sceGuDepthRange(0xc350, 0x2710); */
	#define  Z_CLIP_LENGTH	(10000)
	sceGuDepthRange((0xffff-Z_CLIP_LENGTH), Z_CLIP_LENGTH);
//	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	#if 1
	/* �J�����O���[�h(�������|���S���͕`�悵�Ȃ�)�ŁA�L�����W�n(�E����W�n�ɂ��邩������W�n�ɂ��邩)�����߂� */
	sceGuEnable(GU_CULL_FACE);	/* �J�����O�L��(�������|���S���͕`�悵�Ȃ�) */
	sceGuFrontFace(GU_CW);		/* clock wise(���v����) 		  �E���A�E����W�n(OpenGL �W��?)	 */
//	sceGuFrontFace(GU_CCW); 	/* counter clock wise(�����v����) �����A������W�n(Direct X �W��?) */
/*
  ���v���(�Ƃ����܂��)�Ƃ����̂́A���v�̐j���i�ޕ����Ɠ�����]�����̎��ł���B
�と�E���������A���邢�͖k�������쁨���̏��ɉ�]��������ł���B�E���Ƃ�������B
  �t�̉�]�������u�����v���v(�͂�Ƃ����܂��)�Ƃ����B���l�ɁA����͍����ƌ�����B
�����v���͉p�ꌗ�ł� counterclockwise �ƌĂ΂�邪�A����� CCW �Ɨ����ꍇ�������B

  ���v���̉�]�����͓����v��͂��Ă���B�����v�͖k��A�����k�Ŕ������ꂽ�B
�����v�̉�]�����́A�k��A�����k�ɂ����Ă͎��v���ɂȂ邪��ł͔����v���ɂȂ�B

  ����(���ʊp)�́A�k��0���Ƃ��Ď��v���ɐ��𑝂₷���A
�t�ɐ��w�ɂ����ẮA���ʊp�͔����v���̕����𐳂Ƃ��Čv�������B

  �r�̂ӂ���l�W���́A���v���ɉ񂷂ƒ��܂�A�����v���ɉ񂷂Ɗɂނ悤�ɍ���Ă��邪�A
�����͉E�����̏ꍇ�A���v���̕����͂�����Ղ��Ƃ������R�ɂ��B
�͂̓��ꂷ���Ŕj������̂�h���ׂɃK�X�ǂł͂����Ƃ͋t�����ɂȂ��Ă���B

���[�Ȃ̂��[
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
	sceGuEnable(GU_BLEND);			/* �A���t�@�u�����f�B���O�L�� */
	#endif/*???*/

	#if 1
	/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
	sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�� */
	#else
	/* �e�N�X�`���[�������[�h�ł́A����ʓ��̃t���b�g�|���S�����g����(�e�N�X�`���[�|���S���A2D�摜�]�����͏o���Ȃ�) */
	sceGuDisable(GU_TEXTURE_2D);	/* �e�N�X�`���[���� */
	#endif

	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);//Z_BUFFER
/*??*/	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);	/*noiz07*/
	#endif/* (1==USE_ZBUFFER) */
//	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);	/*noiz07*/

	#if 1
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* ������ */
	#else
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);/* �������s�H */
	#endif

//	sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����(�g�厞�o�Ȑ��⊮)*/
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����(�g�厞�����⊮)*/
	sceGuTexScale(1.0f, 1.0f);//sceGuTexScale(1,1);
	sceGuTexOffset(0.0f, 0.0f);
//
	/* ���F�����߂�H */
	sceGuTexEnvColor(	0x00000000);
//	sceGuTexEnvColor(	0x00ffff00);
	sceGuAmbientColor(	0xffffffff);
	sceGuColor( 		0xff000000);

//


//	u8	ColorKey	= 1;
/* �J���[�L�[��ݒ肷�� */
//	if (ColorKey)
	{
//		if (!sceGuGetStatus(GU_COLOR_TEST))
		{	sceGuEnable(GU_COLOR_TEST);}/*�J���[�L�[�L��*/
	//	sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00fefefe);
		sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00ffffff);
	//	sendCommandi(216, GU_NOTEQUAL & 0x03);
	//	sendCommandi(217, 0 & 0xffffff);
	//	sendCommandi(218, 0x00fefefe);
	}
//	else
//	{
//		if (sceGuGetStatus(GU_COLOR_TEST))
//		{	sceGuDisable(GU_COLOR_TEST);}/*�J���[�L�[����*/
//	}
	#if 1
	sceGuEnable(GU_CLIP_PLANES);
	#endif
//
	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/*vsync*/
	sceGuDisplay(GU_TRUE/*1*/);/*���ON*/
	/* �����܂ŏ����ݒ� */

//	sceDisplayWaitVblankStart();/*vsync*/

	/* --- ���͑��u�̏����ݒ� */
	sceCtrlSetSamplingCycle(0); 	/*???*/
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);	/* �A�i���O�L�[�L��(�W���ł̓f�W�^���̂�) */

	//static TGameTexture *TGameTexture_Create(void)
	{
		unsigned int i;
		/* --- �ėp�X�v���C�g�}�l�[�W���̏����� */
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
		/* --- �e�N�X�`���}�l�[�W���̏����� */
		for (i=0; i<TEXTURE_MAX; i++)
		{
		//	my_resource[i].my_texture = NULL;
			my_resource[i].my_texture		= initial_resource[i].my_texture;
			my_resource[i].texture_width	= initial_resource[i].texture_width;
			my_resource[i].texture_height	= initial_resource[i].texture_height;
			my_resource[i].buffer_width 	= initial_resource[i].buffer_width;
		//
			my_resource[i].color_format 	= initial_resource[i].color_format;
			my_resource[i].hh				= initial_resource[i].hh;/*�\��*/
			my_resource[i].jj				= initial_resource[i].jj;/*�\��*/
			my_resource[i].file_name		= initial_resource[i].file_name;
			/* --- �e�N�X�`���̏����� */
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

	/* --- ���̑��̏����ݒ� */
	draw_side_panel 		= 0;
	draw_boss_hp_value		= 0;
//	callback_gu_draw_haikei = NULL;//dr aw_bg_screen = 0;
	conv_bg_alpha			= 0xffffffff;
//	conv_bg_alpha			= 0x7f7f7f7f;
//

	/* --- ���[�f�B���O��� */
	player_now_stage		= 0;
	stage_bg_load_surface();
	draw_loading_screen_test();
}
/*-------------*/
/*-------------*/

/*---------------------------------------------------------
	�e�N�X�`�������[�h����
---------------------------------------------------------*/

#define USE_T128_SWIZZLE 0

//#if 1//(1==USE_COLOR_16_AND_32)
#if (1==USE_COLOR_16_AND_32)
static void convert_swizzle
(
		  u8	*out_data,		/* �o�͉摜�̐擪�|�C���^ */
	const u8	*in_data,		/* ���͉摜�̐擪�|�C���^ */
/* (�Ⴆ�΁A512x512�e�N�X�`���[�̏ꍇ) */
	const u32	width_x_size,	/* ��������f�[�^�[�T�C�Y */	/*max: 512*4 */
	/* (�Ⴆ�΁AARGB8888 �Ȃ� 512*4==width_x_size) */
	/* (�Ⴆ�΁AARGB1555 �Ȃ� 512*2==width_x_size) */
	const u32	height			/* ���� */		/*max: 512*/
	/* (�Ⴆ�΁A512==height) */
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
	�e�N�X�`���[�̓ǂݍ��݁A�o�^
---------------------------------------------------------*/

static void TGameTexture_Load_Surface( int num )	/*, char *file_name*/
{
	/* ----- �e�N�X�`���ԍ����s����������I�� */
//	if (num < 0) return;
//	if (num > TEXTURE_MAX) return;

	/* ----- ���Ƀe�N�X�`�������������� */
	if (NULL != my_resource[num].my_texture)
	{
		#if (1==USE_SDL_image)
		SDL_FreeSurface(my_resource[num].my_texture);
		#else
		png_free_my_image(my_resource[num].my_texture);
		#endif
		my_resource[num].my_texture = NULL;
	}

	/* ----- �e�N�X�`���[�̓ǂݍ��� */
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

	/* --- PSP �����Ƀe�N�X�`�������H���� */

	/* --- PSP�� 16byte align �ɖ����� �s��������(�x��?) �炵���B */
	/* --- ������ memalign(); ���g���A���������E�����킹��B */
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
	/* --- PSP �ŊȈ�16�r�b�g�F���w�肵���ꍇ��16bit�ɗ��Ƃ� */
	#if (1==USE_SDL_image)
	if (my_texture[num]->format->BytesPerPixel == 4)	/* PSP16bit mode && DIB==32bit color */
	#else
	if (/*BytesPerPixel*/4 == 4)	/* PSP16bit mode && DIB==32bit color */
	#endif
	//if (0)
	{
	unsigned short *pixdst;
	unsigned long  *pixsrc;
		/* --- 16bit�F�� ���F���ĕێ� */
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
			/* --- 15bit�F�� ���F���ĕێ� (color key �g�p��) */
			*pixdst =
				((*pixsrc & 0x80000000) >> 16) |
				((*pixsrc & 0x00f80000) >>	9) |
				((*pixsrc & 0x0000f800) >>	6) |
				((*pixsrc & 0x000000f8) >>	3);
		//	*pixdst = MAKECOL15A(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc), GETA32(*pixsrc));
			#else
			/* --- 16bit�F�� ���F���ĕێ� (color key �g�p�s��) */
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
	// 16bit�F
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 2 */  my_map_TW128/*512*/*2, 	/* short ������ 2�{ */
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
	}
	else/* 32bit mode */
#endif
	{
		/* --- 32bit�F �ʏ탂�[�h */
		((my_resource[num].my_texture)->MY_DIB_DATA) = (void*)memalign(16, msize);
		memcpy(/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/, nonalign, msize);
		#if (1)
	// 32bit�F
		convert_swizzle(
			(u8*)((my_resource[num].my_texture)->MY_DIB_DATA),
			(const u8*)gulist/*convert_works*/,
			/* texture width x 4 */  my_map_TW128/*512*/*4, 	/* int ������ 4�{ */
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#endif
	}
	free(nonalign);
}

/*---------------------------------------------------------
	�ėp�X�v���C�g�̕\�� (��]�g��k���Ȃ�)���[�`��
	�ėp�X�v���C�g�̕\��(��ʓ]��)
---------------------------------------------------------*/

#if (1==USE_ZBUFFER)
	/* �n�[�h�E�F�A�[�Ń\�[�g */
	#define SPR_PRIORITY	(spr->priority)
#else
	/* �\�t�g�E�F�A�[�� Z�\�[�g */
	#define SPR_PRIORITY	(0)
#endif
/* --- �ėp�X�v���C�g���ꖇ�X�N���[���ɓ\��t���� */
static void render_object_no_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;

	/* --- �����������l */
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

	/* --- �ėp�X�v���C�g���ꖇ�`�悷�� */
	/* ������x�̑傫�����������X�v���C�g�͒Z����ɕ������Ȃ��� */
	/* �ƂĂ��Ȃ��x���Ȃ� */

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
	//	if (((i)+1*2) < (count2))	{}	else		/* �Ō� */
		if (i==(count2-2))		/* �Ō� */
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
	/* --- �`�惊�N�G�X�g */
//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS4444, (count2), NULL, vertices);
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650_C32, (count2), NULL, vertices);
}

/*---------------------------------------------------------
	�ėp�X�v���C�g�̕\�� (��]�g��k������)���[�`��
	-------------------------------------------------------
	����(2009-11-13����)������ slice ���ĂȂ����ǁA�ǂ����c����
	 slice ���Ȃ���(�`���m�⃌�~���A�{���̑��x�ቺ���Ղ肩�画�f����)
	�_���ł��ˁB���Ƃŉ��ǂ��悤�B
	-------------------------------------------------------
	�t�ɒe����(64[dots]�ȉ��� > ��e���炢�Ȃ�) slice ���Ȃ��Ă�
	�����̂�������Ȃ��B
	�e���̓V�X�e���I�ɂ��u�ėp�X�v���C�g�ŕ`���Ȃ������������ǂ��v�̂ŁA
	��p���[�`���ɕ��򂷂�\��(2009-11-13����)�ł��B
---------------------------------------------------------*/

static void render_object_use_rot_zoom(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- �\���X�C�b�` */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- ��]�g��O���� */
	int sin_angle;	/* �ʑ�  +0�x�i�� */
	int cos_angle;	/* �ʑ�  +90�x�i�� */
//	int c18_angle;	/* �ʑ� +180�x�i�� */
	{
		/* �p�x��0-511�x */
		unsigned int rotation_angle512;
	//	rotation_angle512	= ((spr->rotation_z) / (128/*65536/512*/)); 	/* �p�x��0-65535�x�Ȃ̂�0-511�x�֕ϊ��B */
		rotation_angle512	= ((spr->rotation_z) /*>> (7)*/);
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle512)&(512-1))]/*<<8*/);
	}
//
	/* --- �����������l */
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

	/* --- �ėp�X�v���C�g���ꖇ�`�悷�� */
	/* ������x�̑傫�����������X�v���C�g�͒Z����ɕ������Ȃ��� */
	/* �ƂĂ��Ȃ��x���Ȃ� */

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
	//	if (((i)+1*4) < (count4))	{}	else		/* �Ō� */
		if (i==(count4-4))	/* �Ō� */
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
		/* --- ��]�g�又�� */
		int center_x;
		int center_y;
		center_x = ((spr->x256>>8)		) + (spr->w >> 1);/*/2*/
		center_y = ((spr->y256>>8)		) + (spr->h >> 1);/*/2*/
		#if (1==USE_ZOOM_XY)
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->zoom_x256) * ((65536/256));	/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
	//	zoom_y256 = (spr->zoom_y256) * ((65536/256));	/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_x256 = (spr->zoom_x256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_y256 = (spr->zoom_y256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		#else //(0==USE_ZOOM_XY)
		int zoom_xy256;
	//	zoom_xy256 = (spr->zoom_xy256) * ((65536/256)); /* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
		zoom_xy256 = (spr->zoom_xy256) /*<< (8)*/;		/* �g�嗦��0-256�{�Ȃ̂�0-65536�{�֕ϊ��B */
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
			/* [�ECW]�E�� 0�x�Ŏ��v���̊p�x�n */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [��CCW(?)]���� 0�x�Ŕ����v���̊p�x�n */
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
	/* --- �`�惊�N�G�X�g */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), NULL, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (count4), NULL, vertices);
}

/*---------------------------------------------------------
	�t���[���^�C�~���O�ɂ��S�`��
---------------------------------------------------------*/

#if 000
static /*int*/UINT8 texture_cacheId = 127/*-1*/;
static void trans_texture(void)
{
	MY_DIB_SURFACE *texture_cache_ptr;
	//	texture_cache_ptr = NULL/*0*/;
	//	texture_cache_ptr = ggg_Sprites[i].Texture_ptr;
		texture_cache_ptr = my_texture[texture_cacheId];
		/* - �e�N�X�`���]���R�}���h */
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
	/* --- �ėp�X�v���C�g�`�� */
	//for (i=0; i<SPRITEMAX; i++)
	offs_t i;
	i = SPRITEMAX;
	do
	{
		i--;/*���F�����*/
		if (/*g_scr.*/ aaa_Sprites[i]./*->*/used == 1/*TR UE*/)
		{
		//	if ( aaa_Sprites[i].texture_id	!= 127/*-1*/)
		//	if ( aaa_Sprites[i].Texture_ptr != NULL)
			{
//				debug_view_objs++;
				/* --- �e�N�X�`���̓]�����K�v�Ȃ� */
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
				/* --- �ėp�X�v���C�g�̕`�� */
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
							/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
							render_object_no_rot_zoom( &aaa_Sprites[i]);
						}
						else
						{
							/* ��]�g��k������ */
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
	�C���N�������g�Ŕėp�X�v���C�g��n��
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
	�ėp�X�v���C�g�̑S�N���A
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
//	OBJ_BANK_09_EFFECT, /*[�\��]*/
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
	SPRITE *sss = sprite_list000_head;/* ���X�g���X�v���C�g�ŁA���X�g�̐擪 ����T�� */
	while (NULL != sss)/* ���X�g���X�v���C�g�ŁA���X�g�̍Ō�܂Œ��ׂ� */
	{
		if (/*S P_BULLET*/obj_group/*S P_GROUP_BULLETS*/ & sss->type)
		{
			/* --- �I�u�W�F�N�g�|�C���^�󂯎�� */
			obj 				= TGameScreen_GetSpriteSerial();
			obj->used			= 1;					/*TR UE*/
		//	obj->alpha			= sss->alpha;			/* ���l(0xff==255 �ŕs�����A0 �œ���) */
			obj->color8888		= sss->color32; 		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
//			obj->texture_id 	= TEX_06_BULLET;//sss->m_texture_id ;

			/* --- �I�u�W�F�N�g�\���ݒ� */
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
			obj->zoom_x256	= sss->m_zoom_xy256;		/* �g�嗦 0x100==256 �� x1.0�{ */
			obj->zoom_y256	= sss->m_zoom_xy256;		/* �g�嗦 0x100==256 �� x1.0�{ */
			#else //(0==USE_ZOOM_XY)
		//	obj->zoom_xy256 = 256;//sss->m_zoom_xy256;		/* �g�嗦 0x100==256 �� x1.0�{ */
			obj->zoom_xy256 = sss->m_zoom_x256; 			//sss->m_zoom_xy256;	/* �g�嗦 0x100==256 �� x1.0�{ */
			#endif/* (1==USE_ZOOM_XY) */
			/* �`��p�p�x(����0�x�ō����(�����v���)) */
			obj->rotation_z = (sss->m_angleCCW512/*<<(7)*/);	/*m_RollZ*/ 	/* �p�x��0-512�x */ 	/* �p�x��0-65535�x */
//
			#if (1==USE_ZBUFFER)
			obj->priority	= sss->m_priority;			/*aaa*/ 	/* �\���D�揇�� */
			#endif/* (1==USE_ZBUFFER) */
		//	s->type = SP_DELETE;
		}
		sss = sss->next;/*��*/
	}
}

#include "gu_render.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

extern int draw_script_screen;					/* ����ӃE�B���h�E�\���t���O */
/*extern*/ int msg_time;						/* ����ӃE�B���h�E�\������(��) */
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
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*�����dou ble buffer???*/
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	sceGuTexImage(0, 512, 512, 512, msg_window_image);
	gu_draw_serifu_screen();
}


/*---------------------------------------------------------
	�e�N�X�`�����Z�b�g
	-------------------------------------------------------
	�e�N�X�`���ݒ�R�}���h(sceGuTexImage)�͓����ŁA�L���b�V��(GPU)��
	�t���b�V���o�b�N����A���x�ቺ���܂��B
	�g���߂���(100��ȉ����炢�Ȃ���Ȃ�����)�� 60fps��ۂ͖̂����ł��̂ŁA
	���Ȃ��Ȃ�悤�ɃQ�[����݌v���܂��B
---------------------------------------------------------*/
static void gu_set_texture(int texture_number)
{
	//	sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);//???
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* ������ */
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
	�Q�[�����̕`�揈�����̂P
	---------------------------------------------------------
	�e���v���C�I���e�B�[�����ɂ�����̂��v���C�I���e�B�[���ɕ`���B
	�v���C�I���e�B�[���Q�DBG ��ʂ�`��
	�v���C�I���e�B�[���R�D�w�i��Q����`��
	�v���C�I���e�B�[���S�D�����w��`��
	�v���C�I���e�B�[���T�Djiki��`��
---------------------------------------------------------*/

//static void callback_gu_draw_haikei_all(void)
static void gu_blit_haikei_maho_jiki(void)
{
/* -- �P�DBG ��ʂ�`�� */
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
/* -- �Q�D�w�i��Q����`�� */
	#if (1)
	#endif
/* -- �R�D�����w��`�� */
	#if (1)
	{
		if (0!=draw_boss_hp_value)
		{
			gu_set_texture(TEX_02_MAHOUJIN);	/* �e�N�X�`�����Z�b�g */
			gu_draw_big_maho_jin();
		}
	}
	#endif
/* -- �S�Djiki��`�� */
	#if (1)
//	if (NULL != callback_gu_draw_haikei)	//���pif (0!=dr aw_bg_screen)
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
	��ʕ`��(�ėp)
---------------------------------------------------------*/

#define USE_VSYNC			(0)
/* 0:VSYNC���Ȃ��B USE_VSYNC ������v-sync 1 �� on �����A�܂��x���̂Ŗ����B */
#define USE_MAX_GU_TIME 	(0)
/* 0:USE_MAX_GU_TIME [Gu���Ԃ��ő�Ɏ��^�C�v](�F�X��������)
	Gu�̓]�����l���āAGu���łȂ�ׂ����񓮍삳����悤�Ƀ^�C�~���O���l�������́B
	psp�Ŏn�߂Ď��������̂́A http://www.fumi2kick.com/pspapp/ ���̕ӁB
	SDL���p����ꍇ(�\�t�g�`��)�́A(�^�C�~���O�I��)��΂ɊԂɍ���Ȃ��B
	(�t���[���o�b�t�@��݂��ăX���b�v������ΊԂɍ����B�܂�g���v���o�b�t�@�B�A���������H�����A�x���Ȃ邩���H)
	(Gu�`�悵�����Ȃ��Ȃ�A1:USE_MAX_GU_TIME�ŏ\���ł͂���B)
	�����SDL���p����̂ŁA1:USE_MAX_GU_TIME�ɏo���Ȃ��B(SDL�g��Ȃ��Ȃ�0:USE_MAX_GU_TIME�ɏo����)
*/
extern	void do_input_vbl(void);
void vbl_draw_screen(void)
{
	do_input_vbl();/*�L�[����(�����̓s����A�����Ɉړ�)*/
	#if (1==USE_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu���Ԃ��ő�Ɏ��^�C�v](�F�X��������) */
//	sceGuFinish();����
	/* - �p�P�b�g�`��I���҂� */
	sceGuSync(0, 0);/* (�����܂� Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* �������� Gu */
	/* --- �X�N���[���A�b�v�f�[�g */
	sceGuStart(GU_DIRECT, gulist );

	/* --- �v���C�I���e�B�[���P�D��ʃN���A */
	#if 1
	{
		const unsigned int bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/*(��)*/
			0xff106010,/*1��(��)*/
			0xff802010,/*2��(��)*/	//	0xff102080,/*2��*/
			0xff104010,/*3��(��)*/
			0xff402020,/*4��(��)*/
			0xff601030,/*5��(��)*/
			0xff601030,/*6��(��)*/
			0xff000000,/*ending(��)*/
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

	/* -- ��ʍX�V */

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
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
//
	/* -- BG�w�i ��ʂ�`�� */
//	if (1==dr aw_bg_screen) /* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG ��ʂ�`�� */
		/* -- �v���C�I���e�B�[���Q�`���T ��ʂ�`�� */
		gu_blit_haikei_maho_jiki();
	}
//
/* -- �v���C�I���e�B�[���U���C���e�X�g��ʂ�`�� */
	#if (1==LINE_TEST)
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- ���C���e�X�g��ʂ�`�� */
		gu_blit_lines();
	}
	#endif /*(000)*/
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
/* -- �v���C�I���e�B�[���V�DSDL ��ʂ�`�� */
	#if (1)
		#if (0)
		/*������ƃQ�[����SDL�����Ă݂�e�X�g*/
		if (NULL == callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
		#endif /*(000)*/
	{
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* ������ */
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*�����dou ble buffer???*/
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
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����*/
		gu_draw_sdl_screen();
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����*/
	}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
//
/* -- �v���C�I���e�B�[���W�D�G��`�� */
	/* �� �G�ʃG�t�F�N�g */
	/* �U�R(�G�ʃG�t�F�N�g) */
	/* �{�X(�G�ʃG�t�F�N�g) */
	#if (1)
	{
		gu_set_texture(TEX_04_TEKI);	/* �e�N�X�`�����Z�b�g */
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
/* -- �v���C�I���e�B�[���X�DITEM��`�� */
//	/* ���̑��̎��@�f��(�A�C�e���ʃG�t�F�N�g) */
	#if (1)
	{
		gu_set_texture(TEX_06_BULLET);	/* �e�N�X�`�����Z�b�g */
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
/* -- �v���C�I���e�B�[���P�O�D�G�e��`�� */
	/* �� �e���ʃG�t�F�N�g */
	#if (1)
	{
	//	gu_set_texture(TEX_06_BULLET);	/* �e�N�X�`�����Z�b�g */
		#if (1)
		/* �e�����X�g�͕�������\�� �Ȃ̂Ő�p�ɏ���������\�� */
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
/* -- �v���C�I���e�B�[���P�P�D�t�����g�ʂ�`�� */
	/* �� �t�����g�ʃG�t�F�N�g */
//	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
//	/* ������ / �U�R���Ŕ���(�t�����g�ʃG�t�F�N�g) */
//	/* �Ή�����(�t�����g�ʃG�t�F�N�g) */
	/* �{���o�[���S(�t�����g�ʃG�t�F�N�g) */
	/* /[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
	#if (1)
	{
		gu_set_texture(TEX_07_FRONT);	/* �e�N�X�`�����Z�b�g */
		/*front_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+0);
			common_transfer_objects(TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+(OBJ_BANK_07_FRONT_BANK0*OBJ_BANK_SIZE));
		}
		blit_all_objects(ggg_Sprites);/*PRIORITY_06_FRONT*/
	}
	/* �v���C�I���e�B�[���P�Q�D�� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p) */
	//{//PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (0 != pd_bomber_time)
	{
		gu_draw_front_spell_logo();
	}
	//}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
/* -- �v���C�I���e�B�[���P�R�D�p�l���ʂ�`�� */
	#if (1)
	if (0 != draw_side_panel)
	{
		gu_draw_side_panel();
	}
	#endif /*(000)*/
//
	/* -- �v���C�I���e�B�[���P�S�D�X�N���v�g��ʂ�`�� */
//	if (1==draw_script_screen)		/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (0 != (draw_script_screen+msg_time))
	{
		if (0 != (msg_time))/* ���b�Z�[�W(���Ή�)�\������ */
		{
			msg_time--;
		}
		gu_draw_script_window();
	}

	/* -- �v���C�I���e�B�[���P�T�D�J���f�o�b�O�t�H���g */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - �p�P�b�g�ς݁A�����܂� */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [�ʏ�^�C�v]  */
	sceGuSync(0, 0);/* (�����܂� Gu) */
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
	/* �\�� voice ���Đ� */
	voice_play_vbl();
	#endif
}


/*---------------------------------------------------------
	��ʕ`��(�N������p)
---------------------------------------------------------*/

void draw_loading_screen_test(void)
{
	#if (1==USE_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu���Ԃ��ő�Ɏ��^�C�v](�F�X��������) */
//	sceGuFinish();����
	/* - �p�P�b�g�`��I���҂� */
	sceGuSync(0, 0);/* (�����܂� Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* �������� Gu */
	/* --- �X�N���[���A�b�v�f�[�g */
	sceGuStart(GU_DIRECT, gulist );

	/* --- ��ʃN���A */
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- ��ʍX�V */

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
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
//
	/* -- BG�w�i ��ʂ�`�� */
//	if (1==dr aw_bg_screen) /* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG ��ʂ�`�� */
		gu_blit_haikei_maho_jiki();
	}
//

	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
	/* -- �J���f�o�b�O�t�H���g */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - �p�P�b�g�ς݁A�����܂� */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [�ʏ�^�C�v]  */
	sceGuSync(0, 0);/* (�����܂� Gu) */
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
