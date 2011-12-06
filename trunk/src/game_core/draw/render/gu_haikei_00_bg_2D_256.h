
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------
	2D背景(製作中)
	-------------------------------------------------------
	説明:
	GU_SPRITES(頂点をスプライトとして描く)、				多分割2D平面を描く場合、1分割領域あたり2頂点無駄。縦分割をするとさらに2頂点無駄。
	GU_TRIANGLES(頂点を三角形の繰り返しとして描く)、		多分割2D平面を描く場合、1分割領域あたり2頂点無駄。縦分割をするとさらに2頂点無駄。
	GU_TRIANGLE_FAN(始点の頂点を中心に扇上にして描く)、 	多分割2D平面を描く場合、4分割領域あたり5頂点無駄。分割数が奇数の場合、作るのめんどくさいかも。
	GU_TRIANGLE_STRIP(梯子上頂点を使って描く)、 			多分割2D平面を描く場合、1分割領域あたり0頂点無駄。縦分割をするとさらに2頂点無駄。
	-------------------------------------------------------
	GU_TRIANGLE_FANは円(正多角形)のポリゴンに2Dテクスチャをuv座標で簡単に張れる。
	-------------------------------------------------------
	この中で、多分割2D平面を描くのなら、GU_TRIANGLE_STRIP を使う方式が、
	最も頂点を減らせて、どんな平面でも1ポリゴンで描ける。
	ここでは、GU_TRIANGLE_STRIP を使う。
	-------------------------------------------------------

2D背景を描く幅[pixel] == 352[pixel] == 480[pixel]-128[pixel] == (psp横)[pixel]-(パネル横)[pixel]
11 [分割] == 352[pixel] / 32[slice]

	-------------------------------------------------------

11分割の場合は、矩形を描くので12頂点分いる。

1	2	3  ... 11  12(12頂点分横に必要)
+---+---+-	  -+---+
| 1 | 2 |  ... | 11| (11分割で、矩形を描く)
+---+---+-	  -+---+

	-------------------------------------------------------

	矩形なので、横に 12 頂点分、縦に 2 頂点分、必要。
	つまり全部で 24頂点分、必要。
	ND_24V == 2D背景24頂点分(Ni Dei haikei 24 Vertexes.)

	-------------------------------------------------------
	テクスチャ縦が256[pixel]なので、 1:1分割の場合、縦256までしか描けない。
	(無理やり描くと2D変形されてしまうので描けない。)
	pspの縦は272[pixel]なので、256[pixel]では足りない。そこで 2回に分けて描く。
	272[pixel] == 256[pixel] + 16[pixel]
	-------------------------------------------------------
	...
	-------------------------------------------------------
	説明上は2回に分けて描く。のだが、ここは1回で描ける。
	(説明上はポリゴンを2回に分割するが、実際の実装は1つのポリゴンで描けるので1回で描く。)
	GU_TRIANGLE_STRIP(梯子上頂点を使って描く)
	-------------------------------------------------------
	前提条件として、裏向きポリゴンの描画を禁止してある事(==カリング有効)が必要。
	模倣風では、
	sceGuEnable(GU_CULL_FACE);	カリング有効(裏向きポリゴンは描画しない)
	sceGuFrontFace(GU_CW);		右回りのポリゴンを表として設定。
	つまり右回りのポリゴンは描く。左回りのポリゴンは描かない。
	左上から順に、梯子上に矩形を描画する。
	直後に左下に、梯子上に矩形を描画する。
	そうすると右上から左下に移る部分は、必ずポリゴンが裏向きになる。
	裏向きポリゴンは、描画されないので都合がよい。
	-------------------------------------------------------
	裏向きポリゴンは、ハードウェアーでgpuが描画前に跳ねるので、
	どんなにサイズが大きくても描画速度に影響しない。
	-------------------------------------------------------
	Scissorを使って描画範囲座標を指定しておけば、ハードウェアーでgpuが描画前に跳ねるので、
	描画範囲外なら、どんなにサイズが大きくても描画速度に影響しない。
	-------------------------------------------------------
---------------------------------------------------------*/
#if (1)
static void gu_draw_bg_2D(void)
{
	#define ND_24V ((12*2))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(((ND_24V)+(ND_24V))*sizeof(Vertex_uvcxyz));	/* ちゃんと確保 */
//
	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((cg.conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(cg.conv_bg_alpha)
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (((cg.conv_bg_alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->center.x256>>8);
		#if 1
		val3_256 -= (cg.current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define ND_Y000 (0)
#define ND_Y255 ((256)-1)
#define ND_Y271 ((272)-1)
/* 何故か1[ドット]ずれる(???) もしかしてY座標0からじゃなくて1から？？ */
#define ND_ADD1 (1)
//#define ND_Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	上側 */
		/* 176==(GAME_WIDTH/2) */
		x2 = (0) /*+ ((val1&0x7f)-64)*/;
		x3 = (0);
		x4 = (0);
		for (j=0; j<(ND_24V); j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = (ND_Y255);
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff+1)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = (ND_Y000);
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 +=  32;/*上縁側*/
			x3 +=  32;/*下縁側*/
			x4 +=  32/*64*/;/*テクスチャ*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
/*	下側 */
		/* 176==(GAME_WIDTH/2) */
		x2 = (0) /*+ ((val1&0x7f)-64)*/;
		x3 = (0);
		x4 = (0);
		for (/*j=0*/; j<((ND_24V)+(ND_24V)); j+=2)	/* ちゃんと確保(jは途中から) */
		{
			vertices[j].x = x3;
			vertices[j].y = (ND_Y271+ND_ADD1);
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (/*0xff*/0x0f+1+ND_ADD1)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = (ND_Y255/*+1*/);
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 +=  32;/*上縁側*/
			x3 +=  32;/*下縁側*/
			x4 +=  32/*64*/;/*テクスチャ*/
		}
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, ((ND_24V)+(ND_24V)), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (ND_12V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D専用？*/
}
#endif
