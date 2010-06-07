
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	爆発作成
---------------------------------------------------------*/

/*---------------------------------------------------------
	爆発エフェクトの表示
---------------------------------------------------------*/

static void move_bakuhatsu(SPRITE *src)
{
	src->base_time_out--;
	if (0 < src->base_time_out)
	{
		src->x256 += src->vx256;
		src->y256 += src->vy256;
	}
	else
	{
		src->type = SP_DELETE;
	}
}

/*---------------------------------------------------------
	種類別に爆発を追加する
---------------------------------------------------------*/
/*int x256, int y256*/
/*int delay_wait,*/
global void bakuhatsu_add_type_ddd(SPRITE *src, int type)
{
	{
		SPRITE *h;
		h					= sprite_add_gu_error();
		if (NULL!=h)/* 登録できた場合のみ */
		{
			h->m_Hit256R		= TAMA_ATARI_JIPPOU32_PNG;/*????*/
			h->base_time_out	= 30;/*30 フレーム*/

//			h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
			h->flags			&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定のOFF(無敵) */

			#if 1
			h->x256 			= (src->x256);
			h->y256 			= (src->y256);
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
				h->vx256	= (ra_nd()&0x07ff)-(0x03ff);
				h->vy256	= (ra_nd()&0x07ff)-(0x03ff);
			}
			else
			{
				h->vx256	= (0);
				h->vy256	= (0);
			}
			h->type 			= SP_MUTEKI;
			h->callback_mover	= move_bakuhatsu;
		}
	}
}


/*---------------------------------------------------------
	円状領域に広がる爆発
---------------------------------------------------------*/

global void bakuhatsu_add_circle(SPRITE *src, int mode)
{
	int i;
	int j;
	j=0;
	for (i=0; i</*64*/(64)/*25*/; i+=8)
	{
		j += (ra_nd()&(/*64*/512-1));
		obj_send1->x256 = src->x256+((sin1024(j))*i);
		obj_send1->y256 = src->y256+((cos1024(j))*i);
//
		bakuhatsu_add_type_ddd(obj_send1,
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((1==mode)?(ra_nd()&((64-1)<<8)):(0)));	/* pspは0レジスタがあるので0と比較したほうが速い */
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0!=mode)?(ra_nd()&((64-1)<<8)):(0)));	/* pspは0レジスタがあるので0と比較したほうが速い */
			BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0==mode)?(0):(ra_nd()&((64-1)<<8))));	/* pspは0レジスタがあるので0と比較したほうが速い */
	}
}
