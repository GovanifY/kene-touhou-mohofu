#ifndef _BONUS_H_
#define _BONUS_H_

#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "sprite.h"
#include "support.h"
#include "player.h" 	//[***090116		í«â¡

enum
{
	SCORE_30=0,
	SCORE_50,
	SCORE_100,
	SCORE_1000,
};

#define BONUS_FLAG_NONE			0x00
#define BONUS_FLAG_COLLECT		0x01
#define BONUS_FLAG_RAND_X		0x02
#define BONUS_FLAG_RAND_Y		0x04
#define BONUS_FLAG_RAND_XY		(BONUS_FLAG_RAND_X|BONUS_FLAG_RAND_Y)


//static void bonus_move(SPRITE *src);
//static void bonus_info_move(SPRITE *src);
//static void bonus_infotext_move(SPRITE *src);

//void bonus_add(SPRITE *src/*int x, int y*/, int type, int num, int up_flags);/*îpé~*/
void bonus_multi_add(SPRITE *src/*int x, int y*/, int type, int num, int up_flags);

void bonus_info_add(SPRITE *src/*int x, int y*/, int info_type/*char *filename*/);

//extern void bonus_info_text(int x, int y, char *text, int font);/*îpé~*/
//static void bonus_info_s_text(SPRITE *src/*int x, int y*/, char *text, int font);
extern void bonus_info_score(SPRITE *src/*int x, int y*/, int score_type);
extern void bonus_info_any_score(SPRITE *src/*int x, int y*/, int score_num);


#endif
