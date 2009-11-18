
/*---------------------------------------------------------
	ヘッダ
---------------------------------------------------------*/

#include "game_main.h"


/*---------------------------------------------------------
	math.h
---------------------------------------------------------*/

/* この配列もCPU内蔵の命令キャッシュに乗る(とおもう)よ。 */
/*extern*/ int sin_tbl512[SINTABLE_SIZE];


#if 0
/* CWの場合 */
#else
/* CCWの場合 */
	/*とりあえず*/
	#define rad2deg512(x)		( (int)((x)*((512.0)/(T_GU_M_PI*2))/*+512*/)&(512-1) )
#endif
/* この関数はCPU内蔵の命令キャッシュに乗るよ(てい。というか乗ってるはず)。 */
int atan_512( int y, int x )
{
	#if 0
/* CWの場合 */
//	return (rad2deg512(atan2(y,x)));	/* 右CWの場合。  一般的な右CW の場合。 */
	#else
/* CCWの場合 */
	return (rad2deg512(atan2(x,y)));	/* 下CCWの場合。 (x y を入れ替えて対処してあります。) */
	#endif
}

/*
psp では、 atan2(), sin(), sqrt() 等の超越関数系命令は、
psp の MIPS CPU 内 のコプロセッサが処理をする。
コプロセッサ変換処理があちこちにあると、非常にパフォーマンスが悪いので、
一ヶ所に纏めた方が実行速度は遥かに速い。
(CPU内蔵の命令キャッシュに乗るために実行速度が速くなる)

参考:TECH I Vol.39 MIPSプロセッサ入門	http://www.cqpub.co.jp/interface/TechI/Vol39/
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
	この関数はただの初期化。命令キャッシュに乗らない(はずだ)よ。
---------------------------------------------------------*/

void init_math(void)
{
	unsigned int i;
	for (i=0; i<SINTABLE_SIZE; i++)
	{
	//	sin_tbl512[i] = (int)(sinf( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
		sin_tbl512[i] = (int)(sin( (i*(GU_PI*2) / SINTABLE_SIZE) )*256/**65536.0*/ );
	}
}


/*---------------------------------------------------------
	キー入力関連の処理(汎用)
---------------------------------------------------------*/

/*extern*/ int pad_config[KEY_NUM12_MAX];


extern void save_screen_shot(void);
/*global*/Uint32 my_pad;		/*今回入力*/
/*global*/Uint32 my_pad_alter;	/*前回入力*/
/*global*/short my_analog_x;	/* アナログ量、補正済み */
/*global*/short my_analog_y;	/* アナログ量、補正済み */

/*static*/ void do_input_vbl(void)
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
		if (pad.Lx < 64/*70*/)			{	pad_data |= PSP_CTRL_LEFT;		my_analog_x = ((64-pad.Lx)<<2); 	}
		else if (pad.Lx > 192/*185*/)	{	pad_data |= PSP_CTRL_RIGHT; 	my_analog_x = ((pad.Lx-192)<<2);	}
		//
		if (pad.Ly < 64/*70*/)			{	pad_data |= PSP_CTRL_UP;		my_analog_y = ((64-pad.Ly)<<2); 	}
		else if (pad.Ly > 192/*185*/)	{	pad_data |= PSP_CTRL_DOWN;		my_analog_y = ((pad.Ly-192)<<2);	}
	}
	#if (1==USE_KEY_CONFIG)
	my_pad_alter = my_pad;
	my_pad = 0;
	/* PSPのデジタル入力をキーコンフィグの形式に変換 */
	if (pad_data & PSP_CTRL_SELECT) 	{	my_pad |= pad_config[KEY_NUM00_SELECT]; 	}	//	if (keyboard[KINOU_01_SELECT])		{my_pad |= (PSP_KEY_SELECT);}
	if (pad_data & PSP_CTRL_START)		{	my_pad |= pad_config[KEY_NUM01_START];		}	//	if (keyboard[KINOU_02_PAUSE])		{my_pad |= (PSP_KEY_PAUSE);}
//
	if (pad_data & PSP_CTRL_LTRIGGER)	{	my_pad |= pad_config[KEY_NUM06_L_TRIG]; 	}	//	if (keyboard[KINOU_07_SNAP_SHOT])	{my_pad |= (PSP_KEY_SNAP_SHOT);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{	my_pad |= pad_config[KEY_NUM07_R_TRIG]; 	}	//	if (keyboard[KINOU_08_SYSTEM])		{my_pad |= (PSP_KEY_SYSTEM);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{	my_pad |= pad_config[KEY_NUM08_TRIANGLE];	}	//	if (keyboard[KINOU_09_SLOW])		{my_pad |= (PSP_KEY_SLOW);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{	my_pad |= pad_config[KEY_NUM09_CIRCLE]; 	}	//	if (keyboard[KINOU_10_OPTION])		{my_pad |= (PSP_KEY_OPTION);}
	if (pad_data & PSP_CTRL_CROSS)		{	my_pad |= pad_config[KEY_NUM10_CROSS];		}	//	if (keyboard[KINOU_11_SHOT])		{my_pad |= (PSP_KEY_SHOT_OK);}
	if (pad_data & PSP_CTRL_SQUARE) 	{	my_pad |= pad_config[KEY_NUM11_SQUARE]; 	}	//	if (keyboard[KINOU_12_BOMB])		{my_pad |= (PSP_KEY_BOMB_CANCEL);}

	/* キーコンフィグの形式では、ゲーム中速度的に無理なので、PSPのデジタル入力に似た方式に再び変換 */

	my_pad |= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_KEY_DOWN|PSP_CTRL_LEFT));

	#endif /* (1==USE_KEY_CONFIG) */
	/* スクリーンショット機能。 */
	// keypollに入れると何故かうまくいかなかったのでこっちに場所を変更。
	if (/*keyboard[KINOU_07_SNAP_SHOT]*/my_pad & PSP_KEY_SNAP_SHOT) { 	save_screen_shot(); }

	/* アナログサポート機能 */
//	if (1==use_analog)
	{
		/* デジタルよりアナログ優先 */
		if (0 == (my_analog_x+my_analog_y) )
		/*アナログ押してないと思われる場合(アナログ押してる場合はアナログ量をそのまま使う)*/
		{
			/* デジタルよりアナログ量を算出 */
			#if (0)
				 if (pad_data & PSP_CTRL_UP)		{	my_analog_y =	256;	}
			else if (pad_data & PSP_CTRL_DOWN)		{	my_analog_y =	256;	}
				 if (pad_data & PSP_CTRL_RIGHT) 	{	my_analog_x =	256;	}
			else if (pad_data & PSP_CTRL_LEFT)		{	my_analog_x =	256;	}
			#else
			if (pad_data & (PSP_CTRL_UP|PSP_CTRL_DOWN)) 	{	my_analog_y =	256;	}	/* 上下のアナログ量 */
			if (pad_data & (PSP_CTRL_RIGHT|PSP_CTRL_LEFT))	{	my_analog_x =	256;	}	/* 左右のアナログ量 */
			#endif
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
	memset(ptr, 0, size);/* calloc()風に、必ず0クリアーするように変更 */
	return (ptr);
}


/*---------------------------------------------------------
	画像キャッシュ関連

	同じ画像を複数読み込んだ場合にメモリが無駄になりもったいない。
	そこで同じ画像を読み込んだ場合には、実際には読み込まないで、
	前に読み込んだ画像と同じものを使う。
	トータルでいくつ同じ画像を読み込んだかは、それぞれの画像キャッシュの参照数でわかる。
---------------------------------------------------------*/

typedef struct _imglist
{
	SDL_Surface *img;			/* 読み込んだサーフェイスを保持 */
	struct _imglist *next;		/* 次の画像へのリストポインタ */
	int refcount;				/* 同じ画像の参照数 */
	char name[128/*256*/];		/* 名前 */
} MY_IMAGE_LIST;

/* 画像キャッシュのリスト */
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
		error(ERR_FATAL, "load0:cant load image %s:\n"/*" %s"*/,fn/*,SDL_GetError()*/);
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
	*sがMY_IMAGE_LIST内にあるか確認をし、
	画像が見つかった場合、図形キャッシュリストの参照数を一つ減らす。
---------------------------------------------------------*/

void unloadbmp_by_surface(SDL_Surface *s)
{
	MY_IMAGE_LIST *i = my_image_list;/* 図形キャッシュリストの先頭 */
	while (NULL != i)
	{
		if (s == i->img)	/* 画像が見つかった */
		{
			if (0 == i->refcount)	/* 図形キャッシュリストの参照数 */
			{
				/* ロードしてないのに開放。 */
				CHECKPOINT;
				error(ERR_WARN, "unloadbmp_by_surface: refcount for object %s is already zero",i->name);
			}
			else
			{
				i->refcount--;	/* 一つ減らす。 */
			}
			return; 	/* 正常終了 */
		}
		i = i->next;
	}
	/* 見つからない。 */
	CHECKPOINT;
	error(ERR_WARN, "unloadbmp_by_surface: object not found");
//	return; 	/* 異常終了 */
}

