
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ゲーム開始時の難易度/プレイヤー選択
	-------------------------------------------------------
	[難易度選択メニュー]
	[プレイヤー選択メニュー]
---------------------------------------------------------*/

#include "kanji_system.h"
#include "111_my_file.h"/*(my_file_common_name)*/

/*---------------------------------------------------------
	player select用
---------------------------------------------------------*/

#define BASE_SP_BG_PNG		(MAX_08_PLAYER*0)
#define BASE_SP_ST_PNG		(MAX_08_PLAYER*1)
#define BASE_SP_PNG_MAX 	(MAX_08_PLAYER*2)

/*---------------------------------------------------------
	必ずどっちか。
---------------------------------------------------------*/
enum
{
	spec_message_00_SELECT_PLAYER = 0,
	spec_message_01_SELECT_RANK,
};
//		 if (spec_message_00_SELECT_PLAYER==set_mode)	{	... 	}
//	else if (spec_message_01_SELECT_RANK==set_mode) 	{	... 	}

/*---------------------------------------------------------
	状態をメッセージ表示。
	-------------------------------------------------------
	[難易度選択メニュー]
	[プレイヤー選択メニュー]
---------------------------------------------------------*/

static void draw_spec_message(unsigned int set_mode)
{
	kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
	static const char *const_player_spec_str[(8)/*(8人)*/+(4)/*(4難易度)*/][(2)/*(白文字+黄文字)*/] =
	{	/*"針巫女の攻撃力の解説は、おかしい気がするけど貼りつかなければ弱い。"*/
		{	"楽園の素敵な巫女\\n"	"博麗 霊夢 (霊符)\\n"		"ホーミングアミュレット\\n" "夢想封印\\n",					"移動速度★★☆\\n" 				"攻撃範囲★★★\\n" 	"攻撃力　★☆☆",	},// No. 0 霊夢 A(霊符)
		{	"楽園の素敵な巫女\\n"	"博麗 霊夢 (夢符)\\n"		"封魔針\\n" 				"夢想結界\\n",					"移動速度★★☆\\n" 				"攻撃範囲★★☆\\n" 	"攻撃力　★★☆",	},// No. 1 霊夢 B(夢符)
		{	"普通の黒魔術師\\n" 	"霧雨 魔理沙 (魔符)\\n" 	"マジックミサイル\\n"		"スターダストレヴァリエ\\n",	"移動速度★★★\\n" 				"攻撃範囲★★★\\n" 	"攻撃力　★☆☆",	},// No. 2 魔理沙 A(魔符)
		{	"古風な魔法使い\\n" 	"霧雨 魔理沙 (恋符)\\n" 	"イリュージョンレーザー\\n" "マスタースパーク\\n",			"移動速度★★★\\n" 				"攻撃範囲★☆☆\\n" 	"攻撃力　★★★",	},// No. 3 魔理沙 B(恋符)
		{	"吸血鬼\\n" 			"レミリア スカーレット\\n"	"ナイトダンス\\n"			"不夜城ブレッド\\n",			"移動速度★★☆\\n" 				"攻撃範囲★★☆\\n" 	"攻撃力　★★☆",	},// No. 4 レミリア(血符)
		{	"華胥の亡霊\\n" 		"西行寺 幽々子\\n"			"対岸の誘い\\n" 			"ギャストリドリーム\\n",		"移動速度☆☆☆\\n" 				"攻撃範囲★★☆\\n" 	"攻撃力　★★★",	},// No. 5 幽々子(符蝶)
		{	"湖上の氷精\\n" 		"チルノ (氷符)\\n"			"アイシクルニードル\\n" 	"アイシクルストライク\\n",		"移動速度★★★★\\n"				"攻撃範囲？？？\\n" 	"攻撃力　★★☆",	},// No. 6 チルノ A(氷符)
		{	"湖上の氷精\\n" 		"チルノ (⑨符)\\n"			"アイシクルニードル\\n" 	"ガッチガチ\\n",				"移動速度★☆★☆★☆★☆★\\n" 	"攻撃範囲？？？\\n" 	"攻撃力　⑨⑨⑨",	},// No. 7 チルノ Q(⑨系)
		{	"Easy\\n"				"簡単かどうかは人による。\\n",															"エンディング無し\\n"	"(全５面)", },// No. 0 easy
		{	"Normal\\n" 			"普通な人にはハード。\\n",																"(全６面)", },// No. 1 normal
		{	"Hard\\n"				"ハードな人には普通。\\n",																"(全６面)", },// No. 2 hard
		{	"Lunatic\\n"			"つきが必要。\\n",																		"(全６面)", },// No. 3 lunatic
		// 1節目(白)																										// 2節目(黄色)
	};/* 氷柱 == Icicle == アイシクル */
	//
	const unsigned int aaa = (spec_message_00_SELECT_PLAYER==set_mode)?((unsigned)cg_game_select_player):((8)+((unsigned)cg.game_difficulty));
	// 文字を決める。													// 色を決める。 				// 描画する。
	strcpy(my_font_text, (char *)const_player_spec_str[((aaa))][0]);	kanji_color(7);/*(白色)*/		kanji_draw();
	strcpy(my_font_text, (char *)const_player_spec_str[((aaa))][1]);	kanji_color(14);/*(黄色)*/		kanji_draw();
	strcpy(my_font_text, (spec_message_00_SELECT_PLAYER==set_mode)?("\\n誰が行くの？"):("\\n\\n\\n\\n\\nモードを選択してね")	 );
																		kanji_color(9);/*(青/白色)*/	kanji_draw();
}
	//	"Easy\\n"		"簡単に言えば、ボムゲです。\\n" 		"(ちゃんと全６面)", // No. 0 easy
	//	"Normal\\n" 	"普通に言えば、難しいです。\\n" 		"　　　　(全６面)", // No. 1 normal
	//	"Hard\\n"		"お硬く言えば、厳しいです。\\n" 		"　　　　(全６面)", // No. 2 hard
	//	"Lunatic\\n"	"月並みに言えば、激しいです。\\n"		"　　　　(全６面)", // No. 3 lunatic
	//#if (1==US E_EASY_BADEND)
	//#else
	//	"Easy\\n"		"簡単には攻略できない。\\n" 			"　　　　(全６面)", // No. 0 easy
	//#endif

#define SOZAI_HABA_184 (185-1)		/* 素材幅184[ドット] */
#define SOZAI_TATE_064 ( 64-1)		/* 素材幅 63[ドット] */

/*---------------------------------------------------------
	ローカル使用変数。
---------------------------------------------------------*/
static SDL_Surface *player_select_bg0_surface;
static SDL_Surface *player_select_bg2_surface;
static SDL_Surface *player_select_fg0_surface;
static SDL_Surface *player_select_fg2_surface;

static int sp_scale256;
static int sp_diff256;
static int is_turn_right;
static SDL_Surface *difficulty_select_fg0_surface;




/*---------------------------------------------------------
	漢字表示移動処理
---------------------------------------------------------*/
static int kanji_set_x_position;
static int kanji_move_x_position;
static void move_kanji(void)
{
	const int sabun = (kanji_move_x_position-kanji_set_x_position);
	if (0 != (sabun))
	{
		const int value = (32 - __builtin_allegrex_clz(sabun - 1));
		if ( 0 > (sabun) )
		{
			kanji_move_x_position += value;
		}
		else
		{
			kanji_move_x_position -= value;
		}
	}
	unsigned int ii;
	for (ii=0; ii<7; ii++)
	{
		ml_font[ii].x		= (kanji_move_x_position);/* X位置 */
	}
}

/*---------------------------------------------------------
	[プレイヤー選択メニュー]
---------------------------------------------------------*/
static void player_select_03_load(void);/*(宣言が必要)*/
static void player_select_05_select(void)/* [選択] */
{
	cg.msg_time = byou60(5);/* 必要 */	/* 約 5 秒 */
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* 左か右かショットかキャンセルのいづれか */
		{	/* 状態が変わる場合で*/
			/* 状態が変わる場合、ちゃんと SDLサーフェイスを開放 しないとメモリーリークするよ */
			unloadbmp_by_surface(player_select_bg0_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_bg0_surface = NULL;
			unloadbmp_by_surface(player_select_fg0_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_fg0_surface = NULL;
			unloadbmp_by_surface(player_select_bg2_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_bg2_surface = NULL;
			unloadbmp_by_surface(player_select_fg2_surface);	//キャッシュに入ってるのでNULLに出来ない。player_select_fg2_surface = NULL;
		//
			if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT)/*左か右の両方*/  ) /* 左右ボタン入力 */
			{
				is_turn_right = (psp_pad.pad_data & PSP_KEY_RIGHT)?1:0;/* 右なら1, 左なら0 */
				voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				cb.main_call_func = player_select_03_load;	/* [load] */	/*(戻る)*/
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				cg.msg_time = (0);/* 必要 */
				{
					/* 状態が変わる場合、ちゃんと SDLサーフェイスを開放 しないとメモリーリークするよ */
					unloadbmp_by_surface(difficulty_select_fg0_surface);	//キャッシュに入ってるのでNULLに出来ない。difficulty_select_fg0_surface = NULL;
				}
				voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* テキトー */
				if (0!=cg.game_practice_mode)
				{/* プラクティスモードの場合 */
					cb.main_call_func = stage_select_menu_start;	/* ステージ選択メニューの開始 */
				}
				else
				{
					cb.main_call_func = stage_first_init;			/* ゲーム開始 */
				}
			}
			else
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
			{
				cg.msg_time = (0);/* 必要 */
				voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
				/* 難易度選択メニューに戻る */
				cb.main_call_func = difficulty_select_menu_start;	/* 難易度選択メニューへ(戻る) */
			}
		}
	}
	move_kanji();
}

/*---------------------------------------------------------
	[プレイヤー選択メニュー]
---------------------------------------------------------*/
static void player_select_04_move_draw(void)/* [描画1] */
{
	sp_scale256 -= (15);/*18*/
	if (sp_scale256 < 0 )
	{
		sp_scale256 = (0);
		cb.main_call_func = player_select_05_select;/* [選択] */
	}
	SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256) );
	PSPL_UpperBlit(player_select_bg2_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	PSPL_UpperBlit(player_select_bg0_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
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
			sp_diff256 += (sp_scale256);
			aaa = (((sp_diff256))>>8)-16;
			if ((  is_turn_right))	{	aaa = -(aaa);}
			dr.x = PLAYER_OBJ_LOCATE_X + (aaa);
		}
		src 		= player_select_fg2_surface;
		dr.w = (src->w);
		dr.h = (src->h);
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, (	  sp_scale256));
		PSPL_UpperBlit(src, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
		{int aaa;
			sp_diff256 += (sp_scale256);
			aaa=(((sp_diff256))>>8)-16;
			if ((1-is_turn_right))	{	aaa = -(aaa);}
			dr.x = PLAYER_OBJ_LOCATE_X+(aaa);
		}
		src 		= player_select_fg0_surface;
		dr.w = (src->w);
		dr.h = (src->h);
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, (255-sp_scale256));
		PSPL_UpperBlit(src, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
	}
	/* 難易度タグを描画 */
	{
		SDL_Rect ds;
		ds.x = (0);
		ds.y = (/*jj*/((cg.game_difficulty))<<6);
		ds.w = (SOZAI_HABA_184);	/* 素材幅191[ドット] */
		ds.h = (SOZAI_TATE_064);	/* 素材幅 63[ドット] */
	//
		SDL_Rect dr;
		SDL_Surface *src;
		src 		= difficulty_select_fg0_surface;
		dr.w = (src->w);
		dr.h = (63/*src->h*/);
		{	/*(表示位置)*/
			dr.x = (16);//(16/*128*/);
			dr.y = (16+(128)-8+(64));//(16)+(/*jj*/(2)<<6);
		}
	//	if ((jj==difficulty))
	//	{
	//		dr.x -= (8);
	//		dr.y -= (8);
	//	}
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, ((/*(jj==difficulty)?(255):*/(127))));
		PSPL_UpperBlit(src, &ds, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
	}
	move_kanji();
}
/*---------------------------------------------------------
	選択プレイヤー画像と背景の名前を決める。
---------------------------------------------------------*/

static void set_file_name(unsigned char is_pl)
{
	strcpy(my_file_common_name, (char *)DIRECTRY_NAME_DATA_STR "/select/" "pl00.png");
	my_file_common_name[DIRECTRY_NAME_DATA_LENGTH+(10)] = '0'+(cg_game_select_player);
	my_file_common_name[DIRECTRY_NAME_DATA_LENGTH+(11)] = '0'+(is_pl);
}
/*---------------------------------------------------------
	[プレイヤー選択メニュー]選択プレイヤー画像と背景をロード。
---------------------------------------------------------*/

static void player_select_03_load(void)/* [load] */
{
	(cg_game_select_player) &= (0x07);
	//
	set_file_name(0);	player_select_fg2_surface = load_chache_bmp();
	set_file_name(1);	player_select_bg2_surface = load_chache_bmp();
	SDL_SetColorKey(player_select_fg2_surface, (SDL_SRCCOLORKEY), 0x00000000);
	//
	if (0==is_turn_right)
			{	(cg_game_select_player)--;	}
	else	{	(cg_game_select_player)++;	}
	(cg_game_select_player) &= 0x07;
	//
	set_file_name(0);	player_select_fg0_surface = load_chache_bmp();
	set_file_name(1);	player_select_bg0_surface = load_chache_bmp();
	SDL_SetColorKey(player_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
	//
	sp_scale256 	= (255);
	sp_diff256		= (0);
	//
	kanji_set_x_position = (24);/*(漢字移動先座標設定)*/
	draw_spec_message(spec_message_00_SELECT_PLAYER);	// [プレイヤー選択]状態をメッセージ表示。
	cb.main_call_func = player_select_04_move_draw;
}


/*---------------------------------------------------------
	キャンセル音を鳴らして、タイトルメニューへ移動。
	共通サブ。(外部のあちこちから呼ばれる)
---------------------------------------------------------*/

global void menu_cancel_and_voice(void)
{
	voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
	cb.main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
}


/*---------------------------------------------------------
	[難易度選択メニュー]
---------------------------------------------------------*/
static void rank_select_00_initialize_value(void);/*(宣言が必要)*/
static void rank_select_02_select(void)/* [選択] */
{
	cg.msg_time = byou60(5);/* 必要 */	/* 約 5 秒 */
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL)) /* 左か右かショットかキャンセルのいづれか */
		{	/* 状態が変わる場合で*/
		//
			if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN)/*上か下の両方*/ ) /* 上下ボタン入力 */
			{
				if (psp_pad.pad_data & PSP_KEY_UP)
						{	cg.game_difficulty--;	cg.game_difficulty = psp_max(cg.game_difficulty, (0)   );	}	/* メニュー上で停止 */
				else	{	cg.game_difficulty++;	cg.game_difficulty = psp_min(cg.game_difficulty, (4-1) );	}	/* メニュー下で停止 */
				voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				cb.main_call_func = rank_select_00_initialize_value;	/* [load] */	/*(戻る)*/
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				cg.msg_time = (0);/* 必要 */
				voice_play(VOICE01_MENU_OK, TRACK01_EXPLODE);/* テキトー */
				/* 通常／プラクティス、ゲーム開始 */
				// static void player_opt_init(void)吸収。なし
				{
					is_turn_right = 0;/* 必要 */
					(cg_game_select_player)++;/* プログラムplayer_select_work()の都合上調整 */	/*player_select_work()::[init]*/
				}
				cb.main_call_func = player_select_03_load;
			}
			else
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL)
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
	move_kanji();
}

/*---------------------------------------------------------
	[難易度選択メニュー]
---------------------------------------------------------*/
static void rank_select_01_move_draw(void)/* [描画1] */
{
	sp_scale256 -= 15/*18*/;
	if (sp_scale256 < 0 )
	{
		sp_scale256 = (0);
		cb.main_call_func = rank_select_02_select;/* [選択] */
	}
//	SDL_SetAlpha(player_select_bg0_surface, SDL_SRCALPHA, (255-sp_scale256) );
//	PSPL_UpperBlit(player_select_bg0_surface,NULL,sdl_screen[SDL_00_VIEW_SCREEN],NULL);
	psp_pop_screen();
	u32 jj;
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
		if ((jj==((unsigned)cg.game_difficulty)))
		{
			dr.x -= (8);
			dr.y -= (8);
		}
	//	SDL_SetColorKey(src, (SDL_SRCCOLORKEY), 0x00000000);
		SDL_SetAlpha(src, SDL_SRCALPHA, (((jj==((unsigned)cg.game_difficulty))?(255):(127))));
		PSPL_UpperBlit(src, &ds, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dr);
	}
	move_kanji();/*(現状意味無いっぽい)*/
}

/*---------------------------------------------------------
	[難易度選択メニュー]変数等を開始状態へ初期化設定。
---------------------------------------------------------*/
static void rank_select_00_initialize_value(void)
{
	(cg.game_difficulty) &= (0x03);
	//
	sp_scale256 	= (255);
	sp_diff256		= (0);
	//
	kanji_set_x_position = (256+32);/*(漢字移動先座標設定)*/
	draw_spec_message(spec_message_01_SELECT_RANK); 	// [難易度選択]状態をメッセージ表示。
	cb.main_call_func = rank_select_01_move_draw;
}

/*---------------------------------------------------------
	[難易度選択メニュー]の開始。
---------------------------------------------------------*/
global void difficulty_select_menu_start(void)
{
//	psp_clear_screen();
	/* [init] */
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合 */
	{
		psp_push_screen();
		/* [load] */
		{
			strcpy(my_file_common_name, (char *)DIRECTRY_NAME_DATA_STR "/teki/" "teki00_256.png" );
			difficulty_select_fg0_surface = load_chache_bmp();
			SDL_SetColorKey(difficulty_select_fg0_surface, (SDL_SRCCOLORKEY), 0x00000000);
		}
		{
			#if (1)/*(漢字関連の初期化)*/
			set_kanji_origin_xy(24, 64);/*(表示原点の設定)*/
			set_kanji_origin_kankaku(19);/*(字間を少し広げる)*/
			set_kanji_hide_line(ML_LINE_99_MAX);/*(全行表示する。)*/

			kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
			cg.msg_time = (65536);	/* 約 18 分 */
			#endif
		{
			/* 8行目のみ特殊な位置へ移動。 */
			ml_font[ML_LINE_08].x		= (16);/* X位置 */
			ml_font[ML_LINE_08].y		= (16);/* Y位置 */
		}
	//		kanji_window_clear_line(5); /* 漢字ウィンドウの1行目(==0)の内容を消す。 */
	//		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (5) ); /* カーソルを1行目(==0)へ移動 */
	//		//
	//		kanji_color((9));
	//		strcpy(my_font_text, "モードを選択してね"	 );
	//		kanji_draw();
		}
		cb.main_call_func = rank_select_00_initialize_value;
	}
}
