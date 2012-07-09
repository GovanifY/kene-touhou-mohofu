
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�˖��� ���̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	��	�ʏ�U��1
	-------------------------------------------------------
		��(12)�e
		��(12)�e
		��(12)�e
	-------------------------------------------------------
	�Ƃ肠�����A�܂��������ĂȂ��B
	�Ƃ������A�܂������Ⴄ�B
	�u�ʏ�U��1�v�͂悭�킩��Ȃ��B�̂ŁA�F�X�ς����Ⴄ�Ǝv���B
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	bbb_angle65536
	REG_0b_REG3 	���񂾂�傫���Ȃ�~�̔��a�B
	REG_0c_REG4 	���p�J�E���^�B(48���1��) �����B
	-------------------------------------------------------
	REG_0d_REG5 	x�ʒu/�ޔ�/����
	REG_0e_REG6 	y�ʒu/�ޔ�/����
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_15_aya_misogi1)
{
//	if ((0x08)==((REG_10_BOSS_SPELL_TIMER)&0x0f))/* (16���1��)(16�񖈂ɔ��e) */
	count_up_limit_NUM(REG_NUM_08_REG0, 16);//	/* (16���1��)(16�񖈂ɔ��e) */
	if (1==REG_08_REG0) /* (16���1��) ���e */
	{
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* ���(�����12�e) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
	//	const unsigned int REG_0b_REG3 = (((~(REG_10_BOSS_SPELL_TIMER))>>3)&0x1f);/* �e�L�g�[�ɑ傫�����Ă݂�B */
		count_up_limit_NUM(REG_NUM_0b_REG3, 24);//		/*(r34, 32)*/
		//------------------
		#if (0)//
		// �ޔ�
		REG_0d_REG5 = REG_02_DEST_X;
		REG_0e_REG6 = REG_03_DEST_Y;
		//------------------
		#endif
		unsigned int jj;
		for (jj=0; jj<(65536); jj+=(65536/NUM_12_OF_CIRCLE_ONE))/* ��� */
		{
		//	REG_02_DEST_X += (sin65536((jj))*(16)); 			/* �e��x256 */
		//	REG_03_DEST_Y += (cos65536((jj))*(16)); 			/* �e��y256 */
			#if (1)//
		//	REG_02_DEST_X += ((si n65536((jj))*((16+(REG_0b_REG3)))));/*fps_factor*/	/* �e��x256 */	/* CCW�̏ꍇ */
		//	REG_03_DEST_Y += ((co s65536((jj))*((16+(REG_0b_REG3)))));/*fps_factor*/	/* �e��y256 */
		//	REG_02_DEST_X += ((si n1024((deg65536to1024(jj)))*((16+(REG_0b_REG3)))));/*fps_factor*/ 	/* �e��x256 */	/* CCW�̏ꍇ */
		//	REG_03_DEST_Y += ((co s1024((deg65536to1024(jj)))*((16+(REG_0b_REG3)))));/*fps_factor*/ 	/* �e��y256 */
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(jj)), &sin_value_t256, &cos_value_t256); 	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
				REG_02_DEST_X += ((sin_value_t256*((12+(REG_0b_REG3)))));/*fps_factor*/ 	/*(r34, 16)*/
				REG_03_DEST_Y += ((cos_value_t256*((12+(REG_0b_REG3)))));/*fps_factor*/ 	/*(r34, 16)*/
			}
			#else/*(�_��)*/
			//------------------
			HATSUDAN_01_speed256	= ((12+(REG_0b_REG3))<<8);
			HATSUDAN_03_angle65536	= (((jj)));
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
		//	REG_09_REG1 -= ( ((REG_03_DEST_Y))); 	/* �e�� */
		//	h->center.y256 = (REG_02_DEST_X);/*fps_factor*/
			//------------------
			//------------------
			// ����
			REG_02_DEST_X = REG_0d_REG5 + (REG_03_DEST_Y);
			REG_03_DEST_Y = REG_0e_REG6 + (REG_02_DEST_X);
			//------------------
			#endif
			unsigned int ii;
			for (ii=0; ii<(65536); ii+=(65536/2))/* ��� */
			{
			// �������
				// �����(��CCW������A�����CCW)
				HATSUDAN_01_speed256			= (t256(0.75)); 							/* �e�� */		/*3.5 2.5 2.0*/
				HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
				HATSUDAN_03_angle65536			= (((REG_09_REG1)+ii)&(65536-1));			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);// 				/* [�g�t���ےe] */
				hatudan_system_regist_single();/* (r33-) */
			// ����
				// �t���(��CCW������A�E���CW)
				HATSUDAN_01_speed256			= (t256(0.50)); 							/* �e�� */		/*3.5 2.5 2.0*/
				HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
				HATSUDAN_03_angle65536			= (((REG_0a_REG2)+ii)&(65536-1));			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				HATSUDAN_05_bullet_obj_type 	= (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);// 				/* [�g�t�� �ےe] */
				hatudan_system_regist_single();/* (r33-) */
			}
		}
		// ��]��
		REG_09_REG1 += (int)(	   (65536/(22)));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
		REG_0a_REG2 += (int)(65536-(65536/(15)));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	// ���p
	//	if ((0x10)==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (32���1�񔭉�) */
	//	if ((0x40)==((REG_10_BOSS_SPELL_TIMER)&0x7f))/* (128���1�񔭉�) */
	//	count_up_limit_NUM(REG_NUM_0c_REG4, 8);//	/* (8���1��) ((16x8)==128���1�񔭉�) */
		count_up_limit_NUM(REG_NUM_0c_REG4, 3);//	/* (2���1��) ((16x3)==48���1�񔭉�) */
		if (1==REG_0c_REG4) /* (48���1��) ���� */
		{
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
}


/*---------------------------------------------------------
	��	�ʏ�U��2
	-------------------------------------------------------
	normal
		��(12)�e	CW�e������								�E��45�x����J�n
		��(12)�e	CCW�e���x��(�e��2.5�{���炢�H)		����45�x����J�n
	-------------------------------------------------------
	luna
		��(12)�e	CW�e������								�E��45�x����J�n
		��(12)�e	CCW�e���x��(�e��1.5�{���炢�H)		����45�x����J�n
	-------------------------------------------------------
	???�B
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa_angle65536
	REG_0a_REG2 	bbb_angle65536
	REG_0c_REG4 	���p�J�E���^�B(40���1��) �����B
---------------------------------------------------------*/

local OBJ_CALL_FUNC(spell_create_23_aya_misogi2)
{
//	if ((0x04)==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (8���1�� ���e) */
	count_up_limit_NUM(REG_NUM_08_REG0, 8);//	/* (8���1�� ���e) */
	if (1==REG_08_REG0) /* (8���1��) ���e */
	{
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* ���(�����12�e) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
		unsigned int jj;
		for (jj=0; jj<(65536);	jj += (65536/NUM_12_OF_CIRCLE_ONE))/* ��� */
		{
		//	REG_02_DEST_X += (sin65536((jj))*(16)); 			/* �e��x256 */
		//	REG_03_DEST_Y += (cos65536((jj))*(16)); 			/* �e��y256 */
			#if (0)//
			REG_02_DEST_X += ((si n1024((deg65536to1024(jj)))*(16)));/*fps_factor*/ 	/* �e��x256 */	/* CCW�̏ꍇ */
			REG_03_DEST_Y += ((co s1024((deg65536to1024(jj)))*(16)));/*fps_factor*/ 	/* �e��y256 */
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(jj)), &sin_value_t256, &cos_value_t256); 	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
				REG_02_DEST_X += ((sin_value_t256*(12)));/*fps_factor*/ 	/*(r34, 16)*/
				REG_03_DEST_Y += ((cos_value_t256*(12)));/*fps_factor*/ 	/*(r34, 16)*/
			}
			#endif
		// [�������]
			// �����(��CCW������A�����CCW)
			HATSUDAN_01_speed256		= (t256(0.75)); 									/* �e�� */		/*3.5 2.5 2.0*/
			HATSUDAN_02_speed_offset	= t256(0);/*(�e�X�g)*/
			HATSUDAN_03_angle65536		= (( (REG_09_REG1))&(65536-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_03_AOI);// 						/* [�g�t���ےe] */
			hatudan_system_regist_single();/* (r33-) */
		// [����]
			// �t���(��CCW������A�E���CW)
			HATSUDAN_01_speed256		= (t256(0.50)); 									/* �e�� */		/*3.5 2.5 2.0*/
			HATSUDAN_02_speed_offset	= t256(1);/*(�e�X�g)*/
			HATSUDAN_03_angle65536		= (( (REG_0a_REG2))&(65536-1)); 				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_05_bullet_obj_type = (BULLET_WAKU12_BASE + TAMA_IRO_01_AKA);// 						/* [�g�t�� �ےe] */
			hatudan_system_regist_single();/* (r33-) */
		}
		// ��]��
	//	REG_09_REG1 += (((int)( 	(65536/(22)))+((int)(65536/(2)))));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	//	REG_0a_REG2 += (((int)(65536-(65536/(15)))+((int)(65536/(2)))));/* ROTATE_ANGLE */ 			/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	//	REG_09_REG1 += (((int)( 	(65536/(44)))+((int)(65536/(2)))));/* ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	//	REG_0a_REG2 += (((int)(65536-(65536/(30)))+((int)(65536/(2)))));/* ROTATE_ANGLE */ 			/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
		// ��]�ʂ� 22����(�A��������, ����44����)�̏ꍇ.
		REG_09_REG1 += ((int)(		  (65536/(44)) + (65536/(2))   ));		/* �p�x(65536[360/360�x]�� 22�����A�A���������ɕ`�� ==	((65536/44)+(65536/2)) ) */
		// ��]�ʂ� 15����(�A��������, ����30����)�̏ꍇ.
		REG_0a_REG2 += ((int)( 65536-( (65536/(30)) + (65536/(2)) ) )); 	/* �p�x(65536[360/360�x]�� 15�����A�A���������ɕ`�� == -((65536/30)+(65536/2)) ) */
	// ���p
	//	if ((0x10)==((REG_10_BOSS_SPELL_TIMER)&0x1f))/* (32���1��) */
	//	if ((0x40)==((REG_10_BOSS_SPELL_TIMER)&0x7f))/* (128���1��) */
		count_up_limit_NUM(REG_NUM_0c_REG4, 5);//	/* (5���1��) ((8x5)==40���1�񔭉�) */
		if (1==REG_0c_REG4) /* (40���1��) ���� */
		{
			#if (1)
			cpu_bullet_play_15_auto();
			#endif
		}
	}
}




/*---------------------------------------------------------
	��	�򕄁u�V�̔��ˁv
	-------------------------------------------------------
	�U�wx32�����e
	-------------------------------------------------------
	�����W�J���A�������E22.5/360�x[1��/16����]�͈̔͂͒e�����Ȃ��B

		 /|X
		/ | X
	   /  |  X
	  /   |   X
	 /	  |    X
	/	  | 	X
	22.5  | 22.5
	�e�����Ȃ��͈� == [1��/16����] + [1��/16����]
	����̃X�N�V�����瑪��Ƃ����B
	-------------------------------------------------------
	�����ǁApsp���Ɗp�x���L��������B
	�㉺�؂�Ă���(�Q�[���t�B�[���h��)���L�̉�ʂ�����A�����������񂾂낤�ȁB
	����(�ڂ̍��o):
	�����l���c�Ȃ̕��Ɖ��Ȃ̕��𒅂��ꍇ�A�c�Ȃ̕��̕���(�S�̂�)�ׂ�������B
	�����J�[�h�ł����E�g���~���O���ăJ�b�g����ƁA�ڂ̍��o�ōׂ�������B
	�����J�[�h�ł��㉺�g���~���O���ăJ�b�g����ƁA�ڂ̍��o�ő���������B
	���̏ꍇ(psp)���������ɓ�������A�ڂ̍��o�ŋɒ[�ɑ���������B
	-------------------------------------------------------
	������Ƃ����āA�p�x�����߂���A�S�̂��k�����Ē�������ƁA�]�v�����Q�ɋ߂Â��C������B
	�l�I�ɂ́A�u����(��)���v����Ԃ�肽�������񂾂��Apsp�̉�ʋ�������A����(r32)����(��)���͖����Q���ۂ��B
	����(��)���͖����Q���ۂ����A����(��)���͂ł���B
	-------------------------------------------------------
	����(��)��:���ɒ���t���Ȃ���A���̗̑͂����U���@�B(�U���n�ȊO��z��B�U���n�ȊO�ōU���ł���)
	����(��)��:�P�ɒe���(��)���邾���A�U���ł��Ȃ��B�U���n�Ȃ�U���ł��邩���m��Ȃ����z��O(����̖͕핗�ł͏o���Ȃ�)�B
	���̃J�[�h�͒P�ɒe���(��)���邾���Ȃ�ȒP�B
	-------------------------------------------------------
	����(r32)�ł͍U�����w�b�^������Ȃ��������ɕς��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(aya_danmaku_01_callback)/* �򕄁u�V�̔��ˁv */
{
	if (((32*18)-HATUDAN_FRAME64)==((REG_0a_REG2) ))/* ��0.33[�b](==20[frame])��~ */
	{
		/* �����ł͌��ʉ��Ȃ� */
		/*(�e���O���[�v(1)��S�e��~�B������ς���B)*/
		{
			src->hatudan_register_tra65536					= t256(0);		/* ���������e */
			src->hatudan_register_speed65536				= t256(0);		/* �e�� */
		}
	}
	else
	if (((32*18)-HATUDAN_FRAME64-20)==((REG_0a_REG2) ))/* �~�܂��Ă���20[frame]�� */ /*(32*17)*/
	{
		/* �ړ��B �ĉ����B */
			/*(�e���O���[�v(1)��S�e����������B)
				-------------------------------------------------------
				�����e(���m�ɂ͓������e)�Ȃ̂ɁA�����e�Ɍ����Ȃ��ŁA
				�����e(���m�ɂ͓����x�e�A����==0)�Ɍ������Ⴄ��肪����B
			*/
		{
			#if 1
			/* �[������==���Z�Œ�l(KOTEITI_AYA_1234) */
		//	#define KOTEITI_AYA_1234 (1234<<6)
		//	#define KOTEITI_AYA_1234 (78976)
			#define KOTEITI_AYA_1234 (7777)
			REG_09_REG1 += (KOTEITI_AYA_1234);
		//	src->rotationCCW1024				= (ra_nd()&((1024)-1)); 	/* ���� */	/* �����͗����łȂ��C������ */
		//	src->rotationCCW1024				= (deg65536to1024(REG_09_REG1));			/* ���� */
		//	src->rotationCCW1024				= ((((deg65536to1024(REG_09_REG1))&(512-1))+(256*(4-1)))&(1024-1)); 		/* ����(��(180/360[�x])�ɂ��������Ȃ�) */
			src->rotationCCW1024				= ((((deg65536to1024(REG_09_REG1))&(256-1))+(128*(8-1)))&(1024-1)); 		/* ����(��( 90/360[�x])�ɂ��������Ȃ�) */
			#endif
			src->hatudan_register_tra65536			= t256(1);	/* (1) ���������e */
		/* �`���m(�p�[�t�F�N�g�t���[�Y)�̏ꍇ�͐�ɕϐg���邪�A��(�V�̔���)�̏ꍇ�͌�ŕϐg����B */
			src->obj_type_set							= (BULLET_MARU10_BASE + TAMA_IRO_03_AOI);	/* �ےe */
			reflect_sprite_spec(src, OBJ_BANK_SIZE_00_TAMA); 	/* �e�O���ƒe�����蔻���ύX����B */
			/* (�ʏ�e�֕ϐg����) */
			src->hatudan_register_spec_data 		= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		}
		/* �~�܂����������A�����n�߂鎞�Ɍ��ʉ��炵������������ۂ��B */
		#if (1)
		AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;/* ���ʉ�: ������ */
	//	AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;/* ���ʉ�: ������ */
		AUDIO_19_voice_truck	= TRACK03_IVENT_DAN;	cpu_voice_play();
		AUDIO_19_voice_truck	= TRACK04_TEKIDAN;		cpu_voice_play();
		/* �厖�Ȏ��Ȃ̂łQ�񌾂��܂����B */
		#endif
	}
//	danmaku_00_standard_angle_mover(src);/*(�p�x�e�ړ�+��ʊO�e����)*/
	hatudan_system_B_move_angle_001(src);/*(�p�x�e�ړ�)*/
}


/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_init_22_aya_yatimata)
{
	card.danmaku_callback[1] = aya_danmaku_01_callback;/*(�򕄁u�V�̔��ˁv�p�B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
//	REG_09_REG1 	add_angle
	REG_0a_REG2 	�{�X�^�C�}�[�l�A�R�[���o�b�N���ɘA���p�B(�Ƃ肠����)
//	REG_0b_REG3 	�ĉ����̑��x�p�Ɉꎞ�g�p�B
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_22_aya_yatimata)
{
	REG_0a_REG2 = (REG_10_BOSS_SPELL_TIMER);/*(�Ƃ肠����)*/
	if ((32*20)-1==((REG_10_BOSS_SPELL_TIMER) ))/* ��1.0[�b](==64[frame])�S�e��W�J */
	{
		/* �e���� */
			HATSUDAN_01_speed256				= (t256(0.5));			/* �e��(psp�̉�ʂ͋���) */
			HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
			HATSUDAN_05_bullet_obj_type 		= (BULLET_WAKU12_BASE + TAMA_IRO_00_SIRO);		/* ���e */
		//
		unsigned int jj;
		for (jj=0; jj<6; jj++)/* �U�w�B */
		{
		//	HATSUDAN_02_speed_offset			= t256(-5); 		/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
		//	HATSUDAN_02_speed_offset			= t256(-jj);/*(�e�X�g)*/
		//	HATSUDAN_02_speed_offset			= t256(jj<<2);/* x4�{ �Ă���*/
			HATSUDAN_02_speed_offset			= (jj<<(2+8));/* x4�{ �Ă���*/
		//	HATSUDAN_03_angle65536				= (65536/16);			/* ���� */
			HATSUDAN_03_angle65536				= (65536/2);							/* [�j��] */	/* �^����� */
			HATSUDAN_06_n_way					= (32); 								/* [�j��] */	/* [32way] */	/* 32�����e */
			HATSUDAN_07_div_angle65536			= /*(1792);*/((65536-(65536/8))/32);	/* [�j��] */	/* �����p�x(�e�����Ȃ��͈͂�����32����) */	/* (ra_nd16>>4) == ����(0-4095) */
			hatudan_system_regist_n_way();/* (r33-) */
		}
		cpu_bullet_play_15_auto();
	}
}


/*---------------------------------------------------------
	�Ǖ��u�R�_�n��v(normal)
	�Ǖ��u�V��V���̏ƚ��v(luna)
	-------------------------------------------------------
	���񂺂����ĂȂ�...
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	aaa
	REG_0a_REG2 	bbb
	REG_0b_REG3 	ggg
	REG_0c_REG4 	zzz
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) ����
local OBJ_CALL_FUNC(spell_create_26_aya_saifu)
{
	if ((SPELL_TIME_9999-1)==((REG_10_BOSS_SPELL_TIMER) ))/* ������ */
	{
		REG_0c_REG4 = (64);/* 64����n�߂�B */
		REG_0b_REG3 = 0;
	}
	REG_0b_REG3--;
	if (0 > REG_0b_REG3)
	{
		REG_0b_REG3 = REG_0c_REG4;
	//	#define min_24zzz (26)/* (r33) */
	//	#define min_24zzz (40)/* (r34)�኱�e�؂ꂷ��B */
		#define min_24zzz (44)/* (r34) */
		REG_0c_REG4 -= (REG_0f_GAME_DIFFICULTY);
		if (min_24zzz > REG_0c_REG4)		{	REG_0c_REG4 = min_24zzz;		}
		#undef min_24zzz
	{
//		HATSUDAN_01_speed256			= (t256(1.00)); 			/* �e�� */
		HATSUDAN_01_speed256			= (t256(0.50)); 			/* �e�� */
//		HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
		HATSUDAN_03_angle65536			= (0);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		{	const unsigned int cc_type[4] =
			{
				(BULLET_MINI8_BASE + TAMA_IRO_05_MIDORI),
				(BULLET_MINI8_BASE + TAMA_IRO_00_SIRO), 		// �Ǖ��u�R�_�n��v(normal)
				(BULLET_KOME_BASE  + TAMA_IRO_06_KI_IRO),
				(BULLET_KOME_BASE  + TAMA_IRO_06_KI_IRO),		// �Ǖ��u�V��V���̏ƚ��v(luna)
			};
			HATSUDAN_05_bullet_obj_type = cc_type[(REG_0f_GAME_DIFFICULTY)];	/* [ �e] */
		}
		HATSUDAN_06_n_way				= (16*5);			/*(6)*/ 	/* ���e�� */
		HATSUDAN_07_div_angle65536		= (int)(65536/(16*5));	/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
	}
//	if ((0x02)==((REG_10_BOSS_SPELL_TIMER)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x04)==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (2���1��)(8�񖈂ɔ��e) */
	{
		REG_0a_REG2 += (77);
		REG_09_REG1++;
		REG_09_REG1 &= (0x07);
		if (0==REG_09_REG1) 	{	REG_09_REG1++;	};
//		HATSUDAN_01_speed256			= (t256(1.00)); 	/* �e�� */
		HATSUDAN_01_speed256			= (t256(0.50)); 	/* �e�� */
//		HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
//		HATSUDAN_03_angle65536			= ((65536/2));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		{	const unsigned int ba_type[4] =
			{
				(BULLET_KOME_BASE	 + TAMA_IRO_03_AOI),
				(BULLET_KOME_BASE	 + TAMA_IRO_03_AOI),	// �Ǖ��u�R�_�n��v(normal)
				(BULLET_UROKO14_BASE + TAMA_IRO_06_KI_IRO),
				(BULLET_UROKO14_BASE + TAMA_IRO_06_KI_IRO), // �Ǖ��u�V��V���̏ƚ��v(luna)
			};
			HATSUDAN_05_bullet_obj_type = ba_type[(REG_0f_GAME_DIFFICULTY)];			/* [ �e] */
		}
	//	HATSUDAN_06_n_way					= (17); 			/*(6)*/ 	/* ���e�� */
		HATSUDAN_06_n_way					= (16); 			/*(8)*/ 	/* ���e�� */
	//	HATSUDAN_07_div_angle65536			= (int)(65536/(16))-(((si n1024(( ra_nd() & REG_10_BOSS_SPELL_TIMER)))&0xff));	/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		HATSUDAN_07_div_angle65536			= (int)(65536/(16/*8*/));	/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
//
//		HATSUDAN_03_angle65536				= (int)((65536*1/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		HATSUDAN_03_angle65536				= ((REG_0a_REG2));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x08))
		{
//			HATSUDAN_03_angle65536			= (int)((65536*3/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_03_angle65536			= -(REG_0a_REG2);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		}
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
}


/*---------------------------------------------------------
	��	�댯�Ȏ�e�J�[�h
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
//tern OBJ_CALL_FUNC(add_zako_aya_doll);/* ���e */
extern OBJ_CALL_FUNC(add_zako_aya_doll);/* ���l�`�J�[�h */
local OBJ_CALL_FUNC(spell_create_0f_aya_doll)
{
	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	{
		add_zako_aya_doll(src);
		#if (1)
		cpu_bullet_play_15_auto();
		#endif
	}
}

#if 0
/*---------------------------------------------------------
	��ʒe
---------------------------------------------------------*/

local OBJ_CALL_FUNC(bullet_create_aya_kougeki_03)
{
	s_bullet_create_aya_oodama3(src, /* �d�l�ύX */(t256(4.0)+((REG_0f_GAME_DIFFICULTY)<<7))/*, 10*/);
}


/*---------------------------------------------------------
	�댯�Ȏ�e
---------------------------------------------------------*/

local OBJ_CALL_FUNC(bullet_create_aya_kougeki_02)
{
	add_zako_aya_doll(src);
};
#endif


/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
#if 0
local OBJ_CALL_FUNC(spell_init_1d_amefuri_test)
{
	REG_09_REG1 	= (t256(1.5)+((REG_0f_GAME_DIFFICULTY)<<6));//[�萔1]�J�̑��x
//	REG_0a_REG2 	= ((1024/2)+(1024/24)+(REG_0f_GAME_DIFFICULTY<<3));//[�萔2]�ԐN�i�C���Ȃ���p�x
//	card.danmaku_callback[1] = meirin_danmaku_02_aka_ao_kunai_callback;/*(�ԐN�i�C�p)*/
	card.danmaku_callback[2] = common_danmaku_01_amefuri_callback;/*(�J�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
#else
	#define spell_init_1d_amefuri_test spell_init_12_common_amefuri
#endif
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------

---------------------------------------------------------*/
#define spell_create_1d_amefuri_test spell_create_common_amefuri

