
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/

/*---------------------------------------------------------
	せりふ等を表示する場合の、
	枠の半透明ポリゴンを描く。
---------------------------------------------------------*/

static void gu_draw_rect_window(int rect_type)
{
	/* -- せりふウィンドウを描画 */
	/* テクスチャーがあるとテクスチャー優先でフラットポリゴンが描画出来ないので */
	/* 一時的にテクスチャー無効モードに切り替える */
	PGC_SEND(PGC_TEXTURE_2D, PGC_OFF);	//sceGuDisable(GU_TEXTURE_2D);/* テクスチャー無効モードにする */
	{
		typedef struct
		{
			u16 	p1x;
			u16 	p1y;
			u16 	p2w;
			u16 	p2h;
			u32 	c1;
			u32 	c2;
		} RECT_WINDOW_TABLE;
		static RECT_WINDOW_TABLE rect_table[(ML_HAIKEI_04_MAX)] =
		{
			{(SERIFU_WINDOW_X_POSITION_020),(SERIFU_WINDOW_Y_POSITION_182),(SERIFU_WINDOW_X_SIZE_256),	(SERIFU_WINDOW_Y_SIZE_012),(0x7f0f0f0f),(0x7f0f0f0f)},/* ML_HAIKEI_00_MISIYOU		  / 緑 GREEN 未使用 */
			{(SUPEKA_WINDOW_X_POSITION_096),(SUPEKA_WINDOW_Y_POSITION_012),(SERIFU_WINDOW_X_SIZE_256),	(SERIFU_WINDOW_Y_SIZE_012),(0x7fff0f0f),(0x7f0f0f0f)},/* ML_HAIKEI_01_BOSS_SPELL_CARD / 青 BLUE ボスカード */
			{(SERIFU_WINDOW_X_POSITION_020),(SERIFU_WINDOW_Y_POSITION_212),(SERIFU_WINDOW_X_SIZE_256),	(SERIFU_WINDOW_Y_SIZE_012),(0x7f0f0fff),(0x7f0f0f0f)},/* ML_HAIKEI_02_JIKI_SPELL_CARD / 赤 RED	自機カード */
			{(SERIFU_WINDOW_X_POSITION_020),(SERIFU_WINDOW_Y_POSITION_182),(SERIFU_WINDOW_X_SIZE_340),	(SERIFU_WINDOW_Y_SIZE_052),(0x7f0f0f0f),(0x7f0f0f0f)},/* ML_HAIKEI_03_MESSAGE		  / 黒 DARK GRAY メッセージ背景 */
		};
	//	#define SERIFU_WINDOW_TEXTURE_FLAGS 	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	//	#define SERIFU_WINDOW_TEXTURE_FLAGS 	(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#define SERIFU_WINDOW_TEXTURE_FLAGS 	(				GU_COLOR_8888 | 	GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//GU_VERTEX_32BITF|GU_TRANSFORM_2D
		// we make local copies of the line into the main buffer here, so we don't have to flush the cache
		Point_Vertex_color* my_vertices = sceGuGetMemory((5) * sizeof(Point_Vertex_color));
		// create a lineloop
		my_vertices[0].x = (rect_table[(rect_type)].p1x);									my_vertices[0].y = (rect_table[(rect_type)].p1y);
		my_vertices[1].x = (rect_table[(rect_type)].p1x)+(rect_table[(rect_type)].p2w); 	my_vertices[1].y = (rect_table[(rect_type)].p1y);
		my_vertices[2].x = (rect_table[(rect_type)].p1x)+(rect_table[(rect_type)].p2w); 	my_vertices[2].y = (rect_table[(rect_type)].p1y)+(rect_table[(rect_type)].p2h);
		my_vertices[3].x = (rect_table[(rect_type)].p1x);									my_vertices[3].y = (rect_table[(rect_type)].p1y)+(rect_table[(rect_type)].p2h);
		my_vertices[4].x = (rect_table[(rect_type)].p1x);									my_vertices[4].y = (rect_table[(rect_type)].p1y);
//
		my_vertices[0].color = (rect_table[(rect_type)].c1);
		my_vertices[1].color = (rect_table[(rect_type)].c1);
		my_vertices[2].color = (rect_table[(rect_type)].c2);
		my_vertices[3].color = (rect_table[(rect_type)].c2);
		my_vertices[4].color = (rect_table[(rect_type)].c1);
	//
	//	#define abgr222 0xffffffff
	//	#define abgr222 0x7f0f0f0f
	//	#define abgr222 0x7f7f7f7f
	//	sceGuColor(/*0xaaaa*/ ((abgr222&0x7f7f7f7f)>>1) /*abgr*/);
	//	sceGuColor(abgr222/*0xffff*/ /*abgr*/);
		sceGuColor( (0x7f0f0f0f) /*abgr*/);
		sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, SERIFU_WINDOW_TEXTURE_FLAGS, (3), NULL, my_vertices);
		sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, SERIFU_WINDOW_TEXTURE_FLAGS, (3), NULL, &my_vertices[2]);
	//
	//	sceGuColor(abgr222/*0xffff*/ /*abgr*/);
	//	sceGuColor(/*0xaaaa*/ ((abgr222&0x7f7f7f7f)>>1) /*abgr*/);
		sceGuColor( (0x3f878787) /*abgr*/);
		sceGuDrawArray(GU_LINE_STRIP, SERIFU_WINDOW_TEXTURE_FLAGS, (5), NULL, my_vertices);
	}
	/* テクスチャー有効モードでは、線や面等のフラットポリゴンが使えない */
	PGC_SEND(PGC_TEXTURE_2D, PGC_ON);	//sceGuEnable(GU_TEXTURE_2D);	/* テクスチャー有効モードに戻す */
}

