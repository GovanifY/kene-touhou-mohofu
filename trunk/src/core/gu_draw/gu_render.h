/*---------------------------------------------------------
	�w�i(���쒆)
	-------------------------------------------------------
---------------------------------------------------------*/
extern /*static*/ int current_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A���ݑ��x */
/*	int val1 = 0;*/

static int val3 /*= 0*/;
static int val3_256 /*= 0*/;

/*---------------------------------------------------------
	�[��3D�w�i(���쒆)
	-------------------------------------------------------
	���x���Ԃɍ����̂Ȃ�AGum��3D�ɂ��������A���݉��f�ށB
	Gum�Ɉڍs���Ă����炩�̃T�|�[�g��Gu�͗v�邩���m��Ȃ��B(��ʂ̒[�����̕��Ƃ�)
	(3D�|���S���ł��ׂĖ��ߐs�����̂͑��x�I�ȃR�X�g�����������m��Ȃ��B����ĂȂ����画��܂���)
---------------------------------------------------------*/

		#if 1//(1==USE_F3D)
static void gu_draw_bg_fake3D(void)
{
//	#define GS_12V (5*2)
//	#define GS_12V (8*2)
	#define GS_12V (6*2)

//	#define GS_22V (GS_12V+(8*2))
//	#define GS_22V (GS_12V+(4*2))	/*dame*/
	#define GS_22V ((GS_12V)+(5*2))

	#define GS_33V ((GS_12V)+(GS_22V))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(((GS_12V)+(GS_22V))*sizeof(Vertex_uvcxyz));	/* �m�� */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);

//	u16 b00 =(conv_bg_alpha & 0xf0);
//	u16 b_1 = (b00&0x80)?(0xf0):(b00|0x80); 	u16 blendlevel_2 = ((b_1)|(b_1>>4));	blendlevel_2 = ((blendlevel_2)|(blendlevel_2<<8));
//	u16 b_2 = (b00);							u16 blendlevel_1 = ((b_2)|(b_2>>4));	blendlevel_1 = ((blendlevel_1)|(blendlevel_1<<8));
//	u16 b_3 = (b00&0x80)?(b00&0x7f):(0x00); 	u16 blendlevel_0 = ((b_3)|(b_3>>4));	blendlevel_0 = ((blendlevel_0)|(blendlevel_0<<8));

	const u16 u16_b_tbl[3][8] =
	{
		{	0xcccc,0xcccc,0xdddd,0xdddd,	0xeeee,0xeeee,0xffff,0xffff,	},
		{	0x8888,0x9999,0xaaaa,0xbbbb,	0xcccc,0xdddd,0xeeee,0xffff,	},
		{	0x8888,0x8888,0x8888,0x8888,	0x8888,0x8888,0x8888,0x8888,	},
	};
	u8 baa = ((conv_bg_alpha & 0xe0)>>5);
	int blendlevel_0;
	int blendlevel_1;
	int blendlevel_2;
	blendlevel_0 = u16_b_tbl[0][baa];
	blendlevel_1 = u16_b_tbl[1][baa];
	blendlevel_2 = u16_b_tbl[2][baa];


//(conv_bg_alpha)
		#else
	u32 blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define GS_Y000 (0)
#define GS_Y100 (100)
#define GS_Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	�㑤 */
		/* 176==(GAME_WIDTH/2) */
		x2 = ((GAME_WIDTH/2) - 176 -8) /*+ ((val1&127)-64)*/;
		x3 = ((GAME_WIDTH/2) - 256 -32 -64 +8+1);
		x4 = 180+1;
		for (j=0; j<(GS_22V); j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = GS_Y100;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel_1;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = GS_Y000;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel_2;
			#endif
			x2 +=  36;/*�㉏��*/
			x3 +=  72;/*������*/
			x4 +=  32/*64*/;/*�e�N�X�`��*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
/*	���� */
		x2 = ((GAME_WIDTH/2) -176) /*+ ((val1&127)-64)*/;/*(�Œ�[)*/
		x3 = ((GAME_WIDTH/2) -256 -32);
		x4 = 256-1;
		for (/*j=0*/; j<((GS_22V)+(GS_12V)); j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = GS_Y272;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel_0;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = GS_Y100;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel_1;
			#endif
			x2 +=  72;/*�㉏��*/
			x3 += 128;/*������*/
			x4 +=  32/*64*/;/*�e�N�X�`��*/
		}
//		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (GS_22V), NULL, vertices);
//		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (GS_12V), NULL, &vertices[GS_22V]);
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, ((GS_22V)+(GS_12V)), NULL, vertices);
}
		#endif
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (GS_12V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/

/*---------------------------------------------------------
	�i�����w�i(���쒆)
	-------------------------------------------------------
---------------------------------------------------------*/

		#if 1//(1==USE_F3D)
static void gu_draw_bg_eientei(void)
{
//	#define EH_24V (5*2)
//	#define EH_24V (8*2)
	#define EH_24V (12*2)
	#define EH_R08V (4*2)
	#define EH_L08V (4*2)
	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory((EH_24V+EH_R08V+EH_L08V)*sizeof(Vertex_uvcxyz));	/* GPU���Ԃɍ���Ȃ��̂ł����Ɗm�� */
	/* ??? */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define EH_Y000 (0)

		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
#define EH_yyy02 x2
#define EH_yyy03 x3

/* ======== �㑤�A�E�ӂ��� ================================================ */
		EH_yyy02	= ( 80+16+7-(4*2) );
		EH_yyy03	= (272+16+7-(4*23) );
		x4		= 256-1/*180+1*/;
		for (j=0; j<EH_R08V; j+=2)
		{
			vertices[j].x = x4/*+10*/ /*0*/ /*100*/ /*EH_yyy03*/;
			vertices[j].y = EH_yyy03/*EH_Y_LOCATE_199*/;
			vertices[j].z = 0;
			vertices[j].u = (0x7f)/*x4*/;
			vertices[j].v = x4+(val3)/*(0xff)+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x4/*0*/ /*EH_yyy02*/;
			vertices[j+1].y = EH_yyy02/*EH_Y000*/;
			vertices[j+1].z = 0;
			vertices[j+1].u = 0/*x4*/;
			vertices[j+1].v = x4+(val3)/*0+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			EH_yyy02 -=  2;/*�㉏��*/
			EH_yyy03 +=  (24-1)/*16*/;/*������*/
			x4 +=  32/*32*/ /*64*/;/*�e�N�X�`��*/
		}

/* ======== �㑤�A���ӂ��� ================================================ */
		EH_yyy02	= (80);
		EH_yyy03	= (272-1);
		x4		= 0/*180+1*/;
		for (/*j=0*/; j<(EH_R08V+EH_L08V); j+=2)	/* GPU�]�����Ԃɍ���Ȃ��̂ŁA�����Ɗm��(j�͓r������) */
		{
			vertices[j].x = x4/*+10*/ /*0*/ /*100*/ /*EH_yyy03*/;
			vertices[j].y = EH_yyy03/*EH_Y_LOCATE_199*/;
			vertices[j].z = 0;
			vertices[j].u = (0x7f)/*x4*/;
			vertices[j].v = x4+((-val3)&(0xff))/*(0xff)+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x4/*0*/ /*EH_yyy02*/;
			vertices[j+1].y = EH_yyy02/*EH_Y000*/;
			vertices[j+1].z = 0;
			vertices[j+1].u = 0/*x4*/;
			vertices[j+1].v = x4+((-val3)&(0xff))/*0+(val3)*/;
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			EH_yyy02 +=  2;/*�㉏��*/
			EH_yyy03 -=  24/*16*/;/*������*/
			x4 +=  32/*32*/ /*64*/;/*�e�N�X�`��*/
		}


/* ======== ������ ================================================ */
		x2 = ( 96) /*+ ((val1&127)-64)*/;/*(�Œ�[)*/
		x3 = (	0);
		x4 = /*0x7f*/0xbf/*128*/ /*256-1*/;
#define EH_Y_LOCATE_199 (200/*100*/-1)
#define EH_Y_LOCATE_271 ((PSP_HEIGHT272-1)/*-1*/)
		int val333=(val3<<2);
		for (/*j=0*/; j<((EH_R08V+EH_L08V)+(EH_24V)); j+=2) 	/* GPU�]�����Ԃɍ���Ȃ��̂ŁA�����Ɗm��(j�͓r������) */
		{
			vertices[j].x = x3;
			vertices[j].y = EH_Y_LOCATE_271;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val333);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = EH_Y_LOCATE_199;
			vertices[j+1].z = 65535/*0*/;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val333);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 += 14   /*72/2*/;/*�㉏��*/
			x3 += 32   /*128/2*/;/*������*/
			x4 ^= 0x40/*0x80*/;  /*x4 +=  32*//*64*/;/*�e�N�X�`��*/
		}
		#if 0
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_R08V), NULL, vertices);						/* �㑤�A�E�ӂ��� */
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_L08V), NULL, &vertices[EH_R08V]);				/* �㑤�A���ӂ��� */
		#else
		/* ���ӁF�J�����O�L���̏ꍇ�A�����ɂȂ�܂��F�����|���S�����`����Ȃ����𗘗p�����n�b�N */
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_R08V+EH_L08V), NULL, vertices);				/* �㑤�A�E�ӂ��� */	/* �㑤�A���ӂ��� */
		#endif
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (EH_24V), NULL, &vertices[(EH_R08V+EH_L08V)]); 	/* ������ */

}
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (EH_24V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/


/*
0111 7

1000 8
1001 9
1010 a
1011 b

1100 c
1101 d
1110 e
1111 f

*/

// 1 3 5 7 9
// 0 2 4 6 8

		#endif

/*---------------------------------------------------------
	2D�w�i(���쒆)
	-------------------------------------------------------
---------------------------------------------------------*/
		#if (1)
static void gu_draw_bg_2D(void)
{
//	#define ND_12V (5*2)
	#define ND_12V (6*2)
//	#define ND_12V (8*2)
//	#define ND_24V (ND_12V+(8*2))
	#define ND_24V (ND_12V+(6*2))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory((/*ND_12V*/ND_24V)*sizeof(Vertex_uvcxyz));	/* GPU���Ԃɍ����̂ŏȗ��m�� */
	/* ��:������͍Ŕw�ʂȂ̂ŏȗ����Ă�GPU�]�����Ԃɍ���(�{���͏ȗ�������ǂ��Ȃ��̂����m��Ȃ�???) */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define ND_Y000 (0)
#define ND_Y272 ((255)/*-1*/)
//#define ND_Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	�㑤 */
		/* 176==(GAME_WIDTH/2) */
		x2 = (0) /*+ ((val1&127)-64)*/;
		x3 = (0);
		x4 = (0);
		for (j=0; j<ND_24V; j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = ND_Y272;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = ND_Y000;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color = blendlevel;
			#endif
			x2 +=  32;/*�㉏��*/
			x3 +=  32;/*������*/
			x4 +=  32/*64*/;/*�e�N�X�`��*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (ND_24V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (ND_12V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/
}
		#endif


/*---------------------------------------------------------
	�{�X�̌��̖����w(��)
---------------------------------------------------------*/

 int boss_x256;
 int boss_y256;
static int aaa_yyy;
static void gu_draw_big_maho_jin(void)
{
//	#define BM_32V ((16*2)+2)	/* �����(+2�͏d�Ȃ镪) */
//	#define BM_32V ((16+1)*2)	/* �����(+2�͏d�Ȃ镪) */
	#define BM_32V ((32+1)*2)	/* �����(+2�͏d�Ȃ镪) */

	Vertex_uvcxyz_C32* vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((BM_32V)*sizeof(Vertex_uvcxyz_C32));
//
	/* --- �����������l */
	//#if (1==USE_VCOLOR)
	//unsigned int blendlevel = (0xaa2f7fff);/* ������ */
	//#endif
	//#define big_maho_color8888 (0x997f2fff)		/* --- �����������l */
	/*								���ΐ� AABBGGRR */
		const unsigned int maho_color_list[8] =
		{	/*AABBGGRR*/
			0x997f2fff, //		0x997f2fff,/**/
			0x77ff993f, //		0x77ff5f1f,/* 1�� */		/* �A���X */
			0x553fff3f, //		0x99ff7f2f,/* 2�� */		/* ��	  */
			0xaa2f7fff, //		0xaa2f7fff,/* 3�� */		/* ����   */
			0xaaffffff, //		0x997f2fff,/* 4�� */		/* �P��   */
			0x33ff5f1f, //		0x33ff5f1f,/* 5�� */		/* �p�`�F */
			0x997f2fff, //		0x997f2fff,/* 6�� */		/* ���   */
			0x997f2fff, //		0x997f2fff,/* ending */
		//	0xff601010, //	//	0xff601010,/**/
		}; /* ���ΐ� AABBGGRR */
	const unsigned int big_maho_color8888 = maho_color_list[player_now_stage&0x07];
//
	unsigned int j;
	unsigned short uv_x4;

//	int boss_center_x = ((GAME_WIDTH)/2);
//	int boss_center_y = ((GAME_HEIGHT)/2);
	int boss_center_x = ((boss_x256)>>8)+(16);
	int boss_center_y = ((boss_y256)>>8)+(24);

	/* ���� */
//	int hankei_111 = (draw_boss_hp_value>>2);/* 128==1024/8 */
	int hankei_111 = (boss_life_value>>(2+3));/* 128==1024/8 */
	/* �O�� */
	int hankei_222 = (hankei_111+(16)/*(8)*/);/* 8[dot]�������� */

	/* �����̃e�N�X�`��x�ʒu */
	aaa_yyy++;
	uv_x4 = (/*val5*/(aaa_yyy) /*& 0xff*/);

/* --- [ ��]���� ] --- */
	int rotation_angle512 = (0);
	for (j=0; j<BM_32V; j+=2)/*32����*/
	{
	int sin_angle;
	int cos_angle;
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle512)&(512-1))]/*<<8*/);
	int ifx;
	int ify;
		ifx = ((hankei_222 * sin_angle)>>8);
		ify = ((hankei_222 * cos_angle)>>8);
		vertices[j].x = (boss_center_x + ifx);
		vertices[j].y = (boss_center_y + ify);
		vertices[j].z = 0;
		vertices[j].u = uv_x4;
		vertices[j].v = (127);	/*(�����̃e�N�X�`��y�ʒu[����]tex_str__y_low_127)*/
		#if (1==USE_VCOLOR)
		vertices[j].color	= (big_maho_color8888); 	/*blendlevel*/
		#endif
	//
		ifx = ((hankei_111 * sin_angle)>>8);
		ify = ((hankei_111 * cos_angle)>>8);
		vertices[j+1].x = (boss_center_x + ifx);
		vertices[j+1].y = (boss_center_y + ify);
		vertices[j+1].z = 0;
		vertices[j+1].u = uv_x4;
		vertices[j+1].v = (111)/*(111)*/;	/*111=127-16(�����̃e�N�X�`��y�ʒu[���]tex_str_y_high_111)*/
		#if (1==USE_VCOLOR)
		vertices[j+1].color = (big_maho_color8888); 	/*blendlevel*/
		#endif
		uv_x4				+=	32; 	/*64*/	/*�e�N�X�`��*/
		rotation_angle512	+=	16/*32*/;	/*64*/	/* �p�x */
	}
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (BM_32V), NULL, &vertices[0/*SL_24V*/]);
}
// 1 3 5 7 9
// 0 2 4 6 8


/*---------------------------------------------------------

---------------------------------------------------------*/


	/* �� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p) */
/*---------------------------------------------------------
	�������{��(�X�y�J)�������ꍇ�ɏo��B�����̃��S
	Border Power of Spiritual ��̂ŗ͂𔭊����鋫�E
	-------------------------------------------------------
	[ ������ ] / [ �c���� ] �Ɋւ��Ă� uv�X�N���[�� ���g���Ă܂��B
	���Ă�[���{�������������� uv�X�N���[�� ���Ǝv����B
---------------------------------------------------------*/

static void gu_draw_front_spell_logo(void)
{
//	#define SL_20V (9*2)	/* ��ʓr���B16�h�b�g����Ȃ��B((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL_20V (10*2)	/* ��ʋ�ݏo���B16�h�b�g�����B((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL_24V (12*2)	/* ��ʒ��x�B((12-1)*32 == 352 == 480[psp�̉�ʕ`�敝]-128[�p�l����]) [32slice] */

	Vertex_uvcxyz_C32* vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((SL_24V+SL_20V)*sizeof(Vertex_uvcxyz_C32));	/* GPU���Ԃɍ���Ȃ��̂ł����Ɗm�� */
	/* ��:������͍őO�ʂȂ̂ŏȗ������GPU�]�����Ԃɍ���Ȃ� */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (0x7777);
//(conv_bg_alpha)
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
//	#define SL_BLENDLEVEL (0x3f3f3f3f)	/*	63 �W������ */
	#define SL_BLENDLEVEL (0x64646464)	/* 100 */
//	#define SL_BLENDLEVEL (0x7f7f7f7f)	/* 127 �Â� */
//	#define SL_BLENDLEVEL (0xc8c8c8c8)	/* 200 �Â����� */
		#endif
	#endif
//
	unsigned int j;
	unsigned short x5;
	unsigned short x4;

/* Border Power of Spiritual �����̃e�N�X�`���ʒu(�㑤) */
#define TEX_STR_Y_HIGH_217		(200)/*217*/
/* Border Power of Spiritual �����̃e�N�X�`���ʒu(����) */
#define TEX_STR__Y_LOW_255		(255)
/* Border Power of Spiritual ��`�悷�镶���̍���(�X���C�X��64�ȓ��ɂ��Ȃ��ƁA�啝�ɑ��x�ቺ����Ǝv��) */
#define SPELL_LOGO_HEIGHT_64	(64)/*40*/
/* --- [ ������ ] --- */
/* SPELL_LOGO_LOCATE_Y01_160: ��ʍ��ォ��� y�I�t�Z�b�g(����) */
#define SPELL_LOGO_LOCATE_Y01_160 (160)
#define SPELL_LOGO_LOCATE_Y02_200 (SPELL_LOGO_LOCATE_Y01_160+SPELL_LOGO_HEIGHT_64)
	x5 = 0;
	x4 = (/*val5*/(pd_bomber_time) /*& 0xff*/);
	for (j=0; j<SL_24V; j+=2)
	{
		vertices[j].x = x5;
		vertices[j].y = SPELL_LOGO_LOCATE_Y02_200;
		vertices[j].z = 0;
		vertices[j].u = x4;
		vertices[j].v = (TEX_STR__Y_LOW_255);
		#if (1==USE_VCOLOR)
		vertices[j].color	= SL_BLENDLEVEL;
		#endif
	//
		vertices[j+1].x = x5;
		vertices[j+1].y = SPELL_LOGO_LOCATE_Y01_160;
		vertices[j+1].z = 0;
		vertices[j+1].u = x4;
		vertices[j+1].v = (TEX_STR_Y_HIGH_217);
		#if (1==USE_VCOLOR)
		vertices[j+1].color = SL_BLENDLEVEL;
		#endif
		x5 +=  32/*36*/;/*��/������*/
		x4 +=  32/*64*/;/*�e�N�X�`��*/
	}

/* --- [ �c���� ] --- */
/* SPELL_LOGO_LOCATE_X01_200: ��ʍ��ォ��� x�I�t�Z�b�g(����) */
#define SPELL_LOGO_LOCATE_X01_200 (16/*200*/)
#define SPELL_LOGO_LOCATE_X02_240 (SPELL_LOGO_LOCATE_X01_200+SPELL_LOGO_HEIGHT_64)
#define y5 x5
	y5 = 0; 	//	x4 = (val5 & 0xff);
	for (/*j=0*/; j<SL_24V+SL_20V; j+=2)	/* GPU�]�����Ԃɍ���Ȃ��̂ŁA�����Ɗm��(j�͓r������) */
	{
		vertices[j].x = SPELL_LOGO_LOCATE_X01_200;
		vertices[j].y = y5;
		vertices[j].z = 0;
		vertices[j].u = x4;
		vertices[j].v = (TEX_STR__Y_LOW_255);
		#if (1==USE_VCOLOR)
		vertices[j].color	= SL_BLENDLEVEL;
		#endif
	//
		vertices[j+1].x = SPELL_LOGO_LOCATE_X02_240;
		vertices[j+1].y = y5;
		vertices[j+1].z = 0;
		vertices[j+1].u = x4;
		vertices[j+1].v = (TEX_STR_Y_HIGH_217);
		#if (1==USE_VCOLOR)
		vertices[j+1].color = SL_BLENDLEVEL;
		#endif
		y5 +=  32/*36*/;/*��/������*/
		x4 +=  32/*64*/;/*�e�N�X�`��*/
	}
	#if 0
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_24V), NULL, vertices); 			/* [ ������ ] */
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_20V), NULL, &vertices[SL_24V]);	/* [ �c���� ] */
	#else
	/* ���ӁF�J�����O�L���̏ꍇ�A�����ɂȂ�܂��F�����|���S�����`����Ȃ����𗘗p�����n�b�N */
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_24V+SL_20V), NULL, vertices);		/* [ ������ ] */		/* [ �c���� ] */
	#endif
}
// 1 3 5 7 9
// 0 2 4 6 8


/*---------------------------------------------------------

---------------------------------------------------------*/


static void gu_draw_sdl_screen(void)
{
/* ����:
(a) SLICE_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
#define USE_SLICE_32 (0)
#if (0==USE_SLICE_32)
	#define SLICE_SIZE32	(64)
#else
	#define SLICE_SIZE32	(32)
#endif
	unsigned int j;
	unsigned int i;
	#if 0
	unsigned int count2;
	count2 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		count2 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			#define count2 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		#else
			#define count2 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	#if (0==USE_SLICE_32)
	unsigned int w_size;
	w_size = SLICE_SIZE32;
	#else
		#define w_size SLICE_SIZE32
	#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count2; )
	{
		#if (0==USE_SLICE_32)
		/* �Ō� */
		if (i==(count2-2))
		{
			w_size = 32;	/* 32 == 512-480 */
		}
		#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 0;
		vertices[(i)].x = j/**480/512*/;
		vertices[(i)].y = 0;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+w_size);
		vertices[(i)+1].v = 272/*320*/;
		vertices[(i)+1].x = (j+w_size)/**480/512*/;
		vertices[(i)+1].y = 272;
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count2), NULL, vertices);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
	#define SCRIPT_WINDOW_X_SIZE_340		(340)
	#define SCRIPT_WINDOW_Y_SIZE_070		(70)
	#define SCRIPT_WINDOW_SPACE_020 		(20)	/* [��ʒ[�}�[�W��] */
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
/* ����:
(a) SLICE_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
	unsigned int j;
	unsigned int i;
	#define SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_X 	(0) /* [�e�N�X�`���I�t�Z�b�g] */
	#define SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_Y 	(1+8) /* [�e�N�X�`���I�t�Z�b�g��1] */
	#define SCRIPT_FONT_SCREEN_TEXTURE_SIZE_X 		(1+4+47/*70*/) /* [�e�N�X�`���T�C�Y] */
	#define FONT_SCREEN_MARGIN_010		(10)	/* [�t�H���g�[�}�[�W��] */
	/* [��1] ((16/2)[dots]+1[dots](Gu�����C��)) */
	/* 8[���C��(depth_32bit_int)]==16[���C��(depth_16bit_short)] */
	#if 0
	unsigned int count29;
	count29 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		count29 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			/*
			320 == 480-((6[��ʒ[�}�[�W��] + 10[�t�H���g�[�}�[�W��] )x 2) -(128[�p�l���E�B���h�E��])
			*/
		//	#define count29 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		//	#define count29 (10)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
			#define count29 (10)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		//	#define count29 (12)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		#else
			#define count29 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count29 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	//#if (0==USE_SLICE_32)
	//unsigned int w_size;
	//w_size = SLICE_SIZE32;
	//#else
		#define w_size99 SLICE_SIZE32
	//#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count29; )
	{
	//	#if (0==USE_SLICE_32)
	//	/* �Ō� */
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
		vertices[(i)+1].v = 1+(SCRIPT_FONT_SCREEN_TEXTURE_OFFSET_Y)+(SCRIPT_FONT_SCREEN_TEXTURE_SIZE_X);/*272*/ /*320*/
		vertices[(i)+1].x = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_X_POSITION_020)+(j+w_size99);/**480/512*/
		vertices[(i)+1].y = (FONT_SCREEN_MARGIN_010)+(SCRIPT_WINDOW_Y_POSITION_182)+(SCRIPT_FONT_SCREEN_TEXTURE_SIZE_X);/*272*/
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count29), NULL, vertices);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
	//#define SUPEKA_WINDOW_Y_SIZE_070		(32/*70*/)/*70 == (16*4)+6 ??? */
	/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-SUPEKA_WINDOW_X_SIZE_340)/2) == SUPEKA_WINDOW_SPACE_020 */
	//#define SUPEKA_WINDOW_SPACE_020		(6)
	#define SUPEKA_WINDOW_X_POSITION_020	(128-32)
	#define SUPEKA_WINDOW_Y_POSITION_182	(12)
#endif
static void gu_draw_supeka_screen(void)
{
/* ����:
(a) SLICE_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
	unsigned int j;
	unsigned int i;
	#define SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_X 	(0) /* [�e�N�X�`���I�t�Z�b�g] */
	#define SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_Y 	(1+8) /* [�e�N�X�`���I�t�Z�b�g��1] */
	#define SUPEKA_FONT_SCREEN_TEXTURE_SIZE_X 		(18) /* [�e�N�X�`���T�C�Y] 70 */
	/* [��1] ((16/2)[dots]+1[dots](Gu�����C��)) */
	/* 8[���C��(depth_32bit_int)]==16[���C��(depth_16bit_short)] */
	#if 0
	unsigned int SUPEKA_count29;
	SUPEKA_count29 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		SUPEKA_count29 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			/*
			320 == 480-((6[��ʒ[�}�[�W��] + 10[�t�H���g�[�}�[�W��] )x 2) -(128[�p�l���E�B���h�E��])
			*/
		//	#define SUPEKA_count29 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		//	#define SUPEKA_count29 (10)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
			#define SUPEKA_count29 (8/*10*/)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		//	#define SUPEKA_count29 (12)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		#else
			#define SUPEKA_count29 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(SUPEKA_count29 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0+(SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_X);
	//#if (0==USE_SLICE_32)
	//unsigned int w_size;
	//w_size = SLICE_SIZE32;
	//#else
		#define SUPEKA_w_size99 SLICE_SIZE32
	//#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<SUPEKA_count29; )
	{
	//	#if (0==USE_SLICE_32)
	//	/* �Ō� */
	//	if (i==(SUPEKA_count29-2))
	//	{
	//	//	w_size = 32;	/* 32 == 512-480 */
	//		w_size = 44;	/* 44 == 384-340 */
	//	}
	//	#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 1+(SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_Y);/*+(0)*/
		vertices[(i)].x = (SUPEKA_WINDOW_X_POSITION_020)+j/**480/512*/;
		vertices[(i)].y = (SUPEKA_WINDOW_Y_POSITION_182);/*+(0)*/
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+SUPEKA_w_size99);
		vertices[(i)+1].v = 1+(SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_Y)+(SUPEKA_FONT_SCREEN_TEXTURE_SIZE_X);// /*272*/ /*320*/
		vertices[(i)+1].x = (SUPEKA_WINDOW_X_POSITION_020)+(j+SUPEKA_w_size99);/**480/512*/
		vertices[(i)+1].y = (SUPEKA_WINDOW_Y_POSITION_182)+(SUPEKA_FONT_SCREEN_TEXTURE_SIZE_X);/*272*/
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (SUPEKA_count29), NULL, vertices);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void gu_draw_side_panel(void)
{
	if (0!=draw_boss_hp_value)
	{
	//	#define draw_boss_hp_value			(340)
		#define SIDE_WINDOW_Y_SIZE_070		(4)/*70*/
		/* 6 == ((480-128-340)/2) == ((GAME_WIDTH-draw_boss_hp_value)/2) == SIDE_WINDOW_X_POSITION_020 */
		#define SIDE_WINDOW_X_POSITION_020	(64)/* (50)HPGAUGE_X_OFS+(10)drec.x */
		#define SIDE_WINDOW_Y_POSITION_182	(1+3)/*GAME_HEIGHT-SIDE_WINDOW_Y_SIZE_070-SIDE_WINDOW_X_POSITION_020*/

		/* -- ��`�� */
		/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
		/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
		sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
		{
			//#define TEXTURE_FLAGS1111 	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define SIDE_PANEL_TEXTURE_FLAGS	(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define SIDE_PANEL_TEXTURE_FLAGS	(				GU_COLOR_8888 | 	GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//GU_VERTEX_32BITF|GU_TRANSFORM_2D
			// we make local copies of the line into the main buffer here, so we don't have to flush the cache
			Point_Vertex_color* my_vertices = sceGuGetMemory((4/*5*/) * sizeof(Point_Vertex_color));

			// create a lineloop
			my_vertices[0].x = (SIDE_WINDOW_X_POSITION_020) 					/*x-width*/;	my_vertices[0].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[1].x = (SIDE_WINDOW_X_POSITION_020)+(draw_boss_hp_value)/*x+width*/;	my_vertices[1].y = (SIDE_WINDOW_Y_POSITION_182)   /*y-height*/;
			my_vertices[2].x = (SIDE_WINDOW_X_POSITION_020) 					/*x-width*/;	my_vertices[2].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
			my_vertices[3].x = (SIDE_WINDOW_X_POSITION_020)+(draw_boss_hp_value)/*x+width*/;	my_vertices[3].y = (SIDE_WINDOW_Y_POSITION_182)+(SIDE_WINDOW_Y_SIZE_070)/*y+height*/;
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
		sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
//
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	HAIKEI_00_BOSS_SPELL_CARD = 0,
	HAIKEI_01_JIKI_SPELL_CARD,
	HAIKEI_02,
	HAIKEI_03_MESSAGE,
	HAIKEI_04_MAX,
};
static void gu_draw_rect_window(int rect_type)
{
	/* -- ����ӃE�B���h�E��`�� */
	/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
	/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
	sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
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
		static RECT_WINDOW_TABLE rect_table[(HAIKEI_04_MAX)] =
		{
			{(SCRIPT_WINDOW_X_POSITION_020),(SCRIPT_WINDOW_Y_POSITION_212),(SCRIPT_WINDOW_X_SIZE_256),	(SCRIPT_WINDOW_Y_SIZE_012),(0x7fff0f0f),(0x7f0f0f0f)},/* BLUE �{�X�X�y�J */
			{(SCRIPT_WINDOW_X_POSITION_020),(SCRIPT_WINDOW_Y_POSITION_212),(SCRIPT_WINDOW_X_SIZE_256),	(SCRIPT_WINDOW_Y_SIZE_012),(0x7f0f0fff),(0x7f0f0f0f)},/* RED	���@�X�y�J */
			{(SCRIPT_WINDOW_X_POSITION_020),(SCRIPT_WINDOW_Y_POSITION_182),(SCRIPT_WINDOW_X_SIZE_256),	(SCRIPT_WINDOW_Y_SIZE_012),(0x7f0f0f0f),(0x7f0f0f0f)},/* ���g�p */
			{(SCRIPT_WINDOW_X_POSITION_020),(SCRIPT_WINDOW_Y_POSITION_182),(SCRIPT_WINDOW_X_SIZE_340),	(SCRIPT_WINDOW_Y_SIZE_052),(0x7f0f0f0f),(0x7f0f0f0f)},/* MASSAGE*/
		};
	//	#define SCRIPT_WINDOW_TEXTURE_FLAGS 	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	//	#define SCRIPT_WINDOW_TEXTURE_FLAGS 	(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#define SCRIPT_WINDOW_TEXTURE_FLAGS 	(				GU_COLOR_8888 | 	GU_VERTEX_16BIT | GU_TRANSFORM_2D)
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
		sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, SCRIPT_WINDOW_TEXTURE_FLAGS, (3), NULL, my_vertices);
		sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, SCRIPT_WINDOW_TEXTURE_FLAGS, (3), NULL, &my_vertices[2]);
	//
	//	sceGuColor(abgr222/*0xffff*/ /*abgr*/);
	//	sceGuColor(/*0xaaaa*/ ((abgr222&0x7f7f7f7f)>>1) /*abgr*/);
		sceGuColor( (0x3f878787) /*abgr*/);
		sceGuDrawArray(GU_LINE_STRIP, SCRIPT_WINDOW_TEXTURE_FLAGS, (5), NULL, my_vertices);
	}
	/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
	sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
}


#if (1==LINE_TEST)
/*---------------------------------------------------------
	���C���e�X�g
---------------------------------------------------------*/
static void gu_blit_lines(void)
{
	/* --- ��]�g��O���� */
	int sin_angle;
	int cos_angle;
	{
		/* �p�x��0-65535�x */
		static unsigned int rotation_angle65535;
		rotation_angle65535 += 50;
	//
		/* �p�x��0-511�x */
		unsigned int rotation_angle512;
	//	rotation_angle512	= ((spr->rotation_z) / (128/*65536/512*/)); 	/* �p�x��0-65535�x�Ȃ̂�0-511�x�֕ϊ��B */
	//	rotation_angle512	= ((spr->rotation_z) /*>> (7)*/);
		rotation_angle512	= ((rotation_angle65535) >> (7));
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle512)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle512)&(512-1))]/*<<8*/);
	}
	int boss_center_x = ((boss_x256)>>8)+(16);
	int boss_center_y = ((boss_y256)>>8)+(24);
	int player_center_x = ((player->x256)>>8)+(16);
	int player_center_y = ((player->y256)>>8)+(24);

	/* --  ��`�� */
	/* �e�N�X�`���[������ƃe�N�X�`���[�D��Ńt���b�g�|���S�����`��o���Ȃ��̂� */
	/* �ꎞ�I�Ƀe�N�X�`���[�������[�h�ɐ؂�ւ��� */
	sceGuDisable(GU_TEXTURE_2D);/* �e�N�X�`���[�������[�h�ɂ��� */
	{
	//	#define LINE_TEST_TEXTURE_FLAGS 	(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#define LINE_TEST_TEXTURE_FLAGS 	(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	//	#define LINE_TEST_TEXTURE_FLAGS 	(				GU_COLOR_8888 | 	GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//GU_VERTEX_32BITF|GU_TRANSFORM_2D
		// we make local copies of the line into the main buffer here, so we don't have to flush the cache
		Point_Vertex/*_color*/* my_vertices = sceGuGetMemory((5+2) * sizeof(Point_Vertex/*_color*/));
		// create a lineloop
		int e1x;		e1x = (boss_center_x);	/* (��]���S)���Ƀ{�X�̍��Wx */
		int e1y;		e1y = (boss_center_y);	/* (��]���S)���Ƀ{�X�̍��Wy */
		int p1x;		p1x = 0/*30*/;			/* (��]�I�t�Z�b�g)���[�U�[�I�t�Z�b�g����x */
		int p1y;		p1y = 64;				/* (��]�I�t�Z�b�g)���[�U�[�I�t�Z�b�g����y */
		int p2w;		p2w = 8;				/* ���[�U�[���̔��� */
		int p2h;		p2h = 256;				/* ���[�U�[���� */
		/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
		int ifx;
		int ify;
		int irx;
		int iry;
//
		ifx = (p1x)-(p2w);
		ify = (p1y);
		irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);
		iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);
		my_vertices[0].x = (e1x)+(irx); 		my_vertices[0].y = (e1y)+(iry);
		my_vertices[4].x = my_vertices[0].x;	my_vertices[4].y = my_vertices[0].y;
//
		ifx = (p1x)+(p2w);
		ify = (p1y);
		irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);
		iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);
		my_vertices[1].x = (e1x)+(irx); 		my_vertices[1].y = (e1y)+(iry);
//
		ifx = (p1x)+(p2w);
		ify = (p1y)+(p2h);
		irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);
		iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);
		my_vertices[2].x = (e1x)+(irx); 		my_vertices[2].y = (e1y)+(iry);
//
		ifx = (p1x)-(p2w);
		ify = (p1y)+(p2h);
		irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);
		iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);
		my_vertices[3].x = (e1x)+(irx); 		my_vertices[3].y = (e1y)+(iry);
//
	//	ifx = (p1x);
	//	ify = (p1y);
	//	irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);
	//	iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);
	//	my_vertices[4].x = (irx);				my_vertices[4].y = (iry);
//
		my_vertices[5].x = (e1x);				my_vertices[5].y = (e1y);	/* ���Ƀ{�X�̍��W */
		my_vertices[6].x = (player_center_x);	my_vertices[6].y = (player_center_y);
//
	//	my_vertices[0].color = (c1);
	//	my_vertices[1].color = (c1);
	//	my_vertices[2].color = (c2);
	//	my_vertices[3].color = (c2);
	//	my_vertices[4].color = (c1);
	//
	/* �{����]���S����t���̊p�x�։�]�����Ĕ��肷�� */
		ifx = (boss_center_x)-(player_center_x);/* (�{�� ifx �� -angle) */
		ify = (boss_center_y)-(player_center_y);/* (�{�� ify �� -angle) */
	//	ifx = (player_center_x)-(boss_center_x);/* ���ӁF�ȗ��v�Z�F ifx�� -ifx ���v�Z(�{�� ifx �� -angle) */
	//	ify = (player_center_y)-(boss_center_y);/* ���ӁF�ȗ��v�Z�F ify�� -ify ���v�Z(�{�� ify �� -angle) */
			#if 0
			/* [��CCW]���� 0�x�Ŕ����v���̊p�x�n */
			irx = ((ify * sin_angle) + (ifx * cos_angle));	irx = (irx >>8/*16*/);	/*90�x�i��*/
			iry = ((ify * cos_angle) - (ifx * sin_angle));	iry = (iry >>8/*16*/);	/*90�x�i��*/
			#endif
			#define sin2_angle	(-sin_angle)	/* sin(-angle) == -sin(angle) */	/* �ʑ� +180�x�i�� */
			#define cos2_angle	(cos_angle) 	/* cos(-angle) ==  cos(angle) */	/* �ʑ� +180�x�i�� */
		//	#define sin2_angle	(cos_angle) 	/* �ʑ�  +90�x�i�� */
		//	#define cos2_angle	(-sin_angle)	/* �ʑ� +180�x�i�� */
		//	#define sin2_angle	(-cos_angle)	/* �ʑ�  +90�x�i�� */
		//	#define cos2_angle	(sin_angle) 	/* �ʑ� +180�x�i�� */
			irx = ((ify * sin2_angle) + (ifx * cos2_angle));	irx = (irx >>8/*16*/);	/*90�x�i��*/
			iry = ((ify * cos2_angle) - (ifx * sin2_angle));	iry = (iry >>8/*16*/);	/*90�x�i��*/
	//	irx = ((ify * cos_angle) + (ifx * sin_angle));	irx = (irx >>8/*16*/);	/*90�x�x���*/
	//	iry = ((ify * sin_angle) - (ifx * cos_angle));	iry = (iry >>8/*16*/);	/*90�x�x���*/
	//
	//	sceGuColor(abgr222/*0xffff*/ /*abgr*/);
	//	sceGuColor(/*0xaaaa*/ ((abgr222&0x7f7f7f7f)>>1) /*abgr*/);
	//	sceGuColor( (0x3f878787) /*abgr*/);
	/* �_(����) vs ��`�̈�(���[�U�[)�Ŕ��� */
	if (
			((-p2w)<irx)	&&
			(irx<(p2w)) 	&&
		//	((p1y)<iry)
		//	((-p1y)<iry)
			((-p1y)>iry)	/* �ʑ� +180�x�i�݂Ȃ̂Œ��덇�킹 */
	)
	{
	//	sceGuColor( (0xafff5737) /*abgr*/);/*��*/
		sceGuColor( (0xaf3757ff) /*abgr*/);/*��*/		/* �������� */
	}
	else
	{
		sceGuColor( (0x7f878787) /*abgr*/);/*��*/		/* �������ĂȂ� */
	}
		sceGuDrawArray(GU_LINE_STRIP, LINE_TEST_TEXTURE_FLAGS, (5+2), NULL, my_vertices);
	}
	/* �e�N�X�`���[�L�����[�h�ł́A����ʓ��̃t���b�g�|���S�����g���Ȃ� */
	sceGuEnable(GU_TEXTURE_2D); 	/* �e�N�X�`���[�L�����[�h�ɖ߂� */
}
#endif


/*---------------------------------------------------------
	3D�w�i(�e�X�g��)
	-------------------------------------------------------
	Gum���g��Ȃ���(Gu+vfpu)��3D���o����炵���̂ŁA
	�e�X�g�ō���Ă݂�B
	���x�́H�H�H
---------------------------------------------------------*/

#include "psp_vfpu.h"
#if 0
typedef struct quat
{
	float x, y, z, w;
} __attribute__((aligned(16))) ScePspQuatMatrix;

void vfpu_srand(unsigned int x);
float vfpu_randf(float min, float max);

float vfpu_ease_in_out(float t);

void vfpu_perspective_matrix(ScePspFMatrix4 *m, float fovy, float aspect, float near, float far);
void vfpu_identity_matrix(ScePspFMatrix4 *m);

void vfpu_quaternion_copy(ScePspQuatMatrix *dst, ScePspQuatMatrix *src);
void vfpu_quaternion_from_euler(ScePspQuatMatrix *qout, float rx, float ry, float rz);
void vfpu_quaternion_exp(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_sample_linear(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, float t);
void vfpu_quaternion_to_matrix(ScePspQuatMatrix *q, ScePspFMatrix4 *m);
#endif

typedef struct
{
	/*float*/unsigned char u;
	/*float*/unsigned char v;
	unsigned int color;
	/*float*/signed char x;
	/*float*/signed char y;
	/*float*/signed char z;
} Vertex_cube;
		Vertex_cube 	__attribute__((aligned(16))) cube_polygon_model[(12*3)] =
{
	/* u	v	 aabbggrr */
	{  0,	0, 0xffffffff,-127,-127, 127}, // #0	// 0	/* �� */
	{255, 255, 0xffff7f7f, 127, 127, 127}, // #2	// 5	/* �� �� */
	{255,	0, 0xffffffff,-127, 127, 127}, // #1	// 4[]

	{  0,	0, 0xffffffff,-127,-127, 127}, // #0	// 0
	{  0, 255, 0xffffffff, 127,-127, 127}, // #3	// 1
	{255, 255, 0xffff7f7f, 127, 127, 127}, // #2	// 5	/* �� �� */
//
	{  0,	0, 0xffffffff,-127,-127,-127}, // #4	// 3[]	/* �� */
	{255, 255, 0xffffffff, 127, 127,-127}, // #6	// 6
	{255,	0, 0xffffffff, 127,-127,-127}, // #5	// 2

	{  0,	0, 0xffffffff,-127,-127,-127}, // #4	// 3[]
	{  0, 255, 0xffffffff,-127, 127,-127}, // #7	// 7
	{255, 255, 0xffffffff, 127, 127,-127}, // #6	// 6
//
	{  0,	0, 0xffffffff, 127,-127,-127}, // #5	// 0	/* �� */
	{255, 255, 0xff7fff7f, 127, 127, 127}, // #2	// 7	/* �� �� */
	{255,	0, 0xffffffff, 127,-127, 127}, // #3	// 3

	{  0,	0, 0xffffffff, 127,-127,-127}, // #5	// 0
	{  0, 255, 0xffffffff, 127, 127,-127}, // #6	// 4
	{255, 255, 0xff7fff7f, 127, 127, 127}, // #2	// 7	/* �� �� */
//
	{  0,	0, 0xffffffff,-127,-127,-127}, // #4	// 0[]	/* �� */
	{255, 255, 0xffffffff,-127, 127, 127}, // #1	// 7[]
	{255,	0, 0xffffffff,-127, 127,-127}, // #7	// 3

	{  0,	0, 0xffffffff,-127,-127,-127}, // #4	// 0[]
	{  0, 255, 0xffffffff,-127,-127, 127}, // #0	// 4
	{255, 255, 0xffffffff,-127, 127, 127}, // #1	// 7[]
//
	{  0,	0, 0xffffffff,-127, 127,-127}, // #7	// 0	/* �� */
	{255, 255, 0xff7f7fff, 127, 127, 127}, // #2	// 2	/* �� �� */
	{255,	0, 0xffffffff, 127, 127,-127}, // #6	// 1

	{  0,	0, 0xffffffff,-127, 127,-127}, // #7	// 0
	{  0, 255, 0xffffffff,-127, 127, 127}, // #1	// 3[]
	{255, 255, 0xff7f7fff, 127, 127, 127}, // #2	// 2	/* �� �� */
//
	{  0,	0, 0xffffffff,-127,-127,-127}, // #4	// 4[]	/* �Ԕ� */
	{255, 255, 0xffffffff, 127,-127, 127}, // #3	// 6
	{255,	0, 0xffffffff,-127,-127, 127}, // #0	// 7

	{  0,	0, 0xffffffff,-127,-127,-127}, // #4	// 4[]
	{  0, 255, 0xffffffff, 127,-127,-127}, // #5	// 5
	{255, 255, 0xffffffff, 127,-127, 127}, // #3	// 6

};


#define TEST_AUTO_ROT 1
ScePspFMatrix4 m4_projection3d;
ScePspFMatrix4 m4_view;
ScePspFMatrix4 m4_model;

#if (1==TEST_AUTO_ROT)
ScePspQuatMatrix p9_qcam;
ScePspQuatMatrix p9_qa;
ScePspQuatMatrix p9_qb;
#endif
ScePspQuatMatrix p9_test;

static	int nnn;
	// run sample
//	/*int*/float val_x;
//	/*int*/float val_y;
//	/*int*/float val_z;
//	/*int*/float val_w;
static void gu_init_vfpu(void)
{
	// vfpu has a random number generator,
	// and it needs to be seeded
	{
		u64 tick;
		sceRtcGetCurrentTick(&tick);
	//	vfpu_srand((u32)tick / 10000.0f);	����ł͎g���������������̂ŊԈႢ�B
		vfpu_srand((u32)tick ); 			/* vfpu_srand gets int. */
	}
	vfpu_perspective_matrix(&m4_projection3d, 60.0f, 480.0f/272.0f, 0.5f, 1000.0f);
	/* ������ */
	vfpu_identity_matrix(&m4_view); 		/* �P�ʍs��ŏ����� */
	vfpu_identity_matrix(&m4_model);		/* �P�ʍs��ŏ����� */
	vfpu_quaternion_identity(&p9_test); 	/* �P�ʃN�H�[�^�j�I���ŏ����� */

	// build a random starfield, using sphere to cartesian coordinate conversions
	// note how we use vfpu_randf here to generate random radian angles,
	// and how we use vfpu_rand_8888 to generate random colors for the star_cube_polygon_model
//	for (n=0; n<STARS_MAX; n++)
//	{
//		vfpu_sphere_to_cartesian(
//			vfpu_randf(0.0f, 6.28319f),
//			vfpu_randf(0.0f, 3.14159f),
//			1000.0f,
//			(ScePspFVector4 *)&(star_cube_polygon_model[n])
//		);
//		star_cube_polygon_model[n].color = vfpu_rand_8888(160, 255);
//	}

	// start off with 2 random rotations for the interpolation
//	vfpu_quaternion_from_euler(&p9_qa, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));	vfpu_quaternion_ln(&p9_qa, &p9_qa);
//	vfpu_quaternion_from_euler(&p9_qb, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));	vfpu_quaternion_ln(&p9_qb, &p9_qb);

	nnn = 0;


//	val_x = 0;
//	val_y = 0;
//	val_z = 0;
//	val_w = 0;

}


static void gu_draw_bg_3D_test01(void)
{

		// when we reach the limit of our interpolation:
		//		copy qb to qa
		//		generate a new random quaternion in qb
		//		take the log of quaternion qb
		//		reset the time counter

		nnn++;
		if (nnn >= (5*60) ) /* 300==(5[sec]*60[flame])*/
		{
			nnn = 0;
		#if (1==TEST_AUTO_ROT)
			vfpu_quaternion_copy(&p9_qa, &p9_qb);
		//	vfpu_quaternion_from_euler(&p9_qb,
		//		vfpu_randf(0.0f, 360.0f),
		//		vfpu_randf(0.0f, 360.0f),
		//		vfpu_randf(0.0f, 360.0f));
			vfpu_quaternion_from_euler512_int(&p9_qb,
			//	(int)vfpu_randf(0.0f, 512.0f),
			//	(int)vfpu_randf(0.0f, 512.0f),
			//	(int)vfpu_randf(0.0f, 512.0f));
				((vfpu_rand_8888(0, 255))&(512-1)),
				((vfpu_rand_8888(0, 255))&(512-1)),
				((vfpu_rand_8888(0, 255))&(512-1)));
		//	vfpu_quaternion_exp(&p9_qb, &p9_qb);
			vfpu_quaternion_ln(&p9_qb, &p9_qb);
		#endif
		}

#if 0
		{
			SceCtrlData pad;
			sceCtrlPeekBufferPositive(&pad, 1);
			if (pad.Buttons & PSP_CTRL_UP)			{		val_x-=0.01;			}
			if (pad.Buttons & PSP_CTRL_RIGHT)		{		val_y+=0.01;			}
			if (pad.Buttons & PSP_CTRL_DOWN)		{		val_x+=0.01;			}
			if (pad.Buttons & PSP_CTRL_LEFT)		{		val_y-=0.01;			}
		//
			if (pad.Buttons & PSP_CTRL_LTRIGGER)	{		running = 0;			}
			if (pad.Buttons & PSP_CTRL_RTRIGGER)	{		running = 0;			}
		//
			if (pad.Buttons & PSP_CTRL_TRIANGLE)	{		val_z-=0.01;			}
			if (pad.Buttons & PSP_CTRL_CIRCLE)		{		val_w+=0.01;			}
			if (pad.Buttons & PSP_CTRL_CROSS)		{		val_z+=0.01;			}
			if (pad.Buttons & PSP_CTRL_SQUARE)		{		val_w-=0.01;			}
		}
#endif

		// to build the view matrix:
		//		get the interpolated quaternion in qcam
		//		take the exp of qcam
		//		build a rotation matrix from qcam, place into view matrix
		//
		// here we use the function 0.5 + (cos(t) * 0.5) for our 'time' variable
		// this gives us the smooth ease in/ease out motion you see as the demo
		// interpolates between qa and qb
		//
		// this is why we must do 300-n, as cos(0) = 1, cos(PI) = -1, we want
		// the reverse, -1 to 1
	#if (1==TEST_AUTO_ROT)
		{
			float t;
			t = (nnn)*((float)(1.0f/300.0f));
			vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
		}
		vfpu_quaternion_exp(&p9_qcam, &p9_qcam);
		vfpu_quaternion_to_matrix(&p9_qcam, &m4_view);
	#endif

//
	#if 0
	p9_test.x = val_x;
	p9_test.y = val_y;
	p9_test.z = val_z;
	p9_test.w = val_w;
//
	vfpu_quaternion_to_matrix(&p9_test, &m4_projection3d);
	#endif
//
		// load our generated matrix data into the display list
		sceGuSetMatrix(GU_PROJECTION,	&m4_projection3d);		/* �����ϊ��p�}�g���N�X */
		sceGuSetMatrix(GU_VIEW, 		&m4_view);				/* ���_�ϊ��r���[�p�}�g���N�X */
		sceGuSetMatrix(GU_MODEL,		&m4_model); 			/* ���[���h���W�p�}�g���N�X */

	//	sceGuDrawArray(GU_POINTS,		GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_LINES,		GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_SPRITES,		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGumDrawArray(GU_SPRITES, 	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
//		sceGuDrawArray(GU_SPRITES,		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
//
		// draw cube
	//	sceGuFrontFace(GU_CCW);
//		sceGumDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
		sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_8BIT | GU_COLOR_8888 | GU_VERTEX_8BIT | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
	//	sceGuFrontFace(GU_CW);
}

//#define GU_VERTEX_SHIFT(n)	((n)<<7)
//#define GU_VERTEX_8BIT		GU_VERTEX_SHIFT(1)
//#define GU_VERTEX_16BIT		GU_VERTEX_SHIFT(2)
//#define GU_VERTEX_32BITF		GU_VERTEX_SHIFT(3)
//#define GU_VERTEX_BITS		GU_VERTEX_SHIFT(3)
