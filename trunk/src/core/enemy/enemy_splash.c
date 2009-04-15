#include "enemy.h"

typedef struct
{
	ENEMY_BASE b;
	int state;
	int level;
	int wait;
	int ani_turn;
	int n;
} SPLASH_DATA;

static void enemy_splash_move(SPRITE *s)
{
	SPLASH_DATA *d=(SPLASH_DATA *)s->data;
	/*double*/int p512;
	int i2;

	switch (d->state)
	{
	case 0: 	//“oê
		if (d->wait<40)
		{
				 if (d->level<3)	{	s->x-=2*fps_factor; 	}
			else if (d->level<7)	{	s->y+=2*fps_factor; 	}
			else					{	s->x+=2*fps_factor; 	}
			d->wait++;
		}
		else
		{
			d->state=1;
			d->wait=10;
		}
		break;
	case 1: 	//wait
		if (d->wait<0)
		{	d->state=2;}
		else
		{
			p512=atan_512(player->y-s->y,player->x-s->x);
			if (	((p512>= rad2deg512(M_PI*2*1/16))&&(p512< rad2deg512(M_PI*2*7/16)))||
					((p512>=-rad2deg512(M_PI*2*5/16))&&(p512<-rad2deg512(M_PI*2*3/16))))	s->aktframe=(s->aktframe&(4-1))+ 8;
			else if ((p512>=-rad2deg512(M_PI*2*1/16))&&(p512< rad2deg512(M_PI*2*1/16))) 	s->aktframe=(s->aktframe&(4-1))+16;
			else if ((p512>=-rad2deg512(M_PI*2*3/16))&&(p512<-rad2deg512(M_PI*2*1/16))) 	s->aktframe=(s->aktframe&(4-1))+12;
			else if ((p512>=-rad2deg512(M_PI*2*7/16))&&(p512<-rad2deg512(M_PI*2*5/16))) 	s->aktframe=(s->aktframe&(4-1))+ 4;
			else if ((p512>= rad2deg512(M_PI*2*7/16))||(p512<-rad2deg512(M_PI*2*7/16))) 	s->aktframe=(s->aktframe&(4-1));
			d->wait--;
		}
		break;
	case 2: 	//shot
		d->n++;
		d->wait=50;
		d->state=1;
		bullet_create_haridan180(s, ANGLE_JIKINERAI_DAN, t256(3), 0, 0);
		bullet_create_haridan180(s, ANGLE_JIKINERAI_DAN, t256(4), 0, 0);

		p512=atan_512(player->y+player->h/2-s->y,player->x-player->w/2-s->x);
		SPRITE *h;
		BULLET_DATA *data;
		if (difficulty>0)
		{
			for (i2=0;i2<4;i2++)
			{		/*ŠÛ’e‚W(Ô‚q‚f‚a—ÎŽá‰©Â)*/
				h=NULL;
			//	h				=sprite_add_file("bs hoot.png",1,PR_BULLETS/*PR_ENEMY*/);
				h				=sprite_add_file("bullet_maru8.png",8,PR_BULLETS/*PR_ENEMY*/);
				h->type 		=SP_EN_LASER;
				h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
				h->mover		=enemy_laser_move;
				h->aktframe 	=/*0*/7-i2;/*‚Æ‚è‚ ‚¦‚¸*/
				h->x			=s->x+(s->w/2-h->w/2);
				h->y			=s->y+(s->h/2-h->h/2);
				data			=NULL;
				data			=mmalloc(sizeof(BULLET_DATA));
				h->data 		=data;
				data->angle512	=p512+rad2deg512((1-((i2&(2-1)/*%2*/))*2)*M_PI/6);
				data->speed 	=3+i2/2;
			}
			if (difficulty>1)
			{
				for (i2=0;i2<4;i2++)
				{		/*ŠÛ’e‚W(Ô‚q‚f‚a—ÎŽá‰©Â)*/
					h				=NULL;
				//	h				=sprite_add_file("bs hoot.png",1,PR_BULLETS/*PR_ENEMY*/);
					h				=sprite_add_file("bullet_maru8.png",8,PR_BULLETS/*PR_ENEMY*/);
					h->type 		=SP_EN_LASER;
					h->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
					h->mover		=enemy_laser_move;
					h->aktframe 	=/*0*/3-i2;/*‚Æ‚è‚ ‚¦‚¸*/
					h->x			=s->x+(s->w/2-h->w/2);
					h->y			=s->y+(s->h/2-h->h/2);
					data			=NULL;
					data			=mmalloc(sizeof(BULLET_DATA));
					h->data 		=data;
					data->angle512	= (/*rad2deg512*/(p512))+(1-i2%2*2)*(rad2deg512(M_PI/12));
					data->speed 	=2+i2/2;
				}
			}
		}
		if (d->n>3)
		{	d->state=3;}
		break;
	case 3: 	//‘Þê€”õ
		if (d->level<3) 		{	s->aktframe=(s->aktframe&(4-1))+16;}
		else if (d->level<7)	{	s->aktframe=(s->aktframe&(4-1))+ 8;}
		else					{	s->aktframe=(s->aktframe&(4-1))+16;}
		d->state=4;
		d->wait=0;
		break;
	case 4: 	//‘Þê
		if (d->wait>50)
		{
			s->type=SP_DELETE;
		}
		else
		{
			if (d->level<3) 		{	s->x+=2*fps_factor;}
			else if (d->level<7)	{	s->y-=2*fps_factor;}
			else					{	s->x-=2*fps_factor;}
		}
		break;
	}

	if (!d->ani_turn)
	{
		if ((s->aktframe&(4-1))==3)
		{
			d->ani_turn=1;
			s->aktframe--;
		}
		s->aktframe++;
	}
	else
	{
		if ((s->aktframe&(4-1))==0)
		{
			d->ani_turn=0;
			s->aktframe++;
		}
		s->aktframe--;
	}
}

void enemy_splash_add(int lv)
{
	if ( (lv)>9) {lv=9;}
	SPRITE *s;
	s				= sprite_add_file("splash.png",20,PR_ENEMY);	s->anim_speed=0;
	s->type 		= SP_EN_SPLASH;
	s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->mover		= enemy_splash_move;

	short spr_tbl[10][4] =
	{
{/* case 0: 	s->x=*/GAME_WIDTH+20,	1,/*-s->w;*/		/*s->y=*/100,		/*s->aktframe=*/0}, 	//‰E‰º
{/* case 1: 	s->x=*/GAME_WIDTH+40,	1,/*-s->w;*/		/*s->y=*/70,		/*s->aktframe=*/0}, 	//‰E’†
{/* case 2: 	s->x=*/GAME_WIDTH+60,	1,/*-s->w;*/		/*s->y=*/40,		/*s->aktframe=*/0}, 	//‰Eã
{/* case 3: 	s->x=*/300, 		3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->aktframe=*/8}, 	//ã‰E‰E
{/* case 4: 	s->x=*/220, 		3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->aktframe=*/8}, 	//ã‰E
{/* case 5: 	s->x=*/160, 		3,/*-s->w/2;*/		/*s->y=*/-50,		/*s->aktframe=*/8}, 	//ã¶
{/* case 6: 	s->x=*/ 80, 		3,/*-s->w/2;*/		/*s->y=*/-30,		/*s->aktframe=*/8}, 	//ã¶¶
{/* case 7: 	s->x=*/-20, 		0,/*		*/		/*s->y=*/40,		/*s->aktframe=*/16},	//¶ã
{/* case 8: 	s->x=*/-40, 		0,/*		*/		/*s->y=*/70,		/*s->aktframe=*/16},	//¶’†
{/* case 9: 	s->x=*/-60, 		0,/*		*/		/*s->y=*/100,		/*s->aktframe=*/16},	//¶‰º
	};
	s->aktframe 	=spr_tbl[lv][3];
	s->y			=spr_tbl[lv][2];
	s->x			=spr_tbl[lv][0];
		 if (1==spr_tbl[lv][1]) {s->x -= ((s->w)   );}
	else if (3==spr_tbl[lv][1]) {s->x -= ((s->w)>>1);}

	SPLASH_DATA *data;
	data			= mmalloc(sizeof(SPLASH_DATA));
	s->data 		= data;
	data->b.score	= score(50*2);
	data->b.health	= 20+(difficulty<<2);
	data->state 	= 0;
	data->level 	= lv;
	data->wait		= 0;
	data->ani_turn	= 0;
	data->n 		= 0;
}
