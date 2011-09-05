
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは漢字表示に関係がある場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _KANJI_SYSTEM_H_
#define _KANJI_SYSTEM_H_

/* 外部からメッセージウィンドウに漢字を描く。 */

extern void kanji_color(unsigned int set_color_type);
extern void kanji_wait(unsigned int set_wait);

/* 外部からメッセージウィンドウに漢字を描く。 */
extern int kanji_draw(void);

/*(メッセージウィンドウに r35font.png を使って描く。)*/
extern void kanji_ascii_font16_draw(void);

/*
	kanji_draw の特殊機能(color_typeに指定)
	STR_CODE_NO_ENTER	改行しない
*/
#define STR_CODE_NO_ENTER (0x10)
//
#define KANJI_FONT_08_HARF_WIDTH	(8) 							/* 半角漢字の幅 */
#define KANJI_FONT_16_WIDTH 		(KANJI_FONT_08_HARF_WIDTH*2)	/* 全角漢字の幅 */
#define KANJI_FONT_16_HEIGHT_P0 	(16)							/* 全角漢字の書体の高さ */
#define KANJI_FONT_18_HEIGHT_P2 	(KANJI_FONT_16_HEIGHT_P0+2) 	/* 全角漢字の行の高さ */


/*---------------------------------------------------------

---------------------------------------------------------*/

/* メッセージウィンドウの内容を消す。 */
/* 漢字画面を全行消す。 */
extern void kanji_window_clear(void);
/* 指定行を1行消す。 */
extern void kanji_window_clear_line(unsigned int line_num);

/* 漢字カーソルをホームポジションへ移動 */
extern void kanji_cursor_move_home_position(void);

/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動 */
extern void kanji_window_all_clear(void);

/* カーソルを指定位置へ移動 */
/*
	x座標の単位は[pixel]
	y座標の単位は[pixel]
	y座標を KANJI_FONT_18_HEIGHT_P2 の倍数で指定しない場合、変な表示になる。
	暫定仕様なので、変わる可能性大。
*/
// 仮想バッファのレンダリング位置を指定。
extern void set_kanji_xy(unsigned int set_x_offset_pixel, unsigned int set_y_offset_pixel);
// 画面表示の基準位置仮想を指定。
extern void set_kanji_origin_xy(unsigned int set_x_offset_pixel, unsigned int set_y_offset_pixel);
extern void set_kanji_origin_kankaku(unsigned int set_y_offset_pixel);
/* 外部からメッセージウィンドウに漢字を描画する場合のリセット */
//extern void sc ript_message_window_clear(void);

#if 1
/*---------------------------------------------------------
	独立ライン関連
---------------------------------------------------------*/
enum
{
	ML_OFF	= 0x00000000,	/* 非表示状態を明示する。 */
	ML_ON	= 0x00ffffff,	/*	 表示状態を明示する。 */
};
enum
{					// 日本語でコメント書いてると混乱するので、定数をコメントに合わせる。
	ML_LINE_01 = 0, // 0 == 1行目(0行目は無いので注意)
	ML_LINE_02, 	// 1 == 2行目
	ML_LINE_03, 	// 2 == 3行目
	ML_LINE_04, 	// 3 == 4行目
	ML_LINE_05, 	// 4 == 5行目
	ML_LINE_06, 	// 5 == 6行目
	ML_LINE_07, 	// 6 == 7行目
	ML_LINE_08, 	// 7 == 8行目
	ML_LINE_99_MAX/*(2^nの最大行)*/
};

enum
{
	ML_HAIKEI_m1_OFF		= -1,
	ML_HAIKEI_00_MISIYOU	= 0,	/*[緑/未使用]*/
	ML_HAIKEI_01_BOSS_SPELL_CARD,	/*[青/ボスカード用背景]*/
	ML_HAIKEI_02_JIKI_SPELL_CARD,	/*[赤/自機カード用背景]*/
	ML_HAIKEI_03_MESSAGE,			/*[黒/会話用背景]*/
	ML_HAIKEI_04_MAX/*(最大)*/
};

typedef struct
{
	int timer;		// 0で非表示。1フレーム表示する毎にデクリメント。(自動で値を1減らす)。
	int haikei; 	// 文字の背景の種類。
	int x;			/* x位置[pixel] */
	int y;			/* y位置[pixel] */
} ML_FONT;
//	char *str;		/* 文字列(実体へのポインタのみ保持) */
//	void *render;	/* レンダリング位置 */

extern ML_FONT ml_font[ML_LINE_99_MAX];

//	kanji_system_hide_message(ML_LINE_02);/*(2行目以下を非表示にする。)*/
extern void set_kanji_hide_line(unsigned int line_num);
#endif

/*---------------------------------------------------------
	スクリプト等用、標準初期化
---------------------------------------------------------*/

extern void kanji_init_standard(void);/*(漢字関連の標準初期化)*/

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void dec_print_format(unsigned int num, int size, char *my_str);

#endif /* _KANJI_SYSTEM_H_ */
