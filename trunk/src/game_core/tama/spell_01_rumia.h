
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���[�~�A�̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������J�[�h(�\��)
	-------------------------------------------------------
	5�e�ڂ����@�_���� 32����16���� �|��e�B
	-------------------------------------------------------
---a aaaa [aaa]
0000 xxxx 16�� ����         �F (0x00==((time_out)&0x20))
0001 0000 ���@�_���p�쐬    �F (0x00==((time_out)&0x20))
0010 xxxx 16�� ����         ���F
0011 0000 ���@�_���p�쐬    ���F
0100 xxxx 16�� ����         �F (0x00==((time_out)&0x20))
0101 0000 ���@�_���p�쐬    �F (0x00==((time_out)&0x20))
0110 xxxx 16�� ����         ���F
0111 0000 ���@�_���p�쐬    ���F
//
1000 0000 start
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^1�B
	REG_09_REG1 	�J�E���^2�B
	REG_0a_REG2 	�e�̊�p�x�Baaa_angle65536
	REG_0b_REG3 	�e�ԍ��B0x0f��1�e�ځA0x0e��2�e�ځB
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_08_rumia_night_bird)
{
	count_up_limit_NUM(REG_NUM_08_REG0, 32);
//	if (0x10==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* ���@�_���p�쐬 */
	if (1==(REG_08_REG0))/* ���@�_���p�쐬 */
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 2);/*(�J�E���^2)*/
		#if 1
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* ���@�_���p�쐬 */
		REG_0a_REG2 = HATSUDAN_03_angle65536;/* ���@�_���p */
		#endif
        //  2048 == (65536)/(32)        : 1����32���������p�x�B
        //  8192 == (2048) * (4)        : 5�e��(4�e���̕�)�B
        // 57344 == (65536) - (8192)    : �J�n�p�x�B(���Z���Ȃ��ƌ덷���o��B)
    //  REG_0a_REG2 += (const int)(65536-(int)((65536*4)/(32)));/* 5�e�ڂ����@�_�� */
	//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
		if (1==(REG_09_REG1))/*(�J�E���^2)*/
				{	REG_0a_REG2 += (57344); 	}/* 5�e��(4�e���̕�)�����@�_��(65536-(4*(65536/32))) */
		else	{	REG_0a_REG2 += (8192);		}/* 5�e��(4�e���̕�)�����@�_��(65536+(4*(65536/32))) */
		mask65536(REG_0a_REG2);
		/* �p�x(65536[360/360�x]�� 32����) */
		REG_0b_REG3 = 0x10;
	}
//	else
//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x10))/* 16�� ���� */
	if (15<(REG_08_REG0))/* (16 ... 31), 16�� ���� */
	{
		{
			/* ���� */
			REG_0b_REG3 -= (1);
		//	REG_0b_REG3 = ((REG_10_BOSS_SPELL_TIMER)&0x0f);
			#if (0)/* �f�o�b�O(���@�_���e�̐F��ς���) */
			if (0x0b==REG_0b_REG3)/* 0x0b==5�e�� (0x0f��1�e�ڂ�����0x0b��5�e��) */
						{	HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);		}/* �g���ԐF�e �F�ς���B */
			else
			#endif
			#if 1
			{
			//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
				if (1==(REG_09_REG1))/*(�J�E���^2)*/
						{	HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);		}/* �g���F�e */
				else	{	HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_04_MIZU_IRO);	}/* �g�����F�e */
			}
			#endif
		//	HATSUDAN_01_speed256			= (t256(1.5));								/* �e�� */
			HATSUDAN_01_speed256			= (t256(0.75))+((16-(REG_0b_REG3))<<5); 			/* �e�� */
			/* �e�� 0.75 - 2.75 �{ */
			HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
			HATSUDAN_03_angle65536			= (REG_0a_REG2);	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(0)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			/* [�g�t���F�e] [�g�t�����F�e] */
			hatudan_system_regist_single();/* (r33-) */
		}
		/* ���̒e�̊p�x */
		//	 2048 == (65536)/(32)		: 1����32���������p�x�B
		//	63488 == (65536) - (2048)	: ���Z�̏ꍇ(���Z���Ȃ��ƌ덷���o��B)
	//	if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
		if (1==(REG_09_REG1))/*(�J�E���^2)*/
				{	REG_0a_REG2 += (2048);		}/* �p�x(65536[360/360�x]�� 32����) (int)(65536/(32)); */
		else	{	REG_0a_REG2 += (63488); 	}/* �p�x(65536[360/360�x]�� 32����) (65536)-(int)(65536/(32)); */
		mask65536(REG_0a_REG2);
		//
		bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
	}
}


/*---------------------------------------------------------
	�g1�ʃ{�X ���[�~�A �ŕ��u�f�B�}�[�P�C�V�����v�ɂ�����Ƃ��������J�[�h(�\��)
	-------------------------------------------------------
	���[�~�A�����e:
	�����e�ɂ��ǂ�����Ǝ�ނ�����悤�ȋC�����Ă����B
	�ŕ��u�f�B�}�[�P�C�V�����v�̌����e�́B
		1. ��苗���ɔ�яo���B(�ʏ팸���e)
		2. �e��~�B
		3. �ˑR (�e�O����)90�x��]����B(�ɍ��E���݂����Ȋ����Ŋ��炩�ł͂��邪�A���΂₭��])
		4.�e�O�����X���Ȃ���e���L����B
	-------------------------------------------------------
	16
	-------------------------------------------------------
	0010 01--	0x027f == 64*10-1
	0010 00--	0x023f == 64* 9-1
				...
	0000 00--	0x003f == 64* 1-1
	-------------------------------------------------------
640==0x280
512==0x200
	32*0		�T
	32*1		�U
	32*2		�R
	32*3		�S
	32*4		�P
	32*5		�Q
	32*6		�x��
	32*7		�x��
	32*8		�x��
	32*9			��
	32*10		�x��
	32*11			��
	32*12		�x��
	32*13			��
	32*14		�x��
	32*15		�x��

---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
	�����e�B�����v���B
	[�e���O���[�v(2)�Z�N�V����]
	-------------------------------------------------------
	�����e�B���v���B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(rumia_danmaku_01_callback)
{
	/* 0-32 �J�E���g�܂� */ 	/* [0]�J�E���g==���e�ʒu */
	if ((HATUDAN_ITI_NO_JIKAN-32) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[0-31]�J�E���g�o�߂����e */
	{
		;/* ���e��A�����͉������Ȃ�==���i�e */
	}
	else/* 32-64 �J�E���g�܂� */
	if ((HATUDAN_ITI_NO_JIKAN-64) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[32-63]�J�E���g�o�߂����e */
	{
		if (0==(src->hatudan_register_spec_data & TAMA_SPEC_KAITEN_HOUKOU_BIT))
		{
			/*(�͕핗�ł͉�CCW�Ȃ̂ŁA�������������v���)*/
		//	src->rotationCCW1024 += (8);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 += (10);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
		else
		{
			/*(�͕핗�ł͉�CCW�Ȃ̂ŁA�����������v���)*/
		//	src->rotationCCW1024 -= (8);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
			src->rotationCCW1024 -= (10);/* 90/360 �x �Ȃ��Ă݂�e�X�g(32�J�E���g�����ċȂ���B256==8*32) */
		}
	}
	else/* 64-256 �J�E���g�܂� */
	if ((HATUDAN_ITI_NO_JIKAN-256) < src->jyumyou)/* ���e�G�t�F�N�g�ォ��[64-255]�J�E���g�o�߂����e */
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
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}


	#if (0)
	/* ���e�� 512 �J�E���g�ȏ�͒e�������B */
	if ((HATUDAN_ITI_NO_JIKAN-512) > src->jyumyou/*(�����Ȃ̂Ŏ����Ō���)*/)/* ���e�G�t�F�N�g�ォ��[512]�J�E���g�ȏ�o�߂����e */
	{
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
	#endif


/*---------------------------------------------------------
	[�e���O���[�v(2)�Z�N�V����]
	-------------------------------------------------------
	�A�e�B���@�_���B
	-------------------------------------------------------
	���@��_��Ȃ��ꍇ�����邪�A�������ǂ�������Ȃ��B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(rumia_danmaku_02_callback)/* �A�e */
{
	/* 32 �J�E���g�Ȃ� */
	if ((HATUDAN_ITI_NO_JIKAN-32) == src->jyumyou)/* ���e�G�t�F�N�g�ォ��[32]�J�E���g�o�߂����e */
	{
		REG_02_DEST_X	= ((src->center.x256));
		REG_03_DEST_Y	= ((src->center.y256));
		calculate_jikinerai();/* ���@�_���p�쐬 */
		#if 0
		/*(�_��)*/
	//	src->rotationCCW1024 = (0);//�`��p�p�x(deg65536to1024(HATSUDAN_03_angle65536));/* ���@�_���p */
		#else
		/*(���ʃP�[�X�A���̂܂�(������)�̊p�x�ňړ�����)*/
		src->rotationCCW1024 = (deg65536to1024(HATSUDAN_03_angle65536));/* ���@�_���p */
		/* �����̕`��p�p�x�͌�ŗ��p����B */
		#endif
		//
		src->hatudan_register_speed65536	= (t256(2.0)<<8);			/* �e�� */
		//
		/* (�ʏ�e�֕ϐg����) */
		src->hatudan_register_spec_data 	= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}


/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_init_29_rumia_demarcation)
{
	card.danmaku_callback[1] = rumia_danmaku_01_callback;/*(�Ēe�B�����e�B�����v���B) (�Ēe�B�����e�B���v���B)*/
	card.danmaku_callback[2] = rumia_danmaku_02_callback;/*(�ےe�B�A�e�B���@�_���B)*/
//	card.danmaku_callback[3] = NULL;
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B(32���1�񔭒e����)
	REG_09_REG1 	�J�E���^�B(16���)
	REG_0a_REG2 	�e�F(�����e)�B
	REG_0b_REG3 	�e�p�x(�����e)�B	�e�p�x(�A�e)�B

---------------------------------------------------------*/
// �J�[�h�X�N���v�g��ł́uelse�v�͖����B
local OBJ_CALL_FUNC(spell_create_29_rumia_demarcation)
{
//	if ((0x1f)==((REG_10_BOSS_SPELL_TIMER)&0x1f))
	count_up_limit_NUM(REG_NUM_08_REG0, 32);
	if (0==(REG_08_REG0))
	{
		count_up_limit_NUM(REG_NUM_09_REG1, 16);//
		#if (1)/*(���e���W���C��)*/
			REG_02_DEST_X	= ((src->center.x256));
			REG_03_DEST_Y	= ((src->center.y256));
		#endif
		if (2==REG_09_REG1)
		{
			REG_0a_REG2 = (TAMA_IRO_03_AOI);		/* �e */
			goto kousadan;
		}
		if (4==REG_09_REG1)
		{
			REG_0a_REG2 = (TAMA_IRO_05_MIDORI); 	/* �Βe */
			goto kousadan;
		}
		if (6==REG_09_REG1)
		{
			REG_0a_REG2 = (TAMA_IRO_01_AKA);		/* �Ԓe */
			goto kousadan;
		}
		if (14<REG_09_REG1)/* �Ō�̕��͎��Ԃ�����Ȃ��̂ŁA�ϐg�������Ԃɍ���Ȃ��B */
		{
			goto oyasumi;
		}
		if (9<REG_09_REG1)		/* ���@�_���A�e */
		{
		//	dimmer_shot_02_rendan(src);
			/*---------------------------------------------------------
				���@�_���A�e
			-------------------------------------------------------
			�L�������ō�肪���Ȃ̂ŁA����Ń`�F�b�N�Bnormal�̏ꍇ�A���ł���A�e�͂S�񂾂����B
			---------------------------------------------------------*/
			//local OBJ_CALL_FUNC(dimmer_shot_02_rendan)
			unsigned int ii;
			for (ii=0; ii<8; ii++)
			{
				#if 1
				REG_02_DEST_X	= ((src->center.x256));
				REG_03_DEST_Y	= ((src->center.y256));
				calculate_jikinerai();/* ���@�_���p�쐬 */
				REG_0b_REG3 = HATSUDAN_03_angle65536;/* ���@�_���p */
				#endif
				//	2048 == (65536)/(32)		: 1����32���������p�x�B
				//	8192 == (2048) * (4)		: 5�e��(4�e���̕�)�B
				// 57344 == (65536) - (8192)	: �J�n�p�x�B(���Z���Ȃ��ƌ덷���o��B)
			//	REG_0b_REG3 += (const int)(65536-(int)((65536*4)/(32)));/* 5�e�ڂ����@�_�� */
				if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
						{	REG_0b_REG3 += (57344); 	}/* 5�e��(4�e���̕�)�����@�_��(65536-(4*(65536/32))) */
				else	{	REG_0b_REG3 += (8192);		}/* 5�e��(4�e���̕�)�����@�_��(65536+(4*(65536/32))) */
				mask65536(REG_0b_REG3);
				/* �p�x(65536[360/360�x]�� 32����) */
					HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_02)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
					HATSUDAN_05_bullet_obj_type 		= BULLET_WAKU12_BASE + TAMA_IRO_03_AOI; 		/* [�g�t���F�e] [�g�t�����F�e] */
				unsigned int jj;
				for (jj=0; jj<16; jj++)
				{
				//	HATSUDAN_01_speed256				= (t256(1.5));								/* �e�� */
					HATSUDAN_01_speed256				= (t256(0.75))+((16-(jj))<<5);				/* �e�� */
					/* �e�� 0.75 - 2.75 �{ */
					HATSUDAN_02_speed_offset			= (ii<<6);//t256(0);/*(�e�X�g)*/
					HATSUDAN_03_angle65536				= (REG_0b_REG3);	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
					hatudan_system_regist_single();/* (r33-) */
					/* ���̒e�̊p�x */
					//	 2048 == (65536)/(32)		: 1����32���������p�x�B
					//	63488 == (65536) - (2048)	: ���Z�̏ꍇ(���Z���Ȃ��ƌ덷���o��B)
					if (0x00==((REG_10_BOSS_SPELL_TIMER)&0x20))
							{	REG_0b_REG3 += (2048);		}/* �p�x(65536[360/360�x]�� 32����) (int)(65536/(32)); */
					else	{	REG_0b_REG3 += (63488); 	}/* �p�x(65536[360/360�x]�� 32����) (65536)-(int)(65536/(32)); */
					mask65536(REG_0b_REG3);
				}
			}
			bullet_play_04_auto(VOICE14_SAKUYA_SHOT02);/*VOICE15_BOSS_KOUGEKI_01*/
		}
		goto oyasumi;
	kousadan:	/* �����e�B �e / �Βe / �Ԓe */
		{
		//	dimmer_shot_01_kousadan(src, REG_0a_REG2/* �e�F */);
			/*---------------------------------------------------------
				�����e�B �e / �Βe / �Ԓe
			---------------------------------------------------------*/
		//	local void dimmer_shot_01_kousadan(OBJ/**/ *src, u8 tama_type)
			REG_0b_REG3 = (((REG_0a_REG2)&0x06)<<8);/* �e�F�ʂɔ��e�p��ς���B(((REG_0a_REG2)&0x06)<<6) */
			HATSUDAN_01_speed256				= (t256(1.0));			/* �e�� (t256(1.5)) */
			HATSUDAN_02_speed_offset			= t256(0);/*(�e�X�g)*/
			HATSUDAN_03_angle65536				= (REG_0b_REG3);					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(0)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			HATSUDAN_05_bullet_obj_type 		= BULLET_KOME_BASE + (REG_0a_REG2); 		/* [�g���F�e] [�g�����F�e] */
			HATSUDAN_06_n_way					= (32); 				/* (32) [32way] */
			HATSUDAN_07_div_angle65536			= (65536/32);			/* �����p�x (65536/32) */
			hatudan_system_regist_katayori_n_way();/*(r33-)*/
			/* HATSUDAN_03_angle65536��hatudan_system_regist_katayori_n_way();�Ŕj�󂳂��(�d�l��)�̂ōĐݒ肪�K�v�B */
			HATSUDAN_03_angle65536				= (REG_0b_REG3);					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_KAITEN_HOUKOU_BIT)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
			hatudan_system_regist_katayori_n_way();
			cpu_bullet_play_15_auto();
		}
	oyasumi:	/* ���x�� */
		;
	}

}
		//{
		//	#define P0x80		0x80
		//	#define YASUMI000	0x00
		//	const u8 ttt[16] =
		//	{
		//	/*0x0f*/0	YASUMI000,	/* �x�� */
		//	/*0x0e*/1	YASUMI000,	/* �x�� */
		//	/*0x0d*/2	(BULLET_KOME_03_AOI    - BULLET_KOME_00_SIRO),		/* �e */
		//	/*0x0c*/3	YASUMI000,	/* �x�� */
		//	/*0x0b*/4	(BULLET_KOME_05_MIDORI - BULLET_KOME_00_SIRO),		/* �Βe */
		//	/*0x0a*/5	YASUMI000,	/* �x�� */
		//	/*0x09*/6	(BULLET_KOME_01_AKA    - BULLET_KOME_00_SIRO),		/* �Ԓe */
		//	/*0x08*/7	YASUMI000,	/* �x�� */
		//	/*0x07*/8	YASUMI000,	/* �x�� */
		//	/*0x06*/9	YASUMI000,	/* �x�� */
		//	/*0x05*/10	P0x80,	/* ���@�_���A�e */
		//	/*0x04*/	P0x80,	/* ���@�_���A�e */
		//	/*0x03*/	P0x80,	/* ���@�_���A�e */
		//	/*0x02*/	P0x80,	/* ���@�_���A�e */
		//	/*0x01*/	P0x80,	/* ���@�_���A�e */
		//	/*0x00*/	P0x80,	/* ���@�_���A�e */
		//	};
		//	tama_type = ttt[((REG_09_REG1/*(REG_10_BOSS_SPELL_TIMER)>>5*/)&0x0f)];
		//}
