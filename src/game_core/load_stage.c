
/*(カードシステム初期化用)*/
#include "./boss/boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ステージ敵データー(dat)の読み込み
---------------------------------------------------------*/

#include "menu/kaiwa_sprite.h"

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
	PARAM_00_time60 = 0,			//	u32 time60; 			/* 出現時間(1/60秒単位) */
	PARAM_01_user_i_code,			//	int user_command;		/* １文字コマンド(敵やメッセージ等の種別) */
	PARAM_02_user_255_code, 			//	int move_type;			/* (ザコ敵、移動処理の種類) */
	PARAM_03_user_select_gazou, 	//	int select_gazou;		/* アニメ/画像種類 */
	PARAM_04_user_kougeki_type, 	//	int user_kougeki_type;	/* 敵が攻撃する場合のカードの種類 */
	PARAM_05_user_locate_x, 		//	int user_locate_x;		/* 数字パラメーター１(出現Ｘ座標など) */
	PARAM_06_user_locate_y, 		//	int user_locate_y;		/* 数字パラメーター２(出現Ｙ座標、敵難度など) */
	PARAM_07_user_angle1024,		//	int user_angle1024; 	/* 数字パラメーター１(角度[deg1024]) */
	PARAM_08_user_speed256, 		//	int user_speed256;		/* 数字パラメーター２(速度[t256]) */
	PARAM_09_user_hp,				//	int user_hp;			/* 数字パラメーター２(敵体力) */
	PARAM_10_user_item8,			//	int user_item8; 		/* 数字パラメーター２(倒した後に出るアイテム) */
	PARAM_11_user_score,			//	int user_score; 		/* 数字パラメーター２(倒した後に得られる得点) */
	PARAM_99_MAX/*最大数*/
};


/*---------------------------------------------------------
	子関数
	Get ascii a interger number.
---------------------------------------------------------*/

#if (1)
/*static*/ int is_digit_or_hifun(char *ccc)
{
	#if 0
	/* 負数にも対応 / isdi git : 数字かどうかの判定 */
	if (
		(isdi git((int)((char)(*ccc)))) /* gcc 4.3.5 */
		||
		(((char)'-')==(*ccc))
	)
	{
		return (1/*TRUE*/);
	}/*ok!==*/
	//else
	{
		return (0/*FALSE*/);/*NG!,*/
	}/*(ng!)*/
	#else
	if (('9') < (*ccc))
	{
		goto my_false;/*NG!,*/
	}
	//else
	if (('0') > (*ccc))
	{
		/*('-'の可能性がある。)*/
		if ('-'!=(*ccc))
		{
			goto my_false;
		}
	}
//	my_ok:
		return (1/*TRUE*/);
	my_false:
		return (0/*FALSE*/);/*NG!,*/
	#endif
}
#endif
/*[is_digit_or_hifun();'数字'もしくは'-'記号の場合。であるかを調べる。]*/
static char *load_stage_get_int(char *ccc, int *number)
{
	char buffer[32];/*128*/
	char *ddd = buffer;
	int i = 0;
	while ( is_digit_or_hifun(ccc) )/*(負数に対応した)*/
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

extern void stage_bg_load_texture(void);
extern void gu_set_bg_u32_clear_color(u32 set_u32_clear_color);
extern void spell_cpu_douchuu_init(void);

extern void teki_random_item_table_initialize(void);/*int set_seed*/

#if (1==USE_AFTER_LOAD_STAGE)
#else
global void load_stage_make_filename(void)
{
//	char my_file_common_name[128];
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%02d.dat", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%01d.txt", cg.game_now_stage);
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%c.txt", ('0'+ cg.game_now_stage) );
//	sp rintf(my_file_common_name, DIRECTRY_NAME_DATA_STR "/dat/" "stage%c.txt", ('0'+ cg.game_now_stage) );
	strcpy(my_file_common_name, DIRECTRY_NAME_DATA_STR "/kaiwa/s/" "stageZ.txt");
	/*(stage'Z'の文字の位置が+14文字目)*/
	#define DIRECTRY_DOUCHU_OFFSET_LENGTH		(14)
	my_file_common_name[DIRECTRY_DOUCHU_OFFSET_LENGTH+DIRECTRY_NAME_DATA_LENGTH] = get_stage_chr(cg.game_now_stage); /*load_stage_number*/
	//
}
#endif

/*---------------------------------------------------------
	子関数
---------------------------------------------------------*/

/* Do set the entry. */
static void load_stage_add_entry(const int *c_pn)		// オブジェクトの生成
{
	GAME_COMMAND *new_entry;
	new_entry						= my_calloc(sizeof(GAME_COMMAND));
	#define MUSIC_CONVERT_TIME		(10)
	/* 10[frame](==0.1666[秒]ずらす。)読み込みと変換に時間がかかる事を考慮して、時間をずらす。 */
	new_entry->v_time				= MUSIC_CONVERT_TIME+(c_pn[PARAM_00_time60]);/* 元々フレーム単位 */
//
	new_entry->user_i_code			= (c_pn[PARAM_01_user_i_code]&0x07);
	new_entry->user_255_code		= (u8)((c_pn[PARAM_02_user_255_code])&0xff);
//
	new_entry->user_select_gazou	= c_pn[PARAM_03_user_select_gazou];
	new_entry->user_kougeki_type	= c_pn[PARAM_04_user_kougeki_type];
	new_entry->user_locate_x		= c_pn[PARAM_05_user_locate_x];
	new_entry->user_locate_y		= c_pn[PARAM_06_user_locate_y];
	new_entry->user_angle1024		= c_pn[PARAM_07_user_angle1024];
	new_entry->user_speed256		= c_pn[PARAM_08_user_speed256];
	#if 1
	new_entry->user_hp				= ((4)*c_pn[PARAM_09_user_hp]);/*(r36とりあえず)*/
	//
//デバッグ//	new_entry->user_hp				= (1);/*(r39とりあえず)*/
	#endif
	new_entry->user_item8			= c_pn[PARAM_10_user_item8];
	new_entry->user_score			= c_pn[PARAM_11_user_score];
//
	if (NULL==stage_command_table)	// 最初の解析行か？
			{	new_entry->next = (GAME_COMMAND *)NULL;}
	else	{	new_entry->next = stage_command_table;	}	// stage_command_tableは前回のデータ
	stage_command_table = new_entry;		// stage_command_tableに今生成したデータのアドレスを代入。
	// nextにはdatファイル的には現在走査中の行の上の行が入っている。
}

global void load_stage_data(void)
{
//	/*FILE*/char *fp;
	if (NULL==(/*fp =*/my_file_fopen()))	/* 開けなかったとき */	/*my_file_common_name*/ /*,"r"*/
	{	/* 読み込み失敗 */
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load stage: %d 面の道中敵設定" "\\n"
			"ファイルがありません。" "\\n"
			"%s",
			/*load_stage_number*/cg.game_now_stage,
			my_file_common_name );
	}
//
	load_stage_free_entry();	/* 前回まで確保したメモリがあれば、先に開放する。 */
	int readed_lines		= 0;	/* 有効行数の調査 */
	{
		MY_FILE_LOOP_BEGIN;
		//
		#if (1==USE_PARTH_DEBUG)
			/*(USE_PARTH_DEBUG 1:意味の解からない行があった場合は、ゲーム中止。デバッグ用)*/
			#define GOTO_ERR_WARN goto err_warn
		#else
			/*(USE_PARTH_DEBUG 0:意味の解からない行があった場合は、その行を無視してゲーム継続。標準)*/
			#define GOTO_ERR_WARN MY_FILE_LOOP_CONTINUE
		#endif
		/* parth start. 構文解析開始 */
		{
			int c_pn[PARAM_99_MAX];/*6*/
			#if 1
			{
				int iii;
				iii = PARAM_00_time60;
		loop_iii_start:
				ch = load_stage_get_int(ch, &c_pn[iii]);
				/* データーの値が悪い場合はエラー */
				if (NULL == ch) 	{	GOTO_ERR_WARN;	}
				//
				if (PARAM_11_user_score==iii)
				{
					goto loop_iii_end;
				}
				iii++;
				/* データーの区切りに ',' が無い場合はエラー */
				if (',' != *ch++)	{	GOTO_ERR_WARN;	}
				goto loop_iii_start;
			}
		loop_iii_end:
			#endif
			/* do set register entry. 読み込み成功したデーターを、追加登録する。 */
			/* 追加登録する */
			load_stage_add_entry(c_pn);
		}
		readed_lines++; 	/* 実際に読めた有効行数 */
		MY_FILE_LOOP_CONTINUE;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load stage: 道中敵設定ファイルの、" "\\n"
			"%d行目は意味が判りません。" "\\n"
			"%s",
			debug_number_of_read_line,
			my_file_common_name);
		MY_FILE_LOOP_CONTINUE;
	#endif
		MY_FILE_LOOP_END;
	}
	my_file_fclose(/*fp*/);
	if (0==readed_lines)		/* 有効行数がなければエラー */
	{
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load stage: STAGE%d.TXT は道中敵設定" "\\n"
			"ファイルではありません。", cg.game_now_stage);
	}
}


/*---------------------------------------------------------
	ステージ読み込み前の初期化
	-------------------------------------------------------
	ステージ読み込みより前に初期化する部分。
---------------------------------------------------------*/

global void load_stage_init(void)
{
	set_default_bullet_clip();		/* 弾の範囲を標準に設定 */
	#if (1)
	spell_cpu_douchuu_init();		/* 弾消しコールバックを道中と共用している都合上ここに必要。 */
	card.spell_used_number		= SPELL_00;/*(道中コールバック有効フラグと共用)*/
	#endif
	;/* リプレイ対応出来るように、乱数系列の初期化。 */
	teki_random_item_table_initialize();/*set_seed*/	/* ランダムアイテム敵の場合に使う、共通発生テーブルを初期化。 */
	cg.game_now_stage++;
	{
		/*(必ず消すフラグ)*/
		{
			/*(アイテム自動収集フラグを戻す)*/
			/* 自動収集モードフラグを全て消す。 */
			cg.state_flag &=
			(	~(	  JIKI_FLAG_0x0010_JYOU_BU_SYUU_SYUU					/* 終わり */	/* MAX時の上回収 */
					| JIKI_FLAG_0x0020_BOMBER_SYUU_SYUU 					/* 終わり */	/* ボム発動中のみ回収 */
					| JIKI_FLAG_0x0040_BOSS_GO_ITEM_JIDOU_SYUU_SYUU 		/* 終わり */	/* ボス撃破時の一時回収 */
				)
			);
		}
		cg.side_panel_draw_flag 		= (1);/*(右パネル表示on)*/
		cg.player_option_mode			= (0);/* どこかで初期化が必ず必要 */

		kaiwa_all_obj_draw_on_off(0);	/* 立ち絵を描画しない。 */
	//	player_option_position_initilize(); 	/*(オプションの位置を初期化する)*/
	}
	{
		const u32 bg_color_list[8] =
		{	/*AABBGGRR*/
			0xffaa9999,/*extra_stage(白紫)*/		//	0xff000000 (黒)
			0xffaa9999,/*1面(白紫、魔法の森)*/		//	0xffcdb4b9	0xff106010, (緑、魔法の森)
			0xff401010,/*3面(青、迷いの竹林)*/		//	0xff401010	//	0xff104010,/*3面(緑)*/
			0xff402020,/*4面(青、永遠亭)*/			//	0xff402020
			0xff804010,/*2面(水、妖怪の山、滝)*/	//	0xff804010	//	0xff802010,/*2面(青)*/		0xff102080,
			0xff601030,/*5面(青、図書館)*/			//	0xff601030
			0xff1311ee,/*6面(赤、紅魔館)*/			//	0xff333af4	0xff301060	//	0xff601030,/*6面(青)*/
			0xff000000,/*ending(黒)*/				//
		//	0xff601010,/**/
		};
		gu_set_bg_u32_clear_color((bg_color_list[cg.game_now_stage&0x07]));
	}
	stage_bg_load_texture();
}
