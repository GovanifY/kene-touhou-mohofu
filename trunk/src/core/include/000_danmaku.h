
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
		弾幕定義
	-------------------------------------------------------
	★弾幕はシステムでサポートする構想があります。(構想中)
	システムでサポートされると、どのボスが、どの弾幕でも撃てるようになります。
	-------------------------------------------------------
	boss->boss_base_danmaku_type に撃ちたい弾幕をセットすると、弾幕生成開始。
	弾幕生成が終了すると自動的に boss->boss_base_danmaku_type が DANMAKU_00 になる。
	-------------------------------------------------------
	弾幕生成中は boss->boss_base_danmaku_time_out が勝手に毎フレーム減算され、0 になると弾幕生成終了。
	この時間は弾幕ごとに違う標準時間が設定されているので、これを設定するには
	danmaku_set_time_out(boss); 	// 弾幕の時間切れを設定
	とする。
	---------------------------------------------------------*/
enum
{
	DANMAKU_00 = 0, 			/* 弾幕生成終了フラグ。 */
	/* [咲夜] */
	DANMAKU_01_sakuya,			/* 咲夜禊弾幕 / 紅5面中ボス 咲夜 「通常攻撃」 */
	DANMAKU_02_sakuya,			/* 咲夜24弾幕 (奇術「ミスディレクションもどき(1/2)」) */
	DANMAKU_03_sakuya,			/* 咲夜11弾幕 (奇術「ミスディレクションもどき(2/2)」) */
//	咲夜予約
	DANMAKU_04_sakuya,			/* 紅5面ボス 咲夜 「通常攻撃1(1/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_05_sakuya,			/* 紅5面ボス 咲夜 「通常攻撃1/2(2/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_06_sakuya,			/* 紅5面ボス 咲夜 幻象「ルナクロック(1/2)」にちょっとだけ似た弾幕(予定) */
	DANMAKU_07_sakuya,			/* 紅5面ボス 咲夜 「通常攻撃3(1/2)」にちょっとだけ似た弾幕(予定) */
//	/* [アリス(仮)] -------------------- */
	DANMAKU_08_rumia,			/* 紅1面ボス ルーミア 夜符「ナイトバード」にちょっとだけ似た弾幕(予定) */
	DANMAKU_09_alice,			/* アリスっぽい？テキトー弾幕 */
	DANMAKU_0a_houka_kenran,	/* 紅美鈴 華符「芳華絢爛」にちょっとだけ似た弾幕(予定) */
	DANMAKU_0b_alice_doll,		/* アリス人形弾幕 */	//40[sec]
	DANMAKU_0c_hana_test,		/* 魅魔、花てすと弾幕 */
	DANMAKU_0d_mima_sekkin, 	/* 魅魔、接近弾幕 */
	DANMAKU_0e_test,
	DANMAKU_0f_test,
//	/* [チルノ(仮)] -------------------- */
	DANMAKU_10_cirno,			/* チルノ禊弾幕 */
	DANMAKU_11_tengu,/*???*/
	DANMAKU_12_aya_merin_test,	/* 文に美鈴っぽい弾幕撃たせてみるテスト */
	DANMAKU_13_perfect_freeze,	/* パーフェクトフリーズ */
	DANMAKU_14,
	DANMAKU_15,
	DANMAKU_16,
	DANMAKU_17,
//	/* [輝夜(仮)] -------------------- */
	DANMAKU_18_kaguya01,
	DANMAKU_19_kaguya04,
	DANMAKU_1a_dai_yousei_01,	/* 全周 48分割、時計回り、緑クナイ弾 */
	DANMAKU_1b_dai_yousei_02,	/* 全周 48分割、反時計回り、赤クナイ弾 */
	DANMAKU_1c,
	DANMAKU_1d,
	DANMAKU_1e,
	DANMAKU_1f,
//
	DANMAKU_MAX,
};

/* target->tmp_angleCCW1024 に 自機狙い弾の角度を計算 */
extern void tmp_angleCCW1024_jikinerai(SPRITE *p, SPRITE *target);
extern void tmp_angleCCW65536_jikinerai(SPRITE *p, SPRITE *target);

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
#endif /* _BOSS_H_ */
