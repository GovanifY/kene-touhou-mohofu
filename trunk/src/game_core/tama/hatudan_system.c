
#include "game_main.h"

/*---------------------------------------------------------
  東方模倣風 ～ Toho Imitation Style.
  プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	発弾システム
	-------------------------------------------------------
	(r33)もう一回汎用的な弾発射システムを構築し直してみる。
	旧 bullet_angle.c と
	旧 bullet_vector.c は
	ここに統合される予定。
	レーザー関連は統合されないで、別システムになる予定。
	-------------------------------------------------------
	(r32-)喰み出しチェックについて、
	「移動時に」喰み出しチェックをしない事が前提で設計されているカードが多い。
	この為「移動時に」システムで喰み出しチェックをしません。
	例えば画面端で跳ね返る弾は「移動時に」喰み出しチェックをしない事により実現している機能です。
	「移動と別で」全弾配列を調べて「喰み出しチェック」を行います。
---------------------------------------------------------*/

/*---------------------------------------------------------
	交差弾は色々弾によって違いが多すぎる。(個々の時間とか)
	全部システム側で吸収すると、遅くなる気がする。
	交差弾システム側で吸収するなら、角度弾と統合して角度弾無くすべき
	だけど(将来はともかく)現状では角度弾汎用にすると遅すぎる。
	-------------------------------------------------------
	現状の速度なら、交差弾は全弾配列書き換えで対応すべき。
	(パチェとか書き換えの嵐になりそうですが。)
	システム側交差弾対応は、無くなりそう。
---------------------------------------------------------*/

/*---------------------------------------------------------
	角度弾、基本定義
	-------------------------------------------------------
	角度弾では、ベクトル移動をしない。
	代わりに基点座標として使う。
---------------------------------------------------------*/

#if 1/* 角度弾規格(策定案tama_system) */
	#define ox256				vx256/* 発弾位置 座標x */
	#define oy256				vy256/* 発弾位置 座標y */
#endif


#define hatudan_system_kousadan_angle65536 tmp_angleCCW1024


/*---------------------------------------------------------
	発弾エフェクト
	-------------------------------------------------------
	傾かない弾 / 傾き弾(通常)
---------------------------------------------------------*/
static void set_mover(SPRITE *src);
static void move_bullet_hatsudan_effect(SPRITE *src)
{
//	hatudan_system_common_hatudan_000(src);
	{
		const int aaabbb = (src->hatudan_register_flame_counter);
		#if (0)
		src->cx256 = (src->ox256) + ((si n1024((src->rotationCCW1024))*(aaabbb)) );/*fps_factor*/
		src->cy256 = (src->oy256) + ((co s1024((src->rotationCCW1024))*(aaabbb)) );/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
			src->cx256 = (src->ox256) + ((sin_value_t256*(aaabbb))>>(((src->hatudan_register_spec_data)>>12)&0x03));/*fps_factor*/
			src->cy256 = (src->oy256) + ((cos_value_t256*(aaabbb))>>(((src->hatudan_register_spec_data)>>12)&0x03));/*fps_factor*/
		}
		#endif
		src->color32		= ((256-aaabbb)<<(24))|0x00ffffff;
		src->m_zoom_x256	= t256(1.0) + (aaabbb) + (aaabbb);
		src->m_zoom_y256	= t256(1.0) + (aaabbb) + (aaabbb);
	}
	src->hatudan_register_flame_counter -= (4);
	if (0 > src->hatudan_register_flame_counter)
	{
	//	src->hatudan_register_flame_counter = 0;
//		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_4000_NON_MOVE))
//		{
//			src->callback_mover 			= hatudan_system_move_angle_001;/*(雑魚等では問題無いが、弾幕には遅すぎる)*/
//		}
//		else	/* 本家は恐らく共通移動処理はそもそも無い。 */
//		{
//		//	src->callback_mover 			= NULL;/*(弾幕処理中に移動処理もするので、OBJで移動処理は不要)*/
//			src->callback_mover 			= hatudan_system_move_dummy_000;/*(弾幕処理中に移動処理もするので、OBJで移動処理は不要)*/
//		}
		src->hatudan_register_spec_data |= (TAMA_SPEC_3000_EFFECT_NONE);
		set_mover(src);
	}
}
/*---------------------------------------------------------
	移動処理選択。
---------------------------------------------------------*/
static void set_mover(SPRITE *src)
{
	void (*aaa[4/*8*/])(SPRITE *bbb) =
	{
//	/* 移動処理あり */	move_bullet_hatsudan_effect,/* エフェクト大(旧r34互換) */
//	/* 移動処理あり */	move_bullet_hatsudan_effect,/* エフェクト中 */
//	/* 移動処理あり */	move_bullet_hatsudan_effect,/* エフェクト小 */
//	/* 移動処理あり */	NULL,//hatudan_system_move_angle_001,/* エフェクトなし */
	/* 移動処理なし */	move_bullet_hatsudan_effect,/* エフェクト大(旧r34互換) */
	/* 移動処理なし */	move_bullet_hatsudan_effect,/* エフェクト中 */
	/* 移動処理なし */	move_bullet_hatsudan_effect,/* エフェクト小 */
	/* 移動処理なし */	NULL,//hatudan_system_move_dummy_000,/* エフェクトなし */
	};
//	src->callback_mover 	= aaa[((src->hatudan_register_spec_data)>>12)&0x07];
	src->callback_mover 	= aaa[((src->hatudan_register_spec_data)>>12)&0x03];
	if ( 3 == (((src->hatudan_register_spec_data)>>12)&0x03) )	/* 発弾開始処理 */
	{
		/* 発弾開始直前にあたり判定を有効にする。 */
		/* 非傾き弾は、発弾開始直前に設定。 */
		src->flags |= (SP_FLAG_COLISION_CHECK);/* あたり判定有効 */
		if (src->hatudan_register_spec_data & TAMA_SPEC_8000_NON_TILT)/* 非傾き弾 */
		{
			src->m_zoom_y256 = M_ZOOM_Y256_NO_TILT;/* 特殊機能で傾かないようシステム拡張(r33)。 */
		}
	//	src->hatudan_register_flame_counter = 0;/*(この後で使うなら)*/
		#if (1)/*(発弾エフェクトが無い場合に困る)*/
		src->cx256 = (src->ox256);/*fps_factor*/
		src->cy256 = (src->oy256);/*fps_factor*/
		#endif
	}
//	else	{;}/*(発弾エフェクト処理)*/
}
/*---------------------------------------------------------
	弾を登録する。
	-------------------------------------------------------
	弾発動時のエフェクト等の関係上、発弾部分は全部纏める必要がある。
	-------------------------------------------------------
	ベクトル弾と角度弾を統合するかもしれない。
---------------------------------------------------------*/


/*---------------------------------------------------------
	標準の角度弾を複数(n way弾)登録する。
	-------------------------------------------------------
	弾の発弾方式は、1ヶ所に纏める予定なのだが、
	現在仕様が決まっていない。
	弾の発弾が、1ヶ所に纏まっていないと、発弾のエフェクトが作れない。
	「発弾のエフェクトが作れない」というのは、
	弾の発弾が、1ヶ所に纏まっていないのに強引に「作ると遅くなる」からやりたくないという意味。
	現在(r33)も速度がネック(遅い)になっているので、そういう事はしない。というか出来ない。
	もちろん1ヶ所に纏まって速度も解消できたら発弾のエフェクトを作る予定。
	現在(r33)の速度低下は、
		サイドのパネルがSDL描画。
		会話がSDL描画。
		Gu回転描画で横sliceしてない。
		ボスがカードシステムに移行出来てないので無駄な処理がある。
	あたりが、主な処理落ち。
	-------------------------------------------------------
	あくまで暫定仕様(r32)
	-------------------------------------------------------
	BULLET_REGIST_06_n_way: 		n way弾の総弾数
		一度に弾を撃つ数。1つしか撃たないなら1。
	BULLET_REGIST_02_angle65536:		開始向き
		１周を65536分割した値(0 ... 65535)でnway弾の発弾開始角度を指定する。
		下向きが0度で半時計回り。(下CCW)
	BULLET_REGIST_04_bullet_obj_type:	弾の種類(具体的にはグラ)の番号

	BULLET_REGIST_00_speed256:		初速(打ち出し速度)

	BULLET_REGIST_07_div_angle65536:	n way弾の分割角度

	BULLET_REGIST_01_speed_offset:	加速度(又は減速度)

---------------------------------------------------------*/

/*---------------------------------------------------------
	弾システム:
	基本弾発生
	-------------------------------------------------------
	仕様： 引数は破壊しない。
---------------------------------------------------------*/

global void hatudan_system_regist_single(void)
{
	SPRITE		*h;
	h										= obj_add_00_tama_error();/* 発弾登録 */
	if (NULL != h)/* 登録できた場合 */
	{
		h->ox256							= REG_02_DEST_X;/* 発弾位置 座標x */
		h->oy256							= REG_03_DEST_Y;/* 発弾位置 座標y */
		h->type 							= (HATSUDAN_05_bullet_obj_type);
		h->flags &= ~(SP_FLAG_COLISION_CHECK);/* あたり判定無効(発弾エフェクト用) */
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
	//
	//
		h->hatudan_system_kousadan_angle65536	= (HATSUDAN_03_angle65536); 				/* 交差弾用 */
		h->rotationCCW1024					= (deg65536to1024(HATSUDAN_03_angle65536)); /* 「1周が65536分割」から「1周が1024分割」へ変換する。 */	/* (i<<4) deg_360_to_512(90) */
		h->m_Hit256R						= TAMA_ATARI_JIPPOU32_PNG;
	//
		h->hatudan_register_speed65536		= ((HATSUDAN_01_speed256)<<8);		/* t65536形式で。 速度 */	/* 初速(打ち出し速度) */
		h->hatudan_register_flame_counter		= (0xff);//(0);//(-(0x100));
		h->hatudan_register_tra65536			= (HATSUDAN_02_speed_offset);		/* t65536形式で。 調整減速弾 */ 	/* この方式になるか検討中 */
		h->hatudan_register_spec_data			= (HATSUDAN_04_tama_spec);			/* 弾の基本所属能力を設定する。 */
		set_mover(h);
	}
}


/*---------------------------------------------------------
	弾システム:
	偏り n way 弾
	紅魔郷の n way 弾システムは、どうもこういうものっぽい。
	★ 5面ボス咲夜の「通常攻撃1」及び「通常攻撃2」で
	咲夜が生成した魔方陣は、 5way とか 6way とか 弾を撃ってくるが、
	一気に描かないで、左右で2回にかけて描く。
	つまり左右でプライオリティー(弾の重なり具合)が違う。

	★ 5面中-ボス咲夜の「通常攻撃」は(青ナイフ、赤クナイ共に)左右が固定の固定弾なので、
	こういう n way 弾だと都合が良い。(旧作のなぎ払いを再現するのも都合が良い)
	★ 5面中-ボス咲夜の「ミスディレクション」も、正確な自機狙いが保証されるので、
	こういう n way 弾だと都合が良い。
	-------------------------------------------------------
	以下は基本部分で撃てるのか、それとも若干改変したものなのか不明(たぶん基本部分で撃てる)

	★ 5面ボス咲夜の「通常攻撃1」のピンクのナイフは基本部分で撃てるのか
	それとも基本部分を若干改変したものかは判らないが殆ど同じもの。

	★ ルーミアの紅魔郷の「ナイトバード」は基本部分で撃てるのか
	それとも基本部分を若干改変したものかは判らないが殆ど同じもの。
	-------------------------------------------------------
	仕様： 下記の引数を破壊するので注意
	HATSUDAN_03_angle65536	[破壊]
---------------------------------------------------------*/

global void hatudan_system_regist_katayori_n_way(void)
{
	#if (0)/*[ここ有効にすれば、たぶん復旧可能]*/
	const int j_angle65536	= (HATSUDAN_03_angle65536);/* 始めの開始角度、向き */
	#endif
	{
		int i;
		for (i=(0); i<(HATSUDAN_06_n_way); i++)/* n way弾を登録する。 */
		{
			hatudan_system_regist_single();
			/* 次の角度 */
			HATSUDAN_03_angle65536 += (HATSUDAN_07_div_angle65536);
		//	mask65536(HATSUDAN_03_angle65536);たぶん要らない。
		}
	}
	#if (0)/*[ここ有効にすれば、たぶん復旧可能]*/
	HATSUDAN_03_angle65536		= (j_angle65536);/*[復旧]*/
	#endif
}
/*
	弾システム:
	n way 弾
	n way 弾は、一気に生成しないで、偏り n way 弾を2回呼ぶ事で生成する。
	これは奇数弾の場合の狙い誤差を最小にする為に必要な措置。
	奇数弾の積算誤差が大きいと自機狙い弾のつもりで生成しても
	多弾生成時の積算誤差で角度がずれるので、自機を狙わなくなってしまう。
	プライオリティー(弾の重なり具合)を調べると原作でもこの方式を採用している事がわかる。
	原作では弾の生成順にプライオリティー(ZバッファなのでZ値)を決めている。
	-------------------------------------------------------
	仕様： 下記の引数を破壊するので注意
	HATSUDAN_03_angle65536		[破壊]
	HATSUDAN_06_n_way			[破壊]
	HATSUDAN_07_div_angle65536	[破壊]
*/
global void hatudan_system_regist_n_way(void)
{
	const int j_angle65536	= (HATSUDAN_03_angle65536);/* 開始向き */
	const int j_n_way		= (HATSUDAN_06_n_way)&0xff;/* 弾数 */
	const int j_n_way_harf	= (j_n_way>>1)&0xff;/* 弾数 */
	//
	{
		/* 偶数弾の場合の差分を生成
			偶数弾の場合、分割角度の半分の角度がオフセット角度。
			奇数弾の場合オフセット角度は必ずゼロ。
		*/
	//	const int j_guusuu_harf_angle_offset = ((HATSUDAN_07_div_angle65536)>>1);	/* 偶数弾なら分割角の半分が差分、奇数弾はなし */
		const int j_guusuu_harf_angle_offset = (0==(j_n_way&1)) ? ((HATSUDAN_07_div_angle65536)>>1) : (0);	/* 偶数弾なら分割角の半分が差分、奇数弾はなし */
		/*(右半分を生成)*/
		/* 右半分(差分が正値の場合) */
		HATSUDAN_03_angle65536		= j_angle65536 + j_guusuu_harf_angle_offset;/* 偶数弾 & 奇数弾 */
		HATSUDAN_06_n_way			= (j_n_way - (j_n_way_harf));	/* 例えば 7way なら 4弾描く。 8way なら 4弾描く。 */
		hatudan_system_regist_katayori_n_way();
		/*(左半分を生成)*/
		#if 0/* デバッグ用 */
			HATSUDAN_05_bullet_obj_type ^= 2;/* デバッグ用: 同じグループだけど、左右で違う色の弾にしてみる。 */
		#endif
		/* 差分角度を逆にする。(逆周りに描くway弾になる) */
		HATSUDAN_07_div_angle65536	= (65536)-(HATSUDAN_07_div_angle65536);
		#if 1
		HATSUDAN_03_angle65536		= j_angle65536 - j_guusuu_harf_angle_offset;/* 偶数弾 & 奇数弾 */
		/* 左半分(差分が正値の場合) */
		if (0!=(j_n_way&1))/* 奇数弾 */
		{
			HATSUDAN_03_angle65536 += HATSUDAN_07_div_angle65536;/* 奇数弾(は1弾少ないのでずらす) */
		}
	//	else/* 偶数弾 */
	//	{;/* なにもしない */}
	//	HATSUDAN_03_angle65536		= j_angle65536 + ((0==(j_n_way&1))?(-j_guusuu_harf_angle_offset):(HATSUDAN_07_div_angle65536));
		HATSUDAN_06_n_way			= ( 		 (j_n_way_harf));	/* 例えば 7way なら 3弾描く。 8way なら 4弾描く。 */
		hatudan_system_regist_katayori_n_way();
		#endif
	}
	#if (0)/*[ここ有効にすれば、たぶん復旧可能]*/
	HATSUDAN_03_angle65536		= (j_angle65536);/*[復旧]*/
	HATSUDAN_06_n_way			= (j_n_way);/*[復旧]*/
	HATSUDAN_07_div_angle65536	= (65536)-(HATSUDAN_07_div_angle65536);/*[たぶん復旧]*/
	#endif
}
