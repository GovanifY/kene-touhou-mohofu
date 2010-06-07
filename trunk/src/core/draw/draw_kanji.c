
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風  〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	漢字ウィンドウ処理
---------------------------------------------------------*/

#include "kanji_system.h"

#include "111_my_file.h"/*is_kanji_1st()*/



#define FONT_WIDTH			(8)
#define FONT_HEIGHT 		(16)

/*
現実の問題として、東雲フォント16には
2バイト半角漢字はフォントデーターがない(作成されてない。作成すれば使える)
そのため 2バイト半角漢字は表示されない。(データーが全て空白の為)
//
その上で、根源的な問題として、pspで2バイト半角漢字(==NEC9800シリーズ依存文字)を
サポートする意味がない気がする。
//
	http://www.wdic.org/w/WDIC/%E3%82%B7%E3%83%95%E3%83%88JIS
//
だめ文字(シフトJISの2バイト目が0x5c、すなわち'\'('＼')である文字のこと。)
	ソ噂浬欺圭構蚕十申曾箪貼能表暴予禄兔喀媾彌拿杤歃濬畚秉綵臀藹觸軆鐔饅鷭
*/

/* USE_HARF_KANJI == (0): shift jisコード、$fdxx の2バイト半角漢字をサポートしない場合 */
/* USE_HARF_KANJI == (1): shift jisコード、$fdxx の2バイト半角漢字をサポートする場合 */
#define USE_HARF_KANJI		(0)
#if (1==USE_HARF_KANJI)
/*---------------------------------------------------------
	文字種がshift jis漢字と判明している場合に、
	(つまりis_kanji()判定後の場合専用)
	2バイト半角かどうかを文字単位で返す子関数
---------------------------------------------------------*/

static int size_of_kanji(/*unsigned char*/int high_byte)
{
	high_byte &= 0xff;
	if (0xfd == high_byte)	// 2バイト半角
	{			return (1); 	}
	else	{	return (2); 	}
}
#endif

/*---------------------------------------------------------
	東雲文字コード規格 の文字を描画する子関数
---------------------------------------------------------*/

extern const char shinonome_font16p[];
/*
	x				表示座標
	y				表示座標
	col_n			フォントの種類
	code_offset 	文字
//
	dx		ドットx
	dy		ドットy
	bit 	調べるbyte情報
	bitcnt	調べてるbyteが後何回調べられるか:最大8回(後述)
			bitと&することで調べてるbyteのbit情報を得る。
*/

static int font_color_number;


/*
	文字を描画する描画先のテクスチャー
*/
extern UINT16 *kanji_window_screen_image;						/* メッセージウィンドウ(画像) */


#if (0)
//512static int font_bg_bitmap_surface_pitch;
//static int font_bg_bitmap_surface_width;
#endif

/*
	drawmap 			描き込み先(書き込み先ならsource(元)じゃなくてdestination(先))
	rect				描き込み領域
	str 				描き込む文字列
	color_type			描き込む書体の色を指定する画像番号
	wait				1字づつ描く場合に、1つの文字に掛ける時間
						0==wait の場合は一回で全文字描画する
						文字列を描いている途中から 0に変更することも可能
//
	my_string_offset	文字アドレスのオフセット / 0 の場合は 初期化フラグ
	STR_LIMIT_MAX_X_WIDTH	横に並べられる文字のドット数
*/

//#define BG_FONT_HAIKEI_LENGTH ((512*4))	/* 4[laster](半分)	*/
#define BG_FONT_HAIKEI_LENGTH ((512*8)) 	/* 8[laster](16x16ドットフォントは(width==16x16==横256)なので、横512換算で8[laster]分) */

//#define BG_FONT_HAIKEI_OFFSET ((512*16*2))	/* 16[laster] */
#define BG_FONT_HAIKEI_OFFSET ((512*10*2))		/* 10[laster]==(8+1+1) */

static void s_draw_shinonome_moji(
	int x,
	int y,
	int code_offset,
	int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=hidari_gawa)
	{
		x += FONT_WIDTH;
	}
	if (x > (/*drawmap->w*/(320)/*drawmap_width*/-FONT_WIDTH) ) 	/* 範囲外なら */
	{
		return;/* 描画しない */
	}
	int haikei_offset;
//	if (1==hidari_gawa) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=hidari_gawa)
	{
		code_offset++;
		haikei_offset=8;
	}
	else
	{
		haikei_offset=0;
	}
	haikei_offset += (font_color_number<<4);
	code_offset<<=4;
	int bit;
	int bitcnt;
	bitcnt = 0;
	bit = 0;
	int dy;
	for (dy=0; dy<FONT_HEIGHT; dy++)
	{
		int dx;
		for (dx=0; dx<FONT_WIDTH; dx++)
		{
			bitcnt >>= 1;
			if (0==bitcnt)	// (0==bitcnt)の時
			{
				bit = shinonome_font16p[code_offset++];
				bitcnt = 0x80;		// 1byteの情報量が8の為 80 40 20 10 8 4 2 1(0になったらすぐ80になるためカウントされない)
			}
			if (bit & bitcnt)
			{
				#if 1
				/*
					いまいち良くわかんない(が、とりあえず動く)。
					アドレスはバイト単位なので u8 ポインタでＣ言語に計算させてる。
					(short なら2で割るとか、int なら4で割るとか)
					変える場合は、定数(BG_FONT_HAIKEI_OFFSET等)や変数(dy,y,他)も変えないとダメ。
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/kanji_window_screen_image)	/*surface*/
					+ (dy * (512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx)
					+ (haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(kanji_window_screen_image)/*surface->pixels*/
					+ (((dy+y))*(512*2))/*surface->pitch*/
					+ (dx+dx)
					+ (x+x)
					+ (BG_FONT_HAIKEI_OFFSET);	/* 512[dot]x 2[bytes](short) */
				*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				#endif
			}
		}
	}
}
			//	u8 *p = (u8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;

/*---------------------------------------------------------
http://openlab.ring.gr.jp/efont/shinonome/
	shift jis文字コード規格 を 東雲文字コード規格 へ変換後、描画する
---------------------------------------------------------*/

static void s_draw_sjis_kanji(int x, int y, int high_byte, int low_byte)
{
	if (0==high_byte)	/* 半角文字の場合(asciiコード) */
	{
		/* asciiコード → shinonomeコード 変換 */
		static	const	u8/*int*/ sinonome_ascii_bank_offset[16] =
		{// +0	  +1	+2	  +3	 +4    +5	 +6    +7	  +8	+9	  +a	+b	   +c	 +d    +e	 +f
			0xff, 0xff, 0x00, 0x10,  0x20, 0x30, 0x40, 0x50,  0xff, 0xff, 0x60, 0x70,  0x80, 0x90, 0xff, 0xff
		};
	//		 if (low_byte < 0x20)	{	return; }				// ascii 0x00-0x1f
	//	else if (low_byte < 0x80)	{	low_byte -= 0x20; } 	// ascii 0x20-0x7f
	//	else if (low_byte < 0xa0)	{	return; }				// ascii 0x80-0x9f
	//	else if (low_byte < 0xe0)	{	low_byte -= 0x40; } 	// ascii 0xa0-0xdf
	//	else						{	return; }				// ascii 0xe0-0xff
	//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// ここでの9は多分1つの文字に使うバイト数。(1byte == 8bit)*9=72 == 6*12
		u8 idx8;
		idx8 = sinonome_ascii_bank_offset[((low_byte>>4) & 0x0f)];
		/* 存在マップが 0xff の場合は、 東雲文字コード規格 にない文字なので表示できない */
		if (0xff == idx8)
		{
			return;
		}
		idx8 = ((idx8)|((low_byte & 0x0f)));
		s_draw_shinonome_moji(x, y, (idx8), 0);/*low_byte<<4*/
	}
	else				/* 全角文字の場合(shift jisコード) */
	{
		#if 1
		/*下位バイトのチェック*/
		if (low_byte < 0x40)	// 0x40未満(0x00-0x3f)の場合は shift jis文字コード領域 ではない。
		{
			return;
		}
		#endif
		low_byte -= 0x40;
		/* 上位バイトから 東雲文字コード規格 のインデックスを得る */
		int idx;
		#define NN (-1)
		{
			/* 東雲文字コード規格 の存在マップ */
			//#define FDxx (47)/* 半角2バイト漢字文字 */
			static	const	signed char/*int*/ sinonome_sjis_bank_offset[0x40] =
			{// +0	+1	+2	+3	 +4  +5  +6  +7   +8  +9  +a  +b   +c  +d  +e  +f
				NN,  0,  1,  2,   3, NN, NN,  4,   5,  6,  7,  8,	9, 10, 11, 12,		/* 0x80xx */
				13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,  25, 26, 27, 28,		/* 0x90xx */
				29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, NN,  NN, NN, NN, NN,		/* 0xe0xx */
				NN, NN, NN, NN,  NN, NN, NN, NN,  NN, NN, NN, NN,  NN, NN, NN, NN		/* 0xf0xx */
			//	以下の領域は東雲フォント16にはフォントデーターがない(eaa4が最終データーだった)
			//	29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, NN,  NN, 40, 41, NN,		/* 0xe0xx */	東雲フォント16にはフォントデーターがない
			//	NN, NN, NN, NN,  NN, NN, NN, NN,  42, 43, 44, 45,  46, 47, NN, NN		/* 0xf0xx */	東雲フォント16にはフォントデーターがない
			};
			idx = sinonome_sjis_bank_offset[high_byte & 0x3f];
		}
		/* 存在マップが NN の場合は、 東雲文字コード規格 にない文字なので表示できない */
		if (NN == idx)
		{
			return;
		}
		#undef NN
		//
		idx = (16*10)/*半角文字をスキップ*/ + (idx*2*192)/*codetbl１個あたり192文字*/;
		#if (1==USE_HARF_KANJI)
		if (1 == size_of_kanji(high_byte))	// 2バイト半角
		{
			idx += (low_byte);
		//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// ここでの9は多分1つの文字に使うバイト数。(1byte == 8bit)*9=72 == 6*12
			s_draw_shinonome_moji(x, y, (idx), 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2バイト全角
		#endif
		{
			idx += (low_byte+low_byte);/* x2 */
			s_draw_shinonome_moji(x, y, (idx), 0); /* 右半分 */
			s_draw_shinonome_moji(x, y, (idx), 1); /* 左半分 */
		}
	}
}

/*---------------------------------------------------------
	文字列の描画
---------------------------------------------------------*/
// static extern int cursor_continue;	/* カーソル継続 */
static int cursor_x;					/* カーソル位置 */
static int cursor_y;					/* カーソル位置 */


static int my_string_offset;/*=0*/
/*static*/global void clear_my_string_offset(void)
{
	my_string_offset = 0;/* 初期化 */
}

/*---------------------------------------------------------
	カーソルをホームポジションへ移動
---------------------------------------------------------*/
/*static*/global void home_cursor(void)
{
	cursor_x = 0;
	cursor_y = 0;
}

/*---------------------------------------------------------
	メッセージウィンドウの内容を消す。
---------------------------------------------------------*/

/*static*/global void kanji_window_clear(void)
{
//	#define TEST_OFFSET (/*0*/512*10/2) /* 10 [laster? ] */
	#define TEST_OFFSET (0) 	/* put_pixel使うから(0) */
	{
	//	u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(3/*3 2[行]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+(BG_FONT_HAIKEI_OFFSET);	/* 512[dot]x 2[bytes](short) */
			*(u16 *)p = 0/*pixel*/;
		}
	}
}
			//	ccc_putpixel16(dx);//, 0, /*0xaaaa*/(0x0000)/*dot color16 */);
		//	*(u16 *)p = 0xaa55;
		//	p++;
		//	u8 *p = (u8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;

	#if 0/*test*/
	{
		int kk;
		for (kk=0; kk<(BG_FONT_HAIKEI_LENGTH)/*6*/; kk++)
		{
			(*(kanji_window_screen_image+kk)) = (*(font_bg_bitmap_surface_image+kk));/*BG_FONT_HAIKEI_OFFSET*/
		}
	}
	#endif

#if 0
/*---------------------------------------------------------
	外部からメッセージウィンドウに漢字を描画する場合のリセット
---------------------------------------------------------*/

global /*static*/ void script_message_window_clear(void)
{
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
//	msg_window_init();/*???*/

//	inits				= 1;/*???*/
//	is_bg				= 0;/*???*/
//	draw_script_screen	= 0;/*???*/

	home_cursor();		/* カーソルをホームポジションへ移動 */
//	cursor_x_chached	= 0;/*???*/ /* カーソル初期化 */
//	cursor_y_chached	= 0;/*???*/ /* カーソル初期化 */
//	clear_my_string_offset();/*???*/

	//set_write_text(0);		/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
}
#endif

//static void msg_window_init(void)
//{
//
//}
/*---------------------------------------------------------
	shift jis文字 漢字かなまじり書体の文字列を描画する
	 (wait指定で、frame毎に一文字描画。 wait0で、続きを全描画)
---------------------------------------------------------*/

global /*static*/ int print_kanji000(const char *str, int color_type, int wait)
{
	font_color_number = (color_type & 0x0f);
//
	int terminate_this_frame;	terminate_this_frame=0;
	int need_draw_this_flame;	need_draw_this_flame=0;
	{	/* wait が 0 の場合は一回で全文字描画する */
		static unsigned int count_wait_time;
		count_wait_time++;
		if (count_wait_time > wait)
		{
			count_wait_time = 0;
			need_draw_this_flame = 1;
		}
	}
//	if (1==need_draw_this_flame)	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=need_draw_this_flame)
	{
			#define IS_LOCK_LOCAL_SCREEN (0)/* 読み込みだけなら、ロックする必要はない(?) */
			#if (1==IS_LOCK_LOCAL_SCREEN)
			if (SD L_MUSTLOCK(font_bg_bitmap_surface))	{	SD L_LockSurface(font_bg_bitmap_surface);	}	/* ロック */
			#endif
		loop:
		;
		/*unsigned char*/int high_byte;
		high_byte = (*(str+my_string_offset) & 0xff);
		if (0x00 != high_byte )
		{
			int xx;
			int yy;
			xx = 0/*(rect->x)*/+(cursor_x)*(FONT_WIDTH);
			yy = 0/*(rect->y)*/+(cursor_y)*(FONT_HEIGHT+2);
		//	パフォーマンスが低下するのでなるべくロックしない。(ロックは最小限に)
		/* 旧ロック */
			if (is_kanji_1st(high_byte)) /* shift jis 漢字 */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(str+my_string_offset+1) & 0xff);
				s_draw_sjis_kanji(xx, yy, high_byte, low_byte);
				#if (1==USE_HARF_KANJI)
				my_string_offset  += size_of_kanji(high_byte);	/* 拡張半角文字領域 fdxx の場合は半角になる */
				cursor_x		  += size_of_kanji(high_byte);	/* 拡張半角文字領域 fdxx の場合は半角になる */
				#else
				my_string_offset  += (2);
				cursor_x		  += (2);
				#endif
			}
			else				/* ascii文字 */
			{
				if ('\\' == high_byte)		/* エスケープシークエンス処理(escape sequence) */
				{
					/*unsigned char*/int escape_sequence = (*(str+my_string_offset+1) & 0xff);
					if ('n' == escape_sequence) 	/* 改行コード */
					{
						my_string_offset += 2;
						cursor_x = 999;/* 適当に大きな値を指定して改行させる */
					//	cursor_x=0;
					//	cursor_y++;
					}
				}
				else					/* 半角文字 */
				{
					s_draw_sjis_kanji(xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x++;
				}
			}/* 旧ロック解除 */
			if (0==(color_type & STR_CODE_NO_ENTER))
			{
				/* 改行処理 */
				#define STR_LIMIT_MAX_X_WIDTH ((int)((300/*(rect->w)*/)/FONT_WIDTH)+2)
				if ( (STR_LIMIT_MAX_X_WIDTH-1) < cursor_x)
				{
					cursor_x=0;
					cursor_y++;
					#define MAX_cursor_y (2)
					if (MAX_cursor_y < cursor_y)
					{
						cursor_y = 0;
					//	home_cursor();
						kanji_window_clear();
					}
				}
			}
		//	terminate_this_frame=0; 	/* continue frame */
			if (0 == wait)	/* wait が 0 の場合は一回で全文字描画する */
			{
				goto loop;	//loop処理
			}
		}/*while*/
		else			//終了処理
		{
			terminate_this_frame = 1;	/* end frame */
		}
			#if (1==IS_LOCK_LOCAL_SCREEN)
			if (SD L_MUSTLOCK(font_bg_bitmap_surface))	{	SD L_UnlockSurface(font_bg_bitmap_surface); 	}	/* ロック解除 */
			#endif
	//	if (1==terminate_this_frame)	//終了処理	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=terminate_this_frame)	//終了処理
		{
			{
			//	STR_LIMIT_MAX_X_WIDTH=1/* 0 (割り算なので0は危険) */;/*???*/
				my_string_offset = 0;	/* 次回は、初期化 */
				//cursor_continue=0;	/* 次回は、カーソル継続しない */
			}
		}
	}
	return (terminate_this_frame);		/* 0:continue, 1:end */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#include "graphics00.h"
global void kanji_system_init(void)/* 組み込み */
{
	/* メッセージ表示用RAM確保 */	/* 現在表示が16bit色なので(32bit色表示なら変わる) */
	/* 背景用に10[laster]余分に確保 */
//	kanji_window_screen_image	= (UINT16 *)malloc((512*(10+64)*2));	/* (10+ 64)[laster] */
	kanji_window_screen_image	= (UINT16 *)malloc((512*(10+128)*2));	/* (10+128)[laster] */
//
	{
	/* 文字背景用画像を文字描画ワークの先頭に読み込み、画像をコピーしてから、開放する。 */
	#if 1
	/*static*/ /*SDL_Surface*/my_image *font_bg_bitmap_surface;/*=NULL*/
	//	{
	//		char fn[128/*64*/];
	//		strcpy(fn, DIRECTRY_NAME_DATA_STR "/fonts/" "font_bg16.png" );
	//		font_bg_bitmap_surface = IM G_Load(fn);/*ここでロードすると確実に処理落ちするよ*/
	//		font_bg_bitmap_surface = IM G_Load(DIRECTRY_NAME_DATA_STR "/fonts/" "font_bg16.png");/*ここでロードすると確実に処理落ちするよ*/
			font_bg_bitmap_surface = png_load_my_image(DIRECTRY_NAME_DATA_STR "/fonts/" "font_bg16.png");/*ここでロードすると確実に処理落ちするよ*/
	//	}
	//	font_bg_bitmap_surface			= SDL_DisplayFormat(font_bg_bitmap_surface);/*サーフェスを表示フォーマットに変換する。*/
	//	font_bg_bitmap_surface_pitch	= font_bg_bitmap_surface->pitch;
		#endif
		#if 1
		{
			/* 現在表示が16bit色なので、32->16変換する。(32bit色表示なら変わる) */
		//	/*static*/ UINT16 *font_bg_bitmap_surface_image;
		//	font_bg_bitmap_surface_image	= font_bg_bitmap_surface->pixels;
			u32 *pixsrc;
			u16 *pixdst;
			pixsrc = (void *)((font_bg_bitmap_surface->pixels));
			pixdst = (kanji_window_screen_image);
			trans_format16(pixsrc, pixdst, (BG_FONT_HAIKEI_LENGTH));
		}
		#endif
		/* 文字背景用画像を開放する。 */
	//	SDL_FreeSurface(font_bg_bitmap_surface);
		png_free_my_image(font_bg_bitmap_surface);
	}

}

/*---------------------------------------------------------
	終了時に開放する部分...何だけど、
	ハングアップしたりするので、取り敢えず無効になってる。
	(開放しなくても終了すれば、OSがメモリ回収するので問題ないって言えば問題ない)
---------------------------------------------------------*/

global void kanji_system_exit(void)/* 外す */
{
	/*msg_window_init()*/
//
	#if 0/* 本当は要る */
//	kanji_system_terminate();
	if (NULL != kanji_window_screen_image)		{	free(kanji_window_screen_image);					kanji_window_screen_image = NULL;	}
	#endif
}
