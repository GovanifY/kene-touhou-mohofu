
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	Portable psp Gu Customized library.
---------------------------------------------------------*/

/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

//#include "guInternal.h"

#include <pspkernel.h>
#include <pspge.h>

#include <pspdisplay.h>

#ifndef __guInternal_h__
#define __guInternal_h__

#include "pspgu.h"
#include "pgc.h"

/*(内部拡張設定)*/
#if (1)/*(模倣風用設定)*/
	#define USE_EXTENSION_01		(0)/*sceGuTerm*/
	#define USE_EXTENSION_02		(0)/*Rev2*/
	#define USE_DRAW_BUFFER_LIST	(0)/*DrawBufferList*/
	#define USE_EXTENSION_04		(0)/*DXPv1*/
	#define USE_EXTENSION_05		(0)/*DXPv2*/
	#define PGC_ON_FOR_MOHOUFU		PGC_ON
#endif
#if (0)/*(某自作ソフト用設定)*/
	#define USE_EXTENSION_01		(1)/*sceGuTerm*/
	#define USE_EXTENSION_02		(1)/*Rev2*/
	#define USE_DRAW_BUFFER_LIST	(1)/*DrawBufferList*/
	#define USE_EXTENSION_04		(0)/*DXPv1*/
	#define USE_EXTENSION_05		(0)/*DXPv2*/
	#define PGC_ON_FOR_MOHOUFU		PGC_OFF
#endif
#if (0)/*(DXP用設定)*/
	#define USE_EXTENSION_01		(1)/*sceGuTerm*/
	#define USE_EXTENSION_02		(1)/*Rev2*/
	#define USE_DRAW_BUFFER_LIST	(1)/*DrawBufferList*/
	#define USE_EXTENSION_04		(1)/*DXPv1*/
	#define USE_EXTENSION_05		(1)/*DXPv2*/
	#define PGC_ON_FOR_MOHOUFU		PGC_OFF
#endif
typedef void (*GuCallback)(int);

typedef struct
{
	GuCallback sig;
	GuCallback fin;
	short signal_history[16];
	int signal_offset;
	int kernel_event_flag;
	int ge_callback_id;

	GuSwapBuffersCallback swapBuffersCallback;
	int swapBuffersBehaviour;
} GuSettings;

typedef struct
{
	unsigned int *start;
	unsigned int *current;
	int parent_context;
} GuDisplayList;

typedef struct
{
	GuDisplayList list;
	int scissor_enable;
	int scissor_start[2];
	int scissor_end[2];
	int near_plane;
	int far_plane;
	int depth_offset;
	int fragment_2x;
	int texture_function;
	int texture_proj_map_mode;
	int texture_map_mode;
	int sprite_mode[4];
	unsigned int clear_color;
	unsigned int clear_stencil;
	unsigned int clear_depth;
	int texture_mode;
} GuContext;

typedef struct
{
	int pixel_size;
	int frame_width;
	void *frame_buffer;
	void *disp_buffer;
	void *depth_buffer;
	int depth_width;
	int width;
	int height;
} GuDrawBuffer;

typedef struct
{
	/* row 0 */
	unsigned char enable;	// Light enable
	unsigned char type; 	// Light type
	unsigned char xpos; 	// X position
	unsigned char ypos; 	// Y position

	/* row 1 */
	unsigned char zpos; 	// Z position
	unsigned char xdir; 	// X direction
	unsigned char ydir; 	// Y direction
	unsigned char zdir; 	// Z direction

	/* row 2 */
	unsigned char ambient;		// Ambient color
	unsigned char diffuse;		// Diffuse color
	unsigned char specular; 	// Specular color
	unsigned char constant; 	// Constant attenuation

	/* row 3 */
	unsigned char linear;		// Linear attenuation
	unsigned char quadratic;	// Quadratic attenuation
	unsigned char exponent; 	// Light exponent
	unsigned char cutoff;		// Light cutoff
} GuLightSettings;

//extern unsigned int gu_current_frame;
//extern GuContext gu_contexts[3];
//extern int ge_list_executed[2];
//extern void *ge_edram_address;
//extern GuSettings gu_settings;
//extern GuDisplayList *gu_list;
//extern int gu_curr_context;
//extern int gu_init_flag;
//extern int gu_display_on;
//extern int gu_call_mode;
//extern int gu_states;
//extern GuDrawBuffer gu_draw_buffer;
//
//extern unsigned int *gu_object_stack[];
//extern int gu_object_stack_depth;
//
//extern GuLightSettings light_settings[4];

//void pspGuSendCommandi(int cmd, int argument);
//void sendCommandf(int cmd, float argument);
//void pspGuSendStall(void);

//void callbackSig(int id, void *arg);
//void callbackFin(int id, void *arg);
//void resetValues();

#endif

#define TEST0 (0)

//
//static unsigned int gu_current_frame;

static GuDisplayList *gu_list;
//

static GuContext gu_contexts[3];
static int ge_list_executed[2];
static void *ge_edram_address;
static GuSettings gu_settings;

static GuDrawBuffer gu_draw_buffer;
//???static unsigned int *gu_object_stack[32];

// flags
static int gu_curr_context; /* 0, or 1, or 2, only. */
static u32 gu_states;/*(need 22bit)*/
static u8 gu_init_flag;
static u8 gu_display_on;
#if TEST0
static u8 gu_call_mode;
static u8 gu_object_stack_depth;
#endif
//

#if 000
GuLightSettings light_settings[4] =
{
	{
		0x18, 0x5f, 0x63, 0x64,
		0x65, 0x6f, 0x70, 0x71,
		0x8f, 0x90, 0x91, 0x7b,
		0x7c, 0x7d, 0x87, 0x8b
	},

	{
		0x19, 0x60, 0x66, 0x67,
		0x68, 0x72, 0x73, 0x74,
		0x92, 0x93, 0x94, 0x7e,
		0x7f, 0x80, 0x88, 0x8c
	},

	{
		0x1a, 0x61, 0x69, 0x6a,
		0x6b, 0x75, 0x76, 0x77,
		0x95, 0x96, 0x97, 0x81,
		0x82, 0x83, 0x89, 0x8d
	},

	{
		0x1b, 0x62, 0x6c, 0x6d,
		0x6e, 0x78, 0x79, 0x7a,
		0x98, 0x99, 0x9a, 0x84,
		0x85, 0x86, 0x8a, 0x8e
	}
};
#endif

int sceGuSync(int mode, int what)
{
	#if 000
	switch (mode)
	{
	case 0: 	return sceGeDrawSync(what); break;
	case 3: 	return sceGeDrawSync(ge_list_executed[0]);
	case 4: 	return sceGeDrawSync(ge_list_executed[1]);
	default:	case 1: case 2: return (0);
	}
	#else
	return sceGeDrawSync(0);
	#endif
}

void *sceGuGetMemory(int size)
{
	// some kind of 4-byte alignment?
	size += 3;
	size += ((unsigned int)(size >> 31)) >> 30;
	size &= 0xfffffffc; 	// size = (size >> 2) << 2;
	unsigned int *orig_ptr	= gu_list->current;
	unsigned int *new_ptr	= (unsigned int *)(((unsigned int)orig_ptr) + size + 8);

	int lo = ( 8 << 24) |  (((unsigned int)new_ptr) 	  & 0x00ffffff);
	int hi = (16 << 24) | ((((unsigned int)new_ptr) >> 8) & 0x0f0000  );

	orig_ptr[0] = hi;
	orig_ptr[1] = lo;

	gu_list->current = new_ptr;

	if (!gu_curr_context)
	{	sceGeListUpdateStallAddr(ge_list_executed[0], new_ptr);}

	return orig_ptr + 2;
}


#if (1)
	#define SEND_CMD_I(ccc) 			{*(gu_list->current++) = (ccc);}
	#define pspGuSendCommandi(aaa,bbb)	SEND_CMD_I(PXI(aaa,bbb))
#else
	#define SEND_CMD_I(ccc) pgc_send(ccc)
#endif
/*static*/ void pgc_send(u32 argument)
{
	*(gu_list->current++) = (argument );
}
//	/*static*/ //void pspGuSendCommandi(int cmd, int argument)
//	/*static*/ void pspGuSendCommandi(u8 cmd, int argument)
//	{
//		*(gu_list->current++) = (cmd << 24) | (argument & 0x00ffffff);
//	}

//static void sendCommandf(int cmd, float argument)
static void sendCommandf(u8 cmd, float argument)
{
	union
	{
		float f;
		unsigned int i;
	} t;
	t.f = argument;
	pspGuSendCommandi(cmd, ((t.i) >> 8) );
}

static void pspGuSendStall(void)
{
	if (
		#if TEST0
		!gu_object_stack_depth &&
		#endif
		!gu_curr_context)
	{	sceGeListUpdateStallAddr(ge_list_executed[0], gu_list->current);
	}
}

/********************************/
/********************************/
/********************************/
void pgc_set_matrix_PROJECTION(const ScePspFMatrix4 *matrix)
{
	const float *fmatrix = (const float *)matrix;
//	case GU_PROJECTION/*0*/:
	SEND_CMD_I(PXI(62, 0));//sendCommandf(62, 0);
	// 4*4 - most probably projection
	{	unsigned int i;
		for (i=0; i<16; i++)
		{	sendCommandf(63, fmatrix[i]);}
	}
}
void pgc_set_matrix_VIEW(const ScePspFMatrix4 *matrix)
{
	const float *fmatrix = (const float *)matrix;
//	case GU_VIEW/*1*/:
	SEND_CMD_I(PXI(60, 0));//sendCommandf(60, 0);
	// 4*4 -> 3*4 - view matrix?
	{	unsigned int i;
		for (i=0; i<4; i++)
		{	unsigned int j;
			for (j=0; j<3; j++)
			{	sendCommandf(61, fmatrix[j+i*4]);}
		}
	}
}
void pgc_set_matrix_MODEL(const ScePspFMatrix4 *matrix)
{
	const float *fmatrix = (const float *)matrix;
//	case GU_MODEL/*2*/:
	SEND_CMD_I(PXI(58, 0));//sendCommandf(58, 0);
	// 4*4 -> 3*4 - ???
	{	unsigned int i;
		for (i=0; i<4; i++)
		{
			unsigned int j;
			for (j=0; j<3; j++)
			{	sendCommandf(59, fmatrix[j+i*4]);}
		}
	}
}


#if 0
void pgc_set_matrix_TEXTURE(const ScePspFMatrix4 *matrix)
{
	const float *fmatrix = (const float *)matrix;
//	case GU_TEXTURE/*3*/:
	SEND_CMD_I(PXI(64, 0));//sendCommandf(64, 0);
	// 4*4 -> 3*4 - ???
	{	unsigned int i;
		for (i=0; i<4; i++)
		{
			unsigned int j;
			for (j=0; j<3; j++)
			{	sendCommandf(65, fmatrix[j+i*4]);}
		}
	}
}
void sceGuSetMatrix(int type, const ScePspFMatrix4 *matrix)
{
	unsigned int i, j;
	const float *fmatrix = (const float *)matrix;
	switch (type)
	{
	case GU_PROJECTION/*0*/:
		{
			SEND_CMD_I(PXI(62, 0));//sendCommandf(62, 0);
			// 4*4 - most probably projection
			for (i=0; i<16; i++)
			{	sendCommandf(63, fmatrix[i]);}
		}
		break;
	case GU_VIEW/*1*/:
		{
			SEND_CMD_I(PXI(60, 0));//sendCommandf(60, 0);
			// 4*4 -> 3*4 - view matrix?
			for (i=0; i<4; i++)
			{
				for (j=0; j<3; j++)
				{	sendCommandf(61, fmatrix[j+i*4]);}
			}
		}
		break;
	case GU_MODEL/*2*/:
		{
			SEND_CMD_I(PXI(58, 0));//sendCommandf(58, 0);
			// 4*4 -> 3*4 - ???
			for (i=0; i<4; i++)
			{
				for (j=0; j<3; j++)
				{	sendCommandf(59, fmatrix[j+i*4]);}
			}
		}
		break;
	case GU_TEXTURE/*3*/:
		{
			SEND_CMD_I(PXI(64, 0));//sendCommandf(64, 0);
			// 4*4 -> 3*4 - ???
			for (i=0; i<4; i++)
			{
				for (j=0; j<3; j++)
				{	sendCommandf(65, fmatrix[j+i*4]);}
			}
		}
		break;
	}
}
#endif
void sceGuTexScale(float u, float v)
{
	sendCommandf(72, u);
	sendCommandf(73, v);
}
void sceGuTexOffset(float u, float v)
{
	SEND_CMD_I(PXI(74, 0));//sendCommandf(74, u);
	SEND_CMD_I(PXI(75, 0));//sendCommandf(75, v);
}

/********************************/
/********************************/
/********************************/

static void s_sceGuTexFlush(void)
{
	SEND_CMD_I(PXI(203, 0));//sendCommandf(203, 0.0f);
}

void sceGuTexMode(int tpsm, int maxmips, int a2, int swizzle)
{
	GuContext *context = &gu_contexts[gu_curr_context];
	context->texture_mode = tpsm;

	pspGuSendCommandi(194, ((maxmips << 16) | (a2 << 8) | (swizzle)) );
	pspGuSendCommandi(195, tpsm);

	s_sceGuTexFlush();
}


static int s_getExp(int val)
{
	#if (0)
	/* alegrex(pspのcpu)対応の組み込み関数を使う */
	ダメ	return (1 << (32 - __builtin_allegrex_clz(val - 1)));
	#else
	unsigned int i;
	for (i = 9; (i > 0) && !((val >> i) & 1); --i);
	return (i);
	#endif
}

void sceGuTexImage(int mipmap, int width, int height, int tbw, const void *tbp)
{
//	static const /*int*/u8 tbpcmd_tbl[8]	= { 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7 };		// 0x30A18
//	static const /*int*/u8 tbwcmd_tbl[8]	= { 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf };		// 0x30A38
//	static const /*int*/u8 tsizecmd_tbl[8]	= { 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf };		// 0x30A58
//	pspGuSendCommandi(tbpcmd_tbl[mipmap],	((unsigned int)tbp) & 0x00ffffff);
//	pspGuSendCommandi(tbwcmd_tbl[mipmap],	((((unsigned int)tbp) >> 8) & 0x0f0000)|tbw);
//	pspGuSendCommandi(tsizecmd_tbl[mipmap], (s_getExp(height) << 8)|(s_getExp(width)));
	pspGuSendCommandi(/*tbp_cmd*/	(0xa0+mipmap),	  (((unsigned int)tbp)		 & 0x00ffffff) );
	pspGuSendCommandi(/*tbw_cmd*/	(0xa8+mipmap),	(((((unsigned int)tbp) >> 8) & 0x0f0000  )|tbw) );
	pspGuSendCommandi(/*tsize_cmd*/ (0xb8+mipmap),	((s_getExp(height) << 8)|(s_getExp(width))) );
	s_sceGuTexFlush();
}

/********************************/
/********************************/
/********************************/

void sceGuTexFunc(int tfx, int tcc)
{
	GuContext *context = &gu_contexts[gu_curr_context];
	context->texture_function = (tcc << 8) | tfx;

	pspGuSendCommandi(201, (((tcc << 8)|tfx)|context->fragment_2x) );
}

/********************************/
/********************************/
/********************************/
// グーローシェーディングは頂点ごとに光源計算を行い、頂点の色を計算し、三角形の内部を色補間します。
// フォンシェーディングは三角形内部を法線補間し、画素毎に色を計算します。
// pspの色補間機能は多分グーローシェーディング(?)。
// スムースシェーディング、トライリニア補間
// リニアってのは直線の事だからたぶん多分グーローシェーディング(?)。
// グーローシェーディングのon/off
//void pcg_gouraud_shading(u8 mode)
void sceGuShadeModel(int mode)
{
//	#if 0
//	switch (mode)
//	{
//	case 1: 	SEND_CMD_I(PXI(80, 1)); break;
//	default:	SEND_CMD_I(PXI(80, 0)); break;
//	}
//	#else
//	if (0==mode)	{	SEND_CMD_I(PXI(80, 0)); 	}
//	else			{	SEND_CMD_I(PXI(80, 1)); 	}
//	#endif
//	#define GU_FLAT 		(0)
//	#define GU_SMOOTH		(1)
	SEND_CMD_I(PXI(80, mode));
}

/********************************/
/********************************/
/********************************/

void sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix)
{
	pspGuSendCommandi(223, (src | (dest << 4) | (op << 8)) );
	pspGuSendCommandi(224, (srcfix	& 0x00ffffff) );
	pspGuSendCommandi(225, (destfix & 0x00ffffff) );
}

void sceGuColorFunc(int func, unsigned int color, unsigned int mask)
{
	pspGuSendCommandi(216, (func  & 	  0x03) );
	pspGuSendCommandi(217, (color & 0x00ffffff) );
	pspGuSendCommandi(218, mask);
}

void sceGuClearColor(unsigned int color)
{
	GuContext *context = &gu_contexts[gu_curr_context];
	context->clear_color = color;
}

void sceGuAmbientColor(unsigned int color)
{
	pspGuSendCommandi(85, (color & 0x00ffffff) );
	pspGuSendCommandi(88, (color >> 24) );
}

/*
202 CA	TEC 		Texture Environment Color
						0-7: Red Component
						8-15: Green Component
						16-23: Blue Component
---- ---- bbbb bbbb gggg gggg rrrr rrrr
---- ---- 3210 9876 5432 1098 7654 3210
*/
//	PGC_SEND(PGC_TEX_ENV_COLOR,color);
//void sceGuTexEnvColor(unsigned int color)
//{
//	pspGuSendCommandi(202, (color & 0x00ffffff) );
//}

/********************************/
/********************************/
/********************************/

void sceGuOffset(unsigned int x, unsigned int y)
{
	pspGuSendCommandi(76, (x << 4) );
	pspGuSendCommandi(77, (y << 4) );
}

/*
198 C6	TFLT
		Texture Filter
			0-2: Minifying filter
			8-10: Magnifying filter
				000: Nearest
				001: Linear
				100: Nearest; Mipmap Nearest
				101: Linear; Mipmap Nearest
				110: Nearest; Mipmap Linear
				111: Linear; Mipmap Linear
---- ---- ---- ---- ---- -mag ---- -min
---- ---- ---- ---- ---- -098 ---- -210
*/
//			PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);
//			PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);
//void sceGuTexFilter(int min, int mag)
//{
//	pspGuSendCommandi(198, ((mag << 8)|min) );
//	pspGuSendCommandi(198, (min|(mag << 8)) );
//}

void sceGuFrontFace(int order)
{
	if (order)
	{	SEND_CMD_I(PXI(155, 0));}
	else
	{	SEND_CMD_I(PXI(155, 1));}
}

void sceGuViewport(int cx, int cy, int width, int height)
{
	sendCommandf(66, (float)(width>>1));
	sendCommandf(67, (float)((-height)>>1));
	sendCommandf(69, (float)cx);
	sendCommandf(70, (float)cy);
}

/********************************/
/********************************/
/********************************/

void sceGuScissor(int x, int y, int w, int h)
{
	GuContext *context = &gu_contexts[gu_curr_context];
	context->scissor_start[0]	= x;
	context->scissor_start[1]	= y;
	context->scissor_end[0] 	= w-1;
	context->scissor_end[1] 	= h-1;
	if (context->scissor_enable)
	{
		pspGuSendCommandi(212, ((context->scissor_start[1] << 10)|context->scissor_start[0]) );
		pspGuSendCommandi(213, ((context->scissor_end[1]   << 10)|context->scissor_end[0]  ) );
	}
}

#if 000
static void s_sceGuMaterial(int mode, int color)
{
	if (mode & 0x01)
	{
		pspGuSendCommandi(85, color & 0x00ffffff);
		pspGuSendCommandi(88, color >> 24);
	}
	if (mode & 0x02)
	{	pspGuSendCommandi(86, color & 0x00ffffff);}
	if (mode & 0x04)
	{	pspGuSendCommandi(87, color & 0x00ffffff);}
}
void sceGuColor(unsigned int color)
{
	s_sceGuMaterial(7, color);
}
#else
void sceGuColor(unsigned int color)
{
	pspGuSendCommandi(85, color & 0x00ffffff);
	pspGuSendCommandi(88, color >> 24);
	pspGuSendCommandi(86, color & 0x00ffffff);
	pspGuSendCommandi(87, color & 0x00ffffff);
}
#endif

#if 0
void sceGuDisable(int state)
{
	switch(state)
	{
	case GU_ALPHA_TEST: 	SEND_CMD_I(PXI(34, 0)); break;	/*「アルファブレンディング(透明度付き、色重ね合わせ)」無効*/
	case GU_DEPTH_TEST: 	SEND_CMD_I(PXI(35, 0)); break;	/* Zバッファ無効 */
	case GU_SCISSOR_TEST:
		{
			GuContext *context = &gu_contexts[gu_curr_context];
			context->scissor_enable = 0;
			SEND_CMD_I(PXI(212, 0));
			pspGuSendCommandi(213, ((gu_draw_buffer.height-1) << 10)|(gu_draw_buffer.width-1));
		}
		break;
	case GU_STENCIL_TEST:			SEND_CMD_I(PXI(36, 0)); break;
	case GU_BLEND:					SEND_CMD_I(PXI(33, 0)); break;	/* アルファブレンディング無効 */
	case GU_CULL_FACE:				SEND_CMD_I(PXI(29, 0)); break;	/* カリングモード */
	#if TEST0
	case GU_DITHER: 				SEND_CMD_I(PXI(32, 0)); break;
	case GU_FOG:					SEND_CMD_I(PXI(31, 0)); break;
	#endif
	case GU_CLIP_PLANES:			SEND_CMD_I(PXI(28, 0)); break;
	case GU_TEXTURE_2D: 			SEND_CMD_I(PXI(30, 0)); break;	/* テクスチャー無効 */
	#if TEST0
	case GU_LIGHTING:				SEND_CMD_I(PXI(23, 0)); break;
	case GU_LIGHT0: 				SEND_CMD_I(PXI(24, 0)); break;
	case GU_LIGHT1: 				SEND_CMD_I(PXI(25, 0)); break;
	case GU_LIGHT2: 				SEND_CMD_I(PXI(26, 0)); break;
	case GU_LIGHT3: 				SEND_CMD_I(PXI(27, 0)); break;
	#endif
	case GU_LINE_SMOOTH:			SEND_CMD_I(PXI(37, 0)); break;
	case GU_PATCH_CULL_FACE:		SEND_CMD_I(PXI(38, 0)); break;
	case GU_COLOR_TEST: 			SEND_CMD_I(PXI(39, 0)); break;
	case GU_COLOR_LOGIC_OP: 		SEND_CMD_I(PXI(40, 0)); break;
	#if TEST0
	case GU_FACE_NORMAL_REVERSE:	SEND_CMD_I(PXI(81, 0)); break;
	case GU_PATCH_FACE: 			SEND_CMD_I(PXI(56, 0)); break;
	case GU_FRAGMENT_2X:
		{
			GuContext *context = &gu_contexts[gu_curr_context];
			context->fragment_2x = 0;
			pspGuSendCommandi(201, context->texture_function);
		}
		break;
	#endif
	}
	if (state < 22)
	{	gu_states &= ~(1 << state);}
}
#endif
void pgc_scissor_enable(void)
{
//#define GU_SCISSOR_TEST		(2)
		{
			GuContext *context = &gu_contexts[gu_curr_context];
			context->scissor_enable = 1;
			pspGuSendCommandi(212, ((context->scissor_start[1]<<10)|context->scissor_start[0]) );
			pspGuSendCommandi(213, ((context->scissor_end[1]  <<10)|context->scissor_end[0]  ) );
		}
//	if (state < 22)
	{	gu_states |= (1 << (2)/*state*/);}

}
#if 0
void sceGuEnable(int state)
{
	switch(state)
	{
	case GU_SCISSOR_TEST:/*(2)*/
		{
			GuContext *context = &gu_contexts[gu_curr_context];
			context->scissor_enable = 1;
			pspGuSendCommandi(212, (context->scissor_start[1]<<10)|context->scissor_start[0]);
			pspGuSendCommandi(213, (context->scissor_end[1]<<10)|context->scissor_end[0]);
		}
		break;
	//
	#if TEST0
	case GU_LIGHTING:				SEND_CMD_I(PXI(23, 1)); break;
	case GU_LIGHT0: 				SEND_CMD_I(PXI(24, 1)); break;
	case GU_LIGHT1: 				SEND_CMD_I(PXI(25, 1)); break;
	case GU_LIGHT2: 				SEND_CMD_I(PXI(26, 1)); break;
	case GU_LIGHT3: 				SEND_CMD_I(PXI(27, 1)); break;
	#endif
	case GU_CLIP_PLANES:			SEND_CMD_I(PXI(28, 1)); break;
	case GU_CULL_FACE:				SEND_CMD_I(PXI(29, 1)); break;	/* カリングモード */
	case GU_TEXTURE_2D: 			SEND_CMD_I(PXI(30, 1)); break;	/* テクスチャー有効 */
	#if TEST0
	case GU_FOG:					SEND_CMD_I(PXI(31, 1)); break;
	case GU_DITHER: 				SEND_CMD_I(PXI(32, 1)); break;
	#endif
	case GU_BLEND:					SEND_CMD_I(PXI(33, 1)); break;	/* アルファブレンディング有効 */
	case GU_ALPHA_TEST: 			SEND_CMD_I(PXI(34, 1)); break;
	case GU_DEPTH_TEST: 			SEND_CMD_I(PXI(35, 1)); break;
	case GU_STENCIL_TEST:			SEND_CMD_I(PXI(36, 1)); break;
	case GU_LINE_SMOOTH:			SEND_CMD_I(PXI(37, 1)); break;
	case GU_PATCH_CULL_FACE:		SEND_CMD_I(PXI(38, 1)); break;
	case GU_COLOR_TEST: 			SEND_CMD_I(PXI(39, 1)); break;/*カラーキー有効*/
	case GU_COLOR_LOGIC_OP: 		SEND_CMD_I(PXI(40, 1)); break;
	#if TEST0
	case GU_FACE_NORMAL_REVERSE:	SEND_CMD_I(PXI(81, 1)); break;
	case GU_PATCH_FACE: 			SEND_CMD_I(PXI(56, 1)); break;
	case GU_FRAGMENT_2X:/*(21)*/
		{
			GuContext *context		= &gu_contexts[gu_curr_context];
			context->fragment_2x	= 0x10000;
			pspGuSendCommandi(201, (0x10000|context->texture_function) );
		}
		break;
	#endif
	}
	if (state < 22)
	{	gu_states |= (1 << state);}
}
#endif

void sceGuDrawArray(int prim, int vtype, int count, const void *indices, const void *vertices)
{
	if (vtype)
	{	pspGuSendCommandi(18, vtype);}
	if (indices)
	{
		pspGuSendCommandi(16, (((unsigned int)indices) >> 8) & 0x0f0000);
		pspGuSendCommandi( 2,  ((unsigned int)indices)		 & 0x00ffffff);
	}
	if (vertices)
	{
		pspGuSendCommandi(16, (((unsigned int)vertices) >> 8) & 0x0f0000);
		pspGuSendCommandi( 1,  ((unsigned int)vertices) 	  & 0x00ffffff);
	}
	pspGuSendCommandi(4, ((prim << 16)|count) );
	pspGuSendStall();
}

void sceGuClear(int flags)
{
	GuContext *context	= &gu_contexts[gu_curr_context];
	unsigned int filter;
	struct Vertex
	{
		u32 color;
		u16 x, y, z;
		u16 pad;
	};
	switch (gu_draw_buffer.pixel_size)
	{
	case 0: 	filter = (context->clear_color & 0x00ffffff); break;
	case 1: 	filter = (context->clear_color & 0x00ffffff) | (context->clear_stencil << 31); break;
	case 2: 	filter = (context->clear_color & 0x00ffffff) | (context->clear_stencil << 28); break;
	case 3: 	filter = (context->clear_color & 0x00ffffff) | (context->clear_stencil << 24); break;
	default:	filter = 0; break;
	}

	struct Vertex *vertices;
	int count;
	#if TEST0
	/*(従来互換)*/
	if (!(flags & GU_FAST_CLEAR_BIT))
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));
		count = 2;

		vertices[0].color	= 0;
		vertices[0].x		= 0;
		vertices[0].y		= 0;
		vertices[0].z		= context->clear_depth;

		vertices[1].color	= filter;
		vertices[1].x		= gu_draw_buffer.width;
		vertices[1].y		= gu_draw_buffer.height;
		vertices[1].z		= context->clear_depth;
	}
	else
	#endif
	/*(GU_FAST_CLEAR_BITがある場合) (64SLICEでクリア) */
	{
		vertices	= (struct Vertex *)sceGuGetMemory(16 * sizeof(struct Vertex));
		count		= 16;
		{
			unsigned int i;
			unsigned int j;
			i = 0;
			j = 0;
			for (; i<16; )
			{
				vertices[i].color	= filter;
				vertices[i].x		= (j);
				vertices[i].y		= (0);
				vertices[i].z		= context->clear_depth;
				vertices[i+1].color = filter;
				vertices[i+1].x 	= (j + 64);
				vertices[i+1].y 	= gu_draw_buffer.height;
				vertices[i+1].z 	= context->clear_depth;
				i += 2;
				j += 64;
			}
		}
	}
	pspGuSendCommandi(211, ((flags & (GU_COLOR_BUFFER_BIT|GU_STENCIL_BUFFER_BIT|GU_DEPTH_BUFFER_BIT)) << 8) | 0x01);
	sceGuDrawArray(GU_SPRITES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, count, 0, vertices);
	SEND_CMD_I(PXI(211, 0));
}

//

void *sceGuSwapBuffers(void)
{
	void *temp	= gu_draw_buffer.disp_buffer;
	gu_draw_buffer.disp_buffer	= gu_draw_buffer.frame_buffer;
	gu_draw_buffer.frame_buffer = temp;

	if (gu_settings.swapBuffersCallback)
	{	gu_settings.swapBuffersCallback(&gu_draw_buffer.disp_buffer, gu_draw_buffer.frame_buffer);}

	if (gu_display_on)
	{	sceDisplaySetFrameBuf(
			(void *)((unsigned int)ge_edram_address + (unsigned int)gu_draw_buffer.disp_buffer),
			gu_draw_buffer.frame_width,
			gu_draw_buffer.pixel_size,
			gu_settings.swapBuffersBehaviour);
	}

	// TODO: remove this? it serves no real purpose
//	gu_current_frame ^= 1;

//	return (void *)gu_settings.swapBuffersBehaviour;
	return gu_draw_buffer.frame_buffer;
}

void guSwapBuffersBehaviour(int behaviour)
{
	gu_settings.swapBuffersBehaviour = behaviour;
}

void guSwapBuffersCallback(GuSwapBuffersCallback callback)
{
	gu_settings.swapBuffersCallback = callback;
}

int sceGuFinish(void)
{
	switch (gu_curr_context)
	{
	case GU_DIRECT:
	case GU_SEND:
		{
			SEND_CMD_I(PXI(15, 0));
			pspGuSendCommandi(12, 0);
			pspGuSendStall();
		}
		break;
	case GU_CALL:
		{
			#if TEST0
			if (0==gu_call_mode)
			#endif
			{
				SEND_CMD_I(PXI(11, 0));
			}
			#if TEST0
			else
		//	if (1==gu_call_mode)
			{
				pspGuSendCommandi(14, 0x120000);
				SEND_CMD_I(PXI(12, 0));
				pspGuSendCommandi(0, 0);
				pspGuSendStall();
			}
			#endif
		}
		break;
	}

	unsigned int size = ((unsigned int)gu_list->current) - ((unsigned int)gu_list->start);

	// go to parent list
	gu_curr_context = gu_list->parent_context;
	gu_list = &gu_contexts[gu_curr_context].list;
	return size;
}

int sceGuFinishId(unsigned int id)
{
	switch (gu_curr_context)
	{
	case GU_DIRECT:
	case GU_SEND:
		{
			pspGuSendCommandi(15, id & 0xffff);
			pspGuSendCommandi(12, 0);
			pspGuSendStall();
		}
		break;
	case GU_CALL:
		{
			#if TEST0
			if (0==gu_call_mode)
			#endif
			{
				SEND_CMD_I(PXI(11, 0));
			}
			#if TEST0
			else
		//	if (1==gu_call_mode)
			{
				pspGuSendCommandi(14, 0x120000);
				SEND_CMD_I(PXI(12, 0));
				pspGuSendCommandi(0, 0);
				pspGuSendStall();
			}
			#endif
		}
		break;
	}

	unsigned int size = ((unsigned int)gu_list->current) - ((unsigned int)gu_list->start);

	// go to parent list
	gu_curr_context 	= gu_list->parent_context;
	gu_list 			= &gu_contexts[gu_curr_context].list;
	return size;
}

void sceGuStart(int cid, void *list)
{
	GuContext *context			= &gu_contexts[cid];
	unsigned int *local_list	= (unsigned int *)(((unsigned int)list) | 0x40000000);
	// setup display list
	context->list.start 			= local_list;
	context->list.current			= local_list;
	context->list.parent_context	= gu_curr_context;
	gu_list = &context->list;
	// store current context
	gu_curr_context = cid;
	if (!cid)
	{
		ge_list_executed[0] 		= sceGeListEnQueue(local_list, local_list, gu_settings.ge_callback_id, 0);
		gu_settings.signal_offset	= 0;
	}
	if (0==gu_init_flag)
	{
		#if TEST0
		static int dither_matrix[16] =
		{
			-4,  0, -3,  1,
			 2, -2,  3, -1,
			-3,  1, -4,  0,
			 3, -1,  2, -2
		};
		sceGuSetDither((ScePspIMatrix4 *)dither_matrix);
		sceGuPatchDivide(16, 16);
		sceGuColorMaterial(GU_AMBIENT|GU_DIFFUSE|GU_SPECULAR);
		sceGuSpecular(1.0f);
		#endif
		sceGuTexScale(1.0f, 1.0f);
		gu_init_flag = (1);
	}
	if (!gu_curr_context)
	{
		if (gu_draw_buffer.frame_width)
		{
			pspGuSendCommandi(156,	 ((unsigned int)gu_draw_buffer.frame_buffer) & 0x00ffffff);
			pspGuSendCommandi(157, ((((unsigned int)gu_draw_buffer.frame_buffer) & 0xff000000) >> 8) | gu_draw_buffer.frame_width);
		}
	}
}

//int sceGuDisplay(/*int*/u8 state)
void pgc_display(/*int*/u8 state)
{
	gu_display_on = state;
	if (state)
	{	sceDisplaySetFrameBuf(
			(void *)((unsigned int)ge_edram_address+(unsigned int)gu_draw_buffer.disp_buffer),
			gu_draw_buffer.frame_width,
			gu_draw_buffer.pixel_size,
			PSP_DISPLAY_SETBUF_NEXTFRAME);
	}
	else
	{	sceDisplaySetFrameBuf(0, 0, 0, PSP_DISPLAY_SETBUF_NEXTFRAME);
	}
//	return (state);
}

void drawRegion(int x, int y, int width, int height)
{
	pspGuSendCommandi(21, (y << 10) | x);
	pspGuSendCommandi(22, (((y + height)-1) << 10) | ((x + width)-1));
}

void sceGuDispBuffer(int width, int height, void *dispbp, int dispbw)
{
	gu_draw_buffer.width		= width;
	gu_draw_buffer.height		= height;
	gu_draw_buffer.disp_buffer	= dispbp;
	if (!gu_draw_buffer.frame_width || (gu_draw_buffer.frame_width != dispbw))
	{	gu_draw_buffer.frame_width = dispbw;
	}
	drawRegion(0, 0, gu_draw_buffer.width, gu_draw_buffer.height);
	sceDisplaySetMode(0, gu_draw_buffer.width, gu_draw_buffer.height);
	if (gu_display_on)
	{	sceDisplaySetFrameBuf(
			(void *)(((unsigned int)ge_edram_address) + ((unsigned int)gu_draw_buffer.disp_buffer)),
			dispbw,
			gu_draw_buffer.pixel_size,
			PSP_DISPLAY_SETBUF_NEXTFRAME);
	}
}

#if (1==USE_EXTENSION_04)/*DXP*/
//sceGuTexWrap
int sceGuCheckList(void)
{
	return ((int)gu_list->current)-((int)gu_list->start);
}
void sceGuTexWrap(int u, int v)
{
	pspGuSendCommandi(199, (v << 8)|(u));
}
void sceGuTexFlush(void)
{
	s_sceGuTexFlush();//	sendCommandf(203, 0.0f);
}
int sceGuGetStatus(int state)
{
	if (state < 22)
	{	return (gu_states >> state) & 1;}
	return (0);
}
void sceGuDepthBuffer(void* zbp, int zbw)
{
	gu_draw_buffer.depth_buffer = zbp;
	if (!gu_draw_buffer.depth_width || (gu_draw_buffer.depth_width != zbw))
	{	gu_draw_buffer.depth_width = zbw;}
	pspGuSendCommandi(158, ((unsigned int)zbp) & 0xffffff);
	pspGuSendCommandi(159, ((((unsigned int)zbp) & 0xff000000) >> 8)|zbw);
}
#endif

#if (1==USE_EXTENSION_05)/*DXP*/
static void sceGuEnable_0_21(int state)
{
	if (GU_SCISSOR_TEST==state)/*(2)*/
	{
		GuContext *context = &gu_contexts[gu_curr_context];
		context->scissor_enable = 1;
		pspGuSendCommandi(212, (context->scissor_start[1]<<10)|context->scissor_start[0]);
		pspGuSendCommandi(213, (context->scissor_end[1]  <<10)|context->scissor_end[0]);
	}
	//
	#if TEST0
	else
	if (GU_FRAGMENT_2X==state)/*(21)*/
	{
		GuContext *context		= &gu_contexts[gu_curr_context];
		context->fragment_2x	= 0x10000;
		pspGuSendCommandi(201, (0x10000|context->texture_function) );
	}
	#endif

//	if (state < 22)
	{	gu_states |= (1 << state);}
}
static void sceGuDisable_0_21(int state)
{
	if (GU_SCISSOR_TEST==state)/*(2)*/
	{
		GuContext *context = &gu_contexts[gu_curr_context];
		context->scissor_enable = 0;
		SEND_CMD_I(PXI(212, 0));
		pspGuSendCommandi(213, ((gu_draw_buffer.height-1) << 10)|(gu_draw_buffer.width-1));
	}
	#if TEST0
	else
	if (GU_FRAGMENT_2X==state)/*(21)*/
	{
		GuContext *context		= &gu_contexts[gu_curr_context];
		context->fragment_2x	= 0;
		pspGuSendCommandi(201, context->texture_function);
	}
	#endif
//	if (state < 22)
	{	gu_states &= ~(1 << state);}
}
void sceGuSetAllStatus(int status)
{
	unsigned int i;
	for (i=0; i<22; ++i)
	{
		if ((status >> i)&1)
				{	sceGuEnable_0_21(i);}
		else	{	sceGuDisable_0_21(i);}
	}
}
int sceGuGetAllStatus(void)
{
	return gu_states;
}
#endif

#if (1==USE_EXTENSION_02)/*Rev2*/
void sceGuDepthRange(int near, int far)
{
	GuContext* context	= &gu_contexts[gu_curr_context];
	unsigned int max	= (unsigned int)near + (unsigned int)far;
	int val 	= (int)((max >> 31) + max);
	float z 	= (float)(val >> 1);

	context->near_plane 	= near;
	context->far_plane	= far;

	sendCommandf(68, z - ((float)near));
	sendCommandf(71, z + ((float)context->depth_offset));

	if (near > far)
	{
		int temp	= near;
		near		= far;
		far 		= temp;
	}
	pspGuSendCommandi(214,near);
	pspGuSendCommandi(215,far);
}

void sceGuClearDepth(unsigned int depth)
{
	GuContext* context = &gu_contexts[gu_curr_context];
	context->clear_depth = depth;
}

void sceGuDepthFunc(int function)
{
	pspGuSendCommandi(222, function);
}

void sceGuDepthMask(int mask)
{
	pspGuSendCommandi(231, mask);
}

void sceGuAlphaFunc(int a0, int a1, int a2)
{
	int arg = a0 | ((a1 & 0xff) << 8) | ((a2 & 0xff) << 16);
	pspGuSendCommandi(219, arg);
}

void sceGuClutMode(unsigned int cpsm, unsigned int shift, unsigned int mask, unsigned int a3)
{
	unsigned int argument = (cpsm) | (shift << 2) | (mask << 8) | (a3 << 16);
	pspGuSendCommandi(197, argument);
}

void sceGuClutLoad(int num_blocks, const void* cbp)
{
	pspGuSendCommandi(176, ((unsigned int)cbp) & 0xffffff);
	pspGuSendCommandi(177, (((unsigned int)cbp) >> 8) & 0xf0000);
	pspGuSendCommandi(196, num_blocks);
}

void sceGuSetDither(const ScePspIMatrix4* matrix)
{
	pspGuSendCommandi(226, (matrix->x.x & 0x0f)|((matrix->x.y & 0x0f) << 4)|((matrix->x.z & 0x0f) << 8)|((matrix->x.w & 0x0f) << 12));
	pspGuSendCommandi(227, (matrix->y.x & 0x0f)|((matrix->y.y & 0x0f) << 4)|((matrix->y.z & 0x0f) << 8)|((matrix->y.w & 0x0f) << 12));
	pspGuSendCommandi(228, (matrix->z.x & 0x0f)|((matrix->z.y & 0x0f) << 4)|((matrix->z.z & 0x0f) << 8)|((matrix->z.w & 0x0f) << 12));
	pspGuSendCommandi(229, (matrix->w.x & 0x0f)|((matrix->w.y & 0x0f) << 4)|((matrix->w.z & 0x0f) << 8)|((matrix->w.w & 0x0f) << 12));
}

#endif

/*
	sceGuDrawBufferList(); はあとで使う予定なので対応可能にしとく。
*/

#if (1==USE_DRAW_BUFFER_LIST)
void sceGuDrawBufferList(int psm, void* fbp, int frame_width)
{
	pspGuSendCommandi(210, psm);
	pspGuSendCommandi(156,	 ((unsigned int)fbp) & 0x00ffffff);
	pspGuSendCommandi(157, ((((unsigned int)fbp) & 0xff000000) >> 8) | frame_width);
}
#endif
void sceGuDrawBuffer(int psm, void *fbp, int frame_width)
{
	gu_draw_buffer.pixel_size		= psm;
	gu_draw_buffer.frame_width		= frame_width;
	gu_draw_buffer.frame_buffer 	= fbp;
	if (!gu_draw_buffer.depth_buffer && gu_draw_buffer.height)
	{	gu_draw_buffer.depth_buffer = (void *)(((unsigned int)fbp) + (unsigned int)((gu_draw_buffer.height * frame_width) << 2));
	}
	if (!gu_draw_buffer.depth_width)
	{	gu_draw_buffer.depth_width = frame_width;
	}
	#if (1==USE_DRAW_BUFFER_LIST)
	sceGuDrawBufferList(  psm,	 fbp,	frame_width);
	#else
	pspGuSendCommandi(210, psm);
	pspGuSendCommandi(156,	 ((unsigned int)gu_draw_buffer.frame_buffer) & 0x00ffffff);
	pspGuSendCommandi(157, ((((unsigned int)gu_draw_buffer.frame_buffer) & 0xff000000) >> 8) | gu_draw_buffer.frame_width);
	#endif
	#if 1
	pspGuSendCommandi(158,	 ((unsigned int)gu_draw_buffer.depth_buffer) & 0x00ffffff);
	pspGuSendCommandi(159, ((((unsigned int)gu_draw_buffer.depth_buffer) & 0xff000000) >> 8) | gu_draw_buffer.depth_width);
	#endif
}

static void resetValues(void)
{
	unsigned int i;
	gu_init_flag	= (0);
	//
	gu_states				= 0;
//	gu_current_frame		= 0;
	//
	gu_display_on			= 0;
	#if TEST0
	gu_call_mode			= (0);
	gu_object_stack_depth	= (0);
	#endif
	//
	gu_draw_buffer.pixel_size	= 1;
	gu_draw_buffer.frame_width	= 0;
	gu_draw_buffer.frame_buffer = 0;
	gu_draw_buffer.disp_buffer	= 0;
	gu_draw_buffer.depth_buffer = 0;
	gu_draw_buffer.depth_width	= 0;
	gu_draw_buffer.width		= (480);
	gu_draw_buffer.height		= (272);
	//
	for (i=0; i<3; i++)
	{
		GuContext *context = &gu_contexts[i];
		//
		context->scissor_enable 		= 0;
		context->scissor_start[0]		= 0;
		context->scissor_start[1]		= 0;
		context->scissor_end[0] 		= 0;
		context->scissor_end[1] 		= 0;
		//
		context->near_plane 			= 0;
		context->far_plane				= 1;
		//
		context->depth_offset			= 0;
		context->fragment_2x			= 0;
		context->texture_function		= 0;
		context->texture_proj_map_mode	= 0;
		context->texture_map_mode		= 0;
		context->sprite_mode[0] 		= 0;
		context->sprite_mode[1] 		= 0;
		context->sprite_mode[2] 		= 0;
		context->sprite_mode[3] 		= 0;
		context->clear_color			= 0;
		context->clear_stencil			= 0;
		context->clear_depth			= 0xffff;
		context->texture_mode			= 0;
	}
	gu_settings.sig 	= 0;
	gu_settings.fin 	= 0;
}

static const u32 const_ge_init_list[] __attribute__((aligned(16))) =
{
	//				//0x00	NOP 		No Operation
	PXI(0x01, 0),	//0x01	VADDR		Vertex Address (BASE)
	PXI(0x02, 0),	//0x02	IADDR		Index Address (BASE)
	//				//0x03(無し???)
	//				//0x04	PRIM		Primitive Kick
	//				//0x05	BEZIER		Bezier Patch Kick
	//				//0x06	SPLINE		Spline Surface Kick
	//				//0x07	BBOX		Bounding Box					0-15: Number of vertices to test for conditional rendering (0-65535)
	//				//0x08	JUMP		Jump To New Address (BASE)		0-23: 24 least significant bits of pointer
	//				//0x09	BJUMP		Conditional Jump (BASE) 		0-23: 24 least significant bits of pointer
	//				//0x0a	CALL		Call Address (BASE) 			0-23: 24 least significant bits of pointer
	//				//0x0b	RET 		Return From Call
	//				//0x0c	END 		Stop execution
	//				//0x0d(無し???)
	//				//0x0e	SIGNAL		Raise Signal Interrupt			0-15: Argument to pass to signal handler	16-23: Signal index to trigger
	//				//0x0f	FINISH		Complete rendering				0-15: Finish signal argument
	PXI(0x10, 0),	//0x10	BASE		Base Address Register
	//11			//0x11(無し???)
	PXI(0x12, 0),	// 0x12  VTYPE		 Vertex Type
	PXI(0x13, 0),	// 0x13  ???		 Offset Address (BASE)
	//14			// 0x14  ???		 Origin Address (BASE)
	PXI(0x15, 0),	// 0x15  REGION1	 Draw Region Start
	PXI(0x16, 0),	// 0x16  REGION2     Draw Region End
	PXI(0x17, 0),					// PGC_LIGHTING 		= 0x17,//23,/*(?)*/ 		(ハ－ドウェアー光源のon/off)
	PXI(0x18, 0),					// PGC_LIGHT0			= 0x18,//24,/*(?)*/ 		(ハ－ドウェアー光源1のon/off)
	PXI(0x19, 0),					// PGC_LIGHT1			= 0x19,//25,/*(?)*/ 		(ハ－ドウェアー光源2のon/off)
	PXI(0x1a, 0),					// PGC_LIGHT2			= 0x1a,//26,/*(?)*/ 		(ハ－ドウェアー光源3のon/off)
	PXI(0x1b, 0),					// PGC_LIGHT3			= 0x1b,//27,/*(?)*/ 		(ハ－ドウェアー光源4のon/off)
	PXI(0x1c, PGC_ON_FOR_MOHOUFU),	// PGC_CLIP_PLANES		= 0x1c,//28,//<模倣風>		クリップ。
	PXI(0x1d, PGC_ON_FOR_MOHOUFU),	// PGC_CULL_FACE		= 0x1d,//29,//<模倣風>		カリング(裏ポリゴンの描画)モードのon/off。
	PXI(0x1e, PGC_ON),				// PGC_TEXTURE_2D		= 0x1e,//30,//<ON>			テクスチャマッピングon/off。
	PXI(0x1f, 0),					// PGC_FOG				= 0x1f,//31,/*(?)*/			フォグ(霧)。
	PXI(0x20, 0),					// PGC_DITHER			= 0x20,//32,/*(?)*/			ディザ。
	PXI(0x21, PGC_ON_FOR_MOHOUFU),	// PGC_BLEND			= 0x21,//33,//<模倣風>		アルファブレンディング。
	PXI(0x22, 0),					// PGC_ALPHA_TEST		= 0x22,//34,				アルファテスト。
	PXI(0x23, 0),					// PGC_DEPTH_TEST		= 0x23,//35,				Ｚバッファ。
	PXI(0x24, 0),					// PGC_STENCIL_TEST 	= 0x24,//36,/*(?)*/			ステンシル。
	PXI(0x25, 0),					// PGC_LINE_SMOOTH		= 0x25,//37,/*(?)*/			アンチエイリアシング機能。
	PXI(0x26, 0),					// PGC_PATCH_CULL_FACE	= 0x26,//38,/*(?)*/			。
	PXI(0x27, PGC_ON_FOR_MOHOUFU),	// PGC_COLOR_TEST		= 0x27,//39,//<模倣風>		カラーテスト。
	PXI(0x28, 0),					// PGC_COLOR_LOGIC_OP	= 0x28,//40,/*(?)*/ 		ロジカルオペレーション。(色論理演算機能)
	//290x29(無し???)
	PXI(0x2a, 0),					//		0x2a  BOFS		Bone Matrix Offset
	PXI(0x2b, 0),					//		0x2b  BONE		Bone Matrix Upload
	PXI(0x2c, 0),					//		0x2C  MW0		Morph Weight 0				0-23: Morph Value (GE Float)
	PXI(0x2d, 0),					//		0x2D  MW1		Morph Weight 1				0-23: Morph Value (GE Float)
	PXI(0x2e, 0),					//		0x2E  MW2		Morph Weight 2				0-23: Morph Value (GE Float)
	PXI(0x2f, 0), //				//		0x2F  MW3		Morph Weight 3				0-23: Morph Value (GE Float)
//
	PXI(0x30, 0),					//		0x30  MW4		Morph Weight 4				0-23: Morph Value (GE Float)
	PXI(0x31, 0),					//		0x31  MW5		Morph Weight 5				0-23: Morph Value (GE Float)
	PXI(0x32, 0),					//		0x32  MW6		Morph Weight 6				0-23: Morph Value (GE Float)
	PXI(0x33, 0),					//		0x33  MW7		Morph Weight 7				0-23: Morph Value (GE Float)
	//34//0x34(無し???)
	//35//0x35(無し???)
	PXI(0x36, 0),					//0x36  PSUB        Patch Subdivision
	PXI(0x37, 0),					//0x37  PPRIM       Patch Primitive
	PXI(0x38, 0),					//0x38  PFACE       Patch Front Face	0:CW/1:CCW			PGC_PATCH_FACE				= 0x38,//56,/*(?)*/
	//39//0x39(無し???)
	PXI(0x3a, 0),
	PXI(0x3b, 0),
	PXI(0x3c, 0),
	PXI(0x3d, 0),
	PXI(0x3e, 0),
	PXI(0x3f, 0), //
//
	PXI(0x40, 0),
	PXI(0x41, 0),
	PXI(0x42, 0),
	PXI(0x43, 0),
	PXI(0x44, 0),
	PXI(0x45, 0),
	PXI(0x46, 0),
	PXI(0x47, 0), //
	PXI(0x48, 0),
	PXI(0x49, 0),
	PXI(0x4a, 0),
	PXI(0x4b, 0),
	PXI(0x4c, 0),
	PXI(0x4d, 0), //
	//4e
	//4f
//
	PXI(0x50, 0),
	PXI(0x51, 0), //	PGC_FACE_NORMAL_REVERSE 	= 0x51,//81,/*(?)*/
	//52
	PXI(0x53, 0),
	PXI(0x54, 0),
	PXI(0x55, 0),
	PXI(0x56, 0),
	PXI(0x57, 0), //
	PXI(0x58, 0),
	//59
	//5a
	PXI(0x5b, 0),
	PXI(0x5c, 0),
	PXI(0x5d, 0),
	PXI(0x5e, 0),
	PXI(0x5f, 0), //
//
	PXI(0x60, 0),
	PXI(0x61, 0),
	PXI(0x62, 0),
	PXI(0x63, 0),
	PXI(0x64, 0),
	PXI(0x65, 0),
	PXI(0x66, 0),
	PXI(0x67, 0), //
	PXI(0x68, 0),
	PXI(0x69, 0),
	PXI(0x6a, 0),
	PXI(0x6b, 0),
	PXI(0x6c, 0),
	PXI(0x6d, 0),
	PXI(0x6e, 0),
	PXI(0x6f, 0), //
//
	PXI(0x70, 0),
	PXI(0x71, 0),
	PXI(0x72, 0),
	PXI(0x73, 0),
	PXI(0x74, 0),
	PXI(0x75, 0),
	PXI(0x76, 0),
	PXI(0x77, 0), //
	PXI(0x78, 0),
	PXI(0x79, 0),
	PXI(0x7a, 0),
	PXI(0x7b, 0),
	PXI(0x7c, 0),
	PXI(0x7d, 0),
	PXI(0x7e, 0),
	PXI(0x7f, 0), //
//
	PXI(0x80, 0),
	PXI(0x81, 0),
	PXI(0x82, 0),
	PXI(0x83, 0),
	PXI(0x84, 0),
	PXI(0x85, 0),
	PXI(0x86, 0),
	PXI(0x87, 0), //
	PXI(0x88, 0),
	PXI(0x89, 0),
	PXI(0x8a, 0),
	PXI(0x8b, 0),
	PXI(0x8c, 0),
	PXI(0x8d, 0),
	PXI(0x8e, 0),
	PXI(0x8f, 0), //
//
	PXI(0x90, 0),
	PXI(0x91, 0),
	PXI(0x92, 0),
	PXI(0x93, 0),
	PXI(0x94, 0),
	PXI(0x95, 0),
	PXI(0x96, 0),
	PXI(0x97, 0), //
	PXI(0x98, 0),
	PXI(0x99, 0),
	PXI(0x9a, 0),
	PXI(0x9b, 0),
	PXI(0x9c, 0),
	PXI(0x9d, 0),
	PXI(0x9e, 0),
	PXI(0x9f, 0), //
//
	PXI(0xa0, 0),
	PXI(0xa1, 0),
	PXI(0xa2, 0),
	PXI(0xa3, 0),
	PXI(0xa4, 0),
	PXI(0xa5, 0),
	PXI(0xa6, 0),
	PXI(0xa7, 0), //
	PXI(0xa8, 0x040004),
	PXI(0xa9, 0),
	PXI(0xaa, 0),
	PXI(0xab, 0),
	PXI(0xac, 0),
	PXI(0xad, 0),
	PXI(0xae, 0),
	PXI(0xaf, 0), //
//
	PXI(0xb0, 0),
	PXI(0xb1, 0),
	PXI(0xb2, 0),
	PXI(0xb3, 0),
	PXI(0xb4, 0),
	PXI(0xb5, 0), //
	//b6
	//b7
	PXI(0xb8, 0x000101),
	PXI(0xb9, 0),
	PXI(0xba, 0),
	PXI(0xbb, 0),
	PXI(0xbc, 0),
	PXI(0xbd, 0),
	PXI(0xbe, 0),
	PXI(0xbf, 0), //
//
	PXI(0xc0, 0),
	PXI(0xc1, 0),
	PXI(0xc2, 0),
	PXI(0xc3, 0),
	PXI(0xc4, 0),
	PXI(0xc5, 0),
	PXI(0xc6, 0),
	PXI(0xc7, 0), //
	PXI(0xc8, 0),
	PXI(0xc9, 0),
	PXI(0xca, 0),
	PXI(0xcb, 0),
	PXI(0xcc, 0),
	PXI(0xcd, 0),
	PXI(0xce, 0),
	PXI(0xcf, 0), //
//
	PXI(0xd0, 0),
	//d1
	PXI(0xd2, 0),
	PXI(0xd3, 0),
	PXI(0xd4, 0),
	PXI(0xd5, 0),
	PXI(0xd6, 0),
	PXI(0xd7, 0), //
	PXI(0xd8, 0),
	PXI(0xd9, 0),
	PXI(0xda, 0),
	PXI(0xdb, 0),
	PXI(0xdc, 0),
	PXI(0xdd, 0),
	PXI(0xde, 0),
	PXI(0xdf, 0), //
//
	PXI(0xe0, 0),
	PXI(0xe1, 0),
	PXI(0xe2, 0),
	PXI(0xe3, 0),
	PXI(0xe4, 0),
	PXI(0xe5, 0),
	PXI(0xe6, 0),
	PXI(0xe7, 0), //
	PXI(0xe8, 0),
	PXI(0xe9, 0),
	//ea
	PXI(0xeb, 0),
	PXI(0xec, 0),
	//ed
	PXI(0xee, 0),
	//ef
//
	PXI(0xf0, 0),
	PXI(0xf1, 0),
	PXI(0xf2, 0),
	PXI(0xf3, 0),
	PXI(0xf4, 0),
	PXI(0xf5, 0),
	PXI(0xf6, 0),
	PXI(0xf7, 0), //
	PXI(0xf8, 0),
	PXI(0xf9, 0), //
	//fa
	//fb
	//fc
	//fd
	//fe
	//ff
//
	PXI(0x0f, 0),
	PXI(0x0c, 0),
	0, 0
};

static void callbackSig(int id, void *arg)
{
	GuSettings *settings = (GuSettings *)arg;
	settings->signal_history[(settings->signal_offset++) & 15] = id & 0xffff;
	if (settings->sig)
	{	settings->sig(id & 0xffff);}
	sceKernelSetEventFlag(settings->kernel_event_flag, 1);
}

static void callbackFin(int id, void *arg)
{
	GuSettings *settings = (GuSettings *)arg;
	if (settings->fin)
	{	settings->fin(id & 0xffff);}
}

//void sceGuInit(void)
void pgc_first_initilize(void)
{
	PspGeCallbackData callback;
	callback.signal_func	= callbackSig;
	callback.signal_arg 	= &gu_settings;
	callback.finish_func	= callbackFin;
	callback.finish_arg 	= &gu_settings;
	gu_settings.ge_callback_id = sceGeSetCallback(&callback);
	//
	gu_settings.swapBuffersCallback 	= 0;
	gu_settings.swapBuffersBehaviour	= PSP_DISPLAY_SETBUF_IMMEDIATE;
	ge_edram_address = sceGeEdramGetAddr();
	// initialize graphics hardware
	ge_list_executed[0] = sceGeListEnQueue(
		(void *)((u32)const_ge_init_list & 0x1fffffff),
		0,
		gu_settings.ge_callback_id, 0);
	resetValues();
	gu_settings.kernel_event_flag = sceKernelCreateEventFlag("SceGuSignal", 512, 3, 0);
	// wait for init to complete
	sceGeListSync(ge_list_executed[0], 0);
}
#if (1==USE_EXTENSION_01)
void sceGuTerm(void)
{
	sceKernelDeleteEventFlag(gu_settings.kernel_event_flag);
	sceGeUnsetCallback(gu_settings.ge_callback_id);
}
#endif
