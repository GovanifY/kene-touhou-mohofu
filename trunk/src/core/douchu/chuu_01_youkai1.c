
#include "bullet_object.h"

/*---------------------------------------------------------
		"—d‰ö1"(Â),(Žb’è“I)		"GFAIRY",
	-------------------------------------------------------
	·•ªŽ‚Ì—d‰öA‚±‚ê‚Í”pŽ~‚µ‚Ä "—d‰ö2" ‚É‹zŽû—\’èB
---------------------------------------------------------*/

//#define tx256		user_data00
//#define ty256		user_data01
#define time_out	user_data02
//#define speed256	user_data03
//#define state		user_data04

typedef struct
{
//	ENEMY_BASE base;
	int state1;
//	int time_out;			/* s“®wait */
	int repeat; 			/* ŒJ‚è•Ô‚µ‰ñ” */
	int start_limit_y256;	/* “oêyÀ•W */
	SPRITE *s2;
	int wait2;			/* UŒ‚wait */
} YOKAI1_DATA;


/*---------------------------------------------------------
	s = ƒ{ƒX–{‘Ì boss sprite
	t = ƒvƒŒƒCƒ„[‚Ì’e player's weapon
---------------------------------------------------------*/

static void callback_hit_youkai1(SPRITE *src/*“GŽ©‘Ì*/, SPRITE *tama /*Ž©’e*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	YOKAI1_DATA *data	= (YOKAI1_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_hp -= /*w->*/tama->base_weapon_strength;	/* UŒ‚‚µ‚Ä‘Ì—ÍŒ¸‚ç‚·(‹­‚³•ªˆø‚­) */
	if (0 >= /*data->base.*/src->base_hp)			/* ‚O‚©•‰’l‚È‚çA“|‚µ‚½B */
	{
		bullets_to_hosi();/* ’e‘S•”A¯ƒAƒCƒeƒ€‚É‚·‚é */
	//
		item_create(src, SP_ITEM_05_TENSU, 7, ITEM_MOVE_FLAG_06_RAND_XY);/*“_”‚ðo‚·*/ 	/* ‚¿‚ç‚Î‚é */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
		src->type				= SP_DELETE;
		data->s2->type			= SP_DELETE;
		/* ƒR[ƒ‹ƒoƒbƒN“o˜^ */
		src->callback_hit_enemy = NULL; 	/* ƒ_ƒ~[ƒR[ƒ‹ƒoƒbƒN“o˜^ */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void lose_youkai1(SPRITE *src)
//{
//
//}


/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS03,
	SS04,
};
static void move_youkai1(SPRITE *src)
{
	YOKAI1_DATA *data=(YOKAI1_DATA *)src->data;
	src->time_out--;
	switch (data->state1)
	{
	case SS00:	/* ã‚©‚ç“oê */
		data->s2->y256	+= t256(2.0);	/*fps_factor*/
		src->y256		+= t256(2.0);	/*fps_factor*/
		if (data->start_limit_y256 < src->y256) 	{	src->time_out=30;	data->state1 = SS03;	}
		break;
//---------
	case SS01:
		if (0 > src->time_out) 	{	src->time_out=30;	data->state1=SS03;	/*data->wait2=(8-1) (10-1);*/	}
		else
		{
			data->wait2--;
			if (0 > data->wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				data->wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*ƒeƒLƒg[*/
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				#if (0) // 1975405
				if (data->repeat&1)
				{
					send1_obj->BULLET_REGIST_speed256			=	(t256(3.25)+((difficulty)<<6));
					send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_04_MIZUIRO;
					send1_obj->BULLET_REGIST_n_way				=	(7);
				}
				else
				{
					send1_obj->BULLET_REGIST_speed256			=	(t256(3.0)+((difficulty)<<6));
					send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_03_MIDORI;
					send1_obj->BULLET_REGIST_n_way				=	(8);
				}
				#else // 1975357
				{	const int aaa = (data->repeat&1);
					send1_obj->BULLET_REGIST_speed256			=	(t256(3.0)+((difficulty)<<6))+(aaa<<(8-2));
					send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_03_MIDORI+(aaa);
					send1_obj->BULLET_REGIST_n_way				=	(7)+(aaa);
				}
				#endif
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
				bullet_regist_basic();
			}
		}
		break;
	case SS03:	/* ‚µ‚Î‚µ‘Ò‚Â */
		if (0 > src->time_out)
		{
			src->time_out=100/*150*/;
			/* ‹K’è‚ÌŒJ‚è•Ô‚µ‰ñ”‚±‚È‚µ‚½‚ç‘Þê */
			data->repeat--;
			if ((0) != data->repeat)	{	data->state1	= SS01; 		}/* ŒJ‚è•Ô‚µ */
			else						{	data->state1++;/* = SS04*/		}/* ã‚Ö‘Þê */
		}
		break;
//---------
	case SS04:	/* ã‚Ö‘Þê */
		data->s2->y256	-= t256(2.0);	/*fps_factor*/
		src->y256		-= t256(2.0);	/*fps_factor*/
		#if 0/*SDL(¶‹÷À•W)*/
		if ( -((src->h128+src->h128)) > src->y256)
		#else/*Gu(’†SÀ•W)*/
		if ( 0 > src->y256)
		#endif
		{
			src->type = SP_DELETE;
			data->s2->type = SP_DELETE;
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_54_CHOU1+((src->time_out&0x10)>>4);
	}
//
	data->s2->m_angleCCW512++;
	mask512(data->s2->m_angleCCW512);
}
//	src->an im_frame	= ((src->time_out&0x10)>>4);

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static SPRITE *create_usiro_no_mahojin(SPRITE *src) //–‚•ûwƒOƒ‰ƒtƒBƒbƒN¶¬
{
	SPRITE *s2; 		// –‚•ûwƒOƒ‰ƒtƒBƒbƒN‚ÌƒXƒvƒ‰ƒCƒg
	s2					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	s2->type			= TEKI_52_MAHOJIN;/*SP_MUTEKI*/
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s2->color32 		= 0xaaff0000;		/* Â‚Á‚Û‚­ */		/*	s2->alpha			= 0x00;*/
	s2->x256			= src->x256/*+((src->w128-s2->w128))*/ /*-t256((8.0))*/;
	s2->y256			= src->y256/*+((src->h128-s2->h128))*/ /*-t256((8.0))*/;
	return (s2);
}

void add_chuu_youkai1(STAGE_DATA *l)
{
/*

 64=1*16+48
 80=2*16+48
112=4*16+48
160=7*16+48

*/
	{
		SPRITE *s1;
		SPRITE *s2;
	//	s1->x256				= ((l->user_x)*t256(35.0))+t256(40.0);
	//	s1->x256				= ((l->user_x)*t256(32.0))+t256(48.0);
	//	s1->x256				= ((l->user_x)<<(8+5))+t256(48.0);
		send1_obj->x256 		= ((l->user_x)<<(8));
		send1_obj->y256 		= t256(-30.0);
		s1						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		s2						= create_usiro_no_mahojin(send1_obj);
//
		s1->type				= TEKI_54_CHOU1;
		s1->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s1->callback_mover		= move_youkai1;
	//	s1->callback_loser		= lose_youkai1;
		s1->callback_hit_enemy	= callback_hit_youkai1; 	/* ƒR[ƒ‹ƒoƒbƒN“o˜^ */
		s1->x256				= send1_obj->x256;
		s1->y256				= send1_obj->y256;
//
		/*data->base.*/s1->base_hp		= (8*160)+(1/*di fficulty*/<<(4+3-1));	/* easy‚Å‚à‘¶ÝŠ´‚ðˆóÛ‚Ã‚¯‚éˆ×‚É 200 ‚Í•K—v */ 	// 50+150*di fficulty;
		/*data->base.*/s1->base_score		= score(100)+score(100)*difficulty;
//
		YOKAI1_DATA *data;
		data								= mmalloc(sizeof(YOKAI1_DATA));
		s1->data							= data;
		data->state1						= SS00;
		data->repeat						= (2+2+1);
		data->start_limit_y256				= ((l->user_y)<<(8));/* t256(50.0) */
		data->s2							= s2;
		data->wait2 						= (10-1);/*0*/
	}
}
