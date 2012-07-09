
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����l�̌����ɂ���āA���e�ʒu���C������B
	-------------------------------------------------------
	���e�ʒu�͖����l���S�ʒu����̍����Ȃ̂ŁA
	���12[pixel]
	�E��22[pixel]
	����22[pixel]
	�Ƃ���B
	�p�^�[��0-3�̏ꍇ�A�E����(+22)�B
	�p�^�[��4-7�̏ꍇ�A������(-22)�B

    ����22[pixel]    �E��22[pixel]
    +--------- 50 --------+
    |                     |
    | ()               () | -- ---(���e�ʒu)
    |  |  -22  |  22   |  |
    |          |          | -12 (���12[pixel])
    |          |          |
    50         + -------  | -- ---(�����l���S�ʒu)
    |                     |
    |                     |
    |                     |
    |                     |
    |                     |
    +--------- 50 --------+
    �����l�� 50x50[pixel]�Ƃ���B
---------------------------------------------------------*/

local OBJ_CALL_FUNC(mima_set_REG_DEST_XY)
{
	/* �e�� ���e �ʒu���C�� */
	REG_02_DEST_X += ((0==(src->obj_type_set&4))?(t256(22)):-(t256(22)));/* �E�ړ����͉E�����猂�� */
	REG_03_DEST_Y -= t256(12);
}


/*---------------------------------------------------------
	�����A�ԂĂ��ƃJ�[�h
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	adj_hari65536	�J�n�n�_
	REG_0a_REG2 	x�ʒu/�ޔ�/����
	REG_0b_REG3 	y�ʒu/�ޔ�/����
---------------------------------------------------------*/
/*---------------------------------------------------------
	#001 �p�x�e�̈ړ����s��(�ʏ�e�p)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(mima_danmaku_01_callback)
{
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* ���e�G�t�F�N�g���͖��� */
	{
		return;
	}
	#endif
	#if (1)
	if ((HATUDAN_ITI_NO_JIKAN-512) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��512�J�E���g�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	}
	#endif
	//
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
local OBJ_CALL_FUNC(mima_boss01_nway_fire)
{
	REG_02_DEST_X	= ((src->center.x256));
	REG_03_DEST_Y	= ((src->center.y256));
	calculate_jikinerai();/* ���@�_���p�쐬 */
	REG_09_REG1 = (HATSUDAN_03_angle65536);
//
	const int speed256 = (/*0x200*/0x100)+(ra_nd()&0xff)-((3-(REG_0f_GAME_DIFFICULTY))<<6);/* ��Փx�ʂő��x�ቺ */
	mima_set_REG_DEST_XY(src);	/* �����Ō��ʒu��ς���B */
	#if (1)
	//------------------
	// �ޔ�
	REG_0a_REG2 = REG_02_DEST_X;
	REG_0b_REG3 = REG_03_DEST_Y;
	//------------------
	#endif
//
//	HATSUDAN_02_speed_offset			= t256(1/*0*/);/*(�e�X�g)*/
	HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
	HATSUDAN_05_bullet_obj_type 		= BULLET_HARI32_00_AOI + ((REG_10_BOSS_SPELL_TIMER)>>5);	/* [���F�j�e] */	/*hari_no_iro*/
//
	int ii65536;
	for (ii65536=(REG_09_REG1/*0*/); ii65536<(REG_09_REG1+65536-(20*64)); ii65536+=(65536/5) )
	{
		int jj65536;
		int kk65536;
		kk65536 = (0);
		for (jj65536 = (0); jj65536<(65536/2); jj65536 += deg1024to65536(32) )
		{
			#if (0)//
			{
				int sin_value_t256; 		// sin_value_t256 = 0;
				int cos_value_t256; 		// cos_value_t256 = 0;
				int256_si ncos1024( ((deg65536to1024(jj65536) )), &sin_value_t256, &cos_value_t256);
				HATSUDAN_01_speed256		= (int)(speed256 + ((sin_value_t256))); 	/* �e�� */
			}
			#else
			//------------------
			HATSUDAN_01_speed256	= t256(1.00);
			HATSUDAN_03_angle65536	= (((jj65536)));
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
			HATSUDAN_01_speed256 = (int)(speed256 + ((REG_03_DEST_Y))); 	/* �e�� */
		//	h->center.y256 = (REG_02_DEST_X);/*fps_factor*/
			//------------------
			// ����
			REG_02_DEST_X = REG_0a_REG2;
			REG_03_DEST_Y = REG_0b_REG3;
			//------------------
			#endif
			#if 0
		//	HATSUDAN_02_speed_offset	= -t256(HATSUDAN_01_speed256>>(6));/* (�����llunatic��)����ł��ǂ��C�����邯�ǁB */
			HATSUDAN_02_speed_offset	= -(HATSUDAN_01_speed256<<(8-6));/* (�����llunatic��)����ł��ǂ��C�����邯�ǁB */
			#endif
		//	HATSUDAN_02_speed_offset	= -t256(HATSUDAN_01_speed256>>(7));/* (�p�`�F�����邩��)�����ȒP�ɂ��Ƃ��B */
			HATSUDAN_02_speed_offset	= -(HATSUDAN_01_speed256<<(8-7));/* (�p�`�F�����邩��)�����ȒP�ɂ��Ƃ��B */
			HATSUDAN_03_angle65536		= ((ii65536)+(kk65536));				/* �p�x */
			hatudan_system_regist_single();
			kk65536 += ((65536/16)/5);
		}
	}
}

/*---------------------------------------------------------
	�����A�ԂĂ��ƃJ�[�h
	-------------------------------------------------------
	�����\�ł�4�����o���}�L���ۂ��̂������Ă��邪�A
	�A�����W����5�����B�ԉf�˂��ۂ��z�ɁB
	-------------------------------------------------------
	�e�L�g�[
-dd 1 0000 0x10 == (xxx)&0x1f
-11 1 0000 luna
-10 1 0000 luna hard
-01 1 0000 luna hard norm
-00 1 0000 luna hard norm easy
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_0c_hana_test)
{
	card.danmaku_callback[1] = mima_danmaku_01_callback;/*(�B)*/
//	card.danmaku_callback[2] = NULL;/*(0)*/
//	card.danmaku_callback[3] = NULL;/*(0)*/
}

local OBJ_CALL_FUNC(spell_create_0c_hana_test)
{
//	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xcf))/* 4�� */
//	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (16���1��)(128�Ȃ�v4��) */
	{
		if (((unsigned)REG_0f_GAME_DIFFICULTY) >= (((unsigned)REG_10_BOSS_SPELL_TIMER)>>5))
		{
			mima_boss01_nway_fire(src);
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/* �e�L�g�[ */
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
	}
}

/*---------------------------------------------------------
	�����A�ڋ߃J�[�h
	-------------------------------------------------------
	�����ŉ������˂�4�����A�d�͒e���ۂ��z(�\��)
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_0e_REG6 	src_shot_angle65536 �J�n�n�_
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_0d_mima_sekkin)
{
//	if (0x40==((REG_10_BOSS_SPELL_TIMER)&0xcf))/* 4�� */
//	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (32���1��)(128�Ȃ�v ��) */
	if (0x04==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (8���1��) */
	{
	//	if (difficulty >= ((REG_10_BOSS_SPELL_TIMER)>>5))
		{
		//	REG_0e_REG6 -= (16);		/* cv1024r(10)*/
		//	REG_0e_REG6 -= (16*64); 		/* cv1024r(10)*/
			REG_0e_REG6 -= (1820);		/* cv1024r(10)*/	// 1820.44444444444444444444444444444 == 65536/36
		//	REG_0e_REG6 = ((src->shot_angle65536));/*(1024to65536)*/
		// (�����l��p�i���g���B�W���̂ł̓_��) set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			mima_set_REG_DEST_XY(src);	/* �����Ō��ʒu��ς���B */
			shot_common_gin_tama(src);/*(���ʕ���)*/
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/* �e�L�g�[ */
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
	}
}

#if 0
/*---------------------------------------------------------
	�����A�g�Q�J�[�h(�H �×t�A�t���u�����̗��t�v���ۂ��H)
	-------------------------------------------------------
	�e�L�g�[
	[0] f e d
	[c] b a 9
	[8] 7 6 5
	[4] 3 2 1
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	kaiten_aaa
	REG_0a_REG2 	kaiten_bbb
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_24_mima_toge)
{
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		REG_09_REG1 -= (10);
		HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_HARI32_00_AOI);				/* [���F�j�e] */
		//
		if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			HATSUDAN_01_speed256		= (t256(2.0) ); 	/* �e�� */
			HATSUDAN_06_n_way			= (1);									/* [1way] */
			HATSUDAN_07_div_angle65536	= (int)(65536/24);						/* �����p�x */
		}
		else
		if (0x0c==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			HATSUDAN_01_speed256		= (t256(1.75) );	/* �e�� */
			HATSUDAN_06_n_way			= (2);									/* [2way] */
			HATSUDAN_07_div_angle65536	= (int)(65536/24);						/* �����p�x */
		}
		else
		if (0x08==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			HATSUDAN_01_speed256		= (t256(1.5) ); 	/* �e�� */
			HATSUDAN_06_n_way				= (2);									/* [2way] */
			HATSUDAN_07_div_angle65536		= (int)(65536/12);						/* �����p�x */
		//	REG_0a_REG2 -= ((int)(65536/48)/*+(int)(65536/24)*/);
			REG_0a_REG2 -= ((int)(65536/24));
		}
		//
	//	REG_0a_REG2 -= ((int)(65536/48)/*+(int)(65536/24)*/);
		REG_0a_REG2 -= ((int)(65536/48));
		mask65536(REG_0a_REG2);
		HATSUDAN_03_angle65536				= (((REG_09_REG1+REG_0a_REG2))&(65536-1));	/* �p�x */
		//
		if (0x04!=((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
	}
	if ((SPELL_TIME_0640-1)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE17_BOSS_TAME01);
		#endif
	}
}
#endif

#if 1
/*---------------------------------------------------------
	�����A�g�Q�J�[�h(�H �×t�A�t���u�����̗��t�v���ۂ��H)
	-------------------------------------------------------
	�e�L�g�[
	f e d [c] 1100
	b a 9 [8] 1000
	7 6 5 [4] 0100
	3 2 1 [0] 0000
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	kaiten_aaa
	REG_0a_REG2 	x�ʒu/�ޔ�/����
	REG_0b_REG3 	y�ʒu/�ޔ�/����
	REG_0c_REG4 	kakudo_111
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_24_mima_toge)
{
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x33))
	{
		#if (1)
		//------------------
		// �ޔ�
		REG_0a_REG2 = REG_02_DEST_X;
		REG_0b_REG3 = REG_03_DEST_Y;
		//------------------
		#endif
		if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x3f))
		{	/* ������ƃA���X���� */
			REG_0c_REG4 += deg1024to65536(7);
			#if 0
			{
				int sin_value_t256; 		// sin_value_t256 = 0;
				int cos_value_t256; 		// cos_value_t256 = 0;
				int256_si ncos1024( ((deg65536to1024(REG_0c_REG4)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
				REG_09_REG1 -= (( (sin_value_t256))<<6);
			}
			#else
			//------------------
			HATSUDAN_01_speed256	= t256(64.00);
			HATSUDAN_03_angle65536	= (((REG_0c_REG4)));
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
			REG_09_REG1 -= ( ((REG_03_DEST_Y))); 	/* �e�� */
		//	h->center.y256 = (REG_02_DEST_X);/*fps_factor*/
			//------------------
			#endif
			#if (1)
			AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;
			AUDIO_19_voice_truck	= TRACK04_TEKIDAN;
			cpu_voice_play();
		//	bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
			#endif
		}
		//------------------
		// ����
		REG_02_DEST_X = REG_0a_REG2;
		REG_03_DEST_Y = REG_0b_REG3;
		//------------------
		HATSUDAN_01_speed256				= (t256(1.5));		/* �e�� */
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 		= (BULLET_HARI32_00_AOI);				/* [���F�j�e] */
		HATSUDAN_06_n_way					= (5);									/* [5way] */
		HATSUDAN_07_div_angle65536			= (int)(65536/5);						/* �����p�x */
		//
		unsigned int aaa = ((REG_10_BOSS_SPELL_TIMER)&0x0f);
		unsigned int bbb = ((0x0c-aaa)<<8);
		HATSUDAN_02_speed_offset			= (t256(1)+(aaa<<8));/*(�e�X�g)*/
		//
		HATSUDAN_03_angle65536				= (((REG_09_REG1+bbb))&(65536-1));		/* �p�x */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		HATSUDAN_03_angle65536				= (((REG_09_REG1-bbb))&(65536-1));		/* �p�x */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}
#endif
