
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	背景コントロール
---------------------------------------------------------*/

#define USE_BG_LOAD 0



global int current_bg0_y_scroll_speed256;	/* bg0のスクロール、現在速度 */
static int request_bg0_y_scroll_speed256;	/* bg0のスクロール、予約速度 */


/*---------------------------------------------------------

---------------------------------------------------------*/

extern unsigned int conv_bg_alpha;

static int current_bg_alpha;
static int request_bg_alpha;

global void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void tile_work(void)
{
	if (current_bg_alpha == request_bg_alpha) /*最もありそうな可能性を排除*/
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= (4)/*6*/;
		}
		else
		{
			current_bg_alpha += (4)/*6*/;
			if (245 < current_bg_alpha) 	/* じわじわするので */
			{	current_bg_alpha = (255);	}
		}
		/* converted  */
		u8 aaa = (current_bg_alpha/*>>1*/);
	//	conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
		conv_bg_alpha = ((aaa&0xf0));
	}

//	if (current_bg_alpha < 250/*255*/ )
//			{	current_bg_alpha += 6;		}
//	else	{	current_bg_alpha = 255; 	}
//
	if (current_bg0_y_scroll_speed256 == request_bg0_y_scroll_speed256) /*最もありそうな可能性を排除*/
	{
		;
	}
	else
	if (current_bg0_y_scroll_speed256 > request_bg0_y_scroll_speed256)
	{
		current_bg0_y_scroll_speed256--;
	}
	else
	{
		current_bg0_y_scroll_speed256++;
	}
//


//
	#if 0/*laster_spritデバッグ用*/
	/* パネルのスコア欄にlaster_spritを グレイズ欄に追加bg枚数を 表示させる。っていうか書き換えちゃう。 */
	pd_score		= (t256_floor(laster_sprit256));
	pd_graze_point	= number_of_bg;
	#endif
}


/*---------------------------------------------------------

---------------------------------------------------------*/

global void bg2_move_main(void)
{
	{
		tile_work();
	//	#if (1 != US E_GU)
		#if 0000
		tile_draw(draw_bmp/*bg0_bmp*/);
		#else
		/* (1 == US E_GU) */
//		psp_clear_screen();
		#endif
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void clouds_destroy(void);
global void bg2_destroy(void)
{
	clouds_destroy();
}


//extern int tile_bg2_add(void/*int lev*/);
/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
global void add_enemy_load_bg(STAGE_DATA *l)
{


}

/*---------------------------------------------------------
	ステージ読み込み開始時に、毎回初期化する
---------------------------------------------------------*/
global void bg2_start_stage(void)
{
	bg2_destroy();
//	if (1==use_clouds)	{}
	request_bg0_y_scroll_speed256 = current_bg0_y_scroll_speed256 = t256(0.5);/*初期値*/
}

/*---------------------------------------------------------
	psp起動時に一度だけ初期化する
---------------------------------------------------------*/
extern void clouds_system_init(void);
global void bg2_system_init(void)
{
	/* 画像読み込み用サーフェイス */
	clouds_system_init();
}

/*---------------------------------------------------------
	BGコントロールコマンド(テスト中)
---------------------------------------------------------*/
enum
{
	BG2_00_ERROR = 0,
	BG2_01_SET_SCROOL_OFFSET,
	BG2_02_BG_STOP,
//	BG2_03_BG_SWAP,
	BG2_03_DESTOROY_CLOUDS,
};
global void bg2_control(STAGE_DATA *l)
{
	short xxx;
	short yyy;
//	short speed256;
	xxx = l->user_x;
	yyy = l->user_y;
//	speed256 = l->scroll_speed256;
	//
		{
			/* KETMは約20-30fps動作ですが、現在約60fps動作なのであまりに速すぎる為、速度値を半分に修正しました。 */
			#if 1
			{const unsigned short speed256_tbl[16] =
			{
				#if 0
				/*ketm09互換*/
				t256(0.5), t256(0.1), t256(0.2), t256(0.3),/* - - - - */
				t256(0.4), t256(0.5), t256(0.6), t256(0.7),/* - 5 6 7 */
				t256(0.8), t256(0.9), t256(0.5), t256(0.5),/* 8 9 - - */
				t256(0.5), t256(0.5), t256(0.5), t256(0.5) /* - - - - */
				#else
				/*拡張*/
				t256(0.0), t256(0.1), t256(0.2), t256(0.3),/* 0 1 2 3 */	/* 00 01 02 03 */
				t256(0.4), t256(0.5), t256(0.6), t256(0.7),/* 4 5 6 7 */	/* 04 05 06 07 */
				t256(0.8), t256(0.9), t256(1.0), t256(1.5),/* 8 9 J K */	/* 08 09 10 11 */
				t256(2.0), t256(2.5), t256(3.0), t256(4.0) /* L M N O */	/* 12 13 14 15 */
				#endif
			};
		//	new_entry->scroll_speed256/*ctype*/ = speed256_tbl[((new_entry->user_255_code) & 0x0f)];
			request_bg0_y_scroll_speed256 = speed256_tbl[((l->user_255_code) & 0x0f)];	/* bg0のスクロール、予約速度を設定 */
			}
			#endif
		}
	switch (xxx)
	{
	case BG2_01_SET_SCROOL_OFFSET:	/* スクロール値を直接セット */
		#if (1==USE_BG_LOAD)
		bg0_bmp_y256 = yyy;
		#endif /* (1==USE_BG_LOAD) */
		break;
	case BG2_02_BG_STOP:	/* テスト中 */
		#if (1==USE_BG_LOAD)
		laster_sprit256 = t256(272);
		sprit_flag = 0; 	/* 分割位置を判断する必要がない */
		#endif /* (1==USE_BG_LOAD) */
		break;
//	case BG2_03_BG_SWAP:	/* テスト中 */
//		bg0_bmp_y256 = 0;
//		which_bg++;
//		which_bg &= 1;
//		bg2_swap();
//		break;
	case BG2_03_DESTOROY_CLOUDS:	/* テスト中 */
		clouds_destroy();
		break;
	}
}
