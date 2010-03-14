


/*
ファイル関連のサポートルーチンを他に書くと遅くなるので
ファイル関連のサポートルーチンは、ここに記述してください。

ファイル関連のサポートルーチン：
	追加モジュール(MOD)ファイル読み込み
	ハイスコアファイル読み込み／保存
	設定読み込み／保存
	画面保存
*/


/*---------------------------------------------------------
	ファイル関連処理
---------------------------------------------------------*/

#include "game_main.h"
#include "name_entry.h"/**/

#include "my_file.h"



/*---------------------------------------------------------
	shift jisコード、全角1バイト目かどうか判定する子関数
---------------------------------------------------------*/

/*static*/ int is_kanji_1st(/*unsigned char*/int/*int*/ high_byte)
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
/*static*/ char *load_my_file_get_str(char *str, char *buffer, int *end_arg/*, const char kugiri_1_char*/)
{
	int string_error_limiter;
	string_error_limiter = 200;/* 200 文字以上はエラー */
	/*unsigned char*/int high_byte;
	{loop:;
		high_byte = ((*str) & 0xff);
		/* 空文字列の可能性があるから、始めに判定 */
		if (/*'|' ','*/','/*kugiri_1_char*/ == high_byte)	/* ','区切りでおしまいの場合 */
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
/*static*/ char my_fopen_file_name[128];	/* fopenするファイル名 */
#if 1
static unsigned long file_size;
static unsigned long file_seek;
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;

/*static*/ void *my_fopen(void/*const char *my_fopen_file_name*/ /*, const char *dummy*/)
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
/*static*/ int my_file_fgets(void/*char *buffer_name, int num, char *wfp*/)
{
ggg_loop:;
	int ii;
	ii = my_fgets();
			/* ';'で始まる行はコメント行なので、次の行まで飛ばす。 */
	if (';'==(buffer_text_1_line[0]))		{	goto ggg_loop;/*continue;*/ }	/* ';'で始まる行はコメントなのでやらないでとばす */
	return (ii);
}
//1968725
/*static*/ void my_fclose(void/*void *wfp*/)
{
	free(malloc_buf);
}
/*static*/ void my_fseek_begin(void/*void *wfp*/)
{
//	file_seek = 0;
}
#else
	#define my_fopen	fopen
	#define my_fgets	fgets
	#define my_fclose	fclose
#endif

//#define FILE_NAME_SETTING_TXT "setting.ini"
#define FILE_NAME_SETTING_TXT	DIRECTRY_NAME_DATA"/setting.txt"

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
//	int int_result;	int_result = -1;
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
			while (isspace(*ch)) 	{	ch++;					}	/* 空白やTABを除去 */
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
//			if (';'==(*ch))		{	goto loop;/*continue;*/ }	/* ';'で始まる行はコメントなのでやらないでとばす */
			//
		//	error(ERR_FATAL, "test %s\n", c );/*test k0a,1 */
			//
			{char *sc = item_name;
				int i=0;
				while (',' != (*ch))	/* 区切り',' を探す */
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
				//	while (0x0d != (*ch)/*13*/)	/* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
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
static int ini_load_local(void)
{
//	FILE *fp;
//	char my_fopen_file_name[128/*64 50*/];
//	strcpy(my_fopen_file_name, "./" FILE_NAME_SETTING_TXT);
	strcpy(my_fopen_file_name, FILE_NAME_SETTING_TXT);
	int tmp;
	int int_result;
	int_result = 0;/* 異常 */
	int ng1;
	ng1 = 0;/*fopen()成功*/
;
//	if ( NULL == /* fp =*/ fopen(my_fopen_file_name, "r"))
	if ( NULL == /* fp =*/ my_fopen(/*my_fopen_file_name, "r"*/))
	{
		ng1 = 1;/*fopen()失敗*/ goto error00;/* return (-1); */
	}
//
//	if (ini_load_item(/*fp,*/ "moddir", moddir)==-1)	{	goto error00/*return (-1)*/;	}
//	fscanf(/*fp,*/ "moddir=%s",moddir);
	#define CONFIG_LOAD_ITEM(aaa,bbb) \
		{\
			tmp = ini_load_item(/*fp,*/ aaa, NULL);\
			if (-1 != tmp)	{	bbb = tmp;	}\
			else			{	goto error00/*return (-1)*/;	}\
		}

	#if 0
	CONFIG_LOAD_ITEM("SELECT",		pad_config[KEY_NUM00_SELECT]);
	CONFIG_LOAD_ITEM("START",		pad_config[KEY_NUM01_START]);
	CONFIG_LOAD_ITEM("UP",			pad_config[KEY_NUM02_UP    ]);
	CONFIG_LOAD_ITEM("RIGHT",		pad_config[KEY_NUM03_RIGHT ]);
	CONFIG_LOAD_ITEM("DOWN",		pad_config[KEY_NUM04_DOWN  ]);
	CONFIG_LOAD_ITEM("LEFT",		pad_config[KEY_NUM05_LEFT  ]);
	CONFIG_LOAD_ITEM("L_T", 		pad_config[KEY_NUM06_L_TRIG]);
	CONFIG_LOAD_ITEM("R_T", 		pad_config[KEY_NUM07_R_TRIG]);
	CONFIG_LOAD_ITEM("TRIANGLE",	pad_config[KEY_NUM08_TRIANGLE]);
	CONFIG_LOAD_ITEM("CIRCLE",		pad_config[KEY_NUM09_CIRCLE]);
	CONFIG_LOAD_ITEM("CROSS",		pad_config[KEY_NUM10_CROSS]);
	CONFIG_LOAD_ITEM("SQUARE",		pad_config[KEY_NUM11_SQUARE]);
	#else
	CONFIG_LOAD_ITEM("K0a", 		pad_config[KEY_NUM00_SELECT]);
	CONFIG_LOAD_ITEM("K0b", 		pad_config[KEY_NUM01_START]);
	CONFIG_LOAD_ITEM("K0c", 		pad_config[KEY_NUM02_UP    ]);
	CONFIG_LOAD_ITEM("K0d", 		pad_config[KEY_NUM03_RIGHT ]);
	CONFIG_LOAD_ITEM("K0e", 		pad_config[KEY_NUM04_DOWN  ]);
	CONFIG_LOAD_ITEM("K0f", 		pad_config[KEY_NUM05_LEFT  ]);
	CONFIG_LOAD_ITEM("K0g", 		pad_config[KEY_NUM06_L_TRIG]);
	CONFIG_LOAD_ITEM("K0h", 		pad_config[KEY_NUM07_R_TRIG]);
	CONFIG_LOAD_ITEM("K0i", 		pad_config[KEY_NUM08_TRIANGLE]);
	CONFIG_LOAD_ITEM("K0j", 		pad_config[KEY_NUM09_CIRCLE]);
	CONFIG_LOAD_ITEM("K0k", 		pad_config[KEY_NUM10_CROSS]);
	CONFIG_LOAD_ITEM("K0l", 		pad_config[KEY_NUM11_SQUARE]);
	#endif
	{
		int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			CONFIG_LOAD_ITEM( (char *)my_config_title[i],				option_config[i]	);
		}
	}
//	CONFIG_LOAD_ITEM("player",				option_config[OPTION_CONFIG_00_PLAYER]	);
//	CONFIG_LOAD_ITEM("bomb",				option_config[OPTION_CONFIG_01_BOMB]	);
//	CONFIG_LOAD_ITEM("bgm", 				option_config[OPTION_CONFIG_02_BGM] );
//	CONFIG_LOAD_ITEM("sound",				option_config[OPTION_CONFIG_03_SOUND]	);
//	CONFIG_LOAD_ITEM("current_difficulty",	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	);
//	CONFIG_LOAD_ITEM("current_player",		option_config[OPTION_CONFIG_05_CURRENT_PLAYER]	);
//	CONFIG_LOAD_ITEM("analog",				option_config[OPTION_CONFIG_06_ANALOG]	);
//	CONFIG_LOAD_ITEM("open",				option_config[OPTION_CONFIG_07_OPEN]	);
	difficulty		= option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY];
	select_player	= option_config[OPTION_CONFIG_05_CURRENT_PLAYER];
//	if (-1 == ini_load_item(/*fp,*/ "pa ssword", st r_pass_word))	{	goto error00;/* return (-1); */ 	}
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
						ng2=1;
					}
				}
			/*	else // 直前の ini_load_item() でエラーが起きた場合に初期化するので このelse無効。 */
			//	if (1==ng2) /* pspは0レジスタがあるので0と比較したほうが速い */
				if (0!=ng2)
				{
					high_score_table[j][i].final_stage = (6-i); 	/* 到達ステージ */
					static const int init_score_tbl[5]=
					{
						score(100000000),	//	score(70000000),		//score(50000000),
						score( 50000000),	//	score(60000000),		//score(4000000),
						score( 10000000),	//	score(50000000),		//score(300000),
						score(  5000000),	//	score(10000000),		//score(20000),
						score(  1000000),	//	score( 5000000),		//score(1000),
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
	return (int_result)/*1*/;
}
extern void set_default_key(int *key_setting_map, int key_setting_type);
extern	void set_default_option(int *option_setting_map);
void ini_load(void)
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
//
	if (0==ini_load_local()) // 090110
	{
//		chooseModDir();
		#if 1
		set_default_key(pad_config, 0/*0==type 01 模倣風 標準*/);
		#else
	//	pad_config[KEY_NUM00_SELECT]	= KINOU_01_SELECT;		// SELECT
	//	pad_config[KEY_NUM01_START] 	= KINOU_02_PAUSE;		// START
	//	pad_config[KEY_NUM02_UP]		= KINOU_03_UP;			// ↑
	//	pad_config[KEY_NUM03_RIGHT] 	= KINOU_04_RIGHT;		// →
	//	pad_config[KEY_NUM04_DOWN]		= KINOU_05_DOWN;		// ↓
	//	pad_config[KEY_NUM05_LEFT]		= KINOU_06_LEFT;		// ←
	//	pad_config[KEY_NUM06_L_TRIG]	= KINOU_07_SNAP_SHOT;	// L
	//	pad_config[KEY_NUM07_R_TRIG]	= KINOU_09_SLOW;		// R
	//	pad_config[KEY_NUM08_TRIANGLE]	= KINOU_09_SLOW;		// △
	//	pad_config[KEY_NUM09_CIRCLE]	= KINOU_10_OPTION;		// ○
	//	pad_config[KEY_NUM10_CROSS] 	= KINOU_11_SHOT;		// ×
	//	pad_config[KEY_NUM11_SQUARE]	= KINOU_12_BOMB;		// □
		#endif
		set_default_option(option_config);/* OPTION_CONFIG_00_PLAYER から OPTION_CONFIG_03_SOUND まで設定 */
	//	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= 0;
	//	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= 0;
	//	option_config[OPTION_CONFIG_06_ANALOG]				= 0;
		option_config[OPTION_CONFIG_07_OPEN]				= 0x0500;
	}
//	範囲外の場合は修正
	if (difficulty>3 || 0>difficulty)/* (easy)0 1 2 3(Lunatic) */
	{	difficulty = 0/* 0==easy 2*/;}
}

//2017385 2018189
static void write_buf(/*FILE *fp*/SceUID fd, char *str_buf)
{
//	const char k = 13;
//	fprintf(fp, "%s%c\n", str_buf,	k);
	char *str_buf_head;
	str_buf_head = str_buf;
	int len;
	len=0;
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
void ini_save(void)
{
//	FILE *fp;
	char filename[128/*64 50*/];
	strcpy(filename, "./" FILE_NAME_SETTING_TXT);
//	fp = fopen(filename, "w");
	SceUID fd = sceIoOpen(filename, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
//	if (fd < 0)
//	{
//		/*"セーブデータがない。"*/
//	}
//	if ( NULL == fp)	{	return; 	}
//
	char buf[64/*50*/];
//	fprintf(fp, "moddir=%s",	moddir);
//	#if 0
//	sprintf(buf, "SELECT,%d",		pad_config[KEY_NUM00_SELECT]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "START,%d",		pad_config[KEY_NUM01_START]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "UP,%d",			pad_config[KEY_NUM02_UP]);			write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "RIGHT,%d",		pad_config[KEY_NUM03_RIGHT]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "DOWN,%d", 		pad_config[KEY_NUM04_DOWN]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "LEFT,%d", 		pad_config[KEY_NUM05_LEFT]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "L_T,%d",			pad_config[KEY_NUM06_L_TRIG]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "R_T,%d",			pad_config[KEY_NUM07_R_TRIG]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "TRIANGLE,%d", 	pad_config[KEY_NUM08_TRIANGLE]);	write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "CIRCLE,%d",		pad_config[KEY_NUM09_CIRCLE]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "CROSS,%d",		pad_config[KEY_NUM10_CROSS]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "SQUARE,%d",		pad_config[KEY_NUM11_SQUARE]);		write_buf(/*fp,*/fd, buf);
//	#else
//	sprintf(buf, "K0a,%d",			pad_config[KEY_NUM00_SELECT]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0b,%d",			pad_config[KEY_NUM01_START]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0c,%d",			pad_config[KEY_NUM02_UP]);			write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0d,%d",			pad_config[KEY_NUM03_RIGHT]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0e,%d",			pad_config[KEY_NUM04_DOWN]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0f,%d",			pad_config[KEY_NUM05_LEFT]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0g,%d",			pad_config[KEY_NUM06_L_TRIG]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0h,%d",			pad_config[KEY_NUM07_R_TRIG]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0i,%d",			pad_config[KEY_NUM08_TRIANGLE]);	write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0j,%d",			pad_config[KEY_NUM09_CIRCLE]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0k,%d",			pad_config[KEY_NUM10_CROSS]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "K0l,%d",			pad_config[KEY_NUM11_SQUARE]);		write_buf(/*fp,*/fd, buf);
//	#endif
	{
		int i;
		for (i=0; i<KEY_NUM12_MAX; i++)
		{
			sprintf(buf,
				"K0%c,%d",
				('a'+i),
				pad_config[i]);
				write_buf(/*fp,*/fd, buf);
		}
	}
	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= difficulty;
	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= select_player;

	{
		int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			sprintf(buf,
				"%s,%d",
				my_config_title[i],
				option_config[i]);
				write_buf(/*fp,*/fd, buf);
		}
	}
//	sprintf(buf, "player,%d",				option_config[OPTION_CONFIG_00_PLAYER]);				write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "bomb,%d", 				option_config[OPTION_CONFIG_01_BOMB]);					write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "bgm,%d",					option_config[OPTION_CONFIG_02_BGM]);					write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "sound,%d",				option_config[OPTION_CONFIG_03_SOUND]); 				write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "current_difficulty,%d",	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]);	write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "current_player,%d",		option_config[OPTION_CONFIG_05_CURRENT_PLAYER]);		write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "analog,%d",				option_config[OPTION_CONFIG_06_ANALOG]);				write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "open,%d", 				option_config[OPTION_CONFIG_07_OPEN]);					write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "pa ssword,%s",			str_pa ss_word);		write_buf(/*fp,*/fd, buf);
	/* high_score save */
	{int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{	int i;
			for (i=0; i<5; i++)
			{
				sprintf(buf,
					"SCORE" 		/* == dummy */
					"%c"			/* player number */
					"%c"			/* rank number */
					"," 			/* == 区切り dummy */
					"0" 			/* practice mode */
					"%c" 			/* final stage */
					"%8s"			/* name */
					"%09d"/*"0"*/,		/* score */
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
void save_screen_shot(void)
{
	static int screen_num = 0;
/*static*/ char screen_buf[64/*32 20*/];
//	sp rintf(screen_buf,"ms0:/PICTURE/Toho_Moho%d.bmp", screen_num);		// 保存場所の変更。
//	strcpy(screen_buf,"ms0:/PICTURE/東方模倣風/Toho_MohoZ.bmp");	//	「33文字目を強制書き換え」
//	strcpy(screen_buf,"ms0:/PICTURE/kene/MohoZ.bmp");	/* "ms0:/PICTURE/kene/"の作成方法がわからない */
//	strcpy(screen_buf,"ms0:/PICTURE/Toho_MohoZ.bmp");				//	「22文字目を強制書き換え」
	strcpy(screen_buf,"ms0:/PICTURE/東方模倣風/スクショ_Z.bmp"); 	//	「33文字目を強制書き換え」
//	-------------------0123456789012345678901234567890123
//	-------------------0000000000111111111122222222223333
	screen_num++;
	screen_num &= 0x1f;
	screen_buf[33/*22*/]= ((9+1)<screen_num)?(('A'-(9+1)-1)+screen_num):(('0'-1)+screen_num);
	gu_save_screen();
	SDL_SaveBMP(sdl_screen[SDL_00_VIEW_SCREEN], screen_buf);
}
