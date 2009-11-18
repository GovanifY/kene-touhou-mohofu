#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include "game_main.h"

//static extern int thescript(void);
extern int script_init(char *filename, char *bg_name,int width);

/* SDL_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
extern void script_system_init(void);/* 組み込み */
extern void script_system_exit(void);/* 外す */



/* 外部からメッセージウィンドウに漢字を描画する場合のリセット */
extern void script_message_window_clear(void);

/* 外部からメッセージウィンドウに漢字を描く。 */
//static extern int print_kanji(SDL_Surface *drawmap, SDL_Rect *rect, const char *str, int color_type, int wait);
extern /*static*/ int print_kanji(/*SDL_Surface *drawmap,*/ SDL_Rect *rect, const char *str, int color_type, int wait);

#endif /* _SCENARIO_H_ */
