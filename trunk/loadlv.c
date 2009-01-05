#include "loadlv.h"

extern int errno;

LEVELENTRY *leveltab=NULL;

int loadlv(int level)
{
	int line_nr=0;
	FILE *file;
	char buffer[128];		//走査するラインの取得
	char *c;				//走査中のラインの分析用
	int time;				//実行コマンドの出てくるタイミングの取得
	char command;			//敵なのかメッセージなのか
	char para1[128];
	int para2; 
	char filename[128];
	int entrys=0;

	loadlv_freeentry();
	sprintf(filename,"%s/level%02d.dat",moddir,level);

	if((file=fopen(filename,"r"))==NULL) {
		error(ERR_FATAL,"can't read levelfile %s\nerrno: %d (%s)",filename,errno,strerror(errno));
	}
		
	while(fgets(buffer,128,file)!=NULL) {

		line_nr++;
		c=buffer;

		/* Leerzeilen ueberspringen */
		if(*c=='\n')
			continue;

		/* fuehrende leerzeichen uebergehen */
		while(isspace(*c))
			c++;

		/* Kommentarzeile ? */
		if(*c=='#')
			continue;

		/* Startzeitpunkt holen */
		if((c=loadlv_getint(c, &time))==NULL) {		//時間の取得
			loadlv_serror(filename,line_nr);
			continue;
		}

		if(*c++!='|') {
			loadlv_serror(filename,line_nr);
			continue;
		}

		/* Befehl */
		command=*c++;	//commandメッセージか敵かの判定
		
		if(*c++!='|') {
			loadlv_serror(filename,line_nr);
			continue;
		}

		if((c=loadlv_getstr(c, para1))==NULL) {
			loadlv_serror(filename,line_nr);
			continue;
		}

		if(*c++!='|') {
			loadlv_serror(filename,line_nr);
			continue;
		}

		if((c=loadlv_getint(c, &para2))==NULL) {
			loadlv_serror(filename,line_nr);
			continue;
		}
		loadlv_addentry(time, command, para1, para2);
		entrys++;
	}

	fclose(file);
	return(entrys);
}

char *loadlv_getint(char *c, int *nr)
{
	char buffer[128];
	char *d=buffer;
	int i=0;

	while(isdigit(*c)) {	//isdigit 数字かどうかの判定
		i++;
		if(i>=128)
			return (char *) NULL;
		*d++=*c++;
	}
	*d=0;
	*nr=atoi(buffer);
	return c;
}

char *loadlv_getstr(char *c, char *buffer)
{
	int i=0;

	while(*c!='|') {
		i++;
		if(i>=128)
			return (char *) NULL;
		*buffer++=*c++;
	}
	*buffer=0;
	return c;
}

void loadlv_serror(char *filename, int line_nr)
{
	error(ERR_WARN,"syntax error in levelfile '%s', line no: %d",filename,line_nr);
}

void loadlv_addentry(Uint32 time, char command, char *para1, int para2)		//オブジェクトの生成
{
	LEVELENTRY *new;

	new=mmalloc(sizeof(LEVELENTRY));

	new->time=time;
	new->command=command;
	strncpy(new->para1,para1,63);
	new->para2=para2;
	new->done=0;

	if(leveltab==NULL)		//最初のラインか？
		new->next=(LEVELENTRY *)NULL;
	else
		new->next=leveltab;		//leveltabは前回のデータ

	leveltab=new;		//leveltabに今生成したデータのアドレスを代入。
	//nextにはdatファイル的には現在走査中のラインの上のラインが入っている。
}

void loadlv_freeentry()
{
	LEVELENTRY *l=leveltab,*n;

	while(l!=NULL) {
		n=l->next;
		free(l);
		l=n;
	}
	leveltab=(LEVELENTRY *)NULL;
}
