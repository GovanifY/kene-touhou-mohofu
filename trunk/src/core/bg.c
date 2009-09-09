
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "game_main.h"

#define USE_BG_LOAD 0

#if (1==USE_BG_LOAD)
/* tile */
static SDL_Surface *bg0_bmp/*=NULL*/;
static SDL_Surface *bg1_bmp/*=NULL*/;		// [***090201		追加

static SDL_Surface *draw_bmp;
static SDL_Surface *load_bmp;
static signed int bg0_bmp_y256;

static int which_bg;						// [***090209追加:	次回どちらに読むのかを示す番号。0==bg0_bmp, 1==bg1_bmp			//	追加背景番号

static int number_of_bg;					/* bg追加した枚数 */
//static int use_clouds;

static int exsist_tuika;					/* 追加bgはある？フラグ */

static unsigned int laster_sprit256;		/* bg0_bmpと bg1_bmpの 表示ラスタ分割位置 */
static int sprit_flag;						/* 分割位置を判断する必要があるか？フラグ */
#endif /* (1==USE_BG_LOAD) */


/*static*/ int current_bg0_y_scroll_speed256;	/* bg0のスクロール、現在速度 */
static int request_bg0_y_scroll_speed256;	/* bg0のスクロール、予約速度 */

//#define USE_EXTRACT_WAIT (0)
//#if (1==USE_EXTRACT_WAIT)
//static int now_exetracting;
//#endif

#if (1==USE_BG_LOAD)
/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

static void load_btile(STAGE_DATA *l, int num)
{
	char *text;
	text=l->user_string;
	switch (num)
	{
	case 0/*1*/:
		if (NULL != bg0_bmp)	{	unloadbmp_by_surface(bg0_bmp);	bg0_bmp = NULL; }
		bg0_bmp = loadbmp0(text/*filename*/, 0, 1);
//		#if (1==USE_EXTRACT_WAIT)
//		now_exetracting=60;
//		#endif
		break;
	case 1/*2*/:
		if (NULL != bg1_bmp)	{	unloadbmp_by_surface(bg1_bmp);	bg1_bmp = NULL; }
		bg1_bmp = loadbmp0(text/*filename*/, 0, 1);
//		#if (1==USE_EXTRACT_WAIT)
//		now_exetracting=60;
//		#endif
		break;
	}
}

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

static void bg2_swap(void)
{
	if (0==which_bg)/* フラグの意味が次なので逆になる */
	{
		draw_bmp = bg1_bmp;
		load_bmp = bg0_bmp;
	}
	else
	{
		draw_bmp = bg0_bmp;
		load_bmp = bg1_bmp;
	}
}
#endif /* (1==USE_BG_LOAD) */

/*---------------------------------------------------------

---------------------------------------------------------*/

extern unsigned int conv_bg_alpha;

static int current_bg_alpha;
static int request_bg_alpha;

void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}
/*---------------------------------------------------------

---------------------------------------------------------*/

static void tile_work(void)
{
//
	if (current_bg_alpha == request_bg_alpha) /*最もありそうな可能性を排除*/
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= 4/*6*/;
		}
		else
		{
			current_bg_alpha += 4/*6*/;
			if (245 < current_bg_alpha) 	/* じわじわするので */
			{	current_bg_alpha = 255; 	}
		}
		/* converted  */
		u8 aaa =(current_bg_alpha/*>>1*/);
		conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
	}

//	if (current_bg_alpha < 250/*255*/ )
//			{	current_bg_alpha += 6;		}
//	else	{	current_bg_alpha = 255; 	}
//
	if (current_bg0_y_scroll_speed256 == request_bg0_y_scroll_speed256) /*最もありそうな可能性を排除*/
	{
		;
	}
	else
	if (current_bg0_y_scroll_speed256 > request_bg0_y_scroll_speed256)
	{
		current_bg0_y_scroll_speed256--;
	}
	else
	{
		current_bg0_y_scroll_speed256++;
	}
//
	#if (1==USE_BG_LOAD)
	bg0_bmp_y256 -= current_bg0_y_scroll_speed256;
	//
	if (laster_sprit256 < t256(272) )
	{
		laster_sprit256 += current_bg0_y_scroll_speed256;
		sprit_flag = 1; 	/* 分割位置を判断する必要がある */
	}
	else
	{
		laster_sprit256 = t256(272);
		sprit_flag = 0; 	/* 分割位置を判断する必要がない */
	}
	if (bg0_bmp_y256 < (0) )
	{
	//	if (1==exsist_tuika)	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (0!=exsist_tuika)
		{
			exsist_tuika = 0;
			if (1 < number_of_bg)	/* 2枚以上bg追加した場合のみ、分割ラインを設定する */
			{
				laster_sprit256 = t256(0);
				sprit_flag = 1; 	/* 分割位置を判断する必要がある */
				/*	ここの sprit_flag = 1; がここにもあるのは、一見効率悪いが、
					処理の順序を変えると他の問題(加算順序による誤差の辻褄合わせ)が出てもっと効率悪くなるので、
					ここで辻褄を合わせている。 */
			}
			bg2_swap();
		}
		bg0_bmp_y256 += ((bg0_bmp->h)<<8);
	}
	#endif /* (1==USE_BG_LOAD) */
//
	#if 0/*laster_spritデバッグ用*/
	/* パネルのスコア欄にlaster_spritを グレイズ欄に追加bg枚数を 表示させる。っていうか書き換えちゃう。 */
	((PLAYER_DATA *)player->data)->score		= (t256_floor(laster_sprit256));
	((PLAYER_DATA *)player->data)->graze_point	= number_of_bg;
	#endif
}

/*---------------------------------------------------------

---------------------------------------------------------*/
	#if 0//(1 != USE_GU)
	/* psp の液晶は残像が残るので、これぐらいやっても、殆んど違いが判らない(但し60fps付近で動いてる場合のみ) */
	/* 0 < type3 < 122 < type2 < 132 < type3 <	250 < type1 */
	int blit_type;
			if (250 < current_bg_alpha )	{	blit_type = BLIT_TYPE01_255;	}/* アルファ殆んど無いなら勝手になし */
	else	if (132 < current_bg_alpha )	{	blit_type = BLIT_TYPE03_000;	}
	else	if (122 < current_bg_alpha )	{	blit_type = BLIT_TYPE02_127;	}/* 半分付近なら、勝手に半分固定 */
	else									{	blit_type = BLIT_TYPE03_000;	}
//
	#endif


					/* ここを 関数ポインタで分岐させると、とてつもなく遅い。vsync取ってないとはいえ 約60fps が 30fps 未満になる */
					/* って事は、ここだけで 16[msec]以上消費するって事なのかな？？？ */
					/* psp(MIPS) は pcと違ってポインタ使った場合のペナルティーがとてつもなくでかい。 */
					/* おそらくCPU内蔵の命令キャッシュ(I-Chache)が壊れるので、とてつもなく遅くなるんだろうな */
					#if 0//(1 != USE_GU)
					if (BLIT_TYPE01_255==blit_type)
					#endif
					#if 0//(1 != USE_GU)
					else
					if (BLIT_TYPE02_127==blit_type)
					{
						(*pd) = (((*ps)&PSP_SCREEN_FORMAT_LMASK)>>1);	/* 暗い転送(アルファ半分固定) */
					}
					else	//if (BLIT_TYPE03_000==blit_type)
					{	/* 汎用転送(アルファ任意) */
						const Uint16 aaa = (*ps);
						(*pd) = (Uint16)(/*(PSP_SCREEN_FORMAT_AMASK)|*/((
							(((aaa & PSP_SCREEN_FORMAT_BMASK)*current_bg_alpha) & (PSP_SCREEN_FORMAT_BMASK<<8)) |
							(((aaa & PSP_SCREEN_FORMAT_GMASK)*current_bg_alpha) & (PSP_SCREEN_FORMAT_GMASK<<8)) |
							(((aaa & PSP_SCREEN_FORMAT_RMASK)*current_bg_alpha) & (PSP_SCREEN_FORMAT_RMASK<<8))
						)>>8));
					}
					#endif




#if 0000

enum
{
	BLIT_TYPE01_255 = 0,	/* 単純転送(アルファなし) */
	BLIT_TYPE02_127,		/* 暗い転送(アルファ半分固定) */
	BLIT_TYPE03_000,		/* 汎用転送(アルファ任意) */
};
static void tile_draw(SDL_Surface *src)
{
	/*blit*/
	{
		SDL_Surface *dst;
		dst = sdl_screen[SDL_00_SCREEN];
		if (SDL_MUSTLOCK(src))	{	SDL_LockSurface(src);	}/*ロックする*/
		if (SDL_MUSTLOCK(dst))	{	SDL_LockSurface(dst);	}/*ロックする*/
		{
			int src_max_h;		src_max_h = src->h;
			unsigned int jj;	jj = (t256_floor(bg0_bmp_y256));
			unsigned int j2;	j2 = (jj*380);
			unsigned int yy256;
			for (yy256=0; yy256<GAME_HEIGHT*256; yy256+=256)
			{
				Uint16 *pd; 	pd = (Uint16 *)dst->pixels + (yy256+yy256)/*(yy<<9)*/;	/*(yy*480)*/  /* (yy<<9); (yy*512)*/
			/*	Uint16 *ps; 	ps = (Uint16 *)src->pixels + (jj*380);*/	/*(jj*480)*/
				Uint16 *ps; 	ps = (Uint16 *)src->pixels + (j2);		/*(jj*480)*/
				jj++;j2 += (380);
				if (jj > (src_max_h-1)) {jj -= src_max_h;	j2 = (jj*380);}
				unsigned int xx;
				for (xx=0; xx<GAME_WIDTH; xx++)
				{
					{
						(*pd) = (*ps);	/* 単純転送(アルファなし) */
					}
					pd++;
					ps++;
				}
				if (0 != sprit_flag)		/* 分割位置を判断する必要==1があるか？ */
				{
					if ( (yy256+t256(1)) >/*=*/ (laster_sprit256) )/* 画面分割位置 */	/* 注意： 256固定小数点なので >= は使えない */
					{
						sprit_flag = 0; 	/* 分割位置を判断する必要がない */
						if (SDL_MUSTLOCK(src))	{	SDL_UnlockSurface(src); 	}/*ロック解除*/
						src = load_bmp;
						if (SDL_MUSTLOCK(src))	{	SDL_LockSurface(src);	}/*ロックする*/
					}
				}
			}
		}
		if (SDL_MUSTLOCK(src))	{	SDL_UnlockSurface(src); 	}/*ロック解除*/
		if (SDL_MUSTLOCK(dst))	{	SDL_UnlockSurface(dst); 	}/*ロック解除*/
	}
}
					#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

//	#if (1==USE_EXTRACT_WAIT)
//	{
//		/* 画像読み込み中(IMG_ Load()内部の画像展開処理(libpngとかlibjpegとか)が遅いので処理落ち中) */
//		now_exetracting--;
//		if (0 < now_exetracting)
//		{
//			return; 	/* 処理落ち時に描かなくしてみたけど、全然変わらない */
//		}
//		now_exetracting=1;
//	}
//	#endif

void bg_work_draw(void)
{
	#if (1==USE_BG_LOAD)
	if (0==number_of_bg)
	{
		psp_clear_screen();
	}
	else
	#endif /* (1==USE_BG_LOAD) */
	{
		tile_work();
	//	#if (1 != USE_GU)
		#if 0000
		tile_draw(draw_bmp/*bg0_bmp*/);
		#else
		/* (1 == USE_GU) */
		psp_clear_screen();
		#endif
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void clouds_destroy(void);
void bg2_destroy(void)
{
	#if (1==USE_BG_LOAD)
	if (0!=number_of_bg)
	{
		number_of_bg = 0;
		//tile_remove();
		//static void tile_remove(void)
		{
			if (NULL != bg0_bmp)	{	unloadbmp_by_surface(bg0_bmp);	bg0_bmp = NULL; }
			if (NULL != bg1_bmp)	{	unloadbmp_by_surface(bg1_bmp);	bg1_bmp = NULL; }
		}
	}
	#endif /* (1==USE_BG_LOAD) */
	clouds_destroy();
}


//extern int tile_bg2_add(void/*int lev*/);
/*---------------------------------------------------------
	敵を追加する
---------------------------------------------------------*/
extern int tiny_strcmp(char *aaa, const char *bbb);
void add_enemy_load_bg(STAGE_DATA *l)
{
#if (1==USE_BG_LOAD)
	char *text;
	text=l->user_string;
	//	if (NULL != (text) )
	//if ( 0 == tiny_strcmp(text,"0") ) /* ファイル名が０の場合システムコマンド[拡張予定] */
	if ( '0' == text[0] )	/* ファイル名の1字目が０の場合システムコマンド[拡張予定] */
	{
		;
	}
	#if (1==USE_BG_LOAD)
	else
	{
		load_btile(l, which_bg/*0*/ /*1*/);
		which_bg++;
		which_bg &= 1;
		#if 1
		number_of_bg++;
		if (/*0*/1 == number_of_bg) 	/* 初回のみ*/
		#else
		/* pspは0レジスタがあるので0と比較したほうが速い */
		/* これでいい筈???なんだけど自信ないなあ(先比較、後加算の筈) */
		if (/*0*/0 == number_of_bg++)	/* 初回のみ*/
		#endif
		{
		//	number_of_bg = 1;
			current_bg_alpha = 0;		/* 初回のみ*/
			request_bg_alpha = 255; 	/* 初回のみ*/
			bg0_bmp_y256 = 0;			/* 初回のみ*/
			draw_bmp = bg0_bmp; 		/* 初回のみロード時に描画用にする */
			load_bmp = bg0_bmp;
		}
		exsist_tuika			= 1;
	}
	#endif /* (1==USE_BG_LOAD) */
#endif /* (1==USE_BG_LOAD) */
}

/*---------------------------------------------------------
	ステージ読み込み開始時に、毎回初期化する
---------------------------------------------------------*/
void bg2_start_stage(void)
{
	#if (1==USE_BG_LOAD)
	number_of_bg			= 0;	/* 初回のみを判断するのと兼用 */
	which_bg				= 0 /*1*/ /*0*/;
	exsist_tuika			= 0;
	#endif /* (1==USE_BG_LOAD) */
	bg2_destroy();
//	if (1==use_clouds) {}
	request_bg0_y_scroll_speed256 = current_bg0_y_scroll_speed256 = t256(0.5);/*初期値*/
}

/*---------------------------------------------------------
	psp起動時に一度だけ初期化する
---------------------------------------------------------*/
extern void clouds_system_init(void);
void bg2_system_init(void)
{
//	#if (1==USE_EXTRACT_WAIT)
//	now_exetracting 		= 1;
//	#endif

//	number_of_bg			= 0;
//	use_clouds				= 0;
	/* 画像読み込み用サーフェイス */
	#if (1==USE_BG_LOAD)
	bg0_bmp 				= //NULL;
	bg1_bmp 				= //NULL;
	draw_bmp				= //NULL/*bg0_bmp*/;
	load_bmp				= NULL/*bg0_bmp*/;
	sprit_flag				= 0;	/* 分割位置を判断する必要がない */
	laster_sprit256 		= t256(272);
	#endif /* (1==USE_BG_LOAD) */
	clouds_system_init();
}

/*---------------------------------------------------------
	BGコントロールコマンド(テスト中)
---------------------------------------------------------*/
enum
{
	BG2_00_ERROR = 0,
	BG2_01_SET_SCROOL_OFFSET,
	BG2_02_BG_STOP,
//	BG2_03_BG_SWAP,
	BG2_03_DESTOROY_CLOUDS,
};
void bg2_control(STAGE_DATA *l)
{
	short xxx;
	short yyy;
//	short speed256;
	xxx = l->user_x;
	yyy = l->user_y;
//	speed256 = l->scroll_speed256;
	//
	request_bg0_y_scroll_speed256 = l->scroll_speed256; 	/* bg0のスクロール、予約速度を設定 */
	switch (xxx)
	{
	case BG2_01_SET_SCROOL_OFFSET:	/* スクロール値を直接セット */
		#if (1==USE_BG_LOAD)
		bg0_bmp_y256 = yyy;
		#endif /* (1==USE_BG_LOAD) */
		break;
	case BG2_02_BG_STOP:	/* テスト中 */
		#if (1==USE_BG_LOAD)
		laster_sprit256 = t256(272);
		sprit_flag = 0; 	/* 分割位置を判断する必要がない */
		#endif /* (1==USE_BG_LOAD) */
		break;
//	case BG2_03_BG_SWAP:	/* テスト中 */
//		bg0_bmp_y256 = 0;
//		which_bg++;
//		which_bg &= 1;
//		bg2_swap();
//		break;
	case BG2_03_DESTOROY_CLOUDS:	/* テスト中 */
		clouds_destroy();
		break;
	}
}
