
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

#include "game_main.h"


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
/*extern*/ int sin_tbl512[SINTABLE_SIZE];

/* この関数はただの初期化。命令キャッシュに乗らないよ */
void init_math(void)
{
	unsigned int i;
	for (i=0; i<SINTABLE_SIZE; i++)
	{
	//	sin_tbl512[i] = (int)(sinf( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
		sin_tbl512[i] = (int)(sin( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
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

static void do_input_vbl(void)
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
		error(ERR_WARN, "can't alloc %d bytes, trying garbage collection",size);
		imglist_garbagecollect();
		ptr = malloc(size);
		if (NULL == ptr)
		{
			error(ERR_FATAL, "I'm sorry, but you're out of memory!");
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
		error(ERR_FATAL, "cant load image %s:\n"/*" %s"*/,fn/*,SDL_GetError()*/);
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
		error(ERR_FATAL, "cant convert image %s to display format:"/*" %s"*/,fn/*,SDL_GetError()*/);
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
				error(ERR_WARN, "unloadbmp_by_surface: refcount for object %s is already zero",i->name);
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
	error(ERR_WARN, "unloadbmp_by_surface: object not found");
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
				error(ERR_WARN, "unload_bmp_by_name: refcount for object %s is already zero",i->name);
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
	error(ERR_WARN, "unload_bmp_by_name: object not found");
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
		error(ERR_FATAL, "cant handle different pixelformats (yet?)");
	}
	if (src->format->BitsPerPixel!=16)
	{
		CHECKPOINT;
		error(ERR_FATAL, "can only handle 16bit-pixelformat");
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
		error(ERR_FATAL, "cant copy screen");
	}
	if ((screen=SDL_SetVideoMode(WIDTH,HEIGHT,depth,videoflags))==NULL) {
		CHECKPOINT;
		error(ERR_FATAL, "cant open change fullscreen/window: %s",SDL_GetError());
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

	error(ERR_DEBUG, "============ SDL VideoInfo ============");

	s=SDL_GetVideoInfo();

	if (SDL_VideoDriverName(driver,256)==NULL)
		error(ERR_WARN, "couldn't get video driver name");
	else
		error(ERR_DEBUG, "Video Driver: %s",driver);

	error(ERR_DEBUG, "BitsPerPixel: %d",s->vfmt->BitsPerPixel);
	if (s->vfmt->palette==NULL) {
		error(ERR_DEBUG, "R Mask      : 0x%.8x",s->vfmt->Rmask);
		error(ERR_DEBUG, "G Mask      : 0x%.8x",s->vfmt->Gmask);
		error(ERR_DEBUG, "B Mask      : 0x%.8x",s->vfmt->Bmask);
	}
	error(ERR_DEBUG, "HW Surface  : %savailable",s->hw_available ? "" : "not ");
	error(ERR_DEBUG, "Win-Manager : %savailable",s->wm_available ? "" : "not ");
	error(ERR_DEBUG, "H->H Blit   : %savailable",s->blit_hw ? "" : "not ");
	error(ERR_DEBUG, "H->H Blit CC: %savailable",s->blit_hw_CC ? "" : "not ");
	error(ERR_DEBUG, "H->H Blit A : %savailable",s->blit_hw_A ? "" : "not ");
	error(ERR_DEBUG, "S->H Blit   : %savailable",s->blit_sw ? "" : "not ");
	error(ERR_DEBUG, "S->H Blit CC: %savailable",s->blit_sw_CC ? "" : "not ");
	error(ERR_DEBUG, "S->H Blit A : %savailable",s->blit_sw_A ? "" : "not ");
	error(ERR_DEBUG, "Color Fill  : %savailable",s->blit_fill ? "" : "not ");
	error(ERR_DEBUG, "Video-Mem   : %d",s->video_mem);

	error(ERR_DEBUG, "Available Fullscreen modes:");
	modes=SDL_ListModes(NULL,SDL_FULLSCREEN);
	for (i=0;modes[i];i++) {
		error(ERR_DEBUG, "%d: %dx%d",i+1,modes[i]->w,modes[i]->h);
	}
	error(ERR_DEBUG, "Available HW-surfaces modes:");
	modes=SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_HWSURFACE);
	for (i=0;modes[i];i++) {
		error(ERR_DEBUG, "%d: %dx%d",i+1,modes[i]->w,modes[i]->h);
	}
	error(ERR_DEBUG, "=======================================");
}
*/

/*------------------------------------------------------------- */

#if (1==USE_GU)

//#define USE_SDL_image 1
#define USE_SDL_image 0
#if (1==USE_SDL_image)
	//#include "_SDL_image.h"//#include "_SDL_image.h"
	#define MY_DIB_SURFACE	SDL_Surface
	#define MY_DIB_DATA 	pixels
#else
	#include "graphics.h"
	#define MY_DIB_SURFACE	Image
//	#define MY_DIB_DATA 	data
	#define MY_DIB_DATA 	pixels
#endif


//#define USE_16BIT_BLIT 0
#define USE_16BIT_BLIT 1

//#define USE_BLIT_COLOR_BIT 32
#define USE_BLIT_COLOR_BIT 16

/*  */
//#if (1==USE_16BIT_BLIT)
//	#define USE_COLOR_16_AND_32 0
//#endif
	#define USE_COLOR_16_AND_32 1


/* 弾テクスチャの幅 */
#define TW128 128
/* 弾テクスチャの高さ */
#define TH128 128

/* 弾テクスチャを描画する場合に、描画ワークエリアの幅 */
#define WK512 512


/* 大きなスプライトを描画するときの分割サイズ */
#define SLICE_SIZE	(64)

/* 0:頂点カラーとか使わない． */
#define USE_VCOLOR 1/*1*/

/* --- GU 描画のためのパケットサイズ */
#define PACKET_SIZE  (262144)

/* gulist設定が小さすぎると都合が悪い */
#if (PACKET_SIZE<(512*512)) /* shere short16==(512*512/2*short) and int32 ==(512*512*int) */
	#undef	PACKET_SIZE
	#define PACKET_SIZE  (512*512)
#endif

static	unsigned int __attribute__((aligned(16))) gulist[PACKET_SIZE];

//#define UNKNOWN_MY_FLAGS (GU_COLOR_5650)

	#if (1==USE_VCOLOR)
		/* 頂点カラーを使う場合。一般的。 */
		#if (16==USE_BLIT_COLOR_BIT)
			/* 15/16bit描画 */
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
//
		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS5650	(GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		//	#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_4444 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#else
			/* 32bit描画 */
			#define TEXTURE_FLAGS5650	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS4444	(GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)
		#endif
	#else
		/* 頂点カラーを使わない場合。「GU_COLOR」を付けると動かない。 */
		//	#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS		(									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
	#endif







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


//#define USE_SWIZZLE 0
#define USE_SWIZZLE 1

/* --- 管理する最大テクスチャー数 */
#define TEXTUREMAX	2


/* -------------------------------------------------------------- */
/* --- スクリーン管理											  */
/* -------------------------------------------------------------- */

static	TGameSprite  ggg_Sprites[SPRITEMAX];
static	MY_DIB_SURFACE *my_texture[TEXTUREMAX];

typedef struct
{
	unsigned short	u;
	unsigned short	v;
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
			unsigned short	color;/*GU_COLOR_5650 GU_COLOR_5551 GU_COLOR_4444*/
		#else
			unsigned int	color;
		#endif
	#endif
	short			x;
	short			y;
	short			z;
} Vertex_uvcxyz;

typedef struct
{
	/*float*/unsigned short x;
	/*float*/unsigned short y;
	/*float*/unsigned short z;
} Point_Vertex;



#endif /* (1==USE_GU) */

#if (1==USE_GU)
static UINT16 *render_image;

static UINT16 *render_image_back;
static UINT16 *render_image_tex;

/*static*/ UINT16 *msg_window_image;

//static UINT16 *bullet_image;
static SDL_Surface *SDL_VRAM_SCREEN;

unsigned int draw_bg_screen;
unsigned int conv_bg_alpha;



static void TGameTexture_Load_Surface(int num, char *filename);//, /*int*/UINT8 true32bit);
void stage_bg_load_surface(void)
{
	char filename[128];
	strcpy(filename, DIRECTRY_NAME_DATA "/bg/backZ_256.png");
	filename[8+DIRECTRY_NAME_LENGTH] = ('0'+/*level*/player_now_stage/*load_stage_number*/ /*player_now_stage*/);
	TGameTexture_Load_Surface( 1, filename);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
	draw_bg_screen = 1;
}

#endif


extern void sendCommandi(int cmd, int argument);
void psp_video_init(void)
{
	draw_bg_screen = 0;
	conv_bg_alpha=0xffffffff;
//	conv_bg_alpha=0x7f7f7f7f;
//
	unsigned int i;
	if (atexit(SDL_Quit))
	{
		CHECKPOINT;
		error(ERR_WARN, "atexit dont returns zero");
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
		error(ERR_FATAL, "cant open screen: "/*"%s", SDL_GetError()*/);
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
		error(ERR_FATAL, "cant create SDL_Surface: "/*"%s", SDL_GetError()*/);
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





//	#if ((1==USE_GU)|| defined(ENABLE_PSP))
	#if (1==USE_GU)
	//# /* カスタムライブラリかGuを使う場合 */



#if (1==USE_16BIT_BLIT)
	#define  SCREEN_DEPTH	16
#else
	#define  SCREEN_DEPTH	32
#endif

	/* setup GU initialise */
	sceGuInit();
	#if 0
	/* 無くても殆ど同じ */
	sceGuDisplay(GU_FALSE);/*画面OFF*/
	#endif
	sceGuStart(GU_DIRECT, gulist);
	#if (16 == SCREEN_DEPTH/*depth*/)/*16 bit(5650), 15 bit(5551), 12 bit(4444) */
	//	sceGuDrawBuffer(GU_PSM_5551, (void*)0, BUF_WIDTH512);
		sceGuDrawBuffer(SDL_GU_PSM_0000, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x44000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x88000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#else // (32 == SCREEN_DEPTH/*depth*/)/*32 bit(8888) */
		sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUF_WIDTH512);
		sceGuDispBuffer(PSP_WIDTH480, PSP_HEIGHT272, (void*)0x88000, BUF_WIDTH512);
		#if (1==USE_ZBUFFER)
		sceGuDepthBuffer((void*)0x110000, BUF_WIDTH512);
		#endif /* (1==USE_ZBUFFER) */
	#endif /* SCREEN_DEPTH */
	sceGuOffset(2048 - (PSP_WIDTH480 / 2), 2048 - (PSP_HEIGHT272 / 2));
	sceGuViewport(2048, 2048, PSP_WIDTH480, PSP_HEIGHT272);

	#if (1==USE_ZBUFFER)
	/* psp の z値は 65535(近く) から 0(遠く)で、Z値が通常と逆。 */
	/* このz値は unsigned short なので、少しはみ出すと、回り込んで変な描画になってしまう。 */
	/* そこで、回り込まないように、適当に余裕分を見繕う。PSPSDK のサンプルでは */
	/* 0xc350(50000) 近く側の境界面、0x2710(10000)遠く側の境界面 となっているが、適当な値なら何でもよい */
	/* sceGuDepthRange(0xc350, 0x2710); */
	#define  Z_CLIP_LENGTH	(10000)
	sceGuDepthRange((0xffff-Z_CLIP_LENGTH), Z_CLIP_LENGTH);
	#endif /* (1==USE_ZBUFFER) */
	sceGuScissor(0, 0, PSP_WIDTH480, PSP_HEIGHT272);
	sceGuEnable(GU_SCISSOR_TEST);

	#if 1/*???*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);		/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuDepthFunc(GU_GEQUAL);
	//sceGuEnable(GU_DEPTH_TEST);		/*sceGuDisable(GU_DEPTH_TEST);noiz07*/
	#endif/*???*/

#if 000/*???*/
	#if 1/*???*/
	/*「アルファブレンディング(透明度付き、色重ね合わせ)」*/
	sceGuAlphaFunc(GU_ALWAYS,0,0xff);//sceGuAlphaFunc(GU_LEQUAL, 0, 0x01);
	sceGuEnable(GU_ALPHA_TEST); 	/*sceGuDisable(GU_ALPHA_TEST);noiz07*/
	//sceGuAlphaFunc(GU_ALWAYS,0,0xff);
	//sceGuEnable(GU_ALPHA_TEST);			/* 有効 */
	//sceGuDisable(GU_ALPHA_TEST);			/* 無効 */	/*noiz07*/
	#endif/*???*/
#endif/*000???*/

	#if (1==USE_ZBUFFER)
	//sceGuEnable(GU_DEPTH_TEST);			/* 有効 */
	//sceGuDisable(GU_DEPTH_TEST);			/* 無効 */	/*noiz07*/
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

	#if 1
	/* カリングモード(裏向きポリゴンは描画しない)で、有効座標系(右手座標系にするか左手座標系にするか)を決める */
	sceGuEnable(GU_CULL_FACE);	/* カリング有効(裏向きポリゴンは描画しない) */
	sceGuFrontFace(GU_CW);		/* 右手座標系(OpenGL 標準?) */
//	sceGuFrontFace(GU_CCW); 	/* 左手座標系(Direct X 標準?) */
	#endif

#if 1/*???*/
	#if 1/*???*/
	sceGuShadeModel(GU_SMOOTH);
	#endif/*???*/
#endif/*1???*/

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

		#if 1
				sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*半透明*/
		#else
				sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);/*半透明不可？*/
		#endif

				sceGuTexFilter(GU_LINEAR, GU_LINEAR);
				sceGuTexScale(1.0f, 1.0f);//sceGuTexScale(1,1);
				sceGuTexOffset(0.0f, 0.0f);
//
	/* 環境色を決める？ */
	sceGuTexEnvColor(	0x00000000);
//	sceGuTexEnvColor(	0x00ffff00);
	sceGuAmbientColor(	0xffffffff);
	sceGuColor(			0xff000000);

//


//	u8	ColorKey	= 1;
//カラーキーを設定する
//	if (ColorKey)
	{
//		if (!sceGuGetStatus(GU_COLOR_TEST))
		{	sceGuEnable(GU_COLOR_TEST);}/*カラーキー有効*/
	//	sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00fefefe);
		sceGuColorFunc(GU_NOTEQUAL, 0/*texptr->colorkey*/, 0x00ffffff);
	//	sendCommandi(216, GU_NOTEQUAL & 0x03);
	//	sendCommandi(217, 0 & 0xffffff);
	//	sendCommandi(218, 0x00fefefe);
	}
//	else
//	{
//		if (sceGuGetStatus(GU_COLOR_TEST))
//		{	sceGuDisable(GU_COLOR_TEST);}/*カラーキー無効*/
//	}
	#if 1
	sceGuEnable(GU_CLIP_PLANES);
	#endif
//
	sceGuFinish();
	sceGuSync(0, 0);

//	sceDisplayWaitVblankStart();/*vsync*/
	sceGuDisplay(GU_TRUE/*1*/);/*画面ON*/
	/* ここまで初期設定 */

//	sceDisplayWaitVblankStart();/*vsync*/

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	/* --- テクスチャマネージャの初期化 */
	//static TGameTexture *TGameTexture_Create(void)
	{
		for (i=0; i<TEXTUREMAX; i++)
		{
			my_texture[i] = NULL;
		}
	}

	/* --- スプライトマネージャの初期化 */
	for (i=0; i<SPRITEMAX; i++)
	{
		ggg_Sprites[i].TextureId	= 0/*-1*/;
	//	ggg_Sprites[i].Texture_ptr	= NULL;
		ggg_Sprites[i].used 		= 0/*FA LSE*/;
		#if (1==USE_ZOOM_XY)
		ggg_Sprites[i].zoom_x256	= 256/*1.0*/;
		ggg_Sprites[i].zoom_y256	= 256/*1.0*/;
		#else //(0==USE_ZOOM_XY)
		ggg_Sprites[i].zoom_xy256	= 256/*1.0*/;
		#endif/* (1==USE_ZOOM_XY) */
		ggg_Sprites[i].rotation_z	= 0/*.0*/;
	}

	/* --- テクスチャの初期化 */
	{
	/* --- テクスチャファイル名 */
	//char* texture_name[] =
	//{
	//	#if 1
			#define TEXTURE_NAME_00 DIRECTRY_NAME_DATA "/tama/bullet.png"
//			#define TEXTURE_NAME_01 "texture02.png"
	//	#else
	//		#define TEXTURE_NAME_00 "texture1.bmp"/*"texture1.png"*/
//	//		#define TEXTURE_NAME_01 "clip1.png"
	//	#endif
	//};
//		#ifdef PSP
	//	TGameTexture_Load_Surface( 0, TEXTURE_NAME_00);//, 1/*TR UE*/ /*FA LSE*/);/*game*/
		TGameTexture_Load_Surface( 0, TEXTURE_NAME_00);//, 0/*TR UE*/ /*FA LSE*/);/*game*/
//		TGameTexture_Load_Surface( 1, TEXTURE_NAME_01);//, 0/*FA LSE*/);/*title*/
//		#else
//		TGameTexture_Load_Surface( 0, TEXTURE_NAME_00);//, 1/*TR UE*/);/*Pure*/
//		TGameTexture_Load_Surface( 1, TEXTURE_NAME_01);//, 1/*TR UE*/);/*Pure*/
//		#endif
	}
	#endif /* (1==USE_GU) */

	/* 入力装置の初期設定 */

	/* の初期設定 */

}
/*-------------*/
/*-------------*/
#define USE_T128_SWIZZLE 0
/* ---------------------------------------- */
/* --- テクスチャをロードする				*/
#if (1==USE_T128_SWIZZLE)
static void convert_swizzle_RGB8888_T128/*512*/
(
		  u8	*out_data,
	const u8	*in_data
)
{
	const u8	*src_y = in_data;
	u32 		*dst  = (u32*)out_data;
	u8 block_y;
	block_y = /*height_blocks*/((TH128/*512*/) >>3/*/  8*/);/*max: 64-1*/
	while (block_y--)
	{
		const u8	*src_x = src_y;
		u8 block_x;
		block_x = /*width_blocks*/((TW128/*512*/*4)	>>4/*/ 16*/);/*max: 32-1*/
		while (block_x--)
		{
			const u32	*src = (u32*)src_x;
			u8 j;
			j=8;	/*max: 8-1*/
			while (j--)
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += /*src_pitch*/(((TW128/*512*/*4)-16)>>2/*/4*/);
			}
			src_x += 16;
		}
		src_y += /*src_row*/((TH128/*512*/*4) <<3/** 8*/);
	}
}
#endif /*(0)*/

//#if 1//(1==USE_COLOR_16_AND_32)
#if (1==USE_COLOR_16_AND_32)
static void convert_swizzle
(
		  u8	*out_data,		/* 出力画像の先頭ポインタ */
	const u8	*in_data,		/* 入力画像の先頭ポインタ */
/* (例えば、512x512テクスチャーの場合) */
	const u32	width_x_size,	/* 幅かけるデーターサイズ */	/*max: 512*4 */
	/* (例えば、ARGB8888 なら 512*4==width_x_size) */
	/* (例えば、ARGB1555 なら 512*2==width_x_size) */
	const u32	height			/* 高さ */		/*max: 512*/
	/* (例えば、512==height) */
)
{
	#if (1==USE_SWIZZLE)
	const u8	*src_y = in_data;
	u32 		*dst  = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		const u8	*src_x = src_y;
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			const u32	*src = (u32*)src_x;
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				src += /*src_pitch*/((width_x_size-16)>>2/* /4 */);
			}
			src_x += 16;
		}
		src_y += /*src_row*/(width_x_size <<3/* *8 */);
	}
	#else
	u32 		*src = (u32*)in_data;
	u32 		*dst = (u32*)out_data;
	u8 block_y;
	for (block_y = 0; block_y < /*height_blocks*/(height >>3/* /8 */); block_y++)/*max: 64-1*/
	{
		u8 block_x;
		for (block_x = 0; block_x < /*width_blocks*/(width_x_size  >>4/* /16 */); block_x++)/*max: 32-1*/
		{
			u8 j;
			for (j = 0; j < 8; j++)/*max: 8-1*/
			{
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
				*(dst++) = *(src++);
			}
		}
	}
	#endif
}
#endif
//#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	//#define RMASK 0xff000000
	//#define GMASK 0x00ff0000
	//#define BMASK 0x0000ff00
	//#define AMASK 0x000000ff
	//#define RMASK16 0x0000f800
	//#define GMASK16 0x000007c0
	//#define BMASK16 0x0000003f
	//#define AMASK16 0x00000001
//#else
	//#define RMASK 0x000000ff
	//#define GMASK 0x0000ff00
	//#define BMASK 0x00ff0000
	//#define AMASK 0xff000000
	#define RMASK16 0x0000001f
	#define GMASK16 0x000003e0
	#define BMASK16 0x00007c00
	#define AMASK16 0x00008000
//#endif

/* ---------------------------------------- */
/* --- テクスチャーの読み込み、登録 		*/
/* ---------------------------------------- */
static void TGameTexture_Load_Surface(
		int num,
		char *filename//,
//		MY_DIB_SURFACE *GameScreen,
		)
{
//TGameTexture *pclass=/*pclass*/ggg_Texture;
	//Image *plane;//
	MY_DIB_SURFACE *plane;
//	MY_DIB_SURFACE *standard;
	void *nonalign;
	unsigned int msize;
	int loop;
/*int*/UINT8 true32bit = 0;
//	standard = 0;
	nonalign = 0;
	msize = 0;
	loop = 0;
	//if (ggg_Texture/*pclass*/ == NULL)
	//{
	//	return;
	//}

	/* ----- テクスチャ番号が不正だったら終了 */
//	if (num < 0) return;
//	if (num > TEXTUREMAX) return;

	int mymap_128 = 128;
	switch (num)
	{
	case 0: mymap_128 = 128;
		break;
	default:
	case 1: mymap_128 = 256;
	}
	#define my_map_TW128 mymap_128
	#define my_map_TH128 mymap_128

	/* ----- 既にテクスチャがあったら解放 */
	if (my_texture[num] != NULL)
	{
		#if (1==USE_SDL_image)
		SDL_FreeSurface(my_texture[num]);
		#else
		my_png_freeImage(my_texture[num]);
		#endif
		my_texture[num] = NULL;
	}

	/* ----- テクスチャーの読み込み */
	#if (1==USE_SDL_image)
	plane = IMG_Load(filename);
//	plane = SDL_LoadBMP(filename);
	#else
	plane = my_png_loadImage(filename);//
	#endif

	if (NULL == plane)
	{
		#if (1)
		CHECKPOINT;
		error(ERR_FATAL, "cant load image %s:"/*" %s"*/,filename/*,SDL_GetError()*/);
		#endif
		my_texture[num] = NULL;
		return;
	}
	//#if 1
	#ifndef PSP
	/* --- Normal SDL work for PC */
	if (true32bit ==  0/*FA LSE*/)/*16bit mode*/
	{
		my_texture[num] = SDL_ConvertSurface(plane,
				GameScreen->format,
				SDL_SWSURFACE|SDL_SRCALPHA);
		if ((plane != NULL) &&
			(plane != my_texture[num]))
		{
			SDL_FreeSurface(plane);
		}
	}
	else/*32bit mode*/
	{
		my_texture[num] = plane;
	}
	#else
	/* --- PSP 向けにテクスチャを加工する */

	my_texture[num] = plane;

	/* --- PSPは 16byte align に無いと 都合が悪い(遅い?) らしい。 */
	/* --- そこで memalign(); を使い、メモリ境界を合わせる。 */
	nonalign = my_texture[num]->MY_DIB_DATA;

	#if (1==USE_SDL_image)
	msize = (my_texture[num]->w * my_texture[num]->h)
		 * my_texture[num]->format->BytesPerPixel;
	#else
	msize = (/*w*/my_map_TW128/*512*/ * /*h*/my_map_TH128/*512*/) * /*bpp*/4;
	#endif

#if (1==USE_COLOR_16_AND_32)
	/* --- PSP で簡易16ビット色を指定した場合は16bitに落とす */
	#if (1==USE_SDL_image)
	if ((true32bit ==  0/*FA LSE*/) &&
		(my_texture[num]->format->BytesPerPixel == 4))	/* PSP16bit mode && DIB==32bit color */
	#else
	if ((true32bit ==  0/*FA LSE*/) &&
		(/*BytesPerPixel*/4 == 4))	/* PSP16bit mode && DIB==32bit color */
	#endif
	//if (0)
	{
	unsigned short *pixdst;
	unsigned long  *pixsrc;
		/* --- 16bit色に 減色して保持 */
		my_texture[num]->pixels = (void*)memalign(16, (msize>>1)/*(msize / 2)*/);
		#if (1==USE_SDL_image)
		msize = (my_texture[num]->w * my_texture[num]->h);
		#else
		msize = (my_map_TW128 * my_map_TH128);
		#endif
		pixdst = (unsigned short *)/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/;
		pixsrc = (unsigned long *)nonalign;
		for (loop=0; loop<msize; loop++)
		{
			#if 0
			/* --- 15bit色に 減色して保持 (color key 使用可) */
			*pixdst =
				((*pixsrc & 0x80000000) >> 16) |
				((*pixsrc & 0x00f80000) >>	9) |
				((*pixsrc & 0x0000f800) >>	6) |
				((*pixsrc & 0x000000f8) >>	3);
		//	*pixdst = MAKECOL15A(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc), GETA32(*pixsrc));
			#else
			/* --- 16bit色に 減色して保持 (color key 使用不可) */
			*pixdst = MAKECOL16(GETR32(*pixsrc), GETG32(*pixsrc), GETB32(*pixsrc));
			#endif
			pixdst++;
			pixsrc++;
		}

	//	unsigned short *pixsrc2;
	//	pixsrc2 = (unsigned short *)pclass->bitmap[num]->pixels;
	//	pixdst = (unsigned short *)gulist/*convert_works*/;
	//	for (loop=0; loop<msize; loop++)
	//	{
	//		(*pixdst) = (*pixsrc2);
	//		pixdst++;
	//		pixsrc2++;
	//	}
		#if (1==USE_SDL_image)
		my_texture[num]->format->BytesPerPixel =  2;
		my_texture[num]->format->BitsPerPixel  = 16;
		my_texture[num]->format->Rmask = /*PSP_SCREEN_FORMAT_RMASK*/RMASK16/*RMASK16*/;
		my_texture[num]->format->Gmask = /*PSP_SCREEN_FORMAT_GMASK*/GMASK16/*GMASK16*/;
		my_texture[num]->format->Bmask = /*PSP_SCREEN_FORMAT_BMASK*/BMASK16/*BMASK16*/;
		my_texture[num]->format->Amask = /*PSP_SCREEN_FORMAT_AMASK*/AMASK16/*AMASK16*/;
		#endif
	// 16bit色
		convert_swizzle(
			(u8*)my_texture[num]->MY_DIB_DATA,
			(const u8*)gulist/*convert_works*/,
			/* texture width x 2 */  my_map_TW128/*512*/*2, 	/*short だから 2倍*/
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
	}
	else/*32bit mode*/
#endif
	{
		/* --- 32bit色 通常モード */
		my_texture[num]->MY_DIB_DATA = (void*)memalign(16, msize);
		memcpy(/*convert_works*/gulist/*pclass->bitmap[num]->pixels*/, nonalign, msize);
		#if (1==USE_T128_SWIZZLE)/*0*/
	// 32bit色
	//	convert_swizzle(
		convert_swizzle_RGB8888_T128/*512*/(
			(u8*)my_texture[num]->MY_DIB_DATA,
			(const u8*)gulist/*convert_works*/
	//	,
	//		/* texture width x 4 */ 512*4,	/*int だから 4倍*/
	//		/* texture height	 */ 512
		);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#else
	// 32bit色
		convert_swizzle(
			(u8*)my_texture[num]->MY_DIB_DATA,
			(const u8*)gulist/*convert_works*/,
			/* texture width x 4 */  my_map_TW128/*512*/*4, 	/*int だから 2倍*/
			/* texture height	 */  my_map_TH128/*512*/);
		// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
		#endif
	}
	free(nonalign);
	#endif
}



/* ---------------------------------------- */
/* --- スプライトの表示(画面転送)			*/

#if (1==USE_ZBUFFER)
	/* ハードウェアーでソート */
	#define SPR_PRIORITY	(spr->priority)
#else
	/* ソフトウェアーで Zソート */
	#define SPR_PRIORITY	(0)
#endif
/* --- スプライトを一枚スクリーンに貼り付ける */
static void Render_Rect(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- 表示スイッチ */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;

	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

	/* --- スプライトを一枚描画する */
	/* ある程度の大きさを持ったスプライトは短冊状に分割しないと */
	/* とてつもなく遅くなる */

	unsigned int/*short*/ w_rest;
	w_rest = spr->w + SLICE_SIZE;

	unsigned int/*short*/ count2;
	#if 0
	//count2 = (((spr->w / SLICE_SIZE)+1)*2);
	#else
	count2 = 0;
//	for (j=0; (j+SLICE_SIZE)<=(/*480*/spr->w/*512*/); j+=SLICE_SIZE)
	unsigned int/*short*/ i;
	i = 0;
	do
	{
		count2 += 2;
		i += SLICE_SIZE;
		w_rest -= SLICE_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	for (; i<count2; )
	{
	//	if ((i1+1) < count)
	//	if (((i)+1*2) < (count2))	{}	else		/* 最後 */
		if (i==(count2-2))		/* 最後 */
		{
			w_size = w_rest;
		}
	//
		vertices[(i)].u = (spr->tx + pos);
		vertices[(i)].v = spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color = blendlevel;
		#endif
		vertices[(i)].x = (x_pos + pos);
		vertices[(i)].y = y_pos;
		vertices[(i)].z = SPR_PRIORITY;
	//
		vertices[(i)+1].u	= (spr->tx + pos) + w_size;
		vertices[(i)+1].v	= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color = blendlevel;
		#endif
		vertices[(i)+1].x	= (x_pos + pos) + w_size;
		vertices[(i)+1].y	= y_pos + spr->h;
		vertices[(i)+1].z	= SPR_PRIORITY;
		i += 2;
		pos += SLICE_SIZE;
	}
	/* --- 描画リクエスト */
//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS4444, (count2), /*0*/NULL, vertices);
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count2), /*0*/NULL, vertices);
}


/* ---------------------------------------- */
/* --- スプライトの表示(回転拡大縮小あり) */
static void Render_Zoom_Rot(/*TGameScreen *pclass,*/ TGameSprite *spr)
{
	/* --- 表示スイッチ */
//	if (spr->used ==  0/*FA LSE*/) return;
//	if (spr->Texture == NULL) return;
//
	/* --- 回転拡大前処理 */
	int sin_angle;
	int cos_angle;
	{
		/* 角度は0-65535度なので0-511度へ変換。 */
		unsigned int rotation_angle;
	//	rotation_angle	= ((spr->rotation_z) / (128/*65536/512*/));
		rotation_angle	= ((spr->rotation_z) /*>> (7)*/);
		sin_angle = (sin_tbl512[/*rot_sin*/((/*OFFS_SIN+*/rotation_angle)&(512-1))]/*<<8*/);
		cos_angle = (sin_tbl512[/*rot_cos*/((  OFFS_COS+  rotation_angle)&(512-1))]/*<<8*/);
	}
//
	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

	/* --- スプライトを一枚描画する */
	/* ある程度の大きさを持ったスプライトは短冊状に分割しないと */
	/* とてつもなく遅くなる */

	unsigned int w_rest;
	w_rest = spr->w + SLICE_SIZE;

	unsigned int count4;
	#if 0
	//count4 = (((spr->w / SLICE_SIZE)+1)*4);
	#else
	count4 = 0;
	unsigned int i;
	i = 0;
	do
	{
		count4 += 4;
		i += SLICE_SIZE;
		w_rest -= SLICE_SIZE;
	}
	while ( (i) <(/*480*/spr->w/*512*/));
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count4 * sizeof(Vertex_uvcxyz));
	i = 0;
	unsigned int/*short*/ pos = 0;
	unsigned int/*short*/ w_size;
	w_size	= SLICE_SIZE;
	unsigned int/*short*/ x_pos = ((spr->x256>>8));
	unsigned int/*short*/ y_pos = ((spr->y256>>8));
	for (; i<count4; )
	{
	//	if (((i)+1*4) < (count4))	{}	else		/* 最後 */
		if (i==(count4-4))	/* 最後 */
		{
			w_size = w_rest;
		}
		unsigned int pos_w_size = (pos + w_size);
		vertices[(i)].u 		= spr->tx + pos;
		vertices[(i)].v 		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)].color 	= blendlevel;
		#endif
		vertices[(i)].x 		= x_pos + pos;
		vertices[(i)].y 		= y_pos;
		vertices[(i)].z 		= SPR_PRIORITY;
	//
		vertices[(i)+1].u		= spr->tx + (pos_w_size);
		vertices[(i)+1].v		= spr->ty;
		#if (1==USE_VCOLOR)
		vertices[(i)+1].color	= blendlevel;
		#endif
		vertices[(i)+1].x		= x_pos + (pos_w_size);
		vertices[(i)+1].y		= y_pos;
		vertices[(i)+1].z		= SPR_PRIORITY;
	//
		vertices[(i)+2].u		= spr->tx + pos;
		vertices[(i)+2].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+2].color	= blendlevel;
		#endif
		vertices[(i)+2].x		= x_pos + pos;
		vertices[(i)+2].y		= y_pos + spr->h;
		vertices[(i)+2].z		= SPR_PRIORITY;
	//
		vertices[(i)+3].u		= spr->tx + (pos_w_size);
		vertices[(i)+3].v		= spr->ty + spr->h;
		#if (1==USE_VCOLOR)
		vertices[(i)+3].color	= blendlevel;
		#endif
		vertices[(i)+3].x		= x_pos + (pos_w_size);
		vertices[(i)+3].y		= y_pos + spr->h;
		vertices[(i)+3].z		= SPR_PRIORITY;
	//
		pos += SLICE_SIZE;
		/* --- 回転拡大処理 */
		int center_x;
		int center_y;
		center_x = ((spr->x256>>8)		) + (spr->w >> 1);/*/2*/
		center_y = ((spr->y256>>8)		) + (spr->h >> 1);/*/2*/
		#if (1==USE_ZOOM_XY)
		int zoom_x256;
		int zoom_y256;
	//	zoom_x256 = (spr->zoom_x256) * ((65536/256));	/* 拡大率は0-256倍なので0-65536倍へ変換。 */
	//	zoom_y256 = (spr->zoom_y256) * ((65536/256));	/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_x256 = (spr->zoom_x256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_y256 = (spr->zoom_y256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		#else //(0==USE_ZOOM_XY)
		int zoom_xy256;
	//	zoom_xy256 = (spr->zoom_xy256) * ((65536/256)); /* 拡大率は0-256倍なので0-65536倍へ変換。 */
		zoom_xy256 = (spr->zoom_xy256) /*<< (8)*/;		/* 拡大率は0-256倍なので0-65536倍へ変換。 */
		#endif/* (1==USE_ZOOM_XY) */
		unsigned int j;
		for (j=0; j<4; j++)
		{
			int ifx;
			int ify;
			int irx;
			int iry;
			ifx = vertices[(i)+j].x;
			ify = vertices[(i)+j].y;
			ifx -= center_x;
			ify -= center_y;
//			rx = ((fx * sprite_cos[rot]) - (fy * sprite_sin[rot])) * spr->zoomx;
//			ry = ((fx * sprite_sin[rot]) + (fy * sprite_cos[rot])) * spr->zoomy;
			#if 0
			/* 右が 0度で時計回りの角度系 */
			irx = ((ifx * cos_angle) - (ify * sin_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * sin_angle) + (ify * cos_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
			#if 1
			/* (???)下が 0度で反時計回りの角度系 */
			/* 左が 0度で反時計回りの角度系 */
			irx = ((ifx * sin_angle) - (ify * cos_angle));	irx = (irx >>8/*16*/);	//	rx = rx / (65536.0);
			iry = ((ifx * cos_angle) + (ify * sin_angle));	iry = (iry >>8/*16*/);	//	ry = ry / (65536.0);
			#endif
//
			#if (1==USE_ZOOM_XY)
			irx = (irx * (zoom_x256)>>8);
			iry = (iry * (zoom_y256)>>8);
			#else //(0==USE_ZOOM_XY)
			irx = ((irx * (zoom_xy256))>>8);
			iry = ((iry * (zoom_xy256))>>8);
			#endif/* (1==USE_ZOOM_XY) */
		//	irx = (irx >>8/*16*/);	//	rx = rx / (256.0/*65536.0*/);
		//	iry = (iry >>8/*16*/);	//	ry = ry / (256.0/*65536.0*/);

			irx += center_x;
			iry += center_y;

			vertices[(i)+j].x = (short)irx;
			vertices[(i)+j].y = (short)iry;
		}
		i += 4;
	}
	/* --- 描画リクエスト */
//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (count4), 0, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (count4), 0, vertices);
}


/* ---------------------------------------- */
/* --- フレームタイミングによる全描画		*/
/*extern*/ int debug_view_objs;
static void blit_all_objects(void /*TGameScreen *pclass*/)
{
#if 000
	static /*int*/UINT8 texture_cacheId = 127/*-1*/;
#endif
//const TGameScreen *pclass =g_scr;

	debug_view_objs=0;
	/* --- スプライト描画 */
	//for (i=0; i<SPRITEMAX; i++)
	offs_t i;
	i = SPRITEMAX;
	do
	{
		i--;/*註：先引き*/
		if (/*g_scr.*/ ggg_Sprites[i]./*->*/used == 1/*TR UE*/)
		{
		//	if ( ggg_Sprites[i].TextureId	!= 127/*-1*/)
		//	if ( ggg_Sprites[i].Texture_ptr != NULL)
			{
				debug_view_objs++;
				/* --- テクスチャの転送が必要なら */
#if 000
				if (( ggg_Sprites[i].TextureId	 != texture_cacheId) /*||
					( ggg_Sprites[i].Texture_ptr != texture_cache_ptr)*/)
				{
					texture_cacheId   = ggg_Sprites[i].TextureId;
				MY_DIB_SURFACE *texture_cache_ptr;
				//	texture_cache_ptr = NULL/*0*/;
				//	texture_cache_ptr = ggg_Sprites[i].Texture_ptr;
					texture_cache_ptr = my_texture[texture_cacheId];
					/* - テクスチャ転送コマンド */
					sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

					#if (1==USE_COLOR_16_AND_32)
					if (/*g_scr.*/ texture_cache_ptr->format->BitsPerPixel == 16)
					{
						sceGuTexMode(GU_PSM_5551,0,0,1/*0 swizzle*/);
					}
					else
					#endif
					{
						sceGuTexMode(GU_PSM_8888,0,0,1/*0 swizzle*/);
					}
					sceGuTexImage(0,
						 512/*texture_cache_ptr->w*/,
						 512/*texture_cache_ptr->h*/,
						 512/*texture_cache_ptr->w*/,
						 texture_cache_ptr->MY_DIB_DATA);
				//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
					sceGuTexFilter(GU_LINEAR, GU_LINEAR);
				//	sceGuTexScale(1.0f,1.0f);
				//	sceGuTexOffset(0.0f,0.0f);
				}
#endif
				/* --- スプライトの描画 */
				if (ggg_Sprites[i].used == 1/*TR UE*/)
				{
					//if (ggg_Sprites[i].Texture_ptr != NULL)
					{
						if ((/*g_scr.*/ ggg_Sprites[i]./*->*/rotation_z == 0/*.0*/) &&
							#if (1==USE_ZOOM_XY)
							(/*g_scr.*/ ggg_Sprites[i]./*->*/zoom_x256 == 256/*1.0*/) &&
							(/*g_scr.*/ ggg_Sprites[i]./*->*/zoom_y256 == 256/*1.0*/)
							#else //(0==USE_ZOOM_XY)
							(/*g_scr.*/ ggg_Sprites[i]./*->*/zoom_xy256 == 256/*1.0*/)
							#endif/* (1==USE_ZOOM_XY) */
						)
						{
							/* 回転拡大縮小なし、単純矩形コピー */
							Render_Rect( &ggg_Sprites[i]);
						}
						else
						{
							/* 回転拡大縮小あり */
							Render_Zoom_Rot( &ggg_Sprites[i]);
						}
					}
				}
			}
		}
	} while (0 != i);
}







static	int ggg_SpriteSerial;

/* ---------------------------------------- */
/* --- インクリメントでスプライトを渡す */
TGameSprite *TGameScreen_GetSpriteSerial(void/*TGameScreen *pclass*/)
{
	if (/*pclass*/ggg_SpriteSerial < SPRITEMAX)
	{
		/*pclass*/ggg_SpriteSerial++;
	}
	return (/*pclass*/ /*g_scr->*/&ggg_Sprites[(/*pclass*/ggg_SpriteSerial - 1)]);
}


/* ---------------------------------------- */
/* --- スプライトの全クリア */
void TGameScreen_ClearSprite(void/*TGameScreen *pclass*/)
{
	int i;
	for (i=0; i<SPRITEMAX; i++)
	{
		/*pclass*/ /*g_scr->*/ggg_Sprites[i]./*->*/used =  0/*FA LSE*/;
	}
	/*pclass*/ggg_SpriteSerial = 0;
}


/*---------------------------------------------------------

---------------------------------------------------------*/
extern SPRITE *sprite_list000_head;


typedef struct
{
	unsigned short	u;
	unsigned short	v;
//
	unsigned short	w;
	unsigned short	h;
} Vtama00;
static Vtama00 tama00[(7*8)] =
{
	{  1, 1,  8, 8},// BULLET_MARU8_00_AKA
	{ 10, 1,  8, 8},// BULLET_MARU8_01_YUKARI,
	{ 19, 1,  8, 8},// BULLET_MARU8_02_AOI,
	{ 28, 1,  8, 8},// BULLET_MARU8_03_MIDORI,
	{ 37, 1,  8, 8},// BULLET_MARU8_04_MIDORI,
	{ 46, 1,  8, 8},// BULLET_MARU8_05_MIDORI,
	{ 55, 1,  8, 8},// BULLET_MARU8_06_KIIRO,
	{ 64, 1,  8, 8},// BULLET_MARU8_07_AOI,
//
	{ 73, 1,  6, 6},// BULLET_MINI8_00_AKA,
	{ 80, 1,  6, 6},// BULLET_MINI8_01_AKA,
	{ 87, 1,  6, 6},// BULLET_MINI8_02_KIIRO,
	{ 94, 1,  6, 6},// BULLET_MINI8_03_AKA,
	{101, 1,  6, 6},// BULLET_MINI8_04_KIIRO,
	{108, 1,  6, 6},// BULLET_MINI8_05_AOI,
	{115, 1,  6, 6},// BULLET_MINI8_06_AOI,
	{122, 1,  6, 6},// BULLET_MINI8_07_YUKARI,
//
	{  1,10, 12,12},// BULLET_MARU12_00_SIRO,
	{ 14,10, 12,12},// BULLET_MARU12_01_AKA,
	{ 27,10, 12,12},// BULLET_MARU12_02_MIDORI,
	{ 40,10, 12,12},// BULLET_MARU12_03_AOI,
	{ 53,11, 10,10},// BULLET_MARU10_00_AOI,
	{ 64,11, 10,10},// BULLET_MARU10_01_MIDORI,
	{ 75,11, 10,10},// BULLET_MARU10_02_MIDORI,
	{ 86,11, 10,10},// BULLET_MARU10_03_MIDORI,
//
	{105, 9,  5,32},// BULLET_HARI32_00_AOI
	{111, 9,  5,32},// BULLET_HARI32_01_AKA
	{117, 9,  5,32},// BULLET_HARI32_02_KIIRO
	{123, 9,  5,32},// BULLET_HARI32_03_DAIDAI
	{ 97,10,  7,15},// BULLET_CAP16_04_SIROI
	{ 97,26,  7,15},// BULLET_CAP16_04_AKA
	{ 10,74,  8,21},// BULLET_KNIFE20_06_AOI,
	{ 19,74,  9,21},// BULLET_KNIFE20_07_AKA,
//
	{  1,23, 15,14},// BULLET_UROKO14_00_AOI,
	{ 17,23, 15,14},// BULLET_UROKO14_01_AKA,
	{ 33,23, 15,14},// BULLET_UROKO14_02_YUKARI,
	{ 49,23, 15,14},// BULLET_UROKO14_03_MIDORI,
	{ 65,23, 15,14},// BULLET_UROKO14_04_MIZUIRO,
	{ 81,23, 15,14},// BULLET_UROKO14_05_KIIRO,
	{  1,38,  7,15},// BULLET_KNAI20_06_AOI
	{ 17,38,  7,15},// BULLET_KNAI20_07_MIDORI
//
	{  9,38,  7,15},// BULLET_KUNAI12_00_AKA,
	{  9,38,  7,15},// BULLET_KUNAI12_01_AKA,
	{ 25,38,  7,15},// BULLET_KUNAI12_02_MIDORI,
	{ 33,38,  7,15},// BULLET_KUNAI12_03_YUKARI,
	{ 41,38,  7,15},// BULLET_KUNAI12_04_AOI,
	{ 49,38,  9,14},// BULLET_OFUDA12_05_AOI,
	{ 59,38,  9,14},// BULLET_OFUDA12_06_AKA,
	{ 69,38,  9,14},// BULLET_OFUDA12_07_MIDORI,
//
	{  1,64, 32,32},// BULLET_OODAMA32_00_SIROI,
	{ 32,64, 32,32},// BULLET_OODAMA32_01_AOI,
	{ 64,64, 32,32},// BULLET_OODAMA32_02_AKA,
	{ 96,64, 32,32},// BULLET_OODAMA32_03_YUKARI,
	{  1,96, 32,32},// BULLET_OODAMA32_04_MIDORI,
	{ 32,96, 32,32},// BULLET_OODAMA32_05_AOI,
	{ 64,96, 32,32},// BULLET_OODAMA32_06_KIIRO,
	{ 96,96, 32,32},// BULLET_OODAMA32_07_PINK,
//
};

void aaa_trans_bullets(void)
{
	TGameSprite *obj;
	SPRITE *sss = sprite_list000_head;/* スプライト リストの先頭 から探す */
	while (NULL != sss)/* スプライト リストの最後まで調べる */
	{
		if (/*SP_BULLET*/SP_GROUP_BULLETS & sss->type)
		{
			/* --- オブジェクトポインタ受け取り */
			obj 				= TGameScreen_GetSpriteSerial();
			obj->used			= 1/*TR UE*/;
			obj->alpha			= sss->alpha;	/* α値(0xff==255 で不透明、0 で透明) */
			obj->TextureId		= 1;//sss->m_TextureId ;

			/* --- オブジェクト表示設定 */
			obj->x256			= ((int)sss->x256 /*>>8*/ /*/(2 *256)*/ ) /*+(56)*/ /*+ 80*/;
			obj->y256			= ((int)sss->y256 /*>>8*/ /*/(2 *256)*/ ) /*+ 16*/;
		//	if (sss->m_PosCenter == 1/*TR UE*/)
		//	{
		//		obj->x -= (sss->w / 2);
		//		obj->y -= (sss->h / 2);
		//	}
			obj->w			= tama00[(SP_GROUP_SUB_TYPE & sss->type)].w/*((sss->w128)>>7)*/;
			obj->h			= tama00[(SP_GROUP_SUB_TYPE & sss->type)].h/*((sss->h128)>>7)*/;
			obj->tx 		= tama00[(SP_GROUP_SUB_TYPE & sss->type)].u/*1*/;//sss->texture_x;
			obj->ty 		= tama00[(SP_GROUP_SUB_TYPE & sss->type)].v;//sss->texture_y;
			#if (1==USE_ZOOM_XY)
			obj->zoom_x256	= sss->m_zoom_xy256;		/* 拡大率 0x100==256 が x1.0倍 */
			obj->zoom_y256	= sss->m_zoom_xy256;		/* 拡大率 0x100==256 が x1.0倍 */
			#else //(0==USE_ZOOM_XY)
			obj->zoom_xy256 = 256;//sss->m_zoom_xy256;		/* 拡大率 0x100==256 が x1.0倍 */
			#endif/* (1==USE_ZOOM_XY) */
			obj->rotation_z = (sss->m_angle512/*<<(7)*/) /*m_RollZ*/; 		/* 角度は0-512度 */ 	/* 角度は0-65535度 */
//
			#if (1==USE_ZBUFFER)
			obj->priority	= sss->m_priority/*aaa*/;		/* 表示優先順位 */
			#endif/* (1==USE_ZBUFFER) */


		//	s->type = SP_DELETE;
		}
		sss = sss->next;/*次*/
	}
}
static void bullet_trans_test(void)
{
	TGameScreen_ClearSprite();
	aaa_trans_bullets();
}
/*-------------*/
		#if 1
extern /*static*/ int current_bg0_y_scroll_speed256;	/* bg0のスクロール、現在速度 */
	int val1 = 0;
	int val3 = 0;
	int val3_256 = 0;
static void bg_trans_test(void)
{
//	#define V10 (5*2)
	#define V10 (8*2)
	Vertex_uvcxyz* vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(V10*sizeof(Vertex_uvcxyz));

	/* --- 半透明合成値 */
	#if (1==USE_VCOLOR)
		#if (16==USE_BLIT_COLOR_BIT)
		/*ARGB4444*/
//	unsigned /*int*/short blendlevel = (((spr->alpha & 0xf0) << 8) | 0x0fff);
//	unsigned /*int*/short blendlevel = ((( 0xf0) << 8) | 0x0fff);
	unsigned /*int*/short blendlevel = (((conv_bg_alpha &  0xf0) << 8) | 0x0fff);
//(conv_bg_alpha)
		#else
	unsigned int blendlevel = (((spr->alpha & 0xff) << 24) | 0x00ffffff);
		#endif
	#endif

//	val1 = (player->x256>>8);
		#if 1
		val3_256 -= (current_bg0_y_scroll_speed256);
	//	val3--;
		val3 = t256_floor(val3_256);
		val3 &= 255;
		#endif

#define Y200 (0)
#define Y272 ((PSP_HEIGHT272)/*-1*/)
		unsigned int j;
		unsigned short x2;
		unsigned short x3;
		unsigned short x4;
		x2 = ((GAME_WIDTH/2) - (100)-32) + ((val1&127)-64);
		x3 = ((GAME_WIDTH/2) - (200)-32);
		x4 = 0;
		for (j=0; j<V10; j+=2)
		{
			vertices[j].x = x3;
			vertices[j].y = Y272;
			vertices[j].z = 0;
			vertices[j].u = x4;
			vertices[j].v = 255+(val3);
			#if (1==USE_VCOLOR)
			vertices[j].color	= blendlevel;
			#endif
	//
			vertices[j+1].x = x2;
			vertices[j+1].y = Y200;
			vertices[j+1].z = 0;
			vertices[j+1].u = x4;
			vertices[j+1].v = 0+(val3);
			#if (1==USE_VCOLOR)
			vertices[j+1].color	= blendlevel;
			#endif
			x2 +=  64;/*上縁側*/
			x3 += 100;/*下縁側*/
			x4 +=  64;/*テクスチャ*/
		}
// 1 3 5 7 9
// 0 2 4 6 8
		sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS5650, (V10), /*0*/NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS4444, (V10), /*0*/NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_STRIP, TEXTURE_FLAGS, 4, /*0*/NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLES, TEXTURE_FLAGS, 3, /*0*/NULL, vertices);
	//	sceGuDrawArray(GU_TRIANGLE_FAN, TEXTURE_FLAGS, 4, /*0*/NULL, vertices);
	//	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 4, /*0*/NULL, vertices);/*2D専用？*/
}
		#endif

/*-------------*/
/*-------------*/
/*-------------*/

#if (1==USE_GU)
#if (1)

static void rect_trans_test(void)
{
/* 実験:
(a) SLICE_SIZE 64 で 512 x 272 転送 (場合分けなし)
(b) SLICE_SIZE 64 で 480 x 272 転送 (場合分けあり)
(c) SLICE_SIZE 32 で 480 x 272 転送 (場合分けなし)
結果: 速い : (b) > (c) > (a)  : 遅い
 */
#define USE_SLICE_32 (0)
#if (0==USE_SLICE_32)
	#define SLICE_SIZE32	(64)
#else
	#define SLICE_SIZE32	(32)
#endif
	unsigned int j;
	unsigned int i;
	#if 0
	unsigned int count2;
	count2 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		count2 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
			#define count2 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		#else
			#define count2 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count2 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	#if (0==USE_SLICE_32)
	unsigned int w_size;
	w_size = SLICE_SIZE32;
	#else
		#define w_size SLICE_SIZE32
	#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count2; )
	{
		#if (0==USE_SLICE_32)
		/* 最後 */
		if (i==(count2-2))
		{
			w_size = 32;	/* 32 == 512-480 */
		}
		#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 0;
		vertices[(i)].x = j/**480/512*/;
		vertices[(i)].y = 0;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+w_size);
		vertices[(i)+1].v = 272/*320*/;
		vertices[(i)+1].x = (j+w_size)/**480/512*/;
		vertices[(i)+1].y = 272;
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count2), NULL, vertices);
}


static void rect_trans_test_script_window(void)
{
/* 実験:
(a) SLICE_SIZE 64 で 512 x 272 転送 (場合分けなし)
(b) SLICE_SIZE 64 で 480 x 272 転送 (場合分けあり)
(c) SLICE_SIZE 32 で 480 x 272 転送 (場合分けなし)
結果: 速い : (b) > (c) > (a)  : 遅い
 */
	unsigned int j;
	unsigned int i;
	#if 0
	unsigned int count29;
	count29 = 0;
	for (j=0; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); j+=SLICE_SIZE32)
	{
		count29 += 2;
	}
	#else
		#if (0==USE_SLICE_32)
		//	#define count29 (16)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		//	#define count29 (10)/* 16 == 2 x (512/64[SLICE_SIZE32] ) */
		//	#define count29 (10)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
			#define count29 (12)/* 10 == 2 x (320/64[SLICE_SIZE32] ) */
		#else
			#define count29 (30)/* 30 == 2 x (480/32[SLICE_SIZE32] ) */
		#endif
	#endif
	Vertex_uvcxyz	*vertices;
	vertices = (Vertex_uvcxyz*)sceGuGetMemory(count29 * sizeof(Vertex_uvcxyz));
	i=0;
	j=0;
	//#if (0==USE_SLICE_32)
	//unsigned int w_size;
	//w_size = SLICE_SIZE32;
	//#else
		#define w_size99 SLICE_SIZE32
	//#endif
//	for (; (j+SLICE_SIZE32)<=(/*480*/512/*512*/); )
	for (; i<count29; )
	{
	//	#if (0==USE_SLICE_32)
	//	/* 最後 */
	//	if (i==(count29-2))
	//	{
	//	//	w_size = 32;	/* 32 == 512-480 */
	//		w_size = 44;	/* 44 == 384-340 */
	//	}
	//	#endif
		vertices[(i)].u = j;
		vertices[(i)].v = 1+(0);
		vertices[(i)].x = (10)+(20)+j/**480/512*/;
		vertices[(i)].y = (10)+(182)+0;
		vertices[(i)].z = 0;
		vertices[(i)+1].u = (j+w_size99);
		vertices[(i)+1].v = 1+(1+4+47/*70*/)/*272*/ /*320*/;
		vertices[(i)+1].x = (10)+(20)+(j+w_size99)/**480/512*/;
		vertices[(i)+1].y = (10)+(182)+(1+4+47/*70*/)/*272*/;
		vertices[(i)+1].z = 0;
		i += 2;
		j += SLICE_SIZE32;
	}
	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS5650, (count29), NULL, vertices);
}

static void gu_draw_script(void)
{
		/* -- せりふウィンドウを描画 */
		/* テクスチャーがあるとテクスチャー優先でフラットポリゴンが描画出来ないので */
		/* 一時的にテクスチャー無効モードに切り替える */
		sceGuDisable(GU_TEXTURE_2D);/* テクスチャー無効モードにする */
		{
			//#define TEXTURE_FLAGS1111		(GU_TEXTURE_16BIT | 				GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			#define TEXTURE_FLAGS1111		( 									GU_VERTEX_16BIT | GU_TRANSFORM_2D)
			//GU_VERTEX_32BITF|GU_TRANSFORM_2D
			// we make local copies of the line into the main buffer here, so we don't have to flush the cache
			Point_Vertex* my_vertices = sceGuGetMemory((5) * sizeof(Point_Vertex));

			// create a lineloop
			my_vertices[0].x = (20)      /*x-width*/; 	my_vertices[0].y = (182)     /*y-height*/;
			my_vertices[1].x = (20)+(340)/*x+width*/; 	my_vertices[1].y = (182)     /*y-height*/;
			my_vertices[2].x = (20)+(340)/*x+width*/; 	my_vertices[2].y = (182)+(70)/*y+height*/;
			my_vertices[3].x = (20)      /*x-width*/; 	my_vertices[3].y = (182)+(70)/*y+height*/;
			my_vertices[4].x = (20)      /*x-width*/; 	my_vertices[4].y = (182)     /*y-height*/;
		//
		//	#define argb 0xffffffff
			#define argb 0x7f0f0f0f
		//	#define argb 0x7f7f7f7f
		//	sceGuColor(/*0xaaaa*/ ((argb&0x7f7f7f7f)>>1) /*argb*/);
			sceGuColor(argb/*0xffff*/ /*argb*/);
			sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, TEXTURE_FLAGS1111, (3), NULL, my_vertices);
			sceGuDrawArray(GU_TRIANGLES/*GU_LINE_STRIP*/, TEXTURE_FLAGS1111, (3), NULL, &my_vertices[2]);
		//
			sceGuColor(/*0xaaaa*/ ((argb&0x7f7f7f7f)>>1) /*argb*/);
		//	sceGuColor(argb/*0xffff*/ /*argb*/);
			sceGuDrawArray(GU_LINE_STRIP, TEXTURE_FLAGS1111, (5), NULL, my_vertices);
		}
		/* テクスチャー有効モードでは、線や面等のフラットポリゴンが使えない */
		sceGuEnable(GU_TEXTURE_2D); 	/* テクスチャー有効モードに戻す */
	//
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdouble buffer???*/
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
		sceGuTexImage(0, 512, 512, 512, msg_window_image);
		rect_trans_test_script_window();
}

#endif/*(000)*/
#endif

#define USE_VSYNC			(0)
#define USE_MAX_GU_TIME 	(0)
extern int draw_script_screen; 					/* せりふウィンドウ表示フラグ */
void vbl_draw_screen(void)
{
	do_input_vbl();/*キー入力(処理の都合上、ここに移動)*/
	#if (1==USE_GU)
//	fps_show();
//
	#if (0!=USE_MAX_GU_TIME)
	/* [Gu時間を最大に取るタイプ](色々制限あり) */
//	sceGuFinish();実験
	/* - パケット描画終了待ち */
	sceGuSync(0, 0);/* (ここまで Gu) */

	/* --- VSync and swap frame buffer */
	#if (1==USE_VSYNC)
	sceDisplayWaitVblankStart();/*vsync*/
	#endif /* (1==USE_VSYNC) */
//	do_input_vbl();
	sceGuSwapBuffers();
	#endif
	/* ここから Gu */
	/* --- スクリーンアップデート */
	sceGuStart(GU_DIRECT, gulist );

	/* --- 画面クリア */
	#if 1
	{
		const unsigned int bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/**/
			0xff106010,/*1面*/
			0xff102080,/*2面*/
			0xff104010,/*3面*/
			0xff402020,/*4面*/
			0xff601030,/*5面*/
			0xff000000,/*ending*/
			0xff601010,/**/
		};
		sceGuClearColor(bg_color_list[player_now_stage&0x07]);/*AABBGGRR*/
	//	sceGuClearColor(/*0*/0xff601010/*0xff601010*/);/*AABBGGRR*/
	}
	#endif
	#if (1==USE_ZBUFFER)
	sceGuClearDepth(0);// Z_BUFFER
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_FALSE);
	#else // (0==USE_ZBUFFER)
	sceGuClear(GU_COLOR_BUFFER_BIT/*|GU_DEPTH_BUFFER_BIT*/);
	#endif/* (1==USE_ZBUFFER) */

	/* -- 画面更新 */

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

//	if (1==draw_bg_screen) 	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=draw_bg_screen)
	{
	/* -- BG 画面を描画 */
	//	sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);//???
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*半透明*/
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
			#if (16==USE_BLIT_COLOR_BIT)
	//	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_4444/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
		sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5650/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
			#else
		sceGuTexMode(/*GU_PSM_5551*/GU_PSM_8888/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
			#endif
	//	sceGuTexImage(0, TW128, TH128, 512, bullet_image);
	//	sceGuTexImage(0, TW128/2, TH128/2, TW128/2/*WK512*/, my_texture[0]->pixels);
	//	sceGuTexImage(0, TW128, TH128, TW128/*WK512*/, my_texture[1]->pixels);
		sceGuTexImage(0, 256, 256, 256/*WK512*/, my_texture[1]->pixels);

	//	sceGuEnable(GU_CLIP_PLANES);
		bg_trans_test();
	//	sceGuDisable(GU_CLIP_PLANES);
	}

/* -- SDL 画面を描画 */
	#if (1)
//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*半透明*/
//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdouble buffer???*/
//
//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

//	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
//	sceGuBlendFunc(GU_SUBTRACT, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
//	sceGuBlendFunc(GU_ADD, GU_SRC_COLOR, GU_DST_COLOR, 0, 0);
//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f007f7f, 0x3f3f3f00);
//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_ONE_MINUS_SRC_ALPHA, (conv_bg_alpha), 0xffffffff);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);


//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0, 0);
//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
//	sceGuBlendFunc(GU_MIN, GU_FIX, GU_FIX, 0, 0);
//	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0x7f7f7f7f, 0x7f7f7f7f);
//
	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, 0/*0 swizzle*/);
	sceGuTexImage(0, 512, 512, 512, render_image);
	rect_trans_test();
	#endif /*(000)*/
//
/* -- 敵弾を描画 */
	#if (1)
//	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);/*???*/		/*勝手にdouble buffer???*/
//	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);/*半透明*/
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGB);
//
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
//
		#if (16==USE_BLIT_COLOR_BIT)
//	sceGuTexMode(/*GU_PSM_5551*/SDL_GU_PSM_0000/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5551/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
//	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_4444/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_5650/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
		#else
	sceGuTexMode(/*GU_PSM_5551*/GU_PSM_8888/*GU_PSM_5650*/, 0, 0, /*1*/(USE_SWIZZLE)/*1 swizzle*/);
		#endif
//	sceGuTexImage(0, TW128, TH128, 512, bullet_image);
//	sceGuTexImage(0, TW128, TH128, WK512, my_texture[0]->pixels);
	sceGuTexImage(0, 128, 128, 128, my_texture[0]->pixels);
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	bullet_trans_test();
	blit_all_objects();
	#endif /*(000)*/

	/* -- スクリプト画面を描画 */
//	if (1==draw_script_screen)  	/* pspは0レジスタがあるので0と比較したほうが速い */
	if (0!=draw_script_screen)
	{		gu_draw_script();	}

	/* -- 開発デバッグフォント */
	#if (1==DEBUG)
	TDebugDisp();
	#endif // (1==DEBUG)

	#if (1==USE_ZBUFFER)
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(GU_TRUE);
	#endif/* (1==USE_ZBUFFER) */

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
	/* 予約 voice を再生 */
	//voice_play_vbl();
}
