#ifndef _BONUS_H_
#define _BONUS_H_

#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "sprite.h"
#include "support.h"
#include "player.h" 	//[***090116		í«â¡

typedef struct		//[***090116		ïœçX
{
	double sum;
	double gra;
	int pl_up;
} BONUS_DATA;

typedef struct
{
	double distance;
} BIT_DATA;

enum
{
	SCORE_30=0,
	SCORE_50,
	SCORE_100,
};


void bonus_multi_add(int x, int y,int type, int num, int up);
void bonus_add(int x, int y, int type, int up);
void bonus_move(SPRITE *s);

void bonus_info_add(int x, int y, char *filename);
void bonus_info_move(SPRITE *c);

//extern void bonus_info_text(int x, int y, char *text, int font);
extern void bonus_info_s_text(SPRITE *src/*int x, int y*/, char *text, int font);
extern void bonus_info_score(SPRITE *src/*int x, int y*/, int score_type);
extern void bonus_info_any_score(SPRITE *src/*int x, int y*/, int score_num);

void bonus_infotext_move(SPRITE *c);

#endif
