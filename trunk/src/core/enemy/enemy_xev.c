#include "enemy.h"

extern SPRITE *player;
extern double fps_factor;

typedef struct
{
	ENEMY_BASE b;
	int state;
	int tx;
	int ty;
	int level;
} XEV_DATA;

static void enemy_xev_move(SPRITE *s)
{
	double speed=0;
	XEV_DATA *d=(XEV_DATA *)s->data;

	switch (d->state)
	{
	case 0:
		speed=3;
		if ((s->x >= player->x)||(s->y>400))
		{
			d->state		=1;
			s->anim_speed	=/*-*/1;	/*逆転アニメ禁止に変更*/
			d->tx			=-100;
			d->ty			=player->y;
			if (d->level)
			{	enemy_bullet_create(s,1+d->level);}
			{
				/* [***20090223 追加 テーブルにしたよ */
				static const Uint16/*int*/ item_table[10]=
				{
					SP_BONUS_00_FIREPOWER/*0x1000*/,
					SP_BONUS_02_BOMB/*0x1003*/,
					SP_BONUS_00_FIREPOWER/*0x1000*/,
					SP_BONUS_07_ADDSPEED/*0x1002*/,

					SP_BONUS_00_FIREPOWER/*0x1000*/,
					SP_BONUS_01_FIREPOWER_G/*0x1007*/,
					SP_BONUS_02_BOMB/*0x1003*/,
					SP_BONUS_07_ADDSPEED/*0x1002*/,

					SP_BONUS_06_COIN/*0x1001*/,
					SP_BONUS_03_HEALTH/*0x1005*/
				};
				static unsigned int drop_item=0;
				bonus_multi_add(s, item_table[drop_item], 1, BONUS_FLAG_RAND_XY);
				drop_item++;
				if (drop_item>9)
				{	drop_item=0;}
			}
		}
		break;/* ??? [***20090210	追加  */
	case 1:
		speed=6;
		if (s->x <= -s->w)
		{
			s->type=SP_DELETE;
		}
		break;/* ??? [***20090409	追加  */
	}
	{	/*double*/int angle512;
		angle512=atan_512(d->ty-s->y,d->tx-s->x);
		s->x+=co_s512(angle512)*speed*fps_factor;
		s->y+=si_n512(angle512)*speed*fps_factor;
	}
}

void enemy_xev_add(int lv)
{
	int i;
	for (i=0;i<3;i++)
	{
		SPRITE *s;
		s				=sprite_add_file("crusher.png",15,PR_ENEMY);		s->anim_speed=1;
		s->type 		=SP_EN_XEV;
		s->flags		|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		s->mover		=enemy_xev_move;
		s->aktframe 	=0;
		switch (rand()%3)
		{
		case 0: s->x=rand()%20; break;
		case 1: s->x=GAME_WIDTH-rand()%20; break;		//ウィンドウ幅の変更
		case 2: s->x=(GAME_WIDTH/2)+10+rand()%20; break;		//ウィンドウ幅の変更
		}
		s->y			=rand()%20-50;
		XEV_DATA *data;
		data			=mmalloc(sizeof(XEV_DATA));
		s->data 		=data;
		data->b.score	=score(5*2);
		data->b.health	=1+(difficulty<<2);
		data->state 	=0;
		data->tx		=player->x;
		data->ty		=player->y;
		data->level 	=lv;
	}
}
