
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̑��̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�������̗d��1(�V��l?)�����e��
	-------------------------------------------------------
	28�x�� �� 100��e �� 7���� BULLET_UROKO14_03_MIDORI		speed256	= t256(3.25)+((difficulty)<<6);
	28�x�� �� 100�����e �� 8���� BULLET_UROKO14_04_MIZUIRO		speed256	= (t256(3.0)+((difficulty)<<6));
---------------------------------------------------------*/
static void danmaku_create_11_tengu_shot(SPRITE *src)
{
	int aaa;
	if (128 > ((src->boss_base_danmaku_time_out)))
	{/* �����e �� 8���� */
		aaa = 1;
	}
	else
	{/* ��e �� 7���� */
		aaa = 0;
	}
	/* ���߂� 28 flame�͂��x�� */
	if (100 > ((src->boss_base_danmaku_time_out)&0x7f))
	{
	//	static int YOKAI1_DATA_wait2	= (10-1);/*0*/
		static int YOKAI1_DATA_wait2	= 0;		/* �U��wait */
		YOKAI1_DATA_wait2--;
		if (0 > YOKAI1_DATA_wait2)
		{
			/* 0: 20 == 20-(0*4) */
			/* 1: 16 == 20-(1*4) */
			/* 2: 12 == 20-(2*4) */
			/* 3:  8 == 20-(3*4) */
			YOKAI1_DATA_wait2 = ((20-1)-((difficulty)<<2));/*(8-1)*/ /*(10-1)*/
			voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);/*�e�L�g�[*/
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256 = src->cx256;
			obj_send1->cy256 = src->cy256;
			{
				br.BULLET_REGIST_speed256			= (t256(3.0)+((difficulty)<<6))+(aaa<<(8-2));
				br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI+(aaa);
				br.BULLET_REGIST_n_way				= (7)+(aaa);
			}
			br.BULLET_REGIST_angle1024				= ANGLE_JIKI_NERAI_DAN;
			br.BULLET_REGIST_div_angle1024			= (int)(1024/24);
			br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;
			bullet_regist_vector();
		}
	}

}

#if 0
{
	static int aaa_angle1024;
	static int data_wait2_start;/* ���e�Ԋustart */
	static int data_wait2;/* ���e�Ԋu */

//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		data_wait2_start	= (20-((difficulty)<<2));	/*8*/ /*10*/
		data_wait2			= 10;
		tmp_angleCCW1024_jiki_nerai(src);/*���@�˂炢�p�쐬*/
		aaa_angle1024		= src->tmp_angleCCW1024;/*���@�˂炢�p*/
		aaa_angle1024		-= (int)(1024*(5)/(32));/* 5�e�ڂ����@�_�� */
		/* �p�x(1024[360/360�x]�� 32����) */
	}
	else
//	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16�� */
	{
		data_wait2--;
		if (1 > data_wait2)
		{
			data_wait2 = data_wait2_start;
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
			#endif
			obj_send1->cy256 						= (src->cy256);								/* �e��y256 */	/* ���e�ʒu�� offset �p */
			{
			int iii;
				iii = (src->boss_base_danmaku_time_out);
			int tama_color;/* �e�F */
			int bbb;/* ���� */
				if (0x00==((src->boss_base_danmaku_time_out)&0x20))
						{	bbb=-1;tama_color=0;/* �g���e */}
				else	{	bbb= 1;tama_color=1;/* �g���Βe */}
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			//	obj_send1->cx256 = src->cx256+((16-(iii&0x0f))<<(3+8))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
				obj_send1->cx256 = src->cx256+((16-(iii&0x0f))<<(3+7))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
				br.BULLET_REGIST_speed256			= (t256(1.5));								/* �e�� */
				br.BULLET_REGIST_angle1024			= ((/*0+*/(aaa_angle1024)*bbb)&(1024-1));	/* �e���p�x1024 */
		//		br.BULLET_REGIST_div_angle1024		= (0);										/* �_�~�[�����p�x(���g�p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [�g���e] [�g���Βe] */ 	/*BULLET_MARU12_03_AOI*/
				br.BULLET_REGIST_n_way				= (1);										/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			}
			/* ���̒e�̊p�x */
			aaa_angle1024 += (int)(1024/(32));/* �p�x(1024[360/360�x]�� 32����) */
		}
	}
}
#endif

/*---------------------------------------------------------
	�A���X normal�ʏ�U��3(�̈ꕔ)
	-------------------------------------------------------
	�Ȃ񂩑S�R�܂��������ĂȂ��B
	�d�X�����̂���őn���Ă��������Y�k���ɂȂ����B
	���������Ă�̂��킩��Ȃ��Ǝv�����A�킽���ɂ��킩��Ȃ���B
	���낵�����̂̕З؂��ȉ����B
	psp�𑜓x���S�R�Ⴄ����A�����Ɏ����悤�Ƃ��Ȃ������ǂ������H
---------------------------------------------------------*/


/* [CW ���v���] */
static void danmaku_create_20_sonota_debug_cw_ao/*CW*/(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW65536 -= (int)(65536/24);
		#if 0/*1885624*/
		src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[difficulty]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
	//
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256 						= (src->cx256);							/* �e��x256 */
			obj_send1->cy256 						= (src->cy256);							/* �e��y256 */
			//
			br.BULLET_REGIST_speed256				= (t256(1.0));	/* �e�� */
			br.BULLET_REGIST_div_angle65536 		= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x  */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_MARU12_02_AOI; 				/* [�g�t���ےe] */
			br.BULLET_REGIST_n_way					= (6);									/* [6way] */
	//
			br.BULLET_REGIST_speed_offset			= t256(1);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* ���݂��ꂵ���Ȃ����v�� */
		{
			br.BULLET_REGIST_angle65536 			= (src->tmp_angleCCW65536); 							/* �p�x */
			bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		//
			br.BULLET_REGIST_angle65536 			= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* �p�x */
			bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		}
	}
}

/*[CCW �����v���]*/
static void danmaku_create_21_sonota_debug_ccw_aka/*CCW*/(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW65536 += (int)(65536/24);
		#if 0/*1885624*/
		src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
		#else
		{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
			src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[difficulty]) );
		}
		#endif
		mask65536(src->tmp_angleCCW65536);
	//
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256 						= (src->cx256);							/* �e��x256 */
			obj_send1->cy256 						= (src->cy256);							/* �e��y256 */
			//
			br.BULLET_REGIST_speed256				= (t256(1.0));	/* �e�� */
			br.BULLET_REGIST_div_angle65536 		= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x  */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_MARU12_01_AKA; 				/* [�g�t���Ԋےe] */
			br.BULLET_REGIST_n_way					= (6);									/* [6way] */
	//
			br.BULLET_REGIST_speed_offset			= t256(1);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* ���݂��ꂵ���Ȃ����v�� */
		{
			br.BULLET_REGIST_angle65536 			= (src->tmp_angleCCW65536); 							/* �p�x */
			bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		//
			br.BULLET_REGIST_angle65536 			= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* �p�x */
			bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		}
	}
}

