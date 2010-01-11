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

// 輝夜 4面
//tern void kaguya_06_keitai(SPRITE *src);
//tern void kaguya_05_keitai(SPRITE *src);
extern void kaguya_04_keitai(SPRITE *src);
//tern void kaguya_03_keitai(SPRITE *src);
//tern void kaguya_02_keitai(SPRITE *src);
extern void kaguya_01_keitai(SPRITE *src);

// 未定 3面
extern void mitei_01_keitai(SPRITE *src);

// 文 2面
extern void aya_05_keitai(SPRITE *src);
extern void aya_04_keitai(SPRITE *src);
//tern void aya_03_keitai(SPRITE *src);
extern void aya_02_keitai(SPRITE *src);
extern void aya_01_keitai(SPRITE *src);

// アリス 1面
extern void alice_01_keitai(SPRITE *src);

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
	void (*spell_move_callback)(SPRITE *sss);	/* 移動処理 */
	const char *spell_str_name; 	/* スペカ名称 */
} SPELL_CARD_RESOURCE;

//	{	 500,	  20,		"氷符「開発中000」" },/*NULL*/		/* スペカ生成しない */
	/* 名前はテキトーです */

/* スペル時間(==単位[フレーム])は、64倍(==単位[約1秒])が規格 */
#define s_time(aaa) ((aaa)<<6)

/*static*/ SPELL_CARD_RESOURCE my_111resource[SPELL_CARD_MAX] =
{
// 咲夜 6面
	{	 500,	  s_time(160),	common_00_keitai,	"形態変更"							"\n"	},/* 500 160 初回の攻撃分(手動設定) */
	{	 500,	  s_time(20),	sakuya_01_keitai,	"通常攻撃"							"\n"	},		/*	"第一形態: 左右に動いて小弾撃ち(通常攻撃)"		*/
	{	 500,	  s_time(20),	sakuya_02_keitai,	"奇術「ミスディレクション」"		"\n"	},		/*	"第二形態: 全方位、豆まき(ミスディレクション)"	*/
	{	 500,	  s_time(20),	sakuya_03_keitai,	"時雨「バーティカルナイフ」"		"\n"	},		/*	"第三形態: 垂直ナイフが落ちてくるよ"			*/
	{	 500,	  s_time(20),	sakuya_04_keitai,	"幻魂「アンリミテッドメイド」"		"\n"	},		/*	"第四形態: 魔方陣生成"							*/
	{	2500,	  s_time(20),	sakuya_06_keitai,	"幻種「デンジャラスワールド」"		"\n"	},		/*	"第五形態: (黄色マスカット弾)"					*/
//	{	 500,	  s_time(20),	common_00_keitai,	"奇術「咲夜テスト007」" 			"\n"	},		/*	"第七形態: (分散魔方陣)追加計画中"				*/
//	{	 500,	  s_time(20),	common_00_keitai,	"奇術「咲夜テスト008」" 			"\n"	},		/*	"第八形態: (時止めナイフ)追加計画中"			*/
	{	2500,	  s_time(20),	sakuya_09_keitai,	"幻葬「フェスティバルナイフ」"		"\n"	},		/*	"第九形態: 最終形態(その1)" 					*/
	{	1500,	  s_time(20),	sakuya_10_keitai,	"メイド秘技「ファイナルタイプ」"	"\n"	},		/*	"第10形態: 最終形態(その2)" 					*/
	{	9000,	  s_time(20),	sakuya_11_keitai,	"メイド秘技「ファイナルβ」"		"\n"	},		/*	"第11形態: 最終形態(その3)" 					*/
// パチェ 5面
	{	 500,	  s_time(120),	common_00_keitai,	"形態変更"							"\n"	},/* 3069 120 初回の攻撃分(手動設定) */
	{	 500,	  s_time(20),	pache_01_keitai,	"通常攻撃"							"\n"	},
	{	 500,	  s_time(40),	pache_01_keitai,	"火符「パチュリー未作成000」"		"\n"	},
	{	 500,	  s_time(20),	pache_01_keitai,	"水符「パチュリー未作成001」"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"木符「シルフィー未作成002」"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"金符「パチュリー未作成003」"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"土符「パチュリー未作成004」"		"\n"	},
	{	 500,	  s_time(40),	pache_04_keitai,	"日符「パチュリー未作成005」"		"\n"	},
//	{	 500,	  s_time(40),	pache_04_keitai,	"月符「サイレント未作成006」"		"\n"	},
//	{	 500,	  s_time(40),	pache_04_keitai,	"氷符「パチュリー未作成007」"		"\n"	},
// 輝夜 4面
	{	 500,	  s_time(99),	common_00_keitai,	"形態変更"							"\n"	},/* 500 99 初回の攻撃分(手動設定) */
	{	 500,	  s_time(20),	kaguya_01_keitai,	"通常攻撃"							"\n"	},
	{	 500,	  s_time(40),	kaguya_01_keitai,	"難題「未作成001」" 				"\n"	},
	{	 500,	  s_time(40),	kaguya_01_keitai,	"難題「未作成002」" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"難題「未作成003」" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"難題「未作成004」" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"難題「未作成005」" 				"\n"	},
	{	 700,	  s_time(40),	kaguya_04_keitai,	"難題「未作成006」" 				"\n"	},
// 未定 3面
	{	 500,	  s_time(40),	common_00_keitai,	"形態変更"							"\n"	},/* 500 40 初回の攻撃分(手動設定) */
	{	 500,	  s_time(20),	mitei_01_keitai,	"通常攻撃"							"\n"	},
	{	 500,	  s_time(20),	mitei_01_keitai,	"未定「未作成スペカ001」"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"未定「未作成スペカ002」"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"未定「未作成スペカ003」"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"未定「未作成スペカ004」"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"未定「未作成スペカ005」"			"\n"	},
	{	 500,	  s_time(40),	mitei_01_keitai,	"未定「未作成スペカ006」"			"\n"	},
// 文 2面
	{	 500,	  s_time(120),	common_00_keitai,	"形態変更"							"\n"	},/* 500 120 初回の攻撃分(手動設定) */
	{	 500,	  s_time(20),	aya_01_keitai,		"通常攻撃"							"\n"	},/*4*/
	{	1500,	  s_time(40),	aya_02_keitai,		"華符「芳華絢爛」のテスト"			"\n"	},/*4*/
	{	 500,	  s_time(20),	aya_04_keitai,		"岐符「未作成スペカ001」"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_04_keitai,		"岐符「未作成スペカ002」"			"\n"	},/*4*/
	{	1000,	  s_time(40),	aya_05_keitai,		"岐符「未作成スペカ003」"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_04_keitai,		"岐符「未作成スペカ004」"			"\n"	},/*4*/
	{	1000,	  s_time(40),	aya_05_keitai,		"岐符「未作成スペカ005」"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_05_keitai,		"岐符「未作成スペカ006」"			"\n"	},/*5*/
	{	1000,	  s_time(40),	aya_05_keitai,		"岐符「未作成スペカ007」"			"\n"	},/*5*/
// アリス 1面
	{	1023,	  s_time(75),	common_00_keitai,	"形態変更"							"\n"	},/* 1023 75 初回の攻撃分(手動設定) */
	{	 500,	  s_time(20),	alice_01_keitai,	"通常攻撃"							"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の仏蘭西人形000」" 	"\n"	},
	{	 500,	  s_time(20),	alice_01_keitai,	"操符「未作成の阿蘭陀人形001」" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の伊太利亜人形002」"	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の英吉利人形003」" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の露西亜人形004」" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の瑞西人形005」"		"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の白耳義人形006」" 	"\n"	},
	{	 500,	  s_time(40),	alice_01_keitai,	"操符「未作成の波蘭人形007」"		"\n"	},
//
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
				spell_card_mode 		= 1/*on*/;
			}
		}
	}
}

/*---------------------------------------------------------
	スペカ名称表示
---------------------------------------------------------*/

void draw_spell_card_name(void)
{
	script_message_window_clear();/*スクリプトメッセージ画面を消す*/
	msg_time = (60*5);
	print_kanji000(/*SDL_Surface *drawmap,*/ /*SDL_Rect *rect*/ /*0,*/
		my_111resource[spell_card_number].spell_str_name, //	aaa_str[(spell_card_number&15)],
		/*int color_type*/7, /*int wait*/0
	);
}


/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ撃破後アイテム出す
---------------------------------------------------------*/
//	#define alice_put_items common_boss_put_items
//	#define mitei_put_items common_boss_put_items
//	#define kaguya_put_items common_boss_put_items
//	#define pache_put_items common_boss_put_items

/*---------------------------------------------------------

---------------------------------------------------------*/
#if 0
/*extern*/ void create_spell_card(SPRITE *src, int spell_card_type)
{
//	BOSS99_DATA *data = (BOSS99_DATA *)src->data;
//
	int aaa;	/* 現在体力 から 撃ちたいスペカ分 引いた体力値 */
	aaa = ((/*data->boss_base.boss*/src->base_health) - (my_111resource[spell_card_type].spell_life));
	/* ボスがスペカを撃てる一定体力がある場合 */
	if ( 0 < aaa )
	{
		spell_card_limit_health = aaa;
		spell_card_mode 		= 1/*on*/;
	}
}
#endif

/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ登録
---------------------------------------------------------*/

/*static*/ void regist_spell_card222(SPRITE *src)
{
//	spell_card_limit_health -= 1000/*500*/;
	spell_card_limit_health -= (my_111resource[spell_card_number+1].spell_life);	/*1000 500*/
	if (0 >= spell_card_limit_health)
	{
		spell_card_limit_health = 0;
		spell_card_mode 		= 0/*off*/;
	}
	else
	{
	//	BOSS06_DATA *data = (BOSS06_DATA *)src->data;
		spell_card_mode 		= 1/*on*/;
		spell_card_number++;
		if (spell_card_max < spell_card_number)
		{
			/* 形態変更しない、アイテム吐かない */
			spell_card_number--;
		//	src->callback_loser 			= lose_boss;
		}
		else
		{
			/* アイテム吐く */
			if (NULL != src->callback_loser)
			{
				(src->callback_loser)(src); 	/* sakuya_put_items(src); */
			}
		}
	}
}
#if 0

/*---------------------------------------------------------
	[スペカシステム内に移動予定]	スペカ登録
---------------------------------------------------------*/

static void regist_spell_card(SPRITE *src)
{
	spell_card_limit_health -= 1000/*500*/;
	if (0 >= spell_card_limit_health)
	{
		spell_card_limit_health = 0;
		spell_card_mode 		= 0/*off*/;
	}
	else
	{
	//	BOSS02_DATA *data = (BOSS02_DATA *)src->data;
	//	if (b_health_alter_low1024 < (data->boss_base.boss_health & (1024-1)))/* 形態変更したら */
		spell_card_mode 		= 1/*on*/;
		spell_card_number++;
		if (spell_card_max < spell_card_number)
		{
			spell_card_number--;
		}
//		if (0==sakuya->base_health)
		{
//			if (0 != (data->boss_base.boss_life))/* 形態変更したら */
			{
//				data->boss_base.boss_life--;
				(src->callback_loser)(src);
			}
		}
	}
}

#endif

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

/*extern*/ void spell_card_boss_init_regist(SPRITE *src)
{
	((PLAYER_DATA *)player->data)->boss = src;/*輝夜本人*/
//
	src->x256						= BOSS_XP256/*t256(GAME_WIDTH/2)-(sakuya->w128)*/;
	src->y256						= t256(-100);
//
	src->callback_hit_enemy 		= NULL; 	/* ダミーコールバック登録 */
//
	/* spell_card common init */
//	{
//	//	spell_card_limit_health 	= 0;
//	//	spell_card_mode 			= 0/*off*/;
//	}
	spell_card_boss_timer	= (((my_111resource[spell_card_number].spell_time)));		/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
	spell_card_limit_health = (src->base_health) - (my_111resource[spell_card_number].spell_life);/* 500 1000 通常攻撃(初回攻撃)の攻撃分(手動設定) */
	spell_card_mode 		= 1/*on*/;	/* 特殊？ */
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

/*static*/ void common_00_keitai(SPRITE *src)
{
	if (t256(15/*40*/ /*30*/) > src->y256)	/* 下へ移動 */
	{
		src->y256 += t256(1/*2*/);	/**fps_fa ctor*/
	}
	else
	if (t256(16/*40*/ /*30*/) > src->y256)	/* 下へ移動 */
	{
		src->y256 += t256(1/*2*/);	/**fps_fa ctor*/
		((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
	}
	else	/* 会話終了を待つ */
	{
		if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )	/* 会話終了? */
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
				spell_card_mode 		= 1/*on*/;
			}
		//	/*時間制限カウント有効化*/
		//	data->state1++/* = ST_02*/;
			spell_card_number++;
		}
	}
}

