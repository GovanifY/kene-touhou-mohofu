
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------

---------------------------------------------------------*/


static void gu_draw_sdl_screen(void)
{
/* ����:
(a) SLICE_64_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_64_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_64_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
#define USE_SLICE_32 (0)
#if (0==USE_SLICE_32)
	#define SLICE_64_SIZE32 (64)
#else
	#define SLICE_64_SIZE32 (32)
#endif
	unsigned int j;
	unsigned int i;
	#if 0
	unsigned int count2;
	count2 = 0;
	for (j=0; (j+SLICE_64_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_64_SIZE32)
	{
		count2 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			#define count2 (16)/* 16 == 2 x (512/64[SLICE_64_SIZE32] ) */
		#else
			#define count2 (30)/* 30 == 2 x (480/32[SLICE_64_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz));
	i = 0;
	j = 0;
	#if (0==USE_SLICE_32)
	unsigned int w_size;
	w_size = SLICE_64_SIZE32;
	#else
		#define w_size SLICE_64_SIZE32
	#endif
//	for (; (j+SLICE_64_SIZE32)<=(/*480*/512/*512*/); )
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
		j += SLICE_64_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count2), NULL, vertices);
}
