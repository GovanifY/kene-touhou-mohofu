
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリス人形弾幕
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形 -easy-」
アリスの放った四体の人形が、二回分裂する7way弾を撃つ。
4 x 7 x 7 == 最大 196[弾](妖々夢)
3 x 7 x 7 == 最大 147[弾](模倣風)	//3 x 8 x 8 == 192
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形」	赤鱗弾
アリスの放った六体の人形が、二回分裂する7way弾を撃つ。
6 x 7 x 7 == 最大 294[弾](妖々夢)
5 x 7 x 7 == 最大 245[弾](模倣風)	//5 x 8 x 8 == 320
	-------------------------------------------------------
蒼符「博愛の仏蘭西人形 -hard-」
アリスの放った八体の人形が、二回分裂する7way弾を撃つ。
8 x 7 x 7 == 最大 392[弾](妖々夢)
9 x 7 x 7 == 最大 441[弾](模倣風)	//7 x 8 x 8 == 448
	-------------------------------------------------------
蒼符「博愛のオルレアン人形」	赤鱗弾→緑鱗弾
アリスの放った八体の人形が、三回分裂する7way弾を撃つ。
8 x 7 x 7 x 7 == 最大2744[弾](妖々夢)
2 x 7 x 7 x 7 == 最大 686[弾](模倣風)	//9 x 8 x 8 == 576
	-------------------------------------------------------
	人形が分裂(模倣風)
	弾が分裂(妖々夢)
	現在人形は雑魚を利用しているが、
	このような方式になっているのは、実験中の為。
	-------------------------------------------------------
	実際「分裂」は(弾は弾幕リストなので)弾幕リストを調べて
	「(角度を)変更」＋「(新たに弾を)追加」するんじゃないかな？
	つまり大雑把に仕組みを言えば、「分裂」弾じゃなくて「追加」弾。
	「分裂」と「追加」だと、結果的に何が違うかと言えば、
	最終的な弾の「表示プライオリティー」に微妙な違いが出る。
	-------------------------------------------------------
	つまり、7way弾に分裂するのではなくて、
	1弾は変身させて、6弾追加するのだと思う。
---------------------------------------------------------*/

#define target_x256 	user_data00 	/* 基準固定 x位置 */
#define target_y256 	user_data01 	/* 基準固定 y位置 */
#define radius			user_data02 	/* 円の半径 */
#define boss_time_out	user_data03 	/* 経過時間 */
#define recursive		user_data04 	/* 分裂回数(0>分裂しない) */


/*---------------------------------------------------------
	分裂(last)
	-------------------------------------------------------
	BU LLET_UROKO14_00_AOI,
	BU LLET_UROKO14_01_AKA, 	normal
	BU LLET_UROKO14_02_YUKARI,
	BU LLET_UROKO14_03_MIDORI,	luna
//
	BULLET_UROKO14_00_SIRO, 		//白	//BU LLET_UROKO14_00_AOI,		//
	BULLET_UROKO14_01_AKA,			//赤	//BU LLET_UROKO14_01_AKA,		//
	BULLET_UROKO14_02_YUKARI,		//紫	//BU LLET_UROKO14_02_YUKARI,	//
	BULLET_UROKO14_03_AOI,			//青	//BU LLET_UROKO14_03_MIDORI,	//
	BULLET_UROKO14_04_MIZUIRO,		//水	//BU LLET_UROKO14_04_MIZUIRO,	//
	BULLET_UROKO14_05_MIDORI,		//緑	//BU LLET_UROKO14_05_KIIRO, 	//
	BULLET_UROKO14_06_KIIRO,		//黄	//BU LLET_UROKO14_06_dummy, 	//
	BULLET_UROKO14_07_DAIDAI,		//橙	//BU LLET_UROKO14_07_dummy, 	//
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(SPRITE *src)
{
	obj_send1->cx256					= (src->cx256); 	/* 弾源x256 */
	obj_send1->cy256					= (src->cy256); 	/* 弾源y256 */
	#if 0
	br.BULLET_REGIST_00_speed256				= (t256(2.0));						/* 弾速 */
	br.BULLET_REGIST_02_VECTOR_angle1024		= (src->rotationCCW1024);			/* 角度 */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_03_AOI+((cg_game_difficulty)); 		/* [青鱗弾] */
	br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
	br.BULLET_REGIST_06_n_way					= (7);								/* [7way] [8way] */
	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/23);					/* 分割角度 (1024/27) (1024/24) */
	bul let_reg ist_vec tor();
	#endif
	#if 1
	const u8 tama_color[4] =
	{
		3,//BULLET_UROKO14_03_AOI,
		1,//BULLET_UROKO14_01_AKA,
		2,//BULLET_UROKO14_02_YUKARI,
		5 //BULLET_UROKO14_05_MIDORI
	};
	br.BULLET_REGIST_01_speed_offset			= t256(1);/*(テスト)*/
	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)標準弾 */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_00_SIRO+(tama_color[((cg_game_difficulty))]);			/* [青鱗弾] */
//未定br.BULLET_REGIST_05_regist_type			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)標準弾 */
//	br.BULLET_REGIST_06_n_way					= (1);								/* [7way] [8way] */
//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/23);					/* 分割角度 (1024/27) (1024/24) */
	int first_angle_65536;
	first_angle_65536 = ((src->rotationCCW1024)<<6);
	unsigned int i;
	for (i=(0); i<(7); i += (1) )	/* 弾数 */
	{
		enum
		{
			CCC_00_SPEED256 = 0,
			CCC_01_ADD_ANGLE65536,
			CCC_99_MAX
		};
		const int ccc[8][CCC_99_MAX] =
		{
			{ t256(1.00), ((65536*10)/(14))},	// 速度速い
			{ t256(0.50), ((65536* 8)/(14))},	// 速度遅い
			{ t256(1.00), ((65536* 9)/(14))},	// 速度速い
			{ t256(0.75), ((65536* 7)/(14))},	// 速度中
			//
			{ t256(1.00), ((65536* 6)/(14))},	// 速度速い
			{ t256(0.50), ((65536* 5)/(14))},	// 速度遅い
			{ t256(1.00), ((65536* 4)/(14))},	// 速度速い
			{ t256(1.00), ((65536* 7)/(14))},	// [未使用](.align)
		};
		br.BULLET_REGIST_00_speed256				= (ccc[i][CCC_00_SPEED256]);									/* 弾速 */
		br.BULLET_REGIST_02_angle65536				= (first_angle_65536+ccc[i][CCC_01_ADD_ANGLE65536]);			/* 角度 */
		tama_system_regist_single();/* (r33-) */
	}
	#endif
}
/*
	180度反転する。
	７弾に分裂
  A   A    A   A   --- 速度速い

		A------------- 速度中
		|
   A	|	 A	------ 速度遅い
		+
//
*/
/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src);

static void move_alice_doll_all(SPRITE *src)
{
	src->boss_time_out--;/* 時間経過 */
	if ( 0 > src->boss_time_out )/* 移動終了 */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
		{
			if (0 > src->recursive)/* 再分裂する必要あり？ */
			{
				move_alice_doll_last_burrets(src);/* 再分裂しない。ので弾にする。 */
			}
			else/* 再分裂する */
			{
				add_zako_alice_doll_2nd_CCW(src);/* (再帰で)再分裂する */
			}
		}
		src->jyumyou = JYUMYOU_NASI;	/* おしまい */
	//	return;
	}
	else/* 移動処理 */
	{
		src->rotationCCW1024 += (((src->recursive)&(1))?(10):(-10));/* 人形＆グラ回転 */
		mask1024(src->rotationCCW1024);
		{
			int bbb_rotationCCW1024;
			bbb_rotationCCW1024 = src->rotationCCW1024+(1024/8);/*	(1024/8) ==  45/360度傾ける */
		//	bbb_rotationCCW1024 = src->rotationCCW1024-(1024/8);/* -(1024/8) == -45/360度傾ける */
			mask1024(bbb_rotationCCW1024);
			src->radius++;
			src->cx256 = src->target_x256 + ((sin1024((bbb_rotationCCW1024))*src->radius));/*fps_factor*/
			src->cy256 = src->target_y256 + ((cos1024((bbb_rotationCCW1024))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	敵を追加する(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
//	const int add_angle = ( (1024/7) ); /* 加算角度 */	/* ２回目以降の分列数は常に7回 */
	int first_angle1024;	/* 開始角度 */
	first_angle1024 = (src->rotationCCW1024)+(1024/4);
	int i_angle1024;	/* 積算角度 */
//	for (i_angle1024=(0); i_angle1024<(1024); i_angle1024 += (1024/7)/*add_angle*/) 	/* 弾数 */
	/* 半周を7分割 */
	for (i_angle1024=(0); i_angle1024<(1024/2); i_angle1024 += (1024/(7*2))/*add_angle*/)	/* 弾数 */
	{
		SPRITE *h;
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= /*TEKI_16_YOUSEI1_5*/TEKI_12_YOUSEI1_1+((1)<<2)+((src->recursive)<<2);	/*SP_ZAKO*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_teki	= callback_hit_zako;
			h->rotationCCW1024		= (first_angle1024+i_angle1024);
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*ダミー*/
			/*data->base.*/h->base_hp			= (9999);/*倒せない*/
	//
			h->boss_time_out			= (60);/* 1[秒](60[frame])後に、再分裂 */
			{
				/* 初期位置 */
			/*	h->cx256 =*/ h->target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->target_y256 = (src->cy256);
			//
		//		h->vx256 = (0);/*右方向*/
		//		h->vy256 = (0);/*下方向*/
			}
			h->radius				= (0);
			h->recursive			= ( ((src->recursive)-1) );/* 再分裂回数を1減らす。(テロメア) */
		}
	}
}


/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[模倣風]*/ 	/*(オルレアン人形)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(オルレアン人形)*/ 	/* [現在の方式では無理] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[妖々夢風]*/	/*(オルレアン人形)*/	/* [現在の方式では無理] */
		/* 模倣風はr32現在あたり判定意図的に小さくしているので、いくら1面とはいえ
			やっぱ難易度低すぎる気もする。(オルレアン人形)以外妖々夢風にした。 */
	};
	const int add_angle1024 = ( (aaa_tbl[((cg_game_difficulty))])); /* 加算角度 */	/* ２回目以降の分列数は常に7回 */

	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* 一周 */
	{
		SPRITE *h;
		h							= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->rotationCCW1024		= (i_angle1024);
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*ダミー*/
			/*data->base.*/h->base_hp			= (9999);/*倒せない*/
	//
			h->boss_time_out			= (60);
			{
				/* 初期位置 */
			/*	h->cx256 =*/ h->target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->target_y256 = (src->cy256);
			//
		//		h->vx256 = (0);/*右方向*/
		//		h->vy256 = (0);/*下方向*/
			}
			h->radius				= (0);
			h->recursive			= ( ((3==(cg_game_difficulty))?(1):(0)));/*(Lunatic==オルレアン人形)*/
		}
	}
}



#if 00
/*---------------------------------------------------------
	敵を追加する(common)
---------------------------------------------------------*/

static void add_zako_alice_doll_common(SPRITE *src, int is_the_first)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[模倣風]*/ 	/*(オルレアン人形)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(オルレアン人形)*/ 	/* [現在の方式では無理] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[妖々夢風]*/	/*(オルレアン人形)*/	/* [現在の方式では無理] */
		/* 模倣風はr32現在あたり判定意図的に小さくしているので、いくら1面とはいえ
			やっぱ難易度低すぎる気もする。(オルレアン人形)以外妖々夢風にした。 */
	};
	const int add_angle1024 = ((0==is_the_first)?(1024/7):(aaa_tbl[(difficulty)])); /* 加算角度 */	/* ２回目以降の分列数は常に7回 */
//
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* 弾数 */
	{
		SPRITE *h;
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((src->recursive)<<2);
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
		//
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
			h->callback_hit_teki	= callback_hit_zako;
		//
		//	h->base_score			= score(25*2);/*ダミー*/
			h->base_hp				= (9999);/*倒せない*/
		//
			h->rotationCCW1024		= (i_angle1024);
			h->boss_time_out			= (60);
			{
				/* 初期位置 */
			/*	h->cx256 =*/ h->target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->target_y256 = (src->cy256);
			//
		//		h->vx256 = (0);/*右方向*/
		//		h->vy256 = (0);/*下方向*/
			}
			h->radius		= (0);
			h->recursive	= ((0==is_the_first)?((src->recursive)-1):((3==difficulty)?(1):(0)));
		}
	}
}


/*---------------------------------------------------------
	敵を追加する(2nd)
---------------------------------------------------------*/
static void add_zako_alice_doll_2nd_CCW(SPRITE *src)
{
	add_zako_alice_doll_common(src, 0);
}


/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll(SPRITE *src)
{
	add_zako_alice_doll_common(src, 1);
}
#endif
