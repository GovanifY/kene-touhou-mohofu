#include "bonus.h"

extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;		//[***090116		追加

static SPRITE *bonus_sel(int x, int y, int type)
{
	SPRITE *s;
	switch (type)
	{
	case SP_BONUS_FIREPOWER:	s=sprite_add_file("bonus_p.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090123		変更
	case SP_BONUS_ADDSPEED: 	s=sprite_add_file("bonus_s.png",16,PR_BONUS);	s->anim_speed=2;	break;
	case SP_BONUS_BOMB: 		s=sprite_add_file("bonus_f.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090123		変更
	case SP_BONUS_COIN: 		s=sprite_add_file("coin.png",1,PR_BONUS);		s->anim_speed=0;	break;		//[***090123		変更
	case SP_BONUS_EXTRA:		s=sprite_add_file("bonus_x.png",16,PR_BONUS);	s->anim_speed=3;	break;
	case SP_BONUS_HEALTH:		s=sprite_add_file("bonus_h.png",9,PR_BONUS);	s->anim_speed=2;	break;
	case SP_BONUS_FIREPOWER_G:	s=sprite_add_file("bonus_p_.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090124		追加
	default:
		CHECKPOINT;
		error(ERR_WARN,"cant add unknown bonus! debug me! please! now!");
		return NULL;
	}
	#if 0
	if (x </*WIDTH2-20*/600/*600*/) x += 20;		/*600は元のPC版KETMの横幅修正忘れ(640-40==600)*/
	else							x -= 10;
	#else
	//x += 20;		/*重なると見にくいので横に20dotずらす*/
	//if ( WIDTH2 < x ) {	x = ((WIDTH2 - x) & (512-1));	}	/* はみ出たら折り返す、ついでにマスク */
	x += 16;		/*重なると見にくいので横に20dotずらす*/
	if ( WIDTH2 < x ) { x = (WIDTH2-256)+((x) & (256-1));	}	/* はみ出たら、なんとなく右側の方にする */
	#endif
	//	#if 0
	//	if (/*s->*/x < /*WIDTH2-20*/480/*480*/) x += 20;
	//	else							x -= 10;
	//	#else
	//	x += 20;		/*重なると見にくいので横に20dotずらす*/
	//	if ( WIDTH2 < x ) { x = (WIDTH2 - x);	}	/* はみ出たら折り返す */
	//	#endif
/* ここでxが異常な値になる場合があるので、ここ以前にマスクしておく */
	s->x = x;
	s->y = y;
	s->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	s->type=type;
	s->mover=bonus_move;
	return (s);
}


void bonus_add(int x, int y, int type, int up)
{
	SPRITE *s;
	s=bonus_sel( x, y, type);
	if (NULL==s) return;
//
	BONUS_DATA *data;
	data=mmalloc(sizeof(BONUS_DATA));
	s->data=data;
	data->sum=-2;
	data->gra=0.08;
	data->pl_up=up;
}

void bonus_multi_add(int x, int y, int type, int num, int up)	//[***090125		追加:up 1でプレイヤーに集まります。0で普通の状態。
{
	int i;
	for (i=0;i<num;i++)
	{
		SPRITE *s;
		s=bonus_sel(x + (rand()%100-50), y + (rand()%60-30), type);
		if (NULL==s) return;
//
		BONUS_DATA *data;
		data=mmalloc(sizeof(BONUS_DATA));
		s->data=data;
		data->sum=-5;
		data->gra=0.08;
		data->pl_up=up; 	//ちょっと変更
	}
}


void bonus_move(SPRITE *s)
{
	BONUS_DATA *d=(BONUS_DATA *)s->data;
	if (((PLAYER_DATA *)player->data)->bonus)
		d->pl_up=1;
	if (!d->pl_up)
	{
		if (d->sum<4)		//[***090123		変更5=>4
			d->sum+=d->gra;
		s->y+=d->sum*fps_factor;
		if (s->y>HEIGHT) s->type=-1; // denis 480
	}
	else
	{
		d->sum-=5;
		if (d->sum<0)
		{
			d->gra=atan2(player->y-s->y,player->x-s->x);
			d->sum=14;
		}
		s->x+=cos(d->gra)*12*fps_factor;
		s->y+=sin(d->gra)*12*fps_factor;
	}
}

void bonus_info_add(int x, int y, char *filename)
{
	SPRITE *c;
	c			= sprite_add_file(filename,1,PR_BONUS);
	c->flags	= SP_FLAG_VISIBLE;
	c->type		= SP_ETC;
	c->x		= x;
	c->y		= y;
	c->alpha	= 255;
	c->mover	= bonus_info_move;
}

void bonus_info_move(SPRITE *c)
{
	if (c->alpha>=3*fps_factor)
	{
		c->alpha-=3*fps_factor;
		c->y-=fps_factor;
	}
	else
	{
		c->type = -1;
	}
}

void bonus_info_s_text(SPRITE *src/*int x, int y*/, char *text, int font)
{
	SDL_Surface *t;
	SPRITE *s;
	t			= font_render(text,font);
	s			= sprite_add(t,1,PR_BONUS,1);
	s->flags	|= SP_FLAG_FREESURFACE|SP_FLAG_VISIBLE;
	s->type 	= SP_ETC;
	s->x		= src->x;
	s->y		= src->y;
	s->alpha	= 255;
	s->data 	= mmalloc(sizeof(BIT_DATA));
	((BIT_DATA *)s->data)->distance=255;
	s->mover	= bonus_infotext_move;
}
void bonus_info_score(SPRITE *src/*int x, int y*/, int score_type)
{
	/*const*/ Uint8 score_tbl[4]=
	{
	/* 0==SCORE_30*/ 	score(30),
	/* 1==SCORE_50*/ 	score(50),
	/* 2==SCORE_100*/	score(100),
	/* 3==*/			score(500),
	};
	((PLAYER_DATA *)player->data)->score += score_tbl[score_type];
//
	/*const*/ char *score_name[4]=
	{
	/* 0==SCORE_30*/ 	"30",
	/* 1==SCORE_50*/ 	"50",
	/* 2==SCORE_100*/	"100",
	/* 3==*/			"500",
	};
	bonus_info_s_text(src/*int x, int y*/, (char *)score_name[(score_type)&(4-1)], FONT07);
}
void bonus_info_any_score(SPRITE *src/*int x, int y*/, int score_num)
{
	((PLAYER_DATA *)player->data)->score += score_num;
//
	char buffer[16];
	sprintf(buffer,"%d0", score_num );
	bonus_info_s_text(src/*int x, int y*/, buffer, FONT07);
}
void bonus_infotext_move(SPRITE *c)
{
	BIT_DATA *b=(BIT_DATA *)c->data;
//	SDL_Surface *s;
	c->y		+= fps_factor*2;
	b->distance -= fps_factor*3;
	c->alpha	= b->distance;
	if (b->distance <= 0)
	{
		//s=sprite_getcurrimg(c);
		//parsys_add(s,1,1,c->x,c->y,30,0,0,100,PIXELIZE,NULL);
		c->type = -1;
	}
}
