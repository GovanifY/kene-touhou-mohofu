
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルはシナリオに関係がある場合のみ、インクルードします。
	"game_main.h"より、後でインクルードします。
---------------------------------------------------------*/

#ifndef _KAIWA_SYSTEM_H_
#define _KAIWA_SYSTEM_H_

extern void kaiwa_system_init(void);/* 組み込み */
extern void kaiwa_system_exit(void);/* 外す */

extern void kaiwa_system_set_re_draw(void); 	/* 会話システムにSDL再描画指示。 */


///////////////////////////////////// 分離予定(?)

//未定 extern void ending_system_init(void);/* 組み込み */

#endif /* _KAIWA_SYSTEM_H_ */
