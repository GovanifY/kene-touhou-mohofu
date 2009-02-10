#include "player.h"

#define rand_percent(aaa) ((unsigned char)(rand())<=(unsigned char)( (aaa*256)/100 ) ) /* aa%の確率で */
extern SDL_Surface *screen;
extern int keyboard[];
extern KEYCONFIG keyconfig;
extern double fps_factor;
extern SPRITE *sprite;
extern int difficulty;
extern int select_player;
int bomb_wait;			//次のボムを出せるまでの時間
int weapon_List;		//どの武器を装備しているか
int weapon_chain;		//どれだけ連続でwepon_upを取ったか
SPRITE *player;
int graze_check[5]={-1,-1,-1,-1,-1};	//グレイズ用、弾id格納場所。
										//1つにしなかったのは複数の弾を同時にグレイズしたときのため。
int is_graze;	//player1が弾幕に触れたか？
SDL_Surface *weapon_p;		//ウェポンゲージ

SPRITE *c1, *c2;	//***090128	オプション用

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
	int player_speed_mini;		//***090205		追加
	int player_speed_max;		//***090205		追加	プレイヤー上限値
	int extra;
	int bonus;		//***090116		追加
	int hit_bomb_wait;		//***090125		追加
	int option;
	double extra_wait;
	double extra_time;
	double weapon_wait;
	SPRITE *core;
	SPRITE *enemy;		//***090125		追加:playerに当たった物
} PLAYER_DATA;

enum _priority {
	PR_BACK0=0,
	PR_GROUNDER,
	PR_BACK1,
	PR_BACK2,
	PR_ENEMY,
	PR_ENEMY_WEAPON,
	PR_TARGET,
	PR_BONUS,
	PR_PLAYER,
	PR_PLAYER2,
	PR_TEXT,
	PR_FRONT0,
	PR_FRONT1,
	PR_FRONT2
};
*/


void option_check(PLAYER_DATA *d){
	if(d->weapon>35){
		if(d->option==0){
			d->option=1;
			switch(select_player){
				case REIMU:
					re_add_option(player);
					break;
				case MARISA:
					ma_add_option(player);
					break;
			}
		}
	}
	else
		d->option=0;
}

void weapon_check(int w)		//weaponの段階から今の装備を決める
{								//***090123		大幅な変更。最大128へ。
	if(w<=5)
		weapon_List=WP_SINGLE;
	else if(w<=10)
		weapon_List=WP_DOUBLE;
	else if(w<=60)
		weapon_List=WP_TRIPLE;
	else if(w<=85)
		weapon_List=WP_QUAD;
	else if(w<=128)
		weapon_List=WP_FIFTH;
	else
		weapon_List=WP_LAST;
}

void player_init()
{

	PLAYER_DATA *data;
	switch(select_player){
		case REIMU:
			player=sprite_add_file("ship-med.png",11,PR_PLAYER);
			break;
		case MARISA:
			player=sprite_add_file("ship-med-ma.png",11,PR_PLAYER);
			break;
	}
	player->type=SP_PLAYER;
	player->flags|=(SP_FLAG_VISIBLE|SP_FLAG_COLCHECK);
	player->mover=player_move;
	player->x=WIDTH2/2;
	player->y=screen->h-player->h;
	player->aktframe=5;
	data=mmalloc(sizeof(PLAYER_DATA));
	player->data=data;
	switch(select_player){
		case REIMU:
			data->player_speed_mini=3;
			data->player_speed=3;
			data->player_speed_max=6;
			break;
		case MARISA:
			data->player_speed_mini=4;
			data->player_speed=4;
			data->player_speed_max=8;
			break;
	}
	data->level=1;
	data->bossmode=0;
	data->state=PL_SAVE;
	data->explode=0;
	data->save_delay=120;
	data->anim_delay=0;
	data->weapon=0;
	data->weapon_wait=0;
	data->extra=PLX_NONE;
	data->extra_wait=0;
	data->extra_time=0;

	data->graze=0;
	data->lives=3;
	data->bombs=3;
	data->score=0;
	data->bonus=0;
	data->hit_bomb_wait=0;		//***090125		追加
	data->option=0;

	bomb_wait = 0;
	weapon_List=0;
	weapon_chain=0;
	weapon_p=loadbmp2("weapon_p.png");		//俺の環境だとloadbmpで読み込むと表示されなくなる。
	data->core=player_add_core(player);		//○の追加
}

void player_move(SPRITE *s1)
{

	PLAYER_DATA *d=(PLAYER_DATA *)s1->data;

	switch(d->state) {

		case PL_NORMAL:

			player_keycontrol(s1);
			player_colcheck(s1,SP_SHOW_ENEMYS|SP_SHOW_ENEMY_WEAPONS|SP_SHOW_BONUS);
			player_colcheck2(s1,SP_SHOW_ENEMYS|SP_SHOW_ENEMY_WEAPONS|SP_SHOW_BONUS);
			break;

		//***090125		追加
		case PL_HIT_BOMB:
			if(d->hit_bomb_wait<6){
				if((keyboard[KEY_BOMB])&&(d->bombs>0)){
					if(bomb_wait==0)
					{
						switch(select_player){
							case REIMU:
								playChunk(7);
								d->save_delay=200;					//無敵時間。たまにシールドをすり抜ける者が現れるので
								d->state=PL_SAVE;
								d->extra=PLX_BOMB;
								d->extra_time=200;
								d->hit_bomb_wait=0;
								player_add_shield(player);		//シールドの追加
								bomb_wait = 200;
								break;
							case MARISA:
								playChunk(7);
								d->save_delay=200;					//無敵時間。つーかさすがにボム時間中無敵じゃないのはきつい
								d->state=PL_SAVE;
								d->hit_bomb_wait=0;
								player_add_levarie(player);
								bomb_wait = 100;
								//d->save_delay=5;
								d->state=PL_SAVE;
								break;
						}
						d->bombs--;
					}
				}
				else
					d->hit_bomb_wait++;
			}
			else
			{
				explosion_add(d->enemy->x+5,d->enemy->y+5,0,rand()%3+1);
				d->explode=0;
				d->enemy->type=-1;
				s1->flags&=~SP_FLAG_VISIBLE;
				d->core->flags&=~SP_FLAG_VISIBLE;
				d->lives--;
				d->bonus=0;
				d->hit_bomb_wait=0;
				d->state=PL_EXPLODE;
			}
			break;

		case PL_EXPLODE:
			//ここでアイテムを吐き出すようにすればそれっぽくなるかも
			if(d->lives){
				bonus_multi_add(s1->x, s1->y,SP_BONUS_FIREPOWER,5,0);//というわけで実装
				if(d->weapon>30)		//***090124ちょっと追加
					bonus_multi_add(s1->x, s1->y,SP_BONUS_FIREPOWER_G,1,0);
			}
			if(!d->explode) {
				//playChunk(4);		//***090127	場所を移動する。
				explosion_add(s1->x+5,s1->y+5,0,rand()%3+1);	
				explosion_add(s1->x+5,s1->y+20,0,rand()%3+1);	
				explosion_add(s1->x+20,s1->y+5,0,rand()%3+1);	
				explosion_add(s1->x+20,s1->y+20,0,rand()%3+1);	
				d->save_delay=150;					//無敵時間
				d->state=PL_RESET;					//無敵状態？
				s1->flags|=SP_FLAG_VISIBLE;			//可視フラグのOFF(不可視)
				d->core->flags|=SP_FLAG_VISIBLE;	//○消し
				d->weapon-=(difficulty*4)+12;			//武器レベルの低下
				d->bombs = 3;						//ボム所持数の初期化
				if ( d->weapon<0)
					d->weapon=0; //denis - reset weapons
					
				if(d->player_speed>d->player_speed_mini) 				//player_speed_mini以下にならないように
					d->player_speed--;
				weapon_check(d->weapon);			//リストを見て装備の変更
				weapon_chain=0;						//weapon_chainの初期化
				option_check(d);
				s1->x=WIDTH2/2-s1->w/2;				//プレイヤー位置の初期化
				s1->y=272;
			}
			break;

		case PL_RESET:
			if(d->save_delay>120)	//ちょっとした待ち時間
				d->save_delay-=fps_factor;
			else
			{
				if(s1->y<240)	//ちょっと前に出てくる
				{
					d->state=PL_SAVE;
				}
				else
				{
					s1->y-=fps_factor;
					if((int)(s1->y/10)%2==0){		//点滅するかなと思って入れてみたけどしなかったよ
						s1->alpha=80;
						d->core->alpha=80;
					}else{
						s1->alpha=255;
						d->core->alpha=255;
					}
				}
			}

		case PL_SAVE:

			player_keycontrol(s1);
			player_colcheck(s1,SP_SHOW_BONUS);


			if(d->save_delay>0) {
				d->save_delay-=fps_factor;
				s1->alpha=80;
				if(d->lives==0)			//ゲームオーバー時の○の表示処理
					d->core->alpha=0;
				else
					d->core->alpha=80;
			} else {
				d->state=PL_NORMAL;
				s1->alpha=255;
				d->core->alpha=255;
			}
			break;	
	}
		
}

void player_keycontrol(SPRITE *s)
{
	int direction=0;
	PLAYER_DATA *d=(PLAYER_DATA *)s->data;

	//if player is invisible (state gameover) allow no keycontrol
	if((!(s->flags&SP_FLAG_VISIBLE))||(d->state==PL_RESET))
		return;
	/*
	if(keyboard[KEY_LEFT]) {
		direction=-1;
		if(s->x>0) {
			if(keyboard[KEY_SLOW])
				s->x-=(d->player_speed/2)*fps_factor;
			else
				s->x-=d->player_speed*fps_factor;
		}
	}

	if(keyboard[KEY_RIGHT]) {
		direction=1;
		if(s->x<WIDTH2-s->w) {		//プレイヤーのx座標 < スクリーンの横幅-プレイヤーの横幅
			if(keyboard[KEY_SLOW])
				s->x+=(d->player_speed/2)*fps_factor;
			else
				s->x+=d->player_speed*fps_factor;
		}
	}

	if((keyboard[KEY_UP]) && (s->y>0)) {
		if(keyboard[KEY_SLOW])
			s->y-=(d->player_speed/2)*fps_factor;
		else
			s->y-=d->player_speed*fps_factor;
		if(d->weapon==128)		//***090126		変更
			if(s->y<50)
				d->bonus=0x01;
		//parsys_add(NULL,50,0,s->x+s->w/2,s->y+s->h,10,90,10,100,PIXELATE,NULL);
	}

	if((keyboard[KEY_DOWN]) && (s->y<screen->h-s->h)) {
		if(keyboard[KEY_SLOW])
			s->y+=(d->player_speed/2)*fps_factor;
		else
			s->y+=d->player_speed*fps_factor;
		if(s->y>50)
			d->bonus&=~0x01;
	}
	*/
		int my_pad;
		my_pad = 0;
		if(keyboard[KEY_LEFT]){
			my_pad |= PSP_CTRL_LEFT;
			direction=-1; 
		}
		else if(keyboard[KEY_RIGHT]){
			my_pad |= PSP_CTRL_RIGHT;
			direction=1;
		}
		if(keyboard[KEY_UP]){
			my_pad |= PSP_CTRL_UP;
		}
		else if(keyboard[KEY_DOWN]) {
			my_pad |= PSP_CTRL_DOWN;
		}

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
		
		const unsigned int my_slow =(keyboard[KEY_SLOW])?(1):(0);		//値をシフトして半分にする？
		s->x += ((d->player_speed*(shipMv[((my_pad&0xf0)>>4)][0] >>my_slow))>>8) *fps_factor;
		s->y += ((d->player_speed*(shipMv[((my_pad&0xf0)>>4)][1] >>my_slow))>>8) *fps_factor;
		// はみだしたら修正。 
		if(s->x < 0){
			s->x = 0;
		} 
		if(s->x > WIDTH2-s->w){
			s->x = WIDTH2-s->w;
		} 
		if(s->y < 0){
			s->y = 0;
		} 
		if(s->y > HEIGHT-s->h){
			s->y = HEIGHT-s->h;
		} 
		// ??? (下記は、よくわかんないけど、だいたい互換のはず？) 
		if(my_pad & PSP_CTRL_UP){ /*&& (s->y>0)*/
			if(d->weapon==128){ //***090123 変更 
				if(s->y<50){
					d->bonus=0x01;
				}
			} 
		//parsys_add(NULL,50,0,s->x+s->w/2,s->y+s->h,10,90,10,100,PIXELATE,NULL); 
		}
		else if(my_pad & PSP_CTRL_DOWN) /*&& (s->y<screen->h*-s->h)*/{ 
			if(d->bonus){
				d->bonus&=~0x01;
			} 
		}
		
	if(d->weapon_wait>0)
		d->weapon_wait-=fps_factor;

	if(d->extra_wait>0)
		d->extra_wait-=fps_factor;

	if(d->extra_time>0)
		d->extra_time-=fps_factor;
	else
		d->extra=PLX_NONE;
	if(bomb_wait>0){					//ボムウェイト処理
		bomb_wait-=fps_factor;
		if(bomb_wait<190)
			d->bonus&=~0x02;
	}
	
	if((keyboard[KEY_BOMB])&&(d->bombs>0))
	{
		if(bomb_wait==0)
		{
			switch(select_player){
				case REIMU:
					playChunk(7);
					d->save_delay=200;					//無敵時間。たまにシールドをすり抜ける者が現れるので
					d->state=PL_SAVE;
					d->extra=PLX_BOMB;
					d->extra_time=200;
					player_add_shield(player);		//シールドの追加
					bomb_wait = 200;
					break;
				case MARISA:
					d->save_delay=200;					//無敵時間。たまにシールドをすり抜ける者が現れるので
					d->state=PL_SAVE;
					playChunk(7);
					player_add_levarie(player);
					bomb_wait = 100;
					break;
			}
			d->bombs--;
		}
	}

	if(keyboard[KEY_SHOT]) {
		if(d->weapon_wait<=0) {
			playChunk(0);
			switch(select_player){
				case REIMU:
					switch(weapon_List) {
						case WP_SINGLE:
							player_add_fuda(s);
							d->weapon_wait=5;
							break;
						case WP_DOUBLE:
							player_add_doublefuda(s);
							d->weapon_wait=5;
							break;
						case WP_TRIPLE:
							player_add_triplefuda(s);
							d->weapon_wait=5;
							break;
						case WP_QUAD:
							player_add_quadfuda(s);
							d->weapon_wait=5;
							break;
						case WP_FIFTH:
							player_add_fifthfuda(s);
							d->weapon_wait=5;
							break;
						case WP_LAST:
							player_add_killray(s);
							d->weapon_wait=5;
							break;
					}
					break;
				
				case MARISA:
					switch(weapon_List) {
						case WP_SINGLE:
							player_add_star(s);
							d->weapon_wait=6;
							break;
						case WP_DOUBLE:
							player_add_doublestar(s);
							d->weapon_wait=6;
							break;
						case WP_TRIPLE:
							player_add_doublestar(s);
							d->weapon_wait=5;
							break;
						case WP_QUAD:
							player_add_triplestar(s);
							d->weapon_wait=5;
							break;
						case WP_FIFTH:
							player_add_quadstar(s);
							d->weapon_wait=5;
							break;
						case WP_LAST:
							player_add_killray(s);
							d->weapon_wait=5;
							break;
					}
					break;
			}
		}
		if(d->extra_wait<=0) {
			switch(d->extra) {
				case PLX_HOMING:
					player_add_homing(s);
					d->extra_wait=60;
					break;
				case PLX_HLASER:
					player_add_hlaser(s);
					d->extra_wait=60;
					break;
			}
		}
	}

	if(d->anim_delay>0) {
		d->anim_delay-=fps_factor;
	} else {
		d->anim_delay=2;
		switch(direction) {
			case -1:
				if(s->aktframe>0) s->aktframe--;
				break;
			case 1:
				if(s->aktframe<10) s->aktframe++;
				break;
			default:
				if(s->aktframe>5) s->aktframe--;
				if(s->aktframe<5) s->aktframe++;
				break;
		}
	}
}

void player_colcheck(SPRITE *s, int mask)
{
	is_graze=0;
	/*
		s		プレイヤー
		mask	調べる対象？
				敵か敵の弾かアイテムか
	*/
	SPRITE *c;	//対象
	//SDL_Surface *spimg;
	PLAYER_DATA *d=(PLAYER_DATA *)s->data;

	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	if((c=sprite_colcheck(s,mask))!=NULL) {
		
		switch(c->type) {
			case SP_BONUS_FIREPOWER:
				playChunk(5);
				
				if(d->weapon<128) {		//maxでないとき
					d->weapon++;
					bonus_info_add(c->x,c->y,"weapon.png");
					weapon_check(d->weapon);
				}
				else {					//maxの時
					char buffer[8];
					weapon_chain++;
					d->score+=10*weapon_chain;
					sprintf(buffer,"%d",10*weapon_chain);		//ウェポンボーナス得点の表示
					bonus_info_text(c->x,c->y,buffer,FONT07);
				}
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				c->type=-1;
				option_check(d);
				break;

			case SP_BONUS_FIREPOWER_G:		//***090123		追加
				playChunk(5);
				if(d->weapon<120) {
					d->weapon+=8;
					bonus_info_add(c->x,c->y,"weapon.png");
					weapon_check(d->weapon);
				}
				else if(d->weapon==128){
					char buffer[8];
					weapon_chain+=8;
					sprintf(buffer,"%d",10*weapon_chain);		//ウェポンボーナス得点の表示
					bonus_info_text(c->x,c->y,buffer,FONT07);
				}
				else{
					d->weapon=128;
					weapon_check(d->weapon);
				}
				c->type=-1;
				option_check(d);
				break;

			case SP_BONUS_BOMB:
				playChunk(5);
				//player_add_bomb(c);		//*****本来はコメントアウトしないよ
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				if(d->bombs<9) {
					d->bombs++;
					bonus_info_add(c->x,c->y,"firebomb.png");
				}
				else {
					d->score+=100;
					bonus_info_add(c->x,c->y,"plus100.png");
				}
				c->type=-1;
				break;

			case SP_BONUS_COIN:
				playChunk(5);
				d->score+=100;		//***090123		変更
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				bonus_info_add(c->x,c->y,"plus100.png");		//***090123		変更
				c->type=-1;
				break;

			case SP_BONUS_ADDSPEED:
				playChunk(5);
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				if(d->player_speed<d->player_speed_max) {
					d->player_speed++;
					bonus_info_add(c->x,c->y,"speed.png");
				} else {
					d->score+=100;
					bonus_info_add(c->x,c->y,"plus100.png");
				}
				c->type=-1;
				break;

			case SP_BONUS_EXTRA:
				playChunk(5);
				if(bomb_wait == 0)	//ボム中はこれを取っても1000ポイント入るだけだよ
				{
					sprite_remove_all_type(SP_PL_SHIELD);
					d->extra=(rand()%(PLX_LAST-2))+1;
					switch(d->extra) {
						case PLX_HOMING:
							d->extra_time=800;
							bonus_info_text(c->x,c->y,"homing missiles",FONT04);
							bonus_info_add(c->x,c->y,"extra.png");
							break;
						case PLX_SHIELD:		//ウェポンアイテム(強)
							d->extra_time=30;
							if(d->weapon<112) {		//***090123		変更
								d->weapon+=16;
								bonus_info_add(c->x,c->y,"weapon.png");
								weapon_check(d->weapon);
							}
							else if(d->weapon==128){		//***090123		変更
								d->score+=1000;
								weapon_chain+=16;
								bonus_info_add(c->x,c->y,"plus1000.png");
								weapon_check(d->weapon);
							}
							else{
								d->weapon=128;		//***090123		変更
								weapon_check(d->weapon);
							}
							option_check(d);
							break;
						case PLX_HLASER:
							d->extra_time=1000;
							bonus_info_text(c->x,c->y,"homing laser",FONT04);
							bonus_info_add(c->x,c->y,"extra.png");
							break;
					}
					//spimg=sprite_getcurrimg(c);
					//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
					//SDL_FreeSurface(spimg);
					c->type=-1;
					break;
				}
				else
				{
					d->score+=1000;
					bonus_info_add(c->x,c->y,"plus1000.png");
					c->type=-1;
					break;
				}
					

			case SP_BONUS_HEALTH:
				playChunk(6);
				//spimg=sprite_getcurrimg(c);
				//parsys_add(spimg, c->w,1, c->x,c->y, 5, 0, 0, 80, LINESPLIT, NULL);
				//SDL_FreeSurface(spimg);
				if(d->lives<9) {
					d->lives++;
					bonus_info_add(c->x,c->y,"energy.png");
				} else {
					d->score+=1000;
					bonus_info_add(c->x,c->y,"plus1000.png");
				}
				c->type=-1;
				break;

			case SP_EN_BOSS01:
				enemy_boss01_hitbyplayer(c);
				break;
			case SP_EN_BOSS02:
				enemy_boss02_hitbyplayer(c);
				break;
			case SP_EN_BOSS03:
				enemy_boss03_hitbyplayer(c);
				break;
			case SP_EN_BOSS04:
				enemy_boss04_hitbyplayer(c);
				break;

			case SP_EN_BULLET:
				is_graze=1;
				int i;
				for(i=0;i<5;i++)
				{
					if(graze_check[i]==((BULLET_DATA *)c->data)->id)	//同じ場合
						break;											//forを抜ける
				}
				if(i==5)	//同じIDの弾が無かった時
				{
					d->graze++;
					d->score+=100+(difficulty*200);
					graze_check[(d->graze)%5]=((BULLET_DATA *)c->data)->id;
					playChunk(9);
				}
				break;
				
			case SP_EN_LASER:
				is_graze=1;
				int j;
				for(j=0;j<5;j++)
				{
					if(graze_check[j]==((LASER_DATA *)c->data)->id)	//同じ場合
						break;											//forを抜ける
				}
				if(j==5)
				{
					d->graze++;
					d->score+=100+(difficulty*200);
					graze_check[(d->graze)%5]=((LASER_DATA *)c->data)->id;
					playChunk(9);
				}
				break;
			
			case SP_EN_BIGBULLET:
				is_graze=1;
				int l;
				for(l=0;l<5;l++)
				{
					if(graze_check[l]==((BIGBULLET_DATA *)c->data)->id)	//同じ場合
						break;											//forを抜ける
				}
				if(l==5)
				{
					d->graze++;
					d->score+=100+(difficulty*200);
					graze_check[(d->graze)%5]=((BIGBULLET_DATA *)c->data)->id;
					playChunk(9);
				}
				break;
				
			case SP_EN_BIGBULLET_DUMMY:		//2つ目のスプライトは当たり判定が無い
				break;

			default:			//敵そのものに当たった場合
			//	spimg=sprite_getcurrimg(c);
			//	parsys_add(spimg, 2,2, c->x,c->y, 10, 270, 10, 30, EXPLODE|DIFFSIZE, NULL);
				//SDL_FreeSurface(spimg);
			//	spimg=NULL;
				playChunk(4);		//***090127	変更先
				d->state=PL_HIT_BOMB;		//***090125		追加
				d->enemy=c;
				//explosion_add(c->x+5,c->y+5,0,rand()%3+1);		//***090125		以下コメントアウト
				//d->explode=0;

				//spimg=sprite_getcurrimg(s);
				//parsys_add(spimg, 2,2, c->x,c->y, 10, 90, 10, 30, EXPLODE|DIFFSIZE, &d->explode);
				//SDL_FreeSurface(spimg);
				//spimg=NULL;
				//c->type=-1;
				//s->flags&=~SP_FLAG_VISIBLE;
				//d->core->flags&=~SP_FLAG_VISIBLE;		//○も消す
				//d->lives--;
				//d->bonus=0;
		}
	}
}

void player_colcheck2(SPRITE *s, int mask)
{
	is_graze=0;
	/*
		s		プレイヤー
		mask	調べる対象？
				敵か敵の弾かアイテムか
	*/
	SPRITE *c;	//対象
	//SDL_Surface *spimg;
	PLAYER_DATA *d=(PLAYER_DATA *)s->data;

	/* Kollision Player <> Feind, Feindwaffe oder Bonusitem */
	if((c=sprite_colcheck2(s,mask))!=NULL) {
		
		switch(c->type) {
			case SP_EN_BULLET:
				is_graze=1;
				int i;
				for(i=0;i<5;i++)
				{
					if(graze_check[i]==((BULLET_DATA *)c->data)->id)	//同じ場合
						break;											//forを抜ける
				}
				if(i==5)	//同じIDの弾が無かった時
				{
					d->graze++;
					d->score+=100+(difficulty*200);
					graze_check[(d->graze)%5]=((BULLET_DATA *)c->data)->id;
					playChunk(9);
				}
				break;
				
			case SP_EN_LASER:
				is_graze=1;
				int j;
				for(j=0;j<5;j++)
				{
					if(graze_check[j]==((LASER_DATA *)c->data)->id)	//同じ場合
						break;											//forを抜ける
				}
				if(j==5)
				{
					d->graze++;
					d->score+=100+(difficulty*200);
					graze_check[(d->graze)%5]=((LASER_DATA *)c->data)->id;
					playChunk(9);
				}
				break;
			
			case SP_EN_BIGBULLET:
				is_graze=1;
				int l;
				for(l=0;l<5;l++)
				{
					if(graze_check[l]==((BIGBULLET_DATA *)c->data)->id)	//同じ場合
						break;											//forを抜ける
				}
				if(l==5)
				{
					d->graze++;
					d->score+=100+(difficulty*200);
					graze_check[(d->graze)%5]=((BIGBULLET_DATA *)c->data)->id;
					playChunk(9);
				}
				break;
				
			case SP_EN_BIGBULLET_DUMMY:		//2つ目のスプライトは当たり判定が無い
				break;

			
		}
	}
}

SPRITE *player_add_core(SPRITE *s1)		//○の追加
{
	SPRITE *s2;
	switch(select_player){
		case REIMU:
			s2=sprite_add_file("core.png",1,PR_PLAYER2);
			break;
		case MARISA:
			s2=sprite_add_file("core-ma.png",1,PR_PLAYER2);
			break;
	}
	s2->anim_speed=0;
	s2->type=SP_PLAYER2;
	s2->x=s1->x+s1->w/2-s2->w/2;
	s2->y=s1->y-s2->h+20;
	s2->mover=player_move_core;
	s2->flags|=SP_FLAG_VISIBLE;
	return s2;
}

void player_move_core(SPRITE *s2)
{
	s2->x=player->x+player->w/2-s2->w/2-1;
	s2->y=player->y-s2->h+20;
	if(is_graze)	//プレイヤースプライトが弾に触れているか
	{
		if(((PLAYER_DATA *)player->data)->state==PL_NORMAL){
			SPRITE *c;
			if((c=sprite_colcheck(s2,SP_SHOW_ENEMY_WEAPONS))!=NULL) {
				switch(c->type) {			//各弾の処理。弾の種類を追加した場合。ここにも追加する必要あり。
					case SP_EN_BULLET:
						playChunk(4);		//***090127	変更先
						((PLAYER_DATA *)player->data)->state=PL_HIT_BOMB;
						((PLAYER_DATA *)player->data)->enemy=c;
						break;
					case SP_EN_LASER:
						playChunk(4);		//***090127	変更先
						((PLAYER_DATA *)player->data)->state=PL_HIT_BOMB;
						((PLAYER_DATA *)player->data)->enemy=c;
						break;
					case SP_EN_BIGBULLET:
						playChunk(4);		//***090127	変更先
						((PLAYER_DATA *)player->data)->state=PL_HIT_BOMB;
						((PLAYER_DATA *)player->data)->enemy=c;
						break;
				}
			}
		}
	}
}

void re_add_option(SPRITE *s)	//霊夢
{
	RE_OPTION_DATA *d1, *d2;

	c1=sprite_add_file("option.png",8,PR_PLAYER);
	c1->flags|=SP_FLAG_VISIBLE;
	d1=mmalloc(sizeof(RE_OPTION_DATA));
	c1->data=d1;
	c1->mover=re_move_option;
	d1->length=11;
	d1->pos=-1;
	d1->ani_speed=2;
	d1->angle=-M_PI;
	d1->state=0;
	c1->aktframe=0;
	c1->type=SP_PLAYER2;
	c1->x=s->x+s->w/2-d1->length-c1->w/2;
	c1->y=s->y+s->h/2-c1->h/2;
	d1->w_wait=0;

	c2=sprite_add_file("option.png",8,PR_PLAYER);
	c2->flags|=SP_FLAG_VISIBLE;
	d2=mmalloc(sizeof(RE_OPTION_DATA));
	c2->data=d2;
	c2->mover=re_move_option;
	d2->length=11;
	d2->pos=1;
	d2->ani_speed=2;
	d2->angle=0;
	d2->state=0;
	c2->aktframe=0;
	c2->type=SP_PLAYER2;
	c2->x=s->x+s->w/2-d2->length-c2->w/2;
	c2->y=s->y+s->h/2-c2->h/2;
	d2->w_wait=0;
}

void re_move_option(SPRITE *s)	//霊夢
{
	RE_OPTION_DATA *o=(RE_OPTION_DATA *)s->data;
	
	if(o->ani_speed==0){
		s->aktframe=(s->aktframe+o->pos)%8;
		if(s->aktframe<0)
			s->aktframe=7;
		o->ani_speed=2;
	}
	else
		o->ani_speed--;
	
	if(((PLAYER_DATA *)player->data)->state==PL_SAVE||((PLAYER_DATA *)player->data)->state==PL_NORMAL)
	{
		if(o->w_wait<0){
			if(keyboard[KEY_SHOT]){
				if(((PLAYER_DATA *)player->data)->weapon<=90){
					re_add_fireball(s, s->x, s->y-7);
				}
				else{
					re_add_fireball(s, s->x-5, s->y-7);
					re_add_fireball(s, s->x+5, s->y-7);
				}	
			}
			o->w_wait=4;
		}
		else
			o->w_wait--;
	}
	if(keyboard[KEY_SLOW]){
		switch(o->state){
			case 0:
				o->state=1;
				o->angle-=o->pos*0.1;
				break;
			case 1:
				if((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05)){
					o->state=2;
					o->angle=-M_PI/2;
				}
				else
					o->angle-=o->pos*0.1;
				break;
			case 2:
				break;
		}
	}
	else{
		switch(o->state){
			case 0:
				break;
			case 1:
				if(o->angle<-M_PI || o->angle>0){
					o->state=0;
					o->angle=(o->pos+1)*M_PI/2-M_PI;
				}
				else
					o->angle+=o->pos*0.1;
				break;
			case 2:
				o->state=1;
				o->angle+=o->pos*0.1;
				break;
		}
	}
	if(((PLAYER_DATA *)player->data)->option==0 || ((PLAYER_DATA *)player->data)->lives==0)
		s->type=-1;
	s->y=player->y+player->h/2+sin(o->angle)*o->length-s->h/2-5;
	s->x=player->x+(o->pos+1)*player->w/2+cos(o->angle)*o->length-s->w/2-o->pos*6;
}

void ma_add_option(SPRITE *s)	//魔理沙
{
	RE_OPTION_DATA *d1, *d2;		//霊夢のを流用してます。

	c1=sprite_add_file("core.png",1,PR_PLAYER);
	c1->flags|=SP_FLAG_VISIBLE;
	d1=mmalloc(sizeof(RE_OPTION_DATA));
	c1->data=d1;
	c1->mover=ma_move_option;
	d1->length=11;
	d1->pos=-1;
	d1->angle=-M_PI;
	d1->state=0;
	c1->aktframe=0;
	c1->type=SP_PLAYER2;
	c1->x=s->x+s->w/2-d1->length-c1->w/2;
	c1->y=s->y+s->h/2-c1->h/2;
	d1->w_wait=0;

	c2=sprite_add_file("core.png",1,PR_PLAYER);
	c2->flags|=SP_FLAG_VISIBLE;
	d2=mmalloc(sizeof(RE_OPTION_DATA));
	c2->data=d2;
	c2->mover=ma_move_option;
	d2->length=11;
	d2->pos=1;
	d2->angle=0;
	d2->state=0;
	c2->aktframe=0;
	c2->type=SP_PLAYER2;
	c2->x=s->x+s->w/2-d2->length-c2->w/2;
	c2->y=s->y+s->h/2-c2->h/2;
	d2->w_wait=0;
}

void ma_move_option(SPRITE *s)	//魔理沙
{
	RE_OPTION_DATA *o=(RE_OPTION_DATA *)s->data;
	
	if(((PLAYER_DATA *)player->data)->state==PL_SAVE||((PLAYER_DATA *)player->data)->state==PL_NORMAL)
	{
		if(o->w_wait<0){
			if(keyboard[KEY_SHOT]){
				if(((PLAYER_DATA *)player->data)->weapon<=70){
					ma_add_fireball(s, s->x-5, s->y-7);
				}
				else if(((PLAYER_DATA *)player->data)->weapon<=100){
					ma_add_fireball(s, s->x-12, s->y-7);
					ma_add_fireball(s, s->x+2, s->y-7);
				}
				else{
					ma_add_fireball(s, s->x-12, s->y-7);
					ma_add_fireball(s, s->x-5, s->y-45);
					ma_add_fireball(s, s->x+2, s->y-7);
				}	
			}
			o->w_wait=8;
		}
		else
			o->w_wait--;
	}
	if(keyboard[KEY_SLOW]){
		switch(o->state){
			case 0:
				o->state=1;
				o->angle-=o->pos*0.1;
				break;
			case 1:
				if((o->angle>-M_PI/2-0.05)&&(o->angle<-M_PI/2+0.05)){
					o->state=2;
					o->angle=-M_PI/2;
				}
				else
					o->angle-=o->pos*0.1;
				break;
			case 2:
				break;
		}
	}
	else{
		switch(o->state){
			case 0:
				break;
			case 1:
				if(o->angle<-M_PI || o->angle>0){
					o->state=0;
					o->angle=(o->pos+1)*M_PI/2-M_PI;
				}
				else
					o->angle+=o->pos*0.1;
				break;
			case 2:
				o->state=1;
				o->angle+=o->pos*0.1;
				break;
		}
	}
	if(((PLAYER_DATA *)player->data)->option==0 || ((PLAYER_DATA *)player->data)->lives==0)
		s->type=-1;
	s->y=player->y+player->h/2+sin(o->angle)*o->length-s->h/2-5;
	s->x=player->x+(o->pos+1)*player->w/2+cos(o->angle)*o->length-s->w/2-o->pos*6;
}

void re_add_fireball(SPRITE *s, int x, int y)	//霊夢
{
	SPRITE *shot;
	PL_FIREBALL_DATA *data;

	shot=sprite_add_file2("fireball1.png",2,PR_PLAYER);
	shot->anim_speed=1;
	shot->type=SP_PL_FIREBALL;
	shot->x=x;
	shot->y=y;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=18;
	data->strength=3;
}

void ma_add_fireball(SPRITE *s, int x, int y)	//魔理沙
{
	SPRITE *shot;
	PL_FIREBALL_DATA *data;

	shot=sprite_add_file("missile.png",1,PR_PLAYER);
	shot->anim_speed=0;
	shot->type=SP_PL_FIREBALL;
	shot->x=x;
	shot->y=y;
	shot->alpha=150;
	shot->mover=player_move_fireball;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_FIREBALL_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=10;
	data->strength=4;
}

void player_move_fireball(SPRITE *s)	//霊夢、魔理沙
{
	PL_FIREBALL_DATA *p=(PL_FIREBALL_DATA *)s->data;


	s->x+=cos(degtorad(p->angle))*p->speed*fps_factor;
	s->y+=sin(degtorad(p->angle))*p->speed*fps_factor;

	if(s->y<-10) {
		s->type=-1;
	} else {
		weapon_colcheck(s,p->angle,1,0);
	}
}

void player_add_fuda(SPRITE *player)	//霊夢
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;	
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;
}

void player_add_doublefuda(SPRITE *player)	//霊夢
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;
}

void player_add_triplefuda(SPRITE *player)	//霊夢
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-6;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+6;
	data->speed=15;
	data->strength=2;
}

void player_add_quadfuda(SPRITE *player)	//霊夢
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-10;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+10;
	data->speed=15;
	data->strength=2;
}

void player_add_fifthfuda(SPRITE *player)	//霊夢
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-15;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-8;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+8;
	data->speed=15;
	data->strength=2;

	shot=sprite_add_file("plasma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+15;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+15;
	data->speed=15;
	data->strength=2;
}

void player_add_star(SPRITE *player)	//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=3;
}

void player_add_doublestar(SPRITE *player)	//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=3;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=3;
}

void player_add_triplestar(SPRITE *player)		//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-4;
	data->speed=12;
	data->strength=2;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=12;
	data->strength=2;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+5;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	shot->alpha=150;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+4;
	data->speed=12;
	data->strength=2;
}

void player_add_quadstar(SPRITE *player)	//魔理沙
{
	SPRITE *shot;
	PL_PLASMA_DATA *data;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-10;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-7;
	data->speed=12;
	data->strength=3;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2-6;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270-3;
	data->speed=12;
	data->strength=3;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+6;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+3;
	data->speed=12;
	data->strength=3;

	shot=sprite_add_file("plasma_ma.png",1,PR_PLAYER);
	shot->type=SP_PL_PLASMA;
	shot->x=player->x+player->w/2-shot->w/2+10;
	shot->y=player->y-shot->h+25;
	shot->mover=player_move_plasma;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_PLASMA_DATA));
	shot->data=data;
	data->angle=270+7;
	data->speed=12;
	data->strength=3;
}

void player_move_plasma(SPRITE *s)	//霊夢
{
	PL_PLASMA_DATA *p=(PL_PLASMA_DATA *)s->data;


	s->x+=cos(degtorad(p->angle))*p->speed*fps_factor;
	s->y+=sin(degtorad(p->angle))*p->speed*fps_factor;

	if(s->y<-10) {
		s->type=-1;
	} else {
		weapon_colcheck(s,p->angle,1,0);
	}
}

void player_add_killray(SPRITE *player)		//使わない
{
	SPRITE *shot;
	PL_KILLRAY_DATA *data;
	static int last=0;

	if(last) {
		shot=sprite_add_file("killray-r.png",12,PR_PLAYER);
		last=0;
	} else {
		shot=sprite_add_file("killray-b.png",12,PR_PLAYER);
		last=1;
	}
	shot->type=SP_PL_KILLRAY;
	shot->x=player->x+player->w/2-shot->w/2;
	shot->y=player->y-shot->h+20;
	shot->mover=player_move_killray;
	shot->flags|=SP_FLAG_VISIBLE;
	data=mmalloc(sizeof(PL_KILLRAY_DATA));
	shot->data=data;
	data->angle=270;
	data->speed=25;
	data->dist=0;
	data->strength=6;
}

void player_move_killray(SPRITE *s)
{
	PL_KILLRAY_DATA *p=(PL_KILLRAY_DATA *)s->data;

	s->x+=cos(degtorad(p->angle))*p->speed*fps_factor;
	s->y+=sin(degtorad(p->angle))*p->speed*fps_factor;
	p->dist+=p->speed*fps_factor;

	if((p->dist>20)&&(s->aktframe<11)) {
		p->dist=0;
		s->aktframe++;
	}

	if(s->y<-10) {
		s->type=-1;
	} else {
		weapon_colcheck(s,p->angle,1,0);
	}
}


void player_add_bomb(SPRITE *s)
{
	SPRITE *c;
	PL_BOMB_DATA *b;

	int a;

	for(a=0;a<355;a+=5) {
		c=sprite_add_file("fireball.png",1,PR_PLAYER);
		c->type=SP_PL_BOMB;
		c->x=player->x+player->w/2;
		c->y=player->y+player->h/2;
		c->mover=player_move_bomb;
		c->flags|=SP_FLAG_VISIBLE;
		b=mmalloc(sizeof(PL_BOMB_DATA));
		c->data=b;
		b->angle=a;
		b->speed=1;
		b->strength=2;
	}
}

void player_move_bomb(SPRITE *s)
{
	static int k=5;
	PL_BOMB_DATA *b=(PL_BOMB_DATA *)s->data;

	s->x+=cos(degtorad(b->angle))*b->speed*fps_factor;
	s->y+=sin(degtorad(b->angle))*b->speed*fps_factor;

	if((s->x<0-100)||(s->x>WIDTH+100)||(s->y<0-100)||(s->y>HEIGHT+100)) {
		s->type=-1;
	} else {
		weapon_colcheck(s,b->angle,1,0);
	}
	if(k>0){k--;}else{b->speed++;k=5;}
}

void player_add_homing(SPRITE *s)
{
	SPRITE *c;
	SPRITE *t;
	PL_HOMING_DATA *b;

	c=sprite_add_file("homing.png",20,PR_PLAYER);
	c->type=SP_PL_HOMING;
	c->x=(s->x+s->w/2)-40;
	c->y=s->y+s->h/2;
	c->mover=player_move_homing;
	c->flags|=SP_FLAG_VISIBLE;
	b=mmalloc(sizeof(PL_HOMING_DATA));
	c->data=b;
	b->nr=0;
	b->angle=100;
	b->speed=7;
	b->strength=5;
	b->range=350;
	b->state=0;
	b->target_id=-1;
	t=sprite_add_file("target.png",1,PR_TARGET);
	b->tgsprite=t;
	t->type=SP_ETC;
	t->flags&=~(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE);
	t->mover=NULL;
	t->data=NULL;
	t->anim_speed=0;

	c=sprite_add_file("homing.png",20,PR_PLAYER);
	c->type=SP_PL_HOMING;
	c->x=(s->x+s->w/2)-10;
	c->y=s->y+s->h/2;
	c->mover=player_move_homing;
	c->flags|=SP_FLAG_VISIBLE;
	b=mmalloc(sizeof(PL_HOMING_DATA));
	c->data=b;
	b->nr=1;
	b->angle=80;
	b->speed=7;
	b->strength=5;
	b->range=350;
	b->state=0;
	b->target_id=-1;
	t=sprite_add_file("target.png",1,PR_TARGET);
	b->tgsprite=t;
	t->type=SP_ETC;
	t->flags&=~(SP_FLAG_COLCHECK|SP_FLAG_VISIBLE);
	t->mover=NULL;
	t->data=NULL;
	t->anim_speed=0;
}

void player_move_homing(SPRITE *s)
{
	PL_HOMING_DATA *b=(PL_HOMING_DATA *)s->data;
	int ta;
	SPRITE *tg=(SPRITE *)b->tgsprite; /* Target-Fadenkreuz */
	SPRITE *target=NULL;	/* Target */
	double dangle;

	switch(b->state) {
		case 0:	/* Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
			if(b->nr==0)
				b->angle+=8*fps_factor;	/* linke Rakete: rot. im UZ */
			else
				b->angle-=8*fps_factor;	/* rechte Rekete: rot. gegen UZ */

			if(b->angle<0)
				b->angle+=360;
			if(b->angle>360)
				b->angle-=360;

			if((b->angle>=266)&&(b->angle<=274)) {
				/* Sprengkopf in Ausgangspos? */
				b->state=2;
				b->angle=270;
			}
			break;

		case 2: /* Ziel suchen */
			b->target_id=search_enemy();
			if(b->target_id!=-1) {
				target=sprite_get_by_id(b->target_id);
				if(target!=NULL) {
					b->state=3;
					target->flags|=SP_FLAG_PARANOIA;
					tg->flags|=SP_FLAG_VISIBLE;
				} else {
					tg->flags&=~SP_FLAG_VISIBLE;
				}
			}
			break;
		case 3: /* Ziel verfolgen */
			if(b->target_id==-1) {
				error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
				b->state=2;
			} else {
				target=sprite_get_by_id(b->target_id);
				if((target!=NULL) &&
				   (target->flags&SP_FLAG_VISIBLE) &&
				   (target->x>0) &&
				   (target->x<WIDTH2-target->w) &&
				   (target->y>0) &&
				   (target->y<screen->h-target->h)) {

					ta=radtodeg(atan2(target->y-s->y,target->x-s->x));

					if(b->angle>ta) {
						if(b->angle-ta<175)
							b->angle-=9*fps_factor;
						else if(b->angle-ta>185)
							b->angle+=9*fps_factor;
					} else if(b->angle<ta) {
						if(ta-b->angle<175)
							b->angle+=9*fps_factor;
						else if(ta-b->angle>185)
							b->angle-=9*fps_factor;
					}

					if(b->angle<0)
						b->angle+=360;
					if(b->angle>360)
						b->angle-=360;

					tg->x=target->x+target->w/2-tg->w/2+rand()%6-3;
					tg->y=target->y+target->h/2-tg->h/2+rand()%6-3;
				} else {
					/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
					b->target_id=-1;
					tg->flags&=~SP_FLAG_VISIBLE;
					b->state=2;
				}
			}
			break;
	}

	s->x+=cos(degtorad(b->angle))*b->speed*fps_factor;
	s->y+=sin(degtorad(b->angle))*b->speed*fps_factor;

	dangle=b->angle+180;
	if(dangle>360)
		dangle-=360;
	//parsys_add(NULL,20,0,s->x+s->w/2,s->y+s->h/2,20,dangle,30,10,PIXELATE,NULL);

	s->aktframe=(b->angle*20)/360;
	s->aktframe%=20;

	weapon_colcheck(s,b->angle,1,0);

	b->range-=fps_factor;
	if(b->range<=0) { /* max. Reichweite erreicht? */
		s->type=-1;
		tg->type=-1;
		if(target!=NULL)
			/* Target freigeben */
			target->flags&=~SP_FLAG_PARANOIA;
	}
}

int search_enemy()
{
	/* Suche Enemy-Sprite, das noch nicht verfolgt wird, liefert SPRITE-ID oder -1 */
	SPRITE *s=sprite;
	
	while(s!=NULL) {
		if((s->type&SP_SHOW_ENEMYS)&&(!(s->flags&SP_FLAG_PARANOIA))&&(s->flags&SP_FLAG_VISIBLE)) {
			if((s->x>0)&&(s->x<WIDTH2-s->w)&&(s->y>0)&&(s->y<screen->h-s->h)) {
				return s->id;
			}
		}
		s=s->next;
	}
	return -1;
}


void player_add_shield(SPRITE *s)		//シールドの追加	//霊夢
{
	SPRITE *c;
	PL_SHIELD_DATA *d;
	PLAYER_DATA *sd=(PLAYER_DATA *)s->data;
	int i;
	sd->bonus=0x02;
	for(i=0;i<=359;i+=45) {
		if(i%90 == 0)
		{
			c=sprite_add_file2("cshoot1.png",36,PR_PLAYER);
			c->flags|=SP_FLAG_VISIBLE;
			d=mmalloc(sizeof(PL_SHIELD_DATA));
			c->data=d;
			c->mover=player_move_shield;
			d->angle=i;
			d->speed=23;
			d->rad=38;
			c->type=SP_PL_SHIELD;
		}
		else		//1つだと不安なので2つ作った。
		{
			c=sprite_add_file2("cshoot2.png",36,PR_PLAYER);
			c->flags|=SP_FLAG_VISIBLE;
			d=mmalloc(sizeof(PL_SHIELD_DATA));
			c->data=d;
			c->mover=player_move_shield2;
			d->angle=i;
			d->speed=17;
			d->rad=35;
			c->type=SP_PL_SHIELD;
		}
	}

}

void player_move_shield(SPRITE *s)	//霊夢
{
	PL_SHIELD_DATA *d=(PL_SHIELD_DATA *)s->data;
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;

	d->angle+=(d->speed*fps_factor);
	if(d->angle>360)
		d->angle-=360;
	s->x=player->x+player->w/2+cos(degtorad(d->angle))*d->rad-s->w/2;
	/*
		s->x		シールドのx座標
		player->x	プレイヤーのx座標
		player->w/2	プレイヤーの横幅の半分
		cos(degtorad(d->angle))		angleの角度をπに直したあとcos。この場合は40°の倍数＋１０*fpsのcos。
		d->rad		40
		s->w/2		シールドの横幅の半分
		プレイヤーのx座標＋プレイヤーの横幅の半分＝プレイヤーの中心座標
		なので
		シールドのx座標＝プレイヤーのx座標＋プレイヤーの横幅の半分＋cos(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
		               ＝プレイヤーの中心座標＋cos(radian((４０°*x)＋speed*fps))×40－シールドの横幅の半分
	*/
	s->y=player->y+player->h/2+sin(degtorad(d->angle))*d->rad-s->h/2;		//反時計回り
	s->aktframe=(d->angle/10);
	s->aktframe%=36;

	weapon_colcheck(s,d->angle,0,1);

	if(p->extra_time<=0)
		s->type=-1;
}

void player_move_shield2(SPRITE *s)	//霊夢
{
	PL_SHIELD_DATA *d=(PL_SHIELD_DATA *)s->data;
	PLAYER_DATA *p=(PLAYER_DATA *)player->data;

	d->angle+=(d->speed*fps_factor);
	if(d->angle>360)
		d->angle-=360;
	s->x=player->x+player->w/2+cos(degtorad(d->angle))*d->rad-s->w/2;
	s->y=player->y+player->h/2-sin(degtorad(d->angle))*d->rad-s->h/2;		//時計回り
	s->aktframe=(d->angle/10);
	s->aktframe%=36;

	weapon_colcheck(s,d->angle,0,1);

	if(p->extra_time<=0)
		s->type=-1;
}

void player_add_levarie(SPRITE *s)	//魔理沙
{
	SPRITE *c;
	PL_LEVARIE_DATA *d;
	PLAYER_DATA *sd=(PLAYER_DATA *)s->data;
	int i;
	double ang=0;

	sd->bonus=0x02;
	for(i=0;i<16;i++){
		switch(i%6){
			case 0:
				c=sprite_add_file("star_shield_blue.png",3,PR_PLAYER);
				break;
			case 1:
				c=sprite_add_file("star_shields_red.png",3,PR_PLAYER);
				break;
			case 2:
				c=sprite_add_file("star_shield_green.png",3,PR_PLAYER);
				break;
			case 3:
				c=sprite_add_file("star_shields_blue.png",3,PR_PLAYER);
				break;
			case 4:
				c=sprite_add_file("star_shield_red.png",3,PR_PLAYER);
				break;
			case 5:
				c=sprite_add_file("star_shields_green.png",3,PR_PLAYER);
				break;
		}
		c->anim_speed=5;
		c->flags|=SP_FLAG_VISIBLE;
		d=mmalloc(sizeof(PL_LEVARIE_DATA));
		c->data=d;
		c->mover=player_move_levarie;
		d->angle=ang;
		if(i%2==0){
			d->speed=1;
			c->alpha=150;
		}
		else{
			d->speed=1.2;
			c->alpha=180;
		}
		c->type=SP_PL_SHIELD;
		c->x=s->x+s->w/2-c->w/2;
		c->y=s->y+s->h/2-c->h/2;
		ang+=0.39269908169872415481;
	}
}

void player_move_levarie(SPRITE *s){
	PL_LEVARIE_DATA *d=(PL_LEVARIE_DATA *)s->data;
	
	s->x+=cos(d->angle)*d->speed*fps_factor;
	s->y+=sin(d->angle)*d->speed*fps_factor;

	weapon_colcheck(s,d->angle,0,1);

	if((s->x+s->w<0)||(s->x>WIDTH2)||(s->y+s->h<0)||(s->y>HEIGHT)) {
		s->type=-1;
	}
}

void player_add_hlaser(SPRITE *s)
{
	SPRITE *c;
	PL_HLASER_DATA *b;
	CONTROLLER *e;
	int *id_array;
	int i,j;

	for(j=0;j<2;j++) {
		e=controller_add();
		e->max=12;		//***090128		半分にしてみる
		id_array=mmalloc(sizeof(int)*e->max);
		e->e=id_array;
		e->con=player_controller_hlaser;

		for(i=0;i<e->max;i++) {
			switch(select_player){
				case REIMU:
					c=sprite_add_file("tshoot.png",6,PR_PLAYER);
					break;
				case MARISA:
					c=sprite_add_file("tshoot-ma.png",6,PR_PLAYER);
					break;
			}
			id_array[i]=c->id;
			c->type=SP_PL_HLASER;
			c->x=(s->x+s->w/2)-5;
			c->y=s->y+s->h/2+15;
			c->flags|=SP_FLAG_VISIBLE;
			c->aktframe=5-((double)6/e->max)*i;
			if(i==0) {
				c->mover=player_move_hlaser;
				b=mmalloc(sizeof(PL_HLASER_DATA));
				c->data=b;
				b->nr=j;
				b->angle=100;
				b->speed=12;
				b->strength=6;
				b->range=200;
				b->state=0;
				b->target_id=-1;
			} else {
				c->mover=NULL;
				c->data=NULL;
			}
		}
	}
}

void player_controller_hlaser(CONTROLLER *c)
{
	int *id_array=c->e;
	SPRITE *s, *l=NULL;
	int i;

	if(sprite_get_by_id(id_array[0])==NULL) {
		for(i=1;i<c->max;i++) {
			s=sprite_get_by_id(id_array[i]);
			if(s!=NULL)
				s->type=-1;
		}
		controller_remove(c);
	} else {
		for(i=c->max-1;i>=1;i--) {
			s=sprite_get_by_id(id_array[i]);
			l=sprite_get_by_id(id_array[i-1]);
			if((s!=NULL)&&(l!=NULL)) {
				// draw_line(screen,s->x,s->y,l->x,l->y,
				// SDL_MapRGB(screen->format,0xff,0xff,0xff),
				// SDL_MapRGB(screen->format,0xa0,0xff,0xa0));
				s->x=l->x;
				s->y=l->y;
			}
		}
	}
}
		

void player_move_hlaser(SPRITE *s)
{
	PL_HLASER_DATA *b=(PL_HLASER_DATA *)s->data;
	int ta;
	SPRITE *target=NULL;	/* Target */

	switch(b->state) {
		case 0:	/* Raketen in Ausgangspos. bringen (Sprengkopf nach Norden) */
			if(b->nr==0)
				b->angle+=8*fps_factor;	/* linke Rakete: rot. im UZ */
			else
				b->angle-=8*fps_factor;	/* rechte Rekete: rot. gegen UZ */

			if(b->angle<0)
				b->angle+=360;
			if(b->angle>360)
				b->angle-=360;

			if((b->angle>=266)&&(b->angle<=274)) {
				/* Sprengkopf in Ausgangspos? */
				b->state=2;
				b->angle=270;
			}
			break;

		case 2: /* Ziel suchen */
			b->target_id=search_enemy();
			if(b->target_id!=-1) {
				target=sprite_get_by_id(b->target_id);
				if(target!=NULL) {
					b->state=3;
					target->flags|=SP_FLAG_PARANOIA;
				}
			}
			break;
		case 3: /* Ziel verfolgen */
			if(b->target_id==-1) {
				error(ERR_WARN,"homing: target_id==-1&&state==3 ->back to state 2 (this shouldn't happen)");
				b->state=2;
			} else {
				target=sprite_get_by_id(b->target_id);
				if((target!=NULL) &&
				   (target->flags&SP_FLAG_VISIBLE) &&
				   (target->x>0) &&
				   (target->x<WIDTH2-target->w) &&
				   (target->y>0) &&
				   (target->y<screen->h-target->h)) {

					ta=radtodeg(atan2(target->y-s->y,target->x-s->x));

					if(b->angle>ta) {
						if(b->angle-ta<175)
							b->angle-=8*fps_factor;
						else if(b->angle-ta>185)
							b->angle+=8*fps_factor;
					} else if(b->angle<ta) {
						if(ta-b->angle<175)
							b->angle+=8*fps_factor;
						else if(ta-b->angle>185)
							b->angle-=8*fps_factor;
					}

					if(b->angle<0)
						b->angle+=360;
					if(b->angle>360)
						b->angle-=360;

				} else {
					/* Ziel wurde anderweitig vernichtet, neues Ziel suchen */
					b->target_id=-1;
					b->state=2;
				}
			}
			break;
	}

	s->x+=cos(degtorad(b->angle))*b->speed*fps_factor;
	s->y+=sin(degtorad(b->angle))*b->speed*fps_factor;

	weapon_colcheck(s,b->angle,1,0);

	b->range-=fps_factor;
	if(b->range<=0) { /* max. Reichweite erreicht? */
		s->type=-1;
		if(target!=NULL)
			/* Target freigeben */
			target->flags&=~SP_FLAG_PARANOIA;
	}
}

void weapon_colcheck(SPRITE *s, int angle, int destroy, int check_bullets)
{
	//s		主人公の弾
	SPRITE *c;		//敵スプライト
	SDL_Surface *spimg;
	ENEMY_BASE *d;
	WEAPON_BASE *b=(WEAPON_BASE *)s->data;
	PL_HOMING_DATA *h=(PL_HOMING_DATA *)s->data;
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	char buffer[50];

	/* wenn flag check_bullets gesetzt ist, werden auch gegnerische schuesse getroffen */
	if(check_bullets) {
		if((c=sprite_colcheck(s,SP_SHOW_ENEMY_WEAPONS))!=NULL) {	//敵の弾なのか？
			playChunk(2);
			//spimg=sprite_getcurrimg(c);
			//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 30, EXPLODE|DIFFSIZE, NULL);
			//SDL_FreeSurface(spimg);
			c->type=-1;
		}
	}

	if((c=sprite_colcheck(s,SP_SHOW_ENEMYS))!=NULL) {		//敵そのものなのか？
		
		if(s->type==SP_PL_HOMING) {
			/* Gegner von Homing-Missile getroffen? */
			/* Paranoia-Flag und Target-Fadenkreuz entfernen */
			h=(PL_HOMING_DATA *)s->data;
			h->tgsprite->type=-1;
		}

		/* wenn flag destroy gesetzt ist, wird der Schuss vernichtet */
		if(destroy)
			s->type=-1;

		// ??? why did i do this ???
		// c->flags&=~SP_FLAG_PARANOIA;
		// ???

		switch(c->type) {
			case SP_EN_BOSS01:
				if(s->type!=SP_PL_SHIELD)		//ボスに当たったのがシールドだった場合はダメージ処理を行わない
				{
					enemy_boss01_hitbyweapon(c,s,angle);
				}
				break;

			case SP_EN_BOSS02:
				if(s->type!=SP_PL_SHIELD)
				{
					enemy_boss02_hitbyweapon(c,s,angle);
				}
				break;
			case SP_EN_BOSS03:
				if(s->type!=SP_PL_SHIELD)
				{
					enemy_boss03_hitbyweapon(c,s,angle);
				}
				break;
			case SP_EN_BOSS04:		//***090127		追加
				if(s->type!=SP_PL_SHIELD)
				{
					enemy_boss04_hitbyweapon(c,s,angle);
				}
				break;
			case SP_EN_GFAIRY:
				if(s->type!=SP_PL_SHIELD)
				{
					enemy_nonshield_hitbyweapon(c,s,angle);
				}
				break;

			default:
				d=(ENEMY_BASE *)c->data;
				d->health-=b->strength;
				//spimg=sprite_getcurrimg(c);
				if(d->health>0) {
				//	playChunk(2);
				//	parsys_add(spimg, 2,2, c->x,c->y, 10, 0, 0, 30, EXPLODE|DIFFSIZE, NULL);
				} else {
					playChunk(8);
				    	explosion_add(c->x,c->y+5,0,rand()%3+1);	
					//parsys_add(spimg, 2,2, c->x,c->y, 10, angle, 10, 50, EXPLODE|DIFFSIZE, NULL);
					switch(c->type){		//***090123		追加
						case SP_EN_GREETER:
							if(rand_percent(30)){
								if(rand_percent(50))
									bonus_add(c->x, c->y, SP_BONUS_FIREPOWER, s->type==SP_PL_SHIELD);
								else
									bonus_add(c->x, c->y, SP_BONUS_COIN, s->type==SP_PL_SHIELD);
							}
							break;
						case SP_EN_MING:
							if(rand_percent(30)){
								if(rand_percent(50))
									bonus_add(c->x, c->y, SP_BONUS_FIREPOWER, s->type==SP_PL_SHIELD);
								else
									bonus_add(c->x, c->y, SP_BONUS_COIN, s->type==SP_PL_SHIELD);
							}
							break;
						case SP_EN_GROUNDER:
							if(rand_percent(30))
								bonus_add(c->x, c->y, SP_BONUS_FIREPOWER, s->type==SP_PL_SHIELD);
							if(rand_percent(30))
								bonus_add(c->x+5, c->y+5, SP_BONUS_COIN, s->type==SP_PL_SHIELD);
							if(rand_percent(30))
								bonus_add(c->x+10, c->y-5, SP_BONUS_FIREPOWER, s->type==SP_PL_SHIELD);
							break;
						case SP_EN_BADGUY:
							if(rand_percent(10))
								bonus_add(c->x, c->y, SP_BONUS_ADDSPEED, s->type==SP_PL_SHIELD);
							break;
						case SP_EN_SPLASH:
							bonus_add(c->x, c->y, SP_BONUS_COIN, s->type==SP_PL_SHIELD);
							break;
						case SP_EN_FAIRY:
							if(rand_percent(30))
								bonus_add(c->x, c->y, SP_BONUS_FIREPOWER, s->type==SP_PL_SHIELD);
							break;
					}
					pd->score+=d->score;
					sprintf(buffer,"%d",d->score);
					bonus_info_text(c->x,c->y,buffer,FONT07);
					c->type=-1;
				}
				//SDL_FreeSurface(spimg);
				break;
		}
	}
}
