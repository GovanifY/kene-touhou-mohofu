#include "enemy.h"

SPRITE *sb03[6];
extern SPRITE *player;
extern double fps_factor;
extern SDL_Surface *screen;
extern int difficulty;

typedef struct _boss03_data {
	ENEMY_BASE b;
	int state;
	int health_flag;
	int level;
} BOSS03_DATA;

void enemy_boss03_add(int lv)
{
	int i;
	BOSS03_DATA *b;

	if(lv==0){
		sb03[0]=sprite_add_file("boss05-lo.png",2,PR_ENEMY);
		sb03[1]=sprite_add_file("boss05-mo.png",2,PR_ENEMY);
		sb03[2]=sprite_add_file("boss05-ro.png",2,PR_ENEMY);
		sb03[3]=sprite_add_file("boss05-lu.png",2,PR_ENEMY);
		sb03[4]=sprite_add_file("boss05-mu.png",2,PR_ENEMY);
		sb03[5]=sprite_add_file("boss05-ru.png",2,PR_ENEMY);
	}
	else{
		sb03[0]=sprite_add_file("boss03-lo.png",2,PR_ENEMY);
		sb03[1]=sprite_add_file("boss03-mo.png",2,PR_ENEMY);
		sb03[2]=sprite_add_file("boss03-ro.png",2,PR_ENEMY);
		sb03[3]=sprite_add_file("boss03-lu.png",2,PR_ENEMY);
		sb03[4]=sprite_add_file("boss03-mu.png",2,PR_ENEMY);
		sb03[5]=sprite_add_file("boss03-ru.png",2,PR_ENEMY);
	}

	for(i=0;i<6;i++) {
		sb03[i]->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		sb03[i]->anim_speed=0;
		sb03[i]->aktframe=0;
		sb03[i]->type=SP_EN_BOSS03;
		b=mmalloc(sizeof(BOSS03_DATA));
		sb03[i]->data=b;
		if(lv==0)		//***090114		追加
		{
			if(i==1)
			{
				b->b.health=2000;
				b->b.score=3000+2000*difficulty;
			}
			else
			{
				b->b.health=200;
				b->b.score=600*(difficulty+1);
			}
		}
		else
		{
			b->b.health=40;  //denis was 40
			b->b.score=500*(difficulty+1);
		}
		b->health_flag=0;
		b->level=lv;
		if(i==1) {
			b->state=0;
			sb03[i]->mover=enemy_boss03_move;
		}
	}

	((PLAYER_DATA *)player->data)->bossmode=1;
}

void enemy_boss03_setpos(int x, int y)
{
	sb03[0]->x=x;
	sb03[0]->y=y;
	sb03[1]->x=sb03[0]->w+sb03[0]->x;
	sb03[1]->y=y;
	sb03[2]->x=sb03[1]->w+sb03[1]->x;
	sb03[2]->y=y;
	sb03[3]->x=x;
	sb03[3]->y=sb03[0]->h+y;
	sb03[4]->x=sb03[3]->w+sb03[3]->x;
	sb03[4]->y=sb03[1]->h+y;
	sb03[5]->x=sb03[4]->w+sb03[4]->x;
	sb03[5]->y=sb03[2]->h+y;
}

/* boss wurde von player ber・rt */
void enemy_boss03_hitbyplayer(SPRITE *c)
{
}

/* boss wurde von player-weapon ber・rt */
void enemy_boss03_hitbyweapon(SPRITE *c, SPRITE *s, int angle)
{
	int i,j;
	BOSS03_DATA *b;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	for(i=0;i<6;i++)
		if(c==sb03[i])
			break;

	switch(i) {
		case 0:
			if(sb03[3]->flags&SP_FLAG_VISIBLE)
				i=3;
			break;

		case 1:
			if(sb03[4]->flags&SP_FLAG_VISIBLE)
				i=4;
			else if(sb03[3]->flags&SP_FLAG_VISIBLE)
				i=3;
			else if(sb03[5]->flags&SP_FLAG_VISIBLE)
				i=5;
			else if(sb03[0]->flags&SP_FLAG_VISIBLE)
				i=0;
			else if(sb03[2]->flags&SP_FLAG_VISIBLE)
				i=2;
			break;

		case 2:
			if(sb03[5]->flags&SP_FLAG_VISIBLE)
				i=5;
			break;
	}
	
	b=(BOSS03_DATA *)sb03[i]->data;
	b->b.health-=w->strength;

	explosion_add(s->x,s->y,0,rand()%3+1);
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);
	

	if((b->b.health<=15)&&(b->health_flag==0)) {
		b->health_flag=1;
		explosion_add(sb03[i]->x+sb03[i]->w/2,sb03[i]->y+sb03[i]->h/2,0,0);
	}
	if(b->b.health<=0) {
		explosion_add(sb03[i]->x+sb03[i]->w/2,sb03[i]->y+sb03[i]->h/2,0,0);
		sb03[i]->flags&=~SP_FLAG_VISIBLE;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		bonus_multi_add(sb03[i]->x, sb03[i]->y,SP_BONUS_COIN,7,1);
	}

	j=0;
	for(i=0;i<6;i++) {
		if (b->health_flag==1)
			sb03[i]->aktframe=1;
		if(sb03[i]->flags&SP_FLAG_VISIBLE)
			j++;
	}
	if(j==0) {
		/* boss komplett zerstt */
		for(i=0;i<6;i++) {
			sb03[i]->type=-1;
			explosion_add(sb03[i]->x+sb03[i]->w/2,sb03[i]->y+sb03[i]->h/2,rand()%20,0);
		}
		((PLAYER_DATA *)player->data)->bossmode=2;
	}
}


//***090114		いろいろと追加します。
//3面と4面の差別化。
//4面には新しく追加した弾軌道を組み込む。
void enemy_boss03_move(SPRITE *c)
{
	BOSS03_DATA *b=(BOSS03_DATA *)c->data;
	static double x,y;
	static double w;
	static double firewait1;
	static int firewait2;
	static int firewait3;
	static int bomb_n;
	int i;

	switch(b->state) {
		case 0:
			x=WIDTH2/2-(sb03[0]->w+sb03[1]->w+sb03[2]->w)/2;		//ウィンドウ幅の変更
			y=-100;
			enemy_boss03_setpos(x,y);
			b->state=1;
			firewait1=45;
			firewait2=4;
			firewait3=0;
			bomb_n=0;
			break;
		case 1:
			y+=fps_factor*(b->level+1);
			enemy_boss03_setpos(x,y);
			if(y>=0) {
				b->state=2;
				w=0;
			}
			break;
		case 2:
			w+=fps_factor;
			if(w>=40) {
				b->state=3;
			}
			break;
		case 3:
			y+=fps_factor*(b->level+1);
			enemy_boss03_setpos(x,y);
			if(y>=30) {
				b->state=4;
				w=0;
			}
			break;
		case 4:
			w+=fps_factor;
			if(w>=30) {
				b->state=5;
				w=0;
			}
			break;
		case 5:
			y+=fps_factor*(b->level+1);
			x-=2*fps_factor*(b->level+1);
			w+=fps_factor;
			enemy_boss03_setpos(x,y);
			if(w>=60)
				b->state=6;
			break;
		case 6:
			y-=fps_factor*(b->level+1);
			x+=2*fps_factor*(b->level+1);
			w-=fps_factor;
			enemy_boss03_setpos(x,y);
			if(w<=0)
				b->state=7;
			break;
		case 7:
			y+=fps_factor*(b->level+1);
			x+=2*fps_factor*(b->level+1);
			w+=fps_factor;
			enemy_boss03_setpos(x,y);
			if(w>=60)
				b->state=8;
			break;
		case 8:
			y-=fps_factor*(b->level+1);
			x-=2*fps_factor*(b->level+1);
			w-=fps_factor;
			enemy_boss03_setpos(x,y);
			if(w<=0)
				b->state=4;
			break;
	}
	firewait1-=fps_factor*(b->level+1);
	if(firewait1<=0) {
		firewait1=45;
		firewait2--;
		if(firewait2==0) {
			enemy_boss03_fire(2);
			firewait2=4;
		} else {
			enemy_boss03_fire(0);
			enemy_boss03_fire(1);
		}
	}
	if(b->level==0)
	{
		if(b->b.health<=600)
		{
			if(firewait3 < 0)
			{
				if(bomb_n<32)
					bomb_n++;
				for(i=0;i<=(int)((double)bomb_n/3.0);i++)
				{
					enemy_pong_bullet_create(c, 5.0, 6*M_PI*i/(double)bomb_n+1, 0.07, 2);		//***090116		微調整
				}
				if(b->b.health>100)
					firewait3=b->b.health/2;
				else
					firewait3=50;
			}
			else
				firewait3--;
		}
	}
}

void enemy_boss03_fire(int where) /* 0: left, 1: right, 2: bombenhagel */
{
	SPRITE *b;
	LASER_DATA *ldata;
	int angle;

	switch(where) {
		case 0:
			if(sb03[3]->flags&SP_FLAG_VISIBLE)
				enemy_bullet_create(sb03[3],5);
			break;
		case 1:
			if(sb03[5]->flags&SP_FLAG_VISIBLE)
				enemy_bullet_create(sb03[5],5);
			break;
		case 2:
			for(angle=0;angle<=180;angle+=20) {
				b=sprite_add_file("bshoot.png",1,PR_ENEMY);
				b->type=SP_EN_LASER;
				b->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
				b->mover=enemy_laser_move;
				//b->aktframe=0;
				b->x=sb03[4]->x;
				b->y=sb03[4]->y;
				ldata=mmalloc(sizeof(LASER_DATA));
				b->data=ldata;
				ldata->angle=degtorad(angle);
				ldata->speed=6;
			}
			break;
	}
}
