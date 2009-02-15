#ifndef _HISCORE_H_
#define _HISCORE_H_

#include <SDL/SDL.h>
#include <math.h>

#include "font.h"
#include "sprite.h"
#include "player.h"

enum _hclist_edit_states
{
	HCLISTE_INIT,
	HCLISTE_ENTRY,
	HCLISTE_QUIT
};

typedef struct
{
	char name[4];
	int score;
} HSC_LIST;

extern void hsc_init(void);
extern void hsc_show_init(void);
extern void hsc_show_work(void);
//static void hsc_show_move(void);

extern void hsc_entry_init(void);
extern void hsc_entry_work(void);
//static void hsc_entry_show(void);

extern void hsc_save(void);
extern void hsc_load(void);
#endif
