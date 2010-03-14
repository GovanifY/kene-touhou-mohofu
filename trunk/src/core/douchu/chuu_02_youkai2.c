
#include "bullet_object.h"

/*---------------------------------------------------------
		"—d‰ö2"(Ô),		"GFAIRY",
	-------------------------------------------------------

---------------------------------------------------------*/

//#define tx256		user_data00
//#define ty256		user_data01
#define time_out	user_data02
//#define speed256	user_data03
//#define state		user_data04

typedef struct
{
//	ENEMY_BASE base;
//	BO SS_BASE boss_base;
//------------ ˆÚ“®ŠÖ˜A
//	int st ate1;
//	int time_out;			/* s“®wait */
	int repeat; 			/* ŒJ‚è•Ô‚µ‰ñ” */
	int start_limit_y256;	/* “oêyÀ•W */
	SPRITE *s2;
	int start_danmaku;
} YOKAI1_DATA;
//	int wait2;	/* UŒ‚wait */


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
	’e–‹‚ªI‚í‚é‚Ü‚Å‘Ò‚ÂB
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)
{
//	YOKAI1_DATA *data = (YOKAI1_DATA *)src->data;
	if (DANMAKU_00 == src->boss_base_danmaku_type)
	{
		src->boss_base_state001++;/* = nextstate*/
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};
//	ST02,
//	ST02bbb,
//	ST03,

#if 0
			;/* 8‹ô”’e */
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
				send1_obj->BULLET_REGIST_speed256			=	(t256(3.0)+((difficulty)<<6));
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_03_MIDORI;
				send1_obj->BULLET_REGIST_n_way				=	(8);
				bullet_regist_basic();
			}
#endif

#if 0
			;/* 7Šï”’e */
			data->wait2--;
			if (0 > data->wait2)
			{
				data->wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*ƒeƒLƒg[*/
				send1_obj->x256 = src->x256;
				send1_obj->y256 = src->y256;
				send1_obj->BULLET_REGIST_speed256			=	t256(3.25)+((difficulty)<<6);
				send1_obj->BULLET_REGIST_angle512			=	ANGLE_JIKI_NERAI_DAN;
				send1_obj->BULLET_REGIST_div_angle512		=	(int)(512/24);
				send1_obj->BULLET_REGIST_bullet_obj_type	=	BULLET_UROKO14_04_MIZUIRO;
				send1_obj->BULLET_REGIST_n_way				=	(7);
				bullet_regist_basic();
			}
#endif
static void move_youkai1(SPRITE *src)
{
	YOKAI1_DATA *data=(YOKAI1_DATA *)src->data;
	src->time_out--;
	switch (src->boss_base_state001)
	{
	case SS00:	/* ã‚©‚ç“oê */
		data->s2->y256	+= t256(2.0);	/*fps_factor*/
		src->y256		+= t256(2.0);	/*fps_factor*/
		if (data->start_limit_y256 < src->y256) 	{	src->time_out=30;	src->boss_base_state001 = SS03;	}
		break;
//---------
	case SS01:	/* ’e–‹ƒZƒbƒg */
			/*data->wait2 = (8-1) (10-1)*/
			src->boss_base_state001++;/* = SS02;*/
			;/* 8‹ô”’e */
			;/* 7Šï”’e */
			src->boss_base_danmaku_type		= data->start_danmaku/*DANMAKU_01*/;			/* âS’e–‹‚ðƒZƒbƒg */
			src->boss_base_danmaku_time_out	= DANMAKU_01_SET_TIME;	/* âS’e–‹‚Ì”­¶ŽžŠÔ */
		break;
	case SS02:	/* âS’e–‹‚ªI‚í‚é‚Ü‚Å‘Ò‹@ */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* ‚µ‚Î‚µ‘Ò‚Â */
		if (0 > src->time_out)
		{
			src->time_out=100/*150*/;
			/* ‹K’è‚ÌŒJ‚è•Ô‚µ‰ñ”‚±‚È‚µ‚½‚ç‘Þê */
			data->repeat--;
			if ((0) != data->repeat)	{	src->boss_base_state001	= SS01; 		}/* ŒJ‚è•Ô‚µ */
			else						{	src->boss_base_state001++;/* = SS04*/		}/* ã‚Ö‘Þê */
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
//
	danmaku_generator(src); /* ’e–‹¶¬ */
}
//	src->an im_frame	= ((src->time_out&0x10)>>4);

/*---------------------------------------------------------
	“G‚ð’Ç‰Á‚·‚é
---------------------------------------------------------*/

static SPRITE *create_usiro_no_mahojin(SPRITE *src) 	/* –‚•ûwƒOƒ‰ƒtƒBƒbƒN¶¬ */
{
	SPRITE *s2; 		// –‚•ûwƒOƒ‰ƒtƒBƒbƒN‚ÌƒXƒvƒ‰ƒCƒg
	s2					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
	s2->type			= TEKI_52_MAHOJIN;/*S P_M UTEKI*/
//	s2->type			= MAHOU_JIN_00_aaa/*S P_M UTEKI*/;
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	s2->color32 		= 0xaa0000ff;		/* Ô‚Á‚Û‚­ */		/*	s2->alpha			= 0x00;*/
	s2->x256			= src->x256/*+((src->w128-s2->w128))*/ /*-t256((8+8))*/;
	s2->y256			= src->y256/*+((src->h128-s2->h128))*/ /*-t256((8+8))*/;
	return (s2);
}
/*
 (46-24)/2 == 11
*/

void add_chuu_youkai2(STAGE_DATA *l)
{
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
		s1->boss_base_state001						= SS00;
		data->repeat						= (2+2+1);
		data->start_limit_y256				= ((l->user_y)<<(8));/* t256(50.0) */
		data->s2							= s2;
//		data->wait2 						= (10-1);/*0*/
		#if 1
	//------------ ’e–‹ŠÖ˜A
		data->start_danmaku 				= ((l->user_1_moji+0x10/*‚Æ‚è‚ ‚¦‚¸r27ŒÝŠ·*/)&0x1f);
		s1->boss_base_danmaku_type		= 0;
		s1->boss_base_danmaku_time_out	= 0;
		#endif
	}
}
