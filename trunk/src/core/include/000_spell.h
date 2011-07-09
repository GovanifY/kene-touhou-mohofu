
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _SPELL_H_
#define _SPELL_H_
	/*---------------------------------------------------------
	スペル(弾幕)システム(あらかじめ弾幕の定義をしておくシステム)
	-------------------------------------------------------
	模倣風ではスペル(弾幕)はシステムでサポートしています。
	スペル(弾幕)として定義しておくと、どのボスが、どのスペル(弾幕)でも撃てるようになります。
	-------------------------------------------------------
	またスペル(弾幕)はスペルカードシステムに番号を登録しておくと、スペルカードシステムでスペル(弾幕)が取得できます。
	(つまりスペルカードシステム経由でスペル(弾幕)が撃てます)
	-------------------------------------------------------
	(r32)現在、雑魚(模倣風のdatの中-ボス)もスペル(弾幕)を撃てますが、
	この機能は将来なくなるかも知れません。
	雑魚(模倣風のdatの中-ボス)がスペル(弾幕)を撃てる機能は「風神禄」で思いついたのですが、
	こういう機能が無い方がゲームの速度が速く出来る気もするし、
	もしかしたらこういう機能は本家に無いのかもと思うこともあります。良く判りません。
	-------------------------------------------------------
	boss->boss_base_spell_type に撃ちたいスペル(弾幕)をセットすると、スペル(弾幕)生成開始。
	スペル(弾幕)生成が終了すると自動的に boss->boss_base_spell_type が DANMAKU_00 になる。
	-------------------------------------------------------
	スペル(弾幕)生成中は boss->boss_base_spell_time_out が勝手に毎フレーム減算され、0 になるとスペル(弾幕)生成終了。
	この時間はスペル(弾幕)ごとに違う標準時間が設定されているので、これを設定するには
	spell_set_time_out(boss);	// スペル(弾幕)の時間切れを設定
	とする。
	---------------------------------------------------------*/

/* スペル(弾幕)番号(各スペル(弾幕)を区別する為の番号) */
enum
{
	SPELL_00 = 0,						/* スペル(弾幕)生成終了フラグ。 */
//	/* 雑魚用 */
	SPELL_01_sakuya_misogi_normal,		/* [咲夜] 紅5面中-ボス 禊弾幕 / 紅5面中-ボス 咲夜 「通常攻撃」 */
	SPELL_02_sakuya_old_test,			/* [咲夜] 紅5面中-ボス 24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	SPELL_03_sakuya_old_test,			/* [咲夜] 紅5面中-ボス 11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
	SPELL_04_sakuya_pink_hearts,		/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
	SPELL_05_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	SPELL_06_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	SPELL_07_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
//
	SPELL_08_rumia, 					/* [ルーミア] 紅1面ボス [ルーミア] 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	SPELL_09_alice, 					/* [アリス] っぽい？テキトー弾幕 */
	SPELL_0a_dai_yousei_01, 			/* [大妖精] 全周 48分割、時計回り、緑クナイ弾 */
	SPELL_0b_dai_yousei_02, 			/* [大妖精] 全周 48分割、反時計回り、赤クナイ弾 */
	SPELL_0c_sakuya_jack32, 			/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
	SPELL_0d_sakuya_miss_direction, 	/* [咲夜] 奇術「ミスディレクション」 */
	SPELL_0e_remilia_00,				/* [レミリア] テスト */
	SPELL_0f_tengu, 					/* [その他] 差分氏の妖怪1(天狗様?)が撃つ弾幕(予定) */		/*???*/
//
	SPELL_10_cirno, 					/* [チルノ] 禊弾幕 */
	SPELL_11_perfect_freeze,			/* [チルノ] パーフェクトフリーズ */
	SPELL_12_diamond_blizzard,			/* [チルノ] 雪符「ダイアモンドブリザード」 */
	SPELL_13_alice_suwako,				/* [アリス] 諏訪子っぽい(?)弾幕 */
	SPELL_14_alice_youmu300,			/* [アリス] 妖3面ボス  normal通常攻撃3(の一部) */
	SPELL_15_alice_aka_2nd, 			/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
	SPELL_16_alice_doll,				/* [アリス] 妖3面ボス [アリス]人形弾幕(もどき) */	// 40[sec]
	SPELL_17_alice_doll_02, 			/* [アリス] 作成中 */
//
	SPELL_18_hana_test, 				/* [魅魔] 花てすと弾幕 */
	SPELL_19_mima_sekkin,				/* [魅魔] 接近弾幕 */
	SPELL_1a_mima_toge, 				/* [魅魔] トゲてすと弾幕 */
	SPELL_1b_cirno_icecle_fall, 		/* [チルノ] 氷符「アイシクルフォール」 */
	SPELL_1c_kakuya_tamanoe,			/* [輝夜]、蓬莱の玉の枝もどき。作成中 */
	SPELL_1d_amefuri_test,				/* [] 雨てすと */
	SPELL_1e_kaguya01,					/* [輝夜] むりやり変換1 */
	SPELL_1f_kaguya04,					/* [輝夜] むりやり変換2 */
//	/* ボス用 */
	SPELL_20_aya_misogi1,				/*	1[文] の通常弾っぽい弾幕撃たせてみるテスト1 */
	SPELL_21_aya_misogi2,				/*	2[文] の通常弾っぽい弾幕撃たせてみるテスト2 */
	SPELL_22_ame_no_yatimata,			/*	3[文] 岐符「天の八衢」 */
	SPELL_23_aya_merin_test,			/*	4[文] に美鈴っぽい弾幕撃たせてみるテスト */
	SPELL_24_aya_doll,					/*	5[文] 人形弾幕 */ //40[sec]
	SPELL_25_houka_kenran,				/*	6[紅美鈴] 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	SPELL_26_aya_saifu, 				/*	7[文] 塞符 */
	SPELL_27_,							/*	8[パチェ] のレーザーもどき */
//
	SPELL_28_remilia_tamaoki1,			/*	9[咲夜] 紅 面ボス  れみりゃっぽい咲夜の玉置弾幕(予定) */	//	SPE LL_28_rumia_night_bird, 		/*	9[ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	SPELL_29_rumia_demarcation, 		/* 10[ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似た弾幕(予定) */
	SPELL_2a_sakuya_baramaki1,			/* 11[咲夜] 紅5面中-ボス ばら撒き1 弾幕 () */
	SPELL_2b_sakuya_baramaki2,			/* 12[咲夜] 紅5面中-ボス ばら撒き2 弾幕 () */
	SPELL_2c_sakuya_blue_red,			/* 13[咲夜] 紅5面中-ボス 「通常攻撃3」青赤ナイフ */
	SPELL_2d_sakuya_misogi_lunatic, 	/* 14[咲夜] 紅5面中-ボス 禊弾幕 / 紅5面中-ボス 咲夜 「通常攻撃」lunatic */
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
	SPELL_40_SONOTA_DEBUG_CW_AO,		/* [その他] デバッグ弾幕CW青 */
	SPELL_41_SONOTA_DEBUG_CCW_AKA,		/* [その他] デバッグ弾幕CCW赤 */
	SPELL_42_PACHE_LASER1,				/* [パチェ] 「通常攻撃1」のレーザーもどき */
	SPELL_43_PACHE_LASER2,				/* [パチェ] 「通常攻撃3」のレーザーもどき */
//
	SPELL_44,				/* */
	SPELL_45,				/* */
	SPELL_46,				/* */
	SPELL_47_sakuya_meek,				/* [咲夜] むりやり変換3 */
//	SPELL_2e_SONOTA_DEBUG_CW_AO,		/* [その他] デバッグ弾幕CW青 */
//	SPELL_2f_SONOTA_DEBUG_CCW_AKA,		/* [その他] デバッグ弾幕CCW赤 */
//
	SPELL_MAX,
};

/* target->tmp_angleCCW65536 に 自機狙い弾の角度を計算 */
extern void tmp_angleCCW65536_src_nerai(SPRITE *src, SPRITE *dest);
extern void tmp_angleCCW65536_jiki_nerai(SPRITE *target);

/* スペル(弾幕)の時間切れを設定 */
extern void spell_set_time_out(SPRITE *src);

/* スペル(弾幕)生成 */
extern void spell_generator(SPRITE *src);



#endif /* _SPELL_H_ */


#ifndef _BOSS_COMMON_H_
#define _BOSS_COMMON_H_
enum
{
	DESTROY_CHECK_00_WIN_BOSS = 0,
	DESTROY_CHECK_01_IS_TIME_OUT,
};
/*static*/extern  void boss_destroy_check_type(SPRITE *src/*敵自体*/, int check_type);/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */
/*static*/extern  void callback_hit_boss_dummy(SPRITE *src, SPRITE *tama); /*, int angle*/
/*static*/extern  void callback_hit_boss(SPRITE *src, SPRITE *tama); /*, int angle*/

/*static*/extern  void common_boss_put_items(SPRITE *src);

/*static*/extern  void lose_boss(SPRITE *src);






/* スペカシステム */
/*static*/extern void spell_card_get_spell_number(SPRITE *src);
#endif /* _BOSS_H_ */
