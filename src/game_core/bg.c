
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	背景コントロール
--------------------------------------------------------- */

#define USE_BG_LOAD 0

static int request_bg0_y_scroll_speed256;	/* bg0のスクロール速度、設定値 */

/*---------------------------------------------------------

--------------------------------------------------------- */

static s32 current_bg_alpha;	/* bg0の明るさ、現在値 */
static s32 request_bg_alpha;	/* bg0の明るさ、設定値 */

global void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}


/*---------------------------------------------------------
	ゲーム中、背景の移動処理
	(移動処理のみで描画処理はしない)
--------------------------------------------------------- */

global void bg_move_all(void)
{
	/*(背景の明るさ変更処理)*/
	if (current_bg_alpha == request_bg_alpha) /* 最もありそうな可能性を排除 */
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= (4);/*(6)*/
		}
		else
		{
			current_bg_alpha += (4);/*(6)*/
			current_bg_alpha = psp_min(current_bg_alpha, (255) );	/* じわじわするので */
		//	if (245 < current_bg_alpha) 	{	current_bg_alpha = (255);	}	/* じわじわするので */
		}
		/* converted */
		u8 aaa = (current_bg_alpha/* >>1 */);
	//	cg.conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
		cg.conv_bg_alpha = ((aaa&0xf0));
	}
	//
	/*(背景のスクロール速度変更処理)*/
	if (cg.current_bg0_y_scroll_speed256 == request_bg0_y_scroll_speed256) /* 最もありそうな可能性を排除 */
	{
		;
	}
	else
	if (cg.current_bg0_y_scroll_speed256 > request_bg0_y_scroll_speed256)
	{
		cg.current_bg0_y_scroll_speed256--;
	}
	else
	{
		cg.current_bg0_y_scroll_speed256++;
	}
	//
	#if 0/* laster_spritデバッグ用 */
	/* パネルのスコア欄にlaster_spritを グレイズ欄に追加bg枚数を 表示させる。っていうか書き換えちゃう。 */
	pd_score		= (t256_floor(laster_sprit256));
	pd_graze_point	= number_of_bg;
	#endif
}

/*---------------------------------------------------------
	BGコントロールコマンド(テスト中)
--------------------------------------------------------- */

global void game_command_03_bg_control(GAME_COMMAND *l)
{
	request_bg0_y_scroll_speed256 = (l->user_speed256); 	/* bg0のスクロール、予約速度を設定 */
}
