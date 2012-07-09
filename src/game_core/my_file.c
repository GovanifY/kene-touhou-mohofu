
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ファイル関連処理
	-------------------------------------------------------
	ファイル関連のサポートルーチンは、ここに記述してください。

	敵出現データー(kaiwa/s/xxxx.txt)ファイル読み込み
	シナリオ会話(kaiwa/0/xxxx.txt)ファイル読み込み
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
	会話ファイルの文字列部分の読み込み
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

//atic char my_file_line_buffer256[128];		/* parth text, 1 line buffer */ 	/* 走査する行の取得 */
/*static*/ char my_file_line_buffer256[256];	// 仮想fgets()用の行バッファ		/* parth text, 1 line buffer */ 	/* 走査する行の取得 */
/*static*/ char my_file_common_name[256];		// 読み/書きする場合のファイル名	/* fopenするファイル名 (128) */
#if 1
static u32 file_size; 	// 読み込んだファイルサイズ
static u32 file_seek; 	// 仮想ファイルの読み出し位置
//static char *my_buf;
//static void *malloc_buf;
static char *malloc_buf;			// 仮想ファイルバッファ

/*---------------------------------------------------------
	指定したファイル全部を、仮想ファイルバッファに読み込みます。
	仮想ファイルの読み出し位置(file_seek)を先頭に移動させます。
---------------------------------------------------------*/

/*static*/global void *my_file_fopen(void)/*const char *my_file_common_name*/ /*, const char *dummy*/
{
	SceUID fd;
	fd = sceIoOpen((char *)my_file_common_name, PSP_O_RDONLY, 0777);	// ファイルを開く
	if (0 == fd)	// 開けなかったらエラー
	{
		goto error111;
	}
	file_size = sceIoLseek32(fd, 0, PSP_SEEK_END);	// (実シーク位置を最後に移動させて)ファイルのサイズを調べる。
	file_seek = 0;									// 仮想シーク位置を先頭に移動。

	malloc_buf = malloc(file_size); 				// ファイルのサイズ分のメモリを(取得して)確保する。
	if (NULL == malloc_buf) 						// メモリが確保出来ない場合はエラー
	{
		sceIoClose(fd);
		goto error111;
	}
	sceIoLseek32(fd, 0, PSP_SEEK_SET);				// 実シーク位置を先頭に移動。
	sceIoRead(fd, malloc_buf, file_size);			// 一気に全部メモリに読み込む。
	sceIoClose(fd);
//	my_buf = malloc_buf;
//
	return (malloc_buf);
error111:
	return (NULL);	// 失敗
}
global int my_file_now_size(void)
{
	return (file_size);
}


/*---------------------------------------------------------
	指定したメモリ(char*) に 指定した長さ読み込みます。
	仮想 file_seek を移動させます。
---------------------------------------------------------*/

/*static*/global void my_file_read_to_mem(char *write_memory, int read_length)
{
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
	if (file_seek + read_length <= file_size)
	{
		memcpy(write_memory, malloc_buf + file_seek, read_length);
		file_seek += read_length;
	}
	else
	{
		//error
	}
}

/*---------------------------------------------------------
	読み込み済みのメモリからfgets()っぽい読み込みをします。
	仮想 file_seek を移動させます。
---------------------------------------------------------*/

static int my_fgets(void)/*char *buffer_name, int num, char *wfp*/
{
	int ii;
	ii = 0;
	char aaa;
//	char bbb;
//bbb=0;
	fgets_loop:;
	aaa = /*buffer_name*/my_file_line_buffer256[ii] = malloc_buf[file_seek];/*(*my_buf)*/
//	my_buf++;
	ii++;
	file_seek++;
	if (0x0a == aaa)			{	return (1);}
	if (file_size < file_seek)	{	return (0);}/*NULL*/
	goto fgets_loop;
//	error(ERR_FATAL, "TEST %s\nno: %d (%s)",buffer_name,errno,strerror(errno));
//	return (NULL);
}

/*---------------------------------------------------------
	上記に加え、先頭が ';' で始まるものを無効にします。
---------------------------------------------------------*/

/*static*/global int my_file_fgets(void)/*char *buffer_name, int num, char *wfp*/
{
ggg_loop:;
	int ii;
	ii = my_fgets();
	/* ';'で始まる行はコメント行なので、処理しないで次の行まで飛ばす。 */
	if (COMMENT_SEMIKORON_1_CHAR == (my_file_line_buffer256[0]))
	{	goto ggg_loop;/*continue;*/ }
	return (ii);
}

/*---------------------------------------------------------
	ファイルバッファを破棄します。
---------------------------------------------------------*/

/*static*/global void my_file_fclose(void)/*void *wfp*/
{
	free(malloc_buf);
}
//(????)
	/*static*/global void my_fseek_begin(void)/*void *wfp*/
	{
	//	file_seek = 0;
	}
#else
	#define my_file_fopen	fopen
	#define my_fgets		fgets
	#define my_file_fclose	fclose
#endif

//#define FILE_NAME_SETTING_TXT "set ting.ini"
//#define FILE_NAME_SETTING_TXT 	DIRECTRY_NAME_DATA_STR "/set ting.txt"
#define FILE_NAME_SETTING_TXT		DIRECTRY_NAME_DATA_STR "/config_r35.txt"


/*---------------------------------------------------------
	ハイスコアファイル関連処理
---------------------------------------------------------*/

/*---------------------------------------------------------
	設定ファイル関連処理
	-------------------------------------------------------
	設定ファイルの数字や文字列を読みこむ。
	0以上はok
	負値はエラー
---------------------------------------------------------*/
static int ini_load_value(char *str_search, char *str_result)/*FILE *fp,*/
{
//	char my_file_line_buffer256[128];		/* 行バッファ */
	char item_name[32];/*30*/			/* コンフィグ項目名 */
//	int int_result; int_result = -1;
	/* 走査するよ */
	my_fseek_begin();//fseek(fp, 0L, SEEK_SET);
	{
		MY_FILE_LOOP_BEGIN;
		{	char *sc = item_name;
			int i=0;
			while (KUGIRI_KAMMA_1_CHAR != (*ch))	/* 区切り',' を探す */
			{
				i++;
				if (30 < i) 	{return (-1);}	/* >= 30 長すぎたらエラー */
				*sc++ = *ch++;
			}
			ch++;		/* 区切り',' を無視 (',' の分をとばす) */
			*sc = 0;	// '\0' を追記。  NULL
		}
		//
	//	error(ERR_FATAL, "test %s | %s\n", c, item_name);/* test 1\n | k0a */
		//
		if (0==tiny_strcmp(item_name, str_search))
		{
			if (NULL==str_result) // 対象を整数として解析する。(PARTH_MODE_INT) */
			{
				char tmp_str32bytes[32];/*(atoiの場合、バッファ長32[bytes]==最大31文字に統一)*/
				char *tmp_str_position = tmp_str32bytes;
			//	while ('\n' != (*ch))/* intの方は数字じゃない物は排除してくれるみたいだから問題なし。(13でなくてok) */
				while (0x0a != (*ch))/* intの方は数字じゃない物は排除してくれるみたいだから問題なし。(13でなくてok) */
				{
					*tmp_str_position++ = *ch++;
				}
				tmp_str_position = 0;
				/*int_result =*/return (atoi(tmp_str32bytes));
//				goto my_exit;
			}
			else // 対象を文字列として解析する。 (PARTH_MODE_CHAR) */
			{
				char *tmp_str_position = str_result;
			//	while (0x0d != (*ch)/*13*/) /* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
				while (0x0a != (*ch))		/* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
				{
					*tmp_str_position++ = *ch++;
				}
				tmp_str_position = 0;
				/*int_result =*/return (0);/* ok */
//				goto my_exit;
			}
		}
		MY_FILE_LOOP_CONTINUE;
		MY_FILE_LOOP_END;
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

extern void set_default_key(u32 *key_setting_map, int key_setting_type);
extern void set_default_option(int *option_setting_map);
extern void check_limit_value_option(int *option_setting_map);
global void ini_file_load(void)
{
	#if 1
	/* sceIoMkdir();でパスを作成する場合、最後が '/' だと作成されないので注意。 */
	// 要望があったので全力で対応
//	sceIoMkdir(const char *aaa_dir, SceMode mode);
	sceIoMkdir("ms0:/PICTURE", 0777);	/* "ms0:/PICTURE"がない場合、
	"ms0:/PICTURE/東方模倣風"が作れないので、必ず必要。
	ない場合、無視されるのではなくて、最悪「ハングアップしたりする」。
 */
//	sceIoMkdir("ms0:/PICTURE/東方模倣風/", 0777);/* 出来ない */
	sceIoMkdir("ms0:/PICTURE/東方模倣風", 0777);/* 出来る */
	#endif
//	char my_file_common_name[128/*64 50*/];
//	strcpy(my_file_common_name, "./" FILE_NAME_SETTING_TXT);
	strcpy(my_file_common_name, FILE_NAME_SETTING_TXT);
	int int_result;
	int_result = 0;/* 異常 */
	int ng1;
	ng1 = 0;/*fopen()成功*/
//
//	FILE *fp;
	if (NULL == /* fp =*/ my_file_fopen())	/* 開けなかったとき */		/*my_file_common_name, "r"*/
	{	/* 読み込み失敗 */
		ng1 = 1;/*fopen()失敗*/ goto error00;
	}
	/* キーコンフィグ設定の読み込み */
	{
		char key_config_tag[4];
		key_config_tag[0] = 'K';
		key_config_tag[1] = '0';
//		key_config_tag[2] = ('a'+i);
		key_config_tag[3] = 0;
		{	unsigned int i;
			for (i=0; i<KINOU_08_WARIATE_MAX; i++)
			{
				key_config_tag[2] = ('a'+i);
			//
				{	int value;
					value = ini_load_value(key_config_tag, NULL);/*fp,*/
					if (0 > value)	{goto error00;	}
					else			{pad_config[i] = value;	}
				}
			}
		}
	}
	/* オプション設定の読み込み */
	{	unsigned int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			int value;
			value = ini_load_value((char *)my_config_title[i], NULL);/*fp,*/
			if (0 > value)	{goto error00;	}
			else			{option_config[i] = value; }
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
		char search_str_name32[8];/*(検索する名前)*/
		//-----------------01234567
		strcpy(search_str_name32, "SCORE00");
		search_str_name32[7] = 0;/* 念の為 */
//
		int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{
			search_str_name32[5] = '0'+j;/* PLAYER */
			unsigned int i;
			for (i=0; i<MAX_5_RANKING; i++)
			{
				search_str_name32[6] = '0'+i;/* score rank number. 順位 */
				if (0==ng2)
				{
				//#define readed_str_name32 my_file_line_buffer256
					char readed_str_name32[64/*50*/];
				//	if (1 == fscanf(fp, "%23s\n", readed_str_name32 ))
				//	if (0 != my_fgets(/*fp, "%23s\n", readed_str_name32*/ ))
					int value;
					value = ini_load_value(search_str_name32, readed_str_name32);
					if (0 > value)	{ng2 = 1;/* エラー */}
					else
					{
						/* 埋め込む */
						high_score_table[j][i].final_stage = readed_str_name32[1/*10*/]-('0');

					//	strncpy(high_score_table[j][i].name, &readed_str_name32[10], 3);
						high_score_table[j][i].name[0] = readed_str_name32[2];/*10*/
						high_score_table[j][i].name[1] = readed_str_name32[3];/*11*/
						high_score_table[j][i].name[2] = readed_str_name32[4];/*12*/
						high_score_table[j][i].name[3] = readed_str_name32[5];/*13*/
//
						high_score_table[j][i].name[4] = readed_str_name32[6];/*14*/
						high_score_table[j][i].name[5] = readed_str_name32[7];/*15*/
						high_score_table[j][i].name[6] = readed_str_name32[8];/*16*/
						high_score_table[j][i].name[7] = readed_str_name32[9];/*16*/
						{	char tmp_str32bytes[32];/*(atoiの場合、バッファ長32[bytes]==最大31文字に統一)*/
							strcpy(tmp_str32bytes, &readed_str_name32[10]);/*5 13*/
							tmpscore = atoi(tmp_str32bytes);
						}
					}
				}
				#if (1)
			/*	else // 直前の ini_load_value() でエラーが起きた場合に初期化するので このelse無効。 */
			//	if (1==ng2) /* pspは0レジスタがあるので0と比較したほうが速い */
				if (0!=ng2)/*(エラー発生している場合、強制的にスコアテーブルを初期化する。)*/
				{
					high_score_table[j][i].final_stage = (6-i);/* 到達ステージ */
					static const int init_score_tbl[5] =
					{
						score(100000000),//score(70000000),//score(50000000),
						score( 50000000),//score(60000000),//score(4000000),
						score( 10000000),//score(50000000),//score(300000),
						score(	5000000),//score(10000000),//score(20000),
						score(	1000000),//score( 5000000),//score(1000),
					};
				//	strcpy(high_score_table[j][i].name, "12345678");
				//	strcpy(high_score_table[j][i].name, "ZUN     ");
					strcpy(high_score_table[j][i].name, "Nanashi ");
					tmpscore = init_score_tbl[i];
				}
				#endif
				high_score_table[j][i].score = tmpscore;
			}
		}
	//	top_score = high_score_table[0][0].score;	// 常に表示するハイコアの取得=>score.cで利用
	}
//
	if (0==ng1)
	{
		my_file_fclose();/*fp*/ //fclose(fp);
	}
	//return (int_result)/*1*/;
	if (0==(int_result)) // 20090110
	{
//		chooseModDir();
		set_default_key(pad_config, 0/*0==type 01 模倣風 標準*/);
		set_default_option(option_config);/* OPTION_CONFIG_00_PLAYER から OPTION_CONFIG_03_SOUND まで設定 */
	//	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= 0;
	//	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= 0;
	//	option_config[OPTION_CONFIG_06_ANALOG]				= 0;
		option_config[OPTION_CONFIG_07_OPEN]				= 0x0500;
	}
	/* 範囲外チェック(範囲外の場合は修正) */
	check_limit_value_option(option_config);/* */
	/* 読み込んだデーターで初期設定 */
	cg.game_difficulty		= option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY];
	cg_game_select_player	= option_config[OPTION_CONFIG_05_CURRENT_PLAYER];
	#if 1/*(範囲外の場合は強制的に修正する。)*/
	cg.game_difficulty		&= 0x03;/* (easy)0 1 2 3(Lunatic) */
	cg_game_select_player	&= 0x07;
	#endif
}


/*---------------------------------------------------------
	ラインバッファの内容(EOS形式)に、改行を[CR+LF]形式として付け加えます。
	その後ファイルに書き出します。
---------------------------------------------------------*/

static void write_line_buffer_to_file(SceUID fd)/*FILE *fp*/ /*, char *str_buf*/
{
	char *str_pointer;
	str_pointer = my_file_line_buffer256;
//	const char k = 13;
//	fprintf(fp, "%s%c\n", str_pointer,	k);
	unsigned int len;
	len = 0;
	/* 文字列終端を調べる。 */
	{
	string_retry:/*(もう一度)*/
		if (0 == (*str_pointer))	/* 文字列終端か。 */
		{	goto string_terminate;	}
		#if (0)/*(エラーチェック)*/
	//	if (253 < len)/* 長すぎる場合は強制打ち切り。(254==0x0d, 255==0x0a, [256]なので[255]まで使える。) */
		if (250 < len)/* テキトー */
		{	goto string_terminate;	}
		#endif
		len++;
		str_pointer++;
		goto string_retry;
	string_terminate:/*(調査完了)*/
		;
	}
	/* 差分氏模倣風のファイル形式が[CR+LF]だったので互換を取る。 */
	*str_pointer = 0x0d;	len++;		str_pointer++;/*[CR]*/
	*str_pointer = 0x0a;	len++;		str_pointer++;/*[LF]*/
//	fprintf(fp, "%s\n", str_pointer);
	sceIoWrite(fd, my_file_line_buffer256, len);/*sizeof(int)*/ /**save_data_size*/ /*, 1, fp*/
}
global void ini_file_save(void)
{
//	FILE *fp;
//	char buf[128];/*64 50*/
	strcpy(my_file_common_name, "./" FILE_NAME_SETTING_TXT);
//	fp = fopen(buf, "w");
	SceUID fd = sceIoOpen((char *)my_file_common_name, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
//	if (0 > fd)
//	{
//		/*"セーブデータがない。"*/
//	}
//	if (NULL == fp) 	{	return; 	}
//
	/* 巧くいくけど off. */
	#define USE_MEMO (0)
	#if (1==USE_MEMO)

	// 'CR'=='\r' というのは carrige Return (改行)の省略形。 '\r' の意味は(改行)コード。
	// 'LF'=='\n' というのは New line (新行)の省略形。 '\n' の意味は(新規に次の行にする)コード。
	// 'LF' は、 Line Feed (行変え)の省略形。行を変えるのだから、意味は新行と全く同じ。
	// これは元々端末用語ではなく、プリンタ(印字)用語。これが端末(Terminal)用語に継承された。
	//---------------------------
	// MS-DOS/Windows では、テキストファイルの行末に[CR+LF]形式を採用している。
	// "メモ帳"やms-dosの"edit"で新規ファイルを作成し、適当に改行を入れて保存するとこの形式。
	//---------------------------
	// Unix(Linux/FreeBSD等)では、テキストファイルの行末に[LF]形式を採用している。
	// "vi"等"editer"で新規ファイルを作成し、適当に改行を入れて保存するとこの形式。
	//---------------------------
	// インターネットのサーバーでは、標準的にテキストファイルの行末に[LF]形式を採用している。
	//---------------------------
	// [Windows系 PC]->[サーバー]のupload時に、テキストファイルを[CR+LF]->[LF]変換し、
	// [Windows系 PC]<-[サーバー]のdownload時に、テキストファイルを[LF]->[CR+LF]変換している。
	// サーバーの相手が[Unix系 PC]の場合は、何も変換しない。
	//---------------------------
	// 但し、テキストファイルでないものをテキストファイルと認識した場合、変換機能があるとファイルが破壊される為、
	// テキストファイルを強制的に"バイナリファイル"として扱い、変換しないサーバーもある。
	// 逆に明らかに".png"画像なのに、(Windows系 PCでdownload時に)勝手に[LF]->[CR+LF]形式に変換し、
	// 読めなくなっちゃうサーバーもある。(もちろん管理者の設定が悪い)。何処とは言わないが。
	//---------------------------
	#define KAIGYOU_CR_LF "\r\n"
	strcpy(my_file_line_buffer256,
		";-------------------------------------" KAIGYOU_CR_LF
		"; 東方模倣風(r35) configuration file."  KAIGYOU_CR_LF
		";-------------------------------------" KAIGYOU_CR_LF
		KAIGYOU_CR_LF
		"; キーコンフィグ設定" KAIGYOU_CR_LF
	);
	write_line_buffer_to_file(fd);/*fp*/
	#endif	/* (USE_MEMO) */
	//
	{
		unsigned int i;
		for (i=0; i<KINOU_08_WARIATE_MAX; i++)
		{
			sprintf(my_file_line_buffer256,
				"K0%c,%d",
				('a'+i),
				(signed)pad_config[i]);
			write_line_buffer_to_file(fd);/*fp*/
		}
	}
	//---------------------------
	#if (1==USE_MEMO)
	#define TITLE_OPTION_SETTEI 	KAIGYOU_CR_LF "; オプション設定" KAIGYOU_CR_LF
	strcpy(my_file_line_buffer256, TITLE_OPTION_SETTEI );
	write_line_buffer_to_file(fd);/*fp*/
	#endif	/* (USE_MEMO) */
	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	= (cg.game_difficulty);
	option_config[OPTION_CONFIG_05_CURRENT_PLAYER]		= (cg_game_select_player);

	{
		unsigned int i;
		for (i=0; i<OPTION_CONFIG_08_MAX; i++)
		{
			sprintf(my_file_line_buffer256,
				"%s,%d",
				my_config_title[i],
				option_config[i]);
			write_line_buffer_to_file(fd);/*fp*/
		}
	}
	//---------------------------
	#if (1==USE_MEMO)
	#define TITLE_YUME_NO_KIROKU	KAIGYOU_CR_LF "; 夢の記録" KAIGYOU_CR_LF
	strcpy(my_file_line_buffer256, TITLE_YUME_NO_KIROKU );
	write_line_buffer_to_file(fd);/*fp*/
	#endif	/* (USE_MEMO) */
	/* high_score save */
	{	unsigned int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{	unsigned int i;
			for (i=0; i<MAX_5_RANKING; i++)
			{
				sprintf(my_file_line_buffer256,
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
					(high_score_table[j][i].final_stage+'0'),/* final stage */
					high_score_table[j][i].name,
					(int)high_score_table[j][i].score/* gcc 4.3.5 */
				);
				write_line_buffer_to_file(fd);/*fp*/
			}
		}
	}
	sceIoClose(fd);/*fclose(fp);*/
}

/*---------------------------------------------------------
	画面保存機能
---------------------------------------------------------*/

global void save_screen_shot(void)
{
	static int screen_num = 0;
//	sp rintf(my_file_common_name, "ms0:/PICTURE/Toho_Moho%d.bmp", screen_num);	// 保存場所の変更。
//	strcpy(my_file_common_name, "ms0:/PICTURE/東方模倣風/Toho_MohoZ.bmp");	//	「33文字目を強制書き換え」
//	strcpy(my_file_common_name, "ms0:/PICTURE/kene/MohoZ.bmp"); /* "ms0:/PICTURE/kene/"の作成方法がわからない */
//	strcpy(my_file_common_name, "ms0:/PICTURE/Toho_MohoZ.bmp"); 			//	「22文字目を強制書き換え」
	strcpy(my_file_common_name, "ms0:/PICTURE/東方模倣風/スクショ_Z.bmp");	//	「33文字目を強制書き換え」
//	-----------------------------0123456789012345678901234567890123
//	-----------------------------0000000000111111111122222222223333
	screen_num++;
	screen_num &= 0x1f;
	my_file_common_name[33/*22*/]= ((9+1)<screen_num)?(('A'-(9+1)-1)+screen_num):(('0'-1)+screen_num);
	#if (0==USE_32BIT_DRAW_MODE)
	PSPL_save_vram16_to_file(my_file_common_name);	/*(画面モードが16bitの場合に画面をセーブ)*/
	#else
	/*(作ってない)*/								/*(画面モードが32bitの場合に画面をセーブ)*/
	#endif
}
