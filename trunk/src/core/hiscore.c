#include "hiscore.h"

extern GAMESTATE state;
extern SDL_Surface *screen;
extern double fps_factor;


/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����
---------------------------------------------------------*/

/*global*/

 HSC_LIST hsc_table[5];
 int lastscore;
 int high_score;


/*local static*/

static SPRITE *hscsprite[6];
static SDL_Surface *hscsurface[6];
static int all_inplace;
//static int nr_of_sprites;
//static SDL_Surface *back;

typedef struct
{
	/*double*/int xg256;
	/*double*/int yg256;
	int dir;
	/*double*/int ph512;
	/*double*/int phspeed;
	/*double*/int amp256;
	/*double*/int ampspeed256;
	int arrived;
} HSC_DATA;

static void hsc_show_move(SPRITE *s)
{
	HSC_DATA *d=(HSC_DATA *)s->data;

	if (d->arrived==0)
	{
		s->x = ((d->xg256+(int)(co_s512((d->ph512))*d->amp256))>>8);
		s->y = ((d->yg256)>>8);
		d->ph512 += d->phspeed/**fps_factor*/;
		// d->ph+=d->phspeed;
		// d->ph%=360;
		mask512(d->ph512);//if (d->ph>=360) {	d->ph-=360;}
		if (d->dir==0)
		{
			/* slide-in */
			d->amp256 -= d->ampspeed256;
			if (d->amp256 <= t256(0.3) )
			{
				s->x = ((d->xg256)>>8);
				s->y = ((d->yg256)>>8);
				d->arrived=1;
				all_inplace++;
			}
		}
		else
		{
			/* slide-out */
			d->amp256 += d->ampspeed256;
			if ((d->amp256 >= t256(400) ) &&
				((s->x<-s->w)||(s->x>PSP_WIDTH)))
			{
				d->arrived=1;
				all_inplace++;
			}
		}
	}
}


enum _hclist_show_states
{
	HCLISTS_INIT = 0,
	HCLISTS_FADEIN,
	HCLISTS_WAIT,
	HCLISTS_FADEOUT,
	HCLISTS_QUIT
};

void hsc_show_init(void)
{
	char ttmp[64/*50*/];
	HSC_DATA *hd;
	int i;
	for (i=0;i<5;i++)
	{
		sprintf(ttmp,"%02d %-3s %09d0",i+1,hsc_table[i].name,hsc_table[i].score);
	//	hscsurface[i]			= font_render(ttmp,i==0?FONT05:FONT01);
	//	hscsurface[i]			= font_render(ttmp,i==0?FONT06:FONT07);
		hscsurface[i]			= font_render(ttmp,FONT06);
		hscsprite[i]			= sprite_add(hscsurface[i],1,PR_TEXT,1);
		hd						= mmalloc(sizeof(HSC_DATA));
		hscsprite[i]->data		= hd;
		hd->xg256				= (t256( 4)*i) + t256( 20);
		hd->yg256				= (t256(25)*i) + t256(150);
		hd->arrived 			= 0;
		hd->ph512				= ((i&(2-1))==0)?deg_360_to_512(0):deg_360_to_512(180);
		hd->phspeed 			= 4;
		hd->amp256				= t256(300);
		hd->ampspeed256 		= t256(1);
		hd->dir 				= 0;
		hscsprite[i]->flags 	|= SP_FLAG_VISIBLE;
		hscsprite[i]->type		= SP_ETC;
		hscsprite[i]->x 		= 0;
		hscsprite[i]->y 		= i*25+110;
		hscsprite[i]->mover 	= hsc_show_move;
	}

	hscsurface[5]			= font_render("TOP FIVE FIGHTERS",FONT05);
	hscsprite[5]			= sprite_add(hscsurface[5],1,PR_TEXT,1);
	hd						= mmalloc(sizeof(HSC_DATA));
	hscsprite[5]->data		= hd;
	hd->xg256				= t256(30);
	hd->yg256				= t256(110);
	hd->ph512				= deg_360_to_512(0);
	hd->phspeed 			= 5;
	hd->amp256				= t256(300);
	hd->ampspeed256 		= t256(1.0);
	hd->arrived 			= 0;
	hd->dir 				= 0;
	hscsprite[5]->flags 	|= SP_FLAG_VISIBLE;
	hscsprite[5]->type		= SP_ETC;
	hscsprite[5]->x 		= 30;
	hscsprite[5]->y 		= 50;
	hscsprite[5]->mover 	= hsc_show_move;

	all_inplace=0;
	psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	//if (NULL==back)
	//{
	//	CHECKPOINT;
	//	error(ERR_FATAL,"cant create background surface");
	//}
	newstate(ST_SHOW_HCLIST,HCLISTS_FADEIN,0);
}


void hsc_show_work(void)
{
	if (state.mainstate!=ST_SHOW_HCLIST || state.newstate==1) return;

	if (IS_KEYBOARD_PULLED/*keyboard_keypressed()*/)
	{
		newstate(ST_SHOW_HCLIST,HCLISTS_QUIT,0);
	}
//
	psp_pop_screen();	//SDL_BlitSurface(back,NULL,screen,NULL);
//
	int i;
	HSC_DATA *d;
	static double w;

	switch (state.substate)
	{
	case HCLISTS_FADEIN:
		if (all_inplace==6)
		{
			newstate(ST_SHOW_HCLIST,HCLISTS_WAIT,0);
			w=200;
		}
		break;
	case HCLISTS_WAIT:
		w-=fps_factor;
		if (w<=0)
		{
			newstate(ST_SHOW_HCLIST,HCLISTS_FADEOUT,0);
			all_inplace=0;
			for (i=0;i<6;i++)
			{
				d=(HSC_DATA *)hscsprite[i]->data;
				d->arrived=0;
				d->dir=1;
			}
		}
		break;
	case HCLISTS_FADEOUT:
		if (all_inplace==6)
		{
			newstate(ST_SHOW_HCLIST,HCLISTS_QUIT,0);
		}
		break;
	case HCLISTS_QUIT:
		for (i=0;i<6;i++)
		{
			hscsprite[i]->type=SP_DELETE;
			SDL_FreeSurface(hscsurface[i]);
		}
	//	SDL_FreeSurface(back);
	//	newstate(ST_MENU,0,1);
		newstate(ST_MENU/*ST_INTRO*/,0,1);
		break;
	}
	sprite_work(SP_SHOW_ETC);
	sprite_display(SP_SHOW_ETC);
}



/*---------------------------------------------------------
	�n�C�X�R�A���O���͉��
---------------------------------------------------------*/

static char *entry;
static int wstat;
static int sel;

typedef struct
{
	SDL_Surface *l;
	double s;
	char ascii;
	int xpos,ypos;
} LETTER;

static LETTER letter[40];

static int now_select_name_chr;
static int plate_x;
static int plate_y;

void hsc_entry_init(void)
{
	/*
	int i=0,j;
	unsigned char c='A';
	char tmp_str[100];

	tmp_str[1]=0;
	for (c='A';c<='Z';c++) {
		tmp_str[0]=c;
		letter[i].ascii=c;
		letter[i++].l=font_render(tmp_str,FONT02);
	} // 0-25
	for (c='0';c<='9';c++) {
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

	for (i=0;i<40;i++) {
		SDL_SetColorKey(letter[i].l,SDL_SRCCOLORKEY,0x00000000);
		letter[i].xpos=(i%10)*25+30;
		letter[i].ypos=(i/10)*25+100;
	}
	*/
	int i/*=0*/,j;
	char tmp_str[/*128*/32/*100*/];
	tmp_str[1]=0;
	for (i=0; i<(40-2); )
	{
		const char *str_aaa = /* 8*5==40==38(chars)+1(dummy)+1(0) */
		{ "ABCDEFG123" "HIJKLMN456" "OPQRSTU789" "VWXYZ.-0" /*.align*/" " };
		unsigned char c /*= 'A'*/;
		c = str_aaa[i];
		tmp_str[0] = c;
		letter[i].ascii = c; letter[i].l = font_render(tmp_str, FONT02); /* */
		i++;
	}
	letter[i].ascii=-1; 	letter[i++].l=font_render("DEL",FONT02); /* 39 */
	letter[i].ascii=-2; 	letter[i++].l=font_render("OK", FONT02); /* 40 */
	{
		int k;
		k = 0;
		for (j=0; j<4; j++)
		{
			for (i=0; i<10; i++)
			{
				SDL_SetColorKey(letter[k].l,SDL_SRCCOLORKEY,0x00000000);
				letter[k].xpos = (i)*30/*25*/ + 48/* 30*/;
				letter[k].ypos = (j)*36/*25*/ + 84/*100*/;
				k++;
			}
		}
	}
	sel=-1;
	wstat=-1;
	for (i=0;i<40;i++)
	{
		letter[i].s=0.0;
	}
	for (i=0;i<5;i++)
	{
		if (lastscore>hsc_table[i].score)
		{	break;}
	}
	for (j=4;j>i;j--)
	{
		hsc_table[j]=hsc_table[j-1];
	}
	hsc_table[i].score=lastscore;
	entry=hsc_table[i].name;
	entry[0]=' ';
	entry[1]=' ';
	entry[2]=' ';
	entry[3]=0;
	now_select_name_chr=0;
//	switch (i)
//	{
//	case 0: 	sprintf(tmp_str,"NEW HISCORE"); 				break;
//	default:	sprintf(tmp_str,"PLACE %d IN THE LIST",i+1);	break;
//	}
	char *aaaa[5]=
	{
		"1ST"/*"NEW HISCORE"*/,
		"2ND"/*"PLACE 2 IN THE LIST"*/,
		"3RD"/*"PLACE 3 IN THE LIST"*/,
		"4TH"/*"PLACE 4 IN THE LIST"*/,
		"5TH"/*"PLACE 5 IN THE LIST"*/,
	};
	/* ���ʃ^�C�g���`�� */
	{	/*static*/ SDL_Surface *headline;
		headline=font_render(/*tmp_str*/(&aaaa[i][0]),FONT06);
		SDL_Rect r;
		r.x=GAME_WIDTH/2-headline->w/2;
		r.y=40;
		r.w=headline->w;
		r.h=headline->h;
		SDL_BlitSurface(headline,NULL,screen,&r);
		SDL_FreeSurface(headline);
	}
	/* �v���[�g�`�� */
	{
		/*static*/ SDL_Surface *plate;
		plate=loadbmp("plate.png");
		SDL_Rect r;
		plate_x = r.x=GAME_WIDTH/2-plate->w/2;
		plate_y = r.y=(GAME_HEIGHT-12)-plate->h;
		r.w=plate->w;
		r.h=plate->h;
		SDL_BlitSurface(plate,NULL,screen,&r);
		if (plate)
		{
			unloadbmp_by_surface(plate);/*unloadbmp_by_name("plate.png");*/
		}
	}
	psp_push_screen();	//back=SDL_ConvertSurface(screen,screen->format,screen->flags);
	//if (NULL==back)
	//{
	//	CHECKPOINT;
	//	error(ERR_FATAL,"cant create background surface");
	//}

	newstate(ST_ENTRY_HCLIST,HCLISTE_ENTRY,0);
}

static void hsc_entry_show(void)
{
	int i;
	SDL_Rect s;
	SDL_Rect r;
	SDL_Surface *e;
	static /*double*/int angle512=0;
	int xa,ya;

	psp_pop_screen();	//SDL_BlitSurface(back,NULL,screen,NULL);

	angle512 += deg_360_to_512(15)/**fps_factor*/;
	mask512(angle512);//	if (angle>360)	{	angle-=360;}
	s.x=0;
	s.y=0;
	for (i=0;i<40;i++)
	{
		if (i!=sel)
		{
			s.w=letter[i].l->w;
			s.h=letter[i].l->h;
			r.x=letter[i].xpos-(letter[i].l->w*letter[i].s/2);
			r.y=letter[i].ypos-(letter[i].l->h*letter[i].s/2);
			r.w=s.w*letter[i].s;
			r.h=s.h*letter[i].s;
			blit_scaled(letter[i].l,&s,screen,&r);
			if (sel>=0)
			{	if (letter[i].s>1)
				{	letter[i].s-=0.05;}}
		}
		else
		{
			if (sel>=0)
			{	if (letter[i].s<=3)
				{	letter[i].s+=0.2;}}
		}
	}
	if (sel>=0)
	{
		xa=co_s512((angle512))*10;
		ya=si_n512((angle512))*10;
		s.w=letter[sel].l->w;
		s.h=letter[sel].l->h;
		r.x=letter[sel].xpos-(letter[sel].l->w*letter[sel].s/2)+xa;
		r.y=letter[sel].ypos-(letter[sel].l->h*letter[sel].s/2)+ya;
		r.w=s.w*letter[sel].s;
		r.h=s.h*letter[sel].s;
		blit_scaled(letter[sel].l,&s,screen,&r);
	}

	e=font_render(entry,FONT02);
	s.x=0;
	s.y=0;
	s.w=e->w;
	s.h=e->h;
	r.x= plate_x + 5;
	r.y= plate_y + 5;
	r.w=e->w*2;
	r.h=e->h*2;
	blit_scaled(e,&s,screen,&r);
	SDL_FreeSurface(e);
}

void hsc_entry_work(void)
{
	if (state.mainstate!=ST_ENTRY_HCLIST || state.newstate==1) return;
//
	int i;
	switch (wstat)
	{
	case -1:
		for (i=0;i<40;i++)	{	letter[i].s+=0.1;			}
		if (letter[0].s>=3) {	wstat=-2;					}
		break;
	case -2:
		for (i=0;i<40;i++)	{	letter[i].s-=0.1;			}
		if (letter[0].s<=1) {	wstat=8;	sel=0;			}
		break;
	default:
		//	wstat++;
		//	if (wstat==8) { wstat=0;}
		wstat--;
		if (wstat==0)
		{
			wstat=8;
				 if (my_pad & PSP_KEY_LEFT /*keybo ard[KEY_LEFT]*/) 	{	wstat=20;		sel--;		if (sel<0)		sel=39; 			}
			else if (my_pad & PSP_KEY_RIGHT/*keybo ard[KEY_RIGHT]*/)	{	wstat=20;		sel++;		if (sel==40)	sel=0;				}
			else if (my_pad & PSP_KEY_UP   /*keybo ard[KEY_UP]*/)		{	wstat=20;		sel-=10;	if (sel<0)		sel+=40;			}
			else if (my_pad & PSP_KEY_DOWN /*keybo ard[KEY_DOWN]*/) 	{	wstat=20;		sel+=10;	if (sel>39) 	sel-=40;			}
			//
			if (my_pad & PSP_KEY_SHOT/*keybo ard[KEY_SHOT]*/)
			{
				wstat=20;
				switch (letter[sel].ascii)
				{
				case -1: /* Delete last character */
					goto delete_last_character;
					break;
				case -2: /* Input completed. ���͏I���B Eingabe abgeschlossen. */
					goto agree_entry;
					break;
				default:
					if (now_select_name_chr < 3) /* 3 chrs, name input entry. */
					{
						entry[now_select_name_chr] = letter[sel].ascii;
						now_select_name_chr++;
					}
					else
					{
						sel = 39;/* force set [OK] */
					}
					break;
				}
			}
			else if (my_pad & PSP_KEY_CANCEL/*keybo ard[KEY_CANCEL]*/)		// �L�����Z���{�^���̒ǉ�
			{
				wstat=20;
			delete_last_character:
				if (now_select_name_chr > 0) /* at first chr? */	// �������͂���Ă��Ȃ��Ƃ��͎��s���Ȃ�
				{
					now_select_name_chr--;
					entry[now_select_name_chr]=' ';
				}
			}
			else if (my_pad & PSP_KEY_PAUSE/*keybo ard[KEY_PAUSE]*/)	// �I���{�^���̒ǉ�
			{
				wstat=20;
			agree_entry:
				if (strcmp(entry,"   "))	// �������͂���Ă���Ƃ�(strcmp��=����0��Ԃ�)
				{
					for (i=0; i<40; i++)
					{
						SDL_FreeSurface(letter[i].l);
					}
				//	SDL_FreeSurface(back);
					newstate(ST_MENU/*ST_INTRO*/,0,1);
					return;
				}
			}
		}
		break;
	}
	hsc_entry_show();
}
