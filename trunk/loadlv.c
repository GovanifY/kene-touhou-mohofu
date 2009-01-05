#include "loadlv.h"

extern int errno;

LEVELENTRY *leveltab=NULL;

int loadlv(int level)
{
	int line_nr=0;
	FILE *file;
	char buffer[128];		//�������郉�C���̎擾
	char *c;				//�������̃��C���̕��͗p
	int time;				//���s�R�}���h�̏o�Ă���^�C�~���O�̎擾
	char command;			//�G�Ȃ̂����b�Z�[�W�Ȃ̂�
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
		if((c=loadlv_getint(c, &time))==NULL) {		//���Ԃ̎擾
			loadlv_serror(filename,line_nr);
			continue;
		}

		if(*c++!='|') {
			loadlv_serror(filename,line_nr);
			continue;
		}

		/* Befehl */
		command=*c++;	//command���b�Z�[�W���G���̔���
		
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

	while(isdigit(*c)) {	//isdigit �������ǂ����̔���
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

void loadlv_addentry(Uint32 time, char command, char *para1, int para2)		//�I�u�W�F�N�g�̐���
{
	LEVELENTRY *new;

	new=mmalloc(sizeof(LEVELENTRY));

	new->time=time;
	new->command=command;
	strncpy(new->para1,para1,63);
	new->para2=para2;
	new->done=0;

	if(leveltab==NULL)		//�ŏ��̃��C�����H
		new->next=(LEVELENTRY *)NULL;
	else
		new->next=leveltab;		//leveltab�͑O��̃f�[�^

	leveltab=new;		//leveltab�ɍ����������f�[�^�̃A�h���X�����B
	//next�ɂ�dat�t�@�C���I�ɂ͌��ݑ������̃��C���̏�̃��C���������Ă���B
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
