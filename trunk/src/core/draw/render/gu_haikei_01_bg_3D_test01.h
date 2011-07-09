
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------
	3D背景(テスト中)
	-------------------------------------------------------
	Gumを使わないで(Gu+vfpu)で3Dが出来るらしいので、
	テストで作ってみる。
	速度は？？？
---------------------------------------------------------*/

#include "psp_vfpu.h"
#if 0
typedef struct quat
{
	float x, y, z, w;
} __attribute__((aligned(16))) ScePspQuatMatrix;

void vfpu_srand(unsigned int x);
float vfpu_randf(float min, float max);

float vfpu_ease_in_out(float t);

void vfpu_perspective_matrix(ScePspFMatrix4 *m, float fovy, float aspect, float near, float far);
void vfpu_identity_matrix(ScePspFMatrix4 *m);

void vfpu_quaternion_copy(ScePspQuatMatrix *dst, ScePspQuatMatrix *src);
void vfpu_quaternion_from_euler(ScePspQuatMatrix *qout, float rx, float ry, float rz);
void vfpu_quaternion_exp(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_sample_linear(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, float t);
void vfpu_quaternion_to_matrix(ScePspQuatMatrix *q, ScePspFMatrix4 *m);
#endif

//								--aabbggrr (ABGR8888)
#define MY_COLOR_ARGB888_WHITE	0xffffffff
#define MY_COLOR_ARGB888_BLUE	0xffff7f7f
#define MY_COLOR_ARGB888_GREEN	0xff7fff7f
#define MY_COLOR_ARGB888_RED	0xff7f7fff

typedef struct
{
	/*float*/unsigned char u;	/* 0(左端) ... 255(右端) */ 		/* テクスチャ(u, v)座標 */
	/*float*/unsigned char v;	/* 0(上端) ... 255(下端) */
	unsigned int color; 		/* 0x00000000 ... 0xffffffff */ 	/* 頂点カラー(ARGB8888) */
	/*float*/signed char x; 	/* -128 ... 0(中心) ... 127 */		/* 頂点(x, y, z)座標 */
	/*float*/signed char y; 	/* -128 ... 0(中心) ... 127 */
	/*float*/signed char z; 	/* -128 ... 0(中心) ... 127 */
} Vertex_cube;
static Vertex_cube	__attribute__((aligned(16))) cube_polygon_model[(12*3)] =
{
	/* テクスチャは画像サイズに対して(0...255, 0...255)の(u, v)座標で指定する。*/
	/* (u, v)座標はテクスチャの貼り付ける位置を指定する。 */
	/* 画像サイズが512, 64で(u,v)が(  0,  0)なら、画像位置は(  0,  0) */
	/* 画像サイズが512, 64で(u,v)が(127,127)なら、画像位置は(255, 31) */
	/* 画像サイズが512, 64で(u,v)が(255,255)なら、画像位置は(511, 63) */
//
	/* 画像サイズが256,256で(u,v)が(  0,  0)なら、画像位置は(  0,  0) */
	/* 画像サイズが256,256で(u,v)が(127,127)なら、画像位置は(127,127) */
	/* 画像サイズが256,256で(u,v)が(255,255)なら、画像位置は(255,255) */
	/* つまり画像サイズが256,256なら、dot by dot、画像位置==(u,v) になる。 */
//
	/* 頂点に色を付けるという事は、つまり(法線という意味ではなくて)グーロシェーディングで補完されるという事。 */
	/* このハードウェアー補完は描画モード(色の解像度16bitとか)に関係なく常に32bitで行われるのできれい。 */
//
	/* この辺から解かるように、Gu内部には色32bit(8888)の描画エンジンしか無い。 */
	/* 色の解像度16bitとか(5650, 5551, 4444)やclutの描画モードの場合は「外部(つまりvram)転送時」(つまり描画時)に色変換している。 */
	/* 「Guからみたvramのアドレスが変わる事で速度低下するペナルティー >>> 描画時の色変換速度」 なので、16bitやclutの方が結果的に描画が速い。 */
//
	/* テクスチャの位置 				頂点の位置座標 */
	/* u	v  色(ABGR8888) 			 x	  y    z */
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 0	/* 青 */
	{255, 255, MY_COLOR_ARGB888_BLUE,  127, 127, 127}, // #2	// 5	/* 青 青 */
	{255,	0, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 4[]

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 1
	{255, 255, MY_COLOR_ARGB888_BLUE,  127, 127, 127}, // #2	// 5	/* 青 青 */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 3[]	/* 青白 */
	{255, 255, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 6
	{255,	0, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 2

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 3[]
	{  0, 255, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 7
	{255, 255, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 6
//
	{  0,	0, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 0	/* 緑 */
	{255, 255, MY_COLOR_ARGB888_GREEN, 127, 127, 127}, // #2	// 7	/* 緑 緑 */
	{255,	0, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 3

	{  0,	0, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 4
	{255, 255, MY_COLOR_ARGB888_GREEN, 127, 127, 127}, // #2	// 7	/* 緑 緑 */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 0[]	/* 白 */
	{255, 255, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 7[]
	{255,	0, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 3

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 0[]
	{  0, 255, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 4
	{255, 255, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 7[]
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 0	/* 赤 */
	{255, 255, MY_COLOR_ARGB888_RED,   127, 127, 127}, // #2	// 2	/* 赤 赤 */
	{255,	0, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 1

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 3[]
	{255, 255, MY_COLOR_ARGB888_RED,   127, 127, 127}, // #2	// 2	/* 赤 赤 */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 4[]	/* 赤白 */
	{255, 255, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 6
	{255,	0, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 7

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 4[]
	{  0, 255, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 5
	{255, 255, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 6

};


#define TEST_AUTO_ROT 1
ScePspFMatrix4 m4_projection3d;
ScePspFMatrix4 m4_view;
ScePspFMatrix4 m4_model;

#if (1==TEST_AUTO_ROT)
ScePspQuatMatrix p9_qcam;
ScePspQuatMatrix p9_qa;
ScePspQuatMatrix p9_qb;
#endif
ScePspQuatMatrix p9_test;

static	int nnn;
	// run sample
//	/*int*/float val_x;
//	/*int*/float val_y;
//	/*int*/float val_z;
//	/*int*/float val_w;


#if (1==TEST_AUTO_ROT)
static void test_auto_rot_aaaa(void)
{
	vfpu_quaternion_copy(&p9_qa, &p9_qb);
//	vfpu_quaternion_from_euler(&p9_qb,
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f));
	vfpu_quaternion_from_euler512_int(&p9_qb,
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f));
		((vfpu_rand_8888(0, 255))&(512-1)),
		((vfpu_rand_8888(0, 255))&(512-1)),
		((vfpu_rand_8888(0, 255))&(512-1)));
//	vfpu_quaternion_exp(&p9_qb, &p9_qb);
	vfpu_quaternion_ln(&p9_qb, &p9_qb);
}
#endif

static void gu_init_vfpu(void)
{
	// vfpu has a random number generator,
	// and it needs to be seeded
	{
		u64 tick;
		sceRtcGetCurrentTick(&tick);
	//	vfpu_srand((u32)tick / 10000.0f);	これでは使い方がおかしいので間違い。
		vfpu_srand((u32)tick ); 			/* vfpu_srand gets int. */
	}
	vfpu_perspective_matrix(&m4_projection3d, 60.0f, 480.0f/272.0f, 0.5f, 1000.0f);
	/* 初期化 */
	vfpu_identity_matrix(&m4_view); 		/* 単位行列で初期化 */
	vfpu_identity_matrix(&m4_model);		/* 単位行列で初期化 */
	vfpu_quaternion_identity(&p9_test); 	/* 単位クォータニオンで初期化 */

	// build a random starfield, using sphere to cartesian coordinate conversions
	// note how we use vfpu_randf here to generate random radian angles,
	// and how we use vfpu_rand_8888 to generate random colors for the star_cube_polygon_model
//	for (n=0; n<STARS_MAX; n++)
//	{
//		vfpu_sphere_to_cartesian(
//			vfpu_randf(0.0f, 6.28319f),
//			vfpu_randf(0.0f, 3.14159f),
//			1000.0f,
//			(ScePspFVector4 *)&(star_cube_polygon_model[n])
//		);
//		star_cube_polygon_model[n].color = vfpu_rand_8888(160, 255);
//	}

	// start off with 2 random rotations for the interpolation
//	vfpu_quaternion_from_euler(&p9_qa, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));	vfpu_quaternion_ln(&p9_qa, &p9_qa);
//	vfpu_quaternion_from_euler(&p9_qb, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));	vfpu_quaternion_ln(&p9_qb, &p9_qb);

	nnn = 0;
	test_auto_rot_aaaa();

//	val_x = 0;
//	val_y = 0;
//	val_z = 0;
//	val_w = 0;

}


static void gu_draw_bg_3D_test01(void)
{

		// when we reach the limit of our interpolation:
		//		copy qb to qa
		//		generate a new random quaternion in qb
		//		take the log of quaternion qb
		//		reset the time counter

		nnn++;
		if (nnn >= byou60(5) ) /* 300==(5[sec] x 60[flame])*/
		{
			nnn = 0;
			test_auto_rot_aaaa();
		}

#if 0
		{
			SceCtrlData pad;
			sceCtrlPeekBufferPositive(&pad, 1);
			if (pad.Buttons & PSP_CTRL_UP)			{		val_x-=0.01;			}
			if (pad.Buttons & PSP_CTRL_RIGHT)		{		val_y+=0.01;			}
			if (pad.Buttons & PSP_CTRL_DOWN)		{		val_x+=0.01;			}
			if (pad.Buttons & PSP_CTRL_LEFT)		{		val_y-=0.01;			}
		//
			if (pad.Buttons & PSP_CTRL_LTRIGGER)	{		running = 0;			}
			if (pad.Buttons & PSP_CTRL_RTRIGGER)	{		running = 0;			}
		//
			if (pad.Buttons & PSP_CTRL_TRIANGLE)	{		val_z-=0.01;			}
			if (pad.Buttons & PSP_CTRL_CIRCLE)		{		val_w+=0.01;			}
			if (pad.Buttons & PSP_CTRL_CROSS)		{		val_z+=0.01;			}
			if (pad.Buttons & PSP_CTRL_SQUARE)		{		val_w-=0.01;			}
		}
#endif

		// to build the view matrix:
		//		get the interpolated quaternion in qcam
		//		take the exp of qcam
		//		build a rotation matrix from qcam, place into view matrix
		//
		// here we use the function 0.5 + (cos(t) * 0.5) for our 'time' variable
		// this gives us the smooth ease in/ease out motion you see as the demo
		// interpolates between qa and qb
		//
		// this is why we must do 300-n, as cos(0) = 1, cos(PI) = -1, we want
		// the reverse, -1 to 1
	#if (1==TEST_AUTO_ROT)
		{
		//	float t = (nnn)*((float)(1.0f/300.0f));
			u32 t65536 = (nnn)*((u32)(65536.0/300.0));
		//	vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
		u32 i_rate65536;
		//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
			i_rate65536 = (int)(vfpu_ease_in_out65536(t65536));
			vfpu_quaternion_sample_linear65536(&p9_qcam, &p9_qa, &p9_qb, i_rate65536);
		}
		vfpu_quaternion_exp(&p9_qcam, &p9_qcam);
		vfpu_quaternion_to_matrix(&p9_qcam, &m4_view);
	#endif

//
	#if 0
	p9_test.x = val_x;
	p9_test.y = val_y;
	p9_test.z = val_z;
	p9_test.w = val_w;
//
	vfpu_quaternion_to_matrix(&p9_test, &m4_projection3d);
	#endif
//
		// load our generated matrix data into the display list
		sceGuSetMatrix(GU_PROJECTION,	&m4_projection3d);		/* 透視変換用マトリクス */
		sceGuSetMatrix(GU_VIEW, 		&m4_view);				/* 視点変換ビュー用マトリクス */
		sceGuSetMatrix(GU_MODEL,		&m4_model); 			/* ワールド座標用マトリクス */

	//	sceGuDrawArray(GU_POINTS,		GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_LINES,		GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_SPRITES,		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGumDrawArray(GU_SPRITES, 	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
//		sceGuDrawArray(GU_SPRITES,		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
//
		// draw cube
	//	sceGuFrontFace(GU_CCW);
//		sceGumDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
		sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_8BIT | GU_COLOR_8888 | GU_VERTEX_8BIT | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
	//	sceGuFrontFace(GU_CW);
}

//#define GU_VERTEX_SHIFT(n)	((n)<<7)
//#define GU_VERTEX_8BIT		GU_VERTEX_SHIFT(1)
//#define GU_VERTEX_16BIT		GU_VERTEX_SHIFT(2)
//#define GU_VERTEX_32BITF		GU_VERTEX_SHIFT(3)
//#define GU_VERTEX_BITS		GU_VERTEX_SHIFT(3)
