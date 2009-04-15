/*
アリス

色々問題はあるが、遅すぎるので fps_factor やめた。

*/
#include "enemy.h"

typedef struct _boss01_data
{
	ENEMY_BASE b;
	int health_flag;
} BOSS01_DATA;

// FLG_FIRE1 弾フラグ。真ん中の人形が壊れていて、かつ指定位置に付いた時。onになる。
#define FLG_FIRE1 0x01/*左上の藍い娘(人形)が指定位置に付いたか*/
#define FLG_FIRE2 0x02/*右上の碧の娘(人形)が指定位置に付いたか*/
//
#define FLG_DOLL0 0x04/*左上の藍い娘(人形)*/
#define FLG_DOLL1 0x08/*アリス本人*/
#define FLG_DOLL2 0x10/*右上の碧の娘(人形)*/
#define FLG_DOLL3 0x20/*左前の人形*/
#define FLG_DOLL4 0x40/*真中の紅い娘(人形)*/
#define FLG_DOLL5 0x80/*右前の人形*/


#define FLG_ALL_CAST   (FLG_DOLL0|FLG_DOLL1|FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5)/*全員*/
#define FLG_ALL_DOLL   (FLG_DOLL0|			FLG_DOLL2|FLG_DOLL3|FLG_DOLL4|FLG_DOLL5)/*アリス以外*/

static unsigned int b01flags/*=0*/;
static int b01level;
static int vvv256;
static unsigned int aaa_count;
static int b01state;

static int bwait1;		//弾発射までの待ち時間。
static int bwait2;
static int br1_angle512;		//弾の角度
static int br2_angle512;

static SPRITE *sb01[6];

/* ボスにプレイヤーが体当たりした場合。
	プレイヤーはプレイヤー側処理で死ぬとして、
	その場合のボス側の特殊処理。
 boss wurde von player ber??rt */
static void callback_enemy_boss01_hitbyplayer(SPRITE *c)
{
}

/* 人員の配置メモ
[0][1][2]
[3][4][5]
*/

/* boss wurde von player-weapon ber??rt */
static void callback_enemy_boss01_hitbyweapon_dummy(SPRITE *c, SPRITE *s/*, int angle*/)
{	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
}
static void callback_enemy_boss01_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/)
{
	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
//	if (((BOSS01_DATA *)sb01[1]->data)->state<4)
//	{
//		return;
//	}

	/*
		c = boss sprite
		s = player's weapon
	*/
	int i;
	for (i=0;i<6;i++)
	{
		if (c==sb01[i])
		{
			break;
		}
	}
//
	switch (i)
	{
	case 0: /* is left back? */
		if (0==(b01flags&FLG_DOLL3)/*sb01[3]->flags&SP_FLAG_VISIBLE*/)		{					i=3;	}	/* left front */
		break;
	case 1: /* is boss core? (center back) */
			 if (0==(b01flags&FLG_DOLL3)/*sb01[3]->flags&SP_FLAG_VISIBLE*/) {					i=3;	}	/* left   front */
		else if (0==(b01flags&FLG_DOLL5)/*sb01[5]->flags&SP_FLAG_VISIBLE*/) {					i=5;	}	/* right  front */
		else if (0==(b01flags&FLG_DOLL0)/*sb01[0]->flags&SP_FLAG_VISIBLE*/) {	if (0==(b01flags&FLG_FIRE1)) {	i=0;}	}	/* left   back */
		else if (0==(b01flags&FLG_DOLL2)/*sb01[2]->flags&SP_FLAG_VISIBLE*/) {	if (0==(b01flags&FLG_FIRE2)) {	i=2;}	}	/* right  back */
		else if (0==(b01flags&FLG_DOLL4)/*sb01[4]->flags&SP_FLAG_VISIBLE*/) {					i=4;	}	/* center front */
	/*	else										{					i=1;	}	   center back */
		break;
	case 2: /* is right back? */
		if (0==(b01flags&FLG_DOLL5)/*sb01[5]->flags&SP_FLAG_VISIBLE*/)		{					i=5;	}
		break;
	//case 4: /* center front */
	//	//完全に姿を現すまで攻撃を一切受けない。代わりにアリス本体が攻撃を受け止める。
	//	//if (((BOSS01_DATA *)sb01[1]->data)->state<4)	{	i=1;}
	}
//
	BOSS01_DATA *b;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	b=(BOSS01_DATA *)sb01[i]->data;
	if (/*((BOSS01_DATA *)sb01[1]->data)->state*/b01state>1)
	{
		b->b.health-=w->strength;
	}

	explosion_add(s->x,s->y,0,EXPLOSION_MINI00);
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);

	if ((b->b.health<=15)&&(b->health_flag==0))
	{
		b->health_flag=1;
		explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,0,EXPLOSION_FIRE08);
	}
	if (b->b.health<=0)/*該当者の体力切れなら*/
	{
		b01flags			|= (FLG_DOLL0<<i);/*倒した人をONにする*/
		explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,0,EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
		sb01[i]->flags		&= ~SP_FLAG_VISIBLE;
		bonus_multi_add(sb01[i], SP_BONUS_06_COIN, 7, BONUS_FLAG_COLLECT);
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		if (i==1)	//アリスを倒すと皆破壊される。
		{
			b01flags |= FLG_ALL_CAST;	//アリスを倒すと皆破壊される。
			//bonus_multi_add(sb01[i], SP_BONUS_COIN,7);
			explosion_add(sb01[0]->x+(/*sb01[0]->w*/62/(2*2)),sb01[0]->y+(/*sb01[0]->h*/49/2),0,EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
			sb01[0]->flags	&= ~SP_FLAG_VISIBLE;
			explosion_add(sb01[2]->x+(/*sb01[2]->w*/53/(2*2)),sb01[2]->y+(/*sb01[2]->h*/42/2),0,EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
			sb01[2]->flags	&= ~SP_FLAG_VISIBLE;
		}
	}

//	int j;
//	j=0;
//	for (i=0;i<6;i++)
//	{
//	//	if (b->health_flag==1)	{	sb01[i]->aktframe=1;}
//		if (sb01[i]->flags&SP_FLAG_VISIBLE)
//		{	j++;}
//	}
//	if (j==0)
	if (FLG_ALL_CAST==(b01flags&FLG_ALL_CAST))/*全員倒した？*/
	{
		/* boss komplett zerst??t */
		for (i=0;i<6;i++)
		{
			sb01[i]->type=SP_DELETE;
		//	explosion_add(sb01[i]->x+sb01[i]->w/2,sb01[i]->y+sb01[i]->h/2,(rand()&(16-1)/*%20*/),EXPLOSION_FIRE08);
			explosion_add_circle(sb01[i]);
		}
		((PLAYER_DATA *)player->data)->bossmode=B02_DEATH_WAIT;
	}
}


static void enemy_boss01_setpos(int x256, int y256)
{
	int x;
	int y;
	x=(x256>>8);
	y=(y256>>8);
	/*
		真ん中の人形が破壊されると、アリスの左右にいる人形の行動パターンが変わる。
	*/
	if (0==(b01flags&FLG_DOLL4)/*sb01[4]->flags&SP_FLAG_VISIBLE*/)	//真ん中の人形の生存確認
	{
		sb01[0]->x=x+(3-4+4)/*offset*/;
		sb01[2]->x=x+(1-4+4)/*offset*/+/*sb01[1]->w*/60/2+/*sb01[0]->w*/62/2;
		sb01[0]->y=y+(15)/*offset*/;
		sb01[2]->y=y+(15)/*offset*/;
	}
	else	/* 真ん中の人形が破壊されると */
	{
		if (0==(b01flags&FLG_FIRE1))	//fire1が発射できるかどうか
		{
			#if 0
			/* 考え方はこれで正しい。Visual C++ならこれで問題ない。 */
			/* しかし GCC は超越関数は constにしてくれないので、 */
			/* これでは遅すぎる。(処理落ちしてずれたりしてた) */
			sb01[0]->x+=co s((M_PI/4)*5)*3/**fps_factor*/;		//左斜め後ろに移動
			sb01[0]->y+=si n((M_PI/4)*5)*3/**fps_factor*/;
			#else
			/* GCC はあらかじめ計算しとこう */
			sb01[0]->x-=2/**fps_factor*/;		//左斜め後ろに移動
			sb01[0]->y-=2/**fps_factor*/;
			#endif
			if ((sb01[0]->x<10+(3-4+4)/*offset*/)||
				(sb01[0]->y<10+(15)/*offset*/))
			{	b01flags |= FLG_FIRE1;	}		//fire1移動完了
		}
		if (0==(b01flags&FLG_FIRE2))	//fire2が発射できるかどうか
		{
			#if 0
			/* GCC は超越関数は constにしてくれない */
			sb01[2]->x+=co s((M_PI/4)*7)*3/**fps_factor*/;		//右斜め後ろに移動
			sb01[2]->y+=si n((M_PI/4)*7)*3/**fps_factor*/;
			#else
			/* GCC はあらかじめ計算しとこう */
			sb01[2]->x+=2/**fps_factor*/;		//右斜め後ろに移動
			sb01[2]->y-=2/**fps_factor*/;
			#endif
			if ((sb01[2]->x>GAME_WIDTH-50+(1-4+4)/*offset*/)||
				(sb01[2]->y<10+(15)/*offset*/))
			{	b01flags |= FLG_FIRE2;	}		//fire2移動完了
		}
	}
	sb01[1]->x=x+62/2/*sb01[0]->w*/;			//通常処理
	sb01[3]->x=x+(9-3)/*offset*/;
	sb01[4]->x=x/*sb01[3]->x*/+62/2/*sb01[3]->w*/+(7-4)/*offset*/;
	sb01[5]->x=x/*sb01[4]->x*/+62/2/*sb01[3]->w*/+60/2/*sb01[4]->w*/+(8-3)/*offset*/;

	sb01[1]->y=y;
	sb01[3]->y=y+49/*sb01[0]->h*/+(3)/*offset*/;
	sb01[4]->y=y+42/*sb01[1]->h*/+(8)/*offset*/;
	sb01[5]->y=y+42/*sb01[2]->h*/+(10)/*offset*/;

	BOSS01_DATA *bdata;
	bdata=(BOSS01_DATA *)sb01[0]->data;
	if ((FLG_FIRE1==(b01flags&FLG_FIRE1))&&(bdata->b.health>0)) //破壊済みなのに攻撃してこないように
	{	/* 東方っぽく無いのは、弾速が速すぎるからだと思うので修正(3+b01level) */
		if (bwait1<=0)
		{
			bwait1=5+(3-difficulty)*5;
			br1_angle512 += deg_360_to_512(10);
			enemy_gr_bullet_create(sb01[0], (1+b01level)/*(3+b01level)*/ /*(4+bdata->level)*/, /*deg512_2rad*/( (br1_angle512&255/*%180*/)-deg_360_to_512(90) ), t256(0.04));
		}
		else
		{	bwait1--;}
	}
	bdata=(BOSS01_DATA *)sb01[2]->data;
	if ((FLG_FIRE2==(b01flags&FLG_FIRE2))&&(bdata->b.health>0))
	{
		if (bwait2<=0)
		{
			bwait2=5+(3-difficulty)*5;
			br2_angle512 += deg_360_to_512(10);
			enemy_gr_bullet_create(sb01[2], (1+b01level)/*(3+b01level)*/ /*(4+bdata->level)*/, /*deg512_2rad*/( (br2_angle512&255/*%180*/)+deg_360_to_512(90) ), t256(0.04));
		}
		else
		{	bwait2--;}
	}
}

/* where / 0: left, 1: right, 2: bombenhagel */
static void enemy_boss01_LR_dole_fire(void)
{
	//bullet_way(sb01[4],100, 0, M_PI*2/100,2);
	if (/*((BOSS01_DATA *)sb01[1]->data)->state*/b01state>2)
	{
	//case 0://left
		if (0==(b01flags&FLG_DOLL3)/*sb01[3]->flags&SP_FLAG_VISIBLE*/)
		{	enemy_bullet_create(sb01[3],4-difficulty /*5*/);}
	//	break;
	//case 1://right
		if (0==(b01flags&FLG_DOLL5)/*sb01[5]->flags&SP_FLAG_VISIBLE*/)
		{	enemy_bullet_create(sb01[5],4+difficulty /*5*/);}
	//	break;
	}
}



static void enemy_boss01_nway_fire(SPRITE *src, int x_offset)
{
	//bullet_way(sb01[4],100, 0, M_PI*2/100,2);
	{
	//case 2://bombenhagel
		if (/*((BOSS01_DATA *)sb01[1]->data)->state*/b01state>2)
		{
			playChunk(14);
			aaa_count=16/*8*/;
		//
			int angle512;
			for (angle512=deg_360_to_512(0);angle512<=deg_360_to_512(180);angle512+=deg_360_to_512(20))
			{
				const short spd_tbl1[4]= { t256(3), t256(3), t256(2), t256(1) };/* 遅い方が難しい */
				bullet_create_haridan180(src, angle512, spd_tbl1[difficulty]/*(4-difficulty)*/ /*4*/, /*offsx*/x_offset/*(src->w/2)*/, /*offsy*/0/*(src->h/2)*/ );
			}
		}
		if (FLG_DOLL4==(b01flags&FLG_DOLL4)/*(0==(sb01[4]->flags&SP_FLAG_VISIBLE))*/)	/* 真ん中の人形が破壊されると */
		{
			/* 東方っぽく無いのは、弾速が速すぎるからだと思うので修正 */
			const char spd_tbl2[4]= { 3,3,4,4 };
			enemy_n_way_bullet_type(sb01[1], BU_TYPE01_KUGEL_PNG/*"kugel.png", 0*/, 12, spd_tbl2[difficulty]/*(3+difficulty)*/, ANGLE_JIKINERAI_KISUDAN );	/* [A]を[B]に変更した(殆ど同じ) */
			//[A]	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w	   )/2));
			//[B]	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x-((player->w+s->w)/2));/*???(original)*/	/* 奇数弾の場合に自機狙い */
			//[C]	angle512=atan_512(player->y-s->y+((player->h-s->h)/2),player->x-s->x+((player->w-s->w)/2));/*???*/			/* 偶数弾の場合に自機狙い */
		}
	//	break;
	}
}
static void enemy_vvv(void)
{
	vvv256++/*vvv256+=vvv256*/;
	if (vvv256> t256(1) )
	{	vvv256= t256(1);}
//
	if (aaa_count)
	{
		aaa_count--;
		sb01[1]->aktframe=4;
	}
	else
	{
		if (t256(0.3)>vvv256)		{	sb01[1]->aktframe=3;}
		else
		if (t256(0.5)>vvv256)		{	sb01[1]->aktframe=2;}
		else
		if (t256(0.7)>vvv256)		{	sb01[1]->aktframe=1;}
		else						{	sb01[1]->aktframe=0;}
	}
}

static void enemy_boss01_move(SPRITE *c)
{
//	BOSS01_DATA *b=(BOSS01_DATA *)c->data;
	static /*double*/int x256;
	static /*double*/int y256;
	static /*double*/int my_wait;
	static /*double*/int firewait1;
	static int firewait2;
	static int firewait3;

	switch (/*b->state*/b01state)
	{
	case 0:
	//	x256		= (GAME_WIDTH*256/2); // /*t256*/256*(GAME_WIDTH/2-(/*sb01[0]->w*/62/2+/*sb01[1]->w*/60/2+/*sb01[2]->w*/53/2)/2);/*???*/
	//	x256		= ( 256*( (GAME_WIDTH)-(62/2)-(60/2)-(53/2) ) )/(2);/*???*/  /*sb01[0]->w*/ /*sb01[1]->w*/ /*sb01[2]->w*/
		x256		= t256( 146);
		y256		= t256(-100);
		enemy_boss01_setpos(x256,y256);
		/*b->state*/b01state	= 1;
		firewait1	= 45;
		firewait2	= 4;
		firewait3	= 0;
		break;
	case 1:
		y256 += t256(2)/**fps_factor*/;
		enemy_boss01_setpos(x256,y256);
		if (y256 >= t256(20) )
		{
			/*b->state*/b01state=2;
			my_wait=0;
			((PLAYER_DATA *)player->data)->bossmode=B05_BEFORE_LOAD;
		}
		break;
	case 2:
		if (((PLAYER_DATA *)player->data)->bossmode==B01_BATTLE)
		{
			my_wait += (1)/*fps_factor*/;
			if (my_wait >= 40)
			{
				/*b->state*/b01state=3;
			}
		}
		break;
	case 3:/*下へ移動*/
		y256 += t256(1)/*fps_factor*/;
		enemy_boss01_setpos(x256,y256);
		if (y256 >= t256(50) )
		{
			/*b->state*/b01state=4;
			my_wait=0;
			/* プレイヤー弾受け付け、コールバックを登録 */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss01_hitbyweapon;
		}
		break;
/*完全に姿を現す*/
	case 4: 																																				my_wait+=1/*fps_factor*/;		if (my_wait>=30)	{	sb01[1]->aktframe=0;			/*b->state*/b01state=5; vvv256=1;			my_wait=/*60*/50*4; }		break;
//
	case 5:/*右へ*/ y256+=	vvv256; x256-=2*vvv256; enemy_vvv();	if (x256< t256(5/*10*/) )		{	x256=t256(5/*10*/);}	enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_factor*/;		if (my_wait<=0) 	{			/*b->state*/b01state=6; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	case 6: 		y256-=	vvv256; x256+=2*vvv256; enemy_vvv();																enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_factor*/;		if (my_wait<=0) 	{			/*b->state*/b01state=7; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	case 7:/*左へ*/ y256+=	vvv256; x256+=2*vvv256; enemy_vvv();	if (x256> t256(290/*350*/) )	{	x256=t256(290/*350*/);}	enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_factor*/;		if (my_wait<=0) 	{			/*b->state*/b01state=8; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	case 8: 		y256-=	vvv256; x256-=2*vvv256; enemy_vvv();																enemy_boss01_setpos(x256,y256); 	my_wait-=1/*fps_factor*/;		if (my_wait<=0) 	{			/*b->state*/b01state=4; vvv256=1;	my_wait=/*60*/50*4; 	}		break;
	}
	firewait1 -= /*fps_factor**/(b01level/*b->level+1*/);
	if (firewait1<=0)
	{
		firewait1=45;
		firewait2--;
		if (firewait2==0)
		{
			firewait2=4;
			firewait3=1+difficulty;
			if (FLG_ALL_DOLL==(b01flags&FLG_ALL_CAST))/*アリス以外全員倒した？*/
			{
				firewait3++;	/* 最後が簡単すぎるので少し手強くする */
			}
		}
		else
		{
			enemy_boss01_LR_dole_fire();/*enemy_boss01_fire(0);enemy_boss01_fire(1);*/
		}
		if (firewait3)
		{
			firewait3--;
			enemy_boss01_nway_fire(sb01[4],firewait3<<3);/*enemy_boss01_fire(2);*/
		}
	}
}

void enemy_boss01_add(int lv)
{
	aaa_count=0;
	b01level			= lv+1;
//	vvv256=/*fps_factor**/t256(b01level/*b->level+1*/)/4;
	b01flags=0;
	br1_angle512=deg_360_to_512(0);
	br2_angle512=deg_360_to_512(0);
	sb01[0]=sprite_add_file("boss01_0.png",4,PR_ENEMY); sb01[0]->anim_speed=8;/*"boss01-lo.png"62x49, offset(3-4+4)x(15)*/
	sb01[1]=sprite_add_file("boss01_1.png",8,PR_ENEMY); sb01[1]->anim_speed=0;/*"boss01-mo.png"60x42, offset*/
	sb01[2]=sprite_add_file("boss01_2.png",4,PR_ENEMY); sb01[2]->anim_speed=8;/*"boss01-ro.png"53x42, offset(1-4+4)x(15)*/
	sb01[3]=sprite_add_file("boss01_3.png",2,PR_ENEMY); sb01[3]->anim_speed=8;/*"boss01-lu.png"62x67, offset(9-3)x(3)*/
	sb01[4]=sprite_add_file("boss01_4.png",2,PR_ENEMY); sb01[4]->anim_speed=8;/*"boss01-mu.png"60x57, offset(7-4)x(8)*/ //key
	sb01[5]=sprite_add_file("boss01_3.png",2,PR_ENEMY); sb01[5]->anim_speed=8;/*"boss01-ru.png"53x57, offset(8-3)x(10)*/
//
	/*b->state*/b01state=0;
	BOSS01_DATA *b;
	int i;
	for (i=0;i<6;i++)
	{
		const unsigned short b01_health[6] =
		{
		/*	   if (i==0)	{	b->b.health=*/ 400,//;	}	//[***090114		変更(+50)
		/*else if (i==1)	{	b->b.health=*/1023,//;	}	//アリス本体のHP。もう少し高くてもいいかも。//[***090305	変更
		/*else if (i==2)	{	b->b.health=*/ 400,//;	}	//[***090114		変更(+50)
		/*else if (i==3)	{	b->b.health=*/ 200,//;	}	//denis was 40		//[***090114	変更(+20)
		/*else if (i==4)	{	b->b.health=*/	20,//(RANK_MAX-difficulty)*20;	}	//真ん中の子
		/*else if (i==5)	{	b->b.health=*/ 200 //;	}	//denis was 40		//[***090114	変更(+20)
		};
		sb01[i]->flags		|= (SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
//		sb01[i]->anim_speed=0;
		sb01[i]->aktframe	= 0;
		sb01[i]->type		= SP_EN_BOSS/*SP_EN_BOSS01*/;
		b					= mmalloc(sizeof(BOSS01_DATA));
		sb01[i]->data		= b;
		b->health_flag		= 0;
		// 4==真ん中の子(最高ランクの場合20、以下ランク下がるごとに20ずつ増える)
		b->b.health 		= ((4==i)?((RANK_MAX-difficulty)*20):(b01_health[i]));
		//
		if (i==1)		{	sb01[i]->mover=enemy_boss01_move;
							b->b.score=score(1000)*(difficulty+1);}
		else			{	b->b.score=score( 500)*(difficulty+1);}
	}
	//((PLAYER_DATA *)player->data)->bossmode=B01_BATTLE;
	((PLAYER_DATA *)player->data)->boss=sb01[1];
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss01_hitbyweapon_dummy;	/* ダミーコールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyplayer=callback_enemy_boss01_hitbyplayer;
}
