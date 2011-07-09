
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ステージ敵データー(dat)の読み込み
---------------------------------------------------------*/

global GAME_COMMAND *stage_command_table = NULL;

/*---------------------------------------------------------
	子関数
	前回まで確保したメモリがあれば開放する。
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/
//#include <malloc.h>/* free(); */

static void load_stage_free_entry(void)
{
	GAME_COMMAND *d = stage_command_table;
	GAME_COMMAND *n;
	while (NULL != d)
	{
		n = d->next;
		free (d);
		#if 0
		d = NULL;/* 念の為要る？ */
		#endif
		d = n;
	}
	stage_command_table = (GAME_COMMAND *)NULL;/* head to set NULL */
}

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

enum
{
	PARAM_00_time60 = 0,		//	u32 time60; 			/* 出現時間(1/60秒単位) */
	PARAM_01_user_command,		//	int user_command;		/* １文字コマンド(敵やメッセージ等の種別) */
	PARAM_02_user_select_gazou, //	int select_gazou;		/* アニメ/画像種類 */
	PARAM_03_user_kougeki_type, //	int user_kougeki_type;	/* 敵が攻撃する場合のスペカの種類 */
	PARAM_04_user_x,			//	int user_x; 			/* 数字パラメーター１(出現Ｘ座標など) */
	PARAM_05_user_y,			//	int user_y; 			/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
	PARAM_06_user_hp,			//	int user_hp;			/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
	PARAM_07_user_item8,		//	int user_item8; 		/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
	PARAM_08_user_score,		//	int user_score; 		/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
	PARAM_99_MAX/*最大数*/
};

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/
extern void load_bg2_chache(char *filename, int use_alpha);
/* Do set the entry. */
static void load_stage_add_entry( char *user_string, const int *c_pn )		// オブジェクトの生成
{
	GAME_COMMAND *new_entry;
	new_entry						= my_calloc(sizeof(GAME_COMMAND));
	#define MUSIC_CONVERT_TIME		(10)
	/* 10[flame](==0.1666[秒]ずらす。)読み込みと変換に時間がかかる事を考慮して、時間をずらす。 */
	new_entry->v_time				= MUSIC_CONVERT_TIME+(c_pn[PARAM_00_time60]);/* 元々フレーム単位 */
	new_entry->user_select_gazou	= c_pn[PARAM_02_user_select_gazou];
	new_entry->user_kougeki_type	= c_pn[PARAM_03_user_kougeki_type];
	new_entry->user_x				= c_pn[PARAM_04_user_x];
	new_entry->user_y				= c_pn[PARAM_05_user_y];
	new_entry->user_hp				= c_pn[PARAM_06_user_hp];
	new_entry->user_item8			= c_pn[PARAM_07_user_item8];
	new_entry->user_score			= c_pn[PARAM_08_user_score];
//
	new_entry->user_255_code		= (u8)((c_pn[PARAM_01_user_command])&0xff);
	new_entry->user_i_code			= 0;

	strncpy(new_entry->user_string, user_string, (MAX_PARA1_36-1)/*63*/);

/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

	const char *ctype_name[GC_CODE_MAX] = 	/* (r34)現在 6 種類 */
	{
		NULL,/* (sjis-text表示) [番兵区切り] */
	//	その他
		"QUIT", 	/* ゲーム 全ステージ クリアー */
		"BG",		/*	"BG_CONTROL"*/		/* ←システムコマンドなので英語にした */
	// 敵
		"ボス", 	/* (ボス / 中-ボス)用 スクリプト起動 */
		"中ボス",	/* ザコ中-ボス(スクリプト起動しない) */
		"ザコ", 	/* ザコ */
	};
//
	/* 読み込んだコマンドを中間コード形式に変換する */
	{
		for (new_entry->user_i_code = (GC_CODE_MAX-1);
			(0) < new_entry->user_i_code;
			new_entry->user_i_code--)
		{
			if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_i_code][0] ) )
			{
				break;
			}
		} // 0/*CT YPE_00_unknown*/ == aaa );
	}
//
	if (NULL==stage_command_table)	// 最初の解析行か？
	{	new_entry->next = (GAME_COMMAND *)NULL;}
	else
	{	new_entry->next = stage_command_table;	}	// stage_command_tableは前回のデータ
	stage_command_table = new_entry;		// stage_command_tableに今生成したデータのアドレスを代入。
	// nextにはdatファイル的には現在走査中の行の上の行が入っている。
}


/*---------------------------------------------------------
	子関数
	Get ascii a interger number.
---------------------------------------------------------*/

static char *load_stage_get_int(char *ccc, int *number)
{
	char buffer[32];/*128*/
	char *ddd = buffer;
	int i = 0;
//	while (isdigit(*ccc))			/* isdigit : 数字かどうかの判定 */
	while (
		(isdigit((int)((char)(*ccc))))	/* gcc 4.3.5 */
		||
		(((char)'-')==(*ccc))
	)		/* 負数にも対応 / isdigit : 数字かどうかの判定 */
	{
		i++;
		if (i >= 32)/*128*/
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
	*ddd = 0;
	*number = atoi(buffer);
	return (ccc);
/*error*/
ne222:
	return ((char *) NULL);
}
/*
gcc 4.3.5	warning: array subscript has type 'char'
isdigit( int );
*/

#include "111_my_file.h"

/*---------------------------------------------------------
	ステージ読み込み
	-------------------------------------------------------
	この関数は、仕様上「とても処理落ちします」
	★ libpng や jpeglib の画像展開、(これは仕方ない、目安200フレーム未満)
	★ 曲データーの展開、(oggにすると軽減される模様、目安40フレーム未満)
	★ ステージデーターのパーシング(KETMよりはだいぶ軽い、目安10フレーム未満、
	FILE構造体fopen()とかfgets()とか使う限り遅い。sceIoOpenとかsceIoReadとか使えばいいらしい)
	後の対応が悪いと、後まで処理落ちするので、
	後の対策が必須です。（無駄な時間待ち等）
---------------------------------------------------------*/

extern void set_default_bullet_clip(void);

extern int select_player;
extern void bg2_start_stage(void);
extern void stage_bg_load_texture(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);

extern void teki_random_item_table_initialize(void);/*int set_seed*/
global void load_stage(void)
{
	set_default_bullet_clip();		/* 弾幕の範囲を標準に設定 */
	bg2_start_stage();
	;/* リプレイ対応出来るように、乱数系列の初期化。 */
	teki_random_item_table_initialize();/*set_seed*/	/* ランダムアイテム敵の場合に使う、共通発生テーブルを初期化。 */
	cg.game_now_stage++;
	// change music soundtrack
	play_music_num(cg.game_now_stage);	/* n面道中 */
	{
		/* (r32) */cg.state_flag		|= STATE_FLAG_14_ZAKO_TUIKA;	/* 雑魚追加処理をする場合on */
		/* (r32) */cg.state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*ボスoff*/
		cg.side_panel_draw_flag = (1);	/* パネル表示on */	/* cg.state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW */
	}
	{
		const u32 bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/*(黒)*/
			0xff106010,/*1面(緑、魔法の森)*/
			0xff401010,/*3面(青、迷いの竹林)*/		//	0xff104010,/*3面(緑)*/
			0xff402020,/*4面(青、永遠亭)*/
			0xff804010,/*2面(水、妖怪の山、滝)*/	//	0xff802010,/*2面(青)*/		0xff102080,
			0xff601030,/*5面(青、図書館)*/
			0xff301060,/*6面(赤、紅魔館)*/			//	0xff601030,/*6面(青)*/
			0xff000000,/*ending(黒)*/
		//	0xff601010,/**/
		};
		gu_set_bg_u32_clear_color((bg_color_list[cg.game_now_stage&0x07]));
	}
	stage_bg_load_texture();
//
//	char my_file_common_name[128];
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%02d.dat", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%01d.txt", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ cg.game_now_stage) );
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ cg.game_now_stage) );
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/stageZ.txt");
	my_file_common_name[10+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage); /*load_stage_number*/

//	/*FILE*/char *fp;
	if (NULL==(/*fp =*/my_file_fopen()))	/* 開けなかったとき */	/*my_file_common_name*/ /*,"r"*/
	{	/* 読み込み失敗 */
	//	error(ERR_FATAL, (char*)"can't read stage data %s\nerrno: %d (%s)",my_file_common_name,errno,strerror(errno));
		error(ERR_FATAL, (char*)"can't read stage %d data %s\n", /*load_stage_number*/cg.game_now_stage, my_file_common_name );
	}
//
	load_stage_free_entry();	/* 前回まで確保したメモリがあれば、先に開放する。 */
	int entrys		= 0;		/* 有効行数の調査 */
	{
		MY_FILE_LOOP_BEGIN;
		//
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* データーの値が悪い場合はエラー */
		#define CHECK_ERROR_VALUE			if (NULL == ch) 	{	GOTO_ERR_WARN;	}
		/* データーの区切りに ',' が無い場合はエラー */
		#define CHECK_ERROR_VALUE_KUGIRI	CHECK_ERROR_VALUE	if (',' != *ch++)	{	GOTO_ERR_WARN;	}
		/* parth start. 構文解析開始 */
		int end_arg/*=0*/;	end_arg=0;		/* [だみー]行末 == 引数の取得の中止 */
		char user_string[128];		/* 文字列(メッセージやファイル名) */
		{
			int c_pn[PARAM_99_MAX];/*6*/
			ch = load_stage_get_int(ch, &c_pn[PARAM_00_time60]);				CHECK_ERROR_VALUE_KUGIRI	/* load int time60. */	/* 出現時間[フレーム(1/60秒)単位]の取得 */
			ch = load_my_file_get_str(ch, user_string, &end_arg);				CHECK_ERROR_VALUE_KUGIRI	/* load str user_string. */ 	/*, '|'*/
			ch = load_stage_get_int(ch, &c_pn[PARAM_01_user_command]);			CHECK_ERROR_VALUE_KUGIRI	/* load 1 char commnd. １文字コマンド */
			ch = load_stage_get_int(ch, &c_pn[PARAM_02_user_select_gazou]); 	CHECK_ERROR_VALUE_KUGIRI	/* load graphics select. */
			ch = load_stage_get_int(ch, &c_pn[PARAM_03_user_kougeki_type]); 	CHECK_ERROR_VALUE_KUGIRI	/* load graphics select. */
			ch = load_stage_get_int(ch, &c_pn[PARAM_04_user_x]);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_x. 出現x座標 */
			ch = load_stage_get_int(ch, &c_pn[PARAM_05_user_y]);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_y. 出現y座標 */
			ch = load_stage_get_int(ch, &c_pn[PARAM_06_user_hp]);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_hp. 体力 */
			ch = load_stage_get_int(ch, &c_pn[PARAM_07_user_item8]);			CHECK_ERROR_VALUE_KUGIRI	/* load int user_. 出現アイテム */
			ch = load_stage_get_int(ch, &c_pn[PARAM_08_user_score]);			CHECK_ERROR_VALUE			/* load int user_. 撃破時の獲得スコア[(1/10単位)] */
			/* do set register entry. 読み込み成功したデーターを、追加登録する。 */
			/* 追加登録する */
			load_stage_add_entry( user_string, c_pn );
		}
		entrys++;		/* 有効行数 */
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(/*ERR_WARN*/ERR_FATAL, (char*)"syntax error in stage data '%s', line no: %d", filename, debug_number_of_read_line);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	my_file_fclose(/*fp*/);
	if (0==entrys)		/* 有効行数がなければエラー */
	{
		error(/*ERR_WARN*/ERR_FATAL, (char*)"no entrys for STAGE%d.TXT", cg.game_now_stage);
	}
	//fps_init();/* ??? auto fps初期化 */
}
