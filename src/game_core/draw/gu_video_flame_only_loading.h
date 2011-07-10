
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	画面描画(起動時専用)
---------------------------------------------------------*/
#if 1//仕様変更でダメ
void draw_loading_screen_test(void)
{
	#if 1//(1==US E_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu時間を最大に取るタイプ](色々制限あり) */
//	sceGuFinish();実験
	/* - パケット描画終了待ち */
	sceGuSync(0, 0);/* (ここまで Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* ここから Gu */
	/* --- スクリーンアップデート */
	sceGuStart(GU_DIRECT, gulist );

	/* --- 画面クリア */
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- 画面更新 */

//	sceDisplayWaitVblankStart();/*vsync*/
//	PSPL_UpperBlit(sdl_screen[SDL_00_VIEW_SCREEN], NULL, SDL_VRAM_SCREEN, NULL);
//	{
//	//	memcpy( (u16 *)((u32) 0x44000000), render_image, (512*256) );/*test*/
//	//	memcpy( (u16 *)((u32) 0x44000000), render_image, (512*272*2) );
//	}
//	SD L_Flip(SDL_VRAM_SCREEN);

	//#if (0==USE_MAX_TRANS)
	sceKernelDcacheWritebackAll();
	//#endif // (0==USE_MAX_TRANS)
//
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
//
	/* -- BG背景 画面を描画 */
//	if (1==dr aw_bg_screen) /* pspは0レジスタがあるので0と比較したほうが速い */
//	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
//	{
//		/* -- BG 画面を描画 */
//		gu_blit_render_screen_01();/* 仕様変更 */
//	}
//
/* -- プライオリティー＃15．SDL 画面を描画 */
	#if (1)
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
		#if (0)
		/*ちょっとゲーム中SDL抜いてみるテスト*/
		if (NULL == callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
		#endif /*(000)*/
	{
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* 半透明 */
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdou ble buffer???*/
	//
	//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_SUBTRACT, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_SRC_COLOR, GU_DST_COLOR, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f007f7f, 0x3f3f3f00);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0, 0);
	//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
	//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
	//
		sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
		sceGuTexImage(0, 512, 512, 512, render_image);
		PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*くっきり拡大画面*/
		gu_draw_sdl_screen();
		PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*ぼやぼや拡大画面*/
	}
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
	#endif /*(000)*/
//


	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
//
	/* -- プライオリティー＃25．開発デバッグフォント */
	//TDebugDisp();

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - パケット積み、ここまで */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [通常タイプ] */
	sceGuSync(0, 0);/* (ここまで Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*(vsync)*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
//
	#endif
//
	//fps_newframe();
}
#endif
