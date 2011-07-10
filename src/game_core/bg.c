
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	背景コントロール
--------------------------------------------------------- */

#define USE_BG_LOAD 0




static int request_bg0_y_scroll_speed256;	/* bg0のスクロール、予約速度 */


/*---------------------------------------------------------

--------------------------------------------------------- */


static int current_bg_alpha;
static int request_bg_alpha;

global void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}
/*---------------------------------------------------------

--------------------------------------------------------- */

//static void tile_work(void)
global void bg2_move_main(void)
{
	if (current_bg_alpha == request_bg_alpha) /* 最もありそうな可能性を排除 */
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= (4)/*6 */;
		}
		else
		{
			current_bg_alpha += (4)/*6 */;
			if (245 < current_bg_alpha) 	/* じわじわするので */
			{	current_bg_alpha = (255);	}
		}
		/* converted */
		u8 aaa = (current_bg_alpha/* >>1 */);
	//	cg.conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
		cg.conv_bg_alpha = ((aaa&0xf0));
	}

//	if (current_bg_alpha < 250 )/* 255 */
//			{	current_bg_alpha += 6;		}
//	else	{	current_bg_alpha = 255; 	}
//
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


//
	#if 0/* laster_spritデバッグ用 */
	/* パネルのスコア欄にlaster_spritを グレイズ欄に追加bg枚数を 表示させる。っていうか書き換えちゃう。 */
	pd_score		= (t256_floor(laster_sprit256));
	pd_graze_point	= number_of_bg;
	#endif
}



/*---------------------------------------------------------
	ステージ読み込み開始時に、毎回初期化する
--------------------------------------------------------- */
global void bg2_start_stage(void)
{
	request_bg0_y_scroll_speed256 = cg.current_bg0_y_scroll_speed256 = t256(0.5);/* 初期値 */
}

/*---------------------------------------------------------
	BGコントロールコマンド(テスト中)
--------------------------------------------------------- */

global void game_command_02_bg_control(GAME_COMMAND *l)
{
	request_bg0_y_scroll_speed256 = (l->user_y);	/* bg0のスクロール、予約速度を設定 */

}
//		{
//			/* KETMは約20-30fps動作ですが、現在約60fps動作なのであまりに速すぎる為、速度値を半分に修正しました。 */
//			#if 1
//			{
//		//	new_entry->scroll_speed256/* ctype */ = speed256_tbl[((new_entry->user_255_code) & 0x0f)];
//			}
//			#endif
//		}
//	enum
//	{
//		BG2_00_ERROR = 0,
//		BG2_01_SET_SCROOL_OFFSET,
//		BG2_02_BG_STOP,
//	//	BG2_03_BG_SWAP,
//		BG2_03_DESTOROY_CLOUDS,
//	};
//	short xxx;
//	short yyy;
//	short speed256;
//	xxx = l->user_x;
//	yyy = l->user_y;
//	speed256 = l->scroll_speed256;
	//

//	sw itch (xxx)
//	{
//	ca se BG2_01_SET_SCROOL_OFFSET: /* スクロール値を直接セット */
//		#if (1==USE_BG_LOAD)
//		bg0_bmp_y256 = yyy;
//		#endif /* (1==USE_BG_LOAD) */
//		break;
//	ca se BG2_02_BG_STOP:	/* テスト中 */
//		#if (1==USE_BG_LOAD)
//		laster_sprit256 = t256(272);
//		sprit_flag = 0; 	/* 分割位置を判断する必要がない */
//		#endif /* (1==USE_BG_LOAD) */
//		break;
//	ca se BG2_03_BG_SWAP:	/* テスト中 */
//		bg0_bmp_y256 = 0;
//		which_bg++;
//		which_bg &= 1;
//		bg2_swap();
//		break;
//	ca se BG2_03_DESTOROY_CLOUDS:	/* テスト中 */
//		clouds_destroy();
//		break;
//	}

/*---------------------------------------------------------
	psp起動時に一度だけ初期化する
--------------------------------------------------------- */
//extern void clouds_system_init(void);
//global void bg2_system_init(void)
//{
//	/* 画像読み込み用サーフェイス */
//	clouds_system_init();
//}
