
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "support.h"
/*---------------------------------------------------------
	時間測定関連
---------------------------------------------------------*/

#include "fps.h"

//#include <psptypes.h>
//#include <psprtc.h>

/*---------------------------------------------------------
	現在時間を調べる
	-------------------------------------------------------
	1[nano sec]単位で、時間を測定する。  ( 1[nano sec] == 1/(1000*1000)[sec] )
	用途：ゲーム時間計測用。
	注意：uint64 を unsigned int にキャストしているので、厳密には不具合がある。
	(約72分毎に1フレーム) 0xffff ffff == 4294967295 / (60 * 1000 000) == 71.58278825 [分]
	これを使ったfps同期ルーチンはその事を考慮しておく必要がある。
---------------------------------------------------------*/

/*static*/ unsigned int psp_get_uint32_ticks(void)
{
	u64 current_ticks;/* 1[nano sec] == 1/(1000*1000)[sec] */
	sceRtcGetCurrentTick(&current_ticks);
	return (current_ticks/*>>8*/);
}

/*---------------------------------------------------------
	前回からどれだけ時間が経ったかを調べる
	-------------------------------------------------------
	前回計測時間との差分を 1[nano sec] 単位  ( 1[nano sec] == 1/(1000*1000)[sec] )
	の unsigned int == 符号なし32bit形式で返す。
	差分で返すので、(前回から約72分以内に呼べば)上の関数のような問題は生じない。
	用途：fps計測用。
	注意：複数ヵ所で呼ばれる事は考慮されてないので、必ず一ヶ所から呼ぶ事。
---------------------------------------------------------*/

/*static*/ unsigned int psp_get_differencial_ticks(void)
{
	static u64 ticks_alt;	/* 前回の時間 the clock time of alter ticks. */
	static u64 ticks_now;	/* 今回の時間 the clock time of current ticks. */
	ticks_alt	= ticks_now;
	sceRtcGetCurrentTick(&ticks_now);
	return ((unsigned int)(ticks_now - ticks_alt));/* 差分時間 the clock time of differencial ticks. */
}
/*---------------------------------------------------------

---------------------------------------------------------*/

#define USE_FPS (0)

#if (1==USE_FPS)

#define FPS_MAX_HISTORY 8/*5*/ /* 測定履歴数 */
static dou ble fps_history_value[FPS_MAX_HISTORY]; /* FPS 履歴値 */
static Uint32 fps_old_time; /* 前回測定した時の時間 */

void fps_init(void)
{
	fps_old_time/*=fps_gamestart; fps_gamestart*/= psp_get_uint32_ticks();
	fps_factor=(dou ble)(1);/* ==1 当り障りは無いが、いい加減な値 */
	int i;
	for (i=0; i<FPS_MAX_HISTORY; i++)
	{	fps_history_value[i] = (dou ble)((dou ble)(1)/(dou ble)(FPS_MAX_HISTORY));	}
	/* ==1.0/FPS_MAX_HISTORY 当り障りは無いが、いい加減な値 */
}

void fps_newframe(void)
{
	static Uint32 fps_now_time;
	static Uint32 fps_history_index=0; /* 履歴インデックス */
	static int difficulty_old=99;/* ==99 この値は開始時に必ず再計算させる都合上、ありえない設定値とする。 */
	int i;
	static dou ble tick_addj;
	/* 難易度が変更された場合 */
	if (difficulty_old != difficulty)
	{
		difficulty_old = difficulty;
		/* ↓東方を目指してるのなら、難易度でゲーム全体の速度を変えない方が良いと思いますが、 */
		/* 現在旧版と互換の為、下記の様にしてあります。 */
		/* ゲーム全体のゲーム速度 調整値の計算 */
		#define ADJUST_INTERVAL_VALUE (22)		//(28-difficulty*3)=>difficulty=2
	//	#define ADJUST_INTERVAL_VALUE (40)		//(28-difficulty*3)=>difficulty=2
		tick_addj = (dou ble)(1.0/((dou ble)(ADJUST_INTERVAL_VALUE)*(dou ble)(FPS_MAX_HISTORY*1000) ));
	}
	/* FPS 履歴値の計算(現在、過去時間から、履歴を計算し蓄える) */
	fps_old_time = fps_now_time; /* 前回の時間 */
	fps_now_time = psp_get_uint32_ticks(); /* 今回の時間 */

	fps_history_index++;
	if (fps_history_index>=FPS_MAX_HISTORY) 	{	fps_history_index=0;	}

	fps_history_value[fps_history_index] = (dou ble)(fps_now_time-fps_old_time)*tick_addj;
	/* FPS 調整値の計算(予め加重で割ってあるので足すだけ) */
	fps_factor=0;
	for (i=0; i<FPS_MAX_HISTORY; i++)	{	fps_factor += fps_history_value[i]; }
}
#else
void fps_init(void){}
void fps_newframe(void){}
#endif // (1==USE_FPS)
