
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���ʕ����B
	-------------------------------------------------------
	(r32-)��ݏo���`�F�b�N�ɂ��āA
	�u�ړ����Ɂv��ݏo���`�F�b�N�����Ȃ������O��Ő݌v����Ă���J�[�h�������B
	���ׁ̈u�ړ����Ɂv�V�X�e���ŋ�ݏo���`�F�b�N�����܂���B
	�Ⴆ�Ή�ʒ[�Œ��˕Ԃ�e�́u�ړ����Ɂv��ݏo���`�F�b�N�����Ȃ����ɂ��������Ă���@�\�ł��B
	�u�ړ��ƕʂŁv�S�e�z��𒲂ׂāu��ݏo���`�F�b�N�v���s���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�����e�͐F�X�e�ɂ���ĈႢ����������B(�X�̎��ԂƂ�)
	�S���V�X�e�����ŋz������ƁA�x���Ȃ�C������B
	�����e�V�X�e�����ŋz������Ȃ�A�p�x�e�Ɠ������Ċp�x�e�������ׂ�
	������(�����͂Ƃ�����)����ł͊p�x�e�ėp�ɂ���ƒx������B
	-------------------------------------------------------
	����̑��x�Ȃ�A�����e�͑S�e�z�񏑂������őΉ����ׂ��B
	(�p�`�F�Ƃ����������̗��ɂȂ肻���ł����B)
	�V�X�e���������e�Ή��́A�����Ȃ肻���B
---------------------------------------------------------*/

#if (1)
	/*(tama_system.c�̓���@�\)*/
	#define hatudan_system_kousadan_angle65536 tmp_angleCCW1024
#endif
/*---------------------------------------------------------
	#001 �p�x�e�̈ړ����s��(�ʏ�e�p)
---------------------------------------------------------*/

static OBJ_CALL_FUNC(hatudan_system_B_move_angle_001)/*(�p�x�e�ړ�)*/
{
	#if 1/* ������(�����Ă��o���邯�ǁA�����������ȒP) */
	src->hatudan_register_speed65536		+= (src->hatudan_register_tra65536);		/* ���������� */
	#endif
	/*( 1[frame]������ɐi�ދ����B(���a) )*/int aaa 		 = (src->hatudan_register_speed65536>>8);	/* ���x */
//
	#if (0)//
	src->center.x256 += ((si n1024((src->rotationCCW1024))*(aaa))>>8);/*fps_factor*/
	src->center.y256 += ((co s1024((src->rotationCCW1024))*(aaa))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		src->center.x256 += ((sin_value_t256*(aaa))>>8);/*fps_factor*/
		src->center.y256 += ((cos_value_t256*(aaa))>>8);/*fps_factor*/
	}
	#endif
}

static OBJ_CALL_FUNC(danmaku_00_standard_angle_mover)/*(�p�x�e�ړ�+��ʊO�e����)*/
{
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
	hatudan_system_B_gamen_gai_tama_kesu(src);/*(��ʊO�e����)*/
}
static OBJ_CALL_FUNC(danmaku_01_standard_angle_sayuu_hansya_mover)/*(�p�x�e�ړ�+��ʍ��E���˒e����)*/
{
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
	hatudan_system_B_side_hansya(src);/*(��ʍ��E���˒e����)*/
}
local OBJ_CALL_FUNC(angle_to_vector)/*(�p�x��X���AY���̃x�N�g�����x�֕ϊ�����)*/
{
	//	REG_0b_REG3 = ((src->rotationCCW1024)<<6);/*1024to65536*/
		REG_0b_REG3 = ((src->hatudan_system_kousadan_angle65536));/*(�����e�p�ɔ��e���̊p�x��65536�ł���̂Ŏg���B)*/
	//	REG_0b_REG3 = (src->rotationCCW1024);
//		REG_0b_REG3 = ((HATSUDAN_03_angle65536));
		#if (0)
		// �x�N�g���e�o�^�B regist vector.
		sincos65536_NUM(REG_NUM_0b_REG3, REG_NUM_0c_REG4);
		REG_02_DEST_X	 = (((REG_0c_REG4)*(REG_09_REG1))>>8);
		REG_03_DEST_Y	 = (((REG_0d_REG5)*(REG_09_REG1))>>8);
		//
		#else
		{
//			unsigned int angle1024;
//		//	angle1024 = HATSUDAN_03_angle65536;//spell_register[register_number_angle];
//			angle1024 = REG_0b_REG3;//spell_register[register_number_angle];
//			angle1024 = deg65536to1024(angle1024);	/*(65536������1024�����ɕύX)*/

			#if (0)//
			spell_register[register_number_value]		= (((si n1024((angle1024))) ) );		/* CCW�̏ꍇ */
			spell_register[register_number_value+1] 	= (((co s1024((angle1024))) ) );
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (REG_0b_REG3>>6), &sin_value_t256, &cos_value_t256);
				/*	spell_register[register_number_value]	*/	REG_0c_REG4 = (sin_value_t256);
				/*	spell_register[register_number_value+1] */	REG_0d_REG5 = (cos_value_t256);
				REG_02_DEST_X	= (((REG_0c_REG4)*(REG_09_REG1))>>8);
				REG_03_DEST_Y	= (((REG_0d_REG5)*(REG_09_REG1))>>8);
			}
			#endif
		}
		#endif
	//	src->rotationCCW1024	= (0);/*(�d���̂Ŋp�x�O�ɂ���)*/
		src->math_vector.x256 = (REG_02_DEST_X);/*fps_factor*/
		src->math_vector.y256 = (REG_03_DEST_Y);/*fps_factor*/
}


local OBJ_CALL_FUNC(move_vector_gamen_sita)
{
	// �x�N�g���e�ړ��Bmove vector.
	src->center.x256 += (src->math_vector.x256);	/*fps_factor*/
	src->center.y256 += (src->math_vector.y256);	/*fps_factor*/
//	src->center.x256 += 1;//(t256( 0.1));	/*fps_factor*/
//	src->center.y256 += 1;//(t256(-0.1));	/*fps_factor*/
//	src->math_vector.y256 += ((src->hatudan_register_tra65536)<<8);	/* ���������� */
	src->math_vector.y256 += ((src->hatudan_register_tra65536) ); /* ���������� */
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
//	( (src->center.y256) > (rect_clip.bullet_clip_max.y256) ) ) /*(��ʉ��̂�)*/
	( (src->center.y256) > (t256(272)) ) )	/*(��ʉ��̂�)*/
	{
		src->jyumyou = JYUMYOU_NASI;
	}
	#endif
}

/* HATUDAN_ITI_NO_JIKAN==���e�G�t�F�N�g�ォ��[0]�J�E���g�o�߂����e�B���e�G�t�F�N�g����==[64]�J�E���g�B */
#define HATUDAN_ITI_NO_JIKAN	(JYUMYOU_1MIN-64)

/*---------------------------------------------------------
	�J�~��e
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	[�萔1]�J�̑��x
//	REG_0a_REG2 	[�萔2]�ԐN�i�C���Ȃ���p�x(180/360���傢��])
	-------------------------------------------------------
	sincos�v�Z�p
	REG_0b_REG3 	angle_65536
	REG_0c_REG4 	sine   �̒l�p�B
	REG_0d_REG5 	cosine �̒l�p�B
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(common_danmaku_01_amefuri_callback)
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
	}
	#endif
	move_vector_gamen_sita(src);/*(�x�N�g���ړ�+��ʉ�����)*/
}
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(meirin_danmaku_02_aka_ao_kunai_callback);
local OBJ_CALL_FUNC(meirin_danmaku_03_aka_ao_kunai_time256_callback);
local OBJ_CALL_FUNC(spell_init_12_common_amefuri)
{
	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[�萔1]�J�̑��x
	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[�萔2]�ԐN�i�C���Ȃ���p�x
	card.danmaku_callback[1] = common_danmaku_01_amefuri_callback;/*(�J�p)*/
	card.danmaku_callback[2] = meirin_danmaku_02_aka_ao_kunai_callback;/*(�ԐN�i�C�p1)*/
	card.danmaku_callback[3] = meirin_danmaku_03_aka_ao_kunai_time256_callback;/*(�ԐN�i�C�p2)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_common_amefuri)
{
	#if 0
	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))	// 4�J�E���g��1����8way�e������
	{
			HATSUDAN_01_speed256				= (REG_09_REG1);
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* [���Ēe] */	/* �e�O�� */
	//		HATSUDAN_06_n_way					= (8);
//			HATSUDAN_07_div_angle65536			= (int)(65536/64);
		unsigned int j;
		for (j=(0); j<(8); j++)
		{
			/* �V���b�g */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			HATSUDAN_03_angle65536		= (65536/4)+(65536/8)+(j<<(5+6))+((REG_10_BOSS_SPELL_TIMER>>(2-6))&((0x3f)<<6));
			HATSUDAN_07_VECTOR_leg acy_dan_delta256 = ((ra_nd()&0x03)+2);//t256(0.04)
			bul let_reg ist_leg acy_vec tor_dir ect();
		}
	}
	#endif
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))	// 4�J�E���g��1����8way�e������
		{
				HATSUDAN_01_speed256				= REG_09_REG1;// [�萔1]�J�̑��x
				#if 0
					/*�Â�*/
				#else
					/*�V����*/
//				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|/*(TAMA_SPEC_4000_NON_MOVE)|*/(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
//				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
				#endif
				HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_00_SIRO);	/* �e�O�� */	/* [���Ēe] */
			unsigned int jj;
			for (jj=(0); jj<(8); jj++)
			{
				#if 0
				/* �V���b�g */
			//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
				HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(5+6))+((REG_10_BOSS_SPELL_TIMER>>(2-6))&((0x3f)<<6));
	//			HATSUDAN_06_n_way					= (8);
//				HATSUDAN_07_div_angle65536					= (int)(65536/64);
				HATSUDAN_07_VECTOR_leg acy_dan_delta256 	= ((ra_nd()&0x03)+2);//t256(0.04)
				bul let_reg ist_leg acy_vec tor_dir ect();
				#else
				/* �V���b�g */
			//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
				HATSUDAN_02_speed_offset			= ((ra_nd()&0x03)+2);//t256(0.04)
			//	HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(5+6))+((REG_10_BOSS_SPELL_TIMER<<(6-2))&(0x3f<<6));
	//			int aaa_1024;
	//							aaa_1024				= (1024/4)+(1024/8)+(jj<<5)+((REG_10_BOSS_SPELL_TIMER>>2)&0x3f);
	//			HATSUDAN_03_angle65536				= ((aaa_1024)<<6);

			//	HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11));//+((REG_10_BOSS_SPELL_TIMER<<(6-2))&(0x0fff));
		//		HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11))+((REG_10_BOSS_SPELL_TIMER<<(4))&(0x0fff));
			//	HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11))+((REG_10_BOSS_SPELL_TIMER<<(5))&(0x0fff));
				HATSUDAN_03_angle65536				= (65536/4)+(65536/8)+(jj<<(11))+((REG_10_BOSS_SPELL_TIMER<<(4))&(0x0fff));

	//			HATSUDAN_06_n_way					= (8);
//				HATSUDAN_07_div_angle65536			= (int)(65536/64);
				hatudan_system_regist_single();
				#endif
			}
		}
}

/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
	�Ƃ肠�������ʋK�i
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_mima_kaguya)
{
	REG_09_REG1 	= (t256(1.0));//[�萔1]�J�̑��x
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[�萔2]�ԐN�i�C���Ȃ���p�x
	card.danmaku_callback[1] = common_danmaku_01_amefuri_callback;/*(�J�p)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}

/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_0e_REG6 	src_shot_angle65536 �J�n�n�_
---------------------------------------------------------*/
global/*local*/ OBJ_CALL_FUNC(shot_common_gin_tama)
{
	#if (1)/*(���ʕ���)*/
	HATSUDAN_01_speed256					= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//	HATSUDAN_02_speed_offset				= ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
	HATSUDAN_02_speed_offset				= ((ra_nd()&0x03)+2);//t256(0.04)
//	HATSUDAN_03_angle65536					= (src->shot_angle65536&(256*64-1))+512*64+128*64;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
//	HATSUDAN_03_angle65536					= (src->shot_angle65536&(256*64-1))+(65536/2)+(65536/8);// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_03_angle65536					= (REG_0e_REG6&((65536/4)-1))+(65536/4)+(65536/8);// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_01)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	HATSUDAN_05_bullet_obj_type 			= (BULLET_MINI8_BASE + TAMA_IRO_03_AOI);	/* �e�O�� */
	hatudan_system_regist_single();
	#endif
}
	#if (0)/*(��)*/
	HATSUDAN_01_speed256					= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//	HATSUDAN_03_angle65536					= (((src->shot_angle65536&(256*64-1))+512*64+128*64));// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_03_angle65536					= (((REG_0e_REG6&(256-1))-512-128)<<6);// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
	HATSUDAN_05_bullet_obj_type 			= (BULLET_MINI8_BASE + TAMA_IRO_03_AOI);	/* �e�O�� */
//	HATSUDAN_06_n_way						= (8);
//	HATSUDAN_07_VECTOR_leg acy_dan_delta256 = ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
	HATSUDAN_07_VECTOR_leg acy_dan_delta256 = ((ra_nd()&0x03)+2);//t256(0.04)
	bullet_regist_leg acy_vector_direct();
	#endif

#if (1)/*(�K�v)*/
/*---------------------------------------------------------
	[r34�݊��@�\]�B
	-------------------------------------------------------
	�����͉����������s���܂��񂪁Ar35���݁A�K�v�ł��B
	-------------------------------------------------------
	(r35����)���ƍ��ł��̋@�\(r34�݊��@�\)���g�p���Ă���ׂɕK�v�B
	������������r35�̎d�l��A�ʏ�e�����ĂȂ��B(�ʏ�e�̏���������ɍs���Ȃ�)
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_48_r34_gokan_kinou)
{
	/* (�����͉����������s���܂��񂪁Ar35���݁A�K�v�ł�) */
	// (r35-)�V�X�e���ڍs�����̂ŁA(r35-)�V�X�e���̓s����A(-r34�܂�)�̃X�y�J���Č�����ׂɕK�v�ȃ_�~�[�B
};
#endif

