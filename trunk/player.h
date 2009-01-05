#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL/SDL.h>
#include <math.h>

#include "support.h"
#include "sprite.h"
#include "particle.h"
#include "enemy.h"

enum _player_state { PL_NORMAL, PL_EXPLODE, PL_RESET, PL_SAVE };

typedef struct {
	int lives;
	int bombs;
	int graze;
	int score;
	int level;
	int bossmode;
	int state;
	int explode;
	double save_delay;
	double anim_delay;
	int weapon;
	int player_speed;
	int extra;
	double extra_wait;
	double extra_time;
	double weapon_wait;
	SPRITE *core;
} PLAYER_DATA;

enum _weapon_type {
	WP_PLASMA=0,
	WP_DOUBLEPLASMA,
	WP_QUADPLASMA,
	WP_FIREBALL,
	WP_DOUBLEFIREBALL,
	WP_QUADFIREBALL,
	WP_FIFTHFIREBALL,
	WP_KILLRAY,
	WP_LAST
};

enum _player_extras {
	PLX_NONE=0,
	PLX_HOMING,
	PLX_SHIELD,
	PLX_HLASER,
	PLX_BOMB,
	PLX_LAST
};

typedef struct {
	int strength;
} WEAPON_BASE;

typedef struct {
	int strength;
	int angle;
	double speed;
} PL_PLASMA_DATA;

typedef struct {
	int strength;
	int angle;
	double speed;
} PL_FIREBALL_DATA;

typedef struct {
	int strength;
	int angle;
	double speed;
	double dist;
} PL_KILLRAY_DATA;

typedef struct {
	int strength;
	int angle;
	double speed;
} PL_BOMB_DATA;

typedef struct {
	int strength;
	int nr;
	int state;
	double angle;
	double range;
	double speed;
	SPRITE *tgsprite;
	int target_id;
} PL_HOMING_DATA;

typedef struct {
	double angle;
	int rad;
	double speed;
} PL_SHIELD_DATA;

typedef struct {
	int strength;
	int nr;
	int state;
	double angle;
	double range;
	double speed;
	int target_id;
} PL_HLASER_DATA;

void player_init();
void player_move(SPRITE *s);
void player_keycontrol(SPRITE *s);
void player_colcheck(SPRITE *s, int mask);

void weapon_check(int w);

SPRITE *player_add_core(SPRITE *s1);
void player_move_core(SPRITE *s2);
void player_add_fireball(SPRITE *player);
void player_add_doublefireball(SPRITE *player);
void player_add_quadfireball(SPRITE *player);
void player_add_fifthfireball(SPRITE *player);
void player_move_fireball(SPRITE *s);

void player_add_plasma(SPRITE *player);
void player_add_doubleplasma(SPRITE *player);
void player_add_quadplasma(SPRITE *player);
void player_move_plasma(SPRITE *s);

void player_add_killray(SPRITE *player);
void player_move_killray(SPRITE *s);

void player_add_bomb(SPRITE *s);
void player_move_bomb(SPRITE *s);

void player_add_homing(SPRITE *s);
void player_move_homing(SPRITE *s);
int search_enemy();

void player_add_shield(SPRITE *s);
void player_move_shield(SPRITE *s);
void player_move_shield2(SPRITE *s);

void player_add_hlaser(SPRITE *s);
void player_controller_hlaser(CONTROLLER *c);
void player_move_hlaser(SPRITE *s);

void weapon_colcheck(SPRITE *s, int angle, int destroy, int check_bullets);
#endif
