
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _BGM_VOICE_H_
#define _BGM_VOICE_H_

//;「音楽素材」必要なファイル

enum	//; 効果音(Voice)
{
	VOICE00_SHOT = 0,			//	"ショット", 		/* a				ショット音							[ちりちりちり] */
	VOICE01_MENU_OK,			//	"メニュー決定", 	/*[b]	[s(?)]		音									[びょいー] */
	VOICE02_MENU_SELECT,		//	"メニュー選択", 	/* c				サコ死に音 + 弾消し音 + メニュー	[ちゅ] */
	VOICE03_BOSS_HAKAI, 		//	"ボス退場", 		/*[d];	[s(?)]		ボス死に音? 						[ぼがーーん] */
	VOICE04_SHIP_HAKAI, 		//	"ピチューン",		/* e :				自機破壊音							[ぴちゅーん] */
	VOICE05_BONUS,				//	"アイテム取得", 	/* f				アイテム取得音						[かつん] */
	VOICE06_EXTEND, 			//	"増えた",			/* g :				残機増加音							[びゅわわわわん] */
	VOICE07_BOMB,				//	"自分カード",		/*[h]	[s(?)]		音									[かしゅるしゃしゃん] */
//
	VOICE08_DEATH,				//	"ザコ倒した",		/* i				サコ死に音専用						[どすーーー] */
	VOICE09_GRAZE,				//	"グレイズ", 		/* j				グレイズ音							[か] */
	VOICE10_COUNT_TIMER,		//	"時間制限", 		/* p :				時間制限カウント音					[へにょん] */
	VOICE11_BOSS_KIRARIN,		//	"キラリン☆",		/* l				きらりん音 */
//
	VOICE12_MAGICF_E_SHOT,		//	"ボスショット1",	/* k				音									[ふわ] */
	VOICE13_SAKUYA_B2_SHOT, 	//	"ボスショット2",	/* m				咲夜 音 							[ぱあん] */
	VOICE14_SAKUYA_SHOT02,		//	"ボスショット3",	/* n				咲夜じゅわショット音				[ちゅるちゅるちゅる] */
	VOICE15_BOSS_KOUGEKI_01,	//	"ボスショット4",	/*[o];	[s(?)]		ボス攻撃音? 						[ばあん] */
	VOICE16_BOSS_KYUPIN,		//	"ボスショット5",	/* l				キュピーン音 */
//
	VOICE17_BOSS_TAME01,		//	"ボス溜め1",		/* r				うぃーん音 */
	VOICE18_MAX_FILES,
};
	/* 本家に低音系の効果音は無い＝＝合わないので廃止。 咲夜 音 [どどどどど] */


enum	//; 曲(Music) [.it や .ogg形式]
{
	BGM_00_stop = 0,	/* ----- BGM 演奏停止コマンド */
	BGM_01_stage1,		/* th12_02.IT 1面道中 春の湊に */
	BGM_02_stage2,		/* th05_06.IT 2面道中 ロマンティック・チルドレン */
	BGM_03_stage3,		/* th08_13.IT 3面道中 ヴォヤージュ1969 */
	BGM_04_stage4,		/* th10_08.IT 4面道中 フォールオブフォール ～ 秋めく滝 */
	BGM_05_stage5,		/* th06_08.IT 5面道中 ヴアル魔法図書館 */
	BGM_06_stage6,		/* th06_10.IT 6面道中 メイドと血の懐中時計 */
	BGM_07_stage7,		/* th00_01.IT エンディング(道中) [夢違科学世紀] 童祭 ～ Innocent Treasures */
	BGM_08_stage8,		/* th06_04.IT エキストラステージ道中 ルーネイトエルフ */
	BGM_09_stage9,		/* th07_04.IT ファンタズムステージ道中 遠野幻想物語 */
	BGM_10_menu04,		/* th06_06.IT 上海紅茶館 */
	BGM_11_boss01,		/* th07_07.IT 1面ボス (アリス) 人形裁判 ～ 人の形弄びし少女 */
	BGM_12_boss02,		/* th02_11.IT 2面ボス (魅魔) コンプリート・ダークネス */
	BGM_13_boss03,		/* th08_15.IT 3面ボス (輝夜) 竹取飛翔 ～ Lunatic Princess */
	BGM_14_boss04,		/* th00_11.IT 4面ボス (文) [文花帖] 風神少女 (Full Version) */
	BGM_15_boss05,		/* th06_09.IT 5面ボス (パチェ) ラクトガール ～ 少女密室 */
	BGM_16_boss06,		/* th06_11.IT 6面ボス (咲夜) 月時計 ～ ルナ・ダイアル */
	BGM_17_boss07,		/* th06_05.IT チルノ エキストラステージ1ボス おてんば恋娘 */
	BGM_18_boss08,		/* th05_17.IT アリス エキストラステージ2ボス グリモア オブ アリス */
	BGM_19_boss09,		/* th08_10.IT 魔理沙 エキストラステージ3ボス 恋色マスタースパーク */
	BGM_20_boss10,		/* th07_11.IT 妖夢 隠しボス 広有射怪鳥事 ～ Till When? */
	BGM_21_boss11,		/* th06_13.IT レミリア 隠しボス 亡き王女の為のセプテット */
	BGM_22_boss00,		/* th06_15.IT フラン 隠しボス U.N.オーエンは彼女なのか？ */
	BGM_23_boss12,		/* th04_10.IT 霊夢 少女綺想曲 ～ capriccio */
	BGM_24_boss13,		/* th06_03.IT ルーミア 妖魔夜行 */
	BGM_25_menu03,		/* th10_18.IT エンディング(シナリオ) プレイヤーズスコア */
	BGM_26_menu02,		/* th07_02.IT キーコンフィグ 無何有の郷 ～ Deep Mountain */
	BGM_27_menu01,		/* th07_01.IT タイトル画面 妖々夢 ～ Snow or Cherry Petal */
	BGM_28_stage10, 	/* th04_07.IT 使用曲です。 バッド・アップル ～ Bad Apple!! */
	BGM_29_stage11, 	/* th08_08.IT 使用曲です。 永夜の報い */
	BGM_30_stage12, 	/* th13_02.IT 使用曲です。 死霊の夜桜 */
	BGM_31_menu05,		/* th06_16.IT 紅より儚い永遠 */
	BGM_32_stage13, 	/* th07_15.IT 道中曲です。 妖々跋扈 */
	BGM_33_stage14, 	/* th07_06.IT 道中曲です。 ブクレシュティの人形師 */
/* ----- 曲の数(読み込みファイル数) */
	USE_31_MUSIC_FILES
};

/*(ゲーム中のトラック設計)*/
enum
{
	TRACK00_JIKI_SHOT = 0,	// "自機弾"用。(ゲーム中は、常に発音しているので、他用途に使えない。基本的に専用)
	TRACK01_PICHUN, 		// "自機ピチューン音" / "自機増えた音"用。
	TRACK02_JIKI_BOMBER,	// "自機ボム"用。
	TRACK03_IVENT_DAN,		// (敵弾)"敵弾目立たせたい用"/"目立たせたいイベント"用。
	TRACK04_TEKIDAN,		// (敵弾)"敵弾通常用"
	TRACK05_ZAKO_DEATH, 	// "雑魚破壊"用。(現在専用)
	TRACK06_ITEM,			// 自機"アイテム"取得用。
	TRACK07_GRAZE_AUDIO_DRIVER_INTERNAL/*(voice_play_graze();以外で、外部から使用しても正常に鳴らない)*/
};
#if (1)/*(検索出来るように、エイリアス[別名]定義する。)*/
	/*(メニューのトラック設計)*/
	#define TRACK01_MENU01 TRACK01_PICHUN
	#define TRACK02_MENU02 TRACK02_JIKI_BOMBER
#endif

extern void init_audio(void);
extern void exit_audio(void);

extern void set_voice_volume(int volume);
extern void set_music_volume(int volume);

//extern void stop_music(void);/*(廃止)*/
//#define stop_music(aaa)	play_music_num(BGM_00_stop)
extern void fadeout_music(void);
extern void pause_music(void);
extern void resume_music(void);

extern void play_music_num(int num);

extern void play_voice_auto_track(int req_idx);
extern void bullet_play_04_auto(int req_idx);
extern void voice_play_graze(void); 	/* グレイズ音に関しては、専用の発音処理が必要。 */


extern void voice_play_vbl(void);
extern void voice_play(int req, int play_track );

/* 効果音の音響設計を行う(トラック割り振り) */
#define USE_DESIGN_TRACK	(1)

#endif /* _BGM_VOICE_H_ */





/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/
// "000_my_math.h" より後の必要あり。

#ifndef _GAME_FILE_H_
#define _GAME_FILE_H_

/*---------------------------------------------------------
	ディレクトリ名称
	-------------------------------------------------------
	ディレクトリ名称は変わる可能性があるから game_main.h で定義
---------------------------------------------------------*/

#define DIRECTRY_NAME_DATA_STR			"data"
/* 'data' の文字数は 4 文字 */
#define DIRECTRY_NAME_DATA_LENGTH		(4)

#define DIRECTRY_NAME_KAIWA_STR 		"/kaiwa/"
/* '/kaiwa/' の文字数は 7 文字 */
#define DIRECTRY_NAME_KAIWA_LENGTH		(7)

#define DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR			".txt"

/*
メモ:
/fonts/ の場合、/src/core/font.c のみ。
	"font01.png"
	"font03.png"
	"font05.png"
/fonts/ の場合、/src/core/draw/draw_kanji.c のみ。


/fonts/ の場合、/src/core/draw/score_panel.c のみ。
	"panel_base.png"

/kaiwa/s/ の場合、/src/core/load_stage.c のみ。
/kaiwa/0/ の場合、/src/core/menu/kaiwa.c のみ。

*/
/*---------------------------------------------------------
	スコアフォーマット
---------------------------------------------------------*/

typedef struct
{
	char name[16/*4*/];
//
	u32 score;
	int player;
	int final_stage;	/* 到達ステージ */
	int difficulty; 	/* 難易度 */
//
	int use_continue;	/* コンティニュー回数 */
	int count_miss; 	/* ミス回数 */
	int used_bomber;	/* ボム使用回数 */
	int count_bonus;	/* カードボーナス回数 */
} SCORE_FORMAT;

#define MAX_8_SAVE_PLAYERS	(8)
#define MAX_5_RANKING		(5)

extern SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING];

/*---------------------------------------------------------
	オプションメニューで設定する設定値
---------------------------------------------------------*/

enum /*_option_config_*/
{
	OPTION_CONFIG_00_PLAYER = 0,
	OPTION_CONFIG_01_BOMB,
	OPTION_CONFIG_02_BGM,
	OPTION_CONFIG_03_SOUND,
	OPTION_CONFIG_04_CURRENT_DIFFICULTY,
	OPTION_CONFIG_05_CURRENT_PLAYER,
	OPTION_CONFIG_06_ANALOG,
	OPTION_CONFIG_07_OPEN,
	OPTION_CONFIG_08_MAX	/* 最大数 */
};

extern int option_config[OPTION_CONFIG_08_MAX]; 	/* 8 */ 	// 数字=ボタン変数

/*---------------------------------------------------------
	コンフィグ
---------------------------------------------------------*/

extern u32 pad_config[KINOU_08_WARIATE_MAX];
//extern int ke yconfig[KINOU_08_WARIATE_MAX];

/*---------------------------------------------------------
	背景読みこみ
---------------------------------------------------------*/

enum /*_bg_resource_*/
{
	BG_TYPE_00_title_bg = 0,
	BG_TYPE_01_name_regist,
	BG_TYPE_02_key_config,
	BG_TYPE_03_music_room,
	BG_TYPE_04_loading, 		/*(蓮池の画像)*/
	BG_TYPE_99_MAX_HAIKEI,
};
//extern void load_S DL_bg(int bg_type_number);/*([廃止] my_file_common_name[0] = bg_type_number;psp_load_bg_file_name();で互換)*/
extern void psp_load_bg_file_name(void);


#endif /* _GAME_FILE_H_ */


/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _SDL_USE_FONT_H_
#define _SDL_USE_FONT_H_

/*---------------------------------------------------------
	フォント
---------------------------------------------------------*/

//enum FONTS
//{
//	FONT10W = 0,		/*	8x10 x WHITE 白 (mini) */
//	FONT16B,			/* 16x16 x 漢字のＢＧ用  */
//	FONT16K,			/* 16x16 x キーコンフィグ用 */
//	FONT16W,			/* 16x16 x WHITE 白 */
//	FONT_MAX
//};
/*(-r34互換用)*/	/* 16x16 x RED   紅(廃止) */
//#define FONT16R 	FONT16W


/* サーフェイスを作らないで、直接画面に表示 */
//extern void font88_print_screen_xy(void);//	char *text, int font_type, int x, int y);
extern void font_render_view_screen(void);

/* [廃止予定]新規サーフェイスを作成し、文字列をレンダリング */
extern SDL_Surface *font16_render_new_surface(void);//	char *text, int font_type);

/* */
extern void font_print(void);// 			char *text, int font_type, int x, int y);

#define MY_FONT_256_TEXT_LENGTH 		(256)
extern char my_font_text[MY_FONT_256_TEXT_LENGTH];

#endif /* _SDL_USE_FONT_H_ */
