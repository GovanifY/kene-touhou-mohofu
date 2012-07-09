
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���~���A ���ۂ��J�[�h���`���܂��B
---------------------------------------------------------*/
/*---------------------------------------------------------
	�V���b�g���e�T�u�B
	-------------------------------------------------------
	C����̏ꍇ�͋��ʃT�u�B
	�X�N���v�g�������ꍇ�́A�R�s�y����B
	-------------------------------------------------------
	�g�p���W�X�^
	src->tmp_angleCCW65536;	���n���A�_����p�x
---------------------------------------------------------*/
local OBJ_CALL_FUNC(remilia_shot00)
{
	// ��  [��ʒe]
//	HATSUDAN_01_speed256			= (t256(3.5));						/* �e�� */
//	HATSUDAN_01_speed256			= (t256(2.25)); 					/* �e�� */
	HATSUDAN_01_speed256			= (t256(8.00)); 					/* �e�� */
//	HATSUDAN_02_speed_offset		= -t256(1);/* �����e�H */
	HATSUDAN_02_speed_offset		= -t256(4);/* �����e�H */
	HATSUDAN_03_angle65536			= src->tmp_angleCCW65536;/* ���n���A�_����p�x */
	HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_1000_EFFECT_MIDDLE)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	HATSUDAN_05_bullet_obj_type 	= BULLET_OODAMA32_01_AKA;			/* [�ԑ�ʒe] */
	hatudan_system_regist_single();
		// ��  [���ʒe]
	//	HATSUDAN_01_speed256			= (t256(6.00)); 					/* �e�� */
		HATSUDAN_02_speed_offset		= -t256(2);/* �����e�H */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE + TAMA_IRO_01_AKA);			/* [�ԑ咆�e] */
	{
		u32 rrr;
		rrr = (ra_nd());
		u32 ii;
		for (ii=0; ii<(8*3); ii++)
		{
			rrr += 13579;
			rrr &= 0xffffffffu;
			HATSUDAN_01_speed256		= (t256(5.00)+(rrr&0x1ff)); 					/* �e�� */
			HATSUDAN_03_angle65536		= src->tmp_angleCCW65536 + ((rrr) & ((65536/8)-1)) - ((65536/16)-1);/* ���@�_���p */
			hatudan_system_regist_single();
		}
		// ��  [���ʒe]
	//	HATSUDAN_01_speed256			= (t256(4.00)); 					/* �e�� */
		HATSUDAN_02_speed_offset		= -t256(1);/* �����e�H */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);				/* [�ԑ咆�e] */
		for (ii=0; ii<(16*2); ii++)/*(16)*/
		{
			rrr += 1234567;
			rrr &= 0xffffffffu;
			HATSUDAN_01_speed256		= (t256(2.00)+(rrr&0x1ff)); 					/* �e�� */
			HATSUDAN_03_angle65536		= src->tmp_angleCCW65536 + ((rrr) & ((65536/4)-1)) - ((65536/8)-1);/* ���@�_���p */
			hatudan_system_regist_single();
		}
	}
	#if (1)
	cpu_bullet_play_15_auto();
	#endif
}

/*---------------------------------------------------------
	[�������Z�N�V����]	���~���A �ʏ�U��2 ���ۂ�����
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B��{�I��24��Ɉ�񏈗��B
	REG_09_REG1 	���񐔐����B
	REG_0a_REG2 	���Z�p�x�B
	REG_0b_REG3 	�e�p�x(��ʒu��ێ�)�B
---------------------------------------------------------*/
//local OBJ_CALL_FUNC(spell_init_1f_remilia_kougeki2)
//{
//}
/*---------------------------------------------------------
	���~���A �ʏ�U��2 ���ۂ�����
	-------------------------------------------------------
	1�b��������A3�b�x��(�ړ�)�B

	REG_0f	��Փx
	REG_10	�{�X�^�C�}�[
---- ---- ---- ----
0000 0011 1111 1111
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_1f_remilia_kougeki2)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 24);// ��{�I��24��Ɉ�񏈗��B(����)
	if (0==(REG_08_REG0))
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 16);// 16, �ő�10(10.666==256/24)�Ȃ̂�16�̓_�~�[�l�B
		if (1==(REG_09_REG1))	/* �n�߂Ă̏ꍇ */
		{
			calculate_jikinerai();/* ���@�_���p�쐬 */
			src->tmp_angleCCW65536		= HATSUDAN_03_angle65536;/* ���@�_���p */
			REG_0a_REG2 	= (int)( ((65536/12)));/*(table3[]���Z�p�x��ݒ�)*/
			if ((src->center.x256)&0x100)
			{
				REG_0a_REG2 	= -REG_0a_REG2;/*(table3[]���Z�p�x��ݒ�)*/
			}
			src->tmp_angleCCW65536		-= REG_0a_REG2;/*(table3[]���Z�p�x���ړ�)*/
			src->tmp_angleCCW65536		-= REG_0a_REG2;/*(table3[]���Z�p�x���ړ�)*/
			src->tmp_angleCCW65536		-= REG_0a_REG2;/*(table3[]���Z�p�x���ړ�)*/
			src->tmp_angleCCW65536		-= REG_0a_REG2;/*(table3[]���Z�p�x���ړ�)*/
			REG_0b_REG3 = src->tmp_angleCCW65536;/*(�n�߂̊p�x��ݒ�)*/
		}
		if (0<(6-1)+(REG_0f_GAME_DIFFICULTY)-(REG_09_REG1))
		{
			// easy 	�� 6�� 3�� 0000
			// normal	�� 7�� 5�� 0001
			// hard 	�� 8�� 7�� 0010
			// lunatic	�� 9�� 9�� 0011
			/*(�ێ��p�x���ړ�)*/
			{
				REG_0b_REG3 	+= REG_0a_REG2;/*(table2[]�ێ��p�x���ړ�)*/
				/*(��p�x�ɕێ��p�x��ݒ�)*/
				src->tmp_angleCCW65536		= REG_0b_REG3;/*(��p�x���w��)*/
			}
			remilia_shot00(src);
		}
	}
}

#if 0
	if (0x80==((REG_10_BOSS_SPELL_TIMER)&0x80))/* �㔼 */
	{
	//	if (0xc0==((REG_10_BOSS_SPELL_TIMER)&0xc0))
		if (0xc0==((REG_10_BOSS_SPELL_TIMER)&0xc7))/* �W��Ɉ�� */
		{
			#if 0
			/* ���^�[�� 8�e */
					if (0x0000==((REG_10_BOSS_SPELL_TIMER)&0x30))	{	remilia_shot00(src);	}	/* ���@�_������ */
			else	if (0x0100==((REG_10_BOSS_SPELL_TIMER)&0x30))	{	remilia_shot00(src);	}	/* CW ���� */
			else	if (0x0200==((REG_10_BOSS_SPELL_TIMER)&0x30))	{	remilia_shot00(src);	}	/* CCW ���� */
			else/*	if (0x0300==((REG_10_BOSS_SPELL_TIMER)&0x30)) */{	remilia_shot00(src);	}	/* ���@�_������ */	/* ��C���� */
			#endif
		}
	}
#endif


/*---------------------------------------------------------

��e�����e�����e����
�����u�g�F�̖��E�v
128way�����󍂑��i�C�t�e
�Ԑj�e�΂�T�����XWAY�ԑ�e�A�ˁ��S����12way���e�A�ˁi���[�v�j
�g���u�X�J�[���b�g�V���[�g�v
---------------------------------------------------------*/


/*---------------------------------------------------------
 	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B(8���1�񔭒e����)
	REG_0a_REG2 	�ێ��p�x�B

	-------------------------------------------------------
128way�����󍂑��i�C�t�e
	�g�U�^�S���ʃi�C�t�e
	80�����e == (8way[hi to 0]+8way[slow to 0]) x 5way(5����)
	-------------------------------------------------------

---------------------------------------------------------*/
//local OBJ_CALL_FUNC(spell_create_07_80way_dual_five)
local OBJ_CALL_FUNC(spell_create_4d_remilia_rasen_knife)/* ���łȂ��ė����ł��B */
{
//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x01))
	{
				HATSUDAN_02_speed_offset		= t256(1/*0*/);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI+1;				/* [aka�i�C�t�e] */
				HATSUDAN_03_angle65536			= REG_0a_REG2;			/* �p�x */
		{	int ii; 	/* 1����65536���������p�x */
			for (ii=0; ii<(4); ii += (1) ) /* [4����] */	/* �p�x(65536[90/360�x]�� 4 ����) */
			{
				HATSUDAN_03_angle65536 += (65536/4);			/* �p�x */
				mask65536(HATSUDAN_03_angle65536);
				//
				HATSUDAN_01_speed256			= (t256(2.00) ); 	/* ���e t256(3.00) */
				hatudan_system_regist_single();/* (r33-) */
				//
				HATSUDAN_01_speed256			= (t256(2.40) ); 	/* �����e t256(3.14) */
				hatudan_system_regist_single();/* (r33-) */
			}
		}
				HATSUDAN_03_angle65536 -= (65536/128);					/* ���̊p�x */
				REG_0a_REG2 		= HATSUDAN_03_angle65536;			/* �p�x��ێ� */
	}
		#if (0)
		cpu_bullet_play_15_auto();
		#endif
}


/*---------------------------------------------------------
	���f�u�u���h�E�c�F�y�V���̎􂢁v
	-------------------------------------------------------
	�u���b�h�}�W�b�N�v
	-------------------------------------------------------
	��u�����ʃT�u
	-------------------------------------------------------
	C����̏ꍇ�͋��ʃT�u�B
	�X�N���v�g�������ꍇ�́A�R�s�y����B
---------------------------------------------------------*/
#define TAMA_DATA_10_TAMA_OKI_KANKAKU	src->hatudan_register_user_data_a01
local OBJ_CALL_FUNC(remilia_common_tama_oki_sub)
{
		src->hatudan_register_speed65536	+= (1<<8);			/* �e�� */
	//
	//	if (0==(src->jyumyou&0x0f))/*16���1��e��u��*/
	//	if (0==(src->jyumyou&0x07))/*8���1��e��u��*/
		TAMA_DATA_10_TAMA_OKI_KANKAKU--;
		if (0 > TAMA_DATA_10_TAMA_OKI_KANKAKU)
		{
			TAMA_DATA_10_TAMA_OKI_KANKAKU = REG_09_REG1;//[�萔1]n��Ɉ��e��u���B
			//
		//	REG_02_DEST_X	= (src->center.x256); 	/* �e��x256 �{�X���S���甭�e�B */
		//	REG_03_DEST_Y	= (src->center.y256); 	/* �e��y256 �{�X���S���甭�e�B */
			set_REG_DEST_XY(src);
			REG_00_SRC_X	= (REG_0b_REG3);
			REG_01_SRC_Y	= (REG_0c_REG4);
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
		}
}

/*---------------------------------------------------------
	���f�u�u���h�E�c�F�y�V���̎􂢁v
	-------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(brp_tamaoki_danmaku_01_callback)/* ��u�� */
{
	/*(225==1+224==1+7*32)*/
//	if ((HATUDAN_ITI_NO_JIKAN-224) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[224]�J�E���g�����̒e */
	if ((HATUDAN_ITI_NO_JIKAN-225) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[225]�J�E���g�����̒e */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{	/*CW(���v���)*/
		//	src->rotationCCW1024 -= (6);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 -= (3);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		else
		{	/*CCW(�����v���)*/
		//	src->rotationCCW1024 += (6);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 += (3);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		remilia_common_tama_oki_sub(src);
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
	�u���b�h�}�W�b�N�v
	-------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(red_magic_tamaoki_danmaku_01_callback)/* ��u�� */
{
	/*(225==1+224==1+7*32)*/
//	if ((HATUDAN_ITI_NO_JIKAN-224) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[224]�J�E���g�����̒e */
	if ((HATUDAN_ITI_NO_JIKAN-225) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[225]�J�E���g�����̒e */
	{
		if (0!=(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))/*TAMA_SPEC_AKA_AO_KUNAI_BIT*/
		{
			/*(���E�̕ǂŔ��˂���B)*/
			if (src->center.x256 < t256(32))
			{
				/*(��x�������˂��Ȃ�)*/
			//	src->hatudan_register_spec_data = (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			//	src->rotationCCW1024 -= (1024/2);/* 180/360 �x �Ȃ��Ă݂�e�X�g */
				goto red_magic_hansya_syori;
			}
			else
			if (src->center.x256 > t256(32)+t256(320) )
			{
			red_magic_hansya_syori:
				/*(��x�������˂��Ȃ�)*/
				src->hatudan_register_spec_data = (DANMAKU_LAYER_01)|(0)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			//	src->rotationCCW1024 -= (1024/2);/* 180/360 �x �Ȃ��Ă݂�e�X�g */
				src->rotationCCW1024 = (1024)-(src->rotationCCW1024);	/* ���][��1] */
			}
		}
		remilia_common_tama_oki_sub(src);
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
	[�e���O���[�v(2)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(remilia_tahane_danmaku_02_callback)/* �쒵�� */
{
	if ((HATUDAN_ITI_NO_JIKAN-128) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[128]�J�E���g �̒e */
	{
		src->hatudan_register_speed65536 = (1<<16); 		/* �e�� */
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data = (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
/*---------------------------------------------------------
	[�������Z�N�V����]	���f�u�u���h�E�c�F�y�V���̎􂢁v
	-------------------------------------------------------
	REG_09_REG1[�萔1]n��Ɉ��e��u���B
	0easy:			16
	1normal:		12
	2hard:			8
	3lunatic:		4
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_4e_remilia_vlad_tepes)
{
//	TAMA_DATA_10_TAMA_OKI_KANKAKU	= (0);// �J�E���^
	REG_09_REG1 	= ( (16)-((REG_0f_GAME_DIFFICULTY)<<2));//[�萔1]n��Ɉ��e��u���B
//	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[�萔1]�J�̑��x
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[�萔2]�ԐN�i�C���Ȃ���p�x
	card.danmaku_callback[1] = brp_tamaoki_danmaku_01_callback;/*(��u���p)*/
	card.danmaku_callback[2] = remilia_tahane_danmaku_02_callback;/*(�쒵�˗p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
/*---------------------------------------------------------
	[�������Z�N�V����]	�u���b�h�}�W�b�N�v
	-------------------------------------------------------
	REG_09_REG1[�萔1]n��Ɉ��e��u���B
	0easy:			32
	1normal:		24
	2hard:			16
	3lunatic:		8
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_4f_remilia_red_magic)
{
//	TAMA_DATA_10_TAMA_OKI_KANKAKU	= (0);// �J�E���^
//	REG_09_REG1 	= ( (16)-((REG_0f_GAME_DIFFICULTY)<<2));//[�萔1]n��Ɉ��e��u���B
	REG_09_REG1 	= ( (32)-((REG_0f_GAME_DIFFICULTY)<<3));//[�萔1]n��Ɉ��e��u���B
//	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[�萔1]�J�̑��x
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[�萔2]�ԐN�i�C���Ȃ���p�x
	//
	REG_0d_REG5 	= const_init_nan_ido_table	[tama_const_H09_NUMS_REMI_MAGIC 	+(REG_0f_GAME_DIFFICULTY)];
	REG_0e_REG6 	= const_init_nan_ido_table	[tama_const_H10_DIVS_REMI_MAGIC 	+(REG_0f_GAME_DIFFICULTY)];
//
	card.danmaku_callback[1] = red_magic_tamaoki_danmaku_01_callback;/*(��u���p)*/
	card.danmaku_callback[2] = remilia_tahane_danmaku_02_callback;/*(�쒵�˗p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/


}

/*---------------------------------------------------------
	[���e�Z�N�V����]	���f�u�u���h�E�c�F�y�V���̎􂢁v
	-------------------------------------------------------
	����(r35)�̃X�y�J�V�X�e���̏ꍇ�A�d�l��
	�X�y�J���܂����郌�W�X�^�l��ێ��ł��Ȃ��B
	(�X�y�J���n�܂�^�C�~���O�ŋ����I�Ƀ��W�X�^��0�N���A����Ă��܂���)
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_0b_REG3 	���e��A�W�܂�X���W(t256�`��)
	REG_0c_REG4 	���e��A�W�܂�Y���W(t256�`��)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_4e_remilia_vlad_tepes)
{
	if ((SPELL_TIME_0256-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(��Ԏn��)*/
	{
		HATSUDAN_01_speed256			= t256(1.0);	/* �e�� */
		HATSUDAN_02_speed_offset		= t256(5);/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= 0;	/* ��p�x */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|((ra_nd()) & TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
		HATSUDAN_06_n_way				= (13); 	/* [13way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(13));	/* �����p�x(65536[360/360�x]�� 13 ����) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		u32 aaa = ra_nd();
		REG_0b_REG3 = ((aaa    )&0xff00) + t256(((480-128-32-256)/2));
		REG_0c_REG4 = ((aaa<<8 )&0xff00) + t256(((272-256)/2));
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}


/*---------------------------------------------------------
	[���e�Z�N�V����]	�u���b�h�}�W�b�N�v
	-------------------------------------------------------
	����(r35)�̃X�y�J�V�X�e���̏ꍇ�A�d�l��
	�X�y�J���܂����郌�W�X�^�l��ێ��ł��Ȃ��B
	(�X�y�J���n�܂�^�C�~���O�ŋ����I�Ƀ��W�X�^��0�N���A����Ă��܂���)
	-------------------------------------------------------
	�g�p���W�X�^:
	REG_0b_REG3 	���e��A�W�܂�X���W(t256�`��)
	REG_0c_REG4 	���e��A�W�܂�Y���W(t256�`��)
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_4f_remilia_red_magic)
{
	if ((SPELL_TIME_0256-1)==((REG_10_BOSS_SPELL_TIMER) ))/*(��Ԏn��)*/
	{
		/*(���f�u�u���h�E�c�F�y�V���̎􂢁v�ɔ�ׂāA�u���b�h�}�W�b�N�v�̑�ʒe�̓����͔��ɒx��)*/
		HATSUDAN_01_speed256			= t256(0.5);	/* �e��(r40==0.5, r39==1.0) */
		HATSUDAN_02_speed_offset		= t256(3);/*(�e�X�g)*/
	//	HATSUDAN_03_angle65536			= (65536/4);	/* ��p�x(�����Ɍ��B���E����O�x) */
		HATSUDAN_03_angle65536			= ((ra_nd())&0x1fff);	/* ��p�x(?) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_OODAMA32_01_AKA); 	/* [����e] */
		HATSUDAN_06_n_way				= REG_0d_REG5;//(10); 	/* [10way] */		/* ���e�� */
		HATSUDAN_07_div_angle65536		= REG_0e_REG6;//(int)(65536/(10));	/* �����p�x(65536[360/360�x]�� 10 ����) */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
		u32 aaa = ra_nd();
		REG_0b_REG3 = ((aaa    )&0xff00) + t256(((480-128-32-256)/2));
		REG_0c_REG4 = ((aaa<<8 )&0xff00) + t256(((272-256)/2));
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}
