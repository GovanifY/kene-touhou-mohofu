/*****************************************************************************
*zenkaku.c
*全角文字列表示サンプル
*****************************************************************************/
#include <SDL/SDL_image.h>
//#include	<pspuser.h>
//#include	<pspgu.h>
//#include	<pspdisplay.h>
#include "zenkaku.h"
/*****************************************************************************
*macros
*****************************************************************************/
/*キャッシュなしVRAM　VRAM[y][x]としてアクセス*/
//#define	VRAM		((long(*)[BUF_WIDTH])(((char*)0x4000000)+0x40000000))
/*****************************************************************************
*prots
*****************************************************************************/
static	void	draw_char(SDL_Surface *src,int x,int y,SDL_Surface *col_n,int chr);
static	void	draw_zenkaku(SDL_Surface *src,int x,int y,int hi,int lo,SDL_Surface *col_n);
static	void	draw_hankaku(SDL_Surface *src,int x,int y,int ch,SDL_Surface *col_n);
static	int	iskanji(int c);
static	int	chrwidth(int hi);
/*****************************************************************************
*for script
*****************************************************************************/
static int count_width=0;

/* グローバル */
int count_i=0;
SDL_Surface *col_sdl=NULL;
extern int count_char;
extern double fps_factor;

/*****************************************************************************
*文字をVRAMに転送しません
*****************************************************************************/
static	void	draw_char(SDL_Surface *src,int x,int y,SDL_Surface *col_n,int chr)
{
	/*
		x	表示座標
		y	表示座標
		col_n	フォントの種類
		chr	文字
	*/
	extern	const	char	shinonomefont[];
	int	dx, dy, bit, bitcnt;
	Uint32 dot;
	/*
		dx		ドットx
		dy		ドットy
		bit		調べるbyte情報
		bitcnt	調べてるbyteが後何回調べられるか:最大8回(後述)
				bitと&することで調べてるbyteのbit情報を得る。
	*/
	if(x > src->w-FONTWIDTH){
		return;
	}
	bitcnt = 0;
	bit = 0;
	chr *= 16	/* 9 */;	//ここでの9は多分1つの文字に使うバイト数。(1byte == 8bit)*9=72 == 6*12
//	col_n &= 0xffffff;
	for(dy = 0 ; dy < FONTHEIGHT ; dy++) {
		for(dx = 0 ; dx < FONTWIDTH ; dx++) {
			bitcnt >>= 1;
			if(!bitcnt){	//bitcnt==0の時
				bit = shinonomefont[chr++];
				bitcnt = 0x80;		//1byteの情報量が8の為 80 40 20 10 8 4 2 1(0になったらすぐ80になるためカウントされない)
			}
			if(bit & bitcnt){
				dot=getpixel(col_n, dx, dy);
				putpixel(src, dx+x, dy+y, dot);
			}
		}
	}
}
/*****************************************************************************
*半角文字の描画
*****************************************************************************/
static	void	draw_hankaku(SDL_Surface *src,int x,int y,int ch,SDL_Surface *col_n)
{
	if(ch < 0x20){		//00-1f
		return;
	}else if(ch < 0x80){	//20-7f
		ch -= 0x20;
	}else if(ch < 0xa0){	//80-9f
		return;
	}else if(ch < 0xe0){	//a0-df
		ch -= 0x40;
	}else{			//e0-ff
		return;
	}
	draw_char(src,x,y,col_n,ch);
}
/*****************************************************************************
*全角文字の描画
*****************************************************************************/
static	void	draw_zenkaku(SDL_Surface *src,int x,int y,int hi,int lo,SDL_Surface *col_n) {
	/*
		hi	上位byte
		lo	下位byte
	*/
	int	idx;
	static	const	int codetbl[] = {
/*80*/		-1, 0, 1, 2, 3,-1,-1, 4, 5, 6, 7, 8, 9,10,11,12,
/*90*/		13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,
/*e0*/		29,30,31,32,33,34,35,36,37,38,39,-1,-1,40,41,-1,
/*f0*/		-1,-1,-1,-1,-1,-1,-1,-1,42,43,44,45,46,47,-1,-1
	};
/*下位バイトのチェック*/
	if(lo < 0x40){		//0x40以下の場合は該当する漢字が存在しないため。
		return;
	}
	lo -= 0x40;
/*上位バイトからインデックスを得る*/
	idx = codetbl[hi & 0x3f];
/*表にない文字は無視*/
	if(idx == -1){
		return;
	}
/**/
	idx = (16*10)/*半角文字をスキップ*/ + idx*2*192/*codetbl１個あたり192文字*/;
	if(chrwidth(hi) == 1){	//２バイト半角
		idx += lo;
		draw_char(src,x,y,col_n,idx);
	}else{
		idx += lo*2;
		draw_char(src,x,y,col_n,idx);
		draw_char(src,x+FONTWIDTH,y,col_n,idx+1);
	}
}
/*****************************************************************************
*文字列の描画
*****************************************************************************/
void mh_print(SDL_Surface *src,SDL_Rect *rect,int col,const char *str)
{
	/*
		src		書き込み先
		rect	書き込み位置
		col		フォントの地の色の画像番号
		str		書き込む文字
	*/
	int	ch = 0,bef = 0,init_x=rect->x;
	char buffer[30];
	sprintf(buffer,"font_bg%02d.png", col);

	int cw=(int)((rect->w)/FONTWIDTH)+2;
/*************mh_print_fpsの初期化****************/
	if(count_char){		/* カーソル位置が記憶されている場合 */
		rect->y+=(count_char/cw)*(FONTHEIGHT+2);
		rect->x+=(count_char%cw)*FONTWIDTH;

		if(rect->x > init_x+rect->w){
			rect->x=init_x;
			rect->y+=FONTHEIGHT+2;
		}
	}
	if(col_sdl!=NULL){	unloadbmp_by_surface(col_sdl);	col_sdl=NULL;	}
	//col_sdl=loadbmp(buffer);

	char fn[64];
	strcpy(fn,moddir);
	strcat(fn,"/fonts/");
	strcat(fn,buffer);
	col_sdl = IMG_Load(fn);
	col_sdl = SDL_DisplayFormat(col_sdl);

	count_i=0;

/*******************実際の処理********************/
	if (SDL_MUSTLOCK(src)){	SDL_LockSurface(src);	}
	if (SDL_MUSTLOCK(col_sdl)){	SDL_LockSurface(col_sdl);	}

	while((ch = (*str++ & 0xff)) != 0) {
		if(ch=='\\'){		/* エスケープ処理 */
			char esc = *str++ & 0xff;

			if(esc =='n'){
				count_char+=(int)((rect->w-(rect->x-init_x))/FONTWIDTH)+2;
				rect->x=init_x;
				rect->y+=FONTHEIGHT+2;
			}
		}
		else{
			if (bef!=0) {		//2byteだった場合。実際の処理。
				draw_zenkaku(src,rect->x, rect->y, bef, ch, col_sdl);
				rect->x += FONTWIDTH*chrwidth(bef);
				count_char+=chrwidth(bef);
				bef=0;
			} else {
				if(iskanji(ch)){		//2byteだった場合。ここではフラグだけ。
					bef = ch;
				} else {
					draw_hankaku(src,rect->x,rect->y,ch,col_sdl);
					rect->x += FONTWIDTH;
					count_char++;
				}
			}
			if(rect->x > init_x+rect->w+FONTWIDTH){	/*画面を越えさせない*/
				if(rect->x > init_x+rect->w+2*FONTWIDTH)
					rect->x=init_x+FONTWIDTH;
				else
					rect->x=init_x;
				rect->y+=FONTHEIGHT+2;
			}
		}
	}/*while*/

	if (SDL_MUSTLOCK(src)){	SDL_UnlockSurface(src);	}
	if (SDL_MUSTLOCK(col_sdl)){	SDL_UnlockSurface(col_sdl);	}

	unloadbmp_by_surface(col_sdl);
	col_sdl=NULL;
}

/*****************************************************************************
* fps毎に一文字描写 
*****************************************************************************/
int mh_print_fps(SDL_Surface *src,int col, SDL_Rect *rect,const char *str,int wait)
{
	/*
		count_i		アドレスのオフセット
		count_width	横に並べられる文字の数
		col_sdl		文字の色
		wait		1つの文字に掛ける時間
	*/
	static unsigned int ch_wait;
	static int esc_len;
	ch_wait++;
	if(!count_i)		//初期化
	{
		char buffer[30];
		sprintf(buffer,"font_bg%02d.png", col);
		if(col_sdl!=NULL)	{	unloadbmp_by_surface(col_sdl);	col_sdl=NULL;	}
		count_width=(int)((rect->w)/FONTWIDTH)+2;
		esc_len=0;

		char fn[64];
		strcpy(fn,moddir);
		strcat(fn,"/fonts/");
		strcat(fn,buffer);
		col_sdl = IMG_Load(fn);
		col_sdl = SDL_DisplayFormat(col_sdl);
	}

	if(ch_wait>wait)
	{
		ch_wait=0;
		if (SDL_MUSTLOCK(src)){	SDL_LockSurface(src);	}				/* ロック */
		if (SDL_MUSTLOCK(col_sdl)){	SDL_LockSurface(col_sdl);	}
		int	ch = 0,bef = 0;

		if((ch = (*(str+count_i) & 0xff)) != 0) {
			if(ch=='\\'){		/* エスケープ処理 */
				char esc = *(str+count_i+1) & 0xff;

				if(esc =='n'){
					esc_len+=count_width-(count_i+count_char+esc_len)%count_width-2;
					count_i+=2;
				}
			}
			else{
				if(iskanji(ch)){
					bef = ch;
					ch = (*(str+count_i+1) & 0xff);
					draw_zenkaku(src,rect->x+((count_i+count_char+esc_len)%count_width)*FONTWIDTH,rect->y+((count_i+count_char+esc_len)/count_width)*(FONTHEIGHT+2), bef, ch, col_sdl);
					count_i+=chrwidth(bef);
				}
				else{
					draw_hankaku(src,rect->x+((count_i+count_char+esc_len)%count_width)*FONTWIDTH,rect->y+((count_i+count_char+esc_len)/count_width)*(FONTHEIGHT+2),ch,col_sdl);
					count_i++;
				}
			}
		}
		else			//終了処理
		{
			if (SDL_MUSTLOCK(src)){	SDL_UnlockSurface(src);	}				/* ロック解除 */
			if (SDL_MUSTLOCK(col_sdl)){	SDL_UnlockSurface(col_sdl);	}
			unloadbmp_by_surface(col_sdl);
			col_sdl=NULL;	
			count_width=0;
			count_char+=count_i+esc_len;
			count_i=0;
			return 1;		/* end */
		}
		if (SDL_MUSTLOCK(src)){	SDL_UnlockSurface(src);	}				/* ロック解除 */
		if (SDL_MUSTLOCK(col_sdl)){	SDL_UnlockSurface(col_sdl);	}

	}
	return 0;		/* continue */
}
/*****************************************************************************
*全角一バイト目かどうか判定
*****************************************************************************/
static	int	iskanji(int c)
{
	c &= 0xff;
	return ((c >= 0x81) && (c < 0xa0)) || ((c >= 0xe0) && (c <= 0xfd));
}
/*****************************************************************************
*文字の描画幅を文字単位で返す
*****************************************************************************/
static	int	chrwidth(int hi)
{
	hi &= 0xff;
	if(hi == 0xfd){ //２バイト半角
		return 1;
	}
	return iskanji(hi) ? 2 : 1;
}
