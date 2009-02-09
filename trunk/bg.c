#include "bg.h"

extern double fps_factor;
extern SDL_Surface *screen;
extern SPRITE *player;

int akt_bgtype;
double bg_alpha;

int is_bg_add;		//***090201		í«â¡:í«â¡ÉtÉâÉO
int is_bg_end;		//***090201		í«â¡:ëOîwåièIóπÉtÉâÉO
int is_bg_fin;		//***090202		í«â¡:îwåiîÒÉãÅ[ÉvÉtÉâÉO
int n_bg;		//***090209		í«â¡:í«â¡îwåiî‘çÜ

/* clouds */
SPRITE *w1[5];
SPRITE *w2[10];
SPRITE *w3[20];

/* tile */
SDL_Surface *btile=NULL;
SDL_Surface *btile2=NULL;		//***090201		í«â¡
double btile_y;

void bg_init(int bg_type,int lev)
{

	if((bg_type>=BG_LAST)||(bg_type<BG_BLACK))
		error(ERR_FATAL,"unknown BG_TYPE: %d",bg_type);

	akt_bgtype=bg_type;
	bg_alpha=0;
	switch(bg_type) {

	case BG_BLACK:
		break;

	case BG_CLOUDS:
		tile_init(lev);
		clouds_init(lev);
		break;

	case BG_STARS:
		// stars_init();
		break;

	case BG_TILE:
		tile_init(lev);
		break;
	}
	
}

void bg_work()
{
	if(akt_bgtype<0) {
		error(ERR_WARN,"bg_work with no bg_type");
		return;
	}

	if(bg_alpha<255)
		bg_alpha+=6*fps_factor;
	else
		bg_alpha=255;

	switch(akt_bgtype) {

	case BG_BLACK:
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		break;

	case BG_CLOUDS:
		// SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0xff));
		//SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		//SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,bg_alpha));
		tile_work();
		tile_display();
		break;

	case BG_STARS:
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		break;

	case BG_TILE:
		tile_work();
		tile_display();
		break;
	}
}

void bg_destroy()
{
	if(akt_bgtype<0) {
		error(ERR_WARN,"bg_destroy with no bg_type");
		return;
	}
	switch(akt_bgtype) {

	case BG_BLACK:
		break;

	case BG_CLOUDS:
		clouds_remove();
		break;

	case BG_STARS:
		// stars_remove();
		break;

	case BG_TILE:
		tile_remove();
		break;
	}
	akt_bgtype=-1;
}


void clouds_init(int lev)
{
	int i;
	CLOUDS_DATA *d;
	char filename[20];
	sprintf(filename,"wolke03_%d.png",lev);

	for(i=0;i<5;i++) {
		w3[i]=sprite_add_file(filename,1,PR_BACK0);
		w3[i]->x=rand()%WIDTH2-w3[i]->w;
		w3[i]->y=rand()%HEIGHT;
		w3[i]->flags&=~SP_FLAG_COLCHECK;
		w3[i]->flags|=SP_FLAG_VISIBLE;
		w3[i]->type=SP_ETC;
		w3[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w3[i]->data=d;
		w3[i]->alpha=bg_alpha;
		d->speed_base=1;
		d->speed_rand=(double)(rand()%100)/100;
	}

	sprintf(filename,"wolke02_%d.png",lev);
	for(i=0;i<2;i++) {
		w2[i]=sprite_add_file(filename,1,PR_BACK1);
		w2[i]->x=rand()%WIDTH2-w2[i]->w;
		w2[i]->y=rand()%HEIGHT;
		w2[i]->flags&=~SP_FLAG_COLCHECK;
		w2[i]->flags|=SP_FLAG_VISIBLE;
		w2[i]->type=SP_ETC;
		w2[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w2[i]->data=d;
		w2[i]->alpha=bg_alpha;
		d->speed_base=2;
		d->speed_rand=(double)(rand()%100)/100;
	}

	sprintf(filename,"wolke01_%d.png",lev);
	for(i=0;i<1;i++) {
		w1[i]=sprite_add_file(filename,1,PR_BACK2);
		w1[i]->x=rand()%WIDTH2-w1[i]->w;
		w1[i]->y=rand()%HEIGHT;
		w1[i]->flags&=~SP_FLAG_COLCHECK;
		w1[i]->flags|=SP_FLAG_VISIBLE;
		w1[i]->type=SP_ETC;
		w1[i]->mover=clouds_mover;
		d=mmalloc(sizeof(CLOUDS_DATA));
		w1[i]->data=d;
		w1[i]->alpha=bg_alpha;
		d->speed_base=3;
		d->speed_rand=(double)(rand()%100)/100;
	}
	
}

void clouds_remove()
{
	int i;

	for(i=0;i<5;i++) {
		if(i<1) w1[i]->type=-1;
		if(i<2) w2[i]->type=-1;
		w3[i]->type=-1;
	}
}

void clouds_mover(SPRITE *c)
{
	CLOUDS_DATA *d=(CLOUDS_DATA *)c->data;

	c->y+=d->speed_base*fps_factor;
	c->y+=d->speed_rand*fps_factor;
	if(c->y>HEIGHT) {
		c->y-=HEIGHT+c->w;
		d->speed_rand=(double)(rand()%100)/100;
		c->x=rand()%WIDTH2-c->w;
		//c->alpha=bg_alpha;
		c->alpha=200;
	}
}

void tile_init(int lev)
{
	char filename[20];
	sprintf(filename,"back%d.jpg",lev);
	//if(btile==NULL) {
		btile=loadbmp(filename);
	//}
	btile_y=WIDTH2;
	
}

void tile_add(int lev)		//***090201		í«â¡
{
	char filename[20];
	sprintf(filename,"back%d_a-%d.jpg",lev,n_bg);
	btile2=loadbmp(filename);
	is_bg_add=1;		//ìÒñáñ⁄ÇÃîwåií«â¡ÉtÉâÉO
}

void tile_work()
{
	if(!is_bg_fin){		//îÒÉãÅ[ÉvÉtÉâÉOÇ™óßÇ¡ÇƒÇ»Ç¢Ç∆Ç´(í èÌÉãÅ[Ég)
		btile_y+=fps_factor;
		if(is_bg_end){		//îwåií«â¡ñΩóﬂÇ™èoÇƒÇ©ÇÁëOîwåiÇ™èIí[Ç‹Ç≈óàÇΩÇ∆Ç´
			if(btile_y>btile2->h-1){
				btile_y-=btile2->h;
			}
			else if(btile_y>HEIGHT && btile!=NULL){	//â∫èÄîı
				unloadbmp_by_surface(btile);
				btile=NULL;
				unloadbmp_by_surface(btile2);
				btile2=NULL;
				char filename[20];
				PLAYER_DATA *d=(PLAYER_DATA *)player->data;
				
				sprintf(filename,"back%d_a-%d.jpg",d->level,n_bg);
				btile=loadbmp(filename);
				is_bg_add=0;
				is_bg_end=0;
				n_bg++;
			}
		}
		else{		//ì¡éÍÇ»ñΩóﬂÇ™èoÇƒÇ»Ç¢Ç∆Ç´orîwåií«â¡ñΩóﬂÇ™èoÇƒÇ©ÇÁëOîwåiÇ™èIí[Ç‹Ç≈óàÇƒÇ»Ç¢Ç∆Ç´
			if(btile_y>btile->h-1){		//btile_yÇ™ëOîwåiÇ∆ìØÇ∂çÇÇ≥Ç…Ç»Ç¡ÇΩÇ∆Ç´
				if(is_bg_add){		//îwåií«â¡ñΩóﬂÇ™èoÇΩÇ∆Ç´
					btile_y=0;
					is_bg_end=1;		//ëOîwåièIóπÉtÉâÉO
				}
				else{		//í èÌèàóù
					btile_y-=btile->h;
				}
			}
		}
	}
	else{		//îÒÉãÅ[ÉvÇÃèÍçá Å¶ç°ÇÃèäã@î\ÇµÇƒÇ»Ç¢Ç©Ç‡ÇµÇÍÇ»Ç¢
		if(is_bg_end){
			if(btile_y+fps_factor<=btile2->h-1){
				btile_y+=fps_factor;
			}
		}
		else{
			if(btile_y+fps_factor<=btile->h-1){
				btile_y+=fps_factor;
			}
		}
	}
}

void tile_display()
{
	int i,j;
	SDL_Rect r;
	SDL_Rect r2;
	if(bg_alpha<255) {
	 	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		SDL_SetAlpha(btile,SDL_SRCALPHA,bg_alpha);
	} else 
		SDL_SetAlpha(btile,SDL_SRCALPHA,255);
	if(is_bg_add || is_bg_end){
		SDL_SetAlpha(btile2,SDL_SRCALPHA,bg_alpha);
		r2.w=btile2->w;
		r2.h=btile2->h;
	}

	r.w=btile->w;
	r.h=btile->h;

	if(is_bg_end){
		for(i=0;i<WIDTH2;i+=btile2->w){
			r2.x=i;
			r2.y=-btile2->h+btile_y;
			SDL_BlitSurface(btile2,NULL,screen,&r2);
		}
		for(i=0;i<WIDTH2;i+=btile->w) {
			for(j=btile_y;j<HEIGHT;j+=btile->h) {
				r.x=i;
				r.y=j;
				SDL_BlitSurface(btile,NULL,screen,&r);
			}
		}
	}
	else{
		for(i=0;i<WIDTH2;i+=btile->w) {
			for(j=-btile->h;j<HEIGHT;j+=btile->h) {
				r.x=i;
				r.y=j+btile_y;
				SDL_BlitSurface(btile,NULL,screen,&r);
			}
		}
	}
}

void tile_remove()
{ 
	if(btile!=NULL)
		unloadbmp_by_surface(btile);
	btile=NULL;

	if(btile2!=NULL)
		unloadbmp_by_surface(btile2);
	btile2=NULL;
}
