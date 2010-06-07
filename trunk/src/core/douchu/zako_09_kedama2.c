
#include "douchu.h"

/*---------------------------------------------------------
	東方模倣風  〜 Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"毛玉2",	"BADGUY",	(r27から新規追加)
	-------------------------------------------------------
	本家毛玉
---------------------------------------------------------*/
#if 0/* めも */
/* ボス共通規格 */
	#define target_x256 		user_data00 	/* 目標x座標 */
	#define target_y256 		user_data01 	/* 目標y座標 */
	#define vvv256				user_data02 	/* 目標座標への到達割合 */
	#define time_out			user_data03 	/* 制限時間 */
#endif

//#define tx256 	user_data00
//#define ty256 	user_data01
//#define speed256	user_data06

#define time_out	user_data03
#define state		user_data04

#define kougeki_type		user_data05

#define USE_X_HOUKOU (1)

/*---------------------------------------------------------
	敵移動
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST02,
	ST03,
};

static void move_kedama2(SPRITE *src)
{
	src->time_out--;/*fps_factor*/
//
	switch (src->state)
	{
	case ST00:	/* ばらばらに登場させる */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			/*画面外*/;
		}
		else
		{
			src->time_out	= 64;
			src->state++;	/* 登場する */
		}
		break;
	case ST02:	/* 攻撃 */
		{	enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* 最大数 */
			};
			const u16 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:紅っぽい毛玉(狭角度 奇数弾) */
				{0x3f, (u16)(1024/12), (5)},	/* (u32)(1024/12):広角なので簡単 */
				{0x1f, (u16)(1024/16), (3)},	/* 0x0f:普通に避けれる間隔 */
				{0x0f, (u16)(1024/20), (3)},	/* 0x0f:普通に避けれる間隔 */
				{0x07, (u16)(1024/24), (5)},	/* (u32)(1024/24):普通の狭角 */
				/* 1:妖っぽい毛玉(90度 4方向) (無駄弾なので難易度は低い筈) */
				{0x3f, (u16)(1024/8),  (8)}, 	/* 8:見せ弾(無駄弾なので難易度は低い筈) */
				{0x1f, (u16)(1024/8),  (8)}, 	/* 0x1f:普通に避けれる間隔 */
				{0x07, (u16)(1024/4),  (4)},
				{0x03, (u16)(1024/4),  (4)}, 	/* 0x03:連続弾っぽく */
			};
			if (0==(src->time_out & kougeki_tbl[src->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				obj_send1->x256 = src->x256;
				obj_send1->y256 = src->y256;
				br.BULLET_REGIST_speed256			= (t256(2.5)+((difficulty)<<6));/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN;
				br.BULLET_REGIST_div_angle1024		= kougeki_tbl[src->kougeki_type][K01_SUKIMA_KAKUDO];	//	(int)(1024/24),
				br.BULLET_REGIST_bullet_obj_type	= BULLET_KOME_01_AOI+(src->x256&0x07);
				br.BULLET_REGIST_n_way				= kougeki_tbl[src->kougeki_type][K02_TAMA_KAZU]; 		//	(3/*+difficulty+difficulty*/)
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
		}
		if (0 > src->time_out)
		{
			src->state++;
		}
		break;
	case ST03:
		gamen_gai_nara_zako_osimai(src);/* 画面外ならおしまい */
		break;
	}

	/* 移動する */
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
//
	src->m_angleCCW1024 += 10;
	mask1024(src->m_angleCCW1024);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

global void add_zako_kedama2(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* 登録できた場合のみ */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_59_HAI_KEDAMA;
		h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*白っぽく */
		h->callback_mover		= move_kedama2;
		//
		h->y256 				= ((l->user_y)<<8);
		h->vx256				= ((l->user_x));
		h->vy256				= (0);
		//
		{
			if (0 < (h->vx256))
			{/* [引数 user_y が正方向の場合、右へ移動(→)] [引数yが負方向の場合、下へ移動(↓)] */
				h->x256 			= t256(-30)/*-(i<<(5+8))*/;
			}
			else
			{/* [引数 user_y が正方向の場合、左へ移動(←)] [引数yが負方向の場合、上へ移動(↑)] */
				h->x256 			= t256(360)/*+(i<<(5+8))*/;/* 360 > 352(x_max) > 272(y_max) */
			}
		}
		//
		if (0 < (h->y256))
		{	;	}
		else
		{
			/* y座標が負方向の場合は、x座標指定なので(x座標 y座標 を逆にする) */
			{
				int s_sss;
				s_sss				= h->x256;
				h->x256 			= -(h->y256);
				h->y256 			= s_sss;
			}
			h->vy256			= (h->vx256);
			h->vx256			= (0);
		}
		h->state			= ST00;
		h->time_out 		= 64/*+(i<<4)*/;
		h->kougeki_type 	= (((l->user_255_code)&1)<<2)|(difficulty/*&0x03*/);
	}
}
