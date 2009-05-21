
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "support.h"
/*---------------------------------------------------------
	���ԑ���֘A
---------------------------------------------------------*/

#include "fps.h"

//#include <psptypes.h>
//#include <psprtc.h>

/*---------------------------------------------------------
	���ݎ��Ԃ𒲂ׂ�
	-------------------------------------------------------
	1[nano sec]�P�ʂŁA���Ԃ𑪒肷��B  ( 1[nano sec] == 1/(1000*1000)[sec] )
	�p�r�F�Q�[�����Ԍv���p�B
	���ӁFuint64 �� unsigned int �ɃL���X�g���Ă���̂ŁA�����ɂ͕s�������B
	(��72������1�t���[��) 0xffff ffff == 4294967295 / (60 * 1000 000) == 71.58278825 [��]
	������g����fps�������[�`���͂��̎����l�����Ă����K�v������B
---------------------------------------------------------*/

/*static*/ unsigned int psp_get_uint32_ticks(void)
{
	u64 current_ticks;/* 1[nano sec] == 1/(1000*1000)[sec] */
	sceRtcGetCurrentTick(&current_ticks);
	return (current_ticks/*>>8*/);
}

/*---------------------------------------------------------
	�O�񂩂�ǂꂾ�����Ԃ��o�������𒲂ׂ�
	-------------------------------------------------------
	�O��v�����ԂƂ̍����� 1[nano sec] �P��  ( 1[nano sec] == 1/(1000*1000)[sec] )
	�� unsigned int == �����Ȃ�32bit�`���ŕԂ��B
	�����ŕԂ��̂ŁA(�O�񂩂��72���ȓ��ɌĂׂ�)��̊֐��̂悤�Ȗ��͐����Ȃ��B
	�p�r�Ffps�v���p�B
	���ӁF���������ŌĂ΂�鎖�͍l������ĂȂ��̂ŁA�K���ꃖ������ĂԎ��B
---------------------------------------------------------*/

/*static*/ unsigned int psp_get_differencial_ticks(void)
{
	static u64 ticks_alt;	/* �O��̎��� the clock time of alter ticks. */
	static u64 ticks_now;	/* ����̎��� the clock time of current ticks. */
	ticks_alt	= ticks_now;
	sceRtcGetCurrentTick(&ticks_now);
	return ((unsigned int)(ticks_now - ticks_alt));/* �������� the clock time of differencial ticks. */
}
/*---------------------------------------------------------

---------------------------------------------------------*/

#define USE_FPS (0)

#if (1==USE_FPS)

#define FPS_MAX_HISTORY 8/*5*/ /* ���藚�� */
static dou ble fps_history_value[FPS_MAX_HISTORY]; /* FPS ����l */
static Uint32 fps_old_time; /* �O�񑪒肵�����̎��� */

void fps_init(void)
{
	fps_old_time/*=fps_gamestart; fps_gamestart*/= psp_get_uint32_ticks();
	fps_factor=(dou ble)(1);/* ==1 ������͖������A���������Ȓl */
	int i;
	for (i=0; i<FPS_MAX_HISTORY; i++)
	{	fps_history_value[i] = (dou ble)((dou ble)(1)/(dou ble)(FPS_MAX_HISTORY));	}
	/* ==1.0/FPS_MAX_HISTORY ������͖������A���������Ȓl */
}

void fps_newframe(void)
{
	static Uint32 fps_now_time;
	static Uint32 fps_history_index=0; /* �����C���f�b�N�X */
	static int difficulty_old=99;/* ==99 ���̒l�͊J�n���ɕK���Čv�Z������s����A���肦�Ȃ��ݒ�l�Ƃ���B */
	int i;
	static dou ble tick_addj;
	/* ��Փx���ύX���ꂽ�ꍇ */
	if (difficulty_old != difficulty)
	{
		difficulty_old = difficulty;
		/* ��������ڎw���Ă�̂Ȃ�A��Փx�ŃQ�[���S�̂̑��x��ς��Ȃ������ǂ��Ǝv���܂����A */
		/* ���݋��łƌ݊��ׁ̈A���L�̗l�ɂ��Ă���܂��B */
		/* �Q�[���S�̂̃Q�[�����x �����l�̌v�Z */
		#define ADJUST_INTERVAL_VALUE (22)		//(28-difficulty*3)=>difficulty=2
	//	#define ADJUST_INTERVAL_VALUE (40)		//(28-difficulty*3)=>difficulty=2
		tick_addj = (dou ble)(1.0/((dou ble)(ADJUST_INTERVAL_VALUE)*(dou ble)(FPS_MAX_HISTORY*1000) ));
	}
	/* FPS ����l�̌v�Z(���݁A�ߋ����Ԃ���A�������v�Z���~����) */
	fps_old_time = fps_now_time; /* �O��̎��� */
	fps_now_time = psp_get_uint32_ticks(); /* ����̎��� */

	fps_history_index++;
	if (fps_history_index>=FPS_MAX_HISTORY) 	{	fps_history_index=0;	}

	fps_history_value[fps_history_index] = (dou ble)(fps_now_time-fps_old_time)*tick_addj;
	/* FPS �����l�̌v�Z(�\�߉��d�Ŋ����Ă���̂ő�������) */
	fps_factor=0;
	for (i=0; i<FPS_MAX_HISTORY; i++)	{	fps_factor += fps_history_value[i]; }
}
#else
void fps_init(void){}
void fps_newframe(void){}
#endif // (1==USE_FPS)
