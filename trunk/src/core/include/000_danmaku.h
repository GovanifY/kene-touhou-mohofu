
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _DANMAKU_H_
#define _DANMAKU_H_
	/*---------------------------------------------------------
	弾幕システム(あらかじめ弾幕の定義をしておくシステム)
	-------------------------------------------------------
	模倣風では弾幕はシステムでサポートしています。
	弾幕として定義しておくと、どのボスが、どの弾幕でも撃てるようになります。
	-------------------------------------------------------
	また弾幕はスペルカードシステムに番号を登録しておくと、スペルカードシステムで弾幕が取得できます。
	(つまりスペルカードシステム経由で弾幕が撃てます)
	-------------------------------------------------------
	(r32)現在、雑魚(模倣風のdatの中ボス)も弾幕を撃てますが、
	この機能は将来なくなるかも知れません。
	雑魚(模倣風のdatの中ボス)が弾幕を撃てる機能は「風神禄」で思いついたのですが、
	こういう機能が無い方がゲームの速度が速く出来る気もするし、
	もしかしたらこういう機能は本家に無いのかもと思うこともあります。良く判りません。
	-------------------------------------------------------
	boss->boss_base_danmaku_type に撃ちたい弾幕をセットすると、弾幕生成開始。
	弾幕生成が終了すると自動的に boss->boss_base_danmaku_type が DANMAKU_00 になる。
	-------------------------------------------------------
	弾幕生成中は boss->boss_base_danmaku_time_out が勝手に毎フレーム減算され、0 になると弾幕生成終了。
	この時間は弾幕ごとに違う標準時間が設定されているので、これを設定するには
	danmaku_set_time_out(boss); 	// 弾幕の時間切れを設定
	とする。
	---------------------------------------------------------*/

/* 弾幕番号(各弾幕を区別する為の番号) */
enum
{
	DANMAKU_00 = 0, 					/* 弾幕生成終了フラグ。 */
//	/* 雑魚用 */
	DANMAKU_01_sakuya,					/* [咲夜] 紅5面中ボス 禊弾幕 / 紅5面中ボス 咲夜 「通常攻撃」 */
	DANMAKU_02_sakuya_old_test, 		/* [咲夜] 紅5面中ボス 24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	DANMAKU_03_sakuya_old_test, 		/* [咲夜] 紅5面中ボス 11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
	DANMAKU_04_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_05_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_06_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_07_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
//
	DANMAKU_08_rumia,					/* [ルーミア] 紅1面ボス [ルーミア] 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	DANMAKU_09_alice,					/* [アリス] っぽい？テキトー弾幕 */
	DANMAKU_0a_dai_yousei_01,			/* [大妖精] 全周 48分割、時計回り、緑クナイ弾 */
	DANMAKU_0b_dai_yousei_02,			/* [大妖精] 全周 48分割、反時計回り、赤クナイ弾 */
	DANMAKU_0c_sakuya_jack32,			/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
	DANMAKU_0d_sakuya_miss_direction,	/* [咲夜] 奇術「ミスディレクション」 */
	DANMAKU_0e_remilia_00,				/* [レミリア] テスト */
	DANMAKU_0f_tengu,					/* [その他] 差分氏の妖怪1(天狗様?)が撃つ弾幕(予定) */		/*???*/
//
	DANMAKU_10_cirno,					/* [チルノ] 禊弾幕 */
	DANMAKU_11_perfect_freeze,			/* [チルノ] パーフェクトフリーズ */
	DANMAKU_12_diamond_blizzard,		/* [チルノ] 雪符「ダイアモンドブリザード」 */
	DANMAKU_13_alice_suwako,			/* [アリス] 諏訪子っぽい(?)弾幕 */
	DANMAKU_14_alice_youmu300,			/* [アリス] 妖3面ボス  normal通常攻撃3(の一部) */
	DANMAKU_15_alice_aka_2nd,			/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
	DANMAKU_16_alice_doll,				/* [アリス] 妖3面ボス [アリス]人形弾幕(もどき) */	// 40[sec]
	DANMAKU_17_alice_doll_02,			/* [アリス] 作成中 */
//
	DANMAKU_18_hana_test,				/* [魅魔] 花てすと弾幕 */
	DANMAKU_19_mima_sekkin, 			/* [魅魔] 接近弾幕 */
	DANMAKU_1a_mima_toge,				/* [魅魔] トゲてすと弾幕 */
	DANMAKU_1b,
	DANMAKU_1c_kakuya_tamanoe,			/* [輝夜]、蓬莱の玉の枝もどき。作成中 */
	DANMAKU_1d,
	DANMAKU_1e_kaguya01,				/* [輝夜] むりやり変換1 */
	DANMAKU_1f_kaguya04,				/* [輝夜] むりやり変換2 */
//	/* ボス用 */
	DANMAKU_20_aya_misogi1, 			/* [文] の通常弾っぽい弾幕撃たせてみるテスト1 */
	DANMAKU_21_aya_misogi2, 			/* [文] の通常弾っぽい弾幕撃たせてみるテスト2 */
	DANMAKU_22_ame_no_yatimata, 		/* [文] 岐符「天の八衢」 */
	DANMAKU_23_aya_merin_test,			/* [文] に美鈴っぽい弾幕撃たせてみるテスト */
	DANMAKU_24_aya_doll,				/* [文] 人形弾幕 */ //40[sec]
	DANMAKU_25_houka_kenran,			/* [紅美鈴] 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	DANMAKU_26_aya_saifu,				/* [文] 塞符 */
	DANMAKU_27_PACHE_LASER2,			/* [パチェ] のレーザーもどき */
//
	DANMAKU_28_rumia_night_bird,		/* [ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	DANMAKU_29_rumia_demarcation,		/* [ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似た弾幕(予定) */
	DANMAKU_2a_amefuri_test,			/* [] 雨てすと */
	DANMAKU_2b,
	DANMAKU_2c,
	DANMAKU_2d,
	DANMAKU_2e_SONOTA_DEBUG_CW_AO,		/* [その他] デバッグ弾幕CW青 */
	DANMAKU_2f_SONOTA_DEBUG_CCW_AKA,	/* [その他] デバッグ弾幕CCW赤 */
//
	DANMAKU_MAX,
};

/* target->tmp_angleCCW1024 に 自機狙い弾の角度を計算 */
extern void tmp_angleCCW1024_src_nerai(SPRITE *src, SPRITE *dest);
extern void tmp_angleCCW1024_jiki_nerai(SPRITE *target);
extern void tmp_angleCCW65536_src_nerai(SPRITE *src, SPRITE *dest);
extern void tmp_angleCCW65536_jiki_nerai(SPRITE *target);

/* 弾幕の時間切れを設定 */
extern void danmaku_set_time_out(SPRITE *src);

/* 弾幕生成 */
extern void danmaku_generator(SPRITE *src);



#endif /* _DANMAKU_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_
enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*敵自体*/, int check_type);/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */
/*static*/extern  void callback_hit_boss_dummy(SPRITE *src, SPRITE *tama /*, int angle*/);
/*static*/extern  void callback_hit_boss(SPRITE *src, SPRITE *tama /*, int angle*/);

/*static*/extern  void common_boss_put_items(SPRITE *src);

/*static*/extern  void lose_boss(SPRITE *src);






/* スペカシステム */
/*static*/extern void spell_card_get_danmaku_number(SPRITE *src);
#endif /* _BOSS_H_ */
