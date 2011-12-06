
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	2D�w�i(���쒆)
	-------------------------------------------------------
	����:
	GU_SPRITES(���_���X�v���C�g�Ƃ��ĕ`��)�A				������2D���ʂ�`���ꍇ�A1�����̈悠����2���_���ʁB�c����������Ƃ����2���_���ʁB
	GU_TRIANGLES(���_���O�p�`�̌J��Ԃ��Ƃ��ĕ`��)�A		������2D���ʂ�`���ꍇ�A1�����̈悠����2���_���ʁB�c����������Ƃ����2���_���ʁB
	GU_TRIANGLE_FAN(�n�_�̒��_�𒆐S�ɐ��ɂ��ĕ`��)�A 	������2D���ʂ�`���ꍇ�A4�����̈悠����5���_���ʁB����������̏ꍇ�A���̂߂�ǂ����������B
	GU_TRIANGLE_STRIP(��q�㒸�_���g���ĕ`��)�A 			������2D���ʂ�`���ꍇ�A1�����̈悠����0���_���ʁB�c����������Ƃ����2���_���ʁB
	-------------------------------------------------------
	GU_TRIANGLE_FAN�͉~(�����p�`)�̃|���S����2D�e�N�X�`����uv���W�ŊȒP�ɒ����B
	-------------------------------------------------------
	���̒��ŁA������2D���ʂ�`���̂Ȃ�AGU_TRIANGLE_STRIP ���g���������A
	�ł����_�����点�āA�ǂ�ȕ��ʂł�1�|���S���ŕ`����B
	�����ł́AGU_TRIANGLE_STRIP ���g���B
	-------------------------------------------------------

2D�w�i��`����[pixel] == 352[pixel] == 480[pixel]-128[pixel] == (psp��)[pixel]-(�p�l����)[pixel]
11 [����] == 352[pixel] / 32[slice]

	-------------------------------------------------------

11�����̏ꍇ�́A��`��`���̂�12���_������B

1	2	3  ... 11  12(12���_�����ɕK�v)
+---+---+-	  -+---+
| 1 | 2 |  ... | 11| (11�����ŁA��`��`��)
+---+---+-	  -+---+

	-------------------------------------------------------

	��`�Ȃ̂ŁA���� 12 ���_���A�c�� 2 ���_���A�K�v�B
	�܂�S���� 24���_���A�K�v�B
	ND_24V == 2D�w�i24���_��(Ni Dei haikei 24 Vertexes.)

	-------------------------------------------------------
	�e�N�X�`���c��256[pixel]�Ȃ̂ŁA 1:1�����̏ꍇ�A�c256�܂ł����`���Ȃ��B
	(�������`����2D�ό`����Ă��܂��̂ŕ`���Ȃ��B)
	psp�̏c��272[pixel]�Ȃ̂ŁA256[pixel]�ł͑���Ȃ��B������ 2��ɕ����ĕ`���B
	272[pixel] == 256[pixel] + 16[pixel]
	-------------------------------------------------------
	...
	-------------------------------------------------------
	�������2��ɕ����ĕ`���B�̂����A������1��ŕ`����B
	(������̓|���S����2��ɕ������邪�A���ۂ̎�����1�̃|���S���ŕ`����̂�1��ŕ`���B)
	GU_TRIANGLE_STRIP(��q�㒸�_���g���ĕ`��)
	-------------------------------------------------------
	�O������Ƃ��āA�������|���S���̕`����֎~���Ă��鎖(==�J�����O�L��)���K�v�B
	�͕핗�ł́A
	sceGuEnable(GU_CULL_FACE);	�J�����O�L��(�������|���S���͕`�悵�Ȃ�)
	sceGuFrontFace(GU_CW);		�E���̃|���S����\�Ƃ��Đݒ�B
	�܂�E���̃|���S���͕`���B�����̃|���S���͕`���Ȃ��B
	���ォ�珇�ɁA��q��ɋ�`��`�悷��B
	����ɍ����ɁA��q��ɋ�`��`�悷��B
	��������ƉE�ォ�獶���Ɉڂ镔���́A�K���|���S�����������ɂȂ�B
	�������|���S���́A�`�悳��Ȃ��̂œs�����悢�B
	-------------------------------------------------------
	�������|���S���́A�n�[�h�E�F�A�[��gpu���`��O�ɒ��˂�̂ŁA
	�ǂ�ȂɃT�C�Y���傫���Ă��`�摬�x�ɉe�����Ȃ��B
	-------------------------------------------------------
	Scissor���g���ĕ`��͈͍��W���w�肵�Ă����΁A�n�[�h�E�F�A�[��gpu���`��O�ɒ��˂�̂ŁA
	�`��͈͊O�Ȃ�A�ǂ�ȂɃT�C�Y���傫���Ă��`�摬�x�ɉe�����Ȃ��B
	-------------------------------------------------------
---------------------------------------------------------*/
#if (1)
static void gu_draw_bg_2D(void)
{
	#define ND_24V ((12*2))

	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(((ND_24V)+(ND_24V))*sizeof(Vertex_uvcxyz));	/* �����Ɗm�� */
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((cg.conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(cg.conv_bg_alpha)
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
	unsigned int blendlevel = (((cg.conv_bg_alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->center.x256>>8);
		#if 1
		val3_256 -= (cg.current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= (0xff);
		#endif

#define ND_Y000 (0)
#define ND_Y255 ((256)-1)
#define ND_Y271 ((272)-1)
/* ���̂�1[�h�b�g]�����(???) ����������Y���W0���炶��Ȃ���1����H�H */
#define ND_ADD1 (1)
//#define ND_Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
/*	�㑤 */
		/* 176==(GAME_WIDTH/2) */
		x2 = (0) /*+ ((val1&0x7f)-64)*/;
		x3 = (0);
		x4 = (0);
		for (j=0; j<(ND_24V); j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = (ND_Y255);
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (0xff+1)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = (ND_Y000);
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
/*	���� */
		/* 176==(GAME_WIDTH/2) */
		x2 = (0) /*+ ((val1&0x7f)-64)*/;
		x3 = (0);
		x4 = (0);
		for (/*j=0*/; j<((ND_24V)+(ND_24V)); j+=2)	/* �����Ɗm��(j�͓r������) */
		{
			vertices[j].x = x3;
			vertices[j].y = (ND_Y271+ND_ADD1);
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = (/*0xff*/0x0f+1+ND_ADD1)+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = (ND_Y255/*+1*/);
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
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, ((ND_24V)+(ND_24V)), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (ND_12V), NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, NULL, vertices);/*2D��p�H*/
}
#endif
