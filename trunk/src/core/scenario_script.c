
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

#define USE_2ND_SCREEN		(0)


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

static Uint32 getpixel16(SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	return (*(Uint16 *)p);
}

static void putpixel16(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	*(Uint16 *)p=pixel;
}
static void draw_shinonome_moji(SDL_Surface *drawmap, SDL_Surface *backmap, int x, int y, int code_offset, int hidari_gawa)
{
//	if (1==hidari_gawa) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=hidari_gawa)
	{
		x += FONT_WIDTH;
	}
	if (x > (drawmap->w-FONT_WIDTH) )	/* 範囲外なら */
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
				putpixel16(drawmap, dx+x, dy+y, dot);
			}
		}
	}
}

/*---------------------------------------------------------
http://openlab.ring.gr.jp/efont/shinonome/
	shift jis文字コード規格 を 東雲文字コード規格 へ変換後、描画する
---------------------------------------------------------*/

static void draw_sjis_kanji(SDL_Surface *drawmap, SDL_Surface *backmap, int x, int y, int high_byte, int low_byte)
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
		draw_shinonome_moji(drawmap, backmap, x, y, (idx8), 0);/*low_byte<<4*/
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
		{
			/* 東雲文字コード規格 の存在マップ */
			//#define FDxx (47)/* 半角2バイト漢字文字 */
			static	const	signed char/*int*/ sinonome_sjis_bank_offset[0x40] =
			{// +0	+1	+2	+3	 +4  +5  +6  +7   +8  +9  +a  +b   +c  +d  +e  +f
				-1,  0,  1,  2,   3, -1, -1,  4,   5,  6,  7,  8,	9, 10, 11, 12,		/* 0x80xx */
				13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,  25, 26, 27, 28,		/* 0x90xx */
				29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, -1,  -1, -1, -1, -1,		/* 0xe0xx */
				-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1		/* 0xf0xx */
			//	以下の領域は東雲フォント16にはフォントデーターがない(eaa4が最終データーだった)
			//	29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, -1,  -1, 40, 41, -1,		/* 0xe0xx */	東雲フォント16にはフォントデーターがない
			//	-1, -1, -1, -1,  -1, -1, -1, -1,  42, 43, 44, 45,  46, 47, -1, -1		/* 0xf0xx */	東雲フォント16にはフォントデーターがない
			};
			idx = sinonome_sjis_bank_offset[high_byte & 0x3f];
		}
		/* 存在マップが -1 の場合は、 東雲文字コード規格 にない文字なので表示できない */
		if (-1 == idx)
		{
			return;
		}
		//
		idx = (16*10)/*半角文字をスキップ*/ + (idx*2*192)/*codetbl１個あたり192文字*/;
		#if (1==USE_HARF_KANJI)
		if (1 == size_of_kanji(high_byte))	// 2バイト半角
		{
			idx += (low_byte);
		//	code_offset <<= 4;	//code_offset *= 16;	/* 9 */ 	// ここでの9は多分1つの文字に使うバイト数。(1byte == 8bit)*9=72 == 6*12
			draw_shinonome_moji(drawmap, backmap, x, y, (idx) , 0);
		}
		else	/* if ( 2 == size_of_kanji(high_byte))*/	// 2バイト全角
		#endif
		{
			idx += (low_byte+low_byte);/* x2 */
			draw_shinonome_moji(drawmap, backmap, x, y, (idx) , 0); /* 右半分 */
			draw_shinonome_moji(drawmap, backmap, x, y, (idx) , 1); /* 左半分 */
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
			font_color_bitmap = IMG_Load(fn);/*ここでロードすると確実に処置落ちするよ*/
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
/*extern*/ static int print_kanji(SDL_Surface *drawmap, SDL_Rect *rect, const char *str, int color_type, int wait)
{
	font_color_number = (color_type & 0x0f);

	static int count_width=1/* 0 (割り算なので0は危険) */;/*???*/
//	static int esc_len;
	if (0 == my_string_offset)	//初期化
	{
		count_width=(int)((rect->w)/FONT_WIDTH)+2;
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
			if (SDL_MUSTLOCK(drawmap))				{	SDL_LockSurface(drawmap);			}	/* ロック */
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
			xx = rect->x+(cursor_x)*(FONT_WIDTH);
			yy = rect->y+(cursor_y)*(FONT_HEIGHT+2);
		//	パフォーマンスが低下するのでなるべくロックしない。(ロックは最小限に)
		/* 旧ロック */
			if (is_kanji_1st(high_byte)) /* shift jis 漢字 */
			{
			/*unsigned char*/int low_byte;	low_byte = (*(str+my_string_offset+1) & 0xff);
			//	draw_sjis_kanji(drawmap, font_color_bitmap, xx, yy, high_byte, low_byte);
				draw_sjis_kanji(drawmap, font_bg_bitmap, xx, yy, high_byte, low_byte);
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
				//	draw_sjis_kanji(drawmap, font_color_bitmap, xx, yy, 0, high_byte);
					draw_sjis_kanji(drawmap, font_bg_bitmap, xx, yy, 0, high_byte);
					my_string_offset++;
					cursor_x++;
				}
			}/* 旧ロック解除 */
			/* 改行処理 */
			if ( (count_width-1) < cursor_x)
			{
				cursor_x=0;
				cursor_y++;
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
			if (SDL_MUSTLOCK(drawmap))				{	SDL_UnlockSurface(drawmap); 			}	/* ロック解除 */
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
//		CLCURSOR	/* カーソルの初期化 */
//		LOADCUR 	/* 記憶したカーソル位置の呼び出し */
//		SAVECUR 	/* カーソル位置の記憶 */
// [テキスト表示/クリック待ち]
//		CLICK		/* クリック待ち */
//		BGTEXT		/* 背景にテキストを打ち込む */
//		TEXT		/* メッセージウィンドウへの書き込み */


//		FILTER
//		JUMP		/* pauseボタンを押した時のjump先。 */
//		LOADBG		/* 背景ファイルの変更 */
//		LOADL		/* 立ち絵左 スプライトのロード */
//		LOADR		/* 立ち絵右 スプライトのロード */
//		LOADSP		/* 汎用スプライトのロード */
//		MOVESP		/* スプライトの移動 */
//		PARAMSP
//		RELOADSP	/* スプライトの再読み込み */
//		SUBG		/* 背景の表示/非表示 */
//		SUFILTER	/* フィルター表示/非表示&透過度 */

//		SUL 		/* 立ち絵左スプライトの表示/非表示 */
//		SUR 		/* 立ち絵右スプライトの表示/非表示 */
//		SUSPRITE	/* 汎用スプライトの表示*/

//		SUTWINDOW	/* 強調立ち絵ウィンドウの表示/非表示 para2はスコアパネルのon/off */
//		SUWINDOW	/* メッセージウィンドウの表示/非表示 para2はbpp速度。*/

//		TWINDOW 	/* 立ち絵強調ウィンドウの初期化 */
//		WAIT		/* wait */

enum
{
// [テキストのカーソル制御]
	SCP_LOADCUR,
	SCP_SAVECUR,
	SCP_CLCURSOR,
// [テキスト表示/クリック待ち]
	SCP_CLICK,
	SCP_BGTEXT,
	SCP_TEXT,
//

	SCP_FILTER,
	SCP_JUMP,
	SCP_LOADBG,
	SCP_LOAD_LR,
	SCP_LOADSP,
	SCP_MOVESP,
	SCP_PARAMSP,
	SCP_RELOADSP,
	SCP_SUBG,
	SCP_SUFILTER,
	SCP_SUL,
	SCP_SUR,
	SCP_SUSPRITE,
	SCP_SUTWINDOW,
	SCP_SUWINDOW,
	SCP_TWINDOW,
	SCP_WAIT
};

typedef struct _scenario_script
{
	int command;
	int done;		// 終わったかどうか
	int chain;		// 同時実行かどうか
					// 0=違うよ 1=1つ目 2=2つ目
	void *data;
//
	int para1;
	int para2;
	int para3;
	struct _scenario_script *next;
//
	char para0[(7*32)/*200*/];/* 32の倍数 */
} S_SCRIPT;/* == 256bytes */


typedef struct /*_sc_sprite*/
{
	SDL_Surface *img;				/* Images */
	Uint8 frames;					/* アニメーションさせる予定 */
	Uint8 anim_speed;				/* 上に同じ */
	Uint8 anim_type;				/* どういう風にアニメーションさせるか */
	Uint8 anim_time;				/* アニメーション回数 */
	Uint8 alpha;					/* alpha値 */
	Uint8 flags;					/* 0:非表示, 1:表示, 2:tachie_window(2nd screen)に表示	ここでは表示させるかどうかだけだよ */
	int aktframe;					/* 現在のフレーム */
	int anim_count; 	/* アニメーションのタイミング */
	int/*int*/ x;		/* x座標だよ */
	int/*int*/ y;		/* y座標だよ */
	int w;							/* 幅 */			//	int cw; 	/* 中心座標(幅)だよ */
	int h;							/* 高さ */			//	int ch; 	/* 中心座標(高さ)だよ */
	int move_wait;					/* 動きがあったときの制御用 */
	int target_x;					/* 目標座標 */
	int target_y;					/* 目標座標 */
	int x256;						/* 精度確保用(256固定小数点形式) */
	int y256;						/* 精度確保用(256固定小数点形式) */
	int angle512;		/* 正確な方向 */	//	int r_course;	/* 大体の方向 */
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
#if (1==USE_2ND_SCREEN)
static SDL_Surface *tachie_window/*=NULL*/; 	/* 優先立ち絵ウィンドウ */
#endif /* (1==USE_2ND_SCREEN) */
static SDL_Surface *bg_story_window;			/* スクリプトにおける背景 */

static SDL_Surface *filter_window;				/* 演出用ウィンドウ */
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

#if (1==USE_2ND_SCREEN)
static int is_tachie_window/*=0*/;				/* 立ち絵強調表示フラグ */
#endif /* (1==USE_2ND_SCREEN) */
static int is_bg/*=0*/; 						/* 背景表示/非表示フラグ */
static int is_filter/*=0*/; 					/* フィルター表示フラグ */
//static int 380/*scenario_width*/; 			/* 横幅 */

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/
extern int tiny_strcmp(char *aaa, const char *bbb);
static int cha_search(char *str)
{
	int n;
	if (0==tiny_strcmp(str, "right"))		{	return (-2);	}
	else if (0==tiny_strcmp(str, "left"))	{	return (-3);	}
	else
	{
		char buffer[32/*20*/];
		char *d = buffer;
		while (isdigit(*str))		{	*d++ = *str++;	}
		n=atoi(buffer);
		if (0<=n && n<15)			{	return (n); }
	}
	return (-1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_Surface(char *filename, int alpha)
{
	char fn[128];
//	strcpy(fn, data_dir);	strcat(fn, "/scenario/");
	strcpy(fn, DIRECTRY_NAME_DATA "/"/*"scenario/"*/);
	strcat(fn, filename);
//
	SDL_Surface *s1;/*temporaly*/
	SDL_Surface *s2;/*result*/
//
	s1 = IMG_Load(fn);/*ここでロードすると確実に処置落ちするよ*/
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant load image %s:"/*" %s"*/,fn/*,SDL_GetError()*/);
		return (NULL);
	}
	if ((-1==alpha) || (0==alpha))
	{
		s2 = SDL_DisplayFormat(s1);
		SDL_SetColorKey(s2,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	else
	{
		s2 = SDL_DisplayFormatAlpha(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
		return (NULL);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_local(char *str, SDL_Surface *s, int alpha)
{
	char fn[128];
//	strcpy(fn, data_dir);	strcat(fn, "/scenario/");
	strcpy(fn, DIRECTRY_NAME_DATA "/"/*"scenario/"*/);
	strcat(fn, str);
	SDL_Surface *tmp = NULL;

	if (NULL != s)
	{
		SDL_FreeSurface(s);
		s = NULL;
	}

	tmp = IMG_Load(fn);/*ここでロードすると確実に処置落ちするよ*/
	if ( NULL == tmp )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant load image %s:"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	if (	(-1==alpha) /* スクリプト中で省略した場合(デフォルト) */
		|| (0==alpha)	/* 明示した場合 */
	)
	{
		/* アルファ使わない */
		s = SDL_DisplayFormat(tmp);
		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000); 	/* カラーキー(抜き色、透明色)は黒 */
	}
	else
	{
		/* アルファ使う */
		s = SDL_DisplayFormatAlpha(tmp);
	}
	if ( NULL == s )
	{
		CHECKPOINT;
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	SDL_FreeSurface(tmp);
	tmp = NULL;
	return (s);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void remove_sc_sprite(SC_SPRITE *s)
{
	if (NULL != s)
	{
		SDL_FreeSurface(s->img);
		free (s);
		s = NULL;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void reload_sc_sprite(char *filename, int no, int frames, int alpha)
{
	SC_SPRITE *sc;
	if (no==-1) 		{	return; 		}
//	else if (no==-2)	{	sc=std_obj[SPRITE_tachie_r/*32-2*/];	}	/* right */
//	else if (no==-3)	{	sc=std_obj[SPRITE_tachie_l/*32-3*/];	}	/* left */
	else				{	sc=std_obj[(no)&(SPRITE_MAX-1)];	}	/* 汎用スプライト */
	if (NULL==sc)		{	return; 		}

	SDL_FreeSurface(sc->img);

	SDL_Surface *tmp = NULL;
	tmp 		= load_local(filename,tmp,alpha);
	sc->img 	= tmp;
	sc->frames	= frames;
	sc->w		= (tmp->w/frames);	//sc->cw		= ((sc->w)>>1);
	sc->h		= (tmp->h); 		//sc->ch		= ((sc->h)>>1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SC_SPRITE *load_sc_sprite(char *filename, int sc_sp_num, int frames, int alpha, SDL_Rect *rect)
{
	if (NULL != std_obj[sc_sp_num]) 	{	remove_sc_sprite(std_obj[sc_sp_num]); }
	SC_SPRITE *tmp;
	tmp 				= mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf	= NULL;
	suf 				= load_Surface(filename,alpha);
	if (NULL==suf)		{ return (NULL);			}
	tmp->img			= suf;
	if (frames==-1) 	{	tmp->frames = 1;		}
	else				{	tmp->frames = frames;	}
	tmp->aktframe		= 0;
	tmp->anim_speed 	= 0;
	tmp->anim_type		= 0;
	tmp->anim_time		= 0;
	tmp->anim_count 	= 0;
	tmp->alpha			= 255;
	tmp->w				= (suf->w/frames);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
	tmp->flags			= 0;
	tmp->move_wait		= 0;
	tmp->x				= rect->x;
	tmp->y				= rect->y;
	std_obj[sc_sp_num]	= tmp;
	return (tmp);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void load_local_sprite(char *filename, int sc_sp_num, int frames, int alpha, SDL_Rect *rect)
{
	if (NULL != std_obj[sc_sp_num]) 	{	remove_sc_sprite(std_obj[sc_sp_num]); }
	SC_SPRITE *tmp;
	tmp 				= mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf	= NULL;
	suf 				= load_local(filename,suf,alpha);
	if (NULL==suf)		{	return; }
	std_obj[sc_sp_num]	= tmp;
	tmp->img			= suf;
	if (frames==-1) 	{	tmp->frames=1;	}
	else				{	tmp->frames=frames; }
	tmp->aktframe		= 0;
	tmp->anim_speed 	= 0;
	tmp->anim_type		= 0;
	tmp->anim_time		= 0;
	tmp->anim_count 	= 0;
	tmp->alpha			= 255;
	tmp->w				= (suf->w/frames);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
	tmp->flags			= 0;
	tmp->move_wait		= 0;
	tmp->x				= rect->x-(rect->w*tmp->w);
	tmp->y				= rect->y-tmp->h;
}
//	tmp->cw 			= suf->w/frames/2;	//tachie_r
//	tmp->cw 			= suf->w/frames;	//tachie_l


/*---------------------------------------------------------

---------------------------------------------------------*/

static int inits;								/* 各スクリプトコマンドごとの初期化判定 */

/*---------------------------------------------------------
	SCP_MOVESP	スプライトの移動
---------------------------------------------------------*/

static int do_move_sc_sprite(char *l_or_r, int x, int y, int speed_aaa)
{
	SC_SPRITE *sc;
	int n=cha_search(l_or_r);
	if (n==-1)			{	return (-1);	}
//	else if (n==-2) 	{	sc = tachie_r;	}	/* right */
//	else if (n==-3) 	{	sc = tachie_l;	}	/* left */
	else				{	sc = std_obj[((n)&(SPRITE_MAX-1))]; }	/* 汎用スプライト */
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
	}
//
	/* 10で割るのは遅過ぎるので、3ビットシフトして(1/8)の値にする */
	int speed256;
	speed256 = (speed_aaa<<(8-3));		/* 精度確保用 */
//	sc->x += ((cos512(sc->angle512)*speed)/10);
//	sc->y += ((sin512(sc->angle512)*speed)/10);
	sc->x256 += ((cos512(sc->angle512)*speed256)>>8);	/* 精度確保用 */
	sc->y256 += ((sin512(sc->angle512)*speed256)>>8);	/* 精度確保用 */
//
	sc->x	= (t256_floor(sc->x256));		/* 精度確保用 */
	sc->y	= (t256_floor(sc->y256));		/* 精度確保用 */
	/* 大体の方向: なので ４５度回転してずらして、上位２ビットが大まかな方向 */
	switch ((((sc->angle512+64/*deg_360_to_512(45)*/) & 0x180 )))/*sc->r_course*/
	{	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
	case (0<<7)/* 1:→(512_0) */:	if (sc->x >= sc->target_x)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:↓(512_1) */:	if (sc->y <= sc->target_y)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:←(512_2) */:	if (sc->x <= sc->target_x)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:↑(512_3) */:	if (sc->y >= sc->target_y)	{	goto move_complete; 	}	break;
	}
	return (0);/*移動中*/
move_complete:
	/* 移動完了した場合は、正確な座標に修正する。 */
	sc->x = sc->target_x;
	sc->y = sc->target_y;
	return (1);/*移動完了*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static int script_wait(int n)
{
	static int w_tick;
	if (inits)
	{
		w_tick = n;
	}
	w_tick -= 1/*fps_fa ctor*/;
	if (w_tick < 0/*1?*/)
	{
		w_tick = 0;
		return (1);
	}
	return (0);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void msg_window_clear(void)
{
	SDL_FillRect(msg_window,NULL,SDL_MapRGB(msg_window->format,0,0,0));
}
static void msg_window_init(void)
{

}

/*---------------------------------------------------------

---------------------------------------------------------*/
#if (1==USE_2ND_SCREEN)
static void tachie_window_init(void)
{
}
#endif /* (1==USE_2ND_SCREEN) */
/*---------------------------------------------------------

---------------------------------------------------------*/

static void filter_init(int r,int g,int b)
{
	/*filter_init()*/
	SDL_FillRect(filter_window,NULL,SDL_MapRGB(filter_window->format,r,g,b));
	SDL_SetAlpha(filter_window, SDL_SRCALPHA, 0);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Rect msgw_rect;				/* メッセージウィンドウ表示位置 */
static void msgw_rect_init(int x,int y)
{
	msgw_rect.x = x;
	msgw_rect.y = y;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/ int draw_script_screen/*=0*/;					/* せりふウィンドウ表示フラグ */

static int window_effect(int w_is, int efe)
{
	/* 何故か効果がない原因不明 */
	if (-1 == efe)
	{
		draw_script_screen = w_is;
		return (1);
	}
//
	static int lines;
	static int st_line;
//	else
//	{
	//
	if (inits)
	{
		lines = 0;
		draw_script_screen = 2;
		msg_window_init();
		if (efe<1)
		{	st_line = 1;}
		else
		{	st_line = efe;}
	}
	Sint32 start_y;
	Sint32 end_y;
	if (w_is)
	{
		start_y 	= lines;
		end_y		= lines+st_line;
	}
	else
	{
		msg_window_init();/*???*/
		start_y 	= (0);
		end_y		= (56)/*std_window->h*/-(lines);
	}
	#if (000)
	if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window);	}
	if (SDL_MUSTLOCK(std_window))	{	SDL_LockSurface(std_window);	}
	{
		Sint32 y;
		for (y = start_y; y<end_y; y++)
		{
			Sint32 x;
			for (x = 0; x<(320)/*std_window->w*/; x++)
			{
				Uint32 col = getpixel16(std_window, x, y);
				putpixel16(msg_window, x, y, col);
			}
		}
	}
	if (SDL_MUSTLOCK(msg_window))	{	SDL_UnlockSurface(msg_window);	}
	if (SDL_MUSTLOCK(std_window))	{	SDL_UnlockSurface(std_window);	}
	#endif
	if (lines>(56)/*std_window->h*/)		{	msg_window_init();	draw_script_screen=w_is; return (1); }
	lines += st_line;
//	}
	return (0);
}

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
		if (NULL != l->data)	{ free (l->data);	l->data = NULL; }
		free (l);
		l = n;
	}
	sscript = (S_SCRIPT *)NULL;
	entry_script = (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_command(char *c, char *buffer, int *end)
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
		*end = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_str(char *c, char *buffer, int *end)
{
	int i = 0;
	int e = 0;		/* エスケープ */
	while ((((*c == ',') && (e==1))||(*c != ',')) && (*c != 13))
	{
		if ((*c == '\\') && (!e) && (*(c+1) == ','))	{	e = 1;	c++;	}
		if (e)			{	e=0;	}
		i++;
		if (i >= 200)	{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	if (*c == 13)
	{
		*end = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_int(char *c, int *nr, int *end)
{
	char buffer[32/*20*/];
	char *d = buffer;
	int i = 0;
	while (isdigit(*c)||*c=='-') /* ',' または '\n' が来たら終了 */
	{
		i++;
		if (i >= 20)
		{	return ((char *)NULL);}
		*d++ = *c++;
	}
	if (*c == 13)	{	/* 改行コードは OD OA */
		*end = 1;
	}
	*d = 0;
	if ((*c==',' || *c==13) && *(c-1)==',') {	*nr=-1; }
	else									{	*nr = atoi(buffer); }
	return (c);
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
//
	new_script->done	= 0;
	new_script->chain	= chain;
	new_script->next	= NULL;
	new_script->data	= NULL;

	switch (*command)
	{
	case 'B':
		if (0==tiny_strcmp(command, "BGTEXT"))	/* 背景にテキストを打ち込む */
		{
			SDL_Rect *new_rect0;
			new_script->command 	= SCP_BGTEXT;
			new_rect0				= mmalloc(sizeof(SDL_Rect));
			new_rect0->x			= new_script->para2;/* <- デフォルト */ 	//	/*c_p2*/c_pn[PARAM_02];
			new_rect0->y			= new_script->para3;/* <- デフォルト */ 	//	/*c_p3*/c_pn[PARAM_03];
			new_rect0->w			= /*c_p4*/c_pn[PARAM_04];
			new_rect0->h			= 1500;
			new_script->data		= new_rect0;
			new_script->para2		= /*c_p5*/c_pn[PARAM_05];		/* 書き込み速度 */
		}
		break;
	case 'C':
		if (0==tiny_strcmp(command, "CLCURSOR"))		{	new_script->command=SCP_CLCURSOR;	}	/* カーソルの初期化 */
		else if (0==tiny_strcmp(command, "CLICK"))		{	new_script->command=SCP_CLICK;		}	/* クリック待ち */
		break;
	case 'F':
		if (0==tiny_strcmp(command, "FILTER"))
		{
			new_script->command 	= SCP_FILTER;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */
		}
		break;
	case 'J':
		if (0==tiny_strcmp(command, "JUMP"))			{	new_script->command=SCP_JUMP;		}	/* pauseボタンを押した時のjump先。 */
	case 'L':
		if (0==tiny_strcmp(command, "LOADBG"))	/* 背景ファイルの変更 */
		{
			new_script->command=SCP_LOADBG;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */	/* フェイドイン／アウト */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */
		}
		else if (0==tiny_strcmp(command, "LOADCUR"))	{	new_script->command=SCP_LOADCUR;	}	/* 記憶したカーソル位置の呼び出し */
		else if (0==tiny_strcmp(command, "LOADL"))	/*	立ち絵L スプライトのロード */
		{
			SDL_Rect *new_rect3;
			new_script->command=SCP_LOAD_LR;
			new_rect3 = mmalloc(sizeof(SDL_Rect));
			if (   (-1==/*c_p1*/c_pn[PARAM_01])
				&& (-1==/*c_p2*/c_pn[PARAM_02]) )	/* 無指定の場合 */
			{
				new_rect3->x = 0;		/* x */
				new_rect3->y = 272; 	/* y */
			}
			else
			{
				new_rect3->x = /*c_p1*/c_pn[PARAM_01];		/* x */
				new_rect3->y = /*c_p2*/c_pn[PARAM_02];		/* y */
			}
			new_rect3->w		= 1;				/* w */
			new_rect3->h		= 0;				/* h */
			new_script->data	= new_rect3;
			new_script->para1	= SPRITE_tachie_l;		/* SPRITE_tachie_l */
			new_script->para2	= /*c_p3*/c_pn[PARAM_03];		/* flame */
			new_script->para3	= /*c_p4*/c_pn[PARAM_04];		/* alpha */
		}
		else if (0==tiny_strcmp(command, "LOADR"))	/*	立ち絵R スプライトのロード */
		{
			SDL_Rect *new_rect4;
			new_script->command=SCP_LOAD_LR;
			new_rect4 = mmalloc(sizeof(SDL_Rect));
			if ( (-1==/*c_p1*/c_pn[PARAM_01]) && (-1==/*c_p2*/c_pn[PARAM_02]) ) 	/* 無指定の場合 */
			{
				new_rect4->x = 380; 	/* x */ 	/*380 scenario_width*/
				new_rect4->y = 272; 	/* y */
			}
			else
			{
				new_rect4->x	= /*c_p1*/c_pn[PARAM_01];	/* x */
				new_rect4->y	= /*c_p2*/c_pn[PARAM_02];	/* y */
			}
			new_rect4->w		= 0;			/* w */
			new_rect4->h		= 0;			/* h */
			new_script->data	= new_rect4;
			new_script->para1	= SPRITE_tachie_r;	/* SPRITE_tachie_r */
			new_script->para2	= /*c_p3*/c_pn[PARAM_03];	/* flame */
			new_script->para3	= /*c_p4*/c_pn[PARAM_04];	/* alpha */
		}
		else if (0==tiny_strcmp(command, "LOADSP")) /* 汎用スプライトのロード */
		{
			SDL_Rect *new_rect1;
			new_script->command=SCP_LOADSP;
			new_rect1 = mmalloc(sizeof(SDL_Rect));
			new_rect1->x		= new_script->para2;/* <- デフォルト */ 	///*c_p2*/c_pn[PARAM_02];		/* x */
			new_rect1->y		= new_script->para3;/* <- デフォルト */ 	///*c_p3*/c_pn[PARAM_03];		/* y */
			new_rect1->w		= 0;			/* w */
			new_rect1->h		= 0;			/* h */
			new_script->data	=new_rect1;
			new_script->para2	= /*c_p4*/c_pn[PARAM_04];	/* flame */
			new_script->para3	= /*c_p5*/c_pn[PARAM_05];	/* alpha */
		}
		break;
	case 'M':
		if (0==tiny_strcmp(command, "MOVESP"))		/* スプライトの移動 */
		{
			new_script->command 	= SCP_MOVESP; /* para1=x */
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */		/* y */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */		/* speed */
			/* 異常値の場合、修正する */
			if (new_script->para3 < 1)
			{	new_script->para3 = 10;}
		}
		break;
	case 'P':
		if (0==tiny_strcmp(command, "PARAMSP"))
		{
			new_script->command 	= SCP_PARAMSP;					/* para1=α値 */
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */		/* anim_speed */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */		/* anim_type */
		}
		break;
	case 'R':
		if (0==tiny_strcmp(command, "RELOADSP"))		/* スプライトの再読み込み */
		{
			new_script->command 	= SCP_RELOADSP; 	/* para1=no */
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */		/* flames */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */		/* alpha */
		}
		break;
	case 'S':
			 if (0==tiny_strcmp(command, "SAVECUR"))	{	new_script->command = SCP_SAVECUR;	}	/* カーソル位置の記憶 */
		else if (0==tiny_strcmp(command, "SUBG"))		{	new_script->command = SCP_SUBG; 	}	/* 背景の表示/非表示 */
		else if (0==tiny_strcmp(command, "SUFILTER"))	{	new_script->command = SCP_SUFILTER; }	/* フィルター表示/非表示&透過度 */
		else if (0==tiny_strcmp(command, "SUL"))		{	new_script->command = SCP_SUL;		}	/* 立ち絵左の表示/非表示 */
		else if (0==tiny_strcmp(command, "SUR"))		{	new_script->command = SCP_SUR;		}	/* 立ち絵右の表示/非表示 */
		else if (0==tiny_strcmp(command, "SUSPRITE"))
		{
			new_script->command 	= SCP_SUSPRITE;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
		}/*スプライトの表示*/
		else if (0==tiny_strcmp(command, "SUTWINDOW"))
		{
			new_script->command 	= SCP_SUTWINDOW;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
		}	/* 強調立ち絵ウィンドウの表示/非表示 para2はスコアパネルのon/off */
		else if (0==tiny_strcmp(command, "SUWINDOW"))
		{
			new_script->command 	= SCP_SUWINDOW;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
		}	/* メッセージウィンドウの表示/非表示 para2はbpp速度。*/
		break;
	case 'T':
		if (0==tiny_strcmp(command, "TEXT"))				/* メッセージウィンドウへの書き込み */
		{
			SDL_Rect *new_rect2;
			new_script->command=SCP_TEXT;
			new_rect2 = mmalloc(sizeof(SDL_Rect));
			new_rect2->x			= 0/*10*/;
			new_rect2->y			= 0/*10*/;
			new_rect2->w			= 310;
			new_rect2->h			= 64/*1500*/;
			new_script->data		= new_rect2;
//			new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */		/* 書き込み速度 */
//			new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */		/* 書き込み後のcount_charの処遇 */
		}
		else if (0==tiny_strcmp(command, "TWINDOW"))	{	new_script->command=SCP_TWINDOW;		}/* 立ち絵強調ウィンドウの初期化 */
		break;
	case 'W':
		if (0==tiny_strcmp(command, "WAIT"))			{	new_script->command=SCP_WAIT;			}	/* wait */
		break;

	default:
		#if 0
		//ps pDebugScreenPrintf("unknown command :%s",command);
		//sc eKernelDelayThread(3000000);
		#endif
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
	if (!(fd = sceIoOpen((char *)file_name, PSP_O_RDONLY, 0777)))
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

	int entrys		= 0;		/* 命令がが書き込まれているかどうか。 */
	int line_num	= 0;		/* 行番号 */
	int chains		= 0;
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
		char *c = NULL; 				/* 走査中の行の分析用 */
		int end_arg=0;				/* 引数の取得の中止 */
		c = buffer_text_1_line; 	/* 先頭アドレスを渡すよ */

		line_num++;

		/* skiped lines. */
		if (*c=='\n')		{	continue;	}
		while (isspace(*c)) {	c++;		}
		if (*c=='#')		{	continue;	}
		if (*c=='-')		{	chains++;	c++;	}		/* 連続した命令 */
		else				{	chains=0;	}

		if (NULL==(c = load_command(c, char_command, &end_arg)))	{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
		if (!end_arg)
		{
			if (*c++ != ' ')									{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
			if (NULL==(c = load_str(c, c_p0, &end_arg)))		{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
		}
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				if (!end_arg)
				{
					if (*c++ != ',')									{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
					if (NULL==(c = load_int(c, &c_pn[kk], &end_arg)))	{	error(ERR_WARN, "syntax error in scriptfile '%s', line no: %d", filename, line_num); continue;	}
				}
			}
		}
	//	regist_script(char_command, c_p0, c_pn[0],c_pn[1],c_pn[2],c_pn[3],c_pn[4],c_pn[5],c_pn[6], chains);
		regist_script(char_command, c_p0, c_pn, chains);
		entrys++;
	}
	my_fclose (/*fp*/);
	//return (entrys);
	if (0 == entrys)
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
	for (i=0;i<SPRITE_MAX/*20*/;i++)
	{
		remove_sc_sprite(std_obj[i]);
	}
//	remove_sc_sprite(std_obj[SPRITE_tachie_r]);
//	remove_sc_sprite(std_obj[SPRITE_tachie_l]);
	if (NULL != bg_story_window)	{	SDL_FreeSurface(bg_story_window);	bg_story_window 	= NULL; }
//	if (NULL != msg_window) 		{	SDL_FreeSurface(msg_window);		msg_window			= NULL; }
#if (1==USE_2ND_SCREEN)
//	if (NULL != tachie_window)		{	SDL_FreeSurface(tachie_window); 	tachie_window		= NULL; }
#endif /* (1==USE_2ND_SCREEN) */
//	if (NULL != filter_window)		{	SDL_FreeSurface(filter_window); 	filter_window		= NULL; }
//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); font_color_bitmap	= NULL; }
	load_script_free();

//	count_char=0;			/* カーソル位置 */
//	cursor=0;
	cursor_x=0;
	cursor_y=0;
	cursor_x_chached=0; 	/* カーソル位置 保存用 */
	cursor_y_chached=0; 	/* カーソル位置 保存用 */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	スプライトオブジェクトの動作(動き/アニメーション)
---------------------------------------------------------*/

static /*SC_SPRITE **/void animate_work_my_sprite(SC_SPRITE *sp)
{
//	if (NULL == sp) 	{	return (NULL);	}/* 重複チェックなので不要 */
	if (sp->frames == 1)	{ return /*(sp)*/;	}/*  */
	/*
	/// anim_typeについて ///
	0:	左から右に。最後までいったら左へ戻る。(ループ)
	1:	右から左に。最後までいったら右へ戻る。(ループ)
	2:	左から右に。最後までいったらそのまま。(繰り返し無し)
	3:	右から左に。最後までいったらそのまま。(繰り返し無し)
	4:	左から右に。最後までいったら右から左に。(ループ)
	5:	右から左に。最後までいったら左から右に。(ループ)
	6:	アニメーションしない。(停止)
	*/
	sp->anim_count += 1/*fps_fa ctor*/;
	if (sp->anim_speed < sp->anim_count)
	{
		sp->anim_count=0;
		switch (sp->anim_type)
		{
		case 0: 	sp->aktframe++; 	if (sp->aktframe>=sp->frames)	{	sp->aktframe=0; 								}	break;
		case 1: 	sp->aktframe--; 	if (sp->aktframe<0) 			{	sp->aktframe=sp->frames-1;						}	break;
		case 2: 	sp->aktframe++; 	if (sp->aktframe>=sp->frames)	{	sp->aktframe=sp->frames-1;	sp->anim_type=6;	}	break;
		case 3: 	sp->aktframe--; 	if (sp->aktframe<0) 			{	sp->aktframe=0; 			sp->anim_type=6;	}	break;
		case 4: 	sp->aktframe++; 	if (sp->aktframe>=sp->frames)	{	sp->aktframe-=2;			sp->anim_type=5;	}	break;
		case 5: 	sp->aktframe--; 	if (sp->aktframe<0) 			{	sp->aktframe+=2;			sp->anim_type=4;	}	break;
		case 6: 	break;
		}
	}
//	return /*(sp)*/;
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
			if (std_obj[nnn]->flags)
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = std_obj[nnn]->w*std_obj[nnn]->aktframe;
				src_r.y = 0;
				src_r.w = std_obj[nnn]->w;
				src_r.h = std_obj[nnn]->h;
				dst_r.x = std_obj[nnn]->x;
				dst_r.y = std_obj[nnn]->y;
				#if (1==USE_2ND_SCREEN)
				if (std_obj[nnn]->flags==1)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r); }
				else if (std_obj[nnn]->flags==2)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, tachie_window, &dst_r);	}
				#else
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r); }
				#endif /* (1==USE_2ND_SCREEN) */
			}
			/*std_obj[nnn] =*/ animate_work_my_sprite(std_obj[nnn]);
		}
	}
}
#if (0)
static void tachie_draw_my_sprite(int nnn)
{
	{
		if (NULL != std_obj[nnn])
		{
			if (std_obj[nnn]->flags)
			{
				SDL_Rect src_r,dst_r;
				src_r.x = std_obj[nnn]->w*std_obj[nnn]->aktframe;
				src_r.y = 0;
				src_r.w = std_obj[nnn]->w;
				src_r.h = std_obj[nnn]->h;
				dst_r.x = std_obj[nnn]->x;
				dst_r.y = std_obj[nnn]->y;
				if (std_obj[nnn]->flags==1)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, sdl_screen[SDL_00_SCREEN], &dst_r); }
				#if (1==USE_2ND_SCREEN)
				else if (std_obj[nnn]->flags==2)
				{	SDL_BlitSurface(std_obj[nnn]->img, &src_r, tachie_window, &dst_r);	}
				#endif /* (1==USE_2ND_SCREEN) */
			}
			std_obj[nnn] = work_my_sprite(std_obj[nnn]);
		}
	}
}
#endif
/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* 背景α値用 */
static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window = load_local(filename, bg_story_window, alpha);
	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
}
static int load_bg_local(char *filename, int alpha, int fade_command, int set_alpha_speed)
{
	static int bg_alpha_speed;		/* 背景α値用 */
	switch (fade_command)
	{
	case -1:
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

---------------------------------------------------------*/

#define USE_KOMONO 0

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
	#if (1==USE_KOMONO)
	dr aw_my_sprite(0,9);/* 小物 10枚 */
	#endif /* (1==USE_KOMONO) */
	draw_my_sprite(SPRITE_tachie_l,SPRITE_tachie_l);/* 立ち絵 1枚 */
	#if (1==USE_KOMONO)
	dr aw_my_sprite(10,11);/* 小物 2枚 */
	#endif /* (1==USE_KOMONO) */
	draw_my_sprite(SPRITE_tachie_r,SPRITE_tachie_r);/* 立ち絵 1枚 */
	#if (1==USE_KOMONO)
	dr aw_my_sprite(12,15);/* 小物 4枚 */

	/* 7.  を描く */
	if (is_filter)
	{
		SDL_BlitSurface(filter_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
	}

	/* 8.  小物を描く */
	dr aw_my_sprite(16,16);/* 小物 1枚 */
	#endif /* (1==USE_KOMONO) */

	#if (1==USE_2ND_SCREEN)
	/* 9.  を描く(立ち絵強調) */
	if (is_tachie_window)
	{
		SDL_BlitSurface(tachie_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
	}
	#endif /* (1==USE_2ND_SCREEN) */

	#if (1==USE_KOMONO)
	/* 10.	小物を描く */
	dr aw_my_sprite(17,17);/* 小物 1枚 */

	/* 12.	小物を描く */
	dr aw_my_sprite(18,19);/* 小物 2枚 */
	#endif /* (1==USE_KOMONO) */
//


#if (000)

	/* 11.	せりふウィンドウを描く */
	if (draw_script_screen)
	{
		/* ウィンドウの半透明枠 を描く */
	//	if (draw_script_screen != 2)
		{
			#if (000)
			SDL_BlitSurface(std_window, NULL, sdl_screen[SDL_00_SCREEN], &msgw_rect);
			#endif /* (000) */
		}
		/*	*/
			SDL_BlitSurface(msg_window, NULL, sdl_screen[SDL_00_SCREEN], &msgw_rect);
	}
#endif /* (000) */
//
}


/*---------------------------------------------------------

---- やりたいこと ----
	1フレームでは終わらない処理があったときのためにこのコマンドからの命令には全て
	終わったことを知らせる引数を付けておくこと。=>doneに代入で終了。
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
			while ((NULL != ssc->next) && (SCP_JUMP != ssc->command))
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
			case SCP_LOADCUR:		/* カーソル位置、復元 */
			//	count_char=cursor;
				cursor_x = cursor_x_chached;
				cursor_y = cursor_y_chached;
				ssc->done = 1;
				break;
			case SCP_SAVECUR:		/* カーソル位置、記憶 */
			//	cursor=count_char;
				cursor_x_chached = cursor_x;
				cursor_y_chached = cursor_y;
				ssc->done = 1;
				break;
			case SCP_CLCURSOR:		/* カーソルの初期化 */
			//	count_char=0;
				cursor_x=0;
				cursor_y=0;
				ssc->done = 1;
				break;
			// [テキスト表示/クリック待ち]
			case SCP_CLICK:
				if (0==shot_ositenai)
				{	ssc->done = 1;	}
				break;
			case SCP_BGTEXT:
				if (0==shot_ositenai)	{	ssc->para2=0;	}	/* ショット押したら、残りは全部書く */
				ssc->done = print_kanji(bg_story_window, ssc->data, ssc->para0, ssc->para1, ssc->para2);
				break;
			case SCP_TEXT:
				if (0 == tmp_all[ssc->chain])
				{
					if (0==shot_ositenai)	{	ssc->para2=0;	}	/* ショット押したら、残りは全部書く */
					tmp_all[ssc->chain] = print_kanji(msg_window, ssc->data, ssc->para0, ssc->para1, ssc->para2);
				}
				else	/*if (1==tmp_all[ssc->chain])*/
				{
					if (0==(ssc->para3 & 0xf8))
					{
						if ( 0x00 == ( (ssc->para3 & shot_ositenai) & 0x01) )
						{
							if (0x00==(ssc->para3 & 0x02))
							{
								;//cursor_continue = 1; 	/* カーソル継続 */
							}
							else
							{
						//	//	count_char=0;
								cursor_x=0; 	/* カーソル初期化 */
								cursor_y=0; 	/* カーソル初期化 */
							}
							if (0x04==(ssc->para3 & 0x04))
							{
								msg_window_clear(); 	/* ウィンドウ初期化 */
								msg_window_init();		/* ウィンドウ初期化 */
							}
							ssc->done = 1;
							tmp_all[ssc->chain]=0;
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
			case SCP_FILTER:
				filter_init(ssc->para1,ssc->para2,ssc->para3);
				ssc->done = 1;
				break;
			case SCP_JUMP:
				ssc->done = 1;
				break;
			case SCP_LOADBG:
			//	bg_story_window = load_local(ssc->para0, bg_story_window, ssc->para1);
			//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
			//	ssc->done = 1;
				ssc->done=load_bg_local(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				break;
			case SCP_LOAD_LR:	/* 立ち絵L 立ち絵R */
				load_local_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->data);
				ssc->done = 1;
				break;
			case SCP_LOADSP:
				load_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->data);
				ssc->done = 1;
				break;
			case SCP_MOVESP:
				ssc->done=do_move_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				if (ssc->done == -1)
				{
					#if 0
					//ps pDebugScreenPrintf("no sprite in No.%s\n",ssc->para0);
					//sc eKernelDelayThread(2000000);
					#endif
					return_the_script=(1);
					return /*(1)*/;
				}
				break;
			case SCP_PARAMSP:
				tmp_all[ssc->chain] = cha_search(ssc->para0);
				if (tmp_all[ssc->chain] == -1)
				{
					return_the_script=(-1);
					return /*(-1)*/;
				}
				{
					SC_SPRITE *sc_tmp;
//					sc_tmp = NULL;
				//	else if (tmp_all[ssc->chain] == -2) {	sc_tmp=std_obj[SPRITE_tachie_r];	}
				//	else if (tmp_all[ssc->chain] == -3) {	sc_tmp=std_obj[SPRITE_tachie_l];	}
//					else
					{	sc_tmp=std_obj[((tmp_all[ssc->chain])&(SPRITE_MAX-1))]; }
					if (ssc->para1 != -1)
					{
						sc_tmp->alpha=ssc->para1;
						SDL_SetAlpha(sc_tmp->img, SDL_SRCALPHA, sc_tmp->alpha);
					}
					if (ssc->para2 != -1)
					{
						sc_tmp->anim_speed=ssc->para2;
					}
					if (ssc->para3 > 4) 		{	sc_tmp->anim_type = 0;				}
					else if (ssc->para3 == 3)	{	sc_tmp->anim_type = 3;	sc_tmp->aktframe = sc_tmp->frames-1;	}
					else if (ssc->para3 != -1)	{	sc_tmp->anim_type = ssc->para3; 	}
				}
					ssc->done = 1;
				break;
			case SCP_RELOADSP:
				reload_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				ssc->done = 1;
				break;
			case SCP_SUBG:
				is_bg = ssc->para1; 	ssc->done = 1;
				break;
			case SCP_SUFILTER:
				is_filter = ssc->para1;
				SDL_SetAlpha(filter_window, SDL_SRCALPHA, ssc->para2);
				ssc->done = 1;
				break;
			case SCP_SUL:
				std_obj[SPRITE_tachie_l]->flags = ssc->para1; ssc->done = 1;
				break;
			case SCP_SUR:
				std_obj[SPRITE_tachie_r]->flags = ssc->para1; ssc->done = 1;
				break;
			case SCP_SUSPRITE:
			//		 if (ssc->para1==-2)	{	std_obj[SPRITE_tachie_r]->flags = ssc->para2; }
			//	else if (ssc->para1==-3)	{	std_obj[SPRITE_tachie_l]->flags = ssc->para2; }
			/*	else	*/					{	std_obj[((ssc->para1)&(SPRITE_MAX-1))]->flags = ssc->para2; }
				ssc->done = 1;
				break;
			case SCP_SUTWINDOW:
				#if (1==USE_2ND_SCREEN)
				is_tachie_window=ssc->para1;
				#endif /* (1==USE_2ND_SCREEN) */
			//	is_pa nel_window=ssc->para2;
				{
					PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
					pd->state_flag &= (~(STATE_FLAG_09_IS_PANEL_WINDOW));
				//	pd->state_flag |= ((ssc->para2&1)<<STATE_FLAG_IS_PANEL_WINDOW_SHIFT);
					if (1 == ssc->para2)
					{
						pd->state_flag |= STATE_FLAG_09_IS_PANEL_WINDOW;
					}
				}
				ssc->done = 1;
				/*追加 is_pa nel_window 20090406 */
				break;
			case SCP_SUWINDOW:		/* うまくいってない */
				ssc->done=window_effect(ssc->para1,ssc->para2);
				break;

			case SCP_TWINDOW:
				#if (1==USE_2ND_SCREEN)
				tachie_window_init();
				#endif /* (1==USE_2ND_SCREEN) */
				ssc->done = 1;
				break;
			case SCP_WAIT:
				ssc->done = script_wait(ssc->para1);
				break;
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
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	/*ボス戦闘後イベント*/
#if 0
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	//if (pd->bo ssmode==B06_AFTER_EVENT)
	{
		work_the_script();
		if (1==return_the_script)
		{
			pd->state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));
			pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 	/*	pd->bo ssmode=B09_STAGE_LOAD;*/
		}
		/*draw_the_script();*/
	}	// [***090313	追加	ここに移動。
	/*ボス戦闘前イベント*/
	else
	//if (pd->bo ssmode==B03_BEFORE_EVENT)
#endif
	{
		work_the_script();
	//	if (1==return_the_script)	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=return_the_script)
		{
			draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
			pd->state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));
			pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 	/*pd->bo ssmode=B08_START;*/
		}
		/*else	{	draw_the_script();}*/
	}	// [***090313	追加
}

/*---------------------------------------------------------

---------------------------------------------------------*/

extern int select_player;
void script_load(void/*int mode*/)
{
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	char buffer1[16/*10*/];
	#if 0
	/*ボス戦闘後イベント*/
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	/*B07_AF TER_LOAD*/
	{
		sp rintf(buffer1,"stage%d-%d_end",player_now_stage,select_player);	/* sp rintf()は遅い */
	}
	/*ボス戦闘前イベント*/
	else
//	if (0==mode)/*B05_BE FORE_LOAD*/
	{
		set_music_volume(80);
		sp rintf(buffer1,"stage%d-%d",player_now_stage,select_player);	/* sp rintf()は遅い */
	}
	#else
	strcpy(buffer1,"stageZ-Z_end");
	buffer1[5] =('0'+player_now_stage);
	buffer1[7] =('0'+select_player);
	if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
	{;}
	else
	{	buffer1[8] = 0; 	set_music_volume(80);	}
	#endif
//
	if (0 == script_init(buffer1, NULL, 380))		// ファイルがない場合はイベントを飛ばす
			{	pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B09_STAGE_LOAD;*/	}
	else	{	pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;			/*pd->bo ssmode=B00_NONE;*/ /*B06_AFTER_EVENT*/ 	}
//
//	if (0 == script_init(buffer1, NULL, 380))
//			{	pd->state_flag |= STATE_FLAG_12_END_SCRIPT; 		/*pd->bo ssmode=B08_START;*/ }
//	else	{	pd->state_flag |= STATE_FLAG_06_IS_SCRIPT;			/*pd->bo ssmode=B00_NONE;*/ /*B03_BEFORE_EVENT*/	}
}
/*---------------------------------------------------------

---------------------------------------------------------*/

int script_init(char *filename, char *bg_name, int width)		/* シナリオファイル名と背景ファイル名 */
{
//	380/*scenario_width*/=width;
	if (NULL != bg_name)
	{
		load_bg_aaa(bg_name, 0/*alpha*/);
	//	bg_story_window = load_local(bg_name, bg_story_window, 0);
	//	SDL_SetColorKey(bg_story_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	if (0 == load_scenario(filename))
	{
		return (0);
	}
	/* ウィンドウの半透明枠 */
//	std_window			= loadbmp0("fonts/window.png", 1, 1);/*2*/
	msg_window_clear(); 	/* ウィンドウ初期化 */
	msg_window_init();
	msgw_rect_init(20,182);
	#if (1==USE_2ND_SCREEN)
	tachie_window_init();
	#endif /* (1==USE_2ND_SCREEN) */
	inits				= 1;
	is_bg				= 0;
	draw_script_screen			= 0;
	#if (1==USE_2ND_SCREEN)
	is_tachie_window	= 0;
	#endif /* (1==USE_2ND_SCREEN) */
	is_filter			= 0;
//	count_char			= 0;
//	cursor				= 0;
	cursor_x=0; 			/* カーソル初期化 */
	cursor_y=0; 			/* カーソル初期化 */
	cursor_x_chached=0; 	/* カーソル初期化 */
	cursor_y_chached=0; 	/* カーソル初期化 */

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

enum /*_story_state_*/
{
	STORY_INIT=0,
//	STORY_FADEOUT1,
//	STORY_LOAD,
//	STORY_FADEIN,
	STORY_WORKS,
//	STORY_FADEOUT2,
	STORY_QUIT
};
//	static SDL_Surface *fade_out_window;			/* 演出用 */
//	static int tick;								/* 時間測定用 */

void story_work(void)
{
	switch ((Uint8)(psp_loop&0xff)/*state.substate*/)
	{
	case STORY_INIT:
#if 0
//		//void story_init(void)
//		if (NULL==fade_out_window)
//		{
//			fade_out_window=loadbmp2("fonts/fade_black.png");
//		}
//		bg_alpha=0;
//		//keyboa rd_clear();
//		tick=0;
//		psp_loop++;//newsta te(ST_STORY,STORY_FADEOUT1,0);
//		break;
//	case STORY_FADEOUT1:
//		if (tick>30)
//		{
//			psp_loop++;//newsta te(ST_STORY,STORY_LOAD,0);
//			#if 1
//			/*	何か良くわからないけど、ここはok???。	*/
//			if (NULL!=fade_out_window)
//			{
//				unloadbmp_by_surface(fade_out_window);
//			}
//			#endif
//		}
//		else
//		{
//			SDL_BlitSurface(fade_out_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
//			tick++;
//		}
//		break;
//	case STORY_LOAD:
#endif
//		bg_alpha=0;
		script_init("story", /*NULL*/ NULL/*"story/arasuzi.jpg"*/ /*"story_bg.jpg"*/, 480);
//		bg_alpha=0;
		psp_loop++;//newsta te(ST_STORY,STORY_FADEIN,0);
		break;
//	case STORY_FADEIN:
//			psp_clear_screen();
//			SDL_SetAlpha(bg_story_window, SDL_SRCALPHA, bg_alpha);
//			SDL_BlitSurface(bg_story_window, NULL, sdl_screen[SDL_00_SCREEN], NULL);
//			if (bg_alpha<255)
//			{
//				bg_alpha += 5;
//			}
//			else
//			{
//				bg_alpha = 255;
//				#if 1
//				/*	何か良くわからないけど、ここはおかしい???。 */
//				if (NULL!=bg_story_window)
//				{
//					unloadbmp_by_surface(bg_story_window);
//				}
//				#endif
//				psp_loop++;//newsta te(ST_STORY,STORY_WORKS,0);
//			}
//			break;
	case STORY_WORKS:
		psp_clear_screen();
		work_the_script();
		/*draw_the_script();*/
		if (return_the_script)
		{
			psp_loop++;//newsta te(ST_STORY,STORY_QUIT,0);
		}
		break;
//	case STORY_FADEOUT2:
//		break;
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
		/* メインメニューに戻る */
		psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);//newsta te(ST_MENU/*ST_INTRO*/,0/*1*/,1);
		break;
	}
}


/*---------------------------------------------------------
	pspでは開放が正常動作出来ないので、起動時に確保して(終了時まで)開放しない
---------------------------------------------------------*/
extern UINT16 *msg_window_image;
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
	#if (1==USE_2ND_SCREEN)
	is_tachie_window	= 0;	/* 立ち絵強調表示フラグ */
	#endif /* (1==USE_2ND_SCREEN) */
	is_bg				= 0;	/* 背景表示フラグ */
	is_filter			= 0;	/* フィルター表示フラグ */
//
//	count_char			= 0;
//	cursor				= 0;
	cursor_x			= 0;	/* カーソル位置 */
	cursor_y			= 0;	/* カーソル位置 */
	cursor_x_chached	= 0;	/* カーソル位置 保存用 */
	cursor_y_chached	= 0;	/* カーソル位置 保存用 */
//
	#endif
	bg_alpha			= 255;	/* 255==初期値 */

	/* SDL_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */

	#if (1==USE_2ND_SCREEN)
	/* pspでは開放が正常動作出来ないので、起動時に確保して開放しない */
	/*tachie_window_init()*/
//	tachie_window	= NULL; 	/* 優先立ち絵ウィンドウ */
//	if (NULL != tachie_window)	{	SDL_FreeSurface(tachie_window); tachie_window = NULL; }
	tachie_window	= SDL_CreateRGBSurface(/*SDL_SRCCOLORKEY|*/SDL_SWSURFACE,/* メインメモリへ確保する */
			380/*scenario_width*/,
			272,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	SDL_SetColorKey(tachie_window,SDL_SRCCOLORKEY/*|SDL_RLEACCEL*/,0x00000000);
	#endif /* (1==USE_2ND_SCREEN) */
//
	/* pspでは開放が正常動作出来ないので、起動時に確保して開放しない */
	/*filter_init()*/
	#if 0
	if (NULL != filter_window)	{	SDL_FreeSurface(filter_window); filter_window = NULL; }
	#endif
	filter_window = SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* メインメモリへ確保する */
			380/*scenario_width*/,
			272,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	SDL_SetColorKey(filter_window, SDL_RLEACCEL, 0x00000000);
//
		#if 1
		/*???*/
	//	font_color_bitmap=loadbmp(buffer);
	//	esc_len=0;
		{
			char fn[128/*64*/];
		//	strcpy(fn, data_dir);	strcat(fn, "/fonts/" "font_bg16.png" );
			strcpy(fn, DIRECTRY_NAME_DATA "/fonts/" "font_bg16.png" );
			font_bg_bitmap = IMG_Load(fn);/*ここでロードすると確実に処置落ちするよ*/
		}
		font_bg_bitmap = SDL_DisplayFormat(font_bg_bitmap);
		#endif

//
	/* pspでは SDL_FreeSurface() が正常に動作しませんので、このままではメモリ不足？でハングアップします。 */
	/*msg_window_init()*/
//	if (NULL != msg_window) 	{	SDL_FreeSurface(msg_window);	msg_window = NULL;	}
	msg_window=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* メインメモリへ確保する */
			320/*340*/,
			56/*70*/ /*100*/,
			sdl_screen[SDL_00_SCREEN]->format->BitsPerPixel,
			sdl_screen[SDL_00_SCREEN]->format->Rmask,
			sdl_screen[SDL_00_SCREEN]->format->Gmask,
			sdl_screen[SDL_00_SCREEN]->format->Bmask,
			sdl_screen[SDL_00_SCREEN]->format->Amask);
	if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window); }
	msg_window_image = (UINT16 *)msg_window->pixels;
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
	#if (1==USE_2ND_SCREEN)
	/*tachie_window_init()*/
//	if (NULL != tachie_window)	{	SDL_FreeSurface(tachie_window); tachie_window = NULL; }
	#endif /* (1==USE_2ND_SCREEN) */
	/*filter_init()*/
//	if (NULL != filter_window)	{	SDL_FreeSurface(filter_window); filter_window = NULL; }

	//	if (NULL != font_color_bitmap)	{	SDL_FreeSurface(font_color_bitmap); 		font_color_bitmap	= NULL; }
//		if (NULL != font_color_bitmap)	{	unloadbmp_by_surface(font_color_bitmap);	font_color_bitmap	= NULL; }
}
