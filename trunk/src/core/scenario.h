#ifndef _SCENARIO_H_
#define _SCENARIO_H_

#include "support.h"

//static extern int thescript(void);
extern int script_init(char *filename, char *bg_name,int width);

/* SDL_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */
extern void script_system_init(void);/* �g�ݍ��� */
extern void script_system_exit(void);/* �O�� */

/* ���̏��K�v�Ȃ����ǁA�O������`���Ȃ�B */
//static extern int print_kanji(SDL_Surface *drawmap, SDL_Rect *rect, const char *str, int color_type, int wait);

#endif /* _SCENARIO_H_ */
