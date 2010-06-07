
/*---------------------------------------------------------
	東方模倣風	〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	画面描画(汎用)
---------------------------------------------------------*/

extern	void do_input_vbl(void);
//extern	void gu_draw_score_chache(void);
void vbl_draw_screen(void)
{
	/*r31*/do_input_vbl();/*キー入力(処理の都合上、1[frame]に呼ぶのは1回のみ。)*/
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
//	/*r99*/do_input_vbl();/*キー入力(将来的にはv_syncとる予定)*/
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* ここから Gu */
	/* --- スクリーンアップデート */
	sceGuStart(GU_DIRECT, gulist );

	/* --- プライオリティー＃１．画面クリア */
	#if 1
	{	static u32 gu_bg_alt_clear_color;
		if(gu_bg_set_clear_color!=gu_bg_alt_clear_color)
		{
			gu_bg_alt_clear_color = gu_bg_set_clear_color;
			sceGuClearColor(gu_bg_set_clear_color);/*AABBGGRR*/
		//	sceGuClearColor(/*0*/0xff601010/*0xff601010*/);/*AABBGGRR*/
		}
	}
	#endif
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
//	SDL_BlitSurface(sdl_screen[SDL_00_VIEW_SCREEN], NULL, SDL_VRAM_SCREEN, NULL);
//	{
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*256) );/*test*/
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*272*2) );
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
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG 画面を描画 */
		/* -- プライオリティー＃２〜＃５ 画面を描画 */
		gu_blit_haikei_maho_jiki();
	}
//
/* -- プライオリティー＃６ラインテスト画面を描画 */
	#if (1==LINE_TEST)
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- ラインテスト画面を描画 */
		gu_blit_lines();
	}
	#endif /*(000)*/
	#if 000
//	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
//	[ 旧SDL画面描画 ]
//	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
	#endif
//
/* -- プライオリティー＃８．敵を描画 */
	/* ☆ 敵面エフェクト */
	/* ザコ(敵面エフェクト) */
	/* ボス(敵面エフェクト) */
	#if (1)
	{
		gu_set_texture(TEX_04_TEKI);	/* テクスチャをセット */
		#if (1)
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(sprite_list000_head, TEX_04_TEKI, S P_GROUP_BULLETS, obj_status_table+0);
			#if 0000/* 別で設定 */
			my_resource[TEX_04_TEKI].object_table_head = (obj_status_table+(OBJ_BANK_11_ZAKO_STAGE1*OBJ_BANK_SIZE));
			#endif
			common_transfer_objects(sprite_list000_head, TEX_04_TEKI, (SP_GROUP_TEKI/*|SP_GROUP_BOSS*/) );
		}
		s_blit_all_objects();/*PRIORITY_05_BULLETS*/
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- プライオリティー＃９．ITEMを描画 */
//	/* その他の自機素材(アイテム面エフェクト) */
	#if (1)
	{
		gu_set_texture(TEX_06_BULLET);	/* テクスチャをセット */
		#if (1)
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(sprite_list444_head, TEX_06_BULLET, S P_GROUP_BULLETS, obj_status_table+0);
			my_resource[TEX_06_BULLET].object_table_head = (obj_status_table+(OBJ_BANK_01_ITEM*OBJ_BANK_SIZE));
			common_transfer_444objects(/*sprite_list444_head,*/ TEX_06_BULLET, SP_GROUP_ITEMS );
		}
		s_blit_all_objects();/*PRIORITY_05_BULLETS*/
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- プライオリティー＃１０．敵弾を描画 */
	/* ☆ 弾幕面エフェクト */
	#if (1)
	/*
		VRAMが使えないと、タイミングの関係でめんどくさい？
	*/
	/* 0==リリース, 1==開発中 */
//	#define KAIHATSU_MODE (0)
//	#if (0==KAIHATSU_MODE)
//	if (shooting_game_core_work == main_call_func)/* game_coreの場合のみ弾幕描画(continue画面とかでは描かない) */
//	#endif
	{
	//	gu_set_texture(TEX_06_BULLET);	/* テクスチャをセット */
		#if (1)
		/* 弾幕リストは分割する予定 なので専用に書き換える予定 */
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(sprite_list444_head, TEX_06_BULLET, S P_GROUP_BULLETS, obj_status_table+0);
			my_resource[TEX_06_BULLET].object_table_head = (obj_status_table+(OBJ_BANK_00_TAMA*OBJ_BANK_SIZE));
			common_transfer_444objects(/*sprite_list444_head,*/ TEX_06_BULLET, SP_GROUP_BULLETS );
		}
		s_blit_all_objects();/*PRIORITY_05_BULLETS*/
		#else
		blit_bullet_all();
		#endif /*(000)*/
	}
	#endif /*(000)*/
//
/* -- プライオリティー＃７．SDL 画面を描画 */
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
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*くっきり拡大画面*/
		gu_draw_sdl_screen();
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*ぼやぼや拡大画面*/
	}
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* 描画範囲を設定する */
	#endif /*(000)*/
//


/* -- プライオリティー＃１１．フロント面を描画 */
	/* ☆ フロント面エフェクト */
//	/* 自分のあたり判定位置表示用コア(フロント面エフェクト) */
//	/* 小爆発 / ザコ消滅爆発(フロント面エフェクト) */
//	/* 火炎爆発(フロント面エフェクト) */
	/* ボンバーロゴ(フロント面エフェクト) */
	/* /[コンティニュー文字(bank00)/メニュー文字(bank01)/メニュー文字(bank02)] */
	#if (1)
	{
		gu_set_texture(TEX_07_FRONT);	/* テクスチャをセット */
		/*front_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(sprite_list000_head, TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+0);
			my_resource[TEX_07_FRONT].object_table_head = (obj_status_table+(OBJ_BANK_02_FRONT_BANK0*OBJ_BANK_SIZE));
			common_transfer_objects(sprite_list000_head, TEX_07_FRONT, SP_GROUP_FRONT );
		}
		/* FRONT 面を間借りして、スコア表示 */
		/*
			game_coreの場合のみ 描画 /
			(continue画面とかでは描かない) /
			現在作成中でスペカ時間経過が中にあるので対策
		*/
		if (shooting_game_core_work == main_call_func)/* game_coreの場合のみ 描画 */
		{
			gu_draw_score_chache();/* / 現在作成中でスペカ時間経過が中にあるので対策 */
		}
		s_blit_all_objects();/*PRIORITY_06_FRONT*/
	}



	/* プライオリティー＃１２．☆ フロント面エフェクト(テクスチャ共用) */
	//{
	if (0 != pd_bomber_time)
	{
		gu_draw_front_spell_logo();
	}
	//}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* 描画範囲を設定する */
//
/* -- プライオリティー＃１３．パネル面を描画 */
	#if (1)
	if (0 != draw_side_panel)
	{
//		gu_set_texture(TEX_07_FRONT);	/* テクスチャをセット */
		gu_draw_side_panel();/* とりあえずボスゲージ描画 */
	}
	#endif /*(000)*/
//
	/* -- プライオリティー＃１４．スクリプト画面を描画 */
//	xxxif (1==dr aw_script_screen)		/* pspは0レジスタがあるので0と比較したほうが速い */
	{
		#define FLAG_DRAW_SERIFU (0x01)/* せりふ文字の描画指示 */
		#define FLAG_DRAW_SUPEKA (0x02)/* スペカ文字の描画指示 */
		int 	use_draw_FONT;/* 文字の描画指示 */
		use_draw_FONT = 0;/* 描画指示をクリア */
		if (0 != (draw_script_screen+msg_time))
		{
			if (0 != (msg_time))/* メッセージ(仮対応)表示時間 */
			{
				msg_time--;
			}
			gu_draw_script_window();
			use_draw_FONT |= FLAG_DRAW_SERIFU;/* せりふ文字を描画指示 */
		}
		if (0!=draw_boss_hp_value)
		{
			use_draw_FONT |= FLAG_DRAW_SUPEKA;/* スペカ文字を描画指示 */
		}
		if (0!=use_draw_FONT)/* 描画指示があれば描画 */
		{
			gu_draw_font_texture_transfer();
			if (use_draw_FONT&FLAG_DRAW_SERIFU) 	{	gu_draw_serifu_screen();	}
			if (use_draw_FONT&FLAG_DRAW_SUPEKA) 	{	gu_draw_supeka_screen();	}
		}
	}
	/* -- プライオリティー＃１５．開発デバッグフォント */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - パケット積み、ここまで */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [通常タイプ]  */
	sceGuSync(0, 0);/* (ここまで Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	/*r99*/do_input_vbl();/*キー入力(将来的にはv_syncとる予定)*/
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
//
	#endif
//
	//fps_newframe();

	#if (1==USE_DESIGN_TRACK)
	/* 予約 voice を再生 */
	voice_play_vbl();
	#endif
}

