
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ゲーム開始時の難易度/プレイヤー選択
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

//global int (cg_game_select_player);

static int is_turn_right;
static SDL_Surface *difficulty_select_fg0_surface;

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void ini_save(void); 	// [***090115
/******自分でコンパイルする人へ******
このままだと配布バイナリと違うので
文字列"無駄"を適当に変えといて下さい。
************************************/
/* チルド (chilled) は、冷却されること。
「冷やす」を意味する英語の動詞チル (chill) の過去分詞である。
何で chillno じゃなくて cirno なんだろな？ */

//#define YUYUKO_PASSWORD		"TABERARENAINO9"
//#define CIRNO_PASSWORD		"CIRNO9PASSWORD"
//#define REMILIA_PASSWORD		"RUMIAISMYBRIDE"

//	/*static*/ char str_pa ss_word[32/*20*/];		/* [***090222 */
global void game_clear_set_password(void)
{
#if 0
	if (0 < difficulty)/* NORMAL 以上でクリアせよ */
	{
		/* [***090222	追加 */
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) /* 既にクリアしてたら */
		{
		/*	strcpy(str_pass_word, FURAN_PASSWORD)*/;	/* ??? ＆ 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))	/* 既にクリアしてたら */
		{
			strcpy(str_pass_word, YUYUKO_PASSWORD); /* 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD)) /* 既にクリアしてたら */
		{
			strcpy(str_pass_word, CIRNO_PASSWORD);	/* チルノ ＆ レミリア 開放 */
		}
		else
		{
			strcpy(str_pass_word, REMILIA_PASSWORD);	/* レミリア 開放 */
		}
	}
#endif
	{
		ini_save();
	}
}



//player select用

#define MAX_PLAYER (8/*5*/)/*4*/

#define BASE_SP_BG_PNG		(MAX_PLAYER*0)
#define BASE_SP_ST_PNG		(MAX_PLAYER*1)
#define BASE_SP_PNG_MAX 	(MAX_PLAYER*2)

/*---------------------------------------------------------

---------------------------------------------------------*/

static /*const*/const char *player00_res[(BASE_SP_PNG_MAX)] =
{
	/*	0 REIMU */		"select/bg00.png",	//	"select/bg_re.png", 	// "select/sp_reimu_bg.png",
	/*	1 REIMU */		"select/bg01.png",	//	"select/bg_re.png", 	// "select/sp_reimu_bg.png",
	/*	2 MARISA */ 	"select/bg02.png",	//	"select/bg_ma.png", 	// "select/sp_marisa_bg.png",
	/*	3 MARISA */ 	"select/bg03.png",	//	"select/bg_ma.png", 	// "select/sp_marisa_bg.png",
	/*	4 REMILIA */	"select/bg04.png",	//	"select/bg_ma.png", 	// "select/sp_marisa_bg.png",
	/*	5 YUYUKO */ 	"select/bg05.png",	//	"select/bg_oz.png", 	// "select/sp_remiria_bg.png",
	/*	6 CIRNO */		"select/bg06.png",	//	"select/bg_ci.png", 	// "select/sp_cirno_bg.png",
	/*	7 CIRNO */		"select/bg07.png",	//	"select/bg_yu.png", 	// "select/sp_yuyuko_bg.png",
//
	/*	8 REIMU */		"select/pl00.png",	//	"select/p_re.png",		// "select/sp_reimu_st.png",
	/*	9 REIMU */		"select/pl01.png",	//	"select/p_re.png",		// "select/sp_reimu_st.png",
	/* 10 MARISA */ 	"select/pl02.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 11 MARISA */ 	"select/pl03.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 12 REMILIA */	"select/pl04.png",	//	"select/p_ma.png",		// "select/sp_marisa_st.png",
	/* 13 YUYUKO */ 	"select/pl05.png",	//	"select/p_oz.png",		// "select/sp_remiria_st.png",
	/* 14 CIRNO */		"select/pl06.png",	//	"select/p_ci.png",		// "select/sp_cirno_st.png",
	/* 15 CIRNO */		"select/pl07.png",	//	"select/p_yu.png",		// "select/sp_yuyuko_st.png",
};


/*---------------------------------------------------------
	プレイヤー選択メニュー
---------------------------------------------------------*/

static int sp_diff256;

static void draw_player_spec_message(void)
{
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	static const char *const_player_spec_str[(8)] =
	{
		"ホーミングアミュレット\\n" 	"夢想封印\\n"						"速度★★☆ 攻撃★★☆",		// No. 0 霊夢 A(霊符)
		"封魔針\\n" 					"夢想結界\\n"						"速度★★☆ 攻撃★★☆",		// No. 1 霊夢 B(夢符)
		"マジックミサイル\\n"			"スターダストレヴァリエ\\n" 		"速度★★★ 攻撃★☆☆",		// No. 2 魔理沙 A(魔符)
		"イリュージョンレーザー\\n" 	"マスタースパーク\\n"				"速度★★★ 攻撃★★☆",		// No. 3 魔理沙 B(恋符)
		"ナイトダンス\\n"				"不夜城ブレッド\\n" 				"速度★★☆ 攻撃★★☆",		// No. 4 レミリア(血符)
		"対岸の誘い\\n" 				"ギャストリドリーム\\n" 			"速度★☆☆ 攻撃★★★",		// No. 5 幽々子(符蝶)
		"アイシクルニードル\\n" 		"アイシクルストライク\\n"			"速度★★★ 攻撃★☆☆",		// No. 6 チルノ A(氷符)
		"アイシクルニードル\\n" 		"氷とか\\n" 						"速度はええ 攻撃つおい",		// No. 7 チルノ Q(⑨系)
	};/* 氷柱 == Icicle == アイシクル */
	print_kanji000((char *)const_player_spec_str[((cg_game_select_player))], /*int color_type*/7, /*int wait*/0);
}

enum
{
	RANK_SELECT_00_LOAD = 0,
	RANK_SELECT_01_MOVE_DRAW,
	RANK_SELECT_02_SELECT,
//
	PLAYER_SELECT_03_LOAD,
	PLAYER_SELECT_04_MOVE_DRAW,
	PLAYER_SELECT_05_SELECT,
};


#define SOZAI_HABA_184 (185-1)		/* 素材幅184[ドット] */
#define SOZAI_TATE_064 ( 64-1)		/* 素材幅 63[ドット] */

static u8 my_ppp_loop;
static void player_select_work(void)
{
	static SDL_Surface *player_select_bg0_surface;
	static SDL_Surface *player_select_bg2_surface;
	static SDL_Surface *player_select_fg0_surface;
	static SDL_Surface *player_select_fg2_surface;
	static int sp_scale256;
//	psp_clear_screen();

	if ((PLAYER_SELECT_03_LOAD)==my_ppp_loop)/* [load] */
	{
		(cg_game_select_player)	&= 7;
//
		player_select_bg2_surface=load_chache_bmp( (char *)player00_res[BASE_SP_BG_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		player_select_fg2_surface=load_chache_bmp( (char *)player00_res[BASE_SP_ST_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg2_surface, (SDL_SRCCOLORKEY), 0x00000000);
		if (0==is_turn_right)
				{	(cg_game_select_player)--;	}
		else	{	(cg_game_select_player)++;	}
		(cg_game_select_player) &= 0x07;
		player_select_bg0_surface=load_chache_bmp( (char *)player00_res[BASE_SP_BG_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		player_select_fg0_surface=load_chache_bmp( (char *)player00_res[BASE_SP_ST_PNG+(cg_game_select_player)]);//, 0, 0/*1*/);
		SDL_SetColorKey(player_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
//
		sp_scale256 	= (255);
		sp_diff256		= (0);
//
		draw_player_spec_message();
		my_ppp_loop++;
	}
	else
	if ((PLAYER_SELECT_04_MOVE_DRAW)==my_ppp_loop)/* [描画1] */
	{
		sp_scale256 -= 15/*18*/;
		if (sp_scale256 < 0 )
		{
			sp_scale256 = (0);
			my_ppp_loop++;/* [選択] */
		}
		SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256)/*bg_alpha_bbb*/);
		SDL_BlitSurface(player_select_bg2_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		/* プレイヤー(1&2) / 背景(1&2) / 描画 */
		{
		//	#define PLAYER_OBJ_LOCATE_X 	(250)		/* (r32) 素材[200x200] */
		//	#define PLAYER_OBJ_LOCATE_Y 	(50)		/* (r32) 素材[200x200] */
			#define PLAYER_OBJ_LOCATE_X 	(256-32)	/* (r33) 素材[256x256] */
			#define PLAYER_OBJ_LOCATE_Y 	(8) 		/* (r33) 素材[256x256] */
			SDL_Rect dr;
			dr.y = PLAYER_OBJ_LOCATE_Y;
			SDL_Surface *src;
		//	player_opt_img256(player_select_fg2_surface, (256-sp_scale256), (  is_turn_right)/*1*/);	/* 右1 左0 */
		//	player_opt_img256(player_select_fg0_surface, (	  sp_scale256), (1-is_turn_right)/*0*/);	/* 右0 左1 */
		//static void player_opt_img256() /* 0==r, 1==l, */
			{int aaa;
				sp_diff256+=(sp_scale256);
				aaa=(((sp_diff256))>>8)-16;
				if ((  is_turn_right))	{	aaa = -(aaa);}
				dr.x = PLAYER_OBJ_LOCATE_X+(aaa);
			}
			src 		= player_select_fg2_surface;
			dr.w = (src->w);
			dr.h = (src->h);
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (	  sp_scale256));
			SDL_BlitSurface(src, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
			{int aaa;
				sp_diff256+=(sp_scale256);
				aaa=(((sp_diff256))>>8)-16;
				if ((1-is_turn_right))	{	aaa = -(aaa);}
				dr.x = PLAYER_OBJ_LOCATE_X+(aaa);
			}
			src 		= player_select_fg0_surface;
			dr.w = (src->w);
			dr.h = (src->h);
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (255-sp_scale256));
			SDL_BlitSurface(src, NULL, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
		/* 難易度タグを描画 */
		{
			SDL_Rect ds;
			ds.x = (0);
			ds.y = (/*jj*/((cg_game_difficulty))<<6);
			ds.w = (SOZAI_HABA_184);	/* 素材幅191[ドット] */
			ds.h = (SOZAI_TATE_064);	/* 素材幅 63[ドット] */
		//
			SDL_Rect dr;
			SDL_Surface *src;
			src 		= difficulty_select_fg0_surface;
			dr.w = (src->w);
			dr.h = (63/*src->h*/);
			{
				dr.x = (16);//(16/*128*/);
				dr.y = (16+(128)-8);//(16)+(/*jj*/(2)<<6);
			}
		//	if ((jj==difficulty))
		//	{
		//		dr.x -= (8);
		//		dr.y -= (8);
		//	}
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, ((/*(jj==difficulty)?(255):*/(127))));
			SDL_BlitSurface(src, &ds, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
	}
	else
	if ((PLAYER_SELECT_05_SELECT)==my_ppp_loop)/* [選択] */
	{
		cg.msg_time = byou60(5);/* 必要 */	/* 約 5 秒 */
		if (0==cg_my_pad_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
		{
			if (cg_my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* 左か右かショットかキャンセルのいづれか */
			{	/* 状態が変わる場合で*/
				/* 状態が変わる場合、ちゃんと SDLサーフェイスを開放 しないとメモリーリークするよ */
				unloadbmp_by_surface(player_select_bg0_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_bg0_surface = NULL;
				unloadbmp_by_surface(player_select_fg0_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_fg0_surface = NULL;
				unloadbmp_by_surface(player_select_bg2_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_bg2_surface = NULL;
				unloadbmp_by_surface(player_select_fg2_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_fg2_surface = NULL;
			//
				if (cg_my_pad & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*左か右の両方*/  ) /* 左右ボタン入力 */
				{
					is_turn_right = (cg_my_pad & PSP_KEY_RIGHT)?1:0;/* 右なら1, 左なら0 */
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
					my_ppp_loop = (PLAYER_SELECT_03_LOAD);	/* [load] */
				}
				if (cg_my_pad & PSP_KEY_SHOT_OK)
				{
					cg.msg_time = (0);/* 必要 */
					{
						/* 状態が変わる場合、ちゃんと SDLサーフェイスを開放 しないとメモリーリークするよ */
						unloadbmp_by_surface(difficulty_select_fg0_surface);	//キャッシュに入ってるのでNULLに出来ない。difficulty_select_fg0_surface = NULL;
					}
					voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* テキトー */
					main_call_func = stage_first_init;
				}
				else
				if (cg_my_pad & PSP_KEY_BOMB_CANCEL)
				{
					cg.msg_time = (0);/* 必要 */
					voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
					/* 難易度選択メニューに戻る */
					main_call_func = difficulty_select_menu_start;	/* 難易度選択メニューへ */
				}
			}
		}
	}
}


/*---------------------------------------------------------
	キャンセル音を鳴らして、タイトルメニューへ移動。
	共通サブ。(外部のあちこちから呼ばれる)
---------------------------------------------------------*/

global void menu_cancel_and_voice(void)
{
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
	main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
}

/*---------------------------------------------------------
	難易度選択メニュー(モードを選択してね)
	難易度を選択してね
---------------------------------------------------------*/

static void draw_difficulty_spec_message(void)
{
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	static const char *const_difficulty_spec_str[(4)] =
	{
	//	"Easy\\n"		"簡単に言えば、ボムゲです。\\n" 		"　　　　　　　　　(ちゃんと全６面)",	// No. 0 easy
	//	"Normal\\n" 	"普通に言えば、難しいです。\\n" 		"　　　　　　　　　　　　　(全６面)",	// No. 1 normal
	//	"Hard\\n"		"お硬く言えば、厳しいです。\\n" 		"　　　　　　　　　　　　　(全６面)",	// No. 2 hard
	//	"Lunatic\\n"	"月並みに言えば、激しいです。\\n"		"　　　　　　　　　　　　　(全６面)",	// No. 3 lunatic
		"Easy\\n"		"簡単には攻略できない。\\n" 			"　　　　　　　　　(おおよそ全６面)",	// No. 0 easy
		"Normal\\n" 	"普通の人にはお勧めできない。\\n"		"　　　　　　　　　　　　　(全６面)",	// No. 1 normal
		"Hard\\n"		"硬い。\\n" 							"　　　　　　　　　　　　　(全６面)",	// No. 2 hard
		"Lunatic\\n"	"月並みとは限らない。\\n"				"　　　　　　　　　　　　　(全６面)",	// No. 3 lunatic
	};
	print_kanji000((char *)const_difficulty_spec_str[((cg_game_difficulty))], /*int color_type*/7, /*int wait*/0);
}

static void difficulty_select_menu_local_work(void)
{
	static int sp_scale256;
	if ((RANK_SELECT_00_LOAD)==my_ppp_loop) 	/* [load] */
	{
		(cg_game_difficulty) &= 0x03;
//
		sp_scale256 	= (255);
		sp_diff256		= (0);
//
		draw_difficulty_spec_message();
		my_ppp_loop++;
	}
	else
	if ((RANK_SELECT_01_MOVE_DRAW)==my_ppp_loop)/* [描画1] */
	{
		sp_scale256 -= 15/*18*/;
		if (sp_scale256 < 0 )
		{
			sp_scale256 = (0);
			my_ppp_loop++;/* [選択] */
		}
//		SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256)/*bg_alpha_bbb*/);
//		SDL_BlitSurface(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
		psp_pop_screen();
		int jj;
		for (jj=0; jj<4; jj++)
		{
			SDL_Rect ds;
			ds.x = (0);
			ds.y = (jj<<6);
			ds.w = (SOZAI_HABA_184);	/* 素材幅184[ドット] */
			ds.h = (SOZAI_TATE_064);	/* 素材幅 63[ドット] */
		//
			SDL_Rect dr;
			SDL_Surface *src;
			src 		= difficulty_select_fg0_surface;
			dr.w = (src->w);
			dr.h = (63/*src->h*/);
			{
				dr.x = (128);
				dr.y = (16)+(jj<<6);
			}
			if ((jj==(cg_game_difficulty)))
			{
				dr.x -= (8);
				dr.y -= (8);
			}
		//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
			SDL_SetAlpha(src, SDL_SRCALPHA, (((jj==(cg_game_difficulty))?(255):(127))));
			SDL_BlitSurface(src, &ds, sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		}
	}
	else
	if ((RANK_SELECT_02_SELECT)==my_ppp_loop)/* [選択] */
	{
		cg.msg_time = byou60(5);/* 必要 */	/* 約 5 秒 */
		if (0==cg_my_pad_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
		{
			if (cg_my_pad & (PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* 左か右かショットかキャンセルのいづれか */
			{	/* 状態が変わる場合で*/
			//
				if (cg_my_pad & (PSP_KEY_UP|PSP_KEY_DOWN)/*上か下の両方*/ ) /* 上下ボタン入力 */
				{
					if (cg_my_pad & PSP_KEY_UP)
							{	(cg_game_difficulty)--;	}
					else	{	(cg_game_difficulty)++;	}
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
					my_ppp_loop = (RANK_SELECT_00_LOAD);	/* [load] */
				}
				if (cg_my_pad & PSP_KEY_SHOT_OK)
				{
					cg.msg_time = (0);/* 必要 */
					voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* テキトー */
					/* 通常／プラクティス、ゲーム開始 */
					// static void player_opt_init(void)吸収。なし
					{
						is_turn_right = 0;/* 必要 */
						(cg_game_select_player)++;/* プログラムplayer_select_work()の都合上調整 */	/*player_select_work()::[init]*/
					}
					my_ppp_loop++;/* = (PLAYER_SELECT_03_LOAD) */
					main_call_func = player_select_work;
				}
				else
				if (cg_my_pad & PSP_KEY_BOMB_CANCEL)
				{
					cg.msg_time = (0);/* 必要 */
					{
						/* 状態が変わる場合、ちゃんと SDLサーフェイスを開放 しないとメモリーリークするよ */
						unloadbmp_by_surface(difficulty_select_fg0_surface);	//キャッシュに入ってるのでNULLに出来ない。difficulty_select_fg0_surface = NULL;
					}
					menu_cancel_and_voice();	/* タイトルメニューへ移動 */
				}
			}
		}
	}
}

global void difficulty_select_menu_start(void)
{
//	psp_clear_screen();
	/* [init] */
	if (0==cg_my_pad_alter)/* さっき何も押されてなかった場合 */
	{
		psp_push_screen();
		/* [load] */
		{
			difficulty_select_fg0_surface = load_chache_bmp( (char *)"teki/teki00_256.png");//, 0, 0/*1*/);
			SDL_SetColorKey(difficulty_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
		}
		my_ppp_loop = (RANK_SELECT_00_LOAD);
		main_call_func = difficulty_select_menu_local_work;
	}
}
