
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"魔方陣2"		消える魔方陣
	"魔方陣1"		ばらまき攻撃魔方陣
	-------------------------------------------------------
	地形に関連する敵
---------------------------------------------------------*/

/* 時間設定 */

//#define MAHOU_TIME_LIMIT_00_MAX	(0x1ff)
//#define MAHOU_TIME_LIMIT_00_MAX	(0x17f)
#define MAHOU_TIME_LIMIT_00_MAX 	(0x13f)
#define MAHOU_TIME_LIMIT_01_ADJ 	((int)(0x0ff/3))
#define MAHOU_TIME_LIMIT_02_ADJ 	(MAHOU_TIME_LIMIT_00_MAX - MAHOU_TIME_LIMIT_01_ADJ)
#define MAHOU_TIME_LIMIT_03_EPS 	(0x04)

/*---------------------------------------------------------
	敵攻撃しない
---------------------------------------------------------*/

static void attack_kougeki_mahoujin00(SPRITE *src)
{
	/* 攻撃しない */
}
/*---------------------------------------------------------
	1:縦攻撃のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin01(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->jyumyou)
	{
		br.BULLET_REGIST_00_speed256					= (t256(2.5));			/* 弾速 */
		br.BULLET_REGIST_02_VECTOR_angle1024			= (int)(1024/(4));		/* 右向き(上下弾)、偶数弾なので結果的に上下弾。 */
	//	br.BULLET_REGIST_03_VECTOR_regist_type			= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_04_bullet_obj_type 			= BULLET_KOME_01_AKA;	/* 赤米弾 */
		br.BULLET_REGIST_06_n_way						= (2);					/* [2way] */
		br.BULLET_REGIST_07_VECTOR_div_angle1024		= (int)(1024/(2));		/* 角度(一周を 2 分割) */	/* 1周をn分割した角度 */
		bullet_regist_multi_vector_direct();
	}
}

/*---------------------------------------------------------
	2:ランダムばらまき攻撃("魔方陣2")のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin02(SPRITE *src)
{
	if (0 < src->MAHOU_TEKI_wait2)
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (8);/*(5) (プログラムが速くなってる分遅くする)*/
			src->MAHOU_TEKI_wait2--;
			voice_play(VOICE12_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			/* 速度は 1.0-3.0までの無段階乱数に変更 */
			br.BULLET_REGIST_00_speed256							= ((ra_nd()&(512-1))+t256(1.0)+t256(1.0));	/* 弾速 */
			br.BULLET_REGIST_02_VECTOR_angle1024					= ((ra_nd()&(1024-1))); 					/* 角度 */
			br.BULLET_REGIST_03_VECTOR_regist_type					= LEGACY_REGIST_TYPE_01_TOMARI;
			br.BULLET_REGIST_04_bullet_obj_type 					= (BULLET_MINI8_01_AKA+(0));				/* 弾グラ */
			br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256			= (2);//10.24==t256(0.04);					/* 減速定数 */
			bullet_regist_legacy_vector_direct();
		}
	}
}

/*---------------------------------------------------------
	とりあえず共通部分
---------------------------------------------------------*/

static void attack_kougeki_mahoujin_common(SPRITE *src)
{
	//		br.BULLET_REGIST_02_VECTOR_angle1024					= (int)(1024/(7));							/* 角度(一周を 7 分割) */	/* 1周をn分割した角度 */
			br.BULLET_REGIST_03_VECTOR_regist_type					= LEGACY_REGIST_TYPE_01_TOMARI;
			br.BULLET_REGIST_04_bullet_obj_type 					= (BULLET_MINI8_01_AKA+(0));				/* 弾グラ */
			br.BULLET_REGIST_06_VECTOR_tomari_dan_next_angle1024	= ANGLE_NO_SHOT_DAN;
			br.BULLET_REGIST_07_VECTOR_legacy_dan_delta256			= (1);//t256(0.03); 						/* 減速定数 */
			bullet_regist_legacy_vector_direct();

}
/*---------------------------------------------------------
	3:ランダムばらまき攻撃+自機狙い("魔方陣1")のスペカ
---------------------------------------------------------*/

static void attack_kougeki_mahoujin03(SPRITE *src)
{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
	if (
			(src->cy256 > zzz_player->cy256)
	 || 	(src->cy256 > t256(100))
	)
	{
	//	src->time_out++;/* = ST01;*/
		src->jyumyou									= MAHOU_TIME_LIMIT_01_ADJ;	/* 消える */
		br.BULLET_REGIST_00_speed256					= t256(1.0);				/* 弾速 */
		br.BULLET_REGIST_02_VECTOR_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
	//	br.BULLET_REGIST_03_VECTOR_regist_type			= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_04_bullet_obj_type 			= BULLET_MINI8_03_AOI;		/* [赤弾] */		/* 弾グラ */
		br.BULLET_REGIST_06_n_way						= (1);						/* [1way] */
	//	br.BULLET_REGIST_07_VECTOR_div_angle1024		= (0);						/* ダミー角度(未使用) */
		bullet_regist_multi_vector_direct();
	}
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (25-((cg_game_difficulty)*5)); /* ばらまき間隔カウンタ */
			voice_play(VOICE12_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			/* 速度は 1.0-4.0までの乱数(ただし上方域に偏差)に変更 */
			br.BULLET_REGIST_00_speed256							= ((ra_nd()&(1024-1))|(0x100))+t256(1.0);	/* 弾速 */
			br.BULLET_REGIST_02_VECTOR_angle1024					= ((ra_nd()&(1024-1))); 					/* 角度 */
			attack_kougeki_mahoujin_common(src);
		}
	}
}

/*---------------------------------------------------------
	4:円状攻撃のスペカ(小左)
---------------------------------------------------------*/

static void attack_kougeki_mahoujin04(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->jyumyou&0x07))
	{
			br.BULLET_REGIST_00_speed256							= (t256(1.5)+(cos1024(src->jyumyou<<4)));					/* 弾速 */
			br.BULLET_REGIST_02_VECTOR_angle1024					= ( (src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7));	/* 向き */
			attack_kougeki_mahoujin_common(src);
	}
}

/*---------------------------------------------------------
	5:円状攻撃のスペカ(中右)
---------------------------------------------------------*/

static void attack_kougeki_mahoujin05(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->jyumyou&0x07))
	{
			br.BULLET_REGIST_00_speed256							= (t256(1.0)+(cos1024(src->jyumyou<<3)<<2));					/* 弾速 */
			br.BULLET_REGIST_02_VECTOR_angle1024					= (-(src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7));	/* 向き */
			attack_kougeki_mahoujin_common(src);
	}
}

/*---------------------------------------------------------
	6:円状攻撃のスペカ(大左)
---------------------------------------------------------*/

static void attack_kougeki_mahoujin06(SPRITE *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->jyumyou&0x07))
	{
			br.BULLET_REGIST_00_speed256							= (t256(1.0)+(cos1024(src->jyumyou<<2)<<2));					/* 弾速 */
			br.BULLET_REGIST_02_VECTOR_angle1024					= ( (src->jyumyou+src->jyumyou)+((src->jyumyou&0x07)<<7));	/* 向き */
			attack_kougeki_mahoujin_common(src);
	}
}


/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
		#if 0
		{			attack_kougeki_mahoujin02(src); }	/* ("魔方陣2") 消える魔方陣 */
		{			attack_kougeki_mahoujin03(src); }	/* ("魔方陣1") ばらまき攻撃魔方陣 */
		#endif

static void move_kougeki_mahoujin(SPRITE *src)
{
	/* 出現 */
	if (MAHOU_TIME_LIMIT_02_ADJ < src->jyumyou)
	{
		src->color32 += 0x03000000; 		/*	src->alpha += 0x03;*/ /*4*/ /*fps_factor*/
	}
	/* あたり判定、発生 */
	else
	if (MAHOU_TIME_LIMIT_02_ADJ == src->jyumyou)
	{
		src->flags		|= ( (SP_FLAG_COLISION_CHECK)); 	/* あたり判定あり */
	}
	/* 攻撃 */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ < src->jyumyou)
	{
		send1_xy(src);	/* 弾源x256 y256 中心から発弾。 */
		/* スペカ選択 */
		void (*aaa[8])(SPRITE *src) =
		{
			attack_kougeki_mahoujin00,		/* 攻撃しない */
			attack_kougeki_mahoujin01,		/* 縦弾 */
			attack_kougeki_mahoujin02,		/* ばらまき #01(差分氏、旧"魔方陣2") */
			attack_kougeki_mahoujin03,		/* ばらまき #02(差分氏、旧"魔方陣1") */
			attack_kougeki_mahoujin04,		/* 8way花弾(中) */
			attack_kougeki_mahoujin05,		/* 8way花弾(中) */
			attack_kougeki_mahoujin06,		/* 8way花弾(中) */
			attack_kougeki_mahoujin04,		/* 8way花弾 */
		};
		(*aaa[ (int)((src->cx256)&0x07) ])(src);	/* 各関数に分岐する */
	}
	/* あたり判定、なし */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ == src->jyumyou)
	{
		src->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
	}
	/* 消える */
	else
	if ( MAHOU_TIME_LIMIT_03_EPS < src->jyumyou )	/*	if ( 0x04 0x00 > (unsigned int)(src->alpha))*/
	{
		src->color32 -= 0x03000000; 	/*	src->alpha += 0x03;*/ /*4*/ /*fps_factor*/
	}
	/* おしまい */
	else
	{
	//	src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;*/
		src->jyumyou = JYUMYOU_NASI;
	}
	/* アニメ (揺れる) */
	src->cy256 = src->start_y256 + ((cos1024(((src->jyumyou<<5)&(1024-1))))<<2);

//	src->rotationCCW1024++;
//	mask1024(src->rotationCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
static void add_common_mahoujin(GAME_COMMAND *l, SPRITE *h)
{
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定無し */
	/* KETMの相対座標指定は廃止。画面座標指定にする */
	h->cx256						= (((short)l->user_x)<<8)|(((char)l->user_kougeki_type)&0x07);/* こっそりインターリーブ */
	h->start_y256					= (((short)l->user_y)<<8);
	//
	h->jyumyou						= MAHOU_TIME_LIMIT_00_MAX;/* SS00 または ST00 */

	h->MAHOU_TEKI_wait1 			= (10); 	/*+90*/ /* */
	h->MAHOU_TEKI_wait2 			= (5);		/* 最大五回 */
	h->color32						= 0x00ffffff;	/*	h->alpha				= 0x02 0x00;*/
}

/* ばらまき攻撃魔方陣 */	/* "魔方陣1" "GROUNDER",*/
static void regist_zako_000_mahoujin1(GAME_COMMAND *l, SPRITE *h)// MAHOUJIN A
{
	{
		h->m_Hit256R				= ZAKO_ATARI04_PNG;/* あたり判定サイズを変える */
		h->callback_loser			= item_create_000_kougeki_mahoujin;
//		h->MAHOU_TEKI_wait1 		= 100;
	//	h->MAHOU_TEKI_wait2 		= 0;/* 未使用 */
	}
	add_common_mahoujin(l, h);
}

/* 消える魔方陣 */	/* "魔方陣2" "MAGICF",*/
static void regist_zako_001_mahoujin2(GAME_COMMAND *l, SPRITE *h)// MAHOUJIN B
{
	{
		h->m_Hit256R				= ZAKO_ATARI16_PNG;
//		h->MAHOU_TEKI_wait1 		= 10/*+90*/;
	//	h->MAHOU_TEKI_wait2 		= (5);		/* 最大五回 */
	}
	add_common_mahoujin(l, h);
}
//		/*data->base.*/h->base_hp	= (9999);/* 倒せない */
//		/*data->base.*/h->base_hp		+= (8*25)+(1/*di fficulty*/<<(3/*4*/+3));	//+(di fficulty*15)
//		/*data->base.*/h->base_score	= score(200*2);
