
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/

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
(a) SLICE_64_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_64_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_64_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
	unsigned int j;
	unsigned int i;
	#define SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_X 	(0) /* [�e�N�X�`���I�t�Z�b�g] */
	#define SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_Y 	(1+8) /* [�e�N�X�`���I�t�Z�b�g��1] */
	#define SUPEKA_FONT_SCREEN_TEXTURE_SIZE_X		(18) /* [�e�N�X�`���T�C�Y] 70 */
	/* [��1] ((16/2)[dots]+1[dots](Gu�����C��)) */
	/* 8[���C��(depth_32bit_int)]==16[���C��(depth_16bit_short)] */
	#if 0
	unsigned int SUPEKA_count29;
	SUPEKA_count29 = 0;
	for (j=0; (j+SLICE_64_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_64_SIZE32)
	{
		SUPEKA_count29 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			/*
			320 == 480-((6[��ʒ[�}�[�W��] + 10[�t�H���g�[�}�[�W��] )x 2) -(128[�p�l���E�B���h�E��])
			*/
		//	#define SUPEKA_count29 (16)/* 16 == 2 x (512/64[SLICE_64_SIZE32] ) */
		//	#define SUPEKA_count29 (10)/* 16 == 2 x (512/64[SLICE_64_SIZE32] ) */
			#define SUPEKA_count29 (8/*10*/)/* 10 == 2 x (320/64[SLICE_64_SIZE32] ) */
		//	#define SUPEKA_count29 (12)/* 10 == 2 x (320/64[SLICE_64_SIZE32] ) */
		#else
			#define SUPEKA_count29 (30)/* 30 == 2 x (480/32[SLICE_64_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(SUPEKA_count29 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0+(SUPEKA_FONT_SCREEN_TEXTURE_OFFSET_X);
	//#if (0==USE_SLICE_32)
	//unsigned int w_size;
	//w_size = SLICE_64_SIZE32;
	//#else
		#define SUPEKA_w_size99 SLICE_64_SIZE32
	//#endif
//	for (; (j+SLICE_64_SIZE32)<=(/*480*/512/*512*/); )
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
		j += SLICE_64_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (SUPEKA_count29), NULL, vertices);
}

