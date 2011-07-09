
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルはシナリオに関係がある場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _SCENARIO_SCRIPT_SYSTEM_H_
#define _SCENARIO_SCRIPT_SYSTEM_H_

//static extern int thescript(void);
//extern int script_init(char *filename, char *bg_name,int width);
//static extern int script_start(char *filename);

/* SDL_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
extern void story_script_system_init(void);/* 組み込み */
extern void script_system_exit(void);/* 外す */

extern void script_system_set_re_draw(void);	/* スクリプトシステムにSDL再描画指示。 */


#endif /* _SCENARIO_SCRIPT_SYSTEM_H_ */


///////////////////////////////////// 分離予定

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルはシナリオに関係がある場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _GAME_SCRIPT_SYSTEM_H_
#define _GAME_SCRIPT_SYSTEM_H_

extern void game_script_system_init(void);/* 組み込み */

#endif /* _GAME_SCRIPT_SYSTEM_H_ */


///////////////////////////////////// 分離予定
