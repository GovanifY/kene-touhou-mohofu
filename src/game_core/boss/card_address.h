
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
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
	CARD_ADDRESS_06E_chrno_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_06N_chrno_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_06H_chrno_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_06L_chrno_gekiha,	/*(r36強制的に撃破)*/
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
	CARD_ADDRESS_03E_sakuya_ccc,	/* 第三形態: 螺旋赤ナイフ  */
	CARD_ADDRESS_03N_sakuya_ccc,	/* 第三形態: 螺旋赤ナイフ  */
	CARD_ADDRESS_03H_sakuya_ccc,	/* 第三形態: 螺旋赤ナイフ  */
	CARD_ADDRESS_03L_sakuya_ccc,	/* 第三形態: 螺旋赤ナイフ  */
	CARD_ADDRESS_04E_sakuya_ddd,	/* 第四形態:  */
	CARD_ADDRESS_04N_sakuya_ddd,	/* 第四形態:  */
	CARD_ADDRESS_04H_sakuya_ddd,	/* 第四形態:  */
	CARD_ADDRESS_04L_sakuya_ddd,	/* 第四形態:  */
	CARD_ADDRESS_05E_sakuya_eee,	/* 第五形態:  */
	CARD_ADDRESS_05N_sakuya_eee,	/* 第五形態:  */
	CARD_ADDRESS_05H_sakuya_eee,	/* 第五形態:  */
	CARD_ADDRESS_05L_sakuya_eee,	/* 第五形態:  */
	CARD_ADDRESS_06E_sakuya_fff,	/* 第六形態:  */
	CARD_ADDRESS_06N_sakuya_fff,	/* 第六形態:  */
	CARD_ADDRESS_06H_sakuya_fff,	/* 第六形態:  */
	CARD_ADDRESS_06L_sakuya_fff,	/* 第六形態:  */
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
	CARD_ADDRESS_12E_sakuya_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_12N_sakuya_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_12H_sakuya_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_12L_sakuya_gekiha, /*(r36強制的に撃破)*/
// パチェA 5面
	CARD_ADDRESS_00E_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_00N_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_00H_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_00L_pacheA_000,	/* "形態変更" */
	CARD_ADDRESS_01E_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_01N_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_01H_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_01L_pacheA_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_02E_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_02N_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_02H_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_02L_pacheA_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_03E_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_03N_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_03H_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_03L_pacheA_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_04E_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_04N_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_04H_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_04L_pacheA_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_05E_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_05N_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_05H_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_05L_pacheA_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_06E_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_06N_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_06H_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_06L_pacheA_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_07E_pacheA_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_07N_pacheA_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_07H_pacheA_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_07L_pacheA_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_08E_pacheA_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_08N_pacheA_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_08H_pacheA_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_08L_pacheA_gekiha, /*(r36強制的に撃破)*/
// パチェB 5面
	CARD_ADDRESS_10E_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_10N_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_10H_pacheB_000,	/* "形態変更" */
	CARD_ADDRESS_10L_pacheB_000,	/* "形態変更" */
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
	CARD_ADDRESS_17E_pacheB_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_17N_pacheB_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_17H_pacheB_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_17L_pacheB_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_18E_pacheB_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_18N_pacheB_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_18H_pacheB_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_18L_pacheB_gekiha, /*(r36強制的に撃破)*/
// パチェC 5面
	CARD_ADDRESS_20E_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_20N_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_20H_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_20L_pacheC_000,	/* "形態変更" */
	CARD_ADDRESS_21E_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_21N_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_21H_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_21L_pacheC_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_22E_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_22N_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_22H_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_22L_pacheC_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_23E_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_23N_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_23H_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_23L_pacheC_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_24E_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_24N_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_24H_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_24L_pacheC_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_25E_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_25N_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_25H_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_25L_pacheC_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_26E_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_26N_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_26H_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_26L_pacheC_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_27E_pacheC_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_27N_pacheC_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_27H_pacheC_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_27L_pacheC_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_28E_pacheC_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_28N_pacheC_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_28H_pacheC_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_28L_pacheC_gekiha, /*(r36強制的に撃破)*/
// パチェD 5面
	CARD_ADDRESS_30E_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_30N_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_30H_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_30L_pacheD_000,	/* "形態変更" */
	CARD_ADDRESS_31E_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_31N_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_31H_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_31L_pacheD_bbb,	/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_32E_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_32N_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_32H_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_32L_pacheD_ccc,	/* 第二形態: カードⅠ */
	CARD_ADDRESS_33E_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_33N_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_33H_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_33L_pacheD_ddd,	/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_34E_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_34N_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_34H_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_34L_pacheD_eee,	/* 第四形態: カードⅡ */
	CARD_ADDRESS_35E_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_35N_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_35H_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_35L_pacheD_fff,	/* 第五形態: カードⅢ */
	CARD_ADDRESS_36E_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_36N_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_36H_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_36L_pacheD_ggg,	/* 第六形態: カードⅣ */
	CARD_ADDRESS_37E_pacheD_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_37N_pacheD_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_37H_pacheD_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_37L_pacheD_hhh,	/* 第七形態: カードⅤ */
	CARD_ADDRESS_38E_pacheD_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_38N_pacheD_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_38H_pacheD_gekiha, /*(r36強制的に撃破)*/
	CARD_ADDRESS_38L_pacheD_gekiha, /*(r36強制的に撃破)*/
// 文 4面
	CARD_ADDRESS_00E_aya_000,		/* "形態変更" */
	CARD_ADDRESS_00N_aya_000,		/* "形態変更" */
	CARD_ADDRESS_00H_aya_000,		/* "形態変更" */
	CARD_ADDRESS_00L_aya_000,		/* "形態変更" */
	CARD_ADDRESS_01E_aya_bbb,		/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_01N_aya_bbb,		/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_01H_aya_bbb,		/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_01L_aya_bbb,		/* 第一形態: 通常攻撃1 */
	CARD_ADDRESS_02E_aya_ccc,		/* 第二形態: カードⅠ */
	CARD_ADDRESS_02N_aya_ccc,		/* 第二形態: カードⅠ */
	CARD_ADDRESS_02H_aya_ccc,		/* 第二形態: カードⅠ */
	CARD_ADDRESS_02L_aya_ccc,		/* 第二形態: カードⅠ */
	CARD_ADDRESS_03E_aya_ddd,		/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_03N_aya_ddd,		/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_03H_aya_ddd,		/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_03L_aya_ddd,		/* 第三形態: 通常攻撃2 */
	CARD_ADDRESS_04E_aya_eee,		/* 第四形態: カードⅡ */
	CARD_ADDRESS_04N_aya_eee,		/* 第四形態: カードⅡ */
	CARD_ADDRESS_04H_aya_eee,		/* 第四形態: カードⅡ */
	CARD_ADDRESS_04L_aya_eee,		/* 第四形態: カードⅡ */
	CARD_ADDRESS_05E_aya_fff,		/* 第五形態: カードⅢ */
	CARD_ADDRESS_05N_aya_fff,		/* 第五形態: カードⅢ */
	CARD_ADDRESS_05H_aya_fff,		/* 第五形態: カードⅢ */
	CARD_ADDRESS_05L_aya_fff,		/* 第五形態: カードⅢ */
	CARD_ADDRESS_06E_aya_ggg,		/* 第六形態: カードⅣ */
	CARD_ADDRESS_06N_aya_ggg,		/* 第六形態: カードⅣ */
	CARD_ADDRESS_06H_aya_ggg,		/* 第六形態: カードⅣ */
	CARD_ADDRESS_06L_aya_ggg,		/* 第六形態: カードⅣ */
	CARD_ADDRESS_07E_aya_hhh,		/* 第七形態: カードⅤ */
	CARD_ADDRESS_07N_aya_hhh,		/* 第七形態: カードⅤ */
	CARD_ADDRESS_07H_aya_hhh,		/* 第七形態: カードⅤ */
	CARD_ADDRESS_07L_aya_hhh,		/* 第七形態: カードⅤ */
	CARD_ADDRESS_08E_aya_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_08N_aya_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_08H_aya_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_08L_aya_gekiha,	/*(r36強制的に撃破)*/
// 輝夜 3面
	CARD_ADDRESS_00E_kaguya_000,
	CARD_ADDRESS_00N_kaguya_000,
	CARD_ADDRESS_00H_kaguya_000,
	CARD_ADDRESS_00L_kaguya_000,
	CARD_ADDRESS_51E_kaguya_bbb,
	CARD_ADDRESS_51N_kaguya_bbb,
	CARD_ADDRESS_51H_kaguya_bbb,
	CARD_ADDRESS_51L_kaguya_bbb,
	CARD_ADDRESS_52E_kaguya_ccc,
	CARD_ADDRESS_52N_kaguya_ccc,
	CARD_ADDRESS_52H_kaguya_ccc,
	CARD_ADDRESS_52L_kaguya_ccc,
	CARD_ADDRESS_53E_kaguya_ddd,
	CARD_ADDRESS_53N_kaguya_ddd,
	CARD_ADDRESS_53H_kaguya_ddd,
	CARD_ADDRESS_53L_kaguya_ddd,
	CARD_ADDRESS_54E_kaguya_eee,
	CARD_ADDRESS_54N_kaguya_eee,
	CARD_ADDRESS_54H_kaguya_eee,
	CARD_ADDRESS_54L_kaguya_eee,
	CARD_ADDRESS_55E_kaguya_fff,
	CARD_ADDRESS_55N_kaguya_fff,
	CARD_ADDRESS_55H_kaguya_fff,
	CARD_ADDRESS_55L_kaguya_fff,
	CARD_ADDRESS_56E_kaguya_ggg,
	CARD_ADDRESS_56N_kaguya_ggg,
	CARD_ADDRESS_56H_kaguya_ggg,
	CARD_ADDRESS_56L_kaguya_ggg,
	CARD_ADDRESS_57E_kaguya_hhh,
	CARD_ADDRESS_57N_kaguya_hhh,
	CARD_ADDRESS_57H_kaguya_hhh,
	CARD_ADDRESS_57L_kaguya_hhh,
	CARD_ADDRESS_58E_kaguya_gekiha, 	/*(r36強制的に撃破)*/
	CARD_ADDRESS_58N_kaguya_gekiha, 	/*(r36強制的に撃破)*/
	CARD_ADDRESS_58H_kaguya_gekiha, 	/*(r36強制的に撃破)*/
	CARD_ADDRESS_58L_kaguya_gekiha, 	/*(r36強制的に撃破)*/
// 未定 2面
	CARD_ADDRESS_00E_mima_000,
	CARD_ADDRESS_00N_mima_000,
	CARD_ADDRESS_00H_mima_000,
	CARD_ADDRESS_00L_mima_000,
	CARD_ADDRESS_61E_mima_bbb,
	CARD_ADDRESS_61N_mima_bbb,
	CARD_ADDRESS_61H_mima_bbb,
	CARD_ADDRESS_61L_mima_bbb,
	CARD_ADDRESS_62E_mima_ccc,
	CARD_ADDRESS_62N_mima_ccc,
	CARD_ADDRESS_62H_mima_ccc,
	CARD_ADDRESS_62L_mima_ccc,
	CARD_ADDRESS_63E_mima_ddd,
	CARD_ADDRESS_63N_mima_ddd,
	CARD_ADDRESS_63H_mima_ddd,
	CARD_ADDRESS_63L_mima_ddd,
	CARD_ADDRESS_64E_mima_eee,
	CARD_ADDRESS_64N_mima_eee,
	CARD_ADDRESS_64H_mima_eee,
	CARD_ADDRESS_64L_mima_eee,
	CARD_ADDRESS_65E_mima_fff,
	CARD_ADDRESS_65N_mima_fff,
	CARD_ADDRESS_65H_mima_fff,
	CARD_ADDRESS_65L_mima_fff,
	CARD_ADDRESS_66E_mima_ggg,
	CARD_ADDRESS_66N_mima_ggg,
	CARD_ADDRESS_66H_mima_ggg,
	CARD_ADDRESS_66L_mima_ggg,
	CARD_ADDRESS_67E_mima_hhh,
	CARD_ADDRESS_67N_mima_hhh,
	CARD_ADDRESS_67H_mima_hhh,
	CARD_ADDRESS_67L_mima_hhh,
	CARD_ADDRESS_68E_mima_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_68N_mima_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_68H_mima_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_68L_mima_gekiha,	/*(r36強制的に撃破)*/
// アリス 1面
	CARD_ADDRESS_00E_alice_000,
	CARD_ADDRESS_00N_alice_000,
	CARD_ADDRESS_00H_alice_000,
	CARD_ADDRESS_00L_alice_000,
	CARD_ADDRESS_71E_alice_bbb,
	CARD_ADDRESS_71N_alice_bbb,
	CARD_ADDRESS_71H_alice_bbb,
	CARD_ADDRESS_71L_alice_bbb,
	CARD_ADDRESS_72E_alice_ccc,
	CARD_ADDRESS_72N_alice_ccc,
	CARD_ADDRESS_72H_alice_ccc,
	CARD_ADDRESS_72L_alice_ccc,
	CARD_ADDRESS_73E_alice_ddd,
	CARD_ADDRESS_73N_alice_ddd,
	CARD_ADDRESS_73H_alice_ddd,
	CARD_ADDRESS_73L_alice_ddd,
	CARD_ADDRESS_74E_alice_eee,
	CARD_ADDRESS_74N_alice_eee,
	CARD_ADDRESS_74H_alice_eee,
	CARD_ADDRESS_74L_alice_eee,
	CARD_ADDRESS_75E_alice_fff,
	CARD_ADDRESS_75N_alice_fff,
	CARD_ADDRESS_75H_alice_fff,
	CARD_ADDRESS_75L_alice_fff,
	CARD_ADDRESS_76E_alice_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_76N_alice_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_76H_alice_gekiha,	/*(r36強制的に撃破)*/
	CARD_ADDRESS_76L_alice_gekiha,	/*(r36強制的に撃破)*/
	//
	CARD_ADDRESS_MAX/* 最大数 */
};


/*
	スペルシステム==カードの管理システム。(カードシステムとは違うので注意)
*/


	/*---------------------------------------------------------
	スペルシステム(あらかじめカードの定義をしておくシステム)
	-------------------------------------------------------
	模倣風ではスペルはシステムでサポートしています。
	スペルとして定義(カード)しておくと、どのボスが、どのスペルでも撃てるようになります。
	-------------------------------------------------------
	またスペルはカードシステムに番号を登録しておくと、カードシステムでスペルが取得できます。
	(つまりカードシステム経由でスペルが撃てます)
	-------------------------------------------------------
	(r32)現在、雑魚(模倣風のdatの中-ボス)もスペルを撃てますが、
	この機能は将来なくなるかも知れません。
	雑魚(模倣風のdatの中-ボス)がスペルを撃てる機能は「風神禄」で思いついたのですが、
	こういう機能が無い方がゲームの速度が速く出来る気もするし、
	もしかしたらこういう機能は本家に無いのかもと思うこともあります。良く判りません。
	-------------------------------------------------------
	card.spell_used_number に撃ちたいスペルをセットすると、スペル生成開始。
	スペル生成が終了すると自動的に card.spell_used_number が DANMAKU_00 になる。
	-------------------------------------------------------
	スペル生成中は REG_10_BOSS_SPELL_TIMER が勝手に毎フレーム減算され、0 になるとスペル生成終了。
	この時間はスペルごとに違う標準時間が設定されている。
	これはカードの初期化内で自動初期化される。
	-------------------------------------------------------
	カードの初期化。
	カードが変わると毎回行う必要がある。
	---------------------------------------------------------*/

/* カード番号(各スペルを区別する為の番号) */
enum
{
	SPELL_00 = 0,						/* スペル生成終了フラグ。 */
//	/* 雑魚用 */
	SPELL_01_sakuya_misogi_normal,		/* [咲夜] 紅5面中-ボス 禊カード / 紅5面中-ボス 咲夜 「通常攻撃」 */
	SPELL_02_sakuya_old_test,			/* [咲夜] 紅5面中-ボス 24カード (奇術「ミスディレクションもどき(1/2)」) */
	SPELL_03_sakuya_old_test,			/* [咲夜] 紅5面中-ボス 11カード (奇術「ミスディレクションもどき(2/2)」) */
	SPELL_04_sakuya_pink_hearts,		/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1(1/2)」にちょっとだけ似たカード(予定) */
	SPELL_05_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃1/2(2/2)」にちょっとだけ似たカード(予定) */
	SPELL_06_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 幻象「ルナクロック(1/2)」にちょっとだけ似たカード(予定) */
	SPELL_07_sakuya,					/* [咲夜] 紅5面ボス [咲夜] 「通常攻撃3(1/2)」にちょっとだけ似たカード(予定) */
//
	SPELL_08_rumia, 					/* [ルーミア] 紅1面ボス [ルーミア] 夜符「ナイトバード」にちょっとだけ似たカード(予定) */
	SPELL_09_alice, 					/* [アリス] っぽい？テキトーカード */
	SPELL_0a_dai_yousei_01, 			/* [大妖精] 全周 48分割、時計回り、緑クナイ弾 */
	SPELL_0b_dai_yousei_02, 			/* [大妖精] 全周 48分割、反時計回り、赤クナイ弾 */
	SPELL_0c_sakuya_jack32, 			/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
	SPELL_0d_sakuya_miss_direction, 	/* [咲夜] 奇術「ミスディレクション」 */
	SPELL_0e_remilia_00,				/* [レミリア] テスト */
	SPELL_0f_tengu, 					/* [その他] 差分氏の妖怪1(天狗様?)が撃つカード(予定) */ 	/*???*/
//
	SPELL_10_cirno, 					/* [チルノ] 禊カード */
	SPELL_11_perfect_freeze,			/* [チルノ] パーフェクトフリーズ */
	SPELL_12_diamond_blizzard,			/* [チルノ] 雪符「ダイアモンドブリザード」 */
	SPELL_13_alice_suwako,				/* [アリス] 諏訪子っぽい(?)カード */
	SPELL_14_alice_youmu300,			/* [アリス] 妖3面ボス  normal通常攻撃3(の一部) */
	SPELL_15_alice_aka_2nd, 			/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
	SPELL_16_alice_zako_doll,				/* [アリス] 妖3面ボス [アリス]人形カード(もどき) */ // 40[sec]
	SPELL_17_alice_tama_doll_02,			/* [アリス] 作成中 */
//
	SPELL_18_hana_test, 				/* [魅魔] 花てすとカード */
	SPELL_40_SONOTA_DEBUG_CW_AO,		/* [その他] デバッグカードCW青 */
	SPELL_41_SONOTA_DEBUG_CCW_AKA,		/* [その他] デバッグカードCCW赤 */
	SPELL_1b_cirno_icecle_fall, 		/* [チルノ] 氷符「アイシクルフォール」 */
	SPELL_44_ruiz3_test,				/* [ルイズ] カード語のてすと。 */
	SPELL_1d_amefuri_test,				/* [] 雨てすと */
	SPELL_1e_momiji_no_nonoji,			/* [椛] ののじカード */
	SPELL_1f_koakuma,					/* [小悪魔] てすと用 */
//	/* ボス用 */
	SPELL_20_aya_misogi1,				/*	1[文] の通常弾っぽいカード撃たせてみるテスト1 */
	SPELL_21_aya_misogi2,				/*	2[文] の通常弾っぽいカード撃たせてみるテスト2 */
	SPELL_22_ame_no_yatimata,			/*	3[文] 岐符「天の八衢」 */
	SPELL_23_meirin_magaru_kunai,		/*	4[美鈴] 通常攻撃 */
	SPELL_24_aya_doll,					/*	5[文] 人形カード */ //40[sec]
	SPELL_25_houka_kenran,				/*	6[紅美鈴] 華符「芳華絢爛」にちょっとだけ似たカード(予定) */
	SPELL_26_aya_saifu, 				/*	7[文] 塞符 */
	SPELL_27_hosigata_test, 			/*	8 [その他] 星型テスト */
//
	SPELL_28,							/* 未定 */
	SPELL_29_rumia_demarcation, 		/* 10[ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似たカード(予定) */
	SPELL_2a_sakuya_baramaki1,			/* 11[咲夜] 紅5面中-ボス ばら撒き1 カード () */
	SPELL_2b_sakuya_baramaki2,			/* 12[咲夜] 紅5面中-ボス ばら撒き2 カード () */
	SPELL_2c_sakuya_blue_red,			/* 13[咲夜] 紅5面中-ボス 「通常攻撃3」青赤ナイフ */
	SPELL_2d_sakuya_misogi_lunatic, 	/* 14[咲夜] 紅5面中-ボス 禊カード / 紅5面中-ボス 咲夜 「通常攻撃」lunatic */
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
	SPELL_42_PACHE_LASER1,				/* [パチェ] 「通常攻撃1」のレーザーもどき */
	SPELL_43_PACHE_LASER2,				/* [パチェ] 「通常攻撃3」のレーザーもどき */
	SPELL_19_mima_sekkin,				/* [魅魔] 接近カード */
	SPELL_1a_mima_toge, 				/* [魅魔] トゲてすとカード */
	SPELL_1e_kaguya01,					/* [輝夜] むりやり変換1 */
	SPELL_1f_kaguya04,					/* [輝夜] むりやり変換2 */
	SPELL_1c_kakuya_tamanoe,			/* [輝夜]、蓬莱の玉の枝もどき。作成中 */
	SPELL_47_sakuya_festival_knife, 	/* [咲夜] 幻想「フェスティバルナイフ」(?) むりやり変換 */
//
	#if (1)
	SPELL_48_r34_gokan_kinou,			/* [r34]r34以前の互換機能。(r35-)システムで必ず必要。 */
	SPELL_49,							/* 未定 */
	SPELL_4a,							/* 未定 */
	SPELL_4b,							/* 未定 */
	SPELL_4c,							/* 未定 */
	SPELL_4d_remilia_rasen_knife,		/*	9[咲夜/レミリア] 紅6面ボス 赤ナイフ螺旋弾(予定) */
	SPELL_4e_remilia_vlad_tepes,		/*	9[咲夜/レミリア] 紅6面ボス 呪詛「ブラド・ツェペシュの呪い」*/
	SPELL_4f_remilia_red_magic, 		/*	9[咲夜/レミリア] 紅6面ボス 「レッドマジック」っぽい(?) */
	#endif
//
	SPELL_MAX,
};
//	SPELL_1f_silent_selena, 			/* [パチェ] サイレントセレナ の てすと用 */

#endif /* _CARD_ADDRESS_VALUE_H_ */
