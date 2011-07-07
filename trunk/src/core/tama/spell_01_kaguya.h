
/*---------------------------------------------------------
 �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�P��̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�P��
	-------------------------------------------------------
	�������ǉ�
---------------------------------------------------------*/

/*---------------------------------------------------------
	4�ʐ�p
---------------------------------------------------------*/

static void kaguya_uroko_dan_seisei(SPRITE *src)
{
	/* 4�ʂ̏ꍇ---- */
	const Uint16 my_ra_nd = ra_nd();
	if (my_ra_nd & 0x8000)		/* 50%�̊m���� */
	{
		int kakudo65536;
	//	kakudo65536 = ((255-32)<<6);
	//	kakudo65536 = ((256-32)<<6);
	//	kakudo65536 = (((1024/4)-(1024/32))<<6);
		kakudo65536 = ((65536/4)-(65536/32));
		kakudo65536 += ((my_ra_nd)&((/*64<<6*/4096)-1));
//
		obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		br.BULLET_REGIST_01_speed_offset			= t256(0);			/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
		br.BULLET_REGIST_02_angle65536				= (kakudo65536);
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
		br.BULLET_REGIST_06_n_way					= (24);
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/24);				/* �����p�x([360/360]�x��24����) */
//
	static const void *JumpTable[(8)] =
	{
		&&OP0x00, &&OP0x01, &&OP0x02, &&OP0x03,
		&&OP0x04, &&OP0x05, &&OP0x06, &&OP0x07,
	};
	//	goto ((src->base_hp)&(0xe000) )
		goto *JumpTable[((src->base_hp>>13)&(0x07))];
	//	switch (src->boss_base_bo ss_life)
		{
		OP0x03:/*not_break;*/
			br.BULLET_REGIST_00_speed256		= t256(0.6);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_05_MIDORI; 	tama_system_regist_katayori_n_way();
			br.BULLET_REGIST_00_speed256		= t256(1.4);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_06_KI_IRO; 	tama_system_regist_katayori_n_way();
		OP0x02:/*not_break;*/
		OP0x04:/*not_break;*/
			br.BULLET_REGIST_00_speed256		= t256(1.0);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_03_AOI;		tama_system_regist_katayori_n_way();
			br.BULLET_REGIST_00_speed256		= t256(1.8);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_04_MIZUIRO;	tama_system_regist_katayori_n_way();
		OP0x05:/*not_break;*/
		OP0x01:/*not_break;*/
			br.BULLET_REGIST_00_speed256		= t256(1.2);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_02_YUKARI; 	tama_system_regist_katayori_n_way();
			br.BULLET_REGIST_00_speed256		= t256(0.8);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_01_AKA;		tama_system_regist_katayori_n_way();
			br.BULLET_REGIST_00_speed256		= t256(2.0);	br.BULLET_REGIST_04_bullet_obj_type = BULLET_UROKO14_01_AKA;		tama_system_regist_katayori_n_way();
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

static void kaguya_hari_shot(SPRITE *src)
{
	static int hari=0;
	hari--;
	hari &= 32;
	voice_play(VOICE11_BOSS_KIRARIN/*VOICE14_BOSS_KOUGEKI_01*/, TRACK04_TEKIDAN);/*�e�L�g�[*/
//
	int hari_no_iro;
	hari_no_iro = BULLET_HARI32_00_AOI;
	/* 4�ʂ̏ꍇ---- */
	{
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
		obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= hari_no_iro;		/* [ �e] */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
//
	int jj;
	int spd;
	spd = (0x180)+(ra_nd() & 0x00ff);
	for (
			jj = (0);
			jj < ((4/*difficulty*/+1)<<16);
		//	angle512 += (/*22*/21+hari-difficulty/*cv1024r(20)*/)		/* 23�{(360) �� 8�{(180) */
			jj += ((/*44*/36<<6)+((hari)<<7)-((cg_game_difficulty)<<(8)) /*cv1024r(20)*/) /* �Œ� == 10 == 48�{(360)	*/
		)
	{
		br.BULLET_REGIST_00_speed256			= (int)(spd);				/* �e�� */	/*6*/	/*t256(2.5)*/
		br.BULLET_REGIST_02_angle65536			= (jj);
		tama_system_regist_single();
		spd += (64>>3/*difficulty*/);
	}
}

/*---------------------------------------------------------
	���E�V���b�g
---------------------------------------------------------*/

static void kaguya_hari_dan_seisei(SPRITE *src)
{
//	if (SPELL_CARD_00_kaguya_000 < sp ell_card_number)
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
//static int bomb_n;
			//
			//	bomb_n++;
			//	if (31 < bomb_n)		{	bomb_n = 31;		}
			//	1 ... 31
				// 341.333333333333333333333333333333 ==  1024 / 3
			//	int jj;
			//	jj = (100)*(11-(int)((bomb_n*(0x100/3))>>8));
			//	jj = (1024*3)/(bomb_n+(2*3));
			//	for (ii=0; i <= (int)((bomb_n*(0x100/3))>>8); i++)
//	bomb_n				= 0;
//	h->callback_mover	= move_kaguya;

static int fire_wait3;
static int bomb_aaa;
static void kaguya_pong_boll(SPRITE *src)
{
	/* 4�ʂ̏ꍇ���˂�� */
	{
		/* �̗̓Q�[�W�� 3�{�ȉ��ɂȂ�����A���˂��𗎂Ƃ��n�߂�B */
		//switch ( (src->base_hp)&0xe000 )
		//if (src->boss_base_bo ss_life < 2)
	//	if (src->base_hp < (3*(8192)))/*(r32)*/
		if (src->base_hp < (3*(1024*32)))/*(r33)*/
		{
			fire_wait3--;
			if (fire_wait3 < 0)
			{
			//	fire_wait3 = ((src->base_hp)>>(2+3));
			//	fire_wait3 = ((src->base_hp)>>(7)); 	/* 3*(8192)>>7 == 192 */
				fire_wait3 = ((src->base_hp)>>(7+2)); 	/* 3*(32768)>>9 == 192 */
				//
				#define LIMIT_LOW50 (/*50*/64)
				if (fire_wait3 < (LIMIT_LOW50))
				{	fire_wait3 = (LIMIT_LOW50);}
			//	1 ... 40
					obj_send1->cx256							= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
					obj_send1->cy256							= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
					br.BULLET_REGIST_00_speed256							= t256(0.05)+((cg_game_difficulty)<<6); 		/* ���x t256(5.0) */
					br.BULLET_REGIST_05_regist_type 						= REGIST_TYPE_01_HAZUMI;
					br.BULLET_REGIST_06_VECTOR_jyuryoku_dan_bound_counts	= ((cg_game_difficulty));						/* bound_counts (2) */
					br.BULLET_REGIST_07_VECTOR_jyuryoku_dan_delta256		= (t256(0.07)); 						/* delta256 */	/*17.92==t256(0.07)*/
				bomb_aaa += 9;
			//	if (448 < bomb_aaa) 	{	bomb_aaa = 448; 	}	/* 512-448 == 64 == 1024/16...���� / �ő�16�����̏ꍇ�B */
			//	if (433 < bomb_aaa) 	{	bomb_aaa = 433; 	}	/* 512-433 == 79 == 1024/13...���� / �ő�13�����̏ꍇ�B */
			//	if (427 < bomb_aaa) 	{	bomb_aaa = 427; 	}	/* 512-427 == 85 == 1024/12...���� / �ő�12�����̏ꍇ�B */
				if (419 < bomb_aaa) 	{	bomb_aaa = 419; 	}	/* 512-419 == 93 == 1024/11...���� / �ő�11�����̏ꍇ�B */
				int ii;
				for (ii=0; ii<(1024); ii+=(512-bomb_aaa))	/* ���X�ő�16�������炢�o�Ȃ��Ƃ������Ĕ�����Ȃ��B */
				{
					br.BULLET_REGIST_02_VECTOR_angle1024					= (ii); 				/* �p�x */
					bullet_regist_vector();
				}
			}
		}
	}
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
extern void add_zako_kaguya_houmotsu(SPRITE *src);
global void boss_init_kaguya(SPRITE *h)
{
	fire_wait3			= 0;
	bomb_aaa			= 0;
	add_zako_kaguya_houmotsu(h);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
static void spell_create_18_kaguya01(SPRITE *src)
{
//	if (50==((src->boss_base_spell_time_out) ))
	if ((64-10)==((src->boss_base_spell_time_out) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
}
static void spell_create_19_kaguya04(SPRITE *src)
{
//	if (50==((src->boss_base_spell_time_out) ))
	if ((64-10)==((src->boss_base_spell_time_out) ))
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
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) ����
static void spell_create_23_kaguya_tamanoe(SPRITE *src)
{
//	if ((0x02)==((src->boss_base_spell_time_out)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x04)==((src->boss_base_spell_time_out)&0x07))/* (2���1��)(8�񖈂ɔ��e) */
	{
		obj_send1->cx256						= (src->cx256); 	/* �e��x256 �{�X���S���甭�e�B */
		obj_send1->cy256						= (src->cy256); 	/* �e��y256 �{�X���S���甭�e�B */
		//
		static unsigned int bbb;
		bbb += 77;
		static unsigned int aaa;
		aaa++;
		aaa &= (0x07);
		if (0==aaa) 	{	aaa++;	};
		//
//		br.BULLET_REGIST_00_speed256				= (t256(1.00)); 										/* �e�� */
		br.BULLET_REGIST_00_speed256				= (t256(0.50)); 										/* �e�� */
//		br.BULLET_REGIST_01_speed_offset			= t256(0);/*(�e�X�g)*/
		br.BULLET_REGIST_01_speed_offset			= t256(1);/*(�e�X�g)*/
	//	br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_0000_TILT);/* (r33-)�W���e */
		br.BULLET_REGIST_03_tama_data   			= (TAMA_DATA_8000_NON_TILT);/* (r33-)��X���e */
		br.BULLET_REGIST_04_bullet_obj_type 		= (BULLET_MARU10_01_AKA-1)+aaa; 								/* [ �e] */
	//����br.BULLET_REGIST_05_regist_type 			= TAMA_TYPE_00_ANGLE_TILT;/* (r33-)�W���e */
	//	br.BULLET_REGIST_06_n_way					= (17); 				/*(6)*/ 				/* [17way] */	/* ���e�� */
		br.BULLET_REGIST_06_n_way					= (16/*8*/);			/*(6)*/ 				/* [16way] */	/* ���e�� */
	//	br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16))-(((sin1024(( ra_nd() & src->boss_base_spell_time_out)))&0xff)); 	/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_07_div_angle65536			= (int)(65536/(16/*8*/));	/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
//
//		br.BULLET_REGIST_02_angle65536				= ((65536/2));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
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
