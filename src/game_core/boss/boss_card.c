
#include "boss.h"


/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	カード管理システム
	-------------------------------------------------------
	カードはシステムでサポートしているので、どのボスが、どのカードでも撃てます。
	-------------------------------------------------------
	ToDo:
	★ 攻撃の管理
	★ 動作の管理(結構未定)
	★ 雑魚(中雑魚)がカードを撃てる機能は、廃止した方が良いかも知れない。
	これ(中雑魚＆中雑魚カード機能)を廃止して、代わりに道中にボスを出現させる(つまり中-ボス)
	の方が、システムがすっきりして良い気がする。そうする為には色々改造個所がある。
	現状の dat の規格だと、これらは出来ないので、主に dat の規格を変えて、
	dat からボスの会話を起動するようにする必要がある。
	プレイヤーに拠って会話が違うので、その辺の規格策定はまだ未定。(分岐シナリオとか)
	-------------------------------------------------------
	★ 雑魚(中雑魚)がカードを撃てる機能は、廃止して、
	雑魚(中雑魚)がカードを撃ちたい場合は、「カードが撃てるザコ」という物を新設してそれを使い
	システムではサポートしない方が良いと思う。のでそういう方向で改造予定。
	-------------------------------------------------------
	GAME_X_OFFSET 対応。
---------------------------------------------------------*/

#include "kanji_system.h"

/*---------------------------------------------------------
	ボス追加
---------------------------------------------------------*/

// チルノ EX1面
extern void cirno_02_keitai(SPRITE *src);
// 咲夜 6面
extern void sakuya_11_keitai(SPRITE *src);
extern void sakuya_10_keitai(SPRITE *src);
extern void sakuya_09_keitai(SPRITE *src);
extern void sakuya_08_keitai(SPRITE *src);
extern void sakuya_07_keitai(SPRITE *src);
extern void sakuya_06_keitai(SPRITE *src);
extern void sakuya_04_keitai(SPRITE *src);
//tern void sakuya_03_keitai(SPRITE *src);/* 同じ */
extern void sakuya_02_keitai(SPRITE *src);
//tern void sakuya_01_keitai(SPRITE *src);/* 同じ */

// パチェ 5面

//レーザー
extern void add_laser_off(SPRITE *src);
extern void add_laser_on(SPRITE *src);

// 文 4面
extern void aya_05_keitai(SPRITE *src);
//tern void aya_04_keitai(SPRITE *src);/* 同じ */
//tern void aya_03_keitai(SPRITE *src);/* 同じ */
//tern void aya_02_keitai(SPRITE *src);/* 同じ */
//tern void aya_01_keitai(SPRITE *src);/* 同じ */

// 輝夜 3面
extern void add_zako_kaguya_houmotsu(SPRITE *src);
extern void add_zako_kaguya_dolls02(SPRITE *src);
extern void add_zako_kaguya_dolls01(SPRITE *src);
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
extern void boss_move_05_xy_douki_differential64(SPRITE *src);
extern void boss_move_04_xy_douki_differential32(SPRITE *src);
extern void boss_move_03_x_douki(SPRITE *src);
extern void boss_move_02_xy_hidouki(SPRITE *src);
extern void boss_move_01_not_move(SPRITE *src);

/*(会話中の形態)*/
extern void kaiwa_00_keitai(SPRITE *src);
//extern void common_99_keitai(SPRITE *src);/* 撃破後に画面外にボスが逃げる */


/* 共通部 */
global void init_00_boss_clip000(SPRITE *h);/* 標準タイプ */
global void init_00_boss_clip111(SPRITE *h);/* 上に広いタイプ */




//------------ カード関連
#include "card_address.h"
#if (1)
	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
	/* AYA５形態の場合。(r35-)のシステムで通常弾を利用する為に、カードシステムの初期化が必要。 */
	/* したがって、どんな形態でも(r35以降は)必ずスペカにする必要がある。 */
	#define init_48_r34_gokan_kinou 	card_maikai_init_and_get_spell_number
	#if (1)
		#define SPELL_r34_GOKAN_KINOU		SPELL_48_r34_gokan_kinou/*(必要)*/
	#else
	//	#define SPELL_r34_GOKAN_KINOU		SPELL_00/*(ダメ)*/
	#endif
#endif
//
// int card.limit_health;	/* 規定値以下になれば カードモード解除 */
// int card.boss_state; 	/* 負値になればボス カードモードに入らない */
// int card.mode;			/* カードモード */
// int card.boss_timer; 	/* 共用 */	// 制限時間
// int card.number; 		/* 共用 */	// カード番号

global CARD_SYSTEM_GLOBAL_CLASS card;
/*global*/ static int card_syoji_maisuu;		/* 共用 */	// カード番号最大限界値



typedef struct
{
	int spell_life; 							/* カードに登録された一定体力 */
	int spell_limit_time;						/* カードの制限時間。(カードに登録された一定時間) */
//
	const char *spell_str_name; 				/* カード名称 */
	int card_number;							/* カードの種類 */
//
	void (*spell_init_callback)(SPRITE *sss);			/* 初期化移動処理 */
	void (*boss_move_keitai_callback)(SPRITE *sss); 	/* ボス移動形態選択処理 */
} CARD_ADDRESS_RESOURCE;
//	void (*spell_yuudou_callback)(SPRITE *sss); 		/* カード誘導移動処理 */
//廃止	void (*spell_tamakesi04_callback)(void);		/* カード弾画面外処理(弾消し / 弾反射 / ...) */

	/* 名前はテキトーです */

/* カード時間(==単位[フレーム])は、64倍(==単位[約1秒])が規格 */
#define s_time(aaa) (((int)(aaa))<<6)

/* ボス体力単位は、1024倍を単位にしてみる */
#define s_hp(aaa) (((int)(aaa))<<10)

#if 0
	// 32でゲージ1本(r32)
	32768==1024*32==s_hp(32)
//
311296==32768*9.5
294912==32768*9.0
262144==32768*8.0
237568==32768*7.25
229376==32768*7.0
196608==32768*6.0
163840==32768*5.0
147456==32768*4.5
131072==32768*4.0
114688==32768*3.5
98304==32768*3.0
81920==32768*2.5
65536==32768*2.0
49152==32768*1.5
#endif
/*---------------------------------------------------------
	氷符「アイシクルフォール」動かないでカードを撃つ。
---------------------------------------------------------*/

static CARD_ADDRESS_RESOURCE my_card_resource[CARD_ADDRESS_MAX] =
{									// 最大半角で30文字。
// チルノ エキストラステージ		"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
//	3.5
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_10_cirno, 					NULL,						boss_move_02_xy_hidouki,				},	/* "第一形態: " */	/* 初回の攻撃分(手動設定)  500 160	*/
//	3.0
	{	s_hp(24),	s_time(20), 	"　　氷符「アイシクルフォール」" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "第二形態: " */
	{	s_hp(24),	s_time(20), 	"　　氷符「アイシクルフォール」" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "第二形態: " */
	{	s_hp(24),	s_time(20), 	"　　氷符「アイシクルフォール」" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "第二形態: " */
	{	s_hp(24),	s_time(20), 	"　　氷符「アイシクルフォール」" "\n",	SPELL_1b_cirno_icecle_fall, 		NULL,						boss_move_01_not_move,					},	/* "第二形態: " */
//	2.25
	{	 s_hp(8),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第三形態: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第三形態: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第三形態: " */
	{	 s_hp(8),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第三形態: " */
//	2.0
	{	s_hp(16),	s_time(20), 	"　凍符「パーフェクトフリーズ」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "第四形態: " */
	{	s_hp(16),	s_time(20), 	"　凍符「パーフェクトフリーズ」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "第四形態: " */
	{	s_hp(16),	s_time(20), 	"　凍符「パーフェクトフリーズ」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "第四形態: " */
	{	s_hp(16),	s_time(20), 	"　凍符「パーフェクトフリーズ」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},	/* "第四形態: " */
//	1.5
	{	s_hp(32),	s_time(20), 	"雪符「ダイアモンドブリザード」" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "第五形態: " */
	{	s_hp(32),	s_time(20), 	"雪符「ダイアモンドブリザード」" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "第五形態: " */
	{	s_hp(32),	s_time(20), 	"雪符「ダイアモンドブリザード」" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "第五形態: " */
	{	s_hp(32),	s_time(20), 	"雪符「ダイアモンドブリザード」" "\n",	SPELL_12_diamond_blizzard,			NULL,						boss_move_02_xy_hidouki,				},	/* "第五形態: " */
//	0.0
	// 咲夜easyは短い上に段階が少ない。
// 咲夜 6面 						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*2.0),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
	{s_hp(32*3.5),	s_time(900),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},	/* 73728==8192*9.0 */
//	9.5
	{	s_hp(6),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_01_sakuya_misogi_normal,		NULL,						boss_move_04_xy_douki_differential32,	},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	s_hp(10),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_01_sakuya_misogi_normal,		NULL,						boss_move_04_xy_douki_differential32,	},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	s_hp(10),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_01_sakuya_misogi_normal,		NULL,						boss_move_04_xy_douki_differential32,	},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
	{	s_hp(10),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_2d_sakuya_misogi_lunatic, 	NULL,						boss_move_04_xy_douki_differential32,	},	/* "第一形態: 通常攻撃(左右に動いて禊弾撃ち)"		*/	/* 初回の攻撃分(手動設定)  500 160	*/
//	9.0
	{	s_hp(3),	s_time(10), 	"　　奇術「ミスディレクション」" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	s_hp(4),	s_time(10), 	"　　奇術「ミスディレクション」" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	s_hp(5),	s_time(10), 	"奇術「幻惑ミスディレクション」" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
	{	s_hp(5),	s_time(10), 	"奇術「幻惑ミスディレクション」" "\n",	SPELL_0d_sakuya_miss_direction, 	NULL,						sakuya_02_keitai,						},	/* "第二形態: 奇術「ミスディレクション」(全方位、豆まき)"	*/
//	8.25	// bloody==血だらけの。 staind ステインド==ステンド==(色を)焼き付け(て残す)る。
	{	 s_hp(3),	s_time(20), 	NULL,/* "通常攻撃3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "第三形態: " " 　　　　時象「ペッパーナイフ」"		*/
	{	 s_hp(4),	s_time(20), 	NULL,/* "通常攻撃3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "第三形態: " " 　　　　時雨「ステンドナイフ」"		*/
	{	 s_hp(4),	s_time(20), 	NULL,/* "通常攻撃3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "第三形態: " " 　　五月雨「ブラッディナイフ」"		*/
	{	 s_hp(4),	s_time(20), 	NULL,/* "通常攻撃3" "\n"*/				SPELL_2c_sakuya_blue_red,			NULL,						boss_move_04_xy_douki_differential32,	},	/* "第三形態: " " 　　時砂「パーフェクトナイフ」"		*/
//	8.0
	{	s_hp(6),	s_time(20), 	NULL,/* "通常攻撃"白緑弾 */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "第四形態: 魔方陣生成"	*/
	{	s_hp(9),	s_time(20), 	NULL,/* "通常攻撃"白緑弾 */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "第四形態: 魔方陣生成"	*/
	{	s_hp(9),	s_time(20), 	NULL,/* "通常攻撃"白緑弾 */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "第四形態: 魔方陣生成"	*/
	{	s_hp(9),	s_time(20), 	NULL,/* "通常攻撃"白緑弾 */ 			SPELL_2a_sakuya_baramaki1,			NULL,						sakuya_04_keitai,						},	/* "第四形態: 魔方陣生成"	*/
//	7.5 	// meek==素直。
	{	s_hp(6),	s_time(20), 	"鈍詛「ブラドツェペシュの鈍い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第五形態: (黄色マスカット弾A)"	"　　幻定「デンジャラスミーク」"	*/
	{	s_hp(9),	s_time(20), 	"呪詛「ブラドツェペシュの呪い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第五形態: (黄色マスカット弾A)"	"　幻種「デンジャラスワールド」"	*/
	{	s_hp(9),	s_time(20), 	"祝詛「ブラドツェペシュの祝い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第五形態: (黄色マスカット弾A)"	"　　幻象「デンジャラスタイム」"	*/
	{	s_hp(9),	s_time(20), 	"恐詛「ブラドツェペシュの恐い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第五形態: (黄色マスカット弾A)"	"　幻舞「デンジャラスストーム」"	*/
	//
	{	s_hp(6),	s_time(20), 	"鈍詛「ブラドツェペシュの鈍い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第六形態: (黄色マスカット弾B)"					*/
	{	s_hp(9),	s_time(20), 	"呪詛「ブラドツェペシュの呪い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第六形態: (黄色マスカット弾B)"					*/
	{	s_hp(9),	s_time(20), 	"祝詛「ブラドツェペシュの祝い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第六形態: (黄色マスカット弾B)"					*/
	{	s_hp(9),	s_time(20), 	"恐詛「ブラドツェペシュの恐い」" "\n",	SPELL_28_remilia_tamaoki1,			NULL,						boss_move_02_xy_hidouki,				},	/* sakuya_06_keitai"第六形態: (黄色マスカット弾B)"					*/
//	6.5 	// ジャック・ザ・リッパー==Jack the Ripper==切り裂きジャック(殺人鬼)19世紀(1888年)に実在(?)/抽象名詞化。ワールドヒーローズ。 リック==スプラッターハウス
	{	s_hp(12),	s_time(20), 	"　奇抜「ジャック・ガーリック」" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第七形態: (分散魔方陣)追加計画中"				*/
	{	s_hp(16),	s_time(20), 	"奇術「ジャック・ザ・ラッパー」" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第七形態: (分散魔方陣)追加計画中"				*/
	{	s_hp(16),	s_time(20), 	"奇術「ジャック・ザ・ビーンズ」" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第七形態: (分散魔方陣)追加計画中"				*/
	{	s_hp(16),	s_time(20), 	"夢違「ジャック・デ・マメマキ」" "\n",	SPELL_0c_sakuya_jack32, 			NULL,						boss_move_02_xy_hidouki,				},	/* "第七形態: (分散魔方陣)追加計画中"				*/
//
	{	s_hp(8),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "第四形態: 魔方陣生成"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "第四形態: 魔方陣生成"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "第四形態: 魔方陣生成"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_2b_sakuya_baramaki2,			NULL,						sakuya_08_keitai,						},	/* "第四形態: 魔方陣生成"	*/
//	4.5
	{	s_hp(8),	s_time(20), 	"　幻想「フェスティバルナイフ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "第九形態: 最終形態(その1)"					*/
	{	s_hp(12),	s_time(20), 	"　幻葬「フェスティバルナイフ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "第九形態: 最終形態(その1)"					*/
	{	s_hp(12),	s_time(20), 	"　幻奏「フェスティバルナイフ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "第九形態: 最終形態(その1)"					*/
	{	s_hp(12),	s_time(20), 	"　幻惑「フェスティバルナイフ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	sakuya_09_keitai,						},	/* "第九形態: 最終形態(その1)"					*/
//	2.5
	{	s_hp(8),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "第10形態: 最終形態(その2)"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "第10形態: 最終形態(その2)"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "第10形態: 最終形態(その2)"	*/
	{	s_hp(12),	s_time(20), 	NULL,									SPELL_0e_remilia_00,				NULL,						sakuya_10_keitai,						},	/* "第10形態: 最終形態(その2)"	*/
//	0.5
	{	 s_hp(8),	s_time(20), 	"　　メイド秘密「残虐行為手当」" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "第11形態: 最終形態(その3)"					*/	// (easy)実はここにはこない
	{	 s_hp(8),	s_time(20), 	"　　メイド秘技「鯱！鯱！鯱！」" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "第11形態: 最終形態(その3)"					*/
	{	 s_hp(8),	s_time(20), 	"　メイド秘宝「あつくて死ぬぜ」" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "第11形態: 最終形態(その3)"					*/
	{	 s_hp(8),	s_time(20), 	"　　メイド日々「もうすぐボス」" "\n",	SPELL_0e_remilia_00,				NULL,						sakuya_11_keitai,						},	/* "第11形態: 最終形態(その3)"					*/
//	0.25
//	{	s_hp(16),	s_time(20), 	"　　　　奇術「咲夜テストE008」" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	{	s_hp(24),	s_time(20), 	"　　　　奇術「咲夜テストN008」" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	{	s_hp(24),	s_time(20), 	"　　　　奇術「咲夜テストH008」" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "第八形態: (時止めナイフ)追加計画中" 		*/
//	{	s_hp(24),	s_time(20), 	"　　　　奇術「咲夜テストL008」" "\n",	SPELL_00,							NULL,						common_00_keitai,						},	/* "第八形態: (時止めナイフ)追加計画中" 		*/

	// パチェeasyは短い上に1段階少ない。
// A(霊符)
// パチェA 5面						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*2.00), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"　　　　火符「アグニシャイン」" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				boss_move_03_x_douki,					},
	{	s_hp(16),	s_time(30), 	"　　　　火符「アグニシャイン」" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　　火符「アグニシャイン上級」" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　　火符「アグニシャイン上級」" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				boss_move_02_xy_hidouki,				},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"　　土符「レイジィトリリトン」" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				boss_move_01_not_move,					},/*(テスト)*/
	{	s_hp(16),	s_time(30), 	"土符「レイジィトリリトン上級」" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				boss_move_01_not_move,					},/*(テスト)*/
	{	s_hp(16),	s_time(30), 	"　　土符「トリリトンシェイク」" "\n",	SPELL_39_pache_tririton_shake,		add_laser_off,				boss_move_01_not_move,					},/*(テスト)*/
	{	s_hp(16),	s_time(30), 	"　　土符「トリリトンシェイク」" "\n",	SPELL_39_pache_tririton_shake,		add_laser_off,				boss_move_01_not_move,					},/*(テスト)*/
//	1.0
	{	s_hp(10),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
//	0.0
	{	s_hp(10),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},	// (easy)実はここにはこない
	{	s_hp(16),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},

// B(夢符)
// パチェB 5面						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*2.00), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"　水符「プリンセスウンディネ」" "\n",	SPELL_2f_pache_princess_undine, 	add_laser_off,				boss_move_04_xy_douki_differential32,	},
	{	s_hp(16),	s_time(30), 	"　水符「プリンセスウンディネ」" "\n",	SPELL_2f_pache_princess_undine, 	add_laser_off,				boss_move_04_xy_douki_differential32,	},
	{	s_hp(16),	s_time(30), 	"　　　水符「ベリーインレイク」" "\n",	SPELL_37_pache_bury_in_lake,		add_laser_off,				boss_move_04_xy_douki_differential32,	},
	{	s_hp(16),	s_time(30), 	"　　　水符「ベリーインレイク」" "\n",	SPELL_37_pache_bury_in_lake,		add_laser_off,				boss_move_02_xy_hidouki,				},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"　　木符「シルフィホルン上級」" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　木符「シルフィホルン上級」" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　　木符「グリーンストーム」" "\n",	SPELL_38_pache_green_storm, 		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　　木符「グリーンストーム」" "\n",	SPELL_38_pache_green_storm, 		add_laser_off,				boss_move_01_not_move,					},
//	1.0
	{	s_hp(10),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
//	0.0
	{	s_hp(10),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},	// (easy)実はここにはこない
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},

// C(魔符)
// パチェC 5面						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*2.00), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"　　　　木符「シルフィホルン」" "\n",	SPELL_30_pache_sylphy_horn_1,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　　　木符「シルフィホルン」" "\n",	SPELL_30_pache_sylphy_horn_1,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　木符「シルフィホルン上級」" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　木符「シルフィホルン上級」" "\n",	SPELL_34_pache_sylphy_horn_2,		add_laser_off,				boss_move_01_not_move,					},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"　　　　火符「アグニシャイン」" "\n",	SPELL_2e_pache_agni_shine_1,		add_laser_off,				boss_move_03_x_douki,					},
	{	s_hp(16),	s_time(30), 	"　　火符「アグニシャイン上級」" "\n",	SPELL_33_pache_agni_shine_2,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　火符「アグニレイディアンス」" "\n",	SPELL_36_pache_agni_radiance,		add_laser_off,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　火符「アグニレイディアンス」" "\n",	SPELL_36_pache_agni_radiance,		add_laser_off,				boss_move_02_xy_hidouki,				},
//	1.0
	{	s_hp(10),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"木＆火符「フォレストブレイズ」" "\n",	SPELL_3c_pache_forest_blaze,		NULL,						boss_move_01_not_move,					},
//	0.0
	{	s_hp(10),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},	// (easy)実はここにはこない
	{	s_hp(16),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"火＆土符「ラーヴァクロムレク」" "\n",	SPELL_3b_pache_lava_cromlech,		NULL,						boss_move_02_xy_hidouki,				},

// D(恋符)
// パチェD 5面						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*2.00), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
	{s_hp(32*3.75), s_time(800),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 40960==8192*5.0 */
//	5.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_42_PACHE_LASER1,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	4.0
	{	s_hp(10),	s_time(30), 	"　　土符「レイジィトリリトン」" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　土符「レイジィトリリトン」" "\n",	SPELL_31_pache_rage_tririton_1, 	add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"土符「レイジィトリリトン上級」" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"土符「レイジィトリリトン上級」" "\n",	SPELL_35_pache_rage_tririton_2, 	add_laser_off,				boss_move_01_not_move,					},
//	3.0
	{	s_hp(10),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_43_PACHE_LASER2,				add_laser_on,				boss_move_02_xy_hidouki,				},
//	2.0
	{	s_hp(10),	s_time(30), 	"　　金符「メタルファティーグ」" "\n",	SPELL_32_pache_metal_fatigue,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　金符「メタルファティーグ」" "\n",	SPELL_32_pache_metal_fatigue,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　　金符「シルバードラゴン」" "\n",	SPELL_3a_pache_silver_dragon,		add_laser_off,				boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"　　　金符「シルバードラゴン」" "\n",	SPELL_3a_pache_silver_dragon,		add_laser_off,				boss_move_01_not_move,					},
//	1.0
	{	s_hp(10),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"土＆金符「エメラルドメガリス」" "\n",	SPELL_3f_pache_emerald_megalith,	NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
	{	s_hp(16),	s_time(30), 	"　水＆木符「ウォーターエルフ」" "\n",	SPELL_3d_pache_water_elf,			NULL,						boss_move_02_xy_hidouki,				},
//	0.0
	{	s_hp(10),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},	// (easy)実はここにはこない
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},
	{	s_hp(16),	s_time(30), 	"金＆水符「マーキュリポイズン」" "\n",	SPELL_3e_pache_mercury_poison,		NULL,						boss_move_01_not_move,					},

//	{	s_hp(10),	s_time(99), 	" 　月符「サイレント未作成E06」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" 　月符「サイレント未作成N06」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" 　月符「サイレント未作成H06」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" 　月符「サイレント未作成L06」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},

//	{	s_hp(10),	s_time(99), 	" 　氷符「パチュリー未作成E07」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" 　氷符「パチュリー未作成N07」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" 　氷符「パチュリー未作成H07」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},
//	{	s_hp(16),	s_time(99), 	" 　氷符「パチュリー未作成L07」" "\n",	SPELL_00,							NULL,						boss_move_01_not_move,					},

//	SPELL_15_aya_misogi
//	SPELL_25_houka_kenran			/*aya_02_keitai*/
//	SPELL_23_meirin_magaru_kunai	/*aya_01_keitai*/

// 文 4面							"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	//																											/*ボス登場前の初期化[会話の前]*/
	{s_hp(32*3),	s_time(600),	NULL,/* "形態変更" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "形態変更" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "形態変更" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	{s_hp(32*3),	s_time(600),	NULL,/* "形態変更" "\n"*/				SPELL_00,							NULL,						kaiwa_00_keitai,						},/* 32768=32768.0 */
	//	4.0 																									/*ボス戦闘前の初期化[会話の後]*/
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	{	s_hp(16),	s_time(20), 	NULL,/* "通常攻撃1" "\n"*/				SPELL_20_aya_misogi1,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(4)*/
	//	3.5
	{	s_hp(8),	s_time(30), 	"　　　　　　　岐符「天の八衢」" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	{	s_hp(8),	s_time(30), 	"　　　　　　　岐符「天の八衢」" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	{	s_hp(8),	s_time(30), 	"　　　　　　　岐符「天の八衢」" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	{	s_hp(8),	s_time(30), 	"　　　　　　　岐符「天の八衢」" "\n",	SPELL_22_ame_no_yatimata,			init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)*/
	//	3.0
	{	s_hp(10),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"岐符「未作成カード001」"*/
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"岐符「未作成カード001」"*/
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"岐符「未作成カード001」"*/
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃2" "\n"*/				SPELL_21_aya_misogi2,				init_00_boss_clip000,		boss_move_03_x_douki,					},/*(5)"岐符「未作成カード001」"*/
	//	2.5
	{	 s_hp(4),	s_time(40), 	"　　　　　　摘符「穎割大好評」" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"岐符「未作成カード002」"*/
	{	 s_hp(4),	s_time(40), 	"　　　　　風説「穎割大根被害」" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"岐符「未作成カード002」"*/
	{	 s_hp(4),	s_time(40), 	"　　　　　　過剰「山葵大被害」" "\n",	SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"岐符「未作成カード002」"*/
	{	 s_hp(4),	s_time(40), 	"　　　　　大量「麦酒十\升呑過」" "\n", SPELL_24_aya_doll,					NULL,						boss_move_03_x_douki,					},/*(4)"岐符「未作成カード002」"*/
	//	2.0
	{	s_hp(16),	s_time(100),	"　　　　　　　実況「風速３Ｍ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"岐符「未作成カード003」"*/
	{	s_hp(16),	s_time(100),	"　　　　　　実況「風速３３Ｍ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"岐符「未作成カード003」"*/
	{	s_hp(16),	s_time(100),	"　　　　　　実況「風速９８Ｍ」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"岐符「未作成カード003」"*/
	{	s_hp(16),	s_time(100),	"　　　　　　実況「風速測定中」" "\n",	SPELL_r34_GOKAN_KINOU,				init_48_r34_gokan_kinou,	aya_05_keitai,							},/*(5)"岐符「未作成カード003」"*/
	//	1.5
	{	s_hp(8),	s_time(99), 	"　　　　　　路符「小耳の調べ」" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"岐符「未作成カード004」"*/
	{	s_hp(8),	s_time(99), 	"　　　　　　路符「小手の調べ」" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"岐符「未作成カード004」"*/
	{	s_hp(8),	s_time(99), 	"　　　　　　路符「胡弓の調べ」" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"岐符「未作成カード004」"*/
	{	s_hp(8),	s_time(99), 	"　　　　追跡「コロンボの調べ」" "\n",	SPELL_1d_amefuri_test,				init_00_boss_clip111,		boss_move_01_not_move,					},/*(4)"岐符「未作成カード004」"*/
	//	1.0
	{	s_hp(8),	s_time(99), 	"　　華符「うろおぼえ芳華絢爛」" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	{	s_hp(8),	s_time(99), 	"　　　　　　　塞符「山神渡御」" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	{	s_hp(8),	s_time(99), 	"　　　　塞符「天上天下の照國」" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	{	s_hp(8),	s_time(99), 	"　　　　塞符「唯我独尊の照國」" "\n",	SPELL_26_aya_saifu, 				init_00_boss_clip000,		boss_move_01_not_move,					},/*(5)*/
	//	0.5
//	{	s_hp(16),	s_time(99), 	"　　　岐符「未作成カードE006」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"　　　岐符「未作成カードN006」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"　　　岐符「未作成カードH006」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(16),	s_time(99), 	"　　　岐符「未作成カードL006」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
	//	0.0
//	{	s_hp(64),	s_time(99), 	"　　　岐符「未作成カードE007」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"　　　岐符「未作成カードN007」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"　　　岐符「未作成カードH007」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/
//	{	s_hp(64),	s_time(99), 	"　　　岐符「未作成カードL007」" "\n",	SPELL_00,							NULL,						aya_05_keitai,							},/*(5)*/

//	SPELL_1e_kaguya01		ボス行動、第 1形態
//	SPELL_1f_kaguya04		ボス行動、第 4形態
	// 3面ボス、easyはそこそこ簡単に。他はそれなりに。
// 輝夜 3面 						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*2.0),	s_time(500),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	{s_hp(32*3.0),	s_time(500),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	{s_hp(32*3.0),	s_time(500),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	{s_hp(32*3.0),	s_time(500),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 28672==8192*3.5 */
	// 3.5
	{	s_hp(8),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	{	s_hp(12),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1e_kaguya01,					boss_init_kaguya,			kaguya_01_keitai,						}, // 密着(5)還り弾、赤、青。自機狙い連弾(8)赤18セット(秒)ぐらい(始めの5秒は出さない)。(輝夜一定間隔で移動、終了後中央へ移動)
	// 3.0
	{	s_hp(8),	s_time(40), 	" 　　　　　　難題「未作成E01」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成N01」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成H01」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成L01」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 1.龍の頸の玉	(輝夜中央固定、青赤紫緑黄の丸弾、短レーザー)4セットx
	// 2.5
	{	s_hp(8),	s_time(40), 	" 　　　　　　難題「未作成E02」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.仏の御石の鉢
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成N02」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.仏の御石の鉢
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成H02」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.仏の御石の鉢
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成L02」" "\n",	SPELL_1e_kaguya01,					add_zako_kaguya_houmotsu,	kaguya_01_keitai,						},// 2.仏の御石の鉢
	// 2.0
	{	s_hp(8),	s_time(40), 	" 　　　　　　難題「未作成E03」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.火鼠の皮衣
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成N03」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.火鼠の皮衣
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成H03」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.火鼠の皮衣
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成L03」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 3.火鼠の皮衣
	// 1.5
	{	s_hp(8),	s_time(40), 	" 　　　　　　難題「未作成E04」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.燕の子安貝
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成N04」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.燕の子安貝
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成H04」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.燕の子安貝
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成L04」" "\n",	SPELL_1f_kaguya04,					add_zako_kaguya_dolls02,	kaguya_04_keitai,						},// 4.燕の子安貝
	// 1.0
	{	s_hp(8),	s_time(40), 	" 　　　　　　難題「未作成E05」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.蓬莱の玉の枝
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成N05」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.蓬莱の玉の枝
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成H05」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.蓬莱の玉の枝
	{	s_hp(12),	s_time(40), 	" 　　　　　　難題「未作成L05」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},// 5.蓬莱の玉の枝
	// 0.5
	{	s_hp(8),	s_time(40), 	" 　　　難題「蓬莱の玉の枝E06」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	{	s_hp(12),	s_time(40), 	" 　　　難題「蓬莱の玉の枝N06」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	{	s_hp(12),	s_time(40), 	" 　　　難題「蓬莱の玉の枝H06」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	{	s_hp(12),	s_time(40), 	" 　　　難題「蓬莱の玉の枝L06」" "\n",	SPELL_1c_kakuya_tamanoe,			add_zako_kaguya_dolls01,	boss_move_01_not_move,					},//
	// 2面ボスなので、適当に易しく。
// 魅魔 2面 						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*1.4),	s_time(400),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	{s_hp(32*2),	s_time(400),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 20480==8192*2.5 */
	// 2.000 (12==32*0.375) // easy(3) やたら短いのは道中で死んでパワーダウンを想定している。パワーダウンなので弾力がないが、体力が極端に少ないので次の段階に速く移行させ弾消しにつなげる。
	{	 s_hp(3),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	{	 s_hp(6),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	{	 s_hp(6),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	{	 s_hp(6),	s_time(20), 	NULL,/* "通常攻撃" "\n"*/				SPELL_1a_mima_toge, 				NULL,						mima_01_keitai, 						},
	// 2.--
	{	 s_hp(5),	s_time(20), 	" 　　　未定「未作成カードE01」" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* 通常タイプ */
	{	 s_hp(6),	s_time(20), 	" 　　　未定「未作成カードN01」" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* 通常タイプ */
	{	 s_hp(6),	s_time(20), 	" 　　　未定「未作成カードH01」" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* 通常タイプ */
	{	 s_hp(6),	s_time(20), 	" 　　　未定「未作成カードL01」" "\n",	SPELL_27_hosigata_test, 			NULL,						mima_01_keitai, 						},	/* 通常タイプ */
	// 1.625
	{	s_hp(10),	s_time(40), 	"　　闇符「ディマーケイション」" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* 上広タイプ */	// "　　 闇符「ディマーケイション」"
	{	s_hp(18),	s_time(40), 	"　　闇符「ディマーケイション」" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* 上広タイプ */
	{	s_hp(18),	s_time(40), 	"　　闇符「ディマーケイション」" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* 上広タイプ */
	{	s_hp(18),	s_time(40), 	"　　闇符「ディマーケイション」" "\n",	SPELL_29_rumia_demarcation, 		NULL/*init_00_boss_clip111*/,	boss_move_01_not_move,				},	/* 上広タイプ */
	// 1.00
	{	s_hp(12),	s_time(40), 	NULL,/* "通常攻撃" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* 上広タイプ */
	{	s_hp(10),	s_time(40), 	NULL,/* "通常攻撃" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* 上広タイプ */
	{	s_hp(10),	s_time(40), 	NULL,/* "通常攻撃" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* 上広タイプ */
	{	s_hp(10),	s_time(40), 	NULL,/* "通常攻撃" "\n"*/				SPELL_23_meirin_magaru_kunai,		init_00_boss_clip111,		boss_move_01_not_move,					},	/* 上広タイプ */
	// 1.--
	{	 s_hp(6),	s_time(40), 	"　　　　　　　華符「芳華絢爛」" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* 通常タイプ */
	{	 s_hp(8),	s_time(40), 	"　　　　　　　華符「芳華絢爛」" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* 通常タイプ */
	{	 s_hp(8),	s_time(40), 	"　　　　　　　薫符「芳薫絢爛」" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* 通常タイプ */
	{	 s_hp(8),	s_time(40), 	"　　　　　　　蘭符「芳華兼蘭」" "\n",	SPELL_25_houka_kenran,				init_00_boss_clip000,		boss_move_01_not_move,					},	/* 通常タイプ */
	// 1.--
	{	 s_hp(6),	s_time(60), 	" 　　　未定「未作成カードE05」" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	{	 s_hp(8),	s_time(60), 	" 　　　未定「未作成カードN05」" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	{	 s_hp(8),	s_time(60), 	" 　　　未定「未作成カードH05」" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	{	 s_hp(8),	s_time(60), 	" 　　　未定「未作成カードL05」" "\n",	SPELL_18_hana_test, 				NULL,						boss_move_02_xy_hidouki,				},
	// 1.0
	{	s_hp(32),	s_time(60), 	" 　　　未定「てすとカードE06」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// 都合上来ない。
	{	s_hp(32),	s_time(60), 	" 　　　未定「てすとカードN06」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// 都合上来ない。
	{	s_hp(32),	s_time(60), 	" 　　　未定「てすとカードH06」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// 都合上来ない。
	{	s_hp(32),	s_time(60), 	" 　　　未定「てすとカードL06」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},// 都合上来ない。
	// 1面ボスなので、(少なくとも easy、normal あたりは)難しく出来ない。
// アリス 1面						"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	{s_hp(32*1.125), s_time(300),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{s_hp(32*1.25),  s_time(300),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{s_hp(32*1.50),  s_time(300),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	{s_hp(32*1.50),  s_time(300),	NULL,/* "形態変更" "\n"*/				SPELL_00,							init_00_boss_clip000,		kaiwa_00_keitai,						},/* 16384==8192*2 */	/* "通常攻撃"の時間(?) */ /* "通常攻撃"のライフ */
	// 1.25 easy(10) 道中で[F]出るので、パワーは問題ない。まず始めに弾幕は簡単という事に慣れてもらう為、長め。(easyはプレイヤーに死んでもらっては困るので、ボスと戦った気にさせるのが難しい)
	{	 s_hp(10),	 s_time(40),	NULL,/* "通常攻撃" "\n"*/				SPELL_14_alice_youmu300,			NULL,						boss_move_01_not_move,					},// 明らかに簡単な弾幕
	{	  s_hp(6),	 s_time(40),	NULL,/* "通常攻撃" "\n"*/				SPELL_14_alice_youmu300,			NULL,						boss_move_01_not_move,					},
	{	  s_hp(5),	 s_time(40),	NULL,/* "通常攻撃" "\n"*/				SPELL_13_alice_suwako,				NULL,						boss_move_01_not_move,					},
	{	  s_hp(4),	 s_time(40),	NULL,/* "通常攻撃" "\n"*/				SPELL_13_alice_suwako,				NULL,						boss_move_01_not_move,					},
	// 1.--
	{	 s_hp(12),	 s_time(40),	"　　　　　懐古「紅の魔法Lv.2」" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_01_not_move,					},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 s_hp(14),	 s_time(40),	" 　　　　懐古「紅の魔法Lv.16」" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_01_not_move,					},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 s_hp(12),	 s_time(40),	"　　　　懐古「紅の魔法Lv.256」" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_04_xy_douki_differential32,	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 s_hp(10),	 s_time(40),	"　　　懐古「紅の魔法Lv.65536」" "\n",	SPELL_15_alice_aka_2nd, 			NULL,						boss_move_04_xy_douki_differential32,	},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	// 1.-- easy(4) 「パーフェクトフリーズ」は一定の事故率がある為、初心者向きでは無い。拠って不必要に短くする。
	{	  s_hp(4),	 s_time(20),	" 　　 試作「カード語のてすと」" "\n",	SPELL_44_ruiz3_test,				NULL,						boss_move_02_xy_hidouki,				},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	  s_hp(6),	 s_time(20),	"　凍符「パーフェクトフリーズ」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	  s_hp(8),	 s_time(20),	"　凍符「パーティクルフリーク」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	{	 s_hp(12),	 s_time(20),	"　凍符「ブリザードフローズン」" "\n",	SPELL_11_perfect_freeze,			NULL,						boss_move_02_xy_hidouki,				},/* "蒼符「溺愛の仏蘭西人形」"のライフ */
	// 1.00
	{	 s_hp(7),	 s_time(60),	"　蒼符「薄愛のマトリョーシカ」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　蒼符「薄愛のマトリョーシカ」"白符「博愛の円谷人形」 */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"　蒼符「溺愛のマトリョーシカ」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　蒼符「溺愛のマトリョーシカ」"白符「博愛の円谷人形」 */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"　蒼符「自戒のマトリョーシカ」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　蒼符「自戒のマトリョーシカ」"白符「博愛の円谷人形」 */ 	//(r33p)boss_move_04_xy_douki_differential
	{	 s_hp(8),	 s_time(60),	"　蒼符「自害のマトリョーシカ」" "\n",	SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　蒼符「自害のマトリョーシカ」"白符「博愛の円谷人形」 */ 	//(r33p)boss_move_04_xy_douki_differential
	// 0.--
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　　　　操符「淡い眼の案山子」"白符「博愛の円谷人形」 */
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　　　　　操符「幻の機巧人形」"白符「博愛の円谷人形」 */
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　　　　　　操符「古の流し雛」"白符「博愛の円谷人形」 */
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　操符「呪のクラッシュダミー」"白符「博愛の円谷人形」 */
//									"eeddccbbaa99887766554433221100",'\n\0' ワーク文字列バッファ長をこれだけしか用意しない予定なので、あふれたら字が出ない。 */
	#if (1)/*(r35無いとハングアップする？)*/
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　　　　操符「淡い眼の案山子」"白符「博愛の円谷人形」 */
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　　　　　操符「幻の機巧人形」"白符「博愛の円谷人形」 */
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　　　　　　操符「古の流し雛」"白符「博愛の円谷人形」 */
	{	 s_hp(8),	 s_time(60),	NULL,/* "通常攻撃z" "\n"*/				SPELL_16_alice_doll,				NULL,						boss_move_02_xy_hidouki,				},/* " 　操符「呪のクラッシュダミー」"白符「博愛の円谷人形」 */
	#endif
};
//	" 　　　蒼符「博愛の仏蘭西人形」" "\n"
//	" 　蒼符「博愛のオルレアソ\人形」" "\n" " 　蒼符「博愛のオルレアン人形」"...現在の方式では無理です。
//	{	 (1024*16),  s_time(40),	"　操符「未作成の阿蘭陀人形L1」" "\n",	SPELL_00,							NULL,						alice_04_keitai,						},/* 白符「博愛の円谷人形」 */
//	{	(20000*4),	 s_time(20),	"操符「未作成の伊太利亜人形L2」" "\n",	SPELL_00,							NULL,						boss_move_02_xy_hidouki,				},/* 炎符「浅草の人形焼き」 */
//	{	 (5000*4),	 s_time(20),	"　操符「未作成の英吉利人形L3」" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(20),	"　操符「未作成の露西亜人形L4」" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(80),	"　　操符「未作成の瑞西人形L5」" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(80),	"　操符「未作成の白耳義人形L6」" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
//	{	 (5000*4),	 s_time(80),	"　　操符「未作成の波蘭人形L7」" "\n",	SPELL_00,							NULL,						alice_02_keitai,						},
	/*
	マトリョーシカは、露西亜人形でいいのかな？人形というよりより民芸品の趣がある。
	最近は萌えマトリョーシカの需要が日本からあるらしい。
	//
	日本の気象庁は風速17m以上の風を「台風」と定義。
	国際気象機関(WMO)では風速33m以上のものを「typhoon」と定義。それ以下は「storm」。
	*/


/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部 。
---------------------------------------------------------*/
#define TEST_ZAKO_HIDE (0)
#if (1==TEST_ZAKO_HIDE)/* ボスも影響受ける */
static void sprite_sethide_all(SPRITE *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* 全部調べる。 */
	{
		s->BOSS_DATA_05_move_jyumyou	= 0xff;
		#if (1)/* デバッグてすと */
		s->cx256		= (-1);
		#endif
		s++;
	}
}
static void zako_all_timeup(void)/*int ty pe*/
{
	SPRITE *s;
	s = &obj99[OBJ_HEAD_01_TEKI+0];
	sprite_sethide_all(s, OBJ_POOL_01_TEKI_MAX);
}
#endif

/*---------------------------------------------------------
	カードシステムのボス形態に登録されたカード番号を取得し、
//	同時にカードの時間切れを設定する。
	同時にスペルの初期化を行う。

---------------------------------------------------------*/

global void card_maikai_init_and_get_spell_number(SPRITE *src)
{
	card.card_number	= my_card_resource[(card.number)].card_number;	/* カードをセット */
	//
	card_maikai_init(src);		/* カードの制限時間を設定(予めカードごとに設定されている標準時間に設定) */
}


/*---------------------------------------------------------
	ボス形態変更時の共通ルーチン
	カード撃破後アイテム出す。
---------------------------------------------------------*/

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
		カード名称表示
		カード背景は、まだ無いけど、将来的に拡張予定。
	---------------------------------------------------------*/
	//static void draw_card_name(void)
	{
		kanji_window_clear_line(ML_LINE_04);	/* 漢字ウィンドウの4行目(==3)の内容を消す。 */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_04)*(KANJI_FONT_18_HEIGHT_P2));	/* カーソルを4行目(==3)へ移動 */
	//
		if (NULL != my_card_resource[(card.number)].spell_str_name)
		{
			/* カード背景がある場合 */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_supeka;
			//
			// ""カードの譜名表示位置。
			{
			//	ml_font[(ML_LINE_04)].timer 	= ML_ON;	/*"表示ON"*/
				ml_font[(ML_LINE_04)].timer 	= byou60(5);	/* 約 5 秒 */
				ml_font[(ML_LINE_04)].x 		= (96);/* X位置 */
				ml_font[(ML_LINE_04)].y 		= (12);/* Y位置 */
			//	ml_font[(ML_LINE_04)].render	= NULL;
			//	ml_font[(ML_LINE_04)].str		= my_font_text;/*(デバッグ)*/	/*"表示on"*/
			}
			ml_font[(0)].haikei 		= (ML_HAIKEI_01_BOSS_SPELL_CARD);/* [青/ボスカード用背景]ボスカード用せりふ背景on */
			cg.msg_time = byou60(5);	/* 約 5 秒 */
			strcpy(my_font_text, my_card_resource[(card.number)].spell_str_name);
			kanji_color(/*int color_type*/(7)|STR_CODE_NO_ENTER);	/* 改行しない */
			kanji_draw();
		}
		else
		{
			/* カード背景が無い場合 */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
		}
	}
	#endif
	/*--- BOSS 共通して値を 0 にする。------------*/
	src->BOSS_DATA_05_move_jyumyou = (0);
//	src->BOSS_DATA_05_boss_base_state777 = (0);/* 共通して 値を 0 にする */
//	player_dummy_add_score(src->boss_base_score);
}


/*---------------------------------------------------------
	ボスを攻撃した場合のフレームチェック
	-------------------------------------------------------
	ライフが少なくなるとボスがなかなか死なないのは、演出だそうです。
	てきとーに再現？
	-------------------------------------------------------
	ToDo:
	う－ん、最近はライフはやっぱりリニアに減ってて、
	表示だけlogなんじゃね？って気がしてきた。
	そういう風にしようかな。ゲームバランスとか色々変わるから。
---------------------------------------------------------*/
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
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>(6))+3)<<3);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>3)+(3<<3);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>(6))+1)<<4);
	//	limit_max_hp_dec_boss_by_flame = (((test_draw_boss_hp_value>>(6)))<<4)+(1<<4);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2)+(1<<4)-4;
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2)+(16-4);
		#endif

//0 ... 255 == draw_boss_hp_value 0001 1111 1110 0000;	>>=(2+3);  0000 0000 1111 1111; 0xff;
/*
	4096/60 == 68.2666666666666666666666666666667
	4096/256 == 16
*/
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (16)
//#define LIMIT_MAX_HP_DEC_BOSS_BY_FLAME (48)
//	int LIMIT_MAX_HP_DEC_BOSS_BY_FLAME[8] = { (1), (2), (4), (8),  (16), (24), (32), (48), };


global void boss_hp_frame_check(void)
{
	SPRITE *obj_boss;
	obj_boss			= global_obj_boss;
//	obj_boss			= あたり判定の都合上無理&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
	{
		int limit_max_hp_dec_boss_by_flame;
		u8 test_draw_boss_hp_value;
	//	test_draw_boss_hp_value = (obj_boss->base_hp>>5) & 0xff;	/* 8192/32 == 256 (r32) */
		test_draw_boss_hp_value = (obj_boss->base_hp>>7) & 0xff;	/* 32768/32 == 256 (r33) */
		#if 1
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x0f);
	//	limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value>>2) | (0x08);
		limit_max_hp_dec_boss_by_flame = (test_draw_boss_hp_value) | (0x10);
		#endif
		//
		if (limit_max_hp_dec_boss_by_flame < card.boss_hp_dec_by_frame)
		{
			card.boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_flame;
		}
	}
//
	obj_boss->base_hp -= card.boss_hp_dec_by_frame;
	card.boss_hp_dec_by_frame = 0;/* 使ったので消す(フレーム単位) */
	if (card.limit_health >= obj_boss->base_hp) 	/* 規定値以下になればカードモード解除 */
	{
		card.mode			= (CARD_BOSS_MODE_00_OFF);
		boss_destroy_check_type(obj_boss, DESTROY_CHECK_00_WIN_BOSS);
		cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
	}
}


/*---------------------------------------------------------
	カードを次に進める。
---------------------------------------------------------*/
static void card_incliment(void)
{
			card.number += (4)/*1*/;
}

/* 出現時x座標 */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* 中心座標なので */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* 中心座標なので */

/*---------------------------------------------------------
	[カードシステム内に移動予定]	カード登録
---------------------------------------------------------*/

/*global*/global/*static*/ void card_boss_move_generate_check_regist(SPRITE *src)
{
	if ( /*off*/(0)==card.mode)/*(判断)*/
	{
		card.mode		= (CARD_BOSS_MODE_01_IDO_JYUNNBI);/*on*/	/* とりあえず */
		/* [(とりあえず)カード攻撃のみに仕様変更]したので、最後撃てるカードがなくなった場合に攻撃させる為。 */
		{
			int aaa;	/* 現在体力 から 撃ちたいカード分 引いた体力値 */
			aaa = card.limit_health - (my_card_resource[(card.number+(4)/*1*/)].spell_life);	/*1000 500*/
		//	card.limit_health -= 1000/*500*/;
			/* ボスがカードを撃てる一定体力がある場合 */
			if (0 < aaa)	/* カード撃てる。 */
			{
				card.limit_health = aaa;
			//	if (card_syoji_maisuu < card.number)
				if (0 == card_syoji_maisuu )
				{
					/* 形態変更しない、アイテム吐かない */
				//	card.number -= (4);
				//	src->callback_loser 			= lo se_boss;
				}
				else/* 形態変更したら */
				{
					card_incliment();
					card_syoji_maisuu--;
					#if 0//(1==TEST_ZAKO_HIDE)/* ボスも影響受ける */
					zako_all_timeup();/* ザコタイムアウト(フェイドアウト消去処理へ移行) */
					#endif
					/* アイテム吐く */
					if (NULL != src->callback_loser)
					{
						(src->callback_loser)(src); 	/* sakuya_put_items(src); */
					}
				}
			}
			else	/* カード撃てない。 */
			{
				card.limit_health = (0);
			//	card.mode		= (0);/*off*/
			}
		}
		/*---------------------------------------------------------
			カードチェック
		---------------------------------------------------------*/
		if ( /*off*/(0)==card.boss_timer)
		{
			card.limit_health = (0);
		}
	}
	else
	if (CARD_BOSS_MODE_01_IDO_JYUNNBI == card.mode)
	{
		card.mode			= (CARD_BOSS_MODE_02_TAIHI);/*on*/


		REG_10_BOSS_SPELL_TIMER = (0);	/* カード生成を強制的に止める。 */
		bullets_to_hosi();		/* 総ての敵弾を、hosiアイテムに変える */
		/* 真中付近に退避 */
	//	src->BOSS_DATA_04_toutatu_wariai256 			= t256(  0);/* 初期化済みの必要あり */
		src->BOSS_DATA_04_toutatu_wariai256 			= t256(1.0);/* 初期化済みの必要あり */
	//	src->BOSS_DATA_00_target_x256			= t256(153);
		src->BOSS_DATA_00_target_x256			= BOSS_XP256; //t256(0);
	//	src->BOSS_DATA_01_target_y256			= src->cy256;
		src->BOSS_DATA_01_target_y256			= t256(16.0); //t256(0);

		#if 1/* (新[カード始まる前に初期化]) 第0形態から、必ず呼ぶ筈。 */
		/* カード初期化 */
		if (NULL != my_card_resource[(card.number)].spell_init_callback)
		{
			(my_card_resource[(card.number)].spell_init_callback)(src);
		}
		#endif
	}
	else
	if (CARD_BOSS_MODE_02_TAIHI==card.mode) /* 発弾位置まで移動中。 */
	{
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1); 	/* [約	4 秒]== 4.2666==(256/60[flame]) */
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1*4);		/* [約	1 秒]== 1.0666==(256/(4*60)[flame]) */
//		src->BOSS_DATA_04_toutatu_wariai256 -= (1*(4-1));	/* [約	1 秒]== 1.0666==(256/(4*60)[flame]) */
		#if 1/*(r32)*/
	//	boss_yuudou_idou_nomi(src);/*(r32)*/
		boss_yuudou_hiritu_keisan(src);
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1); /* [約	4 秒]== 4.2666==(256/60[flame]) */
		src->BOSS_DATA_04_toutatu_wariai256 -= (1*4);	/* [約	1 秒]== 1.0666==(256/(4*60)[flame]) */
		if (0 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ほぼ画面中心付近まで、移動した。 */
		{
			src->BOSS_DATA_04_toutatu_wariai256 = (0);
			card.mode		= (CARD_BOSS_MODE_03_HATUDAN);/*on*/
		}
		#endif/*(r32)*/
		#if 0/*(r32p)*/
		boss_yuudou_idou_nomi(src);/*(r32p)*/
		boss_yuudou_hiritu_keisan(src);
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1); /* [約	4 秒]== 4.2666==(256/60[flame]) */
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (1*4);	/* [約	1 秒]== 1.0666==(256/(4*60)[flame]) */
	//	if (0 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ほぼ画面中心付近まで、移動した。 */
		if ((0==src->BOSS_DATA_04_toutatu_wariai256))
		{
	//		src->BOSS_DATA_04_toutatu_wariai256 = (0);
			card.mode		= (CARD_BOSS_MODE_03_HATUDAN);/*on*/
		}
		#endif/*(r32p)*/
	}
	else/* [(とりあえず)カード攻撃のみに仕様変更]*/
	/*
		(準備時の移動などもカード扱い)
		(カード無くなった後も扱い)
	*/
	/* ボス行動 */
	if (CARD_BOSS_MODE_03_HATUDAN==card.mode)/*(カード中)*/
	{
		(my_card_resource[(card.number)].boss_move_keitai_callback)(src);/*(ボス移動形態毎に、ボス移動処理を実行する)*/
	}
}


/*---------------------------------------------------------
	[カードシステム内に移動予定]
---------------------------------------------------------*/


/*---------------------------------------------------------
	ボスを攻撃した場合の共通ルーチン
	-------------------------------------------------------
	SPRITE *src;	ボス敵自体
	SPRITE *tama;	自弾
---------------------------------------------------------*/

/*static*/static/*global*/	void s_callback_hit_boss(SPRITE *src, SPRITE *tama)
{
	/* ボス & 中-ボスに自弾があたった場合の火花エフェクトを登録(現在Gu部分がないので描画してない) */
	set_REG_DEST_XY(tama);/* 発弾位置 座標xy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* 先に実行した方が速い */
//
	/* 上と分離した方がコード効率があがる。 */
	{
		card.boss_hp_dec_by_frame += /*w->*/tama->base_weapon_strength; /* 攻撃して体力減らす(強さ分引く) */
	}
}

//static int kaiwa_syuuryou;	/* ボス戦闘前の会話終了を待つ */
global /*static*/ void kaiwa_00_keitai(SPRITE *src)
{
//	if (0!=kaiwa_syuuryou)	/* ボス戦闘前の会話終了を待つ */
	if ( ((cg.state_flag) & STATE_FLAG_05_IS_BOSS) )	/* ボス戦闘前の会話終了? */
	{
		src->BOSS_DATA_04_toutatu_wariai256 = t256(1.0);
	//	common_boss_init_2nd(src);	/* プレイヤー弾受け付け、コールバックを登録 */
		/*---------------------------------------------------------
			ボスの共通、２回目初期化ルーチン(攻撃可能)
		---------------------------------------------------------*/
	//	/*extern*/static void common_boss_init_2nd(SPRITE *src)
		{
			/* プレイヤー弾受け付け、コールバックを登録 */
			src->callback_hit_teki = s_callback_hit_boss;	/* コールバック登録 */
			/* card common init */
			card.mode		= (CARD_BOSS_MODE_01_IDO_JYUNNBI);/*on*/
		//	card.mode		= (0);/*off*/
		}
	//	/*時間制限カウント有効化*/
	//	data->boss_base.state001++/* = ST_02*/;
		card_incliment();
	}
}
//global void called_from_kaiwa_system_boss_start(void)
//{
//	cg.state_flag |= STATE_FLAG_05_IS_BOSS; 	/* ボス戦闘前の会話終了を設定 */
//	kaiwa_syuuryou = (1);
//}
/*---------------------------------------------------------
	共通ボス退避(撃破後に画面外にボスが逃げる)
	[カードシステム内に移動予定]
---------------------------------------------------------*/

global /*static*/ void common_99_keitai(SPRITE *src)
{
	if (0 > (src->cy256+t256(50.0)))/* +t256(50.0) ボスグラの最大サイズ(50[dot]) */
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		src->callback_mover 	= NULL; 		/* おしまい */
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
		if ( (int)(GAME_X_OFFSET*256+(GAME_320_WIDTH*256/2)) > src->cx256)	/* 画面半分の位置 */
		{
			src->cx256 += t256(1.0);	/* 右上に退避 */
		}
		else
		{
			src->cx256 -= t256(1.0);	/* 左上に退避 */
		}
	}
}



/*---------------------------------------------------------
	カードが終わるまで待つ。(廃止)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 0
	/*static*/global void card_state_check_holding(SPRITE *src)
	{
		if (SPELL_00 == card.card_number)
		{
			src->BOSS_DATA_05_boss_base_state777++;
		}
	}
#endif


/*---------------------------------------------------------
	ボスの共通、１回目初期化ルーチン(初回、攻撃不可)
---------------------------------------------------------*/


/*---------------------------------------------------------
	弾の範囲を「標準」に設定
---------------------------------------------------------*/
global void set_default_bullet_clip(void)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET);
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH);
	rect_clip.bullet_clip_min.y256 = t256(-32);
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT);
}


/*---------------------------------------------------------
	弾の範囲を「上と横の広範囲」に設定
---------------------------------------------------------*/
global void set_aya_bullet_clip(void)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET)						+ t256(-100);
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH) + t256( 100);
	rect_clip.bullet_clip_min.y256 = t256(-256);/* あや */
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT);//+t256(100)
}

global void init_00_boss_clip000(SPRITE *h)/* 標準タイプ */
{
	set_default_bullet_clip();	/* 弾の範囲を標準に設定 */
}
global void init_00_boss_clip111(SPRITE *h)/* 上に広いタイプ */
{
	set_aya_bullet_clip();	/* 弾の範囲を標準に設定 */
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
//	#if (0==U SE_BOSS_COMMON_MALLOC)
//	h									= obj_add_01_teki_error();
//	obj_boss							= h;/*輝夜本人*/
//	#else
//	SPRITE *obj_boss;
//	obj_boss = あたり判定の都合上無理&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	SPRITE *h;
//	h = あたり判定の都合上無理&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	#endif


/*static*/extern  void boss_effect_reset(void);
extern void stage_boss_load_texture(void);



// src/core/douchu/boss.h の初期化も参照する事。
global void called_from_kaiwa_system_boss_load(int boss_number)
{
	{
		/* boss_rect_init */
		rect_clip.boss_clip_min.x256	= t256(GAME_X_OFFSET)+t256( 			0)+t256(24);
		rect_clip.boss_clip_max.x256	= t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH)-t256(24);
		rect_clip.boss_clip_min.y256	= t256(0);
		rect_clip.boss_clip_max.y256	= t256(96);
	}
//
	int sss;
	sss = boss_number;
	sss &= (8-1);
//
	//----[BOSS]
	SPRITE *h;
	h					= global_obj_boss;/*輝夜本人*/
//	h					= あたり判定の都合上無理&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_BOSS];
//	if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */		/* 仕様バグ(?) */
	{
	/*(再定義の必要あり)*/	h->type 						= BOSS_00_11;
		h->callback_mover				= common_boss_move;
		h->callback_hit_teki			= NULL; 	/* ダミーコールバック登録 */
	//
		#if 0/* 初期化済みの必要あり */
		h->vx256						= t256( 0);
		h->vy256						= t256( 0);
		#endif
	//
		h->BOSS_DATA_03_kougeki_anime_count 		= (0);	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
		h->BOSS_DATA_05_boss_base_state777			= (0);	/*ST_00*/	/*初期値を0にする。*/
	//	h->BOSS_DATA_05_boss_base_state777			= (-1); /*ST_00*/
	//
	//------------ カード関連
		{
			static const u16/*u8*/ aaa[8+8] =/* カードアドレスが256種類を超えたので、u16 */
			{
			/* ボス開始カード番号(easy) */
				CARD_ADDRESS_00E_chrno_000, 	/* チルノ(?) */ 	/* エキストラ用(boss0) */
				CARD_ADDRESS_00E_alice_000, 	/* アリス */
				CARD_ADDRESS_00E_mima_000,		/* 魅魔 */
				CARD_ADDRESS_00E_kaguya_000,	/* 輝夜 */
			//
				CARD_ADDRESS_00E_aya_000,		/* 文 */
				CARD_ADDRESS_00E_pacheA_000,	/* パチェ */
				CARD_ADDRESS_00E_sakuya_000,	/* 咲夜 */
				CARD_ADDRESS_00E_pacheA_000,	/* フラン(?) */ 	/* ファンタズム用(boss7) */
			/* ボスカード所持数 */
				(6),//(0-0),													/* チルノ(?) */ 	/* エキストラ用(boss0) */
				(5),//(CARD_ADDRESS_16E_alice_ggg-CARD_ADDRESS_00E_alice_000),		/* アリス */		/*CARD_ADDRESS_19_alice_jjj;*/
				(5),//(CARD_ADDRESS_17E_mima_hhh-CARD_ADDRESS_00E_mima_000),		/* 魅魔 */
				(6),//(CARD_ADDRESS_17E_kaguya_hhh-CARD_ADDRESS_00E_kaguya_000),	/* 輝夜 */
			//
				(6),//(CARD_ADDRESS_19E_aya_jjj-CARD_ADDRESS_00E_aya_000),			/* 文 */
				(8),//(CARD_ADDRESS_17E_pache_hhh-CARD_ADDRESS_00E_pache_000),		/* パチェ */
				(9),//(CARD_ADDRESS_11E_sakuya_kkk-CARD_ADDRESS_00E_sakuya_000),	/* 咲夜 */
				(5),//(0-0),													/* フラン(?) */ 	/* ファンタズム用(boss7) */
			};
			card.number 			= aaa[sss  ] + ((cg.game_difficulty)&0x03)
				+ ((5!=sss)?(0):(((cg_game_select_player)&(4-1))<<(2+3)));/* 難易度(2bit==4段階)、カード(3bit==8段階) */
			card_syoji_maisuu		= aaa[sss+8];
		}
		{
	/*(再定義の必要あり)*/	h->base_hp				= (my_card_resource[(card.number)].spell_life); 		/* 全体の体力 */
			card.limit_health = (h->base_hp) - (my_card_resource[(card.number + (4)/*1*/)].spell_life); 	/* 通常攻撃(初回攻撃)の攻撃分 */
			//
			card.boss_timer = (((my_card_resource[(card.number)].spell_limit_time)));	/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
			card.mode		= (CARD_BOSS_MODE_01_IDO_JYUNNBI);/*on*/	/* 特殊？ */
			/*???*/
		//	kaiwa_syuuryou = (0);
		}
		#if 1
	//------------ カード関連
		card.card_number			= SPELL_00; 	/* カード生成終了フラグ */
		card.number_temporaly	= (0);			/*(SPELL_08_rumia-1)*/ /*0*/
		#endif
	//
	}
	boss_effect_reset();
	// ボステクスチャ読み込み
	stage_boss_load_texture();
}
