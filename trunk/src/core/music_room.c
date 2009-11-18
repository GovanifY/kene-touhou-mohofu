
#include "game_main.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	ミュージックルーム
---------------------------------------------------------*/

enum
{
	MUSIC_ROOM_STATE_00_INIT = 0,
	MUSIC_ROOM_STATE_01_FADE_IN,
	MUSIC_ROOM_STATE_02_SELECT_MENU,
	MUSIC_ROOM_STATE_03_FADE_OUT,
};

enum
{
	MOJI_00 = 0,MOJI_01,	MOJI_02,	MOJI_03,
	MOJI_04,	MOJI_05,	MOJI_06,	MOJI_07,
	MOJI_08,	MOJI_09,	MOJI_10,	MOJI_11,
	MOJI_12,	MOJI_13,	MOJI_14,	MOJI_15,
	MOJI_16,	MOJI_17,//	MOJI_18,	MOJI_19,
	MOJI_17_MAX 	/* 最大数 */
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
#define Y_LOCATE_OFFSET 	(100)
#define MAX_KEY_NAMES_20	(MOJI_17_MAX+MUSIC_ROOM_03_MAX)
static void music_room_draw_message(int cursor1, int cursor2)
{
	script_message_window_clear();
	static const char *const_kaisetu_str[(2)][(1+17)] =
	{
		{	/*"BGM 解説"*/
"曲を止めます",
"No.1 春の湊に\\n"							"1面のテーマです。",
"No.2 フォールオブフォール　～ 秋めく滝\\n" "2面のテーマです。",
"No.3 ヴォヤージュ1969\\n"					"3面のテーマです。",
"No.4 広有射怪鳥事 ～ Till When?\\n"		"4面のテーマです。\\n"						"ボス曲ではないのです。",
"No.5 無何有の郷　～ Deep Mountain\\n"		"5面のテーマです。",
"No.6 亡き王女の為のセプテット\\n"			"6面のテーマです。" 						"ボス曲ではないので、\\nずいぶんまったりしてます。",
"No.7 曲\\n"								"スタッフロールのテーマです。",
"No.8 遠野幻想物語\\n"						"エキストラステージのテーマです。\\n"		"2年前、時事ネタの偽ユーロ風",
"No.9 紅より儚い永遠\\n"					"タイトル画面のテーマです。",
"No.10 人形裁判　～ 人の形弄びし少女\\n"	"アリスのテーマです。"						"ハンドベルを入れてみました。人形が一つづつ持ってる？",
"No.11 風神少女　～ Full Version\\n"		"射命丸 文のテーマです。\\n"				"1ループ5分以上あるので、長いです。",
"No.12 まだ作ってません\\n" 				"っていうか3ボス誰にしようか。\\n"			"うさぎにしたい気もします。",
"No.13 竹取飛翔　～ Lunatic Princess\\n"	"蓬莱山 輝夜のテーマです。\\n"				"月人にならわかるかも。",
"No.14 ラクトガール　～ 少女密室\\n"		"パチュリー・ノーレッジのテーマです。\\n"	"この曲の元ネタにも忠実に。",
"No.15 月時計　～ ルナ・ダイアル\\n"		"十\六夜 咲夜のテーマです。\\n" 			"趣味でプログレ風にしてみました。",
"No.16 プレイヤーズスコア\\n"				"エンディングのテーマです。"				"満身創痍",
"No.17 おてんば恋娘\\n" 					"チルノのテーマです。\\n"					"この曲は演歌なんですよね、一応。",
		},
		{	/*"SOUND 解説"*/
"ショット", 		"メニュー決定", 	"メニュー選択", 	"ボス退場",
"ピチューン",		"アイテム取得", 	"増えた",			"自分スペカ",
"ザコ倒した",		"グレイズ", 		"ボスショット1",	"キラリン☆",
"ボスショット2",	"ボスショット3",	"ボスショット4",	"時間制限",
"16音",
"17音",
		},
	};
	print_kanji000((char *)const_kaisetu_str[cursor1][cursor2], /*int color_type*/7, /*int wait*/0);
}

/*static*/ void music_room_work(void)
{
	static int music_room_setting[2+2/*MUSIC_ROOM_03_MAX*/];
	static SDL_Surface *music_room_name_surface[MAX_KEY_NAMES_20];	// 文字の画像
	static int menu_cursor1;
	static int bg_alpha_aaa;
//
	switch (psp_loop)
	{
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_00_INIT):
		play_music_num(BGM_02_stage2);
		{
			SDL_Surface *loadpic	= loadbmp0("bg/music_room.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
			SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
			unloadbmp_by_surface(loadpic);
			//unloadbmp_by_surface(key_haikei_surface);
		}
		music_room_setting[0] = 0;
		music_room_setting[1] = 0;
		music_room_setting[2] = (USE_MUSIC_FILES/*MOJI_17_MAX*/-1);
		music_room_setting[3] = (VOICE16_MAX_FILES/*MOJI_17_MAX*/-1);
		{
			static const char *const_menu_str[(MAX_KEY_NAMES_20)] =
			{
				// [[ 項目 ]]
				" 0",	" 1",	" 2",	" 3",
				" 4",	" 5",	" 6",	" 7",
				" 8",	" 9",	"10",	"11",
				"12",	"13",	"14",	"15",
				"16",	"17",// "18",	"19",
				// [[ メニュー ]]
				"BGM",
				"SOUND",
				"QUIT"
			};
			{
				int i;
				for (i=0; i<(MAX_KEY_NAMES_20); i++)
				{
					music_room_name_surface[i] = font_render((char *)const_menu_str[i],FONT03/*FO NT06*/);
				}
			}
		}
		menu_cursor1		= MUSIC_ROOM_02_QUIT/*0*/;
		bg_alpha_aaa		= 0;
		script_message_window_clear();
		msg_time = (60*5);
		print_kanji000("幻想音樂室\\n"
			"完コピとか私の能\力的に無理ですので、\\nBGMは全曲勝手にアレンジ版です。"
			/*"原曲の面影がないほど変わっちゃってるのもあります。"*/, /*int color_type*/7, /*int wait*/0);
		psp_loop++;//psp_loop = (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_01_FADE_IN);
		//SDL_SetAlpha(key_haikei_surface, SDL_SRCALPHA, 0);
		//keyboa rd_clear();
		break;
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_01_FADE_IN):
		bg_alpha_aaa += /*1*/2/*6*/;/**fps_fa ctor*/
		if ((/*250-6*/200/*224*/) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			psp_push_screen();
			psp_loop++;//psp_loop = (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_02_SELECT_MENU);
		}
		SDL_SetAlpha(/*key_haikei_surface*/sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		break;
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_02_SELECT_MENU):
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		msg_time = (60*5);
		if (0==my_pad)
		{
			if (my_pad_alter & PSP_KEY_UP)				// 上ボタン入力
			{
				if (MUSIC_ROOM_00_BGM == menu_cursor1)		{	menu_cursor1 = (MUSIC_ROOM_03_MAX-1);}
				else										{	menu_cursor1--;}
			}
			else
			if (my_pad_alter & PSP_KEY_DOWN)			// 下ボタン入力
			{
				if ((MUSIC_ROOM_03_MAX-1) == menu_cursor1)	{	menu_cursor1 = MUSIC_ROOM_00_BGM;}
				else										{	menu_cursor1++;}
			}
			else	/* 上下以外の通常処理(斜め入力を排除する為、上下の入力があった場合はそれ以外の処理をしない) */
			{
				if (MUSIC_ROOM_02_QUIT == menu_cursor1) // 項目[ QUIT ] を選んでいる場合
				{
					if (my_pad_alter & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL))	// ショット || キャンセルボタン入力
					{
						#if (0==USE_DESIGN_TRACK)
						play_voice_auto_track(VOICE04_SHIP_HAKAI);
					//	play_voice_auto_track(VOICE02_MENU_SELECT);
						#else
						voice_play(VOICE04_SHIP_HAKAI, TRACK03_SHORT_MUSIC/*TRACK01_EXPLODE*/);/* 自機死に音は、なるべく重ねない */
					//	voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);/*テキトー*/
						#endif
						#if 0
						{
							int i;
							for (i=0; i<(MAX_KEY_NAMES_20); i++)
							{
								/* ここでハングアップ */
								SDL_FreeSurface(music_room_name_surface[i]);
							}
						}
						#endif
						play_music_num(BGM_09_menu1);
						{
							//key_haikei_surface	= loadbmp("bg/title_bg.jpg");
							SDL_Surface *loadpic	= loadbmp0("bg/title_bg.jpg", 0, 0);/*"bg/key_haikei_surface.png"*/
							SDL_BlitSurface(loadpic, NULL, sdl_screen[SDL_01_BACK_SCREEN], NULL);
							unloadbmp_by_surface(loadpic);
							//unloadbmp_by_surface(key_haikei_surface);
						}
						msg_time = (0);
//						bg_alpha_aaa		= 255;
						bg_alpha_aaa		= 0;
						psp_loop++;//	psp_loop = (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_03_FADE_OUT);
					//	psp_loop++;//	psp_loop = (ST_WORK_MUSIC_ROOM|OPTION_MENU_STATE_06_FADE_INIT);
					}
				}
				else	/* 設定項目を変更する場合 */
				if (my_pad_alter & (PSP_KEY_LEFT|PSP_KEY_RIGHT))		// 左右ボタン入力
				{
					if (my_pad_alter & PSP_KEY_LEFT)			// 左ボタン入力
					{
						music_room_setting[menu_cursor1]--;
						if ((0/*MOJI_00*/) > music_room_setting[menu_cursor1]) {	music_room_setting[menu_cursor1] = music_room_setting[menu_cursor1+2]; }
						music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
					}
					else
					if (my_pad_alter & PSP_KEY_RIGHT)			// 右ボタン入力
					{
						music_room_setting[menu_cursor1]++;
						if (music_room_setting[menu_cursor1+2] < music_room_setting[menu_cursor1])	{	music_room_setting[menu_cursor1] = 0/*MOJI_00*/; }
						music_room_draw_message(menu_cursor1,music_room_setting[menu_cursor1]);
					}
				}
				else	/* 設定項目を変更する場合 */
				if (my_pad_alter & PSP_KEY_SHOT_OK) // ショットボタン入力
				{
					if (MUSIC_ROOM_00_BGM == menu_cursor1)	// 項目[ BGM ] を選んでいる場合
					{
						play_music_num(music_room_setting[menu_cursor1]/*BGM_05_stage5*/);
					}
					else
//					if (MUSIC_ROOM_01_SOUND == menu_cursor1)	// 項目[ SOUND ] を選んでいる場合
					{
						#if (0==USE_DESIGN_TRACK)
						play_voice_auto_track(music_room_setting[menu_cursor1]/*VOICE07_BOMB*/);
						#else
						voice_play(music_room_setting[menu_cursor1]/*VOICE07_BOMB*/, TRACK01_EXPLODE);/*テキトー*/
						#endif
					}
				}
				else	/* 設定項目を変更する場合 */
				if (my_pad_alter & PSP_KEY_BOMB_CANCEL) // キャンセルボタン入力
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
			kk=0;
			#define MAX_MENU_02 			(MUSIC_ROOM_02_QUIT)
			for (i=0; i</*12*/(MAX_MENU_02/*MUSIC_ROOM_03_MAX*/)/*MUSIC_ROOM_03_MAX*/; i++)
			{
				/* ボタン設定描画 */
				{
					int kinou_number;
					kinou_number = music_room_setting[i];
					if (menu_cursor1==i)	{	jj= 2;	}
					else					{	jj= 0;	}
					rect_locate_offset.x = X_LOCATE_OFFSET_04-(jj);
					rect_locate_offset.y = Y_LOCATE_OFFSET+(/*num*/kk)-(jj);
					SDL_SetAlpha(	music_room_name_surface[kinou_number], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
					SDL_BlitSurface(music_room_name_surface[kinou_number], NULL,sdl_screen[SDL_00_SCREEN],&rect_locate_offset);
				//	rect_src_offset.x = 0;
				//	rect_src_offset.y = (kinou_number<<4);
				//	rect_src_offset.w = (16*9);
				//	rect_src_offset.h = (16);
				//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],&rect_src_offset,sdl_screen[SDL_00_SCREEN],&rect_locate_offset);
				}
				kk += 20;
			}
			/* ボタン名称描画 */
			kk=0;
			for (i=0; i<(MUSIC_ROOM_03_MAX)/*MUSIC_ROOM_03_MAX*/; i++)
			{
				if (menu_cursor1==i)	{	jj= 2;	}
				else					{	jj= 0;	}
				rect_locate_offset.x = (X_LOCATE_OFFSET_02)-(jj);
				rect_locate_offset.y = (Y_LOCATE_OFFSET)+(/*num*/kk)-(jj);
				SDL_SetAlpha(	music_room_name_surface[i+(MOJI_17_MAX)], SDL_SRCALPHA, /*bg_alpha_aaa*/(127+(jj<<6)));
				SDL_BlitSurface(music_room_name_surface[i+(MOJI_17_MAX)], NULL,sdl_screen[SDL_00_SCREEN],&rect_locate_offset);
				kk += 20;
			}
		}
		break;
	case (ST_WORK_MUSIC_ROOM|MUSIC_ROOM_STATE_03_FADE_OUT):
		bg_alpha_aaa += /*1*/2/*8*/;/**fps_fa ctor*/
		if ((/*250-8*/224) < bg_alpha_aaa)
		{
			bg_alpha_aaa = 255;
			/* メインメニューに戻る */
			psp_loop=(ST_INIT_MENU|0/*ST_ME NU_SUB_MAIN_MENU*/);	//newsta te(ST_MENU/*ST_INTRO*/,0,1);
		}
		SDL_SetAlpha(/*key_haikei_surface*/sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, bg_alpha_aaa);
		psp_pop_screen();//SDL_BlitSurface(key_haikei_surface,NULL,sdl_screen[SDL_00_SCREEN],NULL);
		break;
	}
}
