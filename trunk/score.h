#ifndef _SCORE_H_
#define _SCROE_H_

#include <SDL/SDL.h>

#include "font.h"
#include "hiscore.h"
#include "player.h"
#include "support.h"

void Player_Disp(char *img_name, int num, int x_suf, int y_suf);
void Power_Statas(int weapon, int dx, int dy);
void score_display();
void score_cleanup();
#endif
