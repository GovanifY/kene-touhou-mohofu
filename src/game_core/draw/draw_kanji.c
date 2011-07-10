
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	漢字ウィンドウ処理
---------------------------------------------------------*/

#include "kanji_system.h"

#include "111_my_file.h"/*is_kanji_1st()*/


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


	//#define BG_FONT_HAIKEI_LENGTH 	((512*4))		/* 4[laster](半分)	*/
	#define BG_FONT_HAIKEI_LENGTH		((512*8))		/* 8[laster](16x16ドットフォントは(width==16x16==横256)なので、横512換算で8[laster]分) */

	//#define BG_FONT_HAIKEI_OFFSET 	((512*16))		/* 16[laster] */
	#define BG_FONT_HAIKEI_OFFSET		((512*10))		/* 10[laster]==(8+1+1) */
//	#define BG_FONT_HAIKEI_OFFSET_U16	((BG_FONT_HAIKEI_OFFSET*2)) 	/* 10[laster]==(8+1+1) */

static void s_draw_shinonome_moji(
	int x,
	int y,
	int code_offset,
	int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=hidari_gawa)
	{
		x	+= KANJI_FONT_08_HARF_WIDTH;
	}
	if (x > (/*drawmap->w*/(320)/*drawmap_width*/-KANJI_FONT_08_HARF_WIDTH) )	/* 範囲外なら */
	{
		return;/* 描画しない */
	}
	int haikei_offset;
//	if (1==hidari_gawa) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=hidari_gawa)
	{
		code_offset++;
		haikei_offset	= 8;
	}
	else
	{
		haikei_offset	= 0;
	}
	haikei_offset	+= (font_color_number<<4);
	code_offset 	<<= 4;
	int bit;
	int bitcnt;
	bitcnt	= 0;
	bit 	= 0;
	unsigned int dy;
	for (dy=0; dy<KANJI_FONT_16_HEIGHT_P0; dy++)
	{
		unsigned int dx;
		for (dx=0; dx<KANJI_FONT_08_HARF_WIDTH; dx++)
		{
			bitcnt	>>= 1;
			if (0==bitcnt)	// (0==bitcnt)の時
			{
				bit 	= shinonome_font16p[code_offset++];
				bitcnt	= 0x80; 	// 1byteの情報量が8の為 80 40 20 10 8 4 2 1(0になったらすぐ80になるためカウントされない)
			}
			if (bit & bitcnt)
			{
				#if (0==USE_32BIT_DRAW_MODE)
				/*
					いまいち良くわかんない(が、とりあえず動く)。
					アドレスはバイト単位なので u8 ポインタでＣ言語に計算させてる。
					(short なら2で割るとか、int なら4で割るとか)
					変える場合は、定数(BG_FONT_HAIKEI_OFFSET_U16等)や変数(dy,y,他)も変えないとダメ。
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/cb.kanji_window_screen_image)	/*surface*/
					+ (dy * (512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx)
					+ (haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(cb.kanji_window_screen_image)/*surface->pixels*/
					+ (((dy+y))*(512*2))/*surface->pitch*/
					+ (dx+dx)
					+ (x+x)
					+ (BG_FONT_HAIKEI_OFFSET*2);	/* 512[dot]x 2[bytes](short) */
				*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				#else
				/*
					いまいち良くわかんない(が、とりあえず動く)。
					アドレスはバイト単位なので u8 ポインタでＣ言語に計算させてる。
					(short なら2で割るとか、int なら4で割るとか)
					変える場合は、定数(BG_FONT_HAIKEI_OFFSET_U16等)や変数(dy,y,他)も変えないとダメ。
				*/
			//	s_getpixel16
			volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/cb.kanji_window_screen_image)	/*surface*/
					+ (dy * (2*512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
					+ (dx+dx+dx+dx)
					+ (haikei_offset+haikei_offset+haikei_offset+haikei_offset);
			//	putpixel16
			volatile u8 *dst_p = (u8 *)(cb.kanji_window_screen_image)/*surface->pixels*/
					+ (((dy+y))*(2*512*2))/*surface->pitch*/
					+ (dx+dx+dx+dx)
					+ (x+x+x+x)
					+ (BG_FONT_HAIKEI_OFFSET*4);	/* 512[dot]x 2[bytes](short) */
			//	*(u16 *)dst_p = (u32)(*(u16 *)src_p);
				*(u32 *)dst_p = (u32)(*(u16 *)src_p);
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
		idx8	= sinonome_ascii_bank_offset[((low_byte>>4) & 0x0f)];
		/* 存在マップが 0xff の場合は、 東雲文字コード規格 にない文字なので表示できない */
		if (0xff == idx8)
		{
			return;
		}
		idx8	= ((idx8)|((low_byte & 0x0f)));
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
		idx 	= (16*10)/*半角文字をスキップ*/ + (idx*2*192)/*codetbl１個あたり192文字*/;
		#if (1==USE_HARF_KANJI)
		if (1 == size_of_kanji(high_byte))	// 2バイト半角
		{
			idx 	+= (low_byte);
		//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// ここでの9は多分1つの文字に使うバイト数。(1byte == 8bit)*9=72 == 6*12
			s_draw_shinonome_moji(x, y, (idx), 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2バイト全角
		#endif
		{
			idx 	+= (low_byte+low_byte);/* x2 */
			s_draw_shinonome_moji(x, y, (idx), 0); /* 右半分 */
			s_draw_shinonome_moji(x, y, (idx), 1); /* 左半分 */
		}
	}
}

/*---------------------------------------------------------
	独立ライン関連
	-------------------------------------------------------
	「マルチライン」と「独立ライン」。
	まだ作ってないけど、「マルチライン」と「独立ライン」両方の形式で
	の描画に対応する予定です。
	現在は「マルチライン」で「３行」しかありません。
	「独立ライン」方式に移行して、「マルチライン」は廃止する方向で考えています。
	-------------------------------------------------------
	「独立ライン」方式というのは、(漢字をレンダリングする)
	描画バッファは、現在の「マルチライン」と同じ方式です。
	(描画バッファは連続している)
	-------------------------------------------------------
	「マルチライン」と違うのは、描画する場合に、
	行ごとに(dot単位でx,y)位置が指定できる方式です。
	(字ごとには位置指定できません)
---------------------------------------------------------*/
static unsigned int default_window_locate_x;
static unsigned int default_window_locate_y;
static unsigned int default_window_font_tate_kankaku;
static unsigned int default_window_max_line;
global void set_kanji_origin_xy(unsigned int set_x_offset_dot, unsigned int set_y_offset_dot)
{
	default_window_locate_x = (set_x_offset_dot);/* X位置 */
	default_window_locate_y = (set_y_offset_dot);/* Y位置 */
}
global void set_kanji_origin_kankaku(unsigned int set_y_offset_dot)
{
	default_window_font_tate_kankaku = (set_y_offset_dot);/* Y位置 */
}
global void set_kanji_hide_line(unsigned int set_max_line_num)
{
	default_window_max_line = set_max_line_num;
	unsigned int yyy18;
	yyy18 = (default_window_locate_y);/* Y位置 */
	//
	unsigned int i;
	for (i=0; i<(set_max_line_num); i++)
	{
		ml_font[i].x		= (default_window_locate_x);/* X位置 */
		ml_font[i].y		= (yyy18);
		ml_font[i].timer	= ML_ON;
//		ml_font[i].render	= NULL;
//		ml_font[i].str		= my_font_text;/*(デバッグ)*/
		yyy18 += (18);
	}
	for (; i<(ML_LINE_99_MAX); i++)
	{
		ml_font[i].x		= (32);/* X位置 */
		ml_font[i].y		= (32);
		ml_font[i].timer	= ML_OFF;
//		ml_font[i].render	= NULL;
//		ml_font[i].str		= NULL;
	}
	cg.msg_time = (65536);	/* 約 18 分 */
}

/*---------------------------------------------------------
	マルチライン関連
---------------------------------------------------------*/

/*---------------------------------------------------------
	文字列の描画
---------------------------------------------------------*/
// static extern int cursor_continue;	/* カーソル継続 */
static int cursor_x_dots;				/* カーソル位置[dot] */
static int cursor_y_lines;				/* カーソル位置[dot] (マルチラインの場合行数 / 独立ラインの場合指定行) */


static int my_string_offset;/*=0*/
/*static*/global void clear_my_string_offset(void)
{
	my_string_offset = 0;/* 初期化 */
}

/*---------------------------------------------------------
	カーソルを指定位置へ移動
---------------------------------------------------------*/
/*static*/global void set_kanji_xy(unsigned int set_x_dots, unsigned int set_y_number)
{
	cursor_x_dots		= set_x_dots;
	cursor_y_lines		= set_y_number;
}

/*---------------------------------------------------------
	カーソルをホームポジションへ移動
---------------------------------------------------------*/
/*static*/global void kanji_cursor_move_home_position(void)
{
	#if 1
	cursor_x_dots	= (0)*(KANJI_FONT_08_HARF_WIDTH);
	cursor_y_lines	= (0)*(KANJI_FONT_18_HEIGHT_P2);
	#else
	set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (0)*(KANJI_FONT_18_HEIGHT_P2));
	#endif
}

/*---------------------------------------------------------
	メッセージウィンドウの内容を消す。
---------------------------------------------------------*/
#if (1)
/*static*/global void kanji_window_clear_line(unsigned int line_num)
{
//	#define TEST_OFFSET (/*0*/512*10/2) /* 10 [laster? ] */
	#define TEST_OFFSET (0) 	/* put_pixel使うから(0) */
		#if (0==USE_32BIT_DRAW_MODE)
				#if 0
		line_num *= (512*(16+2)*(1/*3 2[行]*/)/**16*/ *2/*[short]==2[byte]の分*/ );
	{
	//	u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		unsigned int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(1/*3 2[行]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+(BG_FONT_HAIKEI_OFFSET*2)+line_num;	/* 512[dot]x 2[bytes](short) */
			*(u16 *)p = 0/*pixel*/;
		}
	}
				#endif
	{
	//	u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		u16 *p16;	/* VRAMが16bitモード */
		p16 = cb.kanji_window_screen_image;
		p16 += (BG_FONT_HAIKEI_OFFSET); 							/* 註:16bitポインタとしての値、つまり(unsigned shortだから)2倍[byte]される。 */
		p16 += (line_num * (512*(16+2)*(1/*3 2[行]*/)/**16*/)); 	/* 註:16bitポインタとしての値、つまり(unsigned shortだから)2倍[byte]される。 */
		unsigned int dx;
		for (dx=(0); dx<(0)+(512*(16+2)*(1/*3 2[行]*/)/**16*/); dx++)
		{
		//	(*p16) = (0xffff)/*pixel*/;/* てすと(描いてる所白くなる) */
			(*p16) = (0x0000)/*pixel*/;/* 黒(16bit) */
			p16++;
		}
	}
		#else
		line_num *= (512*(16+2)*(1/*3 2[行]*/)/**16*/ *4/*[int]==4[byte]の分*/ );
	{
	//	u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		unsigned int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(1/*3 2[行]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+dx+dx+(BG_FONT_HAIKEI_OFFSET*4)+line_num+line_num; /* 512[dot]x 2[bytes](short) */
			*(u32 *)p = 0/*pixel*/;
		}
	}
		#endif
}
#endif


/*static*/global void kanji_window_clear(void)
{
	unsigned int ii;
//	for (ii=0; ii<(3); ii++)
	for (ii=0; ii<(default_window_max_line); ii++)
	{
		kanji_window_clear_line(ii);
	}
}
#if 0
//	#define TEST_OFFSET (/*0*/512*10/2) /* 10 [laster? ] */
	#define TEST_OFFSET (0) 	/* put_pixel使うから(0) */
		#if (0==USE_32BIT_DRAW_MODE)
	{
	//	u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		unsigned int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(3/*3 2[行]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+(BG_FONT_HAIKEI_OFFSET*2); /* 512[dot]x 2[bytes](short) */
			*(u16 *)p = 0/*pixel*/;
		}
	}
		#else
	{
	//	u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/;//+y*(512*2)/*surface->pitch*/+x+x;	/* 512[dot]x 2[bytes](short) */
		unsigned int dx;
		for (dx=(/*0*/TEST_OFFSET); dx<(TEST_OFFSET)+(512*(16+2)*(3/*3 2[行]*/)/**16*/); dx++)
		{
			u8 *p = (u8 *)cb.kanji_window_screen_image/*surface->pixels*/+/*y*(512*2)*/ /*surface->pitch*/+dx+dx+dx+dx+(BG_FONT_HAIKEI_OFFSET*4);	/* 512[dot]x 4[bytes](int) */
			*(u32 *)p = 0/*pixel*/;
		}
	}
		#endif
#endif

			//	ccc_putpixel16(dx);//, 0, /*0xaaaa*/(0x0000)/*dot color16 */);
		//	*(u16 *)p = 0xaa55;
		//	p++;
		//	u8 *p = (u8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;

	#if 0/*test*/
	{
		unsigned int kk;
		for (kk=0; kk<(BG_FONT_HAIKEI_LENGTH)/*6*/; kk++)
		{
			(*(cb.kanji_window_screen_image+kk)) = (*(font_bg_bitmap_surface_image+kk));/*BG_FONT_HAIKEI_OFFSET_U16*/
		}
	}
	#endif

#if 1
/*---------------------------------------------------------
	漢字画面を全行消す。漢字カーソルをホームポジションへ移動。
	-------------------------------------------------------
	外部からメッセージウィンドウに漢字を描画する場合のリセット。
---------------------------------------------------------*/
global void kanji_window_all_clear(void)
{
	kanji_window_clear();					/* 漢字ウィンドウの内容を消す。 */
	kanji_cursor_move_home_position();		/* 漢字カーソルをホームポジションへ移動 */
}
#endif
//	msg_window_init();/*???*/
//	inits				= 1;/*???*/
//	is_bg				= 0;/*???*/
//	cg.dr aw_flag_kaiwa_screen	= 0;/*???*/
//	cursor_x_chached		= 0;/*???*/ /* カーソル初期化 */
//	cursor_y_lines_chached	= 0;/*???*/ /* カーソル初期化 */
//	clear_my_string_offset();/*???*/
//	set_write_text(0);		/* メッセージウィンドウに漢字の文字を描く様に設定する。 */

//static void msg_window_init(void)
//{
//
//}


/*---------------------------------------------------------
	shift jis文字 漢字かなまじり書体の文字列を描画する
	 (wait指定で、frame毎に一文字描画。 wait0で、続きを全描画)
---------------------------------------------------------*/
static int kanji_color_type;
static unsigned int kanji_wait_value;
extern void kanji_color(unsigned int set_color_type)
{
	kanji_color_type = set_color_type;
}
extern void kanji_wait(unsigned int set_wait)
{
	kanji_wait_value = set_wait;
}
global /*static*/ int kanji_draw(void)
{
//	const char *str;
//	int color_type;
//	/*(要確認)*/int wait;
//
//	strcpy(str, my_font_text);
//	color_type	= kanji_color_type;
//	wait		= kanji_wait_value;


	font_color_number = (kanji_color_type & 0x0f);
//
	int terminate_this_frame;	terminate_this_frame	= 0;
	int need_draw_this_flame;	need_draw_this_flame	= 0;
	{	/* wait が 0 の場合は一回で全文字描画する */
		static unsigned int count_wait_time;
		count_wait_time++;
		if (count_wait_time > (unsigned/*(要確認)*/)kanji_wait_value)
		{
			count_wait_time 		= 0;
			need_draw_this_flame	= 1;
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
		high_byte	= (*(my_font_text+my_string_offset) & 0xff);
		if (0x00 != high_byte )
		{
			int xx;
			int yy;
			xx	= 0/*(rect->x)*/+(cursor_x_dots);//*(KANJI_FONT_08_HARF_WIDTH);
			yy	= 0/*(rect->y)*/+(cursor_y_lines);//*(KANJI_FONT_18_HEIGHT_P2);
		//	パフォーマンスが低下するのでなるべくロックしない。(ロックは最小限に)
		/* 旧ロック */
			if (is_kanji_1st(high_byte)) /* shift jis 漢字 */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(my_font_text+my_string_offset+1) & 0xff);
				s_draw_sjis_kanji(xx, yy, high_byte, low_byte);
				#if (1==USE_HARF_KANJI)
				my_string_offset		+= size_of_kanji(high_byte);	/* 拡張半角文字領域 fdxx の場合は半角になる */
				cursor_x_dots			+= (KANJI_FONT_08_HARF_WIDTH)*size_of_kanji(high_byte); /* 拡張半角文字領域 fdxx の場合は半角になる */
				#else
				my_string_offset		+= (2);
				cursor_x_dots			+= (KANJI_FONT_08_HARF_WIDTH)*(2);
				#endif
			}
			else				/* ascii文字 */
			{
				if ('\\' == high_byte)		/* エスケープシークエンス処理(escape sequence) */
				{
					/*unsigned char*/int escape_sequence = (*(my_font_text+my_string_offset+1) & 0xff);
					if ('n' == escape_sequence) 	/* 改行コード */
					{
						my_string_offset += 2;
						cursor_x_dots = 999;/* 適当に大きな値を指定して改行させる */
					//	cursor_x_dots = 0;
					//	cursor_y_lines++;
					}
				}
				else					/* 半角文字 */
				{
					s_draw_sjis_kanji(xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x_dots += (KANJI_FONT_08_HARF_WIDTH);
				}
			}/* 旧ロック解除 */
			if (0==(kanji_color_type & STR_CODE_NO_ENTER))
			{
				/* 改行処理 */
				#define STR_LIMIT_MAX_X_WIDTH  (((int)((300/*(rect->w)*/) ) + (KANJI_FONT_08_HARF_WIDTH)))
				if ( (STR_LIMIT_MAX_X_WIDTH) < cursor_x_dots)
				{
					cursor_x_dots = ((0)*(KANJI_FONT_08_HARF_WIDTH));
					cursor_y_lines += (KANJI_FONT_18_HEIGHT_P2);
				//	#define MAX_cursor_y_lines	((2)*(KANJI_FONT_18_HEIGHT_P2))
				//	#define MAX_cursor_y_lines	((default_window_max_line)*(KANJI_FONT_18_HEIGHT_P2))
					#define MAX_cursor_y_lines	((8)*(KANJI_FONT_18_HEIGHT_P2))
				//	#define MAX_cursor_y_lines	((5)*(KANJI_FONT_18_HEIGHT_P2))
					if (MAX_cursor_y_lines < cursor_y_lines)
					{
						cursor_y_lines = (0)*(KANJI_FONT_18_HEIGHT_P2);
						kanji_window_clear();					/* 漢字ウィンドウの内容を消す。 */
					//	kanji_cursor_move_home_position();		/* 漢字カーソルをホームポジションへ移動 */
					}
				}
			}
		//	terminate_this_frame=0; 	/* continue frame */
			if (0 == kanji_wait_value)	/* wait が 0 の場合は一回で全文字描画する */
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
	現在 3行 x 18 dot == 54 laster + 背景用 10 laster(?) < 64(??)
	-------------------------------------------------------
	192 -10 == 180 / 18 == 10 行分(?)
---------------------------------------------------------*/

#include "graphics00.h"
global void kanji_system_init(void)/* 組み込み */
{
	set_kanji_origin_xy((10+6), (10+192));/*(表示原点の設定)*/
	set_kanji_origin_kankaku(18);/*(字間を標準にする)*/
	//
//	default_window_locate_x 			= (10+6);/* X位置 */
//	default_window_locate_y 			= (10+192);/* Y位置 */
//	default_window_font_tate_kankaku 	= (18);
	default_window_max_line 			= (3);/*(値3==4行)*/
	#if (0==USE_32BIT_DRAW_MODE)
		/* メッセージ表示用RAM確保 */	/* 現在表示が16bit色なので(32bit色表示なら変わる) */
		/* 背景用に10[laster]余分に確保 */
	//	cb.kanji_window_screen_image	= (u16 *)malloc((512*(10+64)*2));		/* (10+ 64)[laster] */
	//	cb.kanji_window_screen_image	= (u16 *)malloc((512*(10+128)*2));		/* (10+128)[laster] */
		cb.kanji_window_screen_image	= (u16 *)malloc((512*(10+(192-10))*2)); /* (10+ 64)[laster] */
	#else
		/* 色32bitモード */
		/* メッセージ表示用RAM確保 */	/* 現在表示が16bit色なので(32bit色表示なら変わる) */
		/* 背景用に10[laster]余分に確保 */
	//	cb.kanji_window_screen_image	= (u16 *)malloc((512*(10+64)*2));		/* (10+ 64)[laster] */
	//	cb.kanji_window_screen_image	= (u32 *)malloc((2*512*(10+128)*2));	/* (10+128)[laster] */
		cb.kanji_window_screen_image	= (u16 *)malloc((512*(10+(192-10))*2)); /* (10+ 64)[laster] */
	#endif
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
		#if (0==USE_32BIT_DRAW_MODE)
		{
			/* 現在表示が16bit色なので、32->16変換する。(32bit色表示なら変わる) */
		//	/*static*/ u16 *font_bg_bitmap_surface_image;
		//	font_bg_bitmap_surface_image	= font_bg_bitmap_surface->pixels;
			u32 *pixsrc;
			u16 *pixdst;
			pixsrc = (void *)((font_bg_bitmap_surface->pixels));
			pixdst = (cb.kanji_window_screen_image);
			trans_format8888to5650(pixsrc, pixdst, (BG_FONT_HAIKEI_LENGTH));
		}
		#else
		{
			/* 現在表示が16bit色なので、32->16変換する。(32bit色表示なら変わる) */
		//	/*static*/ u16 *font_bg_bitmap_surface_image;
		//	font_bg_bitmap_surface_image	= font_bg_bitmap_surface->pixels;
			u32 *pixsrc;
			u32 *pixdst;
			pixsrc = (void *)((font_bg_bitmap_surface->pixels));
			pixdst = (cb.kanji_window_screen_image);
			{
				unsigned int kk;
				for (kk=0; kk<(BG_FONT_HAIKEI_LENGTH); kk++)
				{
				//	(*(cb.kanji_window_screen_image+kk)) = (*(/*font_bg_bitmap_surface_image*/(u32 *)(font_bg_bitmap_surface->pixels)+kk));/*BG_FONT_HAIKEI_OFFSET_U16*/
					/* --- 16bit色に 減色して保持 (color key 使用不可) */
					*pixdst =  ((*pixsrc));
					pixdst++;
					pixsrc++;
				}
			}
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
	if (NULL != cb.kanji_window_screen_image)		{	free(cb.kanji_window_screen_image); 				cb.kanji_window_screen_image = NULL;	}
	#endif
}



#if (0)/*(????)*/
/*---------------------------------------------------------
	(r35)何故か巧くいかない。
	(ここは問題無いが、他の部分の font surface が消えちゃう)

---------------------------------------------------------*/

/*static*/extern SDL_Surface *FONT_fontimg;
/*---------------------------------------------------------
	任意の漢字イメージに、文字列をレンダリング
---------------------------------------------------------*/
/*, char *text, int font_number, int x_offset, int y_offset*/
global void kanji_ascii_font16_draw(void)
{
//???	if (SDL_MUSTLOCK(FONT_fontimg)) 	{	SDL_LockSurface(FONT_fontimg);	}	/* ロック */
//	SDL_Rect s;
//	SDL_Rect d;
//	s.w = 16/*fonts[cg.SDL_font_type].w*/; d.w = (s.w); 	/*fonts[cg.SDL_font_type].w*/
//	s.h = 16/*fonts[cg.SDL_font_type].h*/; d.h = (s.h); 	/*fonts[cg.SDL_font_type].h*/
	unsigned int/*char*/ i;
	unsigned int/*char*/ j;
	i = 0;
//	for (i=0; i<strlen(my_font_text); i++)
	{
	loop_str:
		j = my_font_text[i];
		if (0==j)
		{
			return;
		}
//		j -= 0x20;//0x20==' ';(space)
//	//	s.x = j*(s.w);						/*fonts[cg.SDL_font_type].w*/
//		s.x = (j & 0x0f)*(s.w);				/*fonts[cg.SDL_font_type].w*/
//	//	s.y = 0;
//		s.y = ((j>>4))*(s.h) + (160)/*fonts[cg.SDL_font_type].y_offset*/;//((cg.SDL_font_type)<<6);

int x;
int y;
//		d.x = (cursor_x_dots)/*cg.SDL_font_x*/ + (i)*(s.w);  	/*fonts[cg.SDL_font_type].w*/
//		d.y = (cursor_y_lines/**16*/)/*cg.SDL_font_y*/; 			/*0*/
	//	PSPL_UpperBlit(fonts[cg.SDL_font_type].fontimg, &s, txt_image_surface, &d);
	//	PSPL_UpperBlit(FONT_fontimg, &s, txt_image_surface, &d);
	x = (cursor_x_dots);//(d.x);
	cursor_x_dots += (16);// 8が16[dots] ??? (s.w);
	y = (cursor_y_lines/**16*/);

//		#define BG_FONT_HAIKEI_OFFSET		((512*10))		/* 10[laster]==(8+1+1) */

	int haikei_offset;
		haikei_offset	= (0);
//		haikei_offset	+= (s.x);
//		haikei_offset	+= ((s.y)*256);
		haikei_offset	+= ((j & 0x0f)*(16));
	//	haikei_offset	+= ((((j>>4))*(16) + (160))*256);/*(テクスチャ横が256[dots])*/
		haikei_offset	+= (((j & 0xf0) | (0x80) )*256);/*(テクスチャ横が256[dots])*/

	//	haikei_offset	= (512*8);
		unsigned int dy;
		for (dy=0; dy<16/*KANJI_FONT_16_HEIGHT_P0*/; dy++)
		{
			unsigned int dx;
			for (dx=0; dx<16/*KANJI_FONT_08_HARF_WIDTH*/; dx++)
			{
				{
					#if (0==USE_32BIT_DRAW_MODE)/*(16bit mode)*/
					/*
						いまいち良くわかんない(が、とりあえず動く)。
						アドレスはバイト単位なので u8 ポインタでＣ言語に計算させてる。
						(short なら2で割るとか、int なら4で割るとか)
						変える場合は、定数(BG_FONT_HAIKEI_OFFSET_U16等)や変数(dy,y,他)も変えないとダメ。
					*/
				//	s_getpixel16
				volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/(FONT_fontimg->pixels)/*cb.kanji_window_screen_image*/)	/*surface*/
						+ (dy * (512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
						+ (dx+dx)
						+ (haikei_offset+haikei_offset);
				//	putpixel16
				volatile u8 *dst_p = (u8 *)(cb.kanji_window_screen_image)/*surface->pixels*/
						+ (((dy+y))*(512*2))/*surface->pitch*/
						+ (dx+dx)
						+ (x+x)
						+ (BG_FONT_HAIKEI_OFFSET*2);	/* 512[dot]x 2[bytes](short) */
					*(u16 *)dst_p = (u32)(*(u16 *)src_p);
					#else/*(32bit mode)*/
					/*
						いまいち良くわかんない(が、とりあえず動く)。
						アドレスはバイト単位なので u8 ポインタでＣ言語に計算させてる。
						(short なら2で割るとか、int なら4で割るとか)
						変える場合は、定数(BG_FONT_HAIKEI_OFFSET_U16等)や変数(dy,y,他)も変えないとダメ。
					*/
				//	s_getpixel16
				volatile u8 *src_p = (u8 *)(/*font_bg_bitmap_surface_image*/(FONT_fontimg->pixels)/*cb.kanji_window_screen_image*/) 	/*surface*/
						+ (dy * (2*512/*font_bg_bitmap_surface_pitch*/))		/*surface*/
						+ (dx+dx+dx+dx)
						+ (haikei_offset+haikei_offset+haikei_offset+haikei_offset);
				//	putpixel16
				volatile u8 *dst_p = (u8 *)(cb.kanji_window_screen_image)/*surface->pixels*/
						+ (((dy+y))*(2*512*2))/*surface->pitch*/
						+ (dx+dx+dx+dx)
						+ (x+x+x+x)
						+ (BG_FONT_HAIKEI_OFFSET*4);	/* 512[dot]x 2[bytes](short) */
				//	*(u16 *)dst_p = (u32)(*(u16 *)src_p);
					*(u32 *)dst_p = (u32)(*(u16 *)src_p);
					#endif
				}
			}
		}
		i++;
		goto loop_str;
	}
//???	if (SDL_MUSTLOCK(FONT_fontimg)) 	{	SDL_UnlockSurface(FONT_fontimg); 	}	/* ロック解除 */
}
#endif // (0)/*(????)*/

