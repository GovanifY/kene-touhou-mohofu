
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
	#define SCRIPT_WINDOW_X_SIZE_340		(340)
	#define SCRIPT_WINDOW_Y_SIZE_070		(70)
	#define SCRIPT_WINDOW_SPACE_020 		(20)	/* [画面端マージン] */
	#define SCRIPT_WINDOW_X_POSITION_020	(SCRIPT_WINDOW_SPACE_020)
	#define SCRIPT_WINDOW_Y_POSITION_182	(272-70-20)/* 182 == (272-70-20) == (GAME_HEIGHT-SCRIPT_WINDOW_Y_SIZE_070-SCRIPT_WINDOW_SPACE_020) */
#else
	#define SCRIPT_WINDOW_X_SIZE_340		(340)
	#define SCRIPT_WINDOW_X_SIZE_256		(256)
	#define SCRIPT_WINDOW_Y_SIZE_070		(70)/*70 == (16*4)+6 ??? */
	#define SCRIPT_WINDOW_Y_SIZE_052		(52/*52*/)/*70 == (16*3)+4 ??? */
	#define SCRIPT_WINDOW_Y_SIZE_012		(12)
	/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-SCRIPT_WINDOW_X_SIZE_340)/2) == SCRIPT_WINDOW_SPACE_020 */
	#define SCRIPT_WINDOW_SPACE_020 		(6)
	#define SCRIPT_WINDOW_X_POSITION_020	(SCRIPT_WINDOW_SPACE_020)
	#define SCRIPT_WINDOW_Y_POSITION_182	(GAME_HEIGHT-SCRIPT_WINDOW_Y_SIZE_070-SCRIPT_WINDOW_SPACE_020)
	#define SCRIPT_WINDOW_Y_POSITION_212	(212)

#endif
static void gu_draw_serifu_screen(void)
{
/* 実験:
(a) SLICE_64_SIZE 64 で 512 x 272 転送 (場合分けなし)
(b) SLICE_64_SIZE 64 で 480 x 272 転送 (場合分けあり)
(c) SLICE_64_SIZE 32 で 480 x 272 転送 (場合分けなし)
結果: 速い : (b) > (c) > (a)  : 遅い
 */
	unsigned int j;
	unsigned int i;
	#define SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_X 	(0) /* [テクスチャオフセット] */
	#define SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_Y 	(1+8) /* [テクスチャオフセット※1] */
	#define SCRIPT_FONT_SCREEN_TEXTURE_SIZE_Y		(1+4+47/*70*/) /* [テクスチャサイズ] */
	#define FONT_SCREEN_MARGIN_010		(10)	/* [フォント端マージン] */
	/* [※1] ((16/2)[dots]+1[dots](Gu干渉ライン)) */
	/* 8[ライン(depth_32bit_int)]==16[ライン(depth_16bit_short)] */
	#if 0
	unsigned int count29;
	count29 = 0;
	for (j=0; (j+SLICE_64_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_64_SIZE32)
	{
		count29 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			/*
			320 == 480-((6[画面端マージン] + 10[フォント端マージン] )x 2) -(128[パネルウィンドウ幅])
			*/
		//	#define count29 (16)/* 16 == 2 x (512/64[SLICE_64_SIZE32] ) */
		//	#define count29 (10)/* 16 == 2 x (512/64[SLICE_64_SIZE32] ) */
			#define count29 (10)/* 10 == 2 x (320/64[SLICE_64_SIZE32] ) */
		//	#define count29 (12)/* 10 == 2 x (320/64[SLICE_64_SIZE32] ) */
		#else
			#define count29 (30)/* 30 == 2 x (480/32[SLICE_64_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count29 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	//#if (0==USE_SLICE_32)
	//unsigned int w_size;
	//w_size = SLICE_64_SIZE32;
	//#else
		#define w_size99 SLICE_64_SIZE32
	//#endif
//	for (; (j+SLICE_64_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count29; )
	{
	//	#if (0==USE_SLICE_32)
	//	/* 最後 */
	//	if (i==(count29-2))
	//	{
	//	//	w_size = 32;	/* 32 == 512-480 */
	//		w_size = 44;	/* 44 == 384-340 */
	//	}
	//	#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 1+(SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_Y);/*+(0)*/
		vertices[(i)].x = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_X_POSITION_020)+j/**480/512*/;
		vertices[(i)].y = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_Y_POSITION_182);/*+(0)*/
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+w_size99);
		vertices[(i)+1].v = 1+(SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_Y)+(SCRIPT_FONT_SCREEN_TEXTURE_SIZE_Y);/*272*/ /*320*/
		vertices[(i)+1].x = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_X_POSITION_020)+(j+w_size99);/**480/512*/
		vertices[(i)+1].y = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_Y_POSITION_182)+(SCRIPT_FONT_SCREEN_TEXTURE_SIZE_Y);/*272*/
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_64_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count29), NULL, vertices);
}
