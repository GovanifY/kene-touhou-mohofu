/*
【東】PSPで弾幕STGやりたい奴作りたい奴15【方】
722 ：231：2009/09/08(火) 23:17:49 ID:qnDIO3of
模倣風の予定について
スペカは実装予定があります。模倣風が実装を想定しているスペカシステムは以下のようなものです。

ボス → spell_card.c → danmaku.c → bullet_object.c という経路を考えています。
このように分けているのは、現在まだありませんが、
中ザコ → danmaku.c → bullet_object.c という経路も考えているからです。

spell_card.c はスペカを管理するシステムです。

ボスがスペカを撃てる一定体力がある場合。スペカに登録された一定体力と引き換えにスペカを撃ちます。
スペカを撃てるチャンスがあっても、スペカに登録された一定体力に満たない場合はスペカを撃ちません。(撃てません)

ボスがスペカを撃つと同時にボスの体力からスペカ分が引かれて、スペカモードになります。
スペカモード中にボスを攻撃するとボスの体力は減らさないで、スペカ(spell_card.c)が管理している
スペカ分の体力(スペカゲージが表示)を減らします。
スペカモード中にスペカに登録された一定時間が経過するか、スペカ(spell_card.c)が管理している
スペカ分の体力(スペカゲージ)が無くなると通常モードに復帰します。
このときスペカ分の体力(スペカゲージ)は強制的に 0 になります。

このようなスペカシステムを想定している為、スペカシステムに体力を減らさせるようにボスを改造をする必要があります。
スペカシステムを自体を作るのとボスの改造には時間が掛かる(たぶん次回(九月末2009-09-27予定)には間に合いません)ので
気長にお待ちください。

上記以外の案の実装予定はありませんが、もっと良い案があればここに書いてくれれば
参考にしますし、良い案ならば実装するかもしれません。

*/

#include "bullet_object.h"
#include "scenario_script.h"

/*---------------------------------------------------------
	スペカ管理システム
	-------------------------------------------------------
	★スペカはシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どのスペカでも撃てるようになります。
	-------------------------------------------------------
	ToDo:
	★攻撃の管理
	★動作の管理(結構未定)
---------------------------------------------------------*/
// 咲夜 6面
extern void sakuya_11_keitai(SPRITE *src);
extern void sakuya_10_keitai(SPRITE *src);
extern void sakuya_09_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
extern void sakuya_03_keitai(SPRITE *src);
extern void sakuya_02_keitai(SPRITE *src);
extern void sakuya_01_keitai(SPRITE *src);

// パチェ 5面
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);

// 文 4面
extern void aya_05_keitai(SPRITE *src);
extern void aya_04_keitai(SPRITE *src);
//tern void aya_03_keitai(SPRITE *src);
extern void aya_02_keitai(SPRITE *src);
extern void aya_01_keitai(SPRITE *src);

// 輝夜 3面
extern void add_zako_kaguya_houmotsu(SPRITE *src);
//tern void kaguya_06_keitai(SPRITE *src);
//tern void kaguya_05_keitai(SPRITE *src);
extern void kaguya_04_keitai(SPRITE *src);
//tern void kaguya_03_keitai(SPRITE *src);
//tern void kaguya_02_keitai(SPRITE *src);
extern void kaguya_01_keitai(SPRITE *src);

// 魅魔 2面
extern void add_zako_mima_dolls(SPRITE *src);
extern void mima_01_keitai(SPRITE *src);

// アリス 1面
extern void alice_02_keitai(SPRITE *src);/* 蒼符「溺愛の仏蘭西人形」^^); */
extern void alice_01_keitai(SPRITE *src);/* 通常攻撃 */

// 共通形態
extern void init_00_keitai(SPRITE *src);	/* ダミー */
extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */


//------------ スペカ関連
/*extern*/ int spell_card_limit_health; 	/* 規定値以下になればスペカモード解除 */
/*extern*/ int spell_card_boss_state;		/* 負値になればボススペカモードに入らない */
/*extern*/ int spell_card_mode; 			/* スペカモード */
/*extern*/ int spell_card_boss_timer;		/* 共用 */	// 制限時間

/*extern*/ int spell_card_number;			/* 共用 */	// スペカ番号
/*extern*/ int spell_card_max;				/* 共用 */	// スペカ番号最大限界値

typedef struct
{
	int spell_life; 	/* スペカに登録された一定体力 */
	int spell_time; 	/* スペカに登録された一定時間 */
	void (*spell_init_callback)(SPRITE *sss);	/* 初期化移動処理 */
	void (*spell_move_callback)(SPRITE *sss);	/* スペカ移動処理 */
	const char *spell_str_name; 	/* スペカ名称 */
} SPELL_CARD_RESOURCE;

//	{	 500,	  20,		"氷符「開発中000」" },/*NULL*/		/* スペカ生成しない */
	/* 名前はテキトーです */

/* スペル時間(==単位[フレーム])は、64倍(==単位[約1秒])が規格 */
#define s_time(aaa) ((aaa)<<6)

/*static*/ SPELL_CARD_RESOURCE my_111resource[SPELL_CARD_MAX] =
{																				//	最大半角で31文字。
// 咲夜 6面 																	"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	73728,	  s_time(200),	init_00_keitai, 			common_00_keitai,	NULL/*"形態変更" "\n"*/	 				  },/* 73728==8192*9 8192==1024*8 */
	{	 5000,	  s_time(20),	init_00_keitai, 			sakuya_01_keitai,	NULL/*"通常攻撃" "\n"*/ 				  },	  /*  "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"	  */  /* 初回の攻撃分(手動設定)  500 160  */
	{	 5000,	  s_time(20),	init_00_keitai, 			sakuya_02_keitai,	" 　　奇術「ミスディレクション」" "\n"	  },	  /*  "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"  */
	{	 5000,	  s_time(20),	init_00_keitai, 			sakuya_03_keitai,	" 　　時雨「バーティカルナイフ」" "\n"	  },	  /*  "第三形態: (垂直ナイフが落ちてくるよ)"		  */
	{	 3000,	  s_time(20),	init_00_keitai, 			sakuya_04_keitai,	" 　幻魂「アンリミテッドメイド」" "\n"	  },	  /*  "第四形態: 魔方陣生成"						  */
	{	10000,	  s_time(20),	init_00_keitai, 			sakuya_06_keitai,	" 　幻種「デンジャラスワールド」" "\n"	  },	  /*  "第五形態: (黄色マスカット弾)"				  */
//	{	 5000,	  s_time(20),	init_00_keitai, 			common_00_keitai,	"　　　　　奇術「咲夜テスト007」" "\n"	  },	  /*  "第七形態: (分散魔方陣)追加計画中"			  */
//	{	 5000,	  s_time(20),	init_00_keitai, 			common_00_keitai,	"　　　　　奇術「咲夜テスト008」" "\n"	  },	  /*  "第八形態: (時止めナイフ)追加計画中"			  */
	{	25000,	  s_time(20),	init_00_keitai, 			sakuya_09_keitai,	" 　幻葬「フェスティバルナイフ」" "\n"	  },	  /*  "第九形態: 最終形態(その1)"					  */
	{	20000,	  s_time(20),	init_00_keitai, 			sakuya_10_keitai,	" 　　　　幻戒「チェックメイド」" "\n"	  },	  /*  "第10形態: 最終形態(その2)"					  */
	{	90000,	  s_time(20),	init_00_keitai, 			sakuya_11_keitai,	" 　　メイド秘技「ファイナルβ」" "\n"	  },	  /*  "第11形態: 最終形態(その3)"					  */

// パチェ 5面																	"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	65536,	  s_time(250),	init_00_keitai, 			common_00_keitai,	NULL/*"形態変更" "\n"*/	 				  },/* 65536==8192*8 3069 150 120 初回の攻撃分(手動設定) */
	{	 5000,	  s_time(20),	init_00_keitai, 			pache_01_keitai,	NULL/*"通常攻撃" "\n"*/ 				  },  /* 初回の攻撃分(手動設定) */
	{	10000,	  s_time(40),	init_00_keitai, 			pache_01_keitai,	"　　火符「パチュリー未作成000」" "\n"	  },
	{	10000,	  s_time(20),	init_00_keitai, 			pache_04_keitai,	"　　水符「パチュリー未作成001」" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"　　木符「シルフィー未作成002」" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"　　金符「パチュリー未作成003」" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"　　土符「パチュリー未作成004」" "\n"	  },
	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"　　日符「パチュリー未作成005」" "\n"	  },
//	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"　　月符「サイレント未作成006」" "\n"	  },
//	{	10000,	  s_time(40),	init_00_keitai, 			pache_04_keitai,	"　　氷符「パチュリー未作成007」" "\n"	  },

// 文 4面																		"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	40960,	  s_time(150),	init_00_keitai, 			common_00_keitai,	NULL/*"形態変更" "\n"*/	 				  },/* 40960==8192*5 49152==8192*6 */
	{	 5000,	  s_time(20),	init_00_keitai, 			aya_01_keitai,		NULL/*"通常攻撃" "\n"*/ 				  },/*4*/	  /* 初回の攻撃分(手動設定) 500 120  */
	{	 5000,	  s_time(30),	init_00_keitai, 			aya_02_keitai,		"　　　 華符「芳華絢爛」のテスト" "\n"	  },/*4*/
	{	 5000,	  s_time(20),	init_00_keitai, 			aya_04_keitai,		"　　　　　　 路符「小手の調べ」" "\n"	  },/*5"岐符「未作成スペカ001」"*/
	{	 8000,	  s_time(40),	init_00_keitai, 			aya_04_keitai,		"　　　 引導「穎割大根風評被害」" "\n"	  },/*4"岐符「未作成スペカ002」"*/
	{	17000,	  s_time(100),	init_00_keitai, 			aya_05_keitai,		"　　　　　　 実況「風速３３Ｍ」" "\n"	  },/*5"岐符「未作成スペカ003」"*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" 　　　 岐符「未作成スペカ004」" "\n"	  },/*4"岐符「未作成スペカ004」"*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" 　　　 岐符「未作成スペカ005」" "\n"	  },/*5*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" 　　　 岐符「未作成スペカ006」" "\n"	  },/*5*/
	{	10000,	  s_time(40),	init_00_keitai, 			aya_05_keitai,		" 　　　 岐符「未作成スペカ007」" "\n"	  },/*5*/

// 輝夜 3面 																	"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	40960,	  s_time(250),	add_zako_kaguya_houmotsu,	common_00_keitai,	NULL/*"形態変更" "\n"*/	 				  },/* 40960==8192*5 */
	{	 5000,	  s_time(23),	add_zako_kaguya_houmotsu,	kaguya_01_keitai,	NULL/*"通常攻撃" "\n"*/ 				  },  /* 初回の攻撃分(手動設定) 500 99	*/	  // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{	 5000,	  s_time(40),	add_zako_kaguya_houmotsu,	kaguya_01_keitai,	"　　　　　　　難題「未作成001」" "\n"	  },// 1.龍の頸の玉   (輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{	 5000,	  s_time(40),	add_zako_kaguya_houmotsu,	kaguya_01_keitai,	"　　　　　　　難題「未作成002」" "\n"	  },// 2.仏の御石の鉢
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"　　　　　　　難題「未作成003」" "\n"	  },// 3.火鼠の皮衣
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"　　　　　　　難題「未作成004」" "\n"	  },// 4.燕の子安貝
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"　　　　　　　難題「未作成005」" "\n"	  },// 5.蓬莱の玉の枝
	{	 7000,	  s_time(40),	add_zako_mima_dolls,		kaguya_04_keitai,	"　　　　　　　難題「未作成006」" "\n"	  },//

// 魅魔 2面 																	"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	57344,	  s_time(200),	init_00_keitai, 			common_00_keitai,	NULL/*"形態変更" "\n"*/	 				  },/* 57344==8192*7 24576==8192*3 */
	{	 8192,	  s_time(20),	add_zako_mima_dolls,		mima_01_keitai, 	NULL/*"通常攻撃" "\n"*/ 				  },  /* 初回の攻撃分(手動設定) 500 40	*/
	{	 8192,	  s_time(20),	add_zako_mima_dolls,		mima_01_keitai, 	"　　　　未定「未作成スペカ001」" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"　　　　未定「未作成スペカ002」" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"　　　　未定「未作成スペカ003」" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"　　　　未定「未作成スペカ004」" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"　　　　未定「未作成スペカ005」" "\n"	  },
	{	 8192,	  s_time(40),	add_zako_mima_dolls,		mima_01_keitai, 	"　　　　未定「未作成スペカ006」" "\n"	  },

// アリス 1面																	"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	12288,	  s_time(100),	init_00_keitai, 			common_00_keitai,	NULL/*"形態変更" "\n"*/	 				  },/* 12288==8192*1.5 16384==8192*2 */   /* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{	 2000,	  s_time(10),	init_00_keitai, 			alice_01_keitai,	NULL/*"通常攻撃" "\n"*/ 				  },  /* 初回の攻撃分(手動設定) 1023 75 */
	{	10000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	" 　蒼符「溺愛のマトリョーシカ」" "\n"	  },/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 5000,	  s_time(40),	init_00_keitai, 			alice_02_keitai,	"　操符「未作成の阿蘭陀人形001」" "\n"	  },/* 白符「博愛の円谷人形」 */
	{	20000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	"操符「未作成の伊太利亜人形002」" "\n"	  },/* 炎符「浅草の人形焼き」 */
//	{	 5000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	"　操符「未作成の英吉利人形003」" "\n"	  },
//	{	 5000,	  s_time(20),	init_00_keitai, 			alice_02_keitai,	"　操符「未作成の露西亜人形004」" "\n"	  },
//	{	 5000,	  s_time(80),	init_00_keitai, 			alice_02_keitai,	"　　操符「未作成の瑞西人形005」" "\n"	  },
//	{	 5000,	  s_time(80),	init_00_keitai, 			alice_02_keitai,	"　操符「未作成の白耳義人形006」" "\n"	  },
//	{	 5000,	  s_time(80),	init_00_keitai, 			alice_02_keitai,	"　　操符「未作成の波蘭人形007」" "\n"	  },
//																				"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
//
	/*
	マトリョーシカは、露西亜人形でいいのかな？人形というよりより民芸品の趣がある。
	最近は萌えマトリョーシカの需要が日本からあるらしい。
	//
	日本の気象庁は風速17m以上の風を「台風」と定義。
	国際気象機関(WMO)では風速33m以上のものを「typhoon」と定義。それ以下は「storm」。
	*/

};

/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ生成
---------------------------------------------------------*/

/*static*/ void spell_card_generator222(SPRITE *src)
{
	(my_111resource[(spell_card_number)].spell_move_callback)(src);
	/*---------------------------------------------------------
		スペカチェック
	---------------------------------------------------------*/
	{
		if (0/*off*/==spell_card_mode)
		{
			if (0/*off*/==spell_card_boss_timer)
			{
				spell_card_limit_health = 0;
				spell_card_mode 		= (1);/*on*/
			}
		}
	}
}

/*---------------------------------------------------------
	スペカ名称表示
---------------------------------------------------------*/
static void draw_spell_card_name(void)
{
	script_message_window_clear();/*スクリプトメッセージ画面を消す*/
	msg_time = (60*5);
	print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
		my_111resource[(spell_card_number)].spell_str_name, //	aaa_str[(spell_card_number&15)],
		/*int color_type*/(7)|STR_CODE_NO_ENTER,	/* 改行しない */
		/*int wait*/(0)
	);
}


/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ撃破後アイテム出す
---------------------------------------------------------*/
//	#define alice_put_items common_boss_put_items
//	#define mima_put_items common_boss_put_items
//	#define kaguya_put_items common_boss_put_items
//	#define pache_put_items common_boss_put_items

/*---------------------------------------------------------
	ボス形態変更時の共通ルーチン
---------------------------------------------------------*/
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* 背景ウィンドウ表示フラグ */
extern void *callback_gu_draw_haikei_supeka;
extern void *callback_gu_draw_haikei_modosu;
//
//extern void draw_spell_card_name(void);/* spell_card.c */
void common_boss_put_items(SPRITE *src)
{
//++	pd_bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
//
	callback_gu_draw_haikei = callback_gu_draw_haikei_supeka;
//
	voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*テキトー*/
	#if (1)
	draw_spell_card_name();
	#endif

}


/*---------------------------------------------------------
	ボスを攻撃した場合のフレームチェック
	-------------------------------------------------------
---------------------------------------------------------*/
/*
	4096/60 == 68.2666666666666666666666666666667
	4096/256 == 16
*/
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (16)
#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (48)
extern int boss_hp_dec_by_frame;/*ボス攻撃減少値、フレーム単位*/
void boss_hp_frame_check(void)
{
	if (LIMIT_MAX_HP_DEC_BOSS_BY_FLAME < boss_hp_dec_by_frame)
	{
		boss_hp_dec_by_frame = LIMIT_MAX_HP_DEC_BOSS_BY_FLAME;
	}
//
		pd_boss->base_hp -= boss_hp_dec_by_frame;
		boss_hp_dec_by_frame = 0;/*使ったので消す(フレーム単位)*/
		if (spell_card_limit_health >= /*data->boss_base.boss*/pd_boss->base_hp)		/* 規定値以下になればスペカモード解除 */
		{
			spell_card_mode 			= 0/*off*/;
			boss_destroy_check_type(pd_boss, DESTROY_CHECK_00_WIN_BOSS);
			callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
		}

}
/*---------------------------------------------------------

---------------------------------------------------------*/
#if 0
/*extern*/ void create_spell_card(SPRITE *src, int spell_card_type)
{
//
	int aaa;	/* 現在体力 から 撃ちたいスペカ分 引いた体力値 */
	aaa = ((/*data->boss_base.boss*/src->base_hp) - (my_111resource[spell_card_type].spell_life));
	/* ボスがスペカを撃てる一定体力がある場合 */
	if ( 0 < aaa )
	{
		spell_card_limit_health = aaa;
		spell_card_mode 		= (1)/*on*/;
	}
}
#endif

/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ登録
---------------------------------------------------------*/

/*static*/ void regist_spell_card222(SPRITE *src)
{
//	spell_card_limit_health -= 1000/*500*/;
	spell_card_limit_health -= (my_111resource[(spell_card_number+1)].spell_life);	/*1000 500*/
	/* ボスがスペカを撃てる一定体力がある場合 */
	if (0 < spell_card_limit_health)
	{
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* 形態変更したら */
		spell_card_mode 		= (1);/*on*/
		spell_card_number++;
		if (spell_card_max < spell_card_number)
		{
			/* 形態変更しない、アイテム吐かない */
			spell_card_number--;
		//	src->callback_loser 			= lose_boss;
		}
		else
//		if (0==sakuya->base_hp)
		{
//			if (0 != (data->boss_base.boss_life))/* 形態変更したら */
			/* アイテム吐く */
			if (NULL != src->callback_loser)
			{
//				data->boss_base.boss_life--;
				(src->callback_loser)(src); 	/* sakuya_put_items(src); */
			}
		}
		/* スペルカード初期化 */
		(my_111resource[(spell_card_number)].spell_init_callback)(src);
	}
	else
	{
		spell_card_limit_health = (0);
		spell_card_mode 		= (0);/*off*/
	}
}


/*---------------------------------------------------------
	ボス移動処理の共通ルーチン
	-------------------------------------------------------
★ 移動範囲の登録、機能：
	あらかじめボス移動範囲を登録しておく。
★ 範囲内ならば移動、機能：
	移動範囲内ならば、ボスが移動する。
	移動範囲外ならば、ボスは移動しない。
★ 移動判定、機能：
	ボスが移動しない場合は、「移動できなかったフラグ」がＯＮになる。
---------------------------------------------------------*/
/*extern*/ int boss_hamidasi;			/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
/*extern*/ POINT256 boss_clip_min;		/* ボス移動範囲(最小値) */
/*extern*/ POINT256 boss_clip_max;		/* ボス移動範囲(最大値) */
void boss_move_clip_rect(SPRITE *src)
{
	src->x256 += (src->vx256);
		 if (src->x256 < boss_clip_min.x256 )	{	src->x256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->x256 > boss_clip_max.x256 )	{	src->x256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
	src->y256 += (src->vy256);
		 if (src->y256 < boss_clip_min.y256 )	{	src->y256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->y256 > boss_clip_max.y256 )	{	src->y256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

/*---------------------------------------------------------
	ボスの共通、１回目初期化ルーチン(初回、攻撃不可)
---------------------------------------------------------*/

#if (0==USE_BOSS_COMMON_MALLOC)
/*extern*/ void spell_card_boss_init_regist(SPRITE *src)
#else
/*extern*/ void spell_card_boss_init_regist_void(void/*SPRITE *src*/)
#endif
{
#if (0==USE_BOSS_COMMON_MALLOC)
	pd_boss = src;/*輝夜本人*/
#else
	SPRITE *src;
	src = pd_boss;/*輝夜本人*/
#endif
//
	src->x256						= BOSS_XP256;/*t256(GAME_WIDTH/2)-(sakuya->w128)*/
	src->y256						= t256(-100);
//
	src->callback_hit_enemy 		= NULL; 	/* ダミーコールバック登録 */
//
	/* spell_card common init */
//	{
//	//	spell_card_limit_health 	= 0;
//	//	spell_card_mode 			= 0;/*off*/
//	}
		#if 1
	//------------ 弾幕関連
		src->boss_base_danmaku_type 	= 0;
		src->boss_base_danmaku_time_out = 0;
		src->boss_base_danmaku_test 	= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif

// 咲夜 	sakuya->base_hp 					= ((8*1024)-1)*(9); 	/* 8192==1024*8 */
// パチェ	sakuya->base_hp 					= ((8*1024)-1)*(3); 	// パチェ本体のHP。もう少し高くてもいいかも。// [***090305	変更
// 輝夜 	sakuya->base_hp 					= ((8*1024)-1)*5;/*5 4test*/
// 未定 	sakuya->base_hp 					= 8*(((difficulty<<6/*[x64]*/)+ 200+1024) );/*test*/
// 文		sakuya->base_hp 					= ((8*1024)-1)*6;/*6 4test*/
// アリス	sakuya->base_hp 					= (((8*1024)-1)*2); 	// アリス本体のHP。もう少し高くてもいいかも。// [***090305	変更
//
	src->base_hp			= (my_111resource[(spell_card_number)].spell_life); 		/* 全体の体力 */
	spell_card_limit_health = (src->base_hp) - (my_111resource[(spell_card_number+1)].spell_life);		/* 通常攻撃(初回攻撃)の攻撃分 */
//
	spell_card_boss_timer	= (((my_111resource[(spell_card_number)].spell_time))); 	/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
	spell_card_mode 		= (1);/*on*/	/* 特殊？ */
	/* boss_rect_init */
	boss_clip_min.x256	= t256( 		0)+t256(24);
	boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
	boss_clip_min.y256	= t256(0);
	boss_clip_max.y256	= t256(96);
	boss_effect_init();

}

/*---------------------------------------------------------
	[スペカシステム内に移動予定]
---------------------------------------------------------*/

/*static*/ void init_00_keitai(SPRITE *src)
{
	/* 現在ダミー */
}

/*static*/ void common_00_keitai(SPRITE *src)
{
	if (t256(15.0) > src->y256) 	/* 下へ移動 */		/*t256(40.0)*/	/*t256(30.0)*/
	{
		src->y256 += t256(1.0); 	/*fps_factor*/	/*t256(2.0)*/
	}
	else
	if (t256(16.0) > src->y256) 	/* 下へ移動 */		/*t256(40.0)*/	/*t256(30.0)*/
	{
		src->y256 += t256(1.0); 	/*fps_factor*/	/*t256(2.0)*/
		pd_state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
	}
	else	/* 会話終了を待つ */
	{
		if ( ((pd_state_flag) & STATE_FLAG_05_IS_BOSS) )	/* 会話終了? */
		{
		//	common_boss_init_2nd(src);	/* プレイヤー弾受け付け、コールバックを登録 */
			/*---------------------------------------------------------
				ボスの共通、２回目初期化ルーチン(攻撃可能)
			---------------------------------------------------------*/
		//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
			{
				/* プレイヤー弾受け付け、コールバックを登録 */
				src->callback_hit_enemy = callback_hit_boss;	/* コールバック登録 */
				/* spell_card common init */
				spell_card_mode 		= (1);/*on*/
			}
		//	/*時間制限カウント有効化*/
		//	data->boss_base.state001++/* = ST_02*/;
			spell_card_number++;
		}
	}
}


/*---------------------------------------------------------
	共通ボス退避(撃破後に画面外にボスが逃げる)
	[スペカシステム内に移動予定]
---------------------------------------------------------*/

/*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->y256+t256(50.0)))/* +t256(50.0) ボスグラの最大サイズ(50[dot]) */
	{
		src->callback_mover 		= NULL; 		/* おしまい */
		src->type					= SP_DELETE;	/* おしまい */
	//	pd_save_timer		= (6);/* 6[フレーム] ボス倒してから次(シナリオ)に進むまでの待ち時間 */
	}
	else
	{
		src->y256 -= t256(0.75);					/* 上に退避 */			/* t256(0.75) 退避速度 0.75[dot/flame] */	/*fps_factor*/
		if ( (int)(GAME_WIDTH*256/2) > src->x256)	/* 画面半分の位置 */
		{
			src->x256 += t256(1.0); 	/* 右上に退避 */
		}
		else
		{
			src->x256 -= t256(1.0); 	/* 左上に退避 */
		}
	}
}
