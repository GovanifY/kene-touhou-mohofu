
#include "boss.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ボスの移動。
	ボスのアクション後の処理。
	-------------------------------------------------------
	雑魚のアクション後の処理。
---------------------------------------------------------*/


/*---------------------------------------------------------
	ボス形態変更時の共通
---------------------------------------------------------*/

//extern OBJ *obj_effect[8]; /* [□]（カードのグラフィック） */
		#if 0/* あたった敵は倒せない敵かもしれないので、ここで必ず消すのはまずい気がする */
	/* あたった敵爆発 */
	bakuhatsu_add_type(src, BAKUHATSU_MINI00);
	cg.teki->jyumyou	= (0);/* ??? */
	set_REG_DEST_XY(src);/* 発弾位置 座標xy */
	bakuhatsu_add_type_ccc(BAKUHATSU_FIRE08);
		#endif
/*---------------------------------------------------------
	敵移動(ザコ消滅爆発エフェクト)
---------------------------------------------------------*/
//	/* ザコ消滅爆発エフェクトを登録(現在Gu部分がないので描画してない) */
//	set_REG_DEST_XY(src);/* 発弾位置 座標xy */
//	bakuhatsu_add_type_ccc(BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/* ザコ消滅爆発 */

static void move_zako_yarare(OBJ *src)
{
	/* 描画用グラ回転 */
	src->rotationCCW1024	= (src->jyumyou<<6);
	/* グラ拡大 */
	src->m_zoom_x256		= (t256(4.0))-(src->jyumyou<<3);
	src->m_zoom_y256		= (t256(4.0))-(src->jyumyou<<3);
	/* 半透明 */
	src->color32 -= 0x08000000; /* 8==256/32 */
}
static void init_zako_yarare(OBJ *src)
{	// ザコ消滅爆発エフェクトを登録
	src->callback_mover 	= move_zako_yarare;
	src->callback_hit_teki	= NULL;
	src->jyumyou			= (32);/* 雑魚やられ時間。 */
	src->atari_hantei		= (ATARI_HANTEI_OFF);/*(ザコ消滅爆発エフェクトには当たり判定無し)*/
	static int bbb = 0;
	const u32 aaa[4] =
	{
		MAKE32RGBA(0xff, 0x00, 0x00, 0xff), // 赤
		MAKE32RGBA(0x00, 0xff, 0x00, 0xff), // 緑
		MAKE32RGBA(0x00, 0x00, 0xff, 0xff), // 青
		MAKE32RGBA(0xff, 0xff, 0x00, 0xff), // 黄
	};
	bbb++;
	bbb &= (4-1);
	src->color32			= aaa[bbb];
}
/*---------------------------------------------------------
	ザコを攻撃して、ザコに自弾があたった場合の共通ルーチン
	-------------------------------------------------------
	ザコは道中ザコ以外に、ボスが生成する使い魔も含む。
	-------------------------------------------------------
	OBJ *src;	ザコ敵自体
	OBJ *tama;	自弾
---------------------------------------------------------*/

/* 使い魔があるからglobal */
global void callback_hit_zako(OBJ *src, OBJ *tama)
{
	/* 雑魚に自弾があたった場合の火花エフェクトを登録(現在Gu部分がないので描画してない) */
	set_REG_DEST_XY(tama);/* 発弾位置 座標xy */
	bakuhatsu_add_type_ccc(BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/* 先に実行した方が速い */
//
	/* 上と分離した方がコード効率があがる。 */
	{
		src->base_hp -= tama->kougeki_ti; 	/* 攻撃して体力減らす(強さ分引く) */
		if (0 >= src->base_hp)			/* ０か負値なら、倒した。 */
		{
			/* (ザコの)カスタムやられ処理 */
			if (NULL != src->callback_loser)	/* カスタムやられ処理があれば実行。 */
			{
				(src->callback_loser)(src); 	/* カスタムやられ処理を実行する。(標準は「ランダムテーブルからアイテム生成」) */
				src->callback_loser = NULL; 	/* 実行したので(念の為)消す。 */
				/* (やられ処理は1度しか実行しない) */
			}
			/* ザコ、スコア加算処理 */
			bonus_info_any_score_nodel(src, ((src->base_score)*(cg.game_now_stage)) );
		//	/* 自動消去へ仕様変更 */src->jyumyou = JYUMYOU_NASI;/* スプライトシステムに再利用をお願いする。 */
			init_zako_yarare(src);	// ザコを消滅爆発エフェクトにする。
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);/* 雑魚爆発音を鳴らす。 */
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static int ee_angle1024;
static int ff_angle1024;
/*static*/ void move_card_square_effect(OBJ *src)
{
	ee_angle1024 += (4);	/* 4 回転速度 */
	ff_angle1024 += (2);	/* 2 拡大縮小速度 */
	int ww_angle1024;	ww_angle1024 = ee_angle1024;
	int vv_angle1024;	vv_angle1024 = ff_angle1024;
	int radius;
	int i;
	for (i=0; i<5; i++)
	{
		ww_angle1024 += 20; 	/* 20 回転速度 */
		vv_angle1024 += 200;	/* 40 拡大縮小速度 */
		{
			int sin_value_t256; 		//	sin_value_t256 = 0;
			int cos_value_t256; 		//	cos_value_t256 = 0;
			int256_sincos1024( ((((vv_angle1024))/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
			radius = (( (sin_value_t256))>>2)+16; /* 80==16+64 */
		}
		OBJ *h;
		h					= &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_08_EFFECT01+i];
		#if (0)//
		h->cx256			= src->cx256 + ((si n1024((ww_angle1024))*(radius)) );/*fps_factor*/	/* CCWの場合 */
		h->cy256			= src->cy256 + ((co s1024((ww_angle1024))*(radius)) );/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (ww_angle1024), &sin_value_t256, &cos_value_t256);
			h->cx256			= src->cx256 + ((sin_value_t256*(radius)) );/*fps_factor*/
			h->cy256			= src->cy256 + ((cos_value_t256*(radius)) );/*fps_factor*/
		}
		#endif
		h->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		h->m_zoom_y256		= ( (1/*+255*/+(radius<<2)) );
		#if 1
		/* 描画用角度(下が0度で左回り(反時計回り)) */
		h->rotationCCW1024	= -ww_angle1024;
		#endif
	}
}

/*static*/ void boss_effect_kotei_obj_r36_taihi(void)
{
	{
		int i;
		for (i=0; i<5; i++)
		{
			OBJ *h;
			h = &obj99[OBJ_HEAD_02_0x0900_KOTEI+FIX_OBJ_08_EFFECT01+i];
			sprite_kotei_obj_r36_taihi(h);/*(effect obj使用中であるが退避位置へ退避)*/
		}
	}
}


/*---------------------------------------------------------
	ボスオプション、共通
---------------------------------------------------------*/
global void check_tukaima_time_out(OBJ *src)
{
	#if 0
	/* ボスを倒すと皆破壊される。 */
//	if (0==boss_life_value)/* ダメかも(?) */
//	if (0==src->base_hp)/* ダメかも(?) */
	OBJ *obj_boss;
	obj_boss = &obj99[OBJ_HEAD_01_0x0800_TEKI+TEKI_OBJ_00_BOSS_HONTAI];/*(ボス本体)*/
	if (0==obj_boss->base_hp)/* ダメかも(?) */
	{
		src->BOSS_DATA_05_move_jyumyou = (0);
	}
	#endif
	if (CARD_BOSS_TIMER_0000_HATUDAN <= card.mode_timer)/*発弾時以外*/ /*(カード中以外)*/
	{
		src->BOSS_DATA_05_move_jyumyou = (0);/*消去*/
	}
	src->BOSS_DATA_05_move_jyumyou--;
	if (0 > src->BOSS_DATA_05_move_jyumyou)
	{
		#if 1
		src->base_hp	= (0);/* (?) これで良いか検証の必要あり */
		#endif
		src->jyumyou		= JYUMYOU_NASI;
	}
}


/*---------------------------------------------------------
	ボスオプション、共通
	時間切れの場合の透明度設定
---------------------------------------------------------*/
global void set_timeout_alpha(OBJ *src)
{
	if (0x1ff > (src->BOSS_DATA_05_move_jyumyou))
	{
		src->color32		= (src->color32 & 0x00ffffff) | ((src->BOSS_DATA_05_move_jyumyou<<(23))&0xff000000);
	}
}
