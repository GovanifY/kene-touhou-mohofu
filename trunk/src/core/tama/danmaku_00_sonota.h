
/*---------------------------------------------------------
 “Œ•û–Í•í•—  ` Toho Imitation Style.
  ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	‚»‚Ì‘¼‚Ì’e–‹‚ð¶¬‚µ‚Ü‚·B
---------------------------------------------------------*/

/*---------------------------------------------------------
	·•ªŽ‚Ì—d‰ö1(“V‹ç—l?)‚ªŒ‚‚Â’e–‹(—\’è)
	-------------------------------------------------------
	–¢ì¬
---------------------------------------------------------*/
static void danmaku_create_11_tengu_shot(SPRITE *src)
{
	static int aaa_angle1024;
	static int data_wait2_start;/* ”­’eŠÔŠustart */
	static int data_wait2;/* ”­’eŠÔŠu */

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		data_wait2_start	= (20-((difficulty)<<2));	/*8*/ /*10*/
		data_wait2			= 10;
		tmp_angleCCW1024_jikinerai(obj_player, src);/*Ž©‹@‚Ë‚ç‚¢Špì¬*/
		aaa_angle1024		= src->tmp_angleCCW1024;/*Ž©‹@‚Ë‚ç‚¢Šp*/
		aaa_angle1024		-= (int)(1024*(5)/(32));/* 5’e–Ú‚ªŽ©‹@‘_‚¢ */
		/* Šp“x(1024[360/360“x]‚ð 32•ªŠ„) */
	}
	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16‰ñ */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			obj_send1->y256 						= (src->y256);								/* ’eŒ¹y256 */	/* ”­’eˆÊ’u‚Ì offset —p */
			{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int tama_color;/* ’eF */
			int bbb;/* •ûŒü */
				if (0x00==((src->boss_base_danmaku_time_out)&0x20))
						{	bbb=-1;tama_color=0;/* ˜g‚Â‚«Â’e */}
				else	{	bbb= 1;tama_color=1;/* ˜g‚Â‚«—Î’e */}
			//	obj_send1->x256 = src->x256+((16-(iii&0x0f))<<(3+8))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
				obj_send1->x256 = src->x256+((16-(iii&0x0f))<<(3+7))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
				br.BULLET_REGIST_speed256			= (t256(1.5));								/* ’e‘¬ */
				br.BULLET_REGIST_angle1024			= ((/*0+*/(aaa_angle1024)*bbb)&(1024-1));	/* ’eŒ¹Šp“x1024 */
		//		br.BULLET_REGIST_div_angle1024		= (0);										/* ƒ_ƒ~[•ªŠ„Šp“x(–¢Žg—p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [˜g‚Â‚«Â’e] [˜g‚Â‚«—Î’e] */ 	/*BULLET_MARU12_03_AOI*/
				br.BULLET_REGIST_n_way				= (1);										/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			/* ŽŸ‚Ì’e‚ÌŠp“x */
			aaa_angle1024 += (int)(1024/(32));/* Šp“x(1024[360/360“x]‚ð 32•ªŠ„) */
		}
	}
}
