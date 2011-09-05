
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	シナリオ会話 処理
	-------------------------------------------------------
	現在の問題点:
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
#include "../draw/gu_draw_screen.h"


/*static*/global KAIWA_OBJ kaiwa_sprite[KAIWA_OBJ_99_MAX];	/* シナリオ会話 汎用オブジェ */ /*32*/ /*20*/

/*---------------------------------------------------------
	インタプリタ命令(オペコード)を示す文字列を読む。
	10文字以上はエラー。
---------------------------------------------------------*/

static char *read_opecode(char *input_string, char *output_string, int *end_arg)
{
	int i = 0;
	while ((*input_string != ' ') && (*input_string != 13))
	{
		i++;
		if ((16-1) <= i)/*(15文字以上はエラー。)*/
		{	goto ne111;}
		*output_string++ = *input_string++;/*(ascii, 1文字転送)*/
	}
	if (13==(*input_string))	/* 改行コードは OD OA */
	{
		*end_arg = (1);
	}
	/*(文字列終端を勝手に書きかえる。)*/
	*output_string = (0);		/* [\0==EOT] */
	return (input_string);/*(???)*/
/*error*/
ne111:
	return ((char *)NULL);/*(エラー。)*/
}


/*[is_digit_or_hifun();'数字'もしくは'-'記号の場合。であるかを調べる。]*/

/* 無指定の場合の引数省略値と他の値をを区別する為に使用しない値を引数省略値に設定する。*/
/*(引数省略値の場合 -777 がセットされる仕様)*/
#define m1_SYOU_RYAKU_TI (-777)
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
		if ((32-1) <= i)/*20*/ /*(31文字以上はエラー。)*/
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
	/* ',' または '\n' が来たら終了 */
	if (13==(*ccc)) 	/* 改行コードは OD OA */
	{
		*line_terminate_end_flag = 1;
	}
	*ddd = 0;
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = m1_SYOU_RYAKU_TI; /*(値の読みこみに失敗した場合省略値とする。)*/ }
	else														{	*number = atoi(buffer); }
	return (ccc);
/*error*/
ne222:
	return ((char *)NULL);/*(エラー。)*/
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
	//
	I_CODE_04_OBJ_DRAW_ON_OFF,/*(r36新設)*/
	I_CODE_05_RESURVED, 	// [現在未使用]
	I_CODE_06_RESURVED, 	// [現在未使用]
	I_CODE_07_RESURVED, 	// [現在未使用]
// 画面制御
	I_CODE_08_DRAW_TEXT,
	I_CODE_09_DRAW_BG,
	I_CODE_0a_DRAW_PANEL,
	I_CODE_0b_DRAW_SKIP,
	//
	I_CODE_0c_DRAW_WAIT,	// (vsync) wait待ち。
	I_CODE_0d_RESURVED, 	// [現在未使用]
	I_CODE_0e_RESURVED, 	// [現在未使用]
	I_CODE_0f_RESURVED, 	// [現在未使用]
// テキスト関連
	I_CODE_10_TEXT,
	I_CODE_11_HOLD,
	I_CODE_12_FONT_COLOR,
	I_CODE_13_FONT_SPEED,
// 背景/BGM
	I_CODE_14_BG_LOAD,
	I_CODE_15_BG_FADE,
	I_CODE_16_BGM,
	I_CODE_17_BGM_VOLUME,
// ボス制御
	I_CODE_18_BOSS_LOAD,
	I_CODE_19_BOSS_TERMINATE,/*(r34新設)*/
	I_CODE_1a_BOSS_START,
	I_CODE_1b_BOSS_RESULT,/*(r35新設)*/
// システム
	I_CODE_1c_STAGE_LOAD,/*(r36新設)*/
	I_CODE_1d_DUMMY,	// [現在未使用]
	I_CODE_1e_DUMMY,	// [現在未使用]
	I_CODE_1f_DUMMY,	// [現在未使用]
//
	I_CODE_99_MAX/*(2^n の必要がある。)*/
};


typedef struct _kaiwa_youso_tag_
{
	s16 i_code; 		/* Interprited Code. / Intermediate Language, Command 中間言語 / 終わったかどうか */
	s16 para1;
	s16 para2;
	s16 para3;
	struct _kaiwa_youso_tag_ *next;
	s32 str_num0;// char para0[(7*32)/*200*/];/* 32の倍数 */
} KAIWA_YOUSO;/* == 16bytes */ /* == 256bytes */
//[16==4*4]
//[32==8*4]

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
	文字列スタック
	-------------------------------------------------------
	64しかないけど、巧く動いてる。
	(旧シナリオ)霊夢A end(s61.txt) で 51 しか消費しない。(r34)現在は64以上消費。
	多分、模倣風の標準シナリオならば、 64 で十分かと思われる。
	が、安全の為、倍の 128 にしとく。
	万一オーバーしても、エラー"string stack over flow.(%d)\n"で
	終了するから問題無い。(メッセージから原因も判るし)
---------------------------------------------------------*/

//#define MAX_STR_STACK 	(64)
#define MAX_STR_STACK		(128)
static int str_stack_position;
static char str_stack[MAX_STR_STACK][(7*32)/*200*/];/* 32の倍数 */

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
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_kaiwa_youso->i_code = I_CODE_14_BG_LOAD; strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;	/*push_str(c_p0);*/ 	}	/* 背景ファイルの変更 */
			else	if (0==tiny_strcmp(command, "BG_FADE")) 	{	new_kaiwa_youso->i_code = I_CODE_15_BG_FADE;		}	/* 背景のフェードアウト */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_kaiwa_youso->i_code = I_CODE_16_BGM;			}	/* BGM変更 */
			else	if (0==tiny_strcmp(command, "BGM_VOLUME"))	{	new_kaiwa_youso->i_code = I_CODE_17_BGM_VOLUME; 	}	/* BGM 音量 変更 */
		if (0==tiny_strcmp(command, 			"BOSS"))
		{
					if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_18_BOSS_LOAD;		}	/* ボス読み込み */
			else	if (0==tiny_strcmp(c_p0,	"term"))		{	new_kaiwa_youso->i_code = I_CODE_19_BOSS_TERMINATE; }	/* result後にゲーム強制終了。 */
			#if (1)
			else	if (0==tiny_strcmp(c_p0,	"start"))		{	new_kaiwa_youso->i_code = I_CODE_1a_BOSS_START; 	}	/* ボス攻撃開始。 */
			else	if (0==tiny_strcmp(c_p0,	"result"))		{	new_kaiwa_youso->i_code = I_CODE_1b_BOSS_RESULT;	}	/* (r35)現在仕様の都合上要る。これが無いとリザルト画面が巧く出ない。 */
			#endif
		}
	}
	else
	if ('D'==(*command))
	{
		if (0==tiny_strcmp(command, 			"DRAW"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_kaiwa_youso->i_code = I_CODE_0a_DRAW_PANEL; 	}	/* スコアパネルの表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_08_DRAW_TEXT;		}	/* テキスト画面の表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_09_DRAW_BG;		}	/* 背景の表示/非表示 */
			else	if (0==tiny_strcmp(c_p0,	"skip"))		{	new_kaiwa_youso->i_code = I_CODE_0b_DRAW_SKIP;		}	/* */
			else	if (0==tiny_strcmp(c_p0,	"color"))		{	new_kaiwa_youso->i_code = I_CODE_12_FONT_COLOR; 	}	/* 文字の色 */
			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_13_FONT_SPEED; 	}	/* 文字の表示速度 */
			else	if (0==tiny_strcmp(c_p0,	"wait"))		{	new_kaiwa_youso->i_code = I_CODE_0c_DRAW_WAIT;		}	/* vsync ウェイト待ち */
		}
	}
	else
	if ('O'==(*command))
	{
//		psp_fatal_error( (char*)
//		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
//			"kaiwa: s_command。" "\\n"
//			"値: %d。 test ok。", str_stack_position);
					if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_kaiwa_youso->i_code = I_CODE_02_OBJ_LOAD;		strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* 汎用オブジェのロード / 再読み込み / 汎用オブジェに画像を読みこむ。 */
					if (0==tiny_strcmp(command, "OBJ_TEXT"))
																{	new_kaiwa_youso->i_code = I_CODE_1c_STAGE_LOAD; 	strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* 雑魚データー読みこみ */
		if (0==tiny_strcmp(command, 			"OBJ"))
		{
					if (0==tiny_strcmp(c_p0,	"look"))		{	new_kaiwa_youso->i_code = I_CODE_00_OBJ_LOOK;			}	/* オブジェクト注目コマンド */
			else	if (0==tiny_strcmp(c_p0,	"xy"))			{	new_kaiwa_youso->i_code = I_CODE_01_OBJ_XY; 			}	/* オブジェクトの原点設定 */
			else	if (0==tiny_strcmp(c_p0,	"wait"))		{	new_kaiwa_youso->i_code = I_CODE_03_OBJ_WAIT;			}	/* オブジェクト移動完了まで待つ。 */
			else	if (0==tiny_strcmp(c_p0,	"draw"))		{	new_kaiwa_youso->i_code = I_CODE_04_OBJ_DRAW_ON_OFF;	}	/* オブジェクトの表示/非表示 */
//			else	if (0==tiny_strcmp(c_p0,	"speed"))		{	new_kaiwa_youso->i_code = I_CODE_OBJ_SPEED; 			}	/* オブジェクトの速度設定 */
		}
	}
	else
//	if ('T'==(*command))
//	{
					if (0==tiny_strcmp(command, "TEXT"))		{	new_kaiwa_youso->i_code = I_CODE_10_TEXT;			strcpy( (&str_stack[str_stack_position][0]), c_p0);str_stack_position++;			}	/* メッセージウィンドウへの書き込み */
//	}
	else
//	if ('H'==(*command))
//	{
					if (0==tiny_strcmp(command, "HOLD"))		{	new_kaiwa_youso->i_code = I_CODE_11_HOLD;				}	/* クリック待ち */
//	}

//	if ('C'==(*command))
//	{	if (0==tiny_strcmp(command, 			"CURSOR"))
//		{
//					if (0==tiny_strcmp(c_p0,	"home"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_HOME;				}	/* カーソルの初期化 */
//			else	if (0==tiny_strcmp(c_p0,	"load"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_POP;				}	/* 記憶したカーソル位置の呼び出し */
//			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_kaiwa_youso->i_code = I_CODE_CUR_PUSH;				}	/* カーソル位置の記憶 */
//			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_kaiwa_youso->i_code = I_CODE_11_HOLD;				}	/* クリック待ち */
//			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_kaiwa_youso->i_code = I_CODE_10_TEXT_WRITE_TEXT;	}	/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
//			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_kaiwa_youso->i_code = I_CODE_BG_WRITE_TEXT; 		}	/* BGウィンドウに漢字の文字を描く様に設定する。 */
//		}
//	}
	if ((MAX_STR_STACK-1) < str_stack_position)
	{	/* 文字列スタック、オーバーフローで強制終了。 */
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"kaiwa: 文字列スタック領域が足りません。" "\\n"
			"値: %d。 最大設定値を増やして下さい。", str_stack_position);
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
			ch = load_my_file_get_str(ch, c_p0, &end_arg);/*, ','*/ 				/* 文字列コマンド(オペランド)読み込み */
			if (NULL==ch)									{	GOTO_ERR_WARN;	}	/* 読めなければエラー */
		}
		{
			int c_pn[PARAM_99_MAX];/*6*/
			{	int kk;
				for (kk=0; kk<PARAM_99_MAX; kk++)/*(6)*/
				{
					c_pn[kk] = m1_SYOU_RYAKU_TI;	/* 無指定の場合, 引数省略値に設定する。 */
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
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"kaiwa: 会話設定ファイルの、" "\\n"
			"%d行目は意味が判りません。" "\\n"
			"%s",
			debug_number_of_read_line,
			my_file_common_name );
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	//
	my_file_fclose(/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		psp_fatal_error(/*ERR_WARN,*/ (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"kaiwa: これは会話設定ファイル" "\\n"
			"ではありません。" "\\n",
			"%s", my_file_common_name);
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

//static int cursor_x_chached;			/* カーソル位置 保存用 */
//static int cursor_y_chached;			/* カーソル位置 保存用 */



/*---------------------------------------------------------
	シナリオ会話システム、内部処理
---------------------------------------------------------*/
static int re_draw_BG_count_flag;
global void kaiwa_system_set_re_draw(void)
{
	re_draw_BG_count_flag = (1);/* 描画してね */
}

/* 内部コマンドの受け渡し形式。 */
#define MY_ICODE_FUNC(name) 		static void name(KAIWA_YOUSO *ssc)
#define MY_ICODE_GET_FUNC(name) 	static int name(void)
#define MY_ICODE_CALL(name) 		name(ssc)

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
	re_draw_BG_count_flag = (1);/* 描画してね */
//	pop_str();
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; 	//	filename			= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
//	int used_alpha_flag;	used_alpha_flag 	= ssc->para1;//アルファ付き読みこみ？？？？
//	int fade_command;		fade_command		= ssc->para2;
//	int set_alpha_speed;//	set_alpha_speed 	= ssc->para3;
	/*---------------------------------------------------------
		指定ファイル名の画像を読みこみSDLサーフェイスに設定する。
	---------------------------------------------------------*/
	char file_name[128];
	strcpy(file_name, DIRECTRY_NAME_DATA_STR "/" );
	strcat(file_name, /*filename*/(&str_stack[str_stack_position][0]));str_stack_position++;/*pop_string();(文字列スタックを消化する)*/
//
	SDL_Surface *s1;/*temporaly*/
	SDL_Surface *s2;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(file_name);/*ここでロードすると確実に処理落ちするよ*/
	if (NULL == s1)
	{
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"kaiwa: 画像ファイルがありません。" "\\n"
			"%s", file_name );
//		return (NULL);
	}
	//
//	if (NULL != s2)
//	{
//		SDL_FreeSurface(s2);
		s2 = NULL;
//	}
//
	#if 0
	if (	(m1_SYOU_RYAKU_TI == used_alpha_flag) /* シナリオ会話 中で省略した場合(デフォルト) */
		||
			(0==used_alpha_flag)	/* 明示した場合 */
	)
	#endif
	{
		/* アルファ使わない */
		s2 = SDL_DisplayFormat(s1);/*サーフェスを表示フォーマットに変換する。*/
	}
	#if (1)/*(作業用領域の解放)*/
	SDL_FreeSurface(s1);
	s1 = NULL;
	#endif
	// 背景を SDL_01_BACK_SCREEN に描画する。
	PSPL_UpperBlit(s2, NULL, cb.sdl_screen[SDL_01_BACK_SCREEN], NULL);
	SDL_FreeSurface(s2);
	s2	= NULL;
}
		#if /*(うまくいかない)*/(0)/* 現状SDL背景がある、 story で要る。 */
		SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);	/* カラーキー(抜き色、透明色)は黒 */
		#endif
	#if 0/*(メモ)*/
	else
	{
		/* アルファ使う */
		bg_story_window_surface = SDL_DisplayFormatAlpha(s1);/*サーフェスを表示フォーマットに変換する。*/
	}
	#endif
	#if 0/*(SDL変換に失敗==pspのメモリが足りない)*/
	if (NULL == bg_story_window_surface)
	{
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"kaiwa: 画像変換するメモリが" "\\n"
			"足りません。" "\\n"
			"%s", file_name );
//		return (NULL);
	}
	#endif
//	bg_story_window_surface =	(bg_st ory_window_surface);
	//{
	//	bg_st ory_window_surface_image	= (u16 *)bg_st ory_window_surface->pixels;
	//	bg_st ory_window_surface_pitch	= bg_st ory_window_surface->pitch;
	//	bg_st ory_window_surface_width	= bg_st ory_window_surface->w;
	//}
	#if 0/* 現状、要らない？。 */
	SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	#endif


//	/*fade_in_command_start*/

	static int draw_bg; 							/* 背景表示:1 / 非表示:0フラグ */
	static int bg_alpha;							/* 背景α値用 */
	static int i_code_fade_bg_bg_alpha_speed;		/* 背景α値用 */
	static int i_code_fade_bg_exec_fade;			/* do fade */

MY_ICODE_FUNC(i_code_fade_bg)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; 		filename		= ssc->para0;
//	int alpha;				alpha			= ssc->para1;
//	int fade_command;	//	fade_command	= ssc->para2;
//	if ((1)==fade_command)/*1==fade_in_command*/

	re_draw_BG_count_flag = (1);/* 描画してね */
//
	//
	bg_alpha = 0;
	int i_code_fade_bg_set_alpha_speed;//	set_alpha_speed = ssc->para3;
//	if (m1_SYOU_RYAKU_TI == set_alpha_speed)/*パラメーター省略時*/
	{
		i_code_fade_bg_set_alpha_speed = (5);
	}
	i_code_fade_bg_bg_alpha_speed = i_code_fade_bg_set_alpha_speed;
	draw_bg 					= (1);/*on*/	/*check_draw_bg();*/
	i_code_fade_bg_exec_fade 	= (1);/*on*/	/* 処理中 */

//	return /*(0)*/; 	/* 処理中 */
}
static void kaiwa_system_vbl_do_BG_fade(void)
{
	if (0!=i_code_fade_bg_exec_fade)
	{
		re_draw_BG_count_flag = (1);/* 描画してね */
	//	psp_clear_screen();
		bg_alpha += i_code_fade_bg_bg_alpha_speed;/*(5)*/
		if (250 < bg_alpha)
		{
			bg_alpha = 255;
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
	unsigned int nnn;
	for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
	{
		int hide_mode;	/*(隠れるモード)*/
		int offset_x;
		int offset_y;
		KAIWA_OBJ *my_std_obj;
		my_std_obj = &kaiwa_sprite[((nnn)&(KAIWA_OBJ_99_MAX-1))]; /* 汎用オブジェ */
		//
		my_std_obj->move_flag = KAIWA_OBJ_MOVE_FLAG_01_MOVE_START;

		if (KAIWA_OBJ_99_MAX>(obj_look_up))/* 通常モード */
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
		// /* 色 */
			my_std_obj->color32 = (0xffffffff);/*(不透明 [255/alpha255])*/
		}
		else/* 注目オブジェ以外ならオフセットあり */
		{	/*(隠れるモード ON)*/
			offset_x	= my_std_obj->offset_x256;
			offset_y	= my_std_obj->offset_y256;
		// /* 色 */
			my_std_obj->color32 = (0x60ffffff);/*(半透明 [180/alpha255])*/
		}
		/* 合成位置[A]として、あらかじめ移動完了位置を算出しておく */
	//	my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-((my_std_obj->w)>>1))<<8);	/* 移動完了座標 */
	//	my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-((my_std_obj->h)>>1))<<8);	/* 移動完了座標 */
	//	my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-((1<<(my_std_obj->w_bit))>>1))<<8);	/* 移動完了座標 */
	//	my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-((1<<(my_std_obj->h_bit))>>1))<<8);	/* 移動完了座標 */
		my_std_obj->target_x256 = my_std_obj->origin_x256 + offset_x + ((-(( (my_std_obj->width_2n))>>1))<<8);		/* 移動完了座標 */
		my_std_obj->target_y256 = my_std_obj->origin_y256 + offset_y + ((-(( (my_std_obj->height_2n))>>1))<<8); 	/* 移動完了座標 */
		//	my_std_obj->target_x256 	= (my_std_obj->target_x256);//移動完了予定位置
		//	my_std_obj->target_y256 	= (my_std_obj->target_y256);//移動完了予定位置

		/* 現在位置を、合成位置[B]として保存。 */
		{
			my_std_obj->alt_x256		= (my_std_obj->cx256);//現在位置
			my_std_obj->alt_y256		= (my_std_obj->cy256);//現在位置
		}
		/*(合成到達割合の設定)*/
		my_std_obj->toutatu_wariai256 = (0);			/* 初速度 (x32) */
	}
}

/*---------------------------------------------------------
OBJ xy,0,128,144
OBJ xy,[オブジェクト指定番号],[原点x座標],[原点y座標]
	[ "i_code_" シナリオ会話システム内部コマンド]
	I_CODE_01_OBJ_XY オブジェクトの原点座標の設定。
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_xy)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *color32_str;		color32_str = ssc->para0;
	int num;				num 		= ssc->para1;/*(オブジェクト指定番号)*/
	int origin_x;			origin_x	= ssc->para2;/* 原点x座標の設定 */
	int origin_y;			origin_y	= ssc->para3;/* 原点y座標の設定 */
//
	KAIWA_OBJ *my_std_obj;
	my_std_obj = &kaiwa_sprite[((num)&(KAIWA_OBJ_99_MAX-1))]; /* 汎用オブジェ */
	//
	my_std_obj->origin_x256 = ((origin_x)<<8);		/* 原点座標の設定 */
	my_std_obj->origin_y256 = ((origin_y)<<8);		/* 原点座標の設定 */
	obj_move_refresh();
}

/*---------------------------------------------------------
OBJ look,0
OBJ look,[オブジェクト指定番号]
	[ "i_code_" シナリオ会話システム内部コマンド]
	I_CODE_00_OBJ_LOOK 注目オブジェの番号を設定。
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_look)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *color32_str;		color32_str = ssc->para0;
/*	int mode;	*/			obj_look_up 		= ssc->para1;/*(オブジェクト指定番号)*/
	obj_move_refresh();
}


/*---------------------------------------------------------
OBJ_LOAD tachie/reimu2.png,0
OBJ_LOAD [ファイル名(dataからの相対パス)],[オブジェクト指定番号]
	[ "i_code_" シナリオ会話システム内部コマンド]
	オブジェクトに画像を読み込む
	i_code_obj_load_gazo
	[ "i_code_" シナリオ会話システム内部コマンド]
	読み込み済みのオブジェクトの画像を再読み込みして、
	位置をそのまま継続し画像は入れ替える。
	i_code_swap_image_sprite
---------------------------------------------------------*/

MY_ICODE_FUNC(i_code_obj_load_gazo)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int sitei_number;	sitei_number	= ssc->para1;/*(オブジェクト指定番号)*/
//
	sitei_number &= ((KAIWA_OBJ_99_MAX-1));
	/*(Guの場合の、読みこみ処理)*/
	{
	//	char my_file_common_name[256/*128*/];
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/" );
		strcat(my_file_common_name, (&str_stack[str_stack_position][0]));
		//
		strcpy(&my_resource[TEX_09_TACHIE_L+sitei_number].file_name[0], (&my_file_common_name[0]) );
		//
		str_stack_position++;/*pop_string();(文字列スタックを消化する)*/
	}
	TGameTexture_Load_Surface(TEX_09_TACHIE_L+sitei_number);
	#if 0
	/*(要らない？) (別で設定すれば要らない)  */
	kaiwa_obj_set2n(sitei_number);
	#endif
}



/*---------------------------------------------------------
OBJ draw,0,1
OBJ draw,[オブジェクト指定番号],[表示:1/非表示:0の切り替え。]
	[ "i_code_" シナリオ会話システム内部コマンド]
	オブジェクトの表示/非表示の切り替え。
---------------------------------------------------------*/
MY_ICODE_FUNC(i_code_obj_draw)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
	int sitei_number;	sitei_number	= ssc->para1;/*(オブジェクト指定番号)*/
	int on_off; 		on_off			= ssc->para2;/*(表示/非表示の切り替え。)*/
//	int yy; 			yy				= ssc->para3;
//	kaiwa_obj_mono_draw_on_off(sitei_number, on_off);
	kaiwa_sprite[(sitei_number&(2-1))].draw_flag = (on_off);	/* 描画on/off。 */
}
#if (1==USE_AFTER_LOAD_STAGE)
/*---------------------------------------------------------

---------------------------------------------------------*/
extern void load_stage_data(void);
extern void init_stage_start_time(void);
MY_ICODE_FUNC(i_code_load_stage)
{
	/* シナリオ会話システム内部コマンドのパラメーターを受け取る。 */
//	char *filename; //	filename		= ssc->para0;
//	strcpy(filename, (&str_stack[str_stack_position][0]));str_stack_position++;
//	int sitei_number;	sitei_number	= ssc->para1;
//
//	sitei_number &= ((KAIWA_OBJ_99_MAX-1));
	/*(Guの場合の、読みこみ処理)*/
	{
	//	char temp_file_name[256/*128*/];
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/");
		strcat(my_file_common_name, (&str_stack[str_stack_position][0]));
		//
	//	strcpy(&my_resource[TEX_09_TACHIE_L+sitei_number].file_name[0], (&my_file_common_name[0]) );
		//
		str_stack_position++;/*pop_string();(文字列スタックを消化する)*/
	}
	#if (0)/*(デバッグ)*/
	{	/* 読み込みテスト */
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"kaiwa: %d 面の道中敵設定" "\\n"
			"ファイルを読みます。" "\\n"
			"%s",
			/*load_stage_number*/cg.game_now_stage,
			my_file_common_name );
	}
	#endif
//	TGameTexture_Load_Surface(TEX_09_TACHIE_L+sitei_number);
	{
		/* Load next stage */
		load_stage_data();//	ステージ読み込み。
		init_stage_start_time();	// ロード中は処理落ちしているので、ロード後に時間を再作成する。
	}
}
#endif
/*---------------------------------------------------------
	実行停止解除用件
---------------------------------------------------------*/
enum /* マルチタスク機能のタスクスイッチ */
{																		// 意味。
	TASK_SWITCH_00_EXECUTE							= 0x00,/*(0<<0)*/	// 会話スクリプト処理実行中。

	TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT	= 0x01,/*(1<<3)*/	// スクリプト開始時に特別に、キー入力待ちを行うその為、スクリプト停止中。
	TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE		= 0x02,/*(1<<2)*/	// キー入力待ちでスクリプト処理停止中。
	TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT			= 0x04,/*(1<<2)*/	// 指定ウェイト待ちでスクリプト処理停止中。
	//
	TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING 		= 0x10,/*(1<<1)*/	// テキスト描画中でスクリプト停止中。
	TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT			= 0x20,/*(1<<0)*/	// オブジェクト移動位置まで移動するのを待ち中。(オブジェクト移動完了状態までスクリプト停止して待つ)
};
// HOLD_REASON == (会話スクリプト)停止理由。
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
	for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
	{
		KAIWA_OBJ *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* 汎用オブジェ */
		if (KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag)
		{
			check_flag = (1);/* 移動中 */
		}
	}
	/* 全OBJが停止している場合のみ、解除。 */
	if (0==check_flag)/* 全OBJ停止している。 */
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT;/* 解除 */
	}
}


/*---------------------------------------------------------
	会話通常時のパッドチェック
	-------------------------------------------------------
	制限時間は、本家と同じ10[秒]とする。
	-------------------------------------------------------
	最後に漢字画面を全行消す。
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
	vsync待ち。
	-------------------------------------------------------
	最後に漢字画面を全行消す。
---------------------------------------------------------*/
static int kaiwa_system_wait_mode;

static void kaijyo_vsync_wait(void)
{
	kaiwa_system_wait_mode--;
	if (0 >= kaiwa_system_wait_mode)
	{
		kaiwa_system_hold_mode &= ~TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT;/* 解除 */
		#if 1	/* 最後のみ行う */
		kanji_window_all_clear();				/* 漢字画面を全行消す。漢字カーソルをホームポジションへ移動。 */
		#endif
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
			kaiwa_system_hold_mode &= ~TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT;/* 解除 */
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
		kaiwa_system_hold_mode &= ~TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING;/* 解除 */
	}
}


/*---------------------------------------------------------
	シナリオ会話システム、オブジェクトの移動
---------------------------------------------------------*/

static void kaiwa_system_vbl_sprite_move(void)
{
	int nnn;
	for (nnn=0; nnn<KAIWA_OBJ_99_MAX; nnn++)
	{
		KAIWA_OBJ *my_std_obj;
		my_std_obj = &kaiwa_sprite[nnn]; /* 汎用オブジェ */
		if (KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE != my_std_obj->move_flag )
		{
			my_std_obj->toutatu_wariai256 += (6);/*(8 立ち絵、速度)*/
			/* 移動完了座標に等しいかはみ出したら、完了とする。 */
			if (my_std_obj->toutatu_wariai256 >= t256(1))
			{
				my_std_obj->move_flag	= KAIWA_OBJ_MOVE_FLAG_00_MOVE_COMPLETE;/*移動完了*/
				//return /*(1)*/;/*移動完了*/
			}
			//
			int move_rate256;
			move_rate256 = my_std_obj->toutatu_wariai256;
			move_rate256 = psp_min(move_rate256, 256);
			move_rate256 = psp_max(move_rate256, 0);
			#if (1)/* 非等分値の計算 */
			{
				u32 i_rate65536;
				i_rate65536 = (move_rate256 << 8);
			//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
				i_rate65536 = (int)(vfpu_ease_in_out65536(i_rate65536));	/* t65536の等分カウンタ値を使い、非等分になるよう値を修正する。
				非等分の方式は「ease_in_out」これは、始めと最後がゆっくりで間が速いという配分方式。 */
				move_rate256 = (i_rate65536 >> 8);
			}
			#endif
			//------------------
			#if (1)/*(到達割合を考慮して、合成する。)*/
			REG_00_SRC_X	= ((my_std_obj->alt_x256)); 	/*(合成位置[A]t256()形式)*/
			REG_01_SRC_Y	= ((my_std_obj->alt_y256)); 	/*(合成位置[A]t256()形式)*/
			REG_02_DEST_X	= ((my_std_obj->target_x256));	/*(合成位置[B]t256()形式)*/
			REG_03_DEST_Y	= ((my_std_obj->target_y256));	/*(合成位置[B]t256()形式)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(合成割合t256()形式)*/
			multiprex_rate_vector();/*(破壊レジスタ多いので注意)*/
			#endif
			// この時点で、
			// REG_02_DEST_X: 合成値
			// REG_03_DEST_Y: 合成値
			//-----------------------
			my_std_obj->cx256 = (REG_02_DEST_X);
			my_std_obj->cy256 = (REG_03_DEST_Y);
		}
	}
}


/*---------------------------------------------------------
	シナリオ会話システム、内部実行処理
	シナリオ会話システム コマンド解釈(実行)系
---------------------------------------------------------*/
/* 会話システムから呼ばれるもの */
#if 1
extern void called_from_kaiwa_system_boss_load(int boss_number);
#endif

static /*int*/void kaiwa_system_execute(void)
{
	KAIWA_YOUSO *ssc;
	ssc = kaiwa_youso_list_scan;/* 実行開始位置 */
//
	#if (1==USE_AFTER_LOAD_STAGE)
		/*(なし)*/
	#else
		#define case_I_CODE_1c_STAGE_LOAD I_CODE_break
	#endif
	void *aaa[I_CODE_99_MAX] =
	{
	// OBJ関連
		&&case_I_CODE_00_OBJ_LOOK,
		&&case_I_CODE_01_OBJ_XY,
		&&case_I_CODE_02_OBJ_LOAD,
		&&case_I_CODE_03_OBJ_WAIT,
		//
		&&case_I_CODE_04_OBJ_DRAW_ON_OFF,/*(r36新設)*/
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
	// 画面制御
		&&case_I_CODE_08_DRAW_TEXT,
		&&case_I_CODE_09_DRAW_BG,
		&&case_I_CODE_0a_DRAW_PANEL,
		&&case_I_CODE_0b_DRAW_SKIP,
		//
		&&case_I_CODE_0c_DRAW_WAIT,/*(r36新設)*/
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
	// テキスト関連
		&&case_I_CODE_10_TEXT,
		&&case_I_CODE_11_HOLD,
		&&case_I_CODE_12_FONT_COLOR,
		&&case_I_CODE_13_FONT_SPEED,
	// 背景/BGM
		&&case_I_CODE_14_BG_LOAD,
		&&case_I_CODE_15_BG_FADE,
		&&case_I_CODE_16_BGM,
		&&case_I_CODE_17_BGM_VOLUME,
	// ボス制御
		&&case_I_CODE_18_BOSS_LOAD,
		&&case_I_CODE_19_BOSS_TERMINATE,/*(r34新設)*/
		&&I_CODE_break, /*(必要ダミー)*/	// &&case_I_CODE_1a_BOSS_START,
		&&I_CODE_break, /*(必要ダミー)*/	// &&case_I_CODE_1b_BOSS_RESULT,/*(r35新設)*/
	//
		&&case_I_CODE_1c_STAGE_LOAD,/*(r36新設)*/
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
		&&I_CODE_break, 	// 必要ないが安全の為
	//
	//	I_CODE_99_MAX/*(2^n)*/	// 必要ないが安全の為
	};
	goto *aaa[(ssc->i_code)&0x1f];/*(ccc)&(I_CODE_99_MAX-1)*/
	{
	case_I_CODE_00_OBJ_LOOK:			MY_ICODE_CALL(i_code_obj_look);			goto I_CODE_break;	/* オブジェクト注目コマンド */
	case_I_CODE_01_OBJ_XY:				MY_ICODE_CALL(i_code_obj_xy); 			goto I_CODE_break;	/*(原点設定のみ)*/
	case_I_CODE_02_OBJ_LOAD:			MY_ICODE_CALL(i_code_obj_load_gazo);	goto I_CODE_break;	/*(読みこみのみ) 汎用絵  立ち絵L	立ち絵R */
	case_I_CODE_04_OBJ_DRAW_ON_OFF: 	MY_ICODE_CALL(i_code_obj_draw);			goto I_CODE_break;	/*(表示の 0:off / 1:on ) 汎用絵  立ち絵L	立ち絵R */
	//
	case_I_CODE_03_OBJ_WAIT:			kaiwa_system_hold_mode |= TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT; /* 停止 */	goto I_CODE_break;
	//
	case_I_CODE_08_DRAW_TEXT:	//廃止	cg.dr aw_flag_kaiwa_screen	= ssc->para1;
		#if (0)/*(最近の(gcc4.3.5)はたぶん同じ。gcc4.0.2ぐらい古いと違う。)*/
		if (ssc->para1&1)	{	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [黒/会話用背景]せりふ背景on */}
		else				{	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */}
		#else
		{	ml_font[(0)].haikei 		= (ssc->para1&1)?(ML_HAIKEI_03_MESSAGE)/* [黒/会話用背景]せりふ背景on */:(ML_HAIKEI_m1_OFF);/* せりふ背景off */}
		#endif
		goto I_CODE_break;
	case_I_CODE_09_DRAW_BG: 		draw_bg 					= ssc->para1;		goto I_CODE_break;/* view bg, 0:on, 1:off */
	case_I_CODE_0a_DRAW_PANEL:		cg.side_panel_draw_flag 	= ssc->para1;		goto I_CODE_break;
	case_I_CODE_0b_DRAW_SKIP:		kaiwa_system_skip_mode		= ssc->para1;		goto I_CODE_break;
	case_I_CODE_0c_DRAW_WAIT:		kaiwa_system_wait_mode		= ssc->para1;		kaiwa_system_hold_mode |= TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT;						goto I_CODE_break;
	//
	case_I_CODE_10_TEXT:
				strcpy(i_code_text_para0, (&str_stack[str_stack_position][0]));str_stack_position++;/*pop_string();(文字列スタックを消化する)*/
			//	strcpy(i_code_text_para0, ssc->para0);
					kaiwa_system_hold_mode |= TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING;					goto I_CODE_break;
	case_I_CODE_11_HOLD:			limit_timer 				= byou60(10);/*(10[秒]制限時間。本家互換)*/ 	kaiwa_system_hold_mode |= TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE; goto I_CODE_break;
	case_I_CODE_12_FONT_COLOR:		i_code_text_color			= ssc->para1;						goto I_CODE_break;
	case_I_CODE_13_FONT_SPEED:		i_code_text_speed_wait		= ssc->para1;						goto I_CODE_break;
	//
	case_I_CODE_14_BG_LOAD: 		MY_ICODE_CALL(i_code_load_bg);									goto I_CODE_break;
	case_I_CODE_15_BG_FADE: 		MY_ICODE_CALL(i_code_fade_bg);									goto I_CODE_break;
	case_I_CODE_16_BGM: 			play_music_num( (ssc->para1) ); 								goto I_CODE_break;
	case_I_CODE_17_BGM_VOLUME:		set_music_volume( (ssc->para1) );								goto I_CODE_break;
	case_I_CODE_18_BOSS_LOAD:		called_from_kaiwa_system_boss_load( (ssc->para1) ); 			goto I_CODE_break;/*boss_number*/
	case_I_CODE_19_BOSS_TERMINATE:
									#if (1==USE_r36_SCENE_FLAG)
	/* off / 道中コマンド追加読み込み処理を停止する。 */
//	cg.state_flag		&= (~SCENE_NUMBER_MASK);	/*(シーンを消す)*/
	cg.state_flag		&= (0xffff00ffu);	/*(シーンを消す)*/
	cg.state_flag		|= (0x00008000u); //プレイヤーループを抜ける処理(とりあえず????)
									#else
									cg.state_flag |= STATE_FLAG_13_GAME_TERMINATE;
									#endif
																									goto I_CODE_break;
	#if (1==USE_AFTER_LOAD_STAGE)
	case_I_CODE_1c_STAGE_LOAD:		MY_ICODE_CALL(i_code_load_stage); 								goto I_CODE_break;	/*(読みこみのみ) 汎用絵  立ち絵L	立ち絵R */
	#endif
		/*	called_from_kaiwa_system_boss_start();*/
		/*	cg.state_flag |= STATE_FLAG_0x0800_IS_BOSS; */
		/* ボス戦闘前の会話終了を設定 */
//	case_I_CODE_1a_BOSS_START:		/*ダミー*/; 													goto I_CODE_break;
//	case_I_CODE_1b_BOSS_RESULT: 	/*ダミー*/; 													goto I_CODE_break;
	//
//	case_I_CODE_OBJ_SPEED:			MY_ICODE_CALL(i_code_set_sprite_speed);							goto I_CODE_break;
	// [テキストのカーソル制御]
//	case I_CODE_CUR_POP:			cursor_x = cursor_x_chached;	cursor_y = cursor_y_chached;		break;	/* カーソル位置、復元 */
//	case I_CODE_CUR_PUSH:			cursor_x_chached = cursor_x;	cursor_y_chached = cursor_y;		break;	/* カーソル位置、記憶 */
//	case I_CODE_CUR_HOME:			kanji_cursor_move_home_position();						break;	/* 漢字カーソルをホームポジションへ移動 */		/* カーソルの初期化 */
//	case I_CODE_BG_WRITE_TEXT:		set_write_text(1);					break;	/* BGウィンドウに漢字の文字を描く様に設定する。 */
//	case I_CODE_10_TEXT_WRITE_TEXT: set_write_text(0);					break;	/* メッセージウィンドウに漢字の文字を描く様に設定する。 */
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



/*---------------------------------------------------------
	会話システム SDL_BG描画
---------------------------------------------------------*/

global void kaiwa_system_SDL_BG_draw(void)
{
	/* SDL_BG描画、遅すぎる。(Gu化するとここは無くなる予定) */
	if (re_draw_BG_count_flag)
	{
		re_draw_BG_count_flag--;
		{
			#define USE_SDL_DRAW (1)
			#if (1==USE_SDL_DRAW)
			/* 会話システム内のオブジェクト描画処理 */
			#if /*???????*/0/*Gu化完了したら要らなくなる*/
			{
				psp_clear_screen(); /* [PAUSE] 復帰時にSDL画面を消す。 */
			}
			#endif
			/* ----- 描画系 ----- */
			/* 1. まず背景を描く */
			if (draw_bg)
			{
			//	#if 1/*(うまくいかない)*/
			//	if (255 > bg_alpha)
			//	{
			//		SDL_SetAlpha(bg_st ory_window_surface, SDL_SRCALPHA, bg_alpha);
			//	}
			//	#endif
			//	PSPL_UpperBlit(bg_st ory_window_surface, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
				psp_pop_screen();
			}
			/* 2. 次に立ち絵を描く */
			#endif /* (1==USE_SDL_DRAW) */
		}
	}
}


/*---------------------------------------------------------
	リセット
---------------------------------------------------------*/

//static extern int my_string_offset;
/*static*/ extern void clear_my_string_offset(void);
static void aaa_kaiwa_system_reset(void)
{
	draw_bg = (0);/* bg 表示 off */
	//
	kanji_cursor_move_home_position();		/* 漢字カーソルをホームポジションへ移動 */
	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	会話モード終了処理
---------------------------------------------------------*/

/*static*/global void do_kaiwa_system_terminate(void)
{
//	bg_alpha = 0;
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* せりふ背景off */
//廃止	cg.dr aw_flag_kaiwa_screen	= (0);	/* せりふウィンドウ表示フラグ off */
	load_kaiwa_youso_free();	/* 前回のシナリオがメモリにあればすべて開放。 */
	aaa_kaiwa_system_reset();
//	draw_bg = 0;/* (描画と分離できない)バグ修正 */
}


/*---------------------------------------------------------
	ゲームコア用、会話モード終了状態へ遷移
---------------------------------------------------------*/
static void kaiwa_system_terminate_game_core(void)
{
	#if (1==USE_r36_SCENE_FLAG)
	NEXT_SCENE;
	#else
	cg.state_flag &= (~(STATE_FLAG_0x0200_IS_KAIWA_MODE));/*off*/
	cg.state_flag |= STATE_FLAG_0x0300_END_KAIWA_MODE;
	/* ボス戦闘前後イベント pd_bo ssmode = B08_START; = B09_STAGE_LOAD; */
	#endif
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
		if (kaiwa_system_hold_mode & TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT)
		{
			kaijyo_obj_all_stop();			/* 全OBJが停止していたら、解除。 */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_04_HOLD_REASON_WAIT_COUNT)
		{
			kaijyo_vsync_wait();			/* wait終了したら、解除。 */
		}
		if (kaiwa_system_hold_mode & TASK_SWITCH_10_HOLD_REASON_TEXT_DRAWING)
		{
			/* ==== text描画処理(非同期) ==== */
			kaijyo_text_drawing();			/* 描画終了なら、解除。 */
		}
	#if (1==USE_r36_SCENE_FLAG)
		/*
			r36から道中イベントでスクリプトが呼ばれるようになりました。
			道中でキー入力待ちがあると、ショットが撃てないので、
			もし道中なら、キー入力待ちイベントは勝手に解除します。(これは暫定的な仕様です)
			---------------------------------------------------------
			将来的に中ボス会話等実装する場合、
			スクリプト上のコマンドでタスク禁止をコントロール出来るようにします。
			(タスク、マスクコマンドを新設する。)
		*/
		if (0==(cg.state_flag&0x0c00))// 道中(ステージタイトル)では、キー入力待ちをしない。
		{	/*(もし道中なら、)*/
			/*(勝手に解除)*/
			kaiwa_system_hold_mode &= ~(TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE|TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT);
		}
		else
	#else
	#endif
		{
			if (kaiwa_system_hold_mode & TASK_SWITCH_02_HOLD_REASON_KEY_HOLD_MODE)
			{
				kaijyo_pad_timer_hold();		/* 時間経過もしくは、ボタン押したら、解除。 */
			}
			if (kaiwa_system_hold_mode & TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT)
			{
				kaijyo_pad_boot_hold(); 		/* 起動時専用。パッドのボタンが離されたら、解除。 */
			}
		}
	}
	/* ==== BG 設定変更処理(非同期) ==== */
	kaiwa_system_vbl_do_BG_fade();
	/* ==== obj 移動処理(非同期) ==== */
	kaiwa_system_vbl_sprite_move(); /* 動作のみ(描画しない) */

	if (/*(0)*/KAIWA_SYSTEM_00_CONTINUE != kaiwa_system_terminate_flag)
	{
		do_kaiwa_system_terminate();
		kaiwa_system_terminate_call_func();
	}
}

/*---------------------------------------------------------
	init obj position.
---------------------------------------------------------*/
/*static*/ void kaiwa_init_obj_position(void)
{
	// プレイヤー
	kaiwa_sprite[0].cx256		= t256(-96);		kaiwa_sprite[0].cy256		= t256(16);
	kaiwa_sprite[0].origin_x256 = t256(-96);		kaiwa_sprite[0].origin_y256 = t256(16);
	kaiwa_sprite[0].offset_x256 = t256(-32);		kaiwa_sprite[0].offset_y256 = t256(16);
	// ボス
	kaiwa_sprite[1].cx256		= t256(352);		kaiwa_sprite[1].cy256		= t256(16);
	kaiwa_sprite[1].origin_x256 = t256(352);		kaiwa_sprite[1].origin_y256 = t256(16);
	kaiwa_sprite[1].offset_x256 = t256( 32);		kaiwa_sprite[1].offset_y256 = t256(16);
}

/*---------------------------------------------------------
	kaiwa obj draw on / off.
---------------------------------------------------------*/
/*static*/global void kaiwa_all_obj_draw_on_off(unsigned int on_off)
{
	kaiwa_sprite[0].draw_flag = (on_off);	/* 描画しない。 */
	kaiwa_sprite[1].draw_flag = (on_off);	/* 描画しない。 */
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
	aaa_kaiwa_system_reset();
	#if 1
		#if 0/*(aaa_kaiwa_system_reset();と重複)*/
	/* std_obj[]初期化 */
//	{	unsigned int i;
//		for (i=0; i<KAIWA_OBJ_99_MAX; i++)/*20*/
//		{
//			kaiwa_sprite[i].img = NULL;
//		}
//	}
//	draw_bg 				= (0);
		#endif
	kaiwa_init_obj_position();
	obj_look_up 							= (KAIWA_OBJ_99_MAX)+(1);/*(全部注目オブジェに設定)*/
	#endif
	//
//	kaiwa_all_obj_draw_on_off(1);	/* 立ち絵を描画する。 */
	//
	kanji_init_standard();/*(漢字関連の標準初期化)*/
	//
	kaiwa_system_skip_mode		= (0);/*off*/
	kaiwa_system_hold_mode		=
		(	TASK_SWITCH_01_HOLD_REASON_KEY_CHECK_ON_BOOT	/*(開始時はパッドが離されている事を確認する)*/
		|	TASK_SWITCH_20_HOLD_REASON_OBJECT_WAIT			/*(開始時はobjが退避している事を確認する)*/
		);
	kaiwa_system_terminate_flag = KAIWA_SYSTEM_00_CONTINUE;/*(0)*/	/* 初期化 */

	ml_font[(0)].haikei 		= (ML_HAIKEI_03_MESSAGE);/* [黒/会話用背景]せりふ背景on */
//廃止	cg.dr aw_flag_kaiwa_screen	= (1);/*0*/
	limit_timer 				= byou60(1);/*(1[秒]制限時間。起動時のパッドチェック時間)*/
	i_code_text_color			= (7);
	i_code_text_speed_wait		= (0);
	str_stack_position = 0;/* 文字列領域 */
	return (1);
}


/*---------------------------------------------------------
	ゲームイベント中のシナリオ会話 開始(ストーリ、エンディング以外)
---------------------------------------------------------*/
/*
r35 r36x
-:	 0: ステージ開始イベント
0:	 1: ボス戦闘前会話イベント
1:	 2: ボス戦闘後会話イベント1
2:	 3: ボス戦闘後会話イベント2


*/
global void kaiwa_load_ivent(void)
{
#if (1==USE_r36_SCENE_FLAG)
	/* ファイル名作成 */
	{
		/* 'data' '/kaiwa/' の分のオフセット */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_KAIWA_LENGTH)
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "Z/sZ0" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_file_common_name[DIRECTRY_NAME_OFFSET+0] = ('0'+(cg_game_select_player));
		my_file_common_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(cg.game_now_stage);
		my_file_common_name[DIRECTRY_NAME_OFFSET+4] += ((cg.state_flag>>10) & (0x03));
		NEXT_SCENE;
		//
		//#if (1==USE_EASY_BADEND)
		if ('1'==my_file_common_name[DIRECTRY_NAME_OFFSET+4])/*(bad判定が必要ならbad判定処理)*/
		{
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
						my_file_common_name[DIRECTRY_NAME_OFFSET+4]++;/*(bad確定)*/
					}
				}
			}
		}
		//#endif
	}
	kaiwa_system_terminate_call_func = kaiwa_system_terminate_game_core;	/* シナリオ会話 終わったらゲームコア用状態遷移 */
	load_kaiwa_youso_free();			/* 前回のシナリオがメモリにあればすべて開放。 */
	if (0 == aaa_kaiwa_system_start())	// ファイルがない場合はイベントを飛ばす
	{
		NEXT_SCENE;
	}

#else
	/* ファイル名作成 */
	{
		/* 'data' '/kaiwa/' の分のオフセット */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_KAIWA_LENGTH)
		strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_KAIWA_STR "Z/sZ1" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_file_common_name[DIRECTRY_NAME_OFFSET+0] = ('0'+(cg_game_select_player));
		my_file_common_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(cg.game_now_stage);
		//
		if ((cg.state_flag & STATE_FLAG_0x0800_IS_BOSS))/*(ボス戦闘後の場合)*/
		{
			// my_file_common_name[DIRECTRY_NAME_OFFSET+4] = '1';
			//#if (1==USE_EASY_BADEND)
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
						my_file_common_name[DIRECTRY_NAME_OFFSET+4]++;/*(bad確定)*/
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
		cg.state_flag |= STATE_FLAG_0x0300_END_KAIWA_MODE;
	}
	else
	{
		cg.state_flag |= STATE_FLAG_0x0200_IS_KAIWA_MODE;	/*on*/
		#if 0
		/* シナリオ中にボムが発生してしまう。バグがあるので。 */
		pd_bomber_time = 0;
		#endif
	}
#endif
}


#if (1)/*Gu描画になると要らなくなる*/
/*---------------------------------------------------------
	「ストーリーモード専用」シナリオ動作
---------------------------------------------------------*/

static void story_mode_local_work(void)
{
	kaiwa_system_SDL_BG_draw(); 				/* シナリオ会話システム SDL_BG 描画(遅い) */
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
	draw_bg 					= (0);			/* 0:off 背景表示フラグ */
	kanji_cursor_move_home_position();			/* 漢字カーソルをホームポジションへ移動 */
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
