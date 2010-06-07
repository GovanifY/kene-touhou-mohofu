
/*---------------------------------------------------------
 “Œ•û–Í•í•—  ` Toho Imitation Style.
  ƒvƒƒWƒFƒNƒgƒy[ƒW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ƒ‹[ƒ~ƒA‚Ì’e–‹‚ğ¶¬‚µ‚Ü‚·B
---------------------------------------------------------*/

/*---------------------------------------------------------
	g1–Êƒ{ƒX ƒ‹[ƒ~ƒA –é•„uƒiƒCƒgƒo[ƒhv‚É‚¿‚å‚Á‚Æ‚¾‚¯—‚½’e–‹(—\’è)
	-------------------------------------------------------
	5’e–Ú‚ª©‹@‘_‚¢‚Ì 32•ªŠ„16•ûŒü ‹|ó’eB
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 Â(0x00==((time_out)&0x20))
0001 0000						Â(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 —Î
0011 0000						—Î
0100 0000 0x00==(time_out)&0x11 Â(0x00==((time_out)&0x20))
0101 0000						Â(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 —Î
0111 0000						—Î
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_08_night_bird(SPRITE *src)
{
	static int aaa_angle65536;
//	const int src_boss_base_danmaku_time_out = src->boss_base_danmaku_time_out;
//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		#if 1
		tmp_angleCCW65536_jikinerai(obj_player, src);/*©‹@‚Ë‚ç‚¢Špì¬*/
		aaa_angle65536 = src->tmp_angleCCW65536;/*©‹@‚Ë‚ç‚¢Šp*/
		#else
		aaa_angle65536 = (0);/* ƒeƒXƒg‰ºŒü‚« */
		#endif
	//	aaa_angle65536 += (const int)(65536-(int)((65536*5)/(32)));/* 5’e–Ú‚ª©‹@‘_‚¢ */
		aaa_angle65536 += (55296);/* 5’e–Ú‚ª©‹@‘_‚¢(65536-(5*(65536/32))) */
		mask65536(aaa_angle65536);
		/* Šp“x(65536[360/360“x]‚ğ 32•ªŠ„) */
	}
	else
	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16‰ñ */
	{
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		obj_send1->y256 						= (src->y256);		/* ’eŒ¹y256 */		/* ”­’eˆÊ’u‚Ì offset —p */
		{
			int tama_color;/* ’eF */
			int bbb;/* •ûŒü */
			int bbb_angle65536;/* •ûŒü */
			int iii;
				iii = ((src->boss_base_danmaku_time_out)&0x0f);
			if (0x00==((src->boss_base_danmaku_time_out)&0x20))
					{	bbb=-1;tama_color=0;/* ˜g‚Â‚«Â’e */bbb_angle65536=65536-aaa_angle65536;}
			else	{	bbb= 1;tama_color=1;/* ˜g‚Â‚«—Î’e */bbb_angle65536=aaa_angle65536;}
			mask65536(bbb_angle65536);
		//	obj_send1->x256 = src->x256+((16-(iii))<<(3+8))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
			obj_send1->x256 = src->x256+((16-(iii))<<(3+7))*bbb;/* –{•¨‚Í”­’eˆÊ’u‚Ì x offset ‚ ‚è */
			br.BULLET_REGIST_speed256			= (t256(1.5));								/* ’e‘¬ */
			br.BULLET_REGIST_angle65536 		= (bbb_angle65536); /* ”­Ë’†SŠp“x / “Áê‹@”\(©‹@‘_‚¢/‘¼) */
	//		br.BULLET_REGIST_div_angle1024		= (0);										/* ƒ_ƒ~[•ªŠ„Šp“x(–¢g—p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [˜g‚Â‚«Â’e] [˜g‚Â‚«—Î’e] */ 	/*BULLET_MARU12_03_AOI*/
			br.BULLET_REGIST_n_way				= (1);										/* [1way] */
			br.BULLET_REGIST_speed_offset		= (0);/*‚Ä‚·‚Æ*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*Œ»İí—Ş‚ª–³‚¢*/
			bullet_regist_angle();/* bullet_regist_vector(); ‚æ‚èáŠ±ƒVƒ“ƒvƒ‹‚Èˆ×Aˆø”‚ÉŒİŠ·‚ª‚È‚¢B */
		}
		/* Ÿ‚Ì’e‚ÌŠp“x */
		aaa_angle65536 += (int)(65536/(32));/* Šp“x(65536[360/360“x]‚ğ 32•ªŠ„) */
		mask65536(aaa_angle65536);
	}
}
