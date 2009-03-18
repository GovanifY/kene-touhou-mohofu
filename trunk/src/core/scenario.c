#include "scenario.h"
#include <SDL/SDL_image.h>

extern int keyboard[];
extern double fps_factor;
extern int errno;
extern GAMESTATE state;
extern SDL_Surface *col_sdl;
extern int count_i;

/*************グローバル変数**************/
SDL_Surface *msg_window=NULL;		/* メッセージウィンドウ */
SDL_Surface *window=NULL;			/* ウィンドウ */
SDL_Rect msgw_rect;					/* メッセージウィンドウ表示位置 */
S_SCRIPT *sscript=NULL;				/* 命令保持用 */
int count_char=0;					/* カーソル位置 */
/*****************************************/

static SDL_Surface *tachie_window=NULL;	/* 優先立ち絵ウィンドウ */
static SDL_Surface *bg_story;		/* スクリプトにおける背景 */
static SDL_Surface *fade_out;		/* 演出用 */
static SDL_Surface *filter;		/* 演出用 */
static SC_SPRITE *sc_sp[20];
static SC_SPRITE *tachie_r;
static SC_SPRITE *tachie_l;
static S_SCRIPT *entry_script;		/* 命令収集用 */
static S_SCRIPT *start_script=NULL;	/* 命令実行用 */
static int tick;					/* 時間測定用 */
static int bg_alpha;				/* 背景α値用 */
static int inits;					/* 初期化判定 */
static int cursor=0;				/* count_char保存用 */
static int is_window=0;				/* ウィンドウ表示フラグ */
static int is_tachie_window=0;		/* 立ち絵強調表示フラグ */
static int is_bg=0;					/* 背景表示フラグ */
static int is_filter=0;				/* フィルター表示フラグ */
static int scenario_width;			/* 横幅 */

static void script_reset();

static int cha_search(char *str)
{
	int n;
	if(!strcmp(str,"right"))	{	return -2;	}
	else if(!strcmp(str,"left")){	return -3;	}
	else{
		char buffer[20];
		char *d = buffer;
		while (isdigit(*str)){	*d++ = *str++;	}
		n=atoi(buffer);
		if(0<=n && n<15){	return n;	}
	}
	return -1;
}

static SDL_Surface *load_Surface(char *str,int alpha)
{ 
	char fn[128];
	strcpy(fn,moddir);
	strcat(fn,"/scenario/");
	strcat(fn,str);
	SDL_Surface *tmp;
	SDL_Surface *result;

	tmp=IMG_Load(fn);
	if ( NULL == tmp )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant load image %s: %s",fn,SDL_GetError());
		return NULL;
	}
	if(alpha==-1 || alpha==0){
		result = SDL_DisplayFormat(tmp);
		SDL_SetColorKey(result,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	else{
		result = SDL_DisplayFormatAlpha(tmp);
	}
	if ( NULL == result )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant convert image %s to display format: %s",fn,SDL_GetError());
		return NULL;
	}
	SDL_FreeSurface(tmp);
	tmp = NULL;
	return result;
}

static SDL_Surface *load_local(char *str,SDL_Surface *s,int alpha)
{
	char fn[128];
	strcpy(fn,moddir);
	strcat(fn,"/scenario/");
	strcat(fn,str);
	SDL_Surface *tmp=NULL;

	if(s!=NULL)
	{
		SDL_FreeSurface(s);
		s=NULL;
	}

	tmp=IMG_Load(fn);
	if ( NULL == tmp )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant load image %s: %s",fn,SDL_GetError());
	}
	if(alpha==-1 || alpha==0){
		s = SDL_DisplayFormat(tmp);
		SDL_SetColorKey(s,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	else{
		s = SDL_DisplayFormatAlpha(tmp);
	}
	if ( NULL == s )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant convert image %s to display format: %s",fn,SDL_GetError());
	}
	SDL_FreeSurface(tmp);
	tmp = NULL;
	return s;
}

static void remove_sc_sprite(SC_SPRITE *s)
{
	if(s!=NULL){
		SDL_FreeSurface(s->img);
		free(s);
		s=NULL;
	}
}

static void reload_sc_sprite(char *filename,int no,int frames,int alpha)
{
	SC_SPRITE *sc;
	if(no==-1)		{	return;			}
	else if(no==-2)	{	sc=tachie_r;	}	/* right */
	else if(no==-3)	{	sc=tachie_l;	}	/* left */
	else			{	sc=sc_sp[no];	}	/* 汎用スプライト */
	if(sc==NULL)	{	return;			}

	SDL_FreeSurface(sc->img);

	SDL_Surface *tmp=NULL;
	tmp=load_local(filename,tmp,alpha);
	sc->img=tmp;
	sc->frames=frames;
	sc->w=tmp->w/frames;
	sc->h=tmp->h;
	sc->cw=tmp->w/frames/2;
	sc->ch=tmp->h/2;
}

static SC_SPRITE *load_sc_sprite(char *filename,int sc_sp_num,int frames,int alpha,SDL_Rect *rect)
{
	if(sc_sp[sc_sp_num]!=NULL){	remove_sc_sprite(sc_sp[sc_sp_num]);	}

	SC_SPRITE *tmp;

	tmp=mmalloc(sizeof(SC_SPRITE));
	SDL_Surface *suf=NULL;
	suf=load_Surface(filename,alpha);
	if(suf==NULL){	return NULL;	}
	tmp->img=suf;
	if(frames==-1){	tmp->frames=1;	}
	else{	tmp->frames=frames;	}
	tmp->aktframe=0;
	tmp->anim_speed=0;
	tmp->anim_type=0;
	tmp->anim_time=0;
	tmp->anim_count=0;
	tmp->alpha=255;
	tmp->w=suf->w/frames;
	tmp->h=suf->h;
	tmp->cw=suf->w/frames/2;
	tmp->ch=suf->h/2;
	tmp->flags=0;
	tmp->move_wait=0;
	tmp->x=rect->x;
	tmp->y=rect->y;
	sc_sp[sc_sp_num]=tmp;
	return tmp;
}

static void load_r(char *filename,int frames,int alpha,SDL_Rect *rect)
{
	if(tachie_r!=NULL){	remove_sc_sprite(tachie_r);	}

	SC_SPRITE *tmp;
	SDL_Surface *suf=NULL;
	tmp=mmalloc(sizeof(SC_SPRITE));
	suf=load_local(filename,suf,alpha);
	if(suf==NULL){	return;	}
	tachie_r=tmp;
	tmp->img=suf;
	if(frames==-1){	tmp->frames=1;	}
	else{	tmp->frames=frames;	}
	tmp->aktframe=0;
	tmp->anim_speed=0;
	tmp->anim_type=0;
	tmp->anim_time=0;
	tmp->anim_count=0;
	tmp->alpha=255;
	tmp->w=suf->w/frames;
	tmp->h=suf->h;
	tmp->cw=suf->w/frames/2;
	tmp->ch=suf->h/2;
	tmp->flags=0;
	tmp->move_wait=0;
	tmp->x=rect->x-(rect->w*tmp->w);
	tmp->y=rect->y-tmp->h;
}

static void load_l(char *filename,int frames,int alpha,SDL_Rect *rect)
{\
	if(tachie_l!=NULL){	remove_sc_sprite(tachie_l);	}

	SC_SPRITE *tmp;
	SDL_Surface *suf=NULL;
	tmp=mmalloc(sizeof(SC_SPRITE));
	suf=load_local(filename,suf,alpha);
	if(suf==NULL){	return;	}
	tachie_l=tmp;
	tmp->img=suf;
	if(frames==-1){	tmp->frames=1;	}
	else{	tmp->frames=frames;	}
	tmp->aktframe=0;
	tmp->anim_speed=0;
	tmp->anim_type=0;
	tmp->anim_time=0;
	tmp->anim_count=0;
	tmp->alpha=255;
	tmp->w=suf->w/frames;
	tmp->h=suf->h;
	tmp->cw=suf->w/frames;
	tmp->ch=suf->h/2;
	tmp->flags=0;
	tmp->move_wait=0;
	tmp->x=rect->x-(rect->w*tmp->w);
	tmp->y=rect->y-tmp->h;
}

static int move_sc_sprite(char *l_or_r,int x,int y,int speed)
{
	SC_SPRITE *sc;
	int n=cha_search(l_or_r);
	if(n==-1){	return -1;	}
	else if(n==-2){	sc=tachie_r;	}	/* right */
	else if(n==-3){	sc=tachie_l;	}	/* left */
	else{	sc=sc_sp[n];	}			/* 汎用スプライト */

	if(inits){
		double tmp_d=atan2(y,x);
		int tmp_i;
		if(   -3*M_PI/4<tmp_d && tmp_d<= -M_PI/4){	tmp_i=0;	}	/* ↑ */
		else if(-M_PI/4<tmp_d && tmp_d<=  M_PI/4){	tmp_i=1;	}	/* → */
		else if( M_PI/4<tmp_d && tmp_d<=3*M_PI/4){	tmp_i=2;	}	/* ↓ */
		else{tmp_i=3;}												/* ← */
		sc->r_course = tmp_i;
		sc->angle    = tmp_d;
		sc->move_x   = sc->x + x;		/* 目標座標 */
		sc->move_y   = sc->y + y;
	}
	sc->x+=cos(sc->angle)*speed/10;
	sc->y+=sin(sc->angle)*speed/10;
	switch(sc->r_course)
	{
	case 0:
		if(sc->y > sc->move_y){	sc->y=sc->move_y;	return 1;}	break;
	case 1:
		if(sc->x > sc->move_x){	sc->x=sc->move_x;	return 1;}	break;
	case 2:
		if(sc->y < sc->move_y){	sc->y=sc->move_y;	return 1;}	break;
	case 3:
		if(sc->x < sc->move_x){	sc->x=sc->move_x;	return 1;}	break;
	}
	return 0;
}

static int script_wait(int n)
{
	static double w_tick;
	if(inits)
	{
		w_tick=n;
	}
	w_tick-=fps_factor;
	if(w_tick<0)
	{
		w_tick=0;
		return 1;
	}
	return 0;
}

static void msg_window_init()
{
	if(msg_window!=NULL)	{	SDL_FreeSurface(msg_window);	msg_window=NULL;	}
	msg_window=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* メインメモリへ確保する */
									340,
									100,
									screen->format->BitsPerPixel,
									screen->format->Rmask,
									screen->format->Gmask,
									screen->format->Bmask,
									screen->format->Amask);
	SDL_SetColorKey(msg_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
}

static void tachie_window_init()
{
	if(tachie_window!=NULL)	{	SDL_FreeSurface(tachie_window);	tachie_window=NULL;	}
	tachie_window=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* メインメモリへ確保する */
									scenario_width,
									272,
									screen->format->BitsPerPixel,
									screen->format->Rmask,
									screen->format->Gmask,
									screen->format->Bmask,
									screen->format->Amask);
	SDL_SetColorKey(tachie_window,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
}

static void filter_init(int r,int g,int b)
{
	if(filter!=NULL)	{	SDL_FreeSurface(filter);	filter=NULL;	}
	filter=SDL_CreateRGBSurface(SDL_SRCCOLORKEY|SDL_SWSURFACE,/* メインメモリへ確保する */
									scenario_width,
									272,
									screen->format->BitsPerPixel,
									screen->format->Rmask,
									screen->format->Gmask,
									screen->format->Bmask,
									screen->format->Amask);
	SDL_SetColorKey(filter,SDL_RLEACCEL,0x00000000);
	SDL_FillRect(filter,NULL,SDL_MapRGB(filter->format,r,g,b));
	SDL_SetAlpha(filter,SDL_SRCALPHA,0);
}

static void msgw_rect_init(int x,int y)
{
	msgw_rect.x=x;
	msgw_rect.y=y;
}

static int window_effect(int w_is,int efe)
{
	/* 何故か効果がない原因不明 */
	static int lines;
	static int st_line;
	Sint32 x, y;
	Uint32 col;
	if(efe==-1){	is_window=w_is;	return 1;	}
	else{
		if(inits){
			lines=0;
			is_window=2;
			msg_window_init();
			if(efe<1)
				st_line=1;
			else
				st_line=efe;
		}
		if (SDL_MUSTLOCK(msg_window))	{	SDL_LockSurface(msg_window);	}
		if (SDL_MUSTLOCK(window))		{	SDL_LockSurface(window);		}
		if(w_is){
			for (y = lines; y<lines+st_line; y++) {
				for (x = 0; x<window->w; x++) {
					col=getpixel(window,x,y);
					putpixel(msg_window,x,y, col);
				}
			}
		}
		else{
			msg_window_init();
			for (y = 0; y<window->h-(lines); y++) {
				for (x = 0; x<window->w; x++) {
					col=getpixel(window,x,y);
					putpixel(msg_window,x,y, col);
				}
			}
		}
		if (SDL_MUSTLOCK(msg_window))	{	SDL_UnlockSurface(msg_window);	}
		if (SDL_MUSTLOCK(window))		{	SDL_UnlockSurface(window);		}
		if(lines>window->h){	msg_window_init();	is_window=w_is;	return 1;	}
		lines+=st_line;
	}
	return 0;
}

/***************************************************************
****		シ		ス		テ		ム		処		理		****
***************************************************************/

static SC_SPRITE *sc_sprite_work(SC_SPRITE *sp)
{
	if(sp==NULL){	return NULL;	}
	if(sp->frames==1){	return sp;	}
	/*
	/// anim_typeについて ///
	0:	左から右に。最後までいったら左へ戻る。
	1:	右から左に。〃
	2:	左から右に。繰り返し無し。
	3:	右から左に。繰り返し無し。
	4:	左から右に。最後までいったら右から左に。
	*/
	sp->anim_count+=fps_factor;
	if(sp->anim_speed<sp->anim_count)
	{
		sp->anim_count=0;
		switch(sp->anim_type)
		{
		case 0:
			sp->aktframe++;
			if(sp->aktframe>=sp->frames){	sp->aktframe=0;	}
			break;
		case 1:
			sp->aktframe--;
			if(sp->aktframe<0)			{	sp->aktframe=sp->frames-1;	}
			break;
		case 2:
			sp->aktframe++;
			if(sp->aktframe>=sp->frames){	sp->aktframe=sp->frames-1;	sp->anim_type=6;	}
			break;
		case 3:
			sp->aktframe--;
			if(sp->aktframe<0)			{	sp->aktframe=0;	sp->anim_type=6;	}
			break;
		case 4:
			sp->aktframe++;
			if(sp->aktframe>=sp->frames){	sp->aktframe-=2;	sp->anim_type=5;	}
			break;
		case 5:
			sp->aktframe--;
			if(sp->aktframe<0)			{	sp->aktframe+=2;	sp->anim_type=4;	}
			break;
		case 6:
			break;
		}
	}
	return sp;
}

static void disprite(int start,int end)
{
	int sp_i;

	for(sp_i=start;sp_i<=end;sp_i++){
		if(sc_sp[sp_i]!=NULL){
			if(sc_sp[sp_i]->flags){
				SDL_Rect src_r,dst_r;
				src_r.x=sc_sp[sp_i]->w*sc_sp[sp_i]->aktframe;
				src_r.y=0;
				src_r.w=sc_sp[sp_i]->w;
				src_r.h=sc_sp[sp_i]->h;
				dst_r.x=sc_sp[sp_i]->x;
				dst_r.y=sc_sp[sp_i]->y;
				SDL_BlitSurface(sc_sp[sp_i]->img, &src_r, screen, &dst_r);
			}
			sc_sp[sp_i]=sc_sprite_work(sc_sp[sp_i]);
		}
	}
}

static void load_script_free(void)
{
	S_SCRIPT *l = sscript;
	S_SCRIPT *n;
	while (l!=NULL)
	{
		n=l->next;
		if(l->data!=NULL){	free(l->data);	l->data=NULL;	}
		free(l);
		l = n;
	}
	sscript = (S_SCRIPT *)NULL;
	entry_script = (S_SCRIPT *)NULL;
}

static char *load_command(char *c, char *buffer, int *end)
{
	int i=0;
	while (*c != ' ' && *c != 13)
	{
		i++;
		if (i >= 10)
		{	return (char *) NULL;}
		*buffer++ = *c++;
	}
	if(*c == 13)	/* 改行コードは OD OA */
	{
		*end=1;
	}
	*buffer = 0;		//NULL
	return c;
}

static char *load_str(char *c, char *buffer, int *end)
{
	int i=0;
	int e=0;		/* エスケープ */
	while (((*c == ',' && e==1)||*c != ',') && *c != 13)
	{
		if(*c == '\\' && !e && *(c+1) == ',')	{	e=1;	c++;	}
		if(e)			{	e=0;	}
		i++;
		if (i >= 200)	{	return (char *)NULL;}
		*buffer++ = *c++;
	}
	if(*c == 13)
	{
		*end=1;
	}
	*buffer = 0;		//NULL
	return c;
}

static char *load_int(char *c, int *nr, int *end)
{
	char buffer[20];
	char *d = buffer;
	int i=0;
	while (isdigit(*c)||*c=='-') /* ',' または '\n' が来たら終了 */
	{
		i++;
		if (i >= 20)
		{	return (char *) NULL;}
		*d++ = *c++;
	}
	if(*c == 13){	/* 改行コードは OD OA */
		*end=1;
	}
	*d = 0;
	if((*c==',' || *c==13) && *(c-1)==','){	*nr=-1;	}
	else{	*nr = atoi(buffer);	}
	return c;
}

static void regist_script(char *command,char *c_p0,int c_p1,int c_p2,int c_p3,int c_p4,int c_p5,int c_p6,int c_p7,int chain)
{
/*

	基本形
	command para0,para1,para2,para3

typedef struct _scenario_script
{
	int command;
/	int done;		//終わったかどうか
/	int chain;		//同時実行かどうか
					//0=違うよ	1=1つ目	2=2つ目
/	char para0[200];
/	int para1;
/	int para2;
	int para3;
	void *data;
/	struct _scenario_script *next;
} S_SCRIPT;
*/
	S_SCRIPT *new_script;

	new_script	=	mmalloc(sizeof(S_SCRIPT));

	strcpy(new_script->para0, c_p0);
	new_script->para1	= c_p1;	/* デフォルト */
	new_script->done 	= 0;
	new_script->chain	= chain;
	new_script->next	= NULL;
	new_script->data	= NULL;

	switch(*command)
	{
	case 'B':
		if		(!strcmp(command, "BGTEXT"))	{		/* 背景にテキストを打ち込む */
			SDL_Rect *new_rect0;
			new_script->command=SCP_BGTEXT;
			new_rect0 = mmalloc(sizeof(SDL_Rect));
			new_rect0->x = c_p2;
			new_rect0->y = c_p3;
			new_rect0->w = c_p4;
			new_rect0->h = 1500;
			new_script->data=new_rect0;
			new_script->para2 = c_p5;		/* 書き込み速度 */
		}
		break;
	case 'C':
		if		(!strcmp(command, "CLCURSOR"))	{	new_script->command=SCP_CLCURSOR;	}	/* カーソルの初期化 */
		else if	(!strcmp(command, "CLICK"))		{	new_script->command=SCP_CLICK;	}		/* クリック待ち */
		break;
	case 'F':
		if		(!strcmp(command, "FILTER"))	{
			new_script->command=SCP_FILTER;
			new_script->para2 = c_p2;
			new_script->para3 = c_p3;
		}
		break;
	case 'J':
		if		(!strcmp(command, "JUMP"))		{	new_script->command=SCP_JUMP;	}		/* pauseボタンを押した時のjump先。 */
	case 'L':
		if		(!strcmp(command, "LOADBG"))	{	new_script->command=SCP_LOADBG;	}		/* 背景ファイルの変更 */
		else if	(!strcmp(command, "LOADCUR"))	{	new_script->command=SCP_LOADCUR;	}	/* 記憶したカーソル位置の呼び出し */
		else if	(!strcmp(command, "LOADL"))		{
			SDL_Rect *new_rect3;
			new_script->command=SCP_LOADL;
			new_rect3 = mmalloc(sizeof(SDL_Rect));
			if(c_p1==-1 && c_p2==-1){
				new_rect3->x = 0;			/* x */
				new_rect3->y = 272;			/* y */
				new_rect3->w = 1;			/* w */
			}
			else{
				new_rect3->x = c_p1;		/* x */
				new_rect3->y = c_p2;		/* y */
				new_rect3->w = 1;			/* w */
			}
			new_rect3->h = 0;			/* h */
			new_script->data=new_rect3;
			new_script->para1=c_p3;		/* flame */
			new_script->para2=c_p4;		/* alpha */
		}
		else if	(!strcmp(command, "LOADR"))		{
			SDL_Rect *new_rect4;
			new_script->command=SCP_LOADR;
			new_rect4 = mmalloc(sizeof(SDL_Rect));
			if(c_p1==-1 && c_p2==-1){
				new_rect4->x = scenario_width;	/* x */
				new_rect4->y = 272;				/* y */
			}
			else{
				new_rect4->x = c_p1;			/* x */
				new_rect4->y = c_p2;			/* y */
			}
			new_rect4->w = 0;			/* w */
			new_rect4->h = 0;			/* h */
			new_script->data=new_rect4;
			new_script->para1=c_p3;		/* flame */
			new_script->para2=c_p4;		/* alpha */
		}
		else if	(!strcmp(command, "LOADSP"))	{											/* スプライトのロード */
			SDL_Rect *new_rect1;
			new_script->command=SCP_LOADSP;
			new_rect1 = mmalloc(sizeof(SDL_Rect));
			new_rect1->x = c_p2;		/* x */
			new_rect1->y = c_p3;		/* y */
			new_rect1->w = 0;			/* w */
			new_rect1->h = 0;			/* h */
			new_script->data=new_rect1;
			new_script->para2=c_p4;		/* flame */
			new_script->para3=c_p5;		/* alpha */
		}
		break;
	case 'M':
		if		(!strcmp(command, "MOVESP"))	{											/* スプライトの移動 */
			new_script->command=SCP_MOVESP;	/* para1=x */
			new_script->para2=c_p2;        	/* y */
			if(c_p3<1)                     	/* speed */
				c_p3=10;
			new_script->para3=c_p3;
		}
		break;
	case 'P':
		if		(!strcmp(command, "PARAMSP"))	{
			new_script->command=SCP_PARAMSP;	/* para1=α値 */
			new_script->para2=c_p2;				/* anim_speed */
			new_script->para3=c_p3;				/* anim_type */
		}
		break;
	case 'R':
		if		(!strcmp(command, "RELOADSP"))	{											/* スプライトの再読み込み */
			new_script->command=SCP_RELOADSP;	/* para1=no */
			new_script->para2=c_p2;				/* flames */
			new_script->para3=c_p3;				/* alpha */
		}
		break;
	case 'S':
		if		(!strcmp(command, "SAVECUR"))	{	new_script->command=SCP_SAVECUR;	}	/* カーソル位置の記憶 */
		else if	(!strcmp(command, "SUBG"))		{	new_script->command=SCP_SUBG;		}	/* 背景の表示/非表示 */
		else if	(!strcmp(command, "SUFILTER"))	{	new_script->command=SCP_SUFILTER;	}	/* フィルター表示/非表示&透過度 */
		else if	(!strcmp(command, "SUL"))		{	new_script->command=SCP_SUL;		}	/* 立ち絵左の表示/非表示 */
		else if	(!strcmp(command, "SUR"))		{	new_script->command=SCP_SUR;		}	/* 立ち絵右の表示/非表示 */
		else if	(!strcmp(command, "SUSPRITE"))	{	new_script->command=SCP_SUSPRITE;	new_script->para2=c_p2;	}/*スプライトの表示*/
		else if	(!strcmp(command, "SUTWINDOW"))	{	new_script->command=SCP_SUTWINDOW;	}	/* 強調立ち絵ウィンドウの表示/非表示 */
		else if	(!strcmp(command, "SUWINDOW"))	{	new_script->command=SCP_SUWINDOW;	new_script->para2=c_p2;	}	/* メッセージウィンドウの表示/非表示 para2はbpp速度。*/
		break;
	case 'T':
		if		(!strcmp(command, "TEXT"))		{											/* メッセージウィンドウへの書き込み */
			SDL_Rect *new_rect2;
			new_script->command=SCP_TEXT;
			new_rect2 = mmalloc(sizeof(SDL_Rect));
			new_rect2->x = 10;
			new_rect2->y = 10;
			new_rect2->w = 310;
			new_rect2->h = 1500;
			new_script->data=new_rect2;
			new_script->para2 = c_p2;		/* 書き込み速度 */
			new_script->para3 = c_p3;		/* 書き込み後のcount_charの処遇 */
		}
		else if	(!strcmp(command, "TWINDOW"))	{	new_script->command=SCP_TWINDOW;		}/* 立ち絵強調ウィンドウの初期化 */
		break;
	case 'W':
		if		(!strcmp(command, "WAIT"))		{	new_script->command=SCP_WAIT;	}		/* wait */
		break;

	default:
		pspDebugScreenPrintf("unknown command :%s",command);
		sceKernelDelayThread(3000000);
		break;
	}

	if(entry_script==NULL){	entry_script=new_script;		}
	else
	{
		entry_script->next=new_script;
		entry_script=entry_script->next;
	}

	if (sscript==NULL){	sscript=entry_script;	}
}

static int load_scenario(char *src_filename)
{
	load_script_free();
/*
	基本形
	command para0,para1,para2,para3

typedef struct _scenario_script
{
	int command;
	int done;		//終わったかどうか
	int chain;		//同時実行かどうか
					//0=違うよ	1=1つ目	2=2つ目
	char para0[200];
	int para1;
	int para2;
	int para3;
	void *data;
	struct _scenario_script *next;
} S_SCRIPT;
*/
	char filename[128];
	sprintf(filename,"%s/text/%s.txt", moddir, src_filename);
	FILE *file;
	if (NULL == (file = fopen(filename,"r")))		/* 開けなかったとき */
	{
		return 0;
	}

	int entrys		= 0;		/* 命令がが書き込まれているかどうか。 */
	int line_num	= 0;		/* 行番号 */
	int chains		= 0;
	char buffer_text_1_line[255];	/*parth text, 1 line buffer */		/* 走査する行の取得 */
	while (fgets(buffer_text_1_line,255,file) != NULL)
	{
		/****************** script_data 用 ******************/
		char char_command[15];		/* 基本コマンド */
		char c_p0[200];
		int c_p1=-1;
		int c_p2=-1;
		int c_p3=-1;
		int c_p4=-1;
		int c_p5=-1;
		int c_p6=-1;
		int c_p7=-1;

		/****************** script_search 用 ****************/
		char *c=NULL;					/* 走査中の行の分析用 */
		int end_arg=0;				/* 引数の取得の中止 */
		c = buffer_text_1_line;		/* 先頭アドレスを渡すよ */
		

		line_num++;

		/* skiped lines. */
		if (*c=='\n')		{	continue;	}
		while (isspace(*c)) {	c++;		}
		if (*c=='#')		{	continue;	}
		if (*c=='-')		{	chains++;	c++;	}		/* 連続した命令 */
		else				{	chains=0;	}

		if ((c = load_command(c, char_command, &end_arg))==NULL){	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		if(!end_arg){
			if (*c++ != ' ')									{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_str(c, c_p0, &end_arg))==NULL)		{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')									{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p1, &end_arg))==NULL)		{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')								{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p2, &end_arg))==NULL)	{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')								{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p3, &end_arg))==NULL)	{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')								{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p4, &end_arg))==NULL)	{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')								{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p5, &end_arg))==NULL)	{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')								{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p6, &end_arg))==NULL)	{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		if(!end_arg){
			if (*c++ != ',')								{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
			if ((c = load_int(c, &c_p7, &end_arg))==NULL)	{	error(ERR_WARN,"syntax error in scriptfile '%s', line no: %d", filename, line_num);	continue;	}
		}
		regist_script(char_command,c_p0,c_p1,c_p2,c_p3,c_p4,c_p5,c_p6,c_p7,chains);
		entrys++;
	}
	fclose(file);
	//return(entrys);
	if (!entrys)
	{
		pspDebugScreenPrintf("can't entrys from this file %s",filename);
		sceKernelDelayThread(3000000);
		error(ERR_WARN,"can't entrys from this file %s",filename);
		return 0;
	}
	return 1;
}

int thescript()
{
/*
****やりたいこと****
	1フレームでは終わらない処理があったときのためにこのコマンドからの命令には全て
	終わったことを知らせる引数を付けておくこと。=>doneに代入で終了。
	常にchainを確認し、0以外の値が入っていたら次の関数も実行する(nextを辿る)。
*/

	S_SCRIPT *ssc=start_script;
	int n=0;					/* 次の命令に進めるかの判定 */
	static int tmp_all[15];		/* ご自由にお使いください。 */
	SC_SPRITE *sc_tmp=NULL;		/* 上に同じ */
	static double button_d;		/* button wait */

	if(inits)
	{
		int i;
		for(i=0;i<15;i++)
		{
			tmp_all[i]=0;
		}
	}

	if(0>button_d || button_d>20)	{	button_d=0;	}
	else if(button_d==0)			{	;	}
	else							{	button_d-=fps_factor;	}

	if(keyboard[KEY_PAUSE] && button_d==0)
	{
		while(ssc->next!=NULL && ssc->command!=SCP_JUMP)
		{
			ssc->done=1;
			ssc=ssc->next;
		}
		if(ssc->next==NULL)	{	script_reset();	return 1;	}
		button_d=10;
	}

	while(1)
	{
		if(ssc->done==0){
			switch(ssc->command)
			{
			case SCP_BGTEXT:
				if(!ssc->para2 || (keyboard[KEY_SHOT] && button_d==0))
				{
					mh_print(bg_story,ssc->data,ssc->para1,ssc->para0);
					ssc->done=1;
					button_d=10;
				}
				else
				{	ssc->done=mh_print_fps(bg_story,ssc->para1,ssc->data,ssc->para0,ssc->para2);	}
				break;
			case SCP_CLICK:
				if(keyboard[KEY_SHOT] && button_d==0)	{	ssc->done=1;	button_d=10;	}
				break;
			case SCP_CLCURSOR:
				count_char=0;	ssc->done=1;
				break;
			case SCP_FILTER:
				filter_init(ssc->para1,ssc->para2,ssc->para3);
				ssc->done=1;
				break;
			case SCP_JUMP:
				ssc->done=1;
				break;
			case SCP_LOADBG:
				bg_story=load_local(ssc->para0,bg_story,ssc->para1);
				SDL_SetColorKey(bg_story,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
				ssc->done=1;
				break;
			case SCP_LOADCUR:
				ssc->done=1;	count_char=cursor;
				break;
			case SCP_LOADL:
				load_l(ssc->para0,ssc->para1,ssc->para2,ssc->data);
				ssc->done=1;
				break;
			case SCP_LOADR:
				load_r(ssc->para0,ssc->para1,ssc->para2,ssc->data);
				ssc->done=1;
				break;
			case SCP_LOADSP:
				load_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3,ssc->data);
				ssc->done=1;
				break;
			case SCP_MOVESP:
				ssc->done=move_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				if(ssc->done==-1){
					pspDebugScreenPrintf("no sprite in No.%s\n",ssc->para0);
					sceKernelDelayThread(2000000);
					return 1;
				}
				break;
			case SCP_PARAMSP:
				tmp_all[ssc->chain]=cha_search(ssc->para0);
				if(tmp_all[ssc->chain]==-1)	{	return -1;	}
				else if(tmp_all[ssc->chain]==-2){	sc_tmp=tachie_r;	}
				else if(tmp_all[ssc->chain]==-3){	sc_tmp=tachie_l;	}
				else{				sc_tmp=sc_sp[tmp_all[ssc->chain]];	}
				if(ssc->para1!=-1){
					sc_tmp->alpha=ssc->para1;
					SDL_SetAlpha(sc_tmp->img, SDL_SRCALPHA, sc_tmp->alpha);
				}
				if(ssc->para2!=-1){
					sc_tmp->anim_speed=ssc->para2;
				}
				if(ssc->para3>4)		{	sc_tmp->anim_type=0;			}
				else if(ssc->para3==3)	{	sc_tmp->anim_type=3;	sc_tmp->aktframe=sc_tmp->frames-1;	}
				else if(ssc->para3!=-1)	{	sc_tmp->anim_type=ssc->para3;	}
				ssc->done=1;
				break;
			case SCP_RELOADSP:
				reload_sc_sprite(ssc->para0,ssc->para1,ssc->para2,ssc->para3);
				ssc->done=1;
				break;
			case SCP_SAVECUR:
				ssc->done=1;	cursor=count_char;
				break;
			case SCP_SUBG:
				is_bg=ssc->para1;	ssc->done=1;
				break;
			case SCP_SUFILTER:
				is_filter=ssc->para1;
				SDL_SetAlpha(filter,SDL_SRCALPHA,ssc->para2);
				ssc->done=1;
				break;
			case SCP_SUL:
				tachie_l->flags=ssc->para1;	ssc->done=1;
				break;
			case SCP_SUR:
				tachie_r->flags=ssc->para1;	ssc->done=1;
				break;
			case SCP_SUSPRITE:
				if(ssc->para1==-2)		{	tachie_r->flags=ssc->para2;	}
				else if(ssc->para1==-3)	{	tachie_l->flags=ssc->para2;	}
				else					{	sc_sp[ssc->para1]->flags=ssc->para2;	}
				ssc->done=1;
				break;
			case SCP_SUTWINDOW:
				is_tachie_window=ssc->para1;	ssc->done=1;
				break;
			case SCP_SUWINDOW:		/* うまくいってない */
				ssc->done=window_effect(ssc->para1,ssc->para2);
				break;
			case SCP_TEXT:
				if(!tmp_all[ssc->chain])
				{
					if(ssc->para2==0 || (keyboard[KEY_SHOT] && button_d==0))
					{
						mh_print(msg_window,ssc->data,ssc->para1,ssc->para0);
						tmp_all[ssc->chain]=1;
						button_d=10;
					}
					else
					{	tmp_all[ssc->chain]=mh_print_fps(msg_window,ssc->para1,ssc->data,ssc->para0,ssc->para2);	}
				}
				else
				{
					switch(ssc->para3)
					{
					case 0:		/* 何もせずに次の命令へ */
						ssc->done=1;	
						tmp_all[ssc->chain]=0;	
						break;
					case 1:		/* ボタンを押したら次の命令へ、カーソル継続 */
						if(keyboard[KEY_SHOT] && button_d==0){	
							ssc->done=1;	
							tmp_all[ssc->chain]=0;	
							button_d=10;	
						}	
						break;
					case 2:		/* カーソル初期化 */
						ssc->done=1;	
						tmp_all[ssc->chain]=0;	
						count_char=0;	
						break;
					case 3:		/* ボタンを押したら次の命令へ、カーソル初期化 */
						if(keyboard[KEY_SHOT] && button_d==0){	
							ssc->done=1;	
							tmp_all[ssc->chain]=0;	
							count_char=0;	
							button_d=10;	
						}	
						break;
					case 4:		/* 何もせずに次の命令へ、ウィンドウ初期化*/
						ssc->done=1;	
						tmp_all[ssc->chain]=0;	
						msg_window_init();	
						break;
					case 5:		/* ボタンを押したら次の命令へ、カーソル継続、ウィンドウ初期化 */
						if(keyboard[KEY_SHOT] && button_d==0){	
							ssc->done=1;	
							tmp_all[ssc->chain]=0;	
							msg_window_init();	
							button_d=10;	
						}	
						break;
					case 6:		/* カーソル初期化、ウィンドウ初期化 */
						ssc->done=1;	
						tmp_all[ssc->chain]=0;	
						count_char=0;	
						msg_window_init();	
						break;
					case 7:		/* ボタンを押したら次の命令へ、カーソル初期化、ウィンドウ初期化 */
						if(keyboard[KEY_SHOT] && button_d==0){	
							ssc->done=1;	
							tmp_all[ssc->chain]=0;	
							count_char=0;	
							msg_window_init();	
							button_d=10;	
						}	
						break;
					}
				}
				break;
			case SCP_TWINDOW:
				tachie_window_init();	ssc->done=1;
				break;
			case SCP_WAIT:
				ssc->done=script_wait(ssc->para1);
				break;
			}
		}
		if(ssc->done==0)	{	n++;	}		/* 継続フラグ */
		if(ssc->next==NULL)	{	break;	}		/* nextがNULLの場合 */
		if(ssc->chain>=ssc->next->chain || ssc->chain==0){	break;	}	/* 現chainが次chainより大きい場合 */
		ssc=ssc->next;
	}
	if(is_bg)
	{
		SDL_BlitSurface(bg_story, NULL, screen, NULL);
	}

	disprite(0,9);

	if(tachie_l!=NULL){
		if(tachie_l->flags)
		{
			SDL_Rect src_r,dst_r;
			src_r.x=tachie_l->w*tachie_l->aktframe;
			src_r.y=0;
			src_r.w=tachie_l->w;
			src_r.h=tachie_l->h;
			dst_r.x=tachie_l->x;
			dst_r.y=tachie_l->y;
			if(tachie_l->flags==1)		{	SDL_BlitSurface(tachie_l->img, &src_r, screen, &dst_r);	}
			else if(tachie_l->flags==2)	{	SDL_BlitSurface(tachie_l->img, &src_r, tachie_window, &dst_r);	}
		}
		tachie_l=sc_sprite_work(tachie_l);
	}

	disprite(10,11);

	if(tachie_r!=NULL){
		if(tachie_r->flags)
		{
			SDL_Rect src_r,dst_r;
			src_r.x=tachie_r->w*tachie_r->aktframe;
			src_r.y=0;
			src_r.w=tachie_r->w;
			src_r.h=tachie_r->h;
			dst_r.x=tachie_r->x;
			dst_r.y=tachie_r->y;
			if(tachie_r->flags==1)		{	SDL_BlitSurface(tachie_r->img, &src_r, screen, &dst_r);	}
			else if(tachie_r->flags==2)	{	SDL_BlitSurface(tachie_r->img, &src_r, tachie_window, &dst_r);	}
		}
		tachie_r=sc_sprite_work(tachie_r);
	}

	disprite(12,15);

	if(is_filter){
		SDL_BlitSurface(filter, NULL, screen, NULL);
	}

	disprite(16,16);

	if(is_tachie_window)
	{
		SDL_BlitSurface(tachie_window, NULL, screen, NULL);
	}

	disprite(17,17);

	if(is_window)
	{
		if(is_window!=2){	SDL_BlitSurface(window, NULL, screen, &msgw_rect);	}
		SDL_BlitSurface(msg_window, NULL, screen, &msgw_rect);
	}

	disprite(18,19);

	inits=0;
	if(n==0)					/* 次の命令の許可 */
	{
		inits=1;
		while(start_script->done)		/* 次の命令へ */
		{
			if(start_script->next==NULL){	script_reset();	return 1;	}
			start_script=start_script->next;
		}
	}
	return 0;
}

static void script_reset()
{
	int i;
	for(i=0;i<20;i++)
	{
		remove_sc_sprite(sc_sp[i]);
	}

	remove_sc_sprite(tachie_r);
	remove_sc_sprite(tachie_l);
	if(bg_story!=NULL)		{	SDL_FreeSurface(bg_story);		bg_story=NULL;	}
	if(msg_window!=NULL)	{	SDL_FreeSurface(msg_window);	msg_window=NULL;	}
	if(tachie_window!=NULL)	{	SDL_FreeSurface(tachie_window);	tachie_window=NULL;	}
	if(filter!=NULL)		{	SDL_FreeSurface(filter);		filter=NULL;	}
	if(col_sdl!=NULL)		{	SDL_FreeSurface(col_sdl);		col_sdl=NULL;	}
	load_script_free();

	cursor=0;
	count_char=0;
	count_i=0;
}

int script_init(char *filename, char *bg_name,int width)		/* シナリオファイル名と背景ファイル名 */
{
	scenario_width=width;
	if(bg_name!=NULL){
		bg_story=load_local(bg_name,bg_story,0);
		SDL_SetColorKey(bg_story,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	}
	if(!load_scenario(filename)){
		return 0;
	}
	window=loadbmp2("window.png");
	msg_window_init();
	msgw_rect_init(20,182);
	tachie_window_init();
	tachie_r=NULL;
	tachie_l=NULL;
	inits=1;
	is_bg=0;
	is_window=0;
	is_tachie_window=0;
	is_filter=0;
	count_char=0;
	cursor=0;
	int i;
	for(i=0;i<20;i++)
	{
		sc_sp[i]=NULL;
	}
	start_script=sscript;
	return 1;
}

void story_init()
{
	if (fade_out==NULL)
	{
		fade_out=loadbmp2("fade_black.png");
	}
	bg_alpha=0;
	newstate(ST_STORY,STORY_FADEOUT1,0);
	keyboard_clear();
	tick=0;
}

void story_work()
{
/*
	enum _story_state
	{
		STORY_FADEOUT1,
		STORY_LOAD,
		STORY_FADEIN,
		STORY_WORKS,
		STORY_FADEOUT2,
		STORY_QUIT
	};
*/
	switch(state.substate)
	{
	case STORY_FADEOUT1:
		if(tick>30)	{	newstate(ST_STORY,STORY_LOAD,0);	unloadbmp_by_surface(fade_out);	}
		else{
			SDL_BlitSurface(fade_out, NULL, screen, NULL);
			tick++;
		}
		break;
	case STORY_LOAD:
		script_init("story","story_bg.jpg",480);
		bg_alpha=0;
		newstate(ST_STORY,STORY_FADEIN,0);
		break;
	case STORY_FADEIN:
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		SDL_SetAlpha(bg_story, SDL_SRCALPHA, bg_alpha);
		SDL_BlitSurface(bg_story, NULL, screen, NULL);
		if(bg_alpha<255)
		{
			bg_alpha+=5;
		}
		else
		{
			bg_alpha=255;
			unloadbmp_by_surface(bg_story);
			newstate(ST_STORY,STORY_WORKS,0);
		}
		break;
	case STORY_WORKS:
		SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
		if(thescript()){
			newstate(ST_STORY,STORY_QUIT,0);
		}
		break;
	case STORY_FADEOUT2:
		
		break;
	case STORY_QUIT:
		unloadbmp_by_surface(bg_story);
		bg_alpha=0;
		inits=1;
		newstate(ST_INTRO,1,1);
		break;
	}
}