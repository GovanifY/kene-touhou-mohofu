
/*---------------------------------------------------------
	ÉåÉ~ÉäÉA Ç¡Ç€Ç¢Ç‡ÇÃ
	-------------------------------------------------------

---------------------------------------------------------*/
static void remilia_shot00(SPRITE *src)
{
	#if (1)
//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
	#endif
//
	// Å°  [ëÂã íe]
//	br.BULLET_REGIST_speed256			= (t256(3.5));							/* íeë¨ */
//	br.BULLET_REGIST_speed256			= (t256(2.25)); 							/* íeë¨ */
	br.BULLET_REGIST_speed256			= (t256(6.00)); 							/* íeë¨ */
	br.BULLET_REGIST_div_angle65536 	= (int)(65536/48);						/* ï™äÑäpìx */
//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*åªç›éÌóﬁÇ™ñ≥Ç¢*/
	br.BULLET_REGIST_bullet_obj_type	= BULLET_OODAMA32_01_AKA;				/* [ê‘ëÂã íe] */
	br.BULLET_REGIST_angle65536 		= src->tmp_angleCCW65536;/*é©ã@ÇÀÇÁÇ¢äp*/
	br.BULLET_REGIST_n_way				= (1);									/* [1way] */
//	br.BULLET_REGIST_speed_offset		= -t256(1);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_speed_offset		= -t256(4);/* å∏ë¨íeÅH */

	bullet_regist_angle();
	// Å°  [íÜã íe]
	br.BULLET_REGIST_speed256			= (t256(4.00)); 							/* íeë¨ */
	br.BULLET_REGIST_speed_offset		= -t256(2);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU10_01_AKA; 			/* [ê‘ëÂíÜíe] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_angle65536 		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/*é©ã@ÇÀÇÁÇ¢äp*/
		bullet_regist_angle();
	}
	// Å°  [è¨ã íe]
	br.BULLET_REGIST_speed256			= (t256(2.00)); 							/* íeë¨ */
	br.BULLET_REGIST_speed_offset		= -t256(1);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_01_AKA;				/* [ê‘ëÂíÜíe] */
	for (ii=0; ii<(0x02<<difficulty); ii++)/*(16)*/
	{
		br.BULLET_REGIST_angle65536 		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/*é©ã@ÇÀÇÁÇ¢äp*/
		bullet_regist_angle();
	}

}
/*---------------------------------------------------------
	ÉåÉ~ÉäÉA í èÌçUåÇ2 Ç¡Ç€Ç¢Ç‡ÇÃ
	-------------------------------------------------------
	1ïbåÇÇ¡ÇΩÇÁÅA3ïbãxÇﬁ(à⁄ìÆ)ÅB

---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

static void danmaku_create_1f_remilia_kougeki2(SPRITE *src)
{
//	if (0xc0==((src->boss_base_danmaku_time_out)&0xc0))
	if (0xc0==((src->boss_base_danmaku_time_out)&0xc7))/* ÇWâÒÇ…àÍâÒ */
	{
				/* íÜêSç¿ïWÇ»ÇÃÇ≈ÅAÉIÉtÉZÉbÉgÇ»Çµ==É{ÉXíÜêSÇ©ÇÁíeèoÇ∑ÅB */
		//	obj_send1->cx256 					= (src->cx256)+t256(24.0);							/* íeåπx256 */
		//	obj_send1->cy256 					= (src->cy256)+t256( 8.0);							/* íeåπy256 */
			obj_send1->cx256 					= (src->cx256);							/* íeåπx256 */
			obj_send1->cy256 					= (src->cy256);							/* íeåπy256 */
		#if 0
		/* åÇÇ¬É^Å[Éì 8íe */
		if (0x0000==((src->boss_base_danmaku_time_out)&0x30))
		{
			/* é©ã@ë_Ç¢åÇÇø */
			remilia_shot00(src);
		}
		else
		if (0x0100==((src->boss_base_danmaku_time_out)&0x30))
		{
			/* CW åÇÇø */
			remilia_shot00(src);
		}
		else
		if (0x0200==((src->boss_base_danmaku_time_out)&0x30))
		{
			/* CCW åÇÇø */
			remilia_shot00(src);
		}
		else
	//	if (0x0300==((src->boss_base_danmaku_time_out)&0x30))
		{
		//	/* àÍãCåÇÇø */
			/* é©ã@ë_Ç¢åÇÇø */
			remilia_shot00(src);
		}
		#endif

		if (0==(((src->boss_base_danmaku_time_out)+0x100)&0x200))
		{
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/*é©ã@ÇÀÇÁÇ¢äpçÏê¨*/
			}
			src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/*é©ã@ÇÀÇÁÇ¢äp*/
			#if (1)
			src->tmp_angleCCW65536		-= (ra_nd()) & ((65536/32)-1);/* [ì‰ÇÃèCê≥] åªèÛë_ÇÌÇ»Ç¢ÇÃÇ≈ÉQÅ[ÉÄÇ…Ç»ÇÁÇ»Ç¢Ç©ÇÁédï˚Ç»Ç≠ÅB */
			#endif
		}
		else
		{
			src->tmp_angleCCW65536		+= (((src->boss_base_danmaku_time_out)&0x20)<<5)-( ((65536/24)-1));/* */
		}
		remilia_shot00(src);
	}
//	else
//	{
//		/* ãxÇ› */
//		;
//	}
}
