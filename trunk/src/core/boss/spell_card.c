
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
extern void sakuya_07_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
extern void sakuya_03_keitai(SPRITE *src);
extern void sakuya_02_keitai(SPRITE *src);
//tern void sakuya_01_keitai(SPRITE *src);/*同じ*/
extern void boss_init_sakuya(SPRITE *src);

// パチェ 5面
//extern void add_zako_pache_laser(SPRITE *src);
extern void add_zako_pache_dolls(SPRITE *src);
//tern void pache_05_keitai(SPRITE *src);
extern void pache_04_keitai(SPRITE *src);
//tern void pache_03_keitai(SPRITE *src);
//tern void pache_02_keitai(SPRITE *src);
extern void pache_01_keitai(SPRITE *src);
extern void boss_init_pache(SPRITE *src);/* パチェレーザー用特殊初期化 */

//レーザー
extern void add_laser_off(SPRITE *src);
extern void add_laser_on(SPRITE *src);

// 文 4面
extern void aya_05_keitai(SPRITE *src);
//tern void aya_04_keitai(SPRITE *src);/*同じ*/
//tern void aya_03_keitai(SPRITE *src);/*同じ*/
//tern void aya_02_keitai(SPRITE *src);/*同じ*/
//tern void aya_01_keitai(SPRITE *src);/*同じ*/
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

extern void mima_01_keitai(SPRITE *src);

// アリス 1面


// 共通形態
extern void boss_move_03_x_douki(SPRITE *src);
extern void boss_move_02_xy_hidouki(SPRITE *src);
extern void boss_move_01_not_move(SPRITE *src);


extern void common_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */


/* 共通部 */
global void init_00_boss_clip000(SPRITE *h);/* 標準タイプ */
global void init_00_boss_clip111(SPRITE *h);/* 上に広いタイプ */




//------------ スペカ関連
#include "spell_card_value.h"

global int spell_card_limit_health; 	/* 規定値以下になればスペカモード解除 */
global int spell_card_boss_state;		/* 負値になればボススペカモードに入らない */
global int spell_card_mode; 			/* スペカモード */
global int spell_card_boss_timer;		/* 共用 */	// 制限時間

global int spell_card_number;			/* 共用 */	// スペカ番号
/*global*/ static int spell_card_syoji_maisuu;				/* 共用 */	// スペカ番号最大限界値

typedef struct
{
	int spell_life; 							/* スペカに登録された一定体力 */
	int spell_limit_time;						/* スペカの制限時間。(スペカに登録された一定時間) */
//
	const char *spell_str_name; 				/* スペカ名称 */
	int danmaku_type;							/* スペカに登録された弾幕 */
//
	void (*spell_init_callback)(SPRITE *sss);	/* 初期化移動処理 */
	void (*spell_move_callback)(SPRITE *sss);	/* スペカ移動処理 */
} SPELL_CARD_RESOURCE;

	/* 名前はテキトーです */

/* スペル時間(==単位[フレーム])は、64倍(==単位[約1秒])が規格 */
#define s_time(aaa) ((aaa)<<6)

#if 0
	8192==1024*8
//
77824==8192*9.5
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
{									//	最大半角で31文字。

// チルノ エキストラステージ		"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 (28672),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (28672),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (28672),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (28672),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
//	3.5
	{	  (4096),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
	{	  (4096),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
	{	  (4096),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
	{	  (4096),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_10_cirno,				NULL,						boss_move_02_xy_hidouki,	},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
//	3.0
	{	  (6144),	s_time(20), 	" 　　氷符「アイシクルフォール」" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "第二形態: " */
	{	  (6144),	s_time(20), 	" 　　氷符「アイシクルフォール」" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "第二形態: " */
	{	  (6144),	s_time(20), 	" 　　氷符「アイシクルフォール」" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "第二形態: " */
	{	  (6144),	s_time(20), 	" 　　氷符「アイシクルフォール」" "\n", DANMAKU_01_sakuya,				NULL,						boss_move_01_not_move,		},	/* "第二形態: " */
//	2.25
	{	  (2048),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第三形態: " */
	{	  (2048),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第三形態: " */
	{	  (2048),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第三形態: " */
	{	  (2048),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第三形態: " */
//	2.0
	{	  (4096),	s_time(20), 	" 　凍符「パーフェクトフリーズ」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "第四形態: " */
	{	  (4096),	s_time(20), 	" 　凍符「パーフェクトフリーズ」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "第四形態: " */
	{	  (4096),	s_time(20), 	" 　凍符「パーフェクトフリーズ」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "第四形態: " */
	{	  (4096),	s_time(20), 	" 　凍符「パーフェクトフリーズ」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},	/* "第四形態: " */
//	1.5
	{	  (8192),	s_time(20), 	" 雪符「ダイアモンドブリザード」" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "第五形態: " */
	{	  (8192),	s_time(20), 	" 雪符「ダイアモンドブリザード」" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "第五形態: " */
	{	  (8192),	s_time(20), 	" 雪符「ダイアモンドブリザード」" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "第五形態: " */
	{	  (8192),	s_time(20), 	" 雪符「ダイアモンドブリザード」" "\n", DANMAKU_12_diamond_blizzard,	NULL,						boss_move_02_xy_hidouki,	},	/* "第五形態: " */
//	0.0

// 咲夜 6面 						"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 (77824),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (77824),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (77824),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
	{	 (77824),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},	/* 73728==8192*9.0 */
//	9.5
	{	 (4096),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	 (4096),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	 (4096),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	 (4096),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_01_sakuya,				boss_init_sakuya,			boss_move_03_x_douki,		},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
//	9.0
	{	 (6144),	s_time(20), 	" 　　奇術「ミスディレクション」" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	 (6144),	s_time(20), 	" 　　奇術「ミスディレクション」" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	 (6144),	s_time(20), 	" 奇術「幻惑ミスディレクション」" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	 (6144),	s_time(20), 	" 奇術「幻惑ミスディレクション」" "\n", DANMAKU_0d_sakuya_miss_direction,				NULL,						sakuya_02_keitai,			},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
//	8.25	// bloody==血だらけの。 staind ステインド==ステンド==(色を)焼き付け(て残す)る。
	{	 (2048),	s_time(20), 	" 　　　　時象「ペッパーナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ)" 		*/
	{	 (2048),	s_time(20), 	" 　　　　時雨「ステンドナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ)" 		*/
	{	 (2048),	s_time(20), 	" 　　五月雨「ブラッディナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ)" 		*/
	{	 (2048),	s_time(20), 	" 　　時砂「パーフェクトナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_03_keitai,			},	/* "第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ)" 		*/
//	8.0
	{	 (4096),	s_time(20), 	" 　　　幻宝「リミテッドメイド」" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "第四形態: 魔方陣生成"							*/
	{	 (4096),	s_time(20), 	" 　幻魂「アンリミテッドメイド」" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "第四形態: 魔方陣生成"							*/
	{	 (4096),	s_time(20), 	" 　　幻実「スプラッターメイド」" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "第四形態: 魔方陣生成"							*/
	{	 (4096),	s_time(20), 	" 　　　　　　幻視「殺伐メイド」" "\n", DANMAKU_00, 					NULL,						sakuya_04_keitai,			},	/* "第四形態: 魔方陣生成"							*/
//	7.5 	// meek==素直。
	{	 (8192),	s_time(20), 	" 　　幻定「デンジャラスミーク」" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "第五形態: (黄色マスカット弾)"					*/
	{	 (8192),	s_time(20), 	" 　幻種「デンジャラスワールド」" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "第五形態: (黄色マスカット弾)"					*/
	{	 (8192),	s_time(20), 	" 　　幻象「デンジャラスタイム」" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "第五形態: (黄色マスカット弾)"					*/
	{	 (8192),	s_time(20), 	" 　幻舞「デンジャラスストーム」" "\n", DANMAKU_00, 					NULL,						sakuya_06_keitai,			},	/* "第五形態: (黄色マスカット弾)"					*/
//	6.5 	// ジャック・ザ・リッパー==Jack the Ripper==切り裂きジャック(殺人鬼)19世紀(1888年)に実在/抽象名詞化。ワールドヒーローズ。 リック==スプラッターハウス
	{	(16384),	s_time(20), 	"　奇抜「ジャック・ガ・リッパ」" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第七形態: (分散魔方陣)追加計画中"				*/
	{	(16384),	s_time(20), 	"奇術「ジャック・ザ・ラッパー」" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第七形態: (分散魔方陣)追加計画中"				*/
	{	(16384),	s_time(20), 	"奇術「ジャック・ザ・ビーンズ」" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第七形態: (分散魔方陣)追加計画中"				*/
	{	(16384),	s_time(20), 	"奇違「ジャック・デ・マメマキ」" "\n", DANMAKU_0c_sakuya_jack32,		NULL,						boss_move_02_xy_hidouki,	},	/* "第七形態: (分散魔方陣)追加計画中"				*/
//
//	{	 (8192),	s_time(20), 	"　　　　　奇術「咲夜テストE08」" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	{	 (8192),	s_time(20), 	"　　　　　奇術「咲夜テストN08」" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	{	 (8192),	s_time(20), 	"　　　　　奇術「咲夜テストH08」" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	{	 (8192),	s_time(20), 	"　　　　　奇術「咲夜テストL08」" "\n", DANMAKU_00, 					NULL,						common_00_keitai,			},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	4.5
	{	(16384),	s_time(20), 	" 　幻想「フェスティバルナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "第九形態: 最終形態(その1)"					*/
	{	(16384),	s_time(20), 	" 　幻葬「フェスティバルナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "第九形態: 最終形態(その1)"					*/
	{	(16384),	s_time(20), 	" 　幻選「フェスティバルナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "第九形態: 最終形態(その1)"					*/
	{	(16384),	s_time(20), 	" 　幻惑「フェスティバルナイフ」" "\n", DANMAKU_00, 					NULL,						sakuya_09_keitai,			},	/* "第九形態: 最終形態(その1)"					*/
//	2.5
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "第10形態: 最終形態(その2)"	" 　　　　幻界「チョットメイド」" "\n"				*/
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "第10形態: 最終形態(その2)"	" 　　　　幻戒「チェックメイド」" "\n"				*/
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "第10形態: 最終形態(その2)"	" 　　　　幻解「ウッカリメイド」" "\n"				*/
	{	(16384),	s_time(20), 	NULL,									DANMAKU_0e_remilia_00,			NULL,						sakuya_10_keitai,			},	/* "第10形態: 最終形態(その2)"	" 　　　　幻怪「ウットリメイド」" "\n"				*/
//	0.5
	{	 (2048),	s_time(20), 	" 　　メイド秘密「残虐行為手当」" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "第11形態: 最終形態(その3)"					*/
	{	 (2048),	s_time(20), 	" 　　メイド秘技「鯱！鯱！鯱！」" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "第11形態: 最終形態(その3)"					*/
	{	 (2048),	s_time(20), 	" 　メイド秘宝「あつくて死ぬぜ」" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "第11形態: 最終形態(その3)"					*/
	{	 (2048),	s_time(20), 	" 　　メイド日々「もうすぐボス」" "\n", DANMAKU_00, 					NULL,						sakuya_11_keitai,			},	/* "第11形態: 最終形態(その3)"					*/
//	0.25

// パチェ 5面						"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	 (40960),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{	 (40960),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{	 (40960),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
	{	 (40960),	s_time(300),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 40960==8192*5.0 */
//	5.0
	{	  (8192),	s_time(30), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
	{	  (8192),	s_time(30), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
	{	  (8192),	s_time(30), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
	{	  (8192),	s_time(30), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_27_PACHE_LASER2,		add_laser_on,				boss_move_01_not_move,		},/*boss_init_pache*/
//	4.0
	{	  (8192),	s_time(30), 	"　　火符「パチュリー未作成E00」" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"　　火符「パチュリー未作成N00」" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"　　火符「パチュリー未作成H00」" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"　　火符「パチュリー未作成L00」" "\n", DANMAKU_00, 					add_laser_off,				pache_01_keitai,			},
//	3.0
	{	  (8192),	s_time(30), 	"　　水符「パチュリー未作成E01」" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"　　水符「パチュリー未作成N01」" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"　　水符「パチュリー未作成H01」" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
	{	  (8192),	s_time(30), 	"　　水符「パチュリー未作成L01」" "\n", DANMAKU_00, 					NULL,						pache_01_keitai,			},
//	2.0
	{	  (8192),	s_time(30), 	"　　木符「シルフィー未作成E02」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　木符「シルフィー未作成N02」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　木符「シルフィー未作成H02」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　木符「シルフィー未作成L02」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
//	1.0
	{	  (8192),	s_time(30), 	"　　金符「パチュリー未作成E03」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　金符「パチュリー未作成N03」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　金符「パチュリー未作成H03」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　金符「パチュリー未作成L03」" "\n", DANMAKU_00, 					add_zako_pache_dolls,		pache_04_keitai,			},
//	0.0
	{	  (8192),	s_time(30), 	"　　土符「パチュリー未作成E04」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　土符「パチュリー未作成N04」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　土符「パチュリー未作成H04」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　土符「パチュリー未作成L04」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	0.0
	{	  (8192),	s_time(30), 	"　　日符「パチュリー未作成E05」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　日符「パチュリー未作成N05」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　日符「パチュリー未作成H05」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
	{	  (8192),	s_time(30), 	"　　日符「パチュリー未作成L05」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},

//	{	  (8192),	s_time(99), 	"　　月符「サイレント未作成E06」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"　　月符「サイレント未作成N06」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"　　月符「サイレント未作成H06」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"　　月符「サイレント未作成L06」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},

//	{	  (8192),	s_time(99), 	"　　氷符「パチュリー未作成E07」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"　　氷符「パチュリー未作成N07」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"　　氷符「パチュリー未作成H07」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},
//	{	  (8192),	s_time(99), 	"　　氷符「パチュリー未作成L07」" "\n", DANMAKU_00, 					NULL,						pache_04_keitai,			},



//	src->boss_base_danmaku_type 	= DANMAKU_15_aya_misogi;		/* 弾幕をセット */
//	src->boss_base_danmaku_type 	= DANMAKU_25_houka_kenran;		/* 弾幕をセット */	/*aya_02_keitai*/
//	src->boss_base_danmaku_type 	= DANMAKU_23_aya_merin_test;	/* 弾幕をセット */	/*aya_01_keitai*/

// 文 4面							"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	//																										/*ボス登場前の初期化[会話の前]*/
	{	(32768),	s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	{	(32768),	s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	{	(32768),	s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	{	(32768),	s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					NULL,						common_00_keitai,			},/* 32768=8192*4.0 */
	//	4.0 																								/*ボス戦闘前の初期化[会話の後]*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				DANMAKU_20_aya_misogi1, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*4*/
	//	3.5
	{		 (4096),s_time(30), 	" 　　　　　　　岐符「天の八衢」" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	{		 (4096),s_time(30), 	" 　　　　　　　岐符「天の八衢」" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	{		 (4096),s_time(30), 	" 　　　　　　　岐符「天の八衢」" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	{		 (4096),s_time(30), 	" 　　　　　　　岐符「天の八衢」" "\n", DANMAKU_22_ame_no_yatimata, 	init_00_boss_clip111,		boss_move_01_not_move,		},/*4*/
	//	3.0
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"岐符「未作成スペカ001」"*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"岐符「未作成スペカ001」"*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"岐符「未作成スペカ001」"*/
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				DANMAKU_21_aya_misogi2, 		init_00_boss_clip000,		boss_move_03_x_douki,		},/*5"岐符「未作成スペカ001」"*/
	//	2.5
	{		 (4096),s_time(40), 	"　　　　　　 摘符「穎割大好評」" "\n", DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"岐符「未作成スペカ002」"*/
	{		 (4096),s_time(40), 	"　　　　　 風説「穎割大根被害」" "\n", DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"岐符「未作成スペカ002」"*/
	{		 (4096),s_time(40), 	"　　　　　　 過剰「山葵大被害」" "\n", DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"岐符「未作成スペカ002」"*/
	{		 (4096),s_time(40), 	"　　　　　 大量「麦酒十\升呑過」" "\n",DANMAKU_24_aya_doll,			NULL,						boss_move_03_x_douki,		},/*4"岐符「未作成スペカ002」"*/
	//	2.0
	{		 (4096),s_time(100),	"　　　　　　　 実況「風速３Ｍ」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"岐符「未作成スペカ003」"*/
	{		 (4096),s_time(100),	"　　　　　　 実況「風速３３Ｍ」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"岐符「未作成スペカ003」"*/
	{		 (4096),s_time(100),	"　　　　　　 実況「風速９８Ｍ」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"岐符「未作成スペカ003」"*/
	{		 (4096),s_time(100),	"　　　　　　 実況「風速測定中」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5"岐符「未作成スペカ003」"*/
	//	1.5
	{		 (4096),s_time(99), 	"　　　　　　 路符「小耳の調べ」" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"岐符「未作成スペカ004」"*/
	{		 (4096),s_time(99), 	"　　　　　　 路符「小手の調べ」" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"岐符「未作成スペカ004」"*/
	{		 (4096),s_time(99), 	"　　　　　　 路符「胡弓の調べ」" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"岐符「未作成スペカ004」"*/
	{		 (4096),s_time(99), 	"　　　　 追跡「コロンボの調べ」" "\n", DANMAKU_2a_amefuri_test,		init_00_boss_clip111,		boss_move_01_not_move,		},/*4"岐符「未作成スペカ004」"*/
	//	1.0
	{		 (4096),s_time(99), 	"　　 華符「うろおぼえ芳華絢爛」" "\n", DANMAKU_25_houka_kenran,		init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	{		 (4096),s_time(99), 	"　　　　　　　 塞符「山神渡御」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	{		 (4096),s_time(99), 	"　　　　 塞符「天上天下の照國」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	{		 (4096),s_time(99), 	"　　　　 塞符「唯我独尊の照國」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},/*5*/
	//	0.5
//	{		 (4096),s_time(99), 	" 　　　 岐符「未作成スペカE06」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		 (4096),s_time(99), 	" 　　　 岐符「未作成スペカN06」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		 (4096),s_time(99), 	" 　　　 岐符「未作成スペカH06」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		 (4096),s_time(99), 	" 　　　 岐符「未作成スペカL06」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
	//	0.0
//	{		(16384),s_time(99), 	" 　　　 岐符「未作成スペカE07」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		(16384),s_time(99), 	" 　　　 岐符「未作成スペカN07」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		(16384),s_time(99), 	" 　　　 岐符「未作成スペカH07」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/
//	{		(16384),s_time(99), 	" 　　　 岐符「未作成スペカL07」" "\n", DANMAKU_00, 					NULL,						aya_05_keitai,				},/*5*/

//	src->boss_base_danmaku_type 	= DANMAKU_1e_kaguya01;		/* 弾幕をセット */	ボス行動、第 1形態
//	src->boss_base_danmaku_type 	= DANMAKU_1f_kaguya04;		/* 弾幕をセット */	ボス行動、第 4形態

// 輝夜 3面 						"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{		(28672),s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{		(28672),s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{		(28672),s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	{		(28672),s_time(250),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 28672==8192*3.5 */
	// 3.5
	{		 (4096),s_time(23), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{		 (4096),s_time(23), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{		 (4096),s_time(23), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{		 (4096),s_time(23), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1e_kaguya01,			boss_init_kaguya,			kaguya_01_keitai,			}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	// 3.0
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成E01」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成N01」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成H01」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成L01」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	// 2.5
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成E02」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.仏の御石の鉢
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成N02」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.仏の御石の鉢
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成H02」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.仏の御石の鉢
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成L02」" "\n", DANMAKU_1e_kaguya01,			add_zako_kaguya_houmotsu,	kaguya_01_keitai,			},// 2.仏の御石の鉢
	// 2.0
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成E03」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.火鼠の皮衣
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成N03」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.火鼠の皮衣
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成H03」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.火鼠の皮衣
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成L03」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 3.火鼠の皮衣
	// 1.5
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成E04」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.燕の子安貝
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成N04」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.燕の子安貝
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成H04」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.燕の子安貝
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成L04」" "\n", DANMAKU_1f_kaguya04,			add_zako_kaguya_dolls,		kaguya_04_keitai,			},// 4.燕の子安貝
	// 1.0
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成E05」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.蓬莱の玉の枝
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成N05」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.蓬莱の玉の枝
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成H05」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.蓬莱の玉の枝
	{		 (4096),s_time(40), 	"　　　　　　　難題「未作成L05」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},// 5.蓬莱の玉の枝
	// 0.5
	{		 (4096),s_time(40), 	"　　　　難題「蓬莱の玉の枝E06」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//
	{		 (4096),s_time(40), 	"　　　　難題「蓬莱の玉の枝N06」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//
	{		 (4096),s_time(40), 	"　　　　難題「蓬莱の玉の枝H06」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//
	{		 (4096),s_time(40), 	"　　　　難題「蓬莱の玉の枝L06」" "\n", DANMAKU_1c_kakuya_tamanoe,		add_zako_kaguya_dolls,		boss_move_01_not_move,		},//

// 魅魔 2面 						"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{		(32768),s_time(200),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{		(32768),s_time(200),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{		(32768),s_time(200),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	{		(32768),s_time(200),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 20480==8192*2.5 */
	// 4.0
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	NULL,/* "通常攻撃" "\n"*/				DANMAKU_1a_mima_toge,			NULL,						mima_01_keitai, 			},
	// 3.5
	{		 (4096),s_time(20), 	"　　　　未定「未作成スペカE01」" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	"　　　　未定「未作成スペカN01」" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	"　　　　未定「未作成スペカH01」" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	{		 (4096),s_time(20), 	"　　　　未定「未作成スペカL01」" "\n", DANMAKU_18_hana_test,			NULL,						mima_01_keitai, 			},
	// 3.0
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカE02」" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},//"　　 闇符「ディマーケイション」"
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカN02」" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカH02」" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカL02」" "\n", DANMAKU_29_rumia_demarcation,	NULL,						boss_move_03_x_douki,		},
	// 2.5
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカE03」" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカN03」" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカH03」" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカL03」" "\n", DANMAKU_23_aya_merin_test,		init_00_boss_clip111,		boss_move_01_not_move,		},
	// 2.0
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカE04」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカN04」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカH04」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカL04」" "\n", DANMAKU_26_aya_saifu,			init_00_boss_clip000,		boss_move_01_not_move,		},
	// 1.5
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカE05」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカN05」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカH05」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	{		 (4096),s_time(40), 	"　　　　未定「未作成スペカL05」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},
	// 1.0
	{		 (4096),s_time(40), 	"　　　　未定「てすとスペカE06」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// 都合上来ない。
	{		 (4096),s_time(40), 	"　　　　未定「てすとスペカN06」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// 都合上来ない。
	{		 (4096),s_time(40), 	"　　　　未定「てすとスペカH06」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// 都合上来ない。
	{		 (4096),s_time(40), 	"　　　　未定「てすとスペカL06」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},// 都合上来ない。
/*
	魅魔様は一応「魔法少女」なんだから、忘れないでよね！
*/

// アリス 1面						"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{	(24576),	 s_time(120),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{	(24576),	 s_time(120),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{	(24576),	 s_time(120),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{	(24576),	 s_time(120),	NULL,/* "形態変更" "\n"*/				DANMAKU_00, 					init_00_boss_clip000,		common_00_keitai,			},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	// 3.0
	{	 (4096),	 s_time(10),	NULL,/* "通常攻撃" "\n"*/				DANMAKU_14_alice_youmu300,		NULL,						boss_move_01_not_move,		},
	{	 (4096),	 s_time(10),	NULL,/* "通常攻撃" "\n"*/				DANMAKU_14_alice_youmu300,		NULL,						boss_move_01_not_move,		},
	{	 (4096),	 s_time(10),	NULL,/* "通常攻撃" "\n"*/				DANMAKU_13_alice_suwako,		NULL,						boss_move_01_not_move,		},
	{	 (4096),	 s_time(10),	NULL,/* "通常攻撃" "\n"*/				DANMAKU_13_alice_suwako,		NULL,						boss_move_01_not_move,		},
	// 2.5
	{	 (3072),	 s_time(20),	" 　　　　　懐古「紅の魔法Lv.2」" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 (3072),	 s_time(20),	"  　　　　懐古「紅の魔法Lv.16」" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 (3072),	 s_time(20),	" 　　　　懐古「紅の魔法Lv.256」" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 (3072),	 s_time(20),	" 　　　懐古「紅の魔法Lv.65536」" "\n", DANMAKU_15_alice_aka_2nd,		NULL,						boss_move_01_not_move,		},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	// 2.125
	{	 (3072),	 s_time(20),	"   　凍符「パーシャルフローズ」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 (3072),	 s_time(20),	" 　凍符「パーフェクトフリーズ」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 (3072),	 s_time(20),	" 　凍符「パーティクルフリーク」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 (3072),	 s_time(20),	" 　凍符「ブリザードフローズン」" "\n", DANMAKU_11_perfect_freeze,		NULL,						boss_move_02_xy_hidouki,	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	// 1.75
	{	(12288),	 s_time(40),	" 　蒼符「薄愛のマトリョーシカ」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　蒼符「薄愛のマトリョーシカ」"白符「博愛の円谷人形」 */
	{	(12288),	 s_time(40),	" 　蒼符「溺愛のマトリョーシカ」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　蒼符「溺愛のマトリョーシカ」"白符「博愛の円谷人形」 */
	{	(12288),	 s_time(40),	" 　蒼符「自戒のマトリョーシカ」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　蒼符「自戒のマトリョーシカ」"白符「博愛の円谷人形」 */
	{	(12288),	 s_time(40),	" 　蒼符「自害のマトリョーシカ」" "\n", DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　蒼符「自害のマトリョーシカ」"白符「博愛の円谷人形」 */
	// 0.25
	{	 (3072),	 s_time(40),	NULL,/* "通常攻撃z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　　　　操符「淡い眼の案山子」"白符「博愛の円谷人形」 */
	{	 (3072),	 s_time(40),	NULL,/* "通常攻撃z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　　　　　操符「幻の機巧人形」"白符「博愛の円谷人形」 */
	{	 (3072),	 s_time(40),	NULL,/* "通常攻撃z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　　　　　　操符「古の流し雛」"白符「博愛の円谷人形」 */
	{	 (3072),	 s_time(40),	NULL,/* "通常攻撃z" "\n"*/				DANMAKU_16_alice_doll,			NULL,						boss_move_02_xy_hidouki,	},/* " 　操符「呪のクラッシュダミー」"白符「博愛の円谷人形」 */
//									"feeddccbbaa99887766554433221100",'\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
};
//	" 　　　蒼符「博愛の仏蘭西人形」" "\n"
//	" 　蒼符「博愛のオルレアソ\人形」" "\n" " 　蒼符「博愛のオルレアン人形」"...現在の方式では無理です。
//	{	 (4096),	 s_time(40),	"　操符「未作成の阿蘭陀人形L01」" "\n", DANMAKU_00, 					NULL,						alice_04_keitai,			},/* 白符「博愛の円谷人形」 */
//	{	(20000),	 s_time(20),	"操符「未作成の伊太利亜人形L02」" "\n", DANMAKU_00, 					NULL,						boss_move_02_xy_hidouki,	},/* 炎符「浅草の人形焼き」 */
//	{	 (5000),	 s_time(20),	"　操符「未作成の英吉利人形L03」" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(20),	"　操符「未作成の露西亜人形L04」" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(80),	"　　操符「未作成の瑞西人形L05」" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(80),	"　操符「未作成の白耳義人形L06」" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
//	{	 (5000),	 s_time(80),	"　　操符「未作成の波蘭人形L07」" "\n", DANMAKU_00, 					NULL,						alice_02_keitai,			},
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




/*---------------------------------------------------------
	スペルカードシステムのボス形態に登録された弾幕番号を取得し、
	同時に弾幕の時間切れを設定する。
---------------------------------------------------------*/

global void spell_card_get_danmaku_number(SPRITE *src)
{
	src->boss_base_danmaku_type 	= my_spell_card_resource[(spell_card_number)].danmaku_type; 	/* 弾幕をセット */
	danmaku_set_time_out(src);		/* 弾幕の制限時間を設定(予め弾幕ごとに設定されている標準時間に設定) */
}


/*---------------------------------------------------------
	ボス形態変更時の共通ルーチン
	スペカ撃破後アイテム出す。
---------------------------------------------------------*/
//	#define alice_put_items 	common_boss_put_items
//	#define aya_put_items		common_boss_put_items
//	#define mima_put_items		common_boss_put_items
//	#define kaguya_put_items	common_boss_put_items
//	#define pache_put_items 	common_boss_put_items
//	h->callback_mover			= move_aya;
//	h->callback_mover			= move_alice;
//	h->callback_loser			= aya_put_items;

//
global void common_boss_put_items(SPRITE *src)
{
//++	pd_bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
	#if (0)/*あやや*/
	bakuhatsu_add_circle(src, 1);
	#endif
//
	voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*テキトー*/
	#if (1)
	/*---------------------------------------------------------
		スペカ名称表示
		スペカ背景は、まだ無いけど、将来的に拡張予定。
	---------------------------------------------------------*/
	//static void draw_spell_card_name(void)
	{
		kanji_window_clear_line(1); 	/* 漢字ウィンドウの2行目(==1)の内容を消す。 */
		set_cursor(0, 1);				/* カーソルを2行目(==1)へ移動 */
	//
		if (NULL != my_spell_card_resource[(spell_card_number)].spell_str_name)
		{
			/* スペカ背景がある場合 */
			callback_gu_draw_haikei = callback_gu_draw_haikei_supeka;
			//
			msg_time = (60*5);
			print_kanji000(
				my_spell_card_resource[(spell_card_number)].spell_str_name,
				/*int color_type*/(7)|STR_CODE_NO_ENTER,	/* 改行しない */
				/*int wait*/(0)
			);
		}
		else
		{
			/* スペカ背景が無い場合 */
			callback_gu_draw_haikei = callback_gu_draw_haikei_modosu;
		}
	}
	#endif
	/*--- BOSS 共通して値を 0 にする。------------*/
	src->boss_time_out = (0);
//	src->boss_base_state777 = (0);/* 共通して 値を 0 にする */
//	player_dummy_add_score(src->boss_base_score);
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
//	SPRITE *obj_boss;
//	obj_boss = &obj00[FIX_OBJ_08_BOSS];
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
		/* 8192/32 == 256 */
		test_draw_boss_hp_value = (global_obj_boss->base_hp>>5) & 0xff;
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
	global_obj_boss->base_hp -= boss_hp_dec_by_frame;
	boss_hp_dec_by_frame = 0;/*使ったので消す(フレーム単位)*/
	if (spell_card_limit_health >= global_obj_boss->base_hp)		/* 規定値以下になればスペカモード解除 */
	{
		spell_card_mode 			= (SPELL_CARD_MODE_00_OFF);
		boss_destroy_check_type(global_obj_boss, DESTROY_CHECK_00_WIN_BOSS);
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
	src->cx256 += (src->vx256);
	src->cy256 += (src->vy256);
}
global void boss_clip_rect(SPRITE *src)
{
		 if (src->cx256 < boss_clip_min.x256 )	{	src->cx256 = boss_clip_min.x256; 	boss_hamidasi=1;	}
	else if (src->cx256 > boss_clip_max.x256 )	{	src->cx256 = boss_clip_max.x256; 	boss_hamidasi=1;	}
//
		 if (src->cy256 < boss_clip_min.y256 )	{	src->cy256 = boss_clip_min.y256; 	boss_hamidasi=1;	}
	else if (src->cy256 > boss_clip_max.y256 )	{	src->cy256 = boss_clip_max.y256; 	boss_hamidasi=1;	}
}

// 現状のシナリオスクリプト規格だとスペカシステムと相性が悪い。
// 仕方ないので、強引にフラグで対応させる。
#define USE_OLD_SCRIPT_SYSTEM (1)

/*---------------------------------------------------------
	スペカを次に進める。
---------------------------------------------------------*/
static void spell_card_incliment(void)
{
			spell_card_number += (4)/*1*/;
}

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
			aaa = spell_card_limit_health - (my_spell_card_resource[(spell_card_number+(4)/*1*/)].spell_life);	/*1000 500*/
		//	spell_card_limit_health -= 1000/*500*/;
			/* ボスがスペカを撃てる一定体力がある場合 */
			if (0 < aaa)
			{
				spell_card_limit_health = aaa;
			//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* 形態変更したら */
				spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/
				spell_card_incliment();
			//	if (spell_card_syoji_maisuu < spell_card_number)
				if (0 == spell_card_syoji_maisuu )
				{
					/* 形態変更しない、アイテム吐かない */
					spell_card_number -= (4);
				//	src->callback_loser 			= lose_boss;
				}
				else
		//		if (0==sakuya->base_hp)
				{
					spell_card_syoji_maisuu--;
		//			if (0 != (data->boss_base.boss_life))/* 形態変更したら */
					/* アイテム吐く */
					if (NULL != src->callback_loser)
					{
		//				data->boss_base.boss_life--;
						(src->callback_loser)(src); 	/* sakuya_put_items(src); */
					}
				}
				#if 0/* (旧[スペカ終わったら初期化]) (形態変更する前に必ず初期化したいのでこの位置は止める。)*/
				/* スペルカード初期化 */
				if (NULL != my_spell_card_resource[(spell_card_number)].spell_init_callback)
				{
					(my_spell_card_resource[(spell_card_number)].spell_init_callback)(src);
				}
				#endif
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
	//	src->target_y256			= src->cy256;
		src->target_y256			= t256(16.0); //t256(0);
		#if 1/* (新[スペカ始まる前に初期化]) 第0形態から、必ず呼ぶ筈。 */
		/* スペルカード初期化 */
		if (NULL != my_spell_card_resource[(spell_card_number)].spell_init_callback)
		{
			(my_spell_card_resource[(spell_card_number)].spell_init_callback)(src);
		}
		#endif
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

	//	if (t256(/*(GAME_WIDTH/2)-32*/144) > src->cx256) 	/* 画面半分の位置より若干左。 */
	//	{
	//		src->cx256 += t256(1.0); 	/* 右 に退避 */
	//	}
	//	else
	//	if (t256(/*(GAME_WIDTH/2)+32*/208) < src->cx256) 	/* 画面半分の位置より若干右。 */
	//	{
	//		src->cx256 -= t256(1.0); 	/* 左 に退避 */
	//	}

/*---------------------------------------------------------
	[スペカシステム内に移動予定]
---------------------------------------------------------*/

//global /*static*/ void NULL(SPRITE *src)
//{
//	/* 現在ダミー */
//}
/*global*/static /*static*/ void common_03_keitai(SPRITE *src)
{
//		if ( ((pd.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* 会話終了? */
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
			spell_card_incliment();
		}
}
/*global*/static /*static*/ void common_02_keitai(SPRITE *src)
{
	//	if ( ((pd.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* 会話終了? */
	//	{
	//		my_spell_card_resource[(spell_card_number)].spell_move_callback = common_03_keitai;
	//	}
}
global /*static*/ void common_00_keitai(SPRITE *src)
{
//	if (0 == src->vvv256)
	if (0 > src->boss_base_state777)
	{
		src->boss_base_state777 = (0);
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
	if (0 > (src->cy256+t256(50.0)))/* +t256(50.0) ボスグラの最大サイズ(50[dot]) */
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
		src->cy256 -= t256(0.75);					/* 上に退避 */			/* t256(0.75) 退避速度 0.75[dot/flame] */	/*fps_factor*/
		if ( (int)(GAME_WIDTH*256/2) > src->cx256)	/* 画面半分の位置 */
		{
			src->cx256 += t256(1.0); 	/* 右上に退避 */
		}
		else
		{
			src->cx256 -= t256(1.0); 	/* 左上に退避 */
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
		src->boss_base_state777++;
	}
}




/*---------------------------------------------------------
	ボスの共通、１回目初期化ルーチン(初回、攻撃不可)
---------------------------------------------------------*/


/*---------------------------------------------------------
	弾幕の範囲を「標準」に設定
---------------------------------------------------------*/
global void set_default_bullet_clip(void)/* call from load_stage.c */
{
	bullet_clip_min.x256 = t256(-32);
	bullet_clip_max.x256 = t256(0)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-32);
	bullet_clip_max.y256 = t256(0)+t256(GAME_HEIGHT);
}


/*---------------------------------------------------------
	弾幕の範囲を「上と横の広範囲」に設定
---------------------------------------------------------*/
global void set_aya_bullet_clip(void)/* call from load_stage.c */
{
	bullet_clip_min.x256 = t256(-100);
	bullet_clip_max.x256 = t256(100)+t256(GAME_WIDTH);
	bullet_clip_min.y256 = t256(-256);/* あや */
	bullet_clip_max.y256 = t256(100)+t256(GAME_HEIGHT);
}
//global void set_default_bullet_clip(void);/* call from load_stage.c */
//global void set_aya_bullet_clip(void);/* */

global void init_00_boss_clip000(SPRITE *h)/* 標準タイプ */
{
	set_default_bullet_clip();	/* 弾幕の範囲を標準に設定 */
}
global void init_00_boss_clip111(SPRITE *h)/* 上に広いタイプ */
{
	set_aya_bullet_clip();	/* 弾幕の範囲を標準に設定 */
}

	//------------ 特殊初期化
	#if 0
	/*	common_boss_init(); より後の必要がある*/
	{
		/* boss_rect_init */
		boss_clip_min.x256	= t256(0.0);
		boss_clip_min.y256	= t256(0.0);
		boss_clip_max.x256	= t256((GAME_WIDTH-50));
		/*
		0	easy:	128 = 128+(32x0) (踏まれない)
		1	normal: 160 = 128+(32x1) (踏まれない)
		2	hard:	192 = 128+(32x2) (踏まれない)
		3	luna:	224 = 128+(32x3) (踏まれる)
		*/
	//	boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+5)));	/*t256(96.0)*/
		/* (踏まれない)
		0	easy:	128 = 128+(16x0) (踏まれない)
		1	normal: 144 = 128+(16x1) (踏まれない)
		2	hard:	160 = 128+(16x2) (踏まれない)
		3	luna:	176 = 128+(16x3) (踏まれない)
		*/
		boss_clip_max.y256	= (t256(128.0)+(difficulty<<(8+4)));	/*t256(96.0)*/
	}
	#endif

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
/* from game_core.c */
global void add_boss_common(STAGE_DATA *l)
{
		pd.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
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
	h									= global_obj_boss;/*輝夜本人*/


//	SPRITE *obj_boss;
//	obj_boss = &obj00[FIX_OBJ_08_BOSS];
//	SPRITE *h;
//	h = &obj00[FIX_OBJ_08_BOSS];

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
		h->cx256 						= BOSS_XP256;/*t256(GAME_WIDTH/2)*/
		h->cy256 						= t256(-100);
	//
	//	h->boss_base_state777			= (0);	/*ST_00*/
		h->boss_base_state777			= (-1); /*ST_00*/
	//
		/* 3面の場合---- */ 	/* 全部一緒 */
		/* 4面の場合---- */ 	/* 輝夜本人 */
		{
			static const unsigned int aaa_score_tbl[8/*8*/] =
			{
				score(1000000), 		/* チルノ */	/* エキストラ用(boss0) */
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
			/* ボス開始スペカ番号(easy) */
				SPELL_CARD_00E_chrno_000,	/* チルノ(?) */ 	/* エキストラ用(boss0) */
				SPELL_CARD_00E_alice_000,	/* アリス */
				SPELL_CARD_00E_mima_000,	/* 魅魔 */
				SPELL_CARD_00E_kaguya_000,	/* 輝夜 */
			//
				SPELL_CARD_00E_aya_000, 	/* 文 */
				SPELL_CARD_00E_pache_000,	/* パチェ */
				SPELL_CARD_00E_sakuya_000,	/* 咲夜 */
				SPELL_CARD_00E_pache_000,	/* フラン(?) */ 	/* ファンタズム用(boss7) */
			/* ボススペカ所持数 */
				(6),//(0-0),													/* チルノ(?) */ 	/* エキストラ用(boss0) */
				(5),//(SPELL_CARD_16E_alice_ggg-SPELL_CARD_00E_alice_000),		/* アリス */		/*SPELL_CARD_19_alice_jjj;*/
				(5),//(SPELL_CARD_17E_mima_hhh-SPELL_CARD_00E_mima_000),		/* 魅魔 */
				(6),//(SPELL_CARD_17E_kaguya_hhh-SPELL_CARD_00E_kaguya_000),	/* 輝夜 */
			//
				(6),//(SPELL_CARD_19E_aya_jjj-SPELL_CARD_00E_aya_000),			/* 文 */
				(7),//(SPELL_CARD_17E_pache_hhh-SPELL_CARD_00E_pache_000),		/* パチェ */
				(9),//(SPELL_CARD_11E_sakuya_kkk-SPELL_CARD_00E_sakuya_000),	/* 咲夜 */
				(5),//(0-0),													/* フラン(?) */ 	/* ファンタズム用(boss7) */
			//
			};
			spell_card_number				= aaa[sss  ]+(difficulty&0x03);
			spell_card_syoji_maisuu 		= aaa[sss+8];
		}
		/* spell_card common init */
	//	{
	//	//	spell_card_limit_health 	= 0;
	//	//	spell_card_mode 			= 0;/*off*/
	//	}
		{
			h->base_hp				= (my_spell_card_resource[(spell_card_number)].spell_life); 		/* 全体の体力 */
			spell_card_limit_health = (h->base_hp) - (my_spell_card_resource[(spell_card_number + (4)/*1*/)].spell_life);		/* 通常攻撃(初回攻撃)の攻撃分 */
			//
			spell_card_boss_timer	= (((my_spell_card_resource[(spell_card_number)].spell_limit_time)));	/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
			spell_card_mode 		= (SPELL_CARD_MODE_01_IDO_JYUNNBI);/*on*/	/* 特殊？ */
			/*???*/
			my_spell_card_resource[(spell_card_number)].spell_move_callback = common_00_keitai;
		}
		#if 1
	//------------ 弾幕関連
		h->boss_base_danmaku_type		= DANMAKU_00;	/* 弾幕生成終了フラグ */
		h->boss_base_danmaku_test		= 0;			/*(DANMAKU_08_rumia-1)*/ /*0*/
		#endif
	//
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
