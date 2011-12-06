
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/*--------------------------------------------------------
	�X�N���[�����t���b�v����B
--------------------------------------------------------*/

//global void psp_video_flip_screen(int vsync)
#if 0
	#define PSP_VIDEO_FLIP_SCREEN \
	{ \
	/*	if (vsync)*/\
	/*	{*/\
	/*		sceDisplayWaitVblankStart();*/\
	/*	}*/\
		cb.show_frame = cb.draw_frame;\
		cb.draw_frame = sceGuSwapBuffers();\
	}
#else
	#define PSP_VIDEO_FLIP_SCREEN sceGuSwapBuffers();
#endif

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
	sceGuSync(0, GU_SYNC_FINISH);/* (�����܂� Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	/*r99*/do_input_vbl();/*�L�[����(�����I�ɂ�v_sync�Ƃ�\��)*/
	PSP_VIDEO_FLIP_SCREEN;
	#endif /* (USE_MAX_GU_TIME) */
	/* �������� Gu */
	/* --- �X�N���[���A�b�v�f�[�g */
	sceGuStart(GU_DIRECT, gulist);
// ��������ԂƓ����B	sceGuDrawBufferList(SDL_GU_PSM_0000/*GU_PSM_5551*/ /*4444*/, cb.draw_frame/*dst*/, PSP_BUFFER_WIDTH512);

	/* --- �v���C�I���e�B�[���P�D��ʃN���A */
	#if 1
	{	static u32 gu_bg_alt_clear_color;/*(�ȑO�̔w�i�F���o���Ă���)*/
		if (gu_bg_alt_clear_color!=gu_bg_set_clear_color)/*(�w�i�F���ύX���ꂽ�ꍇ�ɁA�w�i�F��ύX����B)*/
		{
			gu_bg_alt_clear_color = gu_bg_set_clear_color;/*(�ȑO�̔w�i�F���o���Ă���)*/
			/*(�w�i�F��ύX����B)*/
			sceGuClearColor(gu_bg_set_clear_color);/*AABBGGRR*/
		//	sceGuClearColor(/*0*/0xff601010/*0xff601010*/);/*AABBGGRR*/
		}
	}
	#endif
	/*(��ʂ�S�Ĕw�i�F�œh��Ԃ��`�悷��B)*/
	{	/* �����̃X�R�[�v�͗v��Ȃ��񂾂��ǁA�����P�ʂ𖾊m�ɂ���ׁB */
		#ifndef GU_FAST_CLEAR_BIT
			/* �Â�PSPSDK�ł͖����̂Œ�`�B(64SLICE�ŃN���A) */
			#define GU_FAST_CLEAR_BIT	(16)
		#endif
		#if (1==USE_ZBUFFER)/*(Z�o�b�t�@���g���ꍇ)*/
		sceGuClearDepth(0);// Z_BUFFER
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT|GU_FAST_CLEAR_BIT);/*(�w�i�F�œh��Ԃ��`��B�y�� Z�o�b�t�@��S�ď����B)*/
		sceGuEnable(GU_DEPTH_TEST);
		sceGuDepthMask(GU_FALSE);
		#else // (0==USE_ZBUFFER)/*(Z�o�b�t�@���g��Ȃ��ꍇ)*/
		sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/|GU_FAST_CLEAR_BIT);/*(�w�i�F�œh��Ԃ��`��B�y�� Z�o�b�t�@�͉������Ȃ��B)*/
		#endif/* (1==USE_ZBUFFER) */
	}
	/* -- ��ʍX�V */

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
	#if (0)/*(�f�o�b�O)*/
	sceGuScissor(0/*GAME_X_OFFSET*/, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	#else/*(r35-)*/
	sceGuScissor(GAME_X_OFFSET, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	#endif
//
	/* -- BG�w�i ��ʂ�`�� */
//	if (1==dr aw_bg_screen) /* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	if (NULL != cb.callback_gu_draw_haikei) 	//if (0!=dr aw_bg_screen)
	{
		/* -- BG ��ʂ�`�� */
		/* -- �v���C�I���e�B�[���Q�`��10 ��ʂ�`�� */
		gu_blit_render_screen_01();
		/*
			�v���C�I���e�B�[���Q�DBG ��ʂ�`��
			�v���C�I���e�B�[���R�D�w�i��Q����`��
			�v���C�I���e�B�[���S�D�����w��`��
			�v���C�I���e�B�[���T�D�|���S�����C���ʂ�`��(���쐬)
			�v���C�I���e�B�[���U�D���@��`��(A)
			�v���C�I���e�B�[���U�D���@��`��(B)
			�v���C�I���e�B�[���V�D���C���e�X�g��ʂ�`��
			�v���C�I���e�B�[���W�D�G��`��
			�v���C�I���e�B�[���X�DITEM��`��
			�v���C�I���e�B�[��10�D�G�e��`��
			�v���C�I���e�B�[��11�D(��)�p�l���x�[�X��`��
		*/
	}
//

/* -- �v���C�I���e�B�[��15�DSDL ��ʂ�`�� */
	#if (1)
	sceGuScissor(/*GAME_X_OFFSET*/0/*0*/, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
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
		PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����*/
		gu_draw_sdl_screen();
		PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����*/
	}
	if (0!=(cg.side_panel_draw_flag))
	{	/* �X�R�A�p�l�� �`�掞�̏ꍇ(�Q�[�����̗����G�ړ�) */
		sceGuScissor(GAME_X_OFFSET, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	}
//	else/*(�t�����=="Story" ���[�h��p�̗����G�ړ�)*/
//	{	/* �X�R�A�p�l�� ��`�掞�̏ꍇ */
//		sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */	/*(����)*/
//	}
	#endif /*(000)*/
//
/* -- �v���C�I���e�B�[��??�DSDL_tache ��ʂ�`�� */
	#if (1)
	//sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
	{
		int nnn;
		for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
		{
		//	if (NULL != kaiwa_sprite[nnn].img)	/* �摜��������̂̂ݕ`�悷��B(�g�p���̏ꍇ�ɕ`�悷��) */
			if ((0) != kaiwa_sprite[nnn].draw_flag) 	/* �`��w�肪������̂̂ݕ`�悷��B(�g�p���̏ꍇ�ɕ`�悷��) */
			{
				/*(Gu�̏ꍇ )*/
				gu_set_texture(TEX_09_TACHIE_L+nnn);
				PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����*/
				gu_draw_tache_screen(
					/*dst_r.x =*/ (t256_floor(kaiwa_sprite[nnn].center.x256)),
					/*dst_r.y =*/ (t256_floor(kaiwa_sprite[nnn].center.y256)),
				//	/*color32*/ 0x88ffffff
					/*color32*/ kaiwa_sprite[nnn].color32,
					/*(�����G�摜�̕�)*/(kaiwa_sprite[nnn].width_2n)
				);
				PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����*/
			}
		}
	}
	//sceGuScissor(GAME_X_OFFSET, 0, GAME_WIDTH, GAME_HEIGHT);	/* �`��͈͂�ݒ肷�� */
	#endif /* (1) */
//


/* -- �v���C�I���e�B�[��20�D�t�����g�ʂ�`�� */
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
#if 1/*�V�X�e���̃t�����g�ʕ`��(�G���Ƃ��ēo�^��������)*/
		{
		//	common_transfer_objects(sprite_list000_head, TEX_07_FRONT, OBJ_Z05_FRONT, obj_status_table+0);
			my_resource[TEX_07_FRONT].object_table_head = (obj_status_table+(OBJ_BANK_02_FRONT_BANK0*OBJ_BANK_SIZE));
#if 1/*�V�X�e���̃t�����g�ʕ`��(�G���Ƃ��ēo�^��������)*/
		//	common_transfer_objects_blit(OBJECT_TYPE_01_TEKI, /*sprite_list000_head,*/ TEX_07_FRONT, OBJ_Z05_FRONT );
			/* ���@�̒��ڕ`��(�t�����g��) */
			common_transfer_objects_blit(OBJECT_TYPE_02_KOTEI, /*sprite_list000_head,*/ TEX_07_FRONT, OBJ_Z05_FRONT );
#endif
		}
#endif
	//	s_blit_all_objects();/*PRIORITY_06_FRONT*/
		/* �v���C�I���e�B�[��21�DFRONT �ʂ��Ԏ؂肵�āA�X�R�A�\�� */
		/*
			game_core�̏ꍇ�̂� �`�� /
			(continue��ʂƂ��ł͕`���Ȃ�) /
			���ݍ쐬���ŃJ�[�h���Ԍo�߂����ɂ���̂ő΍�
		*/
		if (shooting_game_core_work == cb.main_call_func)/* game_core�̏ꍇ�̂� �`�� */
		{
			gu_draw_score_chache();/* / ���ݍ쐬���ŃJ�[�h���Ԍo�߂����ɂ���̂ő΍� */
		}
	//	s_blit_all_objects();/*PRIORITY_06_FRONT*/
	}

	/* �v���C�I���e�B�[��22�D�� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p) */
	//{
	if (0 != (cg.bomber_time))
	{
		gu_draw_front_spell_logo();
	}
	//}
	#endif /*(000)*/
//
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);	/* �`��͈͂�ݒ肷�� */
//
/* -- �v���C�I���e�B�[��23�D�p�l���ʂ�`�� */
	#if (1)
	if (0 != (cg.side_panel_draw_flag))
	{
//		gu_set_texture(TEX_07_FRONT);	/* �e�N�X�`�����Z�b�g */
		gu_draw_side_panel();/* �Ƃ肠�����{�X�Q�[�W�`�� */
	}
	#endif /*(000)*/
//
	/* -- �v���C�I���e�B�[��24�D����Ӄ��b�Z�[�W��ʂ�`�� */
//	xxxif (1==dr aw_scr ipt_screen)		/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
	{
	//	if (0 != (cg.msg_time))/* ���b�Z�[�W(���Ή�)�\������ */
		if (0 < (cg.msg_time))/* ���b�Z�[�W(���Ή�)�\������ */
		{
			cg.msg_time--;/*(�S�̂̃��b�Z�[�W�\�����ԁA���b�Z�[�W�\�����ԏ���B)*/
			// �X�̃��b�Z�[�W�\�����Ԃ͓Ɨ����ĕʂɂ���B
			gu_draw_serifu_haikei();
			gu_draw_font_texture_transfer();
			gu_draw_text_lines();
		}
	}
	/* -- �v���C�I���e�B�[��25�D�J���f�o�b�O�t�H���g */
	//TDebugDisp();

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	/* - �p�P�b�g�ς݁A�����܂� */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [�ʏ�^�C�v] */
	sceGuSync(0, GU_SYNC_FINISH);/* (�����܂� Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*(vsync)*/
	#endif /* (1==USE_VSYNC) */
//	/*r99*/do_input_vbl();/*�L�[����(�����I�ɂ�v_sync�Ƃ�\��)*/
	PSP_VIDEO_FLIP_SCREEN;
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
