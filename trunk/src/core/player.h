#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL/SDL.h>
#include <math.h>

#include "support.h"
#include "sprite.h"
//#include "particle.h"
#include "enemy.h"


enum _select_pl 		//[***090203		’Ç‰Á
{
	REIMU=0,
	MARISA,
	REMIRIA
};

enum _boss_mode 		//[***090223		’Ç‰Á
{
	B_NONE=0,
	B_BATTLE,
	B_DEATH,
	B_EVENT,
	B_CHANGE
};

typedef struct
{
	int lives;
	int bombs;
	int graze;
	int score;
//	int now_stage/*level*/;
	int bossmode;
	int state;
	int explode;
	double save_delay;
	double anim_delay;
	int weapon;
	int player_speed;
	int player_speed_mini;
	int player_speed_max;
	int extra;
	int bonus;				//[***090116		’Ç‰Á
	int hit_bomb_wait;		//[***090125		’Ç‰Á
	int option;
	double extra_wait;
	double extra_time;
	double weapon_wait;
	SPRITE *core;
	SPRITE *enemy;			//[***090125		’Ç‰Á:player‚É“–‚½‚Á‚½•¨
	SPRITE *boss;			//[***090305		’Ç‰Á
} PLAYER_DATA;

enum _weapon_type
{
	WP_SINGLE=0,
	WP_DOUBLE,
	WP_TRIPLE,
	WP_QUAD,
	WP_FIFTH,
	WP_LAST
};

enum _player_extras
{
	PLX_NONE=0,
	PLX_HOMING,
	PLX_SHIELD,
	PLX_HLASER,
	PLX_BOMB,
	PLX_LAST
};

typedef struct
{
	int strength;
} WEAPON_BASE;

extern void player_init(void);


//static void player_move_core(SPRITE *s2);
//static void player_move_fireball(SPRITE *s);
//static void player_move_plasma(SPRITE *s);
//static void player_move_killray(SPRITE *s);
//static void player_move_bomb(SPRITE *s);
//static int search_enemy();
//static void player_move_homing(SPRITE *s);
//static void player_move_shield(SPRITE *s);
//static void player_move_shield2(SPRITE *s);
//static void player_move_levarie(SPRITE *s);
//static void player_move_hlaser(SPRITE *s);

//static void player_move(SPRITE *s);
//static void player_keycontrol(SPRITE *s);
//static void player_colcheck(SPRITE *s, int mask);

//static void weapon_check(int w);

//static SPRITE *player_add_core(SPRITE *s1);

//static void re_move_option(SPRITE *s);
//static void ma_move_option(SPRITE *s);

//static void re_add_option(SPRITE *s);
//static void ma_add_option(SPRITE *s);

//static void player_add_doublefireball(SPRITE *player);
//static void player_add_triplefireball(SPRITE *player);
//static void player_add_fifthfireball(SPRITE *player);

//static void re_add_fireball(SPRITE *s, int x, int y);
//static void ma_add_fireball(SPRITE *s, int x, int y);

//static void player_controller_hlaser(CONTROLLER *c);

//static void player_add_doublefireball_ma(SPRITE *player);
//static void player_add_triplefireball_ma(SPRITE *player);
//static void player_add_fifthfireball_ma(SPRITE *player);
//static void player_add_fuda(SPRITE *player);
//static void player_add_doublefuda(SPRITE *player);
//static void player_add_triplefuda(SPRITE *player);
//static void player_add_quadfuda(SPRITE *player);
//static void player_add_fifthfuda(SPRITE *player);

//static void player_add_star(SPRITE *player);
//static void player_add_doublestar(SPRITE *player);
//static void player_add_triplestar(SPRITE *player);
//static void player_add_quadstar(SPRITE *player);

//static void player_add_killray(SPRITE *player);
//static void player_add_bomb(SPRITE *s);
//static void player_add_homing(SPRITE *s);
//static void player_add_shield(SPRITE *s);
//static void player_add_levarie(SPRITE *s);
//static void player_add_hlaser(SPRITE *s);

//static void weapon_colcheck(SPRITE *s, /*int angle,*/ int destroy, int check_bullets);
#endif
