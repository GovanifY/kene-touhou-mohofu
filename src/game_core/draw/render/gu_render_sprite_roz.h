
#define USE_DXP_ROZ (0)
#if (1==USE_DXP_ROZ)
/* 巧くいかない */

/*---------------------------------------------------------
	DXP改造版
---------------------------------------------------------*/
//#include "../graphics.h"
typedef struct DXP_FVF_2DTEX_F
{
	u16 		u, v;
	float		x, y, z;
} DXP_FVF_2DTEX_F;

int DrawModiGraphFI(
	SPRITE *spr,
	int x1,
	int y1,
	int x2,
	int y2,
	int x3,
	int y3,
	int x4,
	int y4,
//	int gh,
	int trans )
{//343972
	register int u0;
	register int u1;
	register int v0;
	register int v1;
//	DXPGRAPHICSHANDLE* gptr;
//	GHANDLE2GPTR(gptr, gh);
//	if(dxpGraphicsSetup2DTex(gptr->tex, trans) < 0)return -1;
	unsigned int sw_bit = 32;//dxpPsm2SliceSize_bit[gptr->tex->psm][0];
	unsigned int sh_bit = 32;//dxpPsm2SliceSize_bit[gptr->tex->psm][1];
	unsigned int sw;
	unsigned int sh;
	sw = (1<<sw_bit);
	sh = (1<<sh_bit);
	int gptr_u0;
	int gptr_u1;
	int gptr_v0;
	int gptr_v1;
	gptr_u0 = spr->tx;
	gptr_u1 = spr->tx + spr->w;
	gptr_v0 = spr->ty;
	gptr_v1 = spr->ty + spr->h;

//	u0 = gptr->u0;
//	v0 = gptr->v0;
//	u1 = gptr->u1 - u0;
//	v1 = gptr->v1 - v0;
	u0 = gptr_u0;
	v0 = gptr_v0;
	u1 = gptr_u1 - u0;
	v1 = gptr_v1 - v0;
//	unsigned int swn = (u1 + sw - 1) / sw;
//	unsigned int shn = (v1 + sh - 1) / sh;
	unsigned int swn = (u1 + sw - 1) >> sw_bit;
	unsigned int shn = (v1 + sh - 1) >> sh_bit;
//	DXP_FVF_2DTEX_F *buf = (DXP_FVF_2DTEX_F*)dxpGuGetMemory(sizeof(DXP_FVF_2DTEX_F) * (swn * 2 + 2) * shn);
	DXP_FVF_2DTEX_F *buf = (DXP_FVF_2DTEX_F*)sceGuGetMemory(sizeof(DXP_FVF_2DTEX_F) * (swn * 2 + 2) * shn);
	DXP_FVF_2DTEX_F *vtxbuf = buf;
	register float z = 0;//dxpGraphicsData.z_2d;
	__asm__ volatile(

		"lv.s       S030,   %0\n"		"lv.s   S031,   %4\n"
		"lv.s       S002,   %1\n"		"lv.s   S003,   %5\n"
		"lv.s       S012,   %2\n"		"lv.s   S013,   %6\n"
		"lv.s       S022,   %3\n"		"lv.s   S023,   %7\n"
	//
		"vi2f.p     C030,   C030,   1/2\n"		//int→float変換
		"vi2f.p     C002,   C002,   1/2\n"		//int→float変換
		"vi2f.p     C012,   C012,   1/2\n"		//int→float変換
		"vi2f.p     C022,   C022,   1/2\n"		//int→float変換
	//
		"vsub.p     C000,   C002,   C030\n"
		"vsub.p     C010,   C022,   C030\n"
		"vsub.p     C020,   C012,   C022\n"
		"vsub.p     C020,   C020,   C000\n"
		"mtv        %8,     S002\n"
		"mtv        %9,     S012\n"
		"vi2f.p     R002,   R002,   1/2\n"
		"vone.s     S033\n"
		"vrcp.p     R002,   R002\n"
		"mtv        %10,    S100\n"
		"mtv        %11,    S110\n"
		"vi2f.p     R100,   R100,   1/2\n"
		:
		:"m"(x1), "m"(x2), "m"(x3), "m"(x4),
			"m"(y1), "m"(y2), "m"(y3), "m"(y4),
			"r"(u1), "r"(v1), "r"(u0), "r"(v0)
	);

//	v0 = gptr->v0;
//	while(v0 < gptr->v1)

	v0 = gptr_v0;
	while(v0 < gptr_v1)
	{
	//	u0 = gptr->u0;
	//	v1 = psp_min(v0 + sh, gptr->v1);
		u0 = gptr_u0;
		v1 = psp_min(v0 + sh, gptr_v1);
		buf = vtxbuf;

		vtxbuf->u = u0;
		vtxbuf->v = v0;
		__asm__ volatile(
			"mtv        %2,     S003\n"
			"mtv        %3,     S013\n"
			"vi2f.p     R003,   R003,   1/2\n"
			"vsub.p     R003,   R003,   R100\n"
			"vmul.p     R003,   R003,   R002\n"
			"vmul.s     S023,   S013,   S003\n"
			"vdot.q     S022,   R003,   R000\n"
			"vdot.q     S032,   R003,   R001\n"
			"sv.s       S022,   %0\n"
			"sv.s       S032,   %1\n"
			:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
			:"r"(u0), "r"(v0)
		);
		vtxbuf->z = z;
		vtxbuf++;

		vtxbuf->u = u0;
		vtxbuf->v = v1;
		__asm__ volatile(
			"mtv        %2,     S003\n"
			"mtv        %3,     S013\n"
			"vi2f.p     R003,   R003,   1/2\n"
			"vsub.p     R003,   R003,   R100\n"
			"vmul.p     R003,   R003,   R002\n"
			"vmul.s     S023,   S013,   S003\n"
			"vdot.q     S022,   R003,   R000\n"
			"vdot.q     S032,   R003,   R001\n"
			"sv.s       S022,   %0\n"
			"sv.s       S032,   %1\n"
			:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
			:"r"(u0), "r"(v1)
		);
		vtxbuf->z = z;
		vtxbuf++;
	//	while(u0 < gptr->u1)
		while(u0 < gptr_u1)
		{
		//	u1 = psp_min(u0 + sw, gptr->u1);
			u1 = psp_min(u0 + sw, gptr_u1);

			vtxbuf->u = u1;
			vtxbuf->v = v0;
			__asm__ volatile(
				"mtv        %2,     S003\n"
				"mtv        %3,     S013\n"
				"vi2f.p     R003,   R003,   1/2\n"
				"vsub.p     R003,   R003,   R100\n"
				"vmul.p     R003,   R003,   R002\n"
				"vmul.s     S023,   S013,   S003\n"
				"vdot.q     S022,   R003,   R000\n"
				"vdot.q     S032,   R003,   R001\n"
				"sv.s       S022,   %0\n"
				"sv.s       S032,   %1\n"
				:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
				:"r"(u1), "r"(v0)
			);
			vtxbuf->z = z;
			vtxbuf++;

			vtxbuf->u = u1;
			vtxbuf->v = v1;
			__asm__ volatile(
				"mtv        %2,     S003\n"
				"mtv        %3,     S013\n"
				"vi2f.p     R003,   R003,   1/2\n"
				"vsub.p     R003,   R003,   R100\n"
				"vmul.p     R003,   R003,   R002\n"
				"vmul.s     S023,   S013,   S003\n"
				"vdot.q     S022,   R003,   R000\n"
				"vdot.q     S032,   R003,   R001\n"
				"sv.s       S022,   %0\n"
				"sv.s       S032,   %1\n"
				:"=m"(vtxbuf->x), "=m"(vtxbuf->y)
				:"r"(u1), "r"(v1)
			);
			vtxbuf->z = z;
			vtxbuf++;

			u0 += sw;
		}
		#define DXP_VTYPE_2DTEX_F (GU_VERTEX_32BITF | GU_TEXTURE_16BIT)
		sceGuDrawArray(GU_TRIANGLE_STRIP, DXP_VTYPE_2DTEX_F | GU_TRANSFORM_2D, 2 * swn + 2, 0, buf);
		v0 += sh;
	}
	return (0);
}

//	DXP_TRANCE_DATA dxpt;
	ScePspIMatrix4 dxp_gi_pos;

static void render_object_use_rot_zoom(SPRITE *spr)
//int DrawRotaGraphI256(void)
//	int x,
//	int y,
//	int scale_t256,
//	int angle_1024,
//	int gh,
//	int trans,
//	int turn)
//	dxpt.p01 = int x,					dxpt.p01 = ((x256)>>8);
//	dxpt.p02 = int y,					dxpt.p02 = ((y256)>>8);
//	dxpt.p03 = int scale_t256,			dxpt.p03 = t256(1.0)/*scale_t256*/;
//	dxpt.p04 = int angle1024,			dxpt.p04 = angle1024;
//	dxpt.p05 = int gh,					dxpt.p05 = gazo_handle;
//	dxpt.p06 = int trans,				dxpt.p06 = TRUE;
//	dxpt.p07 = int turn DXPDEFARG(0)	dxpt.p07 = 0;
{
	int x;
	int y;
	int scale_t256;
	int angle_1024;
//	int gh;
	int trans;
	int turn;
	x			 = ((spr->cx256)>>8);//dxpt.p01;
	y			 = ((spr->cy256)>>8);//xpt.p02;
	scale_t256	 = (spr->m_zoom_x256);//dxpt.p03;
	angle_1024	 = (spr->rotationCCW1024);//dxpt.p04;
//	gh			 = dxpt.p05;
	trans		 = 1;//dxpt.p06;
	turn		 = 0;//dxpt.p07;
	//
//	DXPGRAPHICSHANDLE *gptr;
//	GHANDLE2GPTR(gptr, gh);
	int gptr_u0;
	int gptr_u1;
	int gptr_v0;
	int gptr_v1;
	gptr_u0 = spr->tx;
	gptr_u1 = spr->tx + spr->w;
	gptr_v0 = spr->ty;
	gptr_v1 = spr->ty + spr->h;
//	register int w = gptr->u1 - gptr->u0;
//	register int h = gptr->v1 - gptr->v0;
	register int w = gptr_u1 - gptr_u0;
	register int h = gptr_v1 - gptr_v0;


//	pspvfpu_use_matrices(NULL, 0, VMAT0 | VMAT1 | VMAT2);//なんか用途がよくわからない
//	ScePspFMatrix4 pos;
	ScePspIMatrix4 pos;

// ---------------- Single (単)
// [S000] [S010] [S020] [S030]
// [S001] [S011] [S021] [S031]
// [S002] [S012] [S022] [S032]
// [S003] [S013] [S023] [S033]

	__asm__ volatile(
		"mtv        %1,     S000\n" 			//scale
		"mtv        %2,     S001\n" 			//angle
	//	"vi2f.s     S000,   S000,    8\n"		// int→float変換と同時に1/256倍する。
	//	"vi2f.s     S001,   S001,    8\n"		// int→float変換と同時に1/256倍する。
		"vi2f.p     C000,   C000,    8\n"		// int→float変換と同時に1/256倍する。

// scale  [S010] [S020] [S030]
// angle  [S011] [S021] [S031]
// [S002] [S012] [S022] [S032]
// [S003] [S013] [S023] [S033]

	//	"vcst.s     S002,   VFPU_2_PI\n"		//angleをラジアン単位からVFPU内部の角度単位に直す
	//	"vfim.s     S002,   1.000000\n" 		// S001 = (1.0f);	// constant real
	//	"vmul.s     S001,   S001[-x],   S002\n"
		"vmov.s     S001,   S001[-x]\n"			// angle = -angle;

		//--------------ここまで、で  がバンク0に完成する

// [回転行列] [回転行列] [回転行列] [S130]
// [回転行列] [回転行列] [回転行列] [S131]
// [回転行列] [回転行列] [回転行列] [S132]
// [S103] [S113] [S123] [S133]

		"vmidt.t    M100\n" 					// 回転行列を作成
		"vrot.t     C100,   S001,   [ c, s, 0]\n"
		"vrot.t     C110,   S001,   [-s, c, 0]\n"
// [S100] [S110] [S120] [S130]
// [S101] [S111] [S121] [S131]
// [S102] [S112] [S122] [S132]
// [S103] [S113] [S123] [S133]

		"vmscl.t    M100,   M100,   S000\n" 	// 回転行列をscale倍する

		"mtv        %3,     S102\n" 			// x, y座標を回転行列に入れる。これで変換用の行列が完成する
		"mtv        %4,     S112\n"
		"vi2f.p     R102,   R102,   1/2\n"		// int→float変換
		//--------------ここまで、で変換用の行列がバンク1に完成する

		"mtv        %5,     S220\n" 			// 画像の縦、横の大きさ
		"mtv        %6,     S221\n"
		"vi2f.p     C220,   C220,   1\n"		// int→float変換と同時に1/2倍する
		"vone.s     S222\n" 					// 1をセット。[w, h, 1]というベクトルができるので、これらを変換する

		"vmov.t     C200,   C220[-x, -y, 1]\n"	// [-w, -h-1]
		"vmov.t     C210,   C220[ x, -y, 1]\n"	// [ w, -h-1]
		"vmov.t     C230,   C220[-x,  y, 1]\n"	// [-w,  h-1]
		//--------------ここまで、で  がバンク2に完成する

		"vtfm3.t    C020,   M100,   C220\n" 	// 行列変換
		"vtfm3.t    C000,   M100,   C200\n"
		"vtfm3.t    C010,   M100,   C210\n"
		"vtfm3.t    C030,   M100,   C230\n"
		//
		"vf2iu.q    C000,   C000,    1/2\n" 	// float→int変換
		"vf2iu.q    C010,   C010,    1/2\n" 	// float→int変換
		"vf2iu.q    C020,   C020,    1/2\n" 	// float→int変換
		"vf2iu.q    C030,   C030,    1/2\n" 	// float→int変換
	//	"vf2iu.q    M000,   M000,    1/2\n" 	// float→int変換	// Matrixは使えないらしい。
		//
		"usv.q      C020,   32 + %0\n"			// 変換したデータの書き出し
		"usv.q      C000,    0 + %0\n"
		"usv.q      C010,   16 + %0\n"
		"usv.q      C030,   48 + %0\n"
		:"=m"(pos): "r"(scale_t256), "r"(angle_1024), "r"(x), "r"(y), "r"(w), "r"(h)
	);

	if(turn)
	{
		/*return*/ DrawModiGraphFI(spr,
			pos.y.x, pos.y.y,
			pos.x.x, pos.x.y,
			pos.w.x, pos.w.y,
			pos.z.x, pos.z.y,
		//	gh,
			trans);
	}
	{	/*return*/ DrawModiGraphFI(spr,
			pos.x.x, pos.x.y,
			pos.y.x, pos.y.y,
			pos.z.x, pos.z.y,
			pos.w.x, pos.w.y,
		//	gh,
			trans);
	}
}


#else
/*---------------------------------------------------------
	汎用スプライトの表示 (回転拡大縮小あり)ルーチン
	-------------------------------------------------------
	現在(2009-11-13現在)横しか slice してないけど、どうも縦横に
	 slice しないと(チルノやレミリアボムの速度低下っぷりから判断して)
	ダメですね。あとで改良しよう。
	-------------------------------------------------------
	逆に弾は(64[dots]以下の > 大弾ぐらいなら) slice しなくても
	いいのかもしれない。
	弾はシステム的にも「汎用スプライトで描かない方が効率が良い」ので、
	専用ルーチンに分岐する予定(2009-11-13現在)です。
	-------------------------------------------------------
	回転しない場合と違い、
	回転する場合は、回転公式を使いますので、
	最後まで相対座標(obj中心を原点o(0,0)とする。)で計算し、
	最後にobjの中心座標(cx,cy)を加えて画面上の描画座標(Guの使い方が1:1なので)とします。
---------------------------------------------------------*/

static void render_object_use_rot_zoom(SPRITE *spr)
{
	/* --- 表示スイッチ */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- 回転拡大前処理 */
	int sin_angle_t256; 	/* 位相  +0度進み */
	int cos_angle_t256; 	/* 位相  +90度進み */
//	int c18_angle;	/* 位相 +180度進み */
	{
		/* 角度は0-1023度 */
		#if (0)/*(memo)*/
	//	sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN512+*/			  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
	//	cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS512+OFFS_COS512+  rotation_angle512+rotation_angle512)&(1024-1)))/*<<8*/);
		sin_angle_t256 = (int)(int256_si n1024(/*rot_sin*/((/*OFFS_SIN1024+*/ 			  (spr->rotationCCW1024))&(1024-1)))/*<<8*/);
		cos_angle_t256 = (int)(int256_si n1024(/*rot_cos*/((	OFFS_COS1024+			  (spr->rotationCCW1024))&(1024-1)))/*<<8*/);
		#endif
		//
		#if (0)//
		sin_angle_t256 = ((si n1024(((spr->rotationCCW1024))) ) );/*fps_factor*/	/* CCWの場合 */
		cos_angle_t256 = ((co s1024(((spr->rotationCCW1024))) ) );/*fps_factor*/
		#else
		int256_sincos1024( ((spr->rotationCCW1024)), &sin_angle_t256, &cos_angle_t256);
		#endif
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
	unsigned int blendlevel = (spr->color32);
		#endif
	#endif

	/* --- 汎用スプライトを一枚描画する */
	/* ある程度の大きさを持ったスプライトは短冊状に分割しないと */
	/* とてつもなく遅くなる */

	unsigned int w_rest;
	w_rest = spr->w + SLICE_64_SIZE;

	unsigned int count4;
	#if 0
	//count4 = (((spr->w / SLICE_64_SIZE)+1)*4);
	#else
	count4 = 0;
	unsigned int i;
	i = 0;
	do
	{
		count4 += 4;
		i += SLICE_64_SIZE;
		w_rest -= SLICE_64_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(count4 * sizeof(Vertex_uvcxyz_C32));
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_64_SIZE;
	/* 回転するので x,y 座標は、「画面の座標ではなく」、「obj中心を原点o(0,0)とした、相対座標」で計算しておく。 */
//	unsigned int/*short*/ x_pos = ((spr->cx256>>8));
//	unsigned int/*short*/ y_pos = ((spr->cy256>>8));
//(r32) unsigned int/*short*/ x_pos = ((spr->cx256>>8)-(spr->w >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
//(r32) unsigned int/*short*/ y_pos = ((spr->cy256>>8)-(spr->h >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
	unsigned int/*short*/ x_pos = ( -(spr->w >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
	unsigned int/*short*/ y_pos = ( -(spr->h >> 1));/* 中心座標から画像サイズの半分を引き、左上座標を計算 */
	i = 0;
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
		pos += SLICE_64_SIZE;
		/* --- 回転拡大処理 */
		int center_x;
		int center_y;
//		center_x = ((spr->cx256>>8) 	) + (spr->w >> 1);/*/2*/
//		center_y = ((spr->cy256>>8) 	) + (spr->h >> 1);/*/2*/
		center_x = ((spr->cx256>>8) 	) ;/*/2*/
		center_y = ((spr->cy256>>8) 	) ;/*/2*/
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->m_zoom_x256) * ((65536/256)); /* 拡大率は0-256倍なので0-65536倍へ変換。 */
	//	zoom_y256 = (spr->m_zoom_y256) * ((65536/256)); /* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_x256 = (spr->m_zoom_x256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_y256 = (spr->m_zoom_y256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
//(r33) 		ifx -= center_x;
//(r33) 		ify -= center_y;
//1859267 -> 1859251
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* [右CW]右が 0度で時計回りの角度系 */
			irx = ((ifx * cos_angle_t256) - (ify * sin_angle_t256));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle_t256) + (ify * cos_angle_t256));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* [下CCW]下が 0度で反時計回りの角度系 */
			irx = ((ify * sin_angle_t256) + (ifx * cos_angle_t256));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ify * cos_angle_t256) - (ifx * sin_angle_t256));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 0
			/* [左CCW(?)]左が 0度で反時計回りの角度系 */
			irx = ((ifx * sin_angle_t256) - (ify * cos_angle_t256));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle_t256) + (ify * sin_angle_t256));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
//
			irx = ((irx * (zoom_x256))>>8);
			iry = ((iry * (zoom_y256))>>8);
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
#endif
