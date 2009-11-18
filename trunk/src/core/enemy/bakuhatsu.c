
/*---------------------------------------------------------
	爆発作成
	-------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

//typedef struct
//{
//	int anime_count;			/* アニメーション速度のカウンタ */
//	int anime_speed;			/* アニメーション速度の設定速度 */
//	int wait_count1;			/* 始まるまでの待ち時間 */
//} BAKUHATSU_DATA;

/*---------------------------------------------------------
	爆発エフェクトの表示
---------------------------------------------------------*/

//	BAKUHATSU_DATA *data=(BAKUHATSU_DATA *)src->data;
//	if (0 < data->wait_count1)
//	{	data->wait_count1 -= 1/*fps_fa ctor*/;}
//	else
//	{
//		src->flags			|= (SP_FLAG_VISIBLE);/* 表示 on */
//		data->anime_count -= 1/*fps_fa ctor*/;
//		if (data->anime_count < 1)
//		{
//			data->anime_count = data->anime_speed;
//			src->anim_frame++;
//			if (src->anim_frame > (src->yx_frames&YX_FRAME_LOW_X) )
//			{
//				src->type = SP_DELETE;
//			}
//		}
//		src->x256 += src->vx256;
//		src->y256 += src->vy256;
//	}

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

void bakuhatsu_add_type_ddd(SPRITE *src/*int x256, int y256*/, /*int delay_wait,*/ int type)
{
//	if (BAKUHATSU_MINI00==type) {	type = (ra_nd()%3+1);	}/* あちこちにあると遅い */
	if (0==(type&0xfb))
	{
		type |= (ra_nd()&0x03); if (0==(type & 0x03)) { 	type |= (0x01); }
	}
	{
		SPRITE *s /*= NULL*/;
	//	s					= sp rite_add_res(BASE_TR_BLUE_PNG);
		s					= sprite_add_gu(TAMA_TYPE_BULLET_JIPPOU32_PNG);
		s->base_time_out 	= 30;/*30 フレーム*/

		s->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
		s->flags			&= (~(SP_FLAG_COLISION_CHECK)); 	/* あたり判定のOFF(無敵) */

		s->x256 			= /*x256*/(src->x256)-(s->w128);
		s->y256 			= /*y256*/(src->y256)-(s->h128);
		s->color32			= 0x66ffffff;	/*	s->alpha			= 0x80;*/
		if (12==(type & 0xff))
		{
			s->vx256	= (ra_nd()&0x07ff)-(0x03ff);
			s->vy256	= (ra_nd()&0x07ff)-(0x03ff);
		}
		else
		{
			s->vx256	= (0);
			s->vy256	= (0);
		}
		s->type 			= SP_MUTEKI;
		s->callback_mover	= move_bakuhatsu;
	}
}
//
//	ex->data			= data;
//	data->anime_count	= 0;
//	data->wait_count1	= delay_wait;
//	data->wait_count1	= ((type>>8) & 0xff);

//	ex->anim_speed		= 0;
//	ex->anim_frame		= 0/*-1 ???*/;

//	BAKUHATSU_DATA *data;
//	data = mmalloc(sizeof(BAKUHATSU_DATA));
//	ex->flags			&= (~(SP_FLAG_VISIBLE));/* 表示一時off */

	#if 0
	switch (type & 0xff)
	{
//	case 0: break;/* 小爆発 */
//	case 1: 	ex = spr ite_add_file 0("tr_blue.png",	6, PRIORITY_04_ITEM, 0);	data->anime_speed=1;	break;
//	case 2: 	ex = spr ite_add_file 0("tr_red.png",	6, PRIORITY_04_ITEM, 0);	data->anime_speed=1;	break;
//	case 3: 	ex = spr ite_add_file 0("tr_green.png", 6, PRIORITY_04_ITEM, 0);	data->anime_speed=1;	break;
	case 1: 	ex = sp rite_add_res(BASE_TR_BLUE_PNG);	/*	data->anime_speed=1;*/	break;
	case 2: 	ex = sp rite_add_res(BASE_TR_RED_PNG);	/*	data->anime_speed=1;*/	break;
	case 3: 	ex = sp rite_add_res(BASE_TR_GREEN_PNG); /*	data->anime_speed=1;*/	break;
//
//	case 4: break;/* ザコ消滅爆発 */
//	case 5: 	ex = spr ite_add_file 0("bakuha05.png", 5, PRIORITY_04_ITEM, 1);	data->anime_speed=/*8*/(3)/*1*/; ex->alpha = 150;	break;
//	case 6: 	ex = spr ite_add_file 0("bakuha06.png", 5, PRIORITY_04_ITEM, 1);	data->anime_speed=/*8*/(3)/*1*/; ex->alpha = 150;	break;
//	case 7: 	ex = spr ite_add_file 0("bakuha07.png", 5, PRIORITY_04_ITEM, 1);	data->anime_speed=/*8*/(3)/*1*/; ex->alpha = 150;	break;
	case 5: 	ex = sp rite_add_res(BASE_BAKUHA05_PNG); /*	data->anime_speed=/ *8* /(3)/ *1* /;*/	ex->alpha = 150;	break;
	case 6: 	ex = sp rite_add_res(BASE_BAKUHA06_PNG); /*	data->anime_speed=/ *8* /(3)/ *1* /;*/	ex->alpha = 150;	break;
	case 7: 	ex = sp rite_add_res(BASE_BAKUHA07_PNG); /*	data->anime_speed=/ *8* /(3)/ *1* /;*/	ex->alpha = 150;	break;
//		/* 火炎爆発 */
//	case 8: 	ex = spr ite_add_file 0("ex.png", 29, PRIORITY_04_ITEM, 0); 	data->anime_speed=3/*2*/;	break;
	case 8: 	ex = sp rite_add_res(BASE_BAKUHA_EX_PNG);/*	data->anime_speed=3/ *2* /;*/	break;
//		/*移動爆発*/
	case 12: 	ex = sp rite_add_res(BASE_TR_RED_PNG);	/*	data->anime_speed=1;*/	break;
	default:	error(ERR_WARN, "bakuhatsu_add: unknown type: %d",type);
	}
	#else
	#endif
/*---------------------------------------------------------

	-------------------------------------------------------

---------------------------------------------------------*/

/*static*/ void bakuhatsu_add_zako04(SPRITE *src)
{
	#if 1
	dummy_obj->x256 = src->x256;
	dummy_obj->y256 = src->y256;
	#else
	dummy_obj->x256 = src->x256+((s->w128));
	dummy_obj->y256 = src->y256+((s->h128));/*+t256(5)*/
	#endif
	bakuhatsu_add_type_ddd(dummy_obj/*s->x256,s->y256+t256(5)*/,/*0,*/BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/*ザコ消滅爆発*/
//	bakuhatsu_add_type_ddd(dummy_obj/*s->x256+((s->w128)),s->y256+((s->h128))*/,/*0,*/BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/);
	//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
}

/*---------------------------------------------------------
	円状領域に広がる爆発
---------------------------------------------------------*/

void bakuhatsu_add_circle(SPRITE *src, int mode)
{
	int i;
	int j;
	j=0;
	for (i=0; i</*64*/64/*25*/; i+=8)
	{
		j += (ra_nd()&(/*32*/256-1));
		dummy_obj->x256 = src->x256+((sin512(j))*i);
		dummy_obj->y256 = src->y256+((cos512(j))*i);
//2009469
		bakuhatsu_add_type_ddd(dummy_obj,
		//	src->x256+((cos512(j))*i),
		//	src->y256+((sin512(j))*i),
//			src->x256+((sin512(j))*i),
//			src->y256+((cos512(j))*i),
		//	ra_nd()&(64-1)/*%40*/,
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((1==mode)?(ra_nd()&((64-1)<<8)):(0)));	/* pspは0レジスタがあるので0と比較したほうが速い */
		//	BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0!=mode)?(ra_nd()&((64-1)<<8)):(0)));	/* pspは0レジスタがあるので0と比較したほうが速い */
			BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/ | ((0==mode)?(0):(ra_nd()&((64-1)<<8))));	/* pspは0レジスタがあるので0と比較したほうが速い */
	}
}

/*---------------------------------------------------------
	矩形領域の爆発(KETM)
---------------------------------------------------------*/
#if 0
void bakuhatsu_add_rect(SPRITE *src)
{
	int i;
	int j;
	for (i=0; i<100; i+=30)
	{
		for (j=0; j<100; j+=30)
		{
			bakuhatsu_add_type(src->x256+i*256,src->y256+j*256,/*(ra_nd()&(32-1)/ *%30* /),*/BAKUHATSU_ZAKO04/*BAKUHATSU_FIRE08*/);
		}
	}
//	for (i=0; i<192; i+=40)
//	{
//		for (j=0; j<192; j+=40)
//		{
//			bakuhatsu_add_type(src->x256+i*256,src->y256+j*256,/ *ra_nd()%40,* /BAKUHATSU_FIRE08);
//		}
//	}
}
#endif
