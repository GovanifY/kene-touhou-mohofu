
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _GU_DRAW_SCREEN_H_
#define _GU_DRAW_SCREEN_H_



extern void psp_load_texture( int num );
//static void psp_load_texture(int num);





/*	*/
//#if (1==USE_16BIT_BLIT)
//	#define USE_COLOR_16_AND_32 0
//#endif
	#define USE_COLOR_16_AND_32 1



//#define USE_SDL_image 1
#define USE_SDL_image 0
#if (1==USE_SDL_image)
	//#include "_SDL_image.h"//#include "_SDL_image.h"
	#define MY_DIB_SURFACE	S_DL_Surface
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	unused1
#else
	#include "graphics00.h"
	#define MY_DIB_SURFACE	my_image
//	#define MY_DIB_DATA 	data
	#define MY_DIB_DATA 	pixels
	#define MY_DIB_WK512	wk512
#endif

//#define USE_SWIZZLE 0
#define USE_SWIZZLE 1


#if (0==USE_32BIT_DRAW_MODE)
	/* 0: 32bit�`�� */
	/* 1: 16bit�`�� */
	//#define USE_16BIT_BLIT 0
	#define USE_16BIT_BLIT 1

	//#define USE_BLIT_COLOR_BIT 32
	#define USE_BLIT_COLOR_BIT 16

#else
		/* �F32bit���[�h */
/* 32bit�̂Ă��� */
	#define USE_16BIT_BLIT 0
	#define USE_BLIT_COLOR_BIT 32
#endif




//#define TW128 128/* �e�e�N�X�`���̕� */
//#define TH128 128/* �e�e�N�X�`���̍��� */
//#define WK512 512/* �e�e�N�X�`����`�悷��ꍇ�ɁA�`�惏�[�N�G���A�̕� */

/* �傫�ȃX�v���C�g��`�悷��Ƃ��̕����T�C�Y */
#define SLICE_64_SIZE	(64)

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
//			//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//			//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//				#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
				#define TEXTURE_FLAGS5650		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
				#define TEXTURE_FLAGS5650_C32	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//	#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
				#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_4444 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#else
		#if 0
				/* 32bit�`�� */
				#define TEXTURE_FLAGS5650		(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
				#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#else
			/* 32bit�̂Ă��� */
				/* 32bit�`�� */
				#define TEXTURE_FLAGS5650		(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
				#define TEXTURE_FLAGS5650_C32	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
				#define TEXTURE_FLAGS4444		(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)


		#endif

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





/*static*/extern	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];















/* �Ǘ�����e�N�X�`���[(�������ɓ����Ƀ��[�h����\���̂���e�N�X�`���[) */
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
	TEX_09_TACHIE_L,			/* �����GL */
	TEX_10_TACHIE_R,			/* �����GR */
//	TEX_11_MESSAGE, 			/* [TEX_07_FRONT�ƗZ������\��������(512x512�e�N�X�`���H)]���b�Z�[�W�Œ蕶�� */
	TEXTURE_MAX 				/* --- �Ǘ�����ő�e�N�X�`���[�� */
};

/*---------------------------------------------------------
	�X�N���[���Ǘ�
---------------------------------------------------------*/

/* �e�N�X�`�����X�v���C�g�Ƃ��Ďg���ꍇ�̊Ǘ��e�[�u������ */
typedef struct
{
	unsigned char	u;	/* x�e�N�X�`���ʒu�A���W */
	unsigned char	v;	/* y�e�N�X�`���ʒu�A���W */
//
	unsigned char	w;	/* width �� */
	unsigned char	h;	/* height ���� */
} VIRTUAL_OBJ_STATE;	/* �X�v���C�g���(�̑傫�����Ǘ�) */

/* �e�N�X�`�����ƂɊǗ� */
typedef struct
{
	MY_DIB_SURFACE		*my_texture;		/* �e�N�X�`���摜 */
	int 				texture_width;		/* �e�N�X�`���� */
	int 				texture_height; 	/* �e�N�X�`������ */
	int 				buffer_width;		/* 512 �Œ�H */
//
	int 				color_format;		/* �ϊ��ς݉摜�`�� */
	/*	�ǂݍ��ݎ���ARGB8888����摜�ϊ����s�����A
		�ǂ������`���ɉ摜�ϊ����邩�w�肷��B
		16bit�`���ł� ABGR0565 �Ƃ� ABGR1555 �Ƃ� ABGR4444 ������B
	 */
	int 				hh;/*�\��*/
	VIRTUAL_OBJ_STATE	*object_table_head; 	/* �X�v���C�g�Ǘ��e�[�u���̐擪 */
	char				*file_name;/* �t�@�C���� */
} MY_TEXTURE_RESOURCE;

/*static*/extern MY_TEXTURE_RESOURCE	my_resource[TEXTURE_MAX];






#endif /* _GU_DRAW_SCREEN_H_ */

