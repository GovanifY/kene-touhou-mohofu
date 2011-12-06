
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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
	/*float*/u8 	u;			/* 0(左端) ... 255(右端) */ 		/* テクスチャ(u, v)座標 */
	/*float*/u8 	v;			/* 0(上端) ... 255(下端) */
	u32 			color;		/* 0x00000000 ... 0xffffffff */ 	/* 頂点カラー(ARGB8888) */
	/*float*/s8 	x;			/* -128 ... 0(中心) ... 127 */		/* 頂点(x, y, z)座標 */
	/*float*/s8 	y;			/* -128 ... 0(中心) ... 127 */
	/*float*/s8 	z;			/* -128 ... 0(中心) ... 127 */
} Vertex_cube;
/*
	このポリゴンは単純な正方体。(サイコロ)
	但し、CCWの場合に、内側からみて面が表になるようにしてある。
	(外側から見れば全部裏なので表示されない)
	サイコロなので、6面体。1つの面は GU_TRIANGLES (三角形) を 2つ使って描く。(1つの面 == 正方形 == 四角形 == 三角形が2つ分)
	したがって、 (12*3) == (6[面] * 2[面当たり三角の数] * 3[頂点。三角形なので3頂点])

*/
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
/* 3D空間の為の基本行列 */
static ScePspFMatrix4 m4_projection3d;	/* 良くわかんないけど、画角を計算しておく。 			模倣風では一切動かしてない。標準画角で固定。 */
static ScePspFMatrix4 m4_view;			/* (レンダリングする場合の)視点座標系。 				模倣風では使っている。 */
static ScePspFMatrix4 m4_model; 		/* (レンダリングする場合の)ワールド座標系==基準座標。	模倣風では一切動かしてない。単位行列で初期化して固定。 */
/*
	模倣風では、基本的な3D空間は固定で一切動かしていない。
	物体自体を回転させている。
	物体の回転は次の3つの行列で管理している。
	p9_qcam 	カメラ行列。カメラ(視点)というのは御幣があるかもしれない。
	p9_qa		行列Ａ。(前回の位置を取っておく用)
	p9_qb		行列Ｂ。(計算中の今回の位置用)


*/
#if (1==TEST_AUTO_ROT)
static ScePspQuatMatrix p9_qcam;	/* カメラ(正確には行列じゃなくてクォータニオン。クォータニオン==4x4行列) */
static ScePspQuatMatrix p9_qa;		/* 行列Ａ(正確には行列じゃなくてクォータニオン。クォータニオン==4x4行列) */
static ScePspQuatMatrix p9_qb;		/* 行列Ｂ(正確には行列じゃなくてクォータニオン。クォータニオン==4x4行列) */
#endif
/*
	クォータニオン:
	ポリゴンで姿勢を一意に確定させる為に使う4x4行列。
	-------------------------------------------------------
	姿勢っていっても色々あるよね。
	基準の座標系に対して物体がどれだけ傾いてるかの姿勢。とか
	基準の座標系に対してカメラ(視点)がどれだけ傾いてるかの姿勢。とか

	-------------------------------------------------------
	クォータニオンというのは、3Dでつかう行列で4x4行列。
	3Dなら(x,y,zなんだから)3x3行列で良くないの？と思うかも知れないが、
	実際やってみれば判るが、全然良くない。
	-------------------------------------------------------
	3Dは表示するが「絶対動かさないぞ」ってんなら、3x3行列でもまるで問題無い。
	んが、ゲームでは普通、動くよね。
	動く計算をさせたいなら、4x4行列じゃないと、例外処理だらけで複雑になるわ。
	行列回転させたら元に戻らんわで良い事一つも無い。
	そーゆー訳で4x4行列。
	-------------------------------------------------------
	あ、クォータニオン自体の説明がまだかな？
	んーと、ポリゴンで姿勢をあらわす場合、基準の座標系に対して結局どれだけずれてるかが
	判れば、姿勢は一意に決まる。
	基準の座標系に対して3x3行列。で差分を持っていれば、姿勢は一意に決まる。
	これが、クォータニオン。で、実際は4x4行列。
	-------------------------------------------------------
	姿勢を決めるという考え方は何もクォータニオンの専売特許って訳ではなく、
	クォータニオンで、姿勢を決めるという考え方以外にも、「オイラー角で姿勢を決めるという考え方」もある。
	(こっちの方が古い考え方)
	でも、オイラー角で姿勢を決める場合は、「結果が回転する順序に依存する」ので、
	回転する順序を決めておかないと望む結果が得られない。

	具体的には、
	始めにx軸方向に45度回して、次にy軸方向に60度回した結果の向きと。
	始めにy軸方向に60度回して、次にx軸方向に45度回した結果の向きは、
	同じ向きにならないで、全然違う方向を向いてるって話。

	クォータニオンの場合は、回転する順序とか考え方自体に無いので、こういう問題は起こらない。
*/



/* 直接行列に値を設定してみるテスト。 */
#define USE_CHOKUSETU_SETTEI_TEST (1)

#if (1==USE_CHOKUSETU_SETTEI_TEST)
static ScePspQuatMatrix p9_test;	/* テスト用 */
#endif

		#define USE_5SEC (0)
		#if (1==USE_5SEC)/* 周期カウンタ値の計算 */
static	int nnn;
		#else
static	u8 nnn;
		#endif
	// run sample
//	/*int*/float val_x;
//	/*int*/float val_y;
//	/*int*/float val_z;
//	/*int*/float val_w;


#if (1==TEST_AUTO_ROT)
/*
	新しいカメラ位置を決める。
	カメラ位置は、x値, y値, z値, 共に単なる乱数。
	(x値, y値, z値は角度)。
	角度は 0 ... 511 まで。
	(1周512分割の単位系)。

	この x値, y値, z値 はベクトルではなくて、
	オイラー角を計算する場合に回るカメラの量(パンする量)。

	つまり、
	オイラー角を計算する場合に、
	始めにx軸を x値分 回して、
	次にy軸を y値分 回して、
	最後ににz軸を z値分 回す。
	その結果出来たベクトルを使う。


	カメラ位置から、行列Ｂへ

*/
static void test_auto_rot_aaaa(void)
{
	/* 前回の行列をとっとく(Aが移動完了位置。Bは移動中の位置) */
	vfpu_quaternion_copy(&p9_qa, &p9_qb);	/* 前回の行列Ｂを行列Ａにコピーする。(正確には行列じゃなくてクォータニオン。クォータニオン==4x4行列) */
//	vfpu_quaternion_from_euler(&p9_qb,
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f));
	#if (1)
	/* 新しい向きを決める */
	vfpu_quaternion_from_euler512_int(&p9_qb,	/* オイラー角の方式で回転する。回転順序はｘ軸→ｙ軸→ｚ軸の順序で回転させる。 */
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f));
		((vfpu_rand_8888(0, 255))&(512-1)),
		((vfpu_rand_8888(0, 255))&(512-1)),
		((vfpu_rand_8888(0, 255))&(512-1)));
	/* 向きにする(Bは意味が変わる) */
//	vfpu_quaternion_exp(&p9_qb, &p9_qb);
	vfpu_quaternion_ln(&p9_qb, &p9_qb); 		/* 向きを計算する。 */
	#else
	/* 簡略化して大体こんな感じでも良いかも。このままだと露骨に遅くなりそうだけど)*/
//	vfpu_quaternion_identity(&p9_qb);	/* 単位クォータニオンで初期化 */
	p9_qb.x = (float)((vfpu_rand_8888(0, 255))&(512-1))/512.0f;//。x
	p9_qb.y = (float)((vfpu_rand_8888(0, 255))&(512-1))/512.0f;//。y
	p9_qb.z = (float)((vfpu_rand_8888(0, 255))&(512-1))/512.0f;//。z
	p9_qb.w = 1.0f;
	#endif
}
#endif

static void gu_init_vfpu(void)
{
	// vfpu has a random number generator,
	// and it needs to be seeded
	{
		u64 tick;
		sceRtcGetCurrentTick(&tick);
	//	vfpu_srand((u32)tick / 10000.0f);	これでは使い方がおかしいので間違い。(vfpu_srand は int(32bitの整数値)で値を設定するので PSPSDKのサンプルは間違い。これではfloat->int変換で常に値が0になる)
		vfpu_srand((u32)tick ); 			/* vfpu_srand gets int. (下位32bitのみ使い、u32の範囲で値を設定する。上位32bitは使わないで捨てる。) */
	}
	/* ポリゴンレンダリングする場合の、パースを決める。画角等を意図的に変えたら面白いかもしれない。 */
	vfpu_perspective_matrix(&m4_projection3d, 60.0f, 480.0f/272.0f, 0.5f, 1000.0f);
	/* 初期化 */
	vfpu_identity_matrix(&m4_view); 		/* (視点用行列を)単位行列で初期化する。 */
	vfpu_identity_matrix(&m4_model);		/* (モデル用行列を)単位行列で初期化する。 */
	#if (1==USE_CHOKUSETU_SETTEI_TEST)
	vfpu_quaternion_identity(&p9_test); 	/* 単位クォータニオンで初期化 */
	#endif

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
	#if (1==USE_5SEC)/* 周期カウンタ値の計算 */
	/*
	nnn: 周期カウンタ値: 0 ... 299 までの値。1フレームに1増分。
	*/
	nnn++;
	if (nnn >= byou60(5) ) /* 300==(5[sec] x 60[frame])*/
	{
		nnn = 0;
		/* 300カウントに一度だけ、新カメラ位置を決めなおす。 */
		test_auto_rot_aaaa();
	}
	#else
	/*
	nnn: 周期カウンタ値: 0 ... 255 までの値。1フレームに1増分。
	*/
	nnn++;
	nnn &= 0xff;
	if (0==nnn) /* 256==(4.266...[sec] x 60[frame])*/
	{
		/* 256カウントに一度だけ、新カメラ位置を決めなおす。 */
		test_auto_rot_aaaa();
	}
	#endif

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
			/*
			t65536: 等分カウンタ値: 0 ... 65536 までの値。1フレームに(65536/300)増分。
			*/
		#if (1==USE_5SEC)/* 等分値の計算 */
		//	float t = (nnn)*((float)(1.0f/300.0f));
			u32 t65536 = (nnn)*((u32)(65536.0/300.0));
		//	vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
			/* 周期カウンタが 0 ... 300 なので、非等分値の計算で使える形式の 0 ... 65536 へ変換する。 */
		#else
		//	float t = (nnn)*((float)(1.0f/256.0f));
			u32 t65536 = (((nnn)<<8)|(nnn));
		//	vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
			/* 周期カウンタが 0 ... 255 なので、非等分値の計算で使える形式の 0 ... 65536 へ変換する。 */
		#endif
		#if (1)/* 非等分値の計算 */
		u32 i_rate65536;
		//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
			i_rate65536 = (int)(vfpu_ease_in_out65536(t65536)); 	/* t65536の等分カウンタ値を使い、非等分になるよう値を修正する。
			非等分の方式は「ease_in_out」これは、始めと最後がゆっくりで間が速いという配分方式。 */
		#endif
			/*
				クォーターニオンへ設定する。(カメラと行列Ａと行列Ｂへ設定する)
				設定はリニア値(等分値)で設定するので、予め下準備として「ease_in_out」で非等分値を計算しておく。
			*/
			vfpu_quaternion_sample_linear65536(&p9_qcam, &p9_qa, &p9_qb, i_rate65536);
		}
		vfpu_quaternion_exp(&p9_qcam, &p9_qcam);			/* カメラ用 */
		vfpu_quaternion_to_matrix(&p9_qcam, &m4_view);		/* カメラを視点へセット */
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
		pgc_set_matrix_PROJECTION(	&m4_projection3d);	//sceGuSetMatrix(GU_PROJECTION, &m4_projection3d);		/* 透視変換用マトリクス */
		pgc_set_matrix_VIEW(		&m4_view);			//sceGuSetMatrix(GU_VIEW,		&m4_view);				/* 視点変換ビュー用マトリクス */
		pgc_set_matrix_MODEL(		&m4_model); 		//sceGuSetMatrix(GU_MODEL,		&m4_model); 			/* ワールド座標用マトリクス */

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
