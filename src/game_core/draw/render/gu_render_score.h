
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スコア関連(取った得点を小さな漢字で説明表示)
---------------------------------------------------------*/

/* スコアキャッシュ最大4096文字 */
#define MAX_SCORE_CHACHE (0x1000)

/* 非表示の識別 */
#define SCORE_DELETE (0)

/* スコアキャッシュのデーター形式 */
typedef struct
{
	u32 time_out_color8888; /* 表示時間 / 表示アルファ値 */
	int number; 	/* 表示文字番号(0 ... 9) [一桁の数字] */
	int x256;		/* 表示 x 座標(256固定小数点形式) */
	int y256;		/* 表示 y 座標(256固定小数点形式) */
} SCORE_CHACHE_DATA;

static SCORE_CHACHE_DATA score_number_chache[MAX_SCORE_CHACHE];


/*---------------------------------------------------------
	スコアキャッシュすべて消す
---------------------------------------------------------*/

void clear_score_chache(void)
{
	unsigned int i;
	for (i=0; i<MAX_SCORE_CHACHE; i++)
	{
		score_number_chache[i].time_out_color8888 = SCORE_DELETE;
	}
}

/*---------------------------------------------------------
	一桁の数字を、キャッシュに登録する
---------------------------------------------------------*/

static void regist_score(int number, u32 color8888, int x256, int y256)
{
	if (t256((GAME_X_OFFSET-8)) > x256) 	{ return; } 	/* (8)? (左端より左なら描かない) */
	if (t256((GAME_WIDTH-8)) < x256)		{ return; } 	/* (8)? (右端より右なら描かない) */
	if (t256((GAME_HEIGHT-8)) < y256)		{ return; } 	/* (6)? (あまり下なら描かない) */
//
	static unsigned int index = 0;	/* 登録出来そうな位置 */
//	int iii;
//	iii = 0;	/* 最大登録数まで全部探す */
//	do
//	{
		index++;
		index &= (MAX_SCORE_CHACHE-1);
		/* 使用中 */
//		if (SCORE_DELETE < score_number_chache[index].time_out_color8888)
//		{
//			;	/* 登録できないので次を探す。 */
//		}
//		/* 未使用 */
//		else //if (1 > score_number_chache[index].time_out_color8888)
		{
			/* キャッシュに登録する */
			score_number_chache[index].time_out_color8888 = color8888;//(127*2);/*	60*2*2 =:= 2 [sec]*/
			score_number_chache[index].number	= number;
			score_number_chache[index].x256 	= x256;
			score_number_chache[index].y256 	= y256;
//			iii = MAX_SCORE_CHACHE; 	/* 見つかったよ */
		}
//		iii++;
//	} while (MAX_SCORE_CHACHE > iii);	/* 全部探した？ */
}


/*---------------------------------------------------------
	スコアを一桁の数字に分解し、キャッシュに登録する
---------------------------------------------------------*/

static void bonus_info_shered_add_score10_value(OBJ/**/ *src, s32 score_value)
{
	int y256;	y256 = src->center.y256;
	if (t256((0)) > y256)	{ return; }
//
	int x256;	x256 = src->center.x256;
	int jjj;
	u32 color8888;
	int i;
	jjj=0;
	color8888 = 0xffffffff;
	/* 加算スコアが 999990点 以上の場合は 999990点 の表示にする(表示のみ999990点で実際は、ちゃんとその分加算される) */
//
			  if (99999 < score_value)	{	score_value = 99999;	}	/* MAX 999990[pts] */
	i = 0; while ( 9999 < score_value)	{	score_value -= 10000; i++; jjj=1;	}; if (0!=jjj)	{	color8888=0xff00ffff;	regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 100000 十万 */
	i = 0; while (	999 < score_value)	{	score_value -=	1000; i++; jjj=1;	}; if (0!=jjj)	{	color8888=0xff00ffff;	regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 10000  一万 */
	i = 0; while (	 99 < score_value)	{	score_value -=	 100; i++; jjj=1;	}; if (0!=jjj)	{							regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 1000 	千 */
	i = 0; while (	  9 < score_value)	{	score_value -=	  10; i++; jjj=1;	}; if (0!=jjj)	{							regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 100		百 */
	i = 0; while (	  0 < score_value)	{	score_value--;		  i++; jjj=1;	}; if (0!=jjj)	{							regist_score(i, color8888, (x256), y256); } x256 += t256(8);/* 10		十 */
//
	regist_score(0, color8888, (x256), y256);/* "0" スコアの末尾は必ず０ */
}


/*---------------------------------------------------------
	取ったアイテムの得点を説明表示(任意得点の場合)
---------------------------------------------------------*/

void bonus_info_any_score_nodel(OBJ/**/ *src, u32 score_num_pts)/*int x, int y*/
{
	player_dummy_add_score(score_num_pts);
	bonus_info_shered_add_score10_value(src, (s32)score_num_pts);
}


/*---------------------------------------------------------
	取ったアイテムの得点を説明表示(固定得点の場合)
---------------------------------------------------------*/

void bonus_info_score_nodel(OBJ/**/ *src, int score_type)/*int x, int y*/
{
	static const unsigned short score_tbl[32] =
	{
		score(76800), score(   10), score(	 20), score(   30),
		score(	 40), score(   50), score(	 60), score(   70),
		score(	 80), score(   90), score(	100), score(  200),
		score(	300), score(  400), score(	500), score(  600),
		//
		score(	700), score(  800), score(	900), score( 1000),
		score( 2000), score( 3000), score( 4000), score( 5000),
		score( 6000), score( 7000), score( 8000), score( 9000),
		score(10000), score(11000), score(12000), score(51200),
	};
	int score_num;	score_num = score_tbl[(score_type)/*&(32-1)*/];
	bonus_info_any_score_nodel(src, score_num);
}


