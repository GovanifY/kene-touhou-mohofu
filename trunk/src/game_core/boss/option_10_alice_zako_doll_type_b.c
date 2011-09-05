
#include "boss.h"//#include "game_main.h"

#if (1)
void add_zako_alice_doll_type_b(OBJ *src)
{
	/*(r36現在使ってない!!!)*/
}

#else

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	アリス人形カード
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
	実際「分裂」は(弾は全弾配列なので)全弾配列を調べて
	「(角度を)変更」＋「(新たに弾を)追加」するんじゃないかな？
	つまり大雑把に仕組みを言えば、「分裂」弾じゃなくて「追加」弾。
	「分裂」と「追加」だと、結果的に何が違うかと言えば、
	最終的な弾の「表示プライオリティー」に微妙な違いが出る。
	-------------------------------------------------------
	つまり、7way弾に分裂するのではなくて、
	1弾は変身させて、6弾追加するのだと思う。
	-------------------------------------------------------
	ボス共通規格使用データー:
		BOSS_DATA_00_target_x256	基準固定座標 x位置として使用。
		BOSS_DATA_01_target_y256	基準固定座標 y位置として使用。
		BOSS_DATA_05_move_jyumyou	たぶん。経過時間。
---------------------------------------------------------*/

// バグ対策。 user_data02 良くワカンナイけど空けとく。()


//#define recursive user_data06 	/* 分裂回数(0>分裂しない) */
#define radius		user_data07 	/* 円の半径 */


/*---------------------------------------------------------
	分裂(last)
	-------------------------------------------------------
	BU LLET_UROKO14_00_AOI,
	BU LLET_UROKO14_01_AKA, 	normal
	BU LLET_UROKO14_02_YUKARI,
	BU LLET_UROKO14_03_MIDORI,	luna
---------------------------------------------------------*/

static void move_alice_doll_last_burrets(OBJ *src)
{
	set_REG_DEST_XY(src);	/* 弾源x256 y256 中心から発弾。 */
//	HATSUDAN_01_speed256			= (t256(2.0));						/* 弾速 */
	HATSUDAN_01_speed256			= (t256(1.0));						/* 弾速 */
	HATSUDAN_02_speed_offset		= t256(1);/*(テスト)*/
	HATSUDAN_03_angle65536			= ((src->tmp_angleCCW1024)<<6); 		/* 角度 */
	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)標準弾 */
	HATSUDAN_05_bullet_obj_type 	= BULLET_UROKO14_BASE + TAMA_IRO_01_AKA/*+(difficulty)*/;		/* [赤鱗弾] */
	HATSUDAN_06_n_way				= (7);								/* [7way] [8way] */
	HATSUDAN_07_div_angle65536		= (int)(65536/23);					/* 分割角度 (1024/27) (1024/24) */
	hatudan_system_regist_katayori_n_way();/* (r33-) */
}

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/

static void move_alice_doll_all(OBJ *src)
{
	src->BOSS_DATA_05_move_jyumyou--;/* 時間経過 */
	if (0 > src->BOSS_DATA_05_move_jyumyou)/* 移動終了 */
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
		src->rotationCCW1024	+= ( (-10));	/* 人形グラ回転 */
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
			#if (0)//
			src->cx256	= src->BOSS_DATA_00_target_x256 + ((si n1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/	/* CCWの場合 */
			src->cx256	= src->BOSS_DATA_00_target_x256 + ((si n1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/
			src->cy256	= src->BOSS_DATA_01_target_y256 + ((co s1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/
			src->cy256	= src->BOSS_DATA_01_target_y256 + ((co s1024((bbb_tmp_angleCCW1024))*(src->radius)));	/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (bbb_tmp_angleCCW1024), &sin_value_t256, &cos_value_t256);
				src->cx256			= src->BOSS_DATA_00_target_x256 + ((sin_value_t256)*(src->radius));/*fps_factor*/
				src->cy256			= src->BOSS_DATA_01_target_y256 + ((cos_value_t256)*(src->radius));/*fps_factor*/
			}
			#endif
		}
	}
}

/*---------------------------------------------------------
	敵を追加する(1st)
---------------------------------------------------------*/

void add_zako_alice_doll_type_b(OBJ *src)
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
//	if (1 < (REG_0f_GAME_DIFFICULTY/*&3*/))/*(??????巧くいかない。符号？？？)*/
	if (0 != (REG_0f_GAME_DIFFICULTY/*&3*/))/*(easy 以外は)*/
	{
		REG_0f_GAME_DIFFICULTY = (1);/*(強制 normal にする。)*/
	}/*(r36-: hard, luna をハングアップするので強制 normal にする。)*/
	//
	/*(????????r36現在こっちを使ってる。add_zako_alice_doll_type_aは使ってなかった。)*/
	//
	const int add_angle1024 = ( (aaa_tbl[((REG_0f_GAME_DIFFICULTY))])); /* 加算角度 */	/* ２回目以降の分列数は常に7回 */
	//
	int i_angle1024;	/* 積算角度 */
	for (i_angle1024=0; i_angle1024<(1024); i_angle1024 += add_angle1024)	/* 一周 */
	{
		OBJ *h;
		h							= obj_add_A01_teki_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI02_PNG;
			h->obj_type_set 		= TEKI_16_10;
		//	h->atari_hantei 		= (1あり);/*(score兼用)*/
			h->callback_mover		= move_alice_doll_all;
		//	h->callback_loser		= NULL;
	//		h->callback_hit_teki	= callback_hit_zako;/*???*/
			h->tmp_angleCCW1024 	= (i_angle1024);
			h->rotationCCW1024		= (0);	/* 描画用角度 */
	//
	//		h->base_score		= score(25*2);/*ダミー*/
			h->base_hp			= (9999);/*倒せない*/
	//
			h->BOSS_DATA_05_move_jyumyou			= (60);
			{
				/* 初期位置 */
			/*	h->cx256 =*/ h->BOSS_DATA_00_target_x256 = (src->cx256);
			/*	h->cy256 =*/ h->BOSS_DATA_01_target_y256 = (src->cy256);
			//
		//		h->v x256 = (0);/*右方向*/
		//		h->v y256 = (0);/*下方向*/
			}
			h->radius				= (0);
//			h->recursive			= ( ((3==difficulty)?(1):(0)));/*(Lunatic==オルレアン人形)*/
		}
	}
}
#endif
