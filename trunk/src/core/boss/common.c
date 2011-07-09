
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/

global void lose_option_00(SPRITE *src)
{
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}

/*---------------------------------------------------------
	敵やられ
---------------------------------------------------------*/
#if 0
static void lose_mima_doll(SPRITE *src)
{
//	item_create_for_boss(src, ITEM_CREATE_MODE_02);/* easyはボムを出さなくて済む位軟らかくした */
	#if 1
	item_create(src, SP_ITEM_05_TENSU, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
	#else
	{	int i;
		for (i=0; i<(5); i++)
		{
			item_create_99_random_item(src);
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------
	ボス形態変更時の共通
---------------------------------------------------------*/

//extern SPRITE *obj_effect[8]; /* [□]（スペルカードのグラフィック） */
		#if 0/* あたった敵は倒せない敵かもしれないので、ここで必ず消すのはまずい気がする */
	/* あたった敵爆発 */
	bakuhatsu_add_type(src, BAKUHATSU_MINI00);
	cg.teki->type	= SP_DELETE;/* ??? */
	bakuhatsu_add_type_ddd(src, BAKUHATSU_FIRE08);
		#endif
/*---------------------------------------------------------
	敵移動(ザコ消滅爆発エフェクト)
---------------------------------------------------------*/
//	/* ザコ消滅爆発エフェクトを登録(現在Gu部分がないので描画してない) */
//	bakuhatsu_add_type_ddd(src, BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/* ザコ消滅爆発 */

static void move_zako_yarare(SPRITE *src)
{
	/* 描画用グラ回転 */
	src->rotationCCW1024	= (src->jyumyou<<6);
	/* グラ拡大 */
	src->m_zoom_x256		= (t256(4.0))-(src->jyumyou<<3);
	src->m_zoom_y256		= (t256(4.0))-(src->jyumyou<<3);
	/* 半透明 */
	src->color32 -= 0x08000000; /* 8==256/32 */
}
static void init_zako_yarare(SPRITE *src)
{	// ザコ消滅爆発エフェクトを登録
	src->callback_mover 	= move_zako_yarare;
	src->callback_hit_teki	= NULL;
	src->jyumyou			= (32);/* 雑魚やられ時間。 */
	static int bbb = 0;
	const u32 aaa[4] =
	{
		MAKE32RGBA(0xff, 0x00, 0x00, 0xff), // 赤
		MAKE32RGBA(0x00, 0xff, 0x00, 0xff), // 緑
		MAKE32RGBA(0x00, 0x00, 0xff, 0xff), // 青
		MAKE32RGBA(0xff, 0xff, 0x00, 0xff), // 黄
	};
	bbb++;
	bbb &= (4-1);
	src->color32			= aaa[bbb];
}
/*---------------------------------------------------------
	ザコを攻撃して、ザコに自弾があたった場合の共通ルーチン
	-------------------------------------------------------
	ザコは道中ザコ以外に、ボスが生成する使い魔も含む。
	-------------------------------------------------------
	SPRITE *src;	ザコ敵自体
	SPRITE *tama;	自弾
---------------------------------------------------------*/

/* 使い魔があるからglobal */
global void callback_hit_zako(SPRITE *src, SPRITE *tama)
{
	/* 雑魚に自弾があたった場合の火花エフェクトを登録(現在Gu部分がないので描画してない) */
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* 先に実行した方が速い */
//
	/* 上と分離した方がコード効率があがる。 */
	{
		src->base_hp -= tama->base_weapon_strength; 	/* 攻撃して体力減らす(強さ分引く) */
		if (0 >= src->base_hp)			/* ０か負値なら、倒した。 */
		{
			/* (ザコの)カスタムやられ処理 */
			if (NULL != src->callback_loser)	/* カスタムやられ処理があれば実行。 */
			{
				(src->callback_loser)(src); 	/* カスタムやられ処理を実行する。(標準は「ランダムテーブルからアイテム生成」) */
				src->callback_loser = NULL; 	/* 実行したので(念の為)消す。 */
				/* (やられ処理は1度しか実行しない) */
			}
			/* ザコ、スコア加算処理 */
			bonus_info_any_score_nodel(src, ((src->base_score)*(cg.game_now_stage)) );
		//	/* 自動消去へ仕様変更 */src->jyumyou = JYUMYOU_NASI;/* スプライトシステムに再利用をお願いする。 */
			init_zako_yarare(src);	// ザコを消滅爆発エフェクトにする。
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);/* 雑魚爆発音を鳴らす。 */
		}
	}
}


/*---------------------------------------------------------
	ボス死亡判定
	-------------------------------------------------------
	★ 攻撃の場合の死亡判定 		DESTROY_CHECK_00_WIN_BOSS
	★ 時間切れの場合の死亡判定 	DESTROY_CHECK_01_IS_TIME_OUT
---------------------------------------------------------*/

/*static*/ void boss_destroy_check_type(SPRITE *src/*敵自体*/, int check_type)
{
	if (0 >= /*data->boss_base.boss*/src->base_hp)			/* ０か負値なら、倒した。 */
	{
		/*data->boss_base.boss*/src->base_hp = 0;
		if (DESTROY_CHECK_00_WIN_BOSS == check_type)
		{
			cg.state_flag		|= STATE_FLAG_09_IS_WIN_BOSS;	/* 攻撃で倒したフラグon */
		}
	//	#if 1
		lose_boss(src);/* 共通 */
	//	#else
	//	if (NULL != src->callback_loser)
	//	{
	//		(src->callback_loser)(src);
	//		src->callback_loser = NULL;
	//	}
	//	#endif
	//	voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int ee_angle1024;
static int ff_angle1024;
/*static*/ void move_boss_effect(SPRITE *src)
{
	ee_angle1024 += (4);	/* 4 回転速度 */
	ff_angle1024 += (2);	/* 2 拡大縮小速度 */
	int ww_angle1024;	ww_angle1024 = ee_angle1024;
	int vv_angle1024;	vv_angle1024 = ff_angle1024;
	int radius;
	int i;
	for (i=0; i<5; i++)
	{
		ww_angle1024 += 20; 	/* 20 回転速度 */
		vv_angle1024 += 200;	/* 40 拡大縮小速度 */
		radius = ((sin1024(vv_angle1024))>>2)+16; /* 80==16+64 */
		SPRITE *h;
		h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
		h->cx256			= src->cx256 + ((sin1024((ww_angle1024))*radius));
		h->cy256			= src->cy256 + ((cos1024((ww_angle1024))*radius));
		h->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		h->m_zoom_y256		= ( (1/*+255*/+(radius<<2)) );
		#if 1
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		h->rotationCCW1024	= -ww_angle1024;
		#endif
	}
}

/*static*/ void boss_effect_sprite_add(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<(5+3/*+3*/); i++)
		{
			SPRITE *h;
			#if 0
			h						= obj_add_01_teki_error();
			#else
			h						= obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i);	/* 必ず登録できる。 */
			#endif
//			if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */	/*...うーん*/		/* 仕様バグ(?) */
			h->jyumyou				= JYUMYOU_MUGEN;/* 時間で自動消去しない */
//			{
//				obj99[OBJ_HEAD_02_KOTEI+i]		= h;
//			}
		}
	}

}
/*static*/ void boss_effect_reset(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<(5/*+1*/); i++)
		{
			{
				SPRITE *h;
				h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= (SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/* 非表示 */
			}
		}
	}

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)

	// 低速effect
	SPRITE *h;
	h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
	h->color32		= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* 自機、半透明 */	/*	s1->alpha			= 0x50;*/
}
/*static*/ void boss_effect_initialize_position(void)
{
	{int i;
		for (i=0; i<5; i++)
		{
		//	obj_effect[i]->type 				= SP_DELETE;
			sprite_initialize_position(&obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i]);
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 			user_data00 	/* 目標x座標 */
	#define target_y256 			user_data01 	/* 目標y座標 */
	#define toutatu_wariai256		user_data02 	/* 目標座標への到達割合 */
	#define kougeki_anime_count 	user_data03 	/* 攻撃アニメーション用カウンタ */
	#define boss_time_out			user_data04 	/* 制限時間 */
	#define boss_base_state777		user_data04 	/* 制限時間(boss_time_outと同じ) */
//
	#define boss_spell_timer		user_data05 	/* スペル時間 */
#endif


/*---------------------------------------------------------
	ボス誘導移動のみ。
---------------------------------------------------------*/

global void boss_yuudou_idou_nomi(SPRITE *src)
{
	src->toutatu_wariai256 -= (1);
	if (1 > src->toutatu_wariai256 )
	{
		src->toutatu_wariai256 = (0);
	}
}

/*---------------------------------------------------------
	ボス誘導比率計算。
---------------------------------------------------------*/

global void boss_yuudou_hiritu_keisan(SPRITE *src)	/* 目標を設定し、誘導移動 */
{
	{	/* 差分 == (弾の現在座標 - 弾の誘導座標) */
		int x_sa256 = (src->cx256 - src->target_x256);
		int y_sa256 = (src->cy256 - src->target_y256);
		/* 加算差分 == (弾の差分座標 * 誘導比率) */
		int aaa_x256 = ((x_sa256 * ((src->toutatu_wariai256) ))>>8);	/*fps_factor*/
		int aaa_y256 = ((y_sa256 * ((src->toutatu_wariai256) ))>>8);	/*fps_factor*/
		src->cx256 = (src->target_x256) + (aaa_x256);		/*fps_factor*/
		src->cy256 = (src->target_y256) + (aaa_y256);		/*fps_factor*/
	}
}



/*---------------------------------------------------------
	ボスオプション、共通
---------------------------------------------------------*/
global void check_boss_option_time_out(SPRITE *src)
{
	#if 1
	/* ボスを倒すと皆破壊される。 */
	if (0==boss_life_value)/* ダメかも(?) */
	{
		src->boss_time_out = (0);
	}
	src->boss_time_out--;
	if (0 > src->boss_time_out)
	{
		#if 1
		src->base_hp	= (0);/* (?) これで良いか検証の必要あり */
		#endif
		src->jyumyou		= JYUMYOU_NASI;
	}
	#endif
}


/*---------------------------------------------------------
	ボスオプション、共通
	時間切れの場合の透明度設定
---------------------------------------------------------*/
global void set_timeout_alpha(SPRITE *src)
{
	if (0x1ff > src->boss_time_out)
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->boss_time_out<<(23))&0xff000000);
	}
}


/*---------------------------------------------------------
	敵やられ
	ボス倒した場合の処理
	(プレイヤー側含む)
	-------------------------------------------------------
???謎コメント???	ボスを倒したらすぐ呼ばれる(プレイヤーを無敵にする為)
???謎コメント???	(フラグは仕様上時間待ちがある為、現在この用途には使えない)
---------------------------------------------------------*/

extern void player_loop_quit(void);

extern void set_clear_pdc_special_ivent(void);
extern void set_bg_alpha(int set_bg_alpha);

global /*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY が必要 */
//
	set_clear_pdc_special_ivent();
	{
	//	pd_bo ssmode = B02_BOSS_DESTROY;
//		cg.state_flag |= ST ATE_FLAG_11_IS_BOSS_DESTROY;
		#if 1
		/* バグでアイテム出ない(???) */
	//	if (cg.state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* 攻撃で倒した場合のみ */
		#endif
		{
		//	cg.state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* 終わり */
			/* 相打ちの場合、強制喰らい復活、キー入力有効(0) */
//			cg.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL));	/* 終わり */
			cg.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS/*???|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL*/));	/* 終わり */
			if (0 >= spell_card.boss_timer)
			{
				;/* 時間切れの場合はボーナスアイテムと得点なし。 */
			}
			else
			{
				item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* ボーナスアイテムを出す */
				player_dummy_add_score(src->base_score);			/* ボスの得点加算 */
			}
		}
	}
	//		data->boss_base.boss_health = 0;/* なんかバグるので追加 */
	/* コールバック登録 */
	src->callback_hit_teki	= NULL; 	/* ダミーコールバック登録 */
	#if 0
//	src->callback_mover 		= NULL; 		共通ボス退避(撃破後に画面外にボスが逃げる)
//	src->type					= SP_DELETE;	共通ボス退避(撃破後に画面外にボスが逃げる)
	#else
	src->callback_mover 		= common_99_keitai;/* 共通ボス退避(撃破後に画面外にボスが逃げる) */
	#endif
//
//
//	set_clear_pdc_special_ivent();
//
	cg.bomber_time		= 0;
	#if 1
//	pd_bomber_time = 0;/* 都合上 */
	set_bg_alpha(255);/* 画面を明るくする */
	#endif

	#if 1
	src->base_hp = 0;
	boss_life_value = 0;
	#endif
	#if 1
	draw_boss_hp_value	= 0;/* よくわかんない */	/* アリスを倒すと皆破壊される。 */
	#endif
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* 半透明 */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* 半透明 */
	boss_effect_initialize_position();

	/*---------------------------------------------------------
		ボス特殊処理
	---------------------------------------------------------*/
	//	if (B02_BOSS_DESTROY==pd_bo ssmode) //ボスを倒したときの処理
	//	if (/*ST ATE_FLAG_11_IS_BOSS_DESTROY==*/ (cg.state_flag & ST ATE_FLAG_11_IS_BOSS_DESTROY))
	//void boss_destroy_aaa(void)	/* ボスを倒した直後、「ボス後イベント」前の処理 */
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		// TIME_20_DBWAITフレーム待ってから実行。ボスを倒した時に画面に表示されている弾を全て消す処理のために必要。
		#if 0/* 本家風ならoff */
		play_music_num(BGM_00_stop);
		#endif
//
		voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);
//		voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*予備(うるさい)*/
//
		{
			cg.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//			cg.state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */
			if (
				(0==cg.game_practice_mode)/* 練習モードではボス後イベントは見れないよ。 */
				||/*(または)*/
				(	/* 隠しエンディング */
					(1==cg.game_practice_mode)/* 練習モードではボス後イベントは見れないよ。 */
					&&/*(かつ)*/
					((0)==(cg_game_difficulty))/*(easyの場合)*/
					&&/*(かつ)*/
					((6) == (cg.game_now_stage))/*(6面の場合、隠しエンド)*/
				)
			)
			{
			//	pd_bo ssmode	= B07_AFTER_LOAD;
				cg.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			}
			else/* 練習モードの場合、終了する */
			{
				#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
				cg.game_now_max_continue = 1;	/* コンティニューさせない */
				player_loop_quit();
				#endif
			}
		}
	}
//
}
