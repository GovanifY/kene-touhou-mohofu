
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	輝夜 オプション
	-------------------------------------------------------
	ボス共通規格使用データー:
		BOSS_DATA_00_target_x256	目標x座標 x位置として使用。
		BOSS_DATA_01_target_y256	目標y座標 y位置として使用。
		BOSS_DATA_05_move_jyumyou	user_data05
---------------------------------------------------------*/

// バグ対策。 user_data02 良くワカンナイけど空けとく。()
#define rotate_angle1024			user_data03 	/* ボスを中心として、回転角度。(下CCW1024形式) */
#define shot_angle65536 			user_data06 	/* */
#define now_hankei_dot				user_data07 	/* 半径 */



//		src->shot_angle65536 += (65536/18); /* ショットを撃つ方向を、回転させる。 */
//	//
//		HATSUDAN_01_speed256				= (t256(1.5))+(((difficulty)<<6));	/* 弾速 */
//		HATSUDAN_03_angle65536				= ((src->shot_angle65536) );			/* */
//		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
//		HATSUDAN_06_n_way					= (2+difficulty);					/* [2-5way] */
//		HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* 密着弾 */
//		bullet_regist_multi_vector_direct();

/*---------------------------------------------------------
	子供魔方陣 カード
---------------------------------------------------------*/
extern void shot_common_gin_tama(SPRITE *src);
static void shot_kaguya_doll(SPRITE *src)
{
/*
0:easy	   f  16回に1回発弾。
1:normal   7   8回に1回発弾。
2:hard	   3   4回に1回発弾。
3:lunatic  1   2回に1回発弾。
*/
	if ( 0==(((1<<(4-REG_0f_GAME_DIFFICULTY))-1) & ((src->jyumyou>>3))) )
//	if ( (REG_0f_GAME_DIFFICULTY+1) > ((src->jyumyou>>3)&0x03) )
	{
		set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
		REG_0e_REG6 = ((src->shot_angle65536) );/*(1024to65536)*/
		shot_common_gin_tama(src);/*(共通部分)*/
	}
}

/*---------------------------------------------------------
	子供魔方陣 移動 共通部分
---------------------------------------------------------*/

static void move_kaguya_doll_common(SPRITE *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			src->shot_angle65536 -= (16*64);		/* cv1024r(10)*/
			shot_kaguya_doll(src);
		}
	}
	set_timeout_alpha(src);
}


/*---------------------------------------------------------
	子供魔方陣01 移動
---------------------------------------------------------*/
static void move_kaguya_doll01(SPRITE *src)
{
	move_kaguya_doll_common(src);
	/* オプション位置、回転移動 */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* オプション位置、移動 */
	move_tukaima00(src);
	//	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	//	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
}


/*---------------------------------------------------------
	子供魔方陣02 移動
---------------------------------------------------------*/

static void move_kaguya_doll02(SPRITE *src)
{
	move_kaguya_doll_common(src);
	/* オプション位置、回転移動 */
	/* オプション位置、移動 */
	if (0x2ff == src->BOSS_DATA_05_move_jyumyou)
	{
		voice_play(VOICE11_BOSS_KIRARIN, TRACK02_ALEART_IVENT);/*テキトー*/
		voice_play(VOICE11_BOSS_KIRARIN, TRACK03_SHORT_MUSIC);/*テキトー*/
	}
	if (0x2ff > src->BOSS_DATA_05_move_jyumyou)
	{
		move_tukaima00(src);
	//	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	//	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
	}
}

/*---------------------------------------------------------
	宝物 敵移動
---------------------------------------------------------*/

static void move_kaguya_houmotu_doll(SPRITE *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
	//
	set_timeout_alpha(src);
	//
	/* オプション位置、回転移動 */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* オプション位置、移動 */
	move_tukaima00(src);
	//	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	//	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
}

/*---------------------------------------------------------
	子供魔方陣01/02 登録共通部分
---------------------------------------------------------*/

static void regist_kaguya_dolls_common(SPRITE *h)
{
	h->m_Hit256R			= ZAKO_ATARI16_PNG;
	h->type 				= BOSS_20_YOUSEI1_SYOUMEN;
	h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
	h->callback_loser		= lose_tukaima_00;
	h->callback_hit_teki	= callback_hit_zako;
	//
	h->shot_angle65536		= (0);
	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
	{
		static const unsigned int difficulty_score_tbl[4] = // スコアテーブル
		{
			score( 100000), 	/* easy 10万 (計 50万==5x 10万) */
			score( 300000), 	/* norm 30万 (計150万==5x 30万) */
			score( 500000), 	/* hard 50万 (計250万==5x 50万) */
			score(1000000), 	/* luna100万 (計500万==5x100万) */
		};
		h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* 難易度によるが、かなり稼げる。 */
//		h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
	}
}

/*---------------------------------------------------------
	子供魔方陣02 敵を追加する
---------------------------------------------------------*/

void add_zako_kaguya_dolls02(SPRITE *src)
{
	#define ADD_ANGLE (43)		/* 1周を24分割した角度、 42.66 == 1024/24 */	/* 1日 は 24 時間 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=((1024/2)-(1024*1/6)); i_angle1024<((1024/2)+(1024*1/6)); i_angle1024 += (ADD_ANGLE)) /* 配置範囲(上を中心として1/3周) */
	/* 夜は 1日 の 1/3 時間 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->rotate_angle1024 	= i_angle1024;
			regist_kaguya_dolls_common(h);
			h->callback_mover		= move_kaguya_doll02;
			// 基準位置(画面下方中心)
			h->BOSS_DATA_00_target_x256 		= (t256((GAME_WIDTH/2)));//;(src->cx256);
			h->BOSS_DATA_01_target_y256 		= (t256(272)		   );//;(src->cy256);
			//
			h->now_hankei_dot		= (32*7);			/* 初期半径 224[dot] 256[dot] */
		//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 		/* 制限時間 */
		//	h->BOSS_DATA_05_move_jyumyou		= (0x02ff)+(i<<6);	/* 制限時間 */
			h->BOSS_DATA_05_move_jyumyou		= (0x02ff)-(1024/3)+(i_angle1024);	/* 制限時間 */
		}
	}
}
/*---------------------------------------------------------
	子供魔方陣01 敵を追加する
---------------------------------------------------------*/

void add_zako_kaguya_dolls01(SPRITE *src)
{
//	#define ADD_ANGLE (341) 	/* 1周を 3分割した角度、341.33 == 1024/3  */
//	#define ADD_ANGLE (171) 	/* 1周を 6分割した角度、170.66 == 1024/6  */
	#define ADD_ANGLE (205) 	/* 1周を 5分割した角度、204.80 == 1024/5  */
//	#define ADD_ANGLE (51)		/* 1周を20分割した角度、 51.20 == 1024/20 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* 一周 */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->rotate_angle1024 	= i_angle1024;
			regist_kaguya_dolls_common(h);
			h->callback_mover		= move_kaguya_doll01;
			// 基準位置(輝夜)
			h->BOSS_DATA_00_target_x256 		= (src->cx256);
			h->BOSS_DATA_01_target_y256 		= (src->cy256);
			//
		//	h->now_hankei_dot		= (45); 		/* 初期半径 45[dot] */
			h->now_hankei_dot		= (45) +  48-(REG_0f_GAME_DIFFICULTY<<4);		/* 初期半径 45[dot] */
		//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 	/* 制限時間 */
			h->BOSS_DATA_05_move_jyumyou		= (0x02ff); 	/* 制限時間 */
		}
	}
}

/*---------------------------------------------------------
	宝物 敵を追加する
---------------------------------------------------------*/

void add_zako_kaguya_houmotsu(SPRITE *src)
{
	int i_type;
	i_type=0;
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* 一周 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->rotate_angle1024 	= i_angle1024;
//			regist_kaguya_dolls_common(h);
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_20_YOUSEI1_SYOUMEN + (i_type);	i_type++;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//
			h->callback_loser		= lose_tukaima_00;
			h->callback_hit_teki	= callback_hit_zako;
			//
			h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
			h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40万 */
			//
			h->callback_mover		= move_kaguya_houmotu_doll;
			// 基準位置(輝夜)
			h->BOSS_DATA_00_target_x256 		= (src->cx256);
			h->BOSS_DATA_01_target_y256 		= (src->cy256);
			//
			h->now_hankei_dot		= (45); 			/* 初期半径 45[dot] */
			h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 		/* 制限時間 */
		}
	}
}
