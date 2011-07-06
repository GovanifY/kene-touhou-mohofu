
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは漢字表示に関係がある場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _KANJI_SYSTEM_H_
#define _KANJI_SYSTEM_H_

/* 外部からメッセージウィンドウに漢字を描く。 */
extern int print_kanji000(const char *str, int color_type, int wait);

/*
	print_kanji000 の特殊機能(color_typeに指定)
	STR_CODE_NO_ENTER	改行しない
*/
#define STR_CODE_NO_ENTER (0x10)


/*---------------------------------------------------------

---------------------------------------------------------*/

/* メッセージウィンドウの内容を消す。 */
/* 画面消す */
extern void kanji_window_clear(void);
/* 指定行を1行消す */
extern void kanji_window_clear_line(unsigned int line_num);

/* カーソルをホームポジションへ移動 */
extern void home_cursor(void);

/* カーソルを指定位置へ移動 */
extern void set_cursor(unsigned int set_x, unsigned int set_y);

/* 外部からメッセージウィンドウに漢字を描画する場合のリセット */
//extern void sc ript_message_window_clear(void);


#endif /* _KANJI_SYSTEM_H_ */
