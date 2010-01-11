
/*---------------------------------------------------------
	ステージ敵データー(dat)の読み込み
---------------------------------------------------------*/

#include "game_main.h"
#include "load_stage.h"

/*---------------------------------------------------------

---------------------------------------------------------*/

//extern int errno;

STAGE_DATA *stage_data_table = NULL;

/*---------------------------------------------------------
	子関数
	前回まで確保したメモリがあれば開放する。
	Initial task, "free"ed the altenative entris.
---------------------------------------------------------*/

static void load_stage_free_entry(void)
{
	STAGE_DATA *l = stage_data_table;
	STAGE_DATA *n;
	while (NULL != l)
	{
		n = l->next;
		free (l);
		l = n;
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
static void load_stage_add_entry(Uint32 time60, char user_command, char *user_string, int user_x, int user_y)		// オブジェクトの生成
{
	STAGE_DATA *new_entry;
	new_entry					= mmalloc(sizeof(STAGE_DATA));
//	new_entry->v_time			= (time60*6/*100*1000*/);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/1000000[sec] == 1[nsec] == nano seconds ) */
//	new_entry->v_time			= (time60*6/*100*1000*/);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/60[sec] ) */
	new_entry->v_time			= (time60  /*100*1000*/);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/60[sec] ) */
	new_entry->user_x			= user_x;
	new_entry->user_y			= user_y;
//	new_entry->done 			= 0;
//
	new_entry->user_1_moji		= user_command;
	new_entry->user_i_code		= 0;

	strncpy(new_entry->user_string, user_string, (MAX_PARA1_44-1)/*63*/);

/* ゲーム中は、中間コード形式のコマンドを扱い構文解析はしない。(構文解析はload_stage.cで行う) */

//
	const char *ctype_name[CTYPE_99_MAX] =	/* 現在 32 種類 */
	{
		NULL,/* [番兵区切り] */
	//	その他
		"QUIT", 	/* ゲーム 全ステージ クリアー */
		"BG",		/*	"BG_CONTROL"*/		/* ←システムコマンドなので英語にした */
	//	ボス
		"慧音", 	/*	extra 2 	"BOSS05",*/
		"チルノ",	/*	extra 1 	"BOSS05",*/
		"咲夜", 	/*	stage 6 	"BOSS04",*/ 	// [***090207		追加
		"パチェ",	/*	stage 5 	"BOSS01",*/
		"輝夜", 	/*	stage 4 	"BOSS03",*/
		"未定", 	/*	stage 3 	"BOSS03",*/ 	/* 3面はどうするか未定(永琳? 慧音?) */
		"文",		/*	stage 2 	"BOSS02",*/
		"アリス",	/*	stage 1 	"BOSS01",*/
	/* 特殊敵[中型敵] */
		"中ボス6",
		"小悪魔",	/*	"GFAIRY",*/ 	//		追加
		"鈴仙",
		"てゐ",
		"中ボス2",
		"ルーミア", /*	"GFAIRY",*/ 	//		追加
//		廃止?
		"妖怪2",	/*	"GFAIRY",*/ 	//		追加
		"妖怪1",	/*	"GFAIRY",*/ 	// [***090207		追加
//
	/* 魔方陣 */
		"魔方陣1",	/*	"GROUNDER",*/
		"魔方陣2",	/*	"MAGICF",*/
	/* 竜巻 陰陽玉 */
		"陰陽玉1",	/*	"PLASMABALL",*/
		"竜巻1",	/*	"PROBALL",*/
	/* 妖怪 */
		"囲妖怪1",	/*	"CUBE",*/
	/* その他ザコ */
		"おばけ1",	/*	"DRAGER",*/
		"おばけ2",	/*	"EYEFO",*/
		"紫編隊1",	/*	"MING",*/
		"紫編隊2",	/*	"GREETER",*/
	/* 毛玉 */
		"赤毛玉1",	/*	"MINE",*/
		"緑毛玉1",	/*	"XEV",*/
		"緑毛玉2",	/*	"CRUSHER",*/
		"毛玉1",	/*	"BADGUY",*/
		"毛玉2",	/*	"BADGUY",*/
	/* 中妖精 */
		"メイド1",	/*	"RWINGX",*/
		"メイド2",	/*	"ZATAK",*/
		"メイド3",	/*	"CURVER",*/
		"メイド4",	/*	"CIR",*/
	/* 小妖精 */
		"青妖精1",	/*	"FAIRY",*/		// [***090207		追加
		"青妖精2",	/*	"SPLASH",*/ 	// [***090124		追加
		"青妖精3",	/*	"SPLASH",*/ 	//		追加
		"青妖精4",	/*	"SPLASH",*/ 	//		追加
	};
//


	/* 読み込んだコマンドを中間コード形式に変換する */
	switch (new_entry->user_1_moji)
	{
	case 'T':	new_entry->user_i_code=ETYPE_01_SJIS_TEXT;		break;	/* english Text */
	case 'B':	new_entry->user_i_code=ETYPE_02_LOAD_BG;
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
		break;	/* Background */
	default:	// add background tiles....
		{
			for (new_entry->user_i_code = /*CT YPE_00_unknown+*/(CTYPE_99_MAX-1); /*0*/CTYPE_00_NONE/*CT YPE_00_unknown*/ < new_entry->user_i_code; new_entry->user_i_code--)
			{
				if (0==tiny_strcmp( new_entry->user_string, /*(char *)*/&/*btype_name*/ctype_name[(unsigned int)new_entry->user_i_code][0] ) )
				{
					break;
				}
			} // 0/*CT YPE_00_unknown*/ == aaa );
		}
		break;
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
#include "my_file.h"

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
		//
//		#if (1==US E_ENDING_DEBUG)
//		if (MA X_STAGE6_FOR_CHECK == player_now_stage/*continue_stage*/)
//		{
//		//	if (B07_AFTER_LOAD==pd->bo ssmode)
//			if ((STATE_FLAG_10_IS_LOAD_SCRIPT|ST ATE_FLAG_11_IS_BOSS_DESTROY)==(pd->state_flag&(STATE_FLAG_10_IS_LOAD_SCRIPT|ST ATE_FLAG_11_IS_BOSS_DESTROY)))
//			{
//				load_stage_number=9;/*エンディングデバッグ用*/
//			}
//		}
//		if (9!=load_stage_number)
//		#endif //(1==US E_ENDING_DEBUG)

extern int select_player;
extern void bg2_start_stage(void); // [***090209		追加
extern void stage_bg_load_texture(void);
void load_stage(void)	/*int level*/	/* 元々int */
{
	bg2_start_stage();	// [***090209		追加
	enemy_set_random_seed(/*set_seed*/);	/* リプレイ対応出来るように、乱数系列の初期化。 */
//	int level = player_now_stage;
	player_now_stage++; /*(*level)++*/;
	// change music soundtrack
	play_music_num( /*1+*/	/*level*/player_now_stage/*(*level)*/ );/* n面道中 */
//
//	int load_stage_number = player_now_stage;
	{	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
		{
			pd->state_flag &= (~(STATE_FLAG_05_IS_BOSS));/*ボスoff*/
		}
		draw_side_panel=1/*pd->state_flag |= ST ATE_FLAG_09_IS_PANEL_WINDOW*/;/* パネル表示on */
	}
//
	stage_bg_load_texture();
//
//
//	char my_fopen_file_name[128];
//	sp rintf(my_fopen_file_name,"%s/dat/level%02d.dat", data_dir, /*level*/player_now_stage);
//	sp rintf(my_fopen_file_name,"%s/dat/stage%01d.txt", data_dir, /*level*/player_now_stage);
//	sp rintf(my_fopen_file_name, "%s/dat/stage%c.txt", data_dir, ('0'+/*level*/load_stage_number/*player_now_stage*/) );
//	sp rintf(my_fopen_file_name, DIRECTRY_NAME_DATA "/dat/stage%c.txt", ('0'+/*level*/load_stage_number/*player_now_stage*/) );
	strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA "/dat/stageZ.txt");
	my_fopen_file_name[10+DIRECTRY_NAME_LENGTH] = ('0'+player_now_stage);/*level*/ /*load_stage_number*/ /*player_now_stage*/

//	/*FILE*/char *fp;
	if (NULL==(/*fp=*/my_fopen(/*my_fopen_file_name*/ /*,"r"*/)))
	{
	//	error(ERR_FATAL, "can't read stage data %s\nerrno: %d (%s)",my_fopen_file_name,errno,strerror(errno));
		error(ERR_FATAL, "can't read stage %d data %s\n", /*load_stage_number*/player_now_stage, my_fopen_file_name );
	}
//
	load_stage_free_entry();	/* 前回まで確保したメモリがあれば、先に開放する。 */
	int entrys		= 0;		/* 有効行数の調査 */
	int line_num	= 0;		/* ファイルの実行数 */
	{loop:;
		if (/*NULL*/0 != my_fgets(/*buffer_text_1_line,128,fp*/))
		{
			int end_arg/*=0*/;			/* [だみー]行末 == 引数の取得の中止 */
			int time60; 				/* 出現時間(1/60秒単位)  */
			char char_user_command; 	/* １文字コマンド(敵やメッセージ等の種別) */
			char user_string[128];		/* 文字列(メッセージやファイル名) */
			int user_x; 				/* 数字パラメーター１(出現Ｘ座標など) */
			int user_y; 				/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
			char *c;					/* 走査位置 */
			line_num++; 				/* ファイルの実行数 */
			c = buffer_text_1_line;
//
			/* skiped lines. */
			#if 0
			/* '\n'が悪いのか巧くいかない(???) */
			if (*c=='\n')		{	goto loop;/*continue;*/ }	/* skiped null line. */ 	/* Leerzeilen ueberspringen */
			while (isspace(*c)) {	c++;					}	/* dust left space.  */ 	/* fuehrende leerzeichen uebergehen */
			#else
			{my_isspace:;
				if (*c<=' ')
				{
					c++;
					if (*c==0x0a)
					{	goto loop;/*continue;*/ }	/* skiped null line. */
					else
					{	goto my_isspace;	}
				}
			}
			#endif
			if (*c=='#')		{	goto loop;/*continue;*/ }	/* skiped comment line. */	/* Kommentarzeile ? */
//
		#if (1==USE_PARTH_DEBUG)
			#define GOTO_ERR_WARN goto err_warn
		#else
			#define GOTO_ERR_WARN goto loop
		#endif
			/* parth start */	/* Startzeitpunkt holen */
			c = load_stage_get_int(c, &time60); 	if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int time60 */		/* 時間の取得 */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			char_user_command = *c++;																				/* load 1 char commnd */	/* １文字コマンド */	/* Befehl */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_my_file_get_str(c, user_string, &end_arg/*, '|'*/);
													if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load str user_string */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_int(c, &user_x); 	if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int user_x */
													if (','/*'|'*/ != *c++) {	GOTO_ERR_WARN;/*continue;*/;	}	/* load '|' */
			c = load_stage_get_int(c, &user_y); 	if (NULL == c)			{	GOTO_ERR_WARN;/*continue;*/;	}	/* load int user_y */
			/* do set register entry. */
			#define MUSIC_CONVERT_TIME (10)
			/* 追加登録する */
			load_stage_add_entry(MUSIC_CONVERT_TIME+time60, char_user_command, user_string, user_x, user_y);
			entrys++;		/* 有効行数 */
			goto loop;
		#if (1==USE_PARTH_DEBUG)
		err_warn:
			/* Load parth error. */
		//	static void load_stage_set_error(char *load_filename, int error_line_number)
		//	{
		//		error(ERR_WARN, "syntax error in stage data '%s', line no: %d", load_filename, error_line_number);
		//	}
		//	load_stage_set_error(filename, line_num);
			error(/*ERR_WARN*/ERR_FATAL, "syntax error in stage data '%s', line no: %d", filename, line_num);
			goto loop;
		#endif
		}
	}
	my_fclose (/*fp*/);
	//return (entrys);
	if (0==entrys)		/* 有効行数がなければエラー */
	{
		error(/*ERR_WARN*/ERR_FATAL, "no entrys for STAGE%d.TXT",player_now_stage);	/*level*/ /*load_stage_number*/
	}
	//fps_init();/* ??? auto fps初期化 */
}
