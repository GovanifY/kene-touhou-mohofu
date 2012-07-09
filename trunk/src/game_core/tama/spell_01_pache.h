
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�`�����[�E�m�[���b�W�̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�Ԓe
---------------------------------------------------------*/
local OBJ_CALL_FUNC(test_pache_16_shot_AKA)
{
//	REG_02_DEST_X	= ((src->center.x256));
//	REG_03_DEST_Y	= ((src->center.y256));
	calculate_jikinerai();
	//
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_01_AKA);		/* [�Ԋےe] */
	HATSUDAN_06_n_way					= (16); 										/* [16way] */	/* ���e�� */
	HATSUDAN_07_div_angle65536			= (int)(65536/(16));	/* �����p�x(1024[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
	//
	unsigned int jj;
	for (jj=0; jj<(4); jj++)/* 4�e */
	{
		HATSUDAN_02_speed_offset		= (3+jj)<<8;/*(�e�X�g)*/
		HATSUDAN_01_speed256			= (t256(1.00)+(jj<<7)); 		/* �e�� */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}

/*---------------------------------------------------------
	�e
---------------------------------------------------------*/
local OBJ_CALL_FUNC(test_pache_16_shot_AOI)
{
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);		/* [�ےe] */
//	HATSUDAN_06_n_way					= (16); 										/* [16way] */	/* ���e�� */
//	HATSUDAN_07_div_angle65536			= (int)(65536/(16));	/* �����p�x(1024[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
	HATSUDAN_06_n_way					= (8);											/* [8way] */	/* ���e�� */
	HATSUDAN_07_div_angle65536			= (int)(65536/(8)); /* �����p�x(1024[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
	//
	unsigned int ii;
	for (ii=0; ii<(65536); ii+=(65536/4))/* 45 */
	{
		HATSUDAN_03_angle65536 = (lz[0].laser_angle65536+ii);
		//
		#if 0/*[�p�~�\��](-r39)*/
		REG_0b_REG3 = ((HATSUDAN_03_angle65536));
		// �x�N�g���e�o�^�B regist vector.
		sincos65536_NUM(REG_NUM_0b_REG3, REG_NUM_0c_REG4);
		REG_02_DEST_X	 = (((REG_0c_REG4)*(REG_09_REG1))>>8);
		REG_03_DEST_Y	 = (((REG_0d_REG5)*(REG_09_REG1))>>8);
		#else/*(r40-)*/
		//------------------
		HATSUDAN_01_speed256	= (REG_09_REG1);
	//	HATSUDAN_03_angle65536	= ((HATSUDAN_03_angle65536));
		sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
	//	radius_aaa = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	//	src->math_vector.y256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		//------------------
		#endif
		//
		REG_02_DEST_X	+= ((src->center.x256));
		REG_03_DEST_Y	+= ((src->center.y256));
		//
		calculate_jikinerai();
		//
	//	REG_0c_REG4 = ((HATSUDAN_03_angle65536) );
	//	//
	//	HATSUDAN_03_angle65536 = (REG_0c_REG4 );
	//	REG_02_DEST_X	= ((src->center.x256));
	//	REG_03_DEST_Y	= ((src->center.y256));
		/* �e��x256 y256 �e�̒��S���甭�e�B */
	//	set_REG_DEST_XY(src);	/*(�e�̈ʒu���甭�e)*/
		unsigned int jj;
		for (jj=0; jj<(4); jj++)/* 4�e */
		{
			HATSUDAN_02_speed_offset	= (3+jj)<<8;/*(�e�X�g)*/
			HATSUDAN_01_speed256		= (t256(1.00)+(jj<<7)); 		/* �e�� */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}
/*---------------------------------------------------------
	�����w�̈ʒu����(�Ԋےe)���@�_���e(2��)
---------------------------------------------------------*/
extern OBJ_CALL_FUNC(add_zako_pache_laser);/* �A���X�l�`�J�[�h */
local OBJ_CALL_FUNC(spell_create_42_pache_laser1)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
				{	lz[0].laser_angle65536 = (0+(65536/6)); 		}	/* +1/8��(60/360�x) */
		else	{	lz[0].laser_angle65536 = (65536-(65536/6)); 	}	/* -1/8��(60/360�x) */
	}
	else
	if (100<laser_count)
	{
		;/*��~*/
	}
	else
	{
		;/* �ړ� **/
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 -= (100);
		}
		else
		{
			lz[0].laser_angle65536 += (100);
		}
	}

	if ((65)>((REG_10_BOSS_SPELL_TIMER)&0xff))/* 5�� 3�� */
	{
		if ((0)==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			test_pache_16_shot_AKA(src);
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
	#if 0
	/*0x280*/if ((640-1)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
	/*0x200*/if ((640-128)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
	#endif
}


/*---------------------------------------------------------
	���[�U�[�t����Ɠ���Ȃ肷�������B
	(����̃p�`�F���[�U�[���\���������)
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_43_pache_laser2)
{
	REG_09_REG1 	= (t256(64));//[�萔1]���e�ʒu�܂ł̋����B
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	?�B
	REG_09_REG1 	[�萔1]���e�ʒu�܂ł̋����B
//	REG_0a_REG2 	z
//	REG_0b_REG3 	��Փx�ʒ萔�B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_43_pache_laser2)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
				{	lz[0].laser_angle65536 = (0+(65536/6)); 		}	/* +1/8��(60/360�x) */
		else	{	lz[0].laser_angle65536 = (65536-(65536/6)); 	}	/* -1/8��(60/360�x) */
	}
	else
	if (100<laser_count)
	{
		;/*��~*/
	}
	else
	{
		;/* �ړ� **/
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 -= (100);
		}
		else
		{
			lz[0].laser_angle65536 += (100);
		}
	}
	//
//	if ((65)>((REG_10_BOSS_SPELL_TIMER)&0xff))/* 9�� 5�� 3�� */
	if ((65)>((REG_10_BOSS_SPELL_TIMER)&0xff))/* 9�� 5�� 3�� */
	{
		if ((0)==((REG_10_BOSS_SPELL_TIMER)&0x07))
		{
			test_pache_16_shot_AOI(src);
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
}
	#if 0
	/*0x280*/if ((640-1)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
	/*0x200*/if ((640-128)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_pache_laser(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
	#endif


/*---------------------------------------------------------
	[�p�`�F] "�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15
	-------------------------------------------------------
	�A�O�j�V���C���̉��e�͎G���ɂ������������I�����H�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	����̓T�C�N���C�h�Ȑ����ۂ����������ǁA�ǂ��킩��Ȃ��B
	-------------------------------------------------------
//	REG_XX_REGX 		�e�̌���������(�p�x�A�x�N�g������)
//	REG_YY_REGY 		�e�̌�������(�x�N�g������)
---------------------------------------------------------*/
#define aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU	src->hatudan_register_user_data_a01
local OBJ_CALL_FUNC(agnishine_danmaku_01_callback)/* ��u�� */
{
	/*(225==1+224==1+7*32)*/
//	if ((HATUDAN_ITI_NO_JIKAN-224) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[224]�J�E���g�����̒e */
//	if ((HATUDAN_ITI_NO_JIKAN-225) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[225]�J�E���g�����̒e */
	if ((HATUDAN_ITI_NO_JIKAN-320) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[225]�J�E���g�����̒e */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{	/*CW(���v���)*/
		//	src->rotationCCW1024 -= (6);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */ /*(-r39)*/
		//	src->rotationCCW1024 -= (3);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 -= (1);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */ /*(r40-)*/
		}
		else
		{	/*CCW(�����v���)*/
		//	src->rotationCCW1024 += (6);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */ /*(-r39)*/
		//	src->rotationCCW1024 += (3);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 += (src->jyumyou&1);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */ /*(r40-)*/
		}
//		src->hatudan_register_speed65536	+= (1<<8);		/* �e�� */
	//	src->hatudan_register_speed65536	+= (512);		/*(����)*/	/* �e�� */ /*(-r39)*/
		src->hatudan_register_speed65536	+= (32);		/*(����)*/	/* �e�� */ /*(r40-)*/
		src->hatudan_register_speed65536	-= (REG_0f_GAME_DIFFICULTY);	/* �e�� */ /*(r40-)*/
	//	#if 0/*(�T�C�N���C�h�Ȑ�)*/
	//	REG_XX_REGX++;
	//	REG_YY_REGY++;
		//------------------
//		HATSUDAN_01_speed256	= (REG_YY_REGY);
//		HATSUDAN_03_angle65536	= (REG_XX_REGX);
//		sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
//		src->center.x256 += REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
//		src->center.y256 += REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
//		//------------------
	//	#endif//vfpu_sincos()//
	//	if (0==(src->jyumyou&0x0f))/*16���1��e��u��*/
	//	if (0==(src->jyumyou&0x07))/*8���1��e��u��*/
		aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU--;
		if (0 > aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU)
		{
			aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU = REG_09_REG1;//[�萔1]n��Ɉ��e��u���B
			#if 0
			//
		//	REG_02_DEST_X	= (src->center.x256); 	/* �e��x256 �{�X���S���甭�e�B */
		//	REG_03_DEST_Y	= (src->center.y256); 	/* �e��y256 �{�X���S���甭�e�B */
			set_REG_DEST_XY(src);
			REG_00_SRC_X	= (REG_0c_REG4);
			REG_01_SRC_Y	= (REG_0d_REG5);
			tmp_angleCCW65536_src_nerai();//_bbb(zzz_player, dest);
		//
			/* �e��x256 y256 �e�̒��S���甭�e�B */
		//	set_REG_DEST_XY(src);
			HATSUDAN_01_speed256			= t256(0);	/* �e�� */
			HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
		//	HATSUDAN_03_angle65536			= (0);	/* ��p�x */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_02)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);		/* [�Ԓe] */
			hatudan_system_regist_single();/* (r33-) */
			#endif
		}
	}
	else	/* ���e�� 225 �J�E���g�ȏ�͒e�������B */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
	[�萔1]n��Ɉ��e��u���B
	0easy:			16
	1normal:		12
	2hard:			8
	3lunatic:		4
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_2e_pache_agni_shine_1)
{
//	aaa_TAMA_DATA_10_TAMA_OKI_KANKAKU	= (0);// �J�E���^
	REG_09_REG1 	= ( (16)-((REG_0f_GAME_DIFFICULTY)<<2));//[�萔1]n��Ɉ��e��u���B
//	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[�萔1]�J�̑��x
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[�萔2]�ԐN�i�C���Ȃ���p�x
	card.danmaku_callback[1] = agnishine_danmaku_01_callback;/*(��u���p)*/
//	card.danmaku_callback[2] = aaa_tahane_danmaku_02_callback;/*(�쒵�˗p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	����(r35)�̃X�y�J�V�X�e���̏ꍇ�A�d�l��
	�X�y�J���܂����郌�W�X�^�l��ێ��ł��Ȃ��B
	(�X�y�J���n�܂�^�C�~���O�ŋ����I�Ƀ��W�X�^��0�N���A����Ă��܂���)
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_0c_REG4 	���e��A�W�܂�X���W(t256�`��)
	REG_0d_REG5 	���e��A�W�܂�Y���W(t256�`��)
	REG_0e_REG6 	�Ă��Ɓ[
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_2e_pache_agni_shine_1)
{
//	if ((SPELL_TIME_0256-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(��Ԏn��)*/
//	if (0== (REG_10_BOSS_SPELL_TIMER & 0x7f) ) /* 128���1��Ȃ猂�� */
//	if (0x0f > (REG_10_BOSS_SPELL_TIMER & 0x7f) ) /* 128���1��Ȃ猂�� */ /*(-r39)*/
	if ((0x0f+((REG_0f_GAME_DIFFICULTY)<<1)) > (REG_10_BOSS_SPELL_TIMER & 0x3f) ) /* 64���1��Ȃ猂�� */ /*(r40-)*/
	{
	//	HATSUDAN_01_speed256			= t256(1.0);	/* �e�� */
	//	HATSUDAN_02_speed_offset		= t256(5);/*(�e�X�g)*/
		HATSUDAN_01_speed256			= t256(0.75);	/* �e�� */
		HATSUDAN_02_speed_offset		= t256(1.5);/*(�e�X�g)*/
	//	HATSUDAN_03_angle65536			= 0;	/* ��p�x */ /*(-r39)*/
		HATSUDAN_03_angle65536			= REG_0e_REG6; 	/* ��p�x */ /*(r40-)*/
		REG_0e_REG6 					+= ((ra_nd())&0x0f); /*(r40-)*/
		REG_0e_REG6 					+= ((REG_0f_GAME_DIFFICULTY)<<8); /*(r40-)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|((ra_nd()) & TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [�ԗؒe] */
		HATSUDAN_06_n_way				= (13); 	/* [13way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(13));	/* �����p�x(65536[360/360�x]�� 18 ����) */
		// 13�����B�A�O�j�V���C���B
		// 11�����B�A�O�j�V���C���㋉�B
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		u32 aaa = ra_nd();
		REG_0c_REG4 = ((aaa    )&0xff00) + t256(((480-128-32-256)/2));
		REG_0d_REG5 = ((aaa<<8 )&0xff00) + t256(((272-256)/2));
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}
	#if 0/*(����)*/
	/*"���򉻃A�O�j�V���C��"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2���1��Ȃ猂�� */
	{
		HATSUDAN_01_speed256			= (t256(1.0));		/* �e�� */
		HATSUDAN_02_speed_offset		= (t256(1.5));/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [�ԗؒe] */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
	#endif


/*---------------------------------------------------------
	[�p�`�F] "�@�����u�v�����Z�X�E���f�B�l�v" No.16
	-------------------------------------------------------
	���̃J�[�h�̌��ȏ��́A���e�̂����蔻�肪(�����������)�ł������B
	(���e�̂����蔻��͏������̂�)
	�����璆�e�͌����ڂ��댯�B
	�Ȃ񂾂��ǁA���݂̖͕핗�ɒ��e�������ȁ[�B
	�ےe��(��ʒe�A�~�j�e�A���e�A�g���e)���������B
	���e�͂����蔻����傫�����A�Ƃ肠�����G���ɕύX���悤���ȁH
	-------------------------------------------------------
	���[�U�[���A�Ԋu0.5s���炢(6��)�v3.0s?(8��?)
	0.3s���炢�x��
	��ǂ��e�A�Ԋu0.3s���炢(6��)�v2.0s?
	2.0s���炢�x��
	----
	8.0s==60x8==480(?)
	6.0s==60x6==360(?)
	----
	�Ƃ������Ɋ�����̂����A�����Ԃ�1 loop 4.0s���炢���ۂ��B
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	temp00_angle
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_2f_pache_princess_undine)
{
	if (64 > ((REG_10_BOSS_SPELL_TIMER) ))
	{
		;
	}
	else
	if (128 > ((REG_10_BOSS_SPELL_TIMER) ))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&(16-1)))
		{
			/* ���@�_�� �p���쐬 */
			REG_02_DEST_X	= ((src->center.x256));
			REG_03_DEST_Y	= ((src->center.y256));
			calculate_jikinerai();
		//	REG_09_REG1 = HATSUDAN_03_angle65536;
			/* ���@�_�� 3way ��laser�e */
		//	HATSUDAN_03_angle65536			= REG_09_REG1;
		//	HATSUDAN_01_speed256			= (t256(0.75)); 	/* �e�� */
		//	HATSUDAN_01_speed256			= (t256(1.50)); 	/* �e�� */
			HATSUDAN_01_speed256			= (t256(1.00)); 	/* �e�� */
			HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI);//			/* [�e] */
			HATSUDAN_06_n_way				= (11); 	/* [11way] */				/* ���e�� */
			HATSUDAN_07_div_angle65536		= (int)(65536/(24)); /* �����p�x(65536[360/360�x]�� 16 ����) */ /* 1����n���������p�x */
			hatudan_system_regist_n_way();/* (r33-) */
		}
	}
	else
	if (250 > ((REG_10_BOSS_SPELL_TIMER) ))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&(2-1)))
		{
			/* ���@�_�� 3way ��laser�e */
			//
		//	HATSUDAN_01_speed256			= (t256(0.75)); 	/* �e�� */
		//	HATSUDAN_01_speed256			= (t256(1.50)); 	/* �e�� */
			HATSUDAN_01_speed256			= (t256(4.00)); 	/* �e�� */
			HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_HARI32_00_AOI);// 				/* [�j�e] */
			/* 1���[�U�[�̒��� */
			#define pache_02_LASER_LENGTH (4)
			int jj;
			jj = 0;// for (jj=0; jj<(128*pache_02_LASER_LENGTH); jj+=128)
			{
				HATSUDAN_03_angle65536		= REG_09_REG1;
				HATSUDAN_06_n_way				= (3);	/* [3way] */	/* ���e�� */
				/* �S���̃X�L�}(���ԑS��)�A���Ԃ����鎞��time_out��128������B */
				#define pache_00_SUKIMA_ZENBU		((128/*time_out*/)<<4)/*(4096)*/
				/* �v���C���[��������X�L�} */
				#define pache_01_SUKIMA_KUGURU		(1280)
				HATSUDAN_07_div_angle65536		= jj+((REG_10_BOSS_SPELL_TIMER)<<4)-(pache_00_SUKIMA_ZENBU-pache_01_SUKIMA_KUGURU);
				//(int)(65536/(16));	/* �����p�x(65536[360/360�x]�� n ����) */	/* 1����n���������p�x */
				hatudan_system_regist_n_way();/* (r33-) */
			}
			if (0==((REG_10_BOSS_SPELL_TIMER)&(32-1)))
			{
				/* ���@�_��Ȃ� 16way(?)17way(???) �ےe */
				HATSUDAN_01_speed256			= (t256(0.50)); 					/* �e�� */
				HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
				HATSUDAN_03_angle65536			= REG_09_REG1 + (65536/2);
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);// 			/* [�ےe] */
				HATSUDAN_06_n_way				= (17); 	/* [17way] */				/* ���e�� */
				HATSUDAN_07_div_angle65536		= (int)(65536/(19));	/* �����p�x(65536[360/360�x]�� 19 ����) */
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (254 == ((REG_10_BOSS_SPELL_TIMER) ))
	{
		/* ���@�_�� �p���쐬 */
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();
		REG_09_REG1 = HATSUDAN_03_angle65536;
	}
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�@�@�ؕ��u�V���t�B�z�����v" No.17
	-------------------------------------------------------
	�ؕ��u�V���t�B�z�����v�͒e�̓����������Ƒ����B
	�i�o�n�߂��������x���Ȃ�A��ōĉ�������j
	�R�`�S�����o�Ă��Ȃ��ŁA�ザ��Ȃ��ĉ�����o�Ă���C������B
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	����?�B
	REG_09_REG1 	[�萔1]�J�̑��x
	REG_0a_REG2 	?�B
	REG_0b_REG3 	?�B
//
	REG_0c_REG4 	�ꎞ�g�p�}�X�N�B
//	REG_0d_REG5 	?�B
//	REG_0e_REG6 	?�B
//	REG_0f_REG7 	��Փx�B

---------------------------------------------------------*/


/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B(0x0000)
	�����e�B���v���B(0x0100)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(pache_sh1_danmaku_01_callback)/* �V���t�B�z���� */
{
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* ���e�G�t�F�N�g���͖��� */
	{
		return;
	}
	#endif
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-64) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��64�J�E���g�o�߂����e */
	{
		angle_to_vector(src);
	//	src->math_vector.x256	= -t256(1.0);
	//	src->math_vector.y256	=  t256(1.0);
	//	src->hatudan_register_tra65536 = (0);
	}
	#endif
//	common_danmaku_01_amefuri_callback(src);
//	src->center.x256 &= (0x0001ffff);
	if (t256(32) > (src->center.x256) )
	{
		src->center.x256 += t256(320);/*(320==480-128-32)*/
	}
//	move_vector_gamen_sita(src);/*(�x�N�g���ړ�+��ʉ�����)*/


	REG_08_REG0 += (31);
	REG_0c_REG4 = REG_08_REG0;
	REG_0c_REG4 &= 0xff;

	// �x�N�g���e�ړ��Bmove vector.
//	src->center.x256 -= (t256(1.0));	/*fps_factor*/
//	src->center.y256 += (t256(1.0));	/*fps_factor*/
//	src->center.x256 += (src->math_vector.x256); 	/*fps_factor*/
//	src->center.y256 += (src->math_vector.y256); 	/*fps_factor*/
//	src->center.x256 += (((src->math_vector.x256)*(REG_0c_REG4))>>8);	/*fps_factor*/
//	src->center.y256 += (((src->math_vector.y256)*(REG_0c_REG4))>>8);	/*fps_factor*/
	src->center.x256 += (((src->math_vector.x256)*(REG_0c_REG4))>>7);/*����*/	/* �e��(###x256�{) */	/*fps_factor*/
	src->center.y256 += (((src->math_vector.y256)*(REG_0c_REG4))>>7);/*����*/	/* �e��(###x256�{) */	/*fps_factor*/
//	src->center.x256 += 1;//(t256( 0.1)); 	/*fps_factor*/
//	src->center.y256 += 1;//(t256(-0.1)); 	/*fps_factor*/
//	src->math_vector.y256 += ((src->hatudan_register_tra65536)<<8);	/* ���������� */
//	src->math_vector.y256 += ((src->hatudan_register_tra65536) );	/* ���������� */


	// ��������
	#if 1
	// ��ʊO�͏����B
//	hatudan_system_B_gamen_gai_tama_kesu(src);/*(�ʏ�e�p)*/
	/* ��ʊO�̏ꍇ�͒e�������B */
	if (
//	(0 < (rect_clip.bullet_clip_min.x256)-(src->center.x256) ) ||
//	(0 > (rect_clip.bullet_clip_max.x256)-(src->center.x256) ) ||
//	(0 < (rect_clip.bullet_clip_min.y256)-(src->center.y256) ) ||
//	(0 > (rect_clip.bullet_clip_max.y256)-(src->center.y256) ) )	/*(��ʉ��̂�)*/
//	( (src->center.y256) > (rect_clip.bullet_clip_max.y256) ) )	/*(��ʉ��̂�)*/
	( (src->center.y256) > (t256(272)) ) )	/*(��ʉ��̂�)*/
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}


/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_30_pache_sylphy_horn_1)
{
	REG_09_REG1 	= (t256(1.0));// [�萔1]�J�̑��x
	card.danmaku_callback[1] = pache_sh1_danmaku_01_callback;/*(�g�t���ےe�B�����e�B? ���B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_30_pache_sylphy_horn_1)
{
//	sta tic int REG_08_REG0;
	REG_08_REG0 += (REG_10_BOSS_SPELL_TIMER<<4);
	/*"���򉻃V���t�B�z����"*/
//	if (0==(REG_10_BOSS_SPELL_TIMER & 0x0f))
	if (0==(REG_10_BOSS_SPELL_TIMER & 0x07))
	{
		const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
//		REG_0b_REG3 = replay_rand32 & 0xff;
		REG_08_REG0 += ra_nd32;
		REG_02_DEST_X	= t256(16)+(ra_nd32 & 0xff00)+(ra_nd32 & 0x3f00);	/* �e��x256 */
//		REG_03_DEST_Y	+= -t256(32)+((ra_nd32>>8)&0x3f00); 				/* �e��y256 */
//		REG_02_DEST_X	= ((replay_rand32) & (0x0001ffff) );/*(512-1==0x01ff)*/
		REG_03_DEST_Y	= (t256(-8));/*(��ʊO����~��)*/
	//	REG_03_DEST_Y	= (t256(32));/*(��ʊO����~��)*/
//
//		HATSUDAN_01_speed256			= t256(1.0);		/* �e�� */
//		HATSUDAN_01_speed256			= t256(12.0);		/* �e�� */
//		HATSUDAN_01_speed256			= t256(33.0);		/* �e�� */
//		HATSUDAN_01_speed256			= t256(128.0);		/* �e�� */
		HATSUDAN_01_speed256/*����(�����H)*/	= t256(1.0);		/* �e��(###x256�{) */
//		HATSUDAN_02_speed_offset		= t256(3.0);/*(�e�X�g)*/
	//	HATSUDAN_02_speed_offset		= t256(1.5);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= (0);/*(�e�X�g)*/
//		HATSUDAN_03_angle65536			= (REG_0a_REG2);
//		HATSUDAN_03_angle65536			= (65536)-(65536/8);//+(REG_0b_REG3);
		HATSUDAN_03_angle65536			= (65536)-(65536/16)-(ra_nd32 & 0x0fff);//+(REG_0b_REG3);
//		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_05_MIDORI);			/* [�ΕĒe] */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		hatudan_system_regist_single();
	}
}

/*---------------------------------------------------------
[�p�`�F] "�@�@�y���u���C�W�B�g�����g���v" No.18
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B(0x0000)
	�����e�B���v���B(0x0100)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(pache_rt1_danmaku_01_callback)/* �}�[�L�����|�C�Y�� */
{
	int aaa_jikan;
	aaa_jikan = ((src->jyumyou) & 0x3f);/* 64�Ŋ������]�� */
	if (0==aaa_jikan)/* 0�Ȃ獶�E�ɂԂ��B */
	{
		src->rotationCCW1024 += ((ra_nd()&0xff)-128);/* 90/360 �x(+-45/360) �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		src->hatudan_register_speed65536 = (t256(1.00)<<8); 	/* �e�� */
	}
	else
	/* 32�ȏ�͌��� */
//	if ((32) < aaa_jikan)/* ���e�G�t�F�N�g�ォ��[64]�J�E���g�����̒e */
	{
		src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
//	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
	// ��ʊO�͏����B
//	hatudan_system_B_gamen_gai_tama_kesu(src);/*(�ʏ�e�p)*/
}

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_31_pache_rage_tririton_1)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(�g�t���ےe�B�����e�B? ���B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_31_pache_rage_tririton_1)
{
	// �ŏ��̔��� (64�t���[��==��1�b)�΂�T���B
	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		if ((REG_10_BOSS_SPELL_TIMER&3)<=(REG_0f_GAME_DIFFICULTY))
		{
			HATSUDAN_01_speed256			= (t256(2.00) );						/* �e�� */
			HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
			HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(������)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [���e] */
		//	HATSUDAN_06_n_way				= (1);	/* [1way] */				/* ���e�� */
		//	HATSUDAN_07_div_angle65536		= (int)(65536/(24));	/* �����p�x(65536[360/360�x]�� 19 ����) */
			hatudan_system_regist_single();
		}
	}
}

	#if 0//(1==PACHE_TEST)
	{
		/* ���@�_�� 3way ��laser�e */
		HATSUDAN_03_angle65536			= ((REG_10_BOSS_SPELL_TIMER));
	}
	if ((0)==((REG_10_BOSS_SPELL_TIMER)&4)) // 4���1��`�F�b�N
	{
		exchange_damnaku_pache_sin_sokudo_maru_dan();
	}
	#endif


/*---------------------------------------------------------
[�p�`�F] "�@�@�����u���^���t�@�e�B�[�O�v" No.19
	-------------------------------------------------------
---------------------------------------------------------*/


/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B(0x0000)
	�����e�B���v���B(0x0100)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(pache_mt1_danmaku_01_callback)/* �V���o�[�h���S�� */
{
	if ((HATUDAN_ITI_NO_JIKAN-32) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]�J�E���g�����̒e */
	{
	//	src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	-= (t256(0.03)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	else
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]...[128]�J�E���g�����̒e */
	{
		/*(�e�͏�����)*/
		src->jyumyou = JYUMYOU_NASI;/*(�e������)*/
		//
		/* �e��x256 y256 �e�̒��S���甭�e�B */
		set_REG_DEST_XY(src);	/*(�e�̈ʒu���甭�e)*/
		//
		HATSUDAN_01_speed256			= t256(1.00);		/* �e�� */
	//	HATSUDAN_02_speed_offset		= (1<<(2+8));/* x4�{ �Ă���*/
		HATSUDAN_02_speed_offset		= t256(3+1);/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(������)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_02_KIIRO);	/* [���F ��ʒe] */
		HATSUDAN_06_n_way				= (8);							/* [8way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 	/* �����p�x(65536[360/360�x]�� 8 ����) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_32_pache_metal_fatigue)
{
	card.danmaku_callback[1] = pache_mt1_danmaku_01_callback;/*(���F ��ʒe�B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B(kankaku)
//	REG_09_REG1 	�J�E���^�B(2���)
//	REG_0a_REG2 	�e�F(�����e)�B
//	REG_0b_REG3 	�e�p�x(�����e)�B	�e�p�x(�A�e)�B
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_32_pache_metal_fatigue)
{
	if ((SPELL_TIME_0128-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(��Ԏn��)*/
	{
		HATSUDAN_01_speed256			= t256(2.50);		/* �e�� */
	//	HATSUDAN_02_speed_offset		= (0<<(2+8));/* x4�{ �Ă���*/
		HATSUDAN_02_speed_offset		= t256(3+0);/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (0);/*(�^��)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_02_KIIRO);	/* [���F ��ʒe] */
		HATSUDAN_06_n_way				= (8);							/* [8way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 	/* �����p�x(65536[360/360�x]�� 8 ����) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
		cpu_bullet_play_15_auto();
	}
}

/*---------------------------------------------------------
[�p�`�F] "�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(�Ƃ肠����)*/
local OBJ_CALL_FUNC(spell_create_33_pache_agni_shine_2)
{
}
#else
	#define spell_create_33_pache_agni_shine_2		spell_create_2e_pache_agni_shine_1
	#define spell_init_33_pache_agni_shine_2		spell_init_2e_pache_agni_shine_1
#endif


/*---------------------------------------------------------
[�p�`�F] "�@�@�ؕ��u�V���t�B�z�����㋉�v" No.21
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(�Ƃ肠����)*/
local OBJ_CALL_FUNC(spell_create_34_pache_sylphy_horn_2)
{
}
#else
	#define spell_create_34_pache_sylphy_horn_2 	spell_create_30_pache_sylphy_horn_1
	#define spell_init_34_pache_sylphy_horn_2		spell_init_30_pache_sylphy_horn_1
#endif


/*---------------------------------------------------------
[�p�`�F] "�y���u���C�W�B�g�����g���㋉�v" No.22
	-------------------------------------------------------
	�Ă���
---------------------------------------------------------*/
#if 0
local OBJ_CALL_FUNC(spell_init_31_pache_rage_tririton_1)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(�g�t���ےe�B�����e�B? ���B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
#endif
#if (1)/*(�Ƃ肠����)*/
local OBJ_CALL_FUNC(spell_create_35_pache_rage_tririton_2)
{
	// �ŏ��̔��� (64�t���[��==��1�b)�΂�T���B
//	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* �e�� */
		HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(������)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [���e] */
	//	HATSUDAN_06_n_way				= (1);	/* [1way] */				/* ���e�� */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(24));	/* �����p�x(65536[360/360�x]�� 19 ����) */
		hatudan_system_regist_single();
	}
}
#else
	#define spell_create_35_pache_rage_tririton_2	spell_create_31_pache_rage_tririton_1
//	#define spell_init_35_pache_rage_tririton_2 	spell_init_31_pache_rage_tririton_1
#endif

/*---------------------------------------------------------
[�p�`�F] "�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23
	-------------------------------------------------------
	"�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23 ==
		"�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20 +
		"�ԑ��"
---------------------------------------------------------*/
/*(�Ƃ肠����)*/
local OBJ_CALL_FUNC(spell_create_36_pache_agni_radiance)
{
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2���1��Ȃ猂�� */
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* �e�� */
		HATSUDAN_02_speed_offset		= (t256(4));/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (ra_nd());
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [�ԑ�ʒe] */
		hatudan_system_regist_single();
	}
	/*"���򉻃A�O�j�V���C��"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2���1��Ȃ猂�� */
	{
		HATSUDAN_02_speed_offset		= t256(1.5);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [�ԗؒe] */
		HATSUDAN_01_speed256			= (t256(1.0));		/* �e�� */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�@�����u�x���[�C�����C�N�v" No.24
	-------------------------------------------------------
	�d�l:
		"�@�����u�v�����Z�X�E���f�B�l�v" No.16
		�̏㋉�ł���������̕����ȒP�B
	-------------------------------------------------------
	�d�l:
		Lunatic �� Hard �ɔ�ׂē������������B
---------------------------------------------------------*/
#if (0)/*(�Ƃ肠����)*/
local OBJ_CALL_FUNC(spell_create_37_pache_bury_in_lake)
{
}
#else
	#define spell_create_37_pache_bury_in_lake		spell_create_2f_pache_princess_undine
#endif

/*---------------------------------------------------------
[�p�`�F] "�@�@�@�ؕ��u�O���[���X�g�[���v" No.25
	-------------------------------------------------------
	�V���t�B�z�����㋉�̏㋉�ŁB
---------------------------------------------------------*/
#if (0)/*(�Ƃ肠����)*/
local OBJ_CALL_FUNC(spell_create_38_pache_green_storm)
{
}
#else
	#define spell_create_38_pache_green_storm		spell_create_30_pache_sylphy_horn_1
	#define spell_init_38_pache_green_storm 		spell_init_30_pache_sylphy_horn_1
#endif


/*---------------------------------------------------------
[�p�`�F] "�@�@�y���u�g�����g���V�F�C�N�v" No.26
	-------------------------------------------------------
	"�@�@�y���u�g�����g���V�F�C�N�v" No.26 ==
		"�y���u���C�W�B�g�����g���㋉�v" No.22 +
		"��������ʒe�B���@�_���B"
---------------------------------------------------------*/
#if 0
local OBJ_CALL_FUNC(spell_init_31_pache_rage_tririton_1)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(�g�t���ےe�B�����e�B? ���B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
#endif
local OBJ_CALL_FUNC(spell_create_39_pache_tririton_shake)
{
//	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	�J��Ԃ��J�E���^�B */
	// �ŏ��̔��� (64�t���[��==��1�b)�΂�T���B
	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.00) );						/* �e�� */
		HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(������)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [���e] */
	//	HATSUDAN_06_n_way				= (1);	/* [1way] */				/* ���e�� */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(24));	/* �����p�x(65536[360/360�x]�� 19 ����) */
		hatudan_system_regist_single();
	}
	/*"��������ʒe�B���@�_���B"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0xf) ) /* 16���1��Ȃ猂�� */
	{
		#if 1
	//	REG_02_DEST_X	= ((src->center.x256));
	//	REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* ���@�_���p�쐬 */
	//	REG_0a_REG2 = HATSUDAN_03_angle65536;/* ���@�_���p */
		#endif
		HATSUDAN_01_speed256			= (t256(2.0));		/* �e�� */
	//	HATSUDAN_02_speed_offset		= t256(1.5);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= t256(5);/*(�e�X�g)*/
	//	HATSUDAN_03_angle65536			= (REG_0a_REG2);/* ���@�_���p */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_02_KIIRO);		/* [���F ��ʒe] */
		HATSUDAN_06_n_way				= (7);	/* [7way] */				/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(7)); /* �����p�x(65536[360/360�x]�� 7 ����) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�@�����u�V���o�[�h���S���v" No.27
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B(0x0000)
	�����e�B���v���B(0x0100)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(pache_mt2_danmaku_01_callback)/* �V���o�[�h���S�� */
{
	if ((HATUDAN_ITI_NO_JIKAN-32) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]�J�E���g�����̒e */
	{
	//	src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	-= (t256(0.05)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	else
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]...[128]�J�E���g�����̒e */
	{
		if (0==(ra_nd()&1))
		{
			src->rotationCCW1024 -= (13);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		else
		{
			src->rotationCCW1024 += (777);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		src->hatudan_register_speed65536	+= (1<<8);			/* �e�� */
	}
	else
	if ((HATUDAN_ITI_NO_JIKAN-128) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]...[128]�J�E���g�����̒e */
	{
	//	src->hatudan_register_speed65536	+= (t256(0.01)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	+= (1<<8);			/* �e�� */
	}
	else	/* ���e�� 256 �J�E���g�ȏ�͒e�������B */
//	if ((HATUDAN_ITI_NO_JIKAN-256) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[256]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_3a_pache_silver_dragon)
{
	card.danmaku_callback[1] = pache_mt2_danmaku_01_callback;/*(���e�B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B(kankaku)
//	REG_09_REG1 	�J�E���^�B(2���)
//	REG_0a_REG2 	�e�F(�����e)�B
//	REG_0b_REG3 	�e�p�x(�����e)�B	�e�p�x(�A�e)�B
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_3a_pache_silver_dragon)
{
	if (30==REG_10_BOSS_SPELL_TIMER)
	{
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_00_SIRO);			/* ���e */	/* [���۔��F�e�͖����̂�] */
		HATSUDAN_06_n_way				= (8);							/* [8way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 	/* �����p�x(65536[360/360�x]�� 8 ����) */
		unsigned int jj;
		for (jj=0; jj<(5); jj++)/* 5�w */
		{
		//	HATSUDAN_02_speed_offset		= (jj<<(2+8));/* x4�{ �Ă���*/
			HATSUDAN_02_speed_offset		= (3+jj)<<8;/*(�e�X�g)*/
			HATSUDAN_01_speed256			= (t256(1.00)+(jj<<7)); 		/* �e�� */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		//
		cpu_bullet_play_15_auto();
	}
}


/*---------------------------------------------------------
	[�p�`�F] "�΁��y���u���[���@�N�������N�v" No.28
	-------------------------------------------------------
	"�΁��y���u���[���@�N�������N�v" No.28 ==
		"�@�@�y���u���C�W�B�g�����g���v" No.18 +
		"�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15
	-------------------------------------------------------

	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B

---------------------------------------------------------*/
#if 0
local OBJ_CALL_FUNC(spell_init_31_pache_rage_tririton_1)
{
	card.danmaku_callback[1] = pache_rt1_danmaku_01_callback;/*(�g�t���ےe�B�����e�B? ���B)*/
	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
#endif
local OBJ_CALL_FUNC(spell_create_3b_pache_lava_cromlech)
{
//	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	�J��Ԃ��J�E���^�B */
	// �ŏ��̔��� (64�t���[��==��1�b)�΂�T���B
	if (((64))<(REG_10_BOSS_SPELL_TIMER)) // 192== (64*3)
	{
		HATSUDAN_01_speed256			= (t256(2.00) ); 					/* �e�� */
		HATSUDAN_02_speed_offset		= (t256(3));/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (ra_nd() &(0x7fff)) + (65536/4)+(65536/2);/*(������)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_06_KI_IRO);//			/* [���e] */
		hatudan_system_regist_single();
	}
	/*"���򉻃A�O�j�V���C��"*/
	if (0== (REG_10_BOSS_SPELL_TIMER & 0x01) ) /* 2���1��Ȃ猂�� */
	{
		HATSUDAN_01_speed256			= (t256(1.0));		/* �e�� */
		HATSUDAN_02_speed_offset		= t256(1.5);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);			/* [�ԗؒe] */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0xff;
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
}


/*---------------------------------------------------------
	[�p�`�F] "�؁��Ε��u�t�H���X�g�u���C�Y�v" No.29
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(���쐬)*/
local OBJ_CALL_FUNC(spell_create_3c_pache_forest_blaze)
{
}
#else
	#define spell_create_3c_pache_forest_blaze		spell_create_1f_koakuma
#endif


/*---------------------------------------------------------
	[�p�`�F] "�@�����ؕ��u�E�H�[�^�[�G���t�v" No.30
	-------------------------------------------------------
	�����悭�킩���B
	�p�`�F���ۂ��H�B
	-------------------------------------------------------
	���[��B����Ȃ��Ă�B
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�p�x�B
	REG_09_REG1 	�p�x�ۑ��p�B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_3d_pache_water_elf)
{
	/* ���� ���@�_�� 24�����S���ʁA���F�e BULLET_MARU10_04_MIZU_IRO */
	/* �ؕ� ���@�_�� 20�����S���ʁA�ΐF�e BULLET_MARU10_05_MIDORI */
	/* �ؕ� ���@�_�� 10���������A���ʒe BULLET_OODAMA32_00_AOI */
//	sta tic int REG_08_REG0;
	REG_08_REG0 += (REG_10_BOSS_SPELL_TIMER<<4);
	if (0==((REG_10_BOSS_SPELL_TIMER)&(16-1)))
	{
	//	int REG_09_REG1;
		/* ���@�_�� �p���쐬 */
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();
		REG_09_REG1 = HATSUDAN_03_angle65536;/* hatudan_system_regist_katayori_n_way()�͊p�x�j�󂷂�̂ŁA�܂��ۑ��B */
		REG_08_REG0 += (REG_09_REG1);
		//	HATSUDAN_01_speed256				= (t256(0.75) );		/* �e�� */
		//	HATSUDAN_01_speed256				= (t256(1.50) );		/* �e�� */
	//		HATSUDAN_01_speed256				= (t256(1.00) );		/* �e�� */
			HATSUDAN_01_speed256				= (t256(1.00) );		/* �e�� */
			HATSUDAN_02_speed_offset			= t256(3);/*(�e�X�g)*/
		if (32>((REG_10_BOSS_SPELL_TIMER)&(128-1)))
		{
			/* �ؕ� ���@�_�� 10���������A���ʒe BULLET_OODAMA32_00_AOI */
			HATSUDAN_03_angle65536				= REG_09_REG1;/*[�p�x�A�ݒ�(����)]*/
			//
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_OODAMA32_00_AOI);//				/* [�e] */
			HATSUDAN_06_n_way					= (10); 	/* [10way] */					/* ���e�� */
			HATSUDAN_07_div_angle65536			= (int)(65536/(20));	/* �����p�x(65536[360/360�x]�� 20 ����) */
			hatudan_system_regist_n_way();/* (r33-) */
		}
			HATSUDAN_02_speed_offset			= t256(1);/*(�e�X�g)*/
		if (0x0a90&(REG_08_REG0))
		{
			/* �ؕ� ���@�_�� 20�����S���ʁA�ΐF�e BULLET_MARU10_05_MIDORI */
			HATSUDAN_03_angle65536				= REG_09_REG1;/*[�p�x�A����]*/
			HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_05_MIDORI);//				/* [�Βe] */
			HATSUDAN_06_n_way					= (20); 	/* [20way] */					/* ���e�� */
			HATSUDAN_07_div_angle65536			= (int)(65536/(20));	/* �����p�x(65536[360/360�x]�� 20 ����) */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		if (0x0670&(REG_08_REG0))
		{
			/* ���� ���@�_�� 24�����S���ʁA���F�e BULLET_MARU10_04_MIZU_IRO */
			HATSUDAN_03_angle65536				= REG_09_REG1;/*[�p�x�A����]*/
			HATSUDAN_05_bullet_obj_type 		= (BULLET_MARU10_BASE + TAMA_IRO_04_MIZU_IRO);//				/* [���F�e] */
			HATSUDAN_06_n_way					= (24); 	/* [24way] */					/* ���e�� */
			HATSUDAN_07_div_angle65536			= (int)(65536/(24));	/* �����p�x(65536[360/360�x]�� 24 ����) */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	[�p�`�F] "���������u�}�[�L�����|�C�Y���v" No.31
	-------------------------------------------------------
	����܎��ĂȂ����ǁA���������p�`�F���ۂ��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B(0x0000)
	�����e�B���v���B(0x0100)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(pache_mp1_danmaku_01_callback)/* �}�[�L�����|�C�Y�� */
{
//	if (cnt < 80)/* 80�����͌��� */
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]�J�E���g�����̒e */
	{
		src->hatudan_register_speed65536	-= (t256(0.02)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	= psp_max( (0), (src->hatudan_register_speed65536) );
	}
	else
//	if (cnt == 80)/* 80�Ȃ獶�E�ɂԂ��B */
//	if ((HATUDAN_ITI_NO_JIKAN-80) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[80]�J�E���g�����̒e */
	if ((HATUDAN_ITI_NO_JIKAN-96) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64]...[96]�J�E���g�����̒e */
	{
		#if 0
		s32 ang = bullet_get_angle(eb);
		int add_angle;
		add_angle = ((A_ROUND65536)/4 + deg360to_rad(replay_rand()%10));
		bullet_set_angle(eb, ang - add_angle );
		bullet_set_angle(eb, ang + add_angle );
		#else
		if (src->hatudan_register_spec_data & TAMA_SPEC_AO_MIZU_MERCURY_BIT)
		{
			src->rotationCCW1024 -= (1);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		else
		{
			src->rotationCCW1024 += (1);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		#endif
		src->hatudan_register_speed65536	+= (t256(0.01)<<8); 		/* �e�� */
	}
	else	/* 81���� 100�����͉��� */
//	if (cnt < 200)/* 81���� 200�����͉���(�A��100�Œe��������) */
	if ((HATUDAN_ITI_NO_JIKAN-256) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[96]...[256]�J�E���g�����̒e */
	{
		if (0==(ra_nd()&1))
		{
			goto test_aaa;
		}
	//	bullet_set_speed(eb, (spd256+t256(0.015) ) );/*(����)*/
		if (src->hatudan_register_spec_data & TAMA_SPEC_AO_MIZU_MERCURY_BIT)
		{
			src->rotationCCW1024 -= (1);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		else
		{
			src->rotationCCW1024 += (1);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
	//	src->hatudan_register_speed65536	+= (t256(0.01)<<8); 		/* �e�� */
		src->hatudan_register_speed65536	-= (512);		/* ���� */
	test_aaa:
		;
	}
	else	/* ���e�� 256 �J�E���g�ȏ�͒e�������B */
//	if ((HATUDAN_ITI_NO_JIKAN-256) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[256]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_3e_pache_mercury_poison)
{
	card.danmaku_callback[1] = pache_mp1_danmaku_01_callback;/*(�g�t���ےe�B�����e�B�����v���/���v���B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B(kankaku)
	REG_09_REG1 	�J�E���^�B(2���)
//	REG_0a_REG2 	�e�F(�����e)�B
//	REG_0b_REG3 	�e�p�x(�����e)�B	�e�p�x(�A�e)�B

---------------------------------------------------------*/
#define MERCURY_POISON_KANKAKU_030 (30)
#define MERCURY_POISON_KANKAKU_015 (15)
local OBJ_CALL_FUNC(spell_create_3e_pache_mercury_poison)
{
	REG_08_REG0--;
	if (0>REG_08_REG0)
	{
		REG_08_REG0 = MERCURY_POISON_KANKAKU_030;
		#if 0
		if (0==cg_game_difficulty)
		{
			;
		}
//		if (1==cg_game_difficulty)
		else
		{
			if (enemy.hp < (enemy.maxHp*2/3))	{REG_08_REG0 = (MERCURY_POISON_KANKAKU_015);}
		}
		#endif
	//	static unsigned int REG_09_REG1 = (0);
		REG_09_REG1 ^= 1;
		if (0==REG_09_REG1)
		{
		//	br.BULLET_REGIST_color			= ((8)+(0*5));/*(���F?)*/
			HATSUDAN_05_bullet_obj_type = BULLET_MARU10_BASE + TAMA_IRO_03_AOI; 		/* [���ېF�e] [���ې��F�e] */
		}
		else
		{
		//	br.BULLET_REGIST_color			= ((8)+(1*5));/*(���F?)*/
			HATSUDAN_05_bullet_obj_type = BULLET_MARU10_BASE + TAMA_IRO_06_KI_IRO;		/* [���ېF�e] [���ې��F�e] */
		}
		HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|((REG_09_REG1)<<8)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_06_n_way				= (18); 	/* [18way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(18));	/* �����p�x(65536[360/360�x]�� 18 ����) */
		{
			unsigned int random_angle_offset;
		//	random_angle_offset = deg360to_rad((replay_rand() % 5)); // 910.222222222222222222222222222222
			random_angle_offset = (ra_nd()&0x0fff);// 0xe38 == 3640 == 65536/18
			HATSUDAN_03_angle65536		= random_angle_offset;	/* ��p�x */
	//		HATSUDAN_01_speed256		= t256(3.0) + jj*t256(0.3);
			HATSUDAN_01_speed256		= t256(2.0);	/* �e�� */
	//		bullet_set_till_time(100);/*(�����I�ɏ����鎞��)*/
			hatudan_system_regist_katayori_n_way();/* (r33-) */
			HATSUDAN_03_angle65536		= random_angle_offset;	/* ��p�x */
			HATSUDAN_01_speed256		= t256(2.3);	/* �e�� */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		//
		cpu_bullet_play_15_auto();
	}
}

/*---------------------------------------------------------
	[�p�`�F] "�y�������u�G�������h���K���X�v" No.32
	-------------------------------------------------------
	�e�L�g�[�ɍ���Ă݂��B
	�S�R�Ⴄ�񂾂��ǁA�p�`�F���ۂ��Ƃ����΁A�p�`�F���ۂ��B
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_3f_pache_emerald_megalith)
{
	/* easy��1����, hard��1����, lunatic ��3���� */
	REG_0b_REG3 = (((3==REG_0f_GAME_DIFFICULTY)?3:1));
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�������ۂ����l�B
//	REG_09_REG1 	z
//	REG_0a_REG2 	z
	REG_0b_REG3 	��Փx�ʒ萔�B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_3f_pache_emerald_megalith)
{
	REG_08_REG0 += (REG_10_BOSS_SPELL_TIMER<<4);
	// �����_���Θg�e
	if (0== ((REG_10_BOSS_SPELL_TIMER)&(4-1) ))
	{
	//	REG_08_REG0 = HATSUDAN_03_angle65536;
		/* �Θg�e */
		HATSUDAN_03_angle65536			= ((REG_08_REG0&(32768-1))-16384);
		//
	//	HATSUDAN_01_speed256			= (t256(0.75) );		/* �e�� */
	//	HATSUDAN_01_speed256			= (t256(1.50) );		/* �e�� */
		HATSUDAN_01_speed256			= (t256(0.75)+(REG_08_REG0&0x007f)+((REG_0f_GAME_DIFFICULTY)<<4) ); /* �e�� */
		HATSUDAN_02_speed_offset		= ((REG_08_REG0>>4)&0x07ff);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_05_MIDORI);//		/* [�Θg�e] */
		HATSUDAN_06_n_way				= (2);	/* [2way] */								/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(24)); /* �����p�x(65536[360/360�x]�� 24 ����) */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	// �����_�����������A�Α�ʒe
	if (0== ((REG_10_BOSS_SPELL_TIMER)&(16-1) ))
	{
		const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
		REG_08_REG0 += ra_nd32;
		REG_02_DEST_X	= t256(16)+(ra_nd32&0xff00)+(ra_nd32&0x3f00);	/* �e��x256 */
		REG_03_DEST_Y	+= -t256(32)+((ra_nd32>>8)&0x3f00); 			/* �e��y256 */
		/* ���@�_�� 3way ��laser�e */
		/* ���@�_��Ȃ� 16way(?)17way(???) �ےe */
		HATSUDAN_01_speed256				= (t256(0.50) );			/* �e�� */
		HATSUDAN_02_speed_offset			= t256(8);/*(�e�X�g)*/
		HATSUDAN_03_angle65536				= (0);
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_OODAMA32_03_MIDORI);//				/* [�Α�ʒe] */
		/* easy��1����, hard��1����, lunatic ��3���� */
		if (3==REG_0f_GAME_DIFFICULTY)
		HATSUDAN_06_n_way					= (REG_0b_REG3);	/* [1-3way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536			= (int)(65536/(19));	/* �����p�x(65536[360/360�x]�� 19 ����) */
		hatudan_system_regist_n_way();/* (r33-) */
	}
	// ���ʉ�
	if (0== ((REG_10_BOSS_SPELL_TIMER)&(32-1) ))
	{
		cpu_bullet_play_15_auto();
	}
}
