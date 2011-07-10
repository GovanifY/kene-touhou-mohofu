#include "psp_vfpu.h"
/*
	2��4x4�s��(�N�H�[�^�j�I��)���A��������B

	ScePspQuatMatrix *qout					�������ʂ�4x4�s��(�N�H�[�^�j�I��)

	ScePspQuatMatrix *a,					1�ڂ�4x4�s��(�N�H�[�^�j�I��)
	ScePspQuatMatrix *b,					2�ڂ�4x4�s��(�N�H�[�^�j�I��)

	float multiplex_time1_00
	vfpu_quaternion_sample_linear();		���������́A0.00 ���� 1.00 �̕s�������Ŏw�肷��B

	int multiplex_time65536
	vfpu_quaternion_sample_linear65536(); 	���������́A0 ���� 65536 ��(unsigned int)�����Ŏw�肷��B

*/
#if 1
void vfpu_quaternion_sample_linear(
	ScePspQuatMatrix *qout,
	ScePspQuatMatrix *a,					/* 1�ڂ�4x4�s�� */
	ScePspQuatMatrix *b,					/* 2�ڂ�4x4�s�� */
	float multiplex_time1_00				/* �������� float time, value from(0.00 to 1.00) */
)
{
	__asm__ volatile (
		"lv.q     C000, 0 + %1\n"			/* C000 = (a); */
		"lv.q     C010, 0 + %2\n"			/* C010 = (b); */
		"mtv      %3, S020\n"				/* S020 = (multiplex_time1_00); */
		"vocp.s   S021, S020\n" 			/* S021 = (1)-(S020);���Ԃ� */
		"vscl.q   C000, C000, S021\n"		/* C000 *= S021; 4x4�s����X�P�[�����O */
		"vscl.q   C010, C010, S020\n"		/* C010 *= S020; 4x4�s����X�P�[�����O */
		"vadd.q   C000, C000, C010\n"		/* C000 += C010; */
		"sv.q     C000, 0 + %0\n"			/* (qout) = C000; */
	: "=m"(*qout) : "m"(*a), "m"(*b), "r"(multiplex_time1_00));
}
#endif

