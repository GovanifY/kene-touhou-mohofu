
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	スクリプト処理
	-------------------------------------------------------
	現在の問題点:
	★ 速度が遅すぎる。ゲーム本体が 60fps 近くで動いてるのに、
	   シナリオが入ると SDL の半透明処理 が遅すぎて 30-40fps 程度に低下する。
	★ SDL画面 と Gu 画面の画像合成が正常に出来ない。
	   SDL の半透明処理 は Gu 画面と合成できない為、
		せっかく 多大な CPUパワーを割いて ソフトウェアー半透明処理 をしてるが、
		単に暗くなるだけ。
	(ものすごく無意味、 Gu 化すれば 半透明合成は psp のハードウェアーで簡単に出来るのに...)。
	-------------------------------------------------------
	「ゲーム中のシナリオ処理」と
	「エンディング等のシナリオ処理」は
	色々考えたけど psp では速度的に相性が悪い。
	これらは分離する予定です。
	-------------------------------------------------------
	試行錯誤したけど「ゲーム中のシナリオ処理」は SDL とかじゃ
	処理落ちが激しくて遅くて話にならないし。
	「エンディング等のシナリオ処理」は、別に速度は要らないので、
	(一応Gu化して) SDL の前の方式っぽく戻した方が良いかも知れない。
	-------------------------------------------------------
	本家もこの２つは全然違う処理になってるっぽいです。
---------------------------------------------------------*/

#include "scenario_script.h"

#include "kanji_system.h"

#include "111_my_file.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

//extern int errno;

// _script_command

// [テキストのカーソル制御]

// [テキスト表示/クリック待ち]
//		CURSOR		/*	CURSOR	カーソルの初期化 */
//					/*	CURSOR [廃止]	記憶したカーソル位置の呼び出し */
//					/*	CURSOR [廃止]	カーソル位置の記憶 */
//		CLICK		/* クリック待ち */
//		TEXT		/* メッセージウィンドウへの書き込み */

//		BG_LOAD 	/* LO ADBG		背景ファイルの変更 */
//		OBJ_LOAD	/* LO ADSP		汎用スプライトのロード */
//		OBJ_SWAP	/* RE LO ADSP	汎用スプライトの再読み込み */

//		OBJ_MOVE	/* MO VESP		スプライトの移動 */
//		OBJ_SPEED	/* MO VESP		スプライトの速度 */



//----------- [廃止]
//		SUBG		/* [廃止] [SCREEN]	SU BG背景の表示/非表示 */
//		SUTWINDOW	/* [廃止] [SCREEN]	SU TWINDOW 強調立ち絵ウィンドウの表示/非表示 para2はスコアパネルのon/off */

//----------- [廃止]
//		CUR_HOME	/* [廃止] [CURSOR]	CL CURSOR	カーソルの初期化 */
//		CUR_POP 	/* [廃止] [CURSOR]	LO ADCUR	記憶したカーソル位置の呼び出し */
//		CUR_PUSH	/* [廃止] [CURSOR]	SA VECUR	カーソル位置の記憶 */

//		SUSPRITE	/* [廃止] [廃止    ] SU SPRITE	汎用スプライトの表示/非表示 */
//		OBJ_SET 	/* [廃止] [廃止    ] PA RAMSP	汎用スプライトのアルファセット/アニメーション	*/


//		BGTEXT		/*					 BG TEXT	背景にテキストを打ち込む */
//		FILTER		/* [廃止] [廃止    ] FI LTER */
//		SUFILTER	/* [廃止] [廃止    ] SU FILTER	フィルター表示/非表示&透過度 */

//		SUWINDOW	/* [廃止] [廃止    ] SU WINDOW	メッセージウィンドウの表示/非表示 para2はbpp速度。*/
//		SUL 		/* [廃止] [SU SPRITE] SU L		立ち絵左スプライトの表示/非表示 */
//		SUR 		/* [廃止] [SU SPRITE] SU R		立ち絵右スプライトの表示/非表示 */
//		JUMP		/* [廃止] [廃止    ] JU MP		pauseボタンを押した時のjump先。 */ /*廃止*/
//		LOADL		/* [廃止] [LO ADSP ] LO ADL 	立ち絵左 スプライトのロード */
//		LOADR		/* [廃止] [LO ADSP ] LO ADR 	立ち絵右 スプライトのロード */
//		WAIT		/* [廃止] [廃止    ]  wait */

//		TWINDOW 	/* TW INDOW[廃止] [廃止    ]  立ち絵強調ウィンドウの初期化 */

enum
{
	I_CODE_DONE = 0,	/* 処理済み(このコマンドは終わった) */
// [テキストのカーソル制御]
//	I_CODE_CUR_POP,
//	I_CODE_CUR_PUSH,
	I_CODE_CUR_HOME,
// [テキスト表示/クリック待ち]
	I_CODE_CLICK,
	I_CODE_BG_WRITE_TEXT,
	I_CODE_TEXT_WRITE_TEXT,
	I_CODE_TEXT,
//
	I_CODE_BGM,
	I_CODE_BGM_VOLUME,
//
//	I_CODE_FILTER,/*廃止*/
//	I_CODE_JUMP,/*廃止*/
	I_CODE_BG_LOAD,
//	I_CODE_LOAD_LR,/*廃止*/
	I_CODE_OBJ_LOAD,
	I_CODE_OBJ_MOVE,
//	I_CODE_OBJ_SPEED,/*廃止*/
//	I_CODE_OBJ_SET,/*廃止*/
	I_CODE_OBJ_SWAP,

	I_CODE_SCREEN_BG,
	I_CODE_SCREEN_TEXT,
	I_CODE_SCREEN_PANEL,

	I_CODE_BOSS_LOAD,
	I_CODE_BOSS_START,

//	I_CODE_SUFILTER,/*廃止*/
//	I_CODE_SUL,/*廃止*/
//	I_CODE_SUR,/*廃止*/
//	I_CODE_SUSPRITE,/*廃止*/
//	I_CODE_SUWINDOW,/*廃止*/
//	I_CODE_TWINDOW,/*廃止*/
//	I_CODE_WAIT/*廃止*/
};

typedef struct _scenario_script
{
	int i_code; 		/* Interprited Code. / Intermediate Language, Command 中間言語 / 終わったかどうか */	//	int owattayo;
	int douji_chain;	// 同時実行かどうか
						// 0=違うよ 1=1つ目 2=2つ目
	int time_out;		/* 0:初回, それ以外初回ではない  各スクリプトコマンドごとの初期化判定 */
//
	int para1;
	int para2;
	int para3;
//	int para4;
	struct _scenario_script *next;
//
	char para0[(7*32)/*200*/];/* 32の倍数 */
} S_SCRIPT;/* == 256bytes */


typedef struct /*_script_sprite*/
{
	int cx256;			/* objの中心座標, 精度確保用(256固定小数点形式) */ 	//	int/*int*/ x;		/* x座標だよ */
	int cy256;			/* objの中心座標, 精度確保用(256固定小数点形式) */ 	//	int/*int*/ y;		/* y座標だよ */
//[4]
	int w;				/* objの幅 */			//	int cw; 	/* 中心座標(幅) */
	int h;				/* objの高さ */			//	int ch; 	/* 中心座標(高さ) */
//
	int target_x256;	/* 目標座標 */
	int target_y256;	/* 目標座標 */
//[8]
	int set_speed;		/* 移動速度 */		//	int move_wait;		/* 動きがあったときの制御用 */
	int angle1024;		/* 正確な方向 */	//	int r_course;	/* 大体の方向 */
//[12]
	SDL_Surface *img;	/* Images */
//
//	u8 alpha255;				/* alpha値 */
//	u8 flags;					/* 0:非表示, 1:表示, 2:tachie_window(2nd screen)に表示	ここでは表示させるかどうかだけ */
} SCRIPT_SPRITE;

/*---------------------------------------------------------

---------------------------------------------------------*/

/*************グローバル変数**************/
/* zenkaku.c */

/*****************************************/

/*	pspでは ここで初期化しても正常動作しない場合がある。
	起動時に初期化しないとダメ。(PCのソースをpspでコンパイルして動かない原因のひとつ) */



//
static S_SCRIPT *memory_script_list;			/* 命令保持用 */	/*=NULL*/
static S_SCRIPT *search_script_list;			/* 命令実行用 */	/*=NULL*/
static S_SCRIPT *loading_script_list;			/* 命令収集用 */
//
#define SPRITE_tachie_l (29&1)/*-3*/
#define SPRITE_tachie_r (30&1)/*-2*/
//#define SPRITE_tachie_m (31)/*-1*/
#define SCRIPT_TACHIE_OBJ_MAX		(2/*32*/)
static SCRIPT_SPRITE *std_obj[SCRIPT_TACHIE_OBJ_MAX/*32*/ /*20*/];		/* 汎用スプライト */
//static SCRIPT_SPRITE *tachie_r;
//static SCRIPT_SPRITE *tachie_l;
//


//

static int is_bg;	/*=0*/					/* 背景表示/非表示フラグ */

//static int 380/*scenario_width*/; 			/* 横幅 */

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_local01(char *filename, SDL_Surface *s2)// /*result*/, int alpha)
{
	char file_name[128];
	strcpy(file_name, DIRECTRY_NAME_DATA_STR "/" );
	strcat(file_name, filename);
//
	if (NULL != s2)
	{
		SDL_FreeSurface(s2);
		s2 = NULL;
	}
//
	SDL_Surface *s1;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(file_name);/*ここでロードすると確実に処理落ちするよ*/
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, (char*)"script:cant load image %s:", file_name );
//		return (NULL);
	}
#if 0
	if (	(-1==alpha) /* スクリプト中で省略した場合(デフォルト) */
		||
			(0==alpha)	/* 明示した場合 */
	)
#endif
	{
		/* アルファ使わない */
		s2 = SDL_DisplayFormat(s1);/*サーフェスを表示フォーマットに変換する。*/
		#if 1/* 現状SDL背景がある、 story で要る。 */
		SDL_SetColorKey(s2, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);	/* カラーキー(抜き色、透明色)は黒 */
		#endif
	}
#if 0
	else
	{
		/* アルファ使う */
		s2 = SDL_DisplayFormatAlpha(s1);/*サーフェスを表示フォーマットに変換する。*/
	}
#endif
	#if 0
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, (char*)"cant convert image %s to display format:", file_name );
//		return (NULL);
	}
	#endif
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
#include <malloc.h>/* free(); */

static void remove_script_sprite(SCRIPT_SPRITE *src)
{
	if (NULL != src)
	{
		SDL_FreeSurface(src->img);
		free(src);
		src = NULL;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void swap_my_sprite(S_SCRIPT *ssc)
{
	char *filename;
	int num;
//	int dummy_frames;
//	int alpha;
//
	filename		= ssc->para0;
	num 			= ssc->para1;
//	dummy_frames	= ssc->para2;
//	alpha			= ssc->para3;
//
	SCRIPT_SPRITE *tmp;
	if (-1==num)		{	return; 		}
//	else if (-2==num)	{	tmp=std_obj[SPRITE_tachie_r/*32-2*/];	}	/* right */
//	else if (-3==num)	{	tmp=std_obj[SPRITE_tachie_l/*32-3*/];	}	/* left */
	else				{	tmp=std_obj[(num)&(SCRIPT_TACHIE_OBJ_MAX-1)];	}	/* 汎用スプライト */
	if (NULL==tmp)		{	return; 		}

	SDL_FreeSurface(tmp->img);

	SDL_Surface *suf = NULL;
	suf 		= load_local01(filename, suf);//, 0/*alpha*/);
	tmp->img	= suf;
//	tmp->frames = frames;
	tmp->w		= (suf->w/*/frames*/);	//tmp->cw		= ((tmp->w)>>1);
	tmp->h		= (suf->h); 			//tmp->ch		= ((tmp->h)>>1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

	/*int dummy_frames, int dummy_alpha, SDL_Rect *rect*/
static /*SCRIPT_SPRITE **/void load_my_sprite(S_SCRIPT *ssc)
{
	char *filename;
	int sc_sp_num;
	int xx;
	int yy;
//
	filename	= ssc->para0;
	sc_sp_num	= ssc->para1;
	xx			= ssc->para2;
	yy			= ssc->para3;
//
	sc_sp_num &= ((SCRIPT_TACHIE_OBJ_MAX-1));
	if (NULL != std_obj[sc_sp_num]) 	{	remove_script_sprite(std_obj[sc_sp_num]); }
	SCRIPT_SPRITE *tmp;
	tmp 				= my_calloc(sizeof(SCRIPT_SPRITE));
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename, 0/*alpha*/);
	suf 				= load_local01(filename, suf);//, 0/*alpha*/);
	if (NULL==suf)		{ return /*(NULL)*/;			}
	std_obj[sc_sp_num]	= tmp;
	tmp->img			= suf;
//	if (frames==-1) 	{	tmp->frames = 1;		}
//	else				{	tmp->frames = frames;	}
	tmp->w				= (suf->w/*/frames*/);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
//
//	tmp->ak tframe		= 0;
//	tmp->an im_speed	= 0;
//	tmp->an im_type 	= 0;
//	tmp->an im_time 	= 0;
//	tmp->an im_count	= 0;
	/*tmp->alpha255 	= 255;*/	/*tmp->color32		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);*/
//	tmp->flags			= 0;
//	tmp->move_wait		= 0;
	tmp->cx256			= (xx<<8);	/*rect->x*/
	tmp->cy256			= (yy<<8);	/*rect->y*/
//	tmp->x				= rect->x-(rect->w*tmp->w);
//	tmp->y				= rect->y-(tmp->h);
//	tmp->x				= xx-(0*tmp->w);
//	tmp->y				= yy-(tmp->h);

//	return (tmp);
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static int inits; 							/* 各スクリプトコマンドごとの初期化判定 */

/*---------------------------------------------------------
	I_CODE_OBJ_MOVE スプライトの移動
---------------------------------------------------------*/
//		int n = parth_str_right_or_str_left_or_number(l_or_r);
//		if (n==STR_ERROR/*-1*/) 		{	return (2/*-1*/);	}/*スクリプト終了*/
	//	else if (n==STR_RIGHT/*-2*/)	{	sc = tachie_r;	}	/* right */
	//	else if (n==STR_LEFT/*-3*/) 	{	sc = tachie_l;	}	/* left */
	//	else

//static void set_sprite_speed( int num, int speed_aaa)
//{
//	SCRIPT_SPRITE *tmp;
//	tmp=std_obj[(num)&(SCRIPT_TACHIE_OBJ_MAX-1)];		/* 汎用スプライト */
//	//std_obj[((num)&(SCRIPT_TACHIE_OBJ_MAX-1))].set_speed = speed_aaa;
//	tmp->set_speed = speed_aaa;
//}
static int do_move_script_sprite(S_SCRIPT *ssc)/*, int speed_aaa, int alpha255*/	/*,ssc->para4*/
{
	char *color32_str;
	int num;
	int x;
	int y;
//
	color32_str = ssc->para0;
	num = ssc->para1;
	x = ssc->para2;
	y = ssc->para3;
//
	SCRIPT_SPRITE *my_std_obj;
	my_std_obj = std_obj[((num)&(SCRIPT_TACHIE_OBJ_MAX-1))];	/* 汎用スプライト */
//
	if (0 == ssc->time_out/*inits*/)	/* 初回のみ行う */
	{
		ssc->time_out = 1;/*初回ではない*/
		/* あらかじめ移動完了位置を算出しておく */
		my_std_obj->target_x256 = my_std_obj->cx256 + (x<<8);		/* 移動完了座標 */
		my_std_obj->target_y256 = my_std_obj->cy256 + (y<<8);		/* 移動完了座標 */
//		my_std_obj->cx256		= ((my_std_obj->cx256)/*<<8*/);		/* 精度確保用 */
//		my_std_obj->cy256		= ((my_std_obj->cy256)/*<<8*/);		/* 精度確保用 */
		/* 移動方向を決める */
	//	my_std_obj->angle1024	= at an_1024(y,x);
		my_std_obj->angle1024	= atan_65536(y,x);
		my_std_obj->angle1024	>>= (6);
	//	my_std_obj->color32 	= ((alpha255)<<8);	/* 色 */
	//	my_std_obj->alpha		= ((alpha255)<<8);	/* 色 */
		if ('f'==color32_str[2])
		{
		//	my_std_obj->alpha255 = (255);	/* 色 */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (255) );
		}
		else
		{
		//	my_std_obj->alpha255 = (180);	/* 色 */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (180) );
		}
//		my_std_obj->set_speed = ((40)<<(8-3));	/* 初速度 (x32) */
//		my_std_obj->set_speed = ((20)<<(8-3));	/* 初速度 (x32) */
		my_std_obj->set_speed = (0);			/* 初速度 (x32) */
	}
//
	{
		/* 10で割るのは遅過ぎるので、3ビットシフトして(1/8)の値にする */
		int speed256;
		speed256 = (/*speed_aaa*/(my_std_obj->set_speed));		/* 精度確保用 */
	//	my_std_obj->set_speed += (5*32);			/* 160==5x32(速すぎ) 加速度(東方の立ち絵は常に加速して移動する) (x32) */
	//	my_std_obj->set_speed += (4*32);			/* 128==4x32() 加速度(東方の立ち絵は常に加速して移動する) (x32) */
		my_std_obj->set_speed += (3*32);			/*	96==3x32() 加速度(東方の立ち絵は常に加速して移動する) (x32) */
		if (t256(6.0)<speed256) speed256 = t256(8.0);		/* 最大速度8[dots/flame] t256(6.0) */

		#if 0
		/* CWの場合 */
	//	my_std_obj->x += ((cos512(my_std_obj->angle512)*speed)/10);
	//	my_std_obj->y += ((sin512(my_std_obj->angle512)*speed)/10);
	//	my_std_obj->cx256 += ((cos512(my_std_obj->angle512)*speed256)>>8);	/* 精度確保用 */
	//	my_std_obj->cy256 += ((sin512(my_std_obj->angle512)*speed256)>>8);	/* 精度確保用 */
		#else
		/* CCWの場合 */
		my_std_obj->cx256 += ((sin1024(my_std_obj->angle1024)*speed256)>>8); /* 精度確保用 */
		my_std_obj->cy256 += ((cos1024(my_std_obj->angle1024)*speed256)>>8); /* 精度確保用 */
		#endif
	}
//
//	my_std_obj->cx256	= (/*t256_floor*/(my_std_obj->cx256));		/* 精度確保用 */
//	my_std_obj->cy256	= (/*t256_floor*/(my_std_obj->cy256));		/* 精度確保用 */
	/* 大体の方向: なので ４５度回転してずらして、上位２ビットが大まかな方向 */
	#if 0
	/* CWの場合 */
	switch ((((my_std_obj->angle512+64/*deg_360_to_512(45)*/) & 0x180 )))/*my_std_obj->r_course*/
	{	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
	case (0<<7)/* 1:→(512_0) */:	if (my_std_obj->cx256 >= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:↓(512_1) */:	if (my_std_obj->cy256 <= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:←(512_2) */:	if (my_std_obj->cx256 <= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:↑(512_3) */:	if (my_std_obj->cy256 >= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	}
	#else
	/* CCWの場合 */
//	switch ((((my_std_obj->angle512-64/*deg_360_to_512(45)*/) & 0x180 )))/*my_std_obj->r_course*/
	switch ((((my_std_obj->angle1024-128/*deg_360_to_512(45)*/) & 0x300 )))/*my_std_obj->r_course*/
	{	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
//	case (0<<7)/* 2:↓(512_1) */:	if (my_std_obj->y <= my_std_obj->target_y)	{	goto move_complete; 	}	break;
//	case (1<<7)/* 1:→(512_0) */:	if (my_std_obj->x >= my_std_obj->target_x)	{	goto move_complete; 	}	break;
//	case (2<<7)/* 0:↑(512_3) */:	if (my_std_obj->y >= my_std_obj->target_y)	{	goto move_complete; 	}	break;
//	case (3<<7)/* 3:←(512_2) */:	if (my_std_obj->x <= my_std_obj->target_x)	{	goto move_complete; 	}	break;
	case (1<<8)/* 2:↓(1024_1) */:	if (my_std_obj->cy256 <= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	case (0<<8)/* 1:→(1024_0) */:	if (my_std_obj->cx256 >= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	case (3<<8)/* 0:↑(1024_3) */:	if (my_std_obj->cy256 >= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	case (2<<8)/* 3:←(1024_2) */:	if (my_std_obj->cx256 <= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	}
	#endif
	return (0);/*移動中*/
move_complete:
	/* 移動完了した場合は、正確な座標に修正する。 */
	my_std_obj->cx256 = my_std_obj->target_x256;
	my_std_obj->cy256 = my_std_obj->target_y256;
	return (1);/*移動完了*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global /*static*/ int draw_script_screen/*=0*/; 				/* せりふウィンドウ表示フラグ */

/*---------------------------------------------------------
	システム処理(???)
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static void load_script_free(void)
{
	S_SCRIPT *l = memory_script_list;
	S_SCRIPT *n;
	while (NULL != l)
	{
		n = l->next;
		free (l);
		l = n;
	}
	memory_script_list = (S_SCRIPT *)NULL;
	loading_script_list = (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_command(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	while ((*c != ' ') && (*c != 13))
	{
		i++;
		if (i >= 10)
		{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	if (*c == 13)	/* 改行コードは OD OA */
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_int(char *ccc, int *number, int *line_terminate_end_flag)
{
	char buffer[32/*20*/];
	char *ddd = buffer;
	int i = 0;
	while ((isdigit(*ccc))||('-'==(*ccc)))		/* 負数にも対応 / isdigit : 数字かどうかの判定 */
	{
		i++;
		if (i >= 32/*20*/)
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
/* ',' または '\n' が来たら終了 */

//
	if (13==(*ccc)) /* 改行コードは OD OA */
	{
		*line_terminate_end_flag = 1;
	}
	*ddd = 0;
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = -1; }
	else														{	*number = atoi(buffer); }
	return (ccc);
/*error*/
ne222:
	return ((char *)NULL);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	PARAM_01 = 0,
	PARAM_02,
	PARAM_03,
//	PARAM_04,/*未使用*/
//	PARAM_05,/*未使用*/
//	PARAM_06,/*未使用*/
//	PARAM_07,/*未使用*/
//	PARAM_08,/*未使用*/
	PARAM_MAX/*最大数*/
};

static void regist_script(
	char *command,
	char *c_p0,
//	int c_p1,	//c_pn[0]
//	int c_p2,	//c_pn[1]
//	int c_p3,	//c_pn[2]
//	int c_p4,	//c_pn[3]
//	int c_p5,	//c_pn[4]
//	int c_p6,	//c_pn[5]
//	int c_p7,	//c_pn[6]
	const int *c_pn,
	int chain)
{
/*
	基本形
	command para0,para1,para2,para3
*/
	S_SCRIPT *new_script;
	new_script				= my_calloc(sizeof(S_SCRIPT));

	strcpy(new_script->para0, c_p0);
	new_script->para1		= /*c_p1*/c_pn[PARAM_01];	/* デフォルト */
	new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
	new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */
//	new_script->para4		= /*c_p3*/c_pn[PARAM_04];	/* デフォルト */
//
//	new_script->i_code		= I_CODE_DONE;/*owattayo	= 0;*/
	new_script->douji_chain = chain;
	new_script->next		= NULL;
	new_script->time_out	= 0;


	switch (*command)
	{
	case 'B':
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_script->i_code = I_CODE_BG_LOAD;			}	/* 背景ファイルの変更 */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_script->i_code = I_CODE_BGM;				}	/* BGM変更 */
			else	if (0==tiny_strcmp(command, "BGM_VOLUME"))	{	new_script->i_code = I_CODE_BGM_VOLUME; 		}	/* BGM 音量 変更 */
		if (0==tiny_strcmp(command, 			"BOSS"))
		{
					if (0==tiny_strcmp(c_p0,	"load"))		{	new_script->i_code = I_CODE_BOSS_LOAD;			}	/* ボス読み込み */
			else	if (0==tiny_strcmp(c_p0,	"start"))		{	new_script->i_code = I_CODE_BOSS_START; 		}	/* ボス攻撃開始。 */
		}
		break;

		break;
	case 'C':
		if (0==tiny_strcmp(command, 			"CURSOR"))
		{
					if (0==tiny_strcmp(c_p0,	"home"))		{	new_script->i_code = I_CODE_CUR_HOME;			}	/* カーソルの初期化 */
//			else	if (0==tiny_strcmp(c_p0,	"load"))		{	new_script->i_code = I_CODE_CUR_POP;			}	/* 記憶したカーソル位置の呼び出し */
//			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_script->i_code = I_CODE_CUR_PUSH;			}	/* カーソル位置の記憶 */
			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_script->i_code = I_CODE_CLICK;				}	/* クリック待ち */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->i_code = I_CODE_TEXT_WRITE_TEXT;	}	/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->i_code = I_CODE_BG_WRITE_TEXT;		}	/* BGウィンドウに漢字の文字を描く様に設定する。 */
		}
		break;
	case 'O':
					if (0==tiny_strcmp(command, "OBJ_MOVE"))	{	new_script->i_code = I_CODE_OBJ_MOVE;			}	/* スプライトの移動 */
			else	if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_script->i_code = I_CODE_OBJ_LOAD;			}	/* 汎用スプライトのロード */
			else	if (0==tiny_strcmp(command, "OBJ_SWAP"))	{	new_script->i_code = I_CODE_OBJ_SWAP;			}	/* スプライトの再読み込み */
//			else	if (0==tiny_strcmp(command, "OBJ_SPEED"))	{	new_script->i_code = I_CODE_OBJ_SPEED;			}	/* スプライトの速度設定 */
		break;
	case 'S':
		if (0==tiny_strcmp(command, 			"SCREEN"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_script->i_code = I_CODE_SCREEN_PANEL;		}	/* スコアパネルの表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->i_code = I_CODE_SCREEN_TEXT;		}	/* テキスト画面の表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->i_code = I_CODE_SCREEN_BG;			}	/* 背景の表示/非表示 */
		}
		break;
	case 'T':
					if (0==tiny_strcmp(command, "TEXT"))		{	new_script->i_code = I_CODE_TEXT;				}	/* メッセージウィンドウへの書き込み */
		break;
	}

	if (NULL==loading_script_list)/* 先頭なら */
	{
		loading_script_list 		= new_script;
	}
	else
	{
		loading_script_list->next	= new_script;
		loading_script_list 		= loading_script_list->next;
	}

	if (NULL==memory_script_list)
	{	memory_script_list			= loading_script_list;	}
}



/*---------------------------------------------------------
	スクリプト読み込み
	-------------------------------------------------------
	この関数は、仕様上「とても処理落ちします」

---------------------------------------------------------*/

static int s_load_scenario(void)
{
/*
	基本形
	command para0,para1,para2,para3
*/

//	FILE *fp;
	if (NULL == (/*fp =*/my_fopen(/*my_fopen_file_name*/ /*,"r"*/)))		/* 開けなかったとき */
	{
		return (0);
	}

	int opecode_entrys		= 0;	/* 命令がが書き込まれているかどうか。 */
	int line_num			= 0;	/* 行番号 */
	int opecode_chains		= 0;	/* 連続した命令 */
	while (/*NULL*/0 != my_file_fgets(/*buffer_text_1_line,255,fp*/))
	{
		/****************** script_data 用 ******************/
		char char_command[16/*15*/];		/* 基本コマンド */
		char c_p0[256/*200*/];
		int c_pn[PARAM_MAX/*6*/];
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				c_pn[kk] = (-1);	/* 無指定の場合 を判別する為 */
			}
		}
		/****************** script_search 用 ****************/
		int end_arg;	end_arg = 0;		/* 行末 == 引数の取得の中止 */
		line_num++;
//
		char *ch;					/* 走査位置 */
		ch = buffer_text_1_line;
		/* skiped lines. */
		#if 0
		/* '\n'が悪いのか巧くいかない(???) */
		if ('\n'==(*ch))		{	continue;	}	/* 改行のみの行は空行なのでやらないでとばす */
		while (isspace(*ch))	{	ch++;		}	/* 空白やTABを除去 */
		#else
		{my_isspace:;
			if (' '>=(*ch))
			{
				ch++;
				if (0x0a==(*ch))
				{	/*goto loop;*/continue; }	/* skiped null line. */
				else
				{	goto my_isspace;	}
			}
		}
		#endif
		/* ';'で始まる行はコメント行なので、次の行まで飛ばす。 */
//		if (';'==(*ch)) 	{	continue;	}	/* ';'で始まる行はコメントなのでやらないでとばす */
		/* '-'で始まる行はスクリプトチェイン機能(複数スクリプト行、同時実行機能)。 */
		if ('-'==(*ch)) 	{	ch++;	opecode_chains++;	opecode_chains &= 0x0f; }	/* ワークが16までしかないので最大16命令 */
		else				{	opecode_chains = 0; }
//
	#if (1==USE_PARTH_DEBUG)
		#define GOTO_ERR_WARN goto err_warn
	#else
		#define GOTO_ERR_WARN continue
	#endif
		/* parth start */
		ch = load_command(ch, char_command, &end_arg);		/* 基本コマンド名称(オペコード)読み込み  */
		if (NULL==ch)										{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		if (!end_arg)/* 行末 */
		{
			if (' ' != (*ch))								{	GOTO_ERR_WARN;	}	/* 区切り */
			ch++;
			ch = load_my_file_get_str(ch, c_p0, &end_arg/*, ','*/); 						/* 文字列コマンド(オペランド)読み込み  */
			if (NULL==ch)									{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		}
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*(6)*/; kk++)
			{
				if (!end_arg)
				{
					if (',' != (*ch))						{	GOTO_ERR_WARN;	}	/* 区切り */
					ch++;
					ch = load_int(ch, &c_pn[kk], &end_arg); 						/* 数値コマンド(オペランド)読み込み  */
					if (NULL==ch)							{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
				}
			}
		}
	//	regist_script(char_command, c_p0, c_pn[0],c_pn[1],c_pn[2],c_pn[3],c_pn[4],c_pn[5],c_pn[6], opecode_chains);
		regist_script(char_command, c_p0, c_pn, opecode_chains);
		opecode_entrys++;
		continue;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(ERR_WARN, (char*)"syntax error in scriptfile '%s', line no: %d", my_fopen_file_name, line_num);
		continue;
	#endif
	}
	my_fclose (/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		#if 0
		//ps pDebugScreenPrintf("can't entrys from this file %s", my_fopen_file_name);
		//sc eKernelDelayThread(3000000);
		#endif
		error(ERR_WARN, (char*)"can't entrys from this file %s", my_fopen_file_name);
		return (0);
	}
	return (1);
}

/*---------------------------------------------------------
	スクリプトシステム用
	カーソル位置キャッシュ
---------------------------------------------------------*/

static int cursor_x_chached;			/* カーソル位置 保存用 */
static int cursor_y_chached;			/* カーソル位置 保存用 */

/*---------------------------------------------------------
	スクリプトにおける背景
---------------------------------------------------------*/

#if (1)
static UINT16 *bg_story_window_surface_image;
static int bg_story_window_surface_pitch;
static int bg_story_window_surface_width;
#endif

static SDL_Surface *bg_story_window_surface;			/* スクリプトにおける背景 */

/*---------------------------------------------------------

---------------------------------------------------------*/

//static extern int my_string_offset;
/*static*/ extern void clear_my_string_offset(void);
static void aaa_script_reset(void)
{
	int i;
	for (i=0; i<SCRIPT_TACHIE_OBJ_MAX/*20*/; i++)
	{
		remove_script_sprite(std_obj[i]);
	}
//	remove_script_sprite(std_obj[SPRITE_tachie_r]);
//	remove_script_sprite(std_obj[SPRITE_tachie_l]);
	if (NULL != bg_story_window_surface)
	{
		SDL_FreeSurface(bg_story_window_surface);
		bg_story_window_surface 	= NULL;
	}
	is_bg = 0;/* 開放したんだから bg 表示できない、したがって強制表示 off */
	//

	home_cursor();			/* カーソルをホームポジションへ移動 */	/* カーソル位置 */
	cursor_x_chached = 0;	/* カーソル位置 保存用 */
	cursor_y_chached = 0;	/* カーソル位置 保存用 */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	is_bg が 1 になる可能性のある場所は、bg_story_window_surface が無いと
	描画でハングアップするから、チェックする。
	bg_story_window_surface が無い場合は強制off。
---------------------------------------------------------*/

static void check_is_bg(void)
{
	if (NULL == bg_story_window_surface)
	{
		is_bg = 0;/* 開放したんだから bg 表示できない、したがって強制表示 off */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* 背景α値用 */
static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window_surface = load_local01(filename, bg_story_window_surface);//, alpha);
	{
		bg_story_window_surface_image	= (UINT16 *)bg_story_window_surface->pixels;
		bg_story_window_surface_pitch	= bg_story_window_surface->pitch;
		bg_story_window_surface_width	= bg_story_window_surface->w;
	}
	#if 0/* 現状、要らない？。 */
	SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	#endif
}
static int load_my_bg(S_SCRIPT *ssc)
{
	char *filename;
	int alpha;
	int fade_command;
	int set_alpha_speed;
//
	filename		= ssc->para0;
	alpha			= ssc->para1;
	fade_command	= ssc->para2;
	set_alpha_speed = ssc->para3;
//
	static int bg_alpha_speed;		/* 背景α値用 */
	switch (fade_command)
	{
	case (-1):
		load_bg_aaa(filename, alpha);
	//	ssc->owattayo = 1;
		return (1); /* 処理完了 */
//		break;
	case 1:/*fade_in_command*/
		if (0 == ssc->time_out/*inits*/)	/* 初回のみ行う */
		{
			ssc->time_out = 1;/*初回ではない*/
		//if (inits)/*fade_in_command_start*/
		//{
		//	if (filename)
			{
				load_bg_aaa(filename, 0/*alpha*/);
			}
		//	ssc->owattayo = 1;
			//
			bg_alpha = 0;
		//	if (-1 == set_alpha_speed)/*パラメーター省略時*/
			{
				set_alpha_speed = 5;
			}
			bg_alpha_speed = set_alpha_speed;
		//	inits=0;
			is_bg				= 1;	check_is_bg();
		}
		else
		{
//			psp_clear_screen();
//			SD L_SetAlpha(bg_story_window_surface, SD L_SRCALPHA, bg_alpha);
//			SD L_BlitSurface(bg_story_window_surface, NULL, sdl_screen[SD L_00_SCREEN], NULL);
			bg_alpha += bg_alpha_speed/*5*/;
			if (250 < bg_alpha)
			{
				bg_alpha = 255;
				#if 0/*???*/
				/*	何か良くわからないけど、ここはおかしい???。 */
				if (NULL != bg_story_window_surface)
				{
					unloadbmp_by_surface(bg_story_window_surface);	//キャッシュに入ってるのでNULLに出来ない。bg_story_window_surface = NULL;
				}
				#endif
				return (1); 	/* 処理完了 */
			}
		}
		break;
//	case 2:/*fade_out_command_start*/
//		if (inits)
//		{
//			inits=0;
//		}
//		else
//		{
//		//	if ()
//			{
//			//	return (1); /* 処理完了 */
//			}
//		}
//		break;
	}
	return (0); 	/* 処理中 */
}
#define USE_SDL_DRAW (1)
#if (1==USE_SDL_DRAW)

/*---------------------------------------------------------
	スプライトオブジェクトの描画/動作
---------------------------------------------------------*/

static void draw_my_sprite(SCRIPT_SPRITE *std_obj_nnn_/*int nnn*/)
{
//	int nnn;
//	for (nnn=start; nnn<=end; nnn++)
	{
		if (NULL != std_obj_nnn_)
		{
		//	if (std_obj_nnn_->flags)	/* 汎用スプライトの表示/非表示 */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0)/*std_obj_nnn_->w*std_obj_nnn_->aktframe*/;
				src_r.y = (0);
				src_r.w = std_obj_nnn_->w;
				src_r.h = std_obj_nnn_->h;
				dst_r.x = (t256_floor(std_obj_nnn_->cx256));
				dst_r.y = (t256_floor(std_obj_nnn_->cy256));
				SDL_BlitSurface(std_obj_nnn_->img, &src_r, sdl_screen[SDL_00_VIEW_SCREEN], &dst_r);
			}
			/* 動き/アニメーション */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_draw_the_script(void)
{
	/* ----- 描画系 ----- */
	/* 1. まず背景を描く */
	if (is_bg)
	{
		if (255 > bg_alpha)
		{
			SDL_SetAlpha(bg_story_window_surface, SDL_SRCALPHA, bg_alpha);
		}
		SDL_BlitSurface(bg_story_window_surface, NULL, sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	}
	/* 2. 次に立ち絵を描く */
	draw_my_sprite(std_obj[SPRITE_tachie_l]);/* 立ち絵 1枚 */
	draw_my_sprite(std_obj[SPRITE_tachie_r]);/* 立ち絵 1枚 */
}
#endif /* (1==USE_SDL_DRAW) */


/*---------------------------------------------------------
	作り直す必要がある。
	欠陥があるので描画後にしか出来ない。
---- やりたいこと ----
	1フレームでは終わらない処理があったときのためにこのコマンドからの命令には全て
	該当コマンド終了の場合、中間コード sc->i_code = I_CODE_DONE;に代入で終了。
	常にchainを確認し、0以外の値が入っていたら次の関数も実行する(nextを辿る)。
---------------------------------------------------------*/

static int script_skip_mode;

enum
{
	SCRIPT_00_CONTINUE = 0,
	SCRIPT_01_TERMINATE,
};
static int script_terminate_flag;
static /*int*/void work_the_script(void)
{
	S_SCRIPT *ssc;
	ssc = search_script_list;/* 検索開始位置 */
//
	int shot_ositenai;
	shot_ositenai = 1;	/* ショット押してない */
	if (0 == my_pad)/* 今は押してない */
	{
		if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	/* キャンセル入力 */
		{
			script_skip_mode = 1;/*on*/
			#if 0
			while ((NULL != ssc->next)
			//	&& (I_CODE_JUMP != ssc->i_code)/*廃止*/
				)
			{
				ssc->i_code = I_CODE_DONE;
				ssc = ssc->next;
			}
			if (NULL == ssc->next)
			{
				script_terminate_flag = SCRIPT_01_TERMINATE;/*(1)*/
				return;
			}
			#endif
		}
		if (my_pad_alter & PSP_KEY_SHOT_OK) 		/* ＯＫ入力 */
		{
			shot_ositenai = 0;	/* ショット押したよ */
		}
	}
//
	if (0!=script_skip_mode)
	{
		shot_ositenai = 0;	/* ショット押したよ */
	}
//
	/* ----- スクリプトコマンド解釈(実行)系 ----- */
//	/*static*/ int n9;			/* 次の命令に進めるかの判定 */
//	n9 = 0; 					/* 次の命令に進めるかの判定 */
	while (1)
	{
	//	if (0==ssc->owattayo)
		{
			switch (ssc->i_code)
			{
			// [テキストのカーソル制御]
//			case I_CODE_CUR_POP:		/* カーソル位置、復元 */
//				cursor_x = cursor_x_chached;
//				cursor_y = cursor_y_chached;
//				ssc->i_code = I_CODE_DONE;
//				break;
//			case I_CODE_CUR_PUSH:		/* カーソル位置、記憶 */
//				cursor_x_chached = cursor_x;
//				cursor_y_chached = cursor_y;
//				ssc->i_code = I_CODE_DONE;
//				break;
			case I_CODE_CUR_HOME:		/* カーソルの初期化 */
				home_cursor();				/* カーソルをホームポジションへ移動 */
				ssc->i_code = I_CODE_DONE;
				break;
			// [テキスト表示/クリック待ち]
			case I_CODE_CLICK:
				if (0 == shot_ositenai)
				{	ssc->i_code = I_CODE_DONE;	}
				break;
			case I_CODE_BG_WRITE_TEXT:
				//set_write_text(1);		/* BGウィンドウに漢字の文字を描く様に設定する。 */
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_TEXT_WRITE_TEXT:
				//set_write_text(0);		/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_TEXT:
			{	static int tmp_all[16/*15*/];	/* ご自由にお使いください。 */
				if (0 == ssc->time_out/*inits*/)	/* 初回のみ行う */
				{
					ssc->time_out = 1;/*初回ではない*/
				//if (inits)
				//{
					int i;
					for (i=0; i<16/*15*/; i++)
					{
						tmp_all[i] = 0;
					}
				}

				if (0 == tmp_all[(ssc->douji_chain)])
				{
					if (0 == shot_ositenai) 	{	ssc->para2=0;	}	/* ショット押したら、残りは全部書く */
					/*sdl_lock_surface,*/
					tmp_all[(ssc->douji_chain)] = print_kanji000(ssc->para0, ssc->para1, ssc->para2);
				}
				else	/*if (1==tmp_all[(ssc->chain)])*/
				{
					if (0x00 == (ssc->para3 & 0xf8))	/* 0レジスタと比較 */
					{
						if ( 0x00 == ( (ssc->para3 & shot_ositenai) & 0x01) )	/* 0レジスタと比較 */
						{
							if (0x00 != (ssc->para3 & 0x02))	/* 0レジスタと比較 */
							{
								home_cursor();				/* カーソルをホームポジションへ移動 */
							}
						//			//cursor_continue = 1;	/* カーソル継続 */
							if (0x00 != (ssc->para3 & 0x04))	/* 0レジスタと比較 */
							{
								kanji_window_clear();	/* メッセージウィンドウの内容を消す。 */
							//	msg_window_init();/*???*/		/* ウィンドウ初期化 */
							}
						//
							ssc->i_code = I_CODE_DONE;
							tmp_all[(ssc->douji_chain)] = 0;
						}
					}
				}
			}
				break;
#if 0
/*
	switch (ssc->para3)
	w: 0:しない 	   1:ウィンドウ初期化
	c: 0:カーソル継続  1:カーソル初期化
	b: 0:実行。 	   1:ボタンを押したら実行。
		   wcb
	case 0:000: 何もせずに		 次の命令へ、カーソル継続
	case 1:001: ボタンを押したら 次の命令へ、カーソル継続
	case 2:010: 何もせずに		 次の命令へ、カーソル初期化
	case 3:011: ボタンを押したら 次の命令へ、カーソル初期化
	case 4:100: 何もせずに		 次の命令へ、カーソル継続	 ウィンドウ初期化
	case 5:101: ボタンを押したら 次の命令へ、カーソル継続、  ウィンドウ初期化
	case 6:110: 何もせずに		 次の命令へ、カーソル初期化、ウィンドウ初期化
	case 7:111: ボタンを押したら 次の命令へ、カーソル初期化、ウィンドウ初期化
*/
#endif

			case I_CODE_BG_LOAD:
			//	bg_story_window_surface = load_local01(ssc->para0, bg_story_window_surface);//, 0 ssc->para1);
			//	SD L_SetColorKey(bg_story_window_surface, SD L_SRCCOLORKEY|SD L_RLEACCEL, 0x00000000);
			//	ssc->i_code = I_CODE_DONE;
				{	int aaa;
					aaa = load_my_bg(ssc);
					if (0!=aaa)
					{
						ssc->i_code = I_CODE_DONE;
					}
				}
				break;
			case I_CODE_OBJ_LOAD:	/* 汎用絵  立ち絵L	立ち絵R */
				load_my_sprite(ssc);
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_OBJ_MOVE:
				{	int aaa;
					aaa = do_move_script_sprite(ssc);
					if (0!=aaa)
					{
						ssc->i_code = I_CODE_DONE;
					}
				}
				break;
//			case I_CODE_OBJ_SPEED:
//				set_sprite_speed(/*ssc->para0,*/ssc->para1/*num*/, ssc->para2/*setspeed*/  /*,ssc->para3,ssc->para4*/);
//				ssc->i_code = I_CODE_DONE;
//				break;
			case I_CODE_OBJ_SWAP:
				swap_my_sprite(ssc);
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_SCREEN_BG:			is_bg					= ssc->para1;	check_is_bg();	ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_SCREEN_TEXT:		draw_script_screen		= ssc->para1;					ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_SCREEN_PANEL:		draw_side_panel 		= ssc->para1;					ssc->i_code = I_CODE_DONE;
				#if 1
				/* 本当は要らないが、現状では使いにくいのでパネル切り替え時に細工する。 */
				/* スキップモード、強制的に無効(スキップしない)。 */
				script_skip_mode = 0;/*off*/
				#endif
																												break;
			case I_CODE_BGM:				play_music_num( (ssc->para1) ); 		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BGM_VOLUME: 		set_music_volume( (ssc->para1) );		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_LOAD:			script_boss_load( (ssc->para1) );		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_START: 		script_boss_start( /*(ssc->para1)*/ );	ssc->i_code = I_CODE_DONE;	break;
			}
		}
//		if (0 == ssc->owattayo) 	{	n9++;	}		/* 継続フラグ */
		if (NULL==ssc->next)		{	break;	}		/* nextがNULLの場合 */
		if (
				(ssc->douji_chain >= ssc->next->douji_chain) ||
				(0==ssc->douji_chain)
			)
		{	break;	}	/* 現douji_chain が 次douji_chain より大きい場合 */
		ssc = ssc->next;
	}
//
//
//	inits = 0;
//	if (0 == n9)					/* 次の命令の許可 */
	{
//		inits = 1;
	aaa_loop:
		if (NULL == search_script_list)
		{
			goto loop_quit;
		}
		//while ( (I_CODE_DONE!=search_script_list->i_code) /*(search_script_list->owattayo)*/ )		/* 次の命令へ */
		if ((I_CODE_DONE == search_script_list->i_code))
		{
		//	if ()
		//	if (NULL == search_script_list )
		//	{
		//		break;
		//	}
//			if (NULL == search_script_list->next)
//			{
//				goto loop_quit;
//			}
//			else
			{
				search_script_list = search_script_list->next;/* 次回はここから調べる */
			}
			goto aaa_loop;
		}
	loop_quit:
		;
	}
//
	if (NULL == search_script_list )
	{
		script_terminate_flag = SCRIPT_01_TERMINATE;/*(1)*/
//		return /*(1)*/;
	}
	else
	{
		script_terminate_flag = SCRIPT_00_CONTINUE;/*(0)*/
//		return /*(0)*/;
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static void static_do_script_work_main(void)
{
	/*
	ちゃんと動作と描画が分離できてれば動作がこの位置にあっても問題ない筈だが、現状はハングアップする。
	*/
//
	#if 1/*Gu化完了したら要らなくなる*/
	{
		psp_clear_screen(); /* [PAUSE] 復帰時にSDL画面を消す。 */
	}
	#endif
	#if (1==USE_SDL_DRAW)
	s_draw_the_script();	/* スクリプト描画 */
	#endif /* (1==USE_SDL_DRAW) */
//
	/*
	動作と描画が分離できないとGu化できない。
	現状バグがあって分離できないので困っている処。
	*/
	/*欠陥があるので描画後にしか出来ない*/work_the_script();	/* スクリプト動作 */
//


}
/*
	ちゃんと動作と描画が分離できてれば、SDL描画が重いなら、Gu化しなくても
	シナリオ描画部分だけフレームスキップ(30fpsとか)だって出来るんだけど。
*/

/*---------------------------------------------------------
	スクリプト終了処理
---------------------------------------------------------*/

static void do_script_terminate(void)
{
	draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
	load_script_free();
	aaa_script_reset();
//	is_bg = 0;/* (描画と分離できない)バグ修正 */
}


global void script_move_main(void)
{
	static_do_script_work_main();	/* static_スクリプト動作 / 描画 */
//
	/*ボス戦闘後イベント*/	/*ボス戦闘前イベント*/
	{
	//	if (1==script_terminate_flag)	/* pspは0レジスタがあるので0と比較したほうが速い */
		if (/*(0)*/SCRIPT_00_CONTINUE != script_terminate_flag)
		{
			do_script_terminate();
			{
				pd.state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));/*off*/
				pd.state_flag |= STATE_FLAG_12_END_SCRIPT;	/*pd_bo ssmode=B08_START;*/ /*	pd_bo ssmode=B09_STAGE_LOAD;*/
			}
		}
		/*else	{	draw_the_script();}*/
	}	// [***090313	追加
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static int aaa_script_start(void)	/* シナリオファイル名と背景ファイル名 */	/*, char *bg_name, int width*/
{
	if (0 == s_load_scenario())
	{
		return (0);
	}
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
//
	script_skip_mode = 0;/*off*/
	script_terminate_flag = SCRIPT_00_CONTINUE;/*(0)*/	/* 初期化 */
//	home_cursor();				/* カーソルをホームポジションへ移動 */
	cursor_x_chached	= 0;	/* カーソル初期化 */
	cursor_y_chached	= 0;	/* カーソル初期化 */

	//inits 			= 1;
	is_bg				= 0;
	draw_script_screen	= 1/*0*/;

	#if 1
	/* std_obj[]初期化 */
//	std_obj[SPRITE_tachie_r]			= NULL;
//	std_obj[SPRITE_tachie_l]			= NULL;
	int i;
	for (i=0; i<SCRIPT_TACHIE_OBJ_MAX/*20*/; i++)
	{
		std_obj[i]		= NULL;
	}
	#endif
	search_script_list		= memory_script_list;
	return (1);
}
/*---------------------------------------------------------
	ゲームイベント中のスクリプト読み込み(ストーリ、エンディング以外)
---------------------------------------------------------*/
extern int select_player;
global void script_ivent_load(void/*int mode*/)
{
	/* ファイル名作成 */
	{
		/* 'data' '/text/' の分のオフセット */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_TEXT_LENGTH)
		strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_TEXT_STR "Z/sZ1" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_fopen_file_name[DIRECTRY_NAME_OFFSET+0] = ('0'+select_player);
		my_fopen_file_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(pd.player_now_stage);
	//
		if (/*STATE_FLAG_05_IS_BOSS == */(pd.state_flag & STATE_FLAG_05_IS_BOSS))
		{
			;// my_fopen_file_name[DIRECTRY_NAME_OFFSET+4] = '1';
		}
		else
		{
			my_fopen_file_name[DIRECTRY_NAME_OFFSET+4] = '0';
		}
	}
//
	load_script_free();
	if (0 == aaa_script_start())		// ファイルがない場合はイベントを飛ばす, /*NULL,*/ (GAME_WIDTH) /*380*/
	{
		pd.state_flag |= STATE_FLAG_12_END_SCRIPT;		/*pd_bo ssmode=B09_STAGE_LOAD;*/
	}
	else
	{
		pd.state_flag |= STATE_FLAG_06_IS_SCRIPT;	/*on*/		/*pd_bo ssmode=B00_NONE;*/ /*B06_AFTER_EVENT*/
		#if 0
		/* シナリオ中にボムが発生してしまう。バグがあるので。 */
		pd_bomber_time = 0;
		#endif
	}

//	load_script_free();
//	if (0 == aaa_script_start(file_name)), /*NULL,*/ 380
//			{	pd.state_flag |= STATE_FLAG_12_END_SCRIPT;		/*pd_bo ssmode=B08_START;*/ }
//	else	{	pd.state_flag |= STATE_FLAG_06_IS_SCRIPT;		/*pd_bo ssmode=B00_NONE;*/ /*B03_BEFORE_EVENT*/ }
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void story_script_local_quit(void)
{
	#if 1/*???*/
	/*	何か良くわからないけど、ここはおかしい???。 */
	if (NULL != bg_story_window_surface)
	{
		unloadbmp_by_surface(bg_story_window_surface);	//キャッシュに入ってるのでNULLに出来ない。bg_story_window_surface = NULL;
	}
	#endif
//	bg_alpha = 0;
	//inits=1;
	draw_script_screen = 0; /* せりふウィンドウ表示フラグ off */
	//
	menu_cancel_and_voice();	/* タイトルメニューへ移動 */
}

static void story_script_local_work(void)
{
	static_do_script_work_main();	/* static_スクリプト動作 / 描画 */
	if (/*(0)*/SCRIPT_00_CONTINUE != script_terminate_flag)
	{
		do_script_terminate();
		main_call_func = story_script_local_quit;
	}
}

global void story_script_start(void)
{
//	bg_alpha = 0;
	load_script_free();
	/* ファイル名作成 */
	{
		strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_TEXT_STR "story" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
	}
	aaa_script_start(); //	/*,NULL*/ /*"story/arasuzi.jpg"*/ /*"story_bg.jpg"*/, 480
	main_call_func = story_script_local_work;
}

/*---------------------------------------------------------
	pspでは開放が正常動作出来ないので、起動時に確保して(終了時まで)開放しない
---------------------------------------------------------*/

global void story_script_system_init(void)/* 組み込み */
{
	#if 1
	memory_script_list	= NULL; 	/* 命令保持用 */
	search_script_list	= NULL; 	/* 命令実行用 */
//
	draw_script_screen	= 0;	/* せりふウィンドウ表示フラグ */
	is_bg				= 0;	/* 背景表示フラグ */
//
	home_cursor();				/* カーソルをホームポジションへ移動 */
	cursor_x_chached	= 0;	/* カーソル位置 保存用 */
	cursor_y_chached	= 0;	/* カーソル位置 保存用 */
//
	#endif
	bg_alpha			= 255;	/* 255==初期値 */

	/* SD L_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
//
}


/*---------------------------------------------------------
	終了時に開放する部分...何だけど、
	ハングアップしたりするので、取り敢えず無効になってる。
	(開放しなくても終了すれば、OSがメモリ回収するので問題ないって言えば問題ない)
---------------------------------------------------------*/

global void script_system_exit(void)/* 外す */
{
	/*msg_window_init()*/
//
	#if 0/* 本当は要る */
	#endif
}

