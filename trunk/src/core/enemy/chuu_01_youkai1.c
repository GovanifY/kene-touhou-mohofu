
#include "bullet_object.h"

/*---------------------------------------------------------
		"妖怪2",(予定)		"GFAIRY",	(r27から新規追加)
	-------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
//	ENEMY_BASE base;
	int state;
	int wait1;	/* 行動wait */
	int wait2;	/* 攻撃wait */
	int nnn;
	SPRITE *s2;
} YOKAI1_DATA;


/*---------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_hit_youkai1(SPRITE *src/*敵自体*/, SPRITE *tama/*自弾*/)
{
	dummy_obj->x256 = tama->x256;
	dummy_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(dummy_obj/*tama->x256,tama->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
	YOKAI1_DATA *data	= (YOKAI1_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_health -= /*w->*/tama->base_weapon_strength;
	if (/*data->base.*/src->base_health <= 0)
	{
		bullets_to_hosi();/* 弾全部、星アイテムにする */
	//
		item_create(src, SP_ITEM_06_TENSU, 7, ITEM_MOVE_FLAG_06_RAND_XY/*ITEM_MOVE_FLAG_01_COLLECT*/);/*点数を出す*/	/* ちらばる */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
		src->type				= SP_DELETE;
		data->s2->type			= SP_DELETE;
		/* コールバック登録 */
		src->callback_hit_enemy = NULL; 	/* ダミーコールバック登録 */
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

static void move_youkai1(SPRITE *src)
{
	YOKAI1_DATA *data=(YOKAI1_DATA *)src->data;
		data->wait1--;
	switch (data->state)
	{
	case 0:/* 上から登場 */
		data->s2->y256	+= t256(2); 	/**fps_fa ctor*/
		src->y256		+= t256(2); 	/**fps_fa ctor*/
		if (t256(50) < src->y256)	{					data->state=1;	}
		break;
	case 1:
		if (1 > data->wait1)
		{
			if (2 != data->nnn) {	data->wait1=150;	data->state=2;	}
			else				{						data->state=5;	}
		}
		break;
	case 2:
		if (1 > data->wait1)	{	data->wait1=30; 	data->state=3;	/*data->wait2=8 10*/;	}
		else
		{
			data->wait2--;
			if (1 > data->wait2)
			{
				/* 0: 20 == 20-(0*4) */
				/* 1: 16 == 20-(1*4) */
				/* 2: 12 == 20-(2*4) */
				/* 3:  8 == 20-(3*4) */
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
				#else
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
				#endif
				bullet_create_n_way_dan_sa_type(src,
					(t256(3.0)+((difficulty)<<6)),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_UROKO14_03_MIDORI,
					8);
			}
		}
		break;
	case 3:
		if (1 > data->wait1)	{	data->wait1=100;	data->state=4;	}
		break;
	case 4:
		if (1 > data->wait1)	{	data->wait1=30; 	data->state=1;	/*data->wait2=8 10*/;	data->nnn++;	}
		else
		{
			data->wait2--;
			if (1 > data->wait2)
			{
				data->wait2 = (20-((difficulty)<<2))/*8*/ /*10*/;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE14_BOSS_KOUGEKI_01);
				#else
				voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*テキトー*/
				#endif
				bullet_create_n_way_dan_sa_type(src,
					t256(3.25)+((difficulty)<<6),
					ANGLE_JIKI_NERAI_DAN,
					(int)(512/24),
					BULLET_UROKO14_04_MIZUIRO,
					7);
			}
		}
		break;
	case 5:/* 上へ退場 */
		data->s2->y256 -= t256(2);		/**fps_fa ctor*/
		src->y256 -= t256(2)/**fps_fa ctor*/;
		if ( -((src->h128+src->h128)) > src->y256)
		{
			src->type = SP_DELETE;
			data->s2->type = SP_DELETE;
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
	//	src->an im_frame 	= ((data->wait1&0x10)>>4);
		src->type 			= TEKI_54_CHOU1+((data->wait1&0x10)>>4);
	}
//
	data->s2->m_angleCCW512++;
	mask512(data->s2->m_angleCCW512);
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

static SPRITE *create_usiro_no_mahojin(SPRITE *src) //魔方陣グラフィック生成
{
	SPRITE *s2; 		// 魔方陣グラフィックのスプライト
//	s2					= sp rite_add_res(BASE_MAHOUJIN_0_PNG);		//s2->anim_speed	= 0;/*"boss04-lo.png"*/
//	s2					= sprite_add_gu(TAMA_TYPE_BULLET_JIPPOU32_PNG); 	//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2					= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG); 			//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->type			= TEKI_51_MAHOJIN1/*SP_MUTEKI*/;
//	s2->type			= MAHOU_JIN_00_aaa/*SP_MUTEKI*/;
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
//	s2->an im_frame		= 0;
	s2->color32 		= 0xaa0000ff;		/* 赤っぽく */		/*	s2->alpha			= 0x00;*/
	s2->x256			= src->x256/*+((src->w128-s2->w128))*/-t256(8);
	s2->y256			= src->y256/*+((src->h128-s2->h128))*/-t256(8);
	return (s2);
}

void add_chuu_youkai1(STAGE_DATA *l)/*int lv*/
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
	//	s->x256 				= (l->user_y)*t256(35)+t256(40);
	//	s->x256 				= ((l->user_y)*t256(32))+t256(48);
	//	dummy_obj->x256 		= ((l->user_y)<<(8+5))+t256(48);
		dummy_obj->x256 		= ((l->user_x)<<(8));
		dummy_obj->y256 		= t256(-30);
	//
		s2						= create_usiro_no_mahojin(dummy_obj);
	//
	//	s1						= sp rite_add_res(BASE_GREAT_FAIRY02_PNG);	//s->anim_speed = 3;
		s1						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);	//s->anim_speed = 3;
		s1->type 				= /*SP_CHUU*/TEKI_54_CHOU1/*SP_ZAKO_YOKAI1*/;
	//	s1->type 				= SP_CHUU/*SP_ZAKO_YOKAI1*/;
		s1->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s1->callback_mover		= move_youkai1;
	//	s1->callback_loser		= lose_youkai1;
		s1->callback_hit_enemy	= callback_hit_youkai1; 	/* コールバック登録 */
		s1->x256 				= dummy_obj->x256;
		s1->y256 				= dummy_obj->y256;
		YOKAI1_DATA *data;
		data					= mmalloc(sizeof(YOKAI1_DATA));
		s1->data 				= data;
		data->state 			= 0;
		data->wait1 			= 30;
		data->wait2 			= 10;/*0*/
		/*data->base.*/s1->base_score		= score(100)+score(100)*difficulty;
		/*data->base.*/s1->base_health		= 200+(difficulty<<4);	/* easyでも存在感を印象づける為に 200 は必要 */ 	// 50+150*difficulty;
		data->nnn				= 0;
		data->s2				= s2;
	}
}
