
/*---------------------------------------------------------
	ヘッダ
---------------------------------------------------------*/

#include "support.h"

#ifdef ENABLE_PSP
	//# /* カスタムライブラリを使う */
	#include "SDL_image.h"
#else
	//# /* 標準ライブラリを使う */
	#include <SDL/SDL_image.h>
#endif


/*---------------------------------------------------------
	定数
---------------------------------------------------------*/


/*---------------------------------------------------------
	外部グローバル変数
---------------------------------------------------------*/
extern GAMESTATE state;
extern GAMESTATE laststate;
extern int difficulty;

/*---------------------------------------------------------
	グローバル変数
---------------------------------------------------------*/
/*extern*/SDL_Surface *screen;
SDL_Surface *back_screen;

//int debug=0;
//int use_joystick=1;

KEYCONFIG keyconfig;

/*---------------------------------------------------------
	math.h
---------------------------------------------------------*/

/* この配列もCPU内蔵の命令キャッシュに乗るよ */
/*extern*/ double sin_tbl512[SINTABLE_SIZE];

/* この関数はただの初期化。命令キャッシュに乗らないよ */
void init_math(void)
{
	{
		int i;
		for(i=0; i<SINTABLE_SIZE; i++)
		{
			sin_tbl512[i]=(sin( (i*(M_PI*2) /SINTABLE_SIZE) )/**65536.0*/ );
		}
	}
}
/* この関数はCPU内蔵の命令キャッシュに乗るよ */
int atan_512( int y, int x )
{
	return (rad2deg512(atan2(y,x)));
}
/*
psp では、 atan2(), sin(), sqrt() 等の超越関数系命令は、
psp の MIPS CPU 内 のコプロセッサが処理をする。
コプロセッサ変換処理があちこちにあると、非常にパフォーマンスが悪いので、
一ヶ所に纏めた方が実行速度は遥かに速い。
(CPU内蔵の命令キャッシュに乗るために実行速度が速くなる)
*/


/*---------------------------------------------------------
	キー入力関連の処理
---------------------------------------------------------*/
static int keyboard[16/*15*/];
void keyboard_clear()
{
	int i;
	for (i=0;i<15;i++)
	{
		keyboard[i]=0;
	}
}

/*global*/Uint32 my_pad;		/*今回入力*/
/*global*/Uint32 my_pad_alter;	/*前回入力*/
void keyboard_poll(void)
{
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);
	int pad_data = pad.Buttons;
	if (pad.Lx < 64/*70*/)			{		pad_data |= PSP_CTRL_LEFT;	}
	else if (pad.Lx > 192/*185*/)	{		pad_data |= PSP_CTRL_RIGHT; }

	if (pad.Ly < 64/*70*/)			{		pad_data |= PSP_CTRL_UP;	}
	else if (pad.Ly > 192/*185*/)	{		pad_data |= PSP_CTRL_DOWN;	}
//
	if (pad_data & PSP_CTRL_SELECT) 	{keyboard[keyconfig.sl] |= (pad_data & PSP_CTRL_SELECT);}	else{keyboard[keyconfig.sl] &= (~PSP_CTRL_SELECT);}
	if (pad_data & PSP_CTRL_START)		{keyboard[keyconfig.st] |= (pad_data & PSP_CTRL_START);}	else{keyboard[keyconfig.st] &= (~PSP_CTRL_START);}
	if (pad_data & PSP_CTRL_UP) 		{keyboard[keyconfig.u]	|= (pad_data & PSP_CTRL_UP);}		else{keyboard[keyconfig.u]	&= (~PSP_CTRL_UP);}
	if (pad_data & PSP_CTRL_RIGHT)		{keyboard[keyconfig.r]	|= (pad_data & PSP_CTRL_RIGHT);}	else{keyboard[keyconfig.r]	&= (~PSP_CTRL_RIGHT);}
	if (pad_data & PSP_CTRL_DOWN)		{keyboard[keyconfig.d]	|= (pad_data & PSP_CTRL_DOWN);} 	else{keyboard[keyconfig.d]	&= (~PSP_CTRL_DOWN);}
	if (pad_data & PSP_CTRL_LEFT)		{keyboard[keyconfig.l]	|= (pad_data & PSP_CTRL_LEFT);} 	else{keyboard[keyconfig.l]	&= (~PSP_CTRL_LEFT);}
	if (pad_data & PSP_CTRL_LTRIGGER)	{keyboard[keyconfig.lt] |= (pad_data & PSP_CTRL_LTRIGGER);} else{keyboard[keyconfig.lt] &= (~PSP_CTRL_LTRIGGER);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{keyboard[keyconfig.rt] |= (pad_data & PSP_CTRL_RTRIGGER);} else{keyboard[keyconfig.rt] &= (~PSP_CTRL_RTRIGGER);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{keyboard[keyconfig.sa] |= (pad_data & PSP_CTRL_TRIANGLE);} else{keyboard[keyconfig.sa] &= (~PSP_CTRL_TRIANGLE);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{keyboard[keyconfig.ma] |= (pad_data & PSP_CTRL_CIRCLE);}	else{keyboard[keyconfig.ma] &= (~PSP_CTRL_CIRCLE);}
	if (pad_data & PSP_CTRL_CROSS)		{keyboard[keyconfig.ba] |= (pad_data & PSP_CTRL_CROSS);}	else{keyboard[keyconfig.ba] &= (~PSP_CTRL_CROSS);}
	if (pad_data & PSP_CTRL_SQUARE) 	{keyboard[keyconfig.si] |= (pad_data & PSP_CTRL_SQUARE);}	else{keyboard[keyconfig.si] &= (~PSP_CTRL_SQUARE);}
//
	my_pad_alter = my_pad;
	my_pad = 0;
//	if (keyboard[KEY_SELECT])		{my_pad |= (PSP_KEY_SELECT);}
	if (keyboard[KEY_PAUSE])		{my_pad |= (PSP_KEY_PAUSE);}
//
		 if (keyboard[KEY_UP])		{my_pad |= (PSP_KEY_UP);}
	else if (keyboard[KEY_DOWN])	{my_pad |= (PSP_KEY_DOWN);}
		 if (keyboard[KEY_LEFT])	{my_pad |= (PSP_KEY_LEFT);}
	else if (keyboard[KEY_RIGHT])	{my_pad |= (PSP_KEY_RIGHT);}
//2716498 2716786 2716684
//	if (keyboard[KEY_SYSTEM])		{my_pad |= (PSP_KEY_SYSTEM);}
	if (keyboard[KEY_SLOW]) 		{my_pad |= (PSP_KEY_SLOW);}
	if (keyboard[KEY_SC_SHOT])		{my_pad |= (PSP_KEY_SC_SHOT);}
	if (keyboard[KEY_CANCEL])		{my_pad |= (PSP_KEY_CANCEL);}
	if (keyboard[KEY_SHOT]) 		{my_pad |= (PSP_KEY_SHOT);}
	if (keyboard[KEY_BOMB]) 		{my_pad |= (PSP_KEY_BOMB);}

//
	if (my_pad & PSP_KEY_SC_SHOT/*keybo ard[KEY_SC_SHOT]*/)		//スクリーンショット機能。keypollに入れると何故かうまくいかなかったのでこっちに場所を変更。
	{
		static int screennum=0;
	/*static*/ char screenbuf[32/*20*/];
		sprintf(screenbuf,"ms0:/PICTURE/Toho_Moho%d.bmp",screennum++);		//保存場所の変更。
		SDL_SaveBMP(screen,screenbuf);
	}
}
//		 if (keyboard[KEY_LEFT])	{		my_pad |= PSP_CTRL_LEFT;	/*direction=-1;*/		}
//	else if (keyboard[KEY_RIGHT])	{		my_pad |= PSP_CTRL_RIGHT;	/*direction=1;*/		}
//		 if (keyboard[KEY_UP])		{		my_pad |= PSP_CTRL_UP;		}
//	else if (keyboard[KEY_DOWN])	{		my_pad |= PSP_CTRL_DOWN;	}

//int keyboard_keypressed()
//{
//	int i;
//	for (i=0;i<15;i++)
//	{
//		if (keyboard[i]) return 1;
//	}
//	return 0;
//}



void hit_any_key(void)
{
	SceCtrlData cpad;
	/* 離されるまで待つ */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		if (0==cpad.Buttons)
		{
			goto l_end1;
		//	break;
		}
	}
	l_end1:
	;
	/* 押されるまで待つ */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		/*Any Key*/
		if (cpad.Buttons & (PSP_CTRL_SQUARE|PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE) )
		{
			goto l_end2;
		//	break;
		}
	}
	l_end2:
	;
}

/*---------------------------------------------------------
	エラー処理
---------------------------------------------------------*/
void error(int errorlevel, char *msg, ...)
{
	char msgbuf[128];
	va_list argptr;

	va_start(argptr, msg);
	vsprintf(msgbuf, msg, argptr);
	va_end(argptr);

	switch (errorlevel)
	{
	//case ERR_DEBUG: 	if (debug) { fprintf(stdout,"DEBUG: %s\n",msgbuf); } break;
	//case ERR_INFO:		fprintf(stdout,"INFO: %s\n",msgbuf); break;

	#if 0
	/*デバッグ用*/
	case ERR_WARN:	//fprintf(stdout,"WARNING: %s\n",msgbuf);
		pspDebugScreenSetXY(2,3);
		pspDebugScreenPrintf("WARNING");
		hit_any_key();
		{
		char msgbuf2[32/*128*/];
			int j;
			for (j=0;j<5;j++)
			{
				pspDebugScreenSetXY(2,5+j);
				strncpy(msgbuf2, &msgbuf[j*24], 24);	/*24文字ずつ表示*/
				/* [0] ... [23] で24文字 */
				msgbuf2[24]=0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				msgbuf2[25]=0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				msgbuf2[26]=0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				msgbuf2[27]=0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				pspDebugScreenPrintf("%s",	msgbuf2	);
			}
			hit_any_key();
		}
		break;
	#endif
	case ERR_FATAL: //	fprintf(stdout,"FATAL: %s\n",msgbuf);
		pspDebugScreenSetXY(2,3);
		pspDebugScreenPrintf("FATAL ERROR");
		hit_any_key();
		pspDebugScreenInit();/*要る*/
		pspDebugScreenClear();
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("%s",	msgbuf	);
		hit_any_key();
		sceKernelExitGame();	//if (errorlevel==ERR_FATAL) exit(1);/*exit(1)はpspで使えないので注意*/
		break;
	}
}


/*---------------------------------------------------------
	システムの基礎部分
---------------------------------------------------------*/
void newstate(int m, int s, int n)
{
	laststate=state;
	if (m>=0) state.mainstate=m;
	if (s>=0) state.substate=s;
	if (n>=0) state.newstate=n;
}

static void imglist_garbagecollect(void);
void *mmalloc(size_t size)
{
	void *ptr;
	ptr=malloc(size);
	if (ptr==NULL)
	{
		error(ERR_WARN,"can't alloc %d bytes, trying garbage collection",size);
		imglist_garbagecollect();
		ptr=malloc(size);
		if (ptr==NULL) {
			error(ERR_FATAL,"I'm sorry, but you're out of memory!");
		}
	}
	return ptr;
}

/*---------------------------------------------------------
	画像関連
---------------------------------------------------------*/

void psp_clear_screen(void)
{
	/* 将来Guで描いた場合。ハードウェアー機能で、置き換えられるので今のうちにまとめとく */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
}
void psp_push_screen(void)
{
	SDL_BlitSurface(screen,NULL,back_screen,NULL);
}
void psp_pop_screen(void)
{
	SDL_BlitSurface(back_screen,NULL,screen,NULL);
}


/*---------------------------------------------------------
	画像キャッシュ関連
---------------------------------------------------------*/

/*
	IMGLISTの構造
	struct ImgList{
		char name[64];
		SDL_Surface* pSurface;
		ImgList* Pre;
		ImgList* Next;
	};
*/
typedef struct _imglist
{
	char name[256];
	int refcount;
	SDL_Surface *img;
	struct _imglist *next;
} IMGLIST;

/*画像キャッシュのリスト*/
static IMGLIST *img_list/*=NULL*/;/*←この初期化処理はpspでは正常に動作しない*/

/*extern*/ void init_imglist(void)
{
	img_list = NULL;
}
static void imglist_add(SDL_Surface *s, char *name)
{
	IMGLIST *new_list;
	new_list			= mmalloc(sizeof(IMGLIST));
	strcpy(new_list->name,name);
	new_list->refcount	= 1;
	new_list->img		= s;
	if (img_list==NULL)
	{
		img_list		= new_list;
		new_list->next	= NULL;
	}
	else
	{
		new_list->next	= img_list;
		img_list		= new_list;
	}
}

static SDL_Surface *imglist_search(char *name)
{
	IMGLIST *i=img_list;
	while (i!=NULL)
	{
		if (!strcmp(name,i->name))
		{
			i->refcount++;
			return (i->img);
		}
		i=i->next;
	}
	return(NULL);
}

static void imglist_garbagecollect(void)
{
	IMGLIST *s=img_list,*p=NULL,*n=NULL;
	while (s!=NULL)
	{
		n=s->next;
		if (s->refcount==0)
		{
			if (p==NULL)
			{
				img_list=n;
			} else
			{
				p->next=n;
			}
			//SDL_FreeSurface(s->img);
			free(s);
		} else {
			p=s;
		}
		s=n;
	}
}

SDL_Surface *loadbmp0(char *filename, int use_alpha)
{
	char fn[64/*50*/];
	strcpy(fn,moddir);
	strcat(fn,"/");
	strcat(fn,filename);
//
	SDL_Surface *s1;
	SDL_Surface *s2;
//
	s1 = imglist_search(fn);
	if ( NULL != s1 )
	{
		return s1;
	}
	//if ((s1=SDL_LoadBMP(fn))==NULL)
	s1 = IMG_Load(fn);
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant load image %s:\n %s",fn,SDL_GetError());
	}
	if (use_alpha)
	{
		s2 = SDL_DisplayFormatAlpha(s1);	//α値を持ったsurface
	}
	else
	{
		s2 = SDL_DisplayFormat(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant convert image %s to display format: %s",fn,SDL_GetError());
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	imglist_add(s2,fn);
	return(s2);
}
SDL_Surface *loadbmp(char *filename/*, int use_alpha*/)
{
	return	loadbmp0(filename, 0);
}
SDL_Surface *loadbmp2(char *filename/*, int use_alpha*/)
{
	return	loadbmp0(filename, 1);
}

void unloadbmp_by_surface(SDL_Surface *s)
{
	/*
		*sがIMGLIST内にあるか確認をし、あった場合そのSurfaceのrefcountを0にする。
	*/
	IMGLIST *i=img_list;
	while (i!=NULL)
	{
		if (s==i->img)
		{
			if (!i->refcount)
			{
				CHECKPOINT;
				error(ERR_WARN,"unloadbmp_by_surface: refcount for object %s is already zero",i->name);
			}
			else
			{
				i->refcount--;
			}
			return;
		}
		i=i->next;
	}
	CHECKPOINT;
	error(ERR_WARN,"unloadbmp_by_surface: object not found");
}

/*
void unloadbmp_by_name(char *name)
{
	char fn[50];
	strcpy(fn,moddir);
	strcat(fn,"/");
	strcat(fn,name);
	IMGLIST *i=img_list;

	while (i!=NULL) {
		if (!strcmp(i->name,fn)) {
			if (!i->refcount) {
				CHECKPOINT;
				error(ERR_WARN,"unloadbmp_by_name: refcount for object %s is already zero",i->name);
			} else {
				i->refcount--;
			}
			return;
		}
		i=i->next;
	}
	CHECKPOINT;
	error(ERR_WARN,"unloadbmp_by_name: object not found");
}
*/


/* dont forget to lock surface when using get/putpixel! */
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	#if (16!=depth)
	int bpp=surface->format->BytesPerPixel;
	Uint8 *p=(Uint8 *)surface->pixels+y*surface->pitch+x*bpp;
	#else
	Uint8 *p=(Uint8 *)surface->pixels+y*surface->pitch+x+x;
	#endif
	if (x>=clip_xmin(surface) && x<=clip_xmax(surface) && y>=clip_ymin(surface) && y<=clip_ymax(surface))
	{
		#if (16!=depth)
		switch (bpp)
		{
		case 1:
			return *p;
		case 2:
		#endif
			return *(Uint16 *)p;
		#if (16!=depth)
		case 3:
			if (SDL_BYTEORDER==SDL_BIG_ENDIAN)
			{	return p[0]<<16|p[1]<<8|p[2];}
			else
			{	return p[0]|p[1]<<8|p[2]<<16;}
		case 4:
			return *(Uint32 *)p;
		default:
			return 0;
		}
		#endif
	} else return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	#if (16!=depth)
	int bpp=surface->format->BytesPerPixel;
	Uint8 *p=(Uint8 *)surface->pixels+y*surface->pitch+x*bpp;
	#else
	Uint8 *p=(Uint8 *)surface->pixels+y*surface->pitch+x+x;
	#endif
	if (x>=clip_xmin(surface) && x<=clip_xmax(surface) && y>=clip_ymin(surface) && y<=clip_ymax(surface))
	{
		#if (16!=depth)
		switch (bpp)
		{
		case 1:
			*p=pixel;
			break;
		case 2:
		#endif
			*(Uint16 *)p=pixel;
		#if (16!=depth)
			break;
		case 3:
			if (SDL_BYTEORDER==SDL_BIG_ENDIAN)
			{
				p[0]=(pixel>>16)&0xff;
				p[1]=(pixel>>8)&0xff;
				p[2]=pixel&0xff;
			}
			else
			{
				p[2]=(pixel>>16)&0xff;
				p[1]=(pixel>>8)&0xff;
				p[0]=pixel&0xff;
			}
			break;
		case 4:
			*(Uint32 *)p=pixel;
		}
		#endif
	}
}
/*
void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2)
{
	int dx,dy,dxabs,dyabs,i,px,py,sdx,sdy,x,y;

	dx=x2-x1;
	dy=y2-y1;
	sdx=sign(dx);
	sdy=sign(dy);
	dxabs=abs(dx);
	dyabs=abs(dy);
	x=0;
	y=0;
	px=x1;
	py=y1;

	if (SDL_MUSTLOCK(s))
		SDL_LockSurface(s);

	if (dxabs>=dyabs) {
		for (i=0;i<dxabs;i++) {
			y+=dyabs;
			if (y>=dxabs) {
				y-=dxabs;
				py+=sdy;
			}
			putpixel(s,px,py-1,farbe2);
			putpixel(s,px,py,farbe1);
			putpixel(s,px,py+1,farbe2);
			px+=sdx;
		}
	} else {
		for (i=0;i<dyabs;i++) {
			x+=dxabs;
			if (x>=dyabs) {
				x-=dyabs;
				px+=sdx;
			}
			putpixel(s,px-1,py,farbe2);
			putpixel(s,px,py,farbe1);
			putpixel(s,px+1,py,farbe2);
			py+=sdy;
		}
	}
	if (SDL_MUSTLOCK(s))
		SDL_UnlockSurface(s);
}

void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1)
{
	int dx,dy,dxabs,dyabs,i,px,py,sdx,sdy,x,y;

	dx=x2-x1;
	dy=y2-y1;
	sdx=sign(dx);
	sdy=sign(dy);
	dxabs=abs(dx);
	dyabs=abs(dy);
	x=0;
	y=0;
	px=x1;
	py=y1;

	if (SDL_MUSTLOCK(s))
		SDL_LockSurface(s);

	if (dxabs>=dyabs) {
		for (i=0;i<dxabs;i++) {
			y+=dyabs;
			if (y>=dxabs) {
				y-=dxabs;
				py+=sdy;
			}
			putpixel(s,px,py,farbe1);
			px+=sdx;
		}
	} else {
		for (i=0;i<dyabs;i++) {
			x+=dxabs;
			if (x>=dyabs) {
				x-=dyabs;
				px+=sdx;
			}
			putpixel(s,px,py,farbe1);
			py+=sdy;
		}
	}
	if (SDL_MUSTLOCK(s))
		SDL_UnlockSurface(s);
}
*/

void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct)
{
	/*
		拡大縮小しながら表示する。
	*/
	Sint32 x, y;
	Sint32 u, v;
	Uint32 col, key;
	key=src->format->colorkey;
	if (SDL_MUSTLOCK(src))	{	SDL_LockSurface(src);}
	if (SDL_MUSTLOCK(dst))	{	SDL_LockSurface(dst);}
	for (y = 0; y<dst_rct->h; y++)
	{
		for (x = 0; x<dst_rct->w; x++)
		{
			u = src_rct->w * x / dst_rct->w;
			v = src_rct->h * y / dst_rct->h;
			col=getpixel(src, u + src_rct->x, v + src_rct->y);
			if (col!=key)
			{	putpixel(dst, x + dst_rct->x, y + dst_rct->y, col);}
		}
	}
	if (SDL_MUSTLOCK(src))	{	SDL_UnlockSurface(src);}
	if (SDL_MUSTLOCK(dst))	{	SDL_UnlockSurface(dst);}
}

/* just a quick hack - dont know if i will use it in the final game...
 * blits only every 2nd pixel, to archive a cheap 50%-alpha effect.
 */
 /*
void blit_calpha(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct)
{
	Sint32 x, y;
	Uint32 col, key;

	Uint16 *ps,*pd;
	Uint16 *sps,*spd;

	if (src->format->BitsPerPixel!=dst->format->BitsPerPixel) {
		CHECKPOINT;
		error(ERR_FATAL,"cant handle different pixelformats (yet?)");
	}
	if (src->format->BitsPerPixel!=16) {
		CHECKPOINT;
		error(ERR_FATAL,"can only handle 16bit-pixelformat");
	}
	key=src->format->colorkey;

	if (SDL_MUSTLOCK(src))
		SDL_LockSurface(src);
	if (SDL_MUSTLOCK(dst))
		SDL_LockSurface(dst);

	ps=(Uint16 *)src->pixels+src_rct->y*src->pitch/2+src_rct->x;
	pd=(Uint16 *)dst->pixels+dst_rct->y*dst->pitch/2+dst_rct->x;

	for (y=0;y<src_rct->h;y++) {
		sps=ps;
		spd=pd;

		if (y%2) {
			ps++;
			pd++;
		}

		for (x=0;x<src_rct->w;x+=2) {

			if ((x+dst_rct->x>=0) &&
			   (x+dst_rct->x<dst->w) &&
			   (y+dst_rct->y>=0) &&
			   (y+dst_rct->y<dst->h)) {

				col=*(ps);
				if (col!=key)
					*(pd)=col;
			}
			pd+=2;
			ps+=2;
		}
		ps=sps+src->pitch/2;
		pd=spd+dst->pitch/2;
	}

	if (SDL_MUSTLOCK(src))
		SDL_UnlockSurface(src);
	if (SDL_MUSTLOCK(dst))
		SDL_UnlockSurface(dst);
}
*/

/*
void toggle_fullscreen()
{
	SDL_Surface *tmp;

	if (videoflags==SDL_DOUBLEBUF)
		videoflags=SDL_FULLSCREEN;
	else
		videoflags=SDL_DOUBLEBUF;

	tmp=SDL_ConvertSurface(screen,screen->format,screen->flags);
	if (tmp==NULL) {
		CHECKPOINT;
		error(ERR_FATAL,"cant copy screen");
	}
	if ((screen=SDL_SetVideoMode(WIDTH,HEIGHT,depth,videoflags))==NULL) {
		CHECKPOINT;
		error(ERR_FATAL,"cant open change fullscreen/window: %s",SDL_GetError());
	}
	SDL_BlitSurface(tmp,NULL,screen,NULL);
	//SDL_FreeSurface(tmp);
	// display_vidinfo();
}
*/
/*
void display_vidinfo()
{
	const SDL_VideoInfo *s;
	char driver[256];
	SDL_Rect **modes;
	int i;

	error(ERR_DEBUG,"============ SDL VideoInfo ============");

	s=SDL_GetVideoInfo();

	if (SDL_VideoDriverName(driver,256)==NULL)
		error(ERR_WARN,"couldn't get video driver name");
	else
		error(ERR_DEBUG,"Video Driver: %s",driver);

	error(ERR_DEBUG,"BitsPerPixel: %d",s->vfmt->BitsPerPixel);
	if (s->vfmt->palette==NULL) {
		error(ERR_DEBUG,"R Mask      : 0x%.8x",s->vfmt->Rmask);
		error(ERR_DEBUG,"G Mask      : 0x%.8x",s->vfmt->Gmask);
		error(ERR_DEBUG,"B Mask      : 0x%.8x",s->vfmt->Bmask);
	}
	error(ERR_DEBUG,"HW Surface  : %savailable",s->hw_available ? "" : "not ");
	error(ERR_DEBUG,"Win-Manager : %savailable",s->wm_available ? "" : "not ");
	error(ERR_DEBUG,"H->H Blit   : %savailable",s->blit_hw ? "" : "not ");
	error(ERR_DEBUG,"H->H Blit CC: %savailable",s->blit_hw_CC ? "" : "not ");
	error(ERR_DEBUG,"H->H Blit A : %savailable",s->blit_hw_A ? "" : "not ");
	error(ERR_DEBUG,"S->H Blit   : %savailable",s->blit_sw ? "" : "not ");
	error(ERR_DEBUG,"S->H Blit CC: %savailable",s->blit_sw_CC ? "" : "not ");
	error(ERR_DEBUG,"S->H Blit A : %savailable",s->blit_sw_A ? "" : "not ");
	error(ERR_DEBUG,"Color Fill  : %savailable",s->blit_fill ? "" : "not ");
	error(ERR_DEBUG,"Video-Mem   : %d",s->video_mem);

	error(ERR_DEBUG,"Available Fullscreen modes:");
	modes=SDL_ListModes(NULL,SDL_FULLSCREEN);
	for (i=0;modes[i];i++) {
		error(ERR_DEBUG,"%d: %dx%d",i+1,modes[i]->w,modes[i]->h);
	}
	error(ERR_DEBUG,"Available HW-surfaces modes:");
	modes=SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_HWSURFACE);
	for (i=0;modes[i];i++) {
		error(ERR_DEBUG,"%d: %dx%d",i+1,modes[i]->w,modes[i]->h);
	}
	error(ERR_DEBUG,"=======================================");
}
*/


