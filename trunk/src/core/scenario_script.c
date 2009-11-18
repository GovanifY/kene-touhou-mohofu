
/*---------------------------------------------------------
	全角文字表示ライブラリ zenkaku.c / shinonomefont.c

	PSPで平易に漢字を表示するために公開します。
	フォントは東雲フォントを使用させて頂いています。

＜要請＞
	このライブラリはフリーウェアとし、特に金品は要求しませんが、
	このライブラリを利用してソフトウェアを公開する方は、ドキュメント中に
	以下の文言を挿入して頂けますよう、お願い致します。

	「このソフトウェアはmediumgauge氏作成の全角文字表示ライブラリを使用しています。」

＜仕様＞
	ごく小さなプログラムですので、お読みいただければわかると思いますが、
	zenkaku.h に定義されている mh_print() に座標、文字列、色を渡せば描画されます。
---------------------------------------------------------*/

#include "game_main.h"
#include "scenario_script.h"


#define FONT_WIDTH			(8)
#define FONT_HEIGHT 		(16)

/*---------------------------------------------------------
	shift jisコード、全角1バイト目かどうか判定する子関数
---------------------------------------------------------*/

static int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte)
{
	#if 1
	high_byte &= 0xff;/**/
	return (
		((high_byte >= 0x81) && (high_byte <= 0x9f)) ||
		((high_byte >= 0xe0) && (high_byte <= 0xfd))
	);
	#else
	high_byte ^= 0x20;
	high_byte += (0x100-0xa1);
	high_byte &= 0xff;/**/
	return (high_byte < 0x3du);
	#endif
}


/*
現実の問題として、東雲フォント16には
2バイト半角漢字はフォントデーターがない(作成されてない。作成すれば使える)
そのため 2バイト半角漢字は表示されない。(データーが全て空白の為)
//
その上で、根源的な問題として、pspで2バイト半角漢字をサポートする意味がない気がする。
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
	{			return (1); }
	else	{	return (2); }
}
#endif

/*---------------------------------------------------------
	東雲文字コード規格 の文字を描画する子関数
---------------------------------------------------------*/

static int font_color_number;

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
#if (1)
static UINT16 *bg_story_window_image;
static int bg_story_window_pitch;
static int bg_story_window_width;
#endif
#if (1)
static UINT16 *drawmap_image;
static int drawmap_pitch;
static int drawmap_width;
#endif

extern UINT16 *msg_window_image;
static int msg_window_pitch;
static int msg_window_width;

static Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	return (*(Uint16 *)p);
}

static void putpixel16(/*SDL_Surface *surface,*/ int x, int y, Uint32 pixel)
{
	Uint8 *p = (Uint8 *)drawmap_image/*surface->pixels*/+y*drawmap_pitch/*surface->pitch*/+x+x;
	*(Uint16 *)p=pixel;
}

static void draw_shinonome_moji(/*SDL_Surface *drawmap,*/ SDL_Surface *backmap, int x, int y, int code_offset, int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=hidari_gawa)
	{
		x += FONT_WIDTH;
	}
	if (x > (/*drawmap->w*/drawmap_width-FONT_WIDTH) )	/* 範囲外なら */
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
				Uint32 dot;
				dot=getpixel16(backmap, (dx+haikei_offset), dy);	/*font_color_bitmap*/
				putpixel16(/*drawmap,*/ dx+x, dy+y, dot);
			}
		}
	}
}

/*---------------------------------------------------------
http://openlab.ring.gr.jp/efont/shinonome/
	shift jis文字コード規格 を 東雲文字コード規格 へ変換後、描画する
---------------------------------------------------------*/

static void draw_sjis_kanji(/*SDL_Surface *drawmap,*/ SDL_Surface *backmap, int x, int y, int high_byte, int low_byte)
{
	if (0==high_byte)	/* 半角文字の場合(asciiコード) */
	{
		/* asciiコード → shinonomeコード 変換 */
		static	const	Uint8/*int*/ sinonome_ascii_bank_offset[16] =
		{// +0	  +1	+2	  +3	 +4    +5	 +6    +7	  +8	+9	  +a	+b	   +c	 +d    +e	 +f
			0xff, 0xff, 0x00, 0x10,  0x20, 0x30, 0x40, 0x50,  0xff, 0xff, 0x60, 0x70,  0x80, 0x90, 0xff, 0xff
		};
	//		 if (low_byte < 0x20)	{	return; }				// ascii 0x00-0x1f
	//	else if (low_byte < 0x80)	{	low_byte -= 0x20; } 	// ascii 0x20-0x7f
	//	else if (low_byte < 0xa0)	{	return; }				// ascii 0x80-0x9f
	//	else if (low_byte < 0xe0)	{	low_byte -= 0x40; } 	// ascii 0xa0-0xdf
	//	else						{	return; }				// ascii 0xe0-0xff
	//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// ここでの9は多分1つの文字に使うバイト数。(1byte == 8bit)*9=72 == 6*12
		Uint8 idx8;
		idx8 = sinonome_ascii_bank_offset[((low_byte>>4) & 0x0f)];
		/* 存在マップが 0xff の場合は、 東雲文字コード規格 にない文字なので表示できない */
		if (0xff == idx8)
		{
			return;
		}
		idx8 = ((idx8)|((low_byte & 0x0f)));
		draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx8), 0);/*low_byte<<4*/
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
			draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx) , 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2バイト全角
		#endif
		{
			idx += (low_byte+low_byte);/* x2 */
			draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx) , 0); /* 右半分 */
			draw_shinonome_moji(/*drawmap,*/ backmap, x, y, (idx) , 1); /* 左半分 */
		}
	}
}

/*---------------------------------------------------------
	文字列の描画
---------------------------------------------------------*/

//extern int count_char;/*scenario.c*/

static /*extern*/ /*static*/ int cursor_x;
static /*extern*/ /*static*/ int cursor_y;

static int my_string_offset/*=0*/;
static void clear_my_string_offset(void)
{
	my_string_offset=0;/* 初期化 */
}

/*---------------------------------------------------------
	カーソルをホームポジションへ移動
---------------------------------------------------------*/
static void home_cursor(void)
{
	cursor_x = 0;
	cursor_y = 0;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static SDL_Surface *msg_window/*=NULL*/;		/* メッセージウィンドウ */
static void msg_window_clear(void)
{
	SDL_FillRect(msg_window,NULL,SDL_MapRGB(msg_window->format,0,0,0));
}
//static void msg_window_init(void)
//{
//
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

		#if 0
		/*???*/
	//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); 		font_color_bitmap	= NULL; }
		if (NULL != font_color_bitmap)	{	unloadbmp_by_surface(font_color_bitmap);	font_color_bitmap	= NULL; }
	//	font_color_bitmap=loadbmp(buffer);
	//	esc_len=0;
		{
			char fn[64];
			sp rintf(fn,"%s/" "fonts/font_bg%02d.png", data_dir, color_type);		/* 文字列処理(sp rintf)は非常に遅い */
			font_color_bitmap = IMG_Load(fn);/*ここでロードすると確実に処理落ちするよ*/
		}
		font_color_bitmap = SDL_DisplayFormat(font_color_bitmap);
		#endif
			#if 0
			unloadbmp_by_surface(font_color_bitmap);
			font_color_bitmap = NULL;
			#endif

/*---------------------------------------------------------
	shift jis文字 漢字かなまじり書体の文字列を描画する (wait指定で、frame毎に一文字描画。 wait0で、続きを全描画)
---------------------------------------------------------*/
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
	count_width 		横に並べられる文字の数
	font_color_bitmap	書体の色を指定する画像
*/

//static extern SDL_Surface *font_color_bitmap; /* 字の色パターン画像 */
//static SDL_Surface *font_color_bitmap/*=NULL*/;
static SDL_Surface *font_bg_bitmap/*=NULL*/;


//static /*extern*/ int cursor_continue;				/* カーソル継続 */

/* 今の所必要ないけど、外部から描くなら。 */
/*extern*/ /*static*/ int print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *dummy_rect,*/ const char *str, int color_type, int wait)
{
	font_color_number = (color_type & 0x0f);

	static int count_width=1/* 0 (割り算なので0は危険) */;/*???*/
//	static int esc_len;
	if (0 == my_string_offset)	//初期化
	{
		count_width=(int)((300/*(rect->w)*/)/FONT_WIDTH)+2;
		//
//		if (0==cursor_continue)
//		{	/* カーソル継続しない */
//			cursor_x=(0);
//			cursor_y=(0);
//		}
	//	else	/* カーソル継続する */
	//	{
	//		cursor_continue=0;	/* 次回は、カーソル継続しない */
	//	}
	}
	int terminate_this_frame;	terminate_this_frame=0;
	int need_draw_this_flame;	need_draw_this_flame=0;
//	if (0 == wait)	{		need_draw_this_flame = 1; } else
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
			#define IS_LOCK_LOCAL_SCREEN (1)
			#if (1==IS_LOCK_LOCAL_SCREEN)
/// 		if (SDL_MUSTLOCK(drawmap))				{	SDL_LockSurface(drawmap);			}	/* ロック */
		//	if (SDL_MUSTLOCK(font_color_bitmap))	{	SDL_LockSurface(font_color_bitmap); }	/* ロック */
			if (SDL_MUSTLOCK(font_bg_bitmap))		{	SDL_LockSurface(font_bg_bitmap);	}	/* ロック */
			#endif
		loop:
		;
		/*unsigned char*/int high_byte;
		high_byte = (*(str+my_string_offset) & 0xff);
		if (0x00 != high_byte )
		{
		//	cursor_x=((my_string_offset+count_char+esc_len)%count_width);	/* 除算(%)は非常に遅い */
		//	cursor_y=((my_string_offset+count_char+esc_len)/count_width);	/* 除算(/)は非常に遅い */
		//
			int xx;
			int yy;
			xx = 0/*(rect->x)*/+(cursor_x)*(FONT_WIDTH);
			yy = 0/*(rect->y)*/+(cursor_y)*(FONT_HEIGHT+2);
		//	パフォーマンスが低下するのでなるべくロックしない。(ロックは最小限に)
		/* 旧ロック */
			if (is_kanji_1st(high_byte)) /* shift jis 漢字 */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(str+my_string_offset+1) & 0xff);
			//	draw_sjis_kanji(/*drawmap,*/ font_color_bitmap, xx, yy, high_byte, low_byte);
				draw_sjis_kanji(/*drawmap,*/ font_bg_bitmap, xx, yy, high_byte, low_byte);
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
					//	esc_len += count_width-(cursor_x)-2;
						my_string_offset += 2;
						cursor_x = 999;/* 適当に大きな値を指定して改行させる */
					//	cursor_x=0;
					//	cursor_y++;
					}
				}
				else					/* 半角文字 */
				{
				//	draw_sjis_kanji(/*drawmap,*/ font_color_bitmap, xx, yy, 0, high_byte);
					draw_sjis_kanji(/*drawmap,*/ font_bg_bitmap, xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x++;
				}
			}/* 旧ロック解除 */
			/* 改行処理 */
			if ( (count_width-1) < cursor_x)
			{
				cursor_x=0;
				cursor_y++;
				#define MAX_cursor_y (2)
				if (MAX_cursor_y < cursor_y)
				{
					cursor_y = 0;
				//	home_cursor();
					msg_window_clear();
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
/// 		if (SDL_MUSTLOCK(drawmap))				{	SDL_UnlockSurface(drawmap); 			}	/* ロック解除 */
		//	if (SDL_MUSTLOCK(font_color_bitmap))	{	SDL_UnlockSurface(font_color_bitmap);	}	/* ロック解除 */
			if (SDL_MUSTLOCK(font_bg_bitmap))		{	SDL_UnlockSurface(font_bg_bitmap);		}	/* ロック解除 */
			#endif
	//	if (1==terminate_this_frame)	//終了処理	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=terminate_this_frame)	//終了処理
		{
			{
			//	count_width=1/* 0 (割り算なので0は危険) */;/*???*/
			//	count_char += my_string_offset/*+esc_len*/;
				my_string_offset = 0;	/* 次回は、初期化 */
				//cursor_continue=0;	/* 次回は、カーソル継続しない */
			}
		}
	}
	return (terminate_this_frame);		/* 0:continue, 1:end */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern int errno;

// _script_command

// [テキストのカーソル制御]

// [テキスト表示/クリック待ち]
//		CURSOR		/*	CURSOR	カーソルの初期化 */
//					/*	CURSOR	記憶したカーソル位置の呼び出し */
//					/*	CURSOR	カーソル位置の記憶 */
//		CLICK		/* クリック待ち */
//		TEXT		/* メッセージウィンドウへの書き込み */

//		BG_LOAD 	/* LO ADBG		背景ファイルの変更 */
//		OBJ_LOAD	/* LO ADSP		汎用スプライトのロード */
//		OBJ_SWAP	/* RE LO ADSP	汎用スプライトの再読み込み */

//		OBJ_MOVE	/* MO VESP		スプライトの移動 */



//----------- [廃止]
//		SUBG		/* [廃止] [SCREEN]	SU BG背景の表示/非表示 */
//		SUTWINDOW	/* [廃止] [SCREEN]	SU TWINDOW 強調立ち絵ウィンドウの表示/非表示 para2はスコアパネルのon/off */

//----------- [廃止]
//		CUR_HOME	/* [廃止] [CURSOR]	CL CURSOR	カーソルの初期化 */
//		CUR_POP 	/* [廃止] [CURSOR]	LO ADCUR	記憶したカーソル位置の呼び出し */
//		CUR_PUSH	/* [廃止] [CURSOR]	SA VECUR	カーソル位置の記憶 */

//		SUSPRITE	/* [廃止] [廃止    ] SU SPRITE	汎用スプライトの表示/非表示 */
//		OBJ_SET 	/* [廃止] [廃止    ] PA RAMSP	汎用スプライトのアルファセット/アニメーション	*/


//		BGTEXT		/*					 BG TEXT	背景にテキストを打ち込む */
//		FILTER		/* [廃止] [廃止    ] FI LTER */
//		SUFILTER	/* [廃止] [廃止    ] SU FILTER	フィルター表示/非表示&透過度 */

//		SUWINDOW	/* [廃止] [廃止    ] SU WINDOW	メッセージウィンドウの表示/非表示 para2はbpp速度。*/
//		SUL 		/* [廃止] [SU SPRITE] SU L		立ち絵左スプライトの表示/非表示 */
//		SUR 		/* [廃止] [SU SPRITE] SU R		立ち絵右スプライトの表示/非表示 */
//		JUMP		/* [廃止] [廃止    ] JU MP		pauseボタンを押した時のjump先。 */ /*廃止*/
//		LOADL		/* [廃止] [LO ADSP ] LO ADL 	立ち絵左 スプライトのロード */
//		LOADR		/* [廃止] [LO ADSP ] LO ADR 	立ち絵右 スプライトのロード */
//		WAIT		/* [廃止] [廃止    ]  wait */

//		TWINDOW 	/* TW INDOW[廃止] [廃止    ]  立ち絵強調ウィンドウの初期化 */

enum
{
// [テキストのカーソル制御]
	SCP_CUR_POP,
	SCP_CUR_PUSH,
	SCP_CUR_HOME,
// [テキスト表示/クリック待ち]
	SCP_CLICK,
	SCP_BG_WRITE_TEXT,
	SCP_TEXT_WRITE_TEXT,
	SCP_TEXT,
//
	SCP_BGM,
//
//	SCP_FILTER,/*廃止*/
//	SCP_JUMP,/*廃止*/
	SCP_BG_LOAD,
//	SCP_LOAD_LR,/*廃止*/
	SCP_OBJ_LOAD,
	SCP_OBJ_MOVE,
//	SCP_OBJ_SET,/*廃止*/
	SCP_OBJ_SWAP,

	SCP_SCREEN_BG,
	SCP_SCREEN_TEXT,
	SCP_SCREEN_PANEL,

//	SCP_SUFILTER,/*廃止*/
//	SCP_SUL,/*廃止*/
//	SCP_SUR,/*廃止*/
//	SCP_SUSPRITE,/*廃止*/
//	SCP_SUWINDOW,/*廃止*/
//	SCP_TWINDOW,/*廃止*/
//	SCP_WAIT/*廃止*/
};

typedef struct _scenario_script
{
	int command;
	int done;		// 終わったかどうか
	int chain;		// 同時実行かどうか
					// 0=違うよ 1=1つ目 2=2つ目
	//void *data;/*廃止*/
//
	int para1;
	int para2;
	int para3;
	int para4;
	struct _scenario_script *next;
//
	char para0[(7*32)/*200*/];/* 32の倍数 */
} S_SCRIPT;/* == 256bytes */


typedef struct /*_sc_sprite*/
{
	int/*int*/ x;		/* x座標だよ */
	int/*int*/ y;		/* y座標だよ */
	int x256;						/* 精度確保用(256固定小数点形式) */
	int y256;						/* 精度確保用(256固定小数点形式) */
//[4]
	int w;							/* 幅 */			//	int cw; 	/* 中心座標(幅)だよ */
	int h;							/* 高さ */			//	int ch; 	/* 中心座標(高さ)だよ */
//
	int target_x;					/* 目標座標 */
	int target_y;					/* 目標座標 */
//[8]
	int move_wait;					/* 動きがあったときの制御用 */
	int angle512;		/* 正確な方向 */	//	int r_course;	/* 大体の方向 */
//[12]
	SDL_Surface *img;				/* Images */
//
//	Uint8 alpha255;					/* alpha値 */
//	Uint8 flags;					/* 0:非表示, 1:表示, 2:tachie_window(2nd screen)に表示	ここでは表示させるかどうかだけだよ */
} SC_SPRITE;

/*---------------------------------------------------------

---------------------------------------------------------*/

/*************グローバル変数**************/
/* zenkaku.c */
// static extern int cursor_continue;			/* カーソル継続 */
// /*static*/ int cursor_x; 					/* カーソル位置 */
// /*static*/ int cursor_y; 					/* カーソル位置 */
//int count_char/*=0*/; 						/* カーソル位置 */
//static int cursor/*=0*/;						/* count_char保存用 */
static int cursor_x_chached;					/* カーソル位置 保存用 */
static int cursor_y_chached;					/* カーソル位置 保存用 */
/*****************************************/

/*	pspでは ここで初期化しても正常動作しない場合がある。
	起動時に初期化しないとダメ。(PCのソースをpspでコンパイルして動かない原因のひとつ) */
static SDL_Surface *msg_window/*=NULL*/;		/* メッセージウィンドウ */
//static SDL_Surface *std_window/*=NULL*/;		/* ウィンドウの半透明枠 */

static SDL_Surface *bg_story_window;			/* スクリプトにおける背景 */

static SDL_Surface *sdl_lock_surface;			/* ロックするサーフェイス */

//static SDL_Surface *font_color_bitmap/*=NULL*/;
static SDL_Surface *font_bg_bitmap/*=NULL*/;

//
static S_SCRIPT *sscript/*=NULL*/;				/* 命令保持用 */
static S_SCRIPT *entry_script;					/* 命令収集用 */
static S_SCRIPT *start_script/*=NULL*/; 		/* 命令実行用 */
//
#define SPRITE_tachie_l (29)/*-3*/
#define SPRITE_tachie_r (30)/*-2*/
#define SPRITE_tachie_m (31)/*-1*/
#define SPRITE_MAX		(32)
static SC_SPRITE *std_obj[SPRITE_MAX/*32*/ /*20*/]; 		/* 汎用スプライト */
//static SC_SPRITE *tachie_r;
//static SC_SPRITE *tachie_l;
//


//

static int is_bg/*=0*/; 						/* 背景表示/非表示フラグ */

//static int 380/*scenario_width*/; 			/* 横幅 */

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_local01(char *filename, SDL_Surface *s2/*result*/, int alpha)
{
	char fn[128];
//	strcpy(fn, data_dir);	strcat(fn, "/scenario/");
	strcpy(fn, DIRECTRY_NAME_DATA "/"/*"scenario/"*/);
	strcat(fn, filename);
//
	if (NULL != s2)
	{
		SDL_FreeSurface(s2);
		s2 = NULL;
	}
//
	SDL_Surface *s1;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(fn);/*ここでロードすると確実に処理落ちするよ*/
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "script:cant load image %s:"/*" %s"*/,fn/*,SDL_GetError()*/);
//		return (NULL);
	}
	if (	(-1==alpha) /* スクリプト中で省略した場合(デフォルト) */
		|| (0==alpha)	/* 明示した場合 */
	)
	{
		/* アルファ使わない */
		s2 = SDL_DisplayFormat(s1);
		SDL_SetColorKey(s2,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);	/* カラーキー(抜き色、透明色)は黒 */
	}
	else
	{
		/* アルファ使う */
		s2 = SDL_DisplayFormatAlpha(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
//		return (NULL);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void remove_sc_sprite(SC_SPRITE *src)
{
	if (NULL != src)
	{
		SDL_FreeSurface(src->img);
		free (src);
		src = NULL;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void swap_my_sprite(char *filename, int no, int dummy_frames, int alpha)
{
	SC_SPRITE *tmp;
	if (no==-1) 		{	return; 		}
//	else if (no==-2)	{	tmp=std_obj[SPRITE_tachie_r/*32-2*/];	}	/* right */
//	else if (no==-3)	{	tmp=std_obj[SPRITE_tachie_l/*32-3*/];	}	/* left */
	else				{	tmp=std_obj[(no)&(SPRITE_MAX-1)];	}	/* 汎用スプライト */
	if (NULL==tmp)		{	return; 		}

	SDL_FreeSurface(tmp->img);

	SDL_Surface *suf = NULL;
	suf 		= load_local01(filename,suf,alpha);
	tmp->img	= suf;
//	tmp->frames = frames;
	tmp->w		= (suf->w/*/frames*/);	//tmp->cw		= ((tmp->w)>>1);
	tmp->h		= (suf->h); 		//tmp->ch		= ((tmp->h)>>1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static /*SC_SPRITE **/void load_my_sprite(char *filename, int sc_sp_num, int xx, int yy/*int dummy_frames, int dummy_alpha, SDL_Rect *rect*/)
{
	if (NULL != std_obj[sc_sp_num]) 	{	remove_sc_sprite(std_obj[sc_sp_num]); }
	SC_SPRITE *tmp;
	tmp 				= mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename,0/*alpha*/);
	suf 				= load_local01(filename,suf,0/*alpha*/);
	if (NULL==suf)		{ return /*(NULL)*/;			}
	std_obj[sc_sp_num]	= tmp;
	tmp->img			= suf;
//	if (frames==-1) 	{	tmp->frames = 1;		}
//	else				{	tmp->frames = frames;	}
	tmp->w				= (suf->w/*/frames*/);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
//
//	tmp->aktframe		= 0;
//	tmp->anim_speed 	= 0;
//	tmp->anim_type		= 0;
//	tmp->anim_time		= 0;
//	tmp->anim_count 	= 0;
	/*tmp->alpha255		= 255;*/	/*tmp->color32		= 0xffffffff;*/
//	tmp->flags			= 0;
	tmp->move_wait		= 0;
	tmp->x				= xx/*rect->x*/;
	tmp->y				= yy/*rect->y*/;
//	tmp->x				= rect->x-(rect->w*tmp->w);
//	tmp->y				= rect->y-(tmp->h);
//	tmp->x				= xx-(0*tmp->w);
//	tmp->y				= yy-(tmp->h);

//	return (tmp);
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static int inits;								/* 各スクリプトコマンドごとの初期化判定 */

/*---------------------------------------------------------
	SCP_OBJ_MOVE	スプライトの移動
---------------------------------------------------------*/

static int do_move_sc_sprite(char *color32_str, int n, int x, int y, int speed_aaa/*, int alpha255*/)
{
	SC_SPRITE *sc;
	{
//		int n = parth_str_right_or_str_left_or_number(l_or_r);
//		if (n==STR_ERROR/*-1*/) 		{	return (2/*-1*/);	}/*スクリプト終了*/
	//	else if (n==STR_RIGHT/*-2*/)	{	sc = tachie_r;	}	/* right */
	//	else if (n==STR_LEFT/*-3*/) 	{	sc = tachie_l;	}	/* left */
	//	else
		{	sc = std_obj[((n)&(SPRITE_MAX-1))]; }	/* 汎用スプライト */
	}
//
	if (inits)	/* 初回のみ行う */
	{
		/* あらかじめ移動完了位置を算出しておく */
		sc->target_x	= sc->x + x;		/* 移動完了座標 */
		sc->target_y	= sc->y + y;		/* 移動完了座標 */
		sc->x256		= ((sc->x)<<8); 	/* 精度確保用 */
		sc->y256		= ((sc->y)<<8); 	/* 精度確保用 */
		/* 移動方向を決める */
		sc->angle512	= atan_512(y,x);
	//	sc->color32 	= ((alpha255)<<8);	/* 色 */
	//	sc->alpha		= ((alpha255)<<8);	/* 色 */
		if ('f'==color32_str[2])
		{
		//	sc->alpha255 = (255);	/* 色 */
			SDL_SetAlpha(sc->img, SDL_SRCALPHA, (255) );
		}
		else
		{
		//	sc->alpha255 = (180);	/* 色 */
			SDL_SetAlpha(sc->img, SDL_SRCALPHA, (180) );
		}
	}
//
	/* 10で割るのは遅過ぎるので、3ビットシフトして(1/8)の値にする */
	int speed256;
	speed256 = (speed_aaa<<(8-3));		/* 精度確保用 */
	#if 0
	/* CWの場合 */
//	sc->x += ((cos512(sc->angle512)*speed)/10);
//	sc->y += ((sin512(sc->angle512)*speed)/10);
//	sc->x256 += ((cos512(sc->angle512)*speed256)>>8);	/* 精度確保用 */
//	sc->y256 += ((sin512(sc->angle512)*speed256)>>8);	/* 精度確保用 */
	#else
	/* CCWの場合 */
	sc->x256 += ((sin512(sc->angle512)*speed256)>>8);	/* 精度確保用 */
	sc->y256 += ((cos512(sc->angle512)*speed256)>>8);	/* 精度確保用 */
	#endif

//
	sc->x	= (t256_floor(sc->x256));		/* 精度確保用 */
	sc->y	= (t256_floor(sc->y256));		/* 精度確保用 */
	/* 大体の方向: なので ４５度回転してずらして、上位２ビットが大まかな方向 */
	#if 0
	/* CWの場合 */
	switch ((((sc->angle512+64/*deg_360_to_512(45)*/) & 0x180 )))/*sc->r_course*/
	{	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
	case (0<<7)/* 1:→(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:↓(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:←(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:↑(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
	}
	#else
	/* CCWの場合 */
	switch ((((sc->angle512-64/*deg_360_to_512(45)*/) & 0x180 )))/*sc->r_course*/
	{	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
//	case (0<<7)/* 2:↓(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
//	case (1<<7)/* 1:→(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
//	case (2<<7)/* 0:↑(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
//	case (3<<7)/* 3:←(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:↓(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
	case (0<<7)/* 1:→(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:↑(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:←(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	}
	#endif
	return (0);/*移動中*/
move_complete:
	/* 移動完了した場合は、正確な座標に修正する。 */
	sc->x = sc->target_x;
	sc->y = sc->target_y;
	return (1);/*移動完了*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static void set_write_text(int is_bg_screen)
{
//	if (0==is_bg_screen)
	{
		#if (1)
		/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
		drawmap_image	= msg_window_image;
		drawmap_pitch	= msg_window_pitch;
		drawmap_width	= msg_window_width;
		sdl_lock_surface	= msg_window;
		#endif
	}
//	else
//	{
//		#if (1)
//		/* 背景ウィンドウに漢字の文字を描く様に設定する。 */
//		drawmap_image	= bg_story_window_image;
//		drawmap_pitch	= bg_story_window_pitch;
//		drawmap_width	= bg_story_window_width;
//		sdl_lock_surface	= bg_story_window;
//		#endif
//	}
}
/*---------------------------------------------------------
	外部からメッセージウィンドウに漢字を描画する場合のリセット
---------------------------------------------------------*/

/*static*/ void script_message_window_clear(void)
{
	msg_window_clear(); 	/* メッセージウィンドウの内容を消す。 */
//	msg_window_init();/*???*/

//	inits				= 1;/*???*/
//	is_bg				= 0;/*???*/
//	draw_script_screen	= 0;/*???*/

	home_cursor();		/* カーソルをホームポジションへ移動 */
//	cursor_x_chached	= 0;/*???*/ /* カーソル初期化 */
//	cursor_y_chached	= 0;/*???*/ /* カーソル初期化 */
//	clear_my_string_offset();/*???*/

	set_write_text(0);		/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static SDL_Rect msgw_rect;				/* メッセージウィンドウ表示位置 */
//static void msgw_rect_init(int x,int y)
//{
//	msgw_rect.x = x;
//	msgw_rect.y = y;
//}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ int draw_script_screen/*=0*/;					/* せりふウィンドウ表示フラグ */

/*---------------------------------------------------------
	システム処理(???)
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static void load_script_free(void)
{
	S_SCRIPT *l = sscript;
	S_SCRIPT *n;
	while (NULL != l)
	{
		n=l->next;
		//if (NULL != l->data)	{ free (l->data);	l->data = NULL; }
		free (l);
		l = n;
	}
	sscript = (S_SCRIPT *)NULL;
	entry_script = (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_command(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	while ((*c != ' ') && (*c != 13))
	{
		i++;
		if (i >= 10)
		{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	if (*c == 13)	/* 改行コードは OD OA */
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}

/*---------------------------------------------------------
	スクリプトファイルの文字列部分の読み込み
	-------------------------------------------------------
	shift jis 漢字の2byte目が￥￥の場合や
	エスケープシークエンス処理の2byte目が￥￥の場合でも
	問題がない
---------------------------------------------------------*/
static char *load_script_get_str(char *str, char *buffer, int *end_arg)
{
	int string_error_limiter;
	string_error_limiter = 200;/* 200 文字以上はエラー */
	/*unsigned char*/int high_byte;
	{loop:;
		high_byte = ((*str) & 0xff);
		/* 空文字列の可能性があるから、始めに判定 */
		if (',' == high_byte)	/* ','区切りでおしまいの場合 */
		{
			goto kamma_end;
		}
		else
		if (13 == high_byte)	/* '\n'==13(行末)でおしまいの場合 */
		{
	//	ret13_end:
			*end_arg = 1;/* 行末です。 */
		kamma_end:
			*buffer = '\0'; 	/* EOS を追加 */
			return (str);
		}
		else					/* 文字列を転送する必要のある場合 */
		{
			int flag;
			flag=0;
			if (is_kanji_1st(high_byte)) /* shift jis 漢字 */
			{
				;	//	*buffer++ = *str++;/* 1byte目 */
			}
			else
			{
				if ('\\' == high_byte)		/* エスケープシークエンス処理(escape sequence) */
				{
					;	//	*buffer++ = *str++;/* ￥￥ */
				}
				else					/* 半角文字 */
				{
					flag=1;
				}
			}
			if (0==flag)	/* 半角文字以外(shift jis 漢字、エスケープシークエンス処理)は 2 byte転送 */
			{
				*buffer++ = *str++;
			}
			*buffer++ = *str++; 	/* 1 byteは必ず転送 */
			/* エラーチェック */
			{
				string_error_limiter--;
				if (0 >= string_error_limiter)
				{
					return ((char *)NULL);
				}
			}
		}
		goto loop;
	}
}
#if 0
/*---------------------------------------------------------
	よくわかんない
	-------------------------------------------------------
	このままだとsjisの2byte目が'\'の場合対応できないよ
---------------------------------------------------------*/
static char *load_script_get_str(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	int is_escape_sequence_mode;		/* エスケープ */
	is_escape_sequence_mode = 0;		/* エスケープ */
//
loop:;
	/*while*/
	if (
		(
			((',' == *c) && (1==is_escape_sequence_mode)) || (',' != *c)
		) &&
		(*c != 13)
	)
	{	;	}
	else
	{	goto loop_end;}
//
	{
		if (0 == is_escape_sequence_mode)
		{
			if ('\\' == *c)
			{
				if (',' == *(c+1))
				{
					is_escape_sequence_mode = 1;
					c++;
				}
			}
		}
		else
		if (is_escape_sequence_mode)
		{
			is_escape_sequence_mode = 0;
		}
		i++;
		if (i >= 200)	{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	goto loop;
//
loop_end:;
	if (*c == 13)
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}
#endif
/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_int(char *ccc, int *number, int *line_terminate_end_flag)
{
	char buffer[32/*20*/];
	char *ddd = buffer;
	int i = 0;
	while ((isdigit(*ccc))||('-'==(*ccc)))		/* 負数にも対応 / isdigit : 数字かどうかの判定 */
	{
		i++;
		if (i >= 32/*20*/)
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
/* ',' または '\n' が来たら終了 */

//
	if (13==(*ccc)) /* 改行コードは OD OA */
	{
		*line_terminate_end_flag = 1;
	}
	*ddd = 0;
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = -1; }
	else														{	*number = atoi(buffer); }
	return (ccc);
/*error*/
ne222:
	return ((char *)NULL);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	PARAM_01 = 0,
	PARAM_02,
	PARAM_03,
	PARAM_04,
	PARAM_05,
//	PARAM_06,/*未使用*/
//	PARAM_07,/*未使用*/
//	PARAM_08,/*未使用*/
	PARAM_MAX/*最大数*/
};

static void regist_script(
	char *command,
	char *c_p0,
//	int c_p1,	//c_pn[0]
//	int c_p2,	//c_pn[1]
//	int c_p3,	//c_pn[2]
//	int c_p4,	//c_pn[3]
//	int c_p5,	//c_pn[4]
//	int c_p6,	//c_pn[5]
//	int c_p7,	//c_pn[6]
	const int *c_pn,
	int chain)
{
/*
	基本形
	command para0,para1,para2,para3
*/
	S_SCRIPT *new_script;

	new_script	=	mmalloc(sizeof(S_SCRIPT));

	strcpy(new_script->para0, c_p0);
	new_script->para1		= /*c_p1*/c_pn[PARAM_01];	/* デフォルト */
	new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
	new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */
	new_script->para4		= /*c_p3*/c_pn[PARAM_04];	/* デフォルト */
//
	new_script->done	= 0;
	new_script->chain	= chain;
	new_script->next	= NULL;
//	new_script->data	= NULL;

	switch (*command)
	{
	case 'B':
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_script->command = SCP_BG_LOAD;			}	/* 背景ファイルの変更 */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_script->command = SCP_BGM;				}	/* BGM変更 */
		break;
	case 'C':
		if (0==tiny_strcmp(command, 			"CURSOR"))
		{
					if (0==tiny_strcmp(c_p0,	"home"))		{	new_script->command = SCP_CUR_HOME; 		}	/* カーソルの初期化 */
			else	if (0==tiny_strcmp(c_p0,	"load")) 		{	new_script->command = SCP_CUR_POP;			}	/* 記憶したカーソル位置の呼び出し */
			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_script->command = SCP_CUR_PUSH; 		}	/* カーソル位置の記憶 */
			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_script->command = SCP_CLICK;			}	/* クリック待ち */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->command = SCP_TEXT_WRITE_TEXT;	}	/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->command = SCP_BG_WRITE_TEXT;	}	/* BGウィンドウに漢字の文字を描く様に設定する。 */
		}
		break;
	case 'O':
					if (0==tiny_strcmp(command, "OBJ_MOVE"))	{	new_script->command = SCP_OBJ_MOVE; 		}	/* スプライトの移動 */
			else	if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_script->command = SCP_OBJ_LOAD; 		}	/* 汎用スプライトのロード */
			else	if (0==tiny_strcmp(command, "OBJ_SWAP"))	{	new_script->command = SCP_OBJ_SWAP; 		}	/* スプライトの再読み込み */
		break;
	case 'S':
		if (0==tiny_strcmp(command, 			"SCREEN"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_script->command = SCP_SCREEN_PANEL; 	}	/* スコアパネルの表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->command = SCP_SCREEN_TEXT;		}	/* テキスト画面の表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->command = SCP_SCREEN_BG;		}	/* 背景の表示/非表示 */
		}
		break;
	case 'T':
					if (0==tiny_strcmp(command, "TEXT"))		{	new_script->command = SCP_TEXT; 			}	/* メッセージウィンドウへの書き込み */
		break;
	}

	if (NULL==entry_script)
	{	entry_script		= new_script;		}
	else
	{
		entry_script->next	= new_script;
		entry_script		= entry_script->next;
	}

	if (NULL==sscript)
	{	sscript 			= entry_script; 	}
}

/*---------------------------------------------------------
	汎用性のまるでない読み込み関連。子関数
	-------------------------------------------------------
	標準入出力はpspでは遅すぎるのでsceの関数を使う。(ゲーム中の処理落ち軽減策)
	エラーチェックとか全然無いので注意の事
---------------------------------------------------------*/

//atic char buffer_text_1_line[128];	/* parth text, 1 line buffer */ 	/* 走査する行の取得 */
static char buffer_text_1_line[256];	/* parth text, 1 line buffer */ 	/* 走査する行の取得 */

#if 1
static unsigned long file_size;
static unsigned long file_seek;
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;

static void *my_fopen(const char *file_name/*, const char *dummy*/)
{
	SceUID fd;
	fd = sceIoOpen((char *)file_name, PSP_O_RDONLY, 0777);
	if (0 == fd)
	{
		goto error111;
	}
	file_size = sceIoLseek32(fd, 0, PSP_SEEK_END);
	file_seek = 0;

	malloc_buf = malloc(file_size);
	if (NULL == malloc_buf)
	{
		sceIoClose(fd);
		goto error111;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	sceIoRead( fd, malloc_buf, file_size);
	sceIoClose(fd);
//	my_buf = malloc_buf;
//
	return (malloc_buf);
error111:
	return (NULL);
}
static int my_fgets(void/*char *buffer_name, int num, char *wfp*/)
{
	int ii;
ii=0;
	char aaa;
//	char bbb;
//bbb=0;
	fgets_loop:;
	aaa = /*buffer_name*/buffer_text_1_line[ii] = /*my_buf*/malloc_buf[file_seek]/*(*my_buf)*/;
//	my_buf++;
	ii++;
	file_seek++;
	if (0x0a==aaa)	return (1);
	if (file_size < file_seek)	return (0)/*NULL*/;
	goto fgets_loop;
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
}
static void my_fclose(void/*void *wfp*/)
{
	free(malloc_buf);
}
#else
	#define my_fopen	fopen
	#define my_fgets	fgets
	#define my_fclose	fclose
#endif
/*---------------------------------------------------------
	スクリプト読み込み
	-------------------------------------------------------
	この関数は、仕様上「とても処理落ちします」

---------------------------------------------------------*/

static int load_scenario(char *src_filename)
{
	load_script_free();
/*
	基本形
	command para0,para1,para2,para3
*/
	char filename[128];
//	sp rintf(filename, "%s/text/%s.txt", data_dir, src_filename);
	strcpy(filename, DIRECTRY_NAME_DATA "/text/");
	strcat(filename, src_filename);
	strcat(filename, ".txt");

//	FILE *fp;
	if (NULL == (/*fp =*/my_fopen(filename/*,"r"*/)))		/* 開けなかったとき */
	{
		return (0);
	}

	int opecode_entrys		= 0;	/* 命令がが書き込まれているかどうか。 */
	int line_num			= 0;	/* 行番号 */
	int opecode_chains		= 0;	/* 連続した命令 */
	while (/*NULL*/0 != my_fgets(/*buffer_text_1_line,255,fp*/))
	{
		/****************** script_data 用 ******************/
		char char_command[16/*15*/];		/* 基本コマンド */
		char c_p0[256/*200*/];
		int c_pn[PARAM_MAX/*6*/];
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				c_pn[kk] = -1;	/* 無指定の場合 を判別する為 */
			}
		}
		/****************** script_search 用 ****************/
		char *c = NULL; 			/* 走査中の行の分析用 */
		int end_arg=0;				/* 行末 == 引数の取得の中止 */
		c = buffer_text_1_line; 	/* 先頭アドレスを渡すよ */

		line_num++;

		/* skiped lines. */
		#if 0
		/* '\n'が悪いのか巧くいかない(???) */
		if (*c=='\n')		{	continue;	}
		while (isspace(*c)) {	c++;		}
		#else
		{my_isspace:;
			if (*c<=' ')
			{
				c++;
				if (*c==0x0a)
				{	/*goto loop;*/continue; }	/* skiped null line. */
				else
				{	goto my_isspace;	}
			}
		}
		#endif
		if (*c=='#')		{	continue;	}
		if (*c=='-')		{	c++;	opecode_chains++;	opecode_chains &= 0x0f; }	/* ワークが16までしかないので最大16命令 */
		else				{	opecode_chains = 0; }
	#if (1==USE_PARTH_DEBUG)
		#define GOTO_ERR_WARN goto err_warn
	#else
		#define GOTO_ERR_WARN continue
	#endif
		/* parth start */
		c = load_command(c, char_command, &end_arg);		/* 基本コマンド名称(オペコード)読み込み  */
		if (NULL==c)										{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		if (!end_arg)/* 行末 */
		{
			if (*c++ != ' ')								{	GOTO_ERR_WARN;	}	/* 区切り */
			c = load_script_get_str(c, c_p0, &end_arg); 							/* 文字列コマンド(オペランド)読み込み  */
			if (NULL==c)									{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		}
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				if (!end_arg)
				{
					if (*c++ != ',')						{	GOTO_ERR_WARN;	}	/* 区切り */
					c = load_int(c, &c_pn[kk], &end_arg);							/* 数値コマンド(オペランド)読み込み  */
					if (NULL==c)							{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
				}
			}
		}
	//	regist_script(char_command, c_p0, c_pn[0],c_pn[1],c_pn[2],c_pn[3],c_pn[4],c_pn[5],c_pn[6], opecode_chains);
		regist_script(char_command, c_p0, c_pn, opecode_chains);
		opecode_entrys++;
		continue;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num);
		continue;
	#endif
	}
	my_fclose (/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		#if 0
		//ps pDebugScreenPrintf("can't entrys from this file %s",filename);
		//sc eKernelDelayThread(3000000);
		#endif
		error(ERR_WARN, "can't entrys from this file %s",filename);
		return (0);
	}
	return (1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

//static extern int my_string_offset;
//static extern void clear_my_string_offset(void);
static void script_reset(void)
{
	int i;
	for (i=0; i<SPRITE_MAX/*20*/; i++)
	{
		remove_sc_sprite(std_obj[i]);
	}
//	remove_sc_sprite(std_obj[SPRITE_tachie_r]);
//	remove_sc_sprite(std_obj[SPRITE_tachie_l]);
	if (NULL != bg_story_window)	{	SDL_FreeSurface(bg_story_window);	bg_story_window 	= NULL; }
//	if (NULL != msg_window) 		{	SDL_FreeSurface(msg_window);		msg_window			= NULL; }

//	if (NULL != filter_window)		{	SDL_FreeSurface(filter_window); 	filter_window		= NULL; }
//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); font_color_bitmap	= NULL; }
	load_script_free();

//	count_char=0;			/* カーソル位置 */
//	cursor=0;
	home_cursor();			/* カーソルをホームポジションへ移動 */
	cursor_x_chached = 0;	/* カーソル位置 保存用 */
	cursor_y_chached = 0;	/* カーソル位置 保存用 */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* 背景α値用 */
static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window = load_local01(filename, bg_story_window, alpha);
	{
		bg_story_window_image	= (UINT16 *)bg_story_window->pixels;
		bg_story_window_pitch	= bg_story_window->pitch;
		bg_story_window_width	= bg_story_window->w;
	}
	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
}
static int load_my_bg(char *filename, int alpha, int fade_command, int set_alpha_speed)
{
	static int bg_alpha_speed;		/* 背景α値用 */
	switch (fade_command)
	{
	case (-1):
		load_bg_aaa(filename, alpha);
	//	ssc->done = 1;
		return (1); /* 処理完了 */
//		break;
	case 1:/*fade_in_command*/
		if (inits)/*fade_in_command_start*/
		{
		//	if (filename)
			{
				load_bg_aaa(filename, 0/*alpha*/);
			}
		//	ssc->done = 1;
			//
			bg_alpha = 0;
		//	if (-1 == set_alpha_speed)/*パラメーター省略時*/
			{
				set_alpha_speed = 5;
			}
			bg_alpha_speed = set_alpha_speed;
		//	inits=0;
			is_bg				= 1;
		}
		else
		{
//			psp_clear_screen();
//			SDL_SetAlpha(bg_story_window, SDL_SRCALPHA, bg_alpha);
//			SDL_BlitSurface(bg_story_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
			bg_alpha += bg_alpha_speed/*5*/;
			if (250 < bg_alpha)
			{
				bg_alpha = 255;
				#if 0/*???*/
				/*	何か良くわからないけど、ここはおかしい???。 */
				if (NULL != bg_story_window)
				{
					unloadbmp_by_surface(bg_story_window);
				}
				#endif
				return (1); 	/* 処理完了 */
			}
		}
		break;
//	case 2:/*fade_out_command_start*/
//		if (inits)
//		{
//			inits=0;
//		}
//		else
//		{
//		//	if ()
//			{
//			//	return (1); /* 処理完了 */
//			}
//		}
//		break;
	}
	return (0); 	/* 処理中 */
}


/*---------------------------------------------------------
	スプライトオブジェクトの描画/動作
---------------------------------------------------------*/

static void draw_my_sprite(int start, int end)
{
	int nnn;
	for (nnn=start; nnn<=end; nnn++)
	{
		if (NULL != std_obj[nnn])
		{
		//	if (std_obj[nnn]->flags)	/* 汎用スプライトの表示/非表示 */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0)/*std_obj[nnn]->w*std_obj[nnn]->aktframe*/;
				src_r.y = (0);
				src_r.w = std_obj[nnn]->w;
				src_r.h = std_obj[nnn]->h;
				dst_r.x = std_obj[nnn]->x;
				dst_r.y = std_obj[nnn]->y;
				SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r);
			}
			/* 動き/アニメーション */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_draw_the_script(void)
{
	/* ----- 描画系 ----- */
	/* 1. まず背景を描く */
	if (is_bg)
	{
		if (255 > bg_alpha)
		{
			SDL_SetAlpha(bg_story_window, SDL_SRCALPHA, bg_alpha);
		}
		SDL_BlitSurface(bg_story_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
	}
	/* 2. 次に立ち絵を描く */
	draw_my_sprite(SPRITE_tachie_l,SPRITE_tachie_l);/* 立ち絵 1枚 */
	draw_my_sprite(SPRITE_tachie_r,SPRITE_tachie_r);/* 立ち絵 1枚 */
}


/*---------------------------------------------------------

---- やりたいこと ----
	1フレームでは終わらない処理があったときのためにこのコマンドからの命令には全て
	終わったことを知らせる引数を付けておくこと。=> doneに代入で終了。
	常にchainを確認し、0以外の値が入っていたら次の関数も実行する(nextを辿る)。
---------------------------------------------------------*/

static int n9;						/* 次の命令に進めるかの判定 */

static int return_the_script;
static /*int*/void work_the_script(void)
{
	static int tmp_all[16/*15*/];	/* ご自由にお使いください。 */
	S_SCRIPT *ssc = start_script;
	n9 = 0; 					/* 次の命令に進めるかの判定 */
	if (inits)
	{
		int i;
		for (i=0; i<16/*15*/; i++)
		{
			tmp_all[i] = 0;
		}
	}
//
	int shot_ositenai;
	shot_ositenai = 1;	/* ショット押してない */
	if (0 == my_pad)/* 今は押してない */
	{
		if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	/* キャンセル入力 */
		{
			while ((NULL != ssc->next)
			//	&& (SCP_JUMP != ssc->command)/*廃止*/
				)
			{
				ssc->done = 1;
				ssc=ssc->next;
			}
			if (NULL == ssc->next)
			{
				script_reset();
				return_the_script=(1);
				return /*(1)*/;
			}
		}
		if (my_pad_alter & PSP_KEY_SHOT_OK) 		/* ＯＫ入力 */
		{
			shot_ositenai = 0;	/* ショット押したよ */
		}
	}
//
	/* ----- スクリプトコマンド解釈(実行)系 ----- */
	while (1)
	{
		if (0==ssc->done)
		{
			switch (ssc->command)
			{
			// [テキストのカーソル制御]
			case SCP_CUR_POP:		/* カーソル位置、復元 */
			//	count_char=cursor;
				cursor_x = cursor_x_chached;
				cursor_y = cursor_y_chached;
				ssc->done = 1;
				break;
			case SCP_CUR_PUSH:		/* カーソル位置、記憶 */
			//	cursor=count_char;
				cursor_x_chached = cursor_x;
				cursor_y_chached = cursor_y;
				ssc->done = 1;
				break;
			case SCP_CUR_HOME:		/* カーソルの初期化 */
			//	count_char=0;
				home_cursor();				/* カーソルをホームポジションへ移動 */
				ssc->done = 1;
				break;
			// [テキスト表示/クリック待ち]
			case SCP_CLICK:
				if (0 == shot_ositenai)
				{	ssc->done = 1;	}
				break;
			case SCP_BG_WRITE_TEXT:
				set_write_text(1);		/* BGウィンドウに漢字の文字を描く様に設定する。 */
				ssc->done = 1;
				break;
			case SCP_TEXT_WRITE_TEXT:
				set_write_text(0);		/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
				ssc->done = 1;
				break;
			case SCP_TEXT:
				if (0 == tmp_all[(ssc->chain)])
				{
					if (0 == shot_ositenai) 	{	ssc->para2=0;	}	/* ショット押したら、残りは全部書く */
					#if (1)
				//	if (SDL_MUSTLOCK(sdl_lock_surface)) 			{	SDL_LockSurface(sdl_lock_surface);			}	/* ロック */
					if (SDL_MUSTLOCK(msg_window)) 					{	SDL_LockSurface(msg_window);			}	/* ロック */
					#endif
					tmp_all[(ssc->chain)] = print_kanji000(/*sdl_lock_surface,*/ /*ssc->data,*/ ssc->para0, ssc->para1, ssc->para2);
					#if (1)
				//	if (SDL_MUSTLOCK(sdl_lock_surface)) 			{	SDL_UnlockSurface(sdl_lock_surface);		}	/* ロック解除 */
					if (SDL_MUSTLOCK(msg_window)) 					{	SDL_UnlockSurface(msg_window);		}	/* ロック解除 */
					#endif
				}
				else	/*if (1==tmp_all[(ssc->chain)])*/
				{
					if (0x00 == (ssc->para3 & 0xf8))	/* 0レジスタと比較 */
					{
						if ( 0x00 == ( (ssc->para3 & shot_ositenai) & 0x01) )	/* 0レジスタと比較 */
						{
							if (0x00 != (ssc->para3 & 0x02))	/* 0レジスタと比較 */
							{
						//	//	count_char=0;
								home_cursor();				/* カーソルをホームポジションへ移動 */
							}
						//	else	{	;	}	//cursor_continue = 1;	/* カーソル継続 */
							if (0x00 != (ssc->para3 & 0x04))	/* 0レジスタと比較 */
							{
								msg_window_clear();  	/* メッセージウィンドウの内容を消す。 */
							//	msg_window_init();/*???*/		/* ウィンドウ初期化 */
							}
						//	else	{	;	}
							ssc->done = 1;
							tmp_all[(ssc->chain)]=0;
						}
					}
				}
				break;
#if 0
/*
	switch (ssc->para3)
	w: 0:しない 	   1:ウィンドウ初期化
	c: 0:カーソル継続  1:カーソル初期化
	b: 0:実行。 	   1:ボタンを押したら実行。
		   wcb
	case 0:000: 何もせずに		 次の命令へ、カーソル継続
	case 1:001: ボタンを押したら 次の命令へ、カーソル継続
	case 2:010: 何もせずに		 次の命令へ、カーソル初期化
	case 3:011: ボタンを押したら 次の命令へ、カーソル初期化
	case 4:100: 何もせずに		 次の命令へ、カーソル継続	 ウィンドウ初期化
	case 5:101: ボタンを押したら 次の命令へ、カーソル継続、  ウィンドウ初期化
	case 6:110: 何もせずに		 次の命令へ、カーソル初期化、ウィンドウ初期化
	case 7:111: ボタンを押したら 次の命令へ、カーソル初期化、ウィンドウ初期化
*/
#endif

			case SCP_BG_LOAD:
			//	bg_story_window = load_local01(ssc->para0, bg_story_window, ssc->para1);
			//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
			//	ssc->done = 1;
				ssc->done=load_my_bg(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				break;
			case SCP_OBJ_LOAD:	/* 汎用絵  立ち絵L	立ち絵R */
				load_my_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3/*,ssc->data*/);
				ssc->done = 1;
				break;
			case SCP_OBJ_MOVE:
				ssc->done=do_move_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->para4);
				if (ssc->done == 2/*-1*/)
				{
					#if 0
					//ps pDebugScreenPrintf("no sprite in No.%s\n",ssc->para0);
					//sc eKernelDelayThread(2000000);
					#endif
					return_the_script=(1);
					return /*(1)*/;
				}
				break;
			case SCP_OBJ_SWAP:
				swap_my_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				ssc->done = 1;
				break;
			case SCP_SCREEN_BG: 		is_bg					= ssc->para1;	ssc->done = 1;	break;
			case SCP_SCREEN_TEXT: 		draw_script_screen		= ssc->para1;	ssc->done = 1;	break;
			case SCP_SCREEN_PANEL:		draw_side_panel 		= ssc->para1;	ssc->done = 1;	break;
			case SCP_BGM:				play_music_num( (ssc->para1) ); 	ssc->done = 1;	break;
			}
		}
		if (0 == ssc->done) 	{	n9++;	}		/* 継続フラグ */
		if (NULL==ssc->next)	{	break;	}		/* nextがNULLの場合 */
		if ((ssc->chain >= ssc->next->chain) || (0==ssc->chain))	{ break;	}	/* 現chainが次chainより大きい場合 */
		ssc = ssc->next;
	}
//
	/* ----- 描画系 ----- */
	s_draw_the_script();
//
	inits = 0;
	if (0 == n9)					/* 次の命令の許可 */
	{
		inits = 1;
		while (start_script->done)		/* 次の命令へ */
		{
			if (NULL == start_script->next)
			{
				script_reset();
				return_the_script=(1);
				return /*(1)*/;
			}
			start_script=start_script->next;
		}
	}
//
	return_the_script=(0);
	return /*(0)*/;
}


/*---------------------------------------------------------

---------------------------------------------------------*/

void script_display(void)
{
	/*ボス戦闘後イベント*/	/*ボス戦闘前イベント*/
	{
		work_the_script();
	//	if (1==return_the_script)	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=return_the_script)
		{
			draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
			{
				PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
				pd->state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));/*off*/
				pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 	/*pd->bo ssmode=B08_START;*/	/*	pd->bo ssmode=B09_STAGE_LOAD;*/
			}
		}
		/*else	{	draw_the_script();}*/
	}	// [***090313	追加
}

/*---------------------------------------------------------

---------------------------------------------------------*/
//	380/*scenario_width*/=width;
//	if (NULL != bg_name)
//	{
//		load_bg_aaa(bg_name, 0/*alpha*/);
//	//	bg_story_window = load_local01(bg_name, bg_story_window, 0);
//	//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
//	}

	/* ウィンドウの半透明枠 */
//	std_window			= loadbmp0("fonts/window.png", 1, 1);/*2*/
	#if 0
//	msg_window_clear(); 	/* メッセージウィンドウの内容を消す。 */
//	msg_window_init();/*???*/
//	home_cursor();				/* カーソルをホームポジションへ移動 */
	#else
	#endif


static int script_start(char *filename)		/* シナリオファイル名と背景ファイル名 */	/*, char *bg_name, int width*/
{
	if (0 == load_scenario(filename))
	{
		return (0);
	}
	script_message_window_clear();
	cursor_x_chached	= 0;	/* カーソル初期化 */
	cursor_y_chached	= 0;	/* カーソル初期化 */

//	msgw_rect_init(20,182);
	inits				= 1;
	is_bg				= 0;
	draw_script_screen	= 1/*0*/;
//	count_char			= 0;
//	cursor				= 0;

	#if 1
	/* std_obj[]初期化 */
//	std_obj[SPRITE_tachie_r]			= NULL;
//	std_obj[SPRITE_tachie_l]			= NULL;
	int i;
	for (i=0; i<SPRITE_MAX/*20*/; i++)
	{
		std_obj[i]		= NULL;
	}
	#endif
	start_script		= sscript;
	return (1);
}
/*---------------------------------------------------------

---------------------------------------------------------*/
extern int select_player;
void script_load(void/*int mode*/)
{
	char file_name[16/*10*/];
	strcpy(file_name,"Z/sZ1");
	file_name[0] = ('0'+select_player);
	file_name[3] = ('0'+player_now_stage);
//
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	{
		;// file_name[4] = '1';
	}
	else
	{
		file_name[4] = '0';
		if (0!=boss_bgm_mode)
		{	/* ボスイベント後にボス曲 */
			set_music_volume(80);/* 曲音量低下 */
		}
		else
		{	/* ボスイベント前にボス曲 */
			play_music_num( (BGM_10_boss1-1)+player_now_stage );/* ボス曲鳴らす */
		}
	}
//
	if (0 == script_start(file_name))		// ファイルがない場合はイベントを飛ばす, /*NULL,*/ (GAME_WIDTH)	/*380*/
	{
		pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B09_STAGE_LOAD;*/
	}
	else
	{
		pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;	/*on*/		/*pd->bo ssmode=B00_NONE;*/ /*B06_AFTER_EVENT*/
		#if 0
		/* シナリオ中にボムが発生してしまう。バグがあるので。 */
		pd_bomber_time = 0;
		#endif
	}

//
//	if (0 == script_start(file_name)), /*NULL,*/ 380
//			{	pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B08_START;*/ }
//	else	{	pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;			/*pd->bo ssmode=B00_NONE;*/ /*B03_BEFORE_EVENT*/	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

enum /*_story_state_*/
{
	STORY_INIT=0,
	STORY_WORKS,
	STORY_QUIT
};

void story_work(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case STORY_INIT:
//		bg_alpha=0;
		script_start("story");	//	/*,NULL*/ /*"story/arasuzi.jpg"*/ /*"story_bg.jpg"*/, 480
//		bg_alpha=0;
		psp_loop++;//newsta te(ST_STORY,STORY_FADEIN,0);
		break;
	case STORY_WORKS:
		psp_clear_screen();
		work_the_script();
		/*draw_the_script();*/
		if (return_the_script)
		{
			psp_loop++;//newsta te(ST_STORY,STORY_QUIT,0);
		}
		break;
	case STORY_QUIT:
			#if 1/*???*/
			/*	何か良くわからないけど、ここはおかしい???。 */
			if (NULL != bg_story_window)
			{
				unloadbmp_by_surface(bg_story_window);
			}
			#endif
//		bg_alpha = 0;
		inits=1;
		draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
		//
		menu_cancel_and_voice();	/* メインメニューに戻る */
		break;
	}
}


/*---------------------------------------------------------
	pspでは開放が正常動作出来ないので、起動時に確保して(終了時まで)開放しない
---------------------------------------------------------*/

void script_system_init(void)/* 組み込み */
{
	#if 1
//	msg_window			= NULL; 	/* メッセージウィンドウ */
//	std_window			= NULL; 	/* ウィンドウの半透明枠 */
//
	sscript 			= NULL; 	/* 命令保持用 */
	start_script		= NULL; 	/* 命令実行用 */
//
	draw_script_screen			= 0;	/* せりふウィンドウ表示フラグ */
	is_bg				= 0;	/* 背景表示フラグ */
//
//	count_char			= 0;
//	cursor				= 0;
	home_cursor();				/* カーソルをホームポジションへ移動 */
	cursor_x_chached	= 0;	/* カーソル位置 保存用 */
	cursor_y_chached	= 0;	/* カーソル位置 保存用 */
//
	#endif
	bg_alpha			= 255;	/* 255==初期値 */

	/* SDL_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
//
		#if 1
		/*???*/
	//	font_color_bitmap=loadbmp(buffer);
	//	esc_len=0;
		{
			char fn[128/*64*/];
		//	strcpy(fn, data_dir);	strcat(fn, "/fonts/" "font_bg16.png" );
			strcpy(fn, DIRECTRY_NAME_DATA "/fonts/" "font_bg16.png" );
			font_bg_bitmap = IMG_Load(fn);/*ここでロードすると確実に処理落ちするよ*/
		}
		font_bg_bitmap = SDL_DisplayFormat(font_bg_bitmap);
		#endif

//
	/* pspでは SDL_FreeSurface() が正常に動作しませんので、このままではメモリ不足？でハングアップします。 */
	/*msg_window_init()*/
//	if (NULL != msg_window) 	{	SDL_FreeSurface(msg_window);	msg_window = NULL;	}
	msg_window = SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* メインメモリへ確保する */
			(320)/*340*/,
			(56)/*70*/ /*100*/,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window); }
	msg_window_image	= (UINT16 *)msg_window->pixels;
	msg_window_pitch	= msg_window->pitch;
	msg_window_width	= msg_window->w;
	if (SDL_MUSTLOCK(msg_window))	{	SDL_UnlockSurface(msg_window);	}
	SDL_SetColorKey(msg_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
/* psp では 確保先が VRAM か メインメモリ かの2種類しかありません。
そのうち VRAM 確保された場合、pspのSDL固有の機能であるSDLの画像キャッシュ機能が勝手に働くので、
これが原因で変な事になります。(メインメモリ確保された場合、この問題は起きません)
SDL_SWSURFACE が指定されていれば、必ずメインメモリへ確保される訳ではないようです。
現状この部分は VRAM 確保されているみたいです。 VRAM 確保されないと Alpha が使えないのかも知れません？？？。
 */
}

/*---------------------------------------------------------
	終了時に開放する部分...何だけど、
	ハングアップしたりするので、取り敢えず無効になってる。
	(開放しなくても終了すれば、OSがメモリ回収するので問題ないって言えば問題ない)
---------------------------------------------------------*/

void script_system_exit(void)/* 外す */
{
	/*msg_window_init()*/
//	if (NULL != msg_window) 	{	SDL_FreeSurface(msg_window);	msg_window = NULL;	}
	//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); 		font_color_bitmap	= NULL; }
//		if (NULL != font_color_bitmap)	{	unloadbmp_by_surface(font_color_bitmap);	font_color_bitmap	= NULL; }
}
