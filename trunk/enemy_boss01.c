#include "enemy.h"

SPRITE *sb01[6];
extern SPRITE *player;
extern double fps_factor;
extern SDL_Surface *screen;
extern int difficulty;		//***090114		追加

typedef struct _boss01_data {
	ENEMY_BASE b;
	int state;
	int health_flag;
	int level;
} BOSS01_DATA;

int fire1=0;		//弾フラグ。真ん中の人形が壊れていて、かつ指定位置に付いた時。onになる。
int fire2=0;
int bwait1;			//弾発射までの待ち時間。
int bwait2;
int deg1;			//弾の角度
int deg2;
BOSS01_DATA *bdata;

void enemy_boss01_add(int lv)
{
	int i;
	fire1=0;
	fire2=0;
	deg1=0;
	deg2=0;
	
	BOSS01_DATA *b;

	sb01[0]=sprite_add_file("boss01-lo.png",2,PR_ENEMY);
	sb01[1]=sprite_add_file("boss01-mo.png",2,PR_ENEMY);
	sb01[2]=sprite_add_file("boss01-ro.png",2,PR_ENEMY);
	sb01[3]=sprite_add_file("boss01-lu.png",2,PR_ENEMY);
	sb01[4]=sprite_add_file("boss01-mu.png",2,PR_ENEMY);	//key
	sb01[5]=sprite_add_file("boss01-ru.png",2,PR_ENEMY);

	for(i=0;i<6;i++) {
		sb01[i]->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		sb01[i]->anim_speed=0;
		sb01[i]->aktframe=0;
		sb01[i]->type=SP_EN_BOSS01;
		b=mmalloc(sizeof(BOSS01_DATA));
		sb01[i]->data=b;
		if(i==0)
			b->b.health=200;		//***090114		変更(+50)
		else if(i==1)
			b->b.health=350;		//アリス本体のHP。もう少し高くてもいいかも。***090114	変更(+100)
		else if(i==2)
			b->b.health=200;		//***090114		変更(+50)
		else if(i==4)
			b->b.health=5;		//真ん中の子
		else
			b->b.health=60;  //denis was 40		***090114	変更(+20)
		if(i==1)
			b->b.score=1000*(difficulty+1);
		else
			b->b.score=500*(difficulty+1);
		b->health_flag=0;
		b->level=lv;
		if(i==1) {
			b->state=0;
			sb01[i]->mover=enemy_boss01_move;
		}

	}

	((PLAYER_DATA *)player->data)->bossmode=1;
}

void enemy_boss01_setpos(int x, int y)
{
	/*
		真ん中の人形が破壊されると、アリスの左右にいる人形の行動パターンが変わる。
	*/
	if(sb01[4]->flags&SP_FLAG_VISIBLE)		//真ん中の人形の生存確認
	{
		sb01[0]->x=x;
		sb01[0]->y=y;
		sb01[2]->x=sb01[1]->w+sb01[0]->w+x;
		sb01[2]->y=y;
	}
	else if(!fire1)		//発射できるかどうか
	{
		sb01[0]->x+=cos((M_PI/4)*5)*3*fps_factor;		//斜め後ろに移動
		sb01[0]->y+=sin((M_PI/4)*5)*3*fps_factor;
		if((sb01[0]->x<10)||(sb01[0]->y<10))
			fire1=1;			//移動完了
		if(!fire2)
		{
			sb01[2]->x+=cos((M_PI/4)*7)*3*fps_factor;
			sb01[2]->y+=sin((M_PI/4)*7)*3*fps_factor;
			if((sb01[2]->x>WIDTH2-50)||(sb01[2]->y<10))
				fire2=1;
		}
	}
	else if(!fire2)		//fire1は大丈夫だけどfire2が駄目
	{
		sb01[2]->x+=cos((M_PI/4)*7)*3*fps_factor;
		sb01[2]->y+=sin((M_PI/4)*7)*3*fps_factor;
		if((sb01[2]->x>WIDTH2-50)||(sb01[2]->y<10))
			fire2=1;
	}
	sb01[1]->x=sb01[0]->w+x;			//通常処理
	sb01[1]->y=y;
	sb01[3]->x=x;
	sb01[3]->y=sb01[0]->h+y;
	sb01[4]->x=sb01[3]->w+sb01[3]->x;
	sb01[4]->y=sb01[1]->h+y;
	sb01[5]->x=sb01[4]->w+sb01[4]->x;
	sb01[5]->y=sb01[2]->h+y;
	
	bdata=(BOSS01_DATA *)sb01[0]->data;
	if((fire1==1)&&(bdata->b.health>0))		//破壊済みなのに攻撃してこないように
	{
		if(bwait1<=0)
		{
			bwait1=12-bdata->level;
			deg1+=10;
			enemy_gr_bullet_create(sb01[0], 4+bdata->level, degtorad(-90+deg1%180), 0.04);
		}
		else
			bwait1--;
	}
	bdata=(BOSS01_DATA *)sb01[2]->data;
	if((fire2==1)&&(bdata->b.health>0))
	{
		if(bwait2<=0)
		{
			bwait2=12-bdata->level;
			deg2+=10;
			enemy_gr_bullet_create(sb01[2], 4+bdata->level, degtorad(+90+deg2%180), 0.04);
		}
		else
			bwait2--;
	}
}

/* boss wurde von player ber・rt */
void enemy_boss01_hitbyplayer(SPRITE *c)
{
}

/* boss wurde von player-weapon ber・rt */
void enemy_boss01_hitbyweapon(SPRITE *c, SPRITE *s, int angle)
{
	/*
		c = boss sprite
		s = player's weapon
	*/
	int i,j;
	BOSS01_DATA *b;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	for(i=0;i<6;i++)
		if(c==sb01[i])
			break;

	switch(i) {
		case 0:
			if(sb01[3]->flags&SP_FLAG_VISIBLE)
				i=3;
			break;

		case 1:
			if(sb01[3]->flags&SP_FLAG_VISIBLE)
				i=3;
			else if(sb01[5]->flags&SP_FLAG_VISIBLE)
				i=5;
			else if(sb01[0]->flags&SP_FLAG_VISIBLE)
			{
				if(!fire1)
					i=0;
			}
			else if(sb01[2]->flags&SP_FLAG_VISIBLE)
			{
				if(!fire2)
					i=2;
			}
			else if(sb01[4]->flags&SP_FLAG_VISIBLE)
				i=4;
			break;

		case 2:
			if(sb01[5]->flags&SP_FLAG_VISIBLE)
				i=5;
			break;
		case 4:
			//完全に姿を現すまで攻撃を一切受けない。代わりにアリス本体が攻撃を受け止める。
			b=(BOSS01_DATA *)sb01[1]->data;
			if(b->state<4)
				i=1;
	}
	
	b=(BOSS01_DATA *)sb01[i]->data;
	b->b.health-=w->strength;

	explosion_add(s->x,s->y,0,rand()%3+1);
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);
	

	if((b->b.health<=15)&&(b->health_flag==0)) {
		b->health_flag=1;
		explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,0,0);
	}
	if(b->b.health<=0) {
		explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,0,0);
		sb01[i]->flags&=~SP_FLAG_VISIBLE;
		bonus_multi_add(sb01[i]->x, sb01[i]->y,SP_BONUS_COIN,7,1);
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		if(i==1)		//アリスを倒すと皆破壊される。
		{
			explosion_add(sb01[0]->x+sb01[0]->w/2,sb01[0]->y+sb01[0]->h/2,0,0);
			sb01[0]->flags&=~SP_FLAG_VISIBLE;
			explosion_add(sb01[2]->x+sb01[2]->w/2,sb01[2]->y+sb01[2]->h/2,0,0);
			sb01[2]->flags&=~SP_FLAG_VISIBLE;
		}
	}

	j=0;
	for(i=0;i<6;i++) {
		if (b->health_flag==1)
			sb01[i]->aktframe=1;
		if(sb01[i]->flags&SP_FLAG_VISIBLE)
			j++;
	}
	if(j==0) {
		/* boss komplett zerstt */
		for(i=0;i<6;i++) {
			sb01[i]->type=-1;
			explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,rand()%20,0);
		}
		((PLAYER_DATA *)player->data)->bossmode=2;
	}
}


void enemy_boss01_move(SPRITE *c)
{
	BOSS01_DATA *b=(BOSS01_DATA *)c->data;
	static double x,y;
	static double w;
	static double firewait1;
	static int firewait2;

	switch(b->state) {
		case 0:
			x=WIDTH2/2-(sb01[0]->w+sb01[1]->w+sb01[2]->w)/2;
			y=-100;
			enemy_boss01_setpos(x,y);
			b->state=1;
			firewait1=45;
			firewait2=4;
			break;
		case 1:
			y+=fps_factor*(b->level+1);
			enemy_boss01_setpos(x,y);
			if(y>=30) {
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
			enemy_boss01_setpos(x,y);
			if(y>=80) {
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
			enemy_boss01_setpos(x,y);
			if(w>=60)
				b->state=6;
			break;
		case 6:
			y-=fps_factor*(b->level+1);
			x+=2*fps_factor*(b->level+1);
			w-=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w<=0)
				b->state=7;
			break;
		case 7:
			y+=fps_factor*(b->level+1);
			x+=2*fps_factor*(b->level+1);
			w+=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w>=60)
				b->state=8;
			break;
		case 8:
			y-=fps_factor*(b->level+1);
			x-=2*fps_factor*(b->level+1);
			w-=fps_factor;
			enemy_boss01_setpos(x,y);
			if(w<=0)
				b->state=4;
			break;
	}
	firewait1-=fps_factor*(b->level+1);
	if(firewait1<=0) {
		firewait1=45;
		firewait2--;
		if(firewait2==0) {
			enemy_boss01_fire(2);
			firewait2=4;
		} else {
			enemy_boss01_fire(0);
			enemy_boss01_fire(1);
		}
	}
}

void enemy_boss01_fire(int where) /* 0: left, 1: right, 2: bombenhagel */
{
	SPRITE *b;
	LASER_DATA *ldata;
	int angle;

	switch(where) {
		case 0:
			if(sb01[3]->flags&SP_FLAG_VISIBLE)
				enemy_bullet_create(sb01[3],5);
			break;
		case 1:
			if(sb01[5]->flags&SP_FLAG_VISIBLE)
				enemy_bullet_create(sb01[5],5);
			break;
		case 2:
			
			for(angle=0;angle<=180;angle+=20) {
				b=sprite_add_file("bshoot.png",1,PR_ENEMY);
				b->type=SP_EN_LASER;
				b->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
				b->mover=enemy_laser_move;
				//b->aktframe=0;
				b->x=sb01[4]->x;
				b->y=sb01[4]->y;
				ldata=mmalloc(sizeof(LASER_DATA));
				b->data=ldata;
				ldata->id=rand()%1000;
				ldata->angle=degtorad(angle);
				ldata->speed=4;
			}
			
			break;
	}
}
