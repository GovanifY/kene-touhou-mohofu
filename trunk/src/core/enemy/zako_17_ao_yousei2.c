
#include "enemy.h"

/*---------------------------------------------------------
		"Â—d¸2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	ENEMY_BASE b;
	int state;
	int level;
	int wait;
	int ani_turn;
	int n;
} AO_YOUSEI2_DATA;

/*---------------------------------------------------------
	“G‚â‚ç‚ê
---------------------------------------------------------*/

static void lose_ao_yousei2(SPRITE *s)
{
//	case SP_ZAKO_17_AO_YOUSEI2:
	item_create(s, ((SP_ITEM_06_TENSU&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/
#define SSS00 ( 0)/*‰E*/
#define SSS04 ( 4)/*‰EŽÎ‚ß‰º*/
#define SSS08 ( 8)/*‰º‚P*/
#define SSS12 (12)/*‰º‚Q*/
#define SSS16 (16)/*¶ŽÎ‚ß‰º*/
#define SSS20 (20)/*¶*/
static void move_ao_yousei2(SPRITE *s)
{
	AO_YOUSEI2_DATA *d=(AO_YOUSEI2_DATA *)s->data;
	/*dou ble*/int p512;

	switch (d->state)
	{
	case 0: 	//“oê
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
			s->anim_frame= (s->anim_frame&(4-1))+aa_offs[(p512>>6)];			/* 64  32== 512/16 */
			d->wait--;
		}
		break;
	case 2: 	//shot
		d->n++;
		d->wait=50;
		d->state=1;
		bullet_create_hari_dan180(s, /*((1+difficulty)<<7)*/t256(2.5)/*t256(3)*/, ANGLE_JIKINERAI_DAN, t256(0), t256(0));
		bullet_create_hari_dan180(s, /*((1+difficulty)<<8)*/t256(3.5)/*t256(4)*/, ANGLE_JIKINERAI_DAN, t256(0), t256(0));

		p512=atan_512(player->y256-s->y256+((player->h128)),player->x256-s->x256-((player->w128)));
		if (difficulty>0)
		{
		int k;
			for (k=0; k<4; k++)
			{
				bullet_create_maru8_frame(s, t256(2.5)+(k<<7), ((p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/12)), (/*0*/7-k/*‚Æ‚è‚ ‚¦‚¸*/) );
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
					bullet_create_maru8_frame(s, t256(1.5)+(k<<7), (/**/(p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/24)), (/*0*/3-k/*‚Æ‚è‚ ‚¦‚¸*/) );
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
				}
			}
		}
		if (d->n>3)
		{	d->state=3;}
		break;
	case 3: 	//‘Þê€”õ
			 if (d->level<3)	{	s->anim_frame=(s->anim_frame&(4-1))+SSS00;}
		else if (d->level<7)	{	s->anim_frame=(s->anim_frame&(4-1))+SSS08;}
		else					{	s->anim_frame=(s->anim_frame&(4-1))+SSS00;}
		d->state=4;
		d->wait=0;
		break;
	case 4: 	//‘Þê
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
		s->anim_frame++;
		if ((s->anim_frame&(4-1))==3)
		{
			d->ani_turn=1;
		}
	}
	else
	{
		s->anim_frame--;
		if ((s->anim_frame&(4-1))==0)
		{
			d->ani_turn=0;
		}
	}
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
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
	short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/GAME_WIDTH+20,	1,/*-s->w;*/		/*s->y=*/100,		/*s->anim_frame=*/SSS20},	//‰E‰º
{/* case 1: 	s->x=*/GAME_WIDTH+40,	1,/*-s->w;*/		/*s->y=*/70,		/*s->anim_frame=*/SSS20},	//‰E’†
{/* case 2: 	s->x=*/GAME_WIDTH+60,	1,/*-s->w;*/		/*s->y=*/40,		/*s->anim_frame=*/SSS20},	//‰Eã
{/* case 3: 	s->x=*/300, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->anim_frame=*/SSS08},	//ã‰E‰E
{/* case 4: 	s->x=*/220, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->anim_frame=*/SSS08},	//ã‰E
{/* case 5: 	s->x=*/160, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->anim_frame=*/SSS08},	//ã¶
{/* case 6: 	s->x=*/ 80, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->anim_frame=*/SSS08},	//ã¶¶
{/* case 7: 	s->x=*/-20, 			0,/*		*/		/*s->y=*/40,		/*s->anim_frame=*/SSS00},	//¶ã
{/* case 8: 	s->x=*/-40, 			0,/*		*/		/*s->y=*/70,		/*s->anim_frame=*/SSS00},	//¶’†
{/* case 9: 	s->x=*/-60, 			0,/*		*/		/*s->y=*/100,		/*s->anim_frame=*/SSS00},	//¶‰º
	};
	s->anim_frame	=  spr_tbl[lv][3];
	s->y256 		= (spr_tbl[lv][2]<<8);
	s->x256 		= (spr_tbl[lv][0]<<8);
		 if (1==spr_tbl[lv][1]) {s->x256 -= ((s->w128+s->w128));}
	else if (3==spr_tbl[lv][1]) {s->x256 -= ((s->w128));}

	AO_YOUSEI2_DATA *data;
	data			= mmalloc(sizeof(AO_YOUSEI2_DATA));
	s->data 		= data;
	data->b.score	= score(50*2);
	data->b.health	= 20+(difficulty<<2);
	data->state 	= 0;
	data->level 	= lv;
	data->wait		= 0;
	data->ani_turn	= 0;
	data->n 		= 0;
}
