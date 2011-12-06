
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
enum
{
	KAZU_HI_SCORE01 = 0,// �n�C�X�R�A10����
	KAZU_HI_SCORE02,// �n�C�X�R�A9����
	KAZU_HI_SCORE03,// �n�C�X�R�A8����
	KAZU_HI_SCORE04,// �n�C�X�R�A7����
//
	KAZU_HI_SCORE05,// �n�C�X�R�A6����
	KAZU_HI_SCORE06,// �n�C�X�R�A5����
	KAZU_HI_SCORE07,// �n�C�X�R�A4����
	KAZU_HI_SCORE08,// �n�C�X�R�A3����
//
	KAZU_HI_SCORE09,// �n�C�X�R�A2����
	KAZU_HI_SCORE10,// �n�C�X�R�A1����(�K��'0')
	KAZU_MY_SCORE01,// �X�R�A10����
	KAZU_MY_SCORE02,// �X�R�A9����
//
	KAZU_MY_SCORE03,// �X�R�A8����
	KAZU_MY_SCORE04,// �X�R�A7����
	KAZU_MY_SCORE05,// �X�R�A6����
	KAZU_MY_SCORE06,// �X�R�A5����
//
	KAZU_MY_SCORE07,// �X�R�A4����
	KAZU_MY_SCORE08,// �X�R�A3����
	KAZU_MY_SCORE09,// �X�R�A2����
	KAZU_MY_SCORE10,// �X�R�A1����(�K��'0')
//
	KAZU_GRAZE01,// �O���C�Y�X�R�A5����
	KAZU_GRAZE02,// �O���C�Y�X�R�A4����
	KAZU_GRAZE03,// �O���C�Y�X�R�A3����
	KAZU_GRAZE04,// �O���C�Y�X�R�A2����
//
	KAZU_GRAZE05,// �O���C�Y�X�R�A1����
	KAZU_dummy005,
	KAZU_dummy006,
	KAZU_dummy007,
//
	KAZU_dummy008,
	KAZU_dummy009,
	KAZU_dummy010,
	KAZU_dummy011,
//
	KAZU_32_MAX/*(�ő吔)*/
};
typedef struct
{
	s16 x00;
	s16 y00;
//	s16 suuji;
//	s16 dummy;
} KAZU;
static KAZU kazu_work[KAZU_32_MAX];

static u32 top_score;
global void set_topscore(void)
{
	static const KAZU kazu_const[KAZU_32_MAX] =
	{
		{ (380+5*8+4-(24/*ofs*/))+8*0,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE01 = 0,
		{ (380+5*8+4-(24/*ofs*/))+8*1,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE02,
		{ (380+5*8+4-(24/*ofs*/))+8*2,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE03,
		{ (380+5*8+4-(24/*ofs*/))+8*3,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE04,
		//      //
		{ (380+5*8+4-(24/*ofs*/))+8*4,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE05,
		{ (380+5*8+4-(24/*ofs*/))+8*5,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE06,
		{ (380+5*8+4-(24/*ofs*/))+8*6,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE07,
		{ (380+5*8+4-(24/*ofs*/))+8*7,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE08,
		//      //
		{ (380+5*8+4-(24/*ofs*/))+8*8,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE09,
		{ (380+5*8+4-(24/*ofs*/))+8*9,(3*8+2+(5/*ofs*/)),},//	KAZU_HI_SCORE10,
		{ (380+5*8+4-(24/*ofs*/))+8*0,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE01,
		{ (380+5*8+4-(24/*ofs*/))+8*1,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE02,
		//      //
		{ (380+5*8+4-(24/*ofs*/))+8*2,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE03,
		{ (380+5*8+4-(24/*ofs*/))+8*3,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE04,
		{ (380+5*8+4-(24/*ofs*/))+8*4,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE05,
		{ (380+5*8+4-(24/*ofs*/))+8*5,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE06,
		//      //
		{ (380+5*8+4-(24/*ofs*/))+8*6,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE07,
		{ (380+5*8+4-(24/*ofs*/))+8*7,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE08,
		{ (380+5*8+4-(24/*ofs*/))+8*8,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE09,
		{ (380+5*8+4-(24/*ofs*/))+8*9,(6*8+7+(5/*ofs*/)),},//	KAZU_MY_SCORE10,
		//      //
		{ (380+11*8+4-(24/*ofs*/))+8*0,(20*8+(5/*ofs*/)),},//	KAZU_GRAZE01,
		{ (380+11*8+4-(24/*ofs*/))+8*1,(20*8+(5/*ofs*/)),},//	KAZU_GRAZE02,
		{ (380+11*8+4-(24/*ofs*/))+8*2,(20*8+(5/*ofs*/)),},//	KAZU_GRAZE03,
		{ (380+11*8+4-(24/*ofs*/))+8*3,(20*8+(5/*ofs*/)),},//	KAZU_GRAZE04,
		//      //
		{ 0,0,},//	KAZU_dummy000,
		{ 0,0,},//	KAZU_dummy000,
		{ 0,0,},//	KAZU_dummy000,
		{ 0,0,},//	KAZU_dummy000,
		//      //
		{ 0,0,},//	KAZU_dummy000,
		{ 0,0,},//	KAZU_dummy000,
		{ 0,0,},//	KAZU_dummy000,
		{ 0,0,},//	KAZU_dummy000,
	};
	#if 1
	{
		int i;
		for (i=0; i<KAZU_32_MAX; i++)
		{
			kazu_work[i].x00=kazu_const[i].x00;
			kazu_work[i].y00=kazu_const[i].y00;
		//	kazu_work[i].suuji=kazu_const[i].suuji;
		//	kazu_work[i].dummy=kazu_const[i].dummy;
		}
	}
	#else
	memcpy(kazu_work, kazu_const, sizeof(kazu_const));
	#endif
	top_score			= high_score_table[(cg_game_select_player)][0].score;	// ��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
}
	//	u32 jjj;
	//	jjj = t256(380+5*8+4-(24/*ofs*/));
	//	jjj = t256(250+50);
//		jjj += t256(8);
//		obj->center.x256	= jjj;//((aaa->center.x256));	/*x 512/256 */
//top	obj->center.y256	= t256(3*8+2+(5/*ofs*/));//((aaa->center.y256));	// "high score"
//now	obj->center.y256	= t256(6*8+7+(5/*ofs*/));//((aaa->center.y256));	// "now score"


static void gu_draw_side_panel(void)
{
	#if (1)
	/* �X�R�A�p�l���ɂȂ�\��̕���(����SDL) */
	if (0!=(cg.side_panel_draw_flag))
	{
		PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����(�g�厞�o�Ȑ��⊮)*/
	//	PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����(�g�厞�����⊮)*/
		{
			VIRTUAL_OBJ_STATE *head_obj_status_table;
			head_obj_status_table = my_resource[TEX_07_FRONT/*num*/].object_table_head;
			//
			static	/*TGameSprite*/OBJ gu_bbb_sprite_pool[1];
			/*TGameSprite*/OBJ *obj;
			obj = &gu_bbb_sprite_pool[0];
			/* [ �n�C�X�R�A�\�� ] */
		//	if (top_score < cg.game_score)
		//	{	top_score = cg.game_score;}
			top_score = psp_max(top_score, cg.game_score);
		//	sp rintf(my_font_text,"%09d0", top_score);
			strcpy(my_font_text,
				"0000000000" 	// "high score"
				"0000000000"	// "now score"
				":::0"			// "graze score"
			);
			dec_print_format(top_score, 	9/*8*/, (char *)&my_font_text[KAZU_HI_SCORE01/*0*/]);   // "high score"
			dec_print_format(cg.game_score, 9/*8*/, (char *)&my_font_text[KAZU_MY_SCORE01/*10*/]);	// "now score"
			dec_print_format(cg.graze_point, 4, 	(char *)&my_font_text[KAZU_GRAZE01]);			// "graze score"
			/* FRONT �ʂ��Ԏ؂肵�āA�X�R�A�\�� */
		/* ������\�� */
			int i;
			for (i=0; i<(10+10+4); i++)
			{
				/* ���W��obj�̒��S���W�A */
				/* --- �I�u�W�F�N�g�\���ݒ� */
				obj->center.x256	= (kazu_work[i].x00<<8);//((aaa->center.x256)); 	/*x 512/256 */
				obj->center.y256	= (kazu_work[i].y00<<8);//((aaa->center.y256));
				obj->color32		= 0xffffffff;		/* ���l(0xff==255 �ŕs�����A0 �œ���) */
				{
			//	int tama_index = ((5*8)+4)+(9);//(aaa->number);
			//	int tama_index = ((5*8)+3)+(9)/*(aaa->number)*/;
//				int tama_index = ((5*8)+3)+(my_font_text[i]&0x0f)/*es_panel[i]*/ /*(aaa->number)*/;
				int tama_index = ((5*8)  )+(my_font_text[i]&0x0f)/*es_panel[i]*/ /*(aaa->number)*/;
				obj->w			= head_obj_status_table[tama_index].w;
				obj->h			= head_obj_status_table[tama_index].h;
				obj->tx 		= head_obj_status_table[tama_index].u;		//sss->texture_x;
				obj->ty 		= head_obj_status_table[tama_index].v;		//sss->texture_y;
				}
				#if (1==USE_ZBUFFER)
				obj->priority	= sss->m_priority;			/*aaa*/ 	/* �\���D�揇�� */
				#endif/* (1==USE_ZBUFFER) */
				{
					/* ��]�g��k���Ȃ��A�P����`�R�s�[ */
					render_object_no_rot_zoom(obj);
				}
			}
		}
	//	PGC_SEND(PGC_TEX_FILTER,PU8(GU_NEAREST,GU_NEAREST));	// sceGuTexFilter(GU_NEAREST, GU_NEAREST);/*��������g����(�g�厞�o�Ȑ��⊮)*/
		PGC_SEND(PGC_TEX_FILTER,PU8(GU_LINEAR,GU_LINEAR));		// sceGuTexFilter(GU_LINEAR, GU_LINEAR);/*�ڂ�ڂ�g����(�g�厞�����⊮)*/
	}

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
			my_vertices[1].x = (SIDE_WINDOW_X_POSITION_020)+(cg.draw_boss_hp_value)/*x+width*/; my_vertices[1].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[2].x = (SIDE_WINDOW_X_POSITION_020) 					/*x-width*/;	my_vertices[2].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[3].x = (SIDE_WINDOW_X_POSITION_020)+(cg.draw_boss_hp_value)/*x+width*/; my_vertices[3].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
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
		PGC_SEND(PGC_TEXTURE_2D, PGC_ON);	//sceGuEnable(GU_TEXTURE_2D);	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
//
	}
}
