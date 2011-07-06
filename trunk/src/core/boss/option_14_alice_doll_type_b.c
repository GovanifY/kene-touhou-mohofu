
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
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

#define sx256		user_data00 	/* 基準固定 x位置 */
#define sy256		user_data01 	/* 基準固定 y位置 */
#define time_out	user_data02 	/* 経過時間 */
#define radius		user_data03 	/* 円の半径 */
//#define recursive user_data04 	/* 分裂回数(0>分裂しない) */


/*---------------------------------------------------------
	分裂(last)
	-------------------------------------------------------
	BULLET_UROKO14_00_AOI,
	BULLET_UROKO14_01_AKA,		normal
	BULLET_UROKO14_02_YUKARI,
	BULLET_UROKO14_03_MIDORI,	luna
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(SPRITE *src)
{
	obj_send1->cx256 					= (src->cx256);		/* 弾源x256 */
	obj_send1->cy256 					= (src->cy256);		/* 弾源y256 */
//	br.BULLET_REGIST_speed256			= (t256(2.0));						/* 弾速 */
	br.BULLET_REGIST_speed256			= (t256(1.0));						/* 弾速 */
	br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW1024)<<6); 		/* 角度 */
	br.BULLET_REGIST_div_angle65536 	= (int)(65536/23);					/* 分割角度 (1024/27) (1024/24) */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI+1/*+(difficulty)*/;			/* [赤鱗弾] */
	br.BULLET_REGIST_n_way					= (7);								/* [7way] [8way] */
	br.BULLET_REGIST_speed_offset			= t256(1);/*てすと*/
//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* 現在これしかないが要る */
	bullet_regist_angle();	/* 角度弾として登録 */
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_alice_doll_all(SPRITE *src)
{
	src->time_out--;/* 時間経過 */
	if ( 0 > src->time_out )/* 移動終了 */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* このラインより下からは敵が撃たない */
		{
			move_alice_doll_last_burrets(src);/* 弾を撃つ。 */
		}
		src->jyumyou = JYUMYOU_NASI;	/* おしまい */
	//	return;
	}
	else/* 移動処理 */
	{
		src->rotationCCW1024 	+= ( (-10));	/* 人形グラ回転 */
		mask1024(src->rotationCCW1024);
	//
//		src->tmp_angleCCW1024 += (((src->recursive)&(1))?(10):(-10));/* 人形＆グラ回転 */
		src->tmp_angleCCW1024	+= ( (-10));	/* 人形位置回転 */
		mask1024(src->tmp_angleCCW1024);
		{
			int bbb_tmp_angleCCW1024;
			bbb_tmp_angleCCW1024 = src->tmp_angleCCW1024+(1024/8);/*  (1024/8) ==  45/360度傾ける */
		//	bbb_tmp_angleCCW1024 = src->tmp_angleCCW1024-(1024/8);/* -(1024/8) == -45/360度傾ける */
			mask1024(bbb_tmp_angleCCW1024);
			src->radius++;
			src->cx256 = src->sx256 + ((sin1024((bbb_tmp_angleCCW1024))*src->radius));/*fps_factor*/
			src->cy256 = src->sy256 + ((cos1024((bbb_tmp_angleCCW1024))*src->radius));/*fps_factor*/
		}
	}
}

/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll_type_b(SPRITE *src)
{
	const int aaa_tbl[(4)] =
	{
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/2),/*[模倣風]*/ 	/*(オルレアン人形)*/
//		(int)(1024/3), (int)(1024/5), (int)(1024/9), (int)(1024/3),/*(オルレアン人形)*/ 	/* [現在の方式では無理] */
//		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/2),/*[妖々夢風]*/	/*(オルレアン人形)*/	/* [現在の方式では無理] */
		(int)(1024/4), (int)(1024/6), (int)(1024/8), (int)(1024/8),/*[妖々夢風]*/	/*(オルレアン人形)*/	/* [現在の方式では無理] */
		/* 模倣風はr32現在あたり判定意図的に小さくしているので、いくら1面とはいえ
			やっぱ難易度低すぎる気もする。(オルレアン人形)以外妖々夢風にした。 */
	};
	const int add_angle1024 = ( (aaa_tbl[(difficulty)]));	/* 加算角度 */	/* ２回目以降の分列数は常に7回 */

	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* 一周 */
	{
		SPRITE *h;
		h							= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->type 				= TEKI_12_YOUSEI1_1+((0/*2*/)<<2);	/*SP_ZAKO*/ /*BOSS_16_YOUSEI11*/ /*_02_YUKARI1*/
			h->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_enemy	= callback_hit_zako;/*???*/
			h->tmp_angleCCW1024 	= (i_angle1024);
			h->rotationCCW1024		= (0);	/* 描画用角度 */
	//
	//		/*data->base.*/h->base_score		= score(25*2);/*ダミー*/
			/*data->base.*/h->base_hp			= (9999);/*倒せない*/
	//
			h->time_out 			= (60);
			{
				/* 初期位置 */
			/*	h->cx256 =*/ h->sx256 = (src->cx256);
			/*	h->cy256 =*/ h->sy256 = (src->cy256);
			//
		//		h->vx256 = (0);/*右方向*/
		//		h->vy256 = (0);/*下方向*/
			}
			h->radius				= (0);
//			h->recursive			= ( ((3==difficulty)?(1):(0)));/*(Lunatic==オルレアン人形)*/
		}
	}
}

