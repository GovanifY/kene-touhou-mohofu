
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�P��̃J�[�h���`���܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�P��
	-------------------------------------------------------
	�������ǉ�
---------------------------------------------------------*/

/*---------------------------------------------------------
	4�ʐ�p
---------------------------------------------------------*/

local OBJ_CALL_FUNC(kaguya_uroko_dan_seisei)
{
	/* 4�ʂ̏ꍇ---- */
	const u16 my_ra_nd = ra_nd();
	if (my_ra_nd & 0x8000)		/* 50%�̊m���� */
	{
		s32 kakudo65536;
	//	kakudo65536 = ((255-32)<<6);
	//	kakudo65536 = ((256-32)<<6);
	//	kakudo65536 = (((1024/4)-(1024/32))<<6);
		kakudo65536 = ((65536/4)-(65536/32));
		kakudo65536 += ((my_ra_nd)&((/*64<<6*/4096)-1));
		HATSUDAN_02_speed_offset			= t256(0);			/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
		HATSUDAN_03_angle65536				= (kakudo65536);
		HATSUDAN_04_tama_spec				= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_06_n_way					= (24);
		HATSUDAN_07_div_angle65536			= (int)(65536/24);				/* �����p�x([360/360]�x��24����) */
//
	static const void *JumpTable[(8)] =
	{
		&&OP0x00, &&OP0x01, &&OP0x02, &&OP0x03,
		&&OP0x04, &&OP0x05, &&OP0x06, &&OP0x07,
	};
		goto *JumpTable[((src->base_hp>>13)&(0x07))];
		{
		OP0x03:/*not_break;*/
			HATSUDAN_01_speed256	= t256(0.6);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_05_MIDORI);	hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(1.4);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_06_KI_IRO);	hatudan_system_regist_katayori_n_way();
		OP0x02:/*not_break;*/
		OP0x04:/*not_break;*/
			HATSUDAN_01_speed256	= t256(1.0);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_03_AOI);		hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(1.8);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_04_MIZU_IRO); hatudan_system_regist_katayori_n_way();
		OP0x05:/*not_break;*/
		OP0x01:/*not_break;*/
			HATSUDAN_01_speed256	= t256(1.2);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_02_YUKARI);	hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(0.8);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);		hatudan_system_regist_katayori_n_way();
			HATSUDAN_01_speed256	= t256(2.0);	HATSUDAN_05_bullet_obj_type = (BULLET_UROKO14_BASE + TAMA_IRO_01_AKA);		hatudan_system_regist_katayori_n_way();
		OP0x00:
		OP0x06:
		OP0x07:
			;
		}
	}
}


/*---------------------------------------------------------
	���E�V���b�g
---------------------------------------------------------*/

local OBJ_CALL_FUNC(kaguya_hari_shot)
{
	static int hari=0;
	hari--;
	hari &= 32;
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	{	/* 4�ʂ̏ꍇ---- */
		unsigned short iro_tbl[8] =
		{
/**/		BULLET_HARI32_01_AKA,
/*1*/		BULLET_HARI32_01_AKA,
/*2*/		BULLET_HARI32_03_DAIDAI,
/*3*/		BULLET_HARI32_02_KIIRO,
/*4*/		BULLET_HARI32_00_AOI,
/*5*/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
/**/		BULLET_HARI32_00_AOI,
		};
	//	hari_no_iro = iro_tbl[(src->boss_base_bo ss_life)];
	//	hari_no_iro = iro_tbl[(((src->base_hp)&(0x0000e000) )>>(13))];/*(r32)*/
		hari_no_iro = iro_tbl[(((src->base_hp)&(0x0001c000) )>>(14))];/*(r33)*/
	//											0x0001c000 == 114688 == 3.5*32*1024
	}
		HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= hari_no_iro;		/* [ �e] */
	//
	int jj;
	int spd;/*(�e��)*/
	spd = t256(1.5) + /*(t256(0.0 ... 0.99)�̗���)*/(ra_nd() & 0x00ff);
	for (
			jj = (0);
			jj < ((4/*difficulty*/+1)<<16);
		//	angle512 += (/*22*/21+hari-difficulty/*cv1024r(20)*/)		/* 23�{(360) �� 8�{(180) */
			jj += ((/*44*/36<<6)+((hari)<<7)-((REG_0f_GAME_DIFFICULTY)<<(8)) /*cv1024r(20)*/) /* �Œ� == 10 == 48�{(360)	*/
		)
	{
		HATSUDAN_01_speed256			= (int)(spd);				/* �e�� */	/*6*/	/*t256(2.5)*/
		HATSUDAN_03_angle65536			= (jj);
		hatudan_system_regist_single();
		spd += (64>>3/*difficulty*/);
	}
	//
	AUDIO_18_voice_number	= VOICE11_BOSS_KIRARIN;
	AUDIO_19_voice_truck	= TRACK04_TEKIDAN;/* �e�L�g�[ */
	cpu_voice_play();
}

/*---------------------------------------------------------
	���E�V���b�g
---------------------------------------------------------*/

local OBJ_CALL_FUNC(kaguya_hari_dan_seisei)
{
//	if (CARD_ADDRESS_00_kaguya_000 < sp ell_card_number)
	{
		static int fire_wait1;
		fire_wait1--;
		if (0 > fire_wait1)
		{
			fire_wait1 = (45*4);
			kaguya_hari_shot(src);
		}
	}
}


/*---------------------------------------------------------
	4�ʂ̏ꍇ���˂��
---------------------------------------------------------*/

static int fire_wait3;
static int bomb_aaa;
local OBJ_CALL_FUNC(kaguya_pong_boll)
{
	/* 4�ʂ̏ꍇ���˂�� */
	{
		/* �̗̓Q�[�W�� 3�{�ȉ��ɂȂ�����A���˂��𗎂Ƃ��n�߂�B */
		//sw itch ( (src->base_hp)&0xe000 )
		//if (src->boss_base_bo ss_life < 2)
	//	if (src->base_hp < (3*(8192)))/*(r32)*/
		if (src->base_hp < (3*(1024*32)))/*(r33)*/
		{
			fire_wait3--;
			if (fire_wait3 < 0)
			{
			//	fire_wait3 = ((src->base_hp)>>(2+3));
			//	fire_wait3 = ((src->base_hp)>>(7)); 	/* 3*(8192)>>7 == 192 */
				fire_wait3 = ((src->base_hp)>>(7+2));	/* 3*(32768)>>9 == 192 */
				//
				#define LIMIT_LOW50 (/*50*/64)
				if (fire_wait3 < (LIMIT_LOW50))
				{	fire_wait3 = (LIMIT_LOW50);}
			//	1 ... 40
					#if 0	/*�Â�*/
					HATSUDAN_01_speed256					= t256(0.05)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* ���x t256(5.0) */
					HATSUDAN_05_bullet_obj_type 			= BULLET_MINI8_BASE + ( TAMA_IRO_04_MIZU_IRO +(REG_0f_GAME_DIFFICULTY));
				//	HATSUDAN_06_n_way						= (8);
					HATSUDAN_07_VECTOR_leg acy_dan_delta256 = (t256(0.07)); 								/*17.92==t256(0.07)*/
			//	bomb_aaa += 9;
			//	if (448 < bomb_aaa) 	{	bomb_aaa = 448; 	}	/* 512-448 == 64 == 1024/16...���� / �ő�16�����̏ꍇ�B */
			//	if (433 < bomb_aaa) 	{	bomb_aaa = 433; 	}	/* 512-433 == 79 == 1024/13...���� / �ő�13�����̏ꍇ�B */
			//	if (427 < bomb_aaa) 	{	bomb_aaa = 427; 	}	/* 512-427 == 85 == 1024/12...���� / �ő�12�����̏ꍇ�B */
			//	if (419 < bomb_aaa) 	{	bomb_aaa = 419; 	}	/* 512-419 == 93 == 1024/11...���� / �ő�11�����̏ꍇ�B */
				int ii;
			//	for (ii=0; ii<(1024); ii+=(512-bomb_aaa))	/* ���X�ő�16�������炢�o�Ȃ��Ƃ������Ĕ�����Ȃ��B */
				{
					HATSUDAN_03_angle65536				= ((ii)<<6);	/* �p�x */
					bullet_regist_leg acy_vector_direct();
				}
					#else	/*�V����*/
					HATSUDAN_01_speed256					= t256(0.05)+((REG_0f_GAME_DIFFICULTY)<<6); 	/* ���x t256(5.0) */
					HATSUDAN_02_speed_offset				= (t256(0.07)); 								/*17.92==t256(0.07)*/
					HATSUDAN_04_tama_spec					= (DANMAKU_LAYER_01)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
					HATSUDAN_05_bullet_obj_type 			= BULLET_MINI8_BASE + ( TAMA_IRO_04_MIZU_IRO +(REG_0f_GAME_DIFFICULTY));
				bomb_aaa += (9*64);
				if ((419*64) < bomb_aaa)	{	bomb_aaa = (419*64);	}	/* 512-419 == 93 == 1024/11...���� / �ő�11�����̏ꍇ�B */
				int ii;
				for (ii=0; ii<(65536); ii+=((65536/2)-bomb_aaa))	/* 16������葝�₷�ƁA�������Ĕ�����̂�����Ȃ肷����̂ōő�ł�16�����B */
				{
					HATSUDAN_03_angle65536				= (ii); 		/* �p�x */
					hatudan_system_regist_single();
				}
					#endif
			}
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
global OBJ_CALL_FUNC(boss_init_kaguya)
{
	init_set_dolls_kaguya_T01(src);/*(�g�����V�X�e��)*/
	fire_wait3			= 0;
	bomb_aaa			= 0;
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
local OBJ_CALL_FUNC(spell_create_18_kaguya01)
{
//	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	if ((64-10)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
}
local OBJ_CALL_FUNC(spell_create_19_kaguya04)
{
//	if (50==((REG_10_BOSS_SPELL_TIMER) ))
	if ((64-10)==((REG_10_BOSS_SPELL_TIMER) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
	kaguya_pong_boll(src);
}


/*---------------------------------------------------------
	�P��A�H���̋ʂ̎}���ǂ�
	�_��u�H���̋ʂ̎} - ���F�̋� -�v
	-------------------------------------------------------
	���񂺂����ĂȂ�...
//	�P�D�P�邪���ڌ��e��16�����e��17�����e��16�����e�փW���W���ɕω�����B
	�P�D�P�邪���ڌ��e��8�����e��2�Z�b�g�B(?)
		�Z�b�g�ʂɉ�]�������Ⴄ(?)�B
	�Q�D�|��ɂȂ�񂾃I�v�V��������n�߂����S����(48����)�e���ˁB
	�R�D�|��ɂȂ�񂾃I�v�V���������ʍ��E�[�ɔ��]�F�Ēe���˂��A
		��ʍ��E�[�Œʏ�F�Ēe�̃v���C���[�_���e�ɕς��B
	-------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
	�g�p���W�X�^
	REG_08_REG0 	��̐F(1 ... 7)�B	//	static u32 REG_09_REG1;
	REG_09_REG1 	�p�x?				//	static u32 REG_08_REG0;
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) ����
local OBJ_CALL_FUNC(spell_create_23_kaguya_tamanoe)
{
//	if ((0x02)==((REG_10_BOSS_SPELL_TIMER)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x04)==((REG_10_BOSS_SPELL_TIMER)&0x07))/* (2���1��)(8�񖈂ɔ��e) */
	{
	#if (1)//[-r39]�ϐ�������ƃX�y�J�؂�ւ����Ƀ��Z�b�g�����B
		static u32 kaguya_REG_09_REG1;
		static u32 kaguya_REG_08_REG0;
	#else//[r40]�ϐ�������ƃX�y�J�؂�ւ����Ƀ��Z�b�g�����B
		#define kaguya_REG_09_REG1 REG_09_REG1
		#define kaguya_REG_08_REG0 REG_08_REG0
	#endif
		kaguya_REG_08_REG0++;
		kaguya_REG_08_REG0 &= (0x07);
		if (0==kaguya_REG_08_REG0)	{	kaguya_REG_08_REG0++;	};
		//
//		HATSUDAN_01_speed256			= (t256(1.00)); 			/* �e�� */
		HATSUDAN_01_speed256			= (t256(0.50)); 			/* �e�� */
//		HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
		HATSUDAN_02_speed_offset		= t256(1);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_8000_NON_TILT);/* (r33-)��X���e */
		HATSUDAN_05_bullet_obj_type 	= (BULLET_MARU10_BASE) + kaguya_REG_08_REG0;	/* [���ےe] */
	//	HATSUDAN_06_n_way				= (17); 						/* [17way] */	/* ���e�� */
		HATSUDAN_06_n_way				= (16/*8*/);					/* [16way] */	/* ���e�� */
	//	HATSUDAN_07_div_angle65536		= (int)(65536/(16))-(((si n1024(( ra_nd() & REG_10_BOSS_SPELL_TIMER)))&0xff));	/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		HATSUDAN_07_div_angle65536		= (int)(65536/(16/*8*/));		/* 1����16���������p�x */
//
//		HATSUDAN_03_angle65536			= ((65536/2));					/* ���˒��S�p�x / ����@�\(���@�_��/��) */
//		HATSUDAN_03_angle65536			= (int)((65536*1/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		kaguya_REG_09_REG1 += (77);
		HATSUDAN_03_angle65536			= ((kaguya_REG_09_REG1));						/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		if (0==((REG_10_BOSS_SPELL_TIMER)&0x08))
		{
//			HATSUDAN_03_angle65536		= (int)((65536*3/4))+(REG_10_BOSS_SPELL_TIMER&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			HATSUDAN_03_angle65536		= -(kaguya_REG_09_REG1);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		}
		hatudan_system_regist_katayori_n_way();/* (r33-) */
	}
	/*(r40�Ă���)*/
//	if ((64-10)==((REG_10_BOSS_SPELL_TIMER) ))
//	{
//		kaguya_hari_dan_seisei(src);
//	}
}



/*---------------------------------------------------------
	�H���̋ʂ̎}���ǂ�
	-------------------------------------------------------
	���������͕핗�ŋP�邪�o��̂�����A
	�H���̋ʂ̎}���ǂ�������肽���C������B
	���[�U�[���v��Ȃ����B
	�ȉ������B
//
	�Ԏ����Ή���̏���7�F�J��Ԃ��ŏo�Ă���B��ʒ����㕔�̋P�邩��͑S���ʌ��\������葬�x�ےe�B
	�قړ������������ɉ������Ă�C������B(�C�̂�������)
	�P��͏㉺�ɗh��Ă��邪�A�ˏo�ʒu�͗h��Ȃ��ŌŒ�B
	�S���ʒe(�J������16�����S���ʒe)������������(�����_�����ۂ��p�x��)�e����������B
	(�܂蕪���p�x������)
	17�����S���ʒe���A������ƍ��x��16�����S���ʒe�܂Ō���(����)�B����̌J��Ԃ��B
//
	��ʒ�������(�Ƃ������قډ�ʒ��S)�ɖ����w���������d�Ȃ���7�B���z�u�B
	�J���������A�S���ʕĒe�����Ă���B
	�����ɖ����w����͍��E�ɔ��]�F�A�Ēe�B�p�����w����͓����ɒe���o��B
	�e���͔�r�I������肾���A�P��ےe��葬���B
	��������قړ������������ɉ������Ă�C������B(�C�̂�������)
	�F�̏����͍����͏�Ɠ����B�E���͋t���B
	�T�C�����ۂ������ł��˂��Ďˏo�B7�F���Ƃ��΂��x�ށB
	�E��angle=90[�x]+(sin(����++)*���p�x�萔)�B
	����angle=270[�x]+(sin(����++)*���p�x�萔)�B�݂����ɂ���΂����̂����H
	(���p�x�萔��+-11.25�x(360/(2^5))���炢�H�Ȃ�>>4�Ƃ��ł������ȁH)
	���]�F�A�Ēe�́A���E�̕ǂɂ�����ƐF�����F�Ŕ��]��(�Ⴆ�ΐԔ��]�F�Ēe���ԕĒe�ɂȂ�Ƃ����Ӗ�)�A
	���@�_���e�ɂȂ�B
	-------------------------------------------------------
	��̂���Ȋ������Ǝv�����ǁA�Ԉ���Ă邩������Ȃ��B
	�ȏ�𖞂����΁A���Ȃ�ו��̓���������Ă��A���������ɂȂ�Ȃ����Ȃ��H
---------------------------------------------------------*/
