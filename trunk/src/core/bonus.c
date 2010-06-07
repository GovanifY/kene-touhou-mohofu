
/*---------------------------------------------------------
	東方模倣風  ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アイテム関連
---------------------------------------------------------*/

#include "game_main.h"

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
		src->ITEM_DATA_x_sa256 = (src->x256 - obj_player->x256);
		src->ITEM_DATA_y_sa256 = (src->ITEM_DATA_true_y256 - obj_player->y256);
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
				src->type = SP_DELETE;/* 星点のみ特別処理 */				/* おしまい */
//
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
			}
		}
		src->x256 = obj_player->x256 + (aaa_x256);	/*fps_factor*/
		src->ITEM_DATA_true_y256 = obj_player->y256 + (aaa_y256);	/*fps_factor*/
	}
	if (SP_DELETE != src->type)
	{
		/* 画面内に変換 */
		src->y256 = src->ITEM_DATA_true_y256;
		#if 0
		if (t256(0) >= src->y256)		/* 上にいると取れないけど速い */
		#else
		if (t256(-16) >= src->y256) 	/* この辺の仕様はたぶん変わる。(Guスプライト座標中心管理とかで) */
		#endif
		{
			src->y256 = 0;
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
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
---------------------------------------------------------*/

static void move_item_type01(SPRITE *src)
{
	/* 自動収集モードのどれかが作動してたら、 */
	if (0 != (
		(pd_state_flag) &
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
		/* [***090123 [***090220	変更5=>4=>3 t256(3.0) */
	//	if (src->ITEM_DATA_y_sum256 < t256(1.5) )	/* t256(2.2) アイテム落下、最大速度の調整 */
		if (src->ITEM_DATA_y_sum256 < t256(2.0) )	/* t256(2.2) アイテム落下、最大速度の調整 */
		/* t256(2.2) == コンティニュー復活時の[F]を画面の下隅(右下、左下)で死んだ場合で、
			足が一番遅い、幽々子が(高速モードならば)余裕で取れる速度に調整する。
			(低速モードでは取れなくても構わない) */
		{
			src->ITEM_DATA_y_sum256 += ((src->ITEM_DATA_angle1024)>>1); 	/* x1.5 */
		}
		src->ITEM_DATA_true_y256 += (src->ITEM_DATA_y_sum256);/*fps_factor*/
		if (src->ITEM_DATA_true_y256 > GAME_HEIGHT*256)
		{
			/* ウェポンアイテム (小P) (中P) (F) のいずれか逃したら、チェイン破棄 */
			switch (src->type)
			{
			case SP_ITEM_00_P001:	/* ウェポンアイテム(小P) */
			case SP_ITEM_01_P008:	/* ウェポンアイテム(中P) */
			case SP_ITEM_02_P128:	/* ウェポンアイテム(F) */
				pd_chain_point = 0;
				break;
			/* [点][星][B][1UP]等逃しても、チェイン維持。 */
			}
			src->type = SP_DELETE;				/* 画面外に出たらおしまい */
		}
	}
	else	/* 自動収集ならば、自分に集まる */
	{
		src->callback_mover 		= move_item_type02;
		src->ITEM_DATA_flag_first	= 0;
	}
	if (SP_DELETE != src->type)
	{
		/* 画面内に変換 */
		src->y256 = src->ITEM_DATA_true_y256;
		#if 0
		if (t256(0) >= src->y256)		/* 上にいると取れないけど速い */
		#else
		if (t256(-16) >= src->y256) 	/* この辺の仕様はたぶん変わる。(Guスプライト座標中心管理とかで) */
		#endif
		{
			src->y256 = 0;
			src->type |= 0x08;
		}
		else
		{
			src->type &= (~0x08);
		}
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
static SPRITE *item_mono_create(SPRITE *src, int sel_type)/*  */
{
	//	アイテムの種類を選ぶ
	SPRITE *h;
	h						= sprite_add_only_bullet_error();
	if (NULL != h)
	{
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		{
		//	出現位置を決める
			#define OFFSET_X64		(64*256)/* 64はずらし分 2のn乗の必要有り */
			#define ITEM_WIDTH16	(16*256)/* 16はアイテム幅 */
			#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
			int x256;
			x256 = src->x256;
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
			h->x256 				= (x256)-(OFFSET_X64);
		}
		h->y256 					= src->y256;

		h->callback_mover			= move_item_type01;
	//
		sel_type &= 0x07;		/* アイテムの種類 == グラフィックの種類 */
		sel_type |= SP_ITEM_00_P001;
		h->type 				= sel_type;
	//
		if ( SP_ITEM_06_HOSI == sel_type )	/* 星点のみ特別処理 */
		{
			/* 紅は、こうらしい */
			if ( USER_BOMOUT_WAIT > pd_bomber_time )
			{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
				bonus_info_score_nodel(h, SCORE_100);/*自動消去へ仕様変更s->type = SP_DELETE;*/
			}
			else/* 星点 */
			{	/* ボム後の実質無敵期間中はこちら(稼げる) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((pd_graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->type = SP_DELETE;*/
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
			if ( USER_BOMOUT_WAIT > pd_bomber_time )
			{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
				bonus_info_score_nodel(h, SCORE_100);/*自動消去へ仕様変更s->type = SP_DELETE;*/
			}
			else/* 星点 */
			{	/* ボム後の実質無敵期間中はこちら(稼げる) */
				/* ((graze/3)*10)+(500) pts */
				bonus_info_any_score_nodel(h, (score(500)+(((pd_graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->type = SP_DELETE;*/
			}
//			h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*当たり判定なし*/
		}
	}
		h->type = SP_ITEM_06_HOSI;
		h->ITEM_DATA_true_y256		= (/*src*/h->y256); /* 仮想 */

//		h->ITEM_DATA_angle1024		= (2*6);	/* 2*6.51898646904403967309077986986488 cv1024r(0.08);*/
		h->ITEM_DATA_flags00		= ((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
//	}
	h->m_angleCCW1024				= (0);			/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
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
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* 全部調べる。 */
	{
		SPRITE *s;
		s = &sprite_444pool[ii];
	//
		#if 1/* 要らない気がする(てすとちう) */
		if (s->type != SP_DELETE ) /* 削除済みは飛ばす */
		#endif
		{
			if (SP_GROUP_BULLETS & s->type)
			{
			//	item_create(s, /*put_item_num*/SP_IT EM_06_HOSI/*SP_IT EM_06_HOSI*/, 1, (IT EM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );
			//	s->type = SP_DELETE;
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
			h->ITEM_DATA_true_y256		= (src->y256);	/* 仮想 */
			#if 0
			/* 基本的に画面外(上)にあまり喰み出さない(旧タイプ) */
			/* アイテムマーカーを作成しない事が前提の動き */
			h->ITEM_DATA_y_sum256		= -(/*256*/((src->y256)>>7))-(ra_nd()&0xff);	/* アイテム投げ出し初期値(y軸、上方がマイナス) */
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
//	item_create(src, item_tbl[ITEM_01+difficulty+item_create_mode]/*SP_ITEM_01_P008 */, player_now_stage/*5*/, ITEM_MOVE_FLAG_06_RAND_XY);
	int i;
	for (i=0; i<(player_now_stage+2); i++)
	{
		item_create(src, SP_GROUP_ITEMS+u8_item_tbl[difficulty+item_create_mode+i]/*SP_ITEM_03_1UP	*/, 1, ITEM_MOVE_FLAG_06_RAND_XY);
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

global void enemy_set_random_seed(void) 	/*int set_seed*/
{
	random_item_seed = (ra_nd()&(8-1)); 	/*set_seed*/
}
static int s_enemy_get_random_item(void)
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
	item_create(src, s_enemy_get_random_item(), (1), (/*IT EM_MOVE_FLAG_01_COLLECT|*/ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
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
	return (((convert_score)*(mul_tbl256[difficulty]))>>8);
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

global void player_add_score(u32 score_num_pts)
{
	pd_my_score += score_num_pts;
	/* カンスト(スコアカウンター ストップ)チェックも約1秒(60flame)に1回で
		内部的には問題ないんだけど、表示が変になると思うよ。 */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* カンスト(スコアカウンター ストップ)チェック */
	if ( PLAYER_MAX_SCORE < pd_my_score )	/* カンスト チェック */
	{
		pd_my_score = PLAYER_MAX_SCORE;
	}
	#endif /* (1==USE_MAX_SCORE_COUNTER_STOP_CHECK) */
}

#if (1==USE_EXTEND_CHECK)
static int extend_check_score;
static int extend_check_counter;
#endif /* (1==USE_EXTEND_CHECK) */
#if (1==USE_EXTEND_CHECK)
/* score_panel.c: エクステンドチェックは約1秒(60flame)に1回で問題ないと思う。 */
global void player_check_extend_score(void)
{
	/* 1000万、2500万、5000万、10000万(1億)でエクステンド */
	#if (1==USE_EXTEND_CHECK)
	if ( extend_check_score < pd_my_score ) 	/* エクステンド チェック */
	{
		pd_zanki++; 	/* エクステンド */
		/*
			スコアによるエクステンド音は特殊なので、目立つべき。
			(アイテムによるエクステンド音と違って、目で確認しない)
			実際やってみたら２つ鳴らして、丁度良い。
			特にうるさくはなかった。
		*/
		voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		/* エクステンド音 */
		voice_play(VOICE06_EXTEND, TRACK01_EXPLODE);/*予備(必要)*/						/* エクステンド音 */
		{
			static const unsigned int extend_score_tbl[4/*8*/] =
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
