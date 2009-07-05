
#include "enemy.h"

/*---------------------------------------------------------
	十六夜 咲夜
	PAD
	-------------------------------------------------------
	ボスタイマー対応中
---------------------------------------------------------*/

/* 1:レミと咲夜が弾幕練習をする。0:レミと咲夜は戦わない。 */
#define USE_REMILIA (1)
extern int select_player;

typedef struct _boss05_data
{
	BOSS_BASE boss_base;
	int state1; 	//形態
	int state2; 	//行動
//
	/*dou ble*/int wait1;
	/*dou ble*/int wait2_256;
	/*dou ble*/int wait3;
//	int level;
	/*dou ble*/int move_angle512;
	/*dou ble*/int angle512;
	/*dou ble*/int length_s_p256;	/* 咲夜とプレイヤーとの距離 */
} BOSS05_DATA;
//	int aaa_type;		/* 0: normal, 1: destroyed */

typedef struct _boss05_maho_data
{
	SPRITE *c;
	int state01;	//形態
	int state02;	//行動
	int nnn;
	int mmm;
	/*dou ble*/int wait;
	/*dou ble*/int angle512;
} BOSS05_MAHO_DATA;

enum
{
/*0*/	SAKUYA_01_KEITAI = 0,	// 第一形態: 左右に動いて小弾撃ち
/*1*/	SAKUYA_02_KEITAI,		// 第二形態: 全方位、豆まき
/*2*/	SAKUYA_03_KEITAI,		// 第三形態: 垂直ナイフが落ちてくるよ
/*3*/	SAKUYA_04_KEITAI_A, 	// 第四形態: (黄色マスカット弾)
		SAKUYA_04_KEITAI_B, 	// 第四形態: (黄色マスカット弾)
//	最終形態
/*4*/	SAKUYA_05_KEITAI,		// 第五形態: 最終形態(その1)
/*5*/	SAKUYA_06_KEITAI,		// 第六形態: 最終形態(その2)
};

/*---------------------------------------------------------

---------------------------------------------------------*/

static void callback_enemy_boss05_hitbyweapon_dummy(SPRITE *s, SPRITE *t/*, int angle*/)
{	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
}

static void sakuya_put_items(SPRITE *s/*, SPRITE *t*/)
{
	play_voice_auto_track(VOICE07_BOMB);
//++	((PLAYER_DATA *)player->data)->bo ssmode=B04_CHANGE;
	item_from_bullets(SP_ITEM_05_HOSI);
	item_create_for_boss(s, ITEM_CREATE_MODE_02);
	//t->anim_frame 		= 4;//バグ(ここでプレイヤーショットのアニメパターンを弄るので、レミボムでハングアップする)
	s->anim_frame		= 4;//咲夜のアニメパターンを弄る
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	data->state1++;
	data->state2			= 0;
	data->wait1 		= 80;
	data->wait2_256 	= t256(-100);
	data->wait3 		= 0;//???
	data->boss_base.boss_timer		= (60*64);	// 60 [秒] ???	4096/*2500*/	/*3000*/;//???
}

/*---------------------------------------------------------
		s = ボス本体 boss sprite
		t = プレイヤーの弾 player's weapon
---------------------------------------------------------*/

static void callback_enemy_boss05_hitbyweapon(SPRITE *s, SPRITE *t/*, int angle*/)
{
	explosion_add_type(t->x256,t->y256,/*0,*/EXPLOSION_MINI00);/*先に実行した方が速い*/
	//
	BOSS05_DATA *data	= (BOSS05_DATA *)s->data;
	WEAPON_BASE *w		= (WEAPON_BASE *)t->data;

	data->boss_base.health -= w->strength;
	if (data->boss_base.health < (/*4*/SAKUYA_05_KEITAI-data->state1)*1024)
	{
		sakuya_put_items(s/*,t*/);
	}

	if (data->boss_base.health < 1)/*全員倒した？*/
	{
		item_create(s, SP_ITEM_05_HOSI/*SP_ITEM_06_TENSU*/, 15, ITEM_MOVE_FLAG_01_COLLECT);/*星点を出す*/
		player_add_score(data->boss_base.score);
		//
		player_set_destoroy_boss();
		explosion_add_circle(s, 1); 	/* player_set_destoroy_boss();B02_BOSS_DESTROY が必要 */
		s->type 	= SP_DELETE;
		/* コールバック登録 */
		((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon = callback_enemy_boss05_hitbyweapon_dummy; /* ダミーコールバック登録 */
	}
}

/*static*/extern void enemy_sp2_bullet_create(SPRITE *s, int angle512, /*dou ble*/int gra256, int *sd_nnn);
/*static*/extern void enemy_sp1_bullet_create(SPRITE *s, /*dou ble*/int speed256, /*dou ble*/int angle512, /*dou ble*/int gra256, int r_or_l, int *sd_angle512);


/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_waitstate(SPRITE *s, int nextstate, int anim_frame)
{
	BOSS05_DATA *data = (BOSS05_DATA *)s->data;
	if (data->wait1>0)
	{	data->wait1 -= 1/*fps_fa ctor*/;}
	else
	{
		data->state2 = nextstate;
		s->anim_frame=anim_frame;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_out(SPRITE *s)
{
	if (s->x256 < t256(50)) 								{	s->x256 = t256(50);}
	if (s->x256 > t256(GAME_WIDTH)-t256(50)-((s->w128+s->w128)))	{	s->x256 = t256(GAME_WIDTH)-t256(50)-((s->w128+s->w128));}
	if (s->y256 < t256(10)) 								{	s->y256 = t256(10);}
	if (s->y256 > t256(GAME_HEIGHT)-t256(120)+((s->h128+s->h128)))	{	s->y256 = t256(GAME_HEIGHT)-t256(120)+((s->h128+s->h128));}
}

static void enemy_boss05_more_angle(SPRITE *s, /*dou ble*/int speed256, /*dou ble*/int angle4096, /*dou ble*/int a_angle4096, int k)
{
	if (0 == k) { a_angle4096 = -a_angle4096;	}
	enemy_angle_bullet_create(s, speed256, angle4096,						 a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096(11*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096(10*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 9*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 8*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 7*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 6*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 5*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 4*30), a_angle4096);
}
static void bullet_create_mamemaki(SPRITE *s, int fff, int k)
{
	enemy_boss05_more_angle(s, t256((2.5-0.3)*0.6)+(fff),	/*angle4096*/( 4*256), rad2deg4096(0.013),	k);
	enemy_boss05_more_angle(s, t256((2.5-0.6)*0.6)+(fff),	/*angle4096*/( 2*256), rad2deg4096(0.011),	k);
	enemy_boss05_more_angle(s, t256((2.5-0.9)*0.6)+(fff),	/*angle4096*/( 0*256), rad2deg4096(0.009),	k);
	enemy_boss05_more_angle(s, t256((2.5-1.2)*0.6)+(fff),	/*angle4096*/(14*256), rad2deg4096(0.007),	k);
	enemy_boss05_more_angle(s, t256((2.5-1.5)*0.6)+(fff),	/*angle4096*/(12*256), rad2deg4096(0.005),	k);
	if (difficulty>1)
	{
		enemy_boss05_more_angle(s, t256((2.5-1.8)*0.6)+(fff), /*angle4096*/(10*256), rad2deg4096(0.003),  k);
		enemy_boss05_more_angle(s, t256((2.5-2.1)*0.6)+(fff), /*angle4096*/( 8*256), rad2deg4096(0.001),  k);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_knifes1(SPRITE *s, /*dou ble*/int speed256, int angle512, int height)
{
	//アングルは角度指定で-90～90
	//heightは"playerから"の高さ
	enemy_follow_knife_create1(s, speed256, /*deg_360_to_512*/(deg_360_to_512(180)-angle512), height);
	enemy_follow_knife_create1(s, speed256, /*deg_360_to_512*/(angle512), height);
}
#if 1/*魔方陣用*/
static void enemy_boss05_knifes2(SPRITE *s, /*dou ble*/int speed256)
{
	#if 0
	enemy_knife_create(s, speed256, 				  deg_360_to_512( 0   ),			   0/* 0*/);
	enemy_knife_create(s, speed256, /*( M_PI*(1/9))*/ deg_360_to_512( 1*20)/*( 0.349)*/,   1/*17*/);
	enemy_knife_create(s, speed256, /*( M_PI*(2/9))*/ deg_360_to_512( 2*20)/*( 0.698)*/,   2/*16*/);
	enemy_knife_create(s, speed256, /*( M_PI*(3/9))*/ deg_360_to_512( 3*20)/*( 1.047)*/,   3/*15*/);
	enemy_knife_create(s, speed256, /*( M_PI*(4/9))*/ deg_360_to_512( 4*20)/*( 1.396)*/,   4/*14*/);
	enemy_knife_create(s, speed256, /*( M_PI*(5/9))*/ deg_360_to_512( 5*20)/*( 1.745)*/,   5/*13*/);
	enemy_knife_create(s, speed256, /*( M_PI*(6/9))*/ deg_360_to_512( 6*20)/*( 2.094)*/,   6/*12*/);
	enemy_knife_create(s, speed256, /*( M_PI*(7/9))*/ deg_360_to_512( 7*20)/*( 2.443)*/,   7/*11*/);
	enemy_knife_create(s, speed256, /*( M_PI*(8/9))*/ deg_360_to_512( 8*20)/*( 2.792)*/,   8/*10*/);
	enemy_knife_create(s, speed256, /*( M_PI*(9/9))*/ deg_360_to_512( 9*20)/*( 3.141)*/,   9/* 9*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(8/9))*/ deg_360_to_512(10*20)/*(-2.792)*/,  10/* 8*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(7/9))*/ deg_360_to_512(11*20)/*(-2.443)*/,  11/* 7*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(6/9))*/ deg_360_to_512(12*20)/*(-2.094)*/,  12/* 6*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(5/9))*/ deg_360_to_512(13*20)/*(-1.745)*/,  13/* 5*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(4/9))*/ deg_360_to_512(14*20)/*(-1.396)*/,  14/* 4*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(3/9))*/ deg_360_to_512(15*20)/*(-1.047)*/,  15/* 3*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(2/9))*/ deg_360_to_512(16*20)/*(-0.698)*/,  16/* 2*/);
	enemy_knife_create(s, speed256, /*(-M_PI*(1/9))*/ deg_360_to_512(17*20)/*(-0.349)*/,  17/* 1*/);
	#else
	{
		int angle512;
		angle512 = 0;
	//	/* vs REMILIA  レミの場合、やるきなし */
	//	if (2==select_player)
	//	{
	//		angle512 += 256;
	//	}
		int i;
		for (i=0; i<16/*18*/; i++)
		{
			enemy_knife_create(s, (speed256), (angle512), i/*anime_pattern*/);
			angle512 += 32/*29*/;
	//		mask512(angle512);
		}
	}
	#endif
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 1
/* プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾 */
static void enemy_boss05_knifes3(SPRITE *s/*,*/ /*dou ble speed,*/ /*dou ble*/ /*int length222*/)
{
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	/*dou ble*/int len256;
	/*dou ble*/int tmp1_256;
	/*dou ble*/int tmp2_256;
	int tmp888;
	tmp888=0;
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	len256 = t256(/*70*/80) -((difficulty)<<8) /*-length222*/  /*/2*/;
	data->wait2_256 = t256(0);
	while (1)
	{
		tmp888 = (((int)ra_nd()+(t256_floor(data->wait2_256)) )&(8-1)/*%8*/);
		data->wait2_256 += t256(1);
		len256 += t256(0.1);
		tmp1_256 = player->x256 + ((player->w128-s->w128)) + ((cos512((tmp888<<6))*len256)>>8);
		tmp2_256 = player->y256 + ((player->h128-s->h128)) + ((sin512((tmp888<<6))*len256)>>8);
		if ((( t256(30) < tmp1_256) && (tmp1_256 < t256(GAME_WIDTH-30)))&&
			(( t256(30) < tmp2_256) && (tmp2_256 < t256(GAME_HEIGHT-72))))
		{	break;}
	}
	/*dou ble*/int tmp_x256;
	/*dou ble*/int tmp_y256;
	tmp_x256 = player->x256 + ((player->w128));
	tmp_y256 = player->y256 + ((player->h128));
	int i;
	for (i=0; i<8; i++)
	{
		if (tmp888==i)
		{
			data->angle512	= (i<<6);
			s->x256 		= tmp1_256;
			s->y256 		= tmp2_256;
			s->alpha		= 0;
			s->anim_frame	= 18;
			data->state1		= SAKUYA_06_KEITAI/*5*/;/*???*/
			data->wait2_256 	= /*speed256*/t256(1)+(difficulty<<8);
		}
		else
		{
		//	dou ble tmp_s_x;
		//	dou ble tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* プレイヤー狙い弾 */
			s->x256 		=	tmp_x256 + ((cos512((i<<6))*len256)>>8);
			s->y256 		=	tmp_y256 + ((sin512((i<<6))*len256)>>8);
			enemy_follow_knife_create1/*2*/(s,
				/*speed256*/t256(2),
				(i<<6)/*i*M_PI*2*1/8*/, 							/*angle512*/
				-30+(i&(2-1)/*%2*/)*60);							/* -30 or 30 */
		//	s->x = tmp_s_x;
		//	s->y = tmp_s_y;
		}
	}
}
#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_maho_move(SPRITE *s)
{
	BOSS05_MAHO_DATA *d=(BOSS05_MAHO_DATA *)s->data;
	if (d->state01<5)
	{
		s->x256 = d->c->x256+((d->c->w128-s->w128));
		s->y256 = d->c->y256+((d->c->h128-s->h128));
		if (((BOSS05_DATA *)d->c->data)->state1==SAKUYA_05_KEITAI/*4*/ /*星型を描いてもいいよ*/)
		{
			d->state01=5;/*星型を描くぜ*/
		}
	}
	switch (d->state01)
	{
//------------------------------------------
	case 0:
		s->alpha += 3;
		if (150 < s->alpha)
		{
			s->alpha=150;
			d->state01=1;
			d->wait=50;
		}
		break;

	case 1:
		d->wait -= 1/*fps_fa ctor*/;
		if (d->wait < 1)
		{
			d->state01=2;
			d->state02=0x100;
		}
		break;
	case 2:
		if (0==(d->state02&0x0f/*%10*/))
		{
			bullet_create_tomari2_dan(s, (d->state02/*>>4*/<<3/*1/10*/)/**256*/, d->angle512,					  t256(0.02), d->angle512+(d->state02)-deg_360_to_512(180	 ));	//ra d2deg512((d->state02)>>7/*/100*/)
			bullet_create_tomari2_dan(s, (d->state02/*>>4*/<<3/*1/10*/)/**256*/, d->angle512+deg_360_to_512(120), t256(0.02), d->angle512+(d->state02)-deg_360_to_512(180+120));	//ra d2deg512((d->state02)>>7/*/100*/)
			bullet_create_tomari2_dan(s, (d->state02/*>>4*/<<3/*1/10*/)/**256*/, d->angle512-deg_360_to_512(120), t256(0.02), d->angle512+(d->state02)-deg_360_to_512(180-120));	//ra d2deg512((d->state02)>>7/*/100*/)
		}
		d->state02--;
		if (d->state02 < 1)
		{
			d->state01=3;
			d->wait=15;
			d->state02=0x10;
		}
		break;
	case 3:
		d->wait -= 1/*fps_fa ctor*/;
		if (d->wait < 1)
		{
			d->wait=12;
			play_voice_auto_track(VOICE13_SAKUYA_SHOT02);
			{int i;
				for (i=t256(5); i<t256(9); i+=t256(1))
				{
					bullet_create_hari_dan180(s, i, d->angle512,					 t256(0), t256(0)	);
					bullet_create_hari_dan180(s, i, d->angle512+deg_360_to_512(120), t256(0), t256(0)	);
					bullet_create_hari_dan180(s, i, d->angle512-deg_360_to_512(120), t256(0), t256(0)	);
				}
			}
			/* deg_360_to_512(8.59) 8.59436717317284029518323968680208	PI*2/41.8879008478638666666666666666667 ra d2deg512(0.15)*/
			/* 12.2230999796235950864828297767852 == 512/41.8879008478638666666666666666667 */
			d->angle512 += 12;
			d->state02--;
		}
		if (d->state02<0x0)
		{
			d->state01=2;
			d->state02=0x50;
		}
		break;
#if 1
//------------------------------------------
	/*
	星型線1 190, 48
	星型線2 248, 217
	星型線3 100, 113
	星型線4 280, 113
	星型線5 138, 217
	*/
	case 5: 	//星型を描くよ
		s->anim_frame=0;
		if (((BOSS05_DATA *)d->c->data)->state2==1)
		{
			d->angle512=atan_512(t256(48)-s->y256-((s->h128)), t256(190)-s->x256-((s->w128)));/*星型線1*/
			d->state01=6;
			d->state02=0x0;
		}
		break;
	case 6:
		if (s->y256+((s->h128)) <= t256(48))
		{
			d->state01=7;
			d->angle512=atan_512(t256(217)-s->y256-((s->h128)), t256(248)-s->x256-((s->w128)));/*星型線2*/
			d->wait=50;
			d->nnn++;	if (d->nnn>(/*4-0*/3/*difficulty*/) )	{d->nnn=1/*0*/;}
			d->mmm++;	d->mmm &= 1;
			d->state02=0x0;
			if (2==d->nnn/*%(4-difficulty)*/)/* %2 == (5-3:Lunatic) */
			{	s->anim_frame=1;}
			else
			{	s->anim_frame=0;}
		}
		else
		{
			s->x256 += ((cos512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(3)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015), &(((BOSS05_MAHO_DATA *)s->data)->nnn) );
				}
			}
		}
		break;
	case 7:
		enemy_boss05_knifes2(s, t256(1.2) );
		d->state01=8;
		break;
	case 8:
		if (d->wait<0)
		{	d->state01=9;}
		else
		{	d->wait--;}
		break;

	case 9:
		if (s->y256+((s->h128)) >= t256(217))
		{
			d->state01=10;
			d->angle512=atan_512(t256(113)-s->y256-((s->h128)), t256(100)-s->x256-((s->w128)));/*星型線3*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x256 += ((cos512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(147)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015), &(((BOSS05_MAHO_DATA *)s->data)->nnn) );
				}
			}
		}
		break;
	case 10:
		enemy_boss05_knifes2(s, t256(1.2) );
		d->state01=11;
		break;
	case 11:
		if (d->wait<0)
		{	d->state01=12;}
		else
		{	d->wait--;}
		break;

	case 12:
		if (s->y256+((s->h128)) <= t256(113))
		{
			d->state01=13;
			d->angle512=atan_512(t256(113)-s->y256-((s->h128)), t256(280)-s->x256-((s->w128)));/*星型線4*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x256 += ((cos512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(291)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015), &(((BOSS05_MAHO_DATA *)s->data)->nnn) );
				}
			}
		}
		break;
	case 13:
		enemy_boss05_knifes2(s, t256(1.2) );
		d->state01=14;
		break;
	case 14:
		if (d->wait<0)
		{	d->state01=15;}
		else
		{	d->wait--;}
		break;

	case 15:
		if (s->x256+((s->w128)) >= t256(280))
		{
			d->state01=16;
			d->angle512=atan_512(t256(217)-s->y256-((s->h128)), t256(138)-s->x256-((s->w128)));/*星型線5*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x256 += ((cos512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(75)+((( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/)), t256(0.015), &(((BOSS05_MAHO_DATA *)s->data)->nnn) );
				}
			}
		}
		break;
	case 16:
		enemy_boss05_knifes2(s, t256(1.2) );
		d->state01=17;
		break;
	case 17:
		if (d->wait<0)
		{	d->state01=18;}
		else
		{	d->wait--;}
		break;

	case 18:
		if (s->y256+((s->h128)) >= t256(217) )
		{
			d->state01=19;
			d->angle512=atan_512(t256(48)-s->y256-((s->h128)), t256(190)-s->x256-((s->w128)));/*星型線1*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x256 += ((cos512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((d->angle512))*t256(3/*+difficulty*/))>>8)/**fps_fa ctor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(219)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015), &(((BOSS05_MAHO_DATA *)s->data)->nnn) );
				}
			}
		}
		break;
	case 19:
		enemy_boss05_knifes2(s, t256(1.2) );
		d->state01=20;
		break;
	case 20:
		if (d->wait<0)
		{
			d->state01=6;
			d->state02=0x0;
		}
		else
		{	d->wait--;}
		break;
#endif
	}
	if (d->c->type==SP_DELETE)
	{	s->type=SP_DELETE;}
	#if 0/*魔方陣デバッグ用*/
	/* パネルのスコア欄にstate01を、グレイズ欄にstate02を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= d->state01;
	((PLAYER_DATA *)player->data)->graze_point	= d->state02;
	#endif
}

static void enemy_boss05_maho_create(SPRITE *s) 	//魔方陣グラフィック生成
{
	SPRITE *s2; 		// 魔方陣グラフィックのスプライト
	s2					= sprite_add_res(BASE_MAHOUJIN_0_PNG);	//s2->anim_speed	= 0;/*"boss04-lo.png"*/
	s2->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s2->anim_frame		= 0;
	s2->type			= SP_PLAYER2;
	BOSS05_MAHO_DATA *data;
	data				= mmalloc(sizeof(BOSS05_MAHO_DATA));
	s2->data			= data;
	data->state01		= 0;
	data->state02		= 0x0;
	data->wait			= 0;
	data->angle512		= (0);
	data->c 			= s;
	data->nnn			= 0;
	data->mmm			= 0;
	s2->alpha			= 0;
	s2->callback_mover	= enemy_boss05_maho_move;
	s2->x256			= s->x256+((s->w128-s2->w128));
	s2->y256			= s->y256+((s->h128-s2->h128));
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_bullet_create_lines(SPRITE *s)
{
//
//	int i;
//	for (i=0;i<difficulty;i++)
	bullet_create_aka_maru_jikinerai(s, t256(2.5));
	bullet_create_aka_maru_jikinerai(s, t256(2.0));
	bullet_create_aka_maru_jikinerai(s, t256(3.5));
}
#define SAKUYA_LIMIT_MIN_Y (t256(8))
#define SAKUYA_LIMIT_MAX_Y (t256(GAME_HEIGHT-32))

/* 咲夜さんの誘導ポイント座標 */

#define SAKUYA_POINT_X_LEFT 	(t256(0 					))
#define SAKUYA_POINT_X_MID		(t256(GAME_WIDTH/2/*-s->w/2*/))
#define SAKUYA_POINT_X_RIGHT	(t256(GAME_WIDTH  /*-s->w  */))
#define SAKUYA_POINT_X_MARGIN	(t256(16))

#define SAKUYA_POINT_Y_LEFT 	(t256(50))
#define SAKUYA_POINT_Y_MID		(t256(10))
#define SAKUYA_POINT_Y_RIGHT	(t256(50))
#define SAKUYA_POINT_Y_CENTER	(t256(50))
#define SAKUYA_POINT_Y_MARGIN	(t256(16))

#define SAKUYA_LIMIT_X_LEFT 		(SAKUYA_POINT_X_LEFT +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_LEFT 	(SAKUYA_POINT_X_MID  +SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_MID_RIGHT	(SAKUYA_POINT_X_MID  -SAKUYA_POINT_X_MARGIN)
#define SAKUYA_LIMIT_X_RIGHT		(SAKUYA_POINT_X_RIGHT-SAKUYA_POINT_X_MARGIN)

#define atan_512_SAKUYA_LEFT	atan_512( (SAKUYA_POINT_Y_LEFT) 	-s->y256, (SAKUYA_POINT_X_LEFT) -s->x256)
#define atan_512_SAKUYA_MID 	atan_512( (SAKUYA_POINT_Y_MID)		-s->y256, (SAKUYA_POINT_X_MID)	-s->x256)
#define atan_512_SAKUYA_RIGHT	atan_512( (SAKUYA_POINT_Y_RIGHT)	-s->y256, (SAKUYA_POINT_X_RIGHT)-s->x256)
#define atan_512_SAKUYA_CENTER	atan_512( (SAKUYA_POINT_Y_CENTER)	-s->y256, (SAKUYA_POINT_X_MID)	-s->x256)


/* 咲夜さんの誘導ポイント名称 */
enum
{
	SAKUYA_POINT_00_LEFT=0, 	/*左下*/
	SAKUYA_POINT_01_MID_UP, 	/*中上*/
//	SAKUYA_POINT_02_CENTER, 	/*画面中心*/
	SAKUYA_POINT_03_RIGHT,		/*右下*/
};


/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_sakuya(SPRITE *s, int speed256, int target_point_name)
{
	int b_move_angle512;
	switch (target_point_name)
	{
	case SAKUYA_POINT_00_LEFT:		b_move_angle512 = atan_512_SAKUYA_LEFT;/*50*/		break;
	case SAKUYA_POINT_01_MID_UP:	b_move_angle512 = atan_512_SAKUYA_MID;				break;
//	case SAKUYA_POINT_02_CENTER:	b_move_angle512 = atan_512_SAKUYA_CENTER;		break;
	case SAKUYA_POINT_03_RIGHT: 	b_move_angle512 = atan_512_SAKUYA_RIGHT;/**/	break;
	}
	s->x256 += ((cos512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
	s->y256 += ((sin512((b_move_angle512))*(speed256))>>8)/**fps_fa ctor*/;
		 if (s->y256 < (SAKUYA_LIMIT_MIN_Y) )	{	s->y256 += t256(1); 	}/* はみ出したら修正 */
	else if (s->y256 > (SAKUYA_LIMIT_MAX_Y) )	{	s->y256 -= t256(1); 	}/* はみ出したら修正 */
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_boss05_move(SPRITE *s)
{
	BOSS05_DATA *data=(BOSS05_DATA *)s->data;
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	switch (data->state1)
	{
	case SAKUYA_01_KEITAI/*0*/: // 第一形態: 左右に動いて小弾撃ち
		switch (data->state2)
		{
		case 0: 	/* 下がる */
			if (s->y256 < t256(20))
			{
				s->y256 += t256(1)/*fps_fa ctor*/;
			}
			else	// yが20まで来たら
			{
			/* 登場時には弾を撃たない */
			//	enemy_bullet_create_lines(s);
				s->anim_frame=0;
			//	pd->bo ssmode=B05_BE FORE_LOAD;
				pd->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
				data->state2=10;
			}
			data->boss_base.boss_timer		= (60*64);	// 60 [秒] ??? /*時間制限カウント無効化*/
			break;
		/*完全に姿を現す*/
		case 10:	/* */
			data->boss_base.boss_timer		= (60*64);	// 60 [秒] ??? /*時間制限カウント無効化*/
		//	if (pd->bo ssmode==B01_BA TTLE)
		//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
			if ( ((((PLAYER_DATA *)player->data)->state_flag) & STATE_FLAG_05_IS_BOSS) )
			{
				/* プレイヤー弾受け付け、コールバックを登録 */
				((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss05_hitbyweapon;
				#if (0==USE_REMILIA)/* とりあえず昨夜と対戦チェック */
				/* vs REMILIA */
				if (2==select_player)
				{data->boss_base.health=0/*1*/;}
				#endif
			//	/*時間制限カウント有効化*/
				data->state2=1;
			}
			break;
		case 1: 	/* 左 */
			//if (pd->bo ssmode==B01_BA TTLE)
			{
				if (s->x256 >= t256(50) )
				{
					s->y256 += t256(1)/*fps_fa ctor*/;
					s->x256 -= t256(3)/*fps_fa ctor*/;
					if (0 < s->anim_frame)
					{
						data->wait3 += 1/*fps_fa ctor*/;
						if (data->wait3>=5)
						{
							s->anim_frame--;
							data->wait3=0;
						}
					}
				}
				else	//xが50以下になったら
				{
					data->state2=2;
					data->wait1=50;
					enemy_bullet_create_lines(s);
					s->anim_frame=4;
				}
			}
			break;
		case 2: 	/* 待機 */
			enemy_boss05_waitstate(s, 3, 4);
			break;
		case 3: 	/* 真ん中に戻る */
			if (s->y256 > t256(20) )
			{
				s->y256 -= t256(1)/*fps_fa ctor*/;
				s->x256 += t256(3)/*fps_fa ctor*/;
				if (8 > s->anim_frame)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=5)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			else	//真ん中まで来たら
			{
				data->state2=4;
				enemy_bullet_create_lines(s);
			}
			break;
		case 4: 	/* 右 */
			if (s->x256+((s->w128+s->w128)) <= t256(GAME_WIDTH-50) )
			{
				s->y256 += t256(1)/*fps_fa ctor*/;
				s->x256 += t256(3)/*fps_fa ctor*/;
			}
			else
			{
				data->state2=5;
				data->wait1=50;
				enemy_bullet_create_lines(s);
				s->anim_frame=4;
			}
			break;
		case 5: 	/* 待機 */
			enemy_boss05_waitstate(s, 6, 4);
			break;
		case 6:
			if (s->y256 > t256(20) )
			{
				s->y256 -= t256(1)/*fps_fa ctor*/;
				s->x256 -= t256(3)/*fps_fa ctor*/;
				if (s->anim_frame>0)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=5)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			else	/* 真ん中に戻る */
			{
				data->state2=1;
				enemy_bullet_create_lines(s);
			}
			break;
		}
		break;
	//
	case SAKUYA_02_KEITAI/*1*/: // 第二形態: 全方位、豆まき
		//data->wait2_256		ループ回数
		switch (data->state2)
		{
		case 0:
			if (data->wait1<0)
			{
				data->state2=1;
//++				pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
				data->wait2_256 = t256(0);
			}
			else
			{
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		case 1:
			{
				int ang512;
				if (ra_nd()&(2-1)/*%2*/)
				{
					ang512=(ra_nd()&(256-1)/*%(180*1.42)*/);
					if (ang512>deg_360_to_512(90))		{	ang512+=deg_360_to_512(90); 	s->anim_frame=5;	}
					else								{									s->anim_frame=3;	}
					ang512-=deg_360_to_512(45);
				}
				else
				{
					if (s->x256 > t256(GAME_WIDTH/2) )	{	ang512=deg_360_to_512(180); 	s->anim_frame=6;	}
					else								{	ang512=deg_360_to_512(0);		s->anim_frame=2;	}
				}
				data->move_angle512=/*deg512_2rad*/(ang512);
			}
			data->state2=2;
			data->wait1 = 20+(3-difficulty)*10;
			data->wait2_256 += t256(1);
			break;
		case 2:
			if (data->wait1<0)
			{
				data->wait1=35;
				data->state2=3;
				s->anim_frame=4;
			}
			else
			{
				s->x256 += cos512((data->move_angle512))/**fps_fa ctor*/;
				s->y256 += sin512((data->move_angle512))/**fps_fa ctor*/;
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		case 3:
			enemy_boss05_waitstate(s, 4, 15);
			data->wait3=0;
			break;
		case 4:
			if (s->anim_frame==17)
			{
				play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
				{
				//	 ((t256_floor(data->wait2_256))&(2-1)/*%2*/)2083221
					int k=(((data->wait2_256)>>10)&1);
					bullet_create_mamemaki(s, (data->wait2_256>>3/*/10*/), k);
				}
				s->anim_frame=18;
			}
			else if (s->anim_frame==18)
			{
				data->state2=5;
			}
			else
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					s->anim_frame++;/*???*/
					data->wait3=0;
				}
			}
			break;
		case 5:
			if (s->x256 > t256(GAME_WIDTH/2) )
			{	data->move_angle512 = -(int)(512*3/8);}
			else
			{	data->move_angle512 = -(int)(512*1/8);}
				#if 1
				/*念の為*/
				mask512(data->move_angle512);
				#endif
			data->state2=6;
			data->wait1=5;
			break;
		case 6:
			if (data->wait1<0)
			{
				data->wait1=10;
				data->state2=7;
			}
			else
			{
				if (s->y256 > t256(60))
				{
					s->x256 += ((cos512((data->move_angle512))*t256(2))>>8)/**fps_fa ctor*/;
					s->y256 += ((sin512((data->move_angle512))*t256(2))>>8)/**fps_fa ctor*/;
				}
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		case 7:
			if (s->anim_frame>15)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					s->anim_frame--;
					data->wait3=0;
				}
			}
			enemy_boss05_waitstate(s, 8, 15);
			break;
		case 8:
			if ((data->wait2_256>>(8+3))/*%10*/==0)
			{
				if (s->anim_frame==18)
				{
					s->anim_frame=5;
					play_voice_auto_track(VOICE11_SAKUYA_E_SHOT00);
					int k;
					for (k=0;k<2;k++)
					{
						bullet_create_mamemaki(s, (data->wait2_256>>3/*/10*/), k);
					}
					data->state2=1;
				}
				else
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;/*???*/
						data->wait3=0;
					}
				}
			}
			else
			{
				data->state2=1;
				s->anim_frame=16;
			}
			break;
		}
		enemy_boss05_out(s);
		break;
	//
	case SAKUYA_03_KEITAI/*2*/: 	// 第三形態: 垂直ナイフが落ちてくるよ
		switch (data->state2)
		{
		case 0:
			if (data->wait1<0)	// 待機時間が終わったら
			{
				data->state2		= 1;
//++				pd->bo ssmode		= B00_NONE/*B01_BA TTLE*/;
				s->anim_frame		= 4;
			}
			else
			{
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		case 1:
			if (s->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/ )	// 左隅に来たら 	/* → || (s->y<5)があると無限ループになる*/
			{
				data->state2=2;
				data->wait1=10;
				data->wait2_256=t256(0);
				s->anim_frame=0;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_00_LEFT);
				if (s->anim_frame>0)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			break;
		case 2:
			enemy_boss05_waitstate(s, 3, 2);
			break;
		case 3:
			if (s->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-s->w+3*/ ) // 右端に来たら
			{
				data->state2=4;
				data->wait2_256=t256(0);
				s->anim_frame=8;
			}
			else
			{
				{	const short sss[4] =
					{
						t256(1.2),/*easy:まばらなので簡単*/
						t256(0.5/*1.0*/ /*1.5*/),/*normal:塊なので大きく避ければ簡単*/
						t256(1.0/*2.0*/),/*hard:たまに難しい時がある*/
						t256(18.5/*6.5*/ /*12.5*/ /*8.5*/) /*luna:うへぇこのスキマくぐるんですかぁ？*/
						/*(luna:ボムらなくても気合避けでなんとかなる程度に難易度抑えたいのだがまだ高すぎるな)*/
					};
					data->wait2_256 += /*t256(4)*/sss[difficulty]/*t256(3*1.42)*/;
				}
				if (0==(((int)data->wait2_256)&(16-1)/*%21 t256(15*1.42)*/))
				{
					enemy_fall_knife_create(s, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(360)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.03));
					//if (difficulty)
					{
						enemy_fall_knife_create(s, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(270)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.04));
					//	if (difficulty>1)
						{	enemy_fall_knife_create(s, /*(0.7)*/t256(2)/*(2)*/, (deg_360_to_512(180)-((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
					play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				}
				move_sakuya(s, t256(5), SAKUYA_POINT_03_RIGHT);
				if (s->anim_frame<8)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=7)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case 4:
		//	if (( s->x256 < (SAKUYA_LIMIT_X_MID)/*GAME_WIDTH/2-s->w/2*/ ))	// 真ん中に来たら
			if (( s->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
				( s->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		// 真ん中に来たら
			{
				data->state2=5;
				s->anim_frame=4;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_01_MID_UP);
				if (s->anim_frame>4)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			break;
		case 5:
			if (s->x256 > (SAKUYA_LIMIT_X_RIGHT)/*GAME_WIDTH-s->w-3*/ ) // 右端に来たら /* → || (s->y<5)があると無限ループになる */
			{
				data->state2=6;
				data->wait1=10;
				data->wait2_256=0;
				s->anim_frame=8;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_03_RIGHT);
				if (s->anim_frame<8)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case 6:
			if (s->x256 < (SAKUYA_LIMIT_X_LEFT)/*5*/)	// 左端に来たら
			{
				data->state2=7;
				data->wait2_256=0;
				s->anim_frame=0;
			}
			else
			{
				data->wait2_256 += t256(4)/*(3*1.42)*/;
				if (0x0000==((data->wait2_256)&(0x0f00))/*%(12*1.42)*/)
				{
					enemy_fall_knife_create(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(180)), t256(0.03));
					if (difficulty)
					{
						enemy_fall_knife_create(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)+deg_360_to_512(90)), t256(0.04));
						if (difficulty>1)
						{	enemy_fall_knife_create(s, t256(3), (((t256_floor(data->wait2_256))&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
					play_voice_auto_track(VOICE12_SAKUYA_B2_SHOT);
				}
				move_sakuya(s, t256(5), SAKUYA_POINT_00_LEFT);
				if (s->anim_frame>0)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=7)
					{
						s->anim_frame--;
						data->wait3=0;
					}
				}
			}
			break;
		case 7:
		//	if (( s->x256 > (SAKUYA_LIMIT_X_MID)/*(GAME_WIDTH/2-s->w/2)*/ ))	// 真ん中に来たら
			if (( s->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
				( s->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		// 真ん中に来たら
			{
				data->state2=8;
				data->wait2_256=t256(60);
				s->anim_frame=4;
			}
			else
			{
				move_sakuya(s, t256(3), SAKUYA_POINT_01_MID_UP);
				if (s->anim_frame<4)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case 8:
			data->wait2_256 -= t256(1);
			if (data->wait2_256 < 0)
			{
				data->state2=1;
				s->anim_frame=4;
			}
			else
			{
//				data->move_angle512=atan_512_SAKUYA_MID;
				if (0 == ((t256_floor(data->wait2_256))&(32-1)/*%20*/) )
				{
					int b_wait2_high;
					b_wait2_high=(((int)((t256_floor(data->wait2_256))>>5/*/20*/)));
					s->anim_frame=(s->anim_frame+1)%7;
						enemy_boss05_knifes1(s, (b_wait2_high<<8)+t256(4.0), deg_360_to_512(30)-(b_wait2_high)*deg_360_to_512(40), -80);
						enemy_boss05_knifes1(s, (b_wait2_high<<8)+t256(3.0), deg_360_to_512(30)-(b_wait2_high)*deg_360_to_512(30), -60);
					if (difficulty>1)
					{
						enemy_boss05_knifes1(s, (b_wait2_high<<8)+t256(2.0), deg_360_to_512(30)-(b_wait2_high)*deg_360_to_512(20), -40);
					}
					s->anim_frame=18;
				}
			}
			if (s->anim_frame>15)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=4)
				{
					s->anim_frame--;
					data->wait3=0;
				}
			}
		}
		break;
	//	あまりに短いので少し長くする
	case SAKUYA_04_KEITAI_A/*3*/:	// 第四形態: (黄色マスカット弾)
	case SAKUYA_04_KEITAI_B/*4*/:	// 第四形態: (黄色マスカット弾)
		switch (data->state2)
		{
		case 0:
			if (data->wait1<0)	// 待機時間が終わったら
			{
				data->state2=1;
//++				pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
//???			data->boss_base.boss_timer += (20*64);			// + 20 [秒] ???	/* 16.666[秒] 1000 ???*/
				s->anim_frame=15;
			}
			else
			{
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		case 1:
		//	if ( ( s->y > 50) && (s->x<GAME_WIDTH/2+30 || s->x>GAME_WIDTH/2-30))		// 真ん中に来たら
			if (( s->y256 > (SAKUYA_POINT_Y_CENTER-SAKUYA_POINT_Y_MARGIN) ) &&
				( s->x256 < (SAKUYA_LIMIT_X_MID_LEFT)) &&
				( s->x256 > (SAKUYA_LIMIT_X_MID_RIGHT)))		// 真ん中に来たら
			{
				data->state2=2;
				data->wait1=10;
				data->wait2_256=0;
				s->anim_frame=9;
			}
			else
			{
				data->move_angle512=atan_512_SAKUYA_CENTER/*atan_512(50-s->y,GAME_WIDTH/2-s->w-s->x)*/;
				s->x256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
				if (s->anim_frame<18)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case 2:
			if (s->anim_frame==14)
			{
				enemy_boss05_maho_create(s);		// 魔方陣生成
				data->state2=3;
				data->wait2_256=t256(10);
				data->wait1=5;
			}
			else
			{
				if (s->anim_frame<14)
				{
					data->wait3 += 1/*fps_fa ctor*/;
					if (data->wait3>=3)
					{
						s->anim_frame++;
						data->wait3=0;
					}
				}
			}
			break;
		case 3:
			data->angle512=atan_512(player->y256-s->y256+((player->h128)),player->x256-s->x256-((player->w128)) );
			data->wait2_256 -= t256(1)/*fps_fa ctor*/;
			if (data->wait2_256<t256(0))
			{
				data->wait2_256=t256(50);
				int *s_data_angle512;
				s_data_angle512 = &(((BOSS05_DATA *)s->data)->angle512);
				/* (黄色マスカット弾) */
				enemy_sp1_bullet_create(s, (t256(0.70)), data->angle512-1*42/*deg_360_to_512(30)*/, (t256(0.030)),	1/* 1==R*/, s_data_angle512 );/*0.72 == 1.2*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.70)), data->angle512+1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/, s_data_angle512 );/*0.72 == 1.2*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.85)), data->angle512-2*42/*deg_360_to_512(30)*/, (t256(0.033)),	1/* 1==R*/, s_data_angle512 );/*0.9 == 1.5*0.6*/
				enemy_sp1_bullet_create(s, (t256(0.85)), data->angle512+2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/, s_data_angle512 );/*0.9 == 1.5*0.6*/
				if (difficulty)
				{
					enemy_sp1_bullet_create(s, (t256(1.0)), data->angle512-3*42/*deg_360_to_512(30)*/, (t256(0.036)),  1/* 1==R*/, s_data_angle512 );/*1.08 == 1.8*0.6*/
					enemy_sp1_bullet_create(s, (t256(1.0)), data->angle512+3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/, s_data_angle512 );/*1.08 == 1.8*0.6*/
					if (1<difficulty)
					{
						enemy_sp1_bullet_create(s, (t256(1.15)), data->angle512-4*42/*deg_360_to_512(30)*/, (t256(0.04)),  1/* 1==R*/, s_data_angle512 );/*1.26 == 2.1*0.6*/
						enemy_sp1_bullet_create(s, (t256(1.15)), data->angle512+4*42/*deg_360_to_512(30)*/, (t256(0.04)), -1/*-1==L*/, s_data_angle512 );/*1.26 == 2.1*0.6*/
					}
				}
				//bullet_create_oodama0(s, t256(2), ANGLE_JIKINERAI_DAN, 40, 0, t256(s->w/2), t256(s->h/2));
				data->wait1 -= 1/*fps_fa ctor*/;
				if (data->wait1<0)	//3回撃ったら
				{
					data->wait1=20;
					if (0==(ra_nd()&(2-1)/*%2*/))
					{
						data->state2=4;
						s->anim_frame=2;
					}
					else
					{
						data->state2=5;
						s->anim_frame=6;
					}
				}
			}
			if (s->anim_frame>9)
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					data->wait3=0;
					s->anim_frame--;
				}
			}
			break;
		case 4:
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)
			{
				data->wait1=3;
				data->state2=3;
				data->wait2_256=t256(30);
				s->anim_frame=18;
			}
			else
			{
				if (s->x256 < t256(100) )
				{
					data->state2=5;
					s->anim_frame=7;
				}
				else
				{	s->x256 -= t256(2)/**fps_fa ctor*/;}
			}
			break;
		case 5:
			data->wait1 -= 1/*fps_fa ctor*/;
			if (data->wait1<0)
			{
				data->wait1=3;
				data->state2=3;
				data->wait2_256=t256(30);
				s->anim_frame=18;
			}
			else
			{
				if (s->x256 > t256(GAME_WIDTH-100)-((s->w128+s->w128)) )
				{
					data->state2=4;
					s->anim_frame=1;
				}
				else
				{	s->x256 += t256(2)/**fps_fa ctor*/;}
			}
			break;
		}
		break;
	//
	case SAKUYA_05_KEITAI/*5*/ /*4*/:	// 第五形態: 最終形態(その1)
#if 1
		switch (data->state2)
		{
		case 0:
			if (data->wait1<0)	//待機時間が終わったら
			{
				data->state2=1;
//++				pd->bo ssmode=B00_NONE/*B01_BA TTLE*/;
				data->move_angle512=atan_512(t256(30)-(s->y256),t256(GAME_WIDTH/2)-((s->w128))-(s->x256));
			}
			else
			{
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		case 1:
			if ( (s->x256 < t256(GAME_WIDTH/2+30) ) &&
				 (s->x256 > t256(GAME_WIDTH/2-10) ) &&
				 (s->y256 < t256(50)) ) 	//真ん中に来たら
			{
				data->state2=2;
				data->wait1=20;
				data->wait2_256=0;
				s->anim_frame=4;
			}
			else
			{
				s->x256 += ((cos512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->move_angle512))*t256(3))>>8)/**fps_fa ctor*/;
			}
			break;
		case 2:
			if (data->wait1<0)		{	data->state2=3;s->anim_frame=15;	}		//待機時間が終わったら
			else if (data->wait1<6) {			s->anim_frame=14;	}
			else if (data->wait1<12)	{			s->anim_frame=13;	}
			else if (data->wait1<18)	{			s->anim_frame=12;	}
			else if (data->wait1<24)	{			s->anim_frame=11;	}
			else if (data->wait1<30)	{			s->anim_frame=10;	}
			else if (data->wait1<36)	{			s->anim_frame=9;		}
			data->wait1 -= 1/*fps_fa ctor*/;
			break;
		case 3:
			if (s->anim_frame==18)
			{
			//	距離を求めるのに２乗してルートを取るのはルート演算が遅すぎるのでゲーム中は無理。
			//	予めテーブル化しておくか、代替演算(CODIC)で求めるかどちらか。
			//	(dou ble 使わないで int CODIC なら sqrt() も、おそらくpspでも実用的な速度が出る)
			//	data->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
					int xxx256;
					int yyy256;
					xxx256=abs(player->x256-s->x256);xxx256+=256;/* abs()は展開してくれるから大丈夫 */
					yyy256=abs(player->y256-s->y256);yyy256+=256;
					if (xxx256>yyy256)	{data->length_s_p256=(xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
					else				{data->length_s_p256=(yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
				}
#if 1
				enemy_even_knife_create(s, t256(10), data->length_s_p256, 0/*0==右*/);
				#if 1
				enemy_even_knife_create(s, t256(10), data->length_s_p256, 1/*1==左*/);
				#endif
#endif
				data->wait1=40;
				if (s->y256 > t256(150))
				{	data->move_angle512=deg_360_to_512(210) + (((ra_nd()&(2048-1))*21)>>8) /*(ra_nd()%(deg_360_to_512(120)))*/;}
				else
				{	data->move_angle512=/*deg512_2rad*/((ra_nd()&(512-1)))/*degtorad(ra_nd()%360)*/;}
				#if 1
				/*念の為*/
				mask512(data->move_angle512);
				#endif
				s->anim_frame=16;
				data->state2=4;
			}
			else
			{
				data->wait3 += 1/*fps_fa ctor*/;
				if (data->wait3>=3)
				{
					s->anim_frame++;
					data->wait3=0;
				}
			}
			break;
		case 4:
			if (data->wait1<0)
			{
				data->state2=2;
				data->wait1=120+(3-difficulty)*50;
			}
			else
			{
				s->x256 += ((cos512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
				s->y256 += ((sin512((data->move_angle512))*t256(3.5))>>8)/**fps_fa ctor*/;
				data->wait1 -= 1/*fps_fa ctor*/;
			}
			break;
		}
		if ((player->x256+((player->w128)) < s->x256+((s->w128))+t256(25))&&
			(player->x256+((player->w128)) > s->x256+((s->w128))-t256(25)))
		{	data->wait2_256 += t256(1)/*fps_fa ctor*/;}
		if (data->wait2_256 > t256(20)+(3-difficulty)*t256(30))
		{
			s->anim_frame=16;
		//	距離を求めるのに２乗してルートを取るのはルート演算が遅すぎるのでゲーム中は無理。
		//	予めテーブル化しておくか、代替演算(CODIC)で求めるかどちらか。
		//	(dou ble 使わないで int CODIC なら sqrt() も、おそらくpspでも実用的な速度が出る)
		//	data->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
					int xxx256;
					int yyy256;
					xxx256=abs((int)player->x256-(int)s->x256);xxx256+=256;/* abs()は展開してくれるから大丈夫 */
					yyy256=abs((int)player->y256-(int)s->y256);yyy256+=256;
					if (xxx256>yyy256)	{data->length_s_p256=(xxx256/*+(yyy256>>1)*/);}/*少し難しくする*/
					else				{data->length_s_p256=(yyy256/*+(xxx256>>1)*/);}/*少し難しくする*/
				}
			if (data->length_s_p256>t256(80))
			{	data->length_s_p256=t256(80);}
			enemy_boss05_knifes3(s/*,*/ /*1+difficulty,*/ /*(data->length_s_p256)*/ );
		}
		enemy_boss05_out(s);
#endif
		break;

	//
	case SAKUYA_06_KEITAI/*6*/ /*5*/:	// 第六形態: 最終形態(その2)
#if 1
		if (data->wait2_256 < t256(0) )
		{
			data->wait2_256 = t256(0);
			data->wait1 = 120;
			data->state1	= SAKUYA_05_KEITAI/*4*/;
			data->state2	= 2;
			s->anim_frame = 4;
		}
		else
		{
			data->wait2_256 -= t256(0.03);
			if (s->alpha>249)//if (s->alpha>255)
			{
				s->alpha=255;
			}
			else //if (s->alpha<255)
			{
				s->alpha+=5;
			}
			s->x256 += ((cos512((data->angle512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
			s->y256 += ((sin512((data->angle512))*(data->wait2_256))>>8)/**fps_fa ctor*/;
		}
		if (s->anim_frame>15)
		{
			data->wait3 += 1/*fps_fa ctor*/;
			if (data->wait3 >= 4)
			{
				s->anim_frame--;
				data->wait3 = 0;
			}
		}
		enemy_boss05_out(s);
#endif
		break;
	}
//
	{
		data->boss_base.boss_timer -= 1/*fps_fa ctor*/;
		#if 1/*時間切れ(色々問題あるのでとりあえず無効)*/
		if ((data->boss_base.boss_timer < 1))
		{
			data->boss_base.boss_timer = (60*64);
			if ((data->state1 < SAKUYA_05_KEITAI/*4*/))
			{
				//レミリア戦の開始イベントが見れなくなっちゃうのでここは無効
				#if (0==USE_REMILIA)
				/* vs REMILIA */
				if (2==select_player)
				{data->boss_base.health=0/*1*/;}
				else
				#endif
				{data->boss_base.health 	= ((/*4*/SAKUYA_05_KEITAI*1024)-1)-(data->state1<<10/**1024*/);}
				sakuya_put_items(/*c,*/s);
			}
		}
		#endif
	}
//
	#if 0/*十六夜本人デバッグ用*/
	/* パネルのスコア欄にstate1を、グレイズ欄にstate2を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= data->state1;
	((PLAYER_DATA *)player->data)->graze_point	= data->state2;
	#endif
}

/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/

void add_boss_sakuya(STAGE_DATA *l)/*int lv*/
{
	BOSS05_DATA *data;
	SPRITE *s;
//	s					= spr ite_add_file 0("boss/sakuya.png"/*"boss04.png"*/, 19, PRIORITY_03_ENEMY, 0); s->anim_speed=0;
	s					= sprite_add_res(BASE_BOSS_SAKUYA_PNG);
	s->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
	s->anim_frame		= 4;
	s->type 			= SP_BOSS/*SP_BOSS05*/;
	s->callback_mover	= enemy_boss05_move;
	s->x256 			= (t256(GAME_WIDTH/2))-((s->w128));
	s->y256 			= -((s->h128+s->h128));

	data				= mmalloc(sizeof(BOSS05_DATA));
	s->data 			= data;

	data->boss_base.health=((/*5*/SAKUYA_06_KEITAI*1024)-1);		/*5119==((5*1024)-1)*/

//レミリア戦の開始イベントが見れなくなっちゃうのでここは無効
//				/* vs REMILIA */
//				if (2==select_player)	{	data->boss_base.health=1; }
	/*score(5000)+score(4000)*difficulty*/
	data->boss_base.score = adjust_score_by_difficulty(score(5000000)); 	/* 500万 */
	data->state1		= SAKUYA_01_KEITAI/*0*/;
	data->state2		= 0;
	data->wait1 		= 0;
	data->wait2_256 	= t256(0);
	data->wait3 		= 0;
//	data->aaa_type		= 0;
//	data->level 		= (l->user_y);	/*lv*/
	data->move_angle512 = (0);
//	data->boss_base.boss_timer		= (20*64);		/*1000*/	// + 20 [秒] ???	/* 16.666[秒] 1000 ???*/
	data->boss_base.boss_timer		= (60*64);	// 60 [秒] ???

	((PLAYER_DATA *)player->data)->boss=s;
//	((PLAYER_DATA *)player->data)->bo ssmode=B00_NONE/*B01_BA TTLE*/;
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss05_hitbyweapon_dummy;	/* ダミーコールバック登録 */
}
