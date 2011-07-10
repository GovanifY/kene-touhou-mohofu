
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	カードアドレス
	-------------------------------------------------------
	カードの絶対位置を示す一意の値（カードアドレス）を定義します。
	-------------------------------------------------------
	絶対位置は、難易度別に異なります。
	また五面の場合は、キャラ別によって絶対位置が異なります。
	-------------------------------------------------------
	カードアドレスはカード番号とは異なります。
	違うカードアドレスに同じカード番号を配置する事が出来ます。
	-------------------------------------------------------
	具体例では、
	パチェ 5面	霊夢 B (夢符)で、8形態目（カードアドレス）が
	土＆金符「エメラルドメガリス」（カード番号）で、
	パチェ 5面	魔理沙 A (魔符)、7形態目（カードアドレス）が
	土＆金符「エメラルドメガリス」（カード番号）と言う事です。
---------------------------------------------------------*/

#ifndef _CARD_ADDRESS_VALUE_H_
#define _CARD_ADDRESS_VALUE_H_

//	CARD_ADDRESS_00_OFF = 0,
enum
{
// チルノ エキストラステージ
	CARD_ADDRESS_00E_chrno_000 = 0, /* "形態変更" */
	CARD_ADDRESS_00N_chrno_000, 	/* "形態変更" */
	CARD_ADDRESS_00H_chrno_000, 	/* "形態変更" */
	CARD_ADDRESS_00L_chrno_000, 	/* "形態変更" */
	CARD_ADDRESS_01E_chrno_aaa, 	/* 第一形態: "通常攻撃1"*/
	CARD_ADDRESS_01N_chrno_aaa, 	/* 第一形態: "通常攻撃1"*/
	CARD_ADDRESS_01H_chrno_aaa, 	/* 第一形態: "通常攻撃1"*/
	CARD_ADDRESS_01L_chrno_aaa, 	/* 第一形態: "通常攻撃1"*/
	CARD_ADDRESS_02E_chrno_bbb, 	/* 第二形態: 氷符「アイシクルフォール」*/
	CARD_ADDRESS_02N_chrno_bbb, 	/* 第二形態: 氷符「アイシクルフォール」*/
	CARD_ADDRESS_02H_chrno_bbb, 	/* 第二形態: 氷符「アイシクルフォール」*/
	CARD_ADDRESS_02L_chrno_bbb, 	/* 第二形態: 氷符「アイシクルフォール」*/
	CARD_ADDRESS_03E_chrno_ccc, 	/* 第三形態:「通常攻撃2」 */
	CARD_ADDRESS_03N_chrno_ccc, 	/* 第三形態:「通常攻撃2」 */
	CARD_ADDRESS_03H_chrno_ccc, 	/* 第三形態:「通常攻撃2」 */
	CARD_ADDRESS_03L_chrno_ccc, 	/* 第三形態:「通常攻撃2」 */
	CARD_ADDRESS_04E_chrno_ddd, 	/* 第四形態: 凍符「パーフェクトフリーズ」*/
	CARD_ADDRESS_04N_chrno_ddd, 	/* 第四形態: 凍符「パーフェクトフリーズ」*/
	CARD_ADDRESS_04H_chrno_ddd, 	/* 第四形態: 凍符「パーフェクトフリーズ」*/
	CARD_ADDRESS_04L_chrno_ddd, 	/* 第四形態: 凍符「パーフェクトフリーズ」*/
	CARD_ADDRESS_05E_chrno_eee, 	/* 第五形態: 雪符「ダイアモンドブリザード」*/
	CARD_ADDRESS_05N_chrno_eee, 	/* 第五形態: 雪符「ダイアモンドブリザード」*/
	CARD_ADDRESS_05H_chrno_eee, 	/* 第五形態: 雪符「ダイアモンドブリザード」*/
	CARD_ADDRESS_05L_chrno_eee, 	/* 第五形態: 雪符「ダイアモンドブリザード」*/
// 咲夜 6面
	CARD_ADDRESS_00E_sakuya_000,	/* "形態変更" */
	CARD_ADDRESS_00N_sakuya_000,	/* "形態変更" */
	CARD_ADDRESS_00H_sakuya_000,	/* "形態変更" */
	CARD_ADDRESS_00L_sakuya_000,	/* "形態変更" */
	CARD_ADDRESS_01E_sakuya_aaa,	/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	CARD_ADDRESS_01N_sakuya_aaa,	/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	CARD_ADDRESS_01H_sakuya_aaa,	/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	CARD_ADDRESS_01L_sakuya_aaa,	/* 第一形態: 通常攻撃(左右に動いて禊弾撃ち) */
	CARD_ADDRESS_02E_sakuya_bbb,	/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	CARD_ADDRESS_02N_sakuya_bbb,	/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	CARD_ADDRESS_02H_sakuya_bbb,	/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	CARD_ADDRESS_02L_sakuya_bbb,	/* 第二形態: 奇術「ミスディレクション」(全方位、豆まき) */
	CARD_ADDRESS_03E_sakuya_ccc,	/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	CARD_ADDRESS_03N_sakuya_ccc,	/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	CARD_ADDRESS_03H_sakuya_ccc,	/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	CARD_ADDRESS_03L_sakuya_ccc,	/* 第三形態: 時雨「ペッパーナイフ」(垂直ナイフが落ちてくるよ) */
	CARD_ADDRESS_04E_sakuya_ddd,	/* 第四形態: 魔方陣生成 */
	CARD_ADDRESS_04N_sakuya_ddd,	/* 第四形態: 魔方陣生成 */
	CARD_ADDRESS_04H_sakuya_ddd,	/* 第四形態: 魔方陣生成 */
	CARD_ADDRESS_04L_sakuya_ddd,	/* 第四形態: 魔方陣生成 */
	CARD_ADDRESS_05E_sakuya_eee,	/* 第五形態: (黄色マスカット弾A) */
	CARD_ADDRESS_05N_sakuya_eee,	/* 第五形態: (黄色マスカット弾A) */
	CARD_ADDRESS_05H_sakuya_eee,	/* 第五形態: (黄色マスカット弾A) */
	CARD_ADDRESS_05L_sakuya_eee,	/* 第五形態: (黄色マスカット弾A) */
	CARD_ADDRESS_06E_sakuya_fff,	/* 第六形態: (黄色マスカット弾B) */
	CARD_ADDRESS_06N_sakuya_fff,	/* 第六形態: (黄色マスカット弾B) */
	CARD_ADDRESS_06H_sakuya_fff,	/* 第六形態: (黄色マスカット弾B) */
	CARD_ADDRESS_06L_sakuya_fff,	/* 第六形態: (黄色マスカット弾B) */
	CARD_ADDRESS_07E_sakuya_ggg,	/* 第七形態: (分散魔方陣)追加計画中 */
	CARD_ADDRESS_07N_sakuya_ggg,	/* 第七形態: (分散魔方陣)追加計画中 */
	CARD_ADDRESS_07H_sakuya_ggg,	/* 第七形態: (分散魔方陣)追加計画中 */
	CARD_ADDRESS_07L_sakuya_ggg,	/* 第七形態: (分散魔方陣)追加計画中 */
	CARD_ADDRESS_08E_sakuya_hhh,	/* 第八形態: (時止めナイフ)追加計画中 */
	CARD_ADDRESS_08N_sakuya_hhh,	/* 第八形態: (時止めナイフ)追加計画中 */
	CARD_ADDRESS_08H_sakuya_hhh,	/* 第八形態: (時止めナイフ)追加計画中 */
	CARD_ADDRESS_08L_sakuya_hhh,	/* 第八形態: (時止めナイフ)追加計画中 */
	CARD_ADDRESS_09E_sakuya_iii,	/* 第九形態: 最終形態(その1) */
	CARD_ADDRESS_09N_sakuya_iii,	/* 第九形態: 最終形態(その1) */
	CARD_ADDRESS_09H_sakuya_iii,	/* 第九形態: 最終形態(その1) */
	CARD_ADDRESS_09L_sakuya_iii,	/* 第九形態: 最終形態(その1) */
	CARD_ADDRESS_10E_sakuya_jjj,	/* 第10形態: 最終形態(その2) */
	CARD_ADDRESS_10N_sakuya_jjj,	/* 第10形態: 最終形態(その2) */
	CARD_ADDRESS_10H_sakuya_jjj,	/* 第10形態: 最終形態(その2) */
	CARD_ADDRESS_10L_sakuya_jjj,	/* 第10形態: 最終形態(その2) */
	CARD_ADDRESS_11E_sakuya_kkk,	/* 第11形態: 最終形態(その3) */
	CARD_ADDRESS_11N_sakuya_kkk,	/* 第11形態: 最終形態(その3) */
	CARD_ADDRESS_11H_sakuya_kkk,	/* 第11形態: 最終形態(その3) */
	CARD_ADDRESS_11L_sakuya_kkk,	/* 第11形態: 最終形態(その3) */
// パチェA 5面
	CARD_ADDRESS_00E_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_00N_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_00H_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_00L_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_11E_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11N_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11H_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11L_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_12E_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12N_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12H_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12L_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_13E_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13N_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13H_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13L_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_14E_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14N_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14H_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14L_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_15E_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15N_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15H_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15L_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_16E_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16N_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16H_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16L_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_17E_pacheA_hhh,
	CARD_ADDRESS_17N_pacheA_hhh,
	CARD_ADDRESS_17H_pacheA_hhh,
	CARD_ADDRESS_17L_pacheA_hhh,
// パチェB 5面
	CARD_ADDRESS_00E_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_00N_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_00H_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_00L_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_11E_pacheB_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11N_pacheB_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11H_pacheB_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11L_pacheB_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_12E_pacheB_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12N_pacheB_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12H_pacheB_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12L_pacheB_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_13E_pacheB_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13N_pacheB_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13H_pacheB_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13L_pacheB_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_14E_pacheB_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14N_pacheB_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14H_pacheB_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14L_pacheB_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_15E_pacheB_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15N_pacheB_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15H_pacheB_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15L_pacheB_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_16E_pacheB_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16N_pacheB_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16H_pacheB_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16L_pacheB_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_17E_pacheB_hhh,
	CARD_ADDRESS_17N_pacheB_hhh,
	CARD_ADDRESS_17H_pacheB_hhh,
	CARD_ADDRESS_17L_pacheB_hhh,
// パチェC 5面
	CARD_ADDRESS_00E_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_00N_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_00H_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_00L_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_11E_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11N_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11H_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11L_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_12E_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12N_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12H_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12L_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_13E_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13N_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13H_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13L_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_14E_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14N_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14H_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14L_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_15E_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15N_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15H_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15L_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_16E_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16N_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16H_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16L_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_17E_pacheC_hhh,
	CARD_ADDRESS_17N_pacheC_hhh,
	CARD_ADDRESS_17H_pacheC_hhh,
	CARD_ADDRESS_17L_pacheC_hhh,
// パチェD 5面
	CARD_ADDRESS_00E_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_00N_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_00H_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_00L_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_11E_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11N_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11H_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_11L_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_12E_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12N_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12H_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_12L_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_13E_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13N_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13H_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_13L_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_14E_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14N_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14H_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_14L_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_15E_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15N_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15H_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_15L_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_16E_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16N_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16H_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_16L_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_17E_pacheD_hhh,
	CARD_ADDRESS_17N_pacheD_hhh,
	CARD_ADDRESS_17H_pacheD_hhh,
	CARD_ADDRESS_17L_pacheD_hhh,
// 文 4面
	CARD_ADDRESS_00E_aya_000,
	CARD_ADDRESS_00N_aya_000,
	CARD_ADDRESS_00H_aya_000,
	CARD_ADDRESS_00L_aya_000,
	CARD_ADDRESS_11E_aya_bbb,
	CARD_ADDRESS_11N_aya_bbb,
	CARD_ADDRESS_11H_aya_bbb,
	CARD_ADDRESS_11L_aya_bbb,
	CARD_ADDRESS_12E_aya_ccc,
	CARD_ADDRESS_12N_aya_ccc,
	CARD_ADDRESS_12H_aya_ccc,
	CARD_ADDRESS_12L_aya_ccc,
	CARD_ADDRESS_13E_aya_ddd,
	CARD_ADDRESS_13N_aya_ddd,
	CARD_ADDRESS_13H_aya_ddd,
	CARD_ADDRESS_13L_aya_ddd,
	CARD_ADDRESS_14E_aya_eee,
	CARD_ADDRESS_14N_aya_eee,
	CARD_ADDRESS_14H_aya_eee,
	CARD_ADDRESS_14L_aya_eee,
	CARD_ADDRESS_15E_aya_fff,
	CARD_ADDRESS_15N_aya_fff,
	CARD_ADDRESS_15H_aya_fff,
	CARD_ADDRESS_15L_aya_fff,
	CARD_ADDRESS_16E_aya_ggg,
	CARD_ADDRESS_16N_aya_ggg,
	CARD_ADDRESS_16H_aya_ggg,
	CARD_ADDRESS_16L_aya_ggg,
	CARD_ADDRESS_17E_aya_hhh,
	CARD_ADDRESS_17N_aya_hhh,
	CARD_ADDRESS_17H_aya_hhh,
	CARD_ADDRESS_17L_aya_hhh,
//	CARD_ADDRESS_18E_aya_iii,
//	CARD_ADDRESS_18N_aya_iii,
//	CARD_ADDRESS_18H_aya_iii,
//	CARD_ADDRESS_18L_aya_iii,
//	CARD_ADDRESS_19E_aya_jjj,
//	CARD_ADDRESS_19N_aya_jjj,
//	CARD_ADDRESS_19H_aya_jjj,
//	CARD_ADDRESS_19L_aya_jjj,
// 輝夜 3面
	CARD_ADDRESS_00E_kaguya_000,
	CARD_ADDRESS_00N_kaguya_000,
	CARD_ADDRESS_00H_kaguya_000,
	CARD_ADDRESS_00L_kaguya_000,
	CARD_ADDRESS_11E_kaguya_bbb,
	CARD_ADDRESS_11N_kaguya_bbb,
	CARD_ADDRESS_11H_kaguya_bbb,
	CARD_ADDRESS_11L_kaguya_bbb,
	CARD_ADDRESS_12E_kaguya_ccc,
	CARD_ADDRESS_12N_kaguya_ccc,
	CARD_ADDRESS_12H_kaguya_ccc,
	CARD_ADDRESS_12L_kaguya_ccc,
	CARD_ADDRESS_13E_kaguya_ddd,
	CARD_ADDRESS_13N_kaguya_ddd,
	CARD_ADDRESS_13H_kaguya_ddd,
	CARD_ADDRESS_13L_kaguya_ddd,
	CARD_ADDRESS_14E_kaguya_eee,
	CARD_ADDRESS_14N_kaguya_eee,
	CARD_ADDRESS_14H_kaguya_eee,
	CARD_ADDRESS_14L_kaguya_eee,
	CARD_ADDRESS_15E_kaguya_fff,
	CARD_ADDRESS_15N_kaguya_fff,
	CARD_ADDRESS_15H_kaguya_fff,
	CARD_ADDRESS_15L_kaguya_fff,
	CARD_ADDRESS_16E_kaguya_ggg,
	CARD_ADDRESS_16N_kaguya_ggg,
	CARD_ADDRESS_16H_kaguya_ggg,
	CARD_ADDRESS_16L_kaguya_ggg,
	CARD_ADDRESS_17E_kaguya_hhh,
	CARD_ADDRESS_17N_kaguya_hhh,
	CARD_ADDRESS_17H_kaguya_hhh,
	CARD_ADDRESS_17L_kaguya_hhh,
// 未定 2面
	CARD_ADDRESS_00E_mima_000,
	CARD_ADDRESS_00N_mima_000,
	CARD_ADDRESS_00H_mima_000,
	CARD_ADDRESS_00L_mima_000,
	CARD_ADDRESS_11E_mima_bbb,
	CARD_ADDRESS_11N_mima_bbb,
	CARD_ADDRESS_11H_mima_bbb,
	CARD_ADDRESS_11L_mima_bbb,
	CARD_ADDRESS_12E_mima_ccc,
	CARD_ADDRESS_12N_mima_ccc,
	CARD_ADDRESS_12H_mima_ccc,
	CARD_ADDRESS_12L_mima_ccc,
	CARD_ADDRESS_13E_mima_ddd,
	CARD_ADDRESS_13N_mima_ddd,
	CARD_ADDRESS_13H_mima_ddd,
	CARD_ADDRESS_13L_mima_ddd,
	CARD_ADDRESS_14E_mima_eee,
	CARD_ADDRESS_14N_mima_eee,
	CARD_ADDRESS_14H_mima_eee,
	CARD_ADDRESS_14L_mima_eee,
	CARD_ADDRESS_15E_mima_fff,
	CARD_ADDRESS_15N_mima_fff,
	CARD_ADDRESS_15H_mima_fff,
	CARD_ADDRESS_15L_mima_fff,
	CARD_ADDRESS_16E_mima_ggg,
	CARD_ADDRESS_16N_mima_ggg,
	CARD_ADDRESS_16H_mima_ggg,
	CARD_ADDRESS_16L_mima_ggg,
	CARD_ADDRESS_17E_mima_hhh,
	CARD_ADDRESS_17N_mima_hhh,
	CARD_ADDRESS_17H_mima_hhh,
	CARD_ADDRESS_17L_mima_hhh,
// アリス 1面
	CARD_ADDRESS_00E_alice_000,
	CARD_ADDRESS_00N_alice_000,
	CARD_ADDRESS_00H_alice_000,
	CARD_ADDRESS_00L_alice_000,
	CARD_ADDRESS_11E_alice_bbb,
	CARD_ADDRESS_11N_alice_bbb,
	CARD_ADDRESS_11H_alice_bbb,
	CARD_ADDRESS_11L_alice_bbb,
	CARD_ADDRESS_12E_alice_ccc,
	CARD_ADDRESS_12N_alice_ccc,
	CARD_ADDRESS_12H_alice_ccc,
	CARD_ADDRESS_12L_alice_ccc,
	CARD_ADDRESS_13E_alice_ddd,
	CARD_ADDRESS_13N_alice_ddd,
	CARD_ADDRESS_13H_alice_ddd,
	CARD_ADDRESS_13L_alice_ddd,
	CARD_ADDRESS_14E_alice_eee,
	CARD_ADDRESS_14N_alice_eee,
	CARD_ADDRESS_14H_alice_eee,
	CARD_ADDRESS_14L_alice_eee,
	CARD_ADDRESS_15E_alice_fff,
	CARD_ADDRESS_15N_alice_fff,
	CARD_ADDRESS_15H_alice_fff,
	CARD_ADDRESS_15L_alice_fff,
	#if (1)/*(r35無いとハングアップする。)*/
	CARD_ADDRESS_16E_alice_ggg,
	CARD_ADDRESS_16N_alice_ggg,
	CARD_ADDRESS_16H_alice_ggg,
	CARD_ADDRESS_16L_alice_ggg,
	#endif
//	CARD_ADDRESS_17E_alice_hhh,
//	CARD_ADDRESS_17N_alice_hhh,
//	CARD_ADDRESS_17H_alice_hhh,
//	CARD_ADDRESS_17L_alice_hhh,
//	CARD_ADDRESS_18E_alice_iii,
//	CARD_ADDRESS_18N_alice_iii,
//	CARD_ADDRESS_18H_alice_iii,
//	CARD_ADDRESS_18L_alice_iii,
//	CARD_ADDRESS_19E_alice_jjj,
//	CARD_ADDRESS_19N_alice_jjj,
//	CARD_ADDRESS_19H_alice_jjj,
//	CARD_ADDRESS_19L_alice_jjj,
//
	CARD_ADDRESS_MAX	/* 最大数 */
};

#endif /* _CARD_ADDRESS_VALUE_H_ */

