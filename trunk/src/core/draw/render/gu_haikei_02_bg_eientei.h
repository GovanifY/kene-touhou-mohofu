
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------
	永遠亭背景(製作中)
	-------------------------------------------------------
---------------------------------------------------------*/

		#if 1//(1==USE_F3D)
static void gu_draw_bg_eientei(void)
{
//	#define EH_24V (5*2)
//	#define EH_24V (8*2)
	#define EH_24V (12*2)
	#define EH_R08V (4*2)
	#define EH_L08V (4*2)
	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory((EH_24V+EH_R08V+EH_L08V)*sizeof(Vertex_uvcxyz));	/* GPUが間に合わないのでちゃんと確保 */
	/* ??? */
//
	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
/*32bitのてすと*/
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (((conv_bg_alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->cx256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define EH_Y000 (0)

		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
#define EH_yyy02 x2
#define EH_yyy03 x3

/* ======== 上側、右ふすま ================================================ */
		EH_yyy02	= ( 80+16+7-(4*2) );
		EH_yyy03	= (272+16+7-(4*23) );
		x4		= 256-1/*180+1*/;
		for (j=0; j<EH_R08V; j+=2)
		{
			vertices[j].x = x4/*+10*/ /*0*/ /*100*/ /*EH_yyy03*/;
			vertices[j].y = EH_yyy03/*EH_Y_LOCATE_199*/;
			vertices[j].z = 0;
			vertices[j].u = (0x7f)/*x4*/;
			vertices[j].v = x4+(val3)/*(0xff)+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x4/*0*/ /*EH_yyy02*/;
			vertices[j+1].y = EH_yyy02/*EH_Y000*/;
			vertices[j+1].z = 0;
			vertices[j+1].u = 0/*x4*/;
			vertices[j+1].v = x4+(val3)/*0+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			EH_yyy02 -=  2;/*上縁側*/
			EH_yyy03 +=  (24-1)/*16*/;/*下縁側*/
			x4 +=  32/*32*/ /*64*/;/*テクスチャ*/
		}

/* ======== 上側、左ふすま ================================================ */
		EH_yyy02	= (80);
		EH_yyy03	= (272-1);
		x4		= 0/*180+1*/;
		for (/*j=0*/; j<(EH_R08V+EH_L08V); j+=2)	/* GPU転送が間に合わないので、ちゃんと確保(jは途中から) */
		{
			vertices[j].x = x4/*+10*/ /*0*/ /*100*/ /*EH_yyy03*/;
			vertices[j].y = EH_yyy03/*EH_Y_LOCATE_199*/;
			vertices[j].z = 0;
			vertices[j].u = (0x7f)/*x4*/;
			vertices[j].v = x4+((-val3)&(0xff))/*(0xff)+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x4/*0*/ /*EH_yyy02*/;
			vertices[j+1].y = EH_yyy02/*EH_Y000*/;
			vertices[j+1].z = 0;
			vertices[j+1].u = 0/*x4*/;
			vertices[j+1].v = x4+((-val3)&(0xff))/*0+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			EH_yyy02 +=  2;/*上縁側*/
			EH_yyy03 -=  24/*16*/;/*下縁側*/
			x4 +=  32/*32*/ /*64*/;/*テクスチャ*/
		}


/* ======== 下側床 ================================================ */
		x2 = ( 96) /*+ ((val1&127)-64)*/;/*(固定端)*/
		x3 = (	0);
		x4 = /*0x7f*/0xbf/*128*/ /*256-1*/;
#define EH_Y_LOCATE_199 (200/*100*/-1)
#define EH_Y_LOCATE_271 ((PSP_HEIGHT272-1)/*-1*/)
		int val333=(val3<<2);
		for (/*j=0*/; j<((EH_R08V+EH_L08V)+(EH_24V)); j+=2) 	/* GPU転送が間に合わないので、ちゃんと確保(jは途中から) */
		{
			vertices[j].x = x3;
			vertices[j].y = EH_Y_LOCATE_271;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val333);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = EH_Y_LOCATE_199;
			vertices[j+1].z = 65535/*0*/;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val333);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 += 14   /*72/2*/;/*上縁側*/
			x3 += 32   /*128/2*/;/*下縁側*/
			x4 ^= 0x40/*0x80*/;  /*x4 +=  32*//*64*/;/*テクスチャ*/
		}
		#if 0
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_R08V), NULL, vertices);						/* 上側、右ふすま */
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_L08V), NULL, &vertices[EH_R08V]);				/* 上側、左ふすま */
		#else
		/* 注意：カリング有効の場合、同等になります：裏側ポリゴンが描かれない事を利用したハック */
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_R08V+EH_L08V), NULL, vertices);				/* 上側、右ふすま */	/* 上側、左ふすま */
		#endif
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_24V), NULL, &vertices[(EH_R08V+EH_L08V)]); 	/* 下側床 */

}
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (EH_24V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D専用？*/


/*
0111 7

1000 8
1001 9
1010 a
1011 b

1100 c
1101 d
1110 e
1111 f

*/

// 1 3 5 7 9
// 0 2 4 6 8

		#endif


