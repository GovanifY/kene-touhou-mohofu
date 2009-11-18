
#include "bullet_object.h"

/*---------------------------------------------------------
		"Â—d¸2",		"SPLASH",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;			/* —d¸‚Ìó‘Ô */
	int enemy_rank; 	/* İ’èƒtƒ@ƒCƒ‹‚©‚ç‚Ì“G‚Ì‹­‚³ */
	int time_out;		/* ó‘Ô‘JˆÚ—p‚ÌŠÔØ‚ê */
	int ani_turn;		/* ƒAƒjƒ[ƒVƒ‡ƒ“•ûŒü(‰H‚Ìã‰º) */
	int nnn;			/* ’e‚ğŒ‚‚Â‰ñ” */
} AO_YOUSEI2_DATA;

/*---------------------------------------------------------
	“G‚â‚ç‚ê
---------------------------------------------------------*/

static void lose_ao_yousei2(SPRITE *src)
{
//	case SP_ZAKO_17_AO_YOUSEI2:
	item_create(src, ((SP_ITEM_06_TENSU&0xff)|SP_ITEM_00_P001), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
}

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/
enum
{
	STATE_00 = 0,
	STATE_01,
	STATE_02,
	STATE_03,
};

#define SSS00 ( 0<<2)/*‰E*/
#define SSS04 ( 4<<2)/*‰EÎ‚ß‰º*/
#define SSS08 ( 8<<2)/*‰º‚P*/
#define SSS12 (12<<2)/*‰º‚Q*/
#define SSS16 (16<<2)/*¶Î‚ß‰º*/
#define SSS20 (20<<2)/*¶*/
static void move_ao_yousei2(SPRITE *src)
{
	AO_YOUSEI2_DATA *data = (AO_YOUSEI2_DATA *)src->data;
	int p512;
//
	data->time_out--;	/* í‚ÉŠÔŒo‰ß */
	switch (data->state)
	{
	case STATE_00:	/* “oê */
		if (data->time_out < 0)
		{
			data->time_out = 10;
			data->state++;// = STATE_01;/*Ÿ‚Ö*/
		}
		else
		{	/* “oê‚Ì“®‚« */
				 if (data->enemy_rank<3)	{	src->x256 -= t256(2)/**fps_fa ctor*/;	}
			else if (data->enemy_rank<7)	{	src->y256 += t256(2)/**fps_fa ctor*/;	}
			else							{	src->x256 += t256(2)/**fps_fa ctor*/;	}
		}
		break;
	case STATE_01:	/* ­‚µ‘Ò‚Â */
		if (data->time_out < 0)
		{
			data->time_out = 50;
			data->state++;// = STATE_02;/*Ÿ‚Ö*/
		}
		else
		{
			p512=atan_512(player->y256-src->y256,player->x256-src->x256);
			mask512(p512);
		//	const Uint8 aa_offs[8] = { OF_16, OF_12, OF_08, OF_04, OF_00, OF_04, OF_08, OF_12 };
			const Uint8 aa_offs[8] = { SSS00, SSS04, SSS08, SSS16, SSS20, SSS16, SSS08, SSS04 };
			src->yx_anim_frame = (src->yx_anim_frame&(4-1))+aa_offs[(p512>>6)]; 		/* 64  32== 512/16 */
		}
		break;
	case STATE_02:	/* ’e‚ğŒ‚‚Â */
		bullet_create_offset_dan_type000(src, /*((1+difficulty)<<7)*/t256(2.5)/*t256(3)*/, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);
		bullet_create_offset_dan_type000(src, /*((1+difficulty)<<8)*/t256(3.5)/*t256(4)*/, ANGLE_JIKI_NERAI_DAN/*, t256(0), t256(0)*/, BULLET_HARI32_00_AOI);

		p512=atan_512(player->y256-src->y256+((player->h128)),player->x256-src->x256-((player->w128)));
		if (difficulty>0)
		{
		int k;
			for (k=0; k<4; k++)
			{
				/*bullet_create_maru8_frame*/bullet_create_offset_dan_type000(
					src,			/*	*/
					t256(2.5)+(k<<7),			/* ’e‘¬ */
					((p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/12)),			/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
					(BULLET_MARU8_00_AKA+(/*0*/7-k/*‚Æ‚è‚ ‚¦‚¸*/))	/* ’eƒOƒ‰ */
				);
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
					/*bullet_create_maru8_frame*/bullet_create_offset_dan_type000(
						src,			/*	*/
						t256(1.5)+(k<<7),			/* ’e‘¬ */
						(/**/(p512))+(1-((k&(2-1)/*%2*/)*2))*((512*1/24)),			/* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
						(BULLET_MARU8_00_AKA+(/*0*/3-k/*‚Æ‚è‚ ‚¦‚¸*/))	/* ’eƒOƒ‰ */
					);
/*(1+(((k+difficulty)>>1)))*/  /*(2+((k>>1)))*/
				}
			}
		}

		data->nnn--;
		if (data->nnn < 0)
		{
			/* ‘Şê€”õ */
				 if (data->enemy_rank<3)	{	src->yx_anim_frame=(src->yx_anim_frame&(4-1))+SSS00;}
			else if (data->enemy_rank<7)	{	src->yx_anim_frame=(src->yx_anim_frame&(4-1))+SSS08;}
			else							{	src->yx_anim_frame=(src->yx_anim_frame&(4-1))+SSS00;}
			data->state++;// = STATE_03;/*Ÿ‚Ö*/
			data->time_out = 50;
		}
		else
		{
			data->state--;// = STATE_01;/*–ß‚é*/
		}
		break;
	case STATE_03:	/* ‘Şê */
		if (data->time_out < 0)
		{
			src->type = SP_DELETE;	/* ‚¨‚µ‚Ü‚¢ */
		}
		else
		{	/* ‘Şê‚Ì“®‚« */
				 if (data->enemy_rank<3)	{	src->x256 += t256(2)/**fps_fa ctor*/;}
			else if (data->enemy_rank<7)	{	src->y256 -= t256(2)/**fps_fa ctor*/;}
			else							{	src->x256 -= t256(2)/**fps_fa ctor*/;}
		}
		break;
	}

	if (0==data->ani_turn)
	{
		src->yx_anim_frame++;
		if (3==(src->yx_anim_frame&(4-1)))
		{
			data->ani_turn=1;
		}
	}
	else
	{
		src->yx_anim_frame--;
		if (0==(src->yx_anim_frame&(4-1)))
		{
			data->ani_turn=0;
		}
	}
}

/*---------------------------------------------------------
	“G‚ğ’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_ao_yousei2(STAGE_DATA *l)/*int lv*/
{
	int enemy_rank; 	enemy_rank	= l->user_y;
//
	if ( (enemy_rank)>9) {enemy_rank=9;}
	SPRITE *s;
//	s						= sp rite_add_res(BASE_AO_YOUSEI24_PNG); //s->anim_speed=0;/*20"sp lash.png"*/
	s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	s->type 				= /*SP_ZAKO*/TEKI_16_YOUSEI11/*_17_AO_YOUSEI2*/;
	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s->callback_mover		= move_ao_yousei2;
	s->callback_loser		= lose_ao_yousei2;
	s->callback_hit_enemy	= callback_hit_zako;
	signed short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/GAME_WIDTH+20,	1,/*-s->w;*/		/*s->y=*/100,		/*s->yx_anim_frame=*/SSS20},	//‰E‰º
{/* case 1: 	s->x=*/GAME_WIDTH+40,	1,/*-s->w;*/		/*s->y=*/70,		/*s->yx_anim_frame=*/SSS20},	//‰E’†
{/* case 2: 	s->x=*/GAME_WIDTH+60,	1,/*-s->w;*/		/*s->y=*/40,		/*s->yx_anim_frame=*/SSS20},	//‰Eã
//
{/* case 3: 	s->x=*/300, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->yx_anim_frame=*/SSS08},	//ã‰E‰E
{/* case 4: 	s->x=*/220, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->yx_anim_frame=*/SSS08},	//ã‰E
{/* case 5: 	s->x=*/160, 			3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->yx_anim_frame=*/SSS08},	//ã¶
{/* case 6: 	s->x=*/ 80, 			3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->yx_anim_frame=*/SSS08},	//ã¶¶
//
{/* case 7: 	s->x=*/-20, 			0,/*		*/		/*s->y=*/40,		/*s->yx_anim_frame=*/SSS00},	//¶ã
{/* case 8: 	s->x=*/-40, 			0,/*		*/		/*s->y=*/70,		/*s->yx_anim_frame=*/SSS00},	//¶’†
{/* case 9: 	s->x=*/-60, 			0,/*		*/		/*s->y=*/100,		/*s->yx_anim_frame=*/SSS00},	//¶‰º
	};
	s->yx_anim_frame		=  spr_tbl[enemy_rank][3];
	s->y256 				= (spr_tbl[enemy_rank][2]<<8);
	s->x256 				= (spr_tbl[enemy_rank][0]<<8);
		 if (1==spr_tbl[enemy_rank][1]) {s->x256 -= ((s->w128+s->w128));}
	else if (3==spr_tbl[enemy_rank][1]) {s->x256 -= ((s->w128));}

	AO_YOUSEI2_DATA *data;
	data					= mmalloc(sizeof(AO_YOUSEI2_DATA));
	s->data 				= data;
	/*data->base.*/s->base_score		= score(50*2);
	/*data->base.*/s->base_health		= 20+(difficulty<<2);
	data->state 			= STATE_00;
	data->enemy_rank		= enemy_rank;
	data->time_out			= 40;
	data->ani_turn			= 0;
	data->nnn				= 3;	/* 3‰ñŒ‚‚Â */
}
