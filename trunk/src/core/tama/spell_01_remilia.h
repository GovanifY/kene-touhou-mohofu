
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���~���A ���ۂ��e���𐶐����܂��B
---------------------------------------------------------*/

static void remilia_oyasumi_kougeki(SPRITE *src)
{
	#if (0)
//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
	#endif
//
	// ��  [��ʒe]
//	br.BULLET_REGIST_00_speed256			= (t256(3.5));								/* �e�� */
//	br.BULLET_REGIST_00_speed256			= (t256(2.25)); 							/* �e�� */
	br.BULLET_REGIST_00_speed256			= (t256(6.00)); 							/* �e�� */
//	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* �����e�H */
	br.BULLET_REGIST_01_speed_offset		= -t256(4);/* �����e�H */
	br.BULLET_REGIST_02_angle65536			= (16384)+(((src->boss_base_spell_time_out)&0x40)<<8);/* ���@�_���p */
	br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;				/* [�ԑ�ʒe] */
//����br.BULLET_REGIST_05_regist_type 		= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
	tama_system_regist_single();
	// ��  [���ʒe]
	br.BULLET_REGIST_00_speed256			= (t256(4.00)); 							/* �e�� */
	br.BULLET_REGIST_01_speed_offset		= -t256(2);/* �����e�H */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MARU10_01_AKA; 			/* [�ԑ咆�e] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* ���@�_���p */
		tama_system_regist_single();
	}
	// ��  [���ʒe]
	br.BULLET_REGIST_00_speed256			= (t256(2.00)); 							/* �e�� */
	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* �����e�H */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;				/* [�ԑ咆�e] */
	for (ii=0; ii<(0x02<<(cg_game_difficulty)); ii++)/*(16)*/
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* ���@�_���p */
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
	// ��  [��ʒe]
//	br.BULLET_REGIST_00_speed256			= (t256(3.5));							/* �e�� */
//	br.BULLET_REGIST_00_speed256			= (t256(2.25)); 							/* �e�� */
	br.BULLET_REGIST_00_speed256			= (t256(6.00)); 							/* �e�� */
//	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* �����e�H */
	br.BULLET_REGIST_01_speed_offset		= -t256(4);/* �����e�H */
	br.BULLET_REGIST_02_angle65536			= src->tmp_angleCCW65536;/* ���@�_���p */
	br.BULLET_REGIST_03_tama_data   		= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;				/* [�ԑ�ʒe] */
//����br.BULLET_REGIST_05_regist_type 		= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */

	tama_system_regist_single();
	// ��  [���ʒe]
	br.BULLET_REGIST_00_speed256			= (t256(4.00)); 							/* �e�� */
	br.BULLET_REGIST_01_speed_offset		= -t256(2);/* �����e�H */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MARU10_01_AKA; 			/* [�ԑ咆�e] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/* ���@�_���p */
		tama_system_regist_single();
	}
	// ��  [���ʒe]
	br.BULLET_REGIST_00_speed256			= (t256(2.00)); 							/* �e�� */
	br.BULLET_REGIST_01_speed_offset		= -t256(1);/* �����e�H */
	br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;				/* [�ԑ咆�e] */
	for (ii=0; ii<(0x02<<(cg_game_difficulty)); ii++)/*(16)*/
	{
		br.BULLET_REGIST_02_angle65536		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/* ���@�_���p */
		tama_system_regist_single();
	}
}


/*---------------------------------------------------------
	���~���A �ʏ�U��2 ���ۂ�����
	-------------------------------------------------------
	1�b��������A3�b�x��(�ړ�)�B

---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

static void spell_create_1f_remilia_kougeki2(SPRITE *src)
{
	if (0x80==((src->boss_base_spell_time_out)&0x80))/* �㔼 */
	{
	//	if (0xc0==((src->boss_base_spell_time_out)&0xc0))
		if (0xc0==((src->boss_base_spell_time_out)&0xc7))/* �W��Ɉ�� */
		{
				obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
				obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
			#if 0
			/* ���^�[�� 8�e */
			if (0x0000==((src->boss_base_spell_time_out)&0x30))
			{
				/* ���@�_������ */
				remilia_shot00(src);
			}
			else
			if (0x0100==((src->boss_base_spell_time_out)&0x30))
			{
				/* CW ���� */
				remilia_shot00(src);
			}
			else
			if (0x0200==((src->boss_base_spell_time_out)&0x30))
			{
				/* CCW ���� */
				remilia_shot00(src);
			}
			else
		//	if (0x0300==((src->boss_base_spell_time_out)&0x30))
			{
			//	/* ��C���� */
				/* ���@�_������ */
				remilia_shot00(src);
			}
			#endif
			if (0==(((src->boss_base_spell_time_out)+0x100)&0x200))
			{
				{
					SPRITE *zzz_player;
					zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
					tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/* ���@�_���p�쐬 */
				}
				src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/* ���@�_���p */
			}
			else
			{
				src->tmp_angleCCW65536		+= (((src->boss_base_spell_time_out)&0x20)<<5)-( ((65536/24)-1));/* */
			}
			remilia_shot00(src);
		}
	}
	/* �{���͋x�݂����ǁA���܂�ɊȒP�Ȃ̂ōU�� */
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
		/* �x�� */
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
