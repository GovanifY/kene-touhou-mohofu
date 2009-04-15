/*
輝夜
かぐや
*/
#include "enemy.h"


typedef struct _boss03_data
{
	ENEMY_BASE b;
	int state;
	int health_flag;
	int level;
} BOSS03_DATA;

static SPRITE *sb03[6];


/* boss wurde von player ber??rt */
static void callback_enemy_boss03_hitbyplayer(SPRITE *c)
{
}

/* boss wurde von player-weapon ber??rt */
static void callback_enemy_boss03_hitbyweapon_dummy(SPRITE *c, SPRITE *s/*, int angle*/)
{	/* 完全に姿を現すまで攻撃は、すべて無効とする。 */
}
static void callback_enemy_boss03_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/)
{
	int i,j;
	BOSS03_DATA *b;
	WEAPON_BASE *w=(WEAPON_BASE *)s->data;

	for (i=0;i<6;i++)
	{	if (c==sb03[i])
		{	break;}}

	switch (i)
	{
	case 0:
		if (sb03[3]->flags&SP_FLAG_VISIBLE)
		{	i=3;}
		break;
	case 1:
			 if (sb03[4]->flags&SP_FLAG_VISIBLE)		{		i=4;}
		else if (sb03[3]->flags&SP_FLAG_VISIBLE)		{		i=3;}
		else if (sb03[5]->flags&SP_FLAG_VISIBLE)		{		i=5;}
		else if (sb03[0]->flags&SP_FLAG_VISIBLE)		{		i=0;}
		else if (sb03[2]->flags&SP_FLAG_VISIBLE)		{		i=2;}
		break;
	case 2:
		if (sb03[5]->flags&SP_FLAG_VISIBLE)
		{	i=5;}
		break;
	}

	b=(BOSS03_DATA *)sb03[i]->data;
	b->b.health-=w->strength;

	explosion_add(s->x,s->y,0,EXPLOSION_MINI00);
	//parsys_add(NULL,100,0,s->x,s->y,30,0,0,50,PIXELATE,NULL);


	if ((b->b.health<=15)&&(b->health_flag==0))
	{
		b->health_flag=1;
		explosion_add(sb03[i]->x+sb03[i]->w/2,sb03[i]->y+sb03[i]->h/2,0,EXPLOSION_FIRE08);
	}
	if (b->b.health<=0)/*該当者の体力切れなら*/
	{
		explosion_add(sb03[i]->x+sb03[i]->w/2,sb03[i]->y+sb03[i]->h/2,0,EXPLOSION_ZAKO04/*EXPLOSION_FIRE08*/);
		sb03[i]->flags		&= ~SP_FLAG_VISIBLE;
		((PLAYER_DATA *)player->data)->score+=b->b.score;
		bonus_multi_add(sb03[i], SP_BONUS_06_COIN, 7, BONUS_FLAG_COLLECT);
	}

	j=0;
	for (i=0;i<6;i++)
	{
		if (b->health_flag==1)
		{	sb03[i]->aktframe=1;}
		if (sb03[i]->flags&SP_FLAG_VISIBLE)
		{	j++;}
	}
	if (j==0)
	{
		/* boss komplett zerst??t */
		for (i=0;i<6;i++)
		{
			sb03[i]->type=SP_DELETE;
		//	explosion_add(sb03[i]->x+sb03[i]->w/2,sb03[i]->y+sb03[i]->h/2,(rand()&(16-1)/*%20*/),EXPLOSION_FIRE08);
			explosion_add_circle(sb03[i]);
		}
		((PLAYER_DATA *)player->data)->bossmode=B02_DEATH_WAIT;
	}
}


static void enemy_boss03_setpos(int x, int y)
{
	sb03[0]->x=x;
	sb03[0]->y=y;
	sb03[1]->x=sb03[0]->w+sb03[0]->x;
	sb03[1]->y=y;
	sb03[2]->x=sb03[1]->w+sb03[1]->x;
	sb03[2]->y=y;
	sb03[3]->x=x;
	sb03[3]->y=sb03[0]->h+y;
	sb03[4]->x=sb03[3]->w+sb03[3]->x;
	sb03[4]->y=sb03[1]->h+y;
	sb03[5]->x=sb03[4]->w+sb03[4]->x;
	sb03[5]->y=sb03[2]->h+y;
}


/*int where / 0: left, 1: right, 2: bombenhagel */
static void enemy_boss01_nway_fire(void)
{
//	switch (where)
	{
//	case 2:
		{static int hari=0;
			hari--;
			hari &= 32;
			playChunk(14);
			int angle512;
			for (angle512=deg_360_to_512(0);angle512</*=*/deg_360_to_512(360/*180*/);
			//	angle512+=/*22*/21+hari-difficulty/*deg_360_to_512(20)*/		/* 23本(360) ← 8本(180) */
				angle512+=(/*22*/18+hari-difficulty-difficulty/*deg_360_to_512(20)*/)		/* 最低 == 10 == 48本(360)  */
				)
			{
				bullet_create_haridan180(sb03[4], angle512, (int)(/*6*/ (0x100)+(rand()&0x2ff)/*t256(2.5)*/), /*offsx*/0/*(src->w/2)*/, /*offsy*/0/*(src->h/2)*/ );
			}
		}
//		break;
	}
}

static void enemy_boss03_LR_dole_fire(void)
{
	{
//	case 0:
		if (sb03[3]->flags&SP_FLAG_VISIBLE)
		{	enemy_bullet_create(sb03[3],5);}
//		break;
//	case 1:
		if (sb03[5]->flags&SP_FLAG_VISIBLE)
		{	enemy_bullet_create(sb03[5],5);}
//		break;
	}
}
//[***090114		いろいろと追加します。
//3面と4面の差別化。
//4面には新しく追加した弾軌道を組み込む。
static void enemy_boss03_move(SPRITE *c)
{
	BOSS03_DATA *b=(BOSS03_DATA *)c->data;
	static /*double*/int xxx;
	static /*double*/int yyy;
	static /*double*/int www;
	static int firewait3;
	static int bomb_n;
	int i;
	switch (b->state)
	{
	case 0:
		xxx		=GAME_WIDTH/2-(sb03[0]->w+sb03[1]->w+sb03[2]->w)/2; 	//ウィンドウ幅の変更
		yyy		=-100;
		enemy_boss03_setpos(xxx,yyy);
		b->state	=1;
	//	firewait1	=45;
	//	firewait2	=4;
		firewait3	=0;
		bomb_n		=0;
		break;
	case 1:
		yyy+=1/*fps_factor*(b->level+1)*/;
		enemy_boss03_setpos(xxx,yyy);
		if (yyy>=0)
		{
			b->state=2;
			www=0;
			((PLAYER_DATA *)player->data)->bossmode=B05_BEFORE_LOAD;
		}
		break;
	case 2:
		if (((PLAYER_DATA *)player->data)->bossmode==B01_BATTLE)
		{
			www+=1/*fps_factor*/;
			if (www>=40)
			{
				b->state=3;
			}
		}
		break;
	case 3:
		yyy+=1/*fps_factor*(b->level+1)*/;
		enemy_boss03_setpos(xxx,yyy);
		if (yyy>=30)
		{
			b->state=4;
			www=0;
			/* プレイヤー弾受け付け、コールバックを登録 */
			((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss03_hitbyweapon;
		}
		break;
/*完全に姿を現す*/
	case 4:
		www+=1/*fps_factor*/;
		if (www>=30)
		{
			b->state=5;
			www=0;
		}
		break;
	case 5:
		yyy+=1/*fps_factor*(b->level+1)*/;
		xxx-=2*1/*fps_factor*(b->level+1)*/;
		www+=1/*fps_factor*/;
		enemy_boss03_setpos(xxx,yyy);
		if (www>=60)
		{	b->state=6;}
		break;
	case 6:
		yyy-=1/*fps_factor*(b->level+1)*/;
		xxx+=2*1/*fps_factor*(b->level+1)*/;
		www-=1/*fps_factor*/;
		enemy_boss03_setpos(xxx,yyy);
		if (www<=0)
		{	b->state=7;}
		break;
	case 7:
		yyy+=1/*fps_factor*(b->level+1)*/;
		xxx+=2*1/*fps_factor*(b->level+1)*/;
		www+=1/*fps_factor*/;
		enemy_boss03_setpos(xxx,yyy);
		if (www>=60)
		{	b->state=8;}
		break;
	case 8:
		yyy-=1/*fps_factor*(b->level+1)*/;
		xxx-=2*1/*fps_factor*(b->level+1)*/;
		www-=1/*fps_factor*/;
		enemy_boss03_setpos(xxx,yyy);
		if (www<=0)
		{	b->state=4;}
		break;
	}
	if (/*((BOSS03_DATA *)sb03[1]->data)*/b->state > 2)
	{
	static /*double*/int firewait1=45;
		firewait1-=1/*fps_factor*(b->level+1)*/;
		if (firewait1<=0)
		{
			firewait1=45;
			static unsigned int firewait2=0;
			firewait2--;
			firewait2 &= (4-1);
			if (0==firewait2)
			{
				enemy_boss01_nway_fire();/*enemy_boss03_fire(2);*/
			}
			else
			{
				enemy_boss03_LR_dole_fire();/*enemy_boss03_fire(0);enemy_boss03_fire(1);*/
			}
		}
	}
	/* 4面の場合跳ねる珠 */
	if (0==b->level)
	{
		if (b->b.health<=1000+1024)
		{
			if (firewait3 < 0)
			{
				if (bomb_n<32)
				{	bomb_n++;}
				for (i=0;i<=(int)((double)bomb_n/3.0);i++)
				{
					enemy_pong_bullet_create(c, 5.0,
						(/*(M_PI*2)*/512*3*i/(double)(bomb_n+1))
							/*+rad2deg512(1)*/, /*←多分括弧書かなかったミス*/
						t256(0.07), 2);		//[***090116		微調整
				}
				if (b->b.health>200+1024)
				{	firewait3=b->b.health/4;}
				else
				{	firewait3=50;}
			}
			else
			{	firewait3--;}
		}
	}
}


void enemy_boss03_add(int lv)
{
	if (lv==0)
	{
		sb03[0]=sprite_add_file("boss05-lo.png",2,PR_ENEMY);
		sb03[1]=sprite_add_file("boss05-mo.png",2,PR_ENEMY);
		sb03[2]=sprite_add_file("boss05-ro.png",2,PR_ENEMY);
		sb03[3]=sprite_add_file("boss05-lu.png",2,PR_ENEMY);
		sb03[4]=sprite_add_file("boss05-mu.png",2,PR_ENEMY);
		sb03[5]=sprite_add_file("boss05-ru.png",2,PR_ENEMY);
	}
	else
	{
		sb03[0]=sprite_add_file("boss03-lo.png",2,PR_ENEMY);
		sb03[1]=sprite_add_file("boss03-mo.png",2,PR_ENEMY);
		sb03[2]=sprite_add_file("boss03-ro.png",2,PR_ENEMY);
		sb03[3]=sprite_add_file("boss03-lu.png",2,PR_ENEMY);
		sb03[4]=sprite_add_file("boss03-mu.png",2,PR_ENEMY);
		sb03[5]=sprite_add_file("boss03-ru.png",2,PR_ENEMY);
	}

	int i;
	for (i=0;i<6;i++)
	{
		BOSS03_DATA *b;
		sb03[i]->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
		sb03[i]->anim_speed=0;
		sb03[i]->aktframe=0;
		sb03[i]->type=SP_EN_BOSS/*SP_EN_BOSS03*/;
		b=mmalloc(sizeof(BOSS03_DATA));
		sb03[i]->data=b;
		if (lv==0)		//[***090114		追加
		{
			if (i==1)/*輝夜本人*/
			{
				b->b.health= (difficulty<<10/*[x1024]*/)+2047+1024;
				b->b.score=score(3000)+score(2000)*difficulty;
			}
			else/*小物*/
			{
				b->b.health= (difficulty<<8/*[x256]*/)+ 200+1024;
				b->b.score=score(600)*(difficulty+1);
			}
		}
		else/*全部一緒*/
		{
			b->b.health = (difficulty<<6/*[x64]*/)+40+1024;  //denis was 40
			b->b.score=score(500)*(difficulty+1);
		}
		b->health_flag=0;
		b->level=lv;
		if (i==1)
		{
			b->state=0;
			sb03[i]->mover=enemy_boss03_move;
		}
	}
	((PLAYER_DATA *)player->data)->boss=sb03[1];
	((PLAYER_DATA *)player->data)->bossmode=B01_BATTLE;
	/* コールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyweapon=callback_enemy_boss03_hitbyweapon_dummy;	/* ダミーコールバック登録 */
	((PLAYER_DATA *)player->data)->callback_boss_hitbyplayer=callback_enemy_boss03_hitbyplayer;
}
