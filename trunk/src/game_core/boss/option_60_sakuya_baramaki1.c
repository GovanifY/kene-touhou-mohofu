
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
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
	ボス共通規格使用データー:
		BOSS_DATA_00_target_x256
		BOSS_DATA_01_target_y256
		BOSS_DATA_04_toutatu_wariai256
		BOSS_DATA_05_move_jyumyou			user_data05
---------------------------------------------------------*/

// バグ対策。 user_data02 良くワカンナイけど空けとく。()
#define shot_angle65536 	user_data06 	/* ショットを撃つ方向を保持 */


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_sakuya_baramaki1(SPRITE *src)
{
	check_tukaima_time_out(src);	/* 時間経過で終了。ボスを倒すと皆破壊される。 */
//
	#define AO_OR_AKA	(src->BOSS_DATA_00_target_x256&1)/* [青赤情報] */
//
	/*	旧作 & 咲夜風、回転ショット */
	if (0x0ff < src->BOSS_DATA_05_move_jyumyou)/* 0x100未満は共通規格上撃たない */
	{
		if (0 == ((src->BOSS_DATA_05_move_jyumyou)&0x07))	/* 次のショットを撃つまでの間隔、時間。 */
		{
			if (0 < src->BOSS_DATA_04_toutatu_wariai256 )
			{
				src->BOSS_DATA_04_toutatu_wariai256 -= (1);
			}
		//
			src->shot_angle65536 += ((AO_OR_AKA)?(-(65536/18)):((65536/18)));	/* ショットを撃つ方向を、回転させる。 */
		//
			set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
			/* ここは 6 wayではなくて、 3 way を2回追加する。でないとプライオリティーが変になる。 */
		//	HATSUDAN_06_n_way					= (2+difficulty);						/* [2-5way](r32) */
		//	HATSUDAN_06_n_way					= (3);		/* [破壊] */								/* [5way] */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/160); 					/* 密着弾(r32)6.4 */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/256); 					/* 密着弾(もっと密着)4 */
		//	HATSUDAN_07_div_angle65536			= (int)(65536/512); 					/* 密着弾(もっと密着)2 */
	//
//			HATSUDAN_01_speed256				= (t256(1.5))+((((REG_0f_GAME_DIFFICULTY))<<6));		/* 弾速(r32/r33) */
			HATSUDAN_01_speed256				= (t256(0.75))+((((REG_0f_GAME_DIFFICULTY))<<5));		/* 弾速(r35) */
			HATSUDAN_02_speed_offset			= (0);/*(テスト)*/
			HATSUDAN_03_angle65536				= (src->shot_angle65536);			/* [破壊] */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI) + (AO_OR_AKA)+(AO_OR_AKA);	/* [青緑米弾] */
			HATSUDAN_06_n_way					= (5);		/* [破壊] */								/* [5way] */
			{
				const u16 kakusan_tbl[4] =
				{(int)(65536/512), (int)(65536/512), (int)(65536/256), (int)(65536/128)};		// Lunatic はかなり拡散する。
				HATSUDAN_07_div_angle65536		= kakusan_tbl[(REG_0f_GAME_DIFFICULTY)];		/* [破壊] */		/* 密着弾(もっと密着)2 */
			}
			hatudan_system_regist_n_way();		/* (r33-) */
		}
	}
	#if 1
	/* 魔方陣アニメーション */
//	src->rotationCCW1024--;/* 右回り */
	src->rotationCCW1024 += (((AO_OR_AKA)<<2)-2);
	mask1024(src->rotationCCW1024);
	#endif
	set_timeout_alpha(src);
	/* オプション位置、移動 */
	/* 目標を設定し、誘導移動 */
//	boss_yuudou_idou_nomi(src);
	boss_yuudou_hiritu_keisan(src);
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_zako_sakuya_baramaki1(SPRITE *src)
{
	int i;
	for (i=0; i<(4); i++) /* 4つ */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= BOSS_20_YOUSEI1_SYOUMEN;
			h->flags				= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
	//
			h->callback_mover		= move_sakuya_baramaki1;
			h->callback_loser		= lose_tukaima_00;
			h->callback_hit_teki	= callback_hit_zako;
	//
			h->cx256				= ((src->cx256) & 0xfffffffe);/* [青赤情報]インターリーブ用ビットを1ビット確保 */
			h->cy256				= (src->cy256);
			h->BOSS_DATA_04_toutatu_wariai256				= t256(1.0);
		//
			{
				const s8 locate_xy_table[(4+4)] =
				{	/* 青 */	/* 赤 */	/* 青 */	/* 赤 */
				//	(-90),		(-120), 	(+120), 	(+90),	// x0, x1, x2, x3 (r32)
				//	(+90),		(0),		(0),		(+90),	// y0, y1, y2, y3 (r32)
					(-64),		(-100), 	(+100), 	(+64),	// x0, x1, x2, x3 (r33)
					(+64),		(0),		(0),		(+64),	// y0, y1, y2, y3 (r33)
				};
				h->BOSS_DATA_00_target_x256 = (h->cx256)+((locate_xy_table[i  ])<<8) + (i&1);/* [青赤情報]をインターリーブ */
				h->BOSS_DATA_01_target_y256 = (h->cy256)+((locate_xy_table[i+4])<<8);
			}
	//
			h->base_hp				= (8192-1);
			{
				static const unsigned int difficulty_score_tbl[4] =
				{
					score(	10000), 	/* easy 1万 (計 5万==5x 1万) (r33) */
					score(	30000), 	/* norm 3万 (計15万==5x 3万) (r33) */
					score(	50000), 	/* hard 5万 (計25万==5x 5万) (r33) */
					score( 100000), 	/* luna10万 (計50万==5x10万) (r33) */
				};
			h->base_score			= difficulty_score_tbl[(REG_0f_GAME_DIFFICULTY)];	/* 難易度によるが、かなり稼げる。 */
		//	h->base_score			= adjust_score_by_difficulty(score( 50000));	/* 5万 (計30万==6x5万) (r33) */
			}
			h->shot_angle65536		= (0);
		//	h->BOSS_DATA_05_move_jyumyou		= (0x03ff); 	/* 制限時間 (r32) */
			h->BOSS_DATA_05_move_jyumyou		= (0x03ff); 	/* 制限時間 (r33) */
		}
	}
}
