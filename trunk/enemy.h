#ifndef _ENEMY_H_
#define _ENEMY_H_
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

#include "sprite.h"
//#include "particle.h"
#include "support.h"
#include "bonus.h"
#include "loadlv.h"

extern double fps_factor;

extern SDL_Surface *screen;

extern int player_now_stage;
extern SPRITE *player;
		//[***090126		追加

extern int difficulty;
		//[***090114		追加
		//[***090116		追加
		//[***090124		追加
		//[***090127		追加
		//[***090209		追加
enum
{
EXPLOSION_TYPE00=0,
EXPLOSION_TYPE01,
EXPLOSION_TYPE02,
EXPLOSION_TYPE03,
EXPLOSION_TYPE04
};

typedef struct
{
	int score;
	int health;
} ENEMY_BASE;

typedef struct
{
	double angle;
	double speed;
	int id;
} BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	int state;
} G_BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double gra;
	double sum;
}GR_BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double gra;
	double sum;
	int bounds;
}PO_BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
} LASER_DATA;

typedef struct
{
	ENEMY_BASE b;
	double angle;
	double speed;
	double range;
	double delay;
} HOMING_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	int ransu;
	int wait_bg;
} BIGBULLET_DATA;

typedef struct
{
	double *sx;
	double *sy;
	int *type;
	int sw;
	int sh;
} BIGBULLET_S_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double a;
} NEW_BIGBULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	int timer;
	double a;
} ST_BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double next_angle;
	int timer;
	int state;
	double a;
} ST2_BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double a_angle;
	int timer;
} AN_BULLET_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double sum;
	double gra;
} FALL_KNIFE_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	int speed2;
	int height;
	int target;
	double timer;
} FOLLOW_KNIFE_DATA;

typedef struct
{
	double angle;
	double speed;
	int id;
	double d_angle;
	double length;
	double x;
	double y;
	int state;
	int wait1;	//fire
	int wait2;	//act
} EVEN_KNIFE_DATA;

typedef struct
{
	double wait;
	double framewait;
	double framestat;
} EXPLOSION_DATA;

//[r13]
//void bullet_way(SPRITE *s, int way, double angle, double angle2, double spd);



/* enemy_splash.c enemy_boss01.c enemy_boss02.c enemy_boss03.c */
extern void enemy_laser_move(SPRITE *s);


/* それ以外 move 関連はすべてstatic */
//static void enemy_bullet_move(SPRITE *s);
//void enemy_gr_bullet_move(SPRITE *s);
//void enemy_laser_move2(SPRITE *s);
//void enemy_homing_move(SPRITE *s);
//void enemy_bigbullet_move(SPRITE *s);
//void enemy_bigbullet_move2(SPRITE *s);
//void enemy_new_bigbullet_move(SPRITE *s);
//void enemy_g_bullet_move(SPRITE *s);
//void enemy_pong_bullet_move(SPRITE *s);
//void enemy_stop_bullet_move(SPRITE *s);
//void enemy_stop_bullet2_move(SPRITE *s);
//void enemy_angle_bullet_move(SPRITE *s);

//void enemy_knife_move(SPRITE *s);
//void enemy_fall_knife_move(SPRITE *s);
//void enemy_follow_knife_move(SPRITE *s);
//void enemy_evenl_knife_move(SPRITE *s);
//void enemy_evenr_knife_move(SPRITE *s);
//static void explosion_move(SPRITE *c);

void enemy_bullet_create(SPRITE *s, double speed);
void enemy_gr_bullet_create(SPRITE *s, double speed, double angle, double gra);
void enemy_laser_create(SPRITE *s, double speed);
void enemy_laser_create2(SPRITE *s, double speed, double angle);
void enemy_homing_create(SPRITE *s);
void enemy_homing_update(SPRITE *s);
void enemy_bigbullet_create(SPRITE *s, double ex, double ey, double speed, double angle, int ransu);
void enemy_new_bigbullet_create(SPRITE *s, double speed, double angle, double a);
void enemy_g_bullet_create(SPRITE *s, double speed, int state, double angle);
void enemy_pong_bullet_create(SPRITE *s, double speed, double angle, double gra, int bou);
void enemy_stop_bullet_create(SPRITE *s, double speed, double angle, double gra);
void enemy_stop_bullet2_create(SPRITE *s, double speed, double angle, double a, double next_angle);
void enemy_angle_bullet_create(SPRITE *s, double speed, double angle, double a_angle);
void enemy_knife_create(SPRITE *s, double speed, double angle, int anim);
void enemy_fall_knife_create(SPRITE *s, double speed, double angle, double gra);
void enemy_follow_knife_create(SPRITE *s, double speed, double angle, int height);
void enemy_follow_knife_create2(double x, double y, double speed, double angle, int height);
void enemy_even_knife_create(SPRITE *s, double speed, double length, int r_or_l);




//static void enemy_sp1_bullet_create(SPRITE *s, double speed, double angle, double gra, int r_or_l);
//static void enemy_sp1_bullet_move(SPRITE *s);
//static void enemy_sp2_bullet_create(SPRITE *s, int angle, double gra);
//static void enemy_sp2_bullet_move(SPRITE *s);

void explosion_add(int x, int y, double wait, int type);

//void enemy_magicformation_move(SPRITE *s);

//void enemy_bgpanel_add_xy(const short xxx, const short yyy,  char speed);
//void enemy_bgpanel2_add_xy(const short xxx, const short yyy,	char speed);
extern void enemy_bgpanel_type_add_xy(	const short xxx, const short yyy, const short speed256, int type);
extern void enemy_grounder_add_xy(		const short xxx, const short yyy, const short speed256);
extern void enemy_magicformation_add_xy(const short xxx, const short yyy, const short speed256);


void enemy_nonshield_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);


/* thegame.c のみで ザコ／ボスで宣言が必要なもの(グローバル)は、thegame.c へ移動した。 */

/* ボスで宣言が必要なもの(グローバル) */

extern void enemy_boss01_hitbyplayer(SPRITE *c);
extern void enemy_boss01_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
extern void enemy_boss02_hitbyplayer(SPRITE *c);
extern void enemy_boss02_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
extern void enemy_boss03_hitbyplayer(SPRITE *c);
extern void enemy_boss03_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
extern void enemy_boss04_hitbyplayer(SPRITE *c);
extern void enemy_boss04_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);

void enemy_n_way_bullet(SPRITE *s, char *filename, int frame, int n, double speed, double angle);

void enemy_gametext_add(char *text, int y);



/* 下記はグローバルにする必要がない。(グローバルにすると遅くなる)
static に変更する為、ここでは宣言できない */

//static void enemy_grounder_move(SPRITE *s);
//static void enemy_bgpanel_move(SPRITE *s);
//static void enemy_bgpanel2_move(SPRITE *s);
//static void enemy_gametext_mover(SPRITE *s);
//static void enemy_xev_move(SPRITE *s);
//static void enemy_crusher_controller(CONTROLLER *c);
//static void enemy_crusher_move(SPRITE *s);
//static void enemy_eyefo_controller(CONTROLLER *c);
//static void enemy_eyefo_move(SPRITE *s);
//static void enemy_cube_move(SPRITE *s);
//static void enemy_drager_move(SPRITE *s);
//static void enemy_mine_move(SPRITE *s);
//static void enemy_supermine_add(SPRITE *s,int level);
//static void enemy_supermine_move(SPRITE *s);
//static void enemy_rwingx_move(SPRITE *s);
//static void enemy_cir_move(SPRITE *s);
//static void enemy_zatak_move(SPRITE *s);
//static void enemy_curver_controller(CONTROLLER *c);
//static void enemy_curver_move(SPRITE *s);
//static void enemy_badguy_move(SPRITE *s);
//static void enemy_proball_controller(CONTROLLER *c);
//static void enemy_proball_move(SPRITE *s);
//static void enemy_plasmaball_move(SPRITE *s);
//static void enemy_ming_controller(CONTROLLER *c);
//static void enemy_ming_move(SPRITE *s);
//static void enemy_greeter_controller(CONTROLLER *c);
//static void enemy_greeter_move(SPRITE *s);
//static void enemy_splash_move(SPRITE *s);
//static void enemy_fairy_move(SPRITE *s);
//static void enemy_greatfairy_move(SPRITE *s);

//static void enemy_boss01_setpos(int x, int y);
//static void enemy_boss01_move(SPRITE *c);
//static void enemy_boss01_fire(int where);

//static void enemy_boss02_move(SPRITE *s);
//static void enemy_boss02_waitstate(SPRITE *s, int nextstate);
//static void enemy_boss02_sr_add(SPRITE *c);
//static void enemy_boss02_sr_controller(CONTROLLER *c);
//static void enemy_boss02_sr_add2(SPRITE *c);
//static void enemy_boss02_sr_controller2(CONTROLLER *c);
//static void enemy_boss02_ice_add(SPRITE *c);
//static void enemy_boss02_ice_controller(CONTROLLER *e);
//static void enemy_boss02_ice_move(SPRITE *s);

//static void enemy_boss03_move(SPRITE *s);

//static void enemy_boss04_move(SPRITE *s);
//static void enemy_boss04_waitstate(SPRITE *s, int nextstate, int aktframe);
//static void enemy_boss04_out(SPRITE *s);
//static void enemy_boss04_more_angle(SPRITE *s, double speed, double angle, double a);
//static void enemy_boss04_maho_create(SPRITE *s);
//static void enemy_boss04_maho_move(SPRITE *s);
//static void enemy_boss04_knifes(SPRITE *s, double, int angle, int height);
//static void enemy_boss04_knifes2(SPRITE *s, double speed);
//static void enemy_boss04_knifes3(SPRITE *s, double speed, double length);

#endif
