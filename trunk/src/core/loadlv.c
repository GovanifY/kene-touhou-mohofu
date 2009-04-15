#include "loadlv.h"

extern int errno;

LEVELENTRY *leveltab=NULL;



static void load_stage_free_entry(void)
{
	LEVELENTRY *l = leveltab;
	LEVELENTRY *n;
	while (l!=NULL)
	{
		n=l->next;
		free(l);
		l = n;
	}
	leveltab = (LEVELENTRY *)NULL;
}
static char *load_stage_get_int(char *c, int *nr)
{
	char buffer[128];
	char *d = buffer;
	int i=0;
	while (isdigit(*c)) 	/* isdigit 数字かどうかの判定 */
	{
		i++;
		if (i >= 128)
		{	return (char *) NULL;}
		*d++ = *c++;
	}
	*d = 0;
	*nr = atoi(buffer);
	return c;
}

/* Get ascii strings. */
static char *load_stage_get_str(char *c, char *buffer)
{
	int i=0;
	while (*c != '|')
	{
		i++;
		if (i >= 128)
		{	return (char *) NULL;}
		*buffer++ = *c++;
	}
	*buffer = 0;
	return c;
}

/* Load parth error. */
static void load_stage_serror(char *load_filename, int error_line_number)
{
	error(ERR_WARN,"syntax error in levelfile '%s', line no: %d", load_filename, error_line_number);
}


/* Do set the entry. */
static void load_stage_add_entry(Uint32 time10, char command, char *para1, int para0, int para2)		//オブジェクトの生成
{
	LEVELENTRY *new_entry;

	new_entry			= mmalloc(sizeof(LEVELENTRY));
//	#if 1
	new_entry->time 	= (time10*100*1000);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/1000000[sec]==1[nsec]==nano seconds ) */
//	#else
//	/* 色々問題がある */
//	//Uint32 cv_tbl[4]={100,90,80,70,};
//	new_entry->time 	= (time10*(10-difficulty)*10/*100*/*1000);/* 読み込み後、PSPに都合の良い値に変換(1/10[sec]-> 1/1000000[sec]==1[nsec]==nano seconds ) */
//	#endif
	new_entry->command	= command;
	strncpy(new_entry->para1, para1, (MAX_PARA1_44-1)/*63*/);
	new_entry->para0	= para0;
	new_entry->para2	= para2;
	new_entry->done 	= 0;
//
	switch (new_entry->command)
	{
	case 'E':	/* add enemy */
		{
		/* 現在 23 種類 */
		#define MAX_23_CTYPE (23)
		const char *ctype_name[MAX_23_CTYPE] =
			{	NULL,		"XEV",			"CRUSHER",	"EYEFO",
				"CUBE", 	"DRAGER",		"MINE", 	"RWINGX",
				"CIR",		"ZATAK",		"CURVER",	"BADGUY",
				"PROBALL",	"PLASMABALL",	"MING", 	"GREETER",
				"SPLASH",	"FAIRY",		"GFAIRY",					//[***090124		追加	//[***090207		追加
														"BOSS01",
				"BOSS02",	"BOSS03",		"BOSS04"	//[***090207		追加
			};

			for (new_entry->para0 = /*CTYPE_00_unknown+*/(MAX_23_CTYPE-1); 0/*CTYPE_00_unknown*/ < new_entry->para0; new_entry->para0--)
			{
				if (!strcmp( new_entry->para1, (char *)&ctype_name[new_entry->para0][0] ) )
				{
					break;
				}
			} // 0/*CTYPE_00_unknown*/ == aaa );
		#undef MAX_23_CTYPE
		}
		break;
	case 'T':		/* text */
		break;
	case 'B':		/* Background */
		break;
	case 'P':
	//	new_entry->para0 = new_entry->para2/1000;
	//	new_entry->para2 = new_entry->para2%1000;
		break;
	default:	// add background tiles....
		{
			/*const*/ short speed256 = (short)(t256(1.0));
			#if 1
			switch (new_entry->command)
			{
			case '1':  speed256=(short)(t256(0.2)); break;
			case '2':  speed256=(short)(t256(0.4)); break;
			case '3':  speed256=(short)(t256(0.6)); break;
			case '4':  speed256=(short)(t256(0.8)); break;
			case '5':  speed256=(short)(t256(1.0)); break;
			case '6':  speed256=(short)(t256(1.2)); break;
			case '7':  speed256=(short)(t256(1.4)); break;
			case '8':  speed256=(short)(t256(1.6)); break;
			case '9':  speed256=(short)(t256(1.8)); break;
			default:   speed256=(short)(t256(1.0));
			}
			#else
			//	 = aaa[(new_entry->command & 0x0f)];
			#endif
			new_entry->para3 = speed256;
		}
	//	const short xxx = new_entry->para2/1000;
	//	const short yyy = new_entry->para2%1000;
	//	new_entry->para0 = xxx;
	//	new_entry->para2 = yyy;
			 if (!strcmp(new_entry->para1,"BGPANEL2"))	{	new_entry->command=BTYPE_02_BGPANEL2;	}
		else if (!strcmp(new_entry->para1,"BGPANEL"))	{	new_entry->command=BTYPE_01_BGPANEL1;	}
		else if (!strcmp(new_entry->para1,"GROUNDER"))	{	new_entry->command=BTYPE_03_GROUNDER;			}
		else if (!strcmp(new_entry->para1,"MAGICF")) 	{	new_entry->command=BTYPE_04_MAGIC_FORMATION;	}
		else											{	new_entry->command=BTYPE_00_NONE;
				error(ERR_WARN,"unknown command '%c' in levelfile",new_entry->command);}
		break;
	}
//
	if (NULL==leveltab) 	//最初のラインか？
	{	new_entry->next=(LEVELENTRY *)NULL;}
	else
	{	new_entry->next=leveltab;	}	//leveltabは前回のデータ

	leveltab=new_entry; 	//leveltabに今生成したデータのアドレスを代入。
}

extern int is_bg_add;	//[***090202		追加
extern int is_bg_end;	//[***090202		追加
extern int is_bg_fin;	//[***090202		追加
extern int n_bg;		//[***090209		追加

extern int is_panel_window;
void loadlv(void/*int level*/)		/* 元々int */
{
	is_bg_add=0;
	is_bg_end=0;
	is_bg_fin=0;
	n_bg=0;
//
	player_now_stage++;	/*(*level)++*/;
//	int level = player_now_stage;
//
	is_panel_window =1;/*パネル表示*/
	// change music soundtrack
	{
		//char filename[20];
		//sprintf(filename,"stage%d",*level);
		//playMusic(filename);
		play_music( /*level*/player_now_stage/*(*level)*/ );
	}
//
	load_stage_free_entry();
//
	char filename[128];
//	sprintf(filename,"%s/dat/level%02d.dat", moddir, /*level*/player_now_stage);
//	sprintf(filename,"%s/dat/stage%01d.txt", moddir, /*level*/player_now_stage);
	int load_stage_number = player_now_stage;
	#if (1==USE_ENDING_DEBUG)
	if (5==player_now_stage/*continue_stage*/)
	{	PLAYER_DATA *d=(PLAYER_DATA *)player->data;
		if (B07_AFTER_LOAD==d->bossmode)
		{
			load_stage_number=9;/*エンディングデバッグ用*/
		}
	}
	#endif //(1==USE_ENDING_DEBUG)
	sprintf(filename,"%s/dat/stage%c.txt", moddir, ('0'+/*level*/load_stage_number/*player_now_stage*/) );

	FILE *file;
	if (NULL==(file=fopen(filename,"r")))
	{
		error(ERR_FATAL,"can't read stage data %s\nerrno: %d (%s)",filename,errno,strerror(errno));
	}
//
	int entrys		= 0;
	int line_num	= 0;
	char buffer_text_1_line[128];	/* parth text, 1 line buffer */		/* 走査する行の取得 */
	while (fgets(buffer_text_1_line,128,file) != NULL)
	{
	int time10; 			/* 実行コマンドの出てくるタイミングの取得 */
	char char_command;		/* 敵なのかメッセージなのか */
	char para1[128];
	int para0;
	int para2;
	char *c;				/* 走査中の行の分析用 */
		line_num++;
		c = buffer_text_1_line;
//
		/* skiped lines. */
		if (*c=='\n')		{	continue;	}	/* skiped null line. */ 	/* Leerzeilen ueberspringen */
		while (isspace(*c)) {	c++;		}	/* dust left space.  */ 	/* fuehrende leerzeichen uebergehen */
		if (*c=='#')		{	continue;	}	/* skiped comment line. */	/* Kommentarzeile ? */
//
		/* parth start */	/* Startzeitpunkt holen */
		if (NULL==(c = load_stage_get_int(c, &time10))) {	load_stage_serror(filename, line_num);	continue;	}	/* load int time10 */	/* 時間の取得 */
		if (*c++ != '|')								{	load_stage_serror(filename, line_num);	continue;	}	/* load '|' */
		char_command = *c++;		/* Befehl */																	/* load 1 char commnd */		/* char_commandメッセージか敵かの判定 */
		if (*c++ != '|')								{	load_stage_serror(filename, line_num);	continue;	}	/* load '|' */
		if (NULL==(c = load_stage_get_str(c, para1)))	{	load_stage_serror(filename, line_num);	continue;	}	/* load str para1 */
		if (*c++ != '|')								{	load_stage_serror(filename, line_num);	continue;	}	/* load '|' */
		if (NULL==(c = load_stage_get_int(c, &para0)))	{	load_stage_serror(filename, line_num);	continue;	}	/* load int para0 */
		if (*c++ != '|')								{	load_stage_serror(filename, line_num);	continue;	}	/* load '|' */
		if (NULL==(c = load_stage_get_int(c, &para2)))	{	load_stage_serror(filename, line_num);	continue;	}	/* load int para2 */
		/* do set register entry. */
		#define MUSIC_CONVERT_TIME (10)
		/* ??? */
		load_stage_add_entry(MUSIC_CONVERT_TIME+time10, char_command, para1, para0, para2);
		entrys++;
	}
	fclose(file);
	//return(entrys);
	if (0==entrys)
	{
		error(ERR_WARN,"no entrys for STAGE%d.TXT",/*level*/player_now_stage);
	}
	fps_init();/* ??? auto fps初期化 */
}

