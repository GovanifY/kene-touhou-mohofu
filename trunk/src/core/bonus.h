#ifndef _BONUS_H_
#define _BONUS_H_

#include "game_main.h"

enum
{
	SCORE_76800 = 0,
	SCORE_10,		/* (MAX時以外の) [P]は10点 */
	SCORE_20,
	SCORE_30,
//
	SCORE_40,
	SCORE_50,
	SCORE_60,
	SCORE_70,
//
	SCORE_80,
	SCORE_90,
	SCORE_100,
	SCORE_200,
//
	SCORE_300,
	SCORE_400,
	SCORE_500,
	SCORE_600,
//
	SCORE_700,
	SCORE_800,
	SCORE_900,
	SCORE_1000,
//
	SCORE_2000,
	SCORE_3000,
	SCORE_4000,
	SCORE_5000,
//
	SCORE_6000,
	SCORE_7000,
	SCORE_8000,
	SCORE_9000,
//
	SCORE_10000,
	SCORE_11000,
	SCORE_12000,
	SCORE_51200/*最大値*/
};

#define ITEM_MOVE_FLAG_00_NONE  		0x00
#define ITEM_MOVE_FLAG_01_COLLECT		0x01
#define ITEM_MOVE_FLAG_02_RAND_X		0x02
#define ITEM_MOVE_FLAG_04_RAND_Y		0x04
#define ITEM_MOVE_FLAG_06_RAND_XY		(ITEM_MOVE_FLAG_02_RAND_X|ITEM_MOVE_FLAG_04_RAND_Y)

#define ITEM_CREATE_MODE_01 	((4*8/*ITEM_MAX*/)*0)
#define ITEM_CREATE_MODE_02 	((4*8/*ITEM_MAX*/)*1)

extern void item_create(SPRITE *src/*int x, int y*/, int type, int num, int up_flags);
extern void item_create_for_boss(SPRITE *src, int item_create_mode);

extern void bonus_info_score_nodel(SPRITE *src/*int x, int y*/, int score_type);
extern void bonus_info_any_score_nodel(SPRITE *src/*int x, int y*/, int score_num);
//extern void item_from_bullets(int put_item_num);
extern void bullets_to_hosi(void);

/* スコア加算する場合は、カンスト チェックがあるので、必ずここを使う */
extern void player_add_score(int score_num);
#if (1==USE_EXTEND_CHECK)
extern void player_check_extend_score(void);/* エクステンドチェック */
#endif /* (1==USE_EXTEND_CHECK) */

/* 難易度スコア補正 */
extern int adjust_score_by_difficulty(int convert_score);


//extern void bonus_add(SPRITE *src/*int x, int y*/, int type, int num, int up_flags);/*廃止*/
//extern void bonus_info_text(int x, int y, char *text, int font);/*廃止*/
//static void bonus_info_s_text(SPRITE *src/*int x, int y*/, char *text, int font);

#endif /* _BONUS_H_ */
