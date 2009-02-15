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
	/*static*/ double ang_b2;
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
	double angle;
	double wait;
} BOSS02_SR_DATA;

typedef struct
{
	int no;
	int stat;
	double angle;
	double time;
	double speed;
} BOSS02_ICE_DATA;


void enemy_boss02_hitbyplayer(SPRITE *c)
{
}

void enemy_boss02_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/)
{
	BOSS02_DATA *b=(BOSS02_DATA *)c->data;
	SPRITE *d=c;
	BOSS02_DATA *b2;
	int i,j;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	explosion_add(s->x,s->y,0,EXPLOSION_TYPE04);
//	parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);
	b->b.health-=w->strength;

	if ((b->b.health<=1000)&&(b->type==0)) {	//HPが残り1000になった時。グラフィックの差し替え。攻撃パターン変更
		bonus_multi_add(c->x, c->y,SP_BONUS_FIREPOWER,3,0);
		bonus_multi_add(c->x, c->y,SP_BONUS_FIREPOWER_G,1,0);
		for (i=0;i<192;i+=40) {
			for (j=0;j<192;j+=40) {
				explosion_add(d->x+i,d->y+j,rand()%40,EXPLOSION_TYPE00);
			}
		}
		((PLAYER_DATA *)player->data)->bossmode=4;
		d=sprite_add_file("boss02_v2x.png",9,PR_ENEMY);		d->anim_speed=0/*c->anim_speed*/;
		d->flags=c->flags;
		d->aktframe=c->aktframe;
		d->type=c->type;
		b2=mmalloc(sizeof(BOSS02_DATA));
		memcpy(b2,b,sizeof(BOSS02_DATA));
		b=b2;
		b->type=1;
		d->data=b2;
		d->mover=c->mover;
		d->x=c->x;
		d->y=c->y;
		b2->state2=1;
		b2->state1=13;
		b2->wait1=100;

		c->type=-1;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
	}

	if (b->b.health<=0) {
		/*
		for (i=0;i<192;i+=40) {
			for (j=0;j<192;j+=40) {
				explosion_add(d->x+i,d->y+j,rand()%40,EXPLOSION_TYPE00);
			}
		}
		*/
		bonus_multi_add(s->x, s->y,SP_BONUS_COIN,7,1);
		d->type=-1;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		((PLAYER_DATA *)player->data)->bossmode=2;
	}
}




static void enemy_boss02_sr_controller1(CONTROLLER *c)
{
	BOSS02_SR_DATA *e=(BOSS02_SR_DATA *)c->e;
	SPRITE *l;
	LASER_DATA *d;
	SPRITE *boss;

	if (e->wait>0) {
		e->wait-=fps_factor;
	} else {
		boss=sprite_get_by_id(e->boss_id);	//なければNULL
		e->wait=10;
		e->angle+=15;
		if ((e->angle>360-15)||(boss==NULL)) {
			controller_remove(c);
		} else {
			l=sprite_add_file("bshoot.png",1,PR_ENEMY_WEAPON);
			l->type=SP_EN_LASER;
			l->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
			l->mover=enemy_laser_move;
			l->aktframe=0;
			l->x=boss->x+(boss->w/2);
			l->y=boss->y+(boss->h/2);
			d=mmalloc(sizeof(LASER_DATA));
			l->data=d;
			d->angle=degtorad(e->angle);
			d->speed=7;
		}
	}
}

static void enemy_boss02_sr_add1(SPRITE *c)
{
	CONTROLLER *s;
	BOSS02_SR_DATA *b;

	s=controller_add();
	b=mmalloc(sizeof(BOSS02_SR_DATA));
	b->angle=0;
	b->wait=10;
	b->boss_id=c->id;
	s->con=enemy_boss02_sr_controller1;
	s->e=b;
}

static void enemy_boss02_sr_controller2(CONTROLLER *c)
{
	BOSS02_SR_DATA *e=(BOSS02_SR_DATA *)c->e;
	SPRITE *l;
	LASER_DATA *d;
	SPRITE *boss;

	if (e->wait>0) {
		e->wait-=fps_factor;
	} else {
		boss=sprite_get_by_id(e->boss_id);	//なければNULL
		e->wait=10;
		e->angle-=15;
		if ((e->angle<=15)||(boss==NULL)) {
			controller_remove(c);
		} else {
			l=sprite_add_file("bshoot.png",1,PR_ENEMY_WEAPON);
			l->type=SP_EN_LASER;
			l->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
			l->mover=enemy_laser_move;
			l->aktframe=0;
			l->x=boss->x+(boss->w/2);
			l->y=boss->y+(boss->h/2);
			d=mmalloc(sizeof(LASER_DATA));
			l->data=d;
			d->angle=degtorad(e->angle);
			d->speed=7;
		}
	}
}

static void enemy_boss02_sr_add2(SPRITE *c)
{
	CONTROLLER *s;
	BOSS02_SR_DATA *b;

	s=controller_add();
	b=mmalloc(sizeof(BOSS02_SR_DATA));
	b->angle=360;
	b->wait=8;
	b->boss_id=c->id;
	s->con=enemy_boss02_sr_controller2;
	s->e=b;
}

static void enemy_boss02_ice_move(SPRITE *s)
{
	BOSS02_ICE_DATA *l=(BOSS02_ICE_DATA *)s->data;

	switch (l->stat) {
	case 0:
		l->angle=90;
		l->time=100;
		l->speed=4;
		l->stat=1;
		break;
	case 1:
		if (l->no==0)
			l->angle+=5*fps_factor;
		else
			l->angle-=5*fps_factor;
		l->time-=fps_factor;
		if (l->time<=0)
			l->stat=2;
		break;
	case 2:
		l->angle=rand()%90+45;
		l->speed=12;
		l->stat=3;
		break;
	}

	if (l->angle<0)
		l->angle+=360;
	if (l->angle>360)
		l->angle-=360;

	s->x+=cos(degtorad(l->angle))*l->speed*fps_factor;
	s->y+=sin(degtorad(l->angle))*l->speed*fps_factor;
	s->aktframe=(((int)l->angle+270)/10)%36;

	if ( (s->x<-100)||(s->x>WIDTH2+100)||
		(s->y<-100)||(s->y>/*screen->h*/WIDTH+100) )
	{	s->type=-1;}
}


#define NUM_OF_ENEMIES (30)
static void enemy_boss02_ice_controller(CONTROLLER *e)
{
	int *id_array=e->e;
	SPRITE *s, *l, *master;
	BOSS02_ICE_DATA *d;
	int i;
	int dead=0;

	for (i=0;i<NUM_OF_ENEMIES;i++) {
		s=sprite_get_by_id(id_array[i]);
		if (s==NULL) {
			dead=1;
			break;
		}
	}
	if (dead==1) {
		for (i=0;i<NUM_OF_ENEMIES;i++) {
			s=sprite_get_by_id(id_array[i]);
			if (s!=NULL)
				s->type=-1;
		}
		controller_remove(e);
		return;
	}

	master=sprite_get_by_id(id_array[0]);

	d=(BOSS02_ICE_DATA *)master->data;
	for (i=NUM_OF_ENEMIES-1;i>=1;i--) {
		s=sprite_get_by_id(id_array[i]);
		l=sprite_get_by_id(id_array[i-1]);
		if ((s!=NULL)&&(l!=NULL)) {
			s->x=l->x;
			s->y=l->y;
			s->aktframe=l->aktframe;
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

	for (j=0;j<2;j++) {
		e=controller_add();
	//	NUM_OF_ENEMIES=30;
		id_array=mmalloc(sizeof(int)*NUM_OF_ENEMIES);
		e->e=id_array;
		e->con=enemy_boss02_ice_controller;
		for (i=0;i<NUM_OF_ENEMIES;i++) {
			d=sprite_add_file("boss02_w.png",36,PR_ENEMY_WEAPON);
			id_array[i]=d->id;
			d->type=SP_EN_BOSS02ICE;
			d->flags|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
			if (i==0) {
				d->mover=enemy_boss02_ice_move;
				l=mmalloc(sizeof(BOSS02_ICE_DATA));
				d->data=l;
				l->no=j;
				l->stat=0;
			} else {
				d->mover=NULL;
				d->data=NULL;
			}
			if (j==0) {
				d->x=c->x-19;
				d->y=c->y+135;
			} else {
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

	if (b->wait1>0) {
		b->wait1-=fps_factor;
		if (s->aktframe<4) {
			b->wait2+=fps_factor;
			if (b->wait2>=3) {
				s->aktframe++;
				b->wait2=0;
			}
		} else if (s->aktframe>4) {
			b->wait2+=fps_factor;
			if (b->wait2>=3) {
				s->aktframe--;
				b->wait2=0;
			}
		}
	}
	else
	{
		b->state1=nextstate;
		enemy_bigbullet_create(s, s->x+s->w/2, s->y+s->h/2, 4+difficulty/2, -2, 10);
	}
}
static void enemy_boss02_move(SPRITE *s)
{
	BOSS02_DATA *b=(BOSS02_DATA *)s->data;

	switch (b->state2) {
	case 0: 		//攻撃パターン1

		switch (b->state1) {

		case 0: /* nach unten */
			if (s->y<30)
				s->y+=2*fps_factor;
			else
				b->state1=1;
			break;

		case 1: /* y-pos erreicht: rundumschuss */
			enemy_boss02_sr_add1(s);
			if (b->type==1)
				enemy_boss02_ice_add(s);
			b->wait1=20;
			b->state1=2;
			break;

		case 2: /* warten */
			enemy_boss02_waitstate(s,3);
			break;

		case 3: /* nach links bis zum rand */
			if (s->x>=0) {
				s->x-=2*fps_factor;
				if (s->aktframe>0) {
					b->wait2+=fps_factor;
					if (b->wait2>=3) {
						s->aktframe--;
						b->wait2=0;
					}
				}
			} else {
				b->wait1=20;
				b->state1=4;
			}
			break;

		case 4: /* warten */
			enemy_boss02_waitstate(s,5);
			break;

		case 5: /* nach rechts bis zur mitte */
			if (s->x<b->xp) {
				s->x+=2*fps_factor;
				if (s->aktframe<8) {
					b->wait2+=fps_factor;
					if (b->wait2>=3) {
						s->aktframe++;
						b->wait2=0;
					}
				}
			} else {
				enemy_boss02_sr_add1(s);
				b->wait1=100;
				b->state1=6;
			}
			break;

		case 6: /* warten */
			enemy_boss02_waitstate(s,7);
			break;

		case 7:
			enemy_boss02_ice_add(s);
			b->wait1=100;
			b->state1=8;
			break;

		case 8: /* warten */
			enemy_boss02_waitstate(s,9);
			break;

		case 9: /* nach rechts bis zum rand */
			if (s->x<(WIDTH2-s->w)) {
				s->x+=2*fps_factor;
				if (s->aktframe<8) {
					b->wait2+=fps_factor;
					if (b->wait2>=3) {
						s->aktframe++;
						b->wait2=0;
					}
				}
			} else {
				b->wait1=20;
				b->state1=10;
			}
			break;

		case 10: /* warten */
			enemy_boss02_waitstate(s,11);
			break;

		case 11: /* nach links bis zur mitte */
			if (s->x>b->xp) {
				s->x-=2*fps_factor;
				if (s->aktframe>0) {
					b->wait2+=fps_factor;
					if (b->wait2>=3) {
						s->aktframe--;
						b->wait2=0;
					}
				}
			} else
				b->state1=1;
			break;
		}
	break;

	case 1: 	//攻撃パターン2
		switch (b->state1) {

		case 0: 	//不定:初期位置情報の取得->1へ
			b->ang_b2=atan2(-s->y+20,-s->x+WIDTH2/2-s->w);
			b->state1=1;
			break;

		case 1: 	//不定:初期位置へ戻る->2へ
			if (s->y<30)
			{
				b->state1=2;
				b->wait1+=30+difficulty*10;
				if (b->n_wayt%3==0){
					enemy_n_way_bullet(s, "kugel.png", 0, 8, 5, atan2(player->y-s->y,player->x-s->x));
				}
				if (b->n_wayt%4==0 && b->n_wayt%12!=0){
					enemy_n_way_bullet(s, "kugel.png", 0, 11, 4, atan2(player->y-s->y,player->x-s->x));
					enemy_n_way_bullet(s, "kugel.png", 0, 11, 3, atan2(player->y-s->y,player->x-s->x));
				}
			}
			else
			{
				s->x+=cos(b->ang_b2)*6*fps_factor;
				s->y+=sin(b->ang_b2)*6*fps_factor;
				b->state1=0;
			}
			break;

		case 2: 	//初期位置:大玉3つ->3,4,5
			if (b->wait1<0)
			{
				enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 5, -M_PI/6+atan2(player->y-s->y,player->x-s->x), 1);	//大弾の追加
				enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 5, atan2(player->y-s->y,player->x-s->x), 1);
				enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 5, +M_PI/6+atan2(player->y-s->y,player->x-s->x), 1);
				b->state1=rand()%3+3;
				if (b->state1==5)
					b->ang_b2=atan2(-s->y+player->y,-s->x+player->x);
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
			if (s->x>=0) {
				s->x-=(4+difficulty)*fps_factor;
				if (s->aktframe>0) {
					b->wait2+=fps_factor;
					if (b->wait2>=3) {
						s->aktframe--;
						b->wait2=0;
					}
				}
			} else {
				s->x++; 		//[***090114		追加
				b->wait1=10;
				b->state1=6;
			}
			break;

		case 4: 	//右移動中->8へ
			if (s->x<(WIDTH2-s->w)) {
				s->x+=(4+difficulty)*fps_factor;
				if (s->aktframe<8) {
					b->wait2+=fps_factor;
					if (b->wait2>=3) {
						s->aktframe++;
						b->wait2=0;
					}
				}
			} else {
				s->x--; 		//[***090114		追加
				b->wait1=10;
				b->state1=8;
			}
			break;

		case 5: 	//プレイヤー位置付近移動中->10へ
			if (((s->x<0)||(s->y<0))||((s->x+s->w>WIDTH2)||(s->y+s->h>HEIGHT)))
			{
				b->state1=10;
			}
			else
			{
				s->x+=cos(b->ang_b2)*(4+difficulty)*fps_factor;
				s->y+=sin(b->ang_b2)*(4+difficulty)*fps_factor;
			}
			break;

		case 6: 	//左:待機->7へ
			enemy_boss02_waitstate(s,7);
			break;

		case 7: 	//左:右回転攻撃->0,11
			enemy_boss02_sr_add1(s);
			if (rand()%2==0){
				b->state1=0;
				b->n_wayt++;
			}
			else
				b->state1=11;
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
			enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 3, -M_PI/6+atan2(player->y-s->y,player->x-s->x), 20);
			enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 3, atan2(player->y-s->y,player->x-s->x), 20);
			enemy_bigbullet_create(s, s->x+s->w/3, s->y+s->h/2, 3, +M_PI/6+atan2(player->y-s->y,player->x-s->x), 20);
			b->state1=0;
			b->n_wayt++;
			break;

		case 11:	//左:前へ->12
			if (s->y>HEIGHT/3)
				b->state1=12;
			else
				s->y+=(5+difficulty)*fps_factor;
			break;

		case 12:
			enemy_g_bullet_create(s, 5, 20, atan2(player->y-s->y,player->x-s->x)+M_PI/3);		//いらない弾達。消した方がいいかも
			enemy_g_bullet_create(s, 5, 10, atan2(player->y-s->y,player->x-s->x)+M_PI/6);
			enemy_bullet_create(s, 5);
			enemy_g_bullet_create(s, 5, -10, atan2(player->y-s->y,player->x-s->x)-M_PI/6);
			enemy_g_bullet_create(s, 5, -20, atan2(player->y-s->y,player->x-s->x)-M_PI/3);
			b->state1=0;
			b->n_wayt++;
			break;

		case 13:
			((PLAYER_DATA *)player->data)->bossmode=1;
			b->state1=0;
			break;
		}
	}
}

void enemy_boss02_add(int lv)
{
	BOSS02_DATA *b;
	SPRITE *s;

	s=sprite_add_file("boss02_v2.png",9,PR_ENEMY);	s->anim_speed=0;
	s->flags|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
	s->aktframe=4;
	s->type=SP_EN_BOSS02;
	b=mmalloc(sizeof(BOSS02_DATA));
	s->data=b;
	b->b.health=2500;
	b->b.score=score(2000)+score(1500)*difficulty;
	b->state1=0;
	b->state2=0;
	b->wait1=50;
	b->wait2=0;
	b->type=0;
	b->level=lv;
	b->n_wayt=1;
	s->mover=enemy_boss02_move;
	s->x=(WIDTH2/2)-(s->w/2);
	b->xp=s->x;
	s->y=-(s->h);

	((PLAYER_DATA *)player->data)->bossmode=1;
}
