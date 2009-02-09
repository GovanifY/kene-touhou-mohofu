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
		
		enemy_gr_bullet_create
		下方向に加速していく中弾。
		フレーム毎で計算しているのでかなり加速度を小さくしないと大変なことに。
	//***090114
		大玉の動きを変えてみた
*/

extern SDL_Surface *screen;
extern SPRITE *player;
extern double fps_factor;
extern int difficulty;		//***090114		追加
int b_id;

void enemy_bullet_create(SPRITE *s, double speed)
{
	SPRITE *h;
	BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	data->speed=speed;
}

void enemy_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	BULLET_DATA *d=(BULLET_DATA *)s->data;

	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_laser_create(SPRITE *s, double speed)
{
	SPRITE *h;
	LASER_DATA *data;

	h=sprite_add_file("bshoot.png",1,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_laser_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(LASER_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	data->speed=speed;

}

void enemy_laser_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	LASER_DATA *d=(LASER_DATA *)s->data;

	//s->aktframe=(radtodeg(d->angle)/10)+9;
	//s->aktframe%=18;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_laser_create2(SPRITE *s, double speed, double angle)
{
	//***090129		角度指定を付けただけ
	SPRITE *h;
	LASER_DATA *data;

	h=sprite_add_file("bshoot.png",1,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_laser_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(LASER_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=angle;
	data->speed=speed;

}

void enemy_laser_move2(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	LASER_DATA *d=(LASER_DATA *)s->data;

	//s->aktframe=(radtodeg(d->angle)/10)+9;
	//s->aktframe%=18;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_homing_create(SPRITE *s)
{
	SPRITE *r;
	HOMING_DATA *data;

	r=sprite_add_file("rotating_rocket.png",20,PR_ENEMY);
	r->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	r->type=SP_EN_HOMING;
	r->x=s->x;
	r->y=s->y;
	r->mover=enemy_homing_move;
	data=mmalloc(sizeof(HOMING_DATA));
	r->data=data;
	data->b.score=5;
	data->b.health=1;
	data->angle=0;
	data->speed=2;
	data->range=200;
	data->delay=10;

	enemy_homing_update(r);
}

void enemy_homing_update(SPRITE *s)
{

	HOMING_DATA *d=(HOMING_DATA *)s->data;
	
	/* Zielwinkel erfassen */
	d->angle=atan2(player->y-s->y,player->x-s->x);
	s->aktframe=(radtodeg(d->angle)*20)/360;
	s->aktframe%=20;
}

void enemy_homing_move(SPRITE *s)
{
	//SDL_Surface *tmp;
	HOMING_DATA *d=(HOMING_DATA *)s->data;

	d->delay-=fps_factor;
	if(d->delay >0) {
		s->x+=cos(d->angle)*d->speed*fps_factor;
		s->y+=sin(d->angle)*d->speed*fps_factor;
	} else {
		/* nur jeden 10ten Frame Winkel aktualisieren */
		d->delay=10;
		enemy_homing_update(s);
	}


	/* max. reichweite erreicht? self-destruct! */
	d->range-=fps_factor;
	if(d->range<=0) {
		s->type=-1;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);
	}
}

void enemy_bigbullet_create(SPRITE *s, double ex, double ey, double speed, double angle, int ransu)
{
	/*
		ransu		0～60くらいまで。大きいほど挙動不審な動きになる
					角度的に+方向へ流れる傾向が強い気がする=>天狗第一形態。修正する必要がありそう。
		angle		-2を指定するとプレイヤーに向かう。基本は角度指定(°)
		ex,ey		出現位置
	*/
	SPRITE *h,*hu;
	BIGBULLET_DATA *data;
	BIGBULLET_S_DATA *src_d;

	h=sprite_add_file("bigkugel2.png",1,PR_ENEMY);
	h->type=SP_EN_BIGBULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_bigbullet_move;
	h->aktframe=0;
	h->x=ex-h->w/2;
	h->y=ey-h->h/2;
	
	hu=sprite_add_file2("bigkugel1.png",1,PR_TMP);	
	hu->type=SP_EN_BIGBULLET_DUMMY;
	hu->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	hu->mover=enemy_bigbullet_move2;
	hu->aktframe=0;
	hu->x=ex-hu->w/2;
	hu->y=ey-hu->h/2;

	data=mmalloc(sizeof(BIGBULLET_DATA));
	h->data=data;
	
	src_d=mmalloc(sizeof(BIGBULLET_S_DATA));
	hu->data=src_d;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-player->h/2-s->y,player->x-player->w/2-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->ransu=ransu;
	data->wait_bg=10;
	
	src_d->sw=h->w;
	src_d->sh=h->h;
	src_d->type=&h->type;
	src_d->sx=&h->x;
	src_d->sy=&h->y;
}

void enemy_bigbullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[4];
		pd->score+=100;
		sprintf(buffer,"100");
		bonus_info_text(s->x,s->y,buffer,FONT07);		//表示されてない？
	}
	BIGBULLET_DATA *d=(BIGBULLET_DATA *)s->data;
	if(d->wait_bg>0)		//***090124		追加場所。今までフレーム毎に計算していた所を5フレーム毎に変更
		d->wait_bg--;
	else
	{
		d->wait_bg=5;
		d->angle+=((rand()%d->ransu)-d->ransu/2)/10;
	}
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_bigbullet_move2(SPRITE *s)
{
	BIGBULLET_S_DATA *d=(BIGBULLET_S_DATA *)s->data;
	
	s->x=*(d->sx)+d->sw/2-s->w/2;
	s->y=*(d->sy)+d->sh/2-s->h/2;
	if(*(d->type)==-1) {
		s->type=-1;
	}
}

void enemy_new_bigbullet_create(SPRITE *s, double speed, double angle, double a)
{
	SPRITE *h,*hu;
	NEW_BIGBULLET_DATA *data;
	BIGBULLET_S_DATA *src_d;
	double tmp_angle;

	h=sprite_add_file("bigkugel1.png",1,PR_ENEMY);
	h->type=SP_EN_BIGBULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_new_bigbullet_move;
	h->aktframe=0;
	h->x=s->x-h->w/2;
	h->y=s->y-h->h/2;
	
	hu=sprite_add_file2("new_bigkugel.png",1,PR_TMP);	
	hu->type=SP_EN_BIGBULLET_DUMMY;
	hu->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	hu->mover=enemy_bigbullet_move2;
	hu->aktframe=0;
	hu->x=s->x-hu->w/2;
	hu->y=s->y-hu->h/2;

	data=mmalloc(sizeof(NEW_BIGBULLET_DATA));
	h->data=data;
	
	src_d=mmalloc(sizeof(BIGBULLET_S_DATA));
	hu->data=src_d;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-player->h/2-s->y,player->x-player->w/2-s->x);
	else{
		tmp_angle=angle;
		while(1){
			if(tmp_angle>M_PI)
				tmp_angle-=2*M_PI;
			else if(tmp_angle<-M_PI)
				tmp_angle+=2*M_PI;
			else
				break;
		}
		data->angle=tmp_angle;
	}
	data->speed=speed;
	data->a=a;
	src_d->sw=h->w;
	src_d->sh=h->h;
	src_d->type=&h->type;
	src_d->sx=&h->x;
	src_d->sy=&h->y;
}

void enemy_new_bigbullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[4];
		pd->score+=100;
		sprintf(buffer,"100");
		bonus_info_text(s->x,s->y,buffer,FONT07);		//表示されてない？
	}
	NEW_BIGBULLET_DATA *d=(NEW_BIGBULLET_DATA *)s->data;
	d->speed+=d->a;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x<-s->w)||(s->x>WIDTH2)||(s->y<-s->h)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_g_bullet_create(SPRITE *s, double speed, int state, double angle)
{
	/*
		state	十進数で一の位がx、十の位がy
				指定した分だけ角度が変わる。
		angle	ラジアン指定。-2でプレイヤーに向かう。
	*/
	SPRITE *h;
	G_BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_g_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(G_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-s->y,player->x-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->state=state;
}

void enemy_g_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	G_BULLET_DATA *d=(G_BULLET_DATA *)s->data;

	s->x+=cos(d->angle)*d->speed*fps_factor+(d->state%10);
	s->y+=sin(d->angle)*d->speed*fps_factor+(d->state/10);
	if((s->x<0)||(s->x>WIDTH2)||(s->y<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

//***090114	追加
void enemy_pong_bullet_create(SPRITE *s, double speed, double angle, double gra, int bou)
{
	/*
		angle	-2でプレイヤー
				基本はラジアン
		gra		フレーム毎に下方向にどれだけ加速するか
	*/
	SPRITE *h;
	PO_BULLET_DATA *data;

	h=sprite_add_file("bshoot2.png",3,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_pong_bullet_move;
	h->aktframe=bou;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(PO_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-s->y,player->x-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->gra=gra;
	data->sum=0;
	data->bounds=bou;
}

void enemy_pong_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	PO_BULLET_DATA *d=(PO_BULLET_DATA *)s->data;
	if(d->sum<15)
		d->sum+=d->gra;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor+d->sum;
	if((s->x<0)||(s->x>WIDTH2)) {
		d->angle=atan2(sin(d->angle),-cos(d->angle));
	}
	else if(s->y>HEIGHT)
	{
		if(d->bounds)
		{
			d->speed=-d->speed;
			d->sum=-d->sum;
			d->bounds--;
			s->aktframe=d->bounds;		//***090116		若干変更
			d->angle=atan2(sin(d->angle),-cos(d->angle));		//***090116		追加
		}
		else
			s->type=-1;
	}
}

void enemy_gr_bullet_create(SPRITE *s, double speed, double angle, double gra)
{
	/*
		angle	-2でプレイヤー
				基本はラジアン
		gra		フレーム毎に下方向にどれだけ加速するか
	*/
	SPRITE *h;
	GR_BULLET_DATA *data;

	h=sprite_add_file("bshoot.png",1,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_gr_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(GR_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y-s->y,player->x-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->gra=gra;
	data->sum=0;
}

void enemy_gr_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	GR_BULLET_DATA *d=(GR_BULLET_DATA *)s->data;
	d->sum+=d->gra;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor+d->sum;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<-50)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_stop_bullet_create(SPRITE *s, double speed, double angle, double a)
{
	/*
		angle	-2でプレイヤー
				基本はラジアン
		a		フレーム毎にspeedをどれだけ減速するか
	*/
	SPRITE *h;
	ST_BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_stop_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(GR_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->a=a;
	data->timer=800;
}

void enemy_stop_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	ST_BULLET_DATA *d=(ST_BULLET_DATA *)s->data;
	if(d->speed>0)
		d->speed-=d->a;
	else if(d->speed)
		d->speed=0;
	else
		d->timer--;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor+fps_factor-pd->bossmode*fps_factor;
	if(d->timer<0)
		s->alpha-=17;
	if((s->x<0)||(s->x>WIDTH2)||(s->y>HEIGHT)||(s->y<-100)||(s->alpha<0)) {
		s->type=-1;
	}
}

void enemy_stop_bullet2_create(SPRITE *s, double speed, double angle, double a, double next_angle)
{
	SPRITE *h;
	ST2_BULLET_DATA *data;
	double tmp_angle=next_angle;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_stop_bullet2_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	if(tmp_angle>M_PI)
		tmp_angle-=2*M_PI;
	else if(tmp_angle<-M_PI)
		tmp_angle+=2*M_PI;

	data=mmalloc(sizeof(ST2_BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	if(angle==-2)
		data->angle=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	else
		data->angle=angle;
	data->speed=speed;
	data->state=0;
	data->a=a;
	data->next_angle=tmp_angle;
	data->timer=100;
}

void enemy_stop_bullet2_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	ST2_BULLET_DATA *d=(ST2_BULLET_DATA *)s->data;
	
	switch(d->state){
		case 0:
			d->speed-=d->a;
			if(d->speed<0){
				d->speed=0;
				d->state=1;
			}
			break;
		case 1:
			d->timer--;
			if(d->timer<0){
				d->timer=0;
				d->state=2;
				enemy_new_bigbullet_create(s, 0, d->next_angle, d->a*7);
			}
			break;
		case 2:
			if(s->alpha<0){
				s->alpha=0;
				s->type=-1;
			}
			else if(s->alpha>0){
				s->alpha-=17;
			}
			break;
	}
	
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor+fps_factor-pd->bossmode*fps_factor;
	
	if((s->x<-50)||(s->x>WIDTH2+60)||(s->y>HEIGHT+60)||(s->y<-50)||(s->alpha<0)) {
		s->type=-1;
	}
}

void enemy_angle_bullet_create(SPRITE *s, double speed, double angle, double a_angle){
	/*
	typedef struct {
	double angle;
	double a_angle;
	double speed;
	int id;
	int timer;
} AN_BULLET_DATA;
	*/
	SPRITE *h;
	AN_BULLET_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_angle_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(AN_BULLET_DATA));
	h->data=data;
	b_id++;
	data->id=b_id%1000;
	data->angle=angle;
	data->speed=speed;
	data->a_angle=a_angle;
	data->timer=0;
}

void enemy_angle_bullet_move(SPRITE *s){
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	AN_BULLET_DATA *d=(AN_BULLET_DATA *)s->data;
	if(d->timer<90){
		d->timer++;
		d->angle+=d->a_angle;
	}
	if(d->angle>M_PI)
		d->angle=d->angle-2*M_PI;
	else if(d->angle<-M_PI)
		d->angle=2*M_PI+d->angle;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor+fps_factor;
	if(((s->x<0)||(s->x>WIDTH2)||(s->y>HEIGHT)||(s->y<-100))) {
		s->type=-1;
	}
}

void enemy_knife_create(SPRITE *s, double speed, double angle, int anim)
{
	SPRITE *h;
	BULLET_DATA *data;
	char filename[30];
	sprintf(filename,"knife%d.png",anim);
	h=sprite_add_file2(filename,1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_bullet_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(BULLET_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=angle;
	data->speed=speed;
}

void enemy_knife_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	BULLET_DATA *d=(BULLET_DATA *)s->data;

	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;
	if((s->x+s->w<0)||(s->x>WIDTH2)||(s->y+s->h<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_fall_knife_create(SPRITE *s, double speed, double angle, double gra)
{
	SPRITE *h;
	FALL_KNIFE_DATA *data;
	
	h=sprite_add_file2("knife.png",1,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_fall_knife_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(FALL_KNIFE_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=angle;
	data->speed=speed;
	data->gra=gra;
	data->sum=0;
}

void enemy_fall_knife_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	FALL_KNIFE_DATA *d=(FALL_KNIFE_DATA *)s->data;
	double tmp;
	tmp=sin(d->angle)*d->speed*fps_factor;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	if(tmp+d->sum<=3)
		d->sum+=d->gra;
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=tmp+d->sum;
	if((s->x<0)||(s->x>WIDTH2)||(s->y<-100)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_follow_knife_create(SPRITE *s, double speed, double angle, int height)
{
	SPRITE *h;
	FOLLOW_KNIFE_DATA *data;
	h=sprite_add_file2("knife_core.png",18,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_follow_knife_move;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=angle;
	data->speed=(double)speed;
	data->speed2=speed;
	data->target=0;
	data->height=height;
	data->timer=300;
}

void enemy_follow_knife_create2(double x, double y, double speed, double angle, int height){
	SPRITE *h;
	FOLLOW_KNIFE_DATA *data;
	h=sprite_add_file2("knife_core.png",18,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_follow_knife_move;
	h->aktframe=0;
	h->x=x;
	h->y=x;

	data=mmalloc(sizeof(FOLLOW_KNIFE_DATA));
	h->data=data;

	data->id=rand()%1000;
	data->angle=angle;
	data->speed=(double)speed;
	data->speed2=speed;
	data->target=0;
	data->height=height;
	data->timer=300;
}

void enemy_follow_knife_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	FOLLOW_KNIFE_DATA *d=(FOLLOW_KNIFE_DATA *)s->data;
	if(d->target)
	{
		s->x+=cos(d->angle)*2*d->speed2*fps_factor;
		s->y+=sin(d->angle)*2*d->speed2*fps_factor;
	}
	else
	{
		s->x+=cos(d->angle)*d->speed*fps_factor;
		s->y+=sin(d->angle)*d->speed*fps_factor;
		d->speed-=0.02;
		if(s->aktframe==17)
			s->aktframe=0;
		else
			s->aktframe++;
		if(d->speed<=0)
		{
			d->angle=atan2(player->y+player->h/2+d->height-s->y,player->x-player->w/2-s->x);
			d->target=1;
			s->aktframe=17-(radtodeg(d->angle)/20)%18;
		}
	}
	if(((s->x+s->w<0)||(s->x>WIDTH2)||(s->y+s->h<0)||(s->y>HEIGHT))&&(d->timer<0)) {
		s->type=-1;
	}
	d->timer--;
}

void enemy_even_knife_create(SPRITE *s, double speed, double length, int r_or_l)
{
	double tmp;
	/*
		length	enemy-player間の距離/√2
		r_or_l	0=右	1=左
	*/
	SPRITE *h;
	EVEN_KNIFE_DATA *data;

	h=sprite_add_file("knife_core.png",18,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(EVEN_KNIFE_DATA));
	h->data=data;

	data->id=rand()%1000;
	tmp=atan2(player->y+player->h/2-s->y-s->h/2,player->x-player->w/2-s->x-s->w/2);
	if(r_or_l)
	{
		if(tmp>3*M_PI/4)
			tmp-=7*M_PI/4;
		else
			tmp+=M_PI/4;
		data->x=s->x+s->w/2+cos(tmp)*length;
		data->y=s->y+s->h/2+sin(tmp)*length;
		h->mover=enemy_evenl_knife_move;
	}
	else
	{
		if(tmp<-3*M_PI/4)
			tmp=7*M_PI/4-tmp;
		else
			tmp-=M_PI/4;
		data->x=s->x+s->w/2+cos(tmp)*length;
		data->y=s->y+s->h/2+sin(tmp)*length;
		h->mover=enemy_evenr_knife_move;
	}
	data->d_angle=speed/(2*M_PI*length);	//⊿angle≒任意/(2π×length/√2)
	data->angle=tmp;
	data->speed=speed;
	data->state=0;
	data->wait1=0;
	data->wait2=0;
	data->length=length;
}

void enemy_evenl_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d=(EVEN_KNIFE_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[4];
		pd->score+=100;
		sprintf(buffer,"100");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	switch(d->state)
	{
		case 0:
			d->angle+=d->d_angle*fps_factor;
			if(d->angle>M_PI)
				d->angle=-M_PI;
			if(d->wait1>100){
				d->wait1=0;
				d->state=1;
			}
			s->aktframe=17-(radtodeg(d->angle-M_PI/2)/20)%18;
			s->x=d->x-cos(d->angle)*d->length-s->w/2;
			/*
				s->x		ナイフのx座標
				d->x		ノードのx座標
				s->w/2		ナイフの横幅の半分
				ナイフのx座標＝ノードのx座標＋cos(d->angle)×length－ナイフの横幅の半分
			*/
			s->y=d->y-sin(d->angle)*d->length-s->h/2;
			//s->aktframe=(d->angle/10);
			//s->aktframe%=36;
			break;

		case 1:
			if(d->wait1>50){
				d->wait1=0;
				d->state=0;
			}
			s->x+=cos(d->angle)*d->speed*fps_factor;
			s->y+=sin(d->angle)*d->speed*fps_factor;
			break;
	}
	if(d->wait1>10)
	{
		enemy_knife_create(s, 5, d->angle-M_PI/2, s->aktframe);
		enemy_knife_create(s, 4, d->angle-2*M_PI/3, s->aktframe);
		enemy_knife_create(s, 3, d->angle-5*M_PI/6, s->aktframe);
		if(difficulty>1)
			enemy_knife_create(s, 2, d->angle-M_PI, s->aktframe);
		d->wait1=0;
	}
	//s->type=-1;
	if(d->wait2>150+difficulty*50)
		s->type=-1;
	
	d->wait1++;
	d->wait2++;
}

void enemy_evenr_knife_move(SPRITE *s)
{
	EVEN_KNIFE_DATA *d=(EVEN_KNIFE_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[4];
		pd->score+=100;
		sprintf(buffer,"100");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}
	switch(d->state)
	{
		case 0:
			d->angle-=d->d_angle*fps_factor;
			if(d->angle<-M_PI)
				d->angle=M_PI;
			if(d->wait1>100){
				d->wait1=0;
				d->state=1;
			}
			s->aktframe=17-(radtodeg(d->angle+M_PI/2)/20)%18;
			s->x=d->x-cos(d->angle)*d->length-s->w/2;
			s->y=d->y-sin(d->angle)*d->length-s->h/2;
			//s->aktframe=(d->angle/10);
			//s->aktframe%=36;
			break;
		
		case 1:
			if(d->wait1>50){
				d->wait1=0;
				d->state=0;
			}
			s->x+=cos(d->angle)*d->speed*fps_factor;
			s->y+=sin(d->angle)*d->speed*fps_factor;
			break;
	}
	if(d->wait1>10)
	{
		enemy_knife_create(s, 5, d->angle+M_PI/2, s->aktframe);		//+M_PI/2
		enemy_knife_create(s, 4, d->angle+4*M_PI/6, s->aktframe);
		enemy_knife_create(s, 3, d->angle+5*M_PI/6, s->aktframe);
		if(difficulty>1)
			enemy_knife_create(s, 2, d->angle+M_PI, s->aktframe);
		d->wait1=0;
	}
	//s->type=-1;
	if(d->wait2>150+difficulty*50)
		s->type=-1;
	
	d->wait1++;
	d->wait2++;
}

void enemy_n_way_bullet(SPRITE *s, char *filename, int frame, int n, double speed, double angle){
	//angleはラジアン
	//frameはフレーム数-1
	SPRITE *h;
	BULLET_DATA *data;
	int i;
	double i_angle=angle-(n+2)*0.13089969389957471827;	//π/24

	for(i=0;i<n;i++){
		h=sprite_add_file(filename, frame+1, PR_ENEMY);
		h->type=SP_EN_BULLET;
		h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		h->mover=enemy_bullet_move;
		if(frame)
			h->anim_speed=1;
		h->aktframe=0;
		h->x=s->x+(s->w/2-h->w/2);
		h->y=s->y+(s->h/2-h->h/2);

		data=mmalloc(sizeof(BULLET_DATA));
		h->data=data;

		data->id=rand()%1000;
		i_angle+=0.26179938779914943654;
		if(i_angle>M_PI)
			i_angle-=2*M_PI;
		else if(i_angle<-M_PI)
			i_angle+=2*M_PI;
		data->angle=i_angle;	//π/12
		data->speed=speed;
	}
}

void explosion_add(int x, int y, double wait, int type)
{
	SPRITE *ex=NULL;
	EXPLOSION_DATA *d;

	d=mmalloc(sizeof(EXPLOSION_DATA));
	switch(type) {
	case 0:
		ex=sprite_add_file("ex.png",29,PR_BONUS);
		d->framewait=2;
		break;
	case 1:
		ex=sprite_add_file("tr_blue.png",6,PR_BONUS);
		d->framewait=1;
		break;
	case 2:
		ex=sprite_add_file("tr_red.png",6,PR_BONUS);
		d->framewait=1;
		break;
	case 3:
		ex=sprite_add_file("tr_green.png",6,PR_BONUS);
		d->framewait=1;
		break;
	default:
		error(ERR_WARN,"explosion_add: unknown type: %d",type);
	}
	
	
	ex->flags&=~SP_FLAG_VISIBLE;
	ex->anim_speed=0;
	ex->aktframe=-1;
	ex->type=SP_ETC;
	ex->x=x-(ex->w/2);
	ex->y=y-(ex->h/2);
	ex->mover=explosion_move;
	ex->data=d;
	d->wait=wait;
	d->framestat=0;
}

void explosion_move(SPRITE *c)
{
	EXPLOSION_DATA *d=(EXPLOSION_DATA *)c->data;

	if(d->wait>0)
		d->wait-=fps_factor;
	else {
		d->framestat+=fps_factor;
		if(d->framestat>d->framewait) {
			d->framestat=0;
			c->flags|=SP_FLAG_VISIBLE;
			if(c->aktframe<c->frames-1)
				c->aktframe++;
			else
				c->type=-1;
		}
	}
}
