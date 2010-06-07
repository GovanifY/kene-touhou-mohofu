
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	Guを使った画面描画処理
---------------------------------------------------------*/

#define USE_FAKE_LOADING (1)
#if (1==USE_FAKE_LOADING)
typedef struct
{
	unsigned short u;
	unsigned short v;
	short x;
	short y;
	short z;
} VertexG00_uvxyz;

static void blitAlphamy_imageToScreen(int sx, int sy, int width, int height, my_image* source, int dx, int dy)
{
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuTexMode(/*SDL_GU_PSM_0000*/GU_PSM_8888/*GU_PSM_8888*/, 0, 0, 0);
	sceGuTexImage(0, source->texture_width, source->texture_height, source->texture_width, (void*) source->pixels);
//	trans_format16(pixsrc, pixdst, (msize));
	int j;
	j = 0;
	while (j < width)
	{
		VertexG00_uvxyz* vertices = (VertexG00_uvxyz*) sceGuGetMemory(2 * sizeof(VertexG00_uvxyz));
		int sliceWidth = 64;
		if (j + sliceWidth > width)
		{
			 sliceWidth = width - j;
		}
		vertices[0].u = sx + j;
		vertices[0].v = sy;
		vertices[0].x = dx + j;
		vertices[0].y = dy;
		vertices[0].z = 0;
		vertices[1].u = sx + j + sliceWidth;
		vertices[1].v = sy + height;
		vertices[1].x = dx + j + sliceWidth;
		vertices[1].y = dy + height;
		vertices[1].z = 0;
		#define TEXTURE_FLAGS_AAA (GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS_AAA, (2), NULL, vertices);
		j += sliceWidth;
	}
}
void render_blit_fake_loading_full(void)
{
	char file_name_str[16/*15*/];
	sprintf(file_name_str, "loading512.png");
	my_image	*loading_image;
	loading_image = png_load_my_image(file_name_str);
	if (loading_image)
	{
		sceDisplayWaitVblankStart();
		sceKernelDcacheWritebackInvalidateAll();
		sceGuStart(GU_DIRECT, gulist);//guStart();
		//
		blitAlphamy_imageToScreen(0, 0, 480, 272, loading_image, 0, 0);
		sceGuFinish();
		sceGuSync(0, 0);
	//	sceDisplayWaitVblankStart();
	//	sceGuSwapBuffers(); //flipScreen();
	//	sceGuSwapBuffers(); //flipScreen();
	//	sceGuSwapBuffers(); //flipScreen();

		sceGuSwapBuffers(); 	//flipScreen();
		sceKernelDelayThread(5000000);/* 5[秒]何もしないで待つ */
		png_free_my_image(loading_image);
		sceGuSwapBuffers(); 	//flipScreen();
	}
//	disableGraphics();
}
void render_blit_fake_loading_init(void)
{
	{
		sceGuInit();

		sceGuStart(GU_DIRECT, gulist);//guStart();
	//
//		sceGuDrawBuffer(/*SDL_GU_PSM_0000*/GU_PSM_8888/*GU_PSM_8888*/, (void*)0, PSP_BUFFER_WIDTH512);
//		#define FRAMEBUFFER_SIZE (PSP_BUFFER_WIDTH512*PSP_HEIGHT272*4)
//		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)FRAMEBUFFER_SIZE, PSP_BUFFER_WIDTH512);
//		sceGuDepthBuffer((void*) (FRAMEBUFFER_SIZE*2), PSP_BUFFER_WIDTH512);
	//
	#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
	//	sceGuDrawBuffer(GU_PSM_5551, (void*)0, PSP_BUFFER_WIDTH512);
		sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0, PSP_BUFFER_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, PSP_BUFFER_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x88000, PSP_BUFFER_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
		sceGuDrawBuffer(GU_PSM_8888, (void*)0, PSP_BUFFER_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, PSP_BUFFER_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x110000, PSP_BUFFER_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#endif /* SCREEN_DEPTH */
	//
		sceGuOffset(2048 - (PSP_WIDTH480 / 2), 2048 - (PSP_HEIGHT272 / 2));
		sceGuViewport(2048, 2048, PSP_WIDTH480, PSP_HEIGHT272);
	//
		sceGuEnable(GU_SCISSOR_TEST);
		sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);
	//
		sceGuAlphaFunc(GU_GREATER, 0, 0xff);
		sceGuEnable(GU_ALPHA_TEST);
	//
		sceGuEnable(GU_DEPTH_TEST);
		sceGuDepthRange(0xc350, 0x2710);
		sceGuDepthFunc(GU_GEQUAL);
	//
		sceGuEnable(GU_CULL_FACE);
		sceGuFrontFace(GU_CW);
	//
		sceGuShadeModel(GU_SMOOTH);
	//
		sceGuEnable(GU_BLEND);
	//
		sceGuEnable(GU_TEXTURE_2D);
	//
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	//
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
//		float u = 1.0f / ((float)source->texture_width);
//		float v = 1.0f / ((float)source->texture_height);
//		sceGuTexScale(u, v);
		sceGuTexScale(1.0f, 1.0f);
		sceGuAmbientColor(0xffffffff);
	//
	//	sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	//
		sceGuEnable(GU_CLIP_PLANES);
		sceGuFinish();
		sceGuSync(0, 0);

		sceDisplayWaitVblankStart();
		sceGuDisplay(GU_TRUE);
	}

}
#endif /*(1==USE_FAKE_LOADING)*/
