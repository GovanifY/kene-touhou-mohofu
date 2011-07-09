
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ミュージックルーム 幻想音樂室
---------------------------------------------------------*/

#include "kanji_system.h"

enum
{
	MOJI_00 = 0,MOJI_01,	MOJI_02,	MOJI_03,
	MOJI_04,	MOJI_05,	MOJI_06,	MOJI_07,
	MOJI_08,	MOJI_09,	MOJI_10,	MOJI_11,
	MOJI_12,	MOJI_13,	MOJI_14,	MOJI_15,
	MOJI_16,	MOJI_17,	MOJI_18,	MOJI_19,
	MOJI_20,	MOJI_21,	MOJI_22,	MOJI_23,
	MOJI_24,	MOJI_25,
	MOJI_23_MAX 	/* 最大数 */
};

enum
{
	MUSIC_ROOM_00_BGM = 0,
	MUSIC_ROOM_01_SOUND,
	MUSIC_ROOM_02_QUIT,
	MUSIC_ROOM_03_MAX,
};

#define X_LOCATE_OFFSET_02	(64)
#define X_LOCATE_OFFSET_04	(224)
#define Y_LOCATE_OFFSET 	(128)/*(100)*/
#define MAX_KEY_NAMES_21	(MOJI_23_MAX+MUSIC_ROOM_03_MAX)
/*
だめ文字(シフトJISの2バイト目が0x5c、すなわち'\'('＼')である文字のこと。)
	ソ噂浬欺圭構蚕十申曾箪貼能表暴予禄兔喀媾彌拿杤歃濬畚秉綵臀藹觸軆鐔饅鷭
*/
static void music_room_draw_message(int cursor1, int cursor2)
{
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	static const char *const_kaisetu_str[(2)][(MOJI_23_MAX)] =
	{
		{	/*"BGM 解説"*/
"曲を止めます", 								/* */
"No.1 春の湊に\\n"								"1面のテーマです。",
"No.2 バッド・アップル ～ Bad Apple!!\\n"		"2面のテーマです。",
"No.3 ヴォヤージュ1969\\n"						"3面のテーマです。",
"No.4 フォールオブフォール ～ 秋めく滝\\n"		"4面のテーマです。",
"No.5 ヴアル魔法図書館\\n"						"5面のテーマです。",
"No.6 メイドと血の懐中時計\\n"					"6面のテーマです。",
"No.7 童祭 ～ Innocent Treasures\\n"			"スタッフロールのテーマです。", 			// 夢違科学世紀「童祭 Innocent Treasures」
"No.8 ルーネイトエルフ\\n"						"エキストラステージのテーマです。",
"No.9 遠野幻想物語\\n"							"ファンタズムステージのテーマです。",
"No.10 上海紅茶館  ～ Chinese Tea\\n"			"エンディング2のテーマです。",
"No.11 人形裁判 ～ 人の形弄びし少女\\n" 		"アリスのテーマです。",
"No.12 コンプリート・ダークネス\\n" 			"魅魔のテーマです。",						// Complete Darkness
"No.13 竹取飛翔 ～ Lunatic Princess\\n" 		"蓬莱山 輝夜のテーマです。",
"No.14 風神少女\\n" 							"射命丸 文のテーマです。",
"No.15 ラクトガール ～ 少女密室\\n" 			"パチュリー・ノーレッジのテーマです。",
"No.16 月時計 ～ ルナ・ダイアル\\n" 			"十\六夜 咲夜のテーマです。",
"No.17 おてんば恋娘\\n" 						"チルノのテーマです。",
"No.18 グリモア オブ アリス\\n" 				"アリスのテーマです。", 					// Grimoire of Alice
"No.19 恋色マスタースパーク\\n" 				"魔理沙のテーマです。",
"No.20 広有射怪鳥事 ～ Till When?\\n"			"妖夢のテーマです。",
"No.21 亡き王女の為のセプテット\\n" 			"レミリア・スカーレットのテーマです。",
"No.22 U.N.オーエンは彼女なのか？\\n"			"フランドール・スカーレットのテーマです。",
"No.23 プレイヤーズスコア\\n"					"エンディングのテーマです。",
"No.24 無何有の郷 ～ Deep Mountain\\n"			"キーコンフィグのテーマです。",
"No.25 紅より儚い永遠\\n"						"タイトル画面のテーマです。",
		},
		{	/*"SOUND 解説"*/
"ショット", 		"メニュー決定", 	"メニュー選択", 	"ボス退場",
"ピチューン",		"アイテム取得", 	"増えた",			"自分スペカ",
"ザコ倒した",		"グレイズ", 		"時間制限", 		"キラリン☆",
"ボスショット1",	"ボスショット2",	"ボスショット3",	"ボスショット4",
"ボスショット5",	"ボス溜め1",		"18音", 			"19音",
"20音", 			"21音", 			"22音", 			"23音",
"24音", 			"25音",
		},
	};
	print_kanji000((char *)const_kaisetu_str[cursor1][cursor2], /*int color_type*/(7), /*int wait*/(0) );
}
#if 0
global void music_game_draw_message(int cursor1)
{
	static const char *const_music_title_str[(MOJI_23_MAX)] =
	{	/*	最大半角で31文字。 */
	/*	"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
		" 　　　　　　　　　　　　　　　", // No. 0 "曲を止めます"
		" 　　　　　　　　　　♪春の湊に", // No. 1 "1面のテーマです。"
		"♪永夜の報い Imperishable Night", // No. 2 "2面のテーマです。" 				// ロマチルは止めて入れ替えると思う。
		" 　　　　　　♪ヴォヤージュ1969", // No. 3 "3面のテーマです。"
		" 　 ♪Fall of fall　～ 秋めく滝", // No. 4 "4面のテーマです。"
		" 　　　　　　♪ヴアル魔法図書館", // No. 5 "5面のテーマです。"
		" 　　♪亡き王女の為のセプテット", // No. 6 "6面のテーマです。"
		" 　　　　　　　　　　　　♪童祭", // No. 7 "スタッフロールのテーマです。"
		" 　　　　　　♪ルーネイトエルフ", // No. 8 "エキストラステージのテーマです。"
		" 　　　　　　　　♪遠野幻想物語", // No. 9 "ファンタズムステージのテーマです。"
		" 　♪上海紅茶館  ～ Chinese Tea", // No.10 "エンディング2のテーマです。"
		"♪人形裁判　～ 人の形弄びし少女", // No.11 "アリスのテーマです。"
		" 　　　　　 ♪Complete Darkness", // No.12 "魅魔のテーマです。"
		"♪竹取飛翔　～ Lunatic Princess", // No.13 "蓬莱山 輝夜のテーマです。"
		" 　 ♪風神少女　～ Full Version", // No.14 "射命丸 文のテーマです。"
		" 　 ♪ラクトガール　～ 少女密室", // No.15 "パチュリー・ノーレッジのテーマです。"
		" 　 ♪月時計　～ ルナ・ダイアル", // No.16 "十\六夜 咲夜のテーマです。"
		" 　　　　　　　　♪おてんば恋娘", // No.17 "チルノのテーマです。"
		" 　　　　　 ♪Grimoire of Alice", // No.18 "アリスのテーマです。"
		" 　　　　♪恋色マスタースパーク", // No.19 "魔理沙のテーマです。"
		" 　♪広有射怪鳥事 ～ Till When?", // No.20 "妖夢のテーマです。"				// 妖夢どうしよ。
		" 　　　　　♪プレイヤーズスコア", // No.20 "エンディングのテーマです。"		// これも入れ替えると思う。
		" ♪無何有の郷　～ Deep Mountain", // No.21 "キーコンフィグのテーマです。"
		" 　　　　　　　♪紅より儚い永遠", // No.22 "タイトル画面のテーマです。"
	};
	print_kanji000((char *)const_music_title_str[cursor1], /*int color_type*/(7), /*int wait*/(0) );
}
#endif

static int music_room_setting[(2)+(2)];/*MUSIC_ROOM_03_MAX*/
static SDL_Surface *music_room_name_surface[MAX_KEY_NAMES_21];	/* 文字の表示用画像 */
static int menu_cursor1;
static int bg_alpha_aaa;

/*---------------------------------------------------------
	幻想音樂室 フェードアウトしておしまい中
---------------------------------------------------------*/

static void music_room_state_03_fade_out(void)
{
	bg_alpha_aaa += (2);	/*(1) (8)*/ 	/*fps_factor*/
	if ((224) < bg_alpha_aaa)/*(250-8)*/
	{
		bg_alpha_aaa = (255);
		#if 1/* 文字の表示用画像を開放する */
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_21); i++)
			{
				/* ここでハングアップ */
				SDL_FreeSurface(music_room_name_surface[i]);/*xxx_ok?*/
			}
		}
		#endif
		main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
	}
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------
	幻想音樂室 メニュー選択して、再生したり、描画したり中
---------------------------------------------------------*/

static void music_room_state_02_select_menu(void)
{
	psp_pop_screen();
	cg.msg_time = byou60(5);	/* 約 5 秒 */
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (psp_pad.pad_data & PSP_KEY_UP)			/* 上ボタン入力 */
		{
			if (MUSIC_ROOM_00_BGM == menu_cursor1)		{	menu_cursor1 = (MUSIC_ROOM_03_MAX-1);	}
			else										{	menu_cursor1--; 	}
		}
		else
		if (psp_pad.pad_data & PSP_KEY_DOWN)			/* 下ボタン入力 */
		{
			if ((MUSIC_ROOM_03_MAX-1) == menu_cursor1)	{	menu_cursor1 = MUSIC_ROOM_00_BGM;	}
			else										{	menu_cursor1++; 	}
		}
		else	/* 上下以外の通常処理(斜め入力を排除する為、上下の入力があった場合はそれ以外の処理をしない) */
		{
			if (MUSIC_ROOM_02_QUIT == menu_cursor1) /* 項目[ QUIT ] を選んでいる場合 */
			{
				if (psp_pad.pad_data & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL))	/* ショット || キャンセルボタン入力 */
				{
					#if (1)
					voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
				//	voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*テキトー*/
					#endif
					play_music_num(BGM_25_menu01);
					load_SDL_bg(BG_TYPE_00_title_bg);
					cg.msg_time = (0);	/* 約 0 秒 */
//					bg_alpha_aaa		= 255;
					bg_alpha_aaa		= 0;
					main_call_func = music_room_state_03_fade_out;
				}
			}
			else	/* 設定項目を変更する場合 */
			if (psp_pad.pad_data & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		/* 左右ボタン入力 */
			{
				if (psp_pad.pad_data & PSP_KEY_LEFT)			/* 左ボタン入力 */
				{
					music_room_setting[menu_cursor1]--;
					if ((0/*MOJI_00*/) > music_room_setting[menu_cursor1])	{	music_room_setting[menu_cursor1] = music_room_setting[menu_cursor1+(2)]; }
					music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
				}
				else
				if (psp_pad.pad_data & PSP_KEY_RIGHT)		/* 右ボタン入力 */
				{
					music_room_setting[menu_cursor1]++;
					if (music_room_setting[menu_cursor1+(2)] < music_room_setting[menu_cursor1])	{	music_room_setting[menu_cursor1] = 0/*MOJI_00*/; }
					music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
				}
			}
			else	/* 設定項目を変更する場合 */
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK) /* ショットボタン入力 */
			{
				if (MUSIC_ROOM_00_BGM == menu_cursor1)	/* 項目[ BGM ] を選んでいる場合 */
				{
					play_music_num(music_room_setting[menu_cursor1]/*BGM_05_stage5*/);
				}
				else
//				if (MUSIC_ROOM_01_SOUND == menu_cursor1)	/* 項目[ SOUND ] を選んでいる場合 */
				{
					#if (1)
					voice_play(music_room_setting[menu_cursor1]/*VOICE07_BOMB*/, TRACK01_EXPLODE);/*テキトー*/
					#endif
				}
			}
			else	/* 設定項目を変更する場合 */
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL) /* キャンセルボタン入力 */
			{
				play_music_num(BGM_00_stop);
			}
		}
	}
	/* 描画 */
	{
	//	/*static*/ SDL_Rect rect_src_offset;		/* 位置 */
		/*static*/ SDL_Rect rect_locate_offset; 	/* 表示位置 */
		int i;
		int jj;
		int kk;
		/* 機能番号の描画 */
		kk = (0);
		#define MAX_MENU_02 			(MUSIC_ROOM_02_QUIT)
		for (i=0; i<(MAX_MENU_02); i++)/*MUSIC_ROOM_03_MAX*/ /*12*/
		{
			/* ボタン設定描画 */
			{
				int kinou_number;
				kinou_number = music_room_setting[i];
				if (menu_cursor1==i)	{	jj = 2; 	}
				else					{	jj = 0; 	}
				rect_locate_offset.x = X_LOCATE_OFFSET_04-(jj);
				rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/kk)-(jj);
				SDL_SetAlpha(	music_room_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(music_room_name_surface[kinou_number], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			//	rect_src_offset.x = 0;
			//	rect_src_offset.y = (kinou_number<<4);
			//	rect_src_offset.w = (16*9);
			//	rect_src_offset.h = (16);
			//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],&rect_src_offset,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
			kk += (20);
		}
		/* ボタン名称描画 */
		kk = (0);
		for (i=0; i<(MUSIC_ROOM_03_MAX); i++)/*MUSIC_ROOM_03_MAX*/
		{
			if (menu_cursor1==i)	{	jj = 2; 	}
			else					{	jj = 0; 	}
			rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
			rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/kk)-(jj);
			SDL_SetAlpha(	music_room_name_surface[i+(MOJI_23_MAX)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
			SDL_BlitSurface(music_room_name_surface[i+(MOJI_23_MAX)], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			kk += (20);
		}
	}
}


/*---------------------------------------------------------
	幻想音樂室 フェードイン中
---------------------------------------------------------*/

static void music_room_state_01_fade_in(void)
{
	bg_alpha_aaa += (2);	/*fps_factor*/
	if ((200) < bg_alpha_aaa)/*(250-6) (224)*/
	{
		bg_alpha_aaa = (255);
		psp_push_screen();
		main_call_func = music_room_state_02_select_menu;
	}
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------
	幻想音樂室 開始処理
---------------------------------------------------------*/

global void music_room_start(void)
{
	play_music_num(BGM_06_stage6);
	load_SDL_bg(BG_TYPE_03_music_room);
	music_room_setting[0] = (0);
	music_room_setting[1] = (0);
	music_room_setting[2] = (USE_26_MUSIC_FILES-1);/*MOJI_23_MAX*/
	music_room_setting[3] = (VOICE18_MAX_FILES-1);/*MOJI_23_MAX*/
	{
		static const char *const_menu_str[(MAX_KEY_NAMES_21)] =
		{
			/* [[ 項目 ]] */
			" 0",	" 1",	" 2",	" 3",
			" 4",	" 5",	" 6",	" 7",
			" 8",	" 9",	"10",	"11",
			"12",	"13",	"14",	"15",
			"16",	"17",	"18",	"19",
			"20",	"21",	"22",	"23",
			"24",	"25",
			/* [[ メニュー ]] */
			"BGM",
			"SOUND",
			"QUIT"
		};
		{
			unsigned int i;
			for (i=0; i<(MAX_KEY_NAMES_21); i++)
			{
				music_room_name_surface[i] = font_render( (char *)const_menu_str[i], FONT16R);
			}
		}
	}
	menu_cursor1		= MUSIC_ROOM_02_QUIT;
	bg_alpha_aaa		= (0);
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	cg.msg_time = byou60(5);	/* 約 5 秒 */
	print_kanji000("ようこそ\\n"//"幻想音樂室\\n"
		"完コピとか私の能\力的に無理ですので、\\n"
		"BGMは全曲勝手にアレンジ版です。",
	//	"原曲の面影がないほど変わっちゃってるのもあります。",
		/*int color_type*/(7), /*int wait*/(0) );
	main_call_func = music_room_state_01_fade_in;
}
