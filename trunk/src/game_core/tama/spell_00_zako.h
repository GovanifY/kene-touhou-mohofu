
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�G���̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�A���X(��)	����@�J�[�h
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_09_zako_sentakki)
{
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	{
		src->tmp_angleCCW1024 -= (int)(1024/24);
		mask1024(src->tmp_angleCCW1024);
	//
		#if 0
			HATSUDAN_06_n_way					= (8);										/* [8way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/24);							/* �����p�x */
	//
			HATSUDAN_01_speed256				= (t256(2.0)+((REG_10_BOSS_SPELL_TIMER)<<2)); /* �e�� */
		{
			HATSUDAN_03_angle65536				= ((src->tmp_angleCCW1024)<<6); 				/* �p�x */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI);					/* [�ؒe] */
			bullet_regist_multi_vector_direct();
		//
			HATSUDAN_03_angle65536				= ((-((src->tmp_angleCCW1024)<<6))&(65536-1));		/* �p�x */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_UROKO14_BASE + TAMA_IRO_04_MIZU_IRO); 			/* [���ؒe] */
			bullet_regist_multi_vector_direct();
		}
		#endif
		#if 1
			HATSUDAN_02_speed_offset			= t256(1);		/* ���������e */	/* ���̕����ɂȂ邩������ */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_06_n_way					= (8);										/* [8way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/24);							/* �����p�x */
	//
			HATSUDAN_01_speed256				= (t256(1.0)+((REG_10_BOSS_SPELL_TIMER)<<2)); /* �e�� */
		{
			int first_angle65536 = ((src->tmp_angleCCW1024)<<6);
			HATSUDAN_03_angle65536				= (first_angle65536);						/* �p�x */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI);					/* [�ؒe] */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		//
			HATSUDAN_03_angle65536				= ((-first_angle65536)&(65536-1));			/* �p�x */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_UROKO14_BASE + TAMA_IRO_04_MIZU_IRO); 			/* [���ؒe] */
			hatudan_system_regist_katayori_n_way();/* (r33-) */
		}
		#endif
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}
