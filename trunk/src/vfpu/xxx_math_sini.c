
#include "psp_vfpu.h"

/*---------------------------------------------------------
	sin
---------------------------------------------------------*/
//		"vi2f.p     R102,   R102,   1/2\n"		//int→float変換
//		"vi2f.s   S000, S000,    1\n"	// int→float変換と同時に1/  2倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    2\n"	// int→float変換と同時に1/  4倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    3\n"	// int→float変換と同時に1/  8倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    4\n"	// int→float変換と同時に1/ 16倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    5\n"	// int→float変換と同時に1/ 32倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    6\n"	// int→float変換と同時に1/ 64倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    7\n"	// int→float変換と同時に1/128倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,    8\n"	// int→float変換と同時に1/256倍する。	(所要時間1clock, latency 0)
//	???	動作しない
//		"vi2f.s   S000, S000,    9\n"	// int→float変換と同時に1/512倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   10\n"	// int→float変換と同時に1/1024倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   11\n"	// int→float変換と同時に1/2048倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   12\n"	// int→float変換と同時に1/4096倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   13\n"	// int→float変換と同時に1/8192倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   14\n"	// int→float変換と同時に1/16384倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   15\n"	// int→float変換と同時に1/32768倍する。	(所要時間1clock, latency 0)
//		"vi2f.s   S000, S000,   16\n"	// int→float変換と同時に1/65536倍する。	(所要時間1clock, latency 0)

int int256_sin1024(int angle_1024)
{
//	float angle_float;
//	angle_float = angle_1024;
//	float result;
	int result_t256;
	__asm__ volatile (
		"mtv      %1,   S000\n" 		// S000 = (angle_1024);。				(所要時間不明(たぶん1clock), latency 不明)
	//	"vcst.s   S001, VFPU_2_PI\n"	// S001 = (2/PI);
	//	"vmul.s   S000, S000, S001\n"	// x = x * (2/PI);
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (2.0f/512.0f);	// constant real
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (1.0f/256.0f);	// constant real
	//	"vmul.s   S000, S000, S001\n"	// x = x * (S001)
		"vi2f.s   S000, S000,    8\n"	// int→float変換と同時に1/256倍する。	(所要時間1clock, latency 0)
		"vsin.s   S000, S000\n" 		// S000 = vfpu_sin(S000);				(所要時間? clock(4 clock未満), latency3)
		"vf2iu.s  S000, S000,    8\n"	// float→int変換と同時に256倍する。	(所要時間1clock, latency 0)
		"mfv      %0,   S000\n" 		// result = (S000); 					(所要時間不明(たぶん1clock), latency 不明)

//		: "=r"(result) : "r"(angle_float));
//		: "=r"(result) : "r"(angle_1024));
		: "=r"(result_t256) : "r"(angle_1024));

//	result_t256 = (result*256.0f);
	return (result_t256);
}
#if 0/*(ダメ)*/
int int256_sin65536(int angle_65536)
{
//	float angle_float;
//	angle_float = angle_1024;
//	float result;
	int result_t256;
	__asm__ volatile (
		"mtv      %1,   S000\n" 		// S000 = (angle_1024);。				(所要時間不明(たぶん1clock), latency 不明)
	//	"vcst.s   S001, VFPU_2_PI\n"	// S001 = (2/PI);
	//	"vmul.s   S000, S000, S001\n"	// x = x * (2/PI);
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (2.0f/512.0f);	// constant real
	//	"vfim.s   S001, 0.00390625\n"	// S001 = (1.0f/256.0f);	// constant real
	//	"vmul.s   S000, S000, S001\n"	// x = x * (S001)
	/*(ダメ)*/	"vi2f.s   S000, S000,    14\n"	// int→float変換と同時に1/16384倍する。	(所要時間1clock, latency 0)
		"vsin.s   S000, S000\n" 		// S000 = vfpu_sin(S000);				(所要時間? clock(4 clock未満), latency3)
	/*(ダメ)*/	"vf2iu.s  S000, S000,    14\n"	// float→int変換と同時に16384倍する。	(所要時間1clock, latency 0)
		"mfv      %0,   S000\n" 		// result = (S000); 					(所要時間不明(たぶん1clock), latency 不明)

//		: "=r"(result) : "r"(angle_float));
//		: "=r"(result) : "r"(angle_1024));
		: "=r"(result_t256) : "r"(angle_65536));

//	result_t256 = (result*256.0f);
	return (result_t256);
}
#endif

