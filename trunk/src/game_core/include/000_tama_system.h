
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _CARD_CPU_H_
#define _CARD_CPU_H_
/*---------------------------------------------------------
	カードCPU
---------------------------------------------------------*/

/*---------------------------------------------------------
	カードCPU用のレジスタ(ローカルメモリー)
	-------------------------------------------------------
	将来的に「カード」は、全て「カードスクリプト」で処理する用に移行する(予定)。
	-------------------------------------------------------
	この「カードスクリプト」は、「カードCPU
	(カードスクリプト中央処理ユニット)
	(SPEll-CArd script, Central Prcessing Unit.)」
	が、実際の処理を行う。
	-------------------------------------------------------
	このカードCPUには、レジスタ(Resister)と呼ばれるカードCPUの内部メモリがあり。
	カードCPUを実行させる場合(つまりスペルカードを撃つ場合)に、カードCPUはレジスタ以外アクセスできない。
	-------------------------------------------------------
	仕様:全レジスタ(reg00-reg1f)は、カードスクリプト開始時に必ず(0)で消去される。
	その後、カードスクリプト用の[初期化セクション]で指示された初期化処理が一度だけ行われる。
---------------------------------------------------------*/

enum
{
	// レジスタ
	REG_NUM_00_SRC_X = (0), 		// 0受け渡しレジスタ。
	REG_NUM_01_SRC_Y,				// 1受け渡しレジスタ。
	REG_NUM_02_DEST_X,				// 2受け渡しレジスタ。
	REG_NUM_03_DEST_Y,				// 3受け渡しレジスタ。
	REG_NUM_04_CONDITION_CODE0, 	// CC0レジスタ。IFやLOOP等で暗黙に利用。
	REG_NUM_05_CONDITION_CODE1, 	// CC1レジスタ。IFやLOOP等で暗黙に利用。
	REG_NUM_06_CONDITION_CODE2, 	// CC2レジスタ。IFやLOOP等で暗黙に利用。
	REG_NUM_07_CONDITION_CODE3, 	// CC3レジスタ。IFやLOOP等で暗黙に利用。
	REG_NUM_08_REG0,				// 汎用レジスタ0。 カウンタレジスタ0。カウンタ値1を保持。
	REG_NUM_09_REG1,				// 汎用レジスタ1。 カウンタレジスタ1。カウンタ値2を保持。
	REG_NUM_0a_REG2,				// 汎用レジスタ2。 カウンタレジスタ2。カウンタ値3を保持。
	REG_NUM_0b_REG3,				// 汎用レジスタ3。 カウンタレジスタ3。カウンタ値4を保持。
	REG_NUM_0c_REG4,				// 汎用レジスタ4。
	REG_NUM_0d_REG5,				// 汎用レジスタ5。
	REG_NUM_0e_REG6,				// 汎用レジスタ6。
	REG_NUM_0f_REG7,				// 汎用レジスタ7。(難易度レジスタ)
	// I/Oポート
	REG_NUM_10_BOSS_SPELL_TIMER,	// スペル経過時間用レジスタ。
	REG_NUM_11_TAMA1,				// 発弾用レジスタ1。tama_system に接続。HATSUDAN_01_speed256
	REG_NUM_12_TAMA2,				// 発弾用レジスタ2。tama_system に接続。HATSUDAN_02_speed_offset
	REG_NUM_13_TAMA3,				// 発弾用レジスタ3。tama_system に接続。HATSUDAN_03_angle65536
	REG_NUM_14_TAMA4,				// 発弾用レジスタ4。tama_system に接続。HATSUDAN_04_tama_spec
	REG_NUM_15_TAMA5,				// 発弾用レジスタ5。tama_system に接続。HATSUDAN_05_bullet_obj_type
	REG_NUM_16_TAMA6,				// 発弾用レジスタ6。tama_system に接続。HATSUDAN_06_n_way
	REG_NUM_17_TAMA7,				// 発弾用レジスタ7。tama_system に接続。HATSUDAN_07_div_angle65536
	REG_NUM_18_SOUND_REG0,			// 効果音用レジスタ0。
	REG_NUM_19_SOUND_REG1,			// 効果音用レジスタ1。
	REG_NUM_1a_SOUND_REG2,			// 効果音用レジスタ2。
	REG_NUM_1b_SOUND_REG3,			// 効果音用レジスタ3。
	REG_NUM_1c_SOUND_NUM0,			// 効果音番号保持用レジスタ0。
	REG_NUM_1d_SOUND_NUM1,			// 効果音番号保持用レジスタ1。
	REG_NUM_1e_SOUND_NUM2,			// 効果音番号保持用レジスタ2。
	REG_NUM_1f_SOUND_NUM3,			// 効果音番号保持用レジスタ3。
	#if 0
	// メモリ(レジスタ退避用領域)
	MEM_NUM_20_ 					// メモリ。
	MEM_NUM_21_ 					// メモリ。
	MEM_NUM_22_ 					// メモリ。
	MEM_NUM_23_ 					// メモリ。
	MEM_NUM_24_ 					// メモリ。
	MEM_NUM_25_ 					// メモリ。
	MEM_NUM_26_ 					// メモリ。
	MEM_NUM_27_ 					// メモリ。
	MEM_NUM_28_ 					// メモリ。
	MEM_NUM_29_ 					// メモリ。
	MEM_NUM_2a_ 					// メモリ。
	MEM_NUM_2b_ 					// メモリ。
	MEM_NUM_2c_ 					// メモリ。
	MEM_NUM_2d_ 					// メモリ。
	MEM_NUM_2e_ 					// メモリ。
	MEM_NUM_2f_ 					// メモリ。
	// 拡張メモリ
	MEM_NUM_30_ 					// 拡張メモリ。
	MEM_NUM_31_ 					// 拡張メモリ。
	MEM_NUM_32_ 					// 拡張メモリ。
	MEM_NUM_33_ 					// 拡張メモリ。
	MEM_NUM_34_ 					// 拡張メモリ。
	MEM_NUM_35_ 					// 拡張メモリ。
	MEM_NUM_36_ 					// 拡張メモリ。
	MEM_NUM_37_ 					// 拡張メモリ。
	MEM_NUM_38_ 					// 拡張メモリ。
	MEM_NUM_39_ 					// 拡張メモリ。
	MEM_NUM_3a_ 					// 拡張メモリ。
	MEM_NUM_3b_ 					// 拡張メモリ。
	MEM_NUM_3c_ 					// 拡張メモリ。
	MEM_NUM_3d_ 					// 拡張メモリ。
	MEM_NUM_3e_ 					// 拡張メモリ。
	MEM_NUM_3f_ 					// 拡張メモリ。
	#endif
	REG_NUM_99_MAX// = 0x20/*(.align 32)*/
};
//#define MAX_FIX_VALUE (REG_NUM_01_FIX_VALUE+1)/*固定値の範囲*/
//カードスクリプト実行中には毎フレーム、ボスの現在座標が更新される。
/*static*/extern int spell_register[REG_NUM_99_MAX];

/* 実際のレジスタ */

#define REG_00_SRC_X				spell_register[REG_NUM_00_SRC_X]
#define REG_01_SRC_Y				spell_register[REG_NUM_01_SRC_Y]
#define REG_02_DEST_X				spell_register[REG_NUM_02_DEST_X]
#define REG_03_DEST_Y				spell_register[REG_NUM_03_DEST_Y]
//
#define REG_08_REG0 				spell_register[REG_NUM_08_REG0]
#define REG_09_REG1 				spell_register[REG_NUM_09_REG1]
#define REG_0a_REG2 				spell_register[REG_NUM_0a_REG2]
#define REG_0b_REG3 				spell_register[REG_NUM_0b_REG3]
#define REG_0c_REG4 				spell_register[REG_NUM_0c_REG4]
#define REG_0d_REG5 				spell_register[REG_NUM_0d_REG5]
#define REG_0e_REG6 				spell_register[REG_NUM_0e_REG6]
#define REG_0f_REG7 				spell_register[REG_NUM_0f_REG7]
//
#define REG_10_BOSS_SPELL_TIMER 	spell_register[REG_NUM_10_BOSS_SPELL_TIMER] 	/* スペル経過時間 */
/*---------------------------------------------------------
	難易度レジスタ。
	仕様が決まっていないので、取り敢えず割り当てる。
	割り当て位置は変わる可能性が高い。
	-------------------------------------------------------
	このレジスタは、カード初期化時に難易度が入る。
---------------------------------------------------------*/
#define REG_0f_GAME_DIFFICULTY		REG_0f_REG7

#endif /* _CARD_CPU_H_ */

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#ifndef _HATSUDAN_SYSTEM_H_
#define _HATSUDAN_SYSTEM_H_
/*---------------------------------------------------------
	発弾システム
	-------------------------------------------------------
	どんな弾でも発弾する場合は必ず「発弾システム」を通らなければならない。
	これは、一ヶ所に無いと極端に速度低下するのが主な理由。
	-------------------------------------------------------
	発弾システムは主に発弾エフェクトの指定をするが、
	発弾エフェクトの無い弾でも当然発弾システムを通らなければならない。
	-------------------------------------------------------
	発弾エフェクトは現状発弾システムが担当しているが、
	将来的にもっと効率的な別システムになる。
	「弾幕」に特化して「弾(個弾)」の(移動)処理が出来なくなる。
	これは「弾幕」シューティングの場合、個弾の移動処理は必要無い為、
	個弾の移動処理は無い。あると遅い。(r35現在旧互換であるので遅い)
---------------------------------------------------------*/

/*---------------------------------------------------------
	発弾単位の受け渡し設定(角度弾の場合)
	-------------------------------------------------------
	角度弾の分解能は65536度に決定。
	-------------------------------------------------------
	将来カードスクリプトシステムに移行した場合、
	この構造体は、カードスクリプト用のレジスタ(ローカルメモリー)に
	移動する。(でないとカードスクリプト上から弾をどうこうできないので)
---------------------------------------------------------*/

	// 以下は、 弾の設定。
#define HATSUDAN_01_speed256			spell_register[REG_NUM_11_TAMA1]	/* 速度 */
#define HATSUDAN_02_speed_offset		spell_register[REG_NUM_12_TAMA2]	/* 加速度 */

#define HATSUDAN_03_angle65536			spell_register[REG_NUM_13_TAMA3]	/* 角度 */					/* <分解能65536度に決定> */
#define HATSUDAN_04_tama_spec			spell_register[REG_NUM_14_TAMA4]	/* 弾の能力属性 */

#define HATSUDAN_05_bullet_obj_type 	spell_register[REG_NUM_15_TAMA5]	/* 弾グラフィックの種類 */
	// 以下は、 n_way弾の場合に必要な要素。
#define HATSUDAN_06_n_way				spell_register[REG_NUM_16_TAMA6]	/* n way 弾の弾数 */
#define HATSUDAN_07_div_angle65536		spell_register[REG_NUM_17_TAMA7]	/* n way 弾の分割角度 */	/* <分解能65536度に決定> */

#if 1
	/* 同じ物だけど、 65536分割なのか 1024 分割なのか明示する為に必要。 */
	#define HATSUDAN_03_angle1024		HATSUDAN_03_angle65536	/* 描画用角度 */	/* <描画用の分解能1024度に落としてある事を明示する。> */
#endif


/*---------------------------------------------------------
	登録種類(角度弾の場合)
---------------------------------------------------------*/

extern void hatudan_system_regist_single(void); 			/* 単発 */
extern void hatudan_system_regist_katayori_n_way(void); 	/* 偏り n way弾 */
extern void hatudan_system_regist_n_way(void);				/* 通常 n way弾 */


/*---------------------------------------------------------
	角度弾規格(策定案、仮運用中。)
	基点座標関連は特殊機能に振って変更するかも知れない。
---------------------------------------------------------*/

	#define hatudan_register_speed65536 		user_data00 	/* 加減速 */	//	#define hatudan_system_speed256 user_data02 	/* 加減速 */
	#define hatudan_register_tra65536			user_data01 	/* 加減速調整 */
	#define hatudan_register_spec_data			user_data02 	/* 画面外消去判定や反射機能 */
	#define hatudan_register_flame_counter		user_data03 	/* 発弾フレームカウンタ(正値で発弾) */
	#define hatudan_register_user_data10		user_data04 	/* (r35) */
//	#define hatudan_system_bbb					user_data05 	/* 拡張予定(?) (r33現在未使用) / */
//	#define hatudan_system_ccc					user_data06 	/* 拡張予定(?) (r33現在未使用) / */

#define hatudan_register_2nd_angle65536 		hatudan_register_user_data10	/*(2回目に分裂する角度を保持)*/

/*---------------------------------------------------------
	弾スペック
	弾の能力、状態をあらわす。
	描画する場合のグラフィック、傾き弾。非傾き弾。の区別。
	弾幕システムのどのレイヤーに所属する弾かの区別。
---------------------------------------------------------*/

#if 1
	#define TAMA_SPEC_0000_TILT 				(0x0000)/* 傾き弾(通常弾) */
	#define TAMA_SPEC_8000_NON_TILT 			(0x8000)/* 非傾き弾 */		/* 傾かない弾(大玉弾、チルノ弾、等用) */
//	#define TAMA_SPEC_4000_NON_MOVE 			(0x4000)/* 移動処理なし */
	#define TAMA_SPEC_4000_NON_MOVE 			(0x0000)/* 移動処理なし(r35標準) */
	#define TAMA_SPEC_3000_EFFECT_MASK			(0x3000)/* エフェクト選択 */
	#define TAMA_SPEC_3000_EFFECT_NONE			(0x3000)/* エフェクトなし */
	#define TAMA_SPEC_2000_EFFECT_MINI			(0x2000)/* エフェクト小 */
	#define TAMA_SPEC_1000_EFFECT_MIDDLE		(0x1000)/* エフェクト中 */
	#define TAMA_SPEC_0000_EFFECT_LARGE 		(0x0000)/* エフェクト大(旧r34互換) */
#endif
	/*(CCW)*/
	#define TAMA_SPEC_0x100_CCW_BIT 			(0x0100)
	/*(美鈴の曲がるクナイ連弾)*/
	#define TAMA_SPEC_AKA_AO_KUNAI_BIT			(TAMA_SPEC_0x100_CCW_BIT)
	/*(パチェのマキュリーポイズン)*/
	#define TAMA_SPEC_AO_MIZU_MERCURY_BIT		(TAMA_SPEC_0x100_CCW_BIT)
	/*(咲夜チルノ左右の壁で1度だけ反射)*/
	#define TAMA_SPEC_KABE_SAYUU_HANSYA_BIT 	(TAMA_SPEC_0x100_CCW_BIT)
	/*(レミリア、呪詛「ブラド・ツェペシュの呪い」、回転方向フラグ)*/
	/*(ルーミア、闇符「ディマーケイション」、回転方向フラグ)*/
	#define TAMA_SPEC_KAITEN_HOUKOU_BIT 		(TAMA_SPEC_0x100_CCW_BIT)
//	#define TAMA_SPEC_KAITEN_HOUKOU_CW			(0x0000)
//	#define TAMA_SPEC_KAITEN_HOUKOU_CCW 		(TAMA_SPEC_0x100_CCW_BIT)


#endif /* _HATSUDAN_SYSTEM_H_ */


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

/*---------------------------------------------------------
	レーザーシステム
---------------------------------------------------------*/

#ifndef _LASER_SYSTEM_H_
#define _LASER_SYSTEM_H_

/*
	パチェ「通常攻撃」
	フラン禁忌「レーヴァテイン」
	フラン禁弾「過去を刻む時計」
	これらのカードを作るかどうかは別問題として、これらのレーザー弾に
	システムで対応できるように設計しとく必要がある。
*/
enum
{
	LASER_TYPE_00_OFF		= 0,//	LASER_TYPE_00_1_PACHE	= 0,	// 0==off, lz[0]==offの場合、特殊機能で全部off。
	LASER_TYPE_01_4_PACHE,	/* 4本、外側タイプ(パチェ「通常攻撃」用) */
	LASER_TYPE_02_1_FRAN,	/* 1本、タイプ	  (フラン禁忌「レーヴァテイン」用) */
	LASER_TYPE_03_4_FRAN,	/* 4本、内側タイプ(フラン禁弾「過去を刻む時計」用) */
	MAX_99_LASER_TYPE
};
enum
{
	LASER_00_ = 0,
	LASER_01_,
	MAX_99_LASER
	/*	2弾？==禁忌「レーヴァテイン」(「始め」と「最後か真中あたり」のみ判定があれば、残像全部に判定がなくても大して変わらない気がする)
		8弾？==禁忌「レーヴァテイン」
		最後でなくて、真中あたりにするのは難易度を調整する為。(最後の方が若干難しい)
		物が剣なので、実際は「ぎりぎりで当たってる」のだけど、その場合あたってない事にした方が「ぎりぎりで当たらなかった」という奴。
		２本要る理由は、高速移動キャラで、1本目ですり抜けても、２本目に当たる。
	 */
};
typedef struct _laser_
{
	int laser_type; 		/* レーザーの種類 / 又はoff指定. LASER_TYPE */
	int laser_angle65536;	/* レーザーの角度 */
} LASER;
extern LASER lz[MAX_99_LASER];
/* lz[0]==0;の場合、すべてoff。(レーザーモード 0:off, 1:on) */

#endif /* _LASER_SYSTEM_H_ */
