
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	タイトルメニュー
	-------------------------------------------------------
	実際、現在の共通メニューは使いにくいし、
	戻れないという致命的な欠陥もありますが、
	それでも使ってるのは、「作るのがめんどくさい」＆「先に他にやる事がたくさんある」
	からです。
	そのうち、全廃して改良する予定です。
	順序からいって、Gu化完了してからの方がやり易いです。
	というかSDLで作っても２度手間(作り直し)になる。
---------------------------------------------------------*/

#include "kanji_system.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	MAIN_MENU_START = 0,
	MAIN_MENU_EXTRA_START,
	MAIN_MENU_PRACTICE_START,
//	MAIN_MENU_REPLAY,
	MAIN_MENU_STORY,
	MAIN_MENU_RESULT,
//	MAIN_MENU_MUSIC_ROOM,
	MAIN_MENU_OPTION,
	MAIN_MENU_QUIT,
	MENU_ITEM_99_MAX,	/* オプションの項目数 */
};


#define MAX_MENU_ITEMS (8)
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


//typedef struct
//{
static MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS/*20*/]; 			/* オブジェクト */
static SDL_Surface		*menu_item_surface[MAX_MENU_ITEMS/*20*/];			/* 文字の画像 */
static int				active_item;		/* 現在メニュー上で選択されている項目番号 */

//
static int				menu_brite; 		/* メニューの明るさ(α値) */
static int				time_out_flag;		/* -3==,  -2==, -1==時間切れなし, 0==時間切れ, 時間カウント中=1... */
//} MENU;
#define M1_NOT_TIME_OUT 	(-1)/* 時間切れなしに設定(時間切れなしモード) */
//#define M2_STATE_TIME_OVER	(-2)/* 時間切れありモードで時間切れになった状態 */


//static MENU start_menu;


/*---------------------------------------------------------

---------------------------------------------------------*/

	#define TITLE_X_OFFSET	 (0)/* メニュー右側 */
//	#define TITLE_X_OFFSET (230)/* メニュー左側 */
typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_menu_resource[/*8*/MAX_MENU_ITEMS/*10*/] =
{
	//	RES00_MAIN_MENU 	//		/*const*/ char *start_menu_options[] =/*(char *)*/start_menu_options
	{	360-TITLE_X_OFFSET, 	"START",			},	//	"Start"
	{	250-TITLE_X_OFFSET, 	"EXTRA START",		},	//	"Extra Start"			/* 330 */
	{	245-TITLE_X_OFFSET, 	"PRACTICE START",	},	//	"Practice Start"
//	{	400-TITLE_X_OFFSET, 	"REPLAY",			},	//	"Replay"
	{	320-TITLE_X_OFFSET, 	"STORY",			},	//	"Story" 		// ちょっと変更 */
	{	285-TITLE_X_OFFSET, 	"RESULT",			},	//	"Result"
//	{	310-TITLE_X_OFFSET, 	"MUSIC ROOM",		},	//	"Music Room"
	{	335-TITLE_X_OFFSET, 	"OPTION",			},	//	"Option"
	{	310-TITLE_X_OFFSET, 	"QUIT", 			},	//	"Quit"
	{	0,						NULL,				},	//	""
};

/*---------------------------------------------------------

---------------------------------------------------------*/

//#if 0
//	#define FPS_MENU_FACTOR8	(8*fps_factor)
//	#define FPS_MENU_FACTOR30	(30*fps_factor)
//#else
	#define FPS_MENU_FACTOR8	(16)	/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR30	(6) 	/*4*/
//#endif


/*---------------------------------------------------------
	メニューの文字列を描画する。サブルーチン。
---------------------------------------------------------*/
static void draw_menu_str(void)
{
	{
		static int angle1024/*=0*/;
		angle1024 += (FPS_MENU_FACTOR30);
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
	タイトルメニュー	完了処理
	-------------------------------------------------------
---------------------------------------------------------*/

//		{	int i;
//			for (i=0; i<MENU_ITEM_99_MAX; i++)
//			{
//				//SDL_FreeSurface(menu_item_surface[i]);
//			}
//		}
		#if 0
//		if ((-2) == time_out_flag)/*(???)*/
//		{
//			time_out_flag = (-3);	/*(???)*/
//		}
		#endif


static void generic_menu_workMENU_STATE_03_FININSH(void)
{
	{
		SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
		psp_pop_screen();
	}
	{
		#if 0/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
		if (-3 == /*start_menu.*/time_out)
		{
			/* 時間切れで、強制的にデモ表示  / Timeout, go on and show HighScore List */
			main_call_func = game_play_demonstration;
		}
		else
		#endif
		{
			switch (active_item)
			{
			case MAIN_MENU_START:			cg.game_practice_mode = 0;	cg.game_continue_stage = (1-1); 			/*(0)*/ 	/* STAGE1 == 0 == (1-1)*/
											main_call_func = difficulty_select_menu_start;			break; /* Start */				/* 難易度選択メニューへ */
			case MAIN_MENU_EXTRA_START: 	cg.game_practice_mode = 0;	cg.game_continue_stage = (8-1); 			/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
											if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	cg.game_continue_stage++;	}/* PHANTASM DEBUG */
											main_call_func = difficulty_select_menu_start;			break; /* Extra Start */		/* 難易度選択メニューへ */
			case MAIN_MENU_PRACTICE_START:	cg.game_practice_mode = 1;
											main_call_func = stage_select_menu_start;				break; /* Practice Start */
		//	case MAIN_MENU_REPLAY:																	break; /* Replay */
			case MAIN_MENU_STORY:			main_call_func = story_script_start;					break; /* Story */
			case MAIN_MENU_RESULT:			main_call_func = yume_no_kiroku_start;					break; /* Result */
		//	case MAIN_MENU_MUSIC_ROOM:																break; /* Music Room */
			case MAIN_MENU_OPTION:			main_call_func = option_menu_start; 					break; /* Option */
			case MAIN_MENU_QUIT:			main_call_func = NULL;									break; /* Quit */
			}
		}
	}
}


/*---------------------------------------------------------
	タイトルメニュー	フェードアウト
	-------------------------------------------------------
	選択が決定したので、メニューを暗くしながらフェードアウトさせる。
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_02_FADE_OUT(void)
{
	menu_brite -= (FPS_MENU_FACTOR8);
	if (0 >= (menu_brite) )
	{
		main_call_func = generic_menu_workMENU_STATE_03_FININSH;
		menu_brite	= 0;
	}
	draw_menu_str();
}


/*---------------------------------------------------------
	タイトルメニュー	選択する
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_01_WORK_MENU(void)
{
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
				{	active_item = 0;}
				else
				{
					/* エキストラメニュー選択関連 */
					{
						if ((MAIN_MENU_EXTRA_START-1)==active_item)
						{
							/* 0==						(					  1面までの練習は選べる) */
							/* 1==		  2面まで到達	(					  2面までの練習は選べる) */
							/* ... */
							/* 5==最終面==6面まで到達	(EXTRA STAGE選べない、6面までの練習は選べる) */
							/* 6==最終面==6面クリア 	(EXTRA STAGE選べる) */
							/* 7==EXTRA面==EXTRA面クリア(EXTRA STAGE選べる) */
							if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
							{
								active_item++;
							}
						}
					}
					active_item++;
				}
				time_out_flag = M1_NOT_TIME_OUT;/* 時間切れなし */
			}
			else if (psp_pad.pad_data & PSP_KEY_UP)
			{
				if (0 == active_item)
				{	active_item = MENU_ITEM_99_MAX-1;}
				else
				{
					/* エキストラメニュー選択関連 */
					{
						if ((MAIN_MENU_EXTRA_START+1)==active_item)
						{
							if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
							{
								active_item--;
							}
						}
					}
					active_item--;
				}
				time_out_flag = M1_NOT_TIME_OUT;/* 時間切れなし */
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
				main_call_func = generic_menu_workMENU_STATE_02_FADE_OUT;	/* メニュー消去準備 */
				time_out_flag	= M1_NOT_TIME_OUT;/* 時間切れなし */
			}
		}
		/* 時間切れありモード用、時間経過 */
		if (0 < (time_out_flag) )
		{
			time_out_flag--;// -= FPS_MENU_FACTOR;
			//	time_out -= (1);
			if ((0) == time_out_flag)/* 時間切れ */
			{
				/* 時間切れありモードで時間切れになった */
				main_call_func = generic_menu_workMENU_STATE_02_FADE_OUT;	/* メニュー消去準備 */
			//	time_out_flag	= M2_STATE_TIME_OVER/*(-2)*/;/* 時間切れあり */
			}
		}
	}
	draw_menu_str();
}

/*---------------------------------------------------------
	タイトルメニュー	フェードイン
	-------------------------------------------------------
	明るくしながら、メニューをフェードインさせる。
	Increasing menu_brite, fade menu in
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_00_FADE_IN_MENU(void)
{
	menu_brite += (FPS_MENU_FACTOR8);
	if (255 <= (menu_brite) )
	{
		main_call_func = generic_menu_workMENU_STATE_01_WORK_MENU;
		menu_brite = 255;
	}
	draw_menu_str();
}

/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void title_logo_reset_position(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<(5/*+1*/); i++)
		{
				const u8 aaa_y[8] =
				{
					(8+  0),	// 東
					(8+ 51),	// 方
					(8+102),	// 模
					(8+153),	// 倣
					(8+204),	// 風
					0,0,0		// dummy(.align)
				};
			{
				SPRITE *h;
				h						= obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i);	/* 必ず登録できる。 */
			//
				h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->jyumyou			= JYUMYOU_MUGEN;/* 時間で自動消去しない */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
				h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= PANEL_STR_EASY+i;//(SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */

			//	h->cx256			= t256(100);//src->cx256 + ((sin1024((ww_angle1024))*radius));
			//	h->cy256			= (i<<(8+5));//src->cy256 + ((cos1024((ww_angle1024))*radius));
#if 0
				h->cx256			= t256(128);//src->cx256 + ((sin1024((ww_angle1024))*radius));
#else
	/* (r32では)対応が間にあわなかった。 */
				h->cx256			= t256(480+1);//src->cx256 + ((sin1024((ww_angle1024))*radius));
#endif
				h->cy256			= ((aaa_y[(i)])<<8);//src->cy256 + ((cos1024((ww_angle1024))*radius));
				h->m_zoom_x256		= t256(1.0);
				h->m_zoom_y256		= t256(1.0);
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
				h->rotationCCW1024	= (0);
				#endif
			}
		}
	}

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)
	#if 0
	// 低速effect
	SPRITE *h;
	h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
	h->color32		= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* 自機、半透明 */	/*	s1->alpha			= 0x50;*/
	#endif
}
/*---------------------------------------------------------

---------------------------------------------------------*/
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */
/*static*/global void title_menu_start(void)
{
	callback_gu_draw_haikei = NULL;//dr aw_bg_screen = 0;	/* 背景ウィンドウ表示off */
	set_music_volume(128);/*とりあえず*/
	load_SDL_bg(BG_TYPE_00_title_bg);
	psp_pop_screen();
	title_logo_reset_position();
//	int res_num,//		RES00_MAIN_MENU,
//	MENU *m =		&start_menu;

//
	int i;
	for (i=0; i<(MENU_ITEM_99_MAX); i++)
	{
		menu_item_surface[i] = NULL;
		menu_item_surface[i] = font_render( (char *)my_menu_resource[i].str_name, FONT16R);
//		SDL_SetColorKey(menu_item_surface[i], (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
//		/* カラーキー(抜き色、透明色)は黒 */
		{
			menu_item_my_obj[i].x256			= t256(0);
			menu_item_my_obj[i].y256			= t256(0);

			menu_item_my_obj[i].MENU_DATA_i0_256	= ((my_menu_resource[i].x_offset)<<8);
			menu_item_my_obj[i].MENU_DATA_i1_256	= (((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(i*((5+16))))<<8);
			menu_item_my_obj[i].MENU_DATA_i2		= 0;
			menu_item_my_obj[i].MENU_DATA_i3		= 0;
		}
	}
//	/* ここまでMENUの初期化 */
//
	#if 1
	/* てすと、構造的問題 */
	//if (RES09_PAUSE_MENU==res_num)
	//	int set_fade_out_flag,
//	if (0!=/*pause_filter_flag*/0)
//	{
//		psp_pause_filter(); 	/* ポーズフィルター予定 */
//	}
	#endif
	{
		psp_push_screen();
	}
//	active_item 		= (0);/* ←mainmenuの場合は位置を初期化しない */
	menu_brite			= (0);

//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*構造的問題*/
	time_out_flag	= M1_NOT_TIME_OUT/*時間切れなし*/;
	/* 時間切れ == 500 6000 RESULTにしたから要らなくなったが、将来デモとか付けるならいる */
	main_call_func = generic_menu_workMENU_STATE_00_FADE_IN_MENU;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global void old_menu_system_init(void)
{
//	active_item 		= (0);
}
