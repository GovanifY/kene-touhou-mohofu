
#include "bonus.h"

//extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;		//[***090116		追加

/*---------------------------------------------------------
	ボーナスアイテムの移動
---------------------------------------------------------*/

/* 個々のボーナスアイテムのステータスデーター */
typedef struct
{
	/*double*/int y_sum256; 	/* ボーナスアイテム投げ出し用 y軸 積算値(y軸、上方がマイナス) */
	/*double*/int angle512;
	int flags00;				// 収集フラグ	// [***090116		変更
} BONUS_DATA;

static void bonus_move(SPRITE *src)
{
	BONUS_DATA *d=(BONUS_DATA *)src->data;
	/* 自動収集モードのどれかが作動してたら、 */
	if (0 != (
		(((PLAYER_DATA *)player->data)->bonus_flag) &
		(BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM |		/* MAX時の上回収 */
		 BONUS_FLAG_02_BOMB_AUTO_GET_ITEM | 			/* ボム発動中のみ回収 */
		 BONUS_FLAG_03_SCORE_AUTO_GET_ITEM) ))			/* ボス撃破時の一時回収 */
	{
		d->flags00 |= BONUS_FLAG_COLLECT/*1*/;	/* そのアイテムは自動収集にする */
	}
	/* 自動収集でなければ、単純に放物線を描いて落ちる */
	if (0 == ((d->flags00)&BONUS_FLAG_COLLECT))
	{
		if (d->y_sum256 < t256(3) ) 			// [***090123.0220	変更5=>4=>3
		{
			d->y_sum256 += d->angle512; 		/* x1.5 */
		}
		src->y += (t256_to_double(d->y_sum256))/**fps_factor*/;
		if (src->y > GAME_HEIGHT)
		{
			src->type = SP_DELETE;				/* 画面外に出たらおしまい */
		}
	}
	else	/* 自動収集ならば、自分に集まる */
	{
		d->y_sum256 -= t256(1/*5*0.5*/);
		if (d->y_sum256 < t256(0) )
		{
			d->y_sum256 = t256(/*4*/ 8 /*20*/ /*5.6 14*/);		/* 5.6==(14/(5*0.5))*/
			d->angle512 = atan_512(player->y-src->y,player->x-src->x);
		}
		/*自分に集まる*/
		src->x += co_s512((d->angle512))*/*3*/4/*12*/ /**fps_factor*/;
		src->y += si_n512((d->angle512))*/*3*/4/*12*/ /**fps_factor*/;
	}
}

/*---------------------------------------------------------
	取ったアイテムの種類を説明表示
---------------------------------------------------------*/
/*---------------------------------------------------------
	取った得点を小さな漢字で説明表示
---------------------------------------------------------*/

static void bonus_info_move(SPRITE *src)
{
//	BIT_DATA *b=(BIT_DATA *)src->data;
	if (7 < src->alpha) /*<= 0*/ /*>=3*/ /**fps_factor*/
	{
		src->alpha	-= 3/**fps_factor*/;
	//	b->distance = src->alpha;
		/* src->y	+= 2 *fps_factor;  下に移動(ketm)移動速度速すぎ */
		src->y -= /*1*/0.5/**fps_factor*/; /* 上に移動(ketmと同じ) */	/* 上に移動 */
	}
	else
	{
		src->type = SP_DELETE;/* おしまい */
	}
}



/*---------------------------------------------------------
	アイテム出現させる子関数
---------------------------------------------------------*/

static SPRITE *bonus_sel(SPRITE *src/*int x, int y*/, int type)
{
//	アイテムの種類を選ぶ
	SPRITE *sss;
	sss 			= sprite_add_file("bonus_items.png",8,PR_BONUS);	sss->anim_speed=0;
	sss->anim_speed = 0;
	sss->aktframe	= ((/*SP_BONUS_FIREPOWER-*/type)&0x07);
//	出現位置を決める
	#define OFFSET_X64		(32)/* 64はずらし分 2のn乗の必要有り */
	#define ITEM_WIDTH16	(16)/* 16はアイテム幅 */
	#define ITEM_X_LIMIT	(GAME_WIDTH+OFFSET_X64-ITEM_WIDTH16)
	int x;
	x = src->x;
	x += ((rand()&((OFFSET_X64+OFFSET_X64)-1)));
	if (x < OFFSET_X64)
	{
		x = OFFSET_X64;
	}
	else
	if (x > (ITEM_X_LIMIT))
	{
		x = (ITEM_X_LIMIT);
	}
//	登録する
	sss->x		= x-OFFSET_X64;
	sss->y		= src->y;
	sss->flags	|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	sss->type	= type;
	sss->mover	= bonus_move;
	return (sss);
	#undef OFFSET_X64
	#undef ITEM_WIDTH16
	#undef ITEM_X_LIMIT
}

/*---------------------------------------------------------
	ボーナスアイテムを登録して出現させる
---------------------------------------------------------*/

/* [***090125	追加: up_flags の BONUS_FLAG_COLLECT ビットがオンでプレイヤーに集まります。 */
void bonus_multi_add(SPRITE *src/*int x, int y*/, int type, int num, int up_flags)
{
	int i;
	for (i=0;i<num;i++)
	{
		SPRITE *sss;
	//	int dx;
	//	int dy;
	//	int x;
	//	int y;
	//	x	= src->x/*+(rand()&(8-1))*/;
	//	y	= src->y/*+(rand()&(8-1))*/;
	//	dx	= 0;
	//	dy	= 0;
		/*if (BONUS_FLAG_RAND_X&up_flags)*/ //{ 	dx=((rand()&(128-1))-64);}
		/*if (BONUS_FLAG_RAND_Y&up_flags)*/ //{ 	dy=((rand()&(64-1))-32);}
	//	sss 			= bonus_sel(x + ((rand()%100)-50), y + ((rand()%60)-30), type);
		sss 			= bonus_sel(src, type);
		if (NULL==sss) return;
	//
		BONUS_DATA *data;
		data			= mmalloc(sizeof(BONUS_DATA));
		sss->data		= data;
		/* 画面下方に居る程、投げ出し速度を速くする */
		data->y_sum256	= -(/*256*/((int)(src->y))<<1)-(rand()&0xff);	/* ボーナスアイテム投げ出し初期値(y軸、上方がマイナス) */
	//	data->y_sum256	= t256(-5*0.5); 	//ちょっと変更
	//	data->y_sum256	= t256(-2*0.5); 	/* ボーナスアイテム投げ出し初期値(y軸、上方がマイナス) */
		data->angle512	= rad2deg512(0.08);
		data->flags00	= (up_flags&BONUS_FLAG_COLLECT);
	//
	}
}


/*---------------------------------------------------------
	ボーナスアイテムの名称の表示
---------------------------------------------------------*/

void bonus_info_add(SPRITE *src/*int x, int y*/, int info_type/*char *filename*/)
{
	/*const*/ char *info_name[8]=
	{
	/* SP_BONUS_00_FIREPOWER:		*/ "weapon.png",
	/* SP_BONUS_01_FIREPOWER_G: 	*/ "xxx",
	/* SP_BONUS_02_BOMB:			*/ "firebomb.png",
	/* SP_BONUS_03_HEALTH:			*/ "energy.png",
	/* SP_BONUS_04_FIREPOWER_128:	*/ "xxx",
	/* SP_BONUS_05_HOSI:			*/ "plus1000.png",
	/* SP_BONUS_06_COIN:			*/ "plus100.png",
	/* SP_BONUS_07_ADDSPEED:		*/ "speed.png",
	};
	SPRITE *c;
	c			= sprite_add_file((char *)info_name[(info_type)&(8-1)], 1, PR_BONUS);
	c->flags	= SP_FLAG_VISIBLE;
	c->type 	= SP_ETC;
	c->x		= src->x;
	c->y		= src->y;
	c->alpha	= 255;
	c->data 	= 0/*mmalloc(sizeof(BIT_DATA))*/;
	c->mover	= bonus_info_move;
}


/*---------------------------------------------------------
	取ったアイテムの得点を説明表示の子関数
---------------------------------------------------------*/

static void bonus_info_s_text(SPRITE *src/*int x, int y*/, char *text, int font)
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
	s->data 	= 0/*mmalloc(sizeof(BIT_DATA))*/;	//	((BIT_DATA *)s->data)->distance=255;
	s->mover	= bonus_info_move;
}

/*---------------------------------------------------------
	取ったアイテムの得点を説明表示(固定得点の場合)
---------------------------------------------------------*/

void bonus_info_score(SPRITE *src/*int x, int y*/, int score_type)
{
	/*const*/ Uint8 score_tbl[4]=
	{
	/* 0==SCORE_30*/	score(30),
	/* 1==SCORE_50*/	score(50),
	/* 2==SCORE_100*/	score(100),
	/* 3==SCORE_1000*/	score(1000),
	};
	((PLAYER_DATA *)player->data)->score += score_tbl[score_type];
//
	/*const*/ char *score_name[4]=
	{
	/* 0==SCORE_30*/	"30",
	/* 1==SCORE_50*/	"50",
	/* 2==SCORE_100*/	"100",
	/* 3==SCORE_1000*/	"1000",
	};
	bonus_info_s_text(src/*int x, int y*/, (char *)score_name[(score_type)&(4-1)], FONT07);
}

/*---------------------------------------------------------
	取ったアイテムの得点を説明表示(任意得点の場合)
---------------------------------------------------------*/

void bonus_info_any_score(SPRITE *src/*int x, int y*/, int score_num)
{
	((PLAYER_DATA *)player->data)->score += score_num;
//
	char buffer[16];
	sprintf(buffer,"%d0", score_num );/* 速度の要る場所では sp rintf() は使わない方が良い */		/* 文字列処理(sp rintf)は非常に遅い */
	bonus_info_s_text(src/*int x, int y*/, buffer, FONT07);
}




///--------- 以下メモ

//typedef struct
//{
//	/*double*/int distance;
//} BIT_DATA;

//	SDL_Surface *s;
		//s=sprite_getcurrimg(c);
		//parsys_add(s,1,1,c->x,c->y,30,0,0,100,PIXELIZE,NULL);

	#if 0
	switch (type)
	{
	case SP_BONUS_FIREPOWER:	s=sprite_add_file("bonus_p.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090123		変更
	case SP_BONUS_FIREPOWER_G:	s=sprite_add_file("bonus_p_.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090124		追加
	case SP_BONUS_BOMB: 		s=sprite_add_file("bonus_f.png",1,PR_BONUS);	s->anim_speed=0;	break;		//[***090123		変更
	case SP_BONUS_HEALTH:		s=sprite_add_file("bonus_h.png",9,PR_BONUS);	s->anim_speed=2;	break;
//	case SP_BONUS_EXTRA_HOMING:
//	case SP_BONUS_EXTRA_HLASER:
//	case SP_BONUS_EXTRA_SHIELD:
//								s=sprite_add_file("bonus_x.png",16,PR_BONUS);	s->anim_speed=3;	break;
	case SP_BONUS_COIN: 		s=sprite_add_file("coin.png",1,PR_BONUS);		s->anim_speed=0;	break;		//[***090123		変更
	case SP_BONUS_ADDSPEED: 	s=sprite_add_file("bonus_s.png",16,PR_BONUS);	s->anim_speed=2;	break;
	default:
		CHECKPOINT;
		error(ERR_WARN,"cant add unknown bonus! debug me! please! now!");
		return NULL;
	}
	#else
//	s=sprite_add_file("bonus_items.png",9,PR_BONUS);	s->anim_speed=0;
//	s->aktframe = SP_BONUS_FIREPOWER-type;
	#endif

	#if 0
	s=sprite_add_file("bonus_items.png",8,PR_BONUS);	s->anim_speed=0;
	s->anim_speed=0;
	switch (type)
	{
	case SP_BONUS_00_FIREPOWER: 	s->aktframe =0; break;		//[***090123		変更
	case SP_BONUS_01_FIREPOWER_G:	s->aktframe =1; break;		//[***090124		追加
	case SP_BONUS_02_BOMB:			s->aktframe =2; break;		//[***090123		変更
	case SP_BONUS_03_HEALTH:		s->aktframe =3; break;
	case SP_BONUS_04_FIREPOWER_128: s->aktframe =4; break;		//[***090405		追加
	case SP_BONUS_05_HOSI:			s->aktframe =5; break;		//[***090405		追加
//	case SP_BONUS_EXTRA_HOMING:
//	case SP_BONUS_EXTRA_HLASER:
//	case SP_BONUS_EXTRA_SHIELD: 					break;
	case SP_BONUS_06_COIN:			s->aktframe =6; break;		//[***090123		変更
	case SP_BONUS_07_ADDSPEED:		s->aktframe =7; break;
	default:
		CHECKPOINT;
		error(ERR_FATAL,"cant add unknown bonus! debug me! please! now!");
		return NULL;
	}
	#endif

	#if 0
	if (x </*GAME_WIDTH-20*/600/*600*/) x += 20;		/*600は元のPC版KETMの横幅修正忘れ(640-40==600)*/
	else							x -= 10;
	#else
	//x += 20;		/*重なると見にくいので横に20dotずらす*/
	//if ( GAME_WIDTH < x ) {	x = ((GAME_WIDTH - x) & (512-1));	}	/* はみ出たら折り返す、ついでにマスク */
//	x += 16;		/*重なると見にくいので横に20dotずらす*/
//	if ( (GAME_WIDTH/*-16*/) < (unsigned int)(x) ) { x = (GAME_WIDTH-256-32)+((x) & (256-1));	}	/* はみ出たら、なんとなく右側の方にする */
	#endif
	//	#if 0
	//	if (/*s->*/x < /*GAME_WIDTH-20*/480/*480*/) x += 20;
	//	else							x -= 10;
	//	#else
	//	x += 20;		/*重なると見にくいので横に20dotずらす*/
	//	if ( GAME_WIDTH < x ) { x = (GAME_WIDTH - x);	}	/* はみ出たら折り返す */
	//	#endif
