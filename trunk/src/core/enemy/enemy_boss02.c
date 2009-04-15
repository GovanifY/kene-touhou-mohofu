/*
射命丸 文
天狗
*/

#include "enemy.h"

/*
	自分で思ったよりも第2形態の攻撃パターンがカオスになってしまった。
	もっと丁寧に作り直した方がいいかも。
	***090114
		天狗の速度を難易度ごとに変えてみた。
*/

typedef struct _boss02_data
{
	ENEMY_BASE b;
	/*static*/ /*double*/int angle512/*angle_radian*/;/*radianで持つよりintで持って使う度に変換した方が速い*/
	int type;	/* 0: normal, 1: destroyed */
	int state1;
	int state2; 		//攻撃パターン変更用
	double wait1;		//移動待ち
	double wait2;		//モーション変更用
	int xp;
	int n_wayt;
	int level;
} BOSS02_DATA;

typedef struct
{
	int boss_id;
	/*double*/int angle512;
	double wait;
} BOSS02_SR_DATA;

typedef struct
{
	int no;
	int stat;
	/*double*/int angle512;
	double time;
	double speed;
} BOSS02_ICE_DATA;


static void callback_enemy_boss02_hitbyplayer(SPRITE *c)
{
}

static void callback_enemy_boss02_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/)
{
	BOSS02_DATA *b=(BOSS02_DATA *)c->data;
//	SPRITE *d=c;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	explosion_add(s->x,s->y,0,EXPLOSION_MINI00);
//	parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);
	b->b.health-=w->strength;

	if (b->b.health<=(1024*3/*1024*2*/) && (b->state2==0) )
	{
		bonus_multi_add(c, SP_BONUS_00_FIREPOWER,   3, BONUS_FLAG_RAND_XY);
		bonus_multi_add(c, SP_BONUS_01_FIREPOWER_G, 1, BONUS_FLAG_RAND_XY);
		b->state2=1;
	}
	else if ((b->b.health<=(1024*2/*1024*1*/)) && (b->type==0) )
	{
		//HPが残り1024になった時。グラフィックの差し替え。攻撃パターン変更
		explosion_add_circle(/*d*/c);
		bonus_multi_add(c, SP_BONUS_00_FIREPOWER,   3, BONUS_FLAG_RAND_XY);
		bonus_multi_add(c, SP_BONUS_01_FIREPOWER_G, 1, BONUS_FLAG_RAND_XY);
//グラフィックの差し替えは廃止。
		((PLAYER_DATA *)player->data)->bossmode=B04_CHANGE;
//		d=sprite_add_file("boss02_v2.png"/*"boss02_v2x.png"*/,9,PR_ENEMY);	d->anim_speed=0/*c->anim_speed*/;
//		d->flags=c->flags;
//		d->aktframe=c->aktframe;
//		d->type=c->type;
//		BOSS02_DATA *b2;
//		b2=mmalloc(sizeof(BOSS02_DATA));
//		memcpy(b2,b,sizeof(BOSS02_DATA));
//		b=b2;
		b->type=1;
//		d->data=b2;
//		d->mover=c->mover;
//		d->x=c->x;
//		d->y=c->y;
		/*b2*/b->state2=2;
		/*b2*/b->state1=13;
		/*b2*/b->wait1=40;

//		c->type=SP_DELETE;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
//		((PLAYER_DATA *)player->data)->boss=d;
	}

	if (b->b.health<=0)
	{
		explosion_add_circle(/*d*/c);
		bonus_multi_add(s, SP_BONUS_06_COIN, 7, BONUS_FLAG_COLLECT);
		/*d*/c->type=SP_DELETE;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		((PLAYER_DATA *)player->data)->bossmode=B02_DEATH_WAIT;
	}
}

static void enemy_boss02_sr_controller1(CONTROLLER *c)
{
	BOSS02_SR_DATA *e=(BOSS02_SR_DATA *)c->e;
	if (e->wait>0)
	{
		e->wait-=fps_factor;
	}
	else
	{
		SPRITE *src;/* srcはbossオブジェ */
		src 		= sprite_get_by_id(e->boss_id); //なければNULL
		e->wait 	= 10;
		e->angle512 += deg_360_to_512(15);
		if ((e->angle512 > deg_360_to_512(360-15))||(src==NULL))
		{
			controller_remove(c);
		}
		else
		{
			bullet_create_haridan180(src, e->angle512, t256(7), /*offsx*/0-16/*(src->w/2)*/, /*offsy*/0/*(src->h/2)*/ );
		}
	}
}

static void enemy_boss02_sr_add1(SPRITE *c)
{
	CONTROLLER *s;
	BOSS02_SR_DATA *b;
	s=controller_add();
	b=mmalloc(sizeof(BOSS02_SR_DATA));
	b->angle512 	= deg_360_to_512(0);
	b->wait 		= 10;
	b->boss_id		= c->id;
	s->con			= enemy_boss02_sr_controller1;
	s->e			= b;
}

static void enemy_boss02_sr_controller2(CONTROLLER *c)
{
	BOSS02_SR_DATA *e=(BOSS02_SR_DATA *)c->e;
	if (e->wait>0)
	{
		e->wait-=fps_factor;
	}
	else
	{
		SPRITE *src;/* srcはbossオブジェ */
		src 		= sprite_get_by_id(e->boss_id); //なければNULL
		e->wait 	= 10;
		e->angle512 -= deg_360_to_512(15);
		if ((e->angle512 <= deg_360_to_512(15))||(src==NULL))
		{
			controller_remove(c);
		}
		else
		{
			bullet_create_haridan180(src, e->angle512, t256(7), /*offsx*/0-16/*(src->w/2)*/, /*offsy*/0/*(src->h/2)*/ );
		}
	}
}

static void enemy_boss02_sr_add2(SPRITE *c)
{
	CONTROLLER *s;
	BOSS02_SR_DATA *b;
	s			= controller_add();
	b			= mmalloc(sizeof(BOSS02_SR_DATA));
	b->angle512 = deg_360_to_512(360)-1;/*追加 (-1)*/
	b->wait 	= 8;
	b->boss_id	= c->id;
	s->con		= enemy_boss02_sr_controller2;
	s->e		= b;
}

static void enemy_boss02_ice_move(SPRITE *s)
{
	BOSS02_ICE_DATA *d=(BOSS02_ICE_DATA *)s->data;
	switch (d->stat)
	{
	case 0:
		d->angle512 = deg_360_to_512(90);
		d->time 	= 100;
		d->speed	= 4;
		d->stat 	= 1;
		break;
	case 1:
		if (d->no==0)	{	d->angle512 += deg_360_to_512(5)/**fps_factor*/;}
		else			{	d->angle512 -= deg_360_to_512(5)/**fps_factor*/;}
		d->time-=fps_factor;
		if (d->time<=0)
		{	d->stat=2;}
		break;
	case 2:
		d->angle512 = (rand()&(128-1)/*%90*/)+deg_360_to_512(45);
		d->speed=12;
		d->stat=3;
		break;
	}

//	if (d->angle512 < deg_360_to_512(0))	{	d->angle512 += deg_360_to_512(360);}
//	if (d->angle512 > deg_360_to_512(360))	{	d->angle512 -= deg_360_to_512(360);}
	mask512(d->angle512);

	/*以下rwingx.cと同じ*/
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
//	s->aktframe=(deg_512_to_360(d->angle512+deg_360_to_512(270))/10)%36;
	s->aktframe = ((((d->angle512+deg_360_to_512(270))&(512-1))*(36/2))>>8);
	/*(ここまで)[以下rwingx.cと同じ]*/
//
	if ((s->x<-100)||(s->x>100+GAME_WIDTH)||
		(s->y<-100)||(s->y>100+GAME_HEIGHT) )
	{	s->type=SP_DELETE;}
}


#define NUM_OF_ENEMIES (30)
static void enemy_boss02_ice_controller(CONTROLLER *c)
{
	int *id_array=c->e;

	SPRITE *s;
	int i;
	int dead=0;
//	SPRITE *master;
//	master=sprite_get_by_id(id_array[0]);
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
			if (s!=NULL)
			{	s->type=SP_DELETE;}
		}
		controller_remove(c);
	//	return;
	}
	else
	{
		for (i=NUM_OF_ENEMIES-1;i>=1;i--)
		{
			SPRITE *l;
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if ((s!=NULL)&&(l!=NULL))
			{
				s->x=l->x;
				s->y=l->y;
				s->aktframe=l->aktframe;
			}
		}
	}
}

static void enemy_boss02_ice_add(SPRITE *c)
{
	CONTROLLER *e;
	int *id_array;
	SPRITE * d;
	int i,j;
	BOSS02_ICE_DATA *l;
	for (j=0;j<2;j++)
	{
		e=controller_add();
	//	NUM_OF_ENEMIES=30;
		id_array=mmalloc(sizeof(int)*NUM_OF_ENEMIES);
		e->e=id_array;
		e->con=enemy_boss02_ice_controller;
		for (i=0; i<NUM_OF_ENEMIES; i++)
		{
			d=sprite_add_file("boss02_w.png",36,PR_BULLETS/*PR_ENEMY_WEAPON*/);
			id_array[i]=d->id;
			d->type=SP_EN_BOSS02ICE;
			d->flags|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
			if (i==0)
			{
				d->mover=enemy_boss02_ice_move;
				l=mmalloc(sizeof(BOSS02_ICE_DATA));
				d->data=l;
				l->no=j;
				l->stat=0;
			}
			else
			{
				d->mover=NULL;
				d->data=NULL;
			}
			if (j==0)
			{
				d->x=c->x-19;
				d->y=c->y+135;
			}
			else
			{
				d->x=c->x+81;
				d->y=c->y+138;
			}
		}
	}
}
#undef NUM_OF_ENEMIES

static void enemy_boss02_waitstate(SPRITE *s, int nextstate)
{
	BOSS02_DATA *b=(BOSS02_DATA *)s->data;

	if (b->wait1>0)
	{
		b->wait1-=fps_factor;
		if (s->aktframe<4)
		{
			b->wait2+=fps_factor;
			if (b->wait2>=3)
			{
				s->aktframe++;
				b->wait2=0;
			}
		}
		else if (s->aktframe>4)
		{
			b->wait2+=fps_factor;
			if (b->wait2>=3)
			{
				s->aktframe--;
				b->wait2=0;
			}
		}
	}
	else
	{
		b->state1=nextstate;
		enemy_bigbullet_create(s, s->x+s->w/2, s->y+s->h/2, 4+difficulty/2, 999/*-2*/, 10);
	}
}
static void enemy_boss02_move(SPRITE *s)
{
	BOSS02_DATA *b=(BOSS02_DATA *)s->data;

	switch (b->state2)
	{
	case 0: 		//攻撃パターン1
	case 1:
		switch (b->state1)
		{
		case 0: /* nach unten */
			if (s->y<30)
			{	s->y+=2*fps_factor;}
			else
			{
				b->state1=12;
				((PLAYER_DATA *)player->data)->bossmode=B05_BEFORE_LOAD;
				b->wait1=0;
			}
			break;

		case 12:
			if (((PLAYER_DATA *)player->data)->bossmode==B01_BATTLE)
			{
				b->wait1+=fps_factor;
				if (b->wait1>=40)
				{
					b->state1=1;
				}
			}
			break;

		case 1: /* y-pos erreicht: rundumschuss */
			enemy_boss02_sr_add1(s);
			if (b->state2==0)
			{	enemy_boss02_ice_add(s);}
			b->wait1=20;
			b->state1=2;
			if (b->state2==1)
			{
				enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3-2)) );
				enemy_momiji_create(s, 4,	deg_360_to_512(30*(3  )) );
				enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3+2)) );
			}
			break;

		case 2: /* warten */
			enemy_boss02_waitstate(s,3);
			break;

		case 3: /* nach links bis zum rand */
			if (s->x>=0)
			{
				s->x-=2*fps_factor;
				if (s->aktframe>0)
				{
					b->wait2+=fps_factor;
					if (b->wait2>=3)
					{
						s->aktframe--;
						b->wait2=0;
					}
				}
			}
			else
			{
				b->wait1=20;
				b->state1=4;
				if (b->state2==1)
				{
					enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3-2)) );
					enemy_momiji_create(s, 4,	deg_360_to_512(30*(3  )) );
					enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3+2)) );
				}
			}
			break;

		case 4: /* warten */
			enemy_boss02_waitstate(s,5);
			break;

		case 5: /* nach rechts bis zur mitte */
			if (s->x<b->xp)
			{
				s->x+=2*fps_factor;
				if (s->aktframe<8)
				{
					b->wait2+=fps_factor;
					if (b->wait2>=3)
					{
						s->aktframe++;
						b->wait2=0;
					}
				}
			}
			else
			{
				enemy_boss02_sr_add1(s);
				b->wait1=100;
				b->state1=6;
				if (b->state2==1)
				{
					enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3-2)) );
					enemy_momiji_create(s, 4,	deg_360_to_512(30*(3  )) );
					enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3+2)) );
				}
			}
			break;

		case 6: /* warten */
			enemy_boss02_waitstate(s,7);
			break;

		case 7:
			if (b->state2==0)
			{	enemy_boss02_ice_add(s);}
			b->wait1=100;
			b->state1=8;
			break;

		case 8: /* warten */
			enemy_boss02_waitstate(s,9);
			break;

		case 9: /* nach rechts bis zum rand */
			if (s->x<(GAME_WIDTH-s->w))
			{
				s->x+=2*fps_factor;
				if (s->aktframe<8)
				{
					b->wait2+=fps_factor;
					if (b->wait2>=3)
					{
						s->aktframe++;
						b->wait2=0;
					}
				}
			}
			else
			{
				b->wait1=20;
				b->state1=10;
				if (b->state2==1)
				{
					enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3-2)) );
					enemy_momiji_create(s, 4,	deg_360_to_512(30*(3  )) );
					enemy_momiji_create(s, 3.5, deg_360_to_512(30*(3+2)) );
				}
			}
			break;

		case 10: /* warten */
			enemy_boss02_waitstate(s,11);
			break;

		case 11: /* nach links bis zur mitte */
			if (s->x>b->xp)
			{
				s->x-=2*fps_factor;
				if (s->aktframe>0)
				{
					b->wait2+=fps_factor;
					if (b->wait2>=3)
					{
						s->aktframe--;
						b->wait2=0;
					}
				}
			}
			else
			{	b->state1=1;	}
			break;
		}
		break;

	case 2: 	//攻撃パターン2
		switch (b->state1)
		{
		case 0: 	//不定:初期位置情報の取得->1へ
			b->angle512/*_radian*/=atan_512(20-s->y,GAME_WIDTH/2-s->w-s->x);
			b->state1=1;
			break;

		case 1: 	//不定:初期位置へ戻る->2へ
			if (s->y<30)
			{
				b->state1=2;
				b->wait1+=10+(3-difficulty)*10;
				if ((b->n_wayt%3==0))
				{
					enemy_n_way_bullet_type(s, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 8, 5, ANGLE_JIKINERAI_KISUDAN);	/*なるべく共通化*/	/*atan_512(player->y-s->y,player->x-s->x)*/
				}
				if ( (b->n_wayt%4==0) && (b->n_wayt%12!=0) )
				{
					enemy_n_way_bullet_type(s, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 11, 4, ANGLE_JIKINERAI_KISUDAN);	/*なるべく共通化*/	/*atan_512(player->y-s->y,player->x-s->x)*/
					enemy_n_way_bullet_type(s, BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 11, 3, ANGLE_JIKINERAI_KISUDAN);	/*なるべく共通化*/	/*atan_512(player->y-s->y,player->x-s->x)*/
				}
			}
			else
			{
				s->x+=co_s512((b->angle512/*_radian*/))*6*fps_factor;
				s->y+=si_n512((b->angle512/*_radian*/))*6*fps_factor;
				b->state1=0;
			}
			break;

		case 2: 	//初期位置:大玉3つ->3,4,5
			if (b->wait1<0)
			{
				enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 5, atan_512(player->y-s->y,player->x-s->x)-deg_360_to_512(30), 1); //大弾の追加
				enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 5, atan_512(player->y-s->y,player->x-s->x) 				   	 , 1);
				enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 5, atan_512(player->y-s->y,player->x-s->x)+deg_360_to_512(30), 1);
				b->state1=rand()%3+3;
				if (b->state1==5)
				{	b->angle512/*_radian*/=atan_512(player->y-s->y,player->x-s->x);}
				else
				{
					s->y+=6*fps_factor;
				}
			}
			else
			{
				b->wait1-=fps_factor;
			}
			break;

		case 3: 	//左移動中->6へ
			if (s->x>=0)
			{
				s->x-=(4+difficulty)*fps_factor;
				if (s->aktframe>0)
				{
					b->wait2+=fps_factor;
					if (b->wait2>=3)
					{
						s->aktframe--;
						b->wait2=0;
					}
				}
			}
			else
			{
				s->x++; 		//[***090114		追加
				b->wait1=10;
				b->state1=6;
			}
			break;

		case 4: 	//右移動中->8へ
			if (s->x<(GAME_WIDTH-s->w))
			{
				s->x+=(4+difficulty)*fps_factor;
				if (s->aktframe<8)
				{
					b->wait2+=fps_factor;
					if (b->wait2>=3)
					{
						s->aktframe++;
						b->wait2=0;
					}
				}
			}
			else
			{
				s->x--; 		//[***090114		追加
				b->wait1=10;
				b->state1=8;
			}
			break;

		case 5: 	//プレイヤー位置付近移動中->10へ
			if (((s->x<0)||(s->y<0))||((s->x+s->w>GAME_WIDTH)||(s->y+s->h>GAME_HEIGHT)))
			{
				b->state1=10;
			}
			else
			{
				s->x+=co_s512((b->angle512/*_radian*/))*(4+difficulty)*fps_factor;
				s->y+=si_n512((b->angle512/*_radian*/))*(4+difficulty)*fps_factor;
			}
			break;

		case 6: 	//左:待機->7へ
			enemy_boss02_waitstate(s,7);
			break;

		case 7: 	//左:右回転攻撃->0,11
			enemy_boss02_sr_add1(s);
			if (0==(rand()&(2-1)))
			{
				b->state1=0;
				b->n_wayt++;
			}
			else
			{	b->state1=11;}
			break;

		case 8: 	//右:待機->9へ
			enemy_boss02_waitstate(s,9);
			break;

		case 9: 	//右:左回転攻撃->ひとまず0へ
			enemy_boss02_sr_add2(s);
			b->state1=0;
			b->n_wayt++;
			break;

		case 10:	//プレイヤー位置付近:大玉3つ->ひとまず0へ
			enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 3, atan_512(player->y-s->y,player->x-s->x)-deg_360_to_512(30), 20);
			enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 3, atan_512(player->y-s->y,player->x-s->x) 		   			 , 20);
			enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 3, atan_512(player->y-s->y,player->x-s->x)+deg_360_to_512(30), 20);
			b->state1=0;
			b->n_wayt++;
			break;

		case 11:	//左:前へ->12
			if (s->y>GAME_HEIGHT/3)
			{	b->state1=12;}
			else
			{	s->y+=(5+difficulty)*fps_factor;}
			break;

		case 12:
			enemy_g_bullet_create(s, 5, 20, atan_512(player->y-s->y,player->x-s->x)+deg_360_to_512(60));		//いらない弾達。消した方がいいかも
			enemy_g_bullet_create(s, 5, 10, atan_512(player->y-s->y,player->x-s->x)+deg_360_to_512(30));
			enemy_bullet_create(s, 5);
			enemy_g_bullet_create(s, 5, -10, atan_512(player->y-s->y,player->x-s->x)-deg_360_to_512(30));
			enemy_g_bullet_create(s, 5, -20, atan_512(player->y-s->y,player->x-s->x)-deg_360_to_512(60));
			b->state1=0;
			b->n_wayt++;
			break;

		case 13:
			((PLAYER_DATA *)player->data)->bossmode=B01_BATTLE;
			b->state1=0;
			break;
		}
	}
}

void enemy_boss02_add(int lv)
{
	BOSS02_DATA *b;
	SPRITE *s;
	s				= sprite_add_file("boss02_v2.png",9,PR_ENEMY);	s->anim_speed=0;
	s->flags		|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
	s->aktframe 	= 4;
	s->type 		= SP_EN_BOSS/*SP_EN_BOSS02*/;
	b				= mmalloc(sizeof(BOSS02_DATA));
	s->data 		= b;
	b->b.health 	= (difficulty<<10/**1024*/)+3071+1024;
	b->b.score		= score(2000)+score(1500)*difficulty;
	b->state1		= 0;
	b->state2		= 0;
	b->wait1		= 50;
	b->wait2		= 0;
	b->type 		= 0;
	b->level		= lv;
	b->n_wayt		= 1;
	s->mover		= enemy_boss02_move;
	s->x			= (GAME_WIDTH/2)-(s->w/2);
	b->xp			= s->x;
	s->y			= -(s->h);

	((PLAYER_DATA *)player->data)->bossmode=B01_BATTLE;
	((PLAYER_DATA *)player->data)->boss=s;
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss02_hitbyweapon;
	((PLAYER_DATA *)player->data)->callback_boss_hitbyplayer=callback_enemy_boss02_hitbyplayer;
}
