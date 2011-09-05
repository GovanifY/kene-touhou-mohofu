
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	使い魔系は、規格を共通化する必要がある。
	-------------------------------------------------------
	魅魔 オプション
	輝夜 オプション
	-------------------------------------------------------
	咲夜 ばら撒き1弾 オプション
	-------------------------------------------------------
	ばら撒き 速度遅い
	寿命	 長い
	-------------------------------------------------------
    めも1:密着米(5)弾
        青米弾                  緑米弾
        右回り                  左回り
        (+90度)                 (-45度)

            緑米弾          青米弾
            左回り          右回り
            (+45度)         (0度)
    -------------------------------------------------------
    めも2:密着クナイ(6)弾   [横の壁で反射する]
        赤クナイ弾              青クナイ弾
        右回り                  左回り

            青クナイ弾      赤クナイ弾
            左回り          右回り
    -------------------------------------------------------
    咲夜 ばら撒き2弾 オプション
    -------------------------------------------------------
    ばら撒き 速度速い
    寿命     短い
    -------------------------------------------------------
    めも1:密着米(5)弾
        青米弾                  緑米弾
        右回り                  左回り
        (+90度)                 (-45度)

            緑米弾          青米弾
            左回り          右回り
            (+45度)         (0度)
    -------------------------------------------------------
    めも2:密着クナイ(6)弾   [横の壁で反射する]
        赤クナイ弾              青クナイ弾
        右回り                  左回り

            青クナイ弾      赤クナイ弾
            左回り          右回り
	-------------------------------------------------------
	ボス共通規格使用データー:
		BOSS_DATA_00_target_x256	目標x座標 x位置として使用。
		BOSS_DATA_01_target_y256	目標y座標 y位置として使用。
		BOSS_DATA_05_move_jyumyou	user_data05
---------------------------------------------------------*/



/*---------------------------------------------------------
	使い魔敵やられ
---------------------------------------------------------*/

static/*global*/ void lose_tukaima_00(OBJ *src)
{
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}

/*---------------------------------------------------------
	使い魔敵やられ
---------------------------------------------------------*/
#if 0
static void lose_mima_doll(OBJ *src)
{
//	item_create_for_boss(src, ITEM_CREATE_MODE_02);/* easyはボムを出さなくて済む位軟らかくした */
	#if 1
	item_create(src, SP_ITEM_05_TENSU, 5, ITEM_MOVE_FLAG_06_RAND_XY);
//	item_create(src, SP_ITEM_00_P001, 5, ITEM_MOVE_FLAG_06_RAND_XY);
	#else
	{	int i;
		for (i=0; i<(5); i++)
		{
			item_create_random_table(src);
		}
	}
	#endif
}
#endif


// バグ対策。 user_data02 良くワカンナイけど空けとく。()

// 魅魔: 輝夜:
#define rotate_angle1024			user_data03 	/* ボスを中心として、回転角度。(下CCW1024形式) */
#define shot_angle65536 			user_data04 	/*(使い魔共通) ショットを撃つ方向を保持 */
#define now_hankei_256				user_data06 	/* 現在半径t256()形式[pixel] */
#define set_hankei_256				user_data07 	/* 指定半径t256()形式[pixel] */

// 咲夜: ばら撒き1弾 / ばら撒き2弾

enum
{
	AO_AKA_MASK 			= 0x01,
	MODE_TYPE_00_BARAMAKI01 = 0x00,
	MODE_TYPE_01_BARAMAKI02 = 0x02,
	MODE_TYPE_MASK			= 0x02
};
	#define AO_OR_AKA	(src->set_hankei_256&AO_AKA_MASK)/* [青赤情報] */
//
/*---------------------------------------------------------
	子供魔方陣 カード
---------------------------------------------------------*/
static void tama_uti_sakuya_tukaima(OBJ *src)
{
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
		/* ここは 6 wayではなくて、 3 way を2回追加する。でないとプライオリティーが変になる。 */
	//	HATSUDAN_06_n_way					= (2+difficulty);						/* [2-5way](r32) */
	//	HATSUDAN_06_n_way					= (3);		/* [破壊] */				/* [5way] */ /* [3way x 2] */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/160); 					/* 密着弾(r32)6.4 */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/256); 					/* 密着弾(もっと密着)4 */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/512); 					/* 密着弾(もっと密着)2 */
// MODE_TYPE_00_BARAMAKI01
//		HATSUDAN_01_speed256				= (t256(1.5))+((((REG_0f_GAME_DIFFICULTY))<<6));		/* 弾速(r32/r33) */
		HATSUDAN_01_speed256				= (t256(0.75))+((((REG_0f_GAME_DIFFICULTY))<<5));		/* 弾速(r35) */
// MODE_TYPE_01_BARAMAKI02
	//	HATSUDAN_01_speed256				= (t256(1.5))+(((difficulty)<<6));		/* 弾速(r32) */
//		HATSUDAN_01_speed256				= (t256(2.0))+((((REG_0f_GAME_DIFFICULTY))<<6));		/* 弾速(r33) */
	//	HATSUDAN_01_speed256				= (t256(1.0))+((((REG_0f_GAME_DIFFICULTY))<<5));		/* 弾速(r35) */
//
	if (MODE_TYPE_00_BARAMAKI01==((src->set_hankei_256)&MODE_TYPE_MASK))
	{
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI) + (AO_OR_AKA) + (AO_OR_AKA); 	/* [青緑米弾] */
		HATSUDAN_06_n_way					= (5);		/* [破壊] */				/* [5way] */
	}
	else
	{
		HATSUDAN_01_speed256 += (t256(0.25));
		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA) + (AO_OR_AKA) + (AO_OR_AKA);	/* [青赤クナイ弾] */
		HATSUDAN_06_n_way					= (6);		/* [破壊] */				/* [3way x 2] */
	}
	HATSUDAN_02_speed_offset			= (0);/*(テスト)*/
	HATSUDAN_03_angle65536				= (src->shot_angle65536);			/* [破壊] */
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	{
		const u16 kakusan_tbl[4] =
		{(int)(65536/512), (int)(65536/512), (int)(65536/256), (int)(65536/128)};		// Lunatic はかなり拡散する。
		HATSUDAN_07_div_angle65536		= kakusan_tbl[(REG_0f_GAME_DIFFICULTY)];		/* [破壊] */		/* 密着弾(もっと密着)2 */
	}
	hatudan_system_regist_n_way();		/* (r33-) */
}


//		HATSUDAN_01_speed256				= (t256(1.5))+(((difficulty)<<6));	/* 弾速 */
//		HATSUDAN_03_angle65536				= ((src->shot_angle65536) );			/* */
//		HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
//		HATSUDAN_06_n_way					= (2+difficulty);					/* [2-5way] */
//		HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* 密着弾 */
//		bullet_regist_multi_vector_direct();

static void tama_uti_mima_tukaima(OBJ *src)
{
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
//
//	/*[r35要半分速]*/HATSUDAN_01_speed256				= t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6);		/* 弾速 */
	/*[r35半分速]*/HATSUDAN_01_speed256 				= t256(0.75)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* 弾速 */
	HATSUDAN_02_speed_offset			= (0);/*(テスト)*/
	HATSUDAN_03_angle65536				= (src->shot_angle65536);		/* [破壊] */
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
	HATSUDAN_06_n_way					= (2+(REG_0f_GAME_DIFFICULTY)); 	/* [破壊] */	/* [2-5way] */
	HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* [破壊] */	/* 密着弾 */
//	hatudan_system_regist_katayori_n_way();/* (r33-) */
	hatudan_system_regist_n_way();		/* (r33-) */
}

extern void shot_common_gin_tama(OBJ *src);
static void tama_uti_kaguya_tukaima(OBJ *src)
{
/*
0:easy	   f  16回に1回発弾。
1:normal   7   8回に1回発弾。
2:hard	   3   4回に1回発弾。
3:lunatic  1   2回に1回発弾。
*/
	if (0==(((1<<(4-REG_0f_GAME_DIFFICULTY))-1) & ((src->jyumyou>>3))))
//	if ((REG_0f_GAME_DIFFICULTY+1) > ((src->jyumyou>>3)&0x03))
	{
		set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
		REG_0e_REG6 = (src->shot_angle65536);/*(1024to65536)*/
		shot_common_gin_tama(src);/*(共通部分)*/
	}
}

/*---------------------------------------------------------
	使い魔敵 移動のみ
---------------------------------------------------------*/
static /*global*/ void move_tukaima00(OBJ *src)
{
	//------------------
	HATSUDAN_01_speed256	= (((src->now_hankei_256)));/*(t256形式)*/
	HATSUDAN_03_angle65536	= deg1024to65536((src->rotate_angle1024));
	sincos256();/*(破壊レジスタ多いので注意)*/
	src->cx256 = (src->BOSS_DATA_00_target_x256) + REG_03_DEST_Y;//sin_value_t256	// 縦
	src->cy256 = (src->BOSS_DATA_01_target_y256) + REG_02_DEST_X;//cos_value_t256	// 横
	//------------------
	//define now_hankei_256 t256(45)			/* 半径 */
	if ((src->set_hankei_256) > (src->now_hankei_256)) {	src->now_hankei_256 += t256(1); };
}

/*---------------------------------------------------------
	オプション位置、回転移動 共通部分
---------------------------------------------------------*/

static void move_option_rotate(OBJ *src)
{
	/* オプション位置、回転移動 */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* オプション位置、移動 */
	move_tukaima00(src);
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_T05_sakuya_doll(OBJ *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			src->shot_angle65536 += ((AO_OR_AKA)?(-(65536/18)):((65536/18)));	/* ショットを撃つ方向を、回転させる。 */
			tama_uti_sakuya_tukaima(src);
		}
	}
	#if 1
	/* 魔方陣回転アニメーション */
//	src->rotationCCW1024--;/* 右回り */
	src->rotationCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->rotationCCW1024);
	#endif
	set_timeout_alpha(src);
	/* オプション位置、回転移動 */
	/* オプション位置、移動 */
	move_tukaima00(src);
}

/*---------------------------------------------------------
	子供魔方陣 移動 共通部分
---------------------------------------------------------*/

static void move_kaguya_doll_common(OBJ *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			src->shot_angle65536 -= ((int)(65536/64));		/* cv1024r(10)*/
			tama_uti_kaguya_tukaima(src);
		}
	}
	set_timeout_alpha(src);
}


/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/
//	if ((unsigned int)(0x01ff/3) < src->BOSS_DATA_05_move_jyumyou)/* 最後の 1/3 時間は撃って来ない。 */

static void move_T04_mima_doll(OBJ *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			src->shot_angle65536 += (65536/18); /* ショットを撃つ方向を、回転させる。 */
			tama_uti_mima_tukaima(src);
		}
	}
	set_timeout_alpha(src);
	move_option_rotate(src);
}


/*---------------------------------------------------------
	子供魔方陣01 移動
---------------------------------------------------------*/
static void move_T02_kaguya_doll(OBJ *src)
{
	move_kaguya_doll_common(src);
	move_option_rotate(src);
}


/*---------------------------------------------------------
	子供魔方陣02 移動
---------------------------------------------------------*/

static void move_T03_kaguya_doll(OBJ *src)
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
	}
}

/*---------------------------------------------------------
	宝物 敵移動
---------------------------------------------------------*/

static void move_T01_kaguya_houmotu_doll(OBJ *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
	set_timeout_alpha(src);
	move_option_rotate(src);
}
/*---------------------------------------------------------
	子供魔方陣01/02 登録共通部分
---------------------------------------------------------*/

static void set_score_common(OBJ *h)
{
	static const unsigned int difficulty_score_tbl[4] = // スコアテーブル
	{
	//	score( 100000), 	/* easy 10万 (計 50万==5x 10万) (r32) */
	//	score( 300000), 	/* norm 30万 (計150万==5x 30万) (r32) */
	//	score( 500000), 	/* hard 50万 (計250万==5x 50万) (r32) */
	//	score(1000000), 	/* luna100万 (計500万==5x100万) (r32) */
		score(	10000), 	/* easy 1万 (計 5万==5x 1万) (r33) */
		score(	30000), 	/* norm 3万 (計15万==5x 3万) (r33) */
		score(	50000), 	/* hard 5万 (計25万==5x 5万) (r33) */
		score( 100000), 	/* luna10万 (計50万==5x10万) (r33) */
	};
	h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* 難易度によるが、かなり稼げる。 */
//	h->base_score			= adjust_score_by_difficulty(score( 50000));	/* 5万 (計30万==6x5万) (r33) */
//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) (r32) */
//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
}

/*---------------------------------------------------------
	子供魔方陣01/02 登録共通部分
---------------------------------------------------------*/
//	#define ADD_ANGLE (341) 	/* 1周を 3分割した角度、341.33 == 1024/3  */
//	#define ADD_ANGLE (171) 	/* 1周を 6分割した角度、170.66 == 1024/6  */
//	#define ADD_ANGLE205 (205)	/* 1周を 5分割した角度、204.80 == 1024/5  */
//	#define ADD_ANGLE (51)		/* 1周を20分割した角度、 51.20 == 1024/20 */
//	const int add_angle = (ADD_ANGLE205);	/* 加算角度 */
	//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//	#define ADD_ANGLE043 (43)		/* 1周を24分割した角度、 42.66 == 1024/24 */	/* 1日 は 24 時間 */

typedef struct
{
	int loop_start; 	/*(開始角度)*/
	int loop_stop;		/*(終了角度)*/
	int loop_step;		/*(増分角度)*/
	int add_pattern;	/*(グラ加算量)*/
	//
	int limit_hankei_256;	/*(最大半径)*/
	int limit_move_jyumyou;
	void (*move_callback)(OBJ *sss);
	int dummy;
} AAA_BBB;
static AAA_BBB AAA[8] =
{
/*T00*/ {0, 0, 0, (0), 0, 0, 0, 0,},
/*T01*/ {0, 						(1024), 						(1024/5),	(1), t256(45),		(0x01ff),			move_T01_kaguya_houmotu_doll,	0,},	/* 指定半径 45[pixel] */	/* 制限時間 */
/*T02*/ {0, 						(1024), 						(1024/5),	(0), t256(45+48),	(0x02ff),			move_T02_kaguya_doll,			0,}, /* (0x01ff)制限時間 */ /* 一周 */
/*T03*/ {((1024/2)-(1024*1/6)), 	((1024/2)+(1024*1/6)),			(1024/24),	(0), t256(32*7),	(0x02ff)-(1024/3),	move_T03_kaguya_doll,			0,}, /* 指定半径 224[pixel] 256[pixel] */ /* 配置範囲(上を中心として1/3周) */
/*T04*/ {0, 						(1024), 						(1024/5),	(0), t256(45),		(0x02ff),			move_T04_mima_doll, 			0,}, /* 指定半径 45[pixel] */	/* 制限時間 / 予想以上に速く消える位に。(r33) (0x01ff) */ /* 一周 */
/*T05*/ {((1024*3/4))+0,			((1024*3/4)+((1024/2)+1)),		(1024/6),	(0), t256(64),		(0x04ff),			move_T05_sakuya_doll,			0,}, /* 4つ */ /* 制限時間(長い) (0x03ff) */
/*T06*/ {((1024*3/4))+0,			((1024*3/4)+((1024/2)+1)),		(1024/6),	(0), t256(64),		(0x04ff),			move_T05_sakuya_doll,			0,}, /* 4つ */ /* 制限時間(長い) (0x03ff) */
/*T07*/ {0, 0, 0, (0), 0, 0, 0, 0,},
};


		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
//			h->base_hp				= ((2*1024)-1); 	/* 8192==(8*1024) (r36)やわらかい */
//			h->base_score			= adjust_score_by_difficulty(score( 400000));	/* 40万 */

//	const int add_angle = (ADD_ANGLE205);	/* 加算角度 */


/*---------------------------------------------------------
	(使い魔システム)敵を追加する。
---------------------------------------------------------*/

global void tukaima_system_add_dolls(OBJ *src)
{
	card.tukaima_used_number &= 0x07;
	//
	int jjj;
	jjj=0;
	int i_type;
	i_type=0;
	int i_angle1024;	/* 積算角度 */
	for (
		i_angle1024=AAA[card.tukaima_used_number].loop_start;
		i_angle1024<AAA[card.tukaima_used_number].loop_stop;
		i_angle1024 += AAA[card.tukaima_used_number].loop_step
		) /* 一周 */
	{
		OBJ *h;
		h							= obj_add_A01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->rotate_angle1024 	= ((i_angle1024)&(1024-1));
			// 基準位置(輝夜)
			h->BOSS_DATA_00_target_x256 = (src->cx256);
			h->BOSS_DATA_01_target_y256 = (src->cy256);
			//
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->obj_type_set 		= BOSS_20_YOUSEI1_SYOUMEN;
			#if (1)/*(追加部分)*/
			h->obj_type_set 		+= (i_type);
			i_type += (AAA[card.tukaima_used_number].add_pattern);
			#endif
			h->atari_hantei 		= (1/*スコア兼用*/);
			//
			h->callback_loser		= lose_tukaima_00;//lose_mima_doll;
			h->callback_hit_teki	= callback_hit_zako;
			//
			h->now_hankei_256		= t256(0);			/* 現在半径  0[pixel] */
			h->shot_angle65536		= (0);
		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
		//	h->base_hp				= ((2*1024)-1); 	/* 8192==(8*1024) (r36)やわらかい */
		//	h->base_hp				= ((3*1024)-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
		//	h->base_hp				= (((difficulty+1)<<10)-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
			/* 難度上がると微妙に硬くなる(==倒すのにそれだけ時間がかかる) */
		//	h->base_hp				= (((1024)+((REG_0f_GAME_DIFFICULTY)<<8))-1);	/* 8192==(8*1024) */	/* 硬いけど倒せる */
			h->base_hp				= (((3*1024)+((REG_0f_GAME_DIFFICULTY)<<8))-1); /* 8192==(8*1024) */	/* 硬いけど倒せる */
			//
			set_score_common(h);
			{
				h->callback_mover					= AAA[card.tukaima_used_number].move_callback;
				h->set_hankei_256					= AAA[card.tukaima_used_number].limit_hankei_256;
				h->BOSS_DATA_05_move_jyumyou		= AAA[card.tukaima_used_number].limit_move_jyumyou;
			}
			const void *aaa[(8)] =
			{
				&&label_continue_T00,
				&&label_continue_T00,/*(共通のみ)宝物 敵を追加する */
				&&label_create_dolls_kaguya_T02,
				&&label_create_dolls_kaguya_T03,
				//
				&&label_continue_T00,/*(共通のみ)*/
				&&label_create_dolls_sakuya_T05_T06,
				&&label_create_dolls_sakuya_T05_T06,
				&&label_continue_T00,
			};
			goto *aaa[card.tukaima_used_number];
label_create_dolls_kaguya_T03:			/* 子供魔方陣02 敵を追加する */
			{
				/* 夜は 1日 の 1/3 時間 */
				// 基準位置(画面下方中心)
				h->BOSS_DATA_00_target_x256 		= (t256((GAME_WIDTH/2)));//;(src->cx256);
				h->BOSS_DATA_01_target_y256 		= (t256(272)		   );//;(src->cy256);
				//
			//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 		/* 制限時間 */
			//	h->BOSS_DATA_05_move_jyumyou		= (0x02ff)+(i<<6);	/* 制限時間 */
			//	h->BOSS_DATA_05_move_jyumyou		= (0x02ff)-(1024/3)+(i_angle1024);	/* 制限時間 */
				h->BOSS_DATA_05_move_jyumyou		+= (i_angle1024);	/* 制限時間 */
			}
			goto label_continue_T00;
label_create_dolls_kaguya_T02:/* 子供魔方陣01 敵を追加する */
			{
				h->set_hankei_256					-= (REG_0f_GAME_DIFFICULTY<<(4+8)); 	/* 指定半径 45[pixel] */
			}
			goto label_continue_T00;
label_create_dolls_sakuya_T05_T06:
			{
				#if (1)/*(追加部分)*/
				h->base_hp				= (8192-1);/*(硬い)*/
				#endif
				h->set_hankei_256	 += (card.tukaima_used_number&2/*mode*/) + (jjj);/* [青赤情報]をインターリーブ */
			}
		//	goto label_continue_T00;
label_continue_T00:// なにもせず
				;jjj^=1;
		}
	}
}

/*---------------------------------------------------------
	(使い魔システム)追加敵を設定する。
---------------------------------------------------------*/

global void init_set_dolls_kaguya_T01(OBJ *src) 	{	card.tukaima_used_number = TUKAIMA_01_kaguya_T01;}
global void init_set_dolls_kaguya_T02(OBJ *src) 	{	card.tukaima_used_number = TUKAIMA_02_kaguya_T02;}
global void init_set_dolls_kaguya_T03(OBJ *src) 	{	card.tukaima_used_number = TUKAIMA_03_kaguya_T03;}
global void init_set_dolls_mima_T04(OBJ *src)		{	card.tukaima_used_number = TUKAIMA_04_mima_T04;}
global void init_set_dolls_sakuya_T05(OBJ *src) 	{	card.tukaima_used_number = TUKAIMA_05_sakuya_T05;}
global void init_set_dolls_sakuya_T06(OBJ *src) 	{	card.tukaima_used_number = TUKAIMA_06_sakuya_T06;}
//obal void init_set_dolls_pache_T07(OBJ *src)		{	card.tukaima_used_number = TUKAIMA_07_pache_T07;}
