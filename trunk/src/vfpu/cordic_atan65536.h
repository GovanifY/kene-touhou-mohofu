// http://www.worldserver.com/turk/opensource/index.html#CORDIC
// http://teamcoil.sp.u-tokai.ac.jp/calculator/100224/index.html


//int atan_65536( int y, int x )
/*global*/ int atan_65536( int x, int y )
{
	if ((y == 0) && (x == 0))	 /*(エラーチェック。要らないかも)*/
	{
		return (0);
	}
	else
	{
		/* perform CORDIC conversion to polar coordinates */	/*(このルーチンは単純な級数展開です。)*/
		/* pseudo polarize */
//		x = (x << 8/*16*/);
//		y = (y << 8/*16*/);

		int theta;
		theta = 0;
		/* get the vector into the right half plane */
		if (x < 0)
		{
			x = -x;
			y = -y;
			theta = (2 * (1L << 14)/*QUARTER*/);
		}

		int yi;
		if (y < 0)	/* rotate positive */	 /*(正方向に回転して調べて誤差補正)*/
		{
			yi = y + (x << 1);
			x  = x - (y << 1);
			y  = yi;
			theta -= 11548; 	/* subtract angle */	/*(角度誤差補正)*/
		}
		else		/* rotate negative */	 /*(負方向に回転して調べて誤差補正)*/
		{
			yi = y - (x << 1);
			x  = x + (y << 1);
			y  = yi;
			theta = - theta;
			theta += 11548; 	/* add angle */    /*(角度誤差補正)*/
		}
		{
			static const int arc_tan_table[14] =
			{
			//	(1L << 30)	//	(1L << 14)
			//	1073741824	//	16384
			//	1		//		/= 65536
			/*--*/	//	11548,
			/*--*/		8192,
			/* 1*/		4836,
			/* 2*/		2555,
			/* 3*/		1297,
			/* 4*/		651,
			/* 5*/		326,
			/* 6*/		163,	/* もし大体200倍精度を落として400分割ぐらいならこの辺で収束(一周360度での1度単位ならこの辺) */
			/* 7*/		81,
			/* 8*/		41,
			/* 9*/		20,
			/*10*/		10,    /* もし10倍精度を落として6553分割ならここで収束 */
			/*11*/		5,
			/*12*/		3,
			/*13*/		1	 /* 65536分割ならここで収束 */
			};
			unsigned int i;
			for (i=0; i<14; i++)	/* the resolution of the arctan table */
			{
				if (y < 0)	/* rotate positive */	 /* 正方向に回転して調べて誤差補正 */
				{
					yi = y + (x >> i);
					x  = x - (y >> i);
					y  = yi;
					theta -= arc_tan_table[i];	  /* 角度誤差補正 */
				}
				else		/* rotate negative */	 /* 負方向に回転して調べて誤差補正 */
				{
					yi = y - (x >> i);
					x  = x + (y >> i);
					y  = yi;
					theta += arc_tan_table[i];	  /* 角度誤差補正 */
				}
			}
		}
		return (theta);
	}
}

#if 0
http://www.dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization
//-----------------------------------------------
// Fast arctan2
float arctan2(float y, float x)
{
	coeff_1 = M_PI/4;
	coeff_2 = 3*coeff_1;
	abs_y = fabs(y)+1e-10		// kludge to prevent 0/0 condition
	if (x >= 0)
	{
		r = (x - abs_y) / (x + abs_y);
		angle = coeff_1 - coeff_1 * r;
	}
	else
	{
		r = (x + abs_y) / (abs_y - x);
		angle = coeff_2 - coeff_1 * r;
	}
	if (y < 0)
	{	return (-angle);	}	// negate if in quad III or IV
	else
	{	return (angle); 	}
}

#endif
