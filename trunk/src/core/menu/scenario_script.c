
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 〜 Toho Imitation Style.
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
#include "script_sprite.h"

/*static*/global SCRIPT_SPRITE standard_script_sprite[SCRIPT_SPRITE_99_MAX];	/* スクリプト汎用スプライト */	/*32*/ /*20*/

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

static void remove_script_sprite_num(int nnn)/*SCRIPT_SPRITE *src*/
{
//	if (NULL != src)
	if (NULL != standard_script_sprite[nnn].img)
	{
		SDL_FreeSurface(standard_script_sprite[nnn].img);
		standard_script_sprite[nnn].img = NULL; //	free(src);	//	src = NULL;
	}
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
	char buffer[32];/*20*/
	char *ddd = buffer;
	int i = 0;
	while (
		(isdigit((int)((char)(*ccc))))	/* gcc 4.3.5 */
		||
		(((char)'-')==(*ccc))
	)		/* 負数にも対応 / isdigit : 数字かどうかの判定 */
	{
		i++;
		if (i >= 32)/*20*/
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
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = (-1); }
	else														{	*number = atoi(buffer); }
	return (ccc);
/*error*/
ne222:
	return ((char *)NULL);
}
/*
gcc 4.3.5	warning: array subscript has type 'char'
isdigit( int );
*/

/*---------------------------------------------------------

---------------------------------------------------------*/

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
	I_CODE_BOSS_TERMINATE,/*(r34新設)*/

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
	struct _scenario_script *next;
	int douji_chain;	// 同時実行かどうか// 0=違うよ 1=1つ目 2=2つ目
	int i_code; 		/* Interprited Code. / Intermediate Language, Command 中間言語 / 終わったかどうか */
	int first_flag; 	/* 0:初回, それ以外初回ではない  各スクリプトコマンドごとの初期化判定 */
//[16==4*4]
	int para1;
	int para2;
	int para3;
	int align_dummy000;
//[32==8*4]
	char para0[(7*32)/*200*/];/* 32の倍数 */
} S_SCRIPT;/* == 256bytes */


/*---------------------------------------------------------
	スクリプト、リスト処理。
---------------------------------------------------------*/
static S_SCRIPT *script_list_scan;			/* 命令列の実行位置を保持する。 */	/*=NULL*/	/* 命令列の読み込み位置を保持する。 */
static S_SCRIPT *script_list_bigin; 		/* 命令列の開始位置を保持する。開放時にも必要。 */	/*=NULL*/


/*---------------------------------------------------------
	前回のシナリオがメモリにあればすべて開放。
---------------------------------------------------------*/
#include <malloc.h>/* free(); */
static void load_script_free(void)
{
	S_SCRIPT *l = script_list_bigin;
	S_SCRIPT *n;
	while (NULL != l)
	{
		n = l->next;
		free(l);	/* 1コマンドずつ開放 */
		l = n;
	}
	script_list_bigin	= (S_SCRIPT *)NULL;
	script_list_scan	= (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------
	登録処理
---------------------------------------------------------*/
enum
{
	PARAM_01 = 0,
	PARAM_02,
	PARAM_03,
//	PARAM_04,/*未使用*/
	PARAM_99_MAX/*最大数*/
};
/*
	基本形
	command para0,para1,para2,para3
*/
static void regist_script(
	char *command,
	char *c_p0,
	const int *c_pn,
	int chain)
{
	S_SCRIPT *new_script;
	new_script				= my_calloc(sizeof(S_SCRIPT));/* 1コマンド分 */

	strcpy(new_script->para0, c_p0);
	new_script->para1		= /*c_p1*/c_pn[PARAM_01];	/* デフォルト */
	new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
	new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */
//	new_script->para4		= /*c_p3*/c_pn[PARAM_04];	/* デフォルト */
//
//	new_script->i_code		= I_CODE_DONE;
	new_script->douji_chain = chain;
	new_script->next		= NULL;
	new_script->first_flag	= (0);

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
			else	if (0==tiny_strcmp(c_p0,	"term"))		{	new_script->i_code = I_CODE_BOSS_TERMINATE; 	}	/* result後にゲーム強制終了。 */
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

	if (NULL==script_list_scan)/* 先頭なら */
	{
		script_list_bigin		= new_script;				/* 開放時に必要な先頭位置を保持する。 */
		script_list_scan		= script_list_bigin;		/* 始めのスキャン位置は開始位置とする。 */
	}
	else/* 先頭以外なら連結処理。 */
	{
		script_list_scan->next	= new_script;				/* 現在位置の次に新規を連結。 */
		script_list_scan		= script_list_scan->next;	/* 新規を現在位置に変更する。 */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/


/*---------------------------------------------------------
	スクリプト読み込み
	-------------------------------------------------------
	この関数は、仕様上「とても処理落ちします」

---------------------------------------------------------*/

static int s_load_scenario(void)
{
//	FILE *fp;
	if (NULL == (/*fp =*/my_file_fopen()))	/* 開けなかったとき */	/* my_file_common_name, "r" */
	{
		return (0);/* 読み込み失敗 */
	}

	int opecode_entrys		= 0;	/* 命令がが書き込まれているかどうか。 */
	int opecode_chains		= 0;	/* 連続した命令 */
	{
		MY_FILE_LOOP_BEGIN;
		/* '-'で始まる行はスクリプトチェイン機能(複数スクリプト行、同時実行機能)。 */
		if ('-'==(*ch)) 	{	ch++;	opecode_chains++;	opecode_chains &= 0x0f; }	/* ワークが16までしかないので最大16命令 */
		else				{	opecode_chains = 0; }
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* parth start. 構文解析開始 */
		int end_arg;	end_arg = 0;		/* 行末 == 引数の取得の中止 */
		char char_command[16];/*15*/		/* 基本コマンド */
		char c_p0[256];/*200*/
		ch = load_command(ch, char_command, &end_arg);		/* 基本コマンド名称(オペコード)読み込み */
		if (NULL==ch)										{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		if (!end_arg)/* 行末 */
		{
			if (' ' != (*ch))								{	GOTO_ERR_WARN;	}	/* 区切り */
			ch++;
			ch = load_my_file_get_str(ch, c_p0, &end_arg/*, ','*/); 						/* 文字列コマンド(オペランド)読み込み */
			if (NULL==ch)									{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		}
		{
			int c_pn[PARAM_99_MAX];/*6*/
			{	int kk;
				for (kk=0; kk<PARAM_99_MAX; kk++)/*(6)*/
				{
					c_pn[kk] = (-1);	/* 無指定の場合 を判別する為 */
					if (!end_arg)
					{
						if (',' != (*ch))						{	GOTO_ERR_WARN;	}	/* 区切り */
						ch++;
						ch = load_int(ch, &c_pn[kk], &end_arg); 						/* 数値コマンド(オペランド)読み込み */
						if (NULL==ch)							{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
					}
				}
			}
			regist_script(char_command, c_p0, c_pn, opecode_chains);
		}
		opecode_entrys++;
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(ERR_WARN, (char*)"syntax error in scriptfile '%s', line no: %d", my_file_common_name, debug_number_of_read_line);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	//
	my_file_fclose(/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		error(ERR_WARN, (char*)"can't entrys from this file %s", my_file_common_name);
		return (0);/* 読み込み失敗 */
	}
	/* 読み込み成功 */
	script_list_scan		= script_list_bigin;/* スクリプト実行位置を、開始位置とする。 */
	return (1);
}

/////////////////////////////

/*---------------------------------------------------------
	スクリプトシステム用
	カーソル位置キャッシュ
---------------------------------------------------------*/
static int is_bg;	/*=0*/					/* 背景表示/非表示フラグ */

//static int cursor_x_chached;			/* カーソル位置 保存用 */
//static int cursor_y_chached;			/* カーソル位置 保存用 */

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
	for (i=0; i<SCRIPT_SPRITE_99_MAX; i++)/*20*/
	{
		remove_script_sprite_num(i);
	}
//	remove_script_sprite_num(SCRIPT_SPRITE_00_TACHIE_RIGHT);
//	remove_script_sprite_num(SCRIPT_SPRITE_01_TACHIE_LEFT_);
	if (NULL != bg_story_window_surface)
	{
		SDL_FreeSurface(bg_story_window_surface);
		bg_story_window_surface 	= NULL;
	}
	is_bg = (0);/* 開放したんだから bg 表示できない、したがって強制表示 off */
	//
	home_cursor();				/* カーソルをホームポジションへ移動 */	/* カーソル位置 */
//	cursor_x_chached = (0); 	/* カーソル位置 保存用 */
//	cursor_y_chached = (0); 	/* カーソル位置 保存用 */

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
		is_bg = (0);/* 開放したんだから bg 表示できない、したがって強制表示 off */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* 背景α値用 */

/////////////////////////////

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


/*---------------------------------------------------------
	スクリプトシステム、内部処理
---------------------------------------------------------*/
static int re_draw_count_flag;
global void script_system_set_re_draw(void)
{
	re_draw_count_flag = (1);/* 描画してね */
}
/*---------------------------------------------------------
	[スクリプト内部コマンド]
---------------------------------------------------------*/
extern void load_SDL_bg_file_name(char *file_name);
static int script_system_load_bg(S_SCRIPT *ssc)
{
	/* スクリプト内部コマンドのパラメーターを受け取る。 */
	char *filename; 		filename		= ssc->para0;
	int alpha;				alpha			= ssc->para1;
	int fade_command;		fade_command	= ssc->para2;
	int set_alpha_speed;	set_alpha_speed = ssc->para3;
//
	re_draw_count_flag = (1);/* 描画してね */
//
	static int bg_alpha_speed;		/* 背景α値用 */
	if ((-1)==fade_command)
	{
		load_bg_aaa(filename, alpha);
	//	ssc->owattayo = 1;
		return (1); /* 処理完了 */
//		break;
	}
	else
	if ((1)==fade_command)/*1==fade_in_command*/
	{
		if (0 == ssc->first_flag)	/* 初回のみ行う */
		{
			ssc->first_flag = 1;/*初回ではない*/
		///*fade_in_command_start*/
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
	}
	return (0); 	/* 処理中 */
}
//	case 2:/*2==fade_out_command_start*/
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


/*---------------------------------------------------------
	[スクリプト内部コマンド]
	スプライトに画像を読み込む
	script_system_load_sprite
---------------------------------------------------------*/

static /*SCRIPT_SPRITE **/void script_system_load_sprite(S_SCRIPT *ssc)
{
	/* スクリプト内部コマンドのパラメーターを受け取る。 */
	char *filename; 	filename	= ssc->para0;
	int sc_sp_num;		sc_sp_num	= ssc->para1;
	int xx; 			xx			= ssc->para2;
	int yy; 			yy			= ssc->para3;
//
	re_draw_count_flag = (1);/* 描画してね */
//
	sc_sp_num &= ((SCRIPT_SPRITE_99_MAX-1));
	if (NULL != standard_script_sprite[sc_sp_num].img)	/* 使用中なら */
	{
		//if (NULL != std_obj[sc_sp_num])
		remove_script_sprite_num(sc_sp_num);	/* 強制的に開放 */
	}
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename, 0/*alpha*/);
	suf 				= load_local01(filename, suf);//, 0/*alpha*/);
	if (NULL==suf)		{	return; 	}/*(NULL)*/

	standard_script_sprite[sc_sp_num].img			= suf;	/* 使用中になる。 */
	standard_script_sprite[sc_sp_num].w 			= (suf->w/*/frames*/);	//tmp->cw			= ((tmp->w)>>1);
	standard_script_sprite[sc_sp_num].h 			= (suf->h); 			//tmp->ch			= ((tmp->h)>>1);

	standard_script_sprite[sc_sp_num].cx256 		= (xx<<8);	/*rect->x*/
	standard_script_sprite[sc_sp_num].cy256 		= (yy<<8);	/*rect->y*/
}
//	SCRIPT_SPRITE *tmp;
//	tmp 				= my_calloc(sizeof(SCRIPT_SPRITE));
//	std_obj[sc_sp_num]	= tmp;

//	if (frames==-1) 	{	tmp->frames = 1;		}
//	else				{	tmp->frames = frames;	}
//	tmp->ak tframe		= 0;
//	tmp->an im_speed	= 0;
//	tmp->an im_type 	= 0;
//	tmp->an im_time 	= 0;
//	tmp->an im_count	= 0;
	/*tmp->alpha255 	= 255;*/	/*tmp->color32		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);*/
//	tmp->flags			= 0;
//	tmp->move_wait		= 0;
//	tmp->x				= rect->x-(rect->w*tmp->w);
//	tmp->y				= rect->y-(tmp->h);
//	tmp->x				= xx-(0*tmp->w);
//	tmp->y				= yy-(tmp->h);

//	return (tmp);


/*---------------------------------------------------------
	[スクリプト内部コマンド]
	読み込み済みのスプライトの画像を再読み込みして、
	位置をそのまま継続し画像は入れ替える。
	script_system_swap_image_sprite
---------------------------------------------------------*/

static void script_system_swap_image_sprite(S_SCRIPT *ssc)
{
	/* スクリプト内部コマンドのパラメーターを受け取る。 */
	char *filename; 	filename		= ssc->para0;
	int num;			num 			= ssc->para1;
//	int dummy_frames;	dummy_frames	= ssc->para2;
//	int alpha;			alpha			= ssc->para3;
//
	re_draw_count_flag = (1);/* 描画してね */
//
	int sitei_number;
	if (-1==num)		{	return; 		}
	{	sitei_number=((num)&(SCRIPT_SPRITE_99_MAX-1));	}	/* 汎用スプライト */
//
	/* 使用中の場合のみ処理、使用中でないなら何もしないで戻る。 */	/* 使ってない場合は戻る。 */
//	if (NULL != tmp)
	if (NULL != standard_script_sprite[sitei_number].img)
	{
		/* ここにくるのは使ってる場合.used==1の場合のみ */
		SDL_FreeSurface(standard_script_sprite[sitei_number].img);/* 画像を開放する。 */
		SDL_Surface *suf = NULL;
		suf 		= load_local01(filename, suf);//, 0/*alpha*/);
		standard_script_sprite[sitei_number].img	= suf;
	//	standard_script_sprite[sitei_number].frames = frames;
		standard_script_sprite[sitei_number].w		= (suf->w/*/frames*/);	//tmp->cw		= ((tmp->w)>>1);
		standard_script_sprite[sitei_number].h		= (suf->h); 			//tmp->ch		= ((tmp->h)>>1);
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static int inits; 							/* 各スクリプトコマンドごとの初期化判定 */

/*---------------------------------------------------------
	[スクリプト内部コマンド]
	I_CODE_OBJ_MOVE スプライトの移動
---------------------------------------------------------*/
//		int n = parth_str_right_or_str_left_or_number(l_or_r);
//		if (n==STR_ERROR/*-1*/) 		{	return (2/*-1*/);	}/*スクリプト終了*/


//static void set_sprite_speed( int num, int speed_aaa)
//{
//	SCRIPT_SPRITE *tmp;
//	tmp=std_obj[(num)&(SCRIPT_SPRITE_99_MAX-1)];		/* 汎用スプライト */
//	//std_obj[((num)&(SCRIPT_SPRITE_99_MAX-1))].set_speed = speed_aaa;
//	tmp->set_speed = speed_aaa;
//}
static int script_system_move_sprite(S_SCRIPT *ssc)/*, int speed_aaa, int alpha255*/	/*,ssc->para4*/
{
	/* スクリプト内部コマンドのパラメーターを受け取る。 */
	char *color32_str;		color32_str = ssc->para0;
	int num;				num = ssc->para1;
	int x;					x = ssc->para2;
	int y;					y = ssc->para3;
//
	re_draw_count_flag = (1);/* 描画してね */
//
	SCRIPT_SPRITE *my_std_obj;
	/* (r33)？？？？？？？？？？？たぶん(確認してないので安全でない) */
//	my_std_obj = std_obj[((num)&(SCRIPT_SPRITE_99_MAX-1))]; /* 汎用スプライト */
	my_std_obj = &standard_script_sprite[((num)&(SCRIPT_SPRITE_99_MAX-1))]; /* 汎用スプライト */
//
	if (0 == ssc->first_flag)	/* 初回のみ行う */
	{
		ssc->first_flag = 1;/*初回ではない*/
		/* あらかじめ移動完了位置を算出しておく */
		my_std_obj->target_x256 = my_std_obj->cx256 + (x<<8);		/* 移動完了座標 */
		my_std_obj->target_y256 = my_std_obj->cy256 + (y<<8);		/* 移動完了座標 */
//		my_std_obj->cx256		= ((my_std_obj->cx256)/*<<8*/); 	/* 精度確保用 */
//		my_std_obj->cy256		= ((my_std_obj->cy256)/*<<8*/); 	/* 精度確保用 */
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
		if (t256(6.0)<speed256) 	{	speed256 = t256(8.0);	}	/* 最大速度8[dots/flame] t256(6.0) */

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
	switch ((((my_std_obj->angle1024-128/*deg_360_to_512(45)*/) & 0x300 )) ) /*my_std_obj->r_course*/
	{	/* 移動完了座標に等しいかはみ出したら、完了とする。 */
//	case (0<<7)/* 2:↓(512_1) */:	if (my_std_obj->y <= my_std_obj->target_y)	{	goto move_complete; 	}	break;
//	case (1<<7)/* 1:→(512_0) */:	if (my_std_obj->x >= my_std_obj->target_x)	{	goto move_complete; 	}	break;
//	case (2<<7)/* 0:↑(512_3) */:	if (my_std_obj->y >= my_std_obj->target_y)	{	goto move_complete; 	}	break;
//	case (3<<7)/* 3:←(512_2) */:	if (my_std_obj->x <= my_std_obj->target_x)	{	goto move_complete; 	}	break;

	case (0<<8):/* 1:→(1024_0) */	if (my_std_obj->cx256 < my_std_obj->target_x256)	{	return (0);/*移動中*/	}	goto move_complete;
	case (1<<8):/* 2:↓(1024_1) */	if (my_std_obj->cy256 > my_std_obj->target_y256)	{	return (0);/*移動中*/	}	goto move_complete;
	case (2<<8):/* 3:←(1024_2) */	if (my_std_obj->cx256 > my_std_obj->target_x256)	{	return (0);/*移動中*/	}	goto move_complete;
	case (3<<8):/* 0:↑(1024_3) */	if (my_std_obj->cy256 < my_std_obj->target_y256)	{	return (0);/*移動中*/	}	goto move_complete;
	}
	#endif
move_complete:
	/* 移動完了した場合は、正確な座標に修正する。 */
	my_std_obj->cx256 = my_std_obj->target_x256;
	my_std_obj->cy256 = my_std_obj->target_y256;
	return (1);/*移動完了*/
}


/*---------------------------------------------------------
	スクリプトシステム、内部実行処理
	-------------------------------------------------------
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
static /*int*/void script_system_execute(void)
{
	/* ==== ボタン入力処理 ==== */
	int shot_ositenai;
	shot_ositenai = 1;	/* ショット押してない */
	if (0 == psp_pad.pad_data)/* 今は押してない */
	{
		if (psp_pad.pad_data_alter & PSP_KEY_BOMB_CANCEL)	/* キャンセル入力 */
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
		if (psp_pad.pad_data_alter & PSP_KEY_SHOT_OK)		/* ＯＫ入力 */
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
	S_SCRIPT *ssc;
	ssc = script_list_scan;/* 実行開始位置 */
//
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
				if (0 == ssc->first_flag)	/* 初回のみ行う */
				{
					ssc->first_flag = 1;/*初回ではない*/
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
					aaa = script_system_load_bg(ssc);
					if (0!=aaa)
					{
						ssc->i_code = I_CODE_DONE;
					}
				}
				break;
			case I_CODE_OBJ_LOAD:	/* 汎用絵  立ち絵L	立ち絵R */
				script_system_load_sprite(ssc);
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_OBJ_MOVE:
				{	int aaa;
					aaa = script_system_move_sprite(ssc);
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
				script_system_swap_image_sprite(ssc);
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_SCREEN_BG:			is_bg						= ssc->para1;	check_is_bg();	ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_SCREEN_TEXT:		cg.draw_flag_script_screen	= ssc->para1;					ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_SCREEN_PANEL:		cg.side_panel_draw_flag 	= ssc->para1;					ssc->i_code = I_CODE_DONE;
				#if 1
				/* 本当は要らないが、現状では使いにくいのでパネル切り替え時に細工する。 */
				/* スキップモード、強制的に無効(スキップしない)。 */
				script_skip_mode = 0;/*off*/
				#endif
																												break;
			case I_CODE_BGM:				play_music_num( (ssc->para1) ); 		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BGM_VOLUME: 		set_music_volume( (ssc->para1) );		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_LOAD:			script_boss_load( (ssc->para1)/*boss_number*/ );		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_START: 		script_boss_start( /*(ssc->para1)*/ );					ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_TERMINATE: 	cg.state_flag |= STATE_FLAG_16_GAME_TERMINATE;			ssc->i_code = I_CODE_DONE;	break;
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
	/* 次回用の頭だしと終了判断 */
	{
	aaa_loop:
		if (NULL == script_list_scan)/* スクリプトが最後なら */
		/* (
			calloc() して確保したので、暗黙に NULL が入っているという、ちょっとトリッキーなコード。(calloc() は 0クリアする)
			つまり malloc だと駄目かも。(malloc() は 0クリアしない)
			実使用上問題ないが、あまり好ましくないコード。(C言語の規格上 0==NULL は、規格外なので本来良くない。)
			)
		*/
		{
			goto terminate_all_script;
		}
		if (I_CODE_DONE == script_list_scan->i_code)
		{
			script_list_scan = script_list_scan->next;/* 次回はここから調べる */
			goto aaa_loop;
		}
	}
	script_terminate_flag = SCRIPT_00_CONTINUE;/*(0)*/
	return;/* スクリプトは処理つづく */
//
terminate_all_script:/* スクリプトが最後なら */
	script_terminate_flag = SCRIPT_01_TERMINATE;/*(1)*/
}


#define USE_SDL_DRAW (1)
#if (1==USE_SDL_DRAW)
/*---------------------------------------------------------
	スプライトオブジェクトの描画
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
	#if (0==USE_GU_TACHIE_TEST)/* (r34)テスト */
	{
		int nnn;
		for (nnn=0; nnn<SCRIPT_SPRITE_99_MAX; nnn++)
		{
			if (NULL != standard_script_sprite[nnn].img)	/* 画像があるもののみ描画する。(使用中の場合に描画する) */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0);
				src_r.y = (0);
				src_r.w = standard_script_sprite[nnn].w;
				src_r.h = standard_script_sprite[nnn].h;
				dst_r.x = (t256_floor(standard_script_sprite[nnn].cx256));
				dst_r.y = (t256_floor(standard_script_sprite[nnn].cy256));
				SDL_BlitSurface(standard_script_sprite[nnn].img, &src_r, sdl_screen[SDL_00_VIEW_SCREEN], &dst_r);
			}
		}
	}
	#endif
}
#endif /* (1==USE_SDL_DRAW) */

/*---------------------------------------------------------
	スクリプトシステムSDL描画
---------------------------------------------------------*/

global void script_system_SDL_draw(void)
{
	/* SDL描画、遅すぎる。(Gu化するとここは無くなる予定) */
	if (re_draw_count_flag)
	{
		re_draw_count_flag--;
		#if 0/* 1:フレームスキップ描画のテスト */
		/*
		SDL描画が重いから、Gu化しなくてもシナリオ描画部分だけ、フレームスキップ(30fpsとか15fpsとか)してみた。
		が、たいして変わらなく遅い。
		*/
		static int static_game_v_time = 0;
	//	if (0==(static_game_v_time&(2-1)))/* 実験的に30[flame]描画にしてみる。 */
		if (0==(static_game_v_time&(4-1)))/* 実験的に15[flame]描画にしてみる。 */
		#endif/* 1:フレームスキップ描画のテスト */
		{
			#if 1/*Gu化完了したら要らなくなる*/
			{
				psp_clear_screen(); /* [PAUSE] 復帰時にSDL画面を消す。 */
			}
			#endif
			#if (1==USE_SDL_DRAW)
			s_draw_the_script();	/* スクリプト描画 */
			#endif /* (1==USE_SDL_DRAW) */
		}
	}
}


/*---------------------------------------------------------
	スクリプト終了処理
---------------------------------------------------------*/

static void do_script_terminate(void)
{
//	bg_alpha = 0;
	//inits=1;
	cg.draw_flag_script_screen = (0); /* せりふウィンドウ表示フラグ off */
	load_script_free(); 	/* 前回のシナリオがメモリにあればすべて開放。 */
	aaa_script_reset();
//	is_bg = 0;/* (描画と分離できない)バグ修正 */
}


/*---------------------------------------------------------
	ゲームコア用、スクリプト終了状態へ遷移
---------------------------------------------------------*/
static void script_terminate_game_core(void)
{
	cg.state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));/*off*/
	cg.state_flag |= STATE_FLAG_12_END_SCRIPT;
	/* ボス戦闘前後イベント pd_bo ssmode = B08_START; = B09_STAGE_LOAD; */
}


/*---------------------------------------------------------
	スクリプト動作中
---------------------------------------------------------*/
static void (*script_terminate_call_func)(void);
global void script_move_main(void)
{
	/*---------------------------------------------------------
		(r33)やっと、動作と描画が分離できた−。
		だけど、Gu化、間にあわなかった。
	---------------------------------------------------------*/
	script_system_execute();		/* static_スクリプト動作 */ 	/* 動作のみ(描画しない) */
	if (/*(0)*/SCRIPT_00_CONTINUE != script_terminate_flag)
	{
		do_script_terminate();
		script_terminate_call_func();
	}
}


/*---------------------------------------------------------
	スクリプト開始処理
---------------------------------------------------------*/

static int aaa_script_start(void)	/* シナリオファイル名と背景ファイル名 */
{
	if (0 == s_load_scenario())
	{
		return (0);/* 読み込み失敗 */
	}
	kanji_window_clear();	/* 漢字ウィンドウの内容を消す。 */
	home_cursor();			/* カーソルをホームポジションへ移動 */
//
	script_skip_mode = 0;/*off*/
	script_terminate_flag = SCRIPT_00_CONTINUE;/*(0)*/	/* 初期化 */
//	home_cursor();				/* カーソルをホームポジションへ移動 */
//	cursor_x_chached	= 0;	/* カーソル初期化 */
//	cursor_y_chached	= 0;	/* カーソル初期化 */

	//inits 			= 1;
	is_bg				= 0;
	cg.draw_flag_script_screen	= (1);/*0*/

	#if 1
	/* std_obj[]初期化 */
	{	int i;
		for (i=0; i<SCRIPT_SPRITE_99_MAX; i++)/*20*/
		{
			standard_script_sprite[i].img	= NULL;
		}
	}
	#endif
	return (1);
}


/*---------------------------------------------------------
	ゲームイベント中のスクリプト開始(ストーリ、エンディング以外)
---------------------------------------------------------*/

global void script_ivent_load(void)
{
	/* ファイル名作成 */
	{
		/* 'data' '/text/' の分のオフセット */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_TEXT_LENGTH)
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_TEXT_STR "Z/sZ1" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_file_common_name[DIRECTRY_NAME_OFFSET+0] = ('0'+(cg_game_select_player));
		my_file_common_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(cg.game_now_stage);
		//
		if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(ボス戦闘後の場合)*/
		{
			;// my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '1';
			//#if (1==US E_EASY_BADEND)
			/* 5面終わりでeasyの場合、特殊処理(BAD END) */
			{
			//	if ((5) == (cg.game_now_stage))/*(5面の場合)*/
				if (
					((5) == (cg.game_now_stage))/*(5面の場合)*/
					||
					((6) == (cg.game_now_stage))/*(6面の場合、隠しエンド)*/
				)
				{
					if ((0)==(cg_game_difficulty))/*(easyの場合)*/
					{
					//	my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '2';
						my_file_common_name[DIRECTRY_NAME_OFFSET+4]++;
					}
				}
			}
			//#endif
		}
		else/*(ボス戦闘前の場合)*/
		{
		//	my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '0';
			my_file_common_name[DIRECTRY_NAME_OFFSET+4]--;
		}
	}
	script_terminate_call_func = script_terminate_game_core;	/* スクリプト終わったらゲームコア用状態遷移 */
	load_script_free(); 			/* 前回のシナリオがメモリにあればすべて開放。 */
	if (0 == aaa_script_start())	// ファイルがない場合はイベントを飛ばす
	{
		cg.state_flag |= STATE_FLAG_12_END_SCRIPT;
	}
	else
	{
		cg.state_flag |= STATE_FLAG_06_IS_SCRIPT;	/*on*/
		#if 0
		/* シナリオ中にボムが発生してしまう。バグがあるので。 */
		pd_bomber_time = 0;
		#endif
	}
}


#if 1/*Gu描画になると要らなくなる*/
/*---------------------------------------------------------
	ストーリー動作
---------------------------------------------------------*/

static void story_script_local_work(void)
{
	script_system_SDL_draw();	/* スクリプト SDL 描画(遅い) */
	script_move_main(); 		/* スクリプト動作(移動) */
}
#endif

/*---------------------------------------------------------
	ストーリー開始
---------------------------------------------------------*/

global void story_script_start(void)
{
//	bg_alpha = 0;
	/* ファイル名作成 */
	{
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_TEXT_STR "story" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
	}
	script_terminate_call_func = menu_cancel_and_voice; 	/* スクリプト終わったらタイトルメニューへ移動 */
	load_script_free(); 	/* 前回のシナリオがメモリにあればすべて開放。 */
	aaa_script_start(); 	/* ストーリー用のスクリプト開始 */
	#if 1/*Gu描画になると要らなくなる*/
	main_call_func = story_script_local_work;/* SDL描画の場合 */
	#else
//	main_call_func = script_move_main;/* Gu描画の場合 */
	#endif
}


/*---------------------------------------------------------
	pspでは開放が正常動作出来ないので、起動時に確保して(終了時まで)開放しない
---------------------------------------------------------*/

global void story_script_system_init(void)/* 組み込み */
{
	#if 1
	script_list_bigin	= NULL; 	/* 命令列の開始位置を保持 */
	script_list_scan	= NULL; 	/* 命令走査位置で使用 */
//
	cg.draw_flag_script_screen	= (0);		/* せりふウィンドウ表示フラグ */
	is_bg				= (0);		/* 背景表示フラグ */
//
	home_cursor();					/* カーソルをホームポジションへ移動 */
//	cursor_x_chached	= (0);		/* カーソル位置 保存用 */
//	cursor_y_chached	= (0);		/* カーソル位置 保存用 */
//
	#endif
	bg_alpha			= (255);		/* 255==0xff 初期値 */

	/* SD L_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
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
