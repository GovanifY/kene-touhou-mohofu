
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
	背景スペカ

---------------------------------------------------------*/

/* スペカ時でない通常時のプログラムを指示 */
extern void *callback_gu_draw_haikei_modosu;	/* (スペカ時から通常時へ)戻す用。(==通常時) */

/* スペカ時のプログラムを指示 */
extern void *callback_gu_draw_haikei_supeka;	/* スペカ時用 */

/* 現在使用するプログラムを設定 */
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;	/* 背景ウィンドウ表示フラグ */

/* ゲーム中、 */
//extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */
//extern void *callback_gu_draw_haikei_supeka;
//extern void *callback_gu_draw_haikei_modosu;


/* スペカ時に背景のプログラム種類を設定 */


#endif /* _PSP_GU_RENDER_H_ */
