
/*---------------------------------------------------------
	���~���A ���ۂ�����
	-------------------------------------------------------

---------------------------------------------------------*/
static void remilia_shot00(SPRITE *src)
{
	#if (1)
//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
	#endif
//
	// ��  [��ʒe]
//	br.BULLET_REGIST_speed256			= (t256(3.5));							/* �e�� */
//	br.BULLET_REGIST_speed256			= (t256(2.25)); 							/* �e�� */
	br.BULLET_REGIST_speed256			= (t256(6.00)); 							/* �e�� */
	br.BULLET_REGIST_div_angle65536 	= (int)(65536/48);						/* �����p�x */
//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
	br.BULLET_REGIST_bullet_obj_type	= BULLET_OODAMA32_01_AKA;				/* [�ԑ�ʒe] */
	br.BULLET_REGIST_angle65536 		= src->tmp_angleCCW65536;/*���@�˂炢�p*/
	br.BULLET_REGIST_n_way				= (1);									/* [1way] */
//	br.BULLET_REGIST_speed_offset		= -t256(1);/* �����e�H */
	br.BULLET_REGIST_speed_offset		= -t256(4);/* �����e�H */

	bullet_regist_angle();
	// ��  [���ʒe]
	br.BULLET_REGIST_speed256			= (t256(4.00)); 							/* �e�� */
	br.BULLET_REGIST_speed_offset		= -t256(2);/* �����e�H */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU10_01_AKA; 			/* [�ԑ咆�e] */
	u32 ii;
	for (ii=0; ii<8; ii++)
	{
		br.BULLET_REGIST_angle65536 		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/8)-1)) - ((65536/16)-1);/*���@�˂炢�p*/
		bullet_regist_angle();
	}
	// ��  [���ʒe]
	br.BULLET_REGIST_speed256			= (t256(2.00)); 							/* �e�� */
	br.BULLET_REGIST_speed_offset		= -t256(1);/* �����e�H */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_01_AKA;				/* [�ԑ咆�e] */
	for (ii=0; ii<(0x02<<difficulty); ii++)/*(16)*/
	{
		br.BULLET_REGIST_angle65536 		= src->tmp_angleCCW65536 + ((ra_nd()) & ((65536/4)-1)) - ((65536/8)-1);/*���@�˂炢�p*/
		bullet_regist_angle();
	}

}
/*---------------------------------------------------------
	���~���A �ʏ�U��2 ���ۂ�����
	-------------------------------------------------------
	1�b��������A3�b�x��(�ړ�)�B

---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

static void danmaku_create_1f_remilia_kougeki2(SPRITE *src)
{
//	if (0xc0==((src->boss_base_danmaku_time_out)&0xc0))
	if (0xc0==((src->boss_base_danmaku_time_out)&0xc7))/* �W��Ɉ�� */
	{
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
		//	obj_send1->cx256 					= (src->cx256)+t256(24.0);							/* �e��x256 */
		//	obj_send1->cy256 					= (src->cy256)+t256( 8.0);							/* �e��y256 */
			obj_send1->cx256 					= (src->cx256);							/* �e��x256 */
			obj_send1->cy256 					= (src->cy256);							/* �e��y256 */
		#if 0
		/* ���^�[�� 8�e */
		if (0x0000==((src->boss_base_danmaku_time_out)&0x30))
		{
			/* ���@�_������ */
			remilia_shot00(src);
		}
		else
		if (0x0100==((src->boss_base_danmaku_time_out)&0x30))
		{
			/* CW ���� */
			remilia_shot00(src);
		}
		else
		if (0x0200==((src->boss_base_danmaku_time_out)&0x30))
		{
			/* CCW ���� */
			remilia_shot00(src);
		}
		else
	//	if (0x0300==((src->boss_base_danmaku_time_out)&0x30))
		{
		//	/* ��C���� */
			/* ���@�_������ */
			remilia_shot00(src);
		}
		#endif

		if (0==(((src->boss_base_danmaku_time_out)+0x100)&0x200))
		{
			{
				SPRITE *zzz_player;
				zzz_player = &obj00[FIX_OBJ_00_PLAYER];
				tmp_angleCCW65536_src_nerai(zzz_player, obj_send1);/*���@�˂炢�p�쐬*/
			}
			src->tmp_angleCCW65536		= obj_send1->tmp_angleCCW65536;/*���@�˂炢�p*/
			#if (1)
			src->tmp_angleCCW65536		-= (ra_nd()) & ((65536/32)-1);/* [��̏C��] ����_��Ȃ��̂ŃQ�[���ɂȂ�Ȃ�����d���Ȃ��B */
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
//		/* �x�� */
//		;
//	}
}
