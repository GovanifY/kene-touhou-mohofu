
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ミュージックルーム 幻想音樂室
---------------------------------------------------------*/
#include "111_my_file.h"/*(bg読みこみ。)*/

#include "kanji_system.h"

enum
{
	MOJI_00 = 0,MOJI_01,	MOJI_02,	MOJI_03,
	MOJI_04,	MOJI_05,	MOJI_06,	MOJI_07,
	MOJI_08,	MOJI_09,	MOJI_10,	MOJI_11,
	MOJI_12,	MOJI_13,	MOJI_14,	MOJI_15,
	MOJI_16,	MOJI_17,	MOJI_18,	MOJI_19,
	MOJI_20,	MOJI_21,	MOJI_22,	MOJI_23,
	MOJI_24,	MOJI_25,	MOJI_26,	MOJI_27,
	MOJI_28,	MOJI_29,	MOJI_30,	MOJI_31,
	MOJI_32,	MOJI_33,	MOJI_34_MAX,/*(曲の最大数)*/
	KIDOUYOU_LINE_01,
	KIDOUYOU_LINE_02,
	KIDOUYOU_LINE_03,
	KIDOUYOU_LINE_04_MAX/*(項目最大数)*/
};

enum
{
	SOUND_TYPE_00_BGM = 0,
	SOUND_TYPE_01_SOUND,
	SOUND_TYPE_02_QUIT,
	SOUND_TYPE_03_MAX,
};

enum
{
	SOUND_INDEX_00_BASE_CURSOR_01 = 0,
	SOUND_INDEX_01_BASE,
	SOUND_INDEX_04_BASE_CURSOR_02,
	SOUND_INDEX_05_BASE,
	SOUND_INDEX_02_BASE_MAX_NUM,
	SOUND_INDEX_03_BASE,
	SOUND_INDEX_06_MAX	/* 最大数 */
};
#define MAX_KEY_NAMES_21		(MOJI_34_MAX+SOUND_TYPE_03_MAX)
/*
だめ文字(シフトJISの2バイト目が0x5c、すなわち'\'('＼')である文字のこと。)
	ソ噂浬欺圭構蚕十申曾箪貼能表暴予禄兔喀媾彌拿杤歃濬畚秉綵臀藹觸軆鐔饅鷭
*/

//static int music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02];

static int music_room_setting[SOUND_INDEX_06_MAX/*(2)+(2)+(2)*/];/*SOUND_TYPE_03_MAX*/
static int sound_type;
static int bg_alpha_aaa;

static void music_room_draw_message(unsigned int cursor1, unsigned int cursor2)
{
//	kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
	static const char *const_kaisetu_str[(KIDOUYOU_LINE_04_MAX)][(3)] =
	{
		//								"-------------------------------------"
		//	"SOUND 解説",				"BGM 解説", 								"-------------------------------------"
		{	"ショット", 				"曲を止めます", 							"弾には無音も良いですよね。",					},/* */
		{	"メニュー決定", 			"No.1 春の湊に",							"1面のテーマです。",							},/* */
		{	"メニュー選択", 			"No.2 ロマンティック・チルドレン",			"2面のテーマです。",							},/* */
		{	"ボス退場", 				"No.3 ヴォヤージュ1969",					"3面のテーマです。",							},/* */
		{	"ピチューン",				"No.4 フォールオブフォール ～ 秋めく滝",	"4面のテーマです。",							},/* */
		{	"アイテム取得", 			"No.5 ヴアル魔法図書館",					"5面のテーマです。",							},/* */
		{	"増えた",					"No.6 メイドと血の懐中時計",				"6面のテーマです。",							},/* */
		{	"自分カード",				"No.7 童祭 ～ Innocent Treasures",			"スタッフロールのテーマです。", 				},/* */ 	// 夢違科学世紀「童祭 Innocent Treasures」
		{	"ザコ倒した",				"No.8 ルーネイトエルフ",					"道中曲です。", 								},/* */
		{	"グレイズ", 				"No.9 遠野幻想物語",						"道中曲です。", 								},/* */
		{	"時間制限", 				"No.10 上海紅茶館  ～ Chinese Tea", 		"道中曲です。", 								},/* */
		{	"キラリン☆",				"No.11 人形裁判 ～ 人の形弄びし少女",		"アリス・マーガトロイドのテーマです。", 		},/* */
		{	"ボスショット1",			"No.12 コンプリート・ダークネス",			"魅魔のテーマです。",							},/* */ 	// Complete Darkness
		{	"ボスショット2",			"No.13 竹取飛翔 ～ Lunatic Princess",		"蓬莱山 輝夜のテーマです。",					},/* */
		{	"ボスショット3",			"No.14 風神少女",							"射命丸 文のテーマです。",						},/* */
		{	"ボスショット4",			"No.15 ラクトガール ～ 少女密室",			"パチュリー・ノーレッジのテーマです。", 		},/* */
		{	"ボスショット5",			"No.16 月時計 ～ ルナ・ダイアル",			"十\六夜 咲夜のテーマです。",					},/* */
		{	"ボス溜め1",				"No.17 おてんば恋娘",						"チルノのテーマです。", 						},/* */
		{	""/*"未使用18音"*/, 		"No.18 グリモア オブ アリス",				"アリス・マーガトロイドのテーマです。", 		},/* */ 	// Grimoire of Alice
		{	""/*"未使用19音"*/, 		"No.19 恋色マスタースパーク",				"霧雨 魔理沙のテーマです。",					},/* */
		{	""/*"未使用20音"*/, 		"No.20 広有射怪鳥事 ～ Till When?", 		"妖夢のテーマです。",							},/* */
		{	""/*"未使用21音"*/, 		"No.21 亡き王女の為のセプテット",			"レミリア・スカーレットのテーマです。", 		},/* */
		{	""/*"未使用22音"*/, 		"No.22 U.N.オーエンは彼女なのか？", 		"フランドール・スカーレットのテーマです。", 	},/* */
		{	""/*"未使用23音"*/, 		"No.23 少女綺想曲 ～ capriccio",			"博麗 霊夢のテーマです。",						},/* */ 	// Capriccio == 奇想曲(音楽用語,イタリア語), Un sogno capriccioso == 気まぐれな夢(イタリア語)
		{	""/*"未使用24音"*/, 		"No.24 妖魔夜行",							"ルーミアのテーマです。",						},/* */
		{	""/*"未使用25音"*/, 		"No.25 プレイヤーズスコア", 				"エンディングのテーマです。",					},/* */
		{	""/*"未使用26音"*/, 		"No.26 無何有の郷 ～ Deep Mountain",		"道中曲です。", 								},/* */
		{	""/*"未使用27音"*/, 		"No.27 妖々夢 ～ Snow or Cherry Petal", 	"タイトル画面のテーマです。",					},/* */
		{	""/*"未使用28音"*/, 		"No.28 バッド・アップル ～ Bad Apple!!",	"道中曲です。", 								},/* */
		{	""/*"未使用29音"*/, 		"No.29 永夜の報い", 						"使用してますが、本当は没曲です。", 			},/* */
		{	""/*"未使用28音"*/, 		"No.30 死霊の夜桜", 						"使用してますが、本当は没曲です。", 			},/* */ /*("終了"で使う)*/
		{	""/*"未使用27音"*/, 		"No.31 紅より儚い永遠", 					"名前入力画面のテーマです。",					},/* */
		{	""/*"未使用27音"*/, 		"No.32 妖々跋扈",							"道中曲です。", 								},/* */
		{	""/*"未使用28音"*/, 		"No.33 ブクレシュティの人形師", 			"道中曲です。", 								},/* */
		// "起動用メッセージで(4行)必要"
		{	"あれ？もうお帰りですか？", "まで創りました。", 						"", 											},/* */
		{	"", 						"ようこそ幻想音樂室へ", 					"で該当BGMを作成すると鳴ります。",				},/* */
		{	"", 						"BGMは全曲勝手にアレンジ版です。",			"", 											},/* */
		{	"", 						"アレンジ版が嫌ならば、拡張子.ogg", 		"", 											},/* */
	};
	//
	/*(色はとりあえず白)*/
	{
		unsigned int jj;
		jj = (music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02]);
		unsigned int ii;
		unsigned int yyy18;
		yyy18 = (0);/* Y位置 */
		for (ii=0; ii<(7); ii++) /* 7 行描画(8行目は使わない) */
		{
			kanji_window_clear_line(ii);	/* 漢字ウィンドウの4行目(==3)の内容を消す。 */
			set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (yyy18));	/* カーソルを4行目(==3)へ移動 */
		//	if (0==cursor1)/* BGMモード */
			if (2 > cursor1)/* 0:BGMモード & 1:効果音モード */
			{
				if (0==ii)	/* 1行目のみ(0) */
				{	/*(見だし行)*/
					kanji_color((9)|STR_CODE_NO_ENTER);
					strcpy(my_font_text, (char *)const_kaisetu_str[cursor2][(/*ii+*/1-cursor1)]);
				}
				else
				if (ii<6)	/* 23456行目のみ(1,2,3,4,5) */
				{	/*(スクロール行)*/
					if ((cursor2)==(jj))
					{
						kanji_color((9)|STR_CODE_NO_ENTER);
					}
					else
					{
						kanji_color((8)|STR_CODE_NO_ENTER);
					}
					strcpy(my_font_text, (char *)const_kaisetu_str[jj][(/*ii+*/1-cursor1)]);
					jj += (1);
				}
				else/* 7行目のみ(6) */
				{	/*(解説行)*/
					kanji_color((7)|STR_CODE_NO_ENTER);
					strcpy(my_font_text, (char *)const_kaisetu_str[cursor2][(2/*ii+1-4*/-cursor1-cursor1)]);		/* 効果音モード の場合しょーがないから見出しと同じ解説 */
				}
			}
			else/* 2:終了モード、のみ(r35u2) */
			{
				kanji_color((9)|STR_CODE_NO_ENTER);
				if (0==ii)	/* 1行目のみ */
				{	/*(見だし行終了メッセージ)*/
					strcpy(my_font_text, (char *)const_kaisetu_str[/*33*/(MOJI_34_MAX)/*cursor2*/][0]);
				}
				else	/* 残り(2-7行目)を消す。 */
				{
					strcpy(my_font_text, (char *)"");
				}
			}
			kanji_draw();
			yyy18 += (18);
		}
	}
}
#if 0
global void music_game_draw_message(int cursor1)
{
	static const char *const_music_title_str[(MOJI_34_MAX)] =
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
	strcpy(my_font_text, (char *)const_music_title_str[cursor1]);
	kanji_color(7);
	kanji_draw();
}
#endif


/*---------------------------------------------------------
	幻想音樂室 フェードアウトしておしまい中
---------------------------------------------------------*/

static MAIN_CALL_FUNC(music_room_state_03_fade_out)
{
	bg_alpha_aaa += (2);	/*(1) (8)*/ 	/*fps_factor*/
	if ((224) < bg_alpha_aaa)/*(250-8)*/
	{
		bg_alpha_aaa = (255);
		#if 1/* 文字の表示用画像を開放する */

		#endif
		cb.main_call_func = title_menu_start;	/* タイトルメニューへ移動 */
	}
	SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------
	幻想音樂室 メニュー選択して、再生したり、描画したり中
---------------------------------------------------------*/

static void up_down_allow_key_auto_repeat(void)
{
	#if 1/*(menuでは、強制的にvsyncをとる)*/
//	sceDisplayWaitVblankStart();
	#endif
	#if 1/*(上下ボタン入力の場合、オートリピート処理を行う。)*/
	{
		static u32 auto_repeat_counter;/*(オートリピート用カウンタ)*/
		if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN))		/* 今回上下ボタン入力 */
		{
			if (psp_pad.pad_data_alter & (PSP_KEY_UP|PSP_KEY_DOWN)) 	/* さっきも上下ボタン入力 */
			{
				/*(ここにくれば、上下のどちらかのボタンを押しっぱなしにしているのが確定している。)*/
				auto_repeat_counter++;/*(押しっぱなしの時間をカウント)*/
				#define REPEAT_20_START 	(20)	/*(初回リピート開始時間: 20[frame])*/
				#define REPEAT_08_REPEAT	(8) 	/*(リピート繰り返し間隔:  8[frame])*/
				if (REPEAT_20_START < auto_repeat_counter)/*(20[frame]初回リピート)*/
				{
					auto_repeat_counter = (REPEAT_20_START-REPEAT_08_REPEAT);/*(8[frame]リピート繰り返し間隔)*/
					/*(キーリピート処理)*/
					psp_pad.pad_data_alter = 0;/*(キーが離された事によりリピート==連射を行う)*/
					/*(このタイミングで(前回)キー入力を書き換えても、次回のリアルキー入力には反映しない。(だから書き換えても良い))*/
				}
			}
			else/*(リアルキー入力で上下入力があった場合解除)*/
			{	auto_repeat_counter = 0;/*(オートリピート解除)*/	}
		}
		else/*(リアルキー入力で上下入力があった場合解除)*/
		{	auto_repeat_counter = 0;/*(オートリピート解除)*/	}
	}
	#endif
}

static MAIN_CALL_FUNC(music_room_state_02_select_menu)
{
	psp_pop_screen();
	cg.msg_time = /*(永遠に描画)*/byou60(5);	/* 約 5 秒 */
	//
	up_down_allow_key_auto_repeat();
	if (0==psp_pad.pad_data_alter)/* さっき何も押されてなかった場合にキーチェック(原作準拠) */
	{
		if (psp_pad.pad_data & PSP_KEY_LEFT)			/* 左ボタン入力 */
		{
			if (SOUND_TYPE_00_BGM == sound_type)		{	sound_type = (SOUND_TYPE_03_MAX-1); }
			else										{	sound_type--;	}
		}
		else
		if (psp_pad.pad_data & PSP_KEY_RIGHT)			/* 右ボタン入力 */
		{
			if ((SOUND_TYPE_03_MAX-1) == sound_type)	{	sound_type = SOUND_TYPE_00_BGM; }
			else										{	sound_type++;	}
		}
		else	/* 左右以外の通常処理(斜め入力を排除する為、左右の入力があった場合はそれ以外の処理をしない) */
		{
			if (SOUND_TYPE_02_QUIT == sound_type) /* 項目[ QUIT ] を選んでいる場合 */
			{
				if (psp_pad.pad_data & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL))	/* ショット || キャンセルボタン入力 */
				{
					#if (1)
					voice_play(VOICE04_SHIP_HAKAI, TRACK02_MENU02/*TRACK01_MENU01*/);/* 自機死に音は、なるべく重ねない */
				//	voice_play(VOICE02_MENU_SELECT, TRACK01_MENU01);/*テキトー*/
					#endif
				//	pl ay_music_num(BGM_27_menu01);
					my_file_common_name[0] = BG_TYPE_00_title_bg;psp_load_bg_file_name();/* 裏画面にロード */
					cg.msg_time = (0);	/* 約 0 秒 */
//					bg_alpha_aaa		= 255;
					bg_alpha_aaa		= 0;
					cb.main_call_func = music_room_state_03_fade_out;
				}
			}
			else	/* 設定項目を変更する場合 */
			if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN))		/* 上下ボタン入力 */
			{
				if (psp_pad.pad_data & PSP_KEY_UP)			/* 上ボタン入力 */
				{
					music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01]--;
					if ((music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02]) > music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01])
					{
						music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02]--;
					}
					if ((0/*MOJI_00*/) > music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01])
					{
						music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01]	= music_room_setting[SOUND_INDEX_02_BASE_MAX_NUM+sound_type];	/*(BGMの最大数)*/	/*(効果音の最大数)*/
						music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02] = (music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01])-(4);
					}
				}
				else
				if (psp_pad.pad_data & PSP_KEY_DOWN)		/* 下ボタン入力 */
				{
					music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01]++;
					if ((music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02]+4) < music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01])
					{
						music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02]++;
					}
					if (music_room_setting[sound_type+SOUND_INDEX_02_BASE_MAX_NUM] < music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01])
					{
						music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01] = (0)/*MOJI_00*/;
						music_room_setting[sound_type+SOUND_INDEX_04_BASE_CURSOR_02] = (0);
					}
				}
			}
			else	/* 設定項目を変更する場合 */
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK) /* ショットボタン入力 */
			{
				if (SOUND_TYPE_00_BGM == sound_type)	/* 項目[ BGM ] を選んでいる場合 */
				{
					#if (1)/*(-r38までと互換させるなら有効にする)*/
					play_music_num(BGM_00_stop);/*(r39では一度止めないと同じ曲は始めから鳴らない様に仕様変更した)*/
					/*(別に無くても構わない気もするが、あくまで曲再生の「テストモード」なのだから、始めから鳴らした方が良い。)*/
					/*(「テストモード」が(存在する事が)「主(目的)」で、曲再生が「従(結果)」。)*/
					#endif
					play_music_num(music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01]/*BGM_05_stage5*/);
				}
				else
//				if (SOUND_TYPE_01_SOUND == sound_type)	/* 項目[ SOUND ] を選んでいる場合 */
				{
					#if (1)
					voice_play(music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01]/*VOICE07_BOMB*/, TRACK01_MENU01);/*テキトー*/
					#endif
				}
			}
			else	/* 設定項目を変更する場合 */
			if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL) /* キャンセルボタン入力 */
			{
				play_music_num(BGM_00_stop);
			}
		}
		if (psp_pad.pad_data & (PSP_KEY_UP|PSP_KEY_DOWN|PSP_KEY_LEFT|PSP_KEY_RIGHT))	/* 上下左右ボタン入力 */
		{
			/* 再描画 */
			music_room_draw_message(sound_type, music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01]);
			/* 描画 */
			{
				/* 機能番号の描画 */
				#define MAX_MENU_02 			(SOUND_TYPE_02_QUIT)
				{
				static const char *const_aaa_str[(3)] =
				{
					/* [[ メニュー ]] */
				//	"0123456789"
					"曲再生 #00",
					"効果音 #00",
					"終了      "
				};
				strcpy(my_font_text, (char *)const_aaa_str[sound_type]);
				/* モード描画 */
					/* 番号ボタン設定描画 */
					if (2 > sound_type)/*0 or 1*/
					{
						int kinou_number;
						kinou_number = music_room_setting[sound_type+SOUND_INDEX_00_BASE_CURSOR_01];
						dec_print_format( ((kinou_number)), 2, (char *)&my_font_text[8]);
					}
					kanji_window_clear_line(ML_LINE_08);	/* 漢字ウィンドウの4行目(==3)の内容を消す。 */
					set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_08*18));	/* カーソルを4行目(==3)へ移動 */
					kanji_color(7);
					kanji_draw();
				}
			}
		}
	}
}


/*---------------------------------------------------------
	幻想音樂室 フェードイン中
---------------------------------------------------------*/

static MAIN_CALL_FUNC(music_room_state_01_fade_in)
{
	bg_alpha_aaa += (2);	/*fps_factor*/
	if ((200) < bg_alpha_aaa)/*(250-6) (224)*/
	{
		bg_alpha_aaa = (255);
		psp_push_screen();
		cb.main_call_func = music_room_state_02_select_menu;
	}
	SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
	psp_pop_screen();
}


/*---------------------------------------------------------
	幻想音樂室 開始処理
---------------------------------------------------------*/

global MAIN_CALL_FUNC(music_room_start)
{
	play_music_num(BGM_11_boss01);/* 人形裁判 */
	my_file_common_name[0] = BG_TYPE_03_music_room;psp_load_bg_file_name();/* 裏画面にロード */
	music_room_setting[SOUND_TYPE_00_BGM   + SOUND_INDEX_00_BASE_CURSOR_01] = (0);
	music_room_setting[SOUND_TYPE_01_SOUND + SOUND_INDEX_00_BASE_CURSOR_01] = (0);
	music_room_setting[SOUND_TYPE_00_BGM   + SOUND_INDEX_04_BASE_CURSOR_02] = (0);
	music_room_setting[SOUND_TYPE_01_SOUND + SOUND_INDEX_04_BASE_CURSOR_02] = (0);
	music_room_setting[SOUND_TYPE_00_BGM   + SOUND_INDEX_02_BASE_MAX_NUM] = (USE_31_MUSIC_FILES-1);/*MOJI_34_MAX*/	/*(BGMの最大数)*/
	music_room_setting[SOUND_TYPE_01_SOUND + SOUND_INDEX_02_BASE_MAX_NUM] = (VOICE18_MAX_FILES-1);/*MOJI_34_MAX*/	/*(効果音の最大数)*/
//
	sound_type			= SOUND_TYPE_02_QUIT;
	bg_alpha_aaa		= (0);
	set_kanji_origin_xy(24, 100);/*(表示原点の設定)*/
	set_kanji_origin_kankaku(19);/*(字間を少し広げる)*/
	//
	set_kanji_hide_line(ML_LINE_99_MAX);/*(全行表示する。)*/
	{
		/* 1行目のみ特殊な位置へ移動。 */
		ml_font[ML_LINE_01].x		= (0);/* X位置 */
		ml_font[ML_LINE_01].y		= (64);/* Y位置 */
	}
	{
		/* 8行目のみ特殊な位置へ移動。 */
		ml_font[ML_LINE_08].x		= (160);/* X位置 */
		ml_font[ML_LINE_08].y		= (240);/* Y位置 */
	}
	kanji_window_all_clear();	/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
	cg.msg_time = (65536);		/* 約 18 分 */
//	cg.msg_time = byou60(5);	/* 約 5 秒 */
	#if (1)/*(r38署名版でハングアップバグ修正)*/
	music_room_draw_message(0, (KIDOUYOU_LINE_01)/*(起動用メッセージ)*/ );
	#endif
	cb.main_call_func = music_room_state_01_fade_in;
}
