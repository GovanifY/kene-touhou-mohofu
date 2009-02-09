#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;
extern SDL_Surface *screen;
extern int difficulty;		//***090127		�ǉ�


typedef struct _boss04_data {
	ENEMY_BASE b;
	int type;	/* 0: normal, 1: destroyed */
	int state1;	//�`��
	int state2;	//�s��
	double sp_time;	//��������
	double wait1;
	double wait2;
	int level;
	double move_angle;
	double angle;
	double length_s_p;
} BOSS04_DATA;

typedef struct _boss04_maho_data {
	SPRITE *c;
	int state1;	//�`��
	int state2;	//�s��
	int n;
	double wait;
	double angle;
} BOSS04_MAHO_DATA;

typedef struct {
	BOSS04_DATA *sd;
	double angle;
	double angle2;
	double speed;
	int id;
	double gra;
	double sum;
	int timer;
	int r_or_l;		//r=-1, l=1
}BULLET_SP1_DATA;

typedef struct {
	BOSS04_MAHO_DATA *sd;
	int id;
	int state;
	double angle;
	double gra;
	double sum;
}BULLET_SP2_DATA;

void enemy_sp1_bullet_create(SPRITE *s, double speed, double angle, double gra, int r_or_l)
{
	SPRITE *h;
	BULLET_SP1_DATA *data;

	h=sprite_add_file("bshoot2.png",3,PR_ENEMY);	
	h->type=SP_EN_LASER;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_sp1_bullet_move;
	h->anim_speed=0;
	h->aktframe=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(BULLET_SP1_DATA));
	h->data=data;
	if(angle==-2)
		data->angle=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
	else
		data->angle=angle;
	data->angle2=0;
	data->sd=(BOSS04_DATA *)s->data;
	data->speed=speed;
	data->id=rand()%1000;
	data->speed=speed;
	data->gra=gra;
	data->sum=0;
	data->timer=0;
	data->r_or_l=r_or_l;
}

void enemy_sp1_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	BULLET_SP1_DATA *d=(BULLET_SP1_DATA *)s->data;
	
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=50;
		sprintf(buffer,"50");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}

	if(d->timer>40){
		d->sum+=d->gra;
		s->x+=(cos(d->angle)*d->speed*d->sum/5+cos(d->angle2)*d->sum)*fps_factor;
		s->y+=(sin(d->angle)*d->speed*d->sum/5+sin(d->angle2)*d->sum)*fps_factor;
	}
	else if(d->timer==40){
		d->angle=d->sd->angle-M_PI/2*d->r_or_l;
		s->aktframe=1;
		d->angle2=d->sd->angle;
	}
	else{
		s->x+=cos(d->angle)*d->speed*fps_factor;
		s->y+=sin(d->angle)*d->speed*fps_factor;
	}
		
	if((s->x<-s->x)||(s->x>WIDTH2)||(s->y<-s->y)||(s->y>HEIGHT)) {
		s->type=-1;
	}
	d->timer++;
}

void enemy_sp2_bullet_create(SPRITE *s, int angle, double gra)
{
	/*
		angle	�x�w��
	*/
	SPRITE *h;
	BULLET_SP2_DATA *data;

	h=sprite_add_file("kugel.png",1,PR_ENEMY);	
	h->type=SP_EN_BULLET;
	h->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	h->mover=enemy_sp2_bullet_move;
	h->anim_speed=0;
	h->x=s->x+(s->w/2-h->w/2);
	h->y=s->y+(s->h/2-h->h/2);

	data=mmalloc(sizeof(BULLET_SP2_DATA));
	h->data=data;
	data->id=rand()%1000;
	data->sd=(BOSS04_MAHO_DATA *)s->data;
	data->state=0;
	data->angle=degtorad(angle);
	data->gra=gra;
	data->sum=0;
}

void enemy_sp2_bullet_move(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	BULLET_SP2_DATA *d=(BULLET_SP2_DATA *)s->data;
	
	if(pd->bossmode==2||pd->bossmode==4)
	{
		s->type=-1;
		char buffer[3];
		pd->score+=30;
		sprintf(buffer,"30");
		bonus_info_text(s->x,s->y,buffer,FONT07);
	}

	if(d->sd->n%(4-difficulty)==1){
		d->sum+=d->gra;
		s->x+=cos(d->angle)*d->sum*fps_factor;
		s->y+=sin(d->angle)*d->sum*fps_factor;
	}

	if((s->x<-s->x)||(s->x>WIDTH2)||(s->y<-s->y)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void enemy_boss04_add(int lv)
{
	BOSS04_DATA *b;
	SPRITE *s;

	s=sprite_add_file("boss04.png",7,PR_ENEMY);
	s->flags|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
	s->anim_speed=0;
	s->aktframe=1;
	s->type=SP_EN_BOSS04;
	b=mmalloc(sizeof(BOSS04_DATA));
	s->data=b;
	b->b.health=5000;
	b->b.score=5000+4000*difficulty;
	b->state1=0;
	b->state2=0;
	b->wait1=0;
	b->wait2=0;
	b->type=0;
	b->level=lv;
	b->move_angle=0;
	b->sp_time=1000;
	s->mover=enemy_boss04_move;
	s->x=WIDTH2/2-(s->w/2);
	s->y=-(s->h);

	((PLAYER_DATA *)player->data)->bossmode=1;
}

void enemy_boss04_hitbyplayer(SPRITE *c)
{
}

void enemy_boss04_hitbyweapon(SPRITE *c, SPRITE *s, int angle)
{
	/*
		c		�{�X�{��
		s		�v���C���[�̒e
	*/
	BOSS04_DATA *b=(BOSS04_DATA *)c->data;
	int i,j;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	explosion_add(s->x,s->y,0,rand()%3+1);
	b->b.health-=w->strength;
	if(b->b.health<(4-b->state1)*1000){
		playChunk(7);
		((PLAYER_DATA *)player->data)->bossmode=4;
		bonus_multi_add(c->x, c->y, SP_BONUS_COIN, 5, 1);
		bonus_multi_add(c->x, c->y, SP_BONUS_FIREPOWER, 5, 0);
		bonus_multi_add(c->x, c->y, SP_BONUS_FIREPOWER_G, 1, 0);
		s->aktframe=1;
		b->state1++;
		b->state2=0;
		b->wait1=80;
		b->wait2=-100;
		b->sp_time=3000;
	}

	if(b->b.health<=0) {
		for(i=0;i<100;i+=30) {
			for(j=0;j<100;j+=30) {
				explosion_add(c->x+i,c->y+j,rand()%30,0);
			}
		}
		bonus_multi_add(c->x, c->y,SP_BONUS_COIN,15,1);
		c->type=-1;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		((PLAYER_DATA *)player->data)->bossmode=2;
	}
}

void enemy_boss04_move(SPRITE *s)
{
	BOSS04_DATA *b=(BOSS04_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	int ang;

	b->sp_time-=fps_factor;

	switch(b->state1) {
		case 0:	//���`��:���A���X�Ƃقړ�������
			switch(b->state2) {
				case 0:
					if(s->y<20){
						s->y+=fps_factor;
					}
					else{	//y��20�܂ŗ�����
						b->state2=1;
						enemy_bullet_create(s, 2.5);
						enemy_bullet_create(s, 2);
						enemy_bullet_create(s, 3.5);
						s->aktframe=0;
					}
					break;
				case 1:
					if(s->x>=50){
						s->y+=fps_factor;
						s->x-=fps_factor*3;
					}
					else{	//x��50�ȉ��ɂȂ�����
						b->state2=2;
						b->wait1=50;
						enemy_bullet_create(s, 2.5);
						enemy_bullet_create(s, 2);
						enemy_bullet_create(s, 3.5);
						s->aktframe=1;
					}
					break;
				case 2:
					enemy_boss04_waitstate(s, 3, 2);
					break;
				case 3:
					if(s->y>20){
						s->y-=fps_factor;
						s->x+=fps_factor*3;
					}
					else{	//�^�񒆂܂ŗ�����
						b->state2=4;
						enemy_bullet_create(s, 2.5);
						enemy_bullet_create(s, 2);
						enemy_bullet_create(s, 3.5);
					}
					break;
				case 4:
					if(s->x+s->w<=WIDTH2-50){
						s->y+=fps_factor;
						s->x+=fps_factor*3;
					}
					else{
						b->state2=5;
						b->wait1=50;
						enemy_bullet_create(s, 2.5);
						enemy_bullet_create(s, 2);
						enemy_bullet_create(s, 3.5);
						s->aktframe=1;
					}
					break;
				case 5:
					enemy_boss04_waitstate(s, 6, 0);
					break;
				case 6:
					if(s->y>20){
						s->y-=fps_factor;
						s->x-=fps_factor*3;
					}
					else{
						b->state2=1;
						enemy_bullet_create(s, 2.5);
						enemy_bullet_create(s, 2);
						enemy_bullet_create(s, 3.5);
					}
					break;
			}
			break;

		case 1:	//���`��:���܂����x
				//b->wait2		���[�v��
			switch(b->state2)
			{
				case 0:
					if(b->wait1<0){
						b->state2=1;
						pd->bossmode=1;
						b->wait2=0;
					}else{
						b->wait1-=fps_factor;
					}
					break;
				case 1:
					if(rand()%2){
						ang=rand()%180;
						if(ang>90){
							ang+=90;
							s->aktframe=2;
						}
						else{
							s->aktframe=0;
						}
						ang-=45;
					}
					else{
						if(s->x>WIDTH2/2){
							ang=180;
							s->aktframe=0;
						}
						else{
							ang=0;
							s->aktframe=2;
						}
					}
					b->move_angle=degtorad(ang);
					b->state2=2;
					b->wait1=20+(3-difficulty)*10;
					b->wait2++;
					break;
				case 2:
					if(b->wait1<0){
						b->wait1=35;
						b->state2=3;
						s->aktframe=1;
					}else{
						s->x+=cos(b->move_angle)*fps_factor;
						s->y+=sin(b->move_angle)*fps_factor;
						b->wait1-=fps_factor;
					}
					break;
				case 3:
					enemy_boss04_waitstate(s, 4, 4);
					break;
				case 4:
					playChunk(11);
					if((int)b->wait2%2){
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.3, M_PI/2, 0.013);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.6, M_PI/4, 0.011);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.9, 0, 0.009);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.2, -M_PI/4, 0.007);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.5, -M_PI/2, 0.005);
						if(difficulty>1){
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.8, -3*M_PI/4, 0.003);
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-2.1, M_PI, 0.001);
						}
					}
					else{
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.3, M_PI/2, -0.013);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.6, M_PI/4, -0.011);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.9, 0, -0.009);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.2, -M_PI/4, -0.007);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.5, -M_PI/2, -0.005);
						if(difficulty>1){
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.8, -3*M_PI/4, -0.003);
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-2.1, M_PI, -0.001);
						}
					}
					b->state2=5;
					break;
				case 5:
					if(s->x>WIDTH2/2)
						b->move_angle=-3*M_PI/4;
					else
						b->move_angle=-M_PI/4;
					b->state2=6;
					b->wait1=5;
					s->aktframe=5;
					break;
				case 6:
					if(b->wait1<0){
						b->wait1=10;
						b->state2=7;
						s->aktframe=6;
					}
					else{
						if(s->y>60){
							s->x+=cos(b->move_angle)*2*fps_factor;
							s->y+=sin(b->move_angle)*2*fps_factor;
						}
						b->wait1-=fps_factor;
					}
					break;
				case 7:
					enemy_boss04_waitstate(s, 8, 1);
					break;
				case 8:
					if((int)b->wait2%10==0){
						s->aktframe=5;
						playChunk(11);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.3, M_PI/2, 0.013);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.6, M_PI/4, 0.011);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.9, 0, 0.009);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.2, -M_PI/4, 0.007);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.5, -M_PI/2, 0.005);
						if(difficulty>1){
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.8, -3*M_PI/4, 0.003);
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-2.1, M_PI, 0.001);
						}
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.3, M_PI/2, -0.013);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.6, M_PI/4, -0.011);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-0.9, 0, -0.009);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.2, -M_PI/4, -0.007);
						enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.5, -M_PI/2, -0.005);
						if(difficulty>1){
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-1.8, -3*M_PI/4, -0.003);
							enemy_boss04_more_angle(s, 2.5+b->wait2/10-2.1, M_PI, -0.001);
						}
					}
					b->state2=1;
					break;
			}
			enemy_boss04_out(s);
			break;

		case 2:		//��O�`��:�i�C�t�������Ă����
			switch(b->state2)
			{
			case 0:
				if(b->wait1<0){		//�ҋ@���Ԃ��I�������
					b->state2=1;
					pd->bossmode=1;
					b->move_angle=atan2(-s->y,-s->x);
					s->aktframe=0;
				}else{
					b->wait1-=fps_factor;
				}
				break;
			case 1:
				if(s->x<5 || s->y<5){		//�����ɗ�����
					b->state2=2;
					b->wait1=10;
					b->wait2=0;
					b->move_angle=atan2(50-s->y,WIDTH2-s->w-s->x);
					s->aktframe=1;
				}
				else{
					s->x+=cos(b->move_angle)*3*fps_factor;
					s->y+=sin(b->move_angle)*3*fps_factor;
				}
				break;
			case 2:
				enemy_boss04_waitstate(s, 3, 2);
				break;
			case 3:
				if(s->x>WIDTH2-s->w+3){		//�E�[�ɗ�����
					b->state2=4;
					b->move_angle=atan2(50-s->y,WIDTH2/2-s->w/2-s->x);
					b->wait2=0;
					s->aktframe=0;
				}
				else{
					b->wait2+=3;
					if((int)b->wait2%15==0){
						enemy_fall_knife_create(s, 2, degtorad(360-(int)b->wait2%180), 0.03);
						if(difficulty){
							enemy_fall_knife_create(s, 2, degtorad(270-(int)b->wait2%180), 0.04);
							if(difficulty>1)
								enemy_fall_knife_create(s, 2, degtorad(180-(int)b->wait2%180), 0.02);
						}
						playChunk(12);
					}
					s->x+=cos(b->move_angle)*5*fps_factor;
					s->y+=sin(b->move_angle)*5*fps_factor;
				}
				break;
			case 4:
				if(s->x<WIDTH2/2-s->w/2){		//�^�񒆂ɗ�����
					b->state2=5;
					b->move_angle=atan2(-s->y,WIDTH2-s->w-s->x);
					s->aktframe=2;
				}
				else{
					s->x+=cos(b->move_angle)*3*fps_factor;
					s->y+=sin(b->move_angle)*3*fps_factor;
				}
				break;
			case 5:
				if(s->x>WIDTH2-s->w-3 || s->y<5){		//�E�[�ɗ�����
					b->state2=6;
					b->wait1=10;
					b->wait2=0;
					b->move_angle=atan2(50-s->y,-s->x);
					s->aktframe=0;
				}
				else{
					s->x+=cos(b->move_angle)*3*fps_factor;
					s->y+=sin(b->move_angle)*3*fps_factor;
				}
				break;
			case 6:
				if(s->x<5){		//���[�ɗ�����
					b->state2=7;
					b->move_angle=atan2(50-s->y,WIDTH2/2-s->w/2-s->x);
					b->wait2=0;
					s->aktframe=4;
				}
				else{
					b->wait2+=3;
					if((int)b->wait2%12==0){
						enemy_fall_knife_create(s, 3, degtorad((int)b->wait2%180+180), 0.03);
						if(difficulty){
							enemy_fall_knife_create(s, 3, degtorad((int)b->wait2%180+90), 0.04);
							if(difficulty>1)
								enemy_fall_knife_create(s, 3, degtorad((int)b->wait2%180), 0.02);
						}
						playChunk(12);
					}
					s->x+=cos(b->move_angle)*5*fps_factor;
					s->y+=sin(b->move_angle)*5*fps_factor;
				}
				break;
			case 7:
				if(s->x>WIDTH2/2-s->w/2){		//�^�񒆂ɗ�����
					b->state2=8;
					b->move_angle=atan2(50-s->y,WIDTH2/2-s->w/2-s->x);
					b->wait2=60;
					s->aktframe=4;
				}
				else{
					s->x+=cos(b->move_angle)*3*fps_factor;
					s->y+=sin(b->move_angle)*3*fps_factor;
				}
				break;
			case 8:
				b->wait2--;
				if(b->wait2<0){
					b->state2=1;
					s->aktframe=0;
				}
				else{
					if((int)b->wait2%20==0){
						s->aktframe=(s->aktframe+1)%7;
						enemy_boss04_knifes(s, (int)(b->wait2/20)+4, 30-(int)(b->wait2/20)*40, -80);
						enemy_boss04_knifes(s, (int)(b->wait2/20)+3, 30-(int)(b->wait2/20)*30, -60);
						if(difficulty>1)
							enemy_boss04_knifes(s, (int)(b->wait2/20)+2, 30-(int)(b->wait2/20)*20, -40);
					}
				}
			}
			break;
		case 3:		//��l�`��:
			switch(b->state2)
			{
				case 0:
					if(b->wait1<0){		//�ҋ@���Ԃ��I�������
						b->state2=1;
						pd->bossmode=1;
						b->move_angle=atan2(50-s->y,WIDTH2/2-s->w-s->x);
						b->sp_time+=1000;
					}else{
						b->wait1-=fps_factor;
					}
					break;
				case 1:
					if(s->y>50){		//�^�񒆂ɗ�����
						b->state2=2;
						b->wait1=10;
						b->wait2=0;
						s->aktframe=6;
					}
					else{
						s->x+=cos(b->move_angle)*3*fps_factor;
						s->y+=sin(b->move_angle)*3*fps_factor;
					}
					break;
				case 2:
					enemy_boss04_maho_create(s);		//�����w����
					b->state2=3;
					b->wait2=10;
					b->wait1=5;
					break;
				case 3:
					b->angle=atan2(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
					b->wait2-=fps_factor;
					if(b->wait2<0){
						b->wait2=50;
						enemy_sp1_bullet_create(s, 1.2, b->angle-M_PI/6, 0.03, -1);
						enemy_sp1_bullet_create(s, 1.2, b->angle+M_PI/6, 0.03, 1);
						enemy_sp1_bullet_create(s, 1.5, b->angle-M_PI/3, 0.033, -1);
						enemy_sp1_bullet_create(s, 1.5, b->angle+M_PI/3, 0.033, 1);
						if(difficulty){
							enemy_sp1_bullet_create(s, 1.8, b->angle-M_PI/2, 0.036, -1);
							enemy_sp1_bullet_create(s, 1.8, b->angle+M_PI/2, 0.036, 1);
							if(difficulty>1){
								enemy_sp1_bullet_create(s, 2.1, b->angle-2*M_PI/3, 0.04, -1);
								enemy_sp1_bullet_create(s, 2.1, b->angle+2*M_PI/3, 0.04, 1);
							}
						}
						//enemy_bigbullet_create(s, s->x+s->w/2, s->y+s->h/2, 2, -2, 40);
						b->wait1-=fps_factor;
						if(b->wait1<0){		//3�񌂂�����
							b->wait1=20;
							if(rand()%2==0){
								b->state2=4;
								s->aktframe=0;
							}
							else{
								b->state2=5;
								s->aktframe=2;
							}
						}
					}
					break;
				case 4:
					if(b->wait1<0){
						b->state2=3;
						b->wait2=30;
						b->wait1=3;
						s->aktframe=6;
					}
					else{
						if(s->x<100){
							b->state2=5;
							s->aktframe=2;
						}
						else
							s->x-=2*fps_factor;
					}
					b->wait1-=fps_factor;
					break;
				case 5:
					if(b->wait1<0){
						b->state2=3;
						b->wait2=30;
						b->wait1=3;
						s->aktframe=6;
					}
					else{
						if(s->x>WIDTH2-100-s->w){
							b->state2=4;
							s->aktframe=0;
						}
						else
							s->x+=2*fps_factor;
					}
					b->wait1-=fps_factor;
					break;
			}
			break;

		case 4:		//��܌`��:
			switch(b->state2)
			{
				case 0:
					if(b->wait1<0){		//�ҋ@���Ԃ��I�������
						b->state2=1;
						pd->bossmode=1;
						b->move_angle=atan2(30-s->y,WIDTH2/2-s->w/2-s->x);
					}else{
						b->wait1-=fps_factor;
					}
					break;
				case 1:
					if((s->x<WIDTH2/2+10 && s->x>WIDTH2/2-30)||s->y<50){		//�^�񒆂ɗ�����
						b->state2=2;
						b->wait1=20;
						b->wait2=0;
						s->aktframe=4;
					}
					else{
						s->x+=cos(b->move_angle)*3*fps_factor;
						s->y+=sin(b->move_angle)*3*fps_factor;
					}
					break;
				case 2:
					if(b->wait1<0){		//�ҋ@���Ԃ��I�������
						b->state2=3;
						s->aktframe=5;
					}
					else{
						b->wait1-=fps_factor;
					}
					break;
				case 3:
					b->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
					enemy_even_knife_create(s, 10, b->length_s_p, 0);
					enemy_even_knife_create(s, 10, b->length_s_p, 1);
					b->wait1=40;
					if(s->y>150)
						b->move_angle=degtorad(210+rand()%120);
					else
						b->move_angle=degtorad(rand()%360);
					s->aktframe=6;
					b->state2=4;
					break;
				case 4:
					if(b->wait1<0){
						b->state2=2;
						b->wait1=120+(3-difficulty)*50;
					}
					else{
						s->x+=cos(b->move_angle)*3.5*fps_factor;
						s->y+=sin(b->move_angle)*3.5*fps_factor;
						b->wait1-=fps_factor;
					}
					break;
			}
			if((player->x+player->w/2<s->x+s->w/2+25)&&(player->x+player->w/2>s->x+s->w/2-25))
				b->wait2+=fps_factor;
			if(b->wait2>20+(3-difficulty)*40){
				s->aktframe=5;
				b->length_s_p=sqrt((player->x-s->x)*(player->x-s->x)+(player->y-s->y)*(player->y-s->y));
				if(b->length_s_p>80)
					b->length_s_p=80;
				enemy_boss04_knifes3(s, 1+difficulty, b->length_s_p);
			}
			enemy_boss04_out(s);
			break;

		case 5:
			if(b->wait2<0){
				b->wait2=0;
				b->wait1=120;
				b->state1=4;
				b->state2=2;
				s->aktframe=1;
			}
			else{
				b->wait2-=0.02;
				if(s->alpha>255)
					s->alpha=255;
				else if(s->alpha<255){
					s->alpha+=5;
				}
				s->x+=cos(b->angle)*b->wait2*fps_factor;
				s->y+=sin(b->angle)*b->wait2*fps_factor;
			}
			enemy_boss04_out(s);
			break;
	
	}
	if((b->sp_time<0) && (b->state1<4))
	{
		playChunk(7);
		pd->bossmode=4;
		b->b.health=(4-b->state1)*1000-1;
		b->state1++;
		b->state2=0;
		b->wait1=120;
		b->wait2=0;
		b->sp_time=3500;
	}
}

void enemy_boss04_maho_create(SPRITE *s)
{
	SPRITE *s2; //����
	BOSS04_MAHO_DATA *b;
	
	s2=sprite_add_file("boss04-lo.png",2,PR_GROUNDER);
	s2->flags|=SP_FLAG_VISIBLE|SP_FLAG_COLCHECK;
	s2->anim_speed=0;
	s2->aktframe=0;
	s2->type=SP_PLAYER2;
	b=mmalloc(sizeof(BOSS04_MAHO_DATA));
	s2->data=b;
	b->state1=0;
	b->state2=0;
	b->wait=0;
	b->angle=0;
	b->c=s;
	b->n=0;
	s2->alpha=0;
	s2->mover=enemy_boss04_maho_move;
	s2->x=s->x+s->w/2-s2->w/2;
	s2->y=s->y+s->h/2-s2->h/2;
}

void enemy_boss04_maho_move(SPRITE *s)
{
	BOSS04_MAHO_DATA *d=(BOSS04_MAHO_DATA *)s->data;
	if(d->state1<5){
		s->x=d->c->x+d->c->w/2-s->w/2;
		s->y=d->c->y+d->c->h/2-s->h/2;
		if(((BOSS04_DATA *)d->c->data)->state1==4)
			d->state1=5;
	}
	switch(d->state1)
	{
		case 0:
			if(s->alpha==150){
				d->state1=1;
				d->wait=50;
			}
			else if(s->alpha<150)
				s->alpha+=3;
			else
				s->alpha=150;
			break;
		case 1:
			if(d->wait<0){
				d->state1=2;
				d->state2=100;
			}
			else
				d->wait-=fps_factor;
			break;
		case 2:
			if(d->state2%10==0){
				enemy_stop_bullet2_create(s, d->state2/10, d->angle, 0.02, d->angle-M_PI+d->state2/100);
				enemy_stop_bullet2_create(s, d->state2/10, d->angle+2*M_PI/3, 0.02, d->angle-M_PI+d->state2/100+2*M_PI/3);
				enemy_stop_bullet2_create(s, d->state2/10, d->angle-2*M_PI/3, 0.02, d->angle-M_PI+d->state2/100-2*M_PI/3);
			}
			if(d->state2<0){
				d->state1=3;
				d->wait=15;
				d->state2=11;
			}
			d->state2--;
			break;
		case 3:
			d->wait-=fps_factor;
			if(d->wait<0){
				playChunk(13);
				enemy_laser_create2(s, 8, d->angle);
				enemy_laser_create2(s, 5, d->angle);
				enemy_laser_create2(s, 6, d->angle);
				enemy_laser_create2(s, 7, d->angle);
				enemy_laser_create2(s, 8, d->angle+2*M_PI/3);
				enemy_laser_create2(s, 5, d->angle+2*M_PI/3);
				enemy_laser_create2(s, 6, d->angle+2*M_PI/3);
				enemy_laser_create2(s, 7, d->angle+2*M_PI/3);
				enemy_laser_create2(s, 8, d->angle-2*M_PI/3);
				enemy_laser_create2(s, 5, d->angle-2*M_PI/3);
				enemy_laser_create2(s, 6, d->angle-2*M_PI/3);
				enemy_laser_create2(s, 7, d->angle-2*M_PI/3);
				d->wait=12;
				d->angle+=0.15;
				d->state2--;
			}
			if(d->state2<0){
				d->state1=2;
				d->state2=50;
			}
			break;
		
		case 5:		//���^��`����
		/*
			190, 48
			248, 217
			100, 113
			280, 113
			138, 217
		*/
			s->aktframe=0;
			if(((BOSS04_DATA *)d->c->data)->state2==1){
				d->angle=atan2(48-s->y-s->h/2, 190-s->x-s->w/2);
				d->state1=6;
				d->state2=0;
			}
			break;
		case 6:
			if(s->y+s->h/2<=48){
				d->state1=7;
				d->angle=atan2(217-s->y-s->h/2, 248-s->x-s->w/2);
				d->wait=50;
				d->n++;
				d->state2=0;
				if(d->n%(4-difficulty)==1)
					s->aktframe=1;
				else
					s->aktframe=0;
			}
			else{
				s->x+=cos(d->angle)*(3+difficulty)*fps_factor;
				s->y+=sin(d->angle)*(3+difficulty)*fps_factor;
				d->state2+=3;
				if(d->n%(4-difficulty)==0 && d->state2%5==0){
					enemy_sp2_bullet_create(s, 3+(-d->state2*2*d->n%2+d->state2)/5, 0.015);
				}
			}
			break;
		case 7:
			enemy_boss04_knifes2(s, 1.2);
			d->state1=8;
			break;
		case 8:
			if(d->wait<0)
				d->state1=9;
			else
				d->wait--;
			break;
		
		case 9:
			if(s->y+s->h/2>=217){
				d->state1=10;
				d->angle=atan2(113-s->y-s->h/2, 100-s->x-s->w/2);
				d->wait=50;
				d->state2=0;
			}
			else{
				s->x+=cos(d->angle)*(3+difficulty)*fps_factor;
				s->y+=sin(d->angle)*(3+difficulty)*fps_factor;
				d->state2+=3;
				if(d->n%(4-difficulty)==0 && d->state2%5==0){
					enemy_sp2_bullet_create(s, 147+(-d->state2*2*d->n%2+d->state2)/5, 0.015);
				}
			}
			break;
		case 10:
			enemy_boss04_knifes2(s, 1.2);
			d->state1=11;
			break;
		case 11:
			if(d->wait<0)
				d->state1=12;
			else
				d->wait--;
			break;
		
		case 12:
			if(s->y+s->h/2<=113){
				d->state1=13;
				d->angle=atan2(113-s->y-s->h/2, 280-s->x-s->w/2);
				d->wait=50;
				d->state2=0;
			}
			else{
				s->x+=cos(d->angle)*(3+difficulty)*fps_factor;
				s->y+=sin(d->angle)*(3+difficulty)*fps_factor;
				d->state2+=3;
				if(d->n%(4-difficulty)==0 && d->state2%5==0){
					enemy_sp2_bullet_create(s, 291+(-d->state2*2*d->n%2+d->state2)/5, 0.015);
				}
			}
			break;
		case 13:
			enemy_boss04_knifes2(s, 1.2);
			d->state1=14;
			break;
		case 14:
			if(d->wait<0)
				d->state1=15;
			else
				d->wait--;
			break;
		
		case 15:
			if(s->x+s->w/2>=280){
				d->state1=16;
				d->angle=atan2(217-s->y-s->h/2, 138-s->x-s->w/2);
				d->wait=50;
				d->state2=0;
			}
			else{
				s->x+=cos(d->angle)*(3+difficulty)*fps_factor;
				s->y+=sin(d->angle)*(3+difficulty)*fps_factor;
				d->state2+=3;
				if(d->n%(4-difficulty)==0 && d->state2%5==0){
					enemy_sp2_bullet_create(s, 75+(-d->state2*2*d->n%2+d->state2)/5, 0.015);
				}
			}
			break;
		case 16:
			enemy_boss04_knifes2(s, 1.2);
			d->state1=17;
			break;
		case 17:
			if(d->wait<0)
				d->state1=18;
			else
				d->wait--;
			break;
		
		case 18:
			if(s->y+s->h/2>=217){
				d->state1=19;
				d->angle=atan2(48-s->y-s->h/2, 190-s->x-s->w/2);
				d->wait=50;
				d->state2=0;
			}
			else{
				s->x+=cos(d->angle)*(3+difficulty)*fps_factor;
				s->y+=sin(d->angle)*(3+difficulty)*fps_factor;
				d->state2+=3;
				if(d->n%(4-difficulty)==0 && d->state2%5==0){
					enemy_sp2_bullet_create(s, 219+(-d->state2*2*d->n%2+d->state2)/5, 0.015);
				}
			}
			break;
		case 19:
			enemy_boss04_knifes2(s, 1.2);
			d->state1=20;
			break;
		case 20:
			if(d->wait<0){
				d->state1=6;
				d->state2=0;
			}
			else
				d->wait--;
			break;
	}
	if(d->c->type==-1)
		s->type=-1;
}

void enemy_boss04_waitstate(SPRITE *s, int nextstate, int aktframe)
{
	BOSS04_DATA *b=(BOSS04_DATA *)s->data;

	if(b->wait1>0)
		b->wait1-=fps_factor;
	else{
		b->state2=nextstate;
		s->aktframe=aktframe;
	}
}

void enemy_boss04_out(SPRITE *s)
{
	if(s->x<50)
		s->x=50;
	if(s->x>WIDTH2-s->w-50)
		s->x=WIDTH2-s->w-50;
	if(s->y<10)
		s->y=10;
	if(s->y>HEIGHT+s->h-80)
		s->y=HEIGHT+s->h-80;
}

void enemy_boss04_more_angle(SPRITE *s, double speed, double angle, double a)
{
	enemy_angle_bullet_create(s, speed, angle, a);
	enemy_angle_bullet_create(s, speed, angle-M_PI/6, a);
	enemy_angle_bullet_create(s, speed, angle-M_PI/3, a);
	enemy_angle_bullet_create(s, speed, angle-M_PI/2, a);
	enemy_angle_bullet_create(s, speed, angle-2*M_PI/3, a);
	enemy_angle_bullet_create(s, speed, angle-5*M_PI/6, a);
	enemy_angle_bullet_create(s, speed, angle-M_PI, a);
	enemy_angle_bullet_create(s, speed, angle+5*M_PI/6, a);
	enemy_angle_bullet_create(s, speed, angle+2*M_PI/3, a);
}

void enemy_boss04_knifes(SPRITE *s, double speed, int angle, int height)
{
	//�A���O���͊p�x�w���-90�`90
	//height��"player����"�̍���
	enemy_follow_knife_create(s, speed, degtorad(180-angle), height);
	enemy_follow_knife_create(s, speed, degtorad(angle), height);
}

void enemy_boss04_knifes2(SPRITE *s, double speed)
{
	enemy_knife_create(s, speed, 0, 0);
	enemy_knife_create(s, speed, 0.349, 17);
	enemy_knife_create(s, speed, 0.698, 16);
	enemy_knife_create(s, speed, 1.047, 15);
	enemy_knife_create(s, speed, 1.396, 14);
	enemy_knife_create(s, speed, 1.745, 13);
	enemy_knife_create(s, speed, 2.094, 12);
	enemy_knife_create(s, speed, 2.443, 11);
	enemy_knife_create(s, speed, 2.792, 10);
	enemy_knife_create(s, speed, 3.141, 9);
	enemy_knife_create(s, speed, -2.792, 8);
	enemy_knife_create(s, speed, -2.443, 7);
	enemy_knife_create(s, speed, -2.094, 6);
	enemy_knife_create(s, speed, -1.745, 5);
	enemy_knife_create(s, speed, -1.396, 4);
	enemy_knife_create(s, speed, -1.047, 3);
	enemy_knife_create(s, speed, -0.698, 2);
	enemy_knife_create(s, speed, -0.349, 1);
}

void enemy_boss04_knifes3(SPRITE *s, double speed, double length){
	BOSS04_DATA *b=(BOSS04_DATA *)s->data;
	double len;
	double tmp1;
	double tmp2;
	double tmp_x;
	double tmp_y;
	int tmp;
	int i;
	len=70-length/2;
	b->wait2=0;
	while(1){
		tmp=(rand()+(int)b->wait2)%8;
		b->wait2+=1;
		len+=0.1;
		tmp1=player->x+player->w/2-s->w/2+cos(tmp*2*M_PI/8)*len;
		tmp2=player->y+player->h/2-s->h/2+sin(tmp*2*M_PI/8)*len;
		if((30<tmp1 && tmp1<(WIDTH2-30))&&(30<tmp2 && tmp2<200))
			break;
	}
	tmp_x=player->x+player->w/2;
	tmp_y=player->y+player->h/2;
	for(i=0;i<8;i++){
		if(tmp!=i){
			enemy_follow_knife_create2(tmp_x+cos(i*M_PI/4)*len, tmp_y+sin(i*M_PI/4)*len, speed, i*M_PI/4, -30+(i%2)*60);
		}
		else{
			b->angle=i*M_PI/4;
			s->x=tmp1;
			s->y=tmp2;
			s->alpha=0;
			b->state1=5;
			b->wait2=speed;
		}
	}
}