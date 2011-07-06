static Vertex_cube 	__attribute__((aligned(16))) aaa_polygon_model[(8*3)] =
{
	/* テクスチャの位置 				頂点の位置座標 */
	/* u	v  色(ABGR8888) 		     x    y    z   */
	{  0,	0, MY_COLOR_ARGB888_WHITE,-63,-63, 63}, // #0	// 0	/* 青 */
	{255, 255, MY_COLOR_ARGB888_BLUE,  63, 63, 63}, // #2	// 5	/* 青 青 */
	{255,	0, MY_COLOR_ARGB888_WHITE,-63, 63, 63}, // #1	// 4[]

	{  0,	0, MY_COLOR_ARGB888_WHITE,-63,-63, 63}, // #0	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE, 63,-63, 63}, // #3	// 1
	{255, 255, MY_COLOR_ARGB888_BLUE,  63, 63, 63}, // #2	// 5	/* 青 青 */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-63,-63,-63}, // #4	// 3[]	/* 青白 */
	{255, 255, MY_COLOR_ARGB888_WHITE, 63, 63,-63}, // #6	// 6
	{255,	0, MY_COLOR_ARGB888_WHITE, 63,-63,-63}, // #5	// 2

	{  0,	0, MY_COLOR_ARGB888_WHITE,-63,-63,-63}, // #4	// 3[]
	{  0, 255, MY_COLOR_ARGB888_WHITE,-63, 63,-63}, // #7	// 7
	{255, 255, MY_COLOR_ARGB888_WHITE, 63, 63,-63}, // #6	// 6
//
	{  0,	0, MY_COLOR_ARGB888_WHITE, 63,-63,-63}, // #5	// 0	/* 緑 */
	{255, 255, MY_COLOR_ARGB888_GREEN, 63, 63, 63}, // #2	// 7	/* 緑 緑 */
	{255,	0, MY_COLOR_ARGB888_WHITE, 63,-63, 63}, // #3	// 3

	{  0,	0, MY_COLOR_ARGB888_WHITE, 63,-63,-63}, // #5	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE, 63, 63,-63}, // #6	// 4
	{255, 255, MY_COLOR_ARGB888_GREEN, 63, 63, 63}, // #2	// 7	/* 緑 緑 */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-63,-63,-63}, // #4	// 0[]	/* 白 */
	{255, 255, MY_COLOR_ARGB888_WHITE,-63, 63, 63}, // #1	// 7[]
	{255,	0, MY_COLOR_ARGB888_WHITE,-63, 63,-63}, // #7	// 3

	{  0,	0, MY_COLOR_ARGB888_WHITE,-63,-63,-63}, // #4	// 0[]
	{  0, 255, MY_COLOR_ARGB888_WHITE,-63,-63, 63}, // #0	// 4
	{255, 255, MY_COLOR_ARGB888_WHITE,-63, 63, 63}, // #1	// 7[]
//


};

#if (1==TEST_AUTO_ROT)
static void test_auto_rot_bbbb(void)
{
	vfpu_quaternion_copy(&p9_qa, &p9_qb);
//	vfpu_quaternion_from_euler(&p9_qb,
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f));
	vfpu_quaternion_from_euler512_int(&p9_qb,
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f));
		(( (  0))&(512-1)),	/* x */
		(( (  0))&(512-1)),	/* y */
		(( (  0))&(512-1)));	/* z */
//	vfpu_quaternion_exp(&p9_qb, &p9_qb);
	vfpu_quaternion_ln(&p9_qb, &p9_qb);
}
#endif

static void gu_draw_bg_3D_test02(void)
{

	// when we reach the limit of our interpolation:
	//		copy qb to qa
	//		generate a new random quaternion in qb
	//		take the log of quaternion qb
	//		reset the time counter

	nnn++;
	if (nnn >= (5*60) ) /* 300==(5[sec]*60[flame])*/
	{
		nnn = 0;
		test_auto_rot_bbbb();
	}


	// to build the view matrix:
	//		get the interpolated quaternion in qcam
	//		take the exp of qcam
	//		build a rotation matrix from qcam, place into view matrix
	//
	// here we use the function 0.5 + (cos(t) * 0.5) for our 'time' variable
	// this gives us the smooth ease in/ease out motion you see as the demo
	// interpolates between qa and qb
	//
	// this is why we must do 300-n, as cos(0) = 1, cos(PI) = -1, we want
	// the reverse, -1 to 1
	#if (1==TEST_AUTO_ROT)
	{
	//	float t = (nnn)*((float)(1.0f/300.0f));
		u32 t65536 = (nnn)*((u32)(65536.0/300.0));
	//	vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
	u32 i_rate65536;
	//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
		i_rate65536 = (int)(vfpu_ease_in_out65536(t65536));
		vfpu_quaternion_sample_linear65536(&p9_qcam, &p9_qa, &p9_qb, i_rate65536);
	}
	vfpu_quaternion_exp(&p9_qcam, &p9_qcam);
	vfpu_quaternion_to_matrix(&p9_qcam, &m4_view);
	#endif

//
	#if 0
	p9_test.x = val_x;
	p9_test.y = val_y;
	p9_test.z = val_z;
	p9_test.w = val_w;
//
	vfpu_quaternion_to_matrix(&p9_test, &m4_projection3d);
	#endif
//
		// load our generated matrix data into the display list
		sceGuSetMatrix(GU_PROJECTION,	&m4_projection3d);		/* 透視変換用マトリクス */
		sceGuSetMatrix(GU_VIEW, 		&m4_view);				/* 視点変換ビュー用マトリクス */
		sceGuSetMatrix(GU_MODEL,		&m4_model); 			/* ワールド座標用マトリクス */


		// draw cube
	//	sceGuFrontFace(GU_CCW);
//		sceGumDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
		sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_8BIT | GU_COLOR_8888 | GU_VERTEX_8BIT | GU_TRANSFORM_3D, (8*3), /*0*/NULL, aaa_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_8BIT | GU_COLOR_8888 | GU_VERTEX_8BIT | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
	//	sceGuFrontFace(GU_CW);
}
