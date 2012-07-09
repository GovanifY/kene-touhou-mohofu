
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	��d���̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�g2�ʒ�-�{�X ��d�� �ʏ�U��1(1/3)
	-------------------------------------------------------
�΃N�i�C�e
������n�߂āA���v���A2�i�Â�
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_1a_dai_yousei_midori)
{
//	if (64==(src->boss_spe ll_timer))
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
	}
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_tmp_angleCCW65536
	REG_0a_REG2 	bbb
	REG_0b_REG3 	i (��Փx�ʂɎc��񐔂�ݒ�B)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1a_dai_yousei_midori)
{
//	if ((64-(48))<((src->boss_spe ll_timer)))
	count_up_limit_NUM(REG_NUM_08_REG0, 64);
	if (16<(REG_08_REG0))
	{
	//	if (0x00==((src->boss_spe ll_timer)&(2-1)))/*(2���1��)*/
		{
				HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_05_MIDORI);	/* [ �e] */
		//	u32 i;
		//	for (i=0; i<(1+(unsigned)REG_0f_GAME_DIFFICULTY); i++)/*(2�i�Â�)*/
			REG_0b_REG3 = REG_0f_GAME_DIFFICULTY;
		loop:
			{
				HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2<<2)+(REG_0b_REG3<<6));			/* �e�� */
				HATSUDAN_03_angle65536			= (REG_09_REG1);
				hatudan_system_regist_single();
			}
			REG_0b_REG3--;/*(�c��񐔌��炷�B)*/
			if (0 < REG_0b_REG3)/*(�c��񐔂�����΃��[�v�B)*/
		//	if (0 != REG_0b_REG3)/*(�c��񐔂�����΃��[�v�B)*/
			{
				goto loop;
			}
			REG_09_REG1 -= (65536/48);
			REG_0a_REG2++;
		}
	}
}


/*---------------------------------------------------------
	�g2�ʒ�-�{�X ��d�� �ʏ�U��1(2/3)
	-------------------------------------------------------
�ԃN�i�C�e
������n�߂āA�����v���A2�i�Â�
����:Lunatic���ꉞ����48�����e�B(4��������o��2�i?3�i�ɐ���?)
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_1b_dai_yousei_aka)
{
//	if (64==(src->boss_spe ll_timer))
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
	}
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_tmp_angleCCW65536
	REG_0a_REG2 	bbb
	REG_0b_REG3 	i (��Փx�ʂɎc��񐔂�ݒ�B)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1b_dai_yousei_aka)
{
//	if ((64-(48))<((src->boss_spe ll_timer)))
	count_up_limit_NUM(REG_NUM_08_REG0, 64);
	if (16<(REG_08_REG0))
	{
	//	if (0x00==((src->boss_spe ll_timer)&(2-1)))/*(2���1��)*/
		{
				HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_01_AKA);	/* [ �e] */
		//	u32 i;
		//	for (i=0; i<(1+(unsigned)REG_0f_GAME_DIFFICULTY); i++)/*(2�i�Â�)*/
			REG_0b_REG3 = REG_0f_GAME_DIFFICULTY;
		loop:
			{
				HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2<<2)+(REG_0b_REG3<<6));			/* �e�� */
				HATSUDAN_03_angle65536			= (REG_09_REG1);
				hatudan_system_regist_single();
			}
			REG_0b_REG3--;/*(�c��񐔌��炷�B)*/
			if (0 < REG_0b_REG3)/*(�c��񐔂�����΃��[�v�B)*/
		//	if (0 != REG_0b_REG3)/*(�c��񐔂�����΃��[�v�B)*/
			{
				goto loop;
			}
			REG_09_REG1 += (65536/48);
			REG_0a_REG2++;
		}
	}
}

/*---------------------------------------------------------
	�g4�ʒ�-�{�X ������ �ʏ�U��1
	-------------------------------------------------------
	����ʒe�B8�����B1/24�������v���ɉ�]�B
	�N�i�C�e�B�o���}�L�B
	-------------------------------------------------------
	����ʒe�B�͖����̂ő���ɐ�ʒe�ŁB
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	�p�x1�����p�B
	REG_0a_REG2 	�����B
	REG_0b_REG3 	�������Z�p(�e�L�g�[)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1f_koakuma)
{
//	if (64==(REG_10_BOSS_SPELL_TIMER)) /* �n�߂ɏ����� */
	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	�J��Ԃ��J�E���^�B */
	if (1==REG_08_REG0) /* �n�߂ɏ����� */
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
		REG_0b_REG3 = 0;
		#if (1)
		AUDIO_18_voice_number	= VOICE17_BOSS_TAME01;	//	"�{�X����1",		/* r	�����[�� */
		AUDIO_19_voice_truck	= TRACK03_IVENT_DAN;
		cpu_voice_play();
		#endif
	}
	if (0== (REG_08_REG0 & 0x0f) ) /* 16���1��Ȃ猂�� */
	{
		HATSUDAN_01_speed256			= (t256(1.0));			/* �e�� */
	//	HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= t256(5);/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (REG_09_REG1);
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI); 	/* [��ʒe] ���������� */
		HATSUDAN_06_n_way				= (8);							/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(8)); 			/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
		hatudan_system_regist_katayori_n_way();
		REG_09_REG1 += ((65536/24)-256);	/* ����(1��/24)�𑫂��B(+-256�����𑫂�) */
		REG_09_REG1 += (REG_0b_REG3);		/* �������Z�p */
	}
	if (0== (REG_08_REG0 & 0x01) ) /* 2���1��Ȃ猂�� */
	{
		HATSUDAN_02_speed_offset		= t256(1.5);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_KUNAI12_BASE + TAMA_IRO_03_AOI);			/* [���Ēe] */
		HATSUDAN_01_speed256			= (t256(1.0));		/* �e�� */
		REG_0a_REG2 = ra_nd();
		REG_0b_REG3 = REG_0a_REG2 & 0x01ff;/*(0 ... 511)*/
		//
		HATSUDAN_03_angle65536			= (REG_0a_REG2);
		hatudan_system_regist_single();
	}
}
