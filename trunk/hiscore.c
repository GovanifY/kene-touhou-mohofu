#include "hiscore.h"

/*
	typedef struct {
		char name[4];
		int score;
	} HSC_LIST;
*/

HSC_LIST hsc_table[5];
SPRITE *hscsprite[6];
SDL_Surface *hscsurface[6];
int all_inplace;
int nr_of_sprites;
SDL_Surface *back;
int lastscore;
int high_score;

extern GAMESTATE state;
extern SDL_Surface *screen;
extern int keyboard[];
extern KEYCONFIG keyconfig;
extern double fps_factor;


void hsc_init()
{
	int i;
	for(i=0;i<5;i++) {
		strcpy(hsc_table[i].name,"DEN");
		hsc_table[i].score=(5-i)*10000;
	}
}

void hsc_load()
{
	int i;
	FILE *fp;
	char fn[50];
	int tmpscore;
        strcpy(fn,moddir);
        strcat(fn,"/");
        strcat(fn,"highscore.txt");

 	if ( NULL == (fp = fopen(fn,"r")) ) {
    		return;
	}
	for(i=0;i<5;i++) {
		if (fscanf(fp,"%3s %d\n",hsc_table[i].name,&tmpscore)==2) {
			hsc_table[i].score=tmpscore;
			if(i == 0)
				high_score = hsc_table[i].score;		//��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
		}
	}
	fclose(fp);

}

void hsc_save(){
 int i;
        FILE *fp;
        char fn[50];
//        int tmpscore;
        strcpy(fn,moddir);
        strcat(fn,"/");
        strcat(fn,"highscore.txt");

        if ( NULL == (fp = fopen(fn,"w")) ) {
                return;
        }
        for(i=0;i<5;i++) {
                fprintf(fp,"%3s %d\n",hsc_table[i].name,hsc_table[i].score);
        }
	fclose(fp);
}

void hsc_show_init()
{
	char ttmp[50];
	int i;
	HSC_DATA *hd;

	for(i=0;i<5;i++) {
		sprintf(ttmp,"%02d  %-3s  %07d",i+1,hsc_table[i].name,hsc_table[i].score);
		// hscsurface[i]=font_render(ttmp,i==0?FONT05:FONT01);
		// hscsurface[i]=font_render(ttmp,i==0?FONT06:FONT07);
		hscsurface[i]=font_render(ttmp,FONT06);
		hscsprite[i]=sprite_add(hscsurface[i],1,PR_TEXT,1);
		hd=mmalloc(sizeof(HSC_DATA));
		hscsprite[i]->data=hd;
		hd->xg=20+(i*4);
		hd->yg=i*25+150;
		hd->arrived=0;
		hd->ph=i%2==0?0:180;
		hd->phspeed=4;
		hd->amp=300;
		hd->ampspeed=1;
		hd->dir=0;
		hscsprite[i]->flags|=SP_FLAG_VISIBLE;
		hscsprite[i]->type=SP_ETC;
		hscsprite[i]->x=0;
		hscsprite[i]->y=i*25+110;
		hscsprite[i]->mover=hsc_show_move;
	}

	hscsurface[5]=font_render("TOP FIVE FIGHTERS",FONT05);
	hscsprite[5]=sprite_add(hscsurface[5],1,PR_TEXT,1);
	hd=mmalloc(sizeof(HSC_DATA));
	hscsprite[5]->data=hd;
	hd->xg=30;
	hd->yg=110;
	hd->ph=0;
	hd->phspeed=5;
	hd->amp=300;
	hd->ampspeed=1.0;
	hd->arrived=0;
	hd->dir=0;
	hscsprite[5]->flags|=SP_FLAG_VISIBLE;
	hscsprite[5]->type=SP_ETC;
	hscsprite[5]->x=30;
	hscsprite[5]->y=50;
	hscsprite[5]->mover=hsc_show_move;
	
	all_inplace=0;
	back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	if(back==NULL) {
		CHECKPOINT;
		error(ERR_FATAL,"cant create background surface");
	}
		
	newstate(ST_SHOW_HCLIST,HCLISTS_FADEIN,0);
}

void hsc_show_work()
{
	int i;
	HSC_DATA *d;
	static double w;

	if(state.mainstate!=ST_SHOW_HCLIST || state.newstate==1) return;


	SDL_BlitSurface(back,NULL,screen,NULL);

	if(keyboard_keypressed()) newstate(ST_SHOW_HCLIST,HCLISTS_QUIT,0);

	switch(state.substate) {
		case HCLISTS_FADEIN:
			if(all_inplace==6) {
				newstate(ST_SHOW_HCLIST,HCLISTS_WAIT,0);
				w=200;
			}
			break;

		case HCLISTS_WAIT:
			w-=fps_factor;
			if(w<=0) {
				newstate(ST_SHOW_HCLIST,HCLISTS_FADEOUT,0);
				all_inplace=0;
				for(i=0;i<6;i++) {
					d=(HSC_DATA *)hscsprite[i]->data;
					d->arrived=0;
					d->dir=1;
				}
			}
			break;
		
		case HCLISTS_FADEOUT:
			if(all_inplace==6) {
				newstate(ST_SHOW_HCLIST,HCLISTS_QUIT,0);
			}
			break;

		case HCLISTS_QUIT:
			for(i=0;i<6;i++) {
				hscsprite[i]->type=-1;
				SDL_FreeSurface(hscsurface[i]);
			}
			SDL_FreeSurface(back);
			//newstate(ST_MENU,0,1);
			 newstate(ST_INTRO,0,1);
			break;
				
	}
	sprite_work(SP_SHOW_ETC);
	sprite_display(SP_SHOW_ETC);
}

void hsc_show_move(SPRITE *s)
{
	HSC_DATA *d=(HSC_DATA *)s->data;

	if(d->arrived==0) {
		s->x=d->xg+(cos(degtorad(d->ph))*d->amp);
		s->y=d->yg;
		d->ph+=d->phspeed*fps_factor;
		// d->ph+=d->phspeed;
		// d->ph%=360;
		if(d->ph>=360)
			d->ph-=360;
		if(d->dir==0) {
			/* slide-in */
			d->amp-=d->ampspeed;
			if(d->amp<=0.3) {
				s->x=d->xg;
				s->y=d->yg;
				d->arrived=1;
				all_inplace++;
			}
		} else {
			/* slide-out */
			d->amp+=d->ampspeed;
			if((d->amp>=400)&&((s->x<-s->w)||(s->x>WIDTH))) {
				d->arrived=1;
				all_inplace++;
			}
		}
	}
}

/*******************************************************************************
 * Hiscore Entry
 ******************************************************************************/

static LETTER letter[40];
static int sel;
static int wstat;
static SDL_Surface *headline;
static SDL_Surface *plate;

static char *entry;
static int entidx;

void hsc_entry_init()
{
	/*
	int i=0,j;
	unsigned char c='A';
	char tmp_str[100];
	
	tmp_str[1]=0;
	for(c='A';c<='Z';c++) {
		tmp_str[0]=c;
		letter[i].ascii=c;
		letter[i++].l=font_render(tmp_str,FONT02);
	} // 0-25
	for(c='0';c<='9';c++) {
		tmp_str[0]=c;
		letter[i].ascii=c;
		letter[i++].l=font_render(tmp_str,FONT02);
	} // 26-36
	letter[i].ascii='.';
	letter[i++].l=font_render(".",FONT02); //37
	letter[i].ascii='-';
	letter[i++].l=font_render("-",FONT02); //38
	letter[i].ascii=-1;
	letter[i++].l=font_render("DEL",FONT02); //39
	letter[i].ascii=-2;
	letter[i++].l=font_render("OK",FONT02); //40

	for(i=0;i<40;i++) {
		SDL_SetColorKey(letter[i].l,SDL_SRCCOLORKEY,0x00000000);
		letter[i].xpos=(i%10)*25+30;
		letter[i].ypos=(i/10)*25+100;
	}
	*/
	int i/*=0*/,j; 
	char tmp_str[/*128*/32/*100*/]; 
	tmp_str[1]=0; 
	for (i=0; i<(40-2); ) { 
		const char *str_aaa = /* 8*5==40==38(chars)+1(dummy)+1(0) */ 
		{ "ABCDEFG123" "HIJKLMN456" "OPQRSTU789" "VWXYZ.-0" /*.align*/" " }; 
		unsigned char c /*= 'A'*/; 
		c = str_aaa[i]; 
		tmp_str[0] = c; 
		letter[i].ascii = c; letter[i].l = font_render(tmp_str, FONT02); /* */ 
		i++; 
	} 
	letter[i].ascii=-1;
	letter[i++].l=font_render("DEL",FONT02); /* 39 */ 
	letter[i].ascii=-2;
	letter[i++].l=font_render("OK", FONT02); /* 40 */ 
	{
		int k;
		k = 0; 
		for (j=0; j<4; j++) { 
			for (i=0; i<10; i++) { 
				SDL_SetColorKey(letter[k].l,SDL_SRCCOLORKEY,0x00000000); 
				letter[k].xpos = (i)*30/*25*/ + 48/* 30*/; 
				letter[k].ypos = (j)*36/*25*/ + 84/*100*/; 
				k++; 
			} 
		} 
	}

	back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	if(back==NULL) {
		CHECKPOINT;
		error(ERR_FATAL,"cant create background surface");
	}
	sel=-1;
	wstat=-1;

	for(i=0;i<40;i++) {
		letter[i].s=0.0;
	}

	for(i=0;i<5;i++) {
		if(lastscore>hsc_table[i].score)
			break;
	}

	for(j=4;j>i;j--) {
		hsc_table[j]=hsc_table[j-1];
	}

	hsc_table[i].score=lastscore;
	entry=hsc_table[i].name;
	entry[0]=' ';
	entry[1]=' ';
	entry[2]=' ';
	entry[3]=0;
	entidx=0;
	switch(i) {
		case 0:
			sprintf(tmp_str,"NEW HISCORE");
			break;
		default:
			sprintf(tmp_str,"PLACE %d IN THE LIST",i+1);
			break;
	}
	headline=font_render(tmp_str,FONT06);
	plate=loadbmp("plate.png");

	newstate(ST_ENTRY_HCLIST,HCLISTE_ENTRY,0);
}

void hsc_entry_work()
{
	int i;

	if(state.mainstate!=ST_ENTRY_HCLIST || state.newstate==1) return;

	switch(wstat) {
		case -1:
			for(i=0;i<40;i++) {
				letter[i].s+=0.1;
			}
			if(letter[0].s>=3) {
				wstat=-2;
			}
			break;
		case -2:
			for(i=0;i<40;i++) {
				letter[i].s-=0.1;
			}
			if(letter[0].s<=1) {
				wstat=8;
				sel=0;
			}
			break;
		default:
			// wstat++;
			// if(wstat==8) {
			// 	wstat=0;
			if(--wstat==0) {
				wstat=8;
				if(keyboard[KEY_LEFT]) {
					wstat=20;
					sel--;
					if(sel<0)
						sel=39;
				}
				if(keyboard[KEY_RIGHT]) {
					wstat=20;
					sel++;
					if(sel==40)
						sel=0;
				}
				if(keyboard[KEY_UP]) {
					wstat=20;
					sel-=10;
					if(sel<0)
						sel+=40;
				}
				if(keyboard[KEY_DOWN]) {
					wstat=20;
					sel+=10;
					if(sel>39)
						sel-=40;
				}
				
				
				if(keyboard[KEY_CANCEL]) {		//�L�����Z���{�^���̒ǉ�
					wstat=20;
					if(entidx>0) {		//�������͂���Ă��Ȃ��Ƃ��͎��s���Ȃ�
						entidx--;
						entry[entidx]=' ';
					}
				}
				
				
				if(keyboard[KEY_SHOT]) {
					wstat=20;
					switch(letter[sel].ascii) {
						case -1: /* Delete last character */
							if(entidx>0) {
								entidx--;
								entry[entidx]=' ';
							}
							break;
						case -2: /* Eingabe abgeschlossen */
							if(strcmp(entry,"   "))		//�������͂���Ă���Ƃ�(strcmp��=����0��Ԃ�)
							{
								for(i=0;i<40;i++) {
									SDL_FreeSurface(letter[i].l);
								}
								SDL_FreeSurface(back);
								SDL_FreeSurface(headline);
								unloadbmp_by_surface(plate);/*unloadbmp_by_name("plate.png");*/
								newstate(ST_INTRO,0,1);
								return;
							}
							break;
						default:
							if(entidx<3) {
								entry[entidx]=letter[sel].ascii;
								entidx++;
							}
							break;
					}
				}
				
				
				if(keyboard[KEY_PAUSE]) {		//�I���{�^���̒ǉ�
					if(strcmp(entry,"   "))		//�������͂���Ă���Ƃ�(strcmp��=����0��Ԃ�)
					{
						wstat=20;
						for(i=0;i<40;i++) {
							SDL_FreeSurface(letter[i].l);
						}
						SDL_FreeSurface(back);
						SDL_FreeSurface(headline);
						unloadbmp_by_surface(plate);/*unloadbmp_by_name("plate.png");*/
						newstate(ST_INTRO,0,1);
						return;
					}
				}
			}
			break;
	}
	hsc_entry_show();
}
	

void hsc_entry_show()
{
	int i;
	SDL_Rect r,s;
	SDL_Surface *e;
	static double angle=0;
	int xa,ya;

	SDL_BlitSurface(back,NULL,screen,NULL);

	r.x=WIDTH2/2-headline->w/2;		//r.x=screen->w/2-headline->w/2;
	r.y=40;
	r.w=headline->w;
	r.h=headline->h;
	SDL_BlitSurface(headline,NULL,screen,&r);

	angle+=15*fps_factor;
	if(angle>360)
		angle-=360;
	s.x=0;
	s.y=0;
	for(i=0;i<40;i++) {
		if(i!=sel) {
			s.w=letter[i].l->w;
			s.h=letter[i].l->h;
			r.x=letter[i].xpos-(letter[i].l->w*letter[i].s/2);
			r.y=letter[i].ypos-(letter[i].l->h*letter[i].s/2);
			r.w=s.w*letter[i].s;
			r.h=s.h*letter[i].s;
			blit_scaled(letter[i].l,&s,screen,&r);
			if(sel>=0) 
				if(letter[i].s>1) 
					letter[i].s-=0.05;
		} else {
			if(sel>=0)
				if(letter[i].s<=3)
					letter[i].s+=0.2;
		}
	}
	if(sel>=0) {
		xa=cos(degtorad(angle))*10;
		ya=sin(degtorad(angle))*10;
		s.w=letter[sel].l->w;
		s.h=letter[sel].l->h;
		r.x=letter[sel].xpos-(letter[sel].l->w*letter[sel].s/2)+xa;
		r.y=letter[sel].ypos-(letter[sel].l->h*letter[sel].s/2)+ya;
		r.w=s.w*letter[sel].s;
		r.h=s.h*letter[sel].s;
		blit_scaled(letter[sel].l,&s,screen,&r);
	}

	r.x=WIDTH2/2-plate->w/2;		//r.x=screen->w/2-plate->w/2;
	r.y=(HEIGHT-12)-plate->h;		//r.y=260-plate->h;  //denis
	r.w=plate->w;
	r.h=plate->h;
	SDL_BlitSurface(plate,NULL,screen,&r);


	e=font_render(entry,FONT02);
	s.x=0;
	s.y=0;
	s.w=e->w;
	s.h=e->h;
	r.x+=5;
	r.y+=5;
	r.w=e->w*2;
	r.h=e->h*2;
	blit_scaled(e,&s,screen,&r);
	SDL_FreeSurface(e);
}
