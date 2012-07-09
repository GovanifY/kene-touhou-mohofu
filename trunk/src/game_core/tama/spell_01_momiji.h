
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�̂̂����ۂ��J�[�h�𐶐����悤�Ǝv�������ǖ����ł����B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�g2�ʒ�-�{�X ��d�� �ʏ�U��1(1/3)
	-------------------------------------------------------
	�΃N�i�C�e
	������n�߂āA���v���A2�i�Â�
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	�p�x�����p�B
	REG_0a_REG2 	���x�����p�B
	REG_0b_REG3 	���Z�p(�e�L�g�[)
	REG_0c_REG4 	���Z�p�����l(seed)��ێ��B(�e�L�g�[)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_1e_momiji_no_nonoji)
{
//	if (64==(REG_10_BOSS_SPELL_TIMER)) /* �n�߂ɏ����� */
	count_up_limit_NUM(REG_NUM_08_REG0, 192);// 	/*	�J��Ԃ��J�E���^�B */
	if (1==REG_08_REG0) /* �n�߂ɏ����� */
	{
		REG_09_REG1 = 0;
		REG_0a_REG2 = 0;
	}
//	if ((64-(48))<((REG_10_BOSS_SPELL_TIMER)))
	if (48 > REG_08_REG0) /* 48�ȉ��Ȃ猂�� */
	{
	//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&(2-1)))/*(2���1��)*/
		{
				HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_00_AOI); 	/* [��ʒe] �����̖������� */
			u32 i;
			for (i=0; i<(1+(unsigned)REG_0f_GAME_DIFFICULTY); i++)/*(2�i�Â�)*/
			{
			//	HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2<<2)+(i<<6));			/* �e�� */
				HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2	)+(i<<6));			/* �e�� */
				HATSUDAN_03_angle65536			= (REG_09_REG1);
				hatudan_system_regist_single();
			}
			REG_09_REG1 += (65536/48);	/* �����猂���n�߂�B */
			REG_0a_REG2++;
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
	if (192 > REG_08_REG0) /* 192�ȉ��Ȃ猂�� */
	{
		if (96 < REG_08_REG0) /* 96�ȏ�Ȃ猂�� */
		{
			HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_00_SIRO);			/* [���Ēe] */
			HATSUDAN_01_speed256			= (t256(0.6)+(REG_0a_REG2));		/* �e�� */
			HATSUDAN_03_angle65536			= (REG_09_REG1);
			REG_0c_REG4 += REG_0b_REG3; /* �e�L�g�[? */
			REG_0b_REG3 = REG_0c_REG4;
			REG_0b_REG3 = (REG_0b_REG3 >> (5)); 	/*( /=32 ���������J�[�h���߂��K�v�B)*/
		//	count_up_limit_NUM(REG_NUM_0b_REG3, 1365);//	/* 1365.333333 == (65536/48) �ő���Z�p�x�������Ă݂�B */
			REG_09_REG1 -= 1365; /* (192-96)/48 ==2 �łQ����锤�B�e�L�g�[? */
			REG_09_REG1 -= REG_0b_REG3; /* �e�L�g�[? */
			hatudan_system_regist_single();
		}
	}
}
