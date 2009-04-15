#include "enemy.h"
/*
	グレイズ関係の追加で大幅に変更されている。
	弾のデータにidを追加。idは1つの弾でグレイズを何回もしないようにするため。0-999までのランダムの値をとる。
	弾のスプライトアニメーションは基本一枚に変更した。後で変化する弾を作るかも。
	ボスを倒した時に消えるようにしてある。
	追加した弾
		enemy_bigbullet_create
		いわゆる大弾。1つの弾でスプライトを2つ使っている。
		当たり判定は1つ目にしかない。

		enemy_g_bullet_create
		小弾に変な角度指定機能を付けたもの。
		今気付いたけどこれマイナスの値が取れないね。意味わからん。
		使ってるのは天狗だけなので消したかったら消していいです。

	//[***090114
		大玉の動きを変えてみた
*/

extern SDL_Surface *screen;
extern SPRITE *player;
extern double fps_factor;
extern int difficulty;		//[***090114		追加
int b_id;


typedef struct
{
	ENEMY_BASE b;
	/*double*/int angle512;
	double speed;
	double range;
	double delay;
} HOMING_DATA;


///////////////

//typedef struct
//{
//	/*double*/int angle512; /*union BULLET_DATA*/
//	double speed;	/*union BULLET_DATA*/
//	int id;
//} LASER_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	int state;
} G_BULLET_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	double gra;
	double sum;
} GR_BULLET_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	double gra;
	double sum;
	int bounds;
} PO_BULLET_DATA;



typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//
	double timer;
} MOMIJI_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//
	/*double*/int angle222_512;
	double a;
	int state;
} SMALL_RE_DATA;


typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	int ransu;
	int wait_bg;
} BIGBULLET_DATA;

typedef struct
{
	double *sx;
	double *sy;
	int *type;
	int sw;
	int sh;
} BIGBULLET_S_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	double a;
} NEW_BIGBULLET_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	int timer;
	double a;
} ST_BULLET_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	double next_angle;
	int timer;
	int state;
	double a;
} ST2_BULLET_DATA;


typedef struct
{
	/*double*/int angle512_4096; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	/*double*/int angle4096;	/* 1/4096以上の分解能の精密弾(1/512では足りない) = angle512_4096 */
	/*double*/int d_angle4096;	/* 1/4096以上の分解能の精密弾(1/512では足りない) */
	int timer;
} AN_BULLET_4096_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA(?)*/
	double speed;			/*union BULLET_DATA*/
//	int id;
	double sum;
	double gra;
} FALL_KNIFE_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	int speed2;
	int height;
	int target;
	double timer;
} FOLLOW_KNIFE_DATA;

typedef struct
{
	/*double*/int angle512; /*union BULLET_DATA*/
	double speed;	/*union BULLET_DATA*/
//	int id;
	/*double*/int d_angle512;
	double length;
	double x;
	double y;
	int state;
	int wait1;	//fire
	int wait2;	//act
} EVEN_KNIFE_DATA;

///


typedef struct
{
	double wait;
	double framewait;
	double framestat;
} EXPLOSION_DATA;



#if 0
/*
いろいろと検証用なのでコメントアウト
void bullet_way(SPRITE *s ,int way, dou ble angle512, dou ble angle2, double spd)
{
	//スプライト情報, n way, 角度ベース, 分割角度幅, 速度
	int i;
	for (i=0;i<way;i++)
	{
		SPRITE *h;
		h				= sprite_add_file0("kugel.png",1,PR_BULLETS/ * PR_ENEMY * /,0);
		h->type 		= SP_EN_BULLET;
		h->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		h->mover		= enemy_bullet_move;
		h->aktframe 	= 0;
		h->x			= s->x+((s->w-h->w)/2);
		h->y			= s->y+((s->h-h->h)/2);
		BULLET_DATA *data;
		data			= mmalloc(sizeof(BULLET_DATA));
		h->data 		= data;
		data->angle 	= angle-angle2*(way/2)+angle2*i;
		data->speed 	= spd;
	}
}
*/
#endif

static void mono_move(SPRITE *s, BULLET_DATA *d)
{
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;
}

static void enemy_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	BULLET_DATA *d=(BULLET_DATA *)s->data;
/*[r13] cosf sinf */
	mono_move(s,d);

	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}


//static void enemy_laser_move2(SPRITE *s)
/*static*/ void enemy_laser_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	/*LASER_DATA*/BULLET_DATA *d=(/*LASER_DATA*/BULLET_DATA *)s->data;

	//s->aktframe=(radtodeg(d->angle)/10)+9;
	//s->aktframe%=18;
	mono_move(s,/*(BULLET_DATA *)*/d);

	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}





static void enemy_bigbullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_100);		//表示されてない？
	}
	BIGBULLET_DATA *d=(BIGBULLET_DATA *)s->data;
	if (d->wait_bg>0)		//[***090124		追加場所。今までフレーム毎に計算していた所を5フレーム毎に変更
		d->wait_bg--;
	else
	{
		d->wait_bg=5;
		d->angle512 += rad2deg512(((rand()%d->ransu)-d->ransu/2)/10);
	}
	mono_move(s,(BULLET_DATA *)d);
	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}

static void enemy_bigbullet_move2(SPRITE *s)
{
	BIGBULLET_S_DATA *d=(BIGBULLET_S_DATA *)s->data;
	s->x=*(d->sx)+((d->sw-s->w)/2);
	s->y=*(d->sy)+((d->sh-s->h)/2);
	if (*(d->type)==-1)
	{
		s->type=SP_DELETE;
	}
}



static void enemy_homing_move(SPRITE *s)
{
	//SDL_Surface *tmp;
	HOMING_DATA *d=(HOMING_DATA *)s->data;
	d->delay-=fps_factor;
	if (d->delay >0)
	{
		#if 0
		/*ダメmono_move(s,(BULLET_DATA *)d);*/
		#else
		s->x+=co_s512((d->angle512))*d->speed*fps_factor;
		s->y+=si_n512((d->angle512))*d->speed*fps_factor;
		#endif
	}
	else
	{
		/* nur jeden 10ten Frame Winkel aktualisieren */
		d->delay=10;
		enemy_homing_update(s);
	}
	/* max. reichweite erreicht? self-destruct! */
	d->range-=fps_factor;
	if (d->range<=0)
	{
		s->type=SP_DELETE;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);
	}
}

//
static void enemy_smallbullet_re_create(SPRITE *s, double speed, /*double*/int angle512, double a);
static void enemy_momiji_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	MOMIJI_DATA *d=(MOMIJI_DATA *)s->data;
	mono_move(s,(BULLET_DATA *)d);
	d->timer-=fps_factor;
	if (d->timer<0)
	{
		enemy_smallbullet_re_create(s, 3, d->angle512-rad2deg512(M_PI*2/6), 0.04);
		enemy_smallbullet_re_create(s, 3, d->angle512-rad2deg512(M_PI*1/6), 0.04);
		enemy_smallbullet_re_create(s, 3, d->angle512,						0.04);
		enemy_smallbullet_re_create(s, 3, d->angle512+rad2deg512(M_PI*1/6), 0.04);
		enemy_smallbullet_re_create(s, 3, d->angle512+rad2deg512(M_PI*2/6), 0.04);
		s->type=SP_DELETE;
	}
	if ((s->x<-s->w)||(s->x>GAME_WIDTH)||(s->y<-s->h)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}


static void enemy_smallbullet_re_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	SMALL_RE_DATA *d=(SMALL_RE_DATA *)s->data;

	switch (d->state)
	{
	case 0:
		if (d->speed<0.5)
		{
			d->state		= 1;
			d->angle222_512 += 256/*rad2deg512(M_PI)*/;
			#if 1
			/*ねんのため*/
			mask512(d->angle222_512);
			#endif
			d->angle512 	= d->angle222_512;
		}
		else
		{	d->speed-=d->a; }
		break;
	case 1:
		d->speed+=d->a;
		break;
	}
	if (d->angle512-d->angle222_512 < rad2deg512(M_PI*2/12) )	{	d->angle512 += 1/*0.01*/;	}
			#if 1
			/*ねんのため*/
			mask512(d->angle512);
			#endif
	mono_move(s,(BULLET_DATA *)d);

	if (s->ticks>200)
	{
		if ((s->x<-s->w)||(s->x>GAME_WIDTH)||(s->y<-s->h)||(s->y>GAME_HEIGHT))
		{
			s->type=SP_DELETE;
		}
	}
}


static void enemy_new_bigbullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_100);		//表示されてない？
	}
	NEW_BIGBULLET_DATA *d=(NEW_BIGBULLET_DATA *)s->data;
	d->speed+=d->a;
	mono_move(s,(BULLET_DATA *)d);
	if ((s->x<-s->w)||(s->x>GAME_WIDTH)||(s->y<-s->h)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}

static void my_adjust_y(SPRITE *s,PLAYER_DATA *pd)
{
	s->y+=(1-pd->bossmode)*fps_factor;/*pd->bossmode???*/
}

static void enemy_stop_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;

	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	ST_BULLET_DATA *d=(ST_BULLET_DATA *)s->data;
	if (d->speed>0)
	{	d->speed-=d->a;}
	else if (d->speed)
	{	d->speed=0;}
	else
	{	d->timer--;}

	if (d->timer<0)
	{
		if ( 17< s->alpha)//if (s->alpha>0)
		{
			s->alpha-=17;
		}
		else //if (s->alpha<0)
		{
			s->alpha=0;
			s->type=SP_DELETE;
		}
	}

	my_adjust_y(s,pd);
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x<0)||(s->x>GAME_WIDTH)||
		(s->y>GAME_HEIGHT)||(s->y<-100)
	//	||(s->alpha<0)
	)
	{
		s->type=SP_DELETE;
	}
}

static void enemy_g_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	G_BULLET_DATA *d=(G_BULLET_DATA *)s->data;

	s->x+=(d->state%10);
	s->y+=(d->state/10);

	mono_move(s,(BULLET_DATA *)d);
	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}


static void enemy_pong_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	PO_BULLET_DATA *d=(PO_BULLET_DATA *)s->data;
	if (d->sum<15)
	{	d->sum+=d->gra;}
	s->y+=d->sum;
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x<0)||(s->x>GAME_WIDTH))
	{
		d->angle512 		= atan_512(si_n512((d->angle512)),-co_s512((d->angle512)));
	}
	else if (s->y>GAME_HEIGHT)
	{
		if (d->bounds)
		{
			d->speed		= -d->speed;
			d->sum			= -d->sum;
			d->bounds--;
			s->aktframe 	= d->bounds;		//[***090116		若干変更
			d->angle512 	= atan_512(si_n512((d->angle512)),-co_s512((d->angle512)));		//[***090116		追加
		}
		else
		{	s->type=SP_DELETE;}
	}
}

static void enemy_gr_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	GR_BULLET_DATA *d=(GR_BULLET_DATA *)s->data;
	d->sum+=d->gra;
	s->y+=d->sum;
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<-50)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}


static void enemy_stop_bullet2_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;

	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	ST2_BULLET_DATA *d=(ST2_BULLET_DATA *)s->data;

	switch (d->state)
	{
	case 0:
		d->speed-=d->a;
		if (d->speed<0)
		{
			d->speed=0;
			d->state=1;
		}
		break;
	case 1:
		d->timer--;
		if (d->timer<0)
		{
			d->timer=0;
			d->state=2;
			enemy_new_bigbullet_create(s, 0, d->next_angle, d->a*7);
		}
		break;
	case 2:

		if ( 17< s->alpha)//if (s->alpha>0)
		{
			s->alpha-=17;
		}
		else //if (s->alpha<0)
		{
			s->alpha=0;
			s->type=SP_DELETE;
		}
		break;
	}

	my_adjust_y(s,pd);
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x<-50)||(s->x>GAME_WIDTH+60)||
		(s->y>GAME_HEIGHT+60)||(s->y<-50)
	//	||(s->alpha<0)
	)
	{
		s->type=SP_DELETE;
	}
}

static void enemy_angle_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	AN_BULLET_4096_DATA *d=(AN_BULLET_4096_DATA *)s->data;
	if (d->timer<90)
	{
		d->timer++;
		d->angle4096 += d->d_angle4096;
		d->angle512_4096 = ((d->angle4096)>>3);
		mask512(d->angle512_4096);
		//if		(d->angle512> rad2deg512(M_PI)) {	d->angle512 -= rad2deg512(2*M_PI);}
		//else if (d->angle512<-rad2deg512(M_PI)) { d->angle512 += rad2deg512(2*M_PI);}
	}
	s->y+=fps_factor;
	mono_move(s,(BULLET_DATA *)d);

	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<-100)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}
#if 0
static void enemy_knife_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	BULLET_DATA *d=(BULLET_DATA *)s->data;

	mono_move(s,d);
	if ((s->x+s->w<0)||(s->x>GAME_WIDTH)||(s->y+s->h<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}
#endif
static void enemy_fall_knife_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	FALL_KNIFE_DATA *d=(FALL_KNIFE_DATA *)s->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_50);
	}
	double tmp;
	tmp=si_n512((d->angle512))*d->speed*fps_factor;
	if (tmp+d->sum<=3)
	{	d->sum+=d->gra;}
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=tmp+d->sum;
	if ((s->x<0)||(s->x>GAME_WIDTH)||(s->y<-100)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}


static void enemy_follow_knife_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_30);
	}
	FOLLOW_KNIFE_DATA *d=(FOLLOW_KNIFE_DATA *)s->data;
	if (d->target)
	{
		s->x+=co_s512((d->angle512))*2*d->speed2*fps_factor;
		s->y+=si_n512((d->angle512))*2*d->speed2*fps_factor;
	}
	else
	{
		mono_move(s,(BULLET_DATA *)d);
		d->speed-=0.02;
		if (s->aktframe==17)	{	s->aktframe=0;}
		else					{	s->aktframe++;}
		if (d->speed<=0)
		{
			d->angle512=atan_512(player->y+player->h/2+d->height-s->y,player->x-player->w/2-s->x);
			d->target=1;
			s->aktframe=/*17-*/(deg_512_to_360(d->angle512)/20)%18;
		}
	}
	d->timer--;
	if (d->timer<0)
	{
		if (((s->x+s->w<0)||(s->x>GAME_WIDTH)||(s->y+s->h<0)||(s->y>GAME_HEIGHT)))
		{
			s->type=SP_DELETE;
		}
	}
}

#if 1
static void enemy_evenl_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d=(EVEN_KNIFE_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_100);
	}
	switch (d->state)
	{
	case 0:
		if (d->wait1>100)
		{
			d->wait1=0;
			d->state=1;
		}
		d->angle512 += d->d_angle512 /**fps_factor*/;
		mask512(d->angle512);//if (d->angle512 > rad2deg512(M_PI) ) 		{	d->angle512 =- rad2deg512(M_PI);}
		s->aktframe 	= /*17-*/(deg_512_to_360(d->angle512-rad2deg512(M_PI/2))/20)%18;
		/*
			s->x		ナイフのx座標
			d->x		ノードのx座標
			s->w/2		ナイフの横幅の半分
			ナイフのx座標＝ノードのx座標＋co_s512((d->angle512))×length－ナイフの横幅の半分
		*/
		s->x			= d->x-co_s512((d->angle512))*d->length-s->w/2;
		s->y			= d->y-si_n512((d->angle512))*d->length-s->h/2;
		//s->aktframe	= (d->angle/10)%36;
		break;

	case 1:
		if (d->wait1>50)
		{
			d->wait1=0;
			d->state=0;
		}
		mono_move(s,(BULLET_DATA *)d);
		break;
	}
	if (d->wait1>10)
	{
		d->wait1=0;
		enemy_knife_create(s, t256(5.0), d->angle512-rad2deg512(M_PI*2*3/12), s->aktframe);
		enemy_knife_create(s, t256(4.0), d->angle512-rad2deg512(M_PI*2*4/12), s->aktframe);
		enemy_knife_create(s, t256(3.0), d->angle512-rad2deg512(M_PI*2*5/12), s->aktframe);
		if (difficulty>1)
		{	enemy_knife_create(s, t256(2.0), d->angle512-rad2deg512(M_PI*2*6/12), s->aktframe);}
	}
	//s->type=SP_DELETE;
	if (d->wait2>150+difficulty*50)
	{	s->type=SP_DELETE;}

	d->wait1++;
	d->wait2++;
}
#endif
static void enemy_evenr_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d=(EVEN_KNIFE_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->bossmode==B02_DEATH_WAIT||pd->bossmode==B04_CHANGE)
	{
		s->type=SP_DELETE;
		bonus_info_score(s,SCORE_100);
	}
	switch (d->state)
	{
	case 0:
		if (d->wait1>100)
		{
			d->wait1=0;
			d->state=1;
		}
		d->angle512 -= d->d_angle512 /**fps_factor*/;
		mask512(d->angle512);//if (d->angle512 < -rad2deg512(M_PI)) 	{	d->angle512=rad2deg512(M_PI);}
		s->aktframe 	= /*17-*/(deg_512_to_360(d->angle512+rad2deg512(M_PI/2))/20)%18;
		s->x			= d->x-co_s512((d->angle512))*d->length-s->w/2;
		s->y			= d->y-si_n512((d->angle512))*d->length-s->h/2;
		//s->aktframe	= (d->angle/10)%36;
		break;

	case 1:
		if (d->wait1>50)
		{
			d->wait1=0;
			d->state=0;
		}
		//#if 1
		//d->speed=20/*speed*/;/*よくわかんないが止まっちゃう*/
		//#endif
		mono_move(s,(BULLET_DATA *)d);
		break;
	}



	if (d->wait1>10)
	{
		d->wait1=0;
		enemy_knife_create(s, t256(5.0), d->angle512+rad2deg512(M_PI*2*3/12), s->aktframe); //+M_PI/2
		enemy_knife_create(s, t256(4.0), d->angle512+rad2deg512(M_PI*2*4/12), s->aktframe);
		enemy_knife_create(s, t256(3.0), d->angle512+rad2deg512(M_PI*2*5/12), s->aktframe);
		if (difficulty>1)
		{	enemy_knife_create(s, t256(2.0), d->angle512+rad2deg512(M_PI*2*6/12), s->aktframe);}
	}
	//s->type=SP_DELETE;
	if (d->wait2>150+difficulty*50)
	{	s->type=SP_DELETE;}

	d->wait1++;
	d->wait2++;
}


static void explosion_move(SPRITE *c)
{
	EXPLOSION_DATA *d=(EXPLOSION_DATA *)c->data;
	if (d->wait>0)
	{	d->wait-=fps_factor;}
	else
	{
		d->framestat+=fps_factor;
		if (d->framestat>d->framewait)
		{
			d->framestat=0;
			c->flags|=SP_FLAG_VISIBLE;
			if (c->aktframe<c->frames-1)	{	c->aktframe++;		}
			else							{	c->type=SP_DELETE;	}
		}
	}
}

/*--------*/

/* プレイヤー狙い弾(赤) */
void enemy_bullet_create(SPRITE *s, double speed)
{
	SPRITE *h;
	h				= sprite_add_file0("kugel.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_bullet_move;
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	BULLET_DATA *data;
	data			= mmalloc(sizeof(BULLET_DATA));
	h->data 		= data;
//
	data->angle512	= atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	data->speed 	= speed;
}


void bullet_create_haridan180(SPRITE *s, int angle512, int speed256, int offsx, int offsy )
{
	if (ANGLE_JIKINERAI_DAN==angle512)
	{
		angle512	= atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	}
//
	SPRITE *h;
	h					= sprite_add_file(/*"bs hoot.png"*/"bullet_beam16.png",16/*1*/,PR_BULLETS/*PR_ENEMY_WEAPON*/ /*PR_ENEMY*/);
	h->type 			= SP_EN_LASER;
	h->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover			= enemy_laser_move;
	h->aktframe 		= ((angle512>>4)&0x0f);/*180度*/	//b->aktframe	= 0;
	h->x				= s->x+((s->w-h->w)/2)+offsx;
	h->y				= s->y+((s->h-h->h)/2)+offsy;
	{	/*LASER_DATA*/BULLET_DATA *ldata;
		ldata			= mmalloc(sizeof(/*LASER_DATA*/BULLET_DATA));
		h->data 		= ldata;
		mask512(angle512);/*念の為*/
		ldata->angle512 = /*deg512_2rad*/(angle512);
		ldata->speed	= (speed256>>8);/*←とりあえず*/
	}
}

/*----------*/


void enemy_homing_create(SPRITE *s)
{
	SPRITE *r;
	r				= sprite_add_file0("rotating_rocket.png",20,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	r->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	r->type 		= SP_EN_HOMING;
	r->x			= s->x;
	r->y			= s->y;
	r->mover		= enemy_homing_move;
	HOMING_DATA *data;
	data			= mmalloc(sizeof(HOMING_DATA));
	r->data 		= data;
	data->b.score	= score(5*2);
	data->b.health	= 1;
	data->angle512	= (0);
	data->speed 	= 2;
	data->range 	= 200;
	data->delay 	= 10;

	enemy_homing_update(r);
}

void enemy_homing_update(SPRITE *s)
{
	HOMING_DATA *d=(HOMING_DATA *)s->data;
	/* Zielwinkel erfassen */
	d->angle512 = atan_512(player->y-s->y,player->x-s->x);
	s->aktframe = (deg_512_to_360(d->angle512)*20)/360;
	s->aktframe %=20;
}

void enemy_momiji_create(SPRITE *s, double speed, /*double*/int angle512)
{	/*丸弾８(赤ＲＧＢ緑若黄青)*/
	SPRITE *h;
//	h				= sprite_add_file0("sp ell_bullet_r.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	h				= sprite_add_file0("bullet_maru8.png",8,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	h->aktframe 	= 1;/*とりあえず*/
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->type 		= SP_EN_LASER;
	h->x			= s->x+(s->w>>1);
	h->y			= s->y+(s->h>>1);
	h->mover		= enemy_momiji_move;
	MOMIJI_DATA *data;
	data			= mmalloc(sizeof(MOMIJI_DATA));
	h->data 		= data;
	data->angle512	= /*rad2deg512*/(angle512);
	data->speed 	= speed;
	data->timer 	= 30;
}

static void enemy_smallbullet_re_create(SPRITE *s, double speed, /*double*/int angle512, double a)
{
	SPRITE *h;
	h				= sprite_add_file0("kugel2.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->type 		= SP_EN_BULLET;
	h->x			= s->x+(s->w>>1);
	h->y			= s->y+(s->h>>1);
	h->mover		= enemy_smallbullet_re_move;
	SMALL_RE_DATA *data;
	data			= mmalloc(sizeof(SMALL_RE_DATA));
	h->data 		= data;
	data->state 	= 0;
	data->angle222_512	= /*rad2deg512*/(angle512);
	data->angle512	= /*rad2deg512*/(angle512);
	data->speed 	= speed;
	data->a 		= a;
}

void enemy_bigbullet_create(SPRITE *s, double ex, double ey, double speed, /*double*/int angle512, int ransu)
{
	/*
		ransu		0～60くらいまで。大きいほど挙動不審な動きになる
					角度的に+方向へ流れる傾向が強い気がする=>天狗第一形態。修正する必要がありそう。
		angle		ANGLE_JIKINERAI_DAN (999) を指定するとプレイヤーに向かう。基本は角度指定(°)
		ex,ey		出現位置
	*/
	SPRITE *h,*hu;
	BIGBULLET_DATA *data;
	BIGBULLET_S_DATA *src_d;

	h				= sprite_add_file0("bigkugel2.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*表示部分*/
	h->type			= SP_EN_BIGBULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_bigbullet_move;
	h->aktframe		= 0;
	h->x			= ex-h->w/2;
	h->y			= ey-h->h/2;

	hu				= sprite_add_file0("bigkugel1.png",1,PR_TMP,1);/*あたり判定部分*/
	hu->type		= SP_EN_BIGBULLET_DUMMY;
	hu->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	hu->mover		= enemy_bigbullet_move2;
	hu->aktframe	= 0;
	hu->x			= ex-hu->w/2;
	hu->y			= ey-hu->h/2;

	data			= mmalloc(sizeof(BIGBULLET_DATA));
	h->data			= data;

	src_d			= mmalloc(sizeof(BIGBULLET_S_DATA));
	hu->data		= src_d;

//
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y-player->h/2-s->y,player->x-player->w/2-s->x);}
	else
	{	data->angle512=/*rad2deg512*/(angle512);}
	data->speed 	= speed;
	data->ransu 	= ransu;
	data->wait_bg	= 10;

	src_d->sw		= h->w;
	src_d->sh		= h->h;
	src_d->type 	= &h->type;
	src_d->sx		= &h->x;
	src_d->sy		= &h->y;
}

void enemy_new_bigbullet_create(SPRITE *s, double speed, /*double*/int angle512, double a)
{
	SPRITE *h;
	h				= sprite_add_file0(/*"new_bigkugel.png"*/"bigkugel1.png"/*"bigkugel1.png"*/,1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*表示部分*/
	h->type 		= SP_EN_BIGBULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_new_bigbullet_move;
	h->aktframe 	= 0;
	h->x			= s->x-h->w/2;
	h->y			= s->y-h->h/2;
	SPRITE *hu;
	hu				= sprite_add_file0(/*"bigkugel1.png"*/"new_bigkugel.png"/*"new_bigkugel.png"*/,1,PR_TMP,1);/*あたり判定部分*/
	hu->type		= SP_EN_BIGBULLET_DUMMY;
	hu->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	hu->mover		= enemy_bigbullet_move2;
	hu->aktframe	= 0;
	hu->x			= s->x-hu->w/2;
	hu->y			= s->y-hu->h/2;
	NEW_BIGBULLET_DATA *data;
	data			= mmalloc(sizeof(NEW_BIGBULLET_DATA));
	h->data 		= data;
	BIGBULLET_S_DATA *src_d;
	src_d			= mmalloc(sizeof(BIGBULLET_S_DATA));
	hu->data=src_d;
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y-player->h/2-s->y,player->x-player->w/2-s->x);}
	else
	{
	//	/*double*/int tmp_angle512;
	//	tmp_angle512=rad2deg512(angle);
	//	while (1)
	//	{
	//		if (	 tmp_angle512> rad2deg512(M_PI))		{	tmp_angle512-=rad2deg512(2*M_PI);}
	//		else if (tmp_angle512<-rad2deg512(M_PI))		{	tmp_angle512+=rad2deg512(2*M_PI);}
	//		else
	//		{	break;}
	//	}
	//	data->angle512=tmp_angle512;
		data->angle512=/*rad2deg512*/(angle512);
		mask512(data->angle512);
	}
	data->speed 	= speed;
	data->a 		= a;
	src_d->sw		= h->w;
	src_d->sh		= h->h;
	src_d->type 	= &h->type;
	src_d->sx		= &h->x;
	src_d->sy		= &h->y;
}


void enemy_g_bullet_create(SPRITE *s, double speed, int state, /*double*/int angle512)
{
	/*
		state		十進数で一の位がx、十の位がy	指定した分だけ角度が変わる。
		angle512	ANGLE_JIKINERAI_DAN (999) でプレイヤーに向かう。
	*/
	SPRITE *h;
	h				= sprite_add_file0("kugel.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_g_bullet_move;
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	G_BULLET_DATA *data;
	data			= mmalloc(sizeof(G_BULLET_DATA));
	h->data 		= data;
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y-s->y,player->x-s->x);}
	else
	{	data->angle512=/*rad2deg512*/(angle512);}
	data->speed=speed;
	data->state=state;
}


//[***090114	追加
void enemy_pong_bullet_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int delta256, int bou)
{
	/*
		angle512	ANGLE_JIKINERAI_DAN (999) でプレイヤー狙い
		delta256 	フレーム毎に下方向にどれだけ加速するか
	*/
	SPRITE *h;		/*丸弾８(赤ＲＧＢ緑若黄青)*/
//	h				= sprite_add_file0("bs hoot2.png",3,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*緑黄弾*/
	h				= sprite_add_file0("bullet_maru8.png",8,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*緑黄弾*/
	h->type 		= SP_EN_LASER;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_pong_bullet_move;
	h->aktframe 	= 4+bou;/*とりあえず*/
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	PO_BULLET_DATA *data;
	data			= mmalloc(sizeof(PO_BULLET_DATA));
	h->data=data;
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y-s->y,player->x-s->x);}
	else
	{	data->angle512=/*rad2deg512*/(angle512);}
	data->speed			= speed;
	data->gra			= t256_to_double(delta256);
	data->sum			= 0;
	data->bounds		= bou;
}

/*
	enemy_gr_bullet_create()
	下方向に加速していく中弾。
	フレーム毎で計算しているのでかなり加速度を小さくしないと大変なことに。
*/
void enemy_gr_bullet_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int delta256)
{
	/*
		angle512	ANGLE_JIKINERAI_DAN (999) でプレイヤー狙い
		delta256	フレーム毎に下方向にどれだけ加速するか
	*/
	SPRITE *h;		/*丸弾８(赤ＲＧＢ緑若黄青)*/
//	h				= sprite_add_file0("bs hoot.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*青弾*/
	h				= sprite_add_file0("bullet_maru8.png",8,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*青弾*/
	h->type			= SP_EN_LASER;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_gr_bullet_move;
	h->aktframe		= 7/*0*/;/*とりあえず*/
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	GR_BULLET_DATA *data;
	data			= mmalloc(sizeof(GR_BULLET_DATA));
	h->data			= data;
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y-s->y,player->x-s->x);}
	else
	{	data->angle512=/*rad2deg512*/(angle512);}
	data->speed		= speed;
	data->gra		= t256_to_double(delta256);
	data->sum		= 0;
}


void enemy_stop_bullet_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int delta256)
{
	/*
		angle512	ANGLE_JIKINERAI_DAN (999) でプレイヤー狙い
		delta256	フレーム毎にspeedをどれだけ減速するか
	*/
	SPRITE *h;
	h				= sprite_add_file0("kugel.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*小赤弾*/
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_stop_bullet_move;
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	ST_BULLET_DATA *data;
	data			= mmalloc(sizeof(GR_BULLET_DATA));
	h->data 		= data;
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);}
	else
	{	data->angle512=/*rad2deg512*/(angle512);}
	data->speed		= speed;
	data->a			= t256_to_double(delta256);
	data->timer		= 800;
}


void enemy_stop_bullet3_create(SPRITE *s, /*double*/int speed256, /*double*/int angle512, double a, /*double*/int next_angle512)
{
	SPRITE *h;
	h				= sprite_add_file0("kugel.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);/*小赤弾*/
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_stop_bullet2_move;
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	ST2_BULLET_DATA *data;
	data			= mmalloc(sizeof(ST2_BULLET_DATA));
	h->data 		= data;
	if (ANGLE_JIKINERAI_DAN==angle512)
	{	data->angle512=atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);}
	else
	{	data->angle512=/*rad2deg512*/(angle512);}
	data->speed 	= t256_to_double(speed256);
	data->state 	= 0;
	data->a 		= a;
	{
	//	/*double*/int tmp_angle512=next_angle512;
	//		 if (tmp_angle512 > rad2deg512(M_PI) )	{	tmp_angle512 -= rad2deg512(2*M_PI);}
	//	else if (tmp_angle512< rad2deg512(-M_PI) )	{	tmp_angle512 += rad2deg512(2*M_PI);}
	//	data->next_angle = rad2deg512(tmp_angle512);
		mask512(next_angle512);
		data->next_angle = rad2deg512(next_angle512);
	}
	data->timer=100;
}


void enemy_angle_bullet_create(SPRITE *s, /*double*/int speed256, /*double*/int angle4096, /*double*/int d_angle4096)
{
	SPRITE *h;
	h					= sprite_add_file0("kugel.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,0);
	h->type 			= SP_EN_BULLET;
	h->flags			|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover			= enemy_angle_bullet_move;
	h->aktframe 		= 0;
	h->x				= s->x+((s->w-h->w)/2);
	h->y				= s->y+((s->h-h->h)/2);
	AN_BULLET_4096_DATA *data;
	data				= mmalloc(sizeof(AN_BULLET_4096_DATA));
	h->data 			= data;
	mask4096(angle4096);
	data->angle4096 	= (angle4096);
	data->angle512_4096 	= /*rad2deg512*/((angle4096)>>3);
	mask512(data->angle512_4096);
	double speed;
	speed = t256_to_double(speed256);
	data->speed 		= speed;
	data->d_angle4096	= /*rad2deg512*/(d_angle4096);
	data->timer 		= 0;
}

void enemy_fall_knife_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int gra256)
{
	SPRITE *h;
	h				= sprite_add_file0("knife.png",1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,1);/*垂直降下ナイフ(赤)*/
	h->type 		= SP_EN_LASER;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_fall_knife_move;
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	FALL_KNIFE_DATA *data;
	data			= mmalloc(sizeof(FALL_KNIFE_DATA));
	h->data 		= data;
	mask512(angle512);
	data->angle512	= /*rad2deg512*/(angle512);
	data->speed 	= speed;
	data->gra		= t256_to_double(gra256);
	data->sum		= 0;
}


void enemy_knife_create(SPRITE *s, /*double*/int speed256, /*double*/int angle512, int anim)
{
	SPRITE *h;
	#if 0
	char filename[32/*30*/];
	sp rintf(filename,"knife%d.png",anim);/* 速度の要る場所では sp rintf() は使わない方が良い */ 		/* 文字列処理(sp rintf)は非常に遅い */
	h				= sprite_add_file0(filename,1,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,1);/*全方向ナイフ(青)*/
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_bullet_move;
	h->aktframe 	= /*anim%18*/0;
	#else
	h				= sprite_add_file0("knife_core.png",18,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,1);/*全方向ナイフ(青)*/
	h->type 		= SP_EN_BULLET;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_bullet_move;
	h->aktframe 	= anim%18/*0*/;
	#endif
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);

	BULLET_DATA *data;
	data			= mmalloc(sizeof(BULLET_DATA));
	h->data 		= data;
//
	#if 1
	/*安全策*/
	mask512(angle512);
	#endif
	data->angle512	= /*rad2deg512*/(angle512);
	data->speed 	= t256_to_double(speed256);
}


void enemy_follow_knife_create1(SPRITE *s, /*double*/int speed256, /*double*/int angle512, int height)
{
	SPRITE *h;
	h				= sprite_add_file0("knife_core.png",18,/*PR_BULLETS*/PR_ENEMY/*PR_ENEMY*/,1);/*全方向ナイフ(青)*/
	h->type 		= SP_EN_LASER;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_follow_knife_move;
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	FOLLOW_KNIFE_DATA *data;
	data			= mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data 		= data;
	data->angle512	= /*rad2deg512*/(angle512);
	data->speed 	= t256_to_double(speed256);
	data->speed2	= t256_to_double(speed256);
	data->target	= 0;
	data->height	= height;
	data->timer 	= 300;
}
#if 0
void enemy_follow_knife_create2(SPRITE *s, /*double*/int speed256, /*double*/int angle512, int height)
{/*double x, double y*/
	SPRITE *h;
	h				= sprite_add_file0("knife_core.png",18,PR_ENEMY,1);/*全方向ナイフ(青)*/
	h->type 		= SP_EN_LASER;
	h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover		= enemy_follow_knife_move;
	h->aktframe 	= 0;
	h->x			= s->x;
	h->y			= s->y/*x*/;
	FOLLOW_KNIFE_DATA *data;
	data			= mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data 		= data;
	data->angle512	= /*rad2deg512*/(angle512);
	data->speed 	= t256_to_double(speed256);
	data->speed2	= t256_to_double(speed256);
	data->target	= 0;
	data->height	= height;
	data->timer 	= 300;
}
#endif

#if 1
	/*
		length	enemy-player間の距離/√2
		r_or_l	0==右	1==左
	*/
void enemy_even_knife_create(SPRITE *s, double speed, /*double*/int length, int r_or_l)
{
	SPRITE *h;
	h				= sprite_add_file0("knife_core.png",18,PR_ENEMY,0);/*全方向ナイフ(青)*/
	h->type 		= SP_EN_BULLET;
	h->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->aktframe 	= 0;
	h->x			= s->x+((s->w-h->w)/2);
	h->y			= s->y+((s->h-h->h)/2);
	EVEN_KNIFE_DATA *data;
	data=mmalloc(sizeof(EVEN_KNIFE_DATA));
	h->data=data;

//
	/*double*/int tmp512;
//	tmp512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w/2-s->w/2)));/*???*/
	tmp512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w+s->w)/2));/*???(original)*/ /* 奇数弾の場合に自機狙い */
//	tmp512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x+((player->w-s->w)/2));/*???*/				/* 偶数弾の場合に自機狙い */
	#if 1
	if (1==r_or_l)
	{
		h->mover		= enemy_evenl_knife_move;
		//if (tmp512>rad2deg512(M_PI*2*3/8))	{	tmp512 -= rad2deg512(M_PI*2*7/8);}
		//else									{	tmp512 += rad2deg512(M_PI*2*1/8);}
		tmp512 += (64)/*M_PI*2*1/8*/;
	}
	else
//	if (0==r_or_l)
	#endif
	{
		h->mover		= enemy_evenr_knife_move;
		//if (tmp512 < -rad2deg512(M_PI*2*3/8)) {	tmp512 =  rad2deg512(M_PI*2*7/8)-tmp512;}
		//else									{	tmp512 -= rad2deg512(M_PI*2*1/8);}
		tmp512 -= (64)/*M_PI*2*1/8*/;
	}
	mask512(tmp512);
	if (0==length)	{length=1;}
	data->length		= length;
	data->x 			= /*s->x+s->w/2*/h->x+co_s512((tmp512))*length;
	data->y 			= /*s->y+s->h/2*/h->y+si_n512((tmp512))*length;
	data->angle512		= /*rad2deg512*/(/*deg512_2rad*/(tmp512));
//	data->d_angle512	= rad2deg512(speed/(M_PI*2*length));	//⊿angle≒任意/(2π×length/√2)
//	/*double*/int tmp_512;
//	tmp_512=speed/(/*M_PI*2*/512*length);	//⊿angle≒任意/(2π×length/√2)
	data->d_angle512= 1;//	  /*rad2deg512*/(/*deg512_2rad*/(tmp_512));
//
	data->speed 		= /*10*/10/*8+difficulty*/ /*speed*/;/*よくわかんないが止まっちゃう*/
	data->state 		= 0;
	data->wait1 		= 0;
	data->wait2 		= 0;
}
#endif

void enemy_n_way_bullet_type(SPRITE *s, int bu_type,/*char *filename, int frame,*/ int n, double speed, /*double*/int angle512)
{
	//angle_radianはラジアン
	//frameはフレーム数-1
	SPRITE *h;
	BULLET_DATA *data;
	int i;
	if (ANGLE_JIKINERAI_KISUDAN==angle512)
	{
		angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w+s->w)/2));/*???(original)*/	/* 奇数弾の場合に自機狙い */
	//	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x+((player->w-s->w)/2));/*???*/ 		/* 偶数弾の場合に自機狙い */
	}
	/*double*/int i_angle512;
	i_angle512	= angle512-((n+2)*rad2deg512((M_PI*2)/48)); //π/24 /*0.13089969389957471827*/
	/* 念の為マスク */
	mask512(i_angle512);

	for (i=0;i<n;i++)
	{
		h			= sprite_add_file0("bullet_ming32.png"/*"kugel.png"filename*/, 32/*(0)frame+1*/, PR_ENEMY,0);
		h->type 	= SP_EN_BULLET;
		h->flags	|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		h->mover	= enemy_bullet_move;
		/*h->anim_speed = 0;*/ /*if (frame) {	h->anim_speed=1;}*/
		h->aktframe = ((/*rad2deg512(angle_radian)*/i_angle512)>>4)/*0*/;
		h->x=s->x+((s->w-h->w)/2);
		h->y=s->y+((s->h-h->h)/2);

		data		= mmalloc(sizeof(BULLET_DATA));
		h->data 	= data;

		i_angle512 += rad2deg512(((M_PI*2)/48)*2)/*0.26179938779914943654*/;
	//	if (i_angle_radian>M_PI)		{	i_angle_radian-=2*M_PI;}
	//	else if (i_angle_radian<-M_PI)	{	i_angle_radian+=2*M_PI;}
		mask512(i_angle512);
		data->angle512 = /*deg512_2rad*/(i_angle512);	//π/12
		data->speed=speed;
	}
}


void explosion_add(int x, int y, /*double*/int wait, int type)
{
	SPRITE *ex=NULL;
	EXPLOSION_DATA *d;
	d=mmalloc(sizeof(EXPLOSION_DATA));
//	if (EXPLOSION_MINI00==type) {	type = (rand()%3+1);	}/* あちこちにあると遅い */
	if (0==(type&0xfb))
	{
		type |= (rand()&0x03);	if (0==(type & 0x03)) { 	type |= (0x01); }
	}
	switch (type)
	{
//	case 0: break;/*小爆発*/
	case 1: 	ex=sprite_add_file0("tr_blue.png", 6,PR_BONUS,0);	d->framewait=1; 	break;
	case 2: 	ex=sprite_add_file0("tr_red.png",  6,PR_BONUS,0);	d->framewait=1; 	break;
	case 3: 	ex=sprite_add_file0("tr_green.png",6,PR_BONUS,0);	d->framewait=1; 	break;
//
//	case 4: break;/*ザコ消滅爆発*/
	case 5: 	ex=sprite_add_file0("bakuha05.png",5,PR_BONUS,1);	d->framewait=8/*1*/;	ex->alpha 	= 150;	break;
	case 6: 	ex=sprite_add_file0("bakuha06.png",5,PR_BONUS,1);	d->framewait=8/*1*/;	ex->alpha 	= 150;	break;
	case 7: 	ex=sprite_add_file0("bakuha07.png",5,PR_BONUS,1);	d->framewait=8/*1*/;	ex->alpha 	= 150;	break;
//		/*火炎爆発*/
	case 8: 	ex=sprite_add_file0("ex.png",29,PR_BONUS,0);		d->framewait=3/*2*/;	break;
	default:	error(ERR_WARN,"explosion_add: unknown type: %d",type);
	}

	ex->flags		&= ~SP_FLAG_VISIBLE;
	ex->anim_speed	= 0;
	ex->aktframe	= -1;
	ex->type		= SP_ETC;
	ex->x			= x-(ex->w/2);
	ex->y			= y-(ex->h/2);
	ex->mover		= explosion_move;
	ex->data		= d;
	d->wait 		= wait;
	d->framestat	= 0;
}
#if 0
void explosion_add_rect(SPRITE *src)
{
	int i,j;
	for (i=0;i<100;i+=30)
	{
		for (j=0;j<100;j+=30)
		{
			explosion_add(src->x+i,src->y+j,(rand()&(32-1)/*%30*/),EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
		}
	}
		/*
		for (i=0;i<192;i+=40)
		{
			for (j=0;j<192;j+=40)
			{
				explosion_add(src->x+i,src->y+j,rand()%40,EXPLOSION_FIRE08);
			}
		}
		*/
	//int i,j;
	//	for (i=0;i<100;i+=30)
	//	{
	//		for (j=0;j<100;j+=30)
	//		{
	//			explosion_add(src->x+i,src->y+j,(rand()&(32-1)/*%30*/),EXPLOSION_FIRE08);
	//		}
	//	}
	//	int i,j;
	//	for (i=0;i<192;i+=40)
	//	{
	//		for (j=0;j<192;j+=40)
	//		{
	//			explosion_add(/*d*/c->x+i,/*d*/c->y+j,rand()%40,EXPLOSION_FIRE08);
	//		}
	//	}
}
#endif
#if 1
void explosion_add_circle(SPRITE *src)
{
	int i,j;
	j=0;
	for (i=0; i</*64*/64/*25*/; i+=8)
	{
		j += (rand()&(/*32*/256-1));
		explosion_add(
			src->x+((co_s512(j))*i),
			src->y+((si_n512(j))*i),
			rand()&(64-1)/*%40*/,EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
	}
}
#endif
