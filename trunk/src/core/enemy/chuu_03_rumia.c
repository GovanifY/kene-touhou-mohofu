
#include "bullet_object.h"

/*---------------------------------------------------------
		"ƒ‹[ƒ~ƒA",		"GFAIRY",
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	BOSS_BASE boss_base;
//------------ ˆÚ“®ŠÖ˜A
	int state1;
	int time_out;	/* s“®wait */
	int repeat; 	/* ŒJ‚è•Ô‚µ‰ñ” */
	int start_danmaku;
	int start_x256; /* “oêxÀ•W */
} RUMIA_DATA;
//	int wait2;	/* UŒ‚wait */

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

/*---------------------------------------------------------
	s = ƒ{ƒX–{‘Ì boss sprite
	t = ƒvƒŒƒCƒ„[‚Ì’e player's weapon
---------------------------------------------------------*/

static void callback_hit_rumia1(SPRITE *src/*“G©‘Ì*/, SPRITE *tama/*©’e*/)
{
	dummy_obj->x256 = tama->x256;
	dummy_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(dummy_obj/*tama->x256,tama->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	RUMIA_DATA *data	= (RUMIA_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_health -= /*w->*/tama->base_weapon_strength;
	if (/*data->base.*/src->base_health <= 0)
	{
		src->base_health = 65535;/* ‚Q‰ñ‚Í“|‚¹‚È‚¢ */
		bullets_to_hosi();/* ’e‘S•”A¯ƒAƒCƒeƒ€‚É‚·‚é */
	//
		const u8 item_tbl[4] =
		{
			(SP_ITEM_04_P128&0xff), 	/* [F] */
			(SP_ITEM_01_P008&0xff), 	/* [P]‘å */
			(SP_ITEM_00_P001&0xff), 	/* [p]¬ */
			(SP_ITEM_06_TENSU&0xff), 	/* [“_] */
		};
		item_create(src, (SP_GROUP_ITEMS|(item_tbl[difficulty])), 7, ITEM_MOVE_FLAG_06_RAND_XY/*ITEM_MOVE_FLAG_01_COLLECT*/);/* ‚¿‚ç‚Î‚é */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type				= SP_DELETE;
		data->state1			= SS04; 		/* ã‚Ö‘Şê */
		/* ƒR[ƒ‹ƒoƒbƒN“o˜^ */
		src->callback_hit_enemy = NULL; 	/* ƒ_ƒ~[ƒR[ƒ‹ƒoƒbƒN“o˜^ */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void lose_rumia1(SPRITE *src)
//{
//
//}


/*---------------------------------------------------------
	’e–‹‚ªI‚í‚é‚Ü‚Å‘Ò‚ÂB
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)
{
	RUMIA_DATA *data = (RUMIA_DATA *)src->data;
	if (DANMAKU_00 == data->boss_base.danmaku_type)
	{
		data->state1++/* = nextstate*/;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_rumia1(SPRITE *src)
{
	RUMIA_DATA *data=(RUMIA_DATA *)src->data;
	data->time_out--;
	switch (data->state1)
	{
	case SS00:	/* ã‚©‚ç“oê */
		src->y256		+= t256(2); 	/**fps_fa ctor*/
		if (data->start_x256 < src->y256)	{	data->time_out=30;	data->state1 = SS03; }
		break;
//---------
	case SS01:	/* ’e–‹ƒZƒbƒg */
			/*data->wait2=8 10*/
			data->state1++;/* = SS02;*/
			;/* 8‹ô”’e */
			;/* 7Šï”’e */
			data->boss_base.danmaku_type		= data->start_danmaku/*DANMAKU_01*/;			/* âS’e–‹‚ğƒZƒbƒg */
			data->boss_base.danmaku_time_out	= DANMAKU_01_SET_TIME;	/* âS’e–‹‚Ì”­¶ŠÔ */
		break;
	case SS02:	/* âS’e–‹‚ªI‚í‚é‚Ü‚Å‘Ò‹@ */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* ‚µ‚Î‚µ‘Ò‚Â */
		if (1 > data->time_out)
		{
			data->time_out=100/*150*/;
			/* ‹K’è‚ÌŒJ‚è•Ô‚µ‰ñ”‚±‚È‚µ‚½‚ç‘Şê */
			data->repeat--;
			if ((0) != data->repeat)	{	data->state1	= SS01; 		}/* ŒJ‚è•Ô‚µ */
			else						{	data->state1++/* = SS04*/;		}/* ã‚Ö‘Şê */
		}
		break;
//---------
	case SS04:	/* ã‚Ö‘Şê */
		src->y256		-= t256(2); 	/**fps_fa ctor*/
		if ( -((src->h128+src->h128)) > src->y256)
		{
			src->type = SP_DELETE;
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		src->type 			= TEKI_00_BOSS11+((data->time_out&0x30)>>4);
	}
//	src->yx_anim_frame	= ((data->time_out&0x10)>>4);
//
	danmaku_generator(src); /* ’e–‹¶¬ */
}

/*---------------------------------------------------------
	“G‚ğ’Ç‰Á‚·‚é
---------------------------------------------------------*/

void add_chuu_rumia(STAGE_DATA *l)/*int lv*/
{
	{
		SPRITE *sakuya;
//		sakuya						= sprite_add_res(BASE_RUMIA08_PNG); 	//s->anim_speed = 3;
		sakuya						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 	//s->anim_speed = 3;
		sakuya->type				= /*SP_CHUU*/TEKI_00_BOSS11/*SP_ZAKO_YOKAI1*/;
//		sakuya->type				= SP_CHUU/*SP_ZAKO_YOKAI1*/;
		sakuya->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		sakuya->callback_mover		= move_rumia1;
	//	sakuya->callback_loser		= lose_rumia1;
		sakuya->callback_hit_enemy	= callback_hit_rumia1;	/* ƒR[ƒ‹ƒoƒbƒN“o˜^ */
		sakuya->y256				= t256(-30);
	//	sakuya->x256				= ((l->user_x)*t256(35))+t256(40);
	//	sakuya->x256				= ((l->user_x)*t256(32))+t256(48);
	//	sakuya->x256				= ((l->user_x)<<(8+5))+t256(48);
		sakuya->x256				= ((l->user_x)<<(8));
//
		/*data->base.*/sakuya->base_health		= 200+(difficulty<<4);	/* easy‚Å‚à‘¶İŠ´‚ğˆóÛ‚Ã‚¯‚éˆ×‚É 200 ‚Í•K—v */ 	// 50+150*difficulty;
		/*data->base.*/sakuya->base_score		= score(100)+score(100)*difficulty;
//
		RUMIA_DATA *data;
		data								= mmalloc(sizeof(RUMIA_DATA));
		sakuya->data						= data;
		data->state1						= SS00;
//		data->time_out						= 30;
//		data->wait2 						= 10;/*0*/
		data->repeat						= (2+2+1);
		data->start_x256					= ((l->user_y)<<(8));/* t256(50) */
		#if 1
	//------------ ’e–‹ŠÖ˜A
		data->start_danmaku 				= ((l->user_1_moji+0x10/*‚Æ‚è‚ ‚¦‚¸r27ŒİŠ·*/)&0x1f);
		data->boss_base.danmaku_type		= 0;
		data->boss_base.danmaku_time_out	= 0;
		#endif
	}
}
