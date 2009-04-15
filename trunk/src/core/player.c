#include "player.h"

#define rand_percent(aaa) ((unsigned char)(rand())<=(unsigned char)( (aaa*256)/100 ) ) /* aa%の確率で */

extern SPRITE *sprite;
extern int select_player;


int player_now_stage;		//
SPRITE *player;

enum _player_state
{
	PL_NORMAL,
	PL_HIT_BOMB,		// [***090125		追加:PL_HIT_BOMB
	PL_EXPLODE,
	PL_RESET,
	PL_SAVE
};

//typedef struct
//{
//	int strength;/*union WEAPON_BASE*/
//	int angle512;
//	double speed;
//} PLAYER_BASE_STRUCT;


typedef struct
{
	int strength;/*union WEAPON_BASE*/
	/*double*/double/*int*/ vx; 	/* vector x */
	/*double*/double/*int*/ vy; 	/* vector y */
} PL_SHOT_DATA;

#define PL_LEVARIE_DATA    PL_SHOT_DATA/*PLAYER_BASE_STRUCT*/
#define OZ_BURN_FIRE_DATA  PL_SHOT_DATA/*PLAYER_BASE_STRUCT*/

/* 十字炎本体 の強さの為だけに必要 */
#define PL_CROSS_FIRE_DATA	/*PL_SHOT_DATA*/WEAPON_BASE/*PLAYER_BASE_STRUCT*/

/*
注意：十字炎。別の名称で呼びたくなるかも知れませんが、某宗教団体がうるさいので、この名称でよんでくだしあ。
しかし、なんて読むんだろ？
*/


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
//	SPRITE *tg_sprite;/* ターゲット目標表示マーカー */	/*PL_HOMING_DATAのみ*/
} PL_HOMING_DATA;/*==PL_HLASER_DATA*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	/*double*/int angle512;
	double speed;
	int radius; 	/*半径(radius)*/
} PL_SHIELD_DATA;

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	/*double*/int angle512;
	int ani_speed;
	int option_id/*pos*/;/*option_id*/
	int w_wait;
//
//	int length;
//	int state;
} PL_OPTION_DATA;

#define OPTION_ID_99_LEFT	(-1)/*旧-1 0*/
#define OPTION_ID_01_RIGHT	( 1)/*旧+1 1*/

typedef struct
{
	int strength;/*union WEAPON_BASE*/	/* [***090214 追加 */
	/*double*/int angle512;
	int ani_speed;
	int option_id;
	int w_wait;
//
//	int state1;//共通なので、ここには要らない
	int state2;
	int offset_x;		/* 円の半径と兼用 */
	int offset_y;
	/*double*/int f_angle512;	/* レミの回りを回る場合の、回転角度 */	/* FORMATION_01: レミの周りを回るよ */

	int slow_count;
//	/*double*/int m_angle512;	/* 廃止 */
//	struct _sprite *next;
} REMILIA_OPTION_DATA;

	/*---------------------------------------------------------
		各プレイヤーの性能差
	---------------------------------------------------------*/

#define BASE_BOMBS			(4*0)
#define BASE_HIT_BOMB_WAIT	(4*1)
#define BASE_LIVES			(4*2)
#define BASE_SPEED_ANIME	(4*3)
#define BASE_SPEED_MINIMUM	(4*4)
#define BASE_SPEED_MAXIMUM	(4*5)
#define BASE_MAX			(4*6)
static Uint8 player_status[BASE_MAX]=
{/* REIMU MARISA REMILIA CIRNO */
	 3,  3,  3,  3, 	/* 復活時のボム数 */
	16,  8,  4,  9, 	/* 喰らいボムの受付時間 hit_bomb_wait */
	 3,  4,  2,  9, 	/* 開始時の残りチャンス */
	 4,  2,  3,  1, 	/* アニメーション速度 */

	 2,  3,  4,  9, 	/* 最低速度 player_speed_minimum */
	 4,  6,  4,  9, 	/* 最高速度 player_speed_maximum */
};
//	 3,  2,  1,  9, 	/* 復活時のボム数 */

//	 3,  4,  4,  9, 	/* 最低速度 player_speed_minimum */
//	 6,  8,  7,  9, 	/* 最高速度 player_speed_maximum */
//	 3,  4,  4,  9, 	/* 最低速度 player_speed_minimum */
//	 6,  8,  4,  9, 	/* 最高速度 player_speed_maximum */

/*---------------------------------------------------------
	プレイヤー、生死判定用コア、生死判定
---------------------------------------------------------*/

static int is_graze;	//player1が弾幕に触れたか？

static void player_move_core(SPRITE *s2)
{
	s2->x=player->x+((player->w-s2->w)>>1)-1;
	s2->y=player->y-s2->h+20;
	if (is_graze)	//プレイヤースプライトが弾に触れているか
	{
		if (((PLAYER_DATA *)player->data)->state==PL_NORMAL)
		{
			SPRITE *c;
			if ((c=sprite_colcheck(s2,SP_SHOW_ENEMY_WEAPONS))!=NULL)
			{
				/* 必要のない限り弾の種類を増やさないで下さい。遅くなります。 */
				/* 現状では表示プライオリティぐらいしか必要なケースが無いので。 */
				switch (c->type)	//各弾の処理。弾の種類を追加した場合。ここにも追加する必要あり。
				{
				case SP_EN_BULLET:				/*not_break;*/
			//	case SP_EN_LASER:				/*not_break;*/
			//	case SP_EN_BIGBULLET:			/*not_break;*/
			//	case SP_EN_BOSS02ICE:			/*not_break;*/
					playChunk(4);		// [***090127	変更先
					((PLAYER_DATA *)player->data)->state=PL_HIT_BOMB;
					((PLAYER_DATA *)player->data)->enemy=c;/* あたった敵を覚えとく */
					break;
				}
			}
		}
	}
	is_graze=0;
}


/*---------------------------------------------------------
	プレイヤー武器のあたり判定
---------------------------------------------------------*/

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
		case SP_EN_BOSS04:	if (s->type!=SP_PL_SHIELD)	{	enemy_boss04_hitbyweapon(c,s/*,angle*/);	}	break;	// [***090127		追加
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
				switch (c->type)	// [***090123		追加
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
				bonus_info_any_score(c, ((d->score)*(/*pd->now_stage*/player_now_stage/*level*/)) );
			}
			//SDL_FreeSurface(spimg);
			break;
		}
	}
}

/*---------------------------------------------------------
	プレイヤー、針弾の移動
---------------------------------------------------------*/

static void player_move_needle(SPRITE *s) // 霊夢 ＆ 魔理沙 ＆ チルノ ＆ おぜう
{
	PL_SHOT_DATA *data=(PL_SHOT_DATA *)s->data;
	s->x += data->vx;//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	s->y += data->vy;//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	if (REMILIA==select_player)
	{	int rand_int;
		rand_int=rand();
		s->x += (rand_int&1);
		s->x -= ((rand_int&2)>>1);
	}
	if (s->y < -10)
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
}

/*---------------------------------------------------------
	プレイヤー、ショット弾の移動
---------------------------------------------------------*/
#define player_move_shot player_move_needle
//static void player_move_shot(SPRITE *s)	// 霊夢 ＆ 魔理沙 ＆ チルノ ＆ おぜう
//{
//	PL_SHOT_DATA *data=(PL_SHOT_DATA *)s->data;
//	s->x += data->vx;//co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
//	s->y += data->vy;//si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
//	if (s->y < -10)
//	{
//		s->type=SP_DELETE;
//	}
//	else
//	{
//		weapon_colcheck(s,/*data->angle512,*/1,0);
//	}
//}


#if (1==USE_PLAYER_WAVE)
static void player_move_killray(SPRITE *s)		//使わない
{
	PL_KILLRAY_DATA *data=(PL_KILLRAY_DATA *)s->data;
	s->x+=co_s512((data->angle512))*data->speed*fps_fa_ctor;
	s->y+=si_n512((data->angle512))*data->speed*fps_fa_ctor;
	data->distance+=data->speed*fps_fa_ctor;
	if ((data->distance>20)&&(s->aktframe<11))
	{
		data->distance=0;
		s->aktframe++;
	}
	if (s->y<-10)
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
}
#endif //(1==USE_PLAYER_WAVE)

#if (1==USE_PLAYER_BOMBER)
/* ここは将来スペカになる予定 */
static void player_move_bomb(SPRITE *s)
{
	static int k=5;
	PL_BOMB_DATA *data=(PL_BOMB_DATA *)s->data;
	s->x+=co_s512((data->angle512))*data->speed*fps_fa_ctor;
	s->y+=si_n512((data->angle512))*data->speed*fps_fa_ctor;
	if ((s->x<0-100)||(s->x>100+GAME_WIDTH)||
		(s->y<0-100)||(s->y>100+GAME_HEIGHT))
	{
		s->type=SP_DELETE;
	}
	else
	{
		weapon_colcheck(s,/*data->angle512,*/1,0);
	}
	if (k>0)	{k--;}else{data->speed++;k=5;}
}
#endif //(1==USE_PLAYER_BOMBER)

/*---------------------------------------------------------
	敵を探す子関数
---------------------------------------------------------*/
static /*int*/SPRITE *search_enemy_by_sprite(void)
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s=sprite;/* スプライト リストの先頭 から探す */
	while (NULL != s)/* スプライト リストの最後まで調べる */
	{
		if ((s->type&SP_SHOW_ENEMYS) && 		/* プレイヤーにとっての敵(ザコやボス) */
			(!(s->flags&SP_FLAG_PARANOIA)) &&	/* 同一フラグが無い敵(別の誘導弾が見つけていない敵) */
			(s->flags&SP_FLAG_VISIBLE)) 		/* 使用中の敵 */
		{
			if ((s->x>0) && (s->x<GAME_WIDTH-s->w) &&
				(s->y>0) && (s->y<GAME_HEIGHT-s->h))	/* 表示画面内の敵 */
			{
				return s/*->id*/;/*見つけたよ*/
			}
		}
		s=s->next;
	}
	return NULL/*-1*/;/*見つからなかった*/
}

/*---------------------------------------------------------
	プレイヤー、誘導弾の移動
---------------------------------------------------------*/

#if (1==USE_PLAYER_HOMING)
//static void player_move_homing(SPRITE *s) {}
#endif //(1==USE_PLAYER_HOMING)
static void player_move_hlaser(SPRITE *s)
{
	PL_HOMING_DATA *data;
	data=(PL_HOMING_DATA *)s->data;
//
	SPRITE *target;
	target=NULL;	/* Target */
//	SPRITE *tg;
//	tg=(SPRITE *)data->tg_sprite; /* Target-Fadenkreuz */	/* ターゲット目標表示マーカー */

	switch (data->state)
	{
	case 0: /* 回転しながら正面を向く。 / Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
		if (data->nr==0)	{	data->angle512+=deg_360_to_512(8)/**fps_fa_ctor*/; }	/* 右回りに回転させる。/ linke Rakete: rot. im UZ */
		else			{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/; }	/* 左回りに回転させる。/ rechte Rekete: rot. gegen UZ */
		//
		#if 0
		if (data->angle360<0)		{	data->angle360+=360;	}
		if (data->angle360>360) {	data->angle360-=360;	}
		#else
		mask512(data->angle512);
		#endif
		//
		if ( (1.0) < data->speed )
		{
			data->speed -= /*(int)*/(0.5);/*減速*/
		}
		//
		if ((data->angle512>=deg_360_to_512(270-4/*266*/))&&
			(data->angle512<=deg_360_to_512(270+4/*274*/))) /* ほぼ正面(270)を向いたら */
		{
			/* Sprengkopf in Ausgangspos? */
			data->state=2;						/* 次へ */
			data->angle512=deg_360_to_512(270); /* 向きを正面(270)に固定する */
		}
		break;
	case 2: /* 敵を探す。 / Ziel suchen */
		data->target_obj/*target_id*/=search_enemy_by_sprite();
		if (data->target_obj/*target_id*/ != NULL/*-1*/)
		{
			target=data->target_obj/*sprite_get_by_id (data->target_id)*/;
			if (target!=NULL)
			{
				data->state=3;						/* 次へ */
				target->flags|=SP_FLAG_PARANOIA;	/* 「一つしかないですよ」フラグON */
			//	tg->flags|= SP_FLAG_VISIBLE;	/* ターゲット目標表示マーカー */
			}
		//	else
		//	{
		//	//	tg->flags&= ~SP_FLAG_VISIBLE;	/* ターゲット目標表示マーカー */
		//	}
		}
		break;
	case 3: /* Ziel verfolgen */
		if (/*data->target_id*/data->target_obj == NULL/*-1*/)	/* 見つからない */
		{
		//	/* ...orz ありえねえ */ error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
			data->state=2;	/* 仕方なく戻る */
		}
		else
		{
			if ( (12.0) > data->speed)
			{
				data->speed += /*(int)*/(0.5);/*加速*/
			}
			target=data->target_obj/*sprite_get_by_id (data->target_id)*/;
			if (//(target!=NULL) && 	/* 重複してるので排除 */
				(target->flags&SP_FLAG_VISIBLE) &&
				(target->x>0) &&
				(target->x<GAME_WIDTH-target->w) &&
				(target->y>0) &&
				(target->y<GAME_HEIGHT-target->h))
			{
				{int ta512;
					ta512=atan_512(target->y-s->y,target->x-s->x);
					if (data->angle512>ta512)
					{
							 if (data->angle512-ta512<deg_360_to_512(175))	{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (data->angle512-ta512>deg_360_to_512(185))	{	data->angle512+=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
					else if (data->angle512<ta512)
					{
							 if (ta512-data->angle512<deg_360_to_512(175))	{	data->angle512+=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
						else if (ta512-data->angle512>deg_360_to_512(185))	{	data->angle512-=deg_360_to_512(8)/**fps_fa_ctor*/;} /* hlaser:8, homing:9 */
					}
				}
				#if 0
				if (data->angle360<0)		{	data->angle360+=360;}
				if (data->angle360>360) {	data->angle360-=360;}
				#else
				mask512(data->angle512);
				#endif
			//	tg->x=target->x+((target->w-tg->w)>>1)+rand()%6-3;	/* ターゲット目標表示マーカー */
			//	tg->y=target->y+((target->h-tg->h)>>1)+rand()%6-3;	/* ターゲット目標表示マーカー */
			}
			else
			{
				/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
				data->target_obj=NULL/*data->target_id=-1*/;		/* 画面外に逃げられたら */
				data->state=2;	/* 戻る */
			//	tg->flags&= ~SP_FLAG_VISIBLE;	/* ターゲット目標表示マーカー */
			}
		}
		break;
	}
	#if 1
	/* (多分必要ないけど)念の為マスク */
	mask512(data->angle512);
	#endif
	s->x+=co_s512((data->angle512))*data->speed/**fps_fa_ctor*/;
	s->y+=si_n512((data->angle512))*data->speed/**fps_fa_ctor*/;
	#if 0
	/* homing */
	//	{	double dangle;
	//		dangle=data->angle+180;
	//		if (dangle>360) {	dangle-=360;}
	//		//parsys_add(NULL,20,0,s->x+((s->w)>>1),s->y+((s->h)>>1),20,dangle,30,10,PIXELATE,NULL);
	//	}
		#if 0
		s->aktframe=(((int)( data->angle360*20))/360) % 20;
		#else
		s->aktframe=(((int)( data->angle512*10))>>8);
		#endif
	#endif
	weapon_colcheck(s,/*data->angle,*/1,0);

	data->time_out--/*range-=fps_fa_ctor*/;
	if (data->time_out/*range*/<=0) /* 時間切れ？ / max. Reichweite erreicht? */
	{
		s->type=SP_DELETE;		/* 時間切れなのでおしまい */
	//	tg->type=SP_DELETE; 	/* ターゲット目標表示マーカー*/
		if (target!=NULL)		/* ターゲットがまだ生きてたら、 Target freigeben */
		{
			target->flags &= ~SP_FLAG_PARANOIA; /* 弄ったフラグ戻しとこう。 */
		}
	}
}


/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(霊夢)
---------------------------------------------------------*/

/*
	s->x		シールドのx座標
	player->x	プレイヤーのx座標
	player->w/2 プレイヤーの横幅の半分
	co_s512((data->angle512))	angleの角度をπに直したあとcos。この場合は40°の倍数＋１０*fpsのcos。
	data->radius	円の半径40
	s->w/2		シールドの横幅の半分
	プレイヤーのx座標＋プレイヤーの横幅の半分＝プレイヤーの中心座標
	なので
	シールドのx座標＝プレイヤーのx座標＋プレイヤーの横幅の半分＋co s(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
				   ＝プレイヤーの中心座標＋co s(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
*/
// 時計回り ＆ 反時計回り 兼用
static void player_move_shield(SPRITE *s)	//霊夢
{
	PL_SHIELD_DATA *data	=(PL_SHIELD_DATA *)s->data;
/// PLAYER_DATA *pd 	=(PLAYER_DATA *)player->data;
	data->angle512 += (data->speed/**fps_fa_ctor*/);/*...*/
	mask512(data->angle512);// if (data->angle>360) {	data->angle-=360;}

//	s->aktframe = (((int)(data->angle360))/10)%36;
	s->aktframe = ((((data->angle512)/*&(512-1)*/)*(36/2))>>8);

	s->x=player->x+co_s512((data->angle512))*data->radius+((player->w-s->w)>>1);
	s->y=player->y+si_n512((data->angle512))*data->radius+((player->h-s->h)>>1);
	if (data->radius<255)	{	data->radius+=(rand()&1);}	/* 少しずつ広がるようにした */
	else	{	s->type=SP_DELETE;}/* 回転が広がったらおしまい */
	s->alpha			= ((int)(255-data->radius))&(256-1);				/* だんだん消えるようにした */

	weapon_colcheck(s,/*d->angle,*/0,1);/* ボムで敵を倒すあたり判定 */

//	if (pd->bomber_time<=0) {	s->type=SP_DELETE;}/* 時間制限でおしまい */
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(魔理沙、チルノ)
---------------------------------------------------------*/

static void player_move_levarie(SPRITE *s)
{
	PL_LEVARIE_DATA *data=(PL_LEVARIE_DATA *)s->data;
	s->x += data->vx;//co_s512((data->angle512))*d->speed/**fps_fa_ctor*/;
	s->y += data->vy;//si_n512((data->angle512))*d->speed/**fps_fa_ctor*/;
	if (CIRNO==select_player)
	{	/*1.5*(d->speed)*//**fps_fa_ctor*/;/*チルノ用(暫定的)*/
		s->y -= (abs((data->vx*1.5)));
		s->y -= (abs((data->vy*1.5)));
		s->y--;
	}
	weapon_colcheck(s,/*data->angle,*/0,1);/* ボムで敵を倒すあたり判定 */

	if ((s->x+s->w<0)||(s->x>GAME_WIDTH)||
		(s->y+s->h<0)||(s->y>GAME_HEIGHT))
	{
		s->type=SP_DELETE;/* 画面外に出たらおしまい */
	}
}

/*---------------------------------------------------------
	プレイヤー、高速時ボムの移動(レミリア)
---------------------------------------------------------*/

static void remilia_move_burn_fire(SPRITE *s)
{
	OZ_BURN_FIRE_DATA *data=(OZ_BURN_FIRE_DATA *)s->data;
	// 加速
	data->vx *= 1.1;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	data->vy *= 1.1;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	//
	s->x += data->vx;//co_s512((data->angle512))*p->speed/**fps_fa_ctor*/;
	s->y += data->vy;//si_n512((data->angle512))*p->speed/**fps_fa_ctor*/;
	if ((s->x+s->w<0)||(s->x>GAME_WIDTH)||
		(s->y+s->h<0))
	{
		s->type=SP_DELETE;/* 画面外に出たらおしまい */
	}
	else
	{
		weapon_colcheck(s,/*data->angle,*/0,1);/* ボムで敵を倒すあたり判定 */
	}
}

/* 十字炎ボムの炎の部分 */
/*		r_or_l	[xxx_r] l=2, [xxx_l] r=0 ,u=3 ,d=1	*/
static void remilia_add_burn_fire(SPRITE *s/*, int ggg*/ /*r_or_l*/)	/* [***090221 追加 */
{
	int ggg;
	for (ggg=0;ggg<3;ggg++)
	{
		/*const*/ char *bbb_name[3]=
		{	// 魔理沙
			"fire_wind_l.png",	/*move left*/
			"fire_wind_r.png",	/*move right*/
			"fire_wind_u.png",
		};
		SPRITE *c;
		c=sprite_add_file0((char *)bbb_name[(ggg)],4,PR_PLAYER,0);
		//
		c->flags			|= SP_FLAG_VISIBLE;
		c->type 			= SP_PL_PLASMA/*ボスに有効*/;/*SP_PL_SHIELD ボスに無効*/
		c->anim_speed		= 3;
		c->aktframe 		= 0;
		c->mover			= remilia_move_burn_fire;
		c->alpha			= 100;
		c->x				= s->x+((s->w-c->w)>>1);
	//		 if (0==ggg/*2==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (1==ggg/*0==r_or_l*/)	{	c->y			= s->y+15;}
	//	else if (2==ggg/*3==r_or_l*/)	{	c->y			= s->y+15;	c->y -= s->h;}
	//
		c->y			= s->y+15;
		if (2==ggg/*3==r_or_l*/)	{	c->y -= s->h;}
	//
		OZ_BURN_FIRE_DATA *data;
		data				= mmalloc(sizeof(OZ_BURN_FIRE_DATA));
		c->data 			= data;
	//
	//	data->speed 		= 3/*12*/;/*もうちょっと遅く(遅い方が長く残るので強い)*/
		data->strength		= 9/*10*/;/* お嬢のボムは当てなければ減らないので(外すと減らない)若干強すぎるくらいでいい */
		/* ただ、将来あたり判定が設定できるようになったら、あたり判定は多少小さくしたい所 */
		int aaa512[4] =
		{
			deg_360_to_512(180),	/*l=2*/ 	/*move left*/
			deg_360_to_512(  0),	/*r=0*/ 	/*move right*/
			deg_360_to_512(270),	/*u=3*/
			deg_360_to_512( 90),	/*d=1*/
		};
	//	data->angle512		= ((double)r_or_l/2)*deg_360_to_512(180)/*M_PI*2*1/2*/;
	//	data->angle512		= aaa512[ggg];
		data->vx = co_s512((/*data->angle512*/aaa512[ggg]))*/*data->speed*/(1)/**fps_fa_ctor*/;
		data->vy = si_n512((/*data->angle512*/aaa512[ggg]))*/*data->speed*/(1)/**fps_fa_ctor*/;
	}
}

static void player_move_add_cross_red(SPRITE *s)	/* [***090220 追加 */
{
//	PL_CROSS_FIRE_DATA *data=(PL_CROSS_FIRE_DATA *)s->data;

//	data->angle512 += rad2deg512(1)/*fps_fa_ctor*/;
//	mask512(data->angle512);	//if (data->angle_>M_PI)	{	data->angle_ -= 2*M_PI; }

	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	//
	if (0x3f==(((int)pd->bomber_time)&0x3f))
	{
		remilia_add_burn_fire(s/*, 0*/ /*2*/);
	//	remilia_add_burn_fire(s, 1/*0*/);
	//	remilia_add_burn_fire(s, 2/*3*/);
	}
	//
	if (pd->bomber_time > 0x3f/*31 20*/ )
	{
		/*else */
	//	if (0==(((int)pd->bomber_time)&3/*%4*/))
	//	{	/* 0 1 2 3 */
	//		s->aktframe++;
	//		s->aktframe &= 3;	//if (s->aktframe==4)	{	s->aktframe=0;	}
	//	}
		s->aktframe = ((((int)pd->bomber_time)>>2)&0x03);
	}
	else
	{
	//	if (0==(((int)pd->bomber_time)&3/*%4*/))
	//	{
	//			 if (s->aktframe<4) 	{	s->aktframe=4;}
	//		else if (s->aktframe==4)	{	s->aktframe=5;}
	//		else if (s->aktframe==5)	{	s->aktframe=6;}
	//		else if (s->aktframe==6)	{	s->aktframe=7;}
	//		else if (s->aktframe==7)	{	s->aktframe=8;}
	//		else						{	s->aktframe=9;}
	//	}
		static Uint8 ddd_tbl[8]=
		{	/* ボムの終わりがわかりやすいよう猶予時間(9のパターン)を長く(多く)、速めに警告する */
			 9,  9,  9,  8, 	 7,  6,  5,  4,
		};
		s->aktframe = ddd_tbl[((((int)pd->bomber_time)>>3)&0x07)];
	}
	s->x = player->x+((player->w-s->w)>>1);
	s->y = player->y+((player->h-s->h)>>1);

	weapon_colcheck(s,/*data->angle,*/0,1);/* 十字炎本体で焼き殺す場合 */

	if (pd->bomber_time <= 0) { s->type=SP_DELETE;}/* 時間制限でおしまい */
}


/*---------------------------------------------------------
	プレイヤー、針弾の追加(霊夢)[針弾]
---------------------------------------------------------*/

static void reimu_add_needle(SPRITE *s, int x_offs )	//霊夢
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+REIMU/*select_player*/ ); //sprite_add_file0("fireball1.png",2,PR_PLAYER,1);	shot->anim_speed=1;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= s->x+((s->w-shot->w)>>1) + x_offs;
	shot->y 			= s->y-(7);
	shot->mover 		= player_move_needle;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data			= data;
//	data->angle512		= deg_360_to_512(270);
//	data->speed 		= 18;
	data->vx = co_s512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(18)/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(18)/**fps_fa_ctor*/;
	data->strength		= /*3*/2;	/*強すぎる*/
}

/*---------------------------------------------------------
	プレイヤー、針弾の追加(魔理沙)[森弾]
---------------------------------------------------------*/

static void marisa_add_needle(SPRITE *s, int x_offs, int y_offs)	//魔理沙
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+select_player ); //sprite_add_file0((char *)aaa_name[(k)],1,PR_PLAYER,0); shot->anim_speed=0;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= s->x+((s->w-shot->w)>>1) + x_offs;
	shot->y 			= s->y-(7)+y_offs;
	shot->alpha 		= 150;
	shot->mover 		= player_move_needle;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data			= data;
	data->strength		= 4;
//
//	data->angle512		= deg_360_to_512(270);
//	data->speed 		= 10;
	data->vx = co_s512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(10)/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/deg_360_to_512(270)))*/*p->speed*/(10)/**fps_fa_ctor*/;
}

/*---------------------------------------------------------
	プレイヤー、針弾の追加(レミリア、仮チルノ)[コウモリ弾]
---------------------------------------------------------*/
enum
{
	REMILIA_NEEDLE_TYPE270=0,
	REMILIA_NEEDLE_TYPE263,
	REMILIA_NEEDLE_TYPE277,
	REMILIA_NEEDLE_TYPE260,
};
static void remilia_add_needle(SPRITE *s, int needle_type ) //おぜう	/* [***090220 追加 */
{
	SPRITE *shot;
	shot				= sprite_add_res( BASE_NEEDLE_PNG+select_player/*REMILIA*/ ); //sprite_add_file0("bat.png",5,PR_PLAYER,0); shot->anim_speed=3;
	shot->type			= SP_PL_FIREBALL;
	shot->x 			= s->x+((s->w-shot->w)>>1);
	shot->y 			= s->y-(7);
	shot->mover 		= player_move_needle/*player_move_koumori*/;
	shot->flags 		|= SP_FLAG_VISIBLE;
	PL_SHOT_DATA *data;
	data				= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data			= data;
	data->strength		= 6/*5*/;	/*強すぎる*/
//
//	data->angle512		= bbb[needle_type]/*deg_360_to_512(270)*/;
//	data->speed 		= 3/*8*/;	/*強すぎる*/
	const signed short bbb[4] =
	{
		deg_360_to_512(270),
		deg_360_to_512(270-25),/*263=270-7*/
		deg_360_to_512(270+25),/*277=270+7*/
		deg_360_to_512(260),
	};
	data->vx = co_s512((/*angle512*/bbb[needle_type]))*/*p->speed*/(3)/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/bbb[needle_type]))*/*p->speed*/(3)/**fps_fa_ctor*/;
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動制御
---------------------------------------------------------*/
static int option_flag;
static void reimu_marisa_control_option(SPRITE *s, PL_OPTION_DATA *data, PLAYER_DATA *pd) //霊夢、魔理沙
{
	int test_angle512;
	test_angle512 = data->angle512 - 128;/* 判定用角度：９０度回転させて下が０とする */ /*擬似単位系変換*/
	mask512(test_angle512);
	if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)	/* 押すと前方に移動 */
	{
		if (OPTION_ID_01_RIGHT/*1*/==data->option_id/*pos*/)	/* 1: 右側のオプション */
		{
			if (test_angle512 > /*(224)*/(256-16)/*240 (256-16)*/ )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 -= rad2deg512(0.1);
			}
		}
		else	/* 0: 左側のオプション */
		{
			if (test_angle512 < /*(288)*/(256+16)/*272 (256+16)*/ )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 += rad2deg512(0.1);
			}
		}
	}
	else	/* 放すと後方に移動 */
	{
		if (OPTION_ID_01_RIGHT/*1*/==data->option_id/*pos*/)	/* 1: 右側のオプション */
		{
			if (test_angle512 < 448-16 )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 += rad2deg512(0.1);
			}
		}
		else	/* 0: 左側のオプション */
		{
			if (test_angle512 > 64+16 )//if ((data->angle>-M_PI/2-0.05)&&(data->angle<-M_PI/2+0.05))
			{
				data->angle512 -= rad2deg512(0.1);
			}
		}
	}
	if (
	//	(0==(pd->bonus_flag&BONUS_FLAG_08_OPTION_HIDE))/*0==pd->option_flag*/ ||	/* オプションが消えてれば */
		(0==option_flag)/*0==pd->option_flag*/ ||	/* オプションが消えてれば */
		pd->lives<0)			/* 残チャンスがなければ */
	{	s->type=SP_DELETE;}/*おしまい*/
	#if 1
	mask512(data->angle512);/*ねんのため*/
	#endif
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/-((s->w)>>1)+((player->w-12-1)*data->option_id)+6;
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-12-1)*data->option_id)+6-8;
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/24+11+11)*data->option_id)-11-8;
// -1 : 		 -11 -8
//	1 : +24+11+11-11 -8
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/24-6-6)*data->option_id)+6-8;
// -1 : 		 +6 -8
//	1 : +24 -6 -6 +6 -8
//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+((/*player->w*/(24)-6-6)*data->option_id)+6-((s->w)>>1)/*(8)*/;
//
	//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+(((int)(data->pos+1)*player->w-s->w)/2)-data->pos*6;
	//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+(((int)(data->option_id+1)*/*player->w*/(24)-s->w)/2)-data->option_id*6;			//2598187
	//	s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+ (((24/*player->w*/ /2)-6)*(data->option_id)) -((s->w)>>1) +(24/*player->w*/ /2);	//2598219
		s->x = player->x+co_s512((data->angle512))*(16)/*data->length*/+ ((6)*(2+data->option_id)) -((s->w)>>1) ;	//2598187
		s->y = player->y+si_n512((data->angle512))*(16)/*data->length*/+((player->h-s->h)>>1)-5;
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動(霊夢)
---------------------------------------------------------*/

static void re_move_option(SPRITE *s)	//霊夢
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	data->ani_speed--;
	if (data->ani_speed==0)
	{
		data->ani_speed=12/*2*/;/*陰陽だまはゆっくり回るよ*/
	//	s->aktframe=((s->aktframe+(data->option_id)+(data->option_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->aktframe=((s->aktframe+(data->option_id))&(8-1)/*%8*/);
	//	if (s->aktframe<0)		{	s->aktframe=7;}
	}
	//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
	{
		if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
		{
			if (pd->state==PL_SAVE||pd->state==PL_NORMAL)
			{
				data->w_wait--;
				if (data->w_wait<0)
				{
					data->w_wait=4;
					if ( pd->weapon <= 90-1)
					{
						reimu_add_needle(s,  0);
					}
					else
					{
						reimu_add_needle(s, -5);
						reimu_add_needle(s,  5);
					}
				}
			}
		}
	}
	reimu_marisa_control_option(s, data, pd);
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動(魔理沙)
---------------------------------------------------------*/

static void ma_move_option(SPRITE *s)	//魔理沙
{
	PL_OPTION_DATA *data=(PL_OPTION_DATA *)s->data;
	data->ani_speed--;
	if (data->ani_speed==0)
	{
		data->ani_speed=6;
	//	s->aktframe=((s->aktframe+(data->option_id)+(data->option_id)-1/*data->pos*/)&(8-1)/*%8*/);
		s->aktframe=((s->aktframe+(data->option_id))&(8-1)/*%8*/);
	//	if (s->aktframe<0)		{	s->aktframe=7;}
	}
	//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
	{
		if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
		{
			if (pd->state==PL_SAVE||pd->state==PL_NORMAL)
			{
				data->w_wait--;
				if (data->w_wait<0)
				{
					data->w_wait=8;
					if (pd->weapon <= 70-1)
					{
						marisa_add_needle(s,  0,   0);
					}
					else if (pd->weapon <= 100-1)
					{
						marisa_add_needle(s, -7,   0);
						marisa_add_needle(s,  7,   0);
					}
					else
					{
						marisa_add_needle(s, -7,   0);
						marisa_add_needle(s,  0, -38);/*-38-7==-45*/
						marisa_add_needle(s,  7,   0);
					}
				}
			}
		}
	}
	reimu_marisa_control_option(s, data, pd);
}

/*---------------------------------------------------------
	プレイヤー、オプションの移動(レミリア)
---------------------------------------------------------*/

/* レミリア用 */
static SPRITE *c1, *c2, *c3, *c4; // [***090128.090220	追加:オプション用.c3,c4
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


#define OPTION_ID_01_NUM (0)
#define OPTION_ID_02_NUM (1)
#define OPTION_ID_03_NUM (2)
#define OPTION_ID_04_NUM (3)

static void oz_move_option(SPRITE *s)	//おぜう	/* [***090220 追加 */
{
	static int state1=0;

	REMILIA_OPTION_DATA *data=(REMILIA_OPTION_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
//
	switch (data->option_id)
	{
	case OPTION_ID_01_NUM:
		break;
	case OPTION_ID_02_NUM:
		if (  (pd->weapon<=55-1))			{	s->flags &= ~SP_FLAG_VISIBLE;	}
		else /*if (  (pd->weapon>55-1))*/	{	s->flags |=  SP_FLAG_VISIBLE;	}
		break;
	case OPTION_ID_03_NUM:
		if (  (pd->weapon<=80-1))			{	s->flags &= ~SP_FLAG_VISIBLE;	}
		else /*if (  (pd->weapon>80-1))*/	{	s->flags |=  SP_FLAG_VISIBLE;	}
		break;
	case OPTION_ID_04_NUM:
		if (  (pd->weapon<=108-1))			{	s->flags &= ~SP_FLAG_VISIBLE;	}
		else /*if (  (pd->weapon>108-1))*/	{	s->flags |=  SP_FLAG_VISIBLE;	}
		break;
	}
//
	if (s->flags&SP_FLAG_VISIBLE)	//表示されていれば上記の処理をするよ。
	{
		// アニメーション
		data->ani_speed--;
		if (data->ani_speed==0)
		{
			data->ani_speed=3;
			s->aktframe++;
			s->aktframe &= (8-1);//if (s->aktframe==8)	{s->aktframe=0;}
		}
		// 攻撃支援
		if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
		{
			if (pd->state==PL_SAVE||pd->state==PL_NORMAL)	//攻撃
			{
				if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
				{
					data->w_wait--;
					if (data->w_wait<0)
					{
						switch (data->option_id)
						{
						case OPTION_ID_01_NUM:
						case OPTION_ID_02_NUM:
							if (pd->weapon > 100-1)
							{
								remilia_add_needle(s, REMILIA_NEEDLE_TYPE277 );
								remilia_add_needle(s, REMILIA_NEEDLE_TYPE263 );
								data->w_wait=7+3;
							}
							else
							{
								remilia_add_needle(s, REMILIA_NEEDLE_TYPE270 );
								data->w_wait=5+3;
							}
							break;
						case OPTION_ID_03_NUM:
						case OPTION_ID_04_NUM:
							remilia_add_needle(s, REMILIA_NEEDLE_TYPE270 );
							data->w_wait=5+3;
							break;
						}
					}
				}
			}
		}
		//
		int slow_flag;
		slow_flag=0;
		// 低速移動
		if (my_pad & PSP_KEY_SLOW/*keybo ard[KEY_SLOW]*/)
		{
			if (REMILIA==select_player)
			{
				slow_flag=1;
			}
		//	else
		//	{
		//		/*チルノ*/
		//	}
		}
		int player_offs_x;
		int player_offs_y;
		player_offs_x = player->x+((player->w-s->w)>>1);
		player_offs_y = player->y+((player->h-s->h)>>1);
		if (1==slow_flag)	// 少なくとも低速移動でオプションが止められるのはレミのみ(チルノには合わない)
		{
			/*レミ用*/
			data->slow_count++;
			if ((/*data->*/state1==FORMATION_03) && /* FORMATION_03: 減速時用(好きな位置に置けるよ) */
				(data->slow_count < ((data->option_id+1)<<6)/* *30 64*/))
			{
				s->x = player_offs_x;
				s->y = player_offs_y;
			}
			else if (/*data->*/state1==FORMATION_02)	/* FORMATION_02: 画面後方から支援するよ */
			{
				switch (data->option_id)
				{
				case OPTION_ID_01_NUM: if (s->x<170-s->w)	{s->x+=2/**fps_fa_ctor*/;}	break;
				case OPTION_ID_02_NUM: if (s->x>210)		{s->x-=2/**fps_fa_ctor*/;}	break;
				case OPTION_ID_03_NUM: if (s->x<100-s->w)	{s->x+=4/**fps_fa_ctor*/;}	break;
				case OPTION_ID_04_NUM: if (s->x>280)		{s->x-=4/**fps_fa_ctor*/;}	break;
				}
			}
		}
		else
		{
			/*レミ用*/
			if (data->slow_count)
			{
				data->slow_count=0;
				data->state2=0;
			}
		//
			// フォーメーション変更
			/* OPTION_ID_01_NUM の場合のみキー入力受け付け */
			if (OPTION_ID_01_NUM==data->option_id) //option_id==1によるflag管理
			{
			//	if (my_pad & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)
			/* (キャンセルキーを離した瞬間なら) */
				if ((0==(my_pad & PSP_KEY_CANCEL))) 	/* キャンセルキーが離されている */
				{
					if ( (my_pad^my_pad_alter) & PSP_KEY_CANCEL)	/* キャンセルキーの状態が変わった(トグル、押したまたは離した場合) */
					{
						/*data->*/state1++;
						if (/*data->*/state1==(FORMATION_03+1) )
						{	/*data->*/state1=FORMATION_00;}
					//
						data->state2=0;
						{
						((REMILIA_OPTION_DATA *)c4->data)->state2=0;
						((REMILIA_OPTION_DATA *)c3->data)->state2=0;
						((REMILIA_OPTION_DATA *)c2->data)->state2=0;
						}
					}
				}
			}
			switch (data->state2)
			{
			case 0: 	//初期化
				{
					enum
					{
						FORMATION_00_LOCATE_X=0,
						FORMATION_00_LOCATE_Y,/* 1*/
						FORMATION_01_RADIUS_R,/* 2*/
						FORMATION_01_ADD_R,   /* 3*/
						FORMATION_02_LOCATE_X,/* 4*/
						FORMATION_02_LOCATE_Y,/* 5*/
						FORMATION_MAX,		  /* 6*/
					};
					const signed short fff[FORMATION_MAX][4]=
					{
						{  15, -15,  25, -25 },
						{  15,	15,  20,  20 },
						{  20,	30,  40,  50 }, 	/* FORMATION_01:回転半径 */
						{	1,	 2,  -4,  -8 }, 	/* FORMATION_01:回転方向、角度加算値 */
						{	1*115+10,  2*115+10,  0*115+10,  3*115+10 },
						{ 240, 240, 220, 220 },
					};
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
						data->offset_x=fff[FORMATION_00_LOCATE_X][(data->option_id)];
						data->offset_y=fff[FORMATION_00_LOCATE_Y][(data->option_id)];
						break;
					case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
						data->offset_x=fff[FORMATION_01_RADIUS_R][(data->option_id)];/*40*/ 	/* 円の半径と兼用 */
						data->offset_y=fff[FORMATION_01_ADD_R	][(data->option_id)];			/* 円の回転方向、角度加算値と兼用 */
						data->f_angle512=0/*l_angle512*/;
						break;
					case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
						data->offset_x=fff[FORMATION_02_LOCATE_X][(data->option_id)];
						data->offset_y=fff[FORMATION_02_LOCATE_Y][(data->option_id)];/*240*/
						break;
					case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
						break;
					}
				}
				data->state2=1; 	/* 次へ */
				break;
			case 1: 	// 目標地点へ移動中
				#define oz_offset_r 	(data->offset_x)		/* 円の半径と兼用 */
				#define oz_offset_add_r (data->offset_y)		/* 円の回転方向、角度加算値と兼用 */
				{
				int delta_y=0;				/* delta_y : y座標における目標地点と現在地の差 */
				int delta_x=0;				/* delta_x : x座標における目標地点と現在地の差 */
					switch (/*data->*/state1)
					{
					case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
						delta_y = player_offs_y-s->y + data->offset_y;
						delta_x = player_offs_x-s->x + data->offset_x;
						break;
					case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */						//static /*double*/int l_angle512;
						delta_y = player_offs_y-s->y + si_n512((data->angle512/*+l_angle512*/))*oz_offset_r;
						delta_x = player_offs_x-s->x + co_s512((data->angle512/*+l_angle512*/))*oz_offset_r;
						break;
					case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
						delta_y = (-((s->h)>>1)-s->y+data->offset_y);/*240*/
						delta_x = (-((s->w)>>1)-s->x+data->offset_x);
						break;
					case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
						delta_y = player_offs_y-s->y;
						delta_x = player_offs_x-s->x;
						break;
					}
					{	/* 目標地点に移動する */
						int m_angle512;
						/*data->*/m_angle512=atan_512(delta_y, delta_x);
						s->x+=co_s512((/*data->*/m_angle512))*/*10*/((REMILIA==select_player)?8.0:1.0)/**fps_fa_ctor*/;
						s->y+=si_n512((/*data->*/m_angle512))*/*10*/((REMILIA==select_player)?8.0:1.0)/**fps_fa_ctor*/;
					}
#if 1
					/*レミ用*/
					if (REMILIA==select_player)
					{
						/* 目標付近までたどり着いたら */
						if ( (-5 < delta_y) && (delta_y < 5) )
						{
							data->state2=2; 	/* 次へ */
						}
					}
#endif
				}
				break;
#if 1
			case 2: 	// 位置の維持 & 配置完了flag
				switch (/*data->*/state1)
				{
				case FORMATION_00:	/* FORMATION_00: レミリアの直後に四つ */
					s->x = player_offs_x + data->offset_x;
					s->y = player_offs_y + data->offset_y;
					break;
				case FORMATION_01:	/* FORMATION_01: レミの周りを回るよ */
					/*l_angle512*/data->f_angle512 += oz_offset_add_r/*data->offset_y*/ /*rad2deg512(0.05)*/ /**fps_fa_ctor*/;
					mask512(/*l_angle512*/data->f_angle512);	//if (l_angle>M_PI) 	{	l_angle-=2*M_PI;	}
					s->x = player_offs_x + si_n512((data->angle512+data->f_angle512/*l_angle512*/))*oz_offset_r;
					s->y = player_offs_y + co_s512((data->angle512+data->f_angle512/*l_angle512*/))*oz_offset_r;
					break;
				case FORMATION_02:	/* FORMATION_02: 画面後方から支援するよ */
					s->x = data->offset_x;
					s->y = data->offset_y;
					break;
				case FORMATION_03:	/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
					s->x = player_offs_x;
					s->y = player_offs_y;
					break;
				}
#endif
				break;
			}
		}
	}
//
	if (
	//	(0==(pd->bonus_flag&BONUS_FLAG_08_OPTION_HIDE))/*0==pd->option_flag*/ ||	/* オプションが消えてれば */
		(0==option_flag)/*0==pd->option_flag*/ ||	/* オプションが消えてれば */
		pd->lives<0)			/* 残チャンスがなければ */
	{	s->type=SP_DELETE;}/*おしまい*/
}

/*---------------------------------------------------------
	プレイヤー、オプションの追加
---------------------------------------------------------*/

static void re_add_option(SPRITE *s)	//霊夢
{
	PL_OPTION_DATA *d1, *d2;
	c1				= sprite_add_res( BASE_OPTION1_PNG/*+REIMU*/ );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->mover		= re_move_option;
	d1				= mmalloc(sizeof(PL_OPTION_DATA));
	c1->data		= d1;
	d1->strength	= 5;/* [***090214 追加 */
//	d1->length		= 11;
	c1->x			= s->x+((s->w-c1->w)>>1)-(11)/*d1->length*/;
	c1->y			= s->y+((s->h-c1->h)>>1);
	d1->ani_speed	= 2;
	d1->option_id/*pos*/		= OPTION_ID_99_LEFT/*-1*/;
	d1->angle512	= deg_360_to_512(180)/*-rad2deg512(M_PI)*/;
//	d1->state		= 0;
	d1->w_wait		= 0;
	c1->aktframe	= 0;
	c1->type		= SP_PLAYER2;

	c2				= sprite_add_res( BASE_OPTION1_PNG/*+REIMU*/ );
	c2->flags		|= SP_FLAG_VISIBLE;
	c2->mover		= re_move_option;
	d2				= mmalloc(sizeof(PL_OPTION_DATA));
	c2->data		= d2;
	d2->strength	= 5;/* [***090214 追加 */
//	d2->length		= 11;
	c2->x			= s->x+((s->w-c2->w)>>1)-(11)/*d2->length*/;
	c2->y			= s->y+((s->h-c2->h)>>1);
	d2->ani_speed	= 2;
	d2->option_id/*pos*/		= OPTION_ID_01_RIGHT/*1*/;
	d2->angle512	= deg_360_to_512(0);
//	d2->state		= 0;
	d2->w_wait		= 0;
	c2->aktframe	= 0;
	c2->type		= SP_PLAYER2;
}

static void ma_add_option(SPRITE *s)	//魔理沙
{
	PL_OPTION_DATA *d1, *d2;		//霊夢のを流用してます。
	c1				= sprite_add_res( BASE_OPTION1_PNG+MARISA );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->mover		= ma_move_option;
	d1				= mmalloc(sizeof(PL_OPTION_DATA));
	c1->data		= d1;
	d1->strength	= 5;/* [***090214 追加 */
//	d1->length		= 11;
	c1->x			= s->x+((s->w-c1->w)>>1)-(11)/*d1->length*/;
	c1->y			= s->y+((s->h-c1->h)>>1);
	d1->ani_speed	= 6;
	d1->option_id/*pos*/		= OPTION_ID_99_LEFT/*0*/ /*-1*/;
	d1->angle512	= deg_360_to_512(180)/*-rad2deg512(M_PI)*/;
//	d1->state		= 0;
	d1->w_wait		= 0;
	c1->aktframe	= 0;
	c1->type		= SP_PLAYER2;

	c2				= sprite_add_res( BASE_OPTION1_PNG+MARISA );
	c2->flags		|= SP_FLAG_VISIBLE;
	c2->mover		= ma_move_option;
	d2				= mmalloc(sizeof(PL_OPTION_DATA));
	c2->data		= d2;
	d2->strength	= 5;/* [***090214 追加 */
//	d2->length		= 11;
	c2->x			= s->x+((s->w-c2->w)>>1)-(11)/*d2->length*/;
	c2->y			= s->y+((s->h-c2->h)>>1);
	d2->ani_speed	= 6;
	d2->option_id/*pos*/		= OPTION_ID_01_RIGHT/*1*/;
	d2->angle512	= deg_360_to_512(0);
//	d2->state		= 0;
	d2->w_wait		= 0;
	c2->aktframe	= 0;
	c2->type		= SP_PLAYER2;
}

static void oz_add_option(SPRITE *s)	//おぜう	/* [***090220 追加 */
{
	REMILIA_OPTION_DATA *d;
	c1				= sprite_add_res( BASE_OPTION1_PNG+select_player/*REMILIA*/ );
	c1->flags		|= SP_FLAG_VISIBLE;
	c1->type		= SP_PLAYER2;
	c1->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c1->data		= d;
	c1->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*1)/*M_PI*2*1/8*/;
//	d->next 		= player;
//	d->state1		= 0;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_01_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c1->x			= s->x+((s->w-c1->w)>>1);
	c1->y			= s->y+((s->h-c1->h)>>1)+20;

	c2				= sprite_add_res( BASE_OPTION2_PNG+select_player/*REMILIA*/ );
	c2->flags		&= ~SP_FLAG_VISIBLE;
	c2->type		= SP_PLAYER2;
	c2->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c2->data		= d;
	c2->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*5)/*M_PI*2*5/8*/;
//	d->next 		= c1;
//	d->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_02_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c2->x			= s->x+((s->w-c2->w)>>1);
	c2->y			= s->y+((s->h-c2->h)>>1)+20;

	c3				= sprite_add_res( BASE_OPTION3_PNG+select_player/*REMILIA*/ );
	c3->flags		&= ~SP_FLAG_VISIBLE;
	c3->type		= SP_PLAYER2;
	c3->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c3->data		= d;
	c3->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*3)/*M_PI*2*3/8*/;
//	d->next 		= c2;
//	d->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_03_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c3->x			= s->x+((s->w-c3->w)>>1);
	c3->y			= s->y+((s->h-c3->h)>>1)+20;

	c4				= sprite_add_res( BASE_OPTION4_PNG+select_player/*REMILIA*/ );
	c4->flags		&= ~SP_FLAG_VISIBLE;
	c4->type		= SP_PLAYER2;
	c4->mover		= oz_move_option;
	d				= mmalloc(sizeof(REMILIA_OPTION_DATA));
	c4->data		= d;
	c4->aktframe	= 0;
	d->angle512 	= deg_360_to_512(45*7)/*M_PI*2*7/8*/;
//	d->next 		= c3;
//	d->state1		= ((REMILIA_OPTION_DATA *)c1->data)->state1;///
	d->state2		= 0;
	d->offset_x 	= 0;
	d->offset_y 	= 0;
	d->strength 	= 5;
	d->ani_speed	= 3;
	d->w_wait		= 0;
	d->option_id	= OPTION_ID_04_NUM;
//	d->m_angle512	= deg_360_to_512(0);
	d->slow_count	= 0;
	c4->x			= s->x+((s->w-c4->w)>>1);
	c4->y			= s->y+((s->h-c4->h)>>1)+20;
}

/*---new add-----*/


/*---------------------------------------------------------
	プレイヤー、生死判定用コアの追加
---------------------------------------------------------*/

static SPRITE *player_add_core(SPRITE *s1)		//○の追加
{
	SPRITE *s2;
	s2				= NULL;
	s2				= sprite_add_res( BASE_CORE_PNG+select_player );
	s2->anim_speed	= 0;
	s2->type		= SP_PLAYER2;
	s2->x			= s1->x+((s1->w-s2->w)>>1);
	s2->y			= s1->y-s2->h+20;
	s2->mover		= player_move_core;
	s2->flags		|= SP_FLAG_VISIBLE;
	return s2;
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成、子関数
---------------------------------------------------------*/

enum
{
	REIMU_SHOT_TYPE_00=0,
	REIMU_SHOT_TYPE_01,
	REIMU_SHOT_TYPE_02,
//
	REIMU_SHOT_TYPE_03,
	REIMU_SHOT_TYPE_04,
	REIMU_SHOT_TYPE_05,
	REIMU_SHOT_TYPE_06,
	REIMU_SHOT_TYPE_07,
	REIMU_SHOT_TYPE_08,
	REIMU_SHOT_TYPE_09,
	REIMU_SHOT_TYPE_0a,
//
	MARISA_SHOT_TYPE_03,
	MARISA_SHOT_TYPE_04,
	MARISA_SHOT_TYPE_05,
	MARISA_SHOT_TYPE_06,
	MARISA_SHOT_TYPE_07,
	MARISA_SHOT_TYPE_08,
//
	REIMU_SHOT_TYPE_MAX
};

enum
{
	REI00_x_offset=0,
	REI01_y_offset,
	REI02_angle512,
	REI03_yudo,
	REI04_MAX,
};

static void player_add_shot(SPRITE *player, int shot_type) //霊夢 ＆ 魔理沙 ＆ チルノ ＆ おぜう /* [***090220 追加 */
{
	const Sint16 r_tbl[REIMU_SHOT_TYPE_MAX][REI04_MAX] =
	{	/*x_offset	y_offset  angle512 yudo */
/*00*/	{  0, 20, deg_360_to_512(270),	  0},	/*player_add_fuda*/ 		/*player_add_star*/
/*01*/	{ -5, 20, deg_360_to_512(270),	  0},	/*player_add_double_fuda*/	/*player_add_double_star*/
/*02*/	{ +5, 20, deg_360_to_512(270),	  0},	/*player_add_double_fuda*/	/*player_add_double_star*/
//
/*03*/	{-15,+25, deg_360_to_512(270-6),  1},	/*player_add_triple_fuda*/
/*04*/	{+15,+25, deg_360_to_512(270+6),  1},	/*player_add_triple_fuda*/
/*05*/	{-15,+25, deg_360_to_512(270-10), 1},	/*player_add_quad_fuda*/
/*06*/	{+15,+25, deg_360_to_512(270+10), 1},	/*player_add_quad_fuda*/
//
/*07*/	{-15,+25, deg_360_to_512(270-15), 0},	/*player_add_five_fuda*/
/*08*/	{+15,+25, deg_360_to_512(270+15), 0},	/*player_add_five_fuda*/
/*09*/	{ -5,+20, deg_360_to_512(270-8),  1},	/*player_add_five_fuda*/
/*0a*/	{ +5,+20, deg_360_to_512(270+8),  1},	/*player_add_five_fuda*/
//
/*03*/	{ -5,+20, deg_360_to_512(270-4),  0},	/*player_add_triple_star*/
/*04*/	{ +5,+20, deg_360_to_512(270+4),  0},	/*player_add_triple_star*/
/*05*/	{ -6,+20, deg_360_to_512(270-3),  0},	/*player_add_quad_star*/
/*06*/	{ +6,+20, deg_360_to_512(270+3),  0},	/*player_add_quad_star*/
/*07*/	{-10,+25, deg_360_to_512(270-7),  0},	/*player_add_quad_star*/
/*08*/	{+10,+25, deg_360_to_512(270+7),  0},	/*player_add_quad_star*/
	};
	SPRITE *shot;
	shot			= sprite_add_res( BASE_PLAZMA_PNG+select_player );
	shot->type		= SP_PL_PLASMA;
	shot->x 		= player->x+((player->w-shot->w)>>1)+r_tbl[shot_type][REI00_x_offset];
	shot->y 		= player->y-(shot->h)+r_tbl[shot_type][REI01_y_offset]/*20*/;
	shot->mover 	= player_move_shot;
	shot->flags 	|= SP_FLAG_VISIBLE;
	shot->alpha 	= 150;/*player_add_triplestar*/
	PL_SHOT_DATA *data;
	data			= mmalloc(sizeof(PL_SHOT_DATA));
	shot->data		= data;
//
	int int_angle512;
	int ok;
	ok=0;
	{
		if (1==r_tbl[shot_type][REI03_yudo])/* 自動ショットか？ */
		{
			SPRITE *target;
			target=search_enemy_by_sprite();
			if (target != NULL)
			{
				/*data->*/int_angle512	= atan_512(target->y-player->y,target->x-player->x);
			//	if (255 < /*data->*/int_angle512 )/* 上方180-360度[/360度]の領域のみ追尾する */
				if ( /*(384)*/(128*3) < ((/*data->*/int_angle512+64)&(512-1)) )/* 上方225-315度[/360度]の領域のみ追尾する */
				{
					ok=1;/* 自動追尾可能 */
				}
			}
		}
	}
//
	if (0==ok)/* 自動追尾不可？ */
	{
		/*data->*/int_angle512	= r_tbl[shot_type][REI02_angle512]/*deg_360_to_512(270)*/;/*固有の設定角度を使う*/
	}
	#define CCC_STRENGTH		(4*0)
	#define CCC_SPEED			(4*1)
	#define CCC_MAX 			(4*2)
	static Uint8 ccc_tbl[CCC_MAX] =
	{/* REIMU MARISA REMILIA CIRNO */
		 2, 3, 4, 5,	/* strength ショットの強さ */
		16,10, 9, 8,	/* speed	ショットの速さ (遅い方が画面上の弾数が増えるので強い。ただし重くなる) */
	};
//		 2, 3, 4, 3,	/* strength ショットの強さ */
//		15,12,18,12,	/* speed	ショットの速さ	*/
/* 極端に機体の性能が偏るのは良くない。(強くて速いのはずるい) */
	data->strength	= ccc_tbl[CCC_STRENGTH+select_player];
//	data->speed 	= ccc_tbl[CCC_SPEED+select_player];
//
	data->vx = co_s512((/*angle512*/int_angle512))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
	data->vy = si_n512((/*angle512*/int_angle512))*/*p->speed*/(ccc_tbl[CCC_SPEED+select_player])/**fps_fa_ctor*/;
}

/*---------------------------------------------------------
	プレイヤー弾(ショットのみ)生成
---------------------------------------------------------*/

static void player_add_dual_shot(SPRITE *player)	//霊夢 ＆ 魔理沙 ＆ チルノ ＆ おぜう	/* [***090220 追加 */
{
	player_add_shot(player,REIMU_SHOT_TYPE_01);
	player_add_shot(player,REIMU_SHOT_TYPE_02);
}

static void player_add_triple_fuda(SPRITE *player)	//霊夢
{
	player_add_shot(player,REIMU_SHOT_TYPE_03);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,REIMU_SHOT_TYPE_04);/* 自動ショット */
}

static void player_add_quad_fuda(SPRITE *player) //霊夢
{
	player_add_shot(player,REIMU_SHOT_TYPE_05);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_01);
	player_add_shot(player,REIMU_SHOT_TYPE_02);
	player_add_shot(player,REIMU_SHOT_TYPE_06);/* 自動ショット */
}

static void player_add_five_fuda(SPRITE *player)	//霊夢
{
	player_add_shot(player,REIMU_SHOT_TYPE_07);
	player_add_shot(player,REIMU_SHOT_TYPE_09);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,REIMU_SHOT_TYPE_0a);/* 自動ショット */
	player_add_shot(player,REIMU_SHOT_TYPE_08);
}

static void player_add_triple_star(SPRITE *player)	//魔理沙 ＆ チルノ
{
	player_add_shot(player,MARISA_SHOT_TYPE_03);
	player_add_shot(player,REIMU_SHOT_TYPE_00);
	player_add_shot(player,MARISA_SHOT_TYPE_04);
}

static void player_add_quad_star(SPRITE *player)	//魔理沙 ＆ チルノ
{
	player_add_shot(player,MARISA_SHOT_TYPE_05);
	player_add_shot(player,MARISA_SHOT_TYPE_06);
	player_add_shot(player,MARISA_SHOT_TYPE_07);
	player_add_shot(player,MARISA_SHOT_TYPE_08);
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
	shot->x 		= player->x+((player->w-shot->w)>>1);
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
	int a;
	for (a=0;a<355;a+=5)
	{
		SPRITE *c;
		c				= sprite_add_file0("fireball.png",1,PR_PLAYER,0);
		c->type 		= SP_PL_BOMB;
		c->x			= player->x+((player->w)>>1);
		c->y			= player->y+((player->h)>>1);
		c->mover		= player_move_bomb;
		c->flags		|= SP_FLAG_VISIBLE;
		PL_BOMB_DATA *data;
		data			= mmalloc(sizeof(PL_BOMB_DATA));
		c->data 		= b;
		data->angle 	= a;
		data->speed 	= 1;
		data->strength	= 2;
	}
}
#endif //(1==USE_PLAYER_BOMBER)

/*---------------------------------------------------------
	プレイヤーシールド生成
---------------------------------------------------------*/

static void player_add_shield(SPRITE *s)		//シールドの追加	//霊夢
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	int ii;//	int i;	/* 半象限ずつ */
	for (ii=0;ii<(8);ii++)//	for (i=0;i<360/*i<=359*/;i+=45)
	{
		SPRITE *c;
		PL_SHIELD_DATA *d;
		if ( (ii&1) == 0)// if (i%90 == 0)
		{
			c			= sprite_add_file0("cshoot1.png",36,PR_PLAYER,1);
			d			= mmalloc(sizeof(PL_SHIELD_DATA));
			c->data 	= d;
			d->speed	= 23;						//時計回り
			d->radius	= 5/*48*/;	//d->radius=38; 	/*半径初期値*/	/* 少しずつ広がるようにした */
		}
		else		//1つだと不安なので2つ作った。
		{
			c			= sprite_add_file0("cshoot2r.png"/*"cshoot2.png"*/,36,PR_PLAYER,1);
			d			= mmalloc(sizeof(PL_SHIELD_DATA));
			c->data 	= d;
			d->speed	= -17;							//反時計回り
			d->radius	= 0/*45*/;	//d->radius=35; 	/*半径初期値*/	/* 少しずつ広がるようにした */
		}
			c->mover	= player_move_shield;		//時計回り＆反時計回り
		//	c->mover	= player_move_shield/*2*/;		//反時計回り
		c->flags		|= SP_FLAG_VISIBLE;
		c->type 		= SP_PL_SHIELD;
		d->angle512 	= (ii*deg_360_to_512(45))/*i*/;
		/* 出始めはプレイヤー座標 */
		c->x			= player->x;//+co_s512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->w-s->w)>>1);
		c->y			= player->y;//+si_n512(((ii*deg_360_to_512(45))/*i*/))*d->radius+((player->h-s->h)>>1); //時計回り
		d->strength 	= 5;/* [***090214 追加 */
	}
}

static void player_add_levarie(SPRITE *s)	//魔理沙
{
	SPRITE *c;
	c=NULL;
	PL_LEVARIE_DATA *d;
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
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
//	ang512 = rad2deg512(((M_PI*2)/32))/*0*/;/*チルノ用(暫定的)*/
	ang512 = (0)/*0*/;/*チルノ用(暫定的)*/
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
//		d->angle512 	= ang512;
		double d_speed;
		if ((i&(2-1)/*%2*/)==0)
		{
			/*d->*/d_speed	= 1.0;
			c->alpha	= 150;
		}
		else
		{
			/*d->*/d_speed	= 1.2;
			c->alpha	= 180;
		}
		d->vx = co_s512((/*angle512*/ang512))*/*p->speed*/(d_speed)/**fps_fa_ctor*/;
		d->vy = si_n512((/*angle512*/ang512))*/*p->speed*/(d_speed)/**fps_fa_ctor*/;
//
		c->type 		= SP_PL_SHIELD;
		c->x			= s->x+((s->w-c->w)>>1);
		c->y			= s->y+((s->h-c->h)>>1);
		ang512 += rad2deg512((M_PI*2)/16) ; // 22.5度/360, π/8 /*0.39269908169872415481*/
	}
}

/* お嬢ボムの十字炎本体 */
static void player_add_cross_red_parrent(SPRITE *s) /* おぜう */ /* [***090220 追加 */
{
	PLAYER_DATA *pd = (PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	SPRITE *c;
	c				= sprite_add_file0("cross_red.png",10,PR_PLAYER,0);
	c->flags		|= SP_FLAG_VISIBLE;
	c->anim_speed	= 0;
	c->aktframe 	= 0;
	c->alpha		= 220;
	c->mover		= player_move_add_cross_red;
	c->type 		= SP_PL_SHIELD;
	c->x			= s->x+((s->w-c->w)>>1);
	c->y			= s->y+((s->h-c->h)>>1);
//
	PL_CROSS_FIRE_DATA *data;
	data			= mmalloc(sizeof(PL_CROSS_FIRE_DATA));
	c->data 		= data;
	data->strength	= 3;		/* 十字炎本体 の強さ */
//	data->angle512	= (0);
//	data->vx = player->x+((player->w-s->w)>>1);
//	data->vy = player->y+((player->h-s->h)>>1);
}


/*---------------------------------------------------------
	プレイヤー、誘導弾の移動制御
---------------------------------------------------------*/

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
			if (NULL!=s)
			{	s->type=SP_DELETE;}/* IDからスプライトの実体がみつからなければ、おしまい */
		}
		controller_remove(c);
	}
	else
	{
		for (i=hlaser_NUM_OF_ENEMIES-1;i>=1;i--)/*後ろから調べる*/
		{
			SPRITE *l/*=NULL*/;
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if ((NULL!=s)&&(NULL!=l))
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

/*---------------------------------------------------------
	プレイヤー誘導弾生成
---------------------------------------------------------*/

#if (1==USE_PLAYER_HOMING)
#endif //(1==USE_PLAYER_HOMING)
static void player_add_hlaser(SPRITE *src)
{
	int j;
	j=(rand()&(2-1));//for (j=0;j<2;j++)
	{
		CONTROLLER *c;
		c=controller_add();
	//	hlaser_NUM_OF_ENEMIES=12;		// [***090128		半分にしてみる
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
			s->x			= ((((int)src->x)+src->w)>>1)-5;
			s->y			= ((((int)src->y)+src->h)>>1)+15;
			s->flags		|= SP_FLAG_VISIBLE;
			s->aktframe 	= 5-((double)6.0/hlaser_NUM_OF_ENEMIES)*i;
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
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (0==(((int)pd->bomber_time)&0x3f))
	{
		player_add_hlaser(player);
	}
	s->aktframe 	= (((int)pd->bomber_time>>4)&0x03);/*"bomber_slow.png"*/
	s->x			= player->x+((player->w-s->w)>>1);
	s->y			= player->y+((player->h-s->h)>>1);
	if (pd->bomber_time<=0) {	s->type=SP_DELETE;}/* 時間制限でおしまい */
}
static void player_add_hlaser_parrent(SPRITE *s)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	pd->bonus_flag	|= BONUS_FLAG_02_BOMB_AUTO_GET_ITEM;	/*ボムによる自動収集可能*/
	SPRITE *c;
	c				= sprite_add_file0("bomber_slow.png",4,PR_BACK1,1);
	c->flags		|= SP_FLAG_VISIBLE;
	c->data 		= 0/*d*/;
	c->anim_speed	= 0;
	c->aktframe 	= 0;
	c->alpha		= 128/*220*/;
	c->mover		= player_move_add_bomber_hlaser;
	c->type 		= SP_PL_SHIELD;
	c->x			= s->x+((s->w-c->w)>>1);
	c->y			= s->y+((s->h-c->h)>>1);
}


/*---------------------------------------------------------
	プレイヤーキー操作
---------------------------------------------------------*/

//static int /*bomb_wait*/d->bomber_time;		/* ボムの有効時間 */	//次のボムを出せるまでの時間
static int weapon_List; 	//どの武器を装備しているか

enum _weapon_type
{
	WEAPON_L1=0,
	WEAPON_L2,
	WEAPON_L3,
	WEAPON_L4,
	WEAPON_L5,
//	WEAPON_L6,
	WP_MAX		/* 最大数 */
};
static void player_keycontrol(SPRITE *s, int hit_bomb_mode)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
	//if player is invisible (state gameover) allow no keycontrol
	if ((!(s->flags&SP_FLAG_VISIBLE))||(pd->state==PL_RESET))
	{	return;}

	/*---------------------------------------------------------
		プレイヤーボム関連処理
	---------------------------------------------------------*/
	if ((pd->bossmode==B03_BEFORE_EVENT) || (pd->bossmode==B06_AFTER_EVENT))
	{
		;	/* 会話イベント中はボム関連の処理はしない */
	}
	else
	{
	//	if (d->bomber_time>0)
		if (/*bomb_wait*/pd->bomber_time>0) 				//ボムウェイト処理
		{
			//	d->bomber_time-=fps_fa_ctor;
			/*bomb_wait*/pd->bomber_time--/*-=fps_fa_ctor*/;/**/
			if (/*bomb_wait*/pd->bomber_time<190)
			{
				pd->bonus_flag &= (~BONUS_FLAG_02_BOMB_AUTO_GET_ITEM);	/* ボムによる自動収集は終わり */
			}
		}
		else
		{
			pd->bomber_time=0;
			pd->extra_type=PLX_NONE;
		//}
		//if (/*bomb_wait*/d->bomber_time<=0)
		//{
			if (pd->bombs>0)
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
						pd->bomber_time = 200;			/* ボムの有効時間(設定無敵時間) */
					//	pd->save_delay	= 200+30;		//設定無敵時間。たまにシールドをすり抜ける者が現れるので
					//	pd->extra_type	= PLX_BOMB;
						player_add_shield(player);		//シールドの追加
						break;
					case (CIRNO*2+KOUSOKU_MODE):/*とりあえず*/
					case (MARISA*2+KOUSOKU_MODE):
					//	/*bomb_wait*/d->bomber_time 	= 100;
						pd->bomber_time = 100;			/* ボムの有効時間(設定無敵時間) */
					//	pd->save_delay	= 100+30/*200 5*/;	//設定無敵時間。つーかさすがにボム時間中無敵じゃないのはきつい
					//	pd->extra_type	= PLX_BOMB; 	/*←ここが無いと(extra_tem有効時に)ボムが発動出来ないよ*/
						player_add_levarie(player);
						break;

					case (REMILIA*2+KOUSOKU_MODE):	/* [***090220 追加 */
					//	/*bomb_wait*/d->bomber_time 	= 150/*300*/ /*400*/;	/*400(150+250)は明らかに長すぎる*/
						/* レミボムは強すぎるので、4発から3発に減らし、加速ボムにして当て難くした。 */
						/* ただしレミボムはボスに有効とし、当たれば強い。 */
						pd->bomber_time = 254/* 255==0xff==4発	254==0xfe==3発==0xbf=192 2発==0x7f 150==0x96*/;
					//	pd->save_delay	= 150+30;		//設定無敵時間。
					//	pd->extra_type	= PLX_BOMB;
						player_add_cross_red_parrent(player);
						break;

					case (REMILIA*2+TEISOKU_MODE):/*とりあえず*/
					case (REIMU*2+TEISOKU_MODE):/*とりあえず*/
					case (MARISA*2+TEISOKU_MODE):/*とりあえず*/
					case (CIRNO*2+TEISOKU_MODE):/*とりあえず*/
						pd->bomber_time = 400/*800*/;	/* ボムの有効時間(設定無敵時間)[※１仕様が変わったので半分にした] */
						/*player_move_add_bomber_hlaser*/player_add_hlaser_parrent(player);
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
					pd->save_delay		= pd->bomber_time + USER_BOMOUT_WAIT;/*実質無敵時間*/
					pd->extra_type		= PLX_BOMB;
					pd->hit_bomb_wait	= 0;
					pd->state			= PL_SAVE;
					pd->bombs--;
				}
			}
		}
	}
//
	if (1==hit_bomb_mode) return;
//
	/*
	if (keybo ard[KEY_LEFT]) {		direction=-1;
		if (s->x>0) {
			if (keybo ard[KEY_SLOW])	s->x-=(pd->player_speed/2)*fps_fa_ctor;
			else						s->x-=pd->player_speed*fps_fa_ctor;
		}
	}
	if (keybo ard[KEY_RIGHT]) { 	direction=1;
		if (s->x<GAME_WIDTH-s->w) { 	//プレイヤーのx座標 < スクリーンの横幅-プレイヤーの横幅
			if (keybo ard[KEY_SLOW])
				s->x+=(pd->player_speed/2)*fps_fa_ctor;
			else
				s->x+=pd->player_speed*fps_fa_ctor;
		}
	}
	if ((keybo ard[KEY_UP]) && (s->y>0)) {
		if (keybo ard[KEY_SLOW])	s->y-=(pd->player_speed/2)*fps_fa_ctor;
		else						s->y-=pd->player_speed*fps_fa_ctor;
		if (pd->weapon==127)	//128[***090126 	変更
			if (s->y<50)				pd->bonus=0x01;
		//parsys_add(NULL,50,0,s->x+((s->w)>>1),s->y+s->h,10,90,10,100,PIXELATE,NULL);
	}
	if ((keybo ard[KEY_DOWN]) && (s->y<screen->h-s->h)) {
		if (keybo ard[KEY_SLOW])	s->y+=(pd->player_speed/2)*fps_fa_ctor;
		else						s->y+=pd->player_speed*fps_fa_ctor;
		if (s->y>50)			pd->bonus&=~0x01;
	}
	*/
	//int direction=0;


	/*---------------------------------------------------------
		プレイヤー移動関連処理
	---------------------------------------------------------*/
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
	s->x += ((pd->player_speed*(shipMv[((my_pad&0xf0)>>4)][0] >>my_slow))>>8) /**fps_fa_ctor*/;
	s->y += ((pd->player_speed*(shipMv[((my_pad&0xf0)>>4)][1] >>my_slow))>>8) /**fps_fa_ctor*/;
	// はみだしたら修正。
		 if (s->x < 0)					{	s->x = 0;					}
	else if (s->x > GAME_WIDTH-s->w)	{	s->x = GAME_WIDTH-s->w; 	}
		 if (s->y < 0)					{	s->y = 0;					}
	else if (s->y > GAME_HEIGHT-s->h)	{	s->y = GAME_HEIGHT-s->h;	}

// > 【PSP】PSPで東方やりたい奴8【弾幕】
// > 200 ：名無しさん＠お腹いっぱい。：2009/01/27(火) 18:41:00 ID:xQgI7hCU
// > そろそろSアイテム消して欲しいな。
// > あとFULLパワー時のアイテム引き寄せラインはもうちょっと下の方がいいと思う。
// > 具体的にはこの辺り↓
// > ttp://www4.uploader.jp/user/isisuke/images/isisuke_uljp00081.bmp
// この意見はわたしも同様に思ったので、みんながそう思ったに違いない。
// この画像は484x283[dots]で、y==63[line]に赤線が引いてあった。
// pspの解像度は、 480x272[dots]なので x/272=63/283 , x=(63*272)/283 == 60.55[line]
// 差分/製作氏	50(49)[line] 少し上すぎる。
// スレ8の200氏 約60[line] (感覚的に)
// ここでキャラの高さ分もあるし、64(63)かもっと下72(71)ぐらいでもいいのでは？という訳で64にして実験してみる。
// 結果：やっぱもう少し下がいい。8[dots]といわず16[dots]ぐらい下がいい。再び実験してみる。
// 結果：こんなもんかな？というわけで80(79)[dots]に決定。もう8[dots]下88(87)でもいいかもね。
// 本家の感覚を大事にするならこんなもんだと思う。
#define SPLIT_LINE (80)
	/* MAX時のアイテム自動収集 */
	if (my_pad & PSP_KEY_UP/*PSP_CTRL_UP*/) 	/* 註：斜め上でも回収可能 */ /*&& (s->y>0)*/
	{
		if (pd->weapon==127) //128[***090123 変更 /*max==127==「128段階」*/
		{
			if (s->y < SPLIT_LINE/*50*/)/**/
			{
				pd->bonus_flag |= BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM;	/*上部自動収集可能*/
			}
		}
	}
	else if (my_pad & PSP_KEY_DOWN/*PSP_CTRL_DOWN*/)	/* 註：斜め下でもやめる */ /*&& (s->y<screen->h*-s->h)*/
	{
		//if (pd->bonus_flag & (BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM /*| BONUS_FLAG_02_BOMB_AUTO_GET_ITEM*/) )/*???たぶん*/
		{
			pd->bonus_flag &= (~BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM); 	/*上部自動収集不可*/
		}
	}
//
	/*---------------------------------------------------------
		プレイヤーショット関連処理
	---------------------------------------------------------*/
	if ( (pd->bossmode!=B03_BEFORE_EVENT) && (pd->bossmode!=B06_AFTER_EVENT))
	{
		if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
		{
			{static int weapon_interval=0;
				if (/*pd->*/weapon_interval>0)
				{	/*pd->*/weapon_interval--/*=fps_fa_ctor*/;}
				else //if (/*pd->*/weapon_interval<=0)
				{
					playChunk(0);
					switch (select_player)
					{
					case REIMU:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_shot(s,REIMU_SHOT_TYPE_00);	break;
						case WEAPON_L2: 	player_add_dual_shot(s);				break;
						case WEAPON_L3: 	player_add_triple_fuda(s);				break;
						case WEAPON_L4: 	player_add_quad_fuda(s);				break;
						case WEAPON_L5: 	player_add_five_fuda(s);				break;
						}
						break;
					case CIRNO:/*とりあえず*/
					case MARISA:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_shot(s,REIMU_SHOT_TYPE_00);	break;
						case WEAPON_L2: 	player_add_dual_shot(s);				break;
						case WEAPON_L3: 	player_add_dual_shot(s);				break;
						case WEAPON_L4: 	player_add_triple_star(s);				break;
						case WEAPON_L5: 	player_add_quad_star(s);				break;
						}
						break;
					case REMILIA:
						switch (weapon_List)
						{
						case WEAPON_L1: 	player_add_shot(s,REIMU_SHOT_TYPE_00);		break;
						case WEAPON_L2: 	player_add_shot(s,REIMU_SHOT_TYPE_00);		break;
						case WEAPON_L3: 	player_add_dual_shot(s);					break;
						case WEAPON_L4: 	player_add_dual_shot(s);					break;
						case WEAPON_L5: 	player_add_dual_shot(s);					break;
						#if (1==USE_PLAYER_WAVE)
						case WEAPON_L6: 	player_add_killray(s);						break;
						#endif //(1==USE_PLAYER_WAVE)
						}
						break;
					}
					#define BASE_WEAPON_L1		(4*0)
					#define BASE_WEAPON_L2		(4*1)
					#define BASE_WEAPON_L3		(4*2)
					#define BASE_WEAPON_L4		(4*3)
					#define BASE_WEAPON_L5		(4*4)
					#define BASE_WEAPON_MAX 	(4*5)
					static Uint8 www_tbl[BASE_WEAPON_MAX] =
					{/* REIMU MARISA REMILIA CIRNO */	/* レミ強すぎるので調整(+5) */
						 5,  6,  7+5,  9,	/* WEAPON_L1 */
						 5,  6,  6+5,  9,	/* WEAPON_L2 */
						 5,  5,  7+5,  9,	/* WEAPON_L3 */
						 5,  5,  6+5,  9,	/* WEAPON_L4 */
						 5,  5,  5+5,  9,	/* WEAPON_L5 */
					};
					/*pd->*/weapon_interval = www_tbl[(4*weapon_List)+select_player];
				}
			}
			#if 0
//			{static int extra_interval = 0;
//				if (/*pd->*/extra_interval>0)
//				{	/*pd->*/extra_interval--/*=fps_fa_ctor*/;}
//				else //if (pd->extra_interval<=0)
//				{
//					switch (pd->extra_type)
//					{
//					case PLX_HOMING:		player_add_homing(s);		/*pd->*/extra_interval=63/*60*/;			break;
//					case PLX_HLASER:		player_add_hlaser(s);		/*pd->*/extra_interval=63/*60*/;			break;
//					}
//				}
//			}
			#endif
		}
	}
	/*---------------------------------------------------------
		プレイヤーアニメーション関連処理
	---------------------------------------------------------*/
	{static int anim_delay;
		/*pd->*/anim_delay -= 1/*fps_fa_ctor*/;
		if (0 > /*pd->*/anim_delay)
		{
			/*pd->*/anim_delay = player_status[BASE_SPEED_ANIME+select_player]/*2*/;
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
}


/*---------------------------------------------------------
	武器レベルが変更された場合に、武器やオプションの状態をチェックして変更
---------------------------------------------------------*/
/* [pd->weaponが変更された場合に必ず行う後チェック] */
static void check_weapon_level(PLAYER_DATA *pd)
{
	int pd_weapon;
	pd_weapon=pd->weapon;
	/*---------------------------------------------------------
		武器の段階を決める
	---------------------------------------------------------*/
	//weaponの段階から今の装備を決める		//リストを見て装備の変更
	// [***090123	最大128へ。
	// (0-128の129段階だったが、0-127の128段階に修正)
		 if (pd_weapon <= ( 5-1)) 	{	weapon_List=WEAPON_L1;	}
	else if (pd_weapon <= (10-1)) 	{	weapon_List=WEAPON_L2;	}
	else if (pd_weapon <= (60-1)) 	{	weapon_List=WEAPON_L3;	}
	else if (pd_weapon <= (85-1)) 	{	weapon_List=WEAPON_L4;	}
	else /*if (pd_weapon<=(128-1))*/{	weapon_List=WEAPON_L5;	}	/*max==127==「128段階」*/
	//else							{	weapon_List=WEAPON_L6;	}
	/*---------------------------------------------------------
		プレイヤーオプションチェック(必要であればオプションをセット)
	---------------------------------------------------------*/
	if (pd_weapon > (35-1))
	{
	//	if (0==(pd->bonus_flag&BONUS_FLAG_08_OPTION_HIDE))/*(0==pd->option_flag)*/ /* オプションが消えてれば */
		if (0==option_flag) 	/* オプションが消えてれば */
		{
		//	pd->bonus_flag |= BONUS_FLAG_08_OPTION_HIDE;/*pd->option_flag=1;*/ /* オプションを追加 */
			option_flag=1;	/* オプションを追加 */
			switch (select_player)
			{
			case REIMU: 		re_add_option(player);		break;
			case MARISA:		ma_add_option(player);		break;
			case CIRNO:/*とりあえず*/
			case REMILIA:		oz_add_option(player);		break;/* [***090220 追加 */
			}
		}
	}
	else
	{
	//	pd->bonus_flag &= (~BONUS_FLAG_08_OPTION_HIDE);//pd->option_flag=0;/* オプションを消す */
		option_flag=0;	/* オプションを消す */
	}
}

/*---------------------------------------------------------
	プレイヤーあたり判定
---------------------------------------------------------*/

//グレイズ用、弾id格納場所。
//1つにしなかったのは複数の弾を同時にグレイズしたときのため。
//static int graze_check[5]={-1,-1,-1,-1,-1};

static int weapon_chain;		//どれだけ連続でwepon_upを取ったか

static void player_colcheck(SPRITE *s, int mask)
{
	//SDL_Surface *spimg;
	//is_graze=0;
	/*
		s		プレイヤー
		mask	調べる対象？
				敵か敵の弾かアイテムか
	*/
	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	SPRITE *c;	//対象
	if (NULL!=(c=sprite_colcheck(s,mask)))
	{
	//	int effect_sound_number;/* 効果音 */
	//	effect_sound_number = 99;/* 効果音なし */
		//
		PLAYER_DATA *pd=(PLAYER_DATA *)s->data;
		switch (c->type)
		{
		case SP_BONUS_00_FIREPOWER:
			if (pd->weapon<127) 	//128maxでないとき /*max==127==「128段階」*/
			{
				pd->weapon++;
				bonus_info_add(c, SP_BONUS_00_FIREPOWER/*"weapon.png"*/);
			}
			else					//maxの時
			{
				weapon_chain++;
				//ウェポンボーナス得点の表示
				bonus_info_any_score(c, (weapon_chain));
			}
			check_weapon_level(pd);/* [pd->weaponが変更された場合に必ず行う後チェック] */
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(5);
			break;

		case SP_BONUS_04_FIREPOWER_128: 	//ウェポンアイテム(F)		// [***090123		追加
			goto my_128;
		case SP_BONUS_01_FIREPOWER_G:		//ウェポンアイテム(中)		// [***090123		追加
			if (pd->weapon<(120-1))
			{
				pd->weapon+=8;
				bonus_info_add(c, SP_BONUS_00_FIREPOWER/*"weapon.png"*/);
			}
			else
			{
my_128:
				if (pd->weapon==127)/*127==128段階目*/
				{
					weapon_chain+=8;
					//ウェポンボーナス得点の表示
					bonus_info_any_score(c, (weapon_chain));
				}
				else
				{
					pd->weapon=127;/*max==127==「128段階」*/
				}
			}
			check_weapon_level(pd);/* [pd->weaponが変更された場合に必ず行う後チェック] */
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(5);
			break;

		case SP_BONUS_02_BOMB:
			//player_add_bomb(c);		// [*****本来はコメントアウトしないよ
			if (8 < pd->bombs)	{	goto add_1000pts;	}
			pd->bombs++;
			bonus_info_add(c, SP_BONUS_02_BOMB/*"firebomb.png"*/);
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(5);
			break;

		case SP_BONUS_03_HEALTH:
			if (8 < pd->lives)	{	goto add_1000pts;	}
			pd->lives++;
			bonus_info_add(c, SP_BONUS_03_HEALTH/*"energy.png"*/);
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(6);
			break;

		case SP_BONUS_07_ADDSPEED:
			if (pd->player_speed < player_status[BASE_SPEED_MAXIMUM+select_player]/*pd->player_speed_maximum*/)
			{	;	}	else	{	goto add_1000pts;	}
			pd->player_speed++;
			bonus_info_add(c, SP_BONUS_07_ADDSPEED/*"speed.png"*/);
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(5);
			break;

add_1000pts:
		case SP_BONUS_05_HOSI:		/*not_break;*/
		case SP_BONUS_06_COIN:
			if ( (GAME_HEIGHT/2) > c->y )	/* 上で取ると 1000pts. */
			{
				pd->score+=score(1000);
				bonus_info_add(c, SP_BONUS_05_HOSI/*"plus1000.png"*/);
			}
			else	/* 下で取ると 100pts. */
			{
				pd->score+=score(100);		// [***090123		変更
				bonus_info_add(c, SP_BONUS_06_COIN/*"plus100.png"*/);		// [***090123		変更
			}
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(5);
			break;


//add_1000pts:
//			pd->score+=score(1000);
//			bonus_info_add(c, SP_BONUS_05_HOSI/*"plus1000.png"*/);
//			c->type=SP_DELETE;/* おしまい */
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
			if (pd->weapon<(112-1)) 	// [***090123		変更
			{
				pd->weapon+=16;
				bonus_info_add(c, SP_BONUS_00_FIREPOWER/*"weapon.png"*/);
			}
			else
			if (pd->weapon==127)	// [***090123		変更/*max==127==「128段階」*/
			{
				weapon_chain+=16;
				goto add_1000pts;
			}
			else
			{
				pd->weapon=127; 		// [***090123		変更/*max==127==「128段階」*/
			}
			check_weapon_level(pd);/* [pd->weaponが変更された場合に必ず行う後チェック] */
			c->type=SP_DELETE;/* おしまい */
			/*effect_sound_number=*/playChunk(5);
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
//			bonus_info_add(c, /*"extra.png"*/);
//			//spimg=sprite_getcurrimg(c);
//			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			c->type=SP_DELETE;/* おしまい */
//			/*effect_sound_number=*/playChunk(5);
//			break;
//
//		case SP_BONUS_EXTRA_HLASER:
//			if (pd->bomber_time > 0)	{	goto add_1000pts;	} //ボム中はこれを取っても1000ポイント入るだけだよ
//			//
//		//	sprite_remove_all_type(SP_PL_SHIELD);
//			pd->extra_type		= PLX_BOMB/*PLX_HLASER*/;
//			pd->bomber_time 	= 1000;
//			bonus_info_s_text(c, "homing laser",FONT04);
//			bonus_info_add(c, /*"extra.png"*/);
//			//spimg=sprite_getcurrimg(c);
//			//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
//			//SDL_FreeSurface(spimg);
//			c->type=SP_DELETE;/* おしまい */
//			/*effect_sound_number=*/playChunk(5);
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
		case SP_EN_BULLET:		/*not_break;*/
	//	case SP_EN_LASER:		/*not_break;*/
	//	case SP_EN_BIGBULLET:	/*not_break;*/
	//	case SP_EN_BOSS02ICE:	/*not_break;*/
			is_graze=1;
			if (0==(c->flags & SP_FLAG_GRAZE)) /* グレイズ済? */ /* if (l==5) 同じIDの弾が無かった時*/
			{
				c->flags |= SP_FLAG_GRAZE;/*グレイズ済*/
				pd->graze++;/*グレイズ数*/
				pd->score += (score(100)+score(200)*difficulty);
				/*effect_sound_number=*/playChunk(9);
			}
			break;

		case SP_EN_BIGBULLET_DUMMY: 	//2つ目のスプライトは当たり判定が無い
			break;

		default:			//敵そのものに当たった場合
		//	spimg=sprite_getcurrimg(c);
		//	parsys_add(spimg, 2,2, c->x,c->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
		//	spimg=NULL;
			pd->state=PL_HIT_BOMB;		// [***090125		追加
			pd->enemy=c;/* あたった敵を覚えとく */
			//explosion_add(c->x+5,c->y+5,0,EXPLOSION_MINI00);		// [***090125		以下コメントアウト
			//pd->explode=0;

			//spimg=sprite_getcurrimg(s);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, 90, 10, 30, EXPLODE|DIFFSIZE, &pd->explode);
			//SDL_FreeSurface(spimg);
			//spimg=NULL;
			//c->type=SP_DELETE;/* おしまい */
			//s->flags&= ~SP_FLAG_VISIBLE;
			//pd->core->flags&= ~SP_FLAG_VISIBLE;	//○も消す
			//pd->lives--;
			//pd->bonus=0;
			/*effect_sound_number=*/playChunk(4);		// [***090127	変更先
		}
	//	if (99 != effect_sound_number)/* 効果音あれば */
	//	{
	//		playChunk(effect_sound_number);/* 効果音鳴らす */
	//	}
	}
}

/*---------------------------------------------------------
	プレイヤー移動
---------------------------------------------------------*/

static void player_move(SPRITE *s1)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)s1->data;
	switch (pd->state)
	{
	case PL_NORMAL:/*通常処理*/
		player_keycontrol(s1, 0);
		player_colcheck(s1,SP_SHOW_ENEMYS|SP_SHOW_ENEMY_WEAPONS|SP_SHOW_BONUS);
		break;

	case PL_HIT_BOMB:/* 喰らいボム受付中 */ 				// [***090125		追加
		if (player_status[BASE_HIT_BOMB_WAIT+select_player] < pd->hit_bomb_wait) /*<6*/
		{
			pd->hit_bomb_wait=0;
			/* 死亡確定 */
			pd->state			= PL_EXPLODE;
		}
		else
		{
			pd->hit_bomb_wait++;
			/* 喰らいボム受付中 */
			player_keycontrol(s1, 1);
		}
		break;

	case PL_EXPLODE:/*プレイヤー爆発中*/
	//	if (pd->lives<0) {	;	}else
		{
			//ここでアイテムを吐き出すようにすればそれっぽくなるかも
			/* GAMEOUTでもアイテム吐くべき(最後だけ吐かないのは違和感がある) */
			bonus_multi_add(s1, SP_BONUS_00_FIREPOWER, 5, BONUS_FLAG_RAND_XY);//というわけで実装
			// [***090124ちょっと追加
			bonus_multi_add(s1, ((pd->weapon>(30-1))?(SP_BONUS_01_FIREPOWER_G):(SP_BONUS_04_FIREPOWER_128)), 1, BONUS_FLAG_RAND_XY);
			/* 極端に弱くて死んだ場合の救済策: 上手い人は序盤で FULLPOWER になり、死んでもボム回収などで POWER を維持するので、 */
			/* 極端に弱くては死なない。 極端に弱くて死ぬのは結構下手だと思うので、その場合 FULLPOWERUP にした。 */
		}
//			pd->explode 		= 0;
//		if (0==pd->explode)
		{
			//playChunk(4); 	// [***090127	場所を移動する。
			/* 爆発エフェクト */

			/* あたった敵爆発 */
			explosion_add(pd->enemy->x+5,pd->enemy->y+5,0,EXPLOSION_MINI00);
			#if 0/* あたった敵は倒せない敵かもしれないので、ここで必ず消すのはまずい気がする */
			pd->enemy->type 	= SP_DELETE;/*???*/
			#endif
			/* 自分爆発 */
			explosion_add(s1->x+5, s1->y+5, 0,EXPLOSION_MINI00);
			explosion_add(s1->x+5, s1->y+20,0,EXPLOSION_MINI00);
			explosion_add(s1->x+20,s1->y+5, 0,EXPLOSION_MINI00);
			explosion_add(s1->x+20,s1->y+20,0,EXPLOSION_MINI00);

			/*	*/
			s1->flags			&= ~SP_FLAG_VISIBLE;		// 可視フラグのOFF(不可視)
			pd->core->flags 	&= ~SP_FLAG_VISIBLE;		// ○消し
			pd->extra_type		= PLX_NONE;
			pd->bomber_time 	= 0;			/* ←ここが無いせいで魔理沙(&チルノ)ボム撃てなくなってた */
			pd->bonus_flag		= BONUS_FLAG_00_NONE;

			pd->save_delay		= 150;						// 無敵時間
			pd->state			= PL_RESET; 				// 無敵状態？

			/* 残チャンス減らす */
			pd->lives--;
			if (pd->lives<0)	/* 残チャンスがなければ */
			{
				;/* GAMEOUT中 */
			}
			else
			{
				s1->flags			|= SP_FLAG_VISIBLE; 		// 可視フラグのON(可視)
				pd->core->flags 	|= SP_FLAG_VISIBLE; 		// ○表示
				//
				pd->bombs = player_status[BASE_BOMBS+select_player]/*3*/;			// ボム所持数の初期化
				if (pd->player_speed > player_status[BASE_SPEED_MINIMUM+select_player]/*pd->player_speed_minimum*/) 			//player_speed_minimum以下にならないように
				{	pd->player_speed--;}
				pd->weapon -= (difficulty*4)+12;	// 武器レベルの低下
				if ( pd->weapon<0)	{	pd->weapon=0;}
				weapon_chain=0; 					// weapon_chainの初期化
			}
			check_weapon_level(pd);/* [pd->weaponが変更された場合に必ず行う後チェック] */
			s1->x=GAME_WIDTH/2-((s1->w)>>1);			// プレイヤー位置の初期化
			s1->y=GAME_HEIGHT;
		}
		break;

	case PL_RESET:/*プレイヤー復活中１(画面外から復活地点へ点滅しながら移動中)*/
		if (pd->save_delay>120) //ちょっとした待ち時間
		{	pd->save_delay-=1/*fps_fa_ctor*/;}
		else
		{
			if (s1->y<220/*240*/)	//ちょっと前に出てくる
			{
				pd->state=PL_SAVE;
			}
			else
			{
				s1->y-=1/*fps_fa_ctor*/;
			//	if (((int)(s1->y/10)&(2-1)/*%2*/)==0)		// 点滅するかなと思って入れてみたけどしなかったよ→処理落ちしてた。
				if (((unsigned int)(s1->y)&((2*8)-1))==0)	// 点滅してるよ
				{
					s1->alpha		= 80;
					pd->core->alpha = 80;
				}
				else
				{
					s1->alpha		= 0/*255*/;
					pd->core->alpha = 0/*255*/;
				}
			}
		}
		break;/*??? [***090210 追加 */

	case PL_SAVE:/*プレイヤー復活中２*/
		player_keycontrol(s1, 0);
		player_colcheck(s1,SP_SHOW_BONUS);
		if (pd->save_delay>0)
		{
			pd->save_delay-=1/*fps_fa_ctor*/;
			s1->alpha=80;
			if (pd->lives<0)	// ゲームオーバー時の○の表示処理
			{	pd->core->alpha=0;}
			else
			{	pd->core->alpha=80;}
		}
		else
		{
			pd->state		= PL_NORMAL;
			s1->alpha		= 255;
			pd->core->alpha = 255;
		}
		break;
	}
}

/*---------------------------------------------------------
	プレイヤー初期化
---------------------------------------------------------*/

void player_init(void)
{
	player				= sprite_add_res( BASE_SHIP_MED_PNG+select_player );		/* [***090220 追加 */
	player->type		= SP_PLAYER;
	player->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	player->mover		= player_move;
	player->x			= GAME_WIDTH/2;
	player->y			= GAME_HEIGHT-player->h;
	player->aktframe	= 5;
	PLAYER_DATA *pd;
	pd					= mmalloc(sizeof(PLAYER_DATA));
	player->data		= pd;
//	pd->player_speed_maximum	= player_status[BASE_SPEED_MAXIMUM+select_player];
//	pd->player_speed_minimum	= player_status[BASE_SPEED_MINIMUM+select_player];
	pd->player_speed			= player_status[BASE_SPEED_MINIMUM+select_player]/*pd->player_speed_minimum*/;

	pd->bossmode		= B00_NONE;
	pd->state			= PL_RESET/*PL_SAVE*/;
	pd->save_delay		= 120;
//	pd->anim_delay		= 0;
	pd->weapon			= 0;
	pd->extra_type		= PLX_NONE;
	pd->bomber_time 	= 0;	/*==bomb_wait*/
//	pd->explode 		= 0;
//	pd->weapon_interval = 0;
//	pd->extra_interval	= 0;

	pd->graze			= 0;
	pd->lives			= player_status[BASE_LIVES+select_player];
	pd->bombs			= player_status[BASE_BOMBS+select_player];
	pd->score			= score(0);
	pd->bonus_flag		= BONUS_FLAG_00_NONE;	// 吸収 pd->option_flag 	= 0;/* オプションを消す */
	option_flag 		= 0;/* オプションを消す */
	pd->hit_bomb_wait	= 0;	// [***090125		追加
	pd->boss			= NULL;
	pd->core			= player_add_core(player); // ○の追加

	weapon_List 		= 0;
	weapon_chain		= 0;
}
