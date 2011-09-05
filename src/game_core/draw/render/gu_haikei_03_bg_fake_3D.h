
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------
	擬似3D背景(製作中)
	-------------------------------------------------------
	速度が間に合うのなら、Gumで3Dにしたい所、現在仮素材。
	Gumに移行しても何らかのサポートでGuは要るかも知れない。(画面の端っこの方とか)
	(3Dポリゴンですべて埋め尽くすのは速度的なコストが高いかも知れない。やってないから判りませんが)
---------------------------------------------------------*/

		#if 1//(1==USE_F3D)
static void gu_draw_bg_fake3D(void)
{
//	#define GS_12V (5*2)
//	#define GS_12V (8*2)
	#define GS_12V (6*2)

//	#define GS_22V (GS_12V+(8*2))
//	#define GS_22V (GS_12V+(4*2))	/*dame*/
	#define GS_22V ((GS_12V)+(5*2))

	#define GS_33V ((GS_12V)+(GS_22V))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(((GS_12V)+(GS_22V))*sizeof(Vertex_uvcxyz));	/* 確保 */
//
	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = (((cg.conv_bg_alpha &  0xf0) << 8) | 0x0fff);

//	u16 b00 = (cg.conv_bg_alpha & 0xf0);
//	u16 b_1 = (b00&0x80)?(0xf0):(b00|0x80); 	u16 blendlevel_2 = ((b_1)|(b_1>>4));	blendlevel_2 = ((blendlevel_2)|(blendlevel_2<<8));
//	u16 b_2 = (b00);							u16 blendlevel_1 = ((b_2)|(b_2>>4));	blendlevel_1 = ((blendlevel_1)|(blendlevel_1<<8));
//	u16 b_3 = (b00&0x80)?(b00&0x7f):(0x00); 	u16 blendlevel_0 = ((b_3)|(b_3>>4));	blendlevel_0 = ((blendlevel_0)|(blendlevel_0<<8));

	const u16 u16_b_tbl[3][8] =
	{
		{	0xcccc,0xcccc,0xdddd,0xdddd,	0xeeee,0xeeee,0xffff,0xffff,	},
		{	0x8888,0x9999,0xaaaa,0xbbbb,	0xcccc,0xdddd,0xeeee,0xffff,	},
		{	0x8888,0x8888,0x8888,0x8888,	0x8888,0x8888,0x8888,0x8888,	},
	};
	u8 baa = ((cg.conv_bg_alpha & 0xe0)>>5);
	int blendlevel_0;
	int blendlevel_1;
	int blendlevel_2;
	blendlevel_0 = u16_b_tbl[0][baa];
	blendlevel_1 = u16_b_tbl[1][baa];
	blendlevel_2 = u16_b_tbl[2][baa];


//(cg.conv_bg_alpha)
		#else
//	u32 blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
			/* 32bitのてすと */
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = (((cg.conv_bg_alpha &  0xf0) << 8) | 0x0fff);

//	u16 b00 = (cg.conv_bg_alpha & 0xf0);
//	u16 b_1 = (b00&0x80)?(0xf0):(b00|0x80); 	u16 blendlevel_2 = ((b_1)|(b_1>>4));	blendlevel_2 = ((blendlevel_2)|(blendlevel_2<<8));
//	u16 b_2 = (b00);							u16 blendlevel_1 = ((b_2)|(b_2>>4));	blendlevel_1 = ((blendlevel_1)|(blendlevel_1<<8));
//	u16 b_3 = (b00&0x80)?(b00&0x7f):(0x00); 	u16 blendlevel_0 = ((b_3)|(b_3>>4));	blendlevel_0 = ((blendlevel_0)|(blendlevel_0<<8));

	const u32 u32_b_tbl[3][8] =
	{
		{	0xcccccccc,0xcccccccc,0xdddddddd,0xdddddddd,	0xeeeeeeee,0xeeeeeeee,0xffffffff,0xffffffff,	},
		{	0x88888888,0x99999999,0xaaaaaaaa,0xbbbbbbbb,	0xcccccccc,0xdddddddd,0xeeeeeeee,0xffffffff,	},
		{	0x88888888,0x88888888,0x88888888,0x88888888,	0x88888888,0x88888888,0x88888888,0x88888888,	},
	};
	u8 baa = ((cg.conv_bg_alpha & 0xe0)>>5);
	int blendlevel_0;
	int blendlevel_1;
	int blendlevel_2;
	blendlevel_0 = u32_b_tbl[0][baa];
	blendlevel_1 = u32_b_tbl[1][baa];
	blendlevel_2 = u32_b_tbl[2][baa];
		#endif
	#endif

//	val1 = (player->cx256>>8);
		#if 1
		val3_256 -= (cg.current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define GS_Y000 (0)
#define GS_Y100 (100)
#define GS_Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	上側 */
		/* 176==(GAME_WIDTH/2) */
		x2 = (GAME_X_OFFSET+(GAME_320_WIDTH/2) - 176 -8) /*+ ((val1&0x7f)-64)*/;
		x3 = (GAME_X_OFFSET+(GAME_320_WIDTH/2) - 256 -32 -64 +8+1);
		x4 = 180+1;
		for (j=0; j<(GS_22V); j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = GS_Y100;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel_1;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = GS_Y000;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel_2;
			#endif
			x2 +=  36;/*上縁側*/
			x3 +=  72;/*下縁側*/
			x4 +=  32/*64*/;/*テクスチャ*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
/*	下側 */
		x2 = (GAME_X_OFFSET+(GAME_320_WIDTH/2) -176) /*+ ((val1&0x7f)-64)*/;/*(固定端)*/
		x3 = (GAME_X_OFFSET+(GAME_320_WIDTH/2) -256 -32);
		x4 = (256-1);
		for (/*j=0*/; j<((GS_22V)+(GS_12V)); j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = GS_Y272;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel_0;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = GS_Y100;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel_1;
			#endif
			x2 +=  72;/*上縁側*/
			x3 += 128;/*下縁側*/
			x4 +=  32/*64*/;/*テクスチャ*/
		}
//		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (GS_22V), NULL, vertices);
//		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (GS_12V), NULL, &vertices[GS_22V]);
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, ((GS_22V)+(GS_12V)), NULL, vertices);
}
		#endif
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (GS_12V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D専用？*/
