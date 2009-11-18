
#include "bullet_object.h"

/*---------------------------------------------------------
	地形に関連する敵
	-------------------------------------------------------

---------------------------------------------------------*/

#if 0
typedef struct
{
//	ENEMY_BASE base;
	int speed256;
//	int state;
//	int tx256;
//	int ty256;
//	int level;
} BGPANEL_DATA;

typedef struct
{
//	ENEMY_BASE base;
	int speed256;
	int state;
	int wait1;		// [***090124		追加
	int wait2;
//	int tx256;
//	int ty256;
//	int level;
} GROUNDER_DATA;
#endif

typedef struct
{
#if 1
//	ENEMY_BASE base;		// [***090410		???追加???
#endif
	int speed256;
	int state;
	int wait1;
	int wait2;
//	int level;
} MAGICF_DATA;

#define GROUNDER_DATA MAGICF_DATA
#define BGPANEL_DATA  MAGICF_DATA

/*---------------------------------------------------------
	消える魔方陣(おｋのやつ) "魔方陣2"
---------------------------------------------------------*/
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
};
static void move_magic_formation(SPRITE *src)
{
	MAGICF_DATA *data = (MAGICF_DATA *)src->data;
	switch (data->state)
	{
	case SS00:
		data->wait1 -= 1/*fps_fa ctor*/;
		if (data->wait1 <= 0)
		{
		//	data->wait1=8/*5*/;
		//	src->alpha	= 0;
			data->state++;/* = SS01;*/
		}
		break;
	case SS01:		/* 出現 */
		src->color32 += 0x03000000;			/*	src->alpha += 0x03;*/ /*4*/ /**fps_fa ctor*/
		if (0xb4000000 < (src->color32) ) 	/* (0xb4 < (unsigned int)(src->alpha)) */
		{
			src->color32 = 0xb4ffffff;		/*	src->alpha = 0xb4;*/
			data->state++;/* = SS02;*/
		}
		break;
	case SS02:
		if (0 < data->wait2)		// [***090128		変更
		{
			data->wait1--;
			if (data->wait1 <= 0)
			{
				data->wait1=8/*5*/;/*プログラムが速くなってる分遅くする*/
				data->wait2--;
				#if (0==USE_DESIGN_TRACK)
				play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
				#else
				voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
				#endif
				/* 速度は 1.0-3.0までの無段階乱数に変更 */
				{	int angle512;
					angle512=(ra_nd()&(512-1))/*%360*/;
					bullet_create_tomari2_dan(src,
						/*t256(sp)*/((ra_nd()&(512-1))+256)/*speed256*/,
						/*degtorad*/(angle512),
						t256(0.04),
						ANGLE_NO_SHOT_DAN);
				}
			}
		}
		else
		{
			data->state++;/* = SS03;*/
		}
		break;
	case SS03:		/* 消える */
		src->color32 -= 0x01000000;			/*	src->alpha -= 1;*/	/*2*/ /**fps_fa ctor*/
		if ( 0x04000000 > (src->color32) )	/*	if ( 0x04 0x00 > (unsigned int)(src->alpha))*/
		{
			src->color32 = 0x00ffffff;		/*	src->alpha = 0x00;*/
			src->type = SP_DELETE;
		}
		break;
	}
}
#if 0
				int sp;
				#if 0
				/* sp は 殆ど2か3で、約1/80の確率で極稀に1を出す */
				sp=ra_nd()%3/*+1-1*/;	/* 0 1 2 */
				if (0==sp/*<2-1*/)
				{
					sp=ra_nd()%3/*+1-1*/;
					if (0==sp/*<2-1*/)
					{
						sp=ra_nd()%3/*+1-1*/;
						if (0==sp/*<2-1*/)
						{
							sp=ra_nd()%3/*+1-1*/;
						}
					}
				}
				#else
			//	sp=ra_nd()%80;
				sp=(ra_nd()&(64-1));/* 確率(1/64) に変更 */
				if (0!=sp)
				{
					sp = (sp&1)+1;
				}
				#endif
				sp++;	/* sp = 1 or 2 or 3 */
				/* 速度は (1.0まれ) か 2.0 か 3.0 */
#endif

/*---------------------------------------------------------
	ばらまき攻撃魔方陣 "魔方陣1"
---------------------------------------------------------*/

static void lose_grounder(SPRITE *src)
{
//	case SP_GROUNDER:
	if (rand_percent(30))	{	item_create(src,					  SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(src,/*->x+10, c->y-5,*/ SP_ITEM_00_P001,	1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
	if (rand_percent(30))	{	item_create(src,/*->x+ 5, c->y+5,*/ SP_ITEM_06_TENSU, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY)/*(up_flags)*/ );}
}

/*---------------------------------------------------------
	ばらまき攻撃魔方陣
---------------------------------------------------------*/
enum
{
	ST00 = 0,
	ST01,
	ST02,
	ST03,
};
static void move_grounder(SPRITE *src)	// [***090124 攻撃パターンを変える */
{
	GROUNDER_DATA *data = (GROUNDER_DATA *)src->data;
	switch (data->state)
	{
	case ST00:
		if ((src->y256 >= player->y256) ||
			(src->y256 > t256(100)) )
		{
			data->state++;/* = ST01;*/
			bullet_create_aka_maru_jikinerai(src, t256(1) );
		}
		break;/*??? [***090210 追加 */
	case ST01:
		if (src->y256 > t256(GAME_HEIGHT+16+2))/* GAME_HEIGHT+16+2==290 */
		{
			src->type = SP_DELETE;
		}
	}
	if (data->wait2<(difficulty*7)+10) // [***090128 変更 */
	{
		data->wait1--;
		if (data->wait1 <= 0)
		{
			data->wait1=25-(difficulty*5);
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE10_MAGICF_E_SHOT);
			#else
			voice_play(VOICE10_MAGICF_E_SHOT, TRACK06_ALEART_IVENT_02);/*テキトー*/
			#endif
			/* 速度は 1.0-4.0までの乱数(ただし上方域に偏差)に変更 */
			bullet_create_tomari2_dan(src,
				((ra_nd()&(1024-1))|(0x100))/*t256(sp)*/,
				/*deg512_2rad*/((ra_nd()&(512-1)))/*de gtorad(ra_nd()%360)*/,
				t256(0.03),
				ANGLE_NO_SHOT_DAN);
		}
	}
	src->y256 += data->speed256/**fps_fa ctor*/;
}
#if 0
			int sp;
			#if 0
			/* sp は 殆ど4で、約42%の確率で 1 2 3 を出す */
			sp=((ra_nd()&(4-1))/*+1-1*/);/* 0 1 2 3 */
			if (sp<(4-1))
			{
				sp=((ra_nd()&(4-1))/*+1-1*/);/* 0 1 2 3 */
				if (sp<(4-1))
				{
					sp=((ra_nd()&(4-1))/*+1-1*/);/* 0 1 2 3 */
					/* この時点で 0 1 2 が出る確率は(0.75*0.75*0.75==0.421875%) */
				}
			}
			sp++;
			#else
			if (rand_percent(56))	/*56%の確率で*/
			{
				sp=ra_nd()&3;
				/* この時点で 1 2 3 が出る確率は(約42%==0.56*3/4) */
				if (0==sp)
				{
					sp = 4;/*残りは4*/
				}
			}else
			{
				sp = 4;/*残りは4*/
			}
			#endif
			/* sp == 1 or 2 or 3 or 4 */
#endif

/*---------------------------------------------------------
	ベースパネル
---------------------------------------------------------*/

static void move_bg_panel(SPRITE *src)
{
	BGPANEL_DATA *data = (BGPANEL_DATA *)src->data;
	if (src->y256 > t256(GAME_HEIGHT+2)) /* 272+2 == 274 */
	{
		src->type = SP_DELETE;
	}
	src->y256 += data->speed256/**fps_fa ctor*/;
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/


//#define OLD_PANEL (0/*1*/)
//#if (1==OLD_PANEL)
//extern void enemy_bgpanel_add_xy( 		const short xxx, const short yyy, char speed);/*統合化*/
//extern void enemy_bgpanel2_add_xy(		const short xxx, const short yyy, char speed);/*統合化*/
//extern void enemy_grounder_add_xy(		const short xxx, const short yyy, const short speed256);/*統合化*/
//extern void enemy_magicformation_add_xy(	const short xxx, const short yyy, const short speed256);/*統合化*/
//#endif


//	l->user_x, l->user_y, l->para3, /*type*/l->command/*1-1*/
//actually lv is the x coord
//id enemy_magicformation_add_xy(const short xxx, const short yyy, const short speed256){}/*統合化*/
//id enemy_grounder_add_xy(const short xxx, const short yyy, const short speed256) {}/*統合化*/
void add_enemy_panels(STAGE_DATA *l
//const short xxx,
//const short yyy,
//const short speed256,
//int type
)
{
short xxx;
short yyy;
short speed256;
int type;
//
xxx=l->user_x;
yyy=l->user_y;
speed256 = l->scroll_speed256;
type = /*type*/l->user_command/*1-1*/;
//
	short yy2;
	yy2 = yyy;
	BGPANEL_DATA *data;
//	GROUNDER_DATA *data;
//	MAGICF_DATA *data;
	data			= mmalloc(sizeof(BGPANEL_DATA));
//	data			= mmalloc(sizeof(GROUNDER_DATA));
//	data			= mmalloc(sizeof(MAGICF_DATA));
	data->state 	= 0;/* SS00 または ST00 */
//	data->tx256 	= player->x256;
//	data->ty256 	= player->y256;
//	data->level 	= 0;
//
	SPRITE *s;
	s = NULL;
//
	switch (type)
	{
	case BTYPE_01_BGPANEL1:  /*(1-1)*/		/* パネル台 */		//		"台",
		s=sprite_add_res(BASE_TIKEI_BGPANEL1_PNG);	//	s->anim_speed	= 0/*6*/;/*とりあえずなし*/ 	/*P R_BACK0*/
		goto BTYPE_BGPANEL;
		/*not_break;*/
	case BTYPE_02_BGPANEL2:  /*(2-1)*/		/* パネル棒 */		//		"棒",
		s=sprite_add_res(BASE_TIKEI_BGPANEL2_PNG);	//	s->anim_speed	= 0;/* 0 元々しない, 6==KETMのバグ */	/*P R_BACK0*/
		/*goto BTYPE_BGPANEL;*/
		/*not_break;*/
	BTYPE_BGPANEL:
		s->type 				= SP_ZAKO/*SP_TIKEI*/ /*SP_BGPANEL*/;
	//	s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_bg_panel;
	//	s->alpha				= 255;
		s->anim_speed			= 0/*6*/;
		s->data 				= data;
		/*data->base.*/s->base_score			= score(10*2);
		/*data->base.*/s->base_health			= 2;/*???*/
		data->speed256			= (speed256);
		break;

	case BTYPE_03_GROUNDER: 		/* ばらまき攻撃魔方陣 */	//	"魔方陣1",	/*	"GROUNDER",*/
		/*
		anim_speed: 1 速い
		anim_speed: 3 遅すぎ
		*/
		s						= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	//s->anim_speed =  2/*3*/ /*(5-difficulty)*/ /*1*/;/*9"grounder.png"*/
		s->type 				= SP_ZAKO/*SP_TIKEI*/ /*SP_GROUNDER*/;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_grounder;
		s->callback_hit_enemy	= callback_hit_zako;
		s->callback_loser		= lose_grounder;
	//	s->alpha				= 255;
		s->data 				= data;
		/*data->base.*/s->base_score			= score(200*2);
		/*data->base.*/s->base_health			= 25+(difficulty<<3/*4*/);//+(difficulty*15);
		data->wait1 			= 100;	// [***090124		追加
		data->wait2 			= 0;
		data->speed256			= (speed256);
		break;
	case BTYPE_04_MAGIC_FORMATION:		/* 消える魔方陣 */	//	"魔方陣2",	/*	"MAGICF",*/
		s						= sprite_add_res(BASE_TIKEI_GROUNDER08_PNG);	s->anim_speed	=  3 /*(5-difficulty)*/ /*2*/;/*9"grounder.png"*/
		s->type 				= SP_MUTEKI;
		s->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		s->callback_mover		= move_magic_formation;
	//	s->callback_hit_enemy	= callback_hit_zako;
		s->color32				= 0x02ffffff;	/*	s->alpha				= 0x02 0x00;*/
		s->data 				= data;
	#if 1
		/*data->base.*/s->base_score			= score(200*2); 		// [***090410		???追加???
		/*data->base.*/s->base_health			= 25+(difficulty<<2);	// [***090410		???追加???
	#endif
		data->wait1 			= 10/*+90*/;/* */
		data->wait2 			= 5/*16+(difficulty<<3)*/ /*10+(7*difficulty)*/;
		yy2 = -yyy; 			/* KETMの相対座標指定から、MAGICF形式の絶対座標指定に変換する */
		break;
	}
//
		s->anim_frame			= 0;
		s->x256 				=  ((xxx)<<8);
	//	s->y256 				=  ((yyy)<<8)-((s->h128+s->h128));
		s->y256 				= -((yy2)<<8)-((s->h128+s->h128));
}
