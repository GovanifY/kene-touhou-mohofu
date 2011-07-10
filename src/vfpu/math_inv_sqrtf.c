
#if 1	//0=fast, 1=original //Corn
inline float vfpu_invSqrt(float x)
{
//	return 1.0f/sqrtf(x);

	float result;
	__asm__ volatile (
		"mtv		%0, S000\n"
		"vrsq.s		S000, S000\n"
		"mfv		%0, S000\n"
	: "=r"(result): "r"(x));
	return result;
}
#else
inline float vfpu_invSqrt(float x)	//Trick using int/float to get 1/SQRT() fast on FPU/CPU //Corn
{
 	union
		{
		int itg;
		float flt;
		} c;
	c.flt = x;
	c.itg = 0x5f375a86 - (c.itg >> 1);
	return 0.5f * c.flt *(3.0f - x * c.flt * c.flt );
}
#endif
