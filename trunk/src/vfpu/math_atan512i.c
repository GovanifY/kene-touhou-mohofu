
#include "psp_vfpu.h"
/*---------------------------------------------------------
	atan 512 int
---------------------------------------------------------*/

static int vfpu_atani512(float xxx)
{
	float result;
	// result = asinf(x/sqrt(x*x+1))
	__asm__ volatile (
		"mtv      %1, S000\n"				// S000 = xxx;
		"vmul.s   S001, S000, S000\n"		// S001 = S000 * S000;	// S001 = (x*x);
		"vadd.s   S001, S001, S001[1]\n"	// S001 = S001 + 1;(constant 1, 特別定数1???)	// S001 = (x*x+1);
		"vrsq.s   S001, S001\n" 			// S001 = 1/sqrt(S001)
		"vmul.s   S000, S000, S001\n"		// S000 = S000 * S001;	// S000 = x*(1/sqrt(x*x+1));
		"vasin.s  S000, S000\n" 			// S000 = asinf(S000);
//		"vcst.s   S001, VFPU_PI_2\n"		// VFPU_PI_2 = PI/2;
	//	"vfim.s   S001, 128.0\n"			// S001 = 128.0;	// constant real
		"viim.s   S001, 128\n"				// S001 = 128;		// constant integer (between -32768 and 32767 ?);
		"vmul.s   S000, S000, S001\n"		// S000 = S000 * S001;
		"mfv      %0,   S000\n" 			// result = (S000); 					(所要時間不明(たぶん1clock), latency 不明)
	: "=r"(result) : "r"(xxx));
	int r512;
	r512 = (((result)) /** (256.0f/2.0f)*/);
	//	r *= (256.0f/2.0f);
	return (r512);
}

#if 1
	#include <stdlib.h>/*abs()*/
	int atan_512( int yyy, int xxx )
	{
	//	float y;	y=xxx;
	//	float x;	x=yyy;
		int r512;
		#if 1
		if ( abs(yyy) >= abs(xxx))
		{
			{
				float y;	y=xxx;
				float x;	x=yyy;
				r512 = vfpu_atani512((float)(y/x));
			}
			if (yyy < 0 )
			{
				r512 += (xxx < 0  ? -256 : 256 );
			}
		}
		else
		{
			{
				float y;	y=xxx;
				float x;	x=yyy;
				r512 = vfpu_atani512((x/y));
			}
			r512 = -r512;
			r512 += (xxx < 0  ? -(256 /2 ) : (256 /2 ));
		}
		#else
		#endif
	//	r512 = r512/**((512.0f)/(256.0f*2.0f))*/;
		r512 &= (512-1);
		return (r512);
	}
#else
	/* バグあり(もしうまくいくなら上のタイプよりこの手の方が速い。pspのcpu向き) */
	int atan_512( int xxx, int yyy )
	{
		int r512;
		r512 = 0;
		int flag;
	//
		if (xxx > 0)
		{
			if (yyy > 0)	{					flag=0; 						}
			else			{	yyy = -yyy; 	flag=1; 	r512 += 256;		}
		}
		else
		{
			xxx = -xxx;
			if (yyy > 0)	{					flag=1; 						}
			else			{	yyy = -yyy; 	flag=0; 	r512 += 256;		}
		}
	//
		{
			int aaa;
			{
				float y;	y=xxx;
				float x;	x=yyy;
				if (xxx > yyy)		{	aaa = vfpu_atani512((y / x));		}
				else				{	aaa = vfpu_atani512((x / y));		}
			}
	//
			if (0==flag)
			{
				if (xxx > yyy)		{	r512 +=   0 + aaa;		}
				else				{	r512 += 128 - aaa;		}
			}
			else
			{
				if (xxx > yyy)		{	r512 += 256 - aaa;		}
				else				{	r512 += 128 + aaa;		}
			}
		}
	//	r512 = r512/**((512.0f)/(256.0f*2.0f))*/;
		r512 &= (512-1);
		return (r512);
	}
#endif


#if 0
/* CWの場合 */
#else
/* CCWの場合 */
	/*とりあえず*/
//	#define rad2deg512(x)		( (int)((x)*((512.0)/(GU_PI*2))/*+512*/)&(512-1) )
#endif
	#if 0
/* CWの場合 */
//	return (rad2deg512(atan2(y,x)));	/* 右CWの場合。  一般的な右CW の場合。 */
	#else
/* CCWの場合 */
//	return (rad2deg512(atan2(x,y)));	/* 下CCWの場合。 (x y を入れ替えて対処してあります。) */
//	return (rad2deg512(vfpu_atan2f(x,y)));	/* 下CCWの場合。 (x y を入れ替えて対処してあります。) */
	#endif
