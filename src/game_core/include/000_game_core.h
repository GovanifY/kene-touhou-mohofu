
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _JIKI_H_
#define _JIKI_H_

// > 【PSP】PSPで東方やりたい奴8【弾幕】
// > 200 ：名無しさん＠お腹いっぱい。：2009/01/27(火) 18:41:00 ID:xQgI7hCU
// > そろそろSアイテム消して欲しいな。
// > あとFULLパワー時のアイテム引き寄せラインはもうちょっと下の方がいいと思う。
// > 具体的にはこの辺り↓
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// この意見はわたしも同様に思ったので、みんながそう思ったに違いない。
// この画像は484x283[dots]で、y==63[line]に赤線が引いてあった。
// pspの解像度は、 480x272[dots]なので x/272=63/283, x=(63*272)/283 == 60.55[line]
// 差分/製作氏	50(49)[line] 少し上すぎる。
// スレ8の200氏 約60[line] (感覚的に)
// ここでキャラの高さ分もあるし、64(63)かもっと下72(71)ぐらいでもいいのでは？という訳で64にして実験してみる。
// 結果：やっぱもう少し下がいい。8[dots]といわず16[dots]ぐらい下がいい。再び実験してみる。
// 結果：こんなもんかな？というわけで80(79)[dots]に決定。もう8[dots]下88(87)でもいいかもね。
// 本家の感覚を大事にするならこんなもんだと思う。
//---------------
//「アイテム引き寄せライン」以下で取ると「アイテム引き寄せライン」から322ラインまで距離で減点。
//
//プレイフィールドは 384x448 なので、448-322 == 126
//「アイテム引き寄せライン」は多分上から128ドットかな？
//
//(Bアイテムの大きさが16x16[dot]なので、Bアイテムの消失判定は 448+(16/2)ラインで行うから、
//最後にBアイテムの判定があるのが、BアイテムのＹ軸中心が456[ライン]にきた場合。
//456-322 == 134, 134-128 == 6. この6[ライン]はそもそも自機中心が画面下部に行けない分が数ドット(5[dot?])ある。
//残り1[dot]は不等号で判定したから? )
//---------------
// もし同比率なら、
// (模倣風アイテム引き寄せライン)/(本家アイテム引き寄せライン) == (模倣風ゲームフィールド縦)/(本家ゲームフィールド縦)
// が成立する。(模倣風アイテム引き寄せライン)が知りたいのだから移行して、
// (模倣風アイテム引き寄せライン) == (本家アイテム引き寄せライン) * (模倣風ゲームフィールド縦)/(本家ゲームフィールド縦)
// (模倣風アイテム引き寄せライン) == 128 * 272/448 == 77.7142857142857142857142857142857 =:= 78[dots] 単純な比率計算の場合。
// http://hossy.info/game/toho/k_score.php ここの画像を逆算して計算すると 77 dots あたり。
/* アイテム引き寄せライン */
//#define ITEM_GET_BORDER_LINE256		(t256(77))/* 画像から仮定 */
//#define ITEM_GET_BORDER_LINE256		(t256(78))/* 本家と同比率と仮定し逆算 */
#define ITEM_GET_BORDER_LINE256 		(t256(80))/* 中心座標で管理してない奴(-r31) */
/* 現状(r33) プレイ感覚としてこの3つ(77, 78, 80)の中では (80)が一番本家に近い。(77, 78)は上過ぎる。
あまり下にしても面白くないから、(80)程度が妥当な気がする。 */

/* 敵が撃たないライン (このラインより下からは敵が撃たない) */
#define ENEMY_LAST_SHOT_LINE256 	(t256(272-72))


enum /*_select_player_*/
{
	/*0*/	REIMU_A = 0,
	/*1*/	REIMU_B,
	/*2*/	MARISA_A,
	/*3*/	MARISA_B,
//
	/*4*/	REMILIA,
	/*5*/	YUYUKO,
	/*6*/	CIRNO_A,
	/*7*/	CIRNO_Q,
//
	MAX_08_PLAYER
};

/*(全フラグを消す特殊機能)*/
#define JIKI_FLAG_0x00_ALL_OFF							(0x0000)
//--------
/*(割り込みシステム(r36新規)用に確保)*/
#define II_SYSTEM_TASK_NUMBER							(0x000f)
//--------
/* プレイヤーが上部へ移動し、手動でアイテム収集行う場合にon。 */
#define JIKI_FLAG_0x01_JYOU_BU_SYUU_SYUU				(0x0010)
/* ボムが発動した際に、自動でアイテム収集を行う場合にon。 */
#define JIKI_FLAG_0x02_BOMBER_SYUU_SYUU 				(0x0020)
/* ボス倒し後に点数(★アイテム)やボスが出すアイテムを自動で収集する。この場合にon。 */
#define JIKI_FLAG_0x04_BOSS_GO_ITEM_JIDOU_SYUU_SYUU 	(0x0040)
//-------
/* ★ [プレイヤーのオプションがショットを撃つ場合の通信用フラグ] */
#define JIKI_FLAG_0x08_SHOT_KEY_SEND_FOR_OPTION 		(0x0080)
/* 1==オプションがショットを撃つ。 */
/* 0==オプションがショットを撃たない。 */
//-------
/* ゲーム中、会話モードで on になる。ゲームが再開されると off になる。*/
#define STATE_FLAG_06_IS_KAIWA_MODE 					(0x0100)
/* on */
/* ボスを倒した場合にon(時間切れではOFF) */
//#define STATE_FL AG_09_IS_WIN_BOSS					(0x0200)
/* on */
#define STATE_FLAG_10_IS_LOAD_KAIWA_TXT 				(0x0400)
/* on */
#define STATE_FLAG_12_END_KAIWA_MODE					(0x0800)
/* on */
//-------
#define STATE_FLAG_13_DRAW_BOSS_GAUGE					(0x1000)

#define STATE_FLAG_14_DOUCHU_TUIKA						(0x2000)/* 道中でコマンド追加紺処理を行う場合on */

/* ボスと戦う際にon(ボス前イベントではOFF) */
#define STATE_FLAG_05_IS_BOSS							(0x4000)

#define STATE_FLAG_16_GAME_TERMINATE					(0x8000)/* result後にゲーム終了 */


//#define ST ATE_FLAG_11_IS_BOSS_DESTROY				(0x0400)/* 廃止 */

//#define STATE_FLAG_0123_AUTO_GET_ITEM (JIKI_FLAG_0x01_JYOU_BU_SYUU_SYUU|JIKI_FLAG_0x02_BOMBER_SYUU_SYUU|JIKI_FLAG_0x04_BOSS_GO_ITEM_JIDOU_SYUU_SYUU)
//#define ST ATE_FLAG_14_GAME_LOOP_QUIT 				(0x2000)
//#define ST ATE_FLAG_16_NOT_ALLOW_KEY_CONTROL			(0x8000)/* 廃止 */


//	/*extern*/global void *callback_gu_draw_haikei_modosu;	/* (カード時から通常時へ)戻す用 */
//	/*extern*/global void *callback_gu_draw_haikei_supeka;	/* カード時用 */
//	global void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;
//	extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */
enum
{
	SDL_00_VIEW_SCREEN		= 0,
	SDL_01_BACK_SCREEN,
//	SDL_02_TEX_SCREEN,
//	SDL_03_000_SCREEN,
	SDL_99_MAX_SCREEN,
};

typedef struct _game_core_global_callback_
{
	void (*main_call_func)(void);/* メインコールバック */
	void (*return_call_func)(void);/* ポーズ復帰後の戻り先コールバック */
	/* カード時でない通常時のプログラムを指示 */
	void *callback_gu_draw_haikei_modosu;	/* (カード時から通常時へ)戻す用。(==通常時) */
	/* カード時のプログラムを指示 */
	void *callback_gu_draw_haikei_supeka;	/* カード時用 */

	/* 現在使用するプログラムを設定 */
	void (*callback_gu_draw_haikei)(void);	/* ゲーム中、背景ウィンドウ表示フラグ */
/*
	文字を描画する描画先のテクスチャー
*/
		#if (0==USE_32BIT_DRAW_MODE)
	u16 *kanji_window_screen_image; 					/* メッセージウィンドウ(画像) */
		#else
	u32 *kanji_window_screen_image; 					/* メッセージウィンドウ(画像) */
		#endif
//extern SDL_Surface *screen;
/*extern*/ SDL_Surface *sdl_screen[SDL_99_MAX_SCREEN];/*(4)*/

} GAME_CORE_GLOBAL_CALLBACK;
extern GAME_CORE_GLOBAL_CALLBACK cb;
typedef struct _game_core_global_class_
{
	int state_flag; 	/* 設定フラグ(集) */
	int weapon_power;	/* 0x00-0x80  (0-128 の129段階==本家と同じ)   max==128==「129段階」*/
	int chain_point;
	int bomber_time;	/* Use Gu */
//[0x10 4]
	u32 game_score; 	/* スコア得点 */
	u32 graze_point;	/* グレイズ得点 */
	int bombs;			/* ボム数 */
	int zanki;			/* 残りチャンス */
//[0x20 8]
	s32 game_difficulty;/* cg.game_difficulty: GAME_CORE_GLOBAL_CLASSに入れると速度低下する。(pspのmax,min命令を使っているので signed int の必要がある ) */
	s32 game_rank;		/* ランク変動システムのゲームランク */
	/*u8*/unsigned int jiki_weapon_level_offset;/* 装備した武器レベル(番号)の８倍にプレイヤー値を足した値をオフセットとして保持 */
	unsigned int conv_bg_alpha;
//[0x30 12]
	int current_bg0_y_scroll_speed256;	/* bg0のスクロール、現在速度 */
	/*(描画用)*/
	int draw_boss_hp_value; /* ボスhp描画値 */ ///??????????
	int dummy111;/*無いと巧く動作しない*/
//	/* 集計システム(player_data) */
	int player_data_use_continue;	/* コンティニュー回数 */
//[0x40 16]
	int player_data_count_miss; 	/* ミス回数 */
	int player_data_used_bomber;	/* ボム使用回数 */
	int player_data_use_kurai_bomb; /* 喰らいボム成功回数 */
	int player_data_count_bonus;	/* カードボーナス回数 */
//[0x48 18]
	#if (1)/*(仮対応)*/
	/* 暫定 SDL フォント */
	int SDL_font_type;
	int SDL_font_x;
	int SDL_font_y;
//[0x60 24]
//	暫定 [ラインレンダーに移行予定。]
	int msg_time;/* メッセージ(仮対応)表示時間 */	/* せりふウィンドウ表示時間(仮) */
	#endif
//[0x50 20]
//	この場所は 4[byte]境界ではダメで、何故か32[byte]境界に合わせないと正常動作しない。(4[byte]境界では異常動作する)
	s8 game_now_stage;				/* 現在ステージ番号 */
	s8 game_continue_stage; 		/* 現在コンティニューするステージ番号を保持 */
	s8 game_now_max_continue;		/* コンティニュー可能な回数 */
	s8 game_practice_mode;			/* 練習モード */
//
	s8 side_panel_draw_flag;/* パネル表示on(0以外)/off(0) */
	s8 chuu_boss_mode;
	s8 dummy_status;				/* [未使用]ダミー */
	u8 player_option_mode;/* ここに入れない方がいいかも？、とりあえず1575659 */
	int dummy2222;/*無いと何故か巧く動作しない*/
	int dummy3333;/*無いと何故か巧く動作しない*/
} GAME_CORE_GLOBAL_CLASS;
extern GAME_CORE_GLOBAL_CLASS cg;
// /* 予備2(.align合わせで必要) */
//	s8 dr aw_flag_ka iwa_screen;		/* 会話画面の会話用せりふウィンドウ表示フラグ */



//
//	u8 laser_mode;	/* てすと(レーザーモード 0:off, 1:on) */
	/* 集計システム以外の保持状態 */
//	u8 game_select_player;
//遅過ぎる。	u8 game_difficulty/* = RANK_EASY*/; 	/*	RANK_NORMAL*/
//遅過ぎる。	int game_difficulty;







/* 意図的に入れないもの */
extern unsigned int cg_game_select_player;/* cg.game_difficulty: (将来はともかく)現状(r33)は GAME_CORE_GLOBAL_CLASSに入れない方が良いっぽい。 */

	#if (1)/*(r34)*/
typedef struct _psp_pad_global_class_
{
	u32 pad_data;						/*今回入力*/
	u32 pad_data_alter; 				/*前回入力*/
	s16 analog_absolute_value_x;		/* アナログ量、補正済み */
	s16 analog_absolute_value_y;		/* アナログ量、補正済み */
} PSP_PAD_GLOBAL_CLASS;
extern PSP_PAD_GLOBAL_CLASS psp_pad;
	#endif

#if 1

	#if (0)/* アライメント関係(???) (s16)で GAME_CORE_GLOBAL_CLASSに入れると巧くいかない */
	/* 意図的に入れないもの */
	extern	u32 cg_m y_pad; 		/*今回入力*/
	extern	u32 cg_m y_pad_alter;	/*前回入力*/
	extern	s16 cg_analog_x;		/* アナログ量、補正済み */
	extern	s16 cg_analog_y;		/* アナログ量、補正済み */
	#endif
//
//??????????extern int draw_boss_hp_value;	/* ボスhp描画値 */
//extern int bo ss_life_value;	/* ボスhp体力値 / ボス魔方陣サイズ描画値 */
#endif
extern int boss_x256;
extern int boss_y256;


#define USE_HOLD_GAME_MODE	(0)
#if (1==USE_HOLD_GAME_MODE)
extern void hold_game_mode_on(void);/* ゲーム時間の一時停止 */
extern void hold_game_mode_off(void);/* ゲーム時間の動作開始 */
#endif

//#define IS_KEY BOARD_PULLED ((0==psp_pad.pad_data)&&((psp_pad.pad_data)^(psp_pad.pad_data_alter)))/* 何かキーを離されたら */
/*	キーを離した瞬間が確定とする。押した瞬間が確定だと、押された間だけ動作する物が同時にあると都合が悪い。 */
/*
	r31:原作ではキーを押した瞬間が確定のものが多いので、おかしいところはキーを押した瞬間が確定に修正した。
*/

/* 127 かと思っていたけど、128みたい(つまり129段階) */
//#define MAX_POWER_IS_128 (127)/* 0x00-0x7f  (0-127 の128段階) */
//#define MAX_POWER_IS_128 (128)/* 0x00-0x80  (0-128 の129段階) */
#define MAX_POWER_IS_128 (128)

//typedef struct
//{
//	int strength;
//} WEAPON_BASE;

//#define DEFAULT_MAX_CONTINUE	(3)
//#define DEFAULT_MAX_CONTINUE	(30)
#define DEFAULT_MAX_CONTINUE	(30)


#endif /* _JIKI_H_ */


#ifndef _BONUS_H_
#define _BONUS_H_

enum
{
	SCORE_76800 = 0,
				SCORE_10,	/* (MAX時以外の) [P]は10点 */
							SCORE_20,	SCORE_30,
	SCORE_40,	SCORE_50,	SCORE_60,	SCORE_70,
	SCORE_80,	SCORE_90,	SCORE_100,	SCORE_200,
	SCORE_300,	SCORE_400,	SCORE_500,	SCORE_600,
	SCORE_700,	SCORE_800,	SCORE_900,	SCORE_1000,
	SCORE_2000, SCORE_3000, SCORE_4000, SCORE_5000,
	SCORE_6000, SCORE_7000, SCORE_8000, SCORE_9000,
	SCORE_10000,SCORE_11000,SCORE_12000,SCORE_51200/*(最大値)*/
};

#define ITEM_MOVE_FLAG_00_NONE			0x00
#define ITEM_MOVE_FLAG_01_COLLECT		0x01
#define ITEM_MOVE_FLAG_02_RAND_X		0x02
#define ITEM_MOVE_FLAG_04_RAND_Y		0x04
#define ITEM_MOVE_FLAG_06_RAND_XY		(ITEM_MOVE_FLAG_02_RAND_X|ITEM_MOVE_FLAG_04_RAND_Y)

#define ITEM_CREATE_MODE_01 	((4*8/*ITEM_MAX*/)*0)
#define ITEM_CREATE_MODE_02 	((4*8/*ITEM_MAX*/)*1)

/*(アイテム作成系)*/
//廃止extern void item_create(			SPRITE *src, int type, int num, int up_flags);
extern void item_create_mono(			SPRITE *src, int type);
extern void item_create_num(			SPRITE *src, int type, int num_of_creates );
extern void item_create_for_boss(		SPRITE *src, int item_create_mode);
extern void item_create_random_table(	SPRITE *src);

/*()*/
extern void bonus_info_score_nodel( 		SPRITE *src, int score_type);		/*int x, int y*/
extern void bonus_info_any_score_nodel( 	SPRITE *src, u32 score_num_pts);	/*int x, int y*/

/*(弾を全て星点に変換する)*/
extern void bullets_to_hosi(void);

/*(スコア加算する場合は、カンスト チェックがあるので、必ずここを使う)*/
extern void player_dummy_add_score(u32 score_num_pts);
extern void player_add_score(u32 score_num_pts);
#if (1==USE_EXTEND_CHECK)
extern void player_check_extend_score(void);/*(エクステンドチェック)*/
#endif /* (1==USE_EXTEND_CHECK) */

/*(難易度スコア補正)*/
extern u32 adjust_score_by_difficulty(u32 convert_score);

#endif /* _BONUS_H_ */



#ifndef _LOAD_STAGE_FILE_H_
#define _LOAD_STAGE_FILE_H_

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_36		/*64*/(64-20-8-4+32)/*(64-(4*5))*/

typedef struct _stage_command_
{
	u32 		v_time; 				/* 正数(0<v_time)の場合 1/60 単位の出現時間カウンタ。(-1)の場合出現済み。(0の場合error何もしない。) */
	struct _stage_command_ *next;
	u32 		user_x; 				/* x */
	int 		user_y; 				/* y */
//(16[Bytes] == (4x4) )
	u8			user_i_code;			/* intermediate code 中間コード */
	u8			user_255_code;			/* 'E'とか'T'とか	*/
	u8			user_item8; 			/* item */
	u8			user_select_gazou;		/* アニメ/画像種類 */
	int 		user_hp;				/* hp */
	int 		user_score; 			/* score */
	int 		user_kougeki_type;		/* 敵が攻撃する場合のカードの種類 */
//(20[Bytes] == (4x5) )
	char		user_string[MAX_PARA1_36/*(64)*/];/* strings */
//(64[Bytes] == (4x4)+(4)+(44) )
} GAME_COMMAND; 	/* .align 64 [bytes] */
//	u16 	dummy_scroll_speed256;	/* para3 scroll speed256 */


/* 中間コード形式のコマンド */
/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

enum /*_game_command_code_*/
{
/*中間コード*/
/*0x00*/	GC_CODE_00_SJIS_TEXT = 0,		/* 漢字表示[番兵区切りとしても必要] */
/*0x01*/	GC_CODE_01_ALL_CLEAR,			/* ゲーム 全ステージ クリアー */
/*0x02*/	GC_CODE_02_BG_CONTROL,			/* 背景コントロール。(スクロール速度等) */
/*0x03*/	GC_CODE_03_CHECK_SECRET_BONUS,	/* 隠しボーナスチェック "出力" */
/*0x04*/	GC_CODE_04_BEGIN_SECRET_BONUS,	/* 隠しボーナスチェック "確認" */
/*0x05*/	GC_CODE_05_BOSS_COMMON, 		/* 敵 (ボス / 中-ボス)用 スク リプト起動 */
/*0x06*/	GC_CODE_06_CHUU_BOSS_COMMON,	/* 敵 ザコ中-ボス(スク リプト起動しない) */
/*0x07*/	GC_CODE_07_ZAKO,				/* 敵 ザコ */
			//
/*0x08*/	/* 拡張可能 */
/*...*/ 	/* ... */
/*0x2f*/	/* 拡張可能 */
/*0x30*/	GC_CODE_MAX,		/* 0x30 == '0' == カードコマンド (== 速度コマンド、廃止) */
};

#endif /* _LOAD_STAGE_FILE_H_ */


#ifndef _SPELL_SYSTEM_H_
#define _SPELL_SYSTEM_H_
/*
	スペルシステム==カードの管理システム。(カードシステムとは違うので注意)
*/
	/*---------------------------------------------------------
	スペルシステム(あらかじめカードの定義をしておくシステム)
	-------------------------------------------------------
	模倣風ではスペルはシステムでサポートしています。
	スペルとして定義(カード)しておくと、どのボスが、どのスペルでも撃てるようになります。
	-------------------------------------------------------
	またスペルはカードシステムに番号を登録しておくと、カードシステムでスペルが取得できます。
	(つまりカードシステム経由でスペルが撃てます)
	-------------------------------------------------------
	(r32)現在、雑魚(模倣風のdatの中-ボス)もスペルを撃てますが、
	この機能は将来なくなるかも知れません。
	雑魚(模倣風のdatの中-ボス)がスペルを撃てる機能は「風神禄」で思いついたのですが、
	こういう機能が無い方がゲームの速度が速く出来る気もするし、
	もしかしたらこういう機能は本家に無いのかもと思うこともあります。良く判りません。
	-------------------------------------------------------
	boss->boss_base_card_number に撃ちたいスペルをセットすると、スペル生成開始。
	スペル生成が終了すると自動的に boss->boss_base_card_number が DANMAKU_00 になる。
	-------------------------------------------------------
	スペル生成中は boss->boss_base_spell_time_out が勝手に毎フレーム減算され、0 になるとスペル生成終了。
	この時間はスペルごとに違う標準時間が設定されている。
	これはカードの初期化内で自動初期化される。
	-------------------------------------------------------
	カードの初期化。
	カードが変わると毎回行う必要がある。
	---------------------------------------------------------*/

/* カード番号(各スペルを区別する為の番号) */
enum
{
	SPELL_00 = 0,						/* スペル生成終了フラグ。 */
//	/* 雑魚用 */
	SPELL_01_sakuya_misogi_normal,		/* [咲夜] 紅5面中-ボス 禊カード / 紅5面中-ボス 咲夜 「通常攻撃」 */
	SPELL_02_sakuya_old_test,			/* [咲夜] 紅5面中-ボス 24カード (奇術「ミスディレクションもどき(1/2)」) */
	SPELL_03_sakuya_old_test,			/* [咲夜] 紅5面中-ボス 11カード (奇術「ミスディレクションもどき(2/2)」) */
	SPELL_04_sakuya_pink_hearts,		/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1(1/2)」にちょっとだけ似たカード(予定) */
	SPELL_05_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1/2(2/2)」にちょっとだけ似たカード(予定) */
	SPELL_06_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 幻象「ルナクロック(1/2)」にちょっとだけ似たカード(予定) */
	SPELL_07_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃3(1/2)」にちょっとだけ似たカード(予定) */
//
	SPELL_08_rumia, 					/* [ルーミア] 紅1面ボス [ルーミア] 夜符「ナイトバード」にちょっとだけ似たカード(予定) */
	SPELL_09_alice, 					/* [アリス] っぽい？テキトーカード */
	SPELL_0a_dai_yousei_01, 			/* [大妖精] 全周 48分割、時計回り、緑クナイ弾 */
	SPELL_0b_dai_yousei_02, 			/* [大妖精] 全周 48分割、反時計回り、赤クナイ弾 */
	SPELL_0c_sakuya_jack32, 			/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
	SPELL_0d_sakuya_miss_direction, 	/* [咲夜] 奇術「ミスディレクション」 */
	SPELL_0e_remilia_00,				/* [レミリア] テスト */
	SPELL_0f_tengu, 					/* [その他] 差分氏の妖怪1(天狗様?)が撃つカード(予定) */ 	/*???*/
//
	SPELL_10_cirno, 					/* [チルノ] 禊カード */
	SPELL_11_perfect_freeze,			/* [チルノ] パーフェクトフリーズ */
	SPELL_12_diamond_blizzard,			/* [チルノ] 雪符「ダイアモンドブリザード」 */
	SPELL_13_alice_suwako,				/* [アリス] 諏訪子っぽい(?)カード */
	SPELL_14_alice_youmu300,			/* [アリス] 妖3面ボス  normal通常攻撃3(の一部) */
	SPELL_15_alice_aka_2nd, 			/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
	SPELL_16_alice_doll,				/* [アリス] 妖3面ボス [アリス]人形カード(もどき) */ // 40[sec]
	SPELL_17_alice_doll_02, 			/* [アリス] 作成中 */
//
	SPELL_18_hana_test, 				/* [魅魔] 花てすとカード */
	SPELL_40_SONOTA_DEBUG_CW_AO,		/* [その他] デバッグカードCW青 */
	SPELL_41_SONOTA_DEBUG_CCW_AKA,		/* [その他] デバッグカードCCW赤 */
	SPELL_1b_cirno_icecle_fall, 		/* [チルノ] 氷符「アイシクルフォール」 */
	SPELL_44_ruiz3_test,				/* [ルイズ] カード語のてすと。 */
	SPELL_1d_amefuri_test,				/* [] 雨てすと */
	SPELL_1e_momiji_no_nonoji,			/* [椛] ののじカード */
	SPELL_1f_koakuma,					/* [小悪魔] てすと用 */
//	/* ボス用 */
	SPELL_20_aya_misogi1,				/*	1[文] の通常弾っぽいカード撃たせてみるテスト1 */
	SPELL_21_aya_misogi2,				/*	2[文] の通常弾っぽいカード撃たせてみるテスト2 */
	SPELL_22_ame_no_yatimata,			/*	3[文] 岐符「天の八衢」 */
	SPELL_23_meirin_magaru_kunai,		/*	4[美鈴] 通常攻撃 */
	SPELL_24_aya_doll,					/*	5[文] 人形カード */ //40[sec]
	SPELL_25_houka_kenran,				/*	6[紅美鈴] 華符「芳華絢爛」にちょっとだけ似たカード(予定) */
	SPELL_26_aya_saifu, 				/*	7[文] 塞符 */
	SPELL_27_hosigata_test, 			/*	8 [その他] 星型テスト */
//
	SPELL_28_remilia_tamaoki1,			/*	9[咲夜] 紅 面ボス  れみりゃっぽい咲夜の玉置カード(予定) */
	SPELL_29_rumia_demarcation, 		/* 10[ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似たカード(予定) */
	SPELL_2a_sakuya_baramaki1,			/* 11[咲夜] 紅5面中-ボス ばら撒き1 カード () */
	SPELL_2b_sakuya_baramaki2,			/* 12[咲夜] 紅5面中-ボス ばら撒き2 カード () */
	SPELL_2c_sakuya_blue_red,			/* 13[咲夜] 紅5面中-ボス 「通常攻撃3」青赤ナイフ */
	SPELL_2d_sakuya_misogi_lunatic, 	/* 14[咲夜] 紅5面中-ボス 禊カード / 紅5面中-ボス 咲夜 「通常攻撃」lunatic */
	SPELL_2e_pache_agni_shine_1,		/* [パチェ] "　　　　火符「アグニシャイン」" No.15 */	// agni shine (アグニの輝き)、 (アグニ==インド神話の火の神)
	SPELL_2f_pache_princess_undine, 	/* [パチェ] "　水符「プリンセスウンディネ」" No.16 */	// princess undine (ウンディネの姫)、(ウンディネ==錬金術師パラケルススの「妖精の書」に登場する水の精の名前)
//
	SPELL_30_pache_sylphy_horn_1,		/* [パチェ] "　　　　木符「シルフィホルン」" No.17 */	// sylphy horn (シルフィのホルン)、(シルフィ==風を司る精霊)、(ホルン==楽器==角笛)
	SPELL_31_pache_rage_tririton_1, 	/* [パチェ] "　　土符「レイジィトリリトン」" No.18 */	// rage tririton (怒るトリリトン)、 (トリリトン==世界遺産ストーンヘンジの三つ石)
	SPELL_32_pache_metal_fatigue,		/* [パチェ] "　　金符「メタルファティーグ」" No.19 */	// metal fatigue (金属疲労)
	SPELL_33_pache_agni_shine_2,		/* [パチェ] "　　火符「アグニシャイン上級」" No.20 */
	SPELL_34_pache_sylphy_horn_2,		/* [パチェ] "　　木符「シルフィホルン上級」" No.21 */
	SPELL_35_pache_rage_tririton_2, 	/* [パチェ] "土符「レイジィトリリトン上級」" No.22 */
	SPELL_36_pache_agni_radiance,		/* [パチェ] "　火符「アグニレイディアンス」" No.23 */	// agni radiance (アグニの輝き)、 (アグニ==インド神話の火の神)
	SPELL_37_pache_bury_in_lake,		/* [パチェ] "　　　水符「ベリーインレイク」" No.24 */	// bury in lake (水葬)
//
	SPELL_38_pache_green_storm, 		/* [パチェ] "　　　木符「グリーンストーム」" No.25 */	// green storm (緑の嵐)
	SPELL_39_pache_tririton_shake,		/* [パチェ] "　　土符「トリリトンシェイク」" No.26 */	// tririton shake (揺れるトリリトン)、 (トリリトン==世界遺産ストーンヘンジの三つ石)
	SPELL_3a_pache_silver_dragon,		/* [パチェ] "　　　金符「シルバードラゴン」" No.27 */	// silver dragon (銀翼竜)
	SPELL_3b_pache_lava_cromlech,		/* [パチェ] "火＆土符「ラーヴァクロムレク」" No.28 */	// lava cromlech (溶岩環状列石)
	SPELL_3c_pache_forest_blaze,		/* [パチェ] "木＆火符「フォレストブレイズ」" No.29 */	// forest blaze (森林火災)
	SPELL_3d_pache_water_elf,			/* [パチェ] "　水＆木符「ウォーターエルフ」" No.30 */	// water (水) / elf (妖精)	(ギャグ:水溶性==水に溶けちゃう妖精)
	SPELL_3e_pache_mercury_poison,		/* [パチェ] "金＆水符「マーキュリポイズン」" No.31 */	// mercury poison (水銀中毒)
	SPELL_3f_pache_emerald_megalith,	/* [パチェ] "土＆金符「エメラルドメガリス」" No.32 */	// emerald megalith (巨大なエメラルド石)
//
	SPELL_42_PACHE_LASER1,				/* [パチェ] 「通常攻撃1」のレーザーもどき */
	SPELL_43_PACHE_LASER2,				/* [パチェ] 「通常攻撃3」のレーザーもどき */
	SPELL_19_mima_sekkin,				/* [魅魔] 接近カード */
	SPELL_1a_mima_toge, 				/* [魅魔] トゲてすとカード */
	SPELL_1e_kaguya01,					/* [輝夜] むりやり変換1 */
	SPELL_1f_kaguya04,					/* [輝夜] むりやり変換2 */
	SPELL_1c_kakuya_tamanoe,			/* [輝夜]、蓬莱の玉の枝もどき。作成中 */
	SPELL_47_,							/* 未定 */
//
	#if (1)
	SPELL_48_r34_gokan_kinou, 			/* [r34]r34以前の互換機能。(r35-)システムで必ず必要。 */
	SPELL_49,							/* 未定 */
	SPELL_4a,							/* 未定 */
	SPELL_4b,							/* 未定 */
	SPELL_4c,							/* 未定 */
	SPELL_4d,							/* 未定 */
	SPELL_4e,							/* 未定 */
	SPELL_4f,							/* 未定 */
	#endif
//
	SPELL_MAX,
};
//	SPELL_1f_silent_selena, 			/* [パチェ] サイレントセレナ の てすと用 */
//


/* HATSUDAN_03_angle65536 に 自機狙い弾の角度を計算 */
extern void tmp_angleCCW65536_src_nerai(void);

/* HATSUDAN_03_angle65536 に 自機狙い弾の角度を計算 */
extern void calculate_jikinerai(void);


/* スペルをCPU実行し、カードを１フレーム生成する。 */
extern void card_generate(SPRITE *src);



#endif /* _SPELL_SYSTEM_H_ */


#ifndef _CARD_SYSTEM_H_
#define _CARD_SYSTEM_H_

//------------ カード関連

extern void create_card(SPRITE *src, int card_number);


//extern void ch eck_regist_card(SPRITE *src);/* カードの更新チェック */
extern void card_boss_move_generate_check_regist(SPRITE *src);	/* カード登録可能なら登録 / カード生成 */


// extern void card_state_check_holding(SPRITE *src);/* カードが終わるまで待つ。 */

enum
{
	CARD_BOSS_MODE_00_OFF = 0,			/* カードを使用しない(通常攻撃等)。(カードが撃てるかどうか判断) */
	CARD_BOSS_MODE_01_IDO_JYUNNBI,		/* 撃てる場合。発弾位置まで移動 */
	CARD_BOSS_MODE_02_TAIHI,			/* 退避中。 */
	CARD_BOSS_MODE_03_HATUDAN,			/* 発弾中。 */
};



enum
{
	DANMAKU_LAYER_00 = 0,			//(0)/* 弾幕コントロールしない通常弾(画面外で弾消し) */
	DANMAKU_LAYER_01,				//(1)/* 弾幕コントロールグループ(1)弾 */
	DANMAKU_LAYER_02,				//(2)/* 弾幕コントロールグループ(2)弾 */
	DANMAKU_LAYER_03,				//(3)/* 弾幕コントロールグループ(3)弾 */
	DANMAKU_LAYER_04_MAX/* 弾幕コントロールグループ最大数 */
};

typedef struct /*_card_global_class_*/
{
	int mode;				/* カードモード */
	int limit_health;		/* 規定値以下になればカードモード解除 */
	int boss_state; 		/* 負値になればボスがカードモードに入らない */
	int boss_timer; 		/* [共用]制限時間 */
	//
	int number; 			/* [共用]カード番号 */
	int number_temporaly;	/* [一時使用]カード番号 */
	int card_number;		/* */
	int boss_hp_dec_by_frame;/* ボス攻撃減少値、フレーム単位 */
	//
	void (*danmaku_callback[(DANMAKU_LAYER_04_MAX)])(SPRITE *sss);	/*	弾幕コールバックシステム(スペル変身処理) */
	// 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく)
	int/*u8*/ boss_hamidasi;			/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
} CARD_SYSTEM_GLOBAL_CLASS;
extern CARD_SYSTEM_GLOBAL_CLASS card;

//	int dummy2;
/* とりあえず */
#if 0
//	#define boss_base_resurved000				user_data10/*(r32)現在、未使用。*/
//	#define bo ss_base_state001 				user_data10/*廃止*/
	#define boss_base_spell_temporaly			user_data11
	#define boss_base_card_number				user_data12
	#define boss_base_spell_time_out			user_data13
#endif


//------------ "回"みたいなマークのエフェクト
/*
「"回"みたいなマーク」は、ボスが持ってるカードだそうです。
だから本当は、カードの枚数が減ったら、枚数を減らさなきゃいけないのかな？
でも本家そうなってないよね。
*/
/*static*/extern  void move_card_square_effect(SPRITE *src);
/*static*/extern  void boss_effect_initialize_position(void);

extern void danmaku_system_callback(void);
#endif /* _CARD_SYSTEM_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_

/* ボスで宣言が必要なもの(グローバル) */

enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*敵自体*/, int check_type);

/* カードシステム */

/* カードの初期化。カードが変わると毎回行う必要がある。 */
extern void card_maikai_init(SPRITE *src);						/*(毎回初期化)*/
extern void card_maikai_init_and_get_spell_number(SPRITE *src); /*(毎回初期化)+(現在撃つべく番号をカードシステムから取得)*/

// 共通形態
extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */

#endif /* _BOSS_H_ */


#ifndef _ATARI_HANTEI_H_
#define _ATARI_HANTEI_H_

enum
{
	BAKUHATSU_MINI00 = 0,/* 小爆発 */
	BAKUHATSU_MINI01,
	BAKUHATSU_MINI02,
	BAKUHATSU_MINI03,
//
	BAKUHATSU_ZAKO04,/* ザコ消滅爆発 */
	BAKUHATSU_ZAKO05,
	BAKUHATSU_ZAKO06,
	BAKUHATSU_ZAKO07,
//
	BAKUHATSU_FIRE08,/* 火炎爆発 */
	BAKUHATSU_FIRE09,
	BAKUHATSU_FIRE10,
	BAKUHATSU_FIRE11,
//
	BAKUHATSU_MOVE12,/* 移動爆発 */
	BAKUHATSU_MOVE13,
	BAKUHATSU_MOVE14,
	BAKUHATSU_MOVE15,
};
#define TAMA_ATARI_02					((2)<<8)
#define TAMA_ATARI_04					((4)<<8)

#define TAMA_ATARI_COMMON16_PNG 		TAMA_ATARI_02
#define TAMA_ATARI_MARU16_PNG			TAMA_ATARI_02
#define TAMA_ATARI_KNIFE18_PNG			TAMA_ATARI_02
#define TAMA_ATARI_JIPPOU32_PNG 		TAMA_ATARI_02

#define TAMA_ATARI_OODAMA_08_PNG		TAMA_ATARI_04



#define TAMA_ATARI_BULLET_DUMMY 		TAMA_ATARI_02
#define JIKI_ATARI_ITEM_16				((16)<<8)
#define JIKI_ATARI_ITEM_80				((80)<<8)


#define ZAKO_ATARI02_PNG				TAMA_ATARI_02
#define ZAKO_ATARI04_PNG				TAMA_ATARI_04
#define ZAKO_ATARI16_PNG				JIKI_ATARI_ITEM_16
#define ZAKO_ATARI_HOMING16_PNG 		JIKI_ATARI_ITEM_16


	/* 発弾エフェクトの時間分 */
	#define HATUDAN_FRAME64 	(64)

/**/extern int boss_hamidasi;		/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
typedef struct /*_clip_class_*/
{
	POINT256 boss_clip_min; 	/* ボス移動範囲(最小値) */
	POINT256 boss_clip_max; 	/* ボス移動範囲(最大値) */

	POINT256 bullet_clip_min;	/* 弾の範囲(最小値) */
	POINT256 bullet_clip_max;	/* 弾の範囲(最大値) */
} RECT_CLIP_CLASS;
extern RECT_CLIP_CLASS rect_clip;

extern void bakuhatsu_add_type_ccc(int type);
extern void bakuhatsu_add_rect(SPRITE *src);
extern void bakuhatsu_add_circle(SPRITE *src, int mode);
extern void bakuhatsu_add_zako04(SPRITE *src);

extern void callback_hit_zako(SPRITE *src, SPRITE *t);

#endif /* _ATARI_HANTEI_H_ */

