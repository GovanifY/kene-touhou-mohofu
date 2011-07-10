
/*---------------------------------------------------------
	もやもやエフェクト
	-------------------------------------------------------
	最新の研究(?)の結果、もやもや部分は1パスで描画してなくて、
	少なくとも2パスで描画している事が判った。
	-------------------------------------------------------
	ポーズフィルターの研究
	１．色
	紅でポーズ画面のスクショを取ると白い部分が水色っぽくなる。
	ここで白==RGB(0xff, 0xff, 0xff)とすると、
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
---------------------------------------------------------
	とりあえず、よくわからんので、4点サンプリングによる近傍フィルタをかけてみた。
src1	■■□□□
src2	■□■□□
		□□□□□
-----------------------[パス1]の処理	p1,p2,p3,p4 を平均してp1に描きこむ。
src1	p1 p2 --
src2	p3 -- p4
-----------------------[パス2]の処理	p1,p2,p3,p4 を平均してp4に描きこむ。
src1	p1 -- p2
src2	-- p3 p4
-----------------------
		全画面処理を2回繰り返す。
		ただし、x端の2dot及び、y端の1dotは処理しない。
	*/

	/* パネル部分を除く */		/* GAME_WIDTH==352 == 480-128 */

//	#define MY_FILTER_WINDOW_SIZE_X_03_DOT	(3)
//	#define MY_FILTER_WINDOW_SIZE_Y_02_DOT	(2)
	#define MY_FILTER_X_SIZE_350_DOT	(GAME_WIDTH-(2))/*(xは2[dot]処理しない)*/
	#define MY_FILTER_Y_SIZE_271_DOT	(GAME_HEIGHT-(1))/*(yは1[dot]処理しない)*/

/* 仮想スクリーン(SDL_00_VIEW_SCREEN)にもやもやエフェクトをかける */
global void psp_pause_filter(void)
{
	#if (1)/* 1==全画面にエフェクトをかける。 0==SDL部分のみエフェクトをかける。 */
	s_gu_save_screen();/* vramからSDL画面に強制コピー */
	#endif
	if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ロック */
	{
		// psp_BGR565 == bbbb bggg gggr rrrr
		#define MY_16_G_(aaa) ( (((aaa&0x07e0) )) )
		#define MY_16_BR(aaa) ( (((aaa&0xf81f) )) )
		{/*[パス1]上から処理する。*/
			u16 *src1;	src1 = render_image;								/* 始めのライン */
			u16 *src2;	src2 = render_image + (PSP_BUFFER_WIDTH512);		/* 1ライン下 */
			int jj;
			for (jj=0; jj<(MY_FILTER_Y_SIZE_271_DOT); jj++)/*(yは1[dot]処理しない)*/
			{
				int ii;
				for (ii=0; ii<(MY_FILTER_X_SIZE_350_DOT); ii++)/*(xは2[dot]処理しない)*/ 	/* パネル部分を除く */
				{
					u16 pixel1;
					u16 pixel2;
					u16 pixel3;
					u16 pixel4;
					pixel1 = (*(src1  ));
					pixel2 = (*(src1+1));
					pixel3 = (*(src2  ));
					pixel4 = (*(src2+2));
					{
						u32 g_1;
						u32 br1;
						g_1 = (((MY_16_G_(pixel1))+(MY_16_G_(pixel2))+(MY_16_G_(pixel3))+(MY_16_G_(pixel4)))>>2);
						br1 = (((MY_16_BR(pixel1))+(MY_16_BR(pixel2))+(MY_16_BR(pixel3))+(MY_16_BR(pixel4)))>>2);
						//
						*src1 = (
							   ((br1 ) & 0xf81f)		/* 16bit RGBA5650 として青赤 */
							| (((g_1 ) & 0x07e0)   )	/* 16bit RGBA5650 として緑 */
							);	/* 合成 */
					}
					src1++;
					src2++;
				}
				src1 += (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* 残り */ 	/*(xは2[dot]処理しない)*/
				src2 += (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* 残り */ 	/*(xは2[dot]処理しない)*/
			}
			/*[パス2]下から処理する。*/
			{	/*(最後に処理した移動分は余計なので相殺させる。同時に1[dot]余計なのでその分も消す。)*/
				src1 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT)+(1));/* 残り */ /* 引く */
				src2 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT)+(1));/* 残り */ /* 引く */
			}
			for (jj=0; jj<(MY_FILTER_Y_SIZE_271_DOT); jj++)/*(yは1[dot]処理しない)*/
			{
				int ii;
				for (ii=0; ii<(MY_FILTER_X_SIZE_350_DOT); ii++)/*(xは2[dot]処理しない)*/ 	/* パネル部分を除く */
				{
					u16 pixel1;
					u16 pixel2;
					u16 pixel3;
					u16 pixel4;
					pixel1 = (*(src1  ));
					pixel2 = (*(src1+2));
					pixel3 = (*(src2+1));
					pixel4 = (*(src2+2));
					{
						u32 g_1;
						u32 br1;
						g_1 = (((MY_16_G_(pixel1))+(MY_16_G_(pixel2))+(MY_16_G_(pixel3))+(MY_16_G_(pixel4)))>>2);
						br1 = (((MY_16_BR(pixel1))+(MY_16_BR(pixel2))+(MY_16_BR(pixel3))+(MY_16_BR(pixel4)))>>2);
						//
						*(src2+2) = (
							   ((br1 ) & 0xf81f)		/* 16bit RGBA5650 として青赤 */
							| (((g_1 ) & 0x07e0)   )	/* 16bit RGBA5650 として緑 */
							);	/* 合成 */
					}
					src1--;
					src2--;
				}
				src1 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* 残り */ 	/*(xは2[dot]処理しない)*/
				src2 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* 残り */ 	/*(xは2[dot]処理しない)*/
			}
		}
	}
	/*(色調、変換処理)*/
	{
		u16 *src;	src = render_image;
		int jj;
		for (jj=0; jj<(GAME_HEIGHT); jj++)
		{
			int ii;
			for (ii=0; ii<(GAME_WIDTH); ii++)		/* パネル部分を除く */		/* GAME_WIDTH==352 == 480-128 */
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
			src += (PSP_BUFFER_WIDTH512-PSP_WIDTH480+PANEL_WIDTH);/* 残り */
		}
	}
	if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ロック解除 */
}
