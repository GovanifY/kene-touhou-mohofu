
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

#define USE_REPLAY_MODE 	(0)/*(未作成)*/
/*---------------------------------------------------------

---------------------------------------------------------*/
#define USE_SDL_GAZO		(0)
enum
{
	MAIN_MENU_00_START = 0,
	MAIN_MENU_01_EXTRA_START,
	MAIN_MENU_02_PRACTICE_START,
	#if (1==USE_REPLAY_MODE)/*(未作成)*/
	MAIN_MENU_03_REPLAY,
	#else
	MAIN_MENU_03_STORY,
	#endif
	MAIN_MENU_04_RESULT,
	MAIN_MENU_05_MUSIC_ROOM,
	MAIN_MENU_06_OPTION,
	MAIN_MENU_07_QUIT,
	MENU_ITEM_99_MAX,	/* オプションの項目数 */
};


#define MAX_MENU_ITEMS (8)
typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i2;		// スプライト用(メニュー選択時、横 x の揺れ幅) */
	int MENU_DATA_i3;		// スプライト用(揺れ幅が徐々に戻る用) */
} MY_OBJ;


//typedef struct
//{
static MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS/*20*/]; 			/* オブジェクト */

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
#define MAX_JI_5 (0)
//	#define TITLE_X_OFFSET	 (0)/* メニュー右側 */
//	#define TITLE_X_OFFSET (230)/* メニュー左側 */
typedef struct
{
	int x_offset[6];
	int y_offset[6];
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_menu_resource[/*8*/MAX_MENU_ITEMS+MAX_JI_5/*10*/] =
{
	//	RES00_MAIN_MENU 	//		/*const*/ char *start_menu_options[] =/*(char *)*/start_menu_options
	{	{	512,	360,	360,	360,	512,	360,  },
		{	 92,	 92,	 92,	 92,	 92,	 92,  }, // ((113) +((-1)*(21)))
		"Start",
	},
	{	{	512,	250,	250,	250,	512,	250,  },
		{	113,	113,	113,	113,	113,	113,  }, // ((113) +(0*(21)))
		"Extra Start", /* 330 */
	},
	{	{	512,	245,	245,	245,	512,	245,  },
		{	134,	134,	134,	134,	134,	134,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(1*((5+16))))
		"Practice Start",	// "Practice Start"
	},
	#if (1==USE_REPLAY_MODE)/*(未作成)*/
	{	{	512,	400,	400,	400,	512,	400,  },
		{	155,	155,	155,	155,	155,	155,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(2*((5+16))))
		"Replay",			// "Replay"
	},
	#else
	{	{	512,	320,	320,	320,	512,	320,  },
		{	155,	155,	155,	155,	155,	155,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(2*((5+16))))
		"Story",			// "Story"
	},
	#endif
	{	{	512,	285,	285,	285,	512,	285,  },
		{	176,	176,	176,	176,	176,	176,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(3*((5+16))))
		"Result",			// "Result"
	},
	{	{	512,	310,	310,	310,	512,	310,  },
		{	197,	197,	197,	197,	197,	197,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(4*((5+16))))
		"Music Room",		// "Music Room"
	},
	{	{	512,	335,	335,	335,	512,	335,  },
		{	218,	218,	218,	218,	218,	218,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(5*((5+16))))
		"Option",			// "Option"
	},
	{	{	512,	310,	310,	310,	512,	310,  },
		{	239,	239,	239,	239,	239,	239,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(6*((5+16))))
		"Quit", 			// "Quit"
	},
//	{	{	  0,	  0,	  0,	  0,	  0,	  0,  },
//		{	  0,	  0,	  0,	  0,	  0,	  0,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(7*((5+16))))
//		NULL,	//	""
//	},
};

/*---------------------------------------------------------

---------------------------------------------------------*/

//#if 0
//	#define SPEED_04_FADE_IN	(8*fps_factor)
//	#define FPS_MENU_FACTOR30	(30*fps_factor)
//#else
//	#define SPEED_04_FADE_IN	(16)	/* 8*FPS_MENU_FACTOR */
//#endif
	#define FPS_MENU_FACTOR30	(6) 	/*4*/

/* 登場速度は、ゆっくり。 */
	#define SPEED_04_FADE_IN	(4) 	/* 4*FPS_MENU_FACTOR */
/* 退避速度は、速い。 */
	#define SPEED_06_FADE_OUT	(6) 	/* 6*FPS_MENU_FACTOR */

static int move_mode;
static int move_count;
/*---------------------------------------------------------
	メニューの文字列を描画する。サブルーチン。
---------------------------------------------------------*/
static void move_menu_str(void)
{
//	move_count++;
	move_count = menu_brite;
	move_count = psp_min(move_count, 256);
	move_count = psp_max(move_count, 0);
	{
		static int angle1024;/*=0*/
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
			int x_sa;
				x_sa  = (((my_menu_resource[i].x_offset[move_mode+move_mode  ])*(256-move_count))>>8);
				x_sa += (((my_menu_resource[i].x_offset[move_mode+move_mode+1])*(	 move_count))>>8);
				{
					int sin_value_t256; 		//	sin_value_t256 = 0;
					int cos_value_t256; 		//	cos_value_t256 = 0;
					int256_sincos1024( (((angle1024)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
					menu_item_my_obj[i].x256 = ((x_sa)<<8)+(((sin_value_t256))*menu_item_my_obj[i].MENU_DATA_i2);
				}
		//	menu_item_my_obj[i].y256 = (menu_item_my_obj[i].MENU_DATA_i1_256)+(si n1024((angle1024))*menu_item_my_obj[i].MENU_DATA_i2);
			menu_item_my_obj[i].y256 = ((my_menu_resource[i].y_offset[move_mode+move_mode])<<8);
		//	{
		//		menu_item_my_obj[i].menu_brite	= menu_brite;
		//	}
			#if 1
			ml_font[i].x = ((menu_item_my_obj[i].x256)>>8);/* X位置 */
			ml_font[i].y = ((menu_item_my_obj[i].y256)>>8);
			#endif
		}
	}
}
/*---------------------------------------------------------
	メニューの文字列を描画する。サブルーチン。
---------------------------------------------------------*/
static void draw_menu_str(void)
{
	move_menu_str();
//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//	pause_sp rite_display();/*SP_GROUP_TEXTS*/
	/* 描画 */
	psp_pop_screen();
}

/*---------------------------------------------------------
	タイトルメニュー	完了処理
	-------------------------------------------------------
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_03_FININSH(void)
{
	{
		SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, 255);
		psp_pop_screen();
	}
	{
		#if 0/*RESULTにしたから要らなくなったが、将来デモとか付けるならいる*/
		if (-3 == /*start_menu.*/time_out)
		{
			/* 時間切れで、強制的にデモ表示  / Timeout, go on and show HighScore List */
			cb.main_call_func = game_play_demonstration;
		}
		else
		#endif
		{
			void *aaa[8] =
			{
				&&case_MAIN_MENU_00_START,
				&&case_MAIN_MENU_01_EXTRA_START,
				&&case_MAIN_MENU_02_PRACTICE_START,
				#if (1==USE_REPLAY_MODE)/*(未作成)*/
				&&case_MAIN_MENU_03_REPLAY, 		//	&&MAIN_MENU_break,
				#else
				&&case_MAIN_MENU_03_STORY,
				#endif
				&&case_MAIN_MENU_04_RESULT,
				&&case_MAIN_MENU_05_MUSIC_ROOM,
				&&case_MAIN_MENU_06_OPTION,
				&&case_MAIN_MENU_07_QUIT,
			};
			goto *aaa[(active_item&0x07)];
			{
			case_MAIN_MENU_00_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (1-1);			/*(0)*/ 	/* STAGE1 == 0 == (1-1)*/
				cb.main_call_func		= difficulty_select_menu_start; 	/* 難易度選択メニューへ */
				goto MAIN_MENU_break; /* Start */
			case_MAIN_MENU_01_EXTRA_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (8-1);			/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
				if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	cg.game_continue_stage++;	}/* PHANTASM DEBUG */
				cb.main_call_func		= difficulty_select_menu_start; 	/* 難易度選択メニューへ */
				goto MAIN_MENU_break; /* Extra Start */
			case_MAIN_MENU_02_PRACTICE_START:
				cg.game_practice_mode	= 1;
				cb.main_call_func		= difficulty_select_menu_start; 	/* 難易度選択メニューへ */
				goto MAIN_MENU_break; /* Practice Start */
			#if (1==USE_REPLAY_MODE)/*(未作成)*/
			case_MAIN_MENU_03_REPLAY:		cb.main_call_func		= replay_mode_start;		goto MAIN_MENU_break; /* Replay */
			#else
			case_MAIN_MENU_03_STORY:		cb.main_call_func		= story_mode_start; 		goto MAIN_MENU_break; /* Story */
			#endif
			case_MAIN_MENU_04_RESULT:		cb.main_call_func		= yume_no_kiroku_start; 	goto MAIN_MENU_break; /* Result */
			case_MAIN_MENU_05_MUSIC_ROOM:	cb.main_call_func		= music_room_start; 		goto MAIN_MENU_break; /* Music Room */
			case_MAIN_MENU_06_OPTION:		cb.main_call_func		= option_menu_start;		goto MAIN_MENU_break; /* Option */
			case_MAIN_MENU_07_QUIT: 		cb.main_call_func		= NULL; 					goto MAIN_MENU_break; /* Quit */
			}
			MAIN_MENU_break:
				;
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
	menu_brite -= (SPEED_06_FADE_OUT);
	if (0 >= (menu_brite) )
	{
		cb.main_call_func = generic_menu_workMENU_STATE_03_FININSH;
		menu_brite	= 0;
	}
	draw_menu_str();
}


/*---------------------------------------------------------
	タイトルメニュー	選択する
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_01_WORK_MENU(void)
{
	cg.msg_time = /*(永遠に描画)*/byou60(5);	/* 約 5 秒 */
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
						if ((MAIN_MENU_01_EXTRA_START-1)==active_item)
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
						if ((MAIN_MENU_01_EXTRA_START+1)==active_item)
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
				move_mode++;
				move_count = 0;
				cb.main_call_func = generic_menu_workMENU_STATE_02_FADE_OUT;	/* メニュー消去準備 */
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
				move_mode++;
				move_count = 0;
				cb.main_call_func = generic_menu_workMENU_STATE_02_FADE_OUT;	/* メニュー消去準備 */
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
	menu_brite += (SPEED_04_FADE_IN);
	if (255 <= (menu_brite) )
	{
		move_mode++;
		move_count = 0;
		cb.main_call_func = generic_menu_workMENU_STATE_01_WORK_MENU;
		menu_brite = (255);
	}
	draw_menu_str();
}

/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void title_logo_reset_position(void)
{
	//----[EFFECT]
	{	unsigned int i;
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
				h					= obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i);	/* 必ず登録できる。 */
			//
				h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->jyumyou			= JYUMYOU_MUGEN;/* 時間で自動消去しない */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
				h->flags			&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= PANEL_STR_EASY+i;//(SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
				//
			//	h->cx256			= t256(100);//src->cx256 + ((si n1024((ww_angle1024))*radius));
			//	h->cy256			= (i<<(8+5));//src->cy256 + ((co s1024((ww_angle1024))*radius));
				#if (0)
				h->cx256			= t256(128);//src->cx256 + ((si n1024((ww_angle1024))*radius));
				#else
				/* (r35でも)対応が間にあわなかった。 */
				h->cx256			= t256(480+1);//src->cx256 + ((si n1024((ww_angle1024))*radius));
				#endif
				h->cy256			= ((aaa_y[(i)])<<8);//src->cy256 + ((co s1024((ww_angle1024))*radius));
				h->m_zoom_x256		= t256(1.0);
				h->m_zoom_y256		= t256(1.0);
				#if 1
				/* 描画用角度(下が0度で左回り(反時計回り)) */
				h->rotationCCW1024	= (0);
				#endif
			}
		}
	}
//	#define TEISOKU_EFFECT_00_OBJ (6)
//	#define TEISOKU_EFFECT_01_OBJ (7)
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

/*static*/global void title_menu_start(void)
{
	cb.callback_gu_draw_haikei	= NULL;//dr aw_bg_screen = 0;	/* 背景ウィンドウ表示off */
	set_music_volume(128);/*とりあえず*/
	load_SDL_bg(BG_TYPE_00_title_bg);
	psp_pop_screen();
	title_logo_reset_position();
//	int res_num,//		RES00_MAIN_MENU,
//	MENU *m =		&start_menu;

//
	cg.SDL_font_type	= FONT16W;
	//
	unsigned int yyy18;
	yyy18 = 0;
	//	kanji_window_clear_line(0); 			/* 漢字ウィンドウの1行目(==0)の内容を消す。 */
		kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
	//
	int i;
	for (i=0; i<(MENU_ITEM_99_MAX); i++)
	{
		strcpy(my_font_text, (char *)my_menu_resource[i].str_name);
		//
		kanji_window_clear_line(i); 	/* 漢字ウィンドウのn行目(==0)の内容を消す。 */
		set_kanji_xy( (0), (yyy18) );
		kanji_color(7);
		#if (1)/*(????)*/
		kanji_draw();
		#else
		/*
			(r35)何故か巧くいかない。
			(ここは問題無いが、他の部分の font surface が消えちゃう)
		 */
		kanji_ascii_font16_draw();
		#endif
		//
		{
			menu_item_my_obj[i].x256			= t256(0);
			menu_item_my_obj[i].y256			= t256(0);




			menu_item_my_obj[i].MENU_DATA_i2		= 0;
			menu_item_my_obj[i].MENU_DATA_i3		= 0;
		}
		ml_font[i].x = ((((my_menu_resource[i].x_offset[0]) )) );/* X位置 */
		ml_font[i].y = ((((my_menu_resource[i].y_offset[0]) )) );
		ml_font[i].timer	= ML_ON;
	//	ml_font[i].render	= NULL;
	//	ml_font[i].str		= my_font_text;/*(デバッグ)*/
		yyy18 += (18);
	}
	for (; i<8; i++)
	{
		ml_font[i].x		= (32);/* X位置 */
		ml_font[i].y		= (32);
		ml_font[i].timer	= ML_OFF;
	//	ml_font[i].render	= NULL;
	//	ml_font[i].str		= NULL;
	}
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */
	cg.msg_time = (65536);	/* 約 18 分 */
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
	move_mode		= 0;
	move_count		= 0;
	cb.main_call_func = generic_menu_workMENU_STATE_00_FADE_IN_MENU;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global void old_menu_system_init(void)
{
//	active_item 		= (0);
}
