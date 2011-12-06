
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _PSP_GU_RENDER_H_
#define _PSP_GU_RENDER_H_

enum
{
	GU_HAIKEI_00_2D_256 = 0,	/* 256x256テクスチャを敷き詰める2D。 */
	GU_HAIKEI_01_3D_TEST01, 	/* 3Dのテスト */
	GU_HAIKEI_02_EIENTEI,		/* "永遠亭"の予定。 */
	GU_HAIKEI_03_FAKE_3D,		/* テクスチャ変形による擬似3D 将来的に無くなる予定。 */
	GU_HAIKEI_99_MAX,
};

/*---------------------------------------------------------
	背景ヘルパー
---------------------------------------------------------*/

/* 背景を消す色を 32bit colorで指定する。 */
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);


/*---------------------------------------------------------
	背景の描画プログラムを登録する。
	背景カード
---------------------------------------------------------*/

/* カード時に背景のプログラム種類を設定 */

/*---------------------------------------------------------

---------------------------------------------------------*/

//extern void psp_push_screen(void);/* 仮想スクリーンを退避 */
//extern void psp_pop_screen(void);/* 仮想スクリーンを復活 */
#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
#define psp_pop_screen(aaa)  psp_move_screen( SDL_01_BACK_SCREEN, SDL_00_VIEW_SCREEN )
extern void psp_move_screen(int src_screen_number, int dst_screen_number);


/*---------------------------------------------------------
	メニューシステム
---------------------------------------------------------*/

extern void psp_pause_filter(void);/* 仮想スクリーンにもやもやエフェクトをかける */
extern void psp_clear_screen(void);/* 仮想スクリーンを黒で消す */


#endif /* _PSP_GU_RENDER_H_ */
