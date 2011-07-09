
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	コンティニュー？メニュー
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i0_256;	// 表示用 x オフセット */
	int MENU_DATA_i1_256;	// 表示用 y オフセット */
	int MENU_DATA_i2;		// スプライト用(メニュー選択時、横 x の揺れ幅) */
	int MENU_DATA_i3;		// スプライト用(揺れ幅が徐々に戻る用) */
} MY_OBJ;

enum
{
	MENU_ITEM_00_CONTINUE_YES	= 0,
	MENU_ITEM_01_CONTINUE_NO,
	MENU_ITEM_99_MAX	/* オブジェクトの数 */
};

static MY_OBJ			menu_item_my_obj[MENU_ITEM_99_MAX]; 	/* オブジェクト */
static SDL_Surface		*menu_item_surface[MENU_ITEM_99_MAX];	/* 文字の画像 */
static int				active_item;	/* 現在メニュー上で選択されている項目番号 */
//
static int				menu_brite; 	/* メニューの明るさ(α値) */


#define M1_NOT_TIME_OUT 		(-1)/* 時間切れなしに設定(時間切れなしモード) */
//#define M2_STATE_TIME_OVER	(-2)/* 時間切れありモードで時間切れになった状態 */


/*---------------------------------------------------------

---------------------------------------------------------*/

//#if 0
//	#define FPS_MENU_FACTOR 	(fps_factor)
//	#define FPS_MENU_FACTOR10	(10/fps_factor)/*←どうも不安定(Division by Zero ???)*/
//	#define FPS_MENU_FACTOR8	(8*fps_factor)
//	#define FPS_MENU_FACTOR15	(15*fps_factor)
//#else
	#define FPS_MENU_FACTOR 	(1)
	#define FPS_MENU_FACTOR10	(16/*64*/)/* 10/FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR15	(3/*2*/)
//#endif


/*---------------------------------------------------------

---------------------------------------------------------*/

//	int res_num,//		RES08_ASK_CONTINUE_MENU,
//	MENU *m,//		&ask_continue_menu,

//static u32 pause_start_time = 0;
extern void render_continue(void);


/*---------------------------------------------------------

---------------------------------------------------------*/
static void ask_continue_menu_work_draw(void)
{
	{
		static int angle1024/*=0*/;
		angle1024 += (FPS_MENU_FACTOR15+FPS_MENU_FACTOR15);
		mask1024(angle1024);
		int i;
		for (i=0; i<MENU_ITEM_99_MAX; i++)
		{
			if (i==active_item) /* 選択された */
			{
				menu_item_my_obj[i].MENU_DATA_i2 = 12;	/* ゆれ幅を 12 にする */
			}
			else	/* 選択されてない */
			{
				if ((menu_item_my_obj[i].MENU_DATA_i2) && (0==menu_item_my_obj[i].MENU_DATA_i3))
				{
					menu_item_my_obj[i].MENU_DATA_i2--;
					menu_item_my_obj[i].MENU_DATA_i3 = 3;
				}
			}
			if (menu_item_my_obj[i].MENU_DATA_i3)
			{	menu_item_my_obj[i].MENU_DATA_i3--;}
			menu_item_my_obj[i].x256 = (menu_item_my_obj[i].MENU_DATA_i0_256)+(cos1024((angle1024))*menu_item_my_obj[i].MENU_DATA_i2);
		//	menu_item_my_obj[i].y256 = (menu_item_my_obj[i].MENU_DATA_i1_256)+(sin1024((angle1024))*menu_item_my_obj[i].MENU_DATA_i2);
			menu_item_my_obj[i].y256 = (menu_item_my_obj[i].MENU_DATA_i1_256);
		//	{
		//		menu_item_my_obj[i].menu_brite	= menu_brite;
		//	}
		}
	}
//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//	pause_sp rite_display();/*SP_GROUP_TEXTS*/
	/* 描画 */
	psp_pop_screen();
	{			/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
		int i;
		for (i=0; i<MENU_ITEM_99_MAX; i++)
		{
			rect_locate_offset.x = ((menu_item_my_obj[i].x256)>>8);
			rect_locate_offset.y = ((menu_item_my_obj[i].y256)>>8);
		//	SDL_SetAlpha(	menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			SDL_SetAlpha(	menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(menu_brite));
			SDL_BlitSurface(menu_item_surface[i], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
		}
	}
}


/*---------------------------------------------------------
	fadout fininshed, menu done
---------------------------------------------------------*/

//extern void adjust_start_time(u32 pause_time);
extern void player_continue_value(void);
static void ask_continue_menu_03_FININSH(void)
{
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
	psp_pop_screen();
//	{	int i;
//		for (i=0; i<MENU_ITEM_99_MAX; i++)
//		{
//			//SDL_FreeSurface(menu_item_surface[i]);
//		}
//	}
	if (MENU_ITEM_00_CONTINUE_YES == active_item)
	{ /* Continue Game */
		player_continue_value();
		#if 1/*Gu化完了したら要らなくなる*/
		{
			psp_clear_screen(); /* [PAUSE] 復帰時にSDL画面を消す。 */
		}
		#endif
		main_call_func = shooting_game_core_work;
		//adjust_start_time(pause_start_time);
	}
	else
	{ /* Game over /Quit Game */
	//	gamecore_term();
	//	main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
		main_call_func = gameover_start;
	}
}


/*---------------------------------------------------------
	selection done, decreasing menu_brite, fade menu out
---------------------------------------------------------*/

static void ask_continue_menu_02_FADE_OUT(void)
{
	menu_brite -= FPS_MENU_FACTOR8;
	if (0 >= (menu_brite) )
	{
		menu_brite	= 0;
		main_call_func = ask_continue_menu_03_FININSH;
	}
	ask_continue_menu_work_draw();
}


/*---------------------------------------------------------
	fadein complete
---------------------------------------------------------*/

static void ask_continue_menu_01_WORK_MENU(void)
{
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (psp_pad.pad_data & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
		{
			voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
		}
		if (psp_pad.pad_data & PSP_KEY_DOWN)
		{
			if (active_item == MENU_ITEM_99_MAX-1)
			{	active_item = 0;	}
			else
			{
				active_item++;
			}
		}
		else if (psp_pad.pad_data & PSP_KEY_UP)
		{
			if (0 == active_item)
			{	active_item = MENU_ITEM_99_MAX-1;	}
			else
			{
				active_item--;
			}
		}
		/* セレクトキーを押した場合、必ずコンティニューしない。 */
		if (psp_pad.pad_data & PSP_KEY_SELECT)					/* [select]ボタンで必ずコンティニューしない */
		{
			/*pause_menu.*/active_item	= MENU_ITEM_01_CONTINUE_NO;
			main_call_func = ask_continue_menu_02_FADE_OUT;
		}
		/* ポーズキーを押した場合、必ずコンティニュー決定。 */
		if (psp_pad.pad_data & PSP_KEY_PAUSE) 				/* [start]ボタンで必ずコンティニュー決定 */
		{
			/*pause_menu.*/active_item	= MENU_ITEM_00_CONTINUE_YES;
			main_call_func = ask_continue_menu_02_FADE_OUT;
		}
		if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
		{
			voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
			main_call_func = ask_continue_menu_02_FADE_OUT;/* メニュー消去準備 */
		}
	}
	ask_continue_menu_work_draw();
}


/*---------------------------------------------------------
	Increasing menu_brite, fade menu in
---------------------------------------------------------*/

static void ask_continue_menu_00_FADE_IN_MENU(void)
{
	menu_brite += FPS_MENU_FACTOR8;
	if (255 <= (menu_brite) )
	{
		menu_brite = 255;
		main_call_func = ask_continue_menu_01_WORK_MENU;
	}
	ask_continue_menu_work_draw();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;


/*static*/global void ask_continue_menu_start(void)
{
	render_continue();
	#if 1
	/* てすと、構造的問題 */
	//if (RES09_PAUSE_MENU==res_num)
//	// int set_fade_out_flag,
//	if (0!=/*pause_filter_flag*/(1)/*1 構造的問題*/)
	{
		/* (???)render_continueが消えちゃう。描画順序の問題(???)。 */
//		psp_pause_filter(); 	/* ポーズフィルター予定 */
	}
	#endif
	{
		psp_push_screen();
	}
//
	{
		const static MENU_RESOURCE my_menu_resource[MENU_ITEM_99_MAX] =
		{
			//	RES08_ASK_CONTINUE_MENU 	//	/*const*/ char *ask_continue_menu_options[] =/*(char *)*/ask_continue_menu_options
			{	80, 		"CONTINUE YES", 	},
			{	90, 		"CONTINUE NO",		},
		//	{	0,NULL, 	},
		};
		int i;
		for (i=0; i<MENU_ITEM_99_MAX; i++)
		{
			menu_item_surface[i] = NULL;
			menu_item_surface[i] = font_render( (char *)my_menu_resource[i].str_name, FONT16R);
//			SDL_SetColorKey(menu_item_surface[i], (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
//			/* カラーキー(抜き色、透明色)は黒 */
			{
				menu_item_my_obj[i].x256			= t256(0);
				menu_item_my_obj[i].y256			= t256(0);
				menu_item_my_obj[i].MENU_DATA_i0_256	= ((my_menu_resource[i].x_offset)<<8);
				menu_item_my_obj[i].MENU_DATA_i1_256	= (((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(i*((5+16))))<<8);
				menu_item_my_obj[i].MENU_DATA_i2		= 0;
				menu_item_my_obj[i].MENU_DATA_i3		= 0;
			}
		}
	}
//
	active_item 		= MENU_ITEM_00_CONTINUE_YES;/*"CONTINUE YES"*/
	menu_brite			= (0);
//	fade_out_flag		= (0)/*set_fade_out_flag*/;/*構造的問題*/
//	pause_start_time	= psp_get_uint32_ticks();
	main_call_func = ask_continue_menu_00_FADE_IN_MENU;
}
