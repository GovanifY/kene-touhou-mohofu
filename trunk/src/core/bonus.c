
/*---------------------------------------------------------
	アイテム関連
---------------------------------------------------------*/

#include "game_main.h"
#include "bonus.h"

/*---------------------------------------------------------
	アイテムの移動
---------------------------------------------------------*/

/* 個々のアイテムのステータスデーター */
//typedef struct
//{
#define ITEM_DATA_true_y256 	user_data00 	/* 本来のy座標 */
#define ITEM_DATA_y_sum256		user_data01 	/* アイテム投げ出し用 y軸 積算値(y軸、上方がマイナス) */
#define ITEM_DATA_angle512		user_data02
#define ITEM_DATA_flags00		user_data03 	/* 収集フラグ	[***090116	変更 */
/*	自動収集 */
#define ITEM_DATA_flag_first	user_data04 	/* firstフラグ */
#define ITEM_DATA_x_sa256		user_data05 	/* 差分 x */
#define ITEM_DATA_y_sa256		user_data06 	/* 差分 y */
//} ITEM_DATA;

static void move_items(SPRITE *src)
{
//	ITEM_DATA *data = (ITEM_DATA *)src->data;
	/* 自動収集モードのどれかが作動してたら、 */
	if (0 != (
		(((PLAYER_DATA *)player->data)->state_flag) &
		(STATE_FLAG_01_PLAYER_UP_AUTO_GET_ITEM |		/* MAX時の上回収 */
		 STATE_FLAG_02_BOMB_AUTO_GET_ITEM | 			/* ボム発動中のみ回収 */
		 STATE_FLAG_03_SCORE_AUTO_GET_ITEM				/* ボス撃破時の一時回収 */
		) ))
	{
		src->ITEM_DATA_flags00 |= ITEM_MOVE_FLAG_01_COLLECT/*1*/;	/* そのアイテムは自動収集にする */
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
			src->ITEM_DATA_y_sum256 += src->ITEM_DATA_angle512; 	/* x1.5 */
		}
		src->ITEM_DATA_true_y256 += (src->ITEM_DATA_y_sum256)/**fps_fa ctor*/;
		if (src->ITEM_DATA_true_y256 > GAME_HEIGHT*256)
		{
			/* ウェポンアイテム (小P) (中P) (F) のいずれか逃したら、チェイン破棄 */
			switch (src->type)
			{
			case SP_ITEM_00_P001:	/* ウェポンアイテム(小P) */
			case SP_ITEM_01_P008:	/* ウェポンアイテム(中P) */
			case SP_ITEM_02_P128:	/* ウェポンアイテム(F) */
				((PLAYER_DATA *)player->data)->chain_point = 0;
				break;
			/* [点][星][B][1UP]等逃しても、チェイン維持。 */
			}
			src->type = SP_DELETE;				/* 画面外に出たらおしまい */
		}
	}
	else	/* 自動収集ならば、自分に集まる */
	{
		src->ITEM_DATA_y_sum256--;
		if (1 > src->ITEM_DATA_y_sum256)
		{
			src->ITEM_DATA_y_sum256 = 0;
		}
		if (0 == src->ITEM_DATA_flag_first)
		{
			src->ITEM_DATA_flag_first = 1;
			src->ITEM_DATA_y_sum256 = t256(4.0/*1.0*/); 	/* pspは解像度が低いので細工(x4) */ 	/* (2.5==5*0.5) */
		}
		src->ITEM_DATA_x_sa256 = (src->x256 - player->x256);
		src->ITEM_DATA_y_sa256 = (src->ITEM_DATA_true_y256 - player->y256);
		/* 自分に集まる */
		int aaa_x256;
		int aaa_y256;
		aaa_x256 = ((src->ITEM_DATA_x_sa256 * src->ITEM_DATA_y_sum256)>>10/*8*/); /**fps_fa ctor*/	/* pspは解像度が低いので細工(/4) */
		aaa_y256 = ((src->ITEM_DATA_y_sa256 * src->ITEM_DATA_y_sum256)>>10/*8*/); /**fps_fa ctor*/	/* pspは解像度が低いので細工(/4) */
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
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE05_BONUS);
				#else
				voice_play(VOICE05_BONUS, TRACK07_GRAZE);/*テキトー*/
				#endif
			}
		}
		src->x256 = player->x256 + (aaa_x256);	/**fps_fa ctor*/
		src->ITEM_DATA_true_y256 = player->y256 + (aaa_y256);	/**fps_fa ctor*/
	}
	if (SP_DELETE != src->type)
	{
		/* 画面内に変換 */
		src->y256 = src->ITEM_DATA_true_y256;
		if (0 >= src->y256)
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
	アイテム出現させる子関数
---------------------------------------------------------*/

static SPRITE *item_mono_create(SPRITE *src/*int x, int y*/, int sel_type)
{
//	アイテムの種類を選ぶ
	SPRITE *s;
//	s			= spr ite_add_file 0("bonus_items.png", 8, PRIORITY_04_ITEM, 0);	s->anim_speed=0;
//	s			= sprite_add_res(BASE_BONUS_ITEMS_PNG);
	s			= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	sel_type &= 0x07;
//	s->yx_an im_frame = ((/*SP_ITEM_FIRE_POWER-*/sel_type)/*&0x07*/);
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
	s->x256 			= (x256)-(OFFSET_X64);
	s->y256 			= src->y256;
//	sel_type &= 0x07;
	sel_type |= SP_ITEM_00_P001;
	s->type 			= sel_type;
	s->callback_mover	= move_items;
	if ( SP_ITEM_06_HOSI == sel_type )	/* 星点のみ特別処理 */
	{
		/* 紅は、こうらしい */
	//	if ( USER_BOMOUT_WAIT > ((PLAYER_DATA *)player->data)->bomber_time )
		if ( USER_BOMOUT_WAIT > pd_bomber_time )
		{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
			bonus_info_score_nodel(s, SCORE_100);/*自動消去へ仕様変更s->type = SP_DELETE;*/
		}
		else/* 星点 */
		{	/* ボム後の実質無敵期間中はこちら(稼げる) */
			/* ((graze/3)*10)+(500) pts */
			bonus_info_any_score_nodel(s, (score(500)+(((((PLAYER_DATA *)player->data)->graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->type = SP_DELETE;*/
		}
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*当たり判定なし*/
	}
	else
	{
		s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	}
	return (s);
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
}


/*---------------------------------------------------------
	アイテムを登録して出現させる
---------------------------------------------------------*/

/* [***090125	追加: up_flags の ITEM_MOVE_FLAG_01_COLLECT ビットがオンでプレイヤーに集まります。 */
void item_convert_hosi(
	SPRITE */*src*/h/*int x, int y*/  //,
//	int item_type,
//	int num_of_creates,
//	int up_flags
)
{
//	int i;
//	for (i=0; i<num_of_creates; i++)
//	{
//		SPRITE *h;
//		h			= item_mono_create(src, item_type);
//		if (NULL==h) return;
//
{
//	アイテムの種類を選ぶ
//	SPRITE *s;
//	s			= spr ite_add_file 0("bonus_items.png", 8, PRIORITY_04_ITEM, 0);	s->anim_speed=0;
//	s			= sprite_add_res(BASE_BONUS_ITEMS_PNG);
//	s			= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
//	sel_type &= 0x07;
//	s->yx_an im_frame = ((/*SP_ITEM_FIRE_POWER-*/sel_type)/*&0x07*/);
//	出現位置を決める
	#define OFFSET_X64		(64*256)/* 64はずらし分 2のn乗の必要有り */
	#define ITEM_WIDTH16	(16*256)/* 16はアイテム幅 */
	#define ITEM_X_LIMIT	(GAME_WIDTH*256+OFFSET_X64-ITEM_WIDTH16)
	int x256;
	x256 = /*src*/h->x256;
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
	h->x256 			= (x256)-(OFFSET_X64);
	h->y256 			= /*src*/h->y256;
//	sel_type &= 0x07;
//	sel_type |= SP_ITEM_00_P001;
//	h->type 			= sel_type;
	h->callback_mover	= move_items;
//	if ( SP_ITEM_06_HOSI == sel_type )	/* 星点のみ特別処理 */
	{
		/* 紅は、こうらしい */
	//	if ( USER_BOMOUT_WAIT > ((PLAYER_DATA *)player->data)->bomber_time )
		if ( USER_BOMOUT_WAIT > pd_bomber_time )
		{	/* ボム中(設定無敵時間中)は100pts.(稼げない) */
			bonus_info_score_nodel(h, SCORE_100);/*自動消去へ仕様変更s->type = SP_DELETE;*/
		}
		else/* 星点 */
		{	/* ボム後の実質無敵期間中はこちら(稼げる) */
			/* ((graze/3)*10)+(500) pts */
			bonus_info_any_score_nodel(h, (score(500)+(((((PLAYER_DATA *)player->data)->graze_point*86)>>8)/*(pd->graze_point/3)*/)) );/*自動消去へ仕様変更c->type = SP_DELETE;*/
		}
		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);/*当たり判定なし*/
	}
//	else
//	{
//		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	}
//	return (h);
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
}
	//	ITEM_DATA *data;
	//	data				= mmal loc(sizeof(ITEM_DATA));
	//	h->data 			= data;
		h->type = SP_ITEM_06_HOSI;
		h->ITEM_DATA_true_y256		= (/*src*/h->y256); /* 仮想 */

//		h->ITEM_DATA_angle512		= 6;	/* 6.51898646904403967309077986986488 ra d2deg512(0.08);*/
		h->ITEM_DATA_flags00		= ((ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)&ITEM_MOVE_FLAG_01_COLLECT);
		h->ITEM_DATA_flag_first = 0;
//	}
	h->m_angleCCW512	= 0;			/* 描画用角度(下が0度で左回り(反時計回り)) */	/* 0 == 傾かない。下が0度 */
}


/*---------------------------------------------------------
	アイテムを登録して出現させる
---------------------------------------------------------*/

/* [***090125	追加: up_flags の ITEM_MOVE_FLAG_01_COLLECT ビットがオンでプレイヤーに集まります。 */
void item_create(
	SPRITE *src/*int x, int y*/,
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
		if (NULL==h) return;
//
	//	ITEM_DATA *data;
	//	data				= mmal loc(sizeof(ITEM_DATA));
	//	h->data 			= data;
		h->ITEM_DATA_true_y256		= (src->y256);	/* 仮想 */

		#if 0
		/* 基本的に画面外(上)にあまり喰み出さない(旧タイプ) */
		/* アイテムマーカーを作成しない事が前提の動き */
		h->ITEM_DATA_y_sum256		= -(/*256*/((src->y256)>>7))-(ra_nd()&0xff);	/* アイテム投げ出し初期値(y軸、上方がマイナス) */
		#else
		/* 画面外(上)に喰み出す(が、喰み出しマーカー作ってない) */
		/* アイテムマーカーを作成する事が前提の動き */
	//	h->ITEM_DATA_y_sum256		= -(/*256*/((200*256)>>7))-(ra_nd()&0xff);		/* アイテム投げ出し初期値(y軸、上方がマイナス) */
		h->ITEM_DATA_y_sum256		= -(/*256*/((512)))-(ra_nd()&0xff); 			/* アイテム投げ出し初期値(y軸、上方がマイナス) */
		#endif
		h->ITEM_DATA_angle512		= 6;	/* 6.51898646904403967309077986986488 ra d2deg512(0.08);*/
		h->ITEM_DATA_flags00		= (up_flags&ITEM_MOVE_FLAG_01_COLLECT);
		h->ITEM_DATA_flag_first = 0;
	}
}

/*---------------------------------------------------------
	ボス用アイテム作成
---------------------------------------------------------*/
//	#define ITEM_CREATE_MODE_01 	(12*0)
//	#define ITEM_CREATE_MODE_02 	(12*1)
void item_create_for_boss(SPRITE *src, int item_create_mode)
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
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_05_TENSU&7),	/*stage2*/	/*aya*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_01_P008&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	/*stage3*/	/*mitei*/
		(SP_ITEM_00_P001&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_04_BOMB&7),	(SP_ITEM_01_P008&7),	/*stage4*/	/*kaguya*/
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
	item_create(src, SP_ITEM_05_TENSU/*SP_IT EM_06_HOSI*/, 16/*7*/, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );/*星点を出す*/
}


/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/


/*---------------------------------------------------------
	スコア関連(取った得点を小さな漢字で説明表示)
---------------------------------------------------------*/

/* スコアキャッシュ最大4096文字 */
#define MAX_SCORE_CHACHE (0x1000)

/* 非表示の識別 */
#define SCORE_DELETE (0)

/* スコアキャッシュのデーター形式 */
typedef struct
{
	int time_out;	/* 表示時間 / 表示アルファ値 */
	int number; 	/* 表示文字番号(0 ... 9) [一桁の数字] */
	int x256;		/* 表示 x 座標(256固定小数点形式) */
	int y256;		/* 表示 y 座標(256固定小数点形式) */
} SCORE_CHACHE_DATA;

static SCORE_CHACHE_DATA score_number_chache[MAX_SCORE_CHACHE];


/*---------------------------------------------------------
	スコアキャッシュすべて消す
---------------------------------------------------------*/

void clear_score_chache(void)
{
	int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		score_number_chache[i].time_out = SCORE_DELETE;
	}
}


/*---------------------------------------------------------
	sdl_screen[SDL_00_SCREEN]サーフェイスに、スコアをレンダリング
---------------------------------------------------------*/

//	   void font07_render_scorenum_xy( int time_out, int number, int x256, int y256)
//atic void font07_render_scorenum_xy(SCORE_CHACHE_DATA *aaa){}
void draw_score_chache(void)
{
//				SDL_Rect s,d;
//				s.w=(8); d.w=(8);
//				s.h=(8); d.h=(8);
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); }/*ロックする*/
	int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		SCORE_CHACHE_DATA *aaa;
		aaa = &(score_number_chache[i]);
		if (SCORE_DELETE < aaa->time_out)
		{
			aaa->time_out -= 2;
			{
			//	font07_render_scorenum_xy(
				//	(aaa->time_out),
				//	(aaa->number),
				//	(aaa->x256)/*dx*/,
				//	(aaa->y256)/*dy*/
			//		&(score_number_chache[i])
			//		);
//				s.x = ((aaa->number)<<3)+(8);
//				s.y = 0;
//				d.x = (t256_floor(aaa->x256));
//				d.y = (t256_floor(aaa->y256))/*+((aaa->time_out)>>5) 上に移動*/;
//				#if 0
//				/*遅い*/
//				SDL_SetAlpha(font07_img,SDL_SRCALPHA,(aaa->time_out));
//				#endif
//				SDL_BlitSurface(font07_img,&s,sdl_screen[SDL_00_SCREEN],&d);
				Uint16 alpha256;	alpha256 = (aaa->time_out);

				Uint16 *pd;
				{
					Uint32 y2562 = ((aaa->y256)&0x1ff00);
					pd = (Uint16 *)sdl_screen[SDL_00_SCREEN]->pixels + (y2562+y2562) + (t256_floor(aaa->x256));/*x 512/256 */
				}
				int iii;	iii = (aaa->number);
				int yyy;
				for (yyy=0; yyy<6; yyy++)
				{
					#define v (0)
					#define M (1)
					#define f8(a,b,c,d, e,f,g,h) ((a<<0)|(b<<1)|(c<<2)|(d<<3)|(e<<4)|(f<<5)|(g<<6)|(h<<7))
					static const Uint8 score_font_08x05[/*(6*10)*/(64)] =
					{
f8(v,v,v,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,M,M,M,M,M,v,v),f8(v,v,v,M,v,v,v,v),f8(v,M,v,v,v,v,v,v),f8(v,v,M,M,v,v,v,v),f8(v,v,M,v,v,v,v,v),
f8(v,v,M,v,v,M,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,M,M,M,M,v,v,v),f8(M,M,M,M,M,M,M,v),f8(v,v,v,M,v,v,v,v),f8(v,M,M,M,M,M,M,v),f8(v,M,v,v,M,M,M,v),f8(v,v,v,M,M,v,v,v),f8(v,v,M,M,M,M,v,v),
f8(v,M,v,v,v,v,M,v),f8(v,v,v,v,v,v,v,v),f8(v,v,M,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,v,M,v,M,v,M,v),f8(v,v,M,M,M,M,v,v),f8(v,v,v,v,v,v,v,v),f8(M,M,M,M,M,v,v,v),f8(v,v,v,v,M,v,v,v),f8(M,M,M,v,v,M,v,v),
f8(v,M,v,v,v,v,M,v),f8(v,M,M,M,M,M,M,v),f8(v,v,v,v,v,v,v,v),f8(v,v,M,M,v,v,v,v),f8(M,M,M,v,M,M,M,v),f8(v,v,M,v,v,M,v,v),f8(v,v,M,v,v,M,v,v),f8(v,M,v,v,v,v,v,v),f8(v,M,v,v,M,M,v,v),f8(v,v,M,v,M,M,v,v),
f8(v,v,M,v,v,M,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,v,v,v,v,v,M,v),f8(v,M,M,v,M,M,v,v),f8(v,M,M,v,v,M,M,v),f8(v,M,v,v,v,v,v,v),f8(v,M,v,v,v,M,M,v),f8(v,M,M,v,M,v,v,v),
f8(v,v,v,M,M,v,v,v),f8(v,v,v,v,v,v,v,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,M,M,M,M,M,v),f8(M,M,v,v,v,v,M,M),f8(v,v,M,M,M,M,M,v),f8(M,v,v,v,v,v,M,M),f8(M,M,v,v,v,M,M,M),
						/*dummy*/0,0,0,0
					};
					#undef v
					#undef M
					#undef f8
					{
						const Uint8 ddd = score_font_08x05[iii];
						int kkk;
						kkk = 1;
						int xxx;
						for (xxx=0; xxx<8; xxx++)
						{
							if (ddd & (kkk) )
							{	/* 汎用転送(アルファ任意) */
							#define bgRGB (*pd)
							#define fgRGB (0xffff)
							(*pd) = (Uint16)MAKECOL16(
								(((GETR16F(fgRGB) * (alpha256)) + (GETR16F(bgRGB) * (256 - alpha256))) >> 8),
								(((GETG16F(fgRGB) * (alpha256)) + (GETG16F(bgRGB) * (256 - alpha256))) >> 8),
								(((GETB16F(fgRGB) * (alpha256)) + (GETB16F(bgRGB) * (256 - alpha256))) >> 8)	);
							}
							pd++;
							kkk += kkk;
						}
					}
					pd += (512-8);
					iii += 10;
				}
			}
		}
	}
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}/*ロック解除*/
}


/*---------------------------------------------------------
	一桁の数字を、キャッシュに登録する
---------------------------------------------------------*/

static void regist_score(int number, int x256, int y256)
{
	if (t256((GAME_WIDTH-8)/*(380)*/) < x256)	{ return; } 	/* (8)? (あまり横なら描かない) */
	if (t256((GAME_HEIGHT-8)/*(380)*/) < y256)	{ return; } 	/* (6)? (あまり下なら描かない) */
//
	static int index=0; 	/* 登録出来そうな位置 */
//	int iii;
//	iii = 0;	/* 最大登録数まで全部探す */
//	do
//	{
		index++;
		index &= (MAX_SCORE_CHACHE-1);
		/* 使用中 */
//		if (SCORE_DELETE < score_number_chache[index].time_out)
//		{
//			;	/* 登録できないので次を探す。 */
//		}
//		/* 未使用 */
//		else //if (1 > score_number_chache[index].time_out)
		{
			/* キャッシュに登録する */
			score_number_chache[index].time_out = (127*2);/*	60*2*2 =:= 2 [sec]*/
			score_number_chache[index].number	= number;
			score_number_chache[index].x256 	= x256;
			score_number_chache[index].y256 	= y256;
//			iii = MAX_SCORE_CHACHE; 	/* 見つかったよ */
		}
//		iii++;
//	} while (MAX_SCORE_CHACHE > iii);	/* 全部探した？ */
}


/*---------------------------------------------------------
	スコアを一桁の数字に分解し、キャッシュに登録する
---------------------------------------------------------*/

static void bonus_info_shered_add_score10_value(SPRITE *src, int score_value)
{
	int y256;	y256 = src->y256;
	if (t256((0)) > y256)	{ return; }
//
	int x256;	x256 = src->x256;
	int jjj;
	int i;
	jjj=0;
	/* 加算スコアが 999990点 以上の場合は 999990点 の表示にする(表示のみ999990点で実際は、ちゃんとその分加算される) */
	if (99999 < score_value) {	score_value = 99999;  }   /* MAX 999990[pts] */
	i = 0; while ( 9999 < score_value) { score_value -= 10000; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	999 < score_value) { score_value -=  1000; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	 99 < score_value) { score_value -=   100; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	  9 < score_value) { score_value -=    10; i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	i = 0; while (	  0 < score_value) { score_value--; 	   i++; jjj=1; }; if (0!=jjj) { regist_score(i, x256, y256); }	x256 += t256(8);
	regist_score(0, x256, y256);/* "0" スコアの末尾は必ず０ */
}


/*---------------------------------------------------------
	取ったアイテムの得点を説明表示(任意得点の場合)
---------------------------------------------------------*/

void bonus_info_any_score_nodel(SPRITE *src/*int x, int y*/, int score_num)
{
	player_add_score(score_num);
	bonus_info_shered_add_score10_value(src, score_num);
}


/*---------------------------------------------------------
	取ったアイテムの得点を説明表示(固定得点の場合)
---------------------------------------------------------*/

void bonus_info_score_nodel(SPRITE *src/*int x, int y*/, int score_type)
{
	static const unsigned short score_tbl[32] =
	{
		score(76800), score(   10), score(	 20), score(   30),
		score(	 40), score(   50), score(	 60), score(   70),
		score(	 80), score(   90), score(	100), score(  200),
		score(	300), score(  400), score(	500), score(  600),
		//
		score(	700), score(  800), score(	900), score( 1000),
		score( 2000), score( 3000), score( 4000), score( 5000),
		score( 6000), score( 7000), score( 8000), score( 9000),
		score(10000), score(11000), score(12000), score(51200),
	};
	int score_num;	score_num = score_tbl[(score_type)/*&(32-1)*/];
	bonus_info_any_score_nodel(src, score_num);
}


/*---------------------------------------------------------
	難易度スコア補正
---------------------------------------------------------*/

int adjust_score_by_difficulty(int convert_score)
{
	int mul_tbl256[4] = /* 難易度補正 */
	{
	/* easy */		t256(0.5),
	/* normal */	t256(1.0),
	/* hard */		t256(1.2),
	/* lunatic */	t256(1.5),
	};
	return (((convert_score)*(mul_tbl256[difficulty]))>>8);
}


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

void player_add_score(int score_num)
{
	((PLAYER_DATA *)player->data)->my_score += score_num;
	/* カンスト(スコアカウンター ストップ)チェックも約1秒(60flame)に1回で
		内部的には問題ないんだけど、表示が変になると思うよ。 */
	#if (1==USE_MAX_SCORE_COUNTER_STOP_CHECK)
	/* カンスト(スコアカウンター ストップ)チェック */
	if ( PLAYER_MAX_SCORE < ((PLAYER_DATA *)player->data)->my_score )	/* カンスト チェック */
	{
		((PLAYER_DATA *)player->data)->my_score = PLAYER_MAX_SCORE;
	}
	#endif /* (1==USE_MAX_SCORE_COUNTER_STOP_CHECK) */
}

#if (1==USE_EXTEND_CHECK)
static int extend_check_score;
static int extend_check_counter;
#endif /* (1==USE_EXTEND_CHECK) */
#if (1==USE_EXTEND_CHECK)
/* score_panel.c: エクステンドチェックは約1秒(60flame)に1回で問題ないと思う。 */
void player_check_extend_score(void)
{
	/* 1000万、2500万、5000万、10000万(1億)でエクステンド */
	#if (1==USE_EXTEND_CHECK)
	if ( extend_check_score < ((PLAYER_DATA *)player->data)->my_score ) 	/* エクステンド チェック */
	{
		((PLAYER_DATA *)player->data)->zanki++; 	/* エクステンド */
		#if (0==USE_DESIGN_TRACK)
		play_voice_auto_track(VOICE06_EXTEND);		/* エクステンド音 */
		#else
		/*
			スコアによるエクステンド音は特殊なので、目立つべき。
			(アイテムによるエクステンド音と違って、目で確認しない)
			実際やってみたら２つ鳴らして、丁度良い。
			特にうるさくはなかった。
		*/
		voice_play(VOICE06_EXTEND, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);
		voice_play(VOICE06_EXTEND, TRACK01_EXPLODE);/*予備(必要)*/
		#endif
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
void player_init_extend_score(void)
{
	extend_check_score		= score(   10000000);
	extend_check_counter	= 0;
}
#endif /* (1==USE_EXTEND_CHECK) */
