#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <SDL/SDL.h>
#include <math.h>

#include "support.h"
#include "sprite.h"
//#include "particle.h"
#include "enemy.h"


enum _select_pl 		//[***090203		追加
{
	REIMU=0,
	MARISA,
	REMILIA,
	CIRNO,
	YUYUKO,
};

/*
	bossmodeについて
	0:特になし。							// [ゲーム中]
	1:ボスとの戦闘中。						// [ゲーム中]
	4:形態変更中。強敵撃破。弾回収待ち。	// [ゲーム中](enemy_greatfairy.cでも使う)
	2:ボス撃破中。弾回収待ち。				// [ゲーム中]
//
	5:開始イベントファイル読み込み。		// [戦闘前]
	7:終了イベントファイル読み込み。		// [撃破後]
	3:ボス開始イベント(会話シーン)。		// [戦闘前]
	6:ボス終了イベント(会話シーン)。		// [撃破後]
	8:ボス曲を鳴らし、1ボスとの戦闘へ。 	// [戦闘前]
	9:stage読み込み。						// [撃破後]
*/
#if 1
	/* 旧仕様 */
	enum _boss_mode 		//[***090223		追加
	{
		B00_NONE=0, 		// [ゲーム中]
		B01_BATTLE, 		// [ゲーム中]
		B02_DEATH_WAIT, 	// [ゲーム中]
		B03_BEFORE_EVENT,	// [戦闘前]
		B04_CHANGE, 		// [ゲーム中]
		B05_BEFORE_LOAD,	// [戦闘前]
		B06_AFTER_EVENT,	// [撃破後]
		B07_AFTER_LOAD, 	// [撃破後]
		B08_START,			// [戦闘前]
		B09_STAGE_LOAD, 	// [撃破後]
	};
#else
	enum _boss_mode 		//[***090223		追加
	{
		B00_NONE=0, 		// [ゲーム中]
		B01_BATTLE, 		// [ゲーム中]
		B04_CHANGE, 		// [ゲーム中]
		B02_DEATH_WAIT, 	// [ゲーム中]
	//
		B05_BEFORE_LOAD,	// [戦闘前]
		B07_AFTER_LOAD, 	// [撃破後]
		B03_BEFORE_EVENT,	// [戦闘前]
		B06_AFTER_EVENT,	// [撃破後]
		B08_START,			// [戦闘前]
		B09_STAGE_LOAD, 	// [撃破後]
	};
#endif

/*なし*/
#define BONUS_FLAG_00_NONE							0x00
/* [上部自動収集] MAX時にプレイヤー上部収集で自動アイテム収集 */
#define BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM		0x01
/* [ボムによる自動収集] ボム発動で自動アイテム収集 */
#define BONUS_FLAG_02_BOMB_AUTO_GET_ITEM			0x02
/* [スコア自動収集] ボス倒し後に自動点数収集(★アイテム用) */
#define BONUS_FLAG_03_SCORE_AUTO_GET_ITEM			0x04



#define BONUS_FLAG_07_IS_GRAZE						0x40
#define BONUS_FLAG_08_OPTION_HIDE					0x80


//#define BONUS_FLAG_0123_AUTO_GET_ITEM (BONUS_FLAG_01_PLAYER_UP_AUTO_GET_ITEM|BONUS_FLAG_02_BOMB_AUTO_GET_ITEM|BONUS_FLAG_03_SCORE_AUTO_GET_ITEM)

typedef struct
{
	int lives;		/* 残りチャンス */
	int bombs;		/* ボム数 */
	int graze;		/* グレイズ得点 */
	int score;		/* スコア得点 */
//	int now_stage/*level*/;
	int bossmode;
	int state;
//	int explode;// ←必ず0なので意味なかった。なんかの機能の残りだっけ？？？
	/*double*/int save_delay;
//	/*double*/int an im_delay;	廃止
	int weapon; 				/*	0x00-0x7f  (0-127 の128段階==本家と同じ)   max==127==「128段階」*/
	int player_speed;
//	int player_speed_minimum;	/*各プレイヤーごとの固定値なので削除*/
//	int player_speed_maximum;	/*各プレイヤーごとの固定値なので削除*/
	int extra_type;
	int bonus_flag; 		/* ボーナスアイテムが自分に集まる状態の設定フラグ */	//[***090116		追加
	int hit_bomb_wait;		//[***090125		追加
//	int option_flag;		//	int bonus_flag;ボーナスフラグに吸収
//	double extra_interval;/*武器使用間隔の共通時間は廃止(各武器で間隔は管理)*/
	/*double*/int bomber_time;
//	double weapon_interval;
	SPRITE *core;
	SPRITE *enemy;			//[***090125		追加:playerに当たった物
	SPRITE *boss;			//[***090305		追加
	void (*callback_boss_hitbyweapon)(SPRITE *c, SPRITE *s);		//[***090325		追加
	void (*callback_boss_hitbyplayer)(SPRITE *c);					//[***090325		追加
} PLAYER_DATA;


enum _player_extras
{
	PLX_NONE=0,
//	PLX_HOMING,
//	PLX_HLASER,
//	PLX_SHIELD,
	PLX_BOMB,
	PLX_LAST		/* 最大数 */
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
