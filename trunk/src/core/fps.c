#include "fps.h"
double fps_factor; /* FPS �����l */
extern int difficulty;

#include <psptypes.h>
#include <psprtc.h>

/*static*/ unsigned int PSP_GetTicks(void)
{
	u64 current_ticks;/* 1[nsec] == 1/(1000*1000)[sec] */
	sceRtcGetCurrentTick(&current_ticks);
	return (current_ticks/*>>8*/);
}

#define FPS_MAX_HISTORY 8/*5*/ /* ���藚�� */
static double fps_history_value[FPS_MAX_HISTORY]; /* FPS ����l */
static Uint32 fps_old_time; /* �O�񑪒肵�����̎��� */

void fps_init(void)
{
	fps_old_time/*=fps_gamestart; fps_gamestart*/= PSP_GetTicks();
	fps_factor=(double)(1);/* ==1 ������͖������A���������Ȓl */
	int i;
	for (i=0; i<FPS_MAX_HISTORY; i++)
		{ fps_history_value[i]=(double)((double)(1)/(double)(FPS_MAX_HISTORY));}
	/* ==1.0/FPS_MAX_HISTORY ������͖������A���������Ȓl */
}

void fps_newframe(void)
{
	static Uint32 fps_now_time;
	static Uint32 fps_history_index=0; /* �����C���f�b�N�X */
	static int difficulty_old=99;/* ==99 ���̒l�͊J�n���ɕK���Čv�Z������s����A���肦�Ȃ��ݒ�l�Ƃ���B */
	int i;
	static double tick_addj;
	/* ��Փx���ύX���ꂽ�ꍇ */
	if (difficulty_old != difficulty)
	{
		difficulty_old = difficulty;
		/* ��������ڎw���Ă�̂Ȃ�A��Փx�ŃQ�[���S�̂̑��x��ς��Ȃ������ǂ��Ǝv���܂����A */
		/* ���݋��łƌ݊��ׁ̈A���L�̗l�ɂ��Ă���܂��B */
		/* �Q�[���S�̂̃Q�[�����x �����l�̌v�Z */
		#define ADJUST_INTERVAL_VALUE (22)		//(28-difficulty*3)=>difficulty=2
	//	#define ADJUST_INTERVAL_VALUE (40)		//(28-difficulty*3)=>difficulty=2
		tick_addj = (double)(1.0/((double)(ADJUST_INTERVAL_VALUE)*(double)(FPS_MAX_HISTORY*1000) ));
	}
	/* FPS ����l�̌v�Z(���݁A�ߋ����Ԃ���A�������v�Z���~����) */
	fps_old_time = fps_now_time; /* �O��̎��� */
	fps_now_time = PSP_GetTicks(); /* ����̎��� */

	fps_history_index++;
	if (fps_history_index>=FPS_MAX_HISTORY) { fps_history_index=0;}

	fps_history_value[fps_history_index] = (double)(fps_now_time-fps_old_time)*tick_addj;
	/* FPS �����l�̌v�Z(�\�߉��d�Ŋ����Ă���̂ő�������) */
	fps_factor=0;
	for (i=0; i<FPS_MAX_HISTORY; i++) { fps_factor += fps_history_value[i];}
}
