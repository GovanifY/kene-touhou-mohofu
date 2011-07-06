
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ステージ敵データー(dat)の読み込み
---------------------------------------------------------*/

STAGE_DATA *stage_data_table = NULL;

/*---------------------------------------------------------
	子関数
	前回まで確保したメモリがあれば開放する。
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/
//#include <malloc.h>/* free(); */

static void load_stage_free_entry(void)
{
	STAGE_DATA *d = stage_data_table;
	STAGE_DATA *n;
	while (NULL != d)
	{
		n = d->next;
		free (d);
		#if 0
		d = NULL;/* 念の為要る？ */
		#endif
		d = n;
	}
	stage_data_table = (STAGE_DATA *)NULL;/* head to set NULL */
}

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/


/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/
extern void load_bg2_chache(char *filename, int use_alpha);
/* Do set the entry. */
static void load_stage_add_entry(
	u32 time60,
	char *user_string,
	int user_command,
	int user_select_gazou,
	int user_x,
	int user_y,
	int user_hp,
	int user_item8,
	int user_score
	)		// オブジェクトの生成
{
	STAGE_DATA *new_entry;
	new_entry						= my_calloc(sizeof(STAGE_DATA));
//	new_entry->v_time				= (time60*6/*100*1000*/);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/1000000[sec] == 1[nsec] == nano seconds ) */
//	new_entry->v_time				= (time60*6/*100*1000*/);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/60[sec] ) */
	new_entry->v_time				= (time60  /*100*1000*/);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/60[sec] ) */
	new_entry->user_x				= user_x;
	new_entry->user_y				= user_y;
	new_entry->user_hp				= user_hp;
	new_entry->user_item8			= user_item8;
	new_entry->user_select_gazou	= user_select_gazou;
	new_entry->user_score			= user_score;
//	new_entry->done 				= 0;
//
	new_entry->user_255_code		= (u8)(user_command&0xff);
	new_entry->user_i_code			= 0;

	strncpy(new_entry->user_string, user_string, (MAX_PARA1_36-1)/*63*/);

/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

	//	"慧音", 	/*	extra 2 	"BOSS05",*/
	//	"チルノ",	/*	extra 1 	"BOSS05",*/
	//	"咲夜", 	/*	stage 6 	"BOSS04",*/ 	// [***090207		追加
	//	"パチェ",	/*	stage 5 	"BOSS01",*/
	//	"輝夜", 	/*	stage 4 	"BOSS03",*/
	//	"未定", 	/*	stage 3 	"BOSS03",*/ 	/* 3面はどうするか未定(永琳? 慧音?) */
	//	"文",		/*	stage 2 	"BOSS02",*/
	//	"アリス",	/*	stage 1 	"BOSS01",*/

	//	"中ボス6",
	//	"小悪魔",	/*	"GFAIRY",*/ 	//		追加
	//	"鈴仙",
	//	"てゐ",
	//	"中ボス2",
	//	"ルーミア", /*	"GFAIRY",*/ 	//		追加
//		廃止
	//	"妖怪2",	/*	"GFAIRY",*/ 	//		追加
	//	"妖怪1",	/*	"GFAIRY",*/ 	// [***090207		追加


//
	const char *ctype_name[CTYPE_99_MAX] =	/* 現在 32 種類 */
	{
		NULL,/* [番兵区切り] */
	//	その他
		"QUIT", 	/* ゲーム 全ステージ クリアー */
		"BG",		/*	"BG_CONTROL"*/		/* ←システムコマンドなので英語にした */
	//	ボス
		"ボス", 	/* 共通化(ボススクリプト起動) */
	/* 特殊敵[中型敵] */
		"中ボス",
//
	/* 魔方陣 */
		"魔方陣1",	/*	"GROUNDER",*/
		"魔方陣2",	/*	"MAGICF",*/
	/* 竜巻 陰陽玉 */
		"陰陽玉1",	/*	"PLASMABALL",*/
		"竜巻1",	/*	"PROBALL",*/
	/* 妖怪 */
		"囲妖怪1",	/*	"CUBE",*/
		"赤毛玉1",	/*	"MINE",*/		/* 橙 */
	/* 毛玉 */
		"隕石1",	/*	"MING",*/		/* その他ザコ */
		"紫編隊2",	/*	"GREETER",*/	/* その他ザコ */
		"緑毛玉1",	/*	"XEV",*/
		"緑毛玉2",	/*	"CRUSHER",*/
		"毛玉1",	/*	"BADGUY",*/
		"毛玉2",	/*	"BADGUY",*/
	/* [C妖精]その他ザコ */
		"おばけ1",	/*	"DRAGER",*/
		"おばけ2",	/*	"EYEFO",*/
		"烏1",		/*	"",*/
	/* [B妖精]中妖精 */
		"メイド1",	/*	"RWINGX",*/
		"メイド2",	/*	"ZATAK",*/
		"メイド3",	/*	"CURVER",*/
		"メイド4",	/*	"CIR",*/
	/* [A妖精]小妖精 */
		"青妖精1",	/*	"FAIRY",*/		// [***090207		追加
		"青妖精2",	/*	"SPLASH",*/ 	// [***090124		追加
		"青妖精3",	/*	"SPLASH",*/ 	//		追加
		"青妖精4",	/*	"SPLASH",*/ 	//		追加
	};
//
	/* 読み込んだコマンドを中間コード形式に変換する */
	if ((255)==new_entry->user_255_code)
	{
		new_entry->user_i_code = ETYPE_01_SJIS_TEXT;	/* 'T'ext. */
	}
	else
	if ((128)==new_entry->user_255_code)
	{
		new_entry->user_i_code = ETYPE_02_LOAD_BG;	/* 'B'ack 'G'round. */		/* Background */
		#if 000
	//	if ( 0 == ti ny_strcmp(new_entry->user_string,"0") ) /* ファイル名が０の場合システムコマンド[拡張予定] */
		if ( '0' == new_entry->user_string[0] ) /* ファイル名の1字目が０の場合システムコマンド[拡張予定] */
		{
			;
		}
		else
		{
			load_bg2_chache(new_entry->user_string, 0); /* ゲーム中画像展開すると処理落ちが酷いのでキャッシュに詰める。 */
			/* 注意：手品師じゃないんだから、画像展開ライブラリ(libpngとかlibjpeg)を使う限り、どこかで処理落ちするよ。
				(画像展開ライブラリ(libpngとかlibjpeg)にCPUに負荷がかからないように、別スレッドでゆっくり展開させる機能が無い)
				つまり、ここの場所で展開してるんだから、ここの場所load_stage()で処理落ちが酷いという事だよ。 */
		}
		#endif
	}
	else
	{
		{
			for (new_entry->user_i_code = /*CT YPE_00_unknown+*/(CTYPE_99_MAX-1); /*0*/CTYPE_00_NONE/*CT YPE_00_unknown*/ < new_entry->user_i_code; new_entry->user_i_code--)
			{
				if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_i_code][0] ) )
				{
					break;
				}
			} // 0/*CT YPE_00_unknown*/ == aaa );
		}
	}
//
	if (NULL==stage_data_table) 	// 最初の解析行か？
	{	new_entry->next = (STAGE_DATA *)NULL;}
	else
	{	new_entry->next = stage_data_table; 	}	// stage_data_tableは前回のデータ
	stage_data_table = new_entry;		// stage_data_tableに今生成したデータのアドレスを代入。
	// nextにはdatファイル的には現在走査中の行の上の行が入っている。
}


/*---------------------------------------------------------
	子関数
	Get ascii a interger number.
---------------------------------------------------------*/

static char *load_stage_get_int(char *ccc, int *number)
{
	char buffer[32/*128*/];
	char *ddd = buffer;
	int i = 0;
//	while (isdigit(*c)) 			/* isdigit : 数字かどうかの判定 */
	while ((isdigit(*ccc))||('-'==(*ccc)))		/* 負数にも対応 / isdigit : 数字かどうかの判定 */
	{
		i++;
		if (i >= 32/*128*/)
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
extern void bg2_start_stage(void);	// [***090209		追加
extern void stage_bg_load_texture(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);
void load_stage(void)	/*int level*/	/* 元々int */
{
	set_default_bullet_clip();		/* 弾幕の範囲を標準に設定 */
	bg2_start_stage();				// [***090209		追加
	enemy_set_random_seed(/*set_seed*/);	/* リプレイ対応出来るように、乱数系列の初期化。 */
//	int level = player_now_stage;
	pd.player_now_stage++;				/*(*level)++*/
	// change music soundtrack
	play_music_num(pd.player_now_stage);	/* n面道中 */	 /*1+(*level)*/
//
//	int load_stage_number = player_now_stage;
	{
		{
			pd.state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*ボスoff*/
		}
		draw_side_panel = 1/*pd.state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW*/;/* パネル表示on */
	}
//
	{
		const u32 bg_color_list[8] =
		{	/*AABBGGRR*/
			0xff000000,/*(黒)*/
			0xff106010,/*1面(緑、魔法の森)*/
			0xff401010,/*3面(青、迷いの竹林)*/		//	0xff104010,/*3面(緑)*/
			0xff402020,/*4面(青、永遠亭)*/
			0xff804010,/*2面(水、妖怪の山、滝)*/	//	0xff802010,/*2面(青)*/		0xff102080,/*2面*/
			0xff601030,/*5面(青、図書館)*/
			0xff301060,/*6面(赤、紅魔館)*/			//	0xff601030,/*6面(青)*/
			0xff000000,/*ending(黒)*/
		//	0xff601010,/**/
		};
		gu_set_bg_u32_clear_color((bg_color_list[pd.player_now_stage&0x07]));
	}
	stage_bg_load_texture();
//
//
//	char my_fopen_file_name[128];
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%02d.dat", player_now_stage);
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%01d.txt", player_now_stage);
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ player_now_stage) );
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stage%c.txt", ('0'+ player_now_stage) );
	strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA_STR "/dat/stageZ.txt");
	my_fopen_file_name[10+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(pd.player_now_stage); /*load_stage_number*/

//	/*FILE*/char *fp;
	if (NULL==(/*fp =*/my_fopen(/*my_fopen_file_name*/ /*,"r"*/)))
	{
	//	error(ERR_FATAL, (char*)"can't read stage data %s\nerrno: %d (%s)",my_fopen_file_name,errno,strerror(errno));
		error(ERR_FATAL, (char*)"can't read stage %d data %s\n", /*load_stage_number*/pd.player_now_stage, my_fopen_file_name );
	}
//
	load_stage_free_entry();	/* 前回まで確保したメモリがあれば、先に開放する。 */
	int entrys		= 0;		/* 有効行数の調査 */
	int line_num	= 0;		/* ファイルの実行数 */
	{loop:;
		if (/*NULL*/0 != my_file_fgets(/*buffer_text_1_line,128,fp*/))
		{
			int end_arg/*=0*/;			/* [だみー]行末 == 引数の取得の中止 */
			int time60; 				/* 出現時間(1/60秒単位)  */
			char user_string[128];		/* 文字列(メッセージやファイル名) */
			int int_user_command;		/* １文字コマンド(敵やメッセージ等の種別) */
			int select_gazou;			/* アニメ/画像種類 */
			int user_x; 				/* 数字パラメーター１(出現Ｘ座標など) */
			int user_y; 				/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
			int user_hp;				/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
			int user_item8; 			/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
			int user_score; 			/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
			line_num++; 				/* ファイルの実行数 */
//
			char *ch;					/* 走査位置 */
			ch = buffer_text_1_line;
			/* skiped lines. */
			#if 0
			/* '\n'が悪いのか巧くいかない(???) */
			if ('\n'==(*ch))		{	goto loop;/*continue;*/ }	/* 改行のみの行は空行なのでやらないでとばす */
			while (isspace(*ch))	{	ch++;					}	/* 空白やTABを除去 */
			#else
			{my_isspace:;
				if (' '>=(*ch))
				{
					ch++;
					if (0x0a==(*ch))
					{	goto loop;/*continue;*/ }	/* skiped null line. */
					else
					{	goto my_isspace;	}
				}
			}
			#endif
			/* ';'で始まる行はコメント行なので、次の行まで飛ばす。 */
//			if (';'==(*ch)) 	{	goto loop;/*continue;*/ }	/* ';'で始まる行はコメントなのでやらないでとばす */
			//
		#if (1==USE_PARTH_DEBUG)
			/*continue;*/;
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN goto loop
		#endif
			/* データーの値が悪い場合はエラー */
			#define CHECK_ERROR_VALUE			if (NULL == ch) 	{	GOTO_ERR_WARN;	}
			/* データーの区切りに ',' が無い場合はエラー */
			#define CHECK_ERROR_VALUE_KUGIRI	CHECK_ERROR_VALUE	if (',' != *ch++)	{	GOTO_ERR_WARN;	}
			/* parth start. 構文解析開始 */
			ch = load_stage_get_int(ch, &time60);					CHECK_ERROR_VALUE_KUGIRI	/* load int time60. */	/* 出現時間[フレーム(1/60秒)単位]の取得 */
			ch = load_my_file_get_str(ch, user_string, &end_arg);	CHECK_ERROR_VALUE_KUGIRI	/* load str user_string. */ 	/*, '|'*/
			ch = load_stage_get_int(ch, &int_user_command); 		CHECK_ERROR_VALUE_KUGIRI	/* load 1 char commnd. １文字コマンド */
			ch = load_stage_get_int(ch, &select_gazou); 			CHECK_ERROR_VALUE_KUGIRI	/* load graphics select. */
			ch = load_stage_get_int(ch, &user_x);					CHECK_ERROR_VALUE_KUGIRI	/* load int user_x. 出現x座標 */
			ch = load_stage_get_int(ch, &user_y);					CHECK_ERROR_VALUE_KUGIRI	/* load int user_y. 出現y座標 */
			ch = load_stage_get_int(ch, &user_hp);					CHECK_ERROR_VALUE_KUGIRI	/* load int user_hp. 体力 */
			ch = load_stage_get_int(ch, &user_item8);				CHECK_ERROR_VALUE_KUGIRI	/* load int user_. 出現アイテム */
			ch = load_stage_get_int(ch, &user_score);				CHECK_ERROR_VALUE			/* load int user_. 撃破時の獲得スコア[(1/10単位)] */
			/* do set register entry. 読み込み成功したデーターを、追加登録する。 */
			#define MUSIC_CONVERT_TIME (10)
			/* 追加登録する */
			load_stage_add_entry(
				MUSIC_CONVERT_TIME+time60,
				user_string,
				int_user_command,
				select_gazou,
				user_x,
				user_y,
				user_hp,
				user_item8,
				user_score
			);
			entrys++;		/* 有効行数 */
			goto loop;
		#if (1==USE_PARTH_DEBUG)
		err_warn:
			/* Load parth error. */
		//	static void load_stage_set_error(char *load_filename, int error_line_number)
		//	{
		//		error(ERR_WARN, (char*)"syntax error in stage data '%s', line no: %d", load_filename, error_line_number);
		//	}
		//	load_stage_set_error(filename, line_num);
			error(/*ERR_WARN*/ERR_FATAL, (char*)"syntax error in stage data '%s', line no: %d", filename, line_num);
			goto loop;
		#endif
		}
	}
	my_fclose(/*fp*/);
	//return (entrys);
	if (0==entrys)		/* 有効行数がなければエラー */
	{
		error(/*ERR_WARN*/ERR_FATAL, (char*)"no entrys for STAGE%d.TXT", pd.player_now_stage); /*level*/ /*load_stage_number*/
	}
	//fps_init();/* ??? auto fps初期化 */
}
