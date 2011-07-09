#ifndef _SPELL_CARD_VALUE_H_
#define _SPELL_CARD_VALUE_H_

//	SPELL_CARD_00_OFF = 0,
enum
{
// チルノ エキストラステージ
	SPELL_CARD_00E_chrno_000 = 0,	/* "形態変更" */
	SPELL_CARD_00N_chrno_000,		/* "形態変更" */
	SPELL_CARD_00H_chrno_000,		/* "形態変更" */
	SPELL_CARD_00L_chrno_000,		/* "形態変更" */
	SPELL_CARD_01E_chrno_aaa,		/* 第一形態: "通常攻撃1"*/
	SPELL_CARD_01N_chrno_aaa,		/* 第一形態: "通常攻撃1"*/
	SPELL_CARD_01H_chrno_aaa,		/* 第一形態: "通常攻撃1"*/
	SPELL_CARD_01L_chrno_aaa,		/* 第一形態: "通常攻撃1"*/
	SPELL_CARD_02E_chrno_bbb,		/* 第二形態: 氷符「アイシクルフォール」*/
	SPELL_CARD_02N_chrno_bbb,		/* 第二形態: 氷符「アイシクルフォール」*/
	SPELL_CARD_02H_chrno_bbb,		/* 第二形態: 氷符「アイシクルフォール」*/
	SPELL_CARD_02L_chrno_bbb,		/* 第二形態: 氷符「アイシクルフォール」*/
	SPELL_CARD_03E_chrno_ccc,		/* 第三形態:「通常攻撃2」 */
	SPELL_CARD_03N_chrno_ccc,		/* 第三形態:「通常攻撃2」 */
	SPELL_CARD_03H_chrno_ccc,		/* 第三形態:「通常攻撃2」 */
	SPELL_CARD_03L_chrno_ccc,		/* 第三形態:「通常攻撃2」 */
	SPELL_CARD_04E_chrno_ddd,		/* 第四形態: 凍符「パーフェクトフリーズ」*/
	SPELL_CARD_04N_chrno_ddd,		/* 第四形態: 凍符「パーフェクトフリーズ」*/
	SPELL_CARD_04H_chrno_ddd,		/* 第四形態: 凍符「パーフェクトフリーズ」*/
	SPELL_CARD_04L_chrno_ddd,		/* 第四形態: 凍符「パーフェクトフリーズ」*/
	SPELL_CARD_05E_chrno_eee,		/* 第五形態: 雪符「ダイアモンドブリザード」*/
	SPELL_CARD_05N_chrno_eee,		/* 第五形態: 雪符「ダイアモンドブリザード」*/
	SPELL_CARD_05H_chrno_eee,		/* 第五形態: 雪符「ダイアモンドブリザード」*/
	SPELL_CARD_05L_chrno_eee,		/* 第五形態: 雪符「ダイアモンドブリザード」*/
// 咲夜 6面
	SPELL_CARD_00E_sakuya_000,	/* "形態変更" */
	SPELL_CARD_00N_sakuya_000,	/* "形態変更" */
	SPELL_CARD_00H_sakuya_000,	/* "形態変更" */
	SPELL_CARD_00L_sakuya_000,	/* "形態変更" */
	SPELL_CARD_01E_sakuya_aaa,		/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	SPELL_CARD_01N_sakuya_aaa,		/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	SPELL_CARD_01H_sakuya_aaa,		/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	SPELL_CARD_01L_sakuya_aaa,		/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	SPELL_CARD_02E_sakuya_bbb,		/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	SPELL_CARD_02N_sakuya_bbb,		/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	SPELL_CARD_02H_sakuya_bbb,		/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	SPELL_CARD_02L_sakuya_bbb,		/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	SPELL_CARD_03E_sakuya_ccc,		/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	SPELL_CARD_03N_sakuya_ccc,		/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	SPELL_CARD_03H_sakuya_ccc,		/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	SPELL_CARD_03L_sakuya_ccc,		/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	SPELL_CARD_04E_sakuya_ddd,		/* 第四形態: 魔方陣生成 */
	SPELL_CARD_04N_sakuya_ddd,		/* 第四形態: 魔方陣生成 */
	SPELL_CARD_04H_sakuya_ddd,		/* 第四形態: 魔方陣生成 */
	SPELL_CARD_04L_sakuya_ddd,		/* 第四形態: 魔方陣生成 */
	SPELL_CARD_05E_sakuya_eee,		/* 第五形態: (黄色マスカット弾A) */
	SPELL_CARD_05N_sakuya_eee,		/* 第五形態: (黄色マスカット弾A) */
	SPELL_CARD_05H_sakuya_eee,		/* 第五形態: (黄色マスカット弾A) */
	SPELL_CARD_05L_sakuya_eee,		/* 第五形態: (黄色マスカット弾A) */
	SPELL_CARD_06E_sakuya_fff,		/* 第六形態: (黄色マスカット弾B) */
	SPELL_CARD_06N_sakuya_fff,		/* 第六形態: (黄色マスカット弾B) */
	SPELL_CARD_06H_sakuya_fff,		/* 第六形態: (黄色マスカット弾B) */
	SPELL_CARD_06L_sakuya_fff,		/* 第六形態: (黄色マスカット弾B) */
	SPELL_CARD_07E_sakuya_ggg,		/* 第七形態: (分散魔方陣)追加計画中 */
	SPELL_CARD_07N_sakuya_ggg,		/* 第七形態: (分散魔方陣)追加計画中 */
	SPELL_CARD_07H_sakuya_ggg,		/* 第七形態: (分散魔方陣)追加計画中 */
	SPELL_CARD_07L_sakuya_ggg,		/* 第七形態: (分散魔方陣)追加計画中 */
	SPELL_CARD_08E_sakuya_hhh,		/* 第八形態: (時止めナイフ)追加計画中 */
	SPELL_CARD_08N_sakuya_hhh,		/* 第八形態: (時止めナイフ)追加計画中 */
	SPELL_CARD_08H_sakuya_hhh,		/* 第八形態: (時止めナイフ)追加計画中 */
	SPELL_CARD_08L_sakuya_hhh,		/* 第八形態: (時止めナイフ)追加計画中 */
	SPELL_CARD_09E_sakuya_iii,		/* 第九形態: 最終形態(その1) */
	SPELL_CARD_09N_sakuya_iii,		/* 第九形態: 最終形態(その1) */
	SPELL_CARD_09H_sakuya_iii,		/* 第九形態: 最終形態(その1) */
	SPELL_CARD_09L_sakuya_iii,		/* 第九形態: 最終形態(その1) */
	SPELL_CARD_10E_sakuya_jjj,		/* 第10形態: 最終形態(その2) */
	SPELL_CARD_10N_sakuya_jjj,		/* 第10形態: 最終形態(その2) */
	SPELL_CARD_10H_sakuya_jjj,		/* 第10形態: 最終形態(その2) */
	SPELL_CARD_10L_sakuya_jjj,		/* 第10形態: 最終形態(その2) */
	SPELL_CARD_11E_sakuya_kkk,		/* 第11形態: 最終形態(その3) */
	SPELL_CARD_11N_sakuya_kkk,		/* 第11形態: 最終形態(その3) */
	SPELL_CARD_11H_sakuya_kkk,		/* 第11形態: 最終形態(その3) */
	SPELL_CARD_11L_sakuya_kkk,		/* 第11形態: 最終形態(その3) */
// パチェA 5面
	SPELL_CARD_00E_pacheA_000,	/* "形態変更" */
	SPELL_CARD_00N_pacheA_000,	/* "形態変更" */
	SPELL_CARD_00H_pacheA_000,	/* "形態変更" */
	SPELL_CARD_00L_pacheA_000,	/* "形態変更" */
	SPELL_CARD_11E_pacheA_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11N_pacheA_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11H_pacheA_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11L_pacheA_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_12E_pacheA_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12N_pacheA_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12H_pacheA_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12L_pacheA_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_13E_pacheA_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13N_pacheA_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13H_pacheA_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13L_pacheA_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_14E_pacheA_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14N_pacheA_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14H_pacheA_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14L_pacheA_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_15E_pacheA_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15N_pacheA_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15H_pacheA_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15L_pacheA_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_16E_pacheA_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16N_pacheA_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16H_pacheA_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16L_pacheA_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_17E_pacheA_hhh,
	SPELL_CARD_17N_pacheA_hhh,
	SPELL_CARD_17H_pacheA_hhh,
	SPELL_CARD_17L_pacheA_hhh,
// パチェB 5面
	SPELL_CARD_00E_pacheB_000,	/* "形態変更" */
	SPELL_CARD_00N_pacheB_000,	/* "形態変更" */
	SPELL_CARD_00H_pacheB_000,	/* "形態変更" */
	SPELL_CARD_00L_pacheB_000,	/* "形態変更" */
	SPELL_CARD_11E_pacheB_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11N_pacheB_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11H_pacheB_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11L_pacheB_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_12E_pacheB_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12N_pacheB_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12H_pacheB_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12L_pacheB_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_13E_pacheB_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13N_pacheB_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13H_pacheB_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13L_pacheB_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_14E_pacheB_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14N_pacheB_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14H_pacheB_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14L_pacheB_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_15E_pacheB_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15N_pacheB_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15H_pacheB_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15L_pacheB_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_16E_pacheB_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16N_pacheB_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16H_pacheB_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16L_pacheB_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_17E_pacheB_hhh,
	SPELL_CARD_17N_pacheB_hhh,
	SPELL_CARD_17H_pacheB_hhh,
	SPELL_CARD_17L_pacheB_hhh,
// パチェC 5面
	SPELL_CARD_00E_pacheC_000,	/* "形態変更" */
	SPELL_CARD_00N_pacheC_000,	/* "形態変更" */
	SPELL_CARD_00H_pacheC_000,	/* "形態変更" */
	SPELL_CARD_00L_pacheC_000,	/* "形態変更" */
	SPELL_CARD_11E_pacheC_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11N_pacheC_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11H_pacheC_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11L_pacheC_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_12E_pacheC_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12N_pacheC_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12H_pacheC_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12L_pacheC_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_13E_pacheC_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13N_pacheC_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13H_pacheC_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13L_pacheC_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_14E_pacheC_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14N_pacheC_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14H_pacheC_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14L_pacheC_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_15E_pacheC_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15N_pacheC_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15H_pacheC_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15L_pacheC_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_16E_pacheC_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16N_pacheC_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16H_pacheC_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16L_pacheC_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_17E_pacheC_hhh,
	SPELL_CARD_17N_pacheC_hhh,
	SPELL_CARD_17H_pacheC_hhh,
	SPELL_CARD_17L_pacheC_hhh,
// パチェD 5面
	SPELL_CARD_00E_pacheD_000,	/* "形態変更" */
	SPELL_CARD_00N_pacheD_000,	/* "形態変更" */
	SPELL_CARD_00H_pacheD_000,	/* "形態変更" */
	SPELL_CARD_00L_pacheD_000,	/* "形態変更" */
	SPELL_CARD_11E_pacheD_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11N_pacheD_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11H_pacheD_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_11L_pacheD_bbb,		/* 第一形態: 通常攻撃1 */
	SPELL_CARD_12E_pacheD_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12N_pacheD_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12H_pacheD_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_12L_pacheD_ccc,		/* 第二形態: スペルカードⅠ */
	SPELL_CARD_13E_pacheD_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13N_pacheD_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13H_pacheD_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_13L_pacheD_ddd,		/* 第三形態: 通常攻撃2 */
	SPELL_CARD_14E_pacheD_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14N_pacheD_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14H_pacheD_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_14L_pacheD_eee,		/* 第四形態: スペルカードⅡ */
	SPELL_CARD_15E_pacheD_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15N_pacheD_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15H_pacheD_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_15L_pacheD_fff,		/* 第五形態: スペルカードⅢ */
	SPELL_CARD_16E_pacheD_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16N_pacheD_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16H_pacheD_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_16L_pacheD_ggg,		/* 第六形態: スペルカードⅣ */
	SPELL_CARD_17E_pacheD_hhh,
	SPELL_CARD_17N_pacheD_hhh,
	SPELL_CARD_17H_pacheD_hhh,
	SPELL_CARD_17L_pacheD_hhh,
// 文 4面
	SPELL_CARD_00E_aya_000,
	SPELL_CARD_00N_aya_000,
	SPELL_CARD_00H_aya_000,
	SPELL_CARD_00L_aya_000,
	SPELL_CARD_11E_aya_bbb,
	SPELL_CARD_11N_aya_bbb,
	SPELL_CARD_11H_aya_bbb,
	SPELL_CARD_11L_aya_bbb,
	SPELL_CARD_12E_aya_ccc,
	SPELL_CARD_12N_aya_ccc,
	SPELL_CARD_12H_aya_ccc,
	SPELL_CARD_12L_aya_ccc,
	SPELL_CARD_13E_aya_ddd,
	SPELL_CARD_13N_aya_ddd,
	SPELL_CARD_13H_aya_ddd,
	SPELL_CARD_13L_aya_ddd,
	SPELL_CARD_14E_aya_eee,
	SPELL_CARD_14N_aya_eee,
	SPELL_CARD_14H_aya_eee,
	SPELL_CARD_14L_aya_eee,
	SPELL_CARD_15E_aya_fff,
	SPELL_CARD_15N_aya_fff,
	SPELL_CARD_15H_aya_fff,
	SPELL_CARD_15L_aya_fff,
	SPELL_CARD_16E_aya_ggg,
	SPELL_CARD_16N_aya_ggg,
	SPELL_CARD_16H_aya_ggg,
	SPELL_CARD_16L_aya_ggg,
	SPELL_CARD_17E_aya_hhh,
	SPELL_CARD_17N_aya_hhh,
	SPELL_CARD_17H_aya_hhh,
	SPELL_CARD_17L_aya_hhh,
//	SPELL_CARD_18E_aya_iii,
//	SPELL_CARD_18N_aya_iii,
//	SPELL_CARD_18H_aya_iii,
//	SPELL_CARD_18L_aya_iii,
//	SPELL_CARD_19E_aya_jjj,
//	SPELL_CARD_19N_aya_jjj,
//	SPELL_CARD_19H_aya_jjj,
//	SPELL_CARD_19L_aya_jjj,
// 輝夜 3面
	SPELL_CARD_00E_kaguya_000,
	SPELL_CARD_00N_kaguya_000,
	SPELL_CARD_00H_kaguya_000,
	SPELL_CARD_00L_kaguya_000,
	SPELL_CARD_11E_kaguya_bbb,
	SPELL_CARD_11N_kaguya_bbb,
	SPELL_CARD_11H_kaguya_bbb,
	SPELL_CARD_11L_kaguya_bbb,
	SPELL_CARD_12E_kaguya_ccc,
	SPELL_CARD_12N_kaguya_ccc,
	SPELL_CARD_12H_kaguya_ccc,
	SPELL_CARD_12L_kaguya_ccc,
	SPELL_CARD_13E_kaguya_ddd,
	SPELL_CARD_13N_kaguya_ddd,
	SPELL_CARD_13H_kaguya_ddd,
	SPELL_CARD_13L_kaguya_ddd,
	SPELL_CARD_14E_kaguya_eee,
	SPELL_CARD_14N_kaguya_eee,
	SPELL_CARD_14H_kaguya_eee,
	SPELL_CARD_14L_kaguya_eee,
	SPELL_CARD_15E_kaguya_fff,
	SPELL_CARD_15N_kaguya_fff,
	SPELL_CARD_15H_kaguya_fff,
	SPELL_CARD_15L_kaguya_fff,
	SPELL_CARD_16E_kaguya_ggg,
	SPELL_CARD_16N_kaguya_ggg,
	SPELL_CARD_16H_kaguya_ggg,
	SPELL_CARD_16L_kaguya_ggg,
	SPELL_CARD_17E_kaguya_hhh,
	SPELL_CARD_17N_kaguya_hhh,
	SPELL_CARD_17H_kaguya_hhh,
	SPELL_CARD_17L_kaguya_hhh,
// 未定 2面
	SPELL_CARD_00E_mima_000,
	SPELL_CARD_00N_mima_000,
	SPELL_CARD_00H_mima_000,
	SPELL_CARD_00L_mima_000,
	SPELL_CARD_11E_mima_bbb,
	SPELL_CARD_11N_mima_bbb,
	SPELL_CARD_11H_mima_bbb,
	SPELL_CARD_11L_mima_bbb,
	SPELL_CARD_12E_mima_ccc,
	SPELL_CARD_12N_mima_ccc,
	SPELL_CARD_12H_mima_ccc,
	SPELL_CARD_12L_mima_ccc,
	SPELL_CARD_13E_mima_ddd,
	SPELL_CARD_13N_mima_ddd,
	SPELL_CARD_13H_mima_ddd,
	SPELL_CARD_13L_mima_ddd,
	SPELL_CARD_14E_mima_eee,
	SPELL_CARD_14N_mima_eee,
	SPELL_CARD_14H_mima_eee,
	SPELL_CARD_14L_mima_eee,
	SPELL_CARD_15E_mima_fff,
	SPELL_CARD_15N_mima_fff,
	SPELL_CARD_15H_mima_fff,
	SPELL_CARD_15L_mima_fff,
	SPELL_CARD_16E_mima_ggg,
	SPELL_CARD_16N_mima_ggg,
	SPELL_CARD_16H_mima_ggg,
	SPELL_CARD_16L_mima_ggg,
	SPELL_CARD_17E_mima_hhh,
	SPELL_CARD_17N_mima_hhh,
	SPELL_CARD_17H_mima_hhh,
	SPELL_CARD_17L_mima_hhh,
// アリス 1面
	SPELL_CARD_00E_alice_000,
	SPELL_CARD_00N_alice_000,
	SPELL_CARD_00H_alice_000,
	SPELL_CARD_00L_alice_000,
	SPELL_CARD_11E_alice_bbb,
	SPELL_CARD_11N_alice_bbb,
	SPELL_CARD_11H_alice_bbb,
	SPELL_CARD_11L_alice_bbb,
	SPELL_CARD_12E_alice_ccc,
	SPELL_CARD_12N_alice_ccc,
	SPELL_CARD_12H_alice_ccc,
	SPELL_CARD_12L_alice_ccc,
	SPELL_CARD_13E_alice_ddd,
	SPELL_CARD_13N_alice_ddd,
	SPELL_CARD_13H_alice_ddd,
	SPELL_CARD_13L_alice_ddd,
	SPELL_CARD_14E_alice_eee,
	SPELL_CARD_14N_alice_eee,
	SPELL_CARD_14H_alice_eee,
	SPELL_CARD_14L_alice_eee,
	SPELL_CARD_15E_alice_fff,
	SPELL_CARD_15N_alice_fff,
	SPELL_CARD_15H_alice_fff,
	SPELL_CARD_15L_alice_fff,
	//SPELL_CARD_16E_alice_ggg,
	//SPELL_CARD_16N_alice_ggg,
	//SPELL_CARD_16H_alice_ggg,
	//SPELL_CARD_16L_alice_ggg,
//	SPELL_CARD_17E_alice_hhh,
//	SPELL_CARD_17N_alice_hhh,
//	SPELL_CARD_17H_alice_hhh,
//	SPELL_CARD_17L_alice_hhh,
//	SPELL_CARD_18E_alice_iii,
//	SPELL_CARD_18N_alice_iii,
//	SPELL_CARD_18H_alice_iii,
//	SPELL_CARD_18L_alice_iii,
//	SPELL_CARD_19E_alice_jjj,
//	SPELL_CARD_19N_alice_jjj,
//	SPELL_CARD_19H_alice_jjj,
//	SPELL_CARD_19L_alice_jjj,
//
	SPELL_CARD_MAX	/* 最大数 */
};

#endif /* _SPELL_CARD_VALUE_H_ */

