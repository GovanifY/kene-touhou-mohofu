
#include "./../boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スペル生成システム(カードを生成)
	-------------------------------------------------------
	現状「ボス」以外にも「中ザコ」、「小ザコ」がスペル生成できるので、
	「スペル生成」と「カード管理」は別システムです。
	(現状「中-ボス」は作る予定があるが作ってない。カード履歴システムもまだ作ってない)
	-------------------------------------------------------
	分類		カード履歴		カード	スペル生成
	ボス		○				○				○	// ボス
	中-ボス 	×(?)			○				○	// <予定>			(雑魚の流れを「止める」)
	中ザコ		×				×				○	// るーみあ 		カードを撃てないのでカード名(符名)は無い。
	小ザコ		×				×				○	// 魔道書ザコ		カードを撃てないのでカード名(符名)は無い。
	ザコ		×				×				×	// その他のザコ 	スペル生成出来ない。
	-------------------------------------------------------
	分類			雑魚生成	カード
	ボス専用コア	×			○					// 雑魚生成関連の処理をしないのでその分速い。
	道中専用コア	○			×					// カード管理関連の処理をしないのでその分速い。
	-------------------------------------------------------
	(r33)上記の専用コア計画は廃止。
	今の方式(r33)だとむしろ専用コアにした方が速度低下する。
---------------------------------------------------------*/

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	レイヤーシステム
	-------------------------------------------------------
	レイヤーシステムは「弾幕」の文字通り、
	弾を単体としては認識せず、幕として認識するシステム。
	-------------------------------------------------------
---------------------------------------------------------*/

#include "./../boss/card_address.h"//#include "spell_address.h"

/*---------------------------------------------------------
	(r35)定数等
---------------------------------------------------------*/
//#define MAX_I_COUNT	(8192)/* (1/60)あたりの最大処理命令。 */

global int spell_register[REG_NUM_99_MAX];


/*---------------------------------------------------------
	(r35)カードスクリプト用命令(カードレジスタ全初期化)
---------------------------------------------------------*/

static CPU_FUNC(spell_cpu_initialize_all_register)
{
	unsigned int i;
	for (i=0; i<REG_NUM_99_MAX; i++)
	{
		spell_register[i] = (0);	/* 値(0)で初期化 */
	}
	//
	REG_0f_GAME_DIFFICULTY	= cg.game_difficulty;/* 難易度レジスタ。 */
}

#if (1)/*(将来的にカードインタプリタ上で動作する命令に移行する)*/
/*---------------------------------------------------------
	(r35)これらの関数は、レジスタではなく、レジスタ番号を渡すので注意の事。
	間違えてレジスタ渡した場合、間違い無くpspがハングアップする。
	間違いやすいので、関数名にNUMを入れた。
	-------------------------------------------------------
	(r35)これらの関数が、何故レジスタ値を渡さないかというと、
	カードスクリプト用命令に移行した場合、
	カードスクリプトではそもそもレジスタ値を渡せない。
	という仕様のスクリプトを予定している。
	理由はレジスタ値を渡すとスクリプトの実行速度が低下するから。
---------------------------------------------------------*/


#if (0)
	/*(開発中、デバッグ版、遅い)*/
	#define DEBUG_global global
#else
	/*(リリース版、とりあえず)*/
	#define DEBUG_global static
#endif
/*---------------------------------------------------------
	(r35)カードスクリプト用命令(カウンター)
---------------------------------------------------------*/
/*([global]このモジュールしか使わないが、スクリプト移行する為のものなので最適化防止の為。[static]だとインライン展開されてしまうのでまずい)*/
DEBUG_global/*static*/ void count_up_limit_NUM(int register_number_counter, int max_num)
{
	spell_register[register_number_counter]++;
	if (max_num <= (spell_register[register_number_counter]))
	{
		spell_register[register_number_counter] = (0);
	}
}
#endif

/*---------------------------------------------------------
	(r36)カードスクリプト用命令(sincos)
	-------------------------------------------------------
	使用レジスタ:
	REG_11		== HATSUDAN_01_speed256 	入力(速度、半径, t256形式)
	REG_13		== HATSUDAN_03_angle65536	入力(角度65536, d65536形式)
	REG_02: 	出力(cos(angle) x radius, t256形式)
	REG_03: 	出力(sin(angle) x radius, t256形式)
---------------------------------------------------------*/

global CPU_FUNC(sincos256)
{
	#if (0)//
	h->math_vector.x256  = si n1024((int_angle1024))*(/*p->speed*/(16*KETM075));/*fps_factor*/		/* CCWの場合 */
	h->math_vector.y256  = co s1024((int_angle1024))*(/*p->speed*/(16*KETM075));/*fps_factor*/
	REG_02_DEST_X	= (((int)(co s(de g1024tor ad(REG_10_ANGLE1024))*REG_11_SPEED256)));/*(fps_factor)*/
	REG_03_DEST_Y	= (((int)(si n(de g1024tor ad(REG_10_ANGLE1024))*REG_11_SPEED256)));/*(fps_factor)*/
	#else
	{
		int sin_value_t256; 		// sin_value_t256 = 0;
		int cos_value_t256; 		// cos_value_t256 = 0;
		int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);
		REG_02_DEST_X	 = (((cos_value_t256)*(HATSUDAN_01_speed256))>>8);/*fps_factor*/
		REG_03_DEST_Y	 = (((sin_value_t256)*(HATSUDAN_01_speed256))>>8);/*fps_factor*/
	}
	#endif
}
/*---------------------------------------------------------

---------------------------------------------------------*/

global OBJ_CALL_FUNC(set_REG_DEST_XY)
{
	REG_02_DEST_X	= (src->center.x256);	/* 弾源x256 ボス中心から発弾。 */
	REG_03_DEST_Y	= (src->center.y256);	/* 弾源y256 ボス中心から発弾。 */
}

/*---------------------------------------------------------
	自機(src)狙い弾(dest)の角度(中心→中心)を
	(カードcpuの内部メモリHATSUDAN_03_angle65536に)計算。
	-------------------------------------------------------
	元(src)の中心座標から、
	先(dest)の中心座標へ狙う角度を計算し、
	一時角度変数(HATSUDAN_03_angle65536)に計算する。
	-------------------------------------------------------
	例えば、元(src)==自機、先(dest)==敵弾なら、自機狙い弾。
	-------------------------------------------------------
	(r35)狙い角計算は全て、カードCPUを経由するように変更した。
	狙い角計算がプログラムの複数ヶ所にあると、実行速度が遅すぎて弾幕は無理。
	この変更で問題のある部分も出ているが、
	本家ではこれと似た仕様になっていると思われる。
---------------------------------------------------------*/
extern int atan_65536(int y, int x);/*(vfpu/math_atan65536i.c)*/
global CPU_FUNC(tmp_angleCCW65536_src_nerai)
{
	#if (0)/*(旧仕様メモ)*/
	REG_00_SRC_X	= (src->center.x256);		/*(狙い先)*/
	REG_01_SRC_Y	= (src->center.y256);		/*(狙い先)*/
	REG_02_DEST_X	= (dest->center.x256);		/*(狙い元)*/
	REG_03_DEST_Y	= (dest->center.y256);		/*(狙い元)*/
	#endif
	HATSUDAN_03_angle65536 = (atan_65536((REG_01_SRC_Y)-(REG_03_DEST_Y), (REG_00_SRC_X)-(REG_02_DEST_X)));
}

/*---------------------------------------------------------
	自機狙い角作成
	-------------------------------------------------------
	dest(x,y)の位置から、プレイヤー(x,y)に向けた角度を
	HATSUDAN_03_angle65536に計算する。
---------------------------------------------------------*/
global CPU_FUNC(calculate_jikinerai)
{
	OBJ *zzz_player;
	zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
	REG_00_SRC_X	= (zzz_player->center.x256);		/*(狙い先)*/
	REG_01_SRC_Y	= (zzz_player->center.y256);		/*(狙い先)*/
	#if (0)/*(旧仕様メモ)*/
	REG_02_DEST_X	= (dest->center.x256);			/*(狙い元)*/
	REG_03_DEST_Y	= (dest->center.y256);			/*(狙い元)*/
	#endif
	// 旧仕様メモ(zzz_player, dest);
	tmp_angleCCW65536_src_nerai();
}

/*---------------------------------------------------------
	(r36)カードスクリプト用命令(multiprex_rate_vector)
	複合割合合成。
	-------------------------------------------------------
	使用レジスタ:
	REG_00: 	入力(x0)
	REG_01: 	入力(y0)
	REG_02: 	入力(x1)/出力(x2)
	REG_03: 	入力(y1)/出力(y2)
	REG_11: 	入力(合成割合, t256形式)
---------------------------------------------------------*/
			#if (0)/*(到達割合を考慮して、合成する。)*/
			REG_00_SRC_X	= ((my_menu_resource[i].x_offset[move_mode	])<<8); 	/*(合成位置[A]t256()形式)*/
		//	REG_01_SRC_Y	= ((my_menu_resource[i].y_offset[move_mode	])<<8); 	/*(合成位置[A]t256()形式)*/
			REG_02_DEST_X	= ((my_menu_resource[i].x_offset[move_mode+1])<<8); 	/*(合成位置[B]t256()形式)*/
		//	REG_03_DEST_Y	= ((my_menu_resource[i].y_offset[move_mode+1])<<8); 	/*(合成位置[B]t256()形式)*/
			REG_11_TAMA1	= move_rate256;/*(合成割合t256()形式)*/
			multiprex_rate();
			#endif

global CPU_FUNC(multiprex_rate_vector)
{
	#if 1/*([X]ベクトル合成)*/
	/*(合成順序は割合[A]と割合[B]で、どちらが先でも構わない。)*/
	int x_sa256;/*(t256()形式)*/
	/*合成位置[A]*/x_sa256	= (((REG_00_SRC_X )*(256-REG_11_GOUSEI_WARIAI256))>>8);/*(割合[A]を合成)*/
	/*合成位置[B]*/x_sa256 += (((REG_02_DEST_X)*(	 REG_11_GOUSEI_WARIAI256))>>8);/*(割合[B]を合成)*/
	REG_02_DEST_X	= x_sa256;/*(結果[X])*/
	#endif
	#if 1/*([Y]ベクトル合成)*/
	/*(合成順序は割合[A]と割合[B]で、どちらが先でも構わない。)*/
	int y_sa256;/*(t256()形式)*/
	/*合成位置[A]*/y_sa256	= (((REG_01_SRC_Y )*(256-REG_11_GOUSEI_WARIAI256))>>8);/*(割合[A]を合成)*/
	/*合成位置[B]*/y_sa256 += (((REG_03_DEST_Y)*(	 REG_11_GOUSEI_WARIAI256))>>8);/*(割合[B]を合成)*/
	REG_03_DEST_Y	= y_sa256;/*(結果[Y])*/
	#endif
}

/*---------------------------------------------------------
	(r36)カードスクリプト用命令(replay_rand_init)
	リプレイ対応乱数生成。
	-------------------------------------------------------
	使用レジスタ:
	REG_00: 	入力(リプレイ対応乱数シード設定)
---------------------------------------------------------*/

/*---------------------------------------------------------
	(r36)カードスクリプト用命令(replay_rand)
	リプレイ対応乱数生成。
	-------------------------------------------------------
	使用レジスタ:
	REG_03: 	出力(リプレイ対応乱数値)
---------------------------------------------------------*/


/*---------------------------------------------------------
	カード生成時間
---------------------------------------------------------*/

#define SPELL_TIME_0032 	(32)
#define SPELL_TIME_0048 	(48)
#define SPELL_TIME_0060 	(60)
#define SPELL_TIME_0064 	(64)
#define SPELL_TIME_0070 	(70)
#define SPELL_TIME_0096 	(96)
#define SPELL_TIME_0128 	(128)
#define SPELL_TIME_0192 	(192)
#define SPELL_TIME_0256 	(256)
#define SPELL_TIME_0352 	(352)/*(352==32*11)*/
#define SPELL_TIME_0512 	(512)
//#define SPELL_TIME_64_640 (512+64)
//#define SPELL_TIME_64_640 (640-64)
#define SPELL_TIME_64_640	(512+32)
#define SPELL_TIME_0640 	(640)
//efine SPELL_TIME_0768 	(768)
#define SPELL_TIME_1024 	(1024)
#define SPELL_TIME_9999 	(16384)
/*
704==768-64;
704== (64発弾時間)+(10x64);
*/

/*---------------------------------------------------------
	スペル難易度別テーブル(const_init_nan_ido_なんとか[];)
	-------------------------------------------------------
	カードスクリプトに移行した場合[初期化セクション]で記述する数字のテーブル。
	-------------------------------------------------------
	[初期化セクション](spell_init)のみ、参照できる。
	[発弾セクション](spell_create)では、参照できない。
	-------------------------------------------------------
	万一、spell_createなんとか(){};で、参照していればバグ。
	-------------------------------------------------------
	spell_createなんとか(){};で、参照している場合。
	カードスクリプトに移行出来ないので、参照しない事。
---------------------------------------------------------*/
#if (1)/*(カードスクリプトに移行した場合、無くなる。(カードスクリプト内の[初期化セクション]で行う。))*/
	enum
	{
		H0 = 0,
		H1,
		H2,
		H3,
		H4,
		H5,
		H6,
		H7,
		H8,
		H9,
		H10,
		H11_MAX,
	};
	// ★ 紅 美鈴、華符「芳華絢爛」用
	#define tama_const_H00_NUMS_HOUGA_YELLOW		(H0*4)/*(分割数、黄色)*/
	#define tama_const_H01_NUMS_HOUGA_RED			(H1*4)/*(分割数、紅色)*/
	#define tama_const_H02_DIVS_HOUGA_YELLOW		(H2*4)/*(分割角、黄色)*/
	#define tama_const_H03_DIVS_HOUGA_RED			(H3*4)/*(分割角、紅色)*/
	// ★ チルノ「」用
	#define tama_const_H04_NUMS_CIRNO_BLUE			(H4*4)/*(分割数、青色)*/
	#define tama_const_H05_DIVS_CIRNO_BLUE			(H5*4)/*(分割角、青色)*/
	// ★ アリス 「赤の魔法」用
	#define tama_const_H06_NUMS_ALICE_RED			(H6*4)/*(分割数、紅色)*/
	#define tama_const_H07_DIVS_ALICE_RED			(H7*4)/*(分割角、紅色)*/
//	#define AK A_03_DIV_ANGLE			(H8*4)
	// ★ 「レッドマジック」もどき用
	#define tama_const_H09_NUMS_REMI_MAGIC			(H8*4)/*(分割数、紅色)*/
	#define tama_const_H10_DIVS_REMI_MAGIC			(H9*4)/*(分割角、紅色)*/
	// ★ 幻幽「ジャック・ザ・ルドビレ」もどき用
	#define tama_const_H08_NUMS_SAKUYA_JACK 		(H10*4)/*(分割数、色)*/

	#define const_init_nan_ido_bunkatu_nums_table	const_init_nan_ido_table
	static const /*s32*/u16/*s16 でいいかも？*/ const_init_nan_ido_table[(H11_MAX*4)] =
	{
		// ★ 紅 美鈴、華符「芳華絢爛」用
		// easy 	華符「芳華絢爛」と同じ。(easyは時間で簡単になるように調整する)
		// normal	華符「芳華絢爛」と同じ。
		// hard 	--。
		// lunatic	17分割でちゃんと避けれるの確認済み。(これ以上高密度だと辛いかも)
		/* easy 				normal					hard					lunatic 			*/
	//	(6),					(6),					(10),					(17),					/* 華符「芳華絢爛」用 */
	//	(6*8),					(6*8),					(10*8), 				(17*8), 				/* 華符「芳華絢爛」用 */
	//	(int)(65536/(6)),		(int)(65536/(6)),		(int)(65536/(10)),		(int)(65536/(17)),		/* 華符「芳華絢爛」用 */
	//	(int)(65536/(6*8)), 	(int)(65536/(6*8)), 	(int)(65536/(10*8)),	(int)(65536/(17*8)),	/* 華符「芳華絢爛」用 */
//		// (r39)ボスが中心に拠って来る仕様に変更した為、従来のままでは高密度て避けれない。
		(6),					(6),					(8),					(10),					/* 華符「芳華絢爛」用 */	/* (r39-)仕様を少し変えたので簡単にしないとゲームにならない。 */
		(6*8),					(6*8),					(8*8),  				(10*8), 				/* 華符「芳華絢爛」用 */	/* (r39-)仕様を少し変えたので簡単にしないとゲームにならない。 */
		(int)(65536/(6)),		(int)(65536/(6)),		(int)(65536/(8)),		(int)(65536/(10)),		/* 華符「芳華絢爛」用 */	/* (r39-)仕様を少し変えたので簡単にしないとゲームにならない。 */
		(int)(65536/(6*8)), 	(int)(65536/(6*8)), 	(int)(65536/(8*8)), 	(int)(65536/(10*8)),	/* 華符「芳華絢爛」用 */	/* (r39-)仕様を少し変えたので簡単にしないとゲームにならない。 */
//
		// ★ チルノ「」用
		(0),					(0),					(18),					(36),					/* 註:3より4の方が簡単 */
		(0),					(0),					(int)(65536/(18)),		(int)(65536/(36)),		/* 註:3より4の方が簡単 */
		// ★ アリス 「赤の魔法」用
	//	(3),								(4),								(5),								(6),								/* 分割数:(-r34) */
		(6),								(3),								(5),								(7),								/* 分割数:(r35-) */ 	/* 註:3より4の方が簡単 */
	//	(int)(65536/(18)),					(int)(65536/(24)),					(int)(65536/(40)),					(int)(65536/(64)),					/* 分割角度:  (-r34) */
	//	(int)(65536-(int)(65536/(18))), 	(int)(65536-(int)(65536/(24))), 	(int)(65536-(int)(65536/(40))), 	(int)(65536-(int)(65536/(64))), 	/* 分割角度_R:(-r34) */
		(int)(65536/(12)),					(int)(65536/(18)),					(int)(65536/(20)),					(int)(65536/(24)),					/* 分割角度:  (r35-) */
	//	(int)(65536-(int)(65536/(12))), 	(int)(65536-(int)(65536/(18))), 	(int)(65536-(int)(65536/(20))), 	(int)(65536-(int)(65536/(24))), 	/* 分割角度_R:(r35-) */
		// ★ 「レッドマジック」もどき用
		(10),					(10),					(13),					(18),					/* 「レッドマジック」もどき用 */
		(int)(65536/(10)),		(int)(65536/(10)),		(int)(65536/(13)),		(int)(65536/(18)),		/* 「レッドマジック」もどき用 */
		// ★ 幻幽「ジャック・ザ・ルドビレ」もどき用
		/* easy 				normal					hard					lunatic 			*/
	//	(6),					(12),					(24),					(32),					/* 幻幽「ジャック・ザ・ルドビレ」もどき用 */
		(16),					(32),					(40),					(48),					/* 幻幽「ジャック・ザ・ルドビレ」もどき用 */
	};
#endif

/*---------------------------------------------------------
	カードを生成。
	-------------------------------------------------------
---------------------------------------------------------*/
/* static変数 を調べる作業の為、一時的にlocal にする。 */
#define local static

/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	道中用弾消しコールバック
	弾幕アクション#02
	-------------------------------------------------------
	弾幕の喰み出しチェックを行う(毎フレーム行う必要はない)。
	弾幕が画面外の場合は、弾を消す。
---------------------------------------------------------*/
global RECT_CLIP_CLASS rect_clip;
local OBJ_CALL_FUNC(hatudan_system_B_side_hansya)/* 弾反射コールバック */
{
	{
		/* [左の壁で反射する] 弾が弾幕設定領域画面より少し(4[pixel])手前の位置に来た場合。 */
		if (0 < (rect_clip.bullet_clip_min.x256) + t256(4) - (src->center.x256) )
		{
			goto common_hansya;
		}
		else
		/* [右の壁で反射する] 弾が弾幕設定領域画面より少し(4[pixel])手前の位置に来た場合。 */
		if (0 > (rect_clip.bullet_clip_max.x256) - t256(4) - (src->center.x256) )
		{
			goto common_hansya;
		}
		goto done_hansya;/*(反射しない)*/
	common_hansya:/*(反射する可能性あり)*/
		/* 既に反射している場合は反射しないで、弾を消す。 */
		if (0!=(src->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
		{	/*(反射処理を行わない)*/
			src->jyumyou = JYUMYOU_NASI;/*(弾を消す)*/
		}
		else
	//	if (JYUMYOU_NASI != src->jyumyou)
		{	/*(反射処理を行う)*/
			src->hatudan_register_spec_data |= TAMA_SPEC_KABE_SAYUU_HANSYA_BIT;
		//	src->rotationCCW1024 += (1024*3/4);/* -90度回転 */		/* ダメ[※2] */
		//	src->rotationCCW1024 += (1024/4);/* 90度回転 */ 		/* ダメ[※2] */
			src->rotationCCW1024 = (1024)-(src->rotationCCW1024);	/* 反転[※1] */
			mask1024(src->rotationCCW1024);
			src->hatudan_register_speed65536		= ((t256(1.0)<<8));/*(等速)*/
			src->hatudan_register_tra65536		= (1);/*(微加速)*/
		}
	done_hansya:/*(反射処理終わり)*/
		;
	}
	/* 上下の場合は弾を消す。 */
	if (
		(0 < (rect_clip.bullet_clip_min.y256) - (src->center.y256) )||
		(0 > (rect_clip.bullet_clip_max.y256) - (src->center.y256) )
	)
	{
		src->jyumyou = JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	[弾幕グループ(1)セクション]
	-------------------------------------------------------
	道中用弾消しコールバック
	弾幕アクション#00
	-------------------------------------------------------
	弾幕の喰み出しチェックを行う(毎フレーム行う必要はない)。
	弾幕が画面外の場合は、弾を消す。
---------------------------------------------------------*/
local OBJ_CALL_FUNC(hatudan_system_B_gamen_gai_tama_kesu)/* 画面外弾消しコールバック */
{
	/* 画面外の場合は弾を消す。 */
	if (
	(0 < (rect_clip.bullet_clip_min.x256)-(src->center.x256) ) ||
	(0 > (rect_clip.bullet_clip_max.x256)-(src->center.x256) ) ||
	(0 < (rect_clip.bullet_clip_min.y256)-(src->center.y256) ) ||
	(0 > (rect_clip.bullet_clip_max.y256)-(src->center.y256) ) )
	{
		src->jyumyou = JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	カードを生成。
	-------------------------------------------------------
---------------------------------------------------------*/

#include "spell_00_common.h"
//
#include "spell_00_zako.h"
#include "spell_00_sonota.h"
//
#include "spell_01_mima.h"
#include "spell_01_alice.h"
#include "spell_01_rumia.h"
#include "spell_01_meirin.h"
#include "spell_01_chrno.h"
#include "spell_01_kaguya.h"
#include "spell_01_daiyousei.h"
#include "spell_01_aya.h"
#include "spell_01_momiji.h"

//
#include "spell_01_pache.h"
#include "spell_01_sakuya.h"
#include "spell_01_remilia.h"

//
#include "spell_01_ruiz.h"


/*---------------------------------------------------------
	共通カード生成部
	-------------------------------------------------------
	カードごとに予め生成時間が登録してある。
---------------------------------------------------------*/
enum
{
	TAMA_MAP_06_KOUMA	= 0,	/* 紅魔郷/妖々夢(ナイフ弾/炎弾) */
	TAMA_MAP_08_EIYA,			/* 永夜抄/風神禄(札弾/星弾) */
	TAMA_MAP_10_TIREI,			/* 地霊殿/花映塚(ハート弾/銭弾) */
};
enum
{
	RUN_00_CARD_INTERPRETER 	= 0,	/* カードインタプリタ。 */
	RUN_01_C_LANGUAGE,					/* C言語(廃止予定)。(C言語を全て廃止できれば、実行速度が向上する※1) */
};

typedef struct
{
	void (*spell_generate_section)(OBJ/**/ *sss);		/* [発弾(弾生成)セクション]実行処理 */		/* カードスクリプトに移行した場合の[発弾セクション]。 */
	void (*spell_initialze_section)(OBJ/**/ *sss);		/* [初期化セクション]初期化処理 */			/* カードスクリプトに移行した場合の[初期化セクション]。 */
	u16 spell_limit_max_time;							/* カード寿命時間 */						/* カードスクリプトに移行した場合、本来[初期化セクション]内で行う。 */
	u8 tama_map;										/* 弾画像のマップ番号 */					/* カードスクリプトに移行した場合、本来[初期化セクション]内で行う。 */
	u8 scr_ipt_run_flag;								/* 実行言語(カードインタプリタ / C言語) */
} SPELL_RESOURCE;
//	/*const*/ static void (*spell_create_bbb[(SPELL_MAX/*16+1*/)])(OBJ/**/ *sss) =
	/*const*/ static SPELL_RESOURCE system_spell_resource[(SPELL_MAX/*16+1*/)] =
{
	#define spell_create_99_mitei spell_create_08_rumia_night_bird
/* 00 00 */ {	NULL,									NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 00 */	/* カード生成しない */
			// 0x00- 中ザコ / ボス共用カード
/* 01 01 */ {	spell_create_01_sakuya_misogi_normal,	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 01 */	/* [咲夜] 紅5面中-ボス 「通常攻撃1」咲夜禊カード */
/* 02 02 */ //{ spell_create_02_24nerai,				NULL,										SPELL_TIME_0048,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 02 */	/* [咲夜] 紅5面中-ボス 24カード (奇術「ミスディレクションもどき(1/2)」) */
/* 03 03 */ //{ spell_create_03_11nife, 				NULL,										SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 03 */	/* [咲夜] 紅5面中-ボス 11カード (奇術「ミスディレクションもどき(2/2)」) */
/* 02 02 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 02 */	/* [雑魚] (未使用) */
/* 03 03 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 03 */	/* [雑魚] (未使用) */
/* 04 04 */ {	spell_create_04_pink_hearts,			NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 04 */	/* [咲夜] 紅5面ボス 「通常攻撃1(1/2)」にちょっとだけ似たカード(予定) SPELL_TIME_0128(r32) */
/* 05 05 */ {	spell_create_05_32way_dual, 			NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 05 */	/* [咲夜] 紅5面ボス 「通常攻撃1/2(2/2)」にちょっとだけ似たカード(予定) */
/* 06 06 */ {	spell_create_06_luna_clock_32way,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 06 */	/* [咲夜] 紅5面ボス 幻象「ルナクロック(1/2)」にちょっとだけ似たカード(予定) */
/* 07 07 */ {	spell_create_07_80way_dual_five,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 07 */	/* [咲夜] 紅5面ボス 「通常攻撃3(1/2)」にちょっとだけ似たカード(予定) */
			// 0x08- 中ザコ / ボス共用カード
/* 08 08 */ {	spell_create_08_rumia_night_bird,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 08 */	/* [ルーミア] 紅1面ボス 夜符「ナイトバード」にちょっとだけ似たカード(予定) */
/* 09 09 */ {	spell_create_09_zako_sentakki,			NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 09 */	/* [雑魚] 洗濯機カード(とりあえずカードシステム作成の実験で作ってみた奴) */
/* 10 0a */ {	spell_create_1a_dai_yousei_midori,		spell_init_1a_dai_yousei_midori,			SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1a */	/* [大妖精] 緑巻き */
/* 11 0b */ {	spell_create_1b_dai_yousei_aka, 		spell_init_1b_dai_yousei_aka,				SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1b */	/* [大妖精] 赤巻き */
/* 12 0c */ {	spell_create_14_sakuya_jack_oodama32,	spell_init_14_sakuya_jack_oodama32, 		SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 14 */	/* [咲夜] 幻幽「ジャック・ザ・ルドビレ」になるかもしれないテスト */
/* 13 0d */ {	spell_create_22_sakuya_miss_direction,	spell_init_22_sakuya_miss_direction,		SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1c */	/* [咲夜] 奇術「ミスディレクション」 */
/* 14 0e */ {	spell_create_1f_remilia_kougeki2,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1f */	/* [レミリア] 通常攻撃2 っぽいもの */
/* 15 0f */ {	spell_create_11_tengu_shot, 			NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 11 */	/* [天狗様] 差分氏の妖怪1(天狗様?)が撃つカード */
			// 0x10- 中ザコ / ボス共用カード
/* 16 10 */ {	spell_create_10_cirno_misogi,			spell_init_10_cirno_misogi, 				SPELL_TIME_0096,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 10 */	/* [チルノ] 紅2面ボス チルノ禊カード */
/* 17 11 */ {	spell_create_13_perfect_freeze, 		spell_init_13_perfect_freeze,				SPELL_TIME_64_640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 13 */	/* [チルノ] 紅2面ボス パーフェクトフリーズ */
/* 18 12 */ {	spell_create_16_diamond_blizzard,		NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 16 */	/* [チルノ] 紅2面ボス 雪符「ダイアモンドブリザード」 */
/* 19 13 */ {	spell_create_25_alice_suwako,			NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 25 */	/* [アリス] アリス	諏訪子っぽい(?)カード */
/* 20 14 */ {	spell_create_17_alice_nejiri10sec,		spell_init_17_alice_nejiri10sec,			SPELL_TIME_0192,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 17 */	/* [アリス] 通常攻撃(もどき) ねじり弾10秒 SPELL_TIME_0640 */
/* 21 15 */ {	spell_create_0e_aka_2nd,				spell_init_0e_aka_2nd,						SPELL_TIME_1024,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0e */	/* [アリス] 怪EX面ボス 赤の魔法2段階目もどき */
/* 22 16 */ {	spell_create_0b_alice_zako_doll,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0b */	/* [アリス] SPELL_TIME_0128 妖3面ボス 人形カード(もどき)(256>240=4x60) */
/* 23 17 */ {	spell_create_1e_alice_tama_doll,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1e */	/* [アリス] 妖3面ボス 人形カード(もどき)(256>240=4x60) 蒼符「博愛の仏蘭西人形」 */
			// 0x18- 中ザコ / ボス共用カード
/* 24 18 */ {	spell_create_0c_hana_test,				spell_init_0c_hana_test,					SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0c */	/* [魅魔] 花てすとカード */
/* 64 40 */ {	spell_create_20_sonota_debug_cw_ao, 	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 40 */	/* [その他] デバッグカードCW青 */
/* 65 41 */ {	spell_create_21_sonota_debug_ccw_aka,	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 41 */	/* [その他] デバッグカードCCW赤 */
/* 27 1b */ {	spell_create_1b_cirno_icecle_fall,		spell_init_1b_cirno_icecle_fall,			SPELL_TIME_0352,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [チルノ] 氷符「アイシクルフォール」 */
/* 68 44 */ {	spell_create_44_ruiz3,					NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 44 */	/* [ルイズ] 左右白カード */
/* 29 1d */ {	spell_create_1d_amefuri_test,			spell_init_1d_amefuri_test, 				SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1d */	/* [(文、仮)] 雨てすと */
/* 30 1e */ {	spell_create_1e_momiji_no_nonoji,		NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [椛] ののじカード */
/* 31 1f */ {	spell_create_1f_koakuma,				NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [パチェ] サイレントセレナ の てすと用 */
	// 0x20- ボス専用カード
/* 32 20 */ {	spell_create_15_aya_misogi1,			NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 15 */	/* [文] の通常弾っぽいカード撃たせてみるテスト1 */
/* 33 21 */ {	spell_create_23_aya_misogi2,			NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 23 */	/* [文]の通常弾っぽいカード撃たせてみるテスト2 */
/* 34 22 */ {	spell_create_22_aya_yatimata,			spell_init_22_aya_yatimata, 				SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 22 */	/* [文] 岐符「天の八衢」 */
/* 35 23 */ {	spell_create_12_meirin_magaru_kunai,	spell_init_12_meirin_magaru_kunai,			SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 12 */	/* [美鈴] 紅3面中ボス 「通常攻撃」の曲がるクナイ。 */
/* 36 24 */ {	spell_create_0f_aya_doll,				NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0f */	/* [文] 人形カード(てすとカード「危険な種」) */
/* 37 25 */ {	spell_create_0a_houka_kenran,			spell_init_0a_houka_kenran, 				SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0a */	/* [紅美鈴] 紅3面ボス 華符「芳華絢爛」にちょっとだけ似たカード(予定) */
/* 38 26 */ {	spell_create_26_aya_saifu,				NULL,										SPELL_TIME_9999,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [文] 塞符 */
/* 39 27 */ {	spell_create_27_hosigata_test,			spell_init_27_hosigata_test,				SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 27 */	/* [その他] 星型テスト */
			// 0x28- ボス専用カード
/* 40 28 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 30 */	/* [] */
/* 41 29 */ {	spell_create_29_rumia_demarcation,		spell_init_29_rumia_demarcation,			SPELL_TIME_0512,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 31 */	/* [ルーミア] 紅1面ボス 闇符「ディマーケイション」にちょっとだけ似たカード(予定) dimmercation / demarcation 境界 */
/* 42 2a */ {	spell_create_2a_sakuya_baramaki1,		NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [咲夜] 紅5面中-ボス ばら撒き1 カード () */
/* 43 2b */ {	spell_create_2b_sakuya_baramaki2,		NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [咲夜] 紅5面中-ボス ばら撒き2 カード () */
/* 44 2c */ {	spell_create_2c_sakuya_blue_red_knife,	spell_init_2c_sakuya_blue_red_knife,		SPELL_TIME_0512,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 26 */	/* [咲夜] 紅5面中-ボス 「通常攻撃3」青赤ナイフ */
/* 45 2d */ {	spell_create_2d_sakuya_misogi_lunatic,	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 01 */	/* [咲夜] 紅5面中-ボス 「通常攻撃1」咲夜禊カード(lunatic only) */
/* 46 2e */ {	spell_create_2e_pache_agni_shine_1, 	spell_init_2e_pache_agni_shine_1,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 2e */	/* [パチェ] "　　　　火符「アグニシャイン」" No.15 */	// (火がいっぱいだから) (輝く)アグニ(インド神話の火の神)
/* 47 2f */ {	spell_create_2f_pache_princess_undine,	NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 2f */	/* [パチェ] "　水符「プリンセスウンディネ」" No.16 */	// (水がいっぱいだから) 「妖精の書(著者:錬金術師パラケルスス)」に登場する水の精。
			// 0x30- ボス専用カード
/* 48 30 */ {	spell_create_30_pache_sylphy_horn_1,	spell_init_30_pache_sylphy_horn_1,			SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 30 */	/* [パチェ] "　　　　木符「シルフィホルン」" No.17 */	// (木がいっぱいだから) (風を司る精霊の)角笛
/* 49 31 */ {	spell_create_31_pache_rage_tririton_1,	spell_init_31_pache_rage_tririton_1,		SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 31 */	/* [パチェ] "　　土符「レイジィトリリトン」" No.18 */	// (土がいっぱいだから) (怒る)トリリトン(ストーンヘンジ)
/* 50 32 */ {	spell_create_32_pache_metal_fatigue,	spell_init_32_pache_metal_fatigue,			SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 32 */	/* [パチェ] "　　金符「メタルファティーグ」" No.19 */	// (金(金属)がいっぱいだから) 金属疲労
/* 51 33 */ {	spell_create_33_pache_agni_shine_2, 	spell_init_33_pache_agni_shine_2,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 33 */	/* [パチェ] "　　火符「アグニシャイン上級」" No.20 */	// (火がいっぱいだから) (輝く)アグニ(インド神話の火の神)
/* 52 34 */ {	spell_create_34_pache_sylphy_horn_2,	spell_init_34_pache_sylphy_horn_2,/*共用*/	SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 34 */	/* [パチェ] "　　木符「シルフィホルン上級」" No.21 */	// (木がいっぱいだから) (風を司る精霊の)角笛
/* 53 35 */ {	spell_create_35_pache_rage_tririton_2,	spell_init_31_pache_rage_tririton_1,/*(?)*/ SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 35 */	/* [パチェ] "土符「レイジィトリリトン上級」" No.22 */	// (土がいっぱいだから) (怒る)トリリトン(ストーンヘンジ)
/* 54 36 */ {	spell_create_36_pache_agni_radiance,	NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 36 */	/* [パチェ] "　火符「アグニレイディアンス」" No.23 */	// (火がいっぱいいっぱいだから) アグニ(インド神話の火の神)
/* 55 37 */ {	spell_create_37_pache_bury_in_lake, 	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 37 */	/* [パチェ] "　　　水符「ベリーインレイク」" No.24 */	// (水がいっぱいいっぱいだから)  湖に埋める(水葬)
			// 0x38- ボス専用カード
/* 56 38 */ {	spell_create_38_pache_green_storm,		spell_init_38_pache_green_storm,/*共用*/	SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 38 */	/* [パチェ] "　　　木符「グリーンストーム」" No.25 */	// (木がいっぱいいっぱいだから) 緑の嵐
/* 57 39 */ {	spell_create_39_pache_tririton_shake,	spell_init_31_pache_rage_tririton_1,/*共用*/SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 39 */	/* [パチェ] "　　土符「トリリトンシェイク」" No.26 */	// (土がいっぱいいっぱいだから) (揺れる)トリリトン(ストーンヘンジ)
/* 58 3a */ {	spell_create_3a_pache_silver_dragon,	spell_init_3a_pache_silver_dragon,/*(??)*/	SPELL_TIME_0032,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3a */	/* [パチェ] "　　　金符「シルバードラゴン」" No.27 */	// (金(かね)がいっぱいいっぱいだから) 「金と銀(著者:福山)」
/* 59 3b */ {	spell_create_3b_pache_lava_cromlech,	spell_init_31_pache_rage_tririton_1,/*共用*/SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3b */	/* [パチェ] "火＆土符「ラーヴァクロムレク」" No.28 */	// (火＋土＝溶岩) 環状列石(ストーンヘンジ)
/* 60 3c */ {	spell_create_3c_pache_forest_blaze, 	NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3c */	/* [パチェ] "木＆火符「フォレストブレイズ」" No.29 */	// (木＋火＝)森林火災
/* 61 3d */ {	spell_create_3d_pache_water_elf,		NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3d */	/* [パチェ] "　水＆木符「ウォーターエルフ」" No.30 */	// (水＋木＝)水溶性(水妖精)
/* 62 3e */ {	spell_create_3e_pache_mercury_poison,	spell_init_3e_pache_mercury_poison, 		SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3e */	/* [パチェ] "金＆水符「マーキュリポイズン」" No.31 */	// (金(金属)＋水＝)水銀中毒
/* 63 3f */ {	spell_create_3f_pache_emerald_megalith, spell_init_3f_pache_emerald_megalith,		SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 3f */	/* [パチェ] "土＆金符「エメラルドメガリス」" No.32 */	// (土＋金(かね)＝)エメラルドの巨石
			// 0x40
/* 66 42 */ {	spell_create_42_pache_laser1,			NULL,										SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 42 */	/* [パチェ] 通常攻撃(もどき) のレーザーもどき */
/* 67 43 */ {	spell_create_43_pache_laser2,			spell_init_43_pache_laser2, 				SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 43 */	/* [パチェ] 通常攻撃(もどき) のレーザーもどき */
/* 25 19 */ {	spell_create_0d_mima_sekkin,			spell_init_mima_kaguya,/*(特殊)*/			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 0d */	/* [魅魔] 接近カード */
/* 26 1a */ {	spell_create_24_mima_toge,				spell_init_mima_kaguya,/*(特殊)*/			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 24 */	/* [魅魔] トゲカード */
/* 69 45 */ {	spell_create_18_kaguya01,				spell_init_mima_kaguya,/*(特殊)*/			SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 18 */	/* [輝夜] とりあえず強引に変換 */
/* 70 46 */ {	spell_create_19_kaguya04,				spell_init_mima_kaguya,/*(特殊)*/			SPELL_TIME_0064,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 19 */	/* [輝夜] とりあえず強引に変換 */
/* 28 1c */ {	spell_create_23_kaguya_tamanoe, 		spell_init_mima_kaguya,/*(特殊)*/			SPELL_TIME_0640,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 1d */	/* [輝夜] 蓬莱の玉の枝もどき */
/* 71 47 */ {	spell_create_47_sakuya_festival_knife,	spell_init_47_sakuya_festival_knife,		SPELL_TIME_9999,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 47 */	/* [咲夜] 幻想「フェスティバルナイフ」(?) むりやり変換 */
			// 0x48
/* 48 48 */ {	spell_create_48_r34_gokan_kinou,		NULL,										SPELL_TIME_9999,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 48 */	/* [r34]r34以前の互換機能。(r35-)システムで必ず必要。 */
/* 49 49 */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 49 */	/* [] */
/* 4a 4a */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4a */	/* [] */
/* 4b 4b */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4b */	/* [] */
/* 4c 4c */ {	spell_create_99_mitei,					NULL,										SPELL_TIME_0128,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4c */	/* [] */
/* 4d 4d */ {	spell_create_4d_remilia_rasen_knife,	NULL,										SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4d */	/* [咲夜/レミリア] 紅6面ボス 通常攻撃  */
/* 4e 4e */ {	spell_create_4e_remilia_vlad_tepes, 	spell_init_4e_remilia_vlad_tepes,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 30 */	/* [咲夜/レミリア] 紅6面ボス 呪詛「ブラド・ツェペシュの呪い」 */
/* 4f 4f */ {	spell_create_4f_remilia_red_magic,		spell_init_4f_remilia_red_magic,			SPELL_TIME_0256,	TAMA_MAP_06_KOUMA, RUN_01_C_LANGUAGE},	/* 4f */	/* [咲夜/レミリア] 紅6面ボス 「レッドマジック」 */

};

/*---------------------------------------------------------
	共通部分。
---------------------------------------------------------*/
static void spell_cpu_common_init(void)
{
	#if (1==USE_BOSS_JIKANGIRE)/*(使用予定あり。未検証)*/
	cg.state_flag &= (~JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE);/*(時間切れフラグoff)*/
	#endif
	/* カードレジスタ全初期化 */
	spell_cpu_initialize_all_register();
	#if (1)
	/* 弾幕コールバックシステムの初期化 */
	{
		card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(r36-書き換え禁止)*/	/*(通常弾用)*/
		card.danmaku_callback[1] = NULL;
		card.danmaku_callback[2] = NULL;
		card.danmaku_callback[3] = NULL;
	}
	#endif
}


/*---------------------------------------------------------
	道中の前に初期化する必要がある部分。
---------------------------------------------------------*/

global void spell_cpu_douchuu_init(void)
{
	spell_cpu_common_init();/*(共通部分)*/
}


/*---------------------------------------------------------
	カードの初期化。カードが変わると毎回行う必要がある。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(card_maikai_init)
{
	spell_cpu_common_init();/*(共通部分)*/
	#if (1)/* カードスクリプトに移行した場合の[初期化セクション]。 */
	/* カード初期化セクションを実行する。 */
	if (NULL!=(system_spell_resource[(card.spell_used_number/*-1*/)].spell_initialze_section))
	{
		(system_spell_resource[(card.spell_used_number/*-1*/)].spell_initialze_section)(src);
	}
	/* カード時間に予め登録された寿命時間を設定する。 */
	/* カードの制限時間を設定(予めカードごとに設定されている標準時間に設定) */
	/* カードの時間切れを設定 */
	REG_10_BOSS_SPELL_TIMER = system_spell_resource[(card.spell_used_number/*-1*/)].spell_limit_max_time;
	#endif
}


/*---------------------------------------------------------
	スペルCPUを実行し、カードを１フレーム生成する。
	-------------------------------------------------------
	カード生成すると時間経過し、カード寿命時間が自動で減る。
	カード寿命時間が無くなった場合は、カードタイプが無し(SPELL_00)になる。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(card_generate)
{
	if (SPELL_00 != card.spell_used_number) 	/* スペル生成は必要？ */
	{
		REG_10_BOSS_SPELL_TIMER--;				/* 時間経過する */	/*fps_factor*/
		if ((0) < (REG_10_BOSS_SPELL_TIMER))	/* 時間切れ？ */
		{
			/* 弾源x256 y256 ボスの中心から発弾。 */
			set_REG_DEST_XY(src);
			#if (1)/* カードスクリプトに移行した場合の[発弾セクション]。 */
			/* カード生成中 */
			(system_spell_resource[(card.spell_used_number/*-1*/)].spell_generate_section)(src);
			#endif
		}
		else
		{
			card.spell_used_number = SPELL_00;		/* カード生成終了 */
		}
	}
}
