
#include "bullet_object.h"

/*---------------------------------------------------------
		"–Ñ‹Ê2",	"BADGUY",	(r27‚©‚çV‹K’Ç‰Á)
	-------------------------------------------------------
	–{‰Æ–Ñ‹Ê
---------------------------------------------------------*/

#define USE_X_HOUKOU (1)

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int time_out;
	int kougeki_type;
} KEDAMA1_DATA;


//	int t x256;
//	int t y256;
//	int v x256;
//	int v y256;
//	int speed256;

//static int enemy_rank;		/* Ý’èƒtƒ@ƒCƒ‹‚©‚ç‚Ì“G‚Ì‹­‚³ */

#define NUM_OF_ENEMIES (6)

/*---------------------------------------------------------
	“G‚â‚ç‚ê
---------------------------------------------------------*/

static void lose_kedama2(SPRITE *src)
{
//	case SP_ZAKO_08_KEDAMA1:
//	if (rand_percent(10))
	{
		item_create(src, enemy_get_random_item(), 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );
	}
}

/*---------------------------------------------------------
	“GˆÚ“®
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST02,
	ST03,
};

static void move_kedama2(SPRITE *src)
{
	KEDAMA1_DATA *data = (KEDAMA1_DATA *)src->data;

	data->time_out -= 1/**fps_fa ctor*/;
//

//
	switch (data->state)
	{
	case ST00:	/* ‚Î‚ç‚Î‚ç‚É“oê‚³‚¹‚é */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			/*‰æ–ÊŠO*/;
		}
		else
		{
			data->time_out	= 64;
			data->state++;	/* “oê‚·‚é */
		}
		break;
	case ST02:	/* UŒ‚ */
		{			enum
			{
				K00_KOUGEKI_KANKAKU_MASK = 0,
				K01_SUKIMA_KAKUDO,
				K02_TAMA_KAZU,
				K99_MAX/* Å‘å” */
			};
			const u32 kougeki_tbl[(4*2)][K99_MAX] =
			{
				/* 0:g‚Á‚Û‚¢–Ñ‹Ê(‹·Šp“x Šï”’e) */
				{0x3f, (u32)(512/12), (5/*+difficulty+difficulty*/)},	/* (u32)(512/12):LŠp‚È‚Ì‚ÅŠÈ’P */
				{0x1f, (u32)(512/16), (3/*+difficulty+difficulty*/)},	/* 0x0f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x0f, (u32)(512/20), (3/*+difficulty+difficulty*/)},	/* 0x0f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x07, (u32)(512/24), (5/*+difficulty+difficulty*/)},	/* (u32)(512/24):•’Ê‚Ì‹·Šp */
				/* 1:—d‚Á‚Û‚¢–Ñ‹Ê(90“x 4•ûŒü) (–³‘Ê’e‚È‚Ì‚Å“ïˆÕ“x‚Í’á‚¢”¤) */
				{0x3f, (u32)(512/8),  (8)}, 	/* 8:Œ©‚¹’e(–³‘Ê’e‚È‚Ì‚Å“ïˆÕ“x‚Í’á‚¢”¤) */
				{0x1f, (u32)(512/8),  (8)}, 	/* 0x1f:•’Ê‚É”ð‚¯‚ê‚éŠÔŠu */
				{0x07, (u32)(512/4),  (4)},
				{0x03, (u32)(512/4),  (4)}, 	/* 0x03:˜A‘±’e‚Á‚Û‚­ */
			};
			if (0==(data->time_out & kougeki_tbl[data->kougeki_type][K00_KOUGEKI_KANKAKU_MASK]))/*0x0f*/
			{
				bullet_create_n_way_dan_sa_type(src,
					(t256(2.5)+((difficulty)<<6)),/* [2.5 - 3.25] */		//	(t256(2.0)+((difficulty)<<6)),/* [2.0 - 2.75] */
					ANGLE_JIKI_NERAI_DAN,
					kougeki_tbl[data->kougeki_type][K01_SUKIMA_KAKUDO], 	//	(int)(512/24),
					BULLET_KOME_01_AOI+(src->x256&0x07),
					kougeki_tbl[data->kougeki_type][K02_TAMA_KAZU]);		//	(3/*+difficulty+difficulty*/)
			}
		}
		if (0 > data->time_out)
		{
			data->state++;
		}
		break;
	case ST03:
		/* ‰æ–ÊŠO‚Éo‚½‚çI‚í‚è */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			#endif
		)
		{
			src->type = SP_DELETE;	/* ‚¨‚µ‚Ü‚¢ */
		}
		break;
	}

	/* ˆÚ“®‚·‚é */
	src->x256 += (src->vx256);
	src->y256 += (src->vy256);
//
	src->m_angleCCW512 += 5;
	mask512(src->m_angleCCW512);
}

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_zako_kedama2(STAGE_DATA *l)/*int lv*/
{
	int i;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		SPRITE *s;
//		s						= sp rite_add_res(BASE_KEDAMA16_PNG);	//s->anim_speed=5; /*3*/ /*9"ba dguy.png"*/
		s						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed=5; /*3*/ /*9"ba dguy.png"*/
		s->type 				= /*SP_ZAKO*/TEKI_59_HAI_KEDAMA/*_08_KEDAMA1*/;
//		s->type 				= SP_ZAKO/*_08_KEDAMA1*/;
//		s->anim_speed			= /*-*/3; /*‹t“]ƒAƒjƒ‹ÖŽ~‚É•ÏX*/
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->color32 				= 0xaaffffff;		/*”’‚Á‚Û‚­ */
		s->callback_mover		= move_kedama2;
		s->callback_loser		= lose_kedama2;
		s->callback_hit_enemy	= callback_hit_zako;
//		s->an im_frame			= 0;
		//
		s->y256 				= ((l->user_y)<<8);
		s->vx256				= ((l->user_x));
		s->vy256				= (0);
		//
		{
			if (0 < (s->vx256))
			{/* [ˆø” user_y ‚ª³•ûŒü‚Ìê‡A‰E‚ÖˆÚ“®(¨)] [ˆø”y‚ª•‰•ûŒü‚Ìê‡A‰º‚ÖˆÚ“®(«)] */
				s->x256 			= t256(-30)-(i<<(5+8));
			}
			else
			{/* [ˆø” user_y ‚ª³•ûŒü‚Ìê‡A¶‚ÖˆÚ“®(©)] [ˆø”y‚ª•‰•ûŒü‚Ìê‡Aã‚ÖˆÚ“®(ª)] */
				s->x256 			= t256(360)+(i<<(5+8));/* 360 > 352(x_max) > 272(y_max) */
			}
		}
		//
		if (0 < (s->y256))
		{	;	}
		else
		{
			/* yÀ•W‚ª•‰•ûŒü‚Ìê‡‚ÍAxÀ•WŽw’è‚È‚Ì‚Å(xÀ•W yÀ•W ‚ð‹t‚É‚·‚é) */
			{
				int s_sss;
				s_sss				= s->x256;
				s->x256 			= -(s->y256);
				s->y256 			= s_sss;
			}
			s->vy256			= (s->vx256);
			s->vx256			= (0);
		}
		//
		KEDAMA1_DATA *data;
		data					= mmalloc(sizeof(KEDAMA1_DATA));
		s->data 				= data;
		/*data->base.*/s->base_score		= score(/*50*/5*2*4)/**(enemy_rank+1)*/;
		/*data->base.*/s->base_health		= (8/**8*/)/*+(difficulty<<2)*/ /*(1+(difficulty<<2))*/;/*‚â‚í‚ç‚©‚·‚¬*/
		data->state 			= ST00;
		data->time_out			= 64+(i<<4);
		data->kougeki_type		= (((l->user_1_moji)&1)<<2)|(difficulty/*&0x03*/);
	}
}
#undef NUM_OF_ENEMIES
//			data->anime_houkou	= 0x50;
//			data->anime_houkou	= 0x00;

	//	s->x256 				= t256(200)+	   (ra_nd()&((64*256)-1))/*ra_nd()%40*/;
	//	s->y256 				= ((ra_nd()&((32*256)-1))-t256(80));//	(ra_nd()%40-90);
	//	s->vx256			= (0);
	//	s->vy256			= (0);
//	/*data->*/enemy_rank 	= l->user_y;

//		data->anime_houkou	= 0x20;

//		data->tx256 			= player->x256;
//		data->ty256 			= player->y256;

