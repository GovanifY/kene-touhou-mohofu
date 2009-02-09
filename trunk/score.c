#include "score.h"

extern HSC_LIST hsc_table[];
extern SDL_Surface *screen;
extern SPRITE *player;
extern int high_score;
SDL_Surface *star;
extern weapon_List;
SDL_Surface *bg2;
SDL_Rect bg2r={WIDTH2, 0, 0, 0};	//データウィンドウ用rect->w,h,x,y
extern int is_graze;	//player1が弾幕に触れたか？
extern SDL_Surface *weapon_p;	//ウェポンゲージ。俺の環境だとloadbmp2で作ると普通に表示されるようになった。

/*
typedef struct {
	int lives;
	int bombs;
	int graze;
	int score;
	int level;
	int bossmode;
	int state;
	int explode;
	double save_delay;
	double anim_delay;
	int weapon;
	int player_speed;
	int extra;
	double extra_wait;
	double extra_time;
	double weapon_wait;
} PLAYER_DATA;
*/

void Player_Disp(char *img_name, int num, int x_suf, int y_suf)		//星の表示
{
	SDL_Rect recAll,rec;
	star = loadbmp(img_name);
	SDL_SetColorKey(star,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	if(num<=0)
		num=1;
	recAll.x = x_suf;
	recAll.y = y_suf;
	recAll.w = 100;
	recAll.h = 11;
	
	rec.x = 0;
	rec.y = 0;
	rec.w = 10*(num-1);
	rec.h = 10;
	
	SDL_BlitSurface(star, &rec, screen, &recAll);
}

void Power_Statas(int weapon, int dx, int dy)		//ウェポンゲージの表示。ketmに差分を上書きすると何故か普通に表示される。
{
	SDL_Rect srec, drec;
	srec.x=0;
	srec.y=0;
	srec.h=13;
	srec.w=(int)((double)weapon / 128 * (double)weapon_p->w);		//***090123		変更
	drec.w=weapon_p->w;
	drec.h=weapon_p->h;
	drec.x=dx;
	drec.y=dy;
	SDL_BlitSurface(weapon_p, &srec, screen, &drec);
}

void score_display()		//ここの説明は省略
{
		PLAYER_DATA *p=(PLAYER_DATA *)player->data;
		char buffer[100];
	
		SDL_BlitSurface(bg2,NULL,screen,&bg2r);
	
		//sprintf(buffer,"SHIPS : %d",p->lives);
		//font_print_screen_xy(buffer,FONT07,0,10);

		//sprintf(buffer,"SPEED : %d",p->player_speed);
		//font_print_screen_xy(buffer,FONT07,0,20);
	
		//sprintf(buffer,"STAGE : %d",p->level);
		//font_print_screen_xy(buffer,FONT07,0,30);
	
			if(high_score < p->score)
				high_score = p->score;
			//sprintf(buffer,"H_SCORE:");
			//font_print_screen_xy(buffer,FONT07,387,5);
			sprintf(buffer," %08d", high_score);
			font_print_screen_xy(buffer,FONT07,397,15);
		
			//sprintf(buffer,"SCORE  :");
			//font_print_screen_xy(buffer,FONT07,387,30);
			sprintf(buffer," %08d", p->score);
			font_print_screen_xy(buffer,FONT07,397,40);
			
			//sprintf(buffer,"PLAYER");
			//font_print_screen_xy(buffer,FONT07,387,60);
			Player_Disp("Player_Star.png", p->lives,382,69);
			
			//sprintf(buffer,"BOMB");
			//font_print_screen_xy(buffer,FONT07,387,85);
			Player_Disp("Bomb_Star.png", p->bombs,382,94);
			
			//sprintf(buffer,"POWER  :");
			//font_print_screen_xy(buffer,FONT07,387,110);
			Power_Statas(p->weapon ,387,124);
			if(p->weapon == 128)
			{
				sprintf(buffer,"MAX");
				font_print_screen_xy(buffer,FONT07,388,125);
			}
			else
			{
				sprintf(buffer,"%d",(int)((double)p->weapon / 128 * 100));		//***090123		変更
				font_print_screen_xy(buffer,FONT07,388,125);
			}
			
			if(p->extra!=PLX_NONE) {
				sprintf(buffer,"EXTRA  :");
				font_print_screen_xy(buffer,FONT07,387,160);
				switch(p->extra) {
				case PLX_HOMING:
					sprintf(buffer," MISSILE");
					break;
				case PLX_SHIELD:
					sprintf(buffer," WEAPON_UP");
					break;
				case PLX_HLASER:
					sprintf(buffer," LASER");
					break;
				case PLX_BOMB:
					sprintf(buffer," BOMB");
					break;
				default:
					sprintf(buffer," UNKNOWN ???");
					break;
				}
			
				font_print_screen_xy(buffer,FONT07,387,170);
	
				sprintf(buffer,"  TIME :%3d",(int)((double)p->extra_time/10));
				font_print_screen_xy(buffer,FONT07,387,180);
			}
			//sprintf(buffer,"GRAZE  :");
			
			//font_print_screen_xy(buffer,FONT07,387,140);
			
			sprintf(buffer," %d", p->graze);
			font_print_screen_xy(buffer,FONT07,440,140);
			
		/*
		switch(weapon_List) {
			case WP_PLASMA:
				strcat(buffer,"REIFU-1");
				break;
			case WP_DOUBLEPLASMA:
				strcat(buffer,"REIFU-2");
				break;
			case WP_QUADPLASMA:
				strcat(buffer,"REIFU-4");
				break;
			case WP_FIREBALL:
				strcat(buffer,"YUMEFU-1");
				break;
			case WP_DOUBLEFIREBALL:
				strcat(buffer,"YUMEFU-2");
				break;
			case WP_QUADFIREBALL:
				strcat(buffer,"YUMEFU-4");
				break;
			case WP_FIFTHFIREBALL:
				strcat(buffer,"YUMEFU-5");
				break;
			case WP_KILLRAY:
				strcat(buffer,"OFUDA");
				break;
			default:
				strcat(buffer,"UNKNOWN ???");
				break;
		}
		font_print_screen_xy(buffer,FONT07,397,120);
		*/
}

void score_cleanup()
{
}
