#define	FONTWIDTH	8
#define	FONTHEIGHT	16
#include "support.h"

void mh_print(SDL_Surface *src,SDL_Rect *rect,int col,const char *str);
int mh_print_fps(SDL_Surface *src,int col, SDL_Rect *rect,const char *str,int wait);
