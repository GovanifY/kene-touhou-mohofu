#ifndef _MENU_H_
#define _MENU_H_

#include "game_main.h"
//#include "fps.h"

/* Menues */
enum /*_menu_states_*/
{
	ST_MENU_SUB_MAIN_MENU = 0,	/*intro.c*/
	ST_MENU_SUB_PAUSE,			/*thegame.c*/
	ST_MENU_SUB_ASK_CONTINUE,	/*thegame.c*/
//	ST_MENU_SUB_OPTION,
	ST_MENU_SUB_STAGE_SELECT,
//	ST_MENU_SUB_VOLUME,
//	ST_MENU_SUB_ZANKI,
	ST_MENU_SUB_RANK_SELECT
};

extern void menusystem_init(void);

/* キャンセル音を鳴らしてメニューに強制復帰 */
extern void menu_cancel_and_voice(void);

#endif /* _MENU_H_ */
