
#include "bullet_object.h"

/*---------------------------------------------------------
		"ê¬ódê∏2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE base;
	int state;
	int level;
	int wait;
	int ani_turn;
	int n;
} AO_YOUSEI2_DATA;

/*---------------------------------------------------------
	ìGÇ‚ÇÁÇÍ
---------------------------------------------------------*/

static void lose_ao_yousei2(SPRITE *s)
{
//	case SP_ZAKO_17_AO_YOUSEI2:
	item_create(s, ((SP_ITEM_06_TENSU&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	ìGà⁄ìÆ
---------------------------------------------------------*/
#define SSS00 ( 0<<2)/*âE*/
#define SSS04 ( 4<<2)/*âEéŒÇﬂâ∫*/
#define SSS08 ( 8<<2)/*â∫ÇP*/
#define SSS12 (12<<2)/*â∫ÇQ*/
#define SSS16 (16<<2)/*ç∂éŒÇﬂâ∫*/
#define SSS20 (20<<2)/*ç∂*/
static void move_ao_yousei2(SPRITE *s)
{
	AO_YOUSEI2_DATA *d = (AO_YOUSEI2_DATA *)s->data;
	int p512;

	switch (d->state)
	{
	case 0: 	//ìoèÍ
		if (d->wait<40)
		{
				 if (d->level<3)	{	s->x256 -= t256(2)/**fps_fa ctor*/; 	}
			else if (d->level<7)	{	s->y256 += t256(2)/**fps_fa ctor*/; 	}
			else					{	s->x256 += t256(2)/**fps_fa ctor*/; 	}
			d->wait++;
		}
		else
		{
			d->state=1;
			d->wait=10;
		}
		break;
	case 1: 	//wait
		if (d->wait<0)
		{	d->state=2;}
		else
		{
			p512=atan_512(player->y256-s->y256,player->x256-s->x256);
			mask512(p512);
		//	const Uint8 aa_offs[8] = { OF_16, OF_12, OF_08, OF_04, OF_00, OF_04, OF_08, OF_12 };
			const Uint8 aa_offs[8] = { SSS00, SSS04, SSS08, SSS16, SSS20, SSS16, SSS08, SSS04 };
			s->yx_anim_frame = (s->yx_anim_frame&(4-1))+aa_offs[(p512>>6)];			/* 64  32== 512/16 */
			d->wait--;
		}
		break;
	case 2: 	//shot
		d->n++;
		d->wait=50;
		d->state=1;
		bullet_create_offset_dan_type(s, /*((1+difficulty)<<7)*/t256(2.5)/*t256(3)*/, ANGLE_JIKINERAI_DAN, t256(0), t256(0), BULLET_HARI32_00_AOI);
		bullet_create_offset_dan_type(s, /*((1+difficulty)<<8)*/t256(3.5)/*t256(4)*/, ANGLE_JIKINERAI_DAN, t256(0), t256(0), BULLET_HARI32_00_AOI);

		p512=atan_512(player->y256-s->y256+((player->h128)),player->x256-s->x256-((player->w128)));
		if (difficulty>0)
		{
		int k;
			for (k=0; k<4; k++)
			{
				bullet_create_maru8_frame(s, t256(2.5)+(k<<7), ((p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/12)), (/*0*/7-k/*Ç∆ÇËÇ†Ç¶Ç∏*/) );
/*(2+(((k+difficulty)>>1)))*/  /*(3+((k>>1)))*/

/*
  3+((k>>1))
0 4 3
1 4 4
2 4 4
3 4 5
*/
				if (difficulty > 1)
				{
					bullet_create_maru8_frame(s, t256(1.5)+(k<<7), (/**/(p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/24)), (/*0*/3-k/*Ç∆ÇËÇ†Ç¶Ç∏*/) );
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
				}
			}
		}
		if (d->n>3)
		{	d->state=3;}
		break;
	case 3: 	//ëﬁèÍèÄîı
			 if (d->level<3)	{	s->yx_anim_frame=(s->yx_anim_frame&(4-1))+SSS00;}
		else if (d->level<7)	{	s->yx_anim_frame=(s->yx_anim_frame&(4-1))+SSS08;}
		else					{	s->yx_anim_frame=(s->yx_anim_frame&(4-1))+SSS00;}
		d->state=4;
		d->wait=0;
		break;
	case 4: 	//ëﬁèÍ
		if (d->wait>50)
		{
			s->type=SP_DELETE;
		}
		else
		{
				 if (d->level<3)	{	s->x256 += t256(2)/**fps_fa ctor*/;}
			else if (d->level<7)	{	s->y256 -= t256(2)/**fps_fa ctor*/;}
			else					{	s->x256 -= t256(2)/**fps_fa ctor*/;}
		}
		break;
	}

	if (0==d->ani_turn)
	{
		s->yx_anim_frame++;
		if (3==(s->yx_anim_frame&(4-1)))
		{
			d->ani_turn=1;
		}
	}
	else
	{
		s->yx_anim_frame--;
		if (0==(s->yx_anim_frame&(4-1)))
		{
			d->ani_turn=0;
		}
	}
}

/*---------------------------------------------------------
	ìGÇí«â¡Ç∑ÇÈ
---------------------------------------------------------*/

void add_zako_ao_yousei2(STAGE_DATA *l)/*int lv*/
{
	int lv;
	lv	= l->user_y;
//
	if ( (lv)>9) {lv=9;}
	SPRITE *s;
	s					= sprite_add_res(BASE_AO_YOUSEI24_PNG);	//s->anim_speed=0;/*20"sp lash.png"*/
	s->type 			= SP_ZAKO/*_17_AO_YOUSEI2*/;
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover	= move_ao_yousei2;
	s->callback_loser	= lose_ao_yousei2;
	signed short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/GAME_WIDTH+20,	1,/*-s->w;*/		/*s->y=*/100,		/*s->yx_anim_frame=*/SSS20},	//âEâ∫
{/* case 1: 	s->x=*/GAME_WIDTH+40,	1,/*-s->w;*/		/*s->y=*/70,		/*s->yx_anim_frame=*/SSS20},	//âEíÜ
{/* case 2: 	s->x=*/GAME_WIDTH+60,	1,/*-s->w;*/		/*s->y=*/40,		/*s->yx_anim_frame=*/SSS20},	//âEè„
{/* case 3: 	s->x=*/300, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->yx_anim_frame=*/SSS08},	//è„âEâE
{/* case 4: 	s->x=*/220, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->yx_anim_frame=*/SSS08},	//è„âE
{/* case 5: 	s->x=*/160, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->yx_anim_frame=*/SSS08},	//è„ç∂
{/* case 6: 	s->x=*/ 80, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->yx_anim_frame=*/SSS08},	//è„ç∂ç∂
{/* case 7: 	s->x=*/-20, 			0,/*		*/		/*s->y=*/40,		/*s->yx_anim_frame=*/SSS00},	//ç∂è„
{/* case 8: 	s->x=*/-40, 			0,/*		*/		/*s->y=*/70,		/*s->yx_anim_frame=*/SSS00},	//ç∂íÜ
{/* case 9: 	s->x=*/-60, 			0,/*		*/		/*s->y=*/100,		/*s->yx_anim_frame=*/SSS00},	//ç∂â∫
	};
	s->yx_anim_frame	=  spr_tbl[lv][3];
	s->y256 			= (spr_tbl[lv][2]<<8);
	s->x256 			= (spr_tbl[lv][0]<<8);
		 if (1==spr_tbl[lv][1]) {s->x256 -= ((s->w128+s->w128));}
	else if (3==spr_tbl[lv][1]) {s->x256 -= ((s->w128));}

	AO_YOUSEI2_DATA *data;
	data				= mmalloc(sizeof(AO_YOUSEI2_DATA));
	s->data 			= data;
	data->base.score	= score(50*2);
	data->base.health	= 20+(difficulty<<2);
	data->state 		= 0;
	data->level 		= lv;
	data->wait			= 0;
	data->ani_turn		= 0;
	data->n 			= 0;
}
