
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スペカ管理システム
	-------------------------------------------------------
	★ スペカはシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どのスペカでも撃てるようになります。
	-------------------------------------------------------
	ToDo:
	★ 攻撃の管理
	★ 動作の管理(結構未定)
	★ 雑魚(中雑魚)がスペカを撃てる機能は、廃止した方が良いかも知れない。
	これ(中雑魚＆中雑魚スペカ機能)を廃止して、代わりに道中にボスを出現させる(つまり中ボス)
	の方が、システムがすっきりして良い気がする。そうする為には色々改造個所がある。
	現状の dat の規格だと、これらは出来ないので、主に dat の規格を変えて、
	dat からボスのスクリプトを起動するようにする必要がある。
	プレイヤーに拠ってスクリプトが違うので、その辺の規格策定はまだ未定。(分岐シナリオとか)
	-------------------------------------------------------
	★ 雑魚(中雑魚)がスペカを撃てる機能は、廃止して、
	雑魚(中雑魚)がスペカを撃ちたい場合は、「スペカが撃てるザコ」という物を新設してそれを使い
	システムではサポートしない方が良いと思う。のでそういう方向で改造予定。
	-------------------------------------------------------
	つまり「スペカが撃てるザコ」と「道中にボスを出現させる(つまり中ボス)」の2種類を新設予定。
	将来メインコア入れ換え式になる事が確定しているので、
	☆ 道中コア(雑魚＆ボス(つまり中ボス)が可能)
	☆ ボスコア(ボスのみ可能、雑魚不可)
	☆ 会話コア(ボス会話モードのみ可能、雑魚不可)
---------------------------------------------------------*/

#include "kanji_system.h"

/*---------------------------------------------------------
	ボス追加
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
extern void boss_init_sakuya(SPRITE *src);

// パチェ 5面
extern void add_zako_pache_dolls(SPRITE *src);
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);

// 文 4面
extern void aya_05_keitai(SPRITE *src);
extern void aya_04_keitai(SPRITE *src);
extern void aya_03_keitai(SPRITE *src);
extern void aya_02_keitai(SPRITE *src);
extern void aya_01_keitai(SPRITE *src);
extern void boss_init_aya(SPRITE *src);

// 輝夜 3面
extern void add_zako_kaguya_houmotsu(SPRITE *src);
extern void add_zako_kaguya_dolls(SPRITE *src);
//tern void kaguya_06_keitai(SPRITE *src);
//tern void kaguya_05_keitai(SPRITE *src);
extern void kaguya_04_keitai(SPRITE *src);
//tern void kaguya_03_keitai(SPRITE *src);
//tern void kaguya_02_keitai(SPRITE *src);
extern void kaguya_01_keitai(SPRITE *src);
extern void boss_init_kaguya(SPRITE *src);

// 魅魔 2面
extern void add_zako_mima_dolls(SPRITE *src);
extern void mima_01_keitai(SPRITE *src);

// アリス 1面
extern void alice_03_keitai(SPRITE *src);/* 蒼符「溺愛の仏蘭西人形」^^); */
extern void alice_02_keitai(SPRITE *src);/* 虚覚「パーフェクトフリーズ」 */
extern void alice_01_keitai(SPRITE *src);/* 通常攻撃 */

// 共通形態
extern void init_00_keitai(SPRITE *src);	/* ダミー */
extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */


//------------ スペカ関連
global int spell_card_limit_health; 	/* 規定値以下になればスペカモード解除 */
global int spell_card_boss_state;		/* 負値になればボススペカモードに入らない */
global int spell_card_mode; 			/* スペカモード */
global int spell_card_boss_timer;		/* 共用 */	// 制限時間

global int spell_card_number;			/* 共用 */	// スペカ番号
global int spell_card_max;				/* 共用 */	// スペカ番号最大限界値

typedef struct
{
	int spell_limit_time;						/* スペカの制限時間。(スペカに登録された一定時間) */
	int spell_repeat_time;						/* スペカの繰り返し周期 */
	void (*spell_init_callback)(SPRITE *sss);	/* 初期化移動処理 */
	void (*spell_move_callback)(SPRITE *sss);	/* スペカ移動処理 */
	int spell_life; 							/* スペカに登録された一定体力 */
	const char *spell_str_name; 				/* スペカ名称 */
} SPELL_CARD_RESOURCE;

	/* 名前はテキトーです */

/* スペル時間(==単位[フレーム])は、64倍(==単位[約1秒])が規格 */
#define s_time(aaa) ((aaa)<<6)

#if 0
	8192==1024*8
//
73728==8192*9.0
65536==8192*8.0
57344==8192*7.0
49152==8192*6.0
40960==8192*5.0
36864==8192*4.5
32768==8192*4.0
28672==8192*3.5
24576==8192*3.0
20480==8192*2.5
16384==8192*2.0
12288==8192*1.5
#endif
static SPELL_CARD_RESOURCE my_spell_card_resource[SPELL_CARD_MAX] =
{																							  //	最大半角で31文字。
// 咲夜 6面 																				  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 s_time(300),  s_time(300),  init_00_keitai/*boss_init_sakuya*/,			 common_00_keitai,		73728,NULL/*"形態変更" "\n"*/					},/* 73728==8192*9.0 */
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_01_keitai,		 4096,NULL/*"通常攻撃" "\n"*/					},		/*	"第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_02_keitai,		 4096," 　　奇術「ミスディレクション」" "\n"	},		/*	"第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_03_keitai,		 4096," 　　時雨「バーティカルナイフ」" "\n"	},		/*	"第三形態: (垂直ナイフが落ちてくるよ)"			*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_04_keitai,		 4096," 　幻魂「アンリミテッドメイド」" "\n"	},		/*	"第四形態: 魔方陣生成"							*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_06_keitai,		 8192," 　幻種「デンジャラスワールド」" "\n"	},		/*	"第五形態: (黄色マスカット弾)"					*/
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 common_00_keitai,		 5000,"　　　　　奇術「咲夜テスト007」" "\n"	},		/*	"第七形態: (分散魔方陣)追加計画中"				*/
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 common_00_keitai,		 5000,"　　　　　奇術「咲夜テスト008」" "\n"	},		/*	"第八形態: (時止めナイフ)追加計画中"			*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_09_keitai,		25000," 　幻葬「フェスティバルナイフ」" "\n"	},		/*	"第九形態: 最終形態(その1)" 					*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_10_keitai,		20000," 　　　　幻戒「チェックメイド」" "\n"	},		/*	"第10形態: 最終形態(その2)" 					*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 sakuya_11_keitai,		90000," 　　メイド秘技「ファイナルβ」" "\n"	},		/*	"第11形態: 最終形態(その3)" 					*/

// パチェ 5面																				  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 s_time(300),  s_time(300),  init_00_keitai,			 common_00_keitai,		40960,NULL/*"形態変更" "\n"*/					},/* 40960==8192*5.0 */
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_01_keitai,		 8192,NULL/*"通常攻撃" "\n"*/					},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_01_keitai,		 8192,"　　火符「パチュリー未作成000」" "\n"	},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_01_keitai,		 8192,"　　水符「パチュリー未作成001」" "\n"	},
	{	 s_time(30),   s_time(30),	 add_zako_pache_dolls,		 pache_04_keitai,		 8192,"　　木符「シルフィー未作成002」" "\n"	},
	{	 s_time(30),   s_time(30),	 add_zako_pache_dolls,		 pache_04_keitai,		 8192,"　　金符「パチュリー未作成003」" "\n"	},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_04_keitai,		 8192,"　　土符「パチュリー未作成004」" "\n"	},
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 pache_04_keitai,		 8192,"　　日符「パチュリー未作成005」" "\n"	},
//	{	 s_time(99),   s_time(99),	 init_00_keitai,			 pache_04_keitai,		 8192,"　　月符「サイレント未作成006」" "\n"	},
//	{	 s_time(99),   s_time(99),	 init_00_keitai,			 pache_04_keitai,		 8192,"　　氷符「パチュリー未作成007」" "\n"	},

// 文 4面																					  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 s_time(250),  s_time(250),  init_00_keitai/*boss_init_aya*/,			 common_00_keitai,		32768,NULL/*"形態変更" "\n"*/					},/* 32768=8192*4.0 */
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 aya_01_keitai, 		 4096,NULL/*"通常攻撃" "\n"*/					},/*4*/
	{	 s_time(30),   s_time(30),	 init_00_keitai,			 aya_02_keitai, 		 2048,"　　 華符「うろおぼえ芳華絢爛」" "\n"	},/*4*/
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 aya_03_keitai, 		 2048,"　　　　　　 路符「小手の調べ」" "\n"	},/*5"岐符「未作成スペカ001」"*/
	{	 s_time(40),   s_time(40),	 init_00_keitai,			 aya_04_keitai, 		 4096,"　　　 引導「穎割大根風評被害」" "\n"	},/*4"岐符「未作成スペカ002」"*/
	{	 s_time(100),  s_time(100),  init_00_keitai,			 aya_05_keitai, 		 8192,"　　　　　　 実況「風速３３Ｍ」" "\n"	},/*5"岐符「未作成スペカ003」"*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," 　　　 岐符「未作成スペカ004」" "\n"	},/*4"岐符「未作成スペカ004」"*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," 　　　 岐符「未作成スペカ005」" "\n"	},/*5*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," 　　　 岐符「未作成スペカ006」" "\n"	},/*5*/
	{	 s_time(99),   s_time(99),	 init_00_keitai,			 aya_05_keitai, 		16384," 　　　 岐符「未作成スペカ007」" "\n"	},/*5*/

// 輝夜 3面 																				  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 s_time(250),  s_time(250),  boss_init_kaguya,			 common_00_keitai,		28672,NULL/*"形態変更" "\n"*/					},/* 28672==8192*3.5 */
	{	 s_time(23),   s_time(23),	 add_zako_kaguya_houmotsu,	 kaguya_01_keitai,		 4096,NULL/*"通常攻撃" "\n"*/					}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_houmotsu,	 kaguya_01_keitai,		 4096,"　　　　　　　難題「未作成001」" "\n"	},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_houmotsu,	 kaguya_01_keitai,		 4096,"　　　　　　　難題「未作成002」" "\n"	},// 2.仏の御石の鉢
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"　　　　　　　難題「未作成003」" "\n"	},// 3.火鼠の皮衣
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"　　　　　　　難題「未作成004」" "\n"	},// 4.燕の子安貝
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"　　　　　　　難題「未作成005」" "\n"	},// 5.蓬莱の玉の枝
	{	 s_time(40),   s_time(40),	 add_zako_kaguya_dolls, 	 kaguya_04_keitai,		 4096,"　　　　　　　難題「未作成006」" "\n"	},//

// 魅魔 2面 																				  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 s_time(200),  s_time(200),  init_00_keitai,			 common_00_keitai,		20480,NULL/*"形態変更" "\n"*/					},/* 20480==8192*2.5 */
	{	 s_time(20),   s_time(20),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,NULL/*"通常攻撃" "\n"*/					},
	{	 s_time(20),   s_time(20),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"　　　　未定「未作成スペカ001」" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"　　　　未定「未作成スペカ002」" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"　　　　未定「未作成スペカ003」" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"　　　　未定「未作成スペカ004」" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"　　　　未定「未作成スペカ005」" "\n"	},
	{	 s_time(40),   s_time(40),	 add_zako_mima_dolls,		 mima_01_keitai,		 8192,"　　　　未定「未作成スペカ006」" "\n"	},

// アリス 1面																				  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 s_time(120),  s_time(120),  init_00_keitai,			 common_00_keitai,		16384,NULL/*"形態変更" "\n"*/					},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{	 s_time(10),   s_time(10),	 init_00_keitai,			 alice_01_keitai,		 4096,NULL/*"通常攻撃" "\n"*/					},
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_02_keitai,		 4096," 　凍符「パーフェクトフリーズ」" "\n"	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_03_keitai,		 8100," 　蒼符「溺愛のマトリョーシカ」" "\n"	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 s_time(40),   s_time(40),	 init_00_keitai,			 alice_03_keitai,		 5000,"　操符「未作成の阿蘭陀人形001」" "\n"	},/* 白符「博愛の円谷人形」 */
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_03_keitai,		20000,"操符「未作成の伊太利亜人形002」" "\n"	},/* 炎符「浅草の人形焼き」 */
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_02_keitai,		 5000,"　操符「未作成の英吉利人形003」" "\n"	},
//	{	 s_time(20),   s_time(20),	 init_00_keitai,			 alice_02_keitai,		 5000,"　操符「未作成の露西亜人形004」" "\n"	},
//	{	 s_time(80),   s_time(80),	 init_00_keitai,			 alice_02_keitai,		 5000,"　　操符「未作成の瑞西人形005」" "\n"	},
//	{	 s_time(80),   s_time(80),	 init_00_keitai,			 alice_02_keitai,		 5000,"　操符「未作成の白耳義人形006」" "\n"	},
//	{	 s_time(80),   s_time(80),	 init_00_keitai,			 alice_02_keitai,		 5000,"　　操符「未作成の波蘭人形007」" "\n"	},
//																							  "feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
};
	/*
	マトリョーシカは、露西亜人形でいいのかな？人形というよりより民芸品の趣がある。
	最近は萌えマトリョーシカの需要が日本からあるらしい。
	//
	日本の気象庁は風速17m以上の風を「台風」と定義。
	国際気象機関(WMO)では風速33m以上のものを「typhoon」と定義。それ以下は「storm」。
	*/
		// 咲夜 	sakuya->base_hp 	= ((8*1024)-1)*(9); 	/* 8192==1024*8 */
		// パチェ	sakuya->base_hp 	= ((8*1024)-1)*(3); 	// パチェ本体のHP。もう少し高くてもいいかも。// [***090305	変更
		// 輝夜 	sakuya->base_hp 	= ((8*1024)-1)*5;/*5 4test*/
		// 未定 	sakuya->base_hp 	= 8*(((difficulty<<6/*[x64]*/)+ 200+1024) );/*test*/
		// 文		sakuya->base_hp 	= ((8*1024)-1)*6;/*6 4test*/
		// アリス	sakuya->base_hp 	= (((8*1024)-1)*2); 	// アリス本体のHP。もう少し高くてもいいかも。// [***090305	変更
		//



/*---------------------------------------------------------
	スペカ名称表示
---------------------------------------------------------*/
static void draw_spell_card_name(void)
{
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
	msg_time = (60*5);
	print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
		my_spell_card_resource[(spell_card_number)].spell_str_name, //	aaa_str[(spell_card_number&15)],
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

//
//extern void draw_spell_card_name(void);/* spell_card.c */
global void common_boss_put_items(SPRITE *src)
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
	ライフが少なくなるとボスがなかなか死なないのは、演出だそうです。
	てきとーに再現？
---------------------------------------------------------*/
//0 ... 255 == draw_boss_hp_value 0001 1111 1110 0000;	>>=(2+3);  0000 0000 1111 1111; 0xff;
/*
	4096/60 == 68.2666666666666666666666666666667
	4096/256 == 16
*/
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (16)
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (48)
//	int LIMIT_MAX_HP_DEC_BOSS_BY_FLAME[8] = { (1), (2), (4), (8),  (16), (24), (32), (48), };

extern int boss_hp_dec_by_frame;/*ボス攻撃減少値、フレーム単位*/
global void boss_hp_frame_check(void)
{
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
		/* 8192/32 == 256 */
		test_draw_boss_hp_value = (obj_boss->base_hp>>5) & 0xff;
		#if 0
			 if (0x80 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = (48);	}	//128 ... 255
		else if (0x40 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = (32);	}	// 64 ... 127
		else if (0x20 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = (16);	}	// 32 ...  63
		else if (0x10 < test_draw_boss_hp_value)	{	limit_max_hp_dec_boss_by_flame = ( 4);	}	// 16 ...  31
		else										{	limit_max_hp_dec_boss_by_flame = ( 1);	}	//	0 ...  15
		#endif
		#if 0
			 if (200 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (40);	}	//201 ... 255
		else if (150 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (30);	}	//151 ... 200
		else if (100 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (20);	}	//101 ... 150
		else if ( 50 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (10);	}	// 51 ... 100
		else										{	limit_max_hp_dec_boss_by_flame = ( 5);	}	//	0 ...  50
		#endif
		#if 0
			 if (200 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (48);	}	//201 ... 255  ;  48 = 6  x8
		else if (150 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (40);	}	//151 ... 200  ;  40 = 5  x8
		else if (100 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (32);	}	//101 ... 150  ;  32 = 4  x8
		else if ( 50 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (24);	}	// 51 ... 100  ;  24 = 3  x8
		else										{	limit_max_hp_dec_boss_by_flame = (12);	}	//	0 ...  50  ;  12 = 1.5x8
		#endif
		#if 0
			 if (191 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (48);	}	//192 ... 255  ;  48 = 6  x8
		else if (127 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (40);	}	//128 ... 191  ;  40 = 5  x8
		else if ( 63 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (32);	}	// 64 ... 127  ;  32 = 4  x8
		else										{	limit_max_hp_dec_boss_by_flame = (24);	}	//	0 ...  63  ;  24 = 3  x8
		#endif
		#if 0
			 if (191 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (60);	}	//192 ... 255  ;  60 = 4  x16 -4
		else if (127 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (44);	}	//128 ... 191  ;  44 = 3  x16 -4
		else if ( 63 < test_draw_boss_hp_value) 	{	limit_max_hp_dec_boss_by_flame = (28);	}	// 64 ... 127  ;  28 = 2  x16 -4
		else										{	limit_max_hp_dec_boss_by_flame = (12);	}	//	0 ...  63  ;  12 = 1  x16 -4
		#endif
		#if 0
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>6)+3)<<3);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>3)+(3<<3);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>6)+1)<<4);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>6))<<4)+(1<<4);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2)+(1<<4)-4;
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2)+(16-4);
		#endif
		#if 1
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x0f);
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x08);
		#endif
		//
		if (limit_max_hp_dec_boss_by_flame < boss_hp_dec_by_frame)
		{
			boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_flame;
		}
	}
//
	obj_boss->base_hp -= boss_hp_dec_by_frame;
	boss_hp_dec_by_frame = 0;/*使ったので消す(フレーム単位)*/
	if (spell_card_limit_health >= obj_boss->base_hp)		/* 規定値以下になればスペカモード解除 */
	{
		spell_card_mode 			= (SPELL_CARD_MODE_00_OFF);
		boss_destroy_check_type(obj_boss, DESTROY_CHECK_00_WIN_BOSS);
		callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
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
/*extern*/global int boss_hamidasi; 			/* 「移動できなかったフラグ」(使用前に手動でOFF==0にしとく) */
/*extern*/global POINT256 boss_clip_min;		/* ボス移動範囲(最小値) */
/*extern*/global POINT256 boss_clip_max;		/* ボス移動範囲(最大値) */
global void boss_move_vx_vy(SPRITE *src)
{
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
}
global void boss_clip_rect(SPRITE *src)
{
		 if (src->x256 < boss_clip_min.x256 )	{	src->x256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->x256 > boss_clip_max.x256 )	{	src->x256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
		 if (src->y256 < boss_clip_min.y256 )	{	src->y256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->y256 > boss_clip_max.y256 )	{	src->y256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

// 現状のシナリオスクリプト規格だとスペカシステムと相性が悪い。
// 仕方ないので、強引にフラグで対応させる。
#define USE_OLD_SCRIPT_SYSTEM (1)


/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ登録
---------------------------------------------------------*/
#if (1==USE_OLD_SCRIPT_SYSTEM)
	// 現状のシナリオスクリプト規格だとスペカシステムと相性が悪い。
	// 仕方ないので、強引にフラグで対応させる。
static int speka_first_move_flag;
#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */

//global /*static*/ void re gist_spell_card222(SPRITE *src)
//global /*static*/ void ch eck_regist_spell_card(SPRITE *src)
global /*static*/ void check_regist_generate_spell_card(SPRITE *src)
{
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	// 現状のシナリオスクリプト規格だとスペカシステムと相性が悪い。
	// 仕方ないので、強引にフラグで対応させる。
	int exec_speka;
	exec_speka = 0;
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
//
	if (0/*off*/==spell_card_mode)
	{
	//	if (1)
		{
			int aaa;	/* 現在体力 から 撃ちたいスペカ分 引いた体力値 */
			aaa = spell_card_limit_health - (my_spell_card_resource[(spell_card_number+1)].spell_life); 	/*1000 500*/
		//	spell_card_limit_health -= 1000/*500*/;
			/* ボスがスペカを撃てる一定体力がある場合 */
			if (0 < aaa)
			{
				spell_card_limit_health = aaa;
			//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* 形態変更したら */
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
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
				(my_spell_card_resource[(spell_card_number)].spell_init_callback)(src);
			}
			else
			{
				spell_card_limit_health = (0);
			//	spell_card_mode 		= (0);/*off*/
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* とりあえず */
				/* [(とりあえず)スペカ攻撃のみに仕様変更]したので、最後撃てるスペカがなくなった場合に攻撃させる為。 */
			}
		}
		/*---------------------------------------------------------
			スペカチェック
		---------------------------------------------------------*/
		if (0/*off*/==spell_card_boss_timer)
		{
			spell_card_limit_health = 0;
			spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
		}
	}
	else
	if (SPELL_CARD_MODE_01_IDO_JYUNNBI == spell_card_mode)
	{
		#if (1==USE_OLD_SCRIPT_SYSTEM)
		speka_first_move_flag = 0;
		#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
		src->boss_base_danmaku_time_out = (0);	/* 弾幕生成を強制的に止める。 */
		bullets_to_hosi();		/* 総ての敵弾を、hosiアイテムに変える */
		/* 真中付近に退避 */
	//	src->vvv256 				= t256(  0);/* 初期化済みの必要あり */
		src->vvv256 				= t256(1.0);/* 初期化済みの必要あり */
	//	src->target_x256			= t256(153);
		src->target_x256			= BOSS_XP256; //t256(0);
	//	src->target_y256			= src->y256;
		src->target_y256			= t256(16.0); //t256(0);
		spell_card_mode 			= (SPELL_CARD_MODE_02_TAIHI);/*on*/
	}
	else
	if (SPELL_CARD_MODE_02_TAIHI==spell_card_mode)	/* 発弾位置まで移動中。 */
	{
	//	alice_yuudou_move_only(src);
		alice_yuudou_calc(src);
	//	src->vvv256 -= (1); 	/* [約	4 秒]== 4.2666==(256/60[flame]) */
		src->vvv256 -= (1*4);	/* [約	1 秒]== 1.0666==(256/(4*60)[flame]) */
		if (0 > src->vvv256 )	/* ほぼ画面中心付近まで、移動した。 */
		{
			src->vvv256 = (0);
			spell_card_mode 		= (SPELL_CARD_MODE_03_HATUDAN);/*on*/
		}
	}
	/*---------------------------------------------------------
		[スペカシステム内に移動予定]	スペカ生成
	---------------------------------------------------------*/
	else/* [(とりあえず)スペカ攻撃のみに仕様変更]*/
	/*
		現状シナリオからボスがコントロール出来ないのでこの仕様は暫定的。
		(準備時の移動などもスペカ扱い)
		(スペカ無くなった後も扱い)
	*/
	/* ボス行動 */
	if (SPELL_CARD_MODE_03_HATUDAN==spell_card_mode)
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	{
		exec_speka = 1;
		//(my_spell_card_resource[(spell_card_number)].spell_move_callback)(src);
	}
	if (exec_speka + speka_first_move_flag)
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */
	{
		(my_spell_card_resource[(spell_card_number)].spell_move_callback)(src);
	}
}

	//	if (t256(/*(GAME_WIDTH/2)-32*/144) > src->x256) 	/* 画面半分の位置より若干左。 */
	//	{
	//		src->x256 += t256(1.0); 	/* 右 に退避 */
	//	}
	//	else
	//	if (t256(/*(GAME_WIDTH/2)+32*/208) < src->x256) 	/* 画面半分の位置より若干右。 */
	//	{
	//		src->x256 -= t256(1.0); 	/* 左 に退避 */
	//	}

/*---------------------------------------------------------
	[スペカシステム内に移動予定]
---------------------------------------------------------*/

global /*static*/ void init_00_keitai(SPRITE *src)
{
	/* 現在ダミー */
}
/*global*/static /*static*/ void common_03_keitai(SPRITE *src)
{
//		if ( ((pd_state_flag) & STATE_FLAG_05_IS_BOSS) )	/* 会話終了? */
		{
			src->vvv256 = t256(1.0);
		//	common_boss_init_2nd(src);	/* プレイヤー弾受け付け、コールバックを登録 */
			/*---------------------------------------------------------
				ボスの共通、２回目初期化ルーチン(攻撃可能)
			---------------------------------------------------------*/
		//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
			{
				/* プレイヤー弾受け付け、コールバックを登録 */
				src->callback_hit_enemy = callback_hit_boss;	/* コールバック登録 */
				/* spell_card common init */
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
			//	spell_card_mode 		= (0);/*off*/
			}
		//	/*時間制限カウント有効化*/
		//	data->boss_base.state001++/* = ST_02*/;
			spell_card_number++;
		}
}
/*global*/static /*static*/ void common_02_keitai(SPRITE *src)
{
	//	if ( ((pd_state_flag) & STATE_FLAG_05_IS_BOSS) )	/* 会話終了? */
	//	{
	//		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_03_keitai;
	//	}
}
global /*static*/ void common_00_keitai(SPRITE *src)
{
//	if (0 == src->vvv256)
	if (0 > src->boss_base_state001)
	{
		src->boss_base_state001 = (0);
	//	src->vvv256 = t256(1.0);
	}
	else	/* 会話終了を待つ */
	{
		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_02_keitai;
	}
}
global void script_boss_start(void)
{
		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_03_keitai;
}


/*---------------------------------------------------------
	共通ボス退避(撃破後に画面外にボスが逃げる)
	[スペカシステム内に移動予定]
---------------------------------------------------------*/

global /*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->y256+t256(50.0)))/* +t256(50.0) ボスグラの最大サイズ(50[dot]) */
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		src->callback_mover 		= NULL; 		/* おしまい */
	//	#if (0==US E_BOSS_COMMON_MALLOC)
	//	src->type					= SP_DELETE;	/* おしまい */
	//	#else
		sprite_initialize_position(src);
	//	#endif

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

/*---------------------------------------------------------
	弾幕が終わるまで待つ。
	-------------------------------------------------------

---------------------------------------------------------*/

/*static*/global void danmaku_state_check_holding(SPRITE *src)
{
	if (DANMAKU_00 == src->boss_base_danmaku_type)
	{
		src->boss_base_state001++;
	}
}




/*---------------------------------------------------------
	ボスの共通、１回目初期化ルーチン(初回、攻撃不可)
---------------------------------------------------------*/



/*---------------------------------------------------------
	弾幕の範囲を標準に設定
---------------------------------------------------------*/
global void set_default_bullet_clip(void)/* call from load_stage.c */
{
	//	bullet_clip_min.x256 = t256(-100);
	//	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	//	bullet_clip_min.y256 = t256(-256);/* あや */
	//	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
		bullet_clip_min.x256 = t256(-32);
		bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
		bullet_clip_min.y256 = t256(-32);
		bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
/* from game_core.c */
global void add_boss_common(STAGE_DATA *l)
{
		pd_state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
}
/*static*/extern  void boss_effect_reset(void);
extern void stage_boss_load_texture(void);
global void script_boss_load(int boss_number)
{
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256( 		0)+t256(24);
		boss_clip_max.x256	= t256(GAME_WIDTH)-t256(24);
		boss_clip_min.y256	= t256(0);
		boss_clip_max.y256	= t256(96);
	}
//
	int sss;
//	sss = l->user_255_code;
	sss = boss_number;
	sss &= (8-1);
//
	alice_anime_count	= 0;
	//----[BOSS]
	SPRITE *h;
//	#if (0==U SE_BOSS_COMMON_MALLOC)
//	h									= sprite_add_gu_error();
//	obj_boss							= h;/*輝夜本人*/
//	#else
	h									= obj_boss;/*輝夜本人*/
//	#endif
//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
	{
		h->m_Hit256R					= ZAKO_ATARI16_PNG;
		h->flags						|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
		h->type 						= BOSS_00_BOSS11;
		h->callback_mover				= move_alice;
		h->callback_loser				= common_boss_put_items;
		h->callback_hit_enemy			= NULL; 	/* ダミーコールバック登録 */
	//
		#if 0/* 初期化済みの必要あり */
		h->vx256						= t256( 0);
		h->vy256						= t256( 0);
		#endif
		h->x256 						= BOSS_XP256;/*t256(GAME_WIDTH/2)-(sakuya->w128)*/
		h->y256 						= t256(-100);
	//
	//	h->boss_base_state001			= (0);	/*ST_00*/
		h->boss_base_state001			= (-1); /*ST_00*/
	//
		/* 3面の場合---- */ 	/* 全部一緒 */
		/* 4面の場合---- */ 	/* 輝夜本人 */
		{
			static const unsigned int aaa_score_tbl[8/*8*/] =
			{
				score( 100000),/* エキストラ用(boss0) */
				score( 100000), 		/* アリス */	/*	10万 x 1人 (計100万==(1人x10万)+(6人x5万)+(2人x30万)) */
				score( 500000), 		/* 魅魔 */		/* 50万 (計300万==6x50万) */
				score(2000000), 		/* 輝夜 */		/* 200万 (計400万==(200万)+(5x40万)) */
				//
				score(2000000), 		/* 文 */		/* 200万 */
				score( 500000), 		/* パチェ */	/*	50万 x 1人 ???(計500万==(1人x50万)+(6人x5万)+(2人x30万)) */
				score(5000000), 		/* 咲夜 */		/* 500万 */
				score( 100000),/* ファンタズム用(boss7) */
			};
			h->base_score					= adjust_score_by_difficulty(aaa_score_tbl[sss]);
		}
	//------------ スペカ関連
		{
			static const u8 aaa[8+8] =
			{
				0,/* エキストラ用(boss0) */
				SPELL_CARD_00_alice_000,	/* アリス */
				SPELL_CARD_00_mima_000, 	/* 魅魔 */
				SPELL_CARD_00_kaguya_000,	/* 輝夜 */
			//
				SPELL_CARD_00_aya_000,		/* 文 */
				SPELL_CARD_00_pache_000,	/* パチェ */
				SPELL_CARD_00_sakuya_000,	/* 咲夜 */
				0,/* ファンタズム用(boss7) */
			//
				0,/* エキストラ用(boss0) */
				SPELL_CARD_14_alice_eee,	/* アリス */ /*SPELL_CARD_19_alice_jjj;*/
				SPELL_CARD_17_mima_hhh, 	/* 魅魔 */
				SPELL_CARD_17_kaguya_hhh,	/* 輝夜 */
			//
				SPELL_CARD_19_aya_jjj,		/* 文 */
				SPELL_CARD_17_pache_hhh,	/* パチェ */
				SPELL_CARD_11_sakuya_kkk,	/* 咲夜 */
				0,/* ファンタズム用(boss7) */
			//
			};
			spell_card_number				= aaa[sss];
			spell_card_max					= aaa[sss+8];
		}
		/* spell_card common init */
	//	{
	//	//	spell_card_limit_health 	= 0;
	//	//	spell_card_mode 			= 0;/*off*/
	//	}

		{
			h->base_hp				= (my_spell_card_resource[(spell_card_number)].spell_life); 		/* 全体の体力 */
			spell_card_limit_health = (h->base_hp) - (my_spell_card_resource[(spell_card_number+1)].spell_life);		/* 通常攻撃(初回攻撃)の攻撃分 */
			//
			spell_card_boss_timer	= (((my_spell_card_resource[(spell_card_number)].spell_limit_time)));	/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
			spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* 特殊？ */
			/*???*/
			my_spell_card_resource[(spell_card_number)].spell_move_callback = common_00_keitai;
		}
	//------------
//
		#if 1
	//------------ 弾幕関連
		h->boss_base_danmaku_type		= DANMAKU_00;	/* 弾幕生成終了フラグ */
		h->boss_base_danmaku_test		= 0;	/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	//
	//------------ 弾幕範囲設定 boss_init_aya(); boss_init_sakuya(); (特殊初期化より手前の必要あり)
		set_default_bullet_clip();	/* 弾幕の範囲を標準に設定 */
		{	/* 特殊初期化(ここは本来無い、共通化して廃止予定) */
			void (*bbb[8])(SPRITE *h) =
			{
				NULL,				/* エキストラ用(boss0) */
				NULL,				/* アリス */
				NULL,				/* 魅魔 */
				NULL,				/* 輝夜 */
//
				boss_init_aya,		/* 文 */
				NULL,				/* パチェ */
				boss_init_sakuya,	/* 咲夜 */
				NULL,				/* ファンタズム用(boss7) */
			};
			if (NULL != bbb[ (int)(sss) ])
			{
				(*bbb[ (int)(sss) ])(h);	/* 中間コード形式のコマンドから各関数に分岐する */
			}
		}
	}
	{
		boss_effect_reset();
	}
		// ボステクスチャ読み込み
		stage_boss_load_texture();
	#if (1==USE_OLD_SCRIPT_SYSTEM)
	// 現状のシナリオスクリプト規格だとスペカシステムと相性が悪い。
	// 仕方ないので、強引にフラグで対応させる。
	speka_first_move_flag = 1;
	#endif /* (1==USE_OLD_SCRIPT_SYSTEM) */

}

