
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�� �t�����g�ʃG�t�F�N�g(�e�N�X�`�����p)
	-------------------------------------------------------
	�������{��(�J�[�h)�������ꍇ�ɏo��B�����̃��S
	Border Power of Spiritual ��̂ŗ͂𔭊����鋫�E
	-------------------------------------------------------
	[ ������ ] / [ �c���� ] �Ɋւ��Ă� uv�X�N���[��(�e�N�X�`���X�N���[��) ���g���Ă܂��B
	���Ă�[���{����(Direct 3D�� uv�X�N���[�� �͊ȒP�ɏo����̂�)
	���������� uv�X�N���[�� ���Ǝv����B
---------------------------------------------------------*/

static void gu_draw_front_spell_logo(void)
{
//	#define SL_20V (9*2)	/* ��ʓr���B16�h�b�g����Ȃ��B((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL_20V (10*2)	/* ��ʋ�ݏo���B16�h�b�g�����B((9-1)*32==256 < 272 < 288==(10-1)*32) [32slice] */
	#define SL_24V (12*2)	/* ��ʒ��x�B((12-1)*32 == 352 == 480[psp�̉�ʕ`�敝]-128[�p�l����]) [32slice] */

	Vertex_uvcxyz_C32* vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory((SL_24V+SL_20V)*sizeof(Vertex_uvcxyz_C32));	/* [ ������ + �c���� ] ���m�� */
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
/* Border Power of Spiritual �����̍���(�X���C�X��64�ȓ��ɂ��Ȃ��ƁA�啝�ɑ��x�ቺ����Ǝv��) */
#define SPELL_LOGO_HEIGHT_64	(64)/*40*/
/* --- [ ������ ] --- */
/* SPELL_LOGO_LOCATE_Y01_160: ��ʍ��ォ��� y�I�t�Z�b�g(����) */
#define SPELL_LOGO_LOCATE_Y01_160 (160)
#define SPELL_LOGO_LOCATE_Y02_200 (SPELL_LOGO_LOCATE_Y01_160+SPELL_LOGO_HEIGHT_64)
	x5 = (GAME_X_OFFSET+0);
	x4 = (/*val5*/(cg.bomber_time) /*& 0xff*/);
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
		x5 += (32);/* ��/������ */
		x4 += (32);/* �e�N�X�`�� */
	}

/* --- [ �c���� ] --- */
/* SPELL_LOGO_LOCATE_X01_200: ��ʍ��ォ��� x�I�t�Z�b�g(����) */
#define SPELL_LOGO_LOCATE_X01_200 (GAME_X_OFFSET+(16))/*200*/
#define SPELL_LOGO_LOCATE_X02_240 (SPELL_LOGO_LOCATE_X01_200+SPELL_LOGO_HEIGHT_64)
#define y5 x5
	y5 = 0; 	//	x4 = (val5 & 0xff);
	for (/*j=0*/; j<SL_24V+SL_20V; j+=2)	/* ��������(j�͓r������) */
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
		y5 += (32);/* ��/������ */
		x4 += (32);/* �e�N�X�`�� */
	}
	#if 0
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_24V), NULL, vertices); 			/* [ ������ ] */
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_20V), NULL, &vertices[SL_24V]);	/* [ �c���� ] */
	#else
	/* ���ӁF�J�����O�L���̏ꍇ�A�����ɂȂ�܂��F�����|���S�����`����Ȃ����𗘗p�����n�b�N�B(�`����Ȃ�==(�`��O�ɏ����𝛂˂�̂�)���x�ቺ���Ȃ�) */
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650_C32, (SL_24V+SL_20V), NULL, vertices);		/* [ ������ + �c���� ] */
	#endif
}
// 1 3 5 7 9
// 0 2 4 6 8

