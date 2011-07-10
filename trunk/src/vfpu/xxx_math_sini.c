
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sin
---------------------------------------------------------*/
//		"vi2f.p     R102,   R102,   1/2\n"		//int��float�ϊ�
//		"vi2f.s   S000, S000,    1\n"	// int��float�ϊ��Ɠ�����1/  2�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    2\n"	// int��float�ϊ��Ɠ�����1/  4�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    3\n"	// int��float�ϊ��Ɠ�����1/  8�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    4\n"	// int��float�ϊ��Ɠ�����1/ 16�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    5\n"	// int��float�ϊ��Ɠ�����1/ 32�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    6\n"	// int��float�ϊ��Ɠ�����1/ 64�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    7\n"	// int��float�ϊ��Ɠ�����1/128�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,    8\n"	// int��float�ϊ��Ɠ�����1/256�{����B	(���v����1clock, latency 0)
//	???	���삵�Ȃ�
//		"vi2f.s   S000, S000,    9\n"	// int��float�ϊ��Ɠ�����1/512�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   10\n"	// int��float�ϊ��Ɠ�����1/1024�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   11\n"	// int��float�ϊ��Ɠ�����1/2048�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   12\n"	// int��float�ϊ��Ɠ�����1/4096�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   13\n"	// int��float�ϊ��Ɠ�����1/8192�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   14\n"	// int��float�ϊ��Ɠ�����1/16384�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   15\n"	// int��float�ϊ��Ɠ�����1/32768�{����B	(���v����1clock, latency 0)
//		"vi2f.s   S000, S000,   16\n"	// int��float�ϊ��Ɠ�����1/65536�{����B	(���v����1clock, latency 0)

int int256_sin1024(int angle_1024)
{
//	float angle_float;
//	angle_float = angle_1024;
//	float result;
	int result_t256;
	__asm__ volatile (
		"mtv      %1,   S000\n" 		// S000 = (angle_1024);�B				(���v���ԕs��(���Ԃ�1clock), latency �s��)
	//	"vcst.s   S001, VFPU_2_PI\n"	// S001 = (2/PI);
	//	"vmul.s   S000, S000, S001\n"	// x = x * (2/PI);
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (2.0f/512.0f);	// constant real
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (1.0f/256.0f);	// constant real
	//	"vmul.s   S000, S000, S001\n"	// x = x * (S001)
		"vi2f.s   S000, S000,    8\n"	// int��float�ϊ��Ɠ�����1/256�{����B	(���v����1clock, latency 0)
		"vsin.s   S000, S000\n" 		// S000 = vfpu_sin(S000);				(���v����? clock(4 clock����), latency3)
		"vf2iu.s  S000, S000,    8\n"	// float��int�ϊ��Ɠ�����256�{����B	(���v����1clock, latency 0)
		"mfv      %0,   S000\n" 		// result = (S000); 					(���v���ԕs��(���Ԃ�1clock), latency �s��)

//		: "=r"(result) : "r"(angle_float));
//		: "=r"(result) : "r"(angle_1024));
		: "=r"(result_t256) : "r"(angle_1024));

//	result_t256 = (result*256.0f);
	return (result_t256);
}
#if 0/*(�_��)*/
int int256_sin65536(int angle_65536)
{
//	float angle_float;
//	angle_float = angle_1024;
//	float result;
	int result_t256;
	__asm__ volatile (
		"mtv      %1,   S000\n" 		// S000 = (angle_1024);�B				(���v���ԕs��(���Ԃ�1clock), latency �s��)
	//	"vcst.s   S001, VFPU_2_PI\n"	// S001 = (2/PI);
	//	"vmul.s   S000, S000, S001\n"	// x = x * (2/PI);
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (2.0f/512.0f);	// constant real
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (1.0f/256.0f);	// constant real
	//	"vmul.s   S000, S000, S001\n"	// x = x * (S001)
	/*(�_��)*/	"vi2f.s   S000, S000,    14\n"	// int��float�ϊ��Ɠ�����1/16384�{����B	(���v����1clock, latency 0)
		"vsin.s   S000, S000\n" 		// S000 = vfpu_sin(S000);				(���v����? clock(4 clock����), latency3)
	/*(�_��)*/	"vf2iu.s  S000, S000,    14\n"	// float��int�ϊ��Ɠ�����16384�{����B	(���v����1clock, latency 0)
		"mfv      %0,   S000\n" 		// result = (S000); 					(���v���ԕs��(���Ԃ�1clock), latency �s��)

//		: "=r"(result) : "r"(angle_float));
//		: "=r"(result) : "r"(angle_1024));
		: "=r"(result_t256) : "r"(angle_65536));

//	result_t256 = (result*256.0f);
	return (result_t256);
}
#endif

