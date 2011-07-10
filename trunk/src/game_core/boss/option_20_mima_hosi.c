
#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	魅魔 オプション
	-------------------------------------------------------
	ボス共通規格使用データー:
		BOSS_DATA_00_target_x256	目標x座標 x位置として使用。
		BOSS_DATA_01_target_y256	目標y座標 y位置として使用。
		BOSS_DATA_05_move_jyumyou	user_data05
---------------------------------------------------------*/

// バグ対策。 user_data02 良くワカンナイけど空けとく。()
#define rotate_angle1024		user_data03 	/* ボスを中心として、回転角度。(下CCW1024形式) */
#define shot_angle65536 		user_data06 	/* */
#define now_hankei_dot			user_data07 	/* 半径 */


/*---------------------------------------------------------
	子供魔方陣 移動のみ
---------------------------------------------------------*/
global void move_tukaima00(SPRITE *src)
{
	#if (0)// 1802402
	src->cx256 = (src->BOSS_DATA_00_target_x256) + ((si n1024((src->rotate_angle1024))*(src->now_hankei_dot))); /*CCW*/
	src->cy256 = (src->BOSS_DATA_01_target_y256) + ((co s1024((src->rotate_angle1024))*(src->now_hankei_dot)));
	#else// 1802482 1802450
	{
		int sin_value_t256; //	sin_value_t256 = 0;
		int cos_value_t256; //	cos_value_t256 = 0;
		int256_sincos1024( (src->rotate_angle1024), &sin_value_t256, &cos_value_t256);
		src->cx256 = (src->BOSS_DATA_00_target_x256) + ((sin_value_t256*(src->now_hankei_dot)));	/*CCW*/
		src->cy256 = (src->BOSS_DATA_01_target_y256) + ((cos_value_t256*(src->now_hankei_dot)));
	}
	#endif
}
/*---------------------------------------------------------
	子供魔方陣 移動
---------------------------------------------------------*/

static void move_mima_doll(SPRITE *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
//	if ( (unsigned int)(0x01ff/3) < src->BOSS_DATA_05_move_jyumyou)/* 最後の 1/3 時間は撃って来ない。 */
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			src->shot_angle65536 += (65536/18); /* ショットを撃つ方向を、回転させる。 */
		//
			set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
		//
		//	/*[r35要半分速]*/HATSUDAN_01_speed256				= t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6);		/* 弾速 */
			/*[r35半分速]*/HATSUDAN_01_speed256 				= t256(0.75)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* 弾速 */
			HATSUDAN_02_speed_offset			= (0);/*(テスト)*/
			HATSUDAN_03_angle65536				= ((src->shot_angle65536) );		/* [破壊] */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);			/* [赤クナイ弾] */
			HATSUDAN_06_n_way					= (2+(REG_0f_GAME_DIFFICULTY)); 	/* [破壊] */	/* [2-5way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/160); 				/* [破壊] */	/* 密着弾 */
		//	hatudan_system_regist_katayori_n_way();/* (r33-) */
			hatudan_system_regist_n_way();		/* (r33-) */
		}
	}
	//
	set_timeout_alpha(src);
	//
	/* オプション位置、回転移動 */
	src->rotate_angle1024 += (2);
	mask1024(src->rotate_angle1024);
	/* オプション位置、移動 */
	//define now_hankei_dot (45)			/* 半径 */
	if ((45) > (src->now_hankei_dot))	{	src->now_hankei_dot++;	};
	move_tukaima00(src);

	//#undef now_hankei_dot
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_mima_dolls(SPRITE *src)
{
//	#define ADD_ANGLE (171*2)	/* 1周を3分割した角度、170.66*2 == 1024/3 */
//	#define ADD_ANGLE (85*2)	/* 1周を6分割した角度、 85.33*2 == 1024/6 */
	#define ADD_ANGLE (102*2)	/* 1周を5分割した角度、102.40*2 == 1024/5 */
//	const int add_angle = (ADD_ANGLE);	/* 加算角度 */
//
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += (ADD_ANGLE)) /* 一周 */
	#undef ADD_ANGLE
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_20_YOUSEI1_SYOUMEN;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_mima_doll;
			h->callback_loser		= lose_tukaima_00;//lose_mima_doll;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->BOSS_DATA_00_target_x256 		= (src->cx256);
			h->BOSS_DATA_01_target_y256 		= (src->cy256);
	//
		//	h->base_hp				= ((8*1024)-1); 	/* 8192==(8*1024) */
		//	h->base_hp				= ((3*1024)-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
		//	h->base_hp				= (((difficulty+1)<<10)-1); 	/* 8192==(8*1024) */	/* 硬いけど倒せる */
			/* 難度上がると微妙に硬くなる(==倒すのにそれだけ時間がかかる) */
			h->base_hp				= (((1024)+((REG_0f_GAME_DIFFICULTY)<<8))-1);	/* 8192==(8*1024) */	/* 硬いけど倒せる */
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score( 100000), 	/* easy 10万 (計 50万==5x 10万) */
					score( 300000), 	/* norm 30万 (計150万==5x 30万) */
					score( 500000), 	/* hard 50万 (計250万==5x 50万) */
					score(1000000), 	/* luna100万 (計500万==5x100万) */
				};
			h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* 難易度によるが、かなり稼げる。 */
		//	h->base_score			= adjust_score_by_difficulty(score( 500000));	/* 50万 (計300万==6x50万) */
			}
		//
			h->now_hankei_dot		= (0);			/* 初期半径 0[dot] */
			h->rotate_angle1024 	= i_angle1024;
			h->shot_angle65536		= (0);
		//	h->BOSS_DATA_05_move_jyumyou		= (0x01ff); 	/* 制限時間 / 予想以上に速く消える位に。(r32) */
			h->BOSS_DATA_05_move_jyumyou		= (0x02ff); 	/* 制限時間 / 予想以上に速く消える位に。(r33) */
		}
	}
}
