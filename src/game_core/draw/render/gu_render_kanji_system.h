
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#if 0
	#define SERIFU_WINDOW_X_SIZE_340		(340)
	#define SERIFU_WINDOW_Y_SIZE_070		(70)
	#define SERIFU_WINDOW_SPACE_020 		(20)	/* [画面端マージン] */
	#define SERIFU_WINDOW_X_POSITION_020	(SERIFU_WINDOW_SPACE_020)
	#define SERIFU_WINDOW_Y_POSITION_182	(272-70-20)/* 182 == (272-70-20) == (GAME_HEIGHT-SERIFU_WINDOW_Y_SIZE_070-SERIFU_WINDOW_SPACE_020) */
#else
	#define SERIFU_WINDOW_X_SIZE_340		(340)
	#define SERIFU_WINDOW_X_SIZE_256		(256)
	#define SERIFU_WINDOW_Y_SIZE_070		(70)/*70 == (16*4)+6 ??? */
	#define SERIFU_WINDOW_Y_SIZE_052		(52/*52*/)/*70 == (16*3)+4 ??? */
	#define SERIFU_WINDOW_Y_SIZE_012		(12)
	/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-SERIFU_WINDOW_X_SIZE_340)/2) == SERIFU_WINDOW_SPACE_020 */
	#define SERIFU_WINDOW_SPACE_020 		(6)
	#define SERIFU_WINDOW_X_POSITION_020	(SERIFU_WINDOW_SPACE_020)
	#define SERIFU_WINDOW_Y_POSITION_182	(GAME_HEIGHT-SERIFU_WINDOW_Y_SIZE_070-SERIFU_WINDOW_SPACE_020)
	#define SERIFU_WINDOW_Y_POSITION_212	(212)
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

//	#def ine SC RIPT_FONT_SCREEN_TEXTURE_OFFSET_X 	(0) /* [テクスチャオフセット] */
	#define FONT_SCREEN_MARGIN_010					(10)	/* [フォント端マージン] */


#if 1
	//#define SUPEKA_WINDOW_Y_SIZE_070		(32/*70*/)/*70 == (16*4)+6 ??? */
	/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-SUPEKA_WINDOW_X_SIZE_340)/2) == SUPEKA_WINDOW_SPACE_020 */
	//#define SUPEKA_WINDOW_SPACE_020		(6)
	#define SUPEKA_WINDOW_X_POSITION_096	(128-32)/*(ボス専用)*/
	#define SUPEKA_WINDOW_Y_POSITION_012	(12)/*(ボス専用)*/
#endif

#if 0
	/*(通常時)*/
	ml_font[(ML_LINE_04)].x 	= (FONT_SCREEN_MARGIN_010)+(SERIFU_WINDOW_X_POSITION_020);
	ml_font[(ML_LINE_04)].y 	= (FONT_SCREEN_MARGIN_010)+(SERIFU_WINDOW_Y_POSITION_182);
	/*(ボス時)*/
	ml_font[(ML_LINE_04)].x 	= (SUPEKA_WINDOW_X_POSITION_096);
	ml_font[(ML_LINE_04)].y 	= (SUPEKA_WINDOW_Y_POSITION_012);
#endif

//	#define ML_FONT_TEXTURE_Y_POSITION	(1+8) /* [テクスチャオフセット※1] */
//	#define ML_FONT_SCREEN_X_POSITION
//	#define ML_FONT_SCREEN_Y_POSITION
//	#define ML_FONT_HEIGHT_Y			18*3ぐらい?==(1+4+47/*70*/) /* [テクスチャサイズ] */


	#define ML_FONT_TEXTURE_Y_POSITION	(1+8/*+18*/) /* [テクスチャオフセット※1] */
	#define ML_FONT_SCREEN_X_POSITION	(ml_font[(gg)].x)
	#define ML_FONT_SCREEN_Y_POSITION	(ml_font[(gg)].y)
	#define ML_FONT_HEIGHT_Y			(18) /* [テクスチャサイズ] 70 */


static void gu_draw_text_lines(void)
{
	unsigned int gg;
	unsigned int yyy18;
	yyy18 = 0;
	for (gg=0; gg<(8); gg++)
	{
		if (0 != ml_font[gg].timer)/*(個々のメッセージ表示時間、個々のメッセージ表示時間上限。)*/
		{	// 0で非表示。1フレーム表示する毎にデクリメント。(自動で値を1減らす)。
			ml_font[gg].timer--;/*(オートデクリメント)*/
			// 全体の表示時間のメッセージ表示時間は独立して別にある。
			/* 実験:
			(a) SLICE_64_SIZE 64 で 512 x 272 転送 (場合分けなし)
			(b) SLICE_64_SIZE 64 で 480 x 272 転送 (場合分けあり)
			(c) SLICE_64_SIZE 32 で 480 x 272 転送 (場合分けなし)
			結果: 速い : (b) > (c) > (a)  : 遅い
			 */
			unsigned int j;
			unsigned int i;
			/* [※1] ((16/2)[pixel]+1[pixel](Gu干渉ライン)) */
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
				//	#define count29 (8/*10*/)/* 10 == 2 x (320/64[SLICE_64_SIZE32] ) */ 	/*(カード)*/
					#define count29 (10)/* 10 == 2 x (320/64[SLICE_64_SIZE32] ) */			/*(メッセージ)*/
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
				vertices[(i)].u 	= j;
				vertices[(i)].v 	= 1+(ML_FONT_TEXTURE_Y_POSITION)+yyy18;/*+(0)*/
				vertices[(i)].x 	= (ML_FONT_SCREEN_X_POSITION)+j/**480/512*/;
				vertices[(i)].y 	= (ML_FONT_SCREEN_Y_POSITION);/*+(0)*/
				vertices[(i)].z 	= 0;
				vertices[(i)+1].u	= (j+w_size99);
				vertices[(i)+1].v	= 1+(ML_FONT_TEXTURE_Y_POSITION)+yyy18+(ML_FONT_HEIGHT_Y);// /*272*/ /*320*/
				vertices[(i)+1].x	= (ML_FONT_SCREEN_X_POSITION)+(j+w_size99);/**480/512*/
				vertices[(i)+1].y	= (ML_FONT_SCREEN_Y_POSITION)+(ML_FONT_HEIGHT_Y);/*272*/
				vertices[(i)+1].z	= 0;
				i += 2;
				j += SLICE_64_SIZE32;
			}
			sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count29), NULL, vertices);
		}
		yyy18 += (18);
	}
}

