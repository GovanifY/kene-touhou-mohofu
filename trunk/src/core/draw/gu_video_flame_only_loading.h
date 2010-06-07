
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	��ʕ`��(�N������p)
---------------------------------------------------------*/

void draw_loading_screen_test(void)
{
	#if 1//(1==US E_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu���Ԃ��ő�Ɏ��^�C�v](�F�X��������) */
//	sceGuFinish();����
	/* - �p�P�b�g�`��I���҂� */
	sceGuSync(0, 0);/* (�����܂� Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* �������� Gu */
	/* --- �X�N���[���A�b�v�f�[�g */
	sceGuStart(GU_DIRECT, gulist );

	/* --- ��ʃN���A */
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- ��ʍX�V */

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
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
//
	/* -- BG�w�i ��ʂ�`�� */
//	if (1==dr aw_bg_screen) /* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- BG ��ʂ�`�� */
		gu_blit_haikei_maho_jiki();
	}
//

	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
	/* -- �J���f�o�b�O�t�H���g */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - �p�P�b�g�ς݁A�����܂� */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [�ʏ�^�C�v]  */
	sceGuSync(0, 0);/* (�����܂� Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
//
	#endif
//
	//fps_newframe();
}
