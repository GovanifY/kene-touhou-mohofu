
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�˖��� ���̒e���𐶐����܂��B
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
---------------------------------------------------------*/

static void spell_create_15_aya_misogi1(SPRITE *src)
{
//	if ((0x10)==((src->boss_base_spell_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if ((0x40)==((src->boss_base_spell_time_out)&0x7f))/* (16���1��)(128�Ȃ�v8��) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	}
	if ((0x08)==((src->boss_base_spell_time_out)&0x0f))/* (16���1��)(8�񖈂ɔ��e) */
	{
		static int aaa_angle65536;
		static int bbb_angle65536;
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_NON_ANGLE_TILT;/* (r33-)��X���e */
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* ���(�����12�e) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
	//	const unsigned int aaa = (((~(src->boss_base_spell_time_out))>>3)&0x1f);/* �e�L�g�[�ɑ傫�����Ă݂�B */
		const unsigned int aaa = (((~(src->boss_base_spell_time_out))>>3)&0x1f);/* �e�L�g�[�ɑ傫�����Ă݂�B */
		unsigned int jj;
		for (jj=0; jj<(65536); jj+=(65536/NUM_12_OF_CIRCLE_ONE))/* ��� */
		{
		//	obj_send1->cx256					= (src->cx256) +(sin65536((jj))*16);				/* �e��x256 */
		//	obj_send1->cy256					= (src->cy256) +(cos65536((jj))*16);				/* �e��y256 */
			obj_send1->cx256					= (src->cx256) +(sin65536((jj))*(16+(aaa)));		/* �e��x256 */
			obj_send1->cy256					= (src->cy256) +(cos65536((jj))*(16+(aaa)));		/* �e��y256 */
			unsigned int ii;
			for (ii=0; ii<(65536); ii+=(65536/2))/* ��� */
			{
			//�������
				// �����(��CCW������A�����CCW)
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_WAKU12_03_AOI;//						/* [�g�t���ےe] */
				br.BULLET_REGIST_02_angle65536			= (((aaa_angle65536)+ii)&(65536-1));			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				br.BULLET_REGIST_00_speed256			= (t256(0.75)); 								/* �e�� */		/*3.5 2.5 2.0*/
				br.BULLET_REGIST_01_speed_offset		= t256(0);/*(�e�X�g)*/
				tama_system_regist_single();/* (r33-) */
			//����
				// �t���(��CCW������A�E���CW)
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_WAKU12_01_AKA;//						/* [�g�t�� �ےe] */
				br.BULLET_REGIST_02_angle65536			= (((bbb_angle65536)+ii)&(65536-1));			/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				br.BULLET_REGIST_00_speed256			= (t256(0.50)); 								/* �e�� */		/*3.5 2.5 2.0*/
				br.BULLET_REGIST_01_speed_offset		= t256(1);/*(�e�X�g)*/
				tama_system_regist_single();/* (r33-) */
			}
		}
		// ��]��
		aaa_angle65536 += (int)(	  (65536/(22)));/* HOUGA_16_YEL_ROTATE_ANGLE */ 				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
		bbb_angle65536 += (int)(65536-(65536/(15)));/* HOUGA_16_YEL_ROTATE_ANGLE */ 				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
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
---------------------------------------------------------*/

static void spell_create_23_aya_misogi2(SPRITE *src)
{
//	if ((0x10)==((src->boss_base_spell_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if ((0x40)==((src->boss_base_spell_time_out)&0x7f))/* (16���1��)(128�Ȃ�v8��) */
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	}
	if ((0x04)==((src->boss_base_spell_time_out)&0x07))/* (8���1��)(8�񖈂ɔ��e) */
	{
		static int aaa_angle65536;
		static int bbb_angle65536;
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* ��X���e�ɐݒ肷��(r33-) */
//
	//	#define NUM_12_OF_CIRCLE_ONE	(12)/* ���(�����12�e) */
		#define NUM_12_OF_CIRCLE_ONE	(9)
		unsigned int jj;
		for (jj=0; jj<(65536);	jj += (65536/NUM_12_OF_CIRCLE_ONE))/* ��� */
		{
			obj_send1->cx256					= (src->cx256) +(sin65536((jj))*16);				/* �e��x256 */
			obj_send1->cy256					= (src->cy256) +(cos65536((jj))*16);				/* �e��y256 */
		// [�������]
			// �����(��CCW������A�����CCW)
			br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_WAKU12_03_AOI;//							/* [�g�t���ےe] */
			br.BULLET_REGIST_02_angle65536			= (( (aaa_angle65536))&(65536-1));					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_00_speed256			= (t256(0.75)); 									/* �e�� */		/*3.5 2.5 2.0*/
			br.BULLET_REGIST_01_speed_offset		= t256(0);/*(�e�X�g)*/
			tama_system_regist_single();/* (r33-) */
		// [����]
			// �t���(��CCW������A�E���CW)
			br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_WAKU12_01_AKA;//							/* [�g�t�� �ےe] */
			br.BULLET_REGIST_02_angle65536			= (( (bbb_angle65536))&(65536-1));					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_00_speed256			= (t256(0.50)); 									/* �e�� */		/*3.5 2.5 2.0*/
			br.BULLET_REGIST_01_speed_offset		= t256(1);/*(�e�X�g)*/
			tama_system_regist_single();/* (r33-) */
		}
		// ��]��
	//	aaa_angle65536 += (((int)(		(65536/(22)))+((int)(65536/(2)))));/* HOUGA_16_YEL_ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	//	bbb_angle65536 += (((int)(65536-(65536/(15)))+((int)(65536/(2)))));/* HOUGA_16_YEL_ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	//	aaa_angle65536 += (((int)(		(65536/(44)))+((int)(65536/(2)))));/* HOUGA_16_YEL_ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
	//	bbb_angle65536 += (((int)(65536-(65536/(30)))+((int)(65536/(2)))));/* HOUGA_16_YEL_ROTATE_ANGLE */				/*(1024/(6*8))*/		/* �p�x(1024[360/360�x]�� 48����) */
		// ��]�ʂ� 22����(�A��������, ����44����)�̏ꍇ.
		aaa_angle65536 += ((int)(		  (65536/(44)) + (65536/(2))   ));		/* �p�x(65536[360/360�x]�� 22�����A�A���������ɕ`�� ==	((65536/44)+(65536/2)) ) */
		// ��]�ʂ� 15����(�A��������, ����30����)�̏ꍇ.
		bbb_angle65536 += ((int)( 65536-( (65536/(30)) + (65536/(2)) ) ));		/* �p�x(65536[360/360�x]�� 15�����A�A���������ɕ`�� == -((65536/30)+(65536/2)) ) */
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
	�����e���ł����E�g���~���O���ăJ�b�g����ƁA�ڂ̍��o�ōׂ�������B
	�����e���ł��㉺�g���~���O���ăJ�b�g����ƁA�ڂ̍��o�ő���������B
	���̏ꍇ(psp)���������ɓ�������A�ڂ̍��o�ŋɒ[�ɑ���������B
	-------------------------------------------------------
	������Ƃ����āA�p�x�����߂���A�S�̂��k�����Ē�������ƁA�]�v�����Q�ɋ߂Â��C������B
	�l�I�ɂ́A�u����(��)���v����Ԃ�肽�������񂾂��Apsp�̉�ʋ�������A����(r32)����(��)���͖����Q���ۂ��B
	����(��)���͖����Q���ۂ����A����(��)���͂ł���B
	-------------------------------------------------------
	����(��)��:���ɒ���t���Ȃ���A���̗̑͂����U���@�B(�U���n�ȊO��z��B�U���n�ȊO�ōU���ł���)
	����(��)��:�P�ɒe���(��)���邾���A�U���ł��Ȃ��B�U���n�Ȃ�U���ł��邩���m��Ȃ����z��O(����̖͕핗�ł͏o���Ȃ�)�B
	���̒e���͒P�ɒe���(��)���邾���Ȃ�ȒP�B
	-------------------------------------------------------
	����(r32)�ł͍U�����w�b�^������Ȃ��������ɕς��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	��	�򕄁u�V�̔��ˁv(�g��12�ےe��S�e��~�B���݂̍��W��V��_���W�ɕύX����B������ς���)
	-------------------------------------------------------
---------------------------------------------------------*/

static void exchange_damnaku_aya_tomeru(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* �g��12�ےe�Ȃ� */
		{
			s->tama_system_tra65536 					= t256(0);		/* ���������e */
			s->tama_system_speed65536					= t256(0);		/* �e�� */
		}
	}
}


/*---------------------------------------------------------
	��	�򕄁u�V�̔��ˁv(�g��12�ےe��S�e����������)
	-------------------------------------------------------
	�����e(���m�ɂ͓������e)�Ȃ̂ɁA�����e�Ɍ����Ȃ���
	�����e(���m�ɂ͓����x�e�A����==0)�Ɍ������Ⴄ��肪����B

---------------------------------------------------------*/

static void exchange_damnaku_aya_saikasoku(void)
{
//	int check_type;
//	check_type = br.BULLET_REGIST_04_bullet_obj_type;/* ���ׂ�^�C�v���󂯎�� */
//	int jj = 0;
	int add_angle = 0;
	int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++ )/* �S�����ׂ�B */
	{
		SPRITE *s;
		s = &obj99[OBJ_HEAD_00_TAMA+ii];
	//	if (check_type == (s->type) )	/* ���̐e���Ԓe�Ȃ� */
		if (is_tama_grouip08(BULLET_WAKU12_00_SIRO) == is_tama_grouip08(s->type) )	/* �g��12�ےe�Ȃ� */
		{
		//	s_change_meirin_yajirusi_one(s);
			/* �e�̌��ݍ��W���A�V��_���W�Ƃ���B */
			s->tx256 = (s->cx256);		/* ��_���W�ɃZ�b�g */
			s->ty256 = (s->cy256);		/* ��_���W�ɃZ�b�g */
			s->tama_system_radius256 = t256(0); 	/* ��_���W�ɂȂ����̂Ŕ��a�� 0 �ɂ���B */
			/*	*/
			#if 1
			/* ���Ȃ��Ƃ��������͖{���ɋ߂��B ���A�S�R�Ⴄ�B */
			/* ���Z�Œ�l(KOTEITI_7777)�ȊO�̗v���Ƃ��āA�e�����X�g�̃T�C�Y�Ń��[�v����ׁA�e�����X�g�̃T�C�Y�� */
			/* ���݂� 1024 ����A�{���Ɠ��� 640 �Ɍ��点�΁A����\���͂���B */
		//	#define KOTEITI_1234 (1234<<6)
		//	#define KOTEITI_1234 (78976)
			#define KOTEITI_1234 (7777)
			add_angle += (KOTEITI_1234);
		//	s->rotationCCW1024				= (ra_nd()&((1024)-1)); 			/* ���� */	/* �����͗����łȂ��C������ */
		//	s->rotationCCW1024				= (add_angle>>6);			/* ���� */
		//	s->rotationCCW1024				= ((((add_angle>>6)&(512-1))+(256*(4-1)))&(1024-1));			/* ����(��(180/360[�x])�ɂ��������Ȃ�) */
			s->rotationCCW1024				= ((((add_angle>>6)&(256-1))+(128*(8-1)))&(1024-1));			/* ����(��( 90/360[�x])�ɂ��������Ȃ�) */
			#endif
			s->tama_system_tra65536 					= t256(1);	/* (1) ���������e */
		//	�`���m(�p�[�t�F�N�g�t���[�Y)�̏ꍇ�͐�ɕϐg���邪�A��(�V�̔���)�̏ꍇ�͌�ŕϐg����B
			s->type 						= (BULLET_MARU10_03_AOI);	/* �ےe */
			reflect_sprite_spec444(s, OBJ_BANK_SIZE_00_TAMA);	/* �e�O���ƒe�����蔻���ύX����B */
		}
	}
}

static void spell_create_22_aya_yatimata(SPRITE *src)
{
	if ((32*20)-1==((src->boss_base_spell_time_out) ))/* ��1.0[�b](==64flame)�e���W�J */
	{
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		//
			obj_send1->cx256					= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
			obj_send1->cy256					= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		/* �e���� */
		unsigned int j;
		for (j=0; j<6; j++ )/* �U�w�B */
		{
			br.BULLET_REGIST_00_speed256				= (t256(0.5) ); 			/* �e��(psp�̉�ʂ͋���) */
		//	br.BULLET_REGIST_01_speed_offset			= t256(-5); 		/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
		//	br.BULLET_REGIST_01_speed_offset			= t256(-j);/*(�e�X�g)*/
		//	br.BULLET_REGIST_01_speed_offset			= t256(j<<2);/* x4�{ �Ă���*/
			br.BULLET_REGIST_01_speed_offset			= (j<<(2+8));/* x4�{ �Ă���*/
		//	br.BULLET_REGIST_02_angle65536				= (65536/16)/* */;			/* ���� */
			br.BULLET_REGIST_02_angle65536				= (65536/2)/* */;			/* �^����� */
			br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_00_SIRO;	/* �ےe */
		//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_01_ANGLE_NON_TILT;/* ��X���e�ɐݒ肷��(r33-) */
			br.BULLET_REGIST_06_n_way					= (32); 		/* [32way] */			/* 32�����e */
			br.BULLET_REGIST_07_div_angle65536			= /*(1792);*/((65536-(65536/8))/32);				/* �����p�x(�e�����Ȃ��͈͂�����32����) */	/* (ra_nd16>>4) == ����(0-4095) */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	else
	if (((32*18)-HATUDAN_FRAME64)==((src->boss_base_spell_time_out) ))/* ��0.33[�b](==20flame)��~ */
	{
		/* �����ł͌��ʉ��Ȃ� */
		/* �S�e��~ */
		exchange_damnaku_aya_tomeru();/*�S�ăX�g�b�v*/
	}
	else
	if (((32*18)-HATUDAN_FRAME64-20)==((src->boss_base_spell_time_out) ))/* �~�܂��Ă���20flame�� */ /*(32*17)*/
	{
		/* �~�܂����������A�����n�߂鎞�Ɍ��ʉ��炵������������ۂ��B */
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
	//	bullet_play_04_auto(VOICE11_BOSS_KIRARIN);		/* ���ʉ�: ������ */
		voice_play(VOICE11_BOSS_KIRARIN, TRACK03_SHORT_MUSIC);		/* ���ʉ�: ������ */
		voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);			/* ���ʉ�: ������ */
		/* �厖�Ȏ��Ȃ̂łQ�񌾂��܂����B */
		#endif
		/* �ړ� */
		exchange_damnaku_aya_saikasoku();/*�ĉ���*/
	}

}



/*---------------------------------------------------------
	�Ǖ��u�R�_�n��v(normal)
	�Ǖ��u�V��V���̏ƚ��v(luna)
	-------------------------------------------------------
	���񂺂����ĂȂ�...
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) ����
static void spell_create_26_aya_saifu(SPRITE *src)
{
	static unsigned int zzz;
	static	 signed int ggg;
	if ((SPELL_TIME_9999-1)==((src->boss_base_spell_time_out) ))/* ������ */
	{
		zzz = 64;
		ggg = 0;
	}
	ggg--;
	if (0 > ggg)
	{
		ggg = zzz;
		#define min_24zzz (26)
		zzz -= (cg_game_difficulty);
		if (min_24zzz > zzz)		{	zzz = min_24zzz;		}
		#undef min_24zzz
	{
		obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
//		br.BULLET_REGIST_00_speed256				= (t256(1.00)); 										/* �e�� */
		br.BULLET_REGIST_00_speed256				= (t256(0.50)); 										/* �e�� */
//		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
		br.BULLET_REGIST_02_angle65536				= ((0));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		{	const unsigned int cc_type[4] =
			{
				BULLET_MINI8_05_MIDORI,
				BULLET_MINI8_00_SIRO,			// �Ǖ��u�R�_�n��v(normal)
				BULLET_KOME_06_KI_IRO,
				BULLET_KOME_06_KI_IRO,			// �Ǖ��u�V��V���̏ƚ��v(luna)
			};
			br.BULLET_REGIST_04_bullet_obj_type 	= cc_type[(cg_game_difficulty)];								/* [ �e] */
		}
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= (16*5);			/*(6)*/ 				/* [1way] */	/* ���e�� */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16*5));	/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
	}
//	if ((0x02)==((src->boss_base_spell_time_out)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x04)==((src->boss_base_spell_time_out)&0x07))/* (2���1��)(8�񖈂ɔ��e) */
	{
		obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
		static unsigned int bbb;
		bbb += 77;
		static unsigned int aaa;
		aaa++;
		aaa &= (0x07);
		if (0==aaa) 	{	aaa++;	};
//		br.BULLET_REGIST_00_speed256				= (t256(1.00)); 	/* �e�� */
		br.BULLET_REGIST_00_speed256				= (t256(0.50)); 	/* �e�� */
//		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
//		br.BULLET_REGIST_02_angle65536				= ((65536/2));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		{	const unsigned int ba_type[4] =
			{
				BULLET_KOME_03_AOI,
				BULLET_KOME_03_AOI, 		// �Ǖ��u�R�_�n��v(normal)
				BULLET_UROKO14_06_KI_IRO,
				BULLET_UROKO14_06_KI_IRO,	// �Ǖ��u�V��V���̏ƚ��v(luna)
			};
			br.BULLET_REGIST_04_bullet_obj_type 	= ba_type[(cg_game_difficulty)];			/* [ �e] */
		}
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
	//	br.BULLET_REGIST_06_n_way					= (17); 			/*(6)*/ 	/* [1way] */	/* ���e�� */
		br.BULLET_REGIST_06_n_way					= (16/*8*/);		/*(6)*/ 	/* [1way] */	/* ���e�� */
	//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16))-(((sin1024(( ra_nd() & src->boss_base_spell_time_out)))&0xff)); 	/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16/*8*/));	/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
//
//		br.BULLET_REGIST_02_angle65536				= (int)((65536*1/4))+(src->boss_base_spell_time_out&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_02_angle65536				= ((bbb));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		if (0==((src->boss_base_spell_time_out)&0x08))
		{
//			br.BULLET_REGIST_02_angle65536			= (int)((65536*3/4))+(src->boss_base_spell_time_out&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_02_angle65536			= -(bbb);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		}
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}

/*---------------------------------------------------------
	��	�댯�Ȏ�e	�l�`�e��
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
extern void add_zako_aya_doll(SPRITE *src);/* ���l�`�e�� */
static void spell_create_0f_aya_doll(SPRITE *src)
{
	if (50==((src->boss_base_spell_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_aya_doll(src);
	}
}

#if 0
/*---------------------------------------------------------
	��ʒe
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_03(SPRITE *src)
{
	s_bullet_create_aya_oodama3(src, /*�d�l�ύX*/(t256(4.0)+((cg_game_difficulty)<<7))/*, 10*/);
}


/*---------------------------------------------------------
	�댯�Ȏ�e
---------------------------------------------------------*/

static void bullet_create_aya_kougeki_02(SPRITE *src)
{
	add_zako_aya_doll(src);
};
#endif

/*---------------------------------------------------------
	�J�~��e
	-------------------------------------------------------
---------------------------------------------------------*/

static void spell_create_1d_amefuri_test(SPRITE *src)
{
	if (0==((src->boss_base_spell_time_out)&0x03))	// 4�J�E���g��1����8way�e������
	{
		obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
		unsigned int j;
		for (j=(0); j<(8); j++)
		{
			/* �V���b�g */
		//	b05_fire_flags &= (~(doll_data->identity_bit));/* off */
			br.BULLET_REGIST_00_speed256						= (t256(1.5)+((cg_game_difficulty)<<6));
			br.BULLET_REGIST_02_VECTOR_angle1024				= (1024/4)+(1024/8)+(j<<5)+((src->boss_base_spell_time_out>>2)&0x3f);
//	//		br.BULLET_REGIST_04_bullet_obj_type 				= BULLET_CAP16_04_KOME_SIROI;			/* [���Ēe] */	/* �e�O�� */
			br.BULLET_REGIST_04_bullet_obj_type 				= (BULLET_KOME_03_AOI); 	/* �e�O�� */
			br.BULLET_REGIST_05_regist_type 					= REGIST_TYPE_02_GRAVITY02;
	//		br.BULLET_REGIST_06_n_way							= (8);					/*(4-difficulty)*/
//			br.BULLET_REGIST_07_VECTOR_div_angle1024			= (int)(1024/64);
			br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
			bullet_regist_vector();
		}
	}
}
