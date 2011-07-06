
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
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
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
		obj_send1->cx256 					= src->cx256;
		obj_send1->cy256 					= src->cy256;
		br.BULLET_REGIST_angle65536 		= (kakudo65536);
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/24);				/* �����p�x([360/360]�x��24����) */
		br.BULLET_REGIST_n_way				= (24);
		br.BULLET_REGIST_speed_offset		= t256(0);		/* (-5) (-3)���������e */	/* ���̕����ɂȂ邩������ */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
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
			br.BULLET_REGIST_speed256		= t256(0.6);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_03_MIDORI; 	bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(1.4);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_05_KIIRO;		bullet_regist_angle();
		OP0x02:/*not_break;*/
		OP0x04:/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(1.0);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;		bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(1.8);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;	bullet_regist_angle();
		OP0x05:/*not_break;*/
		OP0x01:/*not_break;*/
			br.BULLET_REGIST_speed256		= t256(1.2);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_02_YUKARI; 	bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(0.8);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;		bullet_regist_angle();
			br.BULLET_REGIST_speed256		= t256(2.0);	br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_01_AKA;		bullet_regist_angle();
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
		hari_no_iro = iro_tbl[(((src->base_hp)&(0xe000) )>>(13))];
	//	hari_no_iro = iro_tbl[(src->boss_base_bo ss_life)];
	}
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
		obj_send1->cx256 					= (src->cx256);
		obj_send1->cy256 					= (src->cy256);
	//	br.BULLET_REGIST_div_angle1024		= (0);				/* �_�~�[�����p�x(���g�p) */
		br.BULLET_REGIST_bullet_obj_type	= hari_no_iro;		/* [ �e] */
		br.BULLET_REGIST_n_way				= (1);				/* [1way] */
		br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
//
	int angle1024;
	int spd;
	spd = (0x180)+(ra_nd() & 0x00ff);
	for (
			angle1024 = (0);	/*cv1024r(0)*/
		//	angle1024 < /*=*/cv1024r(360/*180*/);
			angle1024 < ((4/*difficulty*/+1)<<10);	//	/*=*/ cv1024r(360/*180*/)
		//	angle512 += (/*22*/21+hari-difficulty/*cv1024r(20)*/)		/* 23�{(360) �� 8�{(180) */
			angle1024 += (/*44*/36+hari+hari-(difficulty<<2) /*cv1024r(20)*/)	/* �Œ� == 10 == 48�{(360)	*/
		)
	{
		br.BULLET_REGIST_speed256			= (int)(spd);				/* �e�� */	/*6*/	/*t256(2.5)*/
		br.BULLET_REGIST_angle1024			= angle1024;
		bullet_regist_vector();
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
			kaguya_hari_shot(src);/*enemy_boss04_fire(2);*/
		}
	}
}


/*---------------------------------------------------------
	4�ʂ̏ꍇ���˂��
---------------------------------------------------------*/

static int fire_wait3;
static int bomb_n;
static void kaguya_pong_boll(SPRITE *src)
{
	/* 4�ʂ̏ꍇ���˂�� */
	{
		/* �̗̓Q�[�W�� 3�{�ȉ��ɂȂ�����A���˂��𗎂Ƃ��n�߂�B */
		//switch ( (src->base_hp)&0xe000 )
		if (src->base_hp < (3*(8192)))
		//if (src->boss_base_bo ss_life < 2)
		{
			fire_wait3--;
			if (fire_wait3 < 0)
			{
			//	fire_wait3 = ((src->base_hp)>>(2+3));
				fire_wait3 = ((src->base_hp)>>(7)); 	/* 3*(8192)>>7 == 192 */
				//
				#define LIMIT_LOW50 (/*50*/64)
				if (fire_wait3 < (LIMIT_LOW50))
				{	fire_wait3 = (LIMIT_LOW50);}
			//
				bomb_n++;
				if (31 < bomb_n)	{	bomb_n = 31;	}
			//	1 ... 31
				br.BULLET_REGIST_regist_type				= REGIST_TYPE_01_HAZUMI;
				// 341.333333333333333333333333333333 ==  1024 / 3
				int jj;
			//	jj = (100)*(11-(int)((bomb_n*(0x100/3))>>8));
				jj = (1024*3)/(bomb_n+(2*3));
				int ii;
			//	for (ii=0; i <= (int)((bomb_n*(0x100/3))>>8); i++)
				for (ii=0; ii < (1024); ii+=jj)
				{
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
					obj_send1->cx256 							= (src->cx256);
					obj_send1->cy256 							= (src->cy256);
					br.BULLET_REGIST_speed256					= t256(0.05)+(difficulty<<6);			/* ���x t256(5.0) */
					br.BULLET_REGIST_angle1024					= (ii); 				/* �p�x */
					br.BULLET_REGIST_jyuryoku_dan_delta256		= (t256(0.07)); 						/* delta256 */	/*17.92==t256(0.07)*/
					br.BULLET_REGIST_jyuryoku_dan_bound_counts	= (difficulty); 						/* bound_counts (2) */
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
	bomb_n				= 0;
//	h->callback_mover	= move_kaguya;
	add_zako_kaguya_houmotsu(h);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/
static void danmaku_create_18_kaguya01(SPRITE *src)
{
//	if (50==((src->boss_base_danmaku_time_out) ))
	if ((64-10)==((src->boss_base_danmaku_time_out) ))
	{
		kaguya_uroko_dan_seisei(src);
	}
	kaguya_hari_dan_seisei(src);
}
static void danmaku_create_19_kaguya04(SPRITE *src)
{
//	if (50==((src->boss_base_danmaku_time_out) ))
	if ((64-10)==((src->boss_base_danmaku_time_out) ))
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
//	�P�D�P�邪���ڌ��e��16�����e��17�����e��16�����e�փW���W���ɕω�������������������������������������
	�P�D�P�邪���ڌ��e��8�����e��2�Z�b�g�B(?)
		�Z�b�g�ʂɉ�]�������Ⴄ(?)�B
	�Q�D�|��ɂȂ�񂾃I�v�V��������n�߂����S����(48����)�e���ˁB
	�R�D�|��ɂȂ�񂾃I�v�V���������ʍ��E�[�ɔ��]�F�Ēe���˂��A
		��ʍ��E�[�Œʏ�F�Ēe�̃v���C���[�_���e�ɕς��B
---------------------------------------------------------*/
// 4096 == 65536/16
// 3855.05882352941176470588235294118 == 65536/17
//	240.941176470588235294117647058824 == ((65536/16)-(16636/17)) ����
static void danmaku_create_23_kaguya_tamanoe(SPRITE *src)
{
//	if ((0x02)==((src->boss_base_danmaku_time_out)&0x03))/* (2���1��)(8�񖈂ɔ��e) */
	if ((0x04)==((src->boss_base_danmaku_time_out)&0x07))/* (2���1��)(8�񖈂ɔ��e) */
	{
		static unsigned int bbb;
		bbb += 77;
		static unsigned int aaa;
		aaa++;
		aaa &= (0x07);
		if (0==aaa) 	{	aaa++;	};
		//
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
		obj_send1->cx256 					= (src->cx256);				/* �e��x256 */
		obj_send1->cy256 					= (src->cy256);				/* �e��y256 */
	//	obj_send1->cx256 					= (src->cx256)+t256(25.0);				/* �e��x256 */
	//	obj_send1->cy256 					= (src->cy256)+t256(16.0);				/* �e��y256 */
		//
//		br.BULLET_REGIST_speed256			= (t256(1.00)); 										/* �e�� */
		br.BULLET_REGIST_speed256			= (t256(0.50)); 										/* �e�� */
	//	br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16))-(((sin1024(( ra_nd() & src->boss_base_danmaku_time_out)))&0xff));		/*(int)(1024/(6))*/ 	/* �����p�x(1024[360/360�x]�� 6 ����) */	/* 1����n���������p�x */
		br.BULLET_REGIST_div_angle65536 	= (int)(65536/(16/*8*/));	/* �����p�x(65536[360/360�x]�� 8 ����) */	/* 1����n���������p�x */
//		br.BULLET_REGIST_angle65536 		= ((65536/2));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_bullet_obj_type	= (BULLET_MARU10_01_AKA-1)+aaa; 								/* [ �e] */
	//	br.BULLET_REGIST_n_way				= (17); 			/*(6)*/ 				/* [1way] */	/* ���e�� */
		br.BULLET_REGIST_n_way				= (16/*8*/);			/*(6)*/ 				/* [1way] */	/* ���e�� */
//		br.BULLET_REGIST_speed_offset		= t256(0);/*�Ă���*/
		br.BULLET_REGIST_speed_offset		= t256(1);/*�Ă���*/
	//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
//
//		br.BULLET_REGIST_angle65536 		= (int)((65536*1/4))+(src->boss_base_danmaku_time_out&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		br.BULLET_REGIST_angle65536 		= ((bbb));				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		if (0==((src->boss_base_danmaku_time_out)&0x08))
		{
//			br.BULLET_REGIST_angle65536 		= (int)((65536*3/4))+(src->boss_base_danmaku_time_out&0xff);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_angle65536 		= -(bbb);				/* ���˒��S�p�x / ����@�\(���@�_��/��) */
		}
		bullet_regist_angle();
	}
}
