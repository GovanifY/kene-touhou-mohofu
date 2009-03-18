#include "score.h"

extern HSC_LIST hsc_table[];
extern SDL_Surface *screen;
extern SPRITE *player;
extern int high_score;

extern int weapon_List; /*player.c*/
SDL_Surface *bg2;

//extern int is_graze;				// player1が弾幕に触れたか？
extern SDL_Surface *weapon_p;		// ウェポンゲージ。俺の環境だとloadbmp2で作ると普通に表示されるようになった。
extern SDL_Surface *boss_hp;

enum
{
	R_00_Player_Star_png = 0,
	R_01_Bomb_Star_png,
};
static void player_display( int g_num, int num, int y_suf)		//星の表示
{
	SDL_Surface *star;
	/*const*/ char *img_name[]=
	{
		"Player_Star.png",
		"Bomb_Star.png",
	};
	SDL_Rect recAll,rec;
	star = loadbmp((char *)img_name[g_num]);
	SDL_SetColorKey(star,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	if (num<1)	{	num=0;}

	rec.x = 0;
	rec.y = 0;
	rec.w = (10*num);
	rec.h = 10;

	recAll.x = WIDTH-7-(10*num);
	recAll.y = y_suf;
	recAll.w = 100;
	recAll.h = 11;
	SDL_BlitSurface(star, &rec, screen, &recAll);
}
/* pannel x offset */
#define PPP 380

static void power_status(int weapon, int dx, int dy)		//ウェポンゲージの表示。ketmに差分を上書きすると何故か普通に表示される。
{
	SDL_Rect srec, drec;
	srec.x=0;
	srec.y=0;
	srec.h=13;
	srec.w=(int)((double)weapon / 128 * (double)weapon_p->w);		//[***090123		変更
	drec.w=weapon_p->w;
	drec.h=weapon_p->h;
	drec.x=dx;
	drec.y=dy;
	SDL_BlitSurface(weapon_p, &srec, screen, &drec);
}

#define HPGAUGE 50
static void health_status(int dx, int dy)		//[***090305		変更
{
	char buffer[3];
	SPRITE *s=((PLAYER_DATA *)player->data)->boss;
	sprintf(buffer,"%d", ((ENEMY_DATA *)s->data)->b.health>>10);
	font_print_screen_xy(buffer,FONT03,47,0);
	SDL_Rect srec, drec;
	srec.x=0;
	srec.y=0;
	srec.h=10;
	srec.w=HPGAUGE+((((ENEMY_DATA *)s->data)->b.health  & 0x03FF)>>2);
	drec.w=boss_hp->w;
	drec.h=boss_hp->h;
	drec.x=dx;
	drec.y=dy;
	SDL_BlitSurface(boss_hp, &srec, screen, &drec);
}

void score_display(void)
{
	SDL_Rect bg2r={WIDTH2, 0, 0, 0};	// データウィンドウ用rect->w,h,x,y
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;
	char buffer[64/*100*/];
	SDL_BlitSurface(bg2,NULL,screen,&bg2r);

	//sprintf(buffer,"SHIPS : %d",p->lives);		//	font_print_screen_xy(buffer,FONT07,0,10);
	//sprintf(buffer,"SPEED : %d",p->player_speed); //	font_print_screen_xy(buffer,FONT07,0,20);
	//sprintf(buffer,"STAGE : %d",p->level);		//	font_print_screen_xy(buffer,FONT07,0,30);

	if (high_score < p->score)
	{	high_score = p->score;}
	//sprintf(buffer,"H_SCORE:");					//	font_print_screen_xy(buffer,FONT07,PPP+1*8,5);
	sprintf(buffer,"%09d0", high_score);				font_print_screen_xy(buffer,FONT07,PPP+1*8+3,15);

	//sprintf(buffer,"SCORE  :");					//	font_print_screen_xy(buffer,FONT07,PPP+1*8,30);
	sprintf(buffer,"%09d0", p->score);					font_print_screen_xy(buffer,FONT07,PPP+1*8+3,40);

	//sprintf(buffer,"PLAYER"); 					//	font_print_screen_xy(buffer,FONT07,PPP+1*8,60);
	player_display( R_00_Player_Star_png, (p->lives), 69);

	//sprintf(buffer,"BOMB");						//	font_print_screen_xy(buffer,FONT07,PPP+1*8,85);
	player_display( R_01_Bomb_Star_png, (p->bombs), 94);

	//sprintf(buffer,"POWER  :");					//	font_print_screen_xy(buffer,FONT07,PPP+1*8,110);
	power_status(p->weapon ,PPP+7,124);
	if (p->weapon > (127-1) /*== 128*/)
	{
		sprintf(buffer,"MAX");							font_print_screen_xy(buffer,FONT07,PPP+7*8+3,125);
	}
	else
	{
		#if 0
	//	sprintf(buffer,"%d",(int)((double)p->weapon / 128 * 100 )); 	//[***090123		変更
		#else
		sprintf(buffer,"%d", (((int)(p->weapon) * 200) >>8) );		//[***090214		変更
		#endif
		font_print_screen_xy(buffer,FONT07,PPP+8*8+3,125);
	}

	if (p->extra!=PLX_NONE)
	{
		sprintf(buffer,"EXTRA  :"); 					font_print_screen_xy(buffer,FONT07,PPP+1*8-2,160);
		/*const*/ char *extra_name[4]=
		{
		// 0==PLX_NONE		"NONE",
		/* 1==PLX_HOMING:*/ "MISSILE",
		/* 2==PLX_SHIELD:*/ "WEAPON_UP",
		/* PLX_HLASER:*/	"LASER",
		/* PLX_BOMB:*/		"BOMB",
		//	/*default:*/	"UNKNOWN ???",
		};
		font_print_screen_xy( (char *)extra_name[(p->extra-1)&(4-1)], FONT07,PPP+2*8-4,170);
		sprintf(buffer,"TIME :%3d",(int)(((int)p->extra_time)/10));
		font_print_screen_xy(buffer,FONT07,PPP+3*8-6,180);
	}
	//sprintf(buffer,"GRAZE  :");					//	font_print_screen_xy(buffer,FONT07,PPP+1*8,140);

	sprintf(buffer," %d", p->graze);					font_print_screen_xy(buffer,FONT07,PPP+7*8+3,140);

	if(((PLAYER_DATA *)player->data)->bossmode==1){
		health_status(10, 6);
	}
	/*
	switch (weapon_List) {
	case WP_PLASMA: 			strcat(buffer,"REIFU-1");		break;
	case WP_DOUBLEPLASMA:		strcat(buffer,"REIFU-2");		break;
	case WP_QUADPLASMA: 		strcat(buffer,"REIFU-4");		break;
	case WP_FIREBALL:			strcat(buffer,"YUMEFU-1");		break;
	case WP_DOUBLEFIREBALL: 	strcat(buffer,"YUMEFU-2");		break;
	case WP_QUADFIREBALL:		strcat(buffer,"YUMEFU-4");		break;
	case WP_FIFTHFIREBALL:		strcat(buffer,"YUMEFU-5");		break;
	case WP_KILLRAY:			strcat(buffer,"OFUDA"); 		break;
	default:					strcat(buffer,"UNKNOWN ???");	break;
	}
	font_print_screen_xy(buffer,FONT07,PPP+3*8,120);
	*/
}

//void score_cleanup()
//{
//}
