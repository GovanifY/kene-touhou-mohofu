/*
十六夜 咲夜
PAD
*/

#include "enemy.h"
extern int select_player;

typedef struct _boss04_data
{
	ENEMY_BASE enemy_base;
	int type;	/* 0: normal, 1: destroyed */
	int state1; //形態
	int state2; //行動
//
	int use_sp_time; //制限時間のON/OFF (イベント用)
//
	double sp_time; //制限時間
	double wait1;
	double wait2;
	double wait3;
	int level;
	/*double*/int move_angle512;
	/*double*/int angle512;
	/*double*/int length_s_p;	/* 咲夜とプレイヤーとの距離 */
} BOSS04_DATA;

typedef struct _boss04_maho_data
{
	SPRITE *c;
	int state01;	//形態
	int state02;	//行動
	int nnn;
	int mmm;
	double wait;
	/*double*/int angle512;
} BOSS04_MAHO_DATA;

typedef struct
{
	BOSS04_DATA *sd;
	/*double*/int angle512;
	/*double*/int angle2_512;
	double speed;
//	int id;
	/*double*/int gra256;
	/*double*/int sum256;
	int timer;
	int r_or_l; 	//	/*+1==R*/	/*-1==L*/
} BULLET_SP1_DATA;

typedef struct
{
	BOSS04_MAHO_DATA *sd;
//	int id;
	int state;
	/*double*/int angle512;
	/*double*/int gra256;
	/*double*/int sum256;
} BULLET_SP2_DATA;


static void callback_enemy_boss04_hitbyplayer(SPRITE *c)
{
}

static void callback_enemy_boss04_hitbyweapon_dummy(SPRITE *c, SPRITE *s/*, int angle*/)
{	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
}
static void callback_enemy_boss04_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/)
{
	/*
		c		ボス本体
		s		プレイヤーの弾
	*/
	BOSS04_DATA *b=(BOSS04_DATA *)c->data;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	explosion_add(s->x,s->y,0,EXPLOSION_MINI00);
	b->enemy_base.health -= w->strength;
	if (b->enemy_base.health < (4-b->state1)*1024)
	{
		playChunk(7);
		((PLAYER_DATA *)player->data)->bossmode=B04_CHANGE;
		bonus_multi_add(c, SP_BONUS_06_COIN,        5, BONUS_FLAG_COLLECT);
		bonus_multi_add(c, SP_BONUS_00_FIREPOWER,   5, BONUS_FLAG_RAND_XY);
		bonus_multi_add(c, SP_BONUS_01_FIREPOWER_G, 1, BONUS_FLAG_RAND_XY);
		s->aktframe 	=4;
		b->state1++;
		b->state2		=0;
		b->wait1		=80;
		b->wait2		=-100;
		b->wait3		=0;
		b->sp_time		=2500;
	}

	if (b->enemy_base.health <= 0)
	{
		explosion_add_circle(c);
		bonus_multi_add(c, SP_BONUS_06_COIN, 15, BONUS_FLAG_COLLECT);
		c->type=SP_DELETE;
		((PLAYER_DATA *)player->data)->score+=b->enemy_base.score;
		((PLAYER_DATA *)player->data)->bossmode=B02_DEATH_WAIT;
	}
}


#if 1
/* (黄色マスカット弾) */
static void enemy_sp1_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	BULLET_SP1_DATA *d=(BULLET_SP1_DATA *)s->data;

	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	d->timer++;
	if (d->timer>40)
	{
		d->sum256 += d->gra256;
		s->x+=(((int)(co_s512((d->angle512))*d->speed*(d->sum256>>2/*/5*/)+co_s512((d->angle2_512))*d->sum256))>>8)*fps_factor;
		s->y+=(((int)(si_n512((d->angle512))*d->speed*(d->sum256>>2/*/5*/)+si_n512((d->angle2_512))*d->sum256))>>8)*fps_factor;
	}
	else if (d->timer==40)
	{
		s->aktframe=1;
		d->angle512=d->sd->angle512+deg_360_to_512(90)*d->r_or_l;
		mask512(d->angle512);
		d->angle2_512 = d->sd->angle512;
	}
	else
	{
		s->x+=co_s512((d->angle512))*d->speed*fps_factor;
		s->y+=si_n512((d->angle512))*d->speed*fps_factor;
	}
	if ((s->x<-s->x)||(s->x>GAME_WIDTH)||
		(s->y<-s->y)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}

static void enemy_sp1_bullet_create(SPRITE *s, /*double*/int speed256, /*double*/int angle512, /*double*/int gra256, int r_or_l)
{
	SPRITE *h;		/*丸弾８(赤ＲＧＢ緑若黄青)*/
//	h				= sprite_add_file("bs hoot2.png",3,PR_ENEMY);
	h				= sprite_add_file("bullet_maru8.png",8,PR_ENEMY);
	h->type 		= SP_EN_LASER;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_sp1_bullet_move;
	h->anim_speed	= 0;
	h->aktframe 	= /*4*/((rand()&3)+4)/*0*/;/*とりあえず*/
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	BULLET_SP1_DATA *data;
	data			= mmalloc(sizeof(BULLET_SP1_DATA));
	h->data 		= data;
	if (angle512==999/*-2*/)
	{	data->angle512=atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);}
	else
	{	data->angle512=(angle512);}
	data->angle2_512	= (0);
	data->sd		= (BOSS04_DATA *)s->data;
	data->speed 	= t256_to_double(speed256);
	data->gra256	= gra256;
	data->sum256	= t256(0);
	data->timer 	= 0;
	data->r_or_l	= r_or_l;
}
#endif

#if 1/*魔方陣用*/
static void enemy_sp2_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	{	BULLET_SP2_DATA *d=(BULLET_SP2_DATA *)s->data;
		if (2==d->sd->nnn/*%(4-difficulty)*/)/* ステートが１の時配置して、ステートが２になったら動く */
		{
			d->sum256 += d->gra256;
			s->x+=((((int)(co_s512((d->angle512))*256))*(d->sum256))>>16)/**fps_factor*/;
			s->y+=((((int)(si_n512((d->angle512))*256))*(d->sum256))>>16)/**fps_factor*/;
		}
	}
	if ((s->x<-s->x)||(s->x>GAME_WIDTH)||
		(s->y<-s->y)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}
	/*
		angle512	512度指定
	*/
static void enemy_sp2_bullet_create(SPRITE *s, int angle512, /*double*/int gra256)
{
	SPRITE *h;
	h				= sprite_add_file("kugel.png",1,PR_ENEMY);
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_sp2_bullet_move;
	h->anim_speed	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	BULLET_SP2_DATA *data;
	data			= mmalloc(sizeof(BULLET_SP2_DATA));
	h->data 		= data;
	data->sd		= (BOSS04_MAHO_DATA *)s->data;
	data->state 	= 0;
	data->angle512	= (angle512);
	data->gra256	= gra256;
	data->sum256	= t256(0);
}
#endif


static void enemy_boss04_waitstate(SPRITE *s, int nextstate, int aktframe)
{
	BOSS04_DATA *b=(BOSS04_DATA *)s->data;

	if (b->wait1>0)
	{	b->wait1-=fps_factor;}
	else
	{
		b->state2=nextstate;
		s->aktframe=aktframe;
	}
}

static void enemy_boss04_out(SPRITE *s)
{
	if (s->x<50)				{	s->x=50;}
	if (s->x>GAME_WIDTH-s->w-50)	{	s->x=GAME_WIDTH-s->w-50;}
	if (s->y<10)				{	s->y=10;}
	if (s->y>GAME_HEIGHT+s->h-120)	{	s->y=GAME_HEIGHT+s->h-120;}
}

static void enemy_boss04_more_angle(SPRITE *s, /*double*/int speed256, /*double*/int angle4096, /*double*/int a_angle4096)
{
	enemy_angle_bullet_create(s, speed256, angle4096,						  a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096(11*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096(10*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 9*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 8*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 7*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 6*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 5*30), a_angle4096);
	enemy_angle_bullet_create(s, speed256, angle4096+deg_360_to_4096( 4*30), a_angle4096);
}

static void enemy_boss04_knifes1(SPRITE *s, /*double*/int speed256, int angle512, int height)
{
	//アングルは角度指定で-90～90
	//heightは"playerから"の高さ
	enemy_follow_knife_create1(s, speed256, /*deg_360_to_512*/(deg_360_to_512(180)-angle512), height);
	enemy_follow_knife_create1(s, speed256, /*deg_360_to_512*/(angle512), height);
}
#if 1/*魔方陣用*/
static void enemy_boss04_knifes2(SPRITE *s, /*double*/int speed256)
{
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
}
#endif

#if 1
/* プレイヤーの周りに散らばり、時間が経つとプレイヤー狙い弾 */
static void enemy_boss04_knifes3(SPRITE *s/*,*/ /*double speed,*/ /*double*/ /*int length222*/)
{
	BOSS04_DATA *b=(BOSS04_DATA *)s->data;
	double len;
	double tmp1;
	double tmp2;
	int tmp888;
	tmp888=0;
	/* ある程度遠くに出ないと認識しづらい */
	/* キャラ(コア)がでかいと避ける前に死ぬ。(ナイフが広がる前にあたる) */
	len = /*70*/80 -difficulty /*-length222*/  /*/2*/;
	b->wait2=0;
	while (1)
	{
		tmp888=(((int)rand()+(int)b->wait2)&(8-1)/*%8*/);
		b->wait2+=1;
		len+=0.1;
		tmp1=player->x+((player->w-s->w)/2)+co_s512((tmp888<<6))*len;
		tmp2=player->y+((player->h-s->h)/2)+si_n512((tmp888<<6))*len;
		if (((30<tmp1) && (tmp1<(GAME_WIDTH-30)))&&
			((30<tmp2) && (tmp2<(GAME_HEIGHT-72))))
		{	break;}
	}
	double tmp_x;
	double tmp_y;
	tmp_x=player->x+player->w/2;
	tmp_y=player->y+player->h/2;
	int i;
	for (i=0;i<8;i++)
	{
		if (tmp888==i)
		{
			b->angle512 	= (i<<6);
			s->x			= tmp1;
			s->y			= tmp2;
			s->alpha		= 0;
			s->aktframe 	= 18;
			b->state1		= 5;
			b->wait2		= /*speed*/1+difficulty;
		}
		else
		{
		//	double tmp_s_x;
		//	double tmp_s_y;
		//	tmp_s_x = s->x;
		//	tmp_s_y = s->y;
			/* プレイヤー狙い弾 */
			s->x			=	tmp_x+co_s512((i<<6))*len;
			s->y			=	tmp_y+si_n512((i<<6))*len;
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

static void enemy_boss04_maho_move(SPRITE *s)
{
	BOSS04_MAHO_DATA *d=(BOSS04_MAHO_DATA *)s->data;
	if (d->state01<5)
	{
		s->x=d->c->x+((d->c->w-s->w)/2);
		s->y=d->c->y+((d->c->h-s->h)/2);
		if (((BOSS04_DATA *)d->c->data)->state1==4/*星型を描いてもいいよ*/)
		{
			d->state01=5;/*星型を描くぜ*/
		}
	}
	switch (d->state01)
	{
//------------------------------------------
	case 0:
		if (s->alpha==150)
		{
			d->state01=1;
			d->wait=50;
		}
		else if (s->alpha<150)
		{	s->alpha+=3;}
		else
		{	s->alpha=150;}
		break;

	case 1:
		if (d->wait<0)
		{
			d->state01=2;
			d->state02=0x100;
		}
		else
		{	d->wait-=1/*fps_factor*/;}
		break;
	case 2:
		if ((d->state02&0x0f/*%10*/)==0)
		{
			enemy_stop_bullet3_create(s, (d->state02/*>>4*/<<3/*1/10*/)/**256*/, d->angle512,					 0.02, d->angle512+rad2deg512((d->state02)>>7/*/100*/)-rad2deg512(M_PI		   ));
			enemy_stop_bullet3_create(s, (d->state02/*>>4*/<<3/*1/10*/)/**256*/, d->angle512+deg_360_to_512(120), 0.02, d->angle512+rad2deg512((d->state02)>>7/*/100*/)-rad2deg512(M_PI+M_PI*2/3));
			enemy_stop_bullet3_create(s, (d->state02/*>>4*/<<3/*1/10*/)/**256*/, d->angle512-deg_360_to_512(120), 0.02, d->angle512+rad2deg512((d->state02)>>7/*/100*/)-rad2deg512(M_PI-M_PI*2/3));
		}
		if (d->state02<0x0)
		{
			d->state01=3;
			d->wait=15;
			d->state02=0x11;
		}
		d->state02--;
		break;
	case 3:
		d->wait-=1/*fps_factor*/;
		if (d->wait<0)
		{
			d->wait=12;
			playChunk(13);
			{int i;
				for (i=5*256;i<9*256;i+=256)
				{
					bullet_create_haridan180(s, d->angle512,					 i, 0, 0	);
					bullet_create_haridan180(s, d->angle512+deg_360_to_512(120), i, 0, 0	);
					bullet_create_haridan180(s, d->angle512-deg_360_to_512(120), i, 0, 0	);
				}
			}
			d->angle512+=rad2deg512(0.15);
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
		s->aktframe=0;
		if (((BOSS04_DATA *)d->c->data)->state2==1)
		{
			d->angle512=atan_512(48-s->y-s->h/2, 190-s->x-s->w/2);/*星型線1*/
			d->state01=6;
			d->state02=0x0;
		}
		break;
	case 6:
		if (s->y+s->h/2<=48)
		{
			d->state01=7;
			d->angle512=atan_512(217-s->y-s->h/2, 248-s->x-s->w/2);/*星型線2*/
			d->wait=50;
			d->nnn++;	if (d->nnn>(/*4-0*/3/*difficulty*/) ){d->nnn=1/*0*/;}
			d->mmm++;	d->mmm &= 1;
			d->state02=0x0;
			if (2==d->nnn/*%(4-difficulty)*/)/* %2 == (5-3:Lunatic) */
			{	s->aktframe=1;}
			else
			{	s->aktframe=0;}
		}
		else
		{
			s->x+=co_s512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			s->y+=si_n512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(3)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015) );
				}
			}
		}
		break;
	case 7:
		enemy_boss04_knifes2(s, t256(1.2) );
		d->state01=8;
		break;
	case 8:
		if (d->wait<0)
		{	d->state01=9;}
		else
		{	d->wait--;}
		break;

	case 9:
		if (s->y+s->h/2>=217)
		{
			d->state01=10;
			d->angle512=atan_512(113-s->y-s->h/2, 100-s->x-s->w/2);/*星型線3*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x+=co_s512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			s->y+=si_n512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(147)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015) );
				}
			}
		}
		break;
	case 10:
		enemy_boss04_knifes2(s, t256(1.2) );
		d->state01=11;
		break;
	case 11:
		if (d->wait<0)
		{	d->state01=12;}
		else
		{	d->wait--;}
		break;

	case 12:
		if (s->y+s->h/2<=113)
		{
			d->state01=13;
			d->angle512=atan_512(113-s->y-s->h/2, 280-s->x-s->w/2);/*星型線4*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x+=co_s512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			s->y+=si_n512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(291)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015) );
				}
			}
		}
		break;
	case 13:
		enemy_boss04_knifes2(s, t256(1.2) );
		d->state01=14;
		break;
	case 14:
		if (d->wait<0)
		{	d->state01=15;}
		else
		{	d->wait--;}
		break;

	case 15:
		if (s->x+s->w/2>=280)
		{
			d->state01=16;
			d->angle512=atan_512(217-s->y-s->h/2, 138-s->x-s->w/2);/*星型線5*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x+=co_s512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			s->y+=si_n512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(75)+((( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/)), t256(0.015) );
				}
			}
		}
		break;
	case 16:
		enemy_boss04_knifes2(s, t256(1.2) );
		d->state01=17;
		break;
	case 17:
		if (d->wait<0)
		{	d->state01=18;}
		else
		{	d->wait--;}
		break;

	case 18:
		if (s->y+s->h/2>=217)
		{
			d->state01=19;
			d->angle512=atan_512(48-s->y-s->h/2, 190-s->x-s->w/2);/*星型線1*/
			d->wait=50;
			d->state02=0x0;
		}
		else
		{
			s->x+=co_s512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			s->y+=si_n512((d->angle512))*(3/*+difficulty*/)/**fps_factor*/;
			d->state02+=/*5*/5/*3*/;
			if (1==d->nnn)
			{
				if (0==(d->state02&/*7*/3/*%5*/))
				{
					enemy_sp2_bullet_create(s, deg_360_to_512(219)+(( ((-d->state02*2*d->mmm)) +d->state02)>>3/*/5*/), t256(0.015) );
				}
			}
		}
		break;
	case 19:
		enemy_boss04_knifes2(s, t256(1.2) );
		d->state01=20;
		break;
	case 20:
		if (d->wait<0){
			d->state01=6;
			d->state02=0x0;
		}
		else
		{	d->wait--;}
		break;
#endif
	}
	if (d->c->type==-1)
	{	s->type=SP_DELETE;}
	#if 1/*魔方陣デバッグ用*/
	/* パネルのスコア欄にstate01を、グレイズ欄にstate02を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score=d->state01;
	((PLAYER_DATA *)player->data)->graze=d->state02;
	#endif
}

static void enemy_boss04_maho_create(SPRITE *s) 	//魔方陣グラフィック生成
{
	SPRITE *s2; 	// 魔方陣グラフィックのスプライト
	s2				= sprite_add_file("boss04-lo.png",2,PR_GROUNDER);	s2->anim_speed	= 0;
	s2->flags		|= SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
	s2->aktframe	= 0;
	s2->type		= SP_PLAYER2;
	BOSS04_MAHO_DATA *b;
	b				= mmalloc(sizeof(BOSS04_MAHO_DATA));
	s2->data		= b;
	b->state01		= 0;
	b->state02		= 0x0;
	b->wait 		= 0;
	b->angle512 	= (0);
	b->c			= s;
	b->nnn			= 0;
	b->mmm			= 0;
	s2->alpha		= 0;
	s2->mover		= enemy_boss04_maho_move;
	s2->x			= s->x+((s->w-s2->w)/2);
	s2->y			= s->y+((s->h-s2->h)/2);
}


static void enemy_bullet_create_lines(SPRITE *s)
{
//
//	int i;
//	for (i=0;i<difficulty;i++)
	enemy_bullet_create(s, 2.5);
	enemy_bullet_create(s, 2);
	enemy_bullet_create(s, 3.5);
}

static void enemy_boss04_move(SPRITE *s)
{
	BOSS04_DATA *b=(BOSS04_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (1==b->use_sp_time)
	{
		b->sp_time-=fps_factor;
	}
	switch (b->state1)
	{
	case 0: //第一形態:旧アリスとほぼ同じ動き
		switch (b->state2)
		{
		case 0: 	/* 下がる */
			if (s->y<20)
			{
				s->y+=fps_factor;
			}
			else	//yが20まで来たら
			{
			/* 登場時には弾を撃たない */
			//	enemy_bullet_create_lines(s);
				s->aktframe=0;
				pd->bossmode=B05_BEFORE_LOAD;
				/* プレイヤー弾受け付け、コールバックを登録 */
				((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss04_hitbyweapon;

				/* vs REMIRIA */
				if (2==select_player)
				{b->enemy_base.health=1;}

				b->state2=10;
			}
			break;
		/*完全に姿を現す*/
		case 10:	/* */
			if (pd->bossmode==B01_BATTLE)
			{
				b->use_sp_time=1;/*時間制限カウント有効化*/
				b->state2=1;
			}
			break;
		case 1: 	/* 左 */
			//if (pd->bossmode==B01_BATTLE)
			{
				if (s->x>=50){
					s->y+=fps_factor;
					s->x-=fps_factor*3;
					if (s->aktframe>0) {
						b->wait3+=fps_factor;
						if (b->wait3>=5) {
							s->aktframe--;
							b->wait3=0;
						}
					}
				}
				else{	//xが50以下になったら
					b->state2=2;
					b->wait1=50;
					enemy_bullet_create_lines(s);
					s->aktframe=4;
				}
			}
			break;
		case 2: 	/* 待機 */
			enemy_boss04_waitstate(s, 3, 4);
			break;
		case 3: 	/* 真ん中に戻る */
			if (s->y>20)
			{
				s->y-=fps_factor;
				s->x+=fps_factor*3;
				if (s->aktframe<8) {
					b->wait3+=fps_factor;
					if (b->wait3>=5) {
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			else{	//真ん中まで来たら
				b->state2=4;
				enemy_bullet_create_lines(s);
			}
			break;
		case 4: 	/* 右 */
			if (s->x+s->w<=GAME_WIDTH-50)
			{
				s->y+=fps_factor;
				s->x+=fps_factor*3;
			}
			else{
				b->state2=5;
				b->wait1=50;
				enemy_bullet_create_lines(s);
				s->aktframe=4;
			}
			break;
		case 5: 	/* 待機 */
			enemy_boss04_waitstate(s, 6, 4);
			break;
		case 6:
			if (s->y>20){
				s->y-=fps_factor;
				s->x-=fps_factor*3;
				if (s->aktframe>0) {
					b->wait3+=fps_factor;
					if (b->wait3>=5) {
						s->aktframe--;
						b->wait3=0;
					}
				}
			}
			else{	/* 真ん中に戻る */
				b->state2=1;
				enemy_bullet_create_lines(s);
			}
			break;
		}
		break;
	//
	case 1: //第二形態:豆まき程度
			//b->wait2		ループ回数
		switch (b->state2)
		{
		case 0:
			if (b->wait1<0){
				b->state2=1;
				pd->bossmode=B01_BATTLE;
				b->wait2=0;
			}else{
				b->wait1-=fps_factor;
			}
			break;
		case 1:
			{
				int ang512;
				if (rand()&(2-1)/*%2*/)
				{
					ang512=(rand()&(256-1)/*%(180*1.42)*/);
					if (ang512>deg_360_to_512(90))	{	ang512+=deg_360_to_512(90); 		s->aktframe=5;					}
					else							{										s->aktframe=3;					}
					ang512-=deg_360_to_512(45);
				}
				else
				{
					if (s->x>GAME_WIDTH/2)			{	ang512=deg_360_to_512(180); 		s->aktframe=6;					}
					else							{	ang512=deg_360_to_512(0);			s->aktframe=2;					}
				}
				b->move_angle512=/*deg512_2rad*/(ang512);
			}
			b->state2=2;
			b->wait1=20+(3-difficulty)*10;
			b->wait2++;
			break;
		case 2:
			if (b->wait1<0){
				b->wait1=35;
				b->state2=3;
				s->aktframe=4;
			}else{
				s->x+=co_s512((b->move_angle512))*fps_factor;
				s->y+=si_n512((b->move_angle512))*fps_factor;
				b->wait1-=fps_factor;
			}
			break;
		case 3:
			enemy_boss04_waitstate(s, 4, 15);
			b->wait3=0;
			break;
		case 4:
			if (s->aktframe==17)
			{
				playChunk(11);
				if (((int)b->wait2)&(2-1)/*%2*/)
				{
					enemy_boss04_more_angle(s, (2.5-0.3)*256+b->wait2*(256/10),  /*angle4096*/( 4*256), rad2deg4096(0.013));
					enemy_boss04_more_angle(s, (2.5-0.6)*256+b->wait2*(256/10),  /*angle4096*/( 2*256), rad2deg4096(0.011));
					enemy_boss04_more_angle(s, (2.5-0.9)*256+b->wait2*(256/10),  /*angle4096*/( 0	 ), rad2deg4096(0.009));
					enemy_boss04_more_angle(s, (2.5-1.2)*256+b->wait2*(256/10),  /*angle4096*/(14*256), rad2deg4096(0.007));
					enemy_boss04_more_angle(s, (2.5-1.5)*256+b->wait2*(256/10),  /*angle4096*/(12*256), rad2deg4096(0.005));
					if (difficulty>1)
					{
						enemy_boss04_more_angle(s, (2.5-1.8)*256+b->wait2*(256/10), /*angle4096*/( 10*256), rad2deg4096(0.003));
						enemy_boss04_more_angle(s, (2.5-2.1)*256+b->wait2*(256/10), /*angle4096*/(	8*256), rad2deg4096(0.001));
					}
				}
				else
				{
					enemy_boss04_more_angle(s, (2.5-0.3)*256+b->wait2*(256/10), /*angle4096*/(	4*256), rad2deg4096(-0.013));
					enemy_boss04_more_angle(s, (2.5-0.6)*256+b->wait2*(256/10), /*angle4096*/(	2*256), rad2deg4096(-0.011));
					enemy_boss04_more_angle(s, (2.5-0.9)*256+b->wait2*(256/10), /*angle4096*/(	0	 ), rad2deg4096(-0.009));
					enemy_boss04_more_angle(s, (2.5-1.2)*256+b->wait2*(256/10), /*angle4096*/( 14*256), rad2deg4096(-0.007));
					enemy_boss04_more_angle(s, (2.5-1.5)*256+b->wait2*(256/10), /*angle4096*/( 12*256), rad2deg4096(-0.005));
					if (difficulty>1)
					{
						enemy_boss04_more_angle(s, (2.5-1.8)*256+b->wait2*(256/10), /*angle4096*/( 10*256), rad2deg4096(-0.003));
						enemy_boss04_more_angle(s, (2.5-2.1)*256+b->wait2*(256/10), /*angle4096*/(	8*256), rad2deg4096(-0.001));
					}
				}
				s->aktframe=18;
			}
			else if (s->aktframe==18)
			{
				b->state2=5;
			}
			else
			{
				b->wait3+=fps_factor;
				if (b->wait3>=3)
				{
					s->aktframe++;
					b->wait3=0;
				}
			}
			break;
		case 5:
			if (s->x>GAME_WIDTH/2)
			{	b->move_angle512=-rad2deg512(M_PI*3/4);}
			else
			{	b->move_angle512=-rad2deg512(M_PI/4);}
				#if 1
				/*念の為*/
				mask512(b->move_angle512);
				#endif
			b->state2=6;
			b->wait1=5;
			break;
		case 6:
			if (b->wait1<0){
				b->wait1=10;
				b->state2=7;
			}
			else
			{
				if (s->y>60)
				{
					s->x+=co_s512((b->move_angle512))*2*fps_factor;
					s->y+=si_n512((b->move_angle512))*2*fps_factor;
				}
				b->wait1-=fps_factor;
			}
			break;
		case 7:
			if (s->aktframe>15) {
				b->wait3+=fps_factor;
				if (b->wait3>=3) {
					s->aktframe--;
					b->wait3=0;
				}
			}
			enemy_boss04_waitstate(s, 8, 15);
			break;
		case 8:
			if ((int)b->wait2%10==0)
			{
				if (s->aktframe==18)
				{
					s->aktframe=5;
					playChunk(11);
					enemy_boss04_more_angle(s, (2.5-0.3)*256+b->wait2*(256/10), /*angle4096*/(	4*256), rad2deg4096(0.013));
					enemy_boss04_more_angle(s, (2.5-0.6)*256+b->wait2*(256/10), /*angle4096*/(	2*256), rad2deg4096(0.011));
					enemy_boss04_more_angle(s, (2.5-0.9)*256+b->wait2*(256/10), /*angle4096*/(	0	 ), rad2deg4096(0.009));
					enemy_boss04_more_angle(s, (2.5-1.2)*256+b->wait2*(256/10), /*angle4096*/( 14*256), rad2deg4096(0.007));
					enemy_boss04_more_angle(s, (2.5-1.5)*256+b->wait2*(256/10), /*angle4096*/( 12*256), rad2deg4096(0.005));
					if (difficulty>1)
					{
						enemy_boss04_more_angle(s, (2.5-1.8)*256+b->wait2*(256/10), /*angle4096*/( 10*256), rad2deg4096(0.003));
						enemy_boss04_more_angle(s, (2.5-2.1)*256+b->wait2*(256/10), /*angle4096*/(	8*256), rad2deg4096(0.001));
					}
					enemy_boss04_more_angle(s, (2.5-0.3)*256+b->wait2*(256/10), /*angle4096*/(	4*256), rad2deg4096(-0.013));
					enemy_boss04_more_angle(s, (2.5-0.6)*256+b->wait2*(256/10), /*angle4096*/(	2*256), rad2deg4096(-0.011));
					enemy_boss04_more_angle(s, (2.5-0.9)*256+b->wait2*(256/10), /*angle4096*/(	0	 ), rad2deg4096(-0.009));
					enemy_boss04_more_angle(s, (2.5-1.2)*256+b->wait2*(256/10), /*angle4096*/( 14*256), rad2deg4096(-0.007));
					enemy_boss04_more_angle(s, (2.5-1.5)*256+b->wait2*(256/10), /*angle4096*/( 12*256), rad2deg4096(-0.005));
					if (difficulty>1)
					{
						enemy_boss04_more_angle(s, (2.5-1.8)*256+b->wait2*(256/10), /*angle4096*/( 10*256), rad2deg4096(-0.003));
						enemy_boss04_more_angle(s, (2.5-2.1)*256+b->wait2*(256/10), /*angle4096*/(	8*256), rad2deg4096(-0.001));
					}
					b->state2=1;
				}
				else
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			else
			{
				b->state2=1;
				s->aktframe=16;
			}
			break;
		}
		enemy_boss04_out(s);
		break;
	//
	case 2: 	//第三形態:ナイフが落ちてくるよ
		#define SAKUYA_LIMIT_Y (GAME_HEIGHT-32)
		switch (b->state2)
		{
		case 0:
			if (b->wait1<0) 	//待機時間が終わったら
			{
				b->state2			=1;
				pd->bossmode		=B01_BATTLE;
				b->move_angle512	=atan_512(-s->y,-s->x);
				s->aktframe 		=4;
			}
			else
			{
				b->wait1-=fps_factor;
			}
			break;
		case 1:
			if (s->x<5) 	//左隅に来たら		/* → || (s->y<5)があると無限ループになる*/
			{
				b->state2=2;
				b->wait1=10;
				b->wait2=0;
				b->move_angle512=atan_512(50-s->y,GAME_WIDTH-s->w-s->x);
				s->aktframe=0;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3*fps_factor;
				s->y+=si_n512((b->move_angle512))*3*fps_factor;
					 if (s->y < 0)					{	s->y = 0;						}/* はみ出したら修正 */
				else if (s->y > (SAKUYA_LIMIT_Y) )	{	s->y = (SAKUYA_LIMIT_Y);		}/* はみ出したら修正 */
				if (s->aktframe>0)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe--;
						b->wait3=0;
					}
				}
			}
			break;
		case 2:
			enemy_boss04_waitstate(s, 3, 2);
			break;
		case 3:
			if (s->x > GAME_WIDTH-s->w+3)	//右端に来たら
			{
				b->state2=4;
				b->move_angle512=atan_512(50-s->y,GAME_WIDTH/2-s->w/2-s->x);
				b->wait2=0;
				s->aktframe=8;
			}
			else
			{
				{	const short sss[4]=
					{
						t256(1.2),/*まばらなので簡単*/
						t256(1.5),/*塊なので大きく避ければ簡単*/
						t256(2.0),/*たまに難しい時がある*/
						t256(6.5) /*うへぇこのスキマくぐるんですかぁ？*/
					};
					b->wait2+=/*4*256*/sss[difficulty]/*(3*256*1.42)*/;
				}
				if (0==(((int)b->wait2)&(16-1)/*%21(15*256*1.42)*/))
				{
					enemy_fall_knife_create(s, 2, (deg_360_to_512(360)-((((int)(b->wait2))>>8)&(256-1)/*%(180*1.42)*/)), t256(0.03));
					//if (difficulty)
					{
						enemy_fall_knife_create(s, 2, (deg_360_to_512(270)-((((int)(b->wait2))>>8)&(256-1)/*%(180*1.42)*/)), t256(0.04));
					//	if (difficulty>1)
						{	enemy_fall_knife_create(s, 2, (deg_360_to_512(180)-((((int)(b->wait2))>>8)&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
					playChunk(12);
				}
				s->x+=co_s512((b->move_angle512))*5*fps_factor;
				s->y+=si_n512((b->move_angle512))*5*fps_factor;
					 if (s->y < 0)					{	s->y = 0;						}/* はみ出したら修正 */
				else if (s->y > (SAKUYA_LIMIT_Y) )	{	s->y = (SAKUYA_LIMIT_Y);		}/* はみ出したら修正 */
				if (s->aktframe<8)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=7)
					{
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			break;
		case 4:
			if (s->x<GAME_WIDTH/2-s->w/2)		//真ん中に来たら
			{
				b->state2=5;
				b->move_angle512=atan_512(-s->y,GAME_WIDTH-s->w-s->x);
				s->aktframe=4;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3*fps_factor;
				s->y+=si_n512((b->move_angle512))*3*fps_factor;
					 if (s->y < 0)					{	s->y = 0;						}/* はみ出したら修正 */
				else if (s->y > (SAKUYA_LIMIT_Y) )	{	s->y = (SAKUYA_LIMIT_Y);		}/* はみ出したら修正 */
				if (s->aktframe>4)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe--;
						b->wait3=0;
					}
				}
			}
			break;
		case 5:
			if (s->x>GAME_WIDTH-s->w-3) 	//右端に来たら	/* → || (s->y<5)があると無限ループになる */
			{
				b->state2=6;
				b->wait1=10;
				b->wait2=0;
				b->move_angle512=atan_512(50-s->y,-s->x);
				s->aktframe=8;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3*fps_factor;
				s->y+=si_n512((b->move_angle512))*3*fps_factor;
					 if (s->y < 0)					{	s->y = 0;						}/* はみ出したら修正 */
				else if (s->y > (SAKUYA_LIMIT_Y) )	{	s->y = (SAKUYA_LIMIT_Y);		}/* はみ出したら修正 */
				if (s->aktframe<8)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			break;
		case 6:
			if (s->x<5) 	//左端に来たら
			{
				b->state2=7;
				b->move_angle512=atan_512(50-s->y,GAME_WIDTH/2-s->w/2-s->x);
				b->wait2=0;
				s->aktframe=0;
			}
			else
			{
				b->wait2+=4/*(3*1.42)*/;
				if (0==(((int)b->wait2)&(16-1))/*%(12*1.42)*/)
				{
					enemy_fall_knife_create(s, 3, ((((int)b->wait2)&(256-1)/*%(180*1.42)*/)+deg_360_to_512(180)), t256(0.03));
					if (difficulty)
					{
						enemy_fall_knife_create(s, 3, ((((int)b->wait2)&(256-1)/*%(180*1.42)*/)+deg_360_to_512(90)), t256(0.04));
						if (difficulty>1)
						{	enemy_fall_knife_create(s, 3, ((((int)b->wait2)&(256-1)/*%(180*1.42)*/)), t256(0.02));}
					}
					playChunk(12);
				}
				s->x+=co_s512((b->move_angle512))*5*fps_factor;
				s->y+=si_n512((b->move_angle512))*5*fps_factor;
					 if (s->y < 0)					{	s->y = 0;						}/* はみ出したら修正 */
				else if (s->y > (SAKUYA_LIMIT_Y) )	{	s->y = (SAKUYA_LIMIT_Y);		}/* はみ出したら修正 */
				if (s->aktframe>0)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=7)
					{
						s->aktframe--;
						b->wait3=0;
					}
				}
			}
			break;
		case 7:
			if (s->x>GAME_WIDTH/2-s->w/2)		//真ん中に来たら
			{
				b->state2=8;
				b->move_angle512=atan_512(50-s->y,GAME_WIDTH/2-s->w/2-s->x);
				b->wait2=60;
				s->aktframe=4;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3*fps_factor;
				s->y+=si_n512((b->move_angle512))*3*fps_factor;
					 if (s->y < 0)					{	s->y = 0;						}/* はみ出したら修正 */
				else if (s->y > (SAKUYA_LIMIT_Y) )	{	s->y = (SAKUYA_LIMIT_Y);		}/* はみ出したら修正 */
				if (s->aktframe<4)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			break;
		case 8:
			b->wait2--;
			if (b->wait2<0)
			{
				b->state2=1;
				s->aktframe=4;
			}
			else
			{
				if ((int)b->wait2%20==0)
				{
					int b_wait2_high;
					b_wait2_high=(((int)(b->wait2/20)));
					s->aktframe=(s->aktframe+1)%7;
						enemy_boss04_knifes1(s, (b_wait2_high<<8)+t256(4.0), deg_360_to_512(30)-(b_wait2_high)*deg_360_to_512(40), -80);
						enemy_boss04_knifes1(s, (b_wait2_high<<8)+t256(3.0), deg_360_to_512(30)-(b_wait2_high)*deg_360_to_512(30), -60);
					if (difficulty>1)
					{
						enemy_boss04_knifes1(s, (b_wait2_high<<8)+t256(2.0), deg_360_to_512(30)-(b_wait2_high)*deg_360_to_512(20), -40);
					}
					s->aktframe=18;
				}
			}
			if (s->aktframe>15)
			{
				b->wait3+=fps_factor;
				if (b->wait3>=4)
				{
					s->aktframe--;
					b->wait3=0;
				}
			}
		}
		break;
	//
	case 3: 	//第四形態: (黄色マスカット弾)
		switch (b->state2)
		{
		case 0:
			if (b->wait1<0) 	//待機時間が終わったら
			{
				b->state2=1;
				pd->bossmode=B01_BATTLE;
				b->move_angle512=atan_512(50-s->y,GAME_WIDTH/2-s->w-s->x);
				b->sp_time+=1000;
				s->aktframe=15;
			}
			else
			{
				b->wait1-=fps_factor;
			}
			break;
		case 1:
			if (s->y>50 && (s->x<GAME_WIDTH/2+30 || s->x>GAME_WIDTH/2-30))		//真ん中に来たら
			{
				b->state2=2;
				b->wait1=10;
				b->wait2=0;
				s->aktframe=9;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3*fps_factor;
				s->y+=si_n512((b->move_angle512))*3*fps_factor;
				if (s->aktframe<18)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			break;
		case 2:
			if (s->aktframe==14)
			{
				enemy_boss04_maho_create(s);		//魔方陣生成
				b->state2=3;
				b->wait2=10;
				b->wait1=5;
			}
			else
			{
				if (s->aktframe<14)
				{
					b->wait3+=fps_factor;
					if (b->wait3>=3)
					{
						s->aktframe++;
						b->wait3=0;
					}
				}
			}
			break;
		case 3:
			b->angle512=atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
			b->wait2-=fps_factor;
			if (b->wait2<0)
			{
				b->wait2=50;
				/* (黄色マスカット弾) */
				enemy_sp1_bullet_create(s, (t256(1.2)), b->angle512-1*42/*deg_360_to_512(30)*/, (t256(0.030)),	1/* 1==R*/);
				enemy_sp1_bullet_create(s, (t256(1.2)), b->angle512+1*42/*deg_360_to_512(30)*/, (t256(0.030)), -1/*-1==L*/);
				enemy_sp1_bullet_create(s, (t256(1.5)), b->angle512-2*42/*deg_360_to_512(30)*/, (t256(0.033)),	1/* 1==R*/);
				enemy_sp1_bullet_create(s, (t256(1.5)), b->angle512+2*42/*deg_360_to_512(30)*/, (t256(0.033)), -1/*-1==L*/);
				if (difficulty)
				{
					enemy_sp1_bullet_create(s, (t256(1.8)), b->angle512-3*42/*deg_360_to_512(30)*/, (t256(0.036)),	1/* 1==R*/);
					enemy_sp1_bullet_create(s, (t256(1.8)), b->angle512+3*42/*deg_360_to_512(30)*/, (t256(0.036)), -1/*-1==L*/);
					if (difficulty>1)
					{
						enemy_sp1_bullet_create(s, (t256(2.1)), b->angle512-4*42/*deg_360_to_512(30)*/, (t256(0.04)),  1/* 1==R*/);
						enemy_sp1_bullet_create(s, (t256(2.1)), b->angle512+4*42/*deg_360_to_512(30)*/, (t256(0.04)), -1/*-1==L*/);
					}
				}
				//enemy_bigbullet_create(s, s->x+s->w/2, s->y+s->h/2, 2, -2, 40);
				b->wait1-=fps_factor;
				if (b->wait1<0) 	//3回撃ったら
				{
					b->wait1=20;
					if ((rand()&(2-1)/*%2*/)==0)
					{
						b->state2=4;
						s->aktframe=2;
					}
					else
					{
						b->state2=5;
						s->aktframe=6;
					}
				}
			}
			if (s->aktframe>9)
			{
				b->wait3+=fps_factor;
				if (b->wait3>=3)
				{
					b->wait3=0;
					s->aktframe--;
				}
			}
			break;
		case 4:
			b->wait1-=fps_factor;
			if (b->wait1<0)
			{
				b->wait1=3;
				b->state2=3;
				b->wait2=30;
				s->aktframe=18;
			}
			else
			{
				if (s->x<100)
				{
					b->state2=5;
					s->aktframe=7;
				}
				else
				{	s->x-=2*fps_factor;}
			}
			break;
		case 5:
			b->wait1-=fps_factor;
			if (b->wait1<0)
			{
				b->wait1=3;
				b->state2=3;
				b->wait2=30;
				s->aktframe=18;
			}
			else
			{
				if (s->x>GAME_WIDTH-100-s->w)
				{
					b->state2=4;
					s->aktframe=1;
				}
				else
				{	s->x+=2*fps_factor;}
			}
			break;
		}
		break;
	//
	case 4: 	//第五形態:
#if 1
		switch (b->state2)
		{
		case 0:
			if (b->wait1<0) 	//待機時間が終わったら
			{
				b->state2=1;
				pd->bossmode=B01_BATTLE;
				b->move_angle512=atan_512(30-s->y,GAME_WIDTH/2-s->w/2-s->x);
			}
			else
			{
				b->wait1-=fps_factor;
			}
			break;
		case 1:
			if ((s->x<GAME_WIDTH/2+30 && s->x>GAME_WIDTH/2-10)&&s->y<50)		//真ん中に来たら
			{
				b->state2=2;
				b->wait1=20;
				b->wait2=0;
				s->aktframe=4;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3*fps_factor;
				s->y+=si_n512((b->move_angle512))*3*fps_factor;
			}
			break;
		case 2:
			if (b->wait1<0){	b->state2=3;s->aktframe=15; 	}		//待機時間が終わったら
			else if (b->wait1<6){			s->aktframe=14; 	}
			else if (b->wait1<12){			s->aktframe=13; 	}
			else if (b->wait1<18){			s->aktframe=12; 	}
			else if (b->wait1<24){			s->aktframe=11; 	}
			else if (b->wait1<30){			s->aktframe=10; 	}
			else if (b->wait1<36){			s->aktframe=9;		}
			b->wait1-=fps_factor;
			break;
		case 3:
			if (s->aktframe==18)
			{
			//	距離を求めるのに２乗してルートを取るのはルート演算が遅すぎるのでゲーム中は無理。
			//	予めテーブル化しておくか、代替演算(CODIC)で求めるかどちらか。
			//	(double 使わないで int CODIC なら sqrt() も、おそらくpspでも実用的な速度が出る)
			//	b->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
					int xxx;
					int yyy;
					xxx=abs((int)player->x-(int)s->x);xxx++;/* abs()は展開してくれるから大丈夫 */
					yyy=abs((int)player->y-(int)s->y);yyy++;
					if (xxx>yyy)	{b->length_s_p=(xxx/*+(yyy>>1)*/);}/*少し難しくする*/
					else			{b->length_s_p=(yyy/*+(xxx>>1)*/);}/*少し難しくする*/
				}
#if 1
				enemy_even_knife_create(s, 10, b->length_s_p, 0/*0==右*/);
				#if 1
				enemy_even_knife_create(s, 10, b->length_s_p, 1/*1==左*/);
				#endif
#endif
				b->wait1=40;
				if (s->y>150)
				{	b->move_angle512=deg_360_to_512(210) + (rand()%(deg_360_to_512(120)));}
				else
				{	b->move_angle512=/*deg512_2rad*/((rand()&(512-1)))/*degtorad(rand()%360)*/;}
				#if 1
				/*念の為*/
				mask512(b->move_angle512);
				#endif
				s->aktframe=16;
				b->state2=4;
			}
			else
			{
				b->wait3+=fps_factor;
				if (b->wait3>=3)
				{
					s->aktframe++;
					b->wait3=0;
				}
			}
			break;
		case 4:
			if (b->wait1<0)
			{
				b->state2=2;
				b->wait1=120+(3-difficulty)*50;
			}
			else
			{
				s->x+=co_s512((b->move_angle512))*3.5*fps_factor;
				s->y+=si_n512((b->move_angle512))*3.5*fps_factor;
				b->wait1-=fps_factor;
			}
			break;
		}
		if ((player->x+player->w/2 < s->x+s->w/2+25)&&
			(player->x+player->w/2 > s->x+s->w/2-25))
		{	b->wait2+=fps_factor;}
		if (b->wait2>20+(3-difficulty)*30)
		{
			s->aktframe=16;
		//	距離を求めるのに２乗してルートを取るのはルート演算が遅すぎるのでゲーム中は無理。
		//	予めテーブル化しておくか、代替演算(CODIC)で求めるかどちらか。
		//	(double 使わないで int CODIC なら sqrt() も、おそらくpspでも実用的な速度が出る)
		//	b->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				{	/* sqrt()遅いので簡易版。というか距離には拘らない事にした */
					int xxx;
					int yyy;
					xxx=abs((int)player->x-(int)s->x);xxx++;/* abs()は展開してくれるから大丈夫 */
					yyy=abs((int)player->y-(int)s->y);yyy++;
					if (xxx>yyy)	{b->length_s_p=(xxx/*+(yyy>>1)*/);}/*少し難しくする*/
					else			{b->length_s_p=(yyy/*+(xxx>>1)*/);}/*少し難しくする*/
				}
			if (b->length_s_p>80)
			{	b->length_s_p=80;}
			enemy_boss04_knifes3(s/*,*/ /*1+difficulty,*/ /*(b->length_s_p)*/ );
		}
		enemy_boss04_out(s);
#endif
		break;

	//
	case 5: 	//第6形態:
#if 1
		if (b->wait2<0)
		{
			b->wait2=0;
			b->wait1=120;
			b->state1=4;
			b->state2=2;
			s->aktframe=4;
		}
		else
		{
			b->wait2-=0.03;
			if (s->alpha>249)//if (s->alpha>255)
			{
				s->alpha=255;
			}
			else //if (s->alpha<255)
			{
				s->alpha+=5;
			}
			s->x+=co_s512((b->angle512))*b->wait2*fps_factor;
			s->y+=si_n512((b->angle512))*b->wait2*fps_factor;
		}
		if (s->aktframe>15)
		{
			b->wait3+=fps_factor;
			if (b->wait3>=4)
			{
				s->aktframe--;
				b->wait3=0;
			}
		}
		enemy_boss04_out(s);
#endif
		break;
	}
	if ((b->sp_time<0) && (b->state1<4))
	{
		playChunk(7);
		bonus_multi_add(s, SP_BONUS_00_FIREPOWER,   5, BONUS_FLAG_RAND_XY);
		bonus_multi_add(s, SP_BONUS_01_FIREPOWER_G, 1, BONUS_FLAG_RAND_XY);
		pd->bossmode		= B04_CHANGE;
		b->enemy_base.health		= (4-b->state1)*1024-1;

//レミリア戦の開始イベントが見れなくなっちゃうのでここは無効
//				/* vs REMIRIA */
//				if (2==select_player)
//				{b->enemy_base.health=1;}

		b->state1++;
		b->state2			= 0;
		b->wait1			= 80;
		b->wait2			= -100;
		b->sp_time			= 3000;
		s->aktframe 		= 4;
	}

	#if 0/*十六夜本人デバッグ用*/
	/* パネルのスコア欄にstate1を、グレイズ欄にstate2を表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score=b->state1;
	((PLAYER_DATA *)player->data)->graze=b->state2;
	#endif
}

void enemy_boss04_add(int lv)
{
	BOSS04_DATA *b;
	SPRITE *s;
	s				= sprite_add_file("boss04.png",19,PR_ENEMY);	s->anim_speed=0;
	s->flags		|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
	s->aktframe 	= 4;
	s->type 		= SP_EN_BOSS/*SP_EN_BOSS04*/;
	s->mover		= enemy_boss04_move;
	s->x			= GAME_WIDTH/2-(s->w/2);
	s->y			= -(s->h);

	b				= mmalloc(sizeof(BOSS04_DATA));
	s->data 		= b;

	b->enemy_base.health=((5*1024)-1);		/*5119==((5*1024)-1)*/

//レミリア戦の開始イベントが見れなくなっちゃうのでここは無効
//				/* vs REMIRIA */
//				if (2==select_player){b->enemy_base.health=1;}

	b->enemy_base.score 	= score(5000)+score(4000)*difficulty;
	b->state1		= 0;
	b->state2		= 0;
	b->wait1		= 0;
	b->wait2		= 0;
	b->wait3		= 0;
	b->type 		= 0;
	b->level		= lv;
	b->move_angle512	= (0);
	b->use_sp_time	= 0;/*OFF*/
	b->sp_time		= 1000;

	((PLAYER_DATA *)player->data)->boss=s;
	((PLAYER_DATA *)player->data)->bossmode=B01_BATTLE;
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss04_hitbyweapon_dummy;	/* ダミーコールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyplayer=callback_enemy_boss04_hitbyplayer;
}
