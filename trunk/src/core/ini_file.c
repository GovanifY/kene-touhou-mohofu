
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

//#define FILE_NAME_SETTING_TXT "setting.ini"
#define FILE_NAME_SETTING_TXT	DIRECTRY_NAME_DATA"/setting.txt"

/*---------------------------------------------------------
	ハイスコアファイル関連処理
---------------------------------------------------------*/

/*---------------------------------------------------------
	設定ファイル関連処理
---------------------------------------------------------*/
static int ini_load_item(FILE *fp, char *search, char *result)
{
	char buffer[128];		//行取得用
	char target[32/*30*/];	//何についての情報なのか
	int int_result=-1;
	/* 走査するよ */
	fseek(fp, 0L, SEEK_SET);
	while (fgets(buffer,128,fp) != NULL)
	{
		char *c;		//行解析用
		c = buffer;
		/* skiped lines. */
		if (*c=='\n')		{	continue;	}	/* 改行のみの行は空行なのでやらないでとばす */
		while (isspace(*c)) {	c++;		}	/* 空白やTABを除去 */
		if (*c=='#')		{	continue;	}	/* #で始まる行はコメントなのでやらないでとばす */
		//
		{char *sc = target;
			int i=0;
			while (*c != '=')	/* '=' を探す */
			{
				i++;
				if (i >= 30)	{	return (-1);	}	/* 長すぎたらエラー */
				*sc++ = *c++;
			}
			c++;		/* '=' を無視 ('=' の分をとばす) */
			*sc = 0;	// '\0' を追記。  NULL
		}
		//
		if (0==tiny_strcmp(target,search))
		{
			if (NULL==result) // 対象を整数として解析する。(PARTH_MODE_INT)
			{
				char re_s[32/*30*/];
				char *re_e=re_s;
				while (*c != '\n')		/* intの方は数字じゃない物は排除してくれるみたいだから問題なし。(13でなくてok) */
				{
					*re_e++ = *c++;
				}
				re_e = 0;
				int_result = atoi(re_s);
				break;
			}
			else // 対象を文字列として解析する。 (PARTH_MODE_CHAR)
			{
				char *re_e=result;
				while (*c != 13)		/* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
				{
					*re_e++ = *c++;
				}
				re_e = 0;
				int_result = 1;
				break;
			}
		}
	}
	return (int_result);
}

//extern char str_pa ss_word[];		/* [***090222 */
extern int select_player;
static int ini_load_local(void)
{
	FILE *fp;
	char fn[64/*50*/];
	strcpy(fn,"./" FILE_NAME_SETTING_TXT);
	int tmp;
	int int_result;
	int_result = 0;/* 異常 */
	int ng1;
	ng1 = 0;/*fopen()成功*/
	fp = fopen(fn, "r");
	if ( NULL == fp)	{	ng1 = 1;/*fopen()失敗*/ goto error00;/* return (-1); */ 	}
//
//	if (ini_load_item(fp, "moddir", moddir)==-1)	{	goto error00/*return (-1)*/;	}
//	fscanf(fp, "moddir=%s",moddir);
	#define CONFIG_LOAD_ITEM(aaa,bbb) {tmp=ini_load_item(fp, aaa, NULL);	if (-1 != tmp)	{	bbb=tmp;	}	else	{	goto error00/*return -1*/;	}}

	#if 0
	CONFIG_LOAD_ITEM("SELECT",		pad_config[KEY_NUM00_SELECT]);
	CONFIG_LOAD_ITEM("START",		pad_config[KEY_NUM01_START]);
	CONFIG_LOAD_ITEM("UP",			pad_config[KEY_NUM02_UP ]);
	CONFIG_LOAD_ITEM("RIGHT",		pad_config[KEY_NUM03_RIGHT ]);
	CONFIG_LOAD_ITEM("DOWN",		pad_config[KEY_NUM04_DOWN ]);
	CONFIG_LOAD_ITEM("LEFT",		pad_config[KEY_NUM05_LEFT ]);
	CONFIG_LOAD_ITEM("L_T", 		pad_config[KEY_NUM06_L_TRIG]);
	CONFIG_LOAD_ITEM("R_T", 		pad_config[KEY_NUM07_R_TRIG]);
	CONFIG_LOAD_ITEM("TRIANGLE",	pad_config[KEY_NUM08_TRIANGLE]);
	CONFIG_LOAD_ITEM("CIRCLE",		pad_config[KEY_NUM09_CIRCLE]);
	CONFIG_LOAD_ITEM("CROSS",		pad_config[KEY_NUM10_CROSS]);
	CONFIG_LOAD_ITEM("SQUARE",		pad_config[KEY_NUM11_SQUARE]);
	#else
	CONFIG_LOAD_ITEM("K0a", 		pad_config[KEY_NUM00_SELECT]);
	CONFIG_LOAD_ITEM("K0b", 		pad_config[KEY_NUM01_START]);
	CONFIG_LOAD_ITEM("K0c", 		pad_config[KEY_NUM02_UP ]);
	CONFIG_LOAD_ITEM("K0d", 		pad_config[KEY_NUM03_RIGHT ]);
	CONFIG_LOAD_ITEM("K0e", 		pad_config[KEY_NUM04_DOWN ]);
	CONFIG_LOAD_ITEM("K0f", 		pad_config[KEY_NUM05_LEFT ]);
	CONFIG_LOAD_ITEM("K0g", 		pad_config[KEY_NUM06_L_TRIG]);
	CONFIG_LOAD_ITEM("K0h", 		pad_config[KEY_NUM07_R_TRIG]);
	CONFIG_LOAD_ITEM("K0i", 		pad_config[KEY_NUM08_TRIANGLE]);
	CONFIG_LOAD_ITEM("K0j", 		pad_config[KEY_NUM09_CIRCLE]);
	CONFIG_LOAD_ITEM("K0k", 		pad_config[KEY_NUM10_CROSS]);
	CONFIG_LOAD_ITEM("K0l", 		pad_config[KEY_NUM11_SQUARE]);
	#endif
	CONFIG_LOAD_ITEM("player",				option_config[OPTION_CONFIG_00_PLAYER]	);
	CONFIG_LOAD_ITEM("bomb",				option_config[OPTION_CONFIG_01_BOMB]	);
	CONFIG_LOAD_ITEM("bgm", 				option_config[OPTION_CONFIG_02_BGM] );
	CONFIG_LOAD_ITEM("sound",				option_config[OPTION_CONFIG_03_SOUND]	);
	CONFIG_LOAD_ITEM("current_difficulty",	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]	);
	CONFIG_LOAD_ITEM("current_player",		option_config[OPTION_CONFIG_05_CURRENT_PLAYER]	);
	CONFIG_LOAD_ITEM("analog",				option_config[OPTION_CONFIG_06_ANALOG]	);
	CONFIG_LOAD_ITEM("open",				option_config[OPTION_CONFIG_07_OPEN]	);
	difficulty		= option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY];
	select_player	= option_config[OPTION_CONFIG_05_CURRENT_PLAYER];
//	if (-1 == ini_load_item(fp, "pa ssword", st r_pass_word)) 	{	goto error00;/* return (-1); */ 	}
	int_result = 1;/* 正常 */
//
error00:
	/* high_score load */
	{	int ng2;
		int tmpscore;
		ng2 = ng1;
		tmpscore = 0;
//
		int j;
		for (j=0; j<MAX_8_SAVE_PLAYERS; j++)
		{
			int i;
			for (i=0; i<5; i++)
			{
				if (0==ng2)
				{
					char tmp_str32[64/*50*/];
					if (1 == fscanf(fp, "%23s\n", tmp_str32 ))
					{
						/* 埋め込む */
					//	strncpy(high_score_table[j][i].name, &tmp_str32[10], 3);
						high_score_table[j][i].name[0] = tmp_str32[10];
						high_score_table[j][i].name[1] = tmp_str32[11];
						high_score_table[j][i].name[2] = tmp_str32[12];
						char tmp_str16[64/*50*/];
						strcpy(tmp_str16, &tmp_str32[13]);
						tmpscore = atoi(tmp_str16);
						tmpscore /= 10;
					}
					else/* エラー */
					{
						ng2=1;
					}
				}
			//	if (1==ng2) /* pspは0レジスタがあるので0と比較したほうが速い */
				if (0!=ng2)
				{
					static const int init_score_tbl[5]=
					{
						score(50000000),
						score(4000000),
						score(300000),
						score(20000),
						score(1000),
					};
					strcpy(high_score_table[j][i].name,"ZUN"/*"DEN"*/);
					tmpscore = init_score_tbl[i];
				}
				high_score_table[j][i].score = tmpscore;
			}
		}
	//	top_score = high_score_table[0][0].score;	// 常に表示するハイコアの取得=>score.cで利用
	}
//
	if (0==ng1) {	fclose(fp);}
	return (int_result)/*1*/;
}
extern void set_default_key(int *key_setting_map, int key_setting_type);
extern	void set_default_option(int *option_setting_map);
void ini_load(void)
{
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
	{	difficulty = 2;}
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
	char fn[64/*50*/];
	strcpy(fn, "./" FILE_NAME_SETTING_TXT);
//	fp = fopen(fn, "w");
	SceUID fd = sceIoOpen(fn, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
//	if (fd < 0)
//	{
//		/*"セーブデータがない。"*/
//	}
//	if ( NULL == fp)	{	return; 	}
//
	char buf[64/*50*/];
//	fprintf(fp, "moddir=%s",	moddir);
	#if 0
	sprintf(buf, "SELECT=%d",		pad_config[KEY_NUM00_SELECT]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "START=%d",		pad_config[KEY_NUM01_START]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "UP=%d",			pad_config[KEY_NUM02_UP]);		write_buf(/*fp,*/fd, buf);
	sprintf(buf, "RIGHT=%d",		pad_config[KEY_NUM03_RIGHT]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "DOWN=%d", 		pad_config[KEY_NUM04_DOWN]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "LEFT=%d", 		pad_config[KEY_NUM05_LEFT]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "L_T=%d",			pad_config[KEY_NUM06_L_TRIG]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "R_T=%d",			pad_config[KEY_NUM07_R_TRIG]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "TRIANGLE=%d", 	pad_config[KEY_NUM08_TRIANGLE]); write_buf(/*fp,*/fd, buf);
	sprintf(buf, "CIRCLE=%d",		pad_config[KEY_NUM09_CIRCLE]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "CROSS=%d",		pad_config[KEY_NUM10_CROSS]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "SQUARE=%d",		pad_config[KEY_NUM11_SQUARE]);	write_buf(/*fp,*/fd, buf);
	#else
	sprintf(buf, "K0a=%d",			pad_config[KEY_NUM00_SELECT]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0b=%d",			pad_config[KEY_NUM01_START]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0c=%d",			pad_config[KEY_NUM02_UP]);		write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0d=%d",			pad_config[KEY_NUM03_RIGHT]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0e=%d",			pad_config[KEY_NUM04_DOWN]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0f=%d",			pad_config[KEY_NUM05_LEFT]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0g=%d",			pad_config[KEY_NUM06_L_TRIG]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0h=%d",			pad_config[KEY_NUM07_R_TRIG]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0i=%d",			pad_config[KEY_NUM08_TRIANGLE]); write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0j=%d",			pad_config[KEY_NUM09_CIRCLE]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0k=%d",			pad_config[KEY_NUM10_CROSS]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "K0l=%d",			pad_config[KEY_NUM11_SQUARE]);	write_buf(/*fp,*/fd, buf);
	#endif
	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY] = difficulty;
	option_config[OPTION_CONFIG_05_CURRENT_PLAYER] = select_player;
	sprintf(buf, "player=%d",				option_config[OPTION_CONFIG_00_PLAYER]);				write_buf(/*fp,*/fd, buf);
	sprintf(buf, "bomb=%d", 				option_config[OPTION_CONFIG_01_BOMB]);					write_buf(/*fp,*/fd, buf);
	sprintf(buf, "bgm=%d",					option_config[OPTION_CONFIG_02_BGM]);					write_buf(/*fp,*/fd, buf);
	sprintf(buf, "sound=%d",				option_config[OPTION_CONFIG_03_SOUND]); 				write_buf(/*fp,*/fd, buf);
	sprintf(buf, "current_difficulty=%d",	option_config[OPTION_CONFIG_04_CURRENT_DIFFICULTY]);	write_buf(/*fp,*/fd, buf);
	sprintf(buf, "current_player=%d",		option_config[OPTION_CONFIG_05_CURRENT_PLAYER]);		write_buf(/*fp,*/fd, buf);
	sprintf(buf, "analog=%d",				option_config[OPTION_CONFIG_06_ANALOG]);				write_buf(/*fp,*/fd, buf);
	sprintf(buf, "open=%d", 				option_config[OPTION_CONFIG_07_OPEN]);					write_buf(/*fp,*/fd, buf);
//	sprintf(buf, "pa ssword=%s", 			str_pa ss_word); 		write_buf(/*fp,*/fd, buf);
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
					"0" 			/* practice mode */
					"0" 			/* final stage */
					"=" 			/* == dummy */
					"%3s"			/* name */
					"%09d0",		/* score */
				//
					(j+'0'),		/* player number */
					(i+'0'),		/* rank number */
									/* practice mode */
									/* final stage */
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
/*static*/ char screen_buf[32/*20*/];
//	sp rintf(screen_buf,"ms0:/PICTURE/Toho_Moho%d.bmp", screen_num);		// 保存場所の変更。
	strcpy(screen_buf,"ms0:/PICTURE/Toho_MohoZ.bmp");
	screen_num++;
	screen_num &= 0x1f;
	screen_buf[22]= ((9+1)<screen_num)?(('A'-(9+1)-1)+screen_num):(('0'-1)+screen_num);
	gu_save_screen();
	SDL_SaveBMP(sdl_screen[SDL_00_SCREEN], screen_buf);
}
