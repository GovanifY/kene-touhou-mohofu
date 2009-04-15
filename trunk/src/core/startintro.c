
/*---------------------------------------------------------
	ファイル関連処理
---------------------------------------------------------*/
#include "main.h"



/*---------------------------------------------------------
	追加モジュール(MOD)ファイル関連処理
---------------------------------------------------------*/
static char option_module_files[20][20];
static int modcount=0;

static void readModDirs(void)
{
	//fprintf(stdout,"readDataDirs\n");
	DIR *dp;
	struct dirent *dir;
	if ( (dp = opendir(".")) == NULL )
	{
		fprintf(stderr, "Can't open directory: ");
		exit(1);
	}
	while ((dir = readdir(dp)) != NULL)
	{
		if (dir->d_name[0]!='_') continue;
		//fprintf(stdout,"%s\n", dir->d_name);
		strcpy(option_module_files[modcount],dir->d_name);
		modcount++;
	}
	closedir(dp);
}
static void chooseModDir(void)
{
	int i;
	int selected=0;
	SceCtrlData cpad;
	memset(moddir,0,20);

	readModDirs();
	while (strlen(moddir)<2)
	{
		pspDebugScreenClear();
		pspDebugScreenPrintf("KENE\n");		//意味の無い改変。気付いてる人いるのかな？
		pspDebugScreenPrintf("\t\t version " __DATE__  " \n\n");
		pspDebugScreenPrintf("Please choose the module directries:\n\n");
		for (i=0; i<modcount; i++)
		{
			if (i==selected)
			{	pspDebugScreenPrintf(">>> ");}
			else
			{	pspDebugScreenPrintf("    ");}
			pspDebugScreenPrintf("%s\n",option_module_files[i]);
		}
		while (1)
		{
			sceCtrlReadBufferPositive(&cpad, 1);
			if ((cpad.Buttons & PSP_CTRL_UP) || (cpad.Ly <= 0x30))
			{
				if (selected>0) selected--;
				break;
			}
			else if ((cpad.Buttons & PSP_CTRL_DOWN) || (cpad.Ly >= 0xD0))
			{
				if (selected<modcount-1) selected++;
				break;
			}
			if (cpad.Buttons & PSP_CTRL_CROSS)
			{
				strcpy(moddir,option_module_files[selected]);
				sceKernelDelayThread(100000);		//これを入れないと何故か終了する。
				break;
			}
		}
		for (i=0; i<8; i++)
		{
			sceDisplayWaitVblankStart();/*vsync*/
		}//wait();
	}
	pspDebugScreenClear();
}

/*---------------------------------------------------------
	ハイスコアファイル関連処理
---------------------------------------------------------*/
extern HSC_LIST hsc_table[/*5*/];
extern int high_score;

static void high_score_load(void)
{
	char fn[64/*50*/];
	strcpy(fn,moddir);
	strcat(fn,"/" "highscore.txt");
	int ng;
	ng=0;
	FILE *fp;
	if ( NULL == (fp = fopen(fn,"r")) )
	{
		ng=1;
	}
	int tmpscore;
	int i;
	for (i=0;i<5;i++)
	{
		if (0==ng)
		{
			if (2!=fscanf(fp,"%3s %d\n",hsc_table[i].name,&tmpscore))
			{
				ng=1;
			}
		}
		if (1==ng)
		{
			strcpy(hsc_table[i].name,"ZUN"/*"DEN"*/);
			tmpscore = (5-i)*score(10000);
		}
		hsc_table[i].score=tmpscore;
	}
	high_score = hsc_table[0].score;		//常に表示するハイコアの取得=>score.cで利用
	fclose(fp);
}

void hsc_save(void)
{
	int i;
	FILE *fp;
	char fn[64/*50*/];
//	int tmpscore;
	strcpy(fn,moddir);
	strcat(fn,"/");
	strcat(fn,"highscore.txt");

	if ( NULL == (fp = fopen(fn,"w")) )
	{
		return;
	}
	for (i=0;i<5;i++)
	{
		fprintf(fp,"%3s %d\n",hsc_table[i].name,hsc_table[i].score);
	}
	fclose(fp);
}
/*---------------------------------------------------------
	設定ファイル関連処理
---------------------------------------------------------*/
extern KEYCONFIG keyconfig;

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
				if (i >= 30)	{	return -1;	}	/* 長すぎたらエラー */
				*sc++ = *c++;
			}
			c++;		/* '=' を無視 ('=' の分をとばす) */
			*sc = 0;	// '\0' を追記。  NULL
		}
		//
		if (!strcmp(target,search))
		{
			if (NULL==result) // 対象を整数として解析する。(PARTH_MODE_INT)
			{
				char re_s[32/*30*/];
				char *re_e=re_s;
				while (*c != '\n')		/* intの方は数字じゃない物は排除してくれるみたいだから問題なし。(13でなくてok) */
				{
					*re_e++ = *c++;
				}
				re_e=0;
				int_result=atoi(re_s);
				break;
			}
			else // 対象を文字列として解析する。 (PARTH_MODE_CHAR)
			{
				char *re_e=result;
				while (*c != 13)		/* charの方は\nじゃなくて13にしないとちゃんと取ってくれないよ。(13でないとng) */
				{
					*re_e++=*c++;
				}
				re_e=0;
				int_result=1;
				break;
			}
		}
	}
	return int_result;
}


static int ini_load_local(void)
{
	FILE *fp;
	char fn[64/*50*/];
	strcpy(fn,"./setting.ini");
	int tmp;
	if ( NULL == (fp = fopen(fn,"r")))	{	return -1;	}
//
	if (ini_load_item(fp, "moddir", moddir)==-1)	{	return -1;	}
//	fscanf(fp,"moddir=%s",moddir);
	#define CONFIG_LOAD_ITEM(aaa,bbb) {tmp=ini_load_item(fp, aaa, NULL);	if (tmp!=-1)	{	bbb=tmp;	}	else	{	return -1;	}}
	CONFIG_LOAD_ITEM("difficulty",	difficulty	);
	CONFIG_LOAD_ITEM("UP",			keyconfig.u );
	CONFIG_LOAD_ITEM("DOWN",		keyconfig.d );
	CONFIG_LOAD_ITEM("LEFT",		keyconfig.l );
	CONFIG_LOAD_ITEM("RIGHT",		keyconfig.r );
	CONFIG_LOAD_ITEM("CROSS",		keyconfig.ba);
	CONFIG_LOAD_ITEM("CIRCLE",		keyconfig.ma);
	CONFIG_LOAD_ITEM("TRIANGLE",	keyconfig.sa);
	CONFIG_LOAD_ITEM("SQUARE",		keyconfig.si);
	CONFIG_LOAD_ITEM("R_T", 		keyconfig.rt);
	CONFIG_LOAD_ITEM("L_T", 		keyconfig.lt);
	CONFIG_LOAD_ITEM("SELECT",		keyconfig.sl);
	CONFIG_LOAD_ITEM("START",		keyconfig.st);
//
	if (ini_load_item(fp, "password", password)==-1)	{	return -1;	}
	fclose(fp);
//	範囲外の場合は修正
	if (difficulty>3 || 0>difficulty)/* (easy)0 1 2 3(Lunatic) */
	{	difficulty=2;}
	return 1;
}

void ini_load(void)
{
	if (ini_load_local()==-1) //090110
	{
		chooseModDir();
		keyconfig.si= KEY_BOMB; 	//□
		keyconfig.ma= KEY_CANCEL;	//○
		keyconfig.ba= KEY_SHOT; 	//×
		keyconfig.sa= KEY_SC_SHOT;	//△
		keyconfig.u = KEY_UP;		//↑
		keyconfig.d = KEY_DOWN; 	//↓
		keyconfig.l = KEY_LEFT; 	//←
		keyconfig.r = KEY_RIGHT;	//→
		keyconfig.rt= KEY_SLOW; 	//R
		keyconfig.lt= KEY_BOMB; 	//L
		keyconfig.sl= KEY_NONE; 	//SELECT
		keyconfig.st= KEY_PAUSE;	//START
	}
	high_score_load();
}

void ini_save(void)
{
	FILE *fp;
	char fn[64/*50*/];
	strcpy(fn,"./setting.ini");
	if ( NULL == (fp = fopen(fn,"w")))	{	return; 	}
//
	const char k=13;
	fprintf(fp,"moddir=%s%c\n", 	moddir, 		k);
	fprintf(fp,"difficulty=%d%c\n", difficulty, 	k);
	fprintf(fp,"UP=%d%c\n", 		keyconfig.u,	k);
	fprintf(fp,"DOWN=%d%c\n",		keyconfig.d,	k);
	fprintf(fp,"LEFT=%d%c\n",		keyconfig.l,	k);
	fprintf(fp,"RIGHT=%d%c\n",		keyconfig.r,	k);
	fprintf(fp,"CROSS=%d%c\n",		keyconfig.ba,	k);
	fprintf(fp,"CIRCLE=%d%c\n", 	keyconfig.ma,	k);
	fprintf(fp,"TRIANGLE=%d%c\n",	keyconfig.sa,	k);
	fprintf(fp,"SQUARE=%d%c\n", 	keyconfig.si,	k);
	fprintf(fp,"R_T=%d%c\n",		keyconfig.rt,	k);
	fprintf(fp,"L_T=%d%c\n",		keyconfig.lt,	k);
	fprintf(fp,"SELECT=%d%c\n", 	keyconfig.sl,	k);
	fprintf(fp,"START=%d%c\n",		keyconfig.st,	k);
	fprintf(fp,"password=%s%c\n",	password,		k);
	fclose(fp);
}

