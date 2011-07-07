
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ステージ選択メニュー
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

#define MAX_MENU_ITEMS (8)
typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i0;	// 表示用 x オフセット */
	int MENU_DATA_i1;	// 表示用 y オフセット */
	int MENU_DATA_i2;	// スプライト用(メニュー選択時、横 x の揺れ幅) */
	int MENU_DATA_i3;	// スプライト用(揺れ幅が徐々に戻る用) */
} MY_OBJ;


typedef struct
{
	MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS];			/* オブジェクト */
	SDL_Surface 	*menu_item_surface[MAX_MENU_ITEMS]; 		/* 文字の画像 */
	int 			active_item;	/* 現在メニュー上で選択されている項目番号 */
	int 			max_items;		/* オプションの項目数 */
//
	int 			menu_brite; 	/* メニューの明るさ(α値) */
} MENU;
#define M1_NOT_TIME_OUT 		(-1)/* 時間切れなしに設定(時間切れなしモード) */
//#define M2_STATE_TIME_OVER	(-2)/* 時間切れありモードで時間切れになった状態 */

static MENU aaa;
/*---------------------------------------------------------

---------------------------------------------------------*/


typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_menu_resource[/*8*/MAX_MENU_ITEMS/*10*/] =
{
		//	RES01_STAGE_SELECT	/*const*/ char *stage_select_menu_options[] =/*(char *)*/stage_select_menu_options
		{	360,		"1",			},	// stage1 */
		{	350,		"2",			},	// stage2 */
		{	340,		"3",			},	// stage3 */
		{	330,		"4",			},	// stage4 */
		{	320,		"5",			},	// stage5 */
		{	310,		"6",			},	// stage6 */
	//	{	275,		"ENDING",		},	// ending */
		{	0,NULL, 	},
};

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
static void stage_select_menu_draw_all(void)
{
	{
		static int angle1024/*=0*/;
		angle1024 += (FPS_MENU_FACTOR15+FPS_MENU_FACTOR15);
		mask1024(angle1024);
		int i;
		for (i=0; i<aaa.max_items; i++)
		{
			if (i==aaa.active_item) 	/* 選択された */
			{
				aaa.menu_item_my_obj[i].MENU_DATA_i2 = 12;	/* ゆれ幅を 12 にする */
			}
			else	/* 選択されてない */
			{
				if ((aaa.menu_item_my_obj[i].MENU_DATA_i2) && (0==aaa.menu_item_my_obj[i].MENU_DATA_i3))
				{
					aaa.menu_item_my_obj[i].MENU_DATA_i2--;
					aaa.menu_item_my_obj[i].MENU_DATA_i3 = 3;
				}
			}
			if (aaa.menu_item_my_obj[i].MENU_DATA_i3)
			{	aaa.menu_item_my_obj[i].MENU_DATA_i3--;}
			aaa.menu_item_my_obj[i].x256 = (aaa.menu_item_my_obj[i].MENU_DATA_i0*256)+(cos1024((angle1024))*aaa.menu_item_my_obj[i].MENU_DATA_i2);
		//	aaa.menu_item_my_obj[i].y256 = (aaa.menu_item_my_obj[i].MENU_DATA_i1*256)+(sin1024((angle1024))*aaa.menu_item_my_obj[i].MENU_DATA_i2);
			aaa.menu_item_my_obj[i].y256 = (aaa.menu_item_my_obj[i].MENU_DATA_i1*256);
		//	{
		//		aaa.menu_item_my_obj[i].aaa.menu_brite	= aaa.menu_brite;
		//	}
		}
	}
//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//	pause_sp rite_display();/*SP_GROUP_TEXTS*/
	/* 描画 */
	psp_pop_screen();
	{			/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
		int i;
		for (i=0; i<aaa.max_items; i++)
		{
			rect_locate_offset.x = ((aaa.menu_item_my_obj[i].x256)>>8);
			rect_locate_offset.y = ((aaa.menu_item_my_obj[i].y256)>>8);
		//	SDL_SetAlpha(	aaa.menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			SDL_SetAlpha(	aaa.menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(aaa.menu_brite));
			SDL_BlitSurface(aaa.menu_item_surface[i], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
		}
	}
}
/*---------------------------------------------------------
	fadout fininshed, menu done
---------------------------------------------------------*/


static void stage_select_menu_work_MENU_STATE_03_FININSH(void)
{
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
	psp_pop_screen();
//	{	int i;
//		for (i=0; i<aaa.max_items; i++)
//		{
//			//SDL_FreeSurface(aaa.menu_item_surface[i]);
//		}
//	}
	cg.game_continue_stage = aaa.active_item;
	/* プラクティス ゲーム開始 */
	main_call_func = difficulty_select_menu_start;	/* 難易度選択メニューへ */
//	www=FPS_MENU_FACTOR10;
//	return;
//not stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	selection done, decreasing aaa.menu_brite, fade menu out
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_02_FADE_OUT(void)
{
	aaa.menu_brite -= FPS_MENU_FACTOR8;
	if (0 >= (aaa.menu_brite) )
	{
		aaa.menu_brite	= 0;
		main_call_func = stage_select_menu_work_MENU_STATE_03_FININSH;	/* */
	}
	stage_select_menu_draw_all();
}
/*---------------------------------------------------------
	fadein complete
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_01_WORK_MENU(void)
{
	if (0==cg_my_pad_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (cg_my_pad & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
		{
			voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
		}
		if (cg_my_pad & PSP_KEY_DOWN)
		{
			if (aaa.active_item == aaa.max_items-1)
			{	aaa.active_item = 0;}
			else
			{
				aaa.active_item++;
			}
		//	www=FPS_MENU_FACTOR10;
		}
		else if (cg_my_pad & PSP_KEY_UP)
		{
			if (0 == aaa.active_item)
			{	aaa.active_item = aaa.max_items-1;}
			else
			{
				aaa.active_item--;
			}
		//	www=FPS_MENU_FACTOR10;
		}
		if (cg_my_pad & PSP_KEY_SHOT_OK)
		{
			voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
			main_call_func = stage_select_menu_work_MENU_STATE_02_FADE_OUT; 	/* メニュー消去準備 */
		}
	}
	stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	Increasing aaa.menu_brite, fade menu in
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_00_FADE_IN_MENU(void)
{
//	static int www=10;
	aaa.menu_brite += FPS_MENU_FACTOR8;
	if (255 <= (aaa.menu_brite) )
	{
		aaa.menu_brite = 255;
		main_call_func = stage_select_menu_work_MENU_STATE_01_WORK_MENU;	/* */
	}
	stage_select_menu_draw_all();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/global void stage_select_menu_start(void)
{
	int i;
	for (i=0; i<MAX_MENU_ITEMS/*20*/; i++)
	{
		aaa.menu_item_surface[i] = NULL;
		{
			aaa.menu_item_my_obj[i].x256			= 0;
			aaa.menu_item_my_obj[i].y256			= 0;

			aaa.menu_item_my_obj[i].MENU_DATA_i0	= 0;
			aaa.menu_item_my_obj[i].MENU_DATA_i1	= 0;
			aaa.menu_item_my_obj[i].MENU_DATA_i2	= 0;
			aaa.menu_item_my_obj[i].MENU_DATA_i3	= 0;
		}
	}
	aaa.max_items		= 0;
//
	{
loop:
	{
		if (aaa.max_items>(option_config[OPTION_CONFIG_07_OPEN]&0x07))	{	goto loop_quit;}
	}
		/*while*/if (NULL == my_menu_resource[aaa.max_items].str_name)
		{	goto loop_quit;}
		aaa.menu_item_surface[aaa.max_items] = font_render( (char *)my_menu_resource[aaa.max_items].str_name, FONT16R);
//		SDL_SetColorKey(aaa.menu_item_surface[aaa.max_items], (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
//		/* カラーキー(抜き色、透明色)は黒 */

		aaa.menu_item_my_obj[aaa.max_items].x256	= t256(0);
		aaa.menu_item_my_obj[aaa.max_items].y256	= t256(0);

		aaa.max_items++;
		goto loop;
	}
loop_quit:
	for (i=0; i<aaa.max_items; i++)
	{
		aaa.menu_item_my_obj[i].MENU_DATA_i0	= my_menu_resource[i/*aaa.max_items*/].x_offset;
		aaa.menu_item_my_obj[i].MENU_DATA_i1	= (PSP_HEIGHT272/2+40) -(((5+16))*((aaa.max_items)>>1)) +(i*((5+16)));
		aaa.menu_item_my_obj[i].MENU_DATA_i2	= 0;
		aaa.menu_item_my_obj[i].MENU_DATA_i3	= 0;
	}
//
	#if 1
	//	psp_pause_filter(); 	/* ポーズフィルター予定 */
	#endif
	{
		psp_push_screen();
	}
	aaa.menu_brite		= (0);
//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*構造的問題*/
	//keybo ard_clear();
	#if 1
	/*
		エキストラステージをプレイした後で、
		「エキストラステージ、プラクティス」と
		「エンディング、スタッフロール、プラクティス」が
		選べるバグがあるので、修正する。
	*/
	if ( (6-1) < cg.game_continue_stage)	{cg.game_continue_stage = (6-1);	/* (6-1) */}
	#endif
	aaa.active_item = cg.game_continue_stage;
	main_call_func = stage_select_menu_work_MENU_STATE_00_FADE_IN_MENU;
}
