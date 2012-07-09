
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�A���X(��)	�Ԃ̖��@(2�i�K��)�̂悤�Ȃ��́B
	-------------------------------------------------------
	�ړ��𐧌�����J�[�h�B
	�Ƃ肠�����A���X������A�Ԃ̖��@��2�i�K�ځB
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B(4���1�񔭒e����)
	REG_0a_REG2 	���@�_���p���ꎞ�ێ�����B(���݂̎����ł� HATSUDAN_03_angle65536 ���j�󂳂��d�l�Ȃ̂ŗv�邪�A��ŗv��Ȃ��Ȃ邩���H)
	REG_0b_REG3 	[x���W�ꎞ�ۑ�]
	REG_0c_REG4 	[y���W�ꎞ�ۑ�]
	REG_0d_REG5 	��Փx�ʒ萔1
	REG_0e_REG6 	��Փx�ʒ萔2
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_0e_aka_2nd)
{
	REG_0d_REG5 	= const_init_nan_ido_table	[tama_const_H06_NUMS_ALICE_RED		+(REG_0f_GAME_DIFFICULTY)];
	REG_0e_REG6 	= const_init_nan_ido_table	[tama_const_H07_DIVS_ALICE_RED		+(REG_0f_GAME_DIFFICULTY)];/*(((REG_10_BOSS_SPELL_TIMER)&0x20)>>3)+*/
}
local OBJ_CALL_FUNC(spell_create_0e_aka_2nd)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 4);//	if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
	if (0==(REG_08_REG0))
	{
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x0f))
		{
			REG_0b_REG3 = REG_02_DEST_X;//[�{�Xx���W�ꎞ�ۑ�]
			REG_0c_REG4 = REG_03_DEST_Y;//[�{�Xy���W�ꎞ�ۑ�]
			//------------------
			HATSUDAN_01_speed256	= (t256(1.0));
			HATSUDAN_03_angle65536	= deg1024to65536((((REG_10_BOSS_SPELL_TIMER<<2))));
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
		//	src->center.x256 = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
		//	src->center.y256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
			//------------------
			//
		//[�d��]	HATSUDAN_01_speed256				= (t256(1.0));							/* �e�� */
			HATSUDAN_02_speed_offset			= t256(1);/*6(�e�X�g)*/
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 				/* [�ԕĒe] */
			HATSUDAN_06_n_way					= (8);						/* [8way] */
			HATSUDAN_07_div_angle65536			= (int)(65536/48);			/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
			{
				/* hatudan_system_regist_katayori_n_way�̎d�l���P���ȈׁA�␳���� */
				#define BA_HOSEI48	((65536/(48/8))/2)	/* ��� 65536�n�� 48�����e x 8 way �̐^��(�܂�/2)�t�� */
				const int tmp_kakudo =
					(65536/(4*2)) + 	/* (65536/(4*2)) == �܂�(90/360)�x�̔���������B(���2�{�ɂȂ�) */	/* �e����΂ɂ��Ȃ��͈� */
				//	(65536/(8*2)) + 	/* (65536/(8*2)) == �܂�(45/360)�x�̔���������B(���2�{�ɂȂ�) */	/* �e����΂ɂ��Ȃ��͈� */
				//	(65536/128) +		/* �܂�(2.8125/360)�x������B(���E�Ȃ̂�2�{==5.625�x�ɂȂ�) */		/* �e����΂ɂ��Ȃ��͈� */
					((256 + (REG_03_DEST_Y/*sin_value_t256*/))<<4);					/* ���ɗh�炬����������B */
				// ���ˍ��W���X�g�A�B
				REG_02_DEST_X = REG_0b_REG3;//[�{�Xx���W����]
				REG_03_DEST_Y = REG_0c_REG4;//[�{�Xy���W����]
				/* �E�� */
				HATSUDAN_03_angle65536		= (( tmp_kakudo-BA_HOSEI48)&(65536-1)); 	/* �p�x(��CCW�Ȃ̂Ő�����==�E��) */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
				/* ���� */
				HATSUDAN_03_angle65536		= ((-tmp_kakudo-BA_HOSEI48)&(65536-1)); 	/* �p�x(��CCW�Ȃ̂ŕ�����==����) */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
		}
		{
			if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x1f))
			{
			//(r35�ŃJ�[�h�̏ꍇ�̂ݏȗ��\)	set_REG_DEST_XY(src);
				calculate_jikinerai();/* ���@�_���p�쐬 */
				REG_0a_REG2 						= HATSUDAN_03_angle65536;/* ���@�_���p */
				#if (1)
				cpu_bullet_play_15_auto();
				#endif
			}
			//	HATSUDAN_01_speed256			= (t256(1.1));							/* �e�� */
				HATSUDAN_01_speed256			= (t256(1.0)+((0x1f-((REG_10_BOSS_SPELL_TIMER)&0x1f))));	/* �e�� */ /* ��Ō��ق������� */
				HATSUDAN_02_speed_offset		= t256(3/*6*/);/*(�e�X�g)*/
				HATSUDAN_03_angle65536			= (REG_0a_REG2);/* ���@�_���p */
				HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_2000_EFFECT_MINI)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);				/* [�ԗؒe] */
			//
				HATSUDAN_06_n_way				= REG_0d_REG5;										/* [8way] */
				HATSUDAN_07_div_angle65536		= REG_0e_REG6;	/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
			{
			//	os2x	���e����x�ʒu(�����I�ɉ��ɂ��炷)
				REG_02_DEST_X						+= (ra_nd()&0x1fff)-t256((32)/2);	/* �e��x256 */
				/* ���@�_�� */
			//	hatudan_system_regist_single();
				hatudan_system_regist_n_way();/* (r33-) */
			}
		}
	}
}


#if 1/*(r39)*/
/*---------------------------------------------------------
	�A���X normal�ʏ�U��3(�̈ꕔ)
	-------------------------------------------------------
	-------------------------------------------------------
	�g�p���W�X�^
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B
	[�e���O���[�v(2)�Z�N�V����]
	-------------------------------------------------------
	�����e�B���v���B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(alice_aaa_danmaku_01_callback)
{
	/* 0-256 �J�E���g�܂� */ 	/* [0]�J�E���g==���e�ʒu */
	if ((HATUDAN_ITI_NO_JIKAN-80) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[0-79]�J�E���g�o�߂����e */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))
		{
			src->rotationCCW1024++;/* �Ȃ��Ă݂�e�X�g */
		}
		else
		{
			src->rotationCCW1024--;/* �Ȃ��Ă݂�e�X�g */
		}
	}
	// (���̕���������₷���H)
	else
//	if ((HATUDAN_ITI_NO_JIKAN-256) == src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[256]�J�E���g�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����==��ʓ��ł͒e�͏����Ȃ�) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		#if (1)/*(�f�o�b�O)*/
		src->obj_type_set						= (BULLET_MINI8_BASE + TAMA_IRO_01_AKA);		/* [�~�j�Ԋےe] */
		reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA); 	/* �e�O���ƒe�����蔻���ύX����B */
		#endif
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_17_alice_nejiri10sec)
{
	card.danmaku_callback[1] = alice_aaa_danmaku_01_callback;/*(�Ēe�B�����e�B�����v���B) (�Ēe�B�����e�B���v���B)*/
//	card.danmaku_callback[2] = rumia_danmaku_02_callback;/*(�ےe�B�A�e�B���@�_���B)*/
//	card.danmaku_callback[3] = NULL;
}

local OBJ_CALL_FUNC(spell_create_17_alice_nejiri10sec)
{
}
#endif

#if 0/*(r38)*/
/*---------------------------------------------------------
	�A���X normal�ʏ�U��3(�̈ꕔ)
	-------------------------------------------------------
	�Ȃ񂩑S�R�܂��������ĂȂ��B
	�d�X�����̂���őn���Ă��������Y�k���ɂȂ����B
	���������Ă�̂��킩��Ȃ��Ǝv�����A�킽���ɂ��킩��Ȃ���B
	���낵�����̂̕З؂��ȉ����B
	psp�𑜓x���S�R�Ⴄ����A�����Ɏ����悤�Ƃ��Ȃ������ǂ������H
	-------------------------------------------------------
	�g�p���W�X�^
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_17_alice_nejiri10sec)
{
	if ((64*1)<(REG_10_BOSS_SPELL_TIMER))
	{
		/* [CW ���v���] */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		{
			src->tmp_angleCCW65536 -= (int)(65536/24);
			#if 0
			src->tmp_angleCCW65536 += (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 += (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
			//
				HATSUDAN_01_speed256				= (t256(1.0));							/* �e�� */
				HATSUDAN_02_speed_offset			= t256(1);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
				HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);				/* [�g�t���ےe] */
				HATSUDAN_06_n_way					= (6);									/* [6way] */
				HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
		//
			{
				HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* �p�x */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			//
				HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* �p�x */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
	else
	if ((64*2)<(REG_10_BOSS_SPELL_TIMER))
	{
		/*[CCW �����v���]*/
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x03))
		{
			src->tmp_angleCCW65536 += (int)(65536/24);
			#if 0
			src->tmp_angleCCW65536 -= (int)(ra_nd() & ((1<<difficulty)-1) );
			#else
			{	static const u8 masked_tbl[4] = { 0x00, 0x0f, 0x3f, 0xff };
				src->tmp_angleCCW65536 -= (int)(ra_nd() & (masked_tbl[(REG_0f_GAME_DIFFICULTY)]) );
			}
			#endif
			mask65536(src->tmp_angleCCW65536);
			//
				HATSUDAN_01_speed256				= (t256(1.0));							/* �e�� */
				HATSUDAN_02_speed_offset			= t256(1);/*(�e�X�g)*/
				HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
				HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);				/* [�g�t���Ԋےe] */
				HATSUDAN_06_n_way					= (6);									/* [6way] */
				HATSUDAN_07_div_angle65536			= (int)(65536/48);						/* �����p�x(1024/(6*8) ) �P����8���������̈�������6���������p�x */
			{
				HATSUDAN_03_angle65536				= (src->tmp_angleCCW65536); 							/* �p�x */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			//
				HATSUDAN_03_angle65536				= ((src->tmp_angleCCW65536+(65536/2))&(65536-1));					/* �p�x */
				hatudan_system_regist_katayori_n_way();/* (r33-) */
			}
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
}
#endif

/*---------------------------------------------------------
	�A���X	�A���X�l�`�J�[�h
	-------------------------------------------------------
�����u�����̕������l�` -easy-�v
�A���X�̕������l�̂̐l�`���A��񕪗􂷂�7way�e�����B
4 x 7 x 7 == �ő� 196[�e](�d�X��)
3 x 7 x 7 == �ő� 147[�e](�͕핗)	//3 x 8 x 8 == 192
	-------------------------------------------------------
�����u�����̕������l�`�v
�A���X�̕������Z�̂̐l�`���A��񕪗􂷂�7way�e�����B
6 x 7 x 7 == �ő� 294[�e](�d�X��)
5 x 7 x 7 == �ő� 245[�e](�͕핗)	//5 x 8 x 8 == 320
	-------------------------------------------------------
�����u�����̕������l�` -hard-�v
�A���X�̕��������̂̐l�`���A��񕪗􂷂�7way�e�����B
8 x 7 x 7 == �ő� 392[�e](�d�X��)
9 x 7 x 7 == �ő� 441[�e](�͕핗)	//7 x 8 x 8 == 448
	-------------------------------------------------------
�����u�����̃I�����A���l�`�v�Ԓe���Βe
�A���X�̕��������̂̐l�`���A�O�񕪗􂷂�7way�e�����B
8 x 7 x 7 x 7 == �ő�2744[�e](�d�X��)
2 x 7 x 7 x 7 == �ő� 686[�e](�͕핗)	//9 x 8 x 8 == 576
---------------------------------------------------------*/
extern OBJ_CALL_FUNC(add_zako_alice_doll_type_a);/* �A���X�l�`�J�[�h */
local OBJ_CALL_FUNC(spell_create_0b_alice_zako_doll)
{
	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_alice_doll_type_a(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}

/*---------------------------------------------------------
	�v7�ɕ��􂷂�B
	-------------------------------------------------------


---------------------------------------------------------*/
#if 0
local void exchange_damnaku_alice_7_bunretu(void)
{
	int jj = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_1024_MAX; ii++)/* �S�����ׂ�B */
	{
		OBJ *s;
		s = &obj99[OBJ_HEAD_00_0x0000_TAMA+ii];
		/* �F�Ɋ֌W�Ȃ��ؒe[ ... ]�Ȃ� */
		if (((BULLET_UROKO14_03_AOI+1) & 0xfff8) == (s->type & 0xfff8) )	/* [�ԗؒe]�Ȃ� */
		{
			jj++;
		//	jj &= 0x07; 	// 2 3 4 5	6 7 8 9
			jj &= 0x03; 	// 3 4 5 6
		//	s_change_meirin_yajirusi_one(s);
		//	s->tra65536 					= t256(6);		/* ���������e */
		//	s->tra65536 					= (jj<<8)+t256(2);	/* (2 ... 9) ���������e */
			s->tra65536 					= (jj<<8)+t256(3);	/* (3 ... 6) ���������e */
			/* (1�͒x�������̂ŏ��O) 9 ���Ƒ��߂��邩�� */
		}
	}
}
#endif
extern OBJ_CALL_FUNC(add_zako_alice_doll_type_b);/* �A���X�l�`�J�[�h */
local OBJ_CALL_FUNC(spell_create_1e_alice_tama_doll)
{
	if (250==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_alice_doll_type_b(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x3f))
	{
		/* �e���� */
//		exchange_damnaku_arice_7_bunretu();
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}
/*
// �����u�����̕������l�`�v
	�e�l�`��1�e����(�e)
	�O���Ɏˏo�B
//
	180�x���]����B
	�V�e�ɕ���(�e�����e)
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
//
	180�x���]����B
	�V�e�ɕ���(���e���Ԓe)
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
// �����u�����̃I�����A���l�`�v�̂�
	180�x���]����B
	�V�e�ɕ���(�Ԓe���Βe)
  A   A    A   A   --- ���x����

		A------------- ���x��
		|
   A	|	 A	------ ���x�x��
		+
//

*/

/*---------------------------------------------------------
	�A���X	�z�K�q���ۂ�(?)�J�[�h
	-------------------------------------------------------
	�e�L�g�[
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�����B

---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_25_alice_suwako)
{
//	if ((0) ==((REG_10_BOSS_SPELL_TIMER)&0x01))/* 2���1�� */
	{
	//	if ((0) ==((REG_10_BOSS_SPELL_TIMER)&0x07))/*  8��1�� */
		if ((0) ==((REG_10_BOSS_SPELL_TIMER)&0x0f))/* 16��1�� */
		{
			REG_08_REG0 -= (int)(65536*8/256);
			cpu_bullet_play_15_auto();
		}
	//	REG_08_REG0 -= (65536/256); 	/* 1����256���������p�x */
	//	REG_08_REG0 -= (65536/512); 	/* 1����256���������p�x�̔����B */
		REG_08_REG0 -= (200);	/*	�e�L�g�[ 1����256���������p�x�̔����B */
		/* �e���� */
		{
			HATSUDAN_01_speed256				= (t256(1.0) ); 					/* �e��(psp�̉�ʂ͋���) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_06_n_way					= (8);								/* [8way] */			/* 8�����e */
			HATSUDAN_07_div_angle65536			= (65536/8);						/* �����p�x(�e�����Ȃ��͈͂�����32����) */	/* (ra_nd16>>4) == ����(0-4095) */
			//
			/* 0 [...��] 150 [...��+��] 180 [...��] 255 */
			if (0==((REG_10_BOSS_SPELL_TIMER)&1))
			{
				if ((s32)(128+((REG_0f_GAME_DIFFICULTY)<<2)) > (REG_10_BOSS_SPELL_TIMER))
				{
					HATSUDAN_02_speed_offset		= t256(2);					/* ���������e */	/* ���̕����ɂȂ邩������ */
					HATSUDAN_03_angle65536			= (REG_08_REG0);			/* ���� */
					HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_01_AKA); 	/* �ԕĒe */
					hatudan_system_regist_katayori_n_way();/* (r33-) */
				}
			}
			else
			{
				if ((s32)(128-((REG_0f_GAME_DIFFICULTY)<<2)) < (REG_10_BOSS_SPELL_TIMER))
				{
					HATSUDAN_02_speed_offset		= t256(1);					/* ���������e */	/* ���̕����ɂȂ邩������ */
					HATSUDAN_03_angle65536			= (65536-REG_08_REG0);		/* ���� */
					HATSUDAN_05_bullet_obj_type 	= (BULLET_KOME_BASE + TAMA_IRO_03_AOI); 	/* �Ēe */
					hatudan_system_regist_katayori_n_way();/* (r33-) */
				}
			}
		}
	}

}
