#include "psp_vfpu.h"
/*
	ease in/out �Ƃ����̂́A
	�A�j���[�V�����œ����̋��オ
	�u�ŏ��ƍŌ���������ŁA�Ԃ͑����B�v�Ƃ��������ł��B
	linear				�������
	ease-in 			�n�߂����������
	ease-out			�Ōゾ���������
	ease-in-out 		�n�߂ƍŌゾ���������ŁA�Ԃ͑����B
	cubic-bezier		�x�W�F���
*/
// simple ease in/out function
// input will be clamped to range of (0.00f < anime_time < 1.00f).

u32 vfpu_ease_in_out65536(u32 in_anime_time)
{
	u32 out_anime_time;
	__asm__ volatile (
		"mtv     %1, S000\n"							// S000 = (in_anime_time);
	#if 0
		"vi2f.s  S000, S000,   16\n"					// int��float�ϊ��Ɠ�����1/65536�{����B	(���v����1clock, latency 0)
		"vmov.q  C000[0:1,,,], C000[x, 2, 1, 1/2]\n"	// S000 = clip_range(0.0 < S000 < 1.0); 	S001 = (2);	S002 = (1); S003 = (1/2);		// C000 = [0<in_anime_time<1, 2, 1, 1/2];
		#if 0
		"vmul.s  S000, S000, S001\n"					// S000 *= (2.0);				// raise S000, (0 ... 1 range) to (0 ... 2 range);
		#else
		"vadd.s  S000, S000, S000\n"					// S000 += S000;				// raise S000, (0 ... 1 range) to (0 ... 2 range);
		#endif
		"vsub.s  S000, S000, S002\n"					// S000 -= S002;				// S000 -= (1.0);	// S000 now in -1 to 1 range;
		"vsin.s  S000, S000\n"							// S000 = vfpu_sin(S000);		// S000 = sin(in_anime_time);
		#if 0
		"vmul.s  S000, S000, S003\n"					// S000 *= S003;				// S000 = sin(in_anime_time)/2;
		"vadd.s  S000, S000, S003\n"					// S000 += S003;				// S000 = 0.5 + sin(in_anime_time)/2;
		"vf2iu.s  S000, S000,   16\n"					// float��int�ϊ��Ɠ�����65536�{����B	(���v����1clock, latency 0)
		#else
		"vadd.s  S000, S000, S002\n"					// S000 += S002;				// S000 = 1 + sin(in_anime_time);
		"vf2iu.s  S000, S000,   15\n"					// float��int�ϊ��Ɠ�����32768�{����B	(���v����1clock, latency 0)
		#endif
	#else
		#if 0
		"vi2f.s  S000, S000,   16\n"					// int��float�ϊ��Ɠ�����1/65536�{����B	(���v����1clock, latency 0)
		"vmov.p  C000[0:1,], C000[x, 1]\n"				// S000 = clip_range(0.0 < S000 < 1.0); 	S001 = (1.0);	// C000 = [0<in_anime_time<1, 1];
		"vadd.s  S000, S000, S000\n"					// S000 += S000;				// raise S000, (0 ... 1 range) to (0 ... 2 range);
		#else
		"vi2f.s  S000, S000,   16\n"					// int��float�ϊ��Ɠ�����1/65536�{����B	(���v����1clock, latency 0)
		"vmov.p  C000[0:1,], C000[x, 1]\n"				// S000 = clip_range(0.0 < S000 < 1.0); 	S001 = (1.0);	// C000 = [0<in_anime_time<1, 1];
		"vadd.s  S000, S000, S000\n"					// S000 += S000;				// raise S000, (0 ... 1 range) to (0 ... 2 range);
		#endif
		"vsub.s  S000, S000, S001\n"					// S000 -= S001;				// S000 -= (1.0);	// S000 now in -1 to 1 range;
		"vsin.s  S000, S000\n"							// S000 = vfpu_sin(S000);		// S000 = sin(in_anime_time);
		"vadd.s  S000, S000, S001\n"					// S000 += S001;				// S000 = 1 + sin(in_anime_time);
		"vf2iu.s  S000, S000,   15\n"					// float��int�ϊ��Ɠ�����32768�{����B	(���v����1clock, latency 0)
	#endif
		"mfv     %0, S000\n"							// out_anime_time = S000;
	: "=r"(out_anime_time) : "r"(in_anime_time));
	return (out_anime_time);
}
