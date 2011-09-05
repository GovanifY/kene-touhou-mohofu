
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	タイトルメニュー
	-------------------------------------------------------
	Replay に対応したり。
	時間でアドバタイズ(オートプレイデモ)になったりする予定は
	あるけど、今(r36)の所未定です。(だいぶ先かと)
---------------------------------------------------------*/

#include "kanji_system.h"

#include "../menu/kaiwa_sprite.h"

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	MAIN_MENU_00_START = 0,
	MAIN_MENU_01_EXTRA_START,
	MAIN_MENU_02_PRACTICE_START,
	MAIN_MENU_03_REPLAY,
	MAIN_MENU_04_RESULT,
	MAIN_MENU_05_MUSIC_ROOM,
	MAIN_MENU_06_OPTION,
	MAIN_MENU_07_QUIT,
	MENU_ITEM_99_MAX,	/* オプションの項目数 */
};

typedef struct
{
	int x256;
	int y256;
//
	int x_yurehaba; 	// 揺れ幅(メニュー選択時、横 x の揺れ幅) */
	int x_okureti;		// 遅れ値(揺れ幅が徐々に戻る用のディレイフレームを設定) 3[フレーム] */
} MY_OBJ;

//typedef struct
//{
static MY_OBJ	menu_item_my_obj[MENU_ITEM_99_MAX];			/* オブジェクト */
static int		active_item;		/* 現在メニュー上で選択されている項目番号。(選択中カーソル位置) */
static int		toutatu_wariai; 	/* メニューの到達割合(0 ... 255)。 */
static int		time_out_flag;		/* -3==,  -2==, -1==時間切れなし, 0==時間切れ, 時間カウント中=1... */
//} MENU;
	#define M1_NOT_TIME_OUT 	(-1)/* 時間切れなしに設定(時間切れなしモード) */
//	#define M2_STATE_TIME_OVER	(-2)/* 時間切れありモードで時間切れになった状態 */
//static MENU start_menu;

/*---------------------------------------------------------

---------------------------------------------------------*/
#define USE_REPLAY_MODE 	(0)/*(未作成)*/
#if (1==USE_REPLAY_MODE)/*(未作成)*/
	#define STR_REPLAY	"Replay"
#else
	#define STR_REPLAY	"Story"
#endif
/*(計4状態==3状態+1状態)*/

enum
{
	MODE_00_TOUJYOU = 0,	/*(move_mode[0]: メニュー文字登場中)*/
	MODE_01_MENU_SELECT,	/*(move_mode[1]: メニュー選択中)*/
	MODE_02_TAIJYOU,		/*(move_mode[2]: メニュー文字退避中)*/
	MODE_03_TAIHI_ITI,		/*(+退避位置の先読み分)*/
	MODE_04_MAX_JYOUTAI/*(最大値)*/
};
typedef struct
{
	s16 x_offset[MODE_04_MAX_JYOUTAI];
	s16 y_offset[MODE_04_MAX_JYOUTAI];
	const char *str_name;
} MENU_RESOURCE;
static const MENU_RESOURCE my_menu_resource[MENU_ITEM_99_MAX] =
{
// mode:	[0] 	[1] 	[2] 	[2+1]
	{	{	100,	360,	360,	400,  },
		{	-16,	 92,	 92,	-16,  }, "Start",			},
	{	{	200,	250,	250,	300,  },
		{	-16,	113,	113,	-16,  }, "Extra Start", 	},
	{	{	300,	245,	245,	200,  },
		{	-16,	134,	134,	-16,  }, "Practice Start",	},
	{	{	512,	320,	320,	512,  },// (x_offset: 400)
		{	155,	155,	155,	155,  }, STR_REPLAY,		},
	{	{	100,	285,	285,	200,  },
		{	272,	176,	176,	272,  }, "Result",			},
	{	{	200,	310,	310,	100,  },
		{	272,	197,	197,	272,  }, "Music Room",		},
	{	{	300,	335,	335,	512,  },
		{	272,	218,	218,	100,  }, "Option",			},
	{	{	400,	310,	310,	512,  },
		{	272,	239,	239,	200,  }, "Quit",			},
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
/*---------------------------------------------------------
	メニューの文字列を描画する。サブルーチン。
---------------------------------------------------------*/
static void move_menu_str(void)
{
	int move_rate256;
	move_rate256 = toutatu_wariai;
	move_rate256 = psp_min(move_rate256, 256);
	move_rate256 = psp_max(move_rate256, 0);
	#if (1)/* 非等分値の計算 */
	{
		u32 i_rate65536;
		i_rate65536 = (move_rate256 << 8);
	//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
		i_rate65536 = (int)(vfpu_ease_in_out65536(i_rate65536));	/* t65536の等分カウンタ値を使い、非等分になるよう値を修正する。
		非等分の方式は「ease_in_out」これは、始めと最後がゆっくりで間が速いという配分方式。 */
		move_rate256 = (i_rate65536 >> 8);
	}
	#endif
	{
		static int angle1024;
		angle1024 += (FPS_MENU_FACTOR30);
		mask1024(angle1024);
		/*unsigned*/ int i;
		for (i=0; i<MENU_ITEM_99_MAX; i++)/*(全行処理する)*/
		{
			if (i==active_item) /*(選択された)*/
			{
				menu_item_my_obj[i].x_yurehaba = (12);	/*(揺れ幅を 12 に設定。)*/
			}
			else	/*(選択されてない)*/
			{
				if ((menu_item_my_obj[i].x_yurehaba)/*(揺れ幅があり)*/
					 && (0==menu_item_my_obj[i].x_okureti)/*(遅れ値が解消済みの場合)*/
				)
				{
					menu_item_my_obj[i].x_yurehaba--;	/*(揺れ幅を減らす。)*/
					menu_item_my_obj[i].x_okureti = (3);/*(遅れ値を 3 フレームに設定。)*/
				}
			}
			/*(遅れ値を計算)*/
			if (menu_item_my_obj[i].x_okureti)/*(遅れ値がある場合)*/
			{
				menu_item_my_obj[i].x_okureti--;	/*(遅れ値を減らす。)*/
			}
			//------------------
			HATSUDAN_01_speed256	= ((menu_item_my_obj[i].x_yurehaba)<<8);
			HATSUDAN_03_angle65536	= deg1024to65536(angle1024);
			sincos256();/*(破壊レジスタ多いので注意)*/
			REG_08_REG0 = REG_02_DEST_X;//cos_value_t256	// 横だけ揺らす。
		//	REG_09_REG1 = REG_03_DEST_Y;//sin_value_t256	// 縦は揺らさない。(== y offset は使わない。)
			//------------------
			#if (1)/*(到達割合を考慮して、合成する。)*/
			REG_00_SRC_X	= ((my_menu_resource[i].x_offset[move_mode	])<<8); 	/*(合成位置[A]t256()形式)*/
			REG_01_SRC_Y	= ((my_menu_resource[i].y_offset[move_mode	])<<8); 	/*(合成位置[A]t256()形式)*/
			REG_02_DEST_X	= ((my_menu_resource[i].x_offset[move_mode+1])<<8); 	/*(合成位置[B]t256()形式)*/
			REG_03_DEST_Y	= ((my_menu_resource[i].y_offset[move_mode+1])<<8); 	/*(合成位置[B]t256()形式)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(合成割合t256()形式)*/
			multiprex_rate_vector();/*(破壊レジスタ多いので注意)*/
			#endif
			// この時点で、
			// REG_02_DEST_X: 合成値
			// REG_03_DEST_Y: 合成値
			//-----------------------
			menu_item_my_obj[i].x256 = (REG_02_DEST_X)+(REG_08_REG0);		// 横だけ揺らす。
			menu_item_my_obj[i].y256 = (REG_03_DEST_Y);//+(REG_09_REG1);	// 縦は揺らさない。(== y offset は使わない。)
			#if 1
			ml_font[i].x = ((menu_item_my_obj[i].x256)>>8);/*(X位置)*/
			ml_font[i].y = ((menu_item_my_obj[i].y256)>>8);/*(Y位置)*/
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
	/* 描画 */
	psp_pop_screen();
}


/*---------------------------------------------------------
	[難易度選択メニュー#01]の開始。
---------------------------------------------------------*/
static void title_menu_05_start_wait(void)
{
//	psp_clear_screen();
	/* [init] */
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合 */
	{
		psp_push_screen();/*(新規の場合のみ、バックバッファにタイトル背景画像を保存する。)*/
		cb.main_call_func = rank_select_menu_start;/* 難易度選択メニューへ(移動) */
	}
}


/*---------------------------------------------------------
	タイトルメニュー	完了処理
	-------------------------------------------------------
---------------------------------------------------------*/

static void title_menu_04_osimai(void)
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
			static const void *jump_menu_type[(8)] =
			{
				&&case_MAIN_MENU_00_START,
				&&case_MAIN_MENU_01_EXTRA_START,
				&&case_MAIN_MENU_02_PRACTICE_START,
				&&case_MAIN_MENU_03_REPLAY,
				&&case_MAIN_MENU_04_RESULT,
				&&case_MAIN_MENU_05_MUSIC_ROOM,
				&&case_MAIN_MENU_06_OPTION,
				&&case_MAIN_MENU_07_QUIT,
			};
			goto *jump_menu_type[(active_item&0x07)];
			{
			case_MAIN_MENU_00_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (1-1);			/*(0)*/ 	/* STAGE1 == 0 == (1-1)*/
				cb.main_call_func		= title_menu_05_start_wait; 	/* 難易度選択メニューへ */
				goto MAIN_MENU_break; /* Start */
			case_MAIN_MENU_01_EXTRA_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (8-1);			/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
				if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	cg.game_continue_stage++;	}/* PHANTASM DEBUG */
				cb.main_call_func		= title_menu_05_start_wait; 	/* 難易度選択メニューへ */
				goto MAIN_MENU_break; /* Extra Start */
			case_MAIN_MENU_02_PRACTICE_START:
				cg.game_practice_mode	= 1;
				cb.main_call_func		= title_menu_05_start_wait; 	/* 難易度選択メニューへ */
				goto MAIN_MENU_break; /* Practice Start */
			#if (1==USE_REPLAY_MODE)/*(未作成)*/
			case_MAIN_MENU_03_REPLAY:		cb.main_call_func		= replay_mode_start;		goto MAIN_MENU_break; /* Replay */
			#else
			case_MAIN_MENU_03_REPLAY:		cb.main_call_func		= story_mode_start; 		goto MAIN_MENU_break; /* Story */
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
	タイトルメニュー#03:	文字が散らばる。
	-------------------------------------------------------
	選択が確定(または時間切れ)したので、文字を退避させる。
---------------------------------------------------------*/
static void title_menu_03_moji_taihi(void)
{
	toutatu_wariai += (SPEED_06_FADE_OUT);
	if (255 <= (toutatu_wariai) )
	{
	//	toutatu_wariai	= (0);//(描画の都合上無理)
		cb.main_call_func = title_menu_04_osimai;
	}
	draw_menu_str();
}

/*---------------------------------------------------------
	タイトルメニュー#01:	文字が集まる。
---------------------------------------------------------*/

static void title_menu_02_select_menu(void);
static void title_menu_01_move_atumaru(void)
{
	toutatu_wariai += (SPEED_04_FADE_IN);
	if (255 <= (toutatu_wariai) )
	{
		toutatu_wariai = (0);
		move_mode++;//(1)になる。
		cb.main_call_func = title_menu_02_select_menu;
	}
	draw_menu_str();
}


/*---------------------------------------------------------
	タイトルメニュー#02:	選択する
---------------------------------------------------------*/

static void title_menu_02_select_menu(void)
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
				move_mode++;//(2)になる。
				cb.main_call_func = title_menu_03_moji_taihi;	/* メニュー文字退避移動 */
				time_out_flag	= M1_NOT_TIME_OUT;/* 時間切れなし */
			}
		}
		/* 時間切れありモード用、時間経過 */
		if (0 < (time_out_flag) )
		{
			time_out_flag--;//FPS_MENU_FACTOR;
			if ((0) == time_out_flag)/* 時間切れ */
			{
				/* 時間切れありモードで時間切れになった */
				move_mode++;//(2)になる。
				cb.main_call_func = title_menu_03_moji_taihi;	/* メニュー文字退避移動 */
			//	time_out_flag	= M2_STATE_TIME_OVER/*(-2)*/;/* 時間切れあり */
			}
		}
	}
	draw_menu_str();
}


#if 0
/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void title_logo_reset_position(void)
{
	#if 0
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
				OBJ *h;
				h					= obj_add_Ann_direct(OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_08_EFFECT01+i);	/* 必ず登録できる。 */
			//
				h					= &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->jyumyou			= JYUMYOU_MUGEN;/* 時間で自動消去しない */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
				h->atari_hantei 	= ATARI_HANTEI_OFF; 		/* あたり判定のOFF(無敵) */
			//	h->type 			= AAA;
				h->obj_type_set 			= PANEL_STR_EASY+i;//(SPELL_SQUERE_);
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
	#endif
//	#define TEISOKU_EFFECT_00_OBJ (6)
//	#define TEISOKU_EFFECT_01_OBJ (7)
	#if 0
	// 低速effect
	OBJ *h;
	h					= &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->atari_hantei 	= ATARI_HANTEI_OFF; 		/* あたり判定のOFF(無敵) */
	h->color32			= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* 自機、半透明 */	/*	s1->alpha			= 0x50;*/
	#endif
}
	#endif
/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/extern void kaiwa_init_obj_position(void);
/*static*/global void title_menu_start(void)
{
	cb.callback_gu_draw_haikei	= NULL;//dr aw_bg_screen = 0;	/* 背景ウィンドウ表示off */
	set_music_volume(128);/*とりあえず*/
	load_SDL_bg(BG_TYPE_00_title_bg);
	psp_pop_screen();
//	title_logo_reset_position();
//	int res_num,//		RES00_MAIN_MENU,
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
			menu_item_my_obj[i].x_yurehaba		= (0);	/*(揺れ幅を 0 に初期化する。)*/
			menu_item_my_obj[i].x_okureti		= (0);	/*(遅れ値を 0 に初期化する。)*/
		}
		ml_font[i].x = ((((my_menu_resource[i].x_offset[0]) )) );/*(X位置)*/
		ml_font[i].y = ((((my_menu_resource[i].y_offset[0]) )) );/*(Y位置)*/
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
//	active_item 			= (0);/* ←main menuの場合は位置を初期化しない */
	toutatu_wariai			= (0);

//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*構造的問題*/
	time_out_flag	= M1_NOT_TIME_OUT/*時間切れなし*/;
	/* 時間切れ == 500 6000 RESULTにしたから要らなくなったが、将来デモとか付けるならいる */
	move_mode		= (0);
	{	/*(フル画面=="Story" モード専用の立ち絵移動)*/
		cg.side_panel_draw_flag 	= (0);	/* パネル表示off */
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */
		kaiwa_init_obj_position();
		kaiwa_all_obj_draw_on_off(0);	/* 立ち絵を描画しない。 */
	}
	cb.main_call_func = title_menu_01_move_atumaru;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global void old_menu_system_init(void)
{
//	active_item 		= (0);
}
