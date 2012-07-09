
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	Gu���g������ʕ`�揈��
---------------------------------------------------------*/
#include "111_my_file.h"/*(bg�ǂ݂��݁B)*/

#include "../../libgu/pspgu.h"//#include <pspgu.h>
#include "../../libgu/pgc.h"

#include "gu_draw_screen.h"

#include "../menu/kaiwa_sprite.h"/* �����G */
#include "kanji_system.h"/*"�Ɨ����C���\��"*/



#include <malloc.h>/* memalign() free() */

/*---------------------------------------------------------
	�f�o�b�O�p�e�X�g�ݒ�(�������Ȃ�)
---------------------------------------------------------*/
#define LINE_TEST	(1)


/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	u16 	u;
	u16 	v;
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
			u16 	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			u32 	color;
		#endif
	#endif
	s16 		x;
	s16 		y;
	s16 		z;
} Vertex_uvcxyz;

typedef struct
{
	u16 	u;
	u16 	v;
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			u16 	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			u32 	color;
		#endif
	#endif
	s16 		x;
	s16 		y;
	s16 		z;
} Vertex_uvcxyz_C32;

typedef struct
{
	/*float*/u16 x;
	/*float*/u16 y;
	/*float*/u16 z;
} Point_Vertex;

typedef struct
{
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			u16 	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			u32 	color;
		#endif
	#endif
	/*float*/u16 x;
	/*float*/u16 y;
	/*float*/u16 z;
} Point_Vertex_color;

/*---------------------------------------------------------
	���ʃ��[�N
---------------------------------------------------------*/

/*static*/global	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];

/*---------------------------------------------------------
	�O���[�o���ϐ�
---------------------------------------------------------*/

static u16 *render_image;
static u16 *render_image_back;
//static u16 *render_image_tex;


/*---------------------------------------------------------
	�X�N���[���Ǘ�
---------------------------------------------------------*/

static VIRTUAL_OBJ_STATE obj_status_table[(OBJ_BANK_MAX*OBJ_BANK_SIZE)];/*(6*8*8)*/

//static	MY_DIB_SURFACE *my_texture[TEXTURE_MAX];
/*static*/global			MY_TEXTURE_RESOURCE 	 my_resource[TEXTURE_MAX];
/*static*/static	const	MY_TEXTURE_RESOURCE initial_resource[TEXTURE_MAX] =
{
	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"}, 	//		TEX_00_BACK_GROUND = 0, 	/* 3D�w�i1 */
//	{NULL, 256, 256, 512,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"}, 	//	//	TEX_01_BACK_TEXTURE,		/* �w�i��Q�� */
	{NULL, 128, 128, 128,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/teki/mahoujin128.png"}, //		TEX_02_MAHOUJIN,			/* �����w */
	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/jiki/jiki00.png"},		//		TEX_03_JIKI,				/* ���e/���@ */
	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/teki/teki10_256.png"},	//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	{NULL, 256, 256, 512,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"}, 	//	//	TEX_05_ITEM,				/* �A�C�e��/�����X�R�A */
	{NULL, 128, 128, 128,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/teki/bullet0.png"}, 	//		TEX_06_BULLET,				/* �G�e */
	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/teki/front256.png"},	//		TEX_07_FRONT,				/* ���@������\��/����/[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
//	{NULL, 256, 256, 512,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"}, 	//	//	TEX_08_SCORE_PANEL, 		/* �X�R�A�p�l��/�X�R�A�t�H���g���� */
	{NULL, 128, 256, 128,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/tachie/remilia1.png"},	//	//	TEX_09_TACHIE_L,			/* �����G */
	{NULL, 128, 256, 128,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/tachie/remilia1.png"},	//	//	TEX_10_TACHIE_R,			/* �����G */
//	{NULL, 256, 256, 512,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"}, 	//	//	TEX_11_MESSAGE, 			/* ���b�Z�[�W�Œ蕶�� */
};

//����:
//	{NULL, 128, 256, 128,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/tachie/remilia1.png"},	//	//	TEX_09_TACHIE_L,			/* �����G */
//	{NULL, 128, 256, 128,	0, 0, &obj_status_table[0], (char*)DIRECTRY_NAME_DATA_STR "/tachie/remilia1.png"},	//	//	TEX_10_TACHIE_R,			/* �����G */

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

#include "render/gu_render_score.h"

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	�摜�֘A
---------------------------------------------------------*/

void psp_clear_screen(void)
{
	/* ����Gu�ŕ`�����ꍇ�B�n�[�h�E�F�A�[�@�\�ŁA�u����������̂ō��̂����ɂ܂Ƃ߂Ƃ� */
	SDL_FillRect(cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL, 0/*SDL_MapRGB(cb.sdl_screen[SDL_00_VIEW_SCREEN]->format,0,0,0)*/);
}
void psp_move_screen(int src_screen_number, int dst_screen_number )
{
	/* ����Gu�ŕ`�����ꍇ�B�n�[�h�E�F�A�[�@�\�ŁA�u����������̂ō��̂����ɂ܂Ƃ߂Ƃ� */
	PSPL_UpperBlit(cb.sdl_screen[src_screen_number], NULL, cb.sdl_screen[dst_screen_number], NULL);
}
//void psp_push_screen(void)
//{
//	PSPL_UpperBlit(sdl_screen[SDL_00_VIEW_SCREEN], NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
//}
//void psp_pop_screen(void)
//{
//	PSPL_UpperBlit(sdl_screen[SDL_01_BACK_SCREEN], NULL, sdl_screen[SDL_00_VIEW_SCREEN], NULL);
//}


/*---------------------------------------------------------
	psp�̕\���� vram ����SDL���(���z�X�N���[��)(SDL_00_VIEW_SCREEN)�ɋ����R�s�[
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1/*(r35�v��񂾂����H)*/
static void s_gu_save_screen(void)
{
	if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]); 	}	/* ���b�N */
	u16 *dest;	dest = render_image;
	u16 *src;	src = (u16 *)((u32) 0x44000000);/*VRAM*/
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
//	sceGuStart(GU_DIRECT, gulist);
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
	if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ���b�N���� */
}
#endif


#include "render/SDL_pause_filter.h"


/*------------------------------------------------------------- */


void select_jiki_load_surface(void)
{
//	strcpy(filename, DIRECTRY_NAME_DATA_STR "/jiki/jiki0Z.png");
	my_resource[TEX_03_JIKI].file_name[11+DIRECTRY_NAME_DATA_LENGTH] = ('0'+(cg_game_select_player));
	psp_load_texture( TEX_03_JIKI );
//	draw_jiki_screen = 1;
}
static void gu_draw_bg_fake3D(void);
static void gu_draw_bg_2D(void);
static void gu_draw_bg_eientei(void);
static void gu_draw_bg_3D_test01(void);
static void gu_draw_bg_3D_test02(void);



void stage_bg_load_texture(void)
{
//	strcpy(filename, DIRECTRY_NAME_DATA_STR "/bg/backZ_256.png");
	my_resource[TEX_00_BACK_GROUND].file_name[8+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage);
	psp_load_texture( TEX_00_BACK_GROUND );
//
//	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"},		//		TEX_00_BACK_GROUND = 0, 	/* 3D�w�i1 */
//	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], DIRECTRY_NAME_DATA_STR "/teki/teki10_256.png"}, 	//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], DIRECTRY_NAME_DATA_STR "/teki/boss00_256.png"}, 	//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//
//	my_resource[TEX_04_TEKI].file_name[10+DIRECTRY_NAME_DATA_LENGTH] = ('0'+0/*cg.game_now_stage*/);
	my_resource[TEX_04_TEKI].file_name[10+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage);
	my_resource[TEX_04_TEKI].file_name[11+DIRECTRY_NAME_DATA_LENGTH] = ('0');
	psp_load_texture( TEX_04_TEKI );
//
	my_resource[TEX_04_TEKI].object_table_head = (obj_status_table+(OBJ_BANK_11_ZAKO_STAGE1*OBJ_BANK_SIZE));
//
	void (*aaa[16/*8*/])(void)	=
	{
		gu_draw_bg_2D,			/*loading*/
		gu_draw_bg_fake3D,		/*1��"���@�̐X"*/
		gu_draw_bg_eientei, 	/*3��"�|��"*/
		gu_draw_bg_eientei, 	/*4��"�i���� �L��"*/
		gu_draw_bg_2D,			/*2��"�H�߂���"*/
		gu_draw_bg_fake3D,		/*5��"�g���� �}����"*/
		gu_draw_bg_fake3D,		/*6��"�g���� ��L��"*/
		gu_draw_bg_3D_test01,	/*ending*/
//
		gu_draw_bg_fake3D,		/* extra1�� */
		gu_draw_bg_3D_test02,	/* phantazm1�� */
		gu_draw_bg_fake3D,		/*extra*/
		gu_draw_bg_fake3D,		/*extra*/
		gu_draw_bg_2D,			/*extra*/
		gu_draw_bg_2D,			/*extra*/
		gu_draw_bg_2D,			/*extra*/
		gu_draw_bg_2D,			/*extra*/
	};
	cb.callback_gu_draw_haikei			= aaa[cg.game_now_stage&0x0f];//	dr aw_bg_screen = 1;
//	cb.callback_gu_draw_haikei			= aaa[cg.game_now_stage&0x07];//	dr aw_bg_screen = 1;
//	cb.callback_gu_draw_haikei			= callback_gu_draw_haikei_all;//	dr aw_bg_screen = 1;
	cb.callback_gu_draw_haikei_modosu	= cb.callback_gu_draw_haikei;
}
void stage_boss_load_texture(void)
{
//	strcpy(filename, DIRECTRY_NAME_DATA_STR "/bg/backZ_256.png");
//	my_resource[TEX_00_BACK_GROUND].file_name[8+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage);
//	psp_load_texture( TEX_00_BACK_GROUND );
//
//	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], DIRECTRY_NAME_DATA_STR "/bg/back0_256.png"},		//		TEX_00_BACK_GROUND = 0, 	/* 3D�w�i1 */
//	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], DIRECTRY_NAME_DATA_STR "/teki/teki10_256.png"}, 	//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//	{NULL, 256, 256, 256,	0, 0, &obj_status_table[0], DIRECTRY_NAME_DATA_STR "/teki/boss00_256.png"}, 	//	//	TEX_04_TEKI,				/* �{�X/�U�R�G */
//
	my_resource[TEX_04_TEKI].file_name[10+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage);
	my_resource[TEX_04_TEKI].file_name[11+DIRECTRY_NAME_DATA_LENGTH] = ('1');
	psp_load_texture( TEX_04_TEKI );
//
//	my_resource[TEX_04_TEKI].object_table_head = (obj_status_table+(OBJ_BANK_21_BOSS_STAGE1*OBJ_BANK_SIZE));
	my_resource[TEX_04_TEKI].object_table_head = (obj_status_table+((OBJ_BANK_21_BOSS_STAGE1-1)*OBJ_BANK_SIZE)+(cg.game_now_stage<<6));
}

static void gu_init_vfpu(void);
void draw_loading_screen_test(void);
extern void sendCommandi(int cmd, int argument);

/*only boot once*/extern void /*SD LCALL*/ PSPL_VideoInit(void);/*const char *driver_name, u32 flags*/
/*only boot once*/extern void /*SD LCALL*/ PSPL_AudioInit(void);/*const char *driver_name*/
	#if 0
	if (atexit(SDL_Quit))
	{
		CHECKPOINT;
		error(ERR_WARN, "atexit dont returns zero");
	}
	#endif

global ML_FONT ml_font[ML_LINE_99_MAX];
global void psp_video_init01(void)
{

#if (1==USE_16BIT_BLIT)
	#define  SCREEN_DEPTH	16
#else
	#define  SCREEN_DEPTH	32
#endif
	#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
		#define  FRAMESIZE (0x44000)
	#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
		#define  FRAMESIZE (0x88000)
	#endif /* SCREEN_DEPTH */

#define USE_SWAP_BUFFER (0)

#if (1==USE_SWAP_BUFFER)
	cb.draw_frame = (void *)(FRAMESIZE * 0);
	cb.show_frame = (void *)(FRAMESIZE * 1);
	cb.work_frame = (void *)(FRAMESIZE * 2);
	cb.tex_frame  = (void *)(FRAMESIZE * 3);
#endif


//static u16 *bullet_image;

//	PSPL_Init();//SDL_INIT_VIDEO | SDL_INIT_AUDIO/*|SDL_INIT_NOPARACHUTE*/); /*| SDL_INIT_JOYSTICK*/
	PSPL_AudioInit();/*only boot once*/ 	/* Initialize the audio subsystem */

	{
	//	#define dummy_SDL_VRAM_SCREEN	vram_screen
	//	/*static*/ SDL_Surface *dummy_SDL_VRAM_SCREEN;
	//	dummy_SDL_VRAM_SCREEN =
		PSPL_video_init(
		//	PSP_WIDTH480,
		//	PSP_HEIGHT272,
			/*int depth 		=*/ SDL_5551_15/*PSP_DEPTH16*/,
		//	/*u32 videoflags =*/ (SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE /*| SDL_HW PALETTE | SDL_HW ACCEL*/)
		//	/*u32 videoflags =*/ (SDL_FULLSCREEN /*| SDL_DOUBLEBUF*/ | SDL_HWSURFACE | SDL_HW PALETTE | SDL_HW ACCEL)
			PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
			/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/,	/*0*/
			/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
			/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
			/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/ 	/*5*/
		);	/*0*/ /*only boot once*/ 	/* Initialize the video subsystem */


	//	if (NULL == dummy_SDL_VRAM_SCREEN)
	//	{
	//		CHECKPOINT;
	//		error(ERR_FATAL, "cant open screen.");
	//	}
	}


	#if 1//(1==US E_GU)
	//#define SDL_PSP_BUFFER_WIDTH512 (512)
	cb.sdl_screen[SDL_00_VIEW_SCREEN] = /*()*/
	#if 0
		PSPL_create_ABGR_surface_buffer
		(
		(void *)(FRAMESIZE * 2),
		SDL_PSP_BUFFER_WIDTH512/*PSP_WIDTH480*/,/*sdl_screen[SDL_00_VIEW_SCREEN]->w*/
		PSP_HEIGHT272,/*sdl_screen[SDL_00_VIEW_SCREEN]->h*/
		/*pitch = */ /*surface->w*/(SDL_PSP_BUFFER_WIDTH512)*(2)/*surface->format->BytesPerPixel*/
		);
	#else
		SDL_CreateSurface
		(
		/*SDL_SWSURFACE*/SDL_HWSURFACE,/*VRAM��*/
		SDL_PSP_BUFFER_WIDTH512/*PSP_WIDTH480*/,/*sdl_screen[SDL_00_VIEW_SCREEN]->w*/
		PSP_HEIGHT272// /*sdl_screen[SDL_00_VIEW_SCREEN]->h*/
		);
	#endif
		if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]); }
		render_image = (u16 *)cb.sdl_screen[SDL_00_VIEW_SCREEN]->pixels;
		if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]);	}
	#endif
//	cb.sdl_screen[SDL_01_BACK_SCREEN] = NULL;
	cb.sdl_screen[SDL_01_BACK_SCREEN] = /*()*/
	#if 0
		PSPL_create_ABGR_surface_buffer
		(
		(void *)(FRAMESIZE * 3),
		SDL_PSP_BUFFER_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		/*pitch = */ /*surface->w*/(SDL_PSP_BUFFER_WIDTH512)*(2)/*surface->format->BytesPerPixel*/
		);
	#else
		SDL_CreateSurface
		(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
		SDL_PSP_BUFFER_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272// /*screen->h*/
		);
	#endif
	#if 1//(1==US E_GU)
		if (SDL_MUSTLOCK(cb.sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_LockSurface(cb.sdl_screen[SDL_01_BACK_SCREEN]); }
		render_image_back = (u16 *)cb.sdl_screen[SDL_01_BACK_SCREEN]->pixels;
		if (SDL_MUSTLOCK(cb.sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_UnlockSurface(cb.sdl_screen[SDL_01_BACK_SCREEN]); }
	#endif
	#if (0)
	if (NULL == cb.sdl_screen[SDL_01_BACK_SCREEN])
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant create SDL_Surface.");
	}
	#endif

#if 0
	cb.sdl_screen[SDL_02_TEX_SCREEN] = SDL_CreateRGBSurface
		(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
		SDL_PSP_BUFFER_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/ 	/*0*/
		);
	#if 1//(1==US E_GU)
		if (SDL_MUSTLOCK(cb.sdl_screen[SDL_02_TEX_SCREEN])) {	SDL_LockSurface(cb.sdl_screen[SDL_02_TEX_SCREEN]); }
		render_image_tex = (u16 *)sdl_screen[SDL_02_TEX_SCREEN]->pixels;
		if (SDL_MUSTLOCK(cb.sdl_screen[SDL_02_TEX_SCREEN])) {	SDL_UnlockSurface(cb.sdl_screen[SDL_02_TEX_SCREEN]); }
	#endif
#endif

	//# /* �J�X�^�����C�u������Gu���g���ꍇ */

	#define USE_GULIB (0)
	/* setup GU initialise */
	#if (1==USE_GULIB)
	sceGuInit();/*(���͓����ő��̏���������==Ge�̃��W�X�^�ݒ�͂���)*/
	#else
	pgc_first_initilize();/*(���͓����ő��̏���������==Ge�̃��W�X�^�ݒ�͂���)*/
	#endif
	#if 0
	/*(�����Ă��w�Ǔ���)*/
	pgc_display(PGC_OFF);//sceGuDisplay(GU_FALSE);/*���OFF*/
	#endif
	sceGuStart(GU_DIRECT, gulist);
	#if (1==USE_SWAP_BUFFER)
		#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
		//	sceGuDrawBuffer(GU_PSM_5551, (void*)0, PSP_BUFFER_WIDTH512);
		//	sceGuDrawBuffer(GU_PSM_5551, cb.draw_frame, PSP_BUFFER_WIDTH512);
			sceGuDrawBuffer(/*GU_PSM_5551*/SDL_GU_PSM_0000, cb.draw_frame, PSP_BUFFER_WIDTH512);
		#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
			sceGuDrawBuffer(GU_PSM_8888, cb.draw_frame, PSP_BUFFER_WIDTH512);
		#endif /* SCREEN_DEPTH */
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, cb.show_frame, PSP_BUFFER_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer(cb.work_frame, PSP_BUFFER_WIDTH512);/*(void*)0x88000*/
	//	sceGuDepthBuffer(cb.work_frame, PSP_BUFFER_WIDTH512);/*(void*)0x110000*/
		#endif /* (1==USE_ZBUFFER) */
	#else
		#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
		//	sceGuDrawBuffer(GU_PSM_5551, (void*)0, PSP_BUFFER_WIDTH512);
		//	sceGuDrawBuffer(GU_PSM_5551, cb.draw_frame, PSP_BUFFER_WIDTH512);
			sceGuDrawBuffer(/*GU_PSM_5551*/SDL_GU_PSM_0000, (void*)0, PSP_BUFFER_WIDTH512);
			sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, PSP_BUFFER_WIDTH512);
			#if (1==USE_ZBUFFER)
			sceGuDepthBuffer((void*)0x88000, PSP_BUFFER_WIDTH512);
			#endif /* (1==USE_ZBUFFER) */
		#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
			sceGuDrawBuffer(GU_PSM_8888, (void*)0, PSP_BUFFER_WIDTH512);
			sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, PSP_BUFFER_WIDTH512);
			#if (1==USE_ZBUFFER)
			sceGuDepthBuffer((void*)0x110000, PSP_BUFFER_WIDTH512);
			#endif /* (1==USE_ZBUFFER) */
		#endif /* SCREEN_DEPTH */
	#endif
	sceGuOffset(2048 - (PSP_WIDTH480 / 2), 2048 - (PSP_HEIGHT272 / 2));
	sceGuViewport(2048, 2048, PSP_WIDTH480, PSP_HEIGHT272);

#if 1
	/* �`��͈͂�ݒ肷�� */
	#if (1==USE_GULIB)
	sceGuEnable(GU_SCISSOR_TEST);
	#else
	pgc_scissor_enable();
	#endif
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
	PGC_SEND(PGC_ALPHA_TEST, PGC_ON);	//sceGuEnable(GU_ALPHA_TEST);	/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);			/* �L�� */
	//sceGuDisable(GU_ALPHA_TEST);			/* ���� */	/*noiz07*/
	#endif/*???*/
#endif/*000???*/

	#if (1==USE_ZBUFFER)
	//sceGuEnable(GU_DEPTH_TEST);			/* �L�� */
	//sceGuDisable(GU_DEPTH_TEST);			/* ���� */	/*noiz07*/
	PGC_SEND(PGC_DEPTH_TEST, PGC_OFF);	//sceGuDisable(GU_DEPTH_TEST);
	/* psp �� z�l�� 65535(�߂�) ���� 0(����)�ŁAZ�l���ʏ�Ƌt�B */
	/* ����z�l�� u16 �Ȃ̂ŁA�����͂ݏo���ƁA��荞��ŕςȕ`��ɂȂ��Ă��܂��B */
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
	#if (1==USE_GULIB)
	PGC_SEND(PGC_CULL_FACE, PGC_ON);	//sceGuEnable(GU_CULL_FACE);	/* �J�����O�L��(�������|���S���͕`�悵�Ȃ�) */
	#endif
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
//	#define GU_FLAT 		(0)
//	#define GU_SMOOTH		(1)
//	sceGuShadeModel(GU_FLAT);		/*(�O�[���V�F�[�f�B���O���Ȃ�)	���_�J���[�̐F��ԋ@�\���g��Ȃ��ꍇ�Ɏw��*/
	sceGuShadeModel(GU_SMOOTH); 	/*(�O�[���V�F�[�f�B���O����)	���_�J���[�̐F��ԋ@�\���g���ꍇ�Ɏw��*/
	#endif/*???*/
#endif/*1???*/

	#if (1==USE_GULIB)
		#if 0/*???*/
		PGC_SEND(PGC_BLEND, PGC_OFF);	//sceGuDisable(GU_BLEND);
		#else
		PGC_SEND(PGC_BLEND, PGC_ON);	//sceGuEnable(GU_BLEND);			/* �A���t�@�u�����f�B���O�L�� */
		#endif/*???*/

		#if 1
		/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
		PGC_SEND(PGC_TEXTURE_2D, PGC_ON);	//sceGuEnable(GU_TEXTURE_2D);	/* �e�N�X�`���[�L�� */
		#else
		/* �e�N�X�`���[�������[�h�ł́A����ʓ��̃t���b�g�|���S�����g����(�e�N�X�`���[�|���S���A2D�摜�]�����͏o���Ȃ�) */
		PGC_SEND(PGC_TEXTURE_2D, PGC_OFF);	//sceGuDisable(GU_TEXTURE_2D);	/* �e�N�X�`���[���� */
		#endif
	#endif

	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);//Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);	/*noiz07*/	/*(Z�o�b�t�@������??)*/
//	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);	/*noiz07*/
	#endif/* (1==USE_ZBUFFER) */

	#if (1)
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* ������ */
	#else
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);/* �������s�H */
	#endif

//	PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����(�g�厞�����⊮)*/
	PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����(�g�厞�o�Ȑ��⊮)*/
	sceGuTexScale(1.0f, 1.0f);//sceGuTexScale(1,1);
	sceGuTexOffset(0.0f, 0.0f);
//
	/* ���F�����߂�H */
	PGC_SEND(PGC_TEX_ENV_COLOR, 0x00000000);// sceGuTexEnvColor(0x00000000);
//	sceGuTexEnvColor(			0x00ffff00);
	sceGuAmbientColor(			0xffffffff);
	sceGuColor( 				0xff000000);

//
//	u8	ColorKey	= 1;
/* �J���[�L�[��ݒ肷�� */
//	if (ColorKey)
	{
//		if (!sceGuGetStatus(GU_COLOR_TEST))
		{
			#if (1==USE_GULIB)
			PGC_SEND(PGC_COLOR_TEST, PGC_ON);	//sceGuEnable(GU_COLOR_TEST);
			#endif
		}/*�J���[�L�[�L��*/
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
			#if (1==USE_GULIB)
	#if 1
	PGC_SEND(PGC_CLIP_PLANES, PGC_ON);	//sceGuEnable(GU_CLIP_PLANES);
	#endif
			#endif
//
	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/* vsync */

	#if (1==USE_GULIB)
	sceGuDisplay(GU_TRUE/*1*/);/* ���ON */
	#else
	pgc_display(PGC_ON);/* ���ON */
	#endif
	/* �����܂ŏ����ݒ� */
	gu_init_vfpu();
	/* �����܂ŏ����ݒ� */
//	sceGuSync(0, 0);/* (�����܂� Gu) */
//	sceDisplayWaitVblankStart();/*vsync*/
//	sceGuSwapBuffers();
	//
	/* --- ���͑��u�̏����ݒ� */
	sceCtrlSetSamplingCycle(0); 	/* pad������ */
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);	/* �A�i���O�L�[�L��(�W���ł̓f�W�^���̂�) */
	/*
	psp_pad�������ɂ͎��Ԃ�������B
	�����ŏ��������Ă����B
	���������Ă����ɓǂ����Ƃ��Ă���Ԏn�߂̓��͂��ǂ߂Ȃ��A��������(1[frame]���x)���o�ĂΓǂ߂�B
	�����ŁA���̒���Ƀ��[�f�B���O��ʂ�z�u����B
	���[�f�B���O��ʕ\���͉摜���[�h�ɐ��t���[�����Ԃ�������̂ŁA�p�b�h��������̎��ԉ҂��ɍs���B
	*/
	/* --- ���[�f�B���O��� */
	#if (1)/* ���[�f�B���O��ʂ̉摜��ǂ݂��� */
	my_file_common_name[0] = BG_TYPE_04_loading;psp_load_bg_file_name();/*(�@�r�̉摜�����p��ʂɓǂ݂���)*/
	psp_pop_screen();/*(�ǂ݂��񂾉摜�����p��ʂ���\���p��ʂɓ]������B(���̎��_�ł͕\���p��ʂ̕\���������܂������ׁA�܂��\������Ȃ�))*/
	#endif
	//
	#if (1)/* ���[�h��p��ʕ\�����s���B(�܂����̏��������ς�łȂ��̂Œʏ�̉�ʕ\���͏o���Ȃ�) */
	draw_loading_screen_test();/* ���[�h��ʕ\����p�B */
//	draw_loading_screen_test();//�d�l�ύX�Ń_��
//	blit_direct_vram_loading_screen();
	#endif
	//
	#if (1)/*("�Ɨ����C���\��"�̏�����)*/
	{
		unsigned int yyy;
		yyy = (64);/* Y�ʒu */
		unsigned int gg;
		for (gg=0; gg<ML_LINE_99_MAX; gg++)
		{
			ml_font[gg].x = (32);/* X�ʒu */
			ml_font[gg].y = (yyy);
			ml_font[gg].timer	= ML_OFF;
		//	ml_font[gg].render	= NULL;
		//	ml_font[gg].str 	= NULL;
			yyy += (KANJI_FONT_18_HEIGHT_P2);
		}
	}
//			ml_font[0].timer	= ML_ON;/*(�f�o�b�O)*/
//			ml_font[1].timer	= ML_ON;/*(�f�o�b�O)*/
//			ml_font[2].timer	= ML_ON;/*(�f�o�b�O)*/
//		//	ml_font[0].str	= my_font_text;/*(�f�o�b�O)*/
//		//	ml_font[1].str	= my_font_text;/*(�f�o�b�O)*/
//		//	ml_font[2].str	= my_font_text;/*(�f�o�b�O)*/
	#endif





}
	#if (0)/*(�f�o�b�O)*/
	kanji_window_clear_line(0); 			/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
	kanji_cursor_move_home_position();		/* �J�[�\����1�s��(==0)�ֈړ� */
	{
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_02_JIKI_SPELL_CARD);/* [��/���@�J�[�h�p�w�i]����Ӕw�ion */
		cg.msg_time 				= (65536);	/* �� 18 �� */
		strcpy(my_font_text, "�����V�X�e����g�ݍ��݂܂����B" "\n");
		kanji_color((7)|STR_CODE_NO_ENTER);
		kanji_draw();
	}
	#endif

void psp_video_init02(void)
{
	{	/* �e�N�X�`���̈�̏����� */
		unsigned int i;
		for (i=0; i<TEXTURE_MAX; i++)
		{
		//	my_resource[i].my_texture = NULL;
			my_resource[i].my_texture			= initial_resource[i].my_texture;
			my_resource[i].texture_width		= initial_resource[i].texture_width;/*(�e�N�X�`���S�̂̕��T�C�Y)*/
			my_resource[i].texture_height		= initial_resource[i].texture_height;/*(�e�N�X�`���S�̂̍����T�C�Y)*/
			my_resource[i].buffer_width 		= initial_resource[i].buffer_width;/*(�e�N�X�`���S�̂̕��T�C�Y�̒��ŁA�g�p���Ă���̈�T�C�Y)*/
			//
			my_resource[i].color_format 		= initial_resource[i].color_format;/*([����]�e�N�X�`���̐F�`��)*/
			my_resource[i].hh					= initial_resource[i].hh;/*([����]�\��)*/
			my_resource[i].object_table_head	= initial_resource[i].object_table_head;
			my_resource[i].file_name			= initial_resource[i].file_name;/*([����]�e�N�X�`���̃t�@�C����)*/
			//
			psp_load_texture(i);/*(�e�N�X�`���̏������B�S�Ă̗̈�Ƀe�N�X�`����ǂ݂��ށB)*/
		}
	}
	kaiwa_obj_set_256();/*(��b�X�v���C�g�̐ݒ�B�T�C�Y�� 256x256�ɂ���B)*/
	/* --- ���̑��̏����ݒ� */
	cg.side_panel_draw_flag 	= (0);
	cg.draw_boss_hp_value		= (0);
//	callback_gu_draw_haikei 	= NULL;//dr aw_bg_screen = 0;
	cb.callback_gu_draw_haikei_supeka = gu_draw_bg_3D_test01;
	cg.conv_bg_alpha			= 0xffffffff;
//	cg.conv_bg_alpha			= 0x7f7f7f7f;
//

	cg.game_now_stage	= 0;
//	stage_bg_load_texture();//�d�l�ύX�Ŗ��Ӗ�
//	sceKernelDelayThread(5000000);/* 5[�b]�������Ȃ��ő҂� */
}
			#if (0)
				psp_load_texture( TEX_00_BACK_GROUND	);/*game*/
				psp_load_texture( TEX_02_MAHOUJIN		);/*game*/
				psp_load_texture( TEX_03_JIKI			);/*game*/
				psp_load_texture( TEX_04_TEKI			);/*game*/
				psp_load_texture( TEX_06_BULLET 		);/*game*/
				psp_load_texture( TEX_07_FRONT  		);/*game*/
				psp_load_texture( TEX_09_TACHIE_L		);/*game*/
				psp_load_texture( TEX_10_TACHIE_R		);/*game*/
			#endif
/*-------------*/
//	#include "gu_fake_loading.h"
/*-------------*/

/*---------------------------------------------------------
	�e�N�X�`�������[�h����
---------------------------------------------------------*/



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
#if 0
{
	int kk;
	for (kk=0; kk<(trans_length); kk++)
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
}
#endif
		#if (0==USE_32BIT_DRAW_MODE)
	/*static*/ /*extern*/void trans_format8888to5650(u32 *pixsrc, u16 *pixdst, int trans_length)
	{
		int kk;
		for (kk=0; kk<(trans_length); kk++)
		{
		//	(*(cb.kanji_window_screen_image+kk)) = (*(/*font_bg_bitmap_surface_image*/(u32 *)(font_bg_bitmap_surface->pixels)+kk));/*BG_FONT_HAIKEI_OFFSET*/
			/* --- 16bit�F�� ���F���ĕێ� (color key �g�p�s��) */
			*pixdst = MAKECOL16(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc));
			pixdst++;
			pixsrc++;
		}
	}
		#else
		/* �F32bit���[�h */
		#endif

#include "gu_load_texture.h"



/*---------------------------------------------------------
	�ėp�X�v���C�g�̕\��(��ʓ]��)
---------------------------------------------------------*/

#if (1==USE_ZBUFFER)
	/* �n�[�h�E�F�A�[�Ń\�[�g */
	#define SPR_PRIORITY	(spr->priority)
#else
	/* �\�t�g�E�F�A�[�� Z�\�[�g */
	#define SPR_PRIORITY	(0)
#endif

#include "render/gu_render_sprite_normal.h"
#include "render/gu_render_sprite_roz.h"



/*---------------------------------------------------------
	�t���[���^�C�~���O�ɂ��S�`��
---------------------------------------------------------*/

#if 000
static /*int*/u8 texture_cacheId = 127/*-1*/;
static void trans_texture(void)
{
	MY_DIB_SURFACE *texture_cache_ptr;
	//	texture_cache_ptr = NULL/*0*/;
	//	texture_cache_ptr = gu_bbb_sprite_pool[0].Texture_ptr;
		texture_cache_ptr = my_texture[texture_cacheId];
		/* - �e�N�X�`���]���R�}���h */
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		#if (1==USE_COLOR_16_AND_32)
		if (16 == texture_cache_ptr->format->BitsPerPixel)
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



/*---------------------------------------------------------

---------------------------------------------------------*/
//extern OBJ *sprite_list000_head;
//extern OBJ *sprite_list444_head;

#include "gu_obj_table.h"

/*static*/static void rotate_and_zoom_check_blit(OBJ *sss)
{
	if (M_ZOOM_Y256_NO_TILT/*0.0*/ != sss->m_zoom.y256) 	/* ����@�\ */
	{
		if (256/*t256(1.0)*/ != sss->m_zoom.y256)	{goto my_roz;}
		if (256/*t256(1.0)*/ != sss->m_zoom.x256)	{goto my_roz;}
		if ((0!= sss->rotationCCW1024)) 			{goto my_roz;}
	}
	/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
	render_object_no_rot_zoom(sss);
	return;
my_roz:
	/* ��]�g��k������ */
	render_object_use_rot_zoom(sss);
}
#if 0
//	debug_view_objs++;
	/* --- �e�N�X�`���̓]�����K�v�Ȃ� */
	{
		//if (sss->Texture_ptr != NULL)
		{
			if (( sss->rotationCCW1024 == 0/*.0*/) &&
				( sss->m_zoom.x256 == 256/*1.0*/) &&
				( sss->m_zoom.y256 == 256/*1.0*/)
			)
			{
				/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
				render_object_no_rot_zoom(sss);
			}
			else
			{
				/* ��]�g��k������ */
				render_object_use_rot_zoom(sss);
			}
		}
	}
#endif

/* ���ڕ`�� */
#if 1
typedef struct /*()*/
{
	int offset;
	int maximum;
} AAA_TBL;
	static const  AAA_TBL aaa_tbl[OBJECT_TYPE_04_MAX] =
	{
		{	OBJ_HEAD_00_0x0000_TAMA,		OBJ_POOL_00_TAMA_1024_MAX}, 		// OBJECT_TYPE_00_TAMA
	//	{	OBJ_HEAD_02_0x0900_TEKI_FIX,	OBJ_POOL_02_TEKI_FIX_0256_MAX}, 	// OBJECT_TYPE_01_TEKI
		{	OBJ_HEAD_01_0x0800_TOUROKU, 	(OBJ_POOL_01_TOUROKU_0256_MAX+OBJ_POOL_02_TEKI_FIX_0256_MAX)},	// OBJECT_TYPE_01_TEKI
		{	OBJ_HEAD_03_0x0a00_KOTEI,		OBJ_POOL_03_KOTEI_0016_MAX},		// OBJECT_TYPE_02_KOTEI
		{	OBJ_HEAD_04_0x0a10_PANEL,		OBJ_POOL_04_PANEL_0056_MAX},		// OBJECT_TYPE_03_PANEL
	};

//	OBJ *sss,//, VIRTUAL_OBJ_STATE *head_obj_status_table)
//static void common_transfer_objects_01_teki_blit();
//static void common_transfer_objects_02_kotei_blit();
static void common_transfer_objects_blit(
	const int object_type,
	int num,
	int obj_group)
{
	VIRTUAL_OBJ_STATE *head_obj_status_table;
	head_obj_status_table = my_resource[num].object_table_head;
//
	{
			OBJ *sss;
	//		sss = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+ii];
		int ii;
	//	for (ii=0; ii<OBJ_POOL_03_KOTEI_0016_MAX/*OBJ_111JIKI_POOL_MAX*/; ii++)/* �S�����ׂ�B */
		for (ii=0; ii<aaa_tbl[object_type].maximum/*OBJ_POOL_02_TEKI_FIX_0256_MAX*/; ii++)/* �S�����ׂ�B */
		{
			sss = &obj99[/*OBJ_HEAD_02_0x0900_TEKI_FIX*/(aaa_tbl[object_type].offset)+ii];
		//
			if (JYUMYOU_NASI < sss->jyumyou)/* ����� */
			{
				if (/*SP_BULLET*/obj_group/*OBJ_Z04_TAMA*/ & sss->obj_type_set)
				{
					{
						int tama_index = (OBJ_Zff_MASK_SUB_TYPE & sss->obj_type_set);
						sss->w			= head_obj_status_table[tama_index].w;
						sss->h			= head_obj_status_table[tama_index].h;
						sss->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
						sss->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
					}
					/* --- �ėp�X�v���C�g�`�� */
					rotate_and_zoom_check_blit(sss);
				}
			}
		//	sss += sizeof(OBJ);
//			sss = sss->next;/*��*/
		}
	}
}
#endif

#if 0
/*---------------------------------------------------------

---------------------------------------------------------*/
//static void common_transfer_objects_01_teki_blit();
//static void common_transfer_objects_02_kotei_blit();
static void common_transfer_objects_01_teki_blit(
//	const int object_type,
	int num,
	int obj_group)
{
	VIRTUAL_OBJ_STATE *head_obj_status_table;
	head_obj_status_table = my_resource[num].object_table_head;
//
	{
			OBJ *sss;
	//		sss = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+ii];
		int ii;
	//	for (ii=0; ii<OBJ_POOL_03_KOTEI_0016_MAX/*OBJ_111JIKI_POOL_MAX*/; ii++)/* �S�����ׂ�B */
		for (ii=0; ii</*aaa_tbl[object_type].maximum*/OBJ_POOL_02_TEKI_FIX_0256_MAX/*OBJ_POOL_02_TEKI_FIX_0256_MAX*/; ii++)/* �S�����ׂ�B */
		{
			sss = &obj99[/*OBJ_HEAD_02_0x0900_TEKI_FIX*/OBJ_HEAD_02_0x0900_TEKI_FIX/*(aaa_tbl[object_type].offset)*/+ii];
		//
			if (JYUMYOU_NASI < sss->jyumyou)/* ����� */
			{
				if (/*SP_BULLET*/obj_group/*OBJ_Z04_TAMA*/ & sss->obj_type_set)
				{
					{
						int tama_index = (OBJ_Zff_MASK_SUB_TYPE & sss->obj_type_set);
						sss->w			= head_obj_status_table[tama_index].w;
						sss->h			= head_obj_status_table[tama_index].h;
						sss->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
						sss->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
					}
					/* --- �ėp�X�v���C�g�`�� */
					rotate_and_zoom_check_blit(sss);
				}
			}
		//	sss += sizeof(OBJ);
//			sss = sss->next;/*��*/
		}
	}
}//static void common_transfer_objects_01_teki_blit();
//static void common_transfer_objects_02_kotei_blit();
static void common_transfer_objects_02_kotei_blit(
//	const int object_type,
	int num,
	int obj_group)
{
	VIRTUAL_OBJ_STATE *head_obj_status_table;
	head_obj_status_table = my_resource[num].object_table_head;
//
	{
			OBJ *sss;
	//		sss = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+ii];
		int ii;
	//	for (ii=0; ii<OBJ_POOL_03_KOTEI_0016_MAX/*OBJ_111JIKI_POOL_MAX*/; ii++)/* �S�����ׂ�B */
		for (ii=0; ii</*aaa_tbl[object_type].maximum*/OBJ_POOL_03_KOTEI_0016_MAX/*OBJ_POOL_02_TEKI_FIX_0256_MAX*/; ii++)/* �S�����ׂ�B */
		{
			sss = &obj99[/*OBJ_HEAD_02_0x0900_TEKI_FIX*/OBJ_HEAD_03_0x0a00_KOTEI/*(aaa_tbl[object_type].offset)*/+ii];
		//
			if (JYUMYOU_NASI < sss->jyumyou)/* ����� */
			{
				if (/*SP_BULLET*/obj_group/*OBJ_Z04_TAMA*/ & sss->obj_type_set)
				{
					{
						int tama_index = (OBJ_Zff_MASK_SUB_TYPE & sss->obj_type_set);
						sss->w			= head_obj_status_table[tama_index].w;
						sss->h			= head_obj_status_table[tama_index].h;
						sss->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
						sss->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
					}
					/* --- �ėp�X�v���C�g�`�� */
					rotate_and_zoom_check_blit(sss);
				}
			}
		//	sss += sizeof(OBJ);
//			sss = sss->next;/*��*/
		}
	}
}
#endif


/*---------------------------------------------------------
	�I�u�W�F�o���N�V�X�e��
	-------------------------------------------------------
	�o���N�ݒ�𔽉f������
	�I�u�W�F�N�g�̃O���t�B�b�N�ⓖ���蔻���ύX����B
	�e�O���ƒe�����蔻���ύX����B
---------------------------------------------------------*/

global void reflect_sprite_spec(OBJ *sss, u32 bank_offset)
{
//	int num = TEX_06_BULLET;
	VIRTUAL_OBJ_STATE *head_obj_status_table;
//	my_resource[TEX_06_BULLET].object_table_head = (obj_status_table+(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE));
//	my_resource[TEX_06_BULLET].object_table_head = (obj_status_table+(OBJ_BANK_01_ITEM*OBJ_BANK_SIZE));
//	head_obj_status_table = my_resource[TEX_06_BULLET/*num*/].object_table_head;
	head_obj_status_table = (obj_status_table+(bank_offset));
	{
		int tama_index = (OBJ_Zff_MASK_SUB_TYPE & sss->obj_type_set);
		sss->w			= head_obj_status_table[tama_index].w;
		sss->h			= head_obj_status_table[tama_index].h;
		sss->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
		sss->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
	}
}


/*---------------------------------------------------------
	���ڕ`��
---------------------------------------------------------*/

static void common_transfer_objects_00_tama_blit_non_reflect(
//	OBJ *sss,
//TEX_06_BULLET int num,
	int obj_group)//, VIRTUAL_OBJ_STATE *head_obj_status_table)
{
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_1024_MAX; ii++)/* �S�����ׂ�B */
	{
		OBJ *sss;
		sss = &obj99[OBJ_HEAD_00_0x0000_TAMA+ii];
	//
		if (JYUMYOU_NASI < sss->jyumyou)/* ����� */
		{
			if (/*SP_BULLET*/obj_group/*OBJ_Z04_TAMA*/ & sss->obj_type_set)
			{
				/* --- �ėp�X�v���C�g�̕`�� */
				rotate_and_zoom_check_blit(sss);
			}
		}
	}
	//	s_blit_all_objects();/*PRIORITY_05_BULLETS*/
}


#if 1
/*---------------------------------------------------------
	�{�X�� HP �\���̎q�֐�
---------------------------------------------------------*/
extern void dec_print_format( unsigned int num, int size, char *my_str);


static u8 es_panel[4];

static int draw_boss_hp_value_set;

static int boss_life_value;

static void check_draw_boss_hp_value(void)
{
	OBJ *h;
	h		= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
	#if (1)/*(r36�v��Ȃ��H)*/
	if (NULL == h)
	{
		return;/* �{�X�������ꍇ�͉������Ȃ� */
	}
	#endif
	boss_life_value = (h->base_hp);
	#if 0
	if (0 > boss_life_value)	return;/* �����̏ꍇ�͉������Ȃ� */
	if (9*(4*8*1024) < boss_life_value) return;/* �͈͊O�̏ꍇ�͉������Ȃ� */
	#else
//	if (0 != ((boss_life_value)&(0xffffc000)) ) return;/* �͈͊O�̏ꍇ�͉������Ȃ� */
	if (0 > boss_life_value)
	{
		boss_life_value = 0;	//	/* �܂�������(???) */
	}
	if (0==(boss_life_value/*+bo ss_life_value*/))
	{
		return;/* �͈͊O�̏ꍇ�͉������Ȃ� */
	}
	#endif
//	s_cg_game_boss_cx256 = (h->center.x256);/*(???)141477*/
//	s_cg_game_boss_cy256 = (h->center.y256);/*(???)141477*/
// 0001 1111 1110 0000;  >>=(2+3);		0000 0000 1111 1111; 0xff;
// 0111 1111 1000 0000;  >>=(2+3+2);	0000 0000 1111 1111; 0xff;

//	draw_boss_hp_value_set = ((boss_life_value & 0x03fc)>>2);/* �{�Xhp�`��l */
//	draw_boss_hp_value_set = ((boss_life_value & 0x1fe0)>>(2+3));/* �{�Xhp�`��l */
//	draw_boss_hp_value_set = ((boss_life_value & 0x7f80)>>(7));/* (2+3+2)�{�Xhp�`��l(-r35u1) */
//	draw_boss_hp_value_set = ((boss_life_value & 0x1fe00)>>(9));/* (2+3+2+2)�{�Xhp�`��l(r35u2-) */
	draw_boss_hp_value_set = ((boss_life_value>>(9)) & 0xff);/* (2+3+2+2)�{�Xhp�`��l(r35u2-) */
	if (cg.draw_boss_hp_value < (draw_boss_hp_value_set))
			{	cg.draw_boss_hp_value++;	}
	else	{	cg.draw_boss_hp_value--;	}

}



/*---------------------------------------------------------

---------------------------------------------------------*/
				#if 000
				card.card_timer 	= byou64(60);		/* (�Ƃ肠����) */
				h->base_hp			= (0);			/* (�Ƃ肠����) */
				#endif

static void set_boss_gauge(void)
{
	OBJ *h;
	h		= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
//
	#if 1/*�{�X���Ԍo��*/
	unsigned char boss_draw_timer_low	= ((card.card_timer)&0x3f);/* */
	unsigned int boss_draw_timer_value	= ((card.card_timer)>>(6));/* */
	#endif
	//	99 �ȏ�� 99 �\��
	if (99<boss_draw_timer_value)
	{
		boss_draw_timer_value = 99;
	}

/*
	�J�E���^�ɂ��āF
	�J�E���^�� 9 ����_�Ł�����炷�Ƃ�������
	�S���I�Ɏ󂯂Ƃ��悤�ɂ��Ă���B
	�����{���� 9 ���珈������Ɛl�Ԃ̊��o�ɍ���Ȃ��B
//
	���� 10 (�̏I���)����炷�B����� 10 �̎n�߂� 9 �̏I���Ȃ̂ŁA
	9 ������Ă���Ƃ݂Ȃ���B(������Y���͂Ȃ�)
//
	�_�ł� 10 ����_�ł�����B����� 10 ����{���ɓ_�ł��Ă���B
	���Ӑ[������� 10 �������Ă���B(������O�� 10 �������Ă���)
	�������l�Ԃ̊��o���炷��Ɓu 9 ����_�ł��Ă���悤�Ɋ�����v
	�l�Ԃ̎��o�͗\���������Ƌ�؂�ɔ������Ȃ��B
//
	11 11 11 10 10 10 09 09 09 08 08 08 (���Ԃ̗���)
	�� �� �� �� �� �� �� �� �� �� �� �� (���� 10 (�̏I���)����炷�B������Y���͂Ȃ�)
	�_ �_ �_ �_ �_ �� �_ �_ �� �_ �_ �� (�_�ł� 10 ����_�ł�����B�l�Ԃ̊��o���炷��Ɓu 9 ����_�ł��Ă���悤�Ɋ�����v)
				   ��
*/

	if (0==boss_draw_timer_low)
	{
		/* �J�E���g 9 ���特��炷�D */
	//	if ((10  )>boss_draw_timer_value)	/* (10	)==�ݒ�l 10 �ŁA�J�E���g 8 ���特����悤�ɕ�������D */
		if ((10+1)>boss_draw_timer_value)	/* (10+1)==�ݒ�l 11 �ŁA�J�E���g 9 ���特����悤�ɕ�������D */
		{
			voice_play_menu(VOICE10_COUNT_TIMER, TRACK06_ITEM/*TRACK03_IVENT_DAN*/);/*(�e�L�g�[)*/
		}
	}


	/* boss hp �摜�̕������� / gauge stringth x width */
//	#define HPGAUGE_X_OFS (50)
//	#define HPGAUGE_Y_OFS (6)
	#define HP_FONT_X_OFS (4)	/*(50-(8*5)-3)*/	/*(37+10) dx*/
	#define HP_FONT_Y_OFS (1)/*(0+0) dy*/
	//
	#define BOSS_TIMER_X_OFS (PPP-16-4)/* dx*/
	#define BOSS_TIMER_Y_OFS (1)/*(0+0) dy*/
	/* 1024�ȏ�̒l�͐����ŕ\�� */
	{	char buffer[8/* 4 3*/];
	#if 1
		//	�X�y���c�莞�ԕ\��
		if (
			//	(9<boss_draw_timer_value) ||	/* 10�ȏ�͖������ŕ\�� */		/* �J�E���g 8 ����_�ł��Ă�悤�Ɍ�����D */
				(10<boss_draw_timer_value) ||	/* 11�ȏ�͖������ŕ\�� */		/* �J�E���g 9 ����_�ł��Ă�悤�Ɍ�����D */
				(20<boss_draw_timer_low)		/* �_�� 20=(64/3) */
			)
		{
			strcpy(buffer, "00");
			dec_print_format( (boss_draw_timer_value), 2, (char *)buffer);
			es_panel[2] = (buffer[0]&0x0f); 	/* �c��J�[�h����10�̌� */
			es_panel[3] = (buffer[1]&0x0f); 	/* �c��J�[�h����1�̌� */
		}
	#endif
	//	�c�胉�C�t�\��
	//	es_panel[1] = ((boss_life_value>>(13))&0x0f);	/* (10+3)�{�X�̗͖ڈ� */
	//	es_panel[1] = ((boss_life_value>>(15))&0x0f);	/* (10+3+2)�{�X�̗͖ڈ�(-r35u1) */
		es_panel[1] = ((boss_life_value>>(17))&0x0f);	/* (10+3+2)�{�X�̗͖ڈ�(r35u2-) */
	//	es_panel[0] = (10);/* 10(r38)"ene my" �{�X�̈ʒu�\�� */
	//	es_panel[0] = (13);/* 13(r39)"ene my" �{�X�̈ʒu�\�� */
		es_panel[0] = (8+((PANEL_STR_ene_my)&0x0f));/* 13(r39)"ene my" �{�X�̈ʒu�\�� */
	}
}


/*---------------------------------------------------------
	���ڕ`��
---------------------------------------------------------*/

static void gu_draw_score_chache(void)
{
	VIRTUAL_OBJ_STATE *head_obj_status_table;
	head_obj_status_table = my_resource[TEX_07_FRONT/*num*/].object_table_head;
//
	static	/*TGameSprite*/OBJ gu_bbb_sprite_pool[1];
	/*TGameSprite*/OBJ *obj;
	obj = &gu_bbb_sprite_pool[0];
	int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		SCORE_CHACHE_DATA *aaa;
		aaa = &(score_number_chache[i]);
	//	if (SCORE_DELETE < aaa->time_out_color8888)
		if (0x80000000 & (aaa->time_out_color8888) )
		{
			aaa->time_out_color8888 -= 0x01000000;	// ���Ԍo��
			obj->color32		= ((aaa->time_out_color8888));		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
			/* --- �I�u�W�F�N�g�\���ݒ� */
			obj->center.y256			= ((aaa->y256));	aaa->y256 -= 50;	/* ��Ɉړ� */
			obj->center.x256			= ((aaa->x256));	/*x 512/256 */
			{
		//	int tama_index = ((6*8)+3)+es_panel[i]/*(aaa->number)*/;
			int tama_index = ((6*8)+6)+(aaa->number);
			obj->w			= head_obj_status_table[tama_index].w;
			obj->h			= head_obj_status_table[tama_index].h;
			obj->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
			obj->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
			}
			#if (1==USE_ZBUFFER)
			obj->priority	= sss->m_priority;			/*aaa*/ 	/* �\���D�揇�� */
			#endif/* (1==USE_ZBUFFER) */
			{
				/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
				render_object_no_rot_zoom(obj);
			}
		}
	}
//
	/* [ �{�X�̗̑̓`�F�b�N ] */
	//	if (B01_BA TTLE == pd_bo ssmode)
	//	if ((STATE_FLAG_0x0800_IS_BOSS|0) == (cg.state_flag&(STATE_FLAG_0x0800_IS_BOSS|STATE_FLAG_0x0200_IS_KAIWA_MODE)))
	if (SCENE_NUMBER_0x0800_BOSS_TATAKAU==(cg.state_flag&(SCENE_NUMBER_MASK)))
	{
		check_draw_boss_hp_value();
	}
//
	if (0!=cg.draw_boss_hp_value)
	{
		/* [ �{�X�̗͕̑\�� ] */
		{
			set_boss_gauge();
		}
		/* FRONT �ʂ��Ԏ؂肵�āA�X�R�A�\�� */
	/* ������\�� */
		/* �{�X�̈ʒu��\�� */
		for (i=0; i<4; i++)
		{
			enum
			{
				my_pos_cx = 0,
				my_pos_cy,
				my_pos_MAX
			};
			/* ���W��obj�̒��S���W�A */
			static const int ttt_aaa_tbl[4][my_pos_MAX] =
			{
				/* cx256,					cy256 */
				{t256(256+4),				t256(272-(10/2)-2)},	/* "ene my" */
				{t256(50+4),				t256(1+(10/2))},		/* "boss" */
				{t256((GAME_WIDTH-16-4)),	t256(1+(10/2))},		/* "timex1"8x10���[�}���� */
				{t256((GAME_WIDTH-16-4+8)), t256(1+(10/2))},		/* "timex10"8x10���[�}���� */
			};
			/* --- �I�u�W�F�N�g�\���ݒ� */
			obj->center.y256			= ttt_aaa_tbl[i][my_pos_cy];//((aaa->center.y256)); //	aaa->center.y256 -= 50; /* ��Ɉړ� */
			obj->center.x256			= ttt_aaa_tbl[i][my_pos_cx];//((aaa->center.x256)); /*x 512/256 */
			obj->color32		= 0xffffffff;		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
			if (0==i)
			{
				OBJ *zzz_player;
				zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
				OBJ *obj_boss;
				obj_boss	= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(�{�X�{��)*/
					obj->center.x256			= (obj_boss->center.x256);
				if (t256(32)<abs((obj_boss->center.x256)-(zzz_player->center.x256)))
				{
				//	obj->center.x256			= (obj_boss->center.x256);
					;
				}
				else
				{
				//	obj->center.x256			= (t256(512-128));
// ���			obj->color32		= 0x77ffffff;		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
					obj->color32		= 0x33ffffff;		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
					/* ���ʂ̔��������ƌ��ɂ�������̂ŁA���Ȃ蔖������B */
				}
			}
			{
		//	int tama_index = ((5*8)+4)+(9);//(aaa->number);
		//	int tama_index = ((5*8)+3)+(9)/*(aaa->number)*/;
//			int tama_index = ((5*8)+3)+es_panel[i]/*(aaa->number)*/;		//(r38)
			int tama_index = ((5*8)/*+3*/)+es_panel[i]/*(aaa->number)*/;	//(r39)
			obj->w			= head_obj_status_table[tama_index].w;
			obj->h			= head_obj_status_table[tama_index].h;
			obj->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
			obj->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
			}
			#if (1==USE_ZBUFFER)
			obj->priority	= sss->m_priority;			/*aaa*/ 	/* �\���D�揇�� */
			#endif/* (1==USE_ZBUFFER) */
			{
				/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
				render_object_no_rot_zoom(obj);
			}
		}
	}
}
#endif

	/* ���ʕ��� */
	#include "render/gu_render.h"

	/* �e�N�X�`���ό`�ɂ��[��3D �����I�ɖ����Ȃ�\��B */
	#include "render/gu_haikei_03_bg_fake_3D.h"

	/* "�i����"�̗\��B */
	#include "render/gu_haikei_02_bg_eientei.h"

	/* 256x256�e�N�X�`����~���l�߂�2D�w�i��`�� */
	#include "render/gu_haikei_00_bg_2D_256.h"

	/* �{�X�̌��̖����w(��)��`�� */
	#include "render/gu_render_boss_big_mahoujin.h"

	/* �������{��(�J�[�h)�������ꍇ�ɏo��B�����̃��S��`�� */
	#include "render/gu_render_spell_logo.h"

	/* SDL��ʂ�`�� */
	#include "render/gu_render_SDL_screen.h"
	#include "render/gu_render_tache_screen.h"/*(�����G�̃e�X�g)*/

	#if (0)/*(-r34)*/
//	#include "render/gu_render_serifu_moji.h"	/* ��b�̕�����`�� */
//	#include "render/gu_render_supeka_name.h"	/* �{�X�̃J�[�h����`�� */
	#else
	/* (r35)�����̕���(�Ɨ����C���`��V�X�e���֕ύX) */
	#include "render/gu_render_kanji_system.h"
	#endif

	/* �X�R�A�p�l���ɂȂ�\��̕���(����SDL) */
	/* �{�X�̗͕`��̂�(����Gu) */
	#include "render/gu_render_score_window.h"
//	/* �������̂���ӃE�B���h�E�g */
	#include "render/gu_render_serifu_window.h"


	/* ���C���e�X�g(�܂��g�p���ĂȂ�) */
//	#include "render/gu_render_line_test.h"
	#include "render/gu_render_laser_test.h"

	/* 3D�̃e�X�g */
	#include "render/gu_haikei_01_bg_3D_test01.h"
	#include "render/gu_haikei_01_bg_3D_test02.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

static void gu_draw_serifu_haikei(void)
{
	if (0 <= ml_font[(0)].haikei)/*(-1�Ŗ���)*/
	{
		gu_draw_rect_window(ml_font[(0)].haikei);
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void gu_draw_font_texture_transfer(void)
{
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*�����dou ble buffer???*/
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	sceGuTexImage(0, 512, 512, 512, cb.kanji_window_screen_image);
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
//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (cg.conv_bg_alpha), 0xffffffff);
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
//	sceGuTexFilter(GU_LINEAR, GU_LINEAR);//test
}


/*---------------------------------------------------------
	�Q�[�����̕`�揈�����̂P
	---------------------------------------------------------
	�e���v���C�I���e�B�[�����ɂ�����̂��v���C�I���e�B�[���ɕ`���B
	�v���C�I���e�B�[���Q�DBG ��ʂ�`��
	�v���C�I���e�B�[���R�D�w�i��Q����`��
	�v���C�I���e�B�[���S�D�����w��`��
	�v���C�I���e�B�[���T�D�|���S�����C���ʂ�`��(���쐬)
	�v���C�I���e�B�[���U�D���@��`��(A)
	�v���C�I���e�B�[���U�D���@��`��(B)
	�v���C�I���e�B�[���V�D���C���e�X�g��ʂ�`��
	�v���C�I���e�B�[���W�D�G��`��
	�v���C�I���e�B�[���X�DITEM��`��
	�v���C�I���e�B�[��10�D�G�e��`��
---------------------------------------------------------*/

//static void callback_gu_draw_haikei_all(void)
static void gu_blit_render_screen_01(void)
{
/* -- �v���C�I���e�B�[���QBG ��ʂ�`�� */
	#if (1)
	{
		gu_set_texture(TEX_00_BACK_GROUND);
	//	sceGuEnable(GU_CLIP_PLANES);
	//	#if (1==USE_F3D)
	//	gu_draw_bg_fake3D();
	//	#else
	//	gu_draw_bg_2D();
	//	#endif
		(cb.callback_gu_draw_haikei)(); //callback_gu_draw_haikei_all();
	//	sceGuDisable(GU_CLIP_PLANES);
	}
	#endif
/* -- �v���C�I���e�B�[���R�D�w�i��Q����`�� */
	#if (1)
	#endif
/* -- �v���C�I���e�B�[���S�D�����w��`�� */
	#if (1)
	{
		if (0!=cg.draw_boss_hp_value)
		{
			gu_set_texture(TEX_02_MAHOUJIN);	/* �e�N�X�`�����Z�b�g */
			gu_draw_big_maho_jin();
		}
	}
	#endif
/* -- �v���C�I���e�B�[���T�D�|���S�����C���ʂ�`�� */
	#if (0)
	{
		/* �����w�摜�̈ꕔ���Ԏ؂肷��\�� */
		/* �p�`�F���[�U�[ */
	}
	#endif
/* -- �v���C�I���e�B�[���U�D���@��`�� */
	#if (1)
//	if (NULL != callback_gu_draw_haikei)	//���pif (0!=dr aw_bg_screen)
	{
		gu_set_texture(TEX_03_JIKI);

		/*jiki_transfer_object();*/
		{
		//	common_transfer_objects(sprite_list000_head, TEX_03_JIKI, OBJ_Z01_JIKI_GET_ITEM, obj_status_table+(2*4*8*8));
		//	common_transfer_objects(sprite_list000_head, TEX_03_JIKI, OBJ_Z01_JIKI_GET_ITEM, obj_status_table+(8*8)+((cg_game_select_player)<<6) );
			my_resource[TEX_03_JIKI].object_table_head = (obj_status_table+(OBJ_BANK_01_REIMU_A*OBJ_BANK_SIZE)+((cg_game_select_player)<<6));
			/* ���@���ڕ`��(���@��) */
			common_transfer_objects_blit(OBJECT_TYPE_02_KOTEI, /*sprite_list000_head,*/ TEX_03_JIKI, OBJ_Z01_JIKI_GET_ITEM );
			/* ���̑� */
		//	TGameScreen_ClearSprite();
			#if 1/*�V�X�e���̎��@�ʕ`��(�G���Ƃ��ēo�^��������) */
			common_transfer_objects_blit(OBJECT_TYPE_01_TEKI, /*sprite_list000_head,*/ TEX_03_JIKI, OBJ_Z01_JIKI_GET_ITEM );/* ���@�e��{�����̗����G���̕`�� */
			#endif
		}
	//	s_blit_all_objects();/*PRIORITY_02_PLAYER*/
	}
	#endif /*(000)*/




/* -- �v���C�I���e�B�[���V�D���C���e�X�g��ʂ�`�� */
	#if (1==LINE_TEST)
	if (NULL != cb.callback_gu_draw_haikei) //if (0!=dr aw_bg_screen)
	{
		/* -- ���C���e�X�g��ʂ�`�� */
	//	if (cg.laser_mode)
		if (LASER_TYPE_00_OFF != (lz[0].laser_type))/* lz[0]==0;�̏ꍇ�A���ׂ�off�B(���[�U�[���[�h 0:off, 1:on) */
		{
		//	gu_blit_lines();
			gu_blit_laser_test();
		}
	}
	#endif /*(000)*/
	#if 000
//	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//	[ ��SDL��ʕ`�� ]
//	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	#endif
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
		//	common_transfer_objects(sprite_list000_head, TEX_04_TEKI, OBJ_Z04_TAMA, obj_status_table+0);
			#if 0000/* �ʂŐݒ� */
			my_resource[TEX_04_TEKI].object_table_head = (obj_status_table+(OBJ_BANK_11_ZAKO_STAGE1*OBJ_BANK_SIZE));
			#endif
		//	TGameScreen_ClearSprite();
			common_transfer_objects_blit(OBJECT_TYPE_01_TEKI, /*sprite_list000_head,*/ TEX_04_TEKI, (OBJ_Z02_TEKI) );
		}
	//	s_blit_all_objects();/*PRIORITY_05_BULLETS*/
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
		//	common_transfer_objects(sprite_list444_head, TEX_06_BULLET, OBJ_Z04_TAMA, obj_status_table+0);
			#if 000/* �ʂŐݒ� �g���ĂȂ�*/
			my_resource[TEX_06_BULLET].object_table_head = (obj_status_table+(OBJ_BANK_01_ITEM*OBJ_BANK_SIZE));
			#endif/*000*/
		//	TGameScreen_ClearSprite();//???
			common_transfer_objects_00_tama_blit_non_reflect(/*sprite_list444_head,*/ /*TEX_06_BULLET,*/ (OBJ_Z03_ITEM) );
		}
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- �v���C�I���e�B�[��10�D�G�e��`�� */
	/* �� �e�ʃG�t�F�N�g */
	#if (1)
	/*
		VRAM���g���Ȃ��ƁA�^�C�~���O�̊֌W�ł߂�ǂ������H
	*/
	/* 0==�����[�X, 1==�J���� */
//	#define KAIHATSU_MODE (0)
//	#if (0==KAIHATSU_MODE)
//	if (shooting_game_core_work == cb.main_call_func)/* game_core�̏ꍇ�̂ݒe��`��(continue��ʂƂ��ł͕`���Ȃ�) */
//	#endif
	{
	//	gu_set_texture(TEX_06_BULLET);	/* �e�N�X�`�����Z�b�g */
		#if (1)
		/* �S�e�z��͕�������\�� �Ȃ̂Ő�p�ɏ���������\�� */
		/*bullet_transfer_object();*/
		{
		//	common_transfer_objects(sprite_list444_head, TEX_06_BULLET, OBJ_Z04_TAMA, obj_status_table+0);
			#if 000/* �ʂŐݒ� �g���ĂȂ�*/
			my_resource[TEX_06_BULLET].object_table_head = (obj_status_table+(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE));/*�g���ĂȂ�*/
			#endif/*000*/
		//	TGameScreen_ClearSprite();//???
			common_transfer_objects_00_tama_blit_non_reflect(/*sprite_list444_head,*/ /*TEX_06_BULLET,*/ OBJ_Z04_TAMA );
		}
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
}


static u32 gu_bg_set_clear_color;
global void gu_set_bg_u32_clear_color(u32 set_u32_clear_color)
{
	gu_bg_set_clear_color = set_u32_clear_color;
}

/*---------------------------------------------------------
	��ʕ`��
---------------------------------------------------------*/

	#define USE_VSYNC			(0)
/* 0:VSYNC���Ȃ��B USE_VSYNC ������v-sync 1 �� on �����A�܂��x���̂Ŗ����B */
	#define USE_MAX_GU_TIME 	(0)
//	#define USE_MAX_GU_TIME 	(1)
/* 0:USE_MAX_GU_TIME [Gu���Ԃ��ő�Ɏ��^�C�v](�F�X��������)
	Gu�̓]�����l���āAGu���łȂ�ׂ����񓮍삳����悤�Ƀ^�C�~���O���l�������́B
	psp�Ŏn�߂Ď��������̂́A http://www.fumi2kick.com/pspapp/ ���̕ӁB
	SDL���p����ꍇ(�\�t�g�`��)�́A(�^�C�~���O�I��)��΂ɊԂɍ���Ȃ��B
	(�t���[���o�b�t�@��݂��ăX���b�v������ΊԂɍ����B�܂�g���v���o�b�t�@�B�A���������H�����A�x���Ȃ邩���H)
	(Gu�`�悵�����Ȃ��Ȃ�A1:USE_MAX_GU_TIME�ŏ\���ł͂���B)
	�����SDL���p����̂ŁA1:USE_MAX_GU_TIME�ɏo���Ȃ��B(SDL�g��Ȃ��Ȃ�1:USE_MAX_GU_TIME�ɏo����)
*/
#include "gu_video_frame_normal.h"
#include "gu_video_frame_only_loading.h"

//#include "render/dxp_test.h"

