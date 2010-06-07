
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"赤毛玉1",		"MINE",
	-------------------------------------------------------
	ぐるぐるその場で動かないで、回転アニメしてて
	ほっとくと8つに分裂して
	最後に高速弾撃って来るやつ(橙)
	-------------------------------------------------------
	このキャラのみ分裂するんだけど、システムとして
	分裂はサポートしなくなるかもしれない(?)。
	その場合は始めから、8つ重なって出てくる。
	分裂は、つまりアリス弾だからサポートされるとしたら、
	最終的なアリス弾の仕組み(まだ確定していない)と同じ仕組みになると思う。
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

#define time_out	user_data03

/*---------------------------------------------------------
	敵移動(分裂後)
---------------------------------------------------------*/

static void move_aka_kedama1_2nd(SPRITE *src)
{
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const u16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
	//	if (0==(ra_nd()%(100-/*data->*/enemy_rank*10)))
		if (kakuritu_tbl[/*data->*/(difficulty)/*(enemy_rank)*/] > (ra_nd()&(65536-1)))
		{
			obj_send1->x256 = src->x256;
			obj_send1->y256 = src->y256;
			br.BULLET_REGIST_speed256			= t256(2.5)+(/*data->*/0/*(enemy_rank<<6)*/);				/* 弾速 */	/*高速弾*/	/*t256(3+data->enemy_rank)*/
		//	br.BULLET_REGIST_speed256			= speed256; 				/* 弾速 */
			br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* 自機狙い弾 */	/* 発射中心角度 / 特殊機能(自機狙い/他) */
		//	br.BULLET_REGIST_div_angle1024		= (0);						/* ダミー角度(未使用) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_00_AKA;		/* [赤弾] */ /* 弾グラ */
			br.BULLET_REGIST_n_way				= (1);						/* [1way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}
/* CCWの場合 */
	src->x256 += (src->vx256);/*fps_factor*/
	src->y256 += (src->vy256);/*fps_factor*/
	gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
//
	src->m_angleCCW1024 += (16);	/*(20)*/
	mask1024(src->m_angleCCW1024);
}


/*---------------------------------------------------------
	敵を追加する(変身)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(SPRITE *src/*, int lv*/)
{
	int i;
	for (i=0; i<8; i++)/*分裂*/
	{
		SPRITE *h;
		h						= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->type 				= TEKI_56_CHEN;
			h->callback_mover		= move_aka_kedama1_2nd;
			h->callback_loser		= lose_random_item; 	/* 爆発させたほうがアイテムが稼げる */
			h->callback_hit_enemy	= callback_hit_zako;
			//
			h->flags				= src->flags;	/* 分裂前をそのまま引き継ぐ */
			h->x256 				= src->x256;	/* 分裂前をそのまま引き継ぐ */
			h->y256 				= src->y256;	/* 分裂前をそのまま引き継ぐ */
			/*data->base.*/h->base_score		= src->base_score;		/* 爆発させたほうがスコアが稼げる */
			/*data->base.*/h->base_hp			= src->base_hp; 		/* 分裂前の体力をそのまま引き継ぐ(減らしてあればその分軟らかい) */
			{
				const int data_zzz_angle1024				= (i<<7);	/* 分裂角度 */	//  (/*360*/1024/8)*i;
				const int data_zzz_speed					= (t256(1.5)+(difficulty<<6));	/* 分裂速度 */	/*(3.0)*/
			//
				h->vx256 = ((sin1024((data_zzz_angle1024))*(data_zzz_speed))>>8);/*fps_factor*/
				h->vy256 = ((cos1024((data_zzz_angle1024))*(data_zzz_speed))>>8);/*fps_factor*/
			}
		}
	}
}

//			/*data->base.*/h->base_score		= score(10*2)*(1+(2/*di fficulty*/));		/*(enemy_rank)*/	/*9*/
//			/*data->base.*/h->base_hp			= (8*(1+ 9))+(1/*di fficulty*/<<(4+3)); 	/*(enemy_rank)*/

/*---------------------------------------------------------
	敵移動(本体)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(SPRITE *src)
{
	src->time_out--;
	if (0 > src->time_out)	/* 分裂 */
	{
		add_local_aka_kedama1_2nd(src/*,data->enemy_rank*/);/* 変身 分裂 */
		src->type = SP_DELETE;		/* 本体はおしまい */
	}
	else
	if (31 > src->time_out)
	{
		;							/* 止まる */
	}
	else
	{
									/* 移動 move */
		src->x256 += (src->vx256);	/*fps_factor*/
		src->y256 += (src->vy256);	/*fps_factor*/
		//
		src->m_angleCCW1024 += (10);	/* グラ回転 */
		mask1024(src->m_angleCCW1024);
	}
}


/*---------------------------------------------------------
	敵を追加する(本体)
---------------------------------------------------------*/

global void add_zako_aka_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_56_CHEN;
	//
		h->callback_mover		= move_aka_kedama1_1st;
	//		/* 初期位置 */
		obj_send1->x256 		= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/	/* 弾源x256 */
	//	obj_send1->y256 		= ((ra_nd()&((256*256)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
		obj_send1->y256 		= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/	/* 弾源y256 */
		tmp_angleCCW1024_jikinerai(obj_send1, h);
		h->vx256				= ((sin1024(h->tmp_angleCCW1024)*t256(1.5))>>8);/*fps_factor*/	/* CCWの場合 */
		h->vy256				= ((cos1024(h->tmp_angleCCW1024)*t256(1.5))>>8);/*fps_factor*/
	//
		h->time_out 			= 80+(1*60);
	}
}
