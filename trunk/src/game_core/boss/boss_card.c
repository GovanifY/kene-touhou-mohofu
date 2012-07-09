
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	カード管理システム
	-------------------------------------------------------
	カードはシステムでサポートしているので、どのボスが、どのカードでも撃てます。
	-------------------------------------------------------
	ToDo:
	★ 攻撃の管理
	★ 動作の管理(結構未定)
	★ 雑魚(中雑魚)がカードを撃てる機能は、廃止した方が良いかも知れない。
	これ(中雑魚＆中雑魚カード機能)を廃止して、代わりに道中にボスを出現させる(つまり中-ボス)
	の方が、システムがすっきりして良い気がする。そうする為には色々改造個所がある。
	現状の dat の規格だと、これらは出来ないので、主に dat の規格を変えて、
	dat からボスの会話を起動するようにする必要がある。
	プレイヤーに拠って会話が違うので、その辺の規格策定はまだ未定。(分岐シナリオとか)
	-------------------------------------------------------
	★ 雑魚(中雑魚)がカードを撃てる機能は、廃止して、
	雑魚(中雑魚)がカードを撃ちたい場合は、「カードが撃てるザコ」という物を新設してそれを使い
	システムではサポートしない方が良いと思う。のでそういう方向で改造予定。
---------------------------------------------------------*/

#include "kanji_system.h"

#include "../menu/kaiwa_sprite.h"

/*---------------------------------------------------------
	ボス追加
---------------------------------------------------------*/

// チルノ EX1面
// 咲夜 6面
extern OBJ_CALL_FUNC(boss_move_17_sakuya_bimyou_idou);/*(微妙に移動する)*/
extern OBJ_CALL_FUNC(boss_move_16_sakuya_nazo_keitai);/*(差分氏の謎形態)*/
extern OBJ_CALL_FUNC(boss_move_15_sakuya_festival_knife);/*(幻葬「フェスティバルナイフ」)*/
extern OBJ_CALL_FUNC(boss_move_14_sakuya_miss_direction);/*(奇術「ミスディレクション」)*/

// パチェ 5面
//レーザー
extern OBJ_CALL_FUNC(add_laser_off);
extern OBJ_CALL_FUNC(add_laser_on);
// 文 4面
extern OBJ_CALL_FUNC(boss_move_13_aya_taifu);
// 輝夜 3面
extern OBJ_CALL_FUNC(boss_move_12_kaguya_funya_idou);
extern OBJ_CALL_FUNC(boss_move_11_kaguya_yureru);
extern OBJ_CALL_FUNC(boss_init_kaguya);
// 魅魔 2面
extern OBJ_CALL_FUNC(boss_move_10_mima_keitai);
// アリス 1面

// 共通形態
extern OBJ_CALL_FUNC(boss_move_05_xy_douki_differential64);
extern OBJ_CALL_FUNC(boss_move_04_xy_douki_differential32);
extern OBJ_CALL_FUNC(boss_move_03_x_douki);
extern OBJ_CALL_FUNC(boss_move_02_xy_hidouki);
extern OBJ_CALL_FUNC(boss_move_01_taihi_ue_naka);

/* 共通部 */
static OBJ_CALL_FUNC(init_00_boss_clip000);/* 標準タイプ */
static OBJ_CALL_FUNC(init_00_boss_clip111);/* 上に広いタイプ */
static OBJ_CALL_FUNC(init_00_boss_clip222);/* 上だけ広いタイプ */

//------------ カード関連
#include "card_address.h"
#if (1)
	/* 現在の形態で撃てるカード番号をカードシステムに設定。 */
	/* AYA５形態の場合。(r35-)のシステムで通常弾を利用する為に、カードシステムの初期化が必要。 */
	/* したがって、どんな形態でも(r35以降は)必ずスペカにする必要がある。 */
	#define init_48_r34_gokan_kinou 	card_maikai_init_and_get_spell_number
	#if (1)
		#define SPELL_r34_GOKAN_KINOU		SPELL_48_r34_gokan_kinou/*(必要)*/
	#else
	//	#define SPELL_r34_GOKAN_KINOU		SPELL_00/*(ダメ)*/
	#endif
#endif

/*---------------------------------------------------------
	スプライト リストに登録されたスプライトを全部 。
---------------------------------------------------------*/
#define TEST_ZAKO_HIDE (0)
#if (1==TEST_ZAKO_HIDE)/* ボスも影響受ける */
static void sprite_sethide_all(OBJ *s, unsigned int length)
{
	unsigned int ii;
	for (ii=0; ii<length; ii++)/* 全部調べる。 */
	{
		s->BOSS_DATA_05_move_jyumyou	= 0xff;
		#if (1)/* デバッグてすと */
		s->center.x256		= (-1);
		#endif
		s++;
	}
}
static void zako_all_timeup(void)/*int ty pe*/
{
	OBJ *s;
	s = &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+0];
	sprite_sethide_all(s, OBJ_POOL_02_TEKI_FIX_0256_MAX);
}
#endif


/*---------------------------------------------------------
	[r36新]ボス誘導移動のみ。
---------------------------------------------------------*/

static int bos_ddd_obj_alt_x256;
static int bos_ddd_obj_alt_y256;
static int bos_ddd_obj_target_x256;
static int bos_ddd_obj_target_y256;
//static int bos_ddd_obj_ido_speed256;


/*---------------------------------------------------------
	ボス誘導移動のみ。
---------------------------------------------------------*/
/* 誘導移動/誘導計算 */
static OBJ_CALL_FUNC(boss_r36_yuudou)
{
	{	/*(とりあえず)*/
		src->BOSS_DATA_04_toutatu_wariai256 -= (6);/*(ボス退避、速度。1/t256()形式。(逆数で指定する) )*/
	//	src->BOSS_DATA_04_toutatu_wariai256 -= (bos_ddd_obj_ido_speed256);/*(ボス退避、速度)*/
		src->BOSS_DATA_04_toutatu_wariai256 = psp_max(src->BOSS_DATA_04_toutatu_wariai256, 0);
		//
		if (0 < src->BOSS_DATA_04_toutatu_wariai256)/*(移動が必要な場合のみ移動処理を行う)*/
		{
			int move_rate256;
			move_rate256 = src->BOSS_DATA_04_toutatu_wariai256;
			move_rate256 = psp_min(move_rate256, 256);
			move_rate256 = psp_max(move_rate256, 0);
			#if (1)/* 非等分値の計算 */
			{
				u32 i_rate65536;
				i_rate65536 = (move_rate256 << 8);
			//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
				i_rate65536 = (int)(vfpu_ease_in_out65536(i_rate65536));	/* t65536の等分カウンタ値を使い、非等分になるよう値を修正する。
				非等分の方式は「ease_in_out」これは、始めと最後がゆっくりで間が速いという配分方式。 */
				move_rate256 = (i_rate65536 >> 8);
			}
			#endif
			//------------------
			#if (1)/*(到達割合を考慮して、合成する。)*/
		//	REG_00_SRC_X	= ((bos_ddd_obj_alt_x256)); 	/*(合成位置[A]t256()形式)*/
		//	REG_01_SRC_Y	= ((bos_ddd_obj_alt_y256)); 	/*(合成位置[A]t256()形式)*/
		//	REG_02_DEST_X	= ((bos_ddd_obj_target_x256));	/*(合成位置[B]t256()形式)*/
		//	REG_03_DEST_Y	= ((bos_ddd_obj_target_y256));	/*(合成位置[B]t256()形式)*/
			REG_00_SRC_X	= ((bos_ddd_obj_target_x256));	/*(合成位置[A]t256()形式)*/
			REG_01_SRC_Y	= ((bos_ddd_obj_target_y256));	/*(合成位置[A]t256()形式)*/
			REG_02_DEST_X	= ((bos_ddd_obj_alt_x256)); 	/*(合成位置[B]t256()形式)*/
			REG_03_DEST_Y	= ((bos_ddd_obj_alt_y256)); 	/*(合成位置[B]t256()形式)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(合成割合t256()形式)*/
			multiprex_rate_vector();/*(破壊レジスタ多いので注意)*/
			#endif
			// この時点で、
			// REG_02_DEST_X: 合成値
			// REG_03_DEST_Y: 合成値
			//-----------------------
			src->center.x256 = (REG_02_DEST_X);
			src->center.y256 = (REG_03_DEST_Y);
		}
	}
}
/*---------------------------------------------------------
	ボス誘導移動のみ。
---------------------------------------------------------*/
global OBJ_CALL_FUNC(boss_set_new_position)
{
	src->BOSS_DATA_04_toutatu_wariai256 	= t256(1.0);/*(初期値)*/
	/*(現在位置を設定)*/
	bos_ddd_obj_alt_x256		= (src->center.x256);
	bos_ddd_obj_alt_y256		= (src->center.y256);
	/*(退避位置を設定)*/
	bos_ddd_obj_target_x256 	= REG_02_DEST_X;/*(t256()形式)*/
	bos_ddd_obj_target_y256 	= REG_03_DEST_Y;/*(t256()形式)*/
	/*(移動速度を設定)*/
//	bos_ddd_obj_ido_speed256	= HATSUDAN_01_speed256;/*(==REG_11)*/
}

/*---------------------------------------------------------
	共通ボス退避(撃破後に画面外にボスが逃げる)
	[カードシステム内に移動予定]
---------------------------------------------------------*/

static OBJ_CALL_FUNC(common_99_keitai)
{
	boss_r36_yuudou(src);
	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
	if (src->BOSS_DATA_04_toutatu_wariai256 <= t256(0))
	{
		#if (1)
	//重複	src->callback_mover 	= NULL; 		/* おしまい */
		sprite_kotei_obj_r36_taihi(src);/*(ボスobj使用中であるが退避位置へ退避)*/
		bullets_to_hosi();/* 弾全部、星アイテムにする */
		#endif
		boss_effect_kotei_obj_r36_taihi();
		//return /*(1)*/;/*移動完了*/
	}
}
/* 撃破後に画面外にボスが逃げる */
static OBJ_CALL_FUNC(common_88_keitai)
{
	/*(レーザーモードは、強制的にoffにする。)*/
	add_laser_off(NULL);
	#if (1)
	/*(退避位置を設定)*/
	REG_02_DEST_X = (int)(GAME_X_OFFSET*256+(GAME_320_WIDTH*256/2));
	REG_03_DEST_Y = -t256((50.0/2));/* +t256(50.0) ボスグラの最大サイズ(50[pixel]) */
	boss_set_new_position(src);
	#endif
	src->BOSS_DATA_05_boss_base_state777	= (0);	/*ST_00*/	/*初期値を0にする。*/
	src->callback_mover 					= common_99_keitai; 	/* 最終形態にする。 */
}

/*---------------------------------------------------------
	ボス死亡判定
	-------------------------------------------------------
	★ 攻撃の場合の死亡判定 		DESTROY_CHECK_00_WIN_BOSS
	★ 時間切れの場合の死亡判定 	DESTROY_CHECK_01_IS_TIME_OUT
	-------------------------------------------------------
	ボス敵やられ
	ボス倒した場合の処理
	(プレイヤー側含む)
---------------------------------------------------------*/
/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */

extern void player_loop_quit(void);
extern void jiki_eien_muteki_on(void);/*(自機クラスjiki::eien_muteki_on(void);) (game_core/jiki/jiki.c)*/
extern void set_bg_alpha(int set_bg_alpha);

/* 共通ボス退避(撃破後に画面外にボスが逃げる) */
/*(撃破形態)*/
/*(???)*/static OBJ_CALL_FUNC(NULL_keitai)/*(src==ボス敵本体)*/
{
	/*(card.boss_move_card_callbackがNULLに対応してない場合に、必要なダミー形態)*/
}
//	/*static*/ void boss_destroy_check_type(OBJ/**/ *src/*敵自体*/, int check_type)
//static OBJ_CALL_FUNC(kaiwa_000_dummy)/*(ダミー形態)*/
//{
//}


//	if (0 >= src->base_hp)			/* ０か負値なら、倒した。 */
	// /* ボスを倒した直後、「ボス後イベント」前の処理 */
//	#if (0)
//	enum
//	{
//		DESTROY_CHECK_00_WIN_BOSS = 0,
//		DESTROY_CHECK_01_IS_TIME_OUT,
//	};
//	int check_type;
//	check_type = DESTROY_CHECK_00_WIN_BOSS;/*(とりあえず)*/
//	if (DESTROY_CHECK_00_WIN_BOSS == check_type)
//	#endif

static OBJ_CALL_FUNC(gekiha_keitai)/*(src==ボス敵本体)*/
{
	#if 0
//	bakuhatsu_add_circle(src, 1);/*(爆発エフェクト)*/
	#endif
	src->base_hp = 0;
	/* コールバック登録 */
	src->callback_hit_teki			= NULL; 	/*(ダミーコールバック登録)*/
	src->callback_mover 			= common_88_keitai;/* 共通ボス退避(撃破後に画面外にボスが逃げる) */
	src->callback_root_supeka		= NULL; 	/*(ダミーコールバック登録[念の為])*/
//重複(既に設定済み)	card.boss_move_card_callback	= NULL_keitai;/*(テスト)*/
	{
		#if (1==USE_BOSS_JIKANGIRE)/*(使用予定あり。未検証)*/
		if (0 != (cg.state_flag&JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE))/*0 >= card.card_timer*/
		{
			;/* 時間切れの場合はボーナスアイテムと得点なし。 */
		}
		else
		#endif
		{	/* 攻撃で倒した場合のみ */
			item_create_for_boss(src, ITEM_CREATE_MODE_01); 	/* ボーナスアイテムを出す */
			player_dummy_add_score(src->base_score);			/* ボスの得点加算 */
		}
	}
	//
	#if (0)/*(デバッグ)*/
	{	psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load stage: STAGE%d" "\\n"
			"ボス、撃破テストOK。", cg.game_now_stage);
	}
	#endif
//
	jiki_eien_muteki_on();/*(自機を一時的に、無敵状態にする)*/
	set_bg_alpha(255);/* 画面を明るくする */
//	s1->color32 		= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	s1->alpha		= 0x50; 	/* 半透明 */
//	obj_maru->color32	= MAKE32RGBA(0xff, 0xff, 0xff, 0x50);	//	obj_maru->alpha = 0x50; 	/* 半透明 */
	// ボスを倒したときの処理
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	AUDIO_18_voice_number	= VOICE03_BOSS_HAKAI;
	AUDIO_19_voice_truck	= TRACK03_IVENT_DAN/*TRACK02_JIKI_BOMBER*/; 	cpu_voice_play();
//	AUDIO_19_voice_truck	= TRACK01_PICHUN; /*予備(うるさい)*/			cpu_voice_play();
	cg.draw_boss_hp_value	= (0); /* 必要 */ /*(boss_hp_frame_check()を無効にする。Gu側でチェックさせない)*/
	cg.bomber_time			= (0); /* 都合上 */
	{
//		cg.state_flag		&= (~(JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* 終わり */
		cg.state_flag		|= ( (JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU));		/* 自動収集開始 */
		/* 自動収集モードはステージロード時に強制解除される。 */
		if (
			(0==cg.game_practice_mode)/* 練習モードではボス後イベントは見れないよ。 */
			||/*(または)*/
			(	/* 隠しエンディング */
				((1)==cg.game_practice_mode)/* 練習モードではボス後イベントは見れないよ。 */
				&&/*(かつ)*/
				((0)==(cg.game_difficulty))/*(easyの場合)*/
				&&/*(かつ)*/
				((6) == (cg.game_now_stage))/*(6面の場合、隠しエンド)*/
			)
		)
		{
			#if (1==USE_r36_SCENE_FLAG)
			NEXT_SCENE;/*(次の場面へ設定)*/
			#else
			#endif
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









////////  形態系はここより上に記述く。


/*---------------------------------------------------------
	カード登録
	-------------------------------------------------------
		[カード攻撃のみに仕様変更]
		(準備時の移動などもカード扱い)
		(カード無くなった後もカード扱い)
---------------------------------------------------------*/

// int card.number; 		/* 共用 */	// カード番号

global CARD_SYSTEM_GLOBAL_CLASS card;

/*---------------------------------------------------------
	カード設定。
---------------------------------------------------------*/
#include "card_resource.h"



/*---------------------------------------------------------
	カードを次に進める。
---------------------------------------------------------*/
static void card_address_incliment(void)
{
	/*(撃破形態の場合、カードを次に進めない。)*/
//	if (SPE LL_49_r36_gekiha != my_card_resource[(card.address_set)].spell_set_number)
	{
		card.address_set += (4);/*(カードを次に進める。)*/
	}
}


/*---------------------------------------------------------
	card内ボス移動処理を設定する。
---------------------------------------------------------*/
static void card_set_boss_move_callback(void)
{
	card.boss_move_card_callback = my_card_resource[(card.address_set)].boss_move_keitai_set_callback;/*(card内ボス移動処理を設定する)*/
}
static void set_new_limit(void)
{
	card.limit_health -= (my_card_resource[(card.address_set + (4))].spell_life);	/*1000 500*/
	card.limit_health = psp_max(card.limit_health, 0);		/*(0未満==負数にしない)*/
	//
	#if 0
	/*(制限時間を足す)*/
	card.card_timer += (((my_card_resource[(card.address_set + (4))].spell_limit_time)));	/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
	#else
	/*
		(r35 固定制限時間を設定[仮仕様])
		タイマー値の保持が現状一ヶ所しか無いので、とりあえずこういう形にしといた。(制限時間の設定部分が無い為)
	*/
	card.card_timer = (((my_card_resource[(card.address_set + (4))].spell_limit_time)));	/* 75*64==75[count] 	約99[秒(64/60)](単位は秒ではない) */
	#endif
}
/*---------------------------------------------------------
	カードシステムのボス形態に登録されたカード番号を取得し、
//	同時にカードの時間切れを設定する。
	同時にスペルの初期化を行う。
---------------------------------------------------------*/

global OBJ_CALL_FUNC(card_maikai_init_and_get_spell_number)
{
	card.spell_used_number	= my_card_resource[(card.address_set)].spell_set_number;	/* カードをセット */
	//
	card_maikai_init(src);		/* カードの制限時間を設定(予めカードごとに設定されている標準時間に設定) */
}


/*---------------------------------------------------------
	[ボス行動01: カード撃破後の処理]
	ボスのカードを撃破して、次のカードに移る場合の処理。
---------------------------------------------------------*/
static OBJ_CALL_FUNC(boss_action_a02_new_card_set);
static OBJ_CALL_FUNC(boss_action_01_gekiha_go)
{
	#if (0)/*あやや*/
	bakuhatsu_add_circle(src, 1);
	#endif
	//
	/* ボス形態変更時の共通ルーチン カード撃破後アイテム出す。 */
	//++	pd_bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* 弾全部、星アイテムにする */
	/*(時間内の場合)*/
	if (0 < (card.card_timer)) /*1	(set_new_limit();以前に必要) */
	{
		item_create_for_boss(src, ITEM_CREATE_MODE_02); 	/* アイテム吐く */
	}
	/* [(とりあえず)カード攻撃のみに仕様変更]したので、最後撃てるカードがなくなった場合に攻撃させる為。 */
	/*(規定値の算出)*/
	/*(リミット分引く)*/
	set_new_limit();
	card_address_incliment();
	card_set_boss_move_callback();
	#if 0//(1==TEST_ZAKO_HIDE)/* ボスも影響受ける */
	zako_all_timeup();/* ザコタイムアウト(フェイドアウト消去処理へ移行) */
	#endif
	//
	AUDIO_18_voice_number	= VOICE07_BOMB;
	AUDIO_19_voice_truck	= TRACK02_JIKI_BOMBER;/*テキトー*/
	cpu_voice_play();
	/*--- BOSS 共通して値を 0 にする。------------*/
	src->BOSS_DATA_05_move_jyumyou = (0);
//	src->BOSS_DATA_05_boss_base_state777 = (0);/* 共通して 値を 0 にする */
//	player_dummy_add_score(src->boss_base_score);
	src->callback_root_supeka		= boss_action_a02_new_card_set;
}



/*---------------------------------------------------------
	[ボス行動04: カード攻撃中]
	01カード攻撃中
	ボス、スペカで攻撃中。
---------------------------------------------------------*/
static OBJ_CALL_FUNC(boss_action_04_speka_kougeki)
{
	{
		/*(時間制限カウント)*/
		card.card_timer--;/*fps_factor*/	/*(スペカ時間経過)*/
		if (0 >= (card.card_timer)) /*1*/
		{
			card.card_timer 	= (0);/*(一時的に0にする)*/
			/*(もし次のスペカがあれば、スペカシステム側で次へ移行する。その際に card.card_timer に時間が加算される。)*/
			src->base_hp		= card.limit_health;		/* (とりあえず) */
		//	boss_destroy_check_type(h/*敵自体*/, DESTROY_CHECK_01_IS_TIME_OUT);/*	★ 攻撃の場合の死亡判定 	★ 時間切れの場合の死亡判定 */
			#if (1==USE_BOSS_JIKANGIRE)/*(使用予定あり。未検証)*/
			cg.state_flag |= JIKI_FLAG_16_0x8000_BOSS_JIKAN_GIRE;/*(時間切れフラグon)*/
			#endif
		}
	}
//	#if (0)/*(r36, NULLは登録できない。何もしない場合は NULL_keitai を登録する。)*/
//	if (NULL != card.boss_move_card_callback)
//	#endif
	{
		(card.boss_move_card_callback)(src);/*(ボス移動形態毎に、ボス移動処理を実行する)*/
	}
	boss_r36_yuudou(src);
	if (TUKAIMA_00_OFF != card.tukaima_used_number) 	/* 使い魔生成は必要？ */
	{
		tukaima_system_add_dolls(src);/*(複数の使い魔達の生成をする。使い魔は一人でなくて複数の方が多い。)*/
		card.tukaima_used_number = TUKAIMA_00_OFF;/*(生成完了したので off にする。)*/
	}
	/*---------------------------------------------------------
	ボスを攻撃した場合のフレームチェック
	-------------------------------------------------------
	ライフが少なくなるとボスがなかなか死なないのは、演出だそうです。
	てきとーに再現？
	-------------------------------------------------------
	ToDo:
	う－ん、最近はライフはやっぱりリニアに減ってて、
	表示だけlogなんじゃね？って気がしてきた。
	そういう風にしようかな。ゲームバランスとか色々変わるから。
	---------------------------------------------------------*/
	/*ボスを攻撃した場合のフレームチェック/カードモードチェック*/
//	boss_hp_frame_check(src);
//	static OBJ_CALL_FUNC(boss_hp_frame_check)/*(ボス本体)*/
	{
		s32 limit_max_hp_dec_boss_by_frame;
	//	u8 test_boss_hp_value;
		u16 test_boss_hp_value;
	//	test_boss_hp_value = (src->base_hp>>5) & 0xff; /* 8192/32 == 256 (r32) */
	//	test_boss_hp_value = (src->base_hp>>7) & 0xff; /* 32768/32 == 256 (r33) */
		test_boss_hp_value = (src->base_hp>>9) & 0x03ff;	/* ?/32 == 256*4 (r35u2-) */
		#if 1
	//	limit_max_hp_dec_boss_by_frame = (test_boss_hp_value>>2) | (0x0f);
	//	limit_max_hp_dec_boss_by_frame = (test_boss_hp_value>>2) | (0x08);
	//	limit_max_hp_dec_boss_by_frame = (test_boss_hp_value) | (0x10);/*(-r35u1)*/
		limit_max_hp_dec_boss_by_frame = (test_boss_hp_value) | (0x40);/*(r35u2-)*/
		#endif
		//
	//	if (limit_max_hp_dec_boss_by_frame < card.boss_hp_dec_by_frame)
	//	{
	//		card.boss_hp_dec_by_frame = limit_max_hp_dec_boss_by_frame;
	//	}
		card.boss_hp_dec_by_frame = psp_min(card.boss_hp_dec_by_frame, limit_max_hp_dec_boss_by_frame);
	}
//
	src->base_hp -= card.boss_hp_dec_by_frame;
	card.boss_hp_dec_by_frame = 0;/* 使ったので消す(フレーム単位) */
	if (card.limit_health >= src->base_hp)	/* 規定値以下になればカードモード解除 */
	{
		src->base_hp = card.limit_health;	/* 規定値未満にしない。 */
		src->callback_root_supeka		= boss_action_01_gekiha_go;
	//	boss_destroy_check_type(src, DESTROY_CHECK_00_WIN_BOSS);
		cb.callback_gu_draw_haikei		= cb.callback_gu_draw_haikei_modosu;
	}
}


/*---------------------------------------------------------
	[ボス行動03: カード開始移動]
	02カード開始移動
	カード開始位置までボスが移動する。
	立ち絵の処理も行う。
---------------------------------------------------------*/
static int aaa_wait;
static OBJ_CALL_FUNC(boss_action_03_move_start_position)
{
	#if 1/*(r32p)*/
//	bo ss_y uudou_idou_nomi(src);/*(r32p)*/ //	src->BOSS_DATA_04_toutatu_wariai256 -= (1); /* [約	4 秒]== 4.2666==(256/60[frame]) */
//	bo ss_y uudou_hiritu_keisan(src);
	boss_r36_yuudou(src);
	#if 0
//	if (0 > src->BOSS_DATA_04_toutatu_wariai256 )	/* ほぼ画面中心付近まで、移動した。 */
	if ((0==src->BOSS_DATA_04_toutatu_wariai256))
	{
//		src->BOSS_DATA_04_toutatu_wariai256 = (0);
	}
	#endif
	#endif/*(r32p)*/
	// ボス(立ち絵移動)
	#if 0/*(等速移動)*/
	kaiwa_sprite[1].center.y256 	= t256(272+128) - ((aaa_wait)<<9);
	#else/*(加減速移動)*/
	{
		u32 aaa = (aaa_wait)+(aaa_wait);
		aaa += 128;/*(上下入れ替え)*/
		aaa &= 0xff;/*(0...255)*/
	//	aaa = ((aaa)|(aaa<<8));/*(0...65535)*/
		aaa <<= 8;/*(0...65535)*/
		aaa = vfpu_ease_in_out65536(aaa);
		aaa += 32768;/*(上下入れ替え)*/
		aaa &= 0xffff;/*(0...65535)*/
		aaa += aaa;/*(2倍)*/
		kaiwa_sprite[1].center.y256 	= t256(272+128) - (aaa);
	}
	#endif
	aaa_wait--; /* ボス行動 */
	if (0 > aaa_wait)
	{
		src->callback_root_supeka		= boss_action_04_speka_kougeki;
	}
}

/*---------------------------------------------------------
	[ボス行動02: カード設定]
	03カード設定
	次のカードを始める為の初期設定を行う。
---------------------------------------------------------*/
/* ボスホームポジジョン。x座標。(カード開始時に必ずホームポジジョンへ移動する。) */
//#define BOSS_XP256		(t256(GAME_WIDTH/2))	/* 中心座標なので */
#define BOSS_XP256			(t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH/2))	/* 中心座標なので */
static OBJ_CALL_FUNC(boss_action_a02_new_card_set)
{
	REG_10_BOSS_SPELL_TIMER = (0);	/* カード生成を強制的に止める。 */
	bullets_to_hosi();		/* 総ての敵弾を、hosiアイテムに変える */
	/* 真中付近に退避 */
//	src->BOSS_DATA_04_to utatu_wariai256	= t256(  0);/* 初期化済みの必要あり */
//	src->BOSS_DATA_04_to utatu_wariai256	= t256(1.0);/* 初期化済みの必要あり */
//	src->BOSS_DATA_00_ta rget_x256			= BOSS_XP256; //t256(0); t256(153);
//	src->BOSS_DATA_01_ta rget_y256			= t256(16.0); //t256(0); src->center.y256;
	REG_02_DEST_X	= (BOSS_XP256);/*(t256()形式)*/
	REG_03_DEST_Y	= (t256(16.0));/*(t256()形式)*/
	boss_set_new_position(src);/*(誘導移動座標を設定)*/
	src->callback_root_supeka		= boss_action_03_move_start_position;
	// ボス
	kaiwa_sprite[1].center.x256 	= t256(256);
	aaa_wait	= (127);/*on*/
	//
	#if 1/* (新[カード始まる前に初期化]) 第0形態から、必ず呼ぶ筈。 */
	/* カード初期化 */
//	#if (0)/*(r36, NULLは登録できない。何もしない場合は NULL_keitai を登録する。)*/
//	if (NULL != my_card_resource[(card.address_set)].spell_init_callback)
//	#endif
	{
		(my_card_resource[(card.address_set)].spell_init_callback)(src);
	}
	#endif
	#if (1)
	/*---------------------------------------------------------
		カード名称(符名)表示
		カード背景は、まだ無いけど、将来的に拡張予定。
	---------------------------------------------------------*/
	//static void draw_card_name(void)
	{
		kanji_window_clear_line(ML_LINE_04);	/* 漢字ウィンドウの4行目(==3)の内容を消す。 */
		set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (ML_LINE_04)*(KANJI_FONT_18_HEIGHT_P2));	/* カーソルを4行目(==3)へ移動 */
	//
		if (NULL != my_card_resource[(card.address_set)].spell_str_name)
		{
			/* カード背景がある場合 */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_supeka;
			//
			// ""カードの譜名表示位置。
			{
			//	ml_font[(ML_LINE_04)].timer 	= ML_ON;	/*"表示ON"*/
				ml_font[(ML_LINE_04)].timer 	= byou60(5);	/* 約 5 秒 */
				ml_font[(ML_LINE_04)].x 		= (96);/* X位置 */
				ml_font[(ML_LINE_04)].y 		= (12);/* Y位置 */
			//	ml_font[(ML_LINE_04)].render	= NULL;
			//	ml_font[(ML_LINE_04)].str		= my_font_text;/*(デバッグ)*/	/*"表示on"*/
			}
			ml_font[(0)].haikei 		= (ML_HAIKEI_01_BOSS_SPELL_CARD);/* [青/ボスカード用背景]ボスカード用せりふ背景on */
			cg.msg_time = byou60(5);	/* 約 5 秒 */
			strcpy(my_font_text, my_card_resource[(card.address_set)].spell_str_name);
			kanji_color(/*int color_type*/(7)|STR_CODE_NO_ENTER);	/* 改行しない */
			kanji_draw();
		}
		else
		{
			/* カード背景が無い場合 */
			cb.callback_gu_draw_haikei = cb.callback_gu_draw_haikei_modosu;
		}
	}
	#endif
}

/*---------------------------------------------------------
	[カードシステム内に移動予定]
	-------------------------------------------------------
	ボスを攻撃した場合の共通ルーチン
	-------------------------------------------------------
	OBJ *src;	ボス敵自体
	OBJ *tama;	自弾
---------------------------------------------------------*/

/*static*/static/*global*/	void s_callback_hit_boss(OBJ/**/ *src, OBJ *tama)
{
	/* ボス & 中-ボスに自弾があたった場合の火花エフェクトを登録(現在Gu部分がないので描画してない) */
	set_REG_DEST_XY(tama);/* 発弾位置 座標xy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* 先に実行した方が速い */
//
	/* 上と分離した方がコード効率があがる。 */
	{
		card.boss_hp_dec_by_frame += /*w->*/tama->kougeki_ti; /* 攻撃して体力減らす(強さ分引く) */
	}
}


/*---------------------------------------------------------
	[ボス行動00: 会話終了待ち]
	[会話中の形態]攻撃不可、会話終了まで待機する。
	-------------------------------------------------------
	待機が終わったら、攻撃可能にする。
---------------------------------------------------------*/

static OBJ_CALL_FUNC(boss_action_a01_kaiwa_syuuryou_mati)
{
	/* ボス戦闘前の会話終了済み? */
	if (SCENE_NUMBER_0x0800_BOSS_TATAKAU==(cg.state_flag & 0x0f00))
	{
		/*(プレイヤー弾受け付けコールバックを登録し、ボスへの攻撃が可能になる。)*/
		src->callback_hit_teki		= s_callback_hit_boss;/*(プレイヤー弾受け付けコールバック登録)*/
		src->callback_root_supeka	= boss_action_a02_new_card_set;/*(カードをセットする状態へ移行。)*/
		card_address_incliment();/*(0枚目→1枚目のカードをセットする。)*/
		card_set_boss_move_callback();/*(ボス移動コールバック登録)*/
	}
	boss_r36_yuudou(src);/*(要るかも？？)*/
}

/*---------------------------------------------------------
	弾の範囲を「標準」に設定
	-------------------------------------------------------
	set_default_bullet_clip
---------------------------------------------------------*/
static OBJ_CALL_FUNC(init_00_boss_clip000)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET);						/*(横は標準範囲)*/
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH);	/*(横は標準範囲)*/
	rect_clip.bullet_clip_min.y256 = t256(-32); 			/*(上は標準範囲)*/
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT); 	/*(下は標準範囲)*/
}

/*---------------------------------------------------------
	弾の範囲を「上と横の広範囲」に設定
	-------------------------------------------------------
	set_aya_bullet_clip
---------------------------------------------------------*/
static OBJ_CALL_FUNC(init_00_boss_clip111)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET)					  + t256(-100);/*(横は広範囲)*/
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH) + t256( 100);/*(横は広範囲)*/
	rect_clip.bullet_clip_min.y256 = t256(-256);			/*(上は広範囲)*/		/* あや */
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT); 	/*(下は標準範囲)*/
}

/*---------------------------------------------------------
	弾の範囲を「上だけ広いタイプ」に設定
	-------------------------------------------------------
	set_cirno_bullet_clip
---------------------------------------------------------*/

static OBJ_CALL_FUNC(init_00_boss_clip222)/* call from load_stage.c */
{
	rect_clip.bullet_clip_min.x256 = t256(GAME_X_OFFSET);						/*(横は標準範囲)*/
	rect_clip.bullet_clip_max.x256 = t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH);	/*(横は標準範囲)*/
	rect_clip.bullet_clip_min.y256 = t256(-256);			/*(上は広範囲)*/		/*(てすとチルノ)*/
	rect_clip.bullet_clip_max.y256 = t256(GAME_HEIGHT); 	/*(下は標準範囲)*/
}

/*---------------------------------------------------------
	外部インターフェイス。
	-------------------------------------------------------
	道中用の、弾の範囲を「標準」に設定。(from load_stage.c)
---------------------------------------------------------*/
global void set_default_bullet_clip(void)	/* 標準タイプ */
{
	init_00_boss_clip000(NULL); 			/* 弾の範囲を標準に設定 */
}


/*---------------------------------------------------------
	会話システムからボス敵(キャラ)を読みこんで、
	基本的な初期化を行う。
---------------------------------------------------------*/
	#if 0/* 初期化済みの必要あり(?) */
	h->math_vector.x256 					= t256( 0);
	h->math_vector.y256 					= t256( 0);
	#endif
extern void stage_boss_load_texture(void);
// src/core/douchu/boss.h の初期化も参照する事。
extern OBJ_CALL_FUNC(root_boss_mover);
global void called_from_kaiwa_system_boss_load(int boss_number)
{
	/*(ボス番号は 0-7 のいずれかに勝手に固定(r36現在)。)*/
	boss_number &= (8-1);
	/*(ボス本体の移動範囲を制限する。)*/
	{
		const char boss_clip_y_pixel[4] =
		{
			 (64),/*(easy)*/
			 (80),/*(normal)*/
			 (96),/*(hard)[r39]*/
			 (96),/*(lunatic)*/
		};
		/* boss_rect_init */
		rect_clip.boss_clip_min.x256	= t256(GAME_X_OFFSET)+t256( 			0)+t256(24);
		rect_clip.boss_clip_max.x256	= t256(GAME_X_OFFSET)+t256(GAME_320_WIDTH)-t256(24);
		rect_clip.boss_clip_min.y256	= t256(0);
		rect_clip.boss_clip_max.y256	= ((boss_clip_y_pixel[((cg.game_difficulty)&3)])<<8);/*t256()*/
	}
	//----[BOSS]
	OBJ *h;
	h					= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(ボス本体)*/
	{
		h->obj_type_set 				= BOSS_00_11;/*(再定義の必要あり(?))*/
		h->callback_mover				= root_boss_mover;/*(ボス「ルート処理」を追加。ボスの全ての行動は「ルート処理」から処理する。)*/
		h->callback_hit_teki			= NULL;/*(ダミーコールバック登録) (会話時にプレイヤー弾は受け付けない)*/
		h->callback_root_supeka 		= boss_action_a01_kaiwa_syuuryou_mati;/*(会話終了待ち)*/
	//
		h->BOSS_DATA_03_kougeki_anime_count 		= (0);	/* 攻撃アニメーション用カウンタ / 0以下なら移動アニメーション */
		h->BOSS_DATA_05_boss_base_state777			= (0);	/*ST_00*/	/*(初期値を0にする。) (-1) */
		//------------ カード関連
		{
			static const u16 boss_start_card_address[(8)] =/* カードアドレスが256種類を超えたので最低でも u16 は必要。 */
			{	/* ボス開始カード番号(easy) */
				CARD_ADDRESS_00E_chrno_000, 	/* チルノ(?) */ 	/* エキストラ用(boss0) */
				CARD_ADDRESS_00E_alice_000, 	/* アリス */
				CARD_ADDRESS_00E_mima_000,		/* 魅魔 */
				CARD_ADDRESS_00E_kaguya_000,	/* 輝夜 */
			//
				CARD_ADDRESS_00E_aya_000,		/* 文 */
				CARD_ADDRESS_00E_pacheA_000,	/* パチェ */
				CARD_ADDRESS_00E_sakuya_000,	/* 咲夜 */
				CARD_ADDRESS_00E_pacheA_000,	/* フラン(?) */ 	/* ファンタズム用(boss7) */
			};
			card.address_set =
				#if (1)/*(ボスが違えばスペカも違う)*/
				boss_start_card_address[boss_number]	/*(基準位置)*/
				#endif
				#if (1)/*(難易度別に違うスペカを撃つ)*/
				+ ((cg.game_difficulty)&0x03);	/*(難易度別オフセット)*/
				#endif
			if (5==boss_number)/*(5==パチェの場合、プレイヤー毎に違うスペカを撃つ)*/	/*(パチェの場合オフセット)*/
			{
				u8 aaa;
				aaa = ((((cg_game_select_player)&(4-1)))<<2);/* 4種(0 ... 3)に制限してから、4倍する。(rank == E,N,H,L) */
				card.address_set += ((aaa<<3)+(aaa));/*(aaa *= 9; 9倍する。r36_gekiha 含めスペカ領域 9 種類。)*/
			}
			h->base_hp					= (my_card_resource[(card.address_set)].spell_life);	/* 全体の体力 */	/*(再定義の必要あり)*/
			/*(初回の規定値)*/
			card.limit_health			= (h->base_hp);
		}
	}
			card.spell_used_number		= SPELL_00; 	/* カード生成終了フラグ */
			card.address_temporaly		= (0);			/*(SPELL_08_rumia-1)*/ /*0*/
			#if (0)//(1)
			card_set_boss_move_callback();
			#else/*(たぶん、同じ)*/
			card.boss_move_card_callback = NULL_keitai;/*(会話終了後に形態が設定されるので、それまで何もしない。)*/
			#endif
	//
	/*(リミット分引く)*/		/*(初回攻撃)の攻撃分引く */
	set_new_limit();			/* 通常攻撃(初回攻撃)の攻撃分 */
	// ボステクスチャ読み込み
	stage_boss_load_texture();

	/*---------------------------------------------------------
		「datで設定したボス出現位置」から、
		「ボス出現固定位置」まで移動する。
		これらの初期設定を行う。
	---------------------------------------------------------*/
	// 出現座標の初期設定。
	REG_02_DEST_X	= (BOSS_XP256);/*(t256()形式、ボス出現固定位置)*/
	REG_03_DEST_Y	= (t256(16.0));/*(t256()形式、ボス出現固定位置)*/
	boss_set_new_position(h);/*(誘導移動座標を設定)*/
}

	#if (0)/*(デバッグ)*/
	kanji_window_clear_line(0); 			/* 漢字ウィンドウの1行目(==0)の内容を消す。 */
	kanji_cursor_move_home_position();		/* カーソルを1行目(==0)へ移動 */
	{
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_02_JIKI_SPELL_CARD);/* [赤/自機カード用背景]せりふ背景on */
		cg.msg_time 				= (65536);	/* 約 18 分 */
		strcpy(my_font_text, "ボス、ロードしたよ。" "\n");
		kanji_color((7)|STR_CODE_NO_ENTER);
		kanji_draw();
	}
	#endif
	//
	#if (0)/*(デバッグ)*/
	{	psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load stage: STAGE%d" "\\n"
			"ボス、ロードテストOK。", cg.game_now_stage);
	}
	#endif
/*---------------------------------------------------------
	外部インターフェイス。
	-------------------------------------------------------
	ボスがスペカを変えた場合、同期してボスオプションを消す。
	ボスオプション、消えるかどうかの判断。
---------------------------------------------------------*/
global int check_boss_action(void)
{
	OBJ *src;
	src 				= &obj99[OBJ_HEAD_02_0x0900_TEKI_FIX+TEKI_OBJ_00_BOSS00_HONTAI];/*(ボス本体)*/
	//
	if (boss_action_04_speka_kougeki != src->callback_root_supeka)/*ボス、スペカで攻撃中。以外*/ /*(カード中以外)*/
	{
		/*(攻撃中以外なら消す)*/
		return (1);/*(消す)*/
	}
		/*(攻撃中なら消さない)*/
	return (0);/*(消さない)*/
}
