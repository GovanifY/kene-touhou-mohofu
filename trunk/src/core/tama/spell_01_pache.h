
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�`�����[�E�m�[���b�W�̒e���𐶐����܂��B
---------------------------------------------------------*/

static void test_pache_16_shot_AKA(SPRITE *src)
{
	set_sub_jikinerai(src);
	//
	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16));	/* �����p�x(1024[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
	br.BULLET_REGIST_06_n_way					= (16); 	/* [16way] */	/* ���e�� */
	//
	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_01_AKA;//BULLET_WAKU12_01_AKA;//							/* [�Ԋےe] */
//	br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
//	br.BULLET_REGIST_01_speed_offset			= t256(6);/*(�e�X�g)*/
	unsigned int jj;
	for (jj=0; jj<(4);	jj ++)/* 4�e */
	{
		br.BULLET_REGIST_01_speed_offset		= (3+jj)<<8;/*(�e�X�g)*/
		br.BULLET_REGIST_00_speed256			= (t256(1.00)+(jj<<7)); 		/* �e�� */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}
static void test_pache_16_shot_AOI(SPRITE *src)
{
	set_sub_jikinerai(src);
	//
	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16));	/* �����p�x(1024[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
	br.BULLET_REGIST_06_n_way					= (16); 	/* [16way] */	/* ���e�� */
	//
	br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
	br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_03_AOI;//BULLET_WAKU12_01_AKA;//							/* [�Ԋےe] */
//	br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
//	br.BULLET_REGIST_01_speed_offset			= t256(6);/*(�e�X�g)*/
	unsigned int jj;
	for (jj=0; jj<(4);	jj ++)/* 4�e */
	{
		br.BULLET_REGIST_01_speed_offset		= (3+jj)<<8;/*(�e�X�g)*/
		br.BULLET_REGIST_00_speed256			= (t256(1.00)+(jj<<7)); 		/* �e�� */
		tama_system_regist_katayori_n_way();/* (r33-) */
	}
}
/*
�����w�̈ʒu����(�Ԋےe)���@�_���e(2��)

*/
extern void add_zako_pache_laser(SPRITE *src);/* �A���X�l�`�e�� */
static void spell_create_42_pache_laser1(SPRITE *src)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 = (0+(65536/6)); 		/* 1/8��(60/360�x) */
		}
		else
		{
			lz[0].laser_angle65536 = (65536-(65536/6)); 	/* -1/8��(60/360�x) */
		}
	}
	else
	if (100<laser_count)
	{
		;/*��~*/
	}
	else
	{
		;/* �ړ� **/
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 -= (100);
		}
		else
		{
			lz[0].laser_angle65536 += (100);
		}
	}

	if ((65)>((src->boss_spell_timer)&0xff))/* 5�� 3�� */
	{
		if ((0)==((src->boss_spell_timer)&0x0f))
		{
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
			test_pache_16_shot_AKA(src);
		}
	}
	#if 0
	/*0x280*/if ((640-1)==((src->boss_spell_timer) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
	}
	/*0x200*/if ((640-128)==((src->boss_spell_timer) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
	}
	#endif
}
static void spell_create_43_pache_laser2(SPRITE *src)
{
	static int laser_muki=0;
	static int laser_count=0;
	laser_count--;
	if (0>laser_count)
	{
		laser_count = (150);
		laser_muki ^= 1;
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 = (0+(65536/6)); 		/* 1/8��(60/360�x) */
		}
		else
		{
			lz[0].laser_angle65536 = (65536-(65536/6)); 	/* -1/8��(60/360�x) */
		}
	}
	else
	if (100<laser_count)
	{
		;/*��~*/
	}
	else
	{
		;/* �ړ� **/
		if (0==(laser_muki&1))
		{
			lz[0].laser_angle65536 -= (100);
		}
		else
		{
			lz[0].laser_angle65536 += (100);
		}
	}

	if ((65)>((src->boss_spell_timer)&0xff))/* 9�� 5�� 3�� */
	{
		if ((0)==((src->boss_spell_timer)&0x07))
		{
			#if (1)
		//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
			bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
			#endif
			test_pache_16_shot_AOI(src);
		}
	}
	#if 0
	/*0x280*/if ((640-1)==((src->boss_spell_timer) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
		test_pache_16_shot_AKA(src);
	}
	/*0x200*/if ((640-128)==((src->boss_spell_timer) ))
	{
		#if (1)
	//	voice_play(VOICE15_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE15_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
	}
	#endif
}



/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e��~�B���݂̍��W��V��_���W�ɕύX����B������ς���)
	-------------------------------------------------------
	��芸�����A�ےe�p�B
---------------------------------------------------------*/
#define PACHE_TEST (0)
#if (1==PACHE_TEST)
static void exchange_damnaku_pache_sin_sokudo_maru_dan(void)
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
		//	s->tama_system_tra65536 		= t256(0);		/* ���������e */
			s->tama_system_flame_counter++;
			s->tama_system_speed65536		= ((sin1024(s->tama_system_flame_counter) + t256(1.00))<<8);		/* �e�� */
		//	s->type 						= (BULLET_WAKU12_00_SIRO);
			reflect_sprite_spec444(s, OBJ_BANK_SIZE_00_TAMA);	/* �e�O���ƒe�����蔻���ύX����B */
		}
	}
}
#endif

/*---------------------------------------------------------
	�p�[�t�F�N�g�t���[�Y(�g��12�ےe��S�e��~�B���݂̍��W��V��_���W�ɕύX����B������ς���)
	-------------------------------------------------------
	���e�͖����̂ŁA��芸�����A�ς��ɐԗؒe�g���B
---------------------------------------------------------*/
//			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_UROKO14_01_AKA;				/* [�ԗؒe] */
#if 0
static void exchange_damnaku_pache_sin_sokudo_honou_dan(void)
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
		//	s->tama_system_tra65536 		= t256(0);		/* ���������e */
			s->tama_system_flame_counter++;
			s->tama_system_speed65536		= sin1024(s->tama_system_flame_counter) + t256(1.00);		/* �e�� */
		//	s->type 						= (BULLET_WAKU12_00_SIRO);
			reflect_sprite_spec444(s, OBJ_BANK_SIZE_00_TAMA);	/* �e�O���ƒe�����蔻���ύX����B */
		}
	}
}
#endif

/*---------------------------------------------------------
	[�p�`�F] "�@�@�@�@�Ε��u�A�O�j�V���C���v" No.15
	-------------------------------------------------------
	�A�O�j�V���C���̉��e�͎G���ɂ������������I�����H�B
---------------------------------------------------------*/
static void spell_create_2e_pache_agni_shine_1(SPRITE *src)
{
}


/*---------------------------------------------------------
	[�p�`�F] "�@�����u�v�����Z�X�E���f�B�l�v" No.16
	-------------------------------------------------------
	���̒e���̌��ȏ��́A���e�̂����蔻�肪(�����������)�ł������B
	(���e�̂����蔻��͏������̂�)
	�����璆�e�͌����ڂ��댯�B
	�Ȃ񂾂��ǁA���݂̖͕핗�ɒ��e�������ȁ[�B
	�ےe��(��ʒe�A�~�j�e�A���e�A�g���e)���������B
	���e�͂����蔻����傫�����A�Ƃ肠�����G���ɕύX���悤���ȁH
	-------------------------------------------------------
	���[�U�[���A�Ԋu0.5s���炢(6��)�v3.0s?(8��?)
	0.3s���炢�x��
	��ǂ��e�A�Ԋu0.3s���炢(6��)�v2.0s?
	2.0s���炢�x��
	----
	8.0s==60x8==480(?)
	6.0s==60x6==360(?)
	----
	�Ƃ������Ɋ�����̂����A�����Ԃ�1 loop 4.0s���炢���ۂ��B
---------------------------------------------------------*/

static void spell_create_2f_pache_princess_undine(SPRITE *src)
{
	static int temp00_angle;
	if (64 > ((src->boss_spell_timer) ))
	{
		;
	}
	else
	if (128 > ((src->boss_spell_timer) ))
	{
		if (0==((src->boss_spell_timer)&(16-1)))
		{
			/* ���@�_�� �p���쐬 */
			set_sub_jikinerai(src);
		//	temp00_angle = br.BULLET_REGIST_02_angle65536;
			/* ���@�_�� 3way ��laser�e */
		//	br.BULLET_REGIST_02_angle65536				= temp00_angle;
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* �e�� */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(1.00) );		/* �e�� */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_OODAMA32_00_AOI;// 						/* [�e] */
			br.BULLET_REGIST_06_n_way					= (11); 	/* [3way] */	/* ���e�� */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(24)); /* �����p�x(65536[360/360�x]�� 16 ����) */ /* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	else
	if (250 > ((src->boss_spell_timer) ))
	{
		if (0==((src->boss_spell_timer)&(2-1)))
		{
			/* ���@�_�� 3way ��laser�e */
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* �e�� */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(4.00) );		/* �e�� */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_HARI32_00_AOI;//							/* [�j�e] */
			/* 1���[�U�[�̒��� */
			#define pache_02_LASER_LENGTH (4)
			int jj;
		jj=0;// for (jj=0; jj<(128*pache_02_LASER_LENGTH); jj+=128)
			{
				br.BULLET_REGIST_02_angle65536		= temp00_angle;
				br.BULLET_REGIST_06_n_way				= (3);	/* [3way] */	/* ���e�� */
				/* �S���̃X�L�}(���ԑS��)�A���Ԃ����鎞��time_out��128������B */
				#define pache_00_SUKIMA_ZENBU		((128/*time_out*/)<<4)/*(4096)*/
				/* �v���C���[��������X�L�} */
				#define pache_01_SUKIMA_KUGURU		(1280)
				br.BULLET_REGIST_07_div_angle65536	= jj+((src->boss_spell_timer)<<4)-(pache_00_SUKIMA_ZENBU-pache_01_SUKIMA_KUGURU) ;//(int)(65536/(16));	/* �����p�x(65536[360/360�x]�� 16 ����) */	/* 1����n���������p�x */
				tama_system_regist_n_way();/* (r33-) */
			}
			if (0==((src->boss_spell_timer)&(32-1)))
			{
				/* ���@�_��Ȃ� 16way(?)17way(???) �ےe */
				br.BULLET_REGIST_00_speed256				= (t256(0.50) );		/* �e�� */
				br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
				br.BULLET_REGIST_02_angle65536				= temp00_angle + (65536/2);
				br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_03_AOI;//							/* [�ےe] */
				br.BULLET_REGIST_06_n_way					= (17); 	/* [17way] */	/* ���e�� */
				br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(19));	/* �����p�x(65536[360/360�x]�� 24 ����) */	/* 1����n���������p�x */
				tama_system_regist_n_way();/* (r33-) */
			}
		}
	}
	else
	if (254 == ((src->boss_spell_timer) ))
	{
		/* ���@�_�� �p���쐬 */
		set_sub_jikinerai(src);
		temp00_angle = br.BULLET_REGIST_02_angle65536;
	}
}
//			br.BULLET_REGIST_01_speed_offset		= t256(1);/*(�e�X�g)*/
//			br.BULLET_REGIST_01_speed_offset		= t256(6);/*(�e�X�g)*/


/*---------------------------------------------------------
[�p�`�F] "�@�@�@�@�ؕ��u�V���t�B�z�����v" No.17
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_30_pache_sylphy_horn_1(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�y���u���C�W�B�g�����g���v" No.18
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_31_pache_rage_tririton_1(SPRITE *src)
{
	#if (1==PACHE_TEST)
	// �ŏ��̔��� (64�t���[��==��1�b)�΂�T���B
	if (((64))<(src->boss_spell_timer)) // 192== (64*3)
	{
			/* ���@�_�� 3way ��laser�e */
			br.BULLET_REGIST_02_angle65536				= ((src->boss_spell_timer));
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* �e�� */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(1.00) );		/* �e�� */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_03_AOI;// 						/* ���F�������̂Ŏd���Ȃ�[�e] */
			br.BULLET_REGIST_06_n_way					= (1);  	/* [3way] */	/* ���e�� */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(24)); /* �����p�x(65536[360/360�x]�� 16 ����) */ /* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
	}
	if ((0)==((src->boss_spell_timer)&4)) // 4���1��`�F�b�N
	{
		exchange_damnaku_pache_sin_sokudo_maru_dan();
	}
	#endif
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�����u���^���t�@�e�B�[�O�v" No.19
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_32_pache_metal_fatigue(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�Ε��u�A�O�j�V���C���㋉�v" No.20
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_33_pache_agni_shine_2(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�ؕ��u�V���t�B�z�����㋉�v" No.21
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_34_pache_sylphy_horn_2(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�y���u���C�W�B�g�����g���㋉�v" No.22
	-------------------------------------------------------
	�Ă���
---------------------------------------------------------*/
static void spell_create_35_pache_rage_tririton_2(SPRITE *src)	{	}
//#define spell_create_35_pache_rage_tririton_2 spell_create_31_pache_rage_tririton_1


/*---------------------------------------------------------
[�p�`�F] "�@�Ε��u�A�O�j���C�f�B�A���X�v" No.23
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_36_pache_agni_radiance(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�@�����u�x���[�C�����C�N�v" No.24
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_37_pache_bury_in_lake(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�@�ؕ��u�O���[���X�g�[���v" No.25
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_38_pache_green_storm(SPRITE *src)
{
}


/*---------------------------------------------------------
[�p�`�F] "�@�@�y���u�g�����g���V�F�C�N�v" No.26
	-------------------------------------------------------
	�Ă���
---------------------------------------------------------*/
static void spell_create_39_pache_tririton_shake(SPRITE *src)	{	}
//#define spell_create_39_pache_tririton_shake spell_create_31_pache_rage_tririton_1

/*---------------------------------------------------------
[�p�`�F] "�@�@�@�����u�V���o�[�h���S���v" No.27
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3a_pache_silver_dragon(SPRITE *src)
{
}


/*---------------------------------------------------------
	[�p�`�F] "�΁��y���u���[���@�N�������N�v" No.28
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3b_pache_lava_cromlech(SPRITE *src)
{
}


/*---------------------------------------------------------
	[�p�`�F] "�؁��Ε��u�t�H���X�g�u���C�Y�v" No.29
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3c_pache_forest_blaze(SPRITE *src)
{
}


/*---------------------------------------------------------
	[�p�`�F] "�@�����ؕ��u�E�H�[�^�[�G���t�v" No.30
	-------------------------------------------------------
	�����悭�킩���B
	�p�`�F���ۂ��H�B
---------------------------------------------------------*/
static void spell_create_3d_pache_water_elf(SPRITE *src)
{
	/* ���� ���@�_�� 24�����S���ʁA���F�e BULLET_MARU10_04_MIZU_IRO */
	/* �ؕ� ���@�_�� 20�����S���ʁA�ΐF�e BULLET_MARU10_05_MIDORI */
	/* �ؕ� ���@�_�� 10���������A���ʒe BULLET_OODAMA32_00_AOI */
	static int temp11_angle;
	temp11_angle += (src->boss_spell_timer<<4);
	if (0==((src->boss_spell_timer)&(16-1)))
	{
		int temp00_angle;
		/* ���@�_�� �p���쐬 */
		set_sub_jikinerai(src);
		temp00_angle = br.BULLET_REGIST_02_angle65536;
		temp11_angle += (temp00_angle);
		if (32>((src->boss_spell_timer)&(128-1)))
		{
			/* �ؕ� ���@�_�� 10���������A���ʒe BULLET_OODAMA32_00_AOI */
			br.BULLET_REGIST_02_angle65536				= temp00_angle;
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* �e�� */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(1.00) );		/* �e�� */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_OODAMA32_00_AOI;// 						/* [�e] */
			br.BULLET_REGIST_06_n_way					= (10); 	/* [10way] */	/* ���e�� */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(20));	/* �����p�x(65536[360/360�x]�� 20 ����) */	/* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
		}
		if (0x0a90&(temp11_angle))
		{
			/* �ؕ� ���@�_�� 20�����S���ʁA�ΐF�e BULLET_MARU10_05_MIDORI */
			br.BULLET_REGIST_02_angle65536				= temp00_angle;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_05_MIDORI;//							/* [�e] */
			br.BULLET_REGIST_06_n_way					= (20); 	/* [20way] */	/* ���e�� */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(20));	/* �����p�x(65536[360/360�x]�� 20 ����) */	/* 1����n���������p�x */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
		if (0x0670&(temp11_angle))
		{
			/* ���� ���@�_�� 24�����S���ʁA���F�e BULLET_MARU10_04_MIZU_IRO */
			br.BULLET_REGIST_02_angle65536				= temp00_angle;
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MARU10_04_MIZU_IRO;//							/* [�e] */
			br.BULLET_REGIST_06_n_way					= (24); 	/* [24] */	/* ���e�� */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(24));	/* �����p�x(65536[360/360�x]�� 24 ����) */	/* 1����n���������p�x */
			tama_system_regist_katayori_n_way();/* (r33-) */
		}
	}
}


/*---------------------------------------------------------
	[�p�`�F] "���������u�}�[�L�����|�C�Y���v" No.31
	-------------------------------------------------------
---------------------------------------------------------*/
static void spell_create_3e_pache_mercury_poison(SPRITE *src)
{
}


/*---------------------------------------------------------
	[�p�`�F] "�y�������u�G�������h���K���X�v" No.32
	-------------------------------------------------------
	�e�L�g�[�ɍ���Ă݂��B
	�S�R�Ⴄ�񂾂��ǁA�p�`�F���ۂ��Ƃ����΁A�p�`�F���ۂ��B
---------------------------------------------------------*/
static void spell_create_3f_pache_emerald_megalith(SPRITE *src)
{
	static int temp00_angle;
	temp00_angle += (src->boss_spell_timer<<4);

	// �����_���Θg�e
	if (0== ((src->boss_spell_timer)&(4-1) ))
	{
		if (0==((src->boss_spell_timer)&(4-1)))
		{
		//	temp00_angle = br.BULLET_REGIST_02_angle65536;
			/* �Θg�e */
			br.BULLET_REGIST_02_angle65536				= ((temp00_angle&(32768-1))-16384);
			//
		//	br.BULLET_REGIST_00_speed256				= (t256(0.75) );		/* �e�� */
		//	br.BULLET_REGIST_00_speed256				= (t256(1.50) );		/* �e�� */
			br.BULLET_REGIST_00_speed256				= (t256(0.75)+(temp00_angle&0x007f)+((cg_game_difficulty)<<4) );		/* �e�� */
		//	br.BULLET_REGIST_01_speed_offset			= (3)<<8;/*(�e�X�g)*/
			br.BULLET_REGIST_01_speed_offset			= ((temp00_angle>>4)&0x07ff);/*(�e�X�g)*/
			br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_WAKU12_05_MIDORI;//							/* [�Θg�e] */
			br.BULLET_REGIST_06_n_way					= (2);	/* [3way] */	/* ���e�� */
			br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(24)); /* �����p�x(65536[360/360�x]�� 16 ����) */ /* 1����n���������p�x */
			tama_system_regist_n_way();/* (r33-) */
		}
	}
	// �����_�����������A�Α�ʒe
	if (0== ((src->boss_spell_timer)&(16-1) ))
	{
		const u32 ra_nd32 = (ra_nd()/*&0xffff*/);
		temp00_angle += ra_nd32;
		obj_send1->cx256							= t256(16)+(ra_nd32&0xff00)+(ra_nd32&0x3f00);			/* �e��x256 */
		obj_send1->cy256							+= -t256(32)+((ra_nd32>>8)&0x3f00); 					/* �e��y256 */
		/* ���@�_�� 3way ��laser�e */
		/* ���@�_��Ȃ� 16way(?)17way(???) �ےe */
		br.BULLET_REGIST_00_speed256				= (t256(0.50) );		/* �e�� */
		br.BULLET_REGIST_01_speed_offset			= (8)<<8;/*(�e�X�g)*/
		br.BULLET_REGIST_02_angle65536				= (0);
		br.BULLET_REGIST_03_tama_data				= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_OODAMA32_03_MIDORI;//							/* [�Α�ʒe] */
		/* easy��1����,  lunatic ��3���� */
		br.BULLET_REGIST_06_n_way					= (1+((cg_game_difficulty)>>1));	/* [1way] */	/* ���e�� */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(19));	/* �����p�x(65536[360/360�x]�� 24 ����) */	/* 1����n���������p�x */
		tama_system_regist_n_way();/* (r33-) */
	}
}
