
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アイテム関連
---------------------------------------------------------*/

#include "jiki_local.h"
/*---------------------------------------------------------
	アイテムの移動
---------------------------------------------------------*/

/* 個々のアイテムのステータスデーター */
//typedef struct
//{
/*hatudan_register_speed65536*/
//	/*hatudan_register_tra65536 共用 */#define ITEM_DATA_flag_first 	user_data01 	/* firstフラグ */

//????#define ITEM_DATA_02_flags00		user_data02 	/* (user_data02==hatudan_register_spec_data)収集フラグ(hatudan_register_spec_dataと合わせる必要がある) */
#define ITEM_DATA_02_flags00		user_data03 	/* (user_data02==hatudan_register_spec_data)収集フラグ(hatudan_register_spec_dataと合わせる必要がある) */

#define ITEM_DATA_04_y_sum256		user_data04 				/* アイテム投げ出し用 y軸 積算値(y軸、上方がマイナス) */
#define ITEM_DATA_04_count256		ITEM_DATA_04_y_sum256		/* 自動収拾用カウンタ(共用) */
//efine ITEM_DA TA_time_in			user_data04 	/* 出現時間 */ /*	自動収集 */
//} ITEM_DATA;
#define ITEM_DATA_07_true_y256		user_data07 	/* 本来のy座標 */

/*---------------------------------------------------------
	アイテムの移動(自動収集の場合)
---------------------------------------------------------*/
	#if 0
	if (JYUMYOU_NASI < src->jyumyou)/* あれば */
	{
		/* 画面内に変換 */
		src->center.y256 = src->ITEM_DATA_07_true_y256;
		#if (0)/*(自動収集の場合画面外変換処理をしない。するとおかしくなる。)*/
		#endif
	//	if (t256(0) >= src->center.y256)		/* 上にいると取れないけど速い */
		if (t256(-16) >= src->center.y256)	/* この辺の仕様はたぶん変わる。(Guスプライト座標中心管理とかで) */
	//	if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->center.y256)	/* 中心座標なので */
		{
			src->center.y256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* 画面外表示アイテム高さの半分 */
			src->obj_type_set |= 0x08;
		}
		else
		{
			src->obj_type_set &= (~0x08);
		}
		reflect_sprite_spec(src, OBJ_BANK_SIZE_01_ITEM);
	}
	#endif
		#define FIX_LABEL_ITEM_HEIGHT_DIV2 (8/2)/* 画面外表示アイテム高さの半分 */
static OBJ_CALL_FUNC(gamen_gai_item_sub)	/* 自動収集ならば、自分に集まる */
{
	if (JYUMYOU_NASI < src->jyumyou)/* あれば */
	{
		/* 画面内に変換 */
		src->center.y256 = src->ITEM_DATA_07_true_y256;
//		if (t256(0) >= src->center.y256)		/* 上にいると取れないけど速い */
		if (t256(-16) >= src->center.y256)	/* この辺の仕様はたぶん変わる。(Guスプライト座標中心管理とかで) */
//		if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->center.y256)	/* 中心座標なので */
		{	/* 画面外の場合 */
			src->obj_type_set |= (0x08);
			/* 距離に応じて透明度が変わる。(距離が遠いと暗くなる) */
			int toumeido;
			toumeido = (src->center.y256);
		//	toumeido += t256(16);
		//	toumeido = ( (toumeido));
			/* 0 ... */
			toumeido >>= 7; 	/*(>>8だと面白くない)*/
			#if 0/*(デバッグ)*/
			cg.game_score = toumeido;/*[ スコア欄にデバッグ値表示 ]*/
			#endif
			toumeido = 0xff + ((0x1000)>>7) + toumeido;
			toumeido = psp_max(0x10, toumeido);/*(0以下になるので必ず要る。)*/
		//	toumeido = psp_min(0xff, toumeido);/*(無くて良い筈だが、念の為)*/
			src->color32		= (0x00ffffff) | ((toumeido)<<24);
		//
			src->center.y256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* 画面外表示アイテム高さの半分 */
			src->atari_hantei	= (0);/*(取れない)*/
		}
		else
		{	/* 画面内の場合 */
			src->obj_type_set &= (~0x08);
			/* 念の為、半透明を白に戻す。 */
			src->color32		= (0xffffffff);
			#if (1)/*(てすと)*/
			src->atari_hantei	= (1);/*(取れる)*/
			#else
			src->atari_hantei	= (0);/*(取れない)*/
			#endif
		}
		reflect_sprite_spec(src, OBJ_BANK_SIZE_01_ITEM);
	}
}
/*---------------------------------------------------------
	アイテムの移動(自動収集の場合)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_item_type02)	/* 自動収集ならば、自分に集まる */
{
	{
		src->ITEM_DATA_04_count256--;
		if (0 <= src->ITEM_DATA_04_count256)/*0レジスタ使う*/
		{
			;
		}
		else
	//	if (1 > src->ITEM_DATA_04_count256)
		{
			src->ITEM_DATA_04_count256 = (0);
		}
	//	if (0 == src->ITEM_DATA_flag_first)
	//	{
	//		src->ITEM_DATA_flag_first = (1);
	//		src->ITEM_DATA_04_count256 = t256(2.0/*1.0*/);	/* pspは解像度が低いので細工(x2) */ 	/* (2.5==5*0.5) */
	//	}
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_00_PLAYER];
		//#define ITEM_DATA_05_x_sa256		user_data05 	/* 差分 x */ /* 自動収集 */
		//#define ITEM_DATA_06_y_sa256		user_data06 	/* 差分 y */ /* 自動収集 */
		s32 src_ITEM_DATA_05_x_sa256 = (src->center.x256 - zzz_player->center.x256);
		s32 src_ITEM_DATA_06_y_sa256 = (src->ITEM_DATA_07_true_y256 - zzz_player->center.y256);
		/* 自機狙い角を HATSUDAN_03_angle65536 に作成 */
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->ITEM_DATA_07_true_y256));
		calculate_jikinerai();
		/* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
	//	HATSUDAN_03_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));
		/* 自分に集まる */
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((src_ITEM_DATA_05_x_sa256 * src->ITEM_DATA_04_count256)>>9/*8*/);	/*fps_factor*/	/* pspは解像度が低いので細工(/2) */
		aaa_y256 = ((src_ITEM_DATA_06_y_sa256 * src->ITEM_DATA_04_count256)>>9/*8*/);	/*fps_factor*/	/* pspは解像度が低いので細工(/2) */
		if ((SP_ITEM_06_HOSI) == (src->obj_type_set))	/* 星点のみ特別処理 */
		{
			if (
				//	(/*10*/64/*16*/ > data->y_sum256) ||
				(
					#if 1
					/* [矩形判定] プレイヤを中心として、縦横15x15の領域(左右7[pixel], 上下7[pixel]) */
					(t256(8) > abs(aaa_x256)) &&
					(t256(8) > abs(aaa_y256))
					#else
					/* [菱形判定] プレイヤを中心として、半径(?)10[pixel]の菱形領域<>21x21 */
					(t256(11) > (abs(aaa_x256)+abs(aaa_y256)))
					#endif
				)
			)
			{
				src->jyumyou = JYUMYOU_NASI;/* 星点のみ特別処理 */				/* おしまい */
//
				voice_play(VOICE05_BONUS, TRACK06_ITEM);/* テキトー */
			}
		}
//		src->center.x256					= zzz_player->center.x256 + (aaa_x256); /*fps_factor*/
//		src->ITEM_DATA_07_true_y256 = zzz_player->center.y256 + (aaa_y256); /*fps_factor*/
			#if (0)//
			src->math_vector.x256 = ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/	/* CCWの場合 */
			src->math_vector.y256 = ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256); /* 「1周が65536分割」から「1周が1024分割」へ変換する。 */
				src->center.x256						+= ((sin_value_t256*(/*t256*/(5)))/*>>8*/);/*fps_factor*/
				src->ITEM_DATA_07_true_y256 	+= ((cos_value_t256*(/*t256*/(5)))/*>>8*/);/*fps_factor*/
			}
			#endif
	}
	gamen_gai_item_sub(src);
}
/*
[矩形判定] ○ + ● + ◎ の領域でアイテム回収
[菱形判定] ■ + ● + ◎ の領域でアイテム回収
□□□□□□□□□□■□□□□□□□□□□
□□□□□□□□□■■■□□□□□□□□□
□□□□□□□□■■■■■□□□□□□□□
□□□○○○○●●●●●●●○○○○□□□
□□□○○○●●●●●●●●●○○○□□□
□□□○○●●●●●●●●●●●○○□□□
□□□○●●●●●●●●●●●●●○□□□
□□□●●●●●●●●●●●●●●●□□□
□□■●●●●●●●●●●●●●●●■□□
□■■●●●●●●●●●●●●●●●■■□
■■■●●●●●●●◎●●●●●●●■■■
□■■●●●●●●●●●●●●●●●■■□
□□■●●●●●●●●●●●●●●●■□□
□□□●●●●●●●●●●●●●●●□□□
□□□○●●●●●●●●●●●●●○□□□
□□□○○●●●●●●●●●●●○○□□□
□□□○○○●●●●●●●●●○○○□□□
□□□○○○○●●●●●●●○○○○□□□
□□□□□□□□■■■■■□□□□□□□□
□□□□□□□□□■■■□□□□□□□□□
□□□□□□□□□□■□□□□□□□□□□
*/

/*---------------------------------------------------------
	アイテムの移動(通常時その２)
	-------------------------------------------------------
	アイテム落下速度について:
	-------------------------------------------------------
	アイテム落下速度はゲームバランスに関わるが、
	アイテム発生確率自体調整されていない事もあるし、
	雑魚パターン(dat)もちゃんと作ってないから、
	現状あまり調整されていない。
	-------------------------------------------------------
	アイテム落下速度は原作に比べて異常に遅く設定されている。(のには理由がある)
	(模倣風はpspの解像度が低いので、基本的に原作の 75% の速度をイメージして、
	それに近くなるように設定しているが、それより極端に遅い(さらに半分ぐらい)という意味。)
	-------------------------------------------------------
	これは、pspの縦幅が(272ドットしかなく)少な過ぎる為の処置。
	原作並みの速度の問題点は主に下記の２点。
	１．足が一番遅い、幽々子はまったく取れない。(ので面白くない気がした)
	２．何のアイテムが出たか解りにくい。(pspは残像が酷いので読めない)
	-------------------------------------------------------
	原作では、キャラによって落下速度が違うから、ここで変えても良いんだが、
	元々激遅なので、あんま効果がない気もする。(速くすると上記の問題点が顕著になる)
---------------------------------------------------------*/
		#if (0)
		/* 変更5=>4=>3 t256(3.0) */
	//	if (src->ITEM_DATA_04_y_sum256 < t256(1.5) )	/* t256(2.2) アイテム落下、最大速度の調整 */
		if (src->ITEM_DATA_04_y_sum256 < t256(2.0) )	/* t256(2.2) アイテム落下、最大速度の調整 */
		/* t256(2.2) == コンティニュー復活時の[F]を画面の下隅(右下、左下)で死んだ場合で、
			足が一番遅い、幽々子が(高速モードならば)余裕で取れる速度に調整する。
			(低速モードでは取れなくても構わない) */
		#endif

	/*
		霊夢Aの速度について。
		紅で霊夢Aの速度は意外と速いのです。
		--------------------
		アイテムを画面最上部で出します。
		(妖精が見える前に妖精を画面外で倒します)
		高さが画面中程で待機して待ちます。
		アイテムが霊夢Aの真横に落下してきます。
		この時アイテムの速度は最大速度です。
		アイテムは一定速度以上加速しません。
		この落下アイテムと霊夢Aで、競争してみます。
		--------------------
		紅を普通に遊んでいるとアイテムの最大落下速度と
		霊夢Aの移動速度が同じ位に感じるのですが、
		実験してみると、アイテムの最大落下速度より
		霊夢Aの移動速度の方が速いのです。
		ある程度巧い人でないと、測定できないかもしれませんが、
		アイテムが最大落下速度になった時点(画面少し上部)から競争を始めると、
		画面最下部の時点で。大体1キャラ分ぐらい(16[pixel]ぐらい)速いです。
		--------------------
		模倣風では縦が異常に狭いので、どういう調整をすべきか
		課題がたくさんあります。
		--------------------
		つまり、紅では「一つのアイテムに絞れば必ず取れる」
		という調整がされているのに対し、
		模倣風では、縦が異常に狭いので、見た目線速を合わせると、
		「一つのアイテムに絞っても必ず取れない」
		という調整にしかなりません。
		「一つのアイテムに絞れば必ず取れる」という調整にする為には、
		縦が異常に狭いので、アイテムの見た目線速が倍以上遅くしないと取れません。
	*/
static OBJ_CALL_FUNC(move_item_type01)
{
	/* 自動収集モードのどれかが作動してたら、 */
	if (0 != (
		(cg.state_flag) &
		(JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU 			|		/* MAX時の上回収 */
		 JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU				|		/* ボム発動中のみ回収 */
		 JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU			/* ボス撃破後の自動収集 */
		) ))
	{
		src->ITEM_DATA_02_flags00 |= (ITEM_MOVE_FLAG_01_COLLECT);	/*1*/	/* そのアイテムは自動収集にする */
	}
	/* 自動収集でなければ、単純に放物線を描いて落ちる */
	if (0 == ((src->ITEM_DATA_02_flags00)&ITEM_MOVE_FLAG_01_COLLECT))
	{
	//	#define ITEM_DATA_03_angle1024		user_data03
	//	obj->ITEM_DATA_03_angle1024 		= (6*2);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
		#define src_ITEM_DATA_03_angle1024		(6*2)
		src->ITEM_DATA_04_y_sum256 += ((src_ITEM_DATA_03_angle1024)>>1);	/* x1.5 */
	#if 0
		static const s32 rakka_sokudo[(PLAYERS8)] =
		/* あまり速いとアイテムが取りきれないので、ミス後の復活速度が遅くなる。
		(あえて工夫しないと全部は取れないくらいに調整する。
		パターンを工夫すると殆ど取れるようになってる。
		ただし、これは主にゲーム序盤(～3面)の話で、後半(4面～)は含まない。)
		*/
	/* 高速モード(通常時) */					/* 高速モード(ボム発動時) */
//	#define SPEED_HIGH_REIMU_A		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
//	#define SPEED_HIGH_REIMU_B		(0x02f1)	/* t256(2.94) 2.94140625		  t256(3.0), */
//	#define SPEED_HIGH_MARISA_A 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
//	#define SPEED_HIGH_MARISA_B 	(0x03a3)	/* t256(3.64) 3.63671875		  t256(2.0), */
//	#define SPEED_HIGH_REMILIA		(0x0347)	/* t256(3.28) 3.27734375		  t256(5.0), */
//	#define SPEED_HIGH_YUYUKO		(0x02c7)	/* t256(2.78) 2.77734375		  t256(5.0), */
//	#define SPEED_HIGH_CIRNO_A		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 	/* ⑨だから低速の方が速い */
//	#define SPEED_HIGH_CIRNO_Q		(0x0373)	/* t256(3.45) 3.44921875		  t256(4.5), */ 	/* ⑨だから低速の方が速い */
		// 0x02e6==t256(2.90) 2.8984375
		{//(r35)
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* REIMU_A */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* REIMU_B */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(2.5),		/* MARISA_A */	/* 遅すぎる。 魔理沙は仕様上、遅くする。 (r35- t256(2.75)) (-r34 t256(2.0) ) */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(2.75), 	/* MARISA_B */	/* レイ魔理沙は速くしよう。 (r35- t256(2.75)) (-r34 t256(2.0) ) */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* REMILIA */
			t256(2.0),		//(r35) t256(2.0),		//(-r34)	t256(2.0),		/* YUYUKO */	/* 幽々子は足が遅いので、遅くする。 */
			(0x02e6),		//(r35) t256(2.90), 	//(-r34)	t256(3.0),		/* CIRNO_A */
			t256(4.0),		//(r35) t256(4.0),		//(-r34)	t256(4.0),		/* CIRNO_Q */
		};
		int rakka_sokudo_maximum = rakka_sokudo[((cg_game_select_player))];
	#else
	//	int rakka_sokudo_maximum = (0x02e6);/*(psp縦無いので総合的に考えて、速い気がする)*/
	//	int rakka_sokudo_maximum = (0x02c0);/*(-r35u1 幽々子より遅い)*/
		int rakka_sokudo_maximum = (0x0280);/*(r35u2-)*/
	#endif
		/* レミリア & 幽々子 の場合、低速移動で落下速度を一時的に下げられる。 */
		if (0x04==((cg_game_select_player) & 0x06) )/* 4:REMILIA or 5:YUYUKO */
		{
			if (/*is_slow =*/ (psp_pad.pad_data & PSP_KEY_SLOW))/* 低速移動の場合 */
			{
				rakka_sokudo_maximum >>= 1;/* 一時的に半分 */
			}
		}
		if (src->ITEM_DATA_04_y_sum256 > rakka_sokudo_maximum ) 	/* t256(2.2) アイテム落下、最大速度の調整 */
		{
			src->ITEM_DATA_04_y_sum256 = rakka_sokudo_maximum;
		}
		src->ITEM_DATA_07_true_y256 += (src->ITEM_DATA_04_y_sum256);/*fps_factor*/
		if (src->ITEM_DATA_07_true_y256 > GAME_HEIGHT*256)
		{
			/* ウェポンアイテム (小P) (中P) (F) のいずれか逃したら、チェイン破棄 */
			if ((SP_ITEM_02_P128+1) > src->obj_type_set)
			{
			// SP_ITEM_00_P001: /* ウェポンアイテム(小P) */
			// SP_ITEM_01_P008: /* ウェポンアイテム(中P) */
			// SP_ITEM_02_P128: /* ウェポンアイテム(F) */
				cg.chain_point = (0);/* チェイン破棄 */
				//;
			}
			// else {;} 	/* [点][星][B][1UP]等逃しても、チェイン維持。 */
			src->jyumyou = JYUMYOU_NASI;				/* 画面外に出たらおしまい */
		}
	}
	else	/* 自動収集ならば、自分に集まる */
	{
		src->ITEM_DATA_04_count256 = t256(2.0/*1.0*/);	/* pspは解像度が低いので細工(x2) */ 	/* (2.5==5*0.5) */
		src->callback_mover 		= move_item_type02;
	//	src->ITEM_DATA_flag_first	= (0);
		src->hatudan_register_speed65536	= (0);
	}
	gamen_gai_item_sub(src);

}

/*---------------------------------------------------------
	アイテムの移動(通常時その１)
	初回時の投げ出し処理。
	投げ出し中はあたり判定なし。(回収できない)
---------------------------------------------------------*/
static OBJ_CALL_FUNC(move_item_type00)
{
	src->ITEM_DATA_04_count256--;
	if (0 <= src->ITEM_DATA_04_count256)/*0レジスタ使う*/
	{
		;
			src->center.x256	+= src->math_vector.x256;
			src->center.y256	+= src->math_vector.y256;
	}
	else
//	if (1 > src->ITEM_DATA_04_count256)
	{
		src->atari_hantei				= (1/*スコア兼用*/);/*(あたり判定on==取れる)*/
		src->ITEM_DATA_07_true_y256 	= (src->center.y256); /* 仮想 */
		src->ITEM_DATA_04_y_sum256		= (0);			/* アイテム投げ出し初期値(y軸、上方がマイナス) */
		src->callback_mover 			= move_item_type01;
	}
}

/*---------------------------------------------------------
	アイテム出現させる子関数
	-------------------------------------------------------
	-------------------------------------------------------
	ToDo:
	弾システムに移行する為、廃止予定。
	カードスクリプトシステムが稼動する為には、発弾を一ヶ所に統合しないと
	速度的な面でこのままの形態では無理があるので、
	(ここの発弾処理のせいで他の弾が処理落ちする)
	ここの発弾処理を hatudan_system_regist_single に置きかえる必要がある。
	-------------------------------------------------------
	とはいえ難しそうだな～。
---------------------------------------------------------*/
/*int x, int y*/
static OBJ *aaa_item_mono_create(OBJ/**/ *src, int set_sel_type)/* */
{
	{
		// チルノ(氷符)の場合、グレイズでパワーアップするので[小p][大P][F]は出さない。[点](氷)になる。
		// レミリアの場合[P](血)しか出さない。
		// 幽々子の場合[小p]←→[点]、[大P]←→[ボム]、が入れ替わっている。
		const u8 item_henkan[(PLAYERS8)*(8)] =
		{/* 霊夢(霊符)		霊夢(夢符)					魔理沙(魔符)				魔理沙(恋符)				レミリア(血符)				幽々子(符蝶)				チルノ(氷符)				チルノ(⑨系)			*/
(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),
(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff),
(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff),
(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),
		};
		set_sel_type = (SP_ITEM_00_P001 | (item_henkan[((set_sel_type&0x07)<<3)+(cg_game_select_player)]));
	}
	//
	//	アイテムの種類を選ぶ
	OBJ *obj;
	obj 							= obj_regist_tama();
	if (NULL != obj)
	{
		obj->obj_type_set			= set_sel_type; 	/* アイテムの種類 == グラフィックの種類 */
		obj->m_zoom.y256			= M_ZOOM_Y256_NO_TILT;/* 特殊機能で傾かないようシステム拡張(r33)。 */
		reflect_sprite_spec(obj, OBJ_BANK_SIZE_01_ITEM);
		obj->m_Hit256R				= ZAKO_ATARI16_PNG;
	//
		if (SP_ITEM_06_HOSI == set_sel_type)	/* 星点のみ特別処理 */
		{
			/* 紅は、こうらしい */
			if (USER_BOMOUT_WAIT > cg.bomber_time)
			{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
				bonus_info_score_nodel(obj, SCORE_100);/*自動消去へ仕様変更s->jyumyou = JYUMYOU_NASI;*/
			}
			else/* 星点 */
			{	/* ボム後の実質無敵期間中はこちら(稼げる) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(obj, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->jyumyou = JYUMYOU_NASI;*/
			}
//			/*当たり判定なし*/	/*(???????)*/
			obj->atari_hantei			= (ATARI_HANTEI_OFF/*スコア兼用*/);
		}
	//	else
	//	{
	//	}
	}
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
	return (obj);
}


/*---------------------------------------------------------
	アイテムを登録して出現させる
---------------------------------------------------------*/

static void s_item_convert_hosi(OBJ *obj)
{
	{
		#if (1)
		{
			int temp_cx256;
			int temp_cy256;
			temp_cx256 = obj->center.x256;
			temp_cy256 = obj->center.y256;
			/* バグるので、とりあえず対応。 */
			memset(obj, 0, sizeof(OBJ));
			obj->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);		/*	obj->alpha		= 0xff;*/
			obj->jyumyou	= JYUMYOU_1MIN; 		/* 1分したら勝手に自動消去。 */
			obj->m_zoom.x256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
			obj->m_zoom.y256			= t256(1.00);	/* 表示拡大率 256 == [ x 1.00 ] */
			#if (1)
			/*(レイヤー[0]は(r36以降)必ずdanmaku_00_standard_angle_moverなのでレイヤー[0]に強制設定。)*/
			/*(良くわかんない)*/
			obj->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)非傾き弾 */
			#endif
			obj->center.x256 = temp_cx256;
			obj->center.y256 = temp_cy256;
		}
		#endif
		obj->callback_mover 	= move_item_type01;
	//	if (SP_ITEM_06_HOSI == sel_type)	/* 星点のみ特別処理 */
		{
			/* 紅は、こうらしい */
			if (USER_BOMOUT_WAIT > cg.bomber_time)
			{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
				bonus_info_score_nodel(obj, SCORE_100);/*自動消去へ仕様変更s->jyumyou = JYUMYOU_NASI;*/
			}
			else/* 星点 */
			{	/* ボム後の実質無敵期間中はこちら(稼げる) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(obj, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->jyumyou = JYUMYOU_NASI;*/
			}
//			/*当たり判定なし*/	/*(???????)*/
//			obj->atari_hantei			= (ATARI_HANTEI_OFF/*スコア兼用*/);
		}
	}
		/* エフェクトの場合も星点になる。 */
		obj->obj_type_set = SP_ITEM_06_HOSI;
		obj->rotationCCW1024				= (0);			/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
		{
			/* エフェクトの場合半透明なので、白に戻す。 */
			obj->color32		= (0xffffffff);
			/* エフェクトの場合半透明なので、原寸に戻す。 */
		//	obj->m_zoom.x256	= t256(1.0);M_ZOOM_Y256_NO_TILTは強制1.00倍なので要らない。
		//	obj->m_zoom.y256	= t256(1.0);M_ZOOM_Y256_NO_TILTは強制1.00倍なので要らない。
			obj->m_zoom.y256	= M_ZOOM_Y256_NO_TILT;/* 特殊機能で傾かないようシステム拡張(r33)。 */
		}
		obj->ITEM_DATA_07_true_y256 	= (/*src*/obj->center.y256); /* 仮想 */
		obj->ITEM_DATA_02_flags00	= /*(DANMAKU_LAYER_03)|*/((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
		//
		reflect_sprite_spec(obj, OBJ_BANK_SIZE_01_ITEM);
}


/*---------------------------------------------------------
	総ての敵弾を、hosiアイテムに変える
---------------------------------------------------------*/
extern void item_convert_hosi(OBJ */*src*/h);
global void bullets_to_hosi(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_1024_MAX; ii++)/* 全部調べる。 */
	{
		OBJ *obj;
		obj = &obj99[OBJ_HEAD_00_0x0000_TAMA+ii];
		if (JYUMYOU_NASI < (obj->jyumyou))/* あれば */
		{
			if (SP_ITEM_06_HOSI == obj->obj_type_set)
			{
				/*(r36-既に[星点]なら変換しない)*/;
			}
			else
			if (OBJ_Z04_TAMA & obj->obj_type_set)
			{
			//	item_create(obj, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			//	obj->jyumyou = JYUMYOU_NASI;
			//	obj->type = SP_IT EM_06_HOSI;
				s_item_convert_hosi(obj);//, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			}
		}
	}
}


/*---------------------------------------------------------
	アイテムを登録して出現させる
---------------------------------------------------------*/
	#if 0
	if ((CIRNO_A-1) < (cg_game_select_player))/* 6:CIRNO_A or 7:CIRNO_Q */
	{
	//	if (item_type<SP_ITEM_02_P128)		// [小p][大P]	 は出ない。
		if (item_type<(SP_ITEM_02_P128+1))	// [小p][大P][F] は出ない。
		{
			item_type = SP_ITEM_05_TENSU;
		}
	}
	else
	// レミリアの場合[点]は出さない.
	if (REMILIA==(cg_game_select_player))
	{
		if (item_type==(SP_ITEM_05_TENSU))	// [点] は出ない。
		{
			item_type = SP_ITEM_00_P001;	// [小p]
		}
	}
	#endif

/* up_flags の ITEM_MOVE_FLAG_01_COLLECT ビットがオンでプレイヤーに集まります。 */
static void item_create_flags(
	OBJ *src,
	int item_type,
	int set_item_flags
)
{
	{
		OBJ *h;
		h			= aaa_item_mono_create(src, item_type);
		#if 1
		if (NULL==h) return;
		#else
		if (NULL != h)
		#endif
		{
			h->ITEM_DATA_07_true_y256		= (src->center.y256); /* 仮想 */

			s32 x_offset;
			x_offset = 0;
			/*(WAITモードなら)*/
			if (set_item_flags & ITEM_MOVE_FLAG_02_WAIT)
			{
				h->ITEM_DATA_04_count256	= (64)/*(32)*/;
				h->atari_hantei 		= (ATARI_HANTEI_OFF/*スコア兼用*/);
				h->callback_mover		= move_item_type00;
				//------------------
			u32 my_rand;
				my_rand = ra_nd();
			s32 rnd_spd256				= (my_rand & 0x7f/*angCCW512*/) + t256(0.75);
				HATSUDAN_01_speed256	= ((rnd_spd256));/*(t256形式)*/
				HATSUDAN_03_angle65536	= (my_rand & ((65536/4)-1)) + (65536/4) + (65536/8);
				sincos256();/*(破壊レジスタ多いので注意)*/
				h->math_vector.x256 = REG_03_DEST_Y;//sin_value_t256	// 縦
				h->math_vector.y256 = REG_02_DEST_X;//cos_value_t256	// 横
			}
			else/*(通常モード)*/
			{
				h->callback_mover		= move_item_type01;
				/*(リプレイ時に再現性が必要)*/
				#if 0
				/* 基本的に画面外(上)にあまり喰み出さない(旧タイプ) */
				/* アイテムマーカーを作成しない事が前提の動き */
				h->ITEM_DATA_04_y_sum256		= -(/*256*/((src->center.y256)>>7))-(ra_nd()&0xff); /* アイテム投げ出し初期値(y軸、上方がマイナス) */
				#else
				/* 画面外(上)に喰み出す(喰み出しマーカー必須) */
				/* アイテムマーカーを作成する事が前提の動き */
			//	h->ITEM_DATA_04_y_sum256		= -(/*256*/((200*256)>>7))-(ra_nd()&0xff);		/* アイテム投げ出し初期値(y軸、上方がマイナス) */
				h->ITEM_DATA_04_y_sum256		= -(/*256*/((512)))-(ra_nd()&0xff); 			/* アイテム投げ出し初期値(y軸、上方がマイナス) */
				#endif
				{
				//	出現位置を決める
					#define OFFSET_X64		(64*256)/* 64はずらし分 2のn乗の必要有り */
					#define ITEM_WIDTH16	(16*256)/* 16はアイテム幅 */
					#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
					s32 x256;
					x256 = (src->center.x256);
					x256 += ((ra_nd()&((OFFSET_X64+OFFSET_X64)-1)));	/*(リプレイ時に再現性が必要)*/
							if (x256 < OFFSET_X64)			{		x256 = OFFSET_X64;			}
					else	if (x256 > (ITEM_X_LIMIT))		{		x256 = (ITEM_X_LIMIT);		}
				//	登録する
					x_offset	= (x256)-(OFFSET_X64) - (src->center.x256);
				}
			}
			h->center.x256					= src->center.x256 + (x_offset);
			h->center.y256					= src->center.y256;
			h->ITEM_DATA_02_flags00 			= /*(DANMAKU_LAYER_03)|*/(set_item_flags & ITEM_MOVE_FLAG_01_COLLECT);
		}
	}
}
global void item_create_flags_num(
	OBJ *src,
	int item_type,
	int num_of_creates,
	int set_item_flags
)
{
	/*unsigned*/ int ii;
	for (ii=0; ii<num_of_creates; ii++)
	{
//		item_create_flags(src, item_type, (ITEM_MOVE_FLAG_06_RAND_XY) );
		item_create_flags(src, item_type, (set_item_flags) );
	}
}




global void item_create_mono(
	OBJ *src,
	int item_type
)
{
	item_create_flags(src, item_type, (ITEM_MOVE_FLAG_06_RAND_XY) );
}

/*---------------------------------------------------------
	ボス用アイテム作成
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
global void item_create_for_boss(OBJ/**/ *src, int item_create_mode)
{
	#define ITEM_03 	(4*0)
	#define ITEM_02 	(4*1)
	#define ITEM_01_1	(4*2)
	#define ITEM_01_2	(4*3)
	#define ITEM_01_3	(4*4)
	#define ITEM_01_4	(4*5)
	#define ITEM_01_5	(4*6)
	#define ITEM_01_6	(4*7)
	#define ITEM_MAX	(4*8)
	const u8 u8_item_tbl[ITEM_MAX*2] =
	{	/* easy, normal, hard, lunatic */
	#if 0
	/* 旧 */
	/*mode 0: 撃破後 */
		(SP_ITEM_03_1UP&7), 	(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),
		(SP_ITEM_02_P128&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage3*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage4*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/
	/*mode 1: 撃破中 */
		(SP_ITEM_04_BOMB&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),
		(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage3*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage4*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/
	#else
	/* 新(テスト中) */
	/*mode 0: 撃破後 */ 	/* easy, normal, hard, lunatic */
		(SP_ITEM_03_1UP&7), 	(SP_ITEM_03_1UP&7), 	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	/* (もしstage7まで作るとしたらstage7クリアー後(クリアー後なのでゲーム的には意味無い)もここを共用する) */
		(SP_ITEM_03_1UP&7), 	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/	/*alice*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/	/*mima*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	/*stage3*/	/*kaguya*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	/*stage4*/	/*aya*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/	/*pache*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/	/*sakuya*/		/*---- sakuya撃破後はゲーム的には意味無い */
	/*mode 1: 撃破中 */
		(SP_ITEM_04_BOMB&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),
		(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage1*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage3*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage4*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage5*/
		(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage6*/
	#endif
	};
//	item_create_mono(src, item_tbl[ITEM_03+difficulty+item_create_mode]/*SP_ITEM_03_1UP */ );
//	item_create_mono(src, item_tbl[ITEM_02+difficulty+item_create_mode]/*SP_ITEM_02_P128 */ );
//	item_create(src, item_tbl[ITEM_01+difficulty+item_create_mode]/*SP_ITEM_01_P008 */, cg.game_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	unsigned int i;
	for (i=0; i<(1)+((((unsigned)cg.game_now_stage>>1))&0x0f); i++)
	{
		item_create_mono(src, OBJ_Z03_ITEM + u8_item_tbl[item_create_mode+(((cg.game_difficulty)+i)&((ITEM_MAX)-1))]/*SP_ITEM_03_1UP */ );
	}
	for (i=0; i<(16); i++)/*(7)*/
	{
		item_create_flags(src, SP_ITEM_05_TENSU, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*星点を出す*/
	}
}





/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	ランダムアイテム敵の場合に使う、共通発生テーブル
---------------------------------------------------------*/

static unsigned int random_item_seed;		/*=0*/

global void teki_random_item_table_initialize(void) 	/*int set_seed*/
{	/*(リプレイ時に再現性が必要)*/
	random_item_seed = (ra_nd()&(8-1)); 	/*set_seed*/
}
static int s_teki_get_random_item(void)
{
	static const u16/*int*/ item_table[32] =
	{
		SP_ITEM_01_P008,	SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
		SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,
		SP_ITEM_00_P001,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_05_TENSU,
		SP_ITEM_00_P001,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,	SP_ITEM_00_P001,
		SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_05_TENSU,	SP_ITEM_00_P001,
	};
	random_item_seed++; 	random_item_seed &= 0x1f;
	return (item_table[random_item_seed]);
}


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

global /*static*/ OBJ_CALL_FUNC(item_create_random_table)
{
	item_create_mono(src, s_teki_get_random_item());//, (1), (/*IT EM_MOVE_FLAG_01_COLLECT|*/ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/
}








/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	難易度スコア補正
---------------------------------------------------------*/

global u32 adjust_score_by_difficulty(u32 convert_score)
{
	u32 mul_tbl256[4] = /* 難易度補正 */
	{
//	/* easy */		t256(0.5),	/*原作風*/
//	/* normal */	t256(1.0),	/*原作風*/
//	/* hard */		t256(1.2),	/*原作風*/
//	/* lunatic */	t256(1.5),	/*原作風*/
	/* easy */		t256(0.5),	/*模倣風*/
	/* normal */	t256(1.0),	/*模倣風*/
	/* hard */		t256(2.0),	/*模倣風*/
	/* lunatic */	t256(5.0),	/*模倣風*/
	};
	return (((convert_score)*(mul_tbl256[(cg.game_difficulty)]))>>8);
}

/*
○各種カウンターストップ
　得点のカンスト　　９９億９９９９万９９９０点
　かすり回数　　　　９９９９９回
　プレイヤー数　　　９人
　カード回数　９９９９回
*/
/*---------------------------------------------------------
	スコア加算されると、必ずここが呼ばれる。
	ここでエクステンドチェック(残機が得点で増えるチェック)を
	する予定。(だけど、まだ作っていない)
	-------------------------------------------------------
	エクステンドチェックは実行時間が読めないのはマズい
	約1秒毎に必ずチェックにいく方式。
---------------------------------------------------------*/
//#define PLAYER_MAX_SCORE	(score( 9999999990))
#define PLAYER_MAX_SCORE	(	  ( 999999999 ))

global void player_dummy_add_score(u32 score_num_pts)
{
	cg.game_score += score_num_pts;
	/* カンスト(スコアカウンター ストップ)チェックも約1秒(60[frame])に1回で
		内部的には問題ないんだけど、表示が変になると思うよ。 */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* カンスト(スコアカウンター ストップ)チェック */
	if (PLAYER_MAX_SCORE < cg.game_score) /* カンスト チェック */
	{
		cg.game_score = PLAYER_MAX_SCORE;
	}
	#endif /* (1==USE_MAX_SCORE_COUNTER_STOP_CHECK) */
}



#if (1==USE_EXTEND_CHECK)
static u32 extend_check_score;
static int extend_check_counter;
#endif /* (1==USE_EXTEND_CHECK) */
#if (1==USE_EXTEND_CHECK)
/* score_panel.c: エクステンドチェックは約1秒(60[frame])に1回で問題ないと思う。 */
global void player_check_extend_score(void)
{
	/* 1000万、2500万、5000万、10000万(1億)でエクステンド */
	#if (1==USE_EXTEND_CHECK)
	if (extend_check_score < cg.game_score) /* エクステンド チェック */
	{
		cg.zanki++; 	/* エクステンド */
		/*
			スコアによるエクステンド音は特殊なので、目立つべき。
			(アイテムによるエクステンド音と違って、目で確認しない)
			実際やってみたら２つ鳴らして、丁度良い。
			特にうるさくはなかった。
		*/
		voice_play(VOICE06_EXTEND, TRACK03_IVENT_DAN/*TRACK02_JIKI_BOMBER*/);		/* エクステンド音 */
		voice_play(VOICE06_EXTEND, TRACK01_PICHUN);/*予備(必要)*/					/* エクステンド音 */
		{
			/*
			★ 模倣風で 1000万 エクステンドが無いのは、
			1000万 エクステンドがあると 1面でエクステンドしてしまう。
			1面でエクステンドすると、却ってやる気を削ぐ。
			-------------------------------------------------
			★ 2500万 エクステンドが、あるのは、
			普通のノーミスなら 2面で確実に(2500万点以上になるので)エクステンドする。
			1面でコンティニューしている場合は、多分 2面で2500万点にならないので、エクステンドしない。
			ノーミスでエクステンドすれば、少しやる気が出るのでは？という配慮。
			-------------------------------------------------
			★ この辺はdatのスコア配分のバランスで変わるが、かなり前からそういうスコア配分バランス。
			*/
			static const u32 extend_score_tbl[8] =
			{
				0x7fffffff, 			/* エクステンド ストッパー(これ以上エクステンドしない) */
			//	score(	 25000000),/*( 2500万[pts])*/	/* 設定値の意味: 普通のノーミスなら 2面で確実にextend. (1面では無理だと思う) */
			//	score(	 50000000),/*( 5000万[pts])*/	/* 設定値の意味: 稼げば 2面でextend.(難易度にもよる)  */
			//	score(	100000000),/*(1億[pts])*/		/* 模倣風では 8000万 あたりの方が良い気がする。 */
			// バランス下げてみる。
				score(	 25000000),/*( 2500万[pts])*/	/* 設定値の意味: (1面では無理だと思う)	*/
				score(	 40000000),/*( 4000万[pts])*/	/* 設定値の意味: (4000万 = 1500万 + 2500万)  */
				score(	 60000000),/*( 6000万[pts])*/	/* 設定値の意味: (6000万 = 2000万 + 4000万)  */
				//
				score(	 90000000),/*( 9000万[pts])*/	/* 設定値の意味: (9000万 = 3000万 + 6000万)  */
				score(	130000000),/*(1.3億[pts])*/ 	/* 設定値の意味: (1.3億  = 4000万 + 9000万)  */
				score(	180000000),/*(1.8億[pts])*/ 	/* 設定値の意味: (1.8億  = 5000万 + 1.3億)	*/
				score(	240000000),/*(2.4億[pts])*/ 	/* 設定値の意味: (2.4億  = 6000万 + 1.8億)	*/
			};
			//	score(	 10000000),
			//	score( 9999999990), 	/*==0x3b9ac9ff*/
			//	score(21474836470), 	/*==0x7fffffff*/
			//	score(42949672950), 	/*==0xffffffff*/
			extend_check_counter++;
			extend_check_counter &= (8-1);
			extend_check_score = extend_score_tbl[extend_check_counter];
		}
	}
	#endif /* (1==USE_EXTEND_CHECK) */
}
#endif
#if (1==USE_EXTEND_CHECK)


/*---------------------------------------------------------
	プレイヤー開始時／プレイヤーコンティニュー開始時に呼ばれる。
---------------------------------------------------------*/
global void player_init_extend_score(void)
{
	extend_check_score		= score(   10000000);
	extend_check_counter	= 0;
}
#endif /* (1==USE_EXTEND_CHECK) */
