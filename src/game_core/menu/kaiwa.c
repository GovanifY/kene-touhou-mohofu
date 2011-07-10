
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	シナリオ会話 処理
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

#include "kaiwa_system.h"
#include "kanji_system.h"
#include "111_my_file.h"
#include "kaiwa_sprite.h"

/*static*/global KAIWA_SPRITE kaiwa_sprite[KAIWA_SPRITE_99_MAX];	/* シナリオ会話 汎用オブジェ */ /*32*/ /*20*/

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

/*---------------------------------------------------------
	指定ファイル名の画像を読みこみSDLサーフェイスに設定する。
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
		error(ERR_FATAL, (char*)"kaiwa: cant load image %s:", file_name );
//		return (NULL);
	}
#if 0
	if (	(-1==alpha) /* シナリオ会話 中で省略した場合(デフォルト) */
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
		error(ERR_FATAL, (char*)"cant convert image %s to display format:", file_name );
//		return (NULL);
	}
	#endif
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------
	指定番号のオブジェクト画像のSDLサーフェイスを解放する。
---------------------------------------------------------*/

static void remove_kaiwa_sprite_num(int nnn)/*KAIWA_SPRITE *src*/
{
//	if (NULL != src)
	if (NULL != kaiwa_sprite[nnn].img)	/* 使用中なら */
	{
		//if (NULL != std_obj[nnn])
		SDL_FreeSurface(kaiwa_sprite[nnn].img);/* 画像を開放する。 */
		kaiwa_sprite[nnn].img = NULL; //	free(src);	//	src = NULL;
	}
}

/*---------------------------------------------------------
	インタプリタ命令(オペコード)を示す文字列を読む。
	10文字以上はエラー。
---------------------------------------------------------*/

static char *read_opecode(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	while ((*c != ' ') && (*c != 13))
	{
		i++;
		if (i >= 10)
		{
			return ((char *)NULL);
		}
		*buffer++ = *c++;
	}
	if (*c == 13)	/* 改行コードは OD OA */
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}


/*[is_digit_or_hifun();'数字'もしくは'-'記号の場合。であるかを調べる。]*/

/*---------------------------------------------------------
	複数に渡る命令パラメーター(オペランド)を一つ分だけ読む。
---------------------------------------------------------*/
extern int is_digit_or_hifun(char *ccc);/*(load_stage.c)*/
static char *read_operand_int_only(char *ccc, int *number, int *line_terminate_end_flag)
{
	char buffer[32];/*20*/
	char *ddd = buffer;
	int i = 0;
	while ( is_digit_or_hifun(ccc) )/*(負数に対応した)*/
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

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
// OBJ関連
	I_CODE_00_OBJ_LOOK = 0,
	I_CODE_01_OBJ_XY,
	I_CODE_02_OBJ_LOAD,
	I_CODE_03_OBJ_WAIT,
// 画面制御
	I_CODE_04_DRAW_TEXT,
	I_CODE_05_DRAW_BG,
	I_CODE_06_DRAW_PANEL,
	I_CODE_07_DRAW_SKIP,
// テキスト関連
	I_CODE_08_TEXT,
	I_CODE_09_HOLD,
	I_CODE_0a_FONT_COLOR,
	I_CODE_0b_FONT_SPEED,
// 背景/BGM
	I_CODE_0c_BG_LOAD,
	I_CODE_0d_BG_FADE,
	I_CODE_0e_BGM,
	I_CODE_0f_BGM_VOLUME,
// ボス制御
	I_CODE_10_BOSS_LOAD,
	I_CODE_11_BOSS_TERMINATE,/*(r34新設)*/
	I_CODE_12_BOSS_START,
	I_CODE_13_BOSS_RESULT,/*(r35新設)*/
//
	I_CODE_14_DUMMY,	// 必要ないが安全の為
	I_CODE_15_DUMMY,	// 必要ないが安全の為
	I_CODE_16_DUMMY,	// 必要ないが安全の為
	I_CODE_17_DUMMY,	// 必要ないが安全の為
//
	I_CODE_18_DUMMY,	// 必要ないが安全の為
	I_CODE_19_DUMMY,	// 必要ないが安全の為
	I_CODE_1a_DUMMY,	// 必要ないが安全の為
	I_CODE_1b_DUMMY,	// 必要ないが安全の為
//
	I_CODE_1c_DUMMY,	// 必要ないが安全の為
	I_CODE_1d_DUMMY,	// 必要ないが安全の為
	I_CODE_1e_DUMMY,	// 必要ないが安全の為
	I_CODE_1f_DUMMY,	// 必要ないが安全の為
//
	I_CODE_99_MAX/*(2^n)*/	// 必要ないが安全の為
};






typedef struct _kaiwa_youso_tag_
{
	struct _kaiwa_youso_tag_ *next;
	int i_code; 		/* Interprited Code. / Intermediate Language, Command 中間言語 / 終わったかどうか */
	int para1;
//[16==4*4]
	int para2;
	int para3;
//[32==8*4]
	int str_num0;// char para0[(7*32)/*200*/];/* 32の倍数 */
} KAIWA_YOUSO;/* == 256bytes */
/*
	64しかないけど、巧く動いてる。
	霊夢A end(s61.txt) で 51 しか消費しない。
	多分、模倣風の標準シナリオならば、 64 で十分かと思われる。
	が、安全の為、倍の 128 にしとく。
	万一オーバーしても、エラー"string stack over flow.(%d)\n"で
	終了するから問題無い。(メッセージから原因も判るし)
*/
//#define MAX_STR_STACK 	(64)
#define MAX_STR_STACK		(128)
static int str_stack_position;
static char str_stack[MAX_STR_STACK][(7*32)/*200*/];/* 32の倍数 */

/*---------------------------------------------------------
	シナリオ会話、リスト処理。
---------------------------------------------------------*/
/*
	ここも malloc すると遅いから、固定 256 ぐらいにするべき。
	多分、模倣風の標準シナリオならば、 100 も消費しないと思われる。
*/
static KAIWA_YOUSO *kaiwa_youso_list_scan;			/* 命令列の実行位置を保持する。 */	/*=NULL*/	/* 命令列の読み込み位置を保持する。 */
static KAIWA_YOUSO *kaiwa_youso_list_bigin; 		/* 命令列の開始位置を保持する。開放時にも必要。 */	/*=NULL*/


/*---------------------------------------------------------
	前回のシナリオがメモリにあればすべて開放。
---------------------------------------------------------*/
#include <malloc.h>/* free(); */
static void load_kaiwa_youso_free(void)
{
	KAIWA_YOUSO *l = kaiwa_youso_list_bigin;
	KAIWA_YOUSO *n;
	while (NULL != l)
	{
		n = l->next;
		free(l);	/* 1コマンドずつ開放 */
		l = n;
	}
	kaiwa_youso_list_bigin	= (KAIWA_YOUSO *)NULL;
	kaiwa_youso_list_scan	= (KAIWA_YOUSO *)NULL;
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
static void regist_kaiwa_youso(
	char *command,
	char *c_p0,
	const int *c_pn)
{
	KAIWA_YOUSO *new_kaiwa_youso;
	new_kaiwa_youso 			= my_calloc(sizeof(KAIWA_YOUSO));/* 1コマンド分 */

//てすと	strcpy(new_kaiwa_youso->para0, c_p0);
	new_kaiwa_youso->para1		= /*c_p1*/c_pn[PARAM_01];	/* デフォルト */
	new_kaiwa_youso->para2		= /*c_p2*/c_pn[PARAM_02];	/* デフォルト */
	new_kaiwa_youso->para3		= /*c_p3*/c_pn[PARAM_03];	/* デフォルト */
//	new_kaiwa_youso->para4		= /*c_p3*/c_pn[PARAM_04];	/* デフォルト */
//
//	new_kaiwa_youso->i_code 	= I_CODE_DONE;
	new_kaiwa_youso->next		= NULL;

	if ('B'==(*command))
	{
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_kaiwa_youso->i_code = I_CODE_0c_BG_LOAD; strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;	/*push_str(c_p0);*/ 	}	/* 背景ファイルの変更 */
			else	if (0==tiny_strcmp(command, "BG_FADE")) 	{	new_kaiwa_youso->i_code = I_CODE_0d_BG_FADE;		}	/* 背景のフェードアウト */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_kaiwa_youso->i_code = I_CODE_0e_BGM;			}	/* BGM変更 */
			else	if (0==tiny_strcmp(command, "BGM_VOLUME"))	{	new_kaiwa_youso->i_code = I_CODE_0f_BGM_VOLUME; 	}	/* BGM 音量 変更 */
		if (0==tiny_strcmp(command, 			"BOSS"))
		{
					if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_10_BOSS_LOAD;		}	/* ボス読み込み */
			else	if (0==tiny_strcmp(c_p0,	"term"))		{	new_kaiwa_youso->i_code = I_CODE_11_BOSS_TERMINATE; }	/* result後にゲーム強制終了。 */
			#if (1)
			else	if (0==tiny_strcmp(c_p0,	"start"))		{	new_kaiwa_youso->i_code = I_CODE_12_BOSS_START; 	}	/* ボス攻撃開始。 */
			else	if (0==tiny_strcmp(c_p0,	"result"))		{	new_kaiwa_youso->i_code = I_CODE_13_BOSS_RESULT;	}	/* (r35)現在仕様の都合上要る。これが無いとリザルト画面が巧く出ない。 */
			#endif
		}
	}
	else
	if ('D'==(*command))
	{
		if (0==tiny_strcmp(command, 			"DRAW"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_kaiwa_youso->i_code = I_CODE_06_DRAW_PANEL; 	}	/* スコアパネルの表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_04_DRAW_TEXT;		}	/* テキスト画面の表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_05_DRAW_BG;		}	/* 背景の表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"skip"))		{	new_kaiwa_youso->i_code = I_CODE_07_DRAW_SKIP;		}	/* */
			else	if (0==tiny_strcmp(c_p0,	"color"))		{	new_kaiwa_youso->i_code = I_CODE_0a_FONT_COLOR; 	}	/* 文字の色 */
			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_0b_FONT_SPEED; 	}	/* 文字の表示速度 */
		}
	}
	else
	if ('O'==(*command))
	{
					if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_kaiwa_youso->i_code = I_CODE_02_OBJ_LOAD;		strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* 汎用オブジェのロード / 再読み込み / 汎用オブジェに画像を読みこむ。 */
		if (0==tiny_strcmp(command, 			"OBJ"))
		{
					if (0==tiny_strcmp(c_p0,	"look"))		{	new_kaiwa_youso->i_code = I_CODE_00_OBJ_LOOK;		}	/* オブジェクト注目コマンド */
			else	if (0==tiny_strcmp(c_p0,	"xy"))			{	new_kaiwa_youso->i_code = I_CODE_01_OBJ_XY; 		}	/* オブジェクトの原点設定 */
			else	if (0==tiny_strcmp(c_p0,	"wait"))		{	new_kaiwa_youso->i_code = I_CODE_03_OBJ_WAIT;		}	/* オブジェクト移動完了まで待つ。 */
//			else	if (0==tiny_strcmp(c_p0,	"draw"))		{	new_kaiwa_youso->i_code = I_CODE_03_OBJ_DRAW;		}	/* オブジェクトの表示/非表示 */
//			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_OBJ_SPEED; 		}	/* オブジェクトの速度設定 */
		}
	}
	else
//	if ('T'==(*command))
//	{
					if (0==tiny_strcmp(command, "TEXT"))		{	new_kaiwa_youso->i_code = I_CODE_08_TEXT;			strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* メッセージウィンドウへの書き込み */
//	}
	else
//	if ('H'==(*command))
//	{
					if (0==tiny_strcmp(command, "HOLD"))		{	new_kaiwa_youso->i_code = I_CODE_09_HOLD;				}	/* クリック待ち */
//	}

//	if ('C'==(*command))
//	{	if (0==tiny_strcmp(command, 			"CURSOR"))
//		{
//					if (0==tiny_strcmp(c_p0,	"home"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_HOME;			}	/* カーソルの初期化 */
//			else	if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_POP;			}	/* 記憶したカーソル位置の呼び出し */
//			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_PUSH;			}	/* カーソル位置の記憶 */
//			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_kaiwa_youso->i_code = I_CODE_09_HOLD;				}	/* クリック待ち */
//			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_08_TEXT_WRITE_TEXT; }	/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
//			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_BG_WRITE_TEXT; 	}	/* BGウィンドウに漢字の文字を描く様に設定する。 */
//		}
//	}

	if ( (MAX_STR_STACK-1) < str_stack_position)
	{	/* 文字列スタック、オーバーフローで強制終了。 */
		error(ERR_FATAL, (char*)"string stack over flow.(%d)\n", str_stack_position);
	}
	if (NULL==kaiwa_youso_list_scan)/* 先頭なら */
	{
		kaiwa_youso_list_bigin		= new_kaiwa_youso;				/* 開放時に必要な先頭位置を保持する。 */
		kaiwa_youso_list_scan		= kaiwa_youso_list_bigin;		/* 始めのスキャン位置は開始位置とする。 */
	}
	else/* 先頭以外なら連結処理。 */
	{
		kaiwa_youso_list_scan->next = new_kaiwa_youso;				/* 現在位置の次に新規を連結。 */
		kaiwa_youso_list_scan		= kaiwa_youso_list_scan->next;	/* 新規を現在位置に変更する。 */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/


/*---------------------------------------------------------
	シナリオ会話 読み込み
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
	str_stack_position = 0;/* 文字列領域 */
	int opecode_entrys		= 0;	/* 命令がが書き込まれているかどうか。 */
	{
		MY_FILE_LOOP_BEGIN;
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* parth start. 構文解析開始 */
		int end_arg;	end_arg = 0;		/* 行末 == 引数の取得の中止 */
		char char_command[16];/*15*/		/* 基本コマンド */
		char c_p0[256];/*200*/
		ch = read_opecode(ch, char_command, &end_arg);		/* 基本コマンド名称(オペコード)読み込み */
		if (NULL==ch)										{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		if (!end_arg)/* 行末 */
		{
			if (' ' != (*ch))								{	GOTO_ERR_WARN;	}	/* 区切り */
			ch++;
			ch = load_my_file_get_str(ch, c_p0, &end_arg/*, ','*/); 				/* 文字列コマンド(オペランド)読み込み */
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
						ch = read_operand_int_only(ch, &c_pn[kk], &end_arg);			/* 数値コマンド(オペランド)読み込み */
						if (NULL==ch)							{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
					}
				}
			}
			regist_kaiwa_youso(char_command, c_p0, c_pn);
		}
		opecode_entrys++;
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(ERR_WARN, (char*)"syntax error in kaiwa file '%s', line no: %d", my_file_common_name, debug_number_of_read_line);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	//
	my_file_fclose(/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		error(ERR_WARN, (char*)"can't entrys from kaiwa file %s", my_file_common_name);
		return (0);/* 読み込み失敗 */
	}
	/* 読み込み成功 */
	kaiwa_youso_list_scan		= kaiwa_youso_list_bigin;/* シナリオ会話 実行位置を、開始位置とする。 */
	return (1);
}

/////////////////////////////

/*---------------------------------------------------------
	シナリオ会話システム用
	カーソル位置キャッシュ
---------------------------------------------------------*/
static int is_bg;	/*=0*/					/* 背景表示/非表示フラグ */

//static int cursor_x_chached;			/* カーソル位置 保存用 */
//static int cursor_y_chached;			/* カーソル位置 保存用 */

/*---------------------------------------------------------
	シナリオ会話システム内の背景
---------------------------------------------------------*/

#if (1)
static u16 *bg_story_window_surface_image;
static int bg_story_window_surface_pitch;
static int bg_story_window_surface_width;
#endif

static SDL_Surface *bg_story_window_surface;			/* シナリオ会話システム内の背景 */

/*---------------------------------------------------------

---------------------------------------------------------*/

//static extern int my_string_offset;
/*static*/ extern void clear_my_string_offset(void);
static void aaa_kaiwa_system_reset(void)
{
	unsigned int i;
	for (i=0; i<KAIWA_SPRITE_99_MAX; i++)/*20*/
	{
		remove_kaiwa_sprite_num(i);
	}
//	remove_kaiwa_sprite_num(KAIWA_SPRITE_00_TACHIE_RIGHT);
//	remove_kaiwa_sprite_num(KAIWA_SPRITE_01_TACHIE_LEFT_);
	if (NULL != bg_story_window_surface)
	{
		SDL_FreeSurface(bg_story_window_surface);
		bg_story_window_surface 	= NULL;
	}
	is_bg = (0);/* 開放したんだから bg 表示できない、したがって強制表示 off */
	//
	kanji_cursor_move_home_position();		/* 漢字カーソルをホームポジションへ移動 */

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
		bg_story_window_surface_image	= (u16 *)bg_story_window_surface->pixels;
		bg_story_window_surface_pitch	= bg_story_window_surface->pitch;
		bg_story_window_surface_width	= bg_story_window_surface->w;
	}
	#if 0/* 現状、要らない？。 */
	SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	#endif
}


/*---------------------------------------------------------
	シナリオ会話システム、内部処理
---------------------------------------------------------*/
static int re_draw_count_flag;
global void kaiwa_system_set_re_draw(void)
{
	re_draw_count_flag = (1);/* 描画してね */
}

/* 内部コマンドの受け渡し形式。 */
#define MY_ICODE_FUNC(name) 		static void name(KAIWA_YOUSO *ssc)
#define MY_ICODE_GET_FUNC(name) 	static int name(void)

/*---------------------------------------------------------
	[ "i_code_" シナリオ会話システム内部コマンド]
---------------------------------------------------------*/
extern void load_SDL_bg_file_name(char *file_name);

//static int i_code_fade_bg_exec_fade;
//MY_ICODE_GET_FUNC(i_code_get_bg_fade)
//{
//	return (i_code_fade_bg_exec_fade);
//}

MY_ICODE_FUNC(i_code_load_bg)
{
//	pop_str();
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; 	//	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int alpha;				alpha			= ssc->para1;
//	int fade_command;		fade_command	= ssc->para2;
//	int set_alpha_speed;//	set_alpha_speed = ssc->para3;

	re_draw_count_flag = (1);/* 描画してね */

	load_bg_aaa(/*filename*/(&str_stack[str_stack_position][0]), alpha);str_stack_position++;/*pop_string();(文字列スタックを消化する)*/
}
//	if (filename)
//	{
//		load_bg_aaa(filename, 0/*alpha*/);
//	}

//	/*fade_in_command_start*/

	static int i_code_fade_bg_bg_alpha_speed;		/* 背景α値用 */
	static int i_code_fade_bg_exec_fade;		/* do fade */
MY_ICODE_FUNC(i_code_fade_bg)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; 		filename		= ssc->para0;
//	int alpha;				alpha			= ssc->para1;
//	int fade_command;	//	fade_command	= ssc->para2;
//	if ((1)==fade_command)/*1==fade_in_command*/

	re_draw_count_flag = (1);/* 描画してね */
//
	//
	bg_alpha = 0;
	int i_code_fade_bg_set_alpha_speed;//	set_alpha_speed = ssc->para3;
//	if (-1 == set_alpha_speed)/*パラメーター省略時*/
	{
		i_code_fade_bg_set_alpha_speed = 5;
	}
	i_code_fade_bg_bg_alpha_speed = i_code_fade_bg_set_alpha_speed;
	is_bg					= 1;	check_is_bg();
	i_code_fade_bg_exec_fade = (1);/*on*/	/* 処理中 */

//	return /*(0)*/; 	/* 処理中 */
}
static void kaiwa_system_vbl_do_BG_fade(void)
{
		if (0!=i_code_fade_bg_exec_fade)
		{
			re_draw_count_flag = (1);/* 描画してね */
//			psp_clear_screen();
//			SD L_SetAlpha(bg_story_window_surface, SD L_SRCALPHA, bg_alpha);
//			SD L_BlitSurface(bg_story_window_surface, NULL, sdl_screen[SD L_00_SCR EEN], NULL);
			bg_alpha += i_code_fade_bg_bg_alpha_speed/*5*/;
			if (250 < bg_alpha)
			{
				bg_alpha = 255;
				#if 0/*???*/
				/*	何か良くわからないけど、ここはおかしい???。 */
				if (NULL != bg_story_window_surface)
				{
					unloadbmp_by_surface(bg_story_window_surface);	// キャッシュに入ってるのでNULLに出来ない。bg_story_window_surface = NULL;
				}
				#endif
			//	i_code_fade_bg_exec_fade = (0); 	/* 処理完了 */
				i_code_fade_bg_exec_fade = (0); 	/* 処理完了 */
			//	return /*(1)*/; 	/* 処理完了 */
			}
		//	else
		//	{
		//		i_code_fade_bg_exec_fade = (1); 	/* 処理中 */
		//	}
		}
}

/*---------------------------------------------------------
	move refresh
---------------------------------------------------------*/
static unsigned int obj_look_up;
static void obj_move_refresh(void)
{
	re_draw_count_flag = (1);/* 描画してね */
//
	unsigned int nnn;
	for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
	{
		int hide_mode;	/*(隠れるモード)*/
		int offset_x;
		int offset_y;
		KAIWA_SPRITE *my_std_obj;
		my_std_obj = &kaiwa_sprite[((nnn)&(KAIWA_SPRITE_99_MAX-1))]; /* 汎用オブジェ */
		//
		my_std_obj->move_flag = KAIWA_SPRITE_MOVE_FLAG_01_MOVE_START;

		if (KAIWA_SPRITE_99_MAX>(obj_look_up))/* 通常モード */
		{
			if ((nnn)!=(obj_look_up))/* 注目オブジェ以外ならオフセットあり */
			{			hide_mode = 1;		}	/*(隠れるモード ON)*/
			else/* 注目オブジェならオフセット無し */
			{			hide_mode = 0;		}	/*(隠れるモード OFF)*/
		}
		else/*(初期モード)*/
		{
			hide_mode = 0;	/*(隠れるモード OFF)*/	/*(全部、注目オブジェ)*/
		}
		if (0==hide_mode)/* 注目オブジェならオフセット無し */
		{	/*(隠れるモード OFF)*/
			offset_x	= (0);
			offset_y	= (0);
		//	my_std_obj->alpha255 = (255);	/* 色 */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (255) );
		}
		else/* 注目オブジェ以外ならオフセットあり */
		{	/*(隠れるモード ON)*/
			offset_x	= my_std_obj->offset_x256;
			offset_y	= my_std_obj->offset_y256;
		//	my_std_obj->alpha255 = (180);	/* 色 */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (180) );
		}

		/* あらかじめ移動完了位置を算出しておく */
	//	my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-((my_std_obj->w)>>1))<<8);	/* 移動完了座標 */
	//	my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-((my_std_obj->h)>>1))<<8);	/* 移動完了座標 */
		my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-((1<<(my_std_obj->w_bit))>>1))<<8);	/* 移動完了座標 */
		my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-((1<<(my_std_obj->h_bit))>>1))<<8);	/* 移動完了座標 */

		/* 移動方向を決める */
	//	my_std_obj->angle1024	= at an_1024(y,x);
	//	my_std_obj->angle1024	= at an_65536(y,x);
		/*(移動完了予定位置から現在位置を引けば、方向が出る)*/
		{//1579619 1579683 1579715
			REG_00_SRC_X	= (my_std_obj->target_x256);
			REG_01_SRC_Y	= (my_std_obj->target_y256);
			REG_02_DEST_X	= (my_std_obj->cx256);
			REG_03_DEST_Y	= (my_std_obj->cy256);
			tmp_angleCCW65536_src_nerai();
			my_std_obj->angle1024	= HATSUDAN_03_angle65536;
		}
		my_std_obj->angle1024	>>= (6);	/* 1周の分割数を 65536 から 1024 へ変換 */
		//
	//	my_std_obj->speed256 = ((40)<<(8-3));	/* 初速度 (x32) */
	//	my_std_obj->speed256 = ((20)<<(8-3));	/* 初速度 (x32) */
		my_std_obj->speed256 = (0); 			/* 初速度 (x32) */
	}
}

/*---------------------------------------------------------
	[ "i_code_" シナリオ会話システム内部コマンド]
	I_CODE_01_OBJ_XY オブジェクトの原点座標の設定。
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_xy)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *color32_str;		color32_str = ssc->para0;
	int num;				num 		= ssc->para1;
	int origin_x;			origin_x	= ssc->para2;
	int origin_y;			origin_y	= ssc->para3;
//
	KAIWA_SPRITE *my_std_obj;
	my_std_obj = &kaiwa_sprite[((num)&(KAIWA_SPRITE_99_MAX-1))]; /* 汎用オブジェ */
	//
	my_std_obj->origin_x256 = ((origin_x)<<8);		/* 原点座標の設定 */
	my_std_obj->origin_y256 = ((origin_y)<<8);		/* 原点座標の設定 */
	obj_move_refresh();
}

/*---------------------------------------------------------
	[ "i_code_" シナリオ会話システム内部コマンド]
	I_CODE_00_OBJ_LOOK 注目オブジェの番号を設定。
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_look)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *color32_str;		color32_str = ssc->para0;
/*	int mode;	*/			obj_look_up 		= ssc->para1;
	obj_move_refresh();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------
	[ "i_code_" シナリオ会話システム内部コマンド]
	オブジェクトに画像を読み込む
	i_code_obj_load_gazo
	[ "i_code_" シナリオ会話システム内部コマンド]
	読み込み済みのオブジェクトの画像を再読み込みして、
	位置をそのまま継続し画像は入れ替える。
	i_code_swap_image_sprite
---------------------------------------------------------*/

//static /*KAIWA_SPRITE **/void
MY_ICODE_FUNC(i_code_obj_load_gazo)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int sitei_number;	sitei_number	= ssc->para1;
//	int xx; 			xx				= ssc->para2;
//	int yy; 			yy				= ssc->para3;
//
	re_draw_count_flag = (1);/* 描画してね */
	sitei_number &= ((KAIWA_SPRITE_99_MAX-1));
//	build_sprite(sitei_number);
//	static void build_sprite(int sitei_number)
	remove_kaiwa_sprite_num(sitei_number);	/* 使用中なら */	/* 画像を開放する。 */	/* 強制的に開放 */
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename, 0/*alpha*/);
	suf 				= load_local01(/*filename*/(&str_stack[str_stack_position][0]), suf);str_stack_position++;/*pop_string();(文字列スタックを消化する)*/	//, 0/*alpha*/);
	if (NULL==suf)		{	return; 	}/*(NULL)*/

	KAIWA_SPRITE *my_std_obj;
	my_std_obj = &kaiwa_sprite[sitei_number]; /* 汎用オブジェ */

	my_std_obj->img 	= suf;	/* 使用中になる。 */
//	my_std_obj->frames = frames;
	/*(縦横共、強制的に2^nに変換)*/
	my_std_obj->w_bit		= (32 - __builtin_allegrex_clz((suf->w)-1));/*/frames*/ 	//tmp->cw		= ((tmp->w)>>1);
	my_std_obj->h_bit		= (32 - __builtin_allegrex_clz((suf->h)-1));				//tmp->ch		= ((tmp->h)>>1);

}

/*---------------------------------------------------------
	実行停止解除用件
---------------------------------------------------------*/
enum /* マルチタスク機能のタスクスイッチ */
{
	TASK_SWITCH_00_EXECUTE							= 0x00,/*(0<<0)*/
	TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT			= 0x01,/*(1<<0)*/
	TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE		= 0x02,/*(1<<1)*/
	TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING 		= 0x04,/*(1<<2)*/
	TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT	= 0x08,/*(1<<3)*/
};
enum
{
	KAIWA_SYSTEM_00_CONTINUE = 0,
	KAIWA_SYSTEM_01_TERMINATE,
};
static u8 kaiwa_system_hold_mode;
static u8 kaiwa_system_skip_mode;
static u8 kaiwa_system_terminate_flag;

static void kaijyo_obj_all_stop(void)
{
	int check_flag;
	check_flag = 0;
	/* 全OBJが停止しているかチェックする。 */
	int nnn;
	for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
	{
		KAIWA_SPRITE *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* 汎用オブジェ */
		if (KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag )
		{
			check_flag = (1);/* 移動中 */
		}
	}
	/* 全OBJが停止している場合のみ、解除。 */
	if (0==check_flag)/* 全OBJ停止している。 */
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT;/* 解除 */
	}
}


/*---------------------------------------------------------
	会話通常時のパッドチェック
	-------------------------------------------------------
	制限時間は、本家と同じ10[秒]とする。
---------------------------------------------------------*/

static int limit_timer;/*(制限時間)*/
static void kaijyo_pad_timer_hold(void)
{
	/* ==== ボタン入力処理 ==== */
	u8 shot_osita;
	shot_osita = 0; /* ショット押した */
	if (0 == psp_pad.pad_data)/* 今は押してない */
	{
		if (psp_pad.pad_data_alter & PSP_KEY_BOMB_CANCEL)	/* キャンセル入力 */
		{
			kaiwa_system_skip_mode = (1);/*on*/
		}
		if (psp_pad.pad_data_alter & PSP_KEY_SHOT_OK)		/* ＯＫ入力 */
		{
			shot_osita = 1; 	/* ショット押したよ */
		}
	}
	{
		if (0 != (shot_osita | /*要するにor*/kaiwa_system_skip_mode))/*(ショット押した)*/
		{
			limit_timer = byou60(0);/*(0[秒]時間切れ)*/
		}
		limit_timer--;/*(時間経過)*/
		if (byou60(0) > limit_timer)/*(0[秒]時間切れ)*/
		{
			kaiwa_system_hold_mode &= ~TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE;/* 解除 */
			#if 1	/* 最後のみ行う */
			kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
			#endif
		}
	}
}

/*---------------------------------------------------------
	会話起動時のパッドチェック
	-------------------------------------------------------
	ボタンが離されているのを確認する。
	これが無いと、シナリオが見る前から飛ばされる事がある。
	-------------------------------------------------------
	会話起動時に、ボムボタンが押されていると、
	会話を見る前から、スキップモードになるので、
	会話が読めない。
	これを防止する為の特別機能。
	-------------------------------------------------------
	ただし、ボタンを押しつづけていると、いつまで経っても
	会話が始まらないので、制限時間を設定する。
	制限時間は、とりあえず1[秒]にしといた。
---------------------------------------------------------*/

static void kaijyo_pad_boot_hold(void)
{
	/* ==== ボタン入力処理 ==== */
	{
		if (0 == psp_pad.pad_data)/* 今は押してない */
		{
			limit_timer = byou60(0);/*(0[秒]時間切れ)*/
		}
		limit_timer--;/*(時間経過)*/
		if (byou60(0) > limit_timer)/*(0[秒]時間切れ)*/
		{
			kaiwa_system_hold_mode &= ~TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT;/* 解除 */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static int i_code_text_color;		/* テキストメッセージ描画色 */
static int i_code_text_speed_wait;	/* テキストメッセージ描画ウェイト */

//0static int i_code_shot_ositenai; /* */
static char i_code_text_para0[256];

static void kaijyo_text_drawing(void)
{
//	if (0 == 0/*i_code_shot_ositenai*/) 	{	i_code_text_speed_wait=0;	}	/* ショット押したら、残りは全部書く */
	/*sdl_lock_surface,*/
	strcpy(my_font_text, i_code_text_para0);
	kanji_color(i_code_text_color);
	kanji_wait(i_code_text_speed_wait);
	int osimai;
	osimai = 0;
	osimai = kanji_draw();
	kanji_wait(0);
	if (0!=osimai)
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING;/* 解除 */
	}
}



/*---------------------------------------------------------
	シナリオ会話システム、オブジェクトの移動
---------------------------------------------------------*/

static void kaiwa_system_vbl_sprite_move(void)
{
	int nnn;
	for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
	{
		KAIWA_SPRITE *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* 汎用オブジェ */
		if (KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag )
		{
			re_draw_count_flag = (1);/* 描画してね */
			{
				{
				//	my_std_obj->speed256 += (5*32); 		/* 160==5x32(速すぎ) 加速度(東方の立ち絵は常に加速して移動する) (x32) */
				//	my_std_obj->speed256 += (4*32); 		/* 128==4x32() 加速度(東方の立ち絵は常に加速して移動する) (x32) */
					my_std_obj->speed256 += (3*32); 		/*	96==3x32() 加速度(東方の立ち絵は常に加速して移動する) (x32) */
				//	if (t256(6.0)<my_std_obj->speed256) 	{	my_std_obj->speed256 = t256(8.0);	}	/* 最大速度8[dots/flame] t256(6.0) */
					my_std_obj->speed256 = psp_min(my_std_obj->speed256, (t256(8.0) ));
				//	my_std_obj->speed256 = t256(0.1);/*(でばっぐ)*/
					#if (0)//
					my_std_obj->cx256 += ((si n1024((my_std_obj->angle1024))*(my_std_obj->speed256))>>8);/*fps_factor*/ 	/* CCWの場合 */
					my_std_obj->cy256 += ((co s1024((my_std_obj->angle1024))*(my_std_obj->speed256))>>8);/*fps_factor*/
					#else
					{
						int sin_value_t256; 	//	sin_value_t256 = 0;
						int cos_value_t256; 	//	cos_value_t256 = 0;
						int256_sincos1024( (my_std_obj->angle1024), &sin_value_t256, &cos_value_t256);
						my_std_obj->cx256 += ((sin_value_t256*(my_std_obj->speed256))>>8);/*fps_factor*/
						my_std_obj->cy256 += ((cos_value_t256*(my_std_obj->speed256))>>8);/*fps_factor*/
					}
					#endif
				}
				void *aaa_sss[(4)] =
				{
					&&aaa_sss_0,
					&&aaa_sss_1,
					&&aaa_sss_2,
					&&aaa_sss_3,
				};
				/* 大体の方向: なので ４５度(1周を8分割)回転してずらして、上位２ビットが大まかな方向 */
				goto *aaa_sss[(((my_std_obj->angle1024+1024-(1024/8))>>8) & 0x03)];/*deg_360_to_1024(45)*/
			//	goto *aaa_sss[(((my_std_obj->angle1024+(1024/8))>>8) & 0x03)];/*deg_360_to_1024(45)*/
				/* 移動完了座標に等しいかはみ出したら、完了とする。 */
				aaa_sss_0:/* 1:→(1024_0) */	if (my_std_obj->cx256 < my_std_obj->target_x256)	{	goto move_continue /*(0)*/;/*移動中*/	}	goto move_complete;
				aaa_sss_1:/* 2:↓(1024_1) */	if (my_std_obj->cy256 > my_std_obj->target_y256)	{	goto move_continue /*(0)*/;/*移動中*/	}	goto move_complete;
				aaa_sss_2:/* 3:←(1024_2) */	if (my_std_obj->cx256 > my_std_obj->target_x256)	{	goto move_continue /*(0)*/;/*移動中*/	}	goto move_complete;
				aaa_sss_3:/* 0:↑(1024_3) */	if (my_std_obj->cy256 < my_std_obj->target_y256)	{	goto move_continue /*(0)*/;/*移動中*/	}	goto move_complete;
			move_complete:
				/* 移動完了した場合は、正確な座標に修正する。 */
				my_std_obj->cx256		= my_std_obj->target_x256;
				my_std_obj->cy256		= my_std_obj->target_y256;
				my_std_obj->move_flag	= KAIWA_SPRITE_MOVE_FLAG_00_MOVE_COMPLETE;/*移動完了*/
				//return /*(1)*/;/*移動完了*/
			move_continue:
				;
			}
		}
	}
}
/*
大体の方向の求め方：
角度は下が0度で、半時計回り。(下CCW)
角度が 1024 分割(0-1023)なので 256で割ると 4になる。(0, 1, 2, 3)のどれかになる。
つまり下記図で、それぞれ (0)右下, (1)右上, (2)左上, (3)左下 の領域。
+-------------+-------------+
| +   (2)	  | 	(1)   + |
|	+ [1]	 [1]	[1] +	|
|(2)  + 	  | 	  +  (1)|
|[2]	+	  | 	+	 [0]|
|		  +   |   + 		|
|			+ | +			|
+-[2]---------+---------[0]-+
|			+ | +			|
|[2]	  +   |   + 	 [0]|
|(3)	+	  | 	+	 (0)|
|	  + 	 [3]	  + 	|
|	+ [3]	  | 	[3] +	|
| +   (3)	  | 	(0)   + |
+-------------+-------------+
このままでは使いにくい。図で言えば、それぞれ [0], [1], [2], [3] の領域が欲しい。
そこで予め45度分引いてから、256で割るとれ [0]↓, [1]→, [2]↑, [3]← の領域が出る。
*/


/*---------------------------------------------------------
	シナリオ会話システム、内部実行処理
	シナリオ会話システム コマンド解釈(実行)系
---------------------------------------------------------*/

static /*int*/void kaiwa_system_execute(void)
{
	KAIWA_YOUSO *ssc;
	ssc = kaiwa_youso_list_scan;/* 実行開始位置 */
//
	void *aaa[I_CODE_99_MAX] =
	{
	// OBJ関連
		&&case_I_CODE_00_OBJ_LOOK,
		&&case_I_CODE_01_OBJ_XY,
		&&case_I_CODE_02_OBJ_LOAD,
		&&case_I_CODE_03_OBJ_WAIT,
	// 画面制御
		&&case_I_CODE_04_DRAW_TEXT,
		&&case_I_CODE_05_DRAW_BG,
		&&case_I_CODE_06_DRAW_PANEL,
		&&case_I_CODE_07_DRAW_SKIP,
	// テキスト関連
		&&case_I_CODE_08_TEXT,
		&&case_I_CODE_09_HOLD,
		&&case_I_CODE_0a_FONT_COLOR,
		&&case_I_CODE_0b_FONT_SPEED,
	// 背景/BGM
		&&case_I_CODE_0c_BG_LOAD,
		&&case_I_CODE_0d_BG_FADE,
		&&case_I_CODE_0e_BGM,
		&&case_I_CODE_0f_BGM_VOLUME,
	// ボス制御
		&&case_I_CODE_10_BOSS_LOAD,
		&&case_I_CODE_11_BOSS_TERMINATE,/*(r34新設)*/
		&&I_CODE_break, /*(必要ダミー)*/	// &&case_I_CODE_12_BOSS_START,
		&&I_CODE_break, /*(必要ダミー)*/	// &&case_I_CODE_13_BOSS_RESULT,/*(r35新設)*/
	//
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
	//
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
	//
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
	//
	//	I_CODE_99_MAX/*(2^n)*/	// 必要ないが安全の為
	};
	goto *aaa[(ssc->i_code)&0x1f];/*(ccc)&(I_CODE_99_MAX-1)*/
	{
	case_I_CODE_00_OBJ_LOOK:		i_code_obj_look(ssc);											goto I_CODE_break;	/* オブジェクト注目コマンド */
	case_I_CODE_01_OBJ_XY:			i_code_obj_xy(ssc); 											goto I_CODE_break;	/*(原点設定のみ)*/
	case_I_CODE_02_OBJ_LOAD:		i_code_obj_load_gazo(ssc);										goto I_CODE_break;	/*(読みこみのみ) 汎用絵  立ち絵L	立ち絵R */
	case_I_CODE_03_OBJ_WAIT:		kaiwa_system_hold_mode |= TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT; /* 停止 */	goto I_CODE_break;
	//
	case_I_CODE_04_DRAW_TEXT:	//廃止	cg.dr aw_flag_kaiwa_screen	= ssc->para1;
		#if (0)/*(最近の(gcc4.3.5)はたぶん同じ。gcc4.0.2ぐらい古いと違う。)*/
		if (ssc->para1&1)	{	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [黒/会話用背景]せりふ背景on */}
		else				{	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */}
		#else
		{	ml_font[(0)].haikei 		= (ssc->para1&1)?(ML_HAIKEI_03_MESSAGE)/* [黒/会話用背景]せりふ背景on */:(ML_HAIKEI_m1_OFF);/* せりふ背景off */}
		#endif
		goto I_CODE_break;
	case_I_CODE_05_DRAW_BG: 		is_bg						= ssc->para1;	check_is_bg();		goto I_CODE_break;
	case_I_CODE_06_DRAW_PANEL:		cg.side_panel_draw_flag 	= ssc->para1;						goto I_CODE_break;
	case_I_CODE_07_DRAW_SKIP:		kaiwa_system_skip_mode		= ssc->para1;						goto I_CODE_break;
	//
	case_I_CODE_08_TEXT:
				strcpy(i_code_text_para0, (&str_stack[str_stack_position][0]));str_stack_position++;/*pop_string();(文字列スタックを消化する)*/
			//	strcpy(i_code_text_para0, ssc->para0);
					kaiwa_system_hold_mode |= TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING;					goto I_CODE_break;
	case_I_CODE_09_HOLD:			limit_timer 				= byou60(10);/*(10[秒]制限時間。本家互換)*/ 	kaiwa_system_hold_mode |= TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE; goto I_CODE_break;
	case_I_CODE_0a_FONT_COLOR:		i_code_text_color			= ssc->para1;						goto I_CODE_break;
	case_I_CODE_0b_FONT_SPEED:		i_code_text_speed_wait		= ssc->para1;						goto I_CODE_break;
	//
	case_I_CODE_0c_BG_LOAD: 		i_code_load_bg(ssc);											goto I_CODE_break;
	case_I_CODE_0d_BG_FADE: 		i_code_fade_bg(ssc);											goto I_CODE_break;
	case_I_CODE_0e_BGM: 			play_music_num( (ssc->para1) ); 								goto I_CODE_break;
	case_I_CODE_0f_BGM_VOLUME:		set_music_volume( (ssc->para1) );								goto I_CODE_break;
	case_I_CODE_10_BOSS_LOAD:		called_from_kaiwa_system_boss_load( (ssc->para1));				goto I_CODE_break;/*boss_number*/
	case_I_CODE_11_BOSS_TERMINATE:	cg.state_flag |= STATE_FLAG_16_GAME_TERMINATE;					goto I_CODE_break;
		/*	called_from_kaiwa_system_boss_start();*/
		/*	cg.state_flag |= STATE_FLAG_05_IS_BOSS; */
		/* ボス戦闘前の会話終了を設定 */
//	case_I_CODE_12_BOSS_START:		/*ダミー*/; 													goto I_CODE_break;
//	case_I_CODE_13_BOSS_RESULT: 	/*ダミー*/; 													goto I_CODE_break;
	//
//	case_I_CODE_OBJ_SPEED:			i_code_set_sprite_speed(ssc);									goto I_CODE_break;
	// [テキストのカーソル制御]
//	case I_CODE_CUR_POP:			cursor_x = cursor_x_chached;	cursor_y = cursor_y_chached;		break;	/* カーソル位置、復元 */
//	case I_CODE_CUR_PUSH:			cursor_x_chached = cursor_x;	cursor_y_chached = cursor_y;		break;	/* カーソル位置、記憶 */
//	case I_CODE_CUR_HOME:			kanji_cursor_move_home_position();						break;	/* 漢字カーソルをホームポジションへ移動 */		/* カーソルの初期化 */
//	case I_CODE_BG_WRITE_TEXT:		set_write_text(1);					break;	/* BGウィンドウに漢字の文字を描く様に設定する。 */
//	case I_CODE_08_TEXT_WRITE_TEXT: set_write_text(0);					break;	/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
	// [テキスト表示/クリック待ち]
	}
I_CODE_break:
	;
	/* 次回用の頭だしと終了判断 */
	//
	/* (終わりの場合、次のコマンドを調べる) */
	kaiwa_youso_list_scan = kaiwa_youso_list_scan->next;/* 次回はここから調べる */
	if (NULL == kaiwa_youso_list_scan)/* シナリオ会話が最後なら */
	/* (
		calloc() して確保したので、暗黙に NULL が入っているという、ちょっとトリッキーなコード。(calloc() は 0クリアする)
		つまり malloc だと駄目かも。(malloc() は 0クリアしない)
		実使用上問題ないが、あまり好ましくないコード。(C言語の規格上 0==NULL は、規格外なので本来良くない。)
		)
	*/
	{
		/* シナリオ会話が最後なら */
		kaiwa_system_terminate_flag = KAIWA_SYSTEM_01_TERMINATE;/*(1)*/ 	/* シナリオ会話処理はおしまい */
	}
	else
	{
		kaiwa_system_terminate_flag = KAIWA_SYSTEM_00_CONTINUE;/*(0)*/		/* シナリオ会話処理はつづく */
	}
}

#define USE_SDL_DRAW (1)
#if (1==USE_SDL_DRAW)
/*---------------------------------------------------------
	オブジェクトの描画
---------------------------------------------------------*/

static void s_draw_kaiwa_system(void)
{
	/* ----- 描画系 ----- */
	/* 1. まず背景を描く */
	if (is_bg)
	{
		if (255 > bg_alpha)
		{
			SDL_SetAlpha(bg_story_window_surface, SDL_SRCALPHA, bg_alpha);
		}
		PSPL_UpperBlit(bg_story_window_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	}
	/* 2. 次に立ち絵を描く */
	#if (0==USE_GU_TACHIE_TEST)/* (r34)テスト */
	{
		unsigned int nnn;
		for (nnn=0; nnn<KAIWA_SPRITE_99_MAX; nnn++)
		{
			if (NULL != kaiwa_sprite[nnn].img)	/* 画像があるもののみ描画する。(使用中の場合に描画する) */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0);
				src_r.y = (0);
				src_r.w = (1<<(kaiwa_sprite[nnn].w_bit));
				src_r.h = (1<<(kaiwa_sprite[nnn].h_bit));
				dst_r.x = (t256_floor(kaiwa_sprite[nnn].cx256));
				dst_r.y = (t256_floor(kaiwa_sprite[nnn].cy256));
				PSPL_UpperBlit(kaiwa_sprite[nnn].img, &src_r, cb.sdl_screen[SDL_00_VIEW_SCREEN], &dst_r);
			}
		}
	}
	#endif
}
#endif /* (1==USE_SDL_DRAW) */

/*---------------------------------------------------------
	会話システムSDL描画
---------------------------------------------------------*/

global void kaiwa_system_SDL_draw(void)
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
			s_draw_kaiwa_system();	/* 会話システム内描画処理 */
			#endif /* (1==USE_SDL_DRAW) */
		}
	}
}


/*---------------------------------------------------------
	会話モード終了処理
---------------------------------------------------------*/

static void do_kaiwa_system_terminate(void)
{
//	bg_alpha = 0;
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */
//廃止	cg.dr aw_flag_kaiwa_screen	= (0);	/* せりふウィンドウ表示フラグ off */
	load_kaiwa_youso_free();	/* 前回のシナリオがメモリにあればすべて開放。 */
	aaa_kaiwa_system_reset();
//	is_bg = 0;/* (描画と分離できない)バグ修正 */
}


/*---------------------------------------------------------
	ゲームコア用、会話モード終了状態へ遷移
---------------------------------------------------------*/
static void kaiwa_system_terminate_game_core(void)
{
	cg.state_flag &= (~(STATE_FLAG_06_IS_KAIWA_MODE));/*off*/
	cg.state_flag |= STATE_FLAG_12_END_KAIWA_MODE;
	/* ボス戦闘前後イベント pd_bo ssmode = B08_START; = B09_STAGE_LOAD; */
}


/*---------------------------------------------------------
	シナリオ会話 動作中
---------------------------------------------------------*/
static void (*kaiwa_system_terminate_call_func)(void);

global void kaiwa_system_execute_move_only_main(void)
{
	/*---------------------------------------------------------
		(r33)やっと、動作と描画が分離できた－。
		だけど、Gu化、間にあわなかった。
	---------------------------------------------------------*/
	if (TASK_SWITCH_00_EXECUTE==kaiwa_system_hold_mode)/* 実行停止状態でなければ実行する。 */
	{	/* 実行停止状態でない。 */
		kaiwa_system_execute(); 	/* static_シナリオ会話 動作 */		/* 動作のみ(描画しない) */
	}
	else	/* 実行停止状態。 */
	{	/* 実行停止状態の場合は、実行停止解除用件をチェック */
		/* OBJ_WAITの開始用件 */
		if (kaiwa_system_hold_mode & TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT)
		{
			kaijyo_obj_all_stop();			/* 全OBJが停止していたら、解除。 */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE)
		{
			kaijyo_pad_timer_hold();		/* 時間経過もしくは、ボタン押したら、解除。 */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_04_HOLD_REASON_TEXT_DRAWING)
		{
			/* ==== text描画処理(非同期) ==== */
			kaijyo_text_drawing();			/* 描画終了なら、解除。 */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT)
		{
			kaijyo_pad_boot_hold(); 		/* 起動時専用。パッドのボタンが離されたら、解除。 */
		}
	}
	/* ==== BG描画処理(非同期) ==== */
	kaiwa_system_vbl_do_BG_fade();
	/* ==== obj移動処理(非同期) ==== */
	kaiwa_system_vbl_sprite_move(); /* 動作のみ(描画しない) */

//	/*でばっぐ*/		re_draw_count_flag = (1);/* 描画してね */
	if (/*(0)*/KAIWA_SYSTEM_00_CONTINUE != kaiwa_system_terminate_flag)
	{
		do_kaiwa_system_terminate();
		kaiwa_system_terminate_call_func();
	}
}


/*---------------------------------------------------------
	シナリオ会話、毎回、開始処理
---------------------------------------------------------*/

static int aaa_kaiwa_system_start(void) /* シナリオファイル名と背景ファイル名 */
{
	if (0 == s_load_scenario())
	{
		return (0);/* 読み込み失敗 */
	}
	kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
	set_kanji_hide_line(ML_LINE_04);/*(4行目以下を非表示にする。)*/
//
	kaiwa_system_skip_mode		= (0);/*off*/
	kaiwa_system_hold_mode		=
		(	TASK_SWITCH_08_HOLD_REASON_KEY_CHECK_ON_BOOT	/*(開始時はパッドが離されている事を確認する)*/
		|	TASK_SWITCH_01_HOLD_REASON_OBJECT_WAIT			/*(開始時はobjが退避している事を確認する)*/
		);
	kaiwa_system_terminate_flag = KAIWA_SYSTEM_00_CONTINUE;/*(0)*/	/* 初期化 */

	is_bg					= (0);
	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [黒/会話用背景]せりふ背景on */
//廃止	cg.dr aw_flag_kaiwa_screen	= (1);/*0*/
	#if 1
	/* std_obj[]初期化 */
	{	unsigned int i;
		for (i=0; i<KAIWA_SPRITE_99_MAX; i++)/*20*/
		{
			kaiwa_sprite[i].img = NULL;
		}
	}
	// プレイヤー
	kaiwa_sprite[0].cx256		= t256(-96);		kaiwa_sprite[0].cy256		= t256(16);
	kaiwa_sprite[0].origin_x256 = t256(-96);		kaiwa_sprite[0].origin_y256 = t256(16);
	kaiwa_sprite[0].offset_x256 = t256(-32);		kaiwa_sprite[0].offset_y256 = t256(16);
	// ボス
	kaiwa_sprite[1].cx256		= t256(352);		kaiwa_sprite[1].cy256		= t256(16);
	kaiwa_sprite[1].origin_x256 = t256(352);		kaiwa_sprite[1].origin_y256 = t256(16);
	kaiwa_sprite[1].offset_x256 = t256( 32);		kaiwa_sprite[1].offset_y256 = t256(16);
	obj_look_up 							= (KAIWA_SPRITE_99_MAX)+(1);/*(全部注目オブジェに設定)*/
	#endif
	limit_timer 				= byou60(1);/*(1[秒]制限時間。起動時のパッドチェック時間)*/
	i_code_text_color			= (7);
	i_code_text_speed_wait		= (0);
	str_stack_position = 0;/* 文字列領域 */
	return (1);
}


/*---------------------------------------------------------
	ゲームイベント中のシナリオ会話 開始(ストーリ、エンディング以外)
---------------------------------------------------------*/

global void kaiwa_load_ivent(void)
{
	/* ファイル名作成 */
	{
		/* 'data' '/kaiwa/' の分のオフセット */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_KAIWA_LENGTH)
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "Z/sZ1" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_file_common_name[DIRECTRY_NAME_OFFSET+0] = ('0'+(cg_game_select_player));
		my_file_common_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(cg.game_now_stage);
		//
		if ((cg.state_flag & STATE_FLAG_05_IS_BOSS))/*(ボス戦闘後の場合)*/
		{
			// my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '1';
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
					if ((0)==(cg.game_difficulty))/*(easyの場合)*/
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
	kaiwa_system_terminate_call_func = kaiwa_system_terminate_game_core;	/* シナリオ会話 終わったらゲームコア用状態遷移 */
	load_kaiwa_youso_free();			/* 前回のシナリオがメモリにあればすべて開放。 */
	if (0 == aaa_kaiwa_system_start())	// ファイルがない場合はイベントを飛ばす
	{
		cg.state_flag |= STATE_FLAG_12_END_KAIWA_MODE;
	}
	else
	{
		cg.state_flag |= STATE_FLAG_06_IS_KAIWA_MODE;	/*on*/
		#if 0
		/* シナリオ中にボムが発生してしまう。バグがあるので。 */
		pd_bomber_time = 0;
		#endif
	}
}


#if (1)/*Gu描画になると要らなくなる*/
/*---------------------------------------------------------
	「ストーリーモード専用」シナリオ動作
---------------------------------------------------------*/

static void story_mode_local_work(void)
{
	kaiwa_system_SDL_draw();					/* シナリオ会話システム SDL 描画(遅い) */
	kaiwa_system_execute_move_only_main();		/* シナリオ会話システム 動作(移動) */
}
#endif

/*---------------------------------------------------------
	「ストーリーモード専用」シナリオ開始
---------------------------------------------------------*/

global void story_mode_start(void)
{
//	bg_alpha = 0;
	/* ファイル名作成 */
	{
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "story" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
	}
	kaiwa_system_terminate_call_func = menu_cancel_and_voice;		/* 会話モードシナリオが終わったらタイトルメニューへ移動 */
	load_kaiwa_youso_free();		/* 前回のシナリオがメモリにあればすべて開放。 */
	aaa_kaiwa_system_start();		/* ストーリー用のシナリオ会話システム 開始 */
	#if 1/*Gu描画になると要らなくなる*/
	cb.main_call_func = story_mode_local_work;/* SDL描画の場合 */
	#else
//	cb.main_call_func = kaiwa_system_execute_move_only_main;/* Gu描画の場合 */
	#endif
}


/*---------------------------------------------------------
	pspでは開放が正常動作出来ないので、起動時に確保して(終了時まで)開放しない
---------------------------------------------------------*/

global void kaiwa_system_init(void)/* 組み込み */
{
	#if 1
	kaiwa_youso_list_bigin	= NULL; 	/* 命令列の開始位置を保持 */
	kaiwa_youso_list_scan	= NULL; 	/* 命令走査位置で使用 */
//
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */
//廃止	cg.dr aw_flag_kaiwa_screen	= (0);		/* せりふウィンドウ表示フラグ */
	is_bg						= (0);		/* 背景表示フラグ */
	kanji_cursor_move_home_position();		/* 漢字カーソルをホームポジションへ移動 */
	#endif
	bg_alpha					= (255);		/* 255==0xff 初期値 */

	/* SD L_FreeSurface(); は pspでは多分ちゃんと動かないので その対策 */
}


/*---------------------------------------------------------
	終了時に開放する部分...何だけど、
	ハングアップしたりするので、取り敢えず無効になってる。
	(開放しなくても終了すれば、OSがメモリ回収するので問題ないって言えば問題ない)
---------------------------------------------------------*/

global void kaiwa_system_exit(void)/* 外す */
{
	/*msg_window_init()*/
//
	#if 0/* 本当は要る */
	#endif
}
