#ifndef _LOADLV_H_
#define _LOADLV_H_

#include <SDL/SDL.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//#define  MAX_IVENT_ENTRY	512
#define  MAX_PARA1_44		/*64*/44-4/*(64-(4*5))*/

typedef struct _levelentry
{
	Uint32 time;
	Uint32 para0;	/*ctype*/	/* [***090210	’Ç‰Á */
	int done;
	int para2;
//(16[Bytes]==(4x4) )
	Uint16 para3;	/* scroll speed256 */	/* [***090210	’Ç‰Á */
	char command;
	char command_B; /*resurved now*/		/* [***090210	’Ç‰Á */
//(20[Bytes]==(4x4)+(4) )
	char para1[MAX_PARA1_44/*64*/];
//(64[Bytes]==(4x4)+(4)+(44) )
	struct _levelentry *next;
} LEVELENTRY;

#include "support.h"

enum
{
	BTYPE_00_NONE=0,
	BTYPE_01_BGPANEL1,
	BTYPE_02_BGPANEL2,
	BTYPE_03_GROUNDER,
	BTYPE_04_MAGIC_FORMATION,
};


extern /*int*/void loadlv(void/*int level*/);
//static char *loadlv_getint(char *c, int *nr);
//static char *loadlv_getstr(char *c, char *buffer);
//static void loadlv_serror(char *filename, int line_nr);
//static void loadlv_addentry(Uint32 time, char command, char *para1, int para2);
//static void loadlv_freeentry(void);

#endif
