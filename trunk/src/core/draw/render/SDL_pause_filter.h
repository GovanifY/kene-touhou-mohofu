
/*---------------------------------------------------------
	もやもやエフェクト
	作ろうかと思ったけど、作ってない
	-------------------------------------------------------
	とりあえず青くなる。
	-------------------------------------------------------
	ポーズフィルターの研究
	１．色
	紅でポーズ画面のスクショを取ると白い部分が水色っぽくなる。
	ここで白==RGB(0xff,0xff,0xff)とすると、
水色R == (白G * 0.863) / 16;
水色G == ((白R * 0.863)*0.863);
水色B == (白B * 0.863);
かもしれない。
	-------------------------------------------------------
 R G B	 R G B
 0 0 1	 0 0 1
 1 0 0	 0 1 0
 1 1 1	 0 1 1

R	  G 	B
253   253	253   == 白
0xfd  0xfd	0xfd

13	  190	221   == 水色
0x0d  0xbe	0xdd

 x 1.158...

0x0f  0xdd 0xff



1.15837104072398190045248868778281 == 256/221
0.86328125% == 221/256

13/16 == 0.8125

---------------------------------------------------------*/

/* 仮想スクリーン(SDL_00_VIEW_SCREEN)にもやもやエフェクトをかける */
void psp_pause_filter(void)
{
	#if 1/* 1==全画面にエフェクトをかける。 0==SDL部分のみエフェクトをかける。 */
	gu_save_screen();/* vramからSDL画面に強制コピー */
	#endif
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ロック */
	#if (0!=USE_DEBUG_PAUSE_FILTER)
	#else
		#if 1/*(r33)*/
	{
		u16 *src;	src = render_image;
		int jj;
		for (jj=0; jj<(272/*-1 (もやもやエフェクトにするなら'-1'が要る) */); jj++)
		{
			int ii;
			for (ii=0; ii<(352); ii++)		/* パネル部分を除く */		/* 352 == 480-128 */
			{
				u16 pixel;
				pixel = *src;
				if (0 != pixel)
				{
					pixel = (
						   (pixel & 0xf800) 		/* 16bit RGBA5650 として青のみ(テスト) */
						| ((pixel & 0x0700)>>8) 	/* 16bit RGBA5650 として緑を 落として赤に */
						| ((pixel & 0x001f)<<5) 	/* 16bit RGBA5650 として赤を 落として緑に */
						);	/* テキトー合成 */
					/* 青補正 */
					if (0==pixel)
					{
						pixel = 0x0800;
					}
					*src = pixel/*0x4444*/;
				}
				src++;
			}
			src += (512-480+128);/* 残り */
		}
	}
		#endif/*(r33)*/
		#if 0/*(r32)*/
	{
		u16 *src;	src = render_image;
		int jj;
		for (jj=0; jj<(272/*-1 (もやもやエフェクトにするなら'-1'が要る) */); jj++)
		{
			int ii;
			for (ii=0; ii<(352); ii++)		/* パネル部分を除く */		/* 352 == 480-128 */
			{
				u16 pixel;
				pixel = *src;
				if (0 != pixel)
				{
/* 16bit RGBA5650 専用 */
	/*
	16: bbbb bggg gggr rrrr
	16: bbbb -ggg gg-r rrr-
		1111 0111 1101 1110 == 0xfede;
		0000 1000 0010 0001 == 0x0821; LSB
	*/
		//			pixel &= 0xfede;	/* 16bit RGBA5650 として最下位ビットをマスク */
					pixel &= 0xf800;	/* 16bit RGBA5650 として青のみ(テスト) */
			//		pixel >>= 1;		/* 輝度半減 */
					//
			//		u16 *src2;	src2 = src+(1024/*512*/);/*1024 [bytes] == 512[words(u16)] x 2 */
			//		u16 pixel2;
			//		pixel2 = *src2;
			//		pixel2 &= 0xfede;	/* 16bit RGBA5650 として最下位ビットをマスク */
			//		pixel2 >>= 1;		/* 輝度半減 */
					//
					pixel = (pixel /*| pixel2 | 0x0821*/);	/* テキトー合成 */
					/* 青補正 */
					if (0==pixel)
					{
						pixel = 0x0800;
					}
					*src = pixel/*0x4444*/;
				}
				src++;
			}
			src += (512-480+128);/* 残り */
		}
	}
		#endif/*(r32)*/
	#endif /* (USE_DEBUG_PAUSE_FILTER) */
	//#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
	/* 将来Guで描いた場合。ハードウェアー機能で、置き換えられるので今のうちにまとめとく */
	//SDL_FillRect(sdl_screen[SDL_00_VIEW_SCREEN], NULL, 0/*SD L_MapRGB(sdl_screen[SDL_00_VIEW_SCREEN]->format,0,0,0)*/);
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ロック解除 */
}
