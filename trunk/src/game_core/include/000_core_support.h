
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
	BGM_01_stage1,		/* 1面道中 */
	BGM_02_stage2,		/* 2面道中 */
	BGM_03_stage3,		/* 3面道中 */
	BGM_04_stage4,		/* 4面道中 */
	BGM_05_stage5,		/* 5面道中 */
	BGM_06_stage6,		/* 6面道中 */
	BGM_07_stage7,		/* エンディング(道中) */
	BGM_08_stage8,		/* エキストラステージ道中 */
	BGM_09_stage9,		/* ファンタズムステージ道中 */
	BGM_10_menu04,		/* 上海紅茶館 */
	BGM_11_boss01,		/* 1面ボス (アリス) */
	BGM_12_boss02,		/* 2面ボス (魅魔) */
	BGM_13_boss03,		/* 3面ボス (輝夜) */
	BGM_14_boss04,		/* 4面ボス (文) */
	BGM_15_boss05,		/* 5面ボス (パチェ) */
	BGM_16_boss06,		/* 6面ボス (咲夜) */
	BGM_17_boss07,		/* チルノ エキストラステージ1ボス */
	BGM_18_boss08,		/* アリス エキストラステージ2ボス */
	BGM_19_boss09,		/* 魔理沙 エキストラステージ3ボス */
	BGM_20_boss10,		/* 妖夢 隠しボス */
	BGM_21_boss11,		/* レミリア 隠しボス */
	BGM_22_boss00,		/* フラン 隠しボス */
	BGM_23_boss12,		/* 霊夢 */
	BGM_24_boss13,		/* ルーミア */
	BGM_24_menu03,		/* エンディング(シナリオ) */
	BGM_25_menu02,		/* キーコンフィグ */
	BGM_26_menu01,		/* タイトル画面 */
	BGM_27_stage10,		/* 未使用曲です。 */
/* ----- 曲の数(読み込みファイル数) */
	USE_28_MUSIC_FILES
};

enum
{
	TRACK00_BULLETS = 0,
	TRACK01_EXPLODE,
	TRACK02_ALEART_IVENT,
	TRACK03_SHORT_MUSIC,
	TRACK04_TEKIDAN,
	TRACK05_ZAKO_DEATH,
	TRACK06_ALEART_IVENT_02,
	TRACK07_GRAZE_AUDIO_DRIVER_INTERNAL/*(voice_play_graze();以外で、外部から使用しても正常に鳴らない)*/
};

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
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
	"font_bg16.png"
/fonts/ の場合、/src/core/draw/score_panel.c のみ。
	"panel_base.png"
	"hosi_gauge.png"
/douchuu/ の場合、/src/core/load_stage.c のみ。
/kaiwa/ の場合、/src/core/menu/kaiwa.c のみ。

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
extern void load_SDL_bg(int bg_type_number);


#endif /* _GAME_FILE_H_ */








/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _SDL_USE_FONT_H_
#define _SDL_USE_FONT_H_

/*---------------------------------------------------------
	フォント
---------------------------------------------------------*/

enum FONTS
{
	FONT10W = 0,		/*	8x10 x WHITE 白 (mini) */
	FONT16B,			/* 16x16 x 漢字のＢＧ用  */
	FONT16K,			/* 16x16 x キーコンフィグ用 */
	FONT16W,			/* 16x16 x WHITE 白 */
	FONT_MAX
};
/*(-r34互換用)*/	/* 16x16 x RED   紅(廃止) */
#define FONT16R 	FONT16W

extern void font_init(void);

/* サーフェイスを作らないで、直接画面に表示 */
extern void font88_print_screen_xy(void);//	char *text, int font_type, int x, int y);

/* [廃止予定]新規サーフェイスを作成し、文字列をレンダリング */
extern SDL_Surface *font_render_new_surface(void);//	char *text, int font_type);

/* */
extern void font_print(void);// 			char *text, int font_type, int x, int y);

#define MY_FONT_256_TEXT_LENGTH 		(256)
extern char my_font_text[MY_FONT_256_TEXT_LENGTH];

#endif /* _SDL_USE_FONT_H_ */
