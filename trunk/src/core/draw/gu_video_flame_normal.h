
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	��ʕ`��(�ėp)
---------------------------------------------------------*/

extern	void do_input_vbl(void);
//extern	void gu_draw_score_chache(void);
void vbl_draw_screen(void)
{
	/*r31*/do_input_vbl();/*�L�[����(�����̓s����A1[frame]�ɌĂԂ̂�1��̂݁B)*/
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
//	/*r99*/do_input_vbl();/*�L�[����(�����I�ɂ�v_sync�Ƃ�\��)*/
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
	/* �������� Gu */
	/* --- �X�N���[���A�b�v�f�[�g */
	sceGuStart(GU_DIRECT, gulist );

	/* --- �v���C�I���e�B�[���P�D��ʃN���A */
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
		/* -- �v���C�I���e�B�[���Q�`���T ��ʂ�`�� */
		gu_blit_haikei_maho_jiki();
	}
//
/* -- �v���C�I���e�B�[���U���C���e�X�g��ʂ�`�� */
	#if (1==LINE_TEST)
	if (NULL != callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
	{
		/* -- ���C���e�X�g��ʂ�`�� */
		gu_blit_lines();
	}
	#endif /*(000)*/
	#if 000
//	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//	[ ��SDL��ʕ`�� ]
//	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	#endif
//
/* -- �v���C�I���e�B�[���W�D�G��`�� */
	/* �� �G�ʃG�t�F�N�g */
	/* �U�R(�G�ʃG�t�F�N�g) */
	/* �{�X(�G�ʃG�t�F�N�g) */
	#if (1)
	{
		gu_set_texture(TEX_04_TEKI);	/* �e�N�X�`�����Z�b�g */
		#if (1)
		/*bullet_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(sprite_list000_head, TEX_04_TEKI, S P_GROUP_BULLETS, obj_status_table+0);
			#if 0000/* �ʂŐݒ� */
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
/* -- �v���C�I���e�B�[���X�DITEM��`�� */
//	/* ���̑��̎��@�f��(�A�C�e���ʃG�t�F�N�g) */
	#if (1)
	{
		gu_set_texture(TEX_06_BULLET);	/* �e�N�X�`�����Z�b�g */
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
/* -- �v���C�I���e�B�[���P�O�D�G�e��`�� */
	/* �� �e���ʃG�t�F�N�g */
	#if (1)
	/*
		VRAM���g���Ȃ��ƁA�^�C�~���O�̊֌W�ł߂�ǂ������H
	*/
	/* 0==�����[�X, 1==�J���� */
//	#define KAIHATSU_MODE (0)
//	#if (0==KAIHATSU_MODE)
//	if (shooting_game_core_work == main_call_func)/* game_core�̏ꍇ�̂ݒe���`��(continue��ʂƂ��ł͕`���Ȃ�) */
//	#endif
	{
	//	gu_set_texture(TEX_06_BULLET);	/* �e�N�X�`�����Z�b�g */
		#if (1)
		/* �e�����X�g�͕�������\�� �Ȃ̂Ő�p�ɏ���������\�� */
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
/* -- �v���C�I���e�B�[���V�DSDL ��ʂ�`�� */
	#if (1)
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
		#if (0)
		/*������ƃQ�[����SDL�����Ă݂�e�X�g*/
		if (NULL == callback_gu_draw_haikei)	//if (0!=dr aw_bg_screen)
		#endif /*(000)*/
	{
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/* ������ */
	//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*�����dou ble buffer???*/
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
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����*/
		gu_draw_sdl_screen();
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����*/
	}
	sceGuScissor(0, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	#endif /*(000)*/
//


/* -- �v���C�I���e�B�[���P�P�D�t�����g�ʂ�`�� */
	/* �� �t�����g�ʃG�t�F�N�g */
//	/* �����̂����蔻��ʒu�\���p�R�A(�t�����g�ʃG�t�F�N�g) */
//	/* ������ / �U�R���Ŕ���(�t�����g�ʃG�t�F�N�g) */
//	/* �Ή�����(�t�����g�ʃG�t�F�N�g) */
	/* �{���o�[���S(�t�����g�ʃG�t�F�N�g) */
	/* /[�R���e�B�j���[����(bank00)/���j���[����(bank01)/���j���[����(bank02)] */
	#if (1)
	{
		gu_set_texture(TEX_07_FRONT);	/* �e�N�X�`�����Z�b�g */
		/*front_transfer_object();*/
		{
			TGameScreen_ClearSprite();
		//	common_transfer_objects(sprite_list000_head, TEX_07_FRONT, SP_GROUP_FRONT, obj_status_table+0);
			my_resource[TEX_07_FRONT].object_table_head = (obj_status_table+(OBJ_BANK_02_FRONT_BANK0*OBJ_BANK_SIZE));
			common_transfer_objects(sprite_list000_head, TEX_07_FRONT, SP_GROUP_FRONT );
		}
		/* FRONT �ʂ��Ԏ؂肵�āA�X�R�A�\�� */
		/*
			game_core�̏ꍇ�̂� �`�� /
			(continue��ʂƂ��ł͕`���Ȃ�) /
			���ݍ쐬���ŃX�y�J���Ԍo�߂����ɂ���̂ő΍�
		*/
		if (shooting_game_core_work == main_call_func)/* game_core�̏ꍇ�̂� �`�� */
		{
			gu_draw_score_chache();/* / ���ݍ쐬���ŃX�y�J���Ԍo�߂����ɂ���̂ő΍� */
		}
		s_blit_all_objects();/*PRIORITY_06_FRONT*/
	}



	/* �v���C�I���e�B�[���P�Q�D�� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p) */
	//{
	if (0 != pd_bomber_time)
	{
		gu_draw_front_spell_logo();
	}
	//}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
/* -- �v���C�I���e�B�[���P�R�D�p�l���ʂ�`�� */
	#if (1)
	if (0 != draw_side_panel)
	{
//		gu_set_texture(TEX_07_FRONT);	/* �e�N�X�`�����Z�b�g */
		gu_draw_side_panel();/* �Ƃ肠�����{�X�Q�[�W�`�� */
	}
	#endif /*(000)*/
//
	/* -- �v���C�I���e�B�[���P�S�D�X�N���v�g��ʂ�`�� */
//	xxxif (1==dr aw_script_screen)		/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	{
		#define FLAG_DRAW_SERIFU (0x01)/* ����ӕ����̕`��w�� */
		#define FLAG_DRAW_SUPEKA (0x02)/* �X�y�J�����̕`��w�� */
		int 	use_draw_FONT;/* �����̕`��w�� */
		use_draw_FONT = 0;/* �`��w�����N���A */
		if (0 != (draw_script_screen+msg_time))
		{
			if (0 != (msg_time))/* ���b�Z�[�W(���Ή�)�\������ */
			{
				msg_time--;
			}
			gu_draw_script_window();
			use_draw_FONT |= FLAG_DRAW_SERIFU;/* ����ӕ�����`��w�� */
		}
		if (0!=draw_boss_hp_value)
		{
			use_draw_FONT |= FLAG_DRAW_SUPEKA;/* �X�y�J������`��w�� */
		}
		if (0!=use_draw_FONT)/* �`��w��������Ε`�� */
		{
			gu_draw_font_texture_transfer();
			if (use_draw_FONT&FLAG_DRAW_SERIFU) 	{	gu_draw_serifu_screen();	}
			if (use_draw_FONT&FLAG_DRAW_SUPEKA) 	{	gu_draw_supeka_screen();	}
		}
	}
	/* -- �v���C�I���e�B�[���P�T�D�J���f�o�b�O�t�H���g */
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
//	/*r99*/do_input_vbl();/*�L�[����(�����I�ɂ�v_sync�Ƃ�\��)*/
	sceGuSwapBuffers();
	#endif /* (USE_MAX_GU_TIME) */
//
	#endif
//
	//fps_newframe();

	#if (1==USE_DESIGN_TRACK)
	/* �\�� voice ���Đ� */
	voice_play_vbl();
	#endif
}
