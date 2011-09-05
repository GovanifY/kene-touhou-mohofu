
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	道中のザコ
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

static void attack_kougeki_mahoujin00(OBJ *src)
{
	/* 攻撃しない */
}
/*---------------------------------------------------------
	1:縦攻撃のカード
---------------------------------------------------------*/

static void attack_kougeki_mahoujin01(OBJ *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->jyumyou)
	{
		zako_shot_supeka(src, ZAKO_SPEKA_11_mahou_tate);
	}
}

/*---------------------------------------------------------
	2:ランダムばらまき攻撃("魔方陣2")のカード
---------------------------------------------------------*/

static void attack_kougeki_mahoujin02(OBJ *src)
{
	if (0 < src->MAHOU_TEKI_wait2)
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (8);/*(5) (プログラムが速くなってる分遅くする)*/
			src->MAHOU_TEKI_wait2--;
			voice_play(VOICE12_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			zako_shot_supeka(src, ZAKO_SPEKA_12_mahou_random);
		}
	}
}

/*---------------------------------------------------------
	とりあえず共通部分
---------------------------------------------------------*/

static void attack_kougeki_mahoujin_common(OBJ *src)
{
			zako_shot_supeka(src, ZAKO_SPEKA_0f_mahou_common);

}
/*---------------------------------------------------------
	3:ランダムばらまき攻撃+自機狙い("魔方陣1")のカード
---------------------------------------------------------*/

static void attack_kougeki_mahoujin03(OBJ *src)
{
		OBJ *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_00_PLAYER];
	if (
			(src->cy256 > zzz_player->cy256)
	 || 	(src->cy256 > t256(100))
	)
	{
	//	src->time_out++;/* = ST01;*/
		src->jyumyou								= MAHOU_TIME_LIMIT_01_ADJ;	/* 消える */
			zako_shot_supeka(src, ZAKO_SPEKA_08_mahou_nerai);
	}
	{
		src->MAHOU_TEKI_wait1--;			/* ばらまき間隔カウンタ */
		if (src->MAHOU_TEKI_wait1 <= 0) 	/* ばらまき間隔カウンタ */
		{
			src->MAHOU_TEKI_wait1 = (25-((cg.game_difficulty)*5)); /* ばらまき間隔カウンタ */
			voice_play(VOICE12_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			/* 速度は 1.0-4.0までの乱数(ただし上方域に偏差)に変更 */
//...			HATSUDAN_01_speed256						= ((ra_nd()&(1024-1))|(0x100))+t256(1.0);	/* 弾速 */
			HATSUDAN_03_angle65536						= ((ra_nd()&(65536-1)));					/* 角度 */
			attack_kougeki_mahoujin_common(src);
		}
	}
}

/*---------------------------------------------------------
	4:円状攻撃のカード(小左)
	5:円状攻撃のカード(中右)
	6:円状攻撃のカード(大左)
---------------------------------------------------------*/

static void attack_kougeki_mahoujin4564(OBJ *src)
//atic void attack_kougeki_mahoujin05(OBJ *src)
//atic void attack_kougeki_mahoujin06(OBJ *src)
{
	/* 攻撃開始フレームから16フレーム経過後に 1回だけ攻撃 */
	//if ((MAHOU_TIME_LIMIT_02_ADJ-16) == src->time_out)
	//if (0 == (src->jyumyou&0x07))
	if ((cg.game_difficulty) >= (src->jyumyou&0x03))
	{
			zako_shot_supeka(src, (int)(((src->cx256)&0x07)|(0x10)) );
	}
}




/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
		#if 0
		{			attack_kougeki_mahoujin02(src); }	/* ("魔方陣2") 消える魔方陣 */
		{			attack_kougeki_mahoujin03(src); }	/* ("魔方陣1") ばらまき攻撃魔方陣 */
		#endif

static void move_kougeki_mahoujin(OBJ *src)
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
		src->atari_hantei			= (1/*スコア兼用*/);	/* あたり判定あり */
		/*(ここでスコアを設定する)*/
	}
	/* 攻撃 */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ < src->jyumyou)
	{
	//	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
		/* カード選択 */
		void (*aaa[8])(OBJ *src) =
		{
			attack_kougeki_mahoujin00,		/* 攻撃しない */
			attack_kougeki_mahoujin01,		/* 縦弾 */
			attack_kougeki_mahoujin02,		/* ばらまき #01(差分氏、旧"魔方陣2") */
			attack_kougeki_mahoujin03,		/* ばらまき #02(差分氏、旧"魔方陣1") */
			attack_kougeki_mahoujin4564,		/* 8way花弾(中) */
			attack_kougeki_mahoujin4564,		/* 8way花弾(中) */
			attack_kougeki_mahoujin4564,		/* 8way花弾(中) */
			attack_kougeki_mahoujin4564,		/* 8way花弾 */
		};
		(*aaa[ (int)((src->cx256)&0x07) ])(src);	/* 各関数に分岐する */
	}
	/* あたり判定、なし */
	else
	if (MAHOU_TIME_LIMIT_01_ADJ == src->jyumyou)
	{
		src->atari_hantei			= (ATARI_HANTEI_OFF/*スコア兼用*/); 	/* あたり判定無し */
		/*(あたり判定無し==スコアは無い)*/
	}
	/* 消える */
	else
	if (MAHOU_TIME_LIMIT_03_EPS < src->jyumyou) 	/*	if (0x04 0x00 > (unsigned int)(src->alpha))*/
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
	{
		int sin_value_t256; 		//	sin_value_t256 = 0;
		int cos_value_t256; 		//	cos_value_t256 = 0;
		int256_sincos1024( (((src->jyumyou<<5)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
	//	src->cy256 = src->start_y256 + ((cos1024(((src->jyumyou<<5)&(1024-1))))<<2);
		src->cy256 = src->start_y256 + ((sin_value_t256)<<2);
	}
//	src->rotationCCW1024++;
//	mask1024(src->rotationCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
static void add_common_mahoujin(GAME_COMMAND *l, OBJ *h)
{
	h->atari_hantei		= (ATARI_HANTEI_OFF/*スコア兼用*/); 	/* あたり判定無し */
	/*(あたり判定==獲得スコア)*/
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
static void regist_zako_000_mahoujin1(GAME_COMMAND *l, OBJ *h)// MAHOUJIN A
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
static void regist_zako_001_mahoujin2(GAME_COMMAND *l, OBJ *h)// MAHOUJIN B
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
