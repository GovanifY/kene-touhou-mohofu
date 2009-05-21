#ifndef _MENU_H_
#define _MENU_H_

#include "support.h"
#include "fps.h"

/* Menues */
enum /*_menu_states_*/
{
	ST_MENU_SUB_MAIN_MENU = 0,	/*intro.c*/
	ST_MENU_SUB_PAUSE,			/*thegame.c*/
	ST_MENU_SUB_OPTION,
	//#if (1==DEBUG_MODE)
	ST_MENU_SUB_STAGE_SELECT,
	//#endif
	ST_MENU_SUB_VOLUME,
	ST_MENU_SUB_RANK_SELECT
};

extern void menusystem_init(void);
#endif /* _MENU_H_ */
