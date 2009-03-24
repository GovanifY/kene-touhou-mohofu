#include "zenkaku.h"

enum _story_state
{
	STORY_FADEOUT1,
	STORY_LOAD,
	STORY_FADEIN,
	STORY_WORKS,
	STORY_FADEOUT2,
	STORY_QUIT
};

enum _script_command
{
	SCP_BGTEXT,
	SCP_CLCURSOR,
	SCP_CLICK,
	SCP_FILTER,
	SCP_JUMP,
	SCP_LOADBG,
	SCP_LOADCUR,
	SCP_LOADL,
	SCP_LOADR,
	SCP_LOADSP,
	SCP_MOVESP,
	SCP_PARAMSP,
	SCP_RELOADSP,
	SCP_SAVECUR,
	SCP_SUBG,
	SCP_SUFILTER,
	SCP_SUL,
	SCP_SUR,
	SCP_SUSPRITE,
	SCP_SUTWINDOW,
	SCP_SUWINDOW,
	SCP_TEXT,
	SCP_TWINDOW,
	SCP_WAIT
};

typedef struct _scenario_script
{
	int command;
	int done;		//終わったかどうか
	int chain;		//同時実行かどうか
					//0=違うよ	1=1つ目	2=2つ目
	char para0[200];
	int para1;
	int para2;
	int para3;
	void *data;
	struct _scenario_script *next;
} S_SCRIPT;

typedef struct _sc_sprite
{
	SDL_Surface *img;				/* Images */
	Uint8 frames;					/* アニメーションさせる予定 */
	Uint8 anim_speed;				/* 上に同じ */
	Uint8 anim_type;				/* どういう風にアニメーションさせるか */
	Uint8 anim_time;				/* アニメーション回数 */
	Uint8 alpha;					/* alpha値 */
	Uint8 flags;					/* ここでは表示させるかどうかだけだよ */
	int aktframe;					/* 現在のフレーム */
	double anim_count;				/* アニメーションのタイミング */
	double x,y; 					/* x,y座標だよ */
	int w,h; 						/**/
	int cw,ch; 						/* 中心座標だよ */
	int move_wait;					/* 動きがあったときの制御用 */
	int move_x;						/* 目標座標 */
	int move_y;
	int r_course;					/* 大体の方向 */
	double angle;					/* 正確な方向 */
} SC_SPRITE;

extern int thescript();
extern int script_init(char *filename, char *bg_name,int width);
extern void story_init();
extern void story_work();
