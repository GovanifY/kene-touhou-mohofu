
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�X�R�A�p�l���ɂȂ�\��̕���(����SDL)
	-------------------------------------------------------

	�{�X�̗͕`��̂�(����Gu)
---------------------------------------------------------*/

static void gu_draw_side_panel(void)
{
	#if 0
	/* �X�R�A�p�l���ɂȂ�\��̕���(����SDL) */

	#endif
//
	/* �{�X�Q�[�W�`�� */
	if (0!=cg.draw_boss_hp_value)
	{
	//	#define cg.draw_boss_hp_value			(340)
		#define SIDE_WINDOW_Y_SIZE_070		(4)/*70*/
		/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-cg.draw_boss_hp_value)/2) == SIDE_WINDOW_X_POSITION_020 */
		#define SIDE_WINDOW_X_POSITION_020	(64)/* (50)HPGAUGE_X_OFS+(10)drec.x */
		#define SIDE_WINDOW_Y_POSITION_182	(1+3)/*GAME_HEIGHT-SIDE_WINDOW_Y_SIZE_070-SIDE_WINDOW_X_POSITION_020*/

		/* -- ��`�� */
		/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
		/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
		PGC_SEND(PGC_TEXTURE_2D, PGC_OFF);	//sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
		{
			//#define TEXTURE_FLAGS1111 		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define SIDE_PANEL_TEXTURE_FLAGS	(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define SIDE_PANEL_TEXTURE_FLAGS	(				GU_COLOR_8888 | 	GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//GU_VERTEX_32BITF|GU_TRANSFORM_2D
			// we make local copies of the line into the main buffer here, so we don't have to flush the cache
			Point_Vertex_color* my_vertices = sceGuGetMemory((4/*5*/) * sizeof(Point_Vertex_color));

			// create a lineloop
			my_vertices[0].x = (SIDE_WINDOW_X_POSITION_020) 					/*x-width*/;	my_vertices[0].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[1].x = (SIDE_WINDOW_X_POSITION_020)+(cg.draw_boss_hp_value)/*x+width*/;	my_vertices[1].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[2].x = (SIDE_WINDOW_X_POSITION_020) 					/*x-width*/;	my_vertices[2].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[3].x = (SIDE_WINDOW_X_POSITION_020)+(cg.draw_boss_hp_value)/*x+width*/;	my_vertices[3].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
//
			my_vertices[0].color = (0x7fffffff);
			my_vertices[1].color = (0x7fffffff);
			my_vertices[2].color = (0x7f0f0f0f);
			my_vertices[3].color = (0x7f0f0f0f);
		//
		//	#define abgr111 0xffffffff
	//		#define abgr111 0x7f0f0f0f
		//	#define abgr111 0x7f7f7f7f
		//	sceGuColor(/*0xaaaa*/ ((abgr111&0x7f7f7f7f)>>1) /*abgr*/);
		//	sceGuColor(abgr111/*0xffff*/ /*abgr*/);
		//	sceGuColor(/*0xaaaa*/ ((abgr111&0x7f7f7f7f)>>2) /*abgr*/);
			sceGuColor(/*0xaaaa*/ ((0x7fffffff)) /*abgr*/);
			sceGuDrawArray(GU_TRIANGLE_STRIP/*GU_LINE_STRIP*/, SIDE_PANEL_TEXTURE_FLAGS, (4), NULL, my_vertices);
		//	sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, SIDE_PANEL_TEXTURE_FLAGS, (3), NULL, my_vertices);
		//	sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, SIDE_PANEL_TEXTURE_FLAGS, (3), NULL, &my_vertices[2]);
		//
		//	sceGuColor(abgr111/*0xffff*/ /*abgr*/);
		//	sceGuColor(/*0xaaaa*/ ((abgr111&0x7f7f7f7f)>>1) /*abgr*/);
		//	sceGuColor(/*0xaaaa*/ ((0x3f3f3f3f)) /*abgr*/);
		//	sceGuColor(abgr111/*0xffff*/ /*abgr*/);
		//	sceGuDrawArray(GU_LINE_STRIP, SIDE_PANEL_TEXTURE_FLAGS, (5), NULL, my_vertices);
		}
		/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
		PGC_SEND(PGC_TEXTURE_2D, PGC_ON);	//sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
//
	}
}
