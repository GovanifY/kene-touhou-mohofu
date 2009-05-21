#ifndef _SCENARIO_H_
#define _SCENARIO_H_

#include "support.h"

//static extern int thescript(void);
extern int script_init(char *filename, char *bg_name,int width);

/* SDL_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
extern void script_system_init(void);/* 組み込み */
extern void script_system_exit(void);/* 外す */

/* 今の所必要ないけど、外部から描くなら。 */
//static extern int print_kanji(SDL_Surface *drawmap, SDL_Rect *rect, const char *str, int color_type, int wait);

#endif /* _SCENARIO_H_ */
