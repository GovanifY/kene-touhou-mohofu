#include "player.h"

#define rand_percent(aaa) ((unsigned char)(rand())<=(unsigned char)( (aaa*256)/100 ) ) /* aa%の確率で */

extern SPRITE *sprite;
extern int select_player;


int player_now_stage;		//
SPRITE *player;

enum _player_state
{
	PL_NORMAL,
	PL_HIT_BOMB,		//[***090125		追加:PL_HIT_BOMB
	PL_EXPLODE,
	PL_RESET,
	PL_SAVE
};

typedef struct
{
	int strength;/*union WEAPON_BASE*/
	int angle512;
	double speed;
} PLAYER_BASE_STRUCT;

#define PL_PLASMA_DATA		PLAYER_BASE_STRUCT
#define PL_FIREBALL_DATA	PLAYER_BASE_STRUCT

#if (1==USE_PLAYER_BOMBER)
#define PL_BOMB_DATA	  PLAYER_BASE_STRUCT
#endif //(1==USE_PLAYER_BOMBER)

#if (1==USE_PLAYER_WAVE)
typedef struct
{
	int strength;/*union WEAPON_BASE*/
	int angle512;
	double speed;
	double distance;	/*距離(distance)*/
} PL_KILLRAY_DATA;
#endif //(1==USE_PLAYER_WAVE)

typedef struct
{
	int strength;/*union WEAPON_BASE*/
	/*double*/int angle512;
	double speed;
//
	int nr;
	int state;
	int time_out;	/*double range;*/		/* 設定時間内に見つけられない場合はおしまい */
	SPRITE *target_obj/*int target_id*/;
//	SPRITE *tg_sprite;/*ターゲット目標表示マーカー*/	/*PL_HOMING_DATAのみ*/
} PL_HOMING_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	/*double*/int angle512;
	double speed;
	int radius; 	/*半径(radius)*/
} PL_SHIELD_DATA;

#define PL_LEVARIE_DATA    PLAYER_BASE_STRUCT
#define PL_CROSS_RED_DATA  PLAYER_BASE_STRUCT
#define OZ_MISSILE_DATA    PLAYER_BASE_STRUCT

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	/*double*/int angle512;
	int ani_speed;
	int length;
	int pos;
	int w_wait;
//	int state;
} RE_OPTION_DATA;

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	/*double*/int angle512;
	int ani_speed;
	int state1;
	int state2;
	int w_wait;
	int offset_x;		/* 円の半径と兼用 */
	int offset_y;
	int option_id;
	int slow_count;
	/*double*/int m_angle512;
//	struct _sprite *next;
} OZ_OPTION_DATA;


#define BASE_BOMBS			(4*0)
#define BASE_HIT_BOMB_WAIT	(4*1)
#define BASE_LIVES			(4*2)
#define BASE_SPEED_ANIME	(4*3)
#define BASE_SPEED_MINIMUM	(4*4)
#define BASE_SPEED_MAXIMUM	(4*5)
#define BASE_MAX			(4*6)
static Uint8 player_status[BASE_MAX]=
{/* REIMU MARISA REMIRIA CIRNO */
	 3,  2,  1,  9, 	/* 復活時のボム数 */
	16,  8,  4,  9, 	/* 喰らいボムの受付時間 hit_bomb_wait */
	 3,  4,  2,  9, 	/* 開始時の残りチャンス */
	 2,  2,  2,  1, 	/* アニメーション速度 */
	 3,  4,  4,  9, 	/* 最低速度 player_speed_minimum */
	 6,  8,  7,  9, 	/* 最高速度 player_speed_maximum */
};

static int is_graze;	//player1が弾幕に触れたか？

static void player_move_core(SPRITE *s2)
{
	s2->x=player->x+((player->w-s2->w)/2)-1;
	s2->y=player->y-s2->h+20;
	if (is_graze)	//プレイヤースプライトが弾に触れているか
	{
		if (((PLAYER_DATA *)player->data)->state==PL_NORMAL)
		{
			SPRITE *c;
			if ((c=sprite_colcheck(s2,SP_SHOW_ENEMY_WEAPONS))!=NULL)
			{
				switch (c->type)	//各弾の処理。弾の種類を追加した場合。ここにも追加する必要あり。
				{
				case SP_EN_BULLET:				/*	not_break;*/
				case SP_EN_LASER:				/*	not_break;*/
				case SP_EN_BIGBULLET:
					playChunk(4);		//[***090127	変更先
					((PLAYER_DATA *)player->data)->state=PL_HIT_BOMB;
					((PLAYER_DATA *)player->data)->enemy=c;
					break;
				}
			}
		}
	}
	is_graze=0;
}


static void weapon_colcheck(SPRITE *s, /*int angle,*/ int destroy, int check_bullets)
{
	//s 	主人公の弾
	SPRITE *c;		//敵スプライト
//	SDL_Surface *spimg;
	ENEMY_BASE *d;
	WEAPON_BASE *b=(WEAPON_BASE *)s->data;
//	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;

	/* wenn flag check_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	if (check_bullets)
	{
		if ((c=sprite_colcheck(s,SP_SHOW_ENEMY_WEAPONS))!=NULL) 	//敵の弾なのか？
		{
			playChunk(2);
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
			c->type=SP_DELETE;
		}
	}

	if ((c=sprite_colcheck(s,SP_SHOW_ENEMYS))!=NULL)		//敵そのものなのか？
	{
	//	if (s->type==SP_PL_HOMING)
	//	{
	//		/* Gegner von Homing-Missile getroffen? */
	//		/* Paranoia-Flag und Target-Fadenkreuz entfernen */
	//		/* ホーミング弾が当たったと同時に、ターゲット目標表示マーカーも削除する。 */
	//		PL_HOMING_DATA *h;	h=(PL_HOMING_DATA *)s->data;
	//		h->tg_sprite->type=SP_DELETE;/*ターゲット目標表示マーカーの削除*/
	//	}
		/* wenn flag destroy gesetzt ist, wird der Schuss vernichtet */
		if (destroy)
		{	s->type=SP_DELETE;}

		// ??? why did i do this ???
		// c->flags&=~SP_FLAG_PARANOIA;
		// ???

		switch (c->type)
		{
		//ボスに当たったのがシールドだった場合はダメージ処理を行わない
		#if 0
		case SP_EN_BOSS01:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss01_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_EN_BOSS02:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss02_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_EN_BOSS03:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss03_hitbyweapon(c,s/*,angle*/);	}	break;
		case SP_EN_BOSS04:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss04_hitbyweapon(c,s/*,angle*/);	}	break;	//[***090127		追加
		#else
		case SP_EN_BOSS:	if (s->type!=SP_PL_SHIELD)	{	(((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon)(c,s/*,angle*/);	}	break;
		#endif
		case SP_EN_GFAIRY:	if (s->type!=SP_PL_SHIELD)	{	enemy_nonshield_hitbyweapon(c,s/*,angle*/); 	}	break;

		default:
			d=(ENEMY_BASE *)c->data;
			/* にとりバグわからんので、なさそうな値の場合やらない */
			if (0<b->strength)	/* 正値で */
			{
				if (20>b->strength) /* ２０未満の場合 */
				{
					d->health -= b->strength;	/* 強さ分引く */
				}
			}
			//spimg=sprite_getcurrimg(c);
			if (d->health>0)
			{
			//	playChunk(2);
			//	parsys_add(spimg, 2,2, c->x,c->y, 10, 0, 0, 30, EXPLODE|DIFFSIZE, NULL);
			}
			else
			{
				playChunk(8);
				explosion_add(c->x,c->y+5,0,EXPLOSION_ZAKO04/*EXPLOSION_MINI00*/);/*ザコ消滅爆発*/
				//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
				int put_item;
				int up_flags;
				put_item=99;
				up_flags=(s->type==SP_PL_SHIELD)?(BONUS_FLAG_COLLECT|BONUS_FLAG_RAND_XY):(0);
				switch (c->type)	//[***090123		追加
				{
				case SP_EN_GREETER: 	if (rand_percent(30))	{
											if (rand_percent(50))	{	put_item=(SP_BONUS_00_FIREPOWER&0xff);}
											else					{	put_item=(SP_BONUS_06_COIN&0xff);}
										}	break;
				case SP_EN_MING:		if (rand_percent(30))	{
											if (rand_percent(50))	{	put_item=(SP_BONUS_00_FIREPOWER&0xff);}
											else					{	put_item=(SP_BONUS_06_COIN&0xff);}
										}	break;
				case SP_EN_GROUNDER:	if (rand_percent(30))		{	put_item=(SP_BONUS_00_FIREPOWER&0xff);}
										if (rand_percent(30))		{	bonus_multi_add(c,/*->x+ 5, c->y+5,*/ SP_BONUS_06_COIN, 	 1, (up_flags) );}
										if (rand_percent(30))		{	bonus_multi_add(c,/*->x+10, c->y-5,*/ SP_BONUS_00_FIREPOWER, 1, (up_flags) );}
											break;
				case SP_EN_BADGUY:		if (rand_percent(10))		{	put_item=(SP_BONUS_07_ADDSPEED&0xff);}		break;
				case SP_EN_SPLASH:									{	put_item=(SP_BONUS_06_COIN&0xff);}			break;
				case SP_EN_FAIRY:	//	if (rand_percent(30))		{	put_item=(c->x, c->y, SP_BONUS_00_FIREPOWER&0xff);}
										put_item=(rand()&0x07);
											break;
				}
				if (99!=put_item)
				{
					bonus_multi_add(c, (put_item|SP_BONUS_00_FIREPOWER), 1, (up_flags) );
				}
				c->type=SP_DELETE;
				bonus_info_any_score( c, ((d->score)*(/*pd->now_stage*/player_now_stage/*level*/)) );
			}
			//SDL_FreeSurface(spimg);
			break;
		}
	}
}




static void player_move_fireball(SPRITE *s) //霊夢、魔理沙、おぜう
{
	PL_FIREBALL_DATA *p=(PL_FIREBALL_DATA *)s->data;
	s->x+=co_s512((p->angle512))*p->speed*fps_factor;
	s->y+=si_n512((p->angle512))*p->speed*fps_factor;
	if (s->y<-10)
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*p->angle512,*/1,0);
	}
}

static void player_move_plasma(SPRITE *s)	//霊夢、魔理沙、おぜう
{
	PL_PLASMA_DATA *p=(PL_PLASMA_DATA *)s->data;
	s->x+=co_s512((p->angle512))*p->speed*fps_factor;
	s->y+=si_n512((p->angle512))*p->speed*fps_factor;
	if (s->y<-10)
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*p->angle512,*/1,0);
	}
}


#if (1==USE_PLAYER_WAVE)
static void player_move_killray(SPRITE *s)		//使わない
{
	PL_KILLRAY_DATA *p=(PL_KILLRAY_DATA *)s->data;

	s->x+=co_s512((p->angle512))*p->speed*fps_factor;
	s->y+=si_n512((p->angle512))*p->speed*fps_factor;
	p->distance+=p->speed*fps_factor;

	if ((p->distance>20)&&(s->aktframe<11)) {
		p->distance=0;
		s->aktframe++;
	}

	if (s->y<-10)
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*p->angle512,*/1,0);
	}
}
#endif //(1==USE_PLAYER_WAVE)

#if (1==USE_PLAYER_BOMBER)
/* ここは将来スペカになる予定 */
static void player_move_bomb(SPRITE *s)
{
	static int k=5;
	PL_BOMB_DATA *b=(PL_BOMB_DATA *)s->data;

	s->x+=co_s512((b->angle512))*b->speed*fps_factor;
	s->y+=si_n512((b->angle512))*b->speed*fps_factor;

	if ((s->x<0-100)||(s->x>100+GAME_WIDTH)||
		(s->y<0-100)||(s->y>100+GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*b->angle512,*/1,0);
	}
	if (k>0)	{k--;}else{b->speed++;k=5;}
}
#endif //(1==USE_PLAYER_BOMBER)


static /*int*/SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s=sprite;
	while (s!=NULL)
	{
		if ((s->type&SP_SHOW_ENEMYS)&&
			(!(s->flags&SP_FLAG_PARANOIA))&&
			(s->flags&SP_FLAG_VISIBLE))
		{
			if ((s->x>0)&&(s->x<GAME_WIDTH-s->w)&&
				(s->y>0)&&(s->y<GAME_HEIGHT-s->h))
			{
				return s/*->id*/;
			}
		}
		s=s->next;
	}
	return NULL/*-1*/;/*見つからなかった*/
}
#if (1==USE_PLAYER_HOMING)
//static void player_move_homing(SPRITE *s) {}
#endif //(1==USE_PLAYER_HOMING)
static void player_move_hlaser(SPRITE *s)
{
	PL_HOMING_DATA *h;
	h=(PL_HOMING_DATA *)s->data;
//
	SPRITE *target;
	target=NULL;	/* Target */
//	SPRITE *tg;
//	tg=(SPRITE *)h->tg_sprite; /* Target-Fadenkreuz */	/*ターゲット目標表示マーカー*/

	switch (h->state)
	{
	case 0: /* 回転しながら正面を向く。 / Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
		if (h->nr==0)	{	h->angle512+=deg_360_to_512(8)/**fps_factor*/;	}	/* 右回りに回転させる。/ linke Rakete: rot. im UZ */
		else			{	h->angle512-=deg_360_to_512(8)/**fps_factor*/;	}	/* 左回りに回転させる。/ rechte Rekete: rot. gegen UZ */
		//
		#if 0
		if (h->angle360<0)		{	h->angle360+=360;	}
		if (h->angle360>360)	{	h->angle360-=360;	}
		#else
		mask512(h->angle512);
		#endif
		//
		if ( (1.0) < h->speed )
		{
			h->speed -= (int)(0.5);/*減速*/
		}
		//
		if ((h->angle512>=deg_360_to_512(270-4/*266*/))&&
			(h->angle512<=deg_360_to_512(270+4/*274*/)))	/* ほぼ正面(270)を向いたら */
		{
			/* Sprengkopf in Ausgangspos? */
			h->state=2; 						/* 次へ */
			h->angle512=deg_360_to_512(270);	/* 正面(270)する */
		}
		break;
	case 2: /* 敵を探す。 / Ziel suchen */
		h->target_obj/*target_id*/=search_enemy_by_sprite();
		if (h->target_obj/*target_id*/ != NULL/*-1*/)
		{
			target=h->target_obj/*sprite_get_by_id (h->target_id)*/;
			if (target!=NULL)
			{
				h->state=3; 						/* 次へ */
				target->flags|=SP_FLAG_PARANOIA;	/* 「一つしかないですよ」フラグON */
			//	tg->flags|= SP_FLAG_VISIBLE;	/*ターゲット目標表示マーカー*/
			}
		//	else
		//	{
		//	//	tg->flags&= ~SP_FLAG_VISIBLE;	/*ターゲット目標表示マーカー*/
		//	}
		}
		break;
	case 3: /* Ziel verfolgen */
		if (/*h->target_id*/h->target_obj == NULL/*-1*/)	/* 見つからない */
		{
		//	/* ...orz ありえねえ */ error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
			h->state=2; 	/* 仕方なく戻る */
		}
		else
		{
			if ( (12.0) > h->speed)
			{
				h->speed += (int)(0.5);/*加速*/
			}
			target=h->target_obj/*sprite_get_by_id (h->target_id)*/;
			if (//(target!=NULL) && 	/* 重複してるので排除 */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x>0) &&
				(target->x<GAME_WIDTH-target->w) &&
				(target->y>0) &&
				(target->y<GAME_HEIGHT-target->h))
			{
				{int ta512;
					ta512=atan_512(target->y-s->y,target->x-s->x);
					if (h->angle512>ta512)
					{
							 if (h->angle512-ta512<deg_360_to_512(175)) 	{	h->angle512-=deg_360_to_512(8)*fps_factor;} /* hlaser:8, homing:9 */
						else if (h->angle512-ta512>deg_360_to_512(185)) 	{	h->angle512+=deg_360_to_512(8)*fps_factor;} /* hlaser:8, homing:9 */
					}
					else if (h->angle512<ta512)
					{
							 if (ta512-h->angle512<deg_360_to_512(175)) 	{	h->angle512+=deg_360_to_512(8)*fps_factor;} /* hlaser:8, homing:9 */
						else if (ta512-h->angle512>deg_360_to_512(185)) 	{	h->angle512-=deg_360_to_512(8)*fps_factor;} /* hlaser:8, homing:9 */
					}
				}
				#if 0
				if (h->angle360<0)		{	h->angle360+=360;}
				if (h->angle360>360)	{	h->angle360-=360;}
				#else
				mask512(h->angle512);
				#endif
			//	tg->x=target->x+((target->w-tg->w)/2)+rand()%6-3;	/*ターゲット目標表示マーカー*/
			//	tg->y=target->y+((target->h-tg->h)/2)+rand()%6-3;	/*ターゲット目標表示マーカー*/
			}
			else
			{
				/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
				h->target_obj=NULL/*h->target_id=-1*/;		/* 画面外に逃げられたら */
				h->state=2; 	/* 戻る */
			//	tg->flags&= ~SP_FLAG_VISIBLE;	/*ターゲット目標表示マーカー*/
			}
		}
		break;
	}
	#if 1
	/* (多分必要ないけど)念の為マスク */
	mask512(h->angle512);
	#endif
	s->x+=co_s512((h->angle512))*h->speed*fps_factor;
	s->y+=si_n512((h->angle512))*h->speed*fps_factor;
	#if 0
	/* homing */
	//	{	double dangle;
	//		dangle=h->angle+180;
	//		if (dangle>360) {	dangle-=360;}
	//		//parsys_add(NULL,20,0,s->x+s->w/2,s->y+s->h/2,20,dangle,30,10,PIXELATE,NULL);
	//	}
		#if 0
		s->aktframe=(((int)( h->angle360*20))/360) % 20;
		#else
		s->aktframe=(((int)( h->angle512*10))>>8);
		#endif
	#endif
	weapon_colcheck(s,/*h->angle,*/1,0);

	h->time_out--/*range-=fps_factor*/;
	if (h->time_out/*range*/<=0)	/* 時間切れ？ / max. Reichweite erreicht? */
	{
		s->type=SP_DELETE;		/* 時間切れなのでおしまい */
	//	tg->type=SP_DELETE; 	/* ターゲット目標表示マーカー*/
		if (target!=NULL)		/* ターゲットがまだ生きてたら、 Target freigeben */
		{
			target->flags &= ~SP_FLAG_PARANOIA; /* 弄ったフラグ戻しとこう。 */
		}
	}
}


/*
	s->x		シールドのx座標
	player->x	プレイヤーのx座標
	player->w/2 プレイヤーの横幅の半分
	co_s512((d->angle512))	angleの角度をπに直したあとcos。この場合は40°の倍数＋１０*fpsのcos。
	d->radius	円の半径40
	s->w/2		シールドの横幅の半分
	プレイヤーのx座標＋プレイヤーの横幅の半分＝プレイヤーの中心座標
	なので
	シールドのx座標＝プレイヤーのx座標＋プレイヤーの横幅の半分＋co s(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
				   ＝プレイヤーの中心座標＋co s(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
*/
static void player_move_shield(SPRITE *s)	//霊夢
{
	PL_SHIELD_DATA *d	=(PL_SHIELD_DATA *)s->data;
/// PLAYER_DATA *p		=(PLAYER_DATA *)player->data;
	d->angle512 += (d->speed/**fps_factor*/);/*...*/
	mask512(d->angle512);// if (d->angle>360)	{	d->angle-=360;}

//	s->aktframe = (((int)(d->angle360))/10)%36;
	s->aktframe = ((((d->angle512)/*&(512-1)*/)*(36/2))>>8);

	s->x=player->x+co_s512((d->angle512))*d->radius+((player->w-s->w)/2);
	s->y=player->y+si_n512((d->angle512))*d->radius+((player->h-s->h)/2);		//時計回り
	if (d->radius<255)	{	d->radius+=(rand()&1);} 	/* 少しずつ広がるようにした */
	else	{	s->type=SP_DELETE;}
	s->alpha			= ((int)(255-d->radius))&(256-1);				/* だんだん消えるようにした */


	weapon_colcheck(s,/*d->angle,*/0,1);

//	if (p->bomber_time<=0)
//	{	s->type=SP_DELETE;}
}

#if 0
static void player_move_shield2(SPRITE *s)	//霊夢
{
	PL_SHIELD_DATA *d	=(PL_SHIELD_DATA *)s->data;
	PLAYER_DATA *p		=(PLAYER_DATA *)player->data;
	d->angle512 += (d->speed/**fps_factor*/);/*...*/
	mask512(d->angle512);// if (d->angle>360)	{	d->angle-=360;}
	s->x=player->x+co_s512((d->angle512))*d->radius+((player->w-s->w)/2);
	s->y=player->y-si_n512((d->angle512))*d->radius+((player->h-s->h)/2);		//反時計回り
//	s->aktframe=(((int)(d->angle360))/10)%36;
	s->aktframe = ((((d->angle512)&(512-1))*(36/2))>>8);
	weapon_colcheck(s,/*d->angle,*/0,1);

	if (p->bomber_time<=0)
	{	s->type=SP_DELETE;}
}
#endif

static void player_move_levarie(SPRITE *s)
{
	PL_LEVARIE_DATA *d=(PL_LEVARIE_DATA *)s->data;
	s->x+=co_s512((d->angle512))*d->speed*fps_factor;
	s->y+=si_n512((d->angle512))*d->speed*fps_factor;

	if (CIRNO==select_player)
	{
		s->y -= 1.5*d->speed*fps_factor;/*チルノ用(暫定的)*/
	}

	weapon_colcheck(s,/*d->angle,*/0,1);

	if ((s->x+s->w<0)||(s->x>GAME_WIDTH)||
		(s->y+s->h<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
}

static void oz_move_missile(SPRITE *s)
{
	OZ_MISSILE_DATA *p=(OZ_MISSILE_DATA *)s->data;

	s->x+=co_s512((p->angle512))*p->speed*fps_factor;
	s->y+=si_n512((p->angle512))*p->speed*fps_factor;

	if (s->x<-s->w || GAME_WIDTH<s->x || -s->h>s->y)
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,0,1);
	}
}

static void oz_add_missile(SPRITE *s, int r_or_l)	/* [***090221 追加 */
{
	/*
		r_or_l	r=2, l=0 ,u=3 ,d=1
	*/
	SPRITE *c;
	OZ_MISSILE_DATA *d;
	if (!r_or_l)			{	c=sprite_add_file0("fire_wind_r.png",4,PR_PLAYER,0);}
	else if (r_or_l==2) 	{	c=sprite_add_file0("fire_wind_l.png",4,PR_PLAYER,0);}
	else					{	c=sprite_add_file0("fire_wind_u.png",4,PR_PLAYER,0);}
	c->flags			|= SP_FLAG_VISIBLE;
	d					= mmalloc(sizeof(OZ_MISSILE_DATA));
	c->data 			= d;
	c->type 			= SP_PL_SHIELD;
	c->anim_speed		= 3;
	c->aktframe 		= 0;
	c->mover			= oz_move_missile;
	c->alpha			= 100;
	c->x				= s->x+(s->w-c->w)/2;
	if (r_or_l<3)
	{	c->y			= s->y+15;}
	else
	{	c->y			= s->y-s->h+15;}
	d->strength 		= 10;
	d->angle512 		= ((double)r_or_l/2)*deg_360_to_512(180)/*M_PI*2*1/2*/;
	d->speed			= 12;
}

static void player_move_cross_red(SPRITE *s)	/* [***090220 追加 */
{
	PL_CROSS_RED_DATA *d=(PL_CROSS_RED_DATA *)s->data;
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;
	s->x=player->x+(player->w-s->w)/2;
	s->y=player->y+(player->h-s->h)/2;
	d->angle512 += rad2deg512(1)/*fps_factor*/;
	mask512(d->angle512);	//if (d->angle_>M_PI)	{	d->angle_ -= 2*M_PI;	}
	if (p->bomber_time>20)
	{
		if (0==(((int)p->bomber_time)&0x1f))
		{
			oz_add_missile(s, 2);
			oz_add_missile(s, 0);
			oz_add_missile(s, 3);
		}
		else if ((((int)p->bomber_time)&3/*%4*/)==0)
		{
			s->aktframe++;
			if (s->aktframe==4)
			{
				s->aktframe=0;
			}
		}
	}
	else
	{
		if ((((int)p->bomber_time)&3/*%4*/)==0)
		{
				 if (s->aktframe<4) 	{	s->aktframe=4;}
			else if (s->aktframe==4)	{	s->aktframe=5;}
			else if (s->aktframe==5)	{	s->aktframe=6;}
			else if (s->aktframe==6)	{	s->aktframe=7;}
			else if (s->aktframe==7)	{	s->aktframe=8;}
			else						{	s->aktframe=9;}
		}
	}

	weapon_colcheck(s,/*d->angle,*/0,1);

	if (p->bomber_time<=0)
	{	s->type=SP_DELETE;}
}


//#define hlaser_NUM_OF_ENEMIES (24)
#define hlaser_NUM_OF_ENEMIES (12)		/* [***090128		半分にしてみる */
static void player_controller_hlaser(CONTROLLER *c)
{
	int *id_array=c->e;

	SPRITE *s;
	int i;
	if (NULL==sprite_get_by_id(id_array[0]))
	{
		for (i=1;i<hlaser_NUM_OF_ENEMIES;i++)
		{
			s=sprite_get_by_id(id_array[i]);
			if (s!=NULL)
			{	s->type=SP_DELETE;}
		}
		controller_remove(c);
	}
	else
	{
		for (i=hlaser_NUM_OF_ENEMIES-1;i>=1;i--)
		{
			SPRITE *l/*=NULL*/;
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if ((s!=NULL)&&(l!=NULL))
			{
				s->x=l->x;
				s->y=l->y;
				// draw_line(screen,s->x,s->y,l->x,l->y,
				// SDL_MapRGB(screen->format,0xff,0xff,0xff),
				// SDL_MapRGB(screen->format,0xa0,0xff,0xa0));
			}
		}
	}
}


static void re_add_fireball(SPRITE *s, int x, int y)	//霊夢
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+REIMU/*select_player*/ ); //sprite_add_file0("fireball1.png",2,PR_PLAYER,1);	shot->anim_speed=1;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= x-shot->w/2;
	shot->y 			= y;
	shot->mover 		= player_move_fireball;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_FIREBALL_DATA *data;
	data				= mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data			= data;
	data->angle512		= deg_360_to_512(270);
	data->speed 		= 18;
	data->strength		= 3;
}

static void ma_add_fireball(SPRITE *s, int x, int y)	//魔理沙
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+select_player ); //sprite_add_file0((char *)aaa_name[(k)],1,PR_PLAYER,0); shot->anim_speed=0;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= x-shot->w/2;
	shot->y 			= y;
	shot->alpha 		= 150;
	shot->mover 		= player_move_fireball;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_FIREBALL_DATA *data;
	data				= mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data			= data;
	data->angle512		= deg_360_to_512(270);
	data->speed 		= 10;
	data->strength		= 4;
}
enum
{
	REMIRIA_NEEDLE_TYPE270=0,
	REMIRIA_NEEDLE_TYPE263,
	REMIRIA_NEEDLE_TYPE277,
	REMIRIA_NEEDLE_TYPE260,
};
static void oz_add_fireball(SPRITE *s, int needle_type )	//おぜう	/* [***090220 追加 */
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+select_player/*REMIRIA*/ ); //sprite_add_file0("bat.png",5,PR_PLAYER,0); shot->anim_speed=3;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= (s->x+s->w/2)/*x*/-shot->w/2;
	shot->y 			= (s->y-7)/*y*/;
	shot->mover 		= player_move_fireball;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_FIREBALL_DATA *data;
	data				= mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data			= data;
	const signed short bbb[4] =
	{
		deg_360_to_512(270),
		deg_360_to_512(263),
		deg_360_to_512(277),
		deg_360_to_512(260),
	};
	data->angle512		= bbb[needle_type]/*deg_360_to_512(270)*/;
	data->speed 		= 8;
	data->strength		= 5;
}
//		switch (o->state)
//		{
//		case 0:
//			o->state=1;
//		//	o->angle512 -= o->pos*rad2deg512(0.1);
//			break;
//		case 1:
//			mask512(o->angle512);
//			if (o->angle512 > 256 )//if ((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05))
//			{
//				o->state=2;
//			//	o->angle=-M_PI/2;
//			}
//			else
//			{	o->angle512 -= o->pos*rad2deg512(0.1);}
//			break;
//		case 2:
//			break;
//		}

/*	*/
static void re_ma_control_option(SPRITE *s, RE_OPTION_DATA *o, PLAYER_DATA *d)	//霊夢、魔理沙
{
	int test_angle512;
	test_angle512 = o->angle512 - 128;/* 判定用角度：９０度回転させて下が０とする */	/*擬似単位系変換*/
	mask512(test_angle512);
	if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)	/* 押すと前方に移動 */
	{
		if (1==o->pos)	/* 右側のオプション */
		{
			if (test_angle512 > /*(224)*/(240)/*(256-16)*/ )//if ((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05))
			{
				o->angle512 -= rad2deg512(0.1);
			}
		}
		else	/* 左側のオプション */
		{
			if (test_angle512 < /*(288)*/(272)/*(256+16)*/ )//if ((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05))
			{
				o->angle512 += rad2deg512(0.1);
			}
		}
	}
	else	/* 放すと後方に移動 */
	{
		if (1==o->pos)	/* 右側のオプション */
		{
			if (test_angle512 < 448-16 )//if ((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05))
			{
				o->angle512 += rad2deg512(0.1);
			}
		}
		else	/* 左側のオプション */
		{
			if (test_angle512 > 64+16 )//if ((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05))
			{
				o->angle512 -= rad2deg512(0.1);
			}
		}
	}
	if (d->option==0 || d->lives<0)
	{	s->type=SP_DELETE;}
	#if 1
	mask512(o->angle512);/*ねんのため*/
	#endif
	s->x=player->x+co_s512((o->angle512))*o->length+(((int)(o->pos+1)*player->w-s->w)/2)-o->pos*6;
	s->y=player->y+si_n512((o->angle512))*o->length+((player->h-s->h)/2)-5;

}
//		switch (o->state)
//		{
//		case 0:
//			break;
//		case 1:
//			mask512(o->angle512);
//			if (o->angle512 < 256 )//if (o->angle<-M_PI || o->angle>0)
//			{
//				o->state=0;
//			//	o->angle=(o->pos+1)*(M_PI*2)*1/4-(M_PI*2)*2/4;
//			}
//			else
//			{	o->angle512+=o->pos*rad2deg512(0.1);}
//			break;
//		case 2:
//			o->state=1;
//		//	o->angle512+=o->pos*rad2deg512(0.1);
//			break;
//		}
static void re_move_option(SPRITE *s)	//霊夢
{
	RE_OPTION_DATA *o=(RE_OPTION_DATA *)s->data;
	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
	o->ani_speed--;
	if (o->ani_speed==0)
	{
		o->ani_speed=12/*2*/;/*陰陽だまはゆっくり回るよ*/
		s->aktframe=((s->aktframe+o->pos)&(8-1)/*%8*/);
	//	if (s->aktframe<0)		{	s->aktframe=7;}
	}

	if (d->state==PL_SAVE||d->state==PL_NORMAL)
	{
		o->w_wait--;
		if (o->w_wait<0)
		{
			o->w_wait=4;
			if ( (d->bossmode!=B03_BEFORE_EVENT) && (d->bossmode!=B06_AFTER_EVENT))
			{
				if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
				{
					if (((PLAYER_DATA *)player->data)->weapon<=90-1)
					{
						re_add_fireball(s, s->x+s->w/2, s->y-7);
					}
					else
					{
						re_add_fireball(s, s->x+s->w/2-5, s->y-7);
						re_add_fireball(s, s->x+s->w/2+5, s->y-7);
					}
				}
			}
		}
	}
	re_ma_control_option(s, o, d);
}


static void ma_move_option(SPRITE *s)	//魔理沙
{
	RE_OPTION_DATA *o=(RE_OPTION_DATA *)s->data;
	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
	o->ani_speed--;
	if (o->ani_speed==0)
	{
		o->ani_speed=6;
		s->aktframe=((s->aktframe+o->pos)&(8-1)/*%8*/);
	//	if (s->aktframe<0)		{	s->aktframe=7;}
	}

	if (d->state==PL_SAVE||d->state==PL_NORMAL)
	{
		o->w_wait--;
		if (o->w_wait<0)
		{
			o->w_wait=8;
			if ( (d->bossmode!=B03_BEFORE_EVENT) && (d->bossmode!=B06_AFTER_EVENT))
			{
				if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
				{
					if (((PLAYER_DATA *)player->data)->weapon<=70-1)
					{
						ma_add_fireball(s, s->x+s->w/2, s->y-7);
					}
					else if (((PLAYER_DATA *)player->data)->weapon<=100-1)
					{
						ma_add_fireball(s, s->x+s->w/2-7, s->y-7);
						ma_add_fireball(s, s->x+s->w/2+7, s->y-7);
					}
					else
					{
						ma_add_fireball(s, s->x+s->w/2, s->y-7);
						ma_add_fireball(s, s->x+s->w/2-7, s->y-45);
						ma_add_fireball(s, s->x+s->w/2+7, s->y-7);
					}
				}
			}
		}
	}
	re_ma_control_option(s, o, d);
}

/* レミリア用 */
static SPRITE *c1, *c2, *c3, *c4; //[***090128.090220	追加:オプション用.c3,c4
/*
	目標:state1
	FORMATION_00: レミリアの直後に四つ
	FORMATION_01: レミリアの周りを回るよ
	FORMATION_02: 画面後方から支援するよ (y=250の位置に散らばる)
	FORMATION_03: 減速時用(好きな位置に置けるよ) (レミリアに重なるように。slowを押すと段階的にoption停止)
*/
#define FORMATION_00 (0)
#define FORMATION_01 (1)
#define FORMATION_02 (2)
#define FORMATION_03 (3)


#define OPTION_01 (0)
#define OPTION_02 (1)
#define OPTION_03 (2)
#define OPTION_04 (3)

static void oz_move_option(SPRITE *s)	//おぜう	/* [***090220 追加 */
{
	static /*double*/int l_angle512;
	static int cancel_flag; //- 	もしかしたらいらない子
	static int change_d;	//- 	この2つはid==1が管理する。
	int tmp=0;

	OZ_OPTION_DATA *o=(OZ_OPTION_DATA *)s->data;
	PLAYER_DATA *d=(PLAYER_DATA *)player->data;

	if (s->flags&SP_FLAG_VISIBLE)	//表示されていれば上記の処理をするよ。
	{
		o->ani_speed--;
		if (o->ani_speed==0)		//アニメーション
		{
			o->ani_speed=3;
			s->aktframe++;
			s->aktframe &= (8-1);//if (s->aktframe==8)	{s->aktframe=0;}
		}
		//
		if (d->state==PL_SAVE||d->state==PL_NORMAL) 	//攻撃
		{
			o->w_wait--;
			if (o->w_wait<0)
			{
				if ( (d->bossmode!=B03_BEFORE_EVENT) && (d->bossmode!=B06_AFTER_EVENT))
				{
					if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
					{
						switch (o->option_id)
						{
						case OPTION_01:
						case OPTION_02:
							if (d->weapon>100-1)
							{
								oz_add_fireball(s, REMIRIA_NEEDLE_TYPE277 );
								oz_add_fireball(s, REMIRIA_NEEDLE_TYPE263 );
								o->w_wait=7;
							}
							else
							{
								oz_add_fireball(s, REMIRIA_NEEDLE_TYPE270 );
								o->w_wait=5;
							}
							break;
						case OPTION_03:
						case OPTION_04:
							oz_add_fireball(s, REMIRIA_NEEDLE_TYPE270 );
							o->w_wait=5;
							break;
						}
					}
				}
			}
		}
		//
		if (o->option_id==OPTION_01)	//option_id==0によるflag管理
		{
			if (change_d==20)
			{
				cancel_flag=0;
				change_d--;
			}
			else if (0<change_d && change_d<20)
			{
				change_d--;
			}
			else if (change_d<=0 && !cancel_flag)
			{	cancel_flag=1;}
		}
		int slow_flag;
		slow_flag=0;
		if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)
		{
			if (REMIRIA==select_player)
			{
				slow_flag=1;
			}
		//	else
		//	{
		//		/*チルノ*/
		//	}
		}

		if (1==slow_flag)	// 少なくとも低速移動でオプションが止められるのはレミのみ(チルノには合わない)
		{
			o->slow_count++;
			if ((o->state1==FORMATION_03) &&	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
				(o->slow_count < (o->option_id+1)*30))
			{
				s->x = player->x+(player->w-s->w)/2;
				s->y = player->y+(player->h-s->h)/2;
			}
			else if (o->state1==FORMATION_02)	/* FORMATION_02: 画面後方から支援するよ */
			{
				switch (o->option_id)
				{
				case OPTION_01: if (s->x<170-s->w)	{s->x+=fps_factor*2;}	break;
				case OPTION_02: if (s->x>210)		{s->x-=fps_factor*2;}	break;
				case OPTION_03: if (s->x<100-s->w)	{s->x+=fps_factor*4;}	break;
				case OPTION_04: if (s->x>280)		{s->x-=fps_factor*4;}	break;
				}
			}
		}
		else
		{
			if (o->slow_count)
			{
				o->slow_count=0;
				o->state2=0;
			}
			if (cancel_flag)
			{
				if (my_pad & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)
				{
					if (o->option_id==OPTION_01)	//option_id==1によるflag管理
					{
						if (change_d<=0)
						{
							change_d=20;
							o->state2=0;
							o->state1++;
							if (o->state1==(FORMATION_03+1) )
							{	o->state1=FORMATION_00;}
							if (c4->flags&SP_FLAG_VISIBLE)
							{
								((OZ_OPTION_DATA *)c4->data)->state1=o->state1;
								((OZ_OPTION_DATA *)c4->data)->state2=0;
							}
							if (c3->flags&SP_FLAG_VISIBLE)
							{
								((OZ_OPTION_DATA *)c3->data)->state1=o->state1;
								((OZ_OPTION_DATA *)c3->data)->state2=0;
							}
							if (c2->flags&SP_FLAG_VISIBLE)
							{
								((OZ_OPTION_DATA *)c2->data)->state1=o->state1;
								((OZ_OPTION_DATA *)c2->data)->state2=0;
							}
						}
					}
				}
			}
			switch (o->state2)
			{
			case 0: 	//初期化
				{
					#define FORMATION_00_LOCATE_X 0
					#define FORMATION_00_LOCATE_Y 1
					#define FORMATION_02_LOCATE_X 2
					const signed short fff[3][4]=
					{
						{ 15,-15, 25,-25 },
						{ 15, 15, 20, 20 },
						{  1*115+10,  2*115+10,  0*115+10,	3*115+10 }
					};
					switch (o->state1)
					{
					case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
						o->offset_x=fff[FORMATION_00_LOCATE_X][(o->option_id)];
						o->offset_y=fff[FORMATION_00_LOCATE_Y][(o->option_id)];
						break;
					case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
						o->offset_x=40;
						o->offset_y=0;
						break;
					case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
						o->offset_x=fff[FORMATION_02_LOCATE_X][(o->option_id)];
						o->offset_y=240;
						o->m_angle512=atan_512(240-s->y-s->h/2, o->offset_x-s->x-s->w/2);
						break;
					case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
						break;
					}
				}
				o->state2=1;
				break;
			case 1: 	//移動
				#define oz_offset_r (o->offset_x)		/* 円の半径と兼用 */
				switch (o->state1)
				{
				case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
					tmp=					player->y+				o->offset_y - s->y+((player->h-s->h)/2);	/* tmp : y座標における目標地点と現在地の差 */
					o->m_angle512=atan_512(tmp, player->x+player->w/2+	o->offset_x - s->x-s->w/2);
					if ( (-5 < tmp) && (tmp < 5) )
					{
						s->x=player->x+player->w/2+o->offset_x;
						s->y=player->y+player->h/2+o->offset_y;
						o->state2=2;
					}
					else
					{
						s->x+=co_s512(o->m_angle512)*fps_factor*10;
						s->y+=si_n512(o->m_angle512)*fps_factor*10;
					}
					break;
				case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
					tmp=						player->y+				si_n512(o->angle512+l_angle512)*oz_offset_r - s->y+((player->h-s->h)/2);	/* tmp : y座標における目標地点と現在地の差 */
					o->m_angle512=atan_512(tmp, player->x+player->w/2+	co_s512(o->angle512+l_angle512)*oz_offset_r - s->x-s->w/2);
					if ( (-5 < tmp) && (tmp < 5) )
					{
						s->x=player->x+player->w/2+si_n512(o->angle512+l_angle512)*oz_offset_r;
						s->y=player->y+player->h/2+co_s512(o->angle512+l_angle512)*oz_offset_r;
						o->state2=2;
					}
					else
					{
						s->x+=co_s512(o->m_angle512)*fps_factor*10;
						s->y+=si_n512(o->m_angle512)*fps_factor*10;
					}
					break;
				case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
					tmp=(240 - s->y-s->h/2);
					if ( (-5 < tmp) && (tmp < 5) )
					{
						s->x=o->offset_x;
						s->y=o->offset_y;
						o->state2=2;
					}
					else
					{
						s->x+=co_s512(o->m_angle512)*fps_factor*10;
						s->y+=si_n512(o->m_angle512)*fps_factor*10;
					}
					break;
				case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
					tmp=player->y - s->y+((player->h-s->h)/2);
					o->m_angle512=atan_512(tmp, player->x+player->w/2 - s->x-s->w/2);
					if ( (-5 < tmp) && (tmp < 5) )
					{
						s->y=player->y+(player->h-s->h)/2;
						s->x=player->x+(player->w-s->w)/2;
						o->state2=2;
					}
					else
					{
						s->x+=co_s512(o->m_angle512)*fps_factor*10;
						s->y+=si_n512(o->m_angle512)*fps_factor*10;
					}
					break;
				}
				break;
			case 2: 	//位置の維持 & 配置完了flag
				switch (o->state1)
				{
				case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
					s->x = player->x+(player->w-s->w)/2+o->offset_x;
					s->y = player->y+(player->h-s->h)/2+o->offset_y;
					break;
				case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
					l_angle512 += rad2deg512(0.05)*fps_factor;
					mask512(l_angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
					switch (o->option_id)
					{
					case OPTION_01:
					case OPTION_02:
						s->x = player->x+(player->w-s->w)/2+si_n512(o->angle512+l_angle512)*oz_offset_r;
						s->y = player->y+(player->h-s->h)/2+co_s512(o->angle512+l_angle512)*oz_offset_r;
						break;
					case OPTION_03:
					case OPTION_04:
						s->x = player->x+(player->w-s->w)/2+si_n512(o->angle512-l_angle512)*oz_offset_r;
						s->y = player->y+(player->h-s->h)/2+co_s512(o->angle512-l_angle512)*oz_offset_r;
						break;
					}
					break;
				case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
					s->x = o->offset_x;
					s->y = o->offset_y;
					break;
				case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
					s->x = player->x+(player->w-s->w)/2;
					s->y = player->y+(player->h-s->h)/2;
					break;
				break;
				}
			}
		}
	}

	switch (o->option_id)
	{
	case OPTION_01:
		break;
	case OPTION_02:
		if ((s->flags&SP_FLAG_VISIBLE) && (d->weapon<=55-1))	{s->flags&= ~SP_FLAG_VISIBLE;}
		else if (!(s->flags&SP_FLAG_VISIBLE) && (d->weapon>55-1))
		{
			s->flags|= SP_FLAG_VISIBLE;
			o->state1=((OZ_OPTION_DATA *)c1->data)->state1;
			o->state2=0;
		}
		break;
	case OPTION_03:
		if ((s->flags&SP_FLAG_VISIBLE) && (d->weapon<=80-1))	{s->flags&= ~SP_FLAG_VISIBLE;}
		else if (!(s->flags&SP_FLAG_VISIBLE) && (d->weapon>80-1))
		{
			s->flags|= SP_FLAG_VISIBLE;
			o->state1=((OZ_OPTION_DATA *)c1->data)->state1;
			o->state2=0;
		}
		break;
	case OPTION_04:
		if ((s->flags&SP_FLAG_VISIBLE) && (d->weapon<=108-1))	{s->flags&= ~SP_FLAG_VISIBLE;}
		else if (!(s->flags&SP_FLAG_VISIBLE) && (d->weapon>108-1))
		{
			s->flags|= SP_FLAG_VISIBLE;
			o->state1=((OZ_OPTION_DATA *)c1->data)->state1;
			o->state2=0;
		}
		break;
	}
	if (d->option==0 || d->lives<0) {s->type=SP_DELETE;}
}

static void re_add_option(SPRITE *s)	//霊夢
{
	RE_OPTION_DATA *d1, *d2;
	c1				= sprite_add_res( BASE_OPTION1_PNG/*+REIMU*/ );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->mover		= re_move_option;
	d1				= mmalloc(sizeof(RE_OPTION_DATA));
	c1->data		= d1;
	d1->strength	= 5;/* [***090214 追加 */
	d1->length		= 11;
	c1->x			= s->x-d1->length+(s->w-c1->w)/2;
	c1->y			= s->y+(s->h-c1->h)/2;
	d1->ani_speed	= 2;
	d1->pos 		= -1;
	d1->angle512	= deg_360_to_512(180)/*-rad2deg512(M_PI)*/;
//	d1->state		= 0;
	d1->w_wait		= 0;
	c1->aktframe	= 0;
	c1->type		= SP_PLAYER2;

	c2				= sprite_add_res( BASE_OPTION1_PNG/*+REIMU*/ );
	c2->flags		|= SP_FLAG_VISIBLE;
	c2->mover		= re_move_option;
	d2				= mmalloc(sizeof(RE_OPTION_DATA));
	c2->data		= d2;
	d2->strength	= 5;/* [***090214 追加 */
	d2->length		= 11;
	c2->x			= s->x-d2->length+(s->w-c2->w)/2;
	c2->y			= s->y+(s->h-c2->h)/2;
	d2->ani_speed	= 2;
	d2->pos 		= 1;
	d2->angle512	= deg_360_to_512(0);
//	d2->state		= 0;
	d2->w_wait		= 0;
	c2->aktframe	= 0;
	c2->type		= SP_PLAYER2;
}

static void ma_add_option(SPRITE *s)	//魔理沙
{
	RE_OPTION_DATA *d1, *d2;		//霊夢のを流用してます。
	c1				= sprite_add_res( BASE_OPTION1_PNG+MARISA );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->mover		= ma_move_option;
	d1				= mmalloc(sizeof(RE_OPTION_DATA));
	c1->data		= d1;
	d1->strength	= 5;/* [***090214 追加 */
	d1->length		= 11;
	c1->x			= s->x-d1->length+(s->w-c1->w)/2;
	c1->y			= s->y+(s->h-c1->h)/2;
	d1->ani_speed	= 6;
	d1->pos 		= -1;
	d1->angle512	= deg_360_to_512(180)/*-rad2deg512(M_PI)*/;
//	d1->state		= 0;
	d1->w_wait		= 0;
	c1->aktframe	= 0;
	c1->type		= SP_PLAYER2;

	c2				= sprite_add_res( BASE_OPTION1_PNG+MARISA );
	c2->flags		|= SP_FLAG_VISIBLE;
	c2->mover		= ma_move_option;
	d2				= mmalloc(sizeof(RE_OPTION_DATA));
	c2->data		= d2;
	d2->strength	= 5;/* [***090214 追加 */
	d2->length		= 11;
	c2->x			= s->x-d2->length+(s->w-c2->w)/2;
	c2->y			= s->y+(s->h-c2->h)/2;
	d2->ani_speed	= 6;
	d2->pos 		= 1;
	d2->angle512	= deg_360_to_512(0);
//	d2->state		= 0;
	d2->w_wait		= 0;
	c2->aktframe	= 0;
	c2->type		= SP_PLAYER2;
}

static void oz_add_option(SPRITE *s)	//おぜう	/* [***090220 追加 */
{
	OZ_OPTION_DATA *d;
	c1				= sprite_add_res( BASE_OPTION1_PNG+select_player/*REMIRIA*/ );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->type		= SP_PLAYER2;
	c1->mover		= oz_move_option;
	d				= mmalloc(sizeof(OZ_OPTION_DATA));
	c1->data		= d;
	c1->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*1)/*M_PI*2*1/8*/;
//	d->next 		= player;
	d->state1		= 0;
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_01;
	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c1->x = s->x+(s->w-c1->w)/2;
	c1->y = s->y+(s->h-c1->h)/2+20;

	c2				= sprite_add_res( BASE_OPTION2_PNG+select_player/*REMIRIA*/ );
	c2->flags		&= ~SP_FLAG_VISIBLE;
	c2->type		= SP_PLAYER2;
	c2->mover		= oz_move_option;
	d				= mmalloc(sizeof(OZ_OPTION_DATA));
	c2->data		= d;
	c2->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*5)/*M_PI*2*5/8*/;
//	d->next 		= c1;
	d->state1		= ((OZ_OPTION_DATA *)c1->data)->state1;
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_02;
	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c2->x = s->x+(s->w-c2->w)/2;
	c2->y = s->y+(s->h-c2->h)/2+20;

	c3				= sprite_add_res( BASE_OPTION3_PNG+select_player/*REMIRIA*/ );
	c3->flags		&= ~SP_FLAG_VISIBLE;
	c3->type		= SP_PLAYER2;
	c3->mover		= oz_move_option;
	d				= mmalloc(sizeof(OZ_OPTION_DATA));
	c3->data		= d;
	c3->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*3)/*M_PI*2*3/8*/;
//	d->next 		= c2;
	d->state1		= ((OZ_OPTION_DATA *)c1->data)->state1;
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_03;
	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c3->x = s->x+(s->w-c3->w)/2;
	c3->y = s->y+(s->h-c3->h)/2+20;

	c4				= sprite_add_res( BASE_OPTION4_PNG+select_player/*REMIRIA*/ );
	c4->flags		&= ~SP_FLAG_VISIBLE;
	c4->type		= SP_PLAYER2;
	c4->mover		= oz_move_option;
	d				= mmalloc(sizeof(OZ_OPTION_DATA));
	c4->data		= d;
	c4->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*7)/*M_PI*2*7/8*/;
//	d->next 		= c3;
	d->state1		= ((OZ_OPTION_DATA *)c1->data)->state1;
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_04;
	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c4->x			= s->x+(s->w-c4->w)/2;
	c4->y			= s->y+(s->h-c4->h)/2+20;
}

/*---new add-----*/


static SPRITE *player_add_core(SPRITE *s1)		//○の追加
{
	SPRITE *s2;
	s2				= NULL;
	s2				= sprite_add_res( BASE_CORE_PNG+select_player );
	s2->anim_speed	= 0;
	s2->type		= SP_PLAYER2;
	s2->x			= s1->x+((s1->w-s2->w)/2);
	s2->y			= s1->y-s2->h+20;
	s2->mover		= player_move_core;
	s2->flags		|= SP_FLAG_VISIBLE;
	return s2;
}


enum
{
	REIMU_SHOT_TYPE_00=0,
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
	REIMU_SHOT_TYPE_08,
	REIMU_SHOT_TYPE_09,
	REIMU_SHOT_TYPE_0a,
	REIMU_SHOT_TYPE_MAX
};

//2717794 2717730 2715138
static void player_add_fuda(SPRITE *player, int shot_type) //霊夢
{
	enum
	{
		REI00_x_offset=0,
		REI01_y_offset,
		REI02_angle512,
		REI03_yudo,
		REI04_MAX,
	};
	const Sint16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset	y_offset  angle512 yudo */
/*00*/	{  0, 20, deg_360_to_512(270),	  0},	/*player_add_fuda*/
/*01*/	{ -5, 20, deg_360_to_512(270),	  0},	/*player_add_doublefuda*/
/*02*/	{ +5, 20, deg_360_to_512(270),	  0},	/*player_add_doublefuda*/
/*03*/	{-15,+25, deg_360_to_512(270-6),  1},	/*player_add_triplefuda*/
/*04*/	{+15,+25, deg_360_to_512(270+6),  1},	/*player_add_triplefuda*/
/*05*/	{-15,+25, deg_360_to_512(270-10), 1},	/*player_add_quadfuda*/
/*06*/	{+15,+25, deg_360_to_512(270+10), 1},	/*player_add_quadfuda*/

/*07*/	{-15,+25, deg_360_to_512(270-15), 0},	/*player_add_fifthfuda*/
/*08*/	{+15,+25, deg_360_to_512(270+15), 0},	/*player_add_fifthfuda*/
/*09*/	{ -5,+20, deg_360_to_512(270-8),  1},	/*player_add_fifthfuda*/
/*0a*/	{ +5,+20, deg_360_to_512(270+8),  1},	/*player_add_fifthfuda*/

	};
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+REIMU );
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)+r_tbl[shot_type][REI00_x_offset];
	shot->y 		= player->y-shot->h+r_tbl[shot_type][REI01_y_offset]/*20*/;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	shot->alpha 	= 150;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->strength	= 2;
	data->speed 	= 15;
//
	int ok;
	ok=0;
	{
		if (1==r_tbl[shot_type][REI03_yudo])/* 自動ショットか？ */
		{
			SPRITE *target;
			target=search_enemy_by_sprite();
			if (target != NULL)
			{
				data->angle512	= atan_512(target->y-player->y,target->x-player->x);
			//	if (255 < data->angle512 )/* 上方180-360度[/360度]の領域のみ追尾する */
				if ( /*(384)*/(128*3) < ((data->angle512+64)&(512-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */
				{
					ok=1;/* 自動追尾可能 */
				}
			}
		}
	}
//
	if (0==ok)/* 自動追尾不可？ */
	{
		data->angle512	= r_tbl[shot_type][REI02_angle512]/*deg_360_to_512(270)*/;/*固有の設定角度を使う*/
	}
}

static void player_add_doublefuda(SPRITE *player)	//霊夢
{
	player_add_fuda(player,REIMU_SHOT_TYPE_01);
	player_add_fuda(player,REIMU_SHOT_TYPE_02);
}

static void player_add_triplefuda(SPRITE *player)	//霊夢
{
	player_add_fuda(player,REIMU_SHOT_TYPE_03);/* 自動ショット */
	player_add_fuda(player,REIMU_SHOT_TYPE_00);
	player_add_fuda(player,REIMU_SHOT_TYPE_04);/* 自動ショット */

}

static void player_add_quadfuda(SPRITE *player) //霊夢
{
	player_add_fuda(player,REIMU_SHOT_TYPE_05);/* 自動ショット */
	player_add_fuda(player,REIMU_SHOT_TYPE_01);
	player_add_fuda(player,REIMU_SHOT_TYPE_02);
	player_add_fuda(player,REIMU_SHOT_TYPE_06);/* 自動ショット */
}

static void player_add_fifthfuda(SPRITE *player)	//霊夢
{
	player_add_fuda(player,REIMU_SHOT_TYPE_07);
	player_add_fuda(player,REIMU_SHOT_TYPE_09);/* 自動ショット */
	player_add_fuda(player,REIMU_SHOT_TYPE_00);
	player_add_fuda(player,REIMU_SHOT_TYPE_0a);/* 自動ショット */
	player_add_fuda(player,REIMU_SHOT_TYPE_08);
}

static void player_add_star(SPRITE *player) 	//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2);
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270);
	data->speed 	= 12;
	data->strength	= 3;
}

static void player_add_doublestar(SPRITE *player)	//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)-5;
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270);
	data->speed 	= 12;
	data->strength	= 3;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)+5;
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270);
	data->speed 	= 12;
	data->strength	= 3;
}

static void player_add_triplestar(SPRITE *player)		//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot				= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type			= SP_PL_PLASMA;
	shot->x 			= player->x+((player->w-shot->w)/2)-5;
	shot->y 			= player->y-shot->h+20;
	shot->mover 		= player_move_plasma;
	shot->flags 		|= SP_FLAG_VISIBLE;
	shot->alpha 		= 150;
	data				= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data			= data;
	data->angle512		= deg_360_to_512(270-4);
	data->speed 		= 12;
	data->strength		= 3;

	shot				= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type			= SP_PL_PLASMA;
	shot->x 			= player->x+((player->w-shot->w)/2);
	shot->y 			= player->y-shot->h+20;
	shot->mover 		= player_move_plasma;
	shot->flags 		|= SP_FLAG_VISIBLE;
	shot->alpha 		= 150;
	data				= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data			= data;
	data->angle512		= deg_360_to_512(270);
	data->speed 		= 12;
	data->strength		= 3;

	shot				= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type			= SP_PL_PLASMA;
	shot->x 			= player->x+((player->w-shot->w)/2)+5;
	shot->y 			= player->y-shot->h+20;
	shot->mover 		= player_move_plasma;
	shot->flags 		|= SP_FLAG_VISIBLE;
	shot->alpha 		= 150;
	data				= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data			= data;
	data->angle512		= deg_360_to_512(270+4);
	data->speed 		= 12;
	data->strength		= 3;
}

static void player_add_quadstar(SPRITE *player) //魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)-10;
	shot->y 		= player->y-shot->h+25;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270-7);
	data->speed 	= 12;
	data->strength	= 3;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)-6;
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270-3);
	data->speed 	= 12;
	data->strength	= 3;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)+6;
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270+3);
	data->speed 	= 12;
	data->strength	= 3;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player/*MARISA*/ );/*チルノむりやり兼用(暫定的)*/
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)+10;
	shot->y 		= player->y-shot->h+25;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270+7);
	data->speed 	= 12;
	data->strength	= 3;
}

static void player_add_thing(SPRITE *player)	//おぜう	/* [***090220 追加 */
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+REMIRIA );
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2);
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270);
	data->speed 	= 18;
	data->strength	= 4;
}

static void player_add_doublething(SPRITE *player)	//おぜう	/* [***090220 追加 */
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+REMIRIA );
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)-5;
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270);
	data->speed 	= 18;
	data->strength	= 4;

	shot			= sprite_add_res( BASE_PLAZMA_PNG+REMIRIA );
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)/2)+5;
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_plasma;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data		= data;
	data->angle512	= deg_360_to_512(270);
	data->speed 	= 18;
	data->strength	= 4;
}

#if (1==USE_PLAYER_WAVE)
static void player_add_killray(SPRITE *player)		//使わない
{
	SPRITE *shot;
	PL_KILLRAY_DATA *data;
	static int last=0;
	if (last)
	{
		shot=sprite_add_file0("killray-r.png",12,PR_PLAYER,0);
		last=0;
	}
	else
	{
		shot=sprite_add_file0("killray-b.png",12,PR_PLAYER,0);
		last=1;
	}
	shot->type		= SP_PL_KILLRAY;
	shot->x 		= player->x+((player->w-shot->w)/2);
	shot->y 		= player->y-shot->h+20;
	shot->mover 	= player_move_killray;
	shot->flags 	|= SP_FLAG_VISIBLE;
	data			= mmalloc(sizeof(PL_KILLRAY_DATA));
	shot->data		= data;
	data->angle 	= 270;
	data->speed 	= 25;
	data->distance	= 0;
	data->strength	= 6;
}
#endif //(1==USE_PLAYER_WAVE)

#if (1==USE_PLAYER_BOMBER)
/* ここは将来スペカになる予定 */
static void player_add_bomb(SPRITE *s)
{
	SPRITE *c;
	PL_BOMB_DATA *b;
	int a;
	for (a=0;a<355;a+=5)
	{
		c				= sprite_add_file0("fireball.png",1,PR_PLAYER,0);
		c->type 		= SP_PL_BOMB;
		c->x			= player->x+player->w/2;
		c->y			= player->y+player->h/2;
		c->mover		= player_move_bomb;
		c->flags		|= SP_FLAG_VISIBLE;
		b				= mmalloc(sizeof(PL_BOMB_DATA));
		c->data 		= b;
		b->angle		= a;
		b->speed		= 1;
		b->strength 	= 2;
	}
}
#endif //(1==USE_PLAYER_BOMBER)

static void player_add_shield(SPRITE *s)		//シールドの追加	//霊夢
{
	SPRITE *c;
	PL_SHIELD_DATA *d;
	PLAYER_DATA *sd=(PLAYER_DATA *)s->data;
	int ii;//	int i;	/* 半象限ずつ */
	sd->bonus_flag	|= BONUS_FLAG_02_AUTO_GET_ITEM;
	for (ii=0;ii<(8);ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		if ( (ii&1) == 0)// if (i%90 == 0)
		{
			c			= sprite_add_file0("cshoot1.png",36,PR_PLAYER,1);
			d			= mmalloc(sizeof(PL_SHIELD_DATA));
			c->data 	= d;
			c->mover	= player_move_shield;		//時計回り
			d->speed	= 23;						//時計回り
			d->radius	= 5/*48*/;	//d->radius=38; 	/*半径初期値*/	/* 少しずつ広がるようにした */
		}
		else		//1つだと不安なので2つ作った。
		{
			c			= sprite_add_file0("cshoot2r.png"/*"cshoot2.png"*/,36,PR_PLAYER,1);
			d			= mmalloc(sizeof(PL_SHIELD_DATA));
			c->data 	= d;
			c->mover	= player_move_shield/*2*/;		//反時計回り
			d->speed	= -17;							//反時計回り
			d->radius	= 0/*45*/;	//d->radius=35; 	/*半径初期値*/	/* 少しずつ広がるようにした */
		}
		c->flags		|= SP_FLAG_VISIBLE;
		c->type 		= SP_PL_SHIELD;
		d->angle512 	= (ii*deg_360_to_512(45))/*i*/;
		/* 出始めはプレイヤー座標 */
		c->x			= player->x;//+co_s512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->w-s->w)/2);
		c->y			= player->y;//+si_n512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->h-s->h)/2);	//時計回り
		d->strength 	= 5;/* [***090214 追加 */
	}
}

static void player_add_levarie(SPRITE *s)	//魔理沙
{
	SPRITE *c;
	c=NULL;
	PL_LEVARIE_DATA *d;
	PLAYER_DATA *sd=(PLAYER_DATA *)s->data;
	sd->bonus_flag	|= BONUS_FLAG_02_AUTO_GET_ITEM;
	int i;
	int j;
	int k;
	if (MARISA==select_player)	{k=0;}else{k=6;}
	/*const*/ char *bbb_name[12]=
	{	// 魔理沙
		"star_shield_blue.png",
		"star_shields_red.png",
		"star_shield_green.png",
		"star_shields_blue.png",
		"star_shield_red.png",
		"star_shields_green.png",
		// チルノ
		"cirno_shield_blue.png",
		"cirno_shields_red.png",
		"cirno_shield_green.png",
		"cirno_shields_blue.png",
		"cirno_shield_red.png",
		"cirno_shields_green.png",
	};

	j=0;
	/*double*/int ang512;
	ang512 = rad2deg512(((M_PI*2)/32))/*0*/;/*チルノ用(暫定的)*/
	for (i=0;i<16;i++)
	{
		c				= sprite_add_file0((char *)bbb_name[(k+j)],3,PR_PLAYER,0);
		j++;if (6==j)	{j=0;}
		c->anim_speed	= 5;
		c->flags		|= SP_FLAG_VISIBLE;
		c->mover		= player_move_levarie;

		d				= mmalloc(sizeof(PL_LEVARIE_DATA));
		c->data 		= d;
		d->strength 	= 5;/* [***090214 追加 */
		d->angle512 	= ang512;
		if ((i&(2-1)/*%2*/)==0)
		{
			d->speed	= 1.0;
			c->alpha	= 150;
		}
		else
		{
			d->speed	= 1.2;
			c->alpha	= 180;
		}
		c->type 		= SP_PL_SHIELD;
		c->x			= s->x+((s->w-c->w)/2);
		c->y			= s->y+((s->h-c->h)/2);
		ang512 += rad2deg512((M_PI*2)/16) ; // 22.5度/360, π/8 /*0.39269908169872415481*/
	}
}

static void player_add_cross_red(SPRITE *s) /* おぜう */ /* [***090220 追加 */
{
	SPRITE *c;
	PL_CROSS_RED_DATA *d;
	PLAYER_DATA *sd = (PLAYER_DATA *)s->data;
	sd->bonus_flag	|= BONUS_FLAG_02_AUTO_GET_ITEM;
	c				= sprite_add_file0("cross_red.png",10,PR_PLAYER,0);
	c->flags		|= SP_FLAG_VISIBLE;
	d				= mmalloc(sizeof(PL_CROSS_RED_DATA));
	c->data 		= d;
	c->anim_speed	= 0;
	c->aktframe 	= 0;
	c->alpha		= 220;
	c->mover		= player_move_cross_red;
	c->type 		= SP_PL_SHIELD;
	c->x			= s->x+((s->w-c->w)/2);
	c->y			= s->y+((s->h-c->h)/2);
	d->strength 	= 3;
	d->angle512 	= (0);
}
#if (1==USE_PLAYER_HOMING)
#endif //(1==USE_PLAYER_HOMING)
static void player_add_hlaser(SPRITE *src)
{
	int j;
	j=(rand()&(2-1));//for (j=0;j<2;j++)
	{
		CONTROLLER *c;
		c=controller_add();
	//	hlaser_NUM_OF_ENEMIES=12;		//[***090128		半分にしてみる
		int *id_array;
		id_array	= mmalloc(sizeof(int)*hlaser_NUM_OF_ENEMIES);
		c->e		= id_array;
		c->con		= player_controller_hlaser;
//
//		/*int*/ SPRITE **id_array_bbb= (SPRITE **)c->e;
		int i;
		for (i=0;i<hlaser_NUM_OF_ENEMIES;i++)
		{
			SPRITE *s;	//	s=NULL;
			s				= sprite_add_res( BASE_TSHOOT_PNG+select_player );
			id_array[i] 	= s->id;
			s->type 		= SP_PL_HLASER;
			s->x			= (src->x+src->w/2)-5;
			s->y			= (src->y+src->h/2)+15;
			s->flags		|= SP_FLAG_VISIBLE;
			s->aktframe 	= 5-((double)6/hlaser_NUM_OF_ENEMIES)*i;
			if (i==0)
			{
				s->mover			= player_move_hlaser;
				PL_HOMING_DATA *data;
				data				= mmalloc(sizeof(PL_HOMING_DATA));
				s->data 			= data;
				data->nr			= j;
				data->angle512		= deg_360_to_512(100);
				data->speed 		= 12;
				data->strength		= 6;
				data->time_out/*range*/ 	= 100/*200*/;
				data->state 		= 0;
				data->target_obj	= NULL; 	//b->target_obj/*target_id*/	= /*-1*/NULL;
			}
			else
			{
				s->mover		= NULL;
				s->data 		= NULL;
			}
		}
	}
}
#undef hlaser_NUM_OF_ENEMIES

static void player_move_add_bomber_hlaser(SPRITE *s)
{
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;
	if (0==(((int)p->bomber_time)&0x3f))
	{
		player_add_hlaser(player);
	}
	s->aktframe 	= (((int)p->bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	s->x			= player->x+((player->w-s->w)/2);
	s->y			= player->y+((player->h-s->h)/2);
	if (p->bomber_time<=0)
	{	s->type=SP_DELETE;}
}
static void player_add_hlaser_cross_red(SPRITE *s)
{
	PLAYER_DATA *sd=(PLAYER_DATA *)s->data;
	sd->bonus_flag	|= BONUS_FLAG_02_AUTO_GET_ITEM;
	SPRITE *c;
	c				= sprite_add_file0("bomber_slow.png",4,PR_BACK1,1);
	c->flags		|= SP_FLAG_VISIBLE;
	c->data 		= 0/*d*/;
	c->anim_speed	= 0;
	c->aktframe 	= 0;
	c->alpha		= 128/*220*/;
	c->mover		= player_move_add_bomber_hlaser;
	c->type 		= SP_PL_SHIELD;
	c->x			= s->x+((s->w-c->w)/2);
	c->y			= s->y+((s->h-c->h)/2);
}

//static int /*bomb_wait*/d->bomber_time;		/* ボムの有効時間 */	//次のボムを出せるまでの時間
static int weapon_List; 	//どの武器を装備しているか

static void player_keycontrol(SPRITE *s, int hit_bomb_mode)
{
	PLAYER_DATA *d=(PLAYER_DATA *)s->data;
	//if player is invisible (state gameover) allow no keycontrol
	if ((!(s->flags&SP_FLAG_VISIBLE))||(d->state==PL_RESET))
	{	return;}


	if ((d->bossmode==B03_BEFORE_EVENT) || (d->bossmode==B06_AFTER_EVENT))
	{
		;	/* 会話イベント中はボム関連の処理はしない */
	}
	else
	{
	//	if (d->bomber_time>0)
		if (/*bomb_wait*/d->bomber_time>0)					//ボムウェイト処理
		{
			//	d->bomber_time-=fps_factor;
			/*bomb_wait*/d->bomber_time--/*-=fps_factor*/;/**/
			if (/*bomb_wait*/d->bomber_time<190)
			{	d->bonus_flag &= (~BONUS_FLAG_02_AUTO_GET_ITEM);}
		}
		else
		{
			d->bomber_time=0;
			d->extra_type=PLX_NONE;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (d->bombs>0)
			{
				if (my_pad & PSP_KEY_BOMB/*keybo ard[KEY_BOMB]*/)
				{
					#define KOUSOKU_MODE (0)
					#define TEISOKU_MODE (1)
					playChunk(7);
					int low=0;
					if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)
					{
						low=1;
					}
					switch ((select_player+select_player)+low)
					{
					case (REIMU*2+KOUSOKU_MODE):
					//	/*bomb_wait*/d->bomber_time 	= 200;
						d->bomber_time	= 200;			/* ボムの有効時間(設定無敵時間) */
					//	d->save_delay	= 200+30;		//設定無敵時間。たまにシールドをすり抜ける者が現れるので
					//	d->extra_type	= PLX_BOMB;
						player_add_shield(player);		//シールドの追加
						break;
					case (CIRNO*2+KOUSOKU_MODE):/*とりあえず*/
					case (MARISA*2+KOUSOKU_MODE):
					//	/*bomb_wait*/d->bomber_time 	= 100;
						d->bomber_time	= 100;			/* ボムの有効時間(設定無敵時間) */
					//	d->save_delay	= 100+30/*200 5*/;	//設定無敵時間。つーかさすがにボム時間中無敵じゃないのはきつい
					//	d->extra_type	= PLX_BOMB; 	/*←ここが無いと(extra_tem有効時に)ボムが発動出来ないよ*/
						player_add_levarie(player);
						break;

					case (REMIRIA*2+KOUSOKU_MODE):	/* [***090220 追加 */
					//	/*bomb_wait*/d->bomber_time 	= 150/*300*/ /*400*/;	/*400(150+250)は明らかに長すぎる*/
						d->bomber_time	= 150;
					//	d->save_delay	= 150+30;		//設定無敵時間。
					//	d->extra_type	= PLX_BOMB;
						player_add_cross_red(player);
						break;

					case (REMIRIA*2+TEISOKU_MODE):/*とりあえず*/
					case (REIMU*2+TEISOKU_MODE):/*とりあえず*/
					case (MARISA*2+TEISOKU_MODE):/*とりあえず*/
					case (CIRNO*2+TEISOKU_MODE):/*とりあえず*/
						d->bomber_time	= 400/*800*/;	/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
						/*player_move_add_bomber_hlaser*/player_add_hlaser_cross_red(player);
						break;
					#if (1==USE_PLAYER_HOMING)
					//	d->bomber_time	= 500/*1000*/;	/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
					//	/*player_move_add_bomber_homing*/player_add_homing_cross_red(player);
					//	break;
					#endif //(1==USE_PLAYER_HOMING)
					}
					/*	[※１] homing / hlaser は低速ボムになり武器使用中は無敵となったので、
						従来の有効時間では長すぎる。そこで有効時間を半分にした。*/
					#define USER_BOMOUT_WAIT (30)
					/*	[※２] 実質無敵時間 == (設定無敵時間 + USER_BOMOUT_WAIT)
						フレームスキップが入っていたり処理落ちしたりする関係上、
						グラが必ずしも描き終わっているとは限らない。
						もし描き終わっていない場合は、実質無敵時間 == 設定無敵時間 だと、
						画面にボムが表示されているのに判定で死ぬ。	*/
					/*	[※３]※２の余裕分を入れても、現在の状態ではボムの終わりが
						非常にわかりにくい。というか実質わからない。
						何らかの警告を組み入れる事が絶対必要。(だがまだ作ってない)
						(本家では色々警告がある) */
					d->save_delay		= d->bomber_time + USER_BOMOUT_WAIT;/*実質無敵時間*/
					d->extra_type		= PLX_BOMB;
					d->hit_bomb_wait	= 0;
					d->state			= PL_SAVE;
					d->bombs--;
				}
			}
		}
	}
//
	if (1==hit_bomb_mode) return;
//
	/*
	if (keybo ard[KEY_LEFT]) {
		direction=-1;
		if (s->x>0) {
			if (keybo ard[KEY_SLOW])
				s->x-=(d->player_speed/2)*fps_factor;
			else
				s->x-=d->player_speed*fps_factor;
		}
	}

	if (keybo ard[KEY_RIGHT]) {
		direction=1;
		if (s->x<GAME_WIDTH-s->w) { 	//プレイヤーのx座標 < スクリーンの横幅-プレイヤーの横幅
			if (keybo ard[KEY_SLOW])
				s->x+=(d->player_speed/2)*fps_factor;
			else
				s->x+=d->player_speed*fps_factor;
		}
	}

	if ((keybo ard[KEY_UP]) && (s->y>0)) {
		if (keybo ard[KEY_SLOW])
			s->y-=(d->player_speed/2)*fps_factor;
		else
			s->y-=d->player_speed*fps_factor;
		if (d->weapon==127) 	//128[***090126 	変更
			if (s->y<50)
				d->bonus=0x01;
		//parsys_add(NULL,50,0,s->x+s->w/2,s->y+s->h,10,90,10,100,PIXELATE,NULL);
	}

	if ((keybo ard[KEY_DOWN]) && (s->y<screen->h-s->h)) {
		if (keybo ard[KEY_SLOW])
			s->y+=(d->player_speed/2)*fps_factor;
		else
			s->y+=d->player_speed*fps_factor;
		if (s->y>50)
			d->bonus&=~0x01;
	}
	*/
	//int direction=0;



	static const signed /*int*/short shipMv[16][2] =
	{
	/*LDRU*/ /* y x */
	// 斜め移動が速過ぎるのを修正。
	/*0000*/ {0,0},
	/*0001*/ { 0, -256},/*0 U*/
	/*0010*/ { 256, 0},/*2 R*/
	/*0011*/ { 181, -181},/*1 UR*/
	/*0100*/ { 0, 256},/*4 D*/
	/*0101*/ {0,0},
	/*0110*/ { 181, 181},/*3 DR*/
	/*0111*/ {0,0},
	/*1000*/ {-256, 0},/*6 L*/
	/*1001*/ {-181, -181},/*7 UL*/
	/*1010*/ {0,0},
	/*1011*/ {0,0},
	/*1100*/ {-181, 181},/*5 DL*/
	/*1101*/ {0,0},
	/*1110*/ {0,0},
	/*1111*/ {0,0},
	};

	const unsigned int my_slow =(my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)?(1):(0); 	//値をシフトして半分にする？
	s->x += ((d->player_speed*(shipMv[((my_pad&0xf0)>>4)][0] >>my_slow))>>8) *fps_factor;
	s->y += ((d->player_speed*(shipMv[((my_pad&0xf0)>>4)][1] >>my_slow))>>8) *fps_factor;
	// はみだしたら修正。
		 if (s->x < 0)				{	s->x = 0;				}
	else if (s->x > GAME_WIDTH-s->w)	{	s->x = GAME_WIDTH-s->w; 	}
		 if (s->y < 0)				{	s->y = 0;				}
	else if (s->y > GAME_HEIGHT-s->h)	{	s->y = GAME_HEIGHT-s->h;	}
	// ??? (下記は、よくわかんないけど、だいたい互換のはず？)
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) /*&& (s->y>0)*/
	{
		if (d->weapon==127) //128[***090123 変更 /*max==127==「128段階」*/
		{
			if (s->y<50)
			{
				d->bonus_flag |= BONUS_FLAG_01_aaaa;
			}
		}
		//parsys_add(NULL,50,0,s->x+s->w/2,s->y+s->h,10,90,10,100,PIXELATE,NULL);
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/) /*&& (s->y<screen->h*-s->h)*/
	{
		if (d->bonus_flag & (BONUS_FLAG_01_aaaa /*| BONUS_FLAG_02_AUTO_GET_ITEM*/) )/*???たぶん*/
		{
			d->bonus_flag &= (~BONUS_FLAG_01_aaaa);
		}
	}
//

	if ( (d->bossmode!=B03_BEFORE_EVENT) && (d->bossmode!=B06_AFTER_EVENT))
	{
		if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
		{
			{static int weapon_interval=0;
				if (/*d->*/weapon_interval>0)
				{	/*d->*/weapon_interval--/*=fps_factor*/;}
				else //if (/*d->*/weapon_interval<=0)
				{
					playChunk(0);
					switch (select_player)
					{
					case REIMU:
						switch (weapon_List)
						{
						case WP_SINGLE: 	player_add_fuda(s,REIMU_SHOT_TYPE_00);			/*d->*/weapon_interval=5;		break;
						case WP_DOUBLE: 	player_add_doublefuda(s);		/*d->*/weapon_interval=5;		break;
						case WP_TRIPLE: 	player_add_triplefuda(s);		/*d->*/weapon_interval=5;		break;
						case WP_QUAD:		player_add_quadfuda(s); 		/*d->*/weapon_interval=5;		break;
						case WP_FIFTH:		player_add_fifthfuda(s);		/*d->*/weapon_interval=5;		break;
						#if (1==USE_PLAYER_WAVE)
						case WP_WAVE:		player_add_killray(s);			/*d->*/weapon_interval=5;		break;
						#endif //(1==USE_PLAYER_WAVE)
						}
						break;
					case CIRNO:/*とりあえず*/
					case MARISA:
						switch (weapon_List)
						{
						case WP_SINGLE: 	player_add_star(s); 			/*d->*/weapon_interval=6;		break;
						case WP_DOUBLE: 	player_add_doublestar(s);		/*d->*/weapon_interval=6;		break;
						case WP_TRIPLE: 	player_add_doublestar(s);		/*d->*/weapon_interval=5;		break;
						case WP_QUAD:		player_add_triplestar(s);		/*d->*/weapon_interval=5;		break;
						case WP_FIFTH:		player_add_quadstar(s); 		/*d->*/weapon_interval=5;		break;
						#if (1==USE_PLAYER_WAVE)
						case WP_WAVE:		player_add_killray(s);			/*d->*/weapon_interval=5;		break;
						#endif //(1==USE_PLAYER_WAVE)
						}
						break;
					case REMIRIA:
						switch (weapon_List)
						{
						case WP_SINGLE: 	player_add_thing(s);			/*d->*/weapon_interval=7;		break;
						case WP_DOUBLE: 	player_add_thing(s);			/*d->*/weapon_interval=6;		break;
						case WP_TRIPLE: 	player_add_doublething(s);		/*d->*/weapon_interval=7;		break;
						case WP_QUAD:		player_add_doublething(s);		/*d->*/weapon_interval=6;		break;
						case WP_FIFTH:		player_add_doublething(s);		/*d->*/weapon_interval=5;		break;
						#if (1==USE_PLAYER_WAVE)
						case WP_WAVE:		player_add_killray(s);			/*d->*/weapon_interval=5;		break;
						#endif //(1==USE_PLAYER_WAVE)
						}
						break;
					}
				}
			}
			#if 0
//			{static int extra_interval = 0;
//				if (/*d->*/extra_interval>0)
//				{	/*d->*/extra_interval--/*=fps_factor*/;}
//				else //if (d->extra_interval<=0)
//				{
//					switch (d->extra_type)
//					{
//					case PLX_HOMING:		player_add_homing(s);		/*d->*/extra_interval=63/*60*/; 			break;
//					case PLX_HLASER:		player_add_hlaser(s);		/*d->*/extra_interval=63/*60*/; 			break;
//					}
//				}
//			}
			#endif
		}
	}

	d->anim_delay -= 1/*fps_factor*/;
	if (0 > d->anim_delay)
	{
		d->anim_delay = player_status[BASE_SPEED_ANIME+select_player]/*2*/;
		//
		static int auto_aktframe=4/*5*/;
			 if (my_pad & PSP_KEY_LEFT/*PSP_CTRL_LEFT*/)	{	if (auto_aktframe>0   )    auto_aktframe--; }
		else if (my_pad & PSP_KEY_RIGHT/*PSP_CTRL_RIGHT*/)	{	if (auto_aktframe</*7*/8/*10*/) auto_aktframe++; }
		else
		{
			if (auto_aktframe>4/*5*/) auto_aktframe--;
			if (auto_aktframe<4/*5*/) auto_aktframe++;
		}
		{
			static int auto_aniime;
			auto_aniime++;
			auto_aniime&=3;
				 if (4==auto_aktframe)	{	s->aktframe = (auto_aktframe + auto_aniime);	}
			else if (4<auto_aktframe)	{	s->aktframe = (auto_aktframe + 3);				}
			else						{	s->aktframe = (auto_aktframe ); 				}
		}
	}
}
/*
0 1 2 3 4 5 6  7  8
0 1 2 3 4 8 9 10 11
		5
		6
		7
*/

static void option_check(PLAYER_DATA *d)
{
	if (d->weapon>35-1)
	{
		if (!d->option)
		{
			switch (select_player)
			{
			case REIMU: 		re_add_option(player);		d->option=1;		break;
			case MARISA:		ma_add_option(player);		d->option=1;		break;
			case CIRNO:/*とりあえず*/
			case REMIRIA:		oz_add_option(player);		d->option=1;		break;/* [***090220 追加 */
			}
		}
	}
	else
	{	d->option=0;}
}

static void weapon_check(int w) 	//weaponの段階から今の装備を決める
{									//[***090123		大幅な変更。最大128へ。
	if (w<=5-1) 			weapon_List=WP_SINGLE;
	else if (w<=10-1)		weapon_List=WP_DOUBLE;
	else if (w<=60-1)		weapon_List=WP_TRIPLE;
	else if (w<=85-1)		weapon_List=WP_QUAD;
	else /*if (w<=128-1)*/	weapon_List=WP_FIFTH;/*max==127==「128段階」*/
	//else					weapon_List=WP_WAVE;
}

//グレイズ用、弾id格納場所。
//1つにしなかったのは複数の弾を同時にグレイズしたときのため。
//static int graze_check[5]={-1,-1,-1,-1,-1};

static int weapon_chain;		//どれだけ連続でwepon_upを取ったか

static void player_colcheck(SPRITE *s, int mask)
{
	//is_graze=0;
	/*
		s		プレイヤー
		mask	調べる対象？
				敵か敵の弾かアイテムか
	*/
	SPRITE *c;	//対象
	//SDL_Surface *spimg;
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;

	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	if ((c=sprite_colcheck(s,mask))!=NULL)
	{
		switch (c->type)
		{
		case SP_BONUS_00_FIREPOWER:
			if (pd->weapon<127) 	//128maxでないとき /*max==127==「128段階」*/
			{
				pd->weapon++;
				bonus_info_add(c->x,c->y,"weapon.png");
			}
			else					//maxの時
			{
				weapon_chain++;
				//ウェポンボーナス得点の表示
				bonus_info_any_score(c,(weapon_chain));
			}
			weapon_check(pd->weapon);
			option_check(pd);
			c->type=SP_DELETE;
			playChunk(5);
			break;

		case SP_BONUS_04_FIREPOWER_128: 	//ウェポンアイテム(F)		//[***090123		追加
			goto my_128;
		case SP_BONUS_01_FIREPOWER_G:		//ウェポンアイテム(中)		//[***090123		追加
			if (pd->weapon<(120-1))
			{
				pd->weapon+=8;
				bonus_info_add(c->x,c->y,"weapon.png");
			}
			else
			{
my_128:
				if (pd->weapon==127)/*127==128段階目*/
				{
					weapon_chain+=8;
					//ウェポンボーナス得点の表示
					bonus_info_any_score(c,(weapon_chain));
					/*??weapon_check(pd->weapon);*/
				}
				else
				{
					pd->weapon=127;/*max==127==「128段階」*/
				}
			}
		//	weapon_check(pd->weapon);
			weapon_check(pd->weapon);
			option_check(pd);
			c->type=SP_DELETE;
			playChunk(5);
			break;

		case SP_BONUS_02_BOMB:
			//player_add_bomb(c);		//[*****本来はコメントアウトしないよ
			if (8 < pd->bombs)	{	goto add_1000pts;	}
			pd->bombs++;
			bonus_info_add(c->x,c->y,"firebomb.png");
			c->type=SP_DELETE;
			playChunk(5);
			break;

		case SP_BONUS_03_HEALTH:
			if (8 < pd->lives)	{	goto add_1000pts;	}
			pd->lives++;
			bonus_info_add(c->x,c->y,"energy.png");
			c->type=SP_DELETE;
			playChunk(6);
			break;

		case SP_BONUS_07_ADDSPEED:
			if (pd->player_speed < player_status[BASE_SPEED_MAXIMUM+select_player]/*pd->player_speed_maximum*/)
			{	;	}	else	{	goto add_1000pts;	}
			pd->player_speed++;
			bonus_info_add(c->x,c->y,"speed.png");
			c->type=SP_DELETE;
			playChunk(5);
			break;

add_1000pts:
		case SP_BONUS_05_HOSI:
		case SP_BONUS_06_COIN:
			if ( (GAME_HEIGHT/2) > c->y )	/* 上で取ると 1000pts. */
			{
				pd->score+=score(1000);
				bonus_info_add(c->x,c->y,"plus1000.png");
			}
			else	/* 下で取ると 1000pts. */
			{
				pd->score+=score(100);		//[***090123		変更
				bonus_info_add(c->x,c->y,"plus100.png");		//[***090123		変更
			}
			c->type=SP_DELETE;
			playChunk(5);
			break;


//add_1000pts:
//			pd->score+=score(1000);
//			bonus_info_add(c->x,c->y,"plus1000.png");
//			c->type=SP_DELETE;
//			break;

			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
			//SDL_FreeSurface(spimg);

		#if 0
		case SP_BONUS_EXTRA_SHIELD: 	//ウェポンアイテム(強)
			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //ボム中はこれを取っても1000ポイント入るだけだよ
			//
		//	sprite_remove_all_type(SP_PL_SHIELD);
		//	pd->extra_type=PLX_NONE/*PLX_SHIELD*/;		//ウェポンアイテム(強)
		//	pd->bomber_time=30;
			if (pd->weapon<(112-1)) 	//[***090123		変更
			{
				pd->weapon+=16;
				bonus_info_add(c->x,c->y,"weapon.png");
			}
			else
			if (pd->weapon==127)	//[***090123		変更/*max==127==「128段階」*/
			{
				weapon_chain+=16;
				goto add_1000pts;
			}
			else
			{
				pd->weapon=127; 		//[***090123		変更/*max==127==「128段階」*/
			}
			weapon_check(pd->weapon);
			option_check(pd);
			c->type=SP_DELETE;
			playChunk(5);
			break;
		#endif

		#if 0
//		case SP_BONUS_EXTRA_HOMING:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //ボム中はこれを取っても1000ポイント入るだけだよ
//			//
//		//	sprite_remove_all_type(SP_PL_SHIELD);
//			pd->extra_type		= PLX_BOMB/*PLX_HOMING*/;
//			pd->bomber_time 	= 800;
//			bonus_info_s_text(c, "homing missiles",FONT04);
//			bonus_info_add(c->x,c->y,"extra.png");
//			//spimg=sprite_getcurrimg(c);
//			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			c->type=SP_DELETE;
//			playChunk(5);
//			break;
//
//		case SP_BONUS_EXTRA_HLASER:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //ボム中はこれを取っても1000ポイント入るだけだよ
//			//
//		//	sprite_remove_all_type(SP_PL_SHIELD);
//			pd->extra_type		= PLX_BOMB/*PLX_HLASER*/;
//			pd->bomber_time 	= 1000;
//			bonus_info_s_text(c, "homing laser",FONT04);
//			bonus_info_add(c->x,c->y,"extra.png");
//			//spimg=sprite_getcurrimg(c);
//			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			c->type=SP_DELETE;
//			playChunk(5);
//			break;
		#endif


		#if 0
		case SP_EN_BOSS01:		enemy_boss01_hitbyplayer(c);	break;
		case SP_EN_BOSS02:		enemy_boss02_hitbyplayer(c);	break;
		case SP_EN_BOSS03:		enemy_boss03_hitbyplayer(c);	break;
		case SP_EN_BOSS04:		enemy_boss04_hitbyplayer(c);	break;
		#else
		case SP_EN_BOSS:		(((PLAYER_DATA *)player->data)->callback_boss_hitbyplayer)(c);	break;
		#endif
		case SP_EN_BULLET:
			is_graze=1;
//			int i;
//			for (i=0;i<5;i++)
//			{
//				if (graze_check[i]==((BUL LET_DATA *)c->data)->id)	//同じ場合
//					break;											//forを抜ける
//			}
			if (0==(c->flags & SP_FLAG_GRAZE)) /* グレイズ済? */ /* i==5 同じIDの弾が無かった時*/
			{
				c->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
				pd->graze++;/*グレイズ数*/
				pd->score += (score(100)+score(200)*difficulty);
//				graze_check[(pd->graze)%5]=((BUL LET_DATA *)c->data)->id;
				playChunk(9);
			}
			break;

		case SP_EN_LASER:
			is_graze=1;
//			int j;
//			for (j=0;j<5;j++)
//			{
//				if (graze_check[j]==((LASER_DATA *)c->data)->id)	//同じ場合
//					break;											//forを抜ける
//			}
			if (0==(c->flags & SP_FLAG_GRAZE)) /* グレイズ済? */ /* if (j==5) 同じIDの弾が無かった時*/
			{
				c->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
				pd->graze++;/*グレイズ数*/
				pd->score += (score(100)+score(200)*difficulty);
//				graze_check[(pd->graze)%5]=((LASER_DATA *)c->data)->id;
				playChunk(9);
			}
			break;

		case SP_EN_BIGBULLET:
			is_graze=1;
//			int l;
//			for (l=0;l<5;l++)
//			{
//				if (graze_check[l]==((BIGBUL LET_DATA *)c->data)->id)	//同じ場合
//					break;											//forを抜ける
//			}
			if (0==(c->flags & SP_FLAG_GRAZE)) /* グレイズ済? */ /* if (l==5) 同じIDの弾が無かった時*/
			{
				c->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
				pd->graze++;/*グレイズ数*/
				pd->score += (score(100)+score(200)*difficulty);
//				graze_check[(pd->graze)%5]=((BIGBUL LET_DATA *)c->data)->id;
				playChunk(9);
			}
			break;

		case SP_EN_BIGBULLET_DUMMY: 	//2つ目のスプライトは当たり判定が無い
			break;

		default:			//敵そのものに当たった場合
		//	spimg=sprite_getcurrimg(c);
		//	parsys_add(spimg, 2,2, c->x,c->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
		//	spimg=NULL;
			pd->state=PL_HIT_BOMB;		//[***090125		追加
			pd->enemy=c;
			//explosion_add(c->x+5,c->y+5,0,EXPLOSION_MINI00);		//[***090125		以下コメントアウト
			//pd->explode=0;

			//spimg=sprite_getcurrimg(s);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, 90, 10, 30, EXPLODE|DIFFSIZE, &pd->explode);
			//SDL_FreeSurface(spimg);
			//spimg=NULL;
			//c->type=SP_DELETE;
			//s->flags&= ~SP_FLAG_VISIBLE;
			//pd->core->flags&= ~SP_FLAG_VISIBLE;	//○も消す
			//pd->lives--;
			//pd->bonus=0;
			playChunk(4);		//[***090127	変更先
		}
	}
}

static void player_move(SPRITE *s1)
{
	PLAYER_DATA *d=(PLAYER_DATA *)s1->data;
	switch (d->state)
	{
	case PL_NORMAL:/*通常処理*/
		player_keycontrol(s1, 0);
		player_colcheck(s1,SP_SHOW_ENEMYS|SP_SHOW_ENEMY_WEAPONS|SP_SHOW_BONUS);
		break;

	case PL_HIT_BOMB:/* 喰らいボム受付中 */ 				//[***090125		追加
		if (player_status[BASE_HIT_BOMB_WAIT+select_player] < d->hit_bomb_wait) /*<6*/
		{
			d->hit_bomb_wait=0;
			/* 死亡確定 */
			d->state			= PL_EXPLODE;
		}
		else
		{
			d->hit_bomb_wait++;
			/* 喰らいボム受付中 */
			player_keycontrol(s1, 1);
		}
		break;

	case PL_EXPLODE:/*プレイヤー爆発中*/
	//	if (d->lives<0) {	;	}else
		{
			//ここでアイテムを吐き出すようにすればそれっぽくなるかも
			/* GAMEOUTでもアイテム吐くべき(最後だけ吐かないのは違和感がある) */
			bonus_multi_add(s1, SP_BONUS_00_FIREPOWER, 5, BONUS_FLAG_RAND_XY);//というわけで実装
			//[***090124ちょっと追加
			bonus_multi_add(s1, ((d->weapon>(30-1))?(SP_BONUS_01_FIREPOWER_G):(SP_BONUS_04_FIREPOWER_128)), 1, BONUS_FLAG_RAND_XY);
			/* 極端に弱くて死んだ場合の救済策: 上手い人は序盤で FULLPOWER になり、死んでもボム回収などで POWER を維持するので、 */
			/* 極端に弱くては死なない。 極端に弱くて死ぬのは結構下手だと思うので、その場合 FULLPOWERUP にした。 */
		}
//			d->explode			= 0;
//		if (0==d->explode)
		{
			//playChunk(4); 	//[***090127	場所を移動する。
			d->enemy->type		= SP_DELETE;/*???*/
			s1->flags			&= ~SP_FLAG_VISIBLE;			//可視フラグのOFF(不可視)
			d->core->flags		&= ~SP_FLAG_VISIBLE;			//○消し
			d->extra_type		= PLX_NONE;
			d->bomber_time		= 0;			/* ←ここが無いせいで魔理沙(&チルノ)ボム撃てなくなってた */
			d->bonus_flag		= BONUS_FLAG_00_NONE;

			explosion_add(d->enemy->x+5,d->enemy->y+5,0,EXPLOSION_MINI00);
			explosion_add(s1->x+5,s1->y+5,0,EXPLOSION_MINI00);
			explosion_add(s1->x+5,s1->y+20,0,EXPLOSION_MINI00);
			explosion_add(s1->x+20,s1->y+5,0,EXPLOSION_MINI00);
			explosion_add(s1->x+20,s1->y+20,0,EXPLOSION_MINI00);
			d->save_delay		= 150;						//無敵時間
			d->state			= PL_RESET; 				//無敵状態？


			d->lives--;
			if (d->lives<0)
			{
				;/* GAMEOUT中 */
			}
			else
			{
				s1->flags			|= SP_FLAG_VISIBLE; 		//可視フラグのON(可視)
				d->core->flags		|= SP_FLAG_VISIBLE; 		//○表示
				//
				d->bombs = player_status[BASE_BOMBS+select_player]/*3*/;						//ボム所持数の初期化
				if (d->player_speed > player_status[BASE_SPEED_MINIMUM+select_player]/*d->player_speed_minimum*/)				//player_speed_minimum以下にならないように
				{	d->player_speed--;}
				d->weapon -= (difficulty*4)+12; 	//武器レベルの低下
				if ( d->weapon<0)
				{	d->weapon=0;}	//denis - reset weapons
				weapon_check(d->weapon);			//リストを見て装備の変更
				weapon_chain=0; 					//weapon_chainの初期化
			}
			option_check(d);
			s1->x=GAME_WIDTH/2-s1->w/2; 			//プレイヤー位置の初期化
			s1->y=GAME_HEIGHT;
		}
		break;

	case PL_RESET:/*プレイヤー復活中１(画面外から復活地点へ点滅しながら移動中)*/
		if (d->save_delay>120)	//ちょっとした待ち時間
		{	d->save_delay-=fps_factor;}
		else
		{
			if (s1->y<220/*240*/)	//ちょっと前に出てくる
			{
				d->state=PL_SAVE;
			}
			else
			{
				s1->y-=fps_factor;
			//	if (((int)(s1->y/10)&(2-1)/*%2*/)==0)		//点滅するかなと思って入れてみたけどしなかったよ→処理落ちしてた。
				if (((unsigned int)(s1->y)&((2*8)-1))==0)	//点滅してるよ
				{
					s1->alpha		= 80;
					d->core->alpha	= 80;
				}
				else
				{
					s1->alpha		= 0/*255*/;
					d->core->alpha	= 0/*255*/;
				}
			}
		}
		break;/*??? [***090210 追加 */

	case PL_SAVE:/*プレイヤー復活中２*/
		player_keycontrol(s1, 0);
		player_colcheck(s1,SP_SHOW_BONUS);
		if (d->save_delay>0)
		{
			d->save_delay-=fps_factor;
			s1->alpha=80;
			if (d->lives<0) 	//ゲームオーバー時の○の表示処理
			{	d->core->alpha=0;}
			else
			{	d->core->alpha=80;}
		}
		else
		{
			d->state		= PL_NORMAL;
			s1->alpha		= 255;
			d->core->alpha	= 255;
		}
		break;
	}
}


void player_init(void)
{
	PLAYER_DATA *data;
	player				= sprite_add_res( BASE_SHIP_MED_PNG+select_player );		/* [***090220 追加 */
	player->type		= SP_PLAYER;
	player->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	player->mover		= player_move;
	player->x			= GAME_WIDTH/2;
	player->y			= GAME_HEIGHT-player->h;
	player->aktframe	= 5;
	data				= mmalloc(sizeof(PLAYER_DATA));
	player->data		= data;
//	data->player_speed_maximum	= player_status[BASE_SPEED_MAXIMUM+select_player];
//	data->player_speed_minimum	= player_status[BASE_SPEED_MINIMUM+select_player];
	data->player_speed			= player_status[BASE_SPEED_MINIMUM+select_player]/*data->player_speed_minimum*/;

	data->bossmode		= B00_NONE;
	data->state 		= PL_RESET/*PL_SAVE*/;
	data->save_delay	= 120;
	data->anim_delay	= 0;
	data->weapon		= 0;
	data->extra_type	= PLX_NONE;
	data->bomber_time	= 0;	/*==bomb_wait*/
//	data->explode		= 0;
//	data->weapon_interval	= 0;
//	data->extra_interval	= 0;

	data->graze 		= 0;
	data->lives 		= player_status[BASE_LIVES+select_player];
	data->bombs 		= player_status[BASE_BOMBS+select_player];
	data->score 		= score(0);
	data->bonus_flag	= BONUS_FLAG_00_NONE;
	data->hit_bomb_wait = 0;	//[***090125		追加
	data->option		= 0;
	data->boss			= NULL;
	data->core			= player_add_core(player); //○の追加

	weapon_List 		= 0;
	weapon_chain		= 0;
}
