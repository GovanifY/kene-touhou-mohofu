
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	Portable psp Gu Customized library.
---------------------------------------------------------*/

#ifndef _PSP_GRAPHICS_CUSTOMIZEPORT_H_
#define _PSP_GRAPHICS_CUSTOMIZEPORT_H_

//#define L24B(aaa) 	((aaa) & 0x00ffffff)
#define PXI(aaa,bbb)	(((aaa)<<24)|((bbb) & 0x00ffffff))
#define PU8(aaa,bbb)	(((aaa)|((bbb<<8))))

enum
{
#if (0)
0x00  NOP		  No Operation
0x01  VADDR 	  Vertex Address (BASE)
		 0-23: 24 least significant bits of pointer
0x02  IADDR 	  Index Address (BASE)
		 0-23: 24 least significant bits of pointer
0x04  PRIM		  Primitive Kick
		 0-15: Number of vertices to kick (0-65535)
		16-18: Primitive Type
			000: Points
			001: Lines
			010: Line Strips
			011: Triangles
			100: Triangle Strips
			101: Triangle Fans
			110: Sprites (2D Rectangles)
0x05  BEZIER	  Bezier Patch Kick
		 0-7: U Count
		 8-15: V Count
0x06  SPLINE	  Spline Surface Kick
		 0-7: U Count
		 8-15: V Count
		16-17: U Edges
			00: Close/Close
			01: Open/Close
			10: Close/Open
			11: Open/Open
		18-19: V Edges
			00: Close/Close
			01: Open/Close
			10: Close/Open
			11: Open/Open
0x07  BBOX		  Bounding Box
		0-15: Number of vertices to test for conditional rendering (0-65535)
0x08  JUMP		  Jump To New Address (BASE)
		0-23: 24 least significant bits of pointer
0x09  BJUMP 	  Conditional Jump (BASE)
		0-23: 24 least significant bits of pointer
0x0A  CALL		  Call Address (BASE)
		0-23: 24 least significant bits of pointer
0x0B  RET		  Return From Call
0x0C  END		  Stop execution
0x0E  SIGNAL	  Raise Signal Interrupt
		0-15: Argument to pass to signal handler
		16-23: Signal index to trigger
0x0F  FINISH	  Complete rendering
		0-15: Finish signal argument
0x10  BASE		  Base Address Register
		16-20: 4 most significant bits for address (28 bits total)
0x12  VTYPE 	  Vertex Type
		0-1: Texture Format (2 values ST/UV)
			00: Not present in vertex
			01: 8-bit fixed
			10: 16-bit fixed
			11: 32-bit floats
		2-4: Color Format (1 value)
			000: Not present in vertex
			100: 16-bit BGR-5650
			101: 16-bit ABGR-5551
			110: 16-bit ABGR-4444
			111: 32-bit ABGR-8888
		5-6: Normal Format (3 values XYZ)
			00: Not present in vertex
			01: 8-bit fixed
			10: 16-bit fixed
			11: 32-bit floats
		7-8: Position Format (3 values XYZ)
			00: Not present in vertex
			01: 8-bit fixed
			10: 16-bit fixed
			11: 32-bit floats
		9-10: Weight Format
			00: Not present in vertex
			01: 8-bit fixed
			10: 16-bit fixed
			11: 32-bit floats
		11-12: Index Format
			00: Not using indices
			01: 8-bit
			10: 16-bit
		14-16: Number of weights (Skinning)
			000-111: 1-8 weights
		18-20: Number of vertices (Morphing)
			000-111: 1-8 vertices
		23: Bypass Transform Pipeline
			0: Transformed Coordinates
			1: Raw Coordinates
0x13  ???		  Offset Address (BASE)
0x14  ???		  Origin Address (BASE)
0x15  REGION1	  Draw Region Start
		 0-9:  X Start
		10-19: Y Start
0x16  REGION2	  Draw Region End
		 0-9:  X End ((X + Width)-1)
		10-19: Y End ((Y+Height)-1)
#endif
	// 0x16(22) 以上
	PGC_LIGHTING		= 0x17,//23,/*(?)*/ 	LTE 	Lighting Enable 	(ハ－ドウェアー光源のon/off)
	PGC_LIGHT0			= 0x18,//24,/*(?)*/ 	LTE0	Light Enable 0		(ハ－ドウェアー光源1のon/off)
	PGC_LIGHT1			= 0x19,//25,/*(?)*/ 	LTE1	Light Enable 1		(ハ－ドウェアー光源2のon/off)
	PGC_LIGHT2			= 0x1a,//26,/*(?)*/ 	LTE2	Light Enable 2		(ハ－ドウェアー光源3のon/off)
	PGC_LIGHT3			= 0x1b,//27,/*(?)*/ 	LTE3	Light Enable 3		(ハ－ドウェアー光源4のon/off)
	PGC_CLIP_PLANES 	= 0x1c,//28,			CPE 	Clip Plane Enable			クリップ。
	PGC_CULL_FACE		= 0x1d,//29,			BCE 	Backface Culling Enable 	カリング(裏ポリゴンの描画)モードのon/off。
	PGC_TEXTURE_2D		= 0x1e,//30,			TME 	Texture Mapping Enable		テクスチャマッピングon/off。
	PGC_FOG 			= 0x1f,//31,/*(?)*/ 	FGE 	Fog Enable					フォグ(霧)。
	PGC_DITHER			= 0x20,//32,/*(?)*/ 	DTE 	Dither Enable				ディザ。
	PGC_BLEND			= 0x21,//33,			ABE 	Alpha Blend Enable			アルファブレンディング。
	PGC_ALPHA_TEST		= 0x22,//34,			ATE 	Alpha Test Enable			アルファテスト。
	PGC_DEPTH_TEST		= 0x23,//35,			ZTE 	Depth Test Enable			Ｚバッファ。
	PGC_STENCIL_TEST	= 0x24,//36,/*(?)*/ 	STE 	Stencil Test Enable 		ステンシル。
	PGC_LINE_SMOOTH 	= 0x25,//37,/*(?)*/ 	AAE 	Anti Aliasing Enable.		アンチエイリアシング機能。
	PGC_PATCH_CULL_FACE = 0x26,//38,/*(?)*/ 	PCE 	Patch Cull Enable			。
	PGC_COLOR_TEST		= 0x27,//39,			CTE 	Color Test Enable			カラーテスト。
	PGC_COLOR_LOGIC_OP	= 0x28,//40,/*(?)*/ 	LOE 	Logical Operation Enable.	ロジカルオペレーション。(色論理演算機能)
	//					  0x29(無し???)
	//					  0x2a	BOFS		Bone Matrix Offset
	//					  0x2b	BONE		Bone Matrix Upload
	//
#if 0
0x2C  MW0		  Morph Weight 0						0-23: Morph Value (GE Float)
0x2D  MW1		  Morph Weight 1						0-23: Morph Value (GE Float)
0x2E  MW2		  Morph Weight 2						0-23: Morph Value (GE Float)
0x2F  MW3		  Morph Weight 3						0-23: Morph Value (GE Float)
0x30  MW4		  Morph Weight 4						0-23: Morph Value (GE Float)
0x31  MW5		  Morph Weight 5						0-23: Morph Value (GE Float)
0x32  MW6		  Morph Weight 6						0-23: Morph Value (GE Float)
0x33  MW7		  Morph Weight 7						0-23: Morph Value (GE Float)
//0x34(無し???)
//0x35(無し???)
0x36  PSUB		  Patch Subdivision
					0-7: S Subdivision
					8-15: T Subdivision
0x37  PPRIM 	  Patch Primitive
					0-1:
						00: Triangles
						01: Lines
						10: Points
0x38  PFACE 	  Patch Front Face
						0:
							0: Clockwise
							1: Counter-Clockwise
//0x39(無し???)
#endif
	PGC_PATCH_FACE				= 0x38,//56,/*(?)*/ 	// (CW/CCW)
	PGC_FACE_NORMAL_REVERSE 	= 0x51,//81,/*(?)*/
	PGC_TEX_FILTER				= 0xc6,
	PGC_TEX_ENV_COLOR			= 0xca,
};
/*(別名)*/
//#if (0)
//	#define PGC_ANTI_ALIASING	PGC_LINE_SMOOTH
//#endif
extern void pgc_send(u32 argument);

#define PGC_SEND(aaa,bbb)	pgc_send(PXI(aaa,bbb))

enum
{
	PGC_OFF =	(0),
	PGC_ON	=	(1),
};

extern void pgc_scissor_enable(void);

extern void pgc_set_matrix_PROJECTION(	const ScePspFMatrix4* matrix);
extern void pgc_set_matrix_VIEW(		const ScePspFMatrix4* matrix);
extern void pgc_set_matrix_MODEL(		const ScePspFMatrix4* matrix);
extern void pgc_set_matrix_TEXTURE( 	const ScePspFMatrix4* matrix);

extern void pgc_first_initilize(void);
extern void pgc_display(u8 on_off);

//extern void pcg_gouraud_shading(u8 mode);

#endif /* _PSP_GRAPHICS_CUSTOMIZEPORT_H_ */
