
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	各プレイヤー(霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子)
	REIMU(A/B) MARISA(A/B/C) REMILIA CIRNO YUYUKO
	-------------------------------------------------------
---------------------------------------------------------*/

#include "jiki_local.h"

/*static*/extern int weapon_level_offset; /* 装備した武器レベル(番号)の８倍にプレイヤー値を足した値をオフセットとして保持 */

/*---------------------------------------------------------
	プレイヤー、針弾、ショット弾の移動
	霊夢 ＆ 魔理沙 ＆ レミリア ＆ チルノ ＆ 幽々子
---------------------------------------------------------*/

static void player_move_shot(SPRITE *src)
{
	src->cx256 += src->vx256;		/*fps_factor*/
	src->cy256 += src->vy256;		/*fps_factor*/
	if (REMILIA==select_player) 	/* レミリアはゆらゆら */
	{	u16 rand_int;
		rand_int = ra_nd();
		src->cx256 -= (rand_int&0x0200);/*0x0100*/
		src->cx256 += (rand_int&0x0100);/*0x0080*/
		src->cx256 += (rand_int&0x0100);/*0x0080*/
	}
	else
	if (YUYUKO==select_player)		/* 幽々子はそれる */
	{
		src->vx256 += ((src->vx256*pd.weapon_power)>>11);
	}
	else
//	if (REIMU==select_player)			/* 霊夢の回転ショット */
	if ((REIMU_B+1) > select_player)	/* 霊夢の回転ショット */
	{
		if (((JIKI_SHOT_01)|SP_GROUP_SHOT_SPECIAL)==src->type)
		{
			/* 描画用グラ回転 */
			src->rotationCCW1024 += (32);	/* cv1024r(10) */
			mask1024(src->rotationCCW1024);
		}
	}
	if (src->cy256 < t256(-10))
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	else
	{
		player_weapon_colision_check(src, PLAYER_WEAPON_TYPE_00_SHOT);
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	REI00_x_offset = 0,
	REI01_y_offset,
	REI02_vx256,
	REI03_status,	/* 2==霊夢の回転ショット */
	REI04_MAX		/* 最大数 */
};


//global void player_register_shot_object(SPRITE *src, int /*set_*/shot_type);
/*static*/static/*global*/ void player_register_shot_object(SPRITE *src, int /*set_*/shot_type)
{
	SPRITE *h;/* shot */
	h					= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		static const s8 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
		{	/*x_offset256	y_offset256  vx256 status */
	//	/* 誘導ショット(霊夢 ＆ 魔理沙) */
	/*00*/	{ (  0),	(  0),	(	0), 	0x01},	 /*shot_regist_triple_fuda*/
	//	/* 非誘導ショット(霊夢) */
	/*01*/	{ (  0),	(  0),	(	0), 	0x00},	/* 中 まっすぐ */
	/*02*/	{ ( -5),	(  0),	(	0), 	0x00},	/* 左 まっすぐ */
	/*03*/	{ ( +5),	(  0),	(	0), 	0x00},	/* 右 まっすぐ */
	/*04*/	{ ( -5),	(  0),	( -20), 	0x00},	/* 左 左上 */
	/*05*/	{ ( +5),	(  0),	(  20), 	0x00},	/* 右 右上 */
	/*06*/	{ ( -5),	(  0),	( -35), 	0x00},	/* 左 左斜上 */
	/*07*/	{ ( +5),	(  0),	(  35), 	0x00},	/* 右 右斜上 */
	//	/* 非誘導ショット(幽々子) */
	/*08*/	{ (-15),	(-25),	( -25), 	0x00},	 /* 幽々子 */					 /*(-32)==(4*(- 8))*/
	/*09*/	{ (+15),	(-25),	( +25), 	0x00},	 /* 幽々子 */					 /*( 32)==(4*(	8))*/
	/*0a*/	{ (-25),	(-25),	( -50), 	0x00},	 /* 幽々子 */					 /*(-64)==(4*(-16))*/
	/*0b*/	{ (+25),	(-25),	( +50), 	0x00},	 /* 幽々子 */					 /*( 64)==(4*( 16))*/
	//		/* CCWの場合 */
			{ (  0),	( -7),	(	0), 	0x01},//	cv1024r((180)), 				/* 魔理沙 A/B 霊夢 レミリア ＆ チルノ */		/*t256( 0), t256( 0),*/
			{ (  0),	( -7),	(-108), 	0x01},//	cv1024r((180-25)),/*263=270-7*/ /* レミリア ＆ チルノ */ /*t256( 0), t256( 0),*/
			{ (  0),	( -7),	( 108), 	0x01},//	cv1024r((180+25)),/*277=270+7*/ /* レミリア ＆ チルノ */ /*t256( 0), t256( 0),*/
			{ (  0),	(-47),	(	5), 	0x01},//	cv1024r((181)), 				/* 幽々子専用 */	/*-48*/ /*t256( 0), t256(-40),*/
		};
	//	int shot_type;
		int shot_status;/*針?*/
		shot_status = ((r_tbl[shot_type][REI03_status]));

		int is_needle;/*針?*/
	//	is_needle = (shot_type <= NEEDLE_ANGLE_270)?(1/*針*/):(0/*弾*/);
		is_needle = ((shot_status)&0x01);
	//
		#define OFFS_IS_NEEDLE (select_player+(is_needle<<3))
		const static u8 base_shot_atari[16] =
		{	/* メインショットのあたり判定 */
		/*0,*/	(4),	/* re a */		//	BASE_SHOT_ATARI_0a,/*4*/		/* 霊夢A */ 	/*	5 */ BASE_SHOT_ATARI_0a,/*4*/
		/*0,*/	(4),	/* re b */		//	BASE_SHOT_ATARI_0b,/*4*/		/* 霊夢B */ 	/*	5 */ BASE_SHOT_ATARI_0a,/*4*/
		/*0,*/	(4),	/* ma a */		//	BASE_SHOT_ATARI_0c,/*4*/		/* 魔理沙A */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
		/*0,*/	(4),	/* ma b */		//	BASE_SHOT_ATARI_0d,/*4*/		/* 魔理沙B */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
		/*0,*/	(4),	/* oz / */		//	BASE_SHOT_ATARI_0e,/*4*/		/* 魔理沙C */	/*	7 */ BASE_SHOT_ATARI_0c,/*4*/
		/*0,*/	(8),	/* yu / */		//	BASE_SHOT_ATARI_0f,/*4*/		/* レミリア */	/*	9 */ BASE_SHOT_ATARI_0e,/*8*/
		/*0,*/	(6),	/* ci a */		//	BASE_SHOT_ATARI_0g,/*6*/		/* チルノ */	/*	8 */ BASE_SHOT_ATARI_0d,/*6*/
		/*0,*/	(9),	/* ci q */		//	BASE_SHOT_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */ BASE_SHOT_ATARI_0b,/*4*/
			/* サブショットのあたり判定 */
		/*0,*/	(6),	/* re a */		//	BASE_NEEDLE_ATARI_0a,/*6*/		/* 霊夢A */ 	/*	5 */  BASE_NEEDLE_ATARI_0a,/*6*/
		/*0,*/	(6),	/* re b */		//	BASE_NEEDLE_ATARI_0b,/*6*/		/* 霊夢B */ 	/*	5 */  BASE_NEEDLE_ATARI_0a,/*6*/
		/*0,*/	(8),	/* ma a */		//	BASE_NEEDLE_ATARI_0c,/*8*/		/* 魔理沙A */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		/*0,*/	(8),	/* ma b */		//	BASE_NEEDLE_ATARI_0d,/*8*/		/* 魔理沙B */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		/*0,*/ (12),	/* oz / */		//	BASE_NEEDLE_ATARI_0e,/*8*/		/* 魔理沙C */	/*	7 */  BASE_NEEDLE_ATARI_0c,/*12*/
		/*0,*/	(8),	/* yu / */		//	BASE_NEEDLE_ATARI_0f,/*12*/ 	/* レミリア */	/*	9 */  BASE_NEEDLE_ATARI_0e,/*8*/
		/*0,*/ (16),	/* ci a */		//	BASE_NEEDLE_ATARI_0g,/*16*/ 	/* チルノ */	/*	8 */  BASE_NEEDLE_ATARI_0d,/*16*/
		/*0,*/ (24),	/* ci q */		//	BASE_NEEDLE_ATARI_0h,/*8*/		/* 幽々子 */	/*	6 */  BASE_NEEDLE_ATARI_0b,/*8*/
		};
		h->m_Hit256R			= ((base_shot_atari[(OFFS_IS_NEEDLE)])<<8);/* あたり判定の大きさ */

	//	h->type 			= (/*SP_GROUP_JIKI_GET_ITEM*/JI KI_SHOT_02|SP_GROUP_SHOT_SPECIAL)-shot_status;
		#if 1
	//	else
	//	if(0x01== shot_status)	/* 針 */
		{
			h->type 			= (JIKI_SHOT_07|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/	/*SP_PL_FIREBALL*/
		}
	//
		if (0x00 == shot_status)	/* 針以外 */
		{
			if (REIMU_B+1 > select_player)	/* 霊夢? */
			{
			//	if(0x02== shot_status)	/* 霊夢の回転ショット */
				if (REIMU_SHOT_TYPE_00 != shot_type)/* 誘導ショット以外？ */
				{
					h->type 			= (JIKI_SHOT_01|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
					/* 描画用グラ回転 */
					h->rotationCCW1024	= (ra_nd() & (1024-1));/* 出だしの角度はランダムでないと(レーザーみたいな)変な画面になる */
				}
			/*	else	{	h->type = 誘導ショットは、針と同じ; 	}	*/
			}
			else
			{
				h->type 			= (JIKI_SHOT_02|SP_GROUP_SHOT_SPECIAL); 	/*SP_GROUP_JIKI_GET_ITEM*/
			}
		}
	//	else
		#endif
		#if 1/*Gu(中心座標)*/
		h->cx256			= (src->cx256) + ((r_tbl[shot_type][REI00_x_offset])<<8); /*+ x_offs*/
		h->cy256			= (src->cy256) + ((r_tbl[shot_type][REI01_y_offset])<<8);/*(20)*/
		#endif
		h->callback_mover	= player_move_shot;
//		h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x96);	/*	h->alpha		= 0x96; 150 半透明にする。 */	/*shot_regist_triplestar*/
	//
		#define CCC_STRENGTH_L1 	(PLAYERS8* 0)/* WEAPON_L0 */
		#define CCC_STRENGTH_L2 	(PLAYERS8* 1)/* WEAPON_L1 */
		#define CCC_STRENGTH_L3 	(PLAYERS8* 2)/* WEAPON_L2 */
		#define CCC_STRENGTH_L4 	(PLAYERS8* 3)/* WEAPON_L3 */
		#define CCC_STRENGTH_L5 	(PLAYERS8* 4)/* WEAPON_L4 */
		#define CCC_STRENGTH_L6 	(PLAYERS8* 5)/* WEAPON_L5 */
		#define CCC_SPEED			(PLAYERS8* 6)
		#define DDD_SPEED			(PLAYERS8* 7)
	//
		#define DDD_STRENGTH_L1 	(PLAYERS8* 8)/* WEAPON_L0 */
		#define DDD_STRENGTH_L2 	(PLAYERS8* 9)/* WEAPON_L1 */
		#define DDD_STRENGTH_L3 	(PLAYERS8*10)/* WEAPON_L2 */
		#define DDD_STRENGTH_L4 	(PLAYERS8*11)/* WEAPON_L3 */
		#define DDD_STRENGTH_L5 	(PLAYERS8*12)/* WEAPON_L4 */
		#define DDD_STRENGTH_L6 	(PLAYERS8*13)/* WEAPON_L5 */
	//	#define DDD_MAX 			(PLAYERS8*2)		/* 最大数 */
		#define CCC_MAX 			(PLAYERS8*14)		/* 最大数 */
		static const u8/*u8*/ ccc_tbl[CCC_MAX] =
		{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */ /* (L1ショットは、弾数が少ないので 40以下にしない) */
	//		 16, 24,  24, 16,  32,128,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L4  strength ショットの強さ */
	//		 16, 24,  24,  8,  32,128,	40, 40, 	/* WEAPON_L5  strength ショットの強さ */
	//
			 40, 40,  40, 40,  40,128,	40, 40, 	/* WEAPON_L0  strength ショットの強さ */ /* (L1ショットは、弾数が少ないので 40以下にしない) */
			 20, 24,  24, 24,  37,129,	40, 40, 	/* WEAPON_L1  strength ショットの強さ */
			 17, 24,  24, 24,  36,130,	40, 40, 	/* WEAPON_L2  strength ショットの強さ */
			 16, 24,  24, 24,  35,132,	40, 40, 	/* WEAPON_L3  strength ショットの強さ */
			 15, 25,  24, 24,  34,136,	48, 44, 	/* WEAPON_L4  strength ショットの強さ */
			 14, 26,  25, 25,  33,144,	64, 60, 	/* WEAPON_L5  strength ショットの強さ */

			 16, 14,  10, 12,	9,	4,	 8,  9, 	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
			 18, 18,  10, 10,	3,	4,	 3,  4, 	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
	//
			 16, 32,  32,160,  48, 64,	48, 48, 	/* WEAPON_L0  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 16, 32,  32,162,  48, 64,	48, 48, 	/* WEAPON_L1  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 16, 33,  32,163,  49, 64,	48, 48, 	/* WEAPON_L2  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 16, 34,  32,164,  49, 65,	48, 48, 	/* WEAPON_L3  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 15, 35,  32,165,  50, 65,	56, 52, 	/* WEAPON_L4  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
			 14, 36,  32,166,  52, 66,	80, 78, 	/* WEAPON_L5  strength 針の強さ */	/*REMILIA, CIRNO,  48 40 強すぎる*/
	/* [***20090822 REIMU 24 <- 16 (とりあえず針弱すぎるので) */
	/* r32 霊夢Aの誘導ショット。強すぎる気がするので、32 → 16 にしてみる。*/
		};
	//		 2, 3,	3, 2,  4,16, 5, 5,	/* strength ショットの強さ */
	//		16,14, 10,12,  9, 4, 8, 8,	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
		//};
	//		 2, 2, 3, 3, 4, 3, 3, 3,	/* strength ショットの強さ */
	//		15,15,12,12,18,12,12,12,	/* speed	ショットの速さ	*/
	/* 極端に機体の性能が偏るのは良くない。(強くて速いのはずるい) */
		//static const u8 ddd_tbl[DDD_MAX] =
		//{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */
	//		 3, 3, 4, 4, 6, 8, 6, 6,	/* strength 針の強さ */ 	/*REMILIA, CIRNO,  6 5 強すぎる*/
	/* 霊夢(強くしてみる [***20090930 ) */
	//		 4, 4, 4,10, 6, 8, 6, 6,	/* strength 針の強さ */ 	/*REMILIA, CIRNO,  6 5 強すぎる*/
	//		 4, 4, 4,25, 6, 8, 6, 6,	/* strength 針の強さ */ 	/*REMILIA, CIRNO,  6 5 強すぎる*/
	//		18,18,10,15, 3, 4, 3, 3,	/* speed	針の速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */


		h->base_weapon_strength = ccc_tbl[CCC_STRENGTH_L1+(weapon_level_offset)+(is_needle<<(4+2))];/* 64 == 8[players]*8[line] */
	//	h->base_weapon_strength = ddd_tbl[DDD_STRENGTH_L1+(weapon_level_offset)];

	//
	//	int ok;
	//	ok=0;
		{
			#if 0
		//	if (1==r_tbl[shot_type][REI03_yudo])	/* pspは0レジスタがあるので0と比較したほうが速い */
			if (0 != r_tbl[shot_type][REI03_yudo])/* 誘導ショットか？ */
			#else
		//	if ( (REIMU_SHOT_TYPE_01) > shot_type)/* 誘導ショットか？ */
			if ( (REIMU_SHOT_TYPE_00) == shot_type)/* 誘導ショットか？ */
			#endif
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				SPRITE *target;
				target = search_enemy_by_sprite();
				if (target != zzz_player)/* 見つかったら */
				{
					int int_angle1024;
				//	int_angle1024	= at an_1024(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
					int_angle1024	= atan_65536(target->cy256-zzz_player->cy256, target->cx256-zzz_player->cx256);
					int_angle1024 >>= (6);
				//	if (511 < int_angle1024 )/* 上方180-360度[/360度]の領域のみ追尾する */
					if ( /*(384)*/(256*3) < (( int_angle1024+128+256)&(1024-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */	/* CCWの場合 */
					{
						h->vx256	 = sin1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/ /* CCWの場合 */
						h->vy256	 = cos1024((int_angle1024))*/*p->speed*/(16/*ccc_tbl[CCC_SPEED+select_player]*/);/*fps_factor*/
						#if 1
						/* 描画用グラ回転 */
						if (MARISA_A==select_player)/* (魔理沙魔符の)マジックミサイルは傾かない。 */
						{	int_angle1024 = (0);	}
						else/* (霊夢霊符の)ホーミングアミュレットは模倣風では傾ける事にする。 */
						{
						//	h->rotationCCW1024	= int_angle1024;
							/* 描画用グラ回転(ショットは上が正面なので、回転角を計算)[180/360度]回す。 */
							int_angle1024 += cv1024r((180));
							mask1024(int_angle1024);
						}
						h->rotationCCW1024	= int_angle1024;/* 下が正面の角度が描画用角度 */
						#endif
					//	ok=1;/* 自動追尾可能 */
						return;
					}
				}
			}
		}
	//
	//	if (0==ok)/* 自動追尾不可？ */
		{	/*固有の設定角度を使う*/
			h->vx256 = ((/*int_vx*/r_tbl[shot_type][REI02_vx256]))*/*p->speed*/(ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE]);/*fps_factor*/
			h->vy256 = -(((int)ccc_tbl[CCC_SPEED+OFFS_IS_NEEDLE])<<8);/*fps_factor*/ /*p->speed*/
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A (誘導巫女) 誘導ショット */

static void shot_regist_re_yudou(SPRITE *s) /* 霊夢A 魔理沙A */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_00);/* 誘導ショット */
}

static void shot_regist_yuyuko(SPRITE *s)	/* 幽々子 */
{
	player_register_shot_object(s, NEEDLE_ANGLE_271);
}

/* 霊夢B (針巫女) / 魔理沙A/B / レミリア ＆ チル  */

static void shot_regist_re_hari(SPRITE *s)	/* 魔理沙A/B */ 	/* 兼(現在ダミー用) */
{
	player_register_shot_object(s, NEEDLE_ANGLE_270);
}

static void shot_regist_ci_gggg(SPRITE *s)	/* レミリア ＆ チルノ */
{
	player_register_shot_object(s, NEEDLE_ANGLE_277 );
	player_register_shot_object(s, NEEDLE_ANGLE_263 );
}
static void shot_regist_ma_lazer(SPRITE *s) /* 魔理沙B レーザー */
{
	{
		static u8 count128;
		count128++;
	//	count128 &= 0x7f;
		/* -110 0000 */
		#if 0
	//	if (0x00==(count128&0x60))	/* 強力だが、1/4時間しか効果がない。 */
		#else
		count128 &= 0x7f;
		if ((43)>(count128 ))	/* 強力だが、1/3時間しか効果がない。 */
		#endif
		{
			shot_regist_re_hari(s);
		}
	}
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

/* 霊夢A/B 回転ショット */	/* 通常ショット */
static void shot_regist_1(SPRITE *s)	/* 霊夢 */	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミリア */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
}
static void shot_regist_2(SPRITE *s)	/* 霊夢 */	/* 霊夢 ＆ 魔理沙 ＆ チルノ ＆ レミリア */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
}
static void shot_regist_3(SPRITE *s)	/* 霊夢 */	/* 魔理沙 ＆ チルノ */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}
static void shot_regist_4(SPRITE *s)	/* 霊夢 */	/* 魔理沙 */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_02); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_03); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
}

static void shot_regist_5(SPRITE *s)	/* 霊夢B */ /* 魔理沙 */
{
	player_register_shot_object(s, REIMU_SHOT_TYPE_06); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_04); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_01); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_05); 	/* 通常ショット */
	player_register_shot_object(s, REIMU_SHOT_TYPE_07); 	/* 通常ショット */
}

/* 霊夢B (針巫女) */

/* 魔理沙A(ミサイル魔理沙) */
/* 魔理沙B(レーザー魔理沙) */


static void shot_regist_chou(SPRITE *s) 		/* 幽々子 */
{
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_08);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_09);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0a);
	player_register_shot_object(s, YUYUKO_SHOT_TYPE_0b);
}
/*---------------------------------------------------------
	オプション アニメーション
---------------------------------------------------------*/

static void player_animate_option(SPRITE *src)
{
#if 1
	if (0==player_fix_status[BASE_OPT_ANIM_TYPE+select_player])
	{
		/* 描画用グラ回転 */
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		src->rotationCCW1024 += (2+2);/*...とりあえず*/
		mask1024(src->rotationCCW1024);
	}
	else
//	if (/*YUYUKO==select_player*/)
//	if ((CIRNO_A==select_player)||(CIRNO_Q==select_player))
	{
		src->PL_OPTION_DATA_anime_wait--;
		if (src->PL_OPTION_DATA_anime_wait<1)
		{
//			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(12);/*(12)*/ /*2*/ /*陰陽だまはゆっくり回るよ*/	//yuyuko
//			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(3);/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/		//cirno
			src->PL_OPTION_DATA_anime_wait=/*(anime_speed)*/(player_fix_status[BASE_OPT_ANIM_SPEED+select_player]);/*12*/ /*2*/ /*陰陽だまはゆっくり回るよ*/
		#if 0
		//	src->type=((src->type+(src->PL_OPTION_DATA_opt_anime_add_id)+(src->PL_OPTION_DATA_opt_anime_add_id)-1/*src->PL_OPTION_DATA_pos*/)&(8-1)/*%8*/);
		//	src->type=((src->type+(src->PL_OPTION_DATA_opt_anime_add_id))&(0xfff7)/*%8*/);
			src->type=((src->type+(1))&(0xfff7)/*%8*/);
		//	if (src->type<0)		{	src->type=7;}
		#endif
		#if 0
		//	src->an im_frame=((src->an im_frame+(src->PL_OPTION_DATA_opt_anime_add_id)+(src->PL_OPTION_DATA_opt_anime_add_id)-1/*src->PL_OPTION_DATA_pos*/)&(8-1)/*%8*/);
			src->an im_frame=((src->an im_frame+(src->PL_OPTION_DATA_opt_anime_add_id))&(8-1)/*%8*/);
		//	if (src->an im_frame<0) 	{	src->an im_frame=7;}
		#endif
			#if 0
			src->an im_frame++;
			src->an im_frame &= (8-1);//if (src->an im_frame==8)	{src->an im_frame=0;}
			#endif

		#if 1
		//	src->type = ((src->type) & (0xfff8))|((src->type+1) & (8-1));//if (src->an im_frame==8) 	{src->an im_frame=0;}
			src->type++;
			src->type &= (0xfff7);//if (src->type==8)	{src->type=0;}
		#endif
		}
	}
#endif
}


/*---------------------------------------------------------

---------------------------------------------------------*/
/*
 WEAPON_L0(P000-P008)
 WEAPON_L1(P008-P015)
 WEAPON_L2(P016-P031)
 WEAPON_L3(P032-P063)
 WEAPON_L4(P064-P127)
 WEAPON_L5(P128)
*/
/*static*/static  void shot_regist_remilia_cirno(SPRITE *src)
{	/* 攻撃支援 */
	switch (src->PL_OPTION_DATA_opt_anime_add_id)
	{
	case OPTION_C1:
	case OPTION_C2:
	//	if (pd.weapon_power > (100-1))	/* 旧システムの調整値 100 */
		if (pd.weapon_power > (96)) 	/* システムが変わったので修正してみる(96==128-32) */
		{
			shot_regist_ci_gggg(src);	//	src->PL_OPTION_DATA_opt_shot_interval=(7+3);
		}
		else
		{
			shot_regist_re_hari(src);	//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
		}
		break;
	case OPTION_C3:
	case OPTION_C4:
		shot_regist_re_hari(src);		//	src->PL_OPTION_DATA_opt_shot_interval=(5+3);
		break;
	}
}


/*---------------------------------------------------------
	プレイヤー、オプションの移動制御
---------------------------------------------------------*/
/*static*/extern void player_control_option(SPRITE *src);	/* 全員 */
/*static*/global void player_move_option(SPRITE *src)	/* 全員 */
{
	/* オプションが非表示の場合、何もしない。 */
	if (0==(src->flags & ( SP_FLAG_OPTION_VISIBLE)))	{	return; 	}
//
	/* アニメーション */
	player_animate_option(src);
//
	if (pd.state_flag & STATE_FLAG_15_KEY_SHOT)
	{
		{
			{
				src->PL_OPTION_DATA_opt_shot_interval--;
				if (src->PL_OPTION_DATA_opt_shot_interval<0)
				{
					src->PL_OPTION_DATA_opt_shot_interval = player_fix_status[BASE_OPT_SHOT_INTERVAL0+(weapon_level_offset)];/* (霊夢 魔理沙	仮幽々子)オプションのショット間隔 */
					#if 1//1975613
					static /*const*/ void (*ggg[PLAYERS8])(SPRITE *sss) =
					{
						/*REIMU_A*/ 	shot_regist_re_yudou,			/* 霊夢 A(誘導赤札) */
						/*REIMU_B*/ 	shot_regist_re_hari,			/* 霊夢 B(針) */	/*re_bbbb*/
						/*MARISA_A*/	shot_regist_re_yudou,			/* 魔理沙 A(誘導森弾) */
						/*MARISA_B*/	shot_regist_ma_lazer,			/* 魔理沙 B(きまぐれレーザー) */
						/*REMILIA*/ 	shot_regist_remilia_cirno,		/* レミリア ＆ チルノ */
						/*YUYUKO*/		shot_regist_yuyuko, 			/* 幽々子(蝶) */
						/*CIRNO_A*/ 	shot_regist_remilia_cirno,		/* レミリア ＆ チルノ */
						/*CIRNO_Q*/ 	shot_regist_remilia_cirno,		/* レミリア ＆ チルノ */
					};
					(ggg[select_player])(src);
					#endif
				}
			}
		}
	}
	player_control_option(src);
}


/*---------------------------------------------------------
	自機のショットボタン押しっぱなしで自動ショットを撃つ場合
---------------------------------------------------------*/

global void register_main_shot(SPRITE *s1)
{
	if (
		(MARISA_B==select_player) &&	/* 魔理沙B(恋符) */
		(0<pd.bomber_time)				/* マスタースパーク中 */
	)
	{
		;/* マスタースパーク中はショットが撃てないらしい */
	}
	else
	{
		{static int weapon_interval=0;
			/*pd.*/weapon_interval--/*=fps_fa_ctor*/;
			if (/*pd.*/weapon_interval < 1 )
			{	/* 自機のショットボタン押しっぱなしで自動ショットを撃つ場合の、撃つ間隔[フレーム単位]のテーブル。 */
				static const u8 jiki_auto_shot_interval_table[(WEAPON_L_MAX)*(PLAYERS8)] =
				{/* REIMU(A/B) MARISA(A/B) REMILIA YUYUKO CIRNO(A/Q) */    /* レミリア強すぎるので調整(+5) */
						 5, 5,	6, 6,  7+5,  9+8,  9,  9,	/* WEAPON_L0(P000-P008) */
						 5, 5,	6, 6,  6+5,  8+8,  9,  9,	/* WEAPON_L1(P008-P015) */
						 5, 5,	5, 5,  7+5,  7+8,  9,  9,	/* WEAPON_L2(P016-P031) */
						 5, 5,	5, 5,  6+5,  6+8,  9,  9,	/* WEAPON_L3(P032-P063) */
						 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L4(P064-P127) */
						 5, 5,	5, 5,  5+5,  5+8,  9,  9,	/* WEAPON_L5(P128)		*/
				};
			//	/*pd.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_List<<3)+select_player];
				/*pd.*/weapon_interval = jiki_auto_shot_interval_table[(weapon_level_offset)];
				voice_play(VOICE00_SHOT, TRACK00_BULLETS);
				static /*const*/ void (*bbb[(WEAPON_L_MAX)*(PLAYERS8)])(SPRITE *sss) =
				{
	/*REIMU_A*/ 	/*REIMU_B*/ 	/*MARISA_A*/	/*MARISA_B*/	/*REMILIA*/ 	/*YUYUKO 無段階成長 */	/*CIRNO_A*/ 	/*CIRNO_Q*/
	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L0:(P000-P008) */
	shot_regist_1,	shot_regist_1,	shot_regist_2,	shot_regist_2,	shot_regist_1,	shot_regist_chou,		shot_regist_2,	shot_regist_2,	/* WEAPON_L1:(P008-P015) */
	shot_regist_2,	shot_regist_2,	shot_regist_2,	shot_regist_3,	shot_regist_2,	shot_regist_chou,		shot_regist_2,	shot_regist_3,	/* WEAPON_L2:(P016-P031) */
	shot_regist_3,	shot_regist_3,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L3:(P032-P063) */
	shot_regist_3,	shot_regist_4,	shot_regist_3,	shot_regist_4,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L4:(P064-P127) */
	shot_regist_4,	shot_regist_5,	shot_regist_3,	shot_regist_5,	shot_regist_2,	shot_regist_chou,		shot_regist_3,	shot_regist_3,	/* WEAPON_L5:(P128) 	 */
				};
				(bbb[(weapon_level_offset)])(s1);
			}
		}
	}
}
