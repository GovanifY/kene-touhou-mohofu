
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	爆発作成
---------------------------------------------------------*/

/*---------------------------------------------------------
	爆発エフェクトの表示
---------------------------------------------------------*/

static OBJ_CALL_FUNC(move_bakuhatsu)
{
	/* 寿命があるうちは動かす。(寿命経過はシステム側がする) */
	if (0 < src->jyumyou)
	{
		src->center.x256 += src->math_vector.x256;
		src->center.y256 += src->math_vector.y256;
	}
}

/*---------------------------------------------------------
	種類別に爆発を追加する
---------------------------------------------------------*/
/*int x256, int y256*/
/*int delay_wait,*/
global void bakuhatsu_add_type_ccc(int type)
{
	{
		OBJ *h;
		h					= obj_regist_teki();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R		= TAMA_ATARI_JIPPOU32_PNG;/*????*/
			h->jyumyou			= (30);/*30 フレーム*/

		//	/* あたり判定のOFF(無敵) */
			h->atari_hantei 		= (ATARI_HANTEI_OFF/*スコア兼用*/);

			#if 1
			h->center.x256			= REG_02_DEST_X;/* 発弾位置 座標x */
			h->center.y256			= REG_03_DEST_Y;/* 発弾位置 座標y */
			#endif
			h->color32			= MAKE32RGBA(0xff, 0xff, 0xff, 0x66);	/*	s->alpha			= 0x80;*/
			//
		//	if (BAKUHATSU_MINI00==type) 	{	type = (ra_nd()%3+1);	}/* あちこちにあると遅い */
			if (0==(type&0xfb))
			{
				type |= (ra_nd()&0x03); if (0==(type & 0x03))	{	type |= (0x01); }
			}
			if (12==(type & 0xff))
			{
				h->math_vector.x256	= (ra_nd()&0x07ff)-(0x03ff);
				h->math_vector.y256	= (ra_nd()&0x07ff)-(0x03ff);
			}
			else
			{
				h->math_vector.x256	= (0);
				h->math_vector.y256	= (0);
			}
			h->obj_type_set 			= SP_FRONT_YUKI;
			h->callback_mover	= move_bakuhatsu;
		}
	}
}


/*---------------------------------------------------------
	円状領域に広がる爆発
---------------------------------------------------------*/

global void bakuhatsu_add_circle(OBJ/**/ *src, int mode)
{
	int i;
	int j;
	j=0;
	for (i=0; i</*64*/(64)/*25*/; i+=8)
	{
		j += (ra_nd()&(/*64*/512-1));
		#if (0)//
		REG_02_DEST_X = (src->center.x256) + ((si n1024((j))*(i)));/*fps_factor*/ /* CCWの場合 */
		REG_03_DEST_Y = (src->center.y256) + ((co s1024((j))*(i)));/*fps_factor*/
		#else
		{
			int sin_value_t256; 	//	sin_value_t256 = 0;
			int cos_value_t256; 	//	cos_value_t256 = 0;
			int256_sincos1024( (j), &sin_value_t256, &cos_value_t256);
			REG_02_DEST_X = (src->center.x256) + ((sin_value_t256*(i)));/*fps_factor*/
			REG_03_DEST_Y = (src->center.y256) + ((cos_value_t256*(i)));/*fps_factor*/
		}
		#endif
//
		//	REG_02_DEST_X;/* 発弾位置 座標x */
		//	REG_03_DEST_Y;/* 発弾位置 座標y */
		bakuhatsu_add_type_ccc(
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((1==mode)?(ra_nd()&((64-1)<<8)):(0)));	/* pspは0レジスタがあるので0と比較したほうが速い */
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0!=mode)?(ra_nd()&((64-1)<<8)):(0)));	/* pspは0レジスタがあるので0と比較したほうが速い */
			BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0==mode)?(0):(ra_nd()&((64-1)<<8))));	/* pspは0レジスタがあるので0と比較したほうが速い */
	}
}
