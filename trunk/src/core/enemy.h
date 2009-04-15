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
	EXPLOSION_MINI00=0,/*小爆発*/
	EXPLOSION_MINI01,
	EXPLOSION_MINI02,
	EXPLOSION_MINI03,
//
	EXPLOSION_ZAKO04,/*ザコ消滅爆発*/
	EXPLOSION_ZAKO05,
	EXPLOSION_ZAKO06,
	EXPLOSION_ZAKO07,
//
	EXPLOSION_FIRE08,/*火炎爆発*/
	EXPLOSION_FIRE09,
	EXPLOSION_FIRE10,
	EXPLOSION_FIRE11,
};
/* 奇数弾の場合に自機狙い */
#define ANGLE_JIKINERAI_KISUDAN 999

#define ANGLE_JIKINERAI_DAN 999



typedef struct
{
	int score;
	int health;
} ENEMY_BASE;

typedef struct		/* enemyの一般形 */
{
	ENEMY_BASE b;
	int state;
} ENEMY_DATA;




//2691186
typedef struct
{
	/*double*/int angle512;
	double speed;
//	int id;
} BULLET_DATA;


//[r13]
//void bullet_way(SPRITE *s, int way, dou ble angle, dou ble angle2, double spd);



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

/* 弾の生成 */

void enemy_bullet_create(SPRITE *s, double speed);
void enemy_gr_bullet_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int delta256);

//extern void enemy_laser_create(SPRITE *s, double speed);/*廃止(bullet_create_haridan180()にversion up)*/
//extern void enemy_laser_create2(SPRITE *s, double speed, /*double*/int angle512);/*廃止(bullet_create_haridan180()にversion up)*/
extern void bullet_create_haridan180(SPRITE *src, int angle512, int speed256, int offsx, int offsy );

void enemy_homing_create(SPRITE *s);
void enemy_homing_update(SPRITE *s);
void enemy_momiji_create(SPRITE *s, double speed, /*double*/int angle512);
//static void enemy_smallbullet_re_create(SPRITE *s, double speed, /*double*/int angle512, double a);
void enemy_bigbullet_create(SPRITE *s, double ex, double ey, double speed, /*double*/int angle512, int ransu);
void enemy_new_bigbullet_create(SPRITE *s, double speed, /*double*/int angle512, double a);
void enemy_g_bullet_create(SPRITE *s, double speed, int state, /*double*/int angle512);
void enemy_pong_bullet_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int delta256, int bou);
void enemy_stop_bullet_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int delta256);
//id enemy_stop_bullet2_create(SPRITE *s, double speed, /*double*/int angle512, double a, double next_angle);
void enemy_stop_bullet3_create(SPRITE *s, /*double*/int speed256, /*double*/int angle512, double a, /*double*/int next_angle512);

void enemy_angle_bullet_create(SPRITE *s, /*double*/int speed256, /*double*/int angle512, /*double*/int d_angle4096);
void enemy_knife_create(SPRITE *s, /*double*/int speed256, /*double*/int angle512, int anim);
void enemy_fall_knife_create(SPRITE *s, double speed, /*double*/int angle512, /*double*/int gra256);
void enemy_follow_knife_create1(SPRITE *s,                          /*double*/int speed256, /*double*/int angle512, int height);
//void enemy_follow_knife_create2(SPRITE *s,/*double x, double y,*/ /*double*/int speed256, /*double*/int angle512, int height);
void enemy_even_knife_create(SPRITE *s, double speed, /*double*/int length, int r_or_l);




//static void enemy_sp1_bullet_create(SPRITE *s, double speed, /*double*/int angle512, double gra, int r_or_l);
//static void enemy_sp1_bullet_move(SPRITE *s);
//static void enemy_sp2_bullet_create(SPRITE *s, int angle, double gra);
//static void enemy_sp2_bullet_move(SPRITE *s);

extern void explosion_add(int x, int y, /*double*/int wait, int type);
extern void explosion_add_rect(SPRITE *src);
extern void explosion_add_circle(SPRITE *src);

//void enemy_magicformation_move(SPRITE *s);

//void enemy_bgpanel_add_xy(const short xxx, const short yyy,  char speed);
//void enemy_bgpanel2_add_xy(const short xxx, const short yyy,	char speed);
extern void enemy_bgpanel_type_add_xy(	const short xxx, const short yyy, const short speed256, int type);
extern void enemy_grounder_add_xy(		const short xxx, const short yyy, const short speed256);
extern void enemy_magicformation_add_xy(const short xxx, const short yyy, const short speed256);


void enemy_nonshield_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);


/* thegame.c のみで ザコ／ボスで宣言が必要なもの(グローバル)は、thegame.c へ移動した。 */

/* ボスで宣言が必要なもの(グローバル) */

//static void callback_enemy_boss01_hitbyplayer(SPRITE *c);
//static void callback_enemy_boss02_hitbyplayer(SPRITE *c);
//static void callback_enemy_boss03_hitbyplayer(SPRITE *c);
//static void callback_enemy_boss04_hitbyplayer(SPRITE *c);
//static void callback_enemy_boss05_hitbyplayer(SPRITE *c);
//static void callback_enemy_boss01_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
//static void callback_enemy_boss02_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
//static void callback_enemy_boss03_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
//static void callback_enemy_boss04_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);
//static void callback_enemy_boss05_hitbyweapon(SPRITE *c, SPRITE *s/*, int angle*/);

void enemy_n_way_bullet_type(SPRITE *s, int bu_type,/* char *filename, int frame,*/ int n, double speed, /*double*/int angle512);
#define BU_TYPE01_KUGEL_PNG 0
#define BU_TYPE01_KUNAI_PNG 0

void enemy_gametext_add(char *text, int y);
void enemy_gameimg_add(char *filename,int xpos, int ypos);



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
//static void enemy_boss04_more_angle(SPRITE *s, double speed, /*double*/int angle512, double a);
//static void enemy_boss04_maho_create(SPRITE *s);
//static void enemy_boss04_maho_move(SPRITE *s);
//static void enemy_boss04_knifes(SPRITE *s, double, int angle, int height);
//static void enemy_boss04_knifes2(SPRITE *s, double speed);
//static void enemy_boss04_knifes3(SPRITE *s, double speed, double length);

#endif
