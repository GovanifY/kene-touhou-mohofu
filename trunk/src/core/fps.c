#include "fps.h"
double fps_factor; /* FPS 調整値 */
extern int difficulty;

#include <psptypes.h>
#include <psprtc.h>

/*static*/ unsigned int PSP_GetTicks(void)
{
	u64 current_ticks;/* 1[nsec] == 1/(1000*1000)[sec] */
	sceRtcGetCurrentTick(&current_ticks);
	return (current_ticks/*>>8*/);
}

#define FPS_MAX_HISTORY 8/*5*/ /* 測定履歴数 */
static double fps_history_value[FPS_MAX_HISTORY]; /* FPS 履歴値 */
static Uint32 fps_old_time; /* 前回測定した時の時間 */

void fps_init(void)
{
	fps_old_time/*=fps_gamestart; fps_gamestart*/= PSP_GetTicks();
	fps_factor=(double)(1);/* ==1 当り障りは無いが、いい加減な値 */
	int i;
	for (i=0; i<FPS_MAX_HISTORY; i++)
		{ fps_history_value[i]=(double)((double)(1)/(double)(FPS_MAX_HISTORY));}
	/* ==1.0/FPS_MAX_HISTORY 当り障りは無いが、いい加減な値 */
}

void fps_newframe(void)
{
	static Uint32 fps_now_time;
	static Uint32 fps_history_index=0; /* 履歴インデックス */
	static int difficulty_old=99;/* ==99 この値は開始時に必ず再計算させる都合上、ありえない設定値とする。 */
	int i;
	static double tick_addj;
	/* 難易度が変更された場合 */
	if (difficulty_old != difficulty)
	{
		difficulty_old = difficulty;
		/* ↓東方を目指してるのなら、難易度でゲーム全体の速度を変えない方が良いと思いますが、 */
		/* 現在旧版と互換の為、下記の様にしてあります。 */
		/* ゲーム全体のゲーム速度 調整値の計算 */
		#define ADJUST_INTERVAL_VALUE (22)		//(28-difficulty*3)=>difficulty=2
	//	#define ADJUST_INTERVAL_VALUE (40)		//(28-difficulty*3)=>difficulty=2
		tick_addj = (double)(1.0/((double)(ADJUST_INTERVAL_VALUE)*(double)(FPS_MAX_HISTORY*1000) ));
	}
	/* FPS 履歴値の計算(現在、過去時間から、履歴を計算し蓄える) */
	fps_old_time = fps_now_time; /* 前回の時間 */
	fps_now_time = PSP_GetTicks(); /* 今回の時間 */

	fps_history_index++;
	if (fps_history_index>=FPS_MAX_HISTORY) { fps_history_index=0;}

	fps_history_value[fps_history_index] = (double)(fps_now_time-fps_old_time)*tick_addj;
	/* FPS 調整値の計算(予め加重で割ってあるので足すだけ) */
	fps_factor=0;
	for (i=0; i<FPS_MAX_HISTORY; i++) { fps_factor += fps_history_value[i];}
}
