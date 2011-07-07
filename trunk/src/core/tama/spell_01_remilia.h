
/*---------------------------------------------------------
 ìåï˚ñÕïÌïó Å` Toho Imitation Style.
  ÉvÉçÉWÉFÉNÉgÉyÅ[ÉW http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	ÉåÉ~ÉäÉA Ç¡Ç€Ç¢íeñãÇê∂ê¨ÇµÇ‹Ç∑ÅB
---------------------------------------------------------*/

static void remilia_oyasumi_kougeki(SPRITE *src)
{
	#if (0)
//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
	#endif
//
	// Å°  [ëÂã íe]
//	br.BULLET_REGIST_00_speed256			= (t256(3.5));								/* íeë¨ */
//	br.BULLET_REGIST_00_speed256			= (t256(2.25)); 							/* íeë¨ */
	br.BULLET_REGIST_00_speed256			= (t256(6.00)); 							/* íeë¨ */
//	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_01_speed_offset		= -t256(4);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_02_angle65536			= (16384)+(((src->boss_base_spell_time_out)&0x40)<<8);/* é©ã@ë_Ç¢äp */
	br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)îÒåXÇ´íe */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;				/* [ê‘ëÂã íe] */
//ñ¢íËbr.BULLET_REGIST_05_regist_type 		= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)ïWèÄíe */
	tama_system_regist_single();
	// Å°  [íÜã íe]
	br.BULLET_REGIST_00_speed256			= (t256(4.00)); 							/* íeë¨ */
	br.BULLET_REGIST_01_speed_offset		= -t256(2);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MARU10_01_AKA; 			/* [ê‘ëÂíÜíe] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* é©ã@ë_Ç¢äp */
		tama_system_regist_single();
	}
	// Å°  [è¨ã íe]
	br.BULLET_REGIST_00_speed256			= (t256(2.00)); 							/* íeë¨ */
	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;				/* [ê‘ëÂíÜíe] */
	for (ii=0; ii<(0x02<<(cg_game_difficulty)); ii++)/*(16)*/
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* é©ã@ë_Ç¢äp */
		tama_system_regist_single();
	}
}


static void remilia_shot00(SPRITE *src)
{
	#if (1)
//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
	#endif
//
	// Å°  [ëÂã íe]
//	br.BULLET_REGIST_00_speed256			= (t256(3.5));							/* íeë¨ */
//	br.BULLET_REGIST_00_speed256			= (t256(2.25)); 							/* íeë¨ */
	br.BULLET_REGIST_00_speed256			= (t256(6.00)); 							/* íeë¨ */
//	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_01_speed_offset		= -t256(4);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_02_angle65536			= src->tmp_angleCCW65536;/* é©ã@ë_Ç¢äp */
	br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)îÒåXÇ´íe */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;				/* [ê‘ëÂã íe] */
//ñ¢íËbr.BULLET_REGIST_05_regist_type 		= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)ïWèÄíe */

	tama_system_regist_single();
	// Å°  [íÜã íe]
	br.BULLET_REGIST_00_speed256			= (t256(4.00)); 							/* íeë¨ */
	br.BULLET_REGIST_01_speed_offset		= -t256(2);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MARU10_01_AKA; 			/* [ê‘ëÂíÜíe] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* é©ã@ë_Ç¢äp */
		tama_system_regist_single();
	}
	// Å°  [è¨ã íe]
	br.BULLET_REGIST_00_speed256			= (t256(2.00)); 							/* íeë¨ */
	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* å∏ë¨íeÅH */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;				/* [ê‘ëÂíÜíe] */
	for (ii=0; ii<(0x02<<(cg_game_difficulty)); ii++)/*(16)*/
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* é©ã@ë_Ç¢äp */
		tama_system_regist_single();
	}
}


/*---------------------------------------------------------
	ÉåÉ~ÉäÉA í èÌçUåÇ2 Ç¡Ç€Ç¢Ç‡ÇÃ
	-------------------------------------------------------
	1ïbåÇÇ¡ÇΩÇÁÅA3ïbãxÇﬁ(à⁄ìÆ)ÅB

---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

static void spell_create_1f_remilia_kougeki2(SPRITE *src)
{
	if (0x80==((src->boss_base_spell_time_out)&0x80))/* å„îº */
	{
	//	if (0xc0==((src->boss_base_spell_time_out)&0xc0))
		if (0xc0==((src->boss_base_spell_time_out)&0xc7))/* ÇWâÒÇ…àÍâÒ */
		{
				obj_send1->cx256					= (src->cx256); 	/* íeåπx256 É{ÉXíÜêSÇ©ÇÁî≠íeÅB */
				obj_send1->cy256					= (src->cy256); 	/* íeåπy256 É{ÉXíÜêSÇ©ÇÁî≠íeÅB */
			#if 0
			/* åÇÇ¬É^Å[Éì 8íe */
			if (0x0000==((src->boss_base_spell_time_out)&0x30))
			{
				/* é©ã@ë_Ç¢åÇÇø */
				remilia_shot00(src);
			}
			else
			if (0x0100==((src->boss_base_spell_time_out)&0x30))
			{
				/* CW åÇÇø */
				remilia_shot00(src);
			}
			else
			if (0x0200==((src->boss_base_spell_time_out)&0x30))
			{
				/* CCW åÇÇø */
				remilia_shot00(src);
			}
			else
		//	if (0x0300==((src->boss_base_spell_time_out)&0x30))
			{
			//	/* àÍãCåÇÇø */
				/* é©ã@ë_Ç¢åÇÇø */
				remilia_shot00(src);
			}
			#endif
			if (0==(((src->boss_base_spell_time_out)+0x100)&0x200))
			{
				{
					SPRITE *zzz_player;
					zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
					tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/* é©ã@ë_Ç¢äpçÏê¨ */
				}
				src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/* é©ã@ë_Ç¢äp */
			}
			else
			{
				src->tmp_angleCCW65536		+= (((src->boss_base_spell_time_out)&0x20)<<5)-( ((65536/24)-1));/* */
			}
			remilia_shot00(src);
		}
	}
	/* ñ{ìñÇÕãxÇ›ÇæÇØÇ«ÅAÇ†Ç‹ÇËÇ…ä»íPÇ»ÇÃÇ≈çUåÇ */
	else
	{
/*
8421
1111 f	- - - -
1110 e	- - - -
1101 d	- - - -
1100 c	- - - L
1011 b	- - - -
1010 a	- - - -
1001 9	- - - -
1000 8	- - - L
0111 7	- - - -
0110 6	- - H -
0101 5	- - - -
0100 4	- - H L
0011 3	- - - -
0010 2	- N H -
0001 1	- - - -
0000 0	e N H L
*/
		/* ãxÇ› */
		const u8 aaa[4] =
		{
			0xff,	/* easy */
			0x0d,	/* NORMAL */
			0x09,	/* HARD */
			0x03,	/* LUNATIC */
		};
		if ( 0==((src->boss_base_spell_time_out) & aaa[(cg_game_difficulty)] ))
		{
			remilia_oyasumi_kougeki(src);
		}
	}
}
