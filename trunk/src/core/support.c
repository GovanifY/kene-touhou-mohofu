
/*
ファイル関連のサポートルーチンをここに書くと遅くなるので
ここには追加しないで下さい。
ファイル関連のサポートルーチンは、 startintro.c に記述してください。

ファイル関連のサポートルーチン：
	追加モジュール(MOD)ファイル読み込み
	ハイスコアファイル読み込み／保存
	設定読み込み／保存
	画面保存
*/

/*---------------------------------------------------------
	ヘッダ
---------------------------------------------------------*/

#include "support.h"


/*---------------------------------------------------------
	定数
---------------------------------------------------------*/


/*---------------------------------------------------------
	外部グローバル変数
---------------------------------------------------------*/


/*---------------------------------------------------------
	グローバル変数
---------------------------------------------------------*/
// /*extern*/SDL_Surface *screen;
// SDL_Surface *back_screen;
// SDL_Surface *tex_screen;

SDL_Surface *sdl_screen[4];

//int debug=0;
//int use_joystick=1;

/*---------------------------------------------------------
	math.h
---------------------------------------------------------*/

/* この配列もCPU内蔵の命令キャッシュに乗るよ */
/*extern*/ /*dou ble*/int sin_tbl512[SINTABLE_SIZE];

/* この関数はただの初期化。命令キャッシュに乗らないよ */
void init_math(void)
{
	unsigned int i;
	for (i=0; i<SINTABLE_SIZE; i++)
	{
		sin_tbl512[i] = (int)(sin( (i*(M_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
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
	将来的にリプレイに対応できるように、
	自前の乱数ルーチンを用意しとく。
	(かなりテキトーですが)
---------------------------------------------------------*/

static int rnd;
int ra_nd(void)
{
	rnd = (rnd * 8513/*multiplier*/) + 179/*addend*/;
	rnd = ((rnd) ^ (rnd>>8));
	return (rnd);
}

/*---------------------------------------------------------
	乱数初期値設定(将来的にリプレイに対応できるように、
	面の始めに固定値を設定する)
---------------------------------------------------------*/

void set_rnd_seed(int set_seed)
{
	rnd = set_seed;
}

/*---------------------------------------------------------
	難易度スコア補正
---------------------------------------------------------*/

int adjust_score_by_difficulty(int convert_score)
{
	int mul_tbl256[4] = /* 難易度補正 */
	{
	/* easy */		t256(0.5),
	/* normal */	t256(1.0),
	/* hard */		t256(1.2),
	/* lunatic */	t256(1.5),
	};
	return (((convert_score)*(mul_tbl256[difficulty]))>>8);
}

/*---------------------------------------------------------
	キー入力関連の処理(汎用)
---------------------------------------------------------*/

/*extern*/ int keyconfig[key_MAX];
static int keyboard[16/*15*/];
void keyboard_clear(void)
{
	int i;
	for (i=0;i<15;i++)
	{
		keyboard[i] = 0;
	}
}

extern void save_screen_shot(void);
/*global*/Uint32 my_pad;		/*今回入力*/
/*global*/Uint32 my_pad_alter;	/*前回入力*/
/*global*/short my_analog_x;	/* アナログ量、補正済み */
/*global*/short my_analog_y;	/* アナログ量、補正済み */

void keyboard_poll(void)
{
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);
	#if (1==USE_KEY_CONFIG)
		int
	#else
		my_pad_alter = my_pad;
		#define pad_data my_pad
	#endif /* (1==USE_KEY_CONFIG) */
	pad_data = pad.Buttons;
	my_analog_x = 0;
	my_analog_y = 0;
	/* 標準アナログキー機能 */
//	if (1==use_analog)
	{
		/* PSPのアナログ入力はデジタル入力へ変換、アナログ量は中心を削除し256固定小数点形式へ補正 */
		if (pad.Lx < 64/*70*/)			{		pad_data |= PSP_CTRL_LEFT;		my_analog_x =  ((64-pad.Lx)<<2);	}
		else if (pad.Lx > 192/*185*/)	{		pad_data |= PSP_CTRL_RIGHT; 	my_analog_x =  ((pad.Lx-192)<<2);	}
		//
		if (pad.Ly < 64/*70*/)			{		pad_data |= PSP_CTRL_UP;		my_analog_y =  ((64-pad.Ly)<<2);	}
		else if (pad.Ly > 192/*185*/)	{		pad_data |= PSP_CTRL_DOWN;		my_analog_y =  ((pad.Ly-192)<<2);	}
	}
	#if (1==USE_KEY_CONFIG)
	/* PSPのデジタル入力をキーコンフィグの形式に変換 */
	if (pad_data & PSP_CTRL_SELECT) 	{keyboard[keyconfig[key_00_sl]] |= (pad_data & PSP_CTRL_SELECT);}	else	{keyboard[keyconfig[key_00_sl]] &= (~PSP_CTRL_SELECT);}
	if (pad_data & PSP_CTRL_START)		{keyboard[keyconfig[key_01_st]] |= (pad_data & PSP_CTRL_START);}	else	{keyboard[keyconfig[key_01_st]] &= (~PSP_CTRL_START);}

	#if (1==USE_KEY_CONFIG_ALLOW)
	if (pad_data & PSP_CTRL_UP) 		{keyboard[keyconfig[key_02_u]]	|= (pad_data & PSP_CTRL_UP);}		else	{keyboard[keyconfig[key_02_u]]	&= (~PSP_CTRL_UP);}
	if (pad_data & PSP_CTRL_RIGHT)		{keyboard[keyconfig[key_03_r]]	|= (pad_data & PSP_CTRL_RIGHT);}	else	{keyboard[keyconfig[key_03_r]]	&= (~PSP_CTRL_RIGHT);}
	if (pad_data & PSP_CTRL_DOWN)		{keyboard[keyconfig[key_04_d]]	|= (pad_data & PSP_CTRL_DOWN);} 	else	{keyboard[keyconfig[key_04_d]]	&= (~PSP_CTRL_DOWN);}
	if (pad_data & PSP_CTRL_LEFT)		{keyboard[keyconfig[key_05_l]]	|= (pad_data & PSP_CTRL_LEFT);} 	else	{keyboard[keyconfig[key_05_l]]	&= (~PSP_CTRL_LEFT);}
	#endif /* (1==USE_KEY_CONFIG_ALLOW) */

	if (pad_data & PSP_CTRL_LTRIGGER)	{keyboard[keyconfig[key_06_lt]] |= (pad_data & PSP_CTRL_LTRIGGER);} else	{keyboard[keyconfig[key_06_lt]] &= (~PSP_CTRL_LTRIGGER);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{keyboard[keyconfig[key_07_rt]] |= (pad_data & PSP_CTRL_RTRIGGER);} else	{keyboard[keyconfig[key_07_rt]] &= (~PSP_CTRL_RTRIGGER);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{keyboard[keyconfig[key_08_sa]] |= (pad_data & PSP_CTRL_TRIANGLE);} else	{keyboard[keyconfig[key_08_sa]] &= (~PSP_CTRL_TRIANGLE);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{keyboard[keyconfig[key_09_ma]] |= (pad_data & PSP_CTRL_CIRCLE);}	else	{keyboard[keyconfig[key_09_ma]] &= (~PSP_CTRL_CIRCLE);}
	if (pad_data & PSP_CTRL_CROSS)		{keyboard[keyconfig[key_10_ba]] |= (pad_data & PSP_CTRL_CROSS);}	else	{keyboard[keyconfig[key_10_ba]] &= (~PSP_CTRL_CROSS);}
	if (pad_data & PSP_CTRL_SQUARE) 	{keyboard[keyconfig[key_11_si]] |= (pad_data & PSP_CTRL_SQUARE);}	else	{keyboard[keyconfig[key_11_si]] &= (~PSP_CTRL_SQUARE);}
	/* キーコンフィグの形式では、ゲーム中速度的に無理なので、PSPのデジタル入力に似た方式に再び変換 */
	my_pad_alter = my_pad;
	my_pad = 0;
//	if (keyboard[KINOU_01_SELECT])		{my_pad |= (PSP_KEY_SELECT);}
	if (keyboard[KINOU_02_PAUSE])		{my_pad |= (PSP_KEY_PAUSE);}
//
	#if (1==USE_KEY_CONFIG_ALLOW)
		 if (keyboard[KINOU_03_UP]) 	{my_pad |= (PSP_KEY_UP);}
	else if (keyboard[KINOU_05_DOWN])	{my_pad |= (PSP_KEY_DOWN);}
		 if (keyboard[KINOU_04_RIGHT])	{my_pad |= (PSP_KEY_RIGHT);}
	else if (keyboard[KINOU_06_LEFT])	{my_pad |= (PSP_KEY_LEFT);}
	#else
//		 if (pad_data & PSP_CTRL_UP)	{my_pad |= (PSP_KEY_UP);}
//	else if (pad_data & PSP_CTRL_DOWN)	{my_pad |= (PSP_KEY_DOWN);}
//		 if (pad_data & PSP_CTRL_RIGHT) {my_pad |= (PSP_KEY_RIGHT);}
//	else if (pad_data & PSP_CTRL_LEFT)	{my_pad |= (PSP_KEY_LEFT);}
	my_pad |= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_KEY_DOWN|PSP_CTRL_LEFT));
	#endif /* (1==USE_KEY_CONFIG_ALLOW) */
//
//	if (keyboard[KINOU_07_SNAP_SHOT])	{my_pad |= (PSP_KEY_SC_SHOT);}/*ここでしか使わないので必要ない*/
	if (keyboard[KINOU_08_SYSTEM])		{my_pad |= (PSP_KEY_SYSTEM);}
	if (keyboard[KINOU_09_SLOW])		{my_pad |= (PSP_KEY_SLOW);}
	if (keyboard[KINOU_10_OPTION])		{my_pad |= (PSP_KEY_OPTION);}
	if (keyboard[KINOU_11_SHOT])		{my_pad |= (PSP_KEY_SHOT_OK);}
	if (keyboard[KINOU_12_BOMB])		{my_pad |= (PSP_KEY_BOMB_CANCEL);}

	/* スクリーンショット機能。 */
	// keypollに入れると何故かうまくいかなかったのでこっちに場所を変更。
	if (keyboard[KINOU_07_SNAP_SHOT]/*my_pad & PSP_KEY_SC_SHOT*/) { 	save_screen_shot(); }
	#else
	if (/*keyboard[KINOU_07_SNAP_SHOT]*/my_pad & PSP_KEY_SC_SHOT) { 	save_screen_shot(); }
	#endif /* (1==USE_KEY_CONFIG) */

	/* アナログサポート機能 */
//	if (1==use_analog)
	{
		/* デジタルよりアナログ優先 */
		if (0 == (my_analog_x+my_analog_y) )
		/*アナログ押してないと思われる場合(アナログ押してる場合はアナログ量をそのまま使う)*/
		{
			/* デジタルよりアナログ量を算出 */
				 if (pad_data & PSP_CTRL_UP)	{my_analog_y =	256;}
			else if (pad_data & PSP_CTRL_DOWN)	{my_analog_y =	256;}
				 if (pad_data & PSP_CTRL_RIGHT) {my_analog_x =	256;}
			else if (pad_data & PSP_CTRL_LEFT)	{my_analog_x =	256;}
		}
	}
}
//		 if (keyboard[KINOU_06_LEFT])	{my_pad |= PSP_CTRL_LEFT;	/*direction=-1;*/		}
//	else if (keyboard[KINOU_04_RIGHT])	{my_pad |= PSP_CTRL_RIGHT;	/*direction=1;*/		}
//		 if (keyboard[KINOU_03_UP]) 	{my_pad |= PSP_CTRL_UP; 	}
//	else if (keyboard[KINOU_05_DOWN])	{my_pad |= PSP_CTRL_DOWN;	}

//int keyboard_keypressed()
//{
//	int i;
//	for (i=0;i<15;i++)
//	{
//		if (keyboard[i]) return (1);
//	}
//	return (0);
//}

/*---------------------------------------------------------
	キー入力関連の処理(本来デバッグ用)
	上のキー入力に統合しても良いが、キー入力自体が
	おかしくなる場合もあるので、暫定的に最低限の入力として
	残してある。
---------------------------------------------------------*/

void hit_any_key(void)
{
	SceCtrlData cpad;
	/* 離されるまで待つ */
	while (1)
	{
		sceCtrlReadBufferPositive(&cpad, 1);
		if (0 == cpad.Buttons)
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
	//case ERR_DEBUG:	if (debug) { fprintf(stdout,"DEBUG: %s\n",msgbuf); } break;
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
				msgbuf2[24] = 0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				msgbuf2[25] = 0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				msgbuf2[26] = 0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				msgbuf2[27] = 0;/* strncpyが '\0' 入れてくれないみたいなので、区切りを入れる */
				pspDebugScreenPrintf("%s",	msgbuf2 );
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

static void imglist_garbagecollect(void);
void *mmalloc(size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if (NULL == ptr)
	{
		error(ERR_WARN,"can't alloc %d bytes, trying garbage collection",size);
		imglist_garbagecollect();
		ptr = malloc(size);
		if (NULL == ptr)
		{
			error(ERR_FATAL,"I'm sorry, but you're out of memory!");
		}
	}
	return (ptr);
}

/*---------------------------------------------------------
	画像関連
---------------------------------------------------------*/

void psp_clear_screen(void)
{
	/* 将来Guで描いた場合。ハードウェアー機能で、置き換えられるので今のうちにまとめとく */
	SDL_FillRect(sdl_screen[SDL_00_SCREEN],NULL,SDL_MapRGB(sdl_screen[SDL_00_SCREEN]->format,0,0,0));
}
//void psp_move_screen(SDL_Surface *src_screen, SDL_Surface *dst_screen )
void psp_move_screen(int src_screen_number, int dst_screen_number )
{
	SDL_BlitSurface(sdl_screen[src_screen_number],NULL,sdl_screen[dst_screen_number],NULL);
}
//void psp_push_screen(void)
//{
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN],NULL,sdl_screen[SDL_01_BACK_SCREEN],NULL);
//}
//void psp_pop_screen(void)
//{
//	SDL_BlitSurface(sdl_screen[SDL_01_BACK_SCREEN],NULL,sdl_screen[SDL_00_SCREEN],NULL);
//}


/*---------------------------------------------------------
	画像キャッシュ関連

	同じ画像を複数読み込んだ場合にメモリが無駄になりもったいない。
	そこで同じ画像を読み込んだ場合には、実際には読み込まないで、
	前に読み込んだ画像と同じものを使う。
	トータルでいくつ同じ画像を読み込んだかは、それぞれの画像キャッシュの参照数でわかる。
---------------------------------------------------------*/

/*
	MY_IMAGE_LISTの構造
	struct ImgList
	{
		char name[64];
		SDL_Surface* pSurface;
		ImgList* Pre;
		ImgList* Next;
	};
*/

typedef struct _imglist
{
	SDL_Surface *img;			/* 読み込んだサーフェイスを保持 */
	struct _imglist *next;		/* 次の画像へのリストポインタ */
	int refcount;				/* 同じ画像の参照数 */
	char name[128/*256*/];		/* 名前 */
} MY_IMAGE_LIST;

/*画像キャッシュのリスト*/
static MY_IMAGE_LIST *my_image_list /*= NULL*/;/*←この初期化処理はpspでは正常に動作しないかも？*/

/*extern*/ void init_imglist(void)
{
	my_image_list = NULL;
}

/*---------------------------------------------------------
	画像キャッシュに新規画像を追加
---------------------------------------------------------*/

static void imglist_add(SDL_Surface *s, char *name)
{
	MY_IMAGE_LIST *new_list;
	new_list			= mmalloc(sizeof(MY_IMAGE_LIST));
	strcpy(new_list->name,name);
	new_list->refcount	= 1;
	new_list->img		= s;
	if (NULL == my_image_list)
	{
		my_image_list	= new_list;
		new_list->next	= NULL;
	}
	else
	{
		new_list->next	= my_image_list;
		my_image_list	= new_list;
	}
}

/*---------------------------------------------------------
	画像キャッシュに同じ画像がないか検索
---------------------------------------------------------*/

extern int tiny_strcmp(char *aaa, const char *bbb);
static SDL_Surface *imglist_search(char *name)
{
	MY_IMAGE_LIST *i = my_image_list;
	while (NULL != i)
	{
		if (0 == tiny_strcmp(name, i->name))
		{
			i->refcount++;
			return (i->img);
		}
		i = i->next;
	}
	return (NULL);
}

/*---------------------------------------------------------
	画像キャッシュを開放
	メモリが足りなくなったので画像キャッシュ内で使ってないものを開放
---------------------------------------------------------*/

static void imglist_garbagecollect(void)
{
	MY_IMAGE_LIST *s = my_image_list;
	MY_IMAGE_LIST *p = NULL;
	MY_IMAGE_LIST *n = NULL;
	while (NULL != s)
	{
		n = s->next;
		if (s->refcount == 0)
		{
			if (p == NULL)
			{
				my_image_list = n;
			}
			else
			{
				p->next = n;
			}
		//	SDL_FreeSurface(s->img);
			free (s);
		}
		else
		{
			p = s;
		}
		s = n;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

SDL_Surface *loadbmp0(char *filename, int use_alpha, int use_chache)
{
	char fn[128/*64 50*/];
//	strcpy(fn, data_dir);	strcat(fn, "/");
	strcpy(fn, DIRECTRY_NAME_DATA "/");
	strcat(fn, filename);
//
	SDL_Surface *s1;
	SDL_Surface *s2;
//
	s1 = imglist_search(fn);
	if ( NULL != s1 )
	{
		return (s1);
	}
	//if ((s1=SDL_LoadBMP(fn))==NULL)
//
	s1 = IMG_Load(fn);
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant load image %s:\n"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	if (use_alpha)
	{
		s2 = SDL_DisplayFormatAlpha(s1);	// α値を持ったsurface
	}
	else
	{
		s2 = SDL_DisplayFormat(s1);
	}
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	imglist_add(s2,fn);
	return (s2);
}
//SDL_Surface *loadbmp1(char *filename/*, int use_alpha*/)
//{
//	return	(loadbmp0(filename, 0, 1));
//}
//SDL_Surface *loadbmp2(char *filename/*, int use_alpha*/)
//{
//	return	(loadbmp0(filename, 1, 1));
//}

/*---------------------------------------------------------
		*sがMY_IMAGE_LIST内にあるか確認をし、あった場合 図形キャッシュリストの参照数を減らす。
	×	*sがMY_IMAGE_LIST内にあるか確認をし、あった場合 xxx_[そのSurfaceのrefcountを0にする。]
---------------------------------------------------------*/

void unloadbmp_by_surface(SDL_Surface *s)
{
	MY_IMAGE_LIST *i = my_image_list;/*図形キャッシュリストの先頭*/
	while (NULL != i)
	{
		if (s == i->img)
		{
			if (0 == i->refcount)
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
		i = i->next;
	}
	CHECKPOINT;
	error(ERR_WARN,"unloadbmp_by_surface: object not found");
}

/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0
void unload_bmp_by_name(char *name)/*sprite.c*/
{
	char fn[64/*50*/];
//	strcpy(fn, data_dir);	strcat(fn, "/");
	strcpy(fn, DIRECTRY_NAME_DATA "/");
	strcat(fn,name);
	MY_IMAGE_LIST *i = my_image_list;
	while (NULL != i)
	{
		if (0 == tiny_strcmp(i->name,fn))
		{
			if (0 == i->refcount)
			{
				CHECKPOINT;
				error(ERR_WARN,"unload_bmp_by_name: refcount for object %s is already zero",i->name);
			}
			else
			{
				i->refcount--;
			}
			return;
		}
		i = i->next;
	}
	CHECKPOINT;
	error(ERR_WARN,"unload_bmp_by_name: object not found");
}
#endif

//#define sign(x) ((x) > 0 ? 1 : ((x) == 0 ? 0 : (-1) ))

#define clip_xmin(pnt) (pnt->clip_rect.x)
#define clip_xmax(pnt) (pnt->clip_rect.x + pnt->clip_rect.w-1)
#define clip_ymin(pnt) (pnt->clip_rect.y)
#define clip_ymax(pnt) (pnt->clip_rect.y + pnt->clip_rect.h-1)

/*---------------------------------------------------------

---------------------------------------------------------*/

/* dont forget to lock surface when using get/putpixel! */
static Uint32 s_getpixel(SDL_Surface *surface, int x, int y)
{
	#if (16 != depth)
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x*bpp;
	#else
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	#endif /* (16 != depth) */
	if ((x >= clip_xmin(surface)) &&
		(x <= clip_xmax(surface)) &&
		(y >= clip_ymin(surface)) &&
		(y <= clip_ymax(surface)) )
	{
		#if (16 != depth)
		switch (bpp)
		{
		case 1:
			return (*p);
		case 2:
		#endif /* (16 != depth) */
			return ((*(Uint16 *)p));
		#if (16 != depth)
		case 3:
			if (SDL_BYTEORDER==SDL_BIG_ENDIAN)
			{	return ((p[0]<<16)|(p[1]<<8)|(p[2]));}
			else
			{	return ((p[0])|(p[1]<<8)|(p[2]<<16));}
		case 4:
			return (*(Uint32 *)p);
	//	default:
	//		return (0);
		}
		#endif /* (16 != depth) */
	}
	//else
	//{
		return (0);
	//}
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	#if (16 != depth)
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x*bpp;
	#else
	Uint8 *p = (Uint8 *)surface->pixels+y*surface->pitch+x+x;
	#endif /* (16 != depth) */
	if ((x >= clip_xmin(surface)) &&
		(x <= clip_xmax(surface)) &&
		(y >= clip_ymin(surface)) &&
		(y <= clip_ymax(surface)) )
	{
		#if (16 != depth)
		switch (bpp)
		{
		case 1:
			*p=pixel;
			break;
		case 2:
		#endif /* (16 != depth) */
			*(Uint16 *)p=pixel;
		#if (16 != depth)
			break;
		case 3:
			if (SDL_BYTEORDER==SDL_BIG_ENDIAN)
			{
				p[0] = (pixel>>16)&0xff;
				p[1] = (pixel>> 8)&0xff;
				p[2] = (pixel	 )&0xff;
			}
			else
			{
				p[2] = (pixel>>16)&0xff;
				p[1] = (pixel>> 8)&0xff;
				p[0] = (pixel	 )&0xff;
			}
			break;
		case 4:
			*(Uint32 *)p=pixel;
		}
		#endif /* (16 != depth) */
	}
}
#if (1)
/*---------------------------------------------------------
	拡大縮小しながら表示する。
---------------------------------------------------------*/
void blit_scaled(SDL_Surface *src, SDL_Rect *src_rct, SDL_Surface *dst, SDL_Rect *dst_rct)
{
	if (SDL_MUSTLOCK(src))	{	SDL_LockSurface(src);}
	if (SDL_MUSTLOCK(dst))	{	SDL_LockSurface(dst);}
	Uint32 color_key;	color_key = src->format->colorkey;
	Sint32 y;
	for (y = 0; y<dst_rct->h; y++)
	{
		Sint32 x;
		for (x = 0; x<dst_rct->w; x++)
		{
			Sint32 u;	u = src_rct->w * x / dst_rct->w;
			Sint32 v;	v = src_rct->h * y / dst_rct->h;
			Uint32 get_color;
			get_color = s_getpixel(src, (u + src_rct->x), (v + src_rct->y) );
			if (get_color != color_key)
			{
				s_putpixel(dst, (x + dst_rct->x), (y + dst_rct->y), get_color);
			}
		}
	}
	if (SDL_MUSTLOCK(src))	{	SDL_UnlockSurface(src);}
	if (SDL_MUSTLOCK(dst))	{	SDL_UnlockSurface(dst);}
}
#endif

/*
void draw_line(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1, Uint32 farbe2)
{
	int dx,dx_sign,dx_abs,px,x;
	int dy,dy_sign,dy_abs,py,y;
	int i;
	dx=x2-x1;	dx_sign=sign(dx);	dx_abs=abs(dx); px=x1;	x=0;
	dy=y2-y1;	dy_sign=sign(dy);	dy_abs=abs(dy); py=y1;	y=0;
	if (SDL_MUSTLOCK(s))	{	SDL_LockSurface(s); 	}
	if (dx_abs>=dy_abs)
	{
		for (i=0;i<dx_abs;i++)
		{
			y+=dy_abs;		if (y>=dx_abs)	{	y-=dx_abs;	py+=dy_sign;	}
			putpixel(s,px,py-1,farbe2);
			putpixel(s,px,py  ,farbe1);
			putpixel(s,px,py+1,farbe2);
			px+=dx_sign;
		}
	}
	else
	{
		for (i=0;i<dy_abs;i++)
		{
			x+=dx_abs;		if (x>=dy_abs)	{	x-=dy_abs;	px+=dx_sign;	}
			putpixel(s,px-1,py,farbe2);
			putpixel(s,px  ,py,farbe1);
			putpixel(s,px+1,py,farbe2);
			py+=dy_sign;
		}
	}
	if (SDL_MUSTLOCK(s))	{	SDL_UnlockSurface(s);	}
}
void draw_line_simple(SDL_Surface *s, int x1, int y1, int x2, int y2, Uint32 farbe1)
{
	int dx,dx_sign,dx_abs,px,x;
	int dy,dy_sign,dy_abs,py,y;
	int i;
	dx=x2-x1;	dx_sign=sign(dx);	dx_abs=abs(dx); px=x1;	x=0;
	dy=y2-y1;	dy_sign=sign(dy);	dy_abs=abs(dy); py=y1;	y=0;
	if (SDL_MUSTLOCK(s))	{	SDL_LockSurface(s); 	}
	if (dx_abs>=dy_abs)
	{
		for (i=0;i<dx_abs;i++)
		{
			y+=dy_abs;		if (y>=dx_abs)	{	y-=dx_abs;	py+=dy_sign;	}
			putpixel(s,px,py,farbe1);
			px+=dx_sign;
		}
	}
	else
	{
		for (i=0;i<dy_abs;i++)
		{
			x+=dx_abs;		if (x>=dy_abs)	{	x-=dy_abs;	px+=dx_sign;	}
			putpixel(s,px,py,farbe1);
			py+=dy_sign;
		}
	}
	if (SDL_MUSTLOCK(s))	{	SDL_UnlockSurface(s);	}
}
*/

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
	if (src->format->BitsPerPixel!=dst->format->BitsPerPixel)
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant handle different pixelformats (yet?)");
	}
	if (src->format->BitsPerPixel!=16)
	{
		CHECKPOINT;
		error(ERR_FATAL,"can only handle 16bit-pixelformat");
	}
	key=src->format->colorkey;

	if (SDL_MUSTLOCK(src))		SDL_LockSurface(src);
	if (SDL_MUSTLOCK(dst))		SDL_LockSurface(dst);
	ps=(Uint16 *)src->pixels+src_rct->y*src->pitch/2+src_rct->x;
	pd=(Uint16 *)dst->pixels+dst_rct->y*dst->pitch/2+dst_rct->x;
	for (y=0;y<src_rct->h;y++)
	{
		sps=ps;
		spd=pd;
		if (y&1)	{	ps++;	pd++;	}
		for (x=0;x<src_rct->w;x+=2)
		{
			if	((x+dst_rct->x>=0) &&
				(x+dst_rct->x<dst->w) &&
				(y+dst_rct->y>=0) &&
				(y+dst_rct->y<dst->h))
			{
				col=*(ps);
				if (col!=key)
				{	*(pd)=col;}
			}
			pd+=2;
			ps+=2;
		}
		ps=sps+src->pitch/2;
		pd=spd+dst->pitch/2;
	}
	if (SDL_MUSTLOCK(src))		SDL_UnlockSurface(src);
	if (SDL_MUSTLOCK(dst))		SDL_UnlockSurface(dst);
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

#if ((1==USE_GU)|| defined(ENABLE_PSP))
//#if (1==USE_GU)

static unsigned int   __attribute__((aligned(16))) gulist[262144];

#endif

#if (1==USE_GU)

#define SLICE_SIZE 64 // change this to experiment with different page-cache sizes

/* 0:頂点カラーとか使わない． */
#define USE_VCOLOR 1/*1*/

// /* Vertex Declarations Begin */
//#define GU_TEXTURE_SHIFT(n)	((n)<<0)
//#define GU_TEXTURE_8BIT		GU_TEXTURE_SHIFT(1) 	0x01
//#define GU_TEXTURE_16BIT		GU_TEXTURE_SHIFT(2) 	0x02
//#define GU_TEXTURE_32BITF 	GU_TEXTURE_SHIFT(3) 	0x03
//#define GU_TEXTURE_BITS		GU_TEXTURE_SHIFT(3) 	0x03

//	*	- GU_COLOR_5650 - 16-bit color (R5G6B5A0)
//	*	- GU_COLOR_5551 - 16-bit color (R5G5B5A1)
//	*	- GU_COLOR_4444 - 16-bit color (R4G4B4A4)
//	*	- GU_COLOR_8888 - 32-bit color (R8G8B8A8)

//#define GU_COLOR_SHIFT(n) ((n)<<2)
//#define GU_COLOR_RES1 	GU_COLOR_SHIFT(1)	0x04
//#define GU_COLOR_RES2 	GU_COLOR_SHIFT(2)	0x08
//#define GU_COLOR_RES3 	GU_COLOR_SHIFT(3)	0x0c
//#define GU_COLOR_5650 	GU_COLOR_SHIFT(4)	0x10
//#define GU_COLOR_5551 	GU_COLOR_SHIFT(5)	0x14
//#define GU_COLOR_4444 	GU_COLOR_SHIFT(6)	0x18
//#define GU_COLOR_8888 	GU_COLOR_SHIFT(7)	0x1c
//#define GU_COLOR_BITS 	GU_COLOR_SHIFT(7)	0x1c


//#define UNKNOWN_MY_FLAGS (GU_COLOR_5650)

	#if (1==USE_VCOLOR)
		/* 一般的な、頂点カラーとか使う場合。 */
	//	#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	#else
		/* 註：頂点カラー使わない場合は、間違えて「GU_COLOR_5551」付けると動かんから注意。 */
	//	#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#define TEXTURE_FLAGS		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	#endif

#define USE_GU_ONLY 0

//#if (1==USE_GU_ONLY)
//#else
struct Vertex_uvcxyz
{
	unsigned short u;
	unsigned short v;
	#if (1==USE_VCOLOR)
	unsigned short color;
	#endif
	short x;
	short y;
	short z;
};
//#endif

#if (1==USE_GU_ONLY)
struct Point_Vertex
{
	/*flo at*/unsigned short x;
	/*flo at*/unsigned short y;
	/*flo at*/unsigned short z;
};
#endif

#endif /* (1==USE_GU) */

#if (1==USE_GU)
static SDL_Surface *SDL_VRAM_SCREEN;
#endif

#if (1==USE_GU)
static UINT16 *render_image;
static UINT16 *render_image_back;
static UINT16 *render_image_tex;
#endif

void psp_video_init(void)
{
	if (atexit(SDL_Quit))
	{
		CHECKPOINT;
		error(ERR_WARN,"atexit dont returns zero");
	}
	SDL_VRAM_SCREEN = SDL_SetVideoMode(
		PSP_WIDTH480,
		PSP_HEIGHT272,
		/*int depth 		=*/ SDL_5551_15/*PSP_DEPTH16*/,
		/*UINT32 videoflags =*/ (SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL)
	//	/*UINT32 videoflags =*/ (SDL_FULLSCREEN /*| SDL_DOUBLEBUF*/ | SDL_HWSURFACE | SDL_HWPALETTE | SDL_HWACCEL)
		);
	if (NULL == SDL_VRAM_SCREEN)
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant open screen: "/*"%s", SDL_GetError()*/);
	}
	#if (1==USE_GU)
	//#define SDL_BUF_WIDTH512 (512)
	sdl_screen[SDL_00_SCREEN] = SDL_CreateRGBSurface(
		/*SDL_SWSURFACE*/SDL_HWSURFACE,/*VRAMへ*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*sdl_screen[SDL_00_SCREEN]->w*/
		PSP_HEIGHT272,/*sdl_screen[SDL_00_SCREEN]->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
		if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_SCREEN]); }
		render_image = (UINT16 *)sdl_screen[SDL_00_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_00_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_SCREEN]);	}
	#endif
//	sdl_screen[SDL_01_BACK_SCREEN] = NULL;
	sdl_screen[SDL_01_BACK_SCREEN] = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*メインメモリへ*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
	#if (1==USE_GU)
		if (SDL_MUSTLOCK(sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_01_BACK_SCREEN]);	}
		render_image_back = (UINT16 *)sdl_screen[SDL_01_BACK_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_01_BACK_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_01_BACK_SCREEN]); }
	#endif
	#if (0)
	if (NULL == sdl_screen[SDL_01_BACK_SCREEN])
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant create SDL_Surface: "/*"%s", SDL_GetError()*/);
	}
	#endif
	sdl_screen[SDL_02_TEX_SCREEN] = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*メインメモリへ*/
		SDL_BUF_WIDTH512/*PSP_WIDTH480*/,/*screen->w*/
		PSP_HEIGHT272,/*screen->h*/
		PSP_DEPTH16,/*SDL_VRAM_SCREEN->format->BitsPerPixel*/
		/*0x001f*/PSP_SCREEN_FORMAT_RMASK/*SDL_VRAM_SCREEN->format->Rmask*/,	/*5*/
		/*0x07e0*/PSP_SCREEN_FORMAT_GMASK/*SDL_VRAM_SCREEN->format->Gmask*/,	/*6*/
		/*0xf800*/PSP_SCREEN_FORMAT_BMASK/*SDL_VRAM_SCREEN->format->Bmask*/,	/*5*/
		/*0x0000*/PSP_SCREEN_FORMAT_AMASK/*SDL_VRAM_SCREEN->format->Amask*/);	/*0*/
	#if (1==USE_GU)
		if (SDL_MUSTLOCK(sdl_screen[SDL_02_TEX_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_02_TEX_SCREEN]); }
		render_image_tex = (UINT16 *)sdl_screen[SDL_02_TEX_SCREEN]->pixels;
		if (SDL_MUSTLOCK(sdl_screen[SDL_02_TEX_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_02_TEX_SCREEN]); }
	#endif


	#if 0//defined(ENABLE_PSP)
	sceGuInit();
	sceGuStart(GU_DIRECT, gulist);

	//{
	//	sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0x88000, BUF_WIDTH512);
		sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0, BUF_WIDTH512);
	//	sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x00000, BUF_WIDTH512);
	//	sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, BUF_WIDTH512);
	//	sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x40000, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x88000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	//}

	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/*vsync*/
	sceGuDisplay(GU_TRUE/*1*/);/*画面ON*/
	/* ここまで初期設定 */
	#endif

//	#if ((1==USE_GU)|| defined(ENABLE_PSP))
	#if (1==USE_GU)
	//# /* カスタムライブラリかGuを使う場合 */
	/* ----- GU initialise */
	sceGuInit();
	#if 0
	sceGuDisplay(GU_FALSE);/*画面OFF*/	/* 無くても殆ど同じ(あった方が初期化は極微妙に速いのかも知れない) */
	#endif
	//# /* Guを使う場合 */
	//#if (1==USE_GU)
	sceGuStart(GU_DIRECT, gulist);
	//#endif /*(1==USE_GU)*/
	#if 1//(16 == SCREEN_DEPTH/*depth*/)/*15 bit(5551), 16 bit(5650) */
	//{
		sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x88000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	//}
	#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
	//{
		sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUF_WIDTH);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x110000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	//}
	#endif /* SCREEN_DEPTH */
	sceGuOffset(2048 - (PSP_WIDTH480 / 2), 2048 - (PSP_HEIGHT272 / 2));
	sceGuViewport(2048, 2048, PSP_WIDTH480, PSP_HEIGHT272);

	#if (0==USE_ZBUFFER)
	sceGuDepthRange(0xc350,0x2710);/* (???) */
	#endif/* (0==USE_ZBUFFER) */
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);
	sceGuEnable(GU_SCISSOR_TEST);

	#if 1/*???*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);		/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuDepthFunc(GU_GEQUAL);
	//sceGuEnable(GU_DEPTH_TEST);		/*sceGuDisable(GU_DEPTH_TEST);noiz07*/
	#endif/*???*/

	#if 1/*???*/
	/*「アルファブレンディング(透明度付き、色重ね合わせ)」*/
	sceGuAlphaFunc(GU_ALWAYS,0,0xff);//sceGuAlphaFunc(GU_LEQUAL, 0, 0x01);
	sceGuEnable(GU_ALPHA_TEST); 	/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);			/* 有効 */
	//sceGuDisable(GU_ALPHA_TEST);			/* 無効 */	/*noiz07*/
	#endif/*???*/

	#if (1==USE_ZBUFFER)
	//sceGuEnable(GU_DEPTH_TEST);			/* 有効 */
	//sceGuDisable(GU_DEPTH_TEST);			/* 無効 */	/*noiz07*/
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	sceGuFrontFace(GU_CW);
	#if 0/*???*/
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	#endif/*???*/

	#if 0/*???*/
	sceGuDisable(GU_BLEND);
	#else
	sceGuEnable(GU_BLEND);			/* アルファブレンディング有効 */
	#endif/*???*/

	#if 1
	/* テクスチャー有効モードでは、線や面等のフラットポリゴンが使えない */
	sceGuEnable(GU_TEXTURE_2D); 	/* テクスチャー有効 */
	#else
	/* テクスチャー無効モードでは、線や面等のフラットポリゴンが使える(テクスチャーポリゴン、2D画像転送等は出来ない) */
	sceGuDisable(GU_TEXTURE_2D);	/* テクスチャー無効 */
	#endif

	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);//Z_BUFFER
/*??*/	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);	/*noiz07*/
	#endif/* (1==USE_ZBUFFER) */
//	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);	/*noiz07*/


	#if 1//1
	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	//# /* Guを使う場合 */
	#if (1==USE_GU)
	sceGuTexImage(0, 512, 512, 512, render_image);
	#endif /*(1==USE_GU)*/
//	//sceGuTexFlush();
	//#if (0)
			//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*半透明*/
	//#else
		//		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);/*半透明不可？*/	/*GU_TCC_RGB*/ /*noiz07*/
	//#endif //(1==USE_MAX_TRANS)
				sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdouble buffer???*/

//	sceGuAmbientColor(0/*BGMAIN_COLOR*/);
	#endif

				sceGuTexFilter(GU_LINEAR, GU_LINEAR);
				sceGuTexScale(1.0f, 1.0f);//sceGuTexScale(1,1);
				sceGuTexOffset(0.0f, 0.0f);

//
	/* - テクスチャ転送コマンド */
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
//
	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/*vsync*/
	sceGuDisplay(GU_TRUE/*1*/);/*画面ON*/
	/* ここまで初期設定 */

//	sceDisplayWaitVblankStart();/*vsync*/

	#endif /* (1==USE_GU) */

}

#if (1==USE_GU)
static void rect_trans_test(void)
{
	unsigned int j;
	struct Vertex_uvcxyz	*vertices;
	for (j = 0; (j + SLICE_SIZE) <= /*480*/512/*512*/; j = j + SLICE_SIZE)
	{
		vertices = (struct Vertex_uvcxyz*)sceGuGetMemory(2 * sizeof(struct Vertex_uvcxyz));
		vertices[0].u = j;
		vertices[0].v = 0;
		vertices[0].x = j/**480/512*/;
		vertices[0].y = 0;
		vertices[0].z = 0;
//
		vertices[1].u = (j+SLICE_SIZE);
		vertices[1].v = 272/*320*/;
		vertices[1].x = (j+SLICE_SIZE)/**480/512*/;
		vertices[1].y = 272;
		vertices[1].z = 0;
		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, NULL, vertices);
	}
}
#endif

#define USE_VSYNC			(0)
#define USE_MAX_GU_TIME 	(0)

void vbl_draw_screen(void)
{
	#if (1==USE_GU)
//	fps_show();

	#if (0!=USE_MAX_GU_TIME)
	/* [Gu時間を最大に取るタイプ](色々制限あり) */
//	sceGuFinish();
	sceGuSync(0, 0);
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif
	/* ここから Gu */
	/* --- スクリーンアップデート */
	sceGuStart(GU_DIRECT, gulist );

//	sceDisplayWaitVblankStart();/*vsync*/
//	SDL_BlitSurface(sdl_screen[SDL_00_SCREEN], NULL, SDL_VRAM_SCREEN, NULL);
//	{
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*256) );/*test*/
//	//	memcpy( (UINT16 *)((UINT32) 0x44000000), render_image, (512*272*2) );
//	}
//	SDL_Flip(SDL_VRAM_SCREEN);

	//#if (0==USE_MAX_TRANS)
	sceKernelDcacheWritebackAll();
	//#endif // (0==USE_MAX_TRANS)

	//#if (1==USE_GU_ONLY)
	//#else
	//sprite_display_aaa();
	rect_trans_test();
	//#endif
	/* - パケット積み、ここまで */
	sceGuFinish();
//
	#if (0==USE_MAX_GU_TIME)
	/* [通常タイプ]  */
	sceGuSync(0, 0);/* (ここまで Gu) */
	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	sceGuSwapBuffers();
	#endif /* (0==USE_MAX_GU_TIME) */
//
	#else
	//fps_show();
	SDL_Flip(sdl_screen[SDL_00_SCREEN]);
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
	//SDL_UpdateRect(sdl_screen[SDL_00_SCREEN], 0, 0, 0, 0);
	#endif
//
	//fps_newframe();
	//voice_play_vbl();
	keyboard_poll();/*キー入力(処理の都合上、ここに移動)*/
}
