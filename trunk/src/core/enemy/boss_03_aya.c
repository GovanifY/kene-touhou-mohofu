
#include "bullet_object.h"

/*---------------------------------------------------------
	射命丸 文
	天狗
	-------------------------------------------------------
	ボスタイマー未対応
	-------------------------------------------------------
	自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	もっと丁寧に作り直した方がいいかも。
	[*** 090114 	天狗の速度を難易度ごとに変えてみた。
---------------------------------------------------------*/

typedef struct _boss02_data
{
	BOSS_BASE boss_base;
	/*static*/ int angle512/*angle_radian*/;/*radianで持つよりintで持って使う度に変換した方が速い*/
	int state1;
	int state2; 	// 攻撃パターン変更用
	int wait1;		// 移動待ち
	int wait2;		// モーション変更用
	int xp256;
	int n_wait3;
//	int level;
} BOSS02_DATA;

typedef struct
{
//	int boss_id;	// get_SPRITE ->id;
	int angle512;
	int wait;
} BOSS02_SR_DATA;

typedef struct
{
	int stat;
	int add_angle512;
	int angle512;
	int speed256;
	int time_out;
} BOSS02_ICE_DATA;

/*---------------------------------------------------------

	-------------------------------------------------------
	s = ボス本体 boss sprite
	t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/
static void callback_enemy_boss02_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{
}
static void callback_enemy_boss02_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*先に実行した方が速い*/
//	parsys_add(NULL,100,0,t->x,t->y,30,0,0,50,PIXELATE,NULL);
	BOSS02_DATA *data=(BOSS02_DATA *)s->data;
	WEAPON_BASE *w=(WEAPON_BASE *)t->data;
//
	int data_base_health_alt;
	data_base_health_alt = data->boss_base.health;
	data->boss_base.health -= w->strength;
//
	if ( ((data_base_health_alt)&(1024-1)) < ((data->boss_base.health)&(1024-1)) )
	{
		play_voice_auto_track(VOICE07_BOMB);
		item_from_bullets(SP_ITEM_05_HOSI);
		item_create_for_boss(s, ITEM_CREATE_MODE_02);
		data->state2++;
		if (2 < data->state2)	{	data->state2 = 2;	}
		//
		if (2 == data->state2)
		{
		//	data->state2	= 2;
			data->state1	= 0;
			data->wait1 	= 40;
			player_add_score(data->boss_base.score);
		}
		explosion_add_circle(/*d*/s, 1);
	}
	// HPが残り1024になった時。攻撃パターン変更
//	else if ((data->boss_base.health<=(1024*2/*1024*1*/)) ) {	}

	if (data->boss_base.health <= 0)/*全員倒した？*/
	{
		item_create_for_boss(s, ITEM_CREATE_MODE_01);
//
		player_add_score(data->boss_base.score);
	//
		player_set_destoroy_boss();
		explosion_add_circle(/*d*/s, 1);	/* player_set_destoroy_boss();B02_BOSS_DESTROY が必要 */
		/*d*/s->type	= SP_DELETE;
		/* コールバック登録 */
		((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss02_hitbyweapon_dummy; /* ダミーコールバック登録 */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
#if (1==USE_CONTROLLER)
static void enemy_boss02_sr_controller1(CONTROLLER *c)
{
	BOSS02_SR_DATA *e=(BOSS02_SR_DATA *)c->e;
	if (e->wait>0)
	{
		e->wait -= 1/*fps_fa ctor*/;
	}
	else
	{
		SPRITE *src;/* srcはbossオブジェ */
		src 		= sprite_get_by_id(e->boss_id); //なければNULL
		e->wait 	= 10;
		e->angle512 += deg_360_to_512(15);
		if ((e->angle512 > deg_360_to_512(360-15))||(src==NULL))
		{
			controller_remove_one(c);
		}
		else
		{
			bullet_create_offset_dan_type(src, t256(7), e->angle512, /*offsx*/t256(0-16)/*(src->w/2)*/, /*offsy*/t256(0)/*(src->h/2)*/, BULLET_HARI32_01_AKA);
		}
	}
}
#endif //(1==USE_CONTROLLER)
/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss02_sr_add1(SPRITE *c)
{
	#if (1==USE_CONTROLLER)
	CONTROLLER *s;
	s				= controller_add();
	#endif //(1==USE_CONTROLLER)
	BOSS02_SR_DATA *data;
	data			= mmalloc(sizeof(BOSS02_SR_DATA));
	data->angle512	= deg_360_to_512(0);
	data->wait		= 10;

//	data->boss_id	= c->id;	//get_SPRITE ->id;
	#if (1==USE_CONTROLLER)
	s->con			= enemy_boss02_sr_controller1;
	s->e			= data;
	#endif //(1==USE_CONTROLLER)
}

/*---------------------------------------------------------

---------------------------------------------------------*/
	#if (1==USE_CONTROLLER)

static void enemy_boss02_sr_controller2(CONTROLLER *c)
{
	BOSS02_SR_DATA *e=(BOSS02_SR_DATA *)c->e;
	if (e->wait>0)
	{
		e->wait -= 1/*fps_fa ctor*/;
	}
	else
	{
		SPRITE *src;/* srcはbossオブジェ */
		src 		= sprite_get_by_id(e->boss_id); //なければNULL
		e->wait 	= 10;
		e->angle512 -= deg_360_to_512(15);
		if ((e->angle512 <= deg_360_to_512(15))||(src==NULL))
		{
			controller_remove_one(c);
		}
		else
		{
			bullet_create_offset_dan_type(src, t256(7), e->angle512, /*offsx*/t256(0-16)/*(src->w/2)*/, /*offsy*/t256(0)/*(src->h/2)*/, BULLET_HARI32_01_AKA);
		}
	}
}
	#endif //(1==USE_CONTROLLER)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss02_sr_add2(SPRITE *c)
{
	#if (1==USE_CONTROLLER)
	s			= controller_add();
	CONTROLLER *s;
	#endif //(1==USE_CONTROLLER)
	BOSS02_SR_DATA *data;
	data			= mmalloc(sizeof(BOSS02_SR_DATA));
	data->angle512	= deg_360_to_512(360)-1;/*追加 (-1)*/
	data->wait		= 8;
//	data->boss_id	= c->id;	//get_SPRITE ->id;
	#if (1==USE_CONTROLLER)
	s->con			= enemy_boss02_sr_controller2;
	s->e			= data;
	#endif //(1==USE_CONTROLLER)
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss02_ice_move(SPRITE *s)
{
	BOSS02_ICE_DATA *data = (BOSS02_ICE_DATA *)s->data;
	switch (data->stat)
	{
	case 0:
		data->time_out -= 1/*fps_fa ctor*/;
		if (data->time_out <= 0)
		{	data->stat = 1;}
		data->angle512 += data->add_angle512/**fps_fa ctor*/;
		break;
	case 1:
		{
			const short sss[4] =
			{
				t256(2.5),		/* easy:   2 これが避けれないって事は無いと思うケド */
				t256(3/*4*/),	/* normal: 4 */
				t256(5/*6*/),	/* hard:   6 落ち着いてよく見れば避けれます */
				t256(10/*12*/)	/* luna:  12 落ち着いてよく見れば避けれますが、たまに失敗するwww */
			};
			data->speed256 = sss[difficulty]/*t256(12)*/;
		}
		data->angle512	= (ra_nd()&(128-1)/*%90*/)+deg_360_to_512(45);
		data->stat		= 2;
		break;
	}
//	if (d->angle512 < deg_360_to_512(0))	{	d->angle512 += deg_360_to_512(360);}
//	if (d->angle512 > deg_360_to_512(360))	{	d->angle512 -= deg_360_to_512(360);}
	mask512(data->angle512);

	/*以下rwingx.cと同じ*/
	s->x256+=((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	s->y256+=((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
//	s->anim_frame=(deg_512_to_360(data->angle512+deg_360_to_512(270))/10)%36;
//	s->anim_frame = ((((data->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
//	s->anim_frame = ((((data->angle512)&(512-1))*(32/2))>>8);
	s->anim_frame = ((((data->angle512)&(512-1)))>>4);
	/*(ここまで)[以下rwingx.cと同じ]*/
//
			#if 1
			/* 描画用角度 */
		//	s->m_angle512		= data->angle512;
			int aaa512; 	aaa512 = (/*127*/255-data->angle512);	mask512((aaa512));
			s->m_angle512		= aaa512;
			#endif
	if ((s->x256 < t256(-100))||(s->x256 > t256(100)+t256(GAME_WIDTH))||
		(s->y256 < t256(-100))||(s->y256 > t256(100)+t256(GAME_HEIGHT) ) )
	{	s->type=SP_DELETE;}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#define NUM_OF_ENEMIES (30)

	#if (1==USE_CONTROLLER)
static void enemy_boss02_ice_controller(CONTROLLER *c)
{
	int *id_array = c->e;
	SPRITE *s;
	int i;
	int dead=0;
//	SPRITE *master;
//	master=sp rite_get_by_id(id_array[0]);
//	BOSS02_ICE_DATA *data;
//	data=(BOSS02_ICE_DATA *)master->data;
	for (i=0; i<NUM_OF_ENEMIES; i++)
	{
		s=sprite_get_by_id(id_array[i]);
		if (s==NULL)
		{
			dead=1;
			break;
		}
	}
	if (dead==1)
	{
		for (i=0; i<NUM_OF_ENEMIES; i++)
		{
			s=sprite_get_by_id(id_array[i]);
			if (NULL != s)
			{	s->type=SP_DELETE;}
		}
		controller_remove_one(c);
	//	return;
	}
	else
	{
		for (i=NUM_OF_ENEMIES-1; i>=1; i--)
		{
			SPRITE *l;
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if ((NULL != s)&&(NULL != l))
			{
				s->x256 = l->x256;
				s->y256 = l->y256;
				s->anim_frame = l->anim_frame;
			}
		}
	}
}
	#endif //(1==USE_CONTROLLER)

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss02_ice_add(SPRITE *s)
{
	#if (1==USE_CONTROLLER)
	CONTROLLER *e;
	int *id_array;
	#endif //(1==USE_CONTROLLER)
	SPRITE		*s2;
	int i;
	int j;
	int rand_add_angle;
	rand_add_angle=(ra_nd()&(8-1));
	BOSS02_ICE_DATA *data;
	for (j=0; j<2; j++)
	{
		#if (1==USE_CONTROLLER)
		e				= controller_add();
	//	NUM_OF_ENEMIES=30;
		id_array		= mmalloc(sizeof(int)*NUM_OF_ENEMIES);
		e->e			= id_array;
		e->con			= enemy_boss02_ice_controller;
		#endif //(1==USE_CONTROLLER)

		for (i=0; i<NUM_OF_ENEMIES; i++)
		{
		//	s2					= spr ite_add_file 0("tama/jippou32.png",32,PRIORITY_05_BULLETS/*PRIORITY_03_ENEMY_WEAPON*/, 0);/*36"boss02_w.png"*/
		//	s2					= sprite_add_res(BASE_TAMA_BULLET_JIPPOU32_PNG);
			s2					= sprite_add_bullet(TAMA_TYPE_BULLET_JIPPOU32_PNG);
			#if (1==USE_CONTROLLER)
			id_array[i] 		= s2->id;
			#endif //(1==USE_CONTROLLER)
			s2->type			= BULLET_CAP16_04_SIROI;/*SP_BULLET*/ /*SP_BOSS02ICE*/
			s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		//	if (0==i)
			{
			}
		//	else
		//	{
		//		s2->callback_mover	= NULL;
		//		s2->data			= NULL;
		//	}
				s2->callback_mover	= enemy_boss02_ice_move;
				data				= mmalloc(sizeof(BOSS02_ICE_DATA));
				s2->data			= data;
			//	data->add_angle512	= (j == 0)?(deg_360_to_512(+5)):(deg_360_to_512(-5));
				data->add_angle512	= rand_add_angle;
				data->angle512		= (i<<4)/*deg_360_to_512(90)*/;
				data->stat			= 0;
				data->time_out		= 120/*100*/;
				data->speed256		= t256(4);
			//	s2->x256			= s->x256 + ((0==j)?(t256(-19)):(t256(81)));
				s2->x256			= s->x256 + ((0==j)?(t256(-64)):(t256(64)));
				s2->y256			= s->y256 /*+ t256(135)*/;
			//	s2->y256			= s->y256 /*+ t256(138)*/;
		}
		rand_add_angle = -rand_add_angle;
	}
}
#undef NUM_OF_ENEMIES

static void enemy_boss02_waitstate(SPRITE *s, int next_set_state)
{
	BOSS02_DATA *data = (BOSS02_DATA *)s->data;
	if (data->wait1 > 0)
	{
		data->wait1 -= 1/*fps_fa ctor*/;
		data->wait2 += 1/*fps_fa ctor*/;
		if (data->wait2 >= 3)
		{
			data->wait2 = 0;
			if (s->anim_frame<4)
			{
				s->anim_frame++;
			}
			else if (s->anim_frame>4)
			{
				s->anim_frame--;
			}
		}
	}
	else
	{
		data->state1 = next_set_state;
		bullet_create_oodama0(s, t256(4)+(difficulty<<7), ANGLE_JIKINERAI_DAN, 10, 0, ((s->w128)), ((s->h128)));
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	ST_00 = 0,
	ST_01,
	ST_02,
	ST_03,
	ST_04,
	ST_05,
	ST_06,
	ST_07,
	ST_08,
	ST_09,
	ST_10,
	ST_11,
	ST_12,
};
enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
	SS05,
	SS06,
	SS07,
	SS08,
	SS09,
	SS10,
	SS11,
	SS12,
};

static void enemy_boss02_momiji_add(SPRITE *s)
{
	bullet_create_momiji_dan(s, t256(3.5), deg_360_to_512(30*(3-2)) );
	bullet_create_momiji_dan(s, t256(4.0), deg_360_to_512(30*(3+0)) );
	bullet_create_momiji_dan(s, t256(3.5), deg_360_to_512(30*(3+2)) );
}
static void enemy_boss02_move(SPRITE *s)
{
	BOSS02_DATA *data=(BOSS02_DATA *)s->data;
	switch (data->state2)
	{
	case 0: 		// 攻撃パターン1
	case 1:
		switch (data->state1)
		{
		case ST_00: /* nach unten */
			if (s->y256 < t256(30))
			{	s->y256 += t256(2) /**fps_fa ctor*/;}
			else
			{
				data->state1 = ST_01;
			//	((PLAYER_DATA *)player->data)->bo ssmode=B05_BE FORE_LOAD;
				((PLAYER_DATA *)player->data)->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
				data->wait1 = 0;
			}
			break;

		case ST_01:
		//	if (((PLAYER_DATA *)player->data)->bo ssmode==B01_BA TTLE)
		//	if ((STATE_FLAG_05_IS_BOSS|0) == (((PLAYER_DATA *)player->data)->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
			if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
			{
				data->wait1 += 1/*fps_fa ctor*/;
				if (data->wait1 >= 40)
				{
					data->state1 = ST_02;
				}
			}
			break;

		case ST_02: /* y-pos erreicht: rundumschuss */
			enemy_boss02_sr_add1(s);
			if (0==data->state2)
			{	enemy_boss02_ice_add(s);}
			else if (1==data->state2)
			{	enemy_boss02_momiji_add(s);}
			data->wait1 = 20;
			data->state1 = ST_03;
			break;

		case ST_03: /* warten */
			enemy_boss02_waitstate(s,ST_04);
			break;

		case ST_04: /* nach links bis zum rand */
			if (s->x256 >= t256(0))
			{
				s->x256 -= t256(2)/**fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame--;
						data->wait2=0;
					}
				}
			}
			else
			{
				data->wait1 = 20;
				data->state1 = ST_05;
				if (1==data->state2)
				{	enemy_boss02_momiji_add(s);}
			}
			break;

		case ST_05: /* warten */
			enemy_boss02_waitstate(s,ST_06);
			break;

		case ST_06: /* nach rechts bis zur mitte */
			if (s->x256 < data->xp256)
			{
				s->x256 += t256(2)/**fps_fa ctor*/;
				if (s->anim_frame<8)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame++;
						data->wait2=0;
					}
				}
			}
			else
			{
				enemy_boss02_sr_add1(s);
				data->wait1 = 100;
				data->state1 = ST_07;
				if (1==data->state2)
				{	enemy_boss02_momiji_add(s);}
			}
			break;

		case ST_07: /* warten */
			enemy_boss02_waitstate(s,ST_08);
			break;

		case ST_08:
			if (0==data->state2)
			{	enemy_boss02_ice_add(s);}
			data->wait1 = 100;
			data->state1 = ST_09;
			break;

		case ST_09: /* warten */
			enemy_boss02_waitstate(s,ST_10);
			break;

		case ST_10: /* nach rechts bis zum rand */
			if (s->x256 < (t256(GAME_WIDTH)-((s->w128+s->w128))))
			{
				s->x256 += t256(2)/**fps_fa ctor*/;
				if (s->anim_frame<8)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame++;
						data->wait2=0;
					}
				}
			}
			else
			{
				data->wait1 = 20;
				data->state1 = ST_11;
				if (data->state2==1)
				{	enemy_boss02_momiji_add(s);}
			}
			break;

		case ST_11: /* warten */
			enemy_boss02_waitstate(s,ST_12);
			break;

		case ST_12: /* nach links bis zur mitte */
			if (s->x256 > data->xp256)
			{
				s->x256 -= t256(2)/**fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame--;
						data->wait2=0;
					}
				}
			}
			else
			{	data->state1 = ST_02; }
			break;
		}
		break;

	case 2: 	/* 攻撃パターン2 */
		switch (data->state1)
		{
		case SS00:	/* 不定:初期位置情報の取得->SS01へ */
			data->angle512/*_radian*/=atan_512(t256(20)-s->y256,t256(GAME_WIDTH/2)-s->x256-((s->w128+s->w128)));
			data->state1 = SS01;
			break;

		case SS01:	/* 不定:初期位置へ戻る->SS02へ */
			if (s->y256 < t256(30))
			{
				data->state1 = SS02;
				data->wait1 += 10+(3-difficulty)*10;
/*
[0] 1  2 [0] 1	2 [0] 1  2 [0] 1  2 				/ 3回に一回
<0> 1  2  3 [0] 1  2  3 [0] 1  2  3 				/ 4回に一回
<0> 1  2  3  4	5  6  7  8	9 10 11 				/ 12回に一回以外
[-][-][-][-][+][+][+][+][-][-][-][-][+][+][+][+]
[0] 1  2 [0] 1	2 [0] 1 [0] 1  2 [0] 1	2 [0] 1 	/ 3回に一回
 0 [1] 2  3  0 [1] 2  3  0 [1] 2  3  0 [1] 2  3 	/ 4回に一回
 0	1  2  3  4	5  6  7  8	9 10 11 12 13 14 15 	/ 16回
*/
				{	static const Uint8 n_wait3_tbl[8/*16*/] =
					{ 1,2,0,1, 0,2,1,0, /*1,2,0,1, 0,2,1,0,*/ };
					/* 3回に一回   (0 == ((data->n_wait3)%3)) */
					data->n_wait3 &= 0x07;
					if (n_wait3_tbl[data->n_wait3]&0x01)
					{
						/* 弾に毒塗ってある設定 */
						bullet_create_n_way_dan_sa_type(s, t256(5), ANGLE_JIKINERAI_KISUDAN, (int)(512/24), BULLET_UROKO14_00_AOI+(ra_nd()&0x0f),	 8);	/*なるべく共通化*/
					}
					/* 4回に一回   (0 == ((data->n_wait3)%4)) && */
					/* 但し12回に一回以外  (0 != ((data->n_wait3)%12)) */
					if (n_wait3_tbl[data->n_wait3]&0x02)
					{
						/* 弾に毒塗ってある設定 */
						bullet_create_n_way_dan_sa_type(s, t256(4), ANGLE_JIKINERAI_KISUDAN, (int)(512/24), BULLET_UROKO14_00_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_06_AOI*/,	11);	/*なるべく共通化*/
						bullet_create_n_way_dan_sa_type(s, t256(3), ANGLE_JIKINERAI_KISUDAN, (int)(512/24), BULLET_UROKO14_00_AOI+(ra_nd()&0x0f)/*BULLET_KUNAI12_07_MIDORI*/, 11);	/*なるべく共通化*/
					}
				}
			}
			else
			{
				s->x256 += ((cos512((data->angle512/*_radian*/))*t256(6))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->angle512/*_radian*/))*t256(6))>>8)/**fps_fa ctor*/;
				data->state1 = SS00;
			}
			break;

		case SS02:	/* 初期位置:大玉3つ->SS03, SS04, SS05 */
			if (data->wait1<0)
			{	int angle_jikinerai_512;
				angle_jikinerai_512=atan_512(player->y256-s->y256,player->x256-s->x256);
				bullet_create_oodama0(s, t256(5), angle_jikinerai_512-deg_360_to_512(30), 1, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/ //大弾の追加
				bullet_create_oodama0(s, t256(5), angle_jikinerai_512					, 1, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
				bullet_create_oodama0(s, t256(5), angle_jikinerai_512+deg_360_to_512(30), 1, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
			//	data->state1 = (ra_nd()%3)+SS03;
				data->state1 = (((ra_nd()&(256-1))*3)>>8)+SS03;
				if (data->state1 == SS05)
				{	data->angle512/*_radian*/=angle_jikinerai_512;}
				else
				{
					s->y256 += t256(6)/**fps_fa ctor*/;
				}
			}
			else
			{
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;

		case SS03:	/* 左移動中->SS06へ */
			if (s->x256 >= 0)
			{
				s->x256 -= (t256(4)+(difficulty<<8))/**fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame--;
						data->wait2=0;
					}
				}
			}
			else
			{
				s->x256 += t256(1); 		// [***090114		追加
				data->wait1 = 10;
				data->state1 = SS06;
			}
			break;

		case SS04:	/* 右移動中->SS08へ */
			if (s->x256 < (t256(GAME_WIDTH)-((s->w128+s->w128))))
			{
				s->x256 += (t256(4)+((difficulty)<<8))/**fps_fa ctor*/;
				if (s->anim_frame<8)
				{
					data->wait2 += 1/*fps_fa ctor*/;
					if (data->wait2>=3)
					{
						s->anim_frame++;
						data->wait2=0;
					}
				}
			}
			else
			{
				s->x256 -= t256(1); 		// [***090114		追加
				data->wait1 = 10;
				data->state1 = SS08;
			}
			break;

		case SS05:	/* プレイヤー位置付近移動中->SS10へ */
			if (((s->x256 < t256(0))||(s->y256 < t256(0)))||
				((s->x256+((s->w128+s->w128)) > t256(GAME_WIDTH))||
				 (s->y256+((s->h128+s->h128)) > t256(GAME_HEIGHT))))
			{
				data->state1 = SS10;
			}
			else
			{
				s->x256 += ((cos512((data->angle512/*_radian*/))*(t256(4)+((difficulty)<<8)))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->angle512/*_radian*/))*(t256(4)+((difficulty)<<8)))>>8)/**fps_fa ctor*/;
			}
			break;

		case SS06:	/* 左:待機->SS07へ */
			enemy_boss02_waitstate(s,SS07);
			break;

		case SS07:	/* 左:右回転攻撃->SS00, SS11 */
			enemy_boss02_sr_add1(s);
			if (0==(ra_nd()&(2-1)))
			{
				data->state1 = SS00;
				data->n_wait3++;
			}
			else
			{	data->state1 = SS11;}
			break;

		case SS08:	/* 右:待機->SS09へ */
			enemy_boss02_waitstate(s,SS09);
			break;

		case SS09:	/* 右:左回転攻撃->ひとまずSS00へ */
			enemy_boss02_sr_add2(s);
			data->state1 = SS00;
			data->n_wait3++;
			break;

		case SS10:	/* プレイヤー位置付近:大玉3つ->ひとまずSS00へ */
			{int angle_jikinerai_512;
				angle_jikinerai_512=atan_512(player->y256-s->y256,player->x256-s->x256);
				bullet_create_oodama0(s, t256(3), angle_jikinerai_512-deg_360_to_512(30), 20, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
				bullet_create_oodama0(s, t256(3), angle_jikinerai_512					, 20, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
				bullet_create_oodama0(s, t256(3), angle_jikinerai_512+deg_360_to_512(30), 20, 0, ((s->w128*86*2)>>8), (s->h128)); /*t256(s->w/3), t256(s->h/2)*/
			}
			data->state1 = SS00;
			data->n_wait3++;
			break;

		case SS11:	/* 左:前へ->SS12 */
			if (s->y256 > t256(GAME_HEIGHT/3) )
			{	data->state1 =SS12;}
			else
			{	s->y256 += (t256(5)+((difficulty)<<8))/**fps_fa ctor*/;}
			break;

		case SS12:
			{
				/* 弾に毒塗ってある設定 */
				bullet_create_n_way_dan_sa_type(s, t256(5), ANGLE_JIKINERAI_KISUDAN, deg_360_to_512(30), BULLET_UROKO14_00_AOI+(ra_nd()&0x0f),	 5);	/*なるべく共通化*/
			//	int angle_jikinerai_512;
			//	angle_jikinerai_512=atan_512(player->y256-s->y256,player->x256-s->x256);
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512+deg_360_to_512(60),	2/*state_hi*/, 0/*state_lo*/);		//いらない弾達。消した方がいいかも
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512+deg_360_to_512(30),	1/*state_hi*/, 0/*state_lo*/);
			//	bullet_create_aka_maru_jikinerai(s, t256(5));
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512-deg_360_to_512(30), -1/*state_hi*/, 0/*state_lo*/);
			//	bullet_create_gg_dan(s, t256(5), angle_jikinerai_512-deg_360_to_512(60), -2/*state_hi*/, 0/*state_lo*/);
			}
			data->state1 = SS00;
			data->n_wait3++;
			break;
		}
	}
//	あとでコールバックにして共通化する。
	{
		data->boss_base.boss_timer -= 1/*fps_fa ctor*/;
		#if 1/*時間切れ(色々問題あるのでとりあえず無効)*/
		if ((data->boss_base.boss_timer < 1))
		{
			data->boss_base.boss_timer = (60*64);
		//	if ((data->state1 < SAKUYA_05_KEITAI/*4*/))
		//	{
		//		//レミリア戦の開始イベントが見れなくなっちゃうのでここは無効
		//		#if (0==USE_REMILIA)
		//		/* vs REMILIA */
		//		if (2==select_player)
		//		{data->boss_base.health=0/*1*/;}
		//		else
		//		#endif
		//		{data->boss_base.health 	= ((/*4*/SAKUYA_05_KEITAI*1024)-1)-(data->state1<<10/**1024*/);}
		//		sakuya_put_items(/*c,*/s);
		//	}
		}
		#endif
	}
//
}


/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_aya(STAGE_DATA *l)/*int lv*/
{
	SPRITE *s;
//	s					= spr ite_add_file 0("boss/aya.png"/*"boss02_v2.png"*/,9,PRIORITY_03_ENEMY, 0); s->anim_speed=0;
	s					= sprite_add_res(BASE_BOSS_AYA_PNG);
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s->anim_frame		= 4;
	s->type 			= SP_BOSS/*SP_BOSS02*/;
	BOSS02_DATA *data;
	data				= mmalloc(sizeof(BOSS02_DATA));
	s->data 			= data;
	data->boss_base.health		= (difficulty<<10/**1024*/)+3071+1024;
//	data->boss_base.score		= score(2000)+score(1500)*difficulty;
	data->boss_base.score		= adjust_score_by_difficulty(score(2000000));	/* 200万 */
	data->state1		= 0;
	data->state2		= 0;
	data->wait1 		= 50;
	data->wait2 		= 0;
//	data->level 		= (l->user_y);
	data->n_wait3		= 1;
	s->callback_mover	= enemy_boss02_move;
	s->x256 			= t256(GAME_WIDTH/2)-(s->w128);
	data->xp256 		= s->x256;
	s->y256 			= -(s->h128+s->h128);
	//
	data->boss_base.boss_timer		= 60*64;	/*	[] */
//	((PLAYER_DATA *)player->data)->bo ssmode=B00_NONE/*B01_BA TTLE*/;
	((PLAYER_DATA *)player->data)->boss=s;
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss02_hitbyweapon;
}
