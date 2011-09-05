
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void gu_draw_tache_screen(int ofs_x, int ofs_y, u32 spr_color32)
static void gu_draw_tache_screen(int ofs_x, int ofs_y, u32 blendlevel32, u32 draw_width_2n)
{
/* ����:
(a) SLICE_64_SIZE 64 �� 512 x 272 �]�� (�ꍇ�����Ȃ�)
(b) SLICE_64_SIZE 64 �� 480 x 272 �]�� (�ꍇ��������)
(c) SLICE_64_SIZE 32 �� 480 x 272 �]�� (�ꍇ�����Ȃ�)
����: ���� : (b) > (c) > (a)  : �x��
 */
#define T_USE_SLICE_32 (0)
#if (0==T_USE_SLICE_32)
	#define T_SLICE_64_SIZE32 		(64)
	#define T_SLICE_64_SHIFT_BITS 	(6-1)/*(5, (6-1)�̈Ӗ��� 2^(6-1), 64==2^6)*/
	//		#define T_count2 ( 4)/*  4 == 2 x (128/64[T_SLICE_64_SIZE32] ) */
#else
	#define T_SLICE_64_SIZE32 		(32)
	#define T_SLICE_64_SHIFT_BITS 	(5-1)/*(4, (5-1)�̈Ӗ��� 2^(5-1)), 32==2^5)*/
	//		#define T_count2 ( 8)/*  8 == 2 x (128/32[T_SLICE_64_SIZE32] ) */
#endif
	unsigned int j;
	unsigned int i;
	unsigned int T_count2;/*(�w��SLICE���ł̕`���)*/
	#if 0/*(�w�蕝�ŏ������`�悵���ꍇ�̕`��񐔂��Z�o)*/
	{	/*(�`�敝�𒲂ׂ�)*/
		T_count2=0;
		j=0;
		for (; j<draw_width_2n; )
		{
			j += T_SLICE_64_SIZE32;
			T_count2++;
		}
	}
	#else
	T_count2 = (draw_width_2n>>T_SLICE_64_SHIFT_BITS);
	#endif
	Vertex_uvcxyz_C32	*vertices;
	vertices = (Vertex_uvcxyz_C32*)sceGuGetMemory(T_count2 * sizeof(Vertex_uvcxyz_C32));
	i=0;
	j=0;
//
	/* --- �����������l */
	#if (1==USE_VCOLOR)
		#if 0//(16==USE_BLIT_COLOR_BIT)
			#if 0
				/*ARGB4444*/
				unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
			#endif
			#if 1
				/*ARGB0565*/
				unsigned /*int*/short blendlevel = MAKECOL16( (spr->alpha), (spr->alpha), (spr->alpha) );
			#endif
		#else
//	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
//	unsigned int blendlevel = (spr_color32);
		#endif
	#endif

//	for (; (j+T_SLICE_64_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<T_count2; )	/*(���ׂ��`�敝���`��)*/
//	for (; j<draw_width_2n; )
	{
		vertices[(i)].u = j;
		vertices[(i)].v = 0;
		#if (1==USE_VCOLOR)
		vertices[(i)].color 	= blendlevel32;
		#endif
		vertices[(i)].x = ofs_x+j;/**480/512*/
		vertices[(i)].y = ofs_y+0;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+/*w_size*/T_SLICE_64_SIZE32);
		vertices[(i)+1].v = /*256*/(256)/*272*/;/*320*/ /* (????????) */
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color 	= blendlevel32;
		#endif
		vertices[(i)+1].x = ofs_x+(j+/*w_size*/T_SLICE_64_SIZE32);/**480/512*/
		vertices[(i)+1].y = ofs_y+256/*272*/;
		vertices[(i)+1].z = 0;
		i += 2;
		j += T_SLICE_64_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650_C32, (T_count2), NULL, vertices);
}
