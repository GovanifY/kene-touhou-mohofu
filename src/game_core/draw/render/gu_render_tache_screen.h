
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"draw_screen.c" からのみ間接的にインクルードします。
---------------------------------------------------------*/


/*---------------------------------------------------------

---------------------------------------------------------*/

#if (1==USE_GU_TACHIE_TEST)/* (r34)テスト */

static void gu_draw_tache_screen(int ofs_x, int ofs_y)
{
/* 実験:
(a) SLICE_64_SIZE 64 で 512 x 272 転送 (場合分けなし)
(b) SLICE_64_SIZE 64 で 480 x 272 転送 (場合分けあり)
(c) SLICE_64_SIZE 32 で 480 x 272 転送 (場合分けなし)
結果: 速い : (b) > (c) > (a)  : 遅い
 */
#define T_USE_SLICE_32 (0)
#if (0==T_USE_SLICE_32)
	#define T_SLICE_64_SIZE32 (64)
#else
	#define T_SLICE_64_SIZE32 (32)
#endif
	unsigned int j;
	unsigned int i;
		#if (0==T_USE_SLICE_32)
			#define T_count2 ( 4)/*  4 == 2 x (128/64[T_SLICE_64_SIZE32] ) */
		#else
			#define T_count2 ( 8)/*  8 == 2 x (128/32[T_SLICE_64_SIZE32] ) */
		#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(T_count2 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
//	for (; (j+T_SLICE_64_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<T_count2; )
	{
		vertices[(i)].u = j;
		vertices[(i)].v = 0;
		vertices[(i)].x = ofs_x+j;/**480/512*/
		vertices[(i)].y = ofs_y+0;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+/*w_size*/T_SLICE_64_SIZE32);
		vertices[(i)+1].v = /*256*/(256)/*272*/;/*320*/ /* (????????) */
		vertices[(i)+1].x = ofs_x+(j+/*w_size*/T_SLICE_64_SIZE32);/**480/512*/
		vertices[(i)+1].y = ofs_y+256/*272*/;
		vertices[(i)+1].z = 0;
		i += 2;
		j += T_SLICE_64_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (T_count2), NULL, vertices);
}
#endif /* (1==USE_GU_TACHIE_TEST) */
