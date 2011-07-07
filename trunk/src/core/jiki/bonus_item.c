
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アイテム関連
---------------------------------------------------------*/

/*---------------------------------------------------------
	アイテムの移動
---------------------------------------------------------*/

/* 個々のアイテムのステータスデーター */
//typedef struct
//{
#define ITEM_DATA_true_y256 	user_data00 	/* 本来のy座標 */
#define ITEM_DATA_y_sum256		user_data01 	/* アイテム投げ出し用 y軸 積算値(y軸、上方がマイナス) */
#define ITEM_DATA_angle1024 	user_data02
#define ITEM_DATA_flags00		user_data03 	/* 収集フラグ	[***090116	変更 */
/*	自動収集 */
#define ITEM_DATA_time_in		user_data04 	/* 出現時間 */
#define ITEM_DATA_x_sa256		user_data05 	/* 差分 x */
#define ITEM_DATA_y_sa256		user_data06 	/* 差分 y */
//} ITEM_DATA;
/* 共用 */
#define ITEM_DATA_flag_first	ITEM_DATA_time_in	/* firstフラグ */


/*---------------------------------------------------------
	アイテムの移動(自動収集の場合)
---------------------------------------------------------*/

static void move_item_type02(SPRITE *src)	/* 自動収集ならば、自分に集まる */
{
	{
		src->ITEM_DATA_y_sum256--;
		if (0 <= src->ITEM_DATA_y_sum256)/*0レジスタ使う*/
		{
			;
		}
		else
	//	if (1 > src->ITEM_DATA_y_sum256)
		{
			src->ITEM_DATA_y_sum256 = 0;
		}
		if (0 == src->ITEM_DATA_flag_first)
		{
			src->ITEM_DATA_flag_first = 1;
			src->ITEM_DATA_y_sum256 = t256(2.0/*1.0*/); 	/* pspは解像度が低いので細工(x2) */ 	/* (2.5==5*0.5) */
		}
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		src->ITEM_DATA_x_sa256 = (src->cx256 - zzz_player->cx256);
		src->ITEM_DATA_y_sa256 = (src->ITEM_DATA_true_y256 - zzz_player->cy256);
		/* 自分に集まる */
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((src->ITEM_DATA_x_sa256 * src->ITEM_DATA_y_sum256)>>9/*8*/);	/*fps_factor*/	/* pspは解像度が低いので細工(/2) */
		aaa_y256 = ((src->ITEM_DATA_y_sa256 * src->ITEM_DATA_y_sum256)>>9/*8*/);	/*fps_factor*/	/* pspは解像度が低いので細工(/2) */
		if ( (SP_ITEM_06_HOSI) == (src->type))	/* 星点のみ特別処理 */
		{
			if (
				//	(/*10*/64/*16*/ > data->y_sum256) ||
				(
					#if 1
					/* [矩形判定] プレイヤを中心として、縦横15x15の領域(左右7[dot], 上下7[dot]) */
					(t256(8) > abs(aaa_x256)) &&
					(t256(8) > abs(aaa_y256))
					#else
					/* [菱形判定] プレイヤを中心として、半径(?)10[dot]の菱形領域<>21x21 */
					(t256(11) > (abs(aaa_x256)+abs(aaa_y256)))
					#endif
				)
			)
			{
				src->jyumyou = JYUMYOU_NASI;/* 星点のみ特別処理 */				/* おしまい */
//
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
			}
		}
		src->cx256 = zzz_player->cx256 + (aaa_x256);	/*fps_factor*/
		src->ITEM_DATA_true_y256 = zzz_player->cy256 + (aaa_y256);	/*fps_factor*/
	}
//	if (SP_DELETE != src->type)
	if (JYUMYOU_NASI < src->jyumyou)/* あれば */
	{
		#define FIX_LABEL_ITEM_HEIGHT_DIV2 (8/2)/* 画面外表示アイテム高さの半分 */
		/* 画面内に変換 */
		src->cy256 = src->ITEM_DATA_true_y256;
	//	if (t256(0) >= src->cy256)		/* 上にいると取れないけど速い */
		if (t256(-16) >= src->cy256)	/* この辺の仕様はたぶん変わる。(Guスプライト座標中心管理とかで) */
	//	if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->cy256) 	/* 中心座標なので */
		{
			src->cy256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* 画面外表示アイテム高さの半分 */
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
		reflect_sprite_spec444(src, OBJ_BANK_SIZE_01_ITEM);
	}
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
		/* [***090123 [***090220	変更5=>4=>3 t256(3.0) */
	//	if (src->ITEM_DATA_y_sum256 < t256(1.5) )	/* t256(2.2) アイテム落下、最大速度の調整 */
		if (src->ITEM_DATA_y_sum256 < t256(2.0) )	/* t256(2.2) アイテム落下、最大速度の調整 */
		/* t256(2.2) == コンティニュー復活時の[F]を画面の下隅(右下、左下)で死んだ場合で、
			足が一番遅い、幽々子が(高速モードならば)余裕で取れる速度に調整する。
			(低速モードでは取れなくても構わない) */
		#endif


static void move_item_type01(SPRITE *src)
{
	/* 自動収集モードのどれかが作動してたら、 */
	if (0 != (
		(cg.state_flag) &
		(STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM |		/* MAX時の上回収 */
		 STATE_FLAG_02_BOMB_AUTO_GET_ITEM | 			/* ボム発動中のみ回収 */
		 STATE_FLAG_03_SCORE_AUTO_GET_ITEM				/* ボス撃破時の一時回収 */
		) ))
	{
		src->ITEM_DATA_flags00 |= (ITEM_MOVE_FLAG_01_COLLECT);	/*1*/	/* そのアイテムは自動収集にする */
	}
	/* 自動収集でなければ、単純に放物線を描いて落ちる */
	if (0 == ((src->ITEM_DATA_flags00)&ITEM_MOVE_FLAG_01_COLLECT))
	{
		src->ITEM_DATA_y_sum256 += ((src->ITEM_DATA_angle1024)>>1); 	/* x1.5 */
		static const s32 rakka_sokudo[(8)] =
		/* あまり速いとアイテムが取りきれないので、ミス後の復活速度が遅くなる。
		(あえて工夫しないと全部は取れないくらいに調整する。
		パターンを工夫すると殆ど取れるようになってる。
		ただし、これは主にゲーム序盤(～3面)の話で、後半(4面～)は含まない。)
		*/
		{
			t256(3.0),	/* REIMU_A */
			t256(3.0),	/* REIMU_B */
			t256(2.0),	/* MARISA_A */	/* 魔理沙は仕様上、遅くする。 */
			t256(2.0),	/* MARISA_B */	/* 魔理沙は仕様上、遅くする。 */
			t256(3.0),	/* REMILIA */
			t256(2.0),	/* YUYUKO */	/* 幽々子は足が遅いので、遅くする。 */
			t256(3.0),	/* CIRNO_A */
			t256(4.0),	/* CIRNO_Q */
		};
		int rakka_sokudo_maximum = rakka_sokudo[((cg_game_select_player))];
		/* レミリア & 幽々子 の場合、低速移動で落下速度を一時的に下げられる。 */
		if (0x04==((cg_game_select_player) & 0x06) )/* 4:REMILIA or 5:YUYUKO */
		{
			if (/*is_slow =*/ (cg_my_pad & PSP_KEY_SLOW))/* 低速移動の場合 */
			{
				rakka_sokudo_maximum >>= 1;/* 一時的に半分 */
			}
		}
		if (src->ITEM_DATA_y_sum256 > rakka_sokudo_maximum )	/* t256(2.2) アイテム落下、最大速度の調整 */
		{
			src->ITEM_DATA_y_sum256 = rakka_sokudo_maximum;
		}
		src->ITEM_DATA_true_y256 += (src->ITEM_DATA_y_sum256);/*fps_factor*/
		if (src->ITEM_DATA_true_y256 > GAME_HEIGHT*256)
		{
			/* ウェポンアイテム (小P) (中P) (F) のいずれか逃したら、チェイン破棄 */
			if ((SP_ITEM_02_P128+1) > src->type)
			{
			// SP_ITEM_00_P001: /* ウェポンアイテム(小P) */
			// SP_ITEM_01_P008: /* ウェポンアイテム(中P) */
			// SP_ITEM_02_P128: /* ウェポンアイテム(F) */
				cg.chain_point = 0;/* チェイン破棄 */
				//;
			}
			// else {;} 	/* [点][星][B][1UP]等逃しても、チェイン維持。 */
			src->jyumyou = JYUMYOU_NASI;				/* 画面外に出たらおしまい */
		}
	}
	else	/* 自動収集ならば、自分に集まる */
	{
		src->callback_mover 		= move_item_type02;
		src->ITEM_DATA_flag_first	= 0;
	}
//	if (SP_DELETE != src->type)
	if (JYUMYOU_NASI < src->jyumyou)/* あれば */
	{
		/* 画面内に変換 */
		src->cy256 = src->ITEM_DATA_true_y256;
//		if (t256(0) >= src->cy256)		/* 上にいると取れないけど速い */
		if (t256(-16) >= src->cy256)	/* この辺の仕様はたぶん変わる。(Guスプライト座標中心管理とかで) */
//		if (t256(FIX_LABEL_ITEM_HEIGHT_DIV2) >= src->cy256) 	/* 中心座標なので */
		{
			src->cy256 = t256(FIX_LABEL_ITEM_HEIGHT_DIV2);/* 画面外表示アイテム高さの半分 */
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
		reflect_sprite_spec444(src, OBJ_BANK_SIZE_01_ITEM);
	}
}

/*---------------------------------------------------------
	アイテムの移動(通常時その１)
	初回時の投げ出し処理。
	投げ出し中はあたり判定なし。(回収できない)
---------------------------------------------------------*/
#if 0
static void move_item_type00(SPRITE *src)
{
}
#endif

/*---------------------------------------------------------
	アイテム出現させる子関数
---------------------------------------------------------*/
/*int x, int y*/
static SPRITE *item_mono_create(SPRITE *src, int sel_type)/* */
{
	//	アイテムの種類を選ぶ
	SPRITE *h;
	h						= obj_add_00_tama_error();
	if (NULL != h)
	{
		sel_type &= 0x07;		/* アイテムの種類 == グラフィックの種類 */
		sel_type |= SP_ITEM_00_P001;
		h->type 				= sel_type;
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_01_ITEM);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
	//
		{
		//	出現位置を決める
			#define OFFSET_X64		(64*256)/* 64はずらし分 2のn乗の必要有り */
			#define ITEM_WIDTH16	(16*256)/* 16はアイテム幅 */
			#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
			int x256;
			x256 = src->cx256;
			x256 += ((ra_nd()&((OFFSET_X64+OFFSET_X64)-1)));
			if (x256 < OFFSET_X64)
			{
				x256 = OFFSET_X64;
			}
			else
			if (x256 > (ITEM_X_LIMIT))
			{
				x256 = (ITEM_X_LIMIT);
			}
		//	登録する
			h->cx256				= (x256)-(OFFSET_X64);
		}
		h->cy256					= src->cy256;

		h->callback_mover			= move_item_type01;
	//
	//
		if ( SP_ITEM_06_HOSI == sel_type )	/* 星点のみ特別処理 */
		{
			/* 紅は、こうらしい */
			if ( USER_BOMOUT_WAIT > cg.bomber_time )
			{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
				bonus_info_score_nodel(h, SCORE_100);/*自動消去へ仕様変更s->jyumyou = JYUMYOU_NASI;*/
			}
			else/* 星点 */
			{	/* ボム後の実質無敵期間中はこちら(稼げる) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->jyumyou = JYUMYOU_NASI;*/
			}
			h->flags			= (0/*SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);/*当たり判定なし*/
		}
	//	else
	//	{
	//	}
	}
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
	return (h);
}


/*---------------------------------------------------------
	アイテムを登録して出現させる
---------------------------------------------------------*/

static void s_item_convert_hosi(SPRITE *h)
{
	{
		h->callback_mover	= move_item_type01;
	//	if ( SP_ITEM_06_HOSI == sel_type )	/* 星点のみ特別処理 */
		{
			/* 紅は、こうらしい */
			if ( USER_BOMOUT_WAIT > cg.bomber_time )
			{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
				bonus_info_score_nodel(h, SCORE_100);/*自動消去へ仕様変更s->jyumyou = JYUMYOU_NASI;*/
			}
			else/* 星点 */
			{	/* ボム後の実質無敵期間中はこちら(稼げる) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((cg.graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->jyumyou = JYUMYOU_NASI;*/
			}
//			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*当たり判定なし*/
		}
	}
		/* エフェクトの場合も星点になる。 */
		h->type = SP_ITEM_06_HOSI;
		{
			/* エフェクトの場合半透明なので、白に戻す。 */
			h->color32		= (0xffffffff);
			/* エフェクトの場合半透明なので、原寸に戻す。 */
			h->m_zoom_x256	= t256(1.0);
			h->m_zoom_y256	= t256(1.0);
		}
		reflect_sprite_spec444(h, OBJ_BANK_SIZE_01_ITEM);
		h->ITEM_DATA_true_y256		= (/*src*/h->cy256); /* 仮想 */

//		h->ITEM_DATA_angle1024		= (2*6);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
		h->ITEM_DATA_flags00		= ((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
//	}
	h->rotationCCW1024				= (0);			/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
}


/*---------------------------------------------------------
	総ての敵弾を、hosiアイテムに変える
---------------------------------------------------------*/
//extern SPRITE *sprite_list444_head;/* 弾専用スプライト リスト */
//void item_from_bullets(int put_item_num)
extern void item_convert_hosi(SPRITE */*src*/h);
global void bullets_to_hosi(void)
{
//	SPRITE *s = sprite_list444_head;/* 弾専用スプライト リストの先頭 から探す */
//	while (NULL != s)/* スプライト リストの最後まで調べる */

	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//
	//	#if 1/* 要らない気がする(てすとちう) */
	//	if (s->type != SP_DELETE ) /* 削除済みは飛ばす */
	//	#endif
		if (JYUMYOU_NASI < s->jyumyou)/* あれば */
		{
			if (SP_GROUP_BULLETS & s->type)
			{
			//	item_create(s, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			//	s->jyumyou = JYUMYOU_NASI;
			//	s->type = SP_IT EM_06_HOSI;
				s_item_convert_hosi(s);//, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			}
//			s = s->next;/*次*/
		}
	}
}


/*---------------------------------------------------------
	アイテムを登録して出現させる
---------------------------------------------------------*/

/* [***090125	追加: up_flags の ITEM_MOVE_FLAG_01_COLLECT ビットがオンでプレイヤーに集まります。 */
global void item_create(
	SPRITE *src,/*int x, int y*/
	int item_type,
	int num_of_creates,
	int up_flags
)
{
	#if 0
	if ( (CIRNO_A-1) < ((cg_game_select_player)) )/* 6:CIRNO_A or 7:CIRNO_Q */
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
	{
		// チルノの場合、グレイズでパワーアップするので[小p][大P][F]は出さない。[点](氷)になる。
		// レミリアの場合[P](血)しか出さない。
		// 幽々子の場合[小p]←→[点]、[大P]←→[ボム]、が入れ替わっている。
		const u8 item_henkan[(8)*(8)] =
		{/* 霊夢(霊符)		霊夢(夢符)					魔理沙(魔符)				魔理沙(恋符)				レミリア(血符)				幽々子(符蝶)				チルノ(氷符)				チルノ(⑨系)			*/
(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),		(SP_ITEM_03_1UP&0xff),
(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff)	,	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_01_P008&0xff), 	(SP_ITEM_02_P128&0xff), 	(SP_ITEM_04_BOMB&0xff), 	(SP_ITEM_04_BOMB&0xff),
(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_00_P001&0xff), 	(SP_ITEM_05_TENSU&0xff),	(SP_ITEM_05_TENSU&0xff),
(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff), 	(SP_ITEM_06_HOSI&0xff),
(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),	(SP_ITEM_07_SPECIAL&0xff),
		};
		item_type = (/*SP_ITEM_00_P001*/SP_GROUP_ITEMS | (item_henkan[((item_type&0x07)<<3)+(cg_game_select_player)]));
	}
	//
	int i;
	for (i=0; i<num_of_creates; i++)
	{
		SPRITE *h;
		h			= item_mono_create(src, item_type);
		#if 1
		if (NULL==h) return;
		#else
		if (NULL != h)
		#endif
		{
			h->ITEM_DATA_true_y256		= (src->cy256); /* 仮想 */
			#if 0
			/* 基本的に画面外(上)にあまり喰み出さない(旧タイプ) */
			/* アイテムマーカーを作成しない事が前提の動き */
			h->ITEM_DATA_y_sum256		= -(/*256*/((src->cy256)>>7))-(ra_nd()&0xff);	/* アイテム投げ出し初期値(y軸、上方がマイナス) */
			#else
			/* 画面外(上)に喰み出す(喰み出しマーカー必須) */
			/* アイテムマーカーを作成する事が前提の動き */
		//	h->ITEM_DATA_y_sum256		= -(/*256*/((200*256)>>7))-(ra_nd()&0xff);		/* アイテム投げ出し初期値(y軸、上方がマイナス) */
			h->ITEM_DATA_y_sum256		= -(/*256*/((512)))-(ra_nd()&0xff); 			/* アイテム投げ出し初期値(y軸、上方がマイナス) */
			#endif
			h->ITEM_DATA_angle1024		= (6*2);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
			h->ITEM_DATA_flags00		= (up_flags&ITEM_MOVE_FLAG_01_COLLECT);
		}
	}
}

/*---------------------------------------------------------
	ボス用アイテム作成
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
global void item_create_for_boss(SPRITE *src, int item_create_mode)
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
//	item_create(src, item_tbl[ITEM_03+difficulty+item_create_mode]/*SP_ITEM_03_1UP	*/, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, item_tbl[ITEM_02+difficulty+item_create_mode]/*SP_ITEM_02_P128 */, 1, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, item_tbl[ITEM_01+difficulty+item_create_mode]/*SP_ITEM_01_P008 */, cg.game_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	int i;
	for (i=0; i<1+(((cg.game_now_stage>>1))&0x0f); i++)
	{
		item_create(src, SP_GROUP_ITEMS+u8_item_tbl[item_create_mode+(((cg_game_difficulty)+i)&((ITEM_MAX)-1))]/*SP_ITEM_03_1UP */, 1, ITEM_MOVE_FLAG_06_RAND_XY);
	}
	item_create(src, SP_ITEM_05_TENSU/*SP_IT EM_06_HOSI*/, (16)/*(7)*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*星点を出す*/
}





/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/

/*---------------------------------------------------------
	ランダムアイテム敵の場合に使う、共通発生テーブル
---------------------------------------------------------*/

static unsigned int random_item_seed;		/*=0*/

global void teki_random_item_table_initialize(void) 	/*int set_seed*/
{
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

global /*static*/ void lose_random_item(SPRITE *src)
{
	item_create(src, s_teki_get_random_item(), (1), (/*IT EM_MOVE_FLAG_01_COLLECT|*/ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
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
	return (((convert_score)*(mul_tbl256[(cg_game_difficulty)]))>>8);
}

/*
○各種カウンターストップ
　得点のカンスト　　９９億９９９９万９９９０点
　かすり回数　　　　９９９９９回
　プレイヤー数　　　９人
　スペルカード回数　９９９９回
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
	/* カンスト(スコアカウンター ストップ)チェックも約1秒(60flame)に1回で
		内部的には問題ないんだけど、表示が変になると思うよ。 */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* カンスト(スコアカウンター ストップ)チェック */
	if ( PLAYER_MAX_SCORE < cg.game_score ) /* カンスト チェック */
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
/* score_panel.c: エクステンドチェックは約1秒(60flame)に1回で問題ないと思う。 */
global void player_check_extend_score(void)
{
	/* 1000万、2500万、5000万、10000万(1億)でエクステンド */
	#if (1==USE_EXTEND_CHECK)
	if ( extend_check_score < cg.game_score )	/* エクステンド チェック */
	{
		cg.zanki++; 	/* エクステンド */
		/*
			スコアによるエクステンド音は特殊なので、目立つべき。
			(アイテムによるエクステンド音と違って、目で確認しない)
			実際やってみたら２つ鳴らして、丁度良い。
			特にうるさくはなかった。
		*/
		voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		/* エクステンド音 */
		voice_play(VOICE06_EXTEND, TRACK01_EXPLODE);/*予備(必要)*/						/* エクステンド音 */
		{
			static const u32 extend_score_tbl[4/*8*/] =
			{
				0x7fffffff, 			/* エクステンド ストッパー(これ以上エクステンドしない) */
				score(	 25000000),
				score(	 50000000),
				score(	100000000),
			};
			//	score(	 10000000),
			//	score( 9999999990), 	/*==0x3b9ac9ff*/
			//	score(21474836470), 	/*==0x7fffffff*/
			//	score(42949672950), 	/*==0xffffffff*/
			extend_check_counter++;
			extend_check_counter &= (4-1);
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
