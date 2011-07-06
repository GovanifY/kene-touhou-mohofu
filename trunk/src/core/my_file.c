
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ファイル関連処理
	-------------------------------------------------------
	ファイル関連のサポートルーチンは、ここに記述してください。

	敵出現データー(dat/xxxx.txt)ファイル読み込み
	シナリオスクリプト(text/xxxx.txt)ファイル読み込み
	ハイスコアファイル読み込み／保存
	設定読み込み／保存
	画面保存
---------------------------------------------------------*/

#include "111_my_file.h"
//#include <malloc.h>/* free(); */

// '|' ',' ',' const char kugiri_1_char
#define KUGIRI_KAMMA_1_CHAR 		(',')
#define COMMENT_SEMIKORON_1_CHAR	(';')

/*---------------------------------------------------------
	shift jisコード、全角1バイト目かどうか判定する子関数
---------------------------------------------------------*/

/*static*/global int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte)
{
	#if 1
	high_byte &= 0xff;/**/
	return (
		((high_byte >= 0x81) && (high_byte <= 0x9f)) ||
		((high_byte >= 0xe0) && (high_byte <= 0xfd))
	);
	#else
	high_byte ^= 0x20;
	high_byte += (0x100-0xa1);
	high_byte &= 0xff;/**/
	return (high_byte < 0x3du);
	#endif
}


/*---------------------------------------------------------
	スクリプトファイルの文字列部分の読み込み
	-------------------------------------------------------
	shift jis 漢字の2byte目が￥￥の場合や
	エスケープシークエンス処理の2byte目が￥￥の場合でも
	問題がない
---------------------------------------------------------*/

/*static*/global char *load_my_file_get_str(char *str, char *buffer, int *end_arg)
{
	int string_error_limiter;
	string_error_limiter = 200;/* 200 文字以上はエラー */
	/*unsigned char*/int high_byte;
	{loop:;
		high_byte = ((*str) & 0xff);
		/* 空文字列の可能性があるから、始めに判定 */
		if (KUGIRI_KAMMA_1_CHAR == high_byte)	/* ','区切りでおしまいの場合 */
		{
			goto kamma_end;
		}
		else
		if (13 == high_byte)	/* '\n'==13(行末)でおしまいの場合 */
		{
	//	ret13_end:
			*end_arg = 1;/* 行末です。 */
		kamma_end:
			*buffer = '\0'; 	/* EOS を追加 */
			return (str);
		}
		else					/* 文字列を転送する必要のある場合 */
		{
			int flag;
			flag = 0;
			if (is_kanji_1st(high_byte)) /* shift jis 漢字 */
			{
				;	//	*buffer++ = *str++;/* 1byte目 */
			}
			else
			{
				if ('\\' == high_byte)		/* エスケープシークエンス処理(escape sequence) */
				{
					;	//	*buffer++ = *str++;/* ￥￥ */
				}
				else					/* 半角文字 */
				{
					flag = 1;
				}
			}
			if (0==flag)	/* 半角文字以外(shift jis 漢字、エスケープシークエンス処理)は 2 byte転送 */
			{
				*buffer++ = *str++;
			}
			*buffer++ = *str++; 	/* 1 byteは必ず転送 */
			/* エラーチェック */
			{
				string_error_limiter--;
				if (0 >= string_error_limiter)
				{
					return ((char *)NULL);
				}
			}
		}
		goto loop;
	}
}


/*---------------------------------------------------------
	汎用性のまるでない読み込み関連。子関数
	-------------------------------------------------------
	標準入出力はpspでは遅すぎるのでsceの関数を使う。(ゲーム中の処理落ち軽減策)
	エラーチェックとか全然無いので注意の事
---------------------------------------------------------*/

//atic char buffer_text_1_line[128];	/* parth text, 1 line buffer */ 	/* 走査する行の取得 */
/*static*/ char buffer_text_1_line[256];	/* parth text, 1 line buffer */ 	/* 走査する行の取得 */
/*static*/ char my_fopen_file_name[256];	/* fopenするファイル名 (128) */
#if 1
static unsigned long file_size;
static unsigned long file_seek;
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;

/*static*/global void *my_fopen(void/*const char *my_fopen_file_name*/ /*, const char *dummy*/)
{
	SceUID fd;
	fd = sceIoOpen((char *)my_fopen_file_name, PSP_O_RDONLY, 0777);
	if (0 == fd)
	{
		goto error111;
	}
	file_size = sceIoLseek32(fd, 0, PSP_SEEK_END);
	file_seek = 0;

	malloc_buf = malloc(file_size);
	if (NULL == malloc_buf)
	{
		sceIoClose(fd);
		goto error111;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	sceIoRead( fd, malloc_buf, file_size);
	sceIoClose(fd);
//	my_buf = malloc_buf;
//
	return (malloc_buf);
error111:
	return (NULL);
}
global int my_file_now_size(void)
{
	return (file_size);
}


static int my_fgets(void/*char *buffer_name, int num, char *wfp*/)
{
	int ii;
	ii = 0;
	char aaa;
//	char bbb;
//bbb=0;
	fgets_loop:;
	aaa = /*buffer_name*/buffer_text_1_line[ii] = /*my_buf*/malloc_buf[file_seek]/*(*my_buf)*/;
//	my_buf++;
	ii++;
	file_seek++;
	if (0x0a == aaa)	return (1);
	if (file_size < file_seek)	return (0)/*NULL*/;
	goto fgets_loop;
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
}
/*static*/global int my_file_fgets(void/*char *buffer_name, int num, char *wfp*/)
{
ggg_loop:;
	int ii;
	ii = my_fgets();
	/* ';'で始まる行はコメント行なので、処理しないで次の行まで飛ばす。 */
	if (COMMENT_SEMIKORON_1_CHAR == (buffer_text_1_line[0]))
	{	goto ggg_loop;/*continue;*/ }
	return (ii);
}
/*static*/global void my_fclose(void/*void *wfp*/)
{
	free(malloc_buf);
}
//(????)
	/*static*/global void my_fseek_begin(void/*void *wfp*/)
	{
	//	file_seek = 0;
	}
#else
	#define my_fopen	fopen
	#define my_fgets	fgets
	#define my_fclose	fclose
#endif

//#define FILE_NAME_SETTING_TXT "setting.ini"
#define FILE_NAME_SETTING_TXT	DIRECTRY_NAME_DATA_STR "/setting.txt"


/*---------------------------------------------------------
	ハイスコアファイル関連処理
---------------------------------------------------------*/

/*---------------------------------------------------------
	設定ファイル関連処理
---------------------------------------------------------*/
static int ini_load_item(/*FILE *fp,*/ char *search, char *str_result)
{
//	char buffer_text_1_line[128];		/* 行バッファ */
	char item_name[32/*30*/];			/* コンフィグ項目名 */
//	int int_result; int_result = -1;
	/* 走査するよ */
	my_fseek_begin();//fseek(fp, 0L, SEEK_SET);
	{loop:;
	//	while (/*NULL*/0 != fgets(buffer_text_1_line,128,fp))
	//	if (/*NULL*/0 != fgets(buffer_text_1_line,128,fp))
		if (/*NULL*/0 != my_file_fgets(/*buffer_text_1_line,128,fp*/))
		{
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
//			if (COMMENT_SEMIKORON_1_CHAR==(*ch))	{	goto loop;/*continue;*/ }	/* ';'で始まる行はコメントなのでやらないでとばす */
			//
		//	error(ERR_FATAL, "test %s\n", c );/*test k0a,1 */
			//
			{char *sc = item_name;
				int i=0;
				while (KUGIRI_KAMMA_1_CHAR != (*ch))	/* 区切り',' を探す */
				{
					i++;
					if (30 < i) 	{	return (-1);	}	/* >= 30 長すぎたらエラー */
					*sc++ = *ch++;
				}
				ch++;		/* 区切り',' を無視 (',' の分をとばす) */
				*sc = 0;	// '\0' を追記。  NULL
			}
			//
		//	error(ERR_FATAL, "test %s | %s\n", c, item_name);/*test 1\n | k0a  */
			//
			if (0==tiny_strcmp(item_name, search))
			{
				if (NULL==str_result) // 対象を整数として解析する。(PARTH_MODE_INT) */
				{
					char re_s[32/*30*/];
					char *re_e = re_s;
				//	while ('\n' != (*ch))		/* intの方は数字じゃない物は排除してくれるみたいだから問題なし。(13でなくてok) */
					while (0x0a != (*ch))		/* intの方は数字じゃない物は排除してくれるみたいだから問題なし。(13でなくてok) */
					{
						*re_e++ = *ch++;
					}
					re_e = 0;
					/*int_result =*/return (atoi(re_s));
//					goto my_exit;
			//		break;
				}
				else // 対象を文字列として解析する。 (PARTH_MODE_CHAR) */
				{
					char *re_e = str_result;
				//	while (0x0d != (*ch)/*13*/) /* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
					while (0x0a != (*ch))		/* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
					{
						*re_e++ = *ch++;
					}
					re_e = 0;
					/*int_result =*/return (1);/* ok */
//					goto my_exit;
			//		break;
				}
			}
			goto loop;
		}
	}
//my_exit:
//	return (int_result);
	return (-1);/* 見つからなかったらエラー */
}


static const char *my_config_title[OPTION_CONFIG_08_MAX]=
{
	"player",
	"bomb",
	"bgm",
	"sound",
	"current_difficulty",
	"current_player",
	"analog",
	"open",
};
//extern char str_pa ss_word[]; 	/* [***090222 */
extern int select_player;
extern void set_default_key(int *key_setting_map, int key_setting_type);
extern void set_default_option(int *option_setting_map);
extern void check_limit_value_option(int *option_setting_map);
global void ini_load(void)
{
	#if 1
	// 要望があったので全力で対応
//	sceIoMkdir(const char *aaa_dir, SceMode mode);
	sceIoMkdir("ms0:/PICTURE", 0777);	/* "ms0:/PICTURE"がない場合、
	"ms0:/PICTURE/東方模倣風"が作れないので、必ず必要。
	ない場合、無視されるのではなくて、最悪「ハングアップしたりする」。
	(SDL側のせいって気もするけど???)
	*/
//	sceIoMkdir("ms0:/PICTURE/東方模倣風/", 0777);/* 出来ない */
	sceIoMkdir("ms0:/PICTURE/東方模倣風", 0777);/* 出来る */
	#endif

//	FILE *fp;
//	char my_fopen_file_name[128/*64 50*/];
//	strcpy(my_fopen_file_name, "./" FILE_NAME_SETTING_TXT);
	strcpy(my_fopen_file_name, FILE_NAME_SETTING_TXT);
	int tmp;
	int int_result;
	int_result = 0;/* 異常 */
	int ng1;
	ng1 = 0;/*fopen()成功*/
//
//	if ( NULL == /* fp =*/ fopen(my_fopen_file_name, "r"))
	if ( NULL == /* fp =*/ my_fopen(/*my_fopen_file_name, "r"*/))
	{
		ng1 = 1;/*fopen()失敗*/ goto error00;/* return (-1); */
	}
	/* キーコンフィグ設定の読み込み */
	{
		char bufaaa[4];
		bufaaa[0] = 'K';
		bufaaa[1] = '0';
//		bufaaa[2] = ('a'+i);
		bufaaa[3] = 0;
		int i;
		for (i=0; i<KEY_NUM12_MAX; i++)
		{
			bufaaa[2] = ('a'+i);
		//
		//	CONFIG_LOAD_ITEM( bufaaa,				pad_config[i]	);
			tmp = ini_load_item(/*fp,*/ bufaaa, NULL);
			if (-1 != tmp)	{	pad_config[i] = tmp;	}
			else			{	goto error00/*return (-1)*/;	}
		}
	}
	/* オプション設定の読み込み */
	{
		int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
		//	CONFIG_LOAD_ITEM( (char *)my_config_title[i],				option_config[i]	);
			tmp = ini_load_item(/*fp,*/ (char *)my_config_title[i], NULL);
			if (-1 != tmp)	{	option_config[i] = tmp; }
			else			{	goto error00/*return (-1)*/;	}
		}
	}
	int_result = 1;/* 正常 */
//
error00:
	/* high_score load */
	{	int ng2;
		int tmpscore;
		ng2 = ng1;
		tmpscore = 0;
		char tmp_str_pl[8];
		//-----------------01234567
		strcpy(tmp_str_pl,"SCORE00");
		tmp_str_pl[7] = 0;/* 念の為 */
//
		int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{
			tmp_str_pl[5] = '0'+j;/* PLAYER */
			int i;
			for (i=0; i<5; i++)
			{
				tmp_str_pl[6] = '0'+i;/* score rank number. 順位 */
				if (0==ng2)
				{
				//#define tmp_str32 buffer_text_1_line
					char tmp_str32[64/*50*/];
				//	if (1 == fscanf(fp, "%23s\n", tmp_str32 ))
				//	if (0 != my_fgets(/*fp, "%23s\n", tmp_str32*/ ))
					int aaa;
					aaa = ini_load_item(tmp_str_pl, tmp_str32 );
					if (-1 != aaa)
					{
						/* 埋め込む */
						high_score_table[j][i].final_stage = tmp_str32[1/*10*/]-('0');

					//	strncpy(high_score_table[j][i].name, &tmp_str32[10], 3);
						high_score_table[j][i].name[0] = tmp_str32[2/*10*/];
						high_score_table[j][i].name[1] = tmp_str32[3/*11*/];
						high_score_table[j][i].name[2] = tmp_str32[4/*12*/];
						high_score_table[j][i].name[3] = tmp_str32[5/*13*/];
//
						high_score_table[j][i].name[4] = tmp_str32[6/*14*/];
						high_score_table[j][i].name[5] = tmp_str32[7/*15*/];
						high_score_table[j][i].name[6] = tmp_str32[8/*16*/];
						high_score_table[j][i].name[7] = tmp_str32[9/*16*/];
						char tmp_str16[64/*50*/];
						strcpy(tmp_str16, &tmp_str32[10/*5 13*/]);
						tmpscore = atoi(tmp_str16);
//						tmpscore /= 10;
					}
					else/* エラー */
					{
						ng2 = 1;
					}
				}
			/*	else // 直前の ini_load_item() でエラーが起きた場合に初期化するので このelse無効。 */
			//	if (1==ng2) /* pspは0レジスタがあるので0と比較したほうが速い */
				if (0!=ng2)
				{
					high_score_table[j][i].final_stage = (6-i); 	/* 到達ステージ */
					static const int init_score_tbl[5] =
					{
						score(100000000),	//	score(70000000),		//score(50000000),
						score( 50000000),	//	score(60000000),		//score(4000000),
						score( 10000000),	//	score(50000000),		//score(300000),
						score(	5000000),	//	score(10000000),		//score(20000),
						score(	1000000),	//	score( 5000000),		//score(1000),
					};
				//	strcpy(high_score_table[j][i].name,"12345678"/*"DEN"*/);
				//	strcpy(high_score_table[j][i].name,"ZUN     "/*"DEN"*/);
					strcpy(high_score_table[j][i].name,"NANASHI "/*"DEN"*/);
					tmpscore = init_score_tbl[i];
				}
				high_score_table[j][i].score = tmpscore;
			}
		}
	//	top_score = high_score_table[0][0].score;	// 常に表示するハイコアの取得=>score.cで利用
	}
//
	if (0==ng1)
	{
		my_fclose (/*fp*/); 	//fclose(fp);
	}
	//return (int_result)/*1*/;

	if (0==(int_result)) // 090110
	{
//		chooseModDir();
		set_default_key(pad_config, 0/*0==type 01 模倣風 標準*/);
		set_default_option(option_config);/* OPTION_CONFIG_00_PLAYER から OPTION_CONFIG_03_SOUND まで設定 */
	//	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= 0;
	//	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= 0;
	//	option_config[OPTION_CONFIG_06_ANALOG]				= 0;
		option_config[OPTION_CONFIG_07_OPEN]				= 0x0500;
	}
//
	/* 範囲外チェック(範囲外の場合は修正) */
	check_limit_value_option(option_config);/* */
	/* 読み込んだデーターで初期設定 */
	difficulty		= option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY];
	select_player	= option_config[OPTION_CONFIG_05_CURRENT_PLAYER];
//	if (-1 == ini_load_item(/*fp,*/ "pa ssword", st r_pass_word))	{	goto error00;/* return (-1); */ 	}

	#if 1/*修正ずみ*/
//	範囲外の場合は修正
//	if (difficulty>3 || 0>difficulty)/* (easy)0 1 2 3(Lunatic) */
//	{	difficulty = 0/* 0==easy 2*/;}
	difficulty		&= 0x03;
	select_player	&= 0x07;
	#endif
}



static void write_buf(/*FILE *fp*/SceUID fd, char *str_buf)
{
//	const char k = 13;
//	fprintf(fp, "%s%c\n", str_buf,	k);
	char *str_buf_head;
	str_buf_head = str_buf;
	unsigned int len;
	len = 0;
	while (0!=(*str_buf))
	{
		len++;
		str_buf++;
	};
	/* 差分氏模倣風のファイル形式が[CR+LF]だったので互換を取る。 */
	*str_buf=0x0d;	len++;	str_buf++;/*[CR]*/
	*str_buf=0x0a;	len++;	str_buf++;/*[LF]*/
//	fprintf(fp, "%s\n", str_buf);
	sceIoWrite(fd, str_buf_head, len/*sizeof(int)*/ /**save_data_size*/ /*, 1, fp*/);
}
global void ini_save(void)
{
//	FILE *fp;
//	char buf[64/*50*/];
	char buf[128/*64 50*/];
	strcpy(buf, "./" FILE_NAME_SETTING_TXT);
//	fp = fopen(buf, "w");
	SceUID fd = sceIoOpen(buf, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
//	if (fd < 0)
//	{
//		/*"セーブデータがない。"*/
//	}
//	if ( NULL == fp)	{	return; 	}
//

	/* 何故か巧く行かない */
	#define USE_MEMO (0)
	#if (1==USE_MEMO)
	//---------------------------
	strcpy(buf,
		"; 東方模倣風(r32) configuration file.\n"
		";---------------------------\n"
		"\n"
		"; キーコンフィグ設定\n"
	);
	write_buf(/*fp,*/fd, buf);
	#endif	/* (USE_MEMO) */
	//
	{
		unsigned int i;
		for (i=0; i<KEY_NUM12_MAX; i++)
		{
			sprintf(buf,
				"K0%c,%d",
				('a'+i),
				pad_config[i]);
			write_buf(/*fp,*/fd, buf);
		}
	}
	//---------------------------
	#if (1==USE_MEMO)
	strcpy(buf, "; オプション設定\n" );
	write_buf(/*fp,*/fd, buf);
	#endif	/* (USE_MEMO) */
	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= difficulty;
	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= select_player;

	{
		unsigned int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			sprintf(buf,
				"%s,%d",
				my_config_title[i],
				option_config[i]);
			write_buf(/*fp,*/fd, buf);
		}
	}
	//---------------------------
	#if (1==USE_MEMO)
	strcpy(buf, "; 夢の記録\n" );
	write_buf(/*fp,*/fd, buf);
	#endif	/* (USE_MEMO) */
	/* high_score save */
	{unsigned int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{	unsigned int i;
			for (i=0; i<5; i++)
			{
				sprintf(buf,
					"SCORE" 		/* == dummy */
					"%c"			/* player number */
					"%c"			/* rank number */
					"," 			/* == 区切り dummy */
					"0" 			/* practice mode */
					"%c"			/* final stage */
					"%8s"			/* name */
					"%09d"/*"0"*/,	/* score */
				//
					(j+'0'),		/* player number */
					(i+'0'),		/* score rank number */
									/* practice mode */
					(high_score_table[j][i].final_stage+'0'),				/* final stage */
					high_score_table[j][i].name,
					high_score_table[j][i].score
				);	write_buf(/*fp,*/fd, buf);
			}
		}
	}
	sceIoClose(fd);/*	fclose(fp);*/
}

/*---------------------------------------------------------
	画面保存機能
---------------------------------------------------------*/
extern void gu_save_screen(void);
global void save_screen_shot(void)
{
	static int screen_num = 0;
/*static*/ char screen_buf[64/*32 20*/];
//	sp rintf(screen_buf,"ms0:/PICTURE/Toho_Moho%d.bmp", screen_num);		// 保存場所の変更。
//	strcpy(screen_buf,"ms0:/PICTURE/東方模倣風/Toho_MohoZ.bmp");	//	「33文字目を強制書き換え」
//	strcpy(screen_buf,"ms0:/PICTURE/kene/MohoZ.bmp");	/* "ms0:/PICTURE/kene/"の作成方法がわからない */
//	strcpy(screen_buf,"ms0:/PICTURE/Toho_MohoZ.bmp");				//	「22文字目を強制書き換え」
	strcpy(screen_buf,"ms0:/PICTURE/東方模倣風/スクショ_Z.bmp");	//	「33文字目を強制書き換え」
//	-------------------0123456789012345678901234567890123
//	-------------------0000000000111111111122222222223333
	screen_num++;
	screen_num &= 0x1f;
	screen_buf[33/*22*/]= ((9+1)<screen_num)?(('A'-(9+1)-1)+screen_num):(('0'-1)+screen_num);
	gu_save_screen();
	SDL_SaveBMP(sdl_screen[SDL_00_VIEW_SCREEN], screen_buf);
	psp_clear_screen();
}
