
#include "bullet_object.h"

/*---------------------------------------------------------
		"ê¬ódê∏1",		"FAIRY",
	-------------------------------------------------------
	user_x: 	ë¨ìx256[speed256]
	user_y: 	â°à íu[dots]
---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int angleCCW512;	/* â∫Ç™ÇOÇ∆ÇµÇƒç∂âÒÇË(ãtâÒÇË)ÇÃäpìx */
	int speed256;
	int state;
	int time_out;
} AO_YOUSEI1_DATA;


/*---------------------------------------------------------
	ìGÇ‚ÇÁÇÍ
---------------------------------------------------------*/

static void lose_ao_yousei1(SPRITE *src)
{
//	case SP_ZAKO_16_AO_YOUSEI1:
	//	if (rand_percent(30))	{	put_item=(c->x, c->y, SP_ITEM_00_P001&0xff);}
	//item_create(src, (((ra_nd()&0x07)&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	item_create(src, (enemy_get_random_item()), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	ìGà⁄ìÆ
---------------------------------------------------------*/

static void move_ao_yousei1(SPRITE *src)
{
	AO_YOUSEI1_DATA *data = (AO_YOUSEI1_DATA *)src->data;
//
	data->time_out--;
//	if (1 > data->time_out)
//	{
//		src->type = SP_DELETE;	/* Ç®ÇµÇ‹Ç¢ */
//	}
	switch (data->state)
	{
	case 0: 	/* ìoèÍ çUåÇ */
		data->speed256--;
		if (1 > data->speed256)
		{
			data->speed256 = 1;
		}
		switch (data->time_out)
		{
		case 30:
			data->state++;
			break;
		case 50:
			if (difficulty)
			{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* Ç†Ç∆Ç≈óvÇÈ */
	//		send1_obj->h128 = src->h128;
	//		send1_obj->w128 = src->w128;
			#endif
			//	bullet_create_n_way_dan_sa_type(src,
				send1_obj->BULLET_REGIST_speed256			=		(t256(2.0)/*+((difficulty)<<6)*/);				/* íeë¨ */
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/24)-(difficulty); 							/* äpìx */
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_KUNAI12_04_YUKARI+((ra_nd())&7); 	/* [íeÉOÉâ] */
				send1_obj->BULLET_REGIST_n_way				=		(8) ;/* 8wayíe */
				bullet_regist_basic();
			}	/*Ç»ÇÈÇ◊Ç≠ã§í âª*/
			break;	/*(t256(3.0)+((difficulty)<<6))*/	/*((difficulty<<(1+8)))*/
					/*t256((difficulty<<2))*/ /*5*/ 	/*"kugel.png", 0*/
					/*at an2(player->y-src->y+player->h/2,player->x-src->x-player->w/2)*/
		case 70:	/*not_break;*/
		case 120:	/*not_break;*/
		case 170:	/*not_break;*/
		case 220:
			if (difficulty)
			{
			send1_obj->x256 = src->x256;
			send1_obj->y256 = src->y256;
			#if 1
			/* Ç†Ç∆Ç≈óvÇÈ */
	//		send1_obj->h128 = src->h128;
	//		send1_obj->w128 = src->w128;
			#endif
			//	bullet_create_n_way_dan_sa_type(src,
				//	(512-220/*data->time_out*/)+t256(difficulty),	/* íeë¨ */
				send1_obj->BULLET_REGIST_speed256			=		(t256(2.0));									/* íeë¨ */
				send1_obj->BULLET_REGIST_angle512			=		ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=		(int)(512/24)-(difficulty); 					/* äpìx */
				send1_obj->BULLET_REGIST_bullet_obj_type	=		BULLET_UROKO14_00_AOI;	//	BULLET_MARU8_00_AKA,	/* [ê‘íe] */
				send1_obj->BULLET_REGIST_n_way				=		(difficulty/*1*/) ; 					/* [1way] */
				bullet_regist_basic();
			}
			break;
		case 250:/* ì|ÇπÇÈÇÊÇ§Ç…Ç∑ÇÈ */
			/*data->base.*/src->base_health = (1+(difficulty)); 	/*(3+(difficulty))*/	/*1+(difficulty<<2)*/
			break;
		}
		break;
	case 1: 	/* ó£íE */
		data->speed256 += 2;
		data->angleCCW512 += 2;
		if (255 < data->angleCCW512)
		{
			data->angleCCW512 = 255;/*ê^è„*/
		}
		if (1 > src->y256)
		{
			src->type = SP_DELETE;	/* Ç®ÇµÇ‹Ç¢ */
		}
		break;
	}
	/*à»â∫rwingx.cÇ∆ìØÇ∂*/
/* CCWÇÃèÍçá */
	src->x256+=((sin512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;
	src->y256+=((cos512((data->angleCCW512))*data->speed256)>>8)/**fps_fa ctor*/;

//	src->yx_an im_frame = (8+((data->time_out>>2)&(8-1)));
//	src->yx_an im_frame = (0x20 | ((data->time_out)&(0x10)) | ((data->time_out>>2)&(4-1)));
/*
data->time_out		 ---a bc--
src->yx_an im_frame  yyyy xxxx
src->yx_an im_frame  --1a --bc
*/
	if (SP_DELETE != src->type)
	{
		src->type				= (TEKI_16_YOUSEI11)+(0x08)+((data->time_out>>2)&(8-1));
	}
}

/*---------------------------------------------------------
	ìGÇí«â¡Ç∑ÇÈ
---------------------------------------------------------*/

void add_zako_ao_yousei1(STAGE_DATA *l)/*int lv*/
{
	SPRITE *s;
//	s						= sp rite_add_res(BASE_AO_YOUSEI24_PNG);/*7"fairy.png"*/
	s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	s->type 				= /*SP_ZAKO*/TEKI_16_YOUSEI11/*_16_AO_YOUSEI1*/;
	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover		= move_ao_yousei1;
	s->callback_loser		= lose_ao_yousei1;
	s->callback_hit_enemy	= callback_hit_zako;
//	s->anim_speed			= 0/*4*/;
	AO_YOUSEI1_DATA *data;
	data					= mmalloc(sizeof(AO_YOUSEI1_DATA));
	s->data 				= data;
	s->y256 				= t256(-30);
	s->x256 				= ((l->user_y)<<8);/*lv*t256(35)+t256(40)*/
	data->time_out			= 320;
	data->speed256			= ((l->user_x));			/*t256(0.5)*/	/*+0.15*difficulty*/ /*0.7+0.3*difficulty*/
#if 0
/* CWÇÃèÍçá */
	data->angleCCW512		= deg_360_to_512(0);/*ê^â∫*/
#else
/* CCWÇÃèÍçá */
	data->angleCCW512		= deg_360_to_512CCW(360-0);/*ê^â∫*/
#endif
	data->state 			= 0;
	/*data->base.*/s->base_score		= score(20*2);
	/*data->base.*/s->base_health		= 999;/* ìoèÍéûÇÕì|ÇπÇ»Ç¢ */	/*3+(difficulty)*/	/*1+(difficulty<<2)*/
}
