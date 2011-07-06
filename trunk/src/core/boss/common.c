
#include "boss.h"//#include "game_main.h"//#include "bullet_object.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------2
	ザコ登録の共通ルーチン
	-------------------------------------------------------
---------------------------------------------------------*/

global void add_zako_common(STAGE_DATA *l, SPRITE *src)
{
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);	/* デフォルトはあたり判定あり */
	src->callback_loser 	= lose_random_item; 		/* 「ザコがやられた場合の処理」に、「ランダムテーブルからアイテム生成」を設定 */
	src->callback_hit_enemy = callback_hit_zako;		/* 「ザコに自弾があたった場合の処理」に、標準の処理を設定 */
//
	src->cx256				= ((l->user_x)<<8); 	/* 設定ファイルから出現x座標を決める。 */
	src->cy256				= ((l->user_y)<<8); 	/* 設定ファイルから出現y座標を決める。 */
	src->base_hp			= ((l->user_hp));		/* 設定ファイルから体力を決める。 */
	src->base_score 		= ((l->user_score));	/* 設定ファイルから獲得スコアを決める。 */
}


/*---------------------------------------------------------
	ボス形態変更時の共通
---------------------------------------------------------*/

//extern SPRITE *obj_effect[8];	/* [□]（スペルカードのグラフィック） */

/*---------------------------------------------------------
	ザコを攻撃した場合の共通ルーチン
	-------------------------------------------------------
---------------------------------------------------------*/

void callback_hit_zako(SPRITE *src/*敵自体*/, SPRITE *tama /*自弾*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*先に実行した方が速い*/
//
	{
		src->base_hp -= tama->base_weapon_strength; 	/* 攻撃して体力減らす(強さ分引く) */
		if (0 >= src->base_hp)			/* ０か負値なら、倒した。 */
		{
			bakuhatsu_add_type_ddd(src, BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/*ザコ消滅爆発*/
			/* (ザコの)カスタムやられ処理 */
			if (NULL != src->callback_loser)	/* カスタムやられ処理があれば実行。 */
			{
				(src->callback_loser)(src); 	/* カスタムやられ処理を実行する。(標準は「ランダムテーブルからアイテム生成」) */
				src->callback_loser = NULL; 	/* 実行したので(念の為)消す。 */
			}
			bonus_info_any_score_nodel(src, ((/*data->*/src->base_score)*(/*pd_now_stage*/pd.player_now_stage/*le vel*/)) );
			/*自動消去へ仕様変更*/src->jyumyou = JYUMYOU_NASI;
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
		}
	}
}


/*---------------------------------------------------------
	ボスを攻撃した場合の共通ルーチン
	-------------------------------------------------------
---------------------------------------------------------*/
int boss_hp_dec_by_frame;/*ボス攻撃減少値、フレーム単位*/

/*static*/ void callback_hit_boss(SPRITE *src/*敵自体*/, SPRITE *tama /*自弾*/)/*, int angle*/
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*先に実行した方が速い*/
//
	{
		boss_hp_dec_by_frame += /*w->*/tama->base_weapon_strength;	/* 攻撃して体力減らす(強さ分引く) */
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
			pd.state_flag		|= STATE_FLAG_09_IS_WIN_BOSS;	/* 攻撃で倒したフラグon */
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
		h					= &obj00[FIX_OBJ_08_EFFECT01+i];
		h->cx256 			= src->cx256 + ((sin1024((ww_angle1024))*radius));
		h->cy256 			= src->cy256 + ((cos1024((ww_angle1024))*radius));
		#if (1==USE_ZOOM_XY)
		h->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		h->m_zoom_y256		= ( (1/*+255*/+(radius<<2)) );
		#else
		h->m_zoom_xy256 	= ( (1/*+255*/+(radius<<2)) );
		#endif
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
			h						= sprite_add_gu_error();
			#else
			h						= sprite_add_direct(FIX_OBJ_08_EFFECT01+i); 	/* 必ず登録できる。  */
			#endif
//			if (NULL!=h)/* 登録できた場合のみ */	/* 強制登録 */	/*...うーん*/		/* 仕様バグ(?) */
			h->jyumyou				= JYUMYOU_MUGEN;/* 時間で自動消去しない */
//			{
//				obj00[i]		= h;
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
				h					= &obj00[FIX_OBJ_08_EFFECT01+i];
				h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= (SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/*非表示*/
			}
		}
	}

//#define TEISOKU_EFFECT_00_OBJ (6)
//#define TEISOKU_EFFECT_01_OBJ (7)

	// 低速effect
	SPRITE *h;
	h					= &obj00[FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
	h->color32		= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* 自機、半透明 */	/*	s1->alpha			= 0x50;*/
}
/*static*/ void boss_effect_initialize_position(void)
{
	{int i;
		for (i=0; i<5; i++)
		{
		//	obj_effect[i]->type 				= SP_DELETE;
			sprite_initialize_position(&obj00[FIX_OBJ_08_EFFECT01+i]);
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0/*メモ*/
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define boss_time_out		user_data03 	/* 制限時間 */
#endif


global unsigned int alice_anime_count;

global void alice_yuudou_move_only(SPRITE *src)
{
	src->vvv256 -= (1);
	if (1 > src->vvv256 )
	{
		src->vvv256 = (0);
	}
}

global void alice_yuudou_calc(SPRITE *src)	/* 目標を設定し、誘導移動 */
{
	{	/* 差分 == (弾の現在座標 - 弾の誘導座標) */
		int x_sa256 = (src->cx256 - src->target_x256);
		int y_sa256 = (src->cy256 - src->target_y256);
		/* 加算差分 == (弾の差分座標 * 誘導比率) */
		int aaa_x256 = ((x_sa256 * ((src->vvv256) ))>>8);	/*fps_factor*/
		int aaa_y256 = ((y_sa256 * ((src->vvv256) ))>>8);	/*fps_factor*/
		src->cx256 = (src->target_x256) + (aaa_x256);	/*fps_factor*/
		src->cy256 = (src->target_y256) + (aaa_y256);	/*fps_factor*/
	}
}

static void alice_animation_only(SPRITE *src)
{
	if (alice_anime_count)	/* 攻撃アニメーション */
	{
		alice_anime_count--;
				if ((32)>alice_anime_count) 	{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>alice_anime_count) 	{	src->type = TEKI_10_BOSS33; }	/*src->an im_frame = 0x22;*/
		else									{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* 移動アニメーション */
	{
		int aaa;
		aaa = (src->cx256> src->target_x256)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /*左右*/

		#if 0
				if ( ( 16) > src->vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->vvv256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->vvv256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->vvv256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->vvv256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#endif
		src->type = aaa;
	}
}


/*---------------------------------------------------------
	ボス行動
---------------------------------------------------------*/

global void move_alice(SPRITE *src)
{
//	ch eck_regist_spell_card(src);	/* スペカ登録可能なら登録 */
//	sp ell_card_generator222(src);	/* スペカ生成 */
	check_regist_generate_spell_card(src);	/* スペカ登録可能なら登録 / スペカ生成 */
	#if 1/* [スペカシステム内に移動予定] */
	/*---------------------------------------------------------
		パチェ移動処理
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	boss_move_vx_vy(src);
	boss_clip_rect(src);
	alice_animation_only(src);
//	move_all_doll(src);
	#endif
//
	move_boss_effect(src);		/* 回エフェクト */
	danmaku_generator(src); 	/* 弾幕生成 */
}

/*---------------------------------------------------------
	ボスオプション、共通
---------------------------------------------------------*/
global void check_boss_option_time_out(SPRITE *src)
{
	#if 1
	/* ボスを倒すと皆破壊される。 */
	if (0==boss_life_value)/*ダメかも(?)*/
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
	敵やられ
	ボス倒した場合の処理
	(プレイヤー側含む)
	-------------------------------------------------------
???謎コメント???	ボスを倒したらすぐ呼ばれる(プレイヤーを無敵にする為)
???謎コメント???	(フラグは仕様上時間待ちがある為、現在この用途には使えない)
---------------------------------------------------------*/
extern void player_loop_quit(void);
extern int	now_max_continue;

extern void set_clear_pdc_special_ivent(void);
extern void set_bg_alpha(int set_bg_alpha);
extern int practice_mode;

global /*static*/ void lose_boss(SPRITE *src)
{
	bakuhatsu_add_circle(src, 1);	/* pl ayer_set_destoroy_boss(); B02_BOSS_DESTROY が必要 */
//
	set_clear_pdc_special_ivent();
	{
	//	pd_bo ssmode = B02_BOSS_DESTROY;
//		pd.state_flag |= ST ATE_FLAG_11_IS_BOSS_DESTROY;
		#if 1
		/* バグでアイテム出ない(???) */
	//	if (pd.state_flag&STATE_FLAG_09_IS_WIN_BOSS)		/* 攻撃で倒した場合のみ */
		#endif
		{
		//	pd.state_flag		&= (~STATE_FLAG_09_IS_WIN_BOSS);	/* 終わり */
			/* 相打ちの場合、強制喰らい復活、キー入力有効(0) */
//			pd.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL));	/* 終わり */
			pd.state_flag		&= (~(STATE_FLAG_09_IS_WIN_BOSS/*???|STATE_FLAG_16_NOT_ALLOW_KEY_CONTROL*/));	/* 終わり */
			if (0 >= spell_card_boss_timer)
			{
				;/* 時間切れの場合はボーナスアイテムと得点なし。 */
			}
			else
			{
				item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* ボーナスアイテムを出す */
				player_dummy_add_score(src->base_score);					/* ボスの得点加算 */
			}
		}
	}
	//		data->boss_base.boss_health = 0;/* なんかバグるので追加 */
	/* コールバック登録 */
	src->callback_hit_enemy 	= NULL; 	/* ダミーコールバック登録 */
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
	pd.bomber_time		= 0;
	#if 1
//	pd_bomber_time = 0;/*都合上*/
	set_bg_alpha(255);/* 画面を明るくする */
	#endif

	#if 1
	src->base_hp = 0;
	boss_life_value = 0;
	#endif
	#if 1
	draw_boss_hp_value	= 0;/* よくわかんない */	/* 輝夜/アリスを倒すと皆破壊される。 */
	#endif
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* 半透明 */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* 半透明 */
	boss_effect_initialize_position();

	/*---------------------------------------------------------
		ボス特殊処理
	---------------------------------------------------------*/
	//	if (B02_BOSS_DESTROY==pd_bo ssmode) //ボスを倒したときの処理
	//	if (/*ST ATE_FLAG_11_IS_BOSS_DESTROY==*/ (pd.state_flag & ST ATE_FLAG_11_IS_BOSS_DESTROY))
	//void boss_destroy_aaa(void)	/* ボスを倒した直後、「ボス後イベント」前の処理 */
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		// TIME_20_DBWAITフレーム待ってから実行。ボスを倒した時に画面に表示されている弾を全て消す処理のために必要。
		play_music_num(BGM_00_stop);
//
		voice_play(VOICE03_BOSS_HAKAI, TRACK03_SHORT_MUSIC/*TRACK02_ALEART_IVENT*/);		// [***090313		追加
//		voice_play(VOICE03_BOSS_HAKAI, TRACK01_EXPLODE);/*予備(うるさい)*/
//
		{
			pd.state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//			pd.state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* 終わり */
			if (0==practice_mode)/* 練習モードではボス後イベントは見れないよ。 */
			{
			//	pd_bo ssmode	= B07_AFTER_LOAD;
				pd.state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
			}
			else/* 練習モードの場合、終了する */
			{
				#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
				now_max_continue = 1;	/* コンティニューさせない */
				player_loop_quit();
				#endif
			}
		}
	}
//
}
