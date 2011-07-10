#ifndef _OS_TYPES_LOOKUP_H_
#define _OS_TYPES_LOOKUP_H_

#ifdef _LOW_ACCURACY_
	#define X(n) (((((n)>>22)+1)>>1) - ((((n)>>22)+1)>>9))
	#define LOOKUP_T const unsigned char
#else
	#define X(n) (n)
	#define LOOKUP_T const ogg_int32_t
#endif

#endif	/* _OS_TYPES_LOOKUP_H_ */
