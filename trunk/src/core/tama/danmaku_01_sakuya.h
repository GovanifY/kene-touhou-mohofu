
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�g5�ʒ��{�X ��� ���U���ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���̒e���̖{���́A
	�P�D��r�I����(�Ƃ����������x)�̐i�C�t�e�̕ǂ�������Ȃ���A
	�ُ�ɒx�����������Ȃ̂Œ������܂��Ĕ�����ԃN�i�C�e������鎖�B
	�Q�D�i�C�t�e�ƐԃN�i�C�e�̑��x���Ⴄ�̂ŁA�d�Ȃ��Ĕ�����ꍇ���������鎖�B
	�R�D�������Ȃ���A�d�Ȃ�͂�����x�̎��Ԃ��ώ@���Ă���Δ�������B
	�S�D(�R�D)�ɖ������āA�ώ@���Ԃ��Z��(���������ꍇ���K����������)�Ɖ^�������Ɣ������Ȃ��B
	-------------------------------------------------------
	�� �i�C�t
		�i�C�t�͋Ȃ������ǂɂȂ�Ȃ��Ɩ��͖͂����B
		��r�I�������x�����A���߂���Ɩ��͖͂����B�����ɂ��炢�炷��x�����K�v�B
	�� �ԃN�i�C
		�ԃN�i�C�ɂ͂��炢�炷��x�����K�v�B
		��ʉ��Ŕ����Ă����ꍇ�ɁA�u��ʉ�������ł̂݋}���ɑ��x���ω�����v�B
		�������������x�Œ�������K�v����B
		�O�L�Q�|�S�͎�ɉ�ʉ�������Ŕ������錻�ہB
		��ʒ�������ł́A���̕��G�ɋ߂��̂Ŕ��ː_�o�͗v�邪�A�O�L�Q�|�S�̌��ۂ͔������ɂ����B
	-------------------------------------------------------
	�� �ԃN�i�C
		�Ȃ񂩒m��񂪌����蒼������u�����e�v�������B
		�u�����e�v�{�u�����e�v���Ď����Ȃ��H
	-------------------------------------------------------
	����ƈႢ�A����������ɍs���Ȃ�����A������͏o���Ȃ��B
	(psp�̏c�𑜓x�ł͏�ɍs���X�y�[�X���Ȃ�)
	��������psp�̏c�𑜓x���S�R����Ȃ�����A�{�ƕ��̃`���������͖����B
	������x���E�ɓ�������Ĕ����ĖႤ����z�肵�Ă���B(������e���x��)
	����ɕ����Ė����e�͂����Ă��񂺂�Ⴄ�B(�ʕϐ��Ƃ�̂��ʓ|��������������)
	-------------------------------------------------------
	�i�C�t�̒e���͂��ꂭ�炢�����H�ԃN�i�C�̒e���͑��߂���B
	�ԃN�i�C�e�̎����͌���͂����Ԃ���Ă���̂ŕς������񂾂��ǁA
	�u�X�y�J�V�X�e����d���v���Ȃ��Ə�肭�����Ȃ������H�B
	-------------------------------------------------------
	�u�X�y�J�V�X�e����d���v�F2��ނ̈قȂ����X�y�J�������Ɍ��Ă�X�y�J�V�X�e���B
	�����������̂ɂ���ׂ������Ȃ��ׂ����������B(�p�`�F)
	-------------------------------------------------------
	�܂��S(����)�Ȃ̂ŁA(�v���C���[�ɂ���ȏ���)����ł�����Ă͍���B
	-------------------------------------------------------
	�i�C�tspeed256	speed_offset	/	�ԃN�i�Cspeed256	speed_offset
	t256(2.5)			(0) 			/	(0) 				(4) 			// �ǂ��񂾂��ǖ{��������B
	t256(2.0)			(0) 			/	(0) 				(2) 			// �ԃN�i�C�̏����x�����邩�Ȃ��B// �i�C�t�ƐԃN�i�C�����ꂷ���B�{��������B
	t256(1.75)			(0) 			/	(0) 				(1)
---------------------------------------------------------*/



//#define ADJUST3916	((65536/20)+(640))
#define ADJUST3916	(4000)
static void danmaku_create_01_sakuya_misogi(SPRITE *src)
{
//	DANMAKU_0256_TIME	DANMAKU_0128_TIME
	static int aaa_angle65536;
	static int tama_aaa_interval;		// �i�C�t
//	static int tama_bbb_interval;		// �ԃN�i�C
	if (127 == (src->boss_base_danmaku_time_out))/* �����l�Ƀ��Z�b�g�B */
	{
		#if 1
		/* �i�C�t: �E�㑤���猂���n�߂�B�E���Ȃ̂�90�x���炵�āAbullet_regist_angle()�̕␳���s���B */
	//	src->tmp_angleCCW65536	= (65536+(65536/4)-((65536*8)/20))+(ADJUST3916);				// �i�C�t
	//	src->tmp_angleCCW65536	= (((65536*2)/20))+(ADJUST3916);				// �i�C�t 	bullet_regist_angle();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		src->tmp_angleCCW65536	= (((65536*1)/18))+(ADJUST3916);				// �i�C�t 	bullet_regist_angle();�ŉ�����ɁA���v����(2/20)���猂���n�߂�B
		// �{���͑ł��n�߂̐i�C�t�̒[���E��60�x���炢����ł��n�߁A
		// �ł��I���́A����60�x���炢�̈ʒu�ɂȂ�B
		#endif
		#if 1
		/* �ԃN�i�C: */
	//	aaa_angle65536			= (65536/2);		// �ԃN�i�C
		aaa_angle65536			= (65536/4)-(0);		// �ԃN�i�C (�E������)
		#endif
		tama_aaa_interval = 0;	// �i�C�t
//		tama_bbb_interval = 0;	// �ԃN�i�C
	}
//	if (DANMAKU_0096_TIME > (src->boss_base_danmaku_time_out))/* �b���������Ȃ��B */
	if (100 > (src->boss_base_danmaku_time_out))/* �b���������Ȃ��B */
	{
		tama_aaa_interval--;
		if (0 >= tama_aaa_interval)
	//	if (0==((src->boss_base_danmaku_time_out)&0x07))
		{
			tama_aaa_interval = (12);
			/*	�i�C�t */
			#if (1)
		//	voice_play(VOICE13_SAKUYA_SHOT02, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE13_SAKUYA_SHOT02);
			#endif
		//	src->tmp_angleCCW1024 -= (int)(256/5)+(10); /*CCW*/
		//	src->tmp_angleCCW1024 -= (int)(1024/20)+(10); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640); /*CCW*/
		//	src->tmp_angleCCW65536 -= (int)(65536/20)+(640)+(128); /*CCW*/	/* 3276== (65536/20) */
			src->tmp_angleCCW65536 -= (ADJUST3916); /*CCW*/
			mask65536(src->tmp_angleCCW65536);
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
			obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
		//	br.BULLET_REGIST_speed256			= (t256(2.5));						/* �e�� (t256(2.5)�Q�l�B�g�m�[�}��5��) */
		//	br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
//			br.BULLET_REGIST_speed256			= (t256(1.75)); 					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			br.BULLET_REGIST_speed256			= (t256(1.00)); 					/* �e�� psp�͏c�𑜓x���Ȃ��̂łƂɂ����x�����Ȃ��ƁB */
			br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536));		/* �e���p�x65536 */ 		/* [������e] */
		//	br.BULLET_REGIST_div_angle1024		= (int)(256/5);
		//	br.BULLET_REGIST_div_angle1024		= (int)(1024/20);
		//	br.BULLET_REGIST_div_angle65536 	= (int)(65536/20);					/* �����p�x([360/360]�x��20����) */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/18);					/* �����p�x([360/360]�x��20����)[�ł��I��肪���΂ߏ�ɂȂ�] */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (8);								/* [8way] */
		//	br.BULLET_REGIST_speed_offset		= t256(0);/*�Ă���*/
		//	br.BULLET_REGIST_speed_offset		= t256(1);/*�������Ă݂�Ă���*/
			br.BULLET_REGIST_speed_offset		= t256(6);/*�������Ă݂�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
			bullet_regist_angle();

		//	if (0==((src->boss_base_danmaku_time_out)&0x1f))/* reset (1/32) */
		//	{
		//	//	aaa_angle1024	= (src->tmp_angleCCW1024)-(int)((1024/2));
		//		aaa_angle65536	= (src->tmp_angleCCW65536)-(int)((65536/2));
		//	}
		}
	}
//	/* �ԃN�i�C�̕����x���̂ŁA�i�C�t����Ɍ����n�߂鎖�ɂ���B */
//	if (DANMAKU_0032_TIME > (src->boss_base_danmaku_time_out))/* �b���������Ȃ��B */
	if (41 > (src->boss_base_danmaku_time_out))/* �b���������Ȃ��B */
//	if ((128-40) < (src->boss_base_danmaku_time_out))/* �Ō�͎b���������Ȃ��B */
	{
	//	else/* �������̒e�͖{���͂���ďo�銴���ɂ������B */
//	//	if ((10-1)>((((u8)src->boss_base_danmaku_time_out)^0x20)&0x2f))/* x 10set */
//		if ((10-1)>(((src->boss_base_danmaku_time_out)	  )&0x0f))/* x 10set */
		/* 4��Ɉ��A�v10set */
		if ((0)==(((src->boss_base_danmaku_time_out)	  )&0x03))/* x 10set */
		{
			/* �������̐ԃN�i�C�͑S�R�_���D */
		//	aaa_angle1024 -= (int)(256/5);
		//	aaa_angle1024 -= (int)(1024/20);
			aaa_angle65536 -= (int)(65536/20);
		//	if (0 > (int)aaa_angle65536)	{	aaa_angle65536 = (65536/2); 	}
		//	mask65536(aaa_angle65536);
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
			obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
		//	br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� ���߂��� */
		//	br.BULLET_REGIST_speed256			= (t256(1.0));						/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_speed256			= (t256(0.5));						/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_speed256			= (t256(0.25)); 					/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_angle65536 		= (((aaa_angle65536)+(65536*3/4))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
			br.BULLET_REGIST_angle65536 		= (((aaa_angle65536))&(65536-1));	/* �e���p�x65536 */ 	/*��CCW*/
			#if 1
		//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+difficulty+1+1/*3*/)<<(6)); 	/* [�����e]�̕����p�x */
		//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+1/*3*/)<<(6+1));		/* [�����e]�̕����p�x(r31) */
		//	br.BULLET_REGIST_div_angle65536 	= (int)((difficulty+1/*3*/)<<(6+1+1));		/* [�����e]�̕����p�x(r32) */
		//	static const u16 bbb_bbb[4] = {(u16)(128), (u16)(512-64), (u16)(512), (u16)(512+64)};
			br.BULLET_REGIST_div_angle65536 	= (int)(512-64);		/* [�����e]�̕����p�x(r32) */
			#endif
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
			br.BULLET_REGIST_n_way				= (4);								/* [4way] */
//-------------------------
	// ��  [�����e]
		//	br.BULLET_REGIST_speed256			= (0);								/* �e�� (�o�n�߂͒x��) */
//			br.BULLET_REGIST_speed256			= (t256(0.25)); 							/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_speed256			= (t256(3.25)); 							/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_speed256			= (t256(3.00)); 							/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_speed256			= (t256(2.00)); 							/* �e�� (�o�n�߂͒x��) */

		//	br.BULLET_REGIST_speed_offset		= t256(3/*0*/);/* �������Ă݂� �Ă���*/
		//	br.BULLET_REGIST_speed_offset		= t256(8);/* �������H(�����e�Ȏ��͊ԈႢ�Ȃ�) */
		//	br.BULLET_REGIST_speed_offset		= t256(4);/* �������H(�����e�Ȏ��͊ԈႢ�Ȃ�) */
//			br.BULLET_REGIST_speed_offset		= t256(2);/* �������H(�����e�Ȏ��͊ԈႢ�Ȃ�) */
		//	br.BULLET_REGIST_speed_offset		= t256(1);/* �������H(�����e�Ȏ��͊ԈႢ�Ȃ�) */
			br.BULLET_REGIST_speed_offset		= t256(-1);/* �����H(�����e�Ȏ��͊ԈႢ�Ȃ�) */
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
			bullet_regist_angle();
//-------------------------
	// ��  [�����e]
		//	br.BULLET_REGIST_speed256			= (t256(0.25)); 							/* �e�� (�o�n�߂͒x��) */
		//	br.BULLET_REGIST_speed256			= (t256(1.25)); 							/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_speed256			= (t256(2.25)); 							/* �e�� (�o�n�߂͒x��) */
			br.BULLET_REGIST_speed_offset		= t256(-1);/* �����H(�����e�Ȏ��͊ԈႢ�Ȃ�) */
			bullet_regist_angle();
		}
	}
}

/*---------------------------------------------------------
	�g5�ʒ��{�X ���u��p�F�~�X�f�B���N�V�����v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
---------------------------------------------------------*/
static void danmaku_create_22_sakuya_miss_direction(SPRITE *src)
{
	if (256-64< src->boss_base_danmaku_time_out)
	{;}
	else
	if (256-64-48< src->boss_base_danmaku_time_out)
	{
		/* �ԃN�i�C�e */
		if (0==((src->boss_base_danmaku_time_out)&0x03))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			SPRITE *zzz_player;
			zzz_player = &obj00[FIX_OBJ_00_PLAYER];
			tmp_angleCCW65536_src_nerai(zzz_player, src);/*���@�_���p�쐬*/
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
			obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
		//	br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
			br.BULLET_REGIST_speed256			= (t256(2.0)-((src->boss_base_danmaku_time_out)));	/* �e�� */
		//	#define TORIAEZU_000 (0)/* ��̒����l */
			#define TORIAEZU_000 (int)(6*64)/* ��̒����l */
			br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536)) + TORIAEZU_000;		/* �e���p�x1024 */	/* ���@�_���e */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);					/* �����p�x */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
			br.BULLET_REGIST_n_way				= (24); 							/* [24way] */
			br.BULLET_REGIST_speed_offset		= t256(1);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
			bullet_regist_angle();
		}
	}
	else
//	if (256-192< src->boss_base_danmaku_time_out)
	if (48< src->boss_base_danmaku_time_out)
	{;}
	else
//	if (256-192< src->boss_base_danmaku_time_out)
	{
		/* �i�C�t */
		if (0==((src->boss_base_danmaku_time_out)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			#endif
			SPRITE *zzz_player;
			zzz_player = &obj00[FIX_OBJ_00_PLAYER];
			tmp_angleCCW65536_src_nerai(zzz_player, src);/*���@�˂炢�p�쐬*/
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
			obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
		//	#define TORIAEZU_111 (0)/* ��̒����l */
			#define TORIAEZU_111 (int)(4*64)/* ��̒����l */
			br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4)) + TORIAEZU_111;		/* �e���p�x1024 */
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/(7*4));				/* �����p�x([90/360]�x��7����==28����) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (11); 							/* [11way] */
			br.BULLET_REGIST_speed_offset		= t256(0);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
	// �e1
			br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� */
			bullet_regist_angle();
	// �e2
			br.BULLET_REGIST_speed256			= (t256(1.5));						/* �e�� */
			bullet_regist_angle();
		}
	}
}
/*---------------------------------------------------------
	�g5�ʒ��{�X ���u��p�F�~�X�f�B���N�V����(1/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A�S����24�����e
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
---------------------------------------------------------*/
//DANMAKU_0048_TIME
static void danmaku_create_02_24nerai(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/*���@�_���p�쐬*/
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
		obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
		obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
	//	br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
		br.BULLET_REGIST_speed256			= (t256(2.0)-((src->boss_base_danmaku_time_out)));	/* �e�� */
	//	#define TORIAEZU_000 (0)/* ��̒����l */
		#define TORIAEZU_000 (int)(6*64)/* ��̒����l */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536)) + TORIAEZU_000;		/* �e���p�x1024 */	/* ���@�_���e */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);					/* �����p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KUNAI12_01_AKA;			/* [�ԃN�i�C�e] */
		br.BULLET_REGIST_n_way				= (24); 							/* [24way] */
		br.BULLET_REGIST_speed_offset		= t256(1);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	�g5�ʒ��{�X ��� �u��p�F�~�X�f�B���N�V����(2/2)�v�ɂ�����Ƃ��������e��
	-------------------------------------------------------
	���@�_���A11�e(���S�ƁA���E��5�e���_��)
	-------------------------------------------------------
	[���F����bu llet_create_n_way_dan_sa_type()�̎��@�_���̓��U�g�_��Ȃ���e�Ȃ̂ŁA�����Ȃ���Γ�����܂��񂪁A
	���̂��������͋����e�ɂ���\��Ȃ̂ŁA�����Ȃ���Γ�����悤�ɂȂ�\��ł�]
	-------------------------------------------------------
	������Ȃ�������A�����͂R�񌂂B
---------------------------------------------------------*/
//DANMAKU_0064_TIME
static void danmaku_create_03_11nife(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x07))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		tmp_angleCCW65536_src_nerai(zzz_player, src);/*���@�˂炢�p�쐬*/
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
		obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
		obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
		br.BULLET_REGIST_speed256			= (t256(2.0));						/* �e�� */
	//	#define TORIAEZU_111 (0)/* ��̒����l */
		#define TORIAEZU_111 (int)(4*64)/* ��̒����l */
		br.BULLET_REGIST_angle65536 		= ((src->tmp_angleCCW65536))-(int)(65536*6/(7*4)) + TORIAEZU_111;		/* �e���p�x1024 */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(7*4));				/* �����p�x([90/360]�x��7����==28����) */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
		br.BULLET_REGIST_n_way				= (11); 							/* [11way] */
		br.BULLET_REGIST_speed_offset		= t256(0);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(1/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	24way =(�E12way)+(��12way)
	-------------------------------------------------------

---------------------------------------------------------*/
static void danmaku_create_04_pink_hearts(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x0f))/* �{���� 8 �e(128==0x80==danmaku_time_out) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		obj_send1->cy256						= (src->cy256); 					/* �e��y256 */	/* �{���͔��e�ʒu�� offset �p */
		obj_send1->tmp_angleCCW65536 = (0);/* ������ */
	//
		//	br.BULLET_REGIST_div_angle1024		= (0);								/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_06_YUKARI; 		/* [�s���N�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (1);								/* [1way] */
			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
		//
		int jj;
		jj=0;
		int i;
		for (i=0; i<(13*12)/*144*/; i+= (13) )/*12����*/							/* �p�x(128[(90+a)/360�x]�� 10 ����) */
		{
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= src->cx256+t256(4.0); 			/* �{���͔��e�ʒu�� x offset ���� */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1/*+1*/));				/* �e�� */
			br.BULLET_REGIST_angle65536 		= (jj/*(0+i+i)&(1024-1)*/);
			bullet_regist_angle();
			//
			obj_send1->cx256					= src->cx256-t256(4.0); 			/* �{���͔��e�ʒu�� x offset ���� */
			br.BULLET_REGIST_speed256			= ((160-i)<<(1+1/*+1*/));				/* �e�� */
			br.BULLET_REGIST_angle65536 		= ((0-jj)&(65536-1));
			bullet_regist_angle();
			//
			jj += (13*2*64);
		}
	}
}


/*---------------------------------------------------------
	�g5�ʃ{�X ��� �u�ʏ�U��1(2/2)�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	32�����ɕ����A�����͒ᑬ�e�A��͍����e�B
	16������2��o�^����B
	-------------------------------------------------------
	32����	�p�x(1024[360/360�x]�� 32 ����)1024 == 32 x 32
---------------------------------------------------------*/
static void danmaku_create_05_32way_dual(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		//
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (src->cx256); 					/* �e��x256 */
			obj_send1->cy256					= (src->cy256); 					/* �e��y256 */
		//
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (16); 							/* [16way] */
			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*�Ă���*/
			br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16));		/* �����p�x(65536[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
//			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		//
			br.BULLET_REGIST_speed256			= (t256(0.75)); 		/* �����͒ᑬ�e */
			br.BULLET_REGIST_angle65536 		= (0);					/* �e���p�x65536 */
			bullet_regist_angle();
		//
			br.BULLET_REGIST_speed256			= (t256(1.25)); 		/* ��͍����e */
			br.BULLET_REGIST_angle65536 		= (65536/32);			/* �e���p�x65536 */
			bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	���݁u�N���b�N�R�[�v�X(1/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	���݁u�N���b�N�R�[�v�X(2/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	���ہu���i�N���b�N(1/2)�v
	-------------------------------------------------------
	32way �Œ�A�e	x 4��
---------------------------------------------------------*/
static void danmaku_create_06_luna_clock_32way(SPRITE *src)
{
	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4�� */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
		br.BULLET_REGIST_speed256			= (t256(1.0));				/* �e�� */
		br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*�Ă���*/
		br.BULLET_REGIST_angle65536 		= (0);						/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(32));		/* �����p�x(1024[360/360�x]�� 32 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_bullet_obj_type	= BULLET_CAP16_05_TUTU_SIROI;	/* [������e] */	/* �e�O�� */
		br.BULLET_REGIST_n_way				= (32); 					/* [32way] */		/* ���e�� */
//		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
		bullet_regist_angle();
	}
}


/*---------------------------------------------------------
	���ہu���i�N���b�N(2/2)�v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�u�ʏ�U��3(1/2)�v
	-------------------------------------------------------
	�g�U�^�S���ʃi�C�t�e
	80�����e == (8way[hi to 0]+8way[slow to 0]) x 5way(5����)
	-------------------------------------------------------
	�{���͉�ʏ�Ɖ�ʍ��Ɖ�ʉE�̒[�Ńi�C�t����x�������˂���B
---------------------------------------------------------*/
static void danmaku_create_07_80way_dual_five(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x3f))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 */
	//		br.BULLET_REGIST_div_angle1024		= (0);									/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;				/* [�i�C�t�e] */
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
			br.BULLET_REGIST_speed_offset		= t256(1/*0*/);/*�Ă���*/
//			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	//
		int ii; 	/* 1����65536���������p�x */
		int j;		/* �J�E���^ */
		j=0;
		for (ii=0; ii<(0x10000); ii += 819 )/* 819.2(65536/80)	12.8==(1024/80) 80���� [5����] */	/* 204.8==(1024/5) �p�x(1024[72/360�x]�� 5 ����) */
		{
			j++;
			j &= (16-1);/* 16way(8way[hi to 0]+8way[slow to 0]) */
			br.BULLET_REGIST_speed256			= (t256(3.0)-((j)<<5)-((j&1)<<7));		/*��͍����e*/
			br.BULLET_REGIST_angle65536 		= (((0-ii))&(65536-1)); 			/* �p�x */
			bullet_regist_angle();
		}
	}
}


/*---------------------------------------------------------
	���C�h��Z�u����h�[���v
---------------------------------------------------------*/
#if 0
#endif


/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e����������)
	-------------------------------------------------------
	�����e(���m�ɂ͓������e)�Ȃ̂ɁA�����e�Ɍ����Ȃ���
	�����e(���m�ɂ͓����x�e�A����==0)�Ɍ������Ⴄ��肪����B

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_sakuya_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
//	int jj = 0;
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if ((BULLET_OODAMA32_02_KIIRO & 0xfff8) == (s->type & 0xfff8) ) 	/* �g��12�ےe�Ȃ� */
		{
//			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
//			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tra65536 					= t256(6);			/* ���������e */
		//	s->tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) ���������e */
//			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			s->tra65536 					= t256(3);			/* (3 ... 6) ���������e */
			/* (1�͒x�������̂ŏ��O) 9 ���Ƒ��߂��邩�� */
		}
		if ((BULLET_KNIFE20_04_AOI & 0xfff8) == (s->type & 0xfff8) )	/* �i�C�t�e�Ȃ� */
		{
//			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			s->tra65536 					= t256(2);			/* (3 ... 6) ���������e */
		}
	}
}
#endif

/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e��~�B���݂̍��W��V��_���W�ɕύX����B������ς���)
	-------------------------------------------------------

---------------------------------------------------------*/
#if 1
static void exchange_damnaku_sakuya_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
//	int add_angle = 0;
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if ((BULLET_OODAMA32_01_AKA & 0xfff8) == (s->type & 0xfff8) )	/* [�ԑ�e]�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* �e�̌��ݍ��W���A�V��_���W�Ƃ���B */
			s->tx256 = (s->cx256);		/* ��_���W�ɃZ�b�g */
			s->ty256 = (s->cy256);		/* ��_���W�ɃZ�b�g */
			s->radius256 = t256(0); 	/* ��_���W�ɂȂ����̂Ŕ��a�� 0 �ɂ���B */
			/*	*/
			s->tra65536 					= t256(0);		/* ���������e */
			s->speed65536					= t256(0);		/* �e�� */
			#if 0
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* ���� */	/* �����͗����łȂ��C������ */
			#else
			/* ���Ȃ��Ƃ��������͖{���ɋ߂��B ���A�S�R�Ⴄ�B */
			/* ���Z�Œ�l(KOTEITI_7777)�ȊO�̗v���Ƃ��āA�e�����X�g�̃T�C�Y�Ń��[�v����ׁA�e�����X�g�̃T�C�Y�� */
			/* ���݂� 1024 ����A�{���Ɠ��� 640 �Ɍ��点�΁A����\���͂���B */
//			add_angle += (KOTEITI_7777);
//			s->rotationCCW1024				= (add_angle>>6);			/* ���� */
			#endif
			s->type 						= (BULLET_OODAMA32_02_KIIRO);
		}
	}
}
#endif
#if 1
static SPRITE obj_work;

static void exchange_damnaku_sakuya_tuika(void)
{
	voice_play(VOICE16_BOSS_KYUPIN, TRACK04_TEKIDAN);
//
	br.BULLET_REGIST_speed256			= (t256(0));			/* �e�� */
	br.BULLET_REGIST_div_angle65536 	= (int)(65536/(48));		/* �����p�x(1024[360/360�x]�� 48 ����) */	/* 1����n���������p�x */
	br.BULLET_REGIST_bullet_obj_type	= BULLET_KNIFE20_04_AOI;			/* [�i�C�t�e] */
	br.BULLET_REGIST_n_way				= (1);/* [nway] */
	br.BULLET_REGIST_speed_offset		= t256(0);/*�Ă���*/
//
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
	obj_work.cx256					= (obj_send1->cx256);				/* �e��x256 */
	obj_work.cy256					= (obj_send1->cy256);				/* �e��y256 */
//	/*const*/ SPRITE *obj_work_ptr = &obj_work;

//	int check_type;
//	check_type = br.BULLET_REGIST_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<SPRITE_444POOL_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj44[ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if ((BULLET_OODAMA32_02_KIIRO & 0xfff8) == (s->type & 0xfff8) ) 	/* [����e]�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* �e�̌��ݍ��W�Ƀi�C�t�e��o�^����B */
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256					= (s->cx256);				/* �e��x256 */
			obj_send1->cy256					= (s->cy256);				/* �e��y256 */
			//
		//	tmp_angleCCW65536_src_nerai(obj_player, obj_send1);/*���@�˂炢�p�쐬*/
			tmp_angleCCW65536_src_nerai(&obj_work, s);/*���@�˂炢�p�쐬*/
			//
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
			br.BULLET_REGIST_angle65536 		= s->tmp_angleCCW65536;/*���@�˂炢�p*/
			bullet_regist_angle();
		}
	}
}
#endif

/*
	���H�u�W���b�N�E�U�E���h�r���v�̌����B
	���̃X�y�J�͎n�߂�[�ԑ�e]�������_�����ۂ��΂�T���B
	���̂΂�T���́A���Ď��̂Ȃ������̓����΂�T���Ȃ񂾂��ǁB
	�ǂ������d�g�݂Ȃ̂������ς�킩���̂ŁA�������Ă݂�B
	�܂��A�΂�T�������œK���ɃX�N�V���B�e�B
	��ʂ�2/3�ʂ����e������Ȃ��B
	�����Ń`�}�`�}���𐔂����20�e�`26�e���炢�B
	��ʔ�������16�`18�e���炢�Ȃ̂ŁA�����S����32�e�`36�e�Ȃ̂ł́H�Ɛ����B
	32�e�A36�e�B�ǂ��������肻���Ȃ̂ŉ��x�������Ă݂�B�悭����Ȃ���32�e���ۂ��B
	���Q�[���t�B�[���h��384x448, ��邳��̍��W��291x49,
	�������̍��W��ڎ�(�摜���S���W)�{���G�����\�t�g�Ōv�����鎖�ɂ����B
	��e�Ȃ̂ŁA���G�����\�t�g�œ��ډ~�ɂȂ�悤�Ɏl�p��`���B���̂��ƑΊp���ɐ���`���Ē��S���W������o���B
	----------------
	�ԍ�,�摜���S���W,	���΍��W
	 1�e�ڂ� 94x 87,	-(291- 94)x 87-49,	==(-197,+38)
	 2�e�ڂ�171x 69,	-(291-171)x 69-49,	==(-120,+20)
	 3�e�ڂ�192x 65,	-(291-192)x 65-49,	==(- 99,+16)
	 4�e�ڂ�151x119,	-(291-151)x119-49,	==(-140,+70)
	 5�e�ڂ�181x116,	-(291-181)x116-49,	==(-110,+67)
	 6�e�ڂ�221x 80,	-(291-221)x 80-49,	==(- 70,+31)
	 7�e�ڂ�175x139,	-(291-175)x139-49,	==(-116,+90)
	 8�e�ڂ�159x153,	-(291-159)x153-49,	==(-132,+104)
	 9�e�ڂ�210x152,	-(291-210)x152-49,	==(- 81,+103)
	10�e�ڂ�258x119,	-(291-258)x119-49,	==(- 33,+70)
	11�e�ڂ�255x152,	-(291-255)x152-49,	==(- 36,+103)
	12�e�ڂ�215x178,	-(291-215)x178-49,	==(- 76,+129)
	13�e�ڂ�254x170,	-(291-254)x170-49,	==(- 37,+121)
	14�e�ڂ�206x236,	-(291-206)x236-49,	==(- 85,+187)
	15�e�ڂ�233x273,	-(291-233)x273-49,	==(- 58,+224)
	16�e�ڂ�280x255,	-(291-280)x255-49,	==(- 11,+206)
	------------------���̕ӂ��������ۂ��B
	�����܂ő���B
	���΍��W����p�x������o���Ă݂�B�悭�킩��Ȃ����ǁA
	tan(197/38)�Ŋp�x���o��̂��ȁH
//							(x/y)									tan(x/y)	���Ԃ�p�x
	 1�e�ڂ�(-197,+38)		5.18421052631578947368421052631579		0.0907292755019070511126536298905576
	 2�e�ڂ�(-120,+20)		6										0.105104235265676462511502380139882
	 3�e�ڂ�(- 99,+16)		6.1875									0.108414028737611378659694904675177
	 4�e�ڂ�(-140,+70)		2										0.0349207694917477305004026257737253
	 5�e�ڂ�(-110,+67)		1.64179104477611940298507462686567		0.0286625046177796138852122880504812
	 6�e�ڂ�(- 70,+31)		2.25806451612903225806451612903226		0.0394310774284007674631847355929523
	 7�e�ڂ�(-116,+90)		1.28888888888888888888888888888889		0.0224991500955683949518891752039385
	 8�e�ڂ�(-132,+104) 	1.26923076923076923076923076923077		0.0221558801384605992607822428428949
	 9�e�ڂ�(- 81,+103) 	0.786407766990291262135922330097087 	0.0137262667569302851091532667488299
	10�e�ڂ�(- 33,+70)		0.471428571428571428571428571428571 	0.00822816644161695626063183252739627
	11�e�ڂ�(- 36,+103) 	0.349514563106796116504854368932039 	0.00610025557803738263689086699115253
	12�e�ڂ�(- 76,+129) 	0.589147286821705426356589147286822 	0.010282922344805178445884641481058
	13�e�ڂ�(- 37,+121) 	0.305785123966942148760330578512397 	0.00533700788847012290939546575203758
	14�e�ڂ�(- 85,+187) 	0.454545454545454545454545454545455 	0.00793348122027169612990511645481441
	15�e�ڂ�(- 58,+224) 	0.258928571428571428571428571428571 	0.00451918686372985895525414665154542
	16�e�ڂ�(- 11,+206) 	0.0533980582524271844660194174757282	0.000931972200504819213720633933431873
//
	Windows�̓d����g���Ă݂�B
	Windows�̓d���tan(1)��0.0174550649282175857651288952197278�������ȁB
	tan(1)��(45�x)�̔�������A���W�A��(�P����2�΂̒P�ʌn)�Ƃ���
	(2*pai)/360==tan(1)�H�H�H�B�悭�킩���ȁB
	�ǂ�����tan�Ȃ̂��悭�킩��Ȃ��B
	tan(99)==-6.31375151467504309897946424476819()
	tan(90)==�G���[�����Ȓl�ł��B
	tan(89)==57.2899616307594246872781475371126
	tan(88)==28.6362532829156035507565093209464
	tan(55)==1.4281480067421145021606184849985
//
�킩�����Barc_tan()����Ȃ��Ƒʖڂ��B	Windows�̓d��́Atan(360�x�̊p�x)����B
	----------------
�Ƃɂ����~��ɍL����Ȃ��̂ŁABULLET_REGIST_speed_offset�ɍ������Ȃ肠��̂����H
*/
/*---------------------------------------------------------
	���H�u�W���b�N�E�U�E���h�r���v�ɂȂ邩������Ȃ��e�X�g
---------------------------------------------------------*/
static void danmaku_create_14_sakuya_jack_oodama32(SPRITE *src)
{
	if (96==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256						= (src->cx256); 	/* �e��x256 */
			obj_send1->cy256						= (src->cy256); 	/* �e��y256 */
	//		br.BULLET_REGIST_div_angle65536 		= (0);									/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_OODAMA32_01_AKA;				/* [�ԑ�e] */
			br.BULLET_REGIST_n_way					= (1);									/* [1way] */
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* ���݂��ꂵ���Ȃ����v�� */
	//
		int ii; 	/* ii */
	//	int jj; 	/* jj */
		int kk; 	/* �J�E���^ */
	//	int mm; 	/* �J�E���^ */
		kk=0;
	//	mm=0;
	//	jj=(1024-960);
		const u32 ra_nd32 = ra_nd();
		for (ii=0; ii<(32); ii ++)/* 32�e�΂�T�� */
		{
			/* 960==30720/32,  30720 == 32768 -1024 -1024 */
			/* 936 = 936.228571428571428571428571428571 == (32768/(32+2���E�̗]�T��))	*/
			//	jj += (960);/* (=936)< 1024 [180�x(32768)���኱�����͈͂ɂ΂�T��] */
			//	kk += 386;
				kk += ra_nd32;
			//	kk += 42857142;
			//	mm += 42857142;
			//	br.BULLET_REGIST_speed_offset			= t256(0);/*�Ă���*/
			//	br.BULLET_REGIST_speed_offset			= t256(8);/*�Ă���*/
			//	br.BULLET_REGIST_speed_offset			= t256(12);/*�Ă���*/
				br.BULLET_REGIST_speed_offset			= (t256(4)+((kk&0x07)<<8));/*�Ă���*/
			//	br.BULLET_REGIST_speed256			= ((kk)&(512-1))+t256(0.25);	/*��͍����e*/
				br.BULLET_REGIST_speed256			= ((kk)&(1024-1))+t256(0.25);	/*��͍����e*/
				br.BULLET_REGIST_angle65536 		= ((kk)&(32768-1))+(int)(65536*3/4);			/* �p�x */
			//	br.BULLET_REGIST_angle65536 		= ((mm)&(1024-1))+jj+(int)(65536*3/4);			/* �p�x */
				bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		}
	}
	else
	if (64==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
//		// 48 �t���[���o�ߌ�A�S�ăX�g�b�v�����A�������A�J�E���g���Z�b�g
		exchange_damnaku_sakuya_tomeru();/*�S�ăX�g�b�v*/
	}
	else
	if (56==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		obj_send1->cx256					= t256(0);					/* �e��x256 */
		obj_send1->cy256					= t256(256);				/* �e��y256 */
		exchange_damnaku_sakuya_tuika();/*�i�C�t�ǉ�*/
	}
	else
	if (48==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		obj_send1->cx256					= t256(GAME_WIDTH); 		/* �e��x256 */
		obj_send1->cy256					= t256(256);				/* �e��y256 */
		exchange_damnaku_sakuya_tuika();/*�i�C�t�ǉ�*/
	}
	else
	if (40==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		obj_send1->cx256					= zzz_player->cx256;			/* �e��x256 */
		obj_send1->cy256					= zzz_player->cy256;			/* �e��y256 */
		exchange_damnaku_sakuya_tuika();/*�i�C�t�ǉ�*/
	}
	else
	if (32==(src->boss_base_danmaku_time_out))	// 192== (64*3)
	{
		exchange_damnaku_sakuya_saikasoku();/*�ĉ���*/
	}

}
